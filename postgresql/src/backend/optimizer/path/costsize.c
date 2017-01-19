/*-------------------------------------------------------------------------
 *
 * costsize.c
 *	  Routines to compute (and set) relation sizes and path costs
 *
 * Path costs are measured in units of disk accesses: one sequential page
 * fetch has cost 1.  All else is scaled relative to a page fetch, using
 * the scaling parameters
 *
 *	random_page_cost	Cost of a non-sequential page fetch
 *	cpu_tuple_cost		Cost of typical CPU time to process a tuple
 *	cpu_index_tuple_cost  Cost of typical CPU time to process an index tuple
 *	cpu_operator_cost	Cost of CPU time to process a typical WHERE operator
 *
 * We also use a rough estimate "effective_cache_size" of the number of
 * disk pages in Postgres + OS-level disk cache.  (We can't simply use
 * NBuffers for this purpose because that would ignore the effects of
 * the kernel's disk cache.)
 *
 * Obviously, taking constants for these values is an oversimplification,
 * but it's tough enough to get any useful estimates even at this level of
 * detail.	Note that all of these parameters are user-settable, in case
 * the default values are drastically off for a particular platform.
 *
 * We compute two separate costs for each path:
 *		total_cost: total estimated cost to fetch all tuples
 *		startup_cost: cost that is expended before first tuple is fetched
 * In some scenarios, such as when there is a LIMIT or we are implementing
 * an EXISTS(...) sub-select, it is not necessary to fetch all tuples of the
 * path's result.  A caller can estimate the cost of fetching a partial
 * result by interpolating between startup_cost and total_cost.  In detail:
 *		actual_cost = startup_cost +
 *			(total_cost - startup_cost) * tuples_to_fetch / path->parent->rows;
 * Note that a base relation's rows count (and, by extension, plan_rows for
 * plan nodes below the LIMIT node) are set without regard to any LIMIT, so
 * that this equation works properly.  (Also, these routines guarantee not to
 * set the rows count to zero, so there will be no zero divide.)  The LIMIT is
 * applied as a top-level plan node.
 *
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/optimizer/path/costsize.c,v 1.2 2004/07/05 08:55:29 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <math.h>

#include "catalog/pg_statistic.h"
#include "executor/nodeHash.h"
#include "miscadmin.h"
#include "optimizer/clauses.h"
#include "optimizer/cost.h"
#include "optimizer/pathnode.h"
#include "parser/parsetree.h"
#include "utils/lsyscache.h"
#include "utils/syscache.h"


#define LOG2(x)  (log(x) / 0.693147180559945)
#define LOG6(x)  (log(x) / 1.79175946922805)


double		effective_cache_size = DEFAULT_EFFECTIVE_CACHE_SIZE;
double		random_page_cost = DEFAULT_RANDOM_PAGE_COST;
double		cpu_tuple_cost = DEFAULT_CPU_TUPLE_COST;
double		cpu_index_tuple_cost = DEFAULT_CPU_INDEX_TUPLE_COST;
double		cpu_operator_cost = DEFAULT_CPU_OPERATOR_COST;

Cost		disable_cost = 100000000.0;

bool		enable_seqscan = true;
bool		enable_indexscan = true;
bool		enable_tidscan = true;
bool		enable_sort = true;
bool		enable_nestloop = true;
bool		enable_mergejoin = true;
bool		enable_hashjoin = true;


static Selectivity estimate_hash_bucketsize(Query *root, Var *var);
static bool cost_qual_eval_walker(Node *node, Cost *total);
static Selectivity approx_selectivity(Query *root, List *quals);
static void set_rel_width(Query *root, RelOptInfo *rel);
static double relation_byte_size(double tuples, int width);
static double page_size(double tuples, int width);


/*
 * cost_seqscan
 *	  Determines and returns the cost of scanning a relation sequentially.
 *
 * Note: for historical reasons, this routine and the others in this module
 * use the passed result Path only to store their startup_cost and total_cost
 * results into.  All the input data they need is passed as separate
 * parameters, even though much of it could be extracted from the Path.
 */
void
cost_seqscan(Path *path, Query *root,
			 RelOptInfo *baserel)
{
	Cost		startup_cost = 0;
	Cost		run_cost = 0;
	Cost		cpu_per_tuple;

	/* Should only be applied to base relations */
	Assert(length(baserel->relids) == 1);
	Assert(!baserel->issubquery);

	if (!enable_seqscan)
		startup_cost += disable_cost;

	/*
	 * disk costs
	 *
	 * The cost of reading a page sequentially is 1.0, by definition. Note
	 * that the Unix kernel will typically do some amount of read-ahead
	 * optimization, so that this cost is less than the true cost of
	 * reading a page from disk.  We ignore that issue here, but must take
	 * it into account when estimating the cost of non-sequential
	 * accesses!
	 */
	run_cost += baserel->pages; /* sequential fetches with cost 1.0 */

	/* CPU costs */
	cpu_per_tuple = cpu_tuple_cost + baserel->baserestrictcost;
	run_cost += cpu_per_tuple * baserel->tuples;

	path->startup_cost = startup_cost;
	path->total_cost = startup_cost + run_cost;
}

/*
 * cost_nonsequential_access
 *	  Estimate the cost of accessing one page at random from a relation
 *	  (or sort temp file) of the given size in pages.
 *
 * The simplistic model that the cost is random_page_cost is what we want
 * to use for large relations; but for small ones that is a serious
 * overestimate because of the effects of caching.	This routine tries to
 * account for that.
 *
 * Unfortunately we don't have any good way of estimating the effective cache
 * size we are working with --- we know that Postgres itself has NBuffers
 * internal buffers, but the size of the kernel's disk cache is uncertain,
 * and how much of it we get to use is even less certain.  We punt the problem
 * for now by assuming we are given an effective_cache_size parameter.
 *
 * Given a guesstimated cache size, we estimate the actual I/O cost per page
 * with the entirely ad-hoc equations:
 *	for rel_size <= effective_cache_size:
 *		1 + (random_page_cost/2-1) * (rel_size/effective_cache_size) ** 2
 *	for rel_size >= effective_cache_size:
 *		random_page_cost * (1 - (effective_cache_size/rel_size)/2)
 * These give the right asymptotic behavior (=> 1.0 as rel_size becomes
 * small, => random_page_cost as it becomes large) and meet in the middle
 * with the estimate that the cache is about 50% effective for a relation
 * of the same size as effective_cache_size.  (XXX this is probably all
 * wrong, but I haven't been able to find any theory about how effective
 * a disk cache should be presumed to be.)
 */
static Cost
cost_nonsequential_access(double relpages)
{
	double		relsize;

	/* don't crash on bad input data */
	if (relpages <= 0.0 || effective_cache_size <= 0.0)
		return random_page_cost;

	relsize = relpages / effective_cache_size;

	if (relsize >= 1.0)
		return random_page_cost * (1.0 - 0.5 / relsize);
	else
		return 1.0 + (random_page_cost * 0.5 - 1.0) * relsize * relsize;
}

/*
 * cost_index
 *	  Determines and returns the cost of scanning a relation using an index.
 *
 *	  NOTE: an indexscan plan node can actually represent several passes,
 *	  but here we consider the cost of just one pass.
 *
 * 'root' is the query root
 * 'baserel' is the base relation the index is for
 * 'index' is the index to be used
 * 'indexQuals' is the list of applicable qual clauses (implicit AND semantics)
 * 'is_injoin' is T if we are considering using the index scan as the inside
 *		of a nestloop join (hence, some of the indexQuals are join clauses)
 *
 * NOTE: 'indexQuals' must contain only clauses usable as index restrictions.
 * Any additional quals evaluated as qpquals may reduce the number of returned
 * tuples, but they won't reduce the number of tuples we have to fetch from
 * the table, so they don't reduce the scan cost.
 */
void
cost_index(Path *path, Query *root,
		   RelOptInfo *baserel,
		   IndexOptInfo *index,
		   List *indexQuals,
		   bool is_injoin)
{
	Cost		startup_cost = 0;
	Cost		run_cost = 0;
	Cost		indexStartupCost;
	Cost		indexTotalCost;
	Selectivity indexSelectivity;
	double		indexCorrelation,
				csquared;
	Cost		min_IO_cost,
				max_IO_cost;
	Cost		cpu_per_tuple;
	double		tuples_fetched;
	double		pages_fetched;
	double		T,
				b;

	/* Should only be applied to base relations */
	Assert(IsA(baserel, RelOptInfo) &&IsA(index, IndexOptInfo));
	Assert(length(baserel->relids) == 1);
	Assert(!baserel->issubquery);

	if (!enable_indexscan && !is_injoin)
		startup_cost += disable_cost;

	/*
	 * Call index-access-method-specific code to estimate the processing
	 * cost for scanning the index, as well as the selectivity of the
	 * index (ie, the fraction of main-table tuples we will have to
	 * retrieve) and its correlation to the main-table tuple order.
	 */
	OidFunctionCall8(index->amcostestimate,
					 PointerGetDatum(root),
					 PointerGetDatum(baserel),
					 PointerGetDatum(index),
					 PointerGetDatum(indexQuals),
					 PointerGetDatum(&indexStartupCost),
					 PointerGetDatum(&indexTotalCost),
					 PointerGetDatum(&indexSelectivity),
					 PointerGetDatum(&indexCorrelation));

	/* all costs for touching index itself included here */
	startup_cost += indexStartupCost;
	run_cost += indexTotalCost - indexStartupCost;

	/*----------
	 * Estimate number of main-table tuples and pages fetched.
	 *
	 * When the index ordering is uncorrelated with the table ordering,
	 * we use an approximation proposed by Mackert and Lohman, "Index Scans
	 * Using a Finite LRU Buffer: A Validated I/O Model", ACM Transactions
	 * on Database Systems, Vol. 14, No. 3, September 1989, Pages 401-424.
	 * The Mackert and Lohman approximation is that the number of pages
	 * fetched is
	 *	PF =
	 *		min(2TNs/(2T+Ns), T)			when T <= b
	 *		2TNs/(2T+Ns)					when T > b and Ns <= 2Tb/(2T-b)
	 *		b + (Ns - 2Tb/(2T-b))*(T-b)/T	when T > b and Ns > 2Tb/(2T-b)
	 * where
	 *		T = # pages in table
	 *		N = # tuples in table
	 *		s = selectivity = fraction of table to be scanned
	 *		b = # buffer pages available (we include kernel space here)
	 *
	 * When the index ordering is exactly correlated with the table ordering
	 * (just after a CLUSTER, for example), the number of pages fetched should
	 * be just sT.	What's more, these will be sequential fetches, not the
	 * random fetches that occur in the uncorrelated case.	So, depending on
	 * the extent of correlation, we should estimate the actual I/O cost
	 * somewhere between s * T * 1.0 and PF * random_cost.	We currently
	 * interpolate linearly between these two endpoints based on the
	 * correlation squared (XXX is that appropriate?).
	 *
	 * In any case the number of tuples fetched is Ns.
	 *----------
	 */

	tuples_fetched = indexSelectivity * baserel->tuples;
	/* Don't believe estimates less than 1... */
	if (tuples_fetched < 1.0)
		tuples_fetched = 1.0;

	/* This part is the Mackert and Lohman formula */

	T = (baserel->pages > 1) ? (double) baserel->pages : 1.0;
	b = (effective_cache_size > 1) ? effective_cache_size : 1.0;

	if (T <= b)
	{
		pages_fetched =
			(2.0 * T * tuples_fetched) / (2.0 * T + tuples_fetched);
		if (pages_fetched > T)
			pages_fetched = T;
	}
	else
	{
		double		lim;

		lim = (2.0 * T * b) / (2.0 * T - b);
		if (tuples_fetched <= lim)
		{
			pages_fetched =
				(2.0 * T * tuples_fetched) / (2.0 * T + tuples_fetched);
		}
		else
		{
			pages_fetched =
				b + (tuples_fetched - lim) * (T - b) / T;
		}
	}

	/*
	 * min_IO_cost corresponds to the perfectly correlated case
	 * (csquared=1), max_IO_cost to the perfectly uncorrelated case
	 * (csquared=0).  Note that we just charge random_page_cost per page
	 * in the uncorrelated case, rather than using
	 * cost_nonsequential_access, since we've already accounted for
	 * caching effects by using the Mackert model.
	 */
	min_IO_cost = ceil(indexSelectivity * T);
	max_IO_cost = pages_fetched * random_page_cost;

	/*
	 * Now interpolate based on estimated index order correlation to get
	 * total disk I/O cost for main table accesses.
	 */
	csquared = indexCorrelation * indexCorrelation;

	run_cost += max_IO_cost + csquared * (min_IO_cost - max_IO_cost);

	/*
	 * Estimate CPU costs per tuple.
	 *
	 * Normally the indexquals will be removed from the list of restriction
	 * clauses that we have to evaluate as qpquals, so we should subtract
	 * their costs from baserestrictcost.  XXX For a lossy index, not all
	 * the quals will be removed and so we really shouldn't subtract their
	 * costs; but detecting that seems more expensive than it's worth.
	 * Also, if we are doing a join then some of the indexquals are join
	 * clauses and shouldn't be subtracted.  Rather than work out exactly
	 * how much to subtract, we don't subtract anything.
	 */
	cpu_per_tuple = cpu_tuple_cost + baserel->baserestrictcost;

	if (!is_injoin)
		cpu_per_tuple -= cost_qual_eval(indexQuals);

	run_cost += cpu_per_tuple * tuples_fetched;

	path->startup_cost = startup_cost;
	path->total_cost = startup_cost + run_cost;
}

/*
 * cost_tidscan
 *	  Determines and returns the cost of scanning a relation using TIDs.
 */
void
cost_tidscan(Path *path, Query *root,
			 RelOptInfo *baserel, List *tideval)
{
	Cost		startup_cost = 0;
	Cost		run_cost = 0;
	Cost		cpu_per_tuple;
	int			ntuples = length(tideval);

	if (!enable_tidscan)
		startup_cost += disable_cost;

	/* disk costs --- assume each tuple on a different page */
	run_cost += random_page_cost * ntuples;

	/* CPU costs */
	cpu_per_tuple = cpu_tuple_cost + baserel->baserestrictcost;
	run_cost += cpu_per_tuple * ntuples;

	path->startup_cost = startup_cost;
	path->total_cost = startup_cost + run_cost;
}

/*
 * cost_sort
 *	  Determines and returns the cost of sorting a relation.
 *
 * The cost of supplying the input data is NOT included; the caller should
 * add that cost to both startup and total costs returned from this routine!
 *
 * If the total volume of data to sort is less than SortMem, we will do
 * an in-memory sort, which requires no I/O and about t*log2(t) tuple
 * comparisons for t tuples.
 *
 * If the total volume exceeds SortMem, we switch to a tape-style merge
 * algorithm.  There will still be about t*log2(t) tuple comparisons in
 * total, but we will also need to write and read each tuple once per
 * merge pass.	We expect about ceil(log6(r)) merge passes where r is the
 * number of initial runs formed (log6 because tuplesort.c uses six-tape
 * merging).  Since the average initial run should be about twice SortMem,
 * we have
 *		disk traffic = 2 * relsize * ceil(log6(p / (2*SortMem)))
 *		cpu = comparison_cost * t * log2(t)
 *
 * The disk traffic is assumed to be half sequential and half random
 * accesses (XXX can't we refine that guess?)
 *
 * We charge two operator evals per tuple comparison, which should be in
 * the right ballpark in most cases.
 *
 * 'pathkeys' is a list of sort keys
 * 'tuples' is the number of tuples in the relation
 * 'width' is the average tuple width in bytes
 *
 * NOTE: some callers currently pass NIL for pathkeys because they
 * can't conveniently supply the sort keys.  Since this routine doesn't
 * currently do anything with pathkeys anyway, that doesn't matter...
 * but if it ever does, it should react gracefully to lack of key data.
 */
void
cost_sort(Path *path, Query *root,
		  List *pathkeys, double tuples, int width)
{
	Cost		startup_cost = 0;
	Cost		run_cost = 0;
	double		nbytes = relation_byte_size(tuples, width);
	long		sortmembytes = SortMem * 1024L;

	if (!enable_sort)
		startup_cost += disable_cost;

	/*
	 * We want to be sure the cost of a sort is never estimated as zero,
	 * even if passed-in tuple count is zero.  Besides, mustn't do
	 * log(0)...
	 */
	if (tuples < 2.0)
		tuples = 2.0;

	/*
	 * CPU costs
	 *
	 * Assume about two operator evals per tuple comparison and N log2 N
	 * comparisons
	 */
	startup_cost += 2.0 * cpu_operator_cost * tuples * LOG2(tuples);

	/* disk costs */
	if (nbytes > sortmembytes)
	{
		double		npages = ceil(nbytes / BLCKSZ);
		double		nruns = nbytes / (sortmembytes * 2);
		double		log_runs = ceil(LOG6(nruns));
		double		npageaccesses;

		if (log_runs < 1.0)
			log_runs = 1.0;
		npageaccesses = 2.0 * npages * log_runs;
		/* Assume half are sequential (cost 1), half are not */
		startup_cost += npageaccesses *
			(1.0 + cost_nonsequential_access(npages)) * 0.5;
	}

	/*
	 * Note: should we bother to assign a nonzero run_cost to reflect the
	 * overhead of extracting tuples from the sort result?	Probably not
	 * worth worrying about.
	 */
	path->startup_cost = startup_cost;
	path->total_cost = startup_cost + run_cost;
}


/*
 * cost_nestloop
 *	  Determines and returns the cost of joining two relations using the
 *	  nested loop algorithm.
 *
 * 'outer_path' is the path for the outer relation
 * 'inner_path' is the path for the inner relation
 * 'restrictlist' are the RestrictInfo nodes to be applied at the join
 */
void
cost_nestloop(Path *path, Query *root,
			  Path *outer_path,
			  Path *inner_path,
			  List *restrictlist)
{
	Cost		startup_cost = 0;
	Cost		run_cost = 0;
	Cost		cpu_per_tuple;
	double		ntuples;

	if (!enable_nestloop)
		startup_cost += disable_cost;

	/* cost of source data */

	/*
	 * NOTE: clearly, we must pay both outer and inner paths' startup_cost
	 * before we can start returning tuples, so the join's startup cost is
	 * their sum.  What's not so clear is whether the inner path's
	 * startup_cost must be paid again on each rescan of the inner path.
	 * This is not true if the inner path is materialized, but probably is
	 * true otherwise.	Since we don't yet have clean handling of the
	 * decision whether to materialize a path, we can't tell here which
	 * will happen.  As a compromise, charge 50% of the inner startup cost
	 * for each restart.
	 */
	startup_cost += outer_path->startup_cost + inner_path->startup_cost;
	run_cost += outer_path->total_cost - outer_path->startup_cost;
	run_cost += outer_path->parent->rows *
		(inner_path->total_cost - inner_path->startup_cost);
	if (outer_path->parent->rows > 1)
		run_cost += (outer_path->parent->rows - 1) *
			inner_path->startup_cost * 0.5;

	/*
	 * Number of tuples processed (not number emitted!).  If inner path is
	 * an indexscan, be sure to use its estimated output row count, which
	 * may be lower than the restriction-clause-only row count of its
	 * parent.
	 */
	if (IsA(inner_path, IndexPath))
		ntuples = ((IndexPath *) inner_path)->rows;
	else
		ntuples = inner_path->parent->rows;
	ntuples *= outer_path->parent->rows;

	/* CPU costs */
	cpu_per_tuple = cpu_tuple_cost + cost_qual_eval(restrictlist);
	run_cost += cpu_per_tuple * ntuples;

	path->startup_cost = startup_cost;
	path->total_cost = startup_cost + run_cost;
}

/*
 * cost_mergejoin
 *	  Determines and returns the cost of joining two relations using the
 *	  merge join algorithm.
 *
 * 'outer_path' is the path for the outer relation
 * 'inner_path' is the path for the inner relation
 * 'restrictlist' are the RestrictInfo nodes to be applied at the join
 * 'mergeclauses' are the RestrictInfo nodes to use as merge clauses
 *		(this should be a subset of the restrictlist)
 * 'outersortkeys' and 'innersortkeys' are lists of the keys to be used
 *				to sort the outer and inner relations, or NIL if no explicit
 *				sort is needed because the source path is already ordered
 */
void
cost_mergejoin(Path *path, Query *root,
			   Path *outer_path,
			   Path *inner_path,
			   List *restrictlist,
			   List *mergeclauses,
			   List *outersortkeys,
			   List *innersortkeys)
{
	Cost		startup_cost = 0;
	Cost		run_cost = 0;
	Cost		cpu_per_tuple;
	double		ntuples;
	Path		sort_path;		/* dummy for result of cost_sort */

	if (!enable_mergejoin)
		startup_cost += disable_cost;

	/* cost of source data */

	/*
	 * Note we are assuming that each source tuple is fetched just once,
	 * which is not right in the presence of equal keys.  If we had a way
	 * of estimating the proportion of equal keys, we could apply a
	 * correction factor...
	 */
	if (outersortkeys)			/* do we need to sort outer? */
	{
		startup_cost += outer_path->total_cost;
		cost_sort(&sort_path,
				  root,
				  outersortkeys,
				  outer_path->parent->rows,
				  outer_path->parent->width);
		startup_cost += sort_path.startup_cost;
		run_cost += sort_path.total_cost - sort_path.startup_cost;
	}
	else
	{
		startup_cost += outer_path->startup_cost;
		run_cost += outer_path->total_cost - outer_path->startup_cost;
	}

	if (innersortkeys)			/* do we need to sort inner? */
	{
		startup_cost += inner_path->total_cost;
		cost_sort(&sort_path,
				  root,
				  innersortkeys,
				  inner_path->parent->rows,
				  inner_path->parent->width);
		startup_cost += sort_path.startup_cost;
		run_cost += sort_path.total_cost - sort_path.startup_cost;
	}
	else
	{
		startup_cost += inner_path->startup_cost;
		run_cost += inner_path->total_cost - inner_path->startup_cost;
	}

	/*
	 * The number of tuple comparisons needed depends drastically on the
	 * number of equal keys in the two source relations, which we have no
	 * good way of estimating.	Somewhat arbitrarily, we charge one tuple
	 * comparison (one cpu_operator_cost) for each tuple in the two source
	 * relations.  This is probably a lower bound.
	 */
	run_cost += cpu_operator_cost *
		(outer_path->parent->rows + inner_path->parent->rows);

	/*
	 * For each tuple that gets through the mergejoin proper, we charge
	 * cpu_tuple_cost plus the cost of evaluating additional restriction
	 * clauses that are to be applied at the join.	It's OK to use an
	 * approximate selectivity here, since in most cases this is a minor
	 * component of the cost.
	 */
	ntuples = approx_selectivity(root, mergeclauses) *
		outer_path->parent->rows * inner_path->parent->rows;

	/* CPU costs */
	cpu_per_tuple = cpu_tuple_cost + cost_qual_eval(restrictlist);
	run_cost += cpu_per_tuple * ntuples;

	path->startup_cost = startup_cost;
	path->total_cost = startup_cost + run_cost;
}

/*
 * cost_hashjoin
 *	  Determines and returns the cost of joining two relations using the
 *	  hash join algorithm.
 *
 * 'outer_path' is the path for the outer relation
 * 'inner_path' is the path for the inner relation
 * 'restrictlist' are the RestrictInfo nodes to be applied at the join
 * 'hashclauses' is a list of the hash join clause (always a 1-element list)
 *		(this should be a subset of the restrictlist)
 */
void
cost_hashjoin(Path *path, Query *root,
			  Path *outer_path,
			  Path *inner_path,
			  List *restrictlist,
			  List *hashclauses)
{
	Cost		startup_cost = 0;
	Cost		run_cost = 0;
	Cost		cpu_per_tuple;
	double		ntuples;
	double		outerbytes = relation_byte_size(outer_path->parent->rows,
											  outer_path->parent->width);
	double		innerbytes = relation_byte_size(inner_path->parent->rows,
											  inner_path->parent->width);
	long		hashtablebytes = SortMem * 1024L;
	RestrictInfo *restrictinfo;
	Var		   *left,
			   *right;
	Selectivity innerbucketsize;

	if (!enable_hashjoin)
		startup_cost += disable_cost;

	/* cost of source data */
	startup_cost += outer_path->startup_cost;
	run_cost += outer_path->total_cost - outer_path->startup_cost;
	startup_cost += inner_path->total_cost;

	/* cost of computing hash function: must do it once per input tuple */
	startup_cost += cpu_operator_cost * inner_path->parent->rows;
	run_cost += cpu_operator_cost * outer_path->parent->rows;

	/*
	 * Determine bucketsize fraction for inner relation.  First we have to
	 * figure out which side of the hashjoin clause is the inner side.
	 */
	Assert(length(hashclauses) == 1);
	Assert(IsA(lfirst(hashclauses), RestrictInfo));
	restrictinfo = (RestrictInfo *) lfirst(hashclauses);
	/* these must be OK, since check_hashjoinable accepted the clause */
	left = get_leftop(restrictinfo->clause);
	right = get_rightop(restrictinfo->clause);

	/*
	 * Since we tend to visit the same clauses over and over when planning
	 * a large query, we cache the bucketsize estimate in the RestrictInfo
	 * node to avoid repeated lookups of statistics.
	 */
	if (intMember(right->varno, inner_path->parent->relids))
	{
		/* righthand side is inner */
		innerbucketsize = restrictinfo->right_bucketsize;
		if (innerbucketsize < 0)
		{
			/* not cached yet */
			innerbucketsize = estimate_hash_bucketsize(root, right);
			restrictinfo->right_bucketsize = innerbucketsize;
		}
	}
	else
	{
		Assert(intMember(left->varno, inner_path->parent->relids));
		/* lefthand side is inner */
		innerbucketsize = restrictinfo->left_bucketsize;
		if (innerbucketsize < 0)
		{
			/* not cached yet */
			innerbucketsize = estimate_hash_bucketsize(root, left);
			restrictinfo->left_bucketsize = innerbucketsize;
		}
	}

	/*
	 * The number of tuple comparisons needed is the number of outer
	 * tuples times the typical number of tuples in a hash bucket, which
	 * is the inner relation size times its bucketsize fraction. We charge
	 * one cpu_operator_cost per tuple comparison.
	 */
	run_cost += cpu_operator_cost * outer_path->parent->rows *
		ceil(inner_path->parent->rows * innerbucketsize);

	/*
	 * For each tuple that gets through the hashjoin proper, we charge
	 * cpu_tuple_cost plus the cost of evaluating additional restriction
	 * clauses that are to be applied at the join.	It's OK to use an
	 * approximate selectivity here, since in most cases this is a minor
	 * component of the cost.
	 */
	ntuples = approx_selectivity(root, hashclauses) *
		outer_path->parent->rows * inner_path->parent->rows;

	/* CPU costs */
	cpu_per_tuple = cpu_tuple_cost + cost_qual_eval(restrictlist);
	run_cost += cpu_per_tuple * ntuples;

	/*
	 * if inner relation is too big then we will need to "batch" the join,
	 * which implies writing and reading most of the tuples to disk an
	 * extra time.	Charge one cost unit per page of I/O (correct since it
	 * should be nice and sequential...).  Writing the inner rel counts as
	 * startup cost, all the rest as run cost.
	 */
	if (innerbytes > hashtablebytes)
	{
		double		outerpages = page_size(outer_path->parent->rows,
										   outer_path->parent->width);
		double		innerpages = page_size(inner_path->parent->rows,
										   inner_path->parent->width);

		startup_cost += innerpages;
		run_cost += innerpages + 2 * outerpages;
	}

	/*
	 * Bias against putting larger relation on inside.	We don't want an
	 * absolute prohibition, though, since larger relation might have
	 * better bucketsize --- and we can't trust the size estimates
	 * unreservedly, anyway.  Instead, inflate the startup cost by the
	 * square root of the size ratio.  (Why square root?  No real good
	 * reason, but it seems reasonable...)
	 */
	if (innerbytes > outerbytes && outerbytes > 0)
		startup_cost *= sqrt(innerbytes / outerbytes);

	path->startup_cost = startup_cost;
	path->total_cost = startup_cost + run_cost;
}

/*
 * Estimate hash bucketsize fraction (ie, number of entries in a bucket
 * divided by total tuples in relation) if the specified Var is used
 * as a hash key.
 *
 * XXX This is really pretty bogus since we're effectively assuming that the
 * distribution of hash keys will be the same after applying restriction
 * clauses as it was in the underlying relation.  However, we are not nearly
 * smart enough to figure out how the restrict clauses might change the
 * distribution, so this will have to do for now.
 *
 * We can get the number of buckets the executor will use for the given
 * input relation.	If the data were perfectly distributed, with the same
 * number of tuples going into each available bucket, then the bucketsize
 * fraction would be 1/nbuckets.  But this happy state of affairs will occur
 * only if (a) there are at least nbuckets distinct data values, and (b)
 * we have a not-too-skewed data distribution.	Otherwise the buckets will
 * be nonuniformly occupied.  If the other relation in the join has a key
 * distribution similar to this one's, then the most-loaded buckets are
 * exactly those that will be probed most often.  Therefore, the "average"
 * bucket size for costing purposes should really be taken as something close
 * to the "worst case" bucket size.  We try to estimate this by adjusting the
 * fraction if there are too few distinct data values, and then scaling up
 * by the ratio of the most common value's frequency to the average frequency.
 *
 * If no statistics are available, use a default estimate of 0.1.  This will
 * discourage use of a hash rather strongly if the inner relation is large,
 * which is what we want.  We do not want to hash unless we know that the
 * inner rel is well-dispersed (or the alternatives seem much worse).
 */
static Selectivity
estimate_hash_bucketsize(Query *root, Var *var)
{
	Oid			relid;
	RelOptInfo *rel;
	int			virtualbuckets;
	int			physicalbuckets;
	int			numbatches;
	HeapTuple	tuple;
	Form_pg_statistic stats;
	double		estfract,
				ndistinct,
				mcvfreq,
				avgfreq;
	float4	   *numbers;
	int			nnumbers;

	/*
	 * Lookup info about var's relation and attribute; if none available,
	 * return default estimate.
	 */
	if (!IsA(var, Var))
		return 0.1;

	relid = getrelid(var->varno, root->rtable);
	if (relid == InvalidOid)
		return 0.1;

	rel = find_base_rel(root, var->varno);

	if (rel->tuples <= 0.0 || rel->rows <= 0.0)
		return 0.1;				/* ensure we can divide below */

	/* Get hash table size that executor would use for this relation */
	ExecChooseHashTableSize(rel->rows, rel->width,
							&virtualbuckets,
							&physicalbuckets,
							&numbatches);

	tuple = SearchSysCache(STATRELATT,
						   ObjectIdGetDatum(relid),
						   Int16GetDatum(var->varattno),
						   0, 0);
	if (!HeapTupleIsValid(tuple))
	{
		/*
		 * Perhaps the Var is a system attribute; if so, it will have no
		 * entry in pg_statistic, but we may be able to guess something
		 * about its distribution anyway.
		 */
		switch (var->varattno)
		{
			case ObjectIdAttributeNumber:
			case SelfItemPointerAttributeNumber:
				/* these are unique, so buckets should be well-distributed */
				return 1.0 / (double) virtualbuckets;
			case TableOidAttributeNumber:
				/* hashing this is a terrible idea... */
				return 1.0;
		}
		return 0.1;
	}
	stats = (Form_pg_statistic) GETSTRUCT(tuple);

	/*
	 * Obtain number of distinct data values in raw relation.
	 */
	ndistinct = stats->stadistinct;
	if (ndistinct < 0.0)
		ndistinct = -ndistinct * rel->tuples;

	if (ndistinct <= 0.0)		/* ensure we can divide */
	{
		ReleaseSysCache(tuple);
		return 0.1;
	}

	/* Also compute avg freq of all distinct data values in raw relation */
	avgfreq = (1.0 - stats->stanullfrac) / ndistinct;

	/*
	 * Adjust ndistinct to account for restriction clauses.  Observe we
	 * are assuming that the data distribution is affected uniformly by
	 * the restriction clauses!
	 *
	 * XXX Possibly better way, but much more expensive: multiply by
	 * selectivity of rel's restriction clauses that mention the target
	 * Var.
	 */
	ndistinct *= rel->rows / rel->tuples;

	/*
	 * Initial estimate of bucketsize fraction is 1/nbuckets as long as
	 * the number of buckets is less than the expected number of distinct
	 * values; otherwise it is 1/ndistinct.
	 */
	if (ndistinct > (double) virtualbuckets)
		estfract = 1.0 / (double) virtualbuckets;
	else
		estfract = 1.0 / ndistinct;

	/*
	 * Look up the frequency of the most common value, if available.
	 */
	mcvfreq = 0.0;

	if (get_attstatsslot(tuple, var->vartype, var->vartypmod,
						 STATISTIC_KIND_MCV, InvalidOid,
						 NULL, NULL, &numbers, &nnumbers))
	{
		/*
		 * The first MCV stat is for the most common value.
		 */
		if (nnumbers > 0)
			mcvfreq = numbers[0];
		free_attstatsslot(var->vartype, NULL, 0,
						  numbers, nnumbers);
	}

	/*
	 * Adjust estimated bucketsize upward to account for skewed
	 * distribution.
	 */
	if (avgfreq > 0.0 && mcvfreq > avgfreq)
		estfract *= mcvfreq / avgfreq;

	ReleaseSysCache(tuple);

	return (Selectivity) estfract;
}


/*
 * cost_qual_eval
 *		Estimate the CPU cost of evaluating a WHERE clause (once).
 *		The input can be either an implicitly-ANDed list of boolean
 *		expressions, or a list of RestrictInfo nodes.
 */
Cost
cost_qual_eval(List *quals)
{
	Cost		total = 0;
	List	   *l;

	/* We don't charge any cost for the implicit ANDing at top level ... */

	foreach(l, quals)
	{
		Node	   *qual = (Node *) lfirst(l);

		/*
		 * RestrictInfo nodes contain an eval_cost field reserved for this
		 * routine's use, so that it's not necessary to evaluate the qual
		 * clause's cost more than once.  If the clause's cost hasn't been
		 * computed yet, the field will contain -1.
		 */
		if (qual && IsA(qual, RestrictInfo))
		{
			RestrictInfo *restrictinfo = (RestrictInfo *) qual;

			if (restrictinfo->eval_cost < 0)
			{
				restrictinfo->eval_cost = 0;
				cost_qual_eval_walker((Node *) restrictinfo->clause,
									  &restrictinfo->eval_cost);
			}
			total += restrictinfo->eval_cost;
		}
		else
		{
			/* If it's a bare expression, must always do it the hard way */
			cost_qual_eval_walker(qual, &total);
		}
	}
	return total;
}

static bool
cost_qual_eval_walker(Node *node, Cost *total)
{
	if (node == NULL)
		return false;

	/*
	 * Our basic strategy is to charge one cpu_operator_cost for each
	 * operator or function node in the given tree.  Vars and Consts are
	 * charged zero, and so are boolean operators (AND, OR, NOT).
	 * Simplistic, but a lot better than no model at all.
	 *
	 * Should we try to account for the possibility of short-circuit
	 * evaluation of AND/OR?
	 */
	if (IsA(node, Expr))
	{
		Expr	   *expr = (Expr *) node;

		switch (expr->opType)
		{
			case OP_EXPR:
			case FUNC_EXPR:
				*total += cpu_operator_cost;
				break;
			case OR_EXPR:
			case AND_EXPR:
			case NOT_EXPR:
				break;
			case SUBPLAN_EXPR:

				/*
				 * A subplan node in an expression indicates that the
				 * subplan will be executed on each evaluation, so charge
				 * accordingly. (We assume that sub-selects that can be
				 * executed as InitPlans have already been removed from
				 * the expression.)
				 *
				 * NOTE: this logic should agree with the estimates used by
				 * make_subplan() in plan/subselect.c.
				 */
				{
					SubPlan    *subplan = (SubPlan *) expr->oper;
					Plan	   *plan = subplan->plan;
					Cost		subcost;

					if (subplan->sublink->subLinkType == EXISTS_SUBLINK)
					{
						/* we only need to fetch 1 tuple */
						subcost = plan->startup_cost +
							(plan->total_cost - plan->startup_cost) / plan->plan_rows;
					}
					else if (subplan->sublink->subLinkType == ALL_SUBLINK ||
							 subplan->sublink->subLinkType == ANY_SUBLINK)
					{
						/* assume we need 50% of the tuples */
						subcost = plan->startup_cost +
							0.50 * (plan->total_cost - plan->startup_cost);
						/* XXX what if subplan has been materialized? */
					}
					else
					{
						/* assume we need all tuples */
						subcost = plan->total_cost;
					}
					*total += subcost;
				}
				break;
		}
		/* fall through to examine args of Expr node */
	}
	return expression_tree_walker(node, cost_qual_eval_walker,
								  (void *) total);
}


/*
 * approx_selectivity
 *		Quick-and-dirty estimation of clause selectivities.
 *		The input can be either an implicitly-ANDed list of boolean
 *		expressions, or a list of RestrictInfo nodes (typically the latter).
 *
 * The "quick" part comes from caching the selectivity estimates so we can
 * avoid recomputing them later.  (Since the same clauses are typically
 * examined over and over in different possible join trees, this makes a
 * big difference.)
 *
 * The "dirty" part comes from the fact that the selectivities of multiple
 * clauses are estimated independently and multiplied together.  Currently,
 * clauselist_selectivity can seldom do any better than that anyhow, but
 * someday it might be smarter.
 *
 * Since we are only using the results to estimate how many potential
 * output tuples are generated and passed through qpqual checking, it
 * seems OK to live with the approximation.
 */
static Selectivity
approx_selectivity(Query *root, List *quals)
{
	Selectivity total = 1.0;
	List	   *l;

	foreach(l, quals)
	{
		Node	   *qual = (Node *) lfirst(l);
		Selectivity selec;

		/*
		 * RestrictInfo nodes contain a this_selec field reserved for this
		 * routine's use, so that it's not necessary to evaluate the qual
		 * clause's selectivity more than once.  If the clause's
		 * selectivity hasn't been computed yet, the field will contain
		 * -1.
		 */
		if (qual && IsA(qual, RestrictInfo))
		{
			RestrictInfo *restrictinfo = (RestrictInfo *) qual;

			if (restrictinfo->this_selec < 0)
				restrictinfo->this_selec =
					clause_selectivity(root,
									   (Node *) restrictinfo->clause,
									   0);
			selec = restrictinfo->this_selec;
		}
		else
		{
			/* If it's a bare expression, must always do it the hard way */
			selec = clause_selectivity(root, qual, 0);
		}
		total *= selec;
	}
	return total;
}


/*
 * set_baserel_size_estimates
 *		Set the size estimates for the given base relation.
 *
 * The rel's targetlist and restrictinfo list must have been constructed
 * already.
 *
 * We set the following fields of the rel node:
 *	rows: the estimated number of output tuples (after applying
 *		  restriction clauses).
 *	width: the estimated average output tuple width in bytes.
 *	baserestrictcost: estimated cost of evaluating baserestrictinfo clauses.
 */
void
set_baserel_size_estimates(Query *root, RelOptInfo *rel)
{
	/* Should only be applied to base relations */
	Assert(length(rel->relids) == 1);

	rel->rows = rel->tuples *
		restrictlist_selectivity(root,
								 rel->baserestrictinfo,
								 lfirsti(rel->relids));

	/*
	 * Force estimate to be at least one row, to make explain output look
	 * better and to avoid possible divide-by-zero when interpolating
	 * cost.
	 */
	if (rel->rows < 1.0)
		rel->rows = 1.0;

	rel->baserestrictcost = cost_qual_eval(rel->baserestrictinfo);

	set_rel_width(root, rel);
}

/*
 * set_joinrel_size_estimates
 *		Set the size estimates for the given join relation.
 *
 * The rel's targetlist must have been constructed already, and a
 * restriction clause list that matches the given component rels must
 * be provided.
 *
 * Since there is more than one way to make a joinrel for more than two
 * base relations, the results we get here could depend on which component
 * rel pair is provided.  In theory we should get the same answers no matter
 * which pair is provided; in practice, since the selectivity estimation
 * routines don't handle all cases equally well, we might not.  But there's
 * not much to be done about it.  (Would it make sense to repeat the
 * calculations for each pair of input rels that's encountered, and somehow
 * average the results?  Probably way more trouble than it's worth.)
 *
 * We set the same relnode fields as set_baserel_size_estimates() does.
 */
void
set_joinrel_size_estimates(Query *root, RelOptInfo *rel,
						   RelOptInfo *outer_rel,
						   RelOptInfo *inner_rel,
						   JoinType jointype,
						   List *restrictlist)
{
	double		temp;

	/* Start with the Cartesian product */
	temp = outer_rel->rows * inner_rel->rows;

	/*
	 * Apply join restrictivity.  Note that we are only considering
	 * clauses that become restriction clauses at this join level; we are
	 * not double-counting them because they were not considered in
	 * estimating the sizes of the component rels.
	 */
	temp *= restrictlist_selectivity(root,
									 restrictlist,
									 0);

	/*
	 * If we are doing an outer join, take that into account: the output
	 * must be at least as large as the non-nullable input.  (Is there any
	 * chance of being even smarter?)
	 */
	switch (jointype)
	{
		case JOIN_INNER:
			break;
		case JOIN_LEFT:
			if (temp < outer_rel->rows)
				temp = outer_rel->rows;
			break;
		case JOIN_RIGHT:
			if (temp < inner_rel->rows)
				temp = inner_rel->rows;
			break;
		case JOIN_FULL:
			if (temp < outer_rel->rows)
				temp = outer_rel->rows;
			if (temp < inner_rel->rows)
				temp = inner_rel->rows;
			break;
		default:
			elog(ERROR, "set_joinrel_size_estimates: unsupported join type %d",
				 (int) jointype);
			break;
	}

	/*
	 * Force estimate to be at least one row, to make explain output look
	 * better and to avoid possible divide-by-zero when interpolating
	 * cost.
	 */
	if (temp < 1.0)
		temp = 1.0;

	rel->rows = temp;

	/*
	 * We could apply set_rel_width() to compute the output tuple width
	 * from scratch, but at present it's always just the sum of the input
	 * widths, so why work harder than necessary?  If relnode.c is ever
	 * taught to remove unneeded columns from join targetlists, go back to
	 * using set_rel_width here.
	 */
	rel->width = outer_rel->width + inner_rel->width;
}

/*
 * set_rel_width
 *		Set the estimated output width of the relation.
 *
 * NB: this works best on base relations because it prefers to look at
 * real Vars.  It will fail to make use of pg_statistic info when applied
 * to a subquery relation, even if the subquery outputs are simple vars
 * that we could have gotten info for.	Is it worth trying to be smarter
 * about subqueries?
 */
static void
set_rel_width(Query *root, RelOptInfo *rel)
{
	int32		tuple_width = 0;
	List	   *tllist;

	foreach(tllist, rel->targetlist)
	{
		TargetEntry *tle = (TargetEntry *) lfirst(tllist);
		int32		item_width;

		/*
		 * If it's a Var, try to get statistical info from pg_statistic.
		 */
		if (tle->expr && IsA(tle->expr, Var))
		{
			Var		   *var = (Var *) tle->expr;
			Oid			relid;

			relid = getrelid(var->varno, root->rtable);
			if (relid != InvalidOid)
			{
				item_width = get_attavgwidth(relid, var->varattno);
				if (item_width > 0)
				{
					tuple_width += item_width;
					continue;
				}
			}
		}

		/*
		 * Not a Var, or can't find statistics for it.  Estimate using
		 * just the type info.
		 */
		item_width = get_typavgwidth(tle->resdom->restype,
									 tle->resdom->restypmod);
		Assert(item_width > 0);
		tuple_width += item_width;
	}
	Assert(tuple_width >= 0);
	rel->width = tuple_width;
}

/*
 * relation_byte_size
 *	  Estimate the storage space in bytes for a given number of tuples
 *	  of a given width (size in bytes).
 */
static double
relation_byte_size(double tuples, int width)
{
	return tuples * ((double) MAXALIGN(width + sizeof(HeapTupleData)));
}

/*
 * page_size
 *	  Returns an estimate of the number of pages covered by a given
 *	  number of tuples of a given width (size in bytes).
 */
static double
page_size(double tuples, int width)
{
	return ceil(relation_byte_size(tuples, width) / BLCKSZ);
}
