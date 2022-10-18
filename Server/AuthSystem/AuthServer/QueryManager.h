#pragma once

#include "Query_Base.h"
#include <CriticalSection.h>
#include <Singleton.h>

#include "Query_Netcafe.h"

class DBWorker;

class QueryManager : public util::Singleton<QueryManager>
{
public:
    QueryManager();
    ~QueryManager();

    void Create(int DBWorkerNum);
    void Destroy();
    void Update();

    QueryObject* AllocQuery(eQUERYTYPE type);
    void FreeQuery(QueryObject* pQuery);
    int  GetAllocQueryCount()    { return m_AllocQueryCount; }

    void EndQuery();
    QueryObject* WaitForQueryObj(int WaitTick=INFINITE);
    void AddQueryResult(QueryObject* pQuery);

    // (WAVERIX) (090702) (NOTE) QueryLoginUser에 관련해서...
    // - 종류가 많아지고 있다. 통합할 필요도 있을 듯 하지만... 필요 이상으로 복잡해지는 프로세
    // 스를 담을 가능성 역시 있는 관계로 사례별 구분하기로...
#ifdef _RU_INGAMBA_AUTH_PROCESS
    void QueryLoginUserByRussia(DWORD login_front_id, DWORD front_user_id, int user_guid,
                                const char* user_id, nsPacket::ReAuthInfoBase::ReAuthKeyType re_auth_key,
                                LPTSTR client_ip, LPTSTR local_ip);

    void QueryReLogintUserByRussia(DWORD login_front_id, DWORD front_user_id, const char* user_id, 
                                   nsPacket::ReAuthInfoBase::ReAuthKeyType re_auth_key,
                                   LPTSTR client_ip, LPTSTR local_ip);
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    void QueryLoginUserByAuthTokeyGameChu(DWORD login_front_id,
                                          GAMECHUUSN gamechu_token, DWORD frontUserId,
                                          nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                          LPTSTR clientIp, LPTSTR localIp);
#endif
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    void QueryLoginUserByAuthTokenNorthA(DWORD login_front_id, LPTSTR account,
                                         //BYTE* auth_token, WORD auth_token_length,
                                         DWORD frontUserId,
                                         nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                         LPTSTR clientIp, LPTSTR localIp);
#endif
#ifdef _CHANNELING_AUTH_FLOW
    void QueryLoginUserByAuthTokenKorea(DWORD login_front_id, LPTSTR account,
                                        BYTE* auth_token, WORD auth_token_length,
                                        DWORD frontUserId,
                                        nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                        LPTSTR clientIp, LPTSTR localIp);
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    void QueryLoginUserByAuthTokenGlobal(DWORD login_front_id, LPTSTR game_id,
                                         char* auth_token, WORD auth_token_length,
                                         DWORD frontUserId,
                                         nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                         LPTSTR clientIp, LPTSTR localIp,
                                         LPTSTR mac_address);
#endif
#ifdef _CN_ZY_AUTH_PROCESS
    void QueryCheckPasswordByChinaZY(DWORD login_front_id, DWORD frontUserId, \
                                     LPTSTR account, LPTSTR password, \
                                     nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                     LPTSTR clientIp, LPTSTR localIp);
    void QueryLoginUserByChinaZY(DWORD login_front_id,
                                 LPTSTR account, LPTSTR password, DWORD frontUserId,
                                 nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                                 LPTSTR clientIp, LPTSTR localIp);
#endif //_CN_ZY_AUTH_PROCESS

    void QueryLoginUser(DWORD login_front_id,
                        LPTSTR account, LPTSTR password, DWORD frontUserId,
                        nsPacket::ReAuthInfoBase::ReAuthKeyType reAuthKey,
                        LPTSTR clientIp, LPTSTR localIp);

    //!_AUTH_GLOBAL
    void QueryChangeUserSts(LPTSTR account, DWORD userGuid,
                            LPTSTR glogKey, AGENTID serverId,
                            LPTSTR publicIp, LPTSTR localIp,
                            USERSTATUS_TYPE status, SYSTEMTIME &loginTime,
                            DWORD authKey = 0);
    //!_AUTH_GLOBAL || !_AUTH_RUSSIA
    void QueryConnectLog(DWORD userGuid, LPTSTR account, DWORD serverGuid,
                         LPTSTR charName, short classType, WORD lv, LPTSTR ip, int pcBangGuid,
                         int usedTime, SYSTEMTIME& timeLogin, BYTE status);
    //!_AUTH_GLOBAL || !_AUTH_RUSSIA
    void QueryConnectLogUpdate(DWORD SeqNum, DWORD userGuid, char* CharName, SYSTEMTIME& timeLogin);

    void QueryRecoverSts();
    void QueryControlIpList();
    void QueryGameVersion();
    void QueryUpdateAuthFailCount(LPTSTR account, LPTSTR clientIP, BYTE btFlag,
        DWORD front_user_id, DWORD login_front_id, BYTE btAuthType = ACCOUNT_AUTH);

#ifdef _NET_CAFE
    void QueryNetCafeLogInsert(DWORD key, DWORD dwFrontUserId,
                               LPTSTR account, LPTSTR clientIp, 
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
                               eNetCafeLoginType login_type,
#else
                               bool bLogin,
#endif
                               DWORD re_auth_key); // registered by _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    //_JP000000_ONETIMEPASSWORD_AUTHFLOW_
    void QueryNetCafe2ndAuthResult(DWORD key, DWORD front_user_id, LPTSTR account,
                                   const nsPacket::OneTimePassword& one_time_password);
#endif
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    void Query_UserId(AGENTID game_agent_id, const QueryUserIdInfo& query_info);
#endif

#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    // gb.USP_WS_CheckAuthKey
    //void QueryLoginUserByAuthTokenGlobal
    // gb.USP_INSERT_CONNECTLOG
    void Query_GspUpdateConnectLog(DWORD user_guid,
                                   const char* game_id, //assert(null-terminated)
                                   AGENTID server_guid,
                                   int play_time,
                                   BOOLEAN is_login, // true(login), false(logout)
                                   const char* char_name, //assert(null-terminated)
                                   char char_class,
                                   short char_level,
                                   const char* log_ip_address);
    // gb.USP_INSERT_ACCOUNTSTATE
    void Query_GspUpdateUserState(DWORD user_guid,
                                  AGENTID server_guid,
                                  const char* connect_ip,//assert(null-terminated)
                                  BOOLEAN is_connect);   // true(login), false(logout)
    // gb.USP_WS_GetAccountID
    void Query_GspGetAccountID(AGENTID game_agent_id, const QueryUserIdInfo& query_info);
    void Query_GspGetAccountID_ForAuth(AccountInfo* account); // passing block    
#endif //__NA000000_GLOBAL_AUTH_PROCESS__

#ifdef __CN_20100909_SECURITY_TOKEN__
	void Query_TokenKeyUpdate(DWORD user_guid, char token_base64_key_[TOKEN_BASE64_KEY_MAX + 1]);
#endif
    void QueryPCRoomIPList(const long pc_room_guid);

private:
    util::CCriticalSection m_csPool;
    typedef util::PoolRouter<QUERYTYPE_MAX> QUERY_POOL_ROUTER;
    QUERY_POOL_ROUTER m_QueryPool;
    int m_AllocQueryCount;

    eXtreme::EventQueue m_QueryQueue;

    eXtreme::Mutex m_cslist;
    typedef std::list<QueryObject*> LIST_QUERY_RESULT;
    LIST_QUERY_RESULT m_listQueryResult;

    int m_dbWorkerNum;
    DBWorker** m_dbWorker;
};












