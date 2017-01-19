typedef union
{
	int					ival;
	char				chr;
	char				*str;
	bool				boolean;
	JoinType			jtype;
	List				*list;
	Node				*node;
	Value				*value;

	Attr				*attr;
	Ident				*ident;

	TypeName			*typnam;
	DefElem				*defelt;
	SortGroupBy			*sortgroupby;
	JoinExpr			*jexpr;
	IndexElem			*ielem;
	RangeVar			*range;
	A_Indices			*aind;
	ResTarget			*target;
	ParamNo				*paramno;

	VersionStmt			*vstmt;
	DefineStmt			*dstmt;
	RuleStmt			*rstmt;
	InsertStmt			*istmt;
} YYSTYPE;
#define	ABSOLUTE	257
#define	ACTION	258
#define	ADD	259
#define	ALL	260
#define	ALTER	261
#define	AND	262
#define	ANY	263
#define	AS	264
#define	ASC	265
#define	AT	266
#define	AUTHORIZATION	267
#define	BEGIN_TRANS	268
#define	BETWEEN	269
#define	BOTH	270
#define	BY	271
#define	CASCADE	272
#define	CASE	273
#define	CAST	274
#define	CHAR	275
#define	CHARACTER	276
#define	CHECK	277
#define	CLOSE	278
#define	COALESCE	279
#define	COLLATE	280
#define	COLUMN	281
#define	COMMIT	282
#define	CONSTRAINT	283
#define	CONSTRAINTS	284
#define	CREATE	285
#define	CROSS	286
#define	CURRENT_DATE	287
#define	CURRENT_TIME	288
#define	CURRENT_TIMESTAMP	289
#define	CURRENT_USER	290
#define	CURSOR	291
#define	DAY_P	292
#define	DEC	293
#define	DECIMAL	294
#define	DECLARE	295
#define	DEFAULT	296
#define	DELETE	297
#define	DESC	298
#define	DISTINCT	299
#define	DOUBLE	300
#define	DROP	301
#define	ELSE	302
#define	ENCRYPTED	303
#define	END_TRANS	304
#define	ESCAPE	305
#define	EXCEPT	306
#define	EXECUTE	307
#define	EXISTS	308
#define	EXTRACT	309
#define	FALSE_P	310
#define	FETCH	311
#define	FLOAT	312
#define	FOR	313
#define	FOREIGN	314
#define	FROM	315
#define	FULL	316
#define	GLOBAL	317
#define	GRANT	318
#define	GROUP	319
#define	HAVING	320
#define	HOUR_P	321
#define	IN	322
#define	INNER_P	323
#define	INSENSITIVE	324
#define	INSERT	325
#define	INTERSECT	326
#define	INTERVAL	327
#define	INTO	328
#define	IS	329
#define	ISOLATION	330
#define	JOIN	331
#define	KEY	332
#define	LANGUAGE	333
#define	LEADING	334
#define	LEFT	335
#define	LEVEL	336
#define	LIKE	337
#define	LOCAL	338
#define	MATCH	339
#define	MINUTE_P	340
#define	MONTH_P	341
#define	NAMES	342
#define	NATIONAL	343
#define	NATURAL	344
#define	NCHAR	345
#define	NEXT	346
#define	NO	347
#define	NOT	348
#define	NULLIF	349
#define	NULL_P	350
#define	NUMERIC	351
#define	OF	352
#define	OLD	353
#define	ON	354
#define	ONLY	355
#define	OPTION	356
#define	OR	357
#define	ORDER	358
#define	OUTER_P	359
#define	OVERLAPS	360
#define	PARTIAL	361
#define	POSITION	362
#define	PRECISION	363
#define	PRIMARY	364
#define	PRIOR	365
#define	PRIVILEGES	366
#define	PROCEDURE	367
#define	PUBLIC	368
#define	READ	369
#define	REFERENCES	370
#define	RELATIVE	371
#define	REVOKE	372
#define	RIGHT	373
#define	ROLLBACK	374
#define	SCHEMA	375
#define	SCROLL	376
#define	SECOND_P	377
#define	SELECT	378
#define	SESSION	379
#define	SESSION_USER	380
#define	SET	381
#define	SOME	382
#define	SUBSTRING	383
#define	TABLE	384
#define	TEMPORARY	385
#define	THEN	386
#define	TIME	387
#define	TIMESTAMP	388
#define	TO	389
#define	TRAILING	390
#define	TRANSACTION	391
#define	TRIM	392
#define	TRUE_P	393
#define	UNENCRYPTED	394
#define	UNION	395
#define	UNIQUE	396
#define	UNKNOWN	397
#define	UPDATE	398
#define	USER	399
#define	USING	400
#define	VALUES	401
#define	VARCHAR	402
#define	VARYING	403
#define	VIEW	404
#define	WHEN	405
#define	WHERE	406
#define	WITH	407
#define	WORK	408
#define	YEAR_P	409
#define	ZONE	410
#define	CHAIN	411
#define	CHARACTERISTICS	412
#define	DEFERRABLE	413
#define	DEFERRED	414
#define	IMMEDIATE	415
#define	INITIALLY	416
#define	INOUT	417
#define	OFF	418
#define	OUT	419
#define	PATH_P	420
#define	PENDANT	421
#define	REPLACE	422
#define	RESTRICT	423
#define	TRIGGER	424
#define	WITHOUT	425
#define	COMMITTED	426
#define	SERIALIZABLE	427
#define	TYPE_P	428
#define	ABORT_TRANS	429
#define	ACCESS	430
#define	AFTER	431
#define	AGGREGATE	432
#define	ANALYSE	433
#define	ANALYZE	434
#define	BACKWARD	435
#define	BEFORE	436
#define	BINARY	437
#define	BIT	438
#define	CACHE	439
#define	CHECKPOINT	440
#define	CLUSTER	441
#define	COMMENT	442
#define	COPY	443
#define	CREATEDB	444
#define	CREATEUSER	445
#define	CYCLE	446
#define	DATABASE	447
#define	DELIMITERS	448
#define	DO	449
#define	EACH	450
#define	ENCODING	451
#define	EXCLUSIVE	452
#define	EXPLAIN	453
#define	FORCE	454
#define	FORWARD	455
#define	FREEZE	456
#define	FUNCTION	457
#define	HANDLER	458
#define	ILIKE	459
#define	INCREMENT	460
#define	INDEX	461
#define	INHERITS	462
#define	INSTEAD	463
#define	ISNULL	464
#define	LANCOMPILER	465
#define	LIMIT	466
#define	LISTEN	467
#define	LOAD	468
#define	LOCATION	469
#define	LOCK_P	470
#define	MAXVALUE	471
#define	MINVALUE	472
#define	MODE	473
#define	MOVE	474
#define	NEW	475
#define	NOCREATEDB	476
#define	NOCREATEUSER	477
#define	NONE	478
#define	NOTHING	479
#define	NOTIFY	480
#define	NOTNULL	481
#define	OFFSET	482
#define	OIDS	483
#define	OPERATOR	484
#define	OWNER	485
#define	PASSWORD	486
#define	PROCEDURAL	487
#define	REINDEX	488
#define	RENAME	489
#define	RESET	490
#define	RETURNS	491
#define	ROW	492
#define	RULE	493
#define	SEQUENCE	494
#define	SETOF	495
#define	SHARE	496
#define	SHOW	497
#define	START	498
#define	STATEMENT	499
#define	STATISTICS	500
#define	STDIN	501
#define	STDOUT	502
#define	SYSID	503
#define	TEMP	504
#define	TEMPLATE	505
#define	TOAST	506
#define	TRUNCATE	507
#define	TRUSTED	508
#define	UNLISTEN	509
#define	UNTIL	510
#define	VACUUM	511
#define	VALID	512
#define	VERBOSE	513
#define	VERSION	514
#define	XACOMMIT_TRANS	515
#define	XAPREPARE_TRANS	516
#define	XAROLLBACK_TRANS	517
#define	UNIONJOIN	518
#define	IDENT	519
#define	FCONST	520
#define	SCONST	521
#define	BITCONST	522
#define	Op	523
#define	ICONST	524
#define	PARAM	525
#define	OP	526
#define	POSTFIXOP	527
#define	UMINUS	528
#define	TYPECAST	529


extern YYSTYPE yylval;
