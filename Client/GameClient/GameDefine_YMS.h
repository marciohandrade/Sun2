#pragma once
//////////////////////////////////////////////////////////////////////////
//		유재원 작업 디파인 YJW, i4u4me
//////////////////////////////////////////////////////////////////////////
//

//#define _YJW_SHOP_USING_PPCARD  //! 2011.8.8 / i4u4me /  pp카드
//#define _YJW_SHOP_USING_MILEAGE  //! 2011.8.8 / i4u4me / 마일리지

//#define _YJW_TUTORIAL_SKIP  //! 2011.8.9 / i4u4me / 튜토리얼 스킵 

//#define _YJW_CREATECHARACTER_MOVIE //! 2011.8.3 / i4u4me / 캐릭터 생성후 동영상 보기

//#define _NA003109_STYLE_PRESENTATION_IMPROVEMENT  //! 2011.7.28 / i4u4me /  

//#define _YMS_CHANNEL_LIST_COUNT5    //! 2011.7.12 / i4u4me / 갯수 다섯개 

//#if (WZENGINEVER >= 400)
    //#define _YMS_TREECONTROL   //! 2011.6.13 / i4u4me /  트리콘트롤 / WZENGINEVER >= 400
//#endif //#if (WZENGINEVER >= 40


//#define _YMS_GENDER_FULLSETITEM //! 2011.5.24 / i4u4me / 풀셋 스크립트 수정 

//#define _YMS_WINDOW_MAXSCREEN   //! 2011.4.7 / i4u4me / 창모드 최대화 기능 추가 

//#define _YMS_AUTOBOT_ITEM_KIND //! 2011.3.23 / i4u4me / 자동 사냥 아이템 종류별로 추가 

//#define _YMS_FULLSETITEM_RESOURCE_DIVISION_ //! 2011.3.4 / i4u4me / 엘리트2 풀셋 파트 교환 형식에서 분리

//#define _NA001955_110210_WING_COSTUME   // 2010.02.10 / 유명수, 이은파 / 파닥파닥 장착 아이템 관련 작업

//#define _NA001605_EP2_CHANGE_SKILL_BROADCASTER_ //! 2011.3.3 / i4u4me / 스킬 싱크 

//#define _YMS_SKILL_TOOLTIP_ //! 2011.2.28 / i4u4me / 스킬툴팁 수정: 설명에추가 옵션 넣기

//#define _YMS_PARTY_ERRORCODE_   //! 2011.2.11 / i4u4me / 파티 강제 초대되는 현상 수정 

//#define _YMS_DEATH_SYNC_    //! 2011.2.21 / i4u4me / 공격 결과 동기화

//#define _NA_0_20110216_STATUS_STAT_POINT_SIMULATION           // 2011.02.16 / 남기봉,유명수 / 스텟 포인트 시뮬레이션 추가.
//#if defined(_NA_0_20110216_STATUS_STAT_POINT_SIMULATION)
//    #define _YMS_STAT_SIMULATION_   //! 2011.2.16 / i4u4me / 스텟 시뮬레이션 
//#endif

//#define _YMS_SMOOTH_GAUGE_ //! 2011.2.14 / i4u4me / hp, mp 게이지 스무스하게 움직이게. 


//#define _YMS_LOGIN_RENEWAL_
//#if defined(_YMS_LOGIN_RENEWAL_)
    //#define _YMS_SCREEN_RATIO_  //! 2011.1.24 / i4u4me / 16:10 비율 추가  (WZENGINEVER >= 280) 이상
    //#define _YMS_LOGIN_CHARACTER_CAMERA_RENEWAL_    //! 2011.1.25 / i4u4me / 캐릭터 카메라 설정 변경 
    //#define _YMS_LOADING_IMAGE_ //! 2011.1.27 / i4u4me / 로딩 이미지 하나로 수정 
//#endif 

//#define _NA001605_EP2_CHANGE_SKILL_BROADCASTER_ // 이은파, 유명수 / skill broadcaster 이벤트 방식으로 변경
//#if defined(_NA001605_EP2_CHANGE_SKILL_BROADCASTER_)
//    #define _YMS_EP2_SKILL_PACKET_RENEWAL_ //! 2011.1.10 / i4u4me / 스킬 패킷 수정
//#endif

//#define _NA001605_EP2_ATTRIBUTES_RENEWAL_
//#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
//    #define _YMS_EP2_ATTRIBUTES_RENEWAL_     //! 2011.1.7 / i4u4me / 캐릭터 속성 리뉴얼
//#endif
//
//
////#define _YMS_DIALOG_ANIMATION_ //! 2010.12.13 / i4u4me / 다이얼로그 움직이게  DialogPosInfo.txt  와 같이 배포되어야 함
////#define _YMS_TEXTURE_ANIMATION_ //! 2010.12.27 / i4u4me / 텍스쳐 애니메이션  TextureListInfo.txt, TextureAnimationInfo.txt 같이 배포되어야 함
//#if defined(_YMS_DIALOG_ANIMATION_) && defined(_YMS_TEXTURE_ANIMATION_)
//    #define _YMS_LOGIN_RENEWAL_     //! 2010.12.13 / i4u4me / 로그인 화면 리뉴얼 
//#endif


//#define ENGINE_LIB_PATH  "../../../SunEngine/trunk/lib"

//#define _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_                         // │공통│000000│2010/0118~01  │정만복│캐시타입(PP카드) 기능관련(참조:이은파)
//#define __NA_00387_20070605_SHOP_MILEAGE2
//#define __CN_0_20070507_SHOP_MILEAGE

//#define _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING   //! 2010.10.13 / i4u4me / 아이템샵 하위 메뉴 추가 , 엔진버전 250 이상

//#define _YMS_TRADING_DONOTOPEN_SYSTEMDIALOG //! 2010.10.19 / i4u4me / 거래중일경우에는 시스템 메뉴 열지 않는다 


//#define _YMS_AUTOBOT_GAME  // [2010-7-1 by i4u4me] 오토툴은 엔진버전 211 부터 지원한다.

//------------------------------------------------------------------------------
//적용
//#define _YMS_ADD_RADAR_ICON_100622                  // 2010.06.17 /  미니맵 표시 업데이트 
//#define _YMS_ESCKEY_CLOSE_DIALOG                    // 2010.06.14 /  일부 UI가 ESC 키로 닫히지 않는 현상
//#define _YMS_SYSTEMINPUTBOX_ONLYNUMBER              // 2010.05.28 /  시스템 인풋 창에서 숫자를 못고치는 현상 수정
//#define _NA_0_20100527_ADD_CHAR_RELATION_MEMO       // 2010.06.10 /  친구 메모기능 추가
//#define _YMS_PET_UI                                 // 2010.05.18 /  펫 UI 변신. 
//#define _YMS_GUILD_DETAL_SPEC                       // 2010.05.11 /  길드원 상세 정보 출력 
//#define _YMS_REQUEST_PACKET_QUEST_COMPLETE_SYN      // 2010.05.11 /  포타위치에서 미션 보스 잡으면 퀘가 완료 안되는 현상 

//#define _YMS_CHINA_TAIWAN_MERGE                     // 2010.07.15 / 유명수 / 중국/대만 합침 

//#define _YMS_EVENTFRIEND_SCROLLBAR       // [2010-5-14 by i4u4me] 친구초대 메세지 스크롤 되게

//#define _YMS_LANGUAGE_KIND // [2010-5-6 by i4u4me] 국가별 언어 

//#define _YMS_SUMMONSKILLCONTAINER_RESET // [2010-5-7 by i4u4me] 발키리 소환수 스킬창 리셋


//------------------------------------------------------------------------------
//! 임시
//#define _YMS_USING_FROST_LIBRARY_

//#define _APPLY_HACKSHIELD_MONITOR                           // 2010.03.17 / 유명수 / 중국 핵쉴드 모니터링 적용
//#ifdef _APPLY_HACKSHIELD_MONITOR
//    #define __NA000000_070913_HACKSHIELD_INSTALLATION__
//#endif

//#define _YMS_CONTROL_REDUCE_TEXT    // [2010-3-11 by i4u4me]  _INTERNATIONAL_UI 와 같이사용하지 않음
//
//#define _YMS_NUMBER_TO_MONEY // [2010-3-2 by i4u4me] 하임 천단위에 콤마 입력
//#define _YMS_ESCKEY_COMPOSITE  // [2010-2-26 by i4u4me]  
//#define _YMS_UI_RBUTTONCLICKI_ENCHANT_CRYSTALLIZATION_RANKUP // [2010-2-17 by i4u4me] [한국][0032] 강화창 아이템 등록 펴의 제공
//#define _YMS_UI_STATE_REMEMBER          // [2010-2-17 by i4u4me] [한국][0030]UI 클릭 정보 저장
//#define _YMS_AREACONQUEST_MAP_TOOLTIP   // [2010-2-17 by i4u4me] [한국][0025]AC 보물지도 툴팁 출력 
//#define _YMS_SOUND_CACHE
//#define _YMS_GAMEOPTION_LANGUAGETYPE  //< [2010-1-28 by i4u4me] 국가별 변수 추가
//#define _YMS_RANKUP_TOOLTIP   //< [2010-1-26 by i4u4me] 랭크업 툴팁수정(다음 등급 제한 수치 미출력)
//#define _YMS_CANNOT_EMOTION	//< [2010-1-25 by i4u4me] 피어 상태에서 감정표현 안되게
