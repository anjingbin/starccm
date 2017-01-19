/*-------------------------------------------------------------------------
 *
 *	 FILE
 *		fe-misc.c
 *
 *	 DESCRIPTION
 *		 miscellaneous useful functions
 *
 * The communication routines here are analogous to the ones in
 * backend/libpq/pqcomm.c and backend/libpq/pqcomprim.c, but operate
 * in the considerably different environment of the frontend libpq.
 * In particular, we work with a bare nonblock-mode socket, rather than
 * a stdio stream, so that we can avoid unwanted blocking of the application.
 *
 * XXX: MOVE DEBUG PRINTOUT TO HIGHER LEVEL.  As is, block and restart
 * will cause repeat printouts.
 *
 * We must speak the same transmitted data representations as the backend
 * routines.  Note that this module supports *only* network byte order
 * for transmitted ints, whereas the backend modules (as of this writing)
 * still handle either network or little-endian byte order.
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/interfaces/libpq/fe-misc.c,v 1.2 2004/07/05 08:56:13 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres_fe.h"

#include <errno.h>
#include <signal.h>
#include <time.h>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "win32.h"
#else
#include <unistd.h>
#include <sys/time.h>
#endif

#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif

#include "libpq-fe.h"
#include "libpq-int.h"
#include "pqsignal.h"

#ifdef MULTIBYTE
#include "mb/pg_wchar.h"
#endif


#define DONOTICE(conn,message) \
	((*(conn)->noticeHook) ((conn)->noticeArg, (message)))

static int	pqPutBytes(const char *s, size_t nbytes, PGconn *conn);


/*
 * pqGetc:
 *	get a character from the connection
 *
 *	All these routines return 0 on success, EOF on error.
 *	Note that for the Get routines, EOF only means there is not enough
 *	data in the buffer, not that there is necessarily a hard error.
 */
int
pqGetc(char *result, PGconn *conn)
{
	if (conn->inCursor >= conn->inEnd)
		return EOF;

	*result = conn->inBuffer[conn->inCursor++];

	if (conn->Pfdebug)
		fprintf(conn->Pfdebug, "From backend> %c\n", *result);

	return 0;
}


/*
 * write 1 char to the connection
 */
int
pqPutc(char c, PGconn *conn)
{
	if (pqPutBytes(&c, 1, conn) == EOF)
		return EOF;

	if (conn->Pfdebug)
		fprintf(conn->Pfdebug, "To backend> %c\n", c);

	return 0;
}


/*
 * pqPutBytes: local routine to write N bytes to the connection,
 * with buffering
 */
static int
pqPutBytes(const char *s, size_t nbytes, PGconn *conn)
{
	size_t		avail = Max(conn->outBufSize - conn->outCount, 0);

	/*
	 * if we are non-blocking and the send queue is too full to buffer
	 * this request then try to flush some and return an error
	 */
	if (pqIsnonblocking(conn) && nbytes > avail && pqFlush(conn))
	{
		/*
		 * even if the flush failed we may still have written some data,
		 * recalculate the size of the send-queue relative to the amount
		 * we have to send, we may be able to queue it afterall even
		 * though it's not sent to the database it's ok, any routines that
		 * check the data coming from the database better call pqFlush()
		 * anyway.
		 */
		if (nbytes > Max(conn->outBufSize - conn->outCount, 0))
		{
			printfPQExpBuffer(&conn->errorMessage,
							  libpq_gettext("could not flush enough data (space available: %d, space needed %d)\n"),
						 (int) Max(conn->outBufSize - conn->outCount, 0),
							  (int) nbytes);
			return EOF;
		}
		/* fixup avail for while loop */
		avail = Max(conn->outBufSize - conn->outCount, 0);
	}

	/*
	 * is the amount of data to be sent is larger than the size of the
	 * output buffer then we must flush it to make more room.
	 *
	 * the code above will make sure the loop conditional is never true for
	 * non-blocking connections
	 */
	while (nbytes > avail)
	{
		memcpy(conn->outBuffer + conn->outCount, s, avail);
		conn->outCount += avail;
		s += avail;
		nbytes -= avail;
		if (pqFlush(conn))
			return EOF;
		avail = conn->outBufSize;
	}

	memcpy(conn->outBuffer + conn->outCount, s, nbytes);
	conn->outCount += nbytes;

	return 0;
}

/*
 * pqGets:
 * get a null-terminated string from the connection,
 * and store it in an expansible PQExpBuffer.
 * If we run out of memory, all of the string is still read,
 * but the excess characters are silently discarded.
 */
int
pqGets(PQExpBuffer buf, PGconn *conn)
{
	/* Copy conn data to locals for faster search loop */
	char	   *inBuffer = conn->inBuffer;
	int			inCursor = conn->inCursor;
	int			inEnd = conn->inEnd;
	int			slen;

	while (inCursor < inEnd && inBuffer[inCursor])
		inCursor++;

	if (inCursor >= inEnd)
		return EOF;

	slen = inCursor - conn->inCursor;

	resetPQExpBuffer(buf);
	appendBinaryPQExpBuffer(buf, inBuffer + conn->inCursor, slen);

	conn->inCursor = ++inCursor;

	if (conn->Pfdebug)
		fprintf(conn->Pfdebug, "From backend> \"%s\"\n",
				buf->data);

	return 0;
}


int
pqPuts(const char *s, PGconn *conn)
{
	if (pqPutBytes(s, strlen(s) + 1, conn))
		return EOF;

	if (conn->Pfdebug)
		fprintf(conn->Pfdebug, "To backend> %s\n", s);

	return 0;
}

/*
 * pqGetnchar:
 *	get a string of exactly len bytes in buffer s, no null termination
 */
int
pqGetnchar(char *s, size_t len, PGconn *conn)
{
	if (len < 0 || len > conn->inEnd - conn->inCursor)
		return EOF;

	memcpy(s, conn->inBuffer + conn->inCursor, len);
	/* no terminating null */

	conn->inCursor += len;

	if (conn->Pfdebug)
		fprintf(conn->Pfdebug, "From backend (%lu)> %.*s\n", (unsigned long) len, (int) len, s);

	return 0;
}

/*
 * pqPutnchar:
 *	send a string of exactly len bytes, no null termination needed
 */
int
pqPutnchar(const char *s, size_t len, PGconn *conn)
{
	if (pqPutBytes(s, len, conn))
		return EOF;

	if (conn->Pfdebug)
		fprintf(conn->Pfdebug, "To backend> %.*s\n", (int) len, s);

	return 0;
}

/*
 * pgGetInt
 *	read a 2 or 4 byte integer and convert from network byte order
 *	to local byte order
 */
int
pqGetInt(int *result, size_t bytes, PGconn *conn)
{
	uint16		tmp2;
	uint32		tmp4;
	char		noticeBuf[64];

	switch (bytes)
	{
		case 2:
			if (conn->inCursor + 2 > conn->inEnd)
				return EOF;
			memcpy(&tmp2, conn->inBuffer + conn->inCursor, 2);
			conn->inCursor += 2;
			*result = (int) ntohs(tmp2);
			break;
		case 4:
			if (conn->inCursor + 4 > conn->inEnd)
				return EOF;
			memcpy(&tmp4, conn->inBuffer + conn->inCursor, 4);
			conn->inCursor += 4;
			*result = (int) ntohl(tmp4);
			break;
		default:
			snprintf(noticeBuf, sizeof(noticeBuf),
					 libpq_gettext("integer of size %lu not supported by pqGetInt\n"),
					 (unsigned long) bytes);
			DONOTICE(conn, noticeBuf);
			return EOF;
	}

	if (conn->Pfdebug)
		fprintf(conn->Pfdebug, "From backend (#%lu)> %d\n", (unsigned long) bytes, *result);

	return 0;
}

/*
 * pgPutInt
 * send an integer of 2 or 4 bytes, converting from host byte order
 * to network byte order.
 */
int
pqPutInt(int value, size_t bytes, PGconn *conn)
{
	uint16		tmp2;
	uint32		tmp4;
	char		noticeBuf[64];

	switch (bytes)
	{
		case 2:
			tmp2 = htons((uint16) value);
			if (pqPutBytes((const char *) &tmp2, 2, conn))
				return EOF;
			break;
		case 4:
			tmp4 = htonl((uint32) value);
			if (pqPutBytes((const char *) &tmp4, 4, conn))
				return EOF;
			break;
		default:
			snprintf(noticeBuf, sizeof(noticeBuf),
					 libpq_gettext("integer of size %lu not supported by pqPutInt\n"),
					 (unsigned long) bytes);
			DONOTICE(conn, noticeBuf);
			return EOF;
	}

	if (conn->Pfdebug)
		fprintf(conn->Pfdebug, "To backend (%lu#)> %d\n", (unsigned long) bytes, value);

	return 0;
}

/*
 * pqReadReady: is select() saying the file is ready to read?
 * Returns -1 on failure, 0 if not ready, 1 if ready.
 */
int
pqReadReady(PGconn *conn)
{
	fd_set		input_mask;
	struct timeval timeout;

	if (!conn || conn->sock < 0)
		return -1;

retry:
	FD_ZERO(&input_mask);
	FD_SET(conn->sock, &input_mask);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if (select(conn->sock + 1, &input_mask, (fd_set *) NULL, (fd_set *) NULL,
			   &timeout) < 0)
	{
		if (SOCK_ERRNO == EINTR)
			/* Interrupted system call - we'll just try again */
			goto retry;

		printfPQExpBuffer(&conn->errorMessage,
						  libpq_gettext("select() failed: %s\n"),
						  SOCK_STRERROR(SOCK_ERRNO));
		return -1;
	}

	return FD_ISSET(conn->sock, &input_mask) ? 1 : 0;
}

/*
 * pqWriteReady: is select() saying the file is ready to write?
 * Returns -1 on failure, 0 if not ready, 1 if ready.
 */
int
pqWriteReady(PGconn *conn)
{
	fd_set		input_mask;
	struct timeval timeout;

	if (!conn || conn->sock < 0)
		return -1;

retry:
	FD_ZERO(&input_mask);
	FD_SET(conn->sock, &input_mask);
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	if (select(conn->sock + 1, (fd_set *) NULL, &input_mask, (fd_set *) NULL,
			   &timeout) < 0)
	{
		if (SOCK_ERRNO == EINTR)
			/* Interrupted system call - we'll just try again */
			goto retry;

		printfPQExpBuffer(&conn->errorMessage,
						  libpq_gettext("select() failed: %s\n"),
						  SOCK_STRERROR(SOCK_ERRNO));
		return -1;
	}
	return FD_ISSET(conn->sock, &input_mask) ? 1 : 0;
}

/* ----------
 * pqReadData: read more data, if any is available
 * Possible return values:
 *	 1: successfully loaded at least one more byte
 *	 0: no data is presently available, but no error detected
 *	-1: error detected (including EOF = connection closure);
 *		conn->errorMessage set
 * NOTE: callers must not assume that pointers or indexes into conn->inBuffer
 * remain valid across this call!
 * ----------
 */
int
pqReadData(PGconn *conn)
{
	int			someread = 0;
	int			nread;

	if (conn->sock < 0)
	{
		printfPQExpBuffer(&conn->errorMessage,
						  libpq_gettext("connection not open\n"));
		return -1;
	}

	/* Left-justify any data in the buffer to make room */
	if (conn->inStart < conn->inEnd)
	{
		if (conn->inStart > 0)
		{
			memmove(conn->inBuffer, conn->inBuffer + conn->inStart,
					conn->inEnd - conn->inStart);
			conn->inEnd -= conn->inStart;
			conn->inCursor -= conn->inStart;
			conn->inStart = 0;
		}
	}
	else
	{
		/* buffer is logically empty, reset it */
		conn->inStart = conn->inCursor = conn->inEnd = 0;
	}

	/*
	 * If the buffer is fairly full, enlarge it. We need to be able to
	 * enlarge the buffer in case a single message exceeds the initial
	 * buffer size.  We enlarge before filling the buffer entirely so as
	 * to avoid asking the kernel for a partial packet. The magic constant
	 * here should be large enough for a TCP packet or Unix pipe
	 * bufferload.	8K is the usual pipe buffer size, so...
	 */
	if (conn->inBufSize - conn->inEnd < 8192)
	{
		int			newSize = conn->inBufSize * 2;
		char	   *newBuf = (char *) realloc(conn->inBuffer, newSize);

		if (newBuf)
		{
			conn->inBuffer = newBuf;
			conn->inBufSize = newSize;
		}
	}

	/* OK, try to read some data */
tryAgain:
#ifdef USE_SSL
	if (conn->ssl)
		nread = SSL_read(conn->ssl, conn->inBuffer + conn->inEnd,
						 conn->inBufSize - conn->inEnd);
	else
#endif
		nread = recv(conn->sock, conn->inBuffer + conn->inEnd,
					 conn->inBufSize - conn->inEnd, 0);
	if (nread < 0)
	{
		if (SOCK_ERRNO == EINTR)
			goto tryAgain;
		/* Some systems return EAGAIN/EWOULDBLOCK for no data */
#ifdef EAGAIN
		if (SOCK_ERRNO == EAGAIN)
			return someread;
#endif
#if defined(EWOULDBLOCK) && (!defined(EAGAIN) || (EWOULDBLOCK != EAGAIN))
		if (SOCK_ERRNO == EWOULDBLOCK)
			return someread;
#endif
		/* We might get ECONNRESET here if using TCP and backend died */
#ifdef ECONNRESET
		if (SOCK_ERRNO == ECONNRESET)
			goto definitelyFailed;
#endif
		printfPQExpBuffer(&conn->errorMessage,
			   libpq_gettext("could not receive data from server: %s\n"),
						  SOCK_STRERROR(SOCK_ERRNO));
		return -1;
	}
	if (nread > 0)
	{
		conn->inEnd += nread;

		/*
		 * Hack to deal with the fact that some kernels will only give us
		 * back 1 packet per recv() call, even if we asked for more and
		 * there is more available.  If it looks like we are reading a
		 * long message, loop back to recv() again immediately, until we
		 * run out of data or buffer space.  Without this, the
		 * block-and-restart behavior of libpq's higher levels leads to
		 * O(N^2) performance on long messages.
		 *
		 * Since we left-justified the data above, conn->inEnd gives the
		 * amount of data already read in the current message.	We
		 * consider the message "long" once we have acquired 32k ...
		 */
		if (conn->inEnd > 32768 &&
			(conn->inBufSize - conn->inEnd) >= 8192)
		{
			someread = 1;
			goto tryAgain;
		}
		return 1;
	}

	if (someread)
		return 1;				/* got a zero read after successful tries */

	/*
	 * A return value of 0 could mean just that no data is now available,
	 * or it could mean EOF --- that is, the server has closed the
	 * connection. Since we have the socket in nonblock mode, the only way
	 * to tell the difference is to see if select() is saying that the
	 * file is ready. Grumble.	Fortunately, we don't expect this path to
	 * be taken much, since in normal practice we should not be trying to
	 * read data unless the file selected for reading already.
	 */
	switch (pqReadReady(conn))
	{
		case 0:
			/* definitely no data available */
			return 0;
		case 1:
			/* ready for read */
			break;
		default:
			goto definitelyFailed;
	}

	/*
	 * Still not sure that it's EOF, because some data could have just
	 * arrived.
	 */
tryAgain2:
#ifdef USE_SSL
	if (conn->ssl)
		nread = SSL_read(conn->ssl, conn->inBuffer + conn->inEnd,
						 conn->inBufSize - conn->inEnd);
	else
#endif
		nread = recv(conn->sock, conn->inBuffer + conn->inEnd,
					 conn->inBufSize - conn->inEnd, 0);
	if (nread < 0)
	{
		if (SOCK_ERRNO == EINTR)
			goto tryAgain2;
		/* Some systems return EAGAIN/EWOULDBLOCK for no data */
#ifdef EAGAIN
		if (SOCK_ERRNO == EAGAIN)
			return 0;
#endif
#if defined(EWOULDBLOCK) && (!defined(EAGAIN) || (EWOULDBLOCK != EAGAIN))
		if (SOCK_ERRNO == EWOULDBLOCK)
			return 0;
#endif
		/* We might get ECONNRESET here if using TCP and backend died */
#ifdef ECONNRESET
		if (SOCK_ERRNO == ECONNRESET)
			goto definitelyFailed;
#endif
		printfPQExpBuffer(&conn->errorMessage,
			   libpq_gettext("could not receive data from server: %s\n"),
						  SOCK_STRERROR(SOCK_ERRNO));
		return -1;
	}
	if (nread > 0)
	{
		conn->inEnd += nread;
		return 1;
	}

	/*
	 * OK, we are getting a zero read even though select() says ready.
	 * This means the connection has been closed.  Cope.
	 */
definitelyFailed:
	printfPQExpBuffer(&conn->errorMessage,
					  libpq_gettext(
							"server closed the connection unexpectedly\n"
			   "\tThis probably means the server terminated abnormally\n"
						 "\tbefore or while processing the request.\n"));
	conn->status = CONNECTION_BAD;		/* No more connection to backend */
#ifdef WIN32
	closesocket(conn->sock);
#else
	close(conn->sock);
#endif
	conn->sock = -1;

	return -1;
}

/*
 * pqFlush: send any data waiting in the output buffer
 */
int
pqFlush(PGconn *conn)
{
	char	   *ptr = conn->outBuffer;
	int			len = conn->outCount;

	if (conn->sock < 0)
	{
		printfPQExpBuffer(&conn->errorMessage,
						  libpq_gettext("connection not open\n"));
		return EOF;
	}

	/*
	 * don't try to send zero data, allows us to use this function without
	 * too much worry about overhead
	 */
	if (len == 0)
		return (0);

	/* while there's still data to send */
	while (len > 0)
	{
		/* Prevent being SIGPIPEd if backend has closed the connection. */
#ifndef WIN32
		pqsigfunc	oldsighandler = pqsignal(SIGPIPE, SIG_IGN);
#endif

		int			sent;

#ifdef USE_SSL
		if (conn->ssl)
			sent = SSL_write(conn->ssl, ptr, len);
		else
#endif
			sent = send(conn->sock, ptr, len, 0);

#ifndef WIN32
		pqsignal(SIGPIPE, oldsighandler);
#endif

		if (sent < 0)
		{
			/*
			 * Anything except EAGAIN or EWOULDBLOCK is trouble. If it's
			 * EPIPE or ECONNRESET, assume we've lost the backend
			 * connection permanently.
			 */
			switch (SOCK_ERRNO)
			{
#ifdef EAGAIN
				case EAGAIN:
					break;
#endif
#if defined(EWOULDBLOCK) && (!defined(EAGAIN) || (EWOULDBLOCK != EAGAIN))
				case EWOULDBLOCK:
					break;
#endif
				case EINTR:
					continue;

				case EPIPE:
#ifdef ECONNRESET
				case ECONNRESET:
#endif
					printfPQExpBuffer(&conn->errorMessage,
									  libpq_gettext(
							"server closed the connection unexpectedly\n"
													"\tThis probably means the server terminated abnormally\n"
						 "\tbefore or while processing the request.\n"));

					/*
					 * We used to close the socket here, but that's a bad
					 * idea since there might be unread data waiting
					 * (typically, a NOTICE message from the backend
					 * telling us it's committing hara-kiri...).  Leave
					 * the socket open until pqReadData finds no more data
					 * can be read.
					 */
					return EOF;

				default:
					printfPQExpBuffer(&conn->errorMessage,
					libpq_gettext("could not send data to server: %s\n"),
									  SOCK_STRERROR(SOCK_ERRNO));
					/* We don't assume it's a fatal error... */
					return EOF;
			}
		}
		else
		{
			ptr += sent;
			len -= sent;
		}

		if (len > 0)
		{
			/* We didn't send it all, wait till we can send more */

			/*
			 * if the socket is in non-blocking mode we may need to abort
			 * here
			 */
#ifdef USE_SSL
			/* can't do anything for our SSL users yet */
			if (conn->ssl == NULL)
			{
#endif
				if (pqIsnonblocking(conn))
				{
					/* shift the contents of the buffer */
					memmove(conn->outBuffer, ptr, len);
					conn->outCount = len;
					return EOF;
				}
#ifdef USE_SSL
			}
#endif

			if (pqWait(FALSE, TRUE, conn))
				return EOF;
		}
	}

	conn->outCount = 0;

	if (conn->Pfdebug)
		fflush(conn->Pfdebug);

	return 0;
}

/*
 * pqWait: wait until we can read or write the connection socket
 *
 * We also stop waiting and return if the kernel flags an exception condition
 * on the socket.  The actual error condition will be detected and reported
 * when the caller tries to read or write the socket.
 */
int
pqWait(int forRead, int forWrite, PGconn *conn)
{
	fd_set		input_mask;
	fd_set		output_mask;
	fd_set		except_mask;

	if (conn->sock < 0)
	{
		printfPQExpBuffer(&conn->errorMessage,
						  libpq_gettext("connection not open\n"));
		return EOF;
	}

	if (forRead || forWrite)
	{
retry:
		FD_ZERO(&input_mask);
		FD_ZERO(&output_mask);
		FD_ZERO(&except_mask);
		if (forRead)
			FD_SET(conn->sock, &input_mask);
		if (forWrite)
			FD_SET(conn->sock, &output_mask);
		FD_SET(conn->sock, &except_mask);
		if (select(conn->sock + 1, &input_mask, &output_mask, &except_mask,
				   (struct timeval *) NULL) < 0)
		{
			if (SOCK_ERRNO == EINTR)
				goto retry;
			printfPQExpBuffer(&conn->errorMessage,
							  libpq_gettext("select() failed: %s\n"),
							  SOCK_STRERROR(SOCK_ERRNO));
			return EOF;
		}
	}

	return 0;
}



/*
 * A couple of "miscellaneous" multibyte related functions. They used
 * to be in fe-print.c but that file is doomed.
 */

#ifdef MULTIBYTE
/*
 * returns the byte length of the word beginning s, using the
 * specified encoding.
 */
int
PQmblen(const unsigned char *s, int encoding)
{
	return (pg_encoding_mblen(encoding, s));
}

/*
 * Get encoding id from environment variable PGCLIENTENCODING.
 */
int
PQenv2encoding(void)
{
	char	   *str;
	int			encoding = PG_SQL_ASCII;

	str = getenv("PGCLIENTENCODING");
	if (str && *str != '\0')
		encoding = pg_char_to_encoding(str);
	return (encoding);
}

#else

/* Provide a default definition in case someone calls it anyway */
int
PQmblen(const unsigned char *s, int encoding)
{
	(void) s;
	(void) encoding;
	return 1;
}
int
PQenv2encoding(void)
{
	return 0;
}
#endif   /* MULTIBYTE */


#ifdef ENABLE_NLS
char *
libpq_gettext(const char *msgid)
{
	static int	already_bound = 0;

	if (!already_bound)
	{
		already_bound = 1;
		bindtextdomain("libpq", LOCALEDIR);
	}

	return dgettext("libpq", msgid);
}
#endif   /* ENABLE_NLS */

#ifdef WIN32
/*
 * strerror replacement for windows:
 *
 * This works on WIN2000 and newer, but we don't know where to find WinSock
 * error strings on older Windows flavors.  If you know, clue us in.
 */
const char *
winsock_strerror(int eno)
{
	static char	err_buf[512];
#define WSSE_MAXLEN (sizeof(err_buf)-1-13)	/* 13 for " (0x00000000)" */
	int			length;

	/* First try the "system table", this works on Win2k and up */

	if (FormatMessage(
				FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
					  0,
					  eno,
					  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					  err_buf,
					  WSSE_MAXLEN,
					  NULL))
		goto WSSE_GOODEXIT;

	/* Insert other possible lookup methods here ... */

	/* Everything failed, just tell the user that we don't know the desc */

	strcpy(err_buf, "Socket error, no description available.");

WSSE_GOODEXIT:

	length = strlen(err_buf);
	sprintf(err_buf + (length < WSSE_MAXLEN ? length : WSSE_MAXLEN),
			" (0x%08X)", eno);

	return err_buf;
}

#endif
