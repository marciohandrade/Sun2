#pragma once
#ifndef NETWORK_MODULE_STDAFX_H
#define NETWORK_MODULE_STDAFX_H

//==================================================================================================
#define WIN32_LEAN_AND_MEAN

#ifndef _WIN32_WINNT
#   define _WIN32_WINNT 0x0501 
#endif
#ifndef WINVER
#   define WINVER 0x0501
#endif

//==================================================================================================
// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../../include/Global.Define.h"

//==================================================================================================
//  <PRE-PROCESSOR>
//  <__WAVERIX_071217_NETWORK_REFACTORING__>
//      - Fault Control 계열을 중심으로 이와 관련된 로직들 전면 검증 및 재구성
//      - Control Mechanism - Assert Logic based control
//      - 더 이상 사용되지 않는 로직 제거
//#define __WAVERIX_071217_NETWORK_REFACTORING__

#define WHATTHEHELL_NETWORK_20060519
//#define __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110     // Remove()시 로그 남기기 _DEBUG 제거

//==================================================================================================
//  <HEADERs>
#include <windows.h>
#include <winsock2.h>
#include <mswsock.h>

#include <assert.h>
#include <iostream>

#include <boost/static_assert.hpp>

#include "./WxRingBuffer.h"
#pragma warning(push, 4)
//==================================================================================================
//  <HEADERs><INTERNAL>
#include "NetworkDefine.h"
#include "NetBase.h"
//#include "RingBufferEx.h"

// export to compatible an usage
//using module::RingBufferEx;
//using module::CCriticalSection;
//using module::CSyncCriticalSection;
//
//==================================================================================================
//  <LINKED LIBRARY>    IOCP
#pragma comment(lib, "ws2_32.lib")

#if !defined(SAFE_DELETE)
    #define SAFE_DELETE(ptr) \
        ;{ if (ptr) { \
                delete (ptr); \
                (ptr) = NULL; \
           } \
        };
#endif //
#if !defined(__UNUSED)
    #define __UNUSED(vars)  vars
#endif

// inter memory utility to support a seperated alloc/dealloc
namespace module {
;
template<class _Type>
struct base_type_info
{
    typedef _Type             value_type;
    typedef value_type*       pointer;
    typedef value_type* const pointer_r;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
};

template<class _Type>
struct TAllocator : base_type_info<_Type>
{
    typedef TAllocator<value_type> allocator_type;
    static pointer Allocate()
    {
        void* ptr = ::malloc(sizeof(value_type));
        return new (ptr) value_type();
    }

    template<class _DerivedType>
    static void DeAllocate(_DerivedType* ptr)
    {
        // compile-time check routine
        static_cast<_DerivedType*>(reinterpret_cast<value_type*>(1));
        ptr->~_DrivedType();
        ::free(ptr);
    } 

    static void DeAllocate(pointer ptr)
    {
        ptr->_Type::~_Type();
        ::free(ptr);
    }
};

template<typename T>
class RedirectedAllocator
{
    typedef RedirectedAllocator<T> allocator_local_type;
    typedef T         node_type;
public:
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T         value_type;

    template<typename Other>
    struct rebind
    {
        typedef RedirectedAllocator<Other> other;
    };

    pointer       address(reference x) const       { return &x; }
    const_pointer address(const_reference x) const { return &x; }

    RedirectedAllocator()
    {
    }

    template<typename T1>
    RedirectedAllocator(const RedirectedAllocator<T1>&)
    {
    }

    allocator_local_type& operator=(const allocator_local_type&)
    {
        return *this;
    }

    pointer allocate(size_type n, const void* hint = 0)
    {
        __UNUSED(hint);
        return reinterpret_cast<pointer>(::malloc(sizeof(T) * n));
    }

    void deallocate(void* p, size_type n)
    {
        __UNUSED(n);
        ::free(p);
    }

    void construct(pointer p, const T& val)
    {
        new ((void *)p) T(val);
    }

    void destroy(pointer p)
    {
        p->T::~T();
    }

    size_type max_size() const
    {
        size_type n = (size_type)(-1) / sizeof(node_type);
        return (0 < n ? n : 1);
    }

    bool operator == (const allocator_local_type& r) const
    {
        return this == &r;
    }

    bool operator != (const allocator_local_type& r) const
    {
        return this != &r;
    }

    bool operator < (const allocator_local_type&) const;
    bool operator > (const allocator_local_type&) const;

private:
};

}; //end of namespace

#endif //NETWORK_MODULE_STDAFX_H
