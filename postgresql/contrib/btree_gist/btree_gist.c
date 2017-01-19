#include "postgres.h"

#include "access/gist.h"
#include "access/itup.h"
#include "access/nbtree.h"

#include "utils/palloc.h"
#include "utils/geo_decls.h"
#include "utils/elog.h"

typedef int (*CMPFUNC) (const void *a, const void *b);
typedef void (*BINARY_UNION) (Datum *, char *);

typedef struct intkey
{
	int4		lower;
	int4		upper;
}	INT4KEY;

typedef struct tskey
{
	Timestamp	lower;
	Timestamp	upper;
}	TSKEY;

/* used for sorting */
typedef struct rix
{
	int			index;
	char	   *r;
}	RIX;

/*
** int4key in/out
*/
PG_FUNCTION_INFO_V1(int4key_in);
PG_FUNCTION_INFO_V1(int4key_out);
Datum		int4key_in(PG_FUNCTION_ARGS);
Datum		int4key_out(PG_FUNCTION_ARGS);

/*
** tskey in/out
*/
PG_FUNCTION_INFO_V1(tskey_in);
PG_FUNCTION_INFO_V1(tskey_out);
Datum		tskey_in(PG_FUNCTION_ARGS);
Datum		tskey_out(PG_FUNCTION_ARGS);

/*
** int4 ops
*/
PG_FUNCTION_INFO_V1(gint4_compress);
PG_FUNCTION_INFO_V1(gint4_union);
PG_FUNCTION_INFO_V1(gint4_picksplit);
PG_FUNCTION_INFO_V1(gint4_consistent);
PG_FUNCTION_INFO_V1(gint4_penalty);
PG_FUNCTION_INFO_V1(gint4_same);

Datum		gint4_compress(PG_FUNCTION_ARGS);
Datum		gint4_union(PG_FUNCTION_ARGS);
Datum		gint4_picksplit(PG_FUNCTION_ARGS);
Datum		gint4_consistent(PG_FUNCTION_ARGS);
Datum		gint4_penalty(PG_FUNCTION_ARGS);
Datum		gint4_same(PG_FUNCTION_ARGS);

static void gint4_binary_union(Datum *r1, char *r2);
static int	int4key_cmp(const void *a, const void *b);

/*
** timestamp ops
*/
PG_FUNCTION_INFO_V1(gts_compress);
PG_FUNCTION_INFO_V1(gts_union);
PG_FUNCTION_INFO_V1(gts_picksplit);
PG_FUNCTION_INFO_V1(gts_consistent);
PG_FUNCTION_INFO_V1(gts_penalty);
PG_FUNCTION_INFO_V1(gts_same);

Datum		gts_compress(PG_FUNCTION_ARGS);
Datum		gts_union(PG_FUNCTION_ARGS);
Datum		gts_picksplit(PG_FUNCTION_ARGS);
Datum		gts_consistent(PG_FUNCTION_ARGS);
Datum		gts_penalty(PG_FUNCTION_ARGS);
Datum		gts_same(PG_FUNCTION_ARGS);

static void gts_binary_union(Datum *r1, char *r2);
static int	tskey_cmp(const void *a, const void *b);

#define TimestampGetDatumFast(X) Float8GetDatumFast(X)

/* define for comparison */
#define TSGE( ts1, ts2 ) (DatumGetBool(DirectFunctionCall2( \
		timestamp_ge, \
		PointerGetDatum( ts1 ), \
		PointerGetDatum( ts2 ) \
)))
#define TSGT( ts1, ts2 ) (DatumGetBool(DirectFunctionCall2( \
		timestamp_gt, \
		PointerGetDatum( ts1 ), \
		PointerGetDatum( ts2 ) \
)))
#define TSEQ( ts1, ts2 ) (DatumGetBool(DirectFunctionCall2( \
		timestamp_eq, \
		PointerGetDatum( ts1 ), \
		PointerGetDatum( ts2 ) \
)))
#define TSLT( ts1, ts2 ) (DatumGetBool(DirectFunctionCall2( \
		timestamp_lt, \
		PointerGetDatum( ts1 ), \
		PointerGetDatum( ts2 ) \
)))
#define TSLE( ts1, ts2 ) (DatumGetBool(DirectFunctionCall2( \
		timestamp_le, \
		PointerGetDatum( ts1 ), \
		PointerGetDatum( ts2 ) \
)))

/*
** Common btree-function (for all ops)
*/
static GIST_SPLITVEC *btree_picksplit(bytea *entryvec, GIST_SPLITVEC *v,
				BINARY_UNION bu, CMPFUNC cmp);

PG_FUNCTION_INFO_V1(btree_decompress);
Datum		btree_decompress(PG_FUNCTION_ARGS);

/**************************************************
 * int4 ops
 **************************************************/

Datum
gint4_compress(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	GISTENTRY  *retval;

	if (entry->leafkey)
	{
		INT4KEY    *r = palloc(sizeof(INT4KEY));

		retval = palloc(sizeof(GISTENTRY));
		r->lower = r->upper = (entry->key);

		gistentryinit(*retval, PointerGetDatum(r), entry->rel, entry->page,
					  entry->offset, sizeof(INT4KEY), FALSE);

	}
	else
		retval = entry;
	PG_RETURN_POINTER(retval);
}

Datum
gint4_consistent(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	int4		query = PG_GETARG_INT32(1);
	INT4KEY    *kkk = (INT4KEY *) DatumGetPointer(entry->key);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	bool		retval;

	switch (strategy)
	{
		case BTLessEqualStrategyNumber:
			retval = (query >= kkk->lower);
			break;
		case BTLessStrategyNumber:
			if (GIST_LEAF(entry))
				retval = (query > kkk->lower);
			else
				retval = (query >= kkk->lower);
			break;
		case BTEqualStrategyNumber:
			/* in leaf page kkk->lower always = kkk->upper */
			if (GIST_LEAF(entry))
				retval = (query == kkk->lower);
			else
				retval = (kkk->lower <= query && query <= kkk->upper);
			break;
		case BTGreaterStrategyNumber:
			if (GIST_LEAF(entry))
				retval = (query < kkk->upper);
			else
				retval = (query <= kkk->upper);
			break;
		case BTGreaterEqualStrategyNumber:
			retval = (query <= kkk->upper);
			break;
		default:
			retval = FALSE;
	}
	PG_RETURN_BOOL(retval);
}

Datum
gint4_union(PG_FUNCTION_ARGS)
{
	bytea	   *entryvec = (bytea *) PG_GETARG_POINTER(0);
	int			i,
				numranges;
	INT4KEY    *cur,
			   *out = palloc(sizeof(INT4KEY));

	numranges = (VARSIZE(entryvec) - VARHDRSZ) / sizeof(GISTENTRY);
	*(int *) PG_GETARG_POINTER(1) = sizeof(INT4KEY);

	cur = (INT4KEY *) DatumGetPointer((((GISTENTRY *) (VARDATA(entryvec)))[0].key));
	out->lower = cur->lower;
	out->upper = cur->upper;

	for (i = 1; i < numranges; i++)
	{
		cur = (INT4KEY *) DatumGetPointer((((GISTENTRY *) (VARDATA(entryvec)))[i].key));
		if (out->lower > cur->lower)
			out->lower = cur->lower;
		if (out->upper < cur->upper)
			out->upper = cur->upper;
	}

	PG_RETURN_POINTER(out);
}

Datum
gint4_penalty(PG_FUNCTION_ARGS)
{
	INT4KEY    *origentry = (INT4KEY *) DatumGetPointer(((GISTENTRY *) PG_GETARG_POINTER(0))->key);
	INT4KEY    *newentry = (INT4KEY *) DatumGetPointer(((GISTENTRY *) PG_GETARG_POINTER(1))->key);
	float	   *result = (float *) PG_GETARG_POINTER(2);

	*result = Max(newentry->upper - origentry->upper, 0) +
		Max(origentry->lower - newentry->lower, 0);

	PG_RETURN_POINTER(result);
}

Datum
gint4_picksplit(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(btree_picksplit(
									  (bytea *) PG_GETARG_POINTER(0),
								  (GIST_SPLITVEC *) PG_GETARG_POINTER(1),
									  gint4_binary_union,
									  int4key_cmp
									  ));
}

Datum
gint4_same(PG_FUNCTION_ARGS)
{
	INT4KEY    *b1 = (INT4KEY *) PG_GETARG_POINTER(0);
	INT4KEY    *b2 = (INT4KEY *) PG_GETARG_POINTER(1);
	bool	   *result = (bool *) PG_GETARG_POINTER(2);

	*result = (b1->lower == b2->lower && b1->upper == b2->upper) ? TRUE : FALSE;
	PG_RETURN_POINTER(result);
}

static void
gint4_binary_union(Datum *r1, char *r2)
{
	INT4KEY    *b1;
	INT4KEY    *b2 = (INT4KEY *) r2;

	if (!DatumGetPointer(*r1))
	{
		*r1 = PointerGetDatum(palloc(sizeof(INT4KEY)));
		b1 = (INT4KEY *) DatumGetPointer(*r1);
		b1->upper = b2->upper;
		b1->lower = b2->lower;
	}
	else
	{
		b1 = (INT4KEY *) DatumGetPointer(*r1);

		b1->lower = (b1->lower > b2->lower) ?
			b2->lower : b1->lower;
		b1->upper = (b1->upper > b2->upper) ?
			b1->upper : b2->upper;
	}
}


static int
int4key_cmp(const void *a, const void *b)
{
	return (((INT4KEY *) (((RIX *) a)->r))->lower - ((INT4KEY *) (((RIX *) b)->r))->lower);
}

/**************************************************
 * timestamp ops
 **************************************************/

Datum
gts_compress(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	GISTENTRY  *retval;

	if (entry->leafkey)
	{
		TSKEY	   *r = (TSKEY *) palloc(sizeof(TSKEY));
		retval = palloc(sizeof(GISTENTRY));
		r->lower = r->upper = *(Timestamp *) (entry->key);
		gistentryinit(*retval, PointerGetDatum(r),
			  entry->rel, entry->page,
			  entry->offset, sizeof(TSKEY), FALSE);
	}
	else
		retval = entry;
	PG_RETURN_POINTER(retval);
}

Datum
gts_consistent(PG_FUNCTION_ARGS)
{
	GISTENTRY  *entry = (GISTENTRY *) PG_GETARG_POINTER(0);
	Timestamp  *query = (Timestamp *) PG_GETARG_POINTER(1);
	StrategyNumber strategy = (StrategyNumber) PG_GETARG_UINT16(2);
	bool		retval;
	TSKEY	   *key;

	/*
	 * * if entry is not leaf, use gbox_internal_consistent, * else use
	 * gbox_leaf_consistent
	 */
	if (!entry->key)
		return FALSE;
	key = (TSKEY *) DatumGetPointer(entry->key);

	switch (strategy)
	{
		case BTLessEqualStrategyNumber:
			retval = TSGE(query, &(key->lower));
			break;
		case BTLessStrategyNumber:
			if (GIST_LEAF(entry))
				retval = TSGT(query, &(key->lower));
			else
				retval = TSGE(query, &(key->lower));
			break;
		case BTEqualStrategyNumber:
			/* in leaf page key->lower always = key->upper */
			if (GIST_LEAF(entry))
				retval = TSEQ(query, &(key->lower));
			else
				retval = (TSLE(&(key->lower), query) && TSLE(query, &(key->upper)));
			break;
		case BTGreaterStrategyNumber:
			if (GIST_LEAF(entry))
				retval = TSLT(query, &(key->upper));
			else
				retval = TSLE(query, &(key->upper));
			break;
		case BTGreaterEqualStrategyNumber:
			retval = TSLE(query, &(key->upper));
			break;
		default:
			retval = FALSE;
	}
	PG_RETURN_BOOL(retval);
}

Datum
gts_union(PG_FUNCTION_ARGS)
{
	bytea	   *entryvec = (bytea *) PG_GETARG_POINTER(0);
	int			i,
				numranges;
	TSKEY	   *cur,
			   *out = palloc(sizeof(TSKEY));

	numranges = (VARSIZE(entryvec) - VARHDRSZ) / sizeof(GISTENTRY);
	*(int *) PG_GETARG_POINTER(1) = sizeof(TSKEY);

	cur = (TSKEY *) DatumGetPointer((((GISTENTRY *) (VARDATA(entryvec)))[0].key));
	out->lower = cur->lower;
	out->upper = cur->upper;

	for (i = 1; i < numranges; i++)
	{
		cur = (TSKEY *) DatumGetPointer((((GISTENTRY *) (VARDATA(entryvec)))[i].key));
		if (TSGT(&out->lower, &cur->lower))
			out->lower = cur->lower;
		if (TSLT(&out->upper, &cur->upper))
			out->upper = cur->upper;
	}

	PG_RETURN_POINTER(out);
}

Datum
gts_penalty(PG_FUNCTION_ARGS)
{
	TSKEY	   *origentry = (TSKEY *) DatumGetPointer(((GISTENTRY *) PG_GETARG_POINTER(0))->key);
	TSKEY	   *newentry = (TSKEY *) DatumGetPointer(((GISTENTRY *) PG_GETARG_POINTER(1))->key);
	float	   *result = (float *) PG_GETARG_POINTER(2);
	Interval   *intr;

	intr = DatumGetIntervalP(DirectFunctionCall2(
												 timestamp_mi,
									  TimestampGetDatumFast(newentry->upper),
								   TimestampGetDatumFast(origentry->upper)));

	/* see interval_larger */
	*result = Max(intr->time + intr->month * (30.0 * 86400), 0);
	pfree(intr);

	intr = DatumGetIntervalP(DirectFunctionCall2(
												 timestamp_mi,
									 TimestampGetDatumFast(origentry->lower),
									TimestampGetDatumFast(newentry->lower)));

	/* see interval_larger */
	*result += Max(intr->time + intr->month * (30.0 * 86400), 0);
	pfree(intr);

	PG_RETURN_POINTER(result);
}

Datum
gts_picksplit(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(btree_picksplit(
									  (bytea *) PG_GETARG_POINTER(0),
								  (GIST_SPLITVEC *) PG_GETARG_POINTER(1),
									  gts_binary_union,
									  tskey_cmp
									  ));
}

Datum
gts_same(PG_FUNCTION_ARGS)
{
	TSKEY	   *b1 = (TSKEY *) PG_GETARG_POINTER(0);
	TSKEY	   *b2 = (TSKEY *) PG_GETARG_POINTER(1);

	bool	   *result = (bool *) PG_GETARG_POINTER(2);

	if (b1 && b2)
		*result = (TSEQ(&(b1->lower), &(b2->lower)) && TSEQ(&(b1->upper), &(b2->upper))) ? TRUE : FALSE;
	else
		*result = (b1 == NULL && b2 == NULL) ? TRUE : FALSE;
	PG_RETURN_POINTER(result);
}

static void
gts_binary_union(Datum *r1, char *r2)
{
	TSKEY	   *b1;
	TSKEY	   *b2 = (TSKEY *) r2;

	if (!DatumGetPointer(*r1))
	{
		*r1 = PointerGetDatum(palloc(sizeof(TSKEY)));
		b1 = (TSKEY *) DatumGetPointer(*r1);
		b1->upper = b2->upper;
		b1->lower = b2->lower;
	}
	else
	{
		b1 = (TSKEY *) DatumGetPointer(*r1);

		b1->lower = (TSGT(&b1->lower, &b2->lower)) ?
			b2->lower : b1->lower;
		b1->upper = (TSGT(&b1->upper, &b2->upper)) ?
			b1->upper : b2->upper;
	}
}

static int
tskey_cmp(const void *a, const void *b)
{
	return DatumGetInt32(
						 DirectFunctionCall2(
											 timestamp_cmp,
				  TimestampGetDatumFast(((TSKEY *) (((RIX *) a)->r))->lower),
				   TimestampGetDatumFast(((TSKEY *) (((RIX *) b)->r))->lower)
											 )
		);
}

/**************************************************
 * Common btree-function (for all ops)
 **************************************************/

/*
** The GiST PickSplit method
*/
static GIST_SPLITVEC *
btree_picksplit(bytea *entryvec, GIST_SPLITVEC *v, BINARY_UNION bu, CMPFUNC cmp)
{
	OffsetNumber i;
	RIX		   *array;
	OffsetNumber maxoff;
	int			nbytes;

	maxoff = ((VARSIZE(entryvec) - VARHDRSZ) / sizeof(GISTENTRY)) - 1;
	nbytes = (maxoff + 2) * sizeof(OffsetNumber);
	v->spl_left = (OffsetNumber *) palloc(nbytes);
	v->spl_right = (OffsetNumber *) palloc(nbytes);
	v->spl_nleft = 0;
	v->spl_nright = 0;
	v->spl_ldatum = PointerGetDatum(0);
	v->spl_rdatum = PointerGetDatum(0);
	array = (RIX *) palloc(sizeof(RIX) * (maxoff + 1));

	/* copy the data into RIXes, and sort the RIXes */
	for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i))
	{
		array[i].index = i;
		array[i].r = (char *) DatumGetPointer((((GISTENTRY *) (VARDATA(entryvec)))[i].key));
	}
	qsort((void *) &array[FirstOffsetNumber], maxoff - FirstOffsetNumber + 1,
		  sizeof(RIX), cmp);

	for (i = FirstOffsetNumber; i <= maxoff; i = OffsetNumberNext(i))
	{
		if (i <= (maxoff - FirstOffsetNumber + 1) / 2)
		{
			v->spl_left[v->spl_nleft] = array[i].index;
			v->spl_nleft++;
			(*bu) (&v->spl_ldatum, array[i].r);
		}
		else
		{
			v->spl_right[v->spl_nright] = array[i].index;
			v->spl_nright++;
			(*bu) (&v->spl_rdatum, array[i].r);
		}
	}
	pfree(array);

	return (v);
}

/*
** GiST DeCompress methods
** do not do anything.
*/
Datum
btree_decompress(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(PG_GETARG_POINTER(0));
}


/**************************************************
 * In/Out for keys, not really needed
 **************************************************/
Datum
int4key_in(PG_FUNCTION_ARGS)
{
	INT4KEY    *key = palloc(sizeof(INT4KEY));

	if (sscanf(PG_GETARG_POINTER(0), "%d|%d", &(key->lower), &(key->upper)) != 2)
		elog(ERROR, "Error in input format");

	PG_RETURN_POINTER(key);
}

Datum
int4key_out(PG_FUNCTION_ARGS)
{
	INT4KEY    *key = (INT4KEY *) PG_GETARG_POINTER(0);
	char	   *str = palloc(sizeof(char) * 22);

	sprintf(str, "%d|%d", key->lower, key->upper);
	PG_RETURN_POINTER(str);
}

Datum
tskey_in(PG_FUNCTION_ARGS)
{
	elog(ERROR, "Not implemented");
	PG_RETURN_POINTER(NULL);
}

Datum
tskey_out(PG_FUNCTION_ARGS)
{
	elog(ERROR, "Not implemented");
	PG_RETURN_POINTER(NULL);
}
