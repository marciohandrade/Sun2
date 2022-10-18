#include "stdafx.h"
#include "Handler_GM.h"
#include "PacketHandler.Shared.h"

#include <Sessions/SolarAuthServerSession.h>


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_GM::On##p }


sPACKET_HANDLER_INFO*	Handler_GM::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
        ADD_HANDLER_INFO(GM_CONNECTION, GM_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_HEARTBEAT, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(GM_OPERATION, GM_USERMONEY_NTF, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(GM_OPERATION, GM_OPERATION_USERMONEY_ANS, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(GM_OPERATION, GM_OPERATION_STATISTICS_ANS, PHASE_UNLIMITED),
#ifdef __NA000000_070523_REPORT_TO_SOLARAUTH
		ADD_HANDLER_INFO(GM_OPERATION, GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF, PHASE_UNLIMITED),
#endif //
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_GM::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<GAME_SERVER>( GetHandlerInfo() );
	pInstance->RegisterHandler_<GAME_SERVER>( PacketHandlerShared::GetHandlerInfo() );
	pInstance->RegisterHandler_<GAME_SERVER>( PacketHandlerShared::GetDataHandlerInfo() );
}

//
//
//
#include <MasterServerEx.h>
#include <Sessions/GameServerSession.h>
#include <Sessions/OpServerSession.h>
#include <Function/Statistics.h>
#include <Function/ServerInfoManager.h>

HANDLER_GM_IMPL(GM_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

//HANDLER_GM_IMPL(SERVERCOMMON_HEARTBEAT)

HANDLER_GM_IMPL(GM_USERMONEY_NTF)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_GM_USERMONEY_NTF* pMsgNTF = (MSG_GM_USERMONEY_NTF*)pMsg;

	SERVER_KEY sServerKey = pServerSession->GetServerKey();

	GameStatisticsData* pGameData = pServerSession->GetStatisticsData();
	pGameData->SetMoney( pMsgNTF->ui64TotalGetMoney, pMsgNTF->ui64TotalLoseMoney, pMsgNTF->ui64TotalUserMoney );

	MessageOut(eFULL_LOG, _T("GM_USERMONEY_NTF[W:%u C:%u S:%u] Set(%I64u,%I64u,%I64u)"),
		sServerKey.GetWorldID(), sServerKey.GetChannelID(), sServerKey.GetServerID(), 
		pMsgNTF->ui64TotalGetMoney, pMsgNTF->ui64TotalLoseMoney, pMsgNTF->ui64TotalUserMoney );
}

//// 정상 종료 프로토콜
//HANDLER_GM_IMPL(SERVERCOMMON_SERVERSHUTDOWN_ANS)
//{
//	//// 종료 대기중인 놈이 종료된 것이므로 체크
//	//if( MasterServer::Instance()->IsExitServer() && MasterServer::Instance()->GetExitType() == eEXIT_GAMEWAIT )
//	//{
//	//	MasterServer::Instance()->RemoveExitServer( pServerSession->GetServerKey() );	// 종료되었다!
//	//	MessageOut( eCRITICAL_LOG, "Game Exit Message Recv[%u]", pServerSession->GetServerKey() );
//	//}
//}

// 통화량 통계
HANDLER_GM_IMPL(GM_OPERATION_USERMONEY_ANS)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_GM_OPERATION_USERMONEY_ANS* pMsgANS = (MSG_GM_OPERATION_USERMONEY_ANS*)pMsg;

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	OpServerSession* pOpServer = pMasterServer->ConnectSessions.SessionOpServer();

	GameStatisticsData* pGameData = pServerSession->GetStatisticsData();

	if( pOpServer && pOpServer->IsConnected() )
	{
		pGameData->SetMoney( pMsgANS->ui64TotalPlusMoney, pMsgANS->ui64TotalMinusMoney, pMsgANS->ui64UserMoney );
		pGameData->SetWarehouseMoney( pMsgANS->ui64WarehousePlusMoney, pMsgANS->ui64WarehouseMinusMoney, pMsgANS->ui64WarehouseAveMoney );
		pGameData->SetJackpotMoney( pMsgANS->usJackpotCnt, pMsgANS->JackpotMoney );
	}
	else
	{
		pGameData->ClearMoney();
		pGameData->ClearWarehouseMoney();
		pGameData->ClearJackpotMoney();
	}

	SERVER_KEY serverKey = pServerSession->GetServerKey();

	MessageOut( eCRITICAL_LOG, _T("UserMoney Recv[%u/%u/%u/%u][Get:%I64u][Lose:%I64u][User:%I64u]"),
		serverKey.GetWorldID(), serverKey.GetChannelID(), serverKey.GetServerType(), serverKey.GetServerID(),
		pMsgANS->ui64TotalPlusMoney, pMsgANS->ui64TotalMinusMoney, pMsgANS->ui64UserMoney );

	MessageOut( eCRITICAL_LOG, _T("WareHouseMoney Recv[%u/%u/%u/%u][Get:%I64u][Lose:%I64u][Total:%I64u]"),
		serverKey.GetWorldID(), serverKey.GetChannelID(), serverKey.GetServerType(), serverKey.GetServerID(),
		pMsgANS->ui64WarehousePlusMoney, pMsgANS->ui64WarehouseMinusMoney, pMsgANS->ui64WarehouseAveMoney );

	MessageOut( eCRITICAL_LOG, _T("JackpotMoney Recv[%u/%u/%u/%u][Cnt:%u][Money:%I64u]"),
		serverKey.GetWorldID(), serverKey.GetChannelID(), serverKey.GetServerType(), serverKey.GetServerID(),
		pMsgANS->usJackpotCnt, pMsgANS->JackpotMoney );
	/*
	MSG_MO_RTTG_USERMONEY_ANS		sendMsg;
	SERVER_KEY	serverKey = pServerSession->GetServerKey();

	sendMsg.byWorldID = serverKey.GetWorldID();
	sendMsg.byChannelID = serverKey.GetChannelID();
	sendMsg.byServerType = serverKey.GetServerType();
	sendMsg.byServerID = serverKey.GetServerID();

	sendMsg.ui64TotalUserMoney = pMsgANS->ui64UserMoney;
	sendMsg.ui64TotalGetMoney = pMsgANS->ui64TotalPlusMoney;
	sendMsg.ui64TotalLoseMoney = pMsgANS->ui64TotalMinusMoney;

	ServerSession *pOpServer = MasterServer::Instance()->GetOpServer();
	if( !pOpServer )
	{
	MessageOut( eCRITICAL_LOG, "RTTA 통화량 보고 실패:No Data[%u/%u/%u/%u]", sendMsg.byWorldID, 
	sendMsg.byChannelID, 
	sendMsg.byServerType, 
	sendMsg.byServerID );
	return;
	}

	pOpServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
	*/
}

HANDLER_GM_IMPL(GM_OPERATION_STATISTICS_ANS)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_GM_OPERATION_STATISTICS_ANS* pMsgANS = (MSG_GM_OPERATION_STATISTICS_ANS*)pMsg;

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	OpServerSession* pOpServer = pMasterServer->ConnectSessions.SessionOpServer();
	
	GameStatisticsData* pGameData = pServerSession->GetStatisticsData();

#ifdef _DEBUG
	int iMaxLen = 1024;
	int iWritten = 0;

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	TCHAR* ptszLog = (TCHAR*)PACKET_CONTROL_SM::GetBasePacketPtr4Send();
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	TCHAR ptszLog[1024];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	TCHAR* pWriteOff = ptszLog;

	iWritten = _sntprintf( pWriteOff, iMaxLen, _T("DEBUG CODE - ") );
	pWriteOff += iWritten;
	iMaxLen -= iWritten;
#endif //_DEBUG

	if( pOpServer && pOpServer->IsConnected() )
	{
		pGameData->SetAveMoney( pMsgANS->ui64UserMoney, pMsgANS->ui64WarehouseAveMoney );
		for( DWORD i=0 ; i<(DWORD)eSTATISTIC_MAX ; ++i )
		{
			pGameData->AddStatistics( (eSTATISTICS_TYPE)i, pMsgANS->ui64StatisticsType[i] );
#ifdef _DEBUG

			iWritten = _sntprintf( pWriteOff, iMaxLen, _T("[l64u]"), pMsgANS->ui64StatisticsType[i] );
			pWriteOff += iWritten;
			iMaxLen -= iWritten;
#endif //_DEBUG
		}

#ifdef _DEBUG
		MessageOut( eCRITICAL_LOG, ptszLog );
#endif //_DEBUG
	}
	else
	{
		pGameData->ClearAllStatistics();
	}

	SERVER_KEY serverKey = pServerSession->GetServerKey();
	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_DEFAULT,
		_T("Statistics Information Recv[%u/%u/%u/%u][User:%I64u][Warehouse:%I64u]"),
		serverKey.GetWorldID(), serverKey.GetChannelID(), serverKey.GetServerType(), serverKey.GetServerID(),
		pMsgANS->ui64UserMoney, pMsgANS->ui64WarehouseAveMoney );
}

#ifdef __NA000000_070523_REPORT_TO_SOLARAUTH
HANDLER_GM_IMPL(GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF* pRecvNTF =
		(MSG_GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF*)pMsg;

	if( sizeof(*pRecvNTF) != (SIZE_T)wSize )
	{
		MessageOut( eCRITICAL_LOG, _T("INVALID REPORT INFO") );
		return;
	}

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();

	SolarAuthServerSession* pSession = pMasterServer->ConnectSessions.SessionSolarAuth();

	if( FALSE == (
		(pMasterServer->GetPhaseStatus() == PHASE_SERVICE) &&
		pSession && pSession->IsConnected()
		)
	)
	{
		MessageOut( eCRITICAL_LOG, _T("CAN'T REPORT INFO") );
		return;
	}

	MSG_SM_SERVER_INFO_CACHEITEM_REPORT_NTF msgNTF;
	sCACHEITEM_REPORT_INFO& rINFO_SRC = pRecvNTF->m_INFO;
	sCACHEITEM_REPORT_INFO_MINORVER& rINFO_DEST = msgNTF.m_INFO;
	//INT			dwPriceGuid;
	//WORD		wItemCode;
	//BYTE		byCategoryID;
	//DWORD		dwPrice;

	rINFO_DEST.dwPriceGuid	= rINFO_SRC.dwPriceGuid;
	rINFO_DEST.wItemCode	= rINFO_SRC.wItemCode;
	rINFO_DEST.byCategoryID = rINFO_SRC.byCategoryID;
	rINFO_DEST.dwPrice		= rINFO_SRC.dwPrice;

	pSession->SendPacket( &msgNTF, sizeof(msgNTF) );

}
#endif //

