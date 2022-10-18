#include "stdafx.h"

#include "MasterServerSession.h"
#include "ServerSessionManager.h"

MasterServerSession::MasterServerSession()
{
}

MasterServerSession::~MasterServerSession()
{
}

VOID MasterServerSession::Init()
{
	ServerSessionEx::Init();
	Services().SetSyncKey( MASTER_SERVER );
}

VOID MasterServerSession::Release()
{
	Services().CRYPTINFO.Release();
	ServerSessionEx::Release();
}

VOID MasterServerSession::Update()
{
	ServerSessionEx::Update();
}

// CHANGES: f110617.1L
void MasterServerSession::OnRedirect(
    DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID MasterServerSession::OnAccept( DWORD dwNetworkIndex )
{
	ASSERT( !"여기 들어와서는 안된다!" );
	__asm int 3;
}

VOID MasterServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSessionEx::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		SUNLOG(eCRITICAL_LOG, "Connected to master server." );
	}
}

VOID MasterServerSession::OnDisconnect()
{
	ServerSessionManager* pManager = ServerSessionManager::Instance();
	SERVER_SESSION_DELEGATOR& rSession = pManager->FindServer( this );

	if( rSession == NULL )
	{
		MessageOutEx( g_pSunLog, CSunLog::cCRITICAL_LOG, CSunLog::cOUTPUT_BOTH, CSunLog::cCATE_CONNECTION,
			"[MasterServerSession] 이미 해제한 연결에 대한 제거를 수행하려고 하고 있다. Idx[%d]",
			this->GetSessionIndex()
			);
		return;
	}

	pManager->RemoveServer( rSession );

	ServerSessionEx::OnDisconnect();

	//SUNLOG(eCRITICAL_LOG, "Disconnected from master server." );
}

BOOL MasterServerSession::SendPacket( MSG_BASE* pMsg, WORD wSize )
{
	Services4Session& rService = Services();
	Services4Session::sCRYPTINFO& rCRYPT = rService.CRYPTINFO;

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
												rService.GetServerIPAddress(),
												rService.GetSeqCryptKey() );

		wSize += (WORD)pENCODING->GetHeaderSize();

		bRet = Send( (BYTE*)pENCODING, wSize );

		if( bRet )
			rCRYPT.NextRoundSNKey4Send();
		else
			printf( "Send실패\n" );

		return bRet;
	}
	else
	{
		ASSERT( !"MasterServer상태에서 이곳에 들어오는 경우를 찾아랑~~" );
	}
	return FALSE;
}

VOID MasterServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	Services4Session& rService = Services();
	Services4Session::sCRYPTINFO& rCRYPT = rService.CRYPTINFO;

	CryptFlowPolicy::MakePacket::PostRecv_DefConverter(
											pMsg,
											rService.GetServerIPAddress(),
											rService.GetSeqCryptKey() );

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

					Services().GetPacketHandler()->ParsePacket( MASTER_SERVER, this, pBASE, wSize );

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
