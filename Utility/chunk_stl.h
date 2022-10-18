#pragma once
#ifndef UTILITY_CHUNK_STL_H
#define UTILITY_CHUNK_STL_H

//==================================================================================================
//  <CHUNK_STLs>
//  <HISTORY>
//      07/09/07 - CheckIn (Waverix)
//      11.05.13 - apply logic both client and server

//==================================================================================================
// <DEFINITIONs>

#include <stddef.h>
#include <assert.h>

#if defined(_MSC_VER)
#   if _MSC_VER < 1300
#       error "Can't Support Chunk Memory STL"
#   endif
#else
#   error "Not Test Current Compiler"
#endif

#include <vector>
#include <deque>
#include <string>
#include <list>
#include <set>
#include <map>
#include <hash_set>
#include <hash_map>
#include <boost/pool/pool_alloc.hpp>
#include "SharedMemoryPool.h"

namespace util {
#pragma pack(push, 8)
;
//==================================================================================================
// <CHUNK ALLOCATOR>
//
template<typename Data_T>
class CustomAllocator
{
public:
    typedef CustomAllocator<Data_T> this_type;
    //
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    //
    typedef Data_T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    //
    template<typename _Other>
    struct rebind
    {
        typedef CustomAllocator<_Other> other;
    };
    //
    CustomAllocator() {}
    template<typename Data_T2>
    CustomAllocator(const CustomAllocator<Data_T2>&) {}
    this_type& operator=(const this_type&) { return *this; }
    //
    pointer address(reference x) const;
    const_pointer address(const_reference x) const;
    //
    pointer allocate(size_type n, const void* hint = NULL);
    void deallocate(void* p, size_type n);
    void construct(pointer p, const value_type& value);
    void destroy(pointer p);
    size_type max_size() const;
    //
private:
    bool operator<(const this_type&) const;
    bool operator>(const this_type&) const;
}; //end of class

//==================================================================================================
// inline implements

template<typename Data_T>
inline typename CustomAllocator<Data_T>::pointer
CustomAllocator<Data_T>::address(reference x) const { return &x; };

template<typename Data_T>
inline typename CustomAllocator<Data_T>::const_pointer
CustomAllocator<Data_T>::address(const_reference x) const { return &x; };

template<typename Data_T>
inline typename CustomAllocator<Data_T>::pointer
CustomAllocator<Data_T>::allocate(size_type n, const void* hint)
{
    (hint);
    return reinterpret_cast<pointer>(TAllocMalloc(sizeof(Data_T) * n));
}

template<typename Data_T>
inline void
CustomAllocator<Data_T>::deallocate(void* p, size_type n)
{
    (n);
    TAllocFree(p);
}

template<typename Data_T>
inline void
CustomAllocator<Data_T>::construct(pointer p, const value_type& value)
{
#pragma push_macro("new")
#undef new
    new ((void*)p) Data_T(value);
#pragma pop_macro("new")
}

template<typename Data_T>
inline void
CustomAllocator<Data_T>::destroy(pointer ptr)
{
    ptr->Data_T::~Data_T();
}

template<typename Data_T>
inline typename CustomAllocator<Data_T>::size_type
CustomAllocator<Data_T>::max_size() const
{
    size_type n = size_type(-1) / sizeof(value_type);
    return (0 < n ? n : 1);
}

template<typename _LeftT, typename _RightT>
__forceinline bool operator==(const CustomAllocator<_LeftT>&, const CustomAllocator<_RightT>&)
{
    // CHANGES: f110519.1L, f110518.7L, changes always true to support more efficient swap
    return true;
}

template<typename _LeftT, typename _RightT>
__forceinline bool operator!=(const CustomAllocator<_LeftT>&, const CustomAllocator<_RightT>&)
{
    return false;
}

template < typename T >
struct ContainerAllocator
{
//#ifdef USE_FIXED_SIZE_MEMORY_POOL
//    typedef boost::pool_allocator<T> Type;
//#else
    typedef CustomAllocator<T> Type;
//#endif
};

//==================================================================================================
// <chunk_vector>
//  chunk memory version of vector
//
template< typename T >
class chunk_vector : public std::vector< T, typename ContainerAllocator<T>::Type >
{
    typedef typename std::vector< T, typename ContainerAllocator<T>::Type > super;

public:
    typedef typename super::allocator_type allocator_type;
    typedef typename super::size_type      size_type;
    typedef typename super::const_iterator const_iterator;

    explicit chunk_vector(const allocator_type& al = allocator_type()) : super(al)
    {
    }

    explicit chunk_vector(
        size_type n, const T& v = T(),
        const allocator_type& al = allocator_type())
        : super(n, v, al)
    {
    }

    chunk_vector(
        const_iterator first, const_iterator last,
        const allocator_type& al = allocator_type())
        : super(first, last, al)
    {
    }
};

//==================================================================================================
// <chunk_deque>
//  chunk memory version of dequeue
//
template< typename T >
class chunk_deque : public std::deque< T, typename ContainerAllocator<T>::Type >
{
    typedef typename std::deque< T, typename ContainerAllocator<T>::Type > super;

public:
    typedef typename super::allocator_type allocator_type;
    typedef typename super::size_type      size_type;
    typedef typename super::const_iterator const_iterator;

    explicit chunk_deque(const allocator_type& al = allocator_type()) : super(al)
    {
    }

    explicit chunk_deque(
        size_type n, const T& v = T(),
        const allocator_type& al = allocator_type())
        : super(n, v, al)
    {
    }

    chunk_deque(
        const_iterator first, const_iterator last,
        const allocator_type& al = allocator_type())
        : super(first, last, al)
    {
    }
};

//==================================================================================================
// string

typedef std::basic_string<
    char, 
    std::char_traits<char>, 
    typename ContainerAllocator<char>::Type 
> chunk_string;
typedef std::basic_string<
    wchar_t, 
    std::char_traits<wchar_t>, 
    typename ContainerAllocator<wchar_t>::Type 
> chunk_wstring;

//==================================================================================================
// <chunk_list>
//  chunk memory version of list
//
template< typename T >
class chunk_list : public std::list<T, typename ContainerAllocator<T>::Type >
{
    typedef typename std::list< T, typename ContainerAllocator<T>::Type > super;

public:
    typedef typename super::allocator_type allocator_type;
    typedef typename super::size_type      size_type;
    typedef typename super::const_iterator const_iterator;

    explicit chunk_list(const allocator_type& al = allocator_type()) : super(al)
    {
    }

    explicit chunk_list(
        size_type n, const T& v = T(),
        const allocator_type& al = allocator_type())
        : super(n, v, al)
    {
    }

    chunk_list(
        const_iterator first, const_iterator last,
        const allocator_type& al = allocator_type())
        : super(first, last, al)
    {
    }
};

//==================================================================================================
// <chunk_set>
//  chunk memory version of set
//
//#pragma warning(disable:4786)
//#include <set>

template < typename Key, typename Pred = std::less<Key> >
class chunk_set : public std::set< Key, Pred, typename ContainerAllocator<Key>::Type >
{
    typedef typename std::set< Key, Pred, typename ContainerAllocator<Key>::Type > super;

public:
    typedef typename super::allocator_type  allocator_type;
    typedef typename super::value_type      value_type;

    explicit chunk_set(
        const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(comp, al)
    {
    }

    chunk_set(
        const value_type* first, const value_type* last, const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(first, last, comp, al)
    {
    }
};


//==================================================================================================
// <chunk_multiset>
//  chunk memory version of multiset
//
//#pragma warning(disable:4786)
//#include <set>

template< typename Key, typename Pred=std::less<Key> >
class chunk_multiset : public std::multiset< Key, Pred, typename ContainerAllocator<Key>::Type >
{
    typedef typename std::multiset< Key, Pred, typename ContainerAllocator<Key>::Type > super;

public:
    typedef typename super::allocator_type allocator_type;
    typedef typename super::value_type     value_type;

    explicit chunk_multiset(
        const Pred& comp = Pred(),
        const allocator_type& al = allocator_type()) : super(comp,al)
    {
    }

    chunk_multiset(
        const value_type* first,const value_type* last, const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(first, last, comp, al)
    {
    }
};



//==================================================================================================
// <chunk_map>
//  chunk memory version of map
//
//#pragma warning(disable:4786)
//#include <map>

template< typename Key, typename T, typename Pred = std::less<Key> >
class chunk_map : public std::map< Key, T, Pred, typename ContainerAllocator<T>::Type >
{
    typedef typename std::map< Key, T, Pred, typename ContainerAllocator<T>::Type > super;

public:
    typedef typename super::allocator_type  allocator_type;
    typedef typename super::value_type      value_type;

    explicit chunk_map(
        const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(comp, al)
    {
    }

    chunk_map(
        const value_type *first, const value_type *last, const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(first, last, comp, al)
    {
    }
};


//==================================================================================================
// <chunk_multimap>
//  chunk memory version of multimap
//
//#pragma warning(disable:4786)
//#include <map>

template< typename Key, typename T, typename Pred=std::less<Key> >
class chunk_multimap : public std::multimap< Key, T, Pred, typename ContainerAllocator<T>::Type >
{
    typedef typename std::multimap< Key, T, Pred, typename ContainerAllocator<T>::Type > super;

public:
    typedef typename super::allocator_type allocator_type;
    typedef typename super::value_type     value_type;

    explicit chunk_multimap(
        const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(comp, al)
    {
    }

    chunk_multimap(
        const value_type* first, const value_type* last, const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(first, last, comp, al)
    {
    }
};

//==================================================================================================
// <chunk_hash_set>
//  chunk memory version of hash_set
//
//#pragma warning(disable:4786)
//#include <hash_set>

template< typename Key, typename Pred=_STDEXT hash_compare<Key, std::less<Key> > >
class chunk_hash_set : public stdext::hash_set< Key, Pred, typename ContainerAllocator<Key>::Type >
{
    typedef typename stdext::hash_set< Key, Pred, typename ContainerAllocator<Key>::Type > super;

public:
    typedef typename super::allocator_type allocator_type;
    typedef typename super::value_type     value_type;

    explicit chunk_hash_set(
        const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(comp, al)
    {
    }

    chunk_hash_set(
        const value_type* first, const value_type* last, const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(first, last, comp, al)
    {
    }
};

//==================================================================================================
//  <chunk_hash_map>
//      chunk memory version of hash_map
//
//#pragma warning(disable:4786)
//#include <hash_map>

template< typename Key, typename T, typename Pred=_STDEXT hash_compare<Key, std::less<Key> > >
class chunk_hash_map : public stdext::hash_map< Key, T, Pred, typename ContainerAllocator<T>::Type >
{
    typedef typename stdext::hash_map< Key, T, Pred, typename ContainerAllocator<T>::Type > super;

public:
    typedef typename super::allocator_type allocator_type;
    typedef typename super::value_type     value_type;

    explicit chunk_hash_map(
        const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(comp, al)
    {
    }

    chunk_hash_map(
        const value_type* first, const value_type* last, const Pred& comp = Pred(),
        const allocator_type& al = allocator_type())
        : super(first, last, comp, al)
    {
    }
};

#pragma pack(pop)
}; // namespace util

//==================================================================================================
//

#if defined(_SERVER)
    #define USE_CHUNK_STL_
#endif
#ifdef USE_CHUNK_STL_

#define STLX_VECTOR      ::util::chunk_vector
#define STLX_DEQUE       ::util::chunk_deque
#define STLX_STRING      ::util::chunk_string
#define STLX_WSTRING     ::util::chunk_wstring
#define STLX_LIST        ::util::chunk_list
#define STLX_SET         ::util::chunk_set
#define STLX_MULTISET    ::util::chunk_multiset
#define STLX_MAP         ::util::chunk_map
#define STLX_MULTIMAP    ::util::chunk_multimap
#define STLX_HASH_SET    ::util::chunk_hash_set
#define STLX_HASH_MAP    ::util::chunk_hash_map
#ifdef _UNICODE
#define STLX_TSTRING ::util::chunk_wstring
#else
#define STLX_TSTRING ::util::chunk_string
#endif

#else // USE_CHUNK_STL

#define STLX_VECTOR std::vector
#define STLX_DEQUE std::deque
#define STLX_STRING std::string
#define STLX_WSTRING std::wstring
#define STLX_LIST std::list
#define STLX_SET std::set
#define STLX_MULTISET std::multiset
#define STLX_MAP std::map
#define STLX_MULTIMAP std::multimap
#define STLX_HASH_SET stdext::hash_set
#define STLX_HASH_MAP stdext::hash_map
#ifdef _UNICODE
#define STLX_TSTRING std::wstring
#else
#define STLX_TSTRING std::string
#endif

#endif // USE_CHUNK_STL_

//==================================================================================================
//
#include "SunSTL.Utiltiy.h"

//==================================================================================================

#endif //UTILITY_CHUNK_STL_H