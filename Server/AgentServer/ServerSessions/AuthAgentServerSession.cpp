#include "stdafx.h"

#include <AgentServer.h>
#include <ServerSessions/AuthAgentServerSession.h>
#include <PacketHandler/PacketHandler.h>


AuthAgentServerSession::AuthAgentServerSession()
{
    ClearHeartBeatResponse();
}

AuthAgentServerSession::~AuthAgentServerSession()
{
}

void AuthAgentServerSession::Update()
{
    __super::Update();
    // AuthAgent 서버로부터 대기 시간이 지나도 Heartbeat 응답이 오지 않으면 연결을 끊는다.
    // 이쪽에서는 정상적으로 보여도 AuthAgent 쪽에서는 네트워크 문제로 연결이
    // 끊어진 상태이다.
    if (is_waiting_heart_beat_response_ && heart_beat_response_timer_.IsExpired())
    {
        Disconnect();
    }
}

VOID	AuthAgentServerSession::Init()
{
	ServerSessionEx::Init();
    ClearHeartBeatResponse();

//	LogoutUserFactory::Instance()->Init( 5000 );
}

VOID	AuthAgentServerSession::Release()
{
//	LogoutUserFactory::Instance()->Release();
    ClearHeartBeatResponse();
	ServerSessionEx::Release();
}

// CHANGES: f110617.1L
void AuthAgentServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

VOID	AuthAgentServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	SetIsManualConnect(false);
	SetConnecting(false);

	if( bSuccess )
	{
		OnConnectSuccess( dwNetworkIndex );

		// 접속하자마자 서버확인 패킷을 보낸다.
    #ifdef _NA000000_CONFIGURATION_TO_ACCEPT_MULTI_AGENT_ON_SAME_IP_ADDRESS
        MSG_AU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN msg_syn;
        msg_syn.listen_port_ = AgentServer::GetInstance()->GetListenPort_c();
        Send(reinterpret_cast<uint8_t*>(&msg_syn), msg_syn.GetSize());
    #endif
	}
	else
	{
	}
}

VOID	AuthAgentServerSession::OnRecv( BYTE* pMsg, WORD wSize )
{
	MSG_BASE_INTERNAL* pMsgBASE = (MSG_BASE_INTERNAL*)pMsg;
    if (pMsgBASE->m_byCategory == CATEGORY_AUTH && 
        pMsgBASE->m_byProtocol == AU_AUTH_HEART_BEAT_ACK)
    {
        is_waiting_heart_beat_response_ = false;
        if (heart_beat_response_timer_.IsExpired())
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|Heartbeat response time is too late.|"), __FUNCTION__);
        }
        return;
    }
	PacketHandler::GetHandler()->ParsePacket( GetServerType(), this, pMsgBASE, wSize );
}

BOOL	AuthAgentServerSession::SendPacket( MSG_BASE * pMsg, WORD wSize )
{
	SUNLOG( eCRITICAL_LOG, _T("Unexpected Called Routine, Not Implemented SndPk") );
	return FALSE;
}

VOID	AuthAgentServerSession::OnDisconnect()
{
    ClearHeartBeatResponse();
	ServerSessionEx::OnDisconnect();
}

void AuthAgentServerSession::SendHeartbeatPacket()
{
#ifdef _NA_002455_20110328_AUTH_HEART_BEAT
    if (is_waiting_heart_beat_response_ == false)
    {
        MSG_AU_AUTH_HEART_BEAT_SYN msg;
        ServerSession::SendPacket(&msg, msg.GetSize());
        heart_beat_response_timer_.Reset();
        is_waiting_heart_beat_response_ = true;
    }
#endif
}

void AuthAgentServerSession::ClearHeartBeatResponse()
{
    enum { kHeartBeatResponseTimeout = 60 * 1000 }; // 응답대기시간 1분
    is_waiting_heart_beat_response_ = false;
    heart_beat_response_timer_.SetTimer(kHeartBeatResponseTimeout);
    heart_beat_response_timer_.DisableCheckTime();
}