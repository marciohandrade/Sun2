#include "SunClientPrecompiledHeader.h"
#include "network.h"
#include "SunClientSocket.h"
#include "version.h"
#include "globalfunc.h"

#define DUMMY_SWAP_PROC( A, B )		( (A)^=(B)^=(A)^=(B) )

//CProtocolCryptography::CProtocolCryptography()

VOID		CProtocolCryptography::InitCryptInfo( DWORD dwUserKey, BYTE byProtoMajorVer, BYTE byProtoMinorVer )
{
	// 기존과 다른 코드로...
	struct
	{
		union
		{
			ClientSNKey*	pSNKey;
			DWORD			dwDummy0;
		};
		union
		{
			BYTE			byMajorVer;
			DWORD			dwDummy1;
		};
		union
		{
			BYTE			byMinorVer;
			DWORD			dwDummy2;
		};
	} STK_VAL;

	STK_VAL.pSNKey		= &m_CryptInfo.m_SNKey;
	STK_VAL.byMajorVer	= byProtoMajorVer;
	STK_VAL.byMinorVer	= byProtoMinorVer;

	STK_VAL.pSNKey->Initialize( dwUserKey, STK_VAL.byMajorVer, STK_VAL.byMinorVer );
}


BOOL		CProtocolCryptography::SetCryptographyInfo( MSG_BASE * pMsg )
{
	MSG_CG_CRYPTOGRAPHY_KEY * pRecvPacket = (MSG_CG_CRYPTOGRAPHY_KEY *)pMsg;

	DWORD dwUserKey = CryptManager::GetUserKeyInCryptInfo( pRecvPacket->GetCryptStream() );

	InitCryptInfo( dwUserKey, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO );

	if( GetRecvSNKey() == pRecvPacket->GetSNKey() )
	{
		m_CryptInfo.m_pCryptManager = CryptManager::SetCryptAlgorithmInfo(
														GetRecvPairSNKey(),
														pRecvPacket->GetCryptStream(),
														pRecvPacket->GetCryptInfo() );
		UseCryptography( TRUE );

		NextRoundSNKey4Recv();

		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL		CProtocolCryptography::SetCryptographyInfo_SNK( MSG_BASE * pMsg )
{
	MSG_CG_CRYPTOGRAPHY_SNK_KEY* pRecvKey = (MSG_CG_CRYPTOGRAPHY_SNK_KEY*)pMsg;

	SetKey4SNKey( pRecvKey->m_dwSNKey );

	GetSNKey().Initialize( pRecvKey->m_dwSNKey, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO );
	
	return TRUE;
}

// 패킷의 인코딩
INT			CProtocolCryptography::EncodePacket( BYTE* pEncodedMsg, VOID* pSendMsg, INT iLength )
{
	if( IsUseCryptography() )
	{
		MSG_CG_CRYPTOGRAPHY_PACKET_ENCODING* pEncodingMsg = (MSG_CG_CRYPTOGRAPHY_PACKET_ENCODING*)pEncodedMsg;

		pEncodingMsg->SetSNKey( GetSendSNKey() );
		CopyMemory( pEncodingMsg->GetPacketStream(), pSendMsg, (SIZE_T)iLength );
		Encrypt( pEncodingMsg->GetPacketStream(), iLength, pEncodingMsg->GetCRC() );

		NextRoundSNKey4Send();

		return ( iLength + pEncodingMsg->GetHeaderSize() );
	}
	else
	{
		return 0;
	}
}


// 암호화된 패킷의 디코딩
BOOL		CProtocolCryptography::DecodePacket( SPacketBuffer& rDecodedBuffer, BYTE* pReceivedMsgPtr, INT iLength )
{
	if( IsUseCryptography() )
	{
		MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED* pEncodedMsg = (MSG_CG_CRYPTOGRAPHY_PACKET_ENCODED*)pReceivedMsgPtr;
		if( GetRecvSNKey() == pEncodedMsg->GetSNKey() )
		{
			rDecodedBuffer.CopyFrom( pReceivedMsgPtr, (WORD)iLength );
			if( Decrypt( rDecodedBuffer.PACKET_PTR, rDecodedBuffer.wSIZE, pEncodedMsg->GetCRC() ) )
			{
				NextRoundSNKey4Recv();
				return TRUE;
			}
		}
	}

	return FALSE;
}



//==============================================================================================

//CProtocolCryptographyMgr::CProtocolCryptographyMgr()
//{
//}
//
//CProtocolCryptographyMgr::~CProtocolCryptographyMgr()
//{
//	if(m_CryptoWrapper[0].GetCryptManager())
//	{
//		m_CryptoWrapper[0].GetCryptManager()->Release();
//		delete m_CryptoWrapper[0].GetCryptManager();
//	}
//}


BOOL		CProtocolCryptographyMgr::SetCryptographyInfo_SNK( INT nCryptoType, MSG_BASE* pMsg )
{
	if( nCryptoType < MAX_CRYPTO_TYPE )
		return m_CryptoWrapper[nCryptoType].SetCryptographyInfo_SNK( pMsg );
	else
		return FALSE;	
}

VOID		CProtocolCryptographyMgr::CopyRecvCryptographyInfoFromSend_SNK( VOID )
{
	DWORD& rdwSendSNKey = m_CryptoWrapper[SEND_CRYPTO].GetKey4SNKey();
	ClientSNKey& rRecvSNKey = m_CryptoWrapper[RECV_CRYPTO].GetSNKey();
	rRecvSNKey.Initialize( rdwSendSNKey, C2S_HIGH_VERSION_NO, C2S_MIDDLE_VERSION_NO );
	rdwSendSNKey = 0;
}






