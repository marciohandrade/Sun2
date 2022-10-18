#pragma once
#ifndef UTILITY_MEMORY_POOL_ROUTER_H
#define UTILITY_MEMORY_POOL_ROUTER_H

#include "UtilityCommon.h"
#include "MemoryPoolFactory.h"

#define MEMORY_POOL_ROUTER_VERSION 0x0130

#if 0

//==================================================================================================
//==================================================================================================
//==================================================================================================
#elif MEMORY_POOL_ROUTER_VERSION >= 0x0130
//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace util {
;

template<size_t _NumberOfPoolNodes>
class PoolRouter
{
public:
    PoolRouter();
    ~PoolRouter();
    void Release();

    template<typename _TYPENAME_>
    bool Register(DWORD index);

    template<typename _POOLTYPE>
    bool RegisterPool(DWORD index);

    IVMemoryPool* GetPool(DWORD index) const;
    bool Initialize(DWORD index, DWORD init_size, DWORD chunk_size = 0,
                    const char* type_name = NULL, void* dummy = NULL);

    void* Alloc(DWORD index);
    bool Free(DWORD index, void* node);

private:
    IVMemoryPool* node_list_[_NumberOfPoolNodes];
};

}; //end of namespace

//==================================================================================================

template<size_t _NumberOfPoolNodes>
inline util::PoolRouter<_NumberOfPoolNodes>::PoolRouter()
{
    ZeroMemory(node_list_, sizeof(node_list_));
}

template<size_t _NumberOfPoolNodes>
inline util::PoolRouter<_NumberOfPoolNodes>::~PoolRouter()
{
    Release();
}

template<size_t _NumberOfPoolNodes>
void util::PoolRouter<_NumberOfPoolNodes>::Release()
{
    IVMemoryPool** it = node_list_,
                ** end = node_list_ + _countof(node_list_);
    for (; it != end; ++it) {
        if (*it != NULL) {
            delete (*it);
        };
    }
    ZeroMemory(node_list_, sizeof(node_list_));
}


template<size_t _NumberOfPoolNodes>
template<typename _TYPENAME_>
bool util::PoolRouter<_NumberOfPoolNodes>::Register(DWORD index)
{
    if (FlowControl::FCAssert(index < _NumberOfPoolNodes &&
                                node_list_[index] == NULL) == false)
    {
        return false;
    };

    IVMemoryPool* ipool = new util::CMemoryPoolFactory<_TYPENAME_>;
    node_list_[index] = ipool;
    return true;
}

template<size_t _NumberOfPoolNodes>
template<typename _POOLTYPE>
bool util::PoolRouter<_NumberOfPoolNodes>::RegisterPool(DWORD index)
{
    if (FlowControl::FCAssert(index < _NumberOfPoolNodes &&
                                node_list_[index] == NULL) == false)
    {
        return false;
    };

    IVMemoryPool* ipool = new _POOLTYPE;
    node_list_[index] = ipool;
    return true;
}

template<size_t _NumberOfPoolNodes>
inline util::IVMemoryPool*
util::PoolRouter<_NumberOfPoolNodes>::GetPool(DWORD index) const
{
    if (index < _NumberOfPoolNodes)
    {
        IVMemoryPool* ipool = node_list_[index];
        return ipool;
    }
    return NULL;
}

template<size_t _NumberOfPoolNodes>
bool util::PoolRouter<_NumberOfPoolNodes>::Initialize(
    DWORD index, DWORD init_size, DWORD chunk_size,
    const char* type_name, void* dummy)
{
    __UNUSED(dummy);
    if (index < _NumberOfPoolNodes)
    {
        IVMemoryPool*& ipool_ref = node_list_[index];
        if (ipool_ref) {
            return ipool_ref->Initialize(init_size, chunk_size, type_name);
        };
    }
    return false;
}

template<size_t _NumberOfPoolNodes>
inline void* util::PoolRouter<_NumberOfPoolNodes>::Alloc(DWORD index)
{
    if (index < _NumberOfPoolNodes)
    {
        IVMemoryPool*& ipool_ref = node_list_[index];
        if (ipool_ref) {
            return ipool_ref->Alloc();
        };
    }
    return NULL;
}

template<size_t _NumberOfPoolNodes>
inline bool util::PoolRouter<_NumberOfPoolNodes>::Free(DWORD index, void* node)
{
    if (index < _NumberOfPoolNodes)
    {
        IVMemoryPool*& ipool_ref = node_list_[index];
        if (ipool_ref) {
            return (ipool_ref->Free(node) != false);
        };
    }
    return false;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
#else MEMORY_POOL_ROUTER_VERSION >= 0x0100
//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace util
{
	// ========================================================================================
	//
	//
	template<DWORD dwNodeSize>
	class PoolRouter
	{
	public:
		PoolRouter()
		{
			memset( m_NodeList, 0, sizeof(m_NodeList) );
		}

		~PoolRouter()
		{
			Release();
		}

		VOID	Release()
		{
			for( DWORD i=0 ; i<dwNodeSize ; ++i )
			{
				SAFE_DELETE( m_NodeList[i] );
			}
		}

		template<typename _TYPENAME_>
		BOOL	Register( DWORD dwIndex )
		{
			if( dwIndex < dwNodeSize )
			{
				IVMemoryPool* pPool = new CMemoryPoolFactory<_TYPENAME_>;
				m_NodeList[dwIndex] = pPool;
				return TRUE;
			}
			return FALSE;
		}

		template<typename _POOLTYPE>
		BOOL	RegisterPool( DWORD dwIndex )
		{
			if( dwIndex < dwNodeSize )
			{
				IVMemoryPool* pPool = new _POOLTYPE;
				m_NodeList[dwIndex] = pPool;
				return TRUE;
			}
			return FALSE;
		}

		IVMemoryPool*	GetPool( DWORD dwIndex )
		{
			if( dwIndex < dwNodeSize )
			{
				IVMemoryPool* pPool = m_NodeList[dwIndex];
				return pPool;
			}
			return NULL;
		}

		BOOL	Initialize( DWORD dwIndex, DWORD dwPoolBasicSize, DWORD dwPoolExtendSize = 0, TCHAR* ptszTypeName = NULL, ePOOL_ERROR::AssertionLogger4Pool fnLOGGER = NULL )
		{
			if( dwIndex < dwNodeSize )
			{
				IVMemoryPool*& rPool = m_NodeList[dwIndex];
				if( rPool )
					return rPool->Initialize( dwPoolBasicSize, dwPoolExtendSize, ptszTypeName, fnLOGGER );
			}
			return FALSE;
		}

		PVOID	Alloc( DWORD dwIndex )
		{
			if( dwIndex < dwNodeSize )
			{
				IVMemoryPool*& rPool = m_NodeList[dwIndex];
				if( rPool )
					return rPool->Alloc();
			}
			return NULL;
		}

		BOOL	Free( DWORD dwIndex, PVOID pNode )
		{
			if( dwIndex < dwNodeSize )
			{
				IVMemoryPool*& rPool = m_NodeList[dwIndex];
				if( rPool )
					return rPool->Free( pNode );
			}
			return FALSE;
		}

	private:
		IVMemoryPool*	m_NodeList[dwNodeSize];
	};
	//
	//
	// ========================================================================================

} /// namespace util

//==================================================================================================
//==================================================================================================
//==================================================================================================
#endif //MEMORY_POOL_ROUTER_VERSION >= 0x0100
//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //UTILITY_MEMORY_POOL_ROUTER_H