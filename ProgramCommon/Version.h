/************************************************************************/
/* Version.h : C/S Version 정의
/************************************************************************/

#ifndef	__VERSION_H__
#define	__VERSION_H__

//#include "../include/Global.Define.h"
#include "CountryCode.h" // 국가 코드

// Deployment.h 또는 외부에서 NATION_BUILD_VERSION이 지정되어 있지 않다면 
// 다음의 국가별 버전 설정이 사용된다.

#if defined( _KOREA )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		1601		// 빌드버전(서버, 클라 공통)
  #endif
    #define XIGNCODE_VERSION            111215
	#define	HACKSHIELD_VERSION			5050601

#elif defined( _JAPAN )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		1302		// 빌드버전(서버, 클라 공통)
  #endif
	#define XIGNCODE_VERSION            111215
	#define	HACKSHIELD_VERSION			5040601

#elif defined( _AMERICA )

#elif defined( _TAIWAN )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		904			// 빌드버전(서버, 클라 공통)
  #endif
	#define	HACKSHIELD_VERSION			1055

#elif defined( _CHINA )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		1204		// 빌드버전(서버, 클라 공통)
  #endif
	#define	HACKSHIELD_VERSION			5050401
    #define XIGNCODE_VERSION            130203

#elif defined( _USA )
  #ifndef NATION_BUILD_VERSION
	#define NATION_BUILD_VERSION		1202		// 빌드버전(서버, 클라 공통)
  #endif
	#define	HACKSHIELD_VERSION			1055

#elif defined( _GLOBAL )
  #ifndef NATION_BUILD_VERSION
    #define NATION_BUILD_VERSION		1301		// 빌드버전(서버, 클라 공통)
  #endif
    #define	HACKSHIELD_VERSION			5050601
    #define XIGNCODE_VERSION            111215

#elif defined( _RUSSIA )
#ifndef NATION_BUILD_VERSION
#define NATION_BUILD_VERSION		    1203		// 빌드버전(서버, 클라 공통)
#endif
#define	HACKSHIELD_VERSION			    5040401


#else //
	#error "Not Defined Preprocessor -Nation Definition-"
#endif //

//
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// 카테고리와 프로토콜의 상수 번호 난수 발생기를 위한 수치 입력
// 임의의 상수를 입력하세요.
// <WARNING>
//   1) 반드시 1의 자리는 홀수 값이 되어야 합니다, (1, 3, 5, 7, 9) 중에 하나를 입력하세요.
//   2) 양수(+)값을 입력해 주세요.
//   3) 입력 수치 범위 (최소 1001 ~ 최대 99999999 )
//
// 아래는 샘플
//   #define CATEGORY_ODD_NUMBER		(159371)
//   #define PROTOCOL_ODD_NUMBER		(310025)

//////////////////////////////////////////////////////////////////////////
// nProtect GameGuard 적용시 주의사항
// GameGuard는 라이브러리 생성자의 이름을 지정해야 한다.
// 따라서, 현재는 중국의 경우 SUN1->1204, SUN3->1304(그이상)
// 빌드 시에 디파인으로 SUN1인지 SUN3인지 지정하도록 하여야 할 것이다.
// #if defined(__APPLY_GAMEGUARD)//     
//     #if defined(SUN1)
//         #define _CHINA_GAMEGUARD_NAME "SUN1CN"
//     #elif defined(SUN3)
//         #define _CHINA_GAMEGUARD_NAME "SUN3CN"
//     #else
//         아예 빌드가 깨지도록 하자.
//     #endif // SUN1
// #endif // __APPLY_GAMEGUARD
///////////////////////////////////////////////////////////////////////////

#if defined( _KOREA )
        #if NATION_BUILD_VERSION >= 1601
		#define CATEGORY_ODD_NUMBER         (51375) // 160121
		#define PROTOCOL_ODD_NUMBER         (13547)
		#define __APPLY_XIGNCODE    // 사인코드 적용
        #elif NATION_BUILD_VERSION >= 1504
		#define CATEGORY_ODD_NUMBER         (87953) // 130805
		#define PROTOCOL_ODD_NUMBER         (94215)
		#define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1503
		#define CATEGORY_ODD_NUMBER         (61251) // 130805
		#define PROTOCOL_ODD_NUMBER         (84573)
		#define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1502
		#define CATEGORY_ODD_NUMBER         (56319) // 130805
		#define PROTOCOL_ODD_NUMBER         (74121)
		#define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1501
		#define CATEGORY_ODD_NUMBER         (36757) // 130805
		#define PROTOCOL_ODD_NUMBER         (51379)
		#define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1404
		#define CATEGORY_ODD_NUMBER         (368725) // 130805
		#define PROTOCOL_ODD_NUMBER         (435127)
		#define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1403
		#define CATEGORY_ODD_NUMBER         (218973) // 130805
		#define PROTOCOL_ODD_NUMBER         (348205)
		#define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1303
        #define CATEGORY_ODD_NUMBER         (218971) // 130805
        #define PROTOCOL_ODD_NUMBER         (348203)
        #define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1302
        #define CATEGORY_ODD_NUMBER         (673247) // 130125
        #define PROTOCOL_ODD_NUMBER         (452249)
        #define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1301
        #define CATEGORY_ODD_NUMBER         (346243) // 130125
        #define PROTOCOL_ODD_NUMBER         (622135)
        #define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1204
        #define CATEGORY_ODD_NUMBER         (346241) // 120502
        #define PROTOCOL_ODD_NUMBER         (622133)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION == 1203
        #define CATEGORY_ODD_NUMBER         (304659) // 120502
        #define PROTOCOL_ODD_NUMBER         (342091)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION == 1202
        #define CATEGORY_ODD_NUMBER         (264817) // 120502
        #define PROTOCOL_ODD_NUMBER         (492349)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION == 1201
        #define CATEGORY_ODD_NUMBER         (519425) // 120109
        #define PROTOCOL_ODD_NUMBER         (987821)
        //#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION == 1103
        #define CATEGORY_ODD_NUMBER         ( 56823) // 110524
        #define PROTOCOL_ODD_NUMBER         (167459)
        //#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION == 1102
        #define CATEGORY_ODD_NUMBER         ( 93187) // 110524
        #define PROTOCOL_ODD_NUMBER         (204653)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION == 1101
        #define CATEGORY_ODD_NUMBER         (287631) // 101213
        #define PROTOCOL_ODD_NUMBER         (120945)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION == 1006
        #define CATEGORY_ODD_NUMBER         (212635) // 101105
        #define PROTOCOL_ODD_NUMBER         ( 58473)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION == 1005
        #define CATEGORY_ODD_NUMBER         ( 72989) // 101001
        #define PROTOCOL_ODD_NUMBER         (351765)
    #elif NATION_BUILD_VERSION == 1004
        #define CATEGORY_ODD_NUMBER         (198473) // 100726
        #define PROTOCOL_ODD_NUMBER         ( 68537)
    #elif NATION_BUILD_VERSION == 1002
        #define CATEGORY_ODD_NUMBER         ( 28391) // 100325
        #define PROTOCOL_ODD_NUMBER         (162745)
    #elif NATION_BUILD_VERSION == 1001
        #define CATEGORY_ODD_NUMBER         (149567) // 091127
        #define PROTOCOL_ODD_NUMBER         ( 51391)
    #elif NATION_BUILD_VERSION == 905
        #define CATEGORY_ODD_NUMBER         (19567) // 091019
        #define PROTOCOL_ODD_NUMBER         (251091)
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER         (199567) // 090717
        #define PROTOCOL_ODD_NUMBER         (65191)
    #elif NATION_BUILD_VERSION == 903
        #define CATEGORY_ODD_NUMBER         (45551) // 090305
        #define PROTOCOL_ODD_NUMBER         (119967)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    ""
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    ""
    #endif
#elif defined( _JAPAN )
    #if NATION_BUILD_VERSION >= 1302
        #define CATEGORY_ODD_NUMBER         (3102283)   // 130620
        #define PROTOCOL_ODD_NUMBER         (1245729)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1301
        #define CATEGORY_ODD_NUMBER         (202289)    // 130307
        #define PROTOCOL_ODD_NUMBER         (482825)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION == 1204
        #define CATEGORY_ODD_NUMBER         (452731)    // 120927
        #define PROTOCOL_ODD_NUMBER         (236343)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1203
        #define CATEGORY_ODD_NUMBER         (156829)    // 120927
        #define PROTOCOL_ODD_NUMBER         (267455)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1202
        #define CATEGORY_ODD_NUMBER         (156827)    // 110524
        #define PROTOCOL_ODD_NUMBER         (267453)
    	//#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1201
        #define CATEGORY_ODD_NUMBER         (156825)    // 110524
        #define PROTOCOL_ODD_NUMBER         (267451)
        //#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1103
        #define CATEGORY_ODD_NUMBER         (156825)    // 110524
        #define PROTOCOL_ODD_NUMBER         (267451)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1102
        #define CATEGORY_ODD_NUMBER         ( 93187)    // 110524
        #define PROTOCOL_ODD_NUMBER         (204653)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1101
        #define CATEGORY_ODD_NUMBER         (135895)    // 100910
        #define PROTOCOL_ODD_NUMBER         (586017)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1006
        #define CATEGORY_ODD_NUMBER         ( 39717)    // 100910
        #define PROTOCOL_ODD_NUMBER         (732463)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1005
        #define CATEGORY_ODD_NUMBER         ( 76517)    // 100910
        #define PROTOCOL_ODD_NUMBER         (342433)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1004
        #define CATEGORY_ODD_NUMBER         ( 36519)    // 100910
        #define PROTOCOL_ODD_NUMBER         (782065)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION == 1002
        #define CATEGORY_ODD_NUMBER         ( 18815)    // 100528
        #define PROTOCOL_ODD_NUMBER         (145171)
    #elif NATION_BUILD_VERSION == 1001
        #define CATEGORY_ODD_NUMBER         (188215)    // 100311
        #define PROTOCOL_ODD_NUMBER         ( 45171)
    #elif NATION_BUILD_VERSION == 905
        #define CATEGORY_ODD_NUMBER         (348117)    // 100108
        #define PROTOCOL_ODD_NUMBER         ( 75423)
    #elif NATION_BUILD_VERSION == 903
        #define CATEGORY_ODD_NUMBER         ( 78419)    // 090826
        #define PROTOCOL_ODD_NUMBER         ( 90403)
    #elif NATION_BUILD_VERSION == 902
        #define CATEGORY_ODD_NUMBER         (414419)    // 090619
        #define PROTOCOL_ODD_NUMBER         ( 86407)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    "Japan"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    "JapanClient"
    #endif
#elif defined( _AMERICA )

#elif defined( _TAIWAN )
    #if NATION_BUILD_VERSION >= 905
        #define CATEGORY_ODD_NUMBER         (718143)    // 100311
        #define PROTOCOL_ODD_NUMBER         ( 91221)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER         ( 82197)    // 100312
        #define PROTOCOL_ODD_NUMBER         (102235)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION == 903
        #define CATEGORY_ODD_NUMBER         (4119)      // 091102
        #define PROTOCOL_ODD_NUMBER         (121193)
    #elif NATION_BUILD_VERSION == 902
        #define CATEGORY_ODD_NUMBER         (417717)    // 090605
        #define PROTOCOL_ODD_NUMBER         (902901)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    "Taiwan"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    "TaiwanClient"
    #endif
#elif defined( _CHINA )
    #if NATION_BUILD_VERSION >= 1204
        #define CATEGORY_ODD_NUMBER         (561531)  // 121211
        #define PROTOCOL_ODD_NUMBER         (900379)
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1203
        #define CATEGORY_ODD_NUMBER         (550329)  // 121004
        #define PROTOCOL_ODD_NUMBER         (998925)
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1202
        #define CATEGORY_ODD_NUMBER         (540427)  // 120705
        #define PROTOCOL_ODD_NUMBER         (998823)
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1201
        #define CATEGORY_ODD_NUMBER         (539425) // 120109
        #define PROTOCOL_ODD_NUMBER         (997821)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1103
        #define CATEGORY_ODD_NUMBER         (42613)  // 111222
        #define PROTOCOL_ODD_NUMBER         (62859)
        //#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1102
        #define CATEGORY_ODD_NUMBER         (41611)  // 101130
        #define PROTOCOL_ODD_NUMBER         (61853)
        //#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1101
        #define CATEGORY_ODD_NUMBER         (51315)  // 101130
        #define PROTOCOL_ODD_NUMBER         (63457)
        //#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE
    #elif NATION_BUILD_VERSION >= 1006
        #define CATEGORY_ODD_NUMBER         (62311)  // 101130
        #define PROTOCOL_ODD_NUMBER         (56453)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1005
        #define CATEGORY_ODD_NUMBER         (67323)  // 101027
        #define PROTOCOL_ODD_NUMBER         (55479)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1004
        #define CATEGORY_ODD_NUMBER         (62321)  // 100914
        #define PROTOCOL_ODD_NUMBER         (55673)
    #elif NATION_BUILD_VERSION == 1003
        #define CATEGORY_ODD_NUMBER         (66021)  // 100810
        #define PROTOCOL_ODD_NUMBER         (51679)
    #elif NATION_BUILD_VERSION == 1001
        #define CATEGORY_ODD_NUMBER         (74023)  // 100317
        #define PROTOCOL_ODD_NUMBER         (11273)
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER         (719423)  // 091106
        #define PROTOCOL_ODD_NUMBER         (12777)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    "China"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    "ChinaClient"
    #endif
#elif defined( _USA )
    #if NATION_BUILD_VERSION >= 1002
        #define CATEGORY_ODD_NUMBER           (369571)    // 100625
        #define PROTOCOL_ODD_NUMBER           (546793)
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER           (11211)    // 100201
        #define PROTOCOL_ODD_NUMBER           (87463)
    #elif NATION_BUILD_VERSION == 902
        #define CATEGORY_ODD_NUMBER           (932217)    // 090616
        #define PROTOCOL_ODD_NUMBER           (317921)
    #elif NATION_BUILD_VERSION == 815
        #define CATEGORY_ODD_NUMBER           (114407)    // 080812
        #define PROTOCOL_ODD_NUMBER           (369369)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY    "England"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY    "EnglandClient"
    #endif
#elif defined( _GLOBAL )
    #if NATION_BUILD_VERSION >= 1301
        #define CATEGORY_ODD_NUMBER           (514311)    // 130423
        #define PROTOCOL_ODD_NUMBER           (246789)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1204
        #define CATEGORY_ODD_NUMBER           (412279)    // 121227
        #define PROTOCOL_ODD_NUMBER           (262333)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1203
        #define CATEGORY_ODD_NUMBER           (312035)    // 121008
        #define PROTOCOL_ODD_NUMBER           (652035)
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1202
        #define CATEGORY_ODD_NUMBER           (340025)    // 120423
        #define PROTOCOL_ODD_NUMBER           (650933)
        #define __APPLY_XIGNCODE    // 사인코드 적용
	#elif NATION_BUILD_VERSION >= 1201
        #define CATEGORY_ODD_NUMBER           (220425)    // 120423
        #define PROTOCOL_ODD_NUMBER           (120423)
        //#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1103
        #define CATEGORY_ODD_NUMBER           (211123)    // 111121
        #define PROTOCOL_ODD_NUMBER           (111121)
        //#define __APPLY_HACKSHIELD // 핵쉴드 적용
        #define __APPLY_XIGNCODE    // 사인코드 적용
    #elif NATION_BUILD_VERSION >= 1102
        #define CATEGORY_ODD_NUMBER           (210627)    // 110627
        #define PROTOCOL_ODD_NUMBER           (110625)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1101
        #define CATEGORY_ODD_NUMBER           (210329)    // 110328
        #define PROTOCOL_ODD_NUMBER           (110327)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1006
        #define CATEGORY_ODD_NUMBER           (210223)    // 110224
        #define PROTOCOL_ODD_NUMBER           (110225)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1004
        #define CATEGORY_ODD_NUMBER           (145693)    // 101111
        #define PROTOCOL_ODD_NUMBER           (698541)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION >= 1002
        #define CATEGORY_ODD_NUMBER           (956487)    // 100805
        #define PROTOCOL_ODD_NUMBER           (186491)
        #define __APPLY_HACKSHIELD // 핵쉴드 적용
    #elif NATION_BUILD_VERSION == 904
        #define CATEGORY_ODD_NUMBER           (711937)    // 100303
        #define PROTOCOL_ODD_NUMBER           (295529)
    #elif NATION_BUILD_VERSION == 902
        #define CATEGORY_ODD_NUMBER           (169371)    // 091109
        #define PROTOCOL_ODD_NUMBER           (892217)
    #elif NATION_BUILD_VERSION == 815
        #define CATEGORY_ODD_NUMBER           (114407)    // 080812
        #define PROTOCOL_ODD_NUMBER           (369369)
    #else
        #error "Unsupported BuildVersion"
    #endif
    #ifdef _SERVER
        #define NATION_BUILD_NAME_OF_COUNTRY	"Global"
    #else
        #define NATION_BUILD_NAME_OF_COUNTRY	"GlobalClient"
    #endif
#elif defined( _RUSSIA )
    #if NATION_BUILD_VERSION >= 1203
        #define CATEGORY_ODD_NUMBER           (546539)
        #define PROTOCOL_ODD_NUMBER           (263439)     // 130109
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1202
        #define CATEGORY_ODD_NUMBER           (345537)
        #define PROTOCOL_ODD_NUMBER           (565433)     // 120813
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1103
        #define CATEGORY_ODD_NUMBER           (345335)
        #define PROTOCOL_ODD_NUMBER           (563431)     // 120112
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1102
        #define CATEGORY_ODD_NUMBER           (122229)
        #define PROTOCOL_ODD_NUMBER           (233455)     // 110919
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1101
        #define CATEGORY_ODD_NUMBER           (177663)
        #define PROTOCOL_ODD_NUMBER           (286541)     // 110329
        #define __APPLY_FROST
    #elif NATION_BUILD_VERSION >= 1004
        #define CATEGORY_ODD_NUMBER           (102013)
        #define PROTOCOL_ODD_NUMBER           (234451)
        #define __APPLY_FROST
    #else
        #error "Unsupported BuildVersion"
    #endif
#ifdef _SERVER
#define NATION_BUILD_NAME_OF_COUNTRY	"Russia"
#else
#define NATION_BUILD_NAME_OF_COUNTRY	"RussiaClient"
#endif
#else //
    #error "Not Defined Preprocessor -Nation Definition-"

#endif //

#if defined(__APPLY_GAMEGUARD)
#if defined(SUN1)
#define _CUSTOM_GAMEGUARD_NAME _T("SUN1CN")
#elif defined(SUN3)
#define _CUSTOM_GAMEGUARD_NAME _T("SUN3CN")
#else
//         아예 빌드가 깨지도록 하자.
#endif // SUN1
#endif // __APPLY_GAMEGUARD

//==================================================================================================
// Compiler version control
#if _MSC_VER >= 1600 // vs2010
    #define VC_DIR_REDIRECT     "v100"
#elif _MSC_VER >= 1500 // vs2008
    #define VC_DIR_REDIRECT     "v90"
#elif _MSC_VER >= 1310 // vs2003
    #define VC_DIR_REDIRECT
#else
    #pragma message("not tested on a low version complier");
    #define VC_DIR_REDIRECT
#endif
//==================================================================================================

// C/S Version History
//------------------------------------------------------------------------------------------------------------------
// 2007-11-06(화) Ver 3.2.4  : 0.8.1.1 버전의 리소스 취합후 정기 빌드 : 페이트 아이템
// 2007-01-31(수) Ver 3.2.1  : 0.8.0.8 버전의 리소스 취합후 정기 빌드 : 저주의탑
// 2007-01-31(수) Ver 3.2.0  : 0.8.0.7 버전의 리소스 취합후 정기 빌드 : 패키지/선물/용족미션추가/유료아이템추가
// 2007-01-11(목) Ver 3.1.5  : 
// 2006-12-22(금) Ver 3.1.4  : 캐시아이템 리스트 2차 추가
// 2006-12-19(화) Ver 3.1.3  : PC방 전용옵션 적용
// 2006-12-15(금) Ver 3.1.2  : 기획스크립트 수정, 게임 유료화 관련로그 수정
// 2006-12-14(목) Ver 3.1.1  : [SUN 아이템 상용화 시작버전] 아이템 상용화 버그 수정, 개인상점 리스트 재배포
// 2006-12-12(화) Ver 2.9.1  : 상용화 2차 아이템 유료화 개발 2차 릴리즈 버전(1차 버전 기본적인 버그 수정버전)
// 2006-12-10(일) Ver 2.8.15 : 상용화 2차 아이템 유료화 개발 1차 릴리즈 버전
// 2006-11-29(수) Ver 2.8.14 : 카오퀘스트 추가, 채팅 개선 1차, AC시스템 확장
// 2006-11-14(화) Ver 2.8.13 : 개인상점 난립에 대한 개선
// 2006-10-30(월) Ver 2.8.12 : 신규미션 추가, 게임내 서비스 상용화 1차
// 2006-10-10(화) Ver 2.8.11 : 미션랭킹 체크 강화, 300% 경험치 이벤트
// 2006-09-29(금) Ver 2.8.10 : 미션랭킹 정보 전송 최적화
// 2006-09-06(수) Ver 2.8.9  : 미션랭킹
// 2006-08-23(수) Ver 2.8.8  : 방 이동시 버프유지기능, 필드 리펙토링, wmo분리작업 완료
// 2006-08-16(수) Ver 2.8.7  : 더블하임 PC방 이벤트, 친구상세정보 처리
// 2006-08-08(화) Ver 2.8.6  : PK 릴리즈
// 2006-08-01(화) Ver 2.8.5  : 웨이포인트 작업
// 2006-07-25(화) Ver 2.8.4  : 경쟁헌팅 보상강화 및 어뷰징 방지처리, 채널채팅 기능
// 2006-07-19(수) Ver 2.8.3  : 이벤트 관련 DB수정, 트리거 업데이트 로직 원래대로 수정, 경쟁헌팅 수정 작업
// 2006-07-11(화) Ver 2.8.2  : 친구차단 리스트 최대값 버그 수정
// 2006-06-29(목) Ver 2.8.1  : 조합 관련 작업
// 2006-06-22(목) Ver 2.7.2  : 파티MP창 버그 수정, 스킬 버그 수정, 길드공지 처리
// 2006-06-13(화) Ver 2.7.1  : 경쟁헌팅 자동매칭 기능, 아이템 조합 기능, 길드 및 친구 레벨업 실시간 통지기능
// 2006-06-01(목) Ver 2.6.3  : 이벤트 아이템 버그 수정, 아이템 사라지는 버그 수정, 길드채팅 버그 수정
// 2006-05-29(월) Ver 2.6.2  : 오픈베타 후 첫 정기점검, 게임로그 버그 및 기능 추가, FSM에서 발생하는 렉 수정
// 2006-05-23(화) Ver 2.6.1  : [오픈베타 시작 버전]
// 2006-05-22(월) Ver 2.5.6  : [StressTest 마지막 버전]내구도 처리 및 수리기능, 아이템 나누기
// 2006-05-19(금) Ver 2.5.5  : 내구도 관련 패킷 추가, 파티 경험치 문제 수정, 보스 몬스터 재부활 버그 수정
// 2006-05-15(월) Ver 2.5.4  : 경쟁헌팅 시작위치 다르게 처리, 필드 사망시 시작위치 부활, 기타 버그 수정
// 2006-05-10(수) Ver 2.5.3  : 몬스터 소환 버그 수정, GM명령어 버그 수정
// 2006-05-08(월) Ver 2.5.2  : [StressTest 시작 버전] 퀘스트 버그 수정, 최대레벨이상 오르는 버그 수정
// 2006-05-07(일) Ver 2.5.1  : 피크타임 이벤트, 경쟁헌팅 추가 기능, GM등급 인증에서 받아옴
// 2006-05-05(금) Ver 2.4.3  : [E3 버전] PVP 동기화 버그 수정, 미션 보상경험치 버그 수정 
// 2006-05-04(목) Ver 2.4.2  : 창고저장 버그 수정, 경쟁헌팅 수정 요청 사항 반영버전
// 2006-05-03(수) Ver 2.4.1  : 미션랭킹 관련 DB처리, 점프시 동기화 문제 수정
// 2006-05-02(화) Ver 2.3.8  : 각종 버그 수정(아이템,길드,필드이동,미션 등등)
// 2006-04-29(토) Ver 2.3.7  : 맵정보에서 트리거정보 분리 작업, 경쟁헌팅 Death Match Mode
// 2006-04-26(수) Ver 2.3.6  : 클라이언트와 주고받는 기본 패킷(MSG_FORWARD)에서 DWORD값 없앰
// 2006-04-24(월) Ver 2.3.5  : 주기적인 패킷시리얼 변경 기능
// 2006-04-21(금) Ver 2.3.4  : 매칭DB처리 추가, PVP 파티 버그 수정, 연합 퀘스트
// 2006-04-19(수) Ver 2.3.3  : AC시스템 추가
// 2006-04-18(화) Ver 2.3.2  : 미션맵 테스트 완료 버전
// 2006-04-17(월) Ver 2.3.1  : 빌드시마다 프로토콜 바꾸기 기능 추가
// 2006-04-14(금) Ver 2.2.5  : 방과 필드 경험치 분리, 아이템DB크기 변경
// 2006-04-11(화) Ver 2.2.4  : 몬스터 업데이트 영역 버그 수정, 경쟁헌팅에 따른 서버구현 1차 완료, 아이템 드랍 버그 수정 
// 2006-04-10(월) Ver 2.2.3  : 몬스터 아이템/하임 드랍방식 재 수정, 기존 마을 이동 프로토콜 삭제
// 2006-04-07(금) Ver 2.2.2  : 몬스터 아이템/하임 드랍방식 수정, 경쟁헌팅 1차,마을 전투맵 위치 저장, 퀘스트 최적화 버전
// 2006-04-04(화) Ver 2.2.1  : 음성채팅, 퀘스트 1차 완료, 파티장 근처로 워프 및 소환, 필드 사냥맵 처리버전
// 2006-03-31(금) Ver 2.1.7  : 캐릭터 점프 기능, 멀티파티에 따른 경험치 및 아이템 분배, 아이템 소유권 기능 버전
// 2006-03-21(화) Ver 2.1.6  : 아이템 Cycle 완성, 패킷암호화 시작, 멀티파티 기능, 이용자 정지 시스템 추가 버전
// 2006-03-09(목) Ver 2.1.5  : 멀티파티 프로토콜 추가, 라이브러리화 작업, SUN 패킷 암호화 모듈 추가 버전
// 2006-02-21(화) Ver 2.1.4  : 기본 파라미터 변경, 속성 데미지 적용, 공격 패킷 최적화 버전
// 2006-02-15(수) Ver 2.1.3  : 아이템 관련 패킷 최적화 작업
// 2006-01-25(수) Ver 2.1.2  : [PreOpen 마지막 버전]최대 Join인원 버그 수정, 최대 방갯수 제한 기능, 개인상점 버그 수정 버전
// 2006-01-24(화) Ver 2.1.1  : [PreOpen 시작버전]RetreatState 빈번하게 변경되는 버그 수정, 길드생성조건 변경 버전 
// 2006-01-23(월) Ver 1.5.29 : 캐릭 사망시 HP/MP리젠 안되던 버그수정,어빌리티 인덱스 오류 수정,릴레이헌팅 Join버그 수정버전
// 2006-01-23(월) Ver 1.5.28 : 캐릭터 삭제 안되는 버그 수정, PVP데이터 DB기록 부분 추가, 몬스터 사거리밖 공격버그 수정버전
// 2006-01-22(일) Ver 1.5.27 : 파티장 없어지는 버그 수정, 마을 전용 패킷 버그 수정 버전 
// 2006-01-20(금) Ver 1.5.26 : AI BattleRecode 버그 수정, 거래시 버그 수정, 트리거 버그 수정, 이벤트 아이템 처리 버전
// 2006-01-19(목) Ver 1.5.25 : 마을이동 버그 수정, 트리거 버그 수정, 월드서버 길드정보 복구 처리 버전
// 2006-01-18(수) Ver 1.5.24 : 연구개발 게임로그모듈에서 죽는 버그 주석 처리, 로비관련 프로토콜 수정버전
// 2006-01-16(월) Ver 1.5.23 : 캐릭터의 길드정보 브로드캐스팅, 마법공격 크리티컬 패킷, 아이템 및 기본공식들 수정버전
// 2006-01-13(금) Ver 1.5.22 : 스킬어빌리티 프로토콜 변경, 마을이동에 따른 시작 위치 저장, GM스트링 처리 버전
// 2006-01-11(수) Ver 1.5.21 : 스킬코드 체계 변경, 아이템 관련 기획 수정안 반영, 미션 보상 스크립트 처리 버전
// 2006-01-05(목) Ver 1.5.20 : 헬멧 옵션화 기능, 캐릭터정보 마을 전용 패킷 추가, 에텔레인 마을 추가 버전  
// 2006-01-04(수) Ver 1.5.19 : 마을간 이동, 캐릭터 머리/얼굴/키 Customize 버전
// 2006-01-02(월) Ver 1.5.18 : 길드 생성 및 가입, PC방 경험치 상승처리, 패킷 3차 최적화 버전
// 2005-12-23(금) Ver 1.5.17 : 친구 프로토콜 구현, 길드DB, 소환체 버그 수정 버전
// 2005-12-16(월) Ver 1.5.16 : 소환체 스킬, 릴레이 헌팅방, 아이템 시리얼 중복 처리 버전
// 2005-12-07(수) Ver 1.5.15 : PVP방생성, 마을채팅 뷰포트처리, 앉기/서기, 몬스터 크리티컬, 실시간 통계 처리 버전
// 2005-11-22(화) Ver 1.5.14 : 캐릭터 이모션, GM 클라이언트에서 공지 날리기 버전
// 2005-11-17(목) Ver 1.5.13 : 기본 PVP전투, 몬스터 이속/공속 처리 버전
// 2005-11-13(일) Ver 1.5.12 : [Gstar 마지막 버전] 엘리 부활스킬 버그 수정, 상거래 버그 수정 버전
// 2005-11-11(금) Ver 1.5.11 : 아이템 드롭이 잘 안되던 버그 수정, 로비에서 파티장 버그 수정 버전
// 2005-11-10(목) Ver 1.5.10 : GM 귓말제한 기능, 디바인아이템 방어구 버그 수정, 트리거 리젠 버그 수정 버전
// 2005-11-09(수) Ver 1.5.9  : [GStar 시작 Version] 중간보스 레젠 처리, 아이템 판매 공식 수정 버전
// 2005-11-08(화) Ver 1.5.8  : 상위 몬스터 추가 아이템 드롭, 마을 시작위치 저장, 캐릭터 Stat 계산시 Minus(-)값 처리 버전 
// 2005-11-07(월) Ver 1.5.7  : 기본공식/가격공식 수정, 회복스킬 버그 수정 버전
// 2005-11-02(수) Ver 1.5.6  : 몬스터 스킬(녹백,다운), 발키리 스플래쉬 데미지 처리, 월드서버 재접속 처리 버전
// 2005-10-30(일) Ver 1.5.5  : [클로즈 베타 2차 마지막 버전] 파티 경험치 추가 비율 수정 버전
// 2005-10-29(토) Ver 1.5.4  : 지형엔진에 이상한 값에 대한 검증 추가
// 2005-10-28(금) Ver 1.5.3  : SUNLog 부하 거리는거 수정, 파티 잘못 체크인된 버그 수정, 디바인 아이템 확률 조정 버전
// 2005-10-27(목) Ver 1.5.2  : 채널당 방번호 재할당 못하는 버그 수정, 민첩 제한 오버플로우 수정, 귓속말 처리 버전
// 2005-10-25(화) Ver 1.5.1  : [클로즈 베타 2차 시작버전]
// 2005-10-24(월) Ver 1.4.16 : 캐릭터 생성 패킷 수정, 몬스터 방황 상태 수정, DB ServerInfo수정
// 2005-10-23(일) Ver 1.4.15 : 배틀서버의 방 생성 갯수 제한, 마을에서 걸어서 자동 입장 
// 2005-10-22(토) Ver 1.4.14 : 잿팟처리, 딜레이 스킬 버그 수정, 서버 섹터 분할버그 수정, 게임로그
// 2005-10-18(화) Ver 1.4.12 : 새로운 프로토콜 추가 및 변경(파티 타겟팅, 바로가기 등)
// 2005-10-14(금) Ver 1.4.11 : 새로운 프로토콜 추가 및 변경, 클베 1차 버그 리스트 수정버전
// 2005-10-01(토) Ver 1.4.9  : 클라이언트 패치를 하기 위해 버전 올림(프로토콜 버전과 리소스 패치 버전의 분리 필요함)
// 2005-09-30(금) Ver 1.4.8  : 창고 기능 정지, 클로즈 베타 세째날에 발견된 버그들 수정
// 2005-09-29(목) Ver 1.4.7  : 클로즈 베타 1차 둘째날에 발견된 버그들 수정
// 2005-09-28(수) Ver 1.4.6  : [클로즈 베타 1차 시작 버전 ] 첫날에 발견된 버그들 수정
// 2005-09-25(일) Ver 1.4.5  : 패킷 최적화 작업으로 패킷 Size변경, 서버간 프로토콜 변경, 몬스터 스킬 배포, 리소스 변경
// 2005-09-24(토) Ver 1.4.4  : 패킷 최적화 작업으로 패킷 변경, 리소스 변경
// 2005-09-23(금) Ver 1.4.3  : 방 구조 변경에 따른 파티관련 프로토콜 변경
// 2005-09-22(목) Ver 1.4.2  : 캐릭터 상태정보 프로토콜 변경
// 2005-09-21(수) Ver 1.4.1  : 방생성 구조 변경, 로그인/인증 프로토콜 변경, 맵 재배포, 스킬 버그 수정
// 2005-09-17(토) Ver 1.3.5  : 맵 재배포(화산맵 추가), 몬스터 스킬 1차 구현, 마을 섹터 분할, 스킬 버그 수정
// 2005-09-11(일) Ver 1.3.4  : 인첸트 버그 수정, 스킬/스타일 파서 통합, 파티관련 경험치 공식 수정
// 2005-09-08(목) Ver 1.3.3  : 스킬버그 수정, 스타일 레벨업 버그 수정, 미션 보상 수정
// 2005-09-07(수) Ver 1.3.2  : 물약 구매시 버그 수정, 아이템 소유권 버그 수정, 미션맵 트리거 수정, 스킬 버그 수정
// 2005-09-06(화) Ver 1.3.1  : 맵 재배포 및 프로토콜 변경(퀵슬롯)
// 2005-09-05(월) Ver 1.2.1  : 맵 재배포 받음, 맵버전이 다를 경우 서버에서 필드입장을 허용하지 않음
// 2005-09-02(토) Ver 1.1.1  : 버전 체크 시작
//------------------------------------------------------------------------------------------------------------------


#endif // __VERSION_H__

