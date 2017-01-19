/*-------------------------------------------------------------------------
 *
 * auth.c
 *	  Routines to handle network authentication
 *
 * Portions Copyright (c) 1996-2001, PostgreSQL Global Development Group
 * Portions Copyright (c) 1994, Regents of the University of California
 *
 *
 * IDENTIFICATION
 *	  $Header: /home/hjcvs/OB-CCM-1.0/postgresql/src/backend/libpq/auth.c,v 1.2 2004/07/05 08:55:28 SuLiang Exp $
 *
 *-------------------------------------------------------------------------
 */

#include "postgres.h"

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#if defined(HAVE_STRUCT_CMSGCRED) || defined(HAVE_STRUCT_FCRED) || defined(HAVE_STRUCT_SOCKCRED)
#include <sys/uio.h>
#include <sys/ucred.h>
#include <errno.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>
#include "libpq/auth.h"
#include "libpq/crypt.h"
#include "libpq/hba.h"
#include "libpq/libpq.h"
#include "libpq/password.h"
#include "libpq/pqformat.h"
#include "miscadmin.h"

static void sendAuthRequest(Port *port, AuthRequest areq);
static int	checkPassword(Port *port, char *user, char *password);
static int	old_be_recvauth(Port *port);
static int	map_old_to_new(Port *port, UserAuth old, int status);
static void auth_failed(Port *port, int status);
static int	recv_and_check_password_packet(Port *port);
static int	recv_and_check_passwordv0(Port *port);

char	   *pg_krb_server_keyfile;

#ifdef USE_PAM
#include <security/pam_appl.h>

#define PGSQL_PAM_SERVICE "postgresql"	/* Service name passed to PAM */

static int	CheckPAMAuth(Port *port, char *user, char *password);
static int pam_passwd_conv_proc(int num_msg, const struct pam_message ** msg,
					 struct pam_response ** resp, void *appdata_ptr);

static struct pam_conv pam_passw_conv = {
	&pam_passwd_conv_proc,
	NULL
};

static char *pam_passwd = NULL; /* Workaround for Solaris 2.6 brokenness */
static Port *pam_port_cludge;	/* Workaround for passing "Port *port"
								 * into pam_passwd_conv_proc */
#endif   /* USE_PAM */

#ifdef KRB4
/*----------------------------------------------------------------
 * MIT Kerberos authentication system - protocol version 4
 *----------------------------------------------------------------
 */

#include "krb.h"

/*
 * pg_krb4_recvauth -- server routine to receive authentication information
 *					   from the client
 *
 * Nothing unusual here, except that we compare the username obtained from
 * the client's setup packet to the authenticated name.  (We have to retain
 * the name in the setup packet since we have to retain the ability to handle
 * unauthenticated connections.)
 */
static int
pg_krb4_recvauth(Port *port)
{
	long		krbopts = 0;	/* one-way authentication */
	KTEXT_ST	clttkt;
	char		instance[INST_SZ + 1],
				version[KRB_SENDAUTH_VLEN + 1];
	AUTH_DAT	auth_data;
	Key_schedule key_sched;
	int			status;

	strcpy(instance, "*");		/* don't care, but arg gets expanded
								 * anyway */
	status = krb_recvauth(krbopts,
						  port->sock,
						  &clttkt,
						  PG_KRB_SRVNAM,
						  instance,
						  &port->raddr.in,
						  &port->laddr.in,
						  &auth_data,
						  pg_krb_server_keyfile,
						  key_sched,
						  version);
	if (status != KSUCCESS)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_krb4_recvauth: kerberos error: %s\n",
				 krb_err_txt[status]);
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		return STATUS_ERROR;
	}
	if (strncmp(version, PG_KRB4_VERSION, KRB_SENDAUTH_VLEN))
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_krb4_recvauth: protocol version != \"%s\"\n",
				 PG_KRB4_VERSION);
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		return STATUS_ERROR;
	}
	if (strncmp(port->user, auth_data.pname, SM_USER))
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_krb4_recvauth: name \"%s\" != \"%s\"\n",
				 port->user, auth_data.pname);
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		return STATUS_ERROR;
	}
	return STATUS_OK;
}

#else
static int
pg_krb4_recvauth(Port *port)
{
	snprintf(PQerrormsg, PQERRORMSG_LENGTH,
		 "pg_krb4_recvauth: Kerberos not implemented on this server.\n");
	fputs(PQerrormsg, stderr);
	pqdebug("%s", PQerrormsg);

	return STATUS_ERROR;
}
#endif   /* KRB4 */


#ifdef KRB5
/*----------------------------------------------------------------
 * MIT Kerberos authentication system - protocol version 5
 *----------------------------------------------------------------
 */

#include <krb5.h>
#include <com_err.h>

/*
 * pg_an_to_ln -- return the local name corresponding to an authentication
 *				  name
 *
 * XXX Assumes that the first aname component is the user name.  This is NOT
 *	   necessarily so, since an aname can actually be something out of your
 *	   worst X.400 nightmare, like
 *		  ORGANIZATION=U. C. Berkeley/NAME=Paul M. Aoki@CS.BERKELEY.EDU
 *	   Note that the MIT an_to_ln code does the same thing if you don't
 *	   provide an aname mapping database...it may be a better idea to use
 *	   krb5_an_to_ln, except that it punts if multiple components are found,
 *	   and we can't afford to punt.
 */
static char *
pg_an_to_ln(char *aname)
{
	char	   *p;

	if ((p = strchr(aname, '/')) || (p = strchr(aname, '@')))
		*p = '\0';
	return aname;
}


/*
 * Various krb5 state which is not connection specfic, and a flag to
 * indicate whether we have initialised it yet.
 */
static int	pg_krb5_initialised;
static krb5_context pg_krb5_context;
static krb5_keytab pg_krb5_keytab;
static krb5_principal pg_krb5_server;


static int
pg_krb5_init(void)
{
	krb5_error_code retval;

	if (pg_krb5_initialised)
		return STATUS_OK;

	retval = krb5_init_context(&pg_krb5_context);
	if (retval)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_krb5_init: krb5_init_context returned"
				 " Kerberos error %d\n", retval);
		com_err("postgres", retval, "while initializing krb5");
		return STATUS_ERROR;
	}

	retval = krb5_kt_resolve(pg_krb5_context, pg_krb_server_keyfile, &pg_krb5_keytab);
	if (retval)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_krb5_init: krb5_kt_resolve returned"
				 " Kerberos error %d\n", retval);
		com_err("postgres", retval, "while resolving keytab file %s",
				pg_krb_server_keyfile);
		krb5_free_context(pg_krb5_context);
		return STATUS_ERROR;
	}

	retval = krb5_sname_to_principal(pg_krb5_context, NULL, PG_KRB_SRVNAM,
									 KRB5_NT_SRV_HST, &pg_krb5_server);
	if (retval)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_krb5_init: krb5_sname_to_principal returned"
				 " Kerberos error %d\n", retval);
		com_err("postgres", retval,
				"while getting server principal for service %s",
				pg_krb_server_keyfile);
		krb5_kt_close(pg_krb5_context, pg_krb5_keytab);
		krb5_free_context(pg_krb5_context);
		return STATUS_ERROR;
	}

	pg_krb5_initialised = 1;
	return STATUS_OK;
}


/*
 * pg_krb5_recvauth -- server routine to receive authentication information
 *					   from the client
 *
 * We still need to compare the username obtained from the client's setup
 * packet to the authenticated name, as described in pg_krb4_recvauth.	This
 * is a bit more problematic in v5, as described above in pg_an_to_ln.
 *
 * We have our own keytab file because postgres is unlikely to run as root,
 * and so cannot read the default keytab.
 */
static int
pg_krb5_recvauth(Port *port)
{
	krb5_error_code retval;
	int			ret;
	krb5_auth_context auth_context = NULL;
	krb5_ticket *ticket;
	char	   *kusername;

	ret = pg_krb5_init();
	if (ret != STATUS_OK)
		return ret;

	retval = krb5_recvauth(pg_krb5_context, &auth_context,
						   (krb5_pointer) & port->sock, PG_KRB_SRVNAM,
						   pg_krb5_server, 0, pg_krb5_keytab, &ticket);
	if (retval)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_krb5_recvauth: krb5_recvauth returned"
				 " Kerberos error %d\n", retval);
		com_err("postgres", retval, "from krb5_recvauth");
		return STATUS_ERROR;
	}

	/*
	 * The "client" structure comes out of the ticket and is therefore
	 * authenticated.  Use it to check the username obtained from the
	 * postmaster startup packet.
	 *
	 * I have no idea why this is considered necessary.
	 */
	retval = krb5_unparse_name(pg_krb5_context,
							   ticket->enc_part2->client, &kusername);
	if (retval)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_krb5_recvauth: krb5_unparse_name returned"
				 " Kerberos error %d\n", retval);
		com_err("postgres", retval, "while unparsing client name");
		krb5_free_ticket(pg_krb5_context, ticket);
		krb5_auth_con_free(pg_krb5_context, auth_context);
		return STATUS_ERROR;
	}

	kusername = pg_an_to_ln(kusername);
	if (strncmp(port->user, kusername, SM_USER))
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
			  "pg_krb5_recvauth: user name \"%s\" != krb5 name \"%s\"\n",
				 port->user, kusername);
		ret = STATUS_ERROR;
	}
	else
		ret = STATUS_OK;

	krb5_free_ticket(pg_krb5_context, ticket);
	krb5_auth_con_free(pg_krb5_context, auth_context);
	free(kusername);

	return ret;
}

#else
static int
pg_krb5_recvauth(Port *port)
{
	snprintf(PQerrormsg, PQERRORMSG_LENGTH,
		 "pg_krb5_recvauth: Kerberos not implemented on this server.\n");
	fputs(PQerrormsg, stderr);
	pqdebug("%s", PQerrormsg);

	return STATUS_ERROR;
}
#endif   /* KRB5 */


/*
 * Handle a v0 password packet.
 */
static int
recv_and_check_passwordv0(Port *port)
{
	int32		len;
	char	   *buf;
	PasswordPacketV0 *pp;
	char	   *user,
			   *password,
			   *cp,
			   *start;
	int			status;

	if (pq_getint(&len, 4) == EOF)
		return STATUS_EOF;
	len -= 4;
	buf = palloc(len);
	if (pq_getbytes(buf, len) == EOF)
	{
		pfree(buf);
		return STATUS_EOF;
	}

	pp = (PasswordPacketV0 *) buf;

	/*
	 * The packet is supposed to comprise the user name and the password
	 * as C strings.  Be careful to check that this is the case.
	 */
	user = password = NULL;

	len -= sizeof(pp->unused);

	cp = start = pp->data;

	while (len-- > 0)
		if (*cp++ == '\0')
		{
			if (user == NULL)
				user = start;
			else
			{
				password = start;
				break;
			}

			start = cp;
		}

	if (user == NULL || password == NULL)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "pg_password_recvauth: badly formed password packet.\n");
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		status = STATUS_ERROR;
	}
	else
	{
		UserAuth	saved;

		/* Check the password. */

		saved = port->auth_method;
		port->auth_method = uaPassword;

		status = checkPassword(port, user, password);

		port->auth_method = saved;

		/* Adjust the result if necessary. */
		if (map_old_to_new(port, uaPassword, status) != STATUS_OK)
			status = STATUS_ERROR;
	}

	pfree(buf);

	return status;
}


/*
 * Tell the user the authentication failed, but not (much about) why.
 *
 * There is a tradeoff here between security concerns and making life
 * unnecessarily difficult for legitimate users.  We would not, for example,
 * want to report the password we were expecting to receive...
 * But it seems useful to report the username and authorization method
 * in use, and these are items that must be presumed known to an attacker
 * anyway.
 * Note that many sorts of failure report additional information in the
 * postmaster log, which we hope is only readable by good guys.
 */
static void
auth_failed(Port *port, int status)
{
	const char *authmethod = "Unknown auth method:";

	/*
	 * If we failed due to EOF from client, just quit; there's no point in
	 * trying to send a message to the client, and not much point in
	 * logging the failure in the postmaster log.  (Logging the failure
	 * might be desirable, were it not for the fact that libpq closes the
	 * connection unceremoniously if challenged for a password when it
	 * hasn't got one to send.  We'll get a useless log entry for every
	 * psql connection under password auth, even if it's perfectly
	 * successful, if we log STATUS_EOF events.)
	 */
	if (status == STATUS_EOF)
		proc_exit(0);

	switch (port->auth_method)
	{
		case uaReject:
			authmethod = "Rejected host:";
			break;
		case uaKrb4:
			authmethod = "Kerberos4";
			break;
		case uaKrb5:
			authmethod = "Kerberos5";
			break;
		case uaTrust:
			authmethod = "Trusted";
			break;
		case uaIdent:
			authmethod = "IDENT";
			break;
		case uaMD5:
		case uaCrypt:
		case uaPassword:
			authmethod = "Password";
			break;
#ifdef USE_PAM
		case uaPAM:
			authmethod = "PAM";
			break;
#endif   /* USE_PAM */
	}

	elog(FATAL, "%s authentication failed for user \"%s\"",
		 authmethod, port->user);
	/* doesn't return */
}


/*
 * Client authentication starts here.  If there is an error, this
 * function does not return and the backend process is terminated.
 */
void
ClientAuthentication(Port *port)
{
	int			status = STATUS_ERROR;

	/*
	 * Get the authentication method to use for this frontend/database
	 * combination.  Note: a failure return indicates a problem with the
	 * hba config file, not with the request.  hba.c should have dropped
	 * an error message into the postmaster logfile if it failed.
	 */
	if (hba_getauthmethod(port) != STATUS_OK)
		elog(FATAL, "Missing or erroneous pg_hba.conf file, see postmaster log for details");

	/* Handle old style authentication. */
	if (PG_PROTOCOL_MAJOR(port->proto) == 0)
	{
		status = old_be_recvauth(port);
		if (status != STATUS_OK)
			auth_failed(port, status);
		return;
	}

	/* Handle new style authentication. */
	switch (port->auth_method)
	{
		case uaReject:

			/*
			 * This could have come from an explicit "reject" entry in
			 * pg_hba.conf, but more likely it means there was no matching
			 * entry.  Take pity on the poor user and issue a helpful
			 * error message.  NOTE: this is not a security breach,
			 * because all the info reported here is known at the frontend
			 * and must be assumed known to bad guys. We're merely helping
			 * out the less clueful good guys.
			 */
			{
				const char *hostinfo = "localhost";

				if (port->raddr.sa.sa_family == AF_INET)
					hostinfo = inet_ntoa(port->raddr.in.sin_addr);
				elog(FATAL,
				"No pg_hba.conf entry for host %s, user %s, database %s",
					 hostinfo, port->user, port->database);
				break;
			}

		case uaKrb4:
			sendAuthRequest(port, AUTH_REQ_KRB4);
			status = pg_krb4_recvauth(port);
			break;

		case uaKrb5:
			sendAuthRequest(port, AUTH_REQ_KRB5);
			status = pg_krb5_recvauth(port);
			break;

		case uaIdent:
#if !defined(SO_PEERCRED) && (defined(HAVE_STRUCT_CMSGCRED) || defined(HAVE_STRUCT_FCRED) || (defined(HAVE_STRUCT_SOCKCRED) && defined(LOCAL_CREDS)))

			/*
			 * If we are doing ident on unix-domain sockets, use SCM_CREDS
			 * only if it is defined and SO_PEERCRED isn't.
			 */
#if defined(HAVE_STRUCT_FCRED) || defined(HAVE_STRUCT_SOCKCRED)

			/*
			 * Receive credentials on next message receipt, BSD/OS,
			 * NetBSD. We need to set this before the client sends the
			 * next packet.
			 */
			{
				int			on = 1;

				if (setsockopt(port->sock, 0, LOCAL_CREDS, &on, sizeof(on)) < 0)
					elog(FATAL,
						 "pg_local_sendauth: can't do setsockopt: %s\n", strerror(errno));
			}
#endif
			if (port->raddr.sa.sa_family == AF_UNIX)
				sendAuthRequest(port, AUTH_REQ_SCM_CREDS);
#endif
			status = authident(port);
			break;

		case uaMD5:
			sendAuthRequest(port, AUTH_REQ_MD5);
			status = recv_and_check_password_packet(port);
			break;

		case uaCrypt:
			sendAuthRequest(port, AUTH_REQ_CRYPT);
			status = recv_and_check_password_packet(port);
			break;

		case uaPassword:
			sendAuthRequest(port, AUTH_REQ_PASSWORD);
			status = recv_and_check_password_packet(port);
			break;

#ifdef USE_PAM
		case uaPAM:
			pam_port_cludge = port;
			status = CheckPAMAuth(port, port->user, "");
			break;
#endif   /* USE_PAM */

		case uaTrust:
			status = STATUS_OK;
			break;
	}

	if (status == STATUS_OK)
		sendAuthRequest(port, AUTH_REQ_OK);
	else
		auth_failed(port, status);
}


/*
 * Send an authentication request packet to the frontend.
 */
static void
sendAuthRequest(Port *port, AuthRequest areq)
{
	StringInfoData buf;

	pq_beginmessage(&buf);
	pq_sendbyte(&buf, 'R');
	pq_sendint(&buf, (int32) areq, sizeof(int32));

	/* Add the salt for encrypted passwords. */
	if (areq == AUTH_REQ_MD5)
		pq_sendbytes(&buf, port->md5Salt, 4);
	else if (areq == AUTH_REQ_CRYPT)
		pq_sendbytes(&buf, port->cryptSalt, 2);

	pq_endmessage(&buf);
	pq_flush();
}

#ifdef USE_PAM

/*
 * PAM conversation function
 */

static int
pam_passwd_conv_proc(int num_msg, const struct pam_message ** msg, struct pam_response ** resp, void *appdata_ptr)
{
	StringInfoData buf;
	int32		len;

	if (num_msg != 1 || msg[0]->msg_style != PAM_PROMPT_ECHO_OFF)
	{
		switch (msg[0]->msg_style)
		{
			case PAM_ERROR_MSG:
				snprintf(PQerrormsg, PQERRORMSG_LENGTH,
						 "pam_passwd_conv_proc: Error from underlying PAM layer: '%s'\n", msg[0]->msg);
				fputs(PQerrormsg, stderr);
				pqdebug("%s", PQerrormsg);
				return PAM_CONV_ERR;
			default:
				snprintf(PQerrormsg, PQERRORMSG_LENGTH,
						 "pam_passwd_conv_proc: Unexpected PAM conversation %d/'%s'\n",
						 msg[0]->msg_style, msg[0]->msg);
				fputs(PQerrormsg, stderr);
				pqdebug("%s", PQerrormsg);
				return PAM_CONV_ERR;
		}
	}

	if (!appdata_ptr)
	{
		/*
		 * Workaround for Solaris 2.6 where the PAM library is broken and
		 * does not pass appdata_ptr to the conversation routine
		 */
		appdata_ptr = pam_passwd;
	}

	/*
	 * Password wasn't passed to PAM the first time around - let's go ask
	 * the client to send a password, which we then stuff into PAM.
	 */
	if (strlen(appdata_ptr) == 0)
	{
		sendAuthRequest(pam_port_cludge, AUTH_REQ_PASSWORD);
		if (pq_eof() == EOF || pq_getint(&len, 4) == EOF)
		{
			return PAM_CONV_ERR;	/* client didn't want to send password */
		}

		initStringInfo(&buf);
		pq_getstr(&buf);
		if (DebugLvl > 5)
			fprintf(stderr, "received PAM packet with len=%d, pw=%s\n",
					len, buf.data);

		if (strlen(buf.data) == 0)
		{
			snprintf(PQerrormsg, PQERRORMSG_LENGTH, "pam_passwd_conv_proc: no password\n");
			fputs(PQerrormsg, stderr);
			return PAM_CONV_ERR;
		}
		appdata_ptr = buf.data;
	}

	/*
	 * Explicitly not using palloc here - PAM will free this memory in
	 * pam_end()
	 */
	*resp = calloc(num_msg, sizeof(struct pam_response));
	if (!*resp)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH, "pam_passwd_conv_proc: Out of memory!\n");
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		if (buf.data)
			pfree(buf.data);
		return PAM_CONV_ERR;
	}

	(*resp)[0].resp = strdup((char *) appdata_ptr);
	(*resp)[0].resp_retcode = 0;

	return ((*resp)[0].resp ? PAM_SUCCESS : PAM_CONV_ERR);
}


/*
 * Check authentication against PAM.
 */
static int
CheckPAMAuth(Port *port, char *user, char *password)
{
	int			retval;
	pam_handle_t *pamh = NULL;

	/*
	 * Apparently, Solaris 2.6 is broken, and needs ugly static variable
	 * workaround
	 */
	pam_passwd = password;

	/*
	 * Set the application data portion of the conversation struct This is
	 * later used inside the PAM conversation to pass the password to the
	 * authentication module.
	 */
	pam_passw_conv.appdata_ptr = (char *) password;		/* from password above,
														 * not allocated */

	/* Optionally, one can set the service name in pg_hba.conf */
	if (port->auth_arg[0] == '\0')
		retval = pam_start(PGSQL_PAM_SERVICE, "pgsql@", &pam_passw_conv, &pamh);
	else
		retval = pam_start(port->auth_arg, "pgsql@", &pam_passw_conv, &pamh);

	if (retval != PAM_SUCCESS)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
			  "CheckPAMAuth: Failed to create PAM authenticator: '%s'\n",
				 pam_strerror(pamh, retval));
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		pam_passwd = NULL;		/* Unset pam_passwd */
		return STATUS_ERROR;
	}

	retval = pam_set_item(pamh, PAM_USER, user);

	if (retval != PAM_SUCCESS)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "CheckPAMAuth: pam_set_item(PAM_USER) failed: '%s'\n",
				 pam_strerror(pamh, retval));
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		pam_passwd = NULL;		/* Unset pam_passwd */
		return STATUS_ERROR;
	}

	retval = pam_set_item(pamh, PAM_CONV, &pam_passw_conv);

	if (retval != PAM_SUCCESS)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "CheckPAMAuth: pam_set_item(PAM_CONV) failed: '%s'\n",
				 pam_strerror(pamh, retval));
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		pam_passwd = NULL;		/* Unset pam_passwd */
		return STATUS_ERROR;
	}

	retval = pam_authenticate(pamh, 0);

	if (retval != PAM_SUCCESS)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "CheckPAMAuth: pam_authenticate failed: '%s'\n",
				 pam_strerror(pamh, retval));
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		pam_passwd = NULL;		/* Unset pam_passwd */
		return STATUS_ERROR;
	}

	retval = pam_acct_mgmt(pamh, 0);

	if (retval != PAM_SUCCESS)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
				 "CheckPAMAuth: pam_acct_mgmt failed: '%s'\n",
				 pam_strerror(pamh, retval));
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
		pam_passwd = NULL;		/* Unset pam_passwd */
		return STATUS_ERROR;
	}

	retval = pam_end(pamh, retval);

	if (retval != PAM_SUCCESS)
	{
		snprintf(PQerrormsg, PQERRORMSG_LENGTH,
		 "CheckPAMAuth: Failed to release PAM authenticator: '%s'\n",
				 pam_strerror(pamh, retval));
		fputs(PQerrormsg, stderr);
		pqdebug("%s", PQerrormsg);
	}

	pam_passwd = NULL;		/* Unset pam_passwd */

	return (retval == PAM_SUCCESS ? STATUS_OK : STATUS_ERROR);
}
#endif   /* USE_PAM */


/*
 * Called when we have received the password packet.
 */
static int
recv_and_check_password_packet(Port *port)
{
	StringInfoData buf;
	int32		len;
	int			result;

	if (pq_eof() == EOF || pq_getint(&len, 4) == EOF)
		return STATUS_EOF;		/* client didn't want to send password */

	initStringInfo(&buf);
	if (pq_getstr(&buf) == EOF) /* receive password */
	{
		pfree(buf.data);
		return STATUS_EOF;
	}

	if (DebugLvl > 5)			/* this is probably a BAD idea... */
		fprintf(stderr, "received password packet with len=%d, pw=%s\n",
				len, buf.data);

	result = checkPassword(port, port->user, buf.data);
	pfree(buf.data);
	return result;
}


/*
 * Handle `password' and `crypt' records. If an auth argument was
 * specified, use the respective file. Else use pg_shadow passwords.
 */
static int
checkPassword(Port *port, char *user, char *password)
{
	if (port->auth_arg[0] != '\0')
		return verify_password(port, user, password);

	return md5_crypt_verify(port, user, password);
}


/*
 * Server demux routine for incoming authentication information for protocol
 * version 0.
 */
static int
old_be_recvauth(Port *port)
{
	int			status;
	MsgType		msgtype = (MsgType) port->proto;

	/* Handle the authentication that's offered. */
	switch (msgtype)
	{
		case STARTUP_KRB4_MSG:
			status = map_old_to_new(port, uaKrb4, pg_krb4_recvauth(port));
			break;

		case STARTUP_KRB5_MSG:
			status = map_old_to_new(port, uaKrb5, pg_krb5_recvauth(port));
			break;

		case STARTUP_MSG:
			status = map_old_to_new(port, uaTrust, STATUS_OK);
			break;

		case STARTUP_PASSWORD_MSG:
			status = recv_and_check_passwordv0(port);
			break;

		default:
			fprintf(stderr, "Invalid startup message type: %u\n", msgtype);

			return STATUS_ERROR;
	}

	return status;
}


/*
 * The old style authentication has been done.	Modify the result of this (eg.
 * allow the connection anyway, disallow it anyway, or use the result)
 * depending on what authentication we really want to use.
 */
static int
map_old_to_new(Port *port, UserAuth old, int status)
{
	switch (port->auth_method)
	{
		case uaMD5:
		case uaCrypt:
		case uaReject:
#ifdef USE_PAM
		case uaPAM:
#endif   /* USE_PAM */
			status = STATUS_ERROR;
			break;

		case uaKrb4:
			if (old != uaKrb4)
				status = STATUS_ERROR;
			break;

		case uaKrb5:
			if (old != uaKrb5)
				status = STATUS_ERROR;
			break;

		case uaTrust:
			status = STATUS_OK;
			break;

		case uaIdent:
			status = authident(port);
			break;

		case uaPassword:
			if (old != uaPassword)
				status = STATUS_ERROR;

			break;
	}

	return status;
}
