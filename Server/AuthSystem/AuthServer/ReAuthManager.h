#ifndef _REAUTH_MANAGER_H_
#define _REAUTH_MANAGER_H_

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__

#include <ITimeout.h>
#include <Singleton.h>

namespace util {

template<typename _KEYTYPE>
class CObjKeyGenerator;

}; //end of namespace

class ReAuthInfo;
struct AccountInfo;

class ReAuthManager : public nsPacket::ReAuthInfoBase
                    , public util::Singleton<ReAuthManager>
{
    // (Query Methods)
    // AgentServer(UserKey) -> AccountInfo* -> 'ReAuthKeyType'
    // Client(ReAuthToken) -> extract 'ReAuthKeyType'
public:
    static const ReAuthKeyType ReAuthKeyStart = 2907;
    static const ReAuthKeyType ReAuthKeyEnd = ReAuthKeyStart + 3095;
    static const ReAuthKeyType ReAuthKeyRange = ReAuthKeyEnd - ReAuthKeyStart;

    ReAuthManager();
    ~ReAuthManager();
    void Randomize();

    inline const ReAuthInfo* FindReAuthInfo(ReAuthKeyType reAuthKey) const;
    ReAuthKeyType NumberOfFreeReAuthKey() const;
    void Update();

    BOOLEAN OnAcceptReAuthClient(const ReAuthInfo* pReAuthInfo);
    BOOLEAN OnAcceptQueryLoginUser(const ReAuthInfo* pReAuthInfo); // T : free pReAuthInfo
    BOOLEAN OnCancelByGameServer(const ReAuthInfo* pReAuthInfo);

	const ReAuthInfo* AllocReAuthInfo(DWORD userGuid, DWORD ipAddress,
										const char account_id[ID_MAX_LEN + 1],
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
										GAMECHUUSN gamechu_usn,
#endif
										AccountInfo* account_info = 0 /*for global*/);

private:
    BOOLEAN FreeReAuthInfo(ReAuthInfo* pReAuthInfo);

    typedef util::CObjKeyGenerator<ReAuthKeyType> ReAuthKeyGenerator;
    ReAuthKeyGenerator* m_pReAuthKeyGenerator;
    ITimerBase m_UpdateTimer;
    ReAuthInfo* m_ReAuthInfos[ReAuthKeyRange];
};


inline const ReAuthInfo*
ReAuthManager::FindReAuthInfo(ReAuthKeyType reAuthKey) const
{
    BOOLEAN bInRange = (ReAuthKeyStart <= reAuthKey &&
                                          reAuthKey < ReAuthKeyEnd);
    if(bInRange == false)
        return 0;

    ReAuthKeyType offset = reAuthKey - ReAuthKeyStart;
    return m_ReAuthInfos[offset];
}

#endif //__NA001334_090420_SERVER_SELECT_FACILITY__

#endif //_REAUTH_MANAGER_H_

