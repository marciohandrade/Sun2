#pragma once
#ifndef UTILITY_MEMORY_POOL_FACTORY_H
#define UTILITY_MEMORY_POOL_FACTORY_H

// MemoryPoolFactory.h: interface for the CMemoryPoolFactory class.
// Memory Pool Management Class version 2.1
// source code created by Min-Wook Kim (taiyo@webzen.co.kr)
// 2004-1-10
// 
//-------------------------------------------------------------------
// History.
// 2004-3-31	:	MemoryPoolTempl class ver 0.1
// 2004-6-18	:	MemoryPoolTempl class ver 0.2
// 2004-6-21    :	Error Check Byte added ver 0.21
// 2004-7-26    :	MemoryPool Framework ver1.0
// 2005-1-10    :	SafeMemoryPoolFactory class added 1.01
// 2005-3-28    :	set default value as dwPoolExtendSize = dwPoolBasicSize/2+1
// 2007-6-15	:	Refactoring (Changed to virtual Memory Pool Router & Enhanced Exception Handling)
//                  ver 2.1 - written by Waverix
// 2011-05-14   : ver 3.0, refactoring, changes internal data structure
//                using shared memory architecture after client application.
//                this implements are stripped version based on WxFramework::MemoryPoolSizeBased
//                and WxFramework::MemoryPool.
//
// Be Done
//		- MemoryPool template class of inherited-method and factory-method
//		- (v3.0, be not considered by usage case) Multi-thread safety
//		- (v3.0, prepared) Buffer overflow write check
//		- Single Interface & Multiple (various) Instances - using by MemoryPoolRouter
//      - (v3.0, meanless by SUNLOG interface) Enhanced Exception Handling (strong pool checking)
// To do
//		- Analysis of amount used for band
//		- alloc() for various data type
//-------------------------------------------------------------------
// example)
//
//	1. Thread-NonSafetyPool sample code
//  class Money1 { ... };
//
//  void main() 
//  {
//    CMemoryPoolFactory<Money1> * pool = new CMemoryPoolFactory<Money1>;
//    pool->Initialize( 10000, 510 );
//
//    Money1 * pmoney11 = pool->Alloc();
//    pool->Free(pmoney11);
//
//    pool->Release();
//  }
//
//	2. Thread-SafetyPool sample code
//  class Money1 { ... };
//
//  void main() 
//  {
//    SafeMemoryPoolFactory<Money1> * pool = new SafeMemoryPoolFactory<Money1>; //!!!!!!!! different thing !!!!!!!
//    pool->Initialize( 10000, 510 );
//
//    Money1 * pmoney11 = pool->Alloc();
//    pool->Free(pmoney11);
//
//    pool->Release();
//  }
//
// error situation)
// If (m_pFreeBand of m_pFreeBand->pNext) is NULL in Free() function,
// =>possibility
// 1. Free()을 Alloc()호출한 횟수이상을 호출했을 때 
// 2. 또는 Free(node)의 node가 중복되게 Free되었을 때
//////////////////////////////////////////////////////////////////////

#include "UtilityCommon.h"
#include <boost/pool/pool.hpp>
#include <boost/scoped_ptr.hpp>

#define MEMORY_POOL_FACTORY_VERSION     0x0300

#if 0

//==================================================================================================
//==================================================================================================
//==================================================================================================
#elif MEMORY_POOL_FACTORY_VERSION >= 0x0300
//==================================================================================================
//==================================================================================================
//==================================================================================================

#include "./LinkedListCS.h"

//==================================================================================================

#define __DECL_CUSTOMPOOL_PTR(_CLASS_NAME_) \
    private: \
        typedef ::util::CMemoryPoolFactory<_CLASS_NAME_ >* custom_pool_type; \
        static custom_pool_type custom_pool_static; \
    public: \
        static void SetPool(custom_pool_type initialized_pool_ptr) { \
            custom_pool_static = initialized_pool_ptr; \
            custom_pool_static->SetTypeName(#_CLASS_NAME_); \
        } \
        static _CLASS_NAME_* ALLOC() { \
            return reinterpret_cast<_CLASS_NAME_*>(custom_pool_static->Alloc()); } \
        static void FREE(_CLASS_NAME_* query_ptr) { custom_pool_static->Free(query_ptr); } \
    public:

#define __IMPL_CUSTOMPOOL_PTR(_CLASS_NAME_) \
    _CLASS_NAME_::custom_pool_type _CLASS_NAME_::custom_pool_static = NULL; \

//==================================================================================================

namespace util {
;

//==================================================================================================

struct IVMemoryPool
{
    virtual ~IVMemoryPool() = 0;
    virtual void* Alloc() = 0;
    virtual BOOL Free(void* node) = 0;
    virtual bool Initialize(size_t init_size, size_t chunk_size = 0,
                            const char* type_name = NULL, void* reserved = NULL) = 0;
    virtual void Release() = 0;
};

//==================================================================================================

namespace internal {

// NOTE: f110513.7L, this class is used by CMemoryPoolFactory<T>
class MemoryPoolSizeBased
{
public:
    struct Statistics 
    {
        size_t number_of_free_nodes;
        size_t number_of_alloc_nodes;
        size_t number_of_total_blocks;
    };

    typedef void (*func_object_lifetime)(void*);


    MemoryPoolSizeBased(size_t type_size,
                        func_object_lifetime instance_func,
                        func_object_lifetime delete_func);
    ~MemoryPoolSizeBased();



    void Initialize(size_t init_size, size_t chunk_size = 0,
                    const char* type_name = NULL);

    void* Allocate();
    void Deallocate(void* element);

    void FreeAllElements();

    void SetTypeName(const char* type_name)
    {
        type_name_ = type_name;
    }

    const Statistics& GetStatistics() const
    {
        return statistics_;
    }

private:
    struct TokenBlock;
    struct Token;

    void AddBlock(size_t element_count);

    const size_t type_size_;
    util::__LList<Token> free_list_;
    util::__LList<Token> alloc_list_;
    util::__LList<TokenBlock> total_list_;
    Statistics statistics_;
    size_t init_count_;
    size_t growth_count_;
    const func_object_lifetime instance_func_;
    const func_object_lifetime delete_func_;
    const char* type_name_;
    //
    __DISABLE_COPY(MemoryPoolSizeBased);

}; // MemoryPoolSizeBased


////////////////////////////////////////////////////////////////////////////////////////////////////
// IMemoryPoolFactoryImpl
////////////////////////////////////////////////////////////////////////////////////////////////////

class IMemoryPoolFactoryImpl
{
public:
    IMemoryPoolFactoryImpl() 
    {
    }
    virtual ~IMemoryPoolFactoryImpl() 
    {
    }

public:
    virtual bool Initialize(
        const size_t /*init_size*/, const size_t /*chunk_size*/, const char* /*type_name*/)
    {
        return true;
    }
    virtual void Release() 
    {
    }

public:
    virtual void* Alloc() = 0;
    virtual void Free(void* const ptr) = 0;

public:
    virtual void SetTypeName(const char* /*type_name*/)
    {
    }
    virtual int GetPoolBasicSize() const
    {
        return 0;
    }
    virtual int GetPoolExtendSize() const
    {
        return 0;
    }
    virtual int GetNumberOfBands() const
    {
        return 0;
    }
    virtual int GetAvailableNumberOfTypes() const
    {
        return 0;
    }

}; // IMemoryPoolFactoryImpl


////////////////////////////////////////////////////////////////////////////////////////////////////
// NoChunkImpl
////////////////////////////////////////////////////////////////////////////////////////////////////

// 생성자가 없는 struct의 경우 경고가 new(alloc) OBJECT_T(); 에서 경고가 떠서 해당 경고를 비활성화
#pragma warning (disable:4345)

template<typename OBJECT_T>
class NotUseChunkImpl : public IMemoryPoolFactoryImpl
{
public:
    NotUseChunkImpl()
    {
    }
    virtual ~NotUseChunkImpl()
    {
    }

public:
    virtual void* Alloc()
    {
        void* alloc = ::malloc(sizeof(OBJECT_T));
        if (alloc != NULL)
        {
            new(alloc) OBJECT_T();
        }
        return alloc;        
    }
    virtual void Free(void* const ptr)
    {
        if (ptr != NULL)
        {
            reinterpret_cast<OBJECT_T*>(ptr)->~OBJECT_T();
            ::free(ptr);
        } 
    }
}; // NotUseChunkImpl


////////////////////////////////////////////////////////////////////////////////////////////////////
// VariableSizeChunkImpl
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename OBJECT_T>
class VariableSizeChunkImpl : public IMemoryPoolFactoryImpl
{
public:
    VariableSizeChunkImpl() : 
        allocator_(sizeof(OBJECT_T), &Create, &Destroy)
    {
    }
    virtual ~VariableSizeChunkImpl()
    {
        Release();
    }

public:
    virtual bool Initialize(
        const size_t init_size, const size_t chunk_size, const char* type_name)
    {
        allocator_.Initialize(init_size, chunk_size, type_name);
        return true;
    }
    virtual void Release()
    {
        allocator_.FreeAllElements();
    }

public:
    virtual void* Alloc()
    {
        return allocator_.Allocate();
    }
    virtual void Free(void* const ptr)
    {
        allocator_.Deallocate(ptr);
    }

public:
    virtual void SetTypeName(const char* type_name)
    {
        allocator_.SetTypeName(type_name);
    }
    virtual int GetNumberOfBands() const
    {
        return static_cast<int>(allocator_.GetStatistics().number_of_alloc_nodes);
    }
    virtual int GetAvailableNumberOfTypes() const
    {
        return static_cast<int>(allocator_.GetStatistics().number_of_free_nodes);
    }

private:
    static void Create(void* ptr)
    {
        new(ptr) OBJECT_T;
    }
    static void Destroy(void* ptr)
    {
        reinterpret_cast<OBJECT_T*>(ptr)->~OBJECT_T();
    }

private:
    MemoryPoolSizeBased allocator_;

}; // VariableSizeChunkImpl


////////////////////////////////////////////////////////////////////////////////////////////////////
// FixedSizeChunkImpl
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename OBJECT_T>
class FixedSizeChunkImpl : public IMemoryPoolFactoryImpl
{
public:
    FixedSizeChunkImpl() : impl_(sizeof(OBJECT_T))
    {
    }
    virtual ~FixedSizeChunkImpl()
    {
    }

public:
    virtual void Release()
    {
        impl_.release_memory();
    }

public:
    virtual void* Alloc()
    {
        // 이전 가변형 메모리풀 동작 방식이 이상한것이 생성자 호출을 제외한 메모리 할당을 요구하는데
        // 할당된 객체는 생성자 호출이 완료된 상태를 가정하고 사용한다.
        // 물론 수동으로 다시 생성자를 호출하는 경우도 존재한다.
        // 아래 주석처리된 코드가 맞지만 호환성을 위해 이렇게 처리한다. 
        //return impl_.malloc();
        void* alloc = impl_.malloc();
        if (alloc != NULL)
        {
            new(alloc) OBJECT_T();
        }
        return alloc;        
    }
    virtual void Free(void* const ptr)
    {
        if (ptr != NULL)
        {
            reinterpret_cast<OBJECT_T*>(ptr)->~OBJECT_T();
            impl_.free(ptr);
        }        
    }

private:
    typedef boost::pool<> PoolImpl;
    PoolImpl impl_;

}; // FixedSizeChunkImpl

}; // internal


////////////////////////////////////////////////////////////////////////////////////////////////////
// CMemoryPoolFactory
////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename OBJECT_T>
class CMemoryPoolFactory : public IVMemoryPool
{
public:
    enum ChunkMode
    {
        kNotUseChunk = 0, 
        kVariableChunk = 1, 
        kFixedChunk = 2
    };
//#ifdef USE_FIXED_SIZE_MEMORY_POOL
//    enum { kDefaultChunkMode = kFixedChunk };
//    typedef internal::FixedSizeChunkImpl<OBJECT_T> DefaultChunkImpl;
//#else
    enum { kDefaultChunkMode = kVariableChunk };
    typedef internal::VariableSizeChunkImpl<OBJECT_T> DefaultChunkImpl;
//#endif

public:
    CMemoryPoolFactory()
    {
    }
    virtual ~CMemoryPoolFactory() 
    {
    }
    
public:
    virtual bool Initialize(
        size_t init_size, size_t chunk_size = 0, const char* type_name = NULL, void* reserved = NULL)
    {
        __UNUSED(reserved);

        int chunk_mode = kDefaultChunkMode;
        /*{
            char path_buf[MAX_PATH] = { 0 };
            const size_t written = ::GetModuleFileName(NULL, path_buf, _countof(path_buf));
            if (written > 0)
            {
                std::string ini_path(path_buf, written);
                const size_t deli_pos = ini_path.find_last_of('\\');
                if (deli_pos > 0)
                {
                    ini_path = ini_path.substr(0, deli_pos);
                    ini_path += "\\MemoryPool.ini";
                    chunk_mode = ::GetPrivateProfileInt(
                        "CMEMORY_POOL_FACTORY", "CHUNK_MODE", kDefaultChunkMode, ini_path.c_str());
                }
            }
        }*/
        CreateImpl(chunk_mode);
        return impl_->Initialize(init_size, chunk_size, type_name);
    }
    virtual void* Alloc()
    {
        return impl_->Alloc();
    }
    virtual BOOL Free(void* node)
    {
        impl_->Free(node);
        return TRUE;
    }
    virtual void Release()
    {
        impl_->Release();
    }

public:
    void CreateImpl(const int chunk_mode)
    {
        switch (chunk_mode)
        {
        case kNotUseChunk:
            impl_.reset(new internal::NotUseChunkImpl<OBJECT_T>());
            return;
        case kVariableChunk:
            impl_.reset(new internal::VariableSizeChunkImpl<OBJECT_T>());
        	return;
        case kFixedChunk:
            impl_.reset(new internal::FixedSizeChunkImpl<OBJECT_T>());
            return;
        default:
            impl_.reset(new DefaultChunkImpl());
            return;
        }
    }
    void SetTypeName(const char* type_name)
    {
        impl_->SetTypeName(type_name);
    }
    int GetPoolBasicSize() const
    {
        return impl_->GetPoolBasicSize();
    }
    int GetPoolExtendSize() const
    {
        return impl_->GetPoolExtendSize();
    }
    int GetNumberOfBands() const
    {
        return impl_->GetNumberOfBands();
    }
    int GetAvailableNumberOfTypes() const
    {
        return impl_->GetAvailableNumberOfTypes();
    }

private:
    typedef boost::scoped_ptr<internal::IMemoryPoolFactoryImpl> FactoryImpl;
    FactoryImpl impl_;

}; // CMemoryPoolFactory

}; // util


//==================================================================================================
//==================================================================================================
//==================================================================================================
#elif MEMORY_POOL_FACTORY_VERSION >= 0x0210
//==================================================================================================
//==================================================================================================
//==================================================================================================

#include "MemBand.h"
#include "LinkedListCS.h"

//--------------------------------------------------------------------------------
#define __DECL_CUSTOMPOOL_PTR(_ClassType)								\
	typedef util::CMemoryPoolFactory<_ClassType> * CUSTOM_POOL_PTR;		\
	private:															\
	static CUSTOM_POOL_PTR s_pPool;										\
	public:																\
	static VOID SetPool( CUSTOM_POOL_PTR spPool ) { s_pPool = spPool; }	\
	static _ClassType *	ALLOC() { return (_ClassType *)s_pPool->Alloc(); }			\
	static VOID FREE( _ClassType * pClass ) { s_pPool->Free( pClass ); }
//	static VOID	DisplayerPoolInfo()										\
//{																	\
//	DISPMSG( "[%4u,%4u][band:%u node:%u]", s_pPool->GetPoolBasicSize(), s_pPool->GetPoolExtendSize(),	\
//	s_pPool->GetNumberOfBands(), s_pPool->GetAvailableNumberOfTypes() );	\
//}

#define __IMPL_CUSTOMPOOL_PTR(_ClassType)								\
	_ClassType::CUSTOM_POOL_PTR _ClassType::s_pPool = NULL;


namespace util
{
#ifdef _SERVER
#	define POOL_ASSERTION_LOGGER( errTYPE )			m_LOGINFO.LOGGING( errTYPE )
#else //_CLIENT
#	define POOL_ASSERTION_LOGGER	{ /*(###expression##)*/ ; }
#endif //

	struct ePOOL_ERROR;
	extern	VOID AssertionLogger4Pool_Default( DWORD errTYPE, ePOOL_ERROR* IN pINFO );

	struct ePOOL_ERROR
	{
		enum ERROR_TYPE {
			DEFAULT_LOGGING				,
			INVALID_REFERENCE			,
			CANT_ALLOCATABLE			,
			CANT_DEALLOCATABLE			,
			NOT_MATCHED_ALLOCnDEALLOC	,
			BUFFER_OVERRUN				,
		};

		typedef	VOID	(*AssertionLogger4Pool)( DWORD errTYPE, ePOOL_ERROR* IN pINFO );
		VOID			LOGGING( DWORD errTYPE )
		{
			if( m_LOGGER )
				m_LOGGER( errTYPE, this );
#ifdef _SERVER
			else
				AssertionLogger4Pool_Default( errTYPE, this );
#endif //
		}

		ePOOL_ERROR() : m_LOGGER(NULL), m_pszTYPENAME(NULL) {}

		AssertionLogger4Pool	m_LOGGER;
		TCHAR*					m_pszTYPENAME;
	};

	//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┫
	//

	// ========================================================================================
	//
	class IVMemoryPool
	{
	public:
		virtual ~IVMemoryPool() { Release(); }
		virtual PVOID	Alloc() = 0;
		virtual BOOL	Free( PVOID node ) = 0;
		virtual BOOL	Initialize( DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0, TCHAR* ptszTypeName = NULL, ePOOL_ERROR::AssertionLogger4Pool fnLOGGER = NULL ) = 0;
		virtual VOID	Release() {};
	};
	//
	// ========================================================================================
	union POOL_VERIFIER
	{
		DWORD	m_dwCHECK;
		DWORD*	m_pCHECK;
		DWORD	m_pdwCHECK[1];
	};

	template<typename _TYPE>
	class CMemoryPoolFactory : public IVMemoryPool
	{
		//////////////////////////////////////////////////////////////////////////
		// <POOL VERIFICATION TOOL>
	private:	static const DWORD	MAGIC_CODE	= 0xDEADBEEF;

#pragma warning(push)
#pragma warning(disable : 4512)
// (NOTE) below objects have not default constructor, similar to c structure
	private:	struct sNODE
				{
#			pragma pack(push, 4)
#				pragma pack(push, 1)
					POOL_VERIFIER		HEADER;
					_TYPE				TYPE;
#				pragma pack(pop)
					POOL_VERIFIER		TAIL;
#			pragma pack(pop)

					sNODE() { HEADER.m_dwCHECK = MAGIC_CODE; TAIL.m_dwCHECK = MAGIC_CODE; }
					inline _TYPE*	TypePtr() { return &TYPE; }
					// CanAllocable 은 항상 성공해야 한다.
					inline BOOL		CanAllocable() { return (BOOL)(MAGIC_CODE == HEADER.m_dwCHECK); }
					inline BOOL		CanDeAllocable( POOL_VERIFIER* pROOT )
					{ return (BOOL)( pROOT->m_pdwCHECK == HEADER.m_pCHECK ); }
					inline BOOL		IsBufferOverrun( POOL_VERIFIER* pROOT )
					{ return (BOOL)!( (pROOT->m_pdwCHECK == HEADER.m_pCHECK) && (MAGIC_CODE == TAIL.m_dwCHECK) ); }

					inline VOID		SetAlloced( POOL_VERIFIER* pROOT )
					{ HEADER.m_pCHECK = pROOT->m_pdwCHECK; }
					inline VOID		SetFreed()
					{ HEADER.m_dwCHECK = MAGIC_CODE; }

					inline static sNODE*	GetHeader( _TYPE* pNODE )
					{ return (sNODE*)( ((BYTE*)pNODE) - offsetof(sNODE, TYPE) ); }

					inline static VOID		InitRef( POOL_VERIFIER* pROOT )
					{ pROOT->m_pCHECK = pROOT->m_pdwCHECK; }
				};
				typedef CMemTypeBand<sNODE>		MEMORY_BANDWITH;
#pragma warning(pop)
				//////////////////////////////////////////////////////////////////////////
				// <CONSTRUCTOR><DESTRUCTOR>
	public:		CMemoryPoolFactory() : m_pBandHead( NULL ), m_pFreeBand( NULL ), m_dwPoolExtendSize( 0 )
				{ sNODE::InitRef( &m_ROOT ); }
				virtual ~CMemoryPoolFactory() { Release(); }

				//////////////////////////////////////////////////////////////////////////
				// <FIELDs>
	protected:	CMemTypeBand<sNODE>*		m_pBandHead;
				CMemTypeBand<sNODE>*		m_pFreeBand;
				DWORD						m_dwPoolExtendSize;
	private:	POOL_VERIFIER				m_ROOT;
				ePOOL_ERROR					m_LOGINFO;

				//////////////////////////////////////////////////////////////////////////
				// <INTERFACEs>
	public:		virtual BOOL	Initialize( DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0, TCHAR* ptszTypeName = NULL, ePOOL_ERROR::AssertionLogger4Pool fnLOGGER = NULL )
				{
					m_dwPoolExtendSize =
						dwPoolExtendSize ? dwPoolExtendSize
						: (dwPoolBasicSize>>1) ? (dwPoolBasicSize>>1)
						: 1;

					if( m_pFreeBand )
						return FALSE;

					m_LOGINFO.m_pszTYPENAME = ptszTypeName ? ptszTypeName : NULL;
					m_LOGINFO.m_LOGGER = fnLOGGER ? fnLOGGER : NULL;

					MEMORY_BANDWITH::AllocBand( m_pBandHead, dwPoolBasicSize );
					m_pFreeBand = m_pBandHead;

					if( !m_pFreeBand ) 
						return FALSE;

					return TRUE;
				}

				virtual VOID	Release()
				{
					if( m_pBandHead )
					{
						MEMORY_BANDWITH::FreeBand( m_pBandHead );
						//m_pBandHead->FreeBand();
						m_pBandHead = NULL;
						m_pFreeBand = NULL;
					}
				}

				virtual PVOID	Alloc()
				{
					sNODE* pNODE = m_pFreeBand->AlloObject();

					if( !pNODE )
					{
						if( m_pFreeBand->pPrev )
							m_pFreeBand = m_pFreeBand->pPrev;
						else
						{
							MEMORY_BANDWITH::AllocBand( m_pBandHead, m_dwPoolExtendSize );
							m_pFreeBand = m_pBandHead;
						}

						pNODE = m_pFreeBand->AlloObject();
					}

					//ASSERT( !IsBadReadPtr( pNODE, sizeof(sNODE) ) );
					//ASSERT( !IsBadWritePtr( pNODE, sizeof(sNODE) ) );

					if( !pNODE->CanAllocable() )
						return NULL;

					pNODE->SetAlloced( &m_ROOT );

					return pNODE->TypePtr();
				}

				virtual BOOL Free( PVOID pNode )
				{
					//ASSERT( !IsBadReadPtr( pNODE, sizeof(sNODE) ) );
					const DWORD RET_FAIL = 0x0;
					const DWORD RET_CLEAR_COND1 = 0x1;
					const DWORD RET_CLEAR_COND2 = RET_CLEAR_COND1<<1;
					const DWORD RET_SUCCESS = RET_CLEAR_COND1|RET_CLEAR_COND2;

					DWORD bSuccess = RET_FAIL;

					sNODE* pNODE = sNODE::GetHeader( (_TYPE*)pNode );
					bSuccess = ( pNode && !IsBadWritePtr( pNODE, sizeof(sNODE) ) );
					if( !bSuccess )
					{
						POOL_ASSERTION_LOGGER( ePOOL_ERROR::INVALID_REFERENCE );
						return FALSE;
					}

					bSuccess = pNODE->CanDeAllocable( &m_ROOT ) && !pNODE->IsBufferOverrun( &m_ROOT ) ? RET_CLEAR_COND1 : RET_FAIL;
					if( bSuccess )
						pNODE->SetFreed();

					bSuccess = bSuccess && m_pFreeBand->FreeObject( pNODE ) ? bSuccess|RET_CLEAR_COND2 : bSuccess;

					if( RET_SUCCESS == bSuccess )
						return TRUE;

					if( bSuccess == RET_FAIL )
					{
						POOL_ASSERTION_LOGGER( ePOOL_ERROR::CANT_DEALLOCATABLE );
						return FALSE;
					}

					// 현재 상황 ( bSuccess&RET_CLEAR_COND1 ) == TRUE
					if( !(m_pFreeBand->pNext) )
					{
						POOL_ASSERTION_LOGGER( ePOOL_ERROR::NOT_MATCHED_ALLOCnDEALLOC );
#					ifdef _DEBUG
						__debugbreak();
#					endif //

						return FALSE;
					}

					m_pFreeBand = m_pFreeBand->pNext;
					return m_pFreeBand->FreeObject( pNODE );
				}

				inline DWORD GetPoolBasicSize()
				{
					MEMORY_BANDWITH* pHead = m_pBandHead;
					MEMORY_BANDWITH* ptmpHead = NULL;
					while(pHead)
					{
						ptmpHead = pHead;
						pHead = pHead->pNext;
					}
					if( ptmpHead )
						return ptmpHead->GetMaxNumberOfObjects();
					return 0;
				}

				inline DWORD GetPoolExtendSize()
				{
					return m_dwPoolExtendSize;
				}

				inline DWORD GetNumberOfBands()
				{
					DWORD dwTotalNum = 0;
					MEMORY_BANDWITH* pHead = m_pBandHead;
					while( pHead )
					{
						++dwTotalNum;
						pHead = pHead->pNext;
					}
					return dwTotalNum;
				}

				inline DWORD GetAvailableNumberOfTypes()
				{
					DWORD dwTotalNum = 0;
					MEMORY_BANDWITH* pHead = m_pBandHead;
					while(pHead)
					{
						dwTotalNum += pHead->GetAvailableNumberOfObjects();
						pHead = pHead->pNext;
					}
					return dwTotalNum;
				}
	};

} /// namespace util

//==================================================================================================
//==================================================================================================
//==================================================================================================
#endif //MEMORY_POOL_FACTORY_VERSION >= 0x0210
//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //UTILITY_MEMORY_POOL_FACTORY_H