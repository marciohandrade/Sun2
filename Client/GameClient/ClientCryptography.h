
#ifndef	_CLIENT_CRYPTOGRAPHY_H
#define	_CLIENT_CRYPTOGRAPHY_H

#pragma once

#include <CryptManager_4Client.h>

//#include "CryptManager.h"

//
#define ALIGN32BYTE(size) (((size) + 0x1F) & ~0x1F)

//////////////////////////////////////////////////////////////////////////
// Client 에서 사용하는 Cryptography Wrapper

class CProtocolCryptography
{
private :
	// <FIELDs>
	//BOOL					m_bUseCryptography;
	//ClientSNKey				m_SNKey;
	//CryptManagerDeletator	m_pCryptManager;
	//DWORD					m_dwKey;
	sCLIENT_CRYPT_INFO		m_CryptInfo;

public :

	//////////////////////////////////////////////////////////////////////////
	// <CONSTRUCTOR><DESTRUCTOR>
	CProtocolCryptography()		{ InitData(); }
	~CProtocolCryptography()	{ InitData(); }

	// <INITIALIZE ROUTINES>
	inline VOID				InitData( VOID );
public:
	inline sCLIENT_CRYPT_INFO*	GetCryptData() { return &m_CryptInfo; }

	// <암호화 사용 여부 설정>
	inline BOOL				IsUseCryptography( VOID )			{ return m_CryptInfo.m_bUseCryptography; }
	inline BOOL				UseCryptography( BOOL bInited )		{ return m_CryptInfo.m_bUseCryptography = m_CryptInfo.m_pCryptManager->StartLogic( bInited ); }
	// <SNK 초기화> 함수호출로 변환하자.
	VOID					InitCryptInfo( DWORD dwUserKey, BYTE byProtoMajorVer, BYTE byProtoMinorVer );

	// CryptManger Get/Set
	inline VOID				SetCryptManager( CryptManager* pCryptManager )	{ m_CryptInfo.m_pCryptManager = pCryptManager; }
	inline CryptManagerDeletator&	GetCryptManager( VOID )						{ return m_CryptInfo.m_pCryptManager; }

	// <SNK 컨트롤>
	inline ClientSNKey&		GetSNKey()							{ return m_CryptInfo.m_SNKey; }
	inline VOID				SetSNKey( ClientSNKey& snKey )		{ m_CryptInfo.m_SNKey = snKey; }
	inline DWORD&			GetKey4SNKey()						{ return m_CryptInfo.m_dwKey; }
	inline VOID				SetKey4SNKey( DWORD key )			{ m_CryptInfo.m_dwKey = key; }


	// <SNK 컨트롤><GET SEND/RECV KEY>
	inline VOID				NextRoundSNKey4Send( VOID )			{ m_CryptInfo.m_SNKey.NextRoundSNKey4Send(); };
	inline VOID				NextRoundSNKey4Recv( VOID )			{ m_CryptInfo.m_SNKey.NextRoundSNKey4Recv(); };
	inline BYTE				GetSendSNKey( VOID )				{ return m_CryptInfo.m_SNKey.GetSendSNKey( RandomSNKey::c_dwBaseKey ); };
	inline BYTE				GetSendPairSNKey( VOID )			{ return m_CryptInfo.m_SNKey.GetSendSNKey( RandomSNKey::c_dwPairKey ); };
	inline BYTE				GetRecvSNKey( VOID )				{ return m_CryptInfo.m_SNKey.GetRecvSNKey( RandomSNKey::c_dwBaseKey ); };
	inline BYTE				GetRecvPairSNKey( VOID )			{ return m_CryptInfo.m_SNKey.GetRecvSNKey( RandomSNKey::c_dwPairKey ); };

	// <CryptManager Control>
	// MSG_CG_CRYPTOGRAPHY_KEY 수신시 CryptManager Instance 만들기
	BOOL					SetCryptographyInfo( MSG_BASE * pMsg );
	// MSG_CG_CRYPTOGRAPHY_SNK_KEY 수신시 SNK 변환
	BOOL					SetCryptographyInfo_SNK( MSG_BASE * pMsg );
	//   Get Instance for CLIENT
	inline CryptManager*	SetCryptAlgorithmInfo( BYTE byPairSNKey, BYTE* IN cryptInfo, BYTE cryptIdx );

	//
	inline BOOL				Encrypt( BYTE* IN OUT pEncryptMsg, const INT size, BYTE& OUT crc );
	inline BOOL				Decrypt( BYTE* IN OUT pEncryptMsg, const INT size, BYTE IN crc );

	// ClientLogic::CProtocolCryptographyMgr <-> CProtocolCryptography
	INT						EncodePacket( BYTE* pEncodedMsg, VOID* pMsgPtr, INT iLength );
	BOOL					DecodePacket( SPacketBuffer& rDecodedBuffer, BYTE* pReceivedMsgPtr, INT iLength );

};

//////////////////////////////////////////////////////////////////////////
// Network Session 당 할당되는... SEND/RECV Cryptography
class CProtocolCryptographyMgr
{
public:
	// <TYPE DEFINITION>
	enum { SEND_CRYPTO, RECV_CRYPTO, MAX_CRYPTO_TYPE };

private:
	// [SEND][RECV]
	CProtocolCryptography	m_CryptoWrapper[MAX_CRYPTO_TYPE];

public:
	CProtocolCryptographyMgr() { InitData( ); }
	~CProtocolCryptographyMgr() { InitData( ); }

	inline VOID			InitData( VOID );
	inline sCLIENT_CRYPT_INFO*	GetSendCryptData() { return m_CryptoWrapper[SEND_CRYPTO].GetCryptData(); }
	inline sCLIENT_CRYPT_INFO*	GetRecvCryptData() { return m_CryptoWrapper[RECV_CRYPTO].GetCryptData(); }

	inline BOOL			IsUseSendCryptography( VOID )		{ return m_CryptoWrapper[SEND_CRYPTO].IsUseCryptography(); }
	inline BOOL			IsUseRecvCryptography( VOID )		{ return m_CryptoWrapper[RECV_CRYPTO].IsUseCryptography(); }

	inline INT			EncodePacket( BYTE* pEncodedMsg, VOID* pSendProtocol, INT iLength );
	inline BOOL			DecodePacket( SPacketBuffer& rDecodedBuffer, BYTE* pReceivedMsgPtr, INT iLength );



	// Client Logic <-> Cryptography
	inline BOOL			SetCryptographyInfo( INT nCryptoType, MSG_BASE * pMsg );
	// Routine for RECV
	inline VOID			CopyRecvCryptographyInfoFromSend( VOID );

	//////////////////////////////////////////////////////////////////////////
	// <KEY EXCHANGE ROUTINEs>
	VOID				CopyRecvCryptographyInfoFromSend_SNK( VOID );
	BOOL				SetCryptographyInfo_SNK( INT nCryptoType, MSG_BASE * pMsg );
	//
	//////////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VOID	CProtocolCryptography::InitData( VOID )
{
	m_CryptInfo.m_bUseCryptography	= FALSE;
	// m_CryptInfo.m_SNKey
	m_CryptInfo.m_pCryptManager.Release();
	m_CryptInfo.m_dwKey				= 0;
}

CryptManager*	CProtocolCryptography::SetCryptAlgorithmInfo( BYTE byPairSNKey, BYTE* IN cryptInfo, BYTE cryptIdx )
{
	return CryptManager::SetCryptAlgorithmInfo( byPairSNKey, cryptInfo, cryptIdx );
}

BOOL			CProtocolCryptography::Encrypt( BYTE* IN OUT pEncryptMsg, const INT size, BYTE& OUT crc )
{
	return GetCryptManager()->Encrypt( pEncryptMsg, size, crc );
}

BOOL			CProtocolCryptography::Decrypt( BYTE* IN OUT pEncryptMsg, const INT size, BYTE IN crc )
{
	return GetCryptManager()->Decrypt( pEncryptMsg, size, crc );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

VOID			CProtocolCryptographyMgr::InitData( VOID )
{
	for( INT i = 0; i < MAX_CRYPTO_TYPE; ++i )
		m_CryptoWrapper[i].InitData();
}


INT				CProtocolCryptographyMgr::EncodePacket( BYTE* pEncodedMsg, VOID* pSendProtocol, INT iLength )
{
	CProtocolCryptography& rSendCrypt = m_CryptoWrapper[SEND_CRYPTO];
	return rSendCrypt.EncodePacket( pEncodedMsg, pSendProtocol, iLength );
}


BOOL			CProtocolCryptographyMgr::DecodePacket( SPacketBuffer& rDecodedBuffer, BYTE* pReceivedMsgPtr, INT iLength )
{
	CProtocolCryptography& rRecvCrypt = m_CryptoWrapper[RECV_CRYPTO];
	return rRecvCrypt.DecodePacket( rDecodedBuffer, pReceivedMsgPtr, iLength );
}



BOOL			CProtocolCryptographyMgr::SetCryptographyInfo( INT nCryptoType, MSG_BASE* pMsg )
{
	if( nCryptoType < MAX_CRYPTO_TYPE )
		return m_CryptoWrapper[nCryptoType].SetCryptographyInfo( pMsg );
	else
		return FALSE;
}

VOID			CProtocolCryptographyMgr::CopyRecvCryptographyInfoFromSend( VOID )
{
	memcpy( &m_CryptoWrapper[RECV_CRYPTO], &m_CryptoWrapper[SEND_CRYPTO], sizeof(CProtocolCryptography) );
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


#endif	//_CLIENT_CRYPTOGRAPHY_H
