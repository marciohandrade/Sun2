#ifndef __SAFEMEMORYPOOLFACTORY2_H__
#define __SAFEMEMORYPOOLFACTORY2_H__
#pragma once

// SafeMemoryPoolFactory.h: interface for the CMemoryPoolFactory class.
// Memory Pool Management Class version 1.01
// source code created by Min-Wook Kim (taiyo@webzen.co.kr)
// 2004-1-10
// 
//-------------------------------------------------------------------
// @History.
//  2004-3-31	:	MemoryPoolTempl class ver 0.1
//  2004-6-18	:	MemoryPoolTempl class ver 0.2
//  2004-6-21    :	Error Check Byte added ver 0.21
//  2004-7-26    :	MemoryPool Framework ver1.0
//  2005-1-10    :	SafeMemoryPoolFactory class added 1.01
//  2005-3-28    :	set default value as dwPoolExtendSize = dwPoolBasicSize/2+1
//  ----------- change implementation paradigm milestone ------------
//  2009-9-10    :   changes to zero cleared stream object pool (v0200), update lst1024, guide waverix
//  2010-1-23    :   update to v0210, boost up compile speed, and reduce binary size
//                   add logging handler to report, due to waverix
// Be Done
// - MemoryPool template class of inherited-method and factory-method
// - Multi-thread safety
// -- (WARNING) change to single thread support version based on usage case research, since v0200
// - Buffer overflow write check
// To do
// - Analysis of amount used for band
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

#define SAFE_MEMORY_POOL_VERSION            0x0220


#if 0

#elif SAFE_MEMORY_POOL_VERSION >= 0x0220

// NOTE: f110429.8L, automatic macros don't support anymore,
// if major sample need, show the QueryPoolFactoy decribed in DBProxy

#elif SAFE_MEMORY_POOL_VERSION >= 0x0210

//--------------------------------------------------------------------------------------------------
//  10/01/23|waverix|add SetName to support logging
#define __DECL_SAFECUSTOMPOOL_PTR(_ClassType)                           \
      typedef util::SafeMemoryPoolFactory<_ClassType>* CUSTOM_POOL_PTR; \
    private:                                                            \
      static CUSTOM_POOL_PTR s_pPool;                                   \
    public:                                                             \
      static void SetPool(CUSTOM_POOL_PTR spPool) {         \
        s_pPool = spPool; s_pPool->SetName(#_ClassType);    \
      } \
    static _ClassType* ALLOC() { return s_pPool->Alloc(); } \
    static std::pair<DWORD, DWORD> GetSizeInfo() {          \
        return std::pair<DWORD, DWORD>(s_pPool->GetNumberOfBands(),           \
                                       s_pPool->GetAvailableNumberOfTypes()); \
    } \
    static VOID FREE( _ClassType * pClass ) { s_pPool->Free( pClass ); }
//  static VOID DisplayerPoolInfo()                                     \
//{                                                                 \
//  DISPMSG( "[%4u,%4u][band:%u node:%u]", s_pPool->GetPoolBasicSize(), s_pPool->GetPoolExtendSize(),   \
//  s_pPool->GetNumberOfBands(), s_pPool->GetAvailableNumberOfTypes() );    \
//}

#define __IMPL_SAFECUSTOMPOOL_PTR(_ClassType)                           \
    _ClassType::CUSTOM_POOL_PTR _ClassType::s_pPool = NULL;

#else
//--------------------------------------------------------------------------------------------------
#define __DECL_SAFECUSTOMPOOL_PTR(_ClassType)							\
	typedef util::SafeMemoryPoolFactory<_ClassType> * CUSTOM_POOL_PTR;	\
	private:															\
	static CUSTOM_POOL_PTR s_pPool;										\
	public:																\
	static VOID SetPool( CUSTOM_POOL_PTR spPool ) { s_pPool = spPool; }	\
	static _ClassType *	ALLOC() { return s_pPool->Alloc(); }			\
	static std::pair<DWORD, DWORD> GetSizeInfo() {						\
		return std::pair<DWORD, DWORD>(									\
			s_pPool->GetNumberOfBands()									\
			, s_pPool->GetAvailableNumberOfTypes() );					\
	}																	\
	static VOID FREE( _ClassType * pClass ) { s_pPool->Free( pClass ); }
//	static VOID	DisplayerPoolInfo()										\
//{																	\
//	DISPMSG( "[%4u,%4u][band:%u node:%u]", s_pPool->GetPoolBasicSize(), s_pPool->GetPoolExtendSize(),	\
//	s_pPool->GetNumberOfBands(), s_pPool->GetAvailableNumberOfTypes() );	\
//}

#define __IMPL_SAFECUSTOMPOOL_PTR(_ClassType)							\
	_ClassType::CUSTOM_POOL_PTR _ClassType::s_pPool = NULL;

#endif

//==================================================================================================
//==================================================================================================
//==================================================================================================

#if 0

//==================================================================================================
#elif SAFE_MEMORY_POOL_VERSION >= 0x0210
//==================================================================================================

namespace util {

class SafeMemoryPoolFactoryShared
{
protected:
    typedef void* node_type_ptr;
    struct NodeContainer {
        BOOLEAN aquired_;
        // reserved 1,2,3 slot
        node_type_ptr node_ptr_;
    };

public:
    SafeMemoryPoolFactoryShared(const size_t type_size);
    ~SafeMemoryPoolFactoryShared();
    void Initialize(size_t chunk_size);
    node_type_ptr Alloc();
    BOOLEAN Free(node_type_ptr ptr);
    DWORD GetNumberOfBands() const;
    DWORD GetAvailableNumberOfTypes() const;
    void SetName(const char* type_name);
    const char* GetName() const;

    typedef void (*_LoggingHandlerFunc)(const TCHAR* type_name, void* mem_ptr, size_t mem_size);
    static void RegisterLoggingHandler(_LoggingHandlerFunc handler);

private:
    SafeMemoryPoolFactoryShared& operator=(const SafeMemoryPoolFactoryShared& r);
    void ExpandSize(size_t add_size);
    static void Log(const TCHAR* type_name, void* mem_ptr, size_t mem_size);

    typedef STLX_DEQUE<NodeContainer*> QueueingPool;
    typedef STLX_HASH_MAP<node_type_ptr, NodeContainer> QueueHash;
    QueueingPool free_queue_;
    QueueHash total_list_;
    const size_t type_size_;
    size_t chunk_size_;
    const char* type_name_;
    static _LoggingHandlerFunc log_handler_static_;
};

inline const char* SafeMemoryPoolFactoryShared::GetName() const {
    return type_name_;
};

//--------------------------------------------------------------------------------------------------

template<typename ClassType_>
class SafeMemoryPoolFactory : public SafeMemoryPoolFactoryShared
{
    typedef SafeMemoryPoolFactoryShared base_type;
    typedef ClassType_ node_type;
    typedef node_type* node_type_ptr;

public:
    SafeMemoryPoolFactory()
        : SafeMemoryPoolFactoryShared(sizeof(node_type))
    {
    }

    ~SafeMemoryPoolFactory()
    {
    }

    //void Initialize(size_t chunk_size)
    node_type_ptr Alloc()
    {
        base_type::node_type_ptr pointer = base_type::Alloc();
        assert(pointer != NULL); // pointer is zero cleared stream
        return new (pointer) node_type;
    }

    BOOLEAN Free(node_type_ptr ptr)
    {
        ptr->~node_type();
        return base_type::Free(ptr);
    }

    //DWORD GetNumberOfBands() const;
    //DWORD GetAvailableNumberOfTypes() const;
    //void SetName(const char* type_name);
    //private:
    //void ExpandSize(size_t add_size);
};

}; //end of namespace

//==================================================================================================
#elif SAFE_MEMORY_POOL_VERSION >= 0x0200
//==================================================================================================

namespace util {

template<typename ClassType_>
class SafeMemoryPoolFactory
{
    typedef ClassType_  NodeType;
    typedef NodeType*   NodePtr;
    struct NodeContainer {
        BOOLEAN aquired_;
        // reserved 1,2,3 slot
        NodePtr node_ptr_;
    };

    typedef STLX_DEQUE<NodeContainer*>   QueueingPool;
    typedef STLX_HASH_MAP<NodePtr, NodeContainer>   QueueHash;

public:
    SafeMemoryPoolFactory() : chunk_size_(0) {}

    ~SafeMemoryPoolFactory()
    {
        FOREACH_CONTAINER(const QueueHash::value_type& pair_node, total_list_, QueueHash)
        {
            const NodeContainer& node = pair_node.second;
            if(node.aquired_ != 0) {
                //리소스 반환하지 않은 사례가 있다.
                printf( "Release resource problem, alloc/free mismatch - Pool" );
                //                     SUNLOG(eCRITICAL_LOG, "Release resource problem, alloc/free mismatch - Pool{%s}",
                //                         pool_owner_ ? pool_owner_ : "");
            #if _DEBUG
                __debugbreak();
            #endif
            }

            if(node.node_ptr_)
                TAllocFree(node.node_ptr_);
        }
    }

    void Initialize( size_t chunk_size )
    {
        assert(chunk_size  != 0);
        assert(chunk_size_ == 0);

        chunk_size_ = chunk_size;
    }    

    NodePtr Alloc()
    {
        if(free_queue_.empty())
            ExpandSize(chunk_size_);

        NodeContainer* node = free_queue_.front();
        assert(node->aquired_ == 0 && node->node_ptr_ != 0);
        node->aquired_ = true;
        free_queue_.pop_front();

        ZeroMemory(node->node_ptr_, sizeof(*node->node_ptr_));
        return new (node->node_ptr_) NodeType;
    }

    BOOLEAN Free(NodePtr ptr)
    {
        QueueHash::iterator it = total_list_.find(ptr);
        if(FlowControl::FCAssert(it != total_list_.end())) {
            NodeContainer& node = it->second;
            assert(node.aquired_ != 0 && node.node_ptr_ == ptr);
            node.aquired_ = 0;

            node.node_ptr_->~NodeType();
            free_queue_.push_back(&node);
            return true;
        }
        return false;
    }

    DWORD GetNumberOfBands() const
    {
        return (DWORD)total_list_.size();
    }

    DWORD GetAvailableNumberOfTypes() const
    {
        return (DWORD)free_queue_.size();
    }

private:
    void ExpandSize(size_t add_size)
    {
        for(size_t loop = add_size; loop; --loop) {
            NodePtr token = (NodePtr)TAllocMalloc(sizeof(NodeType));
            NodeContainer& node = total_list_[token];
            node.aquired_ = 0;
            node.node_ptr_ = token;
            free_queue_.push_back(&node);
        }
    }

    QueueingPool free_queue_;
    QueueHash total_list_;
    size_t chunk_size_;
};

}; //end of namespace

//==================================================================================================
#else // covered version range = [0100, 0200)
//==================================================================================================

#include "MemBand.h"

//--------------------------------------------------------------------------------------------------

namespace util {

template <class Type>
class SafeMemoryPoolFactory
{
public:
	SafeMemoryPoolFactory()
		:	m_pBandHead( NULL ),
			m_pFreeBand( NULL ),
			m_dwPoolExtendSize( 0 ){}

	virtual ~SafeMemoryPoolFactory()
	{
		Release();
		DeleteCriticalSection( &m_cs );
	}
	BOOL Initialize( DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0 )
	{
		
		InitializeCriticalSection( &m_cs );

		if( 0 == dwPoolExtendSize )
			m_dwPoolExtendSize = dwPoolBasicSize/2+1;
		else
			m_dwPoolExtendSize = dwPoolExtendSize;

		if( NULL == m_pBandHead )
		{
			CMemTypeBand<Type>::AllocBand( m_pBandHead, dwPoolBasicSize );
			m_pFreeBand = m_pBandHead;
			if( !m_pFreeBand ) 
				return FALSE;
			return TRUE;
		}
		return FALSE;
	}


	void Release()
	{
		if( m_pBandHead )
		{
			CMemTypeBand<Type>::FreeBand( m_pBandHead );
			//m_pBandHead->FreeBand();
			m_pBandHead = NULL;
			m_pFreeBand = NULL;
		}
	}
	
	inline Type * Alloc()
	{
		lock();
		Type * pn = m_pFreeBand->AlloObject();
		if(pn == NULL)
		{
			if( !m_pFreeBand->pPrev )
			{
				CMemTypeBand<Type>::AllocBand( m_pBandHead, m_dwPoolExtendSize );
				m_pFreeBand = m_pBandHead;
				pn = m_pFreeBand->AlloObject();
			}
			else
			{
				m_pFreeBand = m_pFreeBand->pPrev;
				pn = m_pFreeBand->AlloObject();
			}
		}
		unlock();
		return pn;
	}
	inline void Free(Type * pNode)
	{
		lock();
		if(!m_pFreeBand->FreeObject(pNode))
		{
			m_pFreeBand = m_pFreeBand->pNext;
			if(!m_pFreeBand->FreeObject(pNode))
			{
				__debugbreak();
			}
		}
		unlock();
	}
	
	inline DWORD GetPoolBasicSize()
	{
		return m_pBandHead->GetMaxNumberOfObjects();
	}

	inline DWORD GetPoolExtendSize()
	{
		return m_dwPoolExtendSize;
	}

	inline DWORD GetNumberOfBands()
	{
		DWORD dwTotalNum = 0;
		CMemTypeBand<Type> *	pHead = m_pBandHead;
		while( pHead = pHead->pNext )
			++dwTotalNum;
		return dwTotalNum;
	}

	inline DWORD GetAvailableNumberOfTypes()
	{
		DWORD dwTotalNum = 0;
		CMemTypeBand<Type> * pHead = m_pBandHead;
		while( pHead = pHead->pNext )
			dwTotalNum += pHead->GetAvailableNumberOfObjects();
		return dwTotalNum;
	}
protected:

	inline void				lock() 		{ EnterCriticalSection(&m_cs); }
	inline void				unlock() 	{ LeaveCriticalSection(&m_cs); }

	CRITICAL_SECTION		m_cs;

	CMemTypeBand<Type> *	m_pBandHead;
	CMemTypeBand<Type> *	m_pFreeBand;
	DWORD					m_dwPoolExtendSize;
};

}; //end of namespace

#endif  //SAFE_MEMORY_POOL_VERSION

#endif // __SAFEMEMORYPOOLFACTORY2_H__