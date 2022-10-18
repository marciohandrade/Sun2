#include "stdafx.h"
#include "SunLog.h"
#include "SecureLog.h"

#include <ServerGlobal.h>
#include <MemoryPoolFactory.h>

//==================================================================================================

CSunLog* g_pSunLog = NULL;
SecureLog* g_pSecureLog = NULL;

//==================================================================================================

bool MessageOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...)
{
    bool result = false;
    va_list args;
    va_start(args, format);
    if (g_pSunLog)
    {
        result = CSunLog::LOGGINGwithARG(g_pSunLog,
            CSunLog::cOUTPUT_BOTH, CSunLog::eLOGLEVEL(log_level), CSunLog::cCATE_MASK,
            format, args) != false;
    }
    else
    {
        TCHAR making_message[1024];
        result = (_vsntprintf(making_message, _countof(making_message), format, args) >= 0);
        making_message[_countof(making_message) - 1] = _T('\0');
        _putts(making_message);
    }
    va_end(args);
    return result;
};

//==================================================================================================
// NOTE: f110512.3L, added facility to support console output for simple logging without file output
bool ConsoleOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...)
{
    bool result = false;
    va_list args;
    va_start(args, format);
    if (g_pSunLog)
    {
        result = CSunLog::LOGGINGwithARG(g_pSunLog,
            CSunLog::cOUTPUT_CONSOLE, CSunLog::eLOGLEVEL(log_level), CSunLog::cCATE_MASK,
            format, args) != false;
    }
    else
    {
        TCHAR making_message[1024];
        result = (_vsntprintf(making_message, _countof(making_message), format, args) >= 0);
        making_message[_countof(making_message) - 1] = _T('\0');
        _putts(making_message);
    }
    va_end(args);
    return result;
};

//==================================================================================================
// NOTE: logging extention interface using not-default sunlog instance.
bool MessageOutEx(CSunLog* sun_log,
                  CSunLog::eLOGLEVEL log_level,
                  CSunLog::eLOGOPTION log_option,
                  CSunLog::eLOGCATEGORY log_category,
                  const TCHAR* format,
                  ...)
{
    bool result = false;
    va_list args;
    va_start(args, format);
    if (sun_log)
    {
        result = CSunLog::LOGGINGwithARG(sun_log,
            CSunLog::eLOGOPTION(log_option),
            CSunLog::eLOGLEVEL(log_level),
            CSunLog::eLOGCATEGORY(log_category),
            format, args) != false;
    }
    else
    {
        TCHAR making_message[1024];
        result = (_vsntprintf(making_message, _countof(making_message), format, args) >= 0);
        making_message[_countof(making_message) - 1] = _T('\0');
        _putts(making_message);
    }
    va_end(args);
    return result;
};

//==================================================================================================

BOOL
MessageSecure(const DWORD dwUserGUID, const DWORD dwCharGUID,
              const TCHAR* pszUserID,       // 24 + 1
              const TCHAR* pszCharName,     // 15 + 1
              const TCHAR* pszConnectedIP,  // 15 + 1
              const eABUSE_TYPE eAbuseType, const eABUSE_SUB_TYPE eAbuseSubType,
              const TCHAR* pszMsgFormat,
              ...) /*msg arguments*/
{
	BOOL bRet = FALSE;
	va_list args; 
	va_start(args, pszMsgFormat); 

	if( g_pSecureLog )
	{
		g_pSecureLog->SetUserGUID( dwUserGUID );
		g_pSecureLog->SetCharGUID( dwCharGUID );
		g_pSecureLog->SetUserID( pszUserID );
		g_pSecureLog->SetCharName( pszCharName );
		DWORD dwIP = IPOperation::ConvertStrIP2DWORD( pszConnectedIP );
		g_pSecureLog->SetConnectedIP( dwIP );

		g_pSecureLog->SetAbuseType( eAbuseType );
		g_pSecureLog->SetAbuseSubType( eAbuseSubType );

		bRet = CSunLog::LOGGINGwithARG( g_pSecureLog, CSunLog::cOUTPUT_BOTH, CSunLog::cCRITICAL_LOG, CSunLog::cCATE_ABUSE, pszMsgFormat, args	);
	}
	else
	{
		TCHAR pMSG[1024];
		_tcsncpy( pMSG, SUNLOG_ENUM_TO_STRING(cCATE_CONNECTION), 100 );
		INT iWritten = (INT)_tcslen( pMSG );
		bRet = (BOOL)!( 0 > _vsntprintf( pMSG+iWritten, 1024-iWritten, pszMsgFormat, args ) );
		pMSG[1024-1] = _T('\0');
		_putts( pMSG );
	}
	va_end(args);

	return bRet;
}

#if MEMORY_POOL_FACTORY_VERSION >= 0x0300

void AssertionLogger4Pool_Server()
{
    // dummy function;
}

#else

VOID	AssertionLogger4Pool_Server( DWORD errTYPE, util::ePOOL_ERROR* IN pINFO )
{
	TCHAR* pErrTypeMsg = util::ePOOL_ERROR::INVALID_REFERENCE == errTYPE ? _T("Freeing Address is Can't Accessable")
		: util::ePOOL_ERROR::CANT_ALLOCATABLE == errTYPE ? _T("Can't Allocatable")
		: util::ePOOL_ERROR::CANT_DEALLOCATABLE == errTYPE ? _T("Can't DeAllocatable, STAT(Already Freed | Invalid Reference | Overrun)")
		: util::ePOOL_ERROR::NOT_MATCHED_ALLOCnDEALLOC == errTYPE ? _T("Free Count is not matched AllocCount")
		: NULL;

	MessageOut( eCRITICAL_LOG, _T("[POOL ASSERTION][TYPE:%s] \"%s\""), pINFO->m_pszTYPENAME, pErrTypeMsg );
	//printf( _T("[POOL ASSERTION][TYPE:%s] \"%s\"\n"), pINFO->m_pszTYPENAME, pErrTypeMsg );

#	ifdef _DEBUG
	__asm { int 3 }
#	endif //
}

#endif //