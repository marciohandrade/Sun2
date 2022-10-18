#include "stdafx.h"

#include <AgentServer.h>
#include <ServerSessions/TempServerSessionEx.h>
#include <ServerSessions/ServerSessionManager.h>
#include <PacketHandler/PacketHandler.h>

#include <PacketStruct_ServerCommon.h>

TempServerSessionEx::TempServerSessionEx()
{
}

TempServerSessionEx::~TempServerSessionEx()
{
}

VOID	TempServerSessionEx::Init()
{
	TempServerSession::Init();
}

VOID	TempServerSessionEx::OnRecv( BYTE * pMsg, WORD wSize )
{
	PacketHandler::GetHandler()->ParsePacket( TEMP_SERVER, this, (MSG_BASE*)pMsg, wSize );
}

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
void TempServerSessionEx::Redirect2NewSession(MSG_SERVER_TYPE* const first_contact_msg)
{
    DWORD dwSessionIdx = GetSessionIndex(); // backup
	TempServerSession::OnRecv((BYTE*)first_contact_msg, sizeof(*first_contact_msg));

	AgentServer* pAgentServer = AgentServer::GetInstance();
	ServerSession* pRedirectSession = (ServerSession*)pAgentServer->FindSession( dwSessionIdx );
	// NULL로 실패하는 경우도 있을 것이다.
	if( pRedirectSession )	
	{
		eSERVER_TYPE stTYPE = pRedirectSession->GetServerType();
		ServerSessionManager* pManager = ServerSessionManager::Instance();

		switch( stTYPE )
		{
		case FIELD_SERVER:
			{
				FieldServerSession* pFieldServer = (FieldServerSession*)pRedirectSession;
				pManager->AddServer( pFieldServer );

				pFieldServer->CreateVillage();
				pAgentServer->StartClientSideListen();
			}
			break;
		case BATTLE_SERVER:
			{
				FieldServerSession* pBattleServer = (FieldServerSession*)pRedirectSession;
				pManager->AddServer( pBattleServer );
			}
			break;
		default:
			return;
		} // end switch

		switch( stTYPE )
		{
		case FIELD_SERVER:
		case BATTLE_SERVER:
			{	// BATTLE_SERVER | FIELD_SERVER
                ;{
                    MSG_AG_CONNECTION_DBCONNECT_UPDATE_CMD msg_cmd;
                    msg_cmd.db_connector_index_ = pAgentServer->GetDBConnectorIndex();
                    pRedirectSession->SendPacket(&msg_cmd, sizeof(msg_cmd));
                };
                ;{
				    // 월드서버 정보를 필드서버에 알린다.
				    MSG_AG_WORLDSERVER_INFO_CMD msgCMD;
				    msgCMD.m_bIsConnectedWorld = pAgentServer->IsConnectedTo( WORLD_SERVER );
				    pRedirectSession->SendPacket( &msgCMD, sizeof(msgCMD) );
                };
                ;{
                    MSG_AG_CONNECTION_SERVER_INFO_CMD msg_cmd;
                    msg_cmd.pvp_enabled_ = pAgentServer->IsPVPMode() != false;
                    // (CHANGES) (f100424.3L) add channel id to synchronize or debug
                    const SERVER_KEY& server_key = pAgentServer->GetKey();
                    msg_cmd.channel_id_ = server_key.m_ChannelID;
                    pRedirectSession->SendPacket(&msg_cmd, sizeof(msg_cmd));
                };
			}
			break;
		} // end switch
	}
}
