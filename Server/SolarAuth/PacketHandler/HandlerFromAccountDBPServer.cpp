
#include "stdafx.h"

#include <PacketStruct_SX.h>

#include "PacketHandler.h"
#include "HandlerFromAccountDBPServer.h"

#include <SolarAuthServer.h>
#include "Sessions/AccountDBProxySession.h"


#define SX_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define SX_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&HandlerFromAccountDBPServer::On##p }


sPACKET_HANDLER_INFO*	HandlerFromAccountDBPServer::GetHandlerInfo()
{

	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
		SX_HANDLER_INFO(SX_CONNECTION, SX_CONNECTION_REQ_SERVER_INFO_ACK, PHASE_UNLIMITED),
		SX_HANDLER_INFO(SX_CONNECTION, SX_CONNECTION_REQ_SERVER_INFO_CMD, PHASE_UNLIMITED),
		SX_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	HandlerFromAccountDBPServer::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	//BOOL bResult =
	pInstance->RegisterHandler_<ACCOUNT_DBPROXY>( GetHandlerInfo() );
	//
}

#include <Information/ServerList/ServerInfo.h>

HandlerFromAccountDBPServer_IMPL(SX_CONNECTION_REQ_SERVER_INFO_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK* pMsgACK = (MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK*)pMsg;

	DWORD dwCounts = pMsgACK->m_dwNumberOfServerInfo;

	if( MSG_SX_CONNECTION_REQ_SERVER_INFO_ACK::_MAX_SERVERINFO_ARRAY_NUM < dwCounts )
	{
		MessageOut( eCRITICAL_LOG, _T("서버 정보가 이상합니다. 아니면 진짜로 100개 넘는 서버수?") );
		return;
	}

	MessageOut( eCRITICAL_LOG, _T("Number of Server Info: %u"), dwCounts );

	sSERVERLISTFROMDB_ARGS args;
	args.m_SVR_LIST = pMsgACK;

	ServerListFromDB svrFromDB;
	svrFromDB.Init( &args );

	ServerTreeINFO* pINFO = NULL;
	svrFromDB.Load( NULL, 0, pINFO, TRUE );

	//ServerInfoManager::AddServerInfo( pMsgACK->m_ServerInfo, dwCounts );
}

HandlerFromAccountDBPServer_IMPL(SX_CONNECTION_REQ_SERVER_INFO_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	SolarAuthServer* pSolarAuth = SolarAuthServer::GetMainFrame();
	MessageOut( eCRITICAL_LOG, _T("[COMPLETE] Server Information Received") );

	pSolarAuth->StartServer();
}


