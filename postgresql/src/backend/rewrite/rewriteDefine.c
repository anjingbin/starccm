/*-------------------------------------------------------------------------
 *
 * rewriteDefine.c
 *	  routines for defining a rewrite rule
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/rewrite/rewriteDefine.c,v 1.2 2004/07/05 08:55:39 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include "access/heapam.h"
#include "catalog/catname.h"
#include "catalog/indexing.h"
#include "catalog/pg_rewrite.h"
#include "commands/view.h"
#include "miscadmin.h"
#include "optimizer/clauses.h"
#include "parser/parse_relation.h"
#include "rewrite/rewriteDefine.h"
#include "rewrite/rewriteManip.h"
#include "rewrite/rewriteSupport.h"
#include "storage/smgr.h"
#include "utils/builtins.h"
#include "utils/syscache.h"


static void setRuleCheckAsUser(Query *qry, Oid userid);
static bool setRuleCheckAsUser_walker(Node *node, Oid *context);


/*
 * InsertRule -
 *	  takes the arguments and inserts them as attributes into the system
 *	  relation "pg_rewrite"
 */
static Oid
InsertRule(char *rulname,
		   int evtype,
		   Oid eventrel_oid,
		   AttrNumber evslot_index,
		   bool evinstead,
		   char *evqual,
		   char *actiontree)
{
	int			i;
	Datum		values[Natts_pg_rewrite];
	char		nulls[Natts_pg_rewrite];
	NameData	rname;
	Relation	pg_rewrite_desc;
	TupleDesc	tupDesc;
	HeapTuple	tup;
	Oid			rewriteObjectId;

	if (IsDefinedRewriteRule(rulname))
		elog(ERROR, "Attempt to insert rule \"%s\" failed: already exists",
			 rulname);

	/*
	 * Set up *nulls and *values arrays
	 */
	MemSet(nulls, ' ', sizeof(nulls));

	i = 0;
	namestrcpy(&rname, rulname);
	values[i++] = NameGetDatum(&rname);
	values[i++] = CharGetDatum(evtype + '0');
	values[i++] = ObjectIdGetDatum(eventrel_oid);
	values[i++] = Int16GetDatum(evslot_index);
	values[i++] = BoolGetDatum(evinstead);
	values[i++] = DirectFunctionCall1(textin, CStringGetDatum(evqual));
	values[i++] = DirectFunctionCall1(textin, CStringGetDatum(actiontree));

	/*
	 * create a new pg_rewrite tuple
	 */
	pg_rewrite_desc = heap_openr(RewriteRelationName, RowExclusiveLock);

	tupDesc = pg_rewrite_desc->rd_att;

	tup = heap_formtuple(tupDesc,
						 values,
						 nulls);

	heap_insert(pg_rewrite_desc, tup);

	rewriteObjectId = tup->t_data->t_oid;

	if (RelationGetForm(pg_rewrite_desc)->relhasindex)
	{
		Relation	idescs[Num_pg_rewrite_indices];

		CatalogOpenIndices(Num_pg_rewrite_indices, Name_pg_rewrite_indices,
						   idescs);
		CatalogIndexInsert(idescs, Num_pg_rewrite_indices, pg_rewrite_desc,
						   tup);
		CatalogCloseIndices(Num_pg_rewrite_indices, idescs);
	}

	heap_freetuple(tup);

	heap_close(pg_rewrite_desc, RowExclusiveLock);

	return rewriteObjectId;
}

/*
 *		for now, event_object must be a single attribute
 */
static void
ValidateRule(int event_type,
			 char *eobj_string,
			 char *eslot_string,
			 Node *event_qual,
			 List **action,
			 int is_instead,
			 Oid event_attype)
{
	if (((event_type == CMD_INSERT) || (event_type == CMD_DELETE)) &&
		eslot_string)
	{
		elog(ERROR,
		"rules not allowed for insert or delete events to an attribute");
	}

#ifdef NOT_USED

	/*---------
	 * on retrieve to class.attribute do instead nothing is converted to
	 * 'on retrieve to class.attribute do instead:
	 *
	 *	 retrieve (attribute = NULL)'
	 *
	 * this is also a terrible hack that works well -- glass
	 *---------
	 */
	if (is_instead && !*action && eslot_string && event_type == CMD_SELECT)
	{
		char	   *temp_buffer = (char *) palloc(strlen(template) + 80);

		sprintf(temp_buffer, template, event_attype,
				get_typlen(event_attype), eslot_string,
				event_attype);

		*action = (List *) stringToNode(temp_buffer);

		pfree(temp_buffer);
	}
#endif
}

void
DefineQueryRewrite(RuleStmt *stmt)
{
	CmdType		event_type = stmt->event;
	Attr	   *event_obj = stmt->object;
	Node	   *event_qual = stmt->whereClause;
	bool		is_instead = stmt->instead;
	List	   *action = stmt->actions;
	Relation	event_relation;
	Oid			ev_relid;
	Oid			ruleId;
	char	   *eslot_string = NULL;
	int			event_attno;
	Oid			event_attype;
	char	   *actionP,
			   *event_qualP;
	List	   *l;
	Query	   *query;
	bool		RelisBecomingView = false;

	/*
	 * If we are installing an ON SELECT rule, we had better grab
	 * AccessExclusiveLock to ensure no SELECTs are currently running on
	 * the event relation.	For other types of rules, it might be
	 * sufficient to grab ShareLock to lock out insert/update/delete
	 * actions.  But for now, let's just grab AccessExclusiveLock all the
	 * time.
	 */
	event_relation = heap_openr(event_obj->relname, AccessExclusiveLock);
	ev_relid = RelationGetRelid(event_relation);

	/*
	 * The current rewrite handler is known to work on relation level
	 * rules only. And for SELECT events, it expects one non-nothing
	 * action that is instead and returns exactly a tuple of the rewritten
	 * relation. This restricts SELECT rules to views.
	 *
	 * Jan
	 */
	if (event_obj->attrs)
		elog(ERROR, "attribute level rules currently not supported");

	/*
	 * eslot_string = strVal(lfirst(event_obj->attrs));
	 */
	else
		eslot_string = NULL;

	/*
	 * No rule actions that modify OLD or NEW
	 */
	foreach(l, action)
	{
		query = (Query *) lfirst(l);
		if (query->resultRelation == 0)
			continue;
		/* Don't be fooled by INSERT/SELECT */
		if (query != getInsertSelectQuery(query, NULL))
			continue;
		if (query->resultRelation == PRS2_OLD_VARNO)
			elog(ERROR, "rule actions on OLD currently not supported"
				 "\n\tuse views or triggers instead");
		if (query->resultRelation == PRS2_NEW_VARNO)
			elog(ERROR, "rule actions on NEW currently not supported"
				 "\n\tuse triggers instead");
	}

	/*
	 * Rules ON SELECT are restricted to view definitions
	 */
	if (event_type == CMD_SELECT)
	{
		List	   *tllist;
		int			i;
		char	   *expected_name;

		/*
		 * So there cannot be INSTEAD NOTHING, ...
		 */
		if (length(action) == 0)
		{
			elog(ERROR, "instead nothing rules on select currently not supported"
				 "\n\tuse views instead");
		}

		/*
		 * ... there cannot be multiple actions, ...
		 */
		if (length(action) > 1)
			elog(ERROR, "multiple action rules on select currently not supported");

		/*
		 * ... the one action must be a SELECT, ...
		 */
		query = (Query *) lfirst(action);
		if (!is_instead || query->commandType != CMD_SELECT)
			elog(ERROR, "only instead-select rules currently supported on select");

		/*
		 * ... there can be no rule qual, ...
		 */
		if (event_qual != NULL)
			elog(ERROR, "event qualifications not supported for rules on select");

		/*
		 * ... the targetlist of the SELECT action must exactly match the
		 * event relation, ...
		 */
		i = 0;
		foreach(tllist, query->targetList)
		{
			TargetEntry *tle = (TargetEntry *) lfirst(tllist);
			Resdom	   *resdom = tle->resdom;
			Form_pg_attribute attr;
			char	   *attname;

			if (resdom->resjunk)
				continue;
			i++;
			if (i > event_relation->rd_att->natts)
				elog(ERROR, "select rule's target list has too many entries");

			attr = event_relation->rd_att->attrs[i - 1];
			attname = NameStr(attr->attname);

			if (strcmp(resdom->resname, attname) != 0)
				elog(ERROR, "select rule's target entry %d has different column name from %s", i, attname);

			if (attr->atttypid != resdom->restype)
				elog(ERROR, "select rule's target entry %d has different type from attribute %s", i, attname);

			/*
			 * Allow typmods to be different only if one of them is -1,
			 * ie, "unspecified".  This is necessary for cases like
			 * "numeric", where the table will have a filled-in default
			 * length but the select rule's expression will probably have
			 * typmod = -1.
			 */
			if (attr->atttypmod != resdom->restypmod &&
				attr->atttypmod != -1 && resdom->restypmod != -1)
				elog(ERROR, "select rule's target entry %d has different size from attribute %s", i, attname);
		}

		if (i != event_relation->rd_att->natts)
			elog(ERROR, "select rule's target list has too few entries");

		/*
		 * ... there must not be another ON SELECT rule already ...
		 */
		if (event_relation->rd_rules != NULL)
		{
			for (i = 0; i < event_relation->rd_rules->numLocks; i++)
			{
				RewriteRule *rule;

				rule = event_relation->rd_rules->rules[i];
				if (rule->event == CMD_SELECT)
					elog(ERROR, "\"%s\" is already a view",
						 RelationGetRelationName(event_relation));
			}
		}

		/*
		 * ... and finally the rule must be named _RETviewname.
		 */
		expected_name = MakeRetrieveViewRuleName(event_obj->relname);
		if (strcmp(expected_name, stmt->rulename) != 0)
		{
			elog(ERROR, "view rule for \"%s\" must be named \"%s\"",
				 event_obj->relname, expected_name);
		}
		pfree(expected_name);

		/*
		 * Are we converting a relation to a view?
		 *
		 * If so, check that the relation is empty because the storage for
		 * the relation is going to be deleted.
		 */
		if (event_relation->rd_rel->relkind != RELKIND_VIEW)
		{
			HeapScanDesc scanDesc;
			HeapTuple	tuple;

			scanDesc = heap_beginscan(event_relation, 0, SnapshotNow, 0, NULL);
			tuple = heap_getnext(scanDesc, 0);
			if (HeapTupleIsValid(tuple))
				elog(ERROR, "Relation \"%s\" is not empty. Cannot convert it to view",
					 event_obj->relname);

			/*
			 * don't need heap_freetuple because we never got a valid
			 * tuple
			 */
			heap_endscan(scanDesc);

			RelisBecomingView = true;
		}
	}

	/*
	 * This rule is allowed - prepare to install it.
	 */
	if (eslot_string == NULL)
	{
		event_attno = -1;
		event_attype = InvalidOid;
	}
	else
	{
		event_attno = attnameAttNum(event_relation, eslot_string);
		event_attype = attnumTypeId(event_relation, event_attno);
	}

	/* fix bug about instead nothing */
	ValidateRule(event_type, event_obj->relname,
				 eslot_string, event_qual, &action,
				 is_instead, event_attype);

	/*
	 * We want the rule's table references to be checked as though by the
	 * rule owner, not the user referencing the rule.  Therefore, scan
	 * through the rule's rtables and set the checkAsUser field on all
	 * rtable entries.
	 */
	foreach(l, action)
	{
		query = (Query *) lfirst(l);
		setRuleCheckAsUser(query, GetUserId());
	}

	/* discard rule if it's null action and not INSTEAD; it's a no-op */
	if (action != NIL || is_instead)
	{
		event_qualP = nodeToString(event_qual);
		actionP = nodeToString(action);

		ruleId = InsertRule(stmt->rulename,
							event_type,
							ev_relid,
							event_attno,
							is_instead,
							event_qualP,
							actionP);

		/*
		 * Set pg_class 'relhasrules' field TRUE for event relation. If
		 * appropriate, also modify the 'relkind' field to show that the
		 * relation is now a view.
		 *
		 * Important side effect: an SI notice is broadcast to force all
		 * backends (including me!) to update relcache entries with the
		 * new rule.
		 */
		SetRelationRuleStatus(ev_relid, true, RelisBecomingView);
	}

	/*
	 * IF the relation is becoming a view, delete the storage files
	 * associated with it.	NB: we had better have AccessExclusiveLock to
	 * do this ...
	 */
	if (RelisBecomingView)
		smgrunlink(DEFAULT_SMGR, event_relation);

	/* Close rel, but keep lock till commit... */
	heap_close(event_relation, NoLock);
}

/*
 * setRuleCheckAsUser
 *		Recursively scan a query and set the checkAsUser field to the
 *		given userid in all rtable entries.
 *
 * Note: for a view (ON SELECT rule), the checkAsUser field of the *OLD*
 * RTE entry will be overridden when the view rule is expanded, and the
 * checkAsUser field of the *NEW* entry is irrelevant because that entry's
 * checkFor bits will never be set.  However, for other types of rules it's
 * important to set these fields to match the rule owner.  So we just set
 * them always.
 */
static void
setRuleCheckAsUser(Query *qry, Oid userid)
{
	List	   *l;

	/* Set all the RTEs in this query node */
	foreach(l, qry->rtable)
	{
		RangeTblEntry *rte = (RangeTblEntry *) lfirst(l);

		if (rte->subquery)
		{
			/* Recurse into subquery in FROM */
			setRuleCheckAsUser(rte->subquery, userid);
		}
		else
			rte->checkAsUser = userid;
	}

	/* If there are sublinks, search for them and process their RTEs */
	if (qry->hasSubLinks)
		query_tree_walker(qry, setRuleCheckAsUser_walker, (void *) &userid,
						  false /* already did the ones in rtable */ );
}

/*
 * Expression-tree walker to find sublink queries
 */
static bool
setRuleCheckAsUser_walker(Node *node, Oid *context)
{
	if (node == NULL)
		return false;
	if (IsA(node, Query))
	{
		Query	   *qry = (Query *) node;

		setRuleCheckAsUser(qry, *context);
		return false;
	}
	return expression_tree_walker(node, setRuleCheckAsUser_walker,
								  (void *) context);
}


/*
 * Rename an existing rewrite rule.
 *
 * There is not currently a user command to invoke this directly
 * (perhaps there should be).  But we need it anyway to rename the
 * ON SELECT rule associated with a view, when the view is renamed.
 */
void
RenameRewriteRule(char *oldname, char *newname)
{
	Relation	pg_rewrite_desc;
	HeapTuple	ruletup;

	pg_rewrite_desc = heap_openr(RewriteRelationName, RowExclusiveLock);

	ruletup = SearchSysCacheCopy(RULENAME,
								 PointerGetDatum(oldname),
								 0, 0, 0);
	if (!HeapTupleIsValid(ruletup))
		elog(ERROR, "RenameRewriteRule: rule \"%s\" does not exist", oldname);

	/* should not already exist */
	if (IsDefinedRewriteRule(newname))
		elog(ERROR, "Attempt to rename rule \"%s\" failed: \"%s\" already exists",
			 oldname, newname);

	StrNCpy(NameStr(((Form_pg_rewrite) GETSTRUCT(ruletup))->rulename),
			newname, NAMEDATALEN);

	simple_heap_update(pg_rewrite_desc, &ruletup->t_self, ruletup);

	/* keep system catalog indices current */
	if (RelationGetForm(pg_rewrite_desc)->relhasindex)
	{
		Relation	idescs[Num_pg_rewrite_indices];

		CatalogOpenIndices(Num_pg_rewrite_indices, Name_pg_rewrite_indices,
						   idescs);
		CatalogIndexInsert(idescs, Num_pg_rewrite_indices, pg_rewrite_desc,
						   ruletup);
		CatalogCloseIndices(Num_pg_rewrite_indices, idescs);
	}

	heap_freetuple(ruletup);
	heap_close(pg_rewrite_desc, RowExclusiveLock);
}
