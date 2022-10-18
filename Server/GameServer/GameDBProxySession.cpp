#include "stdafx.h"
#include "GameDBProxySession.h"
#include "PacketHandler/PacketHandler.h"
#include <PacketStruct.h>
#include "PacketStruct_DG.h"
#include "GameServerEx.h"
#include "GameZoneManager.h"
#include "GameGuildManager.h"
#include "GameParty/NewGamePartyManager.h"
#include "GameRankingManager.h"

__IMPL_CUSTOMPOOL_PTR( GameDBProxySession )
GameDBProxySession::GameDBProxySession()
{
}

GameDBProxySession::~GameDBProxySession()
{
}

// CHANGES: f110617.1L
void GameDBProxySession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID GameDBProxySession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	ServerSession::OnConnect( bSuccess, dwSessionIndex );

    if (bSuccess)
    {
        // CHANGES: f110615.2L, added to solve high-load network bottleneck problem
        this->UseSendPendingBuffer(10 * 1024 * 1024);
        this->UseRecvDualBuffer(1 * 1024 * 1024);
        SUNLOG(eFULL_LOG, "|["__FUNCTION__"]|Connected to game DB proxy.");

		ServerSession * pSession = g_pGameServer->GetSession( AGENT_SERVER );
		if( pSession && !pSession->IsConnected() && !pSession->IsConnecting() )
			g_pGameServer->ConnectTo( AGENT_SERVER );
        ;{  //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
            MSG_DG_CONNECTION_DBCONNECT_UPDATE_CMD msg_cmd;
            msg_cmd.db_connector_index_ = g_pGameServer->GetDBConnectorIndex();
            g_pGameServer->SendToServer(GAME_DBPROXY, &msg_cmd, sizeof(msg_cmd));
        };
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    #ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
        g_GameRankingMgr.Initialize();
    #endif
#endif // _NA_008012_20150130_RANKING_SYSTEM
	}
	else
	{
		//SUNLOG( eFULL_LOG,  "Can't connect to game DB proxy." );
	}
}

VOID GameDBProxySession::OnDisconnect()
{
	SUNLOG( eCRITICAL_LOG, "Disconnected from game DB proxy." );

	ServerSession * pSession = g_pGameServer->GetSession( AGENT_SERVER );
	if( pSession && pSession->IsConnected() )
	{
		MSG_AG_CONNECTION_DISCONNECT_SERVER_CMD	cmd;
		cmd.m_ServerType = g_pGameServer->GetServerType();
		pSession->Send( (BYTE*)&cmd, sizeof(cmd) );

		g_GameZoneManager.DestroyZoneAll();
		// Link된 유저를 삭제하고 DBP에 알린다.
		// 즉, 모든 유저를 삭제하고 DBP에 알린다.
		PlayerManager::Instance()->AllDisconnect();

		NewGamePartyManager::Instance()->DestroyAllParty();

		g_pGameServer->SetConnectToWorldServer( FALSE );
		g_GameGuildManager.Release();
	}
#ifdef _NA_008012_20150130_RANKING_SYSTEM
#ifdef _NA002635_GP_DP_PROTOCOL_UINIFICATION
    g_GameRankingMgr.OnReceivedStatusControl(eRANKING_STATUS_INIT);
#endif
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    g_GameRankingMgr.Release();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#endif // _NA_008012_20150130_RANKING_SYSTEM
	ServerSession::OnDisconnect();
}

VOID GameDBProxySession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_DBPROXY, this, (MSG_BASE *)pMsg, wSize );
}
