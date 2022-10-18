#include "stdafx.h"
#include "AgentServerSession.h"
#include "PacketHandler/PacketHandler.h"
#include <PacketStruct.h>
#include <PublicMath.h>
#include "GameServerEx.h"
#include "PlayerManager.h"
#include "ObjectFactory.h"
#include "GameZoneManager.h"

#include "GameParty/NewGamePartyManager.h"

#include "GameGuildManager.h"

__IMPL_CUSTOMPOOL_PTR( AgentServerSession )
AgentServerSession::AgentServerSession()
{
}

AgentServerSession::~AgentServerSession()
{
}

// CHANGES: f110617.1L
void AgentServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

void AgentServerSession::OnConnect(BOOL succeed, DWORD session_index)
{
    ServerSession::OnConnect(succeed, session_index);

    if (succeed)
    {
        // CHANGES: f110615.2L, added to solve high-load network bottleneck problem
        this->UseSendPendingBuffer(10 * 1024 * 1024);
        this->UseRecvDualBuffer(1 * 1024 * 1024);
        SUNLOG(eFULL_LOG, "|["__FUNCTION__"]|Connected to agent server.");
    }
}

VOID AgentServerSession::OnDisconnect()
{
	SUNLOG( eCRITICAL_LOG, "[AgentServerSession::OnDisconnect] Disconnected from Agent Server" );

	g_GameZoneManager.DestroyZoneAll();
	// Link된 유저를 삭제하고 DBP에 알린다.
	// 즉, 모든 유저를 삭제하고 DBP에 알린다.
	PlayerManager::Instance()->AllDisconnect();

	NewGamePartyManager::Instance()->DestroyAllParty();

	g_pGameServer->SetConnectToWorldServer( FALSE );
	g_GameGuildManager.Release();

	ServerSession::OnDisconnect();
}

VOID AgentServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_AGENT, this, (MSG_BASE *)pMsg, wSize );
}

VOID AgentServerSession::OnLogString( char *pszLog )
{
	switch( g_pGameServer->GetServerType() )
	{
	case FIELD_SERVER:
		SUNLOG( eCRITICAL_LOG, "[AgentSession:Field] %s", pszLog );
		break;
	case BATTLE_SERVER:
		SUNLOG( eCRITICAL_LOG, "[AgentSession:Battle] %s", pszLog );
		break;
	default:
		assert(0);
	}	
}

BOOL AgentServerSession::Send( BYTE *pMsg, WORD wSize )
{
	BOOL bSuccess = NetworkObject::Send( pMsg, wSize );

	return bSuccess;
}














