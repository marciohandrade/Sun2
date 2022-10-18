
#include "stdafx.h"

#include <PacketStruct_SM.h>

#include "PacketHandler.h"
#include "HandlerFromTempServer.h"

#include <SolarAuthServer.h>
#include "Sessions/TempServerSession.h"


#define SE_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define SE_HANDLER_INFO( c, p, accptablePHASE )		\
	{ MAKEWORD(c, p), accptablePHASE, (PH_fnHandler)&HandlerFromTempServer::On##p }


sPACKET_HANDLER_INFO*	HandlerFromTempServer::GetHandlerInfo()
{

	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//
		//SE_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_HEARTBEAT, PHASE_UNKNOWN),
		SE_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_CONNECTION_SYN, PHASE_UNKNOWN),
		//SE_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_CONNECTION_ACK, PHASE_UNKNOWN),
		//SE_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_CONNECTION_NAK, PHASE_UNKNOWN),
		//SE_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_DISCONNECT_CMD, (ePHASE_SHIFT)(PHASE_UNKNOWN|PHASE_EXCHANGE)),
		//SE_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_PHASE_SHIFT_CMD, PHASE_UNLIMITED),
		//SE_HANDLER_INFO(SM_CONNECTION, SM_CONNECTION_BLOCK_CMD, PHASE_UNLIMITED),
		////
		//SE_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_SYN, PHASE_EXCHANGE),
		//SE_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_ACK, PHASE_EXCHANGE),
		//SE_HANDLER_INFO(SM_SERVER_INFO, SM_SERVER_INFO_AUTHENTICATION_NAK, PHASE_EXCHANGE),
		////
		SE_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_KEY, PHASE_EXCHANGE),
		SE_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_ACK, PHASE_EXCHANGE),
		SE_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_NAK, PHASE_EXCHANGE),
		//SE_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_CMD, PHASE_EXCHANGE),
		//SE_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_PACKET_ENCODED, PHASE_EXCHANGE),	//Test
		//SE_HANDLER_INFO(SM_CRYPTOGRAPHY, SM_CRYPTOGRAPHY_PACKET_ENCODING, PHASE_EXCHANGE),	//Test
		////
		//SE_HANDLER_INFO(SE_DATA_TRANSFER, SE_DATA_TRANSFER_TEMPORARY, (ePHASE_SHIFT)(PHASE_UNKNOWN|PHASE_EXCHANGE)),
		SE_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

VOID	HandlerFromTempServer::RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance )
{
	//BOOL bResult =
	pInstance->RegisterHandler_<TEMP_SERVER>( GetHandlerInfo() );
	//
}


HandlerFromTempServer_IMPL( SM_CONNECTION_HEARTBEAT )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

//
// <Flow>
//   E --> M
// <Description>
//   최초 커넥션에 대한 1차 접속 통과 이후, Temp Server가 논리적 레벨에서 접속 요청 패킷을 날린다.
HandlerFromTempServer_IMPL( SM_CONNECTION_CONNECTION_SYN )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	DWORD& dwAccessCount = pServerSession->AccessCount();
	union
	{
		DWORD	dwIP;
		BYTE	byIP[4];
	};

	dwIP = pServerSession->GetIPAddress();

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
			PostAction_SM_CONNECTION_CONNECTION_SYN( pServerSession, dwAccessCount );
			++dwAccessCount;
		}
		return;
	case 1:
		{
			PostAction_SM_CONNECTION_CONNECTION_SYN( pServerSession, dwAccessCount );
			++dwAccessCount;

			MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
				_T("Master Server와의 연결에 성공했습니다. IP : %u.%u.%u.%u"),
				byIP[0], byIP[1], byIP[2], byIP[3] );

			PostAction_SM_CONNECTION_COMPLETE( pServerSession );
		}
		return;
	}

	// Disconnect 처리할 것

	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
		"비정상적인 초기 접속 시퀀스가 발생했습니다. 인증되지 않은 MASTER SERVER IP : %u.%u.%u.%u",
		byIP[0], byIP[1], byIP[2], byIP[3] );
}
//
//HandlerFromTempServer_IMPL( SM_CONNECTION_CONNECTION_ACK )
//{
//}
//
//HandlerFromTempServer_IMPL( SM_CONNECTION_CONNECTION_NAK )
//{
//}
//
//HandlerFromTempServer_IMPL( SM_CONNECTION_DISCONNECT_CMD )
//{
//}
//
//HandlerFromTempServer_IMPL( SM_CONNECTION_PHASE_SHIFT_CMD )
//{
//}
//
//HandlerFromTempServer_IMPL( SM_CONNECTION_BLOCK_CMD )
//{
//}
//
////
//HandlerFromTempServer_IMPL(SM_SERVER_INFO_AUTHENTICATION_SYN)
//{
//}
//
//HandlerFromTempServer_IMPL(SM_SERVER_INFO_AUTHENTICATION_ACK)
//{
//}
//
//HandlerFromTempServer_IMPL(SM_SERVER_INFO_AUTHENTICATION_NAK)
//{
//}

//
////
HandlerFromTempServer_IMPL( SM_CRYPTOGRAPHY_KEY )
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

#include "Sessions/ServerSessionManager.h"

HandlerFromTempServer_IMPL( SM_CRYPTOGRAPHY_ACK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	printf( "RECEIVED {SM_CRYPTOGRAPHY_ACK} SEND {SM_CRYPTOGRAPHY_CMD}\n" );

	MSG_SM_CRYPTOGRAPHY_CMD msgCMD;
	pServerSession->SendPacket( &msgCMD, sizeof(msgCMD) );

	Services4Session& rService = pServerSession->Services();
	Services4Session::Crypt::StartPacketCrypt( pServerSession->Services() );

#define __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION_SEQUENCE_TEST__
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION_SEQUENCE_TEST__
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 다음은 테스트 시작을 위한 처리 및 패킷이다.

	SERVER_SESSION_DELEGATOR pNewSession = SolarAuthServer::Handler_AllocServerSession( MASTER_SERVER );

	if( pNewSession == NULL )
	{
		pServerSession->Disconnect();
		//pServerSession->OnDisconnect();
		return;
	}

	//// 새로운 NetworkObject로 교체
	//pServerSession->Redirect( pNewSession() );
	//// 새로운 세션에 정보 복사
	//pNewSession->SetSessionIndex( pServerSession->GetSessionIndex() );
	////	pNewSession->OnRedirect();
	//// TempServer 정리하고
	//GLOBAL_SERVER_KEY globalServerKey = pServerSession->Services().GetGlobalServerKey();

	pServerSession->MigrateObject( (MasterServerSession*)pNewSession() );


	pServerSession->Release();
	ServerSessionManager::Instance()->RemoveServer( pServerSession );
	SolarAuthServer::Handler_FreeServerSession_NoRelease( pServerSession );

	// 실제 사용할 세션을 매니저에 추가
	ServerSessionManager::Instance()->AddServer( pNewSession );
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION_SEQUENCE_TEST__


	// 첫 암호화된 패킷이 될 것이다.
#ifndef __WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__
	MSG_SM_CONNECTION_PHASE_SHIFT_CMD msgCMD_SHIFT;
	msgCMD_SHIFT.m_dwPHASE_SHIFT = (DWORD)PHASE_SERVICE;

	pNewSession->SendPacket( &msgCMD_SHIFT, sizeof(msgCMD_SHIFT) );
	printf( "SEND {MSG_SM_CONNECTION_PHASE_SHIFT_CMD} EXCHANGE->SERVICE\n" );
#endif //__WAVERIX_SERVER_AUTHENTICATION_BY_SOLARAUTH__


	//BYTE* ppp = (BYTE*)PACKET_CONTROL_SM::GetPacketPtr_ENCODING();
	//for( int i=0 ; i<20 ; ++i )
	//{
	//	printf( "[%02X]", *ppp );
	//	++ppp;
	//}
	//printf( "\n" );
}

HandlerFromTempServer_IMPL( SM_CRYPTOGRAPHY_NAK )
{
    __TOUCHED((pServerSession, pMsg, wSize));
	printf( "RECEIVED {SM_CRYPTOGRAPHY_NAK} SEND {SM_CONNECTION_DISCONNECT_CMD}\n" );
	MSG_SM_CONNECTION_DISCONNECT_CMD msgCMD;

	pServerSession->SendPacket( &msgCMD, sizeof(msgCMD) );
}

//HandlerFromTempServer_IMPL( SM_CRYPTOGRAPHY_CMD )
//{
//}
//
//HandlerFromTempServer_IMPL( SM_CRYPTOGRAPHY_PACKET_ENCODED )
//{
//}
//
//HandlerFromTempServer_IMPL( SM_CRYPTOGRAPHY_PACKET_ENCODING )
//{
//}
//
////
//HandlerFromTempServer_IMPL( SE_DATA_TRANSFER_TEMPORARY )
//{
//}





TEMPSERVER_POSTACTION_IMPL(SM_CONNECTION_CONNECTION_SYN)( TempServerSessionEx* pServerSession, DWORD dwAccessCount )
{
    __TOUCHED((pServerSession));
	printf( "RECEIVED {SM_CONNECTION_CONNECTION_SYN} " );
	switch( dwAccessCount )
	{
	case 0:
		{
			MSG_SM_CONNECTION_CONNECTION_ACK msgACK;
			DWORD dwSeqCryptKey =
				CryptFlowPolicy::MakePacket::PostRecv_SM_CONNECTION_SYN_1st(
													(BYTE*)&msgACK, pServerSession->Services().GetServerIPAddress()
													);

			pServerSession->Services().SetSeqCryptKey( dwSeqCryptKey );

			pServerSession->Send( (BYTE*)&msgACK, sizeof(MSG_SM_CONNECTION_CONNECTION_ACK) );
			printf( "SEND {MSG_SM_CONNECTION_CONNECTION_ACK} 1st\n" );
		}
		return TRUE;
	case 1:
		{
			MSG_SM_CONNECTION_CONNECTION_ACK msgACK;

			pServerSession->SendPacket( &msgACK, sizeof(MSG_SM_CONNECTION_CONNECTION_ACK) );
			printf( "SEND {MSG_SM_CONNECTION_CONNECTION_ACK} 2nd\n" );
		}
		return TRUE;
	}
	return FALSE;
}

TEMPSERVER_POSTACTION_IMPL(SM_CONNECTION_COMPLETE)( TempServerSessionEx* pServerSession )
{
	//ACTION 1
	MSG_SM_CONNECTION_PHASE_SHIFT_CMD msgCMD_SHIFT;
	msgCMD_SHIFT.m_dwPHASE_SHIFT = (DWORD)PHASE_EXCHANGE;

	pServerSession->SendPacket( &msgCMD_SHIFT, sizeof(msgCMD_SHIFT) );
	printf( "SEND {MSG_SM_CONNECTION_PHASE_SHIFT_CMD} UNKNOWN->EXCHANGE\n" );

	//ACTION 2
	// 키 교환 단계
	Services4Session& rService = pServerSession->Services();
	MSG_SM_CRYPTOGRAPHY_KEY msgKEY;

	Services4Session::Crypt::Init_ServerRoleManager( rService, &msgKEY );

	pServerSession->SendPacket( &msgKEY, sizeof(msgKEY) );

	printf( "SEND {MSG_SM_CRYPTOGRAPHY_KEY}\n" );

	return TRUE;
}


