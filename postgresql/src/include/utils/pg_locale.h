/* -----------------------------------------------------------------------
 * pg_locale.h
 *
 *	 The PostgreSQL locale utils.
 *
 *
 * $Id: pg_locale.h,v 1.2 2004/07/05 08:56:08 SuLiang Exp $
 *
 *	 Portions Copyright (c) 1999-2000, PostgreSQL Global Development Group
 *
 *	Karel Zak - Zakkr
 *
 * -----------------------------------------------------------------------
 */

#ifndef _PG_LOCALE_
#define _PG_LOCALE_

#ifdef USE_LOCALE

/*------
 * POSIX locale categories and environment variable LANG
 *------
 */
typedef struct PG_LocaleCategories
{
	char	   *lang,
			   *lc_ctype,
			   *lc_numeric,
			   *lc_time,
			   *lc_collate,
			   *lc_monetary,
			   *lc_messages;
} PG_LocaleCategories;

/*
 * Save locale category settings into PG memory
 */
extern void PGLC_current(PG_LocaleCategories *lc);

/*
 * Free memory allocated in PGLC_current()
 */
extern void PGLC_free_categories(PG_LocaleCategories *lc);

/*------
 * Return the POSIX lconv struct (contains number/money formatting information)
 * with locale information for all categories.	Note that returned lconv
 * does not depend on currently active category settings, but on external
 * environment variables for locale.
 *------
 */
extern struct lconv *PGLC_localeconv(void);
#endif   /* USE_LOCALE */

#endif   /* _PG_LOCALE_ */
