#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

#ifndef PLPGSQL_YYSTYPE
typedef union {
		int32					ival;
		char					*str;
		struct
		{
			char *name;
			int  lineno;
		}						varname;
		struct
		{
			int  nalloc;
			int  nused;
			int	 *nums;
		}						intlist;
		struct
		{
			int  nalloc;
			int  nused;
			PLpgSQL_diag_item *dtitems;
		}						dtlist;
		struct
		{
			int  reverse;
			PLpgSQL_expr *expr;
		}						forilow;
		struct
		{
			char *label;
			int  n_initvars;
			int  *initvarnos;
		}						declhdr;
		PLpgSQL_type			*dtype;
		PLpgSQL_var				*var;
		PLpgSQL_row				*row;
		PLpgSQL_rec				*rec;
		PLpgSQL_recfield		*recfield;
		PLpgSQL_trigarg			*trigarg;
		PLpgSQL_expr			*expr;
		PLpgSQL_stmt			*stmt;
		PLpgSQL_stmts			*stmts;
		PLpgSQL_stmt_block		*program;
		PLpgSQL_nsitem			*nsitem;
} plpgsql_yystype;
# define PLPGSQL_YYSTYPE plpgsql_yystype
# define PLPGSQL_YYSTYPE_IS_TRIVIAL 1
#endif
# define	K_ALIAS	257
# define	K_ASSIGN	258
# define	K_BEGIN	259
# define	K_CLOSE	260
# define	K_CONSTANT	261
# define	K_CURSOR	262
# define	K_DEBUG	263
# define	K_DECLARE	264
# define	K_DEFAULT	265
# define	K_DIAGNOSTICS	266
# define	K_DOTDOT	267
# define	K_ELSE	268
# define	K_ELSIF	269
# define	K_END	270
# define	K_EXCEPTION	271
# define	K_EXECUTE	272
# define	K_EXIT	273
# define	K_FOR	274
# define	K_FETCH	275
# define	K_FROM	276
# define	K_GET	277
# define	K_IF	278
# define	K_IN	279
# define	K_INTO	280
# define	K_IS	281
# define	K_LOOP	282
# define	K_NOT	283
# define	K_NOTICE	284
# define	K_NULL	285
# define	K_OPEN	286
# define	K_PERFORM	287
# define	K_ROW_COUNT	288
# define	K_RAISE	289
# define	K_RECORD	290
# define	K_RENAME	291
# define	K_RESULT_OID	292
# define	K_RETURN	293
# define	K_REVERSE	294
# define	K_SELECT	295
# define	K_THEN	296
# define	K_TO	297
# define	K_TYPE	298
# define	K_WHEN	299
# define	K_WHILE	300
# define	T_FUNCTION	301
# define	T_TRIGGER	302
# define	T_LABEL	303
# define	T_STRING	304
# define	T_VARIABLE	305
# define	T_ROW	306
# define	T_ROWTYPE	307
# define	T_RECORD	308
# define	T_RECFIELD	309
# define	T_TGARGV	310
# define	T_DTYPE	311
# define	T_WORD	312
# define	T_NUMBER	313
# define	T_ERROR	314
# define	O_OPTION	315
# define	O_DUMP	316


extern PLPGSQL_YYSTYPE plpgsql_yylval;

#endif /* not BISON_Y_TAB_H */
