#ifndef __SUN_STL_TMP_UTILITY__
#define __SUN_STL_TMP_UTILITY__
#pragma once

//==================================================================================================
//	<VERSION CONTROL INFO>
//	-	0x0100
//	-	0x0110	FOREACH_CONTAINER, FOREACH_ARRAY
//	-	0x0120	FOREACH_CONTAINER, FOREACH_ARRAY ->
//              Update (reason: more simple, for passing strict compile)
//  -   0X0130  revision code from boost format
#define SUNSTL_VERSION_CTRL		0x0130

#include "SunSTL.UtiltiyTMP.Def.h"

namespace util
{
;
//==================================================================================================
//  boost 형식에 맞춘 소문자 기반 메타 언어
//  (reference WxFramework 09.09.01)
//==================================================================================================

//--------------------------------------------------------------------------------------------------
//  @name  bit_count_
//  @code  
//    bit_count_<4>::value        ==> 1
//    bit_count_<7>::value        ==> 3
//    bit_count_<15>::value      ==> 4
//    bit_count_<0x1030F070>::value  ==>10
template<DWORD value__>
struct bit_count_
{
    static const DWORD value = !!(value__ & 1) + bit_count_<(value__ >> 1)>::value;
};

template<>
struct bit_count_<0>
{
    static const DWORD value = 0;
};

//--------------------------------------------------------------------------------------------------
//  ()
//  @name  bit_range_
//  @code  
//  -  bit_range_mask_<9>::value   ==> 00000000 | 00000000 | 00000001 | 11111111
//  -  bit_range_mask_<31>::value  ==> 01111111 | 11111111 | 11111111 | 11111111
//  -  bit_count_<bit_range_<9>::value>::value ==> 9
template<DWORD number_of_bits_>
struct bit_range_mask_
{
    static const DWORD value  = (1 << number_of_bits_) - 1;
};

template<>
struct bit_range_mask_<32>
{
    static const DWORD value = DWORD(-1);
};

//--------------------------------------------------------------------------------------------------
//  ()
//  @name  bit_mask_c_
//  @code  
//  -  bit_mask_c_<0x80000081>::value  ==> (2) 10000000 | 00000000 | 00000000 | 10000001
template<DWORD bit_fields_ = 0>
struct bit_mask_c_
{
    static const DWORD value = bit_fields_;
};
//
//
//--------------------------------------------------------------------------------------------------
//  @name  bit_mask_add_
//  usage :
//  -  typedef bit_mask_c_<0x80000081>  original_mask  (#1)
//  -  typedef bit_mask_c_<0x00001031>  add_mask    (#2)
//
//  -  original_mask::value  ==> (2) 10000000 | 00000000 | 00000000 | 10000001
//  -  add_mask::value      ==> (2) 00000000 | 00000000 | 00010000 | 00110001
//  -  bit_mask_add_<original_mask, add_mask>::value  -> 0x800010B1
//                ==> (2) 10000000 | 00000000 | 00010000 | 10110001
//
template<typename _orig_mask, typename _add_mask>
struct bit_mask_add_
{
    static const DWORD value = (_orig_mask::value | _add_mask::value);
};

//--------------------------------------------------------------------------------------------------
//  @name bit_mask_sub_
//  usage :
//  -  original_mask          (#1)
//  -  typedef add_mask  sub_mask  (#2)
//
//  -  original_mask::value  ==> (2) 10000000 | 00000000 | 00000000 | 10000001
//  -  sub_mask::value      ==> (2) 00000000 | 00000000 | 00010000 | 00110001
//  -  bit_mask_sub_<original_mask, sub_mask>::value
//                ==> (2) 10000000 | 00000000 | 00000000 | 10000000
//
template<typename _orig_mask, typename _sub_mask>
struct bit_mask_sub_
{
    static const DWORD value  = (_orig_mask::value & (~_sub_mask::value));
};

//==================================================================================================
//==================================================================================================
//==================================================================================================
//  (WAVERIX) (090907) (NOTE) 하기 내용은 일괄 변형하기로 한다. (예정)
//

//==================================================================================================
//	<Bit Offset>
//	Name: BIT_OFFSET
//	Usage :
//	-	BIT_OFFSET<  0 >::value		==> 0
//	-	BIT_OFFSET<  0 >::bit		==> (2) 00000000 | 00000000 | 00000000 | 00000001
//	-	BIT_OFFSET<  7 >::value		==> 7
//	-	BIT_OFFSET<  7 >::bit		==> (2) 00000000 | 00000000 | 00000000 | 10000000
//	-	<MAX>
//	-	BIT_OFFSET< 31 >::value		==> 31
//	-	BIT_OFFSET< 31 >::bit		==> (2) 10000000 | 00000000 | 00000000 | 00000000
template< DWORD _FIELD >
struct BIT_OFFSET
{
	BOOST_STATIC_ASSERT( (_FIELD < 32) );
	static const DWORD		value		= _FIELD;
	static const DWORD		bit			= 1UL<<value;
};

template<>
struct BIT_OFFSET<32>
{
	static const DWORD		value		= 32;
	static const DWORD		bit			= 0;
};

//==================================================================================================
//	<Bit Mask>
//	Name: BIT_MASK
//	Usage :
//	-	BIT_MASK<
//	-		  BIT_OFFSET<  0 >
//	-		, BIT_OFFSET<  7 >
//	-		, BIT_OFFSET< 31 >
//	-	>::value					==> (2) 10000000 | 00000000 | 00000000 | 10000001
template<
	  typename _BIT_OFFSET_00 = BIT_OFFSET<32>, typename _BIT_OFFSET_01 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_02 = BIT_OFFSET<32>, typename _BIT_OFFSET_03 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_04 = BIT_OFFSET<32>, typename _BIT_OFFSET_05 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_06 = BIT_OFFSET<32>, typename _BIT_OFFSET_07 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_08 = BIT_OFFSET<32>, typename _BIT_OFFSET_09 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_10 = BIT_OFFSET<32>, typename _BIT_OFFSET_11 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_12 = BIT_OFFSET<32>, typename _BIT_OFFSET_13 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_14 = BIT_OFFSET<32>, typename _BIT_OFFSET_15 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_16 = BIT_OFFSET<32>, typename _BIT_OFFSET_17 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_18 = BIT_OFFSET<32>, typename _BIT_OFFSET_19 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_20 = BIT_OFFSET<32>, typename _BIT_OFFSET_21 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_22 = BIT_OFFSET<32>, typename _BIT_OFFSET_23 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_24 = BIT_OFFSET<32>, typename _BIT_OFFSET_25 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_26 = BIT_OFFSET<32>, typename _BIT_OFFSET_27 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_28 = BIT_OFFSET<32>, typename _BIT_OFFSET_29 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_30 = BIT_OFFSET<32>, typename _BIT_OFFSET_31 = BIT_OFFSET<32>
>
struct BIT_MASK
{
	static const DWORD		value		=
		( _BIT_OFFSET_00::bit | _BIT_OFFSET_01::bit | _BIT_OFFSET_02::bit | _BIT_OFFSET_03::bit
		| _BIT_OFFSET_04::bit | _BIT_OFFSET_05::bit | _BIT_OFFSET_06::bit | _BIT_OFFSET_07::bit
		| _BIT_OFFSET_08::bit | _BIT_OFFSET_09::bit | _BIT_OFFSET_10::bit | _BIT_OFFSET_11::bit
		| _BIT_OFFSET_12::bit | _BIT_OFFSET_13::bit | _BIT_OFFSET_14::bit | _BIT_OFFSET_15::bit
		| _BIT_OFFSET_16::bit | _BIT_OFFSET_17::bit | _BIT_OFFSET_18::bit | _BIT_OFFSET_19::bit
		| _BIT_OFFSET_20::bit | _BIT_OFFSET_21::bit | _BIT_OFFSET_22::bit | _BIT_OFFSET_23::bit
		| _BIT_OFFSET_24::bit | _BIT_OFFSET_25::bit | _BIT_OFFSET_26::bit | _BIT_OFFSET_27::bit
		| _BIT_OFFSET_28::bit | _BIT_OFFSET_29::bit | _BIT_OFFSET_30::bit | _BIT_OFFSET_31::bit
		);
};


//==================================================================================================
//	<Bit Mask>
//	Name: BIT_MASK_FILTER
//	Usage :
//	-	typedef BIT_MASK<
//	-		  BIT_OFFSET<  0 >
//	-		, BIT_OFFSET<  7 >
//	-		, BIT_OFFSET< 31 >
//	-	>							ORIGINAL_MASK
//	-	typedef BIT_MASK_REMOVE<
//	-		  ORIGINAL_MASK
//	-		, BIT_OFFSET<  7 >
//	-		, BIT_OFFSET< 31 >
//	-	>							FILTERED_MASK
//
//	-	ORIGINAL_MASK::value	==> (2) 10000000 | 00000000 | 00000000 | 10000001
//	-	FILTERED_MASK::value	==> (2) 00000000 | 00000000 | 00000000 | 00000001
//
template<
	typename _BIT_MASK
	, typename _BIT_OFFSET_00 = BIT_OFFSET<32>, typename _BIT_OFFSET_01 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_02 = BIT_OFFSET<32>, typename _BIT_OFFSET_03 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_04 = BIT_OFFSET<32>, typename _BIT_OFFSET_05 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_06 = BIT_OFFSET<32>, typename _BIT_OFFSET_07 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_08 = BIT_OFFSET<32>, typename _BIT_OFFSET_09 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_10 = BIT_OFFSET<32>, typename _BIT_OFFSET_11 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_12 = BIT_OFFSET<32>, typename _BIT_OFFSET_13 = BIT_OFFSET<32>
	, typename _BIT_OFFSET_14 = BIT_OFFSET<32>, typename _BIT_OFFSET_15 = BIT_OFFSET<32>
>
struct BIT_MASK_FILTER
{
	static const DWORD			original_mask	= _BIT_MASK::value;
	static const DWORD			skipped_mask	=
		( _BIT_OFFSET_00::bit | _BIT_OFFSET_01::bit | _BIT_OFFSET_02::bit | _BIT_OFFSET_03::bit
		| _BIT_OFFSET_04::bit | _BIT_OFFSET_05::bit | _BIT_OFFSET_06::bit | _BIT_OFFSET_07::bit
		| _BIT_OFFSET_08::bit | _BIT_OFFSET_09::bit | _BIT_OFFSET_10::bit | _BIT_OFFSET_11::bit
		| _BIT_OFFSET_12::bit | _BIT_OFFSET_13::bit | _BIT_OFFSET_14::bit | _BIT_OFFSET_15::bit
		);
	static const DWORD			value			= ( original_mask & (~skipped_mask) );
};

//==================================================================================================
//	Definition Bit Shift Value
//	Name : BIT_SHIFT
//	Usage : 
//		BIT_SHIFT<BYTE>::BITSHIFT	==> 3
//		BIT_SHIFT<WORD>::BITSHIFT	==> 4
//		BIT_SHIFT<DWORD>::BITSHIFT	==> 5
template<typename _TYPE>
struct BIT_SHIFT
{
	typedef _TYPE								node_type;
	static const DWORD							BITSHIFT = _BIT_SHIFT<sizeof(node_type)<<3>::BITSHIFT;
};

//==================================================================================================
//	Definition Mask Value
//	Name : SEQ_MASK
//	Usage : DWORD64 범위를 넘어서는 Array 지원 목적, MASK는 해당 자료형의 최대값 형태 NODE_MASK만 지원
//		Range = X
//		SEQ_MASK<BYTE, 0x01, 100>::BITVALUE ==> 0x01
//		SEQ_MASK<BYTE, 0x01, 100>::NODE_MASK ==> 0xFF
//		SEQ_MASK<BYTE, 0x01, 100>::BITSHIFT ==> 3
//		SEQ_MASK<BYTE, 0x01, 100>::COUNT ==> 100
//		SEQ_MASK<BYTE, 0x01, 100>은 논리 구조상 BYTE stream[100]; memset( stream, 0x01, sizeof(stream) ); 을 지원한다.
//	Sample : ref. FOREACH_ARR_CONTAIN sample
template<typename _TYPE, typename _TYPE _BITVALUE, typename DWORD _COUNT>
struct SEQ_MASK
{
	typedef _TYPE								node_type;
	static const node_type						BITVALUE = _BITVALUE;
	static const DWORD							BITSHIFT = BIT_SHIFT<node_type>::BITSHIFT;
	static const node_type						NODE_MASK = node_type(-1);
	static const DWORD							COUNT = _COUNT;
};

//==================================================================================================
//	Definition Mask Value
//	Name : SEQ_BYTE_MASK
//	Usage :
//		Range = (0, 8]
//		SEQ_BYTE_MASK<0x01, 4>::MASK ==> 0x0000000001010101
//		SEQ_BYTE_MASK<0x02, 8>::MASK ==> 0x0202020202020202
template<typename BYTE _BITVALUE, typename DWORD _COUNT>
struct SEQ_BYTE_MASK : public SEQ_MASK<BYTE, _BITVALUE, _COUNT>
{
	typedef SEQ_MASK<BYTE, _BITVALUE, _COUNT>	typeinfo;
	static const DWORD							MASK_NODE_MAX = sizeof(DWORD64)/sizeof(node_type);
	BOOST_STATIC_ASSERT( (0<COUNT) && (COUNT<=MASK_NODE_MAX) );
	static const DWORD64	MASK = ((DWORD64)BITVALUE<<((COUNT-1)<<BITSHIFT)) | SEQ_BYTE_MASK<BITVALUE, COUNT-1>::MASK;
};

template<typename BYTE _BITVALUE>
struct SEQ_BYTE_MASK<_BITVALUE, 1>
{ static const DWORD64	MASK = _BITVALUE<<0; };



//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name : SEQ_WORD_MASK
//	Usage :
//		Range = (0, 4]
//		SEQ_BYTE_MASK<0x0180, 1>::MASK ==> 0x0000000000000180
//		SEQ_BYTE_MASK<0x8011, 4>::MASK ==> 0x8011801180118011
template<typename WORD _BITVALUE, typename DWORD _COUNT>
struct SEQ_WORD_MASK : public SEQ_MASK<WORD, _BITVALUE, _COUNT>
{
	typedef SEQ_MASK<WORD, _BITVALUE, _COUNT>	typeinfo;
	static const DWORD							MASK_NODE_MAX = sizeof(DWORD64)/sizeof(node_type);
	BOOST_STATIC_ASSERT( (0<COUNT) && (COUNT<=MASK_NODE_MAX) );
	static const DWORD64	MASK = ((DWORD64)BITVALUE<<((COUNT-1)<<BITSHIFT)) | SEQ_WORD_MASK<BITVALUE, COUNT-1>::MASK;
};

template<typename WORD _BITVALUE>
struct SEQ_WORD_MASK<_BITVALUE, 1>
{ static const DWORD64	MASK = _BITVALUE<<0; };


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name : SEQ_DWORD_MASK
//	Usage :
//		Range = (0, 2]
//		SEQ_DWORD_MASK<0x01010101, 1>::MASK ==> 0x0000000001010101
//		SEQ_DWORD_MASK<0x0FFFFFF0, 2>::MASK ==> 0x0FFFFFF00FFFFFF0
template<typename DWORD _BITVALUE, typename DWORD _COUNT>
struct SEQ_DWORD_MASK : public SEQ_MASK<DWORD, _BITVALUE, _COUNT>
{
	typedef SEQ_MASK<DWORD, _BITVALUE, _COUNT>	typeinfo;
	static const DWORD							MASK_NODE_MAX = sizeof(DWORD64)/sizeof(node_type);
	BOOST_STATIC_ASSERT( (0<COUNT) && (COUNT<=MASK_NODE_MAX) );
	static const DWORD64	MASK = ((DWORD64)BITVALUE<<((COUNT-1)<<BITSHIFT)) | SEQ_DWORD_MASK<BITVALUE, COUNT-1>::MASK;
};

template<typename DWORD _BITVALUE>
struct SEQ_DWORD_MASK<_BITVALUE, 1>
{ static const DWORD64	MASK = _BITVALUE<<0; };


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	Name :
//		ARR_CHECK_IS_CONTAIN		: 주어진 배열에서 특정 마스크에 해당하는 값이 존재하는 지 여부 검사
//		ARR_CHECK_IS_EMPTY			: 주어진 배열에서 특정 마스크에 해당하는 값이 하나도 없는지 여부 검사
//		FOREACH_ARR_MATCH_MASK		: 주어진 배열에서 임의의 위치의 값이 특정 마스크의 위치 값과 일치하는 경우에 대한 루프 연산 지원
//		FOREACH_ARR_CONTAIN_MASK	: 주어진 배열에서 임의의 위치의 값을 특정 마스크의 위치 값이 포함하는 경우에 대한 루프 연산 지원
//	Usage :
//		<ARGUMENTs>
//		const BASE_ITEMINFO* pItemInfo = ~~;
//		typedef SEQ_BYTE_MASK<eITEM_EFFECT_ON_INVEN, BASE_ITEMINFO::_MAX_OPTION_NUM>
//			OPTION_EFFECT_INVEN_MASK;
//		typedef SEQ_BYTE_MASK<eITEM_EFFECT_ON_EQUIP|eITEM_EFFECT_ON_INVEN, BASE_ITEMINFO::_MAX_OPTION_NUM>
//			OPTION_EFFECT_ALL_MASK;
//
//	Usage Reference :
//		GameServer/InventorySlotContainer.cpp
//		ProgramCommon/ItemAttrCalculator.cpp
//
//#pragma push_macro("CONV_PTRto64")
#define CONV_PTRto64(ptr)				(*(DWORD64*)(ptr))
//─────────────────────────────────────────
//	Usage : ARR_CHECK_IS_CONTAIN
//		// INVENTORY에서 효과가 발동되는 아이템인지 검사, OR condition
//		if( ARR_CHECK_IS_CONTAIN( OPTION_EFFECT_INVEN_MASK::MASK, pItemInfo->m_byOptionExEffect ) )
//			printf( "인벤에서 효과 발동 가능 아이템\n" );
#define ARR_CHECK_IS_CONTAIN( typedef_mask, _array )		(!!( (typedef_mask)&CONV_PTRto64(_array) ))
//─────────────────────────────────────────
//	Usage : ARR_CHECK_IS_EMPTY
//		// INVENTORY에서 효과가 발동되는 아이템이 아니라면, AND condition
//		if( ARR_CHECK_IS_EMPTY( OPTION_EFFECT_INVEN_MASK::MASK, pItemInfo->m_byOptionExEffect ) )
//			return;
#define ARR_CHECK_IS_EMPTY( typedef_mask, _array )			( !( (typedef_mask)&CONV_PTRto64(_array) ))
//#pragma pop_macro("CONV_PTRto64")
//─────────────────────────────────────────
//	Range : { !IsEmpty | [ 0, typedefed_mask::COUNT ) }
//	Usage : FOREACH_ARR_MATCH_MASK
//		// for loop 처리 효과 발동, 해당 사항이 없는 경우 루프 감소 효과 있다.
//		// INVENTORY에서 효과가 발동되는 아이템이 아닐 경우 해당 루프를 실행하지 않는다.
//		// INVENTORY에서 효과가 발동되는 아이템일 경우, 값이 할당된 해당 index만을 처리하도록 지원.
//		FOREACH_ARR_MATCH_MASK( idx, OPTION_EFFECT_INVEN_MASK, pItemInfo->m_byOptionExEffect )
//		{
//			pItemInfo->m_byOptionExEffect[idx] ~~ process
//		}
#define FOREACH_ARR_MATCH_MASK( node_idx, typedef_mask, __array )	\
	if( ARR_CHECK_IS_EMPTY( typedef_mask::MASK, __array ) ) {}		\
	else for( typedef_mask::node_type node_idx=0 ; node_idx!=(typedef_mask::node_type)typedef_mask::COUNT ; ++node_idx )	\
	if( ( __array[node_idx] == (typedef_mask::node_type)( (typedef_mask::MASK>>(node_idx<<typedef_mask::BITSHIFT))&typedef_mask::NODE_MASK ) ) )
//─────────────────────────────────────────
//	Range : { !IsEmpty | [ 0, typedefed_mask::COUNT ) }
//	Usage : FOREACH_ARR_CONTAIN_MASK
//		// for loop 처리 효과 발동, 해당 사항이 없는 경우 루프 감소 효과 있다.
//		// EQUIPMENT | INVENTORY에서 효과가 발동되는 아이템이 아닐 경우 해당 루프를 실행하지 않는다.
//		// EQUIPMENT | INVENTORY에서 효과가 발동되는 아이템일 경우, 값이 할당된 해당 index만을 처리하도록 지원.
//		FOREACH_ARR_CONTAIN_MASK( idx, OPTION_EFFECT_ALL_MASK, pItemInfo->m_byOptionExEffect )
//		{
//			pItemInfo->m_byOptionExEffect[idx] ~~ process
//		}
#define FOREACH_ARR_CONTAIN_MASK( node_idx, typedef_mask, __array )	\
	if( ARR_CHECK_IS_EMPTY( typedef_mask::MASK, __array ) ) {}		\
	else for( typedef_mask::node_type node_idx=0 ; node_idx!=(typedef_mask::node_type)typedef_mask::COUNT ; ++node_idx )	\
	if( ( __array[node_idx] & (typedef_mask::node_type)( (typedef_mask::MASK>>(node_idx<<typedef_mask::BITSHIFT))&typedef_mask::NODE_MASK ) ) )
//─────────────────────────────────────────
//	Range : [ 0, sizeof(__array) )
//	Usage : FOREACH_ARR_MATCH
//		// for loop 처리 효과 발동, 무조건 루프
//		// INVENTORY에서 효과가 발동되는 아이템일 경우, 값이 할당된 해당 index만을 처리하도록 지원.
//		FOREACH_ARR_MATCH( idx, OPTION_EFFECT_INVEN_MASK, pItemInfo->m_byOptionExEffect )
//		{
//			pItemInfo->m_byOptionExEffect[idx] ~~ process
//		}
#define FOREACH_ARR_MATCH( node_idx, typedef_mask, __array )	\
	if( typedef_mask::node_type node_idx=0 ) {}					\
	else for(  typedef_mask::node_type* __T_ptr = __array ; __T_ptr!=&(__array[sizeof(__array)/sizeof(__array[0])]) ; ++node_idx, ++__T_ptr )	\
	if( typedef_mask::BITVALUE == (*__T_ptr) )
//─────────────────────────────────────────
//	Range : [ 0, sizeof(__array) )
//	Usage : FOREACH_ARR_CONTAIN
//		// for loop 처리 효과 발동, 무조건 루프
//		// EQUIPMENT | INVENTORY에서 효과가 발동되는 아이템일 경우, 값이 할당된 해당 index만을 처리하도록 지원.
//		FOREACH_ARR_CONTAIN( idx, OPTION_EFFECT_ALL_MASK, pItemInfo->m_byOptionExEffect )
//		{
//			pItemInfo->m_byOptionExEffect[idx] ~~ process
//		}
//	Sample : ItemCode 배열이 존재한다. 여기에 값이 있으면 FOREACH_ARR_CONTAIN {} 내용을 처리한다.
//		WORD itemcodes[10] = { 1, 0, 3, 4, 0, 6, 7, 0, 0, 10 };
//		typedef SEQ_MASK<WORD, -1, 10>	ITEM_CODE_ARRAY;
//		FOREACH_ARR_CONTAIN( idx, ITEM_CODE_ARRAY, itemcodes )
//		{
//			printf( "[%u:%u] ", idx, itemcodes[idx] );
//		}
//		Result = [0:1] [2:3] [3:4] [5:6] [6:7] [9:10]
#define FOREACH_ARR_CONTAIN( node_idx, typedef_mask, __array )	\
	if( typedef_mask::node_type node_idx=0 ) {}					\
	else for(  typedef_mask::node_type* __T_ptr = __array ; __T_ptr!=&(__array[sizeof(__array)/sizeof(__array[0])]) ; ++node_idx, ++__T_ptr )	\
	if( typedef_mask::BITVALUE & (*__T_ptr) )
//─────────────────────────────────────────
//	Range : [ _container.begin(), _continer.end() )
//	Usage : FOREACH_CONTAINER
//		// for loop 처리 효과 발동, 무조건 루프
//		typedef map< DWORD, DWORD >			TEST_STL_MAP;
//		typedef pair< const DWORD, DWORD >	TEST_STL_MAP_PAIR;
//		TEST_STL_MAP tmap;
//		for( DWORD i=1 ; i<(100+1) ; ++i )
//		tmap.insert( make_pair( i, i ) );
//
//		FOREACH_CONTAINER( TEST_STL_MAP_PAIR& rPair, tmap, TEST_STL_MAP )
//		{
//			printf( "[KEY : %3u][VAL : %3u]\n", rPair.first, rPair.second );
//		}
//		// 또는...
//		FOREACH_CONTAINER( TEST_STL_MAP_PAIR rPair, tmap, TEST_STL_MAP )
//		{
//			printf( "[KEY : %3u][VAL : %3u]\n", rPair.first, rPair.second );
//		}
//		// Result
//		//	[KEY :   1][VAL :   1]
//		//	[KEY :   2][VAL :   2]
//		//	[KEY :   3][VAL :   3]
//		//	...
//		//	[KEY :  99][VAL :  99]
//		//	[KEY : 100][VAL : 100]
//
#if SUNSTL_VERSION_CTRL <= 0x0110
	#define FOREACH_CONTAINER( inst, _container, _container_type )						\
		if( ::util::internal::STL_CONTAINER_WRAPPER<_container_type>* __T_tmp = ::util::internal::make_container_wrapper(_container) ) {}	\
		else if( ::util::internal::STL_CONTAINER_WRAPPER<_container_type>::iterator _T_it= __T_tmp->begin(_container) ) {}	\
		else if( ::util::internal::STL_CONTAINER_WRAPPER<_container_type>::iterator _T_end= __T_tmp->end(_container) ) {}		\
		else for( bool __T_continue = true ; __T_continue && ( _T_it.itr != _T_end.itr ) ; __T_continue ? ++_T_it.itr : 0 )	\
		if( __T_continue = false ) {} else for( inst = *_T_it.itr ; !__T_continue ; __T_continue=true )
#elif SUNSTL_VERSION_CTRL > 0x0110
	#define FOREACH_CONTAINER( inst, _container, _container_type )		\
		if( ::util::internal::STL_CONTAINER_WRAPPER<_container_type>::iterator _T_it= ::util::internal::STL_CONTAINER_WRAPPER<_container_type>::begin(_container) ) {}	\
		else if( ::util::internal::STL_CONTAINER_WRAPPER<_container_type>::iterator _T_end= ::util::internal::STL_CONTAINER_WRAPPER<_container_type>::end(_container) ) {}	\
		else for( bool __T_continue = true ; __T_continue && ( _T_it.itr != _T_end.itr ) ; ++_T_it.itr )	\
		if( __T_continue ? __T_continue=false : false ) {} else for( inst = *_T_it.itr ; !__T_continue ; __T_continue=true )
#endif

//─────────────────────────────────────────
//	Range : [ _array.begin(), _array.end() )
//	Usage : FOREACH_ARRAY
////
//	{	// default
//		typedef ARRAY_TYPEINFO< WORD, 0, 10 >		range_info;
//		range_info::array_type array = { 0,1,2,3,4,5,6,7,8,9 };	// #1
//		FOREACH_ARRAY( const WORD node, array, range_info )
//		{ /*Process node = { 0,1,2,3,4,5,6,7,8,9 }*/ printf( "%u\n", node ); }
//	}
//	{	// const array
//		const WORD array[10] = { 0,1,2,3,4,5,6,7,8,9 };
//		typedef ARRAY_TYPEINFO<WORD, 3, 7>	range_info;
//		FOREACH_ARRAY( const WORD node, array, range_info )
//		{ /*Process node = { 3,4,5,6 }*/ printf( "%u\n", node ); }
//	}
//	{	// pointer loop
//		WORD array[10] = { 0,1,2,3,4,5,6,7,8,9 };
//		const WORD* pArray[10];
//		for( DWORD i=0 ; i<10 ; ++i )
//			pArray[i] = &array[i];
//
//		typedef ARRAY_TYPEINFO<const WORD*, 4, 10>	range_info;
//		FOREACH_ARRAY( const WORD* pnode, pArray, range_info )
//		{ /*Process node = { 4,5,6,7,8,9 }*/ printf( "%u\n", *pnode ); }
//	}
	#define FOREACH_ARRAY( inst, _array, _array_info )						\
		if( bool __T_stop = false ) {}	\
		else for( _array_info::type *_T_it=&_array[_array_info::begin_], *_T_end=&_array[_array_info::end_] ; !__T_stop && ( _T_it != _T_end ) ; ++_T_it )	\
		if( !(__T_stop = true) ) {} else for( inst = *_T_it ; __T_stop ; __T_stop=false )
};	//end namespace

#endif //__SUN_STL_TMP_UTILITY__
