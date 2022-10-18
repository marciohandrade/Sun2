#pragma once
//////////////////////////////////////////////////////////////////////////
//		이도형 작업 디파인 DH, elflee
//////////////////////////////////////////////////////////////////////////

//																		// ┌──┬───┬───────┬───┬────────────────────────────
//																		// │국가│ 번호 │   작업일자   │담당자│내용
//																		// ├──┼───┼───────┼───┼────────────────────────────
//#define _DH_GM_NPC_WINDOW // gm npc
//#define _NA_000000_20120827_SEARCH_VENDOR // 2012.8.27 / 이도형 / 개인상점 검색
//#define _NA_004965_20120613_NEW_CHARACTER_MYSTIC // 2012.06.13 / 김현웅, 이태준 / 신규 캐릭터 미스틱 추가
//#define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT // 2012.06.12 / 이태준 / 신규 캐릭터 클라이언트 수정(스크립트파서관련)
//#define _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP
//#define _NA_000000_20120613_CHAOSZONE_SYSTEM //전장시스템
//#define _NA_004644_20110506_QUEST_NAVIGATION //퀘스트 네비
//#define _DH_SHOW_VENDOR_LIST_POPUP // 개인상점 리스트 팝업 메뉴
//#define _DH_SHOW_ITEM_UNIT_PRICE_IN_VENDOR // 개인상점 판매창 개별 가격 표시
//_DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER / _DH_TRIGGER_LOG_MESSAGE //트리거 로그
//#define _DH_LAST_SELECTED_CHARACTER //마지막 로그인한 캐릭터 저장
//#define _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL //길드리뉴얼
//#define _DH_KEY_BINDING // 2011.11.29. 이도형, 이태준 / 유저 단축키 세팅 지원
//#define _DH_CHANGE_CHAT_HISTORY // 2012.01.09 이도형 / 채팅 히스토리 활성화 키 변경
//#define _DH_MONEY_COLOR_CHANGE // 2012.02.08 / 이도형 / 하임 단위별 색상 수정
//#define _DH_DEBUG_REPORT //디버그 리포트
//#define _DH_BATTLEZONE2_// 2011.06.30 / 이도형 / 배틀존 리뉴얼(클라용)
//#define _DH_BATTLEZONE2_LIST_PAGE //2011.06.30 / 이도형 / 배틀존 리뉴얼: 배틀존 리스트 페이지 방식으로 변경
//#define _DH_BATTLEZONE2_LOBBY_SLOT//2011.06.30 / 이도형 / 배틀존 리뉴얼: 로비슬롯 순서 변경(방장최초 아니게)
//#define _DH_QUEST_ACCEPT_MESSAGE // 2011.05.19 / 이도형 / 퀘스트 수락시 상단에 메시지 출력 하는 작업
//#define _NA_003649_20111115_BATTLE_ZONE_HARDMODE    // 2011.11.15 / 정현성 / 이도형 / 배틀존.하드모드 추가
//#define _SCJ_REMOVE_KEY_HELP_WINDOW
//#define _NA_002521_20110613_TOTAL_GUIDE_SYSTEM                        // │공통│000000│2011/0613~06..│이도형│정보열람실, 도움말, 시스템 가이드(참조:방영훈/김현웅/송찬종)
//#define _NA_002050_20110216_ADD_GENDER
//#define _DH_QUEST_ACCEPT_MESSAGE                                      // │공통│000000│2011/0516~0516│이도형│퀘스트 수락시 메시지 출력
//#define _DH_KEY_BINDING                                               // │공통│000000│2011/0429~0429│이도형│키보드 바인딩 관련 작업
//#define _DH_GLOBAL_VERSION_TEST                                       // │공통│000000│2011/0421~0421│이도형│글로벌 테스트서버 목록 관리 기능(참조:방영훈)
//#define _NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT│공통│000000│2011/0406~0412│이도형│장비 우클릭 해제(참조:이은파)
// 장비보이기 관련 패킷 변경 해야함 : 추후 작업 
// CG_CHARINFO_APPREARANCE_BRD, CG_CHARINFO_DISAPPREARANCE_BRD -> CG_CHARINFO_APPREARANCE_LIST_BRD, CG_CHARINFO_DISAPPREARANCE_LIST_BRD 
//#define _NA_20100322_AGGRO_DISPLAY                                    // │공통│000000│2011/0331~0404│이도형│몬스터 속성 및 어그로 표시 (참조:문상현/김준호)
//#define _DH_SHOW_ITEM_CREATE_RATE                                     // │공통│000000│2011/0325~0325│이도형│아이템 조합(생성)확률 표시 (참조:김준호)
//#define _NA_20100307_BETAKEY_SYSTEM                                   // │공통│000000│2011/0321~0325│이도형│베타키(참조:문상현)
//#define _DH_GM_ITEM_WINDOW                                            // │공통│000000│2011/0302~    │이도형│개발자용 아이템 검색 및 생성 기능
//#define _NA_20110303_ADD_TYPE_OF_NPCSHOP                              // │공통│000000│2011/0307~0307│이도형│타입별 NPC 상점 기능 추가(참조:민정영)
//#define _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT           // │공통│000000│2011/0222~0222│이도형│(EP2-02)지역점령전 성문관리기능 추가블럭 기능
//#define _DH_FATE_REMOVE_MAGIC_ARMOR                                   // │공통│000000│2011/0222~0222│이도형│(EP2)럭키 아이템 마법 방어력 효과, UI컨트롤 제거
//#define _DH_TOOLTIP_PREVIWE_PET                                       // │공통│000000│2011/0328~0328│이도형│펫 툴팁 미리보기
//#define _DH_REMOVE_SOCKET_COMPOSITION_ANIMATION                       // │공통│000000│2011/0222~0222│이도형│(EP2)소켓 애니 간소화
//#define _NA_002128_20110228_MODIFY_ITEM_LEVEL                         // │공통│000000│2011/0221~0221│이도형│(EP2)게임 내 가치에 맞도록 아이템 레벨 변경
//#define _DH_TARGET_INFO_RENEWAL                                       // │공통│000000│2011/0217~0217│이도형│F2로 보는 NPC 정보 EP2에 맞도록 수정(공/방/속성능력등)
//#define _DH_FULL_MAP_PORTAL                                           // │공통│000000│2011/0215~0216│이도형│개발자용 전체맵 찍고 포탈 타기기능
//#define _DH_EP2_FULL_MAP                                              // │공통│000000│2010/1206~1209│이도형│에피소드2 신규 맵
//#define _DH_DIVIDE_MESSAGE_TEXT                                       // │공통│000000│2010/1203~1203│이도형│message.txt를 스트링부분(messagetext.txt)와 데이터부분(message.txt)로 분리
//#define _DH_CHANGE_COOLTIME_DRAW                                      // │공통│000000│2010/1118~1122│이도형│모든 쿨다운 그림 회전방식으로 변경
//#define _DH_QUICK_SLOT_LINK                                           // │공통│000000│2010/1104~1111│이도형│퀵슬롯에 등록된 아이템이 모두 소모된 경우 위치링크 문제 수정
//#define _DH_IMAGE_TOOLTIP                                             // │공통│000000│2010/1022~1024│이도형│툴팁에 이미지 등록 기능 및 한줄에 여러번 등록 기능
//#define _NA_20101011_HEIM_LOTTO_CASH_TICKET                           // │월드│000000│2010/1012~1012│이도형│글로벌 캐쉬 로또 관련(참조:강재준)
//#define _DH_REMOVE_OLD_SOCKET                                         // │공통│000000│2010/1013~0   │송영덕│이전 소켓 삭제
//#define _NA_000849_20101001_ENHANCED_INVITATION_FOR_CHAT_CHANNEL      // │월드│000000│2010/0920~0920│김장현│채널채팅 수락 옵션(channel, chatting..)
//#define _NA_002253_20100811_CUBE_SYSTEM                               // │월드│000000│2010/0831~    │송영덕│아이템 큐브(참조:김현웅)
//#define _NA_0_20100819_AGGRO_RENEWAL                                  // │월드│000000│2010/0830~0830│이정환│몬스터 어그로 수치 출력(참조:문상현)
//#define _DH_DISPLAY_FOR_MISSION_OBJECT								// │월드│000000│2010/0802~0827│송영덕│미션수행 목적 리스트 표시 작업
//#define _DH_OS_LANGUAGETYPE                                           // │월드│000000│2010/0802~    │이도형│운영체제 설정언어값 기반 string선택
//#define _DH_REMOVE_RANKUP                                             // │공통│000000│2010/0928~0   │송영덕│랭크업 삭제
//#define _NA_000251_20100727_SOCKET_SYSTEM                             // │공통│000000│2010/0928~0   │송영덕│소켓 리뉴얼(참조:이호영->김현웅)
//#define _DH_DOUBLECLICK_MOVE_WAYPOINT                                 // │공통│000000│2010/0525~0525│이도형│웨이포인트 리스트 더블클릭으로도 이동 요청 가능하게 변경
//#define _DH_SHOW_REPAIR_HAIM                                          // │공통│000000│2010/0723~0723│김성태│수리시 나오는 알림창에 소모 하임을 ','구분점으로 표시
//#define _DH_TRIGGER_LOG_MESSAGE                                       // │공통│000000│2010/070000000│송영덕│게임중 트리거 내용 확인 메시지 출력 기능(개발용)
//키워드 _DEBUG_STRINGIDEX_SHOW                                         // │공통│000000│2010/0714~0714│이도형│스트링 ID표시 기능
//키워드 _DH_LOGIN_LOG_ID_AND_CHANNEL                                   // │공통│000000│2010/0714~0714│이도형│로그인시 ID및 채널 표시
//#define _DH_SHOW_UI_NAME                                              // │공통│000000│2010/0712~0713│이도형│UI 게임내에서 이름 표시 기능 추가
//#define (없음)찾기키워드(eGM_CMD_GET_MONSTER_ATTACK_SPEED)            // │공통│000000│2010/0607~0607│김성태│퀘스트버그(글로벌) 응답 지연중 퀘스트 제요청 문제 수정
//#define _YMS_DH_ATTACK_SPEED_FORM_SCRIPT(제거됨)                      // │공통│000000│2010/0617~06..│정만복│몬스터 공격속도 수치 자동화 뽑기
//#define _NA_000070_20100609_ENHANCED_ITEM_DIVISION                    // │공통│000000│2010/0610~0611│송영덕│아이템 상점 수량 구매및 나누어서 판매(참조:김현웅)
//#define _DH_NORMAL_CHAT_BLOCK                                         // │공통│000000│2010/0609~0609│김성태│일반채팅 차단된 캐릭터 무시(임시적용)(글로벌)-요청 
//#define _DH_ENABLE_DIVIDING_ITEM_WITH_CLICKING_SLOT_TAB               // │공통│000000│2010/0601~0604│송영덕│아이템 나누기
//#define _DH_ENHANCED_UI_PACKAGE_CHATTING                              // │공통│000000│2010/0507~0000│황세일│확장 UI 작업들(채팅, 맵 등등)
//#define _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_       // │공통│000150│2010/0412~0506│황세일│인벤토리 확장및 필터링
//#define _DH_PARTY_LEADER_CHANGE_CLOSE_OPTWIN							// │공통│000000│2010/0409~0409│권아영│파티장 위임시 아이템분배 옵션창 닫음(추가작업)
//#define _DH_ADD_TYPE_MARK_FOR_ITEM                                    // │공통│000000│2010/0405~0406│송영덕│아이템을 통한 아이템 식별작업(종류별 작은 마크추가)
//#define _NA_0_20100325_QUICK_SLOT_CHANGE                              // │공통│000000│2010/0323~0402│송영덕│퀵슬롯 변경(참조:남기봉)
//#define _DH_COUNT_LOGIN_FRIEND                                        // │공통│000000│2010/0322~0323│송영덕│친구 접속자 수 표시
//#define _DH_CHANGE_EXP_COLOR                                          // │공통│000000│2010/0322~0322│송영덕│최고레벨일 때 경험치 바 색상 변경
//#define _DH_GROUPSYSTEM_CHANGE_PACKET                                 // │공통│000000│2010/0219~0219│민정영│그룹 하임 획득 CW에서 CG로 변경
//#define _DH_CHANGE_SHADOW_SKILL_DARK_BREAK                            // │중국│000000│2010/0201~0201│이호영│다크브레이크 스킬 변경
//#define _DH_GROUPSYSTEM_ALL3                                          // │공통│000000│2010/0119~0119│민정영│그룹멤버 초과에 따른 메시지 출력(서버요청)
//#define _NA00XXXX_090111_CASH_TYPE_SHARED_                            // │공통│000000│2010/0118~01  │정만복│캐시타입(PP카드) 패킷관련(참조:이은파)
//#define _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_                         // │공통│000000│2010/0118~01  │정만복│캐시타입(PP카드) 기능관련(참조:이은파)
//#define _NA_0_20091208_VENDOR_ITEM_SEARCH								// │공통│000000│2010/0111~01  │송영덕│개인상점 검색 기능 추가(참조:남기봉)
//#define _NA_0_20091118_GAMEROOM_BATTLE_RESULT							// │공통│000000│2009/1215~0108│송영덕│전투결과 보여주기[미션,헌팅,PVP](참조:남기봉)
//#define _DH_CUR_SKILL_NEEDSFOR_TOOLTIP								// │공통│000000│2009/12       │권아영│스킬 찍은 거도 숙련도 보기
//#define _DH_UNUSE_LOTTO												// │중국│000000│2009/1204~11  │김종준│하임행운 시스템 중국 임시 미적용 UI끄기
//#define _DH_NOAPPLY_GROUPINGSYSTEM_									// │중국│000000│2009/1126~1126│김종준│그룹시스템 중국 임시 미적용 UI끄기
//#define _DH_PANDORA_TOOLTIP_ADD_MARGIN								// │공통│005290│2009/1125~1125│이도형│사제선물 툴팁 줄 높이 변경
//#define _DH_MISSIONZONE_NO_ITEMMESSAGE								// │공통│020299│2009/1120~1120│권아영│미션에서는 파티 분배 메시지를 띄우지 않음(버그수정)
//#define _DH_HIDE_INCANT_EFFECT										// │공통│001406│2009/1118~1118│권아영│카메라가 가까이 있을 때 인첸트 이펙트 그리지 않기
//#define _KR_0_20091021_FIRENDINVITATION_EXTEND						// │공통│001404│2009/1118~1209│김준호│친구초대이벤트(참조:문상현)
//#define _DH_CUR_SKILL_NEEDSFOR_TOOLTIP								// │공통│001408│2009/1117~1117│권아영│습득한 스킬에도 툴팁에 요구 숙련도/레벨 표시
//#define _DH_BATTLEZONE_CREATE_CLASS_TOOLTIP							// │공통│001412│2009/1117~1117│권아영│배틀존 생성 클래스 아이콘 툴팁 표시
//#define _DH_DBLCLICK_FRIEND_STATUS									// │공통│001405│2009/1116~1116│권아영│친구목록 더블 클릭으로 상세정보 보기
//#define _DH_PARTY_LEADER_CHANGE_CLOSE_OPTWIN							// │공통│000000│2009/1112~1112│권아영│파티장 위임시 아이템분배 옵션창 닫음
//#define _DH_GUILDINFO_OFFLINE_COLOR									// │공통│001407│2009/1111~1111│권아영│길드원정보 오프라인 멤버 회색 표시
//#define _DH_PARTY_UI_CUSTOMIZING										// │공통│001402│2009/1110~1116│권아영│파티UI 보이기/숨기기 옵션
//#define _DH_VENDER_WINPOS												// │공통│001409│2009/1110~1110│권아영│개인상점창 위치 유지
//#define _DH_CHARNAME_CHECK_BUG_005306									// │공통│005306│2009/1109~1109│이도형│캐릭터 이름 16바이트 넘어가면 중복체크시 죽는 현상 수정
//#define _NA_0_20091030_RANDOM_ITEM									// │공통│000000│2009/1102~1102│강경란│랜덤아이템 상점 (참조:남기봉)
//#define _CN_0_20091013_RANKUP_EXTEND									// │공통│000000│2009/1102~1102│김준호│랭크업 시스템 개선 아이템 속성 변경, 초기화석(참조:남기봉)
//#define _DH_INDUNOPTION_PARSER										// │공통│000000│2009/1102~1102│정재한│인스턴스 던전 최대층 파서 작업
//#define _DH_TOOLTIP_LIMIT												// │중국│000000│2009/1029~1029│김종준│중국 아이템 조합시 툴팁이 길어지면 길이 줄이기
//#define _CN_0_20091013_RANKUP_EXTEND									// │중국│000000│2009/1027~1029│김준호│랭크업 시스템 개선
//#define _DH_AC_UI_CHANGE												// │공통│000000│2009/1023~1023│이태준│AC UI 축소 변경 따른 처리
//#define _DH_UNMOVEABLE_CLICKED_NPC									// │공통│000000│2009/1012~1012│이도형│이동불가시 엔피씨 클릭 처리
//#define _DH_GUILDHAIM_WIN_CLOSE										// │공통│000000│2009/1012~1012│이도형│길드창고 거래창 닫을 때 하임 입력창도 같이 닫고, 이동불가
//#define _DH_GIFT_CONFIRM												// │공통│001397│2009/1012~1012│권아영│선물하기 재확인 창 작업
//#define _DH_HELLON_TOWER_ENTER										// │공통│000000│2009/0929~0930│권아영│헬론의 성 입장 및 툴팁
//#define _DH_SUNCREDIT													// │공통│000000│2009/0928~0929│강경란│썬 크래딧 영상 UI
//#define __DH_HAIM_WIN_CLOSE											// │공통│000673│2009/0901~0901│이도형│창고나 거래 창을 닫을 때 하임 입력창도 같이 닫아주도록 변경
//#define __NA_1247_POSSESSION_ITEM										// │공통│004808│2009/0901~0901│이도형│귀속아이템 등록창 1개만 뜨도록 변경
//#define __NA_0_POSSESSION_ITEM_DROP			 						// │공통│001355│2009/0831~0903│김준호│귀속아이템-부서진 형태 드랍 및 원형 드랍(아이콘,툴팁) 참조:김창현
//#define __KR_001355_20090624_GROUPINGSYSTEM_ALL_2 					// │한국│001355│2009/0525~0704│강경란│그룹 맺기 시스템-(캐릭터팝업메뉴로 그룹등록/해재), 오프라인 필터 적용안되도록 수정
//#define __DH_COMMUNITY_MEMO__			 								// │한국│000000│2009/0820~0821│이도형│쪽지 줄간격 변경
//#define __NA_0_AC_KEY_DIRECT_REGISTER			 						// │한국│001375│2009/0713~0715│이도형│AC 열쇠 바로 등록
//#define __DH_004145_20090710_ITEMQUESTBUG		 						// │한국│004145│2009/0710~0710│이도형│버그 수정: 아이템 퀘스트 다모은후 삭제시, 완료로 표시
//																		// │    │      │              │      │서버가 처리하도록 변경되어 디파인 제거
//#define __KR_001355_20090624_GROUPINGSYSTEM_ALL 						// │한국│001355│2009/0525~0704│강경란│그룹 맺기 시스템 (참조: 민정영)~
//#define __DH_ALL_000000_20090525_UNUSEABLE_MAPIMG__					// │공통│ 없음 │2009/0603~0603│이도형│쓰지 않는 미니맵 필터
//#define __DH_ALL_000000_20090525_REMOVE_UI__							// │공통│ 없음 │2009/0522~0522│이도형│안쓰는 UI 제거
//#define __NA001094_080707_SURVIVAL_SOLOING_QUEST_SYSTEM__				// │공통│ 없음 │2009/0522~0522│김종준│타락한 사원에서 MainMenu를 통해서 맵을 열 수 없도록 수정
//#define _DH_KOR_000000_20090522_CHARACTER_SCENE_NAME_OVERFLOW_REPAIR	// │공통│ 없음 │2009/0521~0522│김종준│캐릭터 선택에서 키큰 캐릭터 이름 상시 표시
//#define __NA_001337_20090511_EXPANSION_QUEST_COMPL_NUM				// │한국│001337│2009/0518~0518│이수택│퀘스트 대역폭 추가 클라쪽 필요한 코드만 수정
//#define _DH_KOR_001346_20090512_ADDING_TO_TOOLTIP_COLUMN				// │한국│001346│2009/0512~0518│정만복│툴팁에 칼럼 추가하기
//																		// └──┴───┴───────┴───┴────────────────────────────

//-------------------------------------------------------------------------------------
// 개인 History - elflee
//--[2010]-----------------------------------------------------------------------------
//2010/0609~0609    [글로벌버그] 퀘스트 보상 응답 대기전에는 퀘스트 보상을 다시 선택 불가능 하도록 변경

//--[2009]-----------------------------------------------------------------------------
//2009/11/11		[중국빌드] 090403_1109a, 창고에서 멀어질때 하임창 안닫히는 것 적용 (cn0904, 0905 all)
//2009/11/09		[버그:5306] 캐릭이름 16바이트 이상일때 중복체크시 클라이언트가 죽는 현상 수정(길이 체크 후 오류 메시지)
//2009/11/11		[버그:36514] 익스트라스톤 툴팁 미출력 버그 수정		
//2009/11/11		[버그:5329]AC열쇠가 더이상 사용할 수 없는 아이템으로 출력되며 등록할 수 없는 현상(중국) [확인: 스크립트 문제->전달]
//2009/11/11		[버그:5351]일반창고 NPC와 거리가 멀어지면 하임 입력창도 닫히지 않는 버그 수정
//2009/1110~11		[작업:4622] 파티UI 보이기/숨기기 옵션
//2009/1110~1110	[작업:4611] 개인상점창 이동시키면 지속적 위치 유지												
//1012~1012			선물하기 재확인 창 작업
//0929~0930			헬론의성 입장 및 툴팁
//0928~0929			썬 크래딧 영상 UI
//0921~0925			Solarslot 분석
//0916~				스크립트 체크 검사 MDB 테이블 생성 작업
//0901~				창고/거래 하임입력창 버그 수정
//0891~				귀속아이템 버그수정
//0831~				귀속아이템 드랍작업
//0824~				그룹맺기추가사항
//0820~				쪽지줄간격
//0713~				AC열쇠바로등록
//0710~				버그수정(퀘스트 관련)
//0603 				쓰지 않는 미니맵 필터
//0525~0704			그룹맺기 시스템
//0522 				안쓰는 UI제거
//0522 				타락한 사원 MainMenu 맵 못열게
//0522 				키큰 캐릭터 이름 표시 관련
//0518 				퀘스트 대역폭 추가
//0000 				중국비스타관련 테스트(캐릭터 생성시 이름)
//0000~				각종버그수정
//0000~				중국빌드
//0512~0518	    	툴팁에 칼럼 추가하기
//0413~				Warning 제거
//03~				소스 분석 및 공부
//**2009