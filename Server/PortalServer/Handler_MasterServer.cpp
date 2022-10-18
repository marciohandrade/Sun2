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
	// ���� ����Ű ����
	DISPMSG("Received From Master..ServerKey : (%d) (%d, %d, %d, %d)\n", pRecvMsg->m_ServerKey,
		pRecvMsg->m_ServerKey.GetWorldID(), pRecvMsg->m_ServerKey.GetChannelID(), pRecvMsg->m_ServerKey.GetServerType(), pRecvMsg->m_ServerKey.GetServerID() );
	g_PortalServer.UpdateAppTitle();

	ASSERT( pRecvMsg->m_wDBProxyServerPort != 0 && " DBProxy ��Ʈ�� 0�Դϴ�.����!" );
	ASSERT( pRecvMsg->m_wPortalServerInnerPort != 0 && " Portal ��Ʈ�� 0�Դϴ�.����!" );

	g_PortalServer.SetListenIP_s( pRecvMsg->m_szPortalServerInnerIP );
	g_PortalServer.SetListenPort_s( pRecvMsg->m_wPortalServerInnerPort );

	if( !g_PortalServer.GetSession( GAME_DBPROXY ) )
		g_PortalServer.InitConnectSession( GAME_DBPROXY, pRecvMsg->m_szDBProxyServerIP, pRecvMsg->m_wDBProxyServerPort );

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// Game DB proxy�� ���� �õ�
	g_PortalServer.ConnectTo( GAME_DBPROXY );

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	if( g_PortalServer.IsServerInfoFromFile() )
	{
		// Game DB proxy�� ���� �õ�
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

	ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);	// BLOCK CMD�� �� �ִ�.
	ePHASE_SHIFT prevPhase = g_PortalServer.GetPhaseStatus();

	switch( ePhase )
	{
	case PHASE_UNKNOWN:
		// ���� �ܰ踦 �ٽ� ��ġ��� �ǹ�
		break;
	case PHASE_EXCHANGE:
		{

			if( PHASE_EXCHANGE == prevPhase )
				return;

			g_PortalServer.SetPhaseStatus( ePhase );

			if( PHASE_SERVICE == prevPhase )
			{
				// �ٽ� EXCHANGE���·� ��ȯ�Ǿ��ٴ� ���� ���� ���� ���� ���� ������ ������...
				// PHASE_SERVICE -> PHASE_EXCHANGE
				// �̰��� PROCESS_CONNECTION_PHASE_SHIFT_CMD___ON_START_EXCHANGE�� �����Ѵٴ� �ǹ�

				// �����͸� �ٽ� �޾ƾ� �ϴ��� ����� NOTIFY_SIGNAL�� �ذ��� ����.
				eSCRIPT_DATA_CMD cmd =
					SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2EMPTYDATA;

				ScriptDataManager::DataStatusCommand( cmd, SCRIPT_NOTIFY_SIGNAL );

			}
			else
			{
				//
				// PHASE_UNKNOWN -> PHASE_EXCHANGE
				//
				// ���񽺷��� ��ȯ�̹Ƿ� ���� ���۵��, ���������� SolarAuth���� ���񽺸�
				// ���� �� �ִ�. �Ǵ� �־��� CMD�� ���� ó���� ������ �� �ִ�.
				// �̰����� WorkLoad�����ϸ� �ȴ�.
				// �̰� �̿��� ������ ���� ���� WorkLoad ��������. �ƴϸ� ���ŵǵ��� �����Ѵ�.

				// <THINKING2>
				//   ������ ������� �ʴ� �۾��� ���� �� �ִ�. �̸� ��� �ذ��� ���ΰ�?
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
				ASSERT( !"�߸��� ���� ���´� Ȯ���� ��" );
				return;
			}
			else if( PHASE_SERVICE == prevPhase )
			{
				ASSERT( !"�߸��� ���� ���´� Ȯ���� �� - ���� ������ �� �ƴѰ�?" );
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
			//   �ش� ó���� ���� ������ �ε��� ��û�� ���Ŀ� ���� ù��° ���信 ���� �����̴�.
			// <1> �ش� ��ũ��Ʈ �ڵ尡 �ùٸ� �� �˻� -> F : NAK
			// <2> T : �ش� ��ũ��Ʈ �ڵ忡 ���۸� �Ҵ��Ѵ�.
			//     DataStreamCommand (SET blah~
			// <3> ��Ÿ Ÿ�Ӿƿ������ ������å�� ������ ��

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
			//   �ش� ó���� ���� ������ �ε��� ��û�� ���� ������ ó���ϸ�, ���� �����Ͱ� �ԷµǾ� �´�.
			//   �����ڰ� �ö����� �ش� ��ū�� �ش��ϴ� ��ġ��
			//   (�ش� ��ũ���� �Ҵ�� ���۸� �� �� ��ġ��) �����͸� ������ ����.
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
		// �̰����� �����ϵ��� ��������.
		// �Ϸ�Ǹ�, �ش� ��ũ��Ʈ ������ �Ϸ� �ݹ�(���� ���� ����)�� �����Ѵ�.
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