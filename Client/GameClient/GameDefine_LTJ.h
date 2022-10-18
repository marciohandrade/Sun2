#pragma once
//////////////////////////////////////////////////////////////////////////
//		이태준 작업 디파인
//////////////////////////////////////////////////////////////////////////
//#define __NA001164_20081024_CHAR_WAY_POINT
//#define __001193__20081107_LEVEL_COLOR_MODIFY__
//#define __NA_1260_CHANGE_LEVEL_FOR_STAT_INIT
//#define __001251_MATERIAL_ITEM_COMPOSE_LIST
//#define __NA_01240_GM_CMD_ADD_SERVER_TIME
//#define __NA_1247_POSSESSION_ITEM
//#define __001268_CHANGEUP_SKILL_TOOLTIP
//#define __NA_1288_AC_RENEWAL_FOR_TIME_LINE
//#define __001300_MOD_VENDOR_HYME_COLOR
//#define __LTJ_MOD_DIALOG_DISABLE_RECT
//#define __001285_LTJ_MOD_CREATE_CHAR_DESCRIPTION
//#define __LTJ_REFORM_CHAR_DIALOG_UPBUTTON_HIDE
//#define __001314_LTJ_ADD_CLOCK_OPTION_TIRED_SYSTEM
//#define __LTJ_REFORM_DROP_ITEM_NOTIFY_TIMELINE
//#define __LTJ_DISPLAY_FULL_INVENTORY_MESSAGE
//#define _LTJ_AC_REMOVE_CORRUPT_TEMPLE_ADD_MAIN_QUEST
//#define __LTJ_DRAW_NEW_SERVER_ICON_SERVER_LIST
//#define __001326_LTJ_LEVEL_COLOR_MODIFY_2ND
//#define __LTJ_CHAT_DIALOG_SCROLL_BUGFIX
//#define __LTJ_EVENT_TIMER_MODE_MODIFY
//#define __090512_LTJ_ETHER_CHARGE_JP_BUGFIX
//#define __001333_LTJ_QUEST_LIST_UI_REFORM
//#define __001331_LTJ_AC_TREASUREBOX_TOOLTIP
//#define __001341_LTJ_POSSESSION_ITEM_MODIFY
//#define __001345_LTJ_QUEST_REPEAT_DISPLAY
//#define __001347_LTJ_REMOVE_TEMP_INVENTORY			            // 보조 창고 제거
//#define __LTJ_RADAR_MAP_NAME_BUGFIX
//#define __LTJ_LOAD_SCENE_BLOCK_NETWORK_UPDATE			            //
//#define __LTJ_CHAT_DIALOG_POPUP_MENU_RCLICK_BUGFIX	            // 
//#define __LTJ_HIDE_PET_NAME_JP						            //일본
//#define __LTJ_TOOLTIP_KALBUSRING_EQUIPCLASS			            //일본
//#define __LTJ_GSP_HIDE_CASHSHOP_PCCAFE_BUTTON			            //북미 캐시샵 피시방버튼 숨김
//#define __LTJ_MOD_WASTE_BASKET_DIALOG					            //디파인삭제/적용
//#define __090907_LTJ_GRAPHIC_OPTION_ADD
//#define __090907_LTJ_GAME_OPTION_ADD
//#define __090909_LTJ_CHATDIALOG_MOD
//#define __LTJ_UI_DIALOG_TEXTURE_LOAD_N_RELEASE_MOD	            // 다이얼로그 테스트 로드 및 해제 변경 - WZENGINEVER 101 이상
//발키리 무기 교체 버그 테스트 코드
//#define _LTJ_HERO_INFO_BUGFIX
//#define __KR_001385_20090907_ZONE_OCCUPATIONWAR_PACKET	        //점령전UI 관련 패킷
//#ifdef __KR_001385_20090907_DOMINATION_PACKET
//	#define __001385_LTJ_RENEWAL_NOTICE						        //공지사항 리뉴얼
//	#define __001385_LTJ_ADD_DOMINATIONWAR_TOWER_UI			        //점령타워UI
//#endif
//#define _091229_LTJ_AC_REWARD_HELPMESSAGE_BUGFIX
//#define _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_	        // 안전지대타일속성 추가 및 기존코드 정리겸 디파인(서버디파인으로 통일)
//#define _100105_LTJ_ADD_SSQ_OBVERVER_ASSIST_DIALOG		        // 타락한 사원 관전모드창을 Assist창으로 구현
//#define __LTJ_DISPLAY_ITEM_PICK_NAK_FULLINVENTORY		            // 퀘스트 결과코드 테스트 필요
//#define __LTJ_RELATION_OF_PLAYER_BUGFIX
//#define _LTJ_ADD_SAFEZONE_TILETYPE	                            // 안전지대타일속성 추가 및 기존코드 정리겸 디파인
//#define _100114_LTJ_USE_AMBIENTSOUND_SCRIPT_FOR_AMBIENT_SOUND     // 환경사운드 스크립트제어로 변경작업
//#define _100114_LTJ_JAPAN_HALFWIDTH_CHARACTER_BUGFIX              // 일본어 반각문자처리 버그 수정(1바이트반각문자를 2바이트로 처리하고 있었음. 떄문에 스페이스체크가 패스됨)
//#define _NA_1419_20100121_ACCUMULATE_POINT
//#define _100216_LTJ_NOTICE_BUGFIX
//#define _NA_1419_20100121_PLAYTIME_MILEAGE // 디파인명 변경전
//#define _100408_LTJ_SEPARATE_TIMER_INI                            // PROGRAM.INI 에서 TIMER.INI로 TIMER 관련 부분을 분리
                                                                    // #define __001314_LTJ_ADD_CLOCK_OPTION_TIRED_SYSTEM
                                                                    // #define __LTJ_EVENT_TIMER_MODE_MODIFY
                                                                    // 위 2개 디파인제거
//#define _100416_LTJ_MODIFY_CHAT_FILTERING                         // 채팅 금칙어 필터링 변경
//#define _100506_LTJ_MOD_STATUS_BUFF_IMAGE_RENDER                  // 상태버프 아이콘 텍스쳐통합 작업
//#define  _NA_20100604_BETTER_DOMINATION_MANAGER                   // 점령전 관리기능 보완 작업
//#define  _100705_LTJ_CONTROL_LOD_DISTANCE_PLAYER_COUNT            // 플레이어 카운트에 따라 LOD 카메라 거리 조절
//#define  _100708_LTJ_SHOW_DATETIME_IN_TIMER                       // 타이머(피로도)에 날짜도 표시
//#define _APPLY_JAPAN_GAMECHU_CHANNELING                           // 일본 게임츄 채널링 작업
//#define _TEMP_DEFINE_REMOVE_CHARACTER_SCENE_MONSTER               // 캐릭터생성시 몬스터생성 임시 블럭
//#define _HACKSHIELD_CHECK_TIMER_API_FUNCTION_                     // 핵쉴드지원 타이머API 관련 유효성 체크
//#define  _NA_1937_20100207_NPC_CUSTOMIZING                        // NPC커스터마이징 지원
//#define _TEMP_DEFINE_CHARACTER_SELECT_POSITION_EDIT               
//#define _NA_2088_20100317_MODIFY_QUEST_AND_AC_LIST_ORDER          // 퀘스트/AC 리스트 정렬 및 출력 변경
//#define _NA_2088_20100322_ADD_OPTIONLIST_PARSER_RATIOVALUE_FILED  // OptionList 스크립트에 RatioValue 필드 추가 작업(옵션값이 정수타입에도 의미에 따라 %를 붙여 출력할 수 있도록)

//////////////////////////////////////////////////////////////////////////
//	패킷 카테고리 및 프로토콜 보기
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG //#ifdef _LTJ_PACKET_SHOW_FOR_DEBUG
	#include "../../include/CPShowMSG.h"
	#pragma comment( lib, "../../lib/CPShowLIB_d.lib")
	#pragma comment( linker, "/NODEFAULTLIB:LIBCD" )
	extern CPShowFNTable* g_pPacketShow;
#endif //_LTJ_PACKET_SHOW_FOR_DEBUG
//////////////////////////////////////////////////////////////////////////
//	핵쉴드 테스트 디파인
//	Release(유저배포버전)으로 빌드
//////////////////////////////////////////////////////////////////////////
//#define _LTJ_HACKSHEILD_UPDATE_TEST
#ifdef _LTJ_HACKSHEILD_UPDATE_TEST
	#define __NA000000_070913_HACKSHIELD_INSTALLATION__
	#undef _AHN_SMART_UPDATE_
	#undef __KR000000_090128_CHANNELING__	
#endif //_LTJ_HACKSHEILD_UPDATE_TEST_
//////////////////////////////////////////////////////////////////////////