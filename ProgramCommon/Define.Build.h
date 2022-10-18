#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 공통 Define 구역
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// [네이밍 규칙]
// 1. 국가별 네이밍 : NA(관계없음), KR(한국), JP(일본), US(미국), 대만(TW), 중국(CN), 영국(UK)
// 2. 작업 요청서 번호 : 0(요청서없음)
// 3. 시작 날짜
// 4. 작업명
// [주석 규칙] : 구분자는 '/'
// 1. 작업 요청서 번호
// 2. 작업자들
// 3. 상세한 작업 내용 
// 4. define을 풀었을 경우 추가적인 작업을 해야 하는 내용

// ex)
// #define __[국가코드]_[작업번호]_[날짜]_[작업명]_
// #define __KR_000123_20061020_TOWER_OF_DEVIL		//< [123]	2006. 04. 24	/ 김민욱,유재영 / 칼버스의 역습,저주받은 탑 작업 / odbc를 SUNOnline_20455로 바꿔야 함
// #define __NA_0_20061020_TOWER_OF_DEVIL			//< [0]		2005. 02. 10	/ 김민욱,유재영 / 칼버스의 역습,저주받은 탑 작업 / odbc를 SUNOnline_20455로 바꿔야 함

#include "Version.h"


#define __NA_00000_20080410_FOREIGN_DEFINE			// 국가별 독립적인 DEFINE 따로관리
#ifdef __NA_00000_20080410_FOREIGN_DEFINE
	#include "Foreign.Build.h"
#endif

/*($Delete_[S/C]$)*/#define	__NA_000000_20080515_HACKSHIELD_PATH							// 2008.05.15 / 조재식, 김경표 / 핵쉴드 경로 자동화 작업.
/*($Delete_[S/C]$)*/#define __NA000000_080905_MOLA_VERSION_BUILD_ROUTING__					// 2008.09.05 / 이은파 / MOLA 라이브러리, 국가 및 버전별 라이브러리 경로 설정 라우팅 기능 추가
/*($Delete_[S/C]$)*/#define __NA000824_070103_SOLARAUTH_MASTER_NETWORK_MECHANISM_OPTIMIZE__	// 2007.01.03 / 이은파 / SolarAuth서버와 Master 서버간 네트워크 전송 메커니즘 최적화
/*($Delete_[S/C]$)*/#define __NA001098_080711_SERVER_PROCESS_CONTROL_BUG_FIX__				// 2008.07.11 / 이은파 / [824]연동, SolarAuth 및 기타 서버 컨트롤 미스 문제 수정 관련
/*($Delete_[S/C]$)*/#define _NA_DUPLICATION_CATEGORY_BUG                                    // 
/*($Delete_[S/C]$)*/#define __NA000000_ENABLE_MASTER_SERVER_SCRIPT_LOADING__				// 2008.09.11 / 이은파 / 마스터에서도 스크립트를 읽을 수 있도록 수정
// (WARNING) 하기 working macro는 해외에서 문제발생시 모니터링 하기 위한 것으로 지우지 말 것.
//#define __NA000000_080709_DBQUERY_FAULT_DETECTION__					// 2008.08.09 / 이은파 / DBProxy 메모리 누수 발생할 수 있는 부분 수정 (임시 코드도 좀 있음: 나중에 정리하겠음)
/*($Delete_[S/C]$)*/#define __NA000630_070817_MOLA_5th_UPDATE				// 2007/08/17 /이은파,김지완/가상 클라이언트 방지작업 (MOLA 5차 업데이트) - LINKED {__NA070212_ENHANCED_SECURE_POLICYMENT__}

/*($Delete_[S/C]$)*/#define __NA_0_20090311_ITEMCOPY_BUG_FOR_LEAVESYN_ERROR					// 2009.03.11 / 이준석 / 대만 아이템복사, LeaveSyn 처리부분에서의 오류

//==================================================================================================
//==================================================================================================
//==================================================================================================
// SECURITY MODULE SELECTOR
#if defined(__APPLY_HACKSHIELD)
    // using hackshield module
    #define USING_HACKSHIELD    (1)
    #define USING_XIGNCODE      (0)
    #define USING_FROST         (0)
    #define USING_GAMEGUARD     (0)
    //
    #if defined(_SERVER) || defined(_SUNGAME_VER)
        #define __NA000000_070913_HACKSHIELD_INSTALLATION__ // 2008.01.23 / lammy / 핵쉴드 관련
        #define _AHN_SMART_UPDATE_
    #endif
#elif defined(__APPLY_XIGNCODE)
    //
    #define USING_HACKSHIELD    (0)
    #define USING_XIGNCODE      (1)
    #define USING_FROST         (0)
    #define USING_GAMEGUARD     (0)
    //
#elif defined(__APPLY_FROST)
    // using frost module
    #define USING_HACKSHIELD    (0)
    #define USING_XIGNCODE      (0)
    #define USING_FROST         (1)
    #define USING_GAMEGUARD     (0)
    //
    #if defined(_SUNGAME_VER) && !defined(_SERVER)
        #define _YMS_USING_FROST_LIBRARY_ // 2010.3.22 / 유명수 / 러시아일때는 frost 사용
    #endif
#elif defined(__APPLY_GAMEGUARD)
    //
    #define USING_HACKSHIELD    (0)
    #define USING_XIGNCODE      (0)
    #define USING_FROST         (0)
    #define USING_GAMEGUARD     (1)
        #if defined(_DEV_VER)
            #define NO_GAMEGUARD
        #endif // _DEV_VER
    //
#endif
// Shared value
#define XIGNCODE_GAME_PACKET_SIZE   4000
#define GAMEGUARD_CSAUTH_PACKET_SIZE 4096
#define GAMEGUARD_HACKDATA_PACKET_SIZE 1024
//==================================================================================================
//==================================================================================================
//==================================================================================================

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.6.0.1 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1601
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// 하위 버전 번호
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008610_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / 민경승 / 새 코스튬슬롯 추가(보조코스튬)
    #define _NA_008633_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / 홍성현 / 데코 코스튬 추가
	#define _NA_008647_20160115_NPCSHOP_CALCULATE_CHANGE											// 2016.01.15 / 우호경 / NPC 상점 계산공식 변경
	#define _NA_008695_20160212_DRAGON_NIGHT_DRAGONTANS_BUG2										// 2016.02.15 / 우호경 / 드래곤나이트 용 변신(암드래곤트랜스폼10레벨) 스킬 사용 시 이속 능력치 적용 안되는 버그 수정
    #define _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION                        // 2016.02.03 / 홍성현 / 하임 거래량 알림 옵션 추가
	#define _NA_008702_20160224_CHANGING_HIT_POPULATIONS											// 2016.02.24 / 우호경 / 타격 개체수 변경 요청(스타일코드 추가)
    #define _NA_008706_20160229_TEXTLOG_PK                                                        // 2016.02.29 / 민경승 / 감시대상 pk시 텍스트로그
    #define _NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX                                        // 2016.03.08 / 송찬종 / 자동이동으로 포탈로 갈 때 플레이어 싱크 안맞는 문제 수정
    #define _NA_000000_20160315_ETHERWEAPON_SPECIALMOVE                                           // 2016.03.15 / 송찬종 / 발키리 에테르웨폰도 스페셜무브 추가
    #define _NA_008757_20160329_DOMINATION_GRAPHIC_CONTROL                                        // 2016.03.29 / 송찬종 / 점령전 그래픽 하향조절 기능 복구
    #define _NA_000000_20160308_ATTACK_RATE_RISE_BUG                                              // 2016.03.08 / 홍성현 / 공격속도 상승 버그 수정
	#define _NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE								// 2016.03.14 / 우호경 / 가해 데미지 HP/SD 전환 시 조건 추가 작업
    #define _NA_008732_20160315_WITCHBLADE_BALANCE_ADJUST                                           // 2016.03.15 / 홍성현 / 위치블레이드 밸런싱 조정 작업.
    #define _NA_008740_20160321_MAGICIAN_STAFF_CREATE_PROJECTILE                                    // 2016.03.21 / 홍성현 / 엘리멘탈리스트 지팡이 일반공격 발사체를 생성하도록 작업.
	#define _NA_008765_20160405_DISCONNECTION_REASON_LOG											// 2016.04.05 / 우호경 / 컨넥션 끊겼을 경우 이유를 로그에 남기는 작업
	#define _NA_008766_20160407_PROTECTIONSTATUS_MODIFICATION_ANOMALIES								// 2016.04.07 / 우호경 / 보호상태 이상현상 수정
    #define _NA_008783_20160418_DEFENCEMOTION_CANCEL_BUG                                            // 2016.04.18 / 홍성현 / 디펜스모션이 풀리는 버그 수정

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.5.0.4 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1504
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// 하위 버전 번호
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008334_20150608_SONNENSCHEIN                                  // 2015.06.08 / 민경승, 송찬종 / 몬트샤인 PvE, 소넨샤인
    #define _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL                     // 2015.10.01 / 민경승 / 공격자길드정보 제거, 점령전 사전알림 모든필드유저에게 보내게하기
    #define _NA_008472_20150903_IMPROVED_BALANCE                              // 2015.09.03 / 홍성현 / 밸런스 개선
    #define _NA_008486_20150914_TOTAL_BALANCE                                 // 2015.09.14 / 민경승 / 밸런스작업
    #define _NA_008536_20151023_ELITE4TH_ADD                                  // 2015.10.23 / 홍성현 / 엘리트4차 무기/방어구 추가 작업
    #define _NA_008540_20151027_ADD_ITEMOPTION_ELITE4                         // 2015.10.27 / 민경승 / 엘리트4 옵션 추가 작업
    #define _NA_000000_20151026_TRIGGER_TIMER_SCENE_TYPE_REMOVE               // 2015.10.26 / 홍성현 / 트리거 타이머 SCENE TYPE을 없앤다
    #define _NA_008559_20151103_QUEST_AUTO_ACCEPT_PAGE_FUNC_ADD               // 2015.11.03 / 홍성현 / 퀘스트 자동 수락 페이지 기능 추가
    #define _NA_008536_20151030_AWAKENING_MATERIAL_AMOUNT_ADD                 // 2015.10.30 / 홍성현 / 각성에 재료 개수를 추가하는 작업
    #define _NA_008570_20151116_MODIFY_VENDOR_SEARCH                          // 2015.11.16 / 민경승 / 개인상점 검색 시, 마을제한 제거
    #define _NA_008571_20151116_MODIFY_VENDOR_SEARCH                          // 2015.11.16 / 홍성현 / 개인상점 검색 시, 마을제한 제거
    #define _NA_000000_20151125_MOUSECLICK_AREA_SELECT_CANCLE                 // 2015.11.25 / 송찬종 / 마우스 클릭으로 스킬큐잉이 풀리는 조건 변경
    #define _NA_000000_20151126_MODIFY_COSTUME_CALCULATE_DATETIME             // 2015.11.26 / 민경승 / 코스튬아이템 기간설정관련 수정
    #define _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC                    // 2015.12.02 / 민경승 / 이동스킬 로직 위치변겅
    #define _NA_008606_20151222_MODIFY_GUILD_LEVEL                            // 2015.12.22 / 민경승 / 길드멤버 탈퇴 시, 레벨다운 되지 않도록 수정
    #define _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST               // 2016.02.04 / 민경승 / 적용중인 세트장비효과를 따로 저장시켜둔다

	#define _NA_008543_20151027_ONE_DAYS_GUILD_MISSION						  // 2015.10.29 / 우호경 / 일일 길드 미션 기능 구현

    #if defined(_KOREA)
        #define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                  // 2015.11.05 / 민경승 / 이속공식 변경

    #elif defined(_CHINA)
		#define _CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE       // 2015.03.29 / 우호경 / 중국 AC 보상 엘리트 아이템 전용 거래타입 추가(거래제한 + PK드랍 가능)

    #elif defined(_JAPAN)
        #define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                  // 2015.11.05 / 민경승 / 이속공식 변경
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.5.0.3 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1503
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// 하위 버전 번호
    #endif

    //////////////////////////////////////////////////////////////////////////

    #define _NA_008283_20150511_NOTIFICATIONS_SYSTEM                          // 2015.05.11 / 홍성현, 임상수 / 알림 시스템
    #define _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION                  // 2015.06.17 / 송찬종 / 온천 신규 스타일 모드에 따른 특수 애니메이션 작업
    #define _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL                 // 2015.07.10 / 송찬종 / 수정기둥 ui의 텍스트 중 레벨 값을 스크립트에서 읽어와서 출력
    #define _NA_000000_20151015_CALC_PORTAL_OFFSET_PROBLEM_MODIFY             // 2015.10.15 / 송찬종 / 포탈명 출력 위치 조정 중 무한루프 문제 수정
    #define _NA_008389_20150703_TIMERITEM_PET_DROP_RULE                       // 2015.07.03 / 민경승 / 시간제 펫아이템 드랍될 시, charge sub type 1번도 가능하도록 수정
    #define _NA_008404_20150712_MINIGAME_SYSTEM                               // 2015.07.12 / 임상수 / 미니게임 시스템 추가
    #define _NA_008405_20150716_MINIGAME_BINGO                                // 2015.07.16 / 홍성현 / 미니게임 빙고
    #define _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE                // 2015.08.18 / 임상수 / 즉시사용소멸타입 추가 
    #define _NA_008441_20150819_CRITICAL_DAMAGE_CHANGE_FROM_LEVEL_DIFFERENCCE // 2015.08.19 / 임상수 / 레벨차이에 따른 크리티컬 데미지 증감 수치 변화
    #define _NA_008450_20150818_MODIFY_GUILD_RELATION_SYSTEM                  // 2015.08.25 / 임상수 / 길드 레벨 하락시 길드 관계설정 권한이 없을경우 모든 길드관계를 삭제하도록 수정
    #define _NA_008480_20150909_MODIFY_FIGHTING_ENERGY                        // 2015.09.09 / 민경승 / 스피릿리차지, 검기 풀 충전상태에서도 스킬사용 가능
    #define _NA_008481_20150909_MODIFY_REMOVE_ATTSPEED_300LIMIT               // 2015.09.09 / 민경승 / 공속 300제한 해제
    #define _NA_008502_20150922_BATTLEZONE_LOCK_ICON_BUG                      // 2015.09.22 / 홍성현 / 배틀존의 락 아이콘이 제대로 표시되지 않는 현상 수정 작업
    

    #if defined(_KOREA)
        #if defined(_SUNGAME_VER)
            #define _NA_008364_20150623_CHANGE_TEXT_PATH                              // 2015.06.23 / 홍성현 / TEXT 경로 변경
        #endif // _SUNGAME_VER

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.5.0.2 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1502
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// 하위 버전 번호
    #endif

    //////////////////////////////////////////////////////////////////////////

	#define _NA_008012_20150204_SUN_RANKING_SYSTEM                              // 2015.02.04 / 임상수 / Sun Ranking System
	#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM                              // 2015.02.03 / 홍성현 / SUN 랭킹 시스템
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE                        // 2015.02.03 / 홍성현 / SUN 랭킹 등급 시스템
	#define _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE                       // 2015.02.03 / 홍성현 / SUN 랭킹 시스템 동상
	#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
    #define _NA_008124_20150313_AWAKENING_SYSTEM                                // 2015.03.13 / 임상수 / 각성 시스템
    #define _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM                      // 2015.03.11 / 홍성현 / 장비 각성 시스템
    #define _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM                      // 2015.03.18 / 홍성현 / 장비 진화 시스템
    #define _NA_008069_20150210_ADD_SPA_SYSTEM                         // 2015.02.10 / 민경승, 송찬종 / 온천시스템
    #define _NA_008139_20150318_MODIFY_BUFF_ICON_UI                    // 2015.02.10 / 송찬종 / 버프아이콘(Status) UI 수정
    #define _NA_008185_20150402_COSTUME_ITEM_FOR_NPC                   // 2015.04.02 / 송찬종 / 코스튬 NPC입히기
    #define _NA_008226_AC_PARENTMAP_LIST_SKIP                          // 2015.04.15 / 송찬종 / AC 선행과제 리스트 목록이 길어질 때 생략하기 기능 추가
    #define _NA_008235_BOSS_MONSTER_CRITICAL_DAMAGE_ANIMATION_IGNORE   // 2015.04.23 / 송찬종 / 보스(캡틴)몬스터 크리티컬 타격 애니메이션 무시
    #define _NA_008252_20150427_AURASKILL_PACKET_PENDING               // 2015.04.27 / 송찬종 / 오오라스킬 패킷 중복처리 문제
    #define _NA_000000_20150428_ATTENDANCE_CHECK_SYSTEM_SUPERPOSITION_TYPE_MODIFY  // 2015.04.28 / 송찬종 / 출석체크 시스템 누적형 정렬방식 수정 (달력형과 동일하게)
    #define _NA_008296_20150518_HELLOID_SPIN_CANCLE_BUG_FIX            // 2015.05.18 / 송찬종 / 헬로이드 스핀캔슬 버그 수정
    #define _NA_008298_20150519_ALLOW_HEROMOVE_TO_DOWN_STATE           // 2015.05.19 / 송찬종 / 이동중에 다운이 안걸리는 현상 수정(디버프감소시간4초에 의해서 발생)
	#define _NA_008302_20150601_QUEST_COMPLET_SYN_RECAST               // 2015.06.01 / 송찬종 / 퀘스트 완료 실패 시 재요청 기능 추가
    #define _NA_000000_20150604_BUG_FIND_TEST                          // 2015.06.04 / 송찬종 / 버그 찾기용 테스트 코드 (CTRL+F10 으로 퀘스트 네비게이션 위치 초기화)
    #define _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX       // 2015.06.09 / 송찬종 / 회전 이동 시 움직임 버그 수정
    #define _NA_000000_20150616_SPA_COSTUME_SHOW_HAIR                  // 2015.06.16 / 송찬종 / 온천옷 머리 보일 수 있도록 플래그로 설정하도록 개선
    #define _NA_007863_20141203_VOLUME_LIGHT_SCATTERING                         // 2014.12.03 / 홍성현 / 볼륨 라이트가 제대로 작동하지 않던 현상 수정
    #define _NA_008168_20150330_IMPROVED_REGISTRATION_OF_NEW_SHORTCUTS          // 2015.03.30 / 홍성현 / 신규 단축키 등록 개선
    #define _NA_008166_20150330_MODIFY_AC_LEVELUP                                     // 2015.03.30 / 민경승 / 레벨업AC이 경우 '이전레벨' AC도 같이 검색해서 처리하도록한다
    #define _NA_008016_20150331_SUN_RANKING_SYSTEM_SETTLE_DATE_VIEW             // 2015.03.31 / 홍성현 / SUN 랭킹 시스템 정산일 보여주는 작업
    #define _NA_008182_20150402_MODIFY_ITEM_MOVE                                        // 2015.04.02 / 민경승 / 아이템 이동관련 문제 수정
    #define _NA_008243_20150422_DOMINATION_SCENE_OPEN_NPC_STORE               // 2015.04.22 / 홍성현 / 점령전 씬에서 NPC상점을 열 수 있도록 수정
    #define _NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE                       // 2015.04.22 / 몬스터가 안전지대에서 귀환루트를 못찾을경우 텔레포트시켜버린다
    #define _NA_008256_20150428_MODIFY_CANCLE_STAMP_LOGIC                       // 2015.04.28 / 임상수 / 각인 취소 로직 수정
    #define _NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME                             // 2015.04.29 / 다크오브라이트같은 'Effect'를 사용하는 스킬이 사용자가 죽을경우 같이 사라지도록 한다
    #define _NA_008270_20150511_MODIFY_OUT_OF_RANGE                             // 2015.05.11 / 임상수 / DominationBattleResult에서 OutOfRange 나는 현상 수정
    #define _NA_008299_20150519_MAX_DAMAGE_LOG                                  // 2015.05.19 / 임상수 / 최대데미지 로그 작업
    #define _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK                               // 2015.06.03 / 민경승 / 윈드실드 효과가 강제어택 먹히도록한다
    #define _NA_008321_20150601_BATTLE_INFO                                                 // 2015.06.01 / 민경승 / 밸런스작업을 위해 클라에 전투정보 전달하기
    #define _NA_008333_20150615_BROADCAST_LEFT_RIGHT_TURN                       // 2015.06.15 / 임상수 / 좌우회전 패킷도 브로드캐스트 하도록 수정
    #define _NA_008333_20150615_FIXED_MAX_LEVEL_EXP                             // 2015.06.15 / 임상수 / 만렙 달성시 경험치 고정
    #define _NA_000000_20150714_MOVEMENT_DISTANCE_CHECK                         // 2015.07.14 / 남기봉 / 스피드핵 체크. 이동 가능 거리 체크
    #define _NA_008473_20150904_HELLOIDSPIN_BACKSTEP                                            // 2015.09.04 / 민경승 / 헬로이드스핀 상태에서 BACKSTEP이동은 정면달리기와 같은 속도를 가진다
    #define _NA_007288_20140611_GAMELOG_GAMESHOP                          // 2014.06.11 / 민경승 / 캐시샵의 아이템 구매, 선물관련 DB로그 기록하도록 추가
    #define _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG    // 2015.10.29 / 송찬종 / 앉기 패킷 SYN와 ACK사이의 딜레이동안 움직이면서 위치 안맞는 현상 수정

#ifdef _DEV_VER
    #define _NA_008316_20150604_DEV_VER_SHOW_DAMAGE_INFO                // 2015.06.04 / 송찬종 / 개발모드에서 데미지 가해량을 표시해주는 작업
#endif //_DEV_VER

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.5.0.1 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1501
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(2)		// 하위 버전 번호
    #endif

    //////////////////////////////////////////////////////////////////////////
    #define _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT   // 2015.01.13 / 송찬종 / 개인 웨이포인트 최대 저장 갯수 증가 (10->15)
    #define _NA_000000_20150114_MODIFY_MISSION_REWARD_DIALOG_SLOT     // 2015.01.14 / 송찬종 / 미션보상창 슬롯 출력,툴팁 수정
    #define _NA_007726_20141015_PROTECTION_SCREENSHOT                 // 2014.10.15 / 송찬종 / 스크린샷 암호화(압축) 기능
    #define _NA_007865_20141403_LIGHTMAP_BUILD_EXCLUDE_OBJECTS        // 2014.12.03 / 송찬종 / 라이트맵 빌드 시 오브젝트 관련 제외 & 기능 부활
    #define _NA_007942_20150115_ADD_MAINMENU_DIALOG_ICON              // 2015.01.15 / 송찬종 / 메인메뉴 대륙지도 버튼 추가
    #define _NA_008090_20150226_GAMEOPTION_RECVPACKET_DONT_WAIT       // 2015.02.26 / 송찬종 / 게임종료 시 정보열람실 저장완료 패킷을 기다리지 않고 바로 종료
    #define _NA_007919_20141211_ADD_GMCOMMAND_EVENT								// 2014.12.11 / 민경승 / GM명령어, 경험치이벤트 제어하기
    #define _NA_007935_20150109_GM_COMMAND_EXP_EVENT                            // 2015.01.09 / 홍성현 // GM 명령어 추가 개발(GM명령어로 경험치 이벤트 만들기)
    #define _NA_007932_20150106_USER_EXTRA_INFO                                 // 2015.01.06 / 임상수 / UserExtraInfo 추가
    #define _NA_007973_20150120_ADD_MULTIOPTION_ZARD                            // 2015.01.15 / 민경승 / 어떤쟈드를 합성해도 결과물은 랜덤으로 나오도록 수정
    #define _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC                // 2015.01.27 / 임상수 / AgentServer의 packet handler에서 ProtocolMatrix를 정적 객체로 쓰도록 수정
    #define _NA_000000_20150130_ITEMSHOP_NOT_SLOTTEXT_ALL_FILL                  // 2015.01.29 / 홍성현 / 아이템 샵에서 슬롯의 설명 텍스트를 줄이지 않고 표시
    // #define _NA_008012_20150130_RANKING_SYSTEM                                  // 2015.02.02 / 임상수 / 기존 랭킹 시스템 디파인 설정
    #ifdef _NA_008012_20150130_RANKING_SYSTEM
        #define _NA_008016_20150202_OLD_RANKING_SYSTEM                           // 2015.02.02 / 홍성현 / 기존 랭킹 시스템 디파인 설정
    #endif // _NA_008012_20150130_RANKING_SYSTEM
    #define _NA_007992_20150127_EVENT_MONSTER_INVASION                              // 2015.01.27 / 민경승 / 몬스터침공 이벤트매니저로 제어
    #define _NA_008078_20150211_GM_COMMAND_TIME_DATE                            // 2015.02.12 / 임상수 / GM명령어추가, 서버 시간 및 날짜 변경
    #define _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE                              // 2015.02.25 / 민경승 / 크리스탈펠레스,뱃지아이템 시간만료시 제대로 해제해주지않는문제
    #define _NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION                     // 2015.03.10 / 민경승 / 플레이어끼리 싸움에서 한방에 죽는경우 체크하고 로그로 기록한다
    #define _NA_008150_20150331_MODIFY_EX2_CHARGE_SUB_TYPE_USABLE_ANOTHER_FUNC      // 2015.03.31 / 임상수 / eCHARGE_SUB_FIXED_AMOUNT_EX2 타입 아이템을 DROP 및 LOTTERY로 얻지 못하는 현상 수정
    #define _NA_008220_20150414_BUG_ETHERBULLET_ATTR                                  // 2015.04.14 / 바이스소드같은 지속적으로 동작하는 스킬이 마야탄환의 증뎀률 증가 계산에 문제를 발생시킨다
	#define _NA_000000_20150511_QUEST_BUG                                                 // 2015.05.11 / 민경승 / 퀘스트버그관련작업, db프로시저와 패킷간 싱크조절

    #if defined(_KOREA)
        #define _NA_008132_20150316_MONDSCHEIN_TEXTLOG                                  // 2015.03.16 / 민경승 / 몬트샤인용 텍스트로그, 한방데미지문제 감시용
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM                         // 2015.01.13 / 임상수 / 출석 시스템 추가

    #elif defined(_CHINA)
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM                         // 2015.01.13 / 임상수 / 출석 시스템 추가

    #elif defined(_JAPAN)
        //#undef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        #define _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM_CLOSE                   // 2015.10.16 / 송찬종 / 출석체크 버튼 ui에서 숨기기
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.4.0.4 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1404
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(6)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(1)		// 하위 버전 번호
    #endif

    //////////////////////////////////////////////////////////////////////////
    #define _NA_007732_20141024_RIDER_MOVESPEED_SYNC     // 2014.10.24 / 송찬종 / 다른플레이어의 탈것 이속 싱크 마춤
    #define _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE // 2014.08.28 / 송찬종 / 신규클래스 - 위치블레이드
    #define _NA_000000_20141222_DOMINATION_RIDER_ACTIVATE // 2014.12.22 / 송찬종 / 점령전에서 탈 것 허용
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    #define _NA_007667_20141001_WITCHBLADE_SKILL                // 2014.10.01 / 민경승 / 위치블레이드 스킬작업
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    #define _NA_007620_20140915_STATE_RESISTS // 2014.09.15 / 정현성 / 상태 면역2
    #define _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN // 2014.09.16 / 홍성현 / 에어블로우 다운 처리방식변경
    #define _NA_007264_20140529_BUG_CALCULATE_SUMMONED_ATTRIBUTES       // 2014.05.29 / 민경승 / 발키리소환수 능력치 계산식 오류 수정
    #define _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION             // 2014.10.27 / 민경승 / 모든 인원의 입장아이템 검사를 하는 존타입 추가
    #define _NA_007810_20141112_MODIFY_MSG_RENDER_EFFECT_ITEM_LIST_PACKET // 2014.11.12 / 임상수 / 패킷 비정상적인 자료구조 수정
    #define _NA_007750_20141113_WZDAT_MODIFY_LOG                          // 2014.11.13 / 임상수 / WZDAT 관련 로그 수정
    #define _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE                 // 2014.11.18 / 민경승 / 파티상태에서 캐릭터 상태(HP,MP,SD,FP) 업데이트 방식 변경
    #define _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE                         // 2014.11.27 / 민경승 / 펫도 소모타입 11번을 쓸수있도록한다
	#define _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER                       // 2014.12.22 / 임상수 / 몬스터 이동 트리거 작동하지않는 현상 수정
    #define _NA_007924_20150105_MINIMAP_PORTAL_STRING_OUTRANGE_BUG // 2015.01.05 / 홍성현 / 포탈 텍스트가 미니맵 UI 위에 겹쳐져 출려되는 현상 수정

    #define _NA_008323_20150602_MODIFY_ITEM_SERIALNUMBER_40BITDATA                          // 2015.06.02 / 민경승 / 아이템시리얼넘버를 40비트 까지 사용할수있도록해준다
    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.4.0.3 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1403
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(6)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(0)		// 하위 버전 번호
	#endif
	
	//////////////////////////////////////////////////////////////////////////
    #define _NA_007369_20140625_ACTION_OPERATE_SWITCH_EX // 2014.06.25 / 정현성, 송찬종 / ACTION_OPERATE_SWITCH_EX 트리거 추가
    #define _NA_007156_20140703_TOP_OF_TRIAL_LIGHT // 2014.07.03 / 정현성 / 저주받은탑 빛의시련
    #define _NA_007086_20140318_MONSTERBOOK // 2014.03.18. // 정현성,송찬종 / 몬스터도감
    #define _NA_007477_20140805_RADAR_RAYER_MODIFY // 2014.08.05 // 송찬종 / 미니맵의 층 표시 변경(인던 외에도 표시 가능)
    #define _SCJ_TOGGLE_EDITBOX_CONTROL // 2014.07.22 / 송찬종 / 토글 에디트박스 컨트롤추가
    #define _NA_007407_20140710_CHALLENGE_DAILY_QUEST                       // 2014.07.10 / 민경승 / 도전일일퀘 추가
    #define _NA_007330_20140620_GUILD_SYSTEM_EXTENSION                      // 2014.06.20 / 홍성현, 임상수 / 길드 시스템 확장
    #ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        #define _NA_004034_20120102_GUILD_INTRODUCTION                          //2012.05.10 / 정현성,이도형 / 길드 소개 작업
    #endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    #define _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP // 2014.07.22 / 정현성 / 기간제 아이템 상점 판매
    #define _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION       // 2014.07.18 / 민경승 / 재작재료가 장비일 경우 '인챈트'에 관련된 조건 변경
    #define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / 임상수 / 몬트샤인 공성전 초기화 및 보상 지급 수정

    #define _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG                     // 2014.08.11 / 민경승 / DB길드로그 작업(LogConsole)
    #define _NA_007408_20140711_MODIFY_CRYSTALLIZE                           // 2014.07.11 / 민경승 / 결정화 바꾸기
    #ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
        /*($Delete_[x/C]$)*/#define _NA_007450_20140805_CRYSTALIZATION_RENEWAL                   // 2014.08.05 / 홍성현 / 결정화 리뉴얼
    #endif // _NA_007408_20140711_MODIFY_CRYSTALLIZE
    #define _NA_007665_20140930_CASHSHOP_COSTUME_PREVIEW_BUG // 2014.09.30 / 홍성현 / 캐시샵 코스튬 미리보기 기능에서 성별의 애매함으로 인하여 성별 선택기능을 보이지 않는다.
    #define _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM    // 2014.10.14 / 임상수 / 일반 상태에서는 PK당해도 리벤지 대상이 아닐경우 아이템을 드랍하지 않는다

	#if defined(_KOREA)

    #elif defined(_CHINA)
		#undef _NA_007710_20141014_MODIFY_CHAO_STATE_NORMAL_DO_NOT_DROP_ITEM // #7950 // 중국 요청으로 해당 디파인 비활성화
        #define _NA_007988_20150127_MODIFY_PROTOCOL_MATRIX_TO_STATIC // 2015.01.27 / 임상수 / AgentServer의 packet handler에서 ProtocolMatrix를 정적 객체로 쓰도록 수정

	#elif defined(_JAPAN)

	#elif defined(_GLOBAL)

	#elif defined(_RUSSIA)

	#endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.4.0.2 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1402
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(9)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_000000_20140423_SUMMON_BACKGROUND_PICKING             // 2014.04.23 / 송찬종 / 소환수 후선 픽킹
    #define _NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE       // 2014.04.08 / 송찬종 / 섀도우 소환수 외형 변경
    #define _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION     // 2014.04.08 / 송찬종 / 스킬 사용 후 자동공격 전환 여부 옵션화
    #define _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL         // 2014.03.24 / 송찬종 / 섀도우 스킬 밸런스 리뉴얼
    #define _NA_000000_20140409_ADD_SHADOW_AREA_SKILL_POINTER         // 2014.04.08 / 송찬종 / 섀도우 전용 마법진 추가  
    #define _NA_007124_20140324_SHADOW_MODIFIED_SKILL_SOULCONTROL_SUMMON_GRADE    //2014.03.24 / 임상수 / 섀도우 소울 컨트롤 수정 - 보스 및 캡틴도 소환할 수 있도록 변경(클라 작업 송찬종)
    #define _NA_007123_20140324_SHADOW_SKILL_RENEWAL                     //2014.02.24 / 민경승 / 쉐도우 스킬 리뉴얼
    #define _NA_000000_20140224_SUMMONER_REFACTORING                // 2014.02.24 / 정현성 / 소환체 리뉴얼
    #define _NA_007128_20140326_MONDSCHIEN_DOMINATION            //2014.03.26 /임상수 /(몬트샤인) domination 스크립트 및 파서 수정, domination type 추가, 각인 성공시 턴 오버 추가
    #define _NA_007115_20140313_MONDSCHIEN_RENEWAL                 // 2014.03.13 / 홍성현 / 몬트샤인 리뉴얼
    #define _NA_000000_20140331_MAPLOADING_MEMORYPOOL              // 2014.03.31 / 정현성, 홍성현 / 맵로딩을 MemoryPool로 바꾸는 작업
    #define _NA_007136_20140404_MODIFY_SUCTION_DAMAGE_CALCULATE             //2014.04.04 / 민경승 / 쉐도우의 에너지석션 데미지공식변경
    #define _NA_007065_20140217_CHAOSZONE_GOLDRUSH                      // 2014.02.17 / 민경승 / 신규 전장모드 골드러시
    #define _NA_007114_20140313_CHAOSZONE_GOLDRUSH                      // 2014.03.13 / 홍성현 / 골드러쉬
    /*($Delete_[S/x]$)*/#define _NA_007065_20140217_BATTLEGROUND_REFACTORYING             // 2014.02.17 / 민경승 / 매칭시스템 리팩토링
    #define _NA_000000_20140120_SMART_NPC_SYSTEM // 2014.01.20. / 정현성,송찬종 / 지능형NPC
    #define _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL  //2014.04.02 /임상수 /전장예약은 전장 채널에서만 가능하도록 수정
    #define _NA_007191_20140428_MODIFY_GM_COMMAND                         // 2014.04.28 / 민경승 / GM명령어 추가/수정
    #define _NA_007266_20140530_BUG_ATTACH_STATE_TRIGGER                  // 2014.05.30 / 민경승 / 상태부여 트리거가 맵오브젝트한테 붙였을때만 작동한다
    #define _NA_007294_20140613_DRAGONKNIGHT_RENEWAL                    // 2014.06.13 / 임상수 / 드래곤 나이트 리뉴얼
    #define _NA_007256_20140613_AREAOWNER_ALLIENCE_GUILD_NAME_SHOW      // 2014.06.13 / 홍성현 / 캐릭터 접속 시 몬트샤인 성주길드와 동맹들을 같이 띄워 줄 수 있도록 개선

    #define _NA_007743_20141027_ADD_ISVAILD_CHECK                        // 2014.10.27 / 임상수 / 보호 코드 추가
    #define _NA_007770_20141103_BUG_AREACONQUEST_REWARD_CHECK                   // 2014.11.03 / 민경승 / 패킷변조에 의한 AC보상 무한받기문제 처리

    #if defined(_KOREA)

    #elif defined(_CHINA)
        #define _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY        // 2014.10.27 / 임상수 / MiniDumpWithFullMemory로 기록하도록 수정


    #elif defined(_JAPAN)
        #define _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT               // 2014.07.02 / 민경승 / 골드러시에서 골드포인트 변동사항을 DB로그에 남긴다
        #define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / 임상수 / 몬트샤인 공성전 초기화 및 보상 지급 수정
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.4.0.1 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1401
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(8)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_007019_20140107_PERK_CHANGE_SYSTEM // 2014.01.07 / 홍성현 / 커스텀 패시브 그룹으로 변경하는 작업
    #define _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX      // 2014.02.10 / 송찬종 / 전혀 다른 퀘스트 창이 열리는 현상 수정
    #define _NA_007029_20140120_DOMINATION_RESURRECTION_REFORM // 2014.01.20 / 송찬종 / 점령전 부활 방식 변경 (죽으면 팝업창)
    #define _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY      // 2014.02.19 / 민경승 / 사망 시, 방어구 내구도가 0이면 낮은확률로 장비가 소멸하는 패널티 제거
    #define _NA_007075_20140221_CHARACTER_SELECT_INPUT_BUG // 2014.02.21 / 홍성현 / 캐릭터 선택화면에서 Enter키를 누르고 빠르게 방향키를 누를 경우 선택된 캐릭터가 다른 캐릭터로 접속이되면, "맵버전이 다릅니다" 에러 메시지 이후 클라이언트가 종료되는 현상 수정
    /*($Delete_[x/C]$)*/#define _NA_007080_20140227_DEACTIVATION_APPEARANCE_UPDATE_AND_ATTACKRANGE_UPDATE_BUG // 2014.02.27 / 홍성현 / 장비가 비활성화 되었을 때(내구도 0이나 스텟이 안맞을 때)갱신이 제대로 되지 않는 버그, 펀치상태인데도 원거리 공격이 가능한 버그
    /*($Delete_[S/x]$)*/#define _NA_007079_BUG_WEAPON_INACTIVATE_STATE         // 2014.02.25 / 민경승 / 내구도0이나 스탯변화로 무기 비활성화 시, 무기를 장착해제한 상태로 만든다.
    #define _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION            // 2014.03.04 / 민경승 / 길드일일퀘스트를 '완료'했는데 보상을 받지않은 상태에서 같은그룹퀘스트를 받는 문제 수정
    #define _NA_007129_20140327_MODIFY_CAN_EQUIP_ZERODURA                  //201403.27 / 민경승 / 내구도0인 아이템 장착가능
    #define _NA_007127_20140326_OPEN_INVENTORY_FRAME_DROP_ADJUST // 2014.03.14 / 홍성현 / 인벤토리를 열었을 때 느려지는 현상 수정

    #define _NA_007029_20140114_CHAOS_ZONE_ENTER_BLIND // 2014.01.14 / 송찬종 / 카오스존 입장 활성화/비활성화 추가

    #define _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN                       // 2014.05.19 / 민경승 / DB로그 길드코인기부
    #define _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT      // 2014.05.27 / 민경승 / 아이템조합 성공 기록 시, ItemNum은 인벤토리 총 개수가 아닌, 조합성공한 개수로 한다.

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _JP_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO       // 2014.04.30 / 민경승 / 친구초대 레벨별 보상지급시, 레벨값을 const.h로 빼낸다
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.3.0.4 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1304
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(7)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_000000_20130812_NEW_CHARACTER_HELLROID  // 2013.08.12 / 김신태 / 신규클래스 - 헬로이드
    #define _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR               //2013.08.14 / 민경승 / 랭킹시스템 리뉴얼 및 전장랭킹 추가(마이너)
    #define _NA_006802_20130819_REORGANIZATION_RANK_SYSTEM_MINOR // 2013.08.19 / 홍성현 / 랭킹 시스템 개편 Minor
    #define _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL //2013.10.30 / 민경승 / 헬로이드 스킬위한 크리티컬 어빌리티 상태 추가 및 수정
    #define _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE      // 2013.12.26 / 민경승 / 몬스터가 안전지대로 들어가면 '방황'상태가 아니라 '리턴'상태로 전환하도록 한다.
    #define _NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE              // 2014.01.02 / 민경승 / thrustState(넉백상태)이후 비선공 몬스터가 반격을 하지 않는 문제 처리
    #define _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM   // 2014.01.03 / 민경승 / 아이템 생성시, 메모리할당에서 서버가 죽는 문제로, 오브젝트풀과 메모리할당을 바꿔본다.
    #define _NA_007031_20140116_CAN_SELL_CASH_ITEM_IN_STORE             //2014.01.16 / 민경승 / 상점에서 캐시아이템 판매 가능하도록 수정
    #define _NA_007031_20140116_MODIFIED_PANDORABOX_ITEM_POLICY         //2014.01.16 / 민경승 / 판도라상자에 들어갈수 있는 기간제 아이템 종류 수정
    //#define _NA_007039_20140121_TIME_UNIT_TEXTLOG                        // 2014,01,21 / 민경승 / 텍스트로그가 하루단위에서 시간단위로 파일을 생성하도록 변경

    #if defined(_KOREA)
        #define _NA_000000_20131129_ADD_KISS_API                          // 2013.11.29 / 민경승 / 썬에 KISS를 붙인다. 서버 총 동접자수를 전송한다.
    #elif defined(_CHINA)

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.3.0.3 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1303
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM // 2013.07.04 / 홍성현 / 랭킹 시스템 개편
    #define _NA_006801_20130705_RENEWAL_RANKINGSYSTEM      //2013.07.05 / 민경승 / 랭킹시스템 리뉴얼 및 전장랭킹 추가

    #define _NA_000000_20130828_GUILDMARK_SIZE_MODIFY      // 2013.08.28 / 송찬종 / 길드마크 크기 변경
    #define _NA_006826_20130722_DOMINATION_RENEWAL         // 2013.07.22 / 김진휘, 송찬종 / 피의성전 점령전 개편
    #define _NA_007239_20140521_BUG_GUILDRELATION_COUNT    // 2014.05.21 / 민경승 / 길드관계관련 최대치 수정
    #if defined(_NA_006826_20130722_DOMINATION_RENEWAL) && !defined(_NA_006953_20131114_DOMINATION_MANAGER_UI_RENEWAL)
        #define _TEMP_0068_20130820_DOMINATION_MANAGE_PACKET_CONTROL // 임시로 성문관리 임시패킷 만들어서 베팅리스트에 성문개수를 보여줌
    #endif //
    
    #define _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE // 2013.05.24 / 홍성현 / 튜토리얼 필드에서도 배틀존에 참가 할 수 있도록 개선
    /*($Delete_[x/C]$)*/#define _NA_006886_20130910_BUG_POINT_ACCUMULATE_IMAGE // 2013.09.10 / 홍성현 / 접속 포인트 보상 받기 중 엘리트 2차 재료가 보상으로 받아지는 경우 해당 아이템의 아이콘이 1초마다 각 클래스의 재료 아이템으로 바뀌어 출력되는 현상 수정 작업    
    #define _NA_006928_20131022_HELMET_UI_BUG // 2013.10.22 / 홍성현 / 캐릭터가 처음 접속했을 때 헬멧 보이기 안보이기 UI가 맞지 않는 현상 수정
    #define _NA_006930_20131023_ACCUMULATE_PAUSE_AND_CLOSE_ADD_FUNC // 2013.10.23 / 홍성현 / 접속포인트 보상 받는 도중 ESC키를 통해 정지시키고 dialog를 닫는 기능 추가
    #define _NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION    // 2013.11.12 / 민경승 / 미션내에서 비정상 종료 후 재접속하면, 미션 내 종료된 위치로 배치된다.

    #if defined(_KOREA)
		#define _NA_000000_20130903_CHANNEL_GAGE_INCREASE      // 2013.09.13 / 송찬종 / 채널 혼잡 게이지를 많아보이게 증가
    #elif defined(_CHINA)
        //#define _NA_006998_20131217_DISABLE_VOLUMLIGHT_OPTION   // 2013.12.17 / 김신태 / 게임옵 광원효과(볼륨라이트, 포스트이펙트)가 항상 OFF로 되도록 수정
        #define _CN_000000_20140224_STATE_STEEL_MOD_BUG_LOG2 // 2014.02.24 / 민경승 / 중국 강철모드 버그 관련 로그기록
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.3.0.2 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1302
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////
    
 
    #define _NA_006541_RENEWER_ACDIALOG // 2013.04.9 / 김민철 / AC창 리뉴얼
    #define _NA_006448_ITEM_TOTAL_COMPOSITE_REVIEWER // 2013.03.22 / 김민철 / 아이템 조합창 리뉴얼
    #define _NA_006643_20130417_UI_OUT_WINDOW_BREAK // 2013.04.17 / 김민철 / UI개선및 UI창 이동 개선
    #define _NA_006589_20120415_ENCHANT_PRICE_MODIFY // 2013.04.15 / 김진휘 / 인첸트 비용 공식 수정
    #define _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO // 2013.04.15 / 김진휘, 송찬종 / 타겟창 정보에 쉴드 표시 추가
    #define _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY // 2013.04.16 / 김진휘 / 커뮤니티창 상세정보에 종족이 잘못 표시되는 버그
    #define _NA_000000_20130417_SELFDISTRUCTIONABILITY_MODIFY // 2013.04.17. / 정현성 / SelfDistructionAbility 사용 제한 조건 변경    
    #define _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP //2013.04.22 / 김진휘, 송찬종 / 트리거 추가 - 레이더맵(미니맵) 변경
    #define _NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT       //2013.05.31 / 송찬종 / 채집 실패 시, 에러코드에 따라 채집을 비활성화 시킴
    #define _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND        //2013.03.27 / 홍성현,민경승 / ac과제 추가(자피르 격전지)    
    #define _NA_006599_20130401_BATTLEGROUND_PKPOINT                //2013.04.01 / 홍성현,민경승 / 전장에서 적 처치 시, 추가 득점
    #define _NA_006607_20130402_ADD_QUESTTYPE						//2013.04.02 / 홍성현,민경승 / '미션완료', '전장승리' 퀘스트타입 추가
    #define _NA_000000_20130422_SCALE_FULLMAP                       //2013.04.22 / 김민철 / 지역지도 스케일링 작업.                            
    #define _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY // 2013.04.22 / 김진휘 / 미션 트리거 추가 및 수정작업
    #define _NA_000000_20130423_GAMEOPTION_STRINGCODE_CHANGE        //2013.04.22 / 김민철 / 게임옵션리스트 스트링코드 처리 작업.
    #define _NA_000000_20130426_DUMPTOSCREENSHOT                    //2013.04.26 / 김민철 / 덤프전송시 스크린샷 첨부
    #define _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY // 2013.04.26 / 김진휘 / 발키리 소환수 스킬 싱크 버그
    #define _NA_006621_20130408_MAGICSHILD_MP_DECREASE // 2013.04.08. / 정현성 / 매직실드 MP소모량 변경
    #define _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY // 2013.04.26 / 김진휘 / 아이템 옵션 추가 변경
    #define _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST             //2013.04.15 / 민경승,홍성현 / 전장에 '어시스트' 추가
    #define _NA_006686_20130502_REMAKE_BUFF_PRIORITY                 //2013.05.02 / 민경승 / 상태 버프의 우선순위 계산방식 변경
    /*($Delete_[x/C]$)*/#define _NA_006687_20130502_SETTING_FIGHTINGENERGY_INIT_BUG // 2013.05.02 / 홍성현 / 버서커의 검기충전이 다른 캐릭터에서도 버프 아이콘으로 나타나는 현상 수정
    //#define _NA_000000_20130429_DOMINATION_PENALTY // 2013.04.29. / 정현성 / 지역점령전 패배 패널티 적용
    #define _NA_006679_20130423_1302_SKILL_RENEWAL                          // 2013.04.23 / 김신태 / 1302 스킬밸런싱
#ifdef _DEV_VER
    #define _NA_006688_20130502_1302_SKILL_RENEWAL_AIR_ANIMATION_RENEWAL    // 2013.04.23 / 김신태 / 1302 스킬밸런싱 - 띄우기 애니메이션 변경
#endif 
    #define _NA_006689_20130503_CANNOTUSE_BUFFITEM                     //2013.05.03 / 민경승 / 상위버프가 걸려있을 경우,하위버프 물약 사용 방지
    #define _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER // 미션내 채집 기능 트리거 추가
    #define _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP        //2013.05.06 / 민경승 / 어빌리티타입추가(345), 스킬공격력, 방어효과에 영향받지않는 HP증감
    #define _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION // 2013.05.06 / 김진휘 / 전장 예약중 필드 몬스터 공격 불가
    #define _NA_000000_20130508_DONOT_SERVER_SELECT_WHILE_BATTLE_GROUND_RESERVATION // 2013.05.08 / 김진휘 / 전장 예약중 서버 선택 불가
    /*($Delete_[x/C]$)*/#define _NA_006708_20130509_BUG_ROOM_STRING_LIMIT // 2013.05.09 / 홍성현 / [배틀존&카오스존] 방을 생성할 때 방제목을 한글 15자 영문/숫자 30자로제한 수정 작업
    #define _NA_006710_20130509_CRYSTALWARP_BUG // 2013.05.09 / 김진휘 / 이미 파괴된 크리스탈을 참조하지 못할 경우 강제로 다음 크리스탈을 참조하는 버그
    #define _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME // 2013.05.02 / 김민철, 김진휘 / 게임 옵션 추가(코스튬 보이기 옵션)
    #define _NA_000000_20130521_NOT_SUMMON_SKILL // 2013.05.21 / 김민철 / 서먼스킬UI코드 정리
    #define _NA_006728_20130520_SKILL_BALANCE_MODIFY // 2013.05.20 / 김진휘 / 스킬 밸런스 수정( 반사관련 스킬 수정 )
    #define _NA_000000_20130521_DAILY_QUEST_RESET_ITEM // 2013.05.21. / 정현성 / 일일퀘스트 초기화 아이템
    #define _NA_000000_20130529_STATUS_EXCEPTIONS_AS_NPC_GRADE // 2013.05.29. / 정현성 / NPC등급에 따른 상태이상 확률 적용 예외 항목
    #define _NA_006749_20130604_NOT_LIMIT_DARKBREAK // 2013.06.04 / 홍성현 / 다크 브레이크 스킬의 등급 몬스터 제한을 하지 않는 작업
    #define _NA_006731_20130521_ENCHANT_ADD_OPTION // 2013.05.21 / 김진휘, 송찬종 / 최상급(13~15)인첸트 옵션 추가
    #define _NA_000000_20130612_RANDOMIZER_CHANGE // 2013.06.12. / 정현성 / 인챈트 랜덤 함수 변경
    /*($Delete_[x/C]$)*/#define _NA_006772_20130614_BUG_ENERGY_SUCTION_CONTINUE_ANIMATION // 2013.06.14 / 홍성현 / 섀도우 클래스 커스 계열 3행의 "에너지 석션" 스킬을 사용하다 캐릭터가 사망한 상태에서 스킬 애니메이션이 지속되는 현상 수정 작업
    #define _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES // 2013.06.17 / 김민철, 홍성현 / 사용하지 않는 리소스 정리 작업
    /*($Delete_[x/C]$)*/#define _NA_006781_20130627_BUG_SHOW_OTHERPLAYERNAME_OF_GAMEOPTION // 2013.06.27 / 홍성현 / 옵션 게임 탭에서 "다른 플레이어 이름" 기능을 Off하면 내 캐릭터의 이름도 출력되지 않는 현상 수정 작업
    #define _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2 // 2013.07.10. / 정현성 / 확장정액제2 - 획득시부터 일정 기간 사용
    #define _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM // 2013.07.03 / 김진휘, 송찬종 / 전장 투표 시스템 추가
    #define _NA_006819_20130719_STATE_ICON_RENDER // 2013.07.19 / 김신태 / 상태아이콘(버프) 표시하는 방식을 변경
    #define _NA_000000_20130726_RENEWAL_POINT_REWARD // 2013.07.26 / 김민철 / 보상창 리뉴얼

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _NA_006819_20130719_STATE_ICON_RENDER // 2013.07.19 / 김신태 / 상태아이콘(버프) 표시하는 방식을 변경
        #define _NA_005011_20120621_MULTICLIENTS    // 2012.06.21 / 정현성 / 멀티클라이언트
        #define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / 김신태 / 멀티클라이언트 3개까지만 실행되게끔
    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)
        #define _RU_006860_20130806_COSTUME_PCBANG_EFFECT  //2013.08.06 / 민경승 / 코스튬에 피시방 옵션 적용시키기
        //#define _RU_007007_20131223_POSSIBLE_ENTER_CHRISTMAS_MAP // 2013.12.23 / 홍성현 / 꼬꼬마 눈사람 맵 입장가능하도록 하는 작업
    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.3.0.1 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1301
/////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_006363_20130103_NOT_CALC_ENCHANT_STAT // 2013.01.03 / 정현성 / 인챈트시 증가하는 스텟 요구치 적용하지 않기
    #define _NA_006372_20130108_BADGE_ITEM // 2013.01.09 / 정현성 / 벳지 아이템
	#define _NA_000000_20130114_RENEWER_UI // 2013.01.14 / 김민철 / 게임 UI 리뉴얼 
    #if defined(_NA_000000_20130114_RENEWER_UI)
        //#define _NA_000000_20130218_OPTION_TOOLTIP      // 2013.02.18 / 김신태 / 리뉴얼된 옵션창에서 툴팁표시하기
    #endif 
    #define _NA_000000_20130206_BROADCAST_ITEM_SCORE    // 2013.02.06 / 김신태 / 다른플레이어의 장비평점을 그냥 보내주도록 변경
    #define _NA_006413_20130201_PREMIUMSERVICE_ITEM // 2013.02.01 / 정현성 / PC방 효과 아이템
    #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / 김진휘 / 발키리의 소환수를 소환 하여 몬스터를 공격, 소환수가 캐릭터 근처로 이동 할 때 까지 캐릭터를 뒤로 이동시킬 경우 몬스터가 바보 되는 현상
    #define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD // 2013.03.13 / 김진휘 / 피의성전에서 워프크리스탈을 소환 후 캐스팅이 다 되기전 소환한 유저가 죽어도 크리스탈이 소환되는 버그
    #define _JP_006544_DRAGON_NIGHT_DRAGONTANS_BUG // 2013.03.19 / 김진휘 / 드래곤나이트로 무적상태에서 용변신 안되는 버그 수정
    #define _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY // 2013.03.25 / 김진휘 / 전장 추가작업 #2 <전장 전투 상황 알림>
    /*($Delete_[x/C]$)*/#define _NA_006539_20130319_ITEM_COMPOSIT_CERTAIN // 2013.03.19 / 홍성현 /  제작 성공률표시를 86%~100를 매우 높음에서 100% 일 때 '확실'로 뜨도록 분리 수정
    #define _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG // 2013.03.28 / 김진휘 / 소환사(발키리)의 상태 중 스킬쿨타임 증감 상태가 소환수에게 적용이 안되는 버그
    #define _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY // 2013.04.01 / 김진휘 / 미션 및 전장에서 소환수의 전투결과 데이터를 전투결과창에 반영 (적에게 가한 데미지, 소환수의 킬수, 힐량)
    #define _NA_006606_VALKYRIES_SUMMONS_HEAL_SKILL_DONOT_APPLY_ADD_SKILL_RATIO // 2013.04.01 / 김진휘 / 발키리 소환수의 힐 스킬이 소환사(주인)의 스킬공격력 비율에 맞게 추가 힐량이 계산되지 않은버그
    #define _NA_006623_20130409_STEEL_MODE_WHEN_ENERMY_CAN_USE_SKILL // 2013.04.09 / 김진휘 / 내가 강철 모드일 경우 적이 나에게 스킬이 시전되는 현상 수정.
    #define _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME // 2013.04.02 / 김진휘, 송찬종 / GM명령어로 이벤트 전장 시작
    /*($Delete_[x/C]$)*/#define _NA_006639_20130415_DEATH_AND_NOT_RELIEVE_RIDER // 2013.04.15 / 홍성현 / 탈것을 탄 상태에서 사망 시 탈것 소환과 날기가 해제되지 않는 현상 버그 수정
    /*($Delete_[x/C]$)*/#define _NA_006640_20130415_VIEW_INCREASE_HEAL // 2013.04.15 / 홍성현 / 발키리 소환수 "생명의 정령"의 힐 스킬에 소환사의 스킬 공격력 추가 힐량이 적용되지 않는 현상 수정
    #define _NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL           //2013.04.08 / 민경승 / 이동+관통형 스킬이 1명의 적에게만 피해를 줌
    #define _NA_006624_20130409_CAN_OBTAIN_SOLOBADGE_COUPLEBADGE    //2013.04.09 / 민경승 / 솔로부대뱃지와 커플염장뱃지를 함께 가질 수 있도록 함
    /*($Delete_[x/C]$)*/#define _NA_006647_20130419_TOOLTIP_VIEW_GENDER_COLOR // 2013.04.19 / 홍성현 / 여성 발키리 캐릭터가 "여성 엘리멘탈리스트용 호피무늬 의상"에 마우스 오버시 사용 가능 클래스 출력 부분이 흰색으로 출력되는 현상
	#define _NA_006644_20130419_BUG_QUEST_KILLMONSTER               //2013.04.19 / 민경승 / 몬스터킬 퀘스트 중복수행 가능
    #define _NA_006656_20120422_PC_BANG_BADGE_BUG_ON_DRAGONTRANS // 2013.04.22 / 김진휘 / 용변신 상태에서 크리스탈팰리스 아이템 활성화시 착용하고 있는 장비가 적용되지 않은 버그
    #define _NA_006655_20130422_BUG_ACCESSORY_SETOPTION           //2013.04.22 / 민경승 / 목걸이+반지 세트아이템옵션 중첩문제

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _JP_006736_20130524_CAN_TUTORIAL_FIELD_JOIN_BATTLEZONE_AND_CHAOSZONE // 2013.05.24 / 홍성현 / 튜토리얼 필드에서도 배틀존에 참가 할 수 있도록 개선
        //#define _JP_006740_20130527_LIMIT_EVENT_TUTORIAL_FIELD_WAYPOINT_VIEW_MIN_MAX_LEVEL // 2013.05.27 / 홍성현 / 이벤트 기간 중 엘소드 마을로 가는 웨이 포인트 레벨 표시를 최소~최대 레벨 표시로 개선

    #elif defined(_GLOBAL)
     
    #elif defined(_RUSSIA)

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.2.0.4 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1204
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_000000_20120920_STARTMOTION_CODE // 2012.09.20 / 김신태 / 버서커 드나 CE08, 나머지 CE00 으로 사용하고 있던 캐릭터 시작모션을 CE00으로 통일
	#define _NA_000000_20120911_FULL_MISSION_HELPER // 2012.10.08 / 김민철 / 미션 정보 기능 1204로 디파인 올림.
    #define _NA_006221_20121031_SPRITSLUG_SKILL_MODIFY // 2012.10.31 / 송찬종 / 드래곤나이트 스피릿슬러그 스킬의 발동조건을 섀도우 다크브레이크와 동일하게 변경
    #define _NA_006222_20121031_EVENT_WINDOW_DISTRIBUTE_ENABLE_TEXT // 2012.10.31 / 송찬종 / uiEventWindow 에 이벤트 중일 때와 아닐 때 대사를 나눔
    #define _NA_000000_20121113_DRAW_FTEXT_CLIPPING_TEXT // 2012.11.13 / 송찬종 / FTEXT 출력 시 밑의 잘리는 영역 그리기
    #define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY // 2012.09.25 / 김진휘 / 에테르탄환 정책 변경(기본공격력만 -> 전체데미지)
    #define _NA_000000_20121015_DISABLE_REMOVE_HIDE_STATE // 2012.10.15 / 정현성 / 피격시 하이드상태 해제 제거
    #define _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD // 2012.11.16 / 김진휘 / 큐브시스템 분해시 보상리스트에서 캐시쟈드 제거
    #define _NA_000000_20121120_NONAME_IN_SSQ // 2012.11.20 / 정현성 / 타락한 사원에서 캐릭명 알려주지 않기
    #define _NA_000000_20121210_REFACTORY_GAME_USER // 2012.12.10 / 김민철 / 신규유저 안착을 위한 개선 사항 작업.
    #define _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME // 2012.12.11 / 김진휘 / 개인 웨이포인트 삭제 대기 시간 수정
    #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / 김진휘 / 소환수의 주인이 강철모드일 경우 공격,피격,스킬사용 가능 -> 불가능으로 수정
    #define _JP_20110222_INGAME_CHARNAME_CHANGE
 
    #if defined(_KOREA)
        #define _JP_20110222_INGAME_CHARNAME_CHANGE

    #elif defined(_CHINA)
        #define _CN_CHANGE_ICON     // 2013.05.30 / 김신태 / 중국 퍼블리셔 변경에 맞춰 아이콘 변경
        #define _CN_000000_20130605_MATRIX_CARD_REMOVE // 2013.06.05 / 김진휘 / 중국 메트릭스 카드 제거
        #define _CN_000000_20130605_XIGNCODE_UPDATE // 2013.06.05 / 김진휘 / 중국 싸인코드 업데이트
        #define _CH_006797_20130703_PK_DROP_RULE_MODIFY // 2013.07.03 / 김진휘 / 중국 필드 PK 정책 변경
        #define _CN_000000_20130729_NOT_USE_CUBE // 2013.07.29 / 김민철 / 큐브 사용 제한.
        #define _CN_000000_20130729_COMPOSE_OR_DECOMPOSE_HISTORY_BLOCK // // 2013.07.29 / 김진휘 / 중국 큐브 합성 or 분해 히스토리 제거
        //#define _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG // 2013.07.31 / 김진휘 / 중국 강철모드 버그 관련 로그기록
        #define _CN_006894_20130913_PK_ITEMDROP_ABOUTHOSTILITYRELATION      //2013.09.13 / 민경승 / 중국 PK에서 사망시, 적대관계에서도 아이템이 드랍된다.
        #define _NA_000000_20131031_GAMELOG_REDUCE // 2013.10.31 / 정현성 / 게임로그 줄이기
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)
        #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / 김진휘 / 소환수의 주인이 강철모드일 경우 공격,피격,스킬사용 가능 -> 불가능으로 수정
        #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / 김진휘 / 발키리의 소환수를 소환 하여 몬스터를 공격, 소환수가 캐릭터 근처로 이동 할 때 까지 캐릭터를 뒤로 이동시킬 경우 몬스터가 바보 되는 현상
    #endif
#endif 

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.2.0.3 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1203
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(2)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _NA_004157_20120409_QUEST_BANDWITH_4000 // 2012.04.00 / 정현성 / 퀘스트 대역폭 확장
    #define _NA_005159_20120807_DISABLE_REMOVE_RUSH_STATE // 2012.08.07 / 정현성 / 피격시 러시상태 해제 불가

    #define _NA_004965_20120613_NEW_CHARACTER_MYSTIC // 2012.06.13 / 김현웅, 이태준 / 신규 캐릭터 미스틱 추가
    #ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        #define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT // 2012.06.12 / 이태준 / 신규 캐릭터 클라이언트 수정(스크립트파서관련)
        #define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP // 2012.06.12 / 이태준 / 신규 클래스 타입 추가에 따른 타입MAX 증가로 인한 발생되는 오류 임시 수정
    #endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC

    #define _NA_000000_20120527_CHAOS_ZONE // 2012.05.27 / 송찬종,김진휘 / 카오스존 시스템.
    #ifdef _NA_000000_20120527_CHAOS_ZONE
        #define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND // 2012.06.27 / 송찬종, 김진휘 / 카오스 존 전장 시스템.
        #define _NA_006088_20120910_CHAOS_ZONE_FREE_PVP // 2012.09.10 / 송찬종, 김진휘 / 카오스 존 자유전투 시스템.
        #define _NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION // 2012.09.14 / 송찬종, 김진휘 / 카오스 존 전장 시스템 활성화
    #endif //_NA_000000_20120527_CHAOS_ZONE

    /*($Delete_[S/x]$)*/ //#define _NA_006052_20120904_CANNOT_DROP_CAN_PKDROP // 2012.09.04 / 김진휘 / 아이템 바닥버리기 불가능, PK드롭가능 타입 추가.
    #define _NA_006057_20120904_PK_DROP_RATIO_MODIFY // 2012.09.04 / 김진휘 / 자신의 레벨 보다 높은 플레이어를 죽였을 경우 죽은 플레이어의 아이템드랍률 수정 상한선 100%->50%

	#define _NA_COSTUMEITEM_IDLE_ANIMATION // 2012.09.11 / 김신태 / 코스튬아이템 장착시 아이템별 대기동작 적용
    #define _NA_005988_20120901_VENDOR_SEARCH_SYSTEM // 2012.09.01 / 정현성,이도형 / 개인상점 검색 시스템    
    #define _NA_006135_20120926_MISSION_RESERVATION_PRIORITY_MODIFY // 2012.09.26 / 김진휘 / 배틀존 입장예약 우선순위 변경. (최초 -> 최신순)
    #define _NA_006164_20121012_FREE_PVP_ROOM_PARTY_MEMBER_INFO_CURRENT_ZONE_MODIFY // 2012.10.12 / 김진휘 / PVP룸에서 파티정보창의 현재위치가 잘못 표시 되는 버그 수정
    #define _NA_006174_20121015_MOVE_ZONE_POLICY_AIRSHIP_FIELD_ADD // 2012.10.15 / 김진휘 / 배틀서버에서 필드서버로 이동시 비공정 필드도 가능하도록 수정
    #define _NA_006202_20121017_DASH_AND_PIERCE_SKILL_SYNC_BUG_MODIFY //2012.10.17 / 김진휘 / 돌진 및 관통계열 스킬 싱크및 판정 수정
    #define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION          // 2012.11.01 / 김진휘 / 에너지석션을 시전하는 도중 공격불가상태(스턴, 다운..)가 되어도 스킬이 계속 시전되는 버그

    #if defined(_KOREA)

    #elif defined(_CHINA)
        #define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY // 2012.09.25 / 김진휘 / 에테르탄환 정책 변경(기본공격력만 -> 전체데미지)
        #define _NA_000000_20121120_NONAME_IN_SSQ // 2012.11.20 / 정현성 / 타락한 사원에서 캐릭명 알려주지 않기

    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
		#define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / 김신태 / 멀티클라이언트 3개까지만 실행되게끔

    #elif defined(_RUSSIA)
        #define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY // 2012.12.28 / 김진휘 / 소환수의 주인이 강철모드일 경우 공격,피격,스킬사용 가능 -> 불가능으로 수정
        #define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG // 2013.02.25 / 김진휘 / 발키리의 소환수를 소환 하여 몬스터를 공격, 소환수가 캐릭터 근처로 이동 할 때 까지 캐릭터를 뒤로 이동시킬 경우 몬스터가 바보 되는 현상
        #define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD // 2013.03.13 / 김진휘 / 피의성전에서 워프크리스탈을 소환 후 캐스팅이 다 되기전 소환한 유저가 죽어도 크리스탈이 소환되는 버그
    #endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.2.0.2 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1202
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(1)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////
    #define _NA_003923_20120130_GUILD_RENEWAL // 2012.02.15 / 정현성,이태준 / 길드 리뉴얼
    #ifdef _NA_003923_20120130_GUILD_RENEWAL
        #define _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM // 2012.02.15 / 정현성,이태준 / 길드창고 및 개인창고 슬롯개수 확장
        #define _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER // 2012.02.17 / 이태준 / uiGuildMan 재작성
        #define _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL // 2012.02.22 / 이태준 / 길드관련 UI 리뉴얼 
        #define _NA_004509_20120314_QUESTDROPINDEX10 // 2012.03.12 / 정현성 / 퀘스트 드롭 필드 확장.
        #define _NA004034_20120102_GUILD_POINT_SYSTEM // 2012.02.07 / 정현성,이태준 / 길드 코인 작업
        //#define _NA_004034_20120102_GUILD_INTRODUCTION //2012.05.10 / 정현성,이도형 / 길드 소개 작업
    #endif //_NA_003923_20120130_GUILD_RENEWAL

    //#define _NA_20120220_JUMP_RENEWAL // 2012.02.20 / 김신태 / 점프 리뉴얼
    #define _DH_NPC_GROUP_NAME  // 2012.3.26 / 이도형 / NPC 및 몬스터 Group 네임 표시(머리위에 추가 이름)
    #define _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE // 2012.4.02 / 이도형 / 정보열람실 이미지 개선작업
    //#define _NA_004157_20120409_QUEST_BANDWITH_INCREASING // 2012.04.00 / 정현성 / 퀘스트 대역폭 확장
	#define _NA_004694_20120412_1202_PARTY_ROOM_KEY                    // 2012.04.18 / 김진휘 / 배틀존 방번호를 커뮤니티 화면에 표시.
    #define _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE    // 2012.04.18 / 김진휘 / 배틀존 방 예약 시 비밀방은 입장여부 메세지 보내지 않음.
    #define _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST      // 2012.04.18 / 김진휘 / 접속 종료시 예약된 배틀존 방 리시트를 삭제한다.
    #define _NA_004530_20120315_1202_SKILL_RENEWAL // 2012.03.15 / 김현웅, 김신태 / 1202 스킬 리뉴얼
    #define _NA_0_20120321_SKILLTREE_SECOND_RENEWAL // 2012.03.15 / 김현웅, 김신태 / 1202 스킬 리뉴얼 (스킬트리, 퍽, 액션)
    #define _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT    // 2012.04.26 / 스킬 아이콘 콤보 연출 추가
    #define _NA_004605_20120417_ACTIVE_ETHER_BULLET_TO_SKILL // 2012.04.17 / 김현웅, 김신태 / 스킬 공격에 에테르 탄환 적용
    #define _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING // 2012.04.26 / 유재원, 이도형 / 날개 보임 여부 옵션 작업
    #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / 김진휘 / 발키리 소환수가 타겟을 수동 공격하는 동안, 이미 활성화된 커맨드 미 복종.

    #define _NA_004756_120504_SHOW_SSQ_PLAYER_NAME              // 2012.05.04 / 송찬종 / 타락한 사원에서 플레이어 이름 보이기 GM명령어 추가
    #define _DH_LAST_SELECTED_CHARACTER /// [2012-5-11 by i4u4me] 캐릭터 선택화면에서 최근 플레이한 캐릭터 선택
    #define _NA_004644_20110506_QUEST_NAVIGATION // 2012.05.16 / 이도형 / 퀘스트 네비게이션 색상 정보 강화

    #define _NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL   // 2012.06.18 / 송찬종 / 이동 시 스킬 큐잉 해제 (활성화된 마법진 제외)
    #define _NA_004859_20120518_FORCE_INPUT_SCREENSHOT_KEY    // 2012.05.18 / 김신태 / 채팅중 스크린샷키 입력가능하도록 수정
    #define _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE    // 2012.05.11 / 김진휘 / 타탁한 사원 중앙 돔 구역 페널티 지역 범위 재 설정 및 결계 지역 내 PVP가능.
    #define _NA_004910_20120601_SUMMON_PVP_RULE                     // 2012.06.01 / 김진휘 / 발키리 소환수 SD(99):HP(1) 룰 적용.
    /*($Delete_[x/C]$)*/#define _NA_004899_20120530_CHANGE_DOMINATION_REWARD_OPTION // 2012.05.30 / 김현웅, 이태준 / 점령전 보상 옵션 적용 방식 변경
    #define _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST  // 2012.05.11 / 민정영, 김현웅, 이태준 / 지역 점령전 추가 요청 사항(입찰길드레벨제한 및 공격길드선정)
    #define _NA_004035_20120227_COSTUME_ITEM                        // 2012.05.23 / 김진휘, 김신태 / 코스튬 아이템
    #define _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET   //2012.06.14 / 김진휘 / 퍽효과<대상의 방어력 감소율적용>
    #define _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY            // 2012.06.21 / 김진휘, 송찬종 / 방어 성공률 상한선 수정 99% -> 80%
    #define _NA_005005_20120622_SUMMON_PVP_RULE_SYNC_OWNER_PVP_RULE       // 2012.06.22 / 김진휘 / 발키리 소환수 PVP룰을 주인과 같은 룰 적용.
    #define _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR            // 2011.10.27 / 김신태 / 레이더에 채집가능한 오브젝트의 아이콘표시
	#define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / 김진휘 / 길드코인 하루 기부 최대 갯수 스크립트 데이터로 적용

    #if defined(_KOREA)
        #define _NA_005011_20120621_MULTICLIENTS    // 2012.06.21 / 정현성 / 멀티클라이언트
        #define _NA_005039_20120703_ENABLE_MULTI_CLIENT_COUNT // 2012.07.03 / 김신태 / 멀티클라이언트 3개까지만 실행되게끔
    #elif defined(_CHINA)
        #define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / 김진휘 / 길드코인 하루 기부 최대 갯수 스크립트 데이터로 적용
        #define _CN_006014_20120822_GUILD_LOG_DELETE        // 2012.08.22 / 김진휘 / 중국 길드로그에 왕의증표 입찰갯수 로그 삭제.
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
		#ifndef _NA_COSTUMEITEM_IDLE_ANIMATION	// 글로벌만 1202, 나머지는 1203부터
			#define _NA_COSTUMEITEM_IDLE_ANIMATION // 2012.09.11 / 김신태 / 코스튬아이템 장착시 아이템별 대기동작 적용
		#endif

    #elif defined(_RUSSIA)
        #define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION          // 2012.11.01 / 김진휘 / 에너지석션을 시전하는 도중 공격불가상태(스턴, 다운..)가 되어도 스킬이 계속 시전되는 버그
        #define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA  // 2012.08.23 / 김진휘 / 길드코인 하루 기부 최대 갯수 스크립트 데이터로 적용
    #endif

#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.2.0.1 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1201
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(5)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(0)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////



    #define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / 남기봉 / 파티원 인원수에 따른 하임 가중치 변경.
    #define _NA004034_20120102_POINT_WALLET_SYSTEM // 2012.01.02 / 정현성,이태준 / 화폐 지갑 작업
    #define _NA_0_20120104_DRAGON_COMMON_SKILL                    // 2012.01.05 / 남기봉,이태준 / 드래곤 나이트와 용변신 1,2 차의 스킬 공유 기능 추가.
    #define _NA_003649_20111115_BATTLE_ZONE_HARDMODE    // 2011.11.15 / 정현성 / 이도형 / 배틀존.하드모드 추가
    #define _NA_003740_20111122_RANDOM_ITEM // 2011.11.22 / 김현웅, 김신태 / 랜덤 아이템 생성
    #define _NA_003966_20111227_ADD_ENCHANT // 2011.12.27 / 김현웅, 김신태 / 최상급 인첸트
    #define _NA_004010_20120102_1201_SD_FORMULA // 2012.01.02 / 김현웅 / SD 관련 공식 변경
    #define _NA_004087_20120106_MODIFY_VALKYRIE_ATTACK_FORMULA (1) // 2012.01.06 / 김현웅 / 발키리 공격력 공식 변경
    #define _SCJ_111202_UI_RENEWAL // 2011.12.02 / 송찬종 / UI 리뉴얼 작업 (퀘스트,ac,캐릭터창)
    #define _SCJ_120104_SORT_QUEST_VIEW_PROGRESS // 2012.01.04 / 송찬종 / 퀘스트 미리보기 정렬하기
    #define _NA_20111118_CASHSHOP_UI_RENEWAL // 2011.11.18 / 송찬종 / 캐시샵 UI 리뉴얼 (엔진작업x)
    #define _DH_KEY_BINDING // 2011.11.29. 이도형, 이태준 / 유저 단축키 세팅 지원
    #define _NA_20111213_SKILLTREE_VISUAL_RENEWAL2 //2011.12.13 / 김신태 / 스킬봉인해제 연출 리뉴얼
	#define _NA_20120106_DO_NOT_REWARD_DOMINATION_TOKEN // 2010.01.06 / 민정영 / 왕의 증표 반환되지 않도록 수정
    #define _DH_CHANGE_CHAT_HISTORY // 2012.01.09 이도형 / 채팅 히스토리 활성화 키 변경
    #define _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY // 2012.01.11 / 김현웅 / 게임서버 ObjectFactory 개선
    //#define _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO // 2011.06.10 / 민정영 / 지역 점령전 체널 정보 ini파일 변경(ServerOptionEX.ini -> GameOption.ini)
    #define _DH_MONEY_COLOR_CHANGE // 2012.02.08 / 이도형 / 하임 단위별 색상 수정
    #define _NA_004126_20120111_EXTEND_OBJECT_KEY // 2012.01.11 / 김현웅 / ObjectKey 대역폭 확장
    #define _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN        // 2012.01.31 / 정현성 / 송찬종 / 배틀존.파티원전원습득
    #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT // 2011.08.03 / 남기봉, 로또 아이템 사용가능한 플레이어 레벨 제한.
    //#define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / 김현웅 / 게임 옵션 트랜잭션 처리 기능 추가
    #define _DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR // 2012.03.14 / 이도형 / 개인상점 개별가격 및 색상 적용
    #define _DH_SHOW_VENDOR_LIST_POPUP // 2012.03.22 / 이도형 / 개인상점 리스트에서 우클릭으로 캐릭터 팝업메뉴 기능 적용

    #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT   // 2011.10.21 / 남기봉,김신태 / 마우스 이동시 이동 경로 갯수 변경 (기존 64 -> 512)
    #define _DH_DEBUG_REPORT   // 2012.02.14 / 이도형 / 크래쉬 리포트 수집
    #define _NA_0_20120321_CASHSHOP_POPUP_EFFECT            // 2012.03.21 / 김신태 / 캐시샵 구매확인창 수량선택부분 하이라이트 표시

    #define _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE    /// [2012-4-24]  / 송찬종 , 유재원 / 채집 패킷에 퀘스트 코드 추가
    #define _NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD    // [2012-4-24]  유재원 / 죽었을때 능력치 올라가지 않게 함

    #define _NA_000000_20120410_SPEEDHACK /// [2012-6-14 by i4u4me] 중국 메모리핵  
    #define _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER /// [2013-10-22] 김신태, 정현성 / 히어로의 이동속도, 공격속도를 클라에서 계산하지 않고 서버에서 보내주는 값을 이용하도록 수정


    #if defined(_KOREA)
        #define _KR_0_20120207_CRYPT_SHA256                             // 2012.02.07 / 남기봉 / 암호화 모듈 MD5 -> SHA256 변경.
        #define _NA_20120313_CASHSHOP_DISABLE_ALLITEM   // 2012.03.13 / 김신태 / 캐시샵 '모든아이템' 카테고리 표시하지 않도록 변경
        #define _NA_004651_20120404_CHANGE_PARTY_HEIM_WEIGHT_v02 // 2012.04.03 / 정현성 / 저주받은 탑 하임 드랍량 변경


    #elif defined(_CHINA)
        //#define _NA_000000_20120410_SPEEDHACK /// [2012-6-14 by i4u4me] 중국 메모리핵  
        #define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / 김현웅 / 게임 옵션 트랜잭션 처리 기능 추가

    #elif defined(_JAPAN)
        #undef _DH_CHANGE_CHAT_HISTORY // 채팅 히스토리 활성화 키 변경 - 일본에서는 적용 안함
		#define _NA_004651_20120404_CHANGE_PARTY_HEIM_WEIGHT_v02 // 2012.04.03 / 정현성 / 저주받은 탑 하임 드랍량 변경

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif

#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.1.0.3 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1103
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(9)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    #define _SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET // 2011.11.10 / 송찬종 / 내용변경 있을 때에만 자동사냥 옵션 저장
    #define _SCJ_ADD_FORMAT_TEXT_PARAM_QUEST_START_NPC // 2011.11.04 / 송찬종 / FTEXT 타입 추가 - 퀘스트시작/완료 (NPC이름 색상/툴팁 추가)
    #define _SCJ_ACSYSTEM_OVERLAP_PARENT // 2011.10.05 / 송찬종 / AC과제 목록을 선행과제로 뭉치기
    #define _NA_0_20111013_RADAR_PATHFINDING // 2011.10.13 // 김신태 / NPC 길찾기 기능추가
	#define _NA_003033_20110831_DICECAST_ITEM_ROUTING	// 2011.08.31 / 정현성, 송찬종 / 주사위 획득 방식 추가
    #define _YMS_TREECONTROL   //! 2011.6.13 / i4u4me /  트리콘트롤 / WZENGINEVER >= 400
    #define _NA_003027_20111013_HONOR_SYSTEM								// 명예시스템 (정현성, 이태준)
    #define _NA_002935_20110704_ITEM_SCORE // 2011.07.04 / 김현웅, 송찬종 / 장비 평가 점수 표시
    #define /*($Delete_[x/C]$)*/_NA_20110630_BATTLE_ZONE_RENEWAL// 2011.06.30 / 민정영, 이도형 / 배틀존 리뉴얼

    #ifdef /*($Delete_[x/C]$)*/_NA_20110630_BATTLE_ZONE_RENEWAL
        #define _DH_BATTLEZONE2_// 2011.06.30 / 이도형 / 배틀존 리뉴얼(클라용)
        #define _DH_BATTLEZONE2_LIST_PAGE //2011.06.30 / 이도형 / 배틀존 리뉴얼: 배틀존 리스트 페이지 방식으로 변경
        #define _DH_BATTLEZONE2_LOBBY_SLOT//2011.06.30 / 이도형 / 배틀존 리뉴얼: 로비슬롯 순서 변경(방장최초 아니게)
        #define _NA_LEAVE_PARTY_AT_USE_ITEM_TO_MISSION//2011.11.24 / 민정영 / 미션 아이템 사용시 파티 탈퇴 기능
    #endif//

    /*($Delete_[x/C]$)*/#define _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT// 2010.01.16 / 민정영, 이도형 / 점령전 방벽 데이터 추가
    #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM// 2011.06.13/ 이도형, 김현웅 / 정보열람실
    #define _NA002635_GZ_DZ_PROTOCOL_UINIFICATION // 이은파 / Guild 서버 제거 및 DG 프로토콜과 통합 작업
    #define _NA002823_WINGS_EQUIPMENTS_EXTENTION // 이은파, 송찬종 / 날개 아이템을 특악+방어구 개념으로 확장
    //#define _NA000000_SPEED_HACK_PREVENTION_SUPPORT // 이은파, 유명수 / SpeedHack이슈 방지 처리 = { 클라이언트 시간 자체 검사 + 빠른 이동 요청 방지 + 플레이어 이동 속성 무결성 검사 }
    #define _NA003109_STYLE_PRESENTATION_IMPROVEMENT // 유명수, 이은파 / 공격 스타일 연출 개선
    #define _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION // 이은파, 유명수 / 미션 포탈 연출 강화 (타이머 및 포탈 연결 후 자진 탈퇴 기능 지원)
    #define _NA_002884_AC_DIALOG_RENEWAL // 2011.09.08 / 이태준 / AC 창 리뉴얼 과제항목 10->20개 출력
    #define _SCJ_EXP_EVENT_ENABLE_EFFECT // 2011.09.23 / 송찬종 / 경험치 이벤트 알리미 기능
    /*($Delete_[x/C]$)*/#define _NA_003457_20111013_EXCEPT_DOMINATION_REWARD // 2011.10.13 / 정현성 / 피의성전,타락한사원 인장효과 제거
    #define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / 남기봉, 게임로그 용량에 따른 파일분리.
    #define _NA_003798_SPREAD_WING_KEY // 2011.12.13 / 이태준 / 날개 펴기(비행모드) 키 추가(Shift+Space)
    #define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS// 2011.12.20 / 민정영, 송찬종 / 배틀존 리뉴얼 비정상 접속 종료 유저 처리

    #if defined(_KOREA)
        #define _NA_0_20110810_LOG_COLLECTOR_RESOTRE //문상현 //로그 콜렉터 자동 복구 기능
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                      // 2011.06.13 / 남기봉, 유명수/ 캐시샵 리팩토링.

        #if defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
            #define _NA_0_20111012_ITEM_DATE_EXTEND // 2011.10.12 // 김신태 / 아이템 기간연장 기능추가
        #endif
        //#define _YJW_TUTORIAL_SKIP  //! 2011.8.9 / i4u4me / 튜토리얼 스킵
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / 남기봉,김신태 / 마우스 이동시 이동 경로 갯수 변경 (기존 64 -> 512)

    #elif defined(_CHINA)
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / 남기봉,김신태 / 마우스 이동시 이동 경로 갯수 변경 (기존 64 -> 512)
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                        // 2011.06.13 / 남기봉, 유명수/ 캐시샵 리팩토링.
        #define _NA_20111118_CASHSHOP_UI_RENEWAL                        // 2011.11.18 / 김신태 캐시샵 UI 리뉴얼

        #ifndef _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR      
            #define _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR      // 2011.10.27 / 김신태 / 레이더에 채집가능한 오브젝트의 아이콘표시
        #endif

        #if !defined(_NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT)
            #define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / 남기봉 / 파티원 인원수에 따른 하임 가중치 변경.
        #endif

    #elif defined(_JAPAN)
        #define _DH_DEBUG_REPORT   // 2012.02.14 / 이도형 / 크래쉬 리포트 수집
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT               // 2011.10.21 / 남기봉,김신태 / 마우스 이동시 이동 경로 갯수 변경 (기존 64 -> 512)
        #define _NA_004204_20120131_BATTLE_ZONE_PARTY_OBTAIN        // 2012.01.31 / 정현성 / 송찬종 / 배틀존.파티원전원습득
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                    // 2011.06.13 / 남기봉, 유명수/ 캐시샵 리팩토링.
        #define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS       // 2011.12.20 / 민정영, 송찬종 / 배틀존 리뉴얼 비정상 접속 종료 유저 처리
        #if defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
            #define _NA_0_20111012_ITEM_DATE_EXTEND             // 2011.10.12 // 김신태 / 아이템 기간연장 기능추가
        #endif
        //#define _YJW_TUTORIAL_SKIP      //! 2011.8.9 / i4u4me / 튜토리얼 스킵 

    #elif defined(_GLOBAL)
        #define _NA_0_20110613_GAME_SHOP_RENEWAL

    #elif defined(_RUSSIA)
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 남기봉, 로또 아이템 사용가능한 플레이어 레벨 제한.        
        #define _SCJ_MAKE_DOUBLE_QUOTATION_MARKS // 2011.08.22 / 송찬종 / >0    <꺄아앗 따옴표메시지 처리~!!
        #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / 김진휘 / 발키리 소환수가 타겟을 수동 공격하는 동안, 이미 활성화된 커맨드 미 복종.
        #define _NA_0_20110613_GAME_SHOP_RENEWAL                        // 2011.06.13 / 남기봉, 유명수/ 캐시샵 리팩토링.
        #define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / 남기봉,김신태 / 마우스 이동시 이동 경로 갯수 변경 (기존 64 -> 512)
        #define _NA_004791_20120430_1202_SUMMON_STATE               // 2012.05.2 / 김진휘 / 발키리 소환수가 타겟을 수동 공격하는 동안, 이미 활성화된 커맨드 미 복종.
    #endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.1.0.2 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1102
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(8)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

	#define _NA_001990_ACSYSTEM_ADD										// 2011.02.14 / 정현성, 송찬종 / AC 과제 종류 추가
    #define _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX                      // AC 스크립트 개수 제한은 없애고, 캐릭터가 수행할수 있는 개수만 제한
    #define _NA001955_110210_WING_COSTUME_ACCESSORY_EXTENTION // 2010.02.10 / 유명수, 이은파 / 파닥파닥 장착 아이템 관련 작업, 특악세사리 개수제한 확장관련

    /*($Delete_[S/x]$)*/#define _NA_0_20110304_SKILL_BLADEFURY_SONICBLADE_EFFECT_RENEWAL    // 2011.03.04 // 김신태 / 드래곤 스킬 2종 리뉴얼 (블레이드 퓨리, 소닉 블레이드, 신규 이펙트 리소스와 데이터가 필요함)
    /*($Delete_[S/x]$)*/#define _NA_0_20110304_SKILL_WEAPON_THROWING_PROJECTILE_RENEWAL     // 2011.04.08 // 웨폰쓰로잉 이펙트 리뉴얼
    #define _NA_0_20110308_LOGSYSTEM_RENEWAL // 2010.03.08 // 김신태 / 로그 시스템 리뉴얼
    #define _NA000000_100427_SSQ_DEFAULT_REWARD_ // 이은파, 유명수 / 타락한 사원 참가자 전원 기본 보상 처리 (번호:108)
    #define _NA001955_110210_WING_COSTUME // 2010.02.10 / 유명수, 이은파 / 파닥파닥 장착 아이템 관련 작업
    #if defined(_NA001955_110210_WING_COSTUME)
        /*($Delete_[x/C]$)*/#define _YMS_FULLSETITEM_RESOURCE_DIVISION_ //! 2011.3.4 / i4u4me / 엘리트 풀셋 파트 교환 형식에서 분리
    #endif 
    #define _NA001956_110210_PERK_SWITCHABLE_REGION // 2010.02.10 / 이은파, 송찬종 / 특권의 나무 (퍽 자유 교체 지원)
    /*($Delete_[S]$)*/#define _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM // 2010.02.10 / 이은파 / 대상 검색 알고리즘 개선
    #define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                // 2011.03.08 / 남기봉, 유명수 / 캐릭터 비접속시 스킬 쿨타임 유지 기능 추가.
    #define _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER // 2011.03.07 / 김현웅, 이태준 / NPC 제어 트리거 추가
    
    #ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        #define _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE // 2010.03.11 / 이은파 / NPC 트리거 영역 이벤트 기반 처리 부분
    #endif
    #define _NA_2463_20100328_OPTIMIZATION_RADAR_MAP_TEXTURE // 2011.04.22 / 이태준 / 미니맵 텍스쳐 리소스 최적화 작업
    /*($Delete_[S/x]$)*/#define _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN // 2011.04.06 / 민정영, 이태준 / 점령전 참여 방식 변경
    #define _NA_1937_20100207_NPC_CUSTOMIZING // 2010.03.28 / 이태준 / NPC커스터마이징 지원
    #define _NA_2088_20100317_MODIFY_QUEST_AND_AC_LIST_ORDER // 2010.03.28 / 이태준 / 퀘스트/AC 리스트 정렬 및 출력 변경
    /*($Delete_[S/x]$)*/#define _NA_002050_20110216_ADD_GENDER // 2011.02.16 / 김현웅, 이태준 / 캐릭터 성별 추가
    /*($Delete_[S/x]$)*/#ifdef _NA_002050_20110216_ADD_GENDER
        /*($Delete_[S/x]$)*///#define _USE_MAX_CHAR_LIST_PARSER // 캐릭터 최대 목록 설정 파서 사용 여부
        /*($Delete_[S/x]$)*/#define _NA_0_MOD_CHARACTER_SOUND_INFO // 2011.05.24 / 이태준 / 캐릭터사운드스크립트파서 변경(성별필드 추가)
        /*($Delete_[S/x]$)*/#define _NA_0_MOD_LOAD_CHARACTER_RESOURCE_IN_CHARACTER_SCENE // 2011.06.24 / 이태준 / 캐릭터씬에서 캐릭터생성 시 리소스로딩으로 인한 지연 현상 개선
        /*($Delete_[S/x]$)*/#define _YMS_GENDER_FULLSETITEM //! 2011.5.24 / i4u4me / 풀셋 스크립트 수정 
    /*($Delete_[S/x]$)*/#endif

    #define _SCJ_THREE_LEVELUP_PER_EFFECT   // 2011.04.01 / 송찬종 / 3레벨 마다 레벨업 이펙트 추가/변경
    #define _SCJ_ADD_TYPE_LAYER_FOR_ITEM    // 2011.04.11 / 송찬종 / 아이템 타입에 따른 레이어 추가
    #define _SCJ_TEXT_COLOR_RENEWAL         // 2011.03.07 / 송찬종 / 채팅 텍스트 글씨에 테두리 속성 주기
    #define _YMS_AUTOBOT_ITEM_KIND          // 2011.3.23 / i4u4me / 자동 사냥 아이템 종류별로 추가 

    #define _NA_20100322_AGGRO_DISPLAY                                    // 2011.03.24 어그로 수치 화면에 표시 (문상현, 이도형)
    /*($Delete_[x/S]$)*/#define _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT // 2011.04.21 / 이은파, 이도형 / 장비 벗기 개선작업
    #define _GS_GSP_ALLOW_MOVE_TARGETING_								// 북미 지역지정스킬 이동.점프시에 유지

    #define _NA_002399_20110323_ITEM_ROUTING_METHOD_CHANGE                        // 2011.03.23 / 정현성,송찬종 / 아이템 획득 방식 변경
    
    #define _YMS_WINDOW_MAXSCREEN   //! 2011.4.7 / i4u4me / 창모드 최대화 기능 추가
    #define _NA_002455_20110328_AUTH_HEART_BEAT // 2011.03.28 / 김현웅 / 인증서버 Heartbeat 기능 추가
    #define _NA002635_GP_DP_PROTOCOL_UINIFICATION // 이은파 / Portal 서버 제거 및 DG 프로토콜과 통합 작업
    #define __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT // 2011.05.09 / 이은파 / (중국) 튜토리얼 몰림 현상 완화 지원 작업

    #define _DH_QUEST_ACCEPT_MESSAGE // 2011.05.19 / 이도형 / 퀘스트 수락시 상단에 메시지 출력 하는 작업

    #define _SCJ_SSQ_DIRECT_ENTER  // 2011.05.20 / 송찬종 / 타락한사원 입장권 사용 시 바로 포탈 열기
    #define _SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED // 2011.05.23 / 송찬종 / OptionList.txt의 RatioValue 필드를 Suffix로 변경 작업 (접미사로 사용할 스트링코드를 받음)

    #define _SCJ_CHANGE_SKILL_COOLTIME_COLOR // 2011.06.07 / 송찬종 / 스킬 쿨타임 색상 변경

    #define _NA_0_20110329_GAMEOPTION_SETTING                       // 2011.03.29 / 남기봉, 유명수/ 게임 옵션의 DB 저장

    #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
        #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / 송찬종 / ToTalGuide가 추가되면서 기존에 사용하던 키 도움말 창을 사용안함 
    #endif
    #define _NA_002950_20110627_SSQ_NOTICE // 2011.06.27 / 김현웅, 송찬종 / 타락한 사원 우승 메시지 출력 방식 변경

    #define _SCJ_ADD_SNS_SHORTCUT                                     // 2011.06.22 / 송찬종 / SNS 바로가기 버튼 추가
    #define _SCJ_MODIFY_OPTIONLIST_PARSER_VALUETYPE_FILED_TWO         // 2011.06.22 / 송찬종 / OptionList.txt의 ValueType2 필드가 추가되고 값이 1인 옵션은 음수가 이로운 옵션
    #define _NA_002962_20110630_CHANGE_SUMMONED_PORTAL_MOVE // 2011.06.30 / 김현웅 / 포탈 이동시 소환물 사라지지 않도록 변경

    #define _YMS_CHANNEL_LIST_COUNT5    //! 2011.7.12 / i4u4me / 채널리스트 갯수 4 -> 5개

    #if defined(_KOREA)
        #define _NA_20100307_BETAKEY_SYSTEM                                   //베타키 시스템 (문상현, 이도형)
        /*($Delete_[C]$)*/#define REMOVE_SUMMON_FRIEND    // 2011.05.24 / 이도형 / 친구소환 버튼 제거
        #define _NA003109_STYLE_PRESENTATION_IMPROVEMENT // 유명수, 이은파 / 공격 스타일 연출 개선

    #elif defined(_CHINA)
        #define _YJW_CREATECHARACTER_MOVIE //! 2011.8.3 / i4u4me / 캐릭터 생성후 동영상 보기
        //#define _YJW_TUTORIAL_SKIP  //! 2011.8.9 / i4u4me / 튜토리얼 스킵 

        #define _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION // 이은파, 유명수 / 미션 포탈 연출 강화 (타이머 및 포탈 연결 후 자진 탈퇴 기능 지원)
        #ifndef _NA_0_20110803_DIV_GAMELOG_FILE
            #define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / 남기봉, 게임로그 용량에 따른 파일분리.
        #endif
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 남기봉, 로또 아이템 사용가능한 플레이어 레벨 제한.
        #define _NA_0_20110915_CLIENT_MAC_ADDRESS                       // 2011.09.15 / 유명수, 남기봉, 접속 클라이언트의 MAC 주소 로그 남기기.
        #define _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT     // 2011.10.21 / 남기봉, 파티 인원수에 의한 하임 드랍율 조정.
        #define _NA_0_20111130_DISTROY_WEAPON                         // 2011.10.21 / 남기봉,김신태 / 내구도 0인 무기도 방어구와 같은 방식으로 파괴되도록 기능 수정.

    #elif defined(_JAPAN)
        #define _NA_20100307_BETAKEY_SYSTEM
        #define _NA_003481_20111026_EXP_PARTYBONUS_CHANGE // 파티 인원수에 따른 경험치 증가율 변경 - arycoat 2011.10.26
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 남기봉, 로또 아이템 사용가능한 플레이어 레벨 제한.
    #elif defined(_GLOBAL)
        #define _NA_20100307_BETAKEY_SYSTEM
        #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / 김현웅, 이도형 / 정보 열람실 통합 사용자 가이드 시스템
        #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
            #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / 송찬종 / ToTalGuide가 추가되면서 기존에 사용하던 키 도움말 창을 사용안함 
        #endif
        /*($Delete_[S/C]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND // 2009.10.21 / 문상현, 이도형 / 친구 초대 시스템
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 남기봉, 로또 아이템 사용가능한 플레이어 레벨 제한.
    #elif defined(_RUSSIA)
        #define _DH_KEY_BINDING // 2011.11.29. 이도형, 이태준 / 유저 단축키 세팅 지원
        #define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 남기봉, 로또 아이템 사용가능한 플레이어 레벨 제한.
        #define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / 김현웅, 이도형 / 정보 열람실 통합 사용자 가이드 시스템
        #if defined(_NA_002521_20110613_TOTAL_GUIDE_SYSTEM)
            #define _YMS_TREECONTROL // 2011.07.12. 유재원 / 트리컨트롤
            #define _SCJ_REMOVE_KEY_HELP_WINDOW // 2011.06.20 / 송찬종 / ToTalGuide가 추가되면서 기존에 사용하던 키 도움말 창을 사용안함 
        #endif
    #endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.1.0.1 Episode2 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1101
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(7)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[S/C]$)*/#define _GS_ADD_VOLUME_LIGHT                                        // 2010.09.28 / 김광수 / 포스트 이펙트 옵션 추가
    /*($Delete_[S/C]$)*/#define _DH_EP2_FULL_MAP                                            // 2010.12.13 / 이도형 / 신대륙 지도 + 신배틀존
    /*($Delete_[S/C]$)*/#define _DH_IMAGE_TOOLTIP                                           // 2010.11.03 / 이도형 / 이미지 툴팁
    /*($Delete_[S/C]$)*/#define _NA_000251_20100727_SOCKET_SYSTEM                           // 2010.07.27 / 김현웅, 이도형 / 소켓 시스템
    /*($Delete_[S/C]$)*/#define _NA_002253_20100811_CUBE_SYSTEM                             // 2010.08.11 / 김현웅, 이도형 / 큐브 시스템
    /*($Delete_[S/C]$)*/#define _NA002217_100728_EP2_PERK_SYSTEM_                           // 2010.10.19 / 이은파, 이태준 / 퍽 시스템
    /*($Delete_[S/C]$)*/#define _NA002217_100728_EP2_SKILL_SYSTEM_DB_                       // 2010.10.19 / 이은파 / 스킬 시스템 DB 파트
    /*($Delete_[x/C]$)*/#define _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_                  // 이은파 / 스킬 시스템 버전 관리 및 퍽 시스템 반영
    /*($Delete_[S/C]$)*/#define _NA_0_20100901_SKILLTREE_SYSTEM                             // 2010.09.01 / 남기봉, 김광수/ 스킬트리 시스템 추가
    /*($Delete_[S/C]$)*/#define _GS_SKLL_TREE_SYSTEM                                        // 2010.09.01 / 김광수 / 스킬트리시스템
    /*($Delete_[S/C]$)*/#define _NA_0_20100819_AGGRO_RENEWAL                                // 2010.08.19 / 문상현 이도형 / 어그로 시스템 리뉴얼    
    /*($Delete_[S/C]$)*/#define _NA_000816_20101025_AIRSHIP_SYSTEM                          // 2010.10.25 / 김광수, 김현웅 / 비공정 시스템
    /*($Delete_[S/C]$)*/#define C_NA_0_20100520_ACSYSTEM_CHANGED                            // 2010.05.20 / 문상현, 이태준 / AC 시스템을 업적단위로 변경한다.
    /*($Delete_[x/C]$)*/#define _NA_1376_20101123_RANKING_SETTLEMENT_TIME_SHIFT             // 2010.11.23 / 강재준 / PortalServer 랭킹 결산 시간 변경
    /*($Delete_[x/C]$)*/#define _NA_1388_20101207_EP2_INCREASED_MISSION_MAX                 // 2010.12.07 / 강재준 / 미션 최대 개수 상향
    /*($Delete_[S/C]$)*/#define _TEMP_DEFINE_REMOVE_CHARACTER_SCENE_MONSTER                 // 2010.12.13 / 이태준 / 캐릭터생성시 몬스터생성 임시 블럭(*임시디파인)
    /*($Delete_[S/C]$)*/#define  _LTJ_CENTIPEDE_MONSTER_BURROW_ANMATION                     // 2101.01.06 / 이태준 / 지네몬스터 버로우애니매이션 처리
    /*($Delete_[S/C]$)*/#define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                  // 2010.12.14 / 민정영, 이태준 / 점령전 관리 방식 개선에 따른 경매 아이템 체크 방식 변경
    /*($Delete_[S/C]$)*/#define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / 민정영, 이태준 / 지역 점령전 입찰 방식 변경
    /*($Delete_[x/C]$)*/#define _NA000000_GM_COMMAND_CHANGE_CONCEPT_                        // 이은파 / GM 명령어 분리 작업 및 처리 방법 교체
    /*($Delete_[S/C]$)*/#define _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL                     // 2011.01.31 / 남기봉,김신태 / 스킬트리 시스템의 스킬의 최초 습득 방식 변경.(스킬 기본 레벨 0스킬 -> 1스킬).
    /*($Delete_[S/C]$)*/#define _NA_0_20110215_SKILLTREE_SOUND_RENEWAL                      // 2011.02.15 / 김신태 / 스킬트리 연출 효과음 적용
    /*($Delete_[S/C]$)*/#define _DH_TARGET_INFO_RENEWAL                                     // 2011.02.16 / 이도형 / 몬스터 정보보기 리뉴얼EP2 작업
    /*($Delete_[S/C]$)*/#define _NA_20110303_ADD_TYPE_OF_NPCSHOP                            // 2011.03.03 / 민정영, 이도형 / 타입별 NPC 상점 기능 추가

    /*($Delete_[S/C]$)*/#define _NA_0_20110216_STATUS_STAT_POINT_SIMULATION                 // 2011.02.16 / 남기봉,유명수 / 스텟 포인트 시뮬레이션 추가.
    /*($Delete_[S/C]$)*/#if defined(_NA_0_20110216_STATUS_STAT_POINT_SIMULATION)
        /*($Delete_[S/C]$)*/#define _YMS_STAT_SIMULATION_   //! 2011.2.16 / i4u4me / 스텟 시뮬레이션 
    /*($Delete_[S/C]$)*/#endif

    //
    /*($Delete_[S/C]$)*/#ifdef _NA_000251_20100727_SOCKET_SYSTEM
        /*($Delete_[S/C]$)*/#define _NA_0_20100727_SOCKET_SYSTEM_PARSER // 2010.07.27 / 김현웅 / 소켓 시스템(파서)
        /*($Delete_[S/C]$)*/#define _NA002237_100802_EP2_SOCKET_SYSTEM_DB_ // 이은파 / 소켓 시스템 DB 파트
        /*($Delete_[S/C]$)*/#define _NA_0_20100727_SOCKET_SYSTEM_CONVERT // 2010.07.27 / 김현웅 / 소켓 시스템(solarSlot)
        /*($Delete_[S/C]$)*/#define _NA_001699_20110104_SOCKET_OPTION_POINT // 2011.01.04 / 김현웅, 이도형 / 소켓 옵션 포인트 방식으로 변경        
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_RANKUP // 2010.10.13 / 이도형 / 소켓 시스템(랭크업 제거) 클라전용
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_OLD_SOCKET // 2010.10.16 / 이도형 / 소켓 시스템(기존 소켓 제거) 클라전용
        /*($Delete_[S/C]$)*/#define _DH_REMOVE_SOCKET_COMPOSITION_ANIMATION// 2011.02.23 / 이도형 / 소켓 애니메이션 간소화
    /*($Delete_[S/C]$)*/#endif //
    /*($Delete_[S/C]$)*/#ifdef _NA002217_100820_EP2_SKILL_SYSTEM_VERSION_
        /*($Delete_[S/C]$)*/#define SKILL_SYSTEM_VERSION    (2)
    /*($Delete_[S/C]$)*/#else
        /*($Delete_[S/C]$)*/#define SKILL_SYSTEM_VERSION    (1)
    /*($Delete_[S/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define _NA001605_EP2_ATTRIBUTES_RENEWAL_ // 이은파, 유명수 / 속성 시스템 리뉴얼
    /*($Delete_[S/C]$)*/#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        /*($Delete_[S/C]$)*/#define _YMS_EP2_ATTRIBUTES_RENEWAL_     //! 2011.1.7 / i4u4me / 캐릭터 속성 리뉴얼(ui 창)
    /*($Delete_[S/C]$)*/#endif
    /*($Delete_[S/C]$)*/#define _YMS_EP2_SKILL_PACKET_RENEWAL_ //! 2011.1.10 / i4u4me / 스킬 패킷 수정
    /*($Delete_[S/C]$)*/#define _NA001605_EP2_DAMAGETYPE_EXTENTION_ // 이은파, 유명수 / DAMAGETYPE 확장
    /*($Delete_[S/C]$)*/#define _NA001605_EP2_CHANGE_SKILL_BROADCASTER_ // 이은파, 유명수 / skill broadcaster 이벤트 방식으로 변경

    /*($Delete_[S/C]$)*/#define _NA_0_20101130_REWARD_ITEM_COUNT_INCREASE                   // 2010.11.30 / 남기봉, 송찬종 / 보상 슬롯 증가. 선택 4개 -> 5개
    /*($Delete_[S/C]$)*/#define _NA_0_20101206_PARTY_ITEM_DISTRIBUTE_CHANGE                 // 2010.12.06 / 남기봉, 이도형 / 배틀서버에서 파티 아이템 분배방식 변경(필드와 동일하게)

    /*($Delete_[S/C]$)*/#define _NA_0_20110103_EP2_BATTLEZONE_EXP_PARTY_BONUS_ADJUSTED      // 2011.01.03 / 강재준 / 배틀존(방) 경험치 파티 보너스 비율 조정 (중국 제외)
    /*($Delete_[S/C]$)*/#define _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING            // 2011.01.14 / 강재준, 이태준 / 하임, 아이템 보상을 포함할 수 있는 시스템 메모 수령/삭제 로그 추가
    /*($Delete_[S/C]$)*/#define _NA_20100802_SPEED_HACK_DETECTION                           // 2010.08.02 / 강재준, 유명수 / 개선된 In-Game 스피드핵 탐지

    /*($Delete_[S/C]$)*/#define _YMS_DIALOG_ANIMATION_                                      // 2010.12.13 / i4u4me / 다이얼로그 움직이게  DialogPosInfo.txt  와 같이 배포되어야 함
    /*($Delete_[S/C]$)*/#define _YMS_TEXTURE_ANIMATION_                                     // 2010.12.27 / i4u4me / 텍스쳐 애니메이션  TextureListInfo.txt, TextureAnimationInfo.txt 같이 배포되어야 함
    /*($Delete_[S/C]$)*/#if defined(_YMS_DIALOG_ANIMATION_) && defined(_YMS_TEXTURE_ANIMATION_)
        /*($Delete_[S/C]$)*/#define _YMS_LOGIN_RENEWAL_                                     //! 2010.12.13 / i4u4me / 로그인 화면 리뉴얼 
        /*($Delete_[S/C]$)*/#define _YMS_SCREEN_RATIO_                                      //! 2011.1.24 / i4u4me / 16:10 비율 추가  (WZENGINEVER >= 280) 이상
        /*($Delete_[S/C]$)*/#define _YMS_LOGIN_CHARACTER_CAMERA_RENEWAL_                    //! 2011.1.25 / i4u4me / 캐릭터 카메라 설정 변경 
        /*($Delete_[S/C]$)*/#define _YMS_LOADING_IMAGE_                                     //! 2011.1.27 / i4u4me / 로딩 이미지 하나로 수정 
    /*($Delete_[S/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define _NA_0_20110208_SKILLTREE_VISUAL_RENEWAL                     // 2011.02.11 / 김신태 / 스킬시스템 연출 리뉴얼, 기본레벨 0으로 수정

    /*($Delete_[S/C]$)*/#define _DH_DIVIDE_MESSAGE_TEXT                                     // 2011.12.22 / 이도형 / messagetext.txt 분리작업

    /*($Delete_[S/C]$)*/#define _YMS_SMOOTH_GAUGE_                                          // 2011.2.14 / i4u4me / hp 회복 게이지 움직임 부드럽게

    /*($Delete_[S/C]$)*/#define _YMS_DEATH_SYNC_                                            //! 2011.2.21 / i4u4me / 공격 결과 동기화
    /*($Delete_[S/C]$)*/#define _DH_FATE_REMOVE_MAGIC_ARMOR                                 // 2011.02.23 / 이도형 / 럭키아이템 인증 UI에서 마법 방어력 컨트롤 및 효과 제거

    /*($Delete_[S/C]$)*/#define _NA_002229_20110302_PARTY_DEFUALT_LOOTING_CHANGE            // 2011.03.02 / 강재준 / 파티 디폴트 아이템 분배 방식 변경 (자유 -> 차례대로)
    /*($Delete_[S/C]$)*/#define _NA_002316_20110310_WORLD_SPECIFIC_PEAKTIME_EVENT           // 2011.03.10 / 강재준 / 피크타임 이벤트 월드별 구분 적용 기능 추가

    /*($Delete_[S/C]$)*/#define _YMS_SKILL_TOOLTIP_                                         //! 2011.2.28 / i4u4me / 스킬툴팁 수정: 설명에추가 옵션 넣기
    /*($Delete_[S/C]$)*/#define _NA_002128_20110228_MODIFY_ITEM_LEVEL                       // 2011.02.28 / 김현웅, 이도형 / 아이템 레벨값 설정 변경
    /*($Delete_[S/C]$)*/#define _NA_002399_20110323_MONEY_DISTRIBUTE_METHOD_CHANGE          // 2011.03.23 / 정현성,송찬종 / 하임 분배 방식 변경

    /*($Delete_[S/C]$)*/#define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / 남기봉, 유명수/ 아이템 샵 임시 리팩토링(카테고리)/
    /*($Delete_[x/C]$)*/#define _DH_GM_ITEM_WINDOW  // 2011.03.03 / 이도형 / GM Item Window (파서쪽 디파인)
    
    //#if !defined(_SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED)
    //  #define _NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED  // 2010.03.28 / 이태준 / OptionList 스크립트에 RatioValue 필드 추가 작업(옵션값이 정수타입에도 의미에 따라 %를 붙여 출력할 수 있도록)
    //#endif

    /*($Delete_[S/C]$)*/#define _DH_SHOW_ITEM_CREATE_RATE // 2011.03.28 / 이도형 / 아이템 생성(조합) 확률 표시 기능
    /*($Delete_[S/C]$)*/#define _NA_002474_20110330_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME // 2011.03.30 / 김현웅 / 룬스톤 웨이포인트 삭제 대기 시간 수정

    /*($Delete_[S/C]$)*/#define _NA_0_20110318_ITEM_EXPIRATION_PERIOD                       // 2011.03.18 / 남기봉, / 습득 아이템 완료 기간 설정 추가.
    /*($Delete_[S/C]$)*/#define _NA002479_20110401_DROP_RATIO_SELECTOR_EXTENTION            // 2011.04.01 / 이은파 / 필드,배틀존 아이템 드랍 차별화 관련 작업
    /*($Delete_[S/C]$)*/#define _NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES    // 2011.04.01 / 이은파 / 드랍 Group(4) 모드 추가 (중첩 아이템을 중첩시키지 않고 개수로 드랍)

    /*($Delete_[S/C]$)*/#define _NA_002471_20110408_EXP_BENEFIT_UP                          // 2011.04.08 / 강재준 / 경험치 보너스 최대값 제한 상향 (최대 획득량을 원 경험치의 500%에서 1000%로, 전 국가 공통)

    /*($Delete_[S/C]$)*/#define _K2P_SHOUT_DEFAULT // 2008.07.25 / 김경표 / 외치기 디폴트로 적용 define 이름 변경

    #if defined(_KOREA)
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                 // 2010.10.11 / 남기봉, / 적립 포인트 지급시 레벨/최대갯수 를 제한하는 옵션 추가.

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
        #define _NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN // 2011.04.04 / 이은파 / Ep2<-Ep1 변환시 장비 아이템 관련 보상 처리 (일본 버전)

    #elif defined(_GLOBAL)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER // 2010.06.04 / 민정영, 이태준 / 지역 점령 관리 방식 개선
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION // 2010.10.11 / 남기봉, / 적립 포인트 지급시 레벨/최대갯수 를 제한하는 옵션 추가.
        //#define _DH_GLOBAL_VERSION_TEST // 2011.04.21 / 이도형 / 글로벌 서버리스트 테섭/일반 분리용 코드
        #define _NA_0_20100727_SOCKET_SYSTEM_CONVERT_IN_JAPAN // 2011.04.04 / 이은파 / Ep2<-Ep1 변환시 장비 아이템 관련 보상 처리 (일본 버전)
    #elif defined(_RUSSIA)
        #define _RU_20110801_SEND_GIFT_FRIEND_CHECK //문상현/ 친구에게 선물을 보낼때 내가 상대방의 친구여야 하고 상대방도 나의 친구여야 한다.
    #endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.0.0.6 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1006
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(6)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define _YMS_TRADING_DONOTOPEN_SYSTEMDIALOG //! 2010.10.19 / i4u4me / 거래중일경우에는 시스템 메뉴 열지 않는다 
    /*($Delete_[S/C]$)*/#define _NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM // 이은파, 김광수 / 러키 아이템도 분리 가능토록(에테리아만...)
    /*($Delete_[x/C]$)*/#define _DH_QUICK_SLOT_LINK // 2010.11.12 / 이도형 / 퀵슬롯에 등록된 장비 아이템 관련 로직 변경(인벤,이큅일때 update, 그외의 경우 unlink)
    /*($Delete_[x/C]$)*/#define _DH_CHANGE_COOLTIME_DRAW // 2010.11.22 / 이도형 / 쿨타임 효과 통일
    /*($Delete_[S/C]$)*/#define _NA_0_20110210_INVITE_PARTY_BUG                             // 2011.02.10 / 남기봉 / 2명이서 파티 맺는 도중에 제3자에게 파티신청을 할경우 다른 사람과 파티가 맺어지는 버그 수정.

    #if defined(_KOREA)
        #define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / 남기봉, 유명수/ 아이템 샵 임시 리팩토링(카테고리)/
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 남기봉, 송찬종 / 개인상점 구매/판매 슬롯수 증가(기존 24개->40개)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / 민정영 / 기간 활성화 아이템 거래 불능

    #elif defined(_CHINA)
        #define _HACKSHIELD_CHECK_OPTION_LMP // 핵실드 lmp 적용
		#define _SCJ_PK_POPUP_DIALOG // pk 메세지
        #define _NA_0_20110305_BUG_READ_BIG_LOGFILE                         // 2011.03.05 / 남기봉 / 2GB이상의 로그 읽기 실패 버그.(파일 핸들 읽기 함수로 변경)
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 남기봉, 송찬종 / 개인상점 구매/판매 슬롯수 증가(기존 24개->40개)
        #ifndef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
            #define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                      // 2011.03.08 / 남기봉, 유명수 / 캐릭터 비접속시 스킬 쿨타임 유지 기능 추가.
        #endif

    #elif defined(_JAPAN)
        #define _JP_0_20101123_PMS                                         // 일본 게임온의 서버머 관리 시스템인 PMS 적용
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 남기봉, 송찬종 / 개인상점 구매/판매 슬롯수 증가(기존 24개->40개)
        #define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                 // 2010.12.14 / 민정영, 이태준 / 점령전 관리 방식 개선에 따른 경매 아이템 체크 방식 변경
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                     // 2010.06.04 / 민정영, 이태준 / 지역 점령 관리 방식 개선
        #define _JP_20110222_INGAME_CHARNAME_CHANGE                        // 2010.2.22  / 문상현, 이태준 / 일본에 서버 통합을 위해 캐릭텨명 변경을 게임내에서 지원해준다 (웹으로 캐릭명 변경이 안된다.)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / 민정영 / 기간 활성화 아이템 거래 불능    

    #elif defined(_GLOBAL)
        #define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING                 // 2010.11.04 / 남기봉, 유명수/ 아이템 샵 임시 리팩토링(카테고리)/
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 남기봉, 송찬종 / 개인상점 구매/판매 슬롯수 증가(기존 24개->40개)
        #define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE            // 2010.12.08 / 민정영 / 기간 활성화 아이템 거래 불능        
    #elif defined(_RUSSIA)
        #define _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE                  // 2010.11.24 / 남기봉, 송찬종 / 개인상점 구매/판매 슬롯수 증가(기존 24개->40개)
        #define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                 // 2010.12.14 / 민정영, 이태준 / 점령전 관리 방식 개선에 따른 경매 아이템 체크 방식 변경
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                     // 2010.06.04 / 민정영, 이태준 / 지역 점령 관리 방식 개선

    #endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.0.0.5 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1005
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(5)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[S/C]$)*/#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 민정영, 이태준 / 시스템 메모 통합 작업(지역점령전)
    /*($Delete_[S/C]$)*/#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / 민정영, 이태준 / 시스템 메모 관련 게임 로그 추가
    /*($Delete_[x/C]$)*/#define _101011_LTJ_DOMINATION_GUILDMEMBER_EFFECT                   // 2010.10.13 / 이태준 / 점령전 길드원보상 이펙트
    /*($Delete_[S/C]$)*/#define _NA_0_20100722_AUTO_SYSTEM                                  // 2010.07.22 / 남기봉 / 오토 시스템
    #define _YMS_AUTOBOT_GAME                                           // 2010.07.01 / 유명수 / 오토툴은 엔진버전 211 부터 지원한다.
    /*($Delete_[S/C]$)*/#define _NA_0_20100817_HELLON_LAST_TRIGGER                          // 2010.08.17 / 김현웅, 이태준 / 헬론 메인 미션용 에니메이션 제어 트리거.
    /*($Delete_[x/C]$)*/#define _GS_ADD_CAMERA_VIEW_POS                                     // 2010.10.28 / 김광수 / 캐릭터 카메라 시점조절 옵션
    /*($Delete_[x/C]$)*/#define _DH_DISPLAY_FOR_MISSION_OBJECT                              // 2010.08.02 / 이도형 / 미션 수행 목적 리스트 표시 작업
    /*($Delete_[S/C]$)*/#define _NA_000587_20100928_DOMINATION_BUFF_ITEM                    // 2010.09.28 / 김현웅, 유명수 / 피의 성전 강화 버프 아이템
    /*($Delete_[S/C]$)*/#define _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL    // 2010.10.01 / 김현웅, 이도형 / 채널 초대 기능 강화
    /*($Delete_[S/C]$)*/#define _NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_        // 2010.10.11 / 이은파 / 중국 이슈, 임의의 etherealizable item에 대한 추가 방어 처리 (with script control)

    #if defined(_KOREA)

    #elif defined(_CHINA)

    #elif defined(_JAPAN)
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                // 2010.04.20 / 민정영, 이태준 / 지역 점령전 입찰 방식 변경
      #endif

    #elif defined(_GLOBAL)

    #elif defined(_RUSSIA)

    #endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.0.0.4 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1004
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(4)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define _YMS_ADD_RADAR_ICON_100622            // 2010.06.17 / 유명수 / 미니맵 표시 업데이트 
    /*($Delete_[S/C]$)*/#define _YMS_ESCKEY_CLOSE_DIALOG                                // 2010.06.14 / 유명수 / 일부 UI가 ESC 키로 닫히지 않는 현상
    /*($Delete_[S/C]$)*/#define _YMS_SYSTEMINPUTBOX_ONLYNUMBER                          // 2010.05.28 / 유명수 / 시스템 인풋 창에서 숫자를 못고치는 현상 수정(콘트롤에 숫자만 입력 받을수 있는 기능 추가)
    /*($Delete_[S/C]$)*/#define _YMS_PET_UI                                             // 2010.05.18 / 유명수 / 펫 UI 변신. 
    /*($Delete_[S/C]$)*/#define _YMS_GUILD_DETAL_SPEC                                   // 2010.05.11 / 유명수 / 길드원 상세 정보 출력 
    /*($Delete_[S/C]$)*/#define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                  // 2010.05.11 / 유명수 / 포탈위치에서 미션완료후 퀘스트를 완료 못하는 현상
    /*($Delete_[S/C]$)*/#define _100708_LTJ_SHOW_DATETIME_IN_TIMER                      // 2010.07.26 / 이태준 / 타이머(피로도)에 날짜도 표시
    /*($Delete_[S/C]$)*/#define _100705_LTJ_CONTROL_LOD_DISTANCE_PLAYER_COUNT           // 2010.07.26 / 이태준 / 플레이어 카운트에 따라 LOD 카메라 거리 조절
    /*($Delete_[S/C]$)*/#define _100416_LTJ_MODIFY_CHAT_FILTERING                       // 2010.07.26 / 이태준 / 채팅 금칙어 필터링 변경
    /*($Delete_[S/C]$)*/#define _100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER                // 2010.07.26 / 이태준 / 상태버프 아이콘 텍스쳐통합 작업
    /*($Delete_[S/C]$)*/#define _GS_ADD_PET_ENCHANT_EFFECT                              // 2010.07.26 / 김광수 / 펫인첸트효과
    /*($Delete_[S/C]$)*/#define _YJ_FADE_FAR_TERRAIN                                    // 2010.07.26 / 윤영주 / 원경 개선 작업
    /*($Delete_[S/C]$)*/#define _YJ_SHOW_TERRAIN_DRAWCALLS                              // 2010.07.26 / 윤영주 / 개발 메세지로 지형 드로우콜 수 출력
    /*($Delete_[S/C]$)*/#define _NA_0_20100705_DOMINATION_EVENT                         // 2010.07.05 / 이호영, 윤영주 / 점령전 이벤트 트리거(몬스터리젠).
    /*($Delete_[S/C]$)*/#define _NA_0_20100706_RANDOM_REGEN                             // 2010.07.06 / 이호영 / 랜덤 리젠 재구현.
    /*($Delete_[S/C]$)*/#define _NA_0_20100901_GUILD_NAME_CHANGE_ITEM_LOG               // 2010.07.06 / 이호영 / 길드명 변경 아이템 사용 로그 추가.

    /*($Delete_[S/C]$)*/#define _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_ // 이은파, 이도형 / 인벤토리 슬롯 기능 개선
    /*($Delete_[S/C]$)*/#define _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_ // 이은파 / 인벤토리 분리 작업
    /*($Delete_[S/C]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_ // 이은파, 김광수 / 아이템 커스터마이징
    /*($Delete_[S/C]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_COMPOSITE_RULE_ // 2010.10.13 / 이은파, 이태준 / 조합재료로 사용하는 에테리아 관련 아이템 처리
    /*($Delete_[S/C]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_ // 이은파, 유명수 / C/S 시간 동기화 처리
    /*($Delete_[S/C]$)*/#define _NA000000_100623_CHECK_WMO_CHECKSUM_ // 이은파, 윤영주 / 필드 입장시 WMO 버전 검사 루틴 추가 (engine버전 검사라는 이레귤러 발생...ㅠㅠ)
    /*($Delete_[S/C]$)*/#define _NA000000_100714_NEW_CHARITEM_EXTENDED_FIELD_ // 이은파 / 신규 캐릭에 지급하는 장비 아이템 확장 지원

    /*($Delete_[S/C]$)*/#define _NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM // 2010.05.27 / 김현웅, 김광수 / 캐릭터 외모 변경 시스템
    /*($Delete_[S/C]$)*/#define _NA_000070_20100609_ENHANCED_ITEM_DIVISION // 2010.06.09 / 김현웅, 이도형 / 아이템 분할 방식 개선
    /*($Delete_[S/C]$)*/#define _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST // 2010.06.22 / 김현웅, 이도형 / 차단 목록 개선

    /*($Delete_[S/C]$)*/#define _NA_0_20100325_QUICK_SLOT_CHANGE						// 2010.03.25 / 남기봉,이도형 / Quick 슬롯 변경 (48개의 슬롯->40개의 슬롯으로 변경)
    /*($Delete_[S/C]$)*/#define _DH_SHOW_REPAIR_HAIM                                    // 2010.07.23 / 이도형 / 수리 윈도우 하임 3자리 구분점(,) 추가
    /*($Delete_[S/C]$)*/#define _DH_SHOW_UI_NAME                                        // 2010.07.13 / 이도형 / UI 이름 표시기능
    /*($Delete_[S/C]$)*/#define _DH_NORMAL_CHAT_BLOCK                                   // 2010.06.22 / 이도형 / 차단 목록 개선
    /*($Delete_[S/C]$)*/#define _DH_ENABLE_DIVIDING_ITEM_WITH_CLICKING_SLOT_TAB         // 2010.06.04 / 이도형 / 아이템 분배시에 슬롯 변경 가능하도록 변경
    /*($Delete_[S/C]$)*/#define _DH_ENHANCED_UI_PACKAGE_CHATTING                        // 2010.06.17 / 이도형 / UI리뉴얼 및 채팅창
    /*($Delete_[x/C]$)*/#define _DH_ADD_TYPE_MARK_FOR_ITEM         // 2010.05.06 / 이도형 / 아이콘을 통한 아이템 식별 작업
    /*($Delete_[S/C]$)*/#define _DH_TRIGGER_LOG_MESSAGE                                 // 2010.06.26 / 이도형 / 개발 게임 클라이언트에서 트리거 메시지 출력 작업
    /*($Delete_[S/C]$)*/#define _DH_DOUBLECLICK_MOVE_WAYPOINT                           // 2010.06.26 / 이도형 / 웨이포인트 리스트 더블클릭으로도 이동 요청 가능하게 변경
    /*($Delete_[S/C]$)*/#define _NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION                // 2010.05.20 / 민정영 / 창고 이동 불가능 옵션 추가, 길드 창고는 가능
    /*($Delete_[S/C]$)*/#define _NA_20100621_ADD_DOMINATION_EVENT_SELECT_GUILD_END      // 2010.06.21 / 민정영 / 지역 점령전 이벤트 추가

    /*($Delete_[S/C]$)*/#define _NA000000_100510_CLIENT_PERFORMANCE_TUNEUP_             // 2010.05.10 / 이은파
    /*($Delete_[S/C]$)*/#define _NA_0_20100616_PARTY_JOIN_HOSTILITY_RELATION_BUG        // 2010.06.16 / 남기봉 / 파티 수락시 적대관계의 파티원이 존재함에도 가입되는 버그 수정.
    /*($Delete_[S/C]$)*/#define _GS_MOD_CASH_SHOP_TOOLTIP_BY_TYPE                       // 2010.09.14 / 김광수 / 캐쉬샵에서 에테리아.쉘아이템 툴팁출력되게끔수정

    /*($Delete_[S/C]$)*/#define _NA_20100913_GUILD_WAREHOUSE_GM_COMMAND                 // 2010.09.13 / 강재준 / 길드 창고 초기화 GM 명령어 추가

    /*($Delete_[S/C]$)*/#define _NA000000_DBQUERY_REVERSAL_STATE_PREVENTION             // 2010.11.02 / 이은파 / DBQuery역전 현상 막기 위한 작업

    #if defined(_KOREA)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // 이은파, 김광수 / 아이템 커스터마이징, 아이템 사용한 분리 기능 지원(국가별 선택 컨텐츠)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER                          // 2010.06.04 / 민정영, 이태준 / 지역 점령 관리 방식 개선
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / 이호영, 유명수 / 친구,길드원,리벤지 공유 메모.

    #elif defined(_CHINA)
        #define _YMS_AUTOBOT_GAME                                               // [2010-7-1 by i4u4me] 오토툴은 엔진버전 211 부터 지원한다.
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // 이은파, 김광수 / 아이템 커스터마이징, 아이템 사용한 분리 기능 지원(국가별 선택 컨텐츠)
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                          // 2009.10.21 / 문상현, 이도형 / 친구 초대 시스템
        #define _NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_            // 2010.10.11 / 이은파 / 중국 이슈, 임의의 etherealizable item에 대한 추가 방어 처리 (with script control)
        /*($Delete_[x/C]$)*/#define _NA_20100604_BETTER_DOMINATION_MANAGER
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                    // 2010.10.11 / 남기봉, / 적립 포인트 지급시 레벨/최대갯수 를 제한하는 옵션 추가.
        #define _YMS_TRADING_DONOTOPEN_SYSTEMDIALOG                             // 2010.10.19 / i4u4me / 거래중일경우에는 시스템 메뉴 열지 않는다 
        #define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                       // 2010.10.26 / 민정영 / 그룹 보너스 적용 방법 상,하위 멤버 적용 변경
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / 이호영, 유명수 / 친구,길드원,리벤지 공유 메모.

    #elif defined(_JAPAN)
		/*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_ // 이은파, 김광수 / 아이템 커스터마이징, 아이템 사용한 분리 기능 지원(국가별 선택 컨텐츠)
        #define _NA_0_20100527_ADD_CHAR_RELATION_MEMO                   // 2010.05.25 / 이호영, 유명수 / 친구,길드원,리벤지 공유 메모.

    #elif defined(_GLOBAL)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // 이은파, 김광수 / 아이템 커스터마이징, 아이템 사용한 분리 기능 지원(국가별 선택 컨텐츠)

    #elif defined(_RUSSIA)
        /*($Delete_[S/x]$)*/#define _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_  // 이은파, 김광수 / 아이템 커스터마이징, 아이템 사용한 분리 기능 지원(국가별 선택 컨텐츠)
        #define _RU_110209_RUSSIA_CONTENTS_BLOCK                                // 러시아 컨텐츠 블럭 디파인
        #define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION                    // 2010.10.11 / 남기봉, / 적립 포인트 지급시 레벨/최대갯수 를 제한하는 옵션 추가.
    #endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.0.0.2 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1002
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
    #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
    #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
    #define C2S_LOW_VERSION_NO		(3)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    // unified protocol
    /*($Delete_[S/C]$)*/#define _NA00XXXX_090111_CASH_TYPE_SHARED_                      // 이은파, 이도형 / 캐시타입(PP카드) 추가에 따른 구분 작업 (전 국가 공통)
    /*($Delete_[x/C]$)*/#define _NA_0_20100208_DOMINATION_AREA_INFO                     // 2010.02.08 / 이호영, 이태준 / 점령전 인장 주위에 보여주기.
    /*($Delete_[S/C]$)*/#define _NA_0_20100525_ADD_TIMER_ACTION_TRIGGER                 // 2010.05.25 / 이호영, 이태준 / 타이머 발동 액션 트리거 추가.
    /*($Delete_[S/C]$)*/#define _NA_0_20100514_NPC_DEAD_STATE_TIME                      // 2010.05.14 / 이호영 / 몬스터 죽은 상태 지속 시간 값을 고정값에서 스크립트에서 제어하도록 변경. 
    /*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT_DELETE             // 2010.03.08 / 이호영 / 퀘스트 반복 버그 의심 부분 수정
    /*($Delete_[S/C]$)*/#define _NA_0_20100308_ADD_QUEST_COUNT_CHECK					// 2010.03.08 / 이호영 / 퀘스트 갯수 체크 추가.    
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_DURA                      // 2010.04.12 / 이호영 / 점령전 내에서 사망시 내구도 하락 비율 변경 (10% -> 5%) 
    /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGER // 2010.02.22 / 이호영, 윤영주, 이태준 / 맵툴 개선 요청.
    /*($Delete_[S/C]$)*/#ifdef _NA_0_20100222_UNIT_TRIGGER
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_ADD // 2010.02.22 / 이호영, 윤영주, 이태준 / 맵툴 개선 요청. 
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_PATHLIST // 2010.02.22 / 이호영, 이태준 / 몬스터 AI 개선.  
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_REGEN // 2010.02.22 / 이호영, 윤영주, 이태준 / 리젠 관련 변경.
        /*($Delete_[S/C]$)*/#define _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER // 2010.02.22 / 이호영 / 아군형 몬스터 추가.
        /*($Delete_[S/C]$)*/#define __NA_276_20070213_UNIT_TRIGGER // 2010.02.22 / 김민욱,이호영 / 유닛 관련 미구현 트리거 요청 / 
    /*($Delete_[S/C]$)*/#endif
    /*($Delete_[x/C]$)*/#define _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB              // 2010.02.22 / 이호영 / PVP시 버서커 데미지 리커버리 스킬이 HP를 초과 회복 하는 현상.
    /*($Delete_[x/C]$)*/#define _100114_LTJ_USE_AMBIENTSOUND_SCRIPT_FOR_AMBIENT_SOUND   // 2010.03.25 / 이태준 / 엠비언트사운드 스크립트를 통한 재생 제어
    /*($Delete_[x/C]$)*/#define _GS_MOD_PLAYER_LOADING                // 2010.03.25 / 김광수 / Player리로스 지연로딩 및 멀티쓰레드 기능개선
    /*($Delete_[S/C]$)*/#define _NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY      // 2010.03.16 / 김현웅, 유명수 / 배틀존 로비 입장 가능한 방 보기
    /*($Delete_[S/C]$)*/#define _NA_0062_20100318_CHANGE_LEVEL_FOR_STAT_INIT_2          // 강재준, 유명수 / 무료 스텟 초기화 제한 레벨 40에서 70으로 상향
    /*($Delete_[x/C]$)*/#define _DH_COUNT_LOGIN_FRIEND                                  // 2010.03.23 / 이도형 / 친구 접속자 수 표시, 그룹 포함
    /*($Delete_[x/C]$)*/#define _DH_CHANGE_EXP_COLOR                                    // 2010.03.22 / 이도형 / 최고레벨 달성시 경험치 게이지 색상 변경
    /*($Delete_[x/C]$)*/#define _NA_0_20091118_GAMEROOM_BATTLE_RESULT                   // 2010.03.24 / 이도형, 남기봉 / 전투결과보기 시스템
    /*($Delete_[x/C]$)*/#define _DH_CUR_SKILL_NEEDSFOR_TOOLTIP                          // 2009.11.17 / 이도형 / 이미 찍은 스킬도 요구 숙련도 보기
    /*($Delete_[x/C]$)*/#define _DH_HIDE_INCANT_EFFECT                                  // 2009.11.18 / 이도형 / 카메라 줌인 상태에서 인첸트 이펙트 숨기기
    /*($Delete_[x/C]$)*/#define _DH_DBLCLICK_FRIEND_STATUS                              // 2009.11.16 / 이도형 / 친구목록 더블 클릭으로 상세정보 보기
    /*($Delete_[x/C]$)*/#define _DH_BATTLEZONE_CREATE_CLASS_TOOLTIP                     // 2009.11.17 / 이도형 / 배틀존 생성창에서 클래스 툴팁 보기
    /*($Delete_[x/C]$)*/#define _DH_GUILDINFO_OFFLINE_COLOR                             // 2009.11.11 / 이도형 / 길드창에서 오프라인 유저 회색 색상 적용
    /*($Delete_[x/C]$)*/#define _DH_PARTY_UI_CUSTOMIZING                                // 2009.11.16 / 이도형 / 파티UI 숨기기 기능
    /*($Delete_[x/C]$)*/#define _DH_VENDER_WINPOS                                       // 2009.11.10 / 이도형 / 개인상점 위치 변경 유지
    
    /*($Delete_[x/C]$)*/#define _DH_PARTY_LEADER_CHANGE_CLOSE_OPTWIN                    // 2009.11.12 / 이도형 / 파티장 변경시 옵션창 닫기
    /*($Delete_[x/C]$)*/#define _DH_PANDORA_TOOLTIP_ADD_MARGIN                          // 2009.11.25 / 이도형 / 사제의 선물 툴팁 줄간격 수정
    /*($Delete_[x/C]$)*/#define _DH_MISSIONZONE_NO_ITEMMESSAGE                          // 2009.11.20 / 이도형 / 미션존에서 아이템 분배 메시지 미출력
    /*($Delete_[x/C]$)*/#define _NA_20100106_REPURCHAS_BUGFIX_2496                      //2010.01.06 문상현 배틀존 들어갔다 온후 재구매할때 재구매 안되는 버그 수정

    /*($Delete_[x/C]$)*/#define _YMS_NUMBER_TO_MONEY                                    // 2010.03.25 / 유명수 / 하임 천단위에 콤마 입력
    /*($Delete_[x/C]$)*/#define _YMS_ESCKEY_COMPOSITE                                   // 2010.03.25 / 유명수 / 강화창 esc키로 취소 가능
    /*($Delete_[x/C]$)*/#define _YMS_UI_RBUTTONCLICKI_ENCHANT_CRYSTALLIZATION_RANKUP    // 2010.03.25 / 유명수 / [한국][0032] 강화창 아이템 등록 펴의 제공
    /*($Delete_[x/C]$)*/#define _YMS_UI_STATE_REMEMBER                                  // 2010.03.25 / 유명수 / [한국][0030]UI 클릭 정보 저장
    /*($Delete_[x/C]$)*/#define _YMS_AREACONQUEST_MAP_TOOLTIP                           // 2010.03.25 / 유명수 / [한국][0025]AC 보물지도 툴팁 출력 
    /*($Delete_[x/C]$)*/#define _YMS_GAMEOPTION_LANGUAGETYPE                            // 2010.03.25 / 유명수 / 국가별 변수 추가
    /*($Delete_[x/C]$)*/#define _YMS_SOUND_CACHE                                        // 2010.03.25 / 유명수 / 사운드 캐싱
    /*($Delete_[x/C]$)*/#define _YMS_SUMMONSKILLCONTAINER_RESET                         // 2010.05.07 / 유명수 / 발키리 소환수변경 시 소환수 스킬창이 갱신되지 않는 버그
    /*($Delete_[x/C]$)*/#define _YMS_EVENTFRIEND_SCROLLBAR                              // 2010.05.14 / 유명수 / 친구초대 메세지 스크롤 되게

    /*($Delete_[x/C]$)*/#define _NA_1419_20100121_ACCUMULATE_POINT                      // 2010.01.21 / 남기봉,이태준 / 플레이 시간에 따른 적립 포인트 추가
    /*($Delete_[x/C]$)*/#define _100409_LTJ_SHOW_ITEM_UNIT_PRICE_IN_VENDOR              // 2010.04.13 / 이태준 / 개인상점 개당가격 보여주기
    /*($Delete_[x/C]$)*/#define _100408_LTJ_SEPARATE_TIMER_INI                          // 2010.04.13 / 이태준 / 타이머 관련 설정을 PROGRAM.INI 에서 TIMER.INI 로 분리

    #if defined(_KOREA)
    
    #elif defined(_CHINA)
        //#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 민정영, 이태준 / 시스템 메모 통합 작업(지역점령전)

    #elif defined(_JAPAN)
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                  // 2009.10.21 / 문상현, 이도형 / 친구 초대 시스템
        #define _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST            // 2010.06.22 / 김현웅, 이도형 / 차단 목록 개선

    #elif defined(_GLOBAL)
		#define _100708_LTJ_SHOW_DATETIME_IN_TIMER
		/*($Delete_[S/x]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_
        #define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 민정영, 이태준 / 시스템 메모 통합 작업(지역점령전)
        /*($Delete_[S/x]$)*/#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / 민정영, 이태준 / 시스템 메모 관련 게임 로그 추가
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / 민정영, 이태준 / 지역 점령전 입찰 방식 변경
      #endif
        #define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                      // 2010.05.11 / 유명수 / 포탈위치에서 미션완료후 퀘스트를 완료 못하는 현상

    #elif defined(_RUSSIA)
        #define _100708_LTJ_SHOW_DATETIME_IN_TIMER
        /*($Delete_[S/x]$)*/#define _NA000000_100609_CS_TIME_SYNCHRONIZATION_
        #define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 민정영, 이태준 / 시스템 메모 통합 작업(지역점령전)
        #define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE               // 2010.09.01 / 민정영, 이태준 / 시스템 메모 관련 게임 로그 추가
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                    // 2010.04.20 / 민정영, 이태준 / 지역 점령전 입찰 방식 변경
      #endif
        #define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN                      // 2010.05.11 / 유명수 / 포탈위치에서 미션완료후 퀘스트를 완료 못하는 현상

    #endif
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 1.0.0.1 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 1001
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(2)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    //----------------------------------------------------------------------------------------------

    /*($Delete_[S/C]$)*/#define _NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP                // 2009.11.19 / 강재준 / NPC 상인 판매 대금의 하임 행운 적립율 개별 조절 지원
    /*($Delete_[S/C]$)*/#define _NA_0_20091030_RANDOM_ITEM                                  // 2009.10.30 / 남기봉, 이도형 / 랜덤 아이템 추가.
    /*($Delete_[x/C]$)*/#define _GS_SEPARATE_SPECIAL_TEXTURE                                // 2009.12.04 / 김광수 / 스페셜텍스쳐 스크립트로 분리하는작업
    /*($Delete_[x/C]$)*/#define _GS_MOD_NONE_DRAW_ZEROEXP									// 2009/12.07 / 김광수 / EXP 0일때는안찍기
    
	/*($Delete_[x/C]$)*/#define _091229_LTJ_AC_REWARD_HELPMESSAGE_BUGFIX					// 2009.12.29 / 이태준 / AC 10%단위 도달시 보상안내 헬프메시지 출력되도록 수정
	/*($Delete_[x/C]$)*/#define _100105_LTJ_ADD_SSQ_OBVERVER_ASSIST_DIALOG					// 2010.01.07 / 이태준 / 타락한 사원 관전모드창을 Assist창으로 구현

	/*($Delete_[x/C]$)*/#define _YJ_MOD_FOG_COLOR_IN_CREATE_CHARACTER						// 2010.01.12 / 윤영주 / 캐릭 생성화면 리뉴얼되면서 변경된 포그 컬러 수정 
	/*($Delete_[x/C]$)*/#define _YJ_MOD_POS_IN_CHARACTER_SCENE								// 2010.01.12 / 윤영주 / 캐릭 선택화면 리뉴얼되면서 변경된 캐릭 포지션 수정

    /*($Delete_[S/C]$)*/#define _NA_0_20100114_QUEST_REWARD_CHECK                           // 2010.01.14 / 이호영 / 퀘스트 보상시 NPC와의 거리 체크.
    /*($Delete_[S/C]$)*/#define _NA_001290_20090525_MODIFY_CALC_SD                          // 2010.01.06 / 이호영 / SD 계산 공식 변경 

    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION
    /*($Delete_[S/C]$)*/#ifdef _NA001385_20090924_DOMINATION
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_GAMELOG_ACTION            // 2010.01.12 / 이호영 / 지역 점령전용 게임 로그
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_FINDPATH_ATTACK           // 2009.12.15 / 이호영 / 공격시 가능 체크(findpath)
        /*($Delete_[S/C]$)*/#define _0_20100427_DOMINATION_FINDPATH_ATTACK_BUG              // 2010.04.27 / 이호영 / 수정 워프 이동시 벽 넘어로 이동되는 버그 수정.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_SHADOW                    // 2009.12.15 / 이호영 / 쉐도우 하이드 스킬 변경.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_NPC_MAXHP                 // 2009.12.09 / 이호영 / monster render info에 maxHp 추가
        /*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_SYNC                     // 2009.08.17 / 이호영 / 채집 완료 동기화.
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_BASE                      // 2009.09.24 / 이호영 / 지역점령전._Map
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_STAMP                     // 2009.09.24 / 이호영 / 지역점령전._stamp
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_ETC                       // 2009.09.24 / 이호영 / 지역점령전._etc(패킷,파서)
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_TRIGGER                   // 2009.09.24 / 이호영 / 지역점령전._trigger
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_SKILL                     // 2009.09.24 / 이호영 / 지역점령전._크리스탈 워프 스킬
        /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_EVENT_CONTROL             // 2009.09.24 / 이호영 / 지역점령전._이벤트 컨트롤
        /*($Delete_[S/C]$)*/#define _0_20100419_DOMINATION_RETURN_TO_ENTRANCE_BUG           // 2009.09.24 / 이호영 / 지역점령전.입구로 강제 이동 되지 않는 유저 강제 접속 종료 시키기.
        /*($Delete_[S/C]$)*/#define __KR_001385_20090907_DOMINATION_PACKET                  // 2009.09.07 / 민정영 / 지역 점령전
        /*($Delete_[S/C]$)*/#define _KR_001385_20090907_DOMINATION_PARSER                   // 2009.09.07 / 민정영 / 지역 점령전 파서 작업 정의
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_QUERY                    // 2009.10.13 / 민정영 / 지역 점령전 DB쿼리문 작업
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_GUILDSERVER_BASE         // 2009.10.13 / 민정영 / 길드 서버 기본 작업
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_GAMESERVER_BASE          // 2009.10.13 / 민정영 / 게임 서버 기본 작업
        /*($Delete_[S/C]$)*/#define _KR_001385_20091013_DOMINATION_BASICINFO_SETTING        // 2009.10.13 / 민정영 / 지역 점령전 기본정보 세팅 작업
        /*($Delete_[S/C]$)*/#define _KR_001385_20091103_DOMINATION_ZONEMOVE_NOTIC           // 2009.11.03 / 민정영 / 존 이동시에 점령 지역의 길드 정보 표시 작업
        /*($Delete_[S/C]$)*/#define _KR_001385_20091105_DOMINATION_UPDATEDOMINATIONINFO     // 2009.11.05 / 민정영 / 지역 점령전 정보 업데이트 관련 작업
        /*($Delete_[S/C]$)*/#define _KR_001385_20091202_DOMINATION_RETURN_HEIM              // 2009.12.02 / 민정영 / 지역 점령전 / 하임 되돌려주기 관련 작업
        /*($Delete_[S/C]$)*/#define _KR_001385_20091211_DOMINATION_GAMELOG_MONEY            // 2009.12.13 / 민정영 / 지역 점령전 게임 로그 작업
		/*($Delete_[S/C]$)*/#define __001385_LTJ_RENEWAL_NOTICE								// 2009.11.27 / 이태준 / 공지사항 리뉴얼
		/*($Delete_[S/C]$)*/#define __001385_LTJ_ADD_DOMINATIONWAR_TOWER_UI					// 2009.11.27 / 이태준 / 지역점령전 관련 UI 작업
		/*($Delete_[S/C]$)*/#define _YJ_RENEWAL_CAST_LEAVE_DLG								// 2009.11.27 / 윤영주 / CastLeave Dlg 리뉴얼
		/*($Delete_[S/C]$)*/#define _YJ_MOD_MAPOBJECT_MONSTER								// 2009.11.27 / 윤영주 / 설치형 몬스터 사용 방식 수정	
		/*($Delete_[S/C]$)*/#define _YJ_EDIT_OBJ_COLOR_APPLY								// 2009.11.27 / 윤영주 / 맵툴에서 수정한 맵오브젝트/스피드트리 컬러값 적용
		/*($Delete_[S/C]$)*/#define _YJ_ADD_RADAR_ICON										// 2009.11.27 / 윤영주 / 지역점령전 관련 레이더 아이콘 추가
		/*($Delete_[S/C]$)*/#define _YJ_RENEWAL_TEXT_ON_HEAD								// 2009.11.27 / 윤영주 / 캐릭터 머리위 캐릭터 
		/*($Delete_[S/C]$)*/#define _YJ_ADD_DAMAGE_EFFECT_NPCINFO							// 2009.12.15 / 윤영주 / NPCINFO에 데미지 이펙트 코드 추가
		/*($Delete_[S/C]$)*/#define _YJ_STAMP_GAGE											// 2010.01.08 / 윤영주 / 각인 게이지 작업(Engine Ver 120)
    #endif

    /*($Delete_[x/C]$)*/#define _NA_0_20091104_FIRENDMAP_REFACTORYING                       // 2009.11.04 / 문상현 / 월드 서버의 User 클래스에서 친구 맵 관리 부분을 FriendMap 클래스로 분리 한다.

    /*($Delete_[S/C]$)*/#define _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_				// 2009.11.23 / 이은파, 이태준 / 안전지대 타일 속성 추가 + 에 따른 몬스터 AI변경
    /*($Delete_[x/C]$)*/#define _NA_0_20100312_MODIFY_DELIMITERS_FOR_TEXTSCRIPT             // 2010.03.12 / 이태준 / (QuestText, Message, HelpLoadingMessage)스크립트에 사용되는 구분자중 " 를 사용하지 않도록 변경

    /*($Delete_[x/C]$)*/#define _YMS_RANKUP_TOOLTIP                                         // 2010-03-31 / 유명수 / 랭크업 툴팁수정(다음 등급 제한 수치 미출력)
    /*($Delete_[x/C]$)*/#define _YMS_CANNOT_EMOTION	                                        // 2010-03-31 / 유명수 / 피어 상태에서 감정표현 안되게

    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_NON_PREEMPTIVE_ATTACK_BUG     // 2010.04.09 / 이호영 / 점령전 인장 흭득시 미획득 유저에 의해 타겟팅 되는 버그 수정.
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_MAPNPC_AI_BUG                 // 2010.04.09 / 이호영 / 지역점령전 성문 AI 변경    
    /*($Delete_[S/C]$)*/#define _NA001385_20090924_DOMINATION_STAMP_STYLE_SKILL_BUG         // 2010.04.09 / 이호영 / 각인시 스타일 스킬 미적용 추가
    

    #if defined(_KOREA)
        //만일 이 플레그를 제거 하려고 한다면 DB 친구 테이블에서 친과 관계 코드가 5,6인 값들을 모두 1로 변경해야 안전하다.
        /*($Delete_[S/x]$)*/#define _KR_0_20091021_FIRENDINVITATION_EXTEND                  // 2009.10.21 / 문상현, 이도형 / 친구 초대 시스템
        #define __CN_PET_OPTION_APPLY_FLAG								// 2008.08.24 / 김창현 / 펫 옵션 플레그로 선택 적용(스크립트에서...)
        //#define _USING_GUILD_DESTROY_LOGGING                            // 2010.03.03 / 김현웅 / 길드 해산 문제 추적을 위한 로그 추가
    #elif defined(_CHINA)
        #if defined(__APPLY_HACKSHIELD) && defined(_SUNGAME_VER)
            #define _APPLY_HACKSHIELD_MONITOR                           // 2010.03.17 / 유명수 / 중국 핵쉴드 모니터링 적용
        #endif
        #define _NA000000_100427_SSQ_DEFAULT_REWARD_                    // 이은파, 유명수 / 타락한 사원 참가자 전원 기본 보상 처리
        #define _NA_6260_20100507_BUG_FIX_SPDAMAGED_ABSORB              // 2010.02.22 / 이호영 / PVP시 버서커 데미지 리커버리 스킬이 HP를 초과 회복 하는 현상.
      #ifndef _CH_20100420_CHANGE_DOMINATION_APPLY_WAY
        #define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                // 2010.04.20 / 민정영, 이태준 / 지역 점령전 입찰 방식 변경
      #endif
        #ifndef _YMS_SUMMONSKILLCONTAINER_RESET
            #define _YMS_SUMMONSKILLCONTAINER_RESET                     // 2010.05.07 / 유명수 / 발키리 소환수변경 시 소환수 스킬창이 갱신되지 않는 버그
        #endif
        //#define _YMS_CHINA_TAIWAN_MERGE                                 // 2010.07.15 / 유명수 / 중국/대만 합침 
        #ifndef _NA_20100802_SPEED_HACK_DETECTION
            #define _NA_20100802_SPEED_HACK_DETECTION                   // 2010.08.02 / 강재준, 유명수 / 개선된 In-Game 스피드핵 탐지
        #endif
		#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 민정영, 이태준 / 시스템 메모 통합 작업(지역점령전)
        #ifdef _SERVER
            #define _NA_0_20100929_LANGUAGE_OF_FILTER                       // 2010.09.29 / 남기봉 / 필터를 위한 국가별 언어 설정 추가.
        #endif

    #elif defined(_JAPAN)

    #elif defined(_RUSSIA)

    #endif

#endif
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.9.0.5 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 905
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(4)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(1)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

    //----------------------------------------------------------------------------------------------

	/*($Delete_[x/C]$)*/#define __DH_HAIM_WIN_CLOSE								// 2009.09.11 / 이도형 / 창고 이동시 닫히기
	/*($Delete_[x/C]$)*/#define _DH_CHARNAME_CHECK_BUG_005306					// 2009/1109 / 이도형 / 캐릭터 생성시 이름이 16바이트 넘을 경우 중복체크시 죽는현상 수정
	/*($Delete_[x/C]$)*/#define _DH_GUILDHAIM_WIN_CLOSE							// 2009/1019 / 이도형 / 길드창고 멀어지면 닫는 작업
	/*($Delete_[x/C]$)*/#define _DH_AC_UI_CHANGE								// 2009/1012 / 이도형 / AC UI변경 관련 작업

	/*($Delete_[x/C]$)*/#define __KR_001355_20090624_GROUPINGSYSTEM_ALL_2		// 2009/0704 / 이도형 / 그룹 맺기 시스템-(캐릭터팝업메뉴로 그룹등록/해재), 오프라인 필터 적용안되도록 수정
	/*($Delete_[x/C]$)*/#define _DH_GIFT_CONFIRM								// 2009/1012 / 이도형 / 선물하기 재확인 창 작업
	/*($Delete_[x/C]$)*/#define _DH_HELLON_TOWER_ENTER							// 2009/0930 / 이도형 / 헬론의 성 입장 및 툴팁
	/*($Delete_[x/C]$)*/#define _DH_SUNCREDIT									// 2009/0929 / 이도형 / 썬 크래딧 영상 UI
	/*($Delete_[x/C]$)*/#define __DH_COMMUNITY_MEMO__							// 2009/0821 / 이도형 / 쪽지 줄간격 변경

    /*($Delete_[S/C]$)*/#define __NA000000_090906_SOLAR_SLOT_CODE_ARRANGEMENT__  // 2009.09.06 / 이은파 / SolarSlot 코드 정리

    /*($Delete_[S/C]$)*/#define __NA000000_090906_SOLAR_SLOT_UPDATE_VER6__       // 2009.09.06 / 이은파,김광수 / SolarSlot ItemStream Ver6 update
    /*($Delete_[S/C]$)*/#define __NA001390_090915_RIDING_SYSTEM__                // 2009.09.15 / 이은파,김광수 / Riding System
    /*($Delete_[x/C]$)*/#ifdef __NA001390_090915_RIDING_SYSTEM__
    /*($Delete_[x/C]$)*/#define _GS_RIDER_SYSTEM		// 김광수 / 탈것시스템
    /*($Delete_[x/C]$)*/#endif

    /*($Delete_[S/C]$)*/#define __NA001390_090915_RIDING_SYSTEM_ETC__            // 2009.09.20 / 이은파,김광수 / 고질적으로 복잡한 부분 수정코자... 여력이 되는 한 작업하기로 한다.
    /*($Delete_[S/C]$)*/#define __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__   // 2009.09.15 / 이은파,김광수 / Riding System - Item Part

    /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED        //< v6가 별도 목적으로 추가된 관계로...

	/*($Delete_[x/C]$)*/#define _GS_ELITE_FULLSET_CHANGE_ITEM					// 2009.10.19 / 김광수 / 엘리트풀셋2차 체인지아이템
    /*($Delete_[x/C]$)*/#define _GS_ADD_SHIELD_EFFECT                           // 2009.11.02 / 김광수 / 쉴드이펙트추가작업
    /*($Delete_[x/C]$)*/#define __GS_CHANGE_TARGET_EFFECT_TYPE                  // 2009.11.09 / 김광수 / 타겟팅이펙트타입변경의로인한버그수정
    /*($Delete_[x/C]$)*/#define _GS_ADD_RIDER_IDLE_ANI                          // 2009.11.17 / 김광수 / 탈것관련 IDLE애니메이션추가
    /*($Delete_[x/C]$)*/#define _GS_LOOP_EFFECT_REMAIN                          // 2009.11.05 / 김광수 / 루프애니메이션에서 발생되는루프이펙트 지속적 제거.생성안하기
    /*($Delete_[x/C]$)*/#define _GS_ADD_CANDY_EVENT_WEAPON                      // 2009.11.23 / 김광수 / 캔디이펙트웨폰
    /*($Delete_[x/C]$)*/#define _GS_MOD_SHADOW_HIDE_CONDITION                   // 2009.12.15 / 김광수 / 하이드조거추가(파티멤버&점령전길원에게보이기)

	/*($Delete_[x/C]$)*/#define __090907_LTJ_GRAPHIC_OPTION_ADD					// 2009.10.19 / 이태준 / 타일블렌딩 그래픽옵션추가
	/*($Delete_[x/C]$)*/#define __090907_LTJ_GAME_OPTION_ADD					// 2009.10.19 / 이태준 / 다른캐릭터,상점캐릭터 보이기 게임옵션추가
	/*($Delete_[x/C]$)*/#define __090909_LTJ_CHATDIALOG_MOD						// 2009.10.19 / 이태준 / 채팅창 채팅리스트 로직수정
	/*($Delete_[x/C]$)*/#define __LTJ_CHAT_DIALOG_POPUP_MENU_RCLICK_BUGFIX		// 2009.10.19 / 이태준 / 채팅창 채팅리스트에서 우클릭만으로 채팅팝업메뉴 안뜨는 버그수정
	/*($Delete_[x/C]$)*/#define __LTJ_TOOLTIP_KALBUSRING_EQUIPCLASS				// 2009.08.14 / 이태준 / 칼버스의 반지 착용가능 클래스 툴팁 추가
	/*($Delete_[x/C]$)*/#define __LTJ_HIDE_PET_NAME_JP							// 2009.07.28 / 이태준 / UI 숨김기능시 펫의 게이지 및 이름 숨김
	/*($Delete_[x/C]$)*/#define __090910_LTJ_CHANGEUP_CLASSNAME_JP				// 2009.09.10 / 이태준 / 체인지업 캐릭터의 경우 새로운 클래스명으로 출력
	/*($Delete_[x/C]$)*/#define _LTJ_HERO_INFO_BUGFIX							// 2009.10.30 / 이태준 / 포탈이동 시 클라/서버간의 정보 불일치 현상 및 타락한사원 입장료 차감 안되는 형상 수정 - 히어로의 정보(HERO_INFO)

    /*($Delete_[S/C]$)*/#define __NA_0_20090709_IGNORE_PKTILE_OPTION			// 2009.07.09 / 이호영,이태준 / PK 타일 무시 옵션.

    /*($Delete_[S/C]$)*/#define __NA_0_20090907_GUILD_DESTROY_GM_CMD            // 2009.09.07 / 이호영 / 길드 바로 삭제 가능 GM 명령어. 
    
    /*($Delete_[S/C]$)*/#define __NA_0_POSSESSION_ITEM_DROP						// 2009.08.27 / 김창현, 귀속 아이템 드랍 가능

    /*($Delete_[S/C]$)*/#define __NA_0_20090902_PET_NAME_CARD_LOG               // 2009.09.02 / 강재준 / 펫 네임카드 아이템 사용 로그에 대상 펫에 대한 추가 정보 포함

	/*($Delete_[x/C]$)*/#define _YJ_ADD_CONTINENTMAP_POINT						// 2009.10.20 / 윤영주 / 필드 추가에 따른 대륙지도 지역 추가

    /*($Delete_[S/x]$)*/#define _KR_001399_20091020_CRYSTALLIZE_EXTEND          // 2009.10.20 / 남기봉,이태준 / 결정화 개선. 엘리트 아이템 결정화 추가, 인챈트 Lv에 따른 결정화 아이템 갯수 설정. data\\ItemCrystalizeList.txt 인챈트 Lv필드 추가

    /*($Delete_[x/C]$)*/#define _NA_20100503_MAGIC_SKILL_SAINTAID_CANCLE_STAT_IS_CHANGE // 2010.05.03 / 민정영 / 엘리멘탈 스킬 세인트 에이드가 켄슬하는 상태이상 영역 변경 

    #if defined(_KOREA)

    #elif defined(_CHINA)
        //#undef __KR_001355_20090624_GROUPINGSYSTEM_ALL_2    // [2010-3-16 by i4u4me]  중국버전은 제거
        
    #elif defined(_JAPAN)
        #define _JP_20100114_CHANGE_ATTRIBUTE_LETECRARFE    // 2010.01.14 / 문상현 / 레테의 물병(초기화 아이템)에 패시브 초기화 기능도 추가한다.
        #define _NA_20100106_REPURCHAS_BUGFIX_2496          //2010.01.06 문상현 배틀존 들어갔다 온후 재구매할때 재구매 안되는 버그 수정
    #endif
#endif
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.9.0.4 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 904
    /////////////////////       Protocol Version       //////////////////////
    #ifndef C2S_HIGH_VERSION_NO		
        #define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
    #endif	
    #ifndef C2S_MIDDLE_VERSION_NO
        #define C2S_MIDDLE_VERSION_NO	(3)		// 중간 버전 번호
    #endif	
    #ifndef C2S_LOW_VERSION_NO
        #define C2S_LOW_VERSION_NO		(9)		// 하위 버전 번호
    #endif
    //////////////////////////////////////////////////////////////////////////

	/*($Delete_[x/C]$)*/#define _YJ_SEPARATION_EFFECT_LIST									// 2009.07.17 / 윤영주 / 이펙트 리스트 분리 작업
	/*($Delete_[x/C]$)*/#define _GS_MOD_ITEMNAME_DRAW_BY_BV									// 2009.07.30 / 김광수 / 아이템네임 피킹볼륨위에찍기
	/*($Delete_[x/C]$)*/#define _GS_BUG_FIX_PARTY_POPUP										// 2009.07.31 / 김광수 / 타겟창 팝업창 버그 수정	
	/*($Delete_[x/C]$)*/#define _GS_MOD_HERO_GAUGE_BAR_DRAW									// 2009.07.30 / 김광수 / Hero게이지(hp.mp.sd) 그릴때 줄여서 그리는방식에서 클립해서그리는방식으로변경	
	/*($Delete_[x/C]$)*/#define _GS_MOD_HERO_GAUGE_IMG_NUMBER								// 2009.08.05 / 김광수 / Hero게이지(hp.mp.sd) 수치 text가 아닌 이미지로 그리기
	/*($Delete_[x/C]$)*/#define _GS_GSP_MISSION_FORWARD_TOOLTIP								// 2009.08.18 / 김광수 / npc만났을때 뜨는창 퀘스트명 ...처리
	/*($Delete_[x/C]$)*/#define _GS_GSP_MISSION_HELP_TOOLTIP								// 2009.08.19 / 김광수 / 미션아이템설명창 텍스트설명...처리

    /*($Delete_[S/C]$)*/#define __NA000000_090613_TRIGGER_LOGIC_REARRANGE__                 // 2009.06.13 / 이은파 / 트리거 영역 관련 개선 작업
    /*($Delete_[S/C]$)*/#define __NAM00000_090621_TRIGGER_LOGIC_REARRANGE2__                // 2009.06.21 / 이은파 / 트리거 영역 관련 처리 변경:몬스터 소환 처리 수정
    /*($Delete_[S/C]$)*/#define __NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__              // 2009.06.22 / 이은파 / 비정상 접속 종료에 따른 몬스터 능력시 조정 시점 개선

    /*($Delete_[S/C]$)*/#define __NA000000_090712_SCRIPT_PARSING_SYSTEM_MODIFICATION        // 2009.07.12 / 이은파 / 스크립트 로딩 개선
    /*($Delete_[S/C]$)*/#define __NA000000_090718_SCRIPT_PARSING_SYSTEM_MODIFICATION2       // 2009.07.18 / 이은파 / 스크립트 로딩 개선 (파싱 알고리즘 교체)

    /*($Delete_[S/C]$)*/#define __NA000000_090716_HIGH_LOAD_BALANCED_NETWORK_TUNEUP__       // 2009.07.16 / 이은파 / G->C 로의 패킷 처리상 개선 작업 + 싱크 관련 검수
    /*($Delete_[S/C]$)*/#define __NA000000_090717_SHADOW_SKILL_PROBLEM_MODIFICATION__       // 2009.07.17 / 이은파 / 쉐도우 스킬들의 이레귤러 검증 및 수정
	/*($Delete_[x/C]$)*/#define __NA_0_AC_KEY_DIRECT_REGISTER								// 2009.07.08 / 김창현,이도형 / AC 열쇠 바로 등록
	/*($Delete_[S/C]$)*/#define __CN_1299_WASTE_ITEM_LEVEL_LIMITE_CHECK						// 2009.06.01 / 김창현, 물약 아이템 레벨제한 체크 추가
	/*($Delete_[S/C]$)*/#define __BUG_34356_ATTR_RECOVERY_HP_BUG							// 2009.08.07 / 김창현, HP회복 버그
	
	/*($Delete_[x/C]$)*/#define __LTJ_RADAR_MAP_NAME_BUGFIX									// 2009.07.17 / 이태준 / 미니맵 지역명 텍스트 잘리는 현상 수정 - (...) 줄임표시, 툴팁 출력
	/*($Delete_[x/C]$)*/#define __LTJ_UI_DIALOG_TEXTURE_LOAD_N_RELEASE_MOD					// 2009.07.17 / 이태준 / 불필요한 UI 텍스처 해제 기능 추가
	
    /*($Delete_[S/C]$)*/#define __NA_001369_20090705_ADD_QUEST_GRADE_MONSTERKILL			// 2009.07.05 / 이호영,이태준 / 요구 몬스터 KILL 수행 타입 추가.
	/*($Delete_[S/C]$)*/#define __NA_001370_20090706_ADD_QUEST_SEARCH_AREA        			// 2009.07.06 / 이호영,이태준 / 탐색 퀘스트
    /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / 이호영,이태준 / 퀘스트 GM명령어 추가

    /*($Delete_[x/C]$)*/#define __NA_001244_20090417_ATTACK_RESIST					        // 2009.04.17 / 이호영 / 직업별 공격 내성

    /*($Delete_[S/C]$)*/#define __NA_4128_BUG_FIX_WOUNDSTATUS_SKILLSTATTYPE                 // 2009.06.18 / 이호영 / WOUNDSTATUS 도트 데미지 스탯타입 2로 강제 되어 있는 부분 수정.
    /*($Delete_[S/C]$)*/#define __NA_0_20090618_MODIFY_KOREA_PK_RULE                        // 2009.06.18 / 이호영 / 한국 PK룰 변경.
    /*($Delete_[S/C]$)*/#define __NA_0_20090706_BUG_FIX_COLLECTION_COUNT_SYNK               // 2009.07.06 / 이호영 / AC 보물 상자 채집시 가끔 느낌표 사라지지 않는 버그.
    /*($Delete_[S/C]$)*/#define __NA_0_20090715_NPCSHOP_ITEM_REPURCHASE_LOG                 // 2009.07.15 / 이호영 / 아이템 재구매 게임로그
    /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG             // 2009.07.15 / 이호영 / 길드 랭크업시 권한 체크 하지 않는 버그.
    /*($Delete_[S/C]$)*/#define __NA_0_20090819_GUILDMARK_REGISTER_LOG                      // 2009.08.19 / 이호영 / 길드 인장 아이템 사용 로그.
    /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED                       // 2009.09.25 / 이호영 / Itemstream의 사용하지 않는 부분과 귀속 관련 부분 초기화

    /*($Delete_[S/C]$)*/#define __NA_001290_20090525_SHIELD_SYSTEM							// 2009.07.16 / 이호영,김광수 / 실드 시스템.
    /*($Delete_[S/C]$)*/#define __NA_001290_20090525_SHIELD_SYSTEM_NOTIFY_NOTABILITY_STATE  // 2009.07.16 / 이호영,김광수 / SD상태 동기화.
    /*($Delete_[x/C]$)*/#define _GS_MOD_PET_STX_POS											// 2009.07.16 / 김광수 / 펫위치 스크립트화
	/*($Delete_[x/C]$)*/#define _GS_GSP_PORTAL_NAME_OFFSET									// 2009.07.21 / 김광수 / 미니맵&풀맵 포탈명텍스트 겹치는 버그수정	

    /*($Delete_[S/C]$)*/#define __NA_0_20090818_SYSTEM_MEMO_SUPPORT                         // 2009.08.18 / 강재준 / 시스템 쪽지 지원
    /*($Delete_[S/C]$)*/#define __NA_0_20090820_SSQ_FEE_REFUND_LOG                          // 2009.08.20 / 강재준 / 타락한사원 입장 실패로 인한 입장료 환불시 하임 획득 로그 추가

    /*($Delete_[S/C]$)*/#define __NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION           // 2009.06.27 / 강재준 / 게임 내 하임 소비 정보 타 서버 통지 기능
    /*($Delete_[S/C]$)*/#define __NA_001359_20090619_HEIM_LOTTO_SYSTEM                      // 2009.06.19 / 강재준 / 하임 로또 시스템
    /*($Delete_[S/C]$)*/#define __NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION           // 2009.06.27 / 강재준 / 게임 내 하임 획득 정보 타 서버 통지 기능
    /*($Delete_[S/C]$)*/#define __KR_001355_20090624_GROUPINGSYSTEM_ALL                     //2009.06.08 / 이도형, 민정영 / 그룹핑 시스템 모든 define
    /*($Delete_[S/C]$)*/#ifdef __KR_001355_20090624_GROUPINGSYSTEM_ALL                  
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_CW                  //2009.06.08 / 민정영 / 그룹핑 시스템 [ Client	<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_GW                  //2009.06.08 / 민정영 / 그룹핑 시스템 [ Game	<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_DW                  //2009.06.08 / 민정영 / 그룹핑 시스템 [ DBP		<---->		World ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_DATA                //2009.06.08 / 민정영 / 그룹핑 시스템 [ DATA 관련 ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_GROUPINGSYSTEM_PARSER		        //2009.07.07 / 민정영 / 그룹핑 시스템 [ PARSER ]
        /*($Delete_[S/C]$)*/#define __KR_001355_20090608_EXP_ACQUISITION_NOTIFICATION
        /*($Delete_[S/C]$)*/#define __KR_001355_20090805_CHECK_GROUPSYSTEM_MSG              //그룹 시스템 메세지임을 알리는 작업 define
        /*($Delete_[S/C]$)*/#define _DH_GROUPSYSTEM_CHANGE_PACKET                           //그룹 하임패킷 CW->CG로 변경
    /*($Delete_[S/C]$)*/#endif //__KR_001355_20090624_GROUPINGSYSTEM_ALL

    /*($Delete_[S/C]$)*/#define _NA_20100415_ADJUST_POINTRANKING_SETTLE_TERMS               //2010.04.15 / 민정영 / 포인트 랭킹 결산 조건 수정 Mission Key => RegisterDate
    /*($Delete_[S/C]$)*/#define _NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG              // 2010.04.23 / 민정영 / 그룹 시스템 게임 로그 작업 

    /*($Delete_[S/C]$)*/#define _NA_0_20100517_ITEMSLOTCONTAINER_EQUIPGROUP_DETECTION       // 2010.05.18 / 강재준 / 주어진 참조 장착 위치와 같은 종류의 장착 아이템 (반지, 특수악세사리 등) 검색 기능

    #if defined(_KOREA)
        #define __NA000000_090911_NEOWIZ_USER_CCU_REPORT__ // 2009.09.11 / 이은파 / 네오위즈 채널링에 의한 유저 구분 추가 작업
  
    #elif defined(_CHINA)
		#define __DH_HAIM_WIN_CLOSE										// 2009.09.11 / 이도형 / 창고 이동시 닫히기
        /*($Delete_[S/x]$)*/#define _NA_0_20100114_QUEST_REWARD_CHECK                       // 2010.01.14 / 이호영 / 퀘스트 보상시 NPC와의 거리 체크.
        /*($Delete_[S/x]$)*/#define _NA_001290_20090525_MODIFY_CALC_SD                      // 2010.01.06 / 이호영 / SD 계산 공식 변경 
        #define _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE               // 2010.01.15 / 강재준 / [중국] 하임 행운 복권용지 직접 구입 기능
        //#define _DH_NOAPPLY_GROUPINGSYSTEM_                             // 2010.01.19 / 이도형 / 그룹 미적용        
        #define _NA_0_SETITEM_FULL_OPTION_COUNT_CHANGE                  // 2010.02.01 / 이호영 / 셋트 아이템 풀 옵션 겟수 변경 5 -> 6.
        #define _DH_CHANGE_SHADOW_SKILL_DARK_BREAK                      // 2010.02.02 / 이도형 / 다크브레이크 스킬 변경
        /*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT_DELETE             // 2010.03.08 / 이호영 / 퀘스트 반복 버그 의심 부분 수정
        /*($Delete_[S/x]$)*/#define _NA_0_20100308_ADD_QUEST_COUNT_CHECK					// 2010.03.08 / 이호영 / 퀘스트 갯수 체크 추가.
    #elif defined(_JAPAN)

    #elif defined(_GLOBAL)
        
    #elif defined(_RUSSIA)        

    #endif
#endif
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.9.0.3 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 903
/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(8)		// 하위 버전 번호
	#endif
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define __DH_ALL_000000_20090525_UNUSEABLE_MAPIMG__			// 2009.06.03 // 이도형 // 쓰지 않는 미니맵 필터
	/*($Delete_[x/C]$)*/#define __DH_ALL_000000_20090525_REMOVE_UI__				// 2009.05.29 // 이도형 // 사용하지 않는 UI 제거
	/*($Delete_[x/C]$)*/#define _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN	// 2009.05.22 // 이도형 // 장비 UI 정렬 기능(Tooltip)

	/*($Delete_[x/C]$)*/#define _GS_GSP_PK_BLOCK_ESODE								// 2009.07.30 // 김광수 // 에소드마을에서는  PK 막기
	/*($Delete_[x/C]$)*/#define _GS_GSP_SCROLL_PROFILE								// 2009.07.30 // 김광수 // 캐릭생성창 프로필 스크롤처리

	/*($Delete_[x/C]$)*/#define __KR_0_20090519_EASY_GET_LUCKYMONSTER_				//2009.05.19 / 민정영 / 럭키 몬스터 소멸카운팅을 HP 98%부터 시작

	/*($Delete_[S/C]$)*/#define __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM			// 2009.05.11 / 이호영, 이도형 / 퀘스트 대역폭 1500 -> 3000 상향 
	/*($Delete_[S/C]$)*/#define __CN_000001_20090513_ADD_FRIEND_CHECK_GM				// 2009.05.13 / 이호영 / 중국 GM 케릭터 친구 추가 못함.	(국가 공통)
	/*($Delete_[S/C]$)*/#define __CN_000002_20090513_ADD_INDUN_KIND_MAYA_SCROLL			// 2009.05.13 / 이호영 / 중국 쉐도우 입장 불가 맵		(국가 공통)
	/*($Delete_[S/C]$)*/#define __CN_000003_20090519_CANNOT_MOVE_WAREHOUSE_ELITE_ITEM	// 2009.05.19 / 이호영 / 중국 길드 창고에 보관 불가능 타입추가.(국가 공통)
	/*($Delete_[S/C]$)*/#define __CN_000004_20090521_CHANGE_COLLECT_OBTAIN_PACKET		// 2009.05.21 / 이호영, 김광수 / 중국 채집완료후 보상 패킷 변경.(국가 공통)
    /*($Delete_[S/C]$)*/#define __NA_0_20090714_POTAL_ITEM_DONOT_MOVE_GMMAP             // 2009.07.14 / 이호영 / 작전지도와 룬스톤은 GM맵과 에소드 마을로 이동할수 없으며, 좌표 저장할수 없다.
    /*($Delete_[S/C]$)*/#define __NA_0_20090714_FIX_MODIFY_GUILD_MARK_BUG               // 2009.07.14 / 이호영 / 길드 마크 스크립트 설정 레벨로 변경이 안되는 버그.
    /*($Delete_[S/C]$)*/#define __NA_0_20090720_GUILD_RENDER_INFO_MODIFY                // 2009.07.20 / 이호영, 윤영주 / 길드 랜더 인포 데이터 추가.
    /*($Delete_[S/C]$)*/#define __NA_0__20090724_BUG_FIX_GUILD_JOIN_AROUND_BRD          // 2009.07.24 / 이호영, 윤영주 / 길드 가입시 주위에 알림 패킷 변경 (길드 등급, 관계).

	/*($Delete_[S/C]$)*/#define __NA_1323_CHAO_TIME_DECREASE_ITEM						// 2009.04.16 / 김창현, 김광수 / 카오시간 감소 아이템
	/*($Delete_[S/C]$)*/#define __NA_1324_IGTYPE_IMPROVEMENT							// 2009.04.29 / 김창현 / IGType 개선
	/*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / 김창현 / DROP관련 GM 명령어 추가
	/*($Delete_[S/C]$)*/#define __NA_DKJH0002_MODIFY_POSSESSION_ITEM_RANKUP				// 2009.05.19 / 김창현 / 귀속 아이템 랭크업 수정
	/*($Delete_[S/C]$)*/#define __NA_0_QUEST_ACCEPT_ITEM_IMPROVEMENT					// 2009.05.26 / 김창현, 이태준 / 퀘스트 수락 아이템 개선
	/*($Delete_[S/C]$)*/#define __BUG_4049_QUEST_REQITEM_DELETE_BUG						// 2009.06.03 / 김창현, 퀘스트 요구 아이템 삭제 3개까지만 되는 버그
	/*($Delete_[S/C]$)*/#define __NA_0_CASH_ITEM_RENEWAL_SUPERMASTER					// 2009.07.02 / 김창현, 이태준 / 캐시아이템 리뉴얼 1.명지휘관의 지휘봉

	/*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / 이은파 / 캐시아이템 리뉴얼#2 죽은 후 사라지지 않는 버프

    /*($Delete_[S/C]$)*/#define __NA0000_090416_RETURN_STATE_MOB_CONTROL__                  // 2009.04.16 / 이은파 / 몬스터 복귀에 대한 상태 제약 처리 변경
    /*($Delete_[S/C]$)*/#define __NA000000_090506_EVENT_TYPE_MODIFICATION__                 // 2009.05.06 / 이은파 / 일반 유저 플레이타임 서비스 지원

	/*($Delete_[S/C]$)*/#define __NA001336_090514_PARTY_NAVI_SUPPORT__                      // 2009.05.14 / 이은파,이태준 / 파티원 식별 개선

	/*($Delete_[x/C]$)*/#define __001333_LTJ_QUEST_LIST_UI_REFORM					// 2009.05.22 / 이태준 / 퀘스트창 개선
	/*($Delete_[x/C]$)*/#define __001331_LTJ_AC_TREASUREBOX_TOOLTIP					// 2009.05.22 / 이태준 / AC 창 개선
	/*($Delete_[x/C]$)*/#define __001341_LTJ_POSSESSION_ITEM_MODIFY					// 2009.05.22 / 이태준 / 귀속 아이템 개선 - 귀속인증 아이콘 출력
	/*($Delete_[x/C]$)*/#define __001345_LTJ_QUEST_REPEAT_DISPLAY					// 2009.05.22 / 이태준 / 반복 궤스트 차등 표시

	/*($Delete_[S/C]$)*/#define __NA_001304_20090521_GUILD_LOG_FIX                  // 2009.05.21 / 강재준 / 길드 로그 기능 재작성
    /*($Delete_[x/C]$)*/#define __NA_0_20090812_BINARY_LOGGING_IMPROVEMENT          // 2009.08.12 / 강재준 / 바이너리 게임 로깅 개선
    /*($Delete_[x/C]$)*/#define __NA_0_20090813_BINARY_LOG_ERROR_RECOVERY           // 2009.08.13 / 강재준 / 바이너리 게임 로그 로딩시 오류 복구

	/*($Delete_[x/C]$)*/#define _YJ_MOD_GUILD_MARK_CREATE_GRADE						//2009.07.08 / 윤영주 / 길드마크를 달수있는 길드 레벨 변경(3Lv -> 1Lv)
	/*($Delete_[x/C]$)*/#define _YJ_ADD_GUILD_MARK_TEX								//2009.07.08 / 윤영주 / 길드마크 문양 추가

    /*($Delete_[x/C]$)*/#define _NA_0_20091020_COMPACT_COMPOSITION_LOG_         // 2009.10.20 / 강재준 / 조합 로그 추가 정보에서 재료 아이템 목록 잘림 방지를 위한 축약 포맷 사용

    #if defined(_KOREA)
		#define __001347_LTJ_REMOVE_TEMP_INVENTORY				// 2009.05.22 / 이태준 / 보조 창고 제거 작업

	#elif defined(_CHINA)
        /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / 이호영,이태준 / 퀘스트 GM명령어 추가
        #define __NA_0_POSSESSION_ITEM_DROP								    // 2009.08.27 / 김창현, 귀속 아이템 드랍 가능
        /*($Delete_[S/C]$)*/#define _NA_20090925_ITEMSTREAM_INIT_RESERVED                       // 2009.09.25 / 이호영 / Itemstream의 사용하지 않는 부분과 귀속 관련 부분 초기화
        #define _NA_0_20091020_COMPACT_COMPOSITION_LOG_
		#define _DH_TOOLTIP_LIMIT											// 2009.10.29 / 이도형 / 제작시 조합 가능 아이템 툴팁에 뜨는 아이템 갯수 제한

	#elif defined(_JAPAN)
        /*($Delete_[S/C]$)*/#define __NA_001369_20090705_ADD_QUEST_GRADE_MONSTERKILL			// 2009.07.05 / 이호영,이태준 / 요구 몬스터 KILL 수행 타입 추가.
        /*($Delete_[S/C]$)*/#define __NA_001370_20090706_ADD_QUEST_SEARCH_AREA        			// 2009.07.06 / 이호영,이태준 / 탐색 퀘스트
        /*($Delete_[S/C]$)*/#define __NA_001340_20090519_QUEST_GM_CMD							// 2009.05.19 / 이호영,이태준 / 퀘스트 GM명령어 추가
        #define __NA_0_AC_KEY_DIRECT_REGISTER								// 2009.07.08 / 김창현,이도형 / AC 열쇠 바로 등록
		#define _DH_AC_UI_CHANGE											// 2009/1012 / 이도형 / AC UI변경 관련 작업
		#define _DH_HELLON_TOWER_ENTER										// 2009/0930 / 이도형 / 헬론의 성 입장 및 툴팁
		#define __GS_CHANGE_TARGET_EFFECT_TYPE				                // 2009.11.09 / 김광수 / 타겟팅이펙트타입변경의로인한버그수정
		#define _GS_ADD_CANDY_EVENT_WEAPON					                // 2009.11.23 / 김광수 / 캔디이펙트웨폰
		#define _YJ_SEPARATION_EFFECT_LIST									// 2009.07.17 / 윤영주 / 이펙트 리스트 분리 작업

    #elif defined(_GLOBAL)
        #define _DH_TOOLTIP_LIMIT											// 2009.10.29 / 이도형 / 제작시 조합 가능 아이템 툴팁에 뜨는 아이템 갯수 제한
		#undef _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN

    #elif defined(_RUSSIA)
        #undef _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN	  
	#endif
#endif
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.9.0.2 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 902
/////////////////////       Protocol Version       //////////////////////
#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(6)		// 하위 버전 번호
	#endif
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define __001251_MATERIAL_ITEM_COMPOSE_LIST					// 2009.03.05 // 이태준 // 조합가능 목록 표시
	/*($Delete_[x/C]$)*/#define __001268_CHANGEUP_SKILL_TOOLTIP						// 2009.03.05 // 이태준 // 체인지업 스킬 제약조건 툴팁 추가
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_SKILL_DIALOG_TAB						// 2009.03.11 // 이태준 // 스킬다이얼로그 최근 사용탭 유지
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_CHAR_DIALOG_UPBUTTON_HIDE				// 2009.03.11 // 이태준 // 캐릭터스킬다이얼로그 포인트 업 버튼 숨김
	/*($Delete_[x/C]$)*/#define __001314_LTJ_ADD_CLOCK_OPTION_TIRED_SYSTEM			// 2009.03.12 // 이태준 // 피로도 시스템의 시계 기능 분리
	/*($Delete_[x/C]$)*/#define __001285_LTJ_MOD_CREATE_CHAR_DESCRIPTION			// 2009.03.13 // 이태준 // 캐릭터 생성 설정 텍스트 수정
	/*($Delete_[x/C]$)*/#define __LTJ_REFORM_DROP_ITEM_NOTIFY_TIMELINE				// 2009.03.18 // 이태준 // 드랍아이템 습득 알림시스템에 자동 습득 아이템추가
	/*($Delete_[x/C]$)*/#define __LTJ_DISPLAY_FULL_INVENTORY_MESSAGE				// 2009.03.20 // 이태준 // 아이템 습득 후 인벤토리에 여유 공간이 없을 시 메시지 출력
	/*($Delete_[x/C]$)*/#define __LTJ_DRAW_NEW_SERVER_ICON_SERVER_LIST				// 2009.04.01 // 이태준	// 서버선택창에서 신서버 NEW 버튼 그리기
	/*($Delete_[x/C]$)*/#define __001326_LTJ_LEVEL_COLOR_MODIFY_2ND					// 2009.04.06 // 이태준 // 적정레벨 표시 등급 변경 7단계->5단계로 변경
	/*($Delete_[x/C]$)*/#define __LTJ_CHAT_DIALOG_SCROLL_BUGFIX						// 2009.04.06 // 이태준	// 채팅창 스크롤바 최상위로 올라가는 현상 수정
	/*($Delete_[x/C]$)*/#define __LTJ_EVENT_TIMER_MODE_MODIFY						// 2009.04.21 // 이태준	// 이벤트 타이머 모드추가

	/*($Delete_[x/C]$)*/#define _YJ_RENEWAL_QUEST_UI_001265							// 09.02.05 //윤영주 //퀘스트창 리뉴얼
		/*($Delete_[x/C]$)*/#ifdef _YJ_RENEWAL_QUEST_UI_001265
			/*($Delete_[x/C]$)*/#define __DH_004145_20090710_ITEMQUESTBUG			// 2009.07.17 //이도형 //버그 수정: 아이템 퀘스트 다모은후 삭제시, 완료로 표시
			/*($Delete_[x/C]$)*/#define _KIKI_REMOVE_QUEST_INCOMPLETE_PACKET_LOGIC	// 2009.11.09 //이지연 // CG_QUEST_INCOMPLETE_SYN를 호출하지 않고, 클라이언트에서 자체적으로 퀘스트 미완료 상태로 갱신하도록 수정
			/*($Delete_[x/C]$)*/#define _KIKI_CHECK_SEND_QUEST_COMPLETE_PACKET		// 2009.11.09 //이지연 // CG_QUEST_COMPLETE_SYN 가 중복 전송되지 않도록 체크 후 보낸다.
		/*($Delete_[x/C]$)*/#endif

	/*($Delete_[x/C]$)*/#define _YJ_CONTINENT_MAP_001277							//09.02.05 //윤영주 //대륙 지도 추가
	/*($Delete_[S/C]$)*/#define _YJ_NAVI_SYSTEM_001277								//09.02.05 //윤영주 //네비 시스템 + 이은파(클라 몬스터 네비 지원 처리 in ServerCode)
    /*($Delete_[S/C]$)*/#ifdef _YJ_NAVI_SYSTEM_001277
        /*($Delete_[S/C]$)*/#define __NA001277_090205_NAVI_SYSTEM_SUPPORT__         //09.03.xx //이은파,윤영주/네비 시스템 서버쪽 지원 작업 (디파인 제거시 혼란 방지용으로 추가)
    /*($Delete_[S/C]$)*/#endif

	
	/*($Delete_[x/C]$)*/#define _YJ_TOOLTIP_ON_TEXT_001277							//09.03.09 //윤영주 //퀘스트 관련 툴팁 작업 + 이은파(클라 메시지 구분 지원 처리 in ServerCode)
	/*($Delete_[x/C]$)*/#define _YJ_REUSE_SAME_MAP_RADAR_TEXTURE					//09.03.12 //윤영주 //
	/*($Delete_[x/C]$)*/#define _YJ_COLLECTION_SYSTEM_001283						//09.03.04 //윤영주 //채집 시스템
	/*($Delete_[x/C]$)*/#define _YJ_SHOW_QUEST_EVEN_IF_UNDER_MIN_LEV				//09.04.17 //윤영주 //[001328]다음 퀘스트 미리 보기 작업

	/*($Delete_[S/C]$)*/#define __NA_0_CAN_RESURRECTION_ON_VILLAGE					// 2009.03.25 / 김창현,김광수/ 마을에서 부활 가능
	/*($Delete_[S/C]$)*/#define __NA_0_BUG_FIX_ETHER_BULLET_BY_GM_CMD				// 2009.01.07 / 김창현, 에테르 서버 뻗는 버그 수정
	/*($Delete_[S/C]$)*/#define __NA_1241_AUTO_PICKUP_PET_OPTION					// 2009.01.14 / 김창현, 김광수 / 아이템 자동획득하는 옵션
	/*($Delete_[S/C]$)*/#define __NA_1247_POSSESSION_ITEM							// 2009.01.17 / 김창현, 이태준 / 귀속 아이템
	/*($Delete_[S/C]$)*/#define __NA_1269_CHANGE_STYLE_OPTION_BREAKER_OF_FUZ		// 2009.02.05 / 김창현, 휴즈의 브레이커 3타에만 옵션 적용 

	/*($Delete_[S/C]$)*/#define __NA_1288_AC_RENEWAL_FOR_TIME_LINE					// 2009.02.19 / 김창현, 이태준 / [타임라인]AC 리뉴얼
		/*($Delete_[S/C]$)*/#ifdef __NA_1288_AC_RENEWAL_FOR_TIME_LINE
			/*($Delete_[S/C]$)*/#define _LTJ_AC_REMOVE_CORRUPT_TEMPLE_ADD_MAIN_QUEST	// 2009.03.23 / 이태준, AC 리뉴얼 - 미션 타락한사원 제거, 각지역 메인퀘스트 클리어 횟수 추가
		/*($Delete_[S/C]$)*/#endif
	/*($Delete_[S/C]$)*/#define __NA_1248_SSQ_ADD_LOG								// 2009.01.20 / 김창현, 타락한 사원 로그
	/*($Delete_[S/C]$)*/#define __NA_1307_LOTTERY_SCRIPT_IMPROVEMENT				// 2009.03.02 / 김창현, 복권 스크립트 개선
	/*($Delete_[S/C]$)*/#define __NA_0_ADD_GM_CMD_ABOUT_AC							// 2009.03.05 / 김창현, AC관련 GM명령어 추가
	/*($Delete_[S/C]$)*/#define __NA_1313_AC_OVER_MODIFY							// 2009.03.10 / 김창현, AC 달성률 오버플로 수정
	/*($Delete_[S/C]$)*/#define __NA_0_TEMP_INVENTORY_DELETE						// 2009.03.21 / 김창현, 이태준
	/*($Delete_[S/C]$)*/#define __NA_0_RECALL_ALLIANCE_CAN_USE_ALONE				// 2009.03.25 / 김창현, 혼자 여신의 은혜 사용 가능

	/*($Delete_[S/C]$)*/#define __NA_0_AC_FOR_REAL_TIME_REFRESH						// 2009.02.14 / 김창현, 이태준/ AC 실시간 갱신을 위한 작업

	/*($Delete_[x/C]$)*/#define _DH_KOR_000000_20090522_CHARACTER_SCENE_NAME_OVERFLOW_REPAIR	// 2009.05.25 / 이도형 / 캐릭터 선택에서 키큰 캐릭터 이름 상시 표시, NO.001346

	/*($Delete_[S/C]$)*/#define __NA_0_20080414_GUILD_WAREHOUSE_LOG_BUG							// 2009.04.14 / 이수택 / 길드 창고 로그 잘못 남기던 버그.

	/*($Delete_[x/C]$)*/#define __NA003373_20090402_NOT_FIND_ENGLISHNAME_BUG					// 2009.04.02 / 이수택 / 길드 초대시 영문 대소문자를 구분하는 현상.
	/*($Delete_[S/C]$)*/#define __NA002150_20090402_GUILD_ALLIENCE_NOT_CHK_DESTROY_WAIT_BUG		// 2009.04.02 / 이수택 / 길드 해산 대기중인 길드에게 동맹 신청이 가능한 현상.
	/*($Delete_[S/C]$)*/#define __NA_0_20090407_QUESTACCEPT_GM_CMD								// 2009.04.07 / 이수택 / 퀘스트 강제 수락 gm 명령어
	/*($Delete_[S/C]$)*/#define __NA_0_20090407_QUESTSCRIPT_LOAD_MODIFY							// 2009.04.07 / 이수택 / 퀘스트 스크립트 부여 아이템 항목 3필드 적용 안되는 버그.
	/*($Delete_[S/C]$)*/#define __NA_0_20090408_GUILD_RELATION_CHECKRIGHT_BUG					// 2009.04.08 / 이수택 / 길드 동맹시 권한이 없음 나오는 버그.
	/*($Delete_[S/C]$)*/#define __NA_0_20090324_RANKINGSYSTEMCTL								// 2009.03.24 / 이수택 / 배틀 서버 죽는 버그 임시 수정
	/*($Delete_[S/C]$)*/#define __NA001254_20090109_RTTG_FUNC_ADD								// 2009.01.09 / 이수택 / 웁스 명령 추가
	/*($Delete_[S/C]$)*/#define __NA000000_20090320_DELAY_ITEM_DROP_TIME						// 2009.03.20 / 이수택 / 몬스터 아이템 지연 드랍 
	/*($Delete_[S/C]$)*/#define __NA001140_20080128_SOUND_TRIGGER_ADD							// 2009.01.28 / 이수택, 윤영주 / 사운드 재생 트리거 추가
	/*($Delete_[S/C]$)*/#define __NA001282_20090206_MODIFY_QUEST_TIME							// 2009.02.06 / 이수택, 윤영주 / [타임라인]퀘스트 부여 시간 제한 개선
	/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECT										// 2009.02.11 / 이수택 / [타임라인]채집 시스템
		/*($Delete_[S/C]$)*/#ifdef __NA001283_20090221_COLLECT
			/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_PACKET			
			/*($Delete_[S/C]$)*/#define __NA001283_20090221_COLLECTION_PARSER
			/*($Delete_[S/C]$)*/#define __NA001283_20090223_COLLECTION_CLASS
			/*($Delete_[S/C]$)*/#define __NA001283_20090225_COLLECTION_USE
			/*($Delete_[S/C]$)*/#define __NA001283_20090227_COLLECTION_LIST
			/*($Delete_[S/C]$)*/#define __NA001283_20090303_COLLECTION_GM
			/*($Delete_[S/C]$)*/#define __NA001283_20090303_COLLECTION_MAPOBJECTID
		/*($Delete_[S/C]$)*/#endif

	/*($Delete_[x/C]$)*/#define _GS_MOD_PET_GAUGE_POS_1242							// 2009.03.103 김광수 / 펫 상태 UI 위치 고정
	/*($Delete_[x/C]$)*/#define __GS_ADD_REMOVE_TUTORIAL_MSG_1315					// 2009.03.13  김광수 / 튜토리얼 메시지제거
	/*($Delete_[x/C]$)*/#define __GS_MOD_REWARD_ACTIVE_FOR_CLASS					// 2009.03.18  김광수 / 퀘스트보상창에서 자신클래스맞는아이템선택활성화 
	/*($Delete_[x/C]$)*/#define __GS_ADD_PANDORA_BOX_NAME							// 2009.03.18  김광수 / 판도라상자 네임 아이템과동일하게 변경 
	/*($Delete_[x/C]$)*/#define __GS_MOD_REWARD_ITEM_LIST_COLOR						// 2009.03.19  김광수 / 퀘스트보상창아이템 보상시와 임무시 색다르게변경
	/*($Delete_[x/C]$)*/#define __GS_ADD_SCREEN_OPTION_SCROLL						// 2009.03.22  김광수 / 그래픽창해상도 설정관련콤보박스 마우스휠스크롤추가
	/*($Delete_[x/C]$)*/#define __GS_MOD_WAYPOINT_LEVEL_CHECK						// 2009.03.25  김광수 / 웨이포인트 레벨표시및 제한체크
	/*($Delete_[x/C]$)*/#define __CN_000097_GM_HIDE_TAGETING_BLOCK					// 2009.05.13  김광수 / GM캐릭투명상태시 타겟팅불가

    /*($Delete_[S/C]$)*/#define __NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__         // 2009.02.09 / 이은파,김광수 / [타임라인] 퀘스트 및 필드에서의 미션 + 배틀존 제한 설정
    /*($Delete_[S/C]$)*/#define __NA001310_BATTLEZONE_PORTAL_RETURN_TARGET_MOVE_SUPPORT__   // 2009.02.26 / 이은파,김광수 / 포탈을 통한 마을|필드 복귀시 지정된 맵으로 복귀 지원
    /*($Delete_[S/C]$)*/#define __NA001338_090519_TUTORIAL_FILED_TYPE_CHANGE__              // 2009.05.19 / 이은파,김광수 / 에소드 마을 (필드타입에서 마을 타입으로 변경)


    /*($Delete_[x/C]$)*/#define __NA_4710_20090819_REPAIREMONEY_FIX                     // 2009.08.24 / 민정영 / 수리 금액의 클라이언트와 서버간의 소수점 계산차이에의한 차이버그
    /*($Delete_[x/C]$)*/#define __NA_34726_20090421_ARMDRAGON_TRANSFORM_BUG_FIX //2009.04.21 / 민정영 / 암드레곤 트렌스폼 변신후 게임 종료시 캐쉬 물약 사라지는 버그 수정


    /*($Delete_[S/C]$)*/#define __NA000000_090310_GAMEFIELD_RECURSIVELY_CALL_PROBLEM_FIX__  // 2009.03.10 / 이은파 / Object에 대한 제거시 재귀 구조 문제 해결 작업
    /*($Delete_[S/C]$)*/#define __NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__   // 2009.03.16 / 이은파 / [타임라인] 배틀존 제한 설정 + 로비에서도 해당 제약조건 걸리도록...
    /*($Delete_[S/C]$)*/#define __NA001319_SAME_CLASS_LIMIT_REMOVE__                        // 2009.03.16 / 이은파 / 동일 클래스 제약(5명) 제거
    /*($Delete_[S/C]$)*/#define __NA000000_090320_v0902_LOGIC_CHANGE_ABOUT_QUEST__          // 2009.03.20 / 이은파 / 0902 수정 요청사항들 (퀘스트 관련)
    /*($Delete_[S/C]$)*/#define __NA000000_090327_MONSTER_HELP_FILTER__                     // 2009.03.27 / 이은파 / Grade 9,5의 경우는 Help에 응답하지 않도록 처리
    /*($Delete_[S/C]$)*/#define __NA1335_090407_PORTAL_MISSION_ROOM_CONSTRAINTS__           // 2009.04.07 / 이은파 / 포탈 미션 수행시 '명지휘관의 지휘봉'과 같은 배틀존에 설정되는 속성 적용되도록...
    /*($Delete_[S/C]$)*/#define __NA_1317_20090316_SKILL_DAMAGE_FORMULA_MODIFY		// 2009.03.16 / 이준석 / 스킬데미지 공식 수정
	/*($Delete_[S/C]$)*/#define __NA_1320_20090316_STATUS_RATIO_FORMULA_MODIFY		// 2009.03.16 / 이준석 / 이상상태 확률 공식 수정
	//#define __NA_0_20090317_ATTACKPOWER_FORMULA_MODIFY			// 2009.03.17 / 이준석 / 발키리 공격력 공식 수정 (원복 처리)
	/*($Delete_[S/C]$)*/#define __NA_0_20090318_AGGRO_ABILITY_MODIFY				// 2009.03.18 / 이준석 / 어그로 어빌리티 수정
    

  //------------------------------------------------------------------------------------------------
  // (BUG-FIX)
  /*($Delete_[S/C]$)*/#define __BUG_35162_COMPOSE_BUG_BY_FULL_IVEN					// 2009.06.29 / 김창현, 인벤토리가 가득찬 상태에서 제작조합 시 버그

	#ifdef _KOREA
        #define __KR000000_090128_CHANNELING__                              // 2009.01.28 / 이은파,이태준 / 채널링 관련 변경 작업
	#elif defined(_JAPAN) 
		#define __LTJ_JP_TEMP_INVEN_MOD	
		#define __LTJ_TOOLTIP_KALBUSRING_EQUIPCLASS						// 2009.08.14 / 이태준 / 칼버스의 반지 착용가능 클래스 툴팁 추가
		#define __LTJ_DISABLE_HUNTINGZONE_BUTTON_JP						// 2009.07.30 / 이태준 / 배틀존 로비 헌팅관련 UI 비활성화
		#define __LTJ_HIDE_PET_NAME_JP									// 2009.07.28 / 이태준 / UI 숨김기능시 펫의 게이지 및 이름 숨김
        /*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / 김창현 / DROP관련 GM 명령어 추가		
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG         // 2009.07.15 / 이호영 / 길드 랭크업시 권한 체크 하지 않는 버그.(일본버그)
        /*($Delete_[S/C]$)*/#define __NA_0_20090706_BUG_FIX_COLLECTION_COUNT_SYNK           // 2009.07.06 / 이호영 / AC 보물 상자 채집시 가끔 느낌표 사라지지 않는 버그.

		/*($Delete_[S/C]$)*/#define __NA_0_20090709_IGNORE_PKTILE_OPTION					// 2009.07.09 / 이호영,이태준 / PK 타일 무시 옵션.

        /*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / 이은파 / 캐시아이템 리뉴얼#2 죽은 후 사라지지 않는 버프

	#elif defined(_CHINA)	
		/*($Delete_[S/C]$)*/#define __CN_000001_20090513_ADD_FRIEND_CHECK_GM				// 2009.05.13 / 이수택 / 중국 GM 케릭터 친구 추가 못함.
		/*($Delete_[S/C]$)*/#define __CN_000002_20090513_ADD_INDUN_KIND_MAYA_SCROLL			// 2009.05.13 / 이수택 / 중국 쉐도우 입장 불가 맵
		/*($Delete_[S/C]$)*/#define __CN_000003_20090519_CANNOT_MOVE_WAREHOUSE_ELITE_ITEM	// 2009.05.19 / 이수택 / 중국 길드 창고에 보관 불가능 타입추가.
		/*($Delete_[S/C]$)*/#define __CN_000004_20090521_CHANGE_COLLECT_OBTAIN_PACKET		// 2009.05.21 / 이수택, 김광수 / 중국 채집완료후 보상 패킷 변경.
		#define __CN_000005_20090522_WAREHOUSE_ISACCEPTABLE_X			// 2009.05.22 / 이수택 / 중국 창고 비번에 알파벳 x 사용.
        #define __CN_0_20090617_PRE_CHAO_USE_FLAG	                    // 2009.06.16 / 이수택 / 중국 서버에서는 프리카오가 상점이용 가능.
        /*($Delete_[S/C]$)*/#define __NA_4128_BUG_FIX_WOUNDSTATUS_SKILLSTATTYPE             // 2009.06.18 / 이수택 / WOUNDSTATUS 도트 데미지 스탯타입 2로 강제 되어 있는 부분 수정.
	    /*($Delete_[S/C]$)*/#define __BUG_4049_QUEST_REQITEM_DELETE_BUG						// 2009.06.03 / 김창현, 퀘스트 요구 아이템 삭제 3개까지만 되는 버그
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_NPCSHOP_ITEM_REPURCHASE_LOG             // 2009.07.15 / 이호영 / 아이템 재구매 게임로그

        /*($Delete_[x/C]$)*/#define __CN_0_20090706_CHINA_VISTA_CHARNAME                 	// 2009.07.06 / 이호영 / 중국 비스타 테스트

#elif defined(_GLOBAL) || defined(_RUSSIA)
        /*($Delete_[S/C]$)*/#define __NA_0_20090715_GUILD_RANKUP_NOT_CHECKRIGHT_BUG         // 2009.07.15 / 이호영 / 길드 랭크업시 권한 체크 하지 않는 버그.
        /*($Delete_[S/C]$)*/#define __NA_0_20090714_FIX_MODIFY_GUILD_MARK_BUG               // 2009.07.14 / 이호영 / 길드 마크 스크립트 설정 레벨로 변경이 안되는 버그.
        /*($Delete_[S/C]$)*/#define __NA_0_20090720_GUILD_RENDER_INFO_MODIFY                // 2009.07.20 / 이호영, 윤영주 / 길드 랜더 인포 데이터 추가.
        /*($Delete_[S/C]$)*/#define __NA_0__20090724_BUG_FIX_GUILD_JOIN_AROUND_BRD          // 2009.07.24 / 이호영 / 길드 가입시 주위에 알림 패킷 변경 (길드 등급, 관계).
		#define _YJ_MOD_GUILD_MARK_CREATE_GRADE							// 2009.07.08 / 윤영주 / 길드마크를 달수있는 길드 레벨
        #define _US_ACCOUNT_LENTH_UNCHECKED_                            // 2009.08.28 / 민정영 / 계정 길이 체크하지 않음(북미 버전 제한)
        #define _US_SHOP_GIFT_ERRORCONTROL_ADD                          // 2009.11.11 / 민정영,김광수 / 북미 선물하기 예외, 에러 처리 추가 
        /*($Delete_[S/C]$)*/#define __NA_1325_GM_CMD_ADD_ABOUT_DROP							// 2009.05.08 / 김창현 / DROP관련 GM 명령어 추가  / 2009.09.16 /민정영 북미 테스트용 추가
        /*($Delete_[S/C]$)*/#define __NA001336_090514_PARTY_NAVI_SUPPORT__                  // 2009.05.14 / 이은파,이태준 / 파티원 식별 개선 : 2009.11.10 체력 게이지 오버 버그 수정 디파인
        /*($Delete_[S/C]$)*/#define __NA_0_20090618_MODIFY_KOREA_PK_RULE                    // 2009.06.18 / 이호영 / 한국 PK룰 변경. : 2009.12.02 / 민정영 / pk룰 변경 북미 반영
/*  */#if defined(_GLOBAL) || defined(_RUSSIA)
        /*($Delete_[S/C]$)*/#define __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__       // 2009.07.08 / 이은파 / 캐시아이템 리뉴얼#2 죽은 후 사라지지 않는 버프
/*  */#endif        
    #endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.9.0.1 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 901
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(5)		// 하위 버전 번호
	#endif	
	//////////////////////////////////////////////////////////////////////////
	/*($Delete_[x/C]$)*/#define _GS_MOD_NANUM_FONT_1239								// 2009.02.19 //김광수 // 나눔폰트적용
	/*($Delete_[x/C]$)*/#define __GS_MOD_HIDE_PET_S0032								// 2009.01.22 //김광수 // 쉐도우하이드시펫도하이드+소환몬스터타겟팅변경
	/*($Delete_[x/C]$)*/#define	_GS_ADD_HIDE_ANI_0029								// 2009.01.06 //김광수 // 쉐도우하이드스킬사용시 모션추가

	/*($Delete_[S/C]$)*/#define __NA_S00015_20080828_NEW_HOPE_SHADOW				// 2008.08.28 / 조재식, 김광수, 윤영주 / 섀도우 ( 섀도우 관련 모든 작업은 이 디파인을 묶을 것. )
	/*($Delete_[S/C]$)*/#define __S00013_SHADOW_BASE_WORK							// 2008.08.11 / 최정록 / 쉐도우 베이스작업
	/*($Delete_[S/C]$)*/#define __NA_001196_20081110_RECOVERY_ADD_OPTION			// 2008.11.10 / 조재식 / 리커버리 스킬 수정.( 리커버리 챈스 )
	/*($Delete_[S/C]$)*/#define __NA_001206_20081112_DUPEQUIP_GROUP					// 2008.11.12 / 조재식 / 장착중복그룹 필드 추가.
	/*($Delete_[S/C]$)*/#define __NA_0S0023_20081114_SPAWN_MONSTER_QUEST			// 2008.11.14 / 조재식, 윤영주 / NPC ==> 몬스터 변신 퀘스트
	/*($Delete_[S/C]$)*/#define __NA_S00028_SHADOW_ADD_WORK							// 2008.12.17 / 조재식 / 섀도우 하이드, 소울컨트롤, 패시브, 시체관련 추가 구현
	/*($Delete_[S/C]$)*/#define __NA_000000_20090211_SUMMON_HP_BUG_FIX				// 2009.02.11 / 조재식, 김광수 / 섀도우 소환체 HP 버그 수정.
	/*($Delete_[S/C]$)*/#define __NA_001270_20090209_GUILD_PENALTY_FIX				// 2009.02.09 / 조재식 / 길드 패널티 수정.
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20090910_WOPS2610						// 2009.02.10 / 조재식 / 헤르메시즘의 성서 사용시 소켓옵션이 사라지는 현상 버그 수정.


	/*($Delete_[S/C]$)*/#define __NA_1165_ETHER_DEVICE_SAVE_DB						// 2008.10.26 / 김창현, 김광수 / 에테르 방출장치 정보 DB저장
	/*($Delete_[S/C]$)*/#define __NA_1131_PET_NAME_SETTING							// 2008.11.18 / 김창현, 김광수 / 펫 이름 설정, 이모티콘
	/*($Delete_[S/C]$)*/#define __NA_1170_CHARGE_INVEN_REGISTER_QUICK_SLOT			// 2008.09.23 / 김창현 / 유료 인벤의 슬롯도 퀵슬롯 등록하도록...
	/*($Delete_[S/C]$)*/#define __NA_1234_LOTTERY_ITEM_RESULT_BUG_FIX				// 2008.12.09 / 김창현, 김광수 / 복권 아이템 습득 메세지
	/*($Delete_[S/C]$)*/#define __NA_1168_ADD_CONTENTS_ADD_LOG						// 2008.11.06 / 김창현 / 로그 추가 작업
	/*($Delete_[S/C]$)*/#define __NA_1233_2009_NEW_YEARS_DAY_EVENT					// 2008.12.30 / 김창현, 김광수 / 2009년 구정 이벤트
	/*($Delete_[S/C]$)*/#define __NA_1260_CHANGE_LEVEL_FOR_STAT_INIT				// 2009.01.12 / 김창현, 이태준 / 무료 스탯 초기화 레벨 변경
	/*($Delete_[S/C]$)*/#define __NA_01240_GM_CMD_ADD_SERVER_TIME					// 2009.01.09 / 김창현, 이태준 / GM명령어 추가, 서버시간, 최대 데미지 등등	
	/*($Delete_[S/C]$)*/#define __NA_0_SHADOW_HIDE_BUG_FIX							// 2009.02.20 / 김창현, 김광수 /쉐도우 하이드 버그 수정

	/*($Delete_[S/C]$)*/#define _YJ_SHADOW_S00013
	/*($Delete_[S/C]$)*/#ifdef _YJ_SHADOW_S00013
		/*($Delete_[S/C]$)*/#define _YJ_SHADOW_SKILL_S00013
		/*($Delete_[S/C]$)*/#define _GS_ADD_WEAPON_TYPE
		/*($Delete_[S/C]$)*/#define _YJ_ADD_SKILL_SCRIPT_FIELD_4_AUTO_ATTACK_001212 // 2008.11.25 / 윤영주 / 스킬 오토 공격(스킬사용후 자동공격 할지 안할지)관련 스크립트 필드 추가 
		/*($Delete_[S/C]$)*/#define _K2P_ADD_CREATE_SHADOW_S00018					// 2008.11.21 / 김경표 / 새도우 생성 관련
		/*($Delete_[S/C]$)*/#define _K2P_NEW_SELECT_MAP_							// 2008.11.21 / 김경표 / 새로운 배경 관련 세팅
		/*($Delete_[S/C]$)*/#define _YJ_SHADOW_CRITICAL_DAMAGE_EFFECT					//09.01.05 //윤영주 //쉐도우 캐릭일 경우 다른 크리티컬 이펙트 출력
	/*($Delete_[S/C]$)*/#endif //_YJ_SHADOW_S00013

	/*($Delete_[x/C]$)*/#define _YJ_MOD_DEATH_ANI									//08.12.09 //윤영주 //웨폰타입에 따라 죽는 모션 달라지게 수정
	/*($Delete_[x/C]$)*/#define _YJ_MOD_DAMAGE_EFFECT_POS_001188					//08.12.03 //윤영주 //데미지 이펙트 위치 수정(타겟 바운딩볼륭의 중간값에서 가슴본으로)
	/*($Delete_[x/C]$)*/#define _YJ_SHADOW_QUEST_S00023								//08.12.03 //윤영주 //쉐도우 퀘스트
	/*($Delete_[C]$)*/#define _YJ_CHANGE_GUILD_NAME_001201						    //08.11.18 //윤영주 //길드명 변경 기능 추가

    //! 2011.5.3 / i4u4me / 캐릭팀(조흥래팀장)에서 타격시 딜레이 주지 않게 요청
    //! 관련 작업 undef 함;
	//#define _YJ_MOD_BLOW_MOTION_001188						//08.12.03 //윤영주 //타격감 개선

	/*($Delete_[C]$)*/#define _YJ_RETIRE_EXECUTED_TRIGGER_OBJECT_001014			    //08.12.16 //윤영주 //트리거 작동후 마우스 커서 수정
	/*($Delete_[C]$)*/#define _YJ_SPAWN_MONSTER_WITH_EFFECT_001250				    //08.12.22 //윤영주 //몬스터소환시 이펙트 출력
	/*($Delete_[C]$)*/#define _YJ_PRELOADING_MONSTER								//09.01.05 //윤영주 //트리거 몬스터 스폰시 안버벅이게 수정
	/*($Delete_[C]$)*/#define _YJ_RENEWAL_KNOCKBACK_MOTION_001256					//09.01.07 //윤영주 //넉백 모션 리뉴얼
	/*($Delete_[C]$)*/#define _YJ_ETC_4_PERFORMANCE								    //09.05.22 //윤영주 //클라이언트 성능 개선
	/*($Delete_[C]$)*/#define _YJ_TEST_FREE_ALL_CHCHED_DATA_WHEN_MAP_LOADING		//09.05.22 //윤영주 //미션,필드 진입시 몬스터 선로딩
	/*($Delete_[C]$)*/#define _KIKI_MODIFY_RESOURCE_MANAGER						    // 2009.06.09	/ 이지연 /	ResourceManager 관련 로직 수정 - 삭제대기리스트목록을 2분에 한번씩 체크 (30분이상 참조되지 않은 유닛 제거)

	/*($Delete_[C]$)*/#define	_K2P_SORT_TEXT_UI_001210							// 2008.11.21 / 김경표 / 텍스트 정렬 UI 수정 작업
	/*($Delete_[C]$)*/#define _K2P_CLOSE_WINDOW_AFTER_RETIRVE_001198				// 2008.11.21 / 김경표 / 게임 내 기능 개선[1] / 부활시 자동으로 팝업창 닫기
	/*($Delete_[C]$)*/#define _K2P_WATER_WAVE_001184								// 2008.12.17 / 김경표 / 수면 잔상효과 추가
	/*($Delete_[C]$)*/#define _K2P_AFTER_JUMP_WATER_EFFECT_001182					// 2008.12.17 / 김경표 / 물속 착지 이펙트 수정

	/*($Delete_[C]$)*/#define _K2P_ITEM_DISPLAY_OPTION_001232					    // 2008.12.17 / 김경표 / 드랍 및 습득 아이템 이름 표기
	/*($Delete_[C]$)*/#ifdef _K2P_ITEM_DISPLAY_OPTION_001232
		/*($Delete_[C]$)*/#define __001232_ITEM_DISPLAY_OPTION__REFORM			    // 2008.12.13 / 이태준 / 드랍 및 습득 아이템 이름 표기 에 대한 개선 작업
	/*($Delete_[C]$)*/#endif
		
	/*($Delete_[C]$)*/#define _K2P_MONSTER_IDLE_ANI_001246						    // 2009.01.12 / 김경표 / 캐릭터 생성창 쉬바르츠 애니코드 변경

	/*($Delete_[x/C]$)*/#define __001193__20081107_LEVEL_COLOR_MODIFY__				// 2008.11.21 / 이태준 / 적정 레벨 표시 변경

	/*($Delete_[C]$)*/#define  __001199__20081117_SYSTEM_REFORM					        // 2008.11.21 / 이태준 / 게임 내 기능 개선[2]
	/*($Delete_[C]$)*/#ifdef __001199__20081117_SYSTEM_REFORM
		/*($Delete_[C]$)*/#define __001199_CHAT_WHEEL_SCROLL							// 채팅창 휠 스크롤 적용
		/*($Delete_[C]$)*/#define __001199_GUILD_DUTY_COLOR							    // 길드 지위 부여  선택 군단 셀 컬러 변경
		/*($Delete_[C]$)*/#define __001199_ITEM_DROP_MOUSEOVER_ALT					    // ALT 토글시 마우스 픽킹되는 아이템명 색 변경
		/*($Delete_[C]$)*/#define __001199_DELETE_INVITE_BLOCK_REVENGE				    // 차단, 리벤지 창에서 파티초대 버튼 삭제
		/*($Delete_[C]$)*/#define __001199_BATTLEZONE_LOBBY_CANCEL_BUTTON				// 배틀존 로비에서 Ready 버튼 클릭시 Cancel로 변경
	/*($Delete_[C]$)*/#endif
	
    /*($Delete_[C]$)*/#define __001225__CHECK_INVENTORY_LOCK_USE_ITEM				    // 2008.12.01 / 이태준 / 인벤토리 잠금상태 어뷰징 에외처리

	/*($Delete_[C]$)*/#define __001240_GM_COMMAND_INVENTORY_ITEM_LISTUP			        // 2008.01.07 / 이태준 / GM명령어 - 인벤토리 및 창고 아이템 목록 파일출력
	
	/*($Delete_[S/C]$)*/#define __NA001284_20090205_MODIFY_SUMMONED_ATTR				// 2009.02.05 / 이수택 / 서먼 소환체 공식 수정
	/*($Delete_[S/C]$)*/#define _NA_001231_20081216_ADD_OPTION_KIND						// 2008.12.31 / 이수택 / PVP 개선을 위한 옵션 추가
	/*($Delete_[x/C]$)*/#define __NA003322_20090217_NOT_FIND_ENGLISHNAME_BUG			// 2009.02.17 / 이수택 / 영문 케릭터 이름을 찾지 못하는 버그.
	/*($Delete_[S/C]$)*/#define __NA_0_20090202_BUGFIX_RECV_SHOP_ITEMLIST_DUPLICATION	// 2009.02.02 / 이수택 / 샵 아이템 중복 적용 버그.
	/*($Delete_[S/C]$)*/#define __NA000000_20081106_GUILD_NAME_CHANGE					// 2008.11.06 / 이수택, 윤영주 / 길드명 변경	(작업요청서[1201])
	/*($Delete_[S/C]$)*/#define __NA_003197_20090114_CHARACTER_POS_SYNC_BUG_FIX			// 2009.01.14 / 이수택 / 스킬 사용 실폐시 케릭터 위치 동기화
	/*($Delete_[S/C]$)*/#define __NA_001253_20090107_MAX_CHARACTER_MODIFY				// 2009.01.07 / 이수택 / 케릭터 생성 MAX 치 변경

	/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT						// 2008.10.24 / 이수택, 이태준 / 개인 웨이 포인트
	/*($Delete_[S/C]$)*/#ifdef __NA001164_20081024_CHAR_WAY_POINT		
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_SET_DB_QUERY
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_SET_DB_PACKET
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_SET_DB_FUNC
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_PACKET
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_FUNC
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_ITEM
		/*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_GM_CMD
        /*($Delete_[S/C]$)*/#define __NA001164_20081024_CHAR_WAY_POINT_PORTAL
	/*($Delete_[S/C]$)*/#endif

	/*($Delete_[S/C]$)*/#define __NA_0_20080930_AUTH_REFACTORING					// 2008.09.30 / 이준석 / 인증 리팩토링 중에 필요한 게임쪽 소스 변경
	/*($Delete_[S/C]$)*/#define __NA_0_20081029_PARTY_REFACTORING					// 2008.10.29 / 이준석, 김경표 / 파티 라이브러리 AgentServer 적용 및 리팩토링
	/*($Delete_[S/C]$)*/#define __NA_0_20081124_PARTY_REFACTORING_GAMESERVER		// 2008.11.24 / 이준석 / 파티 라이브러리 GameServer 적용

    /*($Delete_[S/C]$)*/#define __NA001230_SSQ_UPDATE_ADD_EXP_FIELD_SUPPORT__		// 2008.12.04 / 이은파 / 타락한 사원 추가 경험치 보상 별도 추가 처리
    /*($Delete_[S/C]$)*/#define __NA000000_090224_PARTY_RETURN_PROBLEM_CORRECTION__ // 2009.02.24 / 이은파 / 방장 로딩시 복귀 안되는 문제 수정
    /*($Delete_[x/C]$)*/#define __NA_0_20081222_BUGFIX_INITPOS_OF_MAPOBJ_EFFECT       // 2008.12.23 / 김민조 / 맵 오브젝트 이펙트 초기 위치 오류 수정

	#ifdef _KOREA
		
	#elif defined(_CHINA) 
		#ifdef  __CN_CREATE_BLOCK_SHADOW
			#undef _K2P_ADD_CREATE_SHADOW_S00018				// 2008.11.21 / 김경표 / 새도우 생성 관련
		#endif//__CN_CREATE_BLOCK_SHADOW
	
	#elif defined(_JAPAN) 
		#ifdef  __CN_CREATE_BLOCK_SHADOW
			#undef _K2P_ADD_CREATE_SHADOW_S00018				// 2008.11.21 / 김경표 / 새도우 생성 관련
		#endif//__CN_CREATE_BLOCK_SHADOW
	
	#elif defined(_GLOBAL)
		#ifdef  __CN_CREATE_BLOCK_SHADOW								// 2009.07.23 / 김광수 / 새도우 생성 막기
			#undef _K2P_ADD_CREATE_SHADOW_S00018						// 2008.11.21 / 김경표 / 새도우 생성 관련
		#endif//__CN_CREATE_BLOCK_SHADOW
	#endif

#endif

#ifdef __NA_0_20080930_AUTH_REFACTORING
    #include "./SunAuth/AuthDefine.Build.h"
#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.8.1.8 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 818
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(4)		// 하위 버전 번호
	#endif	
//////////////////////////////////////////////////////////////////////////

	/*($Delete_[S/C]$)*/#define __NA001226_INSTANCE_DUNGEON_EXTENTION__				// 2008.12.01 / 이은파, 김광수 / 인스턴스 던젼 이벤트 맵 방식 추가
	/*($Delete_[S/C]$)*/#define __NA_000000_20081202_ADD_EXP_ACTION_2				// 2008.12.02 / 조재식 / 경험치 주기 ==> 파티시 해당 맵에 있는 모든 유저에게 경험치 주기로 변경.
	/*($Delete_[S/C]$)*/#define __NA_0_20081209_ONLEVELUP_BUG_FIX					// 2008.12.09 / 이준석/ 경험치를 많이 받아도 한꺼번에 레벨업이 되지 않고 하나씩 되는 문제 수정
	#ifdef _JAPAN
		#define __090512_LTJ_ETHER_CHARGE_JP_BUGFIX				// 2009.05.12 / 이태준 / 에테르 장전상태에서 서버선택 이나 필드에서 사망 후 마을로 귀환할 때 장전상태 이상 현상 수정		
	#endif
#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.8.1.7 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 817
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(3)		// 하위 버전 번호
	#endif	
	//////////////////////////////////////////////////////////////////////////

	
	/*($Delete_[C]$)*/#define _K2P_NEW_RANKING_UI													// 2008.09.30 / 김경표 / 랭킹 유아이 리뉴얼
	/*($Delete_[C]$)*/#define _GS_MOD_PET_BASEOPTION_EXTEND_1146									// 2008.08.13 / 김광수 / 펫기본옵션확장툴팁
	/*($Delete_[C]$)*/#define	_GS_BUG_FIX_PVP_VIEWPORT											// 2008.12.21 //김광수 // 서버뷰포트변경으로인한 무적상태유지버그

	/*($Delete_[S/C]$)*/#define	__NA_000839_20080129_PVP_OPTION										// 2008.01.29 / 조재식, 김지완 / PVP서버 옵션 기능 추가.	  ==> 2 1,2번을 해외에서 올림.
	/*($Delete_[S/C]$)*/#define	__NA_000000_20071120_FATIGUE_AC_RANKING								// 2007.11.20 / 조재식, 김광수 / AC보상 및 미션랭킹 보상 피로도 적용. ==> 1
	/*($Delete_[S/C]$)*/#define __NA_000000_20070704_FATIGUE_REWARD									// 2007. 07. 04 / 조재식, 김경표 / 피로도시 보상관련 
	/*($Delete_[S/C]$)*/#define __NA_000902_20080226_MESSAGE_RESTRICTION							// 2008.03.10 / 조재식, lammy / 파티원 이외에 메시지 제한

	/*($Delete_[S/C]$)*/#define __NA001145_20080909_AIOLT_TRIGGERS_ADD								// 2008.09.08 / 이수택, 윤영주 / 아이올트 메인미션용 트리거 추가 
	/*($Delete_[S/C]$)*/#define __NA001173_20080924_SUMMON_NONPVP_MODE_ADD							// 2008.09.24 / 이수택, 김광수 / 소환수 NONPVP모드 추가
    /*($Delete_[S/C]$)*/#define __CN_1194__ELITE_1_5_ITEM											// 2008.10.28 / 김창현, 김광수 / 엘리트 1.5차

	/*($Delete_[S/C]$)*/#define __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM_SOLAR_AUTH			// 2008.12.10 //이수택 // GM스크립트추가
	/*($Delete_[S/C]$)*/#define	__NA002410_20081006_TROUBLED_PARTS_RULE_ADD							// 2008.10.06 / 이수택 / pvp 룰 추가 (분쟁지역에서 길드 동맹관계 체크)
	/*($Delete_[S/C]$)*/#define	__NA_001142_20080806_TRIGGER_ADD									// 2008. 8. 6 / 조재식, 윤영주 / 신규 트리거 추가.
	/*($Delete_[S/C]$)*/#define __NA001139_20080922_IGNORE_RESERVEHP_STATE_ADD						// 2008.09.22 / 이수택 / 생명의 나무용 HP적립무시 상태 추가
	/*($Delete_[S/C]$)*/#define __NA002494_20081006_CHANGEUP_SAINTAID_CURE_BUG_FIX					// 2008.10.06 / 이수택 / 세인트에이드 스킬 사용시 상태 제거 안되던 버그.   
	/*($Delete_[S/C]$)*/#define __CN_1159__LUCKY_ELITE_ITEM											// 2008.09.13 / 김창현,김광수 / 럭키 엘리트 아이템 구현 ( 엘리트1.5차 )
	/*($Delete_[S/C]$)*/#define __BUG_2542_PET_INTIMACY_BUG											// 2008.10.14 / 김창현 / 친밀도 버그 수정	
	/*($Delete_[S/C]$)*/#define __BUG_2536_RECOVER_HP_BUG											// 2008.10.16 / 김창현 / HP회복 버그
	/*($Delete_[S/C]$)*/#define __BUG_2598_REVENGE_STONE_MAP_LEVEL_BUG								// 2008.10.21 / 김창현 / 복수의 돌 사용시 레벨 제한에 걸리지 않는 버그
	/*($Delete_[S/C]$)*/#define __BUG_2601_TOWER_OF_CURSE_PARTY_MEMBER_BUG							// 2008.10.21 / 김창현 / 저탑 파티원 맵 로딩시 발생하는 버그
	/*($Delete_[S/C]$)*/#define __BUG_0_TOWER_OF_CURSE_JOIN_BUG										// 2008.10.22 / 김창현 / 저탑을 마을이 아닌곳에서 입장 시 버그
	/*($Delete_[S/C]$)*/#define __BUG_2572_SPIRIT_SLUG_BUG											// 2008.10.30 / 김창현 / 스피릿슬러그 사용시 디바인엠블렘 효과 적용되는 버그
	/*($Delete_[S/C]$)*/#define __CN_1298_BASESTATE_INFO_AND_ENCHANT_ADD							// 2009.02.22 / 김창현, basestate_info, enchant 정보 추가
	/*($Delete_[S/C]$)*/#define _NA_0_20081015_EXP_LEVEL_RATIO										// 2008.10.15 / 이준석 / 경험치공식 수정. 레벨비율값 조절.
	/*($Delete_[S/C]$)*/#define _NA_0_20081024_CALC_MAGICAL_DAMAGE_RATIO_MODIFY						// 2008.10.24 / 이준석 / 마법데미지 관련 수정. 상대방 마법속성저항 감소 옵션 공식 수정!
	/*($Delete_[S/C]$)*/#define _NA_0_20081124_ADDEXP_SENDPACKET_BUG_FIX							// 2008.11.24 / 이준석 / 경험치 물약 패킷 안보내는 문제 수정
	/*($Delete_[S/C]$)*/#define __NA000000_080918_GAMEFIELD_LIST_MANAGE_SYNCHRONIZATION_FIX__		// 2008.09.17 / 이은파 / GameField::Enter, LeaveField 리스트 관리 동기화 수정
    /*($Delete_[S/C]$)*/#define __NA_2206_BUGFIX_CANT_MOVE_UNDER_DRAGON_STATE_IF_ITEM_EXPIRED       // 2008.07.04 / 김민조 / 드래곤 변신 상태에서 장착되어 있는 기간제 아이템 기간 만료시 이상 현상 수정

	//─────────────────────────────────────────
	/*($Delete_[S/C]$)*/#define __NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__					// 2008.07.07 / 이은파, 김경표 / [타락한 사원] 시스템 구현 - (이전 매크로들은 조금씩 이것으로 이전시키도록 하겠음)
	#ifdef __NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__
		/*($Delete_[S/C]$)*/#define __NA000000_080204_TASK_SSQ_REBUILDING_FOR_BASE_FRAME__			// 2008.02.04 / 이은파 / (Zone) 타락한 사원 작업을 위한 서버 기초 프레임 구축 작업 (기존 구조 개선)
		/*($Delete_[S/C]$)*/#define __NA000000_080210_TASK_SSQ_REBUILDING_FOR_BASE_FRAME_PARTY__	// 2008.02.10 / 이은파 / (Party) 타락한 사원 작업을 위한 서버 기초 프레임 구축 작업 (기존 구조 개선)
		/*($Delete_[S/C]$)*/#define __NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__			// 2008.02.04 / 이은파, 김지완 / [타락한 사원] 시스템 구현
		/*($Delete_[S/C]$)*/#define __NA001144_080828_HIGH_RESOLUTION_SECTOR_LOAD_BALANCE_POS_SYNC__// 2008.08.28 / 이은파 / Sync(position)관련 Packet Load Balancing
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SSQ_REWARD_PROCESS__						// 2008.09.04 / 이은파 / 보상,복구 등등에 관련한 처리 (대부분은 다른 매크로에 걸리겠지만...작업 세부 구분 목적)
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST__					// 2008.09.07 / 이은파 / 해당 날짜 이후 작업용
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST_STATUS__			// 2008.09.14 / 이은파 / 상태 제어 관련 작업용
		/*($Delete_[S/C]$)*/#define __NA001094_080928_TASK_SURVIVAL_SOLOING_QUEST_ETC__				// 2008.09.28 / 이은파 / 기타 세부 작업
		/*($Delete_[S/C]$)*/#define __NA001166_080922_TASK_SSQ_HUMANOID_AI_01__						// 2008.09.22 / 이은파 / Humanoid AI 구현 1차
		/*($Delete_[S/C]$)*/#define __NA001094_080904_TASK_SURVIVAL_SOLOING_QUEST_COLLECT__			// 2008.09.07 / 이은파 / 매크로 통합용
		/*($Delete_[S/C]$)*/#define __NA000000_081024_PARTY_MANAGEMENT_EASIABLITY				// 2008.10.24 / 이은파 / 파티 관련 문제점 발생 문제(관리상) 해결
		/*($Delete_[S/C]$)*/#define __NA001205_081104_MONSTER_SIEGE_EVENT_IMPL__				// 2008.11.04 / 이은파 / 몬스터 수성 이벤트
		/*($Delete_[S/C]$)*/#define __NA001094_081104_LOGIC_CHANGES_FOR_0817VER__				// 2008.11.04 / 이은파 / 0817 버전에서의 문제점 또는 요청에 대한 로직 변경 작업
		/*($Delete_[S/C]$)*/#define __NA000000_081110_LIVE_SERVICE_TEST_CLINET_SUPPORT__		// 2008.11.10 / 이은파 / 라이브에서 특정 유저에 한한 테스트 모드 추가(설정 로딩 문제로 임시 하드코딩 값 추가)
		/*($Delete_[S/C]$)*/#define __NA000000_081124_DEAD_AND_LEAVE_PACKET_SYNCHRONIZATION__	// 2008.11.24 / 이은파, 김경표 / 0.8.1.7 이후 패킷 동기화 순서 역전 문제가 발생할 수 있으며, 그중 DEAD, LEAVE의 역전 문제 수정
        /*($Delete_[S/C]$)*/#define __NA000000_081223_LOGIC_CHANGES_FOR_0817VER_SYNC__  // 2008.12.23 / 이은파 / 0817 버전 이후 동기화 관련 문제점들 검증 및 수정

		/*($Delete_[S/C]$)*/#define __NA001187_081015_SSQ_OBSERVER_MODE__							// 2008.10.15 / 이은파, 김경표 / 타락한 사원 관전모드 개발
		/*($Delete_[S/C]$)*/#define __NA001189_081015_SSQ_TICKET_INFO_CTRL__						// 2008.10.15 / 이은파, 김경표 / 타락한 사원 입장권 입장시간 표시
		/*($Delete_[S/C]$)*/#define __NA00XXXX_080922_TASK_SSQ_NPC_SELF_DESTRUCTION__				// 2008.09.22 / 이은파, 김경표 / 몬스터 자폭 처리 관련
		/*($Delete_[S/C]$)*/#define __NA001144_080806_HIGH_RESOLUTION_SECTOR__						// 2008.08.06 / 이은파, 김경표 / Sector Rebuilding + High Resolution Sector를 통한 클라이언트 랙 감소 작업
		/*($Delete_[S/C]$)*/#define __NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__		// 2008.02.15 / 이은파, 김경표 / 타락한 사원 작업을 위한 C/S 공통 모듈 작업
        /*($Delete_[S/C]$)*/#define __NA000000_090223_VARIABLE_SIZE_GRID_SYNC_CORRECTION__      // 2009.02.23 / 이은파,윤영주 / 가변크기 뷰처리로 인해 동기화에 문제가 있는 부분 수정
	#endif
	//─────────────────────────────────────────

	
	

//	#define _NA_0_20080612_WORLDSERVER_REFACTORING		 // 노현철/리팩토링 월드서버 컴파일

	#ifdef _KOREA
		#define			_NA_20081121_ABUMAN_NIJOT_EVENT					// 2008. 11. 21 / 조재식, 김광수 / 미션완료시 미원진행원이 모두 같은 길드원이면 경험치 더주는 이벤트.				
	#elif defined(_CHINA) 
		/*($Delete_[S/C]$)*/#define			__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM_SOLAR_AUTH	// 2008.12.10 //이수택 // GM스크립트추가
		/*($Delete_[S/C]$)*/#undef			_NA_0_20081015_EXP_LEVEL_RATIO								// (중국제외) 2008.10.15 / 이준석 / 경험치공식 수정. 레벨비율값 조절.
		#define			__CN_0_SSQ_CAHO_EXCEPTION									// 2008.12.22 / 김창현 / 타락한사원 카오도 입장
		#define			_GS_ADD_PWD_X_BTN_1245										// 2008.12.22 / 김광수 / 중국소문자x비번추가				
		/*($Delete_[S/C]$)*/#define			__NA_000000_20081202_ADD_EXP_ACTION_2						// 2008.12.02 / 조재식 / 경험치 주기 ==> 파티시 해당 맵에 있는 모든 유저에게 경험치 주기로 변경.
		#define			__CN_0_20090421_EXP_BENEFIT_UP								// 2009.04.21 / 이수택  // 중국 보너스 경험치 배율 상승 400% -> 700% (chd
	#endif

#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.8.1.6 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 816
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(2)		// 하위 버전 번호
	#endif	
	//////////////////////////////////////////////////////////////////////////

	/*($Delete_[S/C]$)*/#define __NA_963_LOG_LIB_VERSION_REFACTORING					// 2008.04.29 / 김창현 / 로그 라이브러리 개선
	/*($Delete_[S/C]$)*/#define __NA_0_ITEM_STREAM_STRUCT_SEPARATION					// 2008.05.30 / 김창현 / 아이템 스트림 구조체 파일 분리
	/*($Delete_[S/C]$)*/#define __NA_1127_DELETE_BUFF_ICON								// 2008.07.21 / 김창현 / 버프 아이콘 삭제
    /*($Delete_[S/C]$)*/#define __BUG_FIX_CAN_NOT_GET_POINT_WHEN_DEAD					// 2008.09.04 / 김창현 / 캐릭터 사망시에 랭킹포인트 얻지 못하도록 수정
	

	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__			// 2008.06.17 / 이은파 / 성능 분석 결과에 기반한 병목 부분 코드 개선 작업 (4th macro)
	/*($Delete_[S/C]$)*/#define __NA001086_080616_BUFF_EVENT_UI_AND_MULTI_SUPPORT_FUNCTION__		// 2008.06.16 / 이은파, 윤영주 / 버프 이벤트 UI 개선 및 복수 지원 가능 구조로 변경
	/*($Delete_[S/C]$)*/#define __NA000000_080707_PLAYER_ENTER_NAK_RESULT_CONTROL__					// 2008.07.07 / 이은파, 윤영주 / 플레이어 입장 실패시 에러 구분 처리
	/*($Delete_[S/C]$)*/#define __NA000000_080903_REGENTIME_CALC_PROBLEM_FIX__						// 2008.09.03 / 이은파 / 맵툴 입력 수치와 관련한 리젠 시간 계산 타입 오버플로 문제 수정
	/*($Delete_[S/C]$)*/#define __NA_001128_20080717_ADD_GM_CMD							// 2008.07.17 / 이수택 / 하임 초기화, 케릭터 사망, 스킬 쿨타임 초기화, 스킬레벨 초기화
	/*($Delete_[S/C]$)*/#define __NA_000921_20080618_TROUBLED_PARTS_REFACTORING			// 2008.06.18 / 이수택 / 분쟁 지역 구조변경
	/*($Delete_[S/C]$)*/#define __NA000000_080818_LEVEL_DIFFERENCE_NO_MASTER_BUG		// 2008.08.18 / 이수택 / 파티장이 없는 파티시 인스던전 입장제한에 걸리지 않는 버그. 
	/*($Delete_[S/C]$)*/#define	__NA_002260_20080821_BORADORY_BUG_FIX					// 2008.08.21 / 이수택 / 카오상태가 되어도 기존 보라돌이 시간동안은 보라돌이가 유지되는 버그 수정.
	/*($Delete_[S/C]$)*/#define	__NA_0_20080905_BORADORY_BUG_FIX						// 2008.09.05 / 이수택 / 분쟁지역에서 보라돌이가 되는 현상.			
	/*($Delete_[S/C]$)*/#define __NA001117_SUMMON_MONSTER_ITEM							// 2008.07.10 / 최정록         / 몬스터소환 캐시아이템 신규추가
	/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM							// 2008.07.17 / 최정록, 윤영주 / 아이템 스텟 체인지 시스템

    /*($Delete_[S/C]$)*/#define __NA_1119_CHANGE_ETHER_EXPEND_STYLE						// 2008.08.12 / 김창현, 김광수 / 에테르 탄환 소비 방식 단발에서 연발로 변환
    /*($Delete_[S/C]$)*/#define __NA_1339_ENCHANT_RENEWAL_EX							// 2009.05.12 / 김창현, 이태준 / 인첸트 실패시 파괴 - 수택씨 작업이후 추가 작업
	/*($Delete_[S/C]$)*/#define __NA_001013_20080428_ENCHANT_RENEWAL					// 2008.04.28 / 이수택, 김민조 / 인챈트 시스템 리뉴얼 
	/*($Delete_[S/C]$)*/#define __NA_001092_20080717_ADD_SKILL_ITEM						// 2008.07.17 / 이수택, 전보환 / 폭죽, 꽃가루, 횃불, 폭 아이템

	#define _YJ_PK_OPTION_001120								//08.08.05 //윤영주 //PK 강제공격 옵션 처리
	/*($Delete_[C]$)*/#define _YJ_STAT_CHANGE_SYSTEM_001124						        //08.07.28 //윤영주 //스탯 체인지 시스템
	/*($Delete_[C]$)*/#define _YJ_ADD_DOF_RANGE_001101							        //08.07.09 //윤영주 //엔진 적용 필요
	/*($Delete_[C]$)*/#define _YJ_TEXT_SHADOW_EFFECT_001905						//08.07.01 //윤영주 //텍스트 음영효과 추가
	/*($Delete_[C]$)*/#define _YJ_ADD_BUFF_EVENT_UI_001086						//08.06,30 //윤영주 //버프이벤트 ui 추가

	/*($Delete_[S/C]$)*/#define __NA001084_PET_IMAGE_DRAW_IN_CHARACTER_SELECT_SCREEN	//            / 최정록, 김광수 / 캐릭터 선택창에서 펫 이미지 출력
	/*($Delete_[S/C]$)*/#define __SUMMONED_SPEED_CHANGE									// 2008.09.01 / 최정록, 김광수 / 서몬 몬스터 스피드 변경

	/*($Delete_[S/C]$)*/#define	__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	// 2007.11.23 / 조재식 / 새로운 스크립트 파싱 시스템.
	/*($Delete_[S/C]$)*/#ifdef __NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	
		/*($Delete_[C]$)*/#define _GS_MOD_NEW_SCRIPT_PARSING_SYSTEM	
		/*($Delete_[S/C]$)*/#define _YJ_NEW_SCRIPT_PARSING_SYSTEM
		/*($Delete_[S/C]$)*/#define _KIKI_20080711_NEW_SCRIPT

		/*($Delete_[S/C]$)*/#define __NEW_SCRIPT_SYSTEM_BY_AEORAGIE_PART // 2008.07.14 / 최정록 / 신규스킬파서
		/*($Delete_[S/C]$)*/#define __NA_000000_20080714_NEW_SCRIPT_PARSING_SYSTEM_APPLY    // 
		/*($Delete_[C]$)*/#define __NA_0_20080716_PARSER_RENEWAL_BY_MJKIM
	/*($Delete_[S/C]$)*/#endif //__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM	

	/*($Delete_[S/C]$)*/#define __NA_001093_20080623_SERVER_DISCONNECT_			    // 2008. 06. 23 / 조재식 / 서버간의 연결이 끊어 져을 때 처리.
	/*($Delete_[S/C]$)*/#define	__000000_20080811_BUG_FIX_VIEW_JOIN_TOP_TIME		// 2008. 8. 11 / 조재식, 김창현 / '저주받은 탑의 일반 열쇠에 시간 표시' ==> null 체크 버그 수정.

	/*($Delete_[C]$)*/#define _GS_ADD_TARGETING_EFFECT_1125							// 2008.08.13 / 김광수 / 몬스터 타겟팅이펙트

	/*($Delete_[C]$)*/#define _GS_ADD_SINGLEMODE_MONSTER_1016							// 2008.08.13 / 김광수 / 싱글모드시 몬스터이동및스킬보기

	/*($Delete_[S/C]$)*/#define __NA_0_20080717_DBUNIT_ADD_SERVERTYPE						// 2008.07.17 / 이준석 / DBUnit에 m_ServerType 멤버변수 추가
	/*($Delete_[S/C]$)*/#define __NA_0_20080825_MODIFY_OPTION_DEFENCE_POWER_CALC			// 2008.08.25 / 이준석 / 옵션 공격력, 방어력 비율 적용 공식 변경


    #define __NA_001062_20080623_STRENGTH_PENALTY					// 2008.06.23 / 이호영, 김광수 / 내구도 감소에 따른 패널티 부여. 

	//국가버전별
	#ifdef _CHINA
		//#undef __NA_001062_20080623_STRENGTH_PENALTY				//중국은 내구도 제거
	#elif defined(_JAPAN)
		#undef _YJ_PK_OPTION_001120									//일본은 PK강제옵션 제거
	#endif

#endif

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.8.1.5 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 815
	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(3)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(1)		// 하위 버전 번호
	#endif	
	//////////////////////////////////////////////////////////////////////////

    /*($Delete_[x/C]$)*/#define __NA_001044_ADD_CUSTOM_UI								// 2008.05.28 / lammy / 케릭터 생성시 미리 입어보기 기능 추가	

	/*($Delete_[C]$)*/#define _K2P_CHARACTER_ROTATE_000986							// 2008.05.28 / lammy / 케릭터 생성시 회전 기능 추가
	/*($Delete_[C]$)*/#define _K2P_ADD_INVITE_BUTTON_001018							// 2008.05.28 / lammy / 초대 버튼 추가
    /*($Delete_[C]$)*/#define _GS_MOD_CHATMSG_MAINTAIN_1058							//[1058] 2008.05.30 / GS / 맵이동시 채팅내용 유지 
    /*($Delete_[C]$)*/#define __NA_001003_PC_MONSTER									// 2008.05.07 / kjws78 / pc형 몬스터 추가
    /*($Delete_[C]$)*/#define __NA_0_20080507_CHANGUP_PC_SKLLL_ADD					// 2008.05.07	/ 전보환 / 체인지업에 따른 PC 스킬 추가
    /*($Delete_[C]$)*/#define __NA_0_2008_06_03_PLAYER_INVALID_EQUIPMENT_CHECK		// 2008.06.04	/ 전보환 / 자신과 다른 클레스의 장비를 착용시 기본 방어구로 셋팅함
    /*($Delete_[C]$)*/#define __NA_1061_20080609_SHOW_MY_ID_RENDERINFO							//08.06.09   / 전보환 / 내 아이디 보이기 기능추가
    /*($Delete_[C]$)*/#define _YJ_MOD_QUEST_NPC_NAME_COLOR_1938									// 08.06.18 / 윤영주 / 퀘스트 NPC 네임 컬러 변경
    
    


    /*($Delete_[S/C]$)*/#define __NA_000962_ADD_GUILD_ERROR								// 2008.05.28 / 노현철, 김경표 / 같은 길드 이름으로 생성 에러값 추가 
	/*($Delete_[S/C]$)*/#define	__NA_001012_20080506_RESERVE_HP_SYSTEM					// 2008.05.06 / 조재식, 김민조 / 기본 HP시스템을 적립화로 변경하여 긴장감 및 물약의 효용성을 높인다.
    /*($Delete_[S/C]$)*/#define _BUG_FIX_LOTTERY_SYSTEM									// 2008.09.11 / 조재식, 김광수 / 복권 시스템 복권 중복되는 버그 수정.
    /*($Delete_[S/C]$)*/#define __NA_000946_20080415_LOTTERY_SYSTEM						// 2008.05.28 / 최정록, 김광수         / 복권시스템					[NO:0946]
    /*($Delete_[S/C]$)*/#define __NA_001068_ACTION_DEAD_PLAYER							// 2008.06.19 / 최정록, 김경표         / 사망한 플레이어 클릭가능	[NO:1068] 
    /*($Delete_[S/C]$)*/#define __NA_000994_CHANGUP_NEW_SKILL							// 2008.05.28 / 최정록, 전보환         / 신규스킬					[NO:0994]

    /*($Delete_[S/C]$)*/#ifdef __NA_000994_CHANGUP_NEW_SKILL                                // 
        /*($Delete_[S/C]$)*/#define CHANGUP_IMMUNITY_DAMAGE_STATUS							// 무적, 마법및 물리 데미지 면역, 공격가능
        /*($Delete_[S/C]$)*/#define CHANGUP_IMMUNITY_DAMAGE_STATUS_BUGFIX					// 상태이상에 걸리던 버그 수정
        /*($Delete_[S/C]$)*/#define CHANGUP_INTENSIFY_SKILLDAMAGE_STATUS					// 강화, 스킬 데미지 300%상승
        /*($Delete_[S/C]$)*/#define CHANGUP_SUMMONMASTERY_SKILL								// 서먼 마스터리
        /*($Delete_[S/C]$)*/#define CHANGUP_IMPOSSIBLE_ATTCAK_STATUS						// 공격불가, 공격할 수 없다.
        /*($Delete_[S/C]$)*/#define CHANGUP_PURGE_SKILL										// 대상을 랜덤한 지역으로 추방한다.
        /*($Delete_[S/C]$)*/#define CHANGEUP_SAINTAID										// 상태이상 해제
        /*($Delete_[S/C]$)*/#define CHANGUP_RECKLESS_STATUS									// 폭주, 소환수의 데미지 대폭 상승
    /*($Delete_[S/C]$)*/#endif


	/*($Delete_[S/C]$)*/#define	__NA_000000_20080522_DROPRATIO_BUGFIX					// 2008.05.22 / 조재식 / 아이템 Drop 의 자료형이 WORD형이여서 생기는 버그 수정.
	/*($Delete_[S/C]$)*/#define	__NA_001042_NEW_CHARACTER_ITEM							// 2008.08.26 / 조재식 / 신규 캐릭터 생성시 뮤료로 캐시아이템을 지급.


    // (WARNING) 하기 working macro는 유용한 방식을 제공할 수 있으므로 정리할 예정. 지우지 말 것.
	/*($Delete_[S/C]$)*/#define	__NA_001070_20080610_DBPROXY_LOG						// 2008.06.09 / 조재식 / DBProxy로그 정리


    // 지우지 말 것////////////////////////////////
    #define _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020					// 2008.05.28 / lammy / 케릭터 선택창에 서버 이동버튼 추가
    #define __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT  // (중국 제외 컨텐츠) 2008.04.28 / 김창현 / 리미티드 아이템 수리 횟수 수정 (무제한으로)
    /////////////////////////////////////////////


	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080616_HP_ZONEMOVE						// 2008.06.16 / 조재식 / 존 이동시 HP 버그( HP확장 아이템을 가지고 있어도 이동후 적용안되는 버그 )
	/*($Delete_[S/C]$)*/#define	__NA_001081_20080612_TUTORIAL_CHANGE					// 2008.06.12 / 조재식, 김광수 / 튜토리얼 입장방식 변경.
	/*($Delete_[S/C]$)*/#define __NA_VENDOR_BUGFIX_										// 2008.09.23 / 조재식 / 교환 널 포인트 참조 버그 수정.
	/*($Delete_[S/C]$)*/#define _BUG_FIX_20081007_WOPS2463_JUMP_DAMAGE					// 2008.10.07 / 조재식 /점프 했을때 데미지 줄지 않는거처럼 보이는 버그 수정.
	/*($Delete_[S/C]$)*/#define __NA_001039_20080509_GM_COMMAND_MODIFY					// 2008.05.09 / 이수택, 김광수 / GM 명령어 수정, 추가	
	/*($Delete_[S/C]$)*/#define __NA_0_20080509_GM_COMMAND_MODIFY_DEV					// 2008.05.09 / 이수택 / 개발 테스트용 GM명령어 저장, 실행, gm스크립트실행
	/*($Delete_[S/C]$)*/#define __NA_001043_20080517_HAMMER_OF_ALCHEMIST_MODIFY			// 2008.05.17 / 이수택 / 명공의 망치 기능 변경
	/*($Delete_[S/C]$)*/#define __NA_1921_20080610_BUG_FIX_HAMMER_OF_ALCHEMIST_MODIFY	// 2008.06.10 / 이수택 / 명공의 망치 수리시 현재 내구도 == 최대 내구도 일시 수리 안되는 버그 
	/*($Delete_[S/C]$)*/#define __NA_001043_20080517_WORLD_SHOUT_CONSUME_MODIFY			// 2008.05.17 / 이수택 / 해방군의 포고문 기능 변경
	/*($Delete_[S/C]$)*/#define __NA_0_20080603_REMOVE_QUESTSCRIPT_ASSERT				// 2007.06.03 / 이수택 / 퀘스트 파서에서 load 함수의 assert, Validating 함수로 빼내기
	/*($Delete_[S/C]$)*/#define __NA_001892_20080618_WORLD_SHOUT_CONSUME_MODIFY_BUGFIX  // 2008.06.18 / 이수택 / 월드 외치기 pc방에서 안되는 버그 수정.
	/*($Delete_[S/C]$)*/#define __NA_0_20080610_CHECK_ATTACK_PROPENSITY_MODYFY			// 2008.06.10 / 이수택, 김지완 / 지역별(분쟁지역등) 공격성향(일반.분쟁.PK등) 체크 변경
	/*($Delete_[S/C]$)*/#define __NA_0_20080627_CHECK_ATTACK_PROPENSITY_MODYFY_INSERTNULL // 2008.06.27 / 이수택 / CHECK_ATTACK_PROPENSITY_MODYFY 작업시 빠진 널체크 추가.
	/*($Delete_[S/C]$)*/#define __NA_2060_20080626_BORADORI_BUG_FIX						// 2008.06.26 / 이수택 / 경계지역에서 버프스킬 시전시 보라돌이 되던 버그.
	/*($Delete_[S/C]$)*/#define __NA_2039_20080630_CHAO_SATISFY_BUGFIX					// 2008.06.30 / 이수택 / 프리카오 유저의 퀘스트 수락 가능하게(스크립트제어).[스크립트 변경시 동시 추가 되어야함]
	/*($Delete_[S/C]$)*/#define __NA_29793_20080701_CHAO_IS_ENEMY						// 2008.07.01 / 이수택 / 카오유저 자동으로 강제공격되지 않는 버그.
	/*($Delete_[S/C]$)*/#define __NA_0_20080724_BUGFIX_TILE_CHECK						// 2008.07.24 / 이수택 / 타일체크시 멤버함수내 스태틱변수 사용 버그 수정.
	/*($Delete_[S/C]$)*/#define __NA_001074_ETHER_EMISSION_DEVICE_EXPAND				//            / 최정록         / 에테르 아이템 옵션
	
	/*($Delete_[S/C]$)*/#define __NA_1047_EXP_POTION												// 2008.06.02 / 김창현 / 경험치 물약
	/*($Delete_[S/C]$)*/#define __NA_1075_FIELD_BATTLE_SERVER_LOG_REGULATION						// 2008.06.09 / 김창현 / 필드 배틀 서버 로그 정리
	/*($Delete_[S/C]$)*/#define __NA000995_080402_SKILL_PROC_UPDATE_V01__							// 2008.04.02 / 이은파, 김지완 / Skill관련 패킷 처리 처리 개선을 위한 기초작업 Version01
	/*($Delete_[S/C]$)*/#define __NA001021_080421_PACKET_COMPRESS_V02__								// 2008.04.21 / 이은파, 김지완 / 일반 압축 알고리즘 적용을 위한 패킷 분석 및 적용
	/*($Delete_[S/C]$)*/#define __NA001021_080619_PACKET_COMPRESS_V03__								// 2008.06.19 / 이은파 / PacketControl 3rd Server<->Server (특정 패킷에 대해서만 작업)
	/*($Delete_[S/C]$)*/#define __NA000000_080228_TARGET_FIND_CORRUPT_FLOW_FIX__					// 2008.02.28 / 이은파 / GameField::SearchTarget 로직, 임시 객체의 포인터를 이용한 위험천만 코드 수정 - 다른 연관된 문제가 발생하는 지의 여부 파악 필요
	/*($Delete_[S/C]$)*/#define __NA000000_GRID_REBUIDING_FOR_READABLE_CODE__						// 2008.04.24 / 이은파 / Player_Grid 가독성 높이기 작업
	/*($Delete_[S/C]$)*/#define __NA844896_080219_TASK_SSQ_AND_100LV_QUEST_NPC_KILL_OPTIMIZER__		// 2008.02.19 / 이은파 / [844,896] NPC Kill Event처리시 막대한 루프 제거용, 이벤트가 할당된 몬스터만 처리할 수 있는 구조 _구 100단 퀘스트 기능에 적용되어 있던 내용 이전 및 추가 작업
	/*($Delete_[S/C]$)*/#define __NA844896_080219_TASK_SSQ_AND_100LV_QUEST_INSTANCE_ZONE__			// 2008.02.19 / 이은파 / [844,896] InstanceDungeon 분할 처리 _구 100단 퀘스트 기능에 적용되어 있던 내용 이전 및 변경 작업
	/*($Delete_[S/C]$)*/#define __NA001003_080527_PLAYER_ANIMATION_TIME_CONTROL__					// 2008.05.27 / 이은파, 김지완 / 플레이어 애니메이션 처리시 위치동기화 맞지 않는 문제 해결을 위한 작업
	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO03__			// 2008.06.XX / 이은파 / 성능 분석 결과에 기반한 병목 부분 코드 개선 작업
	/*($Delete_[S/C]$)*/#define __NA000000_0806XX_SOLARHASH_PROBLEM_CORRECTION_V01__				// 2008.06.19 / 이은파 / SolarHashtable 무결성 검사(DEBUG) & 관련 문제 수정
	/*($Delete_[S/C]$)*/#define __NA000000_080626_TUTORIAL_MISSION_LEAVE_CASE_CORRECTION__			// 2008.06.26 / 이은파 / Agent에 존재하는 튜토리얼 상태가 미사용(언제부터...?)되면서 꼬이는 문제 수정
	/*($Delete_[S/C]$)*/#define __NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL__			// 2008.06.09 / 이은파 / 어뷰징 유저 방지책_20080609 : 정상,비정상 적인 상황에 대한 상세 분석 및 필요한 경우 수정 및 개선
	/*($Delete_[S/C]$)*/#define __NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL_V2__		// 2008.07.01 / 이은파 / HackShield 관련 디텍션 신뢰도 높이기 작업 (안정화 되면 이전 매크로와 합칠 것)
	/*($Delete_[S/C]$)*/#define __NA000000_081218_GAMEFIELD_LEAVE_FIELD_USAGE_CORRECTION__			// 2008.12.18 / 이은파 / GameField::LeaveField시 실 객체 해제 안하는 문제 수정(메모리 누수)
	/*($Delete_[S/C]$)*/#define __NA_0_20080219_DETECT_ABUSING_USER_REFACTORING						// 2008.02.19 / 이준석 / 가상 클라이언트 방지책. 어뷰징 유저 처리 리팩토링
	/*($Delete_[S/C]$)*/#define __NA_0_20080219_DETECT_ABUSING_USER_POWERFUL						// 2008.02.19 / 이준석 / 가상 클라이언트 방지책. 어뷰징 유저 처리 추가사항
	/*($Delete_[S/C]$)*/#define __NA_0_20080430_SKILL_REFACTORING									// 2008.04.30 / 이준석 / 스킬 매니저 리팩토링
	/*($Delete_[S/C]$)*/#define __NA_891_20080602_CHANGE_EXP_RATIO2									// 2008.05.02 / 이준석 / 일본 경험치 배율 조정 공식을 한국에도 적용
	/*($Delete_[S/C]$)*/#define __NA_28934_20080609_CANBE_ATTACKED_CHECKMISS_BUG_FIX				// 2008.06.09 / 이준석 / 스킬 이펙트 처리에서 CanbeAttacked() 체크 추가. 강철상태동안 데미지 들어오는 버그 수정
	/*($Delete_[S/C]$)*/#define __NA_0_20080613_VIEW_PORT_PERFORMANCE_IMPROVEMENT					// 2008.06.13 / 이준석 / 뷰포트 성능개선
	/*($Delete_[S/C]$)*/#define __NA_0_20080616_AGENT_CHARSELECT_INIT_BUG_FIX						// 2008.06.16 / 이준석 / 캐릭터 선택화면으로 돌아올 때 초기화 안된거 수정. RemoveUserForCharGuid 로그 안남게 수정.
	/*($Delete_[S/C]$)*/#define __NA_0_20080617_CLASS_STARTING_POINT_BUG_FIX						// 2008.06.17 / 이준석 / 클래스별 스타팅 포인트 기능 제거. 예전에 기획되었지만 현재 쓰이지 않음.
	/*($Delete_[S/C]$)*/#define __NA_0_20080619_GET_VALID_SPECIAL_AREA								// 2008.06.19 / 이준석 / WzSpecialArea 구할 때 이동가능한 유효한 타일을 선택하는 함수 추가
	/*($Delete_[S/C]$)*/#define _YJ_001063_001064_GUILD_RESULT_CODE_ADD								// 2008.06.09 / 노현철,윤영주 /길드관련 리절트 코드 추가
	/*($Delete_[S/C]$)*/#define _NA_0_20080530_ABUSING_USER_VERIFY_BY_WORLDSERVER					// 2008.5.30 / 노현철 / 월드 서버 가상 클라이언트 구별패킷
	/*($Delete_[S/C]$)*/#define		__NA_BUG_181415_20080820_GUILD_BUG_FIX							// 2008.8.20 / 조재식 / 마스터가 길드원을 예비병으로 변경할 수 있는 버그 수정.

	/*($Delete_[S/C]$)*/#define __NA000844_080324_100LEVEL_CHAGNEUP_REBUILDING__					// 2008.03.24 / 이은파, 전보환 / 100레벨 체인지업
	/*($Delete_[S/C]$)*/#define __NA001048_080514_APPEND_ABILITY_KNOCKBACK2__						// 2008.05.14 / 이은파, 전보환 / 넉백2 어빌리티 추가 (은파:Ability관련 Flow 검증 및 오류 검출도 겸할 예정)
	/*($Delete_[S/C]$)*/#define __NA001051_080526_EVENT_EFFECT_RENDER_BY_SCRIPT_2ndPROCESS__		// 2008.05.26 / 이은파, 윤영주 / [이벤트]2주년 이벤트 이펙트 출력 작업
	/*($Delete_[S/C]$)*/#define __NA001087_080618_APPEND_CASHITEM_PRESENT_PROTECTION_FUNTION__		// 2008.06.18 / 이은파, 김광수 / Cash Shop 의 선물하기 기능 불가능 형태 지원
	/*($Delete_[S/C]$)*/#define __NA_001053_20080522_ATTR_RATIO_TYPE								// 2008.05.22 / 이준석, 김민조 / 비율타입(5) 옵션 정리
	/*($Delete_[S/C]$)*/#define _NA_0_20080530_GUILD_WAREHOUSE_LIMIT_ITEM_LOG						// 2008.5.30 / 노현철, 윤영주 / 길드 창고 로그 일반 아이템과 리미티드 아이템 구별


	#ifdef _CHINA
		#define _GS_ADD_QUEST_SCROLL_1143										// 08.09.06 / 김광수 / 중국NPC퀘스트창 스크롤바추가
        //-------------------------------------------------------
        #undef __NA_1024_MODIFY_LIMITED_ITEM_REPAIR_COUNT   // 2008.04.28 / 김창현 / 리미티드 아이템 수리 횟수 수정 (무제한으로)
        //-------------------------------------------------------
		/*($Delete_[S/C]$)*/#define __BUG_2598_REVENGE_STONE_MAP_LEVEL_BUG							// 2008.10.21 / 김창현 / 복수의 돌 사용시 레벨 제한에 걸리지 않는 버그
		/*($Delete_[S/C]$)*/#define __BUG_2601_TOWER_OF_CURSE_PARTY_MEMBER_BUG						// 2008.10.21 / 김창현 / 저탑 파티원 맵 로딩시 발생하는 버그
		/*($Delete_[S/C]$)*/#define __BUG_0_TOWER_OF_CURSE_JOIN_BUG									// 2008.10.22 / 김창현 / 저탑을 마을이 아닌곳에서 입장 시 버그		
		/*($Delete_[S/C]$)*/#define __CN_1159__LUCKY_ELITE_ITEM										// 2008.09.13 / 김창현,김광수 / 럭키 엘리트 아이템 구현 ( 엘리트1.5차 )		

		/*($Delete_[S/C]$)*/#define	__NA_1170_CHARGE_INVEN_REGISTER_QUICK_SLOT						// 2008.09.23 / 김창현 / 유료 인벤의 슬롯도 퀵슬롯 등록하도록...
		#define __NA_0_STAT_INIT_WHEN_100LEVEL_CHANGEUP							// 2008.09.23 / 김창현,김광수 / 100레벨 체인지업 할때 스탯 초기화		
		#define _GS_ADD_PWD_X_BTN_1143                                          // 08.08.13 / 김광수 / 중국창고비번에x버튼추가

		/*($Delete_[S/C]$)*/#define __CN_1194__ELITE_1_5_ITEM										// 2008.10.28 / 김창현, 김광수 / 엘리트 1.5차
		/*($Delete_[S/C]$)*/#define __BUG_2572_SPIRIT_SLUG_BUG										// 2008.10.30 / 김창현 / 스피릿슬러그 사용시 디바인엠블렘 효과 적용되는 버그
        // (NOTE) (100130) 일전에 기획에 문의시 모든 스크립트가 통일된 상태였다. (값만 틀리고...) 통일시키도록 하자.
		#define __CN_PET_OPTION_APPLY_FLAG										// 2008.08.24 / 김창현,김광수 / 펫 옵션 플레그로 선택 적용(스크립트에서...)
		/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM									// 2008.07.17 / 최정록, 윤영주 / 아이템 스텟 체인지 시스템
		//!여기까지 라이브		
		/*($Delete_[S/C]$)*/#define _NA_0_20081024_CALC_MAGICAL_DAMAGE_RATIO_MODIFY					// 2008.10.24 / 이준석 / 마법데미지 관련 수정. 상대방 마법속성저항 감소 옵션 공식 수정!

	#elif defined(_KOREA) 

	#elif defined(_JAPAN)
		/*($Delete_[S/C]$)*/#define __NA001124_ITEM_CHANGE_SYSTEM									// 2008.07.17 / 최정록, 윤영주 / 아이템 스텟 체인지 시스템
		#define _K2P_ADD_WISPER_WINDOW
		#define _K2P_USE_CHATWINDOW_USING_VENDORLIST
		#define _K2P_HERO_POSITION
		/*($Delete_[C]$*/#define _K2P_CHECK_USE_ITEM_BY_POPUP
        // (NOTE) (100130) 일전에 기획에 문의시 모든 스크립트가 통일된 상태였다. (값만 틀리고...) 통일시키도록 하자.
		#define __CN_PET_OPTION_APPLY_FLAG										// 2008.08.24 / 김창현 / 펫 옵션 플레그로 선택 적용(스크립트에서...)
		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW								// 2008.03.28 / 이준석 / 방리스트 조건체크없이 모두 보여주기. 일단 국내만 적용.
		//#define __NETWORK_FIX_20081119													
	#endif

#endif
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.8.1.4 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 814

	/////////////////////       Protocol Version       //////////////////////
	#ifndef C2S_HIGH_VERSION_NO		
		#define C2S_HIGH_VERSION_NO		(3)		// 상위 버전 번호
	#endif	
	#ifndef C2S_MIDDLE_VERSION_NO
		#define C2S_MIDDLE_VERSION_NO	(2)		// 중간 버전 번호
	#endif	
	#ifndef C2S_LOW_VERSION_NO
		#define C2S_LOW_VERSION_NO		(7)		// 하위 버전 번호
	#endif	
	//////////////////////////////////////////////////////////////////////////

    /*($Delete_[C]$)*/#define _GS_MOD_PERSONAL_VENDER_00877								// 2008.03.24 / 김광수 / 개인상점게시판 스크롤바제거
    /*($Delete_[C]$)*/#define _GS_MOD_ITEM_NAMMING_COLOR_000882							// 2008.03.24 / 김광수 / 아이템네이밍색 & 리미티드 드랍시색수정
    /*($Delete_[C]$)*/#define _GS_MOD_DLG_SCROLL2											// 2008.04.02 / 김광수 / 랭킹&미션관리자,퀘스트창,조합창 스크롤수정
    /*($Delete_[C]$)*/#define _GS_MOD_DLG_EVENTRECT_928									// 2008.04.10 / 김광수 / 미션 help,시스템채팅창 피킹 영역수정
    /*($Delete_[C]$)*/#define _GS_BUGFIX_GUILD_NOTICE_1470								// 2008.04.23 / 김광수 / 인터페이스엔진 에디트컨트롤 줄제한추가
    /*($Delete_[C]$)*/#define __NA_000966_20080312_ADD_PARAM_FOR_ADDITIONAL_SOUND			// 2008.03.24 / 김민조 / 아이템 사운드 추가에 따른 스크립트 변경 및 파라미터 추가
    /*($Delete_[C]$)*/#define __NA_0_20080318_PROCESS_AFTER_RENDERING						// 2008.03.24 / 김민조 / 렌더링 후 처리 함수 추가
    /*($Delete_[C]$)*/#define __KR_000978_20080321_CHANGE_HUNTING_JOIN_LEVEL_OFFSET		// 2008.03.24 / 김민조 / 경쟁 헌팅 입장 레벨 제한 변경
    /*($Delete_[C]$)*/#define __NA_0_20080311_BUGFIX_TARGET_INFO_FOR_TREE_OF_LIFE			// 2008.03.11 / 김민조 / 생명, 정신의 나무 타겟 정보 버그(%d로 표시) 수정

    /*($Delete_[x/C]$)*/#define __NA_676_PK_SYSTEM_IMPROVEMENT								// 2008.01.29 / 김창현, 김지완 / PK 룰 개선

    /*($Delete_[S/C]$)*/#define __MOD_CRYSTALIZE_MAX_CREATE_ITEM_NUM						// 2007.12.29 / 전보환,김창현 / 아이템 결정화 최대 갯수 수정.    
    /*($Delete_[S/C]$)*/#define __NA_000968_ETHER_EMISSION_DEVICE							// 2008.03.14 / 김창현, 전보환 / 에테르 방출장치
    /*($Delete_[S/C]$)*/#define __NA_780_PREVENTION_PARTY_RETURN_ABUSE						// 2008.02.05 / 김창현, 김광수 / 재접속 기능 악용 방지
    /*($Delete_[S/C]$)*/#define __NA_0_PET_SYSTEM											// 2008.02.14 / 김창현, 김광수 / 펫 시스템 - 발렌타인데이 우울하다    
    /*($Delete_[S/C]$)*/#define __NA_1006_POINT_RANKING_REGISTER_LV							// 2008.04.17 / 김창현, 김광수 / 포인트 랭킹 등록 레벨 추가	

    /*($Delete_[S/C]$)*/#define __NA_0_CHARGE_ITEM_SELL_CHECK_REMOVE						// 2008.03.11 / 김창현 / 유료 아이템 팔기 기능 체크 제거( 스크립트에서 제어한답니다. )
    /*($Delete_[S/C]$)*/#define __NA_825_HOLDDROP2											// 2008.01.21 / 김창현 / 종속의 고리2
    /*($Delete_[S/C]$)*/#define	__NA_0_BUG_FIX_POINT_RANKING_PERIOD							// 2008.01.25 / 김창현 / 포인트 랭킹 주기 관련 버그
    /*($Delete_[S/C]$)*/#define __NA_790_MODIFY_BASE_PLAYERINFO_VERSION						// 2007.11.30 / 김창현 / Base_PlayerInfo 로그 관련 버전 수정
    /*($Delete_[S/C]$)*/#define __NA_977_PREVENTION_PARTY_RETURN_ABUSE						// 2008.03.26 / 김창현 / 재접속 기능 악용 방지 추가 작업
    /*($Delete_[S/C]$)*/#define __NA_0_PET_SAVE_TO_DB										// 2008.02.21 / 김창현 / 펫 소환 시 DB에 저장
    /*($Delete_[S/C]$)*/#define __NA_0_PET_FEED												// 2008.03.06 / 김창현 / 펫 먹이
    /*($Delete_[S/C]$)*/#define __NA_0_PET_TEST												// 2008.03.06 / 김창현 / 테스트 코드 조주임님 작업 후 삭제
    /*($Delete_[S/C]$)*/#define __NA_821_STAT_BUG_RECOVERY									// 2007.12.20 / 김창현 / 스텟 버그 복구
    /*($Delete_[S/C]$)*/#define __NA_989_ABOLITION_LIMIT_LV_IN_CURSE_TOWER					// 2008.03.31 / 김창현 / 저주의탑 레벨 제한 철폐
    /*($Delete_[S/C]$)*/#define __NA_1443_BUG_FIX_HOLDDROP									// 2008.04.07 / 김창현 / 아이템 없어도 종속고리 소모되는 버그 수정
    /*($Delete_[S/C]$)*/#define __NA_0_SOCKET_OPTION_BUG_RECOVERY							// 2008.05.08 / 김창현 / 소켓 옵션 버그 복구
    /*($Delete_[S/C]$)*/#define __NA_0_BUG_FIX_SKILL_ALL_STAT_UP							// 2008.05.15 / 김창현 / 스킬로 올스텟 옵션 적용시 뻗는문제 ㅡㅡ;
    /*($Delete_[S/C]$)*/#define __NA_1017_LUCKY_ITEM_IDENTIFY_LOG							// 2008.04.23 / 김창현 / 럭키 아이템 인증 시 로그 남김
    /*($Delete_[S/C]$)*/#define	__NA_0_PET_TIMER_PERIOD_CHANGE								// 2008.05.26 / 김창현 / 펫 타이머 주기 변경
    /*($Delete_[S/C]$)*/#define __NA_1090_NO_TRADE_WHILE_PET_SUMMONING						// 2008.06.18 / 김창현 / 소환 중 거래 및 개인 상점 판매 금지


	/*($Delete_[S/C]$)*/#define __NA_000000_2ND_GUILD_SYSTEM  //2차 길드시스템
	/*($Delete_[S/C]$)*/#ifdef __NA_000000_2ND_GUILD_SYSTEM                                 // 
		/*($Delete_[S/C]$)*/#define __NA_000596_20070821_GUILD_CLASS_SYSTEM					// [596] 2007.08.21 / 조재식 , 전보환 / 길드 계급 시스템
		/*($Delete_[S/C]$)*/#define __NA_000597_20070821_GUILD_CORPS_SYSTEM					// [596] 2007.08.21 / 조재식 , 전보환 / 길드 군단 시스템
		/*($Delete_[S/C]$)*/#define __NA_000614_GUILD_RANK_REFACTORING						// [614] 2007.08.09 / 조재식 , 길드 랭크업 시스템 리팩토링( 중국 길드 랭크업 적용위한 작업 )
		/*($Delete_[S/C]$)*/#define __NA_000593_20071226_GUILD_RELATION_SYSTEM				// [593] 2007.12.26 / 노현철 , 윤영주 / 길드 관계 시스템
		/*($Delete_[S/C]$)*/#define __NA_000598_20070628_GUILD_SYSTEM_PART2_PENALTY	 		// [593] 2007.12.31 / 노현철 , 윤영주 / 길드 탈퇴,제명,해산에 의한 패널티
		/*($Delete_[S/C]$)*/#define __NA_000569_20080117_GUILD_MARK							// [593] 2008.01.17 / 노현철 , 전보환 / 길드 마크
		/*($Delete_[S/C]$)*/#define __NA_0_20070829_GUILD_ADD_INFO							// [000] 2007.12.16 / 노현철 , 윤영주 / 길드 관계 추가 정보
		/*($Delete_[S/C]$)*/#define __NA000584_20080201_GUILD_WAREHOUSE						// [000] 2008.02.01 / 노현철 , 윤영주 / 길드 창고 <2차 길드 시스템에 집어넣어야 할것>
		/*($Delete_[S/C]$)*/#define __NA_000000_2ND_GUILD_RENDERINFO_PACKET					// [584] 2008.02.01 / 노현철 , 길드 렌더 인포 패킷에 정보 추가 / 노현철/
		/*($Delete_[S/C]$)*/#define __NA_000880_20080204_GUILD_NOTICE						// [880] 2008.02.01 / 노현철 , 길드 공지 길이를 150 -> 768로 늘림, 길드 서버에서 관리되던 공지를 월드서버로 옮김
		/*($Delete_[S/C]$)*/#define __NA_0_20080406_GUILD_MARK_MODIFY						// 2008.04.11 / 노현철 / 길드 마크/관계 패킷 전송 수정		
		/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080613_GUILD_CONVERT_GP					// 2008.06.13 / 조재식 / WOPS_1963 / 길드 버그 수정( 아무나 GP변환이 되는 버그 수정. )
		/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080618_GUILD_RIGHT_VIEW					// 2008.06.18 / 조재식 / WOPS_1893 / 길드 권한 보기 버그 수정.
		/*($Delete_[S/C]$)*/#define _BUGFIX_WOPS2627_20081020_GUILD_NOTICE					// 2008.10.20 / 조재식 / WOPS_2627 / 길드 공지 버그 수정.
	/*($Delete_[S/C]$)*/#endif //__NA_000000_2ND_GUILD_SYSTEM

	/*($Delete_[S/C]$)*/#define	__NA_000000_20080709_DBP_MEMERY_LEAK_BUGFIX					// 2008.07.09 / 조재식 / DBP 메모리 릭 버그 수정.
	/*($Delete_[S/C]$)*/#define	__NA_001108_20080704_GM_ADD									// 2008.07.04 / 조재식, 김경표 / GM명령어 일본 MOVE, CALL 되도록 수정.
	/*($Delete_[S/C]$)*/#define	__NA_000950_20080311_HEIM_DISTRIBUTE_SYSTEM					// 2008.03.11 / 조재식 / 하임을 분배시스템에 적용하기.
	/*($Delete_[S/C]$)*/#define	__NA_000000_20080108_WORLDSERVER_HANDLER_MACRO				// 2008.01.08 / 조재식 / 월드서버 패킷 핸들러 매크로 적용.
	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_20080310_WORLD_FRIEND_OVERFLOW					// 2008.03.10 / 조재식 / 월드서버 친구관련 기능 오버플로 버그 수정.
	/*($Delete_[S/C]$)*/#define	__NA_BUGFIX_WOPS1373_20080424_DRAGON_TRANSFORM_STATUP_BUG	// 2008.04.24 / 조재식 / 드래곤 변신상태에서 스텟을 올릴경우 장비 아이템 적용 불가현상 수정.
	/*($Delete_[S/C]$)*/#define __NA000920_20080306_NPCSHOP_ITEM_SELL_HISTORY				// [920] 2008.03.06 /전보환,노현철/ NPCShop 아이템 판매내역 히스토리.
	/*($Delete_[S/C]$)*/#define __NA_0_080131_LF_BUG_MODIFY                                 // [000] 2008.01.10 / 노현철 / 쪽지 보낼시 개행문자가 처리되지 않는 버그 수정.	
	/*($Delete_[S/C]$)*/#define __NA_000921_20080227_TROUBLED_PARTS							// 2008.02.27 / 이수택, 김지완 / 분쟁 지역 추가 & 피아식별 룰
	/*($Delete_[S/C]$)*/#define __NA_0_20080326_ARRANGMENT_LOG_02							// 2008.03.26 / 이수택 / 2차 로그 정리
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080326_RELEASE_PARTY_KEY						// 2008.03.26 / 이수택 / 파티 종료시 유저의 파티키는 남아 있던 버그( 필드 서버 )
	/*($Delete_[S/C]$)*/#define __NA_0_20080417_ADD_GUILD_PK								// 2008.04.17 / 이수택, 김지완 / 길드 관계에 따른  PVP추가(일방적대)
	/*($Delete_[S/C]$)*/#define __NA_0_20080418_ADD_TROUBLED_KILL_LOG						// 2008.04.18 / 이수택 / 분쟁지역 유저 죽임로그 추가
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080421_MODIFY_CHAO_BORADORI_LOGIC				// 2008.04.21 / 이수택 / 같은 파티원끼리 강제공격시 공격은 되지않고 보라돌이는 되던 버그
	/*($Delete_[S/C]$)*/#define __NA_00000_080107_ADD_SERVEROPTIONEX_CHAO_CONDITION_		// 2008.01.07 / 최정록 / 케릭터의 상태(카오,일반)에 따라서 인스던전및 배틀존 이용가능 여부를 설정하는 옵션 추가
	/*($Delete_[S/C]$)*/#define __20080414_SLOTSTREAM_FOR_PACKET_BUGFIX						// 2008.04.14 / 최정록 / SCSlot 관련 Packet Stream 버그 수정
	/*($Delete_[S/C]$)*/#define __NA000773_071108_ITEM_FUNCTION_EX_REVISION__				// 2008.03.12 / 이은파 / __NA000773_071108_ITEM_FUNCTION_EX__에 대한 난독 코드 개선 + [957] 기능 포함
	/*($Delete_[S/C]$)*/#define __NA001033_ARRANGEMENT_AGENT_LOG__							// 2008.04.30 / 이은파 / AgentServer Log 정리
	/*($Delete_[S/C]$)*/#define __NA_0_20080227_BACKTO_CHARLISTREQ_SYN_MODIFY3				// 2008.02.27 / 이준석 / 대만 거래 아이템 복사 문제
	/*($Delete_[S/C]$)*/#define _NA000000_20080227_MOB_AI_TRACKING_PROBLEM_CORRECTION__		// 2008.02.27 / 이준석 / 몬스터 AI Tracking 상태가 무수히 반복될 확률 줄이기 작업( 결국, 엔진문제와 결부되어 무한 Tracking하는 상태는 마찬가지지만...)
	/*($Delete_[S/C]$)*/#define __NA_0_20080411_ALLPLAYERS_EQUIPINFO_BUG_FIX				// 2008.04.11 / 이준석 / 마을에서 캐릭터들이 장비를 벗고 있는 버그 수정
	/*($Delete_[S/C]$)*/#define __NA_997_20080415_SKILL_DAMAGE_FORMULA_MODIFY				// 2008.04.15 / 이준석 / 스킬데미지 계산공식 변경. 숙련도 향상.
	/*($Delete_[S/C]$)*/#define __NA_0_20080415_DBPROXY_USER_SELECT_QUERY_INIT				// 2008.04.15 / 이준석 / Query_User_Select 버퍼를 매번 초기화하도록 수정. 이전 캐릭의 더미값이 저장되는 것을 막음.
	/*($Delete_[S/C]$)*/#define __NA_0_20080425_WORLD_SERVER_ITERATOR_BUG_FIX				// 2008.04.25 / 이준석 / end()시 예외처리 잘못한거 수정
	/*($Delete_[S/C]$)*/#define __NA_0_20080506_DBSERIAL_DUPLICATE_BUGFIX					// 2008.05.06 / 이준석 / 아이템 중복시리얼 버그 수정
	/*($Delete_[S/C]$)*/#define __NA_20080801_SHOP_GAMESERVER_DOWN_BUGFIX					// 2008.08.01 / 이준석 / 샵 아이템 리스트를 받다가 게임서버가 계속 다운되는 문제 발생
	/*($Delete_[S/C]$)*/#define __NA_000890_20080130_SEND_MEMO_BLOCK_CHECK					// 노현철 / 자신을 차단한 대상한테 쪽지 보낼시 차단 여부 체크
	/*($Delete_[S/C]$)*/#define __NA000000_080327_TRADESELL_WAREHOUSE_BUGFIX				// 창고로 이동되면 안되는 아이템이 창고로 이동되는 버그 수정
	/*($Delete_[S/C]$)*/#define __NA_BUGFIX_20080404_FATIGUE_VILLAGE_APPLY_OTTA_			// 피로도 마을에서 적용되지 않는 버그 수정.(중국에서 문제됨.)
	/*($Delete_[S/C]$)*/#define __NA_0_20080416_PARTY_JOIN_PACKET_CHANGE					// 노현철 / 파티 초대시 적대 관계에 있는 길드원들끼리 파티 불가.
	/*($Delete_[S/C]$)*/#define __NA_0_20090122_PARTY_JOIN_PACKET_CHANGE_BUG				// 2009.01.22 / 김창현, 현철씨 파티 버그 수정
	/*($Delete_[S/C]$)*/#define __NA_000998_20080416_INVITE_BY_CHARNAME						// 2008.04.24 / 김경표, 노현철 / 아무존에서나 파티, 길드 초대 가능하게 변경
    /*($Delete_[S/C]$)*/#define __NA_0_20080415_DISP_MY_ITEMINFO_AT_TOOLTIP_IF_SHIFT_DOWN	// 2008.04.21 / 김민조/ 무기/방어 아이템 툴팁 표시시 Ctrl키를 누른 경우, 자신이 장착된 아이템 툴팁 동시 표시
	/*($Delete_[S/C]$)*/#define __NA_1633_20080423_BUGFIX_DONT_CREATE_CHAR_IF_CHARNUM_FULL  // [1633] 2008.04.24 / 김민조 / 캐릭터 5개가 모두 생성되어 있는 상태에서도 캐릭터 생성화면으로 이동하는 버그 수정
    /*($Delete_[S/C]$)*/#define __NA_001030_20080428_CENTER_ALIGN_RANK_OPTION				// 2008.04.30 / 김민조 / 툴팁에서 랭크 옵션 가운데 정렬
    /*($Delete_[S/C]$)*/#define __NA_001688_20080429_BUGFIX_HIDE_LOCKBTN_IF_CURSTATE_IS_UNLOCK	// 2008.04.30 / 김민조 / 창고 잠금 설정시 N버튼 숨기기	
    /*($Delete_[S/C]$)*/#define _YJ_MOD_TOOLTIP_ROW_001023 //'\n' 넣으면 자동개행되게 수정.
    /*($Delete_[S/C]$)*/#define __NA_0_20080331_SHOW_MSG_FOR_DISP_QUEST_PROGRESS			// 2008.04.02 / 김민조 / "Shift키를 누르고 퀘스트 리스트를 클릭하면 퀘스트 진행상황이 표시됩니다" 메시지 표시
	/*($Delete_[S/C]$)*/#define __NA000000_080416_WAREINVEN_LOCK_BUG_FIX__					// 2008.04.16 / 이은파 / 비번 설정시 주민번호 입력 단계에서 취소해도 비번 설정되는 버그 수정
	/*($Delete_[S/C]$)*/#define __NA001051_080515_EVENT_EFFECT_RENDER_BY_SCRIPT_1stPROCESS__	// 2008.05.15 / 이은파, 윤영주 / [이벤트]2주년 이벤트 이펙트 출력 작업 - 임시 (기존 버전 호환)

    // (WAVERIX) (090719) (NOTE) 하기 매크로는 지우지 말것... 일단 쓸 만하면 적용 예정
	//#define _NA_0_20080402_GUILD_DATASTRUCT_CHANGE					// 노현철 / GameGuild의 데이터를 SolarHash에서 stdext::hash로 변경
	/*($Delete_[S/C]$)*/#define _NA_001029_20080428_PARTY_LEAVE_BY_GUILD_RELATION	// 200804.28 /노현철/ 파티 맺을 시 파티 초대할 플레이어와 파티내 멤버중에 적대관계인 길드가 있는지 체크


	#if defined(__CN) || defined(_CHINA) 
		#define _GS_ADD_BIGEVENT_MSG_1122							// 2008.07.11 / 김광수 / 중국올림픽이벤트 메시지출력
        #define __NA_20070715_GUILD_PENALTY_TEMP_REMOVE				// 20080717 / 노현철 / 길드패널티 추가작업 (중국만)

		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW					// 2008.03.28 / 이준석 / 방리스트 조건체크없이 모두 보여주기. 일단 국내만 적용.
		/*($Delete_[S/C]$)*/#define _YJ_001063_001064_GUILD_RESULT_CODE_ADD				// 2008.06.09 / 노현철,윤영주 /길드관련 리절트 코드 추가
		/*($Delete_[S/C]$)*/#define _NA_0_20080530_ABUSING_USER_VERIFY_BY_WORLDSERVER	// 2008.5.30 / 노현철 / 월드 서버 가상 클라이언트 구별패킷
		/*($Delete_[S/C]$)*/#define __NA_0_20080613_VIEW_PORT_PERFORMANCE_IMPROVEMENT	// 2008.06.13 / 이준석 / 뷰포트 성능개선
		/*($Delete_[S/C]$)*/#define __NA000995_080402_SKILL_PROC_UPDATE_V01__			// 2008.04.02 / 이은파, 김지완 / Skill관련 패킷 처리 처리 개선을 위한 기초작업 Version01
		/*($Delete_[S/C]$)*/#define __NA001021_080421_PACKET_COMPRESS_V02__				// 2008.04.21 / 이은파, 김지완 / 일반 압축 알고리즘 적용을 위한 패킷 분석 및 적용
		/*($Delete_[S/C]$)*/#define __NA001021_080619_PACKET_COMPRESS_V03__				// 2008.06.19 / 이은파 / PacketControl 3rd Server<->Server (특정 패킷에 대해서만 작업)
		/*($Delete_[S/C]$)*/#define __NA_0_20080617_CLASS_STARTING_POINT_BUG_FIX		// 2008.06.17 / 이준석 / 클래스별 스타팅 포인트 기능 제거. 예전에 기획되었지만 현재 쓰이지 않음.
		/*($Delete_[S/C]$)*/#define __NA_0_20080610_CHECK_ATTACK_PROPENSITY_MODYFY		// 2008.06.10 / 이수택, 김지완 / 지역별(분쟁지역등) 공격성향(일반.분쟁.PK등) 체크 변경
		/*($Delete_[S/C]$)*/#define __NA_0_20080627_CHECK_ATTACK_PROPENSITY_MODYFY_INSERTNULL // 2008.06.27 / 이수택 / CHECK_ATTACK_PROPENSITY_MODYFY 작업시 빠진 널체크 추가.
		/*($Delete_[S/C]$)*/#define __NA_2060_20080626_BORADORI_BUG_FIX					// 2008.06.26 / 이수택 / 경계지역에서 버프스킬 시전시 보라돌이 되던 버그.
		/*($Delete_[S/C]$)*/#define __NA_2039_20080630_CHAO_SATISFY_BUGFIX				// 2008.06.30 / 이수택 / 프리카오 유저의 퀘스트 수락 가능하게(스크립트제어).[스크립트 변경시 동시 추가 되어야함]
		/*($Delete_[S/C]$)*/#define __NA_29793_20080701_CHAO_IS_ENEMY					// 2008.07.01 / 이수택 / 카오유저 자동으로 강제공격되지 않는 버그.
		/*($Delete_[S/C]$)*/#define __NA_0_20080724_BUGFIX_TILE_CHECK					// 2008.07.24 / 이수택 / 타일체크시 멤버함수내 스태틱변수 사용 버그 수정.
		/*($Delete_[S/C]$)*/#define __NA_0_20080724_CHINA_PK_RULE						// 2008.07.24 / 이수택 / 중국 PK룰 변경
        /*($Delete_[S/C]$)*/#define __NA_2001_20080617_BUGFIX_CLEAR_PARTY_WHEN_RECONNECT// 2008.06.18 / 김민조 / 재 접속시 파티원 정보 제거
        /*($Delete_[S/C]$)*/#define __NA_2206_BUGFIX_CANT_MOVE_UNDER_DRAGON_STATE_IF_ITEM_EXPIRED // 2008.07.04 / 김민조 / 드래곤 변신 상태에서 장착되어 있는 기간제 아이템 기간 만료시 이상 현상 수정
        /*($Delete_[S/C]$)*/#define __NA_0_20080707_CLEAR_HERO_INFO_WHEN_SELECT_CHAR	// 20080707 / 김민조 / 이전 캐릭터 길드 마크가 제거되지 않는 버그 수정

	#elif defined(__JP) || defined(_JAPAN) 
		/*($Delete_[S/C]$)*/#define __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__		// 2008.03.24 / 이은파 / 키보드 이동 핵 방지 및 동기화 제어 루틴 일부 제거

	#elif defined(__KR) || defined(_KOREA) 
		/*($Delete_[S/C]$)*/#define __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__		// 2008.03.24 / 이은파 / 키보드 이동 핵 방지 및 동기화 제어 루틴 일부 제거
		/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW					// 2008.03.28 / 이준석 / 방리스트 조건체크없이 모두 보여주기. 일단 국내만 적용.
	#endif

//==================================================================================================
// (0.8.1.4) (Delete reserved working macros)
//
//==================================================================================================


#endif


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 0.8.0.8 빌드 관련 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#if NATION_BUILD_VERSION >= 808 

	/*($Delete_[x/C]$)*/#define __ARMORKIT_ENABLED_CHECK_BY_CMDnACK__			// 2007/01/11 Armorkit 작동여부 체크하기 위한 패킷정의
	/*($Delete_[S/C]$)*/#define __NA070212_ENHANCED_SECURE_POLICYMENT__			// 2007/02/12 보안 강화 방안 관련 서버/클라 처리 루틴

#endif //NATION_BUILD_VERSION >= 808 


//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 공통 개발자별 Define
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━  담덕 공간  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//#define __NA_0_20061109_EXP_EVENT_								// 2006.11.09 / 이준석 / 플레이시간에 따른 경험치 증가 이벤트
//#define __NA_0_20070329_CHUNTING_FORMULA_BUG						// 2007.03.29 / 이준석 / 경쟁헌팅 보상경험치 구현 버그 수정
//#define __NA_0_20080618_STATE_INFO_SIZE_DECREASE					// 2008.06.18 / 이준석 / STATE_INFO 사이즈 축소. PLAYER_RENDER_INFO의 사이즈를 줄임.

//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


// 2006. 9. 13 / 유재영 / NPC와 대화시 NPC 가 돌아보거나 보지 않도록 하고,미니맵에 표시 여부에따른 표시.(NPCExtraInfo.txt 파서 수정등.)
//#define __CN_269_MATRIX_CARD_AUTH

//━━━━━━━━━━━━━━━━━━━  창현 공간  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//디버그 용
//#define __NA_0_20070521_QUEST_CONDITION_RESULT_CHANGE	// 2007.05.21 / 김창현 / 퀘스트 컨디션 결과 Type을 BOOL에서 enum으로 변경(디버그용)
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//기획팀 지연 작업
//#define WORK_RANKUP_ETC_ITEM							// 2006.09.19 / 김창현 / RankUp에서 같지 않은 아이템으로도 확률을 올릴 수 있는 시스템 - 보환씨 참고하3 
//#define __NA_0_20080421_REBIRTH_NO_PARTY							// 2008.04.21 / 김창현, 김경표 / 파티 아니라도 부활 가능하게...
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
// 다음빌드
//---------------------------------------------------------------------------------------------------------------------------------------------------------------
//작업
//#define __NA_1217_EXTEND_MISSION_TYPE_COUNT						// 2009.02.06 / 김창현, 미션 타입 개수 확장
///*($Delete_[S/x]$)*/#define __NA_001358_ENCHANT_UNIFICATION							// 2009.06.22 / 김창현, 인첸트 일원화

//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━  JSCHO 공간  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//#define __WAVERIX_2006122x_SERVER_TYPE_CHANGE_POSTWORK__// 2006. 12. 2x / 이은파 / 서버 타입 연구개발쪽과 맞추기 위한 작업 (매크로명은 아직 규칙을 따르지는 못했음)
// 이 매크로 적용시 구버전으로 동작(현재 적용중) 이 주석을 제거하면, 새로운 서버타입으로 전환;.

//#define	__NA_PACKETFLOW_TEST	
//#define		__TRIGGER_TEST_LOG
// ==> 위의 길드 랭크업은 해외버전에는 적용되어 있다.
//#define		__NA_000000_CHANGE_TABLE_DATA					// 2008.09.02 / 조재식 / 테이블데이터를 동적으로 변경하자.
//#define		__NA_001192_20081110_TICKET_LOG					// 2008.11.10 / 조재식 / 티켓 로그.


//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//==================================================================================================
//====================================  WAVERIX 공간 시작 ==========================================
//--------------------------------------------------------------------------------------------------
// debugging tools
//#define __NA000000_080317_ABNORMAL_EXIT_REDIRECT__                // 2008.03.17 / 이은파 / CRT Runtime Error Message Box 대신 dump남도록 하는 작업 (문제 확인이 필요한 경우에만 사용할 것)
//#define _NA000000_100510_CLIENT_PERFORMANCE_TUNEUP_USE_SHARED_POOL_  
//--------------------------------------------------------------------------------------------------
// NOTE: applied section, this working macros help reference search.
// a independent facility related to the item customizing content.
// this content can enables various outputs
// when a customizing item is pointed using an etherealizer item
// Keyword: _NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
// Keyword: __NA002610_110428_REMOVE_INFINITY_USING_LUCKY // 2011.04.28 / 이은파 / 러키 시간 (무한) 발급 제거
//--------------------------------------------------------------------------------------------------
// reserved contents for next version
//--------------------------------------------------------------------------------------------------
// specific contents for foreign version
//#define _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_ // 이은파, 이도형 / 캐시타입(PP카드) 추가에 따른 구분 작업
//#define __NA000000_070523_REPORT_TO_SOLARAUTH
//#define _NA000000_110218_RANK_REPORTER_IN_DBUNIT // 2011.02.18 / 이은파 / Ep2<-Ep1 변환시 제거되는 랭크 정보 추출기
//--------------------------------------------------------------------------------------------------
// pending list
//#define __NA000000_070928_PACKET_UNICAST_UPDATE__                 // 2007.09.28 / 이은파 / VarMsg2로 처리되던 부분을 좀더 쉽게 처리할 수 있도록 구상
//#define __NA000851_080122_APPLY_EFFECT_ITEM_ON_INVEN_FOR_CACHE__  // 2008.01.22 / 이은파 / VALENTINE 이벤트를 연동 캐시관련 작업
//#define __NA001243_090112_SKILL_ANIMATION_SYNC_CORRECTION__         // 2009.01.12 / 이은파,김광수 / 스킬에니메이션 동기화 구조 변경 작업
//#define _NA00XXXX_091123_SETUP_CASH_ITEM_AVAILABLE_PERIOD_ // 2009.XX.XX / 이은파, ? / 캐시아이템 유효기간 설정
//#define _NA_0_20100211_SD_EFFECT_MODIFY          // 2010.02.11 / 이호영 / 2타 적용 스킬 시전시 데미지 변경.
//#define _NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_    // 이은파 / 아이템 스트림 버전 8 작업
//#define _NA000000_VALUE_TYPE_SIMPLIFICATION // 2010.02.16 / 이은파 / 수치 입력 타입 설정값 (1,2)로 간소화 작업
//--------------------------------------------------------------------------------------------------
// working contents
//#define _NA000000_CW_PROTOCOL_UNIFICATION // 이은파 / WorldServer<->Client 프로토콜 CG계열과 통합, to change inter-communication
//#define _NA002676_WORLD_SERVER_RENEWAL // 이은파 / WorldServer 재구축
//--------------------------------------------------------------------------------------------------
//=====================================  WAVERIX 공간 끝 ===========================================
//==================================================================================================

//━━━━━━━━━━━━━━━━━━━ GS ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define _GS_MOD_ITEMDURA_PENALTY_1062
//━━━━━━━━━━━━━━━━━━━ END       ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━ AEORAGIE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define __NA001126_NEW_SKILL_SCREEN_EFFECT // 2008.07.16 / 최정록, 김광수 / 스킬 사용 시, 흔들림 효과 추가
//━━━━━━━━━━━━━━━━━━━ END       ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━ SHAKETAKE ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define _NA_0_20080402_GUILD_DATASTRUCT_CHANGE
//#define __NA_001169_20080930_GUILD_LOG_SYSTEM

//#define __NA_20070715_GUILD_PENALTY_TEMP_REMOVE
//#define SHAKETAKE_DATA_CHECK
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//━━━━━━━━━━━━━━━━━━━     야루 DEFINE      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define __NA_000571_20070702_WORLD_UNIT_PARTY_2ND			// 2007.07.02 / 황성익 / 월드단위 파티 2차 작업
//#define __NA_000313_20070223_REALTIME_CHAT_BLOCK			// 2007.02.23 / 황성익 / 실시간 채팅 블럭

//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━  gamelulu(민정영)  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define __[국가코드]_[작업번호]_[날짜]_[작업명]_
//#define Debug_Domination                                          // 지역 점령전 로그 관리 define       
//#define _NA_20091014_ZEROPOINT_RANKING_UNACCEPT                   // 2009.12.14 / 민정영 / 포인트 랭킹 결산시에 0포인트는 등록하지 않는다.
//#define _NA_20100415_ADJUST_POINTRANKING_SETTLE_TERMS             // 2010.04.15 / 민정영 / 포인트 랭킹 결산 조건 수정 Mission Key => RegisterDate
//#define _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC                     // 2010.04.20 / 민정영, 이태준 / 시스템 쪽지 로직 수정
//#define _NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION
//#define _NA_20100621_ADD_DOMINATION_EVENT_SELECT_GUILD_END        // 2010.06.21 / 민정영 / 지역 점령전 이벤트 추가
//#define _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION				// 2010.08.12 / 민정영, 이태준 / 시스템 메모 통합 작업(지역점령전)
//#define _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE             // 2010.09.01 / 민정영, 이태준 / 시스템 메모 관련 게임 로그 추가
//#define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                 // 2010.10.26 / 민정영 / 그룹 보너스 적용 방법 상,하위 멤버 적용 변경
//#define _NA_20101123_RESTORE_DOMINATION_EVENT_STATUS              // 2010.11.23 / 민정영 / 지역 점령전 이벤트 복구
//#define _NA_20101201_EVENT_EXTRA_TIMER                            // 2010.12.01 / 민정영 / 이벤트 임시 타이머
//#define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE           // 2010.12.08 / 민정영 / 기간 활성화 아이템 거래 불능
//#define _NA_20101026_CHANGE_GROUP_BONUS_APPLY_WAY                 // 2010.10.26 / 민정영 / 그룹 보너스 적용 방법 상,하위 멤버 적용 변경
//#define _NA_20101208_SET_PROGRESSTIME_ITEM_IS_NOT_TRADE           // 2010.12.08 / 민정영 / 기간 활성화 아이템 거래 불능
//#define _NA_20100604_BETTER_DOMINATION_MANAGER                    // 2010.06.04 / 민정영, 이태준 / 지역 점령 관리 방식 개선
//#define _CH_20100420_CHANGE_DOMINATION_APPLY_WAY                  // 2010.04.20 / 민정영, 이태준 / 지역 점령전 입찰 방식 변경
//#define _NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY                // 2010.12.14 / 민정영, 이태준 / 점령전 관리 방식 개선에 따른 경매 아이템 체크 방식 변경
//#define _NA_20101123_RESTORE_DOMINATION_EVENT_STATUS              // 2010.11.23 / 민정영 / 지역 점령전 이벤트 복구
//#define _NA_20101201_EVENT_EXTRA_TIMER                            // 2010.12.01 / 민정영 / 이벤트 임시 타이머
//#define _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT       // 2010.01.16 / 민정영, 이도형 / 점령전 방벽 데이터 추가
//#define _NA_20110303_ADD_TYPE_OF_NPCSHOP                          // 2011.03.03 / 민정영, 이도형 / 타입별 NPC 상점 기능 추가
//#define _NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN              // 2011.04.06 / 민정영, 이태준 / 점령전 참여 방식 변경
//#define _NA_20110610_MOVE_DOMINATION_CHANNEL_INFO                 // 2011.06.10 / 민정영 / 지역 점령전 체널 정보 ini파일 변경(ServerOptionEX.ini -> GameOption.ini)
//#define _NA_20110630_BATTLE_ZONE_RENEWAL                          // 2011.06.30 / 민정영, 이도형 / 배틀존 리뉴얼
//#define _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS             // 2011.12.20 / 민정영, 송찬종 / 배틀존 리뉴얼 비정상 접속 종료 유저 처리
//#define _NA_20120106_DO_NOT_REWARD_DOMINATION_TOKEN               // 2010.01.06 / 민정영 / 왕의 증표 반환되지 않도록 수정
//#define _NA_20120511_WORK_TO_DOMINATION_ADDITIONAL_REQUEST          // 2012.05.11 / 민정영 / 지역 점령전 추가 요청 사항
//  
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━  JAYKANG  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define _NA_20101011_HEIM_LOTTO_CASH_TICKET                             // 2010.10.11 / 강재준 / 캐시 아이템으로 하임행운 응모 용지 획득 기능 : _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE와 함께 활성화될 수 없음
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//━━━━━━━━━━━━━━━━━━━  NAMKA  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define _CN_0_20091013_RANKUP_EXTEND                          // 2009.10.13 / 남기봉 / 랭크업 개선. 부적의 돌, 초기화의 돌 추가 및 랭크업시 붙는 속성 옵션 제거
//#define _NA_0_20100325_QUICK_SLOT_CHANGE                      // 2010.03.25 / 남기봉,이도형 / Quick 슬롯 변경 (48개의 슬롯->40개의 슬롯으로 변경)
//#define _NA_0_20091208_VENDOR_ITEM_SEARCH                     // 2010.01.20 / 이도형, 남기봉 / 상점검색 시스템
//#define _NA_0_20100416_RELOAD_MAPOBJECT                       // 2010.04.16 / 남기봉, 윤영주 / 맵오브젝트 파일 리로드
//#define _NA_0_20100519_GMCOMMAND_MOVEFIELD                    // 2010.05.19 / 남기봉 / GM 명령어 추가. 동일한 게임Zone의 특정 필드로 순간 이동
//#define _NA_0_20100519_GMCOMMAND_CREATEMISSION                // 2010.05.19 / 남기봉 / GM 명령어 추가. 동일한 게임Zone의 특정 필드로 순간 이동
//#define _NA_0_20100528_ACCUMULATE_POINT_EVENT                 // 2010.05.28 / 남기봉,이태준 / 적립 포인트 보상 룰렛시 특정 [요일/시간] 상급 아이템 획득 이벤트 추가
//#define _NA_0_20100901_SKILLTREE_SYSTEM                       // 2010.09.01 / 남기봉, / 스킬트리 시스템 추가
//#define _NA_0_20101011_ACCUMULATE_POINT_LIMIT_OPTION          // 2010.10.11 / 남기봉, / 적립 포인트 지급시 레벨/최대갯수 를 제한하는 옵션 추가.
//#define _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL               // 2011.01.31 / 남기봉,김신태 / 스킬트리 시스템의 스킬의 최초 습득 방식 변경.(스킬 기본 레벨 0스킬 -> 1스킬).
//#define _NA_0_20110305_BUG_READ_BIG_LOGFILE                   // 2011.03.05 / 남기봉 / 2GB이상의 로그 읽기 실패 버그.(파일 핸들 읽기 함수로 변경)
//#define _NA_0_20110308_SKILL_COOL_TIME_STORAGE                // 2011.03.08 / 남기봉, 유명수 / 캐릭터 비접속시 스킬 쿨타임 유지 기능 추가.
//#define _NA_0_20110318_ITEM_EXPIRATION_PERIOD                 // 2011.03.18 / 남기봉, / 습득 아이템 완료 기간 설정 추가.
//#define _NA_0_20110329_GAMEOPTION_SETTING                     // 2011.03.29 / 남기봉, 유명수/ 게임 옵션의 DB 저장
//#define _NA_0_20110613_GAME_SHOP_RENEWAL                      // 2011.06.13 / 남기봉, 유명수/ 캐시샵 리팩토링.
//#define _NA_0_20110803_PLAYER_MOVEMENT_SYNC                     // 2011.08.03 / 남기봉, 플레이어 이동 동기화.
//#define _NA_0_20110803_DIV_GAMELOG_FILE                         // 2011.08.03 / 남기봉, 게임로그 용량에 따른 파일분리.
//#define _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT                    // 2011.08.03 / 남기봉, 로또 아이템 사용가능한 플레이어 레벨 제한.
//#define _NA_0_20110915_CLIENT_MAC_ADDRESS                       // 2011.09.15 / 유명수, 남기봉, 접속 클라이언트의 MAC 주소 로그 남기기.
//#define _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT     // 2011.10.21 / 남기봉, / 파티 인원수에 의한 하임 드랍율 조정.
//#define _NA_0_20111114_CHANGE_MOVE_PATH_COUNT                   // 2011.10.21 / 남기봉,김신태 / 마우스 이동시 이동 경로 갯수 변경 (기존 64 -> 512)
//#define _NA_0_20111130_DISTROY_WEAPON                         // 2011.10.21 / 남기봉,김신태 / 내구도 0인 무기도 방어구와 같은 방식으로 파괴되도록 기능 수정.
//#define _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT                 // 2012.01.02 / 남기봉 / 파티원 인원수에 따른 하임 가중치 변경.
//#define _NA_0_20120104_DRAGON_COMMON_SKILL                    // 2012.01.05 / 남기봉,이태준 / 드래곤 나이트와 용변신 1,2 차의 스킬 공유 기능 추가.
//#define _KR_0_20120207_CRYPT_SHA256                             // 2012.02.07 / 남기봉 / 암호화 모듈 MD5 -> SHA256 변경.
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━


//━━━━━━━━━━━━━━━━━━━  YJ  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━  문상현  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//#define _NA_20100307_BETAKEY_SYSTEM                                   //베타키 시스템 (문상현, 이도형)
//#define _NA_20100322_AGGRO_DISPLAY                                    //어그로 수치 화면에 표시 (문상현, 이도형)
//#define C_NA_0_20100520_ACSYSTEM_CHANGED                            //2010.05.20 문상현 이태준 AC 시스템을 업적단위로 변경한다.
//#define _JP_0_20101123_PMS                                          //일본 게임온의 서버머 관리 시스템인 PMS 적용
//#define _JP_20110222_INGAME_CHARNAME_CHANGE                         //일본에 서버 통합을 위해 캐릭텨명 변경을 게임내에서 지원해준다 (웹으로 캐릭명 변경이 안된다.)
//#define _RU_20110801_SEND_GIFT_FRIEND_CHECK //친구에게 선물을 보낼때 내가 상대방의 친구여야 하고 상대방도 나의 친구여야 한다.
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//--------------------------------------------------------------------------------------------------
// hukim(김현웅) 시작
//
//#define _NA_000068_20100419_PARTY_FINDING_SYSTEM // 2010.04.19 / 김현웅, 이도형 / 파티 찾기 시스템
//#define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM // 2011.06.13 / 김현웅, 이도형 / 정보 열람실 통합 사용자 가이드 시스템
//#define _NA_003740_20111122_RANDOM_ITEM // 2011.11.22 / 김현웅, 김신태 / 랜덤 아이템 생성
//#define _NA_003966_20111227_ADD_ENCHANT // 2011.12.27 / 김현웅, 김신태 / 최상급 인첸트
//#define _NA_004010_20120102_1201_SD_FORMULA // 2012.01.02 / 김현웅 / SD 관련 공식 변경
//#define _NA_004087_20120106_MODIFY_VALKYRIE_ATTACK_FORMULA (1) // 2012.01.06 / 김현웅 / 발키리 공격력 공식 변경
//#define _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY // 2012.01.11 / 김현웅 / 게임서버 ObjectFactory 개선
//#define _NA_004126_20120111_EXTEND_OBJECT_KEY // 2012.01.11 / 김현웅 / ObjectKey 대역폭 확장
//#define _NA_004035_20120227_COSTUME_ITEM // 2012.02.27 / 김현웅, 김신태 / 코스튬 아이템
//#define _NA_004517_20120312_GAME_OPTION_TRANSACTION // 2012.03.12 / 김현웅 / 게임 옵션 트랜잭션 처리 기능 추가
//#define _NA_004530_20120315_1202_SKILL_RENEWAL // 2012.03.15 / 김현웅, 김신태 / 1202 스킬 리뉴얼
//#define _NA_004605_20120417_ACTIVE_ETHER_BULLET_TO_SKILL // 2012.04.17 / 김현웅, 김신태 / 스킬 공격에 에테르 탄환 적용
//#define _NA_004899_20120530_CHANGE_DOMINATION_REWARD_OPTION // 2012.05.30 / 김현웅, 이태준 / 점령전 보상 옵션 적용 방식 변경
//#define _NA_004965_20120613_NEW_CHARACTER_MYSTIC // 2012.06.13 / 김현웅, 이태준 / 신규 캐릭터 미스틱 추가
//#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    //#define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT // 2012.06.12 / 이태준 / 신규 캐릭터 클라이언트 수정(스크립트파서관련)
    //#define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP // 2012.06.12 / 이태준 / 신규 클래스 타입 추가에 따른 타입MAX 증가로 인한 발생되는 오류 임시 수정
//#endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
//
// hukim(김현웅) 끝

//━━━━━━━━━━━━━━━━━━━  chief85(김진휘)  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// chief85(김진휘) 시작
//#define _NA_004694_20120412_1202_PARTY_ROOM_KEY                                   // 2012.04.12 / 김진휘 / 배틀존 방번호를 커뮤니티 화면에 표시.
//#define _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE                   // 2012.04.13 / 김진휘 / 배틀존 방 예약 시 비밀방은 입장여부 메세지 보내지 않음.
//#define _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST                     // 2012.04.16 / 김진휘 / 접속 종료시 예약된 배틀존 방 리시트를 삭제한다.
//#define _NA_004791_20120430_1202_SUMMON_STATE                                     // 2012.04.30 / 김진휘 / 발키리 소환수가 타겟을 수동 공격하는 동안, 이미 활성화된 커맨드 미 복종.
//#define _NA_004830_20120511_1202_SSQ_CLOSESPACE_DAMAGE_RANGE                      // 2012.05.11 / 김진휘 / 타탁한 사원 중앙 돔 구역 페널티 지역 범위 재 설정 및 결계 지역 내 PVP가능.
//#define _NA_004035_20120227_COSTUME_ITEM                                          // 2012.05.23 / 김진휘, 김신태 / 코스튬 아이템
//#define _NA_004910_20120601_SUMMON_PVP_RULE                                       // 2012.06.01 / 김진휘 / 발키리 소환수 SD(99):HP(1) 룰 적용.
//#define _NA_000000_20120607_INVENTORY_SORT                                        // 2012.06.07 / 김진휘 / 인벤토러 정렬.
//#define _NA_004994_20120614_RATE_OF_DEFENCE_POWER_REDUCTION_OF_TARGET             // 2012.06.14 / 김진휘 / 퍽효과<대상의 방어력 감소율적용>
//#define _NA_005010_20120621_RATE_OF_DEFENCE_SUCCESS_MODIFY                        // 2012.06.21 / 김진휘, 송찬종 / 방어 성공률 상한선 수정 99% -> 80%
//#define _NA_005005_20120622_SUMMON_PVP_RULE_SYNC_OWNER_PVP_RULE                   // 2012.06.22 / 김진휘 / 발키리 소환수 PVP룰을 주인과 같은 룰 적용.
//#define _NA_005028_20120627_MODIFY_IMPOSSIBLE_CAST_HIDE_SKILL_IN_SSQ              // 2012.06.27 / 김진휘 / 타락한 사원에서 섀도우 하이드 스킬 사용불가능.
//#define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND                              // 2012.06.27 / 송찬종, 김진휘 / 카오스 존 전장 시스템.
//#define _CN_006014_20120822_GUILD_LOG_DELETE                                      // 2012.08.22 / 김진휘 / 중국 길드로그에 왕의증표 입찰갯수 로그 삭제.
//#define _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA             // 2012.08.23 / 김진휘 / 길드코인 하루 기부 최대 갯수 스크립트 데이터로 적용
//#define _NA_006040_20120830_RESIS_BAD_STATUS_RATIO                                // 2012.08.23 / 김진휘 / 물약 및 버프 스킬 사용시 스킬무시확률이 적용되는 버그
//#define _NA_006052_20120904_CANNOT_DROP_CAN_PKDROP                                // 2012.09.04 / 김진휘 / 아이템 바닥버리기 불가능, PK드롭가능 타입 추가.
//#define _NA_006057_20120904_PK_DROP_RATIO_MODIFY                                  // 2012.09.04 / 김진휘 / 자신의 레벨 보다 높은 플레이어를 죽였을 경우 죽은 플레이어의 아이템드랍률 수정 상한선 100%->50%
//#define _NA_006088_20120910_CHAOS_ZONE_FREE_PVP                                   // 2012.09.10 / 송찬종, 김진휘 / 카오스 존 자유전투 시스템.
//#define _NA_006131_20120924_ETHER_DAMAGE_RULE_MODIFY                              // 2012.09.25 / 김진휘 / 에테르탄환 정책 변경(기본공격력만 -> 전체데미지)
//#define _NA_006135_20120926_MISSION_RESERVATION_PRIORITY_MODIFY                   // 2012.09.26 / 김진휘 / 배틀존 입장예약 우선순위 변경. (최초 -> 최신순)
//#define _NA_006164_20121012_FREE_PVP_ROOM_PARTY_MEMBER_INFO_CURRENT_ZONE_MODIFY   // 2012.10.12 / 김진휘 / PVP룸에서 파티정보창의 현재위치가 잘못 표시 되는 버그 수정
//#define _NA_006174_20121015_MOVE_ZONE_POLICY_AIRSHIP_FIELD_ADD                    // 2012.10.15 / 김진휘 / 배틀서버에서 필드서버로 이동시 비공정 필드도 가능하도록 수정
//#define _NA_006202_20121017_DASH_AND_PIERCE_SKILL_SYNC_BUG_MODIFY                 // 2012.10.17 / 김진휘 / 돌진 및 관통계열 스킬 싱크및 판정 수정
//#define _NA_006244_20121101_INCORRECT_WORK_OF_ENERGY_SUCTION                      // 2012.11.01 / 김진휘 / 에너지석션을 시전하는 도중 공격불가상태(스턴, 다운..)가 되어도 스킬이 계속 시전되는 버그
//#define _NA_006251_CUBESYSTEM_Decomposition_REWARD_REMOVE_CASH_ZARD               // 2012.11.16 / 김진휘 / 큐브시스템 분해시 보상리스트에서 캐시쟈드 제거
//#define _NA_006322_20121211_MODIFY_RUNE_STONE_WAYPOINT_DELETE_TIME                // 2012.12.11 / 김진휘 / 개인 웨이포인트 삭제 대기 시간 수정
//#define _NA_006352_20121228_IF_STEEL_MODE_VALKYRIES_MODIFY_SUMMON_POIICY          // 2012.12.28 / 김진휘 / 소환수의 주인이 강철모드일 경우 공격,피격,스킬사용 가능 -> 불가능으로 수정
//#define _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER                              // 2013.01.03 / 김진휘 / 서버이벤트 트리거 추가
//#define _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM                      // 2013.01.04 / 김진휘 / team check condition 트리거 추가
//#define _NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD                       // 2013.01.04 / 김진휘 / [개인/전체]에게 [HP/MP/SD]를 [수치]만큼 [증가/감소]를 적용 action 트리거 추가
//#define _RU_006484_20130225_VALKYRIES_SUMMONS_BUG                                 // 2013.02.25 / 김진휘 / 발키리의 소환수를 소환 하여 몬스터를 공격, 소환수가 캐릭터 근처로 이동 할 때 까지 캐릭터를 뒤로 이동시킬 경우 몬스터가 바보 되는 현상
//#define _RU_006525_CRYSTALWARP_BUG_IN_HOLY_BOOLD                                  // 2013.03.13 / 김진휘 / 피의성전에서 워프크리스탈을 소환 후 캐스팅이 다 되기전 소환한 유저가 죽어도 크리스탈이 소환되는 버그
//#define _JP_006544_DRAGON_NIGHT_DRAGONTANS_BUG                                    // 2013.03.19 / 김진휘 / 드래곤나이트로 무적상태에서 용변신 안되는 버그 수정
//#define _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM                              // 2013.03.20 / 김진휘 / 전장 추가작업 #1 <전장 시간 제어 시스템 추가>
//#define _NA_006577_BATTLE_GROUND_BATTLE_STATE_NOTIFY                              // 2013.03.25 / 김진휘 / 전장 추가작업 #2 <전장 전투 상황 알림>
//#define _NA_006592_VALKYRIES_SUMMONS_SKILL_COOLTIME_BUG                           // 2013.03.28 / 김진휘 / 소환사(발키리)의 상태 중 스킬쿨타임 증감 상태가 소환수에게 적용이 안되는 버그
//#define _NA_006605_VALKYRIES_SUMMONS_BATTLE_RESULT_APPLY                          // 2013.04.01 / 김진휘 / 미션 및 전장에서 소환수의 전투결과 데이터를 전투결과창에 반영 (적에게 가한 데미지, 소환수의 킬수, 힐량)
//#define _NA_006606_VALKYRIES_SUMMONS_HEAL_SKILL_DONOT_APPLY_ADD_SKILL_RATIO       // 2013.04.01 / 김진휘 / 발키리 소환수의 힐 스킬이 소환사(주인)의 스킬공격력 비율에 맞게 추가 힐량이 계산되지 않은버그
//#define _NA_006623_20130409_STEEL_MODE_WHEN_ENERMY_CAN_USE_SKILL                  // 2013.04.09 / 김진휘 / 내가 강철 모드일 경우 적이 나에게 스킬이 시전되는 현상 수정.
//#define _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME                           // 2013.04.02 / 김진휘, 송찬종 / GM명령어로 이벤트 전장 시작
//#define _NA_006589_20120415_ENCHANT_PRICE_MODIFY                                  // 2013.04.15 / 김진휘 / 인첸트 비용 공식 수정
//#define _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO                        // 2013.04.15 / 김진휘, 송찬종 / 타겟창 정보에 쉴드 표시 추가
//#define _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY                             // 2013.04.16 / 김진휘 / 커뮤니티창 상세정보에 종족이 잘못 표시되는 버그
//#define _NA_006656_20120422_PC_BANG_BADGE_BUG_ON_DRAGONTRANS                      // 2013.04.22 / 김진휘 / 용변신 상태에서 크리스탈팰리스 아이템 활성화시 착용하고 있는 장비가 적용되지 않은 버그
//#define _NA_006654_20120422_MISSION_TRIGGER_ADD_AND_MODIFY                        // 2013.04.22 / 김진휘 / 미션 트리거 추가 및 수정작업
//#define _NA_006669_20130426_SUMMON_SKILL_SYNC_BUG_MODIFY                          // 2013.04.26 / 김진휘 / 발키리 소환수 스킬 싱크 버그
//#define _NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY                            // 2013.04.26 / 김진휘 / 아이템 옵션 추가 변경
//#define _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME                            // 2013.05.02 / 김민철, 김진휘 / 게임 옵션 추가(코스튬 보이기 옵션)
//#define _NA_006699_20130506_DONOT_ATTACK_WHILE_BATTLE_GROUND_RESERVATION          // 2013.05.06 / 김진휘 / 전장 예약중 필드 몬스터 공격 불가
//#define _NA_000000_20130508_DONOT_SERVER_SELECT_WHILE_BATTLE_GROUND_RESERVATION   // 2013.05.08 / 김진휘 / 전장 예약중 서버 선택 불가
//#define _NA_006710_20130509_CRYSTALWARP_BUG                                       // 2013.05.09 / 김진휘 / 이미 파괴된 크리스탈을 참조하지 못할 경우 강제로 다음 크리스탈을 참조하는 버그
//#define _NA_006728_20130520_SKILL_BALANCE_MODIFY                                  // 2013.05.20 / 김진휘 / 스킬 밸런스 수정( 반사관련 스킬 수정 )
//#define _NA_006731_20130521_ENCHANT_ADD_OPTION                                    // 2013.05.21 / 김진휘, 송찬종 / 최상급(13~15)인첸트 옵션 추가
//#define _CH_006797_20130703_PK_DROP_RULE_MODIFY                                   // 2013.07.03 / 김진휘 / 중국 필드 PK 정책 변경
//#define _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM                             // 2013.07.03 / 김진휘, 송찬종 / 전장 투표 시스템 추가
//#define _NA_006826_20130722_DOMINATION_RENEWAL                                    // 2013.07.22 / 김진휘, 송찬종 / 피의성전 점령전 개편
//#define _CN_000000_20130731_STATE_STEEL_MOD_BUG_LOG                               // 2013.07.31 / 김진휘 / 중국 강철모드 버그 관련 로그기록
//  
// chief85(김진휘) 끝
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━  민경승  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 민경승 시작
//#define _NA_006593_20130327_ADD_ACHIEVEMENT_BATTLEGROUND        //2013.03.27 / 민경승 / ac과제 추가(자피르 격전지)
//#define _NA_006599_20130401_BATTLEGROUND_PKPOINT                //2013.04.01 / 민경승 / 전장에서 적 처치 시, 추가 득점
//#define _NA_006622_20130408_BUG_DASH_AND_PIERCE_SKILL           //2013.04.08 / 민경승 / 이동+관통형 스킬이 1명의 적에게만 피해를 줌
//#define _NA_006624_20130409_CAN_OBTAIN_SOLOBADGE_COUPLEBADGE    //2013.04.09 / 민경승 / 솔로부대뱃지와 커플염장뱃지를 함께 가질 수 있도록 함
//#define _NA_006607_20130402_ADD_QUESTTYPE						//2013.04.02 / 민경승 / '미션완료', '전장승리' 퀘스트타입 추가
//#define _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST             //2013.04.15 / 민경승 / 전장에 '어시스트' 추가
//#define _NA_006686_20130502_REMAKE_BUFF_PRIORITY                 //2013.05.02 / 민경승 / 상태 버프의 우선순위 계산방식 변경
//#define _NA_006689_20130503_CANNOTUSE_BUFFITEM                     //2013.05.03 / 민경승 / 상위버프가 걸려있을 경우,하위버프 물약 사용 방지
//#define _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP        //2013.05.06 / 민경승 / 어빌리티타입추가(345), 스킬공격력, 방어효과에 영향받지않는 HP증감
//#define _NA_006801_20130705_RENEWAL_RANKINGSYSTEM               //2013.07.05 / 민경승 / 랭킹시스템 리뉴얼 및 전장랭킹 추가
//#define _RU_006860_20130806_COSTUME_PCBANG_EFFECT                //2013.08.06 / 민경승 / 코스튬에 피시방 옵션 적용시키기
//#define _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR               //2013.08.14 / 민경승 / 랭킹시스템 리뉴얼 및 전장랭킹 추가(마이너)
//#define _CN_006894_20130913_PK_ITEMDROP_ABOUTHOSTILITYRELATION      //2013.09.13 / 민경승 / 중국 PK에서 사망시, 적대관계에서도 아이템이 드랍된다.
//#define _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL           //2013.10.30 / 민경승 / 헬로이드 스킬위한 크리티컬 어빌리티 상태 추가 및 수정
//#define _NA_000000_20131112_RECONNECT_CLOSINGUSER_IN_MISSION    // 2013.11.12 / 민경승 / 미션내에서 비정상 종료 후 재접속하면, 미션 내 종료된 위치로 배치된다.
//#define _NA_000000_20131129_ADD_KISS_API                          // 2013.11.29 / 민경승 / 썬에 KISS를 붙인다. 서버 총 동접자수를 전송한다.
//#define _NA_000000_20140103_CHANGE_DASH_AND_PIERCE_SKILL_MOVE     // 2014.01.03 / 민경승 / 돌진관통스킬의 이동처리를 DamageAbility가 아닌, QuickStep어빌리티로 대체한다.
//#define _NA_007009_20131226_RETURN_TO_POSITION_IF_SAFETYZONE      // 2013.12.26 / 민경승 / 몬스터가 안전지대로 들어가면 '방황'상태가 아니라 '리턴'상태로 전환하도록 한다.
//#define _NA_007009_20140102_ATTACK_AFTER_THRUSTSTATE              // 2014.01.02 / 민경승 / thrustState(넉백상태)이후 비선공 몬스터가 반격을 하지 않는 문제 처리
//#define _NA_000000_20140103_MALLOC_CHANGE_TO_TALLOCATE_CREATEITEM   // 2014.01.03 / 민경승 / 아이템 생성시, 메모리할당에서 서버가 죽는 문제로, 오브젝트풀과 메모리할당을 바꿔본다.
//#define _NA_007039_20140121_TIME_UNIT_TEXTLOG                        // 2014,01,21 / 민경승 / 텍스트로그가 하루단위에서 시간단위로 파일을 생성하도록 변경
//#define _NA_007053_20140210_BUG_SUMMONED_MONSTER_ATTACKSPEED      // 2014.02.10 / 민경승 / 소환수의 공격속도가 빨라지는 버그 수정
//#define _NA_007071_20140219_MODIFY_UNUSE_ARMOR_DESTROY_PENALTY      // 2014.02.19 / 민경승 / 사망 시, 방어구 내구도가 0이면 낮은확률로 장비가 소멸하는 패널티 제거
//#define _CN_000000_20140224_STATE_STEEL_MOD_BUG_LOG2 // 2014.02.24 / 민경승 / 중국 강철모드 버그 관련 로그기록
//#define _NA_007079_BUG_WEAPON_INACTIVATE_STATE         // 2014.02.25 / 민경승 / 내구도0이나 스탯변화로 무기 비활성화 시, 무기를 장착해제한 상태로 만든다.
//#define _NA_007085_BUG_GUILD_DAILYQUEST_ACCEPT_CONDITION            // 2014.03.04 / 민경승 / 길드일일퀘스트를 '완료'했는데 보상을 받지않은 상태에서 같은그룹퀘스트를 받는 문제 수정
//#define _NA_007065_20140217_CHAOSZONE_GOLDRUSH                      // 2014.02.17 / 민경승 / 신규 전장모드 골드러시
//#define _NA_007065_20140217_BATTLEGROUND_REFACTORYING             // 2014.02.17 / 민경승 / 매칭시스템 리팩토링
//#define _NA_007112_20140312_MODIFY_PROCESSRECEIVEDPACKET            //2014.03.12 / 민경승 / ProcessReceivedPacket함수에서 패킷처리 실패시 false리턴되는 부분 제거
//#define _NA_007123_20140324_SHADOW_SKILL_RENEWAL                     //2014.02.24 / 민경승 / 쉐도우 스킬 리뉴얼
//#define _NA_007129_20140327_MODIFY_CAN_EQUIP_ZERODURA                  //201403.27 / 민경승 / 내구도0인 아이템 장착가능
//#define _NA_007136_20140404_MODIFY_SUCTION_DAMAGE_CALCULATE             //2014.04.04 / 민경승 / 쉐도우의 에너지석션 데미지공식변경
//#define _NA_007191_20140428_MODIFY_GM_COMMAND                         // 2014.04.28 / 민경승 / GM명령어 추가/수정
//#define _JP_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO       // 2014.04.30 / 민경승 / 친구초대 레벨별 보상지급시, 레벨값을 const.h로 빼낸다
//#define _NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN                       // 2014.05.19 / 민경승 / DB로그 길드코인기부
//#define _NA_007239_20140521_BUG_GUILDRELATION_COUNT                          // 2014.05.21 / 민경승 / 길드관계관련 최대치 수정
//#define _NA_007257_20140527_GAMELOG_ITEM_COMPOSE_SUCCESS_COUNT      // 2014.05.27 / 민경승 / 아이템조합 성공 기록 시, ItemNum은 인벤토리 총 개수가 아닌, 조합성공한 개수로 한다.
//#define _NA_007264_20140529_BUG_CALCULATE_SUMMONED_ATTRIBUTES       // 2014.05.29 / 민경승 / 발키리소환수 능력치 계산식 오류 수정
//#define _NA_007266_20140530_BUG_ATTACH_STATE_TRIGGER                  // 2014.05.30 / 민경승 / 상태부여 트리거가 맵오브젝트한테 붙였을때만 작동한다
//#define _NA_007288_20140611_GAMELOG_GAMESHOP                          // 2014.06.11 / 민경승 / 캐시샵의 아이템 구매, 선물관련 DB로그 기록하도록 추가
//#define _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT               // 2014.07.02 / 민경승 / 골드러시에서 골드포인트 변동사항을 DB로그에 남긴다
//#define _DEV_007390_20140703_AUTH_EXCEPTION_VERSIONCHECK               // 2014.07.03 / 민경승 / 개발,사내서버에서 프로토콜 버전체크 무조건 통과시킨다.
//#define _NA_007407_20140710_CHALLENGE_DAILY_QUEST                       // 2014.07.10 / 민경승 / 도전일일퀘 추가
//#define _NA_007422_20140716_MODIFY_MAX_ACHIEVEMENT_COUNT                       // 2014.07.10 / 민경승 / AC 수행가능횟수 늘리기
//#define _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION       // 2014.07.18 / 민경승 / 재작재료가 장비일 경우 '인챈트'에 관련된 조건 변경
//#define _NA_007408_20140711_MODIFY_CRYSTALLIZE                           // 2014.07.11 / 민경승 / 결정화 바꾸기
//#define _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG                     // 2014.08.11 / 민경승 / DB길드로그 작업
//#define _NA_007617_20140916_ADD_CHARSTATE_UPPERDOWN                       // 2014.09.16 / 민경승 / 이상상태 어퍼다운 추가
//#define _NA_007667_20141001_WITCHBLADE_SKILL                            // 2014.10.01 / 민경승 / 위치블레이드 스킬작업
//#define _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION                   // 2014.10.27 / 민경승 / 모든 인원의 입장아이템 검사를 하는 존타입 추가
//#define _NA_007770_20141103_BUG_AREACONQUEST_REWARD_CHECK                   // 2014.11.03 / 민경승 / 패킷변조에 의한 AC보상 무한받기문제 처리
//#define _NA_007827_20141117_ADD_ACCESSORY_TYPE                              // 2014.11.14 / 민경승 / 악세사리 타입 추가
//#define _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE                 // 2014.11.18 / 민경승 / 파티상태에서 캐릭터 상태(HP,MP,SD,FP) 업데이트 방식 변경
//#define _NA_007854_20141127_MODIFY_PET_CHARGE_SUBTYPE                         // 2014.11.27 / 민경승 / 펫도 소모타입 11번을 쓸수있도록한다
//#define _NA_007909_20141218_SUNLOG_QUESTLIST                                  // 2014.12.18 / 민경승 / 텍스트로그용, 퀘스트 보상받기 실패하면 자신이 받은 퀘스트리스트 코드 모두출력
//#define _NA_007919_20141211_ADD_GMCOMMAND_EVENT								// 2014.12.11 / 민경승 / GM명령어, 경험치이벤트 제어하기
//#define _NA_007973_20150120_ADD_MULTIOPTION_ZARD                                // 2015.01.15 / 민경승 / 어떤쟈드를 합성해도 결과물은 랜덤으로 나오도록 수정
//#define _NA_007992_20150127_EVENT_MONSTER_INVASION                              // 2015.01.27 / 민경승 / 몬스터침공 이벤트매니저로 제어
//#define _NA_008089_20150225_BUG_EXPIRE_UNEQUIP_BADGE                              // 2015.02.25 / 민경승 / 크리스탈펠레스,뱃지아이템 시간만료시 제대로 해제해주지않는문제
//#define _NA_008119_20150310_CHECK_ONESHOT_ONEKILL_SITUATION                     // 2015.03.10 / 민경승 / 플레이어끼리 싸움에서 한방에 죽는경우 체크하고 로그로 기록한다
//#define _NA_008132_20150316_MONDSCHEIN_TEXTLOG                                  // 2015.03.16 / 민경승 / 몬트샤인용 텍스트로그, 한방데미지문제 감시용
//#define _NA_008069_20150210_ADD_SPA_SYSTEM                                       // 2015.02.10 / 민경승 / 온천시스템 추가
//#define _NA_008166_20150330_MODIFY_AC_LEVELUP                                     // 2015.03.30 / 민경승 / 레벨업AC이 경우 '이전레벨' AC도 같이 검색해서 처리하도록한다
//#define _NA_008182_20150402_MODIFY_ITEM_MOVE                                        // 2015.04.02 / 민경승 / 아이템 이동관련 문제 수정
//#define _NA_008220_20150414_BUG_ETHERBULLET_ATTR                                  // 2015.04.14 / 바이스소드같은 지속적으로 동작하는 스킬이 마야탄환의 증뎀률 증가 계산에 문제를 발생시킨다
//#define _NA_008244_20150422_MODIFY_MONSTER_ESCAPE_SAFETY_ZONE                       // 2015.04.22 / 몬스터가 안전지대에서 귀환루트를 못찾을경우 텔레포트시켜버린다
//#define _NA_008260_20150429_MODIFY_EFFECTSKILL_LIFETIME                             // 2015.04.29 / 다크오브라이트같은 'Effect'를 사용하는 스킬이 사용자가 죽을경우 같이 사라지도록 한다
//#define _NA_000000_20150507_TIME_DISPLAY                                              // 2015.05.07 / 민경승 / agentserver, 스페이스바 누르면 시간정보 띄우기
//#define _NA_000000_20150511_QUEST_BUG                                                 // 2015.05.11 / 민경승 / 퀘스트버그관련작업, db프로시저와 패킷간 싱크조절
//#define _NA_008321_20150601_BATTLE_INFO                                                 // 2015.06.01 / 민경승 / 밸런스작업을 위해 클라에 전투정보 전달하기
//#define _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK                               // 2015.06.03 / 민경승 / 윈드실드 효과가 강제어택 먹히도록한다
//#define _NA_008334_20150608_SONNENSCHEIN                                                  // 2015.06.08 / 민경승 / 몬트샤인 PvE, 소넨샤인
//#define _NA_008389_20150703_TIMERITEM_PET_DROP_RULE                                       // 2015.07.03 / 민경승 / 시간제 펫아이템 드랍될 시, charge sub type 1번도 가능하도록 수정
//#define _NA_008456_20150819_ETHER_BULLET_PVP_CHECK                                        // 2015.08.19 / 민경승 / 에테르탄환이 PvP체크를하여 PvP시 수량이 줄지 않도록 한다
//#define _NA_008473_20150904_HELLOIDSPIN_BACKSTEP                                          // 2015.09.04 / 민경승 / 헬로이드스핀 상태에서 BACKSTEP이동은 정면달리기와 같은 속도를 가진다
//#define _NA_008480_20150909_MODIFY_FIGHTING_ENERGY                                        // 2015.09.09 / 민경승 / 스피릿리차지, 검기 풀 충전상태에서도 스킬사용 가능
//#define _NA_008481_20150909_MODIFY_REMOVE_ATTSPEED_300LIMIT                               // 2015.09.09 / 민경승 / 공속 300제한 해제
//#define _NA_008483_20150910_REMOVE_DAYLIGHT_TIME                                          // 2015.09.10 / 민경승 / 국내에서 일광 절약 시간 0으로 세팅해버린다
//#define _NA_008486_20150914_TOTAL_BALANCE                                                 // 2015.09.14 / 민경승 / 밸런스작업
//#define _NA_008506_20150923_LOG_LOGOUT_IN_GAMESERVER                                      // 2015.09.23 / 민경승 / 게임서버에서 기록하는 로그아웃 로그 추가
//#define _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL                                       // 2015.10.01 / 민경승 / 공격자길드정보 제거, 점령전 사전알림 모든필드유저에게 보내게하기
//#define _NA_008540_20151027_ADD_ITEMOPTION_ELITE4                                           // 2015.10.27 / 민경승 / 엘리트4 옵션 추가 작업
//#define _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA                                           // 2015.11.05 / 민경승 / 이속공식 변경
//#define _NA_000000_20151109_BUG_SSQ_HANDLER_NULL                                              // 2015.11.09 / 민경승 / ssq handler 관련 버그
//#define _NA_008570_20151116_MODIFY_VENDOR_SEARCH                                            // 2015.11.16 / 민경승 / 개인상점 검색 시, 마을제한 제거
//#define _NA_000000_20151126_MODIFY_COSTUME_CALCULATE_DATETIME                                 // 2015.11.26 / 민경승 / 코스튬아이템 기간설정관련 수정
//#define _NA_008606_20151222_MODIFY_GUILD_LEVEL                                              // 2015.12.22 / 민경승 / 길드멤버 탈퇴 시, 레벨다운 되지 않도록 수정
//#define _NA_008610_20160104_ADD_DECORATECOSTUME                                               // 2016.01.04 / 민경승 / 새 코스튬슬롯 추가(보조코스튬)
//#define _NA_008588_20151202_MODIFY_MOVEING_SKILL_LOGIC                                      // 2015.12.02 / 민경승 / 이동스킬 로직 위치변겅
//#define _NA_008664_20160129_MODIFY_SETITEMOPTION_CONDITION                                    // 2016.01.29 / 민경승 / 세트아이템 옵션 적용조건 변경
//#define _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST                                       // 2016.02.04 / 민경승 / 적용중인 세트장비효과를 따로 저장시켜둔다
//#define _NA_008706_20160229_TEXTLOG_PK                                                      // 2016.02.29 / 민경승 / 감시대상 pk시 텍스트로그
// 민경승 끝
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━  우호경  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 우호경 시작
//#define _NA_008543_20151027_ONE_DAYS_GUILD_MISSION												// 2015.10.29 / 우호경 / 일일 길드 미션 기능 구현
//#define _NA_008647_20160115_NPCSHOP_CALCULATE_CHANGE												// 2016.01.15 / 우호경 / NPC 상점 계산 공식 변경
//#define _NA_008695_20160212_DRAGON_NIGHT_DRAGONTANS_BUG2											// 2016.02.15 / 우호경 / 드래곤나이트 용 변신(암드래곤트랜스폼10레벨) 스킬 사용 시 이속 능력치 적용 안되는 버그 수정
//#define _NA_008702_20160224_CHANGING_HIT_POPULATIONS												// 2016.02.24 / 우호경 / 타격 개체수 변경 요청(스타일코드 추가)
//#define _NA_008731_20160314_HP_SD_ADDITIONAL_CONDITIONS_INCREASE									// 2016.03.14 / 우호경 / 가해 데미지 HP/SD 전환 시 조건 추가 작업
//#define _NA_008765_20160405_DISCONNECTION_REASON_LOG												// 2016.04.05 / 우호경 / 컨넥션 끊겼을 경우 이유를 로그에 남기는 작업
//#define _CN_008758_20160329_FIELD_PVP_DROPITEM_TRADESELL_CHANGE									// 2015.03.29 / 우호경 / 중국 AC 보상 엘리트 아이템 전용 거래타입 추가(거래제한 + PK드랍 가능)
//#define _NA_008766_20160407_PROTECTIONSTATUS_MODIFICATION_ANOMALIES								// 2016.04.07 / 우호경 / 보호상태 이상현상 수정
// 우호경 끝
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//━━━━━━━━━━━━━━━━━━━  임상수  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 임상수 시작
//#define _NA_007128_20140326_MONDSCHIEN_DOMINATION            //2014.03.13 /임상수 /(몬트샤인) domination 스크립트 및 파서 수정, domination type 추가, 각인 성공시 턴 오버 추가, Map npc 오브젝트 체력회복
//#define _NA_000000_20140402_MODIFY_PERKMANAGER_POSSIBLE_TO_USE_WHILE_BATTLE_GROUND_RESERVATION //2014.04.02 /임상수 /전장예약 중에도 커스텀 패시브 스킬 변경이 가능하도록 수정.
//#define _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD      // 2014.08.11 / 임상수 / 몬트샤인 공성전 초기화 및 보상 지급 수정
// 임상수 끝
//━━━━━━━━━━━━━━━━━━━     끝      ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

//--------------------------------------------------------------------------------------------------
//! 2011.3.4 / i4u4me / 유명수 시작
//#define _YMS_FULLSETITEM_RESOURCE_DIVISION_ //! 2011.3.4 / i4u4me / 엘리트2 풀셋 파트 교환 형식에서 분리 

//#define _NA_000000_20120410_SPEEDHACK
//#define _NA_000000_20120416_COLLECT_OBTAIN_QUESTCODE
//#define _NA_000000_20120417_EQUIPMENTS_CALCULATOR_CHANGES_FOR_DEAD    //!< 죽었을때 능력치 올라가지 않게 함
//#define _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING                   //!< 날개 보임 여부
//#define _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER           //!< 유재원 미션내 채집 기능 트리거 추가
//#define _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND                  //!< 전장

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// arycoat :
//#define _HARDMODE_TRIGGER_PRINT_ // 하드모드 스크립트 작성참고용 트리거 분석 파일 생성기. - arycoat.2011.12.06
//#define _NA004034_20120102_GUILD_POINT_SYSTEM // 2012.02.07 / 정현성,이태준 / 길드 코인 작업
//#define _NA_003923_20120130_GUILD_RENEWAL // 2012.02.15 / 정현성,이태준 / 길드 리뉴얼
//#define _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM // 2012.02.15 / 정현성,이태준 / 길드창고 및 개인창고 슬롯개수 확장
//#define _NA_005988_20120901_VENDOR_SEARCH_SYSTEM // 2012.09.01 / 정현성,이도형 / 개인상점 검색 시스템
//#define _NA_006372_20130108_BADGE_ITEM // 2013.01.09 / 정현성 / 벳지 아이템
//#define _NA_006621_20130408_MAGICSHILD_MP_DECREASE // 2013.04.08. / 정현성 / 매직실드 MP소모량 변경
//#define _NA_000000_20130429_DOMINATION_PENALTY // 2013.04.29. / 정현성 / 지역점령전 패배 패널티 적용
//#define _NA_000000_20130521_DAILY_QUEST_RESET_ITEM // 2013.05.21. / 정현성 / 일일퀘스트 초기화 아이템
//#define _NA_000000_20140120_SMART_NPC_SYSTEM // 2014.01.20. / 정현성,송찬종 / 지능형NPC
//#define _NA_007086_20140318_MONSTERBOOK // 2014.03.18. // 정현성,송찬종 / 몬스터도감



//━━━━━━━━━━━━━ LAST SECTION(Shared) - Dependency Chain ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<SAMPLE>
//		#if defined( A_EX ) && !defined( A )
//			#error "Impossible Build!! - Reason: Violation Dependency Chain"
//		#endif
//
//	<EXPRESSION>
//		{ ∀(A_EX) | (A') ⊃ (A_EX) }   <--   A_EX 기능은 A기능에 종속적이다.
//		if (0) then IMPOSSIBLE,
//			(A') = (A) + (A_EX)
//			(A) = (A') - (A_EX)
//			(0) = (A_EX) - (A')
//		
//	<USEFUL SECTION>
//		1) A기능의 확장
//		2) A기능을 사용한 추가 기능
//

#if defined(_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL) && !defined(_NA_0_20100901_SKILLTREE_SYSTEM)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_004530_20120315_1202_SKILL_RENEWAL) && !defined(_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_0_20100528_ACCUMULATE_POINT_EVENT) && !defined(_NA_1419_20100121_ACCUMULATE_POINT)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_KR_0_20091021_FIRENDINVITATION_EXTEND) && !defined(_NA_0_20091104_FIRENDMAP_REFACTORYING)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_YJ_SHADOW_QUEST_S00023) && !defined(__NA_0S0023_20081114_SPAWN_MONSTER_QUEST)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif


#if defined(__NA_000597_20070821_GUILD_CORPS_SYSTEM) && !defined(__NA_000596_20070821_GUILD_CLASS_SYSTEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_PET_SAVE_TO_DB) && !defined(__NA_0_PET_SYSTEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_PET_FEED) && !defined(__NA_0_PET_SYSTEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined (__NA_000000_20070528_TAIWAN_GUILD_PVP) && !defined( __NA_001027_20080423_RANDOM_REWARD_EVENT_ )
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif


#if defined(__NA000844_080324_100LEVEL_CHAGNEUP_REBUILDING__)
	#if !(defined(__NA844896_080219_TASK_SSQ_AND_100LV_QUEST_INSTANCE_ZONE__) &&		\
		  defined(__NA844896_080219_TASK_SSQ_AND_100LV_QUEST_NPC_KILL_OPTIMIZER__))
		#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

#if defined(__NA001021_080421_PACKET_COMPRESS_V02__) && !defined(__NA000995_080402_SKILL_PROC_UPDATE_V01__)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#if defined(__NA000000_GRID_REBUIDING_FOR_READABLE_CODE__) && !defined(__NA001021_080421_PACKET_COMPRESS_V02__)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#if defined(__NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__)
	#if !defined(__NA000000_0806XX_CODE_REFINE_FOR_PERFORMANCE_TUNEUP_NO04__)	// 기타 추가할 것 찾아서 추가할 것.
		#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif
#if defined(__NA001051_080526_EVENT_EFFECT_RENDER_BY_SCRIPT_2ndPROCESS__) && !defined(__NA000000_GRID_REBUIDING_FOR_READABLE_CODE__)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL_V2__)
	#if !(	defined(__NA_0_20080219_DETECT_ABUSING_USER_REFACTORING)				\
			&& defined(__NA_0_20080219_DETECT_ABUSING_USER_POWERFUL)				\
			&& defined(__NA001083_080609_ABUSING_LOGICAL_DETECTION_ANAL_AND_IMPL__)	\
		)
		#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

#if defined(__NA_1119_CHANGE_ETHER_EXPEND_STYLE) && !defined(__NA_000968_ETHER_EMISSION_DEVICE)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_1247_POSSESSION_ITEM) && !defined(__NA_1131_PET_NAME_SETTING)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__) && \
    !defined(__NA_0S0023_20081114_SPAWN_MONSTER_QUEST)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_20081029_PARTY_REFACTORING)
    #if !defined(__NA_0_20081124_PARTY_REFACTORING_GAMESERVER)
      #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(__NA000000_090224_PARTY_RETURN_PROBLEM_CORRECTION__) && \
   !defined(__NA_0_20081029_PARTY_REFACTORING)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__CN_1159__LUCKY_ELITE_ITEM) && !defined(__CN_1194__ELITE_1_5_ITEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_ADD_GM_CMD_ABOUT_AC) && !defined(__NA_1288_AC_RENEWAL_FOR_TIME_LINE)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA001283_20090221_COLLECT) && !defined(__NA_1288_AC_RENEWAL_FOR_TIME_LINE)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__) && \
    !defined(__NA001226_INSTANCE_DUNGEON_EXTENTION__)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA1335_090407_PORTAL_MISSION_ROOM_CONSTRAINTS__) && \
    !defined(__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_DKJH0002_MODIFY_POSSESSION_ITEM_RANKUP) && !defined(__NA_1247_POSSESSION_ITEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__) && \
    !defined(__NA_1233_2009_NEW_YEARS_DAY_EVENT)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING) && !defined(__NA_0_20090818_SYSTEM_MEMO_SUPPORT)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_001359_20090619_HEIM_LOTTO_SYSTEM) \
    && ( !defined(__NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION) || !defined(__NA_0_20090818_SYSTEM_MEMO_SUPPORT) )
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#if defined(__KR_001355_20090624_GROUPINGSYSTEM_ALL) \
    && ( !defined(__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION) || !defined(__NA_0_20090818_SYSTEM_MEMO_SUPPORT) )
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_001369_20090705_ADD_QUEST_GRADE_MONSTERKILL) && !defined(__NA_0_20090407_QUESTSCRIPT_LOAD_MODIFY)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA000000_090718_SCRIPT_PARSING_SYSTEM_MODIFICATION2) && \
    !defined(__NA000000_090712_SCRIPT_PARSING_SYSTEM_MODIFICATION)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_20090813_BINARY_LOG_ERROR_RECOVERY) \
    && !defined(__NA_0_20090812_BINARY_LOGGING_IMPROVEMENT)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA_0_POSSESSION_ITEM_DROP) && !defined(__NA_1247_POSSESSION_ITEM)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__) && \
   !(defined(__NA000000_090906_SOLAR_SLOT_CODE_ARRANGEMENT__) && defined(_NA_20090925_ITEMSTREAM_INIT_RESERVED))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__) && \
   !(defined(__NA001390_090915_RIDING_SYSTEM__))// && defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP) && !defined(__NA_001359_20090619_HEIM_LOTTO_SYSTEM)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE) && !defined(__NA_001359_20090619_HEIM_LOTTO_SYSTEM)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_0_20100211_SD_EFFECT_MODIFY) && !defined(__NA_001290_20090525_SHIELD_SYSTEM)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_DH_GROUPSYSTEM_CHANGE_PACKET) && !defined(__KR_001355_20090624_GROUPINGSYSTEM_ALL)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_) && \
    !defined(_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif
#if defined(_NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_) && \
    !(defined(__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__) && \
      defined(_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

// NOTE: cross dependency '_NA002217_100728_EP2_SKILL_SYSTEM_DB_'
//                                         and
//                          'C_NA_0_20100520_ACSYSTEM_CHANGED'
#if defined(_NA002217_100728_EP2_SKILL_SYSTEM_DB_) && \
    !(defined(_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_) && \
      defined(C_NA_0_20100520_ACSYSTEM_CHANGED))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(C_NA_0_20100520_ACSYSTEM_CHANGED) && \
    !defined(_NA002217_100728_EP2_SKILL_SYSTEM_DB_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA002823_WINGS_EQUIPMENTS_EXTENTION) && \
    !defined(_NA002237_100802_EP2_SOCKET_SYSTEM_DB_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA000424_GLOBAL_CASH_ITEM_RENEWAL_) && \
    !(defined(_NA000424_GLOBAL_CASH_ITEM_RENEWAL_) && defined(__NA000000_SHOP_USERID_50_PASSING_PROC__))
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_20101011_HEIM_LOTTO_CASH_TICKET) && \
    ( !defined(__NA_001359_20090619_HEIM_LOTTO_SYSTEM) || defined(_NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE) )
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_) || \
    defined(_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_COMPOSITE_RULE_) || \
    defined(_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_) || \
    defined(_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM)
    //_NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_ (default enabled)
  #if !defined(_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
#endif

#if defined(_NA001202_101104_ITEM_CUSTOMIZING_CONTENTS_ETHEREALIZABLE_LUCKYITEM)
  #if !defined(_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_) || \
      !defined(_NA000960_101011_ITEM_CUSTOMIZING_CONTENTS_ADD_RULE_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
  #endif
#endif

#if defined(_NA_20101214_CHANGE_AUCTION_ITEM_CHECK_WAY)
    #if !defined(_NA_20100604_BETTER_DOMINATION_MANAGER) || \
        !defined(_CH_20100420_CHANGE_DOMINATION_APPLY_WAY)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_NA001605_EP2_DAMAGETYPE_EXTENTION_) && !defined(_NA001605_EP2_CHANGE_SKILL_BROADCASTER_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#ifdef _NA_001990_ACSYSTEM_ADD
	#if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED) && !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
	#endif
#endif

#ifdef _NA_001990_ACSYSTEM_ADD_INCREASE_INDEX
    #if !defined(_NA_001990_ACSYSTEM_ADD)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM) && \
   !defined(_NA001605_EP2_CHANGE_SKILL_BROADCASTER_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_20100322_AGGRO_DISPLAY) && !defined(_NA_0_20100819_AGGRO_RENEWAL)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA002479_20110401_OVERLAPPING_ITEM_DISTRIBUTION_CHANGES) && \
   !defined(_NA_0_20110318_ITEM_EXPIRATION_PERIOD)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN)
    #if !defined(_NA001385_20090924_DOMINATION) || \
        !defined(_NA_20100604_BETTER_DOMINATION_MANAGER) || \
        !defined(_CH_20100420_CHANGE_DOMINATION_APPLY_WAY)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_SCJ_MODIFY_OPTIONLIST_PARSER_SUFFIX_FILED) && \
    defined(_NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_NA_002935_20110704_ITEM_SCORE)
    #if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT) || \
        !defined(_NA_001990_ACSYSTEM_ADD)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_SCJ_ACSYSTEM_OVERLAP_PARENT) && !defined(_NA_001990_ACSYSTEM_ADD)
#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

#if defined(_SCJ_CHECK_SEND_GAMEOPTION_AUTOHUNTING_PACKET)
    #if !defined(_YMS_AUTOBOT_ITEM_KIND) || \
        !defined(_NA_0_20110329_GAMEOPTION_SETTING)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
    #endif
#endif

#if defined(_NA_003740_20111122_RANDOM_ITEM) && \
    !defined(_NA002237_100802_EP2_SOCKET_SYSTEM_DB_)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif