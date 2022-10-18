#include "stdafx.h"
#include "PacketHandler.h"

/// DBProxy - Game Server
#include <Protocol_ServerCommon.h>
//#include <PacketStruct_MP.h>
//#include <PacketStruct_AP.h>
#include <PacketStruct_GP.h>
#include <PacketStruct_DP.h>
#include <PacketStruct_MP.h>

#include "Handler_GameServer.h"
#include "Handler_MasterServer.h"
#include "Handler_AgentServer.h"
#include "Handler_DBProxy.h"
#include "Handler_Common.h"


PacketHandler::PacketHandler()
{
	BOOL rt = Resister();
	ASSERT(rt);
}

PacketHandler::~PacketHandler()
{

}

BOOL PacketHandler::Resister()
{
#define HANDLER_COMMON( type, c, p )	if( !AddHandler( type, c, p, Handler_Common::On##p ) ) return FALSE
#define HANDLER_AGENT( c, p )	if( !AddHandler( PACKET_HANDLER_AGENT, c, p, Handler_AgentServer::On##p ) ) return FALSE
#define HANDLER_GAME( c, p )	if( !AddHandler( PACKET_HANDLER_GAME, c, p, Handler_GameServer::On##p ) ) return FALSE
#define HANDLER_MASTER( c, p )	if( !AddHandler( PACKET_HANDLER_MASTER, c, p, Handler_MasterServer::On##p ) ) return FALSE
#define HANDLER_DBPROXY( c, p )	if( !AddHandler( PACKET_HANDLER_DBPROXY, c, p, Handler_DBProxy::On##p ) ) return FALSE

	//------------------------------------------------------------------------------------------------------------
	// GAME
	//------------------------------------------------------------------------------------------------------------
	//HANDLER_COMMON( PACKET_HANDLER_GAME, SERVERCOMMON, SERVERCOMMON_HEARTBEAT );
	HANDLER_GAME(GP_RANKING,GP_RANKING_INFO_REQ_SYN);
	HANDLER_GAME(GP_RANKING,GP_RANKING_MISSION_CLEAR_SYN);
	HANDLER_GAME(GP_RANKING,GP_RANKING_MISSION_REWARD_SYN);
	HANDLER_GAME(GP_RANKING,GP_RANKING_SETTLE_CMD);

	HANDLER_GAME(GP_RANKING,GP_RANKING_REGISTER_MISSION_POINT_SYN);

	HANDLER_GAME(GP_RANKING, GP_MISSION_RANKING_SETTLE_DATE_SYN);

	//------------------------------------------------------------------------------------------------------------
	// DBPROXY
	//------------------------------------------------------------------------------------------------------------
//	HANDLER_COMMON( PACKET_HANDLER_PORTAL, SERVERCOMMON, SERVERCOMMON_HEARTBEAT );
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_INFO_REQ_ACK);
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_INFO_REQ_NAK);
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_INFO_REQ_END_NTF);
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_MISSION_CLEAR_ACK);
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_MISSION_CLEAR_NAK);
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_MISSION_REWARD_ACK);
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_MISSION_REWARD_NAK);
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_REGISTER_MISSION_POINT_ACK);
	HANDLER_DBPROXY(DP_RANKING, DP_RANKING_REGISTER_MISSION_POINT_NAK);

	//------------------------------------------------------------------------------------------------------------
	// MASTER
	//------------------------------------------------------------------------------------------------------------
	HANDLER_MASTER( MP_CONNECTION, MP_CONNECTION_SERVER_INFO_CMD );
	HANDLER_MASTER( SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_REQ);
	//HANDLER_MASTER( MP_CONNECTION, MP_CONNECTION_REQ_SERVER_INFO_DBR );
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	HANDLER_MASTER( MP_CONNECTION, MP_CONNECTION_DISCONNECT_CMD );
	HANDLER_MASTER( MP_CONNECTION, MP_CONNECTION_PHASE_SHIFT_CMD );
	HANDLER_MASTER( MP_CONNECTION, MP_CONNECTION_BLOCK_CMD );

	HANDLER_MASTER( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
	HANDLER_MASTER( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
	HANDLER_MASTER( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__


	//------------------------------------------------------------------------------------------------------------
	// AGENT
	//------------------------------------------------------------------------------------------------------------
	//HANDLER_COMMON( PACKET_HANDLER_AGENT, SERVERCOMMON, SERVERCOMMON_HEARTBEAT );
	//HANDLER_AGENT( AD_CHARINFO, AD_CHARINFO_CHARLISTREQ_SYN );

	return TRUE;
}

BOOL PacketHandler::AddHandler( ePACKET_HANDLER PHandler, BYTE category, BYTE protocol, fnHandler fnHandler )
{
	HANDLER_FUNC * pFuncInfo	= new HANDLER_FUNC;
	pFuncInfo->m_dwFunctionKey	= MAKEWORD( category, protocol );
	pFuncInfo->m_fnHandler		= fnHandler;
	return m_FuncMap[PHandler].Add( pFuncInfo );
}

VOID PacketHandler::ParsePacket( ePACKET_HANDLER PHandler, ServerSession * pSession, MSG_BASE *pMsg, WORD wSize )
{
	HANDLER_FUNC * pFuncInfo = (HANDLER_FUNC *)m_FuncMap[PHandler].Find( MAKEWORD( pMsg->m_byCategory, pMsg->m_byProtocol ) );

	if( pFuncInfo == NULL )
	{
		DISPMSG( "[PacketHandler::ParsePacket] Invalid packet! Category[%u] Protocol[%u] ", pMsg->m_byCategory, pMsg->m_byProtocol );
		ASSERT( ! "Invalid packet from Server!" ) ;

		return ;
	}

	pFuncInfo->m_fnHandler( pSession, pMsg, wSize );
}
