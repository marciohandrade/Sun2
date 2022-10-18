#include "stdafx.h"

#include <AgentServer.h>

#include <PacketStruct_ServerCommon.h>
#include <PacketStruct_AM.h>
#include <PacketStruct_CG.h>

#include "Handler_FromMasterServer.h"
#include "PacketHandler.Shared.h"

#include <ServerSessions/ServerSessionEx.h>
#include <ServerSessions/ServerSessionManager.h>

#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>

#undef ADD_HANDLER_INFO_EOR
#undef ADD_HANDLER_INFO
#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_AM::On##p }


sPACKET_HANDLER_INFO*	Handler_AM::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
        ADD_HANDLER_INFO( AM_CONNECTION, AM_CONNECTION_GROUND_CMD, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AM_OPERATION, AM_OPERATION_CHAR_DISCONNECT_SYN, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AM_OPERATION, AM_OPERATION_CHAR_DISCONNECT_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( AM_OPERATION, AM_OPERATION_LOGIC_VIOLATION_DISCONNECT_CMD, PHASE_UNLIMITED ),

		ADD_HANDLER_INFO( AM_CONNECTION, AM_CONNECTION_SERVER_INFO_CMD, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_SERVERSHUTDOWN_REQ, PHASE_UNLIMITED ),

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		//
		ADD_HANDLER_INFO( AM_CONNECTION, AM_CONNECTION_PHASE_SHIFT_CMD, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_UNLIMITED ),
		ADD_HANDLER_INFO( SERVERCOMMON, SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD, PHASE_UNLIMITED ),
#endif
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_AM::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<MASTER_SERVER>( GetHandlerInfo() );
	//pInstance->RegisterHandler_<MASTER_SERVER>( PacketHandlerShared::GetHandlerInfo() );
}

//
//
//

#undef FINDnALIGN_USER
#define FINDnALIGN_USER(pUserInstance, UserKey)							\
	User* pUserInstance = UserManager::Instance()->GetUser( (UserKey) );	\
	if( !pUserInstance )												\
	{																	\
		SUNLOG( eFULL_LOG, _T("[M2A] Can't Find User %u"), (UserKey) );	\
		return;															\
	}

HANDLER_AM_IMPL(AM_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

HANDLER_AM_IMPL(AM_CONNECTION_SERVER_INFO_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AM_CONNECTION_SERVER_INFO_CMD* pRecvMsg = (MSG_AM_CONNECTION_SERVER_INFO_CMD*)pMsg;

	AgentServer* pAgentServer = AgentServer::GetInstance();
	SERVER_KEY rKey(pRecvMsg->m_ServerKey);

	pAgentServer->SetKey( pRecvMsg->m_ServerKey );

	SUNLOG( eCRITICAL_LOG, _T("�����ͷκ��� ���� ����Ű ����(%u) (%u, %u, %u, %u)"),
		rKey, rKey.GetWorldID(), rKey.GetChannelID(), rKey.GetServerType(), rKey.GetServerID() );

	pAgentServer->UpdateAppTitle();

	//DWORD dwErrCode = 0;
	//if( !dwErrCode )
	//	dwErrCode = (pRecvMsg->m_szAgentServerInnerIP[0] == '0' || !pRecvMsg->m_wAgentServerInnerPort) ? 1 : 0;
	//if( !dwErrCode )
	//	dwErrCode = (pRecvMsg->m_szDBProxyServerIP[0] == '0' || !pRecvMsg->m_wDBProxyServerPort) ? 2 : 0;
	//if( !dwErrCode )
	//	dwErrCode = (pRecvMsg->m_szWorldServerIP[0] == '0' || !pRecvMsg->m_wWorldServerPort) ? 3 : 0;
	//if( !dwErrCode )
	//	dwErrCode = (pRecvMsg->m_szAuthAgentServerIP[0] == '0' || !pRecvMsg->m_wAuthAgentServerPort) ? 4 : 0;
	//if( !dwErrCode )
	//	dwErrCode = (pRecvMsg->m_szGuildServerIP[0] == '0' || !pRecvMsg->m_wGuildServerPort) ? 5 : 0;
	//if( !dwErrCode )
	//	dwErrCode = (pRecvMsg->m_szAgentServerIP[0] == '0' || !pRecvMsg->m_wAgentServerPort) ? 6 : 0;

	//////////////////////////////////////////////////////////////////////////
	if( !pAgentServer->SessionDBProxyServer() )
		pAgentServer->InitConnectSession( GAME_DBPROXY, pRecvMsg->m_szDBProxyServerIP, pRecvMsg->m_wDBProxyServerPort );
	if( !pAgentServer->SessionWorldServer() )
		pAgentServer->InitConnectSession( WORLD_SERVER, pRecvMsg->m_szWorldServerIP, pRecvMsg->m_wWorldServerPort );
	if( !pAgentServer->SessionAuthAgentServer() )
		pAgentServer->InitConnectSession( AUTH_AGENT_SERVER, pRecvMsg->m_szAuthAgentServerIP, pRecvMsg->m_wAuthAgentServerPort );

	pAgentServer->SetWorldServerClientAddr( pRecvMsg->m_szWorldServerClientIP, pRecvMsg->m_wWorldServerClientPort );

#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	pAgentServer->ConnectTo( GAME_DBPROXY );
	pAgentServer->ConnectTo( WORLD_SERVER );
	pAgentServer->ConnectTo( AUTH_AGENT_SERVER );
	pAgentServer->SetListenIP_s( pRecvMsg->m_szAgentServerInnerIP );
	pAgentServer->SetListenPort_s( pRecvMsg->m_wAgentServerInnerPort );
	pAgentServer->SetListenIP_c( pRecvMsg->m_szAgentServerIP );
	pAgentServer->SetListenPort_c( pRecvMsg->m_wAgentServerPort );
	pAgentServer->Listen( SERVER_IOHANDLER );

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	pAgentServer->SetListenIP_s( pRecvMsg->m_szAgentServerInnerIP );
	pAgentServer->SetListenPort_s( pRecvMsg->m_wAgentServerInnerPort );
	pAgentServer->SetListenIP_c( pRecvMsg->m_szAgentServerIP );
	pAgentServer->SetListenPort_c( pRecvMsg->m_wAgentServerPort );
#endif //
}

HANDLER_AM_IMPL(SERVERCOMMON_SERVERSHUTDOWN_REQ)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	// �������� ����!
	AgentServer* pAgentServer = AgentServer::GetInstance();
	pAgentServer->SERVER_EXIT.StartServerExitProcess();

	// �����Ѵٰ� �˷���~
	MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS	msgANS;

	msgANS.m_byCategory = SERVERCOMMON;
	msgANS.m_byProtocol = SERVERCOMMON_SERVERSHUTDOWN_ANS;

	pServerSession->Send( (BYTE*)&msgANS, sizeof(MSG_SERVERCOMMON_SERVERSHUTDOWN_ANS) );
}

//-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#include <AgentServer.h>
#include <PacketStruct_ServerCommon.h>
#include <PacketStruct_AM.h>
#include <PacketHandler/PacketHandler.h>
#include <ServerSessions/MasterServerSession.h>
#include "Handler_FromMasterServer.h"

// MASTER SERVER "SM_CONNECTION_PHASE_SHIFT_CMD" ����� �����ؾ� �Ѵ�.
// ����, SM_CONNECTION_PHASE_SHIFT_CMD�� ������ �� �ϰ������� �����ؾ� �Ѵ�.
HANDLER_AM_IMPL( AM_CONNECTION_PHASE_SHIFT_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AM_CONNECTION_PHASE_SHIFT_CMD* pCMD = (MSG_AM_CONNECTION_PHASE_SHIFT_CMD*)pMsg;
	ePHASE_SHIFT ePhase = (ePHASE_SHIFT)pCMD->m_dwPHASE_SHIFT;

	ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);	// BLOCK CMD�� �� �ִ�.
	ePHASE_SHIFT prevPhase = AgentServer::GetInstance()->GetPhaseStatus();

	switch( ePhase )
	{
	case PHASE_UNKNOWN:
		// ���� �ܰ踦 �ٽ� ��ġ��� �ǹ�
		break;
	case PHASE_EXCHANGE:
		{

			if( PHASE_EXCHANGE == prevPhase )
				return;

			AgentServer::GetInstance()->SetPhaseStatus( ePhase );

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
			AgentServer* pAgentServer = AgentServer::GetInstance();
			pAgentServer->SetPhaseStatus( ePhase );

			pAgentServer->ConnectTo( GAME_DBPROXY );
			pAgentServer->ConnectTo( WORLD_SERVER );
			pAgentServer->ConnectTo( AUTH_AGENT_SERVER );
		}
		break;
	};

	//printf( "RECEIVED {AM_CONNECTION_PHASE_SHIFT_CMD} SEND {MSG_AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN}\n" );
}

//
//HANDLER_AM_IMPL( AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN )

HANDLER_AM_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

HANDLER_AM_IMPL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

HANDLER_AM_IMPL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD )
{
    __TOUCHED((pServerSession, pMsg, wSize));
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

//HANDLER_AM_IMPL( AM_DATA_TRANSFER_SENDDATA_ACK )
//HANDLER_AM_IMPL( AM_DATA_TRANSFER_SENDDATA_NAK )

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//-------------------------------------------------------------------------------------------------

HANDLER_AM_IMPL(AM_OPERATION_CHAR_DISCONNECT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AM_OPERATION_CHAR_DISCONNECT_SYN* pRecvMsg = (MSG_AM_OPERATION_CHAR_DISCONNECT_SYN*)pMsg;

	//1. Agent �������� �ش� ĳ���͸��� ������ �ִ��� �˻��ϰ�,
	//   ������ �ִٸ�, �ش� ĳ���͸� �����Ų��.
	FINDnALIGN_USER( pUser, pRecvMsg->m_dwUserGuid );

	MSG_CG_CONNECTION_DISCONNECT_CMD msgCMD;
	msgCMD.m_dwErrorCode = RC::RC_CONNECTION_REQUESTFROMGAMESERVER;
	pUser->SendPacket( &msgCMD, sizeof(msgCMD) );

	// �ش� ������ �����Ų��.
	pUser->SetDisconnectCode( RC::RC_DISCONNECT_REQ_MASTER );
	pUser->DisconnectUser();

	//���� ���Ӽ������� �����Ű�� �ʰ�, Agent���� �����Ų�ٸ�..
	MSG_AM_OPERATION_CHAR_DISCONNECT_ACK msgACK;
	msgACK.m_dwUserGuid = pRecvMsg->m_dwUserGuid;

	if( pServerSession )
		pServerSession->Send( (BYTE*)&msgACK, sizeof(msgACK) );
}

HANDLER_AM_IMPL(AM_OPERATION_CHAR_DISCONNECT_ACK)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AM_OPERATION_CHAR_DISCONNECT_ACK* pRecvMsg = (MSG_AM_OPERATION_CHAR_DISCONNECT_ACK*)pMsg;

	//1. ���Ӽ������� �ش� ������ ���� �������Ƿ�, �� ����� MASTER���� �뺸�Ѵ�.
}

HANDLER_AM_IMPL(AM_OPERATION_LOGIC_VIOLATION_DISCONNECT_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
	MSG_AM_OPERATION_LOGIC_VIOLATION_DISCONNECT_CMD* pMsgCMD = (MSG_AM_OPERATION_LOGIC_VIOLATION_DISCONNECT_CMD*)pMsg;

	User* pUser = UserManager::Instance()->GetUser( pMsgCMD->UserGUID() );
	if( pUser )
	{
		SUNLOG( eCRITICAL_LOG, "User(%d) Abusing Forced Disconnect!", pMsgCMD->UserGUID() );

		// �ش� ������ Disconnect
		SecurePackNode4User& rSECURE = pUser->GetSecureNode();
		rSECURE.ForcedDisconnectProcess( pUser );
	}
}









