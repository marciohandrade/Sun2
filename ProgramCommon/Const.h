/** Const.h : DEFINE값 정의

*/

#ifndef __CONST_H__
#define __CONST_H__

#include <tchar.h>

enum eNATION_CODE
{
    eNATION_KOREA,
    eNATION_TAIWAN,
    eNATION_CHINA,
    eNATION_JAPAN,
};

//	서버에서 발급할 오브젝트 키 사용 범위
#ifdef _NA_004126_20120111_EXTEND_OBJECT_KEY
enum eOBJECT_KEY
{
    PLAYER_OBJECT_KEY = 0,
    MONSTER_OBJECT_KEY = 500000,
    NPC_OBJECT_KEY = 1000000,
    NONCHARACTER_OBJECT_KEY	= 1500000,
    MAP_OBJECT_KEY = 2000000,
    ITEM_OBJECT_KEY = 2500000,
    PET_OBJECT_KEY = 3000000,
    ONLYCLIENT_OBJECT_KEY = 3400000,	
    ONLYCLIENT_BATTLEZONE_OBJECT_KEY = 3410000,
    MAX_OBJECT_KEY = 3500000,	
};
#else
enum eOBJECT_KEY
{
    PLAYER_OBJECT_KEY		= 0,		//	플레이어	: 1 ~ 5만 ( 최대 5만개 )
    MONSTER_OBJECT_KEY		= 50000,	//	몬스터		: 5만 ~ 10만 ( 최대 5만개, 5만번 이후 사용 )
    NPC_OBJECT_KEY			= 100000,	//	NPC			: 10만 ~ 15만 ( 최대 5만개, 10만번 이후 사용 )
    NONCHARACTER_OBJECT_KEY	= 150000,	//	???			: 15만 ~ 20만 ( 최대 5만개, 15만번 이후 사용 )
    MAP_OBJECT_KEY			= 200000,	//  맵 오브젝트	: 20만 ~ 25만 
    ITEM_OBJECT_KEY			= 250000,	//	아이템		: 25만번 이후 사용( 아이템이 가장 많음 )
    PET_OBJECT_KEY			= 300000,	//  펫오브젝트	: 30만번 
    ONLYCLIENT_OBJECT_KEY	= 340000,	//< 클라에만 사용하는 키. 1만개만 사용
    ONLYCLIENT_BATTLEZONE_OBJECT_KEY	= 341000,	//< 클라에만 사용하는 키.
    MAX_OBJECT_KEY			= 350000,	
};
#endif

enum eOBJECT_TYPE
{
    OBJECT_OBJECT			= ( 1<<1 ),
    CHARACTER_OBJECT		= ( OBJECT_OBJECT | ( 1<<2 ) ),
    NONCHARACTER_OBJECT		= ( OBJECT_OBJECT | ( 1<<3 ) ),
    PLAYER_OBJECT			= ( CHARACTER_OBJECT | ( 1<<4 ) ),
    NPC_OBJECT				= ( CHARACTER_OBJECT | ( 1<<5 ) ),
    MONSTER_OBJECT			= ( NPC_OBJECT | ( 1<<6 ) ),
    SUMMON_OBJECT			= ( NPC_OBJECT | ( 1<<7 ) ),
    MERCHANT_OBJECT			= ( NPC_OBJECT | ( 1<<8 ) ),
    MAPNPC_OBJECT			= ( NPC_OBJECT | ( 1<<9 ) ),
    LUCKYMON_OBJECT			= ( MONSTER_OBJECT | ( 1<<10 ) ),
    ITEM_OBJECT				= ( NONCHARACTER_OBJECT | ( 1<<11 ) ),
    MAP_OBJECT				= ( NONCHARACTER_OBJECT | ( 1<<12) ),
    MONEY_OBJECT			= ( ITEM_OBJECT | ( 1<<13 ) ),
    CAMERA_OBJECT			= ( 1<<14 ),
    TRANSFORM_PLAYER_OBJECT	= ( 1<<15 ),
    TOTEMNPC_OBJECT			= ( NPC_OBJECT | ( 1<<16 ) ),
    PET_OBJECT				= ( CHARACTER_OBJECT | ( 1<<17 ) ),
    SSQMONSTER_OBJECT		= ( MONSTER_OBJECT | ( 1<<18 ) ),
    COLLECTION_OBJECT		= ( NONCHARACTER_OBJECT  | ( 1<<19 ) ),
    LOTTO_NPC_OBJECT		= ( NPC_OBJECT  | ( 1<<20 ) ),
    RIDER_OBJECT			= ( CHARACTER_OBJECT | ( 1<<21 ) ),
    CRYSTALWARP_OBJECT		= ( NPC_OBJECT | ( 1<<22 ) ),
    FRIEND_MONSTER_OBJECT	= ( MONSTER_OBJECT | ( 1<<23 ) ),   // _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    SYNC_MERCHANT_OBJECT    = ( MONSTER_OBJECT | ( 1<<24 ) ),   // _NA_0_20100222_UNIT_TRIGGERS_REGEN
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	STATUE_PLAYER_OBJECT    = ( CHARACTER_OBJECT | ( 1<<25 ) ),
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
    MAX_OBJECT				= (1<<31),
};

enum eOBJECT_STATE
{
    OBJECT_STATE_IDLE = 0,
    OBJECT_STATE_WALK,
    OBJECT_STATE_RUN,
    OBJECT_STATE_ATTACK,
    OBJECT_STATE_DAMAGE,
    OBJECT_STATE_THRUST,
};

enum eCHAR_MOVE_STATE
{
    CMS_WALK,
    CMS_RUN,
    CMS_SWIPE,
    CMS_KNOCKBACK,
    CMS_KNOCKBACK_DOWN,
    CMS_SIDESTEP,
    CMS_BACKSTEP,
    CMS_TUMBLING_FRONT,			// 발키리 대쉬
    CMS_TUMBLING_LEFT,			// 발키리 옆구르기
    CMS_TUMBLING_RIGHT,
    CMS_SHOULDER_CHARGE,		// 버서버 숄더차지
    CMS_SLIDING,				// 드래곤 슬라이딩
    CMS_TELEPORT,				// 엘리 잔상텔레포트
    CMS_TUMBLING_BACK,          // 발키리 백대쉬
    CMS_MONSTER_ATTACKJUMP,     // 몬스터 점프 공격
    CMS_LUCKY_MONSTER_RUNAWAY,	// 럭키몬스터 도망가는 속도
    CMS_STOP,
    CMS_KNOCKBACK2,				//__NA001048_080514_APPEND_ABILITY_KNOCKBACK2__
    CMS_MAX,
};

enum eZONE_TYPE
{
    ZT_VILLAGE					= 1,
    ZT_BATTLEZONE,
};


// 월드 서버내 유져 상태
enum eUSER_STATUS
{
    US_INVALID					= 0,
    US_IN_CHARSCENE,
    US_IN_VILLAGE,
    US_IN_BATTLEZONE,
};

enum eZONE_STATE
{
    ePRS_NOT_AT_ZONE				= ( 1<<1 ),

    ePRS_AT_ZONE					= ( 1<<2 ),
    ePRS_AT_VILLAGE					= ( ePRS_AT_ZONE | ( 1<<3 ) ),
    ePRS_AT_ROOM					= ( ePRS_AT_ZONE | ( 1<<4 ) ),
    ePRS_AT_LOBBY					= ( ePRS_AT_ROOM | ( 1<<5 ) ),
    ePRS_AT_MISSION					= ( ePRS_AT_ROOM | ( 1<<6 ) ),
    ePRS_AT_HUNTING					= ( ePRS_AT_ROOM | ( 1<<7 ) ),
    ePRS_AT_PVP						= ( ePRS_AT_ROOM | ( 1<<8 ) ),
    ePRS_AT_CHARSELECT				= ( ePRS_AT_ROOM | ( 1<<9 ) ),
    ePRS_AT_CHUNTING				= ( ePRS_AT_ROOM | ( 1<<10 ) ),
    ePRS_AT_FIELD					= ( ePRS_AT_ROOM | ( 1<<11 ) ),

    ePRS_BEFORE_ENTER_ZONE			= ( 1<<12 ),
    ePRS_BEFORE_ENTER_VILLAGE		= ( ePRS_BEFORE_ENTER_ZONE | ( 1<<13 ) ),
    ePRS_BEFORE_ENTER_ROOM			= ( ePRS_BEFORE_ENTER_ZONE | ( 1<<14 ) ),
    ePRS_BEFORE_ENTER_LOBBY			= ( ePRS_BEFORE_ENTER_ROOM | ( 1<<15 ) ),
    ePRS_BEFORE_ENTER_MISSION		= ( ePRS_BEFORE_ENTER_ROOM | ( 1<<16 ) ),
    ePRS_BEFORE_ENTER_HUNTING		= ( ePRS_BEFORE_ENTER_ROOM | ( 1<<17 ) ),
    ePRS_BEFORE_ENTER_PVP			= ( ePRS_BEFORE_ENTER_ROOM | ( 1<<18 ) ),
    ePRS_BEFORE_ENTER_CHARSELECT	= ( ePRS_BEFORE_ENTER_ROOM | ( 1<<19 ) ),
    ePRS_BEFORE_ENTER_CHUNTING		= ( ePRS_BEFORE_ENTER_ROOM | ( 1<<20 ) ),
    ePRS_BEFORE_ENTER_FIELD			= ( ePRS_BEFORE_ENTER_ROOM | ( 1<<21 ) ),

    ePRS_AT_INSTANCE				= ( ePRS_AT_ROOM | ( 1<<22 ) ),
    ePRS_BEFORE_ENTER_INSTNACE		= ( ePRS_BEFORE_ENTER_ROOM | ( 1<<23 ) ),	

    ePRS_AT_DOMINATION_FIELD		= ( ePRS_AT_ROOM | ( 1<<24 ) ),
    ePRS_BEFORE_ENTER_DOMINATION_FIELD = ( ePRS_BEFORE_ENTER_ROOM | ( 1<<25 ) ),

    //_NA_008069_20150210_ADD_SPA_SYSTEM
    ePRS_AT_SPA_FIELD               = ( ePRS_AT_ROOM | ( 1<<26 ) ),
    ePRS_BEFORE_ENTER_SPA_FIELD     = ( ePRS_BEFORE_ENTER_ROOM | ( 1<<27 ) ),
};


enum eSERIALIZE
{
    SERIALIZE_STORE,
    SERIALIZE_LOAD,
};

enum eINVISIBLE_OPTION_FLAG
{
    eINVISIBLE_HELMET	= (1<<0),
    eINVISIBLE_CAPE = (1<<1),
    eINVISIBLE_WING = (1<<2),
    eINVISIBLE_COSTUME = (1<<3), // _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
};


/////////////////////////////////////////////////////////////////////////////////////////
// Room
/////////////////////////////////////////////////////////////////////////////////////////
enum eZONEPUBLIC
{
    eZONEPUBLIC_PUBLIC = 0,
    eZONEPUBLIC_PRIVATE,
};

enum eZONETYPE
{
    eZONETYPE_LOBBY				=  0,
    eZONETYPE_VILLAGE			=  1,
    eZONETYPE_CHARSELECT		=  2,
    eZONETYPE_MISSION			=  3,
    eZONETYPE_HUNTING			=  4,
    eZONETYPE_QUEST				=  5,
    eZONETYPE_PVP				=  6,
    eZONETYPE_EVENT				=  7,
    eZONETYPE_GUILD				=  8,
    eZONETYPE_SIEGE				=  9,
    eZONETYPE_FIELD				= 10,
    eZONETYPE_CHUNTING			= 11,	
    // eZONETYPE_TUTORIAL_FIELD는 eZONETYPE_FIELD와 동일하며, eZONETYPE_FIELD의 모든 로직을 그대로 사용한다.
    // (GetZoneType() 사용시 eZONETYPE_FIELD로 나온다는 의미)
    // eZONETYPE_FIELD 타입과의 구분은 sMAPINFO를 통해서만 구분할 수 있도록 한다.
    eZONETYPE_TUTORIAL_FIELD	= 12,	//
    eZONETYPE_INSTANCE			= 13,
//__NA001385_20090924_DOMINATION_BASE
    eZONETYPE_DOMINATION_FIELD	= 14,
    eZONETYPE_AIRSHIP_FIELD 	= 15,
    //_NA_008069_20150210_ADD_SPA_SYSTEM
    eZONETYPE_SPA_FIELD         = 16,
    eZONETYPE_MAX				= 17,	
};

enum eHUNTING_MONSTER_DIFFICULT
{
    eHUNTING_MONSTER_DIFFICULT_EASY = 0,
    eHUNTING_MONSTER_DIFFICULT_NORMAL,
    eHUNTING_MONSTER_DIFFICULT_HARD,
    eHUNTING_MONSTER_DIFFICULT_MAX,
};

enum eHUNTING_BONUS_TYPE
{
    eHUNTING_BONUS_TYPE_ITEM = 0,
    eHUNTING_BONUS_TYPE_EXPERIENCE,

    eHUNTING_BONUS_TYPE_MAX,
};
enum ePVP_RULE_TYPE
{
    ePVP_RULE_10 = 0,
    ePVP_RULE_30,
    ePVP_RULE_INFINITY,

    ePVP_RULE_MAX,

};
enum ePVP_MODE_TYPE
{
    ePVP_PERSONAL_MODE = 0,
    ePVP_TEAM_MODE,

    ePVP_MODE_MAX,
};

// 경쟁헌팅 모드
enum eCOMPETITION_RULE_TYPE
{
    eCOMPETITION_RULE_FREE_FOR_ALL,
    eCOMPETITION_RULE_DUAL_PARTY,

    eCOMPETITION_RULE_MAX,

};

enum eCOMPETITION_MODE_TYPE
{
    eCOMPETITION_MODE_SURVIVAL,			// 서바이벌 모드
    eCOMPETITION_MODE_TIME_ATTACK,		// 타임 어택 모드
    eCOMPETITION_MODE_DEATH_MATCH,		// 데쓰매치 모드

    eCOMPETITION_MODE_MAX,
};

// 기능 제한 타입, 카오상태일 때도 기능을 막아야 한다!
enum eFUNCTIONALRESTRICT_TYPE
{
    eFUNCTIONALRESTRICT_ALLOW					= 0,
    eFUNCTIONALRESTRICT_ENCHANT					= (1<<0),
    eFUNCTIONALRESTRICT_RANKUP					= (1<<1),
    eFUNCTIONALRESTRICT_ADDSOCKET				= (1<<2),
    eFUNCTIONALRESTRICT_FILLSOCKET				= (1<<3),
    eFUNCTIONALRESTRICT_EXTRACTSOCKET			= (1<<4),
    eFUNCTIONALRESTRICT_COMPOSE					= (1<<5),
    eFUNCTIONALRESTRICT_CRYSTALIZE				= (1<<6),
    eFUNCTIONALRESTRICT_USE_EXTRASTONE			= (1<<7),
    eFUNCTIONALRESTRICT_USE_WAREHOUSE			= (1<<8),	//< USE_빼야하는데ㅜ_ㅜ
    eFUNCTIONALRESTRICT_USE_TRADE				= (1<<9),	//< USE_빼야하는데ㅜ_ㅜ
    eFUNCTIONALRESTRICT_USE_VENDOR				= (1<<10),	//< USE_빼야하는데ㅜ_ㅜ
    eFUNCTIONALRESTRICT_USE_GUILD				= (1<<11),	//< USE_빼야하는데ㅜ_ㅜ
    eFUNCTIONALRESTRICT_USE_MONSTERDROP			= (1<<12),	//< USE_빼야하는데ㅜ_ㅜ
    eFUNCTIONALRESTRICT_COMBINE_ITEM			= (1<<13),
    eFUNCTIONALRESTRICT_DIVIDE_ITEM				= (1<<14),
    eFUNCTIONALRESTRICT_REPAIR_ITEM				= (1<<15),
    eFUNCTIONALRESTRICT_SELL_ITEM				= (1<<16),	//< 2006/9/13 추가
    eFUNCTIONALRESTRICT_BUY_ITEM				= (1<<17),	//< 2006/9/13 추가
    eFUNCTIONALRESTRICT_EXCHANGE_ITEM			= (1<<18),	//< 2006/9/13 추가
    eFUNCTIONALRESTRICT_VENDOR_LIST_NPC			= (1<<19),	//< 2006/10/19 추가
    eFUNCTIONALRESTRICT_BUY_SHOPITEM			= (1<<20),	//< 2006/12/9
    eFUNCTIONALRESTRICT_SHOP_SERVICE			= (1<<21),	//< 2006/12/9
    eFUNCTIONALRESTRICT_GM_COMMAND				= (1<<22),	//< 2007/06/19
    eFUNCTIONALRESTRICT_PARTY_RETURN_FOR_UCUSER	= (1<<23),	//< 2007/09/27
    eFUNCTIONALRESTRICT_WAREINVEN_LOCK_BY_PWD	= (1<<24),	//< 2007/11/07	__TASK_WAREHOUSE_N_INVENTORY_LOCK_BY_PWD__ 국가별 기능 제한 걸기 위한...
    eFUNCTIONALRESTRICT_ALL						= ((1<<25)-1),
};


/////////////////////////////////////////////////////////////////////////////////////////
// Item
/////////////////////////////////////////////////////////////////////////////////////////

// eITEMTYPE의 무기의 번호와 일치해야 한다!!!!
enum eWEAPONTYPE
{
    eWEAPONTYPE_INVALID			= 0,
    eWEAPONTYPE_TWOHANDAXE		= 1,
    eWEAPONTYPE_TWOHANDSWORD	= 2,
    eWEAPONTYPE_ONEHANDSWORD	= 3,
    eWEAPONTYPE_SPEAR			= 4,
    eWEAPONTYPE_DAGGER			= 5,
    eWEAPONTYPE_WHIP			= 6,
    eWEAPONTYPE_ONEHANDCROSSBOW	= 7,
    eWEAPONTYPE_ETHERWEAPON		= 8,
//	eWEAPONTYPE_SCIMITAR		= 9,
    eWEAPONTYPE_STAFF			= 10,
    eWEAPONTYPE_ORB				= 11,
    eWEAPONTYPE_FISHING_POLE	= 12,
    eWEAPONTYPE_PUNCH			= 13,//순서 주의 할것 
    eWEAPONTYPE_GUNTLET = 14, // 건틀렛 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eWEAPONTYPE_POLEARM = 15, // 철퇴 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eWEAPONTYPE_HELLROID_1 = 16, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eWEAPONTYPE_ARCBLADE = 17, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    eWEAPONTYPE_MAX,
};

// 장비, 사용여부
enum eITEM_MATERIAL_TYPE
{
    eITEM_MATERIAL_TYPE_NOT_USE_AND_EQUIP = 0,	// 착용 / 사용 불가
    eITEM_MATERIAL_TYPE_CAN_EQUIP,				// 착용 가능
    eITEM_MATERIAL_TYPE_CAN_USE,				// 사용 가능(소모)
    eITEM_MATERIAL_TYPE_CAN_BOTH,				// 착용 / 사용 가능 
    eITEM_MATERIAL_TYPE_CAN_USE_NOT_DELETE,		// 사용 가능(삭제불가)
    eITEM_MATERIAL_TYPE_NOT_USE_CAN_DELETE, // 사용 불가 / 삭제 가능
};


// 장착창의 아이템별 위치 
enum eEQUIPCONTAINER_POS
{
    eEQUIPCONTAINER_WEAPON		= 0,
    eEQUIPCONTAINER_ARMOR		= 1,
    eEQUIPCONTAINER_PROTECTOR	= 2,
    eEQUIPCONTAINER_HELMET		= 3,
    eEQUIPCONTAINER_PANTS		= 4,
    eEQUIPCONTAINER_BOOTS		= 5,
    eEQUIPCONTAINER_GLOVE		= 6,
    eEQUIPCONTAINER_BELT		= 7,
    eEQUIPCONTAINER_SHIRTS		= 8,
    eEQUIPCONTAINER_RING1		= 9,
    eEQUIPCONTAINER_RING2		= 10,
    eEQUIPCONTAINER_NECKLACE	= 11,
    eEQUIPCONTAINER_BOW			= 12,
    eEQUIPCONTAINER_SACCESSORY1	= 13,
    eEQUIPCONTAINER_SACCESSORY2 = 14,
    eEQUIPCONTAINER_SACCESSORY3 = 15,
    eEQUIPCONTAINER_CHARGE1		= 16,
    eEQUIPCONTAINER_CHARGE2		= 17,
    eEQUIPCONTAINER_CHARGE3		= 18,
    eEQUIPCONTAINER_CHARGE4		= 19,
    eEQUIPCONTAINER_CHARGE5		= 20,

    eEQUIPCONTAINER_PC_ROOM1	= 21,
    eEQUIPCONTAINER_PC_ROOM2	= 22,
    eEQUIPCONTAINER_PC_ROOM3	= 23,

    eEQUIPCONTAINER_MAX,
    //망토
    //eEQUIPCONTAINER_CAPE		= xx,
    //문장 현재 사용 안함
    //eEQUIPCONTAINER_CREST		= xx,

    //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    // 91, 92, 93 - USED (eEQUIPCONTAINER_POS_FOR_RIDER_PARTS)

    eEQUIPCONTAINER_INVALID		= 99,
};

//--------------------------------------------------------------------------------------------------
// __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
// 탈 것 종류 (클래스 구분)
struct eRIDER_CLASS
{
    enum eValue {
        None        = 0,
        Charger     , //1 에텔레인 군마
        Leopard     , //2 큰발 표범
        OrcPig      , //3 오크 피그
        Kalvus      , //4 칼버스
        MaxCounts   , //
    };
    BYTE    value;

    // (usage)
    // eRIDER_CLASS rider;
    // rider.value = rider.Charger;
    // assert(sizeof(rider) == 1);
    // assert(rider.value == rider.Charger);
    // eRIDER_CLASS rider_clone = rider;
    // assert(rider.value == rider_clone.value);
};

// 탈 것 장착창의 아이템별 위치 
struct eRIDER_PARTS_POS
{
    enum eValue {
        None        = 0, //이건 파츠 아이템이 아니다.
        StartIndex  = 91,
        HeadPart    = StartIndex,
        BodyPart    , //92
        LegsPart    , //93
        EndIndex    , //94
        Counts      = EndIndex - StartIndex
    };
    BYTE    value;
};
//--------------------------------------------------------------------------------------------------


/*
--2010.05.20 국내 아이템 스크립트 기준

0 : 모두가능
1 : 거래만 불가능
2 : 판매만 불가능
4 : 바닥에 드롭만 불가능
8 : 다른슬롯에 이동 불가능
16 : 휴지통아이콘 삭제만 불가능
32 : 길드 창고및 개인창고 이동불가능
64: 기간제아이템 착용(시간이 흘러감) 시 거래 불가능
128: 캐시샾 선물하기 불가능
256: 길드창고 이동만 불가능
512: 창고 이동만 불가능(길드창고는 이동 가능하지만 창고 이동만 불가능 할경우)
1024: 바닥에 드롭 불가능 & PK 드롭 가능
65535 : 모두 불가능*/

enum eITEM_TRADESELL_TYPE
{
    eITEM_TRADESELL_DOALL			= (0),			//< 0
    eITEM_TRADESELL_TRADE			= (1<<0),		//< 1
    eITEM_TRADESELL_SELL			= (1<<1),		//< 2
    eITEM_TRADESELL_DROP			= (1<<2),		//< 4
    eITEM_TRADESELL_SLOT			= (1<<3),		//< 8
    eITEM_TRADESELL_DELETE			= (1<<4),		//< 16	
    eITEM_TRADESELL_WAREHOUSE		= (1<<5),		//< 32
    eITEM_TRADESELL_USED_ITEM		= (1<<6),		//< 64
    eITEM_TRADESELL_CASH_PRESENT	= (1<<7),		//< 128		//__NA001087_080618_APPEND_CASHITEM_PRESENT_PROTECTION_FUNTION__
    eITEM_TRADESELL_GUILDWAREHOUSE  = (1<<8),		//< 256     
    eITEM_TRADESELL_ONLY_WAREHOUSE  = (1<<9),       //< 512     //_NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION : 창고 이동만 불가능 옵션
    eITEM_TRADESEEL_DROP_CAN_PKDROP = (1<<10),      //< 1024    //_NA_006052_20120904_CANNOT_DROP_CAN_PKDROP : 바닥에 드롭 불가능 하고 PK 드롭 시 가능
    eITEM_TRADESELL_DONTALL			= ((WORD)-1),	//< 65535
};

enum eITEMTYPE
{
    eITEMTYPE_INVALID			= 0,	
    // 무기
    eITEMTYPE_TWOHANDAXE		= 1,	//	양손도끼1
    eITEMTYPE_TWOHANDSWORD		= 2,	//	양손검1
    eITEMTYPE_ONEHANDSWORD		= 3,	//	한손검1
    eITEMTYPE_SPEAR				= 4,	//	창1
    eITEMTYPE_DAGGER			= 5,	//	단검1
    eITEMTYPE_WHIP				= 6,	//	채찍1
    eITEMTYPE_ONEHANDCROSSBOW	= 7,	//	한손석궁1
    eITEMTYPE_ETHERWEAPON		= 8,	//	양손석궁1,에테르웨폰
    eITEMTYPE_SCIMITAR			= 9,	//	곡도1
    eITEMTYPE_STAFF				= 10,	//	지팡이1
    eITEMTYPE_ORB				= 11,	//	오브1
    eITEMTYPE_FISHING_POLE		= 12,	//  낚시대
    eITEMTYPE_PUNCH				= 13,	//	맨주먹
    eITEMTYPE_GUNTLET = 14, // 건틀렛 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eITEMTYPE_POLEARM = 15, // 철퇴 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eITEMTYPE_HELLROID_1 = 16, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eITEMTYPE_ARCBLADE = 17, // 아크블레이드 _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    // 방어구
    eITEMTYPE_ARMOR				= 101,	//	갑옷
    eITEMTYPE_PROTECTOR			= 201,	//	프로텍터
    eITEMTYPE_HELMET			= 301,	//	헬멧
    eITEMTYPE_PANTS				= 401,	//	바지
    eITEMTYPE_BOOTS				= 501,	//	부츠
    eITEMTYPE_GLOVE				= 601,	//	장갑
    eITEMTYPE_BELT				= 701,	//	벨트
    eITEMTYPE_SHIRTS			= 801,	//	셔츠

    // 악세서리
    eITEMTYPE_RING				= 811,	//	반지 
    eITEMTYPE_NECKLACE			= 821,	//	목걸이

    // 특수 악세서리
    /*
    eITEMTYPE_CAPE				= 831,	//	망토
    eITEMTYPE_WING				= 841,	//	날개
    eITEMTYPE_STONE				= 851,	//	속성석
    eITEMTYPE_SPRITE			= 861,	//	수호정령, 제네레이터
    eITEMTYPE_CREST				= 871,	//	문장
    */
    // 클래스별 특수 악세서리로 변경
    eITEMTYPE_BERSERKER_SACCESSORY	= 831,	//	망토
    eITEMTYPE_DRAGON_SACCESSORY		= 841,	//	날개
    eITEMTYPE_VALKYRIE_SACCESSORY	= 851,	//	속성석
    eITEMTYPE_SHADOW_SACCESSORY		= 861,	//	수호정령, 제네레이터
    eITEMTYPE_ELEMENTALIST_SACCESSORY = 871,	//	문장

    eITEMTYPE_BERSERKER_KALBUS_ACCESSORY	= 881,
    eITEMTYPE_DRAGON__KALBUS_ACCESSORY		= 882,
    eITEMTYPE_VALKYRIE__KALBU_ACCESSORY		= 883,
    eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY = 884,
    eITEMTYPE_SHADOW_KALBU_ACCESSORY		= 888,

    eITEMTYPE_FISHING_ROD			= 885,
    eITEMTYPE_FISHING_BAIT			= 886,
    eITEMTYPE_FISH					= 887,

    eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1 = 891, // 미스틱 특수 악세서리1 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY2 = 892, // 미스틱 특수 악세서리2 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1 = 893, // 헬로이드 특수 악세서리1 _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eITEMTYPE_HELLROID_SPECIAL_ACCESSORY2 = 894, // 헬로이드 특수 악세서리2 _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1 = 889, // 위치블레이드 특수 악세서리1 _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY2 = 890, // 위치블레이드 특수 악세서리2 _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    
    eITEMTYPE_ALLCLASS_CHAOSEYE = 895,                     // 타입확장(871) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_ELEMENTALIST_KALVUS_ACCESSORY = 899,         // 타입확장(871) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_BERSERKER_KALVUS_ACCESSORY = 896,            // 타입확장(831) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_DRAGON_KALVUS_ACCESSORY = 897,               // 타입확장(841) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_VALKYRIE_SHADOW_KALVUS_ACCESSORY = 898,      // 타입확장(851) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_MYSTIC_KALVUS_ACCESSORY = 900,               // 타입확장(891) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_HELLROID_WITCHBLADE_KALVUS_ACCESSORY = 901,  //타입확장(893) _NA_007827_20141117_ADD_ACCESSORY_TYPE

    eITEMTYPE_WASTE					= 911,	//	소모성아이템
    eITEMTYPE_ENCHANT				= 921,	//	인첸트보석1
    eITEMTYPE_CRYSTAL				= 931,	//	결정체A
    eITEMTYPE_SOCKET				= 941,	//	소켓아이템
    eITEMTYPE_ARROW					= 951,	//	석궁용 화살

    eITEMTYPE_SOCKETMATERIAL		= 961,	//	소켓재료
    eITEMTYPE_ACCESSORYMATERIAL		= 971,	//	악세서리재료
    eITEMTYPE_SACCESSORYMATERIAL	= 981,	//	특수악세서리재료

    eITEMTYPE_STORY					= 982,
    eITEMTYPE_EXPECT				= 983,
    eITEMTYPE_NORMAL				= 984,
    eITEMTYPE_GAMBLE				= 985,
    eITEMTYPE_SPECIAL				= 986,
    eITEMTYPE_GUILD 				= 987,
    eITEMTYPE_AC					= 988,
    eITEMTYPE_EVENT					= 989,
    eITEMTYPE_JUNK					= 990,
    eITEMTYPE_QUEST					= 991,
    eITEMTYPE_RANDOM				= 992,	// 랜덤아이템(무기/장비/기타아이템)
    eITEMTYPE_TOTEM					= 993,	// 토템아이템
    eITEMTYPE_TOP_OF_TRIAL			= 994,	// 저주의탑 입장 아이템
    eITEMTYPE_TOP_OF_TRIAL_CHARGE	= 995,	// 저주의탑 입장 아이템(캐쉬아이템)
    eITEMTYPE_INDUN_LIMITED			= 994,	// 인던 입장 아이템 (시간 제약 존재)
    eITEMTYPE_INDUN_UNLIMITED		= 995,	// 인던 입장 아이템 (시간 제약 없음)
    eITEMTYPE_PORTAL_QUEST_ITEM		= 996,	// 일단 100레벨 용도이고 (성격:맵이동+퀘스트아이템)이고, 이후 확장할 수 있도록 하자.
    eITEMTYPE_ETHER_BULLET			= 997,	// 에테르 탄환  __NA_000968_ETHER_EMISSION_DEVICE
    eITEMTYPE_ETHER_DEVICE			= 998,	// 에테르 장치	__NA_000968_ETHER_EMISSION_DEVICE
    eITEMTYPE_RENDER_EFFECT_ITEM	= 999,	// Inventory에 존재하지만 외부로 보여지는 Effect가 존재할 경우
                                            // 벳지아이템(_NA_006372_20130108_BADGE_ITEM)
    eITEMTYPE_BILL					= 1000,	//	화폐
    eITEMTYPE_ETHERIA               = 1001, // 에테리아 _NA000000_100427_ITEM_CUSTOMIZING_CONTENTS_
    eITEMTYPE_EVENT_FIELD_PARTY_ALL_MEMBER_PICK_UP = 1002, // 필드 파티 전원 습득용, 이벤트 아이템
    eITEMTYPE_ETHEREALIZER          = 1003, // 에테리아 분리 _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_
    eITEMTYPE_WINGS                 = 1004, // _NA001955_110210_WING_COSTUME
    eITEMTYPE_DAILYQUEST_RESET      = 1005, // 일일퀘스트 초기화 아이템

    // 유료화 관련 추가 아이템
    eITEMTYPE_SUPERMASTER			= 20000,	// 슈퍼방장
    eITEMTYPE_PORTAL				= 20001,	// 포탈용아이템
    eITEMTYPE_SAVE_COORD			= 20002,	// 좌표 저장 아이템
    eITEMTYPE_SOCKETEXTRACT			= 20003,	// 소켓제거아이템
    eITEMTYPE_CHAT_SHOUT			= 20004,	// 외치기아이템(기간제)
    eITEMTYPE_RECALL				= 20005,	// 몬스터소환아이템
    eITEMTYPE_REPAIR				= 20006,	// 내구도수리아이템
    eITEMTYPE_HOLDDROP				= 20007,	// 드랍방지용아이템
    eITEMTYPE_STAT_RESET			= 20008,	// 스텟초기화아이템(힘,민첩,체력,지력,정신력,숙련도1,2,스킬포인트)
    eITEMTYPE_EXTRATAB				= 20009,	// 탭추가아이템(사용시 1탭씩 추가)
    eITEMTYPE_CLEAR_COORD			= 20010,	// 좌표 초기화 아이템
    eITEMTYPE_RECALL_ALLIANCE		= 20011,	// 여신의 은혜 아이템
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    eITEMTYPE_GUILD_PVP_ENTRANCE	= 20012,	// 길드 PVP 입장권
#endif
    eITEMTYPE_EXTEND_CASH_SLOT		= 20013,	// 캐쉬 슬롯 2개 확장
    eITEMTYPE_EXTEND_DATE_WEAPON	= 20014,	// 기간연장 아이템(무기)	
    eITEMTYPE_RANK_DOWN				= 20015,	// 랭크하향 망치
    eITEMTYPE_EXTEND_DATE_ARMOR		= 20016,	// 기간연장 아이템(방어구)	
    eITEMTYPE_HAMMER_OF_ALCHEMIST	= 20017,	// 연금술사의 망치(ALCHEMIST's hammer) 리미티드 아이템 수리
    eITEMTYPE_GUILDMARK_SEAL        = 20018,
    eITEMTYPE_HOLDDROP2				= 20019,	// 종속의 고리2  __NA_825_HOLDDROP2
    eITEMTYPE_PET					= 20020,	// 펫 __NA_0_PET_SYSTEM
    eITEMTYPE_PET_FEED				= 20021,	// 펫 __NA_0_PET_FEED

//_YJ_FISHING_SYSTEM_000933
    eITEMTYPE_FISH_HYBRID			= 20022,	//잡종   물고기
    eITEMTYPE_FISH_WASTE			= 20023,	//소모성 물고기
    eITEMTYPE_FISH_HIGH_CLASS		= 20024,	//고급   물고기
    eITEMTYPE_FISH_RARE				= 20025,	//희귀   물고기
    eITEMTYPE_FISH_AC				= 20026,	//AC     물고기
    eITEMTYPE_FISH_CASH				= 20027,	//캐시   물고기
//_YJ_FISHING_SYSTEM_000933
    eITEMTYPE_GUILDWAREHOUSE_SLOTCASH = 20028,	//길드 창고 슬롯 확장
    eITEMTYPE_LOTTO					= 20029,	//복권 아이템
    eITEMTYPE_CHAT_SHOUT_CONSUME	= 20030,	

    eITEMTYPE_FIRECRACKER			= 20031,	
    eITEMTYPE_POLLEN				= 20032,	
    eITEMTYPE_TORCH					= 20033,	
    eITEMTYPE_MINE					= 20034,	
    eITEMTYPE_SSQ					= 20035,
    eITEMTYPE_STAT_CHANGE			= 20036, //아이템 스텟 교환
    eITEMTYPE_PET_NAME_CARD			= 20037, //< 펫 이름 설정 아이템, __NA_1131_PET_NAME_SETTING

    eITEMTYPE_WAYPOINT				= 20038, // 웨이포인트 아이템, __NA001164_20081024_CHAR_WAY_POINT_ITEM
    eITEMTYPE_CHANGE_GUILD_NAME     = 20039,

    //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    eITEMTYPE_RIDER                 = 20040, // Rider Item
    eITEMTYPE_RIDER_PARTS           = 20041, // Rider Parts Item { HEAD, BODY, LEGS }
    eITEMTYPE_EXTEND_DATE_RIDER     = 20042, // Extend date of time : Rider|Parts with timer
    eITEMTYPE_WAX                   = 20044, // Wax Item for rider parts item (shining enchant)

    // [11/2/2009 namka199] _CN_0_20091013_RANKUP_EXTEND
    eITEMTYPE_RANKUP_GRADE_RAISER   = 20045, // 랭크업시 필요 아이템(부적의 돌)
    eITEMTYPE_RANKUP_OPTION_CHANGER = 20046, // 랭크업시 옵션을 변경해주는 아이템(초기화의 돌)
    
    // _NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
    eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL = 20047, // 캐릭터 모든 외모 변경
    eITEMTYPE_CHANGE_CHARACTER_FACE = 20048, // 캐릭터 얼굴 변경
    eITEMTYPE_CHANGE_CHARACTER_HAIR = 20049, // 캐릭터 헤어 변경
    eITEMTYPE_CHANGE_CHARACTER_HEIGHT = 20050, // 캐릭터 키 변경

    //_NA_0_20100722_AUTO_SYSTEM
    eITEMTYPE_AUTO_HUNTING   = 20051,      // 자동 사냥 아이템
    //_NA_0_20100727_SOKET_SYSTEM_CONVERT
    eITEMTYPE_SOCKET_ZARD	  = 20052,	            // new 소켓아이템 (zard)
    eITEMTYPE_SOCKET_IDENTIFY = 20055,	            // 소켓 갯수 확인 아이템 
    eITEMTYPE_SOCKET_COMPOSE_ADDITIVE = 20054,	    // new 소켓아이템 첨가제

    //_NA002217_100728_EP2_PERK_SYSTEM_
    eITEMTYPE_PERK_CASH_ITEM   = 20053,      // 퍽캐시 아이템

    // _NA_000587_20100928_DOMINATION_BUFF_ITEM
    eITEMTYPE_DOMINATION_BUFF = 20056, // 피의 성전 강화 버프 아이템

    // _NA_20101011_HEIM_LOTTO_CASH_TICKET
    eITEMTYPE_HEIM_LOTTO_TICKET = 20057,            // 하임행운 캐시 응모권

    // _NA_002253_20100811_CUBE_SYSTEM
    eITEMTYPE_CUBE = 20058, // 아이템을 합성 또는 분해하는 큐브 아이템

    // _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    eITEMTYPE_DOMINATION_DEFFENSE_POINT = 20059,
    eITEMTYPE_DOMINATION_DEFFENSE_CASH_POINT = 20060,

    // _NA_002050_20110216_ADD_GENDER
    eITEMTYPE_CHANGE_CHARACTER_GENDER = 20061, // 캐릭터 성별 변경
    eITEMTYPE_EXTEND_CHARACTER_SLOT = 20062, // 캐릭터 슬롯 확장

    // _NA_003740_20111122_RANDOM_ITEM
    eITEMTYPE_RANDOMIZE_ITEM = 20063, // 아이템 랜덤화
    
    // _NA_004035_20120227_COSTUME_ITEM
    eITEMTYPE_COSTUME = 20064, // 코스튬
    eITEMTYPE_PCBANG = 20065, // 프리미엄서비스 아이템(PC방)

    eITEMTYPE_CHARACTER_NAME_CHANGE = 20066,
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION 
    eITEMTYPE_GUILDFACILITY_RESET = 20067,
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    eITEMTYPE_SPA_WARP_SCROLL = 20068, // 온천 이동 스크롤
    eITEMTYPE_SPA_TICKET      = 20069, // 온천 입장권
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    eITEMTYPE_AWAKENING_ALLROUND_MATERIAL = 20070, // 만능 각성 재료
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    eITEMTYPE_MINIGAME_BINGO_BALL = 20071,          // 미니게임빙고 볼
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    //_NA_008610_20160104_ADD_DECORATECOSTUME
    eITEMTYPE_COSTUME_DECORATE = 20072,             // 꾸밈코스튬

    eITEMTYPE_MAX,
};

enum eITEMSUBTYPE
{
    eITEMSUBTYPE_GENERAL		= 0,
    eITEMSUBTYPE_ELITE			= 1,
    eITEMSUBTYPE_UNIQUE			= 2,
    eITEMSUBTYPE_ELITE_SSQ      = 3, // (f100730.3L) add a type for ssq equipments which is equal to ELITE
    //eITEMSUBTYPE_DIVINE			= 3,	// eITEMSUBTYPE_DIVINE 은 사용하지 않는다. 디바인아이템의 확인은 SCItemSlot::IsDivine() 를 이용
    eITEMSUBTYPE_RARE           = 4,
    eITEMSUBTYPE_DIVINE         = 5,    //_NA_007408_20140711_MODIFY_CRYSTALLIZE

    eITEMSUBTYPE_FATE			= 11,
};

// 아이템 고유 레벨값
struct ItemCustomLevel
{
    // changed rules by _NA_002128_20110228_MODIFY_ITEM_LEVEL
#ifdef _NA_002935_20110704_ITEM_SCORE
    enum Value
    {
        kMinLV = 1, // 계산 가능한 최소 레벨
        kGeneralMaxLV = 21, // 일반 아이템 최고 레벨 (레어포함)
        kSuperJapanLV = 21, // 일본 최강
        kCurruptTempleLV = 23, // 타락한 사원
		kFighter_Elite2_0LV = 25,
		kFighter_Elite3_0LV = 27, // 투사의 엘리트 3차.

        kElite1_0stLV = 23, // 엘리트 1차
        kElite1_5stLV = 24, // 엘리트 1.5차
        kElite2_0stLV = 25, // 엘리트 2차
        kElite2_5stLV = 26, // 엘리트 2.5차
		kElite3_0stLV = 27, // 엘리트 3차
        kElite3_5stLV = 28, // 엘리트 3.5차
		kElite4_0stLV = 29, // 엘리트 4차

        kUnique1_5stLV = 31, // 유니크 1.5차
        kUnique2_5stLV = 32, // 유니크 2.5차
		kUnique1_0stLV = 33, // 유니크 1차
        kUnique2_0stLV = 34, // 유니크 2차
        
		kMaxLV = 39, // 계산 가능한 최대 레벨
    };
#else
    enum Value
    {
        kMinLV = 1, // 계산 가능한 최소 레벨
        kGeneralMaxLV = 17, // 일반 아이템 최고 레벨
        kSuperJapanLV = 17, // 일본 최강
        kCurruptTempleLV = 21, // 타락한 사원
        kElite1_0stLV = 21, // 엘리트 1차
        kElite1_5stLV = 23, // 엘리트 1.5차
        kElite2_0stLV = 25, // 엘리트 2차
        kUnique1_0stLV = 31, // 유니크 1차
        kUnique2_0stLV = 35, // 유니크 2차
        kMaxLV = 39, // 계산 가능한 최대 레벨
    };
#endif
};

// NOTE: f110414.x, declared by hukim, 아이템 계열 상세 타입
struct ItemSubTypeDetail
{
    enum Value
    {
        kInvalid = 0, // 유효하지 않은 타입
        kGeneral = 1, // 일반
        kSuperJapan = 2, // 일본 최강
        kCurruptTemple = 3, // 타락한 사원
        kElite1_0st = 4, // 엘리트 1차
        kElite1_5st = 5, // 엘리트 1.5차
        kElite2_0st = 6, // 엘리트 2차
        kUnique1_0st = 7, // 유니크 1차
        kUnique2_0st = 8, // 유니크 2차
        kFate = 9, // 럭키
        kRare = 10, // 레어
		kFigter_Elite2_0st = 11, // 투사 엘리트 2차
		kFigter_Elite3_0st = 12, // 투사 엘리트 3차

		kElite2_5st = 13, // 엘리트 2.5차
		kElite3_0st = 14, // 엘리트 3차
        kElite3_5st = 15, // 엘리트 3.5차
		kElite4_0st = 16, // 엘리트 4차

        kUnique1_5st = 17, // 유니크 1.5차
        kUnique2_5st = 18, // 유니크 2.5차
    };
    ItemSubTypeDetail() : value(0)
    {
    }
    BYTE value; // CHANGES: f110414.1L, waverix, to use field in 'BASE_ITEMINFO'
};
typedef ItemSubTypeDetail::Value ItemSubTypeDetailValue;

enum eWASTETYPE
{
    eITEMWASTE_HPPOTION        = 1,
    eITEMWASTE_MPPOTION        = 2,
    eITEMWASTE_FUNCTION        = 3,    //< 기능성 아이템
    eITEMWASTE_RETURNSCROLL    = 4,    //< 귀환 스크롤 아이템
    eITEMWASTE_QUESTACCEPT     = 5,    //< 퀘스트 수락 요청 아이템
    eITEMWASTE_UPSTONE         = 6,    //< UP 올려 주는 아이템
    eITEMWASTE_REVENGESTONE    = 7,    //< 복수의 돌
    eITEMWASTE_LEARN_SKILL     = 8,    //< 스킬습득
    eITEMWASTE_STAT_POINT_UP   = 9,    //< 스탯포인트 업 아이템
    eITEMWASTE_PET_FEED        = 10,   //< 펫먹이 __NA_0_PET_FEED
    eITEMWASTE_ETHER_BULLET    = 11,   //< 에테르 방출 장치의 총알 __NA_000968_ETHER_EMISSION_DEVICE
    eITEMWASTE_GUILDWAREHOUSE_SLOTCASH = 12,   //< 길드 확장 자재 
    eITEMWASTE_EXP_POTION              = 13,   //< 경험치 물약
    eITEMWASTE_CHAO_TIME_DECREASE      = 14,   //< 카오시간 감소
    eITEMWASTE_FUNCTION_HOLDUP_BUFF_POST_DEAD = 15,   //< 기능성 아이템 죽어도 사라지지 않는 버프 __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__    
    eITEMWASTE_ETHEREALIZER_SHELL = 16, // < #544, change to shell, declared by _NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
    eITEMWASTE_ETHEREALIZER_ETHERIA = 17, // < #544, change to etheria, declared by _NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
    eITEMWASTE_HPMP_POTION = 18, // HP/MP 회복 물약
    //_NA_0_20110613_GAME_SHOP_RENEWAL
    eITEMWASTE_EXTEND_DATE_WEAPON  = 19, //< 기간 연장 아이템(무기).
    eITEMWASTE_EXTEND_DATE_ARMOR  = 20, //< 기간 연장 아이템(방어구).
    eITEMWASTE_EXTEND_DATE_RIDER  = 21, //< 기간 연장 아이템(탈것).
    eITEMWASTE_EXTEND_DATE_RIDER_PARTS  = 22, //< 기간 연장 아이템(탈것의 파츠).
};

enum eWASTETYPE_FOR_STAT_INIT
{
    eItemWASTE_INIT_STR						= 1,
    eItemWASTE_INIT_DEX						= 2,
    eItemWASTE_INIT_VIT						= 3,
    eItemWASTE_INIT_SPR						= 4,
    eItemWASTE_INIT_INT						= 5,
    eItemWASTE_INIT_Experty1				= 6,
    eItemWASTE_INIT_Experty2				= 7,
    eItemWASTE_INIT_ALL						= 8,
    eItemWASTE_INIT_BASIC_STAT_ALL			= 9,
    eItemWASTE_INIT_EXPERTY_ALL				= 10,
    eItemWASTE_INIT_PASSIVE_ALL				= 11,
};

// 아이템 조합
enum eITEMCOMPOSITECODE
{
    eITEMCOMPOSITE_NONE					= 0,	// 아무것도 없는 상태 << 클라에서 필요해서 추가 했음 - 재영
    eITEMCOMPOSITE_LOW_ENCHANT			= 1,	// 하급인첸트
    eITEMCOMPOSITE_MEDIUM_ENCHANT		= 2,	// 중급인첸트
    eITEMCOMPOSITE_HIGH_ENCHANT			= 3,	// 상급인첸트
    eITEMCOMPOSITE_RANK_DOWN			= 4,	// 랭크다운
    eITEMCOMPOSITE_RANK_UP				= 5,	// 랭크업
    eITEMCOMPOSITE_EXTRACT_SOCKET		= 6,	// 소켓옵션제거
    eITEMCOMPOSITE_ADD_SOCKET			= 7,	// 소켓추가(없던곳에 새로 추가한다는 의미)

    /**/
    // >> 클라이언트에서만 필요    
    // 악세서리 생성
    eITEMCOMPOSITE_LOW_RING				= 8,	// 하급반지 >> 조합
    eITEMCOMPOSITE_HIGH_RING			= 9,	// 상급반지
    eITEMCOMPOSITE_LOW_NECKLACE			= 10,	// 하급목걸이
    eITEMCOMPOSITE_HIGH_NECKLACE		= 11,	// 상급목걸이
    //// 악세서리 재료 생성
    eITEMCOPOSITE_PROTECTION_CRYSTAL	= 12,
    eITEMCOPOSITE_SHANING_CRYSTAL		= 13,
    //// 특수 악세서리 생성
    eITEMCOPOSITE_SOUL_SHEILD_LOW		= 14,
    eITEMCOPOSITE_SOUL_SHEILD_HIGH		= 15,
    eITEMCOPOSITE_SOUL_ARMLET_LOW		= 16,
    eITEMCOPOSITE_SOUL_ARMLET_HIGH		= 17,
    eITEMCOPOSITE_SOUL_WING_LOW			= 18,
    eITEMCOPOSITE_SOUL_WING_HIGH		= 19,
    eITEMCOPOSITE_SOUL_EYE_LOW			= 20,
    eITEMCOPOSITE_SOUL_EYE_HIGH			= 21,	
    eITEMCOPOSITE_EXTRASTONE_CREATE		= 22,	// << 조합
    eITEMCOPOSITE_EXTRASTONE_USE		= 23,
    eITEMCOPOSITE_CRYSTALIZE			= 24,	// 결정화
    eITEMCOPOSITE_ZARD			        = 25,	// 쟈드 합성
    eITEMCOPOSITE_TOTAL					= 31,
    eITEMCOMPOSITE_HEIM_ENCHANT         = 32,   // 하임인챈트
    eITEMCOMPOSITE_CASH_ENCHANT         = 33,   // 캐시인챈트
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	eITEMCOMPOSITE_EQUIPMENT_WAKEUP     = 34,   // 각성
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	eITEMCOMPOSITE_EQUIPMENT_EVOLUTION  = 35,
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
    // << 여기까지 클라이언트에서 필요해서 선언했습니다. - j0	
};

// 1BYTE 옵션[CG_ITEM_ENCHANT_SYN]
enum eENCHANT_OPTION
{
    eENCHANT_NOT_OPT= 0x00,
    eENCHANT_100PER	= 0x01,
    eENCHANT_75PER	= 0x02,
    eENCHANT_50PER	= 0x04,
    eENCHANT_25PER	= 0x08,
    eENCHANT_CASH	= 0x10,
};

#define NEED_CHECK_ATTRIBUTE()    BOOST_STATIC_WARNING(0);
//==================================================================================================
// 주의 : 속성의 enum 위치 변경에 주의!!!!!! +11
//--------------------------------------------------------------------------------------------------
enum eATTR_TYPE
{
    eATTR_TYPE_INVALID = 0,                         // 유효하지 않는 속성
    eATTR_STR = 1,                                  // 스탯(힘)
    eATTR_DEX = 2,                                  // 스탯(민첩)
    eATTR_VIT = 3,                                  // 스탯(체력)
    eATTR_INT = 4,                                  // 스탯(지력)
    eATTR_SPR = 5,                                  // 스탯(정신력)

    eATTR_EXPERTY1 = 6,                             // EP2:unused, 숙련도1
    eATTR_EXPERTY2 = 7,                             // EP2:unused, 숙련도2

    eATTR_CUR_HP = 8,                               // 현재 HP
    eATTR_CUR_MP = 9,                               // 현재 MP

    eATTR_MAX_HP = 10,                              // 최대 HP
    eATTR_MAX_MP = 11,                              // 최대 MP

    eATTR_RECOVERY_HP = 12,                         // HP 회복량
    eATTR_RECOVERY_MP = 13,                         // MP 회복량

//--------------------------------------------------------------------------------------------------
// 공격력 관련 파라미터
//--------------------------------------------------------------------------------------------------
    eATTR_BASE_MELEE_MIN_ATTACK_POWER,              // 기본 물리 공격력
    eATTR_BASE_MELEE_MAX_ATTACK_POWER,
    eATTR_BASE_RANGE_MIN_ATTACK_POWER,
    eATTR_BASE_RANGE_MAX_ATTACK_POWER,
    eATTR_BASE_MAGICAL_MIN_ATTACK_POWER,            // EP2:unused, 기본 마법 (최소) 공격력
    eATTR_BASE_MAGICAL_MAX_ATTACK_POWER,            // EP2:unused, 기본 마법 (최대) 공격력

    eATTR_OPTION_PHYSICAL_ATTACK_POWER,             // [V] 옵션 물리 공격력
    eATTR_OPTION_MAGICAL_ATTACK_POWER,              // EP2:unused
    eATTR_OPTION_ALL_ATTACK_POWER,                  // EP2:unused, 옵션 전체 물리 공격력

    eATTR_MAGICAL_WATER_ATTACK_POWER,               // [V] 원소(물) 공격력
    eATTR_MAGICAL_FIRE_ATTACK_POWER,                // [V] 원소(불) 공격력
    eATTR_MAGICAL_WIND_ATTACK_POWER,                // [V] 원소(바람) 공격력
    eATTR_MAGICAL_EARTH_ATTACK_POWER,               // [V] 원소(대지) 공격력
    eATTR_MAGICAL_DARKNESS_ATTACK_POWER,            // [V] 원소(암흑) 공격력
    eATTR_MAGICAL_DIVINE_ATTACK_POWER,              // unused

    eATTR_MAGICAL_ALL_ATTACK_POWER,                 // [V] 마법속성 전체 공격력, EP2={ FIRE, WATER, WIND, EARTH, DARK }

    eATTR_ADD_SKILL_ATTACK_POWER,                   // [V] 스킬 추가 공격력
    eATTR_ADD_SKILL_DAMAGE_RATIO,                   // [R] 스킬 퍼센트 추가 데미지

//--------------------------------------------------------------------------------------------------
// 방어력 관련 파라미터
//--------------------------------------------------------------------------------------------------
    eATTR_BASE_MELEE_DEFENSE_POWER,                 // [V] 기본 (근거리) 물리 방어력
    eATTR_BASE_RANGE_DEFENSE_POWER,                 // [V] 기본 (원거리) 물리 방어력
    eATTR_BASE_MAGICAL_DEFENSE_POWER,               // EP2:unused, 기본 마법 방어력

    eATTR_OPTION_PHYSICAL_DEFENSE_POWER,            // 옵션 물리 방어력
    eATTR_OPTION_MAGICAL_DEFENSE_POWER,             // EP2:unused
    eATTR_OPTION_ALL_DEFENSE_POWER,                 // EP2:unused, 옵션 전체 물리 방어력 <- 옵션 물리+마법 방어력

    eATTR_MAGICAL_WATER_DEFENSE_POWER,              // EP2:unused, [V] 원소(물) 방어력
    eATTR_MAGICAL_FIRE_DEFENSE_POWER,               // EP2:unused, [V] 원소(불) 방어력
    eATTR_MAGICAL_WIND_DEFENSE_POWER,               // EP2:unused, [V] 원소(바람) 방어력
    eATTR_MAGICAL_EARTH_DEFENSE_POWER,              // EP2:unused, [V] 원소(대지) 방어력
    eATTR_MAGICAL_DARKNESS_DEFENSE_POWER,           // EP2:unused, [V] 원소(암흑) 방어력
    eATTR_MAGICAL_DIVINE_DEFENSE_POWER,             // unused

    eATTR_MAGICAL_ALL_DEFENSE_POWER,                // EP2:unused, [V] 원소 전체 방어력 EP2={ FIRE, WATER, WIND, EARTH, DARK }

    eATTR_ADD_ALL_DEFENSE_POWER,                    // EP2:unused, [V] 공격 타입별 추가 방어력
    eATTR_ADD_MELEE_DEFENSE_POWER,                  // EP2:unused, [V] 공격대상 (근접) 추가 방어력
    eATTR_ADD_RANGE_DEFENSE_POWER,                  // EP2:unused, [V] 공격대상 (원거리) 추가 방어력
    eATTR_ADD_WATER_DEFENSE_POWER,                  // EP2:unused, [V] 공격대상 (물) 추가 방어력
    eATTR_ADD_FIRE_DEFENSE_POWER,                   // EP2:unused, [V] 공격대상 (불) 추가 방어력
    eATTR_ADD_WIND_DEFENSE_POWER,                   // EP2:unused, [V] 공격대상 (바람) 추가 방어력
    eATTR_ADD_EARTH_DEFENSE_POWER,                  // EP2:unused, [V] 공격대상 (대지) 추가 방어력
    eATTR_ADD_DARKNESS_DEFENSE_POWER,               // EP2:unused, [V] 공격대상 (암흑) 추가 방어력
    eATTR_ADD_DIVINE_DEFENSE_POWER,                 // EP2:unused
    eATTR_ADD_PHYSICAL_DEFENSE_POWER,               // EP2:unused
    eATTR_ADD_MAGICAL_DEFENSE_POWER,                // EP2:unused
    eATTR_ADD_MAGICAL_ALL_DEFENSE_POWER,            // EP2:unused

    eATTR_DEL_ALL_TARGET_DEFENSE_RATIO,             // EP2:unused
    eATTR_DEL_MELEE_TARGET_DEFENSE_RATIO,           // EP2:unused, (공격타입/아머타입관련)
    eATTR_DEL_RANGE_TARGET_DEFENSE_RATIO,           // EP2:unused, (공격타입/아머타입관련)
    eATTR_DEL_WATER_TARGET_DEFENSE_RATIO,           // [R] 공격대상 원소(물) 방어력 감소율
    eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO,            // [R] 공격대상 원소(불) 방어력 감소율
    eATTR_DEL_WIND_TARGET_DEFENSE_RATIO,            // [R] 공격대상 원소(바람) 방어력 감소율
    eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO,           // [R] 공격대상 원소(대지) 방어력 감소율
    eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO,        // [R] 공격대상 원소(암흑) 방어력 감소율
    eATTR_DEL_DIVINE_TARGET_DEFENSE_RATIO,          // EP2:unused
    eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO,        // [R] 공격대상 물리 방어력 감소율
    eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO,         // EP2:unused
    eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO,     // [R] 공격대상 전체 원소 방어력 감소율

//--------------------------------------------------------------------------------------------------
// 데미지 관련 파라미터
//--------------------------------------------------------------------------------------------------
    eATTR_ADD_ARMOR_HARD_DAMAGE,                    // EP2:unused, 아머 타입별 추가 데미지
    eATTR_ADD_ARMOR_MEDIUM_DAMAGE,                  // EP2:unused
    eATTR_ADD_ARMOR_SOFT_DAMAGE,                    // EP2:unused
    eATTR_ADD_ARMOR_SIEGE_DAMAGE,                   // EP2:unused
    eATTR_ADD_ARMOR_UNARMOR_DAMAGE,                 // EP2:unused

    eATTR_ADD_RATIO_ARMOR_HARD_DAMAGE,              // EP2:unused, 아머 타입별 추가 데미지(퍼센트)
    eATTR_ADD_RATIO_ARMOR_MEDIUM_DAMAGE,            // EP2:unused
    eATTR_ADD_RATIO_ARMOR_SOFT_DAMAGE,              // EP2:unused
    eATTR_ADD_RATIO_ARMOR_SIEGE_DAMAGE,             // EP2:unused
    eATTR_ADD_RATIO_ARMOR_UNARMOR_DAMAGE,           // EP2:unused
    // CHANGES: changes value type to ratio type since EP2
    eATTR_DEL_ALL_DAMAGE,                           // EP2:unused, 감소 데미지
    eATTR_DEL_MELEE_DAMAGE,                         // EP2:unused, (공격타입/아머타입관련)
    eATTR_DEL_RANGE_DAMAGE,                         // EP2:unused, (공격타입/아머타입관련)
    eATTR_DEL_WATER_DAMAGE,                         // [R] 원소(물)에 대한 데미지 차감률
    eATTR_DEL_FIRE_DAMAGE,                          // [R] 원소(불)에 대한 데미지 차감률
    eATTR_DEL_WIND_DAMAGE,                          // [R] 원소(바람)에 대한 데미지 차감률
    eATTR_DEL_EARTH_DAMAGE,                         // [R] 원소(대지)에 대한 데미지 차감률
    eATTR_DEL_DARKNESS_DAMAGE,                      // [R] 원소(암흑)에 대한 데미지 차감률
    eATTR_DEL_DIVINE_DAMAGE,                        // EP2:unused
    eATTR_DEL_PHYSICAL_DAMAGE,                      // [R] EP2: 물리 공격에 대한 데미지 차감률
    eATTR_DEL_MAGICAL_DAMAGE,                       // EP2:unused
    eATTR_DEL_MAGICAL_ALL_DAMAGE,                   // [R] EP2: 원소(모든)에 대한 데미지 차감률

//--------------------------------------------------------------------------------------------------
// 기타 파라미터
//--------------------------------------------------------------------------------------------------
    eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO,            // 공격 성공률(물리)
    eATTR_PHYSICAL_ATTACK_BLOCK_RATIO,              // [R][ 15] 공격 방어율 (옵션) 추가율
    eATTR_PHYSICAL_ATTACK_BLOCK_BASE_ARMOR_RATIO,   // EP2:added, 공격 방어율 아이템 종합 (총 합산 / 5)

    eATTR_MOVE_SPEED,                               // 이동 속도
    eATTR_ATTACK_SPEED,                             // 공격 속도

    eATTR_ALL_ATTACK_RANGE,                         // 모든 사거리
    eATTR_NORMAL_ATTACK_RANGE,                      // 일반 사거리
    eATTR_SKILL_ATTACK_RANGE,                       // 스킬 사거리

    eATTR_SIGHT_RANGE,                              // 시야

    eATTR_CRITICAL_RATIO_CHANGE,                    // 크리티컬 확률 증감
    eATTR_ADD_MAGICAL_CRITICAL_RATIO,               // EP2:unused, 마법 크리티컬 확률
    eATTR_ADD_ALL_CRITICAL_RATIO,                   // EP2:unused, 전체 크리티컬 확률
    eATTR_CRITICAL_DAMAGE_CHANGE,                   // 크리티컬 데미지 추가

    eATTR_ADD_ATTACK_INC_RATIO,                     // [R] 추가 공격 상승율
    eATTR_ADD_DEFENSE_INC_RATIO,                    // [R] 추가 방어 상승율

    eATTR_INCREASE_SKILL_LEVEL,                     // 스킬 레벨 1상승
    eATTR_INCREASE_STAT_POINT,                      // 스텟 포인트(모두)1상승

    //  2006 오후 1:23:11
    eATTR_DECREASE_LIMIT_STAT,                      // 스텟 제한 수치 감소
    eATTR_MP_SPEND_INCREASE,                        // MP 소비 감소

    //  상용화 아이템 옵션 추가
    eATTR_ABSORB_HP,                                // HP흡수
    eATTR_ABSORB_MP,                                // MP,SP 흡수
    eATTR_REFLECT_DAMAGE_RATIO,                     // 데미지 반사
    eATTR_BONUS_MONEY_RATIO,                        // 하임 증가
    eATTR_BONUS_EXP_RATIO,                          // 경험치 증가
    eATTR_AREA_ATTACK_RATIO,                        // 다중공격 확률
    eATTR_BONUS_CASTING_TIME,                       // 65:캐스팅 타임 증감
    eATTR_BONUS_SKILL_COOL_TIME,                    // 75:[R] 스킬 쿨타임 증감
    eATTR_DECREASE_DAMAGE,                          // 데미지 감소

    eATTR_RESURRECTION_RATIO,                       // 52:사망시 스스로 부활 할 확률
    eATTR_DOUBLE_DAMAGE_RATIO,                      // 53:데미지의 두배가 될 확률
    eATTR_LUCKMON_INC_DAMAGE,                       // 54:럭키 몬스터 공격시 추가 데미지
    eATTR_COPOSITE_INC_RATIO,                       // 55:조합 성공률 증가
    eATTR_BYPASS_DEFENCE_RATIO,                     // 56:방어력 무시 확율
    eATTR_INCREASE_MIN_DAMAGE,                      // 57:최소 데미지 증가
    eATTR_INCREASE_MAX_DAMAGE,                      // 58:최대 데미지 증가
    eATTR_DECREASE_ITEMDURA_RATIO,                  // 59:아이템 내구력 감소무시 확율
    eATTR_RESISTANCE_BADSTATUS_RATIO,               // 60:스킬 효과 무지확율
    eATTR_INCREASE_SKILLDURATION,                   // 61:스킬 효과 시간 증가 ( 버프 계열 )
    eATTR_DECREASE_SKILL_SKILLDURATION,             // 62:스킬 효과 시간 증감 ( 디버프 계열 ) < f110531.6L, changed from '감소'
    eATTR_OPTION_ETHER_DAMAGE_RATIO,                // 63:에테르웨폰 데미지 변화
    eATTR_OPTION_ETHER_PvE_DAMAGE_RATIO,            // 76:에테르웨폰 데미지 변화 (PvE용), f110601.4L

    eATTR_INCREASE_RESERVE_HP,                      // 64:적립 HP 증가

    eATTR_RESIST_HOLDING,                           // 66:홀딩 내성
    eATTR_RESIST_SLEEP,                             // 67:슬립 내성
    eATTR_RESIST_POISON,                            // 68:중독 내성
    eATTR_RESIST_KNOCKBACK,                         // 69:넉백 내성
    eATTR_RESIST_DOWN,                              // 70:다운 내성
    eATTR_RESIST_STUN,                              // 71:스턴 내성
    eATTR_DECREASE_PVPDAMAGE,                       // 72:PVP데미지 감소
    eATTR_ADD_DAMAGE,                               // 73:추가데미지
    eATTR_AUTO_ITEM_PICK_UP,                        // 74:Item 자동 줍기
    // NOTE: regenerated index
    eATTR_MAX_SD,                                   // 93:최대 SD
    eATTR_RECOVERY_SD,                              // 94:SD 회복량
    eATTR_CUR_SD,                                   // 현재 SD

    // _NA_003966_20111227_ADD_ENCHANT
    eATTR_INCREASE_ENCHANT_RATIO, // 77:인첸트 성공률 증가

    eATTR_PREMIUMSERVICE_PCBANG,                    // PC방 효과

    //_NA_006731_20130521_ENCHANT_ADD_OPTION
    eATTR_ENEMY_CRITICAL_RATIO_CHANGE,                  // 96:피격 시 상대의 크리티컬 확률 증감

    //_NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    eATTR_ATTACK_DAMAGE_ABSORB_SD_RATIO,             // 98:가해 데미지 SD 전환량
    eATTR_ATTACK_DAMAGE_ABSORB_HP_RATIO,             // 99:가해 데미지 HP 전환량

    //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    eATTR_ENEMY_CRITICAL_DAMAGE_CHANGE,           // 100:피격 시 상대의 크리티컬 데미지 증감
    
    //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    eATTR_CRAFT_COST_RATIO,                         // 101:제작 비용 증감
    eATTR_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO,  // 102:제작 실패시 재료 소멸 방지 확률 증감
    eATTR_ENCHANT_COST_RATIO,                       // 103:인챈트 비용 증감
    eATTR_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO, // 104:인챈트 실패시 아이템 소멸, 다운 방지 확률 증감
    eATTR_RECOVER_POTION_COOLTIME_RATIO,            // 105:회복 포션 쿨타임 증감
    eATTR_RECOVER_POTION_RECOVERY_RATIO,            // 106:회복 포션 회복량 증감
    eATTR_QUEST_REWARD_EXP_RATIO,                   // 107:퀘스트 보상 경험치 증감
    eATTR_MAX_DAMAGE_RATIO,                         // 108:최대 데미지 발생확률 증감
    eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO,        // 109:공성 오브젝트 데미지 증감
    eATTR_SHOP_REPAIR_HEIM_RATIO,                   // 110:상점 수리 하임 증감
    eATTR_SHOP_BUY_HEIM_RATIO,                      // 111:상점 구매 하임 증감
    
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eATTR_MAX_FP,                                   // 112: 위치블레이드 최대FP
    eATTR_RECOVERY_FP,                               // 113: 위치블레이드 FP회복량 (공격시)
    eATTR_INCREASE_DAMAGE_RATIO,                     // 114: 데미지 증가 
    
    //_NA_008124_20150313_AWAKENING_SYSTEM
    eATTR_AWAKENING_PROBABILITY,                     // 115: 각성 확률 증감

    //_NA_008486_20150914_TOTAL_BALANCE
    eATTR_DEBUFF_DURATION,                          // 116: 자신의 디버프기술 효과 지속시간 강화(밀리세컨드)
    
    //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    eATTR_DECREASE_DAMAGE_NPC,                      // 117: npc가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_BERSERKER,                // 118: 버서커가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_DRAGONKNIGHT,             // 119: 드래곤나이트가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_VALKYRIE,                 // 120: 발키리가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_ELEMENTALIST,             // 121: 엘리멘탈리스트가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_SHADOW,                   // 122: 섀도우가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_MYSTIC,                   // 123: 미스틱이 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_HELLROID,                 // 124: 헬로이드가 주는 데미지 감소
    eATTR_DECREASE_DAMAGE_WITCHBLADE,               // 125: 위치블레이드가 주는 데미지 감소
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    // WARNING: must declare inner of eATTR_MAX
    // if the new attribute need a character attribute field.
    eATTR_MAX, // character attribute fields
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

#ifdef _NA_0_20110216_STATUS_STAT_POINT_SIMULATION
    // eATTR_STR = 1,
    // eATTR_DEX = 2,
    // eATTR_VIT = 3,
    // eATTR_INT = 4,
    // eATTR_SPR = 5,
    eATTR_BASE_STAT_START_INDEX = eATTR_STR,
    eATTR_BASE_STAT_COUNT = eATTR_SPR + 1,
#endif
}; //end of enum eATTR_TYPE



enum eATTR_EXTRA_OPTION
{
    eATTR_EXTRA_OPTION_INVALID = 0,
    //
    eATTR_EXTRA_TOTEM_NPC_OPTION_EMPOWERMENT, // _NA001956_110210_PERK_SWITCHABLE_REGION

    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    // NOTE: f110216.4L, an extra switchable status option don't need a character attribute field
    eATTR_EXTRA_MAX,
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
};

//==================================================================================================

//todo:아이디 fix 안되었음 
//캐릭터 마다 스타일이 다르다.
//스타일과무기로 부터 에니메이션을 자동화 할수 있는 방법이....

enum eSTYLE
{
    
    eSTYLE_BERSERKER_PUNCH			= 60000, // 맨손공격 스타일 
    eSTYLE_DRAGON_PUNCH				= 60199, // 맨손공격 스타일 
    eSTYLE_SHADOW_PUNCH             = 60400,
    eSTYLE_VALKYRIE_PUNCH			= 60600, // 맨손공격 스타일 
    eSTYLE_MAGICIAN_PUNCH			= 60800, // 맨손공격 스타일 
   
    eSTYLE_TWOHANDAXE_NORMAL        = 60100,
    eSTYLE_FUSS_BREAKER		        = 60101,//eSTYLE_TWOHANDAXE_DOWN  
    eSTYLE_HACKSTER_KNOCKBACK_AXE   = 60111,//eSTYLE_TWOHANDAXE_DAMAGE
    eSTYLE_ROLLANDS_DOWN	        = 60121,//eSTYLE_TWOHANDAXE_STUN  

    eSTYLE_TWOHANDSWORD_NORMAL       = 60001,
    eSTYLE_BALTURS_DEFENCE           = 60011,//eSTYLE_TWOHANDSWORD_DEFENCE
    eSTYLE_HACKSTER_KNOCKBACK_TWOHAND= 60021,//eSTYLE_TWOHANDSWORD_DOWN   
    eSTYLE_RUDBIGS_STUN				 = 60031,//eSTYLE_TWOHANDSWORD_DAMAGE 

    eSTYLE_ONEHANDSWORD_NORMAL       = 60200,
    eSTYLE_RESTROS_SPEED             = 60201,//eSTYLE_ONEHANDSWORD_SPEED   
    eSTYLE_TORES_CRITICAL            = 60211,//eSTYLE_ONEHANDSWORD_CRITICAL
    eSTYLE_ELEMEOUS_KNOCKBACK_ONEHAND= 60221,//eSTYLE_ONEHANDSWORD_DAMAGE  

    eSTYLE_SPEAR_NORMAL              = 60300,
    eSTYLE_ALRESTER_DAMAGE           = 60301,//eSTYLE_SPEAR_CRITICAL
    eSTYLE_ELEMEOUS_KNOCKBACK_LANCE  = 60311,//eSTYLE_SPEAR_DOWN    
    eSTYLE_FLABIOUS_PIERCING         = 60321,//eSTYLE_SPEAR_PIERCING

    eSTYLE_ONEHANDCROSSBOW_NORMAL    = 60601,
    eSTYLE_ETHER_NORMAL              = 60702,

    eSTYLE_STAFF_NORMAL              = 60801,
    eSTYLE_ORB_NORMAL				 = 60901,

    eSTYLE_DRAGON_TRANSFORM1         = 61001,
    eSTYLE_DRAGON_TRANSFORM2         = 61002,	

    eSTYLE_SHADOW_DAGGER_NORMAL      = 60401,
    eSTYLE_SHADOW_WHIP_NORMAL		 = 60402,

    eSTYLE_MYSTIC_PUNCH = 60403, // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eSTYLE_MYSTIC_GUNTLET_NORMAL = 60404, // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eSTYLE_MYSTIC_POLEARM_NORMAL = 60405, // _NA_004965_20120613_NEW_CHARACTER_MYSTIC

    eSTYLE_HELLROID_PUNCH = 60406, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eSTYLE_HELLROID_WEAPON1 = 60407, //_NA_000000_20130812_NEW_CHARACTER_HELLROID

    eSTYLE_WITCHBLADE_PUNCH = 60408, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eSTYLE_WITCHBLADE_ARCBLADE = 60409, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
};

enum eSKILL
{
    eSKILL_INVALID					= 0,

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 버서커 
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // 직업 1
    eSKILL_TAUNT = 51, 
    eSKILL_STUN_KICK                = 52,
    eSKILL_THORNS_SKIN			    = 53,
    eSKILL_SLASH                    = 54,
    eSKILL_GROUND_SHOCK	    		= 55,
    eSKILL_DAMAGE_RECOVERY          = 56,
    eSKILL_DOUBLE_SLASH             = 57,
    eSKILL_WEAPON_BREAK			    = 58,
    eSKILL_SPECTUAL_GUARD           = 59,
    eSKILL_REVENGE                  = 60,
    eSKILL_JUMPING_CRASH            = 61,
    eSKILL_VITAL_AMPLYFY            = 62,
    eSKILL_WILD_SPIRITS             = 63, 
    eSKILL_TACKLE                   = 66,
    kSkillHardStance = 67, // 하드 스탠스 (지속 체력)
    kSkillIronStance = 68, // 아이언 스탠스 (순간 체력)

    // 직업 2
    eSKILL_BASH			            = 101,
    kSkillSPRecovery = 102,	// SP 리커버리
    eSKILL_BERSERK_MODE 			= 103,
    eSKILL_MONSTER_TACKLE           = 104,
    eSKILL_SPIN_RUSH	    		= 105,
    eSKILL_SPIRIT_RECHARGE			= 106,
    eSKILL_DEADLY_STRIKE			= 107,
    eSKILL_SONIC_EXPLOSION			= 108,
    eSKILL_SPINNING_BLOW		   	= 109,
    eSKILL_SPINING_CRUSH            = 110,
    eSKILL_UPPER_BLOW     			= 111,
    eSKILL_DOUBLE_ATTACK			= 112,
    eSKILL_FURY_STRIKE              = 113,
    eSKILL_WEAPON_THROWING          = 114,
    eSKILL_BERSERKER_FORCE          = 115,
    eSKILL_WARCRY                   = 116, 
    eSKILL_SHOUT                    = 117, 
    eSKILL_DASH                     = 118, 
    kSkillSPRecharge = 119, // SP 리차지
    kSkillBloodHaze = 120, // 블러드 헤이즈 (지속 HP)
    kSkillMotalBlood = 121, // 모탈 블러드 (순간 HP)

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 드래곤 나이트
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // 직업 1
    eSKILL_TRIPLE_PIERCE            = 251, 
    eSKILL_SWORDDANCING             = 252, 
    eSKILL_SONICBLADE               = 253, 
    eSKILL_ILLUSION_DANCE			= 254,
    eSKILL_MACHSLASH                = 255,
    eSKILL_DIFFUSEBLOW              = 256,
    eSKILL_VIPER_BLADE              = 257,
    eSKILL_DEVINE_FORCE             = 258,
    eSKILL_MARK_OF_CHANCE           = 259,
    eSKILL_RAPID_ATTACK             = 260,
    eSKILL_DEATH_DIVIDE             = 261,
    eSKILL_SPIRITUAL_EYE            = 262,
    eSKILL_ARCANE_BLADE             = 263,
    eSKILL_SLAUGHTER                = 264,
    eSKILL_BLADE_FURY               = 265,
    eSKILL_CHARGE_ATTACK			= 266,
    eSKILL_SPRIT_SLUG				= 267,
    kSkillCourageOfKnights = 268, // 커리지 오브 나이츠 (지속 민첩)
    kSkillFeverOfKnights = 269, // 피버 오브 나이츠 (순간 민첩)

    // 직업 2
    eSKILL_ASSAULT                  = 301,
    eSKILL_STUNNINGBLOW             = 302,
    eSKILL_FEAR                     = 303,
    eSKILL_MARK_OF_RECOVERY         = 304,
    eSKILL_DRAGON_EYE               = 305,
    eSKILL_DRAGON_ARM_TRANSFORM     = 306,
    eSKILL_STORM_WAVE               = 307,
    eSKILL_LIGHTING_LANCE           = 308,
    eSKILL_DIVINE_AMBLEM            = 309,
    eSKILL_ARMOR_BREAK              = 310,
    eSKILL_DRAGON_CLER              = 311,
    eSKILL_MARK_OF_SPIRIT_POWER     = 312,
    eSKILL_EXPLOZEN_ATTACK          = 313,
    eSKILL_EBON_BLADE               = 314,
    eSKILL_FIST_OF_FIRE             = 315,
    eSKILL_DRAGON_RAY				= 321,
    eSKILL_DRAGON_GALE				= 322,
    eSKILL_DRAGON_BEAD				= 323, 
    kSkillDragonicForce = 324, // 드래고닉 포스 (지속 공격력)
    kSkillDragonicBless = 325, // 드래고닉 블레스 (순간 공격력)

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 발키리 
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // 직업 1
    eSKILL_FAST_SHOT                = 451, 
    eSKILL_SNIPING                  = 452, 
    eSKILL_MOON_ASSAULT             = 453, 
    eSKILL_SHOCKWAVE                = 454, 
    eSKILL_FROZEN_ARROW             = 455, 
    eSKILL_DOUBLE_SPINKICK          = 456, 
    eSKILL_BLOOD_RAIN				= 457,
    eSKILL_ETHER_BLASE              = 458,
    eSKILL_DEFENCE_MOTION           = 459,
    //eSKILL_AIR_BLOW                 = 460,
    eSKILL_AIRBLOW = 460, 
    eSKILL_HEAVY_SHOT               = 461,
    eSKILL_SPINING_WAVE             = 462,
    eSKILL_PIERCEARROW              = 463,
    eSKILL_OVER_LIMIT               = 464,
    eSKILL_MANA_DRAIN               = 467,
    eSKILL_WILD_SHOT                = 468, 
    eSKILL_CHASE_SHOT				= 470,
    kSkillIncreaseMind = 471, // 인크리스 마인드 (지속 지력)
    kSKillMindSpark = 472, // 마인드 스파크 (순간 지력)

    // 직업 2
    eSKILL_UNLIMIT_FORCE            = 501,
    eSKILL_MARK_OF_FACTION          = 502,
    eSKILL_MARK_OF_SYMPHONY         = 503,
    eSKILL_SOUL_OF_LIFE             = 504,
    eSKILL_PATIENCE                 = 505,
    eSKILL_SOUL_HEAL			    = 506,
    eSKILL_RESSURECTION             = 507,
    eSKILL_WAR_BATTLER              = 508,
    eSKILL_BATTLE_BARRIOR           = 509,
    //eSKILL_BATTLER_HORN             = 510,
    eSKILL_BATTLERHONE = 510, 
    eSKILL_DESTROYER                = 511,
    eSKILL_FILL_OFF	                = 512,
    eSKILL_SACRIFICE	            = 513,
    eSKILL_MAGIC_WALKER             = 514,
    eSKILL_MAGIC_WALKER_FIRE_ARROW  = 515,
    eSKILL_MAGIC_WALKER_CURE        = 516,
    eSKILL_DOUBLE_GORE              = 517,
    eSKILL_DOUBLE_GORE_LIGHTING     = 518,
    eSKILL_DOUBLE_GORE_DEXTERITY    = 519,
    eSKILL_RANGE_HEAL               = 520,
    eSKILL_SUMMON_MASTERY			= 521,
    eSKILL_ADRENALINE				= 522,
    kSkillSummonicShield = 523, // 서머닉 실드 (지속 방어력)
    kSkillSummonicBarrier = 524, // 서머닉 베리어 (순간 방어력)

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 엘리멘탈리스트 
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // 패시브
    eSKILL_FIRE_MASTERY             = 601,
    eSKILL_WATER_MASTERY            = 602,
    eSKILL_WIND_MASTERY             = 603,
    eSKILL_EARTH_MASTERY            = 604,
    eSKILL_MANA_REGENERATION        = 605,

    // 직업 1
    eSKILL_FIRE_ARROW               = 651,
    eSKILL_ICE_DAGGER               = 652,
    eSKILL_HEALING_HAND				= 653,
    eSKILL_TELEPORT                 = 654,
    eSKILL_MAGIC_SHIELD             = 655,
    eSKILL_REVIVAL                  = 656,
    eSKILL_FROZON_SHIELD            = 657,
    eSKILL_FIRE_BALL                = 658,
    eSKILL_BLAZE_FORCE              = 659,
    eSKILL_ICE_BLAST                = 660,
    eSKILL_ICE_FORCE                = 661,
    eSKILL_ICE_BOLT                 = 662,
    eSKILL_MANA_RECOVERY            = 663,
    eSKILL_FIRE_SPEAR               = 664,
    eSKILL_PRESSING_WALL            = 665,
    eSKILL_FIRE_FILA                = 666,
    eSKILL_REVERSE                  = 667,
    eSKILL_ESSENSE     				= 668,
    eSKILL_NATURAL_FORCE            = 669,
    kSkillEnchantPower = 670, // 인챈트 파워 (지속 힘)
    kSkillCatalystPower = 671, // 카탈리스트 파워 (순간 힘)

    // 직업 2
    eSKILL_WIND_BOLT                = 701,
    eSKILL_ELECTRIC_FIELD           = 702,
    eSKILL_POISON_THORN             = 703,
    eSKILL_WIND_SHIELD              = 704,
    eSKILL_NATURAL_ATTACK           = 705,
    eSKILL_LIGHTING                 = 706,
    eSKILL_INCRESE_SPEED            = 707,
    eSKILL_FOCUS       				= 708,
    eSKILL_LIGHTING_WAVE            = 709,
    eSKILL_SONY_VINE                = 710,
    eSKILL_MAGIC_FORCE				= 711,
    eSKILL_POISON_RAIN              = 712,
    eSKILL_ACCRUCY_SPIRIT           = 713,
    eSKILL_MIGHTY_SPIRIT            = 714,
    eSKILL_POWER_WAVE               = 715,
    eSKILL_MIGHT_POWER				= 716,
    eSKILL_STAR_FALL                = 717,
    eSKILL_EARTHQUAKE               = 718,
    eSKILL_BATTLE_HEAL              = 719,
    eSKILL_HEAL_MAGIC_ARRAY         = 720,
    eSKILL_GROUP_HEAL               = 721,
    eSKILL_MANA_REACHARGE           = 722,
    eSKILL_ICE_DAGGER2              = 723,
    eSKILL_TELEPORT2                = 724,
    eSKILL_SACRED_FIRE              = 725,
    eSKILL_INQUIRY                  = 726,
    eSKILL_DEVISH_DANCE             = 727,
    eSKILL_CIRCLE_SPRITS			= 728,
    eSKILL_INABILITTY				= 729,
    eSKILL_SAINT_AID				= 730,
    eSKILL_PURGE					= 731,
    kSkillWhisperOfWind = 732, // 위스퍼 오브 윈드 (지속 이동속도)
    kSkillWhisperOfSylph = 733, // 위스퍼 오브 실프 (순간 이동속도)


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 섀도우
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // 직업 1
    eSKILL_PAIN						= 800, // 페인 (고통)
    eSKILL_DARK_FIRE				= 801, 
    eSKILL_DARK_SPARK				= 802, 
    eSKILL_ARMOR_INCREASE			= 803, // 아머인크리즈 (물리/마법 방어력 증가, 암흑저항력 상승)
    eSKILL_SOUL_CONTROL				= 804, // 소울컨트롤 (영혼지배)
    eSKILL_IMPRTENT					= 805, // 임포턴트 (무력함의 저주)
    eSKILL_VITAL_SUCTION			= 806, // 바이탈석션 (정기흡수)
    eSKILL_ENCHANT_POISON			= 807, // 인첸트포인즌 (독을 바른 검)
    eSKILL_HP_SUCTION				= 808, // HP석션 (HP흡수)
    eSKILL_MP_SUCTION				= 809, // MP석션 (MP흡수)		// 미사용
    eSKILL_CURSE_INCREASE			= 810, // 커즈인크리즈 (저주강화)
    eSKILL_DARK_PAIN				= 811, // 다크페인 (암흑의고통)
    eSKILL_BUFF_CANCEL				= 812, // 버프캔슬 (강화마법해제)
    eSKILL_CONFUSE					= 813, // 컨퓨즈 (혼란의저주)
    eSKILL_DEMON					= 814, // 데몬 (악령)
    eSKILL_DARK_FORCE				= 815, // 다크포스 (어둠의힘)
    kSkillHelronsSoul = 816, // 헬론즈 소울 (지속 정신력)
    kSkillDemonsSoul = 817, // 데몬즈 소울 (순간 정신력)

    // 직업 2
    eSKILL_WHIP_ATTACK				= 850, // 휩어택 (후려치기)
    eSKILL_POISON_DAGGER			= 851, // 포이즌대거 (독이묻은대거)
    eSKILL_DOUBLE_WIDTH				= 852, // 더블위드 (두번횡으로베기)
    eSKILL_DARK_STUN				= 853, // 다크스턴 (최면)
    eSKILL_HIDE						= 854, // 하이드 (은신)
    eSKILL_DEATH_BLOW				= 855, // 다스블로우 (치명타)
    eSKILL_SHADOW_SPEAR				= 856, // 섀도우스피어 (찌르기)
    eSKILL_CANCELLATION				= 857, // 캔슬레이션 (디버프제거)
    eSKILL_RUSH						= 858, // 러쉬 (전력질주)
    eSKILL_SUDDEN_STRIKE			= 859, // 서든스트라이크 (기습)
    eSKILL_DARK_DUST				= 860, // 다크더스트 (암흑의가루, 블라인드)
    eSKILL_FAST_BLOW				= 861, // 패스트블로우 (올려치기)
    eSKILL_DOWN_SLASH				= 862, // 다운슬래쉬 (다운공격함)
    eSKILL_SOUL_SCREAM				= 863, // 소울스크림 (영혼의절규)
    eSKILL_WHIRLWIND_BLOW			= 864, // 휠윈드블로우
    eSKILL_DARK_SLASH				= 865, // 어둠의난도질
    eSKILL_DARK_BREAK				= 866, // 다크브렝크 (자폭)
    kSkillDarkTrace = 867, // 다크 트레이스 (지속 공격속도)
    kSkillDarkRage = 868, // 다크 레이즈 (순간 공격속도)

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 미스틱
    ////////////////////////////////////////////////////////////////////////////////////////////////

    eSKILL_MYSTIC_901            = 901,
    eSKILL_MYSTIC_902            = 902,
    eSKILL_MYSTIC_903            = 903,
    eSKILL_MYSTIC_904            = 904,
    eSKILL_MYSTIC_905            = 905,
    eSKILL_MYSTIC_906            = 906,
    eSKILL_MYSTIC_907            = 907,
    eSKILL_MYSTIC_908            = 908,
    eSKILL_MYSTIC_909            = 909,
    eSKILL_MYSTIC_910            = 910,
    eSKILL_MYSTIC_911            = 911,
    eSKILL_MYSTIC_912            = 912,
    eSKILL_MYSTIC_913            = 913,
    eSKILL_MYSTIC_914            = 914,
    eSKILL_MYSTIC_915            = 915,
    eSKILL_MYSTIC_916            = 916,
    eSKILL_MYSTIC_917            = 917,
    eSKILL_MYSTIC_918            = 918,

    eSKILL_MYSTIC_919            = 919,
    eSKILL_MYSTIC_920            = 920,
    eSKILL_MYSTIC_921            = 921,
    eSKILL_MYSTIC_922            = 922,
    eSKILL_MYSTIC_923            = 923,
    eSKILL_MYSTIC_924            = 924,
    eSKILL_MYSTIC_925            = 925,
    eSKILL_MYSTIC_926            = 926,
    eSKILL_MYSTIC_927            = 927,
    eSKILL_MYSTIC_928            = 928,
    eSKILL_MYSTIC_929            = 929,
    eSKILL_MYSTIC_930            = 930,
    eSKILL_MYSTIC_931            = 931,
    eSKILL_MYSTIC_932            = 932,
    eSKILL_MYSTIC_933            = 933,
    eSKILL_MYSTIC_934            = 934,
    eSKILL_MYSTIC_935            = 935,
    eSKILL_MYSTIC_936            = 936,

	// 연계된 스킬
	eSKILL_MYSTIC_DARKOFLIGHT_ZONE  = 937,
	eSKILL_MYSTIC_GRAVITY_ZONE      = 938,	


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 헬로이드
    ////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eSKILL_HELLROID_1000            = 1000,     // 블래스터펀치
    eSKILL_HELLROID_1001            = 1001,     // 크래쉬암
    eSKILL_HELLROID_1002            = 1002,     // 리페어
    eSKILL_HELLROID_1003            = 1003,     // 드롭킥
    eSKILL_HELLROID_1004            = 1004,     // 슬로우오라
    eSKILL_HELLROID_1005            = 1005,     // 드릴어택
    eSKILL_HELLROID_1006            = 1006,     // 위크너스오라
    eSKILL_HELLROID_1007            = 1007,     // 스핀블로우
    eSKILL_HELLROID_1008            = 1008,     // 플라잉차징
    //eSKILL_HELLROID_1009            = 1009,     // 미사용
    eSKILL_HELLROID_1010            = 1010,     // 하트브레이크
    eSKILL_HELLROID_1011            = 1011,     // 미스챈스오라
    eSKILL_HELLROID_1012            = 1012,     // 포지션체인지
    eSKILL_HELLROID_1013            = 1013,     // 디클라인오라
    eSKILL_HELLROID_1014            = 1014,     // 인베이더모드
    eSKILL_HELLROID_1015            = 1015,     // 헬로이드스핀

    eSKILL_HELLROID_1016            = 1016,     // 헤드벗
    eSKILL_HELLROID_1017            = 1017,     // 에테르차징
    eSKILL_HELLROID_1018            = 1018,     // 로우블로우
    eSKILL_HELLROID_1019            = 1019,     // 스트레이트펀치
    eSKILL_HELLROID_1020            = 1020,     // 리커버리오라
    eSKILL_HELLROID_1021            = 1021,     // 리바이브
    eSKILL_HELLROID_1022            = 1022,     // 부스트오라
    eSKILL_HELLROID_1023            = 1023,     // 블래스터붐
    eSKILL_HELLROID_1024            = 1024,     // 블래스터파이어
    eSKILL_HELLROID_1025            = 1025,     // 헬로이드빔
    eSKILL_HELLROID_1026            = 1026,     // 슬라이딩어택
    eSKILL_HELLROID_1027            = 1027,     // 이그노어오라
    eSKILL_HELLROID_1028            = 1028,     // 에테르베리어
    eSKILL_HELLROID_1029            = 1029,     // 컨센트레이션오라
    eSKILL_HELLROID_1030            = 1030,     // 가디언실드
    eSKILL_HELLROID_1031            = 1031,     // 브레이크어스

    // 연계된 스킬
    eSKILL_HELLROID_AURA_1032       = 1032,
    eSKILL_HELLROID_AURA_1033       = 1033,
    eSKILL_HELLROID_AURA_1034       = 1034,
    eSKILL_HELLROID_AURA_1035       = 1035,
    eSKILL_HELLROID_AURA_1036       = 1036,
    eSKILL_HELLROID_AURA_1037       = 1037,
    eSKILL_HELLROID_AURA_1038       = 1038,
    eSKILL_HELLROID_AURA_1039       = 1039,


#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 위치블레이드
    ////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eSKILL_WITCHBLADE_1100           = 1100,      // 호라이즌잭
    eSKILL_WITCHBLADE_1101           = 1101,      // 터닝킥
    eSKILL_WITCHBLADE_1102           = 1102,      // 피드온레이지
    eSKILL_WITCHBLADE_1103           = 1103,      // 스핀턴쓰러스트
    eSKILL_WITCHBLADE_1104           = 1104,      // 트위스트플랩킥
    eSKILL_WITCHBLADE_1105           = 1105,      // 바이올렛크로스
    eSKILL_WITCHBLADE_1106           = 1106,      // 네일스크래치
    eSKILL_WITCHBLADE_1107           = 1107,      // 페어리블레스
    eSKILL_WITCHBLADE_1108           = 1108,      // 데드폴
    eSKILL_WITCHBLADE_1109           = 1109,      // 드래곤댄스
    eSKILL_WITCHBLADE_1110           = 1110,      // 스피닝하츠
    eSKILL_WITCHBLADE_1111           = 1111,      // 바이스소드
    eSKILL_WITCHBLADE_1112           = 1112,      // 이베이드
    eSKILL_WITCHBLADE_1113           = 1113,      // 다크폴링그레이브
    eSKILL_WITCHBLADE_1114           = 1114,      // 퓨리포메이션
    eSKILL_RISINGFORCE               = 1115,      // 라이징포스
    eSKILL_WITCHBLADE_1116           = 1116,      // 엔젤블레스

    eSKILL_WITCHBLADE_1117           = 1117,      // 서먼_스컬제이크
    eSKILL_WITCHBLADE_1118           = 1118,      // 크로스콤비네이션
    eSKILL_WITCHBLADE_1119           = 1119,      // 다이브헬팡
    eSKILL_WITCHBLADE_1120           = 1120,      // 블레어팡
    eSKILL_WITCHBLADE_1121           = 1121,      // 뱀파이어릭터치
    eSKILL_WITCHBLADE_1122           = 1122,      // 서머너즈리커버리
    eSKILL_WITCHBLADE_1123           = 1123,      // 블레이드커튼
    eSKILL_WITCHBLADE_1124           = 1124,      // 서먼_티타니아
    eSKILL_WITCHBLADE_1125           = 1125,      // 프로스트블레스
    eSKILL_WITCHBLADE_1126           = 1126,      // 멘딩멘탈
    eSKILL_WITCHBLADE_1127           = 1127,      // 서먼_벤쉬
    eSKILL_WITCHBLADE_1128           = 1128,      // 프로즌윈드
    eSKILL_WITCHBLADE_1129           = 1129,      // 디바인어설트
    eSKILL_WITCHBLADE_1130           = 1130,      // 서먼_씨엘리스
    eSKILL_WITCHBLADE_1131           = 1131,      // 멘딩마인드
    eSKILL_WITCHBLADE_1132           = 1132,      // 마그네틱그라피토
    eSKILL_WITCHBLADE_1133           = 1133,      // 루튼포인트
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    // 패시브
    eSKILL_DAGGER_MASTERY			= 900,
    eSKILL_WHIP_SWORD_MASTERY		= 901,
    eSKILL_SHADOW_FORCE				= 902,
    eSKILL_CURSE_TRAINING			= 903,
    eSKILL_DARK_TRAINING			= 904,
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 플레이어 스킬 끝
    ////////////////////////////////////////////////////////////////////////////////////////////////

    
    eSKILL_CREATE_CRYSTAL_WARP		= 1400,
    eSKILL_DESTROY_CRYSTAL_WARP     = 1401,
    
    // 몬스터
    eSKILL_BEAST_MASTER_DEADLY_STRIKE			= 1500,
    eSKILL_BEAST_MASTER_NAKED_BUD				= 1501,
    eSKILL_BEAST_MASTER_DIRTY_SIDE				= 1502,
    eSKILL_BEAST_MASTER_SOUL_OF_BEAST			= 1503,
    eSKILL_BEAST_MASTER_BLIND_TIME				= 1504,
    eSKILL_BEAST_MASTER_SPIRITS_TOUCH			= 1505,
    eSKILL_BEAST_MASTER_SUMMON_VAIN				= 1506,
    eSKILL_BEAST_MASTER_REGEN_ENERMY			= 1507,
    eSKILL_DRAGON_ZOMBIE_UPPER_BLOW				= 1508,
    eSKILL_DRAGON_ZOMBIE_StUNNING_BLOW			= 1509,
    eSKILL_DRAGON_ZOMBIE_HOUL					= 1510,
    eSKILL_DRAGON_ZOMBIE_RECOVERY_HOUL			= 1511,
    eSKILL_DRAGON_ZOMBIE_SOUL_OF_DRAGON			= 1512,
    eSKILL_DRAGON_ZOMBIE_MOON_ASSERT			= 1513,
    eSKILL_GATESENTRY_CHIEF_GROUND_SHOCK		= 1514,
    eSKILL_GATESENTRY_CHIEF_TOTUR_SHOCK			= 1515,
    eSKILL_GATESENTRY_REGEN_ENERMY				= 1516,
    eSKILL_RESTRICTED_DRAGON_SONIC_EXPLORZEN	= 1517,
    eSKILL_RESTRICTED_DRAGON_SPINNING_BLOW		= 1518,
    eSKILL_RESTRICTED_DRAGON_REVERSE_SOLDIER    = 1519,
    eSKILL_EVIL_SORCERER_FIRE_BALL				= 1520,
    eSKILL_EVIL_SORCERER_PRESSING_WALL			= 1521,
    eSKILL_EVIL_SORCERER_ANIMATE_DEAD			= 1522,
    eSKILL_EVIL_SORCERER_SPIRIT_TOUCH			= 1523,
    eSKILL_REDAMOR_WARRIOR_SONIC_BLADE			= 1524,
    eSKILL_REDAMOR_WARRIOR_SPIN_CRUSH			= 1525,
    eSKILL_REDAMOR_WARRIOR_TOURNADO_AIDS		= 1526,
    eSKILL_REDAMOR_WARRIOR_REGEN_ENERMY			= 1527,
    eSKILL_DARK_F_INFANTRY_BASH					= 1528,
    eSKILL_DARK_F_OFFENSE_UP					= 1529,
    eSKILL_DARK_F_REVERSE_SOLDIER				= 1530,
    eSKILL_HEAVY_HUNTER_SNIPING					= 1531,
    eSKILL_HEAVY_NOIZE_SHOT						= 1532,
    eSKILL_HEAVY_WHISTLE_SHOT					= 1533,
    eSKILL_BEAST_KNIGHT_DEATH_DIVIDE			= 1534,
    eSKILL_BEAST_KNIGHT_CURSE_ENERMY			= 1535,
    eSKILL_BEAST_KNIGHT_SELF_HEAL				= 1536,
    eSKILL_GHOUL_STUNNING_BLOW					= 1537,
    eSKILL_GHOUL_GHOUL_DANCE					= 1538,
    eSKILL_GHOUL_ANIMATE_DEAD					= 1539,
    eSKILL_SKULL_INFANTRY_ASSERT				= 1540,
    eSKILL_SKULL_INFANTRY_CUTTER				= 1541,
    eSKILL_SKULL_INFANTRY_SUMMON_SOLDIER		= 1542,
    eSKILL_SNOW_DRAGON_ZOMBIE_UPPER_BLOW		= 1543,
    eSKILL_SNOW_DRAGON_ZOMBIE_STOP_ORDER		= 1544,
    eSKILL_SNOW_DRAGON_ZOMBIE_REVERSE_SOLDIER	= 1545,
    eSKILL_VOLCANO_BEAST_UPPER_BLOW				= 1546,
    eSKILL_VOLCANO_BEAST_STOP_ORDER				= 1547,
    eSKILL_VOLCANO_BEAST_SPIRIT_TOUCH			= 1548,
    eSKILL_BEAST_GUARD_BASH						= 1549,
    eSKILL_BEAST_GUARD_SLEEP_DANCE				= 1550,
    eSKILL_BEAST_GUARD_SELF_HEAL				= 1551,
    eSKILL_GATESENTRY_INFANTRY_GROUND_SHOCK     = 1552,
    eSKILL_GATESENTRY_INFANTRY_ARMMOR_DOWN      = 1553,
    eSKILL_GATESENTRY_INFANTRY_SUMMON_SOLDIER   = 1554,
    eSKILL_DARK_F_PATROL_WEAPON_BREAK           = 1555,
    eSKILL_DARK_F_PATROL_TWIN_STRECH            = 1556,
    eSKILL_DARK_F_PATROL_SELF_HEAL              = 1557,
    eSKILL_LAVA_DRAGON_SONIC_EXPLOZEN           = 1558,
    eSKILL_LAVA_DRAGON_SPINNING_BLOW            = 1559,
    eSKILL_LAVA_DRAGON_SUMMON_SOLDIER           = 1560,
    eSKILL_SLAVE_ARCHER_HEAVY_SHOT				= 1561,
    eSKILL_SLAVE_ARCHER_SOLOW_SHOT				= 1562,
    eSKILL_SLAVE_ARCHER_RECOVERY_SHOT			= 1563,
    eSKILL_ROAM_PRISONER_STURNING_BLOW          = 1564,
    eSKILL_ROAM_PRISONER_CHARGE_BLOW            = 1565,
    eSKILL_ROAM_PRISONER_SPIRITS_TOUCH          = 1566,
    eSKILL_DARK_KNIGHT_DARK_SHOCK               = 1567,
    eSKILL_DARK_KNIGHT_DARK_SWORD               = 1568,
    eSKILL_DARK_KNIGHT_Regen_ENERMY             = 1569,
    eSKILL_DARK_KNIGHT_SPRIRITS_TOUCH           = 1570,
    eSKILL_DARK_F_MAGICIAN_LIGHTNING            = 1571,
    eSKILL_DARK_F_MAGICIAN_SHADOW_ROOTS         = 1572,
    eSKILL_DARK_F_MAGICIAN_SUMMON_SOLIDER       = 1573,
    eSKILL_FIRE_DESTROYER_DOUBLE_HEAD           = 1574,
    eSKILL_FIRE_DESTROYER_FLAME_BREATH          = 1575,
    eSKILL_FIRE_DESTROYER_SELF_HEAL             = 1576,
    eSKILL_FIRE_DESTROYER_SUMMON_VEIN           = 1577,
    eSKILL_BEAST_KEEPER_DRAGON_HOUL             = 1578,
    eSKILL_BEAST_KEEPER_NAKED_BID               = 1579,
    eSKILL_BEAST_KEEPER_SUMMON_VEIN             = 1580,
    eSKILL_BEAST_KEEPER_SPIRITS_TOUCH           = 1581,
    eSKILL_CERBERUS_CUTTER                      = 1582,
    eSKILL_CERBERUS_NOISE_HOUL                  = 1583,
    eSKILL_CERBERUS_SUMMON_VEIN                 = 1584,
    eSKILL_CERBERUS_REGEN_ENERMY                = 1585,
    eSKILL_DARKNESS_SORCERESS_DARK_FORCE        = 1586,
    eSKILL_DARKNESS_SORCERESS_CURSE_HEALTH      = 1587,
    eSKILL_DARKNESS_SORCERESS_SUMMON_SOLIDER    = 1588,
    eSKILL_DARKNESS_OBSERVER_HEABY_BLOW         = 1589,
    eSKILL_DARKNESS_OBSERVER_WHEEL_ATTACK       = 1590,
    eSKILL_DARKNESS_OBSERVER_REVERSE_SOLIDER    = 1591,
    eSKILL_DARKNESS_OBSERVER_SPRITS_TOUCH       = 1592,
    eSKILL_SNOWFIELD_DOOR_KEEPER_ROCK_AWAY      = 1593,
    eSKILL_SNOWFIELD_DOOR_KEEPER_HEAVY_BLOW     = 1594,
    eSKILL_SNOWFIELD_DOOR_KEEPER_SUMMMON_SOLIDER= 1595,
    eSKILL_WHITE_MAGICIAN_ICE_BALL              = 1596,
    eSKILL_WHITE_MAGICIAN_ICE_FORM              = 1597,
    eSKILL_WHITE_MAGICIAN_RERVERSE_SOLIDER      = 1598,
    eSKILL_ICE_CASTLE_KEEPER_TACKLE             = 1599,
    eSKILL_ICE_CASTLE_KEEPER_ICE_THORNS         = 1600,
    eSKILL_ICE_CASTLE_KEEPER_SELF_HEAL          = 1601,
    eSKILL_CURSED_PRIEST_LIGHTING               = 1602,
    eSKILL_CURSED_PRIEST_ICE_SPRAY              = 1603,
    eSKILL_CURSED_PRIEST_SELF_HEAL              = 1604,
    eSKILL_CURSED_PRIEST_SPIRITS_TOUCH          = 1605,
    eSKILL_MASTER_OF_CASTLE_ICE_MISSILE         = 1606,
    eSKILL_MASTER_OF_CASTLE_ICE_CAGE            = 1607,
    eSKILL_MASTER_OF_CASTLE_BLIZZARD            = 1608,
    eSKILL_MASTER_OF_CASTLE_SUMMON_VEIN         = 1609,
    eSKILL_MASTER_OF_CASTLE_REGEN_ENERMY        = 1610,
    eSKILL_MASTER_OF_CASTLE_SPIRITS_TOUCH       = 1611,
    eSKILL_FIRE_CERBERUS_FIRE_BREATH            = 1612,
    eSKILL_FIRE_CERBERUS_GRATE_HOUL             = 1613,
    eSKILL_FIRE_CERBERUS_SUMMON_SOLDIER         = 1614,
    //eSKILL_EVIL_SORCERER_ANIMATE_DEAD           = 1615,
    //eSKILL_FIRE_DRAKE_FIRE_BREATH               = 1616,
    //eSKILL_FIRE_DRAKE_SUMMON_SOLDIER            = 1617,
    eSKILL_FLAME_BEAST_BLOODY_BITE              = 1618,
    eSKILL_FLAME_BEAST_TACKLE                   = 1619,
    eSKILL_FIRE_DRAKE_SLOWER_WING               = 1620,
    eSKILL_FIRE_DRAKE_FIRE_BREATH               = 1621,
    eSKILL_FLAME_SHAMAN_FIRE_ENERGY             = 1622,
    eSKILL_FLAME_SHAMAN_FIRE_WALL               = 1623,
    eSKILL_FLAME_WARRIOR_AXE_FLAME              = 1624,
    eSKILL_FLAME_WARRIOR_KNOCK_OUT              = 1625,
    eSKILL_FLAME_RULER_EXPLORZEN_KNOCKLE        = 1626,
    eSKILL_FLAME_RULER_FIRE_WALL                = 1627,
    eSKILL_FLAME_RULER_GIGANTIC_BOMBER          = 1628,

    eSKILL_CURSED_PRIEST_ICE_STORM              = 1639,

    eSKILL_MASTER_OF_CASTLE_FROST_RING          = 1670,
    eSKILL_BEAST_MASTER_FIRE_OF_DARKNESS		= 1719,
    eSKILL_FLAME_RULER_METEO_SHOWER				= 1722,
    eSKILL_MASTER_OF_CASTLE_BLIZZARD2           = 1737,

    // 07.3월 15일 아이올트 모스터 스킬추가1 - 보환
    eSKILL_FELLEN_WARRIOR_DASH_ATTACK			= 1784,
    eSKILL_FELLEN_WARRIOR_XDANCE				= 1785,
    eSkILL_CURSED_ARCHER_POISON_ARROW			= 1787,
    
    // 07.4월 5일 아이올트 몬스터 스킬 추가2 - 보환
    eSKILL_CURSED_SOCERER_VAMPIRIC_KISS			= 1790,
    eSKILL_DARK_SWORDMAN_TRIPLE_ATTACK			= 1791,
    eSKILL_DARK_SWORDMAN_DEADLY_SMITE			= 1792,
    eSKILL_DARK_WARRIOR_BEAT					= 1793,
    eSKILL_DARK_WARRIOR_FATAL_BLOW				= 1794,
    eSKILL_ARCHBISHOP_OF_DARKNESS_FRUSTRATION_ZONE = 1795,
    eSKILL_ARCHBISHOP_OF_DARKNESS_LACK			= 1796,
    eSKILL_ARCHBISHOP_OF_DARKNESS_DOOM			= 1797,
    eSKILL_ARCHBISHOP_OF_DARKNESS_SPRIT_TOUCH	= 1798,
    eSKILL_ARCHBISHOP_OF_DARKNESS_SUMMON_ALLY	= 1799,
    eSKILL_ARCHBISHOP_OF_DARKNESS_SHIELD_OF_KARMA = 1800,

    // 07.4월 30일 속박의기사, 굶주리학살자 스킬 추가
    eSKILL_KNIGHT_OF_SLAVERY_BINDING			= 1803,
    
    //07.6월 20일 거대한 까마귀,버려진자 스킬 추가
    eSKILL_GIIANTIC_CROW_SCRATCH				= 1833,
    eSKILL_GIANNTIC_CROW_DIVE					= 1834,
    eSKILL_DUMPED_MAN_BLIDING					= 1835,
    eSKILL_DUMPED_MAN_DREIN_RANGER				= 1836,

    eSKILL_KNIGHT_OF_HUNGRY_SLAYER_THUNDER_CLAP = 1904,
    eSKILL_BINDED_ARCHER_STUN_SHOT				= 1905,

    //맵사물 오브젝트 
    eSKILL_MAP_MONSTER_1						= 3000,
    eSKILL_MAP_MONSTER_2						= 3001,
    eSKILL_MAP_MONSTER_3						= 3002,
    eSKILL_MAP_MONSTER_4						= 3003,

    eSKILL_ITEM_MINE							= 5004,
};												  
   

enum eACTION									  
{												  
    eACTION_AUTO_ATTACK			   = 0,			  
    eACTION_PICK_ITEM			   = 1,			  
    eACTION_FOLLOW_PLAYER		   = 2,			  
    eACTION_SIT_AND_STAND		   = 3,			  
    eACTION_TRADE				   = 4,			  
    eACTION_VENDOR				   = 5,			  
    eACTION_GUILD_INVITE		   = 6,			  
    eACTION_PARTY_LEAVE			   = 7,
    eACTION_NORMAL_ATTACK		   = 8,
    eACTION_FRIEND_ADD			   = 9,
    eACTION_PARTY_MEMBER_SUMMON    = 11,
    eACTION_PARTY_MEMBER_WARP      = 12,
    eACTION_ASSIST				   = 13,
    eACTION_ASSIST_AND_ATTACK	   = 14,
    eACTION_TRADE_IN_CHAT_DLG	   = 15, //스킬창 미등록
    eACTION_FRIEND_ADD_IN_CHAT_DLG = 16, //스킬창 미등록

    eACTION_CRYSTAL_WARP_CREATE	   = 17,
    eACTION_CRYSTAL_WARP_DESTROY   = 18,
    eACTION_CRYSTAL_WARP_EXCUTE	   = 19,
    eACTION_ETHERIA_EXTRACT 	   = 20,
    eACTION_IDENTIFY_SOCKET        = 21, //소켓확인
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    eACTION_RANKING_SYSTEM_LOAD    = 22,
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    eACTION_PERK_GROUP_CHANGE      = 23,
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    eACTION_ITEM_CRYSTALIZATION    = 24, //아이템결정화
    eACTION_START_SKILL_CODE	   = 35000,
};


enum eABILITY
{
    eABILITY_INVALID								= 0,

    //-------------------------------------------------------------------------------------------------
    // 수치 관련
    //-------------------------------------------------------------------------------------------------

    eABILITY_DAMAGE									= 1,	// 스킬 데미지
    eABILITY_DAMAGE_PER_TIME						= 2,	// 시간당 데미지

    eABILITY_MAX_HP_INCREASE						= 3,	// 최대 HP 증가
    eABILITY_CUR_HP_INCREASE						= 4,	// 현재 HP 증가
    eABILITY_RECOVER_HP_INCREASE					= 5,	// HP 회복량 증가
    eABILITY_EXHAUST_HP								= 6,	// HP 소진

    eABILITY_MAX_MP_INCREASE						= 7,	// 최대 MP 증가
    eABILITY_CUR_MP_INCREASE						= 8,	// 현재 MP 증가
    eABILITY_RECOVER_MP_INCREASE					= 9,	// MP 회복량 증가
    eABILITY_EXHAUST_MP								= 10,	// MP 소진

    eABILITY_PHYSICAL_ATTACKPOWER_INCREASE			= 11,	// 물리 공격력 증가
    eABILITY_ATTACKPOWER_BY_ARMORTYPE				= 12,	// EP2:unused, 추가 공격력(아머타입별)
    eABILITY_MAGIC_ATTACKPOWER_INCREASE				= 13,	// 마법 공격력 증가

    eABILITY_PHYSICAL_DEFENSE_INCREASE				= 14,	// 물리 방어력 증가
    eABILITY_DEFENSE_BY_ATTACKTYPE					= 15,	// EP2:unused, 추가 방어력(공격타입별)
    eABILITY_MAGIC_DEFENSE_INCREASE					= 16,	// EP2:unused, 마법 방어력 증가

    eABILITY_STAT_INCREASE							= 17,	// 스탯 증가

    eABILITY_PHYSICAL_ATTACKRATE_INCREASE			= 18,	// 물리 공격 성공율 증가
    eABILITY_PHYSICAL_AVOIDRATE_INCREASE			= 19,	// 물리 공격 회피율 증가

    eABILITY_MOVE_SPEED_INCREASE					= 20,	// 이동 속도 증가
    eABILITY_PHYSICAL_SPEED_INCREASE				= 21,	// 물리 공격 속도 증가

    eABILITY_CASTING_TIME_INCREASE					= 22,	// 캐스팅 타임 증가

    eABILITY_DAMAGE_DECREASE						= 23,	// 데미지 감소

    eABILITY_SKILLRANGE_INCREASE					= 24,	// 스킬 사거리 증가

    eABILITY_CRITICAL_RATIO_CHANGE				    = 25,	// (자신 혹은 피격자 공격의) 크리티컬 확률 증감
    eABILITY_CRITICAL_DAMAGE_CHANGE				    = 26,	// (자신 혹은 피격자 공격의) 크리티컬 데미지 증감

    eABILITY_AGGROPOINT_INCREASE					= 27,	// 어그로 포인트 증가
    
    eABILITY_SKILLDAMAGE_INCREASE					= 28,	// 스킬 데미지 증가(CHANGUP_INTENSIFY_SKILLDAMAGE_STATUS)

    //-------------------------------------------------------------------------------------------------
    // 상태 관련
    //-------------------------------------------------------------------------------------------------

    eABILITY_STUN_STATUS							= 100,	// Stun 상태 발생
    eABILITY_ABNORMAL_STATUS						= 101,	// 이상 상태 발생
    eABILITY_WIND_SHIELD							= 102,	// 윈드쉴드
    eABILITY_BUFF_RANGE_DAMAGE						= 103,	// 버프 범위 데미지(일정영역, 캐릭터 주변의 시간당 데미지)

    eABILITY_LOWHP_ATTACKPOWER_CHANGE				= 104,	// EP2:unused, HP 저하시 공격력 증감
    eABILITY_LOWHP_DEFENSE_CHANGE					= 105,	// EP2:unused, HP 저하시 방어력 증감
    eABILITY_BONUS_DAMAGE_PER_ATTACK				= 106,	// 공격당 추가 데미지

    eABILITY_ATTACK_DAMAGE_HP_ABSORPTION			= 107,	// 공격 데미지 HP 흡수
    eABILITY_ATTACK_DAMAGE_MP_ABSORPTION			= 108,	// 공격 데미지 MP 흡수
    eABILITY_ATTACKED_DAMAGE_HP_ABSORPTION			= 109,	// 피격 데미지 HP 흡수
    eABILITY_ATTACKED_DAMAGE_MP_ABSORPTION			= 110,	// 피격 데미지 MP 흡수

    eABILITY_MAGIC_SHIELD							= 111,	// 마법 쉴드

    eABILITY_FEAR									= 112,	// 피어

    eABILITY_REFLECT_DAMAGE							= 113,	// 피격 데미지(데미지 반사)
    eABILITY_REFLECT_STUN							= 114,	// 피격 스턴(데미지 + 모든행동불능)
    eABILITY_REFLECT_FEAR							= 115,	// 피격 피어(데미지 + 도망)
    eABILITY_REFLECT_FROZEN							= 116,	// 피격 프로즌(데미지 + 모든행동불능)

    eABILITY_REFLECT_SLOW							= 117,	// 피격 족쇄(데미지 + 이속저하)
    eABILITY_REFLECT_SLOWDOWN						= 118,	// 피격 둔화(데미지 + 공속저하)

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //-------------------------------------------------------------------------------------------------
    // 길드 시설 어빌리티
    //-------------------------------------------------------------------------------------------------
    eABILITY_CRAFT_COST_RATIO                       = 150,  // 제작 비용 증감
    eABILITY_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO= 151,  // 제작 실패시 재료 소멸 방지 확률 증감
    eABILITY_ENCHANT_COST_RATIO                     = 152,  // 인챈트 비용 증감
    eABILITY_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO = 153,  // 인챈트 실패시 아이템 소멸, 다운 방지 확률 증감
    eABILITY_RECOVER_POTION_COOLTIME_RATIO          = 154,  // 회복 포션 쿨타임 증감
    eABILITY_RECOVER_POTION_RECOVERY_RATIO          = 155,  // 회복 포션 회복량 증감
    eABILITY_QUEST_REWARD_EXP_RATIO                 = 156,  // 퀘스트 보상 경험치 증감
    eABILITY_MAX_DAMAGE_RATIO                       = 157,  // 최대 데미지 발생 확률 증감
    eABILITY_MONEY_RAIO                             = 158,  // 하임 획득량 증감
    eABILITY_DOMINATION_MAPOBJECT_DAMAGE_RATIO      = 159,  // 공성 오브젝트 데미지 증감
    eABILITY_SHOP_REPAIR_HEIM_RATIO                 = 160,  // 상점 수리 하임 증감
    eABILITY_SHOP_BUY_HEIM_RATIO                    = 161,  // 상점 구매 하임 증감
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    //-------------------------------------------------------------------------------------------------
    eABILITY_MONSTER_KILL_EXP_RATIO                 = 162,  // 몬스터 처치 경험치 증감
    //-------------------------------------------------------------------------------------------------
    // 기타
    //-------------------------------------------------------------------------------------------------

    eABILITY_STATUS_HEAL							= 203,	// 상태 치료
    eABILITY_RESURRECTION							= 204,	// 부활
    eABILITY_WEAPON_MASTERY							= 205,	// 웨픈 마스터리
    eABILITY_KNOCKBACK								= 206,	// 녹백
    eABILITY_FIGHTING_ENERGY_NUM_INCREASE			= 207,	// 투기 개수 증가
    eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY		= 208,	// 투기당 데미지 추가
    eABILITY_BONUS_DAMAGE_PER_SP					= 209,	// SP당 추가 공격력
    eABILITY_BONUS_DAMAGE_PER_STATUS				= 210,	// 상태별 추가 데미지 발생
    eABILITY_TELEPORT								= 212,	// 텔레포트

    eABILITY_DRAGON_TRANSFORMATION1					= 213,	// 드래곤변신1
    eABILITY_DRAGON_TRANSFORMATION2					= 214,	// 드래곤변신2(보너스 능력치)
    eABILITY_DRAGON_TRANSFORMATION3					= 216,	// 드래곤변신3(보너스 능력치)

    eABILITY_TARGET_CHANGE							= 215,	// 어그로끌기

    eABILITY_RANDOM_AREA_ATTACK						= 217,	// 랜덤지역공격
    eABILITY_SUMMON_TO_DIE							= 218,	// 소환해서 죽이기

    eABILITY_MONSTER_TRANSFORMATION					= 219,	// 몬스터 변신

    eABILITY_SUMMON									= 220,	// 소환
    eABILITY_SUMMON_CHANGE_STATUS					= 221,  // 소환시 소환체의 능력치를 플레이어의 능력치로 변경.

    eABILITY_PIERCE_ARROW							= 223,	// 피어스 애로우

    eABILITY_SKILL_COOL_TIME_INCREASE				= 225,  // 스킬 쿨타임 증감
    eABILITY_MP_SPEND_INCREASE						= 226,  // 스킬 사용시 MP량 증감

    eABILITY_KNOCKBACK2								= 227,	// KNOCKBACK 2nd - __NA001048_080514_APPEND_ABILITY_KNOCKBACK2__
    eABILITY_ADRENALINE								= 228,	// 아드레날린
    eABILITY_SUMMONMASTERY							= 229,  // 서먼마스터리
    eABILITY_SELF_DESTRUCTION						= 230,	// 자폭
    // __NA_S00015_20080828_NEW_HOPE_SHADOW
    eABILITY_BLOCK_SKILL							= 301,	// 스킬을 막다.(컨퓨즈)
    eABILITY_ATTR_DEFENSIVE_POWER					= 302,	// EP2:원소 방어율 증감 <- EP1:속성방어력 증감
    eABILITY_INCREASE_SKILL_ABILITY					= 303,	// 스킬능력증가.( 커스인크리즈 )
    eABILITY_SUMMON_DEAD_MONSTER					= 304,	// 죽은 몬스터소환
    eABILITY_VITAL_SUCTION							= 305,	// 정기흡수
    eABILITY_ENCHANT_POISON							= 306,	// 인첸트포이즌
    eABILITY_SUCTION_HPMP							= 307,	// HP, MP흡수
    eABILITY_DARK_STUN								= 308,	// 다크스턴
    eABILITY_HIDE									= 309,	// 하이드.
    eABILITY_DARK_BREAK								= 310,	// 다크브레이크.
    eABILITY_ATTR_ATTACK_POWER						= 311,	// 속성 공격력 증감.
    eABILITY_CANCEL_STATUS							= 312,	// 상태해제.
    eABILITY_CHANGE_ATTR							= 313,	// 속성값 변경.
    // end of shadow
    eABILITY_RECOVERY_CHANCE						= 314,	// 적립된 HP 비율 증가.
    //{_NA_001231_20081216_ADD_OPTION_KIND
    eABILITY_RESIST_HOLDING							= 315,	// 홀딩 내성.
    eABILITY_RESIST_SLEEP   						= 316,	// 슬립 내성.
    eABILITY_RESIST_POISON   						= 317,	// 중독 내성.
    eABILITY_RESIST_KNOCKBACK						= 318,	// 넉백 내성.
    eABILITY_RESIST_DOWN   							= 319,	// 다운 내성.
    eABILITY_RESIST_STUN   							= 320,	// 스턴 내성.
    eABILITY_DECREASE_PVPDAMAGE						= 321,	// PVP대미지 감소.
    //}
    //eABILITY_RESIST_ATTACK						= 322,	// unused, 공격 내성. __NA_001244_20090417_ATTACK_RESIST
    //__NA_001290_20090525_SHIELD_SYSTEM
    eABILITY_MAX_SD_INCREASE						= 323,	// 최대 SD 증가
    eABILITY_RECOVER_SD_INCREASE					= 324,	// SD 회복량 증가
    //_NA001385_20090924_DOMINATION_SKILL
    eABILITY_SUMMON_CRYSTALWARP 					= 325,	// 크리스탈 워프(길드소환물)소환
    eABILITY_SUMMON_CRYSTALWARP_DESTROY				= 326,	// 크리스탈 워프(길드소환물)소환 해제
    eABILITY_TOTEM_NPC_OPTION_EMPOWERMENT           = 327, // TotemNpc 옵션 부여

    kAbilityIncreseHeal = 328, // 치유량 증가
    kAbilityActiveComboSkill = 329, // 연계 스킬 활성화
    kAbilityComboSkillEffectAddDamage = 330, // 연계 스킬 발동 효과 - 데미지 추가
    kAbilityComboSkillEffectAddBadStatus = 331, // 연계 스킬 발동 효과 - 상태이상 확률/지속시간 증가
    kAbilityActiveIncreseSkillDamage = 332, // 스킬 데미지 증가 상태 발동
    kAbilityIncreseSkillDamage = 333, // 스킬 데미지 증가

    eABILITY_DETECTING_HIDE                         = 334, // 은신체 감지
    eABILITY_PULLING                                = 335, // 당기기(넉백의 반대)
    //eABILITY_BLIND                                = 336, // 실명상태  => eABILITY_ABNORMAL_STATUS
    //eABILITY_GRAVITY                              = 337, // 중력 증가 => eABILITY_ABNORMAL_STATUS
    //eABILITY_POLYMORPH                            = 338, // 동물로 변형시키는 마법 => eABILITY_ABNORMAL_STATUS
    eABILITY_ROLLING_BOMB                           = 339, // 롤링밤
    eABILITY_QUICKSTEP                              = 340, // 빠른 이동
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    eABILITY_FUGITIVE                               = 341, // 전장 도망자 버프
#endif
    eABILITY_CUR_SD_INCREASE                        = 343, // 현재SD증감
    eABILITY_SKILL_STATE_IGNORE                     = 344, // 스킬상태무시
#ifdef _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
    eABILITY_VARIATION_PUREVALUE_HP                 = 345, // 스킬공격력,방어력,저항력,매직쉴드,SD등에 영향받지않는 체력 증감
#endif //_NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
    //_NA_007667_20141001_WITCHBLADE_SKILL
    eABILITY_AURORA                                 = 347, // 오로라 스킬
    eABILITY_FP_CHANGE                              = 348, // FP 추가/소모 (1회성)
    eABILITY_FP_TOGGLE                              = 349, // FP 추가/소모 (주기성)

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eABILITY_CHARMED          						= 351,	// 매혹 (시전자를 따라오게 한다)
    eABILITY_REFLECT_LINK_SKILL                     = 352,  // 피격시 스킬사용(데미지 + 스킬)
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eABILITY_INCREASE_DAMAGE                        = 353,  // 데미지 증가
    eABILITY_AUTOCAST_BYATTACK                      = 354,  // 출혈공격 버프
    eABILITY_SKILL_CONDITION                        = 355,  // 스킬 조건검사용 어빌리티
    eABILITY_OVERLAP_STATE                          = 356,  // 중첩 어빌리티
    eABILITY_SUMMON_IMMOVABLE                       = 357,  // 고정형 소환수(위치블레이드)
    eABILITY_SUMMON_CHANGE_STATUS_EXTEND            = 358,  // 소환자의 능력치 일부, 소환수가 받아옴, 221어빌리티의 확장어빌리티

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    eABILITY_AWAKENING_PROBABILITY                  = 359,  // 각성 확률 증감
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    eABILITY_MAX
};

// 치유량 증가 형식
struct IncreseHealAbilityType
{
    enum Value
    {
        kSkill = 1, // 스킬 사용시에만 증가
        kItem = 2, // 아이템 사용시에만 증가
        kSkillAndItem = 3 // 스킬과 아이템 둘다 사용시 증가
    };
private:
    IncreseHealAbilityType();
};

// VALUE_TYPE_VALUE => 속성의 m_ItemValue, m_SkillValue 공간에 할당.
// VALUE_TYPE_PERCENT_PER_CUR, VALUE_TYPE_PERCENT_PER_MAX
// => 속성의 m_ItemRatio, m_SkillRatio 공간에 할당.
// ( 어빌리티의 사용방식에 따라 2,3열거형 사용 
// ex) HP에 적용 비율, MaxHP에 적용 비율 )

// VALUE_TYPE_RATIO_VALUE => 속성의 m_ItemValue, m_SkillValue 공간에 할당.
// ( 확률 계산에만 사용되는 값일경우 m_ItemValue, m_SkillValue 공간에 들어갈 값이 없으므로, 
//   updata시 m_CalcValue값이 항상 0이다 )
enum eVALUE_TYPE
{
    VALUE_TYPE_VALUE				= 1,	// 고정 수치
    VALUE_TYPE_PERCENT_PER_CUR		= 2,	// 현재치 대비 비율
    VALUE_TYPE_PERCENT_PER_MAX		= 3,	// 최대치 대비 비율
    VALUE_TYPE_RATIO_VALUE			= 5,	// 수치가 비율값인 경우(이동속도율)
};

enum eFIELDEFFECT_TYPE
{
    EFFECT_TYPE_PERIODIC_DAMAGE		= 0,
    EFFECT_TYPE_BOMB				= 1,
    EFFECT_TYPE_SELF_DESTRUCTION	= 2,	//__NA00XXXX_080922_TASK_SSQ_NPC_SELF_DESTRUCTION__
    EFFECT_TYPE_PERIODIC_SKILL      = 3,
};


enum eCHAR_EVENT
{
    eCHAR_EVENT_INVALID = 0,

    eCHAR_EVENT_DEAD,					// 죽었다.
    eCHAR_EVENT_RESURRECTION,			// 살아났다.
};

enum eSKILL_USER_TYPE
{
    eSKILL_USER_PLAYER		= 0,		// 플레이어
    eSKILL_USER_MONSTER,				// 몬스터
    eSKILL_USER_NPC,					// 일반 NPC
    eSKILL_USER_SUMMONED,				// 소환체
    eSKILL_USER_FOLLOWER,				// 용병
    eSKILL_USER_ACTION,					// 액션
    eSKILL_USER_EMOTICON,				// 이모티콘
    eSKILL_USER_MAX
};

enum eCHAR_TYPE
{
    eCHAR_NONE						= 0,
    eCHAR_BERSERKER					= 1,
    eCHAR_DRAGON					= 2,
    eCHAR_SHADOW					= 3,
    eCHAR_VALKYRIE					= 4,
    eCHAR_MAGICIAN					= 5,
    eCHAR_ELEMENTALIST				= 5,
    eCHAR_MYSTIC                    = 6, // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eCHAR_HELLROID                  = 7, // _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eCHAR_WITCHBLADE                = 8, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eCHAR_TYPE_MAX,
};

enum eTRANSFORM_TYPE
{
    eTRANSFORM_TYPE_NONE				=  0,
    eTRANSFORM_TYPE_DRAGON1				=  100,
    eTRANSFORM_TYPE_DRAGON2				=  101,
};

// 다음은 캐릭터 타입이긴 하지만 별도로 처리
enum eCHAREX_TYPE
{
    eCHAREX_100LEVEL_CHANGEUP_CONSTANT = 10,
    //
    eCHAREX_DRAGON_1stTRANS = eTRANSFORM_TYPE_DRAGON1,      // 드래곤나이트 변신 1차
    eCHAREX_DRAGON_2ndTRANS = eTRANSFORM_TYPE_DRAGON2,      // 드래곤나이트 변신 2차

    eCHAREX_BERSERKER_100LEVEL     = eCHAR_BERSERKER + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // 버서커 100레벨 체인지업 코드
    eCHAREX_DRAGON_KNIGHT_100LEVEL = eCHAR_DRAGON + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // 드래곤나이트 100레벨 체인지업 코드
    eCHAREX_SHADOW_100LEVEL        = eCHAR_SHADOW + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // 쉐도우 100레벨 체인지업 코드
    eCHAREX_VALKYRIE_100LEVEL      = eCHAR_VALKYRIE + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // 발키리 100레벨 체인지업 코드
    eCHAREX_ELEMENTALIST_100LEVEL  = eCHAR_ELEMENTALIST + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // 엘리멘탈리스트 100레벨 체인지업 코드
    eCHAREX_MYSTIC_100LEVEL        = eCHAR_MYSTIC + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // 미스틱 100레벨 체인지업 코드 //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eCHAREX_HELLROID_100LEVEL      = eCHAR_HELLROID + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // 헬로이드 100레벨 체인지업 코드 //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eCHAREX_WITCHBLADE_100LEVEL    = eCHAR_WITCHBLADE + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // 위치블레이드 100레벨 체인지업 코드 //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    eCHAREX_DRAGON_1stTRANS_100LEVEL = eCHAREX_DRAGON_1stTRANS + eCHAREX_100LEVEL_CHANGEUP_CONSTANT,
    eCHAREX_DRAGON_2ndTRANS_100LEVEL = eCHAREX_DRAGON_2ndTRANS + eCHAREX_100LEVEL_CHANGEUP_CONSTANT,
};

#ifdef _NA_0_20120104_DRAGON_COMMON_SKILL
enum eDRAGON_COMMON_SKILL_TYPE
{
    eDRAGON_COMMON_SKILL_1stTRANS = 102,
    eDRAGON_COMMON_SKILL_2ndTRANS = 103,
};
#endif

// 성별 형식
struct GenderType
{
    enum Value
    {
        kAll = 0, // 모두
        kMale = 1, // 남성
        kFemale = 2 // 여성
    };
};

enum eCLASS_TYPE
{
    eCLASS_NONE						= 0,
    eCLASS_BERSERKER				= (1<<0),
    eCLASS_DRAGON					= (1<<1),
    eCLASS_SHADOW					= (1<<2),
    eCLASS_VALKYRIE					= (1<<3),
    eCLASS_ELEMENTALIST				= (1<<4),
    eCLASS_MYSTIC                   = (1<<5), // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eCLASS_HELLROID                 = (1<<6), //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eCLASS_WITCHBLADE               = (1<<7), //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eCLASS_ALL						= (1<<8)-1,
};

enum eNPC_CLASS_TYPE
{
    eNPC_CLASS_HUMAN,
    eNPC_CLASS_WIZARD,
    eNPC_CLASS_DEVIL,
    eNPC_CLASS_DRAGON,
    eNPC_CLASS_UNDEAD,
    eNPC_CLASS_PLANT,
    eNPC_CLASS_INSECT,
    eNPC_CLASS_BEAST,
    eNPC_CLASS_CENTIPEDE,   // 지네몬스터
    eNPC_CLASS_LIFE_SOUL			= 50,
    eNPC_CLASS_WAR_BATTLER,
    eNPC_CLASS_DESTROYER,
    eNPC_CLASS_MAGIC_WALKER,
    eNPC_CLASS_DOUBLE_GORE,
};

// 스킬 범위 설정
template <int kClass, int kJob, int kInBegin, int kInEnd>
struct SkillBoundary
{
    enum Value
    {
        kBegin = kInBegin, 
        kEnd = kInEnd
    };
};
// 51~68
typedef SkillBoundary<eCLASS_BERSERKER, 1, eSKILL_TAUNT, kSkillIronStance> BerserkerJob1Boundary;
// 101~121
typedef SkillBoundary<eCLASS_BERSERKER, 2, eSKILL_BASH, kSkillMotalBlood> BerserkerJob2Boundary;
// 251~269
typedef SkillBoundary<eCLASS_DRAGON, 1, eSKILL_TRIPLE_PIERCE, kSkillFeverOfKnights> DragonJob1Boundary;
// 301~325
typedef SkillBoundary<eCLASS_DRAGON, 2, eSKILL_ASSAULT, kSkillDragonicBless> DragonJob2Boundary;
// 451~472
typedef SkillBoundary<eCLASS_VALKYRIE, 1, eSKILL_FAST_SHOT, kSKillMindSpark> ValkyrieJob1Boundary;
// 501~524
typedef SkillBoundary<eCLASS_VALKYRIE, 2, eSKILL_UNLIMIT_FORCE, kSkillSummonicBarrier> ValkyrieJob2Boundary;
// 651~671
typedef SkillBoundary<eCLASS_ELEMENTALIST, 1, eSKILL_FIRE_ARROW, kSkillCatalystPower> ElementalistJob1Boundary;
// 701~733
typedef SkillBoundary<eCLASS_ELEMENTALIST, 2, eSKILL_WIND_BOLT, kSkillWhisperOfSylph> ElementalistJob2Boundary;
// 800~817
typedef SkillBoundary<eCLASS_SHADOW, 1, eSKILL_PAIN, kSkillDemonsSoul> ShadowJob1Boundary;
// 850~868
typedef SkillBoundary<eCLASS_SHADOW, 2, eSKILL_WHIP_ATTACK, kSkillDarkRage> ShadowJob2Boundary;


//
//enum eDISCONNECT_REASON
//{
//	UNKNOWN_REASON = 0,
//	MOVE_SYNC_BROKEN,
//	KBMOVE_SYNC_BROKEN,
//	ATTACK_SYNC_BROKEN,
//	TOO_LONG_THRUST_DISTANCE,
//	TOO_MANY_ATTACK_TARGETS,
//	TOO_FAST_ATTACK,
//	INCORRECT_ATTACK_SEQUENCE,
//	INVALID_ATTACK_SEQUENCE,
//	INCORRECT_WEAPON_ON_STYLE_ATTACK,
//	INCORRECT_CLASS_ON_STYLE_ATTACK,
//	NOT_ACQUIRED_STYLE,
//	SPEED_HACK,
//	GM_FORCE_DISCONNECT,						// GM이 게임중 유저를 강제 종료시킨다.
//};

// __NA_001290_20090525_SHIELD_SYSTEM
enum eSD_APPLY
{
    eSD_APPLY_NOT = 0,
    eSD_APPLY_DO,
    eSD_APPLY_MAX,
    eSD_APPLY_EMPTY,
};

#ifdef _NA_004010_20120102_1201_SD_FORMULA
    #define SD_ABSORB_RATIO 0.99
#else
    #define SD_ABSORB_RATIO 0.8
#endif // _NA_004010_20120102_1201_SD_FORMULA

//#ifdef __NA_001244_20090417_ATTACK_RESIST	
enum eATTACK_RESIST
{
    eATTACK_RESIST_NOMAL = 0,
    eATTACK_RESIST_SKILL,
    eATTACK_RESIST_ALL,
    //_NA_008486_20150914_TOTAL_BALANCE
    eATTACK_RESIST_SKILL_DOT_DAMAGE,    //DoT데미지 스킬 (지속 피해상태)
    eATTACK_RESIST_MAX,
};

enum eATTACK_TYPE
{
    eATTACK_TYPE_ALL_OPTION     = 0,    // 모든 옵션
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTACK_TYPE_INVALID        = 0,    // EP2, don't use anymore 'eATTACK_TYPE_ALL_OPTION'
#endif
    //
    eATTACK_TYPE_MELEE          = 1,    // 물리(근거리)
    eATTACK_TYPE_RANGE          = 2,    // 물리(원거리)
    eATTACK_TYPE_WATER          = 3,    // 물
    eATTACK_TYPE_FIRE           = 4,    // 불
    eATTACK_TYPE_WIND           = 5,    // 바람
    eATTACK_TYPE_EARTH          = 6,    // 대지
    eATTACK_TYPE_DARKNESS       = 7,    // 암흑
    eATTACK_TYPE_DIVINE         = 8,    // EP2:unused, 신성
    eATTACK_TYPE_PHYSICAL_OPTION= 9,    // 모든 물리
    eATTACK_TYPE_MAGIC_OPTION   = 10,   // 모든 원소
    eATTACK_TYPE_ALL_MAGIC      = 11,   // EP2:unused, 모든 마법속성
    eATTACK_TYPE_MAGIC          = 12,   // EP2:unused, 마법
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    eATTACK_TYPE_INVALID        = 13,   // same as eATTACK_TYPE_NONE
#endif
#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eATTACK_TYPE_SKILL           = 13,   // 스킬공격
#endif //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eATTACK_TYPE_MAX,
    // EP2:
    eATTACK_TYPE_ABILITY_OPTION_PHYSICAL_BITS = (1 << eATTACK_TYPE_PHYSICAL_OPTION),
    eATTACK_TYPE_ABILITY_OPTION_ELEMENTAL_BITS = (1 << eATTACK_TYPE_WATER)
                                               | (1 << eATTACK_TYPE_FIRE)
                                               | (1 << eATTACK_TYPE_WIND)
                                               | (1 << eATTACK_TYPE_EARTH)
                                               | (1 << eATTACK_TYPE_DARKNESS)
                                               | (1 << eATTACK_TYPE_MAGIC_OPTION),
    eATTACK_TYPE_ABILITY_OPTION_ALL_BITS = eATTACK_TYPE_ABILITY_OPTION_PHYSICAL_BITS
                                         | eATTACK_TYPE_ABILITY_OPTION_ELEMENTAL_BITS,
};

#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
enum eATTR_APPLY_TYPE
{
    eATTR_APPLY_TYPE_NONE     = 0,    // 타입 없음
    eATTR_APPLY_TYPE_NORMAL       = 1,    // 일반적용
    eATTR_APPLY_TYPE_DAMAGED    = 2,    // 피격시 적용
    eATTR_APPLY_TYPE_MAX       
};
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL

// EP2: unused
enum eMELEE_TYPE
{
    eMELEE_TYPE_NOT		    = 0,	// 타입없음(공격없음)
    eMELEE_TYPE_SLASH		= 1,	// 베기
    eMELEE_TYPE_PIERCE,				// 찌르기
    eMELEE_TYPE_HIT,				// 타격
    eMELEE_TYPE_MAGIC,				// 마법(오브, 지팡이)

    eMELEE_TYPE_MAX
};

enum eSTAT_TYPE
{
    eSTAT_TYPE_STR			= 1,	// 힘
    eSTAT_TYPE_DEX,					// 민첩성
    eSTAT_TYPE_VIT,					// 체력
    eSTAT_TYPE_SPR,					// 정신력
    eSTAT_TYPE_INT,					// 지력
};

enum eRANGE_TYPE
{
    eRANGE_TYPE_ALL,		// 모든 사거리
    eRANGE_TYPE_NORMAL,		// 일반 사거리
    eRANGE_TYPE_SKILL,		// 스킬 사거리
};

enum eATTACK_SEQUENCE
{
    eATTACK_SEQUENCE_FIRST,
    eATTACK_SEQUENCE_SECOND,
    eATTACK_SEQUENCE_THIRD,
    eATTACK_SEQUENCE_NONE,

    eATTACK_SEQUENCE_MAX

};

// EP2: unused
enum eARMOR_TYPE
{
    eARMOR_HARD		= 1,	// HEAVY
    eARMOR_MEDIUM,			// MEDIUM
    eARMOR_SOFT,			// LIGHT
    eARMOR_SIEGE,
    eARMOR_UNARMOR,

    eARMOR_TYPE_MAX
};

enum eNPC_GRADE
{
    eNPC_GENERAL			=  1,	//  1 : 일반
    eNPC_ELITE				=  2,	//  2 : 정예
    eNPC_LEADER				=  3,	//  3 : 대장
    eNPC_MIDDLEBOSS			=  4,	//  4 : 중보스
    eNPC_BOSS				=  5,	//  5 : 보스
    eNPC_MERCHANT_NPC		=  6,	//  6 : NPC(상인)
    eNPC_SUMMON_NPC			=  7,	//  7 : NPC(소환체)
    eNPC_MAPOBJECT_NPC		=  8,	//  8 : NPC(맵오브젝트)
    eNPC_LUCKY_MONSTER		=  9,	//  9 : 럭키몬스터
    eNPC_TOTEM				= 10,	// 10 : (WAVERIX)(080911)(REORDERING) 확인 결과 클라만 사용중 (targeting이 되지 않도록 하기 위한... 쩌비...)
    eNPC_SSQ_MONSTER		= 11,	// 11 : 타락한사원 몬스터 __NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__	
    eNPC_CRYSTAL_WARP		= 12,
    eNPC_DOMINATION_MAPOBJECT_NPC =  13, //  13 : 지역점령전_맵오브젝트_피아식별가능 (지역점령전 이외에는 사용할 수 없습니다)
                                         //	 클라이언트에서 해당 오브젝트 처리 사항
                                         //		- 아이들,블럭,데스 에니메이션 무시
                                         //		- 주인길드 셋팅(피아식별: 공격 가능 여부, 타겟 창 출력 여부)
                                         //     - 못가는 타일위에 있어도 공격 가능(기본적으로 지역점령전내에선 못가는 상황일 경우 공격할 수 없습니다)
                                         //		- 파괴되었을때 미니맵에 파괴 아이콘 출력(파괴되기 전엔 미니맵에 아무표시도 하지 않음)
    //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    eNPC_FRIEND_MAPOBJECT_NPC = 14, //  14 : 일반 몬스터와는 적대적, Player와 우호 적인 맵 오브젝트   
    eNPC_FRIEND_MONSTER       = 15, //  15 : 일반 몬스터와는 적대적, Player와 우호형 오브젝트
    eNPC_MOVE_SYNC_MERCHANT   = 16, //  16 : 장거리 이동 NPC등을 위한 타입(서버와 위치 동기화) 
                                    //       - 기획에서 필요로함.
    eNPC_GRADE_MAX		   = 17,
};

enum eNPC_MOVE_ATTITUDE			// 몬스터의 이동성향
{
    eMOVE_ATTITUDE_WANDER			= 0,	// 이동형
    eMOVE_ATTITUDE_ETERNAL_STOP		= 1,	// 완전 정지형
    eMOVE_ATTITUDE_SPAWN_STOP		= 2,	// 스폰 정지형(적이 나타나기 전까지만 정지)
    eMOVE_ATTITUDE_PATROL			= 3,	// 패트롤
    eMOVE_ATTITUDE_SEARCH_AREA		= 4,	// 특정 영역을 우선적으로 검색한다.
    eMOVE_ATTITUDE_PATHLIST 		= 5,	// 경로 이동형 (설정된 경로 1회 이동)   //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST
    eMOVE_ATTITUDE_FOLLOW           = 6,    // 주위 아군 따라가기
    eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK = 7,    // 완전 정지(공격 안함)   //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
                                                  // dead시 사라지지 않음. : 0인 상태로 dead 관련 처리되고, 필드에서 오브젝트 삭제 하지 않는다.
#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
    eMOVE_ATTITUDE_ATTACK_GROUND    = 8,    // 어택 그라운드 형(이동명령 전까지는 스폰 정지형과 같음)
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
    eMOVE_ATTITUDE_MAX
};


enum eATTACK_ATTITUDE
{
    ATTACK_ATTITUDE_AFTER				= 1,	// 후공
    ATTACK_ATTITUDE_NEARES_FIRST		= 2,	// 선공(거리)
    ATTACK_ATTITUDE_LOW_HP_FIRST		= 3,	// 선공(체력)
    ATTACK_ATTITUDE_LOW_LEVEL_FIRST		= 4,	// 선공(레벨)
    ATTACK_ATTITUDE_HIGH_MP_FIRST		= 5,	// 선공(마나)
    ATTACK_ATTITUDE_ONE_TARGET			= 6,	// 단순무식
    ATTACK_ATTITUDE_NO_ATTACK			= 7,	// 공격안함 //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
    ATTACK_ATTITUDE_TYPE_MAX
};

//_NA_0_20100222_UNIT_TRIGGERS_REGEN
// 랜덤이 아닌 경우 일반형태다. 기존 0,1의 방식에 중복 가능한 소환시점속성(2)을 추가

#define REGEN_TYPE_NOMAL   0x00   // 정해진 NPC Code 한마리만 정해진 리젠 타임에 소환한다.
#define REGEN_TYPE_RANDOM  0x01   // 여러개의 NPC Code 중 한마리를 정해진 리젠 타임에 소환한다.
#define REGEN_TYPE_TRIGGER 0x02   // 트리거에 의해 소환 시점이 정해진다.


// 스킬 사용 결과 이펙트 : 만약 여기에 한줄 더 추가한다면 m_bySkillEffect의 크기를 변경해야한다!!!
#define SKILL_EFFECT_KNOCKBACK						0x01
#define SKILL_EFFECT_KNOCKDOWN						0x02
#define SKILL_EFFECT_PIERCE							0x04
#define SKILL_EFFECT_STUN							0x08
#define SKILL_EFFECT_CRITICAL						0x10
#define SKILL_EFFECT_DOUBLEDAMAGE					0x20
#define SKILL_EFFECT_ETHERDEVICE_METAL				0x40
#define SKILL_EFFECT_ETHERDEVICE_ETHER				0x80

enum eSKILL_TYPE
{
    SKILL_TYPE_PASSIVE				= 1,
    SKILL_TYPE_ACTIVE,
    SKILL_TYPE_ACTIVE_INSTANT,
    SKILL_TYPE_ACTIVE_DELAYED,
    SKILL_TYPE_STYLE,				// 일반 3타 공격
    SKILL_TYPE_NORMAL,				// 일반 1, 2타 공격
    SKILL_TYPE_NORMAL_AREA,			// 일반 범위공격
    //
    SKILL_TYPE_UPPERBOUND           // no count, max number of types
};

enum eSKILL_FACTOR_TYPE
{
    SKILL_FACTOR_NOMAL,             // 일반적인 클라이언트의 스킬 사용
    SKILL_FACTOR_ITEM,              // 아이템을 이용한 스킬 사용( 스킬 슬롯을 가지지 않는다. 스킬 쿨타임없다 -> 아이템 쿨타임)
    SKILL_FACTOR_DOMINATION,        // 길드점령전전용 스킬 (스킬 슬롯을 가지지 않는다.  스킬 쿨타임없다 -> 점령전길드 쿨타임)
    SKILL_FACTOR_EFFECT,            // 이펙트가 사용하는 스킬(스킬쿨타임 없음)
};

// 공격패킷필드열거 : 4비트
enum eATTACK_PROPENSITY	
{										
    ATTACK_PROPENSITY_NORMAL	= 0,	//일반 공격
    ATTACK_PROPENSITY_FORCE,			//강제 공격
};

enum eUSER_RELATION_TYPE
{
    USER_RELATION_NEUTRALIST	= 0,		//중립 : 버프 불가, 공격 불가
    USER_RELATION_FRIEND,					//아군 : 버프 가능, 공격 불가
    USER_RELATION_ENEMY,					//적군 : 버프 불가, 공격 가능
};
//#endif

enum eABILITY_RANGE_TYPE
{
    SKILL_ABILITY_NONE                  = 0,    // f110309.1L, can't select
    SKILL_ABILITY_ENEMY					= 1,	// 1 : 적 단수
    SKILL_ABILITY_FRIEND,						// 2 : 아군 단수
    SKILL_ABILITY_TARGETAREA_ENEMY,				// 3 : 지정범위내 적군
    SKILL_ABILITY_TARGETAREA_FRIEND,			// 4 : 지정범위내 아군
    SKILL_ABILITY_MYAREA_ENEMY,					// 5 : 자기중심내 적군
    SKILL_ABILITY_MYAREA_FRIEND,				// 6 : 자기중심내 아군
    SKILL_ABILITY_ME,							// 7 : 자기자신
    SKILL_ABILITY_FIELD,						// 8 : 필드 좌표
    SKILL_ABILITY_CORPSE_RESURRECTION,			// 9 : 부활을 위한 시체(파티원 시체)
    SKILL_ABILITY_SUMMON,						// 10 : 소환(특정 타겟이 없는 경우)
    SKILL_ABILITY_CORPSE_FRIEND,				// 11 : 아군 시체(파티원이 아니라도 상관없음)
    SKILL_ABILITY_CORPSE_RESURRECTION_MYAREA,	// 12 : 시체( 부활용이며, 자신과 파티원만 적용 )
    SKILL_ABILITY_SUMMONED_MONSTER,				// 13 : 소환된 몬스터
    SKILL_ABILITY_PLAYER_ENEMY,					// 14 : 적군 단수 플레이어
    SKILL_ABILITY_MYAREA_CORPSE_ENEMY,			// 15 : 자기중심내 적군 시체.
    SKILL_ABILITY_CORPSE_ENEMY,					// 16 : 적군 시체.
    SKILL_ABILITY_MAX
};

enum eSKILL_TARGET_TYPE
{
    SKILL_TARGET_NONE					= 0,
    SKILL_TARGET_ENEMY,							// 1 : 적
    SKILL_TARGET_FRIEND,						// 2 : 아군
    SKILL_TARGET_ME,				            // 3 : 자신
    SKILL_TARGET_AREA,				            // 4 : 지역 
    SKILL_TARGET_FRIEND_CORPSE,					// 5 : 파티원의 시체
    SKILL_TARGET_REACHABLE_ENEMY,				// 6 : 적(타겟팅하기위해서 접근필요)
    SKILL_TARGET_REACHABLE_FRIEND,				// 7 : 아군(타겟팅하기위해서 접근필요)
    SKILL_TARGET_REACHABLE_ME,					// 8 : 자신(타겟팅하기위해서 접근필요)
    SKILL_TARGET_SUMMON,						// 9 : 타겟없음(소환)
    SKILL_TARGET_ENEMY_PLAYER,					// 10 : 적플레이어
    SKILL_TARGET_ENEMY_CORPSE,					// 11 : 적 시체.
    SKILL_TARGET_AREA_ENEMY_CORPSE,				// 12 : 범위 적 시체.
    SKILL_TARGET_ENEMY_AND_ME,					// 13 : 적( 스킬적용대상은 자기자신 )
    SKILL_TARGET_MAX
};

enum eNPC_STATE
{
    eNPC_STATE_HELP_RESPONSE,	// 도움요청에 응답하는 상태
};

enum eCHAR_CONDITION
{
    eCHAR_CONDITION_STANDUP,	// 서기
    eCHAR_CONDITION_SITDOWN,	// 앉기

    eCHAR_CONDITION_MAX,
};

#define CHAR_ACTION_CONDITION_NONE      0x00	    
#define CHAR_ACTION_CONDITION_MOVING    0x01	    // 이동중
#define CHAR_ACTION_CONDITION_FIGHTING  0x02	    // 전투중

enum eCHAR_EMOTION
{
    eEMOTION_BOW,			// 인사
    eEMOTION_LEAVE,			// 작별
    eEMOTION_QUESTION,		// 의문
    eEMOTION_SIGH,			// 한숨
    eEMOTION_CHEER,			// 환호
    eEMOTION_DENIAL,		// 거부
    eEMOTION_CONSENT,		// 승낙
    eEMOTION_ATTACK,		// 공격
    eEMOTION_THANKS,		// 감사
    eEMOTION_APOLOGY,		// 사과
    eEMOTION_KISS,			// 키스
    eEMOTION_LAUGH,			// 웃음
    eEMOTION_CRYING,		// 울음
    eEMOTION_WELCOME,		// 환영
    eEMOTION_HANDCLAP,		// 박수
    eEMOTION_AWE,			// 경외
    eEMOTION_BEST,			// 최고
    eEMOTION_SULKINESS,		// 삐침
    eEMOTION_PROVOCATION,	// 도발
    eEMOTION_SLEEP,			// 수면
    eEMOTION_POSE,			// 포즈
    eEMOTION_DANCE,			// 춤
    eEMOTION_GAG,			// 개그
    eEMOTION_MIMICRY,		// 흉내
    eEMOTION_SHOCK,			// 충격

    eEMOTION_START_CODE		= 36000,	// 이모티콘 시작 스킬 코드
};

// 스킬(스타일) 공격 범위 형태
enum eSKILL_AREA_TYPE
{
    SRF_FOWARD_ONE		= 1,
    SRF_FOWARD_120,
    SRF_FOWARD_160,
    SRF_FOWARD_360,
    SRF_PIERCE,
    SRF_AREA_POSITION,		// 타겟이 없이 좌표만 있는 스킬이다.
    SRF_MAX
};

enum eNPC_TYPE
{
    eNPC_TYPE_NONE				= 0,
    eNPC_TYPE_STORE				= 1, // 상점	
    eNPC_TYPE_BANK				= 2, //	창고	
    eNPC_TYPE_MAKE_ZONE			= 3, //	방생성	
    eNPC_TYPE_ITEM_MIX			= 4, // 조합및결정화
    eNPC_TYPE_GUARD				= 5, // 경비
    eNPC_TYPE_VILLAGE_PORTAL	= 6, // 마을 이동 npc
    eNPC_TYPE_GUILD				= 7, // 길드관리자
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
    eNPC_TYPE_GUILD_EVENT		= 8, // 길드 이벤트
#endif
    eNPC_TYPE_QUEST				= 14,
    eNPC_TYPE_EVENT				= 15,	// 이벤트 npc(연금술사 아인,)
    eNPC_TYPE_MISSION_RANK		= 16,	// 미션랭킹 npc
    eNPC_TYPE_VENDOR_LIST		= 17,   // 상점 리스트 NPC
    eNPC_TYPE_POINT_RANK		= 18,   // 포인트랭킹 NPC
    eNPC_TYPE_EVENT_DONATION	= 19,   //기부 관련 이벤트 npc(구세군,)
    eNPC_TYPE_EVENT_PERIOD_ITEM	= 20,	//솔로,커플 이벤트 npc(뺏지맨,)
    eNPC_TYPE_EVENT_SANTA		= 21,   //이벤트 npc(산타)
    eNPC_TYPE_EVENT_BUFF		= 22,
    eNPC_TYPE_WARP				= 23,
    eNPC_TYPE_FISHING			= 24,
    eNPC_TYPE_LOTTO				= 25,	//로또 npc
    eNPC_TYPE_EVENT_FRIENDINVITATION	= 26,	//친구 추천 이벤트 아이템 보상 NPC, _KR_0_20091021_FIRENDINVITATION_EXTEND
    eNPC_TYPE_AIRSHIP_MOVE = 27, // 비공정 이동 NPC
    eNPC_TYPE_PROTECTS = 28, // 보호대상 NPC
    eNPC_TYPE_SMARTNPC          = 29,   // 지능형NPC _NA_000000_20140120_SMART_NPC_SYSTEM
};

// 현재의 생각으로는 
// 오버랩되지 않는 플레이어의 행동 상태를 나타냄
enum ePLAYER_BEHAVE_STATE
{
    PLAYER_BEHAVE_IDLE_STATE,				//< 기본값
    PLAYER_BEHAVE_TRADE_STATE,				//< 거래 상태
    PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE,
    PLAYER_BEHAVE_VENDOR_OBSERVER_STATE,
    PLAYER_BEHAVE_NPCSHOP_STATE,
#ifdef _DH_UNMOVEABLE_CLICKED_NPC
#else
    PLAYER_BEHAVE_PARTY_THINKING_LEAVE,     //< 파티를 나갈 것인가 고민중
    PLAYER_BEHAVE_PARTY_THINKING_JOIN,      //< 파티초대에 응할 것인가 고민중
#endif//_DH_UNMOVEABLE_CLICKED_NPC
    PLAYER_BEHAVE_SYSTEM_LOCK,				
    PLAYER_BEHAVE_BANK_STATE,				// 창고 이용
    PLAYER_BEHAVE_UNEXPECTED_WAITING,		// 비정상종료 대기중 //__NA_780_PREVENTION_PARTY_RETURN_ABUSE
    PLAYER_BEHAVE_DIALOG_OPEN_STATE,
    PLAYER_BEHAVE_SERVER_SELECTING_STATE,   // 서버 선택을 위해 대기중... //__NA001334_090420_SERVER_SELECT_FACILITY__
    //__KR_001355_20090624_GROUPINGSYSTEM_ALL
#ifdef _DH_UNMOVEABLE_CLICKED_NPC
#else
    PLAYER_BEHAVE_GROUP_THINKING_LEAVE,		//< 그룹을 나갈 것인가 고민중
    PLAYER_BEHAVE_GROUP_THINKING_JOIN,      //< 그룹초대에 응할 것인가 고민중
#endif//_DH_UNMOVEABLE_CLICKED_NPC
    PLAYER_BEHAVE_SOCKET_COMPOSITE_STATE, // 쟈드 합성

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE, // 전장 예약중..
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
};

enum ePLAYER_TITLE
{
    PLAYER_NONE_TITLE		= (0<<0),
    PLAYER_JUNIOR_FORTUNA	= (1<<0),
    PLAYER_SENIOR_FORTUNA	= (1<<1),
};

// DB에 저장되는 플레이어 상태
// BASE_PLAYERINFO struct
// BYTE		m_byCharState;
// char		m_pszStateTime[MAX_TIMEDATA_SIZE];
enum eDBCHAR_STATE
{
    DBCHAR_STATE_CHAR_BLOCK = 1, //< 캐릭터 접속 금지
};


// 캐릭터 상태
enum eCHAR_STATE_TYPE
{
    eCHAR_STATE_INVALID					= 0,

    //-------------------------------------------------------------------------------------------------
    // 상태이상
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_CHAOS					= 1,		// 혼돈(아무나 공격)
    eCHAR_STATE_BLIND					= 2,		// 블라인드(장님)
    eCHAR_STATE_DOWN					= 3,		// 다운(모든 행동 불능)
    eCHAR_STATE_DELAY					= 4,		// 딜레이
    eCHAR_STATE_SEALING					= 5,		// 봉인(마법스킬 봉인)
    eCHAR_STATE_STUN					= 6,		// 스턴(모든 행동 불능)
    eCHAR_STATE_STONE					= 7,		// 석화(모든 행동 불능, 공격받지 않음)
    eCHAR_STATE_SLEEP					= 8,		// 슬립(모든 행동 불능, 공격을 받으면 해제)
    eCHAR_STATE_FROZEN					= 9,		// 프로즌(모든 행동 불능)
    eCHAR_STATE_SEQUELA					= 10,		// 후유증(부활이후 능력 저하)
    eCHAR_STATE_UPPERDOWN               = 11,       // 어퍼다운 (공중에 뜸, 모든 행동 불능) //_NA_007617_20140916_ADD_CHARSTATE_UPPERDOWN

    //-------------------------------------------------------------------------------------------------
    // 상태약화
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_POISON					= 100,		// 중독(지속적 데미지)
    eCHAR_STATE_WOUND					= 101,		// 상처(지속적 데미지)
    eCHAR_STATE_FIRE_WOUND				= 102,		// 화상(지속적 데미지)
    eCHAR_STATE_PANIC					= 103,		// 공황(MP 감소)
    eCHAR_STATE_LOW_SPIRIT				= 104,		// 무기력(공격력 감소)
    eCHAR_STATE_WEAKENING				= 105,		// 약체(방어력 감소)
    eCHAR_STATE_ATTACK_FAIL				= 106,		// 실패(공격성공률 저하)
    eCHAR_STATE_FRUSTRATION				= 107,		// 좌절(블록성공률 저하)
    eCHAR_STATE_FETTER					= 108,		// 족쇄(이동속도 저하)
    eCHAR_STATE_SLOWDOWN				= 109,		// 둔화(공격속도 저하)
    eCHAR_STATE_HP_REDUCE				= 110,		// 역류(HP 감소)	
    eCHAR_STATE_MP_REDUCE				= 111,		// 감속(MP 감소)
    eCHAR_STATE_LOW_STRENGTH			= 112,		// 체력저하(HP 회복량 감소)
    eCHAR_STATE_DICLINE					= 113,		// 디클라인(MP 회복량 감소)
    eCHAR_STATE_MAGIC_EXPOSE			= 114,		// 마법노출(마법방어력 감소)
    eCHAR_STATE_HPRATE_REDUCE			= 116,		// 약점(HP 회복률 감소)
    eCHAR_STATE_MPRATE_REDUCE			= 117,		// 장애(MP 회복률 감소)

    eCHAR_STATE_MAGIC_ATTACK_DECRESE	= 123,		// 마법공격 저하
    eCHAR_STATE_STAT_STR_DECRESE		= 124,		// 힘 감소
    eCHAR_STATE_STAT_DEX_DECRESE		= 125,		// 민첩 감소
    eCHAR_STATE_STAT_VIT_DECRESE		= 126,		// 체력 감소
    eCHAR_STATE_STAT_INT_DECRESE		= 127,		// 정신력 감소
    eCHAR_STATE_STAT_SPI_DECRESE		= 128,		// 지력 감소

    eCHAR_STATE_STAT_LOWHP_ATTACK_DECREASE	= 129,	// 물리공격 저하
    eCHAR_STATE_STAT_LOWHP_DEFENSE_DECREASE	= 130,	// 물리방어 저하

    eCHAR_STATE_FIRE_ATTACK_DECREASE	= 131,		// 불속성 공격력 저하
    eCHAR_STATE_WATER_ATTACK_DECREASE	= 132,		// 물
    eCHAR_STATE_WIND_ATTACK_DECREASE	= 133,		// 바람
    eCHAR_STATE_EARTH_ATTACK_DECREASE	= 134,		// 대지
    eCHAR_STATE_DARK_ATTACK_DECREASE	= 135,		// 암흑
    
    eCHAR_STATE_FIRE_DEFENSE_DECREASE	= 136,		// 불속성 방어력 저하
    eCHAR_STATE_WATER_DEFENSE_DECREASE	= 137,		// 물
    eCHAR_STATE_WIND_DEFENSE_DECREASE	= 138,		// 바람
    eCHAR_STATE_EARTH_DEFENSE_DECREASE	= 139,		// 대지
    eCHAR_STATE_DARK_DEFENSE_DECREASE	= 140,		// 암흑

    eCHAR_STATE_INCREASE_CASTING_TIME	= 141,		// 캐스팅 타임 증가
    eCHAR_STATE_INCREASE_SKILL_COOL_TIME= 142,		// 스킬 쿨타임 증가
    eCHAR_STATE_MP_SPEND_INCREASE		= 143,		// MP저주(MP 소모 증가)
    // __NA_S00015_20080828_NEW_HOPE_SHADOW
    eCHAR_STATE_PAIN					= 144,		// 페인(고통)
    eCHAR_STATE_FIRE_WOUND2				= 145,		// 다크파이어(화염)
    eCHAR_STATE_PAIN2					= 146,		// 다크페인(암흑의고통)
    eCHAR_STATE_STUN2					= 148,		// 다크스턴(최면)
    eCHAR_STATE_POISON2					= 149,		// 포이즌(인첸트포이즌)
    eCHAR_STATE_BUFF_CANCEL				= 150,		// 버프제거(버프캔슬)
    eCHAR_STATE_EXPLOSION				= 151,		// 자폭(다크브레이크)
    eCHAR_STATE_DARK_SPARK				= 152,		// 스파크(다크스파크)
    eCHAR_STATE_IMPOTENT				= 153,		// 임포턴트
    eCHAR_STATE_CONFUSE					= 154,		// 컨퓨즈
    eCHAR_STATE_CANCELATION				= 155,		// 캔슬레이션
    eCHAR_STATE_HP_SUCTION				= 156,		// HP를 흡수한다.( 난 드라큐라... )
    eCHAR_STATE_MP_SUCTION				= 157,		// MP를 흡수한다.( 난 드라큐라... )
    eCHAR_STATE_MP_FEAR2				= 158,		// 소울스크림( 피어와 동일하나, 이펙트 효과때문에 따로 넣는다. )
    eCHAR_STATE_VITAL_SUCTION			= 159,		// 시체한테 HP를 뺏어온다. 
    eCHAR_STATE_RUSH					= 160,		// 러쉬(이동속도증가+이펙트)
    eCHAR_STATE_CHANGE_ATTR				= 161,		// 속성값을 변경한다. ==> 요 상태일 때는 유지시간이 0이다.
    eCHAR_STATE_ENCHANT_POISON			= 162,		// 인첸트포이즌상(독을 바른 상태)
    eCHAR_STATE_DETECTING_HIDE          = 163,      // 은신체감지
    eCHAR_STATE_WHITE_BLIND             = 164,      // 화이트블라인드(미스틱 스킬, 하얀색 실명)
    eCHAR_STATE_PHOENIX_BURN            = 165,      // 피닉스 화상 (미스틱 스킬 피닉스)
    eCHAR_STATE_POLYMORPH               = 166,      // 폴리모프(미스틱 스킬, 동물변신)
    eCHAR_STATE_DARK_OF_LIGHT_ZONE      = 167,      // 다크오브라이트존
    eCHAR_STATE_GRAVITY_ZONE            = 168,      // 그라비티존
    eCHAR_STATE_AWAKEN_OFFENSE          = 169,      // 어웨이큰 공격력 상승(미스틱 스킬)
    eCHAR_STATE_AWAKEN_DEFENSE          = 170,      // 어웨이큰 방어력 상승(미스틱 스킬)
    eCHAR_STATE_SLIP                    = 171,      // 슬리프 (스킬 사용불가)
    eCHAR_STATE_FATAL_POINT             = 172,      // 홀딩 (미스틱 페이탈포인트)
    
    //!~ _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eCHAR_STATE_ALL_ELEMENT_ATTACK_DECREASE  = 174, // 모든원소공격력감소(슬로우오라)
    eCHAR_STATE_ALL_ELEMENT_DEFENSE_DECREASE = 175, // 모든원소저항률감소(위크너스오라)
    eCHAR_STATE_CRITICAL_RATIO_DECREASE      = 176, // 크리티컬확률감소
    eCHAR_STATE_CRITICAL_DAMAGE_DECREASE     = 177, // 크리티컬데미지감소
    eCHAR_STATE_SKILL_DAMAGE_DECREASE        = 178, // 스킬데미지감소
    eCHAR_STATE_SLOW_AURORA                  = 179, // 슬로우오라
    eCHAR_STATE_WEAKNESS_AURORA              = 180, // 위크너스오라
    eCHAR_STATE_MISCHANCE_AURORA             = 181, // 미스챈스오라
    eCHAR_STATE_DECLINE_AURORA               = 182, // 디클라인오라
    eCHAR_STATE_RECOVERY_AURORA              = 183, // 리커버리오라
    eCHAR_STATE_BOOST_AURORA                 = 184, // 부스트오라
    eCHAR_STATE_IGNORE_AURORA                = 185, // 이그노어오라
    eCHAR_STATE_CONCENTRATION_AURORA         = 186, // 컨센트레이션오라
    eCHAR_STATE_IGNORE_RESERVEHP6            = 187, // HP회복(리커버리무시)
    eCHAR_STATE_BUF_RANGE_DAMAGE2            = 188, // 레인지2(헬로이드용)
    eCHAR_STATE_BUF_RANGE_DAMAGE3            = 189, // 레인지3(헬로이드용)
    eCHAR_STATE_BUF_RANGE_DAMAGE4            = 190, // 레인지4(헬로이드용)
    eCHAR_STATE_DAMAGED_CRITICAL_RATIO_DECREASE      = 191, // 피격 시 상대의 크리티컬 확률 감소
    eCHAR_STATE_DAMAGED_CRITICAL_DAMAGE_DECREASE     = 192, // 노련함(피격 시 상대의 크리티컬 데미지 감소)
    eCHAR_STATE_DAMAGED_CRITICAL_RATIO_INCREASE      = 193, // 날렵함(피격 시 상대의 크리티컬 확률 감소)
    eCHAR_STATE_DAMAGED_CRITICAL_DAMAGE_INCREASE     = 194, // 피격 시 상대의 크리티컬 데미지 증가
    eCHAR_STATE_ELECTRICSHOCK                = 195, // 감전
    eCHAR_STATE_GUARDIANSHIELD               = 196, // 가디언쉴드
    eCHAR_STATE_PROTECTION1                  = 197, // 보호 상태
    //!~ _NA_000000_20130812_NEW_CHARACTER_HELLROID

    kCharStatePoison3 = 3001, // 몬스터 스킬 독
    // end of shadow

    //-------------------------------------------------------------------------------------------------
    // 상태강화
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_ABSORB					= 115,		// 피격 데미지 흡수(HP, MP 흡수)
    eCHAR_STATE_ANGER					= 200,		// 분노(공격력 상승)
    eCHAR_STATE_DEFENSE					= 201,		// 방어(방어력 상승)
    eCHAR_STATE_PROTECTION				= 202,		// 보호(이상상태에 걸리지 않음)
    eCHAR_STATE_FIGHTING				= 203,		// 투지(공격성공률 상승)
    eCHAR_STATE_BALANCE					= 204,		// 균형(블록성공률 상승)
    eCHAR_STATE_VITAL_POWER				= 205,		// 활력(HP 회복률 상승)
    eCHAR_STATE_MEDITATION				= 206,		// 명상(MP, SP 회복률 상승)
    eCHAR_STATE_HIGH_SPIRIT				= 207,		// 기세(공격속도 증가)
    eCHAR_STATE_SPEEDING				= 208,		// 질주(이동속도 증가)
    eCHAR_STATE_CONCENTRATION			= 209,		// 집중(크리티컬확률 증가)
    eCHAR_STATE_INCREASE_SKILLRANGE		= 210,		// 시야(스킬사거리 증가)
    eCHAR_STATE_PRECISION				= 211,		// 정밀(크리티컬 데미지 증가)
    eCHAR_STATE_HP_INCREASE				= 212,		// 격려(HP 증가)
    eCHAR_STATE_MP_INCREASE				= 213,		// 탄력(MP 증가)
    eCHAR_STATE_HPRATE_INCREASE			= 214,		// 활기(HP 회복량 증가)
    eCHAR_STATE_MPRATE_INCREASE			= 215,		// 자극(MP 회복량 증가)
    eCHAR_STATE_CURE					= 216,		// 치료(모든상태이상 취소)
    eCHAR_STATE_MAGIC_DEFENSE			= 217,		// 마법방어(마법방어력 증가)
    eCHAR_STATE_MAGIC_SHIELD			= 218,		// 마법보호(마법쉴드 HP)
    eCHAR_STATE_HOLDING					= 219,		// 홀딩(이동불가, 공격가능)
    eCHAR_STATE_SP_BONUS				= 221,		// SP당 추가 공격력(물리데미지 증가)
    eCHAR_STATE_BUF_RANGE_DAMAGE		= 222,		// 레인지(주변적들에게 지속적 데미지)
    eCHAR_STATE_STAT_STR				= 223,		// 힘강화
    eCHAR_STATE_STAT_DEX				= 224,		// 민첩강화
    eCHAR_STATE_STAT_VIT				= 225,		// 체력강화
    eCHAR_STATE_STAT_SPI				= 226,		// 정신력강화
    eCHAR_STATE_STAT_INT				= 227,		// 지력강화
    eCHAR_STATE_MAGIC_ATTACK_INCREASE	= 228,		// 마법공격 상승

    eCHAR_STATE_STAT_LOWHP_ATTACK_INCREASE	= 229,	// 물리공격 상승
    eCHAR_STATE_STAT_LOWHP_DEFENSE_INCREASE	= 230,	// 물리방어 상승

    eCHAR_STATE_STAT_DAMAGE_ADD			= 231,		// 추가 데미지

    eCHAR_STATE_FIRE_ATTACK_INCREASE	= 232,		// 불속성 공격력 상승
    eCHAR_STATE_WATER_ATTACK_INCREASE	= 233,		// 물
    eCHAR_STATE_WIND_ATTACK_INCREASE	= 234,		// 바람
    eCHAR_STATE_EARTH_ATTACK_INCREASE	= 235,		// 대지
    eCHAR_STATE_DARK_ATTACK_INCREASE	= 236,		// 암흑

    eCHAR_STATE_FIRE_DEFENSE_INCREASE	= 237,		// 불속성 방어력 상승
    eCHAR_STATE_WATER_DEFENSE_INCREASE	= 238,		// 물
    eCHAR_STATE_WIND_DEFENSE_INCREASE	= 239,		// 바람
    eCHAR_STATE_EARTH_DEFENSE_INCREASE	= 240,		// 대지
    eCHAR_STATE_DARK_DEFENSE_INCREASE	= 241,		// 암흑

    eCHAR_STATE_DECREASE_CASTING_TIME	= 242,		// 캐스팅 타임 감소
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME= 243,		// 스킬 쿨타임 감소
    eCHAR_STATE_MP_SPEND_DECREASE		= 244,		// MP활력(MP 소모 감소)

    eCHAR_STATE_REFLECT_DAMAGE			= 220,		// 피격데미지반사(데미지 반사)
    eCHAR_STATE_REFLECT_SLOW			= 118,		// 피격족쇄(데미지 + 이속저하)
    eCHAR_STATE_REFLECT_FROZEN			= 119,		// 피격프로즌(데미지 + 모든행동불능)
    eCHAR_STATE_REFLECT_SLOWDOWN		= 120,		// 피격둔화(데미지 + 공속저하)
    eCHAR_STATE_REFLECT_STUN			= 121,		// 피격스턴(데미지 + 모든행동불능)
    eCHAR_STATE_REFLECT_FEAR			= 122,		// 피격피어(데미지 + 도망)
    eCHAR_STATE_IGNORE_RESERVEHP		= 245,		// 활력(HP적립무시)
    eCHAR_STATE_ANGER8					= 246,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE8	= 247,
    eCHAR_STATE_DEFENSE8				= 248,
    eCHAR_STATE_MAGIC_DEFENSE7			= 249,
    eCHAR_STATE_PRECISION7				= 250,
    //{_NA_001231_20081216_ADD_OPTION_KIND
    eCHAR_STATE_RESIST_HOLDING			= 251,		//홀딩 내성.
    eCHAR_STATE_RESIST_SLEEP			= 252,		//슬립 내성.
    eCHAR_STATE_RESIST_POISON			= 253,		//중독 내성.
    eCHAR_STATE_RESIST_KNOCKBACK		= 254,		//넉백 내성.
    eCHAR_STATE_RESIST_DOWN				= 255,		//다운 내성.
    eCHAR_STATE_RESIST_STUN				= 256,		//스턴 내성.
    eCHAR_STATE_DECREASE_PVPDAMAGE		= 257,		//PVP데미지 감소.
    //}
//#ifdef __NA_001244_20090417_ATTACK_RESIST
//	eCHAR_STATE_RESIST_NOMALATTACK_ALLCHAR	= 258,		//	모든케릭 일반공격 내성.
//	eCHAR_STATE_RESIST_SKILLATTACK_ALLCHAR	= 259,		//	모든케릭 스킬공격 내성.
//	eCHAR_STATE_RESIST_ALLATTACK_ALLCHAR	= 260,		//	모든케릭 모든공격 내성.
//	eCHAR_STATE_RESIST_NOMALATTACK_BERSERKER = 261,		//	버서커 일반공격 내성.	
//	eCHAR_STATE_RESIST_SKILLATTACK_BERSERKER = 262,		//	버서커 스킬공격 내성.
//	eCHAR_STATE_RESIST_ALLATTACK_BERSERKER	= 263,		//	버서커 모든공격 내성.	
//	eCHAR_STATE_RESIST_NOMALATTACK_VALKYRIE = 264,  	//	발키리 일반공격 내성.	
//	eCHAR_STATE_RESIST_SKILLATTACK_VALKYRIE = 265,  	//	발키리 스킬공격 내성.
//	eCHAR_STATE_RESIST_ALLATTACK_VALKYRIE	= 266,		//	발키리 모든공격 내성.	
//	eCHAR_STATE_RESIST_NOMALATTACK_DRAGON	= 267,		//	드래곤 일반공격 내성.	
//	eCHAR_STATE_RESIST_SKILLATTACK_DRAGON	= 268,		//	드래곤 스킬공격 내성.
//	eCHAR_STATE_RESIST_ALLATTACK_DRAGON		= 269,		//	드래곤 모든공격 내성.	
//	eCHAR_STATE_RESIST_NOMALATTACK_ELEMENTALIST	= 270,	//	엘리 일반공격 내성.	
//	eCHAR_STATE_RESIST_SKILLATTACK_ELEMENTALIST	= 271,	//	엘리 스킬공격 내성.	
//	eCHAR_STATE_RESIST_ALLATTACK_ELEMENTALIST	= 272,	//	엘리 모든공격 내성.	
//	eCHAR_STATE_RESIST_NOMALATTACK_SHADOW	= 273,		//	쇄도우 일반공격 내성.		
//	eCHAR_STATE_RESIST_SKILLATTACK_SHADOW	= 274,		//	쇄도우 스킬공격 내성.	
//	eCHAR_STATE_RESIST_ALLATTACK_SHADOW		= 275,		//	쇄도우 모든공격 내성.		
//#endif
    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ENCOURAGEMENT      			= 276,		// Max sd 증가
    eCHAR_STATE_PROMOTION       			= 277,		// sd 회복량 증가
    //-------------------------------------------------------------------------------------------------
    // 특수
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_TRANSFORMATION			= 300,		// 변신
    eCHAR_STATE_STEEL					= 301,		// 강철(이동 외 행동 불능, 이동속도 감소)
    eCHAR_STATE_TRANSPARENT				= 302,		// 투명(이동 외의 행동시 해제)
    eCHAR_STATE_FEAR					= 303,		// 피어(대상을 도망가게 한다.)
    eCHAR_STATE_BLUR					= 304,		// 공격불능(대상은 공격을 못한다.)
    eCHAR_STATE_THRUST					= 305,		// 밀리기(밀리고 있는 상태)
    eCHAR_STATE_SUMMON					= 306,		// 소환
    eCHAR_STATE_SPCHARGE				= 307,		// 검기충전
    eCHAR_STATE_CURSE_INCREASE			= 308,		// 저주강화(커스인크리즈)
    //_NA001385_20090924_DOMINATION_ETC
    eCHAR_STATE_STAMP	    			= 309,		// 각인 상태(이상상태에 걸리지 않음)
    kCharStateIncreseHeal = 310, // 치유량 증가 상태
    kCharStateActiveComboSkill = 311, // 연계 스킬 활성화
    kCharStateIncreseSkillDamage = 312, // 스킬 데미지 증가 상태
    // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    eCHAR_STATE_FUGITIVE = 313, // 전장 도망자
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    eCHAR_STATE_TRANSFORMATION6         = 315,      // 빙의
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    //_NA_007667_20141001_WITCHBLADE_SKILL
    eCHAR_STATE_VICE_SWORD              = 400,      // 바이스소드
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eCHAR_STATE_CHARMED                 = 401,      // 매혹(시전자를 따라오게 한다.)
    eCHAR_STATE_ALL_ELEMENTS_INCREASE   = 402,      // 모든 속성 공격력 증가
    eCHAR_STATE_REFLECT_LINK_SKILL      = 403,      // 피격시 스킬사용(데미지 + 스킬)
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eCHAR_STATE_EVADE                   = 404,      // 이베이드
    eCHAR_STATE_FURYFORMATION           = 405,      // 퓨리포메이션
    eCHAR_STATE_RISING_FORCE            = 406,      // 라이징포스
    eCHAR_STATE_FP_POWER_INCREASE       = 407,     // FP에 의한 데미지증가효과를 증가시킨다
    eCHAR_STATE_WIT_VITAL_POWER         = 408,      // 위치블레이드 활력
    eCHAR_STATE_WIT_CONCENTRATION       = 409,      // 위치블레이드 집중
    eCHAR_STATE_INCREASE_DAMAGE         = 410,      // 데미지 증가
    eCHAR_STATE_AUTOCAST_BLEEDING_ATTACK = 411,      // 출혈공격 버프
    eCHAR_STATE_RISING_FORCE_IMMUNITY_DAMAMGE = 412, // 라이징포스용 무적 //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    //-------------------------------------------------------------------------------------------------
    // 상태강화_물약
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_ANGER3						= 500,		// 분노3
    eCHAR_STATE_DEFENSE3					= 501,		// 방어3
    eCHAR_STATE_FIGHTING3					= 502,		// 투지3
    eCHAR_STATE_BALANCE3					= 503,		// 균형3
    eCHAR_STATE_VITAL_POWER3				= 504,		// 활력3
    eCHAR_STATE_MEDITATION3					= 505,		// 명상3
    eCHAR_STATE_HIGH_SPIRIT3				= 506,		// 기세3
    eCHAR_STATE_SPEEDING3					= 507,		// 질주3
    eCHAR_STATE_CONCENTRATION3				= 508,		// 집중3
    eCHAR_STATE_INCREASE_SKILLRANGE3		= 509,		// 시야3
    eCHAR_STATE_PRECISION3					= 510,		// 정밀3
    eCHAR_STATE_HP_INCREASE3				= 511,		// 격려3
    eCHAR_STATE_MP_INCREASE3				= 512,		// 탄력3
    eCHAR_STATE_HPRATE_INCREASE3			= 513,		// 활기3
    eCHAR_STATE_MPRATE_INCREASE3			= 514,		// 자극3
    eCHAR_STATE_MAGICDEFENSE3				= 515,		// 마법방어3
    eCHAR_STATE_STAT_STR3					= 516,		// 힘강화3
    eCHAR_STATE_STAT_DEX3					= 517,		// 민첩강화3
    eCHAR_STATE_STAT_VIT3					= 518,		// 체력강화3
    eCHAR_STATE_STAT_INT3					= 519,		// 정신력강화3
    eCHAR_STATE_STAT_SPI3					= 520,		// 지력강화3
    eCHAR_STATE_MAGIC_ATTACK_INCREASE3		= 521,		// 마법공격상승3
    eCHAR_STATE_STAT_DAMAGE_ADD3			= 522,		// 추가데미지3
    eCHAR_STATE_FIRE_ATTACK_INCREASE3		= 523,		// 불속성공격력증가3
    eCHAR_STATE_WATER_ATTACK_INCREASE3		= 524,		// 물속성공격력증가3
    eCHAR_STATE_WIND_ATTACK_INCREASE3		= 525,		// 바람속성공격력증가3
    eCHAR_STATE_EARTH_ATTACK_INCREASE3		= 526,		// 대지속성공격력증가3
    eCHAR_STATE_DARK_ATTACK_INCREASE3		= 527,		// 암흑속성공격력증가3
    eCHAR_STATE_FIRE_DEFENSE_INCREASE3		= 528,		// 불속성방어력증가3
    eCHAR_STATE_WATER_DEFENSE_INCREASE3		= 529,		// 물속성방어력증가3
    eCHAR_STATE_WIND_DEFENSE_INCREASE3		= 530,		// 바람속성방어력증가3
    eCHAR_STATE_EARTH_DEFENSE_INCREASE3		= 531,		// 대지속성방어력증가3
    eCHAR_STATE_DARK_DEFENSE_INCREASE3		= 532,		// 암흑속성방어력증가3
    eCHAR_STATE_MP_SPEND_INCREASE3			= 533,		// MP저주3(MP 소모 증가)
    eCHAR_STATE_MP_SPEND_DECREASE3			= 534,		// MP활력3(MP 소모 감소)	
    eCHAR_STATE_DECREASE_CASTING_TIME3		= 535,		//캐스팅 타임 감소3
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME3	= 536,		//스킬 쿨타임 감소3	
    eCHAR_STATE_TRANSFORMATION3				= 537,		// 변신3
    //eCHAR_STATE_STEEL3						= 538,		// 강철3(이동 외 행동 불능, 이동속도 감소)
    eCHAR_STATE_TRANSPARENT3				= 539,		// 투명3(이동 외의 행동시 해제)
    eCHAR_STATE_FEAR3						= 540,		// 피어3(대상을 도망가게 한다.)
    eCHAR_STATE_BLUR3						= 541,		// 공격불능3(대상은 공격을 못한다.)
    eCHAR_STATE_THRUST3						= 542,		// 밀리기3(밀리고 있는 상태)
    eCHAR_STATE_SUMMON3						= 543,		// 소환3
    eCHAR_STATE_SPCHARGE3					= 544,		// 검기충전3
    eCHAR_STATE_IGNORE_RESERVEHP3			= 545,		// 활력(HP적립무시3)
    eCHAR_STATE_ANGER9						= 546,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE9		= 547,
    eCHAR_STATE_DEFENSE9					= 548,
    eCHAR_STATE_MAGIC_DEFENSE8				= 549,
    eCHAR_STATE_PRECISION8					= 550,

    //__NA_001290_20090525_SHIELD_SYSTEM
     eCHAR_STATE_ENCOURAGEMENT3      		= 576,		// Max sd 증가
     eCHAR_STATE_PROMOTION3       			= 577,		// sd 회복량 증가

    //-------------------------------------------------------------------------------------------------
    // 상태강화_이벤트
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_ANGER4						= 700,		// 분노4
    eCHAR_STATE_DEFENSE4					= 701,		// 방어4
    eCHAR_STATE_FIGHTING4					= 702,		// 투지4
    eCHAR_STATE_BALANCE4					= 703,		// 균형4
    eCHAR_STATE_VITAL_POWER4				= 704,		// 활력4
    eCHAR_STATE_MEDITATION4					= 705,		// 명상4
    eCHAR_STATE_HIGH_SPIRIT4				= 706,		// 기세4
    eCHAR_STATE_SPEEDING4					= 707,		// 질주4
    eCHAR_STATE_CONCENTRATION4				= 708,		// 집중4
    eCHAR_STATE_INCREASE_SKILLRANGE4		= 709,		// 시야4
    eCHAR_STATE_PRECISION4					= 710,		// 정밀4
    eCHAR_STATE_HP_INCREASE4				= 711,		// 격려4
    eCHAR_STATE_MP_INCREASE4				= 712,		// 탄력4
    eCHAR_STATE_HPRATE_INCREASE4			= 713,		// 활기4
    eCHAR_STATE_MPRATE_INCREASE4			= 714,		// 자극4
    eCHAR_STATE_MAGICDEFENSE4				= 715,		// 마법방어4
    eCHAR_STATE_STAT_STR4					= 716,		// 힘강화4
    eCHAR_STATE_STAT_DEX4					= 717,		// 민첩강화4
    eCHAR_STATE_STAT_VIT4					= 718,		// 체력강화4
    eCHAR_STATE_STAT_INT4					= 719,		// 정신력강화4
    eCHAR_STATE_STAT_SPI4					= 720,		// 지력강화4
    eCHAR_STATE_MAGIC_ATTACK_INCREASE4		= 721,		// 마법공격상승4
    eCHAR_STATE_STAT_DAMAGE_ADD4			= 722,		// 추가데미지4
    eCHAR_STATE_FIRE_ATTACK_INCREASE4		= 723,		// 불속성공격력증가4
    eCHAR_STATE_WATER_ATTACK_INCREASE4		= 724,		// 물속성공격력증가4
    eCHAR_STATE_WIND_ATTACK_INCREASE4		= 725,		// 바람속성공격력증가4
    eCHAR_STATE_EARTH_ATTACK_INCREASE4		= 726,		// 대지속성공격력증가4
    eCHAR_STATE_DARK_ATTACK_INCREASE4		= 727,		// 암흑속성공격력증가4
    eCHAR_STATE_FIRE_DEFENSE_INCREASE4		= 728,		// 불속성방어력증가4
    eCHAR_STATE_WATER_DEFENSE_INCREASE4		= 729,		// 물속성방어력증가4
    eCHAR_STATE_WIND_DEFENSE_INCREASE4		= 730,		// 바람속성방어력증가4
    eCHAR_STATE_EARTH_DEFENSE_INCREASE4		= 731,		// 대지속성방어력증가4
    eCHAR_STATE_DARK_DEFENSE_INCREASE4		= 732,		// 암흑속성방어력증가4
    eCHAR_STATE_MP_SPEND_INCREASE4			= 733,		// MP저주4(MP 소모 증가)
    eCHAR_STATE_SPCHARGE4					= 744,		// 검기충전4
    eCHAR_STATE_MP_SPEND_DECREASE4			= 734,		// MP활력4(MP 소모 감소)	
    eCHAR_STATE_DECREASE_CASTING_TIME4		= 735,		//캐스팅 타임 감소4
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME4	= 736,		//스킬 쿨타임 감소4	
    eCHAR_STATE_TRANSFORMATION4				= 737,		// 변신4
    //eCHAR_STATE_STEEL4						= 738,		// 강철4(이동 외 행동 불능, 이동속도 감소)
    eCHAR_STATE_TRANSPARENT4				= 739,		// 투명4(이동 외의 행동시 해제)
    eCHAR_STATE_FEAR4						= 740,		// 피어4(대상을 도망가게 한다.)
    eCHAR_STATE_BLUR4						= 741,		// 공격불능4(대상은 공격을 못한다.)
    eCHAR_STATE_THRUST4						= 742,		// 밀리기4(밀리고 있는 상태)
    eCHAR_STATE_SUMMON4						= 743,		// 소환4
    eCHAR_STATE_IGNORE_RESERVEHP4			= 745,		// 활력(HP적립무시4)
    eCHAR_STATE_ANGER10						= 746,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE10		= 747,
    eCHAR_STATE_DEFENSE10					= 748,
    eCHAR_STATE_MAGIC_DEFENSE9				= 749,
    eCHAR_STATE_PRECISION9					= 750,
    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ENCOURAGEMENT4     			= 776,		// Max sd 증가
    eCHAR_STATE_PROMOTION4       			= 777,		// sd 회복량 증가
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // 스킬 버프 상태 추가 951 ~ 998
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    kCharStateHardStance = 951, // 하드 스탠스 (지속 체력)
    kCharStateIronStance = 952, // 아이언 스탠스 (순간 체력)
    kCharStateBloodHaze = 953, // 블러드 헤이즈 (지속 HP)
    kCharStateMotalBlood = 954, // 모탈 블러드 (순간 HP)
    kCharStateCourageOfKnights = 955, // 커리지 오브 나이츠 (지속 민첩)
    kCharStateFeverOfKnights = 956, // 피버 오브 나이츠 (순간 민첩)
    kCharStateDragonicForce = 957, // 드래고닉 포스 (지속 공격력)
    kCharStateDragonicBless = 958, // 드래고닉 블레스 (순간 공격력)
    kCharStateHelronsSoul = 959, // 헬론즈 소울 (지속 정신력)
    kCharStateDemonsSoul = 960, // 데몬즈 소울 (순간 정신력)
    kCharStateDarkTrace = 961, // 다크 트레이스 (지속 공격속도)
    kCharStateDarkRage = 962, // 다크 레이지 (순간 공격속도)
    kCharStateIncreaseMind = 963, // 인크리스 마인드 (지속 지력)
    kCharStateMindSpark = 964, // 마인드 스파크 (순간 지력)
    kCharStateSummonicShield = 965, // 서머닉 실드 (지속 방어력)
    kCharStateSummonicBarrier = 966, // 서머닉 베리어 (순간 방어력)
    kCharStateEnchantPower = 967, // 인챈트 파워 (지속 힘)
    kCharStateCatalystPower = 968, // 카탈리스트 파워 (순간 힘)
    kCharStateWhisperOfWind = 969, // 위스퍼 오브 윈드 (지속 이동속도)
    kCharStateWhisperOfSylph = 970, // 위스퍼 오브 실프 (순간 이동속도)
    eCHAR_STATE_FIRE_ATTACK_INCREASE6   = 971,  // 불 속성공력 증가 (미스틱)
    eCHAR_STATE_WATER_ATTACK_INCREASE6  = 972,  // 물 속성공력 증가 (미스틱)
    eCHAR_STATE_WIND_ATTACK_INCREASE6   = 973,  // 바람 속성공력 증가 (미스틱)
    eCHAR_STATE_EARTH_ATTACK_INCREASE6  = 974,  // 대지 속성공력 증가 (미스틱)
    
    kCharStateIncreaseAggroPoint = 999, // 어그로 포인트 증가 상태
    
    //-------------------------------------------------------------------------------------------------
    // 상태강화_기타
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_ANGER5						= 1500,		// 분노5
    eCHAR_STATE_DEFENSE5					= 1501,		// 방어5
    eCHAR_STATE_FIGHTING5					= 1502,		// 투지5
    eCHAR_STATE_BALANCE5					= 1503,		// 균형5
    eCHAR_STATE_VITAL_POWER5				= 1504,		// 활력5
    eCHAR_STATE_MEDITATION5					= 1505,		// 명상5
    eCHAR_STATE_HIGH_SPIRIT5				= 1506,		// 기세5
    eCHAR_STATE_SPEEDING5					= 1507,		// 질주5
    eCHAR_STATE_CONCENTRATION5				= 1508,		// 집중5
    eCHAR_STATE_INCREASE_SKILLRANGE5		= 1509,		// 시야5
    eCHAR_STATE_PRECISION5					= 1510,		// 정밀5
    eCHAR_STATE_HP_INCREASE5				= 1511,		// 격려5
    eCHAR_STATE_MP_INCREASE5				= 1512,		// 탄력5
    eCHAR_STATE_HPRATE_INCREASE5			= 1513,		// 활기5
    eCHAR_STATE_MPRATE_INCREASE5			= 1514,		// 자극5
    eCHAR_STATE_MAGICDEFENSE5				= 1515,		// 마법방어5
    eCHAR_STATE_STAT_STR5					= 1516,		// 힘강화5
    eCHAR_STATE_STAT_DEX5					= 1517,		// 민첩강화5
    eCHAR_STATE_STAT_VIT5					= 1518,		// 체력강화5
    eCHAR_STATE_STAT_INT5					= 1519,		// 정신력강화5
    eCHAR_STATE_STAT_SPI5					= 1520,		// 지력강화5
    eCHAR_STATE_MAGIC_ATTACK_INCREASE5		= 1521,		// 마법공격상승5
    eCHAR_STATE_STAT_DAMAGE_ADD5			= 1522,		// 추가데미지5
    eCHAR_STATE_FIRE_ATTACK_INCREASE5		= 1523,		// 불속성공격력증가5
    eCHAR_STATE_WATER_ATTACK_INCREASE5		= 1524,		// 물속성공격력증가5
    eCHAR_STATE_WIND_ATTACK_INCREASE5		= 1525,		// 바람속성공격력증가5
    eCHAR_STATE_EARTH_ATTACK_INCREASE5		= 1526,		// 대지속성공격력증가5
    eCHAR_STATE_DARK_ATTACK_INCREASE5		= 1527,		// 암흑속성공격력증가5
    eCHAR_STATE_FIRE_DEFENSE_INCREASE5		= 1528,		// 불속성방어력증가5
    eCHAR_STATE_WATER_DEFENSE_INCREASE5		= 1529,		// 물속성방어력증가5
    eCHAR_STATE_WIND_DEFENSE_INCREASE5		= 1530,		// 바람속성방어력증가5
    eCHAR_STATE_EARTH_DEFENSE_INCREASE5		= 1531,		// 대지속성방어력증가5
    eCHAR_STATE_DARK_DEFENSE_INCREASE5		= 1532,		// 암흑속성방어력증가5
    eCHAR_STATE_MP_SPEND_INCREASE5			= 1533,		// MP저주5(MP 소모 증가)
    eCHAR_STATE_MP_SPEND_DECREASE5			= 1534,		// MP활력5(MP 소모 감소)	
    eCHAR_STATE_DECREASE_CASTING_TIME5		= 1535,		//캐스팅 타임 감소5
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME5	= 1536,		//스킬 쿨타임 감소5	
    eCHAR_STATE_TRANSFORMATION5				= 1537,		// 변신5
    //eCHAR_STATE_STEEL5						= 1538,		// 강철5(이동 외 행동 불능, 이동속도 감소)
    eCHAR_STATE_TRANSPARENT5				= 1539,		// 투명5(이동 외의 행동시 해제)
    eCHAR_STATE_FEAR5						= 1540,		// 피어5(대상을 도망가게 한다.)
    eCHAR_STATE_BLUR5						= 1541,		// 공격불능5(대상은 공격을 못한다.)
    eCHAR_STATE_THRUST5						= 1542,		// 밀리기5(밀리고 있는 상태)
    eCHAR_STATE_SUMMON5						= 1543,		// 소환5
    eCHAR_STATE_SPCHARGE5					= 1544,		// 검기충전5
    eCHAR_STATE_IGNORE_RESERVEHP5			= 1545,		// 활력(HP적립무시5)
    eCHAR_STATE_ANGER12						= 1546,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE12		= 1547,
    eCHAR_STATE_DEFENSE12					= 1548,
    eCHAR_STATE_MAGIC_DEFENSE11				= 1549,
    eCHAR_STATE_PRECISION11					= 1550,
    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ENCOURAGEMENT5     			= 1576,		// Max sd 증가
    eCHAR_STATE_PROMOTION5       			= 1577,		// sd 회복량 증가

    eCHAR_STATE_PERIODIC_DAMAGE			= 5000,		// 주기 데미지

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //-------------------------------------------------------------------------------------------------
    // 길드 시설 액티브 상태들
    // 길드 시설 액티브 상태는 사용되는 어빌리티가 가지는 상태와 따로 관리되어야 하므로 전용 상태 생성
    //------------------------------------------------------------------------------------------------- 
    //첫번째 어빌리티 상태들
    eCHAR_STATE_MERCENARIES_ACTIVE          = 4001, // 길드 시설 용병 조합 액티브 상태
    eCHAR_STATE_ELDER_ACTIVE                = 4002, // 길드 시설 원로회 액티브 상태
    eCHAR_STATE_ECLIPSE_ACTIVE              = 4003, // 길드 시설 이클립스 액티브 상태
    eCHAR_STATE_CLAN_ACTIVE                 = 4004, // 길드 시설 클랜 액티브 상태
    eCHAR_STATE_MAGIC_ACTIVE                = 4005, // 길드 시설 마법연구소 액티브 상태
    eCHAR_STATE_PELES_ACTIVE                = 4006, // 길드 시설 팰레스 액티브 상태
    eCHAR_STATE_HER_ACTIVE                  = 4007, // 길드 시설 헬로이드 액티브 상태
    //두번째 어빌리티 상태들
    eCHAR_STATE_ELDER_ACTIVE2               = 4008, // 길드 시설 원로회 액티브 상태2
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    //-------------------------------------------------------------------------------------------------
    // 어빌리티 외의 상태들
    //------------------------------------------------------------------------------------------------- 
    eCHAR_STATE_STYLE_THRUST			= 5001,		// 스타일 공격에서 사용하는 밀리기
    eCHAR_STATE_STYLE_STUN				= 5002,
    eCHAR_STATE_STYLE_DOWN				= 5003,

    eCHAR_STATE_ETC_FLYING				= 5004,		// 공중에 떠 있는 상태
    eCHAR_STATE_ETC_DISABLE_VISION		= 5005,		// 시야가 꺼져 있으면 몬스터들이 찾지도 못하고 공격도 못한다. 
    eCHAR_STATE_ETC_RETURNING			= 5006,		// 부활위치로 돌아가는 상태
    eCHAR_STATE_ETC_HELP				= 5007,		// 도와주러 가는 상태

    eCHAR_STATE_ETC_AUTO_RECOVER_HP		= 5008,		// 자동 HP, MP 회복
    eCHAR_STATE_ETC_AUTO_RECOVER_MP		= 5009,
    eCHAR_STATE_ETC_AUTO_RECOVER_HPMP	= 5010,
    eCHAR_STATE_ETC_ITEM_RECOVER_HP		= 5011,		// 아이템에 의한 HP, MP 회복
    eCHAR_STATE_ETC_ITEM_RECOVER_MP		= 5012,

    eCHAR_STATE_ETC_FORCED_WARP			= 5013,		// 서버에의한 강제워프 상태(이동동기를 맞추기 위해서 잠시 이동을 못한다.)
    eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER	= 5014,		// 시야가 꺼져 있으면 몬스터들이 찾지도 못하고 공격도 못한다. (트리거에서 사용)
    eCHAR_STATE_BLUR_TRIGGER				= 5015,		// 공격불능(대상은 공격을 못한다. 트리거용)

    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ETC_EXIST_SHELD_POINT	= 5016,     //SD 상태 (0이상 존재하는 상태) 
    eCHAR_STATE_ETC_AUTO_RECOVER_SD		= 5017,     //SD 회복 상태 
    eCHAR_STATE_ETC_FIGHTING    		= 5018,     //SD 전투 상태

    //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    eCHAR_STATE_ETC_SKIP_UPDATE_TARGET_LIST = 5019,		// 타겟 리스트 갱신 하지 않는 상태.
    // _NA_0_20100817_HELLON_LAST_TRIGGER
    eCHAR_STATE_ETC_TRIGGER_HOLDING			= 5020,		// 트리거를 통한 연출을 위하여 이동이 금지된 상태 

    // WOPS:6554
    eCHAR_STATE_ETC_ITEM_RECOVER_HP_INSTANTLY = 5021,   // 즉시 회복형 아이템에 의한 HP 회복
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eCHAR_STATE_ETC_AUTO_RECOVER_FP         = 5022,     // 자동 FP회복( - )
    //-------------------------------------------------------------------------------------------------
    // 상태강화_캐쉬
    //------------------------------------------------------------------------------------------------- 
    eCHAR_STATE_ANGER2					= 1000,		// 분노2
    eCHAR_STATE_DEFENSE2				= 1001,		// 방어2
    eCHAR_STATE_FIGHTING2				= 1002,		// 투지2
    eCHAR_STATE_BALANCE2				= 1003,		// 균형2
    eCHAR_STATE_VITAL_POWER2			= 1004,		// 활력2
    eCHAR_STATE_MEDITATION2				= 1005,		// 명상2
    eCHAR_STATE_HIGH_SPIRIT2			= 1006,		// 기세2
    eCHAR_STATE_SPEEDING2				= 1007,		// 질주2
    eCHAR_STATE_CONCENTRATION2			= 1008,		// 집중2
    eCHAR_STATE_INCREASE_SKILLRANGE2	= 1009,		// 시야2
    eCHAR_STATE_PRECISION2				= 1010,		// 정밀2
    eCHAR_STATE_HP_INCREASE2			= 1011,		// 격려2
    eCHAR_STATE_MP_INCREASE2			= 1012,		// 탄력2
    eCHAR_STATE_HPRATE_INCREASE2		= 1013,		// 활기2
    eCHAR_STATE_MPRATE_INCREASE2		= 1014,		// 자극2
    eCHAR_STATE_MAGIC_DEFENSE2			= 1015,		// 마법방어2
    eCHAR_STATE_STAT_STR2				= 1016,		// 힘강화2
    eCHAR_STATE_STAT_DEX2				= 1017,		// 민첩강화2
    eCHAR_STATE_STAT_VIT2				= 1018,		// 체력강화2
    eCHAR_STATE_STAT_INT2				= 1019,		// 정신력강화2
    eCHAR_STATE_STAT_SPI2				= 1020,		// 지력강화2
    eCHAR_STATE_MAGIC_ATTACK_INCREASE2	= 1021,		// 마법공격상승2
    eCHAR_STATE_STAT_DAMAGE_ADD2		= 1022,		// 추가데미지2
    eCHAR_STATE_FIRE_ATTACK_INCREASE2	= 1023,		// 불속성공격력증가2
    eCHAR_STATE_WATER_ATTACK_INCREASE2	= 1024,		// 물속성공격력증가2
    eCHAR_STATE_WIND_ATTACK_INCREASE2	= 1025,		// 바람속성공격력증가2
    eCHAR_STATE_EARTH_ATTACK_INCREASE2	= 1026,		// 대지속성공격력증가2
    eCHAR_STATE_DARK_ATTACK_INCREASE2	= 1027,		// 암흑속성공격력증가2
    eCHAR_STATE_FIRE_DEFENSE_INCREASE2	= 1028,		// 불속성방어력증가2
    eCHAR_STATE_WATER_DEFENSE_INCREASE2	= 1029,		// 물속성방어력증가2
    eCHAR_STATE_WIND_DEFENSE_INCREASE2	= 1030,		// 바람속성방어력증가2
    eCHAR_STATE_EARTH_DEFENSE_INCREASE2	= 1031,		// 대지속성방어력증가2
    eCHAR_STATE_DARK_DEFENSE_INCREASE2	= 1032,		// 암흑속성방어력증가2
    eCHAR_STATE_MP_SPEND_INCREASE2		= 1033,		// MP저주2(MP 소모 증가)
    eCHAR_STATE_MP_SPEND_DECREASE2		= 1034,		// MP활력2(MP 소모 감소)	
    eCHAR_STATE_DECREASE_CASTING_TIME2		= 1035,	//캐스팅 타임 감소2
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME2	= 1036,	//스킬 쿨타임 감소2	
    eCHAR_STATE_TRANSFORMATION2				= 1037, // 변신2
    //eCHAR_STATE_STEEL2						= 1038, // 강철2(이동 외 행동 불능, 이동속도 감소)
    eCHAR_STATE_TRANSPARENT2				= 1039,	// 투명2(이동 외의 행동시 해제)
    eCHAR_STATE_FEAR2						= 1040, // 피어2(대상을 도망가게 한다.)
    eCHAR_STATE_BLUR2						= 1041, // 공격불능2(대상은 공격을 못한다.)
    eCHAR_STATE_THRUST2						= 1042, // 밀리기2(밀리고 있는 상태)
    eCHAR_STATE_SUMMON2						= 1043, // 소환2
    eCHAR_STATE_SPCHARGE2					= 1044, // 검기충전2
    eCHAR_STATE_IGNORE_RESERVEHP2			= 1045,	// 활력(HP적립무시2)
    eCHAR_STATE_ANGER11						= 1046,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE11		= 1047,
    eCHAR_STATE_DEFENSE11					= 1048,
    eCHAR_STATE_MAGIC_DEFENSE10				= 1049,
    eCHAR_STATE_PRECISION10					= 1050,
    //
    eCHAR_STATE_PERK_SWITCHABLE_STATE       = 1051, // 퍽 교체 가능, _NA001956_110210_PERK_SWITCHABLE_REGION
    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ENCOURAGEMENT2      		= 1076,		// Max sd 증가
    eCHAR_STATE_PROMOTION2       			= 1077,		// sd 회복량 증가
    eCHAR_STATE_PLENTY                      = 1078 ,    // 풍요, 하임 획득량 증가
    eCHAR_STATE_SMART                       = 1079 ,    // 총명, 몬스터 경험치 획득량 증가

    eCHAR_STATE_SPEEDING6				= 1200,		// 질주3(이속)
    eCHAR_STATE_HIGH_SPIRIT6			= 1201,		// 기세3(공속)
    eCHAR_STATE_HP_INCREASE6			= 1202,		// 격려(HP증감)
    eCHAR_STATE_TRANSPARENT6			= 1203,		// 투명6(이동 외의 행동시 해제)
    eCHAR_STATE_PRECISION6				= 1204,		// 정밀3(크리티컬 데미지 증가)
    eCHAR_STATE_CONCENTRATION6			= 1205,		// 집중3(크리티컬 확률 증가)
    eCHAR_STATE_ANGER6					= 1206,		// 분노3(추가 공격력 증가)
    eCHAR_STATE_MAGIC_ATTACK_INCREASE6	= 1207,		// 마법공격상승3
    eCHAR_STATE_DEFENSE6				= 1208,		// 물리방어3
    eCHAR_STATE_MAGIC_DEFENSE6			= 1209,		// 마법방어3

    //__NA_000994_CHANGUP_NEW_SKILL
    eCHAR_STATE_IMMUNITY_DAMAGE			= 1301,		// 무적상태( 물리 & 마법 데미지 없음, 공격가능 )
    eCHAR_STATE_INTENSIFY_SKILL			= 1302,		// 스킬데미지가 증가한다.( 스킬데미지 300% )
    eCHAR_STATE_INTENSIFY_SUMMON		= 1303,		// 폭주( 소환수의 공격력이 대폭 증가 )
    eCHAR_STATE_ATTACK_IMPOSSIBLE		= 1304,		// 공격불가( 공격이 불가능 하다 )
    eCHAR_STATE_HP_INCREASE7			= 1305,		// 격려( 최대 HP증가 )
    eCHAR_STATE_BATTLE					= 2001,		// 전투상태. 
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME6	= 7001,	// 스킬 쿨타임 감소3
    eCHAR_STATE_HP_INCREASE8				= 7002,	// 격려
    eCHAR_STATE_ANGER7						= 7003,	// 분노
    eCHAR_STATE_HPRATE_INCREASE6			= 7004,	// 활기
    eCHAR_STATE_DECREASE_CASTING_TIME6		= 7005,	// 정신활력
    eCHAR_STATE_MAGIC_ATTACK_INCREASE7		= 7006,	// 마법공격상승
    eCHAR_STATE_MPRATE_INCREASE6			= 7007,	// 자극
    eCHAR_STATE_CONCENTRATION7				= 7008,	// 집중
    eCHAR_STATE_DEFENSE7					= 7009,	// 방어	
    //eCHAR_STATE_IGNORE_RESERVEHP6			= 7010,	// 활력(HP적립무시6)

    //!_NA_000587_20100928_DOMINATION_BUFF_ITEM
    eCHAR_STATE_HPRATE_INCREASE13           = 7101,	// 활기_성벽	        
    eCHAR_STATE_DEFENSE13                   = 7102,	// 	물리방어_성벽	    
    eCHAR_STATE_MAGICDEFENSE13              = 7103,	// 	마법방어_성벽	    
    eCHAR_STATE_HPRATE_INCREASE14           = 7104,	// 	활기_피의고리	    
    eCHAR_STATE_DEFENSE14                   = 7105,	// 	물리방어_피의고리	    
    eCHAR_STATE_MAGICDEFENSE14              = 7106,	// 	마법방어_피의고리	    
    eCHAR_STATE_HPRATE_INCREASE15           = 7107,	// 	활기_피의결정체	    
    eCHAR_STATE_DEFENSE15                   = 7108,	// 	물리방어_피의결정체	
    eCHAR_STATE_MAGICDEFENSE15              = 7109,	// 	마법방어_피의결정체	

    //----------------------------------------------------------------------------------------------
    //  (WARNING) 아래 상태 이후에 상태 추가해선 안됨! 상태를 이용한 제어 목적으로 사용하고 있음.
    //__NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__
    eCHAR_STATE_SSQ_CTRL_BLOCK_ATTACK   , // No real state|for control|SSQ 상태제어 (공격/피격) 불가능 상태 설정
    eCHAR_STATE_WAR_CTRL_OBSERVER_MODE  , // No real state|for control|관전 만을 위한 상태 설정
    //__NA001390_090915_RIDING_SYSTEM__
    eCHAR_STATE_RIDING_RIDER            , // No real state|for control|Riding Status|
    eCHAR_STATE_NPC_NON_PREEMPTIVE_ATTACK , // 비선공 상태 (점령지역 길드원)   //_NA001385_20090924_DOMINATION_ETC
    // (CHANGES) (f100514.3L) add the flag to prevent ANY actions that
    // a player do a delayed spiking whether the player request a LEAVE.
    eCHAR_STATE_ZONE_TRANSACTION ,
    // CHANGES: f110315.2L, declared by _NA001955_110210_WING_COSTUME
    eCHAR_STATE_SPREAD_WINGS, // whether a player spread winds
    eCHAR_STATE_SPEED_HACK_PREVENTION, //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
};

enum eRECOVER_TYPE
{
    eRECOVER_TYPE_NORMAL		= 0,
    eRECOVER_TYPE_AUTO_HP		= 1,			//자동으로 HP회복
    eRECOVER_TYPE_AUTO_MP		= 2,			//자동으로 MP회복
    eRECOVER_TYPE_AUTO_HPMP		= 3,			//자동으로 HP 및 MP회복
    eRECOVER_TYPE_IGNORE_RESERVE_HP	= 4,		//HP적립 무시하고 HP회복
};

enum eSTATE_TYPE
{
    eSTATE_TYPE_ABNORMAL			= 1,		// 상태이상
    eSTATE_TYPE_WEAKENING			= 2,		// 상태약화
    eSTATE_TYPE_STRENGTHENING		= 3,		// 상태강화
    eSTATE_TYPE_SPECIALITY			= 4,		// 특수
};

#define TRIGGER_ATTRIBUTE_CONDITION_OR_FLAG           0x01
#define TRIGGER_ATTRIBUTE_ACTIVE_FLAG                 0x02
#define TRIGGER_ATTRIBUTE_LOOP_FLAG                   0x04
#define TRIGGER_ATTRIBUTE_ACTIVATE_NEXT_TRIGGER       0x08


enum eSUMMON_COMMAND
{
    eSUMMON_COMMAND_FOLLOW,				// 뒤따라감
    eSUMMON_COMMAND_ATTACK,				// 공격지시
    eSUMMON_COMMAND_DELEGATE_ATTACK,	// 위임(공격형)
    eSUMMON_COMMAND_DELEGATE_DEFENSE,	// 위임(방어형)
    eSUMMON_COMMAND_DESTROY,			// 소멸
    eSUMMON_COMMAND_SKILL_ATTACK,		// 스킬공격
    eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER,	// 위임(공격형)(플레이어 무시) 

    eSUMMON_COMMAND_MAX,
};

//_NA001385_20090924_DOMINATION_SKILL
enum eCRYSTALWARP_COMMAND
{
    eCRYSTALWARP_COMMAND_CREATE,	// 길드 소환체 생성 
    eCRYSTALWARP_COMMAND_DESTROY,	// 길드 소환체 소환해제 
    eCRYSTALWARP_COMMAND_EXECUTE,	// 길드 소환체 실행

    eCRYSTALWARP_COMMAND_MAX,
};

enum eCRYSTALWARP_STATE
{
    eCRYSTALWARP_STATE_CREATE,	    // 길드 소환체 생성 
    eCRYSTALWARP_STATE_DESTROY,	    // 길드 소환체 소환해제 (아군에 의한 파괴)
    eCRYSTALWARP_STATE_DEAD,	    // 길드 소환체 죽음     (적군에 의한 파괴, 소환시간 초과)

    eCRYSTALWARP_STATE_MAX,
};

// 친구 목록에서의 친구 상태
enum eFRIEND_STATUS
{
    eSTATE_ONLINE=1,		// 온라인
    eSTATE_OFFLINE,			// 오프라인(로그아웃 또는 차단 등)
};
enum eNPC_SPECIAL_ACTION_TYPE
{
    eNPC_SPECIAL_ACTION_HELPREQUEST			= 1,	// 지원요청
    eNPC_SPECIAL_ACTION_TRANSFORMATION		= 2,	// 변신
    eNPC_SPECIAL_ACTION_SKILL				= 3,	// 특수스킬 사용
    eNPC_SPECIAL_ACTION_SELP_DESTRUCTION	= 4,	// 자폭	(__NA00XXXX_080922_TASK_SSQ_NPC_SELF_DESTRUCTION__)
    eNPC_SPECIAL_ACTION_MAX
};

//-----------------------------------------------------------------------
// 길드 관련 
//-----------------------------------------------------------------------

// 길드 직책
// ******** 중요 : 여기의 enum 순서가 그대로 DB에 저장된다 따라서 절대로 이 순서를 바꾸지 마라.. ( 바꾸려면 DB를 고려해라. )
#ifdef _NA_003923_20120130_GUILD_RENEWAL
enum eGUILD_DUTY
{
    eGUILD_DUTY_NONE,
    eGUILD_DUTY_MASTER,	                // 길드 마스터
    eGUILD_DUTY_SUBMASTER,	            // 서브 마스터
    eGUILD_DUTY_STRATEGIST,	            // 군사[전략가]     : 길드 리뉴얼 -> X
    eGUILD_DUTY_COMMANDER,	            // 군단장           : 길드 리뉴얼 -> X
    eGUILD_DUTY_LEADER,                 // 근위 대장        : 길드 리뉴얼 -> NEW
    eGUILD_DUTY_CAMP_LEADER,	        // 훈련(캠프)대장   : 길드 리뉴얼 -> X
    eGUILD_DUTY_REGULAR_SOLDIER,	    // 단원(정예군)     : 길드 리뉴얼 -> 일반 병사
    eGUILD_DUTY_CAMP_SOLDIER,	        // 훈련(캠프)병     : 길드 리뉴얼 -> X
    eGUILD_DUTY_MAX,
};
#else
enum eGUILD_DUTY
{
    eGUILD_DUTY_NONE			= 0,
    eGUILD_DUTY_MASTER			= 1,	// 길드 마스터
    eGUILD_DUTY_SUBMASTER		= 2,	// 서브 마스터
    eGUILD_DUTY_STRATEGIST		= 3,	// 군사[전략가]
    eGUILD_DUTY_COMMANDER		= 4,	// 군단장
    eGUILD_DUTY_CAMP_LEADER		= 5,	// 훈련(캠프)대장
    eGUILD_DUTY_REGULAR_SOLDIER = 6,	// 단원(정예군)
    eGUILD_DUTY_CAMP_SOLDIER	= 7,	// 훈련(캠프)병
    eGUILD_DUTY_MAX				= 8,
};
#endif

enum eGUILD_DUTY_BIT
{
    eGUILD_DUTY_BIT_NONE			= eGUILD_DUTY_NONE,
    eGUILD_DUTY_BIT_MASTER			= eGUILD_DUTY_MASTER,			
    eGUILD_DUTY_BIT_SUBMASTER		= eGUILD_DUTY_MASTER<<1,		
    eGUILD_DUTY_BIT_STRATEGIST		= eGUILD_DUTY_MASTER<<2,		
    eGUILD_DUTY_BIT_COMMANDER		= eGUILD_DUTY_MASTER<<3,		
    eGUILD_DUTY_BIT_CAMP_LEADER		= eGUILD_DUTY_MASTER<<4,
    eGUILD_DUTY_BIT_REGULAR_SOLDIER	= eGUILD_DUTY_MASTER<<5,
    eGUILD_DUTY_BIT_CAMP_SOLDIER	= eGUILD_DUTY_MASTER<<6,
    eGUILD_DUTY_BIT_MAX				= eGUILD_DUTY_MASTER<<7,
};

// 길드 클래스
enum eGUILD_CLASS
{
    eGUILD_CLASS_GENERAL,			// 일반 길드
    eGUILD_CLASS_MERCHANT,			// 상인 조합
    eGUILD_CLASS_STOCK,				// 주식 조합
    eGUILD_CLASS_GAIDENCE,			// 가이던스
    eGUILD_CLASS_SEED_GAIDENCE,		// 시드 가이던스
    eGUILD_CLASS_DIVINE_CRUSADER,	// 디바인 크루세이더
    eGUILD_CLASS_ASSASSIN,			// 암살단
    eGUILD_CLASS_REUNION,			// 리유니온
};

// 길드 맴버의 권한
// *** 이 정보도 DB에 저장된다. 따라서 이것을 수정할 때면 스크립트와 DB를 맞춰야 한다. ***
enum eGUILD_RIGHTS
{
    eGUILD_RIGHTS_JOIN				    = 1,							//가입(초대)
    eGUILD_RIGHTS_WITHDRAW			    = 2,							//탈퇴 
    eGUILD_RIGHTS_DESTROY			    = 3,							//해체 
    eGUILD_RIGHTS_KICK				    = 4,							//강퇴 
    eGUILD_RIGHTS_RELATION			    = 5,							//관계
    eGUILD_RIGHTS_RANKUP			    = 6,							//랭크업
    eGUILD_RIGHTS_NOTICE			    = 7,							//길드 공지 
    eGUILD_RIGHTS_CORPS_NOTICE		    = 8,							//군단 공지 
    eGUILD_RIGHTS_RESERVED_NOTICE	    = 9,							//예비군 공지
    eGUILD_RIGHTS_DONATEUP			    = 10,							//UP 적립
    eGUILD_RIGHTS_UP2GP				    = 11,							//UP->GP 변환
    eGUILD_RIGHTS_USE_UPGP			    = 12,							//UP/GP포인트 사용 
    eGUILD_RIGHTS_CHANGE_DUTY		    = 13,							//직책 변경
    eGUILD_RIGHTS_DELEGATION_RIGHT	    = 14,							//권한 위임
    eGUILD_RIGHTS_WAREHOUSE_SAVE	    = 15,							//길드 창고 적립 
    eGUILD_RIGHTS_WAREHOUSE_OUT	        = 16,							//길드 창고 출납

    eGUILD_RIGHTS_WAREHOUSE_LOG		    = 17,							//길드 창고 이용내역 열람
    eGUILD_RIGHTS_GUILD_WAR			    = 18,							//길드전
    eGUILD_RIGHTS_SEIGE_WARFARE		    = 19,							//공성전
    eGUILD_RIGHTS_GUILD_MARK		    = 20,							//길드 마크
    eGUILD_RIGHTS_CREATE_CORPS		    = 21,							//군단창설
    eGUILD_RIGHTS_DISMISS_CORPS		    = 22,							//군단해산
    eGUILD_RIGHTS_CREATE_CAMP		    = 23,							//길드 육성캠프 창설 
    eGUILD_RIGHTS_DISMISS_CAMP		    = 24,							//길드 육성캠프 해산
    eGUILD_RIGHTS_ITEM_BUY			    = 25,							//길드 아이템의 구입 (개인용외)
    eGUILD_RIGHTS_ITEM_EQUIP		    = 26,							//길드 아이템의 장비(개인용제외)	
    eGUILD_RIGHTS_STYLE_ACQUIRE		    = 27,							//길드스타일 획득
    eGUILD_RIGHTS_STYLE_LEVEL_UP	    = 28,							//길드스타일 레벨업 
    eGUILD_RIGHTS_GUILD_STYLE_USE	    = 29,							//길드스타일 사용 
    eGUILD_RIGHTS_CORPS_STYLE_USE	    = 30,							//군단스타일 사용 
    eGUILD_RIGHTS_CLASS_CHANGE		    = 31,							//길드클래스 체인지
    eGUILD_RIGHTS_CONQUEST_JOIN		    = 32,							//길드 컨퀘스트 참여 
    eGUILD_RIGHTS_CHANGE_MASTER		    = 33,							//길드장 위임(변경)
    eGUILD_RIGHTS_VIEW_RIGHT		    = 34,							//길드원 권한 보기
    eGUILD_RIGHTS_CHANGE_CORPS		    = 35,							//군단간 이동
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    eGUILD_RIGHTS_GUILD_EQUIPMENT       = 36,                           //길드 시설 레벨업
    eGUILD_RIGHTS_ACTIVATION_EQUIPMENT  = 37,                           //길드 시설 활성화
#endif
    eGUILD_RIGHTS_MAX,
};

// 길드 공지 분류
enum eGUILD_NOTICE
{
    eGUILD_NOTICE_GUILD = 1,		// 길드 공지
    eGUILD_NOTICE_CORPS,			// 군단 공지
    eGUILD_NOTICE_CAMP,				// 예비군 공지
};

// 길드 관계
enum eGUILD_RELATION
{
    eGUILD_RELATION_NONE,				    // 초기화
    eGUILD_RELATION_ALLIENCE,			    // 동맹
    eGUILD_RELATION_BOTH_HOSTILITY,		    // 쌍방적대
    eGUILD_RELATION_PROCLAIM_HOSTILITY,	    // 적대 선포
    eGUILD_RELATION_ONE_SIDE_HOSTILITY,     // 일방적 적대
    eGUILD_RELATION_CANCEL_BOTH_HOSTILITY,  // 쌍방 적대 취소
};

enum ePROCLAIM_GUILD
{
    NONE_PROCLAIM_GUILD,
    PROCLAIM_GUILD,  //적대 선포한 길드
    ONESIDE_GUILD,  // 적대 선포 당한 길드
};

//군단 타입
enum eGUILD_CORPS_TYPE
{
    eGUILD_GUILD,
    eGUILD_CORPS,
    eGUILD_CAMP,
};

// 길드 대답
enum eGUILD_ANSWER
{
    eGUILD_ANSWER_YES = 1,
    eGUILD_ANSWER_NO,
};

//길드 페널티 타입
enum eGUILD_PENALTY
{
    eGUILD_WITHDRAW = 1,
    eGUILD_KICK,
    eGUILD_DESTROY,
};

//길드 DelChk
enum eGUILD_DELCHK
{
    eGUILD_DELCHK_NORMAL = 0,
    eGUILD_DELCHK_DESTROY,
    eGUILD_DELCHK_DESTROY_WAIT,
};

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

enum eGUILD_JOIN_REQUEST
{
    eGUILD_JOIN_REQUEST_OK = 0, //가입요청 버튼상태
    eGUILD_JOIN_REQUEST_CANCEL, //가입취소 버튼상태
};

enum eGUILD_JOIN_REJECT_TYPE
{
    eGUILD_JOIN_REJECT_TYPE_NORMAL = 0,  //가입 거절에 의한 삭제
    eGUILD_JOIN_REJECT_TYPE_TIMEOUT,     //대기 시간 초과에 의한 삭제
    eGUILD_JOIN_REJECT_TYPE_FAILED,      //다른 길드에 가입 또는 길드 생성에 의한 삭제
};

enum FACILITY_UPGRADE_ITEM_TYPE
{
    FUI_ONLY_GUILDPOINT = 0,             // 길드 시설을 길드 코인으로만 업그레이드
    FUI_ONLY_ITEM,                       // 길드 시설을 아이템으로만 업그레이드
    FUI_GUILDPOINTANDITEM,               // 길드 시설을 길드 코인과 아이템으로 업그레이드
    FUI_MAX,
};
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

enum eJION_TOP_STATE
{
    eJION_TOP_STATE_NONE		= -1,
    eJION_TOP_STATE_ADMISSION,				//	입장허가 상태
    eJION_TOP_STATE_NONADMISSION,			//	입장불허 상태
};

// _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
enum JoinBattleGroundState
{
    kNone = -1,
    kJoinStateAdmission,
    kJoinStateNonadmission,
};

enum eMAPOBJECT_CUSTOMATTRIBUTE
{
    eMAPOBJECT_TYPE = 0,			//param1의 인덱스 // 값이 3이면 웨이포인트.
    eMAPOBJECT_MONSTER = 1,         //param2의 인덱스 // 설치형 몬스터 코드(NPCCode)
    eMAPOBJECT_USE_ALPHA = 2,		//param3의 인덱스 // 알파 블랜딩 사용 여부
    eMAPOBJECT_LINKED_MONSTER_REGEN_TYPE = 3, //param4의 인덱스 
                                              // 2번 : 연결된 몬스터가 있을경우 미리 리젠 하지 않는다.
                                              // 트리거를 통한 리젠 (REGEN_TYPE_TRIGGER) 외에는 지원하지 않는다.

    eMAPOBJECT_MAX = 4,				//< 4개여야 한다.
};

// GM 권한등급 정의
enum eGM_DB_GRADE
{
    eGM_GRADE_BASE		= 100,			//< 얘는 GM은 아님(계산용)
    eGM_GRADE_FIRST,					//< 기본 GM권한(나머지 권한들은 WOPS 참조)
    eGM_GRADE_SECOND,					//< 2단계
    eGM_GRADE_THIRD,					//< 3단계
    eGM_GRADE_MAX,						//< 얘도 GM은 아님(비교용)
    eGM_GRADE_VIRTUAL	= 200,			//< 가상 계정 : sts=200 값 넘어옴
};

// 카오 상태
enum eCHAO_STATE
{
    eCHAO_STATE_NORMAL,			// 일반
    eCHAO_STATE_PREV_CHAO,		// 프리 카오
    eCHAO_STATE_CHAO,			// 카오
};

// 파티탈퇴 이유
enum eREASON_LEAVE_PARTY
{
    eREASON_LEAVE_PARTY_INTEND = 0,         // 정상적으로 의도해서 나갔을때
    eREASON_LEAVE_PARTY_BAN,                // 강퇴 당했을때
    eREASON_LEAVE_PARTY_ABNORMAL,           // 비정상 종료
    eREASON_LEAVE_PARTY_NOREASON,	        // 이유없음(서버에서 존이동시 파티 재설정을 위해 파티탈퇴)
    eREASON_LEAVE_PARTY_BY_GUILDRELATION,   // 파티 멤버들과 적대 관계를 맺은 길드 멤버는 강제 탈퇴
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    eREASON_LEAVE_PARTY_NOT_MATCH_LEVEL,    // 파티룸 미션 레벨 조건에 만족하지 못한 상태
    eREASON_LEAVE_PARTY_NOT_CLEAR_MISSION,  // 이전 미션을 클리어 하지 못한 상태
    eREASON_LEAVE_PARTY_NOT_CLEAR_QUEST,    // 이전 퀘스트를 클리어하지 못한 상태
};

enum ePORTAL_TYPE
{ 
    ePORTAL_TYPE_GM,	
    ePORTAL_TYPE_REVENGE, 
    ePORTAL_TYPE_ITEM,
    ePORTAL_TYPE_ITEM_EX,	//웨이포인트리스트 아이템
    ePORTAL_TYPE_FIRENDINVITATED_EVENT,
};

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
enum eROOM_SPECIAL_TYPE
{
    eROOM_SPECIAL_NORMAL						= (1<<0),
    eROOM_SPECIAL_SUPER							= (1<<1),
    eROOM_SPECIAL_PCBANG						= (1<<2),
    eROOM_SPECIAL_ITEM							= (1<<3),

    //SUPER_BANG
    eROOM_SPECIAL_PCBANG_SUPER_ROOM				= ( eROOM_SPECIAL_PCBANG | eROOM_SPECIAL_SUPER ),
    eROOM_SPECIAL_ITEM1_SUPER_ROOM				= ( eROOM_SPECIAL_ITEM | eROOM_SPECIAL_SUPER ),

    //GUILD_PVP
    eROOM_SPECIAL_GUILD_PVP						= (1<<4),
    eROOM_SPECIAL_PCBANG_SUPER_ROOM_GUILD_PVP	= ( eROOM_SPECIAL_PCBANG_SUPER_ROOM | eROOM_SPECIAL_GUILD_PVP ),
    eROOM_SPECIAL_ITEM1_SUPER_ROOM_GUILD_PVP	= ( eROOM_SPECIAL_ITEM1_SUPER_ROOM | eROOM_SPECIAL_GUILD_PVP ),
};
#else
enum eROOM_SPECIAL_TYPE
{
    eROOM_SPECIAL_NORMAL,
    eROOM_SPECIAL_PCBANG_SUPER_ROOM,	// PC방 슈퍼방
    eROOM_SPECIAL_ITEM1_SUPER_ROOM,		// 슈퍼방장1 아이템으로 만든 슈퍼방
};
#endif

//---------------------------------------------
//상점 리스트 NPC 이용시
//---------------------------------------------
enum eVENDOR_LIST_PAGE_KIND
{
    PAGE_REQUIRE_KIND_NONE,							//초기값
    PAGE_REQUIRE_KIND_ALL,							//클라이언트가 Page 요청시 그 종류 - 전체
    PAGE_REQUIRE_KIND_TITLE,						//검색도 이 Define 사용			   - 상점이름
    PAGE_REQUIRE_KIND_ID,							//검색도 이 Define 사용			   - ID
    PAGE_REQUIRE_KIND_ITEM,							//검색도 이 Define 사용			   - 아이템 이름
};

enum eREWARD_FLAG
{
    REWARD_EXP		= (1<<0),
    REWARD_HIME		= (1<<1),
    REWARD_CHAOTIME = (1<<2),
    REWARD_SKILL	= (1<<3),
    REWARD_ITEM		= (1<<4),
    REWARD_ALL		= (1<<5)-1,
};

//인스턴스 종류
enum eINSTANCE_DUNGEON_KIND
{	// sMAPINFO->byMType 으로 확장됨
    //	__NA001226_INSTANCE_DUNGEON_EXTENTION__에 의해 추가되는 타입들...
    //	세부 제어는 MapInfoParser.IndunRule참조

    //////////////////////////////////////////////////////////////////////////
    //		(+)MissionRanking (point)
    //		(-)RankingTimer
    INDUN_KIND_TOP_OF_TRIAL		=  0,		// 칼버스 (저주의 탑)
    //////////////////////////////////////////////////////////////////////////
    //		(-)MissionRanking
    //		(-)RankingTimer
    //		(+)change-up, class change... (혹 200레벨 체인지업이 존재시, 룰이 많이 다르면, 별도로 나가는 편이 나을 듯.)
    INDUN_KIND_100LEVEL_QUEST	=  1,		// 100레벨 퀘스트 이벤트 맵	__NA000844_080324_100LEVEL_CHAGNEUP_REBUILDING__
    //////////////////////////////////////////////////////////////////////////
    //		(+)MissionRanking <- special
    //		(-)RankingTimer
    //		(+)static dungeon control (임의의 시점에 종류별 단 1개만 존재하는 인던)
    //		(+)use portal enter method
    //		(+)variation logic control rules
    INDUN_KIND_SS_QUEST			=  2,		// 타락한 사원	__NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__
    //////////////////////////////////////////////////////////////////////////
    //		(-)MissionRanking
    //		(-)RankingTimer
    //		(+)can't equip weapon
    //		(+)can't enter equiped weapon
    //		(+)can't auto targeting by 'tab' key
    INDUN_KIND_EXT_03			=  3,
    //////////////////////////////////////////////////////////////////////////
    //		(+)MissionRanking (time-attack)
    //		(+)RankingTimer
    //		(+)need DB modification in server about ranking control
    INDUN_KIND_EXT_04			=  4,
    //////////////////////////////////////////////////////////////////////////
    //		(-)MissionRanking
    //		(-)RankingTimer
    //		(+)NotShadow
    INDUN_KIND_MAYA_SCROLL		=  5,		//(lst1024)중국 쉐도우 입장 불가 맵( 경험치 획득맵 )
    //////////////////////////////////////////////////////////////////////////
    
    INDUN_KIND_CHAOS_ZONE   	=  6,

    INDUN_KIND_MAX				,
};

struct eZONE_RULESET
{
    enum eRules
    {
        ZONE_RULE_EMPTY								= 0,		// 초기값, 또는 인던이 아닐 경우 값
        //////////////////////////////////////////////////////////////////////////
        //	reserved - 다른 존에 대한 세부 컨트롤이 요구될 경우 Mkind에 의해 분리되고, bMtype에 의한 rule이 변경된다?
        //	ex:
        //		ZONE_RULE_MISSION_RANKING_POINT		= 1<< 0,
        //////////////////////////////////////////////////////////////////////////
        //	for instance dungeon control
        ZONE_RULE_INDUN_SHOW_THE_FLOOR				= 1<< 0,	// 층 개념 처리 (저탑 방식)
        ZONE_RULE_OPTION_ROOM_BY_PORTAL             = 1<< 1,    // Portal입장시 가변 옵션, ex) Portal Mission에서 랭킹 미동작 처리
#ifdef _NA_008012_20150130_RANKING_SYSTEM
        ZONE_RULE_INDUN_RANKING_POINT				= 1<< 5,	// 미션랭킹 적용, 포인트방식
        ZONE_RULE_INDUN_RANKING_TIME_ATTACK			= 1<< 6,	// 미션랭킹 적용, 타임어택
        ZONE_RULE_INDUN_RANKING_TIMER				= 1<< 7,	// 미션랭킹 적용, 클라이어트 타이머 보이기
#endif // _NA_008012_20150130_RANKING_SYSTEM
        ZONE_RULE_INDUN_QUEST						= 1<<10,	// 퀘스트를 위한 인던, (체인지업)
        ZONE_RULE_INDUN_CANT_EQUIP_WEAPON_AND_BLOCK	= 1<<15,	// 특수 처리, 입장전/후 공격무기 장착 불가 모드
        ZONE_RULE_INDUN_SINGLE_PLAYING_MODE			= 1<<16,	// 한명만 입장 가능
        ZONE_RULE_INDUN_SHADOW_NOT_PLAYING_MODE		= 1<<17,	// 한명만 입장 불가(쉐도우)	//(lst1024) 중국 쉐도우 입장 불가 맵
        ZONE_RULE_INDUN_STATIC_DUNGEON				= 1<<30,	// 임의 시간에 고정적으로 하나만 존재하는 특수 인던
        //////////////////////////////////////////////////////////////////////////
        //	MAX_BIT_SET							= 1<<31,
    };

    eRules						rules;
};

enum eSCRIPT_CODE
{
    // 스크립트 서버/ 클라 대역폭..
    // 10-199 번까지 : 서버 및 서버클라 공용스크립트
    // 200-299 번까지 : 클라전용 스크립트
    // 반드시 값을 정해서 적어 주십시요. 해외버전 솔라오스와 적용시 해당 값으로 스크립트를 로딩하게 됩니다.
    // 추가로 번호를 확장해야 하는 경우 SCRIPT_LIST_SIZE 값을 늘려 주십시요.

    SCRIPT_EMPTY						=	0,					// Control Flag
    SCRIPT_SCRIPT_ALL					=	1,					// Control Flag
    SCRIPT_NOTIFY_SIGNAL				=	2,					// Control Flag
    SCRIPT_CONTROL_FLAG_UPPERBOUNDARY	=	10,					// Control Flag
    SCRIPT_LIST_SIZE					=	300,				// COUNT Flag

//////////////////////////////////////////////////////////////////////////
//	서버및 서버클라 공용 스크립트
    SCRIPT_ACCESSORYITEMINFO			=	10	,
    SCRIPT_ACSCRIPT						=	11	,
    SCRIPT_AICOMMONINFO					=	12	,
    SCRIPT_AITYPEINFO					=	13	,
    SCRIPT_ARMORITEMINFO				=	14	,
    SCRIPT_ARMORTYPEDAMAGEINFO			=	15	,
    //SCRIPT_COMPOSITE_NPC				=	16	,
    SCRIPT_DEVCOMMAND					=	19	, // NOTE: reserved not used after EP2
    SCRIPT_DROPRATIOINFO				=	20	,
    SCRIPT_ENC1							=	21	,	
    SCRIPT_ENC2							=	22	,
    SCRIPT_ENCHANT						=	23	,
    SCRIPT_EXPVALUEINFO					=	24	,
    SCRIPT_EXTRASTONEOPTIONINFO			=	25	,
    SCRIPT_FATIGUEOPTION				=	26	,
    SCRIPT_FIELD						=	27	,
    SCRIPT_FORMULARATIO					=	29	,
    SCRIPT_GMCOMMAND					=	30	, // NOTE: reserved not used after EP2
    SCRIPT_GROUP						=	32	,
    SCRIPT_INITCHARINFO					=	33	,
    SCRIPT_INITCHARINFOLIST				=	34	,
    SCRIPT_ITEMCOMPOSELIST				=	35	,
    SCRIPT_ITEMCOMPOSITEINFO			=	36	,
    //SCRIPT_ITEMCOMPOSITERESULTINFO		=	37	,
    SCRIPT_ITEMCRYSTALIZELIST			=	38	,
    SCRIPT_ITEMDROPINFO					=	39	,
    SCRIPT_ITEMTYPEDROPRATIO			=	41	,
    SCRIPT_ITEMTYPELIST					=	42	,
    SCRIPT_MAPENTERANCEPORTAL			=	44	,
    SCRIPT_MATCHINFO					=	45	,
    SCRIPT_MISSIONREWARDINFO			=	46	,
    SCRIPT_NPCEXTRAINFO					=	48	,
    SCRIPT_NPCINFO						=	49	,
    SCRIPT_PKSCRIPT						=	50	,
    SCRIPT_QUESTINFO					=	51	,
    SCRIPT_RANKOPTIONINFO				=	52	,
    //SCRIPT_RAREITEMDROPINFO				=	53	,
    SCRIPT_REGEN						=	54	,
    SCRIPT_REGEN_POS					=	55	,
    SCRIPT_REQUIREFIELDLEVELINFO		=	56	,
    SCRIPT_REWARDINFOLIST				=	57	,
    SCRIPT_SHOPINFO						=	58	,
    SCRIPT_SHOUTRANGE					=	59	,
    SCRIPT_SKILLINFO					=	60	,
    SCRIPT_SOCKETOPTIONINFO				=	61	,
    SCRIPT_STATEINFO					=	62	,
    SCRIPT_STYLEINFO					=	63	,
    SCRIPT_STYLEQUICKREGISTINFO			=	64	,
    SCRIPT_SUNEVENTINFO					=	66	,
    SCRIPT_VSHUNTINGREWARDINFO			=	71	,
    SCRIPT_WASTEITEMINFO				=	72	,
    SCRIPT_WEAPONITEMINFO				=	73	,
    SCRIPT_WORLD						=	74	,
    SCRIPT_FIELDWMO						=	75	,
    SCRIPT_FIELDWUT						=	76	,
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    SCRIPT_MISSIONRANKINGREWARD			=	77	,
    SCRIPT_MISSIONRANKINGLVAREA			=	78	,
#endif // _NA_008012_20150130_RANKING_SYSTEM
    SCRIPT_CHARGEITEMINFO				=	79	,
    SCRIPT_MONSTERSUMMON				=	80	,
    SCRIPT_MAP_PARTIAL_LOADING			=	81	,
    SCRIPT_FILTER						=	82	,
    SCRIPT_FILTERNAME					=	83	,
    SCRIPT_WORLD_SUB_INFO				=   86  ,
    SCRIPT_CURSE_TOWER_TIME				=	87  ,
#ifdef _NA_008012_20150130_RANKING_SYSTEM
    SCRIPT_POINT_RANKING				=	88  ,
#endif // _NA_008012_20150130_RANKING_SYSTEM
    SCRIPT_EXCHANGE_REWARD_INFO			=	89, //< [일본/대만] __NA_001027_20080423_RANDOM_REWARD_EVENT_
    SCRIPT_GUILD_RANK_INFO				=	90  ,
    SCRIPT_SERVER_OPTION				=	91  ,
    SCRIPT_GAME_OPTION					=   92	,
    SCRIPT_SETITEM_OPTION_INFO			=	93	,
    SCRIPT_GUILD_RIGHT_INFO				=	94	,

    SCRIPT_GAMBLE_OPTION				=	95 ,
    SCRIPT_LIMITED_ITEM_INFO			=	96 ,

    SCRIPT_FATE_ITEM_INFO				=	97 ,
    SCRIPT_EVENTREWARD_INFO				=	98 ,
    SCRIPT_MAX_GUILD_MEMBER_INFO		=	99 ,

    SCRIPT_SSQINFO						=  100 ,	//< __NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__
    SCRIPT_SSQPATTERN					=  101 ,	//< __NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__
    SCRIPT_ABUSING_OPTION				=  103 ,    //< __NA_0_20080219_DETECT_ABUSING_USER_REFACTORING
    SCRIPT_PET_INFO						=  104 ,	//< __NA_0_PET_SYSTEM
    SCRIPT_ETHER_BULLET_INFO			=  105 ,	//< __NA_000968_ETHER_EMISSION_DEVICE
    SCRIPT_LOTTO_INFO					=  106,		//< __NA_000946_20080415_LOTTERY_SYSTEM
    SCRIPT_NEW_CHARACTER_ITEM_INFO		=  107 ,
    SCRIPT_STRENGTH_PENALTY             =  108, //< __NA_001062_20080623_STRENGTH_PENALTY
    SCRIPT_ABUSING_OPTION_TXT			=  109,		//< SolarAuth 버전 관리 목적	//__NA000000_ENABLE_MASTER_SERVER_SCRIPT_LOADING__
    SCRIPT_COLLECT                      =  110, //< __NA001283_20090221_COLLECTION_PARSER
    SCRIPT_GROUPINFO                    =  111, //그룹 맺기 시스템 기본 정보 스크립트__KR_001355_20090608_GROUPINGSYSTEM_DATA
    SCRIPT_CHAOSZONE_INFO               =  112, //< 카오스존 정보, _NA_008334_20150608_SONNENSCHEIN 
    SCRIPT_RIDING_INFO                  =  113, //< __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    SCRIPT_DOMINATIONINFO               =  114, //< _KR_001385_20090907_DOMINATION_PARSER
    SCRIPT_DOMINATION_EVENT_INFO        =  115,
    SCRIPT_ACCUMULATE_POINT             =  116, //< _NA_1419_20100121_ACCUMULATE_POINT
    SCRIPT_PERKINFO                     =  117, //< _NA002217_100728_EP2_PERK_SYSTEM_
    SCRIPT_CUBE_COMPOSE                 =  118, // 큐브 합성 스크립트 _NA_002253_20100811_CUBE_SYSTEM
    SCRIPT_CUBE_DECOMPOSE               =  119, // 큐브 분해 스크립트 _NA_002253_20100811_CUBE_SYSTEM
    SCRIPT_SOCKET_ITEM_RATIO_INFO       =  120, //< _NA_0_20100727_SOCKET_SYSTEM_PARSER
    SCRIPT_ZARD_COMPOSE_INFO            =  121, //< _NA_0_20100727_SOCKET_SYSTEM_PARSER
    SCRIPT_SKILL_INVEN_STORE            =  122, //< _NA_0_20100901_SKILLTREE_SYSTEM (renumbering)
    SCRIPT_ACHIEVEMENT_INFO             =  123, //<-- C_NA_0_20100520_ACSYSTEM_CHANGED
    SCRIPT_CASHCATEGORY                 =  124, //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    SCRIPT_GMRIGHTS                     =  125, //_NA000000_GM_COMMAND_CHANGE_CONCEPT_
    SCRIPT_ZARD_OPTION_INFO             =  126, // 포인트 방식을 지원하는 쟈드 옵션 스크립트 _NA_001699_20110104_SOCKET_OPTION_POINT
    SCRIPT_ZARD_POINT_INFO              =  127, // 쟈드 포인트 정보 스크립트 _NA_001699_20110104_SOCKET_OPTION_POINT
    SCRIPT_MAX_CHAR_LIST                =  128, // 캐릭터 최대 목록 설정 스크립트 _NA_002050_20110216_ADD_GENDER
    SCRIPT_BATTLEZONE_TREE              =  129, // 배틀존 리뉴얼 트리 스크립트 _NA_20110630_BATTLE_ZONE_RENEWAL
    SCRIPT_FAME_LIST                    =  130, // 명예시스템 명성 스크립트 _NA_003027_20111013_HONOR_SYSTEM
    SCRIPT_REPUTE_LIST                  =  131, // 명예시스템 평판 스크립트 _NA_003027_20111013_HONOR_SYSTEM
    SCRIPT_TITLE_LIST                   =  132, // 명예시스템 칭호 스크립트 _NA_003027_20111013_HONOR_SYSTEM
    SCRIPT_ITEM_SCORE_COMMON            =  133, // 장비 평가 점수 스크립트 _NA_002935_20110704_ITEM_SCORE
    SCRIPT_NPCREPLACE_INFO              =  134, // _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    SCRIPT_RANDOM_ITEM_INFO             =  135, // 랜덤 아이템 정보 스크립트 _NA_003740_20111122_RANDOM_ITEM
    SCRIPT_RANDOM_ITEM_VALUE            =  136, // 랜덤 아이템 값 스크립트 _NA_003740_20111122_RANDOM_ITEM
    SCRIPT_RANDOM_ITEM_OPTION_TYPE      =  137, // 랜덤 아이템 옵션 종류 스크립트 _NA_003740_20111122_RANDOM_ITEM
    SCRIPT_RANDOM_ITEM_KEY              =  138, // 랜덤 아이템 키 스크립트 _NA_003740_20111122_RANDOM_ITEM
    SCRIPT_CURRENCY_INFO                =  139, // 화폐 포인트 스크립트 _NA004034_20120102_POINT_WALLET_SYSTEM
    SCRIPT_GUILDINFO                    =  140, // 길드 기본 정보 _NA_003923_20120130_GUILD_RENEWAL
    SCRIPT_RESOURCE_VERSION             =  141, // 리소스버전 체크 스크립트 _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
    SCRIPT_CHAOSZONE_TREE               =  142, // 카오스존 트리 스크립트 _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND (1504부터 클라전용 스크립트)
    SCRIPT_BATTLE_GROUND_INFO           =  143, // 전장 스크립트 _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SCRIPT_BATTLE_GROUND_FLAG_INFO      =  144, // 전장 스크립트 _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SCRIPT_BATTLE_GROUND_TIME_INFO      =  145, // 전장 스크립트 _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    SCRIPT_GOLDRUSH_INFO                =  146, // 골드러시 스크립트 _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    SCRIPT_GOLDRUSH_MONSTERPOINT        =  147,  // 골드러시 스크립트 _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    SCRIPT_MONSTER_BOOK                 =  148, // 몬스터도감 스크립트 _NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SCRIPT_GUILD_FACILITY_INFO          =  150, // 길드시설 //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    SCRIPT_ATTENDANCE                   =  160, // 출석 스크립트 _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SCRIPT_SUN_RANKING                  =  161, // 썬 랭킹 스크립트 _NA_008012_20150204_SUN_RANKING_SYSTEM
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#if defined(_NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM) || defined(_NA_008124_20150313_AWAKENING_SYSTEM)
	SCRIPT_AWAKENING_INFO               =  162, // 각성 스크립트 _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    SCRIPT_NOTIFICATION_INFO            =  163, // 알림 시스템 스크립트 _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    SCRIPT_SONNENSCHEIN_REWARDINFO      = 164,  // 소넨샤인 보상정보
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    SCRIPT_MINIGAME                     =  165, // 미니 게임 시스템 스크립트 _NA_008404_20150712_MINIGAME_SYSTEM
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	SCRIPT_ONEDAYSBUILDMISSION          =  166, // 일일 길드미션 스크립트 _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#endif // _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    //----------------------------------------------------------------------------------------------
    SCRIPT_TEST							=  199, //__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
    //----------------------------------------------------------------------------------------------

//==================================================================================================
//	클라이언트 전용 스크립트
    SCRIPT_ATTACHMENT_OFFSET_INFO		=	200  ,
    SCRIPT_BGM_SOUND_INFO				=	201  ,
    SCRIPT_CHAR_SOUND_INFO				=	202  ,
    SCRIPT_COMPOSITE_NPC_INFO			=	203  ,
    SCRIPT_DLG_POS_INFO					=	204  ,
    SCRIPT_ENVIRONMENT_INFO				=	205  ,
    SCRIPT_EVENT_INFO					=	206  ,
    SCRIPT_HEAD_COLOR_INFO				=	207  ,
    SCRIPT_HEAD_MESH_INFO				=	208 ,
    SCRIPT_HELP_INFO					=	209 ,
    SCRIPT_HELP_MESSAGE					=	210 ,
    SCRIPT_HERO_HEIGHT_INFO				=	211 ,
    SCRIPT_HERO_VARIATION_INFO			=	212	,
    SCRIPT_HERO_VOICE_INFO				=	213	,
    SCRIPT_IGC_INFO						=	214	,
    SCRIPT_ITEM_SOUND_INFO				=	215	,
    SCRIPT_KEY_INFO						=	216	,
    SCRIPT_LIMITED_MAP_PORT				=	217	,
    SCRIPT_MONSTER_VOICE_INFO			=	218	,
    SCRIPT_NPC_VOICE_INFO				=	219	,
    SCRIPT_OPTION_LIST_INFO				=	220	,
    SCRIPT_PROJECTILE_INFO				=	221	,
    SCRIPT_QUEST_TEXT_INFO				=	222	,
    SCRIPT_RADAR_INFO					=	223	,
    SCRIPT_SKILL_EFFECT_INFO			=	224	,
    SCRIPT_SKILL_TEXT_INFO				=	225	,
    SCRIPT_WEAPON_SOUND_INFO			=	226	,
    SCRIPt_GRAPHIC_RESOURCE_LIST        =   227 ,  
    SCRIPT_______empty_slot________     =   228 , // CHANGES: renumber to 122 'SCRIPT_SKILL_INVEN_STORE'
    SCRIPT_AREA_CONQUEST_INFO           =   229 ,
    SCRIPT_MESSAGE                      =   230 ,
    SCRIPT_QUESTINFO_MANAGER            =   231 , 
    SCRIPT_GAME_HELP_SYSTEM_INFO		=	234 ,
    SCRIPT_GUILD_MARK					=	235 ,
    SCRIPT_ETHER_WEAPON_EFFECT			=	236,    //< __NA_000968_ETHER_EMISSION_DEVICE
    SCRIPT_NPC_IMAGE_INFO				=	237,    //< __NA_001003_PC_MONSTER
    SCRIPT_HEROVARIATION_INFO			=	238,	//< __NA_001044_ADD_CUSTOM_UI
    SCRIPT_STRENGTH_UI					=   239,    //< __NA_001062_20080623_STRENGTH_PENALTY
    SCRIPT_CONSTANT_DIALOG				=	240,    //< __LTJ_UI_DIALOG_TEXTURE_LOAD_N_RELEASE_MOD
    SCRIPT_INDUN_OPTION					=   241,	//< _DH_INDUNOPTION_PARSER
    SCRIPT_SPECIAL_TEXTURE			    =	242,    //< __GS_SPECIAL_TEXTURE
    SCRIPT_AMBIENT_SOUND_INFO           =	243,    //< _100114_LTJ_USE_AMBIENTSOUND_SCRIPT_FOR_AMBIENT_SOUND

    SCRIPT_AUTOMATIC_INFO				=	245,	//!< auto game play
    SCRIPT_AUTOMATIC_COMBO_INFO			=	246,	//!< auto game play
#ifdef _DH_DISPLAY_FOR_MISSION_OBJECT
    SCRIPT_DISPLAY_FOR_MISSION_OBJECT	=	247,	//< _DH_DISPLAY_FOR_MISSION_OBJECT
    SCRIPT_MISSION_TEXT_INFO			=   248,	//SCRIPT_QUEST_TEXT_INFO
#endif//_DH_DISPLAY_FOR_MISSION_OBJECT
#ifdef _DH_DIVIDE_MESSAGE_TEXT
    SCRIPT_MESSAGE_TEXT                 =   249 ,
#endif//_DH_DIVIDE_MESSAGE_TEXT

    SCRIPT_TEXTURE_LIST_INFO			=	250,	//!< 텍스쳐 애니메이션
    SCRIPT_TEXTURE_ANIMATION_INFO	    =	251,	//!< 텍스쳐 애니메이션

    SCRIPT_CREATECHAR_CUSTOMCAMERA_INFO	=	252,	//!< 캐릭터 생성화면 카메라 설정값
    SCRIPT_LOADINGTEXTURE_INFO	        =	253,	//!< 로딩 이미지 리스트
    SCRIPT_CHARACTER_CREATION_INFO      =   254,    //!< 성별추가/캐릭터 생성정보
    SCRIPT_COLOR_INFO                   =   255,    //!< 색상 정보 

    SCRIPT_TREE_CONTROL                 =   256,    //!< 트리컨트롤 정보
    SCRIPT_TREE_CONTROL_ICON            =   257,    //!< 트리컨트롤 정보 아이콘 이미지
    
    SCRIPT_TOTAL_GUIDE                  =   258, // 통합 사용자 가이드 스크립트 _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_PAGE             =   259, // 통합 사용자 가이드 세부 페이지 스크립트 _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_NOTICE           =   260, // 통합 사용자 가이드 알림창 스크립트 _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_MESSAGE          =   261, // 통합 사용자 가이드 메세지 스크립트 _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_IMAGE            =   262, // 통합 사용자 가이드 메세지 스크립트 _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_TREE_CONTROL     =   263, // 통합 사용자 가이드 트리컨트롤 정보
    SCRIPT_HOT_KEY_INFO                 =   264, // 단축키 기본세팅 정보

    SCRIPT_SKILLEVENTEFFECT_INFO        =   265, // 스킬 이벤트 이펙트 정보 (클라용)	
	SCRIPT_MISSIONHELP_INFO				=   266, // 미션 도움말 정보
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
	SCRIPT_COSTUMEITEM_INFO				=	267, // 코스튬 아이템 정보 (클라용)
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION
	SCRIPT_ENDURANCE_INFO				=   268,// 장비 내구도 관련 UI 스크립트

    SCRIPT_OPTION_CATEGORY_INFO         =   269, // 옵션창의 항목정보
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SCRIPT_GUILD_FACILITY_IMAGE_INFO    =   270,  // 길드 시설 이미지 정보
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SCRIPT_MONSTER_BOOK_DROP_ORDER_INFO =   271, // 몬스터도감 스크립트 _NA_007086_20140318_MONSTERBOOK
    SCRIPT_MONSTER_BOOK_DROP_ITEM       =   272, // 몬스터도감 스크립트 _NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	SCRIPT_STATUE_INFO                  =   273, // 동상 정보
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    SCRIPT_SPA_ZONE_INFO                =   274, // 온천 스타일 정보
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SCRIPT_GUILDMISSIONTREE             =   275, // 길드미션 트리정보
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//////////////////////////////////////////////////////////////////////////

};		

enum eVERSION_TYPE
{
    VERSION_NONE,
    VERSION_SCRIPT,
    VERSION_BUILD,
    VERSION_PROTOCOL,
    VERSION_COMPILE,
    VERSION_ENGINE,
    VERSION_MAX
};

enum eINTERCEPTION_TYPE
{
    eINTERCEPTION_TRADE,		//거래 차단
    eINTERCEPTION_WHISPER,	//귓말 차단
};

enum eITEM_WAA_TYPE
{
    eITEM_WAA_TYPE_INVALID		=	0,
    eITEM_WAA_TYPE_WEAPON		=	1,
    eITEM_WAA_TYPE_ARMOR		=	2,
    eITEM_WAA_TYPE_ACCESSORY	=	3,
};

enum eATTR_EQUIP_TYPE
{
    eATTR_EQUIP_TYPE_LEVEL		=	0,							// 유효하지 않는 속성
    eATTR_EQUIP_TYPE_STR		=	1,							// 유효하지 않는 속성
    eATTR_EQUIP_TYPE_DEX		=	2,							// 유효하지 않는 속성
    eATTR_EQUIP_TYPE_VIT		=	3,							// 유효하지 않는 속성
    eATTR_EQUIP_TYPE_INT		=	4,							// 유효하지 않는 속성
    eATTR_EQUIP_TYPE_SPR		=	5,							// 유효하지 않는 속성
    eATTR_EQUIP_TYPE_SKILL1		=	6,							// 유효하지 않는 속성
    eATTR_EQUIP_TYPE_SKILL2		=	7,							// 유효하지 않는 속성	
};

enum eGUILD_TRANSACTION
{
    GUILD_TRANS_ING						= (1<<0),
    GUILD_TRANS_SELECT_ING				= GUILD_TRANS_ING | (1<<1),
    GUILD_TRANS_CREATE_ING				= GUILD_TRANS_ING | (1<<2),
    GUILD_TRANS_DESTROY_ING				= GUILD_TRANS_ING | (1<<3),
    GUILD_TRANS_RANKUP_ING				= GUILD_TRANS_ING | (1<<4),
    GUILD_TRANS_CHANGE_GUILDMARK_ING	= GUILD_TRANS_ING | (1<<5),
    GUILD_TRANS_DONATE_UP_ING			= GUILD_TRANS_ING | (1<<6),
    GUILD_TRANS_CONVERT_UP2GP_ING		= GUILD_TRANS_ING | (1<<7),
    GUILD_TRANS_CHANGE_NOTICE_ING		= GUILD_TRANS_ING | (1<<8),

    GUILD_TRANS_END						= (1<<16),
    GUILD_TRANS_SELECT_END				= GUILD_TRANS_END | (1<<17),
    GUILD_TRANS_CREATE_END				= GUILD_TRANS_END | (1<<18),
    GUILD_TRANS_DESTROY_END				= GUILD_TRANS_END | (1<<19),
    GUILD_TRANS_RANKUP_END				= GUILD_TRANS_END | (1<<20),

    GUILD_TRANS_CHANGE_GUILDMARK_END	= GUILD_TRANS_END | (1<<21),
    GUILD_TRANS_DONATE_UP_END			= GUILD_TRANS_END | (1<<22),
    GUILD_TRANS_CONVERT_UP2GP_END		= GUILD_TRANS_END | (1<<23),
    GUILD_TRANS_CHANGE_NOTICE_END		= GUILD_TRANS_END | (1<<24),

    GUILD_TRANS_CHANGE_DUTY_ING			= GUILD_TRANS_ING | (1<<9),
    GUILD_TRANS_CHANGE_DUTY_END			= GUILD_TRANS_END | (1<<25),

    GUILD_TRANS_RELATION_ING			= GUILD_TRANS_ING | (1<<11),
    GUILD_TRANS_RELATION_END			= GUILD_TRANS_END | (1<<27),

};

enum eGUILD_STATE
{
    eGUILD_STATE_READY,
    eGUILD_STATE_START,
    eGUILD_STATE_END,
};

enum eNOTICE_TYPE
{
    eNOTICE_WORLD,
    eNOTICE_CHANNEL,
    eNOTICE_ZONE,
};

enum ePORTAL_MOVE_TYPE	// 이동타입( 0 : 필드이동, 1 : 마을 NPC 이동, 2 : 웨이 포인트 3: 비공정맵) // MapEnterancePortal.txt 의 이동타입 
{
    ePORTAL_MOVE_FIELD = 0,
    ePORTAL_MOVE_NPC,
    ePORTAL_MOVE_WAYPOINT,
    ePORTAL_MOVE_AIRSHIP,
};

// 서버 상에서 주변의 특수 렌더 정보를 보낼 필요가 있을 때... 중복 적용 가능
enum eSPECIAL_RENDER_EFFECT
{
    eSPECIAL_RENDER_EFFECT_NONE					= 0,
    eSPECIAL_RENDER_EFFECT_CHANGEofCLASS_1st	= 1<<0,
    //
    eSPECIAL_RENDER_EFFECT_SOLO_BADGE			= 1<<10,
    eSPECIAL_RENDER_EFFECT_COUPLE_BADGE			= 1<<11,
    eSPECIAL_RENDER_EFFECT_2nd_EFFECT_ITEM		= 1<<12,
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_01		= 1<<13,	// 대 중국용 추가 (WAVERIX)(080715)
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_02		= 1<<14,
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_03		= 1<<15,
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_04		= 1<<16,
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_05		= 1<<17,
};

//	(Waverix)(NOTE)(071125)(HARDCODE)
//
//	-- 서버 입장에서는 특수 Item에 관련된 효과 적용 여부를 판단하기 어려운 문제가 존재
//	(아이템의 비주얼 적인 효과는 클라에서만 사용한다.)
//	스크립트로 해결할라 치면, 결국 스크립트 열 추가 또는 비주얼 적인 효과를 서버도 알아야 한다.
//	(RENDER 정보의 BRD문제)
//	그리고, 그 타입도 서버는 알아야 한다. 이래 저래 복잡한 상황 많음.
//	-- (071125) 현 시점. 크리스마스 이벤트를 위한 특수한 처리가 요구되고 있다. 기획 자체도 기존과 유사한 듯
//	하면서도 골때리는 미묘한 차이가 존재하면서 발생하고 있는 문제라고 볼 수 있다. 뭐... 이것들을 모두 감내할 수
//	있는 유연한 구조를 만들지 못했다고 추궁한다면 할 말 없지만서두...
//	지금으로써는 다른 상황은 존재하지 않고 있으며, 따라서 이후 유사한 처리가 나올 경우,
//	그것과 같이 묶어서 정리하는 쪽이 더 깔끔한 구조가 나올 거라 기대한다.
//	-- 일단 아래와 같은 특수한 비주얼 타입에 대한 처리 검색을 위한 hard-code를 입력해 둔다.
//	eSPECIAL_RENDER_EFFECT 정보를 위한 것이며, 추후 제거하는 방향으로 나가자.
enum eSPECIAL_RENDER_EFFECT_ADDIN_INFO_IN_SERVER
{
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_SOLO_BADGE		= 40198,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_COUPLE_BADGE		= 40199,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_2nd_EFFECT_ITEM	= 40211,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_01	=  9014,	// 대 중국용 추가 (WAVERIX)(080715)
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_02	=  9015,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_03	=  9016,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_04	=  9017,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_05	=  9018,
};

enum GuildWarehouseLogType
{
    ITEM_IN,    //아이템 입고
    ITEM_OUT, //아이템 출고
    MONEY_IN, //돈 입금
    MONEY_OUT, //돈 출금
};

enum eGUILDWAREHOUSE_UPDATE_FACTOR
{
    UPDATE_FACTOR_COMMON,				// 길드 창고 업데이트에 영향을 끼치지 않는 업데이트
    UPDATE_FACTOR_CHAR_UPDATE,		//캐릭터의 주기적인 업데이트
    UPDATE_FACTOR_LOGOUT,				//캐릭터 로그아웃
    UPDATE_FACTOR_TIEM_OUT,				//길드 창고 오픈시간 타임아웃
    UPDATE_FACTOR_NORMAL_CLOSE,		//정상적인 창고 닫기
    UPDATE_COMPLETE,
};


// T_EventUser DB Table 다목적으로 사용하기 위한 예약코드
enum eEVENT_MODE_MULTIUSE
{
    eEVENT_MULTIUSE_START				= 10000,
    eEVENT_MULTIUSE_SSQ_WINNER_REWARD	,		// 승자 보상					10001
    eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO	,		// 퀘스트 실패 이후 정산정보	10002
    eEVENT_MULTIUSE_SSQ_REFUND			,		// 입장료 환불					10003
};


// __NA_S00015_20080828_NEW_HOPE_SHADOW
//죽었을때의 상태 
enum eCHAR_DEAD_TYPE
{
    eCHAR_DEAD_NOT_DEAD			= 0,			//죽지않음.
    eCHAR_DEAD_NORMAL			= 1,			//일반적으로 죽음
    eCHAR_DEAD_SUCTION			= 2,			//죽어서 HP/MP 빨림.
    eCHAR_DEAD_SUMMON			= 3,			//죽어서 소환되어 사라질 상태.
};

enum eSUMMON_TYPE
{
    eSUMMON_NORMAL				= 0,			//일반소환
    eSUMMON_DEAD_MONSTER		= 1,			//죽은몬스터소환
    eSUMMON_CRYSTALWARP         = 2,            // 크리스탈 워프 //_NA001385_20090924_DOMINATION_SKILL
    eSUMMON_IMMOVABLE           = 3,            // 고정형 소환수 //_NA_007667_20141001_WITCHBLADE_SKILL

    eSUMMON_MAX
};

//__NA001338_090519_TUTORIAL_FILED_TYPE_CHANGE__
//  (WAVERIX) (NOTE) 게임이 시간이 흐르면서 절대 변하지 않는 상수값들이 존재하곤 한다.
//  MapCode역시 마찬가지, 그러나 무조건 상수 처리하기에는 껄끄러운 부분이 존재.
//  그래도 일부 예외처리를 위해서 사용하는데, 찾아들어가는 문제가 복잡하므로 다음과 같이 한다.
//  #1) 20206 (튜토리얼 필드)의 경우
//
struct HardCode
{
    enum eMapCodes{
        MAPCODE_for_TRINGGEL       = 10001,
        MAPCODE_for_VALDCHEN       = 10002,
        MAPCODE_for_GM             = 10005,    // GM 맵(이그니스)   //__NA_0_20090714_POTAL_ITEM_DONOT_MOVE_GMMAP
        MAPCODE_for_TUTORIAL_FIELD = 20206,    //(090520) 에소드 마을
		MAPCODE_for_SONNENSCHEIN   = 43,	   // 소넨샤인
		MAPCODE_for_CROSSINGSPACEOFTIME = 46,  // 시간의 교차공간
    };
    enum eItems {
        ITEMCODE_of_ETHERIA = 4983, // _NA000000_100427_ITEM_CUSTOMIZING_CONTENTS_
    };
};

//__KR_001355_20090608_GROUPINGSYSTEM_DATA
//그룹 시스템에서 유저 자신이 가지고 있는 그룹의 상하위 정의
enum eGROUP_STAT
{
    //그룹내 유저 자신과 그룹원들간의 관계를 알기 위한 STAT
    GROUP_LEVEL_HIGH				= 1,	//내 그룹의 상위 레벨이다.
    GROUP_LEVEL_LOW					= 2,	//내 그룹의 하위 레벨이다.
    GROUP_LEVEL_OUT					= 3,	//내 그룹을 탈퇴 했다.
    GROUP_LEVEL_HIGH_OVERDATE       = 4,    //내 상위 그룹 맴버가 접속한지 1주일이 넘었다.

    //GameServer에서 케릭터의 그룹 상태를 알기 위한 STAT
    GROUP_HAVE_NONE					= 10,	//그룹을 가지고 있지 않다.
    GROUP_HAVE_ONLINE_HIGH			= 11,	//상위 맴버를 가지고 있다.(현재 상태는 온라인)
    GROUP_HAVE_OFFLINE_HIGH			= 12,	//상위 맴버를 가지고 있다.(현재 상태는 오프라인)
    GROUP_HAVE_ONLINE_LOW			= 13,	//하위 맴버를 가지고 있다.(현재 상태는 온라인)
    GROUP_HAVE_OFFLINE_LOW			= 14,	//하위 맴버를 가지고 있다.(현재 상태는 오프라인)

    GROUP_STAT_ERROR                = 1000, //그룹 맴버 정보가 잘못 저장 되었을 경우
};

//__NA_0_20090818_SYSTEM_MEMO_SUPPORT
// 시스템 메모 타입
enum eSYSTEM_MEMO_TYPE
{
    NORMAL_MEMO = 0,
    SYSMEMO_LOTTO_WIN = 1,          // 로또 당첨 안내 쪽지: SYSMEMO_LOTTO_WIN|EntryIndex|DrawNum|Rank|Prize
    SYSMEMO_GROUP_MGR = 2,          // 그룹 경험치 안내
    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
    SYSMEMO_FRIENDINVITATION_INVATES_TART		= 3,          // 자신이 친구추천한 친구가 캐릭터를 만들어서 친구추가 이벤트 시작한다.
    SYSMEMO_FRIENDINVITATION_INVATE_END			= 4,          // 자신이 친구추천한 캐릭터가 이벤트 만료레벨까지 레벨업되어 이벤트 종료한다.
    SYSMEMO_FRIENDINVITATION_INVATE_PRESENT		= 5,		  // 자신이 친구추천한 캐릭터가 이벤트 레벨을 달성하여 선물을 지급한다.
    SYSMEMO_FRIENDINVITATION_BEINVATED_START	= 6,          // 친구추천 받은 유저가 캐릭터를 만들어서 친구추가 이벤트 시작한다.
    SYSMEMO_FRIENDINVITATION_BEINVATED_END		= 7,          // 친구추천 받은 캐릭터가 이벤트 만료레벨까지 레벨업되어 이벤트 종료한다.
    SYSMEMO_FRIENDINVITATION_BEINVATED_PRESENT	= 8,          // 친구추천 받은 캐릭터가 이벤트 레벨을 달성하여 선물을 지급한다.	
    //}
    SYSMEMO_DOMINATION_RETURN_MONEY = 9,                      //지역 점령전 에서 경매에 실패 했을 경우 지급되는 하임
    SYSMEMO_DOMINATION_REWARD_MONEY = 10,                     //지역 점령전에서 방어 길드에게 경매 금액의 일부를 지급

    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION //새로운 시스템 쪽지 타입 추가(신/구 구분)
    SYSMEMO_LOTTO_WIN_NEW = 11,
    SYSMEMO_DOMINATION_RETURN_MONEY_NEW = 12,                     
    SYSMEMO_DOMINATION_REWARD_MONEY_NEW = 13,
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    SYSMEMO_WORLD_RANKING_GRADE = 14,
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SYSMEMO_GUILD_JOIN_REQUEST = 15,                          // 길드 가입 신청 알림 메세지
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SYSMEMO_SUN_RANKING_SETTLE_RESULT   = 16,                 // 썬랭킹 정산 결과 알림 메세지
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
    SYSMEMO_MAX,
};

//_NA001385_20090924_DOMINATION_EVENT_CONTROLL
enum eDOMINATION_EVENT_STATE
{
    DOMINATION_EVENT_AUCTION_START = 0,         // 점령전 입찰 시작
    DOMINATION_EVENT_AUCTION_END,               // 점령전 입찰 종료
    DOMINATION_EVENT_SELECT_ATTACKGUILD,        // 점령전 공격 길드 선정 이벤트
    DOMINATION_EVENT_SELECT_ATTACKGUILD_END,    // 공격 길드 선정 종료 이벤트 종료
    DOMINATION_EVENT_SYSTEM_RESET,              // 점령전 시작전 관련된 모든 시스템 초기화 시간
    DOMINATION_EVENT_WAIT,                      // 점령전 입장후 점령전 본 이벤트까지 대기 시간
    DOMINATION_EVENT_START,                     // 점령전 시작
    DOMINATION_EVENT_END,                       // 점령전 종료
    DOMINATION_EVENT_AUCTION_START2,            // 점령전 종료후 경매 재시작
    DOMINATION_EVENT_MAX                        // MAX 값
}; 

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
//점령전에서 현재 경매 시스템을 사용하지 않으므로 스크립트에서 삭제하고 아래와 같이 상수로 처리한다.
//관련 로직을 모두 수정해야하지만 예기치못한 오류를 막기위한 조치이다.
enum eDOMINATION_EVENT_TIME
{
    DOMINATION_EVENT_AUCTION_START_TIME = 0,
    DOMINATION_EVENT_AUCTION_END_TIME = 1,
    DOMINATION_EVENT_SELECT_ATTACKGUILD_TIME = 300,
    DOMINATION_EVENT_SELECT_ATTACKGUILD_END_TIME = 500,
    DOMINATION_EVENT_AUCTION_START2_TIME = 240000,
};

//새로운 방식의 점령전 종류가 생김에 따라 dominationInfo.txt에 칼럼을 추가하고 아래와 같이 정의
enum eDOMINATION_TYPE//ENTER_CONDITION_TYPE
{
    DOMINATION_TYPE_NORMAL = 0,     //기본 점령전       //길드가 있어야 입장 가능
    DOMINATION_TYPE_MONDSCHIEN = 1, //몬트샤인 점령전   //길드와 성을 가지고 있어야 입장 가능
};
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

enum eDOMINATION_EVENT_TYPE
{
    DOMINATION_INIT = 0,
    DOMINATION_INFO_SET,
    DOMINATION_DEFFENSEWALL_SET,        // 방어 벽 셋
    DOMINATION_SUCCESS_DEFENCE,         // 방어 성공
    DOMINATION_SUCCESS_OFFENCE,         // 공격 성공 (주인 바뀜)
    DOMINATION_DISGUILD_SET,            // 공격 길드 선정
    DOMINATION_GM_OFFENCE_GUILD_SET,    // gm 명령어를 통한 공격 길드 셋팅
    DOMINATION_GM_DEFENCE_GUILD_SET,    // gm 명령어를 통한 방어 길드 셋팅
    DOMINATION_ADD_DEFFENSEWALL_POINT,  // 점령전 방벽 포인트 증가
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    DOMINATION_TURN_OVER,               // (몬트샤인) 공격 성공, 공수 교대
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
}; 

enum eDOMINATION_ADD_GUILD_INFO_TYPE
{
    DOMINATION_JOIN_COUNT       = 1<<0,       // 점령전 참여 횟수 알림
    DOMINATION_DEFENSE_COUNT    = 1<<1,       // 점령전 방어 횟수 알림
    DOMINATION_HUNTING_COUNT    = 1<<2,       // 점령전 보스 사냥 횟수 알림
    DOMINATION_MAP_COUNT        = 1<<3,       // 점령전 점령 지역 횟수 알림
};

enum eDOMINATION_TENDER_TYPE
{
    NEW_TENDER_COST = 0,
    DUP_TENDER_COST = 1,
};

enum eDOMINATION_NOTIFY_TIME
{
    DOMINATION_WAR_TIME = 0,
    DOMINATION_JOIN_TIME = 1,
};

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
// 점령전 점령 지역의 진행상태 표시
enum eDOMINATION_PROGRESS_STATE{
    DOMINATION_PROGRESS_STATE_NONE = 0,
    DOMINATION_PROGRESS_STATE_READY,
    DOMINATION_PROGRESS_STATE_START,
    DOMINATION_PROGRESS_STATE_BREAK_GATE,
    DOMINATION_PROGRESS_STATE_BREAK_TEMPLEOFBLOOD,
    DOMINATION_PROGRESS_STATE_BREAK_CRYSTALIZEDBLOOD,
    DOMINATION_PROGRESS_STATE_END,
    DOMINATION_PROGRESS_STATE_MAX
};
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

//_NA_008334_20150608_SONNENSCHEIN
enum eSONNENSCHEIN_EVENT_STATE
{
    SONNENSCHEIN_EVENT_STATE_NONE = 0,          // 일반상태
    SONNENSCHEIN_EVENT_STATE_WAIT,      // 초기화, 준비, 방생성가능
    SONNENSCHEIN_EVENT_STATE_START,         // 입장, 전투, 각인가능
    SONNENSCHEIN_EVENT_STATE_END,           // 종료 (시간상 종료)
    SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE, // 각인완료 (목표달성으로 인한 종료)
    SONNENSCHEIN_EVENT_STATE_DESTROY,       // 소멸대기
};

// _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
enum eGUILDMISSION_STATE
{
	eGUILDMISSION_NONE = 0,								// 기본값
	eGUILDMISSION_ENTERWAIT,							// 길드원이 들어오길 기다리는 상태
	eGUILDMISSION_MATCHED,								// 전장 진행 상태
	eGUILDMISSION_MATCHEDEND,							// 전장 진행 완료
	eGUILDMISSION_RESULT,								// 결과 상태
	eGUILDMISSION_END,									// 종료 상태
	eGUILDMISSION_KICK_USER,							// 유저 강제 퇴장 
	eGUILDMISSION_DESTROY,								// 방 파괴
	eGUILDMISSION_MAX
};

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
enum eBATTLE_RECORD_TYPE
{
    BATTLE_RECORD_ATTACK = 0,
    BATTLE_RECORD_DAMAGE,
    BATTLE_RECORD_MONSTER_KILL,
    BATTLE_RECORD_PLAYER_KILL,
    BATTLE_RECORD_DEAD,
    BATTLE_RECORD_HEAL,
    BATTLE_RECORD_LEVEL,
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BATTLE_RECORD_STRONGPOINT_ATTACK,
    BATTLE_RECORD_STRONGPOINT_DEFENSE,
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
    BATTLE_RECORD_ASSIST,
#endif //_NA_006633_20130415_ADD_BATTLEGROUND_ASSIST
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    BATTLE_RECORD_GOLDPOINT,
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
};

//_KR_0_20091021_FIRENDINVITATION_EXTEND
#define FRIENDINVITATION_RECALL_MSGBOX_MAXTIME 15000 //친구 소환요청을 할때 상대방의 메시지박스가 사라지기 전까지의 시간

// _NA_0_20100722_AUTO_SYSTEM
enum eAUTO_HUNTING_TYPE
{
    eAUTO_TYPE_INVALID = 0,
    eAUTO_RANGE_FIELD,              // 사냥설정_맵전체
    eAUTO_RANGE_PARTY,              // 사냥설정_파티원주변
    eAUTO_RANGE_USER,               // 사냥설정_특정범위(자신 주변)
    eAUTO_RANGE_BOSS,               // 사냥설정_보스경고
    eAUTO_PICKUP_ALL,               // 줍기설정_모든아이템
    eAUTO_PICKUP_NORMAL,            // 줍기설정_일반아이템
    eAUTO_PICKUP_QUEST,             // 줍기설정_퀘스트아이템
    eAUTO_PICKUP_STONE,             // 줍기설정_스톤아이템
    eAUTO_PICKUP_WEAPON,            // 줍기설정_무기아이템
    eAUTO_PICKUP_ARMOR,             // 줍기설정_방어구아이템
    eAUTO_PICKUP_AC,                // 줍기설정_AC아이템
    eAUTO_PICKUP_HEIM,              // 줍기설정_하임
    eAUTO_PICKUP_ITEMTYPE,          // 줍기설정_특정아이템
    eAUTO_CURE_HP,                  // 회복설정_HP
    eAUTO_CURE_MPSP,                // 회복설정_MPSP
    eAUTO_CURE_INSTANT,             // 회복설정_즉시회복
    eAUTO_CURE_PET_FEED,            // 회복설정_사료(동물 먹이)
    eAUTO_CURE_SITDOWN_HP,          // 회복설정_휴식(앉기, 해당 HP 이하일경우)
    eAUTO_CURE_SITDOWN_MP,          // 회복설정_휴식(앉기, 해당 MP 이하일경우)
    eAUTO_REBIRTH_VILIAGE,          // 자동부활_마을부활
    eAUTO_REBIRTH_WAITING,          // 자동부활_부활대기
    eAUTO_REBIRTH,                  // 자동부활_제자리 부활
    eAUTO_RETURNSCROLL_INVEN,       // 마을이동_인벤토리
    eAUTO_RETURNSCROLL_POTION,      // 마을이동_회복물약
    eAUTO_RETURNSCROLL_PET_FEED,    // 마을이동_사료
    eAUTO_RETURNSCROLL_DURATION,    // 마을이동_내구도
    eAUTO_SELL_ALL,                 // 자동판매_모든아이템             
    eAUTO_SELL_NORMAL,              // 자동판매_일반아이템             
    eAUTO_SELL_QUEST,               // 자동판매_퀘스트아이템           
    eAUTO_SELL_STONE,               // 자동판매_스톤아이템
    eAUTO_SELL_WEAPON,              // 자동판매_무기아이템
    eAUTO_SELL_ARMOR,               // 자동판매_방어구아이템
    eAUTO_SELL_AC,                  // 자동판매_AC아이템
    eAUTO_BUY_RETURNSCROLL,         // 자동구매_귀환스크롤
    eAUTO_BUY_PET_FEED,             // 자동구매_일반사료
    eAUTO_BUY_DURATION,             // 자동구매_내구도전체수리
    eAUTO_BUY_POTION_LEVEL,         // 자동구매_회복물약
    eAUTO_BUY_POTION_COUNT,         // 자동구매_회복물약
    eAUTO_COMMUNITY_PARTY_JOIN,     // 커뮤니티_파티수락
    eAUTO_COMMUNITY_PARTY_BLOCK,    // 커뮤니티_파티거부
    eAUTO_COMMUNITY_SAVE_CHAT,      // 커뮤니티_채팅내용저장
    eAUTO_COMMUNITY_SHOUT,          // 커뮤니티_외치기
    eAUTO_COMMUNITY_CHANNEL,        // 커뮤니티_채널참가

    //! 2011.9.16 / i4u4me /  
    eAUTO_RETURNSCROLL_RETURNFIELD, // 마을이동_이동후 업무 마치고 다시 필드로 //!<44

    eAUTO_TYPE_MAX,        
};

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// 파티 모집 목록 상수
struct PartyFindingListConst
{
    enum EnumValue
    {
        kMinLength = 0, // 최소 파티 모집 수 
        kMaxLength = 100 // 최대 파티 모집 수
    };

    typedef BYTE ValueType;
    
    static ValueType GetValue(const EnumValue enum_value)
    {
        //assert(enum_value >= kMinLength && enum_value <= kMaxLength);
        return static_cast<ValueType>(enum_value);
    }

}; // PartyFindingListConst

// 파티 모집 제목 상수
struct PartyFindingTitleConst
{
    enum
    {
        kMinLength = 5, // 최소 글자수
        kMaxLength = 30 // 최대 글자수
    };

    // 버퍼 길이
    enum { kBufferLength = kMaxLength + 1 };

}; // PartyFindingTitleConst

// 파티 모집 목적 상수
struct PartyFindingGoalConst
{    
    enum EnumValue
    {
        kMinBound = -1, 
        kMission, // 미션
        kQuest, // 퀘스트
        kAC, // AC
        kCursedTower, // 저주받은 탑
        kEtc, // 기타
        kCount
    };

    typedef BYTE ValueType;
    
    static ValueType GetValue(const EnumValue enum_value)
    {
        //assert(enum_value > kMinBound && enum_value < kCount);
        return static_cast<ValueType>(enum_value);
    }

    static ValueType GetDefaultValue()
    {
        return GetValue(kMission);
    }

}; // PartyFindingGoalConst

// 파티 모집 인원수 상수
struct PartyFindingNumOfMemberConst
{    
    enum EnumValue
    {
        kMin = 2, // 최소 인원
        kMax = 10 // 최대 인원
    };

    typedef BYTE ValueType;
    
    template <typename T>
    static ValueType ConvertValue(const T number)
    {
        return static_cast<ValueType>(number);
    }

    static ValueType GetValue(const EnumValue enum_value)
    {
        //assert(enum_value >= kMin && enum_value <= kMax);
        return ConvertValue(enum_value);
    }

    static ValueType GetDefaultValue()
    {
        return GetValue(kMax);
    }
    
}; // PartyFindingNumOfMemberConst

// 파티 모집 응답 종류 상수
struct PartyFindingResponseTypeConst
{
    enum EnumValue
    {
        kMinBound = -1, 
        kUnknown, // 알 수 없는 종류
        kRegister, // 파티 모집 등록
        kUpdate, // 파티 모집 갱신
        kGetList, // 파티 모집 목록 얻기
        kJoin, // 파티 모집 참가
        kCancel, // 파티 모집 취소
        kCount
    };

    typedef BYTE ValueType;

    static ValueType GetValue(const EnumValue enum_value)
    {
        //assert(enum_value > kMinBound && enum_value < kCount);
        return static_cast<ValueType>(enum_value);
    }

    static ValueType GetDefaultValue()
    {
        return GetValue(kUnknown);
    }

}; // PartyFindingResponseTypeConst

#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

////////////////////////////////////////////////////////////////////////////////////////////////////

//_NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM

// 캐릭터 외모 변경 종류 상수
struct ChangeCharacterAppearanceTypeConst
{
    enum EnumValue
    {
        kNone = 0x00, // 변경 없음
        kFace = 0x01, // 얼굴
        kHair = 0x02, // 헤어
        kHeight = 0x04, // 키
        kGender = 0x08, // 성별
        kAll = kFace|kHair|kHeight|kGender // 모두
    };

    typedef BYTE ValueType;

    // 나열자 값을 실제 사용할 값의 형식으로 변환한다.
    inline static ValueType EnumToValue(const EnumValue enum_value)
    {
        return static_cast<ValueType>(enum_value);
    }

    // 모두 변경 되는지 검사한다.
    inline static bool IsChangeAll(const ValueType change_type_value)
    {
        return (change_type_value == kAll);
    }

    // 얼굴이 변경 되는지 검사한다.
    inline static bool IsChangeFace(const ValueType change_type_value)
    {
        return ((change_type_value & kFace) == kFace);
    }

    // 헤어가 변경 되는지 검사한다.
    inline static bool IsChangeHair(const ValueType change_type_value)
    {
        return ((change_type_value & kHair) == kHair);
    }

    // 키가 변경 되는지 검사한다.
    inline static bool IsChangeHeight(const ValueType change_type_value)
    {
        return ((change_type_value & kHeight) == kHeight);
    }

    // 성별이 변경 되는지 검사한다.
    inline static bool IsChangeGender(const ValueType change_type_value)
    {
        return ((change_type_value & kGender) == kGender);
    }
}; // ChangeCharacterAppearanceTypeConst

////////////////////////////////////////////////////////////////////////////////////////////////////

enum NPC_SHOP_TYPE
{
    SHOP_TYPE_OF_NARMAL = 1,    //일반 상점
    SHOP_TYPE_OF_ITEM,          //아이템 상점
    SHOP_TYPE_OF_ACCUMULATE,    //적립 포인트 상점
    SHOP_TYPE_OF_CURRENCY,      //화폐 포인트 상점
    SHOP_TYPE_MAX,
};

enum ePARTY_TYPE
{
    ePARTY_FIELD,
    ePARTY_ROOM,
    ePARTY_INSTANCE,
    ePARTY_MAX
};

//매우 중요 : BattlezoneMissiontree스크립트 Kind 내용을 기준으로 설정
//미션 타입이 추가 되면 반드시 아래 항목도추가 되어야 한다.
enum PartyZoneType{
    PartyOfNone = 1,                //기본
    PartyOfMission = 2,             //미션
    PartyOfTopOfTrialByCash = 3,    //저탑 캐쉬 아이템
    PartyOfTopOfTrialByNormal = 4,  //저탑 기본 
    PartyOfPvPByPersonal = 5,       //PVP개인전
    PartyOfPvPByTeam = 6,           //PVP팀전
    PartyOfEvent = 7,               //이벤트 미션
    PartyOfSSQ = 8,                 //타락한 사원
    PartyOfHardcore = 9,            //하드코어
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    PartyOfChaosZone = 10,          //카오스존
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
    PartyOfTopOfTrialLight_normal = 11, // 저주받은탑 빛의시련
    PartyOfTopOfTrialLight_cash = 12, // 저주받은탑 빛의시련(캐쉬아이템)
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    PartyOfSpecialMission = 13,
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    PartyOfMax
};

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
enum ChaosZoneCategory
{
    kCategoryNone = 0,
    kCategoryZone,
    kCategoryBattleGround,
    kCategoryFreePVP,
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    eCHAOSZONE_CATEGORY_GOLDRUSH,
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
    eCHAOSZONE_CATEGORY_SONNENSCHEIN,
#endif  //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	eCHAOSZONE_CATEGORY_ONEDAYSGUILDMISSION,
#endif  //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
};
enum ChaosZoneType
{
    kChaosZoneTypeNone = 0,
    kBattleGround,
    kFreePVP,
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    eCHAOSZONE_TYPE_GOLDRUSH,
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
    eCHAOSZONE_TYPE_SONNENSCHEIN,
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION,
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
    eCHAOSZONE_TYPE_MAX
};
enum ChaosZoneTeam
{
    kChaosZoneTeamNone =0,
    kRed,
    kBlue,
    kObserve, // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
};

enum BattleGroundPlayerState
{
    kPlayerStateNone = 0,
    kPlayerStateMatch, // 전장 진행상태
    kPlayerStateDeath, // 죽은 상태
    kPlayerStateStandbyResurection, // 부활대기상태
    kPlayerStateStandbyResurectionRefuse, // 부활대기 거부 상태
};
enum BattleGroundStrongpointState
{
    kStrongpointStateNone = 0,      // 중립 거점
    kStrongpointStateWait,          // 대기 상태(선점)
    kStrongpointStateWaitAttack,    // 대기 상태(공격)
    kStrongpointStateConquer,       // 점령상태
    kStrongpointStateDefence,       // 점령방어
};
enum ChaosZoneResultType
{
    kResultNone = 0,
    kResultWin,
    kResultLose,
    kResultDraw,
    kResultAbuse,
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
// 골드러시에서 골드포인트에 따른 등급설정
enum GOLDGRADE
{
    eGOLDGRADE0 = 0,
    eGOLDGRADE1,
    eGOLDGRADE2,
    eGOLDGRADE3,
    eGOLDGRADE4,
    eGOLDGRADE5,
    eGOLDGRADE6,
    eGOLDGRADE_MAX
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
enum eATTENDANCE_INFO
{
    eATTENDANCE_INFO_NOT_ATTEND = 0,    // 출석 안함
    eATTENDANCE_INFO_ATTENDED,          // 출석 함
    eATTENDANCE_INFO_ALEADY_GOT_REWARD, // 보상 받음
    eATTENDANCE_INFO_MAX,
};
enum eATTENDANCE_TYPE
{
    eATTENDANCE_CALENDAR = 0,       // 달력형
    eATTENDANCE_SUPERPOSITION,      // 중첩형
    eATTENDANCE_MAX,
};
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

enum GuildInGameLogCategory{
    GuildGrowthLog,
    GuildMemberLog,
    GuildRelationLog,
    GuildDominationLog,
    GuildMaxLog,
};

//서버 : 길드로그 추가시에는 반드시 GuildInGameLog::_GetLogCategory()에 반드시 추가
enum GuildInGameLogType{
    //GuildGrowthLog
    GuildCreate,		    //길드 창설
    GuildLevelUp,	        //길드 레벨업
    GuildLevelDown,         //길드 레벨 다운
    GuildEquipmentApply,    //길드 시설 활성화
    GuildEquipmentLevelup,  //길드 시설 레벨 업
    GuildEquipmentLevelDown,//길드 시설 레벨 다운
    GuildGivePoint,         //길드 포인트 기부
    GuildChangeName,        //길드 이름 변경
    GuildRegisterMark,      //길드 마크 등록
    GuildRegisterAnnounce,  //길드 공지 등록
    GuildRegisterIntroduce, //길드 소개 등록

    //GuildMemberLog
    GuildMemberJoin,	    //가입
    GuildMemberKick,	    //제명
    GuildMemberOut,	        //탈퇴
    GuildDutyChange,	    //직책 변경
    GuildChangeMaster,      //권한 위임

    //GuildRelationLog
    GuildRelationAllienceSet,   //길드 관계 설정
    GuildRelationAllienceCancle,//길드 관계 해지
    GuildSendHostilitySet,  //길드 적대 관계 선포
    GuildRecvHostilitySet,  //길드 적대 관계 받음
    GuildBothHostilitySet,  //길드 적대 관계 설정
    GuildHostilityCancel,   //길드 적대 관계 해제

    //GuildDominationLog
    GuildSelectedDomiAttack,//점령전 공격 길드 선정
    GuildNotifyDomiAttack,  //내 지역에 공격 길드가 선정
    GuildSuccessDomiAttack, //점령전 공격 성공
    GuildSuccessDomiDefense,//점령전 수비 성공
    GuildAuctionBatting,    //점령전 경매 배팅
    GuildFailAuction,       //점령전 입찰 실패
    GuildPenaltyTime,       // 점령전 패널티 //_NA_000000_20130429_DOMINATION_PENALTY

    GuildInGameLogMax,
};

enum ObtainType
{
    kObtainType_Disable = 0,    //파티원전원획득 비활성
    kObtainType_Enable = 1,     //파티원전원획득 활성
};

enum ToggleItemType
{
    kToggleItem_Badge,
    kToggleItem_PCBang,
    kToggleItemMax,
};

//_NA_007667_20141001_WITCHBLADE_SKILL
// 스킬 발동 조건 검사 (SkillInfo Ability Option2)
enum eSKILL_CONDITION
{
    eSKILL_CONDITION_NONE = 0,
    eSKILL_CONDITION_BELOW_HP = 1,    // 특정 HP값 '이하'일 경우
    
    eSKILL_CONDITION_MAX
};

//_NA_007667_20141001_WITCHBLADE_SKILL
enum eSKILL_UPDATE_CONDITION
{
    eSKILL_UPDATE_CONDITION_RANDOM = 0,     //npcinfo의 SkillUpdateTime값이 가변적
    eSKILL_UPDATE_CONDITION_FIXED = 1,      //npcinfo의 SkillUpdateTime값이 고정됨

    eSKILL_UPDATE_CONDITION_MAX
};

// _NA_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO
// 친구초대 보상 지급되는 레벨구간
enum INVITATIONFRIEND_REWARDLEVEL
{
#ifdef _JP_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO
    eINVITATION_REWARDSECTTION_0 = 0,
    eINVITATION_REWARDSECTION_1 = 30,
    eINVITATION_REWARDSECTION_2 = 60,
    eINVITATION_REWARDSECTION_3 = 90,
    eINVITATION_REWARDSECTION_4 = 120,
    eINVITATION_REWARDSECTION_MAX = 200
#else
    eINVITATION_REWARDSECTTION_0 = 0,
    eINVITATION_REWARDSECTION_1 = 20,
    eINVITATION_REWARDSECTION_2 = 40,
    eINVITATION_REWARDSECTION_3 = 60,
    eINVITATION_REWARDSECTION_4 = 80,
    eINVITATION_REWARDSECTION_MAX = 200
#endif //_NA_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO
};

//_NA_007919_20141211_ADD_GMCOMMAND_EVENT
enum ePEAKTIME_EVENT_NOTIFY 
{
    ePEAKTIME_PRENOTIFY = 0,    // 시작전 사전알림
    ePEAKTIME_START,            // 피크타임이벤트 시작
    ePEAKTIME_END,              // 이벤트 종료
    ePEAKTIME_MAX
};

//_NA_008012_20150204_SUN_RANKING_SYSTEM
/* 주의 - 아래 값들은 DB에 저장되는 타입이므로 변경되지 않아야한다. 추가되는 타입은 END위에 추가되어야한다*/
enum eSUN_RANKING_TYPE
{
	eSUN_RANKING_TYPE_START = 0,
	eSUN_RANKING_TYPE_BEST_OF_BEST = eSUN_RANKING_TYPE_START,    // Best of Best
	eSUN_RANKING_TYPE_ELETE_WARRIOR,   // 엘리트 전사
	eSUN_RANKING_TYPE_LEVEL_KING,      // 레벨왕
	eSUN_RANKING_TYPE_LEGEND_EQUIP,    // 전설적인 장비
	eSUN_RANKING_TYPE_STRONG_ONE_SHOT, // 강력한 한방
	eSUN_RANKING_TYPE_BEST_ATTACKER,   // 최고의 공격수
	eSUN_RANKING_TYPE_MASTER_OF_HEAL,  // 치유의 달인
	eSUN_RANKING_TYPE_END,
};
static eSUN_RANKING_TYPE& operator++(eSUN_RANKING_TYPE& object)
{
    if (object != eSUN_RANKING_TYPE_END)
    {
        object = static_cast<eSUN_RANKING_TYPE>(object + 1);
    }
    return object;
}
enum eSUN_RANKING_GRADE_TYPE
{
    eSUN_RANKING_GRADE_START = 0,
    eSUN_RANKING_GRADE_0 = eSUN_RANKING_GRADE_START,
    eSUN_RANKING_GRADE_1,
    eSUN_RANKING_GRADE_2,
    eSUN_RANKING_GRADE_3,
    eSUN_RANKING_GRADE_4,
    eSUN_RANKING_GRADE_5,
    eSUN_RANKING_GRADE_6,
    eSUN_RANKING_GRADE_7,
    eSUN_RANKING_GRADE_8,
    eSUN_RANKING_GRADE_9,
    eSUN_RANKING_GRADE_NONE,
    eSUN_RANKING_GRADE_END = eSUN_RANKING_GRADE_NONE,
};
static eSUN_RANKING_GRADE_TYPE& operator++(eSUN_RANKING_GRADE_TYPE& object)
{
    if (object != eSUN_RANKING_GRADE_END)
    {
        object = static_cast<eSUN_RANKING_GRADE_TYPE>(object + 1);
    }
    return object;
}
enum eSUN_RANKING_SETTLE_TYPE
{
    eSUN_RANKING_SETTLE_MONTH = 0,  // 월간 정산
    eSUN_RANKING_SETTLE_WEEK,       // 주간 정산
};
enum eSUN_RANKING_STATE
{
    eSUN_RANKING_STATE_INIT = 0,            // 초기화
    eSUN_RANKING_STATE_LOAD,                // 로드
    eSUN_RANKING_STATE_RUN,                 // 작중 중
    eSUN_RANKING_STATE_SETTLE_WAIT,         // 정산 중
    eSUN_RANKING_STATE_SEND_MEMO,           // 메모 보냄
    eSUN_RANKING_STATE_MAX,
};

enum eSTATUE_TYPE
{
    eSTATUE_TYPE_BEST_OF_BEST,
};

enum eSTATUE_INDEX
{
    eSTATUE_INDEX_FIRST,
};

//_NA_008283_20150511_NOTIFICATIONS_SYSTEM
enum NotificationType
{
    eNotificationNone = 0,
    eNotificationEnchant,       // 인챈트
    eNotificationAwakening,     // 각성
    eNotificationDrop,          // 드랍
    eNotificationCraft,         // 제작
    eNotificationEvolution,     // 진화
    eNotificationBox,           // 상자
    eNotificationAcReward,      // AC보상
    eNotificationMissionReward, // 미션보상
	eNotificationBingoReward,   // 빙고보상
};

//_NA_008404_20150712_MINIGAME_SYSTEM
enum eMINIGAME_TYPE
{
    eMINIGAME_TYPE_START = 0,
    eMINIGAME_TYPE_BINGO = eMINIGAME_TYPE_START,
    //
    eMINIGAME_TYPE_NONE,
    eMINIGAME_TYPE_END = eMINIGAME_TYPE_NONE,
};

enum eMINIGAME_BINGO_STATE
{
    eMINIGAME_BINGO_STATE_EMPTY = 0,     // 빈 칸
    eMINIGAME_BINGO_STATE_FILLED,        // 채워진 칸
    eMINIGAME_BINGO_STATE_FREE,          // 프리 칸
};
//==================================================================================================
//==================================================================================================
//==================================================================================================
//== Game constant control utilities
//==================================================================================================

namespace util {
;

template<size_t _Size, size_t _CharSize>
struct GuildNameStrCopyChecker {
    struct DestBufferSize_Mustbe_same_MAX_GUILDNAME_LENGTH_with_null_terminated {
        static const bool value = false;
    };
    C_ASSERT(DestBufferSize_Mustbe_same_MAX_GUILDNAME_LENGTH_with_null_terminated::value);
    typedef TCHAR array[1];
};

template<>
struct GuildNameStrCopyChecker<(MAX_GUILDNAME_LENGTH + 1), sizeof(TCHAR)>
{
    typedef TCHAR array[MAX_GUILDNAME_LENGTH + 1];
};

inline void GUILDNAMECOPY(TCHAR* dest, const TCHAR* src);

// NOTE: f110826.1L
template<size_t _Size>
__forceinline bool CheckNullTerminatedString(const TCHAR (&null_terminated_string)[_Size]);

typedef TCHAR StrArrayForUserID[ID_MAX_LEN + 1];
typedef TCHAR StrArrayForCharName[MAX_CHARNAME_LENGTH + 1];
typedef TCHAR StrArrayForPetName[MAX_PET_NAME_LENGTH + 1];
typedef TCHAR StrArrayForVendorTitle[MAX_VENDOR_TITLE_LENGTH + 1];
typedef TCHAR StrArrayForRoomTitle[MAX_ROOMTITLE_LENGTH + 1];
typedef TCHAR StrArrayForGuildName[MAX_GUILDNAME_LENGTH + 1];
typedef TCHAR StrArrayForGuildNotice[MAX_GUILDNOTICE_LENGTH + 1];
typedef TCHAR StrArrayForMemo[MAX_MEMO_LENGTH + 1];
typedef TCHAR StrArrayForChatting[MAX_CHAT_LEN + 1];
typedef TCHAR StrArrayForGenericChattingMsg[255 + 1];

}; //end of namespace

//==================================================================================================

inline void util::GUILDNAMECOPY(TCHAR* dest, const TCHAR* src) 
{
    _tcsncpy(dest, src, MAX_GUILDNAME_LENGTH);
    dest[MAX_GUILDNAME_LENGTH] = _T('\0'); 
};

// NOTE: the casting of 'dest' is caused by explicit convertion operator. prevent forced casting.
#define GUILDNAMECOPY(dest, src) GUILDNAMECOPY(\
    *(util::GuildNameStrCopyChecker<_countof(dest), sizeof(dest[0])>::array*)\
    static_cast<TCHAR*>(dest), (src))

// NOTE: f110826.1L
template<size_t _Size>
__forceinline bool util::CheckNullTerminatedString(const TCHAR (&null_terminated_string)[_Size])
{
    return (null_terminated_string[_Size - 1] == _T('\0'));
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif // __CONST_H__
