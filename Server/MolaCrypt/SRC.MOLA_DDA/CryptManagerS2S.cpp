//=============================================================================================================================
/// CryptManagerS2S class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 10. 12
	@remark
		- 서버와 서버 사이에 사용되게 되는 패킷 암호화 처리 관리자
	@note
	@history 
		-  
*/
//=============================================================================================================================

#include "stdafx.h"

#include "CryptManagerS2S.h"

#include "BitOperation.h"
#include "CryptHSEL.h"
#include "JSCode.h"


//#define ASSERT(expr)				_ASSERTE(expr)


static struct
{
	DWORD	s_dwStackedGarbageKey;
} s_MOLA_DDA_INFO =
{
	0,
};

CryptManagerS2S::CryptManagerS2S()
{
	m_Inited			= FALSE;
	m_RoleSide			= ROLE_NOALIGN;
	m_pCryptAlgorithm	= NULL;
}

CryptManagerS2S::~CryptManagerS2S()
{
	if( m_Inited )
	{
		Release();
	}
	else
	{
		m_Inited			= FALSE;
		m_RoleSide			= ROLE_NOALIGN;
		m_pCryptAlgorithm	= NULL;
	}
}


VOID	CryptManagerS2S::Initialize( eROLE_SIDE role /*추가사항 있는지 확인할 것*/ )
{
	if( m_Inited )
	{
		//ASSERT( !"한번 이상 초기화?" );
		return;
	}

	if( ROLE_SERVER == role )
	{
		m_RoleSide = ROLE_SERVER;
		CryptHSEL* pHSEL = new CryptHSEL();
		pHSEL->Create();

		m_pCryptAlgorithm = (PVOID)pHSEL;

		m_Inited = TRUE;

		return;
	}
	else if( ROLE_CLIENT == role )
	{
		m_RoleSide = ROLE_CLIENT;

		m_Inited = TRUE;
		return;
	}

	//ASSERT( !"있어서는 안되는 경우" );
}

VOID	CryptManagerS2S::Release()
{
	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptAlgorithm;

	if( pHSEL )
		delete pHSEL;

	m_Inited			= FALSE;
	m_RoleSide			= ROLE_NOALIGN;
	m_pCryptAlgorithm	= NULL;
}

BOOL	CryptManagerS2S::Encrypt( BYTE* IN OUT stream, int size, BYTE& OUT crc )
{
	//ASSERT( m_pCryptAlgorithm );

	if( m_Inited == FALSE )
		return FALSE;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptAlgorithm;

	BOOL bRet = pHSEL->Encrypt( (CHAR*)stream, size );

	if( bRet )
	{
		BYTE byCRC = pHSEL->GetEnCRCConvertChar();
		crc = byCRC;
	}

	return bRet;
}

BOOL	CryptManagerS2S::Decrypt( BYTE* IN OUT stream, int size, BYTE IN crc )
{
	//ASSERT( m_pCryptAlgorithm );

	if( m_Inited == FALSE )
		return FALSE;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptAlgorithm;

	BOOL bRet = pHSEL->Decrypt( (CHAR*)stream, size );

	return bRet;
}

BOOL	CryptManagerS2S::StartLogic( BOOL bset )
{
	if( m_Inited == FALSE )
		return FALSE;

	//ASSERT( m_pCryptAlgorithm );

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptAlgorithm;

	pHSEL->SetInit( bset );
	m_Inited = TRUE;
	return TRUE;
}

// 200 Byte 크기의 블록을 할당해 줘야 한다.
VOID	CryptManagerS2S::MakeGarbageBlock( CRYPT_KEY_STREAM IN OUT pCryptKeyStream, DWORD dwGabageKey )
{
	//EXPORTER(CryptManagerS2S::MakeGarbageBlock);

	//ASSERT( pCryptKeyStream != NULL );

	DWORD* pDW = (DWORD*)pCryptKeyStream;

	dwGabageKey += s_MOLA_DDA_INFO.s_dwStackedGarbageKey;
	dwGabageKey = dwGabageKey*0x37660D + 0x3C29F35F;
	*pDW = dwGabageKey;
	pDW++;

	for( int i=1 ; i<(int)((sizeof(BYTE)*200)/sizeof(DWORD)) ; ++i )
	{
		dwGabageKey = dwGabageKey*0x37660D + 0x3C29F35F;
		*pDW = dwGabageKey;
		pDW++;
	}

	s_MOLA_DDA_INFO.s_dwStackedGarbageKey = dwGabageKey;
	return;
}

//-----------------------------------
// <FORMAT>
//
struct Key_Exchange_HSEL
{
	HselInit	encKey;	// 64B
	DWORD		gap;
	HselInit	decKey;
};

// 암호화 정보, 키교환을 위한 스트림 생성,
//  cryptInfo는 MSG_CRYPTOGRAPHY_CMD::m_byCryptBuffer로 그 크기는 200이다.
//  bySymmetrySNKey는 보내고자 하는 SNKey과 쌍을 이루는 키를 입력한다.
// <return>
//   byPairSNKey%(MAX_KEY_SIZE)
BYTE	CryptManagerS2S::CreateCryptKeyStream( BYTE byPairSNKey, CRYPT_KEY_STREAM IN OUT pCryptKeyStream, DWORD dwKey )
{
	//ASSERT( m_pCryptAlgorithm && pCryptKeyStream );

	//MakeGarbageBlock( pCryptKeyStream, dwKey );
	memset( pCryptKeyStream, 0, 200 );

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptAlgorithm;

	// <Waverix><Warning>
	//   단일 키중 가장 큰 값으로 설정, 항상 최대 키값의 크기등을 신경써야 할 것이다.
	DWORD dwStartIndex = (DWORD)( (byPairSNKey%sizeof(HselInit)) + sizeof(DWORD) );

	BYTE* pOffset = NULL;
	do
	{
		union {
			CRYPT_KEY_STREAM	pSource;
			DWORD*				pDestDWORD;
		};
		pSource = pCryptKeyStream;
		*pDestDWORD = dwKey;
		pOffset = (BYTE*)pSource;
	} while(0);

	pOffset += dwStartIndex;

	// 암,복호화 알고리즘 종류와 StartIndex
	*pOffset = (BYTE)( dwStartIndex );
	++pOffset;

	Key_Exchange_HSEL* pExt_HSEL = (Key_Exchange_HSEL*)pOffset;
	pExt_HSEL->encKey = pHSEL->GetEncryptKey();
	pExt_HSEL->decKey = pHSEL->GetDecryptKey();

	do {

		BYTE reverseBuf[200];
		DWORD* order = (DWORD*)pCryptKeyStream;
		DWORD* rever = (DWORD*)&reverseBuf[200];

		for( int i=0 ; i<((200*sizeof(BYTE))/sizeof(DWORD)) ; ++i ) {
			--rever;
			*rever = *order++;
			*rever = BitOp::ConvertDWORD( *rever, BitOp::MASK::c_ALL );
		}
		memcpy( pCryptKeyStream, reverseBuf, 200*sizeof(BYTE) );

	} while(0);

	//*(eCRYPT_T*)pCopy = m_eTypeCrypt;
	// Server Key와 쌍을 이루는 Client Key와의 XOR을 통한 StartIndex
	return (BYTE) ( ((BYTE)dwStartIndex)^byPairSNKey );
}

VOID	CryptManagerS2S::AllocateCryptKey( PVOID encKey, PVOID decKey )
{
	//ASSERT( m_pCryptAlgorithm && encKey && decKey );

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptAlgorithm;

	pHSEL->Init( *(HselInit*)encKey, *(HselInit*)decKey );
}
// 
//  다음은 클라이언트 역할을 수행하는 쪽에서 사용하는 것들...
DWORD	CryptManagerS2S::ExtractUserKeyFromCryptKeyStream( CRYPT_KEY_STREAM IN OUT pCryptKeyStream )
{
	//ASSERT( pCryptKeyStream );

	do {

		BYTE reverseBuf[200];
		DWORD* order = (DWORD*)pCryptKeyStream;
		DWORD* rever = (DWORD*)&reverseBuf[200];
		for( int i=0 ; i<((200*sizeof(BYTE))/sizeof(DWORD)) ; ++i ) {
			--rever;
			*rever = *order++;
			*rever = BitOp::ConvertDWORD( *rever, BitOp::MASK::c_ALL );
		}
		memcpy( pCryptKeyStream, rever, 200*sizeof(BYTE) );

	} while(0);

	return *(DWORD*)pCryptKeyStream;
}

CryptManagerS2S*
CryptManagerS2S::CreateCryptManagerFromCryptKeyStream(
												BYTE byLocalSNKey,
												CRYPT_KEY_STREAM IN pCryptKeyStream,
												BYTE encodedIdx
												)
{
	//ASSERT( pCryptKeyStream );

	CryptHSEL*	pHSEL;

	BYTE* pCopy = (BYTE*)pCryptKeyStream;

	DWORD dwStartIndex = (DWORD) (encodedIdx^byLocalSNKey);
	pCopy += dwStartIndex;

	CryptManagerS2S* pManager = new CryptManagerS2S();
	pManager->Initialize( ROLE_CLIENT );

	++pCopy;

	Key_Exchange_HSEL* pExt_HSEL;

	pHSEL = new CryptHSEL();
	pManager->m_pCryptAlgorithm = (PVOID)pHSEL;
	pExt_HSEL = (Key_Exchange_HSEL*)pCopy;
	pHSEL->Init( pExt_HSEL->encKey, pExt_HSEL->decKey );

	return pManager;
}


