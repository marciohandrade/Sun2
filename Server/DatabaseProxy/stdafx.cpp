#include "stdafx.h"

#if !defined(DBPROXY_TYPE_SELECTOR)
    #error "must be selected DBPROXY_TYPE_SELECTOR"
#endif

CSunLog*
g_pSunConnLog = NULL;

BOOL
MessageDBConnOut( BYTE byLogLevel, TCHAR* pszMsg, ... )
{
	BOOL bRet = FALSE;
	va_list args; 
	va_start(args, pszMsg); 

	if( g_pSunConnLog )
	{
		bRet = CSunLog::LOGGINGwithARG(
			g_pSunConnLog,
			CSunLog::cOUTPUT_FILE,
			(CSunLog::eLOGLEVEL)byLogLevel,
			CSunLog::cCATE_MASK,
			pszMsg,
			args
			);
	}
	else
	{
		TCHAR pMSG[1024];
		bRet = (BOOL)!( 0 > _vsntprintf( pMSG, 1024, pszMsg, args ) );
	}
	va_end(args);

	return bRet;
}
