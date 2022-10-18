/*--
	Copyright (C) 2007-2010 Wellbia.com Co., Ltd.

	Module Name:
		zwave_sdk_linker.h

	Abstract:
		XIGNCODE Linker for Server

	Author:
		2010-10-18 HyunOh Kim <zextor@wellbia.com>

	Environment:
		win32

	Library:
		win32, stdc

--*/

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include "zwave_sdk_linker.h"

static HMODULE g_hStcDll = NULL;

typedef xbool	(XCALL* PZSWAVE_InitializeW)(xcwstr path);
typedef xbool	(XCALL* PZSWAVE_InitializeA)(xpcch path);
typedef xbool	(XCALL* PZSWAVE_Uninitialize)();
typedef xulong	(XCALL* PZSWAVE_GetDBConnectionCount)();
typedef xbool	(XCALL* PZSWAVE_GetLibraryPathW)(xwstr path, xulong size);
typedef xbool	(XCALL* PZSWAVE_GetLibraryPathA)(xpch path, xulong size);
typedef xhandle (XCALL* PZSWAVE_OpenAt)(int gi, int di, xulong bs);
typedef xhandle (XCALL* PZSWAVE_Open)(xulong bs, xulong type);
typedef xbool	(XCALL* PZSWAVE_Close)(xhandle h);
typedef xbool	(XCALL* PZSWAVE_GetDataAt)(xhandle h, int no, xpch data);
typedef xbool	(XCALL* PZSWAVE_Verify)(xhandle h, xpcch data, xulong size);
typedef int	(XCALL* PZSWAVE_VerifyExA)(xhandle h, xpcch data, xulong size, xstr msg, xulong msgSize);
typedef int	(XCALL* PZSWAVE_VerifyExW)(xhandle h, xpcch data, xulong size, xwstr msg, xulong msgSize);
typedef int	(XCALL* PZSWAVE_GetDataCount)(xhandle s);
typedef xbool (XCALL* PZSWAVE_CheckUpdates)();


typedef xbool	(XCALL *ZSWAVE_RsaFreeBufferT)(xhandle H, xpuch Buffer);
typedef xhandle	(XCALL *ZSWAVE_RsaCreateT)();
typedef xbool	(XCALL *ZSWAVE_RsaCloseT)(xhandle H);
typedef xbool	(XCALL *ZSWAVE_RsaPublicDecryptT)(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize);
typedef xbool	(XCALL *ZSWAVE_RsaPublicEncryptT)(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize);
typedef xbool	(XCALL *ZSWAVE_RsaPrivateDecryptT)(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize);
typedef xbool	(XCALL *ZSWAVE_RsaPrivateEncryptT)(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize);
typedef xbool	(XCALL *ZSWAVE_RsaSetPublicKeyT)(xhandle H, xpcuch Key, xsize KeySize);
typedef xbool	(XCALL *ZSWAVE_RsaSetPrivateKeyT)(xhandle H, xpcuch Key, xsize KeySize);
typedef xbool	(XCALL *ZSWAVE_RsaGenerateKeyT)(xpuch PublicKey, xsize PublicKeySize, xpuch PrivateKey, xsize PrivateKeySize, int Nbits, int Exponent);
typedef xbool	(XCALL *ZSWAVE_DecryptT)(xpcuch Input, xsize InputSize, xpcuch Key, xsize KeySize, xpuch Output, xsize OutputSize, xsize *Required, CryptType Type);
typedef xbool	(XCALL *ZSWAVE_EncryptT)(xpcuch Input, xsize InputSize, xpcuch Key, xsize KeySize, xpuch Output, xsize OutputSize, xsize *Required, CryptType Type);

PZSWAVE_InitializeW				gZSWAVE_InitializeW				= NULL;
PZSWAVE_InitializeA				gZSWAVE_InitializeA				= NULL;
PZSWAVE_Uninitialize				gZSWAVE_Uninitialize				= NULL;
PZSWAVE_GetDBConnectionCount	gZSWAVE_GetDBConnectionCount	= NULL;
PZSWAVE_GetLibraryPathW			gZSWAVE_GetLibraryPathW			= NULL;
PZSWAVE_GetLibraryPathA			gZSWAVE_GetLibraryPathA			= NULL;
PZSWAVE_OpenAt					gZSWAVE_OpenAt					= NULL;
PZSWAVE_Open						gZSWAVE_Open						= NULL;
PZSWAVE_Close						gZSWAVE_Close						= NULL;
PZSWAVE_GetDataAt					gZSWAVE_GetDataAt					= NULL;
PZSWAVE_Verify					gZSWAVE_Verify					= NULL;
PZSWAVE_VerifyExA					gZSWAVE_VerifyExA					= NULL;
PZSWAVE_VerifyExW					gZSWAVE_VerifyExW					= NULL;
PZSWAVE_GetDataCount				gZSWAVE_GetDataCount				= NULL;
PZSWAVE_CheckUpdates				gZSWAVE_CheckUpdates				= NULL;

ZSWAVE_RsaFreeBufferT				g_ZSWAVE_RsaFreeBuffer			= NULL;
ZSWAVE_RsaCreateT					g_ZSWAVE_RsaCreate				= NULL;
ZSWAVE_RsaCloseT					g_ZSWAVE_RsaClose					= NULL;			
ZSWAVE_RsaPublicDecryptT			g_ZSWAVE_RsaPublicDecrypt		= NULL;	
ZSWAVE_RsaPublicEncryptT			g_ZSWAVE_RsaPublicEncrypt		= NULL;
ZSWAVE_RsaPrivateDecryptT		g_ZSWAVE_RsaPrivateDecrypt		= NULL;
ZSWAVE_RsaPrivateEncryptT		g_ZSWAVE_RsaPrivateEncrypt		= NULL;
ZSWAVE_RsaSetPublicKeyT			g_ZSWAVE_RsaSetPublicKey			= NULL;
ZSWAVE_RsaSetPrivateKeyT			g_ZSWAVE_RsaSetPrivateKey		= NULL;
ZSWAVE_RsaGenerateKeyT			g_ZSWAVE_RsaGenerateKey			= NULL;
ZSWAVE_DecryptT					g_ZSWAVE_Decrypt					= NULL;
ZSWAVE_EncryptT					g_ZSWAVE_Encrypt					= NULL;

xbool GetProcAddressEx(HMODULE module, xpvoid ptr, xpcch name)
{
	FARPROC proc;
	proc = GetProcAddress(module, name);
	if(proc)
	{
		*(FARPROC *) ptr = proc;
		return TRUE;
	}

	return FALSE;
}

xbool ZSWAVE_LinkW(xcwstr pszlibdllpath)
{
	if ( g_hStcDll ) return FALSE;
	
	if ( pszlibdllpath != NULL )
	{
		xwch szT[MAX_PATH];
		wcscpy(szT,pszlibdllpath);
		if(szT[wcslen(szT) -1] != L'\\') wcscat(szT, L"\\");
		wcscat(szT, LINK_DLL_NAME);
		g_hStcDll = LoadLibraryW(szT);	

	}
	else
	{
		xwch szT[MAX_PATH];
		GetModuleFileNameW(NULL, szT, MAX_PATH);
		xwch *p = wcsrchr(szT, '\\');
		*(p+1) = 0x00;
		wcscat(szT, LINK_DLL_NAME);
		g_hStcDll = LoadLibraryW(szT);	
	}

	if ( g_hStcDll == NULL ) 
	{
		SetLastError(ZWAVE_STATUS_LINKER_INVALID_LIBDIR);
		return FALSE;
	}

	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_InitializeW,				"ZSWAVE_InitializeW"))			goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_InitializeA,				"ZSWAVE_InitializeA"))			goto $CLEAR;			
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_Uninitialize,			"ZSWAVE_Uninitialize"))			goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_GetDBConnectionCount,	"ZSWAVE_GetDBConnectionCount"))	goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_GetLibraryPathW,		"ZSWAVE_GetLibraryPathW"))		goto $CLEAR;		
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_GetLibraryPathA,		"ZSWAVE_GetLibraryPathA"))		goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_OpenAt,					"ZSWAVE_OpenAt"))					goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_Open,						"ZSWAVE_Open"))					goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_Close,					"ZSWAVE_Close"))					goto $CLEAR;					
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_GetDataAt,				"ZSWAVE_GetDataAt"))				goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_Verify,					"ZSWAVE_Verify"))					goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_VerifyExA,				"ZSWAVE_VerifyExA"))				goto $CLEAR;				
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_VerifyExW,				"ZSWAVE_VerifyExW"))				goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_GetDataCount,			"ZSWAVE_GetDataCount"))			goto $CLEAR;		
	if (!GetProcAddressEx(g_hStcDll, &gZSWAVE_CheckUpdates,			"ZSWAVE_CheckUpdates"))			goto $CLEAR;

	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaFreeBuffer,			"ZSWAVE_RsaFreeBuffer"))			goto $CLEAR;		
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaCreate,				"ZSWAVE_RsaCreate"))				goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaClose,				"ZSWAVE_RsaClose"))				goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaPublicDecrypt,		"ZSWAVE_RsaPublicDecrypt"))		goto $CLEAR;	
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaPublicEncrypt,		"ZSWAVE_RsaPublicEncrypt"))		goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaPrivateDecrypt,		"ZSWAVE_RsaPrivateDecrypt"))		goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaPrivateEncrypt,		"ZSWAVE_RsaPrivateEncrypt"))		goto $CLEAR;
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaSetPublicKey,		"ZSWAVE_RsaSetPublicKey"))		goto $CLEAR;		
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaSetPrivateKey,		"ZSWAVE_RsaSetPrivateKey"))		goto $CLEAR;	
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_RsaGenerateKey,		"ZSWAVE_RsaGenerateKey"))			goto $CLEAR;		
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_Decrypt,				"ZSWAVE_Decrypt"))				goto $CLEAR;				
	if (!GetProcAddressEx(g_hStcDll, &g_ZSWAVE_Encrypt,				"ZSWAVE_Encrypt"))				goto $CLEAR;

	return TRUE;
$CLEAR:
	FreeLibrary(g_hStcDll);
	g_hStcDll = NULL;
	return FALSE;
}

xbool ZSWAVE_LinkA(xpcch pszlibdllpath)
{
	if ( pszlibdllpath != NULL )
	{
		xwch libPath[MAX_PATH + 1];
		if (!MultiByteToWideChar(CP_ACP, 0, pszlibdllpath, -1, libPath, MAX_PATH))
		{
			return FALSE;
		} 
		else return ZSWAVE_LinkW(libPath);
	}
	return ZSWAVE_LinkW(NULL);
}


xbool ZSWAVE_UnLink()
{
	if(g_hStcDll)
	{
		FreeLibrary(g_hStcDll);
		g_hStcDll = NULL;
	}

	return TRUE;
}

xbool XCALL ZSWAVE_InitializeW(xcwstr path)
{
	if ( !ZSWAVE_LinkW(path) ) return FALSE;

	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_InitializeW(path);
	else return FALSE;
}

xbool XCALL ZSWAVE_InitializeA(xpcch path)
{
	if ( !ZSWAVE_LinkA(path) ) return FALSE;

	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_InitializeA(path);
	else return FALSE;
}

xbool XCALL ZSWAVE_Uninitialize()
{
	xbool bret = FALSE;
	if ( g_hStcDll == NULL ) return bret;
	if ( gZSWAVE_InitializeW ) 
		bret = gZSWAVE_Uninitialize();
	
	if ( bret ) 
		bret = ZSWAVE_UnLink();
	
	return bret;
}

xulong XCALL ZSWAVE_GetDBConnectionCount()
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetDBConnectionCount();
	else return FALSE;
}

xbool XCALL ZSWAVE_GetLibraryPathW(xwstr path, xulong size)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetLibraryPathW(path, size);
	else return FALSE;
}

xbool XCALL ZSWAVE_GetLibraryPathA(xpch path, xulong size)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetLibraryPathA(path, size);
	else return FALSE;
}

xhandle XCALL ZSWAVE_OpenAt(int gi, int di, xulong bs)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_OpenAt(gi, di, bs);
	else return FALSE;
}

xhandle XCALL ZSWAVE_Open(xulong bs, xulong type)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_Open(bs, type);
	else return FALSE;
}

xbool XCALL ZSWAVE_Close(xhandle h)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_Close(h);
	else return FALSE;
}

xbool XCALL ZSWAVE_GetDataAt(xhandle h, int no, xpch data)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetDataAt(h, no, data);
	else return FALSE;
}

xbool XCALL ZSWAVE_Verify(xhandle h, xpcch data, xulong size)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_Verify(h, data, size);
	else return FALSE;
}

int	XCALL ZSWAVE_VerifyExA(xhandle h, xpcch data, xulong size, xstr msg, xulong msgSize)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_VerifyExA(h, data, size, msg, msgSize);
	else return FALSE;
}

int XCALL ZSWAVE_VerifyExW(xhandle h, xpcch data, xulong size, xwstr msg, xulong msgSize)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_VerifyExW(h, data, size, msg, msgSize);
	else return FALSE;
}

int	XCALL ZSWAVE_GetDataCount(xhandle s)
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_InitializeW ) return gZSWAVE_GetDataCount(s);
	else return FALSE;
}

xbool XCALL ZSWAVE_CheckUpdates()
{
	if ( g_hStcDll == NULL ) return FALSE;
	if ( gZSWAVE_CheckUpdates ) return gZSWAVE_CheckUpdates();
	else return FALSE;
}

xbool XCALL ZSWAVE_RsaFreeBuffer(xhandle H, xpuch Buffer)
{
	if ( g_ZSWAVE_RsaFreeBuffer )
		return g_ZSWAVE_RsaFreeBuffer(H, Buffer);
	else return NULL;
}

xhandle	XCALL ZSWAVE_RsaCreate()
{
	if ( g_ZSWAVE_RsaCreate )
		return g_ZSWAVE_RsaCreate();
	else return NULL;
}

xbool XCALL ZSWAVE_RsaClose(xhandle H)
{
	if ( g_ZSWAVE_RsaClose )
		return g_ZSWAVE_RsaClose(H);
	else return NULL;
}

xbool XCALL ZSWAVE_RsaPublicDecrypt(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize)
{
	if ( g_ZSWAVE_RsaPublicDecrypt )
		return g_ZSWAVE_RsaPublicDecrypt(H, Input, InputSize, Output, OutputSize);
	else return NULL;
}

xbool XCALL ZSWAVE_RsaPublicEncrypt(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize)
{
	if ( g_ZSWAVE_RsaPublicEncrypt )
		return g_ZSWAVE_RsaPublicEncrypt(H, Input, InputSize, Output, OutputSize);
	else return NULL;
}

xbool XCALL ZSWAVE_RsaPrivateDecrypt(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize)
{
	if ( g_ZSWAVE_RsaPrivateDecrypt )
		return g_ZSWAVE_RsaPrivateDecrypt(H, Input, InputSize, Output, OutputSize);
	else return NULL;
}

xbool XCALL ZSWAVE_RsaPrivateEncrypt(xhandle H, xpcuch Input, xsize InputSize, xpuch *Output, xsize *OutputSize)
{
	if ( g_ZSWAVE_RsaPrivateEncrypt )
		return g_ZSWAVE_RsaPrivateEncrypt(H, Input, InputSize, Output, OutputSize);
	else return NULL;
}

xbool XCALL ZSWAVE_RsaSetPublicKey(xhandle H, xpcuch Key, xsize KeySize)
{
	if ( g_ZSWAVE_RsaSetPublicKey )
		return g_ZSWAVE_RsaSetPublicKey(H, Key, KeySize);
	else return NULL;
}

xbool XCALL ZSWAVE_RsaSetPrivateKey(xhandle H, xpcuch Key, xsize KeySize)
{
	if ( g_ZSWAVE_RsaSetPrivateKey )
		return g_ZSWAVE_RsaSetPrivateKey(H, Key, KeySize);
	else return NULL;
}

xbool XCALL ZSWAVE_RsaGenerateKey(xpuch PublicKey, xsize PublicKeySize, xpuch PrivateKey, xsize PrivateKeySize, int Nbits, int Exponent)
{
	if ( g_ZSWAVE_RsaGenerateKey )
		return g_ZSWAVE_RsaGenerateKey(PublicKey, PublicKeySize, PrivateKey, PrivateKeySize, Nbits, Exponent);
	else return NULL;
}

xbool XCALL ZSWAVE_Decrypt(xpcuch Input, xsize InputSize, xpcuch Key, xsize KeySize, xpuch Output, xsize OutputSize, xsize *Required, CryptType Type)
{
	if ( g_ZSWAVE_Decrypt )
		return g_ZSWAVE_Decrypt(Input, InputSize, Key, KeySize, Output, OutputSize, Required, Type);
	else return NULL;

}
xbool XCALL ZSWAVE_Encrypt(xpcuch Input, xsize InputSize, xpcuch Key, xsize KeySize, xpuch Output, xsize OutputSize, xsize *Required, CryptType Type)
{
	if ( g_ZSWAVE_Encrypt )
		return g_ZSWAVE_Encrypt(Input, InputSize, Key, KeySize, Output, OutputSize, Required, Type);
	else return NULL;
}
