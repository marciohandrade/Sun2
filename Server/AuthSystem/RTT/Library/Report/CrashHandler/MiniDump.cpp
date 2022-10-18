#include <Windows.h>
#include <dbgHelp.h>

#include "MiniDump.h"

typedef BOOL ( WINAPI *MINIDUMPWRITEDUMP )(
	HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam );

bool MiniDump::GenerateMiniDump( PEXCEPTION_POINTERS exceptionInfo, LPCTSTR dumpFileName )
{
	MINIDUMPWRITEDUMP	writeDump;
	HWND				hParent	= 0;
	HMODULE				hDll	= 0;
	char				szDbgHelpPath[_MAX_PATH];

	if( GetModuleFileName( 0, szDbgHelpPath, _MAX_PATH ) )
	{
		char* pSlash = strrchr( szDbgHelpPath, '\\' );

		if( pSlash )
		{
			strcpy( pSlash+1, "DBGHELP.DLL" );
			hDll = ::LoadLibrary( szDbgHelpPath );
		}
	}

	if( hDll == 0 )
		hDll = ::LoadLibrary( "DBGHELP.DLL" );

	if( hDll == 0 )
		return false;

	writeDump = ( MINIDUMPWRITEDUMP )::GetProcAddress( hDll, "MiniDumpWriteDump" );

	if( writeDump != 0 )
	{
		HANDLE	hFile;

		hFile = ::CreateFile( dumpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, 0, CREATE_ALWAYS,
									FILE_ATTRIBUTE_NORMAL, 0 );
		if( hFile != INVALID_HANDLE_VALUE )
		{
			_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

			ExInfo.ThreadId				= ::GetCurrentThreadId();
			ExInfo.ExceptionPointers	= exceptionInfo;
			ExInfo.ClientPointers		= 0;

			//  Write the dump
			if( exceptionInfo == 0  )
				writeDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, 0, 0, 0 );
			else
				writeDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, 0, 0 );

			::CloseHandle( hFile );
		}
	}

	return true;
}
