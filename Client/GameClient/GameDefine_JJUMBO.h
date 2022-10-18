//////////////////////////////////////////////////////////////////////////
//
// 전보환 작업 디파인
//
//////////////////////////////////////////////////////////////////////////

///////////////////////
// 삭제 후 테스트중

///////////////////////////////

//━━━━━━━━━━━━━━━━━━━ 퍼포먼스  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#define __NA_0_20080626_CREATE_CHARACTER_ROOM_LOADING_CHANGE	// 2008.06.26 캐릭터씬 Init()시, 캐릭터 생성 맵 및 리소스 로딩 되던것 수정.
#define __NA_0_20080627_MAPOBJECT_PICKING_CHANGE				// 2008.06.27 맵 오브젝트 픽킹시 픽킹불가능한것은 처리 하지 않음


#ifdef WORK_RANKUP_ETC_ITEM
	#define _JBH_MOD_RANKUP						//06.10.26 랭크업 UI 변경에 따른 작업
#endif 

//━━━━━━━━━━━━━━━━━━━ 보류 및 미진행  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━
// START 시스템 개선 작업 처리
//#define _JBH_MOD_MONEY_OUTPUT					//06.7.10 Money(돈) 관련하여 돈을 한글로 출력되도록 수정.	 
//	__TASK_WAREHOUSE_N_INVENTORY_LOCK_BY_PWD__ 랑 엉킴 디파인 풀때 다시 한번 보세연;;
//#define _JBH_ADD_BLOCK_HIGH_VALUE_ITEM_DRPO	//06.10.18 고가의 아이템은 Drop 하지 못하도록 처리.
//#define _000541_BLOCK_CHAT_REPEAT				//06.7.10 같은 내용의 채팅을 연속으로 입력시 경고 메세지 출력
// END	시스템 개선 작업

//#define _JBH_ADD_VENDOR_COMMISION				//06.9.20 개인상점 수수료 [완료]
//#define _JBH_MOD_VENDOR_SELECT_BUY_NUM			//06.9.25 개인상점 아이템 개별 구매[완료]
//#define _JBH_ADD_MANUAL_KEY_SETTING	
//#define __ADD_GAME_HELP_SYSTEM

//━━━━━━━━━━━━━━━━━━━ 개발 및 TEST용  ━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
#ifdef _DEV_VER
	#define MAKE_REALTIME_SEND_PACKET				//07.02.23  클라이언트가 패킷을 주고 받을수 있게한다.
	#define MAKE_TEST_MODULE_MACRO					//07.12.12  테스트 모듈 메크로.
#endif
//#define _INFINITE_MAP_LOADING_TEST

