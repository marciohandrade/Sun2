#include "stdafx.h"

WorldLog*		g_pLOG = NULL;
CSunLog*		g_pCountLog = NULL;


BOOL	MessageCount( BYTE byLogLevel, TCHAR* pszMsg, ... )
{
	BOOL bRet = FALSE;
	va_list args; 
	va_start(args, pszMsg); 

	if( g_pCountLog )
	{
		bRet = CSunLog::LOGGINGwithARG(
								g_pCountLog,
								CSunLog::cOUTPUT_BOTH,
								(CSunLog::eLOGLEVEL)byLogLevel,
								CSunLog::cCATE_CONNECTION,
								pszMsg,
								args
								);
	}
	else
	{
		TCHAR pMSG[1024];
		_tcsncpy( pMSG, SUNLOG_ENUM_TO_STRING(cCATE_CONNECTION), 100 );
		INT iWritten = (INT)_tcslen( pMSG );
		bRet = (BOOL)!( 0 > _vsntprintf( pMSG+iWritten, 1024-iWritten, pszMsg, args ) );
	}
	va_end(args);

	return bRet;
}


