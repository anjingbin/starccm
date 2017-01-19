/*--------------------------------------------------------------------
 * guc.c
 *
 * Support for grand unified configuration scheme, including SET
 * command, configuration file, and command line options.
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/utils/misc/guc.c,v 1.2 2004/07/05 08:55:45 SuLiang Exp $
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 * Written by Peter Eisentraut <peter_e@gmx.net>.
 *--------------------------------------------------------------------
 */

#include "postgres.h"

#include <errno.h>
#include <float.h>
#include <limits.h>
#include <unistd.h>

#include "utils/guc.h"

#include "access/xlog.h"
#include "commands/async.h"
#include "fmgr.h"
#include "libpq/auth.h"
#include "libpq/pqcomm.h"
#include "miscadmin.h"
#include "optimizer/cost.h"
#include "optimizer/geqo.h"
#include "optimizer/paths.h"
#include "optimizer/planmain.h"
#include "parser/parse_expr.h"
#include "storage/fd.h"
#include "storage/freespace.h"
#include "storage/lock.h"
#include "storage/proc.h"
#include "tcop/tcopprot.h"
#include "utils/datetime.h"
#include "pgstat.h"


/* XXX these should be in other modules' header files */
extern bool Log_connections;
extern int	PreAuthDelay;
extern int	AuthenticationTimeout;
extern int	CheckPointTimeout;
extern int	CommitDelay;
extern int	CommitSiblings;
extern bool FixBTree;

#ifdef ENABLE_SYSLOG
extern char *Syslog_facility;
extern char *Syslog_ident;
static bool check_facility(const char *facility);
#endif

static char *default_iso_level_string;

static bool check_defaultxactisolevel(const char *value);
static void assign_defaultxactisolevel(const char *value);

/*
 * Debugging options
 */
#ifdef USE_ASSERT_CHECKING
bool		assert_enabled = true;
#endif
bool		Debug_print_query = false;
bool		Debug_print_plan = false;
bool		Debug_print_parse = false;
bool		Debug_print_rewritten = false;
bool		Debug_pretty_print = false;

bool		Show_parser_stats = false;
bool		Show_planner_stats = false;
bool		Show_executor_stats = false;
bool		Show_query_stats = false;	/* this is sort of all three above
										 * together */
bool		Show_btree_build_stats = false;

bool		SQL_inheritance = true;

bool		Australian_timezones = false;

bool		Password_encryption = false;

#ifndef PG_KRB_SRVTAB
#define PG_KRB_SRVTAB ""
#endif


/*
 * Declarations for GUC tables
 */
enum config_type
{
	PGC_NONE = 0,
	PGC_BOOL,
	PGC_INT,
	PGC_REAL,
	PGC_STRING
};


struct config_generic
{
	const char *name;
	GucContext	context;
	void	   *variable;
};


struct config_bool
{
	const char *name;
	GucContext	context;
	bool	   *variable;
	bool		default_val;
	/* No need for parse_hook ... presumably both values are legal */
	void		(*assign_hook) (bool newval);
};


struct config_int
{
	const char *name;
	GucContext	context;
	int		   *variable;
	int			default_val;
	int			min;
	int			max;
	bool		(*parse_hook) (int proposed);
	void		(*assign_hook) (int newval);
};


struct config_real
{
	const char *name;
	GucContext	context;
	double	   *variable;
	double		default_val;
	double		min;
	double		max;
	bool		(*parse_hook) (double proposed);
	void		(*assign_hook) (double newval);
};

/*
 * String value options are allocated with strdup, not with the
 * pstrdup/palloc mechanisms. That is because configuration settings
 * are already in place before the memory subsystem is up. It would
 * perhaps be an idea to change that sometime.
 */
struct config_string
{
	const char *name;
	GucContext	context;
	char	  **variable;
	const char *boot_default_val;
	bool		(*parse_hook) (const char *proposed);
	void		(*assign_hook) (const char *newval);
	char	   *default_val;
};


/*
 * TO ADD AN OPTION:
 *
 * 1. Declare a global variable of type bool, int, double, or char*
 * and make use of it.
 *
 * 2. Decide at what times it's safe to set the option. See guc.h for
 * details.
 *
 * 3. Decide on a name, a default value, upper and lower bounds (if
 * applicable), etc.
 *
 * 4. Add a record below.
 *
 * 5. Add it to src/backend/utils/misc/postgresql.conf.sample.
 *
 * 6. Add it to src/bin/psql/tab-complete.c, if it's a USERSET option.
 *
 * 7. Don't forget to document the option.
 */


/******** option names follow ********/

static struct config_bool
			ConfigureNamesBool[] =
{
	{
		"enable_seqscan", PGC_USERSET, &enable_seqscan, true, NULL
	},
	{
		"enable_indexscan", PGC_USERSET, &enable_indexscan, true, NULL
	},
	{
		"enable_tidscan", PGC_USERSET, &enable_tidscan, true, NULL
	},
	{
		"enable_sort", PGC_USERSET, &enable_sort, true, NULL
	},
	{
		"enable_nestloop", PGC_USERSET, &enable_nestloop, true, NULL
	},
	{
		"enable_mergejoin", PGC_USERSET, &enable_mergejoin, true, NULL
	},
	{
		"enable_hashjoin", PGC_USERSET, &enable_hashjoin, true, NULL
	},

	{
		"ksqo", PGC_USERSET, &_use_keyset_query_optimizer, false, NULL
	},
	{
		"geqo", PGC_USERSET, &enable_geqo, true, NULL
	},

	{
		"tcpip_socket", PGC_POSTMASTER, &NetServer, false, NULL
	},
	{
		"ssl", PGC_POSTMASTER, &EnableSSL, false, NULL
	},
	{
		"fsync", PGC_SIGHUP, &enableFsync, true, NULL
	},
	{
		"silent_mode", PGC_POSTMASTER, &SilentMode, false, NULL
	},

	{
		"log_connections", PGC_BACKEND, &Log_connections, false, NULL
	},
	{
		"log_timestamp", PGC_SIGHUP, &Log_timestamp, false, NULL
	},
	{
		"log_pid", PGC_SIGHUP, &Log_pid, false, NULL
	},

#ifdef USE_ASSERT_CHECKING
	{
		"debug_assertions", PGC_USERSET, &assert_enabled, true, NULL
	},
#endif

	{
		"debug_print_query", PGC_USERSET, &Debug_print_query, false, NULL
	},
	{
		"debug_print_parse", PGC_USERSET, &Debug_print_parse, false, NULL
	},
	{
		"debug_print_rewritten", PGC_USERSET, &Debug_print_rewritten, false, NULL
	},
	{
		"debug_print_plan", PGC_USERSET, &Debug_print_plan, false, NULL
	},
	{
		"debug_pretty_print", PGC_USERSET, &Debug_pretty_print, false, NULL
	},

	{
		"show_parser_stats", PGC_USERSET, &Show_parser_stats, false, NULL
	},
	{
		"show_planner_stats", PGC_USERSET, &Show_planner_stats, false, NULL
	},
	{
		"show_executor_stats", PGC_USERSET, &Show_executor_stats, false, NULL
	},
	{
		"show_query_stats", PGC_USERSET, &Show_query_stats, false, NULL
	},
#ifdef BTREE_BUILD_STATS
	{
		"show_btree_build_stats", PGC_SUSET, &Show_btree_build_stats, false, NULL
	},
#endif

	{
		"stats_start_collector", PGC_POSTMASTER, &pgstat_collect_startcollector, true, NULL
	},
	{
		"stats_reset_on_server_start", PGC_POSTMASTER, &pgstat_collect_resetonpmstart, true, NULL
	},
	{
		"stats_command_string", PGC_SUSET, &pgstat_collect_querystring, false, NULL
	},
	{
		"stats_row_level", PGC_SUSET, &pgstat_collect_tuplelevel, false, NULL
	},
	{
		"stats_block_level", PGC_SUSET, &pgstat_collect_blocklevel, false, NULL
	},

	{
		"trace_notify", PGC_USERSET, &Trace_notify, false, NULL
	},

#ifdef LOCK_DEBUG
	{
		"trace_locks", PGC_SUSET, &Trace_locks, false, NULL
	},
	{
		"trace_userlocks", PGC_SUSET, &Trace_userlocks, false, NULL
	},
	{
		"trace_lwlocks", PGC_SUSET, &Trace_lwlocks, false, NULL
	},
	{
		"debug_deadlocks", PGC_SUSET, &Debug_deadlocks, false, NULL
	},
#endif

	{
		"hostname_lookup", PGC_SIGHUP, &HostnameLookup, false, NULL
	},
	{
		"show_source_port", PGC_SIGHUP, &ShowPortNumber, false, NULL
	},

	{
		"sql_inheritance", PGC_USERSET, &SQL_inheritance, true, NULL
	},
	{
		"australian_timezones", PGC_USERSET, &Australian_timezones, false, ClearDateCache
	},
	{
		"fixbtree", PGC_POSTMASTER, &FixBTree, true, NULL
	},
	{
		"password_encryption", PGC_USERSET, &Password_encryption, false, NULL
	},
	{
		"transform_null_equals", PGC_USERSET, &Transform_null_equals, false, NULL
	},

	{
		NULL, 0, NULL, false, NULL
	}
};


static struct config_int
			ConfigureNamesInt[] =
{
	{
		"geqo_threshold", PGC_USERSET, &geqo_rels,
		DEFAULT_GEQO_RELS, 2, INT_MAX, NULL, NULL
	},
	{
		"geqo_pool_size", PGC_USERSET, &Geqo_pool_size,
		DEFAULT_GEQO_POOL_SIZE, 0, MAX_GEQO_POOL_SIZE, NULL, NULL
	},
	{
		"geqo_effort", PGC_USERSET, &Geqo_effort,
		1, 1, INT_MAX, NULL, NULL
	},
	{
		"geqo_generations", PGC_USERSET, &Geqo_generations,
		0, 0, INT_MAX, NULL, NULL
	},
	{
		"geqo_random_seed", PGC_USERSET, &Geqo_random_seed,
		-1, INT_MIN, INT_MAX, NULL, NULL
	},

	{
		"deadlock_timeout", PGC_POSTMASTER, &DeadlockTimeout,
		1000, 0, INT_MAX, NULL, NULL
	},

#ifdef ENABLE_SYSLOG
	{
		"syslog", PGC_SIGHUP, &Use_syslog,
		0, 0, 2, NULL, NULL
	},
#endif

	/*
	 * Note: There is some postprocessing done in PostmasterMain() to make
	 * sure the buffers are at least twice the number of backends, so the
	 * constraints here are partially unused.
	 */
	{
		"max_connections", PGC_POSTMASTER, &MaxBackends,
		DEF_MAXBACKENDS, 1, INT_MAX, NULL, NULL
	},

	{
		"shared_buffers", PGC_POSTMASTER, &NBuffers,
		DEF_NBUFFERS, 16, INT_MAX, NULL, NULL
	},

	{
		"port", PGC_POSTMASTER, &PostPortNumber,
		DEF_PGPORT, 1, 65535, NULL, NULL
	},

	{
		"unix_socket_permissions", PGC_POSTMASTER, &Unix_socket_permissions,
		0777, 0000, 0777, NULL, NULL
	},

	{
		"sort_mem", PGC_USERSET, &SortMem,
		512, 4 * BLCKSZ / 1024, INT_MAX, NULL, NULL
	},

	{
		"vacuum_mem", PGC_USERSET, &VacuumMem,
		8192, 1024, INT_MAX, NULL, NULL
	},

	{
		"max_files_per_process", PGC_BACKEND, &max_files_per_process,
		1000, 25, INT_MAX, NULL, NULL
	},

	{
		"debug_level", PGC_USERSET, &DebugLvl,
		0, 0, 16, NULL, NULL
	},

#ifdef LOCK_DEBUG
	{
		"trace_lock_oidmin", PGC_SUSET, &Trace_lock_oidmin,
		BootstrapObjectIdData, 1, INT_MAX, NULL, NULL
	},
	{
		"trace_lock_table", PGC_SUSET, &Trace_lock_table,
		0, 0, INT_MAX, NULL, NULL
	},
#endif
	{
		"max_expr_depth", PGC_USERSET, &max_expr_depth,
		DEFAULT_MAX_EXPR_DEPTH, 10, INT_MAX, NULL, NULL
	},

	{
		"max_fsm_relations", PGC_POSTMASTER, &MaxFSMRelations,
		100, 10, INT_MAX, NULL, NULL
	},
	{
		"max_fsm_pages", PGC_POSTMASTER, &MaxFSMPages,
		10000, 1000, INT_MAX, NULL, NULL
	},

	{
		"max_locks_per_transaction", PGC_POSTMASTER, &max_locks_per_xact,
		64, 10, INT_MAX, NULL, NULL
	},

	{
		"authentication_timeout", PGC_SIGHUP, &AuthenticationTimeout,
		60, 1, 600, NULL, NULL
	},

	{
		"pre_auth_delay", PGC_SIGHUP, &PreAuthDelay,
		0, 0, 60, NULL, NULL
	},

	{
		"checkpoint_segments", PGC_SIGHUP, &CheckPointSegments,
		3, 1, INT_MAX, NULL, NULL
	},

	{
		"checkpoint_timeout", PGC_SIGHUP, &CheckPointTimeout,
		300, 30, 3600, NULL, NULL
	},

	{
		"wal_buffers", PGC_POSTMASTER, &XLOGbuffers,
		8, 4, INT_MAX, NULL, NULL
	},

	{
		"wal_files", PGC_SIGHUP, &XLOGfiles,
		0, 0, 64, NULL, NULL
	},

	{
		"wal_debug", PGC_SUSET, &XLOG_DEBUG,
		0, 0, 16, NULL, NULL
	},

	{
		"commit_delay", PGC_USERSET, &CommitDelay,
		0, 0, 100000, NULL, NULL
	},

	{
		"commit_siblings", PGC_USERSET, &CommitSiblings,
		5, 1, 1000, NULL, NULL
	},

	{
		NULL, 0, NULL, 0, 0, 0, NULL, NULL
	}
};


static struct config_real
			ConfigureNamesReal[] =
{
	{
		"effective_cache_size", PGC_USERSET, &effective_cache_size,
		DEFAULT_EFFECTIVE_CACHE_SIZE, 0, DBL_MAX, NULL, NULL
	},
	{
		"random_page_cost", PGC_USERSET, &random_page_cost,
		DEFAULT_RANDOM_PAGE_COST, 0, DBL_MAX, NULL, NULL
	},
	{
		"cpu_tuple_cost", PGC_USERSET, &cpu_tuple_cost,
		DEFAULT_CPU_TUPLE_COST, 0, DBL_MAX, NULL, NULL
	},
	{
		"cpu_index_tuple_cost", PGC_USERSET, &cpu_index_tuple_cost,
		DEFAULT_CPU_INDEX_TUPLE_COST, 0, DBL_MAX, NULL, NULL
	},
	{
		"cpu_operator_cost", PGC_USERSET, &cpu_operator_cost,
		DEFAULT_CPU_OPERATOR_COST, 0, DBL_MAX, NULL, NULL
	},

	{
		"geqo_selection_bias", PGC_USERSET, &Geqo_selection_bias,
		DEFAULT_GEQO_SELECTION_BIAS, MIN_GEQO_SELECTION_BIAS,
		MAX_GEQO_SELECTION_BIAS, NULL, NULL
	},

	{
		NULL, 0, NULL, 0.0, 0.0, 0.0, NULL, NULL
	}
};


static struct config_string
			ConfigureNamesString[] =
{
	{
		"default_transaction_isolation", PGC_USERSET, &default_iso_level_string,
		"read committed", check_defaultxactisolevel, assign_defaultxactisolevel
	},

	{
		"dynamic_library_path", PGC_SUSET, &Dynamic_library_path,
		"$libdir", NULL, NULL
	},

	{
		"krb_server_keyfile", PGC_POSTMASTER, &pg_krb_server_keyfile,
		PG_KRB_SRVTAB, NULL, NULL
	},

#ifdef ENABLE_SYSLOG
	{
		"syslog_facility", PGC_POSTMASTER, &Syslog_facility,
		"LOCAL0", check_facility, NULL
	},
	{
		"syslog_ident", PGC_POSTMASTER, &Syslog_ident,
		"postgres", NULL, NULL
	},
#endif

	{
		"unix_socket_group", PGC_POSTMASTER, &Unix_socket_group,
		"", NULL, NULL
	},

	{
		"unix_socket_directory", PGC_POSTMASTER, &UnixSocketDir,
		"", NULL, NULL
	},

	{
		"virtual_host", PGC_POSTMASTER, &VirtualHost,
		"", NULL, NULL
	},

	{
		"wal_sync_method", PGC_SIGHUP, &XLOG_sync_method,
		XLOG_sync_method_default, check_xlog_sync_method,
		assign_xlog_sync_method
	},

	{
		NULL, 0, NULL, NULL, NULL, NULL
	}
};

/******** end of options list ********/



/*
 * Look up option NAME. If it exists, return it's data type, else
 * PGC_NONE (zero). If record is not NULL, store the description of
 * the option there.
 */
static enum config_type
find_option(const char *name, struct config_generic ** record)
{
	int			i;

	Assert(name);

	for (i = 0; ConfigureNamesBool[i].name; i++)
		if (strcasecmp(ConfigureNamesBool[i].name, name) == 0)
		{
			if (record)
				*record = (struct config_generic *) & ConfigureNamesBool[i];
			return PGC_BOOL;
		}

	for (i = 0; ConfigureNamesInt[i].name; i++)
		if (strcasecmp(ConfigureNamesInt[i].name, name) == 0)
		{
			if (record)
				*record = (struct config_generic *) & ConfigureNamesInt[i];
			return PGC_INT;
		}

	for (i = 0; ConfigureNamesReal[i].name; i++)
		if (strcasecmp(ConfigureNamesReal[i].name, name) == 0)
		{
			if (record)
				*record = (struct config_generic *) & ConfigureNamesReal[i];
			return PGC_REAL;
		}

	for (i = 0; ConfigureNamesString[i].name; i++)
		if (strcasecmp(ConfigureNamesString[i].name, name) == 0)
		{
			if (record)
				*record = (struct config_generic *) & ConfigureNamesString[i];
			return PGC_STRING;
		}

	return PGC_NONE;
}



/*
 * Reset all options to their specified default values.  Must be called
 * with isStartup = true at program startup.  May be called later with
 * isStartup = false to reset all resettable options.
 */
void
ResetAllOptions(bool isStartup)
{
	int			i;

	for (i = 0; ConfigureNamesBool[i].name; i++)
	{
		struct config_bool *conf = &ConfigureNamesBool[i];

		if (isStartup ||
			conf->context == PGC_SUSET || conf->context == PGC_USERSET)
		{
			if (conf->assign_hook)
				(conf->assign_hook) (conf->default_val);
			*conf->variable = conf->default_val;
		}
	}

	for (i = 0; ConfigureNamesInt[i].name; i++)
	{
		struct config_int *conf = &ConfigureNamesInt[i];

		if (isStartup ||
			conf->context == PGC_SUSET || conf->context == PGC_USERSET)
		{
			if (conf->assign_hook)
				(conf->assign_hook) (conf->default_val);
			*conf->variable = conf->default_val;
		}
	}

	for (i = 0; ConfigureNamesReal[i].name; i++)
	{
		struct config_real *conf = &ConfigureNamesReal[i];

		if (isStartup ||
			conf->context == PGC_SUSET || conf->context == PGC_USERSET)
		{
			if (conf->assign_hook)
				(conf->assign_hook) (conf->default_val);
			*conf->variable = conf->default_val;
		}
	}

	for (i = 0; ConfigureNamesString[i].name; i++)
	{
		struct config_string *conf = &ConfigureNamesString[i];

		if (isStartup ||
			conf->context == PGC_SUSET || conf->context == PGC_USERSET)
		{
			char	   *str = NULL;

			if (conf->default_val == NULL &&
				conf->boot_default_val)
			{
				str = strdup(conf->boot_default_val);
				if (str == NULL)
					elog(ERROR, "out of memory");
				conf->default_val = str;
			}
			if (conf->default_val)
			{
				str = strdup(conf->default_val);
				if (str == NULL)
					elog(ERROR, "out of memory");
			}
			if (conf->assign_hook)
				(conf->assign_hook) (str);
			if (*conf->variable)
				free(*conf->variable);
			*conf->variable = str;
		}
	}
}



/*
 * Try to interpret value as boolean value.  Valid values are: true,
 * false, yes, no, on, off, 1, 0.  If the string parses okay, return
 * true, else false.  If result is not NULL, return the parsing result
 * there.
 */
static bool
parse_bool(const char *value, bool *result)
{
	size_t		len = strlen(value);

	if (strncasecmp(value, "true", len) == 0)
	{
		if (result)
			*result = true;
	}
	else if (strncasecmp(value, "false", len) == 0)
	{
		if (result)
			*result = false;
	}

	else if (strncasecmp(value, "yes", len) == 0)
	{
		if (result)
			*result = true;
	}
	else if (strncasecmp(value, "no", len) == 0)
	{
		if (result)
			*result = false;
	}

	else if (strcasecmp(value, "on") == 0)
	{
		if (result)
			*result = true;
	}
	else if (strcasecmp(value, "off") == 0)
	{
		if (result)
			*result = false;
	}

	else if (strcasecmp(value, "1") == 0)
	{
		if (result)
			*result = true;
	}
	else if (strcasecmp(value, "0") == 0)
	{
		if (result)
			*result = false;
	}

	else
		return false;
	return true;
}



/*
 * Try to parse value as an integer.  The accepted formats are the
 * usual decimal, octal, or hexadecimal formats.  If the string parses
 * okay, return true, else false.  If result is not NULL, return the
 * value there.
 */
static bool
parse_int(const char *value, int *result)
{
	long		val;
	char	   *endptr;

	errno = 0;
	val = strtol(value, &endptr, 0);
	if (endptr == value || *endptr != '\0' || errno == ERANGE
#ifdef HAVE_LONG_INT_64
	/* if long > 32 bits, check for overflow of int4 */
		|| val != (long) ((int32) val)
#endif
		)
		return false;
	if (result)
		*result = (int) val;
	return true;
}



/*
 * Try to parse value as a floating point constant in the usual
 * format.	If the value parsed okay return true, else false.  If
 * result is not NULL, return the semantic value there.
 */
static bool
parse_real(const char *value, double *result)
{
	double		val;
	char	   *endptr;

	errno = 0;
	val = strtod(value, &endptr);
	if (endptr == value || *endptr != '\0' || errno == ERANGE)
		return false;
	if (result)
		*result = val;
	return true;
}



/*
 * Sets option `name' to given value. The value should be a string
 * which is going to be parsed and converted to the appropriate data
 * type. Parameter context should indicate in which context this
 * function is being called so it can apply the access restrictions
 * properly.
 *
 * If value is NULL, set the option to its default value. If the
 * parameter DoIt is false then don't really set the option but do all
 * the checks to see if it would work.
 *
 * If there is an error (non-existing option, invalid value) then an
 * elog(ERROR) is thrown *unless* this is called as part of the
 * configuration file re-read in the SIGHUP handler, in which case we
 * simply write the error message via elog(DEBUG) and return false. In
 * all other cases the function returns true. This is working around
 * the deficiencies in the elog mechanism, so don't blame me.
 *
 * See also SetConfigOption for an external interface.
 */
bool
set_config_option(const char *name, const char *value,
				  GucContext context, bool DoIt, bool makeDefault)
{
	struct config_generic *record;
	enum config_type type;
	int			elevel;

	elevel = (context == PGC_SIGHUP) ? DEBUG : ERROR;

	type = find_option(name, &record);
	if (type == PGC_NONE)
	{
		elog(elevel, "'%s' is not a valid option name", name);
		return false;
	}

	/*
	 * Check if the option can be set at this time. See guc.h for the
	 * precise rules. Note that we don't want to throw errors if we're in
	 * the SIGHUP context. In that case we just ignore the attempt.
	 */
	switch (record->context)
	{
		case PGC_POSTMASTER:
			if (context == PGC_SIGHUP)
				return true;
			if (context != PGC_POSTMASTER)
				elog(ERROR, "'%s' cannot be changed after server start", name);
			break;
		case PGC_SIGHUP:
			if (context != PGC_SIGHUP && context != PGC_POSTMASTER)
				elog(ERROR, "'%s' cannot be changed now", name);

			/*
			 * Hmm, the idea of the SIGHUP context is "ought to be global,
			 * but can be changed after postmaster start". But there's
			 * nothing that prevents a crafty administrator from sending
			 * SIGHUP signals to individual backends only.
			 */
			break;
		case PGC_BACKEND:
			if (context == PGC_SIGHUP)
			{
				/*
				 * If a PGC_BACKEND parameter is changed in the config
				 * file, we want to accept the new value in the postmaster
				 * (whence it will propagate to subsequently-started
				 * backends), but ignore it in existing backends.  This is
				 * a tad klugy, but necessary because we don't re-read the
				 * config file during backend start.
				 */
				if (IsUnderPostmaster)
					return true;
			}
			else if (context != PGC_BACKEND && context != PGC_POSTMASTER)
				elog(ERROR, "'%s' cannot be set after connection start", name);
			break;
		case PGC_SUSET:
			if (context == PGC_USERSET || context == PGC_BACKEND)
				elog(ERROR, "permission denied");
			break;
		case PGC_USERSET:
			/* always okay */
			break;
	}

	/*
	 * Evaluate value and set variable
	 */
	switch (type)
	{
		case PGC_BOOL:
			{
				struct config_bool *conf = (struct config_bool *) record;

				if (value)
				{
					bool		boolval;

					if (!parse_bool(value, &boolval))
					{
						elog(elevel, "option '%s' requires a boolean value", name);
						return false;
					}
					/* no parse_hook needed for booleans */
					if (DoIt)
					{
						if (conf->assign_hook)
							(conf->assign_hook) (boolval);
						*conf->variable = boolval;
						if (makeDefault)
							conf->default_val = boolval;
					}
				}
				else if (DoIt)
				{
					if (conf->assign_hook)
						(conf->assign_hook) (conf->default_val);
					*conf->variable = conf->default_val;
				}
				break;
			}

		case PGC_INT:
			{
				struct config_int *conf = (struct config_int *) record;

				if (value)
				{
					int			intval;

					if (!parse_int(value, &intval))
					{
						elog(elevel, "option '%s' expects an integer value", name);
						return false;
					}
					if (intval < conf->min || intval > conf->max)
					{
						elog(elevel, "option '%s' value %d is outside"
							 " of permissible range [%d .. %d]",
							 name, intval, conf->min, conf->max);
						return false;
					}
					if (conf->parse_hook && !(conf->parse_hook) (intval))
					{
						elog(elevel, "invalid value for option '%s': %d",
							 name, intval);
						return false;
					}
					if (DoIt)
					{
						if (conf->assign_hook)
							(conf->assign_hook) (intval);
						*conf->variable = intval;
						if (makeDefault)
							conf->default_val = intval;
					}
				}
				else if (DoIt)
				{
					if (conf->assign_hook)
						(conf->assign_hook) (conf->default_val);
					*conf->variable = conf->default_val;
				}
				break;
			}

		case PGC_REAL:
			{
				struct config_real *conf = (struct config_real *) record;

				if (value)
				{
					double		dval;

					if (!parse_real(value, &dval))
					{
						elog(elevel, "option '%s' expects a real number", name);
						return false;
					}
					if (dval < conf->min || dval > conf->max)
					{
						elog(elevel, "option '%s' value %g is outside"
							 " of permissible range [%g .. %g]",
							 name, dval, conf->min, conf->max);
						return false;
					}
					if (conf->parse_hook && !(conf->parse_hook) (dval))
					{
						elog(elevel, "invalid value for option '%s': %g",
							 name, dval);
						return false;
					}
					if (DoIt)
					{
						if (conf->assign_hook)
							(conf->assign_hook) (dval);
						*conf->variable = dval;
						if (makeDefault)
							conf->default_val = dval;
					}
				}
				else if (DoIt)
				{
					if (conf->assign_hook)
						(conf->assign_hook) (conf->default_val);
					*conf->variable = conf->default_val;
				}
				break;
			}

		case PGC_STRING:
			{
				struct config_string *conf = (struct config_string *) record;

				if (value)
				{
					if (conf->parse_hook && !(conf->parse_hook) (value))
					{
						elog(elevel, "invalid value for option '%s': '%s'",
							 name, value);
						return false;
					}
					if (DoIt)
					{
						char	   *str;

						str = strdup(value);
						if (str == NULL)
						{
							elog(elevel, "out of memory");
							return false;
						}
						if (conf->assign_hook)
							(conf->assign_hook) (str);
						if (*conf->variable)
							free(*conf->variable);
						*conf->variable = str;
						if (makeDefault)
						{
							str = strdup(value);
							if (str == NULL)
							{
								elog(elevel, "out of memory");
								return false;
							}
							if (conf->default_val)
								free(conf->default_val);
							conf->default_val = str;
						}
					}
				}
				else if (DoIt)
				{
					char	   *str;

					if (!conf->default_val && conf->boot_default_val)
					{
						str = strdup(conf->boot_default_val);
						if (str == NULL)
						{
							elog(elevel, "out of memory");
							return false;
						}
						conf->default_val = str;
					}
					str = strdup(conf->default_val);
					if (str == NULL)
					{
						elog(elevel, "out of memory");
						return false;
					}
					if (conf->assign_hook)
						(conf->assign_hook) (str);
					if (*conf->variable)
						free(*conf->variable);
					*conf->variable = str;
				}
				break;
			}

		default:;
	}
	return true;
}



/*
 * Set a config option to the given value. See also set_config_option,
 * this is just the wrapper to be called from the outside.
 */
void
SetConfigOption(const char *name, const char *value,
				GucContext context, bool makeDefault)
{
	(void) set_config_option(name, value, context, true, makeDefault);
}



/*
 * This is more or less the SHOW command. It returns a string with the
 * value of the option `name'. If the option doesn't exist, throw an
 * elog and don't return.
 *
 * The string is *not* allocated for modification and is really only
 * valid until the next call to configuration related functions.
 */
const char *
GetConfigOption(const char *name)
{
	struct config_generic *record;
	static char buffer[256];
	enum config_type opttype;

	opttype = find_option(name, &record);
	if (opttype == PGC_NONE)
		elog(ERROR, "Option '%s' is not recognized", name);

	switch (opttype)
	{
		case PGC_BOOL:
			return *((struct config_bool *) record)->variable ? "on" : "off";

		case PGC_INT:
			snprintf(buffer, sizeof(buffer), "%d",
					 *((struct config_int *) record)->variable);
			return buffer;

		case PGC_REAL:
			snprintf(buffer, sizeof(buffer), "%g",
					 *((struct config_real *) record)->variable);
			return buffer;

		case PGC_STRING:
			return *((struct config_string *) record)->variable;

		default:
			;
	}
	return NULL;
}

static void
_ShowOption(enum config_type opttype, struct config_generic * record)
{
	char		buffer[256];
	char	   *val;

	switch (opttype)
	{
		case PGC_BOOL:
			val = *((struct config_bool *) record)->variable ? "on" : "off";
			break;

		case PGC_INT:
			snprintf(buffer, sizeof(buffer), "%d",
					 *((struct config_int *) record)->variable);
			val = buffer;
			break;

		case PGC_REAL:
			snprintf(buffer, sizeof(buffer), "%g",
					 *((struct config_real *) record)->variable);
			val = buffer;
			break;

		case PGC_STRING:
			val = strlen(*((struct config_string *) record)->variable) != 0 ?
				*((struct config_string *) record)->variable : "unset";
			break;

		default:
			val = "???";
	}
	elog(NOTICE, "%s is %s", record->name, val);
}

void
ShowAllGUCConfig(void)
{
	int			i;

	for (i = 0; ConfigureNamesBool[i].name; i++)
		_ShowOption(PGC_BOOL, (struct config_generic *) & ConfigureNamesBool[i]);

	for (i = 0; ConfigureNamesInt[i].name; i++)
		_ShowOption(PGC_INT, (struct config_generic *) & ConfigureNamesInt[i]);

	for (i = 0; ConfigureNamesReal[i].name; i++)
		_ShowOption(PGC_REAL, (struct config_generic *) & ConfigureNamesReal[i]);

	for (i = 0; ConfigureNamesString[i].name; i++)
		_ShowOption(PGC_STRING, (struct config_generic *) & ConfigureNamesString[i]);
}





/*
 * A little "long argument" simulation, although not quite GNU
 * compliant. Takes a string of the form "some-option=some value" and
 * returns name = "some_option" and value = "some value" in malloc'ed
 * storage. Note that '-' is converted to '_' in the option name. If
 * there is no '=' in the input string then value will be NULL.
 */
void
ParseLongOption(const char *string, char **name, char **value)
{
	size_t		equal_pos;
	char	   *cp;

	AssertArg(string);
	AssertArg(name);
	AssertArg(value);

	equal_pos = strcspn(string, "=");

	if (string[equal_pos] == '=')
	{
		*name = malloc(equal_pos + 1);
		if (!*name)
			elog(FATAL, "out of memory");
		strncpy(*name, string, equal_pos);
		(*name)[equal_pos] = '\0';

		*value = strdup(&string[equal_pos + 1]);
		if (!*value)
			elog(FATAL, "out of memory");
	}
	else
	{
		/* no equal sign in string */
		*name = strdup(string);
		if (!*name)
			elog(FATAL, "out of memory");
		*value = NULL;
	}

	for (cp = *name; *cp; cp++)
		if (*cp == '-')
			*cp = '_';
}



#ifdef ENABLE_SYSLOG

static bool
check_facility(const char *facility)
{
	if (strcasecmp(facility, "LOCAL0") == 0)
		return true;
	if (strcasecmp(facility, "LOCAL1") == 0)
		return true;
	if (strcasecmp(facility, "LOCAL2") == 0)
		return true;
	if (strcasecmp(facility, "LOCAL3") == 0)
		return true;
	if (strcasecmp(facility, "LOCAL4") == 0)
		return true;
	if (strcasecmp(facility, "LOCAL5") == 0)
		return true;
	if (strcasecmp(facility, "LOCAL6") == 0)
		return true;
	if (strcasecmp(facility, "LOCAL7") == 0)
		return true;
	return false;
}
#endif



static bool
check_defaultxactisolevel(const char *value)
{
	return (strcasecmp(value, "read committed") == 0
			|| strcasecmp(value, "serializable") == 0)
		? true : false;
}


static void
assign_defaultxactisolevel(const char *value)
{
	if (strcasecmp(value, "serializable") == 0)
		DefaultXactIsoLevel = XACT_SERIALIZABLE;
	else if (strcasecmp(value, "read committed") == 0)
		DefaultXactIsoLevel = XACT_READ_COMMITTED;
	else
		elog(ERROR, "bogus transaction isolation level");
}
