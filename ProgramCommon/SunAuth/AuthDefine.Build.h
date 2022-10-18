#ifndef __AUTH_DEFINE_BUILD_H__
#define __AUTH_DEFINE_BUILD_H__

//==================================================================================================
//
#include "AuthCountryCode.h" // 국가 코드
//
//==================================================================================================
//
#define _AUTH_NATION_CODE_GENERIC   (0)
#define _AUTH_NATION_CODE_KOREA     1000000
#define _AUTH_NATION_CODE_JAPAN     2000000
#define _AUTH_NATION_CODE_USA       3000000
#define _AUTH_NATION_CODE_TAIWAN    4000000
#define _AUTH_NATION_CODE_CHINA     5000000
// 600000 - 현재 ENGLAND라는 이름으로 사용중...(North America)
#define _AUTH_NATION_CODE_GLOBAL    7000000 // (090826) (ADD-IN)
#define _AUTH_NATION_CODE_RUSSIA    8000000 // (20110110) (ADD-IN)
//
//==================================================================================================
// 국가별 디파인 관리(버전에 관계없이 해당 국가에만 들어가는 디파인을 정의)
#if defined(_AUTH_KOREA)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_KOREA
    #define _NATION_BUILD_VERSION   905
    #define _BILLING_ONLYPCBANG     // PC방만 정량 차감/로그아웃 체크
    #define _BILLING_WAITLOGOUT     // 사용자 로그아웃 요청했을 때 Ans 올 때까지 별도로 대기시킨다
    #define _BILLING_DISCONNECT     // 빌링 만료되었을 때 끊어주는 버전
    #define _RESTRICT_ADULT         // 국내 청소년이용불가 판정으로 인하여 18 미만은 접속 불가
    #define _CHANNELING_AUTH_FLOW   // __KR000000_090128_CHANNELING__
    #define _CHANNELING_AUTH_FLOW_HANGAME_USER_ // 채널링 + 한게임 유저 구분

    #define _PCBANG_POLICY_CHANGE_POST_EXPIRED // PC방 정책 변경에 의해, 가맹PC방의 정액 만료 여부에 따른 구분 처리
    #define _PCBANG_POLICY_CHANGE_POST_EXPIRED2 // PC방 정액 만료 후 재 충전시 설정
    #define __NA_20100201_INTERGRATEDBILLING_   //2010.02.01 문상현 / 통합빌링 적용
    //#define __PCBANG_IP_UPDATE                  //통합빌링에서 피씨방 등록된 아이피 목록이 변경되었을때 바로 처리해 준다.
    #define _NA000000_CONFIGURATION_TO_ACCEPT_MULTI_AGENT_ON_SAME_IP_ADDRESS // 이은파 / 인증서버가 동일 IP에서 구동되는 Agent를 지원하기 위한 처리
#elif defined(_AUTH_CHINA)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_CHINA
    #define _NATION_BUILD_VERSION   816
    #define _GM_IPCHECK_DISCONN     // GM이 사내 IP 아닌 것으로 접속하면 끊어버린다
    //#define _SECURE_AUTH            // 보안 인증 추가
    //#define _NEW_MATRIX             // 중국 매트릭스 카드 연속 입력
    #define _TIRED_CHECK            // 중국 피로도 체크 시스템
    #define _AUTH_MATRIX_FAIL_COUNT // FailCount 중국인터페이스
    #define _ACCOUNT_ID_LENGTH_EXTENTION_   // 중국 계정 ID길이 50자리까지 확장
    #define __NA000000_090430_ADDIN_DROP_CODE__ // 2009.04.30 / 이은파 / 웹을 통한 접종 지원
	#define __CN_20100909_SECURITY_TOKEN__		//토큰 인증
    #define _CN_ZY_AUTH_PROCESS // 중국 ZY 인증
    //#define _CN_ZY_AUTH_PROCESS_DBAUTH // 중국 ZY 인증 - DB인증 보안 적용
#elif defined(_AUTH_TAIWAN)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_TAIWAN
    #define _NATION_BUILD_VERSION   815

#elif defined(_AUTH_JAPAN)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_JAPAN
    #define _NATION_BUILD_VERSION   905
    #define _NET_CAFE       // 일본 넷 카페 관련 DB연동
        #undef _NA_005011_20120621_MULTICLIENTS	  // 13.10.08 멀티클라이언트 보류
    //#define _JP000000_ONETIMEPASSWORD_AUTHFLOW_ // |10.01.18|이은파|One-Time Password Authentication flow 추가 작업|
    #define _JP_0_20100723_GAMECHU_AUTHFLOW_    // 문상현 게임츄 관련 인증 작업
    #define _JP_0_20101123_PMS                   //일본 게임온의 서버머 관리 시스템인 PMS 적용
#elif defined(_AUTH_USA)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_USA
    #define _NATION_BUILD_VERSION   902
    #define __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__  // 09.06.29 / 이은파 / 북미 채널링
    #define __NA000000_SHOP_USERID_50_PASSING_PROC__ // 09.07.28 / 이은파 / 샵서버에 UserID(50)전달

#elif defined(_AUTH_GLOBAL)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_GLOBAL
    #define _NATION_BUILD_VERSION   902
    #define __NA000000_GLOBAL_AUTH_PROCESS__    // 09.08.26 / 이은파 / 글로벌 서버 인증 프로세스
    #define _NA000000_091005_EXTENDED_LOG_ // 2009.10.05 / 이은파 / 인증 접속 로그 강화|수정
    #define __NA000000_SHOP_USERID_50_PASSING_PROC__ // 09.07.28 / 이은파 / 샵서버에 UserID(50)전달

	#define _PCBANG_POLICY_CHANGE_POST_EXPIRED  // PC방 정책 변경에 의해, 가맹PC방의 정액 만료 여부에 따른 구분 처리
	#define _PCBANG_POLICY_CHANGE_POST_EXPIRED2 // PC방 정액 만료 후 재 충전시 설정
	#define __NA_20100201_INTERGRATEDBILLING_   //2010.02.01 문상현 / 통합빌링 적용
	#define __20100916_BILLING_GODLCHANNEL__    //골드 채널 서비스

#elif defined(_AUTH_RUSSIA)
    #define _AUTH_NATION_CODE       _AUTH_NATION_CODE_RUSSIA
    #define _NATION_BUILD_VERSION   902
    #define _NA000000_091005_EXTENDED_LOG_              // 2009.10.05 / 이은파 / 인증 접속 로그 강화|수정
    #define __NA000000_SHOP_USERID_50_PASSING_PROC__    // 2009.07.28 / 이은파 / 샵서버에 UserID(50)전달
    #define _RU_INGAMBA_AUTH_PROCESS                    // 2011.03.14 / 민정영 / 러시아 Ingamba 인증 프로세스
    #define _PCBANG_POLICY_CHANGE_POST_EXPIRED          // PC방 정책 변경에 의해, 가맹PC방의 정액 만료 여부에 따른 구분 처리
    #define _NA_20120215_RUSSIA_PREMIUM_SERVICE         // 2012.02.09 / 민정영 / 프리미엄 서비스
#endif

// 현재 개발중인 디파인들
#if( _NATION_BUILD_VERSION >= 10000 )

    //#define _SHAPE_AUTH           // 형상 인증 시스템
    //#define _PAAS_ALERT           // 중국 PAAS 시스템
    //#define _NEW_BILLING_SYSTEM   // 신규 통합빌링 적용

#endif

// 최신 적용된 버전
#if (_NATION_BUILD_VERSION >= 905)
    #define __NA000000_SHOP_USERID_50_PASSING_PROC__ // 09.07.28 / 이은파 / 샵서버에 UserID(50)전달
#endif

#if (_NATION_BUILD_VERSION >= 903)
    #define _NA000000_091005_EXTENDED_LOG_ // 2009.10.05 / 이은파 / 인증 접속 로그 강화|수정
#endif

#if (_NATION_BUILD_VERSION >= 902)
    #define __NA001334_090420_SERVER_SELECT_FACILITY__    // 2009.04.17 / 이은파 / 서버 선택 기능
#endif


#ifdef __NA_20100201_INTERGRATEDBILLING_
	#ifndef _PCBANG_POLICY_CHANGE_POST_EXPIRED2
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

#ifdef __20100916_BILLING_GODLCHANNEL__
	#ifndef __NA_20100201_INTERGRATEDBILLING_
	// 골드채널은 통합빌링을 통해 서비스 한다.
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
	#ifdef _BILLING_ONLYPCBANG
	// 골드 채널은 개인이 피씨방처럼 이용하는 것이므로 피씨방만의 디파인이 들어가면 안된다.
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

//러시아 프리미엄 서비스는 러시아 인증 프로세서 기반으로 동작해야 한다.
#if defined (_NA_20120215_RUSSIA_PREMIUM_SERVICE)
    #if !defined (_RU_INGAMBA_AUTH_PROCESS) || !defined (_PCBANG_POLICY_CHANGE_POST_EXPIRED)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_CHANNELING_AUTH_FLOW_HANGAME_USER_) && \
    !defined(_CHANNELING_AUTH_FLOW)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__PCBANG_IP_UPDATE) && \
    !defined(__NA_20100201_INTERGRATEDBILLING_)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif



//#define __NA_20100201_INTERGRATEDBILLING_   //2010.02.01 문상현 / 통합빌링 적용

#endif //__AUTH_DEFINE_BUILD_H__

