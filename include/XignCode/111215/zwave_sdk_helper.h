#ifndef ZWAVE_SDK_HELPER_H
#define ZWAVE_SDK_HELPER_H

#include "xtypes.h"

#ifdef UNIX
#define XIGNCODE_SERVER_EXT_NAMEA ".so"
#define XIGNCODE_SERVER_EXT_NAMEW L".so"
#else
#define XIGNCODE_SERVER_EXT_NAMEA ".dll"
#define XIGNCODE_SERVER_EXT_NAMEW L".dll"
#endif

#ifdef _M_X64
#define XIGNCODE_SERVER_64_NAMEA "_x64"
#define XIGNCODE_SERVER_64_NAMEW L"_x64"
#else
#define XIGNCODE_SERVER_64_NAMEA
#define XIGNCODE_SERVER_64_NAMEW
#endif

#define XIGNCODE_SERVER_NAMEA	"zwave_sdk_helper" XIGNCODE_SERVER_64_NAMEA XIGNCODE_SERVER_EXT_NAMEA
#define XIGNCODE_SERVER_NAMEW	L"zwave_sdk_helper" XIGNCODE_SERVER_64_NAMEW XIGNCODE_SERVER_EXT_NAMEW

#define XIGNCODE_DIR_NAMEA X_DIR_MARKDA "xigncode"
#define XIGNCODE_DIR_NAMEW X_DIR_MARKDW L"xigncode"

#ifdef UNIX
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <dlfcn.h>
#include <unistd.h>

typedef void *HMODULE;
typedef int (XCALL *FARPROC)();

static
xulong XCALL GetModuleFileNameA(
		HMODULE hModule,
		xstr lpFilename,
		xulong	nSize
		);

static
xulong XCALL GetModuleFileNameW(
		HMODULE hModule,
		xwstr lpFilename,
		xulong	nSize
		);

static
HMODULE
LoadLibraryA( xcstr lpFileName );

static
HMODULE
LoadLibraryW( xcwstr lpFileName );

static
FARPROC
GetProcAddress( HMODULE hModule,
                 xcstr lpProcName );
#else

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#endif


#ifdef _UNICODE
#define CreateXigncodeServer	CreateXigncodeServerW
#define CreateXigncodeServer2	CreateXigncodeServer2W
#define LoadHelperDll			LoadHelperDllW
#define LoadHelperDll2			LoadHelperDll2W
#else
#define CreateXigncodeServer	CreateXigncodeServerA
#define CreateXigncodeServer2	CreateXigncodeServer2A
#define LoadHelperDll			LoadHelperDllA
#define LoadHelperDll2			LoadHelperDll2A
#endif

#ifdef _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES
#define xwcscat wcscat_s
#define xstrcat strcat_s
#define xwcscpy wcscpy_s
#define xstrcpy strcpy_s
#else
#define xwcscat wcscat
#define xstrcat strcat
#define xwcscpy wcscpy
#define xstrcpy strcpy
#endif

#ifndef Z_RETURN
#define Z_RETURN
enum Z_RETURN
{
	Z_RTN_ERROR = -1,			// Packet Error
	Z_RTN_NONE,					// Normal Reply
	Z_RTN_NONCLIENT,			// Detected Nonclient
	Z_RTN_BLACK_CODE,			// Detected hacktools
	Z_RTN_SUSPICIOUS,			// Detected suspicious
	Z_RTN_USERDEFINED,			// Detected userdefines
	Z_RTN_RESEND,				// Request full zce packet
	Z_RTN_TIMEOUT
};
#endif



typedef xbool (XCALL *XigncodeServerSendCallback)(xpvoid uid, xpvoid meta, xpcch buf, xulong size);
typedef xvoid (XCALL *XigncodeServerCallbackW)(xpvoid uid, xpvoid meta, int code, xcwstr report);
typedef xvoid (XCALL *XigncodeServerCallbackA)(xpvoid uid, xpvoid meta, int code, xcstr report);

typedef xbool (XCALL *XigncodeServerSendCallback2)(xuint64 uid, xpvoid meta, xpcch buf, xulong size);
typedef xvoid (XCALL *XigncodeServerCallback2W)(xuint64 uid, xpvoid meta, int code, xcwstr report);
typedef xvoid (XCALL *XigncodeServerCallback2A)(xuint64 uid, xpvoid meta, int code, xcstr report);

class IXigncodeServer
{
public:
	/// Primitive Functions
	virtual xbool OnBegin(xulong blocksize = 512) = 0;
	virtual xbool OnEnd() = 0;
	virtual xbool OnAccept(xpvoid uid, xpvoid meta) = 0;
	virtual xbool OnDisconnect(xpvoid uid) = 0;
	virtual xbool OnReceive(xpvoid uid, xpcch buf, xulong size) = 0;
	virtual xbool Release() = 0; 

public:

	virtual xbool SetUserInformationA(xpvoid uid, xulong inetaddr, xcstr addinfo = NULL) = 0;
	virtual xbool SetUserInformationW(xpvoid uid, xulong inetaddr, xcwstr addinfo = NULL) = 0;

};

class IXigncodeServer2
{
public:
	/// Primitive Functions
	virtual xbool OnBegin(xulong blocksize = 512) = 0;
	virtual xbool OnEnd() = 0;
	virtual xbool OnAccept(xuint64 uid, xpvoid meta) = 0;
	virtual xbool OnDisconnect(xuint64 uid) = 0;
	virtual xbool OnReceive(xuint64 uid, xpcch buf, xulong size) = 0;
	virtual xbool Release() = 0; 

public:

	virtual xbool SetUserInformationA(xuint64 uid, xulong inetaddr, xcstr addinfo = NULL) = 0;
	virtual xbool SetUserInformationW(xuint64 uid, xulong inetaddr, xcwstr addinfo = NULL) = 0;

};

typedef xbool (XCALL* CreateXigncodeServerW)
(
	IXigncodeServer** _interface
	, XigncodeServerSendCallback _pFnSend
	, XigncodeServerCallbackW _pFnCallback
);

typedef xbool (XCALL* CreateXigncodeServerA)
(
	IXigncodeServer** _interface
	, XigncodeServerSendCallback _pFnSend
	, XigncodeServerCallbackA _pFnCallback
 );


typedef xbool (XCALL* CreateXigncodeServer2W)
(
	IXigncodeServer2** _interface
	, XigncodeServerSendCallback2 _pFnSend
	, XigncodeServerCallback2W _pFnCallback
);

typedef xbool (XCALL* CreateXigncodeServer2A)
(
	IXigncodeServer2** _interface
	, XigncodeServerSendCallback2 _pFnSend
	, XigncodeServerCallback2A _pFnCallback
 );

inline CreateXigncodeServerW LoadHelperDllW(xcwstr pszPath)
{
	xwch szT[X_MAX_PATH];
	xwch Base[X_MAX_PATH];

	if ( pszPath == NULL )
	{
		GetModuleFileNameW(NULL, szT, X_MAX_PATH);
		wchar_t* _p = wcsrchr(szT, X_DIR_MARKW);
		if ( _p )
			*_p = 0x00;
		xwcscat(szT, XIGNCODE_DIR_NAMEW);
	}
	else
	{
		xwcscpy(szT, pszPath);
	}

	xwcscpy(Base, szT);
	
	if ( szT[wcslen(szT)-1] != X_DIR_MARKW ) 
	{
		xwcscat(szT, X_DIR_MARKDW);
	}
	
	xwcscat(szT, XIGNCODE_SERVER_NAMEW);

	
	HMODULE h = LoadLibraryW(szT);
	if ( h )
	{
		void (XCALL *fnPPW)(xcwstr) = NULL;
		fnPPW =  (void (XCALL *)(xcwstr))GetProcAddress(h, "PushPathW");
		if ( fnPPW ) fnPPW(Base);
		CreateXigncodeServerW f = (CreateXigncodeServerW) GetProcAddress(h, "ICreateXigncodeServerW");
		if ( f ) return f;
	}
	return NULL;
}


inline CreateXigncodeServerA LoadHelperDllA(xcstr pszPath)
{
	xch szT[X_MAX_PATH];
	xch Base[X_MAX_PATH];

	if ( pszPath == NULL )
	{
		GetModuleFileNameA(NULL, szT, X_MAX_PATH);
		char* _p = strrchr(szT, X_DIR_MARKSA);
		if ( _p )
			*_p = 0x00;
		xstrcat(szT, XIGNCODE_DIR_NAMEA);
	}
	else
	{
		xstrcpy(szT, pszPath);
	}

	xstrcpy(Base, szT);

	if ( szT[strlen(szT)-1] != X_DIR_MARKSA )
	{
		xstrcat(szT, X_DIR_MARKDA);
	}
	
	xstrcat(szT, XIGNCODE_SERVER_NAMEA);


	HMODULE h = LoadLibraryA(szT);
	if ( h )
	{
		void (XCALL *fnPPA)(xcstr) = NULL;
		fnPPA = (void (XCALL *)(xcstr))GetProcAddress(h, "PushPathA");
		if ( fnPPA ) fnPPA(Base);
		CreateXigncodeServerA f = (CreateXigncodeServerA)GetProcAddress(h, "ICreateXigncodeServerA");
		if ( f ) return f;
	}
	return NULL;
}


inline CreateXigncodeServer2W LoadHelperDll2W(xcwstr pszPath)
{
	xwch szT[X_MAX_PATH];
	xwch Base[X_MAX_PATH];

	if ( pszPath == NULL )
	{
		GetModuleFileNameW(NULL, szT, X_MAX_PATH);
		wchar_t* _p = wcsrchr(szT, X_DIR_MARKSW);
		if ( _p )
			*_p = 0x00;
		xwcscat(szT, XIGNCODE_DIR_NAMEW);
	}
	else
	{
		xwcscpy(szT, pszPath);
	}

	xwcscpy(Base, szT);
	
	if ( szT[wcslen(szT)-1] != X_DIR_MARKSW ) 
	{
		xwcscat(szT, X_DIR_MARKDW);
	}
	
	xwcscat(szT, XIGNCODE_SERVER_NAMEW);
	
	HMODULE h = LoadLibraryW(szT);
	if ( h )
	{
		void (XCALL *fnPPW)(xcwstr) = NULL;
		fnPPW =  (void (XCALL *)(xcwstr))GetProcAddress(h, "PushPathW");
		if ( fnPPW ) fnPPW(Base);
		CreateXigncodeServer2W f = (CreateXigncodeServer2W) GetProcAddress(h, "ICreateXigncodeServer2W");
		if ( f ) return f;
	}
	return NULL;
}


inline CreateXigncodeServer2A LoadHelperDll2A(xcstr pszPath)
{
	char szT[X_MAX_PATH];
	char Base[X_MAX_PATH];

	if ( pszPath == NULL )
	{
		GetModuleFileNameA(NULL, szT, X_MAX_PATH);
		char* _p = strrchr(szT, X_DIR_MARKA);
		if ( _p )
			*_p = 0x00;
		xstrcat(szT, XIGNCODE_DIR_NAMEA);
	}
	else
	{
		xstrcpy(szT, pszPath);
	}

	xstrcpy(Base, szT);

	if ( szT[strlen(szT)-1] != X_DIR_MARKA )
	{
		xstrcat(szT, X_DIR_MARKDA);
	}
	
	xstrcat(szT, XIGNCODE_SERVER_NAMEA);

	HMODULE h = LoadLibraryA(szT);
	if ( h )
	{
		void (XCALL *fnPPA)(xcstr) = NULL;
		fnPPA = (void (XCALL *)(xcstr))GetProcAddress(h, "PushPathA");
		if ( fnPPA ) fnPPA(Base);
		CreateXigncodeServer2A f = (CreateXigncodeServer2A)GetProcAddress(h, "ICreateXigncodeServer2A");
		if ( f ) return f;
	}
	return NULL;
}


#ifdef UNIX

static
HMODULE LoadLibraryW( xcwstr lpFileName )
{ 
	void* handle;
	char fileName[X_MAX_PATH];
	wcstombs(fileName,lpFileName,sizeof(fileName));
	handle = dlopen(fileName, RTLD_LAZY);
	if(!handle)
	{
		return	NULL; 
	}
	
	return (HMODULE)handle; 
}

static
HMODULE LoadLibraryA( xcstr lpFileName )
{ 
	void *handle = dlopen(lpFileName, RTLD_LAZY);
	if(!handle)
	{
		return	NULL; 
	}
	
	return (HMODULE)handle;
}

static
FARPROC GetProcAddress( HMODULE hModule, xcstr lpProcName )
{ 
		if( hModule )	
		{
			FARPROC ret = (FARPROC)dlsym(hModule, lpProcName);
			if( ret == NULL)
			{	
				//DWPRINTF((L"GetProcAddress : dlsym error message : %s\n",dlerror())); 
			}
	    		return (FARPROC)ret; 
		}
		return NULL;
} 

static
xulong  
XCALL 
GetModuleFileNameW(
		HMODULE hModule,
		xwstr lpFilename,
		xulong	nSize
		)
{
	xwch filename[X_MAX_PATH] = {0 , };
	xch   arBuf[X_MAX_PATH] = {0, };
	Dl_info dlip;
	xulong ret;

	if( hModule == NULL )
	{
		if( readlink("/proc/self/exe", arBuf, X_MAX_PATH) == -1 )
		{
			return 0;
		}

		ret = mbstowcs( filename, arBuf, X_MAX_PATH);
	}
	else
	{
		if( dladdr( hModule,  &dlip) == 0 )
		{
			return 0;
		}


		ret =(xulong) mbstowcs( filename, dlip.dli_fname , X_MAX_PATH );

	}

	if(ret == (size_t)-1)
	{
		return 0;
	}
	wcscpy(lpFilename, filename);

	return ret;	
}

static
xulong 
XCALL 
GetModuleFileNameA(
		HMODULE hModule,
		xstr lpFilename,
		xulong	nSize
		)
{
	xch   arBuf[X_MAX_PATH] = {0, };
	Dl_info dlip;
	
	if( hModule == NULL )
	{
		if( readlink("/proc/self/exe", arBuf, X_MAX_PATH) == -1 )
		{
			return 0;
		}

		strcpy(lpFilename, arBuf);

	}
	else
	{
		if( dladdr( hModule,  &dlip) == 0 )
		{
			return 0;
		}
		strcpy(lpFilename, dlip.dli_fname);
	}

	return strlen(lpFilename);	
}
#endif    // UNIX


#endif

