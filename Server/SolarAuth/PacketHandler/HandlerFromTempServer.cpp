
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
//   ���� Ŀ�ؼǿ� ���� 1�� ���� ��� ����, Temp Server�� ���� �������� ���� ��û ��Ŷ�� ������.
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
			PostAction_SM_CONNECTION_CONNECTION_SYN( pServerSession, dwAccessCount );
			++dwAccessCount;
		}
		return;
	case 1:
		{
			PostAction_SM_CONNECTION_CONNECTION_SYN( pServerSession, dwAccessCount );
			++dwAccessCount;

			MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
				_T("Master Server���� ���ῡ �����߽��ϴ�. IP : %u.%u.%u.%u"),
				byIP[0], byIP[1], byIP[2], byIP[3] );

			PostAction_SM_CONNECTION_COMPLETE( pServerSession );
		}
		return;
	}

	// Disconnect ó���� ��

	MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
		"���������� �ʱ� ���� �������� �߻��߽��ϴ�. �������� ���� MASTER SERVER IP : %u.%u.%u.%u",
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
	// ������ �׽�Ʈ ������ ���� ó�� �� ��Ŷ�̴�.

	SERVER_SESSION_DELEGATOR pNewSession = SolarAuthServer::Handler_AllocServerSession( MASTER_SERVER );

	if( pNewSession == NULL )
	{
		pServerSession->Disconnect();
		//pServerSession->OnDisconnect();
		return;
	}

	//// ���ο� NetworkObject�� ��ü
	//pServerSession->Redirect( pNewSession() );
	//// ���ο� ���ǿ� ���� ����
	//pNewSession->SetSessionIndex( pServerSession->GetSessionIndex() );
	////	pNewSession->OnRedirect();
	//// TempServer �����ϰ�
	//GLOBAL_SERVER_KEY globalServerKey = pServerSession->Services().GetGlobalServerKey();

	pServerSession->MigrateObject( (MasterServerSession*)pNewSession() );


	pServerSession->Release();
	ServerSessionManager::Instance()->RemoveServer( pServerSession );
	SolarAuthServer::Handler_FreeServerSession_NoRelease( pServerSession );

	// ���� ����� ������ �Ŵ����� �߰�
	ServerSessionManager::Instance()->AddServer( pNewSession );
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION_SEQUENCE_TEST__


	// ù ��ȣȭ�� ��Ŷ�� �� ���̴�.
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
	// Ű ��ȯ �ܰ�
	Services4Session& rService = pServerSession->Services();
	MSG_SM_CRYPTOGRAPHY_KEY msgKEY;

	Services4Session::Crypt::Init_ServerRoleManager( rService, &msgKEY );

	pServerSession->SendPacket( &msgKEY, sizeof(msgKEY) );

	printf( "SEND {MSG_SM_CRYPTOGRAPHY_KEY}\n" );

	return TRUE;
}


