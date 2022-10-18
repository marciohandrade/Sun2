#pragma once

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// 버전에 관계없이 해당 국가에만 들어가는 디파인을 정의하는 곳이다.
//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

#if defined(_CHINA)
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //    ▶ CHINA 중국 버전 ◀
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	#define __CN_000352_20070319_NORMAL_SHOUT_BLOCK				// [352] / 2007. 03. 19 / 조재식 / 중국 일반 외치기 기능 블럭.
//	#define	__CN_OO0510__FATIGUE_MATCHING_UPDATE				// [510] / 2007. 06. 04 / 조재식 / 피로도 시스템 계정단위로 저장
	#define __CN_000417_20070416_WORLD_SHOUT_CONSUME			// 2007.04.16 / 황성익 / 중국_해방군 포고문 1회 사용
	/*($Delete_[S/C]$)*/#define	__NA_000614_GUILD_RANK_REFACTORING					// 2007. 08. 09 / 조재식 / 길드 랭크업 시스템 리팩토링( 중국 길드 랭크업 적용위한 작업 )
	#define __NA_000632_20070817_CHINA_FATIGUE_HEIM				// 2007. 08. 17 / 조재식 / 중국 피로도 하임픽업률을 하임획득량으로 수정.
	#define __CN_0_20070507_SHOP_MILEAGE						// [0] / 2007. 05. 07 / 김민욱,김태우 / 마일리지 작업, 해방군의 훈장, 중국작업이며 추후 국내 도입
	#define __NA_00387_20070605_SHOP_MILEAGE2					// [387]	/	2007. 06. 05	/ 김민욱, 김태우 / [부분 유료화] 마일리지 시스템 2차, 마일리지로 아이템 조회 및 구입
	#define __NA000000_070523_REPORT_TO_SOLARAUTH				// 2007/05/23 동접,캐시아이템 구매 정보 Master로, Master는 SolarAuth로...
	#define _K2P_PRINT_ONCE_MESSAGE								// 피로도 관련 메시지 처음 로긴시만 보여주기
	#define	__CHINA_BLOCK_ITEM_SHOP_PRESENT						// 아이템샾 선물하기 블록
	#define __CHINA_BLOCK_PC_BANG_SHOP_ITEM						// 아이템샾에서 PC방 아이템 블락
	#define __CHINA_PCBANG_SUPER_ROOM_BLOCK						// PC방 슈퍼방 블록
	#define __CHINA_HIDE_PCBANG_ITEM_OPTION						//  2007.05.21		/ 전보환 / 아이템 옵션중 PC방 옵션은 중국에서 안보이도록 	
	#define _JBH_ADD_AGE_LIMIT_MARK
	#define _GS_ADD_PWD_X_BTN_1143								// 08.08.13 / 김광수 / 중국창고비번에x버튼추가
	/*($Delete_[S/C]$)*/#define	__NA_000000_20071120_FATIGUE_AC_RANKING				// 2007.11.20 / 조재식 / AC보상 및 미션랭킹 보상 피로도 적용. ==> 1
	/*($Delete_[S/C]$)*/#define __NA_000000_20070704_FATIGUE_REWARD					// 2007. 07. 04 / 조재식, 김경표 / 피로도시 보상관련 
	#define __NA_0_ADD_VERSION_TO_LOG_FILE_NAME					// 2008.01.10 / 김창현 / 로그파일 이름에 버전추가
	//#define __CN_269_MATRIX_CARD_AUTH							// 2007.11.12	// lammy / 매트릭스 카드 관련
	//#define __CHINA_MATRIXCARD_CODE_ONE_SHOT_SUBMIT				// 2007.11.12	// lammy / 인증 코드 한번에 입력하게 
	#define __MY_PAGE_BLOCK
	#define _INVENTORY_LOCK_BLOCK
	#define	__NA_000000_20080221_NATIONAL_FILTER				// 2008.02.21 / 김경표 / 국가별 스트링 필터 적용.
	#define __CN_000948_20080305_VALIDATE_REIHALT_IMMEDIATELY	// 2008.03.05 / 김민조 / 레이할트 인증의식 결과보기 클릭시 바로 수치보기
	#define __CN_0_20080306_CANT_CRYSTALIZE_FATEITEM			// 2008.03.06 / 김민조 / 럭키 아이템은 결정화할 수 없음
	/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW					// 2008.03.28 / 이준석 / 방리스트 조건체크없이 모두 보여주기. 일단 국내만 적용.
	#define __CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__       // 2009.03.03 / 이은파,김광수 / [중국] 게임 내 UserID자리수 확장 작업
	#define  __CN_ADD_HEAD_EFFECT_FLAG							// 2009.04.06 /김광수/ 중국 vip 이펙트 위치보정	
	#define __CN_000097_GM_HIDE_TAGETING_BLOCK					// 2009.05.13 / 김광수 / GM캐릭하이드시 타겟팅불가
	#define _CN_EXCECPT_MISSION									// 2009.06.04 / 김광수 / 중국룸타입 미션제거
	/*($Delete_[S/C]$)*/#define _GS_ELITE_FULLSET_CHANGE_ITEM						// 2009.10.19 / 김광수 / 엘리트풀셋2차 체인지아이템


#elif defined(_JAPAN)
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //    ▶ JAPAN 일본 버전◀  
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	/*($Delete_[S/C]$)*/#define	__NA_000839_20080129_PVP_OPTION					// 2008.01.29 / 조재식, 김지완 / PVP서버 옵션 기능 추가.	  ==> 2 1,2번을 해외에서 올림.
	/*($Delete_[S/C]$)*/#define __NA_000902_20080226_MESSAGE_RESTRICTION		// 2008.03.10 / 조재식, lammy / 파티원 이외에 메시지 제한
	//#define	__JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE				// 2008.03.18 / 조재식,김경표 / 일본 인첸트 랭크업 시스템을 현지화( 100% 안되게.. )
	#define __NA000000_080211_HIRAKATA_FILTER_CHAR__					// 2008.02.11 / 최정록 / 일본어 버전의 문자체크 ( 특수 문자 필터 관련 )
	#define	__NA_000000_20080221_NATIONAL_FILTER						// 2008.02.21 / 김경표 / 국가별 스트링 필터 적용.
	#define _K2P_SOFTWARE_NUMBER_KEYBOARD_000913						// 2008.03.10 / lammy / 거래시 금액 입력창 키보드로 변경
	#define _K2P_SERVER_LIST_BY_WORD_000897								// 2008.03.10 / lammy / 채널 리스트 유저수 글자로 변경
	#define _K2P_WORD_LENGTH_BUG_IN_JAPAN								// 2008.03.12 / lammy / 지포스 최신 드라이버 글자 길이 버그때문에 툴팁 width값(15pxl) 강제로 늘림
	#define	__JP_000899_20080318_ENCHANT_RANKUP_LOCALIZE				// 2008.03.18 / 조재식,김경표 / 일본 인첸트 랭크업 시스템을 현지화( 100% 안되게.. )
	#define _EXCECPT_HUNTING_001011										// 2008.04.17 / lammy / 배틀존 헌팅방 관련 UI 삭제
	#define __NA_001027_20080423_RANDOM_REWARD_EVENT_
	/*($Delete_[S/C]$)*/#define __NA_000000_20080425_CHECK_START_TILES
	
	/*($Delete_[S/C]$)*/#define __NA_0_20080617_CLASS_STARTING_POINT_BUG_FIX				// 클래스별 스타팅 포인트 사용하지 않음
	/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW							// 2008.03.28 / 이준석 / 방리스트 조건체크없이 모두 보여주기. 일단 국내만 적용.
	#define __090910_LTJ_CHANGEUP_CLASSNAME_JP							// 2009.09.10 / 이태준 / 체인지업 캐릭터의 경우 새로운 클래스명으로 출력
    #define _100331_LTJ_MOD_GM_CHAT_COLOR_JAPAN                         // 2010.03.31 / 이태준 / GM캐릭터의 일반채팅/귓속말 색깔을 일반캐릭과 같도록 변경
    #define _100331_LTJ_MOD_COMMUNITY_BLOCK_MODE_JAPAN                  // 2010.03.31 / 이태준 / 커뮤니티창 차단리스트에서 캐릭터명을 제외한 나머지 탭을 숨김


#elif defined(_GLOBAL)
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //    ▶ GLOBAL 글로벌 버전◀  
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	#define _SCJ_LONG_TEXT_REDUCING										// 범위를 넘어가는 긴 길이의 문자열 줄이기
    #define _YMS_GAMEOPTION_LANGUAGETYPE
    #define _YMS_LANGUAGE_KIND
    #define _DH_OS_LANGUAGETYPE
    #define _DH_NORMAL_CHAT_BLOCK                                       // 차단된 캐릭터 일반채팅 블럭
    #define _INTERNATIONAL_UI	
    #undef _JBH_ADD_AGE_LIMIT_MARK										// 영문버전은 제외	
    #define _GS_MOD_GSP_ITEMSHOP_TXT									// 북미 캐쉬샵 설명이 길때 ...처리	
#if defined(_SERVER) 
	//no operation		
#else
	#define _APPLY_NHN_FILTERFING										// 북미 NHN필터링 모듈적용
#endif//_SERVER
    #define _INTERNATIONAL_DATE_TOOLTIP									// 북미 영문날짜툴팁
    #define __LTJ_TEXT_OUTPUT_AT_LIST_MOD									
    #define _GS_GSP_MISSION_FORWARD_TOOLTIP								// 북미 npc만났을때 뜨는창 ..처리
    #define _GS_GSP_ALLOW_MOVE_TARGETING_								// 북미 지역지정스킬 이동.점프시에 유지
    //#define __CN_CREATE_BLOCK_SHADOW                                    // 북미 쉐도우 블럭컨텐츠
    #define _GS_GSP_REMOVE_LOCKBTN_										// 북미 창고.인벤 락버튼없애기
    #define _GS_GSP_DELETE_CHAR_CHK										// 북미 캐릭삭제(지정문자로하기)
    #define _GS_GSP_PK_BLOCK_ESODE										// 북미 에소드마을에서는  PK 막기	
    #define _GS_GSP_SCROLL_PROFILE										// 북미 캐릭생성창 프로필 스크롤처리
    #define _GS_MOD_ITEMNAME_DRAW_BY_BV									// 북미 아이템네임 피킹볼륨위에찍기
    #define _GS_BUG_FIX_PARTY_POPUP										// 북미 파티장이 비파티멤버클릭시팝업	
    #define _GS_GSP_MISSION_HELP_TOOLTIP								// 북미 미션헬프 아이템설명 툴팁처리
    //#define _GS_GSP_REMOVE_PCROOM_SLOT									// 북미 인벤PC방슬롯제거 
    //#define __LTJ_GSP_HIDE_CASHSHOP_PCCAFE_BUTTON						// 북미 캐시샵 피시방버튼 숨김
    #define _GS_GSP_CASH_SHOP_BLOCK_CONTENT								// 북미 캐쉬샵 개선요청
    //#define _K2P_EXTENSION_BLOCK_										// 북미 기간연장버튼블럭
    #define _GS_GSP_BLOCK_SYSTEMDLG_THEN_DEAD							// 북미 필드사망시에는 시스템창막기
    /*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW							// 2008.03.28 / 이준석 / 방리스트 조건체크없이 모두 보여주기
    //#define _GS_ADD_KEY_FILTERING                                       // GSP 키보드필터링(채팅시에 오직영문만처리)
    #define _DH_AC_UI_CHANGE											// 2009/1012 / 이도형 / AC UI변경 관련 작업
    #define _GS_GSP_TEAM_PVP_MSG                                        // GSP PVP팀전에서 결과보여주기
    #define _GS_GSP_PVP_ENEMY_COLOR                                     // GSP PVP팀전에서 적은 빨간색의로 캐릭명찍기
	#define _GS_ADD_COMPOSITE_OVERLAP_NUM								// GSP 아이템조합시 갯수찍기
    #define _GS_ADD_PET_ENCHANT_EFFECT                                  // GSP 펫인첸트효과 작업
    /*($Delete_[S/x]$)*/#define _NA00XXXX_090111_CASH_TYPE_SHARED_ // 이은파, 이도형 / 캐시타입(PP카드) 추가에 따른 구분 작업 (전 국가 공통)
    #define _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_ // 이은파, 이도형 / 캐시타입(PP카드) 추가에 따른 구분 작업
    #define _GS_USA_TOOLTIP_BORDER										// 2010/0217 / 김광수 / 북미 툴팁시 :표시
    #define _DH_SHOW_REPAIR_HAIM
    /*($Delete_[S/x]$)*/#define _NA000000_100714_NEW_CHARITEM_EXTENDED_FIELD_ // 이은파 / 신규 캐릭에 지급하는 장비 아이템 확장 지원
    #define _NA000424_GLOBAL_CASH_ITEM_RENEWAL_                         // 이은파,이도형 / C/P 코인 결재 타입에 따른 규칙 아이템 속성으로 처리하도록 변경
    #define _NA_20101011_HEIM_LOTTO_CASH_TICKET                         // 2010.10.11 / 강재준 / 캐시 아이템으로 하임행운 응모 용지 획득 기능 : _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE와 함께 활성화될 수 없음

#elif defined(_RUSSIA)
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //    ▶  러시아 버전◀  
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    #define _SCJ_LONG_TEXT_REDUCING										// 범위를 넘어가는 긴 길이의 문자열 줄이기
    #define _YMS_GAMEOPTION_LANGUAGETYPE
    #define _YMS_LANGUAGE_KIND
    //#define _DH_OS_LANGUAGETYPE
    #define _DH_NORMAL_CHAT_BLOCK                                       // 차단된 캐릭터 일반채팅 블럭
    #define _INTERNATIONAL_UI	
    #undef _JBH_ADD_AGE_LIMIT_MARK										// 영문버전은 제외
    #define _GS_MOD_GSP_ITEMSHOP_TXT									// 북미 캐쉬샵 설명이 길때 ...처리	
    #define _INTERNATIONAL_DATE_TOOLTIP									// 북미 영문날짜툴팁
    #define __LTJ_TEXT_OUTPUT_AT_LIST_MOD									
    #define _GS_GSP_MISSION_FORWARD_TOOLTIP								// 북미 npc만났을때 뜨는창 ..처리
    #define _GS_GSP_ALLOW_MOVE_TARGETING_								// 북미 지역지정스킬 이동.점프시에 유지
    //#define __CN_CREATE_BLOCK_SHADOW                                  // 북미 쉐도우 블럭컨텐츠
    #define _GS_GSP_REMOVE_LOCKBTN_										// 북미 창고.인벤 락버튼없애기
    #define _GS_GSP_DELETE_CHAR_CHK										// 북미 캐릭삭제(지정문자로하기)
    #define _GS_GSP_PK_BLOCK_ESODE										// 북미 에소드마을에서는  PK 막기	
    #define _GS_GSP_SCROLL_PROFILE										// 북미 캐릭생성창 프로필 스크롤처리
    #define _GS_MOD_ITEMNAME_DRAW_BY_BV									// 북미 아이템네임 피킹볼륨위에찍기
    #define _GS_BUG_FIX_PARTY_POPUP										// 북미 파티장이 비파티멤버클릭시팝업	
    #define _GS_GSP_MISSION_HELP_TOOLTIP								// 북미 미션헬프 아이템설명 툴팁처리
    //#define _GS_GSP_REMOVE_PCROOM_SLOT									// 북미 인벤PC방슬롯제거 
    #define __LTJ_GSP_HIDE_CASHSHOP_PCCAFE_BUTTON						// 북미 캐시샵 피시방버튼 숨김
    //#define _GS_GSP_CASH_SHOP_BLOCK_CONTENT								// 북미 캐쉬샵 개선요청
    #define _K2P_EXTENSION_BLOCK_										// 북미 기간연장버튼블럭
    //#define _GS_GSP_BLOCK_SYSTEMDLG_THEN_DEAD							// 북미 필드사망시에는 시스템창막기
    /*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW		// 2008.03.28 / 이준석 / 방리스트 조건체크없이 모두 보여주기
    //#define _GS_ADD_KEY_FILTERING                                     // GSP 키보드필터링(채팅시에 오직영문만처리)
    #define _DH_AC_UI_CHANGE											// 2009/1012 / 이도형 / AC UI변경 관련 작업
    #define _GS_GSP_TEAM_PVP_MSG                                        // GSP PVP팀전에서 결과보여주기
    #define _GS_GSP_PVP_ENEMY_COLOR                                     // GSP PVP팀전에서 적은 빨간색의로 캐릭명찍기
    #define _GS_ADD_COMPOSITE_OVERLAP_NUM								// GSP 아이템조합시 갯수찍기
    #define _GS_ADD_PET_ENCHANT_EFFECT                                  // GSP 펫인첸트효과 작업
    /*($Delete_[S/x]$)*/#define _NA00XXXX_090111_CASH_TYPE_SHARED_                          // 이은파, 이도형 / 캐시타입(PP카드) 추가에 따른 구분 작업 (전 국가 공통)
    
    #define _GS_USA_TOOLTIP_BORDER										// 2010/0217 / 김광수 / 북미 툴팁시 :표시
    #define _DH_SHOW_REPAIR_HAIM
    /*($Delete_[S/x]$)*/#define _NA000000_100714_NEW_CHARITEM_EXTENDED_FIELD_               // 이은파 / 신규 캐릭에 지급하는 장비 아이템 확장 지원
    // 러시아는 C/P 캐시 구분이 필요없음 [3/28/2011 iforall]
    //#define _NA000424_GLOBAL_CASH_ITEM_RENEWAL_                         // 이은파,이도형 / C/P 코인 결재 타입에 따른 규칙 아이템 속성으로 처리하도록 변경
    //#define _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_                       // 이은파, 이도형 / 캐시타입(PP카드) 추가에 따른 구분 작업
    #define _NA_20101011_HEIM_LOTTO_CASH_TICKET                         // 2010.10.11 / 강재준 / 캐시 아이템으로 하임행운 응모 용지 획득 기능 : _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE와 함께 활성화될 수 없음
    //#define _LTJ_BLOCK_NOTICE_TOP_OF_TRIAL_TIME                         // 2011.02.15/ 이태준 / 저탑 입장시간 알림공지 표시안함
    #define _RU_0000_20120227_RUSSIA_BLOCK_ITEM_SCORE // 2012.02.27 / 러시아 타플레이어 장비평점보기 블럭
    #define _RU_0000_20120227_RUSSIA_HIDE_DOMINATION_TENDER_COST // 2012.02.27 / 러시아 입찰리스트 입찰개수 숨김
    #define _RU_0000_20120227_RUSSIA_APPLY_CHAT_FILTERING // 2012.02.27 / 러시아 필터링 적용
    #define _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI // 2012.03.12 / 러시아 문자길이 문제에 따른 캐시샵 UI수정
    #define _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT // 2012.03.12 / 리소스 버전 체크 스크립트 추가


#elif defined(_KOREA) 
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
    //    ▶ KOREA 한국 버전◀  
    //━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
	/*($Delete_[S/C]$)*/#define __NA000000_080324_KEYBOARD_MOVE_FILTER_REMOVE__		// 2008.03.24 / 이은파 / 키보드 이동 핵 방지 및 동기화 제어 루틴 일부 제거
	/*($Delete_[S/C]$)*/#define __NA_0_20080328_ROOMLIST_ALL_VIEW					// 2008.03.28 / 이준석 / 방리스트 조건체크없이 모두 보여주기. 일단 국내만 적용.
	#define _JBH_ADD_AGE_LIMIT_MARK							//07.4.18 [000421]주기적으로 15,19세 마크 표시
	
#endif

