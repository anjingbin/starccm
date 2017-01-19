/*
 * psql - the PostgreSQL interactive terminal
 *
 * Copyright 2000 by PostgreSQL Global Development Group
 *
 * $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/psql/mainloop.c,v 1.2 2004/07/05 08:55:54 SuLiang Exp $
 */
#include "postgres_fe.h"
#include "mainloop.h"

#include "pqexpbuffer.h"

#include "settings.h"
#include "prompt.h"
#include "input.h"
#include "common.h"
#include "command.h"

#ifndef WIN32
#include <setjmp.h>
sigjmp_buf	main_loop_jmp;
#endif


/*
 * Main processing loop for reading lines of input
 *	and sending them to the backend.
 *
 * This loop is re-entrant. May be called by \i command
 *	which reads input from a file.
 *
 * FIXME: rewrite this whole thing with flex
 */
int
MainLoop(FILE *source)
{
	PQExpBuffer query_buf;		/* buffer for query being accumulated */
	PQExpBuffer previous_buf;	/* if there isn't anything in the new
								 * buffer yet, use this one for \e, etc. */
	char	   *line;			/* current line of input */
	int			len;			/* length of the line */
	volatile int successResult = EXIT_SUCCESS;
	volatile backslashResult slashCmdStatus;

	bool		success;
	volatile char in_quote;		/* == 0 for no in_quote */
	volatile bool in_xcomment;	/* in extended comment */
	volatile int xcdepth;
	volatile int paren_level;
	unsigned int query_start;
	volatile int count_eof = 0;
	const char *var;
	volatile unsigned int bslash_count = 0;

	int			i,
				prevlen,
				thislen;

	/* Save the prior command source */
	FILE	   *prev_cmd_source;
	bool		prev_cmd_interactive;

	unsigned int prev_lineno;
	volatile bool die_on_error = false;


	/* Save old settings */
	prev_cmd_source = pset.cur_cmd_source;
	prev_cmd_interactive = pset.cur_cmd_interactive;

	/* Establish new source */
	pset.cur_cmd_source = source;
	pset.cur_cmd_interactive = ((source == stdin) && !pset.notty);


	query_buf = createPQExpBuffer();
	previous_buf = createPQExpBuffer();
	if (!query_buf || !previous_buf)
	{
		psql_error("out of memory\n");
		exit(EXIT_FAILURE);
	}

	in_xcomment = false;
	in_quote = 0;
	paren_level = 0;
	slashCmdStatus = CMD_UNKNOWN;		/* set default */
	prev_lineno = pset.lineno;
	pset.lineno = 0;


	/* main loop to get queries and execute them */
	while (1)
	{
#ifndef WIN32

		/*
		 * Welcome code for Control-C
		 */
		if (cancel_pressed)
		{
			if (!pset.cur_cmd_interactive)
			{
				/*
				 * You get here if you stopped a script with Ctrl-C and a
				 * query cancel was issued. In that case we don't do the
				 * longjmp, so the query routine can finish nicely.
				 */
				successResult = EXIT_USER;
				break;
			}

			cancel_pressed = false;
		}

		if (sigsetjmp(main_loop_jmp, 1) != 0)
		{
			/* got here with longjmp */

			if (pset.cur_cmd_interactive)
			{
				fputc('\n', stdout);
				resetPQExpBuffer(query_buf);

				/* reset parsing state */
				in_xcomment = false;
				in_quote = 0;
				paren_level = 0;
				count_eof = 0;
				slashCmdStatus = CMD_UNKNOWN;
			}
			else
			{
				successResult = EXIT_USER;
				break;
			}
		}

		/*
		 * establish the control-C handler only after main_loop_jmp is
		 * ready
		 */
		pqsignal(SIGINT, handle_sigint);		/* control-C => cancel */
#endif   /* not WIN32 */

		if (slashCmdStatus == CMD_NEWEDIT)
		{
			/*
			 * just returned from editing the line? then just copy to the
			 * input buffer
			 */
			line = xstrdup(query_buf->data);
			resetPQExpBuffer(query_buf);
			/* reset parsing state since we are rescanning whole line */
			in_xcomment = false;
			in_quote = 0;
			paren_level = 0;
			slashCmdStatus = CMD_UNKNOWN;
		}
		else
		{
			fflush(stdout);

			/*
			 * otherwise, set interactive prompt if necessary and get
			 * another line
			 */
			if (pset.cur_cmd_interactive)
			{
				int			prompt_status;

				if (in_quote && in_quote == '\'')
					prompt_status = PROMPT_SINGLEQUOTE;
				else if (in_quote && in_quote == '"')
					prompt_status = PROMPT_DOUBLEQUOTE;
				else if (in_xcomment)
					prompt_status = PROMPT_COMMENT;
				else if (paren_level)
					prompt_status = PROMPT_PAREN;
				else if (query_buf->len > 0)
					prompt_status = PROMPT_CONTINUE;
				else
					prompt_status = PROMPT_READY;

				line = gets_interactive(get_prompt(prompt_status));
			}
			else
				line = gets_fromFile(source);
		}


		/* Setting this will not have effect until next line. */
		die_on_error = GetVariableBool(pset.vars, "ON_ERROR_STOP");

		/*
		 * query_buf holds query already accumulated.  line is the
		 * malloc'd new line of input (note it must be freed before
		 * looping around!) query_start is the next command start location
		 * within the line.
		 */

		/* No more input.  Time to quit, or \i done */
		if (line == NULL)
		{
			if (pset.cur_cmd_interactive)
			{
				bool		getout = true;

				/* This tries to mimic bash's IGNOREEOF feature. */
				const char *val = GetVariable(pset.vars, "IGNOREEOF");

				if (val)
				{
					long int	maxeof;
					char	   *endptr;

					if (*val == '\0')
						maxeof = 10;
					else
					{
						maxeof = strtol(val, &endptr, 0);
						if (*endptr != '\0')	/* string not valid as a
												 * number */
							maxeof = 10;
					}

					if (count_eof++ != maxeof)
						getout = false; /* not quite there yet */
				}

				if (getout)
				{
					if (QUIET())
						putc('\n', stdout);
					else
						puts("\\q");
					break;
				}
				else
				{
					if (!QUIET())
						printf(gettext("Use \"\\q\" to leave %s.\n"), pset.progname);
					continue;
				}
			}
			else
/* not interactive */
				break;
		}
		else
			count_eof = 0;

		pset.lineno++;

		/* nothing left on line? then ignore */
		if (line[0] == '\0' && !in_quote)
		{
			free(line);
			continue;
		}

		/* echo back if flag is set */
		var = GetVariable(pset.vars, "ECHO");
		if (!pset.cur_cmd_interactive && var && strcmp(var, "all") == 0)
			puts(line);
		fflush(stdout);

		len = strlen(line);
		query_start = 0;

		/*
		 * Parse line, looking for command separators.
		 *
		 * The current character is at line[i], the prior character at line[i
		 * - prevlen], the next character at line[i + thislen].
		 */
#define ADVANCE_1 (prevlen = thislen, i += thislen, thislen = PQmblen(line+i, pset.encoding))

		success = true;
		for (i = 0, prevlen = 0, thislen = (len > 0) ? PQmblen(line, pset.encoding) : 0;
			 i < len;
			 ADVANCE_1)
		{
			/* was the previous character a backslash? */
			bool		was_bslash = (i > 0 && line[i - prevlen] == '\\');

			if (was_bslash)
				bslash_count++;
			else
				bslash_count = 0;

	rescan:

			/*
			 * It is important to place the in_* test routines before the
			 * in_* detection routines. i.e. we have to test if we are in
			 * a quote before testing for comments. bjm  2000-06-30
			 */

			/* in quote? */
			if (in_quote)
			{
				/* end of quote */
				if (line[i] == in_quote && bslash_count % 2 == 0)
					in_quote = '\0';
			}

			/* in extended comment? */
			else if (in_xcomment)
			{
				if (line[i] == '*' && line[i + thislen] == '/')
				{
					if (xcdepth > 0)
						xcdepth--;
					else
					{
						in_xcomment = false;
						ADVANCE_1;
					}
				}
				else if (line[i] == '/' && line[i + thislen] == '*')
					xcdepth++;
			}

			/* start of extended comment? */
			else if (line[i] == '/' && line[i + thislen] == '*')
			{
				xcdepth = 0;
				in_xcomment = true;
				ADVANCE_1;
			}

			/* start of quote */
			else if (!was_bslash &&
					 (line[i] == '\'' || line[i] == '"'))
				in_quote = line[i];


			/* single-line comment? truncate line */
			else if (line[i] == '-' && line[i + thislen] == '-')
			{
				line[i] = '\0'; /* remove comment */
				break;
			}

			/* count nested parentheses */
			else if (line[i] == '(')
				paren_level++;

			else if (line[i] == ')' && paren_level > 0)
				paren_level--;

			/* colon -> substitute variable */
			/* we need to be on the watch for the '::' operator */
			else if (line[i] == ':' && !was_bslash
				  && strspn(line + i + thislen, VALID_VARIABLE_CHARS) > 0
					 && !(prevlen > 0 && line[i - prevlen] == ':')
				)
			{
				size_t		in_length,
							out_length;
				const char *value;
				char	   *new;
				char		after;		/* the character after the
										 * variable name will be
										 * temporarily overwritten */

				in_length = strspn(&line[i + thislen], VALID_VARIABLE_CHARS);
				after = line[i + thislen + in_length];
				line[i + thislen + in_length] = '\0';

				/*
				 * if the variable doesn't exist we'll leave the string as
				 * is
				 */
				value = GetVariable(pset.vars, &line[i + thislen]);
				if (value)
				{
					out_length = strlen(value);

					new = malloc(len + out_length - (1 + in_length) + 1);
					if (!new)
					{
						psql_error("out of memory\n");
						exit(EXIT_FAILURE);
					}

					sprintf(new, "%.*s%s%c", i, line, value, after);
					if (after)
						strcat(new, line + i + 1 + in_length + 1);

					free(line);
					line = new;
					len = strlen(new);

					goto rescan;	/* reparse the just substituted */
				}
				else
				{
					/* restore overwritten character */
					line[i + thislen + in_length] = after;
					/* move on ... */
				}
			}

			/* semicolon? then send query */
			else if (line[i] == ';' && !was_bslash && !paren_level)
			{
				line[i] = '\0';
				/* is there anything else on the line? */
				if (line[query_start + strspn(line + query_start, " \t\n\r")] != '\0')
				{
					/*
					 * insert a cosmetic newline, if this is not the first
					 * line in the buffer
					 */
					if (query_buf->len > 0)
						appendPQExpBufferChar(query_buf, '\n');
					/* append the line to the query buffer */
					appendPQExpBufferStr(query_buf, line + query_start);
					appendPQExpBufferChar(query_buf, ';');
				}

				/* execute query */
				success = SendQuery(query_buf->data);
				slashCmdStatus = success ? CMD_SEND : CMD_ERROR;

				resetPQExpBuffer(previous_buf);
				appendPQExpBufferStr(previous_buf, query_buf->data);
				resetPQExpBuffer(query_buf);
				query_start = i + thislen;
			}

			/*
			 * if you have a burning need to send a semicolon or colon to
			 * the backend ...
			 */
			else if (was_bslash && (line[i] == ';' || line[i] == ':'))
			{
				/* remove the backslash */
				memmove(line + i - prevlen, line + i, len - i + 1);
				len--;
			}

			/* backslash command */
			else if (was_bslash)
			{
				const char *end_of_cmd = NULL;

				paren_level = 0;
				line[i - prevlen] = '\0';		/* overwrites backslash */

				/* is there anything else on the line for the command? */
				if (line[query_start + strspn(line + query_start, " \t\n\r")] != '\0')
				{
					/*
					 * insert a cosmetic newline, if this is not the first
					 * line in the buffer
					 */
					if (query_buf->len > 0)
						appendPQExpBufferChar(query_buf, '\n');
					/* append the line to the query buffer */
					appendPQExpBufferStr(query_buf, line + query_start);
				}

				/* handle backslash command */
				slashCmdStatus = HandleSlashCmds(&line[i],
						   query_buf->len > 0 ? query_buf : previous_buf,
												 &end_of_cmd);

				success = slashCmdStatus != CMD_ERROR;

				if ((slashCmdStatus == CMD_SEND || slashCmdStatus == CMD_NEWEDIT) &&
					query_buf->len == 0)
				{
					/* copy previous buffer to current for for handling */
					appendPQExpBufferStr(query_buf, previous_buf->data);
				}

				if (slashCmdStatus == CMD_SEND)
				{
					success = SendQuery(query_buf->data);
					query_start = i + thislen;

					resetPQExpBuffer(previous_buf);
					appendPQExpBufferStr(previous_buf, query_buf->data);
					resetPQExpBuffer(query_buf);
				}

				/* process anything left after the backslash command */
				i += end_of_cmd - &line[i];
				query_start = i;
			}


			/* stop the script after error */
			if (!success && die_on_error)
				break;

		}						/* for (line) */


		if (slashCmdStatus == CMD_TERMINATE)
		{
			successResult = EXIT_SUCCESS;
			break;
		}


		/* Put the rest of the line in the query buffer. */
		if (in_quote || line[query_start + strspn(line + query_start, " \t\n\r")] != '\0')
		{
			if (query_buf->len > 0)
				appendPQExpBufferChar(query_buf, '\n');
			appendPQExpBufferStr(query_buf, line + query_start);
		}

		free(line);


		/* In single line mode, send off the query if any */
		if (query_buf->data[0] != '\0' && GetVariableBool(pset.vars, "SINGLELINE"))
		{
			success = SendQuery(query_buf->data);
			slashCmdStatus = success ? CMD_SEND : CMD_ERROR;
			resetPQExpBuffer(previous_buf);
			appendPQExpBufferStr(previous_buf, query_buf->data);
			resetPQExpBuffer(query_buf);
		}


		if (!success && die_on_error && !pset.cur_cmd_interactive)
		{
			successResult = EXIT_USER;
			break;
		}


		/* Have we lost the db connection? */
		if (pset.db == NULL && !pset.cur_cmd_interactive)
		{
			successResult = EXIT_BADCONN;
			break;
		}
	}							/* while !endoffile/session */

	/*
	 * Process query at the end of file without a semicolon
	 */
	if (query_buf->len > 0 && !pset.cur_cmd_interactive &&
		successResult == EXIT_SUCCESS)
	{
		success = SendQuery(query_buf->data);

		if (!success && die_on_error)
			successResult = EXIT_USER;
		else if (pset.db == NULL)
			successResult = EXIT_BADCONN;
	}

	/*
	 * Reset SIGINT handler because main_loop_jmp will be invalid as soon
	 * as we exit this routine.  If there is an outer MainLoop instance,
	 * it will re-enable ^C catching as soon as it gets back to the top of
	 * its loop and resets main_loop_jmp to point to itself.
	 */
#ifndef WIN32
	pqsignal(SIGINT, SIG_DFL);
#endif

	destroyPQExpBuffer(query_buf);
	destroyPQExpBuffer(previous_buf);

	pset.cur_cmd_source = prev_cmd_source;
	pset.cur_cmd_interactive = prev_cmd_interactive;
	pset.lineno = prev_lineno;

	return successResult;
}	/* MainLoop() */
