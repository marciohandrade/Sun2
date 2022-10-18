#include "stdafx.h"
#include "Handler_SM.h"
#include "PacketHandler.Shared.h"
#include <MasterScriptParserManager.h>


#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_SM::On##p }


sPACKET_HANDLER_INFO*	Handler_SM::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		ADD_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_HEARTBEAT, PHASE_UNLIMITED),
		//
		//ADD_HANDLER_INFO(SM_CONNECTION_CONNECTION_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_CONNECTION_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_CONNECTION_NAK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_DISCONNECT_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_PHASE_SHIFT_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_BLOCK_CMD, PHASE_UNLIMITED),
		//
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_SYN, PHASE_EXCHANGE|PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_ACK, PHASE_EXCHANGE|PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_DESTINATION_KEY_NAK, PHASE_EXCHANGE|PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_QUERY_SYN, PHASE_BLOCKED),
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_QUERY_ACK, PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_QUERY_CMD, PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_QUERY_NAK, PHASE_SERVICE),
		//ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_SYN, PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_ACK, PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_NAK, PHASE_SERVICE),
		//
		ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_KEY, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_ACK, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_NAK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_CMD, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_PACKET_ENCODED, PHASE_BLOCKED),
		ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_PACKET_ENCODING, PHASE_BLOCKED),
		ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_KEY, PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_CMD, PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_ACK, PHASE_SERVICE),
		ADD_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_SNK_NAK, PHASE_SERVICE),
		//
		//ADD_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_CMD, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_ACK, PHASE_UNLIMITED),
		//ADD_HANDLER_INFO(SM_DATA_TRANSFER, SM_DATA_TRANSFER_SENDDATA_NAK, PHASE_UNLIMITED),
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	Handler_SM::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	pInstance->RegisterHandler_<SOLARAUTH_SERVER>( GetHandlerInfo() );
	//pInstance->RegisterHandler_<SOLARAUTH_SERVER>( PacketHandlerShared::GetHandlerInfo() );
}

//
//
//

#include <MasterServerEx.h>
#include <Sessions/ServerSessionManager.h>
#include <Function/ServerInfoManager.h>

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
// WorkLoad Handlers
#include "WorkLoad/Handler_SM.WorkLoad.cxx"


HANDLER_SM_IMPL(SM_CONNECTION_HEARTBEAT)
{
}

//
//HANDLER_SM_IMPL(SM_CONNECTION_CONNECTION_SYN)

HANDLER_SM_IMPL(SM_CONNECTION_CONNECTION_ACK)
{
	DWORD& dwAccessCount = pServerSession->AccessCount();

	// 0 : ù��° ��Ŷ
	//   �������� ���� MasterServer�� �ڽ��� ������ SolarAuthKey (SA_ProK1)���� MSG1�� ��ȣȭ�ؼ� MSG' ����
	//   SolarAuthServer�� SA_PriK1���� MSG'�� ��ȣȭ�ؼ� MasterServer�� ���� MSG1�� Ȯ���Ѵ�.
	//   T -> SM_CONNECTION_CONNECTION_ACK (MSG2�� M_ProK2�� ��ȣȭ MSG2' ����)
	// 1 : �ι�° ��Ŷ
	//   �������� ���� MasterServer�� �ڽ��� ������ SolarAuthKey (SA_ProK3)���� MSG3�� ��ȣȭ�ؼ� MSG3' ����
	//   SolarAuthServer�� SA_PriK3���� MSG3'�� ��ȣȭ�ؼ� MasterServer�� ���� MSG3�� Ȯ���Ѵ�.
	//   T -> SM_CONNECTION_CONNECTION_ACK (MSG4�� M_ProK4�� ��ȣȭ MSG4' ����)

	switch( dwAccessCount )
	{
	case 0:
		{
			MSG_SM_CONNECTION_CONNECTION_SYN msgSYN;
			pServerSession->SendByEncode( (BYTE*)&msgSYN, sizeof(MSG_SM_CONNECTION_CONNECTION_SYN) );
			pServerSession->SetSeqCryptKey( pMsg->m_dwKey );

			++dwAccessCount;
		}
		return;
	case 1:
		{
			++dwAccessCount;
			MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
				_T("Solar Auth Server���� ���ῡ �����߽��ϴ�.") );
		}
		return;
	}

	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
		_T("Solar Auth Server���� ���ῡ ���������� �ʱ� ���� �������� �߻��߽��ϴ�.") );
}

HANDLER_SM_IMPL(SM_CONNECTION_CONNECTION_NAK)
{
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_CONNECTION_CONNECTION_NAK}\n" ); );
}

HANDLER_SM_IMPL(SM_CONNECTION_DISCONNECT_CMD)
{
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_CONNECTION_DISCONNECT_CMD}\n" ); );
}

HANDLER_SM_IMPL(SM_CONNECTION_PHASE_SHIFT_CMD)
{
	MSG_SM_CONNECTION_PHASE_SHIFT_CMD* pCMD = (MSG_SM_CONNECTION_PHASE_SHIFT_CMD*)pMsg;
	ePHASE_SHIFT ePhase = (ePHASE_SHIFT)pCMD->m_dwPHASE_SHIFT;

	ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);	// BLOCK CMD�� �� �ִ�.

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	switch( ePhase )
	{
	case PHASE_UNKNOWN:
		{
			// ���� �ܰ踦 �ٽ� ��ġ��� �ǹ�
			// �ϴ���...
			pMasterServer->SetPhaseStatus( ePhase );
		}
		break;
	case PHASE_EXCHANGE:
		{
			// ���� �ܰ踦 ��ģ ���� �ٽ� ��ȯ �ܰ�� ��������.
			ePHASE_SHIFT currPhase = pMasterServer->GetPhaseStatus();
			if( PHASE_UNKNOWN == currPhase )
			{
				// ���ӵ� ���� �ʱ� ������ �����ٴ� �ǹ̴�.
			}
			else if( PHASE_SERVICE == currPhase )
			{
				// �� ������ �䱸�Ǵ� ��Ȳ�ΰ�?
			}
			// �ϴ���...
			pMasterServer->SetPhaseStatus( ePhase );
		}
		break;
	case PHASE_SERVICE:
		{
			// ���� �ܰ踦 ��ģ ���� �ٽ� ��ȯ �ܰ�� ��������.
			ePHASE_SHIFT currPhase = pMasterServer->GetPhaseStatus();
			if( PHASE_SERVICE == currPhase )
				return;

			if( PHASE_UNKNOWN == currPhase )
			{
				ASSERT(!"�̸� �Ǽ��� �ƴϵǿ�!");
			}
			else if( PHASE_EXCHANGE == currPhase )
			{
				// ������ ������ ���� ������ �Ǿ���.
			}
			// �ϴ���...
			pMasterServer->SetPhaseStatus( ePhase );

			// ���񽺷��� ��ȯ�̹Ƿ� ���� ���۵��, ���������� SolarAuth���� ���񽺸� ���� �� �ִ�.
			// �Ǵ� �־��� CMD�� ���� ó���� ������ �� �ִ�.
			// �̰����� WorkLoad�����ϸ� �ȴ�. �̰� �̿��� ������ ���� ���� WorkLoad ��������. �ƴϸ� ���ŵǵ��� �����Ѵ�.

			// <THINKING2>
			//   ������ ������� �ʴ� �۾��� ���� �� �ִ�. �̸� ��� �ذ��� ���ΰ�?
			//if( WorkLoadManager::c_NotExistKey ==
			//	pServerSession->GetWorkLoadContainer().FindWorkKey( scriptCode ) )

			PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE* pWorkNode =
				new PROCESS_SM_CONNECTION_PHASE_SHIFT_CMD___ON_START_SERVICE();
			pWorkNode->SetServerSession( pServerSession );
			WORK_KEY workKey = WorkLoadManager::RegisterTask( pWorkNode );
			if( WorkLoadManager::c_NotExistKey != workKey )
			{
				Services4Session& rSERVICE = pServerSession->Services();
				rSERVICE.GetWorkLoadContainer().RegisterWork( workKey, (DWORD)(__int64)pWorkNode );
				return;
			}

			MessageOut( eCRITICAL_LOG, _T("�������� ���μ������� ������ ���� ��û�� �ϴµ� �����߽��ϴ�. ���μ����� ����� ���ּ���") );
			delete pWorkNode;
		}
		break;
	};

	//if( pCMD->m_dwPHASE_SHIFT == PHASE_SERVICE /*0x00000001<<3*/ )
	//{
	//	MSG_SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN msgSYN;
	//	msgSYN.m_wDataType = 10;
	//	//pServerSession->SendPacket( &msgSYN, sizeof(msgSYN) );
	//	pServerSession->Send( (BYTE*)&msgSYN, sizeof(msgSYN) );

	//	for( DWORD i=11 ; i<20 ; ++i )
	//	{
	//		msgSYN.m_wDataType = i;
	//		pServerSession->Send( (BYTE*)&msgSYN, sizeof(msgSYN) );
	//	}
	//}

	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_CONNECTION_PHASE_SHIFT_CMD} SEND {MSG_SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN}\n" ); );
}

HANDLER_SM_IMPL(SM_CONNECTION_BLOCK_CMD)
{
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_CONNECTION_BLOCK_CMD}\n" ); );
}

//
HANDLER_SM_IMPL(SM_SERVER_INFO_DESTINATION_KEY_SYN)
{
}

HANDLER_SM_IMPL(SM_SERVER_INFO_DESTINATION_KEY_ACK)
{
}

HANDLER_SM_IMPL(SM_SERVER_INFO_DESTINATION_KEY_NAK)
{
}

HANDLER_SM_IMPL(SM_SERVER_INFO_QUERY_SYN)
{
}

HANDLER_SM_IMPL(SM_SERVER_INFO_QUERY_ACK)
{
#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	MSG_SM_SERVER_INFO_QUERY_ACK* pMsgACK = (MSG_SM_SERVER_INFO_QUERY_ACK*)pMsg;

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK* pINFOs = (MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK*)(PACKET_CONTROL_SM::GetBasePacketPtr4Send());
#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK pINFOs[1];
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	DWORD dwCount = (DWORD)pMsgACK->m_wCounts;
	sSERVER_INFO_GENERIC* pSRCLIST = pMsgACK->m_ServerInfos;
	SERVER_INFO* pLIST = pINFOs->m_ServerInfo;

	if( MSG_MX_CONNECTION_REQ_SERVER_INFO_ACK::_MAX_SERVERINFO_ARRAY_NUM < dwCount )
	{
		MessageOut( eCRITICAL_LOG, _T("Invalid ServerList Count. �ƴϸ� ��¥�� 100�� �Ѵ� ������?") );
		return;
	}

	for( DWORD dwIdx=0 ; dwIdx<dwCount ; ++dwIdx, ++pSRCLIST, ++pLIST )
	{
		pLIST->ServerKey = pSRCLIST->m_ServerKey;
		pLIST->dwServerGUID = 0; // not used;

		CHAR* pszIP;
		pszIP = IPOperation::ConvertDWORD2StrIP(pSRCLIST->m_dwPublicIP);
		memcpy( pLIST->szIP, pszIP, MAX_IP_STRING_LEN );
		pLIST->szIP[MAX_IP_STRING_LEN] = '\0';

		pszIP = IPOperation::ConvertDWORD2StrIP(pSRCLIST->m_dwInnerIP);
		memcpy( pLIST->szInnerIP, pszIP, MAX_IP_STRING_LEN+1 );
		pLIST->szInnerIP[MAX_IP_STRING_LEN] = '\0';

		pLIST->wPort = pSRCLIST->m_wPublicPort;
		pLIST->wInnerPort = pSRCLIST->m_wInnerPort;
	}

	MessageOut( eCRITICAL_LOG, _T("Number of Server Info: %u"), dwCount );

	ServerInfoManager::AddServerInfo( pINFOs->m_ServerInfo, dwCount, TRUE );

#else //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_SERVER_INFO_QUERY_ACK}\n" ); );
}

HANDLER_SM_IMPL(SM_SERVER_INFO_QUERY_CMD)
{
#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	pMasterServer->StartListen();
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_SERVER_INFO_QUERY_CMD}\n" ); );
}

HANDLER_SM_IMPL(SM_SERVER_INFO_QUERY_NAK)
{
}

//HANDLER_SM_IMPL(SM_SERVER_INFO_AUTHENTICATION_SYN)

HANDLER_SM_IMPL(SM_SERVER_INFO_AUTHENTICATION_ACK)
{
	MSG_SM_SERVER_INFO_AUTHENTICATION_ACK* pMsgACK = (MSG_SM_SERVER_INFO_AUTHENTICATION_ACK*)pMsg;

#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	sSERVER_INFO_GENERIC& rSERVER_INFO = pMsgACK->m_ServerInfo;
	SERVER_KEY skKey = rSERVER_INFO.m_ServerKey;

	LPSERVER_INFO pFindedInfo = ServerInfoManager::GetServerInfo( skKey );
	if( !pFindedInfo )
	{
		// �ش� ������ ���� ��ϵ��� ���� ������. �̷� ���� �Ͼ���� �ȵǰ���?
		MessageOut( eCRITICAL_LOG, _T("���� ��û���� ���� ������ ���� ���� �����. impossible auth") );
		return;
	}

	//ServerSessionEx* pFindedSession = ServerSessionManager::FindServer( skKey );
	//if( !pFindedSession || !pFindedSession->IsConnected() )
	//{
	//	// �ش� ������ ������ ��û�س��� ������ ���´�. �� ���� �׳� ��������.
	//	return;
	//}

	// ������� �� ���� �ش� ���� ������ ������ ���¶�� ���̴�.
	// ���� Redirect�� ������� ��������
	TempServerSessionEx::Redirect2NewSession( pMsgACK->m_dwInnerIP, skKey );

#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

HANDLER_SM_IMPL(SM_SERVER_INFO_AUTHENTICATION_NAK)
{
	// ��а� ������ ���� �ִٰ� �����ϴ� ���� ���? �ϴ��� �ٷ� ���������? �� ������ �ٵ�?
}

//
HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_KEY)
{
	SolarAuthServerSession::Crypt::Init_ClientRoleManager(
												pServerSession, 
												(MSG_SM_CRYPTOGRAPHY_KEY*)pMsg
												);
	MSG_SM_CRYPTOGRAPHY_ACK msgACK;
	pServerSession->SendByEncode( (BYTE*)&msgACK, sizeof(msgACK) );

	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_CRYPTOGRAPHY_KEY} SEND {MSG_SM_CRYPTOGRAPHY_ACK}\n" ); );
}

//HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_ACK)

//HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_NAK)

HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_CMD)
{
	// ��ȣȭ�� ��Ŷ ��� ����! ���
	__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_CRYPTOGRAPHY_CMD}\n" ); );

	SolarAuthServerSession::Crypt::StartPacketCrypt( pServerSession );
	SolarAuthServerSessionCrypted::ChangeObjectCrypted( pServerSession );

#ifdef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	MSG_SM_SERVER_INFO_QUERY_SYN msgSYN;
	pServerSession->SendPacket( &msgSYN, sizeof(msgSYN) );
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_PACKET_ENCODED)
{
}

HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_PACKET_ENCODING)
{
}

HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_SNK_KEY)
{
}

HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_SNK_CMD)
{
}

HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_SNK_ACK)
{
}

HANDLER_SM_IMPL(SM_CRYPTOGRAPHY_SNK_NAK)
{
}

//
//HANDLER_SM_IMPL(SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN)

HANDLER_SM_IMPL(SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK)
{
	//__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK}\n" ); );
}

HANDLER_SM_IMPL(SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK)
{
}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__DEBUG__
static BOOL bStart = FALSE;
static DWORD dwStartTick = 0;
static DWORD dwEndTick = 0;
static DWORD dwReceivedSize = 0;
static DWORD dwEndCount = 64;
static DWORD dwEndCountInc = 0;
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__DEBUG__

HANDLER_SM_IMPL(SM_DATA_TRANSFER_SENDDATA_CMD)
{
	//__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "RECEIVED {SM_DATA_TRANSFER_SENDDATA_CMD}\n" ); );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__DEBUG__
	if( !bStart )
	{
		dwStartTick = GetTickCount();
		bStart = TRUE;
	}
	dwReceivedSize += wSize;
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__DEBUG__

	MSG_SM_DATA_TRANSFER_SENDDATA_CMD* pMsgCMD = (MSG_SM_DATA_TRANSFER_SENDDATA_CMD*)pMsg;
	switch( (DWORD)pMsgCMD->m_wControlCmd )
	{
	case MSG_SM_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_HEADER:
		{
			const eSCRIPT_CODE scriptType = (eSCRIPT_CODE)pMsgCMD->m_FILE_INFO.m_wDataType;
			DWORD buffer_size = (DWORD)pMsgCMD->m_FILE_INFO.m_dwBufferSize;

			//if( SCRIPT_CODE_KEY(NOTIFY_SIGNAL) == dwDataType )
			//{
			//	printf( "check!\n" );
			//}

			// <Work><Sequence>
			//   �ش� ó���� ���� ������ �ε��� ��û�� ���Ŀ� ���� ù��° ���信 ���� �����̴�.
			// <1> �ش� ��ũ��Ʈ �ڵ尡 �ùٸ� �� �˻� -> F : NAK
			// <2> T : �ش� ��ũ��Ʈ �ڵ忡 ���۸� �Ҵ��Ѵ�.
			//     DataStreamCommand (SET blah~
			// <3> ��Ÿ Ÿ�Ӿƿ������ ������å�� ������ ��

			//printf( "\n\tHEADER {Data Type=%u, Buffer Size = %u\n", dwDataType, dwBufferSize );

			BYTE* pSTREAM;

			const eSCRIPT_DATA_CMD cmdSTREAM =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::BUFFERINFO::BUFFER_SIZE;

			ScriptDataManager::DataStreamCommand( cmdSTREAM, scriptType, pSTREAM, buffer_size );
			const eSCRIPT_DATA_CMD cmdSTATUS =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2LOADING;

			ScriptDataManager::DataStatusCommand( cmdSTATUS, scriptType );

			const DWORD plainSz = pMsgCMD->m_FILE_INFO.m_PlainSize;
			MasterScriptParserManager* pMasterParser = static_cast<MasterScriptParserManager*>(MasterServerEx::GetMainFrame()->GetParserManager());
			pMasterParser->SetCompressInfo( scriptType, buffer_size, plainSz );
		}
		break;
	case MSG_SM_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_DATA:
		{
			const eSCRIPT_CODE scriptType = (eSCRIPT_CODE)pMsgCMD->m_TOKEN_INFO.m_Header.m_wDataType;
			const DWORD tokenNo = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wTokenNo;
			const DWORD buffer_size = (DWORD)pMsgCMD->m_TOKEN_INFO.m_Header.m_wBufferSize;

			//if( SCRIPT_CODE_KEY(NOTIFY_SIGNAL) == dwDataType )
			//{
			//	printf( "check!\n" );
			//}

			// <Work><Sequence>
			//   �ش� ó���� ���� ������ �ε��� ��û�� ���� ������ ó���ϸ�, ���� �����Ͱ� �ԷµǾ� �´�.
			//   �����ڰ� �ö����� �ش� ��ū�� �ش��ϴ� ��ġ��
			//   (�ش� ��ũ���� �Ҵ�� ���۸� �� �� ��ġ��) �����͸� ������ ����.
			//   DataStreamCommand (SET blah~

			ScriptDataManager::SaveDataStreamToken( scriptType, tokenNo, buffer_size, pMsgCMD->m_TOKEN_INFO.m_pSTREAM );

			//if( 27 == dwDataType )
			//{
			//	//printf( "\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize );
			//	printf( (CHAR*)pMsgCMD->m_TOKEN_INFO.m_pSTREAM );
			//	printf( "\n\nDELIM %u\n\n", pMsgCMD->m_TOKEN_INFO.m_pSTREAM[dwBufferSize-1] );
			//}

			//printf( "\n\tDATA {Data Type=%u, Token no = %u, Buffer Size = %u\n", dwDataType, dwTokenNo, dwBufferSize );

		}
		break;
	case MSG_SM_DATA_TRANSFER_SENDDATA_CMD::TRANS_CMD_SEND_CLOSURE:
		// �̰����� �����ϵ��� ��������.
		// �Ϸ�Ǹ�, �ش� ��ũ��Ʈ ������ �Ϸ� �ݹ�(���� ���� ����)�� �����Ѵ�.
		//AsyncReloadData
		{
			const eSCRIPT_CODE scriptType = (eSCRIPT_CODE)pMsgCMD->m_FILE_CLOSURE.m_wDataType;
			const DWORD dwCRC = (DWORD)pMsgCMD->m_FILE_CLOSURE.m_dwCRC;

			//if( SCRIPT_CODE_KEY(NOTIFY_SIGNAL) == dwDataType )
			//{
			//	printf( "check!\n" );
			//}
			MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_DATARELOAD,
				_T("SCRIPT %03u\t RECEIVED\n"), scriptType );

			const eSCRIPT_DATA_CMD cmdSTATUS =
				SCRIPT_DATA_CMD::SET | SCRIPT_DATA_CMD::RANGE::ONENODE | SCRIPT_DATA_CMD::STATUS::MOVE2LOADED;

			ScriptDataManager::DataStatusCommand( cmdSTATUS, scriptType/*, dwCRC*/ );
			ScriptDataManager::ProcessLoadedData( scriptType );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__DEBUG__
			++dwEndCountInc;
			if( dwEndCountInc == dwEndCount )
			{
				dwEndTick = GetTickCount();
				DWORD dwInterval = dwEndTick - dwStartTick;
				printf( "End Time : Transferred %u BYTE, �ҿ�ð� %ums Transfer Rate %u (BYTE/SEC\n",
					dwReceivedSize, dwInterval, (DWORD)(dwReceivedSize/(dwInterval/1000)) );
			}
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__DEBUG__
		}
		break;
	};
}

//HANDLER_SM_IMPL(SM_DATA_TRANSFER_SENDDATA_ACK)

//HANDLER_SM_IMPL(SM_DATA_TRANSFER_SENDDATA_NAK)

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__


