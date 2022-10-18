#include "StdAfx.h"

#include ".\BillingServerCommonImpl.h"
#include ".\AccountManager.h"
#include ".\ServerDataManager.h"
#include ".\AuthAgentServerSession.h"
#include <AuthProtocol_AS.h>
#include <AuthProtocol_BS.h>
#include "AuthFrame.h"

#ifdef __NA_20100201_INTERGRATEDBILLING_
#include "IntegratedBillingServerSession.h"
#else
#include "BillingServerSession.h"
#endif

BillingServerCommonImpl::BillingServerCommonImpl(void)
{

}

BillingServerCommonImpl::~BillingServerCommonImpl(void)
{

}

void BillingServerCommonImpl::OnBS_BILLING_RESTPOINT_ACK(DWORD user_guid, DWORD pc_room_id, BOOLEAN is_charged_billing)
{
#if !defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED2)
    ASSERT(!"Current version|protocol not supported");
#endif

#if defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED2)

#if !defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

    AccountInfo* pAccount = AccountManager::Instance()->FindWithUserGuid(user_guid);
    if(pAccount == 0 || pAccount->m_locationId == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_ALRAM_NTF] pAccount[%u] is NULL!",
            user_guid);
        return;
    }

#ifdef __20100916_BILLING_GODLCHANNEL__
    if(is_charged_billing == 0)
    {
        pAccount->m_pcBangId = 0;
        return;
    }
    else
    {
        pAccount->m_pcBangId = 1;
    }
#else
    if(is_charged_billing == 0)
        return;
#endif

    AuthFrame* const pAuthFrame = AuthFrame::Instance();
    const eSERVER_MODE server_mode = pAuthFrame->GetMode();
    BOOLEAN is_changed_billing_control = pAuthFrame->IsChangePcRoomBillingStatus();

    if(is_changed_billing_control == 0)
        return;

#ifdef _NA_005011_20120621_MULTICLIENTS
    ;{
        IntegratedBillingServerSession* pBillingSession = \
            (IntegratedBillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
        if (pBillingSession) {
            if (pBillingSession->CheckMultiUserTypeAndLogin(*pAccount) != true)
                return;
        }
    };
#endif //_NA_005011_20120621_MULTICLIENTS

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

#ifdef __NA_20100201_INTERGRATEDBILLING_
        IntegratedBillingServerSession* pBillingSession = (IntegratedBillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#else
        BillingServerSession* pBillingSession = (BillingServerSession*)AuthFrame::Instance()->GetSession(BILLING_SERVER);
#endif
        if(pBillingSession) pBillingSession->SendLogin(*pAccount);        
        else
        {
            SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_ALRAM_NTF] Not Found BillingSession");
        }

        is_success = true;
    }

    SUNLOG(eCRITICAL_LOG,
        "["__FUNCTION__"]|Re-charged|result=%d|Account=%s|PCRoomID=%u|Status(Prev):(New)=%d:%d|",
        is_success, pAccount->m_account, pAccount->m_pcBangId,
        prev_status.status_, pAccount->m_pcBangStatus.status_);

#endif //defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED2) && defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED)
}

void BillingServerCommonImpl::SendPCRoomStatus(AccountInfo* pAccount)
{
    AuthAgentServerSession* pAuthAgentServerSession =
        ServerDataManager::Instance()->FindAuthAgentServerSession(pAccount->m_locationId);

    MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN msgSYN;
    msgSYN.game_agent_id_ = pAccount->m_locationId;
    msgSYN.status_.status_ = pAccount->m_pcBangStatus.status_;
    msgSYN.pc_room_id_ = pAccount->m_pcBangId;
    msgSYN.user_key_ = pAccount->m_userGuid;
    msgSYN.auth_user_key_ = pAccount->m_authKey;
    pAuthAgentServerSession->SendPacket(&msgSYN, sizeof(msgSYN));
}

void BillingServerCommonImpl::OnBS_BILLING_EXPIRED_NTF(DWORD dwUserGUID, DWORD dwPCRoomGUID, DWORD dwResult, 
                                                       short nAccessCheck, short nDeductType)
{
    AccountInfo* pAccount = AccountManager::Instance()->FindWithUserGuid(dwUserGUID);
    if(pAccount == 0)
    {
        SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_NTF] pAccount[%u] is NULL!",
            dwUserGUID);
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
        SUNLOG(eCRITICAL_LOG, "[OnBS_BILLING_EXPIRED_NTF] Account[%u] PcBang[%u] Result[%u] AccessCheck[%d] DeductType[%d]",
            dwUserGUID, dwPCRoomGUID, dwResult, nAccessCheck, nDeductType);
    }
}