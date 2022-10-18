#include "stdafx.h"
#include "./UserInfo.h"
//
#include <LogTimeFormat.h>
#include <WZ_MD5.h>
#include <time.h>

#ifdef _KR_0_20120207_CRYPT_SHA256
    #include "../ProgramCommon/SHA256.h"
#endif

//==================================================================================================

void ns_object::UserAccountInfo::WriteLoginTime(bool is_account)
{
    TCHAR* dest_login_time = is_account ?
        LOGIN_INFO.login_time_string : LOGIN_INFO.char_login_time_string;
    BOOST_STATIC_ASSERT(sizeof(LOGIN_INFO.login_time_string) ==
                        sizeof(LOGIN_INFO.char_login_time_string) &&
                        _countof(LOGIN_INFO.login_time_string) == MAX_TIMEDATA_SIZE + 1);

    time_t* dest_time = is_account ? &LOGIN_INFO.login_time : &LOGIN_INFO.char_login_time;
    //
    ZeroMemory(dest_login_time, sizeof(LOGIN_INFO.login_time_string));
    CLogTimeFormat::GetTime(3, dest_login_time,
        _countof(LOGIN_INFO.login_time_string) - 1);
    util::TimeSync::time(dest_time);
}

bool ns_object::UserAccountInfo::IsEqualLastSSN(const BYTE* pLastSSNKey)
{
    if (!pLastSSNKey)
        return false;

    BYTE digest[16] = { 0, };
    wchar_t wszUserLastSSN[MAX_SSN_STREAM_LENGTH+1];
    wchar_t wszUserGUID[64];

    ZeroMemory(wszUserLastSSN, sizeof(wszUserLastSSN));
    ZeroMemory(wszUserGUID, sizeof(wszUserGUID));

    //주민등록 번호 뒷자리는 최대 10자리로 한다.(10자리 까지 지원된다. 7자리는 한국, 일본은 8자리, 중국이 10자리)
    for(int i = 0; i < 10; i++)
        wszUserLastSSN[i] = (wchar_t)pLastSSNKey[i];

    _ultow(KEY_INFO.user_guid, wszUserGUID, 10);

#ifdef _KR_0_20120207_CRYPT_SHA256
    int mode = AgentServer::GetInstance()->GetMode();
    switch (mode)
    {
    case eSERVER_DEV:
        {
            MD5::EncodeString(
                (BYTE*)wszUserLastSSN, (int)wcslen(wszUserLastSSN)*2,
                digest,
                (BYTE*)wszUserGUID, (int)wcslen(wszUserGUID)*2);
            if (0 == strncmp((char*)ID_INFO.ssn_string, (char*)digest, 16))
                return true;
        }
        break;
    case eSERVER_LIVE:
    case eSERVER_TESTLIVE:
    case eSERVER_INTRAOFFICE:
        {
            Crypt::SHA256 sha256;
            bool result = sha256.Compare((const char*)pLastSSNKey, "WEBZEN", (const char*)ID_INFO.ssn_string);
            if (result == false && strlen(sha256.GetLastError()) != 0)
                SUNLOG(eCRITICAL_LOG, "[SHA256]SSN Fail : %s", sha256.GetLastError());
            return result;
        }
        break;
    default:
        return false;
    }
#else
    MD5::EncodeString(
        (BYTE*)wszUserLastSSN, (int)wcslen(wszUserLastSSN)*2,
        digest,
        (BYTE*)wszUserGUID, (int)wcslen(wszUserGUID)*2);
    if (0 == strncmp((char*)ID_INFO.ssn_string, (char*)digest, 16))
        return true;
#endif
    return false;
}




//==================================================================================================
//==================================================================================================
//==================================================================================================

