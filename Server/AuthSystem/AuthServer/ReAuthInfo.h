#ifndef _REAUTH_INFO_H_
#define _REAUTH_INFO_H_

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__

#include "ReAuthManager.h"
#include <AuthStepAuthToken.h>
#include <AuthStructInternal.h>

class ReAuthInfo : public nsPacket::ReAuthInfoBase
{
public:
    enum eReAuthSlot {
        eReAuthSlot_ServerSide,
        eReAuthSlot_ClientSide,
        eReAuthSlot_Count,
    };
    typedef eReAuthStep ReAuthStepSlot[eReAuthSlot_Count];

    inline void Clear();
    inline ReAuthKeyType GetReAuthKey() const { return m_ReAuthKey; }
    inline const ReAuthStepAuthTokenInfo* GetReAuthStepAuthTokenInfo() const;
    inline BOOLEAN IsExpired() { return m_Timeout.IsExpired(); };
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
	GAMECHUUSN gamechu_usn_;	
#endif

    inline const ReAuthStepSlot& CurrentStep() const { return m_ReAuthSteps; }

    inline BOOLEAN CheckPositionInfo(DWORD ipAddress, const char account_id[ID_MAX_LEN + 1]) const;
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    inline const AccountInfo* GetBackupedAccountInfo() const { return &m_AccountInfo; }
#endif
private:
    inline BOOLEAN ChangeStep(eReAuthSlot where, eReAuthStep step);

    ReAuthKeyType m_ReAuthKey;
    DWORD m_FrontKey; // login front id - 클라가 먼저 접속하면 먼저 할당, 나중에 할당도 가능...
    ITimeout m_Timeout;
    DWORD m_UserGuid;
    DWORD m_IpAddress;
    ReAuthStepSlot m_ReAuthSteps;  // Client가 서버 처리보다 먼저 이뤄지는 상황 지연 처리 목적
    ReAuthStepAuthTokenInfo m_ReAuthTokenInfo;
    char  m_AccountID[ID_MAX_LEN + 1];
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    AccountInfo m_AccountInfo; // (NOTE) Global인증 처리는 DB에서 인증키를 검사하기 때문에 별수 없이...
#endif

    friend class ReAuthManager;
};


inline void
ReAuthInfo::Clear()
{
    m_ReAuthKey = 0;
    m_FrontKey = 0;
    m_Timeout.Disabled();
    m_UserGuid = 0;
    m_IpAddress = 0;
    ZeroMemory(&m_ReAuthSteps, sizeof(m_ReAuthSteps));
    ZeroMemory(&m_ReAuthTokenInfo, sizeof(m_ReAuthTokenInfo));
    ZeroMemory(m_AccountID, sizeof(m_AccountID));
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    ZeroMemory(&m_AccountInfo, sizeof(m_AccountInfo));
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
	gamechu_usn_ = 0;
#endif
}

inline const ReAuthStepAuthTokenInfo*
ReAuthInfo::GetReAuthStepAuthTokenInfo() const {
    return &m_ReAuthTokenInfo;
}

inline BOOLEAN
ReAuthInfo::ChangeStep(eReAuthSlot where, eReAuthStep step)
{
    BOOLEAN bAccept = where < eReAuthSlot_Count;
    if(bAccept == false)
        return false;

    bAccept = step < eReAuthStep_Count;
    if(bAccept == false)
        return false;

    m_ReAuthSteps[where] = step;
    return true;
}

inline BOOLEAN
ReAuthInfo::CheckPositionInfo(DWORD ipAddress, const char account_id[ID_MAX_LEN + 1]) const
{
    if(m_IpAddress != ipAddress)
        return false;
#if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
    if(strncmp(m_AccountID, account_id, _countof(m_AccountID)) != 0)
        return false;
#elif defined(__NA000000_GLOBAL_AUTH_PROCESS__)
    if(strncmp(m_AccountInfo.m_GameID, account_id, _countof(m_AccountInfo.m_GameID)) != 0)
        return false;
    BOOST_STATIC_ASSERT(_countof(m_AccountInfo.m_GameID) < ID_MAX_LEN &&
                        sizeof(m_AccountInfo.m_GameID) < sizeof(m_AccountID));
#else
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

    return true;
}

#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

#endif //_REAUTH_INFO_H_
