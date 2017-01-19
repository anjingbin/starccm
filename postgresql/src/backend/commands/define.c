/*-------------------------------------------------------------------------
 *
 * define.c
 *
 *	  These routines execute some of the CREATE statements.  In an earlier
 *	  version of Postgres, these were "define" statements.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/commands/define.c,v 1.2 2004/07/05 08:55:22 SuLiang Exp $
 *
 * DESCRIPTION
 *	  The "DefineFoo" routines take the parse tree and pick out the
 *	  appropriate arguments/flags, passing the results to the
 *	  corresponding "FooDefine" routines (in src/catalog) that do
 *	  the actual catalog-munging.  These routines also verify permission
 *	  of the user to execute the command.
 *
 * NOTES
 *	  These things must be defined and committed in the following order:
 *		"create function":
 *				input/output, recv/send procedures
 *		"create type":
 *				type
 *		"create operator":
 *				operators
 *
 *		Most of the parse-tree manipulation routines are defined in
 *		commands/manip.c.
 *
 *-------------------------------------------------------------------------
 */
#include "postgres.h"

#include <ctype.h>
#include <math.h>

#include "access/heapam.h"
#include "catalog/catname.h"
#include "catalog/pg_aggregate.h"
#include "catalog/pg_language.h"
#include "catalog/pg_operator.h"
#include "catalog/pg_proc.h"
#include "catalog/pg_type.h"
#include "commands/defrem.h"
#include "fmgr.h"
#include "miscadmin.h"
#include "optimizer/cost.h"
#include "parser/parse_expr.h"
#include "utils/builtins.h"
#include "utils/syscache.h"

static char *defGetString(DefElem *def);
static double defGetNumeric(DefElem *def);
static int	defGetTypeLength(DefElem *def);

#define DEFAULT_TYPDELIM		','


static void
case_translate_language_name(const char *input, char *output)
{
/*
 *	Translate the input language name to lower case, except if it's "C",
 * translate to upper case.
 */
	int			i;

	for (i = 0; i < NAMEDATALEN - 1 && input[i]; ++i)
		output[i] = tolower((unsigned char) input[i]);

	output[i] = '\0';

	if (strcmp(output, "c") == 0)
		output[0] = 'C';
}



static void
compute_return_type(TypeName *returnType,
					char **prorettype_p, bool *returnsSet_p)
{
/*
 *	 Examine the "returns" clause returnType of the CREATE FUNCTION statement
 *	 and return information about it as *prorettype_p and *returnsSet.
 */
	*prorettype_p = TypeNameToInternalName(returnType);
	*returnsSet_p = returnType->setof;
}


static void
compute_full_attributes(List *parameters,
						int32 *byte_pct_p, int32 *perbyte_cpu_p,
						int32 *percall_cpu_p, int32 *outin_ratio_p,
						bool *canCache_p, bool *isStrict_p)
{
/*-------------
 *	 Interpret the parameters *parameters and return their contents as
 *	 *byte_pct_p, etc.
 *
 *	These parameters supply optional information about a function.
 *	All have defaults if not specified.
 *
 *	Note: currently, only two of these parameters actually do anything:
 *
 *	 * canCache means the optimizer's constant-folder is allowed to
 *	   pre-evaluate the function when all its inputs are constants.
 *
 *	 * isStrict means the function should not be called when any NULL
 *	   inputs are present; instead a NULL result value should be assumed.
 *
 *	The other four parameters are not used anywhere.	They used to be
 *	used in the "expensive functions" optimizer, but that's been dead code
 *	for a long time.
 *
 *	Since canCache and isStrict are useful for any function, we now allow
 *	attributes to be supplied for all functions regardless of language.
 *------------
 */
	List	   *pl;

	/* the defaults */
	*byte_pct_p = BYTE_PCT;
	*perbyte_cpu_p = PERBYTE_CPU;
	*percall_cpu_p = PERCALL_CPU;
	*outin_ratio_p = OUTIN_RATIO;
	*canCache_p = false;
	*isStrict_p = false;

	foreach(pl, parameters)
	{
		DefElem    *param = (DefElem *) lfirst(pl);

		if (strcasecmp(param->defname, "iscachable") == 0)
			*canCache_p = true;
		else if (strcasecmp(param->defname, "isstrict") == 0)
			*isStrict_p = true;
		else if (strcasecmp(param->defname, "trusted") == 0)
		{
			/*
			 * we don't have untrusted functions any more. The 4.2
			 * implementation is lousy anyway so I took it out. -ay 10/94
			 */
			elog(ERROR, "untrusted function has been decommissioned.");
		}
		else if (strcasecmp(param->defname, "byte_pct") == 0)
			*byte_pct_p = (int) defGetNumeric(param);
		else if (strcasecmp(param->defname, "perbyte_cpu") == 0)
			*perbyte_cpu_p = (int) defGetNumeric(param);
		else if (strcasecmp(param->defname, "percall_cpu") == 0)
			*percall_cpu_p = (int) defGetNumeric(param);
		else if (strcasecmp(param->defname, "outin_ratio") == 0)
			*outin_ratio_p = (int) defGetNumeric(param);
		else
			elog(NOTICE, "Unrecognized function attribute '%s' ignored",
				 param->defname);
	}
}


/*
 * For a dynamically linked C language object, the form of the clause is
 *
 *	   AS <object file name> [, <link symbol name> ]
 *
 * In all other cases
 *
 *	   AS <object reference, or sql code>
 *
 */

static void
interpret_AS_clause(const char *languageName, const List *as,
					char **prosrc_str_p, char **probin_str_p)
{
	Assert(as != NIL);

	if (strcmp(languageName, "C") == 0)
	{
		/*
		 * For "C" language, store the file name in probin and, when
		 * given, the link symbol name in prosrc.
		 */
		*probin_str_p = strVal(lfirst(as));
		if (lnext(as) == NULL)
			*prosrc_str_p = "-";
		else
			*prosrc_str_p = strVal(lsecond(as));
	}
	else
	{
		/* Everything else wants the given string in prosrc. */
		*prosrc_str_p = strVal(lfirst(as));
		*probin_str_p = "-";

		if (lnext(as) != NIL)
			elog(ERROR, "CREATE FUNCTION: only one AS item needed for %s language",
				 languageName);
	}
}



/*
 * CreateFunction
 *	 Execute a CREATE FUNCTION utility statement.
 */
void
CreateFunction(ProcedureStmt *stmt)
{
	char	   *probin_str;

	/* pathname of executable file that executes this function, if any */

	char	   *prosrc_str;

	/* SQL that executes this function, if any */

	char	   *prorettype;

	/* Type of return value (or member of set of values) from function */

	char		languageName[NAMEDATALEN];

	/*
	 * name of language of function, with case adjusted: "C", "internal",
	 * "sql", etc.
	 */

	bool		returnsSet;

	/* The function returns a set of values, as opposed to a singleton. */

	/*
	 * The following are optional user-supplied attributes of the
	 * function.
	 */
	int32		byte_pct,
				perbyte_cpu,
				percall_cpu,
				outin_ratio;
	bool		canCache,
				isStrict;

	/* Convert language name to canonical case */
	case_translate_language_name(stmt->language, languageName);

	/*
	 * Apply appropriate security checks depending on language.
	 */
	if (strcmp(languageName, "C") == 0 ||
		strcmp(languageName, "internal") == 0)
	{
		if (!superuser())
			elog(ERROR,
				 "Only users with Postgres superuser privilege are "
			   "permitted to create a function in the '%s' language.\n\t"
				 "Others may use the 'sql' language "
				 "or the created procedural languages.",
				 languageName);
	}
	else if (strcmp(languageName, "sql") == 0)
	{
		/* No security check needed for SQL functions */
	}
	else
	{
		HeapTuple	languageTuple;
		Form_pg_language languageStruct;

		/* Lookup the language in the system cache */
		languageTuple = SearchSysCache(LANGNAME,
									   PointerGetDatum(languageName),
									   0, 0, 0);
		if (!HeapTupleIsValid(languageTuple))
			elog(ERROR,
				 "Unrecognized language specified in a CREATE FUNCTION: "
				 "'%s'.\n\tPre-installed languages are SQL, C, and "
				 "internal.\n\tAdditional languages may be installed "
				 "using 'createlang'.",
				 languageName);

		/* Check that this language is a PL */
		languageStruct = (Form_pg_language) GETSTRUCT(languageTuple);
		if (!languageStruct->lanispl)
			elog(ERROR,
				 "Language '%s' isn't defined as PL", languageName);

		/*
		 * Functions in untrusted procedural languages are restricted to
		 * be defined by postgres superusers only
		 */
		if (!languageStruct->lanpltrusted && !superuser())
			elog(ERROR, "Only users with Postgres superuser privilege "
				 "are permitted to create a function in the '%s' "
				 "language.",
				 languageName);

		ReleaseSysCache(languageTuple);
	}

	/*
	 * Convert remaining parameters of CREATE to form wanted by
	 * ProcedureCreate.
	 */
	Assert(IsA(stmt->returnType, TypeName));
	compute_return_type((TypeName *) stmt->returnType,
						&prorettype, &returnsSet);

	compute_full_attributes(stmt->withClause,
							&byte_pct, &perbyte_cpu, &percall_cpu,
							&outin_ratio, &canCache, &isStrict);

	interpret_AS_clause(languageName, stmt->as, &prosrc_str, &probin_str);

	/*
	 * And now that we have all the parameters, and know we're permitted
	 * to do so, go ahead and create the function.
	 */
	ProcedureCreate(stmt->funcname,
					stmt->replace,
					returnsSet,
					prorettype,
					languageName,
					prosrc_str, /* converted to text later */
					probin_str, /* converted to text later */
					true,		/* (obsolete "trusted") */
					canCache,
					isStrict,
					byte_pct,
					perbyte_cpu,
					percall_cpu,
					outin_ratio,
					stmt->argTypes);
}



/* --------------------------------
 * DefineOperator
 *
 *		this function extracts all the information from the
 *		parameter list generated by the parser and then has
 *		OperatorCreate() do all the actual work.
 *
 * 'parameters' is a list of DefElem
 * --------------------------------
 */
void
DefineOperator(char *oprName,
			   List *parameters)
{
	uint16		precedence = 0; /* operator precedence */
	bool		canHash = false;	/* operator hashes */
	bool		isLeftAssociative = true;		/* operator is left
												 * associative */
	char	   *functionName = NULL;	/* function for operator */
	char	   *typeName1 = NULL;		/* first type name */
	char	   *typeName2 = NULL;		/* second type name */
	char	   *commutatorName = NULL;	/* optional commutator operator
										 * name */
	char	   *negatorName = NULL;		/* optional negator operator name */
	char	   *restrictionName = NULL; /* optional restrict. sel.
										 * procedure */
	char	   *joinName = NULL;	/* optional join sel. procedure name */
	char	   *sortName1 = NULL;		/* optional first sort operator */
	char	   *sortName2 = NULL;		/* optional second sort operator */
	List	   *pl;

	/*
	 * loop over the definition list and extract the information we need.
	 */
	foreach(pl, parameters)
	{
		DefElem    *defel = (DefElem *) lfirst(pl);

		if (strcasecmp(defel->defname, "leftarg") == 0)
		{
			typeName1 = defGetString(defel);
			if (IsA(defel->arg, TypeName) &&
				((TypeName *) defel->arg)->setof)
				elog(ERROR, "setof type not implemented for leftarg");
		}
		else if (strcasecmp(defel->defname, "rightarg") == 0)
		{
			typeName2 = defGetString(defel);
			if (IsA(defel->arg, TypeName) &&
				((TypeName *) defel->arg)->setof)
				elog(ERROR, "setof type not implemented for rightarg");
		}
		else if (strcasecmp(defel->defname, "procedure") == 0)
			functionName = defGetString(defel);
		else if (strcasecmp(defel->defname, "precedence") == 0)
		{
			/* NOT IMPLEMENTED (never worked in v4.2) */
			elog(NOTICE, "CREATE OPERATOR: precedence not implemented");
		}
		else if (strcasecmp(defel->defname, "associativity") == 0)
		{
			/* NOT IMPLEMENTED (never worked in v4.2) */
			elog(NOTICE, "CREATE OPERATOR: associativity not implemented");
		}
		else if (strcasecmp(defel->defname, "commutator") == 0)
			commutatorName = defGetString(defel);
		else if (strcasecmp(defel->defname, "negator") == 0)
			negatorName = defGetString(defel);
		else if (strcasecmp(defel->defname, "restrict") == 0)
			restrictionName = defGetString(defel);
		else if (strcasecmp(defel->defname, "join") == 0)
			joinName = defGetString(defel);
		else if (strcasecmp(defel->defname, "hashes") == 0)
			canHash = TRUE;
		else if (strcasecmp(defel->defname, "sort1") == 0)
		{
			/* ----------------
			 * XXX ( ... [ , sort1 = oprname ] [ , sort2 = oprname ] ... )
			 * XXX is undocumented in the reference manual source as of
			 * 89/8/22.
			 * ----------------
			 */
			sortName1 = defGetString(defel);
		}
		else if (strcasecmp(defel->defname, "sort2") == 0)
			sortName2 = defGetString(defel);
		else
		{
			elog(NOTICE, "DefineOperator: attribute \"%s\" not recognized",
				 defel->defname);
		}
	}

	/*
	 * make sure we have our required definitions
	 */
	if (functionName == NULL)
		elog(ERROR, "Define: \"procedure\" unspecified");

	/*
	 * now have OperatorCreate do all the work..
	 */
	OperatorCreate(oprName,		/* operator name */
				   typeName1,	/* first type name */
				   typeName2,	/* second type name */
				   functionName,	/* function for operator */
				   precedence,	/* operator precedence */
				   isLeftAssociative,	/* operator is left associative */
				   commutatorName,		/* optional commutator operator
										 * name */
				   negatorName, /* optional negator operator name */
				   restrictionName,		/* optional restrict. sel.
										 * procedure */
				   joinName,	/* optional join sel. procedure name */
				   canHash,		/* operator hashes */
				   sortName1,	/* optional first sort operator */
				   sortName2);	/* optional second sort operator */

}

/* -------------------
 *	DefineAggregate
 * ------------------
 */
void
DefineAggregate(char *aggName, List *parameters)
{
	char	   *transfuncName = NULL;
	char	   *finalfuncName = NULL;
	char	   *baseType = NULL;
	char	   *transType = NULL;
	char	   *initval = NULL;
	List	   *pl;

	foreach(pl, parameters)
	{
		DefElem    *defel = (DefElem *) lfirst(pl);

		/*
		 * sfunc1, stype1, and initcond1 are accepted as obsolete
		 * spellings for sfunc, stype, initcond.
		 */
		if (strcasecmp(defel->defname, "sfunc") == 0)
			transfuncName = defGetString(defel);
		else if (strcasecmp(defel->defname, "sfunc1") == 0)
			transfuncName = defGetString(defel);
		else if (strcasecmp(defel->defname, "finalfunc") == 0)
			finalfuncName = defGetString(defel);
		else if (strcasecmp(defel->defname, "basetype") == 0)
			baseType = defGetString(defel);
		else if (strcasecmp(defel->defname, "stype") == 0)
			transType = defGetString(defel);
		else if (strcasecmp(defel->defname, "stype1") == 0)
			transType = defGetString(defel);
		else if (strcasecmp(defel->defname, "initcond") == 0)
			initval = defGetString(defel);
		else if (strcasecmp(defel->defname, "initcond1") == 0)
			initval = defGetString(defel);
		else
			elog(NOTICE, "DefineAggregate: attribute \"%s\" not recognized",
				 defel->defname);
	}

	/*
	 * make sure we have our required definitions
	 */
	if (baseType == NULL)
		elog(ERROR, "Define: \"basetype\" unspecified");
	if (transType == NULL)
		elog(ERROR, "Define: \"stype\" unspecified");
	if (transfuncName == NULL)
		elog(ERROR, "Define: \"sfunc\" unspecified");

	/*
	 * Most of the argument-checking is done inside of AggregateCreate
	 */
	AggregateCreate(aggName,	/* aggregate name */
					transfuncName,		/* step function name */
					finalfuncName,		/* final function name */
					baseType,	/* type of data being aggregated */
					transType,	/* transition data type */
					initval);	/* initial condition */
}

/*
 * DefineType
 *		Registers a new type.
 */
void
DefineType(char *typeName, List *parameters)
{
	int16		internalLength = -1;	/* int2 */
	int16		externalLength = -1;	/* int2 */
	char	   *elemName = NULL;
	char	   *inputName = NULL;
	char	   *outputName = NULL;
	char	   *sendName = NULL;
	char	   *receiveName = NULL;
	char	   *defaultValue = NULL;
	bool		byValue = false;
	char		delimiter = DEFAULT_TYPDELIM;
	char	   *shadow_type;
	List	   *pl;
	char		alignment = 'i';	/* default alignment */
	char		storage = 'p';	/* default TOAST storage method */

	/*
	 * Type names must be one character shorter than other names, allowing
	 * room to create the corresponding array type name with prepended
	 * "_".
	 */
	if (strlen(typeName) > (NAMEDATALEN - 2))
		elog(ERROR, "DefineType: type names must be %d characters or less",
			 NAMEDATALEN - 2);

	foreach(pl, parameters)
	{
		DefElem    *defel = (DefElem *) lfirst(pl);

		if (strcasecmp(defel->defname, "internallength") == 0)
			internalLength = defGetTypeLength(defel);
		else if (strcasecmp(defel->defname, "externallength") == 0)
			externalLength = defGetTypeLength(defel);
		else if (strcasecmp(defel->defname, "input") == 0)
			inputName = defGetString(defel);
		else if (strcasecmp(defel->defname, "output") == 0)
			outputName = defGetString(defel);
		else if (strcasecmp(defel->defname, "send") == 0)
			sendName = defGetString(defel);
		else if (strcasecmp(defel->defname, "delimiter") == 0)
		{
			char	   *p = defGetString(defel);

			delimiter = p[0];
		}
		else if (strcasecmp(defel->defname, "receive") == 0)
			receiveName = defGetString(defel);
		else if (strcasecmp(defel->defname, "element") == 0)
			elemName = defGetString(defel);
		else if (strcasecmp(defel->defname, "default") == 0)
			defaultValue = defGetString(defel);
		else if (strcasecmp(defel->defname, "passedbyvalue") == 0)
			byValue = true;
		else if (strcasecmp(defel->defname, "alignment") == 0)
		{
			char	   *a = defGetString(defel);

			/*
			 * Note: if argument was an unquoted identifier, parser will
			 * have applied xlateSqlType() to it, so be prepared to
			 * recognize translated type names as well as the nominal
			 * form.
			 */
			if (strcasecmp(a, "double") == 0)
				alignment = 'd';
			else if (strcasecmp(a, "float8") == 0)
				alignment = 'd';
			else if (strcasecmp(a, "int4") == 0)
				alignment = 'i';
			else if (strcasecmp(a, "int2") == 0)
				alignment = 's';
			else if (strcasecmp(a, "char") == 0)
				alignment = 'c';
			else if (strcasecmp(a, "bpchar") == 0)
				alignment = 'c';
			else
				elog(ERROR, "DefineType: \"%s\" alignment not recognized",
					 a);
		}
		else if (strcasecmp(defel->defname, "storage") == 0)
		{
			char	   *a = defGetString(defel);

			if (strcasecmp(a, "plain") == 0)
				storage = 'p';
			else if (strcasecmp(a, "external") == 0)
				storage = 'e';
			else if (strcasecmp(a, "extended") == 0)
				storage = 'x';
			else if (strcasecmp(a, "main") == 0)
				storage = 'm';
			else
				elog(ERROR, "DefineType: \"%s\" storage not recognized",
					 a);
		}
		else
		{
			elog(NOTICE, "DefineType: attribute \"%s\" not recognized",
				 defel->defname);
		}
	}

	/*
	 * make sure we have our required definitions
	 */
	if (inputName == NULL)
		elog(ERROR, "Define: \"input\" unspecified");
	if (outputName == NULL)
		elog(ERROR, "Define: \"output\" unspecified");

	/*
	 * now have TypeCreate do all the real work.
	 */
	TypeCreate(typeName,		/* type name */
			   InvalidOid,		/* preassigned type oid (not done here) */
			   InvalidOid,		/* relation oid (n/a here) */
			   internalLength,	/* internal size */
			   externalLength,	/* external size */
			   'b',				/* type-type (base type) */
			   delimiter,		/* array element delimiter */
			   inputName,		/* input procedure */
			   outputName,		/* output procedure */
			   receiveName,		/* receive procedure */
			   sendName,		/* send procedure */
			   elemName,		/* element type name */
			   defaultValue,	/* default type value */
			   byValue,			/* passed by value */
			   alignment,		/* required alignment */
			   storage);		/* TOAST strategy */

	/*
	 * When we create a base type (as opposed to a complex type) we need
	 * to have an array entry for it in pg_type as well.
	 */
	shadow_type = makeArrayTypeName(typeName);

	/* alignment must be 'i' or 'd' for arrays */
	alignment = (alignment == 'd') ? 'd' : 'i';

	TypeCreate(shadow_type,		/* type name */
			   InvalidOid,		/* preassigned type oid (not done here) */
			   InvalidOid,		/* relation oid (n/a here) */
			   -1,				/* internal size */
			   -1,				/* external size */
			   'b',				/* type-type (base type) */
			   DEFAULT_TYPDELIM,	/* array element delimiter */
			   "array_in",		/* input procedure */
			   "array_out",		/* output procedure */
			   "array_in",		/* receive procedure */
			   "array_out",		/* send procedure */
			   typeName,		/* element type name */
			   NULL,			/* never a default type value */
			   false,			/* never passed by value */
			   alignment,		/* see above */
			   'x');			/* ARRAY is always toastable */

	pfree(shadow_type);
}

static char *
defGetString(DefElem *def)
{
	if (def->arg == NULL)
		elog(ERROR, "Define: \"%s\" requires a parameter",
			 def->defname);
	switch (nodeTag(def->arg))
	{
		case T_Integer:
			{
				char	   *str = palloc(32);

				snprintf(str, 32, "%ld", (long) intVal(def->arg));
				return str;
			}
		case T_Float:

			/*
			 * T_Float values are kept in string form, so this type cheat
			 * works (and doesn't risk losing precision)
			 */
			return strVal(def->arg);
		case T_String:
			return strVal(def->arg);
		case T_TypeName:
			return TypeNameToInternalName((TypeName *) def->arg);
		default:
			elog(ERROR, "Define: cannot interpret argument of \"%s\"",
				 def->defname);
	}
	return NULL;				/* keep compiler quiet */
}

static double
defGetNumeric(DefElem *def)
{
	if (def->arg == NULL)
		elog(ERROR, "Define: \"%s\" requires a numeric value",
			 def->defname);
	switch (nodeTag(def->arg))
	{
		case T_Integer:
			return (double) intVal(def->arg);
		case T_Float:
			return floatVal(def->arg);
		default:
			elog(ERROR, "Define: \"%s\" requires a numeric value",
				 def->defname);
	}
	return 0;					/* keep compiler quiet */
}

static int
defGetTypeLength(DefElem *def)
{
	if (def->arg == NULL)
		elog(ERROR, "Define: \"%s\" requires a parameter",
			 def->defname);
	switch (nodeTag(def->arg))
	{
		case T_Integer:
			return intVal(def->arg);
		case T_Float:
			elog(ERROR, "Define: \"%s\" requires an integral value",
				 def->defname);
			break;
		case T_String:
			if (strcasecmp(strVal(def->arg), "variable") == 0)
				return -1;		/* variable length */
			break;
		case T_TypeName:
			/* cope if grammar chooses to believe "variable" is a typename */
			if (strcasecmp(TypeNameToInternalName((TypeName *) def->arg),
						   "variable") == 0)
				return -1;		/* variable length */
			break;
		default:
			elog(ERROR, "Define: cannot interpret argument of \"%s\"",
				 def->defname);
	}
	elog(ERROR, "Define: invalid argument for \"%s\"",
		 def->defname);
	return 0;					/* keep compiler quiet */
}
