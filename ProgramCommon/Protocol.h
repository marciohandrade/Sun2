

#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

//																	<패킷 접미사 설명>
//
//		REQ (Request) - Server 에 요청								ANS (Answer) - 요청에 대한 답변
//		CMD (Command) - Server 의 명령
//		SYN (Sync) - Server 에 동기화 관련 내용을 알림		BRD (Broadcast) - Server 가 동기화 관련 내용을 알림
//
// => 수정 예정 : 2004-10.19
//		SYN - Server에 요청
//		ACK - 요청에 대한 허락
//		NAK - 요청에 대한 거부
//		NTF - Server의 명령
//		BRD - Server가 동기화 관련 내용을 알림
//		DBR - DBProxy에서만 사용함!
//-------------------------------------------------------------------------------------------------------
/// @name Field Server - Client

#include "Version.h"

#define TYPE_MAX_ROUND()	256

#define USIGNED64_T		(__int64)
#define BU64DECLARE		const __int64

BU64DECLARE ODD_CATEGORY_RANDOM_NUMBER =	USIGNED64_T(( (__int64)-CATEGORY_ODD_NUMBER - ((C2S_HIGH_VERSION_NO)*100) - ((C2S_MIDDLE_VERSION_NO)*100)) );	// 일단 고정 난수값 (홀수 번호)
BU64DECLARE ODD_PROTOCOL_RANDOM_NUMBER =	USIGNED64_T(( (__int64)-PROTOCOL_ODD_NUMBER - ((C2S_HIGH_VERSION_NO)*100) - ((C2S_MIDDLE_VERSION_NO)*10)) );

#define CATEGORY_GENERATOR(num)		( (BYTE)( ((ODD_CATEGORY_RANDOM_NUMBER)*(num))%(TYPE_MAX_ROUND()) ) )
#define PROTOCOL_GENERATOR(num)		( (BYTE)( ((ODD_PROTOCOL_RANDOM_NUMBER)*(num))%(TYPE_MAX_ROUND()) ) )

BU64DECLARE INDEX_RANDOM_NUMBER =		USIGNED64_T( -((__int64)ODD_CATEGORY_RANDOM_NUMBER)*(ODD_PROTOCOL_RANDOM_NUMBER) );
#define INDEX_GENERATOR(num)			( (BYTE)( ((INDEX_RANDOM_NUMBER)*(num))%(TYPE_MAX_ROUND()) ) )


#include "Category.Generator.inc"
#include "Protocol.Generator.inc"
#include "ProtocolGenerators/Protocol.Generator.Includes.hpp"
#include "FI_RndProtocol/FI_RND_GENDER_CALCULATOR.h"

#endif // __PROTOCOL_H__

#ifndef __NATION_CODE_H__
#define __NATION_CODE_H__

#if defined(__KR) || defined(_KOREA)
#define NATION_CODE		CATEGORY_GENERATOR(INDEX_GENERATOR(171))
#elif defined(__JP) || defined(_JAPAN)
#define NATION_CODE		CATEGORY_GENERATOR(INDEX_GENERATOR(12))
#elif defined(__US) || defined(_AMERICA)
#define NATION_CODE		CATEGORY_GENERATOR(INDEX_GENERATOR(153))
#elif defined(__TW) || defined(_TAIWAN)
#define NATION_CODE		CATEGORY_GENERATOR(INDEX_GENERATOR(44))
#elif defined(__CN) || defined(_CHINA)
#define NATION_CODE		CATEGORY_GENERATOR(INDEX_GENERATOR(52))
#elif defined(__US) || defined( _USA )
#define NATION_CODE		CATEGORY_GENERATOR(INDEX_GENERATOR(69))
#elif defined(_GLOBAL)
#define NATION_CODE     CATEGORY_GENERATOR(INDEX_GENERATOR(199))
#elif defined(_RUSSIA)
#define NATION_CODE     CATEGORY_GENERATOR(INDEX_GENERATOR(161))
#endif

#endif __NATION_CODE_H__


