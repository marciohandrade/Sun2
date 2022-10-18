#ifndef __TEMP_USER_H__
#define __TEMP_USER_H__

#pragma once

#include "UserSession.h"

struct MSG_CG_CONNECTION_ENTERSERVER_SYN;

class TempUser : public UserSession
{
public:
    TempUser();
    virtual ~TempUser();

    virtual BOOL SendPacket(MSG_BASE* pMsg, WORD wSize, const BOOLEAN bUsePacking = TRUE);
protected:
    virtual VOID Init();
    virtual VOID Release();
public:
    virtual eUSER_TYPE GetUserType() const { return UT_TEMP_USER; };
    inline ITimeout& NoActionTimeout() { return m_dwNoActionTimeout; }
    // (WAVERIX) (090526) (CHANGES)
    static DWORD GetConnectionTimeout();
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    VOID SetAge(BYTE byAge) { m_byAge = byAge; }
    BYTE GetAge() { return m_byAge; }
    VOID SetRealNameAuth(BYTE byRealNameAuth) { m_byRealNameAuth = byRealNameAuth; }
    inline BYTE GetRealNameAuth() { return m_byRealNameAuth; }
#endif //

private:
    virtual VOID OnAccept(DWORD dwNetworkIndex);
    virtual VOID OnDisconnect();
    virtual VOID OnRecv(BYTE* pMsg, WORD wSize);
    //BOOL _IsAuthenticatedUser(UserSession* OUT pNewUser, MSG_CG_CONNECTION_ENTERSERVER_SYN* pMsg);
    BOOL Authentication(MSG_CG_CONNECTION_ENTERSERVER_SYN * pMsg);
    void SendEnterServerNak(BYTE byErrorCode);

    enum {
        IDLING, CHECKING, PASSING
    };

    BOOL ProcessCHECKING(BYTE* pMsg, WORD wSize);
    BOOL ProcessPASSING(BYTE* pMsg, WORD wSize);

    //----------------------------------------------------------------------------------------------
    //  (Fields)
    //BOOL   m_bFirst;
    DWORD    m_dwChangeMod;
    User*    m_pNewUser;
    ITimeout m_dwNoActionTimeout;
    CHAR     m_szID[ID_MAX_LEN+1];    
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
    BYTE     m_byAge;
    BYTE     m_byRealNameAuth;
#endif  //__CN_OO0510__FATIGUE_MATCHING_UPDATE
    __DISABLE_COPY(TempUser);
};

#endif // __TEMP_USER_H__