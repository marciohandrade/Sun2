#ifndef __SUN_STL_TMP_DEF_UTILITY__
#define __SUN_STL_TMP_DEF_UTILITY__
#pragma once

//==================================================================================================
//
#include <boost/version.hpp>
#if _MSC_VER >= 1500 && BOOST_VERSION > 103700
  #include <boost/serialization/static_warning.hpp>
#else
  #include <boost/static_warning.hpp> // extracted from v1.34
#endif
#include <boost/type_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/placeholders.hpp>
namespace mpl = boost::mpl;
namespace holder = boost::mpl::placeholders;

//==================================================================================================
// MACROs
//  @note: code refactoring & unit testing supporter
//  @history:
//      - 10.01.13|waverix|add macros
#define SUN_CODE_BACKUP          (0)
#define SUN_CODE_SAMPLE          (0)
#define SUN_CODE_RENEWAL         (1)
#define SUN_CODE_DELETE_RESERVED (1)
#define SUN_CODE_TEST            (1)
// user defined
#if !defined(WAVERIX_WRAPPER)
    #define WAVERIX_WRAPPER(_code_block_)
    #define WAVERIX_TEST_BLOCK_IN_CLASS(_type_name_)
#endif
//==================================================================================================
//
namespace util {

//==================================================================================================
//  Generic Template Meta-Programming
//  Author : Waverix
//  Remark : 흔하게 사용되는 TMP계열 정의

template<typename DWORD _COUNT>
struct _BIT_SHIFT
{
    BOOST_STATIC_ASSERT(_COUNT > 0);
    static const DWORD BITSHIFT = 1 + (_BIT_SHIFT< (_COUNT >> 1) >::BITSHIFT);
};

template<>
struct _BIT_SHIFT<1>
{
    static const DWORD BITSHIFT = 0;
};

template< typename _TYPE, DWORD _BEGIN, DWORD _END >
struct ARRAY_TYPEINFO
{
    typedef _TYPE type;
    BOOST_STATIC_ASSERT(_BEGIN <= _END);
    static const DWORD begin_  = _BEGIN;
    static const DWORD end_    = _END;
    static const DWORD length_ = end_-begin_;
    typedef type                array_type[length_];
    static const DWORD size_   = sizeof(array_type);
};


//==================================================================================================
//
namespace internal { // util::internal
//  STL Container Type traits Wrapper
//

#if SUNSTL_VERSION_CTRL <= 0x0110
template<typename _CONTAINER>
struct STL_CONTAINER_WRAPPER
{
    typedef typename _CONTAINER::value_type value_type;
    typedef typename _CONTAINER::reference  reference;

    struct iterator_wrap_const
    {
        typedef typename _CONTAINER::iterator       iterator_base;
        typedef typename _CONTAINER::const_iterator const_iterator_base;
        iterator_wrap_const(iterator_base& it) : itr(it) {};
        iterator_wrap_const(const_iterator_base& it) : itr((iterator_base&)it) {};
        operator bool() const { return false; }
        mutable iterator_base itr;
    };

    struct iterator_wrap : iterator_wrap_const
    {
        iterator_wrap(iterator_base& it) : iterator_wrap_const(it) {};
        iterator_wrap(const_iterator_base& it) : iterator_wrap_const((iterator_base&)it) {};
    };

    typedef iterator_wrap iterator;

    iterator begin(const _CONTAINER& rContainer) const { return rContainer.begin(); }
    iterator end(const _CONTAINER& rContainer) const { return rContainer.end(); }

    iterator begin(_CONTAINER& rContainer) const { return rContainer.begin(); }
    iterator end(_CONTAINER& rContainer) const { return rContainer.end(); }
};

template<typename _CONTAINER>
STL_CONTAINER_WRAPPER<_CONTAINER>*
make_container_wrapper(const _CONTAINER& rC) { return 0; }

#elif SUNSTL_VERSION_CTRL > 0x0110

template<typename _CONTAINER>
struct STL_CONTAINER_WRAPPER
{
    typedef _CONTAINER container;
    struct iterator_wrap
    {
        typedef typename container::iterator       iterator_base;
        typedef typename container::const_iterator const_iterator_base;
        iterator_wrap(iterator_base& rIt) : itr(rIt) {};
        iterator_wrap(const const_iterator_base& rIt) : itr((iterator_base&)rIt) {};
        __forceinline operator  bool() const { return false; }
        mutable iterator_base itr;
    };
    typedef iterator_wrap  iterator;
#pragma warning(push, 3)
    inline static iterator begin(const container& rContainer) {
        return rContainer.begin();
    }
    inline static iterator end(const container& rContainer) {
        return rContainer.end();
    }
#pragma warning(pop)
};

template<typename _CONTAINER>
STL_CONTAINER_WRAPPER<_CONTAINER>*
make_container_wrapper(const _CONTAINER& rC) { return 0; }

#endif //SUNSTL_VERSION_CTRL version comparison

//--------------------------------------------------------------------------------------------------



template< typename _DTYPE, typename _BTYPE >
inline static _DTYPE
downcaster_select_impl(_BTYPE& r, boost::mpl::true_)
{
    return dynamic_cast<_DTYPE>(r);
}

template< typename _DTYPE, typename _BTYPE >
inline static _DTYPE
downcaster_select_impl(_BTYPE& r, boost::mpl::false_)
{
    return static_cast<_DTYPE>(r);
}

}; //end of namespace 'util::internal'

//--------------------------------------------------------------------------------------------------

template< typename _DTYPE, typename _BTYPE >
inline static _DTYPE
downcaster_select(_BTYPE pBtype)
{
#ifdef _DEBUG
    return internal::downcaster_select_impl< _DTYPE, _BTYPE >(pBtype,
                boost::mpl::and_<
                    boost::is_polymorphic< boost::remove_pointer<_DTYPE>::type >::type,
                    boost::is_polymorphic< boost::remove_pointer<_BTYPE>::type >::type >());
#else
    return internal::downcaster_select_impl< _DTYPE, _BTYPE >(pBtype, boost::mpl::false_());
#endif
}

//--------------------------------------------------------------------------------------------------
template<class LT, class RT>
inline void AssertCompileTimeVariableTypeEqual(const LT&, const RT&) {
    BOOST_STATIC_ASSERT((boost::is_same<LT, RT>::value));
}

}; //end namespace
//
//==================================================================================================

#endif //__SUN_STL_TMP_DEF_UTILITY__
