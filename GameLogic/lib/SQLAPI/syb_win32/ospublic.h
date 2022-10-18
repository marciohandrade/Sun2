/*		Sccsid %Z% %M% %I% %G% */

/*
**		Sybase Server-Library
**		Confidential Property of Sybase, Inc.
**		(c) Copyright Sybase, Inc. 1991 - 1994
**		All rights reserved
**
** ospublic.h	This is the header file that contains all definitions that
**		are public to Open Server applications except for error
**		number definitions.  Error number definitions are in oserror.h.
**		oserror.h should also be included by an Open Server application
**		that needs to decode Open Server error numbers.
**
** 22Sep95	Added a sleep flag SRV_M_NOWAKE_ON_ERROR		sureshk
** 01Oct96	removed the prototype for srv_offset() BTS: 65040	abidi
** 04Feb98      Added a wakeup flag SRV_M_CTWAKEUP                      sunilk
*/
#if	!defined(__OSPUBLIC_H__)
#define __OSPUBLIC_H__

/*
** Include common defines and typedefs that are visible to Open Server
** clients.
*/
#include	<cspublic.h>

/*
** Public data structure definitions.
*/

/*
** Public Cursor Information Structure.
*/
typedef struct _srv_curdesc
{
	CS_INT		curid;
	CS_INT		numupcols;
	CS_INT		fetchcnt;
	CS_INT		curstatus;
	CS_INT		curcmd;
	CS_INT		cmdoptions;
	CS_INT		fetchtype;
	CS_INT		rowoffset;
	CS_INT		curnamelen;
	CS_CHAR		curname[CS_MAX_CHAR];
	CS_INT		tabnamelen;
	CS_CHAR		tabname[CS_MAX_CHAR];
	CS_VOID		*userdata;
} SRV_CURDESC;

/*
** Public structure used to obtain procedure name lists.
*/
typedef	struct	srv__proclist
{
	CS_INT		num_procs;	/* The # of procedure names	*/
	CS_CHAR		**proc_list;	/* The procedure name strings	*/
} SRV_PROCLIST;

/*
** Public structure used to obtain remote servername and password
** pairs.  This sturcture is used by srv_thread_props for the SRV_T_RMTPWDS
** property.
*/
typedef struct srv_rmtpwd
{
	CS_INT	servnamelen;
	CS_BYTE	servname[CS_MAX_NAME];
	CS_INT	pwdlen;
	CS_BYTE	pwd[CS_MAX_NAME];
} SRV_RMTPWD;

/* 
** General Purpose Mask Structure.
*/
#define	SRV_MASK_SIZE		(CS_INT)40
#define	SRV_MAXMASK_LENGTH	(CS_INT)(SRV_MASK_SIZE * CS_BITS_PER_BYTE \
					* (sizeof(unsigned int)))
typedef struct	srv_mask_array
{
	unsigned int mask_bits[SRV_MASK_SIZE];
} SRV_MASK_ARRAY;

/*
** Open Server object type (mutexes and message queues)
*/
#define SRV_OBJID	CS_INT 

/*
** Internal Open Server datatype definitions.
*/
#if	!defined(SRV__INTERNAL_STRUCTS)

/*
** Define generic data structure types as void.
*/
#if	!defined(SRV_PROC)
#define	SRV_PROC	CS_VOID
#endif	/* SRV_PROC */

#if	!defined(SRV_SERVER)
#define	SRV_SERVER	CS_VOID
#endif	/* SRV_SERVER */

#if	!defined(SRV_CONFIG)
#define	SRV_CONFIG	CS_VOID
#endif	/* SRV_CONFIG */

#endif	/* SRV__INTERNAL_STRUCTS */

/*
** Include the compatability header file.  This file defines everything 
** required for backward compatability with pre System 10 releases.  This
** must be included before the SRV_POLLFD definition below, since SRV_POLLFD
** might get mapped to an OS specific structure in oscompat.h.
*/
#include	<oscompat.h>

#if	!defined(SRV_POLLIN)
/*
** NOTE:  This SRV_POLLFD is only defined here for platforms that don't support
** the poll system call directly.  oscompat.h maps these defines to the
** OS specific definitions on platforms that support the poll system call.
** However, all applications should migrate to use these new defines in 
** their source code and remove all platform specific definitions of the
** poll structure.
*/

/*
** srv_poll file descriptor polling structure
**
**	srv_fd		File descriptor to poll
**	srv_events	Events to poll for on file descriptor
**	srv_revents	Events detected on file descriptor
*/
typedef struct srv__pollfd
{
	CS_INT	srv_fd;
	CS_INT	srv_events;
	CS_INT	srv_revents;
} SRV_POLLFD;

/*
** Event flags for srv_events and srv_revents
**
**	SRV_POLLIN	A non-blocking read may be completed
**	SRV_POLLPRI	A non-blocking read of urgent data may be completed
**	SRV_POLLOUT	A non-blocking write may be completed
**	SRV_POLLERR	An error occurred on srv_fd (srv_revents only)
**	SRV_POLLHUP	An hang-up occurred on srv_fd (srv_revents only)
**	SRV_POLLNVAL	srv_fd was invalid (srv_revents only)
*/
#define	SRV_POLLIN	(CS_INT)0x0001
#define	SRV_POLLPRI	(CS_INT)0x0002
#define	SRV_POLLOUT	(CS_INT)0x0004
#define	SRV_POLLERR	(CS_INT)0x0008
#define	SRV_POLLHUP	(CS_INT)0x0010
#define	SRV_POLLNVAL	(CS_INT)0x0020
#endif	/* SRV_POLLIN */


/*
** Function argument prototypes
*/

CS_START_EXTERN_C

typedef CS_RETCODE	(CS_PUBLIC * SRV_CALLBACK_FUNC)PROTOTYPE((
				SRV_PROC *spp
				));
typedef CS_RETCODE	(CS_PUBLIC * SRV_ERRHANDLE_FUNC)PROTOTYPE((
				SRV_SERVER *ssp,
				SRV_PROC *spp,
				CS_INT errornum,
				CS_INT severity,
				CS_INT state,
				CS_INT oserrnum,
				CS_CHAR *errtext,
				CS_INT errtextlen,
				CS_CHAR *oserrtext,
				CS_INT oserrtextlen
				));
typedef CS_RETCODE	(CS_PUBLIC * SRV_EVENTHANDLE_FUNC)PROTOTYPE((
				CS_VOID *argp
				));
typedef CS_RETCODE	(CS_PUBLIC * SRV_SERVICETHREAD_FUNC)PROTOTYPE((
				CS_VOID *argp
				));
#if (AIX)
typedef CS_VOID		(CS_VARARGS * SRV_SIGNAL_FUNC)PROTOTYPE((
				CS_INT
				));
#else
typedef CS_VOID		(CS_VARARGS * SRV_SIGNAL_FUNC)PROTOTYPE((
				CS_VOID * CS_ELLIPSE
				));
#endif /* AIX */

CS_END_EXTERN_C

/*
** The NULL pointer define
*/
#if	!defined(NULL)
#define	NULL		0
#endif	/* NULL */

/* 
** Cursor command option values.
*/
#define	SRV_CUR_UNUSED		((CS_INT)0x0001)
#define	SRV_CUR_DEALLOC		((CS_INT)0x0002)
#define	SRV_CUR_UPDATABLE	((CS_INT)0x0004)
#define	SRV_CUR_RDONLY		((CS_INT)0x0008)
#define	SRV_CUR_DYNAMIC		((CS_INT)0x0010)
#define	SRV_CUR_HASARGS		((CS_INT)0x0020)
#define	SRV_CUR_SETROWS		((CS_INT)0x0040)
#define	SRV_CUR_ASKSTATUS	((CS_INT)0x0080)
#define	SRV_CUR_INFORMSTATUS	((CS_INT)0x0100)

/* 
** Dynamic item values.
*/
#define	SRV_DYN_TYPE		(CS_INT)1
#define	SRV_DYN_IDLEN		(CS_INT)2
#define	SRV_DYN_ID		(CS_INT)3
#define	SRV_DYN_STMTLEN		(CS_INT)4
#define	SRV_DYN_STMT		(CS_INT)5

/*
** Open Server Event Types
*/
#define	SRV_ATTENTION		(CS_INT)0
#define	SRV_CONNECT		(CS_INT)1
#define	SRV_DISCONNECT		(CS_INT)2
#define	SRV_LANGUAGE		(CS_INT)3
#define	SRV_RPC			(CS_INT)4
#define	SRV_START		(CS_INT)5
#define	SRV_STOP		(CS_INT)6
#define	SRV_BULK		(CS_INT)7
#define	SRV_CURSOR		(CS_INT)8
#define	SRV_MSG			(CS_INT)9
#define	SRV_OPTION		(CS_INT)10
#define	SRV_DYNAMIC		(CS_INT)11
#define	SRV_URGDISCONNECT	(CS_INT)14
#define SRV_FULLPASSTHRU	(CS_INT)16

/*
** Symbols used by registered procedure routines.
*/
#define SRV_M_PNOTIFYALL	((CS_SMALLINT)0x0002)
#define SRV_M_PNOTIFYNEXT	((CS_SMALLINT)0x0004)
#define SRV_NODEFAULT		((CS_INT)(-2))

/*
** Define the return codes for callback routines.
*/
#define	SRV_S_CONTINUE		(CS_INT)0
#define	SRV_S_INHIBIT		(CS_INT)1

/*
**  Event types
*/
#define	SRV_EQUEUED		(CS_INT)0x01
#define	SRV_EIMMEDIATE		(CS_INT)0x02

/* 
** Object Types
*/
#define SRV_C_MUTEX		(CS_INT)1
#define SRV_C_MQUEUE		(CS_INT)2

/*
** Scheduling and messaging system symbols.
*/
#define SRV_M_URGENT		(CS_INT)0x00000001
#define SRV_M_READ_ONLY		(CS_INT)0x00000002
#define SRV_M_WAIT		(CS_INT)0x00000010
#define SRV_M_NOWAIT		(CS_INT)0x00000020
#define SRV_M_WAKE_ALL		(CS_INT)0x00000040
#define SRV_M_WAKE_FIRST	(CS_INT)0x00000080
#define SRV_M_WAKE_INTR	  	(CS_INT)0x00000100
#define SRV_M_WAKE_NOINTR	(CS_INT)0x00000200
#define SRV_M_ATTNWAKE  	(CS_INT)0x00000400
#define SRV_M_NOATTNWAKE  	(CS_INT)0x00000800
#define SRV_M_NOWAKE_ON_ERROR  	(CS_INT)0x00001000
#define SRV_M_CTWAKEUP          (CS_INT)0x00002000

/*
** Informational symbols.
*/
#define	SRV_I_UNKNOWN		(CS_INT)0
#define SRV_I_SYNC		(CS_INT)1
#define SRV_I_GRANTED		(CS_INT)2
#define SRV_I_NOEXIST		(CS_INT)3
#define SRV_I_DELETED		(CS_INT)4	
#define SRV_I_WOULDWAIT		(CS_INT)5	
#define SRV_I_PASSTHRU_MORE	(CS_INT)6
#define SRV_I_PASSTHRU_EOM	(CS_INT)7
#define SRV_I_INTERRUPTED	(CS_INT)8
#define	SRV_I_PEXISTS		(CS_INT)9
#define	SRV_I_PNOTKNOWN		(CS_INT)10
#define	SRV_I_PCLIENT		(CS_INT)11
#define	SRV_I_PNOTCLIENT	(CS_INT)12
#define	SRV_I_PNOPENDING	(CS_INT)13
#define	SRV_I_PPARAMERR		(CS_INT)14
#define	SRV_I_PNOTIFYERR	(CS_INT)15
#define	SRV_I_PINVOPT		(CS_INT)16
#define	SRV_I_PNOTIFYEXISTS	(CS_INT)17
#define	SRV_I_TIMEOUT		(CS_INT)18

/*
** The following constants are used for the callback installation.
*/
#define SRV_C_SUSPEND		(CS_INT)1
#define SRV_C_RESUME		(CS_INT)2
#define SRV_C_TIMESLICE		(CS_INT)3
#define SRV_C_EXIT		(CS_INT)4
#define	SRV_C_PREEMPT		(CS_INT)5
#define	SRV_C_DEBUG		(CS_INT)6
#define	SRV_C_SELECT		(CS_INT)7
#define	SRV_C_POLL		(CS_INT)8
#define	SRV_C_PROCEXEC		(CS_INT)9

/* 
** Defines for `type' argument to srv_symbol().
*/
#define	SRV_ERROR		(CS_INT)100
#define	SRV_DONE		(CS_INT)101
#define	SRV_DATATYPE		(CS_INT)102
#define	SRV_EVENT		(CS_INT)104

/* 
** These are the possible values for the `type' argument in 
** srv_bind(), srv_descfmt(), and srv_xferdata().
*/
#define	SRV_MSGDATA		(CS_INT)1
#define	SRV_ROWDATA		(CS_INT)2
#define	SRV_RPCDATA		(CS_INT)3
#define	SRV_ERRORDATA		(CS_INT)4
#define	SRV_DYNDATA		(CS_INT)5
#define SRV_CURDATA		(CS_INT)6
#define SRV_KEYDATA		(CS_INT)7
#define SRV_UPCOLDATA		(CS_INT)8
#define SRV_NEGDATA		(CS_INT)9
#define SRV_LANGDATA		(CS_INT)10

/* 
** type args for srv_negotiate
*/
#define	SRV_NEG_CHALLENGE	(CS_INT)1
#define	SRV_NEG_ENCRYPT		(CS_INT)2
#define	SRV_NEG_LOCPWD		(CS_INT)3
#define	SRV_NEG_SECLABEL	(CS_INT)4
#define	SRV_NEG_REMPWD		(CS_INT)5
#define SRV_NEG_SECSESSION	(CS_INT)6

/* 
** These are possible values for the srv_envchange `type' argument.
*/
#define	SRV_ENVDATABASE		(CS_INT)1
#define	SRV_ENVLANG		(CS_INT)2
#define	SRV_ENVCHARSET		(CS_INT)3

/* 
** These are possible values for the srv_msg() `status' argument and
** the cmdoptions field of the SRV_CURDESC structure
*/
#define	SRV_HASPARAMS		(CS_INT)0x01
#define	SRV_NOPARAMS		(CS_INT)0x00

/*
** This is the range of message Ids that are reserved for internal use
** by SYBASE
*/
#define	SRV_MINRESMSG		(CS_INT)0
#define	SRV_MAXRESMSG		(CS_INT)32767

/*
** The maximum char or binary length
*/
#define SRV_MAXCHAR             (CS_INT)255

/* 
** These are possible values for the srv_senddone() `status' argument.
*/
#define SRV_DONE_FINAL		(CS_INT)0x0001
#define SRV_DONE_MORE		(CS_INT)0x0002
#define SRV_DONE_ERROR		(CS_INT)0x0004
#define SRV_DONE_COUNT		(CS_INT)0x0008
#define	SRV_DONE_FLUSH		(CS_INT)0x0010

/*
** Defines for server properies.
*/
#define SRV_S_SERVERNAME        (CS_INT)300
#define SRV_S_NUMCONNECTIONS    (CS_INT)301
#define SRV_S_PREEMPT           (CS_INT)302
#define SRV_S_TIMESLICE         (CS_INT)303
#define SRV_S_STACKSIZE         (CS_INT)304
#define SRV_S_MSGPOOL           (CS_INT)305
#define SRV_S_NUMUSEREVENTS     (CS_INT)306
#define SRV_S_LOGFILE           (CS_INT)307
#define SRV_S_NUMTHREADS        (CS_INT)308
#define SRV_S_NUMREMSITES       (CS_INT)309
#define SRV_S_NUMREMBUF         (CS_INT)310
#define SRV_S_NUMMSGQUEUES      (CS_INT)311
#define SRV_S_NUMMUTEXES        (CS_INT)312
#define SRV_S_NETBUFSIZE        (CS_INT)313
#define SRV_S_VIRTCLKRATE       (CS_INT)314
#define SRV_S_TDSVERSION        (CS_INT)315
#define SRV_S_VIRTTIMER         (CS_INT)316
#define SRV_S_APICHK            (CS_INT)317
#define SRV_S_CURTHREAD         (CS_INT)318
#define SRV_S_VERSION           (CS_INT)319
#define SRV_S_IFILE             (CS_INT)320
#define SRV_S_ALLOCFUNC         (CS_INT)321
#define SRV_S_FREEFUNC          (CS_INT)322
#define SRV_S_REALLOCFUNC       (CS_INT)323
#define SRV_S_ERRHANDLE         (CS_INT)324
#define SRV_S_TRACEFLAG         (CS_INT)325
#define SRV_S_USESRVLANG        (CS_INT)326
#define SRV_S_TRUNCATELOG       (CS_INT)327
#define SRV_S_LOGSIZE           (CS_INT)328
#define SRV_S_RESPONSECAP       (CS_INT)329
#define SRV_S_REQUESTCAP        (CS_INT)330
#define SRV_S_DEFQUEUESIZE      (CS_INT)331
#define SRV_S_DISCONNECT        (CS_INT)332
#define SRV_S_ATTNREASON        (CS_INT)333
#define SRV_S_NETTRACEFILE      (CS_INT)334
#define SRV_S_NETDATAFILE       (CS_INT)335

#define	SRV_S_DSPROVIDER	(CS_INT)336
#define	SRV_S_DSREGISTER	(CS_INT)337
#define	SRV_S_DSFAILOVER	(CS_INT)338

#define SRV_S_SEC_PRINCIPAL     (CS_INT)339
#define SRV_S_SEC_KEYTAB	(CS_INT)340
#define SRV_S_IDLECOUNT		(CS_INT)341

#define	SRV_S_CERT_AUTH		(CS_INT)342
#define	SRV_S_VALIDATE_CB	(CS_INT)343
#define	SRV_S_SSL_VERSION	(CS_INT)344
#define	SRV_S_SSL_CIPHER	(CS_INT)345
#define	SRV_S_SSL_REQUIRE_CLIENT_CERT (CS_INT)346
#define	SRV_S_SSL_LOCAL_ID	(CS_INT)347
 
#define SRV_S_LASTPROP		(CS_INT)347

/* 
** Trace type definitions for SRV_S_TRACEFLAG server property
*/
#define SRV_TR_TDSHDR		(CS_INT)0x0001
#define SRV_TR_TDSDATA		(CS_INT)0x0002
#define SRV_TR_MSGQ		(CS_INT)0x0004
#define SRV_TR_ATTN		(CS_INT)0x0008
#define SRV_TR_TDSTOKEN		(CS_INT)0x0010
#define SRV_TR_EVENT		(CS_INT)0x0020
#define SRV_TR_NETPARAM		(CS_INT)0x0040
#define SRV_TR_NETREQ		(CS_INT)0x0080
#define SRV_TR_NETDRIVER	(CS_INT)0x0100
#define SRV_TR_NETMEM		(CS_INT)0x0200
#define SRV_TR_NETDATA		(CS_INT)0x0400
#define SRV_TR_NETWAKE		(CS_INT)0x0800
#define SRV_TR_DEFQUEUE		(CS_INT)0x1000

/* 
** Defines for thread properties - srv_thread_props().
*/
#define SRV_T_SPID              (CS_INT)200
#define SRV_T_TYPE              (CS_INT)201
#define SRV_T_PRIORITY          (CS_INT)202
#define SRV_T_TDSVERSION        (CS_INT)203
#define SRV_T_EVENT             (CS_INT)204
#define SRV_T_MACHINE           (CS_INT)205
#define SRV_T_USER              (CS_INT)206
#define SRV_T_PWD               (CS_INT)207
#define SRV_T_APPLNAME          (CS_INT)208
#define SRV_T_RMTSERVER         (CS_INT)209
#define SRV_T_CLIB              (CS_INT)210
#define SRV_T_CLIBVERS          (CS_INT)211
#define SRV_T_ROWSENT           (CS_INT)212
#define SRV_T_USERDATA          (CS_INT)213
#define SRV_T_EVENTDATA         (CS_INT)214
#define SRV_T_GOTATTENTION      (CS_INT)215
#define SRV_T_IODEAD            (CS_INT)216
#define SRV_T_PASSTHRU          (CS_INT)217
#define SRV_T_USTATE            (CS_INT)218
#define SRV_T_CLIENTLOGOUT      (CS_INT)219
#define SRV_T_LOGINTYPE         (CS_INT)220
#define SRV_T_HOSTNAME          (CS_INT)221
#define SRV_T_HOSTPROCID        (CS_INT)222
#define SRV_T_BYTEORDER         (CS_INT)223
#define SRV_T_FLTTYPE           (CS_INT)224
#define SRV_T_NOTIFYDB          (CS_INT)225
#define SRV_T_NOTIFYCHARSET     (CS_INT)226
#define SRV_T_NOTIFYLANG        (CS_INT)227
#define SRV_T_DUMPLOAD          (CS_INT)228
#define SRV_T_CONVERTSHORT      (CS_INT)229
#define SRV_T_PACKETSIZE        (CS_INT)230
#define SRV_T_STACKLEFT         (CS_INT)231
#define SRV_T_BULKTYPE          (CS_INT)232
#define SRV_T_LOCALE            (CS_INT)233
#define SRV_T_NUMRMTPWDS        (CS_INT)234
#define SRV_T_RMTPWDS           (CS_INT)235
#define SRV_T_NEGLOGIN          (CS_INT)236
#define SRV_T_USESRVLANG        (CS_INT)237
#define SRV_T_CHARTYPE        	(CS_INT)238
#define SRV_T_ENDPOINT        	(CS_INT)239
#define SRV_T_DONESENT		(CS_INT)240
#define SRV_T_CONTEXT		(CS_INT)241
#define SRV_T_SEC_NETWORKAUTH	(CS_INT)242
#define SRV_T_SEC_DELEGATION	(CS_INT)243
#define SRV_T_SEC_MUTUALAUTH	(CS_INT)244
#define SRV_T_SEC_INTEGRITY	(CS_INT)245
#define SRV_T_SEC_CONFIDENTIALITY	(CS_INT)246
#define SRV_T_SEC_DETECTREPLAY	(CS_INT)247
#define SRV_T_SEC_DETECTSEQ	(CS_INT)248
#define SRV_T_SEC_DATAORIGIN	(CS_INT)249
#define SRV_T_SEC_CHANBIND	(CS_INT)250
#define SRV_T_SEC_SESSTIMEOUT	(CS_INT)251
#define SRV_T_SEC_CREDTIMEOUT	(CS_INT)252
#define SRV_T_SEC_DELEGCRED	(CS_INT)253
#define SRV_T_SEC_MECHANISM	(CS_INT)254
#define SRV_T_FULLPASSTHRU      (CS_INT)255
#define	SRV_T_RMTCERTIFICATE	(CS_INT)256
#define	SRV_T_CIPHER_SUITE	(CS_INT)257
#define	SRV_T_SSL_VERSION	(CS_INT)258
#define SRV_T_NOTIFYPND		(CS_INT)259

#define SRV_T_LASTPROP		(CS_INT)259

/* 
** Possible values for the SRV_T_BULKTYPE thread property
*/
#define	SRV_BULKLOAD		(CS_INT)1
#define	SRV_TEXTLOAD		(CS_INT)2
#define	SRV_IMAGELOAD		(CS_INT)3

/*
** Possible values for option command types
*/
#define	SRV_SETOPTION		(CS_INT)1
#define	SRV_CLEAROPTION		(CS_INT)2
#define	SRV_GETOPTION		(CS_INT)3
#define	SRV_SENDOPTION		(CS_INT)4

/* 
** Possible values for the SRV_T_LOGINTYPE thread property.
*/
#define SRV_SITEHANDLER		(CS_INT)1
#define SRV_SUBCHANNEL		(CS_INT)2
#define SRV_CLIENT		(CS_INT)3

/* 
** Possible values for the SRV_T_NEGLOGIN thread property.
*/
#define SRV_CHALLENGE		(CS_INT)0x0001
#define SRV_ENCRYPT		(CS_INT)0x0002
#define SRV_SECLABEL		(CS_INT)0x0004
#define SRV_APPDEFINED		(CS_INT)0x0008

/*
** Possible values for SRV_T_TYPE thread property
*/
#define	SRV_TCLIENT		(CS_INT)2
#define SRV_TSITE		(CS_INT)4
#define	SRV_TSUBPROC		(CS_INT)5
#define	SRV_TSERVICE		(CS_INT)6

/*
** Possible values for the SRV_T_TDSVERSION and SRV_S_TDSVERSION thread
** property
*/
#define	SRV_TDS_NONE		(CS_INT)0
#define	SRV_TDS_4_0		(CS_INT)4
#define	SRV_TDS_4_0_2		(CS_INT)5
#define	SRV_TDS_4_2		(CS_INT)6
#define	SRV_TDS_4_6		(CS_INT)7
#define	SRV_TDS_4_9_5		(CS_INT)8
#define	SRV_TDS_5_0		(CS_INT)9

/*
** Possible values for SRV_T_BYTEORDER thread property.
*/
#define SRV_LITTLE_ENDIAN	(CS_INT)1
#define SRV_BIG_ENDIAN          (CS_INT)2
 
/* 
** Possible values for SRV_T_FLTTYPE thread property.
*/
#define SRV_FLT_UNKNOWN         (CS_INT)0
#define SRV_FLT_IEEE		(CS_INT)1
#define SRV_FLT_ND5000          (CS_INT)2
#define SRV_FLT_VAX             (CS_INT)3

/* 
** Possible values for SRV_T_CHARTYPE thread property.
*/
#define SRV_CHAR_UNKNOWN        (CS_INT)0
#define SRV_CHAR_ASCII		(CS_INT)1
#define SRV_CHAR_EBCDIC         (CS_INT)2

/*
** Possible modes for adjusting thread priorities.
** SRV_C_NEWPRI is specified with an absolute priority value, and
** SRV_C_DELTAPRI is used to adjust the current value up or down.
*/
#define SRV_C_NEWPRI		0
#define SRV_C_DELTAPRI		1

/*
** Possible values for thread priorities.
** The priority value specified in srv_setpri must be between
** SRV_C_LOWPRIORITY and SRV_C_MAXPRIORITY, inclusive. The default
** value for all Open Server threads is SRV_C_DEFPRIORITY.
*/
#define SRV_C_LOWPRIORITY	(CS_INT)0
#define SRV_C_DEFAULTPRI	(CS_INT)8
#define SRV_C_MAXPRIORITY	(CS_INT)15

/*
** Default stack size argument for srv_spawn
*/
#define SRV_DEFAULT_STACKSIZE		(CS_INT)(-1)

/*
** Minimum allowable stack size that can be configured
*/
#define SRV_MINSTACKSIZE		0x8800

/* 
** Default server configuration parameter defines.
*/
#define SRV_DEF_NUMCONNECTIONS     	(CS_INT)30
#define SRV_DEF_TIMESLICE       	(CS_INT)10
#define SRV_DEF_NUMUSEREVENTS      	(CS_INT)0
#define SRV_DEF_NUMTHREADS		(CS_INT)50
#define SRV_DEF_NUMREMSITES        	(CS_INT)10
#define SRV_DEF_NUMREMBUF		(CS_INT)3
#define SRV_DEF_NUMMUTEXES         	(CS_INT)128
#define SRV_DEF_NETBUFSIZE       	(CS_INT)2048
#define SRV_DEF_NUMMSGQUEUES       	(CS_INT)(SRV_DEF_NUMTHREADS + 128)
#define SRV_DEF_MSGPOOL			(CS_INT)(SRV_DEF_NUMMSGQUEUES * 256)
#define SRV_DEF_VIRTCLKRATE     	(CS_INT)1000000
#define SRV_DEF_DEFQUEUESIZE		(CS_INT)1024
#define	SRV_DEF_LOG			(CS_CHAR *)"srv.log"
#define	SRV_DEF_LOGFD			((CS_BYTE *)NULL)
#define	SRV_DEF_TRACEFD			((CS_BYTE *)NULL)
#define SRV_DEF_STACKSIZE       	SRV_MINSTACKSIZE
#define SRV_DEF_IDLECOUNT		(CS_INT)0

/*
** Open Server prototypes.
*/

CS_START_EXTERN_C

extern CS_RETCODE CS_PUBLIC srv_regwatch PROTOTYPE((
	SRV_PROC *srvproc,
	CS_CHAR *proc_name,
	CS_INT namelen,
	CS_INT options,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_regwatchlist PROTOTYPE((
	SRV_PROC        *srvproc,
	SRV_PROCLIST    **proc_list
	));
extern CS_VOID * CS_PUBLIC srv_alloc PROTOTYPE((
	CS_INT size
	));
extern CS_VOID * CS_PUBLIC srv_realloc PROTOTYPE((
	CS_VOID *ptr,
	CS_INT size
	));
extern CS_INT  CS_PUBLIC srv_select PROTOTYPE((
	CS_INT nfds,
	SRV_MASK_ARRAY *readmask,
	SRV_MASK_ARRAY *writemask,
	SRV_MASK_ARRAY *exceptmask,
	CS_INT waitflag
	));
extern SRV_SIGNAL_FUNC  CS_PUBLIC srv_signal PROTOTYPE((
	CS_INT sig,
	SRV_SIGNAL_FUNC funcp
	));
extern CS_RETCODE CS_PUBLIC srv_sigdump PROTOTYPE((
	CS_INT  sig
	));
extern CS_RETCODE CS_PUBLIC srv_callback PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT callback_type,
	SRV_CALLBACK_FUNC funcp
	));
extern CS_RETCODE CS_PUBLIC srv_regnowatch PROTOTYPE((
	SRV_PROC *srvproc,
	CS_CHAR *proc_name,
	CS_INT namelen,
	CS_INT *info
	));
extern CS_BOOL  CS_PUBLIC srv_capability PROTOTYPE((
	CS_INT capability
	));
extern SRV_PROC * CS_PUBLIC srv_createproc PROTOTYPE((
	SRV_SERVER *server
	));
extern CS_RETCODE CS_PUBLIC srv_charset_set PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT charset
	));
extern CS_RETCODE CS_PUBLIC srv_cursor_props PROTOTYPE((
	SRV_PROC *spp,
	CS_INT cmd,
	SRV_CURDESC *cdp
	));
extern CS_INT  CS_PUBLIC srv_define_event PROTOTYPE((
	SRV_SERVER *ssp,
	CS_INT type,
	CS_CHAR *namep,
	CS_INT namelen
	));
extern CS_INT  CS_PUBLIC srv_dropproc PROTOTYPE((
	SRV_PROC *srvproc
	));
extern CS_RETCODE CS_PUBLIC srv_dynamic PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT cmd,
	CS_INT item,
	CS_VOID *buf,
	CS_INT buflen,
	CS_INT *outlen
	));
extern SRV_ERRHANDLE_FUNC  CS_PUBLIC srv_errhandle PROTOTYPE((
	SRV_ERRHANDLE_FUNC
	));
extern CS_INT  CS_PUBLIC srv_event PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT event,
	CS_VOID *data
	));
extern CS_INT  CS_PUBLIC srv_event_deferred PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT event,
	CS_VOID *data
	));
extern SRV_EVENTHANDLE_FUNC  CS_PUBLIC srv_handle PROTOTYPE((
	SRV_SERVER *unused,
	CS_INT	event,
	SRV_EVENTHANDLE_FUNC
	));
extern SRV_SERVER * CS_PUBLIC srv_init PROTOTYPE((
	SRV_CONFIG *unused,
	CS_CHAR *servername,
	CS_INT namelen
	));
extern CS_INT  CS_PUBLIC srv_langcpy PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT start,
	CS_INT numbytes,
	CS_CHAR *dest
	));
extern CS_INT  CS_PUBLIC srv_langlen PROTOTYPE((
	SRV_PROC *srvproc
	));
extern CS_CHAR * CS_PUBLIC srv_langptr PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT n
	));
extern CS_RETCODE CS_PUBLIC srv_getloginfo PROTOTYPE((
	SRV_PROC *srvproc,
	CS_LOGINFO **loginfo
	));
extern CS_RETCODE CS_PUBLIC srv_setloginfo PROTOTYPE((
	SRV_PROC *srvproc,
	CS_LOGINFO *loginfo
	));
extern CS_RETCODE CS_PUBLIC srv_createmsgq PROTOTYPE((
	CS_CHAR *mqname,
	CS_INT mqlen,
	SRV_OBJID *mqid
	));
extern CS_RETCODE CS_PUBLIC srv_getmsgq PROTOTYPE((
	SRV_OBJID mqid,
	CS_VOID **msgp,
	CS_INT flags,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_putmsgq PROTOTYPE((
	SRV_OBJID mqid,
	CS_VOID *msgp,
	CS_INT flags
	));
extern CS_RETCODE CS_PUBLIC srv_deletemsgq PROTOTYPE((
	CS_CHAR *name,
	CS_INT length,
	SRV_OBJID id
	));
extern CS_RETCODE CS_PUBLIC srv_createmutex PROTOTYPE((
	CS_CHAR *mutex_name,
	CS_INT mutex_namelen,
	SRV_OBJID *mutex_id
	));
extern CS_RETCODE CS_PUBLIC srv_lockmutex PROTOTYPE((
	SRV_OBJID mutex_id,
	CS_INT flags,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_deletemutex PROTOTYPE((
	CS_CHAR *mutex_name,
	CS_INT mutex_namelen,
	SRV_OBJID mutex_id
	));
extern CS_RETCODE CS_PUBLIC srv_unlockmutex PROTOTYPE((
	SRV_OBJID mutex_id
	));
extern CS_RETCODE CS_PUBLIC srv_getobjid PROTOTYPE((
	CS_INT obj_type,
	CS_CHAR *obj_name,
	CS_INT obj_namelen,
	SRV_OBJID *obj_id,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_getobjname PROTOTYPE((
	CS_INT obj_type,
	SRV_OBJID obj_id,
	CS_CHAR *obj_name,
	CS_INT *obj_namelen,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_alt_bind PROTOTYPE((
	SRV_PROC *spp,
	CS_INT altid,
	CS_INT item,
	CS_DATAFMT *lclfmtp,
	CS_BYTE *varaddrp,
	CS_INT *varlenp,
	CS_SMALLINT *indp
	));
extern CS_RETCODE CS_PUBLIC srv_alt_exist PROTOTYPE((
	SRV_PROC *spp,
	CS_INT altid
	));
extern CS_RETCODE CS_PUBLIC srv_alt_descfmt PROTOTYPE((
	SRV_PROC *spp,
	CS_INT altid,
	CS_INT optype,
	CS_INT operand,
	CS_INT item,
	CS_DATAFMT *rmtfmtp
	));
extern CS_RETCODE CS_PUBLIC srv_alt_header PROTOTYPE((
	SRV_PROC *spp,
	CS_INT altid,
	CS_INT numbylist,
	CS_SMALLINT *bylistarray
	));
extern CS_RETCODE CS_PUBLIC srv_alt_xferdata PROTOTYPE((
	SRV_PROC *spp,
	CS_INT altid
	));
extern CS_RETCODE CS_PUBLIC srv_bind PROTOTYPE((
	SRV_PROC *spp,
	CS_INT cmd,
	CS_INT type,
	CS_INT item,
	CS_DATAFMT *lclfmtp,
	CS_BYTE *varaddrp,
	CS_INT *varlenp,
	CS_SMALLINT *indp
	));
extern CS_RETCODE CS_PUBLIC srv_capability_info PROTOTYPE((
        SRV_PROC *spp,
        CS_INT cmd,
        CS_INT type,
        CS_INT capability,
        CS_VOID *val
        ));
extern CS_RETCODE CS_PUBLIC srv_envchange PROTOTYPE((
	SRV_PROC *spp,
	CS_INT type,
	CS_CHAR *oldvalp,
	CS_INT oldvallen,
	CS_CHAR *newvalp,
	CS_INT newvallen
	));
extern CS_RETCODE CS_PUBLIC srv_descfmt PROTOTYPE((
	SRV_PROC *spp,
	CS_INT cmd,
	CS_INT type,
	CS_INT item,
	CS_DATAFMT *rmtfmtp
	));
extern CS_RETCODE CS_PUBLIC srv_envchange PROTOTYPE((
	SRV_PROC *spp,
	CS_INT type,
	CS_CHAR *oldvalp,
	CS_INT oldvallen,
	CS_CHAR *newvalp,
	CS_INT newvallen
	));
extern CS_RETCODE CS_PUBLIC srv_get_text PROTOTYPE((
	SRV_PROC *spp,
	CS_BYTE *buf,
	CS_INT buflen,
	CS_INT *outlen
	));
extern CS_RETCODE CS_PUBLIC srv_msg PROTOTYPE((
	SRV_PROC *spp,
	CS_INT cmd,
	CS_INT *msgid,
	CS_INT *status
	));
extern CS_RETCODE CS_PUBLIC srv_numparams PROTOTYPE((
	SRV_PROC *spp,
	CS_INT *numparams
	));
extern CS_RETCODE CS_PUBLIC srv_orderby PROTOTYPE((
	SRV_PROC *spp,
	CS_INT numcols,
	CS_INT *collist
	));
extern CS_RETCODE CS_PUBLIC srv_props PROTOTYPE((
	CS_CONTEXT *contextp,
	CS_INT cmd,
	CS_INT property,
	CS_VOID *buf,
	CS_INT buflen,
	CS_INT *outlen
	));
extern  CS_RETCODE CS_PUBLIC      srv_sendinfo PROTOTYPE((
        SRV_PROC        *spp,
        CS_SERVERMSG    *errmsg,
        CS_INT          tran_state
        ));
extern CS_RETCODE CS_PUBLIC srv_setcontrol PROTOTYPE((
	SRV_PROC *spp,
	CS_INT colnum,
	CS_BYTE *ctrlinfo,
	CS_INT ctrllen
	));
extern CS_RETCODE CS_PUBLIC srv_send_text PROTOTYPE((
	SRV_PROC *spp,
	CS_BYTE *buf,
	CS_INT buflen
	));
extern CS_RETCODE CS_PUBLIC srv_tabname PROTOTYPE((
	SRV_PROC *spp,
	CS_INT tablenum,
	CS_CHAR *tablename,
	CS_INT namelen
	));
extern CS_RETCODE CS_PUBLIC srv_tabcolname PROTOTYPE((
	SRV_PROC *spp,
	CS_INT colnum,
	CS_BROWSEDESC *bd
	));
extern CS_RETCODE CS_PUBLIC srv_thread_props PROTOTYPE((
        SRV_PROC *spp,
        CS_INT  cmd,
        CS_INT  property,
        CS_VOID *buf,
        CS_INT  buflen,
        CS_INT  *outlen
        ));
extern CS_RETCODE CS_PUBLIC srv_text_info PROTOTYPE((
	SRV_PROC *spp,
	CS_INT cmd,
	CS_INT item,
	CS_IODESC *iodesc
	));
extern CS_RETCODE CS_PUBLIC srv_xferdata PROTOTYPE((
	SRV_PROC *spp,
	CS_INT cmd,
	CS_INT type
	));
extern CS_RETCODE CS_PUBLIC srv_recvpassthru PROTOTYPE((
	SRV_PROC *srvproc,
	CS_BYTE **recv_bufp,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_sendpassthru PROTOTYPE((
	SRV_PROC *srvproc,
	CS_BYTE *send_bufp,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_regdefine PROTOTYPE((
	SRV_PROC *srvproc,
	CS_CHAR *name,
	CS_INT namelen,
	SRV_EVENTHANDLE_FUNC handler
	));
extern CS_RETCODE CS_PUBLIC srv_regexec PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_reginit PROTOTYPE((
	SRV_PROC *srvproc,
	CS_CHAR *name,
	CS_INT namelen,
	CS_INT options
	));
extern CS_RETCODE CS_PUBLIC srv_reglist PROTOTYPE((
	SRV_PROC *srvproc,
	SRV_PROCLIST **proc_list
	));
extern CS_RETCODE CS_PUBLIC srv_reglistfree PROTOTYPE((
	SRV_PROC *srvproc,
	SRV_PROCLIST *proc_list
	));
extern CS_RETCODE CS_PUBLIC srv_regparam PROTOTYPE((
	SRV_PROC *srvproc,
	CS_CHAR *paramname,
	CS_INT namelen,
	CS_INT type,
	CS_INT datalen,
	CS_BYTE *value
	));
extern CS_RETCODE CS_PUBLIC srv_regcreate PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_regdrop PROTOTYPE((
	SRV_PROC *srvproc,
	CS_CHAR *proc_name,
	CS_INT namelen,
	CS_INT *info
	));
extern CS_RETCODE CS_PUBLIC srv_spawn PROTOTYPE((
	SRV_PROC **srvproc,
	CS_INT stacksize,
	SRV_SERVICETHREAD_FUNC funcp,
	CS_VOID *argp,
	CS_INT priority
	));
extern CS_RETCODE CS_PUBLIC srv_yield PROTOTYPE((void));
extern CS_RETCODE CS_PUBLIC srv_sleep PROTOTYPE((
	CS_VOID *sleepevent,
	CS_CHAR *sleeplabel,
	CS_INT sleepflags,
	CS_INT *info,
	CS_VOID	*reserved1,
	CS_VOID	*reserved2
	));
extern CS_RETCODE CS_PUBLIC srv_jsleep PROTOTYPE((
	CS_VOID *sleepevent,
	CS_CHAR *sleeplabel,
	CS_INT sleepflags,
	CS_INT *info,
	CS_VOID	*reserved1,
	CS_VOID	*reserved2
	));
extern CS_RETCODE CS_PUBLIC srv_wakeup PROTOTYPE((
	CS_VOID *sleepevent,
	CS_INT wakeflags,
	CS_VOID	*reserved1,
	CS_VOID	*reserved2
	));
extern CS_RETCODE CS_PUBLIC srv_ucwakeup PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT wakeflags
	));
extern CS_RETCODE CS_PUBLIC srv_termproc PROTOTYPE((
	SRV_PROC *srvproc
	));
extern CS_RETCODE CS_PUBLIC srv_setpri PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT mode,
	CS_INT priority
	));
extern CS_CHAR * CS_PUBLIC srv_rpcdb PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT *len
	));
extern CS_CHAR * CS_PUBLIC srv_rpcname PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT *len
	));
extern CS_SMALLINT CS_PUBLIC srv_rpcnumber PROTOTYPE((
	SRV_PROC *srvproc
	));
extern CS_INT CS_PUBLIC srv_rpcoptions PROTOTYPE((
	SRV_PROC *srvproc
	));
extern CS_CHAR * CS_PUBLIC srv_rpcowner PROTOTYPE((
	SRV_PROC *spp,
	CS_INT *lenp
	));
extern CS_RETCODE CS_PUBLIC srv_run PROTOTYPE((
	SRV_SERVER *unused
	));
extern CS_RETCODE CS_PUBLIC srv_dbg_switch PROTOTYPE((
	CS_INT spid
	));
extern int CS_PUBLIC srv_dbg_stop PROTOTYPE((void));
extern CS_RETCODE CS_PUBLIC srv_dbg_stack PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT depth,
	CS_RETCODE (CS_PUBLIC *funcp)PROTOTYPE((CS_CHAR *buf, CS_INT buflen))
	));
extern CS_RETCODE CS_PUBLIC srv_senddone PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT status,
	CS_INT transtate,
	CS_INT count
	));
extern CS_RETCODE CS_PUBLIC srv_sendstatus PROTOTYPE((
	SRV_PROC *srvproc,
	CS_INT value
	));
extern CS_RETCODE CS_PUBLIC srv_log PROTOTYPE((
	SRV_SERVER *server,
	CS_BOOL dateflag,
	CS_CHAR *msg,
	CS_INT msglen
	));
extern CS_INT CS_PUBLIC srv_poll PROTOTYPE((
	SRV_POLLFD *fdp,
	CS_INT nfds,
	CS_INT waitflag
	));
extern CS_CHAR * CS_PUBLIC srv_symbol PROTOTYPE((
	CS_INT type,
	CS_INT symbol,
	CS_INT *len
	));
extern CS_VOID CS_PUBLIC srv_bmove PROTOTYPE((
	CS_VOID *from,
	CS_VOID *to,
	CS_INT cnt
	));
extern CS_VOID CS_PUBLIC srv_bzero PROTOTYPE((
	CS_VOID *location,
	CS_INT cnt
	));
extern CS_RETCODE CS_PUBLIC srv_free PROTOTYPE((
	CS_VOID *mp
	));
extern CS_RETCODE CS_PUBLIC srv_version PROTOTYPE((
	CS_CONTEXT      *cp,
	CS_INT		version
	));
extern CS_RETCODE CS_PUBLIC srv_mask PROTOTYPE((
	CS_INT		cmd,
	SRV_MASK_ARRAY	*mp,
	CS_INT		bit,
	CS_BOOL		*info
	));
extern CS_RETCODE CS_PUBLIC srv_negotiate PROTOTYPE((
	SRV_PROC	*spp,
	CS_INT		cmd,
	CS_INT		type
	));
extern CS_RETCODE CS_PUBLIC srv_options PROTOTYPE((
	SRV_PROC        *srvproc,
	CS_INT          cmd,
	CS_INT          *optcmd,
	CS_INT          *option,
	CS_VOID         *buf,
	CS_INT          bufsize,
	CS_INT          *outlen
	));
CS_END_EXTERN_C		
#endif	/* __OSPUBLIC_H__ */
