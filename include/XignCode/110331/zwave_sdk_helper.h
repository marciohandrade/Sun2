#pragma once

#ifdef ZWAVE_SDK_HELPER_EXPORTS
#define ZWAVE_SDK_HELPER_API __declspec(dllexport)
#else
#define ZWAVE_SDK_HELPER_API __declspec(dllimport)
#endif

#include "xtypes.h"

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef _UNICODE
#define CreateXigncodeServer	CreateXigncodeServerW
#define LoadHelperDll			LoadHelperDllW
#else
#define CreateXigncodeServer	CreateXigncodeServerA
#define LoadHelperDll			LoadHelperDllA
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

#ifndef SOCKET
typedef UINT_PTR SOCKET;
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


typedef xbool (XCALL *XigncodeServerSendCallback)(SOCKET s, xpvoid meta, xpcch buf, xulong size);
typedef xvoid (XCALL *XigncodeServerCallbackW)(SOCKET s, xpvoid meta, int code, xcwstr report);
typedef xvoid (XCALL *XigncodeServerCallbackA)(SOCKET s, xpvoid meta, int code, xcstr report);

class IXigncodeServer
{
public:
	/// Primitive Functions
	virtual xbool OnBegin(xulong blocksize = 512) = 0;
	virtual xbool OnEnd() = 0;
	virtual xbool OnAccept(SOCKET s, xpvoid meta) = 0;
	virtual xbool OnDisconnect(SOCKET s) = 0;
	virtual xbool OnRecieve(SOCKET s, xpcch buf, xulong size) = 0;
	virtual xbool Release() = 0;

public:

	virtual xbool SetUserInformationA(SOCKET s, xulong inetaddr, xcstr addinfo = NULL) = 0;
	virtual xbool SetUserInformationW(SOCKET s, xulong inetaddr, xcwstr addinfo = NULL) = 0;

};


//************************************
// Prototype of CreateXigncodeServer method
// FullName:  CreateXigncodeServer
// Access:    public 
// Returns:   BOOL
// Qualifier: 
// Parameter:	IXigncodeServer** _interface , 
//				XigncodeServerSendCallback _pFnSend , 
//				XigncodeServerCallback _pFnCallback , 
//				DWORD qtZCE , 
//				DWORD qtNCB , 
//				DWORD qtTimeout
//************************************
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



//************************************
// Method:    LoadHelperDllW
// FullName:  LoadHelperDllW
// Access:    public 
// Returns:   CreateXigncodeServer
// Qualifier:
// Parameter: wchar_t * pszPath
//			  if xigncode library path locates sub-directroy of main exe, set NULL
//				ex) gameserver.exe			c:\game\
//				ex) zwave_sdk_server.dll	c:\game\xigncode
//			  if xigncode library path locates not sub-directory of main exe, set absolute path of xigncode library
//			  
//************************************
inline CreateXigncodeServerW LoadHelperDllW(xcwstr pszPath)
{
	wchar_t szT[MAX_PATH];
	wchar_t Base[MAX_PATH];

	if ( pszPath == NULL )
	{
		GetModuleFileNameW(NULL, szT, MAX_PATH);
		wchar_t* _p = wcsrchr(szT, '\\');
		if ( _p )
			*_p = 0x00;
		xwcscat(szT,  L"\\xigncode");
	}
	else
	{
		xwcscpy(szT, pszPath);
	}

	xwcscpy(Base, szT);
	
	if ( szT[wcslen(szT)-1] != '\\' ) 
	{
		xwcscat(szT, L"\\");
	}

#ifdef _M_X64
	xwcscat(szT, L"zwave_sdk_helper_x64.dll");
#else
	xwcscat(szT, L"zwave_sdk_helper.dll");
#endif
	
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


//************************************
// Method:    LoadHelperDllA
// FullName:  LoadHelperDllA
// Access:    public 
// Returns:   CreateXigncodeServer
// Qualifier:
// Parameter: char * pszPath
//			  if xigncode library path locates sub-directroy of main exe, set NULL
//				ex) gameserver.exe			c:\game\
//				ex) zwave_sdk_server.dll	c:\game\xigncode
//			  if xigncode library path locates not sub-directory of main exe, set absolute path of xigncode library
//************************************
inline CreateXigncodeServerA LoadHelperDllA(xcstr pszPath)
{
	char szT[MAX_PATH];
	char Base[MAX_PATH];

	if ( pszPath == NULL )
	{
		GetModuleFileNameA(NULL, szT, MAX_PATH);
		char* _p = strrchr(szT, '\\');
		if ( _p )
			*_p = 0x00;
		xstrcat(szT, "\\xigncode");
	}
	else
	{
		xstrcpy(szT, pszPath);
	}

	xstrcpy(Base, szT);

	if ( szT[strlen(szT)-1] != '\\' )
	{
		xstrcat(szT, "\\");
	}

#ifdef _M_X64 
	xstrcat(szT, "zwave_sdk_helper_x64.dll");
#else
	xstrcat(szT, "zwave_sdk_helper.dll");
#endif

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