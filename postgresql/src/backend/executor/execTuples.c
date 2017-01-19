/*-------------------------------------------------------------------------
 *
 * execTuples.c
 *	  Routines dealing with the executor tuple tables.	These are used to
 *	  ensure that the executor frees copies of tuples (made by
 *	  ExecTargetList) properly.
 *
 *	  Routines dealing with the type information for tuples. Currently,
 *	  the type information for a tuple is an array of FormData_pg_attribute.
 *	  This information is needed by routines manipulating tuples
 *	  (getattribute, formtuple, etc.).
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/executor/execTuples.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
/*
 * INTERFACE ROUTINES
 *
 *	 TABLE CREATE/DELETE
 *		ExecCreateTupleTable	- create a new tuple table
 *		ExecDropTupleTable		- destroy a table
 *
 *	 SLOT RESERVATION
 *		ExecAllocTableSlot		- find an available slot in the table
 *
 *	 SLOT ACCESSORS
 *		ExecStoreTuple			- store a tuple in the table
 *		ExecFetchTuple			- fetch a tuple from the table
 *		ExecClearTuple			- clear contents of a table slot
 *		ExecSetSlotDescriptor	- set a slot's tuple descriptor
 *		ExecSetSlotDescriptorIsNew - diddle the slot-desc-is-new flag
 *
 *	 SLOT STATUS PREDICATES
 *		TupIsNull				- true when slot contains no tuple(Macro)
 *
 *	 CONVENIENCE INITIALIZATION ROUTINES
 *		ExecInitResultTupleSlot    \	convenience routines to initialize
 *		ExecInitScanTupleSlot		\	the various tuple slots for nodes
 *		ExecInitExtraTupleSlot		/	which store copies of tuples.
 *		ExecInitNullTupleSlot	   /
 *
 *	 Routines that probably belong somewhere else:
 *		ExecTypeFromTL			- form a TupleDesc from a target list
 *
 *	 EXAMPLE OF HOW TABLE ROUTINES WORK
 *		Suppose we have a query such as retrieve (EMP.name) and we have
 *		a single SeqScan node in the query plan.
 *
 *		At ExecStart()
 *		----------------
 *		- InitPlan() calls ExecCreateTupleTable() to create the tuple
 *		  table which will hold tuples processed by the executor.
 *
 *		- ExecInitSeqScan() calls ExecInitScanTupleSlot() and
 *		  ExecInitResultTupleSlot() to reserve places in the tuple
 *		  table for the tuples returned by the access methods and the
 *		  tuples resulting from preforming target list projections.
 *
 *		During ExecRun()
 *		----------------
 *		- SeqNext() calls ExecStoreTuple() to place the tuple returned
 *		  by the access methods into the scan tuple slot.
 *
 *		- ExecSeqScan() calls ExecStoreTuple() to take the result
 *		  tuple from ExecProject() and place it into the result tuple slot.
 *
 *		- ExecutePlan() calls ExecRetrieve() which gets the tuple out of
 *		  the slot passed to it by calling ExecFetchTuple().  this tuple
 *		  is then returned.
 *
 *		At ExecEnd()
 *		----------------
 *		- EndPlan() calls ExecDropTupleTable() to clean up any remaining
 *		  tuples left over from executing the query.
 *
 *		The important thing to watch in the executor code is how pointers
 *		to the slots containing tuples are passed instead of the tuples
 *		themselves.  This facilitates the communication of related information
 *		(such as whether or not a tuple should be pfreed, what buffer contains
 *		this tuple, the tuple's tuple descriptor, etc).   Note that much of
 *		this information is also kept in the ExprContext of each node.
 *		Soon the executor will be redesigned and ExprContext's will contain
 *		only slot pointers.  -cim 3/14/91
 *
 *	 NOTES
 *		The tuple table stuff is relatively new, put here to alleviate
 *		the process growth problems in the executor.  The other routines
 *		are old (from the original lisp system) and may someday become
 *		obsolete.  -cim 6/23/90
 *
 *		In the implementation of nested-dot queries such as
 *		"retrieve (EMP.hobbies.all)", a single scan may return tuples
 *		of many types, so now we return pointers to tuple descriptors
 *		along with tuples returned via the tuple table.  This means
 *		we now have a bunch of routines to diddle the slot descriptors
 *		too.  -cim 1/18/90
 *
 *		The tuple table stuff depends on the executor/tuptable.h macros,
 *		and the TupleTableSlot node in execnodes.h.
 *
 */
#include "postgres.h"

#include "access/heapam.h"
#include "catalog/pg_type.h"
#include "executor/executor.h"


/* ----------------------------------------------------------------
 *				  tuple table create/delete functions
 * ----------------------------------------------------------------
 */
/* --------------------------------
 *		ExecCreateTupleTable
 *
 *		This creates a new tuple table of the specified initial
 *		size.  If the size is insufficient, ExecAllocTableSlot()
 *		will grow the table as necessary.
 *
 *		This should be used by InitPlan() to allocate the table.
 *		The table's address will be stored in the EState structure.
 * --------------------------------
 */
TupleTable						/* return: address of table */
ExecCreateTupleTable(int initialSize)	/* initial number of slots in
										 * table */
{
	TupleTable	newtable;		/* newly allocated table */
	TupleTableSlot *array;		/* newly allocated slot array */

	/*
	 * sanity checks
	 */
	Assert(initialSize >= 1);

	/*
	 * Now allocate our new table along with space for the pointers to the
	 * tuples.
	 */

	newtable = (TupleTable) palloc(sizeof(TupleTableData));
	array = (TupleTableSlot *) palloc(initialSize * sizeof(TupleTableSlot));

	/*
	 * clean out the slots we just allocated
	 */
	MemSet(array, 0, initialSize * sizeof(TupleTableSlot));

	/*
	 * initialize the new table and return it to the caller.
	 */
	newtable->size = initialSize;
	newtable->next = 0;
	newtable->array = array;

	return newtable;
}

/* --------------------------------
 *		ExecDropTupleTable
 *
 *		This frees the storage used by the tuple table itself
 *		and optionally frees the contents of the table also.
 *		It is expected that this routine be called by EndPlan().
 * --------------------------------
 */
void
ExecDropTupleTable(TupleTable table,	/* tuple table */
				   bool shouldFree)		/* true if we should free slot
										 * contents */
{
	int			next;			/* next available slot */
	TupleTableSlot *array;		/* start of table array */
	int			i;				/* counter */

	/*
	 * sanity checks
	 */
	Assert(table != NULL);

	/*
	 * get information from the table
	 */
	array = table->array;
	next = table->next;

	/*
	 * first free all the valid pointers in the tuple array and drop
	 * refcounts of any referenced buffers, if that's what the caller
	 * wants.  (There is probably no good reason for the caller ever not
	 * to want it!)
	 */
	if (shouldFree)
	{
		for (i = 0; i < next; i++)
		{
			ExecClearTuple(&array[i]);
			if (array[i].ttc_shouldFreeDesc &&
				array[i].ttc_tupleDescriptor != NULL)
				FreeTupleDesc(array[i].ttc_tupleDescriptor);
		}
	}

	/*
	 * finally free the tuple array and the table itself.
	 */
	pfree(array);
	pfree(table);
}


/* ----------------------------------------------------------------
 *				  tuple table slot reservation functions
 * ----------------------------------------------------------------
 */
/* --------------------------------
 *		ExecAllocTableSlot
 *
 *		This routine is used to reserve slots in the table for
 *		use by the various plan nodes.	It is expected to be
 *		called by the node init routines (ex: ExecInitNestLoop)
 *		once per slot needed by the node.  Not all nodes need
 *		slots (some just pass tuples around).
 * --------------------------------
 */
TupleTableSlot *
ExecAllocTableSlot(TupleTable table)
{
	int			slotnum;		/* new slot number */
	TupleTableSlot *slot;

	/*
	 * sanity checks
	 */
	Assert(table != NULL);

	/*
	 * if our table is full we have to allocate a larger size table. Since
	 * ExecAllocTableSlot() is only called before the table is ever used
	 * to store tuples, we don't have to worry about the contents of the
	 * old table. If this changes, then we will have to preserve the
	 * contents. -cim 6/23/90
	 *
	 * Unfortunately, we *cannot* do this.	All of the nodes in the plan that
	 * have already initialized their slots will have pointers into
	 * _freed_ memory.	This leads to bad ends.  We now count the number
	 * of slots we will need and create all the slots we will need ahead
	 * of time.  The if below should never happen now.	Fail if it does.
	 * -mer 4 Aug 1992
	 */
	if (table->next >= table->size)
		elog(ERROR, "Plan requires more slots than are available"
			 "\n\tsend mail to your local executor guru to fix this");

	/*
	 * at this point, space in the table is guaranteed so we reserve the
	 * next slot, initialize and return it.
	 */
	slotnum = table->next;
	table->next++;

	slot = &(table->array[slotnum]);

	/* Make sure the allocated slot is valid (and empty) */
	slot->type = T_TupleTableSlot;
	slot->val = (HeapTuple) NULL;
	slot->ttc_shouldFree = true;
	slot->ttc_descIsNew = true;
	slot->ttc_shouldFreeDesc = true;
	slot->ttc_tupleDescriptor = (TupleDesc) NULL;
	slot->ttc_buffer = InvalidBuffer;

	return slot;
}

/* --------------------------------
 *		MakeTupleTableSlot
 *
 *		This routine makes an empty standalone TupleTableSlot.
 *		It really shouldn't exist, but there are a few places
 *		that do this, so we may as well centralize the knowledge
 *		of what's in one ...
 * --------------------------------
 */
TupleTableSlot *
MakeTupleTableSlot(void)
{
	TupleTableSlot *slot = makeNode(TupleTableSlot);

	/* This should match ExecAllocTableSlot() */
	slot->val = (HeapTuple) NULL;
	slot->ttc_shouldFree = true;
	slot->ttc_descIsNew = true;
	slot->ttc_shouldFreeDesc = true;
	slot->ttc_tupleDescriptor = (TupleDesc) NULL;
	slot->ttc_buffer = InvalidBuffer;

	return slot;
}

/* ----------------------------------------------------------------
 *				  tuple table slot accessor functions
 * ----------------------------------------------------------------
 */

/* --------------------------------
 *		ExecStoreTuple
 *
 *		This function is used to store a tuple into a specified
 *		slot in the tuple table.
 *
 *		tuple:	tuple to store
 *		slot:	slot to store it in
 *		buffer: disk buffer if tuple is in a disk page, else InvalidBuffer
 *		shouldFree: true if ExecClearTuple should pfree() the tuple
 *					when done with it
 *
 * If 'buffer' is not InvalidBuffer, the tuple table code acquires a pin
 * on the buffer which is held until the slot is cleared, so that the tuple
 * won't go away on us.
 *
 * shouldFree is normally set 'true' for tuples constructed on-the-fly.
 * It must always be 'false' for tuples that are stored in disk pages,
 * since we don't want to try to pfree those.
 *
 * Another case where it is 'false' is when the referenced tuple is held
 * in a tuple table slot belonging to a lower-level executor Proc node.
 * In this case the lower-level slot retains ownership and responsibility
 * for eventually releasing the tuple.	When this method is used, we must
 * be certain that the upper-level Proc node will lose interest in the tuple
 * sooner than the lower-level one does!  If you're not certain, copy the
 * lower-level tuple with heap_copytuple and let the upper-level table
 * slot assume ownership of the copy!
 *
 * Return value is just the passed-in slot pointer.
 * --------------------------------
 */
TupleTableSlot *
ExecStoreTuple(HeapTuple tuple,
			   TupleTableSlot *slot,
			   Buffer buffer,
			   bool shouldFree)
{
	/*
	 * sanity checks
	 */
	Assert(slot != NULL);
	/* passing shouldFree=true for a tuple on a disk page is not sane */
	Assert(BufferIsValid(buffer) ? (!shouldFree) : true);

	/* clear out any old contents of the slot */
	ExecClearTuple(slot);

	/*
	 * store the new tuple into the specified slot and return the slot
	 * into which we stored the tuple.
	 */
	slot->val = tuple;
	slot->ttc_buffer = buffer;
	slot->ttc_shouldFree = shouldFree;

	/*
	 * If tuple is on a disk page, keep the page pinned as long as we hold
	 * a pointer into it.
	 */
	if (BufferIsValid(buffer))
		IncrBufferRefCount(buffer);

	return slot;
}

/* --------------------------------
 *		ExecClearTuple
 *
 *		This function is used to clear out a slot in the tuple table.
 *
 *		NB: only the tuple is cleared, not the tuple descriptor (if any).
 * --------------------------------
 */
TupleTableSlot *				/* return: slot passed */
ExecClearTuple(TupleTableSlot *slot)	/* slot in which to store tuple */
{
	HeapTuple	oldtuple;		/* prior contents of slot */

	/*
	 * sanity checks
	 */
	Assert(slot != NULL);

	/*
	 * get information from the tuple table
	 */
	oldtuple = slot->val;

	/*
	 * free the old contents of the specified slot if necessary.
	 */
	if (slot->ttc_shouldFree && oldtuple != NULL)
		heap_freetuple(oldtuple);

	slot->val = (HeapTuple) NULL;

	slot->ttc_shouldFree = true;	/* probably useless code... */

	/*
	 * Drop the pin on the referenced buffer, if there is one.
	 */
	if (BufferIsValid(slot->ttc_buffer))
		ReleaseBuffer(slot->ttc_buffer);

	slot->ttc_buffer = InvalidBuffer;

	return slot;
}

/* --------------------------------
 *		ExecSetSlotDescriptor
 *
 *		This function is used to set the tuple descriptor associated
 *		with the slot's tuple.
 * --------------------------------
 */
void
ExecSetSlotDescriptor(TupleTableSlot *slot,		/* slot to change */
					  TupleDesc tupdesc,		/* new tuple descriptor */
					  bool shouldFree)	/* is desc owned by slot? */
{
	if (slot->ttc_shouldFreeDesc &&
		slot->ttc_tupleDescriptor != NULL)
		FreeTupleDesc(slot->ttc_tupleDescriptor);

	slot->ttc_tupleDescriptor = tupdesc;
	slot->ttc_shouldFreeDesc = shouldFree;
}

/* --------------------------------
 *		ExecSetSlotDescriptorIsNew
 *
 *		This function is used to change the setting of the "isNew" flag
 * --------------------------------
 */
void
ExecSetSlotDescriptorIsNew(TupleTableSlot *slot,		/* slot to change */
						   bool isNew)	/* "isNew" setting */
{
	slot->ttc_descIsNew = isNew;
}

/* ----------------------------------------------------------------
 *				  tuple table slot status predicates
 * ----------------------------------------------------------------
 */

/* ----------------------------------------------------------------
 *				convenience initialization routines
 * ----------------------------------------------------------------
 */
/* --------------------------------
 *		ExecInit{Result,Scan,Extra}TupleSlot
 *
 *		These are convenience routines to initialize the specified slot
 *		in nodes inheriting the appropriate state.	ExecInitExtraTupleSlot
 *		is used for initializing special-purpose slots.
 * --------------------------------
 */
#define INIT_SLOT_DEFS \
	TupleTable	   tupleTable; \
	TupleTableSlot*   slot

#define INIT_SLOT_ALLOC \
	tupleTable = (TupleTable) estate->es_tupleTable; \
	slot =		 ExecAllocTableSlot(tupleTable);

/* ----------------
 *		ExecInitResultTupleSlot
 * ----------------
 */
void
ExecInitResultTupleSlot(EState *estate, CommonState *commonstate)
{
	INIT_SLOT_DEFS;
	INIT_SLOT_ALLOC;
	commonstate->cs_ResultTupleSlot = slot;
}

/* ----------------
 *		ExecInitScanTupleSlot
 * ----------------
 */
void
ExecInitScanTupleSlot(EState *estate, CommonScanState *commonscanstate)
{
	INIT_SLOT_DEFS;
	INIT_SLOT_ALLOC;
	commonscanstate->css_ScanTupleSlot = slot;
}

/* ----------------
 *		ExecInitExtraTupleSlot
 * ----------------
 */
TupleTableSlot *
ExecInitExtraTupleSlot(EState *estate)
{
	INIT_SLOT_DEFS;
	INIT_SLOT_ALLOC;
	return slot;
}

/* ----------------
 *		ExecInitNullTupleSlot
 *
 * Build a slot containing an all-nulls tuple of the given type.
 * This is used as a substitute for an input tuple when performing an
 * outer join.
 * ----------------
 */
TupleTableSlot *
ExecInitNullTupleSlot(EState *estate, TupleDesc tupType)
{
	TupleTableSlot *slot = ExecInitExtraTupleSlot(estate);

	/*
	 * Since heap_getattr() will treat attributes beyond a tuple's t_natts
	 * as being NULL, we can make an all-nulls tuple just by making it be
	 * of zero length.	However, the slot descriptor must match the real
	 * tupType.
	 */
	HeapTuple	nullTuple;
	Datum		values[1];
	char		nulls[1];
	static struct tupleDesc NullTupleDesc;		/* we assume this inits to
												 * zeroes */

	ExecSetSlotDescriptor(slot, tupType, false);

	nullTuple = heap_formtuple(&NullTupleDesc, values, nulls);

	return ExecStoreTuple(nullTuple, slot, InvalidBuffer, true);
}

/* ----------------------------------------------------------------
 *		ExecTypeFromTL
 *
 *		Generate a tuple descriptor for the result tuple of a targetlist.
 *		Note that resjunk columns, if any, are included in the result.
 *
 *		Currently there are about 4 different places where we create
 *		TupleDescriptors.  They should all be merged, or perhaps
 *		be rewritten to call BuildDesc().
 * ----------------------------------------------------------------
 */
TupleDesc
ExecTypeFromTL(List *targetList)
{
	List	   *tlitem;
	TupleDesc	typeInfo;
	Resdom	   *resdom;
	Oid			restype;
	int			len;

	/*
	 * examine targetlist - if empty then return NULL
	 */
	len = ExecTargetListLength(targetList);

	if (len == 0)
		return NULL;

	/*
	 * allocate a new typeInfo
	 */
	typeInfo = CreateTemplateTupleDesc(len);

	/*
	 * scan list, generate type info for each entry
	 */
	foreach(tlitem, targetList)
	{
		TargetEntry *tle = lfirst(tlitem);

		if (tle->resdom != NULL)
		{
			resdom = tle->resdom;
			restype = resdom->restype;

			TupleDescInitEntry(typeInfo,
							   resdom->resno,
							   resdom->resname,
							   restype,
							   resdom->restypmod,
							   0,
							   false);

#ifdef NOT_USED
			ExecSetTypeInfo(resdom->resno - 1,
							typeInfo,
							(Oid) restype,
							resdom->resno,
							resdom->reslen,
							NameStr(*resdom->resname),
							get_typbyval(restype),
							get_typalign(restype));
#endif
		}
		else
		{
			/* XXX this branch looks fairly broken ... tgl 12/2000 */
			Resdom	   *fjRes;
			List	   *fjTlistP;
			List	   *fjList = lfirst(tlitem);

#ifdef SETS_FIXED
			TargetEntry *tle;
			Fjoin	   *fjNode = ((TargetEntry *) lfirst(fjList))->fjoin;

			tle = fjNode->fj_innerNode; /* ??? */
#endif
			fjRes = tle->resdom;
			restype = fjRes->restype;

			TupleDescInitEntry(typeInfo,
							   fjRes->resno,
							   fjRes->resname,
							   restype,
							   fjRes->restypmod,
							   0,
							   false);
#ifdef NOT_USED
			ExecSetTypeInfo(fjRes->resno - 1,
							typeInfo,
							(Oid) restype,
							fjRes->resno,
							fjRes->reslen,
							(char *) fjRes->resname,
							get_typbyval(restype),
							get_typalign(restype));
#endif

			foreach(fjTlistP, lnext(fjList))
			{
				TargetEntry *fjTle = lfirst(fjTlistP);

				fjRes = fjTle->resdom;

				TupleDescInitEntry(typeInfo,
								   fjRes->resno,
								   fjRes->resname,
								   restype,
								   fjRes->restypmod,
								   0,
								   false);

#ifdef NOT_USED
				ExecSetTypeInfo(fjRes->resno - 1,
								typeInfo,
								(Oid) fjRes->restype,
								fjRes->resno,
								fjRes->reslen,
								(char *) fjRes->resname,
								get_typbyval(fjRes->restype),
								get_typalign(fjRes->restype));
#endif
			}
		}
	}

	return typeInfo;
}
