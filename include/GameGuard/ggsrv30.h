#ifndef _GGSRV30_H_
#define _GGSRV30_H_

// if Linux
#if !defined(_MSC_VER)
    #include <stdint.h>

	typedef int32_t			INT32,	*PINT32;
	typedef uint32_t		UINT32, *PUINT32;
	typedef int64_t			INT64,	*PINT64;
	typedef uint64_t		UINT64, *PUINT64;

	typedef intptr_t		INT_PTR;
	typedef uintptr_t		UINT_PTR;

	typedef unsigned int	DWORD, *PDWORD;
	typedef unsigned short	WORD,	*PWORD;	
	typedef int				BOOL;
	
	typedef const char*		LPCSTR;
	typedef char*			LPSTR;
	typedef unsigned char	BYTE,	*PBYTE, *LPBYTE, *PUCHAR;

	typedef void*			PVOID;
	typedef void*			LPVOID;	
	typedef void*			HMODULE;
	typedef void*			HANDLE;

	#define TRUE  1
	#define FALSE 0

	#define IN
	#define OUT

	#define _STDCALL
	#define ERROR_SUCCESS	0
	
#else
	#define _STDCALL	__stdcall

#endif

// default is c++.
// if not c++, Choose one of these or gcc -D_HEADER_xxx
//#define _HEADER_C
//#define _HEADER_DLL

#if !defined(_HEADER_C) && !defined(_HEADER_DLL)
#define _HEADER_CPLUSPLUS
#endif

#ifdef _HEADER_C
	#define EXPORT_API
	#define EXPORT_CLASS
	#define CALLTYPE
#endif 

#ifdef _HEADER_CPLUSPLUS
	#define EXPORT_API		extern "C"
	#define EXPORT_CLASS
	#define CALLTYPE
#endif 

#ifdef _HEADER_DLL
	#define EXPORT_API		extern "C" __declspec(dllexport)
	#define EXPORT_CLASS	__declspec(dllexport)
	#define CALLTYPE		__cdecl
#endif

// Callback Functions 
enum LogType
{
	LOG_UPDATE = 0,
	LOG_NORMAL = 1,
	LOG_DEBUG = 2
};

enum UpdateType
{
	UPDATE_PROTOCOL = 0,
	UPDATE_GGVERSION = 1
};

enum CallbackType
{
	CALLBACK_LOG = 0,
	CALLBACK_UPDATE = 1
};

enum UserInformationType
{
	INFO_ID = 0,
	INFO_IPADDR = 1, 
	INFO_EXINFO = 2
};

typedef void (_STDCALL *PCS3LogCallback)( int nLogType, char *szLog );
typedef void (_STDCALL *PCS3UpdateInfoCallback)( int UpdateType, int nBefore, int nAfter );

// export Functions 
EXPORT_API UINT32 CALLTYPE InitCSAuth3(char* szModuleDir);
EXPORT_API UINT32 CALLTYPE CloseCSAuth3();

EXPORT_API UINT32 CALLTYPE DecryptHackData(char* lpszUserKey, LPVOID lpData, DWORD dwLength);
EXPORT_API UINT32 CALLTYPE SetCallbackFunction( int nFunctionType, PVOID pFunction );

#ifndef _HEADER_C

// User class for C++ compiler
class EXPORT_CLASS CCSAuth3
{	
public:
	CCSAuth3();
	~CCSAuth3();

	UINT32 Init( BOOL bIsFirstServer );
	UINT32 Close();

	UINT32 Get( OUT UINT32 *puSize );
	UINT32 Check( IN UINT32 uSize );

	UINT32 SetUserInfo( IN UINT32 uUserType, IN char * szUserInfo );

	UINT32 uMagic;
	BYTE packet[4096];
	
private:
	BYTE data[4096];
};

#endif	// _HEADER_C

// User instance for C compiler
EXPORT_API PBYTE	CALLTYPE CSAuth3_CreateInstance();
EXPORT_API void		CALLTYPE CSAuth3_Release( PBYTE pbInstance );

EXPORT_API void		CSAuth3_SetInstance( PBYTE pBuffer4Set );
EXPORT_API UINT32	CSAuth3_GetInstanceSize();

EXPORT_API UINT32	CALLTYPE CSAuth3_Init( PBYTE pbInstance, BOOL bIsFirstServer );
EXPORT_API UINT32	CALLTYPE CSAuth3_Close( PBYTE pbInstance );

EXPORT_API UINT32	CALLTYPE CSAuth3_Get( PBYTE pbInstance, OUT UINT32 *puSize );
EXPORT_API UINT32	CALLTYPE CSAuth3_Check( PBYTE pbInstance, IN PBYTE pbPacket, IN UINT32 uSize );

EXPORT_API UINT32	CALLTYPE CSAuth3_SetUserInfo( PBYTE pbInstance, IN UINT32 uUserType, IN char * szUserInfo );

EXPORT_API PBYTE	CALLTYPE CSAuth3_GetPacket( PBYTE pbInstance );

#endif	// #ifndef _GGSRV30_H_
