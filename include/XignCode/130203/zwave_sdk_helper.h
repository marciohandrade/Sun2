#ifndef ZWAVE_SDK_HELPER_H
#define ZWAVE_SDK_HELPER_H

#include "xtypes.h"
#include "zwave_sdk_errors.h"

/*#ifndef _CryptType
#define _CryptType
typedef enum _CryptType 
{ 
 Blowfish
 , Rijndael
 , Rc4 
} CryptType;
#endif*/ 


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

	virtual ~IXigncodeServer() {}

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

	virtual ~IXigncodeServer2() {}

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





typedef xbool (XCALL *ZSWAVE_RsaFreeBufferT)(xhandle H, xpuch Buffer);
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



inline
xbool
XCALL
_ZSWAVE_FetchFunction(xpvoid *fn, xcstr name)
{
#ifdef UNIX

	return X_FALSE;

#else
	if(*fn)
		return X_TRUE;

	HMODULE module = GetModuleHandleW(XIGNCODE_SERVER_NAMEW);
	if(!module)
	{
		SetLastError(ZWAVE_STATUS_NOT_INITIALIZED);
		return X_FALSE;
	}

	*fn = (ZSWAVE_RsaFreeBufferT) GetProcAddress(module, name);
	if(!*fn)
	{
		SetLastError(ZWAVE_STATUS_NOT_INITIALIZED);
		return X_FALSE;
	}

	return X_TRUE;
#endif
}

inline
xbool    
XCALL 
ZSWAVE_RsaFreeBuffer(xhandle H, xpuch Buffer)
{
	static ZSWAVE_RsaFreeBufferT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaFreeBuffer"))
		return X_FALSE;

	return pfn(H, Buffer);
}

inline
xhandle	
XCALL 
ZSWAVE_RsaCreate()
{
	static ZSWAVE_RsaCreateT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaCreate"))
		return X_FALSE;

	return pfn();
}

inline
xbool 
XCALL 
ZSWAVE_RsaClose(xhandle H)
{
	static ZSWAVE_RsaCloseT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaClose"))
		return X_FALSE;

	return pfn(H);
}

inline
xbool 
XCALL 
ZSWAVE_RsaPublicDecrypt(xhandle H
							, xpcuch Input
							, xsize InputSize
							, xpuch *Output
							, xsize *OutputSize)
{
	static ZSWAVE_RsaPublicDecryptT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaPublicDecrypt"))
		return X_FALSE;

	return pfn(H, Input, InputSize, Output, OutputSize);
}

inline
xbool 
XCALL 
ZSWAVE_RsaPublicEncrypt(xhandle H
							, xpcuch Input
							, xsize InputSize
							, xpuch *Output
							, xsize *OutputSize)
{
	static ZSWAVE_RsaPublicEncryptT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaPublicEncrypt"))
		return X_FALSE;

	return pfn(H, Input, InputSize, Output, OutputSize);
}

inline
xbool 
XCALL 
ZSWAVE_RsaPrivateDecrypt(xhandle H
							, xpcuch Input
							, xsize InputSize
							, xpuch *Output
							, xsize *OutputSize)
{
	static ZSWAVE_RsaPrivateDecryptT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaPrivateDecrypt"))
		return X_FALSE;

	return pfn(H, Input, InputSize, Output, OutputSize);
}

inline
xbool 
XCALL 
ZSWAVE_RsaPrivateEncrypt(xhandle H
							, xpcuch Input
							, xsize InputSize
							, xpuch *Output
							, xsize *OutputSize)
{
	static ZSWAVE_RsaPrivateEncryptT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaPrivateEncrypt"))
		return X_FALSE;

	return pfn(H, Input, InputSize, Output, OutputSize);
}

inline
xbool 
XCALL 
ZSWAVE_RsaSetPublicKey(xhandle H, xpcuch Key, xsize KeySize)
{
	static ZSWAVE_RsaSetPublicKeyT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaSetPublicKey"))
		return X_FALSE;

	return pfn(H, Key, KeySize);
}

inline
xbool 
XCALL 
ZSWAVE_RsaSetPrivateKey(xhandle H, xpcuch Key, xsize KeySize)
{
	static ZSWAVE_RsaSetPrivateKeyT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaSetPrivateKey"))
		return X_FALSE;

	return pfn(H, Key, KeySize);
}

inline
xbool 
XCALL 
ZSWAVE_RsaGenerateKey(xpuch PublicKey
							, xsize PublicKeySize
							, xpuch PrivateKey
							, xsize PrivateKeySize
							, int Nbits
							, int Exponent)
{
	static ZSWAVE_RsaGenerateKeyT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_RsaGenerateKey"))
		return X_FALSE;

	return pfn(PublicKey, PublicKeySize, PrivateKey, PrivateKeySize, Nbits, Exponent);
}

inline
xbool 
XCALL 
ZSWAVE_Decrypt(xpcuch Input
					, xsize InputSize
					, xpcuch Key
					, xsize KeySize
					, xpuch Output
					, xsize OutputSize
					, xsize *Required
					, CryptType Type)
{
	static ZSWAVE_DecryptT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_Decrypt"))
		return X_FALSE;

	return pfn(Input, InputSize, Key, KeySize, Output, OutputSize, Required, Type);

}

inline
xbool 
XCALL 
ZSWAVE_Encrypt(xpcuch Input
					, xsize InputSize
					, xpcuch Key
					, xsize KeySize
					, xpuch Output
					, xsize OutputSize
					, xsize *Required
					, CryptType Type)
{
	static ZSWAVE_EncryptT pfn = NULL;

	if(!_ZSWAVE_FetchFunction((xpvoid *) &pfn, "ZSWAVE_Encrypt"))
		return X_FALSE;
	
	return pfn(Input, InputSize, Key, KeySize, Output, OutputSize, Required, Type);
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

