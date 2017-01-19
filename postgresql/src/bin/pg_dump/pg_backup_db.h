/*
 *	Definitions for pg_backup_db.c
 *
 *	IDENTIFICATION
 *		$Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/pg_dump/pg_backup_db.h,v 1.2 2004/07/05 08:55:50 SuLiang Exp $
 */

#define BLOB_XREF_TABLE "dump_blob_xref"		/* MUST be lower case */

extern void FixupBlobRefs(ArchiveHandle *AH, char *tablename);
extern int	ExecuteSqlCommand(ArchiveHandle *AH, PQExpBuffer qry, char *desc, bool use_blob);
extern int	ExecuteSqlCommandBuf(ArchiveHandle *AH, void *qry, int bufLen);

extern void CreateBlobXrefTable(ArchiveHandle *AH);
extern void InsertBlobXref(ArchiveHandle *AH, int old, int new);
extern void StartTransaction(ArchiveHandle *AH);
extern void StartTransactionXref(ArchiveHandle *AH);
extern void CommitTransaction(ArchiveHandle *AH);
extern void CommitTransactionXref(ArchiveHandle *AH);
