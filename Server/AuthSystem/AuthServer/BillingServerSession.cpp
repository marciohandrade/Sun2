#include "StdAfx.h"

//#ifdef _NEW_BILLING_SYSTEM
//
//#include "BillingServerSession.h"
//#include "AccountManager.h"
//#include <functional>
//#include "AuthFrame.h"
//#include "AuthAgentServerSession.h"
//#include "ServerDataManager.h"
//#include <AuthProtocol_AS.h>
//#include <AuthProtocol_BS.h>
//
//
//BillingServerSession::BillingServerSession(void)
//{
//}
//
//BillingServerSession::~BillingServerSession(void)
//{
//}
//
//VOID
//BillingServerSession::OnConnect(BOOL bSuccess, DWORD dwSessionIndex)
//{
//    ServerSession::OnConnect(bSuccess, dwSessionIndex);
//
//    SetHeaderSizeType(HEADER_SIZE_TOTAL);
//
//    // 새로 연결되면 현재 차감이 필요한 모든 유저의 정보를 다시 로그인 해야한다.
//    if(bSuccess)
//    {
//        AccountManager::Instance()->ForEachAccount(
//            std::bind1st(std::mem_fun(&BillingServerSession::SendLogin), this));
//
//        // 테스트 코드
//        //AccountInfo m_AccountInfo;
//        //m_AccountInfo.userGuid = 1;
//        //m_AccountInfo.pcBangId = 1;
//        //sprintf(m_AccountInfo.clientIp, "10.101.12.59");
//        //m_AccountInfo.status = ACCOUNTSTATUS_PLAY;
//        //SendLogin(&m_AccountInfo);
//        //
//        //m_AccountInfo.userGuid = 2;
//        //SendLogin(&m_AccountInfo);
//
//    }
//}
//
//VOID
//BillingServerSession::OnDisconnect()
//{
//    ServerSession::OnDisconnect();
//
//    m_mapLogoutWaitAccount.clear();
//}
//
//void
//BillingServerSession::OnRecv(BYTE* pMsg, WORD wSize)
//{
//    MSG_BASE_BILLING* pRecvMsg = (MSG_BASE_BILLING*)pMsg;
//
//    switch(pRecvMsg->m_dwProtocolID)
//    {
//    case BS_BILLING_USER_STATUS_NTF: OnBS_BILLING_USER_STATUS_NTF(pRecvMsg, wSize);	break;
//    default:
//        {
//            SUNLOG(eCRITICAL_LOG, "[BillingServerSession::OnRecv] Invalid Packet dwProtocolID[%u]",
//                   pRecvMsg->m_dwProtocolID);
//        }
//        break;
//    }
//}
//
//BOOL
//BillingServerSession::SendPacket(MSG_BASE_BILLING* pMsg, WORD wSize)
//{
//    return Send((BYTE*)pMsg, wSize);
//}
//
//VOID
//BillingServerSession::OnBS_BILLING_USER_STATUS_NTF(MSG_BASE_BILLING* pMsg, WORD wSize)
//{
//    MSG_BS_BILLING_USER_STATUS_NTF* pRecvMsg = (MSG_BS_BILLING_USER_STATUS_NTF*)pMsg;
//
//    // 로그아웃 대기자를 로그아웃 시킨다.
//    AccountInfo* pAccountInfo = FindLogoutWaitAccount(pRecvMsg->m_dwAccountGUID);
//    if(pAccountInfo)
//    {
//        SendLogout(pAccountInfo);
//        SUNLOG(eCRITICAL_LOG, "[BillingServerSession::OnUserStatus] AccountID[%d] SendLogout!! \n",
//               pAccountInfo->m_userGuid);
//        return;
//    }
//
//    if(pRecvMsg->m_sResultCode != 0)
//    {
//        SUNLOG(eCRITICAL_LOG,
//               "[BillingServerSession::OnUserStatus] "
//               "dwAccountGUID(%d) dwBillingGUID(%d) sResultCode(%d) \n",
//                pRecvMsg->m_dwAccountGUID, pRecvMsg->m_dwBillingGUID, pRecvMsg->m_sResultCode);
//        return;
//    }
//
//    pAccountInfo = AccountManager::Instance()->FindWithUserGuid(pRecvMsg->m_dwAccountGUID);
//    if(!pAccountInfo)
//        return;
//
//    pAccountInfo->m_dwBillingGuid = pRecvMsg->m_dwBillingGUID;
//
//    // 만료 처리
//    if(pRecvMsg->m_sDeductType == eDEDUCTIONTYPE_NONE)
//        OnBTOGExpireTermNTF(pAccountInfo);
//}
//
//// 로그인(PC_LOGIN)시 빌링 서버에 알림
//VOID
//BillingServerSession::SendLogin(AccountInfo* pAccountInfo)
//{
//    // 게임에 들어간 상태가 아니면 과금하지 말아야 한다
//    if(pAccountInfo->m_status != ACCOUNTSTATUS_PLAY)
//        return;
//
//    sockaddr_in	addrIn;
//    ZeroMemory(&addrIn, sizeof(addrIn));
//    addrIn.sin_addr.s_addr = ::inet_addr(pAccountInfo->m_clientIp);
//
//    MSG_SB_BILLING_CLIENT_LOGIN_CMD SendMsg;
//    SendMsg.m_dwAccountGUID = pAccountInfo->m_userGuid;
//    SendMsg.m_dwIPAddress = addrIn.sin_addr.S_un.S_addr;
//    SendMsg.m_dwRoomGUID = pAccountInfo->m_pcBangId;
//    SendMsg.m_dwGameCode = BILLING_SUN_CODE;
//    if(AuthFrame::Instance()->GetMode() == eSERVER_TESTLIVE)
//        SendMsg.m_dwServerType = 0;
//    else
//        SendMsg.m_dwServerType = 1;
//
//    SendPacket(&SendMsg, sizeof(SendMsg));
//}
//
//VOID
//BillingServerSession::SendLogout(AccountInfo* pAccountInfo)
//{
//    // 로그인은 했지만 아직 OnUserStatus 이벤트를 받기 전에 로그아웃을 하려 한다면?
//    if(pAccountInfo->m_dwBillingGuid == 0)
//    {
//        m_mapLogoutWaitAccount.insert(ACCOUNTINFO_MAP::value_type(pAccountInfo->m_userGuid, pAccountInfo));
//        SUNLOG(eCRITICAL_LOG,
//               "[BillingServerSession::SendLogout] dwBillingGuid is 0!! AccountID[%d] \n",
//               pAccountInfo->m_userGuid);
//        return;
//    }
//
//    MSG_SB_BILLING_CLIENT_LOGOUT_CMD SendMsg;
//    SendMsg.m_dwBillingGUID = pAccountInfo->m_dwBillingGuid;
//    SendPacket(&SendMsg, sizeof(SendMsg));
//}
//
//VOID
//BillingServerSession::OnBTOGExpireTermNTF(AccountInfo* pAccountInfo)
//{
//    AuthAgentServerSession* pAuthAgentSession =
//        ServerDataManager::Instance()->FindAuthAgentServerSession(pAccountInfo->m_locationId);
//    if(!pAuthAgentSession)
//        return;
//
//    if(AuthFrame::Instance()->GetMode() != eSERVER_LIVE)
//        return;
//
//    //  과금만료 사용자 접속종료 요청
//    MSG_SA_AUTH_ASK_PC_LOGOUT_SYN sendLogout;
//    sendLogout.m_dwAuthUserID = pAccountInfo->m_authKey;
//    sendLogout.m_GameAgentID = (WORD)pAccountInfo->m_locationId;
//    sendLogout.m_dwUserGUID = pAccountInfo->m_userGuid;
//    CHECK_ID_LENGTH_BUFFER(pAccountInfo->m_account);
//    CHECK_ID_LENGTH_BUFFER(sendLogout.m_szID);
//    ::_tcsncpy(sendLogout.m_szID, pAccountInfo->m_account, _countof(sendLogout.m_szID));
//    sendLogout.m_szID[_countof(sendLogout.m_szID) - 1] = '\0';
//
//    sendLogout.m_dwLogoutCode = eDRC_BILLING_EXPIRE;
//    pAuthAgentSession->SendPacket(&sendLogout, sizeof(sendLogout));
//
//    // 해당 유저의 정보를 로그아웃 대기 상태로 변경
//    pAccountInfo->m_status = ACCOUNTSTATUS_WAITLOGOUT;
//
//    SUNLOG(eCRITICAL_LOG, "[OnBTOGExpireTermNTF] UserGuid = %d, PcBang = %d \n", pAccountInfo->m_userGuid, pAccountInfo->m_pcBangId);
//}
//
//// 차감 만료 임박했음을 알림
//VOID
//BillingServerSession::OnBTOGAlarmExpireTermNTF(AccountInfo* pAccountInfo, double dRestTime)
//{
//    AuthAgentServerSession* pAuthAgentSession = ServerDataManager::Instance()->FindAuthAgentServerSession(pAccountInfo->m_locationId);
//    if(!pAuthAgentSession)
//        return;
//
//    if(AuthFrame::Instance()->GetMode() != eSERVER_LIVE)
//        return;
//
//    MSG_SA_AUTH_BILLING_ALARM_CMD sendPacket;
//    sendPacket.m_dwAuthUserID = pAccountInfo->m_authKey;
//    sendPacket.m_GameAgentID = (WORD)pAccountInfo->m_locationId;
//    sendPacket.m_dwRestTerm = (DWORD)dRestTime;
//    pAuthAgentSession->SendPacket(&sendPacket, sizeof(sendPacket));
//
//    SUNLOG(eCRITICAL_LOG,
//           "[OnBTOGAlarmExpireTermNTF] UserGuid = %d, PcBang = %d, RestTime = %d \n",
//           pAccountInfo->m_userGuid, pAccountInfo->m_pcBangId, dRestTime);
//}
//
//AccountInfo*
//BillingServerSession::FindLogoutWaitAccount(DWORD userGuid)
//{
//    ACCOUNTINFO_MAP::iterator FindIt = m_mapLogoutWaitAccount.find(userGuid);
//    if(FindIt != m_mapLogoutWaitAccount.end())
//        return FindIt->second;
//
//    return NULL;
//}
//
//#else //!_NEW_BILLING_SYSTEM

#include ".\BillingServerSession.h"
#include ".\AccountManager.h"
#include ".\ServerDataManager.h"
#include ".\AuthAgentServerSession.h"
#include <AuthProtocol_AS.h>
#include <AuthProtocol_BS.h>
#include "AuthFrame.h"


BillingServerSession::BillingServerSession(void)
{
}

BillingServerSession::~BillingServerSession(void)
{
}


void
BillingServerSession::OnConnect(BOOL bSuccess, DWORD dwNetworkIndex)
{
    ServerSession::OnConnect(bSuccess, dwNetworkIndex);

    SetHeaderSizeType(HEADER_SIZE_TOTAL);

    if(bSuccess)
    {
        m_BillingTimer.SetTimer(BILLING_INFO_SEND_DELAY_TIME);
    }
}

BOOL
BillingServerSession::SendPacket(MSG_BASE_BILLING* pMsg, WORD wSize)
{
    return Send((BYTE*)pMsg, wSize);
}

void
BillingServerSession::OnDisconnect()
{
    m_BillingTimer.DisableCheckTime();
    // (WAVERIX) (090610) (BUG-FIX) insert 'OnDisconnect'
    ServerSession::OnDisconnect();
}

void
BillingServerSession::OnRecv(BYTE* pMsg, WORD wSize)
{
    MSG_BASE_BILLING* pRecvMsg = (MSG_BASE_BILLING*)pMsg;

    switch(pRecvMsg->m_dwProtocolID)
    {
    case BS_BILLING_EXPIRED_NTF:       OnBS_BILLING_EXPIRED_NTF(pRecvMsg, wSize);  break;
    case BS_BILLING_EXPIRED_ALRAM_NTF: OnBS_BILLING_EXPIRED_ALRAM_NTF(pRecvMsg, wSize); break;
    //_PCBANG_POLICY_CHANGE_POST_EXPIRED2
    case BS_BILLING_RESTPOINT_ACK:     OnBS_BILLING_RESTPOINT_ACK(pRecvMsg, wSize); break;
    default:
        {
            SUNLOG(eCRITICAL_LOG, "[BillingServerSession::OnRecv]Invalid Packet[%u]!",
                   pRecvMsg->m_dwProtocolID);
        }
        break;
    }
}

void
BillingServerSession::Update()
{
    ServerSession::Update();

    // 한번에 최대 3천개씩 전송한다.
#ifdef _BILLING_CODE_REFACTORING_
    if(m_AccountInfoPendings.empty() == false)
        SendDeduction();
#else
    if(m_vectAccountInfo.empty() == false)
        SendDeduction();
#endif

    if(m_BillingTimer.IsExpired())
        MakeList();
}

void
BillingServerSession::MakeList()
{
#ifdef _BILLING_CODE_REFACTORING_
    AccountManager* pAccountManager = AccountManager::Instance();

    DWORD current_tick = ::GetTickCount();

    typedef STLX_VECTOR<AccountInfo*> AccountInfoStreamList;
    AccountInfoStreamList current_list;
    pAccountManager->MakeBillingList(&current_list);

    FOREACH_CONTAINER(const AccountInfoStreamList::value_type account, current_list,
                      AccountInfoStreamList)
    {
        AccountInfo* dest = 0;
        if(m_AccountInfoPool.empty()) {
            dest = new AccountInfo;
        } else {
            dest = m_AccountInfoPool.front();
            m_AccountInfoPool.pop_front();
        }

        *dest = *account;
        // account은 AccountManager에서 관리하는 AccountInfo 객체
        // 이 값의 tickBillingTime을 초기화해 주어야 한다.
        account->m_tickBillingTime = current_tick;
        m_AccountInfoPendings.push_back(dest);
    }

#else //!_BILLING_CODE_REFACTORING_
    VectAccountInfo vect;
    VectAccountInfo::iterator iter;
    AccountInfo *info,
                *item;

    //  m_vectAccountInfo값을 clear하지 않으므로, 아직 처리되지 않고 남아있는 계정 목록이
    //  있다면 새로운 목록이 뒤에 추가된다.
    AccountManager::Instance()->MakeBillingList(&vect);

    for(iter = vect.begin() ; iter != vect.end() ; iter++)
    {
        item = *iter;

        if(m_poolAccountInfo.size() == 0)
            info = new AccountInfo();
        else
        {
            info = m_poolAccountInfo.front();
            m_poolAccountInfo.pop();
        }

        ::CopyMemory(info, item, sizeof(AccountInfo));

        //  item은 AccountManager에서 관리하는 AccountInfo 포인터이며,
        //  이 값의 tickBillingTime을 초기화해주어야 한다.
        item->m_tickBillingTime = ::GetTickCount();

        m_vectAccountInfo.push_back(info);
    }
#endif //!_BILLING_CODE_REFACTORING_
}

void
BillingServerSession::SendDeduction()
{
#ifdef _BILLING_CODE_REFACTORING_
    static MSG_SB_BILLING_DEDUCTION_CMD msgCMD;
    if(m_AccountInfoPendings.empty())
        return;

    msgCMD.Clear();

    // (note) algorithm 개선 할 것. 시간상 간단 정리 후 넘어간다.
    BILLING_DEDUCTION_INFO* dest = msgCMD.m_DeductionInfoList;
    assert(msgCMD.m_Count == 0);

    // 중간에 멤버등이 바뀌는 상황 detection을 위함.
    BOOST_STATIC_ASSERT(sizeof(*dest) == (sizeof(DWORD) * 5));

    DWORD current_tick = ::GetTickCount();
    DWORD& count = msgCMD.m_Count;
    for(AccountInfoQueue::iterator it = m_AccountInfoPendings.begin();
        it != m_AccountInfoPendings.end();
        ++dest)
    {
        AccountInfo* account = *it;

        DWORD ip_value = ::inet_addr(account->m_clientIp);

        dest->m_dwGameCode   = BILLING_SUN_CODE;
        dest->m_dwPCRoomGUID = account->m_pcBangId;
        dest->m_dwPlayTime   = (current_tick - account->m_tickBillingTime) / 1000;
        dest->m_dwUserGUID   = account->m_userGuid;
        dest->m_dwIPAddress  = ip_value;

        // (note) insert ranges, delete ranges로 변형할 것.
        m_AccountInfoPool.push_back(account); // restore pool node
        it = m_AccountInfoPendings.erase(it);

        ++count;

        if(count == MSG_SB_BILLING_DEDUCTION_CMD::MAX_DEDUCTION_SIZE)
            break;
    }

    // 한꺼번에 보내는 상황을 줄이기 위함... 나름 나쁘지 않는 방법이긴 한데...
    SendPacket(&msgCMD, msgCMD.GetSize());

#else //!_BILLING_CODE_REFACTORING_

    MSG_SB_BILLING_DEDUCTION_CMD msg;
    msg.m_Count = 0;
    VectAccountInfo::iterator iter;

    while(m_vectAccountInfo.size() > 0)
    {
        iter = m_vectAccountInfo.begin();

        AccountInfo* info = *iter;
        sockaddr_in addrIn;

        ZeroMemory(&addrIn, sizeof(addrIn));
        addrIn.sin_addr.s_addr = ::inet_addr(info->m_clientIp);

        BILLING_DEDUCTION_INFO& deductionInfo = msg.m_DeductionInfoList[msg.m_Count];
        deductionInfo.m_dwGameCode   = BILLING_SUN_CODE;
        deductionInfo.m_dwPCRoomGUID = info->m_pcBangId;
        deductionInfo.m_dwPlayTime   = (::GetTickCount() - info->m_tickBillingTime) / 1000;
        deductionInfo.m_dwUserGUID   = info->m_userGuid;
        deductionInfo.m_dwIPAddress  = addrIn.sin_addr.S_un.S_addr;
        m_poolAccountInfo.push(info);
        m_vectAccountInfo.erase(iter);

        if(++msg.m_Count >= MSG_SB_BILLING_DEDUCTION_CMD::MAX_DEDUCTION_SIZE)
            break;
    }

    SendPacket(&msg, msg.GetSize());

#endif //!_BILLING_CODE_REFACTORING_
}

// 로그인(PC_LOGIN)시 빌링 서버에 알림
void
BillingServerSession::SendLogin(AccountInfo& info)
{
    MSG_SB_BILLING_USER_LOGIN_CMD msg;

    msg.m_dwResult = 0;
    msg.m_dwGameCode   = BILLING_SUN_CODE;
    msg.m_dwUserGUID   = info.m_userGuid;
    msg.m_dwPCRoomGUID = info.m_pcBangId;
    msg.m_dwIPAddress  = ::inet_addr(info.m_clientIp);
    msg.m_dwPlayTime   = 0;
    SendPacket(&msg, sizeof(msg));
}

void
BillingServerSession::SendLogout(AccountInfo& info)
{
    MSG_SB_BILLING_USER_LOGOUT_CMD msg;

    msg.m_dwResult = 0;
    msg.m_dwGameCode   = BILLING_SUN_CODE;
    msg.m_dwUserGUID   = info.m_userGuid;
    msg.m_dwPCRoomGUID = info.m_pcBangId;
    msg.m_dwIPAddress  = ::inet_addr(info.m_clientIp);
    msg.m_dwPlayTime   = (::GetTickCount() - info.m_tickBillingTime) / 1000;
#ifdef _BILLING_WAITLOGOUT
    if(ACCOUNTSTATUS_OVERTIME == info.m_status)
    {
        // 로그아웃 요청이 올 시간이 지난 경우 지나친 차감을 막기 위해 빌링 차감 1 주기만큼만 차감한다(오차 발생할 수는 있다)
        msg.m_dwPlayTime = BILLING_INFO_SEND_DELAY_TIME / 1000;
        SUNLOG(eCRITICAL_LOG,
               "[BillingServerSession::SendLogout] OverTime occurred! "
               "Account[%s] GameAgentID[%u] PCbang[%u] AuthStatus[%u]",
               info.m_account, info.m_locationId, info.m_pcBangId, info.m_status);
    }
#endif

    SendPacket(&msg, sizeof(msg));
}


void
BillingServerSession::QueryRestPoint(AccountInfo* account)
{
#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED2
    MSG_BS_BILLING_RESTPOINT_SYN msgSYN;
    msgSYN.game_code_ = BILLING_SUN_CODE;
    msgSYN.user_guid_ = account->m_userGuid;
    msgSYN.pc_room_id_ = account->m_pcBangId;
    SendPacket(&msgSYN, sizeof(msgSYN));
#endif
}

void
BillingServerSession::OnBS_BILLING_EXPIRED_NTF(MSG_BASE_BILLING* pMsg, WORD size)
{
    MSG_BS_BILLING_EXPIRED_NTF* pRecvMsg = (MSG_BS_BILLING_EXPIRED_NTF*)pMsg;
    BillingCommonImpl.OnBS_BILLING_EXPIRED_NTF(pRecvMsg->m_dwUserGUID, pRecvMsg->m_dwPCRoomGUID, pRecvMsg->m_dwResult);
/*
    MSG_BS_BILLING_EXPIRED_NTF* pRecvMsg = (MSG_BS_BILLING_EXPIRED_NTF*)pMsg;

    AccountInfo* pAccount = AccountManager::Instance()->FindWithUserGuid(pRecvMsg->m_dwUserGUID);
    if(pAccount == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_NTF] pAccount[%u] is NULL!",
               pRecvMsg->m_dwUserGUID);
        return;
    }

    // (WAVERIX) (090904) (BUG-FIX) 정량 만료 정책은 '라이브/라이트테스트' 둘 모두 동일하다.
    // 기존 처리 '라이브'모드에서만 만료에 대한 종료 처리를 수행하고 있었다.

    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    const eSERVER_MODE server_mode = pAuthFrame->GetMode();
    BOOLEAN is_changed_billing_control = pAuthFrame->IsChangePcRoomBillingStatus();

#if defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED) && defined(_BILLING_DISCONNECT)
    //  과금만료 사용자 PC방 정책 변경 처리를 수행한다.
    AuthAgentServerSession* pAuthAgentServerSession =
        ServerDataManager::Instance()->FindAuthAgentServerSession(pAccount->m_locationId);
    if(pAuthAgentServerSession && (server_mode == eSERVER_LIVE ||
                                   server_mode == eSERVER_TESTLIVE))
    {
        if(is_changed_billing_control)
        {
            MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN msgSYN;
            msgSYN.game_agent_id_ = pAccount->m_locationId;
            msgSYN.status_.status_ = msgSYN.status_.eStatus_PcRoomExpired;
            msgSYN.pc_room_id_ = pAccount->m_pcBangId;
            msgSYN.user_key_ = pAccount->m_userGuid;
            msgSYN.auth_user_key_ = pAccount->m_authKey;
            pAuthAgentServerSession->SendPacket(&msgSYN, sizeof(msgSYN));

            // (CHANGES) (WAVERIX) (090825) (AUTH) AccountInfo에도 반영해 둔다.
            // 기존 상태 제어가 'm_status'에 의해 이뤄졌는데, 만료 상태에서 차감 처리는 넘길 필요 있다.
            pAccount->m_pcBangStatus.status_ = pAccount->m_pcBangStatus.eStatus_PcRoomExpired;

            //_PCBANG_POLICY_CHANGE_POST_EXPIRED2
            // (CHANGES) (WAVERIX) (091013) (AUTH) Query interval for get recharge info. from billing
            DWORD current_tick = GetTickCount();
            pAccount->m_pcBangOnRechargeQueryTime =
                current_tick + AccountManager::PC_ROOM_RECHARGE_QUERY_INTERVAL;
        }
        else
        { // 과금만료 사용자 접속종료 요청
            MSG_SA_AUTH_ASK_PC_LOGOUT_SYN sendLogout;
            sendLogout.m_dwAuthUserID = pAccount->m_authKey;
            sendLogout.m_GameAgentID = (WORD)pAccount->m_locationId;
            sendLogout.m_dwUserGUID = pAccount->m_userGuid;
            ::_tcsncpy(sendLogout.m_szID, pAccount->m_account, _countof(sendLogout.m_szID));
            CHECK_ID_LENGTH_BUFFER(pAccount->m_account);
            CHECK_ID_LENGTH_BUFFER(sendLogout.m_szID);
            sendLogout.m_szID[_countof(sendLogout.m_szID) - 1] = '\0';

            sendLogout.m_dwLogoutCode = eDRC_BILLING_EXPIRE;
            pAuthAgentServerSession->SendPacket(&sendLogout, sizeof(sendLogout));

            // 해당 유저의 정보를 로그아웃 대기 상태로 변경
            pAccount->m_status = ACCOUNTSTATUS_WAITLOGOUT;
        }
    }

#else //!_PCBANG_POLICY_CHANGE_POST_EXPIRED
    #ifdef _BILLING_DISCONNECT
        //  과금만료 사용자 접속종료 요청
        AuthAgentServerSession* pAuthAgentServerSession =
            ServerDataManager::Instance()->FindAuthAgentServerSession(pAccount->m_locationId);
        if(pAuthAgentServerSession && (server_mode == eSERVER_LIVE ||
                                       server_mode == eSERVER_TESTLIVE))
        {
            MSG_SA_AUTH_ASK_PC_LOGOUT_SYN sendLogout;
            sendLogout.m_dwAuthUserID = pAccount->m_authKey;
            sendLogout.m_GameAgentID = (WORD)pAccount->m_locationId;
            sendLogout.m_dwUserGUID = pAccount->m_userGuid;
            ::_tcsncpy(sendLogout.m_szID, pAccount->m_account, _countof(sendLogout.m_szID));
            CHECK_ID_LENGTH_BUFFER(pAccount->m_account);
            CHECK_ID_LENGTH_BUFFER(sendLogout.m_szID);
            sendLogout.m_szID[_countof(sendLogout.m_szID) - 1] = '\0';

            sendLogout.m_dwLogoutCode = eDRC_BILLING_EXPIRE;
            pAuthAgentServerSession->SendPacket(&sendLogout, sizeof(sendLogout));

            // 해당 유저의 정보를 로그아웃 대기 상태로 변경
            pAccount->m_status = ACCOUNTSTATUS_WAITLOGOUT;
        }
    #endif
#endif //!_PCBANG_POLICY_CHANGE_POST_EXPIRED

    if(server_mode == eSERVER_LIVE || server_mode == eSERVER_TESTLIVE)
    {
        SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_NTF] Account[%u] PcBang[%u] Result[%u]",
               pRecvMsg->m_dwUserGUID, pRecvMsg->m_dwPCRoomGUID, pRecvMsg->m_dwResult);
    }
*/
}

// 차감 만료 임박했음을 알림
void
BillingServerSession::OnBS_BILLING_EXPIRED_ALRAM_NTF(MSG_BASE_BILLING* pMsg, WORD size)
{
    MSG_BS_BILLING_EXPIRED_ALRAM_NTF* pRecvMsg = (MSG_BS_BILLING_EXPIRED_ALRAM_NTF*)pMsg;

    AccountInfo* pAccount = AccountManager::Instance()->FindWithUserGuid(pRecvMsg->m_dwUserGUID);
    if(pAccount == 0 || pAccount->m_locationId == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_ALRAM_NTF] pAccount[%u] is NULL!",
               pRecvMsg->m_dwUserGUID);
        return;
    }

    const eSERVER_MODE server_mode = AuthFrame::Instance()->GetMode();

#ifdef _BILLING_DISCONNECT
    AuthAgentServerSession* pAuthAgentServerSession =
        ServerDataManager::Instance()->FindAuthAgentServerSession(pAccount->m_locationId);
    if(pAuthAgentServerSession /*&& server_mode == eSERVER_LIVE*/)
    {
        MSG_SA_AUTH_BILLING_ALARM_CMD sendPacket;
        sendPacket.m_dwAuthUserID = pAccount->m_authKey;
        sendPacket.m_GameAgentID = (WORD)pAccount->m_locationId;
        sendPacket.m_dwRestTerm	= pRecvMsg->m_dwRestTime;
        pAuthAgentServerSession->SendPacket(&sendPacket, sizeof(sendPacket));
    }
#endif

    /*if(server_mode == eSERVER_LIVE)*/
    {
        SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_ALRAM_NTF] Account[%u] PcBang[%u] RestTime[%u]",
               pRecvMsg->m_dwUserGUID, pRecvMsg->m_dwPCRoomGUID, pRecvMsg->m_dwRestTime);
    }
}


void
BillingServerSession::OnBS_BILLING_RESTPOINT_ACK(MSG_BASE_BILLING* pMsg, WORD size)
{
    const MSG_BS_BILLING_RESTPOINT_ACK* pRecvMsg = static_cast<MSG_BS_BILLING_RESTPOINT_ACK*>(pMsg);
    BillingCommonImpl.OnBS_BILLING_RESTPOINT_ACK(pRecvMsg->user_guid_, pRecvMsg->pc_room_id_, !!pRecvMsg->rest_point_);
    /*
#if !defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED2)
    ASSERT(!"Current version|protocol not supported");
#endif

#if defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED2)

  #if !defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif

    // fire on result of QueryRestPoint(protocol)
    const MSG_BS_BILLING_RESTPOINT_ACK* pRecvMsg = static_cast<MSG_BS_BILLING_RESTPOINT_ACK*>(pMsg);
    AccountInfo* pAccount = AccountManager::Instance()->FindWithUserGuid(pRecvMsg->user_guid_);
    if(pAccount == 0 || pAccount->m_locationId == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_ALRAM_NTF] pAccount[%u] is NULL!",
               pRecvMsg->user_guid_);
        return;
    }

    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    const eSERVER_MODE server_mode = pAuthFrame->GetMode();
    BOOLEAN is_changed_billing_control = pAuthFrame->IsChangePcRoomBillingStatus();

    if(is_changed_billing_control == 0)
        return;

    BOOLEAN is_charged_billing = !!pRecvMsg->rest_point_;
    if(is_charged_billing == 0)
        return;

    AuthAgentServerSession* pAuthAgentServerSession =
        ServerDataManager::Instance()->FindAuthAgentServerSession(pAccount->m_locationId);

    BOOLEAN is_success = false;
    CHANGE_PCROOM_STATUS prev_status = pAccount->m_pcBangStatus;
    if(pAuthAgentServerSession) {
        // recharged|change status, auth & game
        MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN msgSYN;
        msgSYN.game_agent_id_ = pAccount->m_locationId;
        msgSYN.status_.status_ = msgSYN.status_.eStatus_PcRoom;
        msgSYN.pc_room_id_ = pAccount->m_pcBangId;
        msgSYN.user_key_ = pAccount->m_userGuid;
        msgSYN.auth_user_key_ = pAccount->m_authKey;
        pAuthAgentServerSession->SendPacket(&msgSYN, sizeof(msgSYN));

        // change to deduction user
        // changed state : eStatus_PcRoomExpired -> eStatus_PcRoom
        ASSERT(pAccount->m_pcBangStatus.status_ == pAccount->m_pcBangStatus.eStatus_PcRoomExpired);
        pAccount->m_pcBangStatus.status_ = pAccount->m_pcBangStatus.eStatus_PcRoom;
        this->SendLogin(*pAccount);

        is_success = true;
    }

    SUNLOG(eCRITICAL_LOG,
           "["__FUNCTION__"]|Re-charged|result=%d|Account=%s|PCRoomID=%u|Status(Prev):(New)=%d:%d|",
           is_success, pAccount->m_account, pAccount->m_pcBangId,
           prev_status.status_, pAccount->m_pcBangStatus.status_);

#endif //defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED2) && defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED)
           */
}

//#endif //!_NEW_BILLING_SYSTEM






