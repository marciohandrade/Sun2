#include "StdAfx.h"
#include ".\rttaserversession.h"
#include ".\QueryManager.h"
#include ".\AccountManager.h"
#include ".\ServerDataManager.h"
#include ".\AuthAgentServerSession.h"
#include <AuthProtocol_AS.h>
#include <AuthProtocol_RS.h>


RTTAServerSession::RTTAServerSession(void)
{
}

RTTAServerSession::~RTTAServerSession(void)
{
}

void
RTTAServerSession::OnConnect(BOOL bSuccess, DWORD dwNetworkIndex)
{
    ServerSession::OnConnect(bSuccess, dwNetworkIndex);
    if(bSuccess)
    {
        SendCertifyReq();
    }
}

void
RTTAServerSession::SendCertifyReq()
{
    MSG_SR_RTT_CERTIFY_SYN sendPacket;
    sendPacket.m_dwServerType = RTT_AUTHSERVER;
    sendPacket.m_dwVersion = 0;
    SendPacket(&sendPacket, sizeof(sendPacket));
}

void
RTTAServerSession::OnRecv(BYTE* pMsg, WORD wSize)
{
    MSG_BASE_INTERNAL* pRecvMsg = (MSG_BASE_INTERNAL*)pMsg;
    SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pRecvMsg, wSize);

    switch(pRecvMsg->m_byProtocol)
    {
    case RS_RTT_CERTIFY_ACK:                    OnRS_RTT_CERTIFY_ACK(pRecvMsg, wSize);  break;
    case RS_RTT_ASK_PC_LOGOUT_SYN:              OnRS_RTT_ASK_PC_LOGOUT_SYN(pRecvMsg, wSize);    break;
    case RS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN:  OnRS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN(pRecvMsg, wSize); break;
    case RS_RTT_ASK_GAME_VERSION_RELOAD_SYN:    OnRS_RTT_ASK_GAME_VERSION_RELOAD_SYN(pRecvMsg, wSize); break;
    default:
        SUNLOG(eCRITICAL_LOG, "[RTTAServerSession] Invalid Packet! Category[%u] Protocol[%u] Size[%u]", pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol, wSize);
        break;
    }
}

void
RTTAServerSession::OnRS_RTT_CERTIFY_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_RS_RTT_CERTIFY_ACK*	pRecvMsg = (MSG_RS_RTT_CERTIFY_ACK*)pMsg;
    BOOL bSuccess = pRecvMsg->m_dwResult == 0;

    SUNLOG(eCRITICAL_LOG, "[OnRS_RTT_CERTIFY_ACK] ServerGUID[%u] Success[%u]!",
           pRecvMsg->m_dwServerGUID, bSuccess);
}

void
RTTAServerSession::OnRS_RTT_ASK_PC_LOGOUT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_RS_RTT_ASK_PC_LOGOUT_SYN* pRecvMsg = (MSG_RS_RTT_ASK_PC_LOGOUT_SYN*)pMsg;

    MSG_SR_RTT_ASK_PC_LOGOUT_ACK sendResult;
    CHECK_ID_LENGTH_BUFFER(sendResult.m_szUserID);
    CHECK_ID_LENGTH_BUFFER(pRecvMsg->m_szUserID);
    ::_tcsncpy(sendResult.m_szUserID, pRecvMsg->m_szUserID, _countof(sendResult.m_szUserID));
    sendResult.m_szUserID[_countof(sendResult.m_szUserID) - 1] = '\0';

#ifdef __NA000000_090430_ADDIN_DROP_CODE__
    sendResult.m_dwDropReason = pRecvMsg->m_dwDropReason;
#endif
    sendResult.m_dwError = 0;

    //  해당 계정의 정보를 찾는다.
    ::_tcsupr(pRecvMsg->m_szUserID);
    AccountInfo* pAccount = AccountManager::Instance()->Find(pRecvMsg->m_szUserID);
    if(pAccount == 0 || pAccount->m_locationId == 0)
    {
        sendResult.m_dwError = 1;
        // (WAVERIX) (090525) (BUG-FIX) fix null problem in 'AuthServer'
        AGENTID agent_id = pAccount == 0 ? 0 : pAccount->m_locationId;
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] UserID {%s} or GameAgentID[%u] is invalid",
               pRecvMsg->m_szUserID, agent_id);
        SendPacket(&sendResult, sizeof(sendResult));
        return;
    }

    //  해당 계정이 접속중인 GameAgent를 찾는다.
    AuthAgentServerSession* pAuthAgentSession =
        ServerDataManager::Instance()->FindAuthAgentServerSession(pAccount->m_locationId);
    if(!pAuthAgentSession)
    {
        sendResult.m_dwError = 2;
        SUNLOG(eCRITICAL_LOG, "[OnRS_RTT_ASK_PC_LOGOUT_SYN] pAuthAgentSession[%u] is NULL!",
               pAccount->m_locationId);
        SendPacket(&sendResult, sizeof(sendResult));
        return;
    }

    // PC 로그아웃을 요청한다.
    MSG_SA_AUTH_ASK_PC_LOGOUT_SYN logoutPacket;
    logoutPacket.m_dwAuthUserID = pAccount->m_authKey;
    logoutPacket.m_GameAgentID = (WORD)pAccount->m_locationId;
    logoutPacket.m_dwUserGUID = pAccount->m_userGuid;
    CHECK_ID_LENGTH_BUFFER(logoutPacket.m_szID);
    CHECK_ID_LENGTH_BUFFER(pAccount->m_account);
    ::_tcsncpy(logoutPacket.m_szID, pAccount->m_account, _countof(logoutPacket.m_szID));
    logoutPacket.m_szID[_countof(logoutPacket.m_szID) - 1] = '\0';
    logoutPacket.m_dwLogoutCode = pRecvMsg->m_dwDropReason;
    pAuthAgentSession->SendPacket(&logoutPacket, sizeof(logoutPacket));

    // GameAgent에서 응답을 주지 않기때문에, 일단 임시방편으로 여기에서 결과를 보낸다.
    sendResult.m_dwError = 0;
    SUNLOG(eCRITICAL_LOG, "[OnRS_RTT_ASK_PC_LOGOUT_SYN] Account[%s] DropReason[%u]",
           pRecvMsg->m_szUserID, pRecvMsg->m_dwDropReason);
    SendPacket(&sendResult, sizeof(sendResult));
}

void
RTTAServerSession::OnRS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_RS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN* pRecvMsg =
        (MSG_RS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN*)pMsg;

    //  Reload 작업 수행
    QueryManager::Instance()->QueryControlIpList();

    //  일단 결과는 무조건 OK
    MSG_SR_RTT_ASK_CONTROL_IPLIST_RELOAD_ACK sendPacket;
    sendPacket.m_dwResult	= 1;
    sendPacket.m_dwWopsKey = pRecvMsg->m_dwWopsKey;
    SendPacket(&sendPacket, sizeof(sendPacket));

    SUNLOG(eCRITICAL_LOG, "[OnRS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN] Success!");
}

void
RTTAServerSession::OnRS_RTT_ASK_GAME_VERSION_RELOAD_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_RS_RTT_ASK_GAME_VERSION_RELOAD_SYN* pRecvMsg =
        (MSG_RS_RTT_ASK_GAME_VERSION_RELOAD_SYN*)pMsg;

    //  Reload 작업 수행
    QueryManager::Instance()->QueryGameVersion();

    //  일단 결과는 무조건 OK.
    MSG_SR_RTT_ASK_GAME_VERSION_RELOAD_ACK sendPacket;
    sendPacket.m_dwResult = 1;
    sendPacket.m_dwWopsKey = pRecvMsg->m_dwWopsKey;
    SendPacket(&sendPacket, sizeof(sendPacket));

    SUNLOG(eCRITICAL_LOG, "[OnRS_RTT_ASK_GAME_VERSION_RELOAD_SYN] Success!");
}







