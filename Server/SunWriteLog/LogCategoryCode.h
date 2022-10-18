#pragma  once

enum eLOG_CATEGORY
{
	LOG_SESSION		=	0x00010001,		//접속로그
	LOG_ITEM		=	0x00020001,		//아이템로그
	LOG_EXP			=	0x00030001,		//경험치
	LOG_ACTION		=	0x00040001,		//액션
	LOG_SKILL		=   0x00050001,		//스킬
	LOG_SNAPSHOT	=   0x00060001,		//스냅샷
	LOG_CHAT		=   0x00070001,		//채팅 로그
	LOG_EVENT		=	0x00090001,		//이벤트
	LOG_ADMIN		=   0x00100001,		//GM(운영자) 로그
	LOG_MONEY		=	0x00200001,		//돈로그
	LOG_GUILD		=	0x00300001,		//길드로그
    LOG_ACCUMULATEPOINT = 0x00400001,		//적립포인트로그
};


/// 접속상태코드
enum eLOG_SESSION
{
	SESSION_LOGIN = 1000,				// 로그인
	SESSION_LOGOUT,						// 로그아웃(이유)
	SESSION_CHAR_CONNECT,				// 캐릭터 접속
	SESSION_CHAR_DISCONNECT,			// 캐릭터 접속 해제
	SESSION_WORLD_LOGIN,				// 월드 로그인 : 아직 사용 안함
	SESSION_WORLD_LOGOUT,				// 월드 로그아웃 : 아직 사용 안함
    // _NA_20100802_SPEED_HACK_DETECTION
    SESSION_SPEED_HACK_SUSPECT,         // 스피드핵 사용 의심
    SESSION_SPEED_HASK_SUSPECT_DISCONN, // 스피드핵 사용 의심에 의한 강제 접속 종료
    // _NA_007750_20141113_WZDAT_MODIFY_LOG
    SESSION_CHANNEL_INFO,               // 채널 정보(동접자, 채널ID)
    //_NA_008506_20150923_LOG_LOGOUT_IN_GAMESERVER
    SESSION_LOGOUT_GAMESERVER,          // 로그아웃(로그아웃 시, 맵/위치정보 저장)
};

enum eLOG_SESSION_LOGOUT_TYPE
{
	eLOGOUT_NORMAL,
	eLOGOUT_ABNORMAL,
	eLOGOUT_HACK,
	eLOGOUT_EXPIRED,
};

/// 아이템로그코드
enum eLOG_ITEM
{
	ITEM_MONSTER_DROP = 2000,			// 몬스터 아이템 드랍
	ITEM_MAKE_SERIAL,					// 아이템 시리얼 발급시

	ITEM_USE,							// 아이템 사용
	ITEM_PICK,							// 아이팀 습득
	ITEM_DROP,							// 아이템 바닥에 버림.
	ITEM_DELETE,						// 아이템 삭제
	ITEM_DIVIDE,						// 아이템 나누기
	ITEM_COMBINE,						// 아이템 합치기
	ITEM_EXCHANGE,						// 아이템 교환

	ITEM_NPC_BUY,						// NPC에게서 구매
	ITEM_NPC_SELL,						// NPC에게 팔기

	ITEM_TRADE_START,					// 거래 시작
	ITEM_TRADE_CANCEL,					// 거래 취소
	ITEM_TRADE_GIVE,					// 거래 주기
	ITEM_TRADE_RECEIVE,					// 거래 받기 

	ITEM_VENDOR_START,					// 개인 상점 시작
	ITEM_VENDOR_PRICE,					// 개인 상점 가격 설정
	ITEM_VENDOR_END,					// 개인 상점 폐쇄
	ITEM_VENDOR_VIEW,					// 개인 상점 관람시작
	ITEM_VENDOR_BUY,					// 개인상점 사기
	ITEM_VENDOR_SELL,					// 개인상점 팔기

	ITEM_WAREHOUSE_GET,					// 창고에서 찾기
	ITEM_WAREHOUSE_PUT,					// 창고에 맡기기
	ITEM_WAREHOUSE_INFO,				// 창고 아이템 정보

	ITEM_ENCHANT_FAILED,				// 인첸트 실패 + 사용 하임
	ITEM_ENCHANT_SUCCESS,				// 인첸트 성공 + 사용 하임

	ITEM_RANKUP_BEFORE,					// 랭크업 전 정보
	ITEM_RANKUP_AFTER,					// 랭크업 후 정보

	ITEM_COMPOSE_SUCCESS,
	ITEM_COMPOSE_FAILED,

	ITEM_CRYSTALIZE_SUCCESS,
	ITEM_CRYSTALIZE_FAILED,

	ITEM_USE_EXTRASTONE,				// 엑스트라 스톤 사용

	ITEM_SOCKET_ADD,					// 소켓 구멍 뚫기(아직구현안됨)
	ITEM_SOCKETOPT_FILL,				// 소켓 옵션 채우기
	ITEM_SOCKETOPT_EXTRACT,				// 소켓 옵션 지우기

	ITEM_QUEST_OBTAIN,					// 퀘스트 수락시 받은 아이템
	ITEM_QUEST_DELETE,					// 퀘스트 삭제 아이템

	ITEM_REWARD_QUEST,					// 퀘스트 보상
	ITEM_REWARD_AC,						// AC 보상
	ITEM_REWARD_MISSION,				// 미션 보상
	ITEM_REWARD_EVENT,					// 이벤트 보상
	ITEM_REWARD_COMPETITION,			// 경쟁헌팅 보상

	ITEM_DURA,							//< 아이템 내구도 수리	

	// 2006년 5월 31일 수요일 오전 4:14:05
	ITEM_EVENT_ROCKET,					//< 로케트베터리 이벤트

	ITEM_EVENT_ETHER_REG,				//< 에테르 경품 응모 이벤트
	ITEM_EVENT_ETHER_REG_FAILED,		//< 에테르 경품 응모 이벤트 실패시 복구된 아이템
	ITEM_EVENT_ETHER_DELETE,			//< 에테르 아이템 교환 이벤트 시 삭제된 아이템

	ITEM_VENDOR_VIEW_END,				//< 개인상점관람종료

	// 20060717 오후 12:06:10
	ITEM_AC_ITEM_USE,					//< AC아이템 사용

	ITEM_PK_DROP,						//< PK에 의한 아이템 드랍

	ITEM_OVERLAPPED_ITEM,				//< 겹쳐지는 아이템에 대한 로그
	ITEM_DELETE_CAUSE_DURA,				//< 내구도0이어서 삭제된 아이템 로그

	ITEM_REWARD_RANKING,				//< 미션 랭킹 로그

	// 20061212 오후 10:17:17
	ITEM_CHARGE_BUY_REQUEST,			//< 캐시아이템 구매 요청
	ITEM_CHARGE_BUY_SUCCESS,			//< 캐시아이템 구매 성공
	ITEM_CHARGE_BUY_FAILED,				//< 캐시아이템 구매 실패

	ITEM_DELETE_REVENGE_PORTAL,			//< 복수의 돌 삭제
	ITEM_DELETE_ITEM_PORTAL,			//< 좌표이동 아이템 삭제
	ITEM_DELETE_PROTECT_PKDROP,			//< PKDROP 방지후 삭제

	ITEM_CHARGE_EXPIRED,				//< 캐시아이템 유효기간 만료
	ITEM_CHARGE_FIRST_EQUIP,			//< 처음 장착하여 유효기간이 설정됨

	ITEM_CHARGE_SEND_GIFT_SUCCESS,		//< 캐시아이템 선물 보내기 성공
	ITEM_CHARGE_SEND_GIFT_FAILED,		//< 캐시아이템 선물 보내기 실패

	ITEM_CHARGE_ACTION_RECV_GIFT_SUCCESS,	//< 캐시아이템 받은 선물 받기 성공
	ITEM_CHARGE_ACTION_RECV_GIFT_FAILED,	//< 캐시아이템 받은 선물 받기 실패

	// 20070618 오후 03:13:27
	ITEM_TEMP_INVENTORY_GET,				//< 보조 창고에서 찾기
	ITEM_DUPLICATE_SERIAL,					//< 아이템 시리얼 중복

	ITEM_RANKDOWN_BEFORE,					//< 랭크다운 전 정보
	ITEM_RANKDOWN_AFTER,					//< 랭크다운 후 정보

	ITEM_IDENTIFY_LUCKY_ITEM,				//< 럭키 아이템 감정

	ITEM_REWARD_SSQ_WINNER,					//< 타락한 사원 승자 보상
	ITEM_REWARD_SSQ_BATTLE_PRIZE,			//< 타락한 사원 전투중 경품

	ITEM_GUILD_WAREHOUSE_GET,					// 길드 창고에서 찾기
	ITEM_GUILD_WAREHOUSE_PUT,					// 길드 창고에 맡기기

	ITEM_ENCHANT_FAILED_DOWN,				// 인첸트 실패_레벨 다운 + 사용 하임

	// 20090130 추가.
	ITEM_GET_BY_LOTTERY,					// 로또나 판도라의 상자에 의한 아이템 습득.
	
	ITEM_SSQ_ENTRANCE_ITEM_REFUND,			//< 타락한 사원 입장 아이템 환불

	ITEM_IDENTIFY_POSSESSION_ITEM,			//< 귀속 아이템 인증
	
	ITEM_ENCHANT_FAILED_DESTROY,			//< 인첸트 실패_아이템 파괴 + 사용 하임

    ITEM_NPC_REPURCHASE,					//< NPC에게서 재구매
    //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    ITEM_CHANGED_TO_SHELL,                  //< 에테리아가 추출된 아이템
    ITEM_COMBINED_WITH_ETHERIA,             //< 에테리아와 결합된 아이템
    //_NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
    ITEM_CHANGED_TO_ETHERIA,                //< 에테리아로 변환된 아이템
    // _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
    ITEM_DOMINATION_AUCTION_REWARD_ITEM_BY_SYSTEM_MEMO, //점령전 아이템 보상:2084 
    ITEM_DOMINATION_AUCTION_RETURN_ITEM_BY_SYSTEM_MEMO, //점령전 아이템 환급:2085
    ITEM_DEFAULT_SYSTEM_MEMO_REWARD,                    //기본적인 시스템 메모, 아이템 보상일 경우

    ITEM_DOMINATION_AUCTION_APPLY,                      //점령전 아이템 입찰의 경우 출력

    // _NA_20110303_ADD_TYPE_OF_NPCSHOP
    ITEM_BUY_NPC_SHOP_TO_ADDITIONAL_TYPE_,              //추가 상점 타입 아이템 구입

    ITEM_CUBE_COMPOSE_MATERIAL, // 큐브 합성에 사용된 재료
    ITEM_CUBE_COMPOSE_REWARD, // 큐브 합성으로 얻은 보상
    ITEM_CUBE_DECOMPOSE_MATERIAL, // 큐브 분해에 사용된 재료
    ITEM_CUBE_DECOMPOSE_REWARD, // 큐브 분해로 얻은 보상
    ITEM_ZARD_COMPOSE_MATERIAL, // 쟈드 합성에 사용된 재료
    ITEM_ZARD_COMPOSE_REWARD, // 쟈드 합성으로 얻은 보상
    // NOTE: f110602.3L, player was basically rewarded for his particifation.
    ITEM_REWARD_SSQ_DEFAULT_PRIZE,
    
    ITEM_DOMINATION_AUCTION_DUP_APPLY,                //점령전 경매 중복 입찰 아이템(왕의증표) 사용

    // _NA_003740_20111122_RANDOM_ITEM
    ITEM_RANDOMIZE_OPTION, // 아이템 옵션 랜덤화
	
    // _NA_004035_20120227_COSTUME_ITEM
    ITEM_USE_COSTUME, // 코스튬 아이템 사용
    ITEM_USE_TOGGLE, // 배지아이템 사용 //_NA_006372_20130108_BADGE_ITEM

    ITEM_SMARTNPC_GIVE,						// SMARTNPC에게 받은 아이템
    ITEM_SMARTNPC_ROB,						// SMARTNPC에게 준 아이템

    // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ITEM_USE_GUILD_FACILITY_UPGRADE,        // 길드 시설 업그레이드로 아이템 사용
    ITEM_USE_GUILD_FACILITY_ACTIVE,         // 길드 시설 활성화로 아이템 사용

    ITEM_ENCHANT_FAILED_PREVENT,            // 인챈트가 실패했으나 아이템 소멸 및 다운 방지

    // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    ITEM_REWARD_ATTENDANCE,                 // 출석 보상

    // _NA_008124_20150313_AWAKENING_SYSTEM
    ITEM_AWAKENING_SUCCESS,                 // 각성 성공
    ITEM_AWAKENING_FAILED,                  // 각성 실패
    ITEM_AWAKENING_DELETE_MATERIAL,         // 각성 재료 삭제
    ITEM_EVOLUTION_SUCCESS,                 // 진화 성공
    ITEM_EVOLUTION_FAILED,                  // 진화 실패
    ITEM_EVOLUTION_DELETE_EQUIPMENT,        // 진화 장비 삭제

    //_NA_008404_20150712_MINIGAME_SYSTEM
    ITEM_REWARD_MINIGAME_BINGO,             // 미니게임/빙고 보상

    ITEM_MAX,
};

enum eLOG_MONEY
{
	MONEY_TOTAL = 9000,					//< 주기적인 + 돈 로그

	MONEY_MONSTER_DROP,
	MONEY_MONSTER_JACKPOT_DROP,

	MONEY_PICK,
	MONEY_DROP,

	MONEY_NPC_BUY,
	MONEY_NPC_SELL,

	MONEY_WAREHOUSE_PUT,
	MONEY_WAREHOUSE_GET,

	MONEY_ENCHANT,
	MONEY_RANKUP,
	MONEY_COMPOSE,
	MONEY_SOCKETOPT_EXTRACT,

	MONEY_TRADE,
	MONEY_VENDOR,						//< 2006/5/30 18:29:00  부터 사용안함-_-;;

	// 퀘스트 보상, AC보상, 미션 보상
	MONEY_REWARD_QUEST,
	MONEY_REWARD_AC,
	MONEY_REWARD_MISSION,
	MONEY_REWARD_COMPETITION,

	MONEY_GREATE_MONEY,					//< 돈이 일정금액 이상일 경우

	MONEY_TOTALWAREHOUSE,				// 창고 통화량

	// 2006/5/30 18:29:00 추가
	MONEY_VENDOR_BUY,					//< 구매자가 개인상점에서 아이템을 산 가격
	MONEY_VENDOR_SELL,					//< 판매자가 개인상점에서 아이템을 판 가격

	MONEY_TRADE_PUT_PRICE,				//< 거래시 넣는 가격 설정
	MONEY_TRADE_GET_PRICE,				//< 거래시 빼는 가격 설정

	// 20061212 오후 10:17:17
	MONEY_CHARGE_BUY_BY_HIME,					//< 캐시아이템 하임으로 구입
	MONEY_CHARGE_RECOVER_FOR_BUY_BY_HIME,		//< 캐시아이템 하임으로 구입 -> 실패시 복구
	MONEY_CHARGE_BUY_BY_CASH,					//< 캐시아이템 하임으로 구입

	MONEY_CHARGE_GIFT_BY_HIME,					//< 캐시아이템 하임으로 선물 
	MONEY_CHARGE_RECOVER_FOR_GIFT_BY_HIME,		//< 캐시아이템 하임으로 선물 -> 실패시 복구

	// 20070619 오후 17:50
	MONEY_SPEND_REPAIR,							//< 무기수리에 하임 소비
	MONEY_SPEND_WAYPOINT,						//< 웨이포인트에 하임 소비

	MONEY_REWARD_SSQ_GAMBLE,					//< 타락한 사원 갬블 승자 보상

	MONEY_GUILD_WAREHOUSE_PUT,				// 길드 창고 돈 집어넣기
	MONEY_GUILD_WAREHOUSE_GET,				// 길드 창고 돈 꺼내기

    MONEY_SPEND_SSQ_ADMISSION_FEE,      //< 타락한 사원 입장료 하임 소비

    // __NA_001359_20090619_HEIM_LOTTO_SYSTEM
    MONEY_LOTTO_REWARD,                         //< 하임 로또 상금 획득

    // __NA_0_20090820_SSQ_FEE_REFUND_LOG
    MONEY_REFUND_SSQ_ADMISSION_FEE,             //< 타락한 사원 입장료 하임 환불

    //_KR_001385_20091211_DOMINATION_GAMELOG_MONEY
    MONEY_DOMINATION_ACTIONMONEY_RETURN,        //지역 점령전 경매 금액 환불 관련:9038
    MONEY_DOMINATION_ACTIONMONEY_REWARD,        //지역 점령전 경매 금액 보상 관련:9039
    MONEY_DOMINATION_ACTIONMONEY_APPLY,         //지역 점령전 경매 금액 입찰 관련:9040

    // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    MONEY_LOTTO_TICKET_PURCHASE,                //< 하임 행운 복권용지 직접 구입

    // _NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG
    MONEY_GROUPSYSTEM_GETMONEY,                 //그룹 시스템에서 상위 멤버로 부터 받는 게임 머니
    
    // _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
    MONEY_DEFAULT_SYSTEM_MEMO_REWARD,           //기본적인 시스템 메모중 하임 보상일 경우:9043

    // __NA_20101209_ADD_GAMELOG_FOR_HEIMLOTTO
    MONEY_HEIM_LOTTO_SETTLE_RESULT,             //하임로또 결산 결과(인원에 따라 출력)

	MONEY_MAX,
};


///	경험치로그코드
enum eLOG_EXPERIENCE
{
	EXP_LEVELUP = 3000,			//< 레벨 업
	EXP_DOWN,					//< 경험치 다운
	EXP_DIVIDECNT,				//< 경험지의 n/10 증가
	EXP_QUEST,					//< 퀘스트 보상 경험치
	EXP_COMPETITION,			//< 경쟁헌팅 보상 경험치
	EXP_AC,
	EXP_MISSION,
    // _NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG
    EXP_GROUPSYSTEM,            //< 그룹 시스템 에서 하위 멤버로 부터 받는 경험치 로그
    // NOTE: f110602.3L, perk experience & changed level log
    EXP_PERK,
    EXP_PERK_LEVEL,
    //
};


/// 스킬 로그 코드
enum eLOG_SKILL
{
	SKL_USE_SKILL = 5000
};


/// 사용자의 의미있는 Action 로그 코드
enum eLOG_ACTION
{
	ACT_CREATE_CHAR = 4000,		// 캐릭터 생성
	ACT_DELETE_CHAR,			// 캐릭터 삭제
	ACT_DEAD_CHAR,				// 캐릭터 사망
	ACT_REBIRTH_CHAR,			// 캐릭터 부활
	ACT_PVP_START,				// PVP시도
	ACT_USE_STAT,				// 스탯 사용
	ACT_ACQUIRE_SKILL,			// 스킬 획득
	ACT_SKILLLV_UP,				// 스킬레벨 업
	ACT_MOVE_ROOM,				// 방으로 이동(마을에서 방으로, 방에서 마을로)
	ACT_MOVE_FIELD,				// 필드로 이동(마을에서 필드로, 필드에서 마을로)
	ACT_INVENLOGIN,				// 로그인 할때 인벤토리, 개인상점에 있는 아이템 정보
	ACT_INVENLOGOUT,			// 로그아웃때 인벤/개인상점에 있는 아아템 정보
	ACT_PVPKILL,				// 다른 유저 죽임
	ACT_PVPDIE,					// 죽임을 당함
	ACT_GETTAX,					// 세금 획득

	ACT_PK_KILL,				// PK로 다른 유저 죽임
	ACT_PK_DIE,					// PK로 죽임을 당함
	ACT_PK_CHANGE_CHAOSTATE,	// 카오상태 변경

	ACT_MISSION_RANKING_SUCCESS,// 미션 랭킹 성공 기록
	ACT_MISSION_RANKING_FAIL,	// 미션 랭킹 실패 기록

	ACT_TRACK_REVENGE,			// 리벤지 대상 추적
	ACT_PORTAL_BY_ITEM,			// 좌표이동 아이템을 이용한 포탈

	ACT_SKILLLV_DOWN,			//스킬 레벨 다운
	ACT_SKILL_UNLEARN,			//스킬 UNLEARN
	ACT_DECREASE_STAT,			//스탯 감소
	ACT_INIT_STAT_ALL,			//스탯 초기화
	ACT_INIT_BASIC_STAT_ALL,	//기본스탯 초기화
	ACT_INIT_EXPERTY_STAT_ALL,	//숙련도 스탯 초기화
	ACT_INIT_PASSIVE_SKILL_ALL,	//패시브 스킬 초기화

	ACT_KILL_SPECIAL_MONSTER,	//중요 몬스터 죽임

	ACT_FATIGUE_PLAY_TIME,		//피로도
	ACT__RANKING_POINT,			//랭킹 포인트

	ACT_TROUBLED_KILL,			// 분쟁지역 다른유저 죽임
	ACT_TROUBLED_DIE,			// 분쟁지역 죽임을 당함

	ACT_HOSTILITY_GUILD_KILL,	// 적대길드 다른유저 죽임
	ACT_HOSTILITY_GUILD_DIE,	// 적대길드 죽임을 당함

	ACT_CHARACTER_CHANGE_UP,	// 캐릭터 체인지 업
	ACT_JOIN_MAP,				// 맵 입장
	ACT_QUEST_ACCEPT,			// 퀘스트 수락
	ACT_QUEST_REWARD,			// 퀘스트 보상
	ACT_QUEST_ABANDON,			// 퀘스트 포기

    // __NA_001359_20090619_HEIM_LOTTO_SYSTEM
    ACT_HEIM_LOTTO_PICK,        // 하임 행운 복권 응모

    //_NA001385_20090924_DOMINATION_GAMELOG_ACTION
    ACT_DOMINATION_FIELD_JOIN,          //지역 점령전 입장
    ACT_DOMINATION_FIELD_LEAVE,         //지역 점령전 퇴장
    ACT_DOMINATION_DESTROY_OBJECT,      //지역 점령전 방어 타워 파괴(성문,에테르기둥)
    ACT_DOMINATION_SET_GATE,            //지역 점령전 성문 방어벽 셋팅
    ACT_DOMINATION_STAMP_START,         //지역 점령전 각인 시작
    ACT_DOMINATION_STAMP_COMPLETE,      //지역 점령전 각인 완료
    ACT_DOMINATION_SUCCESS_DEFENCE,     //지역 점령전 수성 성공
    ACT_DOMINATION_SUCCESS_OFFENCE,     //지역 점령전 공성 성공
    ACT_DOMINATION_KILL,                //지역 점령전에서 다른 유저 죽임
    ACT_DOMINATION_DIE,                 //지역 점령전에서 죽임을 당함
    ACT_DOMINATION_CRYSTALWARP_CREATE,  //지역 점령전 수정워프 소환
    ACT_DOMINATION_CRYSTALWARP_DESTROY, //지역 점령전 수정워프 해제
    ACT_DOMINATION_CRYSTALWARP_DEAD,    //지역 점령전 수정워프 파괴(상대길드, 시간 초과)

    ACT_FRIENDINVITATION_RECALL,        //친구초대 이벤트를 위한 소환 성공 로그

    ACT_DOMINATION_REWORD_EVENT_START,  //지역 점령전 보상 이벤트 구동(럭키 몬스터 소환 트리거)

    ACT_DOMINATION_SET_ATTACK_GUILD,    //지역 점령전 공격 길드 선정 이벤트

	ACT_ACHIEVEMENT_VALUE_CHANGED,      //리뉴얼 AC 정보가 변경됨

    // _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    ACT_SYSTEM_MEMO_RECEIVE,            //시스템 쪽지 수령
    ACT_SYSTEM_MEMO_DELETE,             //시스템 쪽지 삭제 (사용자 조작에 의한)
    ACT_ALL_MEMO_DELETE,                //전체 쪽지 일괄 삭제 (사용자 조작에 의한)

    // _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    ACT_DOMINATION_ADD_DEFFENSEWALL_POINT, // 점령전 방벽 포인트 추가

    ACT_DOMINATION_FIELD_JOIN_COUNT,    //점령전 입장 인원 카운트

    POINTWALLET_DEPOSIT_BY_USING_ITEMS,     // 아이템을 사용하여 충전하기
    POINTWALLET_WITHDRAW_BY_BUYING_ITEMS_ON_MARKET, // 포인트상점에서 포인트로 아이템 구매

    ACT_MISSION_CLEAR,                  // 미션 클리어 로그
    ACT_CHARACTER_NAME_CHANGE,          // 캐릭명 변경
    
    // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ACT_BATTLE_GROUND_ENTER, //전장 입장
    ACT_BATTLE_GROUND_LEAVE, //전장 퇴장
    ACT_BATTLE_GROUND_KILL, //전장 킬
    ACT_BATTLE_GROUND_DEATH, //전장 데스
    ACT_BATTLE_GROUND_REWARD, //전장 보상
    ACT_BATTLE_GROUND_STRONGPOINT_ATTACK, //전장 거점 공격
    ACT_BATTLE_GROUND_STRONGPOINT_DEFENSE, //전장 거점 방어
    ACT_BATTLE_GROUND_WIN, //전장 승
    ACT_BATTLE_GROUND_LOSE, //전장 패
    ACT_BATTLE_GROUND_DRAW, //전장 무
    ACT_BATTLE_GROUND_DIS, //전장 Dis

    //_NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN
    ACT_GUILD_DONATION_GUILDCOIN,    // 길드코인 기부

    // _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
    // GoldRush
    ACT_GOLDRUSH_MONSTERKILL,       // 골드러시 / 몬스터킬
    ACT_GOLDRUSH_GOLDPOINT_GET,     // 골드러시 / 골드포인트 획득
    ACT_GOLDRUSH_GOLDPOINT_LOSE,    // 골드러시 / 골드포인트 손실
    ACT_GOLDRUSH_BATTLERESULT,      // 골드러시 / 최종전투결과 띄움

    // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ACT_GUILD_FACILITY_ACTIVE,      // 길드시설 / 활성화

    // _NA_007086_20140318_MONSTERBOOK
    ACT_MONSTERBOOK_REGISTER,       // 몬스터도감 / 도감에 몬스터등록

    // _NA_007750_20141113_WZDAT_MODIFY_LOG
    ACT_TOTAL_DROP_HEIM_FROM_MONSTER,   // WzDat / 특정 맵내에서 몬스터로부터 드랍된 하임량
    ACT_MISSION_CLEAR_TRIGGER,          // WzDat / 미션 성공 트리거 발동시

    // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    ACT_ATTENDANCE_ATTENDED,            // 출석 / 출석함
    ACT_ATTENDANCE_GOT_REWARD,          // 출석 / 보상받음        

    // _NA_008299_20150519_MAX_DAMAGE_LOG
    ACT_MAX_DAMAGE,                     // 최대데미지 / 최대데미지 발생

    // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    ACT_NOTIFICATION_MESSAGE,           // 알림메세지 / 알림메세지 발생

    //_NA_008334_20150608_SONNENSCHEIN
    ACT_SONNENSCHEIN_ENTER = 4092,      // 소넨샤인 / 입장
    ACT_SONNENSCHEIN_LEAVE,             // 소넨샤인 / 퇴장
    ACT_SONNENSCHEIN_DOMINATE_SUCCESS,  // 소넨샤인 / 점령(각인)성공

	// _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	ACT_ONE_DAYS_GUILD_MISSION_ENTER = 4095,		// 일일 길드미션 / 입장
	ACT_ONE_DAYS_GUILD_MISSION_LEAVE,				// 일일 길드미션 / 퇴장
	ACT_ONE_DAYS_GUILD_MISSION_DOMINATE_SUCCESS,	// 일일 길드미션 / 각인 성공

    ACT_MAX
};

enum eLOG_CHAT
{
	CHAT_NORMAL = 6000,			// 일반 채팅
	CHAT_SHOUT,					// 외치기 채팅
	CHAT_PARTY,					// 파티 채팅
	CHAT_GUILD,					// 길드 채팅
	CHAT_PRIVATE,				// 귓속말
	CHAT_FRIEND,				// 친구채팅
	CHAT_CHANNEL,				// 채널채팅
};


enum eLOG_SNAPSHOT
{
	SNAPSHOT_REGULAR = 7000,		// 주기적인 스냅샷 로그
	SNAPSHOT_LOGININVEN,			// 로그인 할때 인벤토리, 창고에 있는 아이템 정보
	SNAPSHOT_LOGOUTINVEN,			// 로그아웃 할때 인벤토리, 창고에 있는 아이템 정보
	SNAPSHOT_WAREHOUSE_OEPN,		// 창고를 열때 창고의 아이템 정보
	SNAPSHOT_WAREHOUSE_CLOSE,		// 창고를 닫을때 창고의 아이템 정보
	SNAPSHOT_WAREHOUSE_OPEN_INVEN,	// 창고를 열때 인벤의 아이템 정보
	SNAPSHOT_WAREHOUSE_CLOSE_INVEN,	// 창고를 닫을때 인벤의 아이템 정보

	SNAPSHOT_MAX,
};

enum eLOG_GM
{
	GM_WHISPER = 8000,				// 귓속말
	GM_ENTRANCE,					// 입장
	GM_TRANSPARENT,					// 투명
	GM_INVINCIBILITY,				// 무적
	GM_DISCONNECT,					// 접속종료
	GM_CHARACTER_INFO,				// 캐릭터 정보
	GM_ROOMINFO,					// 방정보
	GM_ROOMLIST,					// 방리스트
	GM_SPEED,						// 속도
	GM_REBIRTH,						// 부활
	GM_WORLD_NOTICE,				// 월드_공지
	GM_CHANNEL_NOTICE,				// 채널_공지
	GM_ZONE_NOTICE,			    	// 지역_공지
	GM_REMOVALITEM,					// 아이템제거
	GM_REMOVALMONSTER,				// 몬스터제거
	GM_RECOVERY,					// 회복
	GM_LEVELUP,						// 레벨업
	GM_STATPOINTUP,					// 스텟업
	GM_SKILLPOINTUP,				// 스킬포인트업	
	GM_CREATEMONSTER,				// 몬스터소환
	GM_REMOVAL,						// 제거
	GM_CREATEITEM,					// 생성
	GM_CREATEHEIM,					// 하임
	GM_KILL,						// 킬
	GM_SERVERSHUTDOWN,				//< 서버 종료
	//GM_CHUNTING_CLEAR,			//< 경쟁헌팅 전적 초기화
	//GM_CHUNTING_INCWIN,			//< 경쟁헌팅 연승
	GM_ACQUISITION_ACCMULATE_POINT,	// GM 명령어에 의한 적립포인트 획득
	GM_CONSUMPTION_ACCMULATE_POINT,	// GM 명령어에 의한 적립포인트 차감
	GM_ACQUISITION_ACCMULATE_POINT_BY_QUEST,	// GM 명령어에 의한 적립포인트 차감
};

enum eLOG_GUILD
{
	GUILD_LOG_START = 10000, //사용안함, MAP 인덱스를 만들기 위한 시작표시

	GLD_CRTE,				//길드 창설
	GLD_DTRY_WAIT,	//해산 대기
	GLD_DTRY_CACL,	//해산 신청 취소
	GLD_DTRY_CPLT,	//해산 완료
	
	//군단
	GLD_CORP_CRTE,	//창설
	GLD_CORP_DTRY,	//해산

	//길드 멤버
	GLD_MEM_JOIN,		//가입
	GLD_MEM_KICK,		//제명
	GLD_MEM_OUT,		//탈퇴
	
	// 캠프
	GLD_CAMP_CRT,		//창설
	GLD_CAMP_JOIN,		//가입
	GLD_CAMP_KICK,		//제명
	GLD_CAMP_OUT,		//탈퇴
	GLD_CAMP_DTRY,	//해산
	
	//길드관계
	GLD_REL_SET,		//관계설정
	GLD_REL_CACL,		//관계해지

	//레벨
	GULD_LEV_UP,		//길드랭크업

	//직책
	GLD_DUTY_CHG,	//변경

	//권한
	GLD_RIGT_DLGT,	//위임

	//UP
	GLD_UP_GIVE,	//길드 UP상납

    // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //길드 가입 요청
    GLD_JOIN_REQUEST,       //길드 가입 요청
    //길드 시설
    GLD_FACILITY_UPGRADE,   //길드 시설 업그레이드
    GLD_FACILITY_DOWNGRADE, //길드 시설 다운그레이드

	GUILD_LOG_MAX, //사용안함, MAP 인덱스를 만들기 위한 끝표시
};

enum eLOG_ACCUMULATEPOINT
{
    ACCUM_CHANGE_YEAR,          // 접속시간 누적 년도
    ACCUM_ACQUISITION_TIME,     // 접속 시간에 의한 적립포인트 보상
    ACCUM_ACQUISITION_QUEST,    // 퀘스트에 의한 적립포인트 보상
    ACCUM_CONSUMPTION_ROULETTE, // 아이템 랜덤 룰렛게임에 의한 적립포인트 사용.
};