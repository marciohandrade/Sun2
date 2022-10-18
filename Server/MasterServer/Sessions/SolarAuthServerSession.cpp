#include "stdafx.h"
#include "SolarAuthServerSession.h"

#include <MasterServerEx.h>

SolarAuthServerSession::SolarAuthServerSession()
{
}

SolarAuthServerSession::~SolarAuthServerSession()
{
}

//-------------------------------------------------------------------------------------------------------------
VOID SolarAuthServerSession::Init()
{
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	const CHAR* ipAddress = pMasterServer->GetIPAddress_Outside();

	ServerSessionEx::Init();

	m_dwAccessCount = 0;
	m_dwSeqCryptKey = 0;
	m_dwLocalServerIPAddress = CryptFlowPolicy::Utility::GetIPAddress( ipAddress );

	CRYPTINFO.Init();
}

//-------------------------------------------------------------------------------------------------------------
VOID SolarAuthServerSession::Release()
{
	CRYPTINFO.Release();
	ServerSessionEx::Release();
}

VOID	SolarAuthServerSession::SendServerType()
{
}

VOID SolarAuthServerSession::SendHeartbeatPacket()
{
	//printf( "heartbeat!\n" );
	//	MSG_SM_CONNECTION_HEARTBEAT msg;
	//	Send( (BYTE*)&msg, sizeof(msg) );
}

BOOL	SolarAuthServerSession::Send( BYTE* pMsg, WORD wSize )
{
	return NetworkObject::Send( pMsg, wSize );
}

// 암호화 된 정보용
BOOL	SolarAuthServerSession::SendPacket( MSG_BASE* pMsg, WORD wSize )
{
	return Send( (BYTE*)pMsg, wSize );
}


// CHANGES: f110617.1L
void SolarAuthServerSession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

//-------------------------------------------------------------------------------------------------------------
VOID	SolarAuthServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
		pMasterServer->ConnectSessions.SessionSolarAuth() = this;

		MSG_SM_CONNECTION_CONNECTION_SYN msgSYN;
		this->Send( (BYTE*)&msgSYN, sizeof(MSG_SM_CONNECTION_CONNECTION_SYN) );
	}
	else
	{
	}
}

//-------------------------------------------------------------------------------------------------------------
VOID	SolarAuthServerSession::OnDisconnect()
{
	MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
	const CHAR* ipAddress = pMasterServer->GetIPAddress_Outside();

	pMasterServer->SetPhaseStatus( PHASE_UNKNOWN );
	pMasterServer->ConnectSessions.SessionSolarAuth() = NULL;

	m_dwAccessCount = 0;

	m_dwSeqCryptKey = 0;
	m_dwLocalServerIPAddress = CryptFlowPolicy::Utility::GetIPAddress( ipAddress );

	if( CRYPTINFO.IsCrypted() )
	{
		SolarAuthServerSessionCrypted::ChangeObjectNoCrypt( (SolarAuthServerSessionCrypted*)this );
		CRYPTINFO.Release();
	}

	// 이것 여기다 두면 좋지 않다. 좀더 생각해 볼 것
	// 예를 들어, 재접속 했을 때 할 일을 등록하는 것은 어떨까?
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	ScriptDataManager::ReRegisterReloadDataList();
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__



	ServerSessionEx::OnDisconnect();
}

//-------------------------------------------------------------------------------------------------------------
VOID SolarAuthServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	MSG_BASE *pBaseMsg = (MSG_BASE*)pMsg;

	DWORD dwIpAddr = GetLocalServerIPAddress();	// 현재 자기자신의 CryptKey를 얻는 방법이 없다.
	DWORD dwSeqKey = GetSeqCryptKey();

	if( AccessCount() == 0 )
	{
		dwSeqKey = CryptFlowPolicy::MakePacket::PostRecv_SM_CONNECTION_ACK_1st( pMsg, dwIpAddr );
		SetSeqCryptKey( dwSeqKey );
	}
	else
	{
		CryptFlowPolicy::MakePacket::PostRecv_DefConverter( pMsg, dwIpAddr, dwSeqKey );
	}

	// 암호화된 패킷 여부를 판별해서 Decode하는 로직 추가할 것.
	Services().GetPacketHandler()->ParsePacket( SOLARAUTH_SERVER, this, (MSG_BASE*)pMsg, wSize );
}



#include <SolarAuth.CryptFlowPolicy.h>

sOBJECT_CHANGER* SolarAuthServerSessionCrypted::sm_pOBJECT_CHANGER = NULL;

VOID	SolarAuthServerSessionCrypted::CreateCode()
{
    sm_pOBJECT_CHANGER = sOBJECT_CHANGER::CreateObjectChanger(SolarAuthServerSessionCrypted::MAX);

	sm_pOBJECT_CHANGER->RegisterObject<SolarAuthServerSession>(SolarAuthServerSessionCrypted::NO);
	sm_pOBJECT_CHANGER->RegisterObject<SolarAuthServerSessionCrypted>(SolarAuthServerSessionCrypted::YES);
}

VOID
SolarAuthServerSessionCrypted::ChangeObjectNoCrypt( SolarAuthServerSessionCrypted* pSolarAuthCrypted )
{
	sm_pOBJECT_CHANGER->ObjectChangeTo( pSolarAuthCrypted, SolarAuthServerSessionCrypted::NO );
}

VOID
SolarAuthServerSessionCrypted::ChangeObjectCrypted( SolarAuthServerSession* pSolarAuthNoCrypt )
{
	sm_pOBJECT_CHANGER->ObjectChangeTo( pSolarAuthNoCrypt, SolarAuthServerSessionCrypted::YES );
}

//

BOOL	SolarAuthServerSessionCrypted::SendPacket( MSG_BASE* pMsg, WORD wSize )
{
	sCRYPTINFO& rCRYPT = CRYPTINFO;

	BOOL bRet = FALSE;
	BYTE* pBYTE;

	pBYTE = PACKET_CONTROL_SM::GetBasePacketPtr4Send();
	if( (BYTE*)pMsg != pBYTE )
		CopyMemory( pBYTE, pMsg, (SIZE_T)wSize );

	if( rCRYPT.IsCrypted() )
	{
		MSG_SM_CRYPTOGRAPHY_PACKET_ENCODING* pENCODING = PACKET_CONTROL_SM::GetPacketPtr_ENCODING();

		ASSERT( wSize && wSize<16000 );

		pENCODING->SetSNKey( rCRYPT.GetSendSNK() );
		rCRYPT.CryptManager().Encrypt( pENCODING->GetPacketStream(), wSize, pENCODING->GetCRC() );

		CryptFlowPolicy::MakePacket::PreSend_DefConverter(
			(BYTE*)pENCODING,
			GetLocalServerIPAddress(),
			GetSeqCryptKey() );

		wSize += (WORD)pENCODING->GetHeaderSize();

		bRet = Send( (BYTE*)pENCODING, wSize );

		if( bRet )
			rCRYPT.NextRoundSNKey4Send();
		else
		{
			__WAVERIX_SOLAR_AUTH_DEBUG_CODE( printf( "Send실패\n" ); );
		}

		return bRet;
	}
	else
	{
		ASSERT( !"MasterServer상태에서 이곳에 들어오는 경우를 찾아랑~~" );
	}
	return FALSE;
}

VOID	SolarAuthServerSessionCrypted::OnRecv( BYTE *pMsg, WORD wSize )
{
	////------------------------------------
	//BYTE* ppp = pMsg;
	//for( int i=0 ; i<(int)wSize ; ++i )
	//{
	//	printf( "[%02X]", *ppp );
	//	++ppp;
	//}
	//printf( "\n" );
	////------------------------------------

	sCRYPTINFO& rCRYPT = this->CRYPTINFO;

	CryptFlowPolicy::MakePacket::PostRecv_DefConverter(
		pMsg,
		GetLocalServerIPAddress(),
		GetSeqCryptKey() );

	union
	{
		MSG_BASE* pBASE;
		MSG_SM_CRYPTOGRAPHY_PACKET_ENCODED* pENCODED;
	};

	BOOL bIsCrypted = rCRYPT.IsCrypted();

	if( bIsCrypted )
		pENCODED = PACKET_CONTROL_SM::GetPacketPtr_ENCODED();
	else
		pBASE = (MSG_BASE*)PACKET_CONTROL_SM::GetBasePacketPtr4Recv();

	if( (BYTE*)pBASE != pMsg )
		CopyMemory( pBASE, pMsg, (SIZE_T)wSize );

	if( bIsCrypted )
	{
		if( rCRYPT.GetRecvSNK() == pENCODED->GetSNKey() )
		{
			DWORD dwSize = (DWORD)wSize - pENCODED->GetHeaderSize();
			if( dwSize && (dwSize<16000) )
			{
				if( rCRYPT.CryptManager().Decrypt( pENCODED->GetPacketStream(), dwSize, pENCODED->GetCRC() ) )
				{
					rCRYPT.NextRoundSNKey4Recv();

					pBASE = (MSG_BASE*)PACKET_CONTROL_SM::GetBasePacketPtr4Recv();
					wSize = (WORD)dwSize;
					//printf( "C:P = { %u:%u }\n", pBASE->m_byCategory, pBASE->m_byProtocol );

					Services().GetPacketHandler()->ParsePacket( SOLARAUTH_SERVER, this, (MSG_BASE*)pBASE, wSize );
					return;
				}
			}
		}
	}
	else
	{
		ASSERT( !"MasterServer상태에서 이곳에 들어오는 경우를 찾아랑~~" );
		return;
	}

	ASSERT( !"허허..." );
}


VOID SolarAuthServerSession::Crypt::Init_ClientRoleManager( SolarAuthServerSession* pServerSession, MSG_SM_CRYPTOGRAPHY_KEY* pMsgKEY )
{
	sCRYPTINFO& rCRYPT = pServerSession->CRYPTINFO;
	DWORD dwRndKey = rCRYPT.CryptManager().ExtractUserKeyFromCryptKeyStream( (CRYPT_KEY_STREAM)pMsgKEY->GetCryptStream() );
	rCRYPT.RandomSNK().Initialize( dwRndKey, 7, 4 /*C2S_MIDDLE_VERSION_NO, C2S_LOW_VERSION_NO*/ );	// 현재 HIGH, MIDDLE 값은 rnd값을 만들지 않는다.

	CryptManagerS2S* pCryptManager = 
		rCRYPT.CryptManager().CreateCryptManagerFromCryptKeyStream(
		rCRYPT.GetRecvPairSNK(),
		(CRYPT_KEY_STREAM)pMsgKEY->GetCryptStream(),
		pMsgKEY->CryptInfo()
		);
	rCRYPT.m_pCryptManager = pCryptManager;
}

VOID SolarAuthServerSession::Crypt::StartPacketCrypt( SolarAuthServerSession* pServerSession )
{
	sCRYPTINFO& rCRYPT = pServerSession->CRYPTINFO;

	if( rCRYPT.CryptManager().StartLogic( TRUE ) )
	{
		rCRYPT.IsCrypted() = TRUE;
		return;
	}

	ASSERT( !"패킷 암호화 시작 실패" );
}

