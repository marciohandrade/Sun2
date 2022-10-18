#include "stdafx.h"
#include "Handler_MX.h"
#include "PacketHandler.Shared.h"


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_MX::On##p }


sPACKET_HANDLER_INFO*	Handler_MX::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
        ADD_HANDLER_INFO(MX_CONNECTION, MX_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MX_CONNECTION, MX_CONNECTION_REQ_SERVER_INFO_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MX_CONNECTION, MX_CONNECTION_REQ_SERVER_INFO_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MX_CONNECTION, MX_FUNCRESTRICT_RELOAD_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(MX_CONNECTION, MX_FUNCRESTRICT_RELOAD_NAK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_MX::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<ACCOUNT_DBPROXY>( GetHandlerInfo() );
	pInstance->RegisterHandler_<ACCOUNT_DBPROXY>( PacketHandlerShared::GetHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Function/ServerInfoManager.h>
#include <Sessions/ServerSessionManager.h>

HANDLER_MX_IMPL(MX_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

HANDLER_MX_IMPL(MX_CONNECTION_REQ_SERVER_INFO_ACK)
{
#ifndef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK* pMsgACK = (MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK*)pMsg;

	DWORD dwCounts = pMsgACK->m_dwNumberOfServerInfo;

	if( MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK::_MAX_SERVERINFO_ARRAY_NUM < dwCounts )
	{
		MessageOut( eCRITICAL_LOG, _T("서버 정보가 이상합니다. 아니면 진짜로 100개 넘는 서버수?") );
		return;
	}

	MessageOut( eCRITICAL_LOG, _T("Number of Server Info: %u"), dwCounts );

	ServerInfoManager::AddServerInfo( pMsgACK->m_ServerInfo, dwCounts );
#else //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	MessageOutEx( g_pSecureCollectLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_CONNECTION,
		_T("This version is not supported. REQSVR") );
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

HANDLER_MX_IMPL(MX_CONNECTION_REQ_SERVER_INFO_CMD)
{
#ifndef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();

	if( !pMasterServer->IsRestrictFuncFromFile() )
	{
		// AccountDB로 기능제한 정보 요청
		pMasterServer->GetMasterServerInfo().SendMX_FUNCRESTRICT_RELOAD_REQ( pMasterServer );
	}
	else
	{
		MessageOut( eCRITICAL_LOG, _T("[Function Restrict Mode] from FILE") );
	}

	MessageOut( eCRITICAL_LOG, _T("[COMPLETE] Server Information Received") );

	pMasterServer->StartListen();
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

HANDLER_MX_IMPL(MX_FUNCRESTRICT_RELOAD_ANS)
{
	MSG_MX_FUNCRESTRICT_RELOAD_ANS* pMsgANS = (MSG_MX_FUNCRESTRICT_RELOAD_ANS*)pMsg;

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	sRESTRICT_MGR::MakePacket::Parse_MX_FUNCRESTRICT_RELOAD_ANS( &pMasterServer->RestrictManager(), pMsgANS, FALSE, TRUE );

	pMasterServer->GetMasterServerInfo().SendGM_OPERATION_FUNCRESTRICT_CMD( pMasterServer );

	MessageOut( eCRITICAL_LOG, _T("Reload FuncRestrict from DB & Broadcast GameServers") );
}

HANDLER_MX_IMPL(MX_FUNCRESTRICT_RELOAD_NAK)
{
	MSG_MX_FUNCRESTRICT_RELOAD_NAK* pRecvMsg = (MSG_MX_FUNCRESTRICT_RELOAD_NAK*)pMsg;

	MessageOut( eCRITICAL_LOG, "Func Restrict Reload Failed[ErrorCode:%u]", pRecvMsg->m_dwErrorCode );
}


