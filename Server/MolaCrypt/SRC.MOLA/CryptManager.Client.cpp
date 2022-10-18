#include "stdafx.h"

#include "BBTanStream/BBTanStream.h"

#if CLIENT_CODE
// =======================================================================================

//////////////////////////////////////////////////////////////////////////
// <MODULE VARIABLEs>
//static BYTE*	
//////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#define __MOLA_CLIENT_CODES__

#include "Macros/GenerateRandomBlock.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static BBTanStream s_BBTan;

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL	CryptManager::IsCanDelete()
{
	if( dwUSE_MASK == m_RefCount )
		return FALSE;

	return TRUE;
}

VOID	CryptManager::DecRefCount()
{
	m_RefCount &= ~dwUSE_ONE;
}

inline VOID		IncRefCount( DWORD dwMode, DWORD& rdwRefCount )
{
	if( CryptManager::dwUSE_ONE == dwMode
		&& !(CryptManager::dwUSE_ONE & rdwRefCount) )
		rdwRefCount |= CryptManager::dwUSE_ONE;
	else if( CryptManager::dwUSE_TWO == dwMode
		&& !(CryptManager::dwUSE_TWO & rdwRefCount) )
		rdwRefCount |= CryptManager::dwUSE_TWO;
}


CryptManager::CryptManager()
{
	__DEBUGGING_OUTPUT0( "CryptManager::CryptManager" );
	m_Inited = FALSE;
	m_pCryptMod = NULL;
	m_RefCount = 0;
	IncRefCount( CryptManager::dwUSE_ONE, m_RefCount );
	IncRefCount( CryptManager::dwUSE_TWO, m_RefCount );

	m_dwSolarCode = MOLA_RCG_T1_008;
	m_dwLunarCode = MOLA_RCG_T1_009;
}

CryptManager::~CryptManager()
{
	__DEBUGGING_OUTPUT0( "CryptManager::~CryptManager" );
	// use macro SAFE_DELETE
	if( m_pCryptMod )
		delete m_pCryptMod;
	m_pCryptMod = NULL;

}

VOID	CryptManager::Initialize( BOOL bIsServer, eCRYPT_T eType )
{
	__DEBUGGING_OUTPUT1( "CryptManager::Initialize", bIsServer<<8 | eType );
}

VOID	CryptManager::Release()
{
	__DEBUGGING_OUTPUT0( "CryptManager::Release" );

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;

	//if( pHSEL )
	//{
	//	if( pHSEL->GetInit() )
	//	{
	//		pHSEL->SetInit(FALSE);
	//		delete pHSEL;
	//		m_pCryptMod = NULL;
	//	}
	//	m_pCryptMod = NULL;
	//}
}

CHAR	CryptManager::GetCRCKey()
{
	__DEBUGGING_OUTPUT0( "CryptManager::GetCRCKey" );

	//ASSERT( m_pCryptMod != NULL );
	if( !m_pCryptMod )
		return -1;

	return m_byCRC;
}


//-----------------------------------
// <FORMAT>
//
struct Key_Exchange_HSEL
{
	HSEL_INITIAL	encKey;	// 64B
	DWORD		gap;
	HSEL_INITIAL	decKey;
};

//struct Key_Exchange_JSCode {
//	BYTE		encKey;
//	BYTE		gap[iStartIndex];
//	BYTE		decKey;
//};
//-----------------------------------

//  다음은 클라이언트에서만 사용할 것들...
//  소멸자도 재정의를 할 필요가 있다.

//struct SCryptInfoReverse {
//	DWORD	dw0;
//	DWORD	dw1;
//	DWORD	dw2;
//	DWORD	dw3;
//	DWORD	dw4;
//};
//union UCryptInfoReverse {
//	BYTE reverseBuf[200];
//	DWORD dwBuffer[200/sizeof(DWORD)];
//	SCryptInfoReverse tagBuffer[200/(sizeof(DWORD)*5)];
//};

DWORD			CryptManager::GetUserKeyInCryptInfo( BYTE* pCryptInfo )
{
	__DEBUGGING_OUTPUT1( "CryptManager::GetUserKeyInCryptInfo", (DWORD)(__int64)pCryptInfo );
	//ASSERT( pCryptInfo != NULL );
	if( !pCryptInfo )	return MOLA_BASE_ERR_CODE;

#if CODE_BACKUP
	do {
		BYTE reverseBuf[200];
		DWORD* order = (DWORD*)cryptInfo;
		DWORD* rever = (DWORD*)&reverseBuf[200];
		for( int i=0 ; i<((200*sizeof(BYTE))/sizeof(DWORD)) ; ++i ) {
			--rever;
			*rever = *order++;
			*rever = BitOp::ConvertDWORD( *rever, BitOp::MASK::c_ALL );
		}
		memcpy( cryptInfo, rever, 200*sizeof(BYTE) );
	} while(0);
	return *(DWORD*)cryptInfo;
#endif

	BYTE tempCryptStream[MAX_STREAM_KEY_BLOCK];
	uSTREAM_KEY_BLOCK* pUBLOCK = (uSTREAM_KEY_BLOCK*)tempCryptStream;

	BYTE* pOffset = (BYTE*)ms_DecryptStream;

	for( INT iIDX=0 ; iIDX<MAX_STREAM_KEY_BLOCK ; ++iIDX, ++pOffset )
	{
		BYTE byIDX = *pOffset;
		BYTE byVAL = pCryptInfo[byIDX];
		tempCryptStream[iIDX] = byVAL;
	}
	memcpy( pCryptInfo, tempCryptStream, MAX_STREAM_KEY_BLOCK );

#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		DWORD dwWritten;
		OFSTRUCT pOF;
		HANDLE hFILE = (HANDLE)(__int64)OpenFile( ".\\MOLA.TEST\\BinaryClientGenBlock1.txt.bin" , &pOF, OF_CREATE );

		WriteFile( hFILE, tempCryptStream, MAX_STREAM_KEY_BLOCK, &dwWritten, NULL );

		FlushFileBuffers( hFILE );
		CloseHandle( hFILE );
	}
#endif //

	return pUBLOCK->m_dwUserKey;
}

//  <STATIC>
CryptManager*	CryptManager::SetCryptAlgorithmInfo( BYTE byLocalSNKey, BYTE* pCryptInfo, BYTE encodedIdx )
{
	__DEBUGGING_OUTPUT1( "CryptManager::SetCryptAlgorithmInfo", byLocalSNKey );
	//ASSERT( pCryptInfo != NULL );
	if( !pCryptInfo )
		return NULL;

	CryptManager* pManager = new CryptManager();
	pManager->Initialize( bClientSession, CryptManager::eCRYPT_HSEL );

	uSTREAM_KEY_BLOCK* pUBLOCK = (uSTREAM_KEY_BLOCK*)pCryptInfo;

	CryptHSEL* pHSEL = new CryptHSEL();
	pManager->m_pCryptMod = pHSEL;
	pHSEL->Init( pUBLOCK->m_HSEL_ENC_KEY_BLOCK, pUBLOCK->m_HSEL_DEC_KEY_BLOCK );

#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		DWORD dwWritten;
		OFSTRUCT pOF;
		HANDLE hFILE = (HANDLE)(__int64)OpenFile( ".\\MOLA.TEST\\BinaryClientGenBlock1.txt.bin" , &pOF, OF_CREATE );

		WriteFile( hFILE, pCryptInfo, MAX_STREAM_KEY_BLOCK, &dwWritten, NULL );

		FlushFileBuffers( hFILE );
		CloseHandle( hFILE );
	}
#endif //

	return pManager;
}

BOOL	CryptManager::Encrypt( BYTE* IN OUT pSTREAM, int iSize, BYTE& OUT rbyCRC )
{
	__DEBUGGING_OUTPUT1( "CryptManager::Encrypt", iSize );
	//ASSERT( m_pCryptMod != NULL );
	if( !(m_pCryptMod && m_Inited) )
		return FALSE;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;
	IncRefCount( CryptManager::dwUSE_ONE, m_RefCount );
	if( pHSEL && pHSEL->Encrypt( pSTREAM, iSize ) )
	{
		m_byCRC = (BYTE)pHSEL->GetEnCRCConvertChar();
		rbyCRC = m_byCRC;

		uRND_CONVKEY& rCONV = DWORD2uRND(m_dwLunarCode);
		rCONV.KEYBLOCK = MAKE_RANDOM32_KEY_EXPOSED( MOLA_RCG_T1_013, MOLA_RCG_T1_014, MOLA_RCG_T1_015, rCONV.KEYBLOCK );
		*(WORD*)pSTREAM ^= rCONV.WORDBLOCK[0];

		BYTE byLinkedNo = (pSTREAM)[1];

		__DEBUGGING_OUTPUT2( "CryptManager::Encode1", pSTREAM[2], pSTREAM[3] );
		s_BBTan.Encode( rCONV, pSTREAM, (WORD)iSize, byLinkedNo );
		__DEBUGGING_OUTPUT2( "CryptManager::Encode2", pSTREAM[2], pSTREAM[3] );

		return TRUE;
	}
	return FALSE;
}

BOOL	CryptManager::Decrypt( BYTE* IN OUT pSTREAM, int iSize, BYTE IN byCRC )
{
	__DEBUGGING_OUTPUT1( "CryptManager::Decrypt", iSize );
	//ASSERT( m_pCryptMod != NULL );
	if( !(m_pCryptMod && m_Inited) )
		return FALSE;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;
	IncRefCount( CryptManager::dwUSE_TWO, m_RefCount );

	uRND_CONVKEY& rCONV = DWORD2uRND(m_dwSolarCode);
	rCONV.KEYBLOCK = MAKE_RANDOM32_KEY_EXPOSED( MOLA_RCG_T1_010, MOLA_RCG_T1_011, MOLA_RCG_T1_012, rCONV.KEYBLOCK );
	uRND_CONVKEY mCONV = rCONV;

	BYTE byLinkedNo = pSTREAM[1];
	__DEBUGGING_OUTPUT2( "CryptManager::Decode1", pSTREAM[2], pSTREAM[3] );
	s_BBTan.Decode( rCONV, pSTREAM, (WORD)iSize, byLinkedNo );
	*(WORD*)pSTREAM ^= mCONV.WORDBLOCK[0];
	__DEBUGGING_OUTPUT2( "CryptManager::Decode2", pSTREAM[2], pSTREAM[3] );

	if( pHSEL && pHSEL->Decrypt( pSTREAM, iSize ) )
	{
		__DEBUGGING_OUTPUT2( "CryptManager::Decrypt", ((MSG_BASE*)pSTREAM)->m_byCategory, ((MSG_BASE*)pSTREAM)->m_byProtocol );
		return TRUE;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL	CryptManager::Init1st()
{
	return TRUE;
}

BOOL	CryptManager::InitInfo( PVOID arg )
{
#ifdef __MOLA_CLIENT_DEBUGGING__
	do
	{
		OFSTRUCT pOF;
		shFILE = (HANDLE)(__int64)OpenFile( ".\\MOLA.TEST\\Debug.txt" , &pOF, OF_CREATE );
		__DEBUGGING_OUTPUT1( "CryptManager::InitInfo", (DWORD)(LONGLONG)arg );
		__DEBUGGING_OUTPUT2( "CryptManager::InitInfo", CATEGORY_ODD_NUMBER, PROTOCOL_ODD_NUMBER );
	} while(0);
#endif

	struct
	{
		BYTE	dwRandomNos[256];
		CHAR	msgFormat[256];
	} TEMP_INFO;

	memset( &TEMP_INFO, 0, sizeof(TEMP_INFO) );

	RandomClassDef<
		#include "Macros/Loop256_ValuesComplement.h"
	> rndstream;

	sRANDOM_INDEX_ARRAYs<256>* pComplementStream = rndstream.GetRandomValues();
	BYTE* pOff = pComplementStream->STREAM;
	for( INT i=0 ; i<256 ; ++i, ++pOff )
	{
		BYTE byVal = *pOff;
		byVal = (BYTE)~byVal - 1;
		TEMP_INFO.dwRandomNos[i] = byVal;
	}

	memcpy( ms_DecryptStream, TEMP_INFO.dwRandomNos, 256 );



// (WAVERIX_TEST) 테스트 코드
#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		OFSTRUCT pOF;
		HANDLE hFILE = (HANDLE)(__int64)OpenFile( ".\\MOLA.TEST\\Binary.txt" , &pOF, OF_CREATE );

		RandomClassDef<
			#include "Macros/Loop256_ValuesComplement.h"
		> test_def;

		sRANDOM_INDEX_ARRAYs<256>* pComplementStream = test_def.GetRandomValues();
		BYTE* pOff = pComplementStream->STREAM;
		for( INT i=0 ; i<256 ; ++i, ++pOff )
		{
			BYTE byVal = *pOff;
			byVal = (BYTE)~byVal - 1;
			TEMP_INFO.dwRandomNos[i] = byVal;
		}

		DWORD dwWritten;

		dwWritten = _snprintf( TEMP_INFO.msgFormat, sizeof(TEMP_INFO.msgFormat),
			"\r\nVERSION : (%d.%d.%d)\r\nSTACK_ADDRESS %08X\tSIZE %u\r\n",
			(VERSION_CODE>>24)&0xFF, (VERSION_CODE>>16)&0xFF, (VERSION_CODE&0xFFFF),
			(DWORD)(__int64)&TEMP_INFO, sizeof(uSTREAM_KEY_BLOCK) );
		// VERSION
		WriteFile( hFILE, TEMP_INFO.msgFormat, 256, &dwWritten, NULL );
		WriteFile( hFILE, test_def.GetRandomValues(), 256, &dwWritten, NULL );
		BYTE tmpStrem[256] = { 0, };
		WriteFile( hFILE,tmpStrem, 256, &dwWritten, NULL );
		WriteFile( hFILE, TEMP_INFO.dwRandomNos, 256, &dwWritten, NULL );

		FlushFileBuffers( hFILE );
		CloseHandle( hFILE );
	}
#endif //

	memset( &TEMP_INFO, 0, sizeof(TEMP_INFO) );

	s_BBTan.Init();

	return TRUE;
}

#endif //CLIENT_CODE
