#include "stdafx.h"

#include <MasterServerEx.h>
#include "MasterServerInfo.h"

#include <PacketStruct_GM.h>
#include <PacketStruct_MX.h>

#include <Sessions/ServerSessionManager.h>
#include <Sessions/ServerSessionEx.h>
#include <Sessions/AccountDBProxySession.h>

// WorkLoad
#include <WorkLoad/ExitProcess.WorkLoad.cxx>
#include <WorkLoad/Timer.WorkLoad.cxx>

BOOL MasterServerInfo::SendMX_FUNCRESTRICT_RELOAD_REQ( MasterServerEx* pMasterServer )
{
	MasterServerEx::sConnectSession& rSessions = pMasterServer->ConnectSessions;

	AccountDBProxySession* pSession = rSessions.SessionAccountDB();

	if( pSession && pSession->IsConnected() )
	{
		pMasterServer->RestrictManager().Clear();

		MSG_MX_FUNCRESTRICT_RELOAD_REQ msgREQ;

		pSession->SendPacket( &msgREQ, sizeof(msgREQ) );

		return TRUE;
	}

	return TRUE;
}

BOOL MasterServerInfo::SendGM_OPERATION_FUNCRESTRICT_CMD( MasterServerEx* pMasterServer )
{
	// 금지 기능들 정보 날려준다
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_GM_OPERATION_FUNCRESTRICT_CMD* pMsgCMD =
		new ( PACKET_CONTROL_SM::GetBasePacketPtr4Send() ) MSG_GM_OPERATION_FUNCRESTRICT_CMD();
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_GM_OPERATION_FUNCRESTRICT_CMD pMsgCMD[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	sRESTRICT_MGR::MakePacket::Make_GM_OPERATION_FUNCRESTRICT_CMD( &pMasterServer->RestrictManager(), pMsgCMD );

	ServerSessionManager::SendToGameServers( pMsgCMD, pMsgCMD->GetSize() );

	return TRUE;
}


BOOL MasterServerInfo::SendGM_OPERATION_STATISTICS_REQ( MasterServerEx* pMasterServer )
{
	sPTR_SERVER_NODE_ARRAY ptrARRAY;

	ServerSessionManager::GetGameServerList( &ptrARRAY );

	DWORD dwCount = ptrARRAY.m_dwCount;

	if( !dwCount )
		return FALSE;

	MSG_GM_OPERATION_STATISTICS_REQ	msgREQ;
	sSERVER_NODE** ppNODE = ptrARRAY.m_pPTRs;

	for( DWORD i=0 ; i<dwCount ; ++i, ++ppNODE )
	{
		ServerSessionEx* pGameServer = (*ppNODE)->m_pServerSession;
		pGameServer->SendPacket( &msgREQ, sizeof(msgREQ) );
	}

	return TRUE;
}


BOOL MasterServerInfo::MakeServerExitList(
							SERVER_KEY IN skExitKey,
							EXIT_SERVER_MAP*& OUT pRefAgentServerList,
							EXIT_SERVER_MAP*& OUT pRefDBPServerList,
							EXIT_SERVER_MAP*& OUT pRefServerList )
{
	//===========================================================================================
	// OpServer로 부터 오는 메시지 코드 정의
	//
	// WorldID | ChannID | ServerT | SvrIDX
	//    0    |    x    |    x    |   x		= 전체 World를 대상으로 한다.
	//    x    |   0xFF  |    x    |   x		= 해당 World의 전체 채널, 즉 해당 월드 전체 대상
	//    0    |    0    |    x    |   x		= 전체 World의 0번 채널을 대상으로
	//    0    |    1    |    0    |   x		= 전체 World의 1번 채널에 있는 서버 전체
	//
	//===========================================================================================

	const DWORD c_ALL = (DWORD)-1;
	const DWORD c_AGENT = (DWORD)(1<<AGENT_SERVER);
	const DWORD c_DBP = (DWORD)(1<<GAME_DBPROXY);
	const DWORD c_EXCEPT =
		(DWORD)(1<<ACCOUNT_DBPROXY) | (DWORD)(1<<ACCOUNT_DBPROXY) | (DWORD)(1<<OP_SERVER)
		| (DWORD)(1<<MASTER_SERVER) | (DWORD)(1<<AUTH_AGENT_SERVER) | (DWORD)(1<<TEMP_SERVER)
		| (DWORD)(1<<UNKNOWN_SERVER) | (DWORD)(1<<TOURNAMENT_SERVER) | (DWORD)(1<<SOLARAUTH_SERVER) ;

	DWORD dwWorldID		= (DWORD)skExitKey.GetWorldID();
	DWORD dwChannelID	= (DWORD)skExitKey.GetChannelID();
	DWORD dwServerTID	= (DWORD)skExitKey.GetServerType();
	DWORD dwServerIDX	= (DWORD)skExitKey.GetServerID();

	//

	dwWorldID = (dwWorldID == 0) ? c_ALL : 1<<dwWorldID;
	dwChannelID = (dwChannelID == 0xFF) ? c_ALL : 1<<dwChannelID;
	dwServerTID = (dwServerTID == 0) ? c_ALL : 1<<dwServerTID;
	dwServerIDX = (dwServerIDX == 0) ? c_ALL : 1<<dwServerIDX;
	dwServerTID &= (~c_EXCEPT);

	// 서버 종료 목록
	// Agent 목록 <- 제일 먼저 종료시킬 것
	// DBP 목록 <- 제일 나중에 종료시킬 것
	// 나머지 목록

	EXIT_SERVER_MAP* pAgentList = new EXIT_SERVER_MAP;
	EXIT_SERVER_MAP* pDBPList = new EXIT_SERVER_MAP;
	EXIT_SERVER_MAP* pServerList = new EXIT_SERVER_MAP;

	ServerTreeNODE* pTREE = ServerSessionManager::GetServerTree();

	DWORD dwUpperBound;
	sSERVER_NODE* pNODEs = pTREE->GetServerArrayList( dwUpperBound );
	sSERVER_NODE* pNODE = pNODEs;

	for( DWORD i=0 ; i<dwUpperBound ; ++i, ++pNODE )
	{
		DWORD dwServerTID_T = (DWORD)(1<<pNODE->m_ServerType);
		if( !(dwServerTID & dwServerTID_T) )
			continue;

		if( !pNODE->m_pServerSession )
			continue;

		SERVER_KEY& rKey = pNODE->m_ServerKey;
		DWORD dwWorldID_T = (DWORD)(1<<rKey.GetWorldID());
		DWORD dwChannelID_T = (DWORD)(1<<rKey.GetChannelID());
		DWORD dwServerIDX_T = (DWORD)(1<<rKey.GetServerID());

		if( !(dwWorldID & dwWorldID_T) ) continue;
		if( !(dwChannelID & dwChannelID_T) ) continue;
		if( !(dwServerIDX & dwServerIDX_T) ) continue;

		EXIT_SERVER_MAP* pList = NULL;
		if( c_AGENT == dwServerTID_T ) pList = pAgentList;
		else if( c_DBP == dwServerTID_T ) pList = pDBPList;
		else pList = pServerList;

		pList->insert( pList->end(), EXIT_SERVER_PAIR(pNODE->m_pServerSession, FALSE) );

	} // for

	// register IWorkNode for Exit Server Process
	pRefAgentServerList = pAgentList;
	pRefDBPServerList = pDBPList;
	pRefServerList = pServerList;

	return TRUE;
}

BOOL MasterServerInfo::RegisterServerExitProcess(
									  EXIT_SERVER_MAP*& OUT pAgentServerList,
									  EXIT_SERVER_MAP*& OUT pDBPServerList,
									  EXIT_SERVER_MAP*& OUT pServerList )
{
	PROCESS_EXIT_PROCESS* pWorkNode = new PROCESS_EXIT_PROCESS();

	pWorkNode->SetInformation( pAgentServerList, pDBPServerList, pServerList );

	WORK_KEY wkWorkKey = WorkLoadManager::RegisterTask( pWorkNode );
	if( WorkLoadManager::c_NotExistKey != wkWorkKey )
	{
		GetWorkLoadContainer().RegisterWork( wkWorkKey, (DWORD)(__int64)pWorkNode );
		m_wkExitProcess = wkWorkKey;
		return TRUE;
	}

	return FALSE;
}

VOID MasterServerInfo::StartTimerProcess( DWORD dwInterval )
{
	if( dwInterval == 0 )
		m_dwMoneyTimerInterval = 10000;
	else
		m_dwMoneyTimerInterval = dwInterval;
	m_dwMoneyTimer = GetTickCount() + m_dwMoneyTimerInterval;

	//m_pMoneyTimer = new util::SimpleTimer();
	//m_pMoneyTimer->SetTimer( m_pMasterServer->StatisticsInterval() );

	// 요놈은 좀더 정리되면 사용하자.
	PROCESS_TIMER_PROCESS* pWorkNode = new PROCESS_TIMER_PROCESS();
	WORK_KEY wkWorkKey = WorkLoadManager::RegisterTask( pWorkNode );
	if( WorkLoadManager::c_NotExistKey != wkWorkKey )
	{
		GetWorkLoadContainer().RegisterWork( wkWorkKey, (DWORD)(__int64)pWorkNode );
		//m_wkTimerProcess = wkWorkKey;
	}
}


VOID	MasterServerInfo::Update()
{
	DWORD dwCurTick = GetTickCount();
	if( !(m_dwMoneyTimer > dwCurTick) )
	{
		m_dwMoneyTimer = dwCurTick + m_dwMoneyTimerInterval;

		SendGM_OPERATION_STATISTICS_REQ( m_pMasterServer );
	}
//	if( m_pMoneyTimer->IsExpired() )
//	{
//MessageOut( eMIDDLE_LOG, _T("Timer SendGM_OPERATION_STATISTICS_REQ\n") );
//		SendGM_OPERATION_STATISTICS_REQ( m_pMasterServer );
//	}
}




