/* File:			dlg_specific.h
 *
 * Description:		See "dlg_specific.c"
 *
 * Comments:		See "notice.txt" for copyright and license information.
 *
 */

#ifndef __DLG_SPECIFIC_H__
#define __DLG_SPECIFIC_H__

#include "psqlodbc.h"
#include "connection.h"

#ifdef WIN32
#include  <windowsx.h>
#include "resource.h"
#endif

/*	Unknown data type sizes */
#define UNKNOWNS_AS_MAX					0
#define UNKNOWNS_AS_DONTKNOW			1
#define UNKNOWNS_AS_LONGEST				2

/* ODBC initialization files */
#ifndef WIN32
#define ODBC_INI						".odbc.ini"
#define ODBCINST_INI					"odbcinst.ini"
#else
#define ODBC_INI						"ODBC.INI"
#define ODBCINST_INI					"ODBCINST.INI"
#endif


#define INI_DSN							DBMS_NAME		/* Name of default
														 * Datasource in ini
														 * file (not used?) */
#define INI_KDESC						"Description"	/* Data source
														 * description */
#define INI_SERVER						"Servername"	/* Name of Server
														 * running the Postgres
														 * service */
#define INI_PORT						"Port"	/* Port on which the
												 * Postmaster is listening */
#define INI_DATABASE					"Database"		/* Database Name */
#define INI_USER						"Username"		/* Default User Name */
#define INI_PASSWORD					"Password"		/* Default Password */
#define INI_DEBUG						"Debug" /* Debug flag */
#define INI_FETCH						"Fetch" /* Fetch Max Count */
#define INI_SOCKET						"Socket"		/* Socket buffer size */
#define INI_READONLY					"ReadOnly"		/* Database is read only */
#define INI_COMMLOG						"CommLog"		/* Communication to
														 * backend logging */
#define INI_PROTOCOL					"Protocol"		/* What protocol (6.2) */
#define INI_OPTIMIZER					"Optimizer"		/* Use backend genetic
														 * optimizer */
#define INI_KSQO						"Ksqo"	/* Keyset query
												 * optimization */
#define INI_CONNSETTINGS				 "ConnSettings" /* Anything to send to
														 * backend on successful
														 * connection */
#define INI_UNIQUEINDEX					"UniqueIndex"	/* Recognize unique
														 * indexes */
#define INI_UNKNOWNSIZES				"UnknownSizes"	/* How to handle unknown
														 * result set sizes */

#define INI_CANCELASFREESTMT			"CancelAsFreeStmt"

#define INI_USEDECLAREFETCH				"UseDeclareFetch"		/* Use Declare/Fetch
																 * cursors */

/*	More ini stuff */
#define INI_TEXTASLONGVARCHAR			"TextAsLongVarchar"
#define INI_UNKNOWNSASLONGVARCHAR		"UnknownsAsLongVarchar"
#define INI_BOOLSASCHAR					"BoolsAsChar"
#define INI_MAXVARCHARSIZE				"MaxVarcharSize"
#define INI_MAXLONGVARCHARSIZE			"MaxLongVarcharSize"

#define INI_FAKEOIDINDEX				"FakeOidIndex"
#define INI_SHOWOIDCOLUMN				"ShowOidColumn"
#define INI_ROWVERSIONING				"RowVersioning"
#define INI_SHOWSYSTEMTABLES			"ShowSystemTables"
#define INI_LIE							"Lie"
#define INI_PARSE						"Parse"
#define INI_EXTRASYSTABLEPREFIXES		"ExtraSysTablePrefixes"

#define INI_TRANSLATIONNAME				"TranslationName"
#define INI_TRANSLATIONDLL				"TranslationDLL"
#define INI_TRANSLATIONOPTION			"TranslationOption"
#define INI_DISALLOWPREMATURE			"DisallowPremature"
#define INI_UPDATABLECURSORS			"UpdatableCursors"


/*	Connection Defaults */
#define DEFAULT_PORT					"5432"
#define DEFAULT_READONLY				0
#define DEFAULT_PROTOCOL				"6.4"	/* the latest protocol is
												 * the default */
#define DEFAULT_USEDECLAREFETCH			0
#define DEFAULT_TEXTASLONGVARCHAR		1
#define DEFAULT_UNKNOWNSASLONGVARCHAR	0
#define DEFAULT_BOOLSASCHAR				1
#define DEFAULT_OPTIMIZER				1		/* disable */
#define DEFAULT_KSQO					1		/* on */
#define DEFAULT_UNIQUEINDEX				1		/* dont recognize */
#define DEFAULT_COMMLOG					0		/* dont log */
#define DEFAULT_DEBUG					0
#define DEFAULT_UNKNOWNSIZES			UNKNOWNS_AS_MAX


#define DEFAULT_FAKEOIDINDEX			0
#define DEFAULT_SHOWOIDCOLUMN			0
#define DEFAULT_ROWVERSIONING			0
#define DEFAULT_SHOWSYSTEMTABLES		0		/* dont show system tables */
#define DEFAULT_LIE						0
#define DEFAULT_PARSE					0

#define DEFAULT_CANCELASFREESTMT		0

#define DEFAULT_EXTRASYSTABLEPREFIXES	"dd_;"

/*	prototypes */
void		getCommonDefaults(const char *section, const char *filename, ConnInfo *ci);

#ifdef WIN32
void		SetDlgStuff(HWND hdlg, const ConnInfo *ci);
void		GetDlgStuff(HWND hdlg, ConnInfo *ci);

int CALLBACK driver_optionsProc(HWND hdlg,
				   WORD wMsg,
				   WPARAM wParam,
				   LPARAM lParam);
int CALLBACK ds_optionsProc(HWND hdlg,
			   WORD wMsg,
			   WPARAM wParam,
			   LPARAM lParam);
#endif   /* WIN32 */

void		updateGlobals(void);
void		writeDSNinfo(const ConnInfo *ci);
void		getDSNdefaults(ConnInfo *ci);
void		getDSNinfo(ConnInfo *ci, char overwrite);
void		makeConnectString(char *connect_string, const ConnInfo *ci, UWORD);
void		copyAttributes(ConnInfo *ci, const char *attribute, const char *value);
void		copyCommonAttributes(ConnInfo *ci, const char *attribute, const char *value);

#endif
