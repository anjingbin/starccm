/*-------------------------------------------------------------------------
 *
 * pqcomm.c
 *	  Communication functions between the Frontend and the Backend
 *
 * These routines handle the low-level details of communication between
 * frontend and backend.  They just shove data across the communication
 * channel, and are ignorant of the semantics of the data --- or would be,
 * except for major brain damage in the design of the COPY OUT protocol.
 * Unfortunately, COPY OUT is designed to commandeer the communication
 * channel (it just transfers data without wrapping it into messages).
 * No other messages can be sent while COPY OUT is in progress; and if the
 * copy is aborted by an elog(ERROR), we need to close out the copy so that
 * the frontend gets back into sync.  Therefore, these routines have to be
 * aware of COPY OUT state.
 *
 * NOTE: generally, it's a bad idea to emit outgoing messages directly with
 * pq_putbytes(), especially if the message would require multiple calls
 * to send.  Instead, use the routines in pqformat.c to construct the message
 * in a buffer and then emit it in one call to pq_putmessage.  This helps
 * ensure that the channel will not be clogged by an incomplete message
 * if execution is aborted by elog(ERROR) partway through the message.
 * The only non-libpq code that should call pq_putbytes directly is COPY OUT.
 *
 * At one time, libpq was shared between frontend and backend, but now
 * the backend's "backend/libpq" is quite separate from "interfaces/libpq".
 * All that remains is similarities of names to trap the unwary...
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *	$Id: pqcomm.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

/*------------------------
 * INTERFACE ROUTINES
 *
 * setup/teardown:
 *		StreamServerPort	- Open postmaster's server port
 *		StreamConnection	- Create new connection with client
 *		StreamClose			- Close a client/backend connection
 *		pq_init			- initialize libpq at backend startup
 *		pq_close		- shutdown libpq at backend exit
 *
 * low-level I/O:
 *		pq_getbytes		- get a known number of bytes from connection
 *		pq_getstring	- get a null terminated string from connection
 *		pq_getbyte		- get next byte from connection
 *		pq_peekbyte		- peek at next byte from connection
 *		pq_putbytes		- send bytes to connection (not flushed until pq_flush)
 *		pq_flush		- flush pending output
 *
 * message-level I/O (and COPY OUT cruft):
 *		pq_putmessage	- send a normal message (suppressed in COPY OUT mode)
 *		pq_startcopyout - inform libpq that a COPY OUT transfer is beginning
 *		pq_endcopyout	- end a COPY OUT transfer
 *
 *------------------------
 */
#include "postgres.h"

#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#ifdef HAVE_NETINET_TCP_H
#include <netinet/tcp.h>
#endif
#include <arpa/inet.h>
#include <sys/file.h>

#include "libpq/libpq.h"
#include "miscadmin.h"


static void pq_close(void);


/*
 * Configuration options
 */
int			Unix_socket_permissions;
char	   *Unix_socket_group;


/*
 * Buffers for low-level I/O
 */

#define PQ_BUFFER_SIZE 8192

static unsigned char PqSendBuffer[PQ_BUFFER_SIZE];
static int	PqSendPointer;		/* Next index to store a byte in
								 * PqSendBuffer */

static unsigned char PqRecvBuffer[PQ_BUFFER_SIZE];
static int	PqRecvPointer;		/* Next index to read a byte from
								 * PqRecvBuffer */
static int	PqRecvLength;		/* End of data available in PqRecvBuffer */

/*
 * Message status
 */
static bool DoingCopyOut;


/* --------------------------------
 *		pq_init - initialize libpq at backend startup
 * --------------------------------
 */
void
pq_init(void)
{
	PqSendPointer = PqRecvPointer = PqRecvLength = 0;
	DoingCopyOut = false;
	on_proc_exit(pq_close, 0);
}


/* --------------------------------
 *		pq_close - shutdown libpq at backend exit
 *
 * Note: in a standalone backend MyProcPort will be null,
 * don't crash during exit...
 * --------------------------------
 */
static void
pq_close(void)
{
	if (MyProcPort != NULL)
	{
		close(MyProcPort->sock);
		/* make sure any subsequent attempts to do I/O fail cleanly */
		MyProcPort->sock = -1;
	}
}



/*
 * Streams -- wrapper around Unix socket system calls
 *
 *
 *		Stream functions are used for vanilla TCP connection protocol.
 */

static char sock_path[MAXPGPATH];


/* StreamDoUnlink()
 * Shutdown routine for backend connection
 * If a Unix socket is used for communication, explicitly close it.
 */
static void
StreamDoUnlink(void)
{
	Assert(sock_path[0]);
	unlink(sock_path);
}

/*
 * StreamServerPort -- open a sock stream "listening" port.
 *
 * This initializes the Postmaster's connection-accepting port *fdP.
 *
 * RETURNS: STATUS_OK or STATUS_ERROR
 */

int
StreamServerPort(int family, char *hostName, unsigned short portNumber,
				 char *unixSocketName, int *fdP)
{
	SockAddr	saddr;
	int			fd,
				err;
	int			maxconn;
	size_t		len = 0;
	int			one = 1;

	Assert(family == AF_INET || family == AF_UNIX);

	if ((fd = socket(family, SOCK_STREAM, 0)) < 0)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "FATAL: StreamServerPort: socket() failed: %s\n",
				 strerror(errno));
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		return STATUS_ERROR;
	}

	if (family == AF_INET)
	{
		if ((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &one,
						sizeof(one))) == -1)
		{
			snprintf(PQerrormsg, PQERRORMSG_LENGTH,
					 "FATAL: StreamServerPort: setsockopt(SO_REUSEADDR) failed: %s\n",
					 strerror(errno));
			fputs(PQerrormsg, stderr);
			pqdebug("%s", PQerrormsg);
			return STATUS_ERROR;
		}
	}

	MemSet((char *) &saddr, 0, sizeof(saddr));
	saddr.sa.sa_family = family;

#ifdef HAVE_UNIX_SOCKETS
	if (family == AF_UNIX)
	{
		UNIXSOCK_PATH(saddr.un, portNumber, unixSocketName);
		len = UNIXSOCK_LEN(saddr.un);
		strcpy(sock_path, saddr.un.sun_path);

		/*
		 * Grab an interlock file associated with the socket file.
		 */
		if (!CreateSocketLockFile(sock_path, true))
			return STATUS_ERROR;

		/*
		 * Once we have the interlock, we can safely delete any
		 * pre-existing socket file to avoid failure at bind() time.
		 */
		unlink(sock_path);
	}
#endif   /* HAVE_UNIX_SOCKETS */

	if (family == AF_INET)
	{
		/* TCP/IP socket */
		if (hostName[0] == '\0')
			saddr.in.sin_addr.s_addr = htonl(INADDR_ANY);
		else
		{
			struct hostent *hp;

			hp = gethostbyname(hostName);
			if ((hp == NULL) || (hp->h_addrtype != AF_INET))
			{
				snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				   "FATAL: StreamServerPort: gethostbyname(%s) failed\n",
						 hostName);
				fputs(PQerrormsg, stderr);
				pqdebug("%s", PQerrormsg);
				return STATUS_ERROR;
			}
			memmove((char *) &(saddr.in.sin_addr), (char *) hp->h_addr,
					hp->h_length);
		}

		saddr.in.sin_port = htons(portNumber);
		len = sizeof(struct sockaddr_in);
	}

	err = bind(fd, (struct sockaddr *) & saddr.sa, len);
	if (err < 0)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "FATAL: StreamServerPort: bind() failed: %s\n"
				 "\tIs another postmaster already running on port %d?\n",
				 strerror(errno), (int) portNumber);
		if (family == AF_UNIX)
			snprintf(PQerrormsg + strlen(PQerrormsg),
					 PQERRORMSG_LENGTH - strlen(PQerrormsg),
					 "\tIf not, remove socket node (%s) and retry.\n",
					 sock_path);
		else
			snprintf(PQerrormsg + strlen(PQerrormsg),
					 PQERRORMSG_LENGTH - strlen(PQerrormsg),
					 "\tIf not, wait a few seconds and retry.\n");
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		return STATUS_ERROR;
	}

#ifdef HAVE_UNIX_SOCKETS
	if (family == AF_UNIX)
	{
		/* Arrange to unlink the socket file at exit */
		on_proc_exit(StreamDoUnlink, 0);

		/*
		 * Fix socket ownership/permission if requested.  Note we must do
		 * this before we listen() to avoid a window where unwanted
		 * connections could get accepted.
		 */
		Assert(Unix_socket_group);
		if (Unix_socket_group[0] != '\0')
		{
			char	   *endptr;
			unsigned long int val;
			gid_t		gid;

			val = strtoul(Unix_socket_group, &endptr, 10);
			if (*endptr == '\0')
			{
				/* numeric group id */
				gid = val;
			}
			else
			{
				/* convert group name to id */
				struct group *gr;

				gr = getgrnam(Unix_socket_group);
				if (!gr)
				{
					snprintf(PQerrormsg, PQERRORMSG_LENGTH,
							 "FATAL:  no such group '%s'\n",
							 Unix_socket_group);
					fputs(PQerrormsg, stderr);
					pqdebug("%s", PQerrormsg);
					return STATUS_ERROR;
				}
				gid = gr->gr_gid;
			}
			if (chown(sock_path, -1, gid) == -1)
			{
				snprintf(PQerrormsg, PQERRORMSG_LENGTH,
						 "FATAL:  could not set group of %s: %s\n",
						 sock_path, strerror(errno));
				fputs(PQerrormsg, stderr);
				pqdebug("%s", PQerrormsg);
				return STATUS_ERROR;
			}
		}

		if (chmod(sock_path, Unix_socket_permissions) == -1)
		{
			snprintf(PQerrormsg, PQERRORMSG_LENGTH,
					 "FATAL:  could not set permissions on %s: %s\n",
					 sock_path, strerror(errno));
			fputs(PQerrormsg, stderr);
			pqdebug("%s", PQerrormsg);
			return STATUS_ERROR;
		}
	}
#endif   /* HAVE_UNIX_SOCKETS */

	/*
	 * Select appropriate accept-queue length limit.  PG_SOMAXCONN is only
	 * intended to provide a clamp on the request on platforms where an
	 * overly large request provokes a kernel error (are there any?).
	 */
	maxconn = MaxBackends * 2;
	if (maxconn > PG_SOMAXCONN)
		maxconn = PG_SOMAXCONN;

	err = listen(fd, maxconn);
	if (err < 0)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "FATAL: StreamServerPort: listen() failed: %s\n",
				 strerror(errno));
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		return STATUS_ERROR;
	}

	*fdP = fd;

	return STATUS_OK;
}

/*
 * StreamConnection -- create a new connection with client using
 *		server port.
 *
 * ASSUME: that this doesn't need to be non-blocking because
 *		the Postmaster uses select() to tell when the server master
 *		socket is ready for accept().
 *
 * NB: this can NOT call elog() because it is invoked in the postmaster,
 * not in standard backend context.  If we get an error, the best we can do
 * is log it to stderr.
 *
 * RETURNS: STATUS_OK or STATUS_ERROR
 */
int
StreamConnection(int server_fd, Port *port)
{
	ACCEPT_TYPE_ARG3 addrlen;

	/* accept connection (and fill in the client (remote) address) */
	addrlen = sizeof(port->raddr);
	if ((port->sock = accept(server_fd,
							 (struct sockaddr *) & port->raddr,
							 &addrlen)) < 0)
	{
		perror("postmaster: StreamConnection: accept");
		return STATUS_ERROR;
	}

#ifdef SCO_ACCEPT_BUG

	/*
	 * UnixWare 7+ and OpenServer 5.0.4 are known to have this bug, but it
	 * shouldn't hurt to catch it for all versions of those platforms.
	 */
	if (port->raddr.sa.sa_family == 0)
		port->raddr.sa.sa_family = AF_UNIX;
#endif

	/* fill in the server (local) address */
	addrlen = sizeof(port->laddr);
	if (getsockname(port->sock, (struct sockaddr *) & port->laddr,
					&addrlen) < 0)
	{
		perror("postmaster: StreamConnection: getsockname");
		return STATUS_ERROR;
	}

	/* select NODELAY and KEEPALIVE options if it's a TCP connection */
	if (port->laddr.sa.sa_family == AF_INET)
	{
		int			on = 1;

		if (setsockopt(port->sock, IPPROTO_TCP, TCP_NODELAY,
					   (char *) &on, sizeof(on)) < 0)
		{
			perror("postmaster: StreamConnection: setsockopt(TCP_NODELAY)");
			return STATUS_ERROR;
		}
		if (setsockopt(port->sock, SOL_SOCKET, SO_KEEPALIVE,
					   (char *) &on, sizeof(on)) < 0)
		{
			perror("postmaster: StreamConnection: setsockopt(SO_KEEPALIVE)");
			return STATUS_ERROR;
		}
	}

	return STATUS_OK;
}

/*
 * StreamClose -- close a client/backend connection
 */
void
StreamClose(int sock)
{
	close(sock);
}


/* --------------------------------
 * Low-level I/O routines begin here.
 *
 * These routines communicate with a frontend client across a connection
 * already established by the preceding routines.
 * --------------------------------
 */


/* --------------------------------
 *		pq_recvbuf - load some bytes into the input buffer
 *
 *		returns 0 if OK, EOF if trouble
 * --------------------------------
 */
static int
pq_recvbuf(void)
{
	if (PqRecvPointer > 0)
	{
		if (PqRecvLength > PqRecvPointer)
		{
			/* still some unread data, left-justify it in the buffer */
			memmove(PqRecvBuffer, PqRecvBuffer + PqRecvPointer,
					PqRecvLength - PqRecvPointer);
			PqRecvLength -= PqRecvPointer;
			PqRecvPointer = 0;
		}
		else
			PqRecvLength = PqRecvPointer = 0;
	}

	/* Can fill buffer from PqRecvLength and upwards */
	for (;;)
	{
		int			r;

#ifdef USE_SSL
		if (MyProcPort->ssl)
			r = SSL_read(MyProcPort->ssl, PqRecvBuffer + PqRecvLength,
						 PQ_BUFFER_SIZE - PqRecvLength);
		else
#endif
			r = recv(MyProcPort->sock, PqRecvBuffer + PqRecvLength,
					 PQ_BUFFER_SIZE - PqRecvLength, 0);

		if (r < 0)
		{
			if (errno == EINTR)
				continue;		/* Ok if interrupted */

			/*
			 * Careful: an elog() that tries to write to the client
			 * would cause recursion to here, leading to stack overflow
			 * and core dump!  This message must go *only* to the postmaster
			 * log.  elog(DEBUG) is presently safe.
			 */
			elog(DEBUG, "pq_recvbuf: recv() failed: %m");
			return EOF;
		}
		if (r == 0)
		{
			/* as above, only write to postmaster log */
			elog(DEBUG, "pq_recvbuf: unexpected EOF on client connection");
			return EOF;
		}
		/* r contains number of bytes read, so just incr length */
		PqRecvLength += r;
		return 0;
	}
}

/* --------------------------------
 *		pq_getbyte	- get a single byte from connection, or return EOF
 * --------------------------------
 */
int
pq_getbyte(void)
{
	while (PqRecvPointer >= PqRecvLength)
	{
		if (pq_recvbuf())		/* If nothing in buffer, then recv some */
			return EOF;			/* Failed to recv data */
	}
	return PqRecvBuffer[PqRecvPointer++];
}

/* --------------------------------
 *		pq_peekbyte		- peek at next byte from connection
 *
 *	 Same as pq_getbyte() except we don't advance the pointer.
 * --------------------------------
 */
int
pq_peekbyte(void)
{
	while (PqRecvPointer >= PqRecvLength)
	{
		if (pq_recvbuf())		/* If nothing in buffer, then recv some */
			return EOF;			/* Failed to recv data */
	}
	return PqRecvBuffer[PqRecvPointer];
}

/* --------------------------------
 *		pq_getbytes		- get a known number of bytes from connection
 *
 *		returns 0 if OK, EOF if trouble
 * --------------------------------
 */
int
pq_getbytes(char *s, size_t len)
{
	size_t		amount;

	while (len > 0)
	{
		while (PqRecvPointer >= PqRecvLength)
		{
			if (pq_recvbuf())	/* If nothing in buffer, then recv some */
				return EOF;		/* Failed to recv data */
		}
		amount = PqRecvLength - PqRecvPointer;
		if (amount > len)
			amount = len;
		memcpy(s, PqRecvBuffer + PqRecvPointer, amount);
		PqRecvPointer += amount;
		s += amount;
		len -= amount;
	}
	return 0;
}

/* --------------------------------
 *		pq_getstring	- get a null terminated string from connection
 *
 *		The return value is placed in an expansible StringInfo.
 *		Note that space allocation comes from the current memory context!
 *
 *		NOTE: this routine does not do any MULTIBYTE conversion,
 *		even though it is presumably useful only for text, because
 *		no code in this module should depend on MULTIBYTE mode.
 *		See pq_getstr in pqformat.c for that.
 *
 *		returns 0 if OK, EOF if trouble
 * --------------------------------
 */
int
pq_getstring(StringInfo s)
{
	int			c;

	/* Reset string to empty */
	s->len = 0;
	s->data[0] = '\0';

	/* Read until we get the terminating '\0' */
	while ((c = pq_getbyte()) != EOF && c != '\0')
		appendStringInfoCharMacro(s, c);

	if (c == EOF)
		return EOF;

	return 0;
}


/* --------------------------------
 *		pq_putbytes		- send bytes to connection (not flushed until pq_flush)
 *
 *		returns 0 if OK, EOF if trouble
 * --------------------------------
 */
int
pq_putbytes(const char *s, size_t len)
{
	size_t		amount;

	while (len > 0)
	{
		if (PqSendPointer >= PQ_BUFFER_SIZE)
			if (pq_flush())		/* If buffer is full, then flush it out */
				return EOF;
		amount = PQ_BUFFER_SIZE - PqSendPointer;
		if (amount > len)
			amount = len;
		memcpy(PqSendBuffer + PqSendPointer, s, amount);
		PqSendPointer += amount;
		s += amount;
		len -= amount;
	}
	return 0;
}

/* --------------------------------
 *		pq_flush		- flush pending output
 *
 *		returns 0 if OK, EOF if trouble
 * --------------------------------
 */
int
pq_flush(void)
{
	static int last_reported_send_errno = 0;

	unsigned char *bufptr = PqSendBuffer;
	unsigned char *bufend = PqSendBuffer + PqSendPointer;

	while (bufptr < bufend)
	{
		int			r;

#ifdef USE_SSL
		if (MyProcPort->ssl)
			r = SSL_write(MyProcPort->ssl, bufptr, bufend - bufptr);
		else
#endif
			r = send(MyProcPort->sock, bufptr, bufend - bufptr, 0);

		if (r <= 0)
		{
			if (errno == EINTR)
				continue;		/* Ok if we were interrupted */

			/*
			 * Careful: an elog() that tries to write to the client
			 * would cause recursion to here, leading to stack overflow
			 * and core dump!  This message must go *only* to the postmaster
			 * log.  elog(DEBUG) is presently safe.
			 *
			 * If a client disconnects while we're in the midst of output,
			 * we might write quite a bit of data before we get to a safe
			 * query abort point.  So, suppress duplicate log messages.
			 */
			if (errno != last_reported_send_errno)
			{
				last_reported_send_errno = errno;
				elog(DEBUG, "pq_flush: send() failed: %m");
			}

			/*
			 * We drop the buffered data anyway so that processing can
			 * continue, even though we'll probably quit soon.
			 */
			PqSendPointer = 0;
			return EOF;
		}

		last_reported_send_errno = 0; /* reset after any successful send */
		bufptr += r;
	}

	PqSendPointer = 0;
	return 0;
}


/*
 * Return EOF if the connection has been broken, else 0.
 */
int
pq_eof(void)
{
	char		x;
	int			res;

	res = recv(MyProcPort->sock, &x, 1, MSG_PEEK);

	if (res < 0)
	{
		/* can log to postmaster log only */
		elog(DEBUG, "pq_eof: recv() failed: %m");
		return EOF;
	}
	if (res == 0)
		return EOF;
	else
		return 0;
}


/* --------------------------------
 * Message-level I/O routines begin here.
 *
 * These routines understand about COPY OUT protocol.
 * --------------------------------
 */


/* --------------------------------
 *		pq_putmessage	- send a normal message (suppressed in COPY OUT mode)
 *
 *		If msgtype is not '\0', it is a message type code to place before
 *		the message body (len counts only the body size!).
 *		If msgtype is '\0', then the buffer already includes the type code.
 *
 *		All normal messages are suppressed while COPY OUT is in progress.
 *		(In practice only NOTICE messages might get emitted then; dropping
 *		them is annoying, but at least they will still appear in the
 *		postmaster log.)
 *
 *		returns 0 if OK, EOF if trouble
 * --------------------------------
 */
int
pq_putmessage(char msgtype, const char *s, size_t len)
{
	if (DoingCopyOut)
		return 0;
	if (msgtype)
		if (pq_putbytes(&msgtype, 1))
			return EOF;
	return pq_putbytes(s, len);
}

/* --------------------------------
 *		pq_startcopyout - inform libpq that a COPY OUT transfer is beginning
 * --------------------------------
 */
void
pq_startcopyout(void)
{
	DoingCopyOut = true;
}

/* --------------------------------
 *		pq_endcopyout	- end a COPY OUT transfer
 *
 *		If errorAbort is indicated, we are aborting a COPY OUT due to an error,
 *		and must send a terminator line.  Since a partial data line might have
 *		been emitted, send a couple of newlines first (the first one could
 *		get absorbed by a backslash...)
 * --------------------------------
 */
void
pq_endcopyout(bool errorAbort)
{
	if (!DoingCopyOut)
		return;
	if (errorAbort)
		pq_putbytes("\n\n\\.\n", 5);
	/* in non-error case, copy.c will have emitted the terminator line */
	DoingCopyOut = false;
}
