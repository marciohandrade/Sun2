
#pragma once

#include <Const.h>
#include <ServerConst.h>
#include <winsock2.h>


TCHAR * GetServerType2String( eSERVER_TYPE eType );

VOID DisplayDebugInfo( eSERVER_TYPE ServerType, int iMode, BOOL bDebug, DWORD HighVerNo, DWORD MiddleVerNo, DWORD LowVerNo, DWORD BuildVer, WORD WorldNum, WORD ChannelNum, WORD ServerID = 1 );
BOOL GetCommandLineArg( int idx, char *szCommandLine, char *pszTargetBuf, int nMaxBufSize );
BOOL ParseArguments( char *lpCmdLine );
VOID SetApplicationDirectory();

struct IPOperation
{
	static DWORD			ConvertStrIP2DWORD( const CHAR* pszIP )
	{
		if( pszIP && ( (*(DWORD*)"NULL") == (*(DWORD*)pszIP) ) )
			return 0;

		return (DWORD)inet_addr( pszIP );
	}

	static CHAR*			ConvertDWORD2StrIP( const DWORD dwIP )
	{
		in_addr addr;
		addr.s_addr = dwIP;

		return (CHAR*)inet_ntoa( addr );
	}
};
