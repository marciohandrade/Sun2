#include "stdafx.h"
#include "MOLA.SharedMemoryPool.h"

#if SUN_CODE_BACKUP
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#define BUF_ALIGN_008( __size )			(((__size)+((8)-1))&(~((8)-1)))
#define BUF_ALIGN_008_BY_IDX( __idx )	( (__idx)<<3 )
#define BUF_008_IDX_BY_SIZE( __size )	( (__size)>>3 )

#define BUF_ALIGN_016( __size )			(((__size)+((16)-1))&(~((16)-1)))
#define BUF_ALIGN_016_BY_IDX( __idx )	( (__idx)<<4 )
#define BUF_016_IDX_BY_SIZE( __size )	( (__size)>>4 )

#define BUF_ALIGN_032( __size )			(((__size)+((32)-1))&(~((32)-1)))
#define BUF_ALIGN_032_BY_IDX( __idx )	( (__idx)<<5 )
#define BUF_032_IDX_BY_SIZE( __size )	( (__size)>>5 )

static const DWORD MAGIC_CODE_ALLOC			= ('A'<<24);
static const DWORD MAGIC_CODE_FREE			= ('F'<<24);
static const DWORD	MAGIC_CODE_HDR_ALLOC	= ('S'<<16) | MAGIC_CODE_ALLOC;
static const DWORD	MAGIC_CODE_HDR_FREE		= ('S'<<16) | MAGIC_CODE_FREE;
static const DWORD	MAGIC_CODE_HDR_MASK		= 0xFFFF0000;
static const DWORD	MAGIC_CODE_SZ_MASK		= 0x0000FFFF;

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
class MOLAISharedMemory
{
public:		virtual ~MOLAISharedMemory()	{ Release(); }
			virtual BOOL				Initialize( DWORD dwInitSize = 0, DWORD dwGrowth = 100 ) = 0;
			virtual VOID				Release() {};
			//virtual BOOL				IsElement( PVOID ptr ) = 0;
			//virtual BOOL				IsAllocated( PVOID ptr ) = 0;
			virtual PVOID				Alloc() = 0;
			virtual VOID				Free( PVOID ptr ) = 0;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
#pragma pack(push, 1)
template<typename _TYPE>
struct SHARED_POOL_VERIFIER
{
	DWORD	VERIFIER;
	_TYPE*	pNext;
};

struct /*__declspec(align(4))*/ sNODE_GENERNAL
{
	SHARED_POOL_VERIFIER<BYTE>	HEADER;
	DWORD						TYPE[1];
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
static DWORD			GetSizeN_VerifyBySharedPool( PVOID ptr )
{
	DWORD dwSize = 0;
	sNODE_GENERNAL* pGENNODE = (sNODE_GENERNAL*)ptr;

	if( pGENNODE )
	{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_SHMEM__)
		printf( "offsetof %u\n", offsetof(sNODE_GENERNAL, TYPE) );
#endif
		pGENNODE = (sNODE_GENERNAL*)( ((BYTE*)pGENNODE) - offsetof(sNODE_GENERNAL, TYPE)  );

		DWORD dwMASKING = MAGIC_CODE_HDR_MASK&pGENNODE->HEADER.VERIFIER;
		if( MAGIC_CODE_HDR_ALLOC == dwMASKING || MAGIC_CODE_HDR_FREE == dwMASKING )
		{
			dwSize = MAGIC_CODE_SZ_MASK&pGENNODE->HEADER.VERIFIER;
		}
	}

	return dwSize;
}
#pragma pack(pop)

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// <MOLASharedMemoryBandwith>
template<typename WORD _SZ>
class MOLASharedMemoryBandwith : public MOLAISharedMemory
{
private:	static const DWORD	MAGIC_CODE_ALLOC		= MAGIC_CODE_HDR_ALLOC | (_SZ);
			static const DWORD	MAGIC_CODE_FREE			= MAGIC_CODE_HDR_FREE | (_SZ);

			struct sNODE;
			typedef SHARED_POOL_VERIFIER<sNODE>	SHARED_POOL_VERIFIER_D;
private:	struct __declspec(align(4)) sNODE
			{
#			pragma pack(push, 1)
				SHARED_POOL_VERIFIER_D	HEADER;
				BYTE					TYPE[_SZ];
#			pragma pack(pop)

				sNODE() { HEADER.VERIFIER = MAGIC_CODE_FREE;}
				inline BYTE*	TypePtr() { return TYPE; }
				// CanAllocable 은 항상 성공해야 한다.
				inline BOOL		CanAllocable() { return (BOOL)(MAGIC_CODE_FREE == HEADER.VERIFIER); }
				inline BOOL		CanDeAllocable( SHARED_POOL_VERIFIER_D* pROOT )
				{ return (BOOL)( pROOT->VERIFIER == HEADER.VERIFIER ); }

				inline VOID		SetAlloced()
				{ HEADER.VERIFIER = MAGIC_CODE_ALLOC; }
				inline VOID		SetFreed()
				{ HEADER.VERIFIER = MAGIC_CODE_FREE; }

				inline static sNODE*	GetHeader( PVOID pNODE )
				{ return (sNODE*)( ((BYTE*)pNODE) - sizeof(SHARED_POOL_VERIFIER_D) ); }

				inline static VOID		InitRef( SHARED_POOL_VERIFIER_D* pROOT )
				{ pROOT->VERIFIER = MAGIC_CODE_ALLOC; }

				//inline static DWORD		GetSize( SHARED_POOL_VERIFIER_D* pROOT, sNODE* pNODE )
				//{
				//	DWORD dwNodeSize = 0;
				//	if( (pNODE->HEADER.m_dwCHECK == pROOT->m_dwCHECK)		//FreeNode일 경우
				//		|| ( pNODE->HEADER.m_pCHECK == pROOT->m_pdwCHECK )	// AllocedNode일 경우
				//		)
				//	{
				//		dwNodeSize = MAGIC_CODE_SZ_MASK & pROOT->m_dwCHECK;
				//	}
				//	return dwNodeSize;
				//}
			};

#		pragma pack(push, 4)
			struct sNODE_BLOCK
			{
				sNODE_BLOCK*	pNext;
				sNODE*			pNodeEND;
#			pragma warning(push)
#			pragma warning(disable:4200)
				sNODE			pNODE_HDR[0];
#			pragma warning(pop)
			};
#		pragma pack(pop)

			typedef MOLASharedMemoryBandwith<_SZ>		MEMORY_BANDWITH;

public:		MOLASharedMemoryBandwith( DWORD dwInitSize= 0, DWORD dwGrowth = 100 ) : m_pBlockRoot(NULL), m_pFreeNode(NULL), m_dwGrowthCount(0)
			{
				sNODE::InitRef( &m_ROOT );
				Initialize( dwInitSize, dwGrowth );
			}
			~MOLASharedMemoryBandwith()
			{
				Release();
			}

			virtual BOOL		Initialize( DWORD dwInitSize = 0, DWORD dwGrowth = 100 )
			{
				m_dwGrowthCount = dwGrowth;
				if( dwInitSize )
					_AddBlock( dwInitSize );

				return TRUE;
			}

			virtual VOID		Release()
			{
				sNODE_BLOCK* pBlock = m_pBlockRoot;
				while( pBlock != NULL )
				{
					sNODE_BLOCK* pBlockN = pBlock->pNext;
					free( pBlock );
					pBlock = pBlockN;
				}

				m_pFreeNode = NULL;
				m_pBlockRoot = NULL;
			}

			//virtual BOOL		IsElement( PVOID ptr );
			//virtual BOOL		IsAllocated( PVOID ptr );
			virtual PVOID		Alloc()
			{
				sNODE* pAllocNode = m_pFreeNode;

				if( pAllocNode == NULL )
				{
					_AddBlock( m_dwGrowthCount );
					pAllocNode = m_pFreeNode;
					if( !pAllocNode )
						return NULL;
				}

				m_pFreeNode = m_pFreeNode->HEADER.pNext;

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_SHMEM__)
				printf( "B %p\tHDR %p\n", pAllocNode, pAllocNode->TYPE );
#endif
				pAllocNode->SetAlloced();
				return pAllocNode->TYPE;
			}

			virtual VOID		Free( PVOID ptr )
			{
				sNODE* pNode = (sNODE*)ptr;
				pNode = pNode->GetHeader(pNode);
				if( pNode->CanDeAllocable( &m_ROOT ) )
				{
					pNode->SetFreed();
					pNode->HEADER.pNext = m_pFreeNode;
					m_pFreeNode = pNode;

					return;
				}

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_SHMEM__)
				printf( "Can't DeAllocatable\n" );
#endif

#if CODE_TEST(__MOLA_RESOURCE_TEST__)
				FORCED_INTERRUPT_INMOLA();
#else
				DEBUGGING_INTERRUPT_INMOLA();
#endif
			}


private:	VOID				_AddBlock( DWORD dwIncCount )
			{
#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_SHMEM__)
				printf( "sizeof(sNODE_BLOCK) %u\tsizeof(sNODE) %u\n", sizeof(sNODE_BLOCK), sizeof(sNODE) );
#endif

				sNODE_BLOCK* pBlock = (sNODE_BLOCK*)malloc( sizeof(BYTE)*(sizeof(sNODE_BLOCK) + sizeof(sNODE)*dwIncCount) );
				if( !pBlock )
					return;

				sNODE** ppPrev = &m_pFreeNode;
				sNODE* pNode = pBlock->pNODE_HDR;
				sNODE* pNodeE = (sNODE*)( ((BYTE*)pBlock->pNODE_HDR) + sizeof(sNODE)*dwIncCount );
				while( pNode < pNodeE )
				{
					*ppPrev = new (pNode) sNODE;
					ppPrev = &pNode->HEADER.pNext;

					pNode = (sNODE*)( ((BYTE*)pNode) + sizeof(sNODE) );
				}

				*ppPrev = NULL;
				pBlock->pNodeEND = pNode;
				pBlock->pNext = m_pBlockRoot;
				m_pBlockRoot = pBlock;

			}
private:	sNODE_BLOCK*			m_pBlockRoot;
			sNODE*					m_pFreeNode;
			SHARED_POOL_VERIFIER_D	m_ROOT;
			DWORD					m_dwGrowthCount;
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
struct MOLASMEMINFO
{
	enum eIDX
	{
		SIZE_008		= 1,
		SIZE_016		,
		SIZE_024		,
		SIZE_032		,
		SIZE_040		,
		SIZE_048		,
		SIZE_056		,
		SIZE_064		,
		SIZE_072		,
		SIZE_080		,
		SIZE_088		,
		SIZE_096		,
		SIZE_104		,
		SIZE_112		,
		SIZE_120		,
		SIZE_128		,
		SIZE_008_MAX	,	// 17
		SIZE_144		= SIZE_008_MAX,	// 17
		SIZE_160		,
		SIZE_176		,
		SIZE_192		,
		SIZE_208		,
		SIZE_224		,
		SIZE_240		,
		SIZE_256		,
		SIZE_016_MAX	,	// 25
		SIZE_288		= SIZE_016_MAX,	// 25
		SIZE_320		,
		SIZE_352		,
		SIZE_384		,
		SIZE_416		,
		SIZE_448		,
		SIZE_480		,
		SIZE_512		,
		SIZE_032_MAX	,
	};

	//enum eSIZE
	//{
	//	SIZE_008		= BUF_ALIGN_008_BY_IDX(1),
	//	SIZE_016		= BUF_ALIGN_008_BY_IDX(2),
	//	SIZE_024		= BUF_ALIGN_008_BY_IDX(3),
	//	SIZE_032		= BUF_ALIGN_008_BY_IDX(4),
	//	SIZE_040		= BUF_ALIGN_008_BY_IDX(5),
	//	SIZE_048		= BUF_ALIGN_008_BY_IDX(6),
	//	SIZE_056		= BUF_ALIGN_008_BY_IDX(7),
	//	SIZE_064		= BUF_ALIGN_008_BY_IDX(8),
	//	SIZE_072		= BUF_ALIGN_008_BY_IDX(9),
	//	SIZE_080		= BUF_ALIGN_008_BY_IDX(10),
	//	SIZE_088		= BUF_ALIGN_008_BY_IDX(11),
	//	SIZE_096		= BUF_ALIGN_008_BY_IDX(12),
	//	SIZE_104		= BUF_ALIGN_008_BY_IDX(13),
	//	SIZE_112		= BUF_ALIGN_008_BY_IDX(14),
	//	SIZE_120		= BUF_ALIGN_008_BY_IDX(15),
	//	SIZE_128		= BUF_ALIGN_008_BY_IDX(16),
	//	SIZE_008_MAX		= BUF_ALIGN_008_BY_IDX(17),
	//};

	static const DWORD		MAX_SHARED_MEMORY_008_SIZE	= 128;	// 8x
	static const DWORD		MAX_SHARED_MEMORY_016_SIZE	= 256;	// 16x
	static const DWORD		MAX_SHARED_MEMORY_032_SIZE	= 512;	// 32x

	//static BYTE				IDXofSIZE[MAX_SHARED_MEMORY_008_SIZE];
	static WORD				SIZEofIDX[SIZE_032_MAX];

	static VOID				Initialize();
};

class MOLASharedMemoryRouter
{
public:		MOLASharedMemoryRouter();
			~MOLASharedMemoryRouter();
			inline PVOID		Alloc( SIZE_T sz )
			{
				//sz = sizeof(SHARED_POOL_VERIFIER<BYTE>)+sz;

				if( MOLASMEMINFO::MAX_SHARED_MEMORY_032_SIZE < sz )
					return malloc( sz );

				DWORD dwAlignSize;
				DWORD dwIdx;
				MOLAISharedMemory* pMEMORY;

				if( MOLASMEMINFO::MAX_SHARED_MEMORY_016_SIZE < sz )
				{	// 32x	[SIZE_288=25], 288=9~
					dwAlignSize = (DWORD)BUF_ALIGN_032(sz);
					dwIdx =  BUF_032_IDX_BY_SIZE(dwAlignSize)+16;
					
				}
				else if( MOLASMEMINFO::MAX_SHARED_MEMORY_008_SIZE < sz )
				{	// 16x - [SIZE_144=17], 144=9~
					dwAlignSize = (DWORD)BUF_ALIGN_016(sz);
					dwIdx =  BUF_016_IDX_BY_SIZE(dwAlignSize)+8;
				}
				else
				{	// 8x
					dwAlignSize = (DWORD)BUF_ALIGN_008(sz);
					dwIdx =  BUF_008_IDX_BY_SIZE(dwAlignSize);
				}

				pMEMORY = ROUTER[dwIdx];
				if( dwIdx && pMEMORY )
					return pMEMORY->Alloc();

				return NULL;
			}

			inline VOID			Free( PVOID ptr )
			{
				DWORD dwSize = GetSizeN_VerifyBySharedPool( ptr );
				if( dwSize /*|| 512보다 크면... 그 이후 관리도 감안해서...*/)
				{
					DWORD dwIdx;
					MOLAISharedMemory* pMEMORY;

					if( MOLASMEMINFO::MAX_SHARED_MEMORY_016_SIZE < dwSize )
						dwIdx =  BUF_032_IDX_BY_SIZE(dwSize)+16;
					else if( MOLASMEMINFO::MAX_SHARED_MEMORY_008_SIZE < dwSize )
						dwIdx =  BUF_016_IDX_BY_SIZE(dwSize)+8;
					else
						dwIdx =  BUF_008_IDX_BY_SIZE(dwSize);

					pMEMORY = ROUTER[dwIdx];
					if( dwIdx && pMEMORY )
						return pMEMORY->Free( ptr );
				}

				//__asm { int 3 };
				free( ptr );
			}

			MOLAISharedMemory*	ROUTER[MOLASMEMINFO::SIZE_032_MAX];
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
union uSMCONV
{
	MOLASharedMemoryRouter*		pROUTER;
	PVOID					ptr;
};

MOLASharedMemoryPool*		MOLASharedMemoryPool::ms_pInstance = NULL;
INT						MOLASharedMemoryPool::ms_iCounts = 0;

static BYTE				s_SharedMemoryBlock[sizeof(MOLASharedMemoryPool)];
static BYTE				s_MOLASharedMemoryRouter[sizeof(MOLASharedMemoryRouter)];

static BYTE				s_SharedMemoryToken8[sizeof(MOLASharedMemoryBandwith<8>)];
static BYTE				s_SharedMemoryToken16[sizeof(MOLASharedMemoryBandwith<16>)];
static BYTE				s_SharedMemoryToken24[sizeof(MOLASharedMemoryBandwith<24>)];
static BYTE				s_SharedMemoryToken32[sizeof(MOLASharedMemoryBandwith<32>)];
static BYTE				s_SharedMemoryToken40[sizeof(MOLASharedMemoryBandwith<40>)];
static BYTE				s_SharedMemoryToken48[sizeof(MOLASharedMemoryBandwith<48>)];
static BYTE				s_SharedMemoryToken56[sizeof(MOLASharedMemoryBandwith<56>)];
static BYTE				s_SharedMemoryToken64[sizeof(MOLASharedMemoryBandwith<64>)];
static BYTE				s_SharedMemoryToken72[sizeof(MOLASharedMemoryBandwith<72>)];
static BYTE				s_SharedMemoryToken80[sizeof(MOLASharedMemoryBandwith<80>)];
static BYTE				s_SharedMemoryToken88[sizeof(MOLASharedMemoryBandwith<88>)];
static BYTE				s_SharedMemoryToken96[sizeof(MOLASharedMemoryBandwith<96>)];
static BYTE				s_SharedMemoryToken104[sizeof(MOLASharedMemoryBandwith<104>)];
static BYTE				s_SharedMemoryToken112[sizeof(MOLASharedMemoryBandwith<112>)];
static BYTE				s_SharedMemoryToken120[sizeof(MOLASharedMemoryBandwith<120>)];
static BYTE				s_SharedMemoryToken128[sizeof(MOLASharedMemoryBandwith<128>)];

static BYTE				s_SharedMemoryToken144[sizeof(MOLASharedMemoryBandwith<144>)];
static BYTE				s_SharedMemoryToken160[sizeof(MOLASharedMemoryBandwith<160>)];
static BYTE				s_SharedMemoryToken176[sizeof(MOLASharedMemoryBandwith<176>)];
static BYTE				s_SharedMemoryToken192[sizeof(MOLASharedMemoryBandwith<192>)];
static BYTE				s_SharedMemoryToken208[sizeof(MOLASharedMemoryBandwith<208>)];
static BYTE				s_SharedMemoryToken224[sizeof(MOLASharedMemoryBandwith<224>)];
static BYTE				s_SharedMemoryToken240[sizeof(MOLASharedMemoryBandwith<240>)];
static BYTE				s_SharedMemoryToken256[sizeof(MOLASharedMemoryBandwith<256>)];

static BYTE				s_SharedMemoryToken288[sizeof(MOLASharedMemoryBandwith<288>)];
static BYTE				s_SharedMemoryToken320[sizeof(MOLASharedMemoryBandwith<320>)];
static BYTE				s_SharedMemoryToken352[sizeof(MOLASharedMemoryBandwith<352>)];
static BYTE				s_SharedMemoryToken384[sizeof(MOLASharedMemoryBandwith<384>)];
static BYTE				s_SharedMemoryToken416[sizeof(MOLASharedMemoryBandwith<416>)];
static BYTE				s_SharedMemoryToken448[sizeof(MOLASharedMemoryBandwith<448>)];
static BYTE				s_SharedMemoryToken480[sizeof(MOLASharedMemoryBandwith<480>)];
static BYTE				s_SharedMemoryToken512[sizeof(MOLASharedMemoryBandwith<512>)];

VOID				MOLASharedMemoryPool::CreateInstance()
{
	if( ms_pInstance )
		return;

	ms_pInstance = new (s_SharedMemoryBlock) MOLASharedMemoryPool;
}

VOID				MOLASharedMemoryPool::DestroyInstance()
{
	((MOLASharedMemoryPool*)s_SharedMemoryBlock)->MOLASharedMemoryPool::~MOLASharedMemoryPool();
}

MOLASharedMemoryPool::MOLASharedMemoryPool()
{
	static BOOL sbINIT = FALSE;

	if( !sbINIT )
	{
		//ms_pInstance = this;
		MOLASMEMINFO::Initialize();
		uSMCONV uConv;
		uConv.pROUTER = new (s_MOLASharedMemoryRouter) MOLASharedMemoryRouter;
		m_pInterBlock = uConv.ptr;
		sbINIT = TRUE;
		ms_iCounts = 1;
	}
}

MOLASharedMemoryPool::~MOLASharedMemoryPool()
{
	static BOOL sbFREE = FALSE;

	if( !sbFREE )
	{
		uSMCONV uConv;
		uConv.ptr = m_pInterBlock;

		if( uConv.ptr )
		{
			uConv.pROUTER->MOLASharedMemoryRouter::~MOLASharedMemoryRouter();
			m_pInterBlock = NULL;
		}

		sbFREE = TRUE;
	}
}


PVOID					MOLASharedMemoryPool::MemoryAllocate( SIZE_T sz )
{
	MOLASharedMemoryPool* pINSTANCE = ms_pInstance;
	uSMCONV uConv;
	uConv.ptr = pINSTANCE->m_pInterBlock;

	uConv.ptr = uConv.pROUTER->Alloc( sz );

#if CODE_TEST(__MOLA_CLIENT_DEBUGGING_SHMEM__)
	if( !uConv.ptr || (DWORD)(LONGLONG)uConv.ptr < 10 )
		printf( "NULL\n" );
#endif

	++ms_iCounts;

	return uConv.ptr;
}

VOID					MOLASharedMemoryPool::MemoryDeAllocate( PVOID ptr )
{
	MOLASharedMemoryPool* pINSTANCE = ms_pInstance;
	if( !(ptr && pINSTANCE) )
		return;

	uSMCONV uConv;
	uConv.ptr = pINSTANCE->m_pInterBlock;

	uConv.pROUTER->Free( ptr );

	--ms_iCounts;

	if( ms_iCounts < 2 )
		MOLASharedMemoryPool::DestroyInstance();
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
//BYTE				MOLASMEMINFO::IDXofSIZE[MAX_SHARED_MEMORY_008_SIZE];
WORD				MOLASMEMINFO::SIZEofIDX[SIZE_032_MAX];

VOID				MOLASMEMINFO::Initialize()
{
	//ZeroMemory( IDXofSIZE, sizeof(IDXofSIZE) );
	ZeroMemory( SIZEofIDX, sizeof(SIZEofIDX) );

	for( DWORD dwIdx=SIZE_008 ; dwIdx<SIZE_008_MAX ; ++dwIdx )
	{
		WORD wAlignSize = (WORD)BUF_ALIGN_008_BY_IDX(dwIdx);
		//IDXofSIZE[byAlignSize] = byIdx;
		SIZEofIDX[dwIdx] = wAlignSize;
	}

	for( DWORD dwIdx=SIZE_144 ; dwIdx<SIZE_016_MAX ; ++dwIdx )
	{
		WORD wAlignSize = (WORD)BUF_ALIGN_016_BY_IDX(dwIdx);
		//IDXofSIZE[byAlignSize] = byIdx;
		SIZEofIDX[dwIdx] = wAlignSize;
	}

	for( DWORD dwIdx=SIZE_288 ; dwIdx<SIZE_032_MAX ; ++dwIdx )
	{
		WORD wAlignSize = (WORD)BUF_ALIGN_032_BY_IDX(dwIdx);
		//IDXofSIZE[byAlignSize] = byIdx;
		SIZEofIDX[dwIdx] = wAlignSize;
	}
}


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//
MOLASharedMemoryRouter::MOLASharedMemoryRouter()
{
#define SMALLOCROUTER_DECL(no)	new (s_SharedMemoryToken##no) MOLASharedMemoryBandwith<no>

	ROUTER[MOLASMEMINFO::SIZE_008] = SMALLOCROUTER_DECL(8)( 0, 100 );
	ROUTER[MOLASMEMINFO::SIZE_016] = SMALLOCROUTER_DECL(16)( 0, 100 );
	ROUTER[MOLASMEMINFO::SIZE_024] = SMALLOCROUTER_DECL(24)( 0, 100 );
	ROUTER[MOLASMEMINFO::SIZE_032] = SMALLOCROUTER_DECL(32)( 0, 100 );
	ROUTER[MOLASMEMINFO::SIZE_040] = SMALLOCROUTER_DECL(40)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_048] = SMALLOCROUTER_DECL(48)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_056] = SMALLOCROUTER_DECL(56)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_064] = SMALLOCROUTER_DECL(64)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_072] = SMALLOCROUTER_DECL(72)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_080] = SMALLOCROUTER_DECL(80)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_088] = SMALLOCROUTER_DECL(88)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_096] = SMALLOCROUTER_DECL(96)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_104] = SMALLOCROUTER_DECL(104)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_112] = SMALLOCROUTER_DECL(112)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_120] = SMALLOCROUTER_DECL(120)( 0, 10 );
	ROUTER[MOLASMEMINFO::SIZE_128] = SMALLOCROUTER_DECL(128)( 0, 10 );
	// 16x
	ROUTER[MOLASMEMINFO::SIZE_144] = SMALLOCROUTER_DECL(144)( 0, 5 );
	ROUTER[MOLASMEMINFO::SIZE_160] = SMALLOCROUTER_DECL(160)( 0, 5 );
	ROUTER[MOLASMEMINFO::SIZE_176] = SMALLOCROUTER_DECL(176)( 0, 5 );
	ROUTER[MOLASMEMINFO::SIZE_192] = SMALLOCROUTER_DECL(192)( 0, 5 );
	ROUTER[MOLASMEMINFO::SIZE_208] = SMALLOCROUTER_DECL(208)( 0, 5 );
	ROUTER[MOLASMEMINFO::SIZE_224] = SMALLOCROUTER_DECL(224)( 0, 5 );
	ROUTER[MOLASMEMINFO::SIZE_240] = SMALLOCROUTER_DECL(240)( 0, 5 );
	ROUTER[MOLASMEMINFO::SIZE_256] = SMALLOCROUTER_DECL(256)( 0, 4 );
	// 32x
	ROUTER[MOLASMEMINFO::SIZE_288] = SMALLOCROUTER_DECL(288)( 0, 1 );
	ROUTER[MOLASMEMINFO::SIZE_320] = SMALLOCROUTER_DECL(320)( 0, 1 );
	ROUTER[MOLASMEMINFO::SIZE_352] = SMALLOCROUTER_DECL(352)( 0, 1 );
	ROUTER[MOLASMEMINFO::SIZE_384] = SMALLOCROUTER_DECL(384)( 0, 1 );
	ROUTER[MOLASMEMINFO::SIZE_416] = SMALLOCROUTER_DECL(416)( 0, 1 );
	ROUTER[MOLASMEMINFO::SIZE_448] = SMALLOCROUTER_DECL(448)( 0, 1 );
	ROUTER[MOLASMEMINFO::SIZE_480] = SMALLOCROUTER_DECL(480)( 0, 1 );
	ROUTER[MOLASMEMINFO::SIZE_512] = SMALLOCROUTER_DECL(512)( 0, 1 );


}

MOLASharedMemoryRouter::~MOLASharedMemoryRouter()
{
#define SMDEALLOCROUTER_DECL(no)	\
	((MOLASharedMemoryBandwith<no>*)s_SharedMemoryToken##no)->MOLASharedMemoryBandwith<no>::~MOLASharedMemoryBandwith()

	SMDEALLOCROUTER_DECL(8); 
	SMDEALLOCROUTER_DECL(16);
	SMDEALLOCROUTER_DECL(24);
	SMDEALLOCROUTER_DECL(32);
	SMDEALLOCROUTER_DECL(40);
	SMDEALLOCROUTER_DECL(48);
	SMDEALLOCROUTER_DECL(56);
	SMDEALLOCROUTER_DECL(64);
	SMDEALLOCROUTER_DECL(72);
	SMDEALLOCROUTER_DECL(80);
	SMDEALLOCROUTER_DECL(88);
	SMDEALLOCROUTER_DECL(96);
	SMDEALLOCROUTER_DECL(104);
	SMDEALLOCROUTER_DECL(112);
	SMDEALLOCROUTER_DECL(120);
	SMDEALLOCROUTER_DECL(128);

	SMDEALLOCROUTER_DECL(144);
	SMDEALLOCROUTER_DECL(160);
	SMDEALLOCROUTER_DECL(176);
	SMDEALLOCROUTER_DECL(192);
	SMDEALLOCROUTER_DECL(208);
	SMDEALLOCROUTER_DECL(224);
	SMDEALLOCROUTER_DECL(240);
	SMDEALLOCROUTER_DECL(256);

	SMDEALLOCROUTER_DECL(288);
	SMDEALLOCROUTER_DECL(320);
	SMDEALLOCROUTER_DECL(352);
	SMDEALLOCROUTER_DECL(384);
	SMDEALLOCROUTER_DECL(416);
	SMDEALLOCROUTER_DECL(448);
	SMDEALLOCROUTER_DECL(480);
	SMDEALLOCROUTER_DECL(512);

}

#endif //