/**
 *	@file		GameDefine.h
 *	@brief		작업 디파인 모아놓은 곳
 *  @author		Unknown
 *	@version	Unknown
 */

////------------------------------------------------------------------------------
///**
//
//게임 전반에 관련된 define을 모아두는 곳
//
//@author
//	유재영 (y2jinc@webzen.co.kr)
//@since
//
//@remarks
//- 
//*/

#ifndef GAMEDEFINE_H
#define GAMEDEFINE_H

#include "GameDefine_KIKI.h"
#include "GameDefine_KJWS78.h"
#include "GameDefine_JJUMBO.h"
#include "GameDefine_Engine.h"
#include "GameDefine_YJ.h"
#include "GameDefine_K2P.h"
#include "GameDefine_GS.h"
#include "GameDefine_Mjkim.h"
#include "GameDefine_LTJ.h"
#include "GameDefine_DH.h"
#include "GameDefine_YMS.h"
#include "GameDefine_SCJ.h"
#include "GameDefine_HSH.h"

//////////////////////////////////////////////////////////////////////////
// 버전과 상관없는 클라 전용 디파인
//////////////////////////////////////////////////////////////////////////

#if defined(_DEBUG) || defined(_GAMECLIENT_VER) 
    //#define _YJ_EXECUTE_IN_MAPTOOL                  // 2010.07.26 / 윤영주 / 미션 테스트 환경 개선 작업
    #define _NA_006519_20130311_TEXTURE_VIEWER      // 2013.03.11 / 홍성현 / 클라이언트 텍스쳐 뷰어 제작 작업
#endif //_DEV_VER


//////////////////////////////////////////////////////////////////////////
// 삭제된 디파인 
//#define CUSTOMIZE_HEAD
//#define _JSH_BUGFIX__LEAVE_PARTY_					// 2006.04.20 / 마을에서 파티 탈퇴 명령이 적용되지 않던 버그
//#define _JSH_ADD__ENCRYPT_PROTOCOL_					// 2006.03.15 / 프로토콜 암호화
//#define _JSH_REFACTORING__WORLD_SERVER_CONNECTION_	// 2006.04.03 / 월드서버 연결 리팩토링
//#define _JSH_ADD__GAME_EXIT_STANDING_BY_			// 2006.04.12 / 캐릭터/서버선택 등 게임에서 나갈 때, 잠시동안 대기하는 기능
//#define _JSH_BUGFIX__LEAVE_FROM_PARTY_				// 2006.04.14 / 파티를 떠날 경우, 파티 여부를 먼저 체크
//#define _JSH_MODIFY__CHANGE_TRADE_PROTOCOL_			// 2006.04.14 / 플레이어간의 거래 : 수정된 프로토콜 적용
//#define _ADD_REWARD_SCRIPT							// 2006. 4.  7 / 유재영 / 보상 정보 관련
//#define _ADD_FIELD_SCENE				// 2006. 4. 11 / 유재영 / 필드씬 추가.
//#define _ADD_FIELD_IN_DEAD_PROCESS		// 2006. 4. 11 / 유재영 / 필드에서 죽었을때 처리
//#define _ADD_ITEM_EXCHANGE				// 2006. 4. 14 / 유재영 / 아이템 1:1 교환 (상점에서 아이템 팔때)
//#define _REMOVE_LOCK_KEYBOARD			// 2006. 4. 18 / 유재영 / 키보드 락킹 플레그 제거
//#define _ADD_ITEM_DROP_DELETE_FOR_NOT_DROP_ITEM	// 2006. 4. 26 / 유재영 / 아이템 드롭할수 없는 아이템 삭제.
//#define UI_RESOLUTION_VARIATION			// 2006. 2. 15 / 박준태 / UI 해상도별 사이즈 변환
//#define MINIMAP_POS_CHANGE
//#define NEW_ENGINE
//#define BLOCKED_TEXT                    // 2006. 2. 17 / 박준태 / 캐릭터 이름등에 검은 삼각형을 덧댐
//#define PLAYER_HIT_DELTA_ANIMATION		// 2006. 3. 22. 캐릭터 데미지 애니메이션을 델타 애니로 처리 
//#define MAPOBJECT_USE_WITH_ACTION       // Map오브젝트의 사용도 Action으로 처리
//#define CRITICAL_HIT_ANIMATION
//#define MONSTER_RADIUS					// 몬스터 둘레 관련 처리
//#define AREA_CONQUEST_NEW_PROJECT
//#define UI_AC_DIALOG

//#define MOD_NETWORK_HEARTBEAT						// 2006. 12. 09 / 전보환 + 이은파 작업
//#define _JBH_ADD_AGENT_ENCRYPT_PROTOCOL
//#define _ADD_CHARGE_ITEM_REPAIR_ASK					// 2006. 12. 18 / 유재영 / 수리 아이템 사용시 의견 물어보는 메시지 추가.
//#define _JBH_ADD_OPTION_CAMERA_BASE_MOVE			// 2006.2.23    / 전보환 / 카메라 기준 이동. 
//#define _JBH_ADD_OPTION_CAMERA_BASE_MOVE_CALL_FUNCTION
//#define _JBH_MOD_ARMOR_KIT				// 2006.4.20	/ 전보환 / 아머킷 4.11일자 적용
//#define _124_KIKI_CASH_ITEM_SHOP					// 2006. 11. 21 / 이지연 / 유료 아이템몰
//
//#define _ADD_QUEST_COMMON_FORWARD		// 2006. 3. 30 / 유재영 / 퀘스트 임무 수락 목록 표시 및 일반 NPC 대화시 이용되는 UI
//#define _ADD_QUEST_REWARD				// 2006. 3. 30 / 유재영 / 퀘스트 임무 수락, 진행, 보상을 받는 UI
//#define _ADD_QUEST_LIST_CONTENT			// 2006. 3. 30 / 유재영 / 유저의 요청에 의한 퀘스트의 내용을 표시.
//#define _ADD_QUEST_LIST_MISSION			// 2006. 3. 30 / 유재영 / 유저의 요청에 의한 퀘스트의 임무를 표시.
//#define _ADD_QUEST_MISSION_LIST			// 2006. 3. 30 / 유재영 / 미션정보 표시
//#define _ADD_QUEST_TIME_CHECK			// 2006. 4. 19 / 유재영 / 퀘스트 타임 체크
//#define _ADD_QUEST_AREA_CHECK			// 2006. 4. 19 / 유재영 / 퀘스트 영역 체크
//#define _ADD_QUEST_UNION				// 2006. 4. 19 / 유재영 / 연합퀘스트
//#define _ADD_QUEST_NPC_MARK_IN_RADAR	// 2006. 4. 25 / 유재영 / 퀘스트에 따른 NPC 를 레이더 표시 관련.
//#define _ADD_QUEST_CANCEL_BUTTON		// 2006. 5. 19 / 유재영 / 퀘스트 퀘스트 포기 버튼 추가.
//
//#define _ADD_QUEST						// 2006. 3. 28 / 유재영 / Quest 
//#define _ADD_OPEN_FORWARD_NPC_MEET		// 2006. 3. 30 / 유재영 / NPC 만났을 때 퀘스트 포워드 창 띄우기.
//#define _ADD_QUEST_AUTO_ACCEPT			// 2006. 5. 19 / 유재영 / 퀘스트 자동 수락
//#define _ADD_QUEST_AUTO_ACCEPT_REQUEST  // 2006. 8.  3 / 유재영 / 퀘스트 자동 수락 요청(NPC 없이)
//#define _ADD_QUEST_AUTO_REWARD_REQUEST  // 2006. 8.  3 / 유재영 / 퀘스트 자동 완료보상 요청(NPC 없이)
//#define _ADD_QUEST_CHAO_QUEST			// 2006. 10.30 / 유재영 / 퀘스트 카오시 카오시간 감축 퀘스트 추가.
//#define _ADD_QUEST_VIEW_PROGRESS_QUEST	// 2007.  1.15 / 유재영 / 퀘스트 진행사항 표시.

//#define _ADD_CHARGE_ITEM_SAVE_POSITION_SCROLL	// 2006. 11. 20 / 유재영 / 유료화 아이템 위치 저장 포탈 스크롤 작업.
//#define _ADD_CHARGE_ITEM_EXTEND_INVENTORY		// 2006. 11. 28 / 유재영 / 인벤토리 확장 작업.
//#define PCBANG_ITEM_TOOTIP_PRESENT					// 2006. 12.  9 / 전보환 / PC방 아이템 옵션 출력.
//#define _ADD_CHARGE_ITEM
//#define AUTOTARGET_OPTION					// 2006.2.21	/ 전보환 / 적 공격시 오토타겟 옵션화. 
//#define FIXTARGET_OPTION					// 2006.2.21	/ 전보환 / 탭키 눌를때마다 타겟이 로테이션 되지 않고 고정타겟. 옵션화.
//#define FOG_RATE_CHANGE						// 2006.2.15	/ 전보환 /  포그 weight 값 설정.    
//#define _JBH_ADD_OPTION_SCROLL_ROTATION_SPEED // 2006. 2.23	/ 전보환 / 카메라 회전 속도 스크롤바 옵션.
//#define _JBH_ADD_OPTION_SCROLL_ROTATION_SPEED_CALL_FUNCTION
//#define _JBH_ADD_OPTION_SCROLL_MOUSE_SPEED	// 2006. 2.24	/ 전보환 / 마우스이동 감도조절스크롤바 옵션
//#define _JBH_ADD_OPTION_FORWARD_TARGETING	// 2006. 2.24	/ 전보환 / 전방 타게팅 옵션.
//#define _JBH_ADD_OPTION_FORWARD_TARGETING_CALL_FUNCTION
//#define _JBH_MOD_CAMERA_DISTANCE			// 2006.3.16	/ 카메라와 케릭터 기본 거리 수정.

//#define _JBH_MOD_MOUSE_ACTION_CHANGE		// 2006.3.16	/ 마우스 좌우 버튼 변경.	
//#define _JBH_MOD_OPTION_DIALOG				// 2006.3.30	/ 옵션 게임 다이얼 로그 수정.
//#define _JBH_MOD_CHARACTERSCENE_NOT_MOUSE_CHANGE // 4.13	/ 전보환 / 캐릭터씬에서는 마우스 좌우변경 하지 않게 수정.
//#define	_JBH_ADD_YOUNGMAN_PROTECT_MESSAGE	// 2006.4.10	/ 전보환 / 로딩시 청소년 보호 메세지 출력.
//#define _JBH_MOD_CAMERA_INTERPORATION		// 2006.4.15	/ 전보환 / 마우스 회전시 카메라가 보간되어 Hero뒤로 오던것 삭제.
//#define	_JBH_ADD_NPC_SCALE					// 2006.4.21	/ 전보환 / NPC 스케일값 스크립트에서 읽어와서 처리.
//#define _JBH_ADD_FIELD_AREA_INFO_MESSAGE  // 2006.4.22	/ 전보환 / 필드 입장시 지역 이름 정보 추가.
//#define _JBH_SPAWN_MONSTER_DIRECTION		// 2006.4.26	/ 전보환 / 스폰시 특정 몬스터 방향 설정.
//#define _JBH_ADD_TIRED_SYSTEM_DIRECTING		// 2006.3. 9	/ 전보환 / 피로도 시스템 연출

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 국내 테섭 관련 디파인
//////////////////////////////////////////////////////////////////////////
//#ifdef LAST_TEST_RELEASE
//	#define _SHOW_HEIM_IN_TESTLIVE_CASH_ITEM_SHOP		// 2006. 12. 14 / 이지연 / 테섭에선 유료아이템몰에 하임만 표시할것	(==> 국내 테섭에만 사용하던 디파인인데, 0813 이후 사용안함)
//#endif


//////////////////////////////////////////////////////////////////////////

#if defined ( _DEV_VER )
	// 주석 처리 하지 말아주세요!!
	// 주석처리 안해도 루아 콘솔이 뜨지않습니다.
	// 루아 콘솔 띄울려면 클라이언트 위키 
	// '테스트 케이스 사용법'을 확인하시기 바랍니다.
	#define _USE_LUA_SCRIPT		
#endif

//_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
// 루아 활성화
//#define _USE_LUA_SCRIPT

//////////////////////////////////////////////////////////////////////////
//
//			한게임 채널링 작업
//
//////////////////////////////////////////////////////////////////////////
#if defined(__KR000000_090128_CHANNELING__)
	// 03/26 - 채널링 오픈
	#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__				// 새로운 서버선택 기능 <<- 채널링에 우선 적용 후 전체 버전 적용
		// 서버 공통 디파인 사용
	#else
		#define _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING				// 서버 선택 및 재접속 제거
	#endif	
	#undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020				// undefine 캐릭터선택 창 서버선택 버튼
	#define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// 서버 리스트창의 닫기버튼 삭제( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING 디파인에서 분리(09.04.28) )
	#define _MOD_IDENTITY_NUM__FOR_CHANNELING						// 주민번호 입력제한 6자리로 변경( 7->6 )
	#define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// 로그인 씬(서버선택화면)에서 가입하기 버튼 삭제

	#if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        #define _COMMON_CHANNELING_LOGIC_DEFINE                     // 채널링 로직 관련 공통 디파인
		#define _APPLY_KOREA_CHANNELING                             // 국내 웹로그인 및 웹스타터 적용, 인증프로세스 리팩토링
	#endif	
#endif

//////////////////////////////////////////////////////////////////////////
//
//			글로벌 채널링 작업
//
//////////////////////////////////////////////////////////////////////////
#if defined(__NA000000_GLOBAL_AUTH_PROCESS__)
	
	#undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020					// undefine 캐릭터선택 창 서버선택 버튼
	#define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// 서버 리스트창의 닫기버튼 삭제( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING 디파인에서 분리(09.04.28) )
	#define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// 로그인 씬(서버선택화면)에서 가입하기 버튼 삭제

	#if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        #define _COMMON_CHANNELING_LOGIC_DEFINE                     // 채널링 로직 관련 공통 디파인
		#define _APPLY_KOREA_CHANNELING					            // 웹로그인 및 웹스타터 적용, 인증프로세스 리팩토링
		#define _APPLY_GLOBAL_CHANNELING							// 글로벌 채널링 인증 플로우 작업 -  국내 채널링 작업에 기반하여 추가 작업
	#endif	
#endif


//////////////////////////////////////////////////////////////////////////
//
//			북미(NHN_USA)채널링관련 ACM초기화및 보고모듈 작업
//
//////////////////////////////////////////////////////////////////////////
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
	#undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020					// undefine 캐릭터선택 창 서버선택 버튼
	#define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// 서버 리스트창의 닫기버튼 삭제( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING 디파인에서 분리(09.04.28) )
	#define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// 로그인 씬(서버선택화면)에서 가입하기 버튼 삭제
	#if defined(_SUNGAME_DUMP_VER) || defined(_SUNGAME_VER)
        #define _COMMON_CHANNELING_LOGIC_DEFINE                     // 채널링 로직 관련 공통 디파인
        #define _NHN_USA_CHANNELING						
	#endif	
#endif//__NA000000_NORTH_AMERICA_HANGAME_CHANNELING__(북미채널링)

//////////////////////////////////////////////////////////////////////////
//
//			일본 - 게임츄 채널링관련 작업
//
//////////////////////////////////////////////////////////////////////////
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    #undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020                    // undefine 캐릭터선택 창 서버선택 버튼
    #define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// 서버 리스트창의 닫기버튼 삭제( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING 디파인에서 분리(09.04.28) )
    #define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// 로그인 씬(서버선택화면)에서 가입하기 버튼 삭제
    #define _JAPAN_GAMECHU_SUPPORT_DOUBLE_AUTH_FLOW_                // 개발망테스트를 위해 2가지 인증방식 지원(삭제할 예정이었으나 개발망에서 팔요하여 적용함)
    #define _APPLY_JAPAN_GAMECHU_CHANNELING                         // 일본 게임츄 채널링 작업 디파인    
#endif //_JP_0_20100723_GAMECHU_AUTHFLOW_(일본게임츄채널링)

//////////////////////////////////////////////////////////////////////////
//
//			러시아 - 인감바 채널링관련 작업
//
//////////////////////////////////////////////////////////////////////////
#ifdef _RU_INGAMBA_AUTH_PROCESS
    #undef _K2P_ADD_MOVE_TO_SERVER_BUTTON_001020                    // undefine 캐릭터선택 창 서버선택 버튼
    #define _REMOVE_SERVER_LIST_CLOSE_BUTTON_FOR_CHANNELING			// 서버 리스트창의 닫기버튼 삭제( _REMOVE_TO_SERVER_BUTTON_FOR_CHANNELING 디파인에서 분리(09.04.28) )
    #define _REMOVE_REGISTER_BUTTON_FOR_CHANNELING					// 로그인 씬(서버선택화면)에서 가입하기 버튼 삭제
    #define _APPLY_RUSSIA_INGAMBA_CHANNELING                        // 러시아 인감바 채널링 작업 디파인    
#endif //_RU_INGAMBA_AUTH_PROCESS(러시아인감바채널링)

//////////////////////////////////////////////////////////////////////////
//
// 클라이언트 공동 디파인
//
//////////////////////////////////////////////////////////////////////////

#ifndef _DEBUG
	#if !defined( _CREATE_DUMP_ )	// Full Dump
		#define GGUO_CRASH_DUMP     // Mini Dump             
	#endif
#endif


#define AK_FOR_SUN									//	아머킷 2.0 이후 버젼에서 디파인 필요

//////////////////////////////////////////////////////////////////////////
// 길드 관련
//////////////////////////////////////////////////////////////////////////
/*($Delete$)*/#define __MOD_GUILDMEMBER_REFERENCE				//08.01.15 GuildMain 에서 GuildMeber Reference 잘못하던 부분 수정.


//////////////////////////////////////////////////////////////////////////
//
// 유재영 작업 디파인
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 작업 완료 
//#define _ZONE_TEST_LOBBY_IN_SINGLE	// 싱글 버전상에서 테스트를 위한 처리
//#define _ADD_CHARGE_ITEM_RANDOM_OPTION			// 2006. 11. 27 / 유재영 / 랜덤 옵션 추가 아이템 작업.
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//
// 전보환 작업 디파인
//
//////////////////////////////////////////////////////////////////////////

///////////////   [ 완      료 ] ///////////////////////////////////////////////////////////////// 

//#define CAMERA_SMOOTH_DELETE						// 2006.2.21	/ 전보환 / 카메라 스무스한 이동 삭제.
//#define	_JBH_MOD_MOUSE_MBUTTON_IN_BATTLESCENE	//   4.17	/ 전보환 / 마우스 M버튼 클릭시 카메라 방향으로 향하도록 수정.

//////////////    [     ETC     ] ////////////////////////////////////////				
#ifndef _CHINA // 중국은 하임 자동줍기 안함
	#define _ADD_CHARGE_ITEM_AUTO_GET_HAIM				// 2006. 11. 27 / 전보환 / 하임 자동줍기
#endif


#ifdef _CHINA
	#define _JBH_ADD_ITEM_OVER_PLAYING_TIME_MESSAGE // 2006.3. 7	/ 전보환 / [중국판호획득용] 중독방지 시스템에의한 아이템획득 금지 메세지.							
#endif






//////////////////////////////////////////////////////////////////////////
//
// 김지완 작업 디파인
//
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// 기타
//////////////////////////////////////////////////////////////////////////

// 테스트 중 (검증되지 않은 내용)////////////////////////////////////////////////////////////////
//#define _JSH_ADD__PLAY_MOVIE_BEFORE_MISSION_		// 2006.02.23 / 미션시작전 동영상 출력 ( 현재는 테스트 단계 )
//#define VIBRATE_CHARACTER             // 2006. 2. 17 / 박준태 / 피격시 캐릭터 떨림
//#define HAIR_COLOR_VARIATION          // 2006 .2 .17 / 박준태 / 머리카락 색상 변경~
//#define MAPOBJECT_INSTANCING


//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
	//#	define _CHECK_MEMORY_OVERRUN					// 2006. 12. 27 / 유재영 / 메모리 오버런 의심나는 부분에 해당 함수를 기입.
#endif

#ifdef _CHECK_MEMORY_OVERRUN
	#define CRT_CHECK_INTERRUPT()	if ( !_CrtCheckMemory() ) _asm {int 3};		// 의심나는 부분에 해당 함수를 앞뒤로 넣는다.
#else
	#define CRT_CHECK_INTERRUPT()	
#endif



//////////////////////////////////////////////////////////////////////////
// 테스트 관련 플레그
#define INTERFACE_DIALOG_TEST			FALSE		// Wz Dialog 렌더시 TRUE 이면 Dialog하나씩 Render 한다.
#define ITEM_COMPOSITE_APPLY			TRUE		// 아이템 조합 

//////////////////////////////////////////////////////////////////////////
// 전투관련
#define ATTACK_STAND_TIME				5000


//////////////////////////////////////////////////////////////////////////
//게임
#define TUTO_FIELD 20206
#define TUTO_MISSION 31

#define MAX_ITEM_INFO					1000
#define MAX_NPC_INFO					1000
#define MAX_SKILL_INFO					1000
#define MAX_SKILL_INVENTORY_STORE_INFO  1000
#define MAX_STYLE_INFO					1000
#define MAX_MAP_INFO					100
#define MAX_SHOP_INFO					1000
#define MAX_ITEM_COMPOSITE_INFO			100
#define MAX_ITEM_CRYSTALIZE_INFO		50
#define MAX_ITEM_COMPOSITE_NPC_INFO		50
#define MAX_SOCKET_DESC_INFO			50
#define MAX_GRAPHIC_INFO				10000
#define MAX_DIALOG						100
#define	MAX_BGM							100
#define MAX_STYLE_QUICK_REGISTER		100
#define MAX_ITEM_SOUND_INFO				50
#define MAX_NPC_PORTAL_INFO				50
#define MAX_ENCHANT_INFO				100
#define MAX_SKILL_TEXT_INFO				1000
#define MAX_MISSION_MESSAGE_LENGTH		2048
#define MAX_MESSAGE_LENGTH				1024
#define MAX_HELP_INFO					100
#define MAX_QUEST_TEXT_INFO				50000
#define MAX_MISSION_REWARD_INFO			100
#define MAX_ATTACH_ITEM_INFO            1000
#define MAX_EXPVALUE_INFO				100
#define GROUP_ID_NONE					0
#define MAX_MISSION_TEXT_INFO			500

#ifdef	_JAPAN
#define MAX_SSN_COUNT					8
#else
	#ifdef _MOD_IDENTITY_NUM__FOR_CHANNELING
		#define MAX_SSN_COUNT					6
	#else
		#define MAX_SSN_COUNT					7
	#endif
#endif

#define MAX_ICON_IMAGE					50000

#define DISTANCE_NPC_MEET_EXT			0.70f	// NPC를 만날 수 없는 경우, DISTANCE_NPC_MEET 의 70% 거리까지 다가가기 위한 값
#define DISTANCE_TAB_TARGET_MONSTER		30.0f	// TAB으로 고를 수 있는 몬스터 거리 30 미터
#define DISTANCE_TRADING                6.0f    // 6 미터이내에서만이 거래가 가능하다.
#define DISTANCE_FOLLOW_PLAYER			3.0f	// 플레이어 캐릭터를 따라다닐 때의 거리

#define NONE_ID							0xFFFFFFFF

// 시간 관련
#define SEC_PER_TICK					1000					// 1초당 틱 
#define MIN_PER_TICK					SEC_PER_TICK * 60		// 1분당 틱 
#define HOUR_PER_TICK					MIN_PER_TICK * 60		// 1시간당 틱 
#define DAY_PER_TICK					HOUR_PER_TICK * 24		// 1일당 틱
#define NO_COOLTIME						//쿨타임 체크 끄기
#define DELAY_LEAVE_FIELD_TICK			10*SEC_PER_TICK			// 10초
#define DEALY_SPAWN_VILLAGE             5*SEC_PER_TICK			// 10초
// PVP 관련
#define MAX_TEAM						2

// font 관련 define

// 툴팁 관련
#ifdef _SCJ_SETITEM_TOOLTIP_REFORM
   #define MAX_SLOTINFO_LINE				70
#else
   #define MAX_SLOTINFO_LINE				60
#endif //_SCJ_SETITEM_TOOLTIP_REFORM
#define MAX_SLOTINFO_LENGTH				256

#define MAX_TEMP_VARIABLE				20

// 퀘스트 관련
#define MAX_QUEST_AREA_ID				1000

#define HEIGHT_GAP                      0.3f
// 컬러 관련


#define DEFAULT_FONT_WZCOLOR			WzColor_RGBA(165,165,165,255)
#define TOOLTIP_FONT_WZCOLOR			DEFAULT_FONT_WZCOLOR
#define TOOLTIP_BG_WZCOLOR				WzColor_RGBA(0,0,0,200)
#define TOOLTIP_SIDE_LINE_WZCOLOR		WzColor_RGBA(119,119,119,255)		// 외각선

#define TOOLTIP_OPTION_NAME_WZCOLOR		DEFAULT_FONT_WZCOLOR

#define REVENGE_STONE	7243

#define SSQ_PORTAL		61009
//------------------------------------------------------------------------------
/**
    게임서버목록의 최대목록 갯수 (used in GameServerListParser)
*/
const int c_iMAX_GAMESERVERLIST_PARSER  = 50;

using namespace util;

class Quest;

// typedefs 
typedef DWORD							RESOURCECODE;       ///< 그래픽 리소스
typedef WORD							KEYSETCODE;			///< DInput CODE 
typedef util::_tstring					SUN_STRING;			///< 
typedef std::basic_string<TCHAR>::size_type	STR_INDEX;			/// 
#ifdef _NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX
typedef	WORD QCODE;
typedef std::list<QCODE>				QUEST_LIST;
typedef std::list<QCODE>::iterator		QUEST_LIST_ITER;
#else
typedef std::list<Quest *>				QUEST_LIST;
typedef std::list<Quest*>::iterator		QUEST_LIST_ITER;
#endif //_NA_007060_20140210_DIFFERENT_QUEST_WINDOW_OPENBUG_FIX

//---------------------------------------------------------------------------------------
//		다이나믹 윈도우 아이디 대역폭
//---------------------------------------------------------------------------------------
#define DYNAMIC_WINDOW_ID_MIN 1500
#define DYNAMIC_WINDOW_ID_MAX 1600

#define RANDOM_ITEM_CODE			  65535


#define WzIdToConst(l1,l2,l3,l4) ((l1) * 0x01000000 + (l2) * 0x00010000 + (l3) * 0x00000100 + (l4) * 0x00000001)

#define EFFECT_HIGHT  4000
#define EFFECT_MIDDLE 500
#define EFFECT_LOW    50

#endif 
// GAMEDEFINE_H