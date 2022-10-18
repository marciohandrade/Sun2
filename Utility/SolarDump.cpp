#include "UtilityCommon.h"
#include ".\solardump.h"
#include <time.h>
#include <new>
#include <new.h>
#include "SolarAssert.h"

typedef BOOL (WINAPI * fnMINIDUMPWRITEDUMP)(
    HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
    CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
    CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
    CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam
    );

MINIDUMP_TYPE	SolarDump::s_eDumpType			= MiniDumpNormal;
TCHAR			SolarDump::s_pszDumpFileDir[]	= {0,};
TCHAR			SolarDump::s_pszFilePrefix[]	= {0,};
TCHAR			SolarDump::s_pszBuildVersion[]  = _T("0000");
int SolarDump::old_new_mode_ = 0;

SolarDump::SolarDump( char * pszDir, MINIDUMP_TYPE eDumpType )
{	
	//ASSERT( !IsBadReadPtr( pszDir, sizeof(pszDir) ) );

	lstrcpyn( s_pszDumpFileDir, pszDir, MAX_PATH );

	TCHAR szFilename[MAX_PATH];
    GetModuleFileName( NULL, szFilename, MAX_PATH );

	TCHAR * slash = strrchr( szFilename, '\\' );
    TCHAR * dot = strrchr( slash+1, '.' );
    ::lstrcpyn( s_pszFilePrefix, slash+1, (int)(dot - slash) );
	
	s_eDumpType = eDumpType;
	
    old_new_mode_ = ::_set_new_mode(1);
    ::_set_new_handler(NewHandler);
    ::_set_purecall_handler(PureCallHandler);
#if _MSC_VER == 1310
    // vs2003 전용
    ::_set_security_error_handler(BufferOverflowHandler);
#endif // 1310
    ::SetUnhandledExceptionFilter( ExceptionFilter );
}

SolarDump::~SolarDump()
{
}

void
SolarDump::SetBuildVersion(const char *pszBuildVersion)
{
	lstrcpyn( s_pszBuildVersion, pszBuildVersion, MAX_PATH );
}

LONG WINAPI SolarDump::ExceptionFilter( struct _EXCEPTION_POINTERS * pExceptionInfo )
{
	LONG rt = EXCEPTION_CONTINUE_SEARCH;

	if( pExceptionInfo == NULL )
	{
		static BOOL first = FALSE;
		if( first ) return rt;
		first = TRUE;

		__try 
		{
			RaiseException( EXCEPTION_BREAKPOINT, 0, 0, NULL );
		} 
		__except( ExceptionFilter( GetExceptionInformation() ), EXCEPTION_CONTINUE_EXECUTION ) 
		{
		}
		return rt;
	}
	// 현재 경로에서 dll로드
    const TCHAR* dbghelp_name = _T("dbghelp.dll");
	HANDLE hDLL = ::LoadLibrary(dbghelp_name);
    /*
    // (NOTE) reserved code
    // the dll library search order of a 'LoadLibrary' has a loaded image path first.
    if (hDLL == NULL)
    {
        TCHAR full_path[MAX_PATH];
        TCHAR exe_path[MAX_PATH];
        GetCurrentDirectory(_countof(exe_path), exe_path);
        exe_path[_countof(exe_path) - 1] = _T('\0');
        _sntprintf(full_path, _countof(full_path), _T("%s\\%s"), exe_path, dbghelp_name);
        full_path[_countof(full_path) - 1] = _T('\0');
        hDLL = ::LoadLibrary(full_path);
    }*/
	if( NULL == hDLL )
    {
		SASSERT(NULL, "DBGHELP.DLL을 로드할 수 없습니다." );
        return rt;
    }

	fnMINIDUMPWRITEDUMP pfnMiniDumpWriteDump = (fnMINIDUMPWRITEDUMP)GetProcAddress( (HMODULE)hDLL, "MiniDumpWriteDump" );
	if( NULL == pfnMiniDumpWriteDump )
    {
		SASSERT(NULL, "오래된 버전의 DBGHELP.DLL입니다. 최신버전으로 업데이트 하시기 바랍니다." );
        return rt;
    }

	struct tm * today;
	time_t		ltime;
	time( &ltime );
	today = localtime( &ltime );

	char szDumpPath[MAX_PATH*2];
	wsprintf( szDumpPath, 
		"%s\\%s%02d-%02d-%02d=%02dh%02dm%02ds_%s.dmp", 
		s_pszDumpFileDir, 
		s_pszFilePrefix, 
		today->tm_year+1900, 
		today->tm_mon+1, 
		today->tm_mday, 
		today->tm_hour, 
		today->tm_min, 
		today->tm_sec,
		s_pszBuildVersion);

    HANDLE hFile = CreateFile( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if( INVALID_HANDLE_VALUE == hFile )
    {
		SASSERT(NULL, VSTR( "[%s]파일을 열 수 없습니다.", szDumpPath ) );
        return rt;
    }

	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId				= GetCurrentThreadId();
	eInfo.ExceptionPointers		= pExceptionInfo;
	eInfo.ClientPointers		= FALSE;

	BOOL bResult = pfnMiniDumpWriteDump( GetCurrentProcess(), 
		GetCurrentProcessId(), 
		hFile, 
		s_eDumpType, 
		(pExceptionInfo?&eInfo:NULL), 
		NULL, NULL);

    if( !bResult )
    {
		SASSERT(NULL, "MiniDumpWriteDump() 호출이 실패했습니다." );
		CloseHandle(hFile);
		return ( rt = EXCEPTION_EXECUTE_HANDLER );
    }

    CloseHandle( hFile );

	return rt;
}

int SolarDump::NewHandler(size_t)
{
    SASSERT(NULL, "Memory allocate is fail.");
    int* break_point = NULL;
    *break_point = 200;
    return 0;
}

void SolarDump::PureCallHandler()
{
    SASSERT(NULL, "Pure function is called.");
    int* break_point = NULL;
    *break_point = 100;
}

void SolarDump::BufferOverflowHandler(int code, void* unused)
{
    SASSERT(NULL, "Buffer is overflow.");
    int* break_point = NULL;
    *break_point = 300;
}
