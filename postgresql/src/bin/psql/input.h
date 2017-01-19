/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/psql/input.h,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
 */
#ifndef INPUT_H
#define INPUT_H

/*
 * If some other file needs to have access to readline/history, include this
 * file and save yourself all this work.
 *
 * USE_READLINE and USE_HISTORY are the definite pointers regarding existence or not.
 */
#ifdef HAVE_LIBREADLINE
#if defined(HAVE_READLINE_READLINE_H)
#include <readline/readline.h>
#define USE_READLINE 1
#elif defined(HAVE_READLINE_H)
#include <readline.h>
#define USE_READLINE 1
#endif
#endif

#if defined(HAVE_HISTORY_FUNCTIONS)
#if defined(HAVE_READLINE_HISTORY_H)
#include <readline/history.h>
#define USE_HISTORY 1
#elif defined(HAVE_HISTORY_H)
#include <history.h>
#define USE_HISTORY 1
#endif
#endif

char	   *gets_interactive(char *prompt);
char	   *gets_fromFile(FILE *source);

void		initializeInput(int flags);
bool		saveHistory(char *fname);

#endif   /* INPUT_H */
