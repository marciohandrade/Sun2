#include "stdafx.h"
#include "Statistics.h"

#include <MasterServerEx.h>
#include <Sessions/ServerSessionManager.h>
#include <Sessions/OpServerSession.h>

VOID	GameStatistics::SendPacket_SumMoneyCirculation( MSG_MO_RTTG_USERMONEY_ANS* pMsgANS )
{
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();

	OpServerSession* pOpServer = pMasterServer->ConnectSessions.SessionOpServer();

	if( !(pOpServer && pOpServer->IsConnected()) )
	{
		MessageOut( eCRITICAL_LOG, _T("RTTA 통화량 보고 불가능:No OpServer") );
		return;
	}

	ServerTreeNODE* pServerTREE = ServerSessionManager::GetServerTree();

	ServerTreeNODE::WORLD_LIST* pWorldList = pServerTREE->GetWorldList();
	DWORD dwWorldCount = (DWORD)pWorldList->size();

	const DWORD cdwBitMask = (DWORD)((1<<BATTLE_SERVER) | (1<<FIELD_SERVER) | (1<<GAME_SERVER));

	for( DWORD i=0 ; i<dwWorldCount ; ++i )
	{
		ServerTreeNODE::CHANNEL_LIST* pChannelList = (*pWorldList)[i];
        if (pChannelList == NULL) {
            continue;
        }
		DWORD dwChannelCount = (DWORD)pChannelList->size();

		for( DWORD j=0 ; j<dwChannelCount ; ++j )
		{
			ServerTreeNODE::SERVER_NODE_LIST* pNodeList = (*pChannelList)[j];
            if (pNodeList == NULL) {
                continue;
            }
			pMsgANS->CirculateMoney = 0;
			pMsgANS->ui64TotalUserMoney = 0;
			pMsgANS->ui64TotalGetMoney = 0;
			pMsgANS->ui64TotalLoseMoney = 0;
			pMsgANS->ui64WarehouseAveMoney = 0;
			pMsgANS->ui64WarehousePlusMoney = 0;
			pMsgANS->ui64WarehouseMinusMoney = 0;
			pMsgANS->JackpotMoney = 0;
			pMsgANS->usJackpotCnt = 0;

			DWORD dwInc = 0;

			ServerTreeNODE::SERVER_NODE_LIST::iterator it = pNodeList->begin();
			ServerTreeNODE::SERVER_NODE_LIST::iterator end = pNodeList->end();

			for( ; it != end ; ++it )
			{
				sSERVER_NODE* pNode = *it;
				if( pNode && ( (1<<pNode->m_ServerType) & cdwBitMask ) )
				{
					GameServerSession* pGameServer = (GameServerSession*)pNode->m_pServerSession;
					GameStatisticsData* pGameData = pGameServer->GetStatisticsData();
					MakePacket_SumMoneyCirculation_GatheringInfo( pGameData, pMsgANS );
					++dwInc;
				}
			}

			if( dwInc )
			{
				// 처리된 채널에 GameServer존재했으며, 이를 위해 처리되었음.
				// 채널 당 통화량 계산 완료 및 보고
				pMsgANS->byWorldID = (BYTE)i;
				pMsgANS->byChannelID = (BYTE)j;
				pMsgANS->byServerType = 0;
				pMsgANS->byServerID = 0;

				pOpServer->Send( (BYTE*)pMsgANS, sizeof(*pMsgANS) );
				MessageOutEx( g_pSunLog,
					CSunLog::cMIDDLE_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_DEFAULT,
					_T("통화량 통계 [W:%u][C:%u] - 통화량[%I64u] 잭팟[%I64u]"),
					i, j, pMsgANS->CirculateMoney, pMsgANS->JackpotMoney
					);
			}

#ifdef _DEBUG
			MessageOut( eFULL_LOG, "UserMoney Send[%u/%u][Get:%I64u][Lose:%I64u][User:%I64u]",
				pMsgANS->byWorldID,
				pMsgANS->byChannelID,
				pMsgANS->ui64TotalGetMoney,
				pMsgANS->ui64TotalLoseMoney,
				pMsgANS->ui64TotalUserMoney );

			MessageOut( eFULL_LOG, "WareHouseMoney Send[%u/%u][Get:%I64u][Lose:%I64u][User:%I64u]",
				pMsgANS->byWorldID,
				pMsgANS->byChannelID,
				pMsgANS->ui64WarehousePlusMoney,
				pMsgANS->ui64WarehouseMinusMoney,
				pMsgANS->ui64WarehouseAveMoney );

			MessageOut( eFULL_LOG, "JackpotMoney Send[%u/%u][Cnt:%u][Money:%I64u]",
				pMsgANS->byWorldID,
				pMsgANS->byChannelID,
				pMsgANS->usJackpotCnt,
				pMsgANS->JackpotMoney );
#endif
		} // for "channel"

	} // for "world"
}

VOID	GameStatistics::MakePacket_SumMoneyCirculation_GatheringInfo( GameStatisticsData* pGameData, MSG_MO_RTTG_USERMONEY_ANS* pMsgANS )
{
	ULONGLONG ui64TempUserMoney;
	
	ui64TempUserMoney = pGameData->GetAveUserMoney();
	pMsgANS->ui64TotalUserMoney = min( pMsgANS->ui64TotalUserMoney + ui64TempUserMoney, ULLONG_MAX );

	ui64TempUserMoney = pGameData->GetAveWarehouseMoney();
	pMsgANS->ui64WarehouseAveMoney = min( pMsgANS->ui64WarehouseAveMoney + ui64TempUserMoney, ULLONG_MAX );

	//

	// 필요한 통계만 수집
	for( DWORD j = 0; j < eSTATISTIC_MAX; ++j )
	{
		ui64TempUserMoney = pGameData->GetStatisticsType( (eSTATISTICS_TYPE)j );
		switch( (eSTATISTICS_TYPE)j )
		{
		case eSTATISTIC_MONEY_CIRCULATION:
			{
				pMsgANS->CirculateMoney = min( pMsgANS->CirculateMoney + ui64TempUserMoney, ULLONG_MAX );
				pGameData->ClearStatistics( (eSTATISTICS_TYPE)j );
				break;
			}
		case eSTATISTIC_MONEY_PLUS:
			{
				pMsgANS->ui64TotalGetMoney = min( pMsgANS->ui64TotalGetMoney + ui64TempUserMoney, ULLONG_MAX );
				pGameData->ClearStatistics( (eSTATISTICS_TYPE)j );
				break;
			}
		case eSTATISTIC_MONEY_MINUS:
			{
				pMsgANS->ui64TotalLoseMoney = min( pMsgANS->ui64TotalLoseMoney + ui64TempUserMoney, ULLONG_MAX );
				pGameData->ClearStatistics( (eSTATISTICS_TYPE)j );
				break;
			}
		case eSTATISTIC_MONEY_WAREHOUSE_PLUS:
			{
				pMsgANS->ui64WarehousePlusMoney = min( pMsgANS->ui64WarehousePlusMoney + ui64TempUserMoney, ULLONG_MAX );
				pGameData->ClearStatistics( (eSTATISTICS_TYPE)j );
				break;
			}
		case eSTATISTIC_MONEY_WAREHOUSE_MINUS:
			{
				pMsgANS->ui64WarehouseMinusMoney = min( pMsgANS->ui64WarehouseMinusMoney + ui64TempUserMoney, ULLONG_MAX );
				pGameData->ClearStatistics( (eSTATISTICS_TYPE)j );
				break;
			}
		case eSTATISTIC_JACKPOT_COUNT:
			{
				pMsgANS->usJackpotCnt = min( pMsgANS->usJackpotCnt + (USHORT)ui64TempUserMoney, USHRT_MAX );
				pGameData->ClearStatistics( (eSTATISTICS_TYPE)j );
				break;
			}
		case eSTATISTIC_JACKPOT_MONEY:
			{
				pMsgANS->JackpotMoney = min( pMsgANS->JackpotMoney + ui64TempUserMoney, ULLONG_MAX );
				pGameData->ClearStatistics( (eSTATISTICS_TYPE)j );
				break;
			}
		} // switch
	} // for

	// WOPS에 값 넘겨주기 위해 계산되었으므로 지금의 정보 초기화
	pGameData->ClearMoney();
}

VOID	GameStatistics::SendPacket_StatisticsEtcInfo( MSG_MO_RTTG_GAMESTATISTICS_ANS* pMsgANS )
{
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();

	OpServerSession* pOpServer = pMasterServer->ConnectSessions.SessionOpServer();

	if( !(pOpServer && pOpServer->IsConnected()) )
	{
		MessageOut( eCRITICAL_LOG, _T("RTTA 통화량 보고 불가능:No OpServer") );
		return;
	}

	ServerTreeNODE* pServerTREE = ServerSessionManager::GetServerTree();

	ServerTreeNODE::WORLD_LIST* pWorldList = pServerTREE->GetWorldList();
	DWORD dwWorldCount = (DWORD)pWorldList->size();

	const DWORD cdwBitMask = (DWORD)((1<<BATTLE_SERVER) | (1<<FIELD_SERVER) | (1<<GAME_SERVER));

	for( DWORD i=0 ; i<dwWorldCount ; ++i )
	{
		ServerTreeNODE::CHANNEL_LIST* pChannelList = (*pWorldList)[i];
        if (pChannelList == NULL) {
            continue;
        }
		DWORD dwChannelCount = (DWORD)pChannelList->size();
		for( DWORD j=0 ; j<dwChannelCount ; ++j )
		{
			ServerTreeNODE::SERVER_NODE_LIST* pNodeList = (*pChannelList)[j];
            if (pNodeList == NULL) {
                continue;
            }
			memset( pMsgANS->ui64Statitics, 0, sizeof(pMsgANS->ui64Statitics) );

			DWORD dwInc = 0;

			ServerTreeNODE::SERVER_NODE_LIST::iterator it = pNodeList->begin();
			ServerTreeNODE::SERVER_NODE_LIST::iterator end = pNodeList->end();

			for( ; it != end ; ++it )
			{
				sSERVER_NODE* pNode = *it;
				if( pNode && ( (1<<pNode->m_ServerType) & cdwBitMask ) )
				{
					GameServerSession* pGameServer = (GameServerSession*)pNode->m_pServerSession;
					GameStatisticsData* pGameData = pGameServer->GetStatisticsData();
					MakePacket_StatisticsEtcInfo_GatheringInfo( pGameData, pMsgANS );
					++dwInc;
				}
			}

			if( dwInc )
			{
				// 처리된 채널에 GameServer존재했으며, 이를 위해 처리되었음.
				// 채널 당 통화량 계산 완료 및 보고
				pMsgANS->byWorldID = (BYTE)i;
				pMsgANS->byChannelID = (BYTE)j;
				pMsgANS->byServerType = 0;
				pMsgANS->byServerID = 0;

				pOpServer->Send( (BYTE*)pMsgANS, sizeof(*pMsgANS) );

				MessageOutEx( g_pSunLog,
					CSunLog::cMIDDLE_LOG, CSunLog::cOUTPUT_FILE, CSunLog::cCATE_DEFAULT,
					_T("기타 통계 [W:%u][C:%u] - "), i, j );
			}

#ifdef _DEBUG
			MessageOut( eFULL_LOG, "GameStatistics Send[%u/%u]", pMsgANS->byWorldID, pMsgANS->byChannelID );
#endif
		} // for "channel"

	} // for "world"
}

VOID	GameStatistics::MakePacket_StatisticsEtcInfo_GatheringInfo(
													GameStatisticsData* pGameData,
													MSG_MO_RTTG_GAMESTATISTICS_ANS* pMsgANS )
{
	// 필요한 통계만 수집
	for( DWORD j = 0; j < eSTATISTIC_MAX; ++j )
	{
		switch( (eSTATISTICS_TYPE)j )
		{
		case eSTATISTIC_MONEY_CIRCULATION:
		case eSTATISTIC_MONEY_PLUS:
		case eSTATISTIC_MONEY_MINUS:
		case eSTATISTIC_MONEY_WAREHOUSE_PLUS:
		case eSTATISTIC_MONEY_WAREHOUSE_MINUS:
		case eSTATISTIC_JACKPOT_COUNT:
		case eSTATISTIC_JACKPOT_MONEY:
			{
				// 기존 통화량 정보에 포함되는 값들은 또 넣을 필요 없다
				break;
			}
		default:
			{
				ULONGLONG ui64Temp = pGameData->GetStatisticsType( (eSTATISTICS_TYPE)j );
				pGameData->ClearStatistics( (eSTATISTICS_TYPE)j );
				pMsgANS->ui64Statitics[j] = min( pMsgANS->ui64Statitics[j] + ui64Temp, ULLONG_MAX );
				break;
			}
		} // switch
	} // for
}
