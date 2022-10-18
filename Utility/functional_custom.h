#ifndef UTILITY_FUNCTIONAL_CUSTOM_H
#define UTILITY_FUNCTIONAL_CUSTOM_H
#pragma once

#include <functional>

#pragma warning(push)
#pragma warning(disable : 4510 4610)

// stlport custom code

template <class _Operation>
struct __unary_fun_aux : private _Operation
{
	typedef typename _Operation::argument_type argument_type;
	typedef typename _Operation::result_type result_type;
};

#  define __UNARY_ARG(__Operation,__type)  __unary_fun_aux<__Operation>::__type

template <class _Operation1, class _Operation2>
class unary_compose : 
    public std::unary_function<typename __UNARY_ARG(_Operation2,argument_type),
	typename __UNARY_ARG(_Operation1,result_type)> {
protected:
	_Operation1 _M_fn1;
	_Operation2 _M_fn2;
public:
	unary_compose(const _Operation1& __x, const _Operation2& __y) 
		: _M_fn1(__x), _M_fn2(__y) {}

	typename _Operation1::result_type
	operator()(const typename _Operation2::argument_type& __x) const {
		return _M_fn1(_M_fn2(__x));
	}

	//// const 주석 처리
	//// operator내부에서 맴버를 수정할 수 없어서
	//typename _Operation1::result_type
	//operator()(typename _Operation2::argument_type& __x) /*const*/ {
	//	return _M_fn1(_M_fn2(__x));
	//}
};

// com(g,f) -> g(f(x))
template <class _Operation1, class _Operation2>
inline unary_compose<_Operation1,_Operation2> 
compose1(const _Operation1& __fn1, const _Operation2& __fn2)
{
	return unary_compose<_Operation1,_Operation2>(__fn1, __fn2);
}

template <class _Pair>
struct _Select1st : public std::unary_function<_Pair, typename _Pair::first_type> {
	const typename _Pair::first_type& operator()(const _Pair& __x) const {
		return __x.first;
	}
};

template <class _Pair>
struct _Select2nd : public std::unary_function<_Pair, typename _Pair::second_type>
{
	const typename _Pair::second_type& operator()(const _Pair& __x) const {
		return __x.second;
	}
};

template <class _Pair> struct select1st : public _Select1st<_Pair> {};
template <class _Pair> struct select2nd : public _Select2nd<_Pair> {};


//example
/*std::for_each( m_pZoneHash->begin(), end = m_pZoneHash->end(), 
compose1( mem_fun(&GameZone::Release), select2nd<GAMEZONE_HASH::value_type>() ));*/

#pragma warning(pop)

#endif //UTILITY_FUNCTIONAL_CUSTOM_H