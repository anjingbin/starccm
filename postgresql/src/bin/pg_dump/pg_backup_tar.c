/*-------------------------------------------------------------------------
 *
 * pg_backup_tar.c
 *
 *	This file is copied from the 'files' format file, but dumps data into
 *	one temp file then sends it to the output TAR archive.
 *
 *	See the headers to pg_backup_files & pg_restore for more details.
 *
 * Copyright (c) 2000, Philip Warner
 *		Rights are granted to use this software in any way so long
 *		as this notice is not removed.
 *
 *	The author is not responsible for loss or damages that may
 *	result from it's use.
 *
 *
 * IDENTIFICATION
 *		$Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/bin/pg_dump/pg_backup_tar.c,v 1.2 2004/07/05 08:55:50 SuLiang Exp $
 *
 * Modifications - 28-Jun-2000 - pjw@rhyme.com.au
 *
 *	Initial version.
 *
 * Modifications - 04-Jan-2001 - pjw@rhyme.com.au
 *
 *	  - Check results of IO routines more carefully.
 *
 *-------------------------------------------------------------------------
 */

#include "pg_backup.h"
#include "pg_backup_archiver.h"
#include "pg_backup_tar.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

static void _ArchiveEntry(ArchiveHandle *AH, TocEntry *te);
static void _StartData(ArchiveHandle *AH, TocEntry *te);
static int	_WriteData(ArchiveHandle *AH, const void *data, int dLen);
static void _EndData(ArchiveHandle *AH, TocEntry *te);
static int	_WriteByte(ArchiveHandle *AH, const int i);
static int	_ReadByte(ArchiveHandle *);
static int	_WriteBuf(ArchiveHandle *AH, const void *buf, int len);
static int	_ReadBuf(ArchiveHandle *AH, void *buf, int len);
static void _CloseArchive(ArchiveHandle *AH);
static void _PrintTocData(ArchiveHandle *AH, TocEntry *te, RestoreOptions *ropt);
static void _WriteExtraToc(ArchiveHandle *AH, TocEntry *te);
static void _ReadExtraToc(ArchiveHandle *AH, TocEntry *te);
static void _PrintExtraToc(ArchiveHandle *AH, TocEntry *te);

static void _StartBlobs(ArchiveHandle *AH, TocEntry *te);
static void _StartBlob(ArchiveHandle *AH, TocEntry *te, Oid oid);
static void _EndBlob(ArchiveHandle *AH, TocEntry *te, Oid oid);
static void _EndBlobs(ArchiveHandle *AH, TocEntry *te);

#define K_STD_BUF_SIZE 1024


#ifdef HAVE_LIBZ
 /* typedef gzFile	 ThingFile; */
typedef FILE ThingFile;

#else
typedef FILE ThingFile;
#endif

typedef struct
{
	ThingFile  *zFH;
	FILE	   *nFH;
	FILE	   *tarFH;
	FILE	   *tmpFH;
	char	   *targetFile;
	char		mode;
	int			pos;
	int			fileLen;
	ArchiveHandle *AH;
} TAR_MEMBER;

typedef struct
{
	int			hasSeek;
	int			filePos;
	TAR_MEMBER *blobToc;
	FILE	   *tarFH;
	int			tarFHpos;
	int			tarNextMember;
	TAR_MEMBER *FH;
	int			isSpecialScript;
	TAR_MEMBER *scriptTH;
} lclContext;

typedef struct
{
	TAR_MEMBER *TH;
	char	   *filename;
} lclTocEntry;

static char *modulename = gettext_noop("tar archiver");

static void _LoadBlobs(ArchiveHandle *AH, RestoreOptions *ropt);

static TAR_MEMBER *tarOpen(ArchiveHandle *AH, const char *filename, char mode);
static void tarClose(ArchiveHandle *AH, TAR_MEMBER *TH);

#ifdef __NOT_USED__
static char *tarGets(char *buf, int len, TAR_MEMBER *th);
#endif
static int	tarPrintf(ArchiveHandle *AH, TAR_MEMBER *th, const char *fmt,...);

static void _tarAddFile(ArchiveHandle *AH, TAR_MEMBER *th);
static int	_tarChecksum(char *th);
static TAR_MEMBER *_tarPositionTo(ArchiveHandle *AH, const char *filename);
static int	tarRead(void *buf, int len, TAR_MEMBER *th);
static int	tarWrite(const void *buf, int len, TAR_MEMBER *th);
static void _tarWriteHeader(TAR_MEMBER *th);
static int	_tarGetHeader(ArchiveHandle *AH, TAR_MEMBER *th);
static int	_tarReadRaw(ArchiveHandle *AH, void *buf, int len, TAR_MEMBER *th, FILE *fh);

static int	_scriptOut(ArchiveHandle *AH, const void *buf, int len);

/*
 *	Initializer
 */
void
InitArchiveFmt_Tar(ArchiveHandle *AH)
{
	lclContext *ctx;

	/* Assuming static functions, this can be copied for each format. */
	AH->ArchiveEntryPtr = _ArchiveEntry;
	AH->StartDataPtr = _StartData;
	AH->WriteDataPtr = _WriteData;
	AH->EndDataPtr = _EndData;
	AH->WriteBytePtr = _WriteByte;
	AH->ReadBytePtr = _ReadByte;
	AH->WriteBufPtr = _WriteBuf;
	AH->ReadBufPtr = _ReadBuf;
	AH->ClosePtr = _CloseArchive;
	AH->PrintTocDataPtr = _PrintTocData;
	AH->ReadExtraTocPtr = _ReadExtraToc;
	AH->WriteExtraTocPtr = _WriteExtraToc;
	AH->PrintExtraTocPtr = _PrintExtraToc;

	AH->StartBlobsPtr = _StartBlobs;
	AH->StartBlobPtr = _StartBlob;
	AH->EndBlobPtr = _EndBlob;
	AH->EndBlobsPtr = _EndBlobs;

	/*
	 * Set up some special context used in compressing data.
	 */
	ctx = (lclContext *) malloc(sizeof(lclContext));
	AH->formatData = (void *) ctx;
	ctx->filePos = 0;

	/*
	 * Now open the TOC file
	 */
	if (AH->mode == archModeWrite)
	{

		if (AH->fSpec && strcmp(AH->fSpec, "") != 0)
			ctx->tarFH = fopen(AH->fSpec, PG_BINARY_W);
		else
			ctx->tarFH = stdout;

		if (ctx->tarFH == NULL)
			die_horribly(NULL, modulename,
			"could not open TOC file for output: %s\n", strerror(errno));

		ctx->tarFHpos = 0;

		/*
		 * Make unbuffered since we will dup() it, and the buffers screw
		 * each other
		 */
		/* setvbuf(ctx->tarFH, NULL, _IONBF, 0); */

		ctx->hasSeek = (fseek(ctx->tarFH, 0, SEEK_CUR) == 0);

		if (AH->compression < 0 || AH->compression > 9)
			AH->compression = Z_DEFAULT_COMPRESSION;

		/* Don't compress into tar files unless asked to do so */
		if (AH->compression == Z_DEFAULT_COMPRESSION)
			AH->compression = 0;

		/*
		 * We don't support compression because reading the files back is
		 * not possible since gzdopen uses buffered IO which totally
		 * screws file positioning.
		 */
		if (AH->compression != 0)
			die_horribly(NULL, modulename, "compression not supported by tar output format\n");

	}
	else
	{							/* Read Mode */

		if (AH->fSpec && strcmp(AH->fSpec, "") != 0)
			ctx->tarFH = fopen(AH->fSpec, PG_BINARY_R);
		else
			ctx->tarFH = stdin;

		if (ctx->tarFH == NULL)
			die_horribly(NULL, modulename, "could not open TOC file for input: %s\n", strerror(errno));

		/*
		 * Make unbuffered since we will dup() it, and the buffers screw
		 * each other
		 */
		/* setvbuf(ctx->tarFH, NULL, _IONBF, 0); */

		ctx->tarFHpos = 0;

		ctx->hasSeek = (fseek(ctx->tarFH, 0, SEEK_CUR) == 0);

		/*
		 * Forcibly unmark the header as read since we use the lookahead
		 * buffer
		 */
		AH->readHeader = 0;

		ctx->FH = (void *) tarOpen(AH, "toc.dat", 'r');
		ReadHead(AH);
		ReadToc(AH);
		tarClose(AH, ctx->FH);	/* Nothing else in the file... */
	}

}

/*
 * - Start a new TOC entry
 *	 Setup the output file name.
 */
static void
_ArchiveEntry(ArchiveHandle *AH, TocEntry *te)
{
	lclTocEntry *ctx;
	char		fn[K_STD_BUF_SIZE];

	ctx = (lclTocEntry *) malloc(sizeof(lclTocEntry));
	if (te->dataDumper != NULL)
	{
#ifdef HAVE_LIBZ
		if (AH->compression == 0)
			sprintf(fn, "%d.dat", te->id);
		else
			sprintf(fn, "%d.dat.gz", te->id);
#else
		sprintf(fn, "%d.dat", te->id);
#endif
		ctx->filename = strdup(fn);
	}
	else
	{
		ctx->filename = NULL;
		ctx->TH = NULL;
	}
	te->formatData = (void *) ctx;
}

static void
_WriteExtraToc(ArchiveHandle *AH, TocEntry *te)
{
	lclTocEntry *ctx = (lclTocEntry *) te->formatData;

	if (ctx->filename)
		WriteStr(AH, ctx->filename);
	else
		WriteStr(AH, "");
}

static void
_ReadExtraToc(ArchiveHandle *AH, TocEntry *te)
{
	lclTocEntry *ctx = (lclTocEntry *) te->formatData;

	if (ctx == NULL)
	{
		ctx = (lclTocEntry *) malloc(sizeof(lclTocEntry));
		te->formatData = (void *) ctx;
	}

	ctx->filename = ReadStr(AH);
	if (strlen(ctx->filename) == 0)
	{
		free(ctx->filename);
		ctx->filename = NULL;
	}
	ctx->TH = NULL;
}

static void
_PrintExtraToc(ArchiveHandle *AH, TocEntry *te)
{
	lclTocEntry *ctx = (lclTocEntry *) te->formatData;

	if (ctx->filename != NULL)
		ahprintf(AH, "-- File: %s\n", ctx->filename);
}

static void
_StartData(ArchiveHandle *AH, TocEntry *te)
{
	lclTocEntry *tctx = (lclTocEntry *) te->formatData;

	tctx->TH = tarOpen(AH, tctx->filename, 'w');
}

static TAR_MEMBER *
tarOpen(ArchiveHandle *AH, const char *filename, char mode)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	TAR_MEMBER *tm;

#ifdef HAVE_LIBZ
	char		fmode[10];
#endif

	if (mode == 'r')
	{
		tm = _tarPositionTo(AH, filename);
		if (!tm)				/* Not found */
		{
			if (filename)		/* Couldn't find the requested file.
								 * Future: DO SEEK(0) and retry. */
				die_horribly(AH, modulename, "could not find file %s in archive\n", filename);
			else
/* Any file OK, non left, so return NULL */
				return NULL;
		}

#ifdef HAVE_LIBZ

		if (AH->compression == 0)
			tm->nFH = ctx->tarFH;
		else
			die_horribly(AH, modulename, "compression support is disabled in this format\n");
		/* tm->zFH = gzdopen(dup(fileno(ctx->tarFH)), "rb"); */

#else

		tm->nFH = ctx->tarFH;
#endif

	}
	else
	{
		tm = calloc(1, sizeof(TAR_MEMBER));

		tm->tmpFH = tmpfile();

		if (tm->tmpFH == NULL)
			die_horribly(AH, modulename, "could not generate temporary file name: %s\n", strerror(errno));

#ifdef HAVE_LIBZ

		if (AH->compression != 0)
		{
			sprintf(fmode, "wb%d", AH->compression);
			tm->zFH = gzdopen(dup(fileno(tm->tmpFH)), fmode);
			if (tm->zFH == NULL)
				die_horribly(AH, modulename, "could not gzdopen temporary file\n");

		}
		else
			tm->nFH = tm->tmpFH;

#else

		tm->nFH = tm->tmpFH;
#endif

		tm->AH = AH;
		tm->targetFile = strdup(filename);
	}

	tm->mode = mode;
	tm->tarFH = ctx->tarFH;

	return tm;

}

static void
tarClose(ArchiveHandle *AH, TAR_MEMBER *th)
{
	/*
	 * Close the GZ file since we dup'd. This will flush the buffers.
	 */
	if (AH->compression != 0)
		if (GZCLOSE(th->zFH) != 0)
			die_horribly(AH, modulename, "could not close tar member\n");

	if (th->mode == 'w')
		_tarAddFile(AH, th);	/* This will close the temp file */

	/*
	 * else Nothing to do for normal read since we don't dup() normal file
	 * handle, and we don't use temp files.
	 */

	if (th->targetFile)
		free(th->targetFile);

	th->nFH = NULL;
	th->zFH = NULL;
}

#ifdef __NOT_USED__
static char *
tarGets(char *buf, int len, TAR_MEMBER *th)
{
	char	   *s;
	int			cnt = 0;
	char		c = ' ';
	int			eof = 0;

	/* Can't read past logical EOF */
	if (len > (th->fileLen - th->pos))
		len = th->fileLen - th->pos;

	while (cnt < len && c != '\n')
	{
		if (_tarReadRaw(th->AH, &c, 1, th, NULL) <= 0)
		{
			eof = 1;
			break;
		}
		buf[cnt++] = c;
	}

	if (eof && cnt == 0)
		s = NULL;
	else
	{
		buf[cnt++] = '\0';
		s = buf;
	}

	if (s)
	{
		len = strlen(s);
		th->pos += len;
	}

	return s;
}
#endif

/*
 * Just read bytes from the archive. This is the low level read routine
 * that is used for ALL reads on a tar file.
 */
static int
_tarReadRaw(ArchiveHandle *AH, void *buf, int len, TAR_MEMBER *th, FILE *fh)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	int			avail;
	int			used = 0;
	int			res = 0;

	avail = AH->lookaheadLen - AH->lookaheadPos;
	if (avail > 0)
	{
		/* We have some lookahead bytes to use */
		if (avail >= len)		/* Just use the lookahead buffer */
			used = len;
		else
			used = avail;

		/* Copy, and adjust buffer pos */
		memcpy(buf, AH->lookahead, used);
		AH->lookaheadPos += used;

		/* Adjust required length */
		len -= used;
	}

	/* Read the file if len > 0 */
	if (len > 0)
	{
		if (fh)
			res = fread(&((char *) buf)[used], 1, len, fh);
		else if (th)
		{
			if (th->zFH)
				res = GZREAD(&((char *) buf)[used], 1, len, th->zFH);
			else
				res = fread(&((char *) buf)[used], 1, len, th->nFH);
		}
		else
			die_horribly(AH, modulename, "neither th nor fh specified in tarReadRaw() (internal error)\n");
	}

#if 0
	write_msg(modulename, "requested %d bytes, got %d from lookahead and %d from file\n",
			  reqLen, used, res);
#endif

	ctx->tarFHpos += res + used;

	return (res + used);
}

static int
tarRead(void *buf, int len, TAR_MEMBER *th)
{
	int			res;

	if (th->pos + len > th->fileLen)
		len = th->fileLen - th->pos;

	if (len <= 0)
		return 0;

	res = _tarReadRaw(th->AH, buf, len, th, NULL);

	th->pos += res;

	return res;
}

static int
tarWrite(const void *buf, int len, TAR_MEMBER *th)
{
	int			res;

	if (th->zFH != 0)
		res = GZWRITE((void *) buf, 1, len, th->zFH);
	else
		res = fwrite(buf, 1, len, th->nFH);

	if (res != len)
		die_horribly(th->AH, modulename,
					 "could not write to tar member (wrote %d, attempted %d)\n", res, len);

	th->pos += res;
	return res;
}

static int
_WriteData(ArchiveHandle *AH, const void *data, int dLen)
{
	lclTocEntry *tctx = (lclTocEntry *) AH->currToc->formatData;

	dLen = tarWrite((void *) data, dLen, tctx->TH);

	return dLen;
}

static void
_EndData(ArchiveHandle *AH, TocEntry *te)
{
	lclTocEntry *tctx = (lclTocEntry *) te->formatData;

	/* Close the file */
	tarClose(AH, tctx->TH);
	tctx->TH = NULL;
}

/*
 * Print data for a given file
 */
static void
_PrintFileData(ArchiveHandle *AH, char *filename, RestoreOptions *ropt)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	char		buf[4096];
	int			cnt;
	TAR_MEMBER *th;

	if (!filename)
		return;

	th = tarOpen(AH, filename, 'r');
	ctx->FH = th;

	while ((cnt = tarRead(buf, 4095, th)) > 0)
	{
		buf[cnt] = '\0';
		ahwrite(buf, 1, cnt, AH);
	}

	tarClose(AH, th);
}


/*
 * Print data for a given TOC entry
*/
static void
_PrintTocData(ArchiveHandle *AH, TocEntry *te, RestoreOptions *ropt)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	lclTocEntry *tctx = (lclTocEntry *) te->formatData;
	char	   *tmpCopy;
	int			i,
				pos1,
				pos2;

	if (!tctx->filename)
		return;

	if (ctx->isSpecialScript)
	{
		if (!te->copyStmt)
			return;

		/* Abort the default COPY */
		ahprintf(AH, "\\.\n");

		/* Get a copy of the COPY statement and clean it up */
		tmpCopy = strdup(te->copyStmt);
		for (i = 0; i < strlen(tmpCopy); i++)
			tmpCopy[i] = tolower((unsigned char) tmpCopy[i]);

		/*
		 * This is very nasty; we don't know if the archive used WITH
		 * OIDS, so we search the string for it in a paranoid sort of way.
		 */
		if (strncmp(tmpCopy, "copy ", 5) != 0)
			die_horribly(AH, modulename,
						 "bad COPY statement - could not find \"copy\" in string \"%s\"\n", tmpCopy);

		pos1 = 5;
		for (pos1 = 5; pos1 < strlen(tmpCopy); pos1++)
			if (tmpCopy[pos1] != ' ')
				break;

		if (tmpCopy[pos1] == '"')
			pos1 += 2;

		pos1 += strlen(te->name);

		for (pos2 = pos1; pos2 < strlen(tmpCopy); pos2++)
			if (strncmp(&tmpCopy[pos2], "from stdin", 10) == 0)
				break;

		if (pos2 >= strlen(tmpCopy))
			die_horribly(AH, modulename,
						 "bad COPY statement - could not find \"from stdin\" in string \"%s\" starting at position %d\n",
						 tmpCopy, pos1);

		ahwrite(tmpCopy, 1, pos2, AH);	/* 'copy "table" [with oids]' */
		ahprintf(AH, " from '$$PATH$$/%s' %s", tctx->filename, &tmpCopy[pos2 + 10]);

		return;
	}

	if (strcmp(te->desc, "BLOBS") == 0)
		_LoadBlobs(AH, ropt);
	else
		_PrintFileData(AH, tctx->filename, ropt);
}

/* static void _getBlobTocEntry(ArchiveHandle* AH, int *oid, char fname[K_STD_BUF_SIZE])
 * {
 *	lclContext*		ctx = (lclContext*)AH->formatData;
 *	char			blobTe[K_STD_BUF_SIZE];
 *	int				fpos;
 *	int				eos;
 *
 *	if (tarGets(&blobTe[0], K_STD_BUF_SIZE - 1, ctx->blobToc) != NULL)
 *	{
 *		*oid = atoi(blobTe);
 *
 *		fpos = strcspn(blobTe, " ");
 *
 *		strncpy(fname, &blobTe[fpos+1], K_STD_BUF_SIZE - 1);
 *
 *		eos = strlen(fname)-1;
 *
 *		if (fname[eos] == '\n')
 *			fname[eos] = '\0';
 *
 *	} else {
 *
 *		*oid = 0;
 *		fname[0] = '\0';
 *	}
 *}
 */

static void
_LoadBlobs(ArchiveHandle *AH, RestoreOptions *ropt)
{
	Oid			oid;
	lclContext *ctx = (lclContext *) AH->formatData;
	TAR_MEMBER *th;
	int			cnt;
	char		buf[4096];

	StartRestoreBlobs(AH);

	th = tarOpen(AH, NULL, 'r');	/* Open next file */
	while (th != NULL)
	{
		ctx->FH = th;

		oid = (Oid) strtoul(&th->targetFile[5], NULL, 10);

		if (strncmp(th->targetFile, "blob_", 5) == 0 && oid != 0)
		{
			ahlog(AH, 1, "restoring large object OID %u\n", oid);

			StartRestoreBlob(AH, oid);

			while ((cnt = tarRead(buf, 4095, th)) > 0)
			{
				buf[cnt] = '\0';
				ahwrite(buf, 1, cnt, AH);
			}
			EndRestoreBlob(AH, oid);
		}

		tarClose(AH, th);

		th = tarOpen(AH, NULL, 'r');
	}

	EndRestoreBlobs(AH);

}


static int
_WriteByte(ArchiveHandle *AH, const int i)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	int			res;
	char		b = i;			/* Avoid endian problems */

	res = tarWrite(&b, 1, ctx->FH);
	if (res != EOF)
		ctx->filePos += res;
	return res;
}

static int
_ReadByte(ArchiveHandle *AH)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	int			res;
	char		c = '\0';

	res = tarRead(&c, 1, ctx->FH);
	if (res != EOF)
		ctx->filePos += res;
	return c;
}

static int
_WriteBuf(ArchiveHandle *AH, const void *buf, int len)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	int			res;

	res = tarWrite((void *) buf, len, ctx->FH);
	ctx->filePos += res;
	return res;
}

static int
_ReadBuf(ArchiveHandle *AH, void *buf, int len)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	int			res;

	res = tarRead(buf, len, ctx->FH);
	ctx->filePos += res;
	return res;
}

static void
_CloseArchive(ArchiveHandle *AH)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	TAR_MEMBER *th;
	RestoreOptions *ropt;
	int			savVerbose,
				i;

	if (AH->mode == archModeWrite)
	{
		/*
		 * Write the Header & TOC to the archive FIRST
		 */
		th = tarOpen(AH, "toc.dat", 'w');
		ctx->FH = th;
		WriteHead(AH);
		WriteToc(AH);
		tarClose(AH, th);		/* Not needed any more */

		/*
		 * Now send the data (tables & blobs)
		 */
		WriteDataChunks(AH);

		/*
		 * Now this format wants to append a script which does a full
		 * restore if the files have been extracted.
		 */
		th = tarOpen(AH, "restore.sql", 'w');
		tarPrintf(AH, th, "create temporary table pgdump_restore_path(p text);\n");
		tarPrintf(AH, th, "--\n"
				  "-- NOTE:\n"
				  "--\n"
			 "-- File paths need to be edited. Search for $$PATH$$ and\n"
			  "-- replace it with the path to the directory containing\n"
				  "-- the extracted data files.\n"
				  "--\n"
				  "-- Edit the following to match the path where the\n"
				  "-- tar archive has been extracted.\n"
				  "--\n");
		tarPrintf(AH, th, "insert into pgdump_restore_path values('/tmp');\n\n");

		AH->CustomOutPtr = _scriptOut;

		ctx->isSpecialScript = 1;
		ctx->scriptTH = th;

		ropt = NewRestoreOptions();
		ropt->dropSchema = 1;
		ropt->compression = 0;
		ropt->superuser = PQuser(AH->connection);
		ropt->suppressDumpWarnings = true;

		savVerbose = AH->public.verbose;
		AH->public.verbose = 0;

		RestoreArchive((Archive *) AH, ropt);

		AH->public.verbose = savVerbose;

		tarClose(AH, th);

		/* Add a block of NULLs since it's de-rigeur. */
		for (i = 0; i < 512; i++)
		{
			if (fputc(0, ctx->tarFH) == EOF)
				die_horribly(AH, modulename,
				   "could not write null block at end of tar archive\n");
		}

	}

	AH->FH = NULL;
}

static int
_scriptOut(ArchiveHandle *AH, const void *buf, int len)
{
	lclContext *ctx = (lclContext *) AH->formatData;

	return tarWrite(buf, len, ctx->scriptTH);
}

/*
 * BLOB support
 */

/*
 * Called by the archiver when starting to save all BLOB DATA (not schema).
 * This routine should save whatever format-specific information is needed
 * to read the BLOBs back into memory.
 *
 * It is called just prior to the dumper's DataDumper routine.
 *
 * Optional, but strongly recommended.
 *
 */
static void
_StartBlobs(ArchiveHandle *AH, TocEntry *te)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	char		fname[K_STD_BUF_SIZE];

	sprintf(fname, "blobs.toc");
	ctx->blobToc = tarOpen(AH, fname, 'w');

}

/*
 * Called by the archiver when the dumper calls StartBlob.
 *
 * Mandatory.
 *
 * Must save the passed OID for retrieval at restore-time.
 */
static void
_StartBlob(ArchiveHandle *AH, TocEntry *te, Oid oid)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	lclTocEntry *tctx = (lclTocEntry *) te->formatData;
	char		fname[255];
	char	   *sfx;

	if (oid == 0)
		die_horribly(AH, modulename, "invalid OID for large object (%u)\n", oid);

	if (AH->compression != 0)
		sfx = ".gz";
	else
		sfx = "";

	sprintf(fname, "blob_%d.dat%s", oid, sfx);

	tarPrintf(AH, ctx->blobToc, "%d %s\n", oid, fname);

	tctx->TH = tarOpen(AH, fname, 'w');

}

/*
 * Called by the archiver when the dumper calls EndBlob.
 *
 * Optional.
 *
 */
static void
_EndBlob(ArchiveHandle *AH, TocEntry *te, Oid oid)
{
	lclTocEntry *tctx = (lclTocEntry *) te->formatData;

	tarClose(AH, tctx->TH);
}

/*
 * Called by the archiver when finishing saving all BLOB DATA.
 *
 * Optional.
 *
 */
static void
_EndBlobs(ArchiveHandle *AH, TocEntry *te)
{
	lclContext *ctx = (lclContext *) AH->formatData;

	/* Write out a fake zero OID to mark end-of-blobs. */
	/* WriteInt(AH, 0); */

	tarClose(AH, ctx->blobToc);

}



/*------------
 * TAR Support
 *------------
 */

static int
tarPrintf(ArchiveHandle *AH, TAR_MEMBER *th, const char *fmt,...)
{
	char	   *p = NULL;
	va_list		ap;
	int			bSize = strlen(fmt) + 256;		/* Should be enough */
	int			cnt = -1;

	/*
	 * This is paranoid: deal with the possibility that vsnprintf is
	 * willing to ignore trailing null
	 */

	/*
	 * or returns > 0 even if string does not fit. It may be the case that
	 * it returns cnt = bufsize
	 */
	while (cnt < 0 || cnt >= (bSize - 1))
	{
		if (p != NULL)
			free(p);
		bSize *= 2;
		p = (char *) malloc(bSize);
		if (p == NULL)
			die_horribly(AH, modulename, "out of memory\n");
		va_start(ap, fmt);
		cnt = vsnprintf(p, bSize, fmt, ap);
		va_end(ap);
	}
	cnt = tarWrite(p, cnt, th);
	free(p);
	return cnt;
}

static int
_tarChecksum(char *header)
{
	int			i,
				sum;

	sum = 0;
	for (i = 0; i < 512; i++)
		if (i < 148 || i >= 156)
			sum += 0xFF & header[i];
	return sum + 256;			/* Assume 8 blanks in checksum field */
}

int
isValidTarHeader(char *header)
{
	int			sum;
	int			chk = _tarChecksum(header);

	sscanf(&header[148], "%8o", &sum);

	return (sum == chk && strncmp(&header[257], "ustar  ", 7) == 0);
}

/* Given the member, write the TAR header & copy the file */
static void
_tarAddFile(ArchiveHandle *AH, TAR_MEMBER *th)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	FILE	   *tmp = th->tmpFH;	/* Grab it for convenience */
	char		buf[32768];
	int			cnt;
	int			len = 0;
	int			res;
	int			i,
				pad;

	/*
	 * Find file len & go back to start.
	 */
	fseek(tmp, 0, SEEK_END);
	th->fileLen = ftell(tmp);
	fseek(tmp, 0, SEEK_SET);

	_tarWriteHeader(th);

	while ((cnt = fread(&buf[0], 1, 32767, tmp)) > 0)
	{
		res = fwrite(&buf[0], 1, cnt, th->tarFH);
		if (res != cnt)
			die_horribly(AH, modulename, "write error appending to tar archive (wrote %d, attempted %d)\n", res, cnt);
		len += res;
	}

	if (fclose(tmp) != 0)		/* This *should* delete it... */
		die_horribly(AH, modulename, "could not close tar member: %s\n", strerror(errno));

	if (len != th->fileLen)
		die_horribly(AH, modulename, "actual file length (%d) does not match expected (%d)\n",
					 len, th->pos);

	pad = ((len + 511) & ~511) - len;
	for (i = 0; i < pad; i++)
	{
		if (fputc('\0', th->tarFH) == EOF)
			die_horribly(AH, modulename, "could not output padding at end of tar member\n");
	}

	ctx->tarFHpos += len + pad;
}

/* Locate the file in the archive, read header and position to data */
static TAR_MEMBER *
_tarPositionTo(ArchiveHandle *AH, const char *filename)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	TAR_MEMBER *th = calloc(1, sizeof(TAR_MEMBER));
	char		c;
	char		header[512];
	int			i,
				len,
				blks,
				id;

	th->AH = AH;

	/* Go to end of current file, if any */
	if (ctx->tarFHpos != 0)
	{
		ahlog(AH, 4, "moving from position %d (%x) to next member at file position %d (%x)\n",
			  ctx->tarFHpos, ctx->tarFHpos,
			  ctx->tarNextMember, ctx->tarNextMember);

		while (ctx->tarFHpos < ctx->tarNextMember)
			_tarReadRaw(AH, &c, 1, NULL, ctx->tarFH);
	}

	ahlog(AH, 4, "now at file position %d (%x)\n", ctx->tarFHpos, ctx->tarFHpos);

	/* We are at the start of the file. or at the next member */

	/* Get the header */
	if (!_tarGetHeader(AH, th))
	{
		if (filename)
			die_horribly(AH, modulename, "could not find header for file %s in tar archive\n", filename);
		else
/* We're just scanning the archibe for the next file, so return null */
		{
			free(th);
			return NULL;
		}
	}

	while (filename != NULL && strcmp(th->targetFile, filename) != 0)
	{
		ahlog(AH, 4, "skipping tar member %s\n", th->targetFile);

		id = atoi(th->targetFile);
		if ((TocIDRequired(AH, id, AH->ropt) & 2) != 0)
			die_horribly(AH, modulename, "dumping data out of order is not supported in this archive format: "
			"%s is required, but comes before %s in the archive file.\n",
						 th->targetFile, filename);

		/* Header doesn't match, so read to next header */
		len = ((th->fileLen + 511) & ~511);		/* Padded length */
		blks = len >> 9;		/* # of 512 byte blocks */

		for (i = 0; i < blks; i++)
			_tarReadRaw(AH, &header[0], 512, NULL, ctx->tarFH);

		if (!_tarGetHeader(AH, th))
			die_horribly(AH, modulename, "could not find header for file %s in tar archive\n", filename);

	}

	ctx->tarNextMember = ctx->tarFHpos + ((th->fileLen + 511) & ~511);
	th->pos = 0;

	return th;
}

/* Read & verify a header */
static int
_tarGetHeader(ArchiveHandle *AH, TAR_MEMBER *th)
{
	lclContext *ctx = (lclContext *) AH->formatData;
	char		h[512];
	char		name[100];
	int			sum,
				chk;
	int			len;
	int			hPos;
	int			i;
	bool		gotBlock = false;

	while (!gotBlock)
	{
#if 0
		if (ftell(ctx->tarFH) != ctx->tarFHpos)
			die_horribly(AH, modulename,
						 "mismatch in actual vs. predicted file position (%d vs. %d)\n",
						 ftell(ctx->tarFH), ctx->tarFHpos);
#endif

		/* Save the pos for reporting purposes */
		hPos = ctx->tarFHpos;

		/* Read a 512 byte block, return EOF, exit if short */
		len = _tarReadRaw(AH, &h[0], 512, NULL, ctx->tarFH);
		if (len == 0)			/* EOF */
			return 0;

		if (len != 512)
			die_horribly(AH, modulename, "incomplete tar header found (%d bytes)\n", len);

		/* Calc checksum */
		chk = _tarChecksum(&h[0]);

		/*
		 * If the checksum failed, see if it is a null block. If so, then
		 * just try with next block...
		 */

		if (chk == sum)
			gotBlock = true;
		else
		{
			for (i = 0; i < 512; i++)
			{
				if (h[0] != 0)
				{
					gotBlock = true;
					break;
				}
			}
		}
	}

	sscanf(&h[0], "%99s", &name[0]);
	sscanf(&h[124], "%12o", &len);
	sscanf(&h[148], "%8o", &sum);

	ahlog(AH, 3, "TOC Entry %s at %d (length %d, checksum %d)\n", &name[0], hPos, len, sum);

	if (chk != sum)
		die_horribly(AH, modulename,
					 "corrupt tar header found in %s "
		"(expected %d (%o), computed %d (%o)) file position %ld (%lx)\n",
					 &name[0], sum, sum, chk, chk, ftell(ctx->tarFH), ftell(ctx->tarFH));

	th->targetFile = strdup(name);
	th->fileLen = len;

	return 1;
}

static void
_tarWriteHeader(TAR_MEMBER *th)
{
	char		h[512];
	int			i;
	int			lastSum = 0;
	int			sum;

	for (i = 0; i < 512; i++)
		h[i] = '\0';

	/* Name 100 */
	sprintf(&h[0], "%.99s", th->targetFile);

	/* Mode 8 */
	sprintf(&h[100], "100600 ");

	/* User ID 8 */
	sprintf(&h[108], " 04000 ");

	/* Group 8 */
	sprintf(&h[116], " 02000 ");

	/* File size 12 */
	sprintf(&h[124], "%10o ", th->fileLen);

	/* Mod Time 12 */
	sprintf(&h[136], "%10o ", (int) time(NULL));

	/* Checksum 8 */
	sprintf(&h[148], "%6o ", lastSum);

	/* Type 1 */
	/* sprintf(&h[156], "%c", LF_NORMAL); */
	sprintf(&h[156], "0");

	/* Link name 100 (NULL) */

	/* Magic 8 */
	sprintf(&h[257], "ustar  ");

	/*
	 * GNU Version... sprintf(&h[257], "ustar"); sprintf(&h[263], "00");
	 */

	/* User 32 */
	sprintf(&h[265], "%.31s", "");		/* How do I get username reliably?
										 * Do I need to? */

	/* Group 32 */
	sprintf(&h[297], "%.31s", "");		/* How do I get group reliably? Do
										 * I need to? */

	/* Maj Dev 8 */
	/* sprintf(&h[329], "%6o ", 0); */

	/* Min Dev */
	/* sprintf(&h[337], "%6o ", 0); */


	while ((sum = _tarChecksum(h)) != lastSum)
	{
		sprintf(&h[148], "%6o ", sum);
		lastSum = sum;
	}

	if (fwrite(h, 1, 512, th->tarFH) != 512)
		die_horribly(th->AH, modulename, "unable to write tar header\n");

}
