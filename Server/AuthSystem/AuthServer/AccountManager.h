#pragma once


#include <Singleton.h>


#ifdef __PCBANG_IP_UPDATE
#include "PCRoomIpManager.h"
#else
#include <map>
#include <MemoryPoolFactory.h>
#endif

class AccountManager : public util::Singleton<AccountManager>
{
public:
    //_PCBANG_POLICY_CHANGE_POST_EXPIRED2
#ifdef __NA_20100201_INTERGRATEDBILLING_
    static const DWORD PC_ROOM_RECHARGE_QUERY_INTERVAL = 180 * 1000; //통합 빌리은 3분 
#else
    static const DWORD PC_ROOM_RECHARGE_QUERY_INTERVAL = 60 * 1000; //정량 만료 후 1분 주기 질의
#endif

    AccountManager();
    ~AccountManager();

    void Destroy();
    void Initialize();
    void Update();

    static WORD CalcConnectTime(AccountInfo* info);


    AccountInfo* CreateAccountInfo(LPTSTR account, AccountInfo** overlapped);

    AccountInfo* Find(LPTSTR account);
    AccountInfo* FindWithLoginFrontUserID(DWORD frontId);
    AccountInfo* FindWithAuthKey(DWORD authKey);
    AccountInfo* FindWithUserGuid(DWORD guid);

    BOOL ClearInfo(LPTSTR account);
    BOOL ClearInfo(DWORD frontId);
    void ClearLocation(AGENTID locationId);


    void GenerateAuthKey(AccountInfo* info);
    void GenerateSerialKey(AccountInfo* info);

//#ifdef _NEW_BILLING_SYSTEM
//    template<class Operator>
//    void ForEachAccount(Operator & Opr);
//#else
#ifdef _BILLING_CODE_REFACTORING_
    void MakeBillingList(STLX_VECTOR<AccountInfo*>* stream_list);
#else
    void MakeBillingList(std::vector < AccountInfo* >* buffer);
#endif
//#endif

    void DisplayInfo();
    void DisplayDetailInfo();

#ifdef __PCBANG_IP_UPDATE
    PCRoomIPManager* const GetPCRoomManager() {return &pc_room_manager_;};
#endif

private:
    DWORD GetHashCode(LPTSTR str);

    typedef STLX_MAP<DWORD, AccountInfo*>  MapAccount;
    typedef util::CMemoryPoolFactory<AccountInfo>  ACCOUNT_POOL;

    ACCOUNT_POOL m_AccountPool;
    MapAccount   m_mapAccount;

    DWORD m_authKey;
    DWORD m_authKeyOffset;

#ifdef __PCBANG_IP_UPDATE
    PCRoomIPManager pc_room_manager_;
#endif

    util::Timer  m_UpdateTimer;
};

//#ifdef _NEW_BILLING_SYSTEM
//template<class Operator>
//void AccountManager::ForEachAccount(Operator& Opr)
//{
//    MapAccount::iterator it = m_mapAccount.begin(),
//                         end = m_mapAccount.end();
//    for( ; it != end; ++it)
//    {
//        AccountInfo* pAccountInfo = it->second;
//        Opr(pAccountInfo);
//    }
//}
//#endif










