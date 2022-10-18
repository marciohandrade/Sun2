// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// AgentServer.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.
AgentLog*		g_pLOG = NULL;
CSunLog*		g_pSunConnLog = NULL;
CSunLog* g_pSecureModuleLog = NULL; // USING_XIGNCODE ...

BOOL	MessageConnOut( BYTE byLogLevel, TCHAR* pszMsg, ... )
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

bool MessageSecureModuleOut(eSUN_LOG_LEVEL log_level, const TCHAR* meta_format, ...)
{
    bool result = false;
    va_list args;
    va_start(args, meta_format);

    if (g_pSecureModuleLog)
    {
        result = CSunLog::LOGGINGwithARG(
            g_pSecureModuleLog,
            CSunLog::cOUTPUT_FILE,
            static_cast<CSunLog::eLOGLEVEL>(log_level),
            CSunLog::cCATE_MASK,
            meta_format,
            args) != false;
    }
    else
    {
        TCHAR message[1024];
        result = (0 <= _vsntprintf(message, _countof(message), meta_format, args));
        message[_countof(message) - 1] = _T('\0');
    }
    va_end(args);

    return result;
}
