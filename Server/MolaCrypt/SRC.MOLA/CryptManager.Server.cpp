
#include "stdafx.h"
#include "BBTanStream/BBTanStream.h"

#if SERVER_CODE

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include "Macros/GenerateRandomBlock.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

static const int g_cur_crypt_counts = 1;

// =======================================================================================
// 서버 풀 및 난수 블록 유지 블럭

// <MACROs & TYPE DEFINITIION>
const DWORD CRYPTOBJ_DEF_COUNTS	=	3000;
const DWORD CRYPTOBJ_INC_COUNTS	=	1000;

struct _tmp_Alloc {};

inline void* operator new ( size_t size, _tmp_Alloc* pSlot ) { return pSlot; }
inline void operator delete ( void* tmp, _tmp_Alloc* pSlot ) {}

#define new_alloc(type, __allocator) new ((_tmp_Alloc*)__allocator->Alloc()) type
#define free_alloced(ptr, __disposer) delete (_tmp_Alloc*)__allocator->Free(ptr)

//////////////////////////////////////////////////////////////////////////

typedef util::CMemoryPoolFactory<CryptHSEL>	POOL_CRYPT_HSEL;
typedef util::CMemoryPoolFactory<JSCode>		POOL_JS_CODE;


// <MODULE VARIABLEs>
static BBTanStream s_BBTan;
static DWORD s_dwGarbageKey = 0;

static struct sCRYPTPOOLINFO
{
	BOOL								m_bIsServer;
	BOOL								m_bInited;
	DWORD								m_dwCalls;
	POOL_CRYPT_HSEL*					m_pHSELPool;
	util::CMemoryPoolFactory<JSCode>*			m_pJSCode;

	~sCRYPTPOOLINFO()
	{
		SAFE_DELETE( m_pHSELPool );
		SAFE_DELETE( m_pJSCode );
	};
} tagCryptPools = { FALSE, 0, NULL, NULL };


// =======================================================================================

CryptManager::CryptManager()
{
	m_Inited = FALSE;
	m_pCryptMod = NULL;
	m_RefCount = 0;

	m_dwSolarCode = MOLA_RCG_T1_008;
	m_dwLunarCode = MOLA_RCG_T1_009;
}

CryptManager::~CryptManager()
{
	--tagCryptPools.m_dwCalls;

	if( tagCryptPools.m_dwCalls <= 0 )
	{
		if( tagCryptPools.m_pHSELPool )
			delete tagCryptPools.m_pHSELPool;
		//if( tagCryptPools.m_pJSCode )
		//	delete tagCryptPools.m_pJSCode;
	}
	return;
}

VOID	CryptManager::Initialize( BOOL bIsServer, eCRYPT_T eType )
{
	if( bIsServer )
		this->_Initialize( bServerSession, (CryptManager::eCRYPT_T)( eCRYPT_HSEL ) );
}

VOID	CryptManager::Release()
{
	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;

	if( pHSEL == NULL )
		return;

	if( tagCryptPools.m_bIsServer )
	{
		if( tagCryptPools.m_bInited == FALSE )
			return;

		tagCryptPools.m_pHSELPool->Free( pHSEL );
	}
	else
	{
		if( pHSEL->GetInit() )
		{
			pHSEL->SetInit(FALSE);
			delete pHSEL;
			m_pCryptMod = NULL;
		}
		m_pCryptMod = NULL;
	}
}

VOID	CryptManager::_Initialize( BOOL bIsServer, CryptManager::eCRYPT_T eType )
{
	CryptHSEL* pHSEL;

	if( tagCryptPools.m_bInited == FALSE )
	{
		tagCryptPools.m_bIsServer = bServerSession;
		tagCryptPools.m_pHSELPool = new POOL_CRYPT_HSEL;

		//INT iCounts = (INT)(CRYPTOBJ_DEFCOUNTS()/g_cur_crypt_counts);

		tagCryptPools.m_pHSELPool->Initialize( CRYPTOBJ_DEF_COUNTS, CRYPTOBJ_INC_COUNTS );

		tagCryptPools.m_bInited = TRUE;
	}

	if( FALSE == m_Inited )
	{
		pHSEL = new_alloc( CryptHSEL, tagCryptPools.m_pHSELPool );
	}
	else
	{
		ASSERT( m_pCryptMod );
		pHSEL = (CryptHSEL*)m_pCryptMod;
	}

	pHSEL->Create();
	m_pCryptMod = pHSEL;
	m_RefCount = 0;
}

// size 200
BOOL	CryptManager::SetGarbageBlock( BYTE* IN OUT pKeyBlock, DWORD dwGabageKey )
{
	EXPORTER(CryptManager::SetGarbageBlock);
	ASSERT( pKeyBlock != NULL );

	static DWORD sdwRndKEY = GetTickCount();
	DWORD* pDW = (DWORD*)pKeyBlock;

	sdwRndKEY = SHA32_T0(sdwRndKEY)+MOLA_RCG_T1_003;
	s_dwGarbageKey = sdwRndKEY + SHA32_T3(s_dwGarbageKey) + SHA32_T0(dwGabageKey);
	*pDW = s_dwGarbageKey;
	pDW++;

	const INT ciFixedBlkSz = sizeof(BYTE)*MAX_STREAM_KEY_BLOCK;
	const INT ciCntBlkSz = (INT)(ciFixedBlkSz/sizeof(DWORD));

	for( INT i=1 ; i<ciCntBlkSz ; ++i, ++pDW )
	{
		sdwRndKEY = SHA32_T0(sdwRndKEY)+MOLA_RCG_T1_004;
		dwGabageKey = dwGabageKey + SHA32_T3(s_dwGarbageKey);
		s_dwGarbageKey = sdwRndKEY + dwGabageKey;
		*pDW ^= s_dwGarbageKey;
	}

	return TRUE;
}



VOID	CryptManager::SetCryptKey( PVOID encKey, PVOID decKey )
{
	ASSERT( m_pCryptMod != NULL );

	if( m_Inited == FALSE )
		return;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;

	pHSEL->Init( *(HSEL_INITIAL*)encKey, *(HSEL_INITIAL*)decKey );
}

PVOID	CryptManager::GetEncryptionKey()
{
	ASSERT( m_pCryptMod != NULL );

	if( m_Inited == FALSE )
		return NULL;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;

	return (PVOID)&pHSEL->GetEncryptKey();
}

PVOID	CryptManager::GetDecryptionKey()
{
	ASSERT( m_pCryptMod != NULL );

	if( m_Inited == FALSE )
		return NULL;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;

	return (PVOID)&pHSEL->GetDecryptKey();
}


CHAR	CryptManager::GetCRCKey()
{
	ASSERT( m_pCryptMod != NULL );

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

//////////////////////////////////////////////////////////////////////////
// <Cryptography MOLA-1.0.00>
//  패킷 암호화 키 교환 및 주기적 알고리즘 변환
//	PACKET_CG_OPEN( CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_KEY )
//		BYTE						m_bySN;			// Sequence Number
//		BYTE						m_byCryptInfo;	// 인코딩된 패킷 암호화키 블록 풀기 위한 키
//		union {
//			DWORD					m_dwUserKey;
//			BYTE					m_byCryptData[200];	// 알고리즘 정보 (키, 등등...)
//		};
//		BYTE&						GetSNKey() {	return m_bySN;	}
//		VOID						SetSNKey( BYTE bySNKey ) {	m_bySN = bySNKey;	}
//		BYTE&						GetCryptInfo() {	return m_byCryptInfo;	}
//		VOID						SetCryptInfo( BYTE byCryptInfo ) {	m_byCryptInfo = byCryptInfo;	}
//		BYTE*						GetCryptStream() {	return m_byCryptData;	}
//	PACKET_CG_CLOSE
//////////////////////////////////////////////////////////////////////////
// <Cryptography MOLA-1.0.00>
//  변형되는 부분
//		union {
//			DWORD					m_dwUserKey;
//			BYTE					m_byCryptData[200];	// 알고리즘 정보 (키, 등등...)
//		};
//  변형 이후
//		union
//		{
//			struct {
//				BYTE	prevSTREAM[HEX_KEY_1_GENERATOR_008];
//				DWORD	m_dwUserKey;
//			}
//			BYTE	m_byCryptData[256];
//		};
//

static VOID		GetCryptAlgorithmInfo_Prepare(
									BYTE& rbyPairSNKey, BYTE* pCryptInfo, DWORD& rdwRndUserID,
									CryptHSEL* pHSEL, BYTE pCryptStreamTMP[MAX_STREAM_KEY_BLOCK]
									)
{
#if		CODE_SELECTOR_BY_VER == 0
#elif	CODE_SELECTOR_BY_VER == 1
#elif	CODE_SELECTOR_BY_VER == 2
#elif	CODE_SELECTOR_BY_VER == 3
#endif
}

BYTE	CryptManager::GetCryptAlgorithmInfo( BYTE byPairSNKey, BYTE* pCryptInfo, DWORD dwRndUserID )
{
	ASSERT( m_pCryptMod != NULL );
	ASSERT( pCryptInfo != NULL );

	//----------------------------------------------------------------------------------------
	//	uSTREAM_KEY_BLOCK - 준비
	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;
	// <Waverix><Warning>
	//   단일 키중 가장 큰 값으로 설정, 항상 최대 키값의 크기등을 신경써야 할 것이다.
	BYTE pCryptStreamTMP[MAX_STREAM_KEY_BLOCK];
	SetGarbageBlock( pCryptStreamTMP, dwRndUserID );
	uSTREAM_KEY_BLOCK* pUBLOCK = (uSTREAM_KEY_BLOCK*)pCryptStreamTMP;

	//----------------------------------------------------------------------------------------
	//	uSTREAM_KEY_BLOCK - 값 설정
	INT iStartIDX = CRYPT_KEY_INITIAL_GENERATOR_065^byPairSNKey;

	pUBLOCK->m_dwUserKey = dwRndUserID;
	pUBLOCK->m_byStartIDX = iStartIDX;
	pUBLOCK->m_HSEL_ENC_KEY_BLOCK = pHSEL->GetEncryptKey();
	pUBLOCK->m_HSEL_DEC_KEY_BLOCK = pHSEL->GetDecryptKey();

// (WAVERIX_TEST)
#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		DWORD dwWritten;
		OFSTRUCT pOF;
		HANDLE hFILE = (HANDLE)(__int64)OpenFile( ".\\MOLA.TEST\\BinaryServerGenBlock1.txt.bin" , &pOF, OF_CREATE );

		WriteFile( hFILE, pUBLOCK, MAX_STREAM_KEY_BLOCK, &dwWritten, NULL );

		FlushFileBuffers( hFILE );
		CloseHandle( hFILE );
	}
#endif //

	//----------------------------------------------------------------------------------------
	//	uSTREAM_KEY_BLOCK - KEY BLOCK ENCODE
	RandomClassQuery<
		#include "Macros/Loop256_Values.h"
	> RndKeyExchangeProtocolStream;

	const sRANDOM_INDEX_ARRAYs<256>* pStream = RndKeyExchangeProtocolStream.GetRandomValues();
	BYTE* pOffset = (BYTE*)pStream->STREAM;

	for( INT iIDX=0 ; iIDX<MAX_STREAM_KEY_BLOCK ; ++iIDX, ++pOffset )
	{
		BYTE byIDX = *pOffset;
		pCryptInfo[byIDX] = pCryptStreamTMP[iIDX];
	}

	//do {
	//	BYTE reverseBuf[MAX_STREAM_KEY_BLOCK];
	//	DWORD* order = (DWORD*)cryptInfo;
	//	DWORD* rever = (DWORD*)&reverseBuf[MAX_STREAM_KEY_BLOCK];
	//	for( int i=0 ; i<((MAX_STREAM_KEY_BLOCK*sizeof(BYTE))/sizeof(DWORD)) ; ++i ) {
	//		--rever;
	//		*rever = *order++;
	//		*rever = BitOp::ConvertDWORD( *rever, BitOp::MASK::c_ALL );
	//	}
	//	memcpy( cryptInfo, reverseBuf, MAX_STREAM_KEY_BLOCK*sizeof(BYTE) );
	//} while(0);

// (WAVERIX_TEST)
#if LOG_DEBUG_WRITER(__MOLA_CLIENT_DEBUGGING_WRITELOG__)
	{
		DWORD dwWritten;
		OFSTRUCT pOF;
		HANDLE hFILE = (HANDLE)(__int64)OpenFile( ".\\MOLA.TEST\\BinaryServerGenBlock2.txt.bin" , &pOF, OF_CREATE );

		WriteFile( hFILE, pCryptInfo, MAX_STREAM_KEY_BLOCK, &dwWritten, NULL );

		FlushFileBuffers( hFILE );
		CloseHandle( hFILE );
	}
#endif //

	////*(eCRYPT_T*)pCopy = m_eTypeCrypt;
	//// Server Key와 쌍을 이루는 Client Key와의 XOR을 통한 StartIndex
	return (BYTE) ( ((BYTE)CRYPT_KEY_INITIAL_GENERATOR_065)^byPairSNKey );
}

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

	ASSERT( pCryptInfo != NULL );
	if( !pCryptInfo )	return MOLA_BASE_ERR_CODE;

	//do {
	//	BYTE reverseBuf[200];
	//	DWORD* order = (DWORD*)cryptInfo;
	//	DWORD* rever = (DWORD*)&reverseBuf[200];
	//	for( int i=0 ; i<((200*sizeof(BYTE))/sizeof(DWORD)) ; ++i ) {
	//		--rever;
	//		*rever = *order++;
	//		*rever = BitOp::ConvertDWORD( *rever, BitOp::MASK::c_ALL );
	//	}
	//	memcpy( cryptInfo, rever, 200*sizeof(BYTE) );
	//} while(0);
	//return *(DWORD*)cryptInfo;

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

//  <STATIC> 클라 사용 코드구낭...
CryptManager*	CryptManager::SetCryptAlgorithmInfo( BYTE byLocalSNKey, BYTE* pCryptInfo, BYTE encodedIdx )
{
	__DEBUGGING_OUTPUT1( "CryptManager::SetCryptAlgorithmInfo", byLocalSNKey );

	ASSERT( pCryptInfo != NULL );

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
		HANDLE hFILE = (HANDLE)(__int64)OpenFile( ".\\MOLA.TEST\\BinaryClientGenBlock2.txt.bin" , &pOF, OF_CREATE );

		WriteFile( hFILE, pCryptInfo, MAX_STREAM_KEY_BLOCK, &dwWritten, NULL );

		FlushFileBuffers( hFILE );
		CloseHandle( hFILE );
	}
#endif //

	return pManager;
}

//////////////////////////////////////////////////////////////////////////
//
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

//
//////////////////////////////////////////////////////////////////////////

BOOL	CryptManager::Encrypt( BYTE* IN OUT pSTREAM, int iSize, BYTE& OUT rbyCRC )
{
	__DEBUGGING_OUTPUT1( "CryptManager::Encrypt", iSize );
	//ASSERT( m_pCryptMod != NULL );
	if( m_Inited == FALSE )
		return FALSE;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;
	//IncRefCount( CryptManager::dwUSE_ONE, m_RefCount );
	if( pHSEL && pHSEL->Encrypt( pSTREAM, iSize ) )
	{
		m_byCRC = (BYTE)pHSEL->GetEnCRCConvertChar();
		rbyCRC = m_byCRC;

		uRND_CONVKEY& rCONV = DWORD2uRND(m_dwSolarCode);
		rCONV.KEYBLOCK = MAKE_RANDOM32_KEY_EXPOSED( MOLA_RCG_T1_010, MOLA_RCG_T1_011, MOLA_RCG_T1_012, rCONV.KEYBLOCK );
		*(WORD*)pSTREAM ^= rCONV.WORDBLOCK[0];

		BYTE byLinkedNo = pSTREAM[1];
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
	if( m_Inited == FALSE )
		return FALSE;

	CryptHSEL* pHSEL = (CryptHSEL*)m_pCryptMod;
	//IncRefCount( CryptManager::dwUSE_TWO, m_RefCount );

	uRND_CONVKEY& rCONV = DWORD2uRND(m_dwLunarCode);
	rCONV.KEYBLOCK = MAKE_RANDOM32_KEY_EXPOSED( MOLA_RCG_T1_013, MOLA_RCG_T1_014, MOLA_RCG_T1_015, rCONV.KEYBLOCK );
	uRND_CONVKEY mCONV = rCONV;

	BYTE byLinkedNo = pSTREAM[1];
	__DEBUGGING_OUTPUT2( "CryptManager::Decode1", pSTREAM[2], pSTREAM[3] );
	s_BBTan.Decode( rCONV, pSTREAM, (WORD)iSize, byLinkedNo );
	*(WORD*)pSTREAM ^= mCONV.WORDBLOCK[0];
	__DEBUGGING_OUTPUT2( "CryptManager::Decode2", pSTREAM[2], pSTREAM[3] );

	if( pHSEL && pHSEL->Decrypt( pSTREAM, iSize ) )
	{
		if( byCRC == (BYTE)pHSEL->GetDeCRCConvertChar() )
		{
			return TRUE;
		}
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BOOL	CryptManager::Init1st()
{
	return InitInfo( NULL );
}

BOOL	CryptManager::InitInfo( PVOID arg )
{
#ifdef __MOLA_CLIENT_DEBUGGING__
	do
	{
		OFSTRUCT pOF;
		shFILE = (HANDLE)(__int64)OpenFile( ".\\MOLA.TEST\\Debug.txt" , &pOF, OF_CREATE );
		__DEBUGGING_OUTPUT1( "CryptManager::InitInfo", (DWORD)(__int64)arg );
		__DEBUGGING_OUTPUT2( "CryptManager::InitInfo", CATEGORY_ODD_NUMBER, PROTOCOL_ODD_NUMBER );
	} while(0);
#endif

	RandomClassQuery<
		#include "Macros/Loop256_Values.h"
	> rndstream;

	const sRANDOM_INDEX_ARRAYs<256>* pStream = rndstream.GetRandomValues();
	BYTE* pOff = (BYTE*)pStream->STREAM;
	for( INT i=0 ; i<256 ; ++i, ++pOff )
	{
		BYTE byVal = *pOff;
		//byVal = (BYTE)~byVal - 1;
		ms_DecryptStream[i] = byVal;
	}

	s_BBTan.Init();

	return TRUE;
}

//
//
//
//
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


#endif //SERVER_CODE

