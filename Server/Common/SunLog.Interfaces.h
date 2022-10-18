#pragma once
#ifndef SERVERCOMMON_SUNLOG_INTERFACES_H
#define SERVERCOMMON_SUNLOG_INTERFACES_H

class CSunLog;
class SecureLog;

// 모든 게임서버에 하나씩 존재하는 로그
extern CSunLog* g_pSunLog;

//==================================================================================================
// 기본 SUNLOG로 g_pSunLog의 instance를 사용하는 로그출력 Interface
// NOTE: f110512.3L, added facility to support console output for simple logging without file output
extern bool MessageOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...); // use 'g_pSunLog'
extern bool ConsoleOut(eSUN_LOG_LEVEL log_level, const TCHAR* format, ...); // use 'g_pSunLog'
// NOTE: logging extention interface using not-default sunlog instance.
extern bool MessageOutEx(CSunLog* sun_log,
                         CSunLog::eLOGLEVEL log_level,
                         CSunLog::eLOGOPTION log_option,
                         CSunLog::eLOGCATEGORY log_category,
                         const TCHAR* format,
                         ...);
//==================================================================================================

// 특정 게임 서버에 존재하는 로그
extern SecureLog* g_pSecureLog;

BOOL
MessageSecure(const DWORD dwUserGUID, const DWORD dwCharGUID,
              const TCHAR* pszUserID,       // 24 + 1
              const TCHAR* pszCharName,     // 15 + 1
              const TCHAR* pszConnectedIP,  // 15 + 1
              const eABUSE_TYPE eAbuseType, const eABUSE_SUB_TYPE eAbuseSubType,
              const TCHAR* pszMsgFormat,
              ...); /*msg arguments*/

#define SUNLOG		MessageOut
#define SUNLOGEX	MessageOutEx
#define SECURELOG	MessageSecure

#define SECURELOG_PLAYERINFO( obj, AbuseType, AbuseSubType ) \
    (obj)->GetUserGuid(), (obj)->GetCharGuid(), (obj)->GetUserID(), (obj)->GetCharName(), (obj)->GetUserIP(), AbuseType, AbuseSubType
#define SECURELOG_USERINFO( obj, AbuseType, AbuseSubType ) \
    (obj)->GetUserGUID(), (obj)->GetCharGUID((obj)->GetSelectedCharIndex()), (obj)->GetUserID(), (obj)->GetCharName(), (obj)->GetIP(), AbuseType, AbuseSubType
#define SECURELOG_TEMPUSERINFO( obj, AbuseType, AbuseSubType ) \
    0, 0, (obj)->GetUserID(), NULL, (obj)->GetIP(), AbuseType, AbuseSubType

#if MEMORY_POOL_FACTORY_VERSION >= 0x0300

void AssertionLogger4Pool_Server();

#else

namespace util
{
	struct ePOOL_ERROR;
};
VOID	AssertionLogger4Pool_Server( DWORD errTYPE, util::ePOOL_ERROR* IN pINFO );

#endif

#endif //SERVERCOMMON_SUNLOG_INTERFACES_H