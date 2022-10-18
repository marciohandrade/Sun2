#include "stdafx.h"
#include <Global.h>
#include "misc.h"

#include ".\ServerGlobal.h"


#define SERVER_STRING(serverType)	#serverType

static TCHAR*	s_ServerNames[MAX_SERVER] =
{
	SERVER_STRING(UNKNOWN_SERVER),
	SERVER_STRING(AGENT_SERVER),
	SERVER_STRING(FIELD_SERVER),
	SERVER_STRING(BATTLE_SERVER),
	SERVER_STRING(GAME_DBPROXY),
	SERVER_STRING(WORLD_SERVER),
	SERVER_STRING(MASTER_SERVER),
	SERVER_STRING(GUILD_SERVER),
	SERVER_STRING(ACCOUNT_DBPROXY),
	SERVER_STRING(TOURNAMENT_SERVER),
	SERVER_STRING(AUTH_AGENT_SERVER),
	SERVER_STRING(PORTAL_SERVER),
	SERVER_STRING(SOLARAUTH_SERVER),
	SERVER_STRING(SHOP_SERVER),
	NULL,	//14
	NULL,	//15
	NULL,	//16
	NULL,	//17
	NULL,	//18
	NULL,	//19
	NULL,	//20
	NULL,	//21
	NULL,	//22
	NULL,	//23
	NULL,	//24
	SERVER_STRING(OP_SERVER),
	SERVER_STRING(TEMP_SERVER),
	SERVER_STRING(GAME_SERVER),
	SERVER_STRING(AUTH_SERVER),
	SERVER_STRING(LOGIN_FRONT_SERVER),
	SERVER_STRING(BILLING_SERVER),
	SERVER_STRING(CLIENT_SERVER),
};

//--------------------------------------------------------------------------------
// 서버타입을 문자열로 반환하는 함수
//--------------------------------------------------------------------------------
TCHAR * GetServerType2String( eSERVER_TYPE eType )
{
	if( UNKNOWN_SERVER < eType && eType < MAX_SERVER )
		return s_ServerNames[eType];
	else
		return s_ServerNames[UNKNOWN_SERVER];
}


VOID DisplayDebugInfo( eSERVER_TYPE ServerType, int iMode, BOOL bDebug, DWORD HighVerNo, DWORD MiddleVerNo, DWORD LowVerNo, DWORD BuildVer, WORD WorldNum, WORD ChannelNum, WORD ServerID )
{
	const int TITLE_LEN = 256;
	const int MODE_LEN = 10;
	TCHAR pszTitle[TITLE_LEN+1];
	TCHAR tszDebug[MODE_LEN+1] = "RELEASE";
	TCHAR tszMode[MODE_LEN+1] = "UNKNOWN";

	switch(iMode)
	{
	case 1:
		_tcsncpy( tszMode, "DEV", MODE_LEN );
		break;
	case 2:
		_tcsncpy( tszMode, "INTRAOFFICE", MODE_LEN );
		break;
	case 3:
		_tcsncpy( tszMode, "LIVE", MODE_LEN );
		break;
	case 4:
		_tcsncpy( tszMode, "TESTLIVE", MODE_LEN );
		break;
	}
	if( bDebug ) _tcsncpy( tszDebug, "DEBUG", MODE_LEN );

	tszDebug[MODE_LEN] = '\0';
	tszMode[MODE_LEN] = '\0';

	_sntprintf( pszTitle, TITLE_LEN, "%s-%s-%s-(Ver:%u-%u-%u),Build:(%u),World:%u,Channel:%u,ServerID:%u", 
		GetServerType2String(ServerType), tszMode, tszDebug, 
		HighVerNo, MiddleVerNo, LowVerNo, 
		BuildVer, WorldNum, ChannelNum, ServerID );
	pszTitle[TITLE_LEN] = '\0';

	//if( NULL == hWnd ) { hWnd = GetConsoleWindow(); if( NULL == hWnd ) { hWnd = GetActiveWindow(); if( NULL == hWnd ) { hWnd = GetForegroundWindow(); } } }
	//SetWindowText( hWnd, pszTitle );
	SetConsoleTitle(pszTitle);
}

BOOL GetCommandLineArg( int idx, char *szCommandLine, char *pszTargetBuf, int nMaxBufSize )
{
	char sep[] = " ";
	char line[255];

	strncpy( line, szCommandLine, 255 );

	char *token = strtok( line, sep );

	while( token && idx )
	{
		token = strtok( NULL, sep );
		--idx;
	}

	if( token != NULL )
		strncpy( pszTargetBuf, token, nMaxBufSize );
	else
	{
		pszTargetBuf[0] = NULL;
		return FALSE;
	}

	return TRUE;
}

VOID SetApplicationDirectory()
{
	TCHAR szPath[MAX_PATH], szDir[MAX_PATH];
	GetModuleFileName( NULL, szPath, MAX_PATH );
	char* pBackSlash = strrchr( szPath, '\\');
	int nBackSlash = pBackSlash - szPath;
	strncpy( szDir, szPath, MAX_PATH );
	szDir[nBackSlash] = '\0';
	SetCurrentDirectory( szDir );
	DISPMSG( "Current Work Directory = %s\n", szDir );
}

BOOL ParseArguments( char *lpCmdLine )
{
	char szPath[255];
	GetCommandLineArg( 0, lpCmdLine, szPath, 255 );
    // (CHANGES) (100115) (WAVERIX) changes code block
    szPath[_countof(szPath) - 1] = '\0';

	// 작업디렉토리 설정
    if (szPath[0] == '\0') {
        SetApplicationDirectory();
    }
    else {
        SetCurrentDirectory(szPath);
    }

	return TRUE;
}


















