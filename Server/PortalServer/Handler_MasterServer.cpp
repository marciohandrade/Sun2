#include "StdAfx.h"
#include ".\handler_masterserver.h"
#include ".\PortalServer.h"
#include <PacketStruct_MP.h>
#include <PacketStruct_ServerCommon.h>

Handler_MasterServer_IMPL(MP_CONNECTION_SERVER_INFO_CMD)
{
	MSG_MP_CONNECTION_SERVER_INFO_CMD *pRecvMsg = (MSG_MP_CONNECTION_SERVER_INFO_CMD*)pMsg;

	g_PortalServer.SetGuid( pRecvMsg->m_dwServerGUID );
	g_PortalServer.SetKey( pRecvMsg->m_ServerKey );
	// 받은 서버키 셋팅
	DISPMSG("Received From Master..ServerKey : (%d) (%d, %d, %d, %d)\n", pRecvMsg->m_ServerKey,
		pRecvMsg->m_ServerKey.GetWorldID(), pRecvMsg->m_ServerKey.GetChannelID(), pRecvMsg->m_ServerKey.GetServerType(), pRecvMsg->m_ServerKey.GetServerID() );
	g_PortalServer.UpdateAppTitle();

	ASSERT( pRecvMsg->m_wDBProxyServerPort != 0 && " DBProxy 포트가 0입니다.오류!" );
	ASSERT( pRecvMsg->m_wPortalServerInnerPort != 0 && " Portal 포트가 0입니다.오류!" );

	g_PortalServer.SetListenIP_s( pRecvMsg->m_szPortalServerInnerIP );
	g_PortalServer.SetListenPort_s( pRecvMsg->m_wPortalServerInnerPort );

	if( !g_PortalServer.GetSession( GAME_DBPROXY ) )
		g_PortalServer.InitConnectSession( GAME_DBPROXY, pRecvMsg->m_szDBProxyServerIP, pRecvMsg->m_wDBProxyServerPort );

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// Game DB proxy와 연결 시도
	g_PortalServer.ConnectTo( GAME_DBPROXY );

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	if( g_PortalServer.IsServerInfoFromFile() )
	{
		// Game DB proxy와 연결 시도
		g_PortalServer.ConnectTo( GAME_DBPROXY );
	}

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

}

Handler_MasterServer_IMPL(SERVERCOMMON_SERVERSHUTDOWN_REQ)
{
	MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS msgANS;
	msgANS.m_byCategory = SERVERCOMMON;
	msgANS.m_byProtocol = SERVERCOMMON_SERVERSHUTDOWN_ANS;
	pServerSession->SendPacket( &msgANS, sizeof(msgANS) );

	g_PortalServer.Shutdown();
}



#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

Handler_MasterServer_IMPL( MP_CONNECTION_DISCONNECT_CMD )
{
	printf( "RECEIVED [MP_CONNECTION_DISCONNECT_CMD]\n" );
}

Handler_MasterServer_IMPL( MP_CONNECTION_PHASE_SHIFT_CMD )
{
	MSG_MP_CONNECTION_PHASE_SHIFT_CMD* pCMD = (MSG_MP_CONNECTION_PHASE_SHIFT_CMD*)pMsg;
	ePHASE_SHIFT ePhase = (ePHASE_SHIFT)pCMD->m_dwPHASE_SHIFT;

	ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);	// BLOCK CMD일 수 있다.
	ePHASE_SHIFT prevPhase = g_PortalServer.GetPhaseStatus();

	switch( ePhase )
	{
	case PHASE_UNKNOWN:
		// 인증 단계를 다시 거치라는 의미
		break;
	case PHASE_EXCHANGE:
		{

			if( PHASE_EXCHANGE == prevPhase )
				return;

			g_PortalServer.SetPhaseStatus( ePhase );

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

				ServerSessionEx* pServerEx = (ServerSessionEx*)pServerSession;
				pWorkNode->SetInformation( pServerEx, pServerEx->GetSyncKey(), (DWORD)PHASE_EXCHANGE );
				WORK_KEY workKey = WorkLoadManager::RegisterTask( pWorkNode );
				if( WorkLoadManager::c_NotExistKey != workKey )
				{
					pServerEx->GetWorkLoadContainer().RegisterWork( workKey, (DWORD)(__int64)pWorkNode );
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
			g_PortalServer.SetPhaseStatus( ePhase );

			g_PortalServer.ConnectTo( GAME_DBPROXY );
			g_PortalServer.Listen( SERVER_IOHANDLER );
		}
		break;
	};

	//printf( "RECEIVED {AM_CONNECTION_PHASE_SHIFT_CMD} SEND {MSG_AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN}\n" );
}

Handler_MasterServer_IMPL( MP_CONNECTION_BLOCK_CMD )
{
}

Handler_MasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK )
{
}

Handler_MasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK )
{
}

Handler_MasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD )
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

			//if( SCRIPT_CODE_KEY(FIELD) == dwDataType )
			//{
			//	//printf( "\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize );
			//	printf( (CHAR*)pMsgCMD->m_TOKEN_INFO.m_pSTREAM );
			//	printf( "\n\nDELIM %u\n\n", pMsgCMD->m_TOKEN_INFO.m_pSTREAM[dwBufferSize-1] );
			//}
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

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__