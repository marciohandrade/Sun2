#include "stdafx.h"
#include "Handler_FromMasterServer.h"
#include "PacketHandler.Shared.h"

#include <GuildServer.h>

#include <PacketStruct_MZ.h>

// GUILD
#include <Guild/GuildManager.h>
#include <Guild/Guild.h>
#include <Guild/GuildMember.h>
//
//
//



#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&Handler_FromMasterServer::On##p }


sPACKET_HANDLER_INFO*	Handler_FromMasterServer::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
		ADD_HANDLER_INFO( MZ_CONNECTION, MZ_SERVER_INFO_CMD, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_REQ, PHASE_UNLIMITED ),
		//HANDLER_MASTER( MZ_CONNECTION, MZ_CONNECTION_REQ_SERVER_INFO_DBR, PHASE_UNLIMITED ),
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		ADD_HANDLER_INFO( MZ_CONNECTION, MZ_CONNECTION_DISCONNECT_CMD, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( MZ_CONNECTION, MZ_CONNECTION_PHASE_SHIFT_CMD, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( MZ_CONNECTION, MZ_CONNECTION_BLOCK_CMD, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD, PHASE_UNLIMITED ),
#endif //
        //__NA001254_20090109_RTTG_FUNC_ADD
		ADD_HANDLER_INFO( MZ_GUILD, MZ_GUILD_NAME_CHANGE_SYN, PHASE_UNLIMITED ),
        //
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_FromMasterServer::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<MASTER_SERVER>( GetHandlerInfo() );
	pInstance->RegisterHandler_<MASTER_SERVER>( PacketHandlerShared::GetHandlerInfo() );
}





Handler_FromMasterServer_IMPL( MZ_SERVER_INFO_CMD )
{
	// ���� ���� �޾Ҵ�~
	MSG_MZ_SERVER_INFO_CMD *pRecvMsg = (MSG_MZ_SERVER_INFO_CMD*)pMsg;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();

	pGuildServer->SetGuid( pRecvMsg->m_dwServerGUID );
	pGuildServer->SetKey( pRecvMsg->ServerKey );

	ASSERT( pRecvMsg->m_wDBProxyServerPort != 0 && " DBProxy ��Ʈ�� 0�Դϴ�.MZ_SERVER_INFO_CMD ����!" );
	ASSERT( pRecvMsg->m_wWorldServerPort != 0 && " World ��Ʈ�� 0�Դϴ�.MZ_SERVER_INFO_CMD ����!" );

	GuildServer::sConnectSession& rCONNECTION = pGuildServer->ConnectSessions;
	if( !rCONNECTION.SessionDBProxy() )
		pGuildServer->InitConnectSession( GAME_DBPROXY, pRecvMsg->m_szDBProxyServerIP, pRecvMsg->m_wDBProxyServerPort );
	if( !rCONNECTION.SessionWorld() )
		pGuildServer->InitConnectSession( WORLD_SERVER, pRecvMsg->m_szWorldServerIP, pRecvMsg->m_wWorldServerPort );

	DebugOut("�����ͷκ��� ���� ����Ű ����(%d) (%d, %d, %d, %d)", pRecvMsg->ServerKey,
		pRecvMsg->ServerKey.GetWorldID(), pRecvMsg->ServerKey.GetChannelID(), pRecvMsg->ServerKey.GetServerType(), pRecvMsg->ServerKey.GetServerID() );

	pGuildServer->UpdateAppTitle();

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	pGuildServer->ConnectTo( GAME_DBPROXY );
	pGuildServer->ConnectTo( WORLD_SERVER );
	pGuildServer->SetListenIP_s( pRecvMsg->m_szGuildServerInnerIP );
	pGuildServer->SetListenPort_s( pRecvMsg->m_wGuildServerServerInnerPort );
	pGuildServer->Listen( SERVER_IOHANDLER );
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	pGuildServer->SetListenIP_s( pRecvMsg->m_szGuildServerInnerIP );
	pGuildServer->SetListenPort_s( pRecvMsg->m_wGuildServerServerInnerPort );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

Handler_FromMasterServer_IMPL( SERVERCOMMON_SERVERSHUTDOWN_REQ )
{
	//�����ҰŴ�!!
	MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS sendMsg;
	sendMsg.m_byCategory = SERVERCOMMON;
	sendMsg.m_byProtocol = SERVERCOMMON_SERVERSHUTDOWN_ANS;

	// �̰� �� ���� �����ʹϱ� �ٷ� ����
	pServerSession->SendPacket( &sendMsg, sizeof(sendMsg) );

	DebugOut( "ServerShutdown Packet Recv.");

	GuildServer::GetMainFrame()->Shutdown();	// ����!

	DebugOut( "Server Shutdown Start..." );
}


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

Handler_FromMasterServer_IMPL( MZ_CONNECTION_DISCONNECT_CMD )
{
	printf( "RECEIVED [MZ_CONNECTION_DISCONNECT_CMD]\n" );
}

Handler_FromMasterServer_IMPL( MZ_CONNECTION_PHASE_SHIFT_CMD )
{
	MSG_MZ_CONNECTION_PHASE_SHIFT_CMD* pCMD = (MSG_MZ_CONNECTION_PHASE_SHIFT_CMD*)pMsg;
	ePHASE_SHIFT ePhase = (ePHASE_SHIFT)pCMD->m_dwPHASE_SHIFT;

	GuildServer* pGuildServer = GuildServer::GetMainFrame();

	ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);	// BLOCK CMD�� �� �ִ�.
	ePHASE_SHIFT prevPhase = pGuildServer->GetPhaseStatus();

	switch( ePhase )
	{
	case PHASE_UNKNOWN:
		// ���� �ܰ踦 �ٽ� ��ġ��� �ǹ�
		break;
	case PHASE_EXCHANGE:
		{

			if( PHASE_EXCHANGE == prevPhase )
				return;

			pGuildServer->SetPhaseStatus( ePhase );

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

				pWorkNode->SetInformation( pServerSession, pServerSession->GetSyncKey(), (DWORD)PHASE_EXCHANGE );
				WORK_KEY workKey = WorkLoadManager::RegisterTask( pWorkNode );
				if( WorkLoadManager::c_NotExistKey != workKey )
				{
					pServerSession->GetWorkLoadContainer().RegisterWork( workKey, (DWORD)(__int64)pWorkNode );
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

			pGuildServer->SetPhaseStatus( ePhase );

			pGuildServer->ConnectTo( GAME_DBPROXY );
			pGuildServer->ConnectTo( WORLD_SERVER );

			pGuildServer->Listen( SERVER_IOHANDLER );
		}
		break;
	};

	//printf( "RECEIVED {AM_CONNECTION_PHASE_SHIFT_CMD} SEND {MSG_AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN}\n" );
}

Handler_FromMasterServer_IMPL( MZ_CONNECTION_BLOCK_CMD )
{
}

Handler_FromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK )
{
}

Handler_FromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK )
{
}

Handler_FromMasterServer_IMPL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD )
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

//{__NA001254_20090109_RTTG_FUNC_ADD

Handler_FromMasterServer_IMPL( MZ_GUILD_NAME_CHANGE_SYN )
{
	//////////////////////////////////////////////////////////////////////////
	//	<LOGIC CONTROL>
	FlowControl flow;
	FLOWCONTROL_START()
    {
	    MSG_MZ_GUILD_NAME_CHANGE_SYN* pRecvMsg = (MSG_MZ_GUILD_NAME_CHANGE_SYN*)pMsg;
	    GuildServer* pGuildServer = GuildServer::GetMainFrame();

	    MSG_DZ_GUILD_NAME_CHANGE_SYN msg;
	    msg.m_dwKey			 = pRecvMsg->m_dwKey;
	    msg.m_TargetPos		 = INVALID_POSTYPE_VALUE;
	    msg.m_OperCharGuid	 = 0;//INVALID_CHARGUID;
	    msg.m_GuildGuid		 = pRecvMsg->m_GuildGuid;
	    msg.m_dwSessionIndex = pServerSession->GetSessionIndex();
	    memcpy( msg.m_tszGuildName, pRecvMsg->m_tszGuildName, MAX_GUILDNAME_LENGTH*sizeof(TCHAR) );
	    msg.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0';

	    IfFailThenBreak(pGuildServer->ConnectSessions.SendToDBProxy(&msg, sizeof(msg)),
                        RC::RC_GUILD_DISCONNECTED_DBP_SERVER);

	    SUNLOG(eDEV_LOG, "[MZ_GUILD_NAME_CHANGE_SYN] ���� ���� ��û ó����.[%d] [%s]", pRecvMsg->m_GuildGuid, pRecvMsg->m_tszGuildName );

	    return /*RETURN TRUE VALUE*/;
    }
    FLOWCONTROL_END;

	//////////////////////////////////////////////////////////////////////////
	//	<ERROR CONTROL>

	const DWORD errCode = flow.GetErrorCode();
	assert( SAFE_NUMERIC_TYPECAST( DWORD, errCode, BYTE ) );

	MSG_MZ_GUILD_NAME_CHANGE_SYN* pRecvMsg = (MSG_MZ_GUILD_NAME_CHANGE_SYN*)pMsg;
	MSG_MZ_GUILD_NAME_CHANGE_NAK  NakMsg;
	NakMsg.m_dwKey = pRecvMsg->m_dwKey;
	NakMsg.m_byErrorCode = (BYTE)errCode;

	pServerSession->SendPacket(&NakMsg, sizeof(NakMsg));
}
//}__NA001254_20090109_RTTG_FUNC_ADD