
#include "stdafx.h"
#include "PacketHandler.h"

#include <Sessions/ServerSessionEx.h>

#include <Protocol_ServerCommon.h>
#include <PacketStruct_AZ.h>
#include <PacketStruct_GZ.h>
#include <PacketStruct_DZ.h>
#include <PacketStruct_MZ.h>
#include <PacketStruct_WZ.h>

#include "Handler_FromGameServer.h"
#include "Handler_FromAgentServer.h"
#include "Handler_FromDBProxyServer.h"
#include "Handler_FromMasterServer.h"
#include "Handler_FromWorldServer.h"

//-------------------------------------------------------
// MODULE VARIABLE
static PACKET_HANDLER_DELEGATOR		s_pPacketHandler = NULL;

PacketHandler::PacketHandler()
{
	_Initialize();
}

PacketHandler::~PacketHandler()
{
	m_FuncMaps[FIELD_SERVER] = NULL;
	m_FuncMaps[BATTLE_SERVER] = NULL;

	for( DWORD i=0 ; i<MAX_SERVER ; ++i )
	{
		SAFE_DELETE(m_FuncMaps[i].REFERENCE());
	}
}

PACKET_HANDLER_DELEGATOR	PacketHandler::GetHandler()
{
	return s_pPacketHandler;
}


VOID	PacketHandler::_Initialize()
{
	_Locking();

	memset( m_FuncMaps, 0, sizeof(m_FuncMaps) );

	//m_FuncMaps[UNKNOWN_SERVER] = new util::FunctionMap();
	m_FuncMaps[AGENT_SERVER] = new util::FunctionMap();
	m_FuncMaps[GAME_DBPROXY] = new util::FunctionMap();
	m_FuncMaps[WORLD_SERVER] = new util::FunctionMap();
	m_FuncMaps[MASTER_SERVER] = new util::FunctionMap();
	//m_FuncMaps[GUILD_SERVER] = new util::FunctionMap();
	//m_FuncMaps[ACCOUNT_DBPROXY] = new util::FunctionMap();
	//m_FuncMaps[TOURNAMENT_SERVER].REFERENCE());
	//m_FuncMaps[AUTH_AGENT_SERVER].REFERENCE());
	//m_FuncMaps[OP_SERVER] = new util::FunctionMap();
	m_FuncMaps[TEMP_SERVER] = new util::FunctionMap();
	//m_FuncMaps[PORTAL_SERVER] = new util::FunctionMap();
	//m_FuncMaps[SOLARAUTH_SERVER] = new util::FunctionMap();

	// 공유한다.
	m_FuncMaps[GAME_SERVER] = new util::FunctionMap();
	m_FuncMaps[FIELD_SERVER] = m_FuncMaps[GAME_SERVER];
	m_FuncMaps[BATTLE_SERVER] = m_FuncMaps[GAME_SERVER];
}

VOID	PacketHandler::ReleaseHandler()
{
	SAFE_DELETE(s_pPacketHandler.REFERENCE());
}

VOID	PacketHandler::RegisterHandler()
{
	if( s_pPacketHandler() == NULL )
	{
		s_pPacketHandler = new PacketHandler();
	}

	s_pPacketHandler->_UnLocking();
	Handler_FromGameServer::RegisterHandler_ALL( s_pPacketHandler );
	Handler_FromDBProxyServer::RegisterHandler_ALL( s_pPacketHandler );
	Handler_FromMasterServer::RegisterHandler_ALL( s_pPacketHandler );
	Handler_FromWorldServer::RegisterHandler_ALL( s_pPacketHandler );
	s_pPacketHandler->_Locking();
}


BOOL	PacketHandler::_RegisterHandler( eSERVER_TYPE serverType, sPACKET_HANDLER_INFO* pINFOs )
{
	if( _IsLock() )
		return FALSE;

	sPACKET_HANDLER_INFO* pOffset = pINFOs;

	BOOL bResult = TRUE;

	if( (DWORD)MAX_SERVER > (DWORD)serverType )
	{
		FUNCTION_MAP_DELEGATOR& rFuncMap = m_FuncMaps[serverType];
		if( rFuncMap != NULL )
		{
			for( ; pOffset ; ++pOffset )
			{
				if( pOffset->pfnHandler )
				{
					FUNC_GENERIC_DELEGATOR func = new FUNC_GENERIC;
					func->m_dwFunctionKey = pOffset->wCATEGORYnPROTOCOL;
					func->m_fnHandler = pOffset->pfnHandler;
					bResult = rFuncMap->Add( func() );
					if( !bResult )
						break;
				}
				else if( !(pOffset->wCATEGORYnPROTOCOL) )
				{
					break;
				}
			}
		}
		else
			bResult = FALSE;
	}
	else
		bResult = FALSE;

	if( !bResult )
	{
		BYTE byCategory = (BYTE)(pOffset->wCATEGORYnPROTOCOL&0xFF);
		BYTE byProtocol = (BYTE)((pOffset->wCATEGORYnPROTOCOL>>8)&0xFF);

		MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
			_T("Can't Register %s Packet Handler [C:%03u][P:%03u]"),
			GetServerType2String(serverType), byCategory, byProtocol );
	}

	return bResult;
}

//
//
//
inline
BOOL	PacketHandler::CanDoProcess( FUNC_GENERIC_DELEGATOR& funcDelegator )
{
	if( funcDelegator == NULL )
	{
		return FALSE;
	}
	return TRUE;
}
//
//
//

BOOL	PacketHandler::_ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE* pMsg, WORD wSize )
{
	if( (DWORD)MAX_SERVER > (DWORD)serverType )
	{
		FUNCTION_MAP_DELEGATOR& rFuncMap = m_FuncMaps[serverType];
		if( rFuncMap != NULL )
		{
			WORD wCP = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);

			FUNC_GENERIC_DELEGATOR fnINFO = (FUNC_GENERIC*)rFuncMap->Find( wCP );

			if( CanDoProcess( fnINFO ) )
			{
				fnINFO->m_fnHandler( pSession, pMsg, wSize );
				return TRUE;
			}
		}
	}

	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
		_T("%s Handler Null, Invalid Type, UnAuthorized [C:P=%03d:%03d]"),
		GetServerType2String(serverType), pMsg->m_byCategory, pMsg->m_byProtocol );

	return FALSE;
}

BOOL	PacketHandler::_ParsePacket( eSERVER_TYPE serverType, ServerSession* pSession, MSG_BASE_INTERNAL* pMsg, WORD wSize )
{
	if( (DWORD)MAX_SERVER > (DWORD)serverType )
	{
		FUNCTION_MAP_DELEGATOR& rFuncMap = m_FuncMaps[serverType];
		if( rFuncMap != NULL )
		{
			WORD wCP = MAKEWORD(pMsg->m_byCategory, pMsg->m_byProtocol);

			FUNC_GENERIC_DELEGATOR fnINFO = (FUNC_GENERIC*)rFuncMap->Find( wCP );

			if( CanDoProcess( fnINFO ) )
			{
				fnINFO->m_fnHandler( pSession, (MSG_BASE*)pMsg, wSize );
				return TRUE;
			}
		}
	}

	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
		_T("%s Handler Null, Invalid Type, UnAuthorized [C:P=%03d:%03d]"),
		GetServerType2String(serverType), pMsg->m_byCategory, pMsg->m_byProtocol );

	return FALSE;
}











//BOOL PacketHandler::Resister()
//{
//#define HANDLER_AGENT( c, p )	if( !AddHandler( PACKET_HANDLER_AGENT, c, p, Handler_FromAgentServer::On##p ) ) return FALSE
//#define HANDLER_GAME( c, p )	if( !AddHandler( PACKET_HANDLER_GAME, c, p, Handler_FromGameServer::On##p ) ) return FALSE
//#define HANDLER_DBPROXY( c, p )	if( !AddHandler( PACKET_HANDLER_DBPROXY, c, p, Handler_FromDBProxyServer::On##p ) ) return FALSE
//#define HANDLER_MASTER( c, p )	if( !AddHandler( PACKET_HANDLER_MASTER, c, p, Handler_FromMasterServer::On##p ) ) return FALSE
//#define HANDLER_WORLD( c, p )	if( !AddHandler( PACKET_HANDLER_WORLD, c, p, Handler_FromWorldServer::On##p ) ) return FALSE
//
//	HANDLER_AGENT( 0, SERVERCOMMON_HEARTBEAT );
//	HANDLER_AGENT( AZ_GUILD, AZ_GUILD_LOGIN_CHAR_CMD );
//	HANDLER_AGENT( AZ_GUILD, AZ_GUILD_LOGOUT_CHAR_CMD );
//
//
//	HANDLER_GAME( 0, SERVERCOMMON_HEARTBEAT );	
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_SELECT_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_CREATE_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_DESTROY_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_JOIN_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_WITHDRAW_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_KICKPLAYER_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_CHANGE_NOTICE_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_DONATE_UP_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_CONVERT_UP2GP_SYN );
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_RANKUP_SYN );
//
//	HANDLER_GAME( GZ_GUILD, GZ_GUILD_CHANGE_CHAOSTATE_CMD );
//
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_SELECT_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_CREATE_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_DESTROY_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_JOIN_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_WITHDRAW_ACK );
//
//
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_SELECT_NAK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_CREATE_NAK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_DESTROY_NAK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_JOIN_NAK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_WITHDRAW_NAK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_KICKPLAYER_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_KICKPLAYER_NAK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_CHANGE_NOTICE_NAK );
//
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_DONATE_UP_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_DONATE_UP_NAK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_CONVERT_UP2GP_NAK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_RANKUP_ACK );
//	HANDLER_DBPROXY( DZ_GUILD, DZ_GUILD_RANKUP_NAK );
//
//	HANDLER_MASTER( MZ_CONNECTION, MZ_SERVER_INFO_CMD );
//	HANDLER_MASTER( SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_REQ );
//
//#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//	HANDLER_MASTER( MZ_CONNECTION, MZ_CONNECTION_DISCONNECT_CMD );
//	HANDLER_MASTER( MZ_CONNECTION, MZ_CONNECTION_PHASE_SHIFT_CMD );
//	HANDLER_MASTER( MZ_CONNECTION, MZ_CONNECTION_BLOCK_CMD );
//
//	HANDLER_MASTER( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
//	HANDLER_MASTER( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
//	HANDLER_MASTER( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD );
//#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//
//
//	HANDLER_WORLD( WZ_GUILD, WZ_GUILD_SELECT_SYN );
//
//	return TRUE;
//}

