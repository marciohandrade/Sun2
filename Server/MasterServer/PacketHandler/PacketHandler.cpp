
#include "stdafx.h"
#include "PacketHandler.h"

#include "Sessions/ServerSessionEx.h"

#include "Handler_AM.h"
#include "Handler_DM.h"
#include "Handler_GM.h"
#include "Handler_MO.h"
#include "Handler_MP.h"
#include "Handler_MW.h"
#include "Handler_MX.h"
#include "Handler_MZ.h"
#include "Handler_SM.h"
#include "Handler_ME.h"

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

VOID	PacketHandler::_Initialize()
{
	_Locking();

	memset( m_FuncMaps, 0, sizeof(m_FuncMaps) );

	//m_FuncMaps[UNKNOWN_SERVER] = new util::FunctionMap();
	m_FuncMaps[AGENT_SERVER] = new util::FunctionMap();
	m_FuncMaps[GAME_DBPROXY] = new util::FunctionMap();
	m_FuncMaps[WORLD_SERVER] = new util::FunctionMap();
	//m_FuncMaps[MASTER_SERVER].REFERENCE());
	m_FuncMaps[GUILD_SERVER] = new util::FunctionMap();
	m_FuncMaps[ACCOUNT_DBPROXY] = new util::FunctionMap();
	//m_FuncMaps[TOURNAMENT_SERVER].REFERENCE());
	//m_FuncMaps[AUTH_AGENT_SERVER].REFERENCE());
	m_FuncMaps[OP_SERVER] = new util::FunctionMap();
	m_FuncMaps[TEMP_SERVER] = new util::FunctionMap();
	m_FuncMaps[PORTAL_SERVER] = new util::FunctionMap();
	m_FuncMaps[SOLARAUTH_SERVER] = new util::FunctionMap();

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
	Handler_AM::RegisterHandler_ALL( s_pPacketHandler );
	Handler_DM::RegisterHandler_ALL( s_pPacketHandler );
	Handler_GM::RegisterHandler_ALL( s_pPacketHandler );
	Handler_MO::RegisterHandler_ALL( s_pPacketHandler );
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	Handler_MP::RegisterHandler_ALL( s_pPacketHandler );
#endif
	Handler_MW::RegisterHandler_ALL( s_pPacketHandler );
	Handler_MX::RegisterHandler_ALL( s_pPacketHandler );
	Handler_MZ::RegisterHandler_ALL( s_pPacketHandler );
	Handler_SM::RegisterHandler_ALL( s_pPacketHandler );
	Handler_ME::RegisterHandler_ALL( s_pPacketHandler );
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

//
//	Session 관련
//

VOID	Services4Session::RegisterPacketHandler()
{
	m_pPacketHandler = &s_pPacketHandler;
}

