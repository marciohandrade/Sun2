#include "stdafx.h"
#include <Protocol_GM.h>
#include <PacketStruct_GM.h>
#include "Handler_GM.h"
#include "GameServerEx.h"
#include <ServerSession.h>

// 서버 정상종료 프로토콜 위해
#include <Protocol_ServerCommon.h>
#include <PacketStruct_ServerCommon.h>

#include "FunctionalRestrictManager.h"
#include "StatisticsManager.h"
#include "PeakTimeEvent.h" //_NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "GameShop/GameShop.Manager.h"
#endif

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_GM::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_GM::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(GM_CONNECTION, GM_CONNECTION_GROUND_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GM_CONNECTION, GM_CONNECTION_SERVER_INFO_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GM_OPERATION, GM_RELOAD_DATA_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_REQ, PHASE_SERVICE),
        HANDLER_NODE_INFO(GM_OPERATION, GM_OPERATION_USERMONEY_REQ, PHASE_SERVICE),
        HANDLER_NODE_INFO(GM_OPERATION, GM_OPERATION_FUNCRESTRICT_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GM_OPERATION, GM_OPERATION_STATISTICS_REQ, PHASE_SERVICE),

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
        HANDLER_NODE_INFO(GM_CONNECTION, GM_CONNECTION_DISCONNECT_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GM_CONNECTION, GM_CONNECTION_PHASE_SHIFT_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(GM_CONNECTION, GM_CONNECTION_BLOCK_CMD, PHASE_SERVICE),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_SERVICE),
        HANDLER_NODE_INFO(SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD, PHASE_SERVICE),
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

void Handler_GM::OnGM_CONNECTION_GROUND_CMD(ServerSession *pMasterServer, MSG_BASE* pMsg, WORD wSize)
{
    __UNUSED((pMasterServer, pMsg, wSize));
}

VOID Handler_GM::OnGM_CONNECTION_SERVER_INFO_CMD( ServerSession *pMasterServer, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pMasterServer, pMsg, wSize));
	MSG_GM_CONNECTION_SERVER_INFO_CMD *pRecvMsg = (MSG_GM_CONNECTION_SERVER_INFO_CMD*)pMsg;
	
	g_pGameServer->SetKey( pRecvMsg->m_ServerKey );
	MessageOut( eCRITICAL_LOG, "마스터로부터 받은 서버키 셋팅(%u) (%u, %u, %u, %u)", pRecvMsg->m_ServerKey,
		pRecvMsg->m_ServerKey.GetWorldID(), pRecvMsg->m_ServerKey.GetChannelID(), pRecvMsg->m_ServerKey.GetServerType(), pRecvMsg->m_ServerKey.GetServerID() );
	g_pGameServer->UpdateAppTitle();

	g_pGameServer->SetGuid( pRecvMsg->m_dwServerGUID );
    PeakTimeEvent::Instance()->SieveEventsByWorldId();

	if( !g_pGameServer->GetSession( GAME_DBPROXY ) )
		g_pGameServer->InitConnectSession( GAME_DBPROXY, pRecvMsg->m_szDBProxyServerIP, pRecvMsg->m_wDBProxyServerPort );
	if( !g_pGameServer->GetSession( AGENT_SERVER ) )
		g_pGameServer->InitConnectSession( AGENT_SERVER, pRecvMsg->m_szAgentServerIP, pRecvMsg->m_wAgentServerPort );
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	if( !g_pGameServer->GetSession( GUILD_SERVER ) )
		g_pGameServer->InitConnectSession( GUILD_SERVER, pRecvMsg->m_szGuildServerIP, pRecvMsg->m_wGuildServerPort );
#endif
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	if( !g_pGameServer->GetSession( PORTAL_SERVER ) )
		g_pGameServer->InitConnectSession( PORTAL_SERVER, pRecvMsg->m_szPortalServerIP, pRecvMsg->m_wPortalServerPort );
#endif
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    if (GameShop::Manager* shop_manager = GameShop::Manager::Instance()) {
        shop_manager->Initialize(pRecvMsg->m_szShopServerIP, pRecvMsg->m_wShopServerPort);
    };
#else //if !defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
	if( !g_pGameServer->GetSession( SHOP_SERVER ) )
		g_pGameServer->InitConnectSession( SHOP_SERVER, pRecvMsg->m_szShopServerIP, pRecvMsg->m_wShopServerPort );
#endif
//==================================================================================================
#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//==================================================================================================
	// Game DB proxy와 연결 시도
	g_pGameServer->ConnectTo( GAME_DBPROXY );
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
	g_pGameServer->ConnectTo( GUILD_SERVER );
#endif
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
	g_pGameServer->ConnectTo( PORTAL_SERVER );
#endif
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    GameShop::Manager::Instance()->StartService();
#else
	g_pGameServer->ConnectTo( SHOP_SERVER );
#endif
//==================================================================================================
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//==================================================================================================

	if( g_pGameServer->IsServerInfoFromFile() )
	{
		// Game DB proxy와 연결 시도
		g_pGameServer->ConnectTo( GAME_DBPROXY );
    #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
		g_pGameServer->ConnectTo( GUILD_SERVER );
    #endif
    #if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
		g_pGameServer->ConnectTo( PORTAL_SERVER );
    #endif
    #if !defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
		g_pGameServer->ConnectTo( SHOP_SERVER );
    #endif
	}

//==================================================================================================
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//==================================================================================================
}

VOID Handler_GM::OnGM_RELOAD_DATA_CMD( ServerSession *pMasterServer, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pMasterServer, pMsg, wSize));
	g_pGameServer->ReloadData();
}

VOID Handler_GM::OnSERVERCOMMON_SERVERSHUTDOWN_REQ( ServerSession *pMasterServer, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pMasterServer, pMsg, wSize));
	// 종료한다고 알려줘~
	MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS	sendMsg;

	sendMsg.m_byCategory = SERVERCOMMON;
	sendMsg.m_byProtocol = SERVERCOMMON_SERVERSHUTDOWN_ANS;

	pMasterServer->Send( (BYTE*)&sendMsg, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS) );

	// 정상종료 들어간다!
	g_pGameServer->Shutdown();
}

VOID Handler_GM::OnGM_OPERATION_USERMONEY_REQ( ServerSession *pMasterServer, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pMasterServer, pMsg, wSize));
	// 현재까지 갖고 있던 정보를 전송
	MSG_GM_OPERATION_USERMONEY_ANS	sendMsg;

	if( PlayerManager::Instance()->GetNumberOfPlayers() > 0)
	{
		sendMsg.ui64UserMoney = PlayerManager::Instance()->GetAveUserTotalMoney();
	}
	else
	{
		sendMsg.ui64UserMoney = 0;
	}

	sendMsg.ui64TotalPlusMoney = g_StatisticsManager.GetStatisticType(eSTATISTIC_MONEY_PLUS);
	sendMsg.ui64TotalMinusMoney = g_StatisticsManager.GetStatisticType(eSTATISTIC_MONEY_MINUS);

	// 창고 금액도 같이 전송
	if( PlayerManager::Instance()->GetNumberOfPlayers() > 0)
	{
		sendMsg.ui64WarehouseAveMoney = PlayerManager::Instance()->GetAveWarehouseTotalMoney();
	}
	else
	{
		sendMsg.ui64WarehouseAveMoney = 0;
	}

	sendMsg.ui64WarehousePlusMoney = g_StatisticsManager.GetStatisticType(eSTATISTIC_MONEY_WAREHOUSE_PLUS);
	sendMsg.ui64WarehouseMinusMoney = g_StatisticsManager.GetStatisticType(eSTATISTIC_MONEY_WAREHOUSE_MINUS);

	sendMsg.usJackpotCnt = (USHORT)g_StatisticsManager.GetStatisticType(eSTATISTIC_JACKPOT_COUNT);
	sendMsg.JackpotMoney = g_StatisticsManager.GetStatisticType(eSTATISTIC_JACKPOT_MONEY);

	//sendMsg.CirculatingMoney	= g_StatisticsManager.GetStatisticType(eSTATISTIC_MONEY_CIRCULATION);
	//sendMsg.EnchantCount		= (USHORT)g_StatisticsManager.GetStatisticType(eSTATISTIC_ENCHANT_COUNT);
	//sendMsg.RankupCount		= (USHORT)g_StatisticsManager.GetStatisticType(eSTATISTIC_RANKUP_COUNT);

	pMasterServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
#ifdef _DEBUG
	MessageOut( eCRITICAL_LOG, "UserMoney Send[Tot:%I64u][Get:%I64u][Lose:%I64u]", sendMsg.ui64UserMoney, sendMsg.ui64TotalPlusMoney, sendMsg.ui64TotalMinusMoney );
	MessageOut( eCRITICAL_LOG, "WarehouseMoney Send[Tot:%I64u][Get:%I64u][Lose:%I64u]", sendMsg.ui64WarehouseAveMoney,
																						sendMsg.ui64WarehousePlusMoney,
																						sendMsg.ui64WarehouseMinusMoney );
	MessageOut( eCRITICAL_LOG, "JackpotMoney Send[Cnt:%u][Money:%I64u]", sendMsg.usJackpotCnt, sendMsg.JackpotMoney );
#endif
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_MONEY_CIRCULATION );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_MONEY_PLUS );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_MONEY_MINUS );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_MONEY_WAREHOUSE_PLUS );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_MONEY_WAREHOUSE_MINUS );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_JACKPOT_COUNT );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_JACKPOT_MONEY );
	/*g_StatisticsManager.ClearStatisticType( eSTATISTIC_FEELMISTONE_CREATE );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_FEELMISTONE_DESTROY );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_OVERMISTONE_CREATE );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_OVERMISTONE_DESTROY );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_ENCHANT_COUNT );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_RANKUP_COUNT );
	g_StatisticsManager.ClearStatisticType( eSTATISTIC_CRYSTALIZE_COUNT );
	*/

	// 총 통화량 로그
	GAMELOG->LogTotalMoney( sendMsg.ui64TotalPlusMoney, sendMsg.ui64TotalMinusMoney, 
		PlayerManager::Instance()->GetNumberOfPlayers(),
		PlayerManager::Instance()->GetUserTotalMoney() );

	// 총 창고 통화량 로그
	GAMELOG->LogTotalWarehouseMoney( sendMsg.ui64WarehousePlusMoney, sendMsg.ui64WarehouseMinusMoney, 
		PlayerManager::Instance()->GetNumberOfPlayers(),
		PlayerManager::Instance()->GetWarehouseTotalMoney() );

}

// 전체 통계 요청
VOID Handler_GM::OnGM_OPERATION_STATISTICS_REQ( ServerSession *pMasterServer, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pMasterServer, pMsg, wSize));
	MSG_GM_OPERATION_STATISTICS_ANS	sendMsg;

	// 일단 보유금액 설정
	if( PlayerManager::Instance()->GetNumberOfPlayers() > 0)
	{
		sendMsg.ui64UserMoney = PlayerManager::Instance()->GetAveUserTotalMoney();
	}
	else
	{
		sendMsg.ui64UserMoney = 0;
	}
	if( PlayerManager::Instance()->GetNumberOfPlayers() > 0)
	{
		sendMsg.ui64WarehouseAveMoney = PlayerManager::Instance()->GetAveWarehouseTotalMoney();
	}
	else
	{
		sendMsg.ui64WarehouseAveMoney = 0;
	}

	for( BYTE byCnt = 0; byCnt < eSTATISTIC_MAX; byCnt++ )
	{
		sendMsg.ui64StatisticsType[byCnt] = g_StatisticsManager.GetStatisticType( static_cast<eSTATISTICS_TYPE>(byCnt) );
		g_StatisticsManager.ClearStatisticType( static_cast<eSTATISTICS_TYPE>(byCnt) );
	}
	pMasterServer->Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
#ifdef _DEBUG
	MessageOut( eCRITICAL_LOG, "StatisticsInfo Send[User:%I64u][Warehouse:%I64u]", sendMsg.ui64UserMoney, sendMsg.ui64WarehouseAveMoney );
#endif
}

// 기능 금지
VOID Handler_GM::OnGM_OPERATION_FUNCRESTRICT_CMD( ServerSession *pMasterServer, MSG_BASE *pMsg, WORD wSize )
{
    __TOUCHED((pMasterServer, pMsg, wSize));
	MSG_GM_OPERATION_FUNCRESTRICT_CMD* pRecvMsg = (MSG_GM_OPERATION_FUNCRESTRICT_CMD*)pMsg;

	g_FunctionalRestrictMgr.SetRestrictFunction( eFUNCTIONALRESTRICT_ALLOW );

	for( BYTE i=0; i<pRecvMsg->byRestrictNum; ++i )
	{
		if( !pRecvMsg->SRESTRICT[i].bAllow )
		{
			g_FunctionalRestrictMgr.AddFunctionalRestrict( pRecvMsg->SRESTRICT[i].dwFunc );
			MessageOut( eCRITICAL_LOG, "Functional Restrict[%u]", pRecvMsg->SRESTRICT[i].dwFunc );
		}
		else
		{
			g_FunctionalRestrictMgr.RemoveFunctionalRestrict( pRecvMsg->SRESTRICT[i].dwFunc );
			MessageOut( eCRITICAL_LOG, "Functional Allow[%u]", pRecvMsg->SRESTRICT[i].dwFunc );
		}
	}
}


//-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

HandlerFromMasterServer_IMPL( GM_CONNECTION_DISCONNECT_CMD )
{
	printf( "RECEIVED [GM_CONNECTION_DISCONNECT_CMD]\n" );
}

// MASTER SERVER "SM_CONNECTION_PHASE_SHIFT_CMD" 변경시 참조해야 한다.
// 애초, SM_CONNECTION_PHASE_SHIFT_CMD를 수정할 때 일괄적으로 변경해야 한다.
HandlerFromMasterServer_IMPL( GM_CONNECTION_PHASE_SHIFT_CMD )
{
	MSG_GM_CONNECTION_PHASE_SHIFT_CMD* pCMD = (MSG_GM_CONNECTION_PHASE_SHIFT_CMD*)pMsg;
	ePHASE_SHIFT ePhase = (ePHASE_SHIFT)pCMD->m_dwPHASE_SHIFT;

	ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);	// BLOCK CMD일 수 있다.
	ePHASE_SHIFT prevPhase = g_pGameServer->GetPhaseStatus();

	MasterServerSession* pMasterServer = (MasterServerSession*)pServerSession;

	switch( ePhase )
	{
	case PHASE_UNKNOWN:
		// 인증 단계를 다시 거치라는 의미
		break;
	case PHASE_EXCHANGE:
		{
			
			if( PHASE_EXCHANGE == prevPhase )
				return;

			g_pGameServer->SetPhaseStatus( ePhase );

			if( PHASE_SERVICE == prevPhase )
			{
				// 다시 EXCHANGE상태로 전환되었다는 것은 서버 연결 상의 문제 때문에 제약을...
				// PHASE_SERVICE -> PHASE_EXCHANGE
				// 이것은 PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE가 존재한다는 의미

				// 데이터를 다시 받아야 하는지 등등을 NOTIFY_SIGNAL로 해결해 보자.
				eSCRIPT_DATA_CMD cmd =
					SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2EMPTYDATA;

				ScriptDataManager::DataStatusCommand( cmd, SCRIPT_NOTIFY_SIGNAL );

			}
			else
			{
				//
				// PHASE_UNKNOWN -> PHASE_EXCHANGE
				//
				// 서비스로의 전환이므로 파일 전송등등, 여러가지의 SolarAuth관련 서비스를
				// 받을 수 있다. 또는 주어진 CMD에 대한 처리를 수행할 수 있다.
				// 이곳에서 WorkLoad설정하면 된다.
				// 이것 이외의 상태일 경우는 관련 WorkLoad 제거하자. 아니면 제거되도록 구성한다.

				// <THINKING2>
				//   기존에 종료되지 않는 작업이 있을 수 있다. 이를 어떻게 해결할 것인가?
				//if( WorkLoadManager::c_NotExistKey ==
				//	pServerSession->GetWorkLoadContainer().FindWorkKey( scriptCode ) )

				PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE* pWorkNode =
					new PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE();
				pWorkNode->SetInformation( pMasterServer, pMasterServer->GetSyncKey(), (DWORD)PHASE_EXCHANGE );
				WORK_KEY workKey = WorkLoadManager::RegisterTask( pWorkNode );
				if( WorkLoadManager::c_NotExistKey != workKey )
				{
					pMasterServer->GetWorkLoadContainer().RegisterWork( workKey, (DWORD)(__int64)pWorkNode );
					return;
				}
			}
		}
		break;
	case PHASE_SERVICE:
		{
			if( PHASE_UNKNOWN == prevPhase )
			{
				ASSERT( !"잘못된 제어 상태다 확인할 것" );
				return;
			}
			else if( PHASE_SERVICE == prevPhase )
			{
				ASSERT( !"잘못된 제어 상태다 확인할 것 - 뭔가 착각한 것 아닌가?" );
				return;
			}
			g_pGameServer->SetPhaseStatus( ePhase );
			g_pGameServer->ConnectTo( GAME_DBPROXY );
        #if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
			g_pGameServer->ConnectTo( GUILD_SERVER );
        #endif
        #if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
			g_pGameServer->ConnectTo( PORTAL_SERVER );
        #endif
        #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
            GameShop::Manager::Instance()->StartService();
        #else
			g_pGameServer->ConnectTo( SHOP_SERVER );
        #endif
		}
		break;
	};

	//if( pCMD->m_dwPHASE_SHIFT == PHASE_SERVICE /*0x00000001<<3*/ )
	//{
	//	MSG_AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN msgSYN;
	//	msgSYN.m_wDataType = 10;
	//	//pServerSession->SendPacket( &msgSYN, sizeof(msgSYN) );
	//	pServerSession->Send( (BYTE*)&msgSYN, sizeof(msgSYN) );

	//	for( DWORD i=11 ; i<20 ; ++i )
	//	{
	//		msgSYN.m_wDataType = i;
	//		pServerSession->Send( (BYTE*)&msgSYN, sizeof(msgSYN) );
	//	}
	//}
	//printf( "RECEIVED {AM_CONNECTION_PHASE_SHIFT_CMD} SEND {MSG_AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN}\n" );
}

HandlerFromMasterServer_IMPL( GM_CONNECTION_BLOCK_CMD )
{
}


//
//HandlerFromMasterServer_IMPL( AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN )

HandlerFromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK )
{

}

HandlerFromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK )
{
}

HandlerFromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD )
{
	//printf( "RECEIVED {_DATA_TRANSFER_SENDDATA_CMD} " );

	MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD* pMsgCMD = (MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD*)pMsg;
	switch( (DWORD)pMsgCMD->m_wControlCmd )
	{
	case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_HEADER:
		{
			DWORD dwDataType = (DWORD)pMsgCMD->m_FILE_INFO.m_wDataType;
			DWORD dwBufferSize = (DWORD)pMsgCMD->m_FILE_INFO.m_dwBufferSize;

			// <Work><Sequence>
			//   해당 처리는 현재 데이터 로딩을 요청한 이후에 대한 첫번째 응답에 대한 반응이다.
			// <1> 해당 스크립트 코드가 올바른 지 검사 -> F : NAK
			// <2> T : 해당 스크립트 코드에 버퍼를 할당한다.
			//     DataStreamCommand (SET blah~
			// <3> 기타 타임아웃등등의 보안정책을 강구할 것

			//printf( "\n\tHEADER {Data Type=%u, Buffer Size = %u\n", dwDataType, dwBufferSize );

			BYTE* pSTREAM;

			eSCRIPT_DATA_CMD cmdSTREAM =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE;

			ScriptDataManager::DataStreamCommand( cmdSTREAM, (eSCRIPT_CODE)dwDataType, pSTREAM, dwBufferSize );


			eSCRIPT_DATA_CMD cmdSTATUS =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2LOADING;

			ScriptDataManager::DataStatusCommand( cmdSTATUS, (eSCRIPT_CODE)dwDataType );
		}
		break;

	case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_DATA:
		{
			DWORD dwDataType = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wDataType;
			DWORD dwTokenNo = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wTokenNo;
			DWORD dwBufferSize = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wBufferSize;

			// <Work><Sequence>
			//   해당 처리는 현재 데이터 로딩을 요청에 대한 응답을 처리하며, 실재 데이터가 입력되어 온다.
			//   종결자가 올때까지 해당 토큰에 해당하는 위치에
			//   (해당 스크립에 할당된 버퍼를 얻어서 그 위치에) 데이터를 복사해 간다.
			//   DataStreamCommand (SET blah~

			ScriptDataManager::SaveDataStreamToken(
				(eSCRIPT_CODE)dwDataType,
				dwTokenNo,
				dwBufferSize,
				pMsgCMD->m_TOKEN_INFO.m_pSTREAM
				);
			//printf( "\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize );

		}
		break;

	case MSG_SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_CLOSURE:
		// 이것으로 종료하도록 수행하자.
		// 완료되면, 해당 스크립트 데이터 완료 콜백(내부 연산 목적)을 수행한다.
		//AsyncReloadData
		{
			DWORD dwDataType = (DWORD)pMsgCMD->m_FILE_CLOSURE.m_wDataType;
			DWORD dwCRC = (DWORD)pMsgCMD->m_FILE_CLOSURE.m_dwCRC;

			printf( "SCRIPT %u\t RECEIVED\n", dwDataType );

			eSCRIPT_DATA_CMD cmdSTATUS =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2LOADED;

			ScriptDataManager::DataStatusCommand( cmdSTATUS, (eSCRIPT_CODE)dwDataType/*, dwCRC*/ );
			ScriptDataManager::ProcessLoadedData( (eSCRIPT_CODE)dwDataType );
		}
		break;
	};
}

//HandlerFromMasterServer_IMPL( AM_DATA_TRANSFER_SENDDATA_ACK )
//HandlerFromMasterServer_IMPL( AM_DATA_TRANSFER_SENDDATA_NAK )

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//-------------------------------------------------------------------------------------------------
