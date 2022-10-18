#include "stdafx.h"
#include "AccountManager.h"
#include "QueryManager.h"
#include "AuthFrame.h"
#ifdef __NA_20100201_INTERGRATEDBILLING_
#include "IntegratedBillingServerSession.h"
#else
#include ".\BillingServerSession.h"
#endif

#ifdef _NA_20120215_RUSSIA_PREMIUM_SERVICE
#include "IngambaAuthConnector.h"
#include "ServerDataManager.h"
#include "AuthAgentServerSession.h"
#include "SunAuth/AuthProtocol_GA.h"
#endif


AccountManager::AccountManager()
{
    m_authKey = AuthFrame::Instance()->GetAuthKey();
    m_authKeyOffset = ::GetTickCount();

    // AuthKey는 m_authKey와 Seq한 Offset의 조합으로 이루어진다.
    // 만일 서비스 중 인증서버장애시 재시작을 하게될 경우,
    // AuthKey가 이전 사용자와 중복되어 생성되는 문제가 발생할 수 있기 때문에,
    // Offset의 초기값을 TickCount로 설정한다.
}

AccountManager::~AccountManager()
{
    
}

void
AccountManager::Destroy()
{
    MapAccount::iterator it = m_mapAccount.begin(),
                         end = m_mapAccount.end();
    for( ; it != end; ++it)
        m_AccountPool.Free(it->second);

    m_mapAccount.clear();
    m_AccountPool.Release();
}

void
AccountManager::Initialize()
{
    m_AccountPool.Initialize(500, 500); // (090818) 풀 크기 조정

#if !defined(_AUTH_GLOBAL)
    QueryManager::Instance()->QueryRecoverSts();
#endif

    m_UpdateTimer.SetTimer(30000);

}

#ifdef _NA_20120215_RUSSIA_PREMIUM_SERVICE
void
AccountManager::Update()
{
    if(!m_UpdateTimer.IsExpired(true))
    {
        return;
    }

    AuthFrame* const auth_frame = AuthFrame::Instance();
    const DWORD auth_limit_time = auth_frame->GetAuthLimitTime();
    const DWORD current_tick_count = GetTickCount();
    QueryManager* const query_manager = QueryManager::Instance();

    IngambaAuthConnector* ingamba_auth_connector = auth_frame->GetIngambaAuthConnector();
    if (ingamba_auth_connector == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not initialize ingamba auth connector"
            );
        return;
    }

    MapAccount::iterator it = m_mapAccount.begin();
    while (it != m_mapAccount.end())
    {
        AccountInfo* account_info = it->second;
        ++it; // pre-action before 'Clear'
        if (account_info == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : account info data is null"
                );
            continue;
        }

        BOOLEAN is_cleared_account = false;
        BOOLEAN is_game_playing = 
            account_info->m_status == ACCOUNTSTATUS_PLAY || 
            account_info->m_status == ACCOUNTSTATUS_TRYPLAY;

        if (is_game_playing == 0 && current_tick_count - account_info->m_tickAuth > auth_limit_time)
        {
            // 3분 이상 인증중인 사용자의 정보는 제거한다.
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : Removed from location : Account = %s, location = %u, Status = %u",
                account_info->m_account,
                account_info->m_locationId, 
                account_info->m_status
                );

            query_manager->QueryChangeUserSts(
                account_info->m_account, 
                account_info->m_userGuid,
                account_info->m_glogKey,
                account_info->m_locationId,
                account_info->m_clientIp, 
                account_info->m_localIp,
                USERSTATUS_LOGOUT, 
                account_info->m_loginTime
                );

            ClearInfo(account_info->m_account);
            is_cleared_account = true;
        }

        if (is_cleared_account != 0)
        {
            continue;
        }

        if (account_info->m_pcBangStatus.status_ == CHANGE_PCROOM_STATUS::eStatus_PcRoom)
        {
            RC::PrimiumServiceResult result_code = RC::RC_PRIMIUM_FAIL;
            int remaining_time = 0;
            int check_time = 0;

            bool need_request = current_tick_count > account_info->m_pcBangOnRechargeQueryTime;
            if (ingamba_auth_connector && need_request)
            {
                result_code = 
                    ingamba_auth_connector->IsKeepingPrimiumService(
                    *account_info,
                    check_time,
                    remaining_time
                    );
            }

            if (result_code != RC::RC_PRIMIUM_SUCCESS)
            {
                continue;
            }

            ;{
                // CAUTION!!! remaining_time, check_time is in seconds...

                // cannot be more than the status remaining avaliable time.
                if (check_time > remaining_time) {
                    check_time = remaining_time;
                }

                // unlimited long
                if (remaining_time == -1 || check_time == -1) {
                    check_time = 24*60*60; // check 1 day late...
                }
            };

            account_info->m_pcBangOnRechargeQueryTime = current_tick_count + (check_time*1000);

            if (remaining_time == 0)
            {
                AuthAgentServerSession* auth_agent_session =
                    ServerDataManager::Instance()->FindAuthAgentServerSession(account_info->m_locationId);

                if (auth_agent_session) 
                {
                    MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN send_msg;
                    send_msg.game_agent_id_ = account_info->m_locationId;
                    send_msg.status_.status_ = send_msg.status_.eStatus_PcRoom;
                    send_msg.pc_room_id_ = 0;
                    send_msg.user_key_ = account_info->m_userGuid;
                    send_msg.auth_user_key_ = account_info->m_authKey;
                    auth_agent_session->SendPacket(&send_msg, sizeof(send_msg));

                    account_info->m_pcBangStatus.status_ = 
                        CHANGE_PCROOM_STATUS::eStatus_PcRoomExpired;
                }
            }
        }
    }
}
#else//_NA_20120215_RUSSIA_PREMIUM_SERVICE

void
AccountManager::Update()
{
    if(!m_UpdateTimer.IsExpired(true))
        return;

    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    const DWORD authLimitTime = pAuthFrame->GetAuthLimitTime();
    const DWORD currentTick = GetTickCount();
    QueryManager* const pQueryManager = QueryManager::Instance();
#ifdef __NA_20100201_INTERGRATEDBILLING_
    IntegratedBillingServerSession* pBillingSession = static_cast<IntegratedBillingServerSession*>(pAuthFrame->GetSession(BILLING_SERVER));
#else
    BillingServerSession* pBillingSession = static_cast<BillingServerSession*>(pAuthFrame->GetSession(BILLING_SERVER));
#endif

#ifdef __PCBANG_IP_UPDATE
    PCRoomIPList* const pc_room_list = pc_room_manager_.GetPCROOMIPList();    
#endif
    

    MapAccount::iterator it = m_mapAccount.begin();
    while (it != m_mapAccount.end())
    {
        AccountInfo* pAccountInfo = it->second;
        ++it; // pre-action before 'Clear'

        if (pAccountInfo == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : account info data is null"
                );
            continue;
        }

        BOOLEAN is_cleared_account = false; // (CHANGES) (WAVERIX)
        BOOLEAN is_game_playing = pAccountInfo->m_status == ACCOUNTSTATUS_PLAY ||
                                  pAccountInfo->m_status == ACCOUNTSTATUS_TRYPLAY;
                                  

        // 3분 이상 인증중인 사용자의 정보는 제거한다.
        if(is_game_playing == 0 &&
           currentTick - pAccountInfo->m_tickAuth > authLimitTime)
        {
        #if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
            pQueryManager->QueryChangeUserSts(pAccountInfo->m_account, pAccountInfo->m_userGuid,
                                              pAccountInfo->m_glogKey,
                                              pAccountInfo->m_locationId,
                                              pAccountInfo->m_clientIp, pAccountInfo->m_localIp,
                                              USERSTATUS_LOGOUT, pAccountInfo->m_loginTime);
        #endif
        // in Global - 게임 플레이 중이 아닌 경우는 남기지 않는다.

        #ifdef _NET_CAFE // 일본 넷카페 DB에 유저가 로그아웃함을 통보한다
            if(pAccountInfo->m_pcBangId)
            {
                pQueryManager->QueryNetCafeLogInsert(pAccountInfo->m_LoginFrontId,
                                                     pAccountInfo->m_LoginFrontUserId,
                                                     pAccountInfo->m_account,
                                                     pAccountInfo->m_clientIp,
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
                                                     NETCAFE_LOGOUT,
#else
                                                     false,
#endif
                                                     0);
            }
        #endif

            SUNLOG(eCRITICAL_LOG,
                   "[AccountManager::Update] Account[%s] IP[%s] Removed from location[%u]Status[%u]",
                   pAccountInfo->m_account,
                   pAccountInfo->m_clientIp, pAccountInfo->m_locationId, pAccountInfo->m_status);
            // TODO: Clear의 처리 방법 개선할 것.
            ClearInfo(pAccountInfo->m_account);

            is_cleared_account = true; // (CHANGES) (WAVERIX)
        }

        // (CHANGES) (WAVERIX) (NOTE) 하기 조건 만족시 pAccountInfo의 정보는 초기화 되어 있을 것이다.
        if(is_cleared_account != 0)
            continue;

#ifdef __PCBANG_IP_UPDATE
        if(pc_room_list)
        {
            if(pAccountInfo->m_pcBangStatus.status_ == pAccountInfo->m_pcBangStatus.eStatus_None) //일반유저
            {
                if(pc_room_list->IsPCROOMIP(pAccountInfo->pc_room_ip_value_))//일반유저의 아이피가 피씨방 아이피로 검색 되나 ?
                {
                    //일반 유저를 피시방 유져로 변경 한다.                

                }
            }
            else //피씨방 유저
            {                
                if(pAccountInfo->m_pcBangId == pc_room_list->GetPCRoomGUID()) //같은 피씨방인가 ?
                {   
                    if(!pc_room_list->IsPCROOMIP(pAccountInfo->pc_room_ip_value_)) //같은 피씨방에서 아이피가 검색되지 않나 ?
                    {
                        //피씨방 유저를 일반 유져로 변경한다.

                    }
                }
            }
        }        
#endif

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED2
		
			//------------------------------------------------------------------------------------------
			if(is_game_playing && pBillingSession
#ifdef _BILLING_ONLYPCBANG
				&& pAccountInfo->m_pcBangId
#endif
				) 
			{
				CHANGE_PCROOM_STATUS pc_room_status = pAccountInfo->m_pcBangStatus;
				if(pc_room_status.status_ == pc_room_status.eStatus_PcRoomExpired) 
				{
					// (BUGFIX) (091215) (WAVERIX) prevent billing deduction too much
					// which expired state is not updated 'm_tickBillingTime'
					pAccountInfo->m_tickBillingTime = currentTick; // added, (091215)
					BOOLEAN need_query = currentTick > pAccountInfo->m_pcBangOnRechargeQueryTime;
					if(need_query) 
					{
#ifdef __20100916_BILLING_GODLCHANNEL__
						pBillingSession->InquirePersonDeduct(*pAccountInfo);
#else
						pBillingSession->QueryRestPoint(pAccountInfo);
#endif
						// reset interval for get re-chargeable info query that every 1 minutes
						pAccountInfo->m_pcBangOnRechargeQueryTime =
							currentTick + PC_ROOM_RECHARGE_QUERY_INTERVAL;
					}
				}
			}
		
        //------------------------------------------------------------------------------------------
    #endif
    }

#ifdef __PCBANG_IP_UPDATE
    pc_room_manager_.RemovePCROOMList(pc_room_list);    
#endif
}
#endif//_NA_20120215_RUSSIA_PREMIUM_SERVICE

DWORD
AccountManager::GetHashCode(LPTSTR str)
{
#if defined(_AUTH_GLOBAL)
    // (CHANGES) (f100629.2L) (f100707.1L) changes case-sensitive control policy
    // that the Global version has case-sensitive
    TCHAR account_buffer[ID_MAX_LEN + 10];
    ::_tcsncpy(account_buffer, str, _countof(account_buffer));
    ::_tcsupr(account_buffer);
    account_buffer[_countof(account_buffer) - 1] = _T('\0');
    str = account_buffer;
#endif
    DWORD hash = 5381;

    while(DWORD chr = *str++)
        hash = ((hash << 5) + hash) + chr;

    return hash;
}

WORD
AccountManager::CalcConnectTime(AccountInfo* info)
{
    DWORD curTick = ::GetTickCount();
    WORD  ret = 0;

    if(curTick > info->m_tickConnectTime)
    {   //  정상적인 경우
        ret = (WORD)((curTick - info->m_tickConnectTime) / 60000);
    }
    else
    {   //  Tick count가 Reset된 경우
        ret = (WORD)((0xFFFFFFFF - info->m_tickConnectTime + curTick) / 60000);
    }

    return ret;
}

AccountInfo*
AccountManager::CreateAccountInfo(LPTSTR account, AccountInfo** overlapped)
{
    *overlapped = 0;

    // 이미 동일한 계정이 있는지 체크
    AccountInfo* pAccountInfo = Find(account);
    if(pAccountInfo)
    {
        *overlapped = pAccountInfo;
        return NULL;
    }

    pAccountInfo = (AccountInfo*)m_AccountPool.Alloc();
    ZeroMemory(pAccountInfo, sizeof(*pAccountInfo));

    // (NOTE) 다음 기본 초기화 되는 값은 유념해 두어야 할 것이다.
    //  { m_account, m_tickAuth, m_dwWaitLogoutCnt, m_glogKey }
    CHECK_ID_LENGTH_BUFFER(pAccountInfo->m_account);
    // (CHANGES) (f100629.2L) changes case-sensitive control policy
    // that the Global version has case-sensitive
    ::_tcsncpy(pAccountInfo->m_account, account, _countof(pAccountInfo->m_account));
    pAccountInfo->m_account[_countof(pAccountInfo->m_account) - 1] = _T('\0');
    pAccountInfo->m_tickAuth = ::GetTickCount();
#ifdef _BILLING_WAITLOGOUT
    pAccountInfo->m_dwWaitLogoutCnt = 0;
#endif

    DWORD hashKey = GetHashCode(pAccountInfo->m_account);
    m_mapAccount[hashKey] = pAccountInfo;

    //  GLogKey 생성
    BOOST_STATIC_ASSERT(sizeof(pAccountInfo->m_glogKey) == GLOGKEY_LEN &&
                        _countof(pAccountInfo->m_glogKey) == GLOGKEY_LEN);
    ::memset(pAccountInfo->m_glogKey, 0, sizeof(pAccountInfo->m_glogKey));

    for(int i = 0; i < GLOGKEY_LEN - 1; ++i) //  Null Terminated는 제외
    {
        if(rand() % 2 == 0)
            pAccountInfo->m_glogKey[i] = '0' + rand() % 10;
        else
            pAccountInfo->m_glogKey[i] = 'a' + rand() % 26;
    }

    return pAccountInfo;
}

AccountInfo*
AccountManager::Find(LPTSTR account)
{
    DWORD hashKey = GetHashCode(account);
    MapAccount::iterator iter = m_mapAccount.find(hashKey);
    if(iter == m_mapAccount.end())
        return 0;

    return iter->second;
}

AccountInfo*
AccountManager::FindWithLoginFrontUserID(DWORD frontId)
{
    // TODO: 개선할 것.
    MapAccount::iterator it = m_mapAccount.begin(),
                         end = m_mapAccount.end();
    for( ; it != end; ++it)
    {
        AccountInfo* pAccountInfo = it->second;
        if(pAccountInfo->m_LoginFrontUserId == frontId)
            return pAccountInfo;
    }

    return NULL;
}

AccountInfo*
AccountManager::FindWithAuthKey(DWORD authKey)
{
    // TODO: 개선할 것.
    MapAccount::iterator it = m_mapAccount.begin(),
                         end = m_mapAccount.end();
    for( ; it != end; ++it)
    {
        AccountInfo* pAccountInfo = it->second;
        if(pAccountInfo->m_authKey == authKey)
            return pAccountInfo;
    }

    return NULL;
}

AccountInfo*
AccountManager::FindWithUserGuid(DWORD guid)
{
    // TODO: 개선할 것.
    MapAccount::iterator it = m_mapAccount.begin(),
                         end = m_mapAccount.end();
    for( ; it != end; ++it)
    {
        AccountInfo * pAccountInfo = it->second;
        if(pAccountInfo->m_userGuid == guid)
            return pAccountInfo;
    }

    return NULL;
}

BOOL
AccountManager::ClearInfo(LPTSTR account)
{
    AccountInfo* pAccountInfo = Find(account);
    if(!pAccountInfo)
        return FALSE;

    DWORD hashKey = GetHashCode(account);
    m_mapAccount.erase(hashKey);

    ::ZeroMemory(pAccountInfo, sizeof(*pAccountInfo));
    m_AccountPool.Free(pAccountInfo);

    return TRUE;
}

BOOL
AccountManager::ClearInfo(DWORD frontId)
{
    AccountInfo* pAccountInfo = FindWithLoginFrontUserID(frontId);
    if(!pAccountInfo)
        return FALSE;

    DWORD hashKey = GetHashCode(pAccountInfo->m_account);
    m_mapAccount.erase(hashKey);

    ::ZeroMemory(pAccountInfo, sizeof(*pAccountInfo));
    m_AccountPool.Free(pAccountInfo);

    return TRUE;
}

#ifdef _NA_20120215_RUSSIA_PREMIUM_SERVICE
void
AccountManager::ClearLocation(AGENTID locationId)
{
    QueryManager* const pQueryManager = QueryManager::Instance();

    MapAccount::iterator it = m_mapAccount.begin(),
                        end = m_mapAccount.end();
    while (it != end)
    {
        AccountInfo* account_info = it->second;
        if (account_info == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : account info data is null"
                );
            continue;
        }
        ++it;

        if (account_info->m_pcBangStatus.status_ == CHANGE_PCROOM_STATUS::eStatus_PcRoom)
        {
            IngambaAuthConnector* ingamba_auth_connector = 
                AuthFrame::Instance()->GetIngambaAuthConnector();

            if (ingamba_auth_connector)
            {
                ingamba_auth_connector->StopPrimiumService(*account_info);
            }
        }

        if(account_info->m_locationId == locationId)
        {
            // (NOTE) current state : AgentServer와의 연결이 끊어졌다. 해당 플레이어들은 일괄 끊어질
            // 것이므로 로깅한다.
            // UserSts를 변경한다.
            pQueryManager->QueryChangeUserSts(account_info->m_account, 
                account_info->m_userGuid,
                account_info->m_glogKey,
                account_info->m_locationId,
                account_info->m_clientIp,
                account_info->m_localIp,
                USERSTATUS_LOGOUT, 
                account_info->m_loginTime);

            ClearInfo(account_info->m_account);
        }
    }
}
#else//_NA_20120215_RUSSIA_PREMIUM_SERVICE
void
AccountManager::ClearLocation(AGENTID locationId)
{
#ifdef __NA_20100201_INTERGRATEDBILLING_
    IntegratedBillingServerSession* pBillingSession = (IntegratedBillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#else
    BillingServerSession* pBillingSession = (BillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#endif

    QueryManager* const pQueryManager = QueryManager::Instance();

    MapAccount::iterator it = m_mapAccount.begin(),
                         end = m_mapAccount.end();
    while(it != end)
    {
        AccountInfo* pAccountInfo = it->second;
        if (pAccountInfo == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : account info data is null"
                );
            continue;
        }
        ++it; // pre-action before 'Clear'

        if(pAccountInfo->m_locationId == locationId)
        {
            // 빌링서버에 로그아웃이라고 알려준다
            // (이미 로그아웃 대기중인 경우의 예외처리는 SendLogout()에서 한다)
        #ifdef _BILLING_ONLYPCBANG
            if(pAccountInfo->m_pcBangId)
        #endif
            {
                if(pBillingSession)
                {
                //#ifdef _NEW_BILLING_SYSTEM
                //    pBillingSession->SendLogout(pAccountInfo);
                //#else
                    pBillingSession->SendLogout(*pAccountInfo);
                //#endif
                }
            }

        #ifdef _NET_CAFE // 일본 넷카페 DB에 유저가 로그아웃함을 통보한다
            if(pAccountInfo->m_pcBangId)
            {
                pQueryManager->QueryNetCafeLogInsert(pAccountInfo->m_LoginFrontId,
                                                     pAccountInfo->m_LoginFrontUserId,
                                                     pAccountInfo->m_account,
                                                     pAccountInfo->m_clientIp,
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
                                                     NETCAFE_LOGOUT,
#else
                                                     false,
#endif
                                                     0);
            }
        #endif

            // (NOTE) current state : AgentServer와의 연결이 끊어졌다. 해당 플레이어들은 일괄 끊어질
            // 것이므로 로깅한다.
            // UserSts를 변경한다.
        #if !defined(__NA000000_GLOBAL_AUTH_PROCESS__)
            pQueryManager->QueryChangeUserSts(pAccountInfo->m_account, pAccountInfo->m_userGuid,
                                              pAccountInfo->m_glogKey, pAccountInfo->m_locationId,
                                              pAccountInfo->m_clientIp, pAccountInfo->m_localIp,
                                              USERSTATUS_LOGOUT, pAccountInfo->m_loginTime);
        #elif defined(__NA000000_GLOBAL_AUTH_PROCESS__)
            pQueryManager->Query_GspUpdateUserState(pAccountInfo->m_userGuid,
                                                    pAccountInfo->m_locationId,
                                                    pAccountInfo->m_clientIp,
                                                    false);
        #else
            #error "Impossible Build!! - Reason: Violation Dependency Chain"
        #endif

            ClearInfo(pAccountInfo->m_account);
        }
    }
}
#endif//_NA_20120215_RUSSIA_PREMIUM_SERVICE

//#ifndef _NEW_BILLING_SYSTEM
#ifdef _BILLING_CODE_REFACTORING_

void
AccountManager::MakeBillingList(STLX_VECTOR<AccountInfo*>* stream_list)
{
    int number_of_accounts = (int)m_mapAccount.size();
    if(number_of_accounts == 0)
        return;

    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    BOOLEAN is_changed_billing_control = pAuthFrame->IsChangePcRoomBillingStatus();

    stream_list->reserve(number_of_accounts);

    FOREACH_CONTAINER(const MapAccount::value_type& account, m_mapAccount, MapAccount)
    {
        AccountInfo* account_ptr = account.second;

        // 게임에 들어간 상태가 아니거나
        // (PC방이 아니면 by _BILLING_ONLYPCBANG) 과금하지 말아야 한다
    #ifdef _BILLING_ONLYPCBANG
        if(account_ptr->m_pcBangId == 0)
            continue;
    #endif
    #ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
        if(is_changed_billing_control && account_ptr->m_pcBangStatus.status_ ==
                                         account_ptr->m_pcBangStatus.eStatus_PcRoomExpired)
        { continue; }
    #endif

        if(account_ptr->m_status != ACCOUNTSTATUS_PLAY)
        {
        #ifdef _BILLING_WAITLOGOUT
            if(ACCOUNTSTATUS_WAITLOGOUT == account_ptr->m_status)
            {
                account_ptr->m_status = ACCOUNTSTATUS_OVERTIME;
                SUNLOG(eFULL_LOG,
                       "[MakeBillingList] No ANS_PC_LOGOUT Msg(UserGuid=%u, loc=%u, pcBangId=%u)", 
                       account_ptr->m_userGuid,
                       account_ptr->m_locationId, account_ptr->m_pcBangId);
            }
        #endif
            continue;
        }

        stream_list->push_back(account_ptr);
    }
}

#else //!_BILLING_CODE_REFACTORING_

void
AccountManager::MakeBillingList(std::vector< AccountInfo* >* buffer)
{
    MapAccount::iterator it = m_mapAccount.begin(),
                         end = m_mapAccount.end();

    for( ; it != end; ++it)
    {
        AccountInfo* pAccountInfo = it->second;

    #ifdef _BILLING_ONLYPCBANG
        // 게임에 들어간 상태가 아니거나 PC방이 아니면 과금하지 말아야 한다
        if(pAccountInfo->m_pcBangId == 0)
            continue;
    #else
        // 게임에 들어간 상태가 아니면 과금하지 말아야 한다
    #endif
        if(pAccountInfo->m_status != ACCOUNTSTATUS_PLAY)
        {
    #ifdef _BILLING_WAITLOGOUT
            if(ACCOUNTSTATUS_WAITLOGOUT == pAccountInfo->m_status)
            {
                pAccountInfo->m_status = ACCOUNTSTATUS_OVERTIME;
                SUNLOG(eFULL_LOG,
                       "[MakeBillingList] No ANS_PC_LOGOUT Msg(UserGuid=%u, loc=%u, pcBangId=%u)\n", 
                        pAccountInfo->m_userGuid,
                        pAccountInfo->m_locationId, pAccountInfo->m_pcBangId);
            }
    #endif
            continue;
        }

        buffer->push_back(pAccountInfo);
    }
}

#endif //!_BILLING_CODE_REFACTORING_
//#endif

void
AccountManager::GenerateAuthKey(AccountInfo* info)
{
    InterlockedIncrement((LPLONG)&m_authKeyOffset);
    m_authKeyOffset = m_authKeyOffset % 0x3FFFFFFF;

    info->m_authKey = (m_authKey << 30 | m_authKeyOffset);
}

void
AccountManager::GenerateSerialKey(AccountInfo* info)
{
    DWORD currentTick = GetTickCount();
    for(int i = 0; i < 32; ++i)
    {
        info->m_serialKey[i] = (BYTE)((i + 1) * (info->m_authKey + currentTick));
    }
}

void
AccountManager::DisplayInfo()
{
    int user_num[ACCOUNTSTATUS_MAX];
    ZeroMemory(user_num, sizeof(user_num));

    SUNLOG(eCRITICAL_LOG, "Player Connections : %d", m_mapAccount.size());

    MapAccount::iterator it = m_mapAccount.begin(),
                         end = m_mapAccount.end();

    for( ; it != end; ++it)
    {
        AccountInfo* pAccountInfo = it->second;

        if(pAccountInfo->m_status < ACCOUNTSTATUS_MAX)
            ++user_num[pAccountInfo->m_status];
    }

    SUNLOG(eCRITICAL_LOG,
           "NONE[%d] AUTH[%d] TRYPLAY[%d] PLAY[%d] WAITLOGOUT[%d] OVERTIME[%d]", 
           user_num[ACCOUNTSTATUS_NONE], user_num[ACCOUNTSTATUS_AUTH],
           user_num[ACCOUNTSTATUS_TRYPLAY], user_num[ACCOUNTSTATUS_PLAY],
           user_num[ACCOUNTSTATUS_WAITLOGOUT], user_num[ACCOUNTSTATUS_OVERTIME]);
}

void
AccountManager::DisplayDetailInfo()
{
    static std::string nameListString;
    nameListString.clear();
    int count = 0;

    MapAccount::iterator it = m_mapAccount.begin(),
                         end = m_mapAccount.end();

    for( ; it != end; ++it)
    {
        AccountInfo* pAccountInfo = it->second;

        if(pAccountInfo->m_status == ACCOUNTSTATUS_PLAY)
        {
            nameListString += pAccountInfo->m_account;
            nameListString += "	";
            ++count;
        }

        if(count == 5)
        {
            SUNLOG(eCRITICAL_LOG, "PlayUser : %s", nameListString.c_str());
            count = 0;
            nameListString.clear();
        }
    }

    if(count)
    {
        SUNLOG(eCRITICAL_LOG, "PlayUser : %s", nameListString.c_str());
    }
}


