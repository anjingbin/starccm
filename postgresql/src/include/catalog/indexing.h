/*-------------------------------------------------------------------------
 *
 * indexing.h
 *	  This file provides some definitions to support indexing
 *	  on system catalogs
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * $Id: indexing.h,v 1.2 2004/07/05 08:55:58 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef INDEXING_H
#define INDEXING_H

#include "access/htup.h"

/*
 * Number of indices that exist for each system catalog
 */
#define Num_pg_aggregate_indices	2
#define Num_pg_am_indices			2
#define Num_pg_amop_indices			2
#define Num_pg_amproc_indices		1
#define Num_pg_attr_indices			2
#define Num_pg_attrdef_indices		1
#define Num_pg_class_indices		2
#define Num_pg_database_indices		2
#define Num_pg_description_indices	1
#define Num_pg_group_indices		2
#define Num_pg_index_indices		2
#define Num_pg_inherits_indices		1
#define Num_pg_language_indices		2
#define Num_pg_largeobject_indices	1
#define Num_pg_opclass_indices		2
#define Num_pg_operator_indices		2
#define Num_pg_proc_indices			2
#define Num_pg_relcheck_indices		1
#define Num_pg_rewrite_indices		2
#define Num_pg_shadow_indices		2
#define Num_pg_statistic_indices	1
#define Num_pg_trigger_indices		4
#define Num_pg_type_indices			2

/*
 * Names of indices on system catalogs
 */
#define AccessMethodOperatorIndex	"pg_amop_opc_opr_index"
#define AccessMethodStrategyIndex	"pg_amop_opc_strategy_index"
#define AccessMethodProcedureIndex	"pg_amproc_opc_procnum_index"
#define AggregateNameTypeIndex		"pg_aggregate_name_type_index"
#define AggregateOidIndex			"pg_aggregate_oid_index"
#define AmNameIndex					"pg_am_name_index"
#define AmOidIndex					"pg_am_oid_index"
#define AttrDefaultIndex			"pg_attrdef_adrelid_adnum_index"
#define AttributeRelidNameIndex		"pg_attribute_relid_attnam_index"
#define AttributeRelidNumIndex		"pg_attribute_relid_attnum_index"
#define ClassNameIndex				"pg_class_relname_index"
#define ClassOidIndex				"pg_class_oid_index"
#define DatabaseNameIndex			"pg_database_datname_index"
#define DatabaseOidIndex			"pg_database_oid_index"
#define DescriptionObjIndex			"pg_description_o_c_o_index"
#define GroupNameIndex				"pg_group_name_index"
#define GroupSysidIndex				"pg_group_sysid_index"
#define IndexIndrelidIndex			"pg_index_indrelid_index"
#define IndexRelidIndex				"pg_index_indexrelid_index"
#define InheritsRelidSeqnoIndex		"pg_inherits_relid_seqno_index"
#define LanguageNameIndex			"pg_language_name_index"
#define LanguageOidIndex			"pg_language_oid_index"
#define LargeObjectLOidPNIndex		"pg_largeobject_loid_pn_index"
#define OpclassAmNameIndex			"pg_opclass_am_name_index"
#define OpclassOidIndex				"pg_opclass_oid_index"
#define OperatorNameIndex			"pg_operator_oprname_l_r_k_index"
#define OperatorOidIndex			"pg_operator_oid_index"
#define ProcedureNameIndex			"pg_proc_proname_narg_type_index"
#define ProcedureOidIndex			"pg_proc_oid_index"
#define RelCheckIndex				"pg_relcheck_rcrelid_index"
#define RewriteOidIndex				"pg_rewrite_oid_index"
#define RewriteRulenameIndex		"pg_rewrite_rulename_index"
#define ShadowNameIndex				"pg_shadow_usename_index"
#define ShadowSysidIndex			"pg_shadow_usesysid_index"
#define StatisticRelidAttnumIndex	"pg_statistic_relid_att_index"
#define TriggerConstrNameIndex		"pg_trigger_tgconstrname_index"
#define TriggerConstrRelidIndex		"pg_trigger_tgconstrrelid_index"
#define TriggerRelidIndex			"pg_trigger_tgrelid_index"
#define TriggerOidIndex				"pg_trigger_oid_index"
#define TypeNameIndex				"pg_type_typname_index"
#define TypeOidIndex				"pg_type_oid_index"

/* Arrays of names of indices for each system catalog */
extern char *Name_pg_aggregate_indices[];
extern char *Name_pg_am_indices[];
extern char *Name_pg_amop_indices[];
extern char *Name_pg_amproc_indices[];
extern char *Name_pg_attr_indices[];
extern char *Name_pg_attrdef_indices[];
extern char *Name_pg_class_indices[];
extern char *Name_pg_database_indices[];
extern char *Name_pg_description_indices[];
extern char *Name_pg_group_indices[];
extern char *Name_pg_index_indices[];
extern char *Name_pg_inherits_indices[];
extern char *Name_pg_language_indices[];
extern char *Name_pg_largeobject_indices[];
extern char *Name_pg_opclass_indices[];
extern char *Name_pg_operator_indices[];
extern char *Name_pg_proc_indices[];
extern char *Name_pg_relcheck_indices[];
extern char *Name_pg_rewrite_indices[];
extern char *Name_pg_shadow_indices[];
extern char *Name_pg_statistic_indices[];
extern char *Name_pg_trigger_indices[];
extern char *Name_pg_type_indices[];


/*
 * indexing.c prototypes
 */
extern void CatalogOpenIndices(int nIndices, char **names, Relation *idescs);
extern void CatalogCloseIndices(int nIndices, Relation *idescs);
extern void CatalogIndexInsert(Relation *idescs, int nIndices,
				   Relation heapRelation, HeapTuple heapTuple);

/*
 * Canned functions for indexscans on certain system indexes.
 * All index-value arguments should be passed as Datum for portability!
 */
extern HeapTuple AttributeRelidNumIndexScan(Relation heapRelation,
						   Datum relid, Datum attnum);
extern HeapTuple ClassNameIndexScan(Relation heapRelation, Datum relName);
extern HeapTuple ClassOidIndexScan(Relation heapRelation, Datum relId);


/*
 * These macros are just to keep the C compiler from spitting up on the
 * upcoming commands for genbki.sh.
 */
#define DECLARE_INDEX(x) extern int errno
#define DECLARE_UNIQUE_INDEX(x) extern int errno
#define BUILD_INDICES


/*
 * What follows are lines processed by genbki.sh to create the statements
 * the bootstrap parser will turn into DefineIndex commands.
 *
 * The keyword is DECLARE_INDEX or DECLARE_UNIQUE_INDEX.  Everything after
 * that is just like in a normal 'create index' SQL command.
 */

DECLARE_UNIQUE_INDEX(pg_aggregate_name_type_index on pg_aggregate using btree(aggname name_ops, aggbasetype oid_ops));
DECLARE_UNIQUE_INDEX(pg_aggregate_oid_index on pg_aggregate using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_am_name_index on pg_am using btree(amname name_ops));
DECLARE_UNIQUE_INDEX(pg_am_oid_index on pg_am using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_amop_opc_opr_index on pg_amop using btree(amopclaid oid_ops, amopopr oid_ops));
DECLARE_UNIQUE_INDEX(pg_amop_opc_strategy_index on pg_amop using btree(amopclaid oid_ops, amopstrategy int2_ops));
DECLARE_UNIQUE_INDEX(pg_amproc_opc_procnum_index on pg_amproc using btree(amopclaid oid_ops, amprocnum int2_ops));
DECLARE_UNIQUE_INDEX(pg_attrdef_adrelid_adnum_index on pg_attrdef using btree(adrelid oid_ops, adnum int2_ops));
DECLARE_UNIQUE_INDEX(pg_attribute_relid_attnam_index on pg_attribute using btree(attrelid oid_ops, attname name_ops));
DECLARE_UNIQUE_INDEX(pg_attribute_relid_attnum_index on pg_attribute using btree(attrelid oid_ops, attnum int2_ops));
DECLARE_UNIQUE_INDEX(pg_class_oid_index on pg_class using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_class_relname_index on pg_class using btree(relname name_ops));
DECLARE_UNIQUE_INDEX(pg_database_datname_index on pg_database using btree(datname name_ops));
DECLARE_UNIQUE_INDEX(pg_database_oid_index on pg_database using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_description_o_c_o_index on pg_description using btree(objoid oid_ops, classoid oid_ops, objsubid int4_ops));
DECLARE_UNIQUE_INDEX(pg_group_name_index on pg_group using btree(groname name_ops));
DECLARE_UNIQUE_INDEX(pg_group_sysid_index on pg_group using btree(grosysid int4_ops));
/* This following index is not used for a cache and is not unique */
DECLARE_INDEX(pg_index_indrelid_index on pg_index using btree(indrelid oid_ops));
DECLARE_UNIQUE_INDEX(pg_index_indexrelid_index on pg_index using btree(indexrelid oid_ops));
DECLARE_UNIQUE_INDEX(pg_inherits_relid_seqno_index on pg_inherits using btree(inhrelid oid_ops, inhseqno int4_ops));
DECLARE_UNIQUE_INDEX(pg_language_name_index on pg_language using btree(lanname name_ops));
DECLARE_UNIQUE_INDEX(pg_language_oid_index on pg_language using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_largeobject_loid_pn_index on pg_largeobject using btree(loid oid_ops, pageno int4_ops));
DECLARE_UNIQUE_INDEX(pg_opclass_am_name_index on pg_opclass using btree(opcamid oid_ops, opcname name_ops));
DECLARE_UNIQUE_INDEX(pg_opclass_oid_index on pg_opclass using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_operator_oid_index on pg_operator using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_operator_oprname_l_r_k_index on pg_operator using btree(oprname name_ops, oprleft oid_ops, oprright oid_ops, oprkind char_ops));
DECLARE_UNIQUE_INDEX(pg_proc_oid_index on pg_proc using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_proc_proname_narg_type_index on pg_proc using btree(proname name_ops, pronargs int2_ops, proargtypes oidvector_ops));
/* This following index is not used for a cache and is not unique */
DECLARE_INDEX(pg_relcheck_rcrelid_index on pg_relcheck using btree(rcrelid oid_ops));
DECLARE_UNIQUE_INDEX(pg_rewrite_oid_index on pg_rewrite using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_rewrite_rulename_index on pg_rewrite using btree(rulename name_ops));
DECLARE_UNIQUE_INDEX(pg_shadow_usename_index on pg_shadow using btree(usename name_ops));
DECLARE_UNIQUE_INDEX(pg_shadow_usesysid_index on pg_shadow using btree(usesysid int4_ops));
DECLARE_UNIQUE_INDEX(pg_statistic_relid_att_index on pg_statistic using btree(starelid oid_ops, staattnum int2_ops));
DECLARE_INDEX(pg_trigger_tgconstrname_index on pg_trigger using btree(tgconstrname name_ops));
DECLARE_INDEX(pg_trigger_tgconstrrelid_index on pg_trigger using btree(tgconstrrelid oid_ops));
DECLARE_INDEX(pg_trigger_tgrelid_index on pg_trigger using btree(tgrelid oid_ops));
DECLARE_UNIQUE_INDEX(pg_trigger_oid_index on pg_trigger using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_type_oid_index on pg_type using btree(oid oid_ops));
DECLARE_UNIQUE_INDEX(pg_type_typname_index on pg_type using btree(typname name_ops));

/* last step of initialization script: build the indices declared above */
BUILD_INDICES

#endif   /* INDEXING_H */
