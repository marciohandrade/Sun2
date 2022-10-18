//=============================================================================================================================
/// CryptManagerS2S class
/**
	@author	Lee Eun-pa <waverix@webzen.co.kr>
	@since	2006. 10. 12
	@remark
		- ������ ���� ���̿� ���ǰ� �Ǵ� ��Ŷ ��ȣȭ ó�� ������
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


VOID	CryptManagerS2S::Initialize( eROLE_SIDE role /*�߰����� �ִ��� Ȯ���� ��*/ )
{
	if( m_Inited )
	{
		//ASSERT( !"�ѹ� �̻� �ʱ�ȭ?" );
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

	//ASSERT( !"�־�� �ȵǴ� ���" );
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

// 200 Byte ũ���� ����� �Ҵ��� ��� �Ѵ�.
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

// ��ȣȭ ����, Ű��ȯ�� ���� ��Ʈ�� ����,
//  cryptInfo�� MSG_CRYPTOGRAPHY_CMD::m_byCryptBuffer�� �� ũ��� 200�̴�.
//  bySymmetrySNKey�� �������� �ϴ� SNKey�� ���� �̷�� Ű�� �Է��Ѵ�.
// <return>
//   byPairSNKey%(MAX_KEY_SIZE)
BYTE	CryptManagerS2S::CreateCryptKeyStream( BYTE byPairSNKey, CRYPT_KEY_STREAM IN OUT pCryptKeyStream, DWORD dwKey )
{
	//ASSERT( m_pCryptAlgorithm && pCryptKeyStream );

	//MakeGarbageBlock( pCryptKeyStream, dwKey );
	memset( pCryptKeyStream, 0, 200 );

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptAlgorithm;

	// <Waverix><Warning>
	//   ���� Ű�� ���� ū ������ ����, �׻� �ִ� Ű���� ũ����� �Ű��� �� ���̴�.
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

	// ��,��ȣȭ �˰��� ������ StartIndex
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
	// Server Key�� ���� �̷�� Client Key���� XOR�� ���� StartIndex
	return (BYTE) ( ((BYTE)dwStartIndex)^byPairSNKey );
}

VOID	CryptManagerS2S::AllocateCryptKey( PVOID encKey, PVOID decKey )
{
	//ASSERT( m_pCryptAlgorithm && encKey && decKey );

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptAlgorithm;

	pHSEL->Init( *(HselInit*)encKey, *(HselInit*)decKey );
}
// 
//  ������ Ŭ���̾�Ʈ ������ �����ϴ� �ʿ��� ����ϴ� �͵�...
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


