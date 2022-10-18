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

	// 0 : 첫번째 패킷
	//   인증되지 않은 MasterServer는 자신이 가지는 SolarAuthKey (SA_ProK1)으로 MSG1를 암호화해서 MSG' 전송
	//   SolarAuthServer는 SA_PriK1으로 MSG'을 복호화해서 MasterServer이 보낸 MSG1를 확인한다.
	//   T -> SM_CONNECTION_CONNECTION_ACK (MSG2를 M_ProK2로 암호화 MSG2' 전송)
	// 1 : 두번째 패킷
	//   인증되지 않은 MasterServer는 자신이 가지는 SolarAuthKey (SA_ProK3)으로 MSG3를 암호화해서 MSG3' 전송
	//   SolarAuthServer는 SA_PriK3으로 MSG3'을 복호화해서 MasterServer이 보낸 MSG3를 확인한다.
	//   T -> SM_CONNECTION_CONNECTION_ACK (MSG4를 M_ProK4로 암호화 MSG4' 전송)

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
				_T("Solar Auth Server와의 연결에 성공했습니다.") );
		}
		return;
	}

	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
		_T("Solar Auth Server와의 연결에 비정상적인 초기 접속 시퀀스가 발생했습니다.") );
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

	ePhase = (ePHASE_SHIFT)(ePhase&PHASE_UNLIMITED);	// BLOCK CMD일 수 있다.

	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	switch( ePhase )
	{
	case PHASE_UNKNOWN:
		{
			// 인증 단계를 다시 거치라는 의미
			// 일단은...
			pMasterServer->SetPhaseStatus( ePhase );
		}
		break;
	case PHASE_EXCHANGE:
		{
			// 인증 단계를 거친 이후 다시 교환 단계로 접어들었다.
			ePHASE_SHIFT currPhase = pMasterServer->GetPhaseStatus();
			if( PHASE_UNKNOWN == currPhase )
			{
				// 접속된 이후 초기 인증이 끝났다는 의미다.
			}
			else if( PHASE_SERVICE == currPhase )
			{
				// 재 인증이 요구되는 상황인가?
			}
			// 일단은...
			pMasterServer->SetPhaseStatus( ePhase );
		}
		break;
	case PHASE_SERVICE:
		{
			// 인증 단계를 거친 이후 다시 교환 단계로 접어들었다.
			ePHASE_SHIFT currPhase = pMasterServer->GetPhaseStatus();
			if( PHASE_SERVICE == currPhase )
				return;

			if( PHASE_UNKNOWN == currPhase )
			{
				ASSERT(!"이리 되서는 아니되오!");
			}
			else if( PHASE_EXCHANGE == currPhase )
			{
				// 데이터 전송을 받을 시점이 되었다.
			}
			// 일단은...
			pMasterServer->SetPhaseStatus( ePhase );

			// 서비스로의 전환이므로 파일 전송등등, 여러가지의 SolarAuth관련 서비스를 받을 수 있다.
			// 또는 주어진 CMD에 대한 처리를 수행할 수 있다.
			// 이곳에서 WorkLoad설정하면 된다. 이것 이외의 상태일 경우는 관련 WorkLoad 제거하자. 아니면 제거되도록 구성한다.

			// <THINKING2>
			//   기존에 종료되지 않는 작업이 있을 수 있다. 이를 어떻게 해결할 것인가?
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

			MessageOut( eCRITICAL_LOG, _T("정상적인 프로세스지만 데이터 전송 요청을 하는데 실패했습니다. 프로세스를 재시작 해주세요") );
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
		MessageOut( eCRITICAL_LOG, _T("Invalid ServerList Count. 아니면 진짜로 100개 넘는 서버수?") );
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
		// 해당 정보는 애초 등록되지 않은 서버다. 이런 일이 일어나서는 안되겠죠?
		MessageOut( eCRITICAL_LOG, _T("애초 요청하지 않은 서버에 대한 인증 결과다. impossible auth") );
		return;
	}

	//ServerSessionEx* pFindedSession = ServerSessionManager::FindServer( skKey );
	//if( !pFindedSession || !pFindedSession->IsConnected() )
	//{
	//	// 해당 세션은 인증을 요청해놓고 끊어진 상태다. 이 경우는 그냥 무시하자.
	//	return;
	//}

	// 여기까지 온 것은 해당 서버 인증이 가능한 상태라는 것이다.
	// 이전 Redirect를 수행토록 설정하자
	TempServerSessionEx::Redirect2NewSession( pMsgACK->m_dwInnerIP, skKey );

#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
}

HANDLER_SM_IMPL(SM_SERVER_INFO_AUTHENTICATION_NAK)
{
	// 당분간 접속을 물고 있다가 해제하는 것은 어떤가? 일단은 바로 끊어버릴까? 또 접속할 텐뎅?
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
	// 암호화된 패킷 사용 시작! 명령
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
			//   해당 처리는 현재 데이터 로딩을 요청한 이후에 대한 첫번째 응답에 대한 반응이다.
			// <1> 해당 스크립트 코드가 올바른 지 검사 -> F : NAK
			// <2> T : 해당 스크립트 코드에 버퍼를 할당한다.
			//     DataStreamCommand (SET blah~
			// <3> 기타 타임아웃등등의 보안정책을 강구할 것

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
			//   해당 처리는 현재 데이터 로딩을 요청에 대한 응답을 처리하며, 실재 데이터가 입력되어 온다.
			//   종결자가 올때까지 해당 토큰에 해당하는 위치에
			//   (해당 스크립에 할당된 버퍼를 얻어서 그 위치에) 데이터를 복사해 간다.
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
		// 이것으로 종료하도록 수행하자.
		// 완료되면, 해당 스크립트 데이터 완료 콜백(내부 연산 목적)을 수행한다.
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
				printf( "End Time : Transferred %u BYTE, 소요시간 %ums Transfer Rate %u (BYTE/SEC\n",
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


