#ifndef __MAIN_H__
#define __MAIN_H__

BOOL InitSolarConsole();
VOID ReleaseSolarConsole();
BOOL GetCommandLineArg( int idx, char *szCommandLine, char *pszTargetBuf, int nMaxBufSize );
BOOL ParseArgumentsForGameServer( char *lpCmdLine );
VOID OnClose();
BOOL ParsingGameServerType( LPSTR lpCmdLine );

#endif	//__MAIN_H__