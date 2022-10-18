
#include "stdafx.h"
#include "PacketHandler.h"

#include "HandlerFromTempServer.h"
#include "HandlerFromMasterServer.h"
#include "HandlerFromAccountDBPServer.h"

#include "Sessions/ServerSession.h"

//-------------------------------------------------------
// MODULE VARIABLE
static PACKET_HANDLER_DELEGATOR		s_pPacketHandler = NULL;

//BYTE		PacketHandler::ms_PacketBuffer[32768] = { 0, };

PacketHandler::PacketHandler()
{
	_Initialize();
}

PacketHandler::~PacketHandler()
{
	SAFE_DELETE(m_FuncMaps[TEMP_SERVER].REFERENCE());
	SAFE_DELETE(m_FuncMaps[MASTER_SERVER].REFERENCE());
	SAFE_DELETE(m_FuncMaps[ACCOUNT_DBPROXY].REFERENCE());
}

VOID	PacketHandler::_Initialize()
{
	_Locking();
	m_FuncMaps[TEMP_SERVER] = new util::FunctionMap();
	m_FuncMaps[MASTER_SERVER] = new util::FunctionMap();
	m_FuncMaps[ACCOUNT_DBPROXY] = new util::FunctionMap();
}

VOID	PacketHandler::ClosePacketHandler()
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
	HandlerFromTempServer::RegisterHandler_ALL( s_pPacketHandler );
	HandlerFromMasterServer::RegisterHandler_ALL( s_pPacketHandler );
	HandlerFromAccountDBPServer::RegisterHandler_ALL( s_pPacketHandler );
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

inline
BOOL	PacketHandler::CanDoProcess( FUNC_GENERIC_DELEGATOR& funcDelegator )
{
	if( funcDelegator == NULL )
	{
		return FALSE;
	}
	return TRUE;
}

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


//
//	Session ฐüทร
//

VOID	Services4Session::RegisterPacketHandler()
{
	m_pPacketHandler = &s_pPacketHandler;
}

