#include "stdafx.h"
#include ".\GameAgentServerSession.h"

#include <AuthProtocol_FS.h>
#include <AuthProtocol_AS.h>
#include <SunAuth/AuthProtocol_GA.h>

#include ".\AuthAgentFrame.h"
#include ".\GameAgentManager.h"
#include ".\GameAgentInfoList.h"


__IMPL_CUSTOMPOOL_PTR(GameAgentServerSession);
GameAgentServerSession::GameAgentServerSession()
    : game_agent_id_(0)
{

}

GameAgentServerSession::~GameAgentServerSession()
{

}

// CHANGES: f101030.1L, moved from 'OnAccept' redirect logic
void
GameAgentServerSession::AuthenticateConnectedAgentServer(ulong ip_address, uint16_t listen_port)
{
    const sGAME_AGENT_INFO* agent_info = \
        GameAgentInfoList::Instance()->FindAgentInfoForPrivateIPAddress(ip_address, listen_port);

    const char* ip_address_string = GetIP();
    if (agent_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"]|IP[%s] Access denied!|"), ip_address_string);
        Disconnect(true);
        return;
    };
    //
    game_agent_id_ = agent_info->game_agent_id_;
    //
    if (!GameAgentManager::Instance()->AddAgent(game_agent_id_, this))
    {
        SUNLOG(eCRITICAL_LOG, _T("["__FUNCTION__"]|GameAgentID[%u]IP[%s] Duplicated Connection!|"),
               game_agent_id_, ip_address_string);
        Disconnect(true);
        return;
    };
    ;{
        MSG_AS_AUTH_GAMEAGENT_LOGIN_CMD msg;
        msg.m_ConnectStatus = 1;
        msg.m_GameAgentID = game_agent_id_;
        AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));
    };

    SUNLOG(eCRITICAL_LOG, "[GameAgentServerSession::OnAccept] GameAgentID[%u]IP[%s] is Connected",
           game_agent_id_, ip_address_string);
}

void
GameAgentServerSession::OnAccept(DWORD dwNetworkIndex)
{
    ServerSession::OnAccept(dwNetworkIndex);
    assert(game_agent_id_ == 0);
#if !defined(_NA000000_CONFIGURATION_TO_ACCEPT_MULTI_AGENT_ON_SAME_IP_ADDRESS)
    // CHANGES: f101030.1L
    AuthenticateConnectedAgentServer(GetIPAddress(), 0);
#endif
}

void
GameAgentServerSession::OnDisconnect()
{
    if(game_agent_id_ != 0)
    {
        MSG_AS_AUTH_GAMEAGENT_LOGIN_CMD msg;
        msg.m_ConnectStatus = 0;
        msg.m_GameAgentID = game_agent_id_;
        AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));

        GameAgentManager::Instance()->RemoveAgent(game_agent_id_);
    }
    else
    {
        // 에러
    }

    SUNLOG(eCRITICAL_LOG, "[GameAgentServerSession::OnDisconnect] GameAgentID[%u]IP[%s]",
           game_agent_id_, GetIP());

    ServerSession::OnDisconnect();
    game_agent_id_ = 0;
}

void
GameAgentServerSession::OnRecv(BYTE* pMsg, WORD wSize)
{
    MSG_BASE_INTERNAL* pRecvMsg = (MSG_BASE_INTERNAL*)pMsg;
    SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pRecvMsg, wSize);

    // _NA_002455_20110328_AUTH_HEART_BEAT
    if (pRecvMsg->m_byCategory == CATEGORY_AUTH && 
        pRecvMsg->m_byProtocol == AU_AUTH_HEART_BEAT_SYN)
    {
        MSG_AU_AUTH_HEART_BEAT_ACK msg;
        SendPacket(&msg, msg.GetSize());
        return;
    }

    switch(pRecvMsg->m_byProtocol)
    {
#ifdef _NA000000_CONFIGURATION_TO_ACCEPT_MULTI_AGENT_ON_SAME_IP_ADDRESS
    // first sent protocol
    case AU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN:
        OnAU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN(pRecvMsg, wSize);
        break;
#endif
    case GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK:OnGA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK(pRecvMsg, wSize);break;
    case GA_AUTH_PC_LOGIN_CMD:      OnGA_AUTH_PC_LOGIN_CMD(pRecvMsg, wSize);        break;
    case GA_AUTH_PC_LOGOUT_CMD:     OnGA_AUTH_PC_LOGOUT_CMD(pRecvMsg, wSize);       break;
    case GA_AUTH_ASK_PC_LOGOUT_ACK: OnGA_AUTH_ASK_PC_LOGOUT_ACK(pRecvMsg, wSize);   break;
    case GA_AUTH_BILLING_ALARM_ACK: OnGA_AUTH_BILLING_ALARM_ACK(pRecvMsg, wSize);   break;
    case GA_AUTH_PC_NAME_NTF:       OnGA_AUTH_PC_NAME_NTF(pRecvMsg, wSize);         break;
    case GA_AUTH_USER_COUNT_NTF:    OnGA_AUTH_USER_COUNT_NTF(pRecvMsg, wSize);      break;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    case AU_AUTH_SERVER_SELECT_SYN: OnAU_AUTH_SERVER_SELECT_SYN(pRecvMsg, wSize);   break;
#endif
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    case AU_AUTH_COMMON_QUERY_USER_ID_SYN: OnAU_AUTH_COMMON_QUERY_USER_ID_SYN(pRecvMsg, wSize); break;
#endif
    default:
        {
            SUNLOG(eCRITICAL_LOG,
                   "[GameAgentServerSession] Invalid Packet! "
                   "IP[%s] Category[%u] Protocol[%u] Size[%u]", 
                   GetIP(), pRecvMsg->m_byCategory, pRecvMsg->m_byProtocol, wSize);
        }
        break;
    }

}

#ifdef _NA000000_CONFIGURATION_TO_ACCEPT_MULTI_AGENT_ON_SAME_IP_ADDRESS
// NOTE: f101030.1L, change the specification to accept multi-agent on same ip address
// first sent protocol
void
GameAgentServerSession::OnAU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_AU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN* recv_msg = \
        static_cast<MSG_AU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN*>(pMsg);
    //
    const uint16_t listen_port = recv_msg->listen_port_;
    //
    if (game_agent_id_ != 0)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|")
               _T("Msg=already authentication|Ip='%s', Port=%u|"),
               GetIP(), listen_port);
        Disconnect();
        return;
    }
    const ulong ip_address = GetIPAddress();
    AuthenticateConnectedAgentServer(ip_address, listen_port);
}
#endif

void
GameAgentServerSession::OnGA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK* pRecvMsg = (MSG_GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK*)pMsg;
    static_cast<MSG_AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK*>(reinterpret_cast<MSG_GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK*>(1));

    MSG_AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK msg;
    static_cast<MSG_GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK&>(msg) = *pRecvMsg;
    //memcpy(&msg, pRecvMsg, sizeof(MSG_GA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK));
    msg.m_byProtocol = AS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK;
    msg.m_GameAgentID = game_agent_id_;

    AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));

    SUNLOG(eFULL_LOG,  "[OnGA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK] Success!");
}

void
GameAgentServerSession::OnGA_AUTH_PC_LOGIN_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_GA_AUTH_PC_LOGIN_CMD* pRecvMsg = (MSG_GA_AUTH_PC_LOGIN_CMD*)pMsg;
    static_cast<MSG_AS_AUTH_PC_LOGIN_CMD*>(reinterpret_cast<MSG_GA_AUTH_PC_LOGIN_CMD*>(1));

    MSG_AS_AUTH_PC_LOGIN_CMD msg;
    static_cast<MSG_GA_AUTH_PC_LOGIN_CMD&>(msg) = *pRecvMsg;
    //memcpy(&msg, pRecvMsg, sizeof(MSG_GA_AUTH_PC_LOGIN_CMD));
    msg.m_byProtocol = AS_AUTH_PC_LOGIN_CMD;
    msg.m_GameAgentID = game_agent_id_;

    AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));
}

void
GameAgentServerSession::OnGA_AUTH_PC_LOGOUT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_GA_AUTH_PC_LOGOUT_CMD* pRecvMsg = (MSG_GA_AUTH_PC_LOGOUT_CMD*)pMsg;
    static_cast<MSG_AS_AUTH_PC_LOGOUT_CMD*>(reinterpret_cast<MSG_GA_AUTH_PC_LOGOUT_CMD*>(1));

    MSG_AS_AUTH_PC_LOGOUT_CMD msg;
    static_cast<MSG_GA_AUTH_PC_LOGOUT_CMD&>(msg) = *pRecvMsg;
    //memcpy(&msg, pRecvMsg, sizeof(MSG_GA_AUTH_PC_LOGOUT_CMD));
    msg.m_byProtocol = AS_AUTH_PC_LOGOUT_CMD;
    msg.m_GameAgentID = game_agent_id_;

    AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));
}

void
GameAgentServerSession::OnGA_AUTH_ASK_PC_LOGOUT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_GA_AUTH_ASK_PC_LOGOUT_ACK* pRecvMsg = (MSG_GA_AUTH_ASK_PC_LOGOUT_ACK*)pMsg;
    static_cast<MSG_AS_AUTH_ASK_PC_LOGOUT_ACK*>(reinterpret_cast<MSG_GA_AUTH_ASK_PC_LOGOUT_ACK*>(1));

    MSG_AS_AUTH_ASK_PC_LOGOUT_ACK msg;
    static_cast<MSG_GA_AUTH_ASK_PC_LOGOUT_ACK&>(msg) = *pRecvMsg;
    //memcpy(&msg, pRecvMsg, sizeof(MSG_GA_AUTH_ASK_PC_LOGOUT_ACK));
    msg.m_byProtocol = AS_AUTH_ASK_PC_LOGOUT_ACK;
    msg.m_GameAgentID = game_agent_id_;

    AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));
}


// 빌링 만료 알림 패킷 결과
void
GameAgentServerSession::OnGA_AUTH_BILLING_ALARM_ACK(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_GA_AUTH_BILLING_ALARM_ACK* pRecvMsg = (MSG_GA_AUTH_BILLING_ALARM_ACK*)pMsg;

    // 로그만 남긴다.
    SUNLOG(eCRITICAL_LOG,
           "[OnGA_AUTH_BILLING_ALARM_ACK] GameAgentID[%u] dwAuthUserID[%u] Result[%u]",
           game_agent_id_, pRecvMsg->m_dwAuthUserID, pRecvMsg->m_byResult);
}

void
GameAgentServerSession::OnGA_AUTH_PC_NAME_NTF(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_GA_AUTH_PC_NAME_NTF* pRecvMsg = (MSG_GA_AUTH_PC_NAME_NTF*)pMsg;
    static_cast<MSG_AS_AUTH_PC_NAME_NTF*>(reinterpret_cast<MSG_GA_AUTH_PC_NAME_NTF*>(1));

    MSG_AS_AUTH_PC_NAME_NTF msg;
    static_cast<MSG_GA_AUTH_PC_NAME_NTF&>(msg) = *pRecvMsg;
    //memcpy(&msg, pRecvMsg, sizeof(MSG_GA_AUTH_PC_NAME_NTF));
    msg.m_byProtocol = AS_AUTH_PC_NAME_NTF;
    msg.m_GameAgentID = game_agent_id_;

    AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));
}

void
GameAgentServerSession::OnGA_AUTH_USER_COUNT_NTF(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    MSG_GA_AUTH_USER_COUNT_NTF*    pRecvMsg = (MSG_GA_AUTH_USER_COUNT_NTF*)pMsg;
    static_cast<MSG_AS_AUTH_PC_NAME_NTF*>(reinterpret_cast<MSG_GA_AUTH_PC_NAME_NTF*>(1));

    MSG_AS_AUTH_USER_COUNT_NTF msg;
    static_cast<MSG_GA_AUTH_USER_COUNT_NTF&>(msg) = *pRecvMsg;
    //memcpy(&msg, pRecvMsg, sizeof(MSG_GA_AUTH_USER_COUNT_NTF));
    msg.m_byProtocol = AS_AUTH_USER_COUNT_NTF;
    msg.m_GameAgentID = game_agent_id_;

    AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));
}


#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
void
GameAgentServerSession::OnAU_AUTH_SERVER_SELECT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_AU_AUTH_SERVER_SELECT_SYN* pRecvMsg = static_cast<MSG_AU_AUTH_SERVER_SELECT_SYN*>(pMsg);
    MSG_AU_AUTH_SERVER_SELECT_SYN msg = *pRecvMsg;
    msg.m_GameAgentID = game_agent_id_;
    AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));
}
#endif

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
void
GameAgentServerSession::OnAU_AUTH_COMMON_QUERY_USER_ID_SYN(MSG_BASE_INTERNAL* pMsg, WORD size)
{
    const MSG_AU_AUTH_COMMON_QUERY_USER_ID_SYN* pRecvMsg =
        static_cast<MSG_AU_AUTH_COMMON_QUERY_USER_ID_SYN*>(pMsg);
    MSG_AU_AUTH_COMMON_QUERY_USER_ID_SYN msg = *pRecvMsg;
    msg.game_agent_id_ = game_agent_id_;
    AuthAgentFrame::Instance()->SendAuthServer(&msg, sizeof(msg));
}
#endif





