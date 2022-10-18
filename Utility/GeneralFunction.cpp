#include "UtilityCommon.h"
#include "GeneralFunction.h"
#include <time.h>

BYTE g_byClientLogLevel = 0;

BOOL EnterLockApplication( HANDLE hLock, TCHAR * pszLockName )
{
	hLock = CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 1024, pszLockName);
	if(hLock != NULL && GetLastError() == ERROR_ALREADY_EXISTS) 
	{ 
		CloseHandle(hLock); 
		hLock = NULL;
		return FALSE;
	}

	return TRUE;
}

void LeaveLockApplication( HANDLE hLock )
{
	if(hLock)
	{
		CloseHandle(hLock);
		hLock = NULL;
	}
}

void SetSaveParserLog(BYTE byLogLevel)
{
	g_byClientLogLevel = byLogLevel;
}

BOOL WriteParserErrLog(BYTE byLogLevel, TCHAR* pszMsg, ... )
{
	if(g_byClientLogLevel == 0)
		return FALSE;

	if(g_byClientLogLevel >= byLogLevel)
	{
		char	szErrLog[1024] = {0,};

		va_list args; 
		va_start(args, pszMsg); 
		_vsntprintf( szErrLog, 1023, pszMsg, args ); 
		va_end(args); 

		char szFileName[MAX_PATH] = {0,};
		_snprintf(szFileName, MAX_PATH-1, _T("log\\ParserErr.log"));

		FILE * fp = _tfopen(szFileName, _T("a+t"));

		char	szOutputMsg[1024] = {0,};
		_snprintf(szOutputMsg, 1023, _T("[%d] %s\n"), byLogLevel, szErrLog);
		fwrite( szOutputMsg, _tcslen(szOutputMsg), 1, fp );
		fclose( fp );
	}

	return TRUE;
}

