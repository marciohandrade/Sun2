/*--
	Copyright (C) 2007-2009 Wellbia.com Co., Ltd.

	Module Name:
		zwave_sdk_client.h

	Abstract:
		XIGNCODE Client Library Stub Code

	Author:
		2010-09-16 YoungJin Shin <codewiz@wellbia.com>

	Environment:
		any

	Library:
		none

--*/

#ifndef ZCWAVE_SDK_CLIENT_H
#define ZCWAVE_SDK_CLIENT_H

#include "xtypes.h"
#include "zcwave_errors.h"

#define STUB_NAME L"x3.xem"
#define STUB_FILE_NAME L"\\" STUB_NAME

#define XINLINE __inline

#define ZSOPT_USESYSGUARD	0x01
#define ZSOPT_SHOWSPLASH	0x02
#define ZSOPT_SHOWTRAY		0x04

enum
{
	XIGNCODE_REQUEST_EXIT = 0x1200
};

enum
{
	XSC_INTERNAL_STATE_CHANGED = 0x3120
	, XSC_FILE_MODIFIED
	, XSC_VASHJ_LOAD_FAIL
	, XSC_VASHJ_INIT_FAIL
	, XSC_XSG_CONNECT_FAIL
};


typedef enum _XclioFid
{
	XclioFidSysEnterA
	, XclioFidSysEnterW
	, XclioFidSysExit
	, XclioFidInit
	, XclioFidCleanup
	, XclioFidProbe
	, XclioFidProbeEx
	, XclioFidCreateCodeBox
	, XclioFidCloseCodeBox
	, XclioFidProbeCodeBox
	, XclioFidProbeCodeBoxEx
	, XclioFidEncrypt
	, XclioFidDecrypt
	, XclioFidRsaCreate
	, XclioFidRsaClose
	, XclioFidRsaSetPublicKey
	, XclioFidRsaSetPrivateKey
	, XclioFidRsaPublicEncrypt
	, XclioFidRsaPublicDecrypt
	, XclioFidRsaPrivateEncrypt
	, XclioFidRsaPrivateDecrypt
	, XclioFidCheck
	, XclioFidRegisterCallback
	, XclioFidRsaGenerateKey
	, XclioFidRsaFreeBuffer
	, XclioFidSetup
	, XclioFidSendCommandVa
} XclioFid;


#define ZCWAVE_EQ_REMOTE_IP			0x1
#define ZCWAVE_EQ_REMOTE_PORT		0x2
#define ZCWAVE_EQ_LOCAL_IP			0x4
#define ZCWAVE_EQ_LOCAL_PORT		0x8
#define ZCWAVE_EQ_PID				0x10
#define ZCWAVE_EQ_PROTOCOL			0x20
#define ZCWAVE_EQ_DIRECTION			0x40

#define ZCWAVE_NEQ_REMOTE_IP		0x00010000
#define ZCWAVE_NEQ_REMOTE_PORT		0x00020000
#define ZCWAVE_NEQ_LOCAL_IP			0x00040000
#define ZCWAVE_NEQ_LOCAL_PORT		0x00080000
#define ZCWAVE_NEQ_PID				0x00100000
#define ZCWAVE_NEQ_PROTOCOL			0x00200000
#define ZCWAVE_NEQ_DIRECTION		0x00400000


#define ZCWAVE_ACT_ALLOW			1
#define ZCWAVE_ACT_DENY				2
#define ZCWAVE_ACT_PROMPT			4

#define ZCWAVE_DIR_IN				1
#define ZCWAVE_DIR_OUT				2

#define ZCWAVE_PROTO_UDP			1

#define ZCWAVE_AM_REMOTE_IP			0x1
#define ZCWAVE_AM_REMOTE_PORT		0x2
#define ZCWAVE_AM_LOCAL_IP			0x4
#define ZCWAVE_AM_LOCAL_PORT		0x8
#define ZCWAVE_AM_PID				0x10
#define ZCWAVE_AM_PROTOCOL			0x20
#define ZCWAVE_AM_DIRECTION			0x40

typedef struct _ZCWAVE_NET_RULE
{
	ULONG		conditions;
	ULONG		action;
	ULONG		direction;

	ULONG		remote_ip;
	USHORT		remote_port;

	ULONG		local_ip;
	USHORT		local_port;

	ULONG		pid;
	ULONG		protocol;
	
} ZCWAVE_NET_RULE, *PZCWAVE_NET_RULE;

typedef char *PACKETDATA;
typedef const char *CPACKETDATA;


typedef xbool (CALLBACK *XigncodeCallback)(ULONG code, ULONG w, PVOID l, PVOID context);

typedef void (CALLBACK *ProbeCallback)(xhandle CodeBox
									    , CPACKETDATA Packet
										, PACKETDATA Buffer
										, SIZE_T BufferSize
										, PVOID Context);




typedef xbool (XCALL *XxxSysEnterAT)(xcstr License, xcstr Dir, xulong Flags);

typedef xbool (XCALL *XxxSysEnterWT)(xcwstr License, xcwstr path, xulong opt);

typedef xbool (XCALL *XxxSysExitT)();

typedef xbool (XCALL *XxxInitT)();

typedef xbool (XCALL *XxxCleanupT)();

typedef xbool (XCALL *XxxProbeT)(xpcch data, xpch buf, xsize size);

typedef xbool (XCALL *XxxProbeExT)(xpcch Packet
		, xsize ReplySize
		, ProbeCallback Callback
		, xpvoid Context);

typedef xhandle (XCALL *XxxCreateCodeBoxT)();

typedef xbool (XCALL *XxxCloseCodeBoxT)(xhandle h);

typedef xbool (XCALL *XxxProbeCodeBoxT)(xhandle h, xpcch data, xpch buf, xsize size);

typedef xbool (XCALL *XxxProbeCodeBoxExT)(xhandle h
				, xpcch Packet
				, xsize ReplySize
				, ProbeCallback Callback
				, xpvoid Context);

typedef void (XCALL *XxxRegisterCallbackT)(XigncodeCallback cb, xpvoid context);

typedef void (XCALL *XxxPumpLoopT)();


typedef xstatus (XCALL *XxxQueryFunctionT)(xpvoid *If, XclioFid Id);
typedef xbool (XCALL *XxxSendCommandVaT)(xuint cid, va_list ap);

typedef xbool (XCALL *XxxEncryptT)
(
	xpcuch Input
	, xsize InputSize
	, xpcuch Key
	, xsize KeySize
	, xpuch Output
	, xsize OutputSize
	, xsize *Required
	, int Type
);

typedef xbool (XCALL *XxxDecryptT)
(
	xpcuch Input
	, xsize InputSize
	, xpcuch Key
	, xsize KeySize
	, xpuch Output
	, xsize OutputSize
	, xsize *Required
	, int Type
);

typedef xbool (XCALL *XxxRsaGenerateKeyT)
(
	xpuch PublicKey
	, xsize PublicKeySize
	, xpuch PrivateKey
	, xsize PrivateKeySize
	, int Nbits
	, int Exponent
);

typedef xbool (XCALL *XxxRsaSetPrivateKeyT)(xhandle H, xpcuch Key, xsize KeySize);

typedef xbool (XCALL *XxxRsaSetPublicKeyT)(xhandle H, xpcuch Key, xsize KeySize);

typedef xbool (XCALL *XxxRsaPrivateEncryptT)
(
	xhandle H
	, xpcuch Input
	, xsize InputSize
	, xpuch *Output
	, xsize *OutputSize
);

typedef xbool (XCALL *XxxRsaPrivateDecryptT)
(
	xhandle H
	, xpcuch Input
	, xsize InputSize
	, xpuch *Output
	, xsize *OutputSize
);

typedef xbool (XCALL *XxxRsaPublicEncryptT)
(
	xhandle H
	, xpcuch Input
	, xsize InputSize
	, xpuch *Output
	, xsize *OutputSize
);

typedef xbool (XCALL *XxxRsaPublicDecryptT)
(
	xhandle H
	, xpcuch Input
	, xsize InputSize
	, xpuch *Output
	, xsize *OutputSize
);

typedef xbool (XCALL *XxxRsaCloseT)(xhandle H);

typedef xhandle (XCALL *XxxRsaCreateT)();

typedef xbool (XCALL *XxxRsaFreeBufferT)(xhandle H, xpuch Buffer);


#define XUID_USER_ACCOUNT	1
#define XUID_USER_NAME		2
#define XUID_USER_MAP_POINT	3
#define XUID_USER_EXTRA		4

#define ZCMD_SET_USER_INFO_CALLBACKA	100
#define ZCMD_SET_USER_INFO_CALLBACKW	200
#define ZCMD_GET_USER_INFO_CALLBACKA	300
#define ZCMD_GET_USER_INFO_CALLBACKW	400
#define ZCMD_GET_USER_INFO_CONTEXTA		500
#define ZCMD_GET_USER_INFO_CONTEXTW		600
#define ZCMD_SET_NOTIFICATION_CALLBACK	700
#define ZCMD_GET_NOTIFICATION_CALLBACK	800
#define ZCMD_GET_NOTIFICATION_CONTEXT	900
#define ZCMD_NET_ADD_RULE				1000
#define ZCMD_NET_REMOVE_RULE			1100

typedef 
xstatus 
(XCALL *ZCWAVE_GetUserInfoCallbackAT)(xuint info_id
										, xstr buffer
										, xsize buffer_size
										, xpvoid context);

typedef 
xstatus 
(XCALL *ZCWAVE_GetUserInfoCallbackWT)(xuint info_id
										, xwstr buffer
										, xsize buffer_size
										, xpvoid context);


XINLINE
xbool
XCALL
XxxGetProcAddress(xvoid **Address, XclioFid Fid)
{
	HMODULE Module = GetModuleHandleW(STUB_NAME);
	if(!Module)
	{
		SetLastError(ZCWAVE_E_INVALID_CALL);
		return X_FALSE;
	}

	XxxQueryFunctionT Q = (XxxQueryFunctionT)(xpvoid) GetProcAddress(Module, MAKEINTRESOURCEA(1));
	if(!Q)
	{
		SetLastError(ZCWAVE_E_INVALID_CALL);
		return X_FALSE;
	}

	if(!X_SUCCEEDED(Q(Address, Fid)))
		return X_FALSE;

	return X_TRUE;
}

XINLINE
void
XCALL
GetXigncodeDirectoryW(xwstr Dir, xsize DirSize, xcwstr InputDir)
{
	WCHAR *DirMark;

	if(!InputDir)
	{
		GetModuleFileNameW(X_NULL, Dir, (xulong)(DirSize / sizeof(WCHAR)));
		DirMark = wcsrchr(Dir, X_DIR_MARKW);
		if(DirMark)
			*DirMark = X_EOSW;
	}
	else
	{
		WCHAR *DriveMark = (WCHAR *) wcschr(InputDir, L':');
		if(DriveMark)
		{
			#ifdef _STRSAFE_H_INCLUDED_
			StringCbCopyW(Dir, DirSize, InputDir);
			#else
			lstrcpyW(Dir, InputDir);
			#endif
		}
		else
		{
			GetModuleFileNameW(X_NULL, Dir, (xulong)(DirSize / sizeof(WCHAR)));
			DirMark = wcsrchr(Dir, X_DIR_MARKW);
			if(DirMark)
			{
				if(*InputDir == X_DIR_MARKW)
					*DirMark = X_EOSW;
				else
					*(DirMark + 1) = X_EOSW;
			}

			#ifdef _STRSAFE_H_INCLUDED_
			StringCbCatW(Dir, DirSize, InputDir);
			#else
			lstrcatW(Dir, InputDir);
			#endif
		}

		if(Dir[lstrlenW(Dir) - 1] == X_DIR_MARKW)
			Dir[lstrlenW(Dir) - 1] = X_EOSW;
	}
}

XINLINE
xbool
XCALL
ZCWAVE_SysInit()
{
	return X_TRUE;
}

XINLINE
xbool
XCALL
ZCWAVE_SysCleanup()
{
	return X_TRUE;
}

XINLINE
xbool
XCALL
ZCWAVE_Init()
{
	XxxInitT XxxInit;
	if(!XxxGetProcAddress((xpvoid *)&XxxInit, XclioFidInit))
		return X_FALSE;

	return XxxInit();
}

XINLINE
xbool
XCALL
ZCWAVE_Cleanup()
{
	XxxCleanupT XxxCleanup;
	if(!XxxGetProcAddress((xpvoid *)&XxxCleanup, XclioFidCleanup))
		return X_FALSE;

	return XxxCleanup();
}

XINLINE
xbool
XCALL
ZCWAVE_Probe(xpcch ReceivedPacket, xpch Buffer, xsize BufferSize)
{
	XxxProbeT XxxProbe;
	if(!XxxGetProcAddress((xpvoid *)&XxxProbe, XclioFidProbe))
		return X_FALSE;

	return XxxProbe(ReceivedPacket, Buffer, BufferSize);
}

XINLINE
xhandle
XCALL
ZCWAVE_CreateCodeBox()
{
	XxxCreateCodeBoxT XxxCreateCodeBox;
	if(!XxxGetProcAddress((xpvoid *)&XxxCreateCodeBox, XclioFidCreateCodeBox))
		return X_FALSE;

	return XxxCreateCodeBox();
}

XINLINE
xbool
XCALL
ZCWAVE_CloseCodeBox(xhandle CodeBox)
{
	XxxCloseCodeBoxT XxxCloseCodeBox;
	if(!XxxGetProcAddress((xpvoid *)&XxxCloseCodeBox, XclioFidCloseCodeBox))
		return X_FALSE;

	return XxxCloseCodeBox(CodeBox);
}

XINLINE
xbool
XCALL
ZCWAVE_ProbeCodeBox(xhandle CodeBox, xpcch ReceivedPacket, xpch Buffer, xsize BufferSize)
{
	XxxProbeCodeBoxT XxxProbeCodeBox;
	if(!XxxGetProcAddress((xpvoid *)&XxxProbeCodeBox, XclioFidProbeCodeBox))
		return X_FALSE;

	return XxxProbeCodeBox(CodeBox, ReceivedPacket, Buffer, BufferSize);
}

XINLINE
xbool
XCALL
ZCWAVE_SendCommand(xuint cid, ...)
{
	va_list ap;
	xbool r;
	XxxSendCommandVaT XxxSendCommandVa;
	if(!XxxGetProcAddress((xpvoid *)&XxxSendCommandVa, XclioFidSendCommandVa))
		return X_FALSE;

	va_start(ap, cid);
	r = XxxSendCommandVa(cid, ap);
	va_end(ap);
	
	return r;
}


XINLINE
xbool
XCALL
ZCWAVE_SysEnterW(xcwstr License, xcwstr Path, xulong Flags)
{
	xwch StubPath[MAX_PATH], XigncodeDir[MAX_PATH];
	GetXigncodeDirectoryW(XigncodeDir, sizeof(XigncodeDir), Path);

	#ifdef _STRSAFE_H_INCLUDED_
	StringCbCopyW(StubPath, sizeof(StubPath), XigncodeDir);
	StringCbCatW(StubPath, sizeof(StubPath), STUB_FILE_NAME);
	#else
	lstrcpyW(StubPath, XigncodeDir);
	lstrcatW(StubPath, STUB_FILE_NAME);
	#endif

	PVOID Module = LoadLibraryW(StubPath);
	if(!Module)
	{
		return X_FALSE;
	}

	XxxSysEnterWT XxxSysEnterW;
	if(!XxxGetProcAddress((xpvoid *)&XxxSysEnterW, XclioFidSysEnterW))
	{
		return X_FALSE;
	}

	return XxxSysEnterW(License, XigncodeDir, Flags);
}

XINLINE
xbool
XCALL
ZCWAVE_SysEnterA(xcstr License, xcstr Path, xulong Flags)
{
	xwch wLicense[80];
	xwch wPathBuffer[MAX_PATH];
	xwstr wPath = X_NULL;

	MultiByteToWideChar(CP_ACP, 0, License, -1, wLicense, XArraySize(wLicense));

	if(Path)
	{
		MultiByteToWideChar(CP_ACP, 0, Path, -1, wPathBuffer, XArraySize(wPathBuffer));
		wPath = wPathBuffer;
	}

	return ZCWAVE_SysEnterW(wLicense, wPath, Flags);
}


XINLINE
xbool
XCALL
ZCWAVE_SysExit()
{
	XxxSysExitT XxxSysExit;
	if(!XxxGetProcAddress((xpvoid *)&XxxSysExit, XclioFidSysExit))
		return X_FALSE;

	xbool R = XxxSysExit();

	HMODULE Module = GetModuleHandleW(STUB_NAME);
	FreeLibrary(Module);
	return R;
}



XINLINE
xbool
XCALL
ZCWAVE_RegisterCallback(XigncodeCallback Callback, xpvoid Context)
{
	XxxRegisterCallbackT XxxRegisterCallback;
	if(!XxxGetProcAddress((xpvoid *)&XxxRegisterCallback, XclioFidRegisterCallback))
		return X_FALSE;

	XxxRegisterCallback(Callback, Context);
	return X_TRUE;
}

XINLINE
xbool
XCALL
ZCWAVE_ProbeCodeBoxEx(xhandle CodeBox, xpcch ReceivedPacket, xsize ReplySize, ProbeCallback Callback, xpvoid Context)
{
	XxxProbeCodeBoxExT XxxProbeCodeBoxEx;
	if(!XxxGetProcAddress((xpvoid *)&XxxProbeCodeBoxEx, XclioFidProbeCodeBoxEx))
		return X_FALSE;

	return XxxProbeCodeBoxEx(CodeBox, ReceivedPacket, ReplySize, Callback, Context);
}

XINLINE
xbool
XCALL
ZCWAVE_ProbeEx(xpcch ReceivedPacket, xsize ReplySize, ProbeCallback Callback, xpvoid Context)
{
	XxxProbeExT XxxProbeEx;
	if(!XxxGetProcAddress((xpvoid *)&XxxProbeEx, XclioFidProbeEx))
		return X_FALSE;	

	return XxxProbeEx(ReceivedPacket, ReplySize, Callback, Context);
}

XINLINE
xbool
XCALL
ZCWAVE_Encrypt
(
	xpcuch Input
	, xsize InputSize
	, xpcuch Key
	, xsize KeySize
	, xpuch Output
	, xsize OutputSize
	, xsize *Required
	, int Type
)
{
	XxxEncryptT XxxEncrypt;
	if(!XxxGetProcAddress((xpvoid *)&XxxEncrypt, XclioFidEncrypt))
		return X_FALSE;	

	return XxxEncrypt(Input, InputSize, Key, KeySize, Output, OutputSize, Required, Type);
}

XINLINE
xbool
XCALL
ZCWAVE_Decrypt
(
	xpcuch Input
	, xsize InputSize
	, xpcuch Key
	, xsize KeySize
	, xpuch Output
	, xsize OutputSize
	, xsize *Required
	, int Type
)
{
	XxxDecryptT XxxDecrypt;
	if(!XxxGetProcAddress((xpvoid *)&XxxDecrypt, XclioFidDecrypt))
		return X_FALSE;	

	return XxxDecrypt(Input, InputSize, Key, KeySize, Output, OutputSize, Required, Type);

}

XINLINE
xbool
XCALL
ZCWAVE_RsaGenerateKey
(
	xpuch PublicKey
	, xsize PublicKeySize
	, xpuch PrivateKey
	, xsize PrivateKeySize
	, int Nbits
	, int Exponent	
)
{
	XxxRsaGenerateKeyT XxxRsaGenerateKey;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaGenerateKey, XclioFidRsaGenerateKey))
		return X_FALSE;	

	return XxxRsaGenerateKey(PublicKey, PublicKeySize, PrivateKey, PrivateKeySize, Nbits, Exponent);

}

XINLINE
xbool
XCALL
ZCWAVE_RsaSetPrivateKey(xhandle H, xpcuch Key, xsize KeySize)
{
	XxxRsaSetPrivateKeyT XxxRsaSetPrivateKey;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaSetPrivateKey, XclioFidRsaSetPrivateKey))
		return X_FALSE;	

	return XxxRsaSetPrivateKey(H, Key, KeySize);

}

XINLINE
xbool
XCALL
ZCWAVE_RsaSetPublicKey(xhandle H, xpcuch Key, xsize KeySize)
{
	XxxRsaSetPublicKeyT XxxRsaSetPublicKey;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaSetPublicKey, XclioFidRsaSetPublicKey))
		return X_FALSE;	

	return XxxRsaSetPublicKey(H, Key, KeySize);
}

XINLINE
xbool
XCALL
ZCWAVE_RsaPrivateEncrypt
(
	xhandle H
	, xpcuch Input
	, xsize InputSize
	, xpuch *Output
	, xsize *OutputSize
)
{
	XxxRsaPrivateEncryptT XxxRsaPrivateEncrypt;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaPrivateEncrypt, XclioFidRsaPrivateEncrypt))
		return X_FALSE;	

	return XxxRsaPrivateEncrypt(H, Input, InputSize, Output, OutputSize);
}

XINLINE
xbool
XCALL
ZCWAVE_RsaPrivateDecrypt
(
	xhandle H
	, xpcuch Input
	, xsize InputSize
	, xpuch *Output
	, xsize *OutputSize
)
{
	XxxRsaPrivateDecryptT XxxRsaPrivateDecrypt;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaPrivateDecrypt, XclioFidRsaPrivateDecrypt))
		return X_FALSE;	

	return XxxRsaPrivateDecrypt(H, Input, InputSize, Output, OutputSize);
}

XINLINE
xbool
XCALL
ZCWAVE_RsaPublicEncrypt
(
	xhandle H
	, xpcuch Input
	, xsize InputSize
	, xpuch *Output
	, xsize *OutputSize
)
{
	XxxRsaPublicEncryptT XxxRsaPublicEncrypt;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaPublicEncrypt, XclioFidRsaPublicEncrypt))
		return X_FALSE;	

	return XxxRsaPublicEncrypt(H, Input, InputSize, Output, OutputSize);
}

XINLINE
xbool
XCALL
ZCWAVE_RsaPublicDecrypt
(
	xhandle H
	, xpcuch Input
	, xsize InputSize
	, xpuch *Output
	, xsize *OutputSize
)
{
	XxxRsaPublicDecryptT XxxRsaPublicDecrypt;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaPublicDecrypt, XclioFidRsaPublicDecrypt))
		return X_FALSE;	

	return XxxRsaPublicDecrypt(H, Input, InputSize, Output, OutputSize);
}

XINLINE
xbool
XCALL
ZCWAVE_RsaClose(xhandle H)
{
	XxxRsaCloseT XxxRsaClose;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaClose, XclioFidRsaClose))
		return X_FALSE;	

	return XxxRsaClose(H);
}

XINLINE
xhandle 
XCALL 
ZCWAVE_RsaCreate()
{
	XxxRsaCreateT XxxRsaCreate;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaCreate, XclioFidRsaCreate))
		return X_FALSE;	

	return XxxRsaCreate();
}

XINLINE
xbool
XCALL
ZCWAVE_RsaFreeBuffer(xhandle H, xpuch Buffer)
{
	XxxRsaFreeBufferT XxxRsaFreeBuffer;
	if(!XxxGetProcAddress((xpvoid *)&XxxRsaFreeBuffer, XclioFidRsaFreeBuffer))
		return X_FALSE;	

	return XxxRsaFreeBuffer(H, Buffer);
}

#ifdef _UNICODE
#define ZCWAVE_SysEnter ZCWAVE_SysEnterW
#else
#define ZCWAVE_SysEnter ZCWAVE_SysEnterA
#endif

#endif