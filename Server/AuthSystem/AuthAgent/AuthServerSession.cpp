#include "stdafx.h"
#include "./AuthServerSession.h"

#include <AuthProtocol_AS.h>
#include <SunAuth/AuthProtocol_GA.h>

#include "./AuthAgentFrame.h"
#include "./GameAgentManager.h"
#include "./GameAgentServerSession.h"
#include "./GameAgentInfoList.h"

__IMPL_CUSTOMPOOL_PTR(AuthServerSession)

AuthServerSession::AuthServerSession(void)
{

}

AuthServerSession::~AuthServerSession(void)
{

}

class SendGameAgentLoginOperator
{
public:
    SendGameAgentLoginOperator()
        : m_pAuthAgentFrame(AuthAgentFrame::Instance())
    {}
    void operator() (GameAgentServerSession* pSession)
    {
        MSG_AS_AUTH_GAMEAGENT_LOGIN_CMD msg;
        msg.m_ConnectStatus = 1;
        msg.m_GameAgentID = pSession->GetGameAgentID();
        m_pAuthAgentFrame->SendAuthServer(&msg, sizeof(msg));
    }
private:
    AuthAgentFrame* m_pAuthAgentFrame;
};

void
AuthServerSession::OnConnect(BOOL bSuccess, DWORD dwNetworkIndex)
{
    ServerSession::OnConnect(bSuccess, dwNetworkIndex);

    if(bSuccess)
    {
        SendGameAgentLoginOperator op;
        GameAgentManager::Instance()->Foreach(op);
    }
}

void
AuthServerSession::OnRecv(BYTE* pMsg, WORD wSize)
{
    MSG_BASE_INTERNAL* pRecvMsg = (MSG_BASE_INTERNAL*)pMsg;

    switch(pRecvMsg->m_byProtocol)
    {
    case SA_AUTH_AGENT_IP_LIST_NTF: OnSA_AUTH_AGENT_IP_LIST_NTF(pRecvMsg, wSize); break;
    case SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN:OnSA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN(pRecvMsg, wSize); break;
    case SA_AUTH_ASK_PC_LOGOUT_SYN: OnSA_AUTH_ASK_PC_LOGOUT_SYN(pRecvMsg, wSize); break;
    case SA_AUTH_BILLING_ALARM_CMD: OnSA_AUTH_BILLING_ALARM_CMD(pRecvMsg, wSize); break;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    case AU_AUTH_SERVER_SELECT_ACK: OnAU_AUTH_SERVER_SELECT_ACK(pRecvMsg, wSize); break;
#endif
    //__NA000000_SHOP_USERID_50_PASSING_PROC__
    case AU_AUTH_COMMON_QUERY_USER_ID_ACK: OnAU_AUTH_COMMON_QUERY_USER_ID_ACK(pRecvMsg, wSize); break;
    case AU_AUTH_BILLING_CHANGE_CONTROL_SYN: OnAU_AUTH_BILLING_CHANGE_CONTROL_SYN(pRecvMsg, wSize); break;
    default:
        {
            SUNLOG(eCRITICAL_LOG,
                   "[AuthServerSession]Invalid Packet! Category[%u] Protocol[%u] Size[%u]",
                   pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol, wSize);
        }
        break;
    }
}

void
AuthServerSession::OnSA_AUTH_AGENT_IP_LIST_NTF(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SA_AUTH_AGENT_IP_LIST_NTF* pRecvMsg = (MSG_SA_AUTH_AGENT_IP_LIST_NTF*)pMsg;

    GameAgentInfoList* const pGameAgentInfoList = GameAgentInfoList::Instance();
    pGameAgentInfoList->Release();

    for(int i = 0; i < pRecvMsg->m_Count; ++i)
    {
        sGAME_AGENT_INFO& rAgentIpInfo = pRecvMsg->m_AgentIPInfo[i];
        pGameAgentInfoList->AddAgentInfo(&rAgentIpInfo);

        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|GameAgentID=%5u|")
               _T("Port=%5u|PublicIp='%s'|PrivateIp='%s'|"),
               rAgentIpInfo.game_agent_id_, rAgentIpInfo.game_agent_port_,
               rAgentIpInfo.game_agent_public_ip_, rAgentIpInfo.game_agent_private_ip_);
    }

    ReadyState& readyState = AuthAgentFrame::Instance()->GetReadyState();
    readyState.SetReadyState(AGENT_READY_GAMEAGENT_IPLIST, TRUE);
}

void
AuthServerSession::OnSA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN* pRecvMsg = (MSG_SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN*)pMsg;
    static_cast<MSG_SA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN*>(
        reinterpret_cast<MSG_AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN*>(1));

    GameAgentServerSession* pGameAgent =
        GameAgentManager::Instance()->FindAgent(pRecvMsg->m_GameAgentID);
    if(pGameAgent)
    {
        MSG_AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN msg;
        msg = *pRecvMsg;
        msg.m_byProtocol = AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN;
        pGameAgent->SendPacket(&msg, sizeof(msg));
    }
    else
    {
        MSG_AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK msg;
        msg.m_byResult = 1; //  Agent가 연결되지 않음
        msg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
        msg.m_GameAgentID = pRecvMsg->m_GameAgentID;

        CHECK_ID_LENGTH_BUFFER(pRecvMsg->m_szID);
        CHECK_ID_LENGTH_BUFFER(msg.m_szID);
        ::_tcsncpy(msg.m_szID, pRecvMsg->m_szID, _countof(msg.m_szID));
        msg.m_szID[_countof(msg.m_szID) - 1] = '\0';

        SendPacket(&msg, sizeof(msg)); // auth server

        SUNLOG(eCRITICAL_LOG, "[OnSA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN] pGameAgent[%u] is NULL!",
               pRecvMsg->m_GameAgentID);
    }
}

void
AuthServerSession::OnSA_AUTH_ASK_PC_LOGOUT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SA_AUTH_ASK_PC_LOGOUT_SYN* pRecvMsg = (MSG_SA_AUTH_ASK_PC_LOGOUT_SYN*)pMsg;
    static_cast<MSG_SA_AUTH_ASK_PC_LOGOUT_SYN*>(reinterpret_cast<MSG_AG_AUTH_ASK_PC_LOGOUT_SYN*>(1));

    GameAgentServerSession* pGameAgent =
        GameAgentManager::Instance()->FindAgent(pRecvMsg->m_GameAgentID);
    if(pGameAgent)
    {
        MSG_AG_AUTH_ASK_PC_LOGOUT_SYN msg;
        msg = *pRecvMsg;
        msg.m_byProtocol = AG_AUTH_ASK_PC_LOGOUT_SYN;
        pGameAgent->SendPacket(&msg, sizeof(msg));
    }
    else
    {
        MSG_AS_AUTH_ASK_PC_LOGOUT_ACK msg;
        msg.m_dwAuthUserID = pRecvMsg->m_dwAuthUserID;
        msg.m_GameAgentID = pRecvMsg->m_GameAgentID;
        msg.m_byResult = 0;
    #ifdef _NA000000_091005_EXTENDED_LOG_
        ZeroMemory(msg.m_CharName, sizeof(msg.m_CharName));
        msg.m_CharClass = 0;
        msg.m_CharLevel = 0;
    #endif
        SendPacket(&msg, sizeof(msg));

        SUNLOG(eCRITICAL_LOG, "[OnSA_AUTH_ASK_PC_LOGOUT_SYN] pGameAgent[%u] is NULL!",
               pRecvMsg->m_GameAgentID);
    }
}

void
AuthServerSession::OnSA_AUTH_BILLING_ALARM_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_SA_AUTH_BILLING_ALARM_CMD* pRecvMsg = (MSG_SA_AUTH_BILLING_ALARM_CMD*)pMsg;
    static_cast<MSG_SA_AUTH_BILLING_ALARM_CMD*>(reinterpret_cast<MSG_AG_AUTH_BILLING_ALARM_SYN*>(1));

    GameAgentServerSession* pGameAgent =
        GameAgentManager::Instance()->FindAgent(pRecvMsg->m_GameAgentID);
    if(pGameAgent)
    {
        MSG_AG_AUTH_BILLING_ALARM_SYN msg;
        msg = *pRecvMsg;
        msg.m_byProtocol = AG_AUTH_BILLING_ALARM_SYN;
        pGameAgent->SendPacket(&msg, sizeof(msg));
        SUNLOG(eCRITICAL_LOG, "[OnSA_AUTH_BILLING_ALARM_CMD] GameAgentID[%u] AuthUserID[%u]",
               pRecvMsg->m_GameAgentID, pRecvMsg->m_dwAuthUserID);
    }
}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
void
AuthServerSession::OnAU_AUTH_SERVER_SELECT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_AU_AUTH_SERVER_SELECT_ACK* pRecvMsg = static_cast<MSG_AU_AUTH_SERVER_SELECT_ACK*>(pMsg);
    GameAgentServerSession* pGameAgent =
        GameAgentManager::Instance()->FindAgent(pRecvMsg->m_GameAgentID);
    if(pGameAgent == 0)
        return;

    pGameAgent->SendPacket(pRecvMsg, pRecvMsg->GetSize());
    //
}
#endif

void
AuthServerSession::OnAU_AUTH_COMMON_QUERY_USER_ID_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
#if !defined(__NA000000_SHOP_USERID_50_PASSING_PROC__)
    ASSERT(!"Function not supported in current version");
#endif

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    MSG_AU_AUTH_COMMON_QUERY_USER_ID_ACK* pRecvMsg = static_cast<MSG_AU_AUTH_COMMON_QUERY_USER_ID_ACK*>(pMsg);
    GameAgentServerSession* pGameAgent =
        GameAgentManager::Instance()->FindAgent(pRecvMsg->game_agent_id_);
    if(pGameAgent == 0)
        return;

    pGameAgent->SendPacket(pRecvMsg, pRecvMsg->GetSize());
#endif
}


//_PCBANG_POLICY_CHANGE_POST_EXPIRED
void
AuthServerSession::OnAU_AUTH_BILLING_CHANGE_CONTROL_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
#if !defined(_PCBANG_POLICY_CHANGE_POST_EXPIRED)
    ASSERT(!"Function not supported in current version");
#endif

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
    MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN* pRecvMsg = static_cast<MSG_AU_AUTH_BILLING_CHANGE_CONTROL_SYN*>(pMsg);
    GameAgentServerSession* pGameAgent =
        GameAgentManager::Instance()->FindAgent(pRecvMsg->game_agent_id_);
    if(pGameAgent == 0)
        return;

    pGameAgent->SendPacket(pRecvMsg, sizeof(*pRecvMsg));
#endif
}






