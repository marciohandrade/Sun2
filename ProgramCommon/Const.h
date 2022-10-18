/** Const.h : DEFINE�� ����

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

//	�������� �߱��� ������Ʈ Ű ��� ����
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
    PLAYER_OBJECT_KEY		= 0,		//	�÷��̾�	: 1 ~ 5�� ( �ִ� 5���� )
    MONSTER_OBJECT_KEY		= 50000,	//	����		: 5�� ~ 10�� ( �ִ� 5����, 5���� ���� ��� )
    NPC_OBJECT_KEY			= 100000,	//	NPC			: 10�� ~ 15�� ( �ִ� 5����, 10���� ���� ��� )
    NONCHARACTER_OBJECT_KEY	= 150000,	//	???			: 15�� ~ 20�� ( �ִ� 5����, 15���� ���� ��� )
    MAP_OBJECT_KEY			= 200000,	//  �� ������Ʈ	: 20�� ~ 25�� 
    ITEM_OBJECT_KEY			= 250000,	//	������		: 25���� ���� ���( �������� ���� ���� )
    PET_OBJECT_KEY			= 300000,	//  �������Ʈ	: 30���� 
    ONLYCLIENT_OBJECT_KEY	= 340000,	//< Ŭ�󿡸� ����ϴ� Ű. 1������ ���
    ONLYCLIENT_BATTLEZONE_OBJECT_KEY	= 341000,	//< Ŭ�󿡸� ����ϴ� Ű.
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
    CMS_TUMBLING_FRONT,			// ��Ű�� �뽬
    CMS_TUMBLING_LEFT,			// ��Ű�� ��������
    CMS_TUMBLING_RIGHT,
    CMS_SHOULDER_CHARGE,		// ������ �������
    CMS_SLIDING,				// �巡�� �����̵�
    CMS_TELEPORT,				// ���� �ܻ��ڷ���Ʈ
    CMS_TUMBLING_BACK,          // ��Ű�� ��뽬
    CMS_MONSTER_ATTACKJUMP,     // ���� ���� ����
    CMS_LUCKY_MONSTER_RUNAWAY,	// ��Ű���� �������� �ӵ�
    CMS_STOP,
    CMS_KNOCKBACK2,				//__NA001048_080514_APPEND_ABILITY_KNOCKBACK2__
    CMS_MAX,
};

enum eZONE_TYPE
{
    ZT_VILLAGE					= 1,
    ZT_BATTLEZONE,
};


// ���� ������ ���� ����
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
    // eZONETYPE_TUTORIAL_FIELD�� eZONETYPE_FIELD�� �����ϸ�, eZONETYPE_FIELD�� ��� ������ �״�� ����Ѵ�.
    // (GetZoneType() ���� eZONETYPE_FIELD�� ���´ٴ� �ǹ�)
    // eZONETYPE_FIELD Ÿ�԰��� ������ sMAPINFO�� ���ؼ��� ������ �� �ֵ��� �Ѵ�.
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

// �������� ���
enum eCOMPETITION_RULE_TYPE
{
    eCOMPETITION_RULE_FREE_FOR_ALL,
    eCOMPETITION_RULE_DUAL_PARTY,

    eCOMPETITION_RULE_MAX,

};

enum eCOMPETITION_MODE_TYPE
{
    eCOMPETITION_MODE_SURVIVAL,			// �����̹� ���
    eCOMPETITION_MODE_TIME_ATTACK,		// Ÿ�� ���� ���
    eCOMPETITION_MODE_DEATH_MATCH,		// ������ġ ���

    eCOMPETITION_MODE_MAX,
};

// ��� ���� Ÿ��, ī�������� ���� ����� ���ƾ� �Ѵ�!
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
    eFUNCTIONALRESTRICT_USE_WAREHOUSE			= (1<<8),	//< USE_�����ϴµ���_��
    eFUNCTIONALRESTRICT_USE_TRADE				= (1<<9),	//< USE_�����ϴµ���_��
    eFUNCTIONALRESTRICT_USE_VENDOR				= (1<<10),	//< USE_�����ϴµ���_��
    eFUNCTIONALRESTRICT_USE_GUILD				= (1<<11),	//< USE_�����ϴµ���_��
    eFUNCTIONALRESTRICT_USE_MONSTERDROP			= (1<<12),	//< USE_�����ϴµ���_��
    eFUNCTIONALRESTRICT_COMBINE_ITEM			= (1<<13),
    eFUNCTIONALRESTRICT_DIVIDE_ITEM				= (1<<14),
    eFUNCTIONALRESTRICT_REPAIR_ITEM				= (1<<15),
    eFUNCTIONALRESTRICT_SELL_ITEM				= (1<<16),	//< 2006/9/13 �߰�
    eFUNCTIONALRESTRICT_BUY_ITEM				= (1<<17),	//< 2006/9/13 �߰�
    eFUNCTIONALRESTRICT_EXCHANGE_ITEM			= (1<<18),	//< 2006/9/13 �߰�
    eFUNCTIONALRESTRICT_VENDOR_LIST_NPC			= (1<<19),	//< 2006/10/19 �߰�
    eFUNCTIONALRESTRICT_BUY_SHOPITEM			= (1<<20),	//< 2006/12/9
    eFUNCTIONALRESTRICT_SHOP_SERVICE			= (1<<21),	//< 2006/12/9
    eFUNCTIONALRESTRICT_GM_COMMAND				= (1<<22),	//< 2007/06/19
    eFUNCTIONALRESTRICT_PARTY_RETURN_FOR_UCUSER	= (1<<23),	//< 2007/09/27
    eFUNCTIONALRESTRICT_WAREINVEN_LOCK_BY_PWD	= (1<<24),	//< 2007/11/07	__TASK_WAREHOUSE_N_INVENTORY_LOCK_BY_PWD__ ������ ��� ���� �ɱ� ����...
    eFUNCTIONALRESTRICT_ALL						= ((1<<25)-1),
};


/////////////////////////////////////////////////////////////////////////////////////////
// Item
/////////////////////////////////////////////////////////////////////////////////////////

// eITEMTYPE�� ������ ��ȣ�� ��ġ�ؾ� �Ѵ�!!!!
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
    eWEAPONTYPE_PUNCH			= 13,//���� ���� �Ұ� 
    eWEAPONTYPE_GUNTLET = 14, // ��Ʋ�� _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eWEAPONTYPE_POLEARM = 15, // ö�� _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eWEAPONTYPE_HELLROID_1 = 16, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eWEAPONTYPE_ARCBLADE = 17, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    eWEAPONTYPE_MAX,
};

// ���, ��뿩��
enum eITEM_MATERIAL_TYPE
{
    eITEM_MATERIAL_TYPE_NOT_USE_AND_EQUIP = 0,	// ���� / ��� �Ұ�
    eITEM_MATERIAL_TYPE_CAN_EQUIP,				// ���� ����
    eITEM_MATERIAL_TYPE_CAN_USE,				// ��� ����(�Ҹ�)
    eITEM_MATERIAL_TYPE_CAN_BOTH,				// ���� / ��� ���� 
    eITEM_MATERIAL_TYPE_CAN_USE_NOT_DELETE,		// ��� ����(�����Ұ�)
    eITEM_MATERIAL_TYPE_NOT_USE_CAN_DELETE, // ��� �Ұ� / ���� ����
};


// ����â�� �����ۺ� ��ġ 
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
    //����
    //eEQUIPCONTAINER_CAPE		= xx,
    //���� ���� ��� ����
    //eEQUIPCONTAINER_CREST		= xx,

    //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    // 91, 92, 93 - USED (eEQUIPCONTAINER_POS_FOR_RIDER_PARTS)

    eEQUIPCONTAINER_INVALID		= 99,
};

//--------------------------------------------------------------------------------------------------
// __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
// Ż �� ���� (Ŭ���� ����)
struct eRIDER_CLASS
{
    enum eValue {
        None        = 0,
        Charger     , //1 ���ڷ��� ����
        Leopard     , //2 ū�� ǥ��
        OrcPig      , //3 ��ũ �Ǳ�
        Kalvus      , //4 Į����
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

// Ż �� ����â�� �����ۺ� ��ġ 
struct eRIDER_PARTS_POS
{
    enum eValue {
        None        = 0, //�̰� ���� �������� �ƴϴ�.
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
--2010.05.20 ���� ������ ��ũ��Ʈ ����

0 : ��ΰ���
1 : �ŷ��� �Ұ���
2 : �ǸŸ� �Ұ���
4 : �ٴڿ� ��Ӹ� �Ұ���
8 : �ٸ����Կ� �̵� �Ұ���
16 : ����������� ������ �Ұ���
32 : ��� â��� ����â�� �̵��Ұ���
64: �Ⱓ�������� ����(�ð��� �귯��) �� �ŷ� �Ұ���
128: ĳ�Ø� �����ϱ� �Ұ���
256: ���â�� �̵��� �Ұ���
512: â�� �̵��� �Ұ���(���â��� �̵� ���������� â�� �̵��� �Ұ��� �Ұ��)
1024: �ٴڿ� ��� �Ұ��� & PK ��� ����
65535 : ��� �Ұ���*/

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
    eITEM_TRADESELL_ONLY_WAREHOUSE  = (1<<9),       //< 512     //_NA_20100520_ADD_ITEM_CANNOT_MOVE_OPTION : â�� �̵��� �Ұ��� �ɼ�
    eITEM_TRADESEEL_DROP_CAN_PKDROP = (1<<10),      //< 1024    //_NA_006052_20120904_CANNOT_DROP_CAN_PKDROP : �ٴڿ� ��� �Ұ��� �ϰ� PK ��� �� ����
    eITEM_TRADESELL_DONTALL			= ((WORD)-1),	//< 65535
};

enum eITEMTYPE
{
    eITEMTYPE_INVALID			= 0,	
    // ����
    eITEMTYPE_TWOHANDAXE		= 1,	//	��յ���1
    eITEMTYPE_TWOHANDSWORD		= 2,	//	��հ�1
    eITEMTYPE_ONEHANDSWORD		= 3,	//	�Ѽհ�1
    eITEMTYPE_SPEAR				= 4,	//	â1
    eITEMTYPE_DAGGER			= 5,	//	�ܰ�1
    eITEMTYPE_WHIP				= 6,	//	ä��1
    eITEMTYPE_ONEHANDCROSSBOW	= 7,	//	�Ѽռ���1
    eITEMTYPE_ETHERWEAPON		= 8,	//	��ռ���1,���׸�����
    eITEMTYPE_SCIMITAR			= 9,	//	�1
    eITEMTYPE_STAFF				= 10,	//	������1
    eITEMTYPE_ORB				= 11,	//	����1
    eITEMTYPE_FISHING_POLE		= 12,	//  ���ô�
    eITEMTYPE_PUNCH				= 13,	//	���ָ�
    eITEMTYPE_GUNTLET = 14, // ��Ʋ�� _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eITEMTYPE_POLEARM = 15, // ö�� _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eITEMTYPE_HELLROID_1 = 16, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eITEMTYPE_ARCBLADE = 17, // ��ũ���̵� _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    // ��
    eITEMTYPE_ARMOR				= 101,	//	����
    eITEMTYPE_PROTECTOR			= 201,	//	��������
    eITEMTYPE_HELMET			= 301,	//	���
    eITEMTYPE_PANTS				= 401,	//	����
    eITEMTYPE_BOOTS				= 501,	//	����
    eITEMTYPE_GLOVE				= 601,	//	�尩
    eITEMTYPE_BELT				= 701,	//	��Ʈ
    eITEMTYPE_SHIRTS			= 801,	//	����

    // �Ǽ�����
    eITEMTYPE_RING				= 811,	//	���� 
    eITEMTYPE_NECKLACE			= 821,	//	�����

    // Ư�� �Ǽ�����
    /*
    eITEMTYPE_CAPE				= 831,	//	����
    eITEMTYPE_WING				= 841,	//	����
    eITEMTYPE_STONE				= 851,	//	�Ӽ���
    eITEMTYPE_SPRITE			= 861,	//	��ȣ����, ���׷�����
    eITEMTYPE_CREST				= 871,	//	����
    */
    // Ŭ������ Ư�� �Ǽ������� ����
    eITEMTYPE_BERSERKER_SACCESSORY	= 831,	//	����
    eITEMTYPE_DRAGON_SACCESSORY		= 841,	//	����
    eITEMTYPE_VALKYRIE_SACCESSORY	= 851,	//	�Ӽ���
    eITEMTYPE_SHADOW_SACCESSORY		= 861,	//	��ȣ����, ���׷�����
    eITEMTYPE_ELEMENTALIST_SACCESSORY = 871,	//	����

    eITEMTYPE_BERSERKER_KALBUS_ACCESSORY	= 881,
    eITEMTYPE_DRAGON__KALBUS_ACCESSORY		= 882,
    eITEMTYPE_VALKYRIE__KALBU_ACCESSORY		= 883,
    eITEMTYPE_ELEMENTALIST_KALBUS_ACCESSORY = 884,
    eITEMTYPE_SHADOW_KALBU_ACCESSORY		= 888,

    eITEMTYPE_FISHING_ROD			= 885,
    eITEMTYPE_FISHING_BAIT			= 886,
    eITEMTYPE_FISH					= 887,

    eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY1 = 891, // �̽�ƽ Ư�� �Ǽ�����1 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eITEMTYPE_MYSTIC_SPECIAL_ACCESSORY2 = 892, // �̽�ƽ Ư�� �Ǽ�����2 _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eITEMTYPE_HELLROID_SPECIAL_ACCESSORY1 = 893, // ����̵� Ư�� �Ǽ�����1 _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eITEMTYPE_HELLROID_SPECIAL_ACCESSORY2 = 894, // ����̵� Ư�� �Ǽ�����2 _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY1 = 889, // ��ġ���̵� Ư�� �Ǽ�����1 _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eITEMTYPE_WITCHBLADE_SPECIAL_ACCESSORY2 = 890, // ��ġ���̵� Ư�� �Ǽ�����2 _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    
    eITEMTYPE_ALLCLASS_CHAOSEYE = 895,                     // Ÿ��Ȯ��(871) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_ELEMENTALIST_KALVUS_ACCESSORY = 899,         // Ÿ��Ȯ��(871) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_BERSERKER_KALVUS_ACCESSORY = 896,            // Ÿ��Ȯ��(831) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_DRAGON_KALVUS_ACCESSORY = 897,               // Ÿ��Ȯ��(841) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_VALKYRIE_SHADOW_KALVUS_ACCESSORY = 898,      // Ÿ��Ȯ��(851) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_MYSTIC_KALVUS_ACCESSORY = 900,               // Ÿ��Ȯ��(891) _NA_007827_20141117_ADD_ACCESSORY_TYPE
    eITEMTYPE_HELLROID_WITCHBLADE_KALVUS_ACCESSORY = 901,  //Ÿ��Ȯ��(893) _NA_007827_20141117_ADD_ACCESSORY_TYPE

    eITEMTYPE_WASTE					= 911,	//	�Ҹ𼺾�����
    eITEMTYPE_ENCHANT				= 921,	//	��þƮ����1
    eITEMTYPE_CRYSTAL				= 931,	//	����üA
    eITEMTYPE_SOCKET				= 941,	//	���Ͼ�����
    eITEMTYPE_ARROW					= 951,	//	���ÿ� ȭ��

    eITEMTYPE_SOCKETMATERIAL		= 961,	//	�������
    eITEMTYPE_ACCESSORYMATERIAL		= 971,	//	�Ǽ��������
    eITEMTYPE_SACCESSORYMATERIAL	= 981,	//	Ư���Ǽ��������

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
    eITEMTYPE_RANDOM				= 992,	// ����������(����/���/��Ÿ������)
    eITEMTYPE_TOTEM					= 993,	// ���۾�����
    eITEMTYPE_TOP_OF_TRIAL			= 994,	// ������ž ���� ������
    eITEMTYPE_TOP_OF_TRIAL_CHARGE	= 995,	// ������ž ���� ������(ĳ��������)
    eITEMTYPE_INDUN_LIMITED			= 994,	// �δ� ���� ������ (�ð� ���� ����)
    eITEMTYPE_INDUN_UNLIMITED		= 995,	// �δ� ���� ������ (�ð� ���� ����)
    eITEMTYPE_PORTAL_QUEST_ITEM		= 996,	// �ϴ� 100���� �뵵�̰� (����:���̵�+����Ʈ������)�̰�, ���� Ȯ���� �� �ֵ��� ����.
    eITEMTYPE_ETHER_BULLET			= 997,	// ���׸� źȯ  __NA_000968_ETHER_EMISSION_DEVICE
    eITEMTYPE_ETHER_DEVICE			= 998,	// ���׸� ��ġ	__NA_000968_ETHER_EMISSION_DEVICE
    eITEMTYPE_RENDER_EFFECT_ITEM	= 999,	// Inventory�� ���������� �ܺη� �������� Effect�� ������ ���
                                            // ����������(_NA_006372_20130108_BADGE_ITEM)
    eITEMTYPE_BILL					= 1000,	//	ȭ��
    eITEMTYPE_ETHERIA               = 1001, // ���׸��� _NA000000_100427_ITEM_CUSTOMIZING_CONTENTS_
    eITEMTYPE_EVENT_FIELD_PARTY_ALL_MEMBER_PICK_UP = 1002, // �ʵ� ��Ƽ ���� �����, �̺�Ʈ ������
    eITEMTYPE_ETHEREALIZER          = 1003, // ���׸��� �и� _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_USING_ETHEREALIZER_
    eITEMTYPE_WINGS                 = 1004, // _NA001955_110210_WING_COSTUME
    eITEMTYPE_DAILYQUEST_RESET      = 1005, // ��������Ʈ �ʱ�ȭ ������

    // ����ȭ ���� �߰� ������
    eITEMTYPE_SUPERMASTER			= 20000,	// ���۹���
    eITEMTYPE_PORTAL				= 20001,	// ��Ż�������
    eITEMTYPE_SAVE_COORD			= 20002,	// ��ǥ ���� ������
    eITEMTYPE_SOCKETEXTRACT			= 20003,	// �������ž�����
    eITEMTYPE_CHAT_SHOUT			= 20004,	// ��ġ�������(�Ⱓ��)
    eITEMTYPE_RECALL				= 20005,	// ���ͼ�ȯ������
    eITEMTYPE_REPAIR				= 20006,	// ����������������
    eITEMTYPE_HOLDDROP				= 20007,	// ��������������
    eITEMTYPE_STAT_RESET			= 20008,	// �����ʱ�ȭ������(��,��ø,ü��,����,���ŷ�,���õ�1,2,��ų����Ʈ)
    eITEMTYPE_EXTRATAB				= 20009,	// ���߰�������(���� 1�Ǿ� �߰�)
    eITEMTYPE_CLEAR_COORD			= 20010,	// ��ǥ �ʱ�ȭ ������
    eITEMTYPE_RECALL_ALLIANCE		= 20011,	// ������ ���� ������
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
    eITEMTYPE_GUILD_PVP_ENTRANCE	= 20012,	// ��� PVP �����
#endif
    eITEMTYPE_EXTEND_CASH_SLOT		= 20013,	// ĳ�� ���� 2�� Ȯ��
    eITEMTYPE_EXTEND_DATE_WEAPON	= 20014,	// �Ⱓ���� ������(����)	
    eITEMTYPE_RANK_DOWN				= 20015,	// ��ũ���� ��ġ
    eITEMTYPE_EXTEND_DATE_ARMOR		= 20016,	// �Ⱓ���� ������(��)	
    eITEMTYPE_HAMMER_OF_ALCHEMIST	= 20017,	// ���ݼ����� ��ġ(ALCHEMIST's hammer) ����Ƽ�� ������ ����
    eITEMTYPE_GUILDMARK_SEAL        = 20018,
    eITEMTYPE_HOLDDROP2				= 20019,	// ������ ��2  __NA_825_HOLDDROP2
    eITEMTYPE_PET					= 20020,	// �� __NA_0_PET_SYSTEM
    eITEMTYPE_PET_FEED				= 20021,	// �� __NA_0_PET_FEED

//_YJ_FISHING_SYSTEM_000933
    eITEMTYPE_FISH_HYBRID			= 20022,	//����   �����
    eITEMTYPE_FISH_WASTE			= 20023,	//�Ҹ� �����
    eITEMTYPE_FISH_HIGH_CLASS		= 20024,	//���   �����
    eITEMTYPE_FISH_RARE				= 20025,	//���   �����
    eITEMTYPE_FISH_AC				= 20026,	//AC     �����
    eITEMTYPE_FISH_CASH				= 20027,	//ĳ��   �����
//_YJ_FISHING_SYSTEM_000933
    eITEMTYPE_GUILDWAREHOUSE_SLOTCASH = 20028,	//��� â�� ���� Ȯ��
    eITEMTYPE_LOTTO					= 20029,	//���� ������
    eITEMTYPE_CHAT_SHOUT_CONSUME	= 20030,	

    eITEMTYPE_FIRECRACKER			= 20031,	
    eITEMTYPE_POLLEN				= 20032,	
    eITEMTYPE_TORCH					= 20033,	
    eITEMTYPE_MINE					= 20034,	
    eITEMTYPE_SSQ					= 20035,
    eITEMTYPE_STAT_CHANGE			= 20036, //������ ���� ��ȯ
    eITEMTYPE_PET_NAME_CARD			= 20037, //< �� �̸� ���� ������, __NA_1131_PET_NAME_SETTING

    eITEMTYPE_WAYPOINT				= 20038, // ��������Ʈ ������, __NA001164_20081024_CHAR_WAY_POINT_ITEM
    eITEMTYPE_CHANGE_GUILD_NAME     = 20039,

    //__NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    eITEMTYPE_RIDER                 = 20040, // Rider Item
    eITEMTYPE_RIDER_PARTS           = 20041, // Rider Parts Item { HEAD, BODY, LEGS }
    eITEMTYPE_EXTEND_DATE_RIDER     = 20042, // Extend date of time : Rider|Parts with timer
    eITEMTYPE_WAX                   = 20044, // Wax Item for rider parts item (shining enchant)

    // [11/2/2009 namka199] _CN_0_20091013_RANKUP_EXTEND
    eITEMTYPE_RANKUP_GRADE_RAISER   = 20045, // ��ũ���� �ʿ� ������(������ ��)
    eITEMTYPE_RANKUP_OPTION_CHANGER = 20046, // ��ũ���� �ɼ��� �������ִ� ������(�ʱ�ȭ�� ��)
    
    // _NA_000096_20100527_CHANGE_CHARACTER_APPEARANCE_SYSTEM
    eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL = 20047, // ĳ���� ��� �ܸ� ����
    eITEMTYPE_CHANGE_CHARACTER_FACE = 20048, // ĳ���� �� ����
    eITEMTYPE_CHANGE_CHARACTER_HAIR = 20049, // ĳ���� ��� ����
    eITEMTYPE_CHANGE_CHARACTER_HEIGHT = 20050, // ĳ���� Ű ����

    //_NA_0_20100722_AUTO_SYSTEM
    eITEMTYPE_AUTO_HUNTING   = 20051,      // �ڵ� ��� ������
    //_NA_0_20100727_SOKET_SYSTEM_CONVERT
    eITEMTYPE_SOCKET_ZARD	  = 20052,	            // new ���Ͼ����� (zard)
    eITEMTYPE_SOCKET_IDENTIFY = 20055,	            // ���� ���� Ȯ�� ������ 
    eITEMTYPE_SOCKET_COMPOSE_ADDITIVE = 20054,	    // new ���Ͼ����� ÷����

    //_NA002217_100728_EP2_PERK_SYSTEM_
    eITEMTYPE_PERK_CASH_ITEM   = 20053,      // ��ĳ�� ������

    // _NA_000587_20100928_DOMINATION_BUFF_ITEM
    eITEMTYPE_DOMINATION_BUFF = 20056, // ���� ���� ��ȭ ���� ������

    // _NA_20101011_HEIM_LOTTO_CASH_TICKET
    eITEMTYPE_HEIM_LOTTO_TICKET = 20057,            // ������� ĳ�� �����

    // _NA_002253_20100811_CUBE_SYSTEM
    eITEMTYPE_CUBE = 20058, // �������� �ռ� �Ǵ� �����ϴ� ť�� ������

    // _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    eITEMTYPE_DOMINATION_DEFFENSE_POINT = 20059,
    eITEMTYPE_DOMINATION_DEFFENSE_CASH_POINT = 20060,

    // _NA_002050_20110216_ADD_GENDER
    eITEMTYPE_CHANGE_CHARACTER_GENDER = 20061, // ĳ���� ���� ����
    eITEMTYPE_EXTEND_CHARACTER_SLOT = 20062, // ĳ���� ���� Ȯ��

    // _NA_003740_20111122_RANDOM_ITEM
    eITEMTYPE_RANDOMIZE_ITEM = 20063, // ������ ����ȭ
    
    // _NA_004035_20120227_COSTUME_ITEM
    eITEMTYPE_COSTUME = 20064, // �ڽ�Ƭ
    eITEMTYPE_PCBANG = 20065, // �����̾����� ������(PC��)

    eITEMTYPE_CHARACTER_NAME_CHANGE = 20066,
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION 
    eITEMTYPE_GUILDFACILITY_RESET = 20067,
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    eITEMTYPE_SPA_WARP_SCROLL = 20068, // ��õ �̵� ��ũ��
    eITEMTYPE_SPA_TICKET      = 20069, // ��õ �����
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    eITEMTYPE_AWAKENING_ALLROUND_MATERIAL = 20070, // ���� ���� ���
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    eITEMTYPE_MINIGAME_BINGO_BALL = 20071,          // �̴ϰ��Ӻ��� ��
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
    //_NA_008610_20160104_ADD_DECORATECOSTUME
    eITEMTYPE_COSTUME_DECORATE = 20072,             // �ٹ��ڽ�Ƭ

    eITEMTYPE_MAX,
};

enum eITEMSUBTYPE
{
    eITEMSUBTYPE_GENERAL		= 0,
    eITEMSUBTYPE_ELITE			= 1,
    eITEMSUBTYPE_UNIQUE			= 2,
    eITEMSUBTYPE_ELITE_SSQ      = 3, // (f100730.3L) add a type for ssq equipments which is equal to ELITE
    //eITEMSUBTYPE_DIVINE			= 3,	// eITEMSUBTYPE_DIVINE �� ������� �ʴ´�. ����ξ������� Ȯ���� SCItemSlot::IsDivine() �� �̿�
    eITEMSUBTYPE_RARE           = 4,
    eITEMSUBTYPE_DIVINE         = 5,    //_NA_007408_20140711_MODIFY_CRYSTALLIZE

    eITEMSUBTYPE_FATE			= 11,
};

// ������ ���� ������
struct ItemCustomLevel
{
    // changed rules by _NA_002128_20110228_MODIFY_ITEM_LEVEL
#ifdef _NA_002935_20110704_ITEM_SCORE
    enum Value
    {
        kMinLV = 1, // ��� ������ �ּ� ����
        kGeneralMaxLV = 21, // �Ϲ� ������ �ְ� ���� (��������)
        kSuperJapanLV = 21, // �Ϻ� �ְ�
        kCurruptTempleLV = 23, // Ÿ���� ���
		kFighter_Elite2_0LV = 25,
		kFighter_Elite3_0LV = 27, // ������ ����Ʈ 3��.

        kElite1_0stLV = 23, // ����Ʈ 1��
        kElite1_5stLV = 24, // ����Ʈ 1.5��
        kElite2_0stLV = 25, // ����Ʈ 2��
        kElite2_5stLV = 26, // ����Ʈ 2.5��
		kElite3_0stLV = 27, // ����Ʈ 3��
        kElite3_5stLV = 28, // ����Ʈ 3.5��
		kElite4_0stLV = 29, // ����Ʈ 4��

        kUnique1_5stLV = 31, // ����ũ 1.5��
        kUnique2_5stLV = 32, // ����ũ 2.5��
		kUnique1_0stLV = 33, // ����ũ 1��
        kUnique2_0stLV = 34, // ����ũ 2��
        
		kMaxLV = 39, // ��� ������ �ִ� ����
    };
#else
    enum Value
    {
        kMinLV = 1, // ��� ������ �ּ� ����
        kGeneralMaxLV = 17, // �Ϲ� ������ �ְ� ����
        kSuperJapanLV = 17, // �Ϻ� �ְ�
        kCurruptTempleLV = 21, // Ÿ���� ���
        kElite1_0stLV = 21, // ����Ʈ 1��
        kElite1_5stLV = 23, // ����Ʈ 1.5��
        kElite2_0stLV = 25, // ����Ʈ 2��
        kUnique1_0stLV = 31, // ����ũ 1��
        kUnique2_0stLV = 35, // ����ũ 2��
        kMaxLV = 39, // ��� ������ �ִ� ����
    };
#endif
};

// NOTE: f110414.x, declared by hukim, ������ �迭 �� Ÿ��
struct ItemSubTypeDetail
{
    enum Value
    {
        kInvalid = 0, // ��ȿ���� ���� Ÿ��
        kGeneral = 1, // �Ϲ�
        kSuperJapan = 2, // �Ϻ� �ְ�
        kCurruptTemple = 3, // Ÿ���� ���
        kElite1_0st = 4, // ����Ʈ 1��
        kElite1_5st = 5, // ����Ʈ 1.5��
        kElite2_0st = 6, // ����Ʈ 2��
        kUnique1_0st = 7, // ����ũ 1��
        kUnique2_0st = 8, // ����ũ 2��
        kFate = 9, // ��Ű
        kRare = 10, // ����
		kFigter_Elite2_0st = 11, // ���� ����Ʈ 2��
		kFigter_Elite3_0st = 12, // ���� ����Ʈ 3��

		kElite2_5st = 13, // ����Ʈ 2.5��
		kElite3_0st = 14, // ����Ʈ 3��
        kElite3_5st = 15, // ����Ʈ 3.5��
		kElite4_0st = 16, // ����Ʈ 4��

        kUnique1_5st = 17, // ����ũ 1.5��
        kUnique2_5st = 18, // ����ũ 2.5��
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
    eITEMWASTE_FUNCTION        = 3,    //< ��ɼ� ������
    eITEMWASTE_RETURNSCROLL    = 4,    //< ��ȯ ��ũ�� ������
    eITEMWASTE_QUESTACCEPT     = 5,    //< ����Ʈ ���� ��û ������
    eITEMWASTE_UPSTONE         = 6,    //< UP �÷� �ִ� ������
    eITEMWASTE_REVENGESTONE    = 7,    //< ������ ��
    eITEMWASTE_LEARN_SKILL     = 8,    //< ��ų����
    eITEMWASTE_STAT_POINT_UP   = 9,    //< ��������Ʈ �� ������
    eITEMWASTE_PET_FEED        = 10,   //< ����� __NA_0_PET_FEED
    eITEMWASTE_ETHER_BULLET    = 11,   //< ���׸� ���� ��ġ�� �Ѿ� __NA_000968_ETHER_EMISSION_DEVICE
    eITEMWASTE_GUILDWAREHOUSE_SLOTCASH = 12,   //< ��� Ȯ�� ���� 
    eITEMWASTE_EXP_POTION              = 13,   //< ����ġ ����
    eITEMWASTE_CHAO_TIME_DECREASE      = 14,   //< ī���ð� ����
    eITEMWASTE_FUNCTION_HOLDUP_BUFF_POST_DEAD = 15,   //< ��ɼ� ������ �׾ ������� �ʴ� ���� __NA001374_090708_CACHE_ITEM_BUFF_EXTENTION__    
    eITEMWASTE_ETHEREALIZER_SHELL = 16, // < #544, change to shell, declared by _NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
    eITEMWASTE_ETHEREALIZER_ETHERIA = 17, // < #544, change to etheria, declared by _NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
    eITEMWASTE_HPMP_POTION = 18, // HP/MP ȸ�� ����
    //_NA_0_20110613_GAME_SHOP_RENEWAL
    eITEMWASTE_EXTEND_DATE_WEAPON  = 19, //< �Ⱓ ���� ������(����).
    eITEMWASTE_EXTEND_DATE_ARMOR  = 20, //< �Ⱓ ���� ������(��).
    eITEMWASTE_EXTEND_DATE_RIDER  = 21, //< �Ⱓ ���� ������(Ż��).
    eITEMWASTE_EXTEND_DATE_RIDER_PARTS  = 22, //< �Ⱓ ���� ������(Ż���� ����).
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

// ������ ����
enum eITEMCOMPOSITECODE
{
    eITEMCOMPOSITE_NONE					= 0,	// �ƹ��͵� ���� ���� << Ŭ�󿡼� �ʿ��ؼ� �߰� ���� - �翵
    eITEMCOMPOSITE_LOW_ENCHANT			= 1,	// �ϱ���þƮ
    eITEMCOMPOSITE_MEDIUM_ENCHANT		= 2,	// �߱���þƮ
    eITEMCOMPOSITE_HIGH_ENCHANT			= 3,	// �����þƮ
    eITEMCOMPOSITE_RANK_DOWN			= 4,	// ��ũ�ٿ�
    eITEMCOMPOSITE_RANK_UP				= 5,	// ��ũ��
    eITEMCOMPOSITE_EXTRACT_SOCKET		= 6,	// ���Ͽɼ�����
    eITEMCOMPOSITE_ADD_SOCKET			= 7,	// �����߰�(�������� ���� �߰��Ѵٴ� �ǹ�)

    /**/
    // >> Ŭ���̾�Ʈ������ �ʿ�    
    // �Ǽ����� ����
    eITEMCOMPOSITE_LOW_RING				= 8,	// �ϱ޹��� >> ����
    eITEMCOMPOSITE_HIGH_RING			= 9,	// ��޹���
    eITEMCOMPOSITE_LOW_NECKLACE			= 10,	// �ϱ޸����
    eITEMCOMPOSITE_HIGH_NECKLACE		= 11,	// ��޸����
    //// �Ǽ����� ��� ����
    eITEMCOPOSITE_PROTECTION_CRYSTAL	= 12,
    eITEMCOPOSITE_SHANING_CRYSTAL		= 13,
    //// Ư�� �Ǽ����� ����
    eITEMCOPOSITE_SOUL_SHEILD_LOW		= 14,
    eITEMCOPOSITE_SOUL_SHEILD_HIGH		= 15,
    eITEMCOPOSITE_SOUL_ARMLET_LOW		= 16,
    eITEMCOPOSITE_SOUL_ARMLET_HIGH		= 17,
    eITEMCOPOSITE_SOUL_WING_LOW			= 18,
    eITEMCOPOSITE_SOUL_WING_HIGH		= 19,
    eITEMCOPOSITE_SOUL_EYE_LOW			= 20,
    eITEMCOPOSITE_SOUL_EYE_HIGH			= 21,	
    eITEMCOPOSITE_EXTRASTONE_CREATE		= 22,	// << ����
    eITEMCOPOSITE_EXTRASTONE_USE		= 23,
    eITEMCOPOSITE_CRYSTALIZE			= 24,	// ����ȭ
    eITEMCOPOSITE_ZARD			        = 25,	// ��� �ռ�
    eITEMCOPOSITE_TOTAL					= 31,
    eITEMCOMPOSITE_HEIM_ENCHANT         = 32,   // ������æƮ
    eITEMCOMPOSITE_CASH_ENCHANT         = 33,   // ĳ����æƮ
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
	eITEMCOMPOSITE_EQUIPMENT_WAKEUP     = 34,   // ����
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
	eITEMCOMPOSITE_EQUIPMENT_EVOLUTION  = 35,
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
    // << ������� Ŭ���̾�Ʈ���� �ʿ��ؼ� �����߽��ϴ�. - j0	
};

// 1BYTE �ɼ�[CG_ITEM_ENCHANT_SYN]
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
// ���� : �Ӽ��� enum ��ġ ���濡 ����!!!!!! +11
//--------------------------------------------------------------------------------------------------
enum eATTR_TYPE
{
    eATTR_TYPE_INVALID = 0,                         // ��ȿ���� �ʴ� �Ӽ�
    eATTR_STR = 1,                                  // ����(��)
    eATTR_DEX = 2,                                  // ����(��ø)
    eATTR_VIT = 3,                                  // ����(ü��)
    eATTR_INT = 4,                                  // ����(����)
    eATTR_SPR = 5,                                  // ����(���ŷ�)

    eATTR_EXPERTY1 = 6,                             // EP2:unused, ���õ�1
    eATTR_EXPERTY2 = 7,                             // EP2:unused, ���õ�2

    eATTR_CUR_HP = 8,                               // ���� HP
    eATTR_CUR_MP = 9,                               // ���� MP

    eATTR_MAX_HP = 10,                              // �ִ� HP
    eATTR_MAX_MP = 11,                              // �ִ� MP

    eATTR_RECOVERY_HP = 12,                         // HP ȸ����
    eATTR_RECOVERY_MP = 13,                         // MP ȸ����

//--------------------------------------------------------------------------------------------------
// ���ݷ� ���� �Ķ����
//--------------------------------------------------------------------------------------------------
    eATTR_BASE_MELEE_MIN_ATTACK_POWER,              // �⺻ ���� ���ݷ�
    eATTR_BASE_MELEE_MAX_ATTACK_POWER,
    eATTR_BASE_RANGE_MIN_ATTACK_POWER,
    eATTR_BASE_RANGE_MAX_ATTACK_POWER,
    eATTR_BASE_MAGICAL_MIN_ATTACK_POWER,            // EP2:unused, �⺻ ���� (�ּ�) ���ݷ�
    eATTR_BASE_MAGICAL_MAX_ATTACK_POWER,            // EP2:unused, �⺻ ���� (�ִ�) ���ݷ�

    eATTR_OPTION_PHYSICAL_ATTACK_POWER,             // [V] �ɼ� ���� ���ݷ�
    eATTR_OPTION_MAGICAL_ATTACK_POWER,              // EP2:unused
    eATTR_OPTION_ALL_ATTACK_POWER,                  // EP2:unused, �ɼ� ��ü ���� ���ݷ�

    eATTR_MAGICAL_WATER_ATTACK_POWER,               // [V] ����(��) ���ݷ�
    eATTR_MAGICAL_FIRE_ATTACK_POWER,                // [V] ����(��) ���ݷ�
    eATTR_MAGICAL_WIND_ATTACK_POWER,                // [V] ����(�ٶ�) ���ݷ�
    eATTR_MAGICAL_EARTH_ATTACK_POWER,               // [V] ����(����) ���ݷ�
    eATTR_MAGICAL_DARKNESS_ATTACK_POWER,            // [V] ����(����) ���ݷ�
    eATTR_MAGICAL_DIVINE_ATTACK_POWER,              // unused

    eATTR_MAGICAL_ALL_ATTACK_POWER,                 // [V] �����Ӽ� ��ü ���ݷ�, EP2={ FIRE, WATER, WIND, EARTH, DARK }

    eATTR_ADD_SKILL_ATTACK_POWER,                   // [V] ��ų �߰� ���ݷ�
    eATTR_ADD_SKILL_DAMAGE_RATIO,                   // [R] ��ų �ۼ�Ʈ �߰� ������

//--------------------------------------------------------------------------------------------------
// ���� ���� �Ķ����
//--------------------------------------------------------------------------------------------------
    eATTR_BASE_MELEE_DEFENSE_POWER,                 // [V] �⺻ (�ٰŸ�) ���� ����
    eATTR_BASE_RANGE_DEFENSE_POWER,                 // [V] �⺻ (���Ÿ�) ���� ����
    eATTR_BASE_MAGICAL_DEFENSE_POWER,               // EP2:unused, �⺻ ���� ����

    eATTR_OPTION_PHYSICAL_DEFENSE_POWER,            // �ɼ� ���� ����
    eATTR_OPTION_MAGICAL_DEFENSE_POWER,             // EP2:unused
    eATTR_OPTION_ALL_DEFENSE_POWER,                 // EP2:unused, �ɼ� ��ü ���� ���� <- �ɼ� ����+���� ����

    eATTR_MAGICAL_WATER_DEFENSE_POWER,              // EP2:unused, [V] ����(��) ����
    eATTR_MAGICAL_FIRE_DEFENSE_POWER,               // EP2:unused, [V] ����(��) ����
    eATTR_MAGICAL_WIND_DEFENSE_POWER,               // EP2:unused, [V] ����(�ٶ�) ����
    eATTR_MAGICAL_EARTH_DEFENSE_POWER,              // EP2:unused, [V] ����(����) ����
    eATTR_MAGICAL_DARKNESS_DEFENSE_POWER,           // EP2:unused, [V] ����(����) ����
    eATTR_MAGICAL_DIVINE_DEFENSE_POWER,             // unused

    eATTR_MAGICAL_ALL_DEFENSE_POWER,                // EP2:unused, [V] ���� ��ü ���� EP2={ FIRE, WATER, WIND, EARTH, DARK }

    eATTR_ADD_ALL_DEFENSE_POWER,                    // EP2:unused, [V] ���� Ÿ�Ժ� �߰� ����
    eATTR_ADD_MELEE_DEFENSE_POWER,                  // EP2:unused, [V] ���ݴ�� (����) �߰� ����
    eATTR_ADD_RANGE_DEFENSE_POWER,                  // EP2:unused, [V] ���ݴ�� (���Ÿ�) �߰� ����
    eATTR_ADD_WATER_DEFENSE_POWER,                  // EP2:unused, [V] ���ݴ�� (��) �߰� ����
    eATTR_ADD_FIRE_DEFENSE_POWER,                   // EP2:unused, [V] ���ݴ�� (��) �߰� ����
    eATTR_ADD_WIND_DEFENSE_POWER,                   // EP2:unused, [V] ���ݴ�� (�ٶ�) �߰� ����
    eATTR_ADD_EARTH_DEFENSE_POWER,                  // EP2:unused, [V] ���ݴ�� (����) �߰� ����
    eATTR_ADD_DARKNESS_DEFENSE_POWER,               // EP2:unused, [V] ���ݴ�� (����) �߰� ����
    eATTR_ADD_DIVINE_DEFENSE_POWER,                 // EP2:unused
    eATTR_ADD_PHYSICAL_DEFENSE_POWER,               // EP2:unused
    eATTR_ADD_MAGICAL_DEFENSE_POWER,                // EP2:unused
    eATTR_ADD_MAGICAL_ALL_DEFENSE_POWER,            // EP2:unused

    eATTR_DEL_ALL_TARGET_DEFENSE_RATIO,             // EP2:unused
    eATTR_DEL_MELEE_TARGET_DEFENSE_RATIO,           // EP2:unused, (����Ÿ��/�Ƹ�Ÿ�԰���)
    eATTR_DEL_RANGE_TARGET_DEFENSE_RATIO,           // EP2:unused, (����Ÿ��/�Ƹ�Ÿ�԰���)
    eATTR_DEL_WATER_TARGET_DEFENSE_RATIO,           // [R] ���ݴ�� ����(��) ���� ������
    eATTR_DEL_FIRE_TARGET_DEFENSE_RATIO,            // [R] ���ݴ�� ����(��) ���� ������
    eATTR_DEL_WIND_TARGET_DEFENSE_RATIO,            // [R] ���ݴ�� ����(�ٶ�) ���� ������
    eATTR_DEL_EARTH_TARGET_DEFENSE_RATIO,           // [R] ���ݴ�� ����(����) ���� ������
    eATTR_DEL_DARKNESS_TARGET_DEFENSE_RATIO,        // [R] ���ݴ�� ����(����) ���� ������
    eATTR_DEL_DIVINE_TARGET_DEFENSE_RATIO,          // EP2:unused
    eATTR_DEL_PHYSICAL_TARGET_DEFENSE_RATIO,        // [R] ���ݴ�� ���� ���� ������
    eATTR_DEL_MAGICAL_TARGET_DEFENSE_RATIO,         // EP2:unused
    eATTR_DEL_MAGICAL_ALL_TARGET_DEFENSE_RATIO,     // [R] ���ݴ�� ��ü ���� ���� ������

//--------------------------------------------------------------------------------------------------
// ������ ���� �Ķ����
//--------------------------------------------------------------------------------------------------
    eATTR_ADD_ARMOR_HARD_DAMAGE,                    // EP2:unused, �Ƹ� Ÿ�Ժ� �߰� ������
    eATTR_ADD_ARMOR_MEDIUM_DAMAGE,                  // EP2:unused
    eATTR_ADD_ARMOR_SOFT_DAMAGE,                    // EP2:unused
    eATTR_ADD_ARMOR_SIEGE_DAMAGE,                   // EP2:unused
    eATTR_ADD_ARMOR_UNARMOR_DAMAGE,                 // EP2:unused

    eATTR_ADD_RATIO_ARMOR_HARD_DAMAGE,              // EP2:unused, �Ƹ� Ÿ�Ժ� �߰� ������(�ۼ�Ʈ)
    eATTR_ADD_RATIO_ARMOR_MEDIUM_DAMAGE,            // EP2:unused
    eATTR_ADD_RATIO_ARMOR_SOFT_DAMAGE,              // EP2:unused
    eATTR_ADD_RATIO_ARMOR_SIEGE_DAMAGE,             // EP2:unused
    eATTR_ADD_RATIO_ARMOR_UNARMOR_DAMAGE,           // EP2:unused
    // CHANGES: changes value type to ratio type since EP2
    eATTR_DEL_ALL_DAMAGE,                           // EP2:unused, ���� ������
    eATTR_DEL_MELEE_DAMAGE,                         // EP2:unused, (����Ÿ��/�Ƹ�Ÿ�԰���)
    eATTR_DEL_RANGE_DAMAGE,                         // EP2:unused, (����Ÿ��/�Ƹ�Ÿ�԰���)
    eATTR_DEL_WATER_DAMAGE,                         // [R] ����(��)�� ���� ������ ������
    eATTR_DEL_FIRE_DAMAGE,                          // [R] ����(��)�� ���� ������ ������
    eATTR_DEL_WIND_DAMAGE,                          // [R] ����(�ٶ�)�� ���� ������ ������
    eATTR_DEL_EARTH_DAMAGE,                         // [R] ����(����)�� ���� ������ ������
    eATTR_DEL_DARKNESS_DAMAGE,                      // [R] ����(����)�� ���� ������ ������
    eATTR_DEL_DIVINE_DAMAGE,                        // EP2:unused
    eATTR_DEL_PHYSICAL_DAMAGE,                      // [R] EP2: ���� ���ݿ� ���� ������ ������
    eATTR_DEL_MAGICAL_DAMAGE,                       // EP2:unused
    eATTR_DEL_MAGICAL_ALL_DAMAGE,                   // [R] EP2: ����(���)�� ���� ������ ������

//--------------------------------------------------------------------------------------------------
// ��Ÿ �Ķ����
//--------------------------------------------------------------------------------------------------
    eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO,            // ���� ������(����)
    eATTR_PHYSICAL_ATTACK_BLOCK_RATIO,              // [R][ 15] ���� ����� (�ɼ�) �߰���
    eATTR_PHYSICAL_ATTACK_BLOCK_BASE_ARMOR_RATIO,   // EP2:added, ���� ����� ������ ���� (�� �ջ� / 5)

    eATTR_MOVE_SPEED,                               // �̵� �ӵ�
    eATTR_ATTACK_SPEED,                             // ���� �ӵ�

    eATTR_ALL_ATTACK_RANGE,                         // ��� ��Ÿ�
    eATTR_NORMAL_ATTACK_RANGE,                      // �Ϲ� ��Ÿ�
    eATTR_SKILL_ATTACK_RANGE,                       // ��ų ��Ÿ�

    eATTR_SIGHT_RANGE,                              // �þ�

    eATTR_CRITICAL_RATIO_CHANGE,                    // ũ��Ƽ�� Ȯ�� ����
    eATTR_ADD_MAGICAL_CRITICAL_RATIO,               // EP2:unused, ���� ũ��Ƽ�� Ȯ��
    eATTR_ADD_ALL_CRITICAL_RATIO,                   // EP2:unused, ��ü ũ��Ƽ�� Ȯ��
    eATTR_CRITICAL_DAMAGE_CHANGE,                   // ũ��Ƽ�� ������ �߰�

    eATTR_ADD_ATTACK_INC_RATIO,                     // [R] �߰� ���� �����
    eATTR_ADD_DEFENSE_INC_RATIO,                    // [R] �߰� ��� �����

    eATTR_INCREASE_SKILL_LEVEL,                     // ��ų ���� 1���
    eATTR_INCREASE_STAT_POINT,                      // ���� ����Ʈ(���)1���

    //  2006 ���� 1:23:11
    eATTR_DECREASE_LIMIT_STAT,                      // ���� ���� ��ġ ����
    eATTR_MP_SPEND_INCREASE,                        // MP �Һ� ����

    //  ���ȭ ������ �ɼ� �߰�
    eATTR_ABSORB_HP,                                // HP���
    eATTR_ABSORB_MP,                                // MP,SP ���
    eATTR_REFLECT_DAMAGE_RATIO,                     // ������ �ݻ�
    eATTR_BONUS_MONEY_RATIO,                        // ���� ����
    eATTR_BONUS_EXP_RATIO,                          // ����ġ ����
    eATTR_AREA_ATTACK_RATIO,                        // ���߰��� Ȯ��
    eATTR_BONUS_CASTING_TIME,                       // 65:ĳ���� Ÿ�� ����
    eATTR_BONUS_SKILL_COOL_TIME,                    // 75:[R] ��ų ��Ÿ�� ����
    eATTR_DECREASE_DAMAGE,                          // ������ ����

    eATTR_RESURRECTION_RATIO,                       // 52:����� ������ ��Ȱ �� Ȯ��
    eATTR_DOUBLE_DAMAGE_RATIO,                      // 53:�������� �ι谡 �� Ȯ��
    eATTR_LUCKMON_INC_DAMAGE,                       // 54:��Ű ���� ���ݽ� �߰� ������
    eATTR_COPOSITE_INC_RATIO,                       // 55:���� ������ ����
    eATTR_BYPASS_DEFENCE_RATIO,                     // 56:���� ���� Ȯ��
    eATTR_INCREASE_MIN_DAMAGE,                      // 57:�ּ� ������ ����
    eATTR_INCREASE_MAX_DAMAGE,                      // 58:�ִ� ������ ����
    eATTR_DECREASE_ITEMDURA_RATIO,                  // 59:������ ������ ���ҹ��� Ȯ��
    eATTR_RESISTANCE_BADSTATUS_RATIO,               // 60:��ų ȿ�� ����Ȯ��
    eATTR_INCREASE_SKILLDURATION,                   // 61:��ų ȿ�� �ð� ���� ( ���� �迭 )
    eATTR_DECREASE_SKILL_SKILLDURATION,             // 62:��ų ȿ�� �ð� ���� ( ����� �迭 ) < f110531.6L, changed from '����'
    eATTR_OPTION_ETHER_DAMAGE_RATIO,                // 63:���׸����� ������ ��ȭ
    eATTR_OPTION_ETHER_PvE_DAMAGE_RATIO,            // 76:���׸����� ������ ��ȭ (PvE��), f110601.4L

    eATTR_INCREASE_RESERVE_HP,                      // 64:���� HP ����

    eATTR_RESIST_HOLDING,                           // 66:Ȧ�� ����
    eATTR_RESIST_SLEEP,                             // 67:���� ����
    eATTR_RESIST_POISON,                            // 68:�ߵ� ����
    eATTR_RESIST_KNOCKBACK,                         // 69:�˹� ����
    eATTR_RESIST_DOWN,                              // 70:�ٿ� ����
    eATTR_RESIST_STUN,                              // 71:���� ����
    eATTR_DECREASE_PVPDAMAGE,                       // 72:PVP������ ����
    eATTR_ADD_DAMAGE,                               // 73:�߰�������
    eATTR_AUTO_ITEM_PICK_UP,                        // 74:Item �ڵ� �ݱ�
    // NOTE: regenerated index
    eATTR_MAX_SD,                                   // 93:�ִ� SD
    eATTR_RECOVERY_SD,                              // 94:SD ȸ����
    eATTR_CUR_SD,                                   // ���� SD

    // _NA_003966_20111227_ADD_ENCHANT
    eATTR_INCREASE_ENCHANT_RATIO, // 77:��þƮ ������ ����

    eATTR_PREMIUMSERVICE_PCBANG,                    // PC�� ȿ��

    //_NA_006731_20130521_ENCHANT_ADD_OPTION
    eATTR_ENEMY_CRITICAL_RATIO_CHANGE,                  // 96:�ǰ� �� ����� ũ��Ƽ�� Ȯ�� ����

    //_NA_006680_20130426_ITEM_OPTION_ADD_AND_MODIFY
    eATTR_ATTACK_DAMAGE_ABSORB_SD_RATIO,             // 98:���� ������ SD ��ȯ��
    eATTR_ATTACK_DAMAGE_ABSORB_HP_RATIO,             // 99:���� ������ HP ��ȯ��

    //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
    eATTR_ENEMY_CRITICAL_DAMAGE_CHANGE,           // 100:�ǰ� �� ����� ũ��Ƽ�� ������ ����
    
    //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    eATTR_CRAFT_COST_RATIO,                         // 101:���� ��� ����
    eATTR_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO,  // 102:���� ���н� ��� �Ҹ� ���� Ȯ�� ����
    eATTR_ENCHANT_COST_RATIO,                       // 103:��æƮ ��� ����
    eATTR_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO, // 104:��æƮ ���н� ������ �Ҹ�, �ٿ� ���� Ȯ�� ����
    eATTR_RECOVER_POTION_COOLTIME_RATIO,            // 105:ȸ�� ���� ��Ÿ�� ����
    eATTR_RECOVER_POTION_RECOVERY_RATIO,            // 106:ȸ�� ���� ȸ���� ����
    eATTR_QUEST_REWARD_EXP_RATIO,                   // 107:����Ʈ ���� ����ġ ����
    eATTR_MAX_DAMAGE_RATIO,                         // 108:�ִ� ������ �߻�Ȯ�� ����
    eATTR_DOMINATION_MAPOBJECT_DAMAGE_RATIO,        // 109:���� ������Ʈ ������ ����
    eATTR_SHOP_REPAIR_HEIM_RATIO,                   // 110:���� ���� ���� ����
    eATTR_SHOP_BUY_HEIM_RATIO,                      // 111:���� ���� ���� ����
    
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eATTR_MAX_FP,                                   // 112: ��ġ���̵� �ִ�FP
    eATTR_RECOVERY_FP,                               // 113: ��ġ���̵� FPȸ���� (���ݽ�)
    eATTR_INCREASE_DAMAGE_RATIO,                     // 114: ������ ���� 
    
    //_NA_008124_20150313_AWAKENING_SYSTEM
    eATTR_AWAKENING_PROBABILITY,                     // 115: ���� Ȯ�� ����

    //_NA_008486_20150914_TOTAL_BALANCE
    eATTR_DEBUFF_DURATION,                          // 116: �ڽ��� �������� ȿ�� ���ӽð� ��ȭ(�и�������)
    
    //_NA_008540_20151027_ADD_ITEMOPTION_ELITE4
    eATTR_DECREASE_DAMAGE_NPC,                      // 117: npc�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_BERSERKER,                // 118: ����Ŀ�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_DRAGONKNIGHT,             // 119: �巡�ﳪ��Ʈ�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_VALKYRIE,                 // 120: ��Ű���� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_ELEMENTALIST,             // 121: ������Ż����Ʈ�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_SHADOW,                   // 122: �����찡 �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_MYSTIC,                   // 123: �̽�ƽ�� �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_HELLROID,                 // 124: ����̵尡 �ִ� ������ ����
    eATTR_DECREASE_DAMAGE_WITCHBLADE,               // 125: ��ġ���̵尡 �ִ� ������ ����
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

//todo:���̵� fix �ȵǾ��� 
//ĳ���� ���� ��Ÿ���� �ٸ���.
//��Ÿ�ϰ������ ���� ���ϸ��̼��� �ڵ�ȭ �Ҽ� �ִ� �����....

enum eSTYLE
{
    
    eSTYLE_BERSERKER_PUNCH			= 60000, // �Ǽհ��� ��Ÿ�� 
    eSTYLE_DRAGON_PUNCH				= 60199, // �Ǽհ��� ��Ÿ�� 
    eSTYLE_SHADOW_PUNCH             = 60400,
    eSTYLE_VALKYRIE_PUNCH			= 60600, // �Ǽհ��� ��Ÿ�� 
    eSTYLE_MAGICIAN_PUNCH			= 60800, // �Ǽհ��� ��Ÿ�� 
   
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
    // ����Ŀ 
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // ���� 1
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
    kSkillHardStance = 67, // �ϵ� ���Ľ� (���� ü��)
    kSkillIronStance = 68, // ���̾� ���Ľ� (���� ü��)

    // ���� 2
    eSKILL_BASH			            = 101,
    kSkillSPRecovery = 102,	// SP ��Ŀ����
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
    kSkillSPRecharge = 119, // SP ������
    kSkillBloodHaze = 120, // ���� ������ (���� HP)
    kSkillMotalBlood = 121, // ��Ż ���� (���� HP)

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // �巡�� ����Ʈ
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // ���� 1
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
    kSkillCourageOfKnights = 268, // Ŀ���� ���� ������ (���� ��ø)
    kSkillFeverOfKnights = 269, // �ǹ� ���� ������ (���� ��ø)

    // ���� 2
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
    kSkillDragonicForce = 324, // �巡��� ���� (���� ���ݷ�)
    kSkillDragonicBless = 325, // �巡��� ���� (���� ���ݷ�)

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // ��Ű�� 
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // ���� 1
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
    kSkillIncreaseMind = 471, // ��ũ���� ���ε� (���� ����)
    kSKillMindSpark = 472, // ���ε� ����ũ (���� ����)

    // ���� 2
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
    kSkillSummonicShield = 523, // ���Ӵ� �ǵ� (���� ����)
    kSkillSummonicBarrier = 524, // ���Ӵ� ������ (���� ����)

    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // ������Ż����Ʈ 
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // �нú�
    eSKILL_FIRE_MASTERY             = 601,
    eSKILL_WATER_MASTERY            = 602,
    eSKILL_WIND_MASTERY             = 603,
    eSKILL_EARTH_MASTERY            = 604,
    eSKILL_MANA_REGENERATION        = 605,

    // ���� 1
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
    kSkillEnchantPower = 670, // ��æƮ �Ŀ� (���� ��)
    kSkillCatalystPower = 671, // īŻ����Ʈ �Ŀ� (���� ��)

    // ���� 2
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
    kSkillWhisperOfWind = 732, // ������ ���� ���� (���� �̵��ӵ�)
    kSkillWhisperOfSylph = 733, // ������ ���� ���� (���� �̵��ӵ�)


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // ������
    ////////////////////////////////////////////////////////////////////////////////////////////////

    // ���� 1
    eSKILL_PAIN						= 800, // ���� (����)
    eSKILL_DARK_FIRE				= 801, 
    eSKILL_DARK_SPARK				= 802, 
    eSKILL_ARMOR_INCREASE			= 803, // �Ƹ���ũ���� (����/���� ���� ����, �������׷� ���)
    eSKILL_SOUL_CONTROL				= 804, // �ҿ���Ʈ�� (��ȥ����)
    eSKILL_IMPRTENT					= 805, // ������Ʈ (�������� ����)
    eSKILL_VITAL_SUCTION			= 806, // ����Ż���� (�������)
    eSKILL_ENCHANT_POISON			= 807, // ��þƮ������ (���� �ٸ� ��)
    eSKILL_HP_SUCTION				= 808, // HP���� (HP���)
    eSKILL_MP_SUCTION				= 809, // MP���� (MP���)		// �̻��
    eSKILL_CURSE_INCREASE			= 810, // Ŀ����ũ���� (���ְ�ȭ)
    eSKILL_DARK_PAIN				= 811, // ��ũ���� (�����ǰ���)
    eSKILL_BUFF_CANCEL				= 812, // ����ĵ�� (��ȭ��������)
    eSKILL_CONFUSE					= 813, // ��ǻ�� (ȥ��������)
    eSKILL_DEMON					= 814, // ���� (�Ƿ�)
    eSKILL_DARK_FORCE				= 815, // ��ũ���� (�������)
    kSkillHelronsSoul = 816, // ����� �ҿ� (���� ���ŷ�)
    kSkillDemonsSoul = 817, // ������ �ҿ� (���� ���ŷ�)

    // ���� 2
    eSKILL_WHIP_ATTACK				= 850, // �۾��� (�ķ�ġ��)
    eSKILL_POISON_DAGGER			= 851, // �������� (���̹������)
    eSKILL_DOUBLE_WIDTH				= 852, // �������� (�ι�Ⱦ���κ���)
    eSKILL_DARK_STUN				= 853, // ��ũ���� (�ָ�)
    eSKILL_HIDE						= 854, // ���̵� (����)
    eSKILL_DEATH_BLOW				= 855, // �ٽ���ο� (ġ��Ÿ)
    eSKILL_SHADOW_SPEAR				= 856, // �����콺�Ǿ� (���)
    eSKILL_CANCELLATION				= 857, // ĵ�����̼� (���������)
    eSKILL_RUSH						= 858, // ���� (��������)
    eSKILL_SUDDEN_STRIKE			= 859, // ���罺Ʈ����ũ (���)
    eSKILL_DARK_DUST				= 860, // ��ũ����Ʈ (�����ǰ���, ����ε�)
    eSKILL_FAST_BLOW				= 861, // �н�Ʈ��ο� (�÷�ġ��)
    eSKILL_DOWN_SLASH				= 862, // �ٿ���� (�ٿ������)
    eSKILL_SOUL_SCREAM				= 863, // �ҿｺũ�� (��ȥ������)
    eSKILL_WHIRLWIND_BLOW			= 864, // �������ο�
    eSKILL_DARK_SLASH				= 865, // ����ǳ�����
    eSKILL_DARK_BREAK				= 866, // ��ũ���ũ (����)
    kSkillDarkTrace = 867, // ��ũ Ʈ���̽� (���� ���ݼӵ�)
    kSkillDarkRage = 868, // ��ũ ������ (���� ���ݼӵ�)

    ////////////////////////////////////////////////////////////////////////////////////////////////
    // �̽�ƽ
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

	// ����� ��ų
	eSKILL_MYSTIC_DARKOFLIGHT_ZONE  = 937,
	eSKILL_MYSTIC_GRAVITY_ZONE      = 938,	


    ////////////////////////////////////////////////////////////////////////////////////////////////
    // ����̵�
    ////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eSKILL_HELLROID_1000            = 1000,     // ��������ġ
    eSKILL_HELLROID_1001            = 1001,     // ũ������
    eSKILL_HELLROID_1002            = 1002,     // �����
    eSKILL_HELLROID_1003            = 1003,     // ���ű
    eSKILL_HELLROID_1004            = 1004,     // ���ο����
    eSKILL_HELLROID_1005            = 1005,     // �帱����
    eSKILL_HELLROID_1006            = 1006,     // ��ũ�ʽ�����
    eSKILL_HELLROID_1007            = 1007,     // ���ɺ�ο�
    eSKILL_HELLROID_1008            = 1008,     // �ö�����¡
    //eSKILL_HELLROID_1009            = 1009,     // �̻��
    eSKILL_HELLROID_1010            = 1010,     // ��Ʈ�극��ũ
    eSKILL_HELLROID_1011            = 1011,     // �̽�æ������
    eSKILL_HELLROID_1012            = 1012,     // ������ü����
    eSKILL_HELLROID_1013            = 1013,     // ��Ŭ���ο���
    eSKILL_HELLROID_1014            = 1014,     // �κ��̴����
    eSKILL_HELLROID_1015            = 1015,     // ����̵彺��

    eSKILL_HELLROID_1016            = 1016,     // ����
    eSKILL_HELLROID_1017            = 1017,     // ���׸���¡
    eSKILL_HELLROID_1018            = 1018,     // �ο��ο�
    eSKILL_HELLROID_1019            = 1019,     // ��Ʈ����Ʈ��ġ
    eSKILL_HELLROID_1020            = 1020,     // ��Ŀ��������
    eSKILL_HELLROID_1021            = 1021,     // �����̺�
    eSKILL_HELLROID_1022            = 1022,     // �ν�Ʈ����
    eSKILL_HELLROID_1023            = 1023,     // �����ͺ�
    eSKILL_HELLROID_1024            = 1024,     // ���������̾�
    eSKILL_HELLROID_1025            = 1025,     // ����̵��
    eSKILL_HELLROID_1026            = 1026,     // �����̵�����
    eSKILL_HELLROID_1027            = 1027,     // �̱׳�����
    eSKILL_HELLROID_1028            = 1028,     // ���׸�������
    eSKILL_HELLROID_1029            = 1029,     // ����Ʈ���̼ǿ���
    eSKILL_HELLROID_1030            = 1030,     // �����ǵ�
    eSKILL_HELLROID_1031            = 1031,     // �극��ũ�

    // ����� ��ų
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
    // ��ġ���̵�
    ////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eSKILL_WITCHBLADE_1100           = 1100,      // ȣ��������
    eSKILL_WITCHBLADE_1101           = 1101,      // �ʹ�ű
    eSKILL_WITCHBLADE_1102           = 1102,      // �ǵ�·�����
    eSKILL_WITCHBLADE_1103           = 1103,      // �����Ͼ�����Ʈ
    eSKILL_WITCHBLADE_1104           = 1104,      // Ʈ����Ʈ�÷�ű
    eSKILL_WITCHBLADE_1105           = 1105,      // ���̿÷�ũ�ν�
    eSKILL_WITCHBLADE_1106           = 1106,      // ���Ͻ�ũ��ġ
    eSKILL_WITCHBLADE_1107           = 1107,      // ������
    eSKILL_WITCHBLADE_1108           = 1108,      // ������
    eSKILL_WITCHBLADE_1109           = 1109,      // �巡���
    eSKILL_WITCHBLADE_1110           = 1110,      // ���Ǵ�����
    eSKILL_WITCHBLADE_1111           = 1111,      // ���̽��ҵ�
    eSKILL_WITCHBLADE_1112           = 1112,      // �̺��̵�
    eSKILL_WITCHBLADE_1113           = 1113,      // ��ũ�����׷��̺�
    eSKILL_WITCHBLADE_1114           = 1114,      // ǻ�������̼�
    eSKILL_RISINGFORCE               = 1115,      // ����¡����
    eSKILL_WITCHBLADE_1116           = 1116,      // ��������

    eSKILL_WITCHBLADE_1117           = 1117,      // ����_��������ũ
    eSKILL_WITCHBLADE_1118           = 1118,      // ũ�ν��޺���̼�
    eSKILL_WITCHBLADE_1119           = 1119,      // ���̺�����
    eSKILL_WITCHBLADE_1120           = 1120,      // ������
    eSKILL_WITCHBLADE_1121           = 1121,      // �����̾��ġ
    eSKILL_WITCHBLADE_1122           = 1122,      // ���ӳ��Ŀ����
    eSKILL_WITCHBLADE_1123           = 1123,      // ���̵�Ŀư
    eSKILL_WITCHBLADE_1124           = 1124,      // ����_ƼŸ�Ͼ�
    eSKILL_WITCHBLADE_1125           = 1125,      // ���ν�Ʈ����
    eSKILL_WITCHBLADE_1126           = 1126,      // �����Ż
    eSKILL_WITCHBLADE_1127           = 1127,      // ����_����
    eSKILL_WITCHBLADE_1128           = 1128,      // ����������
    eSKILL_WITCHBLADE_1129           = 1129,      // ����ξƮ
    eSKILL_WITCHBLADE_1130           = 1130,      // ����_��������
    eSKILL_WITCHBLADE_1131           = 1131,      // ������ε�
    eSKILL_WITCHBLADE_1132           = 1132,      // ���׳�ƽ�׶�����
    eSKILL_WITCHBLADE_1133           = 1133,      // ��ư����Ʈ
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    // �нú�
    eSKILL_DAGGER_MASTERY			= 900,
    eSKILL_WHIP_SWORD_MASTERY		= 901,
    eSKILL_SHADOW_FORCE				= 902,
    eSKILL_CURSE_TRAINING			= 903,
    eSKILL_DARK_TRAINING			= 904,
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // �÷��̾� ��ų ��
    ////////////////////////////////////////////////////////////////////////////////////////////////

    
    eSKILL_CREATE_CRYSTAL_WARP		= 1400,
    eSKILL_DESTROY_CRYSTAL_WARP     = 1401,
    
    // ����
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

    // 07.3�� 15�� ���̿�Ʈ ���� ��ų�߰�1 - ��ȯ
    eSKILL_FELLEN_WARRIOR_DASH_ATTACK			= 1784,
    eSKILL_FELLEN_WARRIOR_XDANCE				= 1785,
    eSkILL_CURSED_ARCHER_POISON_ARROW			= 1787,
    
    // 07.4�� 5�� ���̿�Ʈ ���� ��ų �߰�2 - ��ȯ
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

    // 07.4�� 30�� �ӹ��Ǳ��, ���ָ��л��� ��ų �߰�
    eSKILL_KNIGHT_OF_SLAVERY_BINDING			= 1803,
    
    //07.6�� 20�� �Ŵ��� ���,�������� ��ų �߰�
    eSKILL_GIIANTIC_CROW_SCRATCH				= 1833,
    eSKILL_GIANNTIC_CROW_DIVE					= 1834,
    eSKILL_DUMPED_MAN_BLIDING					= 1835,
    eSKILL_DUMPED_MAN_DREIN_RANGER				= 1836,

    eSKILL_KNIGHT_OF_HUNGRY_SLAYER_THUNDER_CLAP = 1904,
    eSKILL_BINDED_ARCHER_STUN_SHOT				= 1905,

    //�ʻ繰 ������Ʈ 
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
    eACTION_TRADE_IN_CHAT_DLG	   = 15, //��ųâ �̵��
    eACTION_FRIEND_ADD_IN_CHAT_DLG = 16, //��ųâ �̵��

    eACTION_CRYSTAL_WARP_CREATE	   = 17,
    eACTION_CRYSTAL_WARP_DESTROY   = 18,
    eACTION_CRYSTAL_WARP_EXCUTE	   = 19,
    eACTION_ETHERIA_EXTRACT 	   = 20,
    eACTION_IDENTIFY_SOCKET        = 21, //����Ȯ��
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    eACTION_RANKING_SYSTEM_LOAD    = 22,
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    eACTION_PERK_GROUP_CHANGE      = 23,
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    eACTION_ITEM_CRYSTALIZATION    = 24, //�����۰���ȭ
    eACTION_START_SKILL_CODE	   = 35000,
};


enum eABILITY
{
    eABILITY_INVALID								= 0,

    //-------------------------------------------------------------------------------------------------
    // ��ġ ����
    //-------------------------------------------------------------------------------------------------

    eABILITY_DAMAGE									= 1,	// ��ų ������
    eABILITY_DAMAGE_PER_TIME						= 2,	// �ð��� ������

    eABILITY_MAX_HP_INCREASE						= 3,	// �ִ� HP ����
    eABILITY_CUR_HP_INCREASE						= 4,	// ���� HP ����
    eABILITY_RECOVER_HP_INCREASE					= 5,	// HP ȸ���� ����
    eABILITY_EXHAUST_HP								= 6,	// HP ����

    eABILITY_MAX_MP_INCREASE						= 7,	// �ִ� MP ����
    eABILITY_CUR_MP_INCREASE						= 8,	// ���� MP ����
    eABILITY_RECOVER_MP_INCREASE					= 9,	// MP ȸ���� ����
    eABILITY_EXHAUST_MP								= 10,	// MP ����

    eABILITY_PHYSICAL_ATTACKPOWER_INCREASE			= 11,	// ���� ���ݷ� ����
    eABILITY_ATTACKPOWER_BY_ARMORTYPE				= 12,	// EP2:unused, �߰� ���ݷ�(�Ƹ�Ÿ�Ժ�)
    eABILITY_MAGIC_ATTACKPOWER_INCREASE				= 13,	// ���� ���ݷ� ����

    eABILITY_PHYSICAL_DEFENSE_INCREASE				= 14,	// ���� ���� ����
    eABILITY_DEFENSE_BY_ATTACKTYPE					= 15,	// EP2:unused, �߰� ����(����Ÿ�Ժ�)
    eABILITY_MAGIC_DEFENSE_INCREASE					= 16,	// EP2:unused, ���� ���� ����

    eABILITY_STAT_INCREASE							= 17,	// ���� ����

    eABILITY_PHYSICAL_ATTACKRATE_INCREASE			= 18,	// ���� ���� ������ ����
    eABILITY_PHYSICAL_AVOIDRATE_INCREASE			= 19,	// ���� ���� ȸ���� ����

    eABILITY_MOVE_SPEED_INCREASE					= 20,	// �̵� �ӵ� ����
    eABILITY_PHYSICAL_SPEED_INCREASE				= 21,	// ���� ���� �ӵ� ����

    eABILITY_CASTING_TIME_INCREASE					= 22,	// ĳ���� Ÿ�� ����

    eABILITY_DAMAGE_DECREASE						= 23,	// ������ ����

    eABILITY_SKILLRANGE_INCREASE					= 24,	// ��ų ��Ÿ� ����

    eABILITY_CRITICAL_RATIO_CHANGE				    = 25,	// (�ڽ� Ȥ�� �ǰ��� ������) ũ��Ƽ�� Ȯ�� ����
    eABILITY_CRITICAL_DAMAGE_CHANGE				    = 26,	// (�ڽ� Ȥ�� �ǰ��� ������) ũ��Ƽ�� ������ ����

    eABILITY_AGGROPOINT_INCREASE					= 27,	// ��׷� ����Ʈ ����
    
    eABILITY_SKILLDAMAGE_INCREASE					= 28,	// ��ų ������ ����(CHANGUP_INTENSIFY_SKILLDAMAGE_STATUS)

    //-------------------------------------------------------------------------------------------------
    // ���� ����
    //-------------------------------------------------------------------------------------------------

    eABILITY_STUN_STATUS							= 100,	// Stun ���� �߻�
    eABILITY_ABNORMAL_STATUS						= 101,	// �̻� ���� �߻�
    eABILITY_WIND_SHIELD							= 102,	// ���彯��
    eABILITY_BUFF_RANGE_DAMAGE						= 103,	// ���� ���� ������(��������, ĳ���� �ֺ��� �ð��� ������)

    eABILITY_LOWHP_ATTACKPOWER_CHANGE				= 104,	// EP2:unused, HP ���Ͻ� ���ݷ� ����
    eABILITY_LOWHP_DEFENSE_CHANGE					= 105,	// EP2:unused, HP ���Ͻ� ���� ����
    eABILITY_BONUS_DAMAGE_PER_ATTACK				= 106,	// ���ݴ� �߰� ������

    eABILITY_ATTACK_DAMAGE_HP_ABSORPTION			= 107,	// ���� ������ HP ���
    eABILITY_ATTACK_DAMAGE_MP_ABSORPTION			= 108,	// ���� ������ MP ���
    eABILITY_ATTACKED_DAMAGE_HP_ABSORPTION			= 109,	// �ǰ� ������ HP ���
    eABILITY_ATTACKED_DAMAGE_MP_ABSORPTION			= 110,	// �ǰ� ������ MP ���

    eABILITY_MAGIC_SHIELD							= 111,	// ���� ����

    eABILITY_FEAR									= 112,	// �Ǿ�

    eABILITY_REFLECT_DAMAGE							= 113,	// �ǰ� ������(������ �ݻ�)
    eABILITY_REFLECT_STUN							= 114,	// �ǰ� ����(������ + ����ൿ�Ҵ�)
    eABILITY_REFLECT_FEAR							= 115,	// �ǰ� �Ǿ�(������ + ����)
    eABILITY_REFLECT_FROZEN							= 116,	// �ǰ� ������(������ + ����ൿ�Ҵ�)

    eABILITY_REFLECT_SLOW							= 117,	// �ǰ� ����(������ + �̼�����)
    eABILITY_REFLECT_SLOWDOWN						= 118,	// �ǰ� ��ȭ(������ + ��������)

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //-------------------------------------------------------------------------------------------------
    // ��� �ü� �����Ƽ
    //-------------------------------------------------------------------------------------------------
    eABILITY_CRAFT_COST_RATIO                       = 150,  // ���� ��� ����
    eABILITY_CRAFT_PREVENT_EXTINCTION_MATERIAL_RATIO= 151,  // ���� ���н� ��� �Ҹ� ���� Ȯ�� ����
    eABILITY_ENCHANT_COST_RATIO                     = 152,  // ��æƮ ��� ����
    eABILITY_ENCHANT_PREVENT_DESTROY_N_DOWNGRADE_ITEM_RATIO = 153,  // ��æƮ ���н� ������ �Ҹ�, �ٿ� ���� Ȯ�� ����
    eABILITY_RECOVER_POTION_COOLTIME_RATIO          = 154,  // ȸ�� ���� ��Ÿ�� ����
    eABILITY_RECOVER_POTION_RECOVERY_RATIO          = 155,  // ȸ�� ���� ȸ���� ����
    eABILITY_QUEST_REWARD_EXP_RATIO                 = 156,  // ����Ʈ ���� ����ġ ����
    eABILITY_MAX_DAMAGE_RATIO                       = 157,  // �ִ� ������ �߻� Ȯ�� ����
    eABILITY_MONEY_RAIO                             = 158,  // ���� ȹ�淮 ����
    eABILITY_DOMINATION_MAPOBJECT_DAMAGE_RATIO      = 159,  // ���� ������Ʈ ������ ����
    eABILITY_SHOP_REPAIR_HEIM_RATIO                 = 160,  // ���� ���� ���� ����
    eABILITY_SHOP_BUY_HEIM_RATIO                    = 161,  // ���� ���� ���� ����
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    //-------------------------------------------------------------------------------------------------
    eABILITY_MONSTER_KILL_EXP_RATIO                 = 162,  // ���� óġ ����ġ ����
    //-------------------------------------------------------------------------------------------------
    // ��Ÿ
    //-------------------------------------------------------------------------------------------------

    eABILITY_STATUS_HEAL							= 203,	// ���� ġ��
    eABILITY_RESURRECTION							= 204,	// ��Ȱ
    eABILITY_WEAPON_MASTERY							= 205,	// ���� �����͸�
    eABILITY_KNOCKBACK								= 206,	// ���
    eABILITY_FIGHTING_ENERGY_NUM_INCREASE			= 207,	// ���� ���� ����
    eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY		= 208,	// ����� ������ �߰�
    eABILITY_BONUS_DAMAGE_PER_SP					= 209,	// SP�� �߰� ���ݷ�
    eABILITY_BONUS_DAMAGE_PER_STATUS				= 210,	// ���º� �߰� ������ �߻�
    eABILITY_TELEPORT								= 212,	// �ڷ���Ʈ

    eABILITY_DRAGON_TRANSFORMATION1					= 213,	// �巡�ﺯ��1
    eABILITY_DRAGON_TRANSFORMATION2					= 214,	// �巡�ﺯ��2(���ʽ� �ɷ�ġ)
    eABILITY_DRAGON_TRANSFORMATION3					= 216,	// �巡�ﺯ��3(���ʽ� �ɷ�ġ)

    eABILITY_TARGET_CHANGE							= 215,	// ��׷β���

    eABILITY_RANDOM_AREA_ATTACK						= 217,	// ������������
    eABILITY_SUMMON_TO_DIE							= 218,	// ��ȯ�ؼ� ���̱�

    eABILITY_MONSTER_TRANSFORMATION					= 219,	// ���� ����

    eABILITY_SUMMON									= 220,	// ��ȯ
    eABILITY_SUMMON_CHANGE_STATUS					= 221,  // ��ȯ�� ��ȯü�� �ɷ�ġ�� �÷��̾��� �ɷ�ġ�� ����.

    eABILITY_PIERCE_ARROW							= 223,	// �Ǿ �ַο�

    eABILITY_SKILL_COOL_TIME_INCREASE				= 225,  // ��ų ��Ÿ�� ����
    eABILITY_MP_SPEND_INCREASE						= 226,  // ��ų ���� MP�� ����

    eABILITY_KNOCKBACK2								= 227,	// KNOCKBACK 2nd - __NA001048_080514_APPEND_ABILITY_KNOCKBACK2__
    eABILITY_ADRENALINE								= 228,	// �Ƶ巹����
    eABILITY_SUMMONMASTERY							= 229,  // ���ո����͸�
    eABILITY_SELF_DESTRUCTION						= 230,	// ����
    // __NA_S00015_20080828_NEW_HOPE_SHADOW
    eABILITY_BLOCK_SKILL							= 301,	// ��ų�� ����.(��ǻ��)
    eABILITY_ATTR_DEFENSIVE_POWER					= 302,	// EP2:���� ����� ���� <- EP1:�Ӽ����� ����
    eABILITY_INCREASE_SKILL_ABILITY					= 303,	// ��ų�ɷ�����.( Ŀ����ũ���� )
    eABILITY_SUMMON_DEAD_MONSTER					= 304,	// ���� ���ͼ�ȯ
    eABILITY_VITAL_SUCTION							= 305,	// �������
    eABILITY_ENCHANT_POISON							= 306,	// ��þƮ������
    eABILITY_SUCTION_HPMP							= 307,	// HP, MP���
    eABILITY_DARK_STUN								= 308,	// ��ũ����
    eABILITY_HIDE									= 309,	// ���̵�.
    eABILITY_DARK_BREAK								= 310,	// ��ũ�극��ũ.
    eABILITY_ATTR_ATTACK_POWER						= 311,	// �Ӽ� ���ݷ� ����.
    eABILITY_CANCEL_STATUS							= 312,	// ��������.
    eABILITY_CHANGE_ATTR							= 313,	// �Ӽ��� ����.
    // end of shadow
    eABILITY_RECOVERY_CHANCE						= 314,	// ������ HP ���� ����.
    //{_NA_001231_20081216_ADD_OPTION_KIND
    eABILITY_RESIST_HOLDING							= 315,	// Ȧ�� ����.
    eABILITY_RESIST_SLEEP   						= 316,	// ���� ����.
    eABILITY_RESIST_POISON   						= 317,	// �ߵ� ����.
    eABILITY_RESIST_KNOCKBACK						= 318,	// �˹� ����.
    eABILITY_RESIST_DOWN   							= 319,	// �ٿ� ����.
    eABILITY_RESIST_STUN   							= 320,	// ���� ����.
    eABILITY_DECREASE_PVPDAMAGE						= 321,	// PVP����� ����.
    //}
    //eABILITY_RESIST_ATTACK						= 322,	// unused, ���� ����. __NA_001244_20090417_ATTACK_RESIST
    //__NA_001290_20090525_SHIELD_SYSTEM
    eABILITY_MAX_SD_INCREASE						= 323,	// �ִ� SD ����
    eABILITY_RECOVER_SD_INCREASE					= 324,	// SD ȸ���� ����
    //_NA001385_20090924_DOMINATION_SKILL
    eABILITY_SUMMON_CRYSTALWARP 					= 325,	// ũ����Ż ����(����ȯ��)��ȯ
    eABILITY_SUMMON_CRYSTALWARP_DESTROY				= 326,	// ũ����Ż ����(����ȯ��)��ȯ ����
    eABILITY_TOTEM_NPC_OPTION_EMPOWERMENT           = 327, // TotemNpc �ɼ� �ο�

    kAbilityIncreseHeal = 328, // ġ���� ����
    kAbilityActiveComboSkill = 329, // ���� ��ų Ȱ��ȭ
    kAbilityComboSkillEffectAddDamage = 330, // ���� ��ų �ߵ� ȿ�� - ������ �߰�
    kAbilityComboSkillEffectAddBadStatus = 331, // ���� ��ų �ߵ� ȿ�� - �����̻� Ȯ��/���ӽð� ����
    kAbilityActiveIncreseSkillDamage = 332, // ��ų ������ ���� ���� �ߵ�
    kAbilityIncreseSkillDamage = 333, // ��ų ������ ����

    eABILITY_DETECTING_HIDE                         = 334, // ����ü ����
    eABILITY_PULLING                                = 335, // ����(�˹��� �ݴ�)
    //eABILITY_BLIND                                = 336, // �Ǹ����  => eABILITY_ABNORMAL_STATUS
    //eABILITY_GRAVITY                              = 337, // �߷� ���� => eABILITY_ABNORMAL_STATUS
    //eABILITY_POLYMORPH                            = 338, // ������ ������Ű�� ���� => eABILITY_ABNORMAL_STATUS
    eABILITY_ROLLING_BOMB                           = 339, // �Ѹ���
    eABILITY_QUICKSTEP                              = 340, // ���� �̵�
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    eABILITY_FUGITIVE                               = 341, // ���� ������ ����
#endif
    eABILITY_CUR_SD_INCREASE                        = 343, // ����SD����
    eABILITY_SKILL_STATE_IGNORE                     = 344, // ��ų���¹���
#ifdef _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
    eABILITY_VARIATION_PUREVALUE_HP                 = 345, // ��ų���ݷ�,����,���׷�,��������,SD� ��������ʴ� ü�� ����
#endif //_NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
    //_NA_007667_20141001_WITCHBLADE_SKILL
    eABILITY_AURORA                                 = 347, // ���ζ� ��ų
    eABILITY_FP_CHANGE                              = 348, // FP �߰�/�Ҹ� (1ȸ��)
    eABILITY_FP_TOGGLE                              = 349, // FP �߰�/�Ҹ� (�ֱ⼺)

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eABILITY_CHARMED          						= 351,	// ��Ȥ (�����ڸ� ������� �Ѵ�)
    eABILITY_REFLECT_LINK_SKILL                     = 352,  // �ǰݽ� ��ų���(������ + ��ų)
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eABILITY_INCREASE_DAMAGE                        = 353,  // ������ ����
    eABILITY_AUTOCAST_BYATTACK                      = 354,  // �������� ����
    eABILITY_SKILL_CONDITION                        = 355,  // ��ų ���ǰ˻�� �����Ƽ
    eABILITY_OVERLAP_STATE                          = 356,  // ��ø �����Ƽ
    eABILITY_SUMMON_IMMOVABLE                       = 357,  // ������ ��ȯ��(��ġ���̵�)
    eABILITY_SUMMON_CHANGE_STATUS_EXTEND            = 358,  // ��ȯ���� �ɷ�ġ �Ϻ�, ��ȯ���� �޾ƿ�, 221�����Ƽ�� Ȯ������Ƽ

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    eABILITY_AWAKENING_PROBABILITY                  = 359,  // ���� Ȯ�� ����
#endif // _NA_008124_20150313_AWAKENING_SYSTEM
    eABILITY_MAX
};

// ġ���� ���� ����
struct IncreseHealAbilityType
{
    enum Value
    {
        kSkill = 1, // ��ų ���ÿ��� ����
        kItem = 2, // ������ ���ÿ��� ����
        kSkillAndItem = 3 // ��ų�� ������ �Ѵ� ���� ����
    };
private:
    IncreseHealAbilityType();
};

// VALUE_TYPE_VALUE => �Ӽ��� m_ItemValue, m_SkillValue ������ �Ҵ�.
// VALUE_TYPE_PERCENT_PER_CUR, VALUE_TYPE_PERCENT_PER_MAX
// => �Ӽ��� m_ItemRatio, m_SkillRatio ������ �Ҵ�.
// ( �����Ƽ�� ����Ŀ� ���� 2,3������ ��� 
// ex) HP�� ���� ����, MaxHP�� ���� ���� )

// VALUE_TYPE_RATIO_VALUE => �Ӽ��� m_ItemValue, m_SkillValue ������ �Ҵ�.
// ( Ȯ�� ��꿡�� ���Ǵ� ���ϰ�� m_ItemValue, m_SkillValue ������ �� ���� �����Ƿ�, 
//   updata�� m_CalcValue���� �׻� 0�̴� )
enum eVALUE_TYPE
{
    VALUE_TYPE_VALUE				= 1,	// ���� ��ġ
    VALUE_TYPE_PERCENT_PER_CUR		= 2,	// ����ġ ��� ����
    VALUE_TYPE_PERCENT_PER_MAX		= 3,	// �ִ�ġ ��� ����
    VALUE_TYPE_RATIO_VALUE			= 5,	// ��ġ�� �������� ���(�̵��ӵ���)
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

    eCHAR_EVENT_DEAD,					// �׾���.
    eCHAR_EVENT_RESURRECTION,			// ��Ƴ���.
};

enum eSKILL_USER_TYPE
{
    eSKILL_USER_PLAYER		= 0,		// �÷��̾�
    eSKILL_USER_MONSTER,				// ����
    eSKILL_USER_NPC,					// �Ϲ� NPC
    eSKILL_USER_SUMMONED,				// ��ȯü
    eSKILL_USER_FOLLOWER,				// �뺴
    eSKILL_USER_ACTION,					// �׼�
    eSKILL_USER_EMOTICON,				// �̸�Ƽ��
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

// ������ ĳ���� Ÿ���̱� ������ ������ ó��
enum eCHAREX_TYPE
{
    eCHAREX_100LEVEL_CHANGEUP_CONSTANT = 10,
    //
    eCHAREX_DRAGON_1stTRANS = eTRANSFORM_TYPE_DRAGON1,      // �巡�ﳪ��Ʈ ���� 1��
    eCHAREX_DRAGON_2ndTRANS = eTRANSFORM_TYPE_DRAGON2,      // �巡�ﳪ��Ʈ ���� 2��

    eCHAREX_BERSERKER_100LEVEL     = eCHAR_BERSERKER + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // ����Ŀ 100���� ü������ �ڵ�
    eCHAREX_DRAGON_KNIGHT_100LEVEL = eCHAR_DRAGON + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // �巡�ﳪ��Ʈ 100���� ü������ �ڵ�
    eCHAREX_SHADOW_100LEVEL        = eCHAR_SHADOW + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // ������ 100���� ü������ �ڵ�
    eCHAREX_VALKYRIE_100LEVEL      = eCHAR_VALKYRIE + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // ��Ű�� 100���� ü������ �ڵ�
    eCHAREX_ELEMENTALIST_100LEVEL  = eCHAR_ELEMENTALIST + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // ������Ż����Ʈ 100���� ü������ �ڵ�
    eCHAREX_MYSTIC_100LEVEL        = eCHAR_MYSTIC + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // �̽�ƽ 100���� ü������ �ڵ� //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eCHAREX_HELLROID_100LEVEL      = eCHAR_HELLROID + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // ����̵� 100���� ü������ �ڵ� //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    eCHAREX_WITCHBLADE_100LEVEL    = eCHAR_WITCHBLADE + eCHAREX_100LEVEL_CHANGEUP_CONSTANT, // ��ġ���̵� 100���� ü������ �ڵ� //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

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

// ���� ����
struct GenderType
{
    enum Value
    {
        kAll = 0, // ���
        kMale = 1, // ����
        kFemale = 2 // ����
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
    eNPC_CLASS_CENTIPEDE,   // ���׸���
    eNPC_CLASS_LIFE_SOUL			= 50,
    eNPC_CLASS_WAR_BATTLER,
    eNPC_CLASS_DESTROYER,
    eNPC_CLASS_MAGIC_WALKER,
    eNPC_CLASS_DOUBLE_GORE,
};

// ��ų ���� ����
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
//	GM_FORCE_DISCONNECT,						// GM�� ������ ������ ���� �����Ų��.
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
    eATTACK_RESIST_SKILL_DOT_DAMAGE,    //DoT������ ��ų (���� ���ػ���)
    eATTACK_RESIST_MAX,
};

enum eATTACK_TYPE
{
    eATTACK_TYPE_ALL_OPTION     = 0,    // ��� �ɼ�
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    eATTACK_TYPE_INVALID        = 0,    // EP2, don't use anymore 'eATTACK_TYPE_ALL_OPTION'
#endif
    //
    eATTACK_TYPE_MELEE          = 1,    // ����(�ٰŸ�)
    eATTACK_TYPE_RANGE          = 2,    // ����(���Ÿ�)
    eATTACK_TYPE_WATER          = 3,    // ��
    eATTACK_TYPE_FIRE           = 4,    // ��
    eATTACK_TYPE_WIND           = 5,    // �ٶ�
    eATTACK_TYPE_EARTH          = 6,    // ����
    eATTACK_TYPE_DARKNESS       = 7,    // ����
    eATTACK_TYPE_DIVINE         = 8,    // EP2:unused, �ż�
    eATTACK_TYPE_PHYSICAL_OPTION= 9,    // ��� ����
    eATTACK_TYPE_MAGIC_OPTION   = 10,   // ��� ����
    eATTACK_TYPE_ALL_MAGIC      = 11,   // EP2:unused, ��� �����Ӽ�
    eATTACK_TYPE_MAGIC          = 12,   // EP2:unused, ����
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    eATTACK_TYPE_INVALID        = 13,   // same as eATTACK_TYPE_NONE
#endif
#ifdef _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    eATTACK_TYPE_SKILL           = 13,   // ��ų����
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
    eATTR_APPLY_TYPE_NONE     = 0,    // Ÿ�� ����
    eATTR_APPLY_TYPE_NORMAL       = 1,    // �Ϲ�����
    eATTR_APPLY_TYPE_DAMAGED    = 2,    // �ǰݽ� ����
    eATTR_APPLY_TYPE_MAX       
};
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL

// EP2: unused
enum eMELEE_TYPE
{
    eMELEE_TYPE_NOT		    = 0,	// Ÿ�Ծ���(���ݾ���)
    eMELEE_TYPE_SLASH		= 1,	// ����
    eMELEE_TYPE_PIERCE,				// ���
    eMELEE_TYPE_HIT,				// Ÿ��
    eMELEE_TYPE_MAGIC,				// ����(����, ������)

    eMELEE_TYPE_MAX
};

enum eSTAT_TYPE
{
    eSTAT_TYPE_STR			= 1,	// ��
    eSTAT_TYPE_DEX,					// ��ø��
    eSTAT_TYPE_VIT,					// ü��
    eSTAT_TYPE_SPR,					// ���ŷ�
    eSTAT_TYPE_INT,					// ����
};

enum eRANGE_TYPE
{
    eRANGE_TYPE_ALL,		// ��� ��Ÿ�
    eRANGE_TYPE_NORMAL,		// �Ϲ� ��Ÿ�
    eRANGE_TYPE_SKILL,		// ��ų ��Ÿ�
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
    eNPC_GENERAL			=  1,	//  1 : �Ϲ�
    eNPC_ELITE				=  2,	//  2 : ����
    eNPC_LEADER				=  3,	//  3 : ����
    eNPC_MIDDLEBOSS			=  4,	//  4 : �ߺ���
    eNPC_BOSS				=  5,	//  5 : ����
    eNPC_MERCHANT_NPC		=  6,	//  6 : NPC(����)
    eNPC_SUMMON_NPC			=  7,	//  7 : NPC(��ȯü)
    eNPC_MAPOBJECT_NPC		=  8,	//  8 : NPC(�ʿ�����Ʈ)
    eNPC_LUCKY_MONSTER		=  9,	//  9 : ��Ű����
    eNPC_TOTEM				= 10,	// 10 : (WAVERIX)(080911)(REORDERING) Ȯ�� ��� Ŭ�� ����� (targeting�� ���� �ʵ��� �ϱ� ����... ¼��...)
    eNPC_SSQ_MONSTER		= 11,	// 11 : Ÿ���ѻ�� ���� __NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__	
    eNPC_CRYSTAL_WARP		= 12,
    eNPC_DOMINATION_MAPOBJECT_NPC =  13, //  13 : ����������_�ʿ�����Ʈ_�Ǿƽĺ����� (���������� �̿ܿ��� ����� �� �����ϴ�)
                                         //	 Ŭ���̾�Ʈ���� �ش� ������Ʈ ó�� ����
                                         //		- ���̵�,��,���� ���ϸ��̼� ����
                                         //		- ���α�� ����(�Ǿƽĺ�: ���� ���� ����, Ÿ�� â ��� ����)
                                         //     - ������ Ÿ������ �־ ���� ����(�⺻������ ���������������� ������ ��Ȳ�� ��� ������ �� �����ϴ�)
                                         //		- �ı��Ǿ����� �̴ϸʿ� �ı� ������ ���(�ı��Ǳ� ���� �̴ϸʿ� �ƹ�ǥ�õ� ���� ����)
    //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    eNPC_FRIEND_MAPOBJECT_NPC = 14, //  14 : �Ϲ� ���Ϳʹ� ������, Player�� ��ȣ ���� �� ������Ʈ   
    eNPC_FRIEND_MONSTER       = 15, //  15 : �Ϲ� ���Ϳʹ� ������, Player�� ��ȣ�� ������Ʈ
    eNPC_MOVE_SYNC_MERCHANT   = 16, //  16 : ��Ÿ� �̵� NPC���� ���� Ÿ��(������ ��ġ ����ȭ) 
                                    //       - ��ȹ���� �ʿ����.
    eNPC_GRADE_MAX		   = 17,
};

enum eNPC_MOVE_ATTITUDE			// ������ �̵�����
{
    eMOVE_ATTITUDE_WANDER			= 0,	// �̵���
    eMOVE_ATTITUDE_ETERNAL_STOP		= 1,	// ���� ������
    eMOVE_ATTITUDE_SPAWN_STOP		= 2,	// ���� ������(���� ��Ÿ���� �������� ����)
    eMOVE_ATTITUDE_PATROL			= 3,	// ��Ʈ��
    eMOVE_ATTITUDE_SEARCH_AREA		= 4,	// Ư�� ������ �켱������ �˻��Ѵ�.
    eMOVE_ATTITUDE_PATHLIST 		= 5,	// ��� �̵��� (������ ��� 1ȸ �̵�)   //_NA_0_20100222_UNIT_TRIGGERS_PATHLIST
    eMOVE_ATTITUDE_FOLLOW           = 6,    // ���� �Ʊ� ���󰡱�
    eMOVE_ATTITUDE_ETERNAL_STOP_NO_ATTACK = 7,    // ���� ����(���� ����)   //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
                                                  // dead�� ������� ����. : 0�� ���·� dead ���� ó���ǰ�, �ʵ忡�� ������Ʈ ���� ���� �ʴ´�.
#ifdef _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
    eMOVE_ATTITUDE_ATTACK_GROUND    = 8,    // ���� �׶��� ��(�̵���� �������� ���� �������� ����)
#endif // _NA_007907_20141222_MODIFY_MONSTER_MOVE_TRIGGER
    eMOVE_ATTITUDE_MAX
};


enum eATTACK_ATTITUDE
{
    ATTACK_ATTITUDE_AFTER				= 1,	// �İ�
    ATTACK_ATTITUDE_NEARES_FIRST		= 2,	// ����(�Ÿ�)
    ATTACK_ATTITUDE_LOW_HP_FIRST		= 3,	// ����(ü��)
    ATTACK_ATTITUDE_LOW_LEVEL_FIRST		= 4,	// ����(����)
    ATTACK_ATTITUDE_HIGH_MP_FIRST		= 5,	// ����(����)
    ATTACK_ATTITUDE_ONE_TARGET			= 6,	// �ܼ�����
    ATTACK_ATTITUDE_NO_ATTACK			= 7,	// ���ݾ��� //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
    ATTACK_ATTITUDE_TYPE_MAX
};

//_NA_0_20100222_UNIT_TRIGGERS_REGEN
// ������ �ƴ� ��� �Ϲ����´�. ���� 0,1�� ��Ŀ� �ߺ� ������ ��ȯ�����Ӽ�(2)�� �߰�

#define REGEN_TYPE_NOMAL   0x00   // ������ NPC Code �Ѹ����� ������ ���� Ÿ�ӿ� ��ȯ�Ѵ�.
#define REGEN_TYPE_RANDOM  0x01   // �������� NPC Code �� �Ѹ����� ������ ���� Ÿ�ӿ� ��ȯ�Ѵ�.
#define REGEN_TYPE_TRIGGER 0x02   // Ʈ���ſ� ���� ��ȯ ������ ��������.


// ��ų ��� ��� ����Ʈ : ���� ���⿡ ���� �� �߰��Ѵٸ� m_bySkillEffect�� ũ�⸦ �����ؾ��Ѵ�!!!
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
    SKILL_TYPE_STYLE,				// �Ϲ� 3Ÿ ����
    SKILL_TYPE_NORMAL,				// �Ϲ� 1, 2Ÿ ����
    SKILL_TYPE_NORMAL_AREA,			// �Ϲ� ��������
    //
    SKILL_TYPE_UPPERBOUND           // no count, max number of types
};

enum eSKILL_FACTOR_TYPE
{
    SKILL_FACTOR_NOMAL,             // �Ϲ����� Ŭ���̾�Ʈ�� ��ų ���
    SKILL_FACTOR_ITEM,              // �������� �̿��� ��ų ���( ��ų ������ ������ �ʴ´�. ��ų ��Ÿ�Ӿ��� -> ������ ��Ÿ��)
    SKILL_FACTOR_DOMINATION,        // ������������� ��ų (��ų ������ ������ �ʴ´�.  ��ų ��Ÿ�Ӿ��� -> ��������� ��Ÿ��)
    SKILL_FACTOR_EFFECT,            // ����Ʈ�� ����ϴ� ��ų(��ų��Ÿ�� ����)
};

// ������Ŷ�ʵ忭�� : 4��Ʈ
enum eATTACK_PROPENSITY	
{										
    ATTACK_PROPENSITY_NORMAL	= 0,	//�Ϲ� ����
    ATTACK_PROPENSITY_FORCE,			//���� ����
};

enum eUSER_RELATION_TYPE
{
    USER_RELATION_NEUTRALIST	= 0,		//�߸� : ���� �Ұ�, ���� �Ұ�
    USER_RELATION_FRIEND,					//�Ʊ� : ���� ����, ���� �Ұ�
    USER_RELATION_ENEMY,					//���� : ���� �Ұ�, ���� ����
};
//#endif

enum eABILITY_RANGE_TYPE
{
    SKILL_ABILITY_NONE                  = 0,    // f110309.1L, can't select
    SKILL_ABILITY_ENEMY					= 1,	// 1 : �� �ܼ�
    SKILL_ABILITY_FRIEND,						// 2 : �Ʊ� �ܼ�
    SKILL_ABILITY_TARGETAREA_ENEMY,				// 3 : ���������� ����
    SKILL_ABILITY_TARGETAREA_FRIEND,			// 4 : ���������� �Ʊ�
    SKILL_ABILITY_MYAREA_ENEMY,					// 5 : �ڱ��߽ɳ� ����
    SKILL_ABILITY_MYAREA_FRIEND,				// 6 : �ڱ��߽ɳ� �Ʊ�
    SKILL_ABILITY_ME,							// 7 : �ڱ��ڽ�
    SKILL_ABILITY_FIELD,						// 8 : �ʵ� ��ǥ
    SKILL_ABILITY_CORPSE_RESURRECTION,			// 9 : ��Ȱ�� ���� ��ü(��Ƽ�� ��ü)
    SKILL_ABILITY_SUMMON,						// 10 : ��ȯ(Ư�� Ÿ���� ���� ���)
    SKILL_ABILITY_CORPSE_FRIEND,				// 11 : �Ʊ� ��ü(��Ƽ���� �ƴ϶� �������)
    SKILL_ABILITY_CORPSE_RESURRECTION_MYAREA,	// 12 : ��ü( ��Ȱ���̸�, �ڽŰ� ��Ƽ���� ���� )
    SKILL_ABILITY_SUMMONED_MONSTER,				// 13 : ��ȯ�� ����
    SKILL_ABILITY_PLAYER_ENEMY,					// 14 : ���� �ܼ� �÷��̾�
    SKILL_ABILITY_MYAREA_CORPSE_ENEMY,			// 15 : �ڱ��߽ɳ� ���� ��ü.
    SKILL_ABILITY_CORPSE_ENEMY,					// 16 : ���� ��ü.
    SKILL_ABILITY_MAX
};

enum eSKILL_TARGET_TYPE
{
    SKILL_TARGET_NONE					= 0,
    SKILL_TARGET_ENEMY,							// 1 : ��
    SKILL_TARGET_FRIEND,						// 2 : �Ʊ�
    SKILL_TARGET_ME,				            // 3 : �ڽ�
    SKILL_TARGET_AREA,				            // 4 : ���� 
    SKILL_TARGET_FRIEND_CORPSE,					// 5 : ��Ƽ���� ��ü
    SKILL_TARGET_REACHABLE_ENEMY,				// 6 : ��(Ÿ�����ϱ����ؼ� �����ʿ�)
    SKILL_TARGET_REACHABLE_FRIEND,				// 7 : �Ʊ�(Ÿ�����ϱ����ؼ� �����ʿ�)
    SKILL_TARGET_REACHABLE_ME,					// 8 : �ڽ�(Ÿ�����ϱ����ؼ� �����ʿ�)
    SKILL_TARGET_SUMMON,						// 9 : Ÿ�پ���(��ȯ)
    SKILL_TARGET_ENEMY_PLAYER,					// 10 : ���÷��̾�
    SKILL_TARGET_ENEMY_CORPSE,					// 11 : �� ��ü.
    SKILL_TARGET_AREA_ENEMY_CORPSE,				// 12 : ���� �� ��ü.
    SKILL_TARGET_ENEMY_AND_ME,					// 13 : ��( ��ų�������� �ڱ��ڽ� )
    SKILL_TARGET_MAX
};

enum eNPC_STATE
{
    eNPC_STATE_HELP_RESPONSE,	// �����û�� �����ϴ� ����
};

enum eCHAR_CONDITION
{
    eCHAR_CONDITION_STANDUP,	// ����
    eCHAR_CONDITION_SITDOWN,	// �ɱ�

    eCHAR_CONDITION_MAX,
};

#define CHAR_ACTION_CONDITION_NONE      0x00	    
#define CHAR_ACTION_CONDITION_MOVING    0x01	    // �̵���
#define CHAR_ACTION_CONDITION_FIGHTING  0x02	    // ������

enum eCHAR_EMOTION
{
    eEMOTION_BOW,			// �λ�
    eEMOTION_LEAVE,			// �ۺ�
    eEMOTION_QUESTION,		// �ǹ�
    eEMOTION_SIGH,			// �Ѽ�
    eEMOTION_CHEER,			// ȯȣ
    eEMOTION_DENIAL,		// �ź�
    eEMOTION_CONSENT,		// �³�
    eEMOTION_ATTACK,		// ����
    eEMOTION_THANKS,		// ����
    eEMOTION_APOLOGY,		// ���
    eEMOTION_KISS,			// Ű��
    eEMOTION_LAUGH,			// ����
    eEMOTION_CRYING,		// ����
    eEMOTION_WELCOME,		// ȯ��
    eEMOTION_HANDCLAP,		// �ڼ�
    eEMOTION_AWE,			// ���
    eEMOTION_BEST,			// �ְ�
    eEMOTION_SULKINESS,		// ��ħ
    eEMOTION_PROVOCATION,	// ����
    eEMOTION_SLEEP,			// ����
    eEMOTION_POSE,			// ����
    eEMOTION_DANCE,			// ��
    eEMOTION_GAG,			// ����
    eEMOTION_MIMICRY,		// �䳻
    eEMOTION_SHOCK,			// ���

    eEMOTION_START_CODE		= 36000,	// �̸�Ƽ�� ���� ��ų �ڵ�
};

// ��ų(��Ÿ��) ���� ���� ����
enum eSKILL_AREA_TYPE
{
    SRF_FOWARD_ONE		= 1,
    SRF_FOWARD_120,
    SRF_FOWARD_160,
    SRF_FOWARD_360,
    SRF_PIERCE,
    SRF_AREA_POSITION,		// Ÿ���� ���� ��ǥ�� �ִ� ��ų�̴�.
    SRF_MAX
};

enum eNPC_TYPE
{
    eNPC_TYPE_NONE				= 0,
    eNPC_TYPE_STORE				= 1, // ����	
    eNPC_TYPE_BANK				= 2, //	â��	
    eNPC_TYPE_MAKE_ZONE			= 3, //	�����	
    eNPC_TYPE_ITEM_MIX			= 4, // ���չװ���ȭ
    eNPC_TYPE_GUARD				= 5, // ���
    eNPC_TYPE_VILLAGE_PORTAL	= 6, // ���� �̵� npc
    eNPC_TYPE_GUILD				= 7, // ��������
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
    eNPC_TYPE_GUILD_EVENT		= 8, // ��� �̺�Ʈ
#endif
    eNPC_TYPE_QUEST				= 14,
    eNPC_TYPE_EVENT				= 15,	// �̺�Ʈ npc(���ݼ��� ����,)
    eNPC_TYPE_MISSION_RANK		= 16,	// �̼Ƿ�ŷ npc
    eNPC_TYPE_VENDOR_LIST		= 17,   // ���� ����Ʈ NPC
    eNPC_TYPE_POINT_RANK		= 18,   // ����Ʈ��ŷ NPC
    eNPC_TYPE_EVENT_DONATION	= 19,   //��� ���� �̺�Ʈ npc(������,)
    eNPC_TYPE_EVENT_PERIOD_ITEM	= 20,	//�ַ�,Ŀ�� �̺�Ʈ npc(������,)
    eNPC_TYPE_EVENT_SANTA		= 21,   //�̺�Ʈ npc(��Ÿ)
    eNPC_TYPE_EVENT_BUFF		= 22,
    eNPC_TYPE_WARP				= 23,
    eNPC_TYPE_FISHING			= 24,
    eNPC_TYPE_LOTTO				= 25,	//�ζ� npc
    eNPC_TYPE_EVENT_FRIENDINVITATION	= 26,	//ģ�� ��õ �̺�Ʈ ������ ���� NPC, _KR_0_20091021_FIRENDINVITATION_EXTEND
    eNPC_TYPE_AIRSHIP_MOVE = 27, // ����� �̵� NPC
    eNPC_TYPE_PROTECTS = 28, // ��ȣ��� NPC
    eNPC_TYPE_SMARTNPC          = 29,   // ������NPC _NA_000000_20140120_SMART_NPC_SYSTEM
};

// ������ �������δ� 
// ���������� �ʴ� �÷��̾��� �ൿ ���¸� ��Ÿ��
enum ePLAYER_BEHAVE_STATE
{
    PLAYER_BEHAVE_IDLE_STATE,				//< �⺻��
    PLAYER_BEHAVE_TRADE_STATE,				//< �ŷ� ����
    PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE,
    PLAYER_BEHAVE_VENDOR_OBSERVER_STATE,
    PLAYER_BEHAVE_NPCSHOP_STATE,
#ifdef _DH_UNMOVEABLE_CLICKED_NPC
#else
    PLAYER_BEHAVE_PARTY_THINKING_LEAVE,     //< ��Ƽ�� ���� ���ΰ� �����
    PLAYER_BEHAVE_PARTY_THINKING_JOIN,      //< ��Ƽ�ʴ뿡 ���� ���ΰ� �����
#endif//_DH_UNMOVEABLE_CLICKED_NPC
    PLAYER_BEHAVE_SYSTEM_LOCK,				
    PLAYER_BEHAVE_BANK_STATE,				// â�� �̿�
    PLAYER_BEHAVE_UNEXPECTED_WAITING,		// ���������� ����� //__NA_780_PREVENTION_PARTY_RETURN_ABUSE
    PLAYER_BEHAVE_DIALOG_OPEN_STATE,
    PLAYER_BEHAVE_SERVER_SELECTING_STATE,   // ���� ������ ���� �����... //__NA001334_090420_SERVER_SELECT_FACILITY__
    //__KR_001355_20090624_GROUPINGSYSTEM_ALL
#ifdef _DH_UNMOVEABLE_CLICKED_NPC
#else
    PLAYER_BEHAVE_GROUP_THINKING_LEAVE,		//< �׷��� ���� ���ΰ� �����
    PLAYER_BEHAVE_GROUP_THINKING_JOIN,      //< �׷��ʴ뿡 ���� ���ΰ� �����
#endif//_DH_UNMOVEABLE_CLICKED_NPC
    PLAYER_BEHAVE_SOCKET_COMPOSITE_STATE, // ��� �ռ�

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE, // ���� ������..
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
};

enum ePLAYER_TITLE
{
    PLAYER_NONE_TITLE		= (0<<0),
    PLAYER_JUNIOR_FORTUNA	= (1<<0),
    PLAYER_SENIOR_FORTUNA	= (1<<1),
};

// DB�� ����Ǵ� �÷��̾� ����
// BASE_PLAYERINFO struct
// BYTE		m_byCharState;
// char		m_pszStateTime[MAX_TIMEDATA_SIZE];
enum eDBCHAR_STATE
{
    DBCHAR_STATE_CHAR_BLOCK = 1, //< ĳ���� ���� ����
};


// ĳ���� ����
enum eCHAR_STATE_TYPE
{
    eCHAR_STATE_INVALID					= 0,

    //-------------------------------------------------------------------------------------------------
    // �����̻�
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_CHAOS					= 1,		// ȥ��(�ƹ��� ����)
    eCHAR_STATE_BLIND					= 2,		// ����ε�(���)
    eCHAR_STATE_DOWN					= 3,		// �ٿ�(��� �ൿ �Ҵ�)
    eCHAR_STATE_DELAY					= 4,		// ������
    eCHAR_STATE_SEALING					= 5,		// ����(������ų ����)
    eCHAR_STATE_STUN					= 6,		// ����(��� �ൿ �Ҵ�)
    eCHAR_STATE_STONE					= 7,		// ��ȭ(��� �ൿ �Ҵ�, ���ݹ��� ����)
    eCHAR_STATE_SLEEP					= 8,		// ����(��� �ൿ �Ҵ�, ������ ������ ����)
    eCHAR_STATE_FROZEN					= 9,		// ������(��� �ൿ �Ҵ�)
    eCHAR_STATE_SEQUELA					= 10,		// ������(��Ȱ���� �ɷ� ����)
    eCHAR_STATE_UPPERDOWN               = 11,       // ���۴ٿ� (���߿� ��, ��� �ൿ �Ҵ�) //_NA_007617_20140916_ADD_CHARSTATE_UPPERDOWN

    //-------------------------------------------------------------------------------------------------
    // ���¾�ȭ
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_POISON					= 100,		// �ߵ�(������ ������)
    eCHAR_STATE_WOUND					= 101,		// ��ó(������ ������)
    eCHAR_STATE_FIRE_WOUND				= 102,		// ȭ��(������ ������)
    eCHAR_STATE_PANIC					= 103,		// ��Ȳ(MP ����)
    eCHAR_STATE_LOW_SPIRIT				= 104,		// �����(���ݷ� ����)
    eCHAR_STATE_WEAKENING				= 105,		// ��ü(���� ����)
    eCHAR_STATE_ATTACK_FAIL				= 106,		// ����(���ݼ����� ����)
    eCHAR_STATE_FRUSTRATION				= 107,		// ����(��ϼ����� ����)
    eCHAR_STATE_FETTER					= 108,		// ����(�̵��ӵ� ����)
    eCHAR_STATE_SLOWDOWN				= 109,		// ��ȭ(���ݼӵ� ����)
    eCHAR_STATE_HP_REDUCE				= 110,		// ����(HP ����)	
    eCHAR_STATE_MP_REDUCE				= 111,		// ����(MP ����)
    eCHAR_STATE_LOW_STRENGTH			= 112,		// ü������(HP ȸ���� ����)
    eCHAR_STATE_DICLINE					= 113,		// ��Ŭ����(MP ȸ���� ����)
    eCHAR_STATE_MAGIC_EXPOSE			= 114,		// ��������(�������� ����)
    eCHAR_STATE_HPRATE_REDUCE			= 116,		// ����(HP ȸ���� ����)
    eCHAR_STATE_MPRATE_REDUCE			= 117,		// ���(MP ȸ���� ����)

    eCHAR_STATE_MAGIC_ATTACK_DECRESE	= 123,		// �������� ����
    eCHAR_STATE_STAT_STR_DECRESE		= 124,		// �� ����
    eCHAR_STATE_STAT_DEX_DECRESE		= 125,		// ��ø ����
    eCHAR_STATE_STAT_VIT_DECRESE		= 126,		// ü�� ����
    eCHAR_STATE_STAT_INT_DECRESE		= 127,		// ���ŷ� ����
    eCHAR_STATE_STAT_SPI_DECRESE		= 128,		// ���� ����

    eCHAR_STATE_STAT_LOWHP_ATTACK_DECREASE	= 129,	// �������� ����
    eCHAR_STATE_STAT_LOWHP_DEFENSE_DECREASE	= 130,	// ������� ����

    eCHAR_STATE_FIRE_ATTACK_DECREASE	= 131,		// �ҼӼ� ���ݷ� ����
    eCHAR_STATE_WATER_ATTACK_DECREASE	= 132,		// ��
    eCHAR_STATE_WIND_ATTACK_DECREASE	= 133,		// �ٶ�
    eCHAR_STATE_EARTH_ATTACK_DECREASE	= 134,		// ����
    eCHAR_STATE_DARK_ATTACK_DECREASE	= 135,		// ����
    
    eCHAR_STATE_FIRE_DEFENSE_DECREASE	= 136,		// �ҼӼ� ���� ����
    eCHAR_STATE_WATER_DEFENSE_DECREASE	= 137,		// ��
    eCHAR_STATE_WIND_DEFENSE_DECREASE	= 138,		// �ٶ�
    eCHAR_STATE_EARTH_DEFENSE_DECREASE	= 139,		// ����
    eCHAR_STATE_DARK_DEFENSE_DECREASE	= 140,		// ����

    eCHAR_STATE_INCREASE_CASTING_TIME	= 141,		// ĳ���� Ÿ�� ����
    eCHAR_STATE_INCREASE_SKILL_COOL_TIME= 142,		// ��ų ��Ÿ�� ����
    eCHAR_STATE_MP_SPEND_INCREASE		= 143,		// MP����(MP �Ҹ� ����)
    // __NA_S00015_20080828_NEW_HOPE_SHADOW
    eCHAR_STATE_PAIN					= 144,		// ����(����)
    eCHAR_STATE_FIRE_WOUND2				= 145,		// ��ũ���̾�(ȭ��)
    eCHAR_STATE_PAIN2					= 146,		// ��ũ����(�����ǰ���)
    eCHAR_STATE_STUN2					= 148,		// ��ũ����(�ָ�)
    eCHAR_STATE_POISON2					= 149,		// ������(��þƮ������)
    eCHAR_STATE_BUFF_CANCEL				= 150,		// ��������(����ĵ��)
    eCHAR_STATE_EXPLOSION				= 151,		// ����(��ũ�극��ũ)
    eCHAR_STATE_DARK_SPARK				= 152,		// ����ũ(��ũ����ũ)
    eCHAR_STATE_IMPOTENT				= 153,		// ������Ʈ
    eCHAR_STATE_CONFUSE					= 154,		// ��ǻ��
    eCHAR_STATE_CANCELATION				= 155,		// ĵ�����̼�
    eCHAR_STATE_HP_SUCTION				= 156,		// HP�� ����Ѵ�.( �� ���ť��... )
    eCHAR_STATE_MP_SUCTION				= 157,		// MP�� ����Ѵ�.( �� ���ť��... )
    eCHAR_STATE_MP_FEAR2				= 158,		// �ҿｺũ��( �Ǿ�� �����ϳ�, ����Ʈ ȿ�������� ���� �ִ´�. )
    eCHAR_STATE_VITAL_SUCTION			= 159,		// ��ü���� HP�� ����´�. 
    eCHAR_STATE_RUSH					= 160,		// ����(�̵��ӵ�����+����Ʈ)
    eCHAR_STATE_CHANGE_ATTR				= 161,		// �Ӽ����� �����Ѵ�. ==> �� ������ ���� �����ð��� 0�̴�.
    eCHAR_STATE_ENCHANT_POISON			= 162,		// ��þƮ�������(���� �ٸ� ����)
    eCHAR_STATE_DETECTING_HIDE          = 163,      // ����ü����
    eCHAR_STATE_WHITE_BLIND             = 164,      // ȭ��Ʈ����ε�(�̽�ƽ ��ų, �Ͼ�� �Ǹ�)
    eCHAR_STATE_PHOENIX_BURN            = 165,      // �Ǵн� ȭ�� (�̽�ƽ ��ų �Ǵн�)
    eCHAR_STATE_POLYMORPH               = 166,      // ��������(�̽�ƽ ��ų, ��������)
    eCHAR_STATE_DARK_OF_LIGHT_ZONE      = 167,      // ��ũ�������Ʈ��
    eCHAR_STATE_GRAVITY_ZONE            = 168,      // �׶��Ƽ��
    eCHAR_STATE_AWAKEN_OFFENSE          = 169,      // �����ū ���ݷ� ���(�̽�ƽ ��ų)
    eCHAR_STATE_AWAKEN_DEFENSE          = 170,      // �����ū ���� ���(�̽�ƽ ��ų)
    eCHAR_STATE_SLIP                    = 171,      // ������ (��ų ���Ұ�)
    eCHAR_STATE_FATAL_POINT             = 172,      // Ȧ�� (�̽�ƽ ����Ż����Ʈ)
    
    //!~ _NA_000000_20130812_NEW_CHARACTER_HELLROID
    eCHAR_STATE_ALL_ELEMENT_ATTACK_DECREASE  = 174, // �����Ұ��ݷ°���(���ο����)
    eCHAR_STATE_ALL_ELEMENT_DEFENSE_DECREASE = 175, // ���������׷�����(��ũ�ʽ�����)
    eCHAR_STATE_CRITICAL_RATIO_DECREASE      = 176, // ũ��Ƽ��Ȯ������
    eCHAR_STATE_CRITICAL_DAMAGE_DECREASE     = 177, // ũ��Ƽ�õ���������
    eCHAR_STATE_SKILL_DAMAGE_DECREASE        = 178, // ��ų����������
    eCHAR_STATE_SLOW_AURORA                  = 179, // ���ο����
    eCHAR_STATE_WEAKNESS_AURORA              = 180, // ��ũ�ʽ�����
    eCHAR_STATE_MISCHANCE_AURORA             = 181, // �̽�æ������
    eCHAR_STATE_DECLINE_AURORA               = 182, // ��Ŭ���ο���
    eCHAR_STATE_RECOVERY_AURORA              = 183, // ��Ŀ��������
    eCHAR_STATE_BOOST_AURORA                 = 184, // �ν�Ʈ����
    eCHAR_STATE_IGNORE_AURORA                = 185, // �̱׳�����
    eCHAR_STATE_CONCENTRATION_AURORA         = 186, // ����Ʈ���̼ǿ���
    eCHAR_STATE_IGNORE_RESERVEHP6            = 187, // HPȸ��(��Ŀ��������)
    eCHAR_STATE_BUF_RANGE_DAMAGE2            = 188, // ������2(����̵��)
    eCHAR_STATE_BUF_RANGE_DAMAGE3            = 189, // ������3(����̵��)
    eCHAR_STATE_BUF_RANGE_DAMAGE4            = 190, // ������4(����̵��)
    eCHAR_STATE_DAMAGED_CRITICAL_RATIO_DECREASE      = 191, // �ǰ� �� ����� ũ��Ƽ�� Ȯ�� ����
    eCHAR_STATE_DAMAGED_CRITICAL_DAMAGE_DECREASE     = 192, // �����(�ǰ� �� ����� ũ��Ƽ�� ������ ����)
    eCHAR_STATE_DAMAGED_CRITICAL_RATIO_INCREASE      = 193, // ������(�ǰ� �� ����� ũ��Ƽ�� Ȯ�� ����)
    eCHAR_STATE_DAMAGED_CRITICAL_DAMAGE_INCREASE     = 194, // �ǰ� �� ����� ũ��Ƽ�� ������ ����
    eCHAR_STATE_ELECTRICSHOCK                = 195, // ����
    eCHAR_STATE_GUARDIANSHIELD               = 196, // ����𽯵�
    eCHAR_STATE_PROTECTION1                  = 197, // ��ȣ ����
    //!~ _NA_000000_20130812_NEW_CHARACTER_HELLROID

    kCharStatePoison3 = 3001, // ���� ��ų ��
    // end of shadow

    //-------------------------------------------------------------------------------------------------
    // ���°�ȭ
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_ABSORB					= 115,		// �ǰ� ������ ���(HP, MP ���)
    eCHAR_STATE_ANGER					= 200,		// �г�(���ݷ� ���)
    eCHAR_STATE_DEFENSE					= 201,		// ���(���� ���)
    eCHAR_STATE_PROTECTION				= 202,		// ��ȣ(�̻���¿� �ɸ��� ����)
    eCHAR_STATE_FIGHTING				= 203,		// ����(���ݼ����� ���)
    eCHAR_STATE_BALANCE					= 204,		// ����(��ϼ����� ���)
    eCHAR_STATE_VITAL_POWER				= 205,		// Ȱ��(HP ȸ���� ���)
    eCHAR_STATE_MEDITATION				= 206,		// ���(MP, SP ȸ���� ���)
    eCHAR_STATE_HIGH_SPIRIT				= 207,		// �⼼(���ݼӵ� ����)
    eCHAR_STATE_SPEEDING				= 208,		// ����(�̵��ӵ� ����)
    eCHAR_STATE_CONCENTRATION			= 209,		// ����(ũ��Ƽ��Ȯ�� ����)
    eCHAR_STATE_INCREASE_SKILLRANGE		= 210,		// �þ�(��ų��Ÿ� ����)
    eCHAR_STATE_PRECISION				= 211,		// ����(ũ��Ƽ�� ������ ����)
    eCHAR_STATE_HP_INCREASE				= 212,		// �ݷ�(HP ����)
    eCHAR_STATE_MP_INCREASE				= 213,		// ź��(MP ����)
    eCHAR_STATE_HPRATE_INCREASE			= 214,		// Ȱ��(HP ȸ���� ����)
    eCHAR_STATE_MPRATE_INCREASE			= 215,		// �ڱ�(MP ȸ���� ����)
    eCHAR_STATE_CURE					= 216,		// ġ��(�������̻� ���)
    eCHAR_STATE_MAGIC_DEFENSE			= 217,		// �������(�������� ����)
    eCHAR_STATE_MAGIC_SHIELD			= 218,		// ������ȣ(�������� HP)
    eCHAR_STATE_HOLDING					= 219,		// Ȧ��(�̵��Ұ�, ���ݰ���)
    eCHAR_STATE_SP_BONUS				= 221,		// SP�� �߰� ���ݷ�(���������� ����)
    eCHAR_STATE_BUF_RANGE_DAMAGE		= 222,		// ������(�ֺ����鿡�� ������ ������)
    eCHAR_STATE_STAT_STR				= 223,		// ����ȭ
    eCHAR_STATE_STAT_DEX				= 224,		// ��ø��ȭ
    eCHAR_STATE_STAT_VIT				= 225,		// ü�°�ȭ
    eCHAR_STATE_STAT_SPI				= 226,		// ���ŷ°�ȭ
    eCHAR_STATE_STAT_INT				= 227,		// ���°�ȭ
    eCHAR_STATE_MAGIC_ATTACK_INCREASE	= 228,		// �������� ���

    eCHAR_STATE_STAT_LOWHP_ATTACK_INCREASE	= 229,	// �������� ���
    eCHAR_STATE_STAT_LOWHP_DEFENSE_INCREASE	= 230,	// ������� ���

    eCHAR_STATE_STAT_DAMAGE_ADD			= 231,		// �߰� ������

    eCHAR_STATE_FIRE_ATTACK_INCREASE	= 232,		// �ҼӼ� ���ݷ� ���
    eCHAR_STATE_WATER_ATTACK_INCREASE	= 233,		// ��
    eCHAR_STATE_WIND_ATTACK_INCREASE	= 234,		// �ٶ�
    eCHAR_STATE_EARTH_ATTACK_INCREASE	= 235,		// ����
    eCHAR_STATE_DARK_ATTACK_INCREASE	= 236,		// ����

    eCHAR_STATE_FIRE_DEFENSE_INCREASE	= 237,		// �ҼӼ� ���� ���
    eCHAR_STATE_WATER_DEFENSE_INCREASE	= 238,		// ��
    eCHAR_STATE_WIND_DEFENSE_INCREASE	= 239,		// �ٶ�
    eCHAR_STATE_EARTH_DEFENSE_INCREASE	= 240,		// ����
    eCHAR_STATE_DARK_DEFENSE_INCREASE	= 241,		// ����

    eCHAR_STATE_DECREASE_CASTING_TIME	= 242,		// ĳ���� Ÿ�� ����
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME= 243,		// ��ų ��Ÿ�� ����
    eCHAR_STATE_MP_SPEND_DECREASE		= 244,		// MPȰ��(MP �Ҹ� ����)

    eCHAR_STATE_REFLECT_DAMAGE			= 220,		// �ǰݵ������ݻ�(������ �ݻ�)
    eCHAR_STATE_REFLECT_SLOW			= 118,		// �ǰ�����(������ + �̼�����)
    eCHAR_STATE_REFLECT_FROZEN			= 119,		// �ǰ�������(������ + ����ൿ�Ҵ�)
    eCHAR_STATE_REFLECT_SLOWDOWN		= 120,		// �ǰݵ�ȭ(������ + ��������)
    eCHAR_STATE_REFLECT_STUN			= 121,		// �ǰݽ���(������ + ����ൿ�Ҵ�)
    eCHAR_STATE_REFLECT_FEAR			= 122,		// �ǰ��Ǿ�(������ + ����)
    eCHAR_STATE_IGNORE_RESERVEHP		= 245,		// Ȱ��(HP��������)
    eCHAR_STATE_ANGER8					= 246,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE8	= 247,
    eCHAR_STATE_DEFENSE8				= 248,
    eCHAR_STATE_MAGIC_DEFENSE7			= 249,
    eCHAR_STATE_PRECISION7				= 250,
    //{_NA_001231_20081216_ADD_OPTION_KIND
    eCHAR_STATE_RESIST_HOLDING			= 251,		//Ȧ�� ����.
    eCHAR_STATE_RESIST_SLEEP			= 252,		//���� ����.
    eCHAR_STATE_RESIST_POISON			= 253,		//�ߵ� ����.
    eCHAR_STATE_RESIST_KNOCKBACK		= 254,		//�˹� ����.
    eCHAR_STATE_RESIST_DOWN				= 255,		//�ٿ� ����.
    eCHAR_STATE_RESIST_STUN				= 256,		//���� ����.
    eCHAR_STATE_DECREASE_PVPDAMAGE		= 257,		//PVP������ ����.
    //}
//#ifdef __NA_001244_20090417_ATTACK_RESIST
//	eCHAR_STATE_RESIST_NOMALATTACK_ALLCHAR	= 258,		//	����ɸ� �Ϲݰ��� ����.
//	eCHAR_STATE_RESIST_SKILLATTACK_ALLCHAR	= 259,		//	����ɸ� ��ų���� ����.
//	eCHAR_STATE_RESIST_ALLATTACK_ALLCHAR	= 260,		//	����ɸ� ������ ����.
//	eCHAR_STATE_RESIST_NOMALATTACK_BERSERKER = 261,		//	����Ŀ �Ϲݰ��� ����.	
//	eCHAR_STATE_RESIST_SKILLATTACK_BERSERKER = 262,		//	����Ŀ ��ų���� ����.
//	eCHAR_STATE_RESIST_ALLATTACK_BERSERKER	= 263,		//	����Ŀ ������ ����.	
//	eCHAR_STATE_RESIST_NOMALATTACK_VALKYRIE = 264,  	//	��Ű�� �Ϲݰ��� ����.	
//	eCHAR_STATE_RESIST_SKILLATTACK_VALKYRIE = 265,  	//	��Ű�� ��ų���� ����.
//	eCHAR_STATE_RESIST_ALLATTACK_VALKYRIE	= 266,		//	��Ű�� ������ ����.	
//	eCHAR_STATE_RESIST_NOMALATTACK_DRAGON	= 267,		//	�巡�� �Ϲݰ��� ����.	
//	eCHAR_STATE_RESIST_SKILLATTACK_DRAGON	= 268,		//	�巡�� ��ų���� ����.
//	eCHAR_STATE_RESIST_ALLATTACK_DRAGON		= 269,		//	�巡�� ������ ����.	
//	eCHAR_STATE_RESIST_NOMALATTACK_ELEMENTALIST	= 270,	//	���� �Ϲݰ��� ����.	
//	eCHAR_STATE_RESIST_SKILLATTACK_ELEMENTALIST	= 271,	//	���� ��ų���� ����.	
//	eCHAR_STATE_RESIST_ALLATTACK_ELEMENTALIST	= 272,	//	���� ������ ����.	
//	eCHAR_STATE_RESIST_NOMALATTACK_SHADOW	= 273,		//	�⵵�� �Ϲݰ��� ����.		
//	eCHAR_STATE_RESIST_SKILLATTACK_SHADOW	= 274,		//	�⵵�� ��ų���� ����.	
//	eCHAR_STATE_RESIST_ALLATTACK_SHADOW		= 275,		//	�⵵�� ������ ����.		
//#endif
    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ENCOURAGEMENT      			= 276,		// Max sd ����
    eCHAR_STATE_PROMOTION       			= 277,		// sd ȸ���� ����
    //-------------------------------------------------------------------------------------------------
    // Ư��
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_TRANSFORMATION			= 300,		// ����
    eCHAR_STATE_STEEL					= 301,		// ��ö(�̵� �� �ൿ �Ҵ�, �̵��ӵ� ����)
    eCHAR_STATE_TRANSPARENT				= 302,		// ����(�̵� ���� �ൿ�� ����)
    eCHAR_STATE_FEAR					= 303,		// �Ǿ�(����� �������� �Ѵ�.)
    eCHAR_STATE_BLUR					= 304,		// ���ݺҴ�(����� ������ ���Ѵ�.)
    eCHAR_STATE_THRUST					= 305,		// �и���(�и��� �ִ� ����)
    eCHAR_STATE_SUMMON					= 306,		// ��ȯ
    eCHAR_STATE_SPCHARGE				= 307,		// �˱�����
    eCHAR_STATE_CURSE_INCREASE			= 308,		// ���ְ�ȭ(Ŀ����ũ����)
    //_NA001385_20090924_DOMINATION_ETC
    eCHAR_STATE_STAMP	    			= 309,		// ���� ����(�̻���¿� �ɸ��� ����)
    kCharStateIncreseHeal = 310, // ġ���� ���� ����
    kCharStateActiveComboSkill = 311, // ���� ��ų Ȱ��ȭ
    kCharStateIncreseSkillDamage = 312, // ��ų ������ ���� ����
    // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    eCHAR_STATE_FUGITIVE = 313, // ���� ������
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    eCHAR_STATE_TRANSFORMATION6         = 315,      // ����
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    //_NA_007667_20141001_WITCHBLADE_SKILL
    eCHAR_STATE_VICE_SWORD              = 400,      // ���̽��ҵ�
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eCHAR_STATE_CHARMED                 = 401,      // ��Ȥ(�����ڸ� ������� �Ѵ�.)
    eCHAR_STATE_ALL_ELEMENTS_INCREASE   = 402,      // ��� �Ӽ� ���ݷ� ����
    eCHAR_STATE_REFLECT_LINK_SKILL      = 403,      // �ǰݽ� ��ų���(������ + ��ų)
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eCHAR_STATE_EVADE                   = 404,      // �̺��̵�
    eCHAR_STATE_FURYFORMATION           = 405,      // ǻ�������̼�
    eCHAR_STATE_RISING_FORCE            = 406,      // ����¡����
    eCHAR_STATE_FP_POWER_INCREASE       = 407,     // FP�� ���� ����������ȿ���� ������Ų��
    eCHAR_STATE_WIT_VITAL_POWER         = 408,      // ��ġ���̵� Ȱ��
    eCHAR_STATE_WIT_CONCENTRATION       = 409,      // ��ġ���̵� ����
    eCHAR_STATE_INCREASE_DAMAGE         = 410,      // ������ ����
    eCHAR_STATE_AUTOCAST_BLEEDING_ATTACK = 411,      // �������� ����
    eCHAR_STATE_RISING_FORCE_IMMUNITY_DAMAMGE = 412, // ����¡������ ���� //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    //-------------------------------------------------------------------------------------------------
    // ���°�ȭ_����
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_ANGER3						= 500,		// �г�3
    eCHAR_STATE_DEFENSE3					= 501,		// ���3
    eCHAR_STATE_FIGHTING3					= 502,		// ����3
    eCHAR_STATE_BALANCE3					= 503,		// ����3
    eCHAR_STATE_VITAL_POWER3				= 504,		// Ȱ��3
    eCHAR_STATE_MEDITATION3					= 505,		// ���3
    eCHAR_STATE_HIGH_SPIRIT3				= 506,		// �⼼3
    eCHAR_STATE_SPEEDING3					= 507,		// ����3
    eCHAR_STATE_CONCENTRATION3				= 508,		// ����3
    eCHAR_STATE_INCREASE_SKILLRANGE3		= 509,		// �þ�3
    eCHAR_STATE_PRECISION3					= 510,		// ����3
    eCHAR_STATE_HP_INCREASE3				= 511,		// �ݷ�3
    eCHAR_STATE_MP_INCREASE3				= 512,		// ź��3
    eCHAR_STATE_HPRATE_INCREASE3			= 513,		// Ȱ��3
    eCHAR_STATE_MPRATE_INCREASE3			= 514,		// �ڱ�3
    eCHAR_STATE_MAGICDEFENSE3				= 515,		// �������3
    eCHAR_STATE_STAT_STR3					= 516,		// ����ȭ3
    eCHAR_STATE_STAT_DEX3					= 517,		// ��ø��ȭ3
    eCHAR_STATE_STAT_VIT3					= 518,		// ü�°�ȭ3
    eCHAR_STATE_STAT_INT3					= 519,		// ���ŷ°�ȭ3
    eCHAR_STATE_STAT_SPI3					= 520,		// ���°�ȭ3
    eCHAR_STATE_MAGIC_ATTACK_INCREASE3		= 521,		// �������ݻ��3
    eCHAR_STATE_STAT_DAMAGE_ADD3			= 522,		// �߰�������3
    eCHAR_STATE_FIRE_ATTACK_INCREASE3		= 523,		// �ҼӼ����ݷ�����3
    eCHAR_STATE_WATER_ATTACK_INCREASE3		= 524,		// ���Ӽ����ݷ�����3
    eCHAR_STATE_WIND_ATTACK_INCREASE3		= 525,		// �ٶ��Ӽ����ݷ�����3
    eCHAR_STATE_EARTH_ATTACK_INCREASE3		= 526,		// �����Ӽ����ݷ�����3
    eCHAR_STATE_DARK_ATTACK_INCREASE3		= 527,		// ����Ӽ����ݷ�����3
    eCHAR_STATE_FIRE_DEFENSE_INCREASE3		= 528,		// �ҼӼ���������3
    eCHAR_STATE_WATER_DEFENSE_INCREASE3		= 529,		// ���Ӽ���������3
    eCHAR_STATE_WIND_DEFENSE_INCREASE3		= 530,		// �ٶ��Ӽ���������3
    eCHAR_STATE_EARTH_DEFENSE_INCREASE3		= 531,		// �����Ӽ���������3
    eCHAR_STATE_DARK_DEFENSE_INCREASE3		= 532,		// ����Ӽ���������3
    eCHAR_STATE_MP_SPEND_INCREASE3			= 533,		// MP����3(MP �Ҹ� ����)
    eCHAR_STATE_MP_SPEND_DECREASE3			= 534,		// MPȰ��3(MP �Ҹ� ����)	
    eCHAR_STATE_DECREASE_CASTING_TIME3		= 535,		//ĳ���� Ÿ�� ����3
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME3	= 536,		//��ų ��Ÿ�� ����3	
    eCHAR_STATE_TRANSFORMATION3				= 537,		// ����3
    //eCHAR_STATE_STEEL3						= 538,		// ��ö3(�̵� �� �ൿ �Ҵ�, �̵��ӵ� ����)
    eCHAR_STATE_TRANSPARENT3				= 539,		// ����3(�̵� ���� �ൿ�� ����)
    eCHAR_STATE_FEAR3						= 540,		// �Ǿ�3(����� �������� �Ѵ�.)
    eCHAR_STATE_BLUR3						= 541,		// ���ݺҴ�3(����� ������ ���Ѵ�.)
    eCHAR_STATE_THRUST3						= 542,		// �и���3(�и��� �ִ� ����)
    eCHAR_STATE_SUMMON3						= 543,		// ��ȯ3
    eCHAR_STATE_SPCHARGE3					= 544,		// �˱�����3
    eCHAR_STATE_IGNORE_RESERVEHP3			= 545,		// Ȱ��(HP��������3)
    eCHAR_STATE_ANGER9						= 546,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE9		= 547,
    eCHAR_STATE_DEFENSE9					= 548,
    eCHAR_STATE_MAGIC_DEFENSE8				= 549,
    eCHAR_STATE_PRECISION8					= 550,

    //__NA_001290_20090525_SHIELD_SYSTEM
     eCHAR_STATE_ENCOURAGEMENT3      		= 576,		// Max sd ����
     eCHAR_STATE_PROMOTION3       			= 577,		// sd ȸ���� ����

    //-------------------------------------------------------------------------------------------------
    // ���°�ȭ_�̺�Ʈ
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_ANGER4						= 700,		// �г�4
    eCHAR_STATE_DEFENSE4					= 701,		// ���4
    eCHAR_STATE_FIGHTING4					= 702,		// ����4
    eCHAR_STATE_BALANCE4					= 703,		// ����4
    eCHAR_STATE_VITAL_POWER4				= 704,		// Ȱ��4
    eCHAR_STATE_MEDITATION4					= 705,		// ���4
    eCHAR_STATE_HIGH_SPIRIT4				= 706,		// �⼼4
    eCHAR_STATE_SPEEDING4					= 707,		// ����4
    eCHAR_STATE_CONCENTRATION4				= 708,		// ����4
    eCHAR_STATE_INCREASE_SKILLRANGE4		= 709,		// �þ�4
    eCHAR_STATE_PRECISION4					= 710,		// ����4
    eCHAR_STATE_HP_INCREASE4				= 711,		// �ݷ�4
    eCHAR_STATE_MP_INCREASE4				= 712,		// ź��4
    eCHAR_STATE_HPRATE_INCREASE4			= 713,		// Ȱ��4
    eCHAR_STATE_MPRATE_INCREASE4			= 714,		// �ڱ�4
    eCHAR_STATE_MAGICDEFENSE4				= 715,		// �������4
    eCHAR_STATE_STAT_STR4					= 716,		// ����ȭ4
    eCHAR_STATE_STAT_DEX4					= 717,		// ��ø��ȭ4
    eCHAR_STATE_STAT_VIT4					= 718,		// ü�°�ȭ4
    eCHAR_STATE_STAT_INT4					= 719,		// ���ŷ°�ȭ4
    eCHAR_STATE_STAT_SPI4					= 720,		// ���°�ȭ4
    eCHAR_STATE_MAGIC_ATTACK_INCREASE4		= 721,		// �������ݻ��4
    eCHAR_STATE_STAT_DAMAGE_ADD4			= 722,		// �߰�������4
    eCHAR_STATE_FIRE_ATTACK_INCREASE4		= 723,		// �ҼӼ����ݷ�����4
    eCHAR_STATE_WATER_ATTACK_INCREASE4		= 724,		// ���Ӽ����ݷ�����4
    eCHAR_STATE_WIND_ATTACK_INCREASE4		= 725,		// �ٶ��Ӽ����ݷ�����4
    eCHAR_STATE_EARTH_ATTACK_INCREASE4		= 726,		// �����Ӽ����ݷ�����4
    eCHAR_STATE_DARK_ATTACK_INCREASE4		= 727,		// ����Ӽ����ݷ�����4
    eCHAR_STATE_FIRE_DEFENSE_INCREASE4		= 728,		// �ҼӼ���������4
    eCHAR_STATE_WATER_DEFENSE_INCREASE4		= 729,		// ���Ӽ���������4
    eCHAR_STATE_WIND_DEFENSE_INCREASE4		= 730,		// �ٶ��Ӽ���������4
    eCHAR_STATE_EARTH_DEFENSE_INCREASE4		= 731,		// �����Ӽ���������4
    eCHAR_STATE_DARK_DEFENSE_INCREASE4		= 732,		// ����Ӽ���������4
    eCHAR_STATE_MP_SPEND_INCREASE4			= 733,		// MP����4(MP �Ҹ� ����)
    eCHAR_STATE_SPCHARGE4					= 744,		// �˱�����4
    eCHAR_STATE_MP_SPEND_DECREASE4			= 734,		// MPȰ��4(MP �Ҹ� ����)	
    eCHAR_STATE_DECREASE_CASTING_TIME4		= 735,		//ĳ���� Ÿ�� ����4
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME4	= 736,		//��ų ��Ÿ�� ����4	
    eCHAR_STATE_TRANSFORMATION4				= 737,		// ����4
    //eCHAR_STATE_STEEL4						= 738,		// ��ö4(�̵� �� �ൿ �Ҵ�, �̵��ӵ� ����)
    eCHAR_STATE_TRANSPARENT4				= 739,		// ����4(�̵� ���� �ൿ�� ����)
    eCHAR_STATE_FEAR4						= 740,		// �Ǿ�4(����� �������� �Ѵ�.)
    eCHAR_STATE_BLUR4						= 741,		// ���ݺҴ�4(����� ������ ���Ѵ�.)
    eCHAR_STATE_THRUST4						= 742,		// �и���4(�и��� �ִ� ����)
    eCHAR_STATE_SUMMON4						= 743,		// ��ȯ4
    eCHAR_STATE_IGNORE_RESERVEHP4			= 745,		// Ȱ��(HP��������4)
    eCHAR_STATE_ANGER10						= 746,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE10		= 747,
    eCHAR_STATE_DEFENSE10					= 748,
    eCHAR_STATE_MAGIC_DEFENSE9				= 749,
    eCHAR_STATE_PRECISION9					= 750,
    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ENCOURAGEMENT4     			= 776,		// Max sd ����
    eCHAR_STATE_PROMOTION4       			= 777,		// sd ȸ���� ����
    
    ////////////////////////////////////////////////////////////////////////////////////////////////
    // ��ų ���� ���� �߰� 951 ~ 998
    ////////////////////////////////////////////////////////////////////////////////////////////////
    
    kCharStateHardStance = 951, // �ϵ� ���Ľ� (���� ü��)
    kCharStateIronStance = 952, // ���̾� ���Ľ� (���� ü��)
    kCharStateBloodHaze = 953, // ���� ������ (���� HP)
    kCharStateMotalBlood = 954, // ��Ż ���� (���� HP)
    kCharStateCourageOfKnights = 955, // Ŀ���� ���� ������ (���� ��ø)
    kCharStateFeverOfKnights = 956, // �ǹ� ���� ������ (���� ��ø)
    kCharStateDragonicForce = 957, // �巡��� ���� (���� ���ݷ�)
    kCharStateDragonicBless = 958, // �巡��� ���� (���� ���ݷ�)
    kCharStateHelronsSoul = 959, // ����� �ҿ� (���� ���ŷ�)
    kCharStateDemonsSoul = 960, // ������ �ҿ� (���� ���ŷ�)
    kCharStateDarkTrace = 961, // ��ũ Ʈ���̽� (���� ���ݼӵ�)
    kCharStateDarkRage = 962, // ��ũ ������ (���� ���ݼӵ�)
    kCharStateIncreaseMind = 963, // ��ũ���� ���ε� (���� ����)
    kCharStateMindSpark = 964, // ���ε� ����ũ (���� ����)
    kCharStateSummonicShield = 965, // ���Ӵ� �ǵ� (���� ����)
    kCharStateSummonicBarrier = 966, // ���Ӵ� ������ (���� ����)
    kCharStateEnchantPower = 967, // ��æƮ �Ŀ� (���� ��)
    kCharStateCatalystPower = 968, // īŻ����Ʈ �Ŀ� (���� ��)
    kCharStateWhisperOfWind = 969, // ������ ���� ���� (���� �̵��ӵ�)
    kCharStateWhisperOfSylph = 970, // ������ ���� ���� (���� �̵��ӵ�)
    eCHAR_STATE_FIRE_ATTACK_INCREASE6   = 971,  // �� �Ӽ����� ���� (�̽�ƽ)
    eCHAR_STATE_WATER_ATTACK_INCREASE6  = 972,  // �� �Ӽ����� ���� (�̽�ƽ)
    eCHAR_STATE_WIND_ATTACK_INCREASE6   = 973,  // �ٶ� �Ӽ����� ���� (�̽�ƽ)
    eCHAR_STATE_EARTH_ATTACK_INCREASE6  = 974,  // ���� �Ӽ����� ���� (�̽�ƽ)
    
    kCharStateIncreaseAggroPoint = 999, // ��׷� ����Ʈ ���� ����
    
    //-------------------------------------------------------------------------------------------------
    // ���°�ȭ_��Ÿ
    //-------------------------------------------------------------------------------------------------
    eCHAR_STATE_ANGER5						= 1500,		// �г�5
    eCHAR_STATE_DEFENSE5					= 1501,		// ���5
    eCHAR_STATE_FIGHTING5					= 1502,		// ����5
    eCHAR_STATE_BALANCE5					= 1503,		// ����5
    eCHAR_STATE_VITAL_POWER5				= 1504,		// Ȱ��5
    eCHAR_STATE_MEDITATION5					= 1505,		// ���5
    eCHAR_STATE_HIGH_SPIRIT5				= 1506,		// �⼼5
    eCHAR_STATE_SPEEDING5					= 1507,		// ����5
    eCHAR_STATE_CONCENTRATION5				= 1508,		// ����5
    eCHAR_STATE_INCREASE_SKILLRANGE5		= 1509,		// �þ�5
    eCHAR_STATE_PRECISION5					= 1510,		// ����5
    eCHAR_STATE_HP_INCREASE5				= 1511,		// �ݷ�5
    eCHAR_STATE_MP_INCREASE5				= 1512,		// ź��5
    eCHAR_STATE_HPRATE_INCREASE5			= 1513,		// Ȱ��5
    eCHAR_STATE_MPRATE_INCREASE5			= 1514,		// �ڱ�5
    eCHAR_STATE_MAGICDEFENSE5				= 1515,		// �������5
    eCHAR_STATE_STAT_STR5					= 1516,		// ����ȭ5
    eCHAR_STATE_STAT_DEX5					= 1517,		// ��ø��ȭ5
    eCHAR_STATE_STAT_VIT5					= 1518,		// ü�°�ȭ5
    eCHAR_STATE_STAT_INT5					= 1519,		// ���ŷ°�ȭ5
    eCHAR_STATE_STAT_SPI5					= 1520,		// ���°�ȭ5
    eCHAR_STATE_MAGIC_ATTACK_INCREASE5		= 1521,		// �������ݻ��5
    eCHAR_STATE_STAT_DAMAGE_ADD5			= 1522,		// �߰�������5
    eCHAR_STATE_FIRE_ATTACK_INCREASE5		= 1523,		// �ҼӼ����ݷ�����5
    eCHAR_STATE_WATER_ATTACK_INCREASE5		= 1524,		// ���Ӽ����ݷ�����5
    eCHAR_STATE_WIND_ATTACK_INCREASE5		= 1525,		// �ٶ��Ӽ����ݷ�����5
    eCHAR_STATE_EARTH_ATTACK_INCREASE5		= 1526,		// �����Ӽ����ݷ�����5
    eCHAR_STATE_DARK_ATTACK_INCREASE5		= 1527,		// ����Ӽ����ݷ�����5
    eCHAR_STATE_FIRE_DEFENSE_INCREASE5		= 1528,		// �ҼӼ���������5
    eCHAR_STATE_WATER_DEFENSE_INCREASE5		= 1529,		// ���Ӽ���������5
    eCHAR_STATE_WIND_DEFENSE_INCREASE5		= 1530,		// �ٶ��Ӽ���������5
    eCHAR_STATE_EARTH_DEFENSE_INCREASE5		= 1531,		// �����Ӽ���������5
    eCHAR_STATE_DARK_DEFENSE_INCREASE5		= 1532,		// ����Ӽ���������5
    eCHAR_STATE_MP_SPEND_INCREASE5			= 1533,		// MP����5(MP �Ҹ� ����)
    eCHAR_STATE_MP_SPEND_DECREASE5			= 1534,		// MPȰ��5(MP �Ҹ� ����)	
    eCHAR_STATE_DECREASE_CASTING_TIME5		= 1535,		//ĳ���� Ÿ�� ����5
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME5	= 1536,		//��ų ��Ÿ�� ����5	
    eCHAR_STATE_TRANSFORMATION5				= 1537,		// ����5
    //eCHAR_STATE_STEEL5						= 1538,		// ��ö5(�̵� �� �ൿ �Ҵ�, �̵��ӵ� ����)
    eCHAR_STATE_TRANSPARENT5				= 1539,		// ����5(�̵� ���� �ൿ�� ����)
    eCHAR_STATE_FEAR5						= 1540,		// �Ǿ�5(����� �������� �Ѵ�.)
    eCHAR_STATE_BLUR5						= 1541,		// ���ݺҴ�5(����� ������ ���Ѵ�.)
    eCHAR_STATE_THRUST5						= 1542,		// �и���5(�и��� �ִ� ����)
    eCHAR_STATE_SUMMON5						= 1543,		// ��ȯ5
    eCHAR_STATE_SPCHARGE5					= 1544,		// �˱�����5
    eCHAR_STATE_IGNORE_RESERVEHP5			= 1545,		// Ȱ��(HP��������5)
    eCHAR_STATE_ANGER12						= 1546,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE12		= 1547,
    eCHAR_STATE_DEFENSE12					= 1548,
    eCHAR_STATE_MAGIC_DEFENSE11				= 1549,
    eCHAR_STATE_PRECISION11					= 1550,
    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ENCOURAGEMENT5     			= 1576,		// Max sd ����
    eCHAR_STATE_PROMOTION5       			= 1577,		// sd ȸ���� ����

    eCHAR_STATE_PERIODIC_DAMAGE			= 5000,		// �ֱ� ������

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //-------------------------------------------------------------------------------------------------
    // ��� �ü� ��Ƽ�� ���µ�
    // ��� �ü� ��Ƽ�� ���´� ���Ǵ� �����Ƽ�� ������ ���¿� ���� �����Ǿ�� �ϹǷ� ���� ���� ����
    //------------------------------------------------------------------------------------------------- 
    //ù��° �����Ƽ ���µ�
    eCHAR_STATE_MERCENARIES_ACTIVE          = 4001, // ��� �ü� �뺴 ���� ��Ƽ�� ����
    eCHAR_STATE_ELDER_ACTIVE                = 4002, // ��� �ü� ����ȸ ��Ƽ�� ����
    eCHAR_STATE_ECLIPSE_ACTIVE              = 4003, // ��� �ü� ��Ŭ���� ��Ƽ�� ����
    eCHAR_STATE_CLAN_ACTIVE                 = 4004, // ��� �ü� Ŭ�� ��Ƽ�� ����
    eCHAR_STATE_MAGIC_ACTIVE                = 4005, // ��� �ü� ���������� ��Ƽ�� ����
    eCHAR_STATE_PELES_ACTIVE                = 4006, // ��� �ü� �ӷ��� ��Ƽ�� ����
    eCHAR_STATE_HER_ACTIVE                  = 4007, // ��� �ü� ����̵� ��Ƽ�� ����
    //�ι�° �����Ƽ ���µ�
    eCHAR_STATE_ELDER_ACTIVE2               = 4008, // ��� �ü� ����ȸ ��Ƽ�� ����2
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    //-------------------------------------------------------------------------------------------------
    // �����Ƽ ���� ���µ�
    //------------------------------------------------------------------------------------------------- 
    eCHAR_STATE_STYLE_THRUST			= 5001,		// ��Ÿ�� ���ݿ��� ����ϴ� �и���
    eCHAR_STATE_STYLE_STUN				= 5002,
    eCHAR_STATE_STYLE_DOWN				= 5003,

    eCHAR_STATE_ETC_FLYING				= 5004,		// ���߿� �� �ִ� ����
    eCHAR_STATE_ETC_DISABLE_VISION		= 5005,		// �þ߰� ���� ������ ���͵��� ã���� ���ϰ� ���ݵ� ���Ѵ�. 
    eCHAR_STATE_ETC_RETURNING			= 5006,		// ��Ȱ��ġ�� ���ư��� ����
    eCHAR_STATE_ETC_HELP				= 5007,		// �����ַ� ���� ����

    eCHAR_STATE_ETC_AUTO_RECOVER_HP		= 5008,		// �ڵ� HP, MP ȸ��
    eCHAR_STATE_ETC_AUTO_RECOVER_MP		= 5009,
    eCHAR_STATE_ETC_AUTO_RECOVER_HPMP	= 5010,
    eCHAR_STATE_ETC_ITEM_RECOVER_HP		= 5011,		// �����ۿ� ���� HP, MP ȸ��
    eCHAR_STATE_ETC_ITEM_RECOVER_MP		= 5012,

    eCHAR_STATE_ETC_FORCED_WARP			= 5013,		// ���������� �������� ����(�̵����⸦ ���߱� ���ؼ� ��� �̵��� ���Ѵ�.)
    eCHAR_STATE_ETC_DISABLE_VISION_TRIGGER	= 5014,		// �þ߰� ���� ������ ���͵��� ã���� ���ϰ� ���ݵ� ���Ѵ�. (Ʈ���ſ��� ���)
    eCHAR_STATE_BLUR_TRIGGER				= 5015,		// ���ݺҴ�(����� ������ ���Ѵ�. Ʈ���ſ�)

    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ETC_EXIST_SHELD_POINT	= 5016,     //SD ���� (0�̻� �����ϴ� ����) 
    eCHAR_STATE_ETC_AUTO_RECOVER_SD		= 5017,     //SD ȸ�� ���� 
    eCHAR_STATE_ETC_FIGHTING    		= 5018,     //SD ���� ����

    //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    eCHAR_STATE_ETC_SKIP_UPDATE_TARGET_LIST = 5019,		// Ÿ�� ����Ʈ ���� ���� �ʴ� ����.
    // _NA_0_20100817_HELLON_LAST_TRIGGER
    eCHAR_STATE_ETC_TRIGGER_HOLDING			= 5020,		// Ʈ���Ÿ� ���� ������ ���Ͽ� �̵��� ������ ���� 

    // WOPS:6554
    eCHAR_STATE_ETC_ITEM_RECOVER_HP_INSTANTLY = 5021,   // ��� ȸ���� �����ۿ� ���� HP ȸ��
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    eCHAR_STATE_ETC_AUTO_RECOVER_FP         = 5022,     // �ڵ� FPȸ��( - )
    //-------------------------------------------------------------------------------------------------
    // ���°�ȭ_ĳ��
    //------------------------------------------------------------------------------------------------- 
    eCHAR_STATE_ANGER2					= 1000,		// �г�2
    eCHAR_STATE_DEFENSE2				= 1001,		// ���2
    eCHAR_STATE_FIGHTING2				= 1002,		// ����2
    eCHAR_STATE_BALANCE2				= 1003,		// ����2
    eCHAR_STATE_VITAL_POWER2			= 1004,		// Ȱ��2
    eCHAR_STATE_MEDITATION2				= 1005,		// ���2
    eCHAR_STATE_HIGH_SPIRIT2			= 1006,		// �⼼2
    eCHAR_STATE_SPEEDING2				= 1007,		// ����2
    eCHAR_STATE_CONCENTRATION2			= 1008,		// ����2
    eCHAR_STATE_INCREASE_SKILLRANGE2	= 1009,		// �þ�2
    eCHAR_STATE_PRECISION2				= 1010,		// ����2
    eCHAR_STATE_HP_INCREASE2			= 1011,		// �ݷ�2
    eCHAR_STATE_MP_INCREASE2			= 1012,		// ź��2
    eCHAR_STATE_HPRATE_INCREASE2		= 1013,		// Ȱ��2
    eCHAR_STATE_MPRATE_INCREASE2		= 1014,		// �ڱ�2
    eCHAR_STATE_MAGIC_DEFENSE2			= 1015,		// �������2
    eCHAR_STATE_STAT_STR2				= 1016,		// ����ȭ2
    eCHAR_STATE_STAT_DEX2				= 1017,		// ��ø��ȭ2
    eCHAR_STATE_STAT_VIT2				= 1018,		// ü�°�ȭ2
    eCHAR_STATE_STAT_INT2				= 1019,		// ���ŷ°�ȭ2
    eCHAR_STATE_STAT_SPI2				= 1020,		// ���°�ȭ2
    eCHAR_STATE_MAGIC_ATTACK_INCREASE2	= 1021,		// �������ݻ��2
    eCHAR_STATE_STAT_DAMAGE_ADD2		= 1022,		// �߰�������2
    eCHAR_STATE_FIRE_ATTACK_INCREASE2	= 1023,		// �ҼӼ����ݷ�����2
    eCHAR_STATE_WATER_ATTACK_INCREASE2	= 1024,		// ���Ӽ����ݷ�����2
    eCHAR_STATE_WIND_ATTACK_INCREASE2	= 1025,		// �ٶ��Ӽ����ݷ�����2
    eCHAR_STATE_EARTH_ATTACK_INCREASE2	= 1026,		// �����Ӽ����ݷ�����2
    eCHAR_STATE_DARK_ATTACK_INCREASE2	= 1027,		// ����Ӽ����ݷ�����2
    eCHAR_STATE_FIRE_DEFENSE_INCREASE2	= 1028,		// �ҼӼ���������2
    eCHAR_STATE_WATER_DEFENSE_INCREASE2	= 1029,		// ���Ӽ���������2
    eCHAR_STATE_WIND_DEFENSE_INCREASE2	= 1030,		// �ٶ��Ӽ���������2
    eCHAR_STATE_EARTH_DEFENSE_INCREASE2	= 1031,		// �����Ӽ���������2
    eCHAR_STATE_DARK_DEFENSE_INCREASE2	= 1032,		// ����Ӽ���������2
    eCHAR_STATE_MP_SPEND_INCREASE2		= 1033,		// MP����2(MP �Ҹ� ����)
    eCHAR_STATE_MP_SPEND_DECREASE2		= 1034,		// MPȰ��2(MP �Ҹ� ����)	
    eCHAR_STATE_DECREASE_CASTING_TIME2		= 1035,	//ĳ���� Ÿ�� ����2
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME2	= 1036,	//��ų ��Ÿ�� ����2	
    eCHAR_STATE_TRANSFORMATION2				= 1037, // ����2
    //eCHAR_STATE_STEEL2						= 1038, // ��ö2(�̵� �� �ൿ �Ҵ�, �̵��ӵ� ����)
    eCHAR_STATE_TRANSPARENT2				= 1039,	// ����2(�̵� ���� �ൿ�� ����)
    eCHAR_STATE_FEAR2						= 1040, // �Ǿ�2(����� �������� �Ѵ�.)
    eCHAR_STATE_BLUR2						= 1041, // ���ݺҴ�2(����� ������ ���Ѵ�.)
    eCHAR_STATE_THRUST2						= 1042, // �и���2(�и��� �ִ� ����)
    eCHAR_STATE_SUMMON2						= 1043, // ��ȯ2
    eCHAR_STATE_SPCHARGE2					= 1044, // �˱�����2
    eCHAR_STATE_IGNORE_RESERVEHP2			= 1045,	// Ȱ��(HP��������2)
    eCHAR_STATE_ANGER11						= 1046,
    eCHAR_STATE_MAGIC_ATTACK_INCREASE11		= 1047,
    eCHAR_STATE_DEFENSE11					= 1048,
    eCHAR_STATE_MAGIC_DEFENSE10				= 1049,
    eCHAR_STATE_PRECISION10					= 1050,
    //
    eCHAR_STATE_PERK_SWITCHABLE_STATE       = 1051, // �� ��ü ����, _NA001956_110210_PERK_SWITCHABLE_REGION
    //__NA_001290_20090525_SHIELD_SYSTEM
    eCHAR_STATE_ENCOURAGEMENT2      		= 1076,		// Max sd ����
    eCHAR_STATE_PROMOTION2       			= 1077,		// sd ȸ���� ����
    eCHAR_STATE_PLENTY                      = 1078 ,    // ǳ��, ���� ȹ�淮 ����
    eCHAR_STATE_SMART                       = 1079 ,    // �Ѹ�, ���� ����ġ ȹ�淮 ����

    eCHAR_STATE_SPEEDING6				= 1200,		// ����3(�̼�)
    eCHAR_STATE_HIGH_SPIRIT6			= 1201,		// �⼼3(����)
    eCHAR_STATE_HP_INCREASE6			= 1202,		// �ݷ�(HP����)
    eCHAR_STATE_TRANSPARENT6			= 1203,		// ����6(�̵� ���� �ൿ�� ����)
    eCHAR_STATE_PRECISION6				= 1204,		// ����3(ũ��Ƽ�� ������ ����)
    eCHAR_STATE_CONCENTRATION6			= 1205,		// ����3(ũ��Ƽ�� Ȯ�� ����)
    eCHAR_STATE_ANGER6					= 1206,		// �г�3(�߰� ���ݷ� ����)
    eCHAR_STATE_MAGIC_ATTACK_INCREASE6	= 1207,		// �������ݻ��3
    eCHAR_STATE_DEFENSE6				= 1208,		// �������3
    eCHAR_STATE_MAGIC_DEFENSE6			= 1209,		// �������3

    //__NA_000994_CHANGUP_NEW_SKILL
    eCHAR_STATE_IMMUNITY_DAMAGE			= 1301,		// ��������( ���� & ���� ������ ����, ���ݰ��� )
    eCHAR_STATE_INTENSIFY_SKILL			= 1302,		// ��ų�������� �����Ѵ�.( ��ų������ 300% )
    eCHAR_STATE_INTENSIFY_SUMMON		= 1303,		// ����( ��ȯ���� ���ݷ��� ���� ���� )
    eCHAR_STATE_ATTACK_IMPOSSIBLE		= 1304,		// ���ݺҰ�( ������ �Ұ��� �ϴ� )
    eCHAR_STATE_HP_INCREASE7			= 1305,		// �ݷ�( �ִ� HP���� )
    eCHAR_STATE_BATTLE					= 2001,		// ��������. 
    eCHAR_STATE_DECREASE_SKILL_COOL_TIME6	= 7001,	// ��ų ��Ÿ�� ����3
    eCHAR_STATE_HP_INCREASE8				= 7002,	// �ݷ�
    eCHAR_STATE_ANGER7						= 7003,	// �г�
    eCHAR_STATE_HPRATE_INCREASE6			= 7004,	// Ȱ��
    eCHAR_STATE_DECREASE_CASTING_TIME6		= 7005,	// ����Ȱ��
    eCHAR_STATE_MAGIC_ATTACK_INCREASE7		= 7006,	// �������ݻ��
    eCHAR_STATE_MPRATE_INCREASE6			= 7007,	// �ڱ�
    eCHAR_STATE_CONCENTRATION7				= 7008,	// ����
    eCHAR_STATE_DEFENSE7					= 7009,	// ���	
    //eCHAR_STATE_IGNORE_RESERVEHP6			= 7010,	// Ȱ��(HP��������6)

    //!_NA_000587_20100928_DOMINATION_BUFF_ITEM
    eCHAR_STATE_HPRATE_INCREASE13           = 7101,	// Ȱ��_����	        
    eCHAR_STATE_DEFENSE13                   = 7102,	// 	�������_����	    
    eCHAR_STATE_MAGICDEFENSE13              = 7103,	// 	�������_����	    
    eCHAR_STATE_HPRATE_INCREASE14           = 7104,	// 	Ȱ��_���ǰ�	    
    eCHAR_STATE_DEFENSE14                   = 7105,	// 	�������_���ǰ�	    
    eCHAR_STATE_MAGICDEFENSE14              = 7106,	// 	�������_���ǰ�	    
    eCHAR_STATE_HPRATE_INCREASE15           = 7107,	// 	Ȱ��_���ǰ���ü	    
    eCHAR_STATE_DEFENSE15                   = 7108,	// 	�������_���ǰ���ü	
    eCHAR_STATE_MAGICDEFENSE15              = 7109,	// 	�������_���ǰ���ü	

    //----------------------------------------------------------------------------------------------
    //  (WARNING) �Ʒ� ���� ���Ŀ� ���� �߰��ؼ� �ȵ�! ���¸� �̿��� ���� �������� ����ϰ� ����.
    //__NA000896_080215_TASK_SURVIVAL_SOLOING_QUEST_CSCOMMON__
    eCHAR_STATE_SSQ_CTRL_BLOCK_ATTACK   , // No real state|for control|SSQ �������� (����/�ǰ�) �Ұ��� ���� ����
    eCHAR_STATE_WAR_CTRL_OBSERVER_MODE  , // No real state|for control|���� ���� ���� ���� ����
    //__NA001390_090915_RIDING_SYSTEM__
    eCHAR_STATE_RIDING_RIDER            , // No real state|for control|Riding Status|
    eCHAR_STATE_NPC_NON_PREEMPTIVE_ATTACK , // �񼱰� ���� (�������� ����)   //_NA001385_20090924_DOMINATION_ETC
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
    eRECOVER_TYPE_AUTO_HP		= 1,			//�ڵ����� HPȸ��
    eRECOVER_TYPE_AUTO_MP		= 2,			//�ڵ����� MPȸ��
    eRECOVER_TYPE_AUTO_HPMP		= 3,			//�ڵ����� HP �� MPȸ��
    eRECOVER_TYPE_IGNORE_RESERVE_HP	= 4,		//HP���� �����ϰ� HPȸ��
};

enum eSTATE_TYPE
{
    eSTATE_TYPE_ABNORMAL			= 1,		// �����̻�
    eSTATE_TYPE_WEAKENING			= 2,		// ���¾�ȭ
    eSTATE_TYPE_STRENGTHENING		= 3,		// ���°�ȭ
    eSTATE_TYPE_SPECIALITY			= 4,		// Ư��
};

#define TRIGGER_ATTRIBUTE_CONDITION_OR_FLAG           0x01
#define TRIGGER_ATTRIBUTE_ACTIVE_FLAG                 0x02
#define TRIGGER_ATTRIBUTE_LOOP_FLAG                   0x04
#define TRIGGER_ATTRIBUTE_ACTIVATE_NEXT_TRIGGER       0x08


enum eSUMMON_COMMAND
{
    eSUMMON_COMMAND_FOLLOW,				// �ڵ���
    eSUMMON_COMMAND_ATTACK,				// ��������
    eSUMMON_COMMAND_DELEGATE_ATTACK,	// ����(������)
    eSUMMON_COMMAND_DELEGATE_DEFENSE,	// ����(�����)
    eSUMMON_COMMAND_DESTROY,			// �Ҹ�
    eSUMMON_COMMAND_SKILL_ATTACK,		// ��ų����
    eSUMMON_COMMAND_DELEGATE_ATTACK_IGNORE_PLAYER,	// ����(������)(�÷��̾� ����) 

    eSUMMON_COMMAND_MAX,
};

//_NA001385_20090924_DOMINATION_SKILL
enum eCRYSTALWARP_COMMAND
{
    eCRYSTALWARP_COMMAND_CREATE,	// ��� ��ȯü ���� 
    eCRYSTALWARP_COMMAND_DESTROY,	// ��� ��ȯü ��ȯ���� 
    eCRYSTALWARP_COMMAND_EXECUTE,	// ��� ��ȯü ����

    eCRYSTALWARP_COMMAND_MAX,
};

enum eCRYSTALWARP_STATE
{
    eCRYSTALWARP_STATE_CREATE,	    // ��� ��ȯü ���� 
    eCRYSTALWARP_STATE_DESTROY,	    // ��� ��ȯü ��ȯ���� (�Ʊ��� ���� �ı�)
    eCRYSTALWARP_STATE_DEAD,	    // ��� ��ȯü ����     (������ ���� �ı�, ��ȯ�ð� �ʰ�)

    eCRYSTALWARP_STATE_MAX,
};

// ģ�� ��Ͽ����� ģ�� ����
enum eFRIEND_STATUS
{
    eSTATE_ONLINE=1,		// �¶���
    eSTATE_OFFLINE,			// ��������(�α׾ƿ� �Ǵ� ���� ��)
};
enum eNPC_SPECIAL_ACTION_TYPE
{
    eNPC_SPECIAL_ACTION_HELPREQUEST			= 1,	// ������û
    eNPC_SPECIAL_ACTION_TRANSFORMATION		= 2,	// ����
    eNPC_SPECIAL_ACTION_SKILL				= 3,	// Ư����ų ���
    eNPC_SPECIAL_ACTION_SELP_DESTRUCTION	= 4,	// ����	(__NA00XXXX_080922_TASK_SSQ_NPC_SELF_DESTRUCTION__)
    eNPC_SPECIAL_ACTION_MAX
};

//-----------------------------------------------------------------------
// ��� ���� 
//-----------------------------------------------------------------------

// ��� ��å
// ******** �߿� : ������ enum ������ �״�� DB�� ����ȴ� ���� ����� �� ������ �ٲ��� ����.. ( �ٲٷ��� DB�� ����ض�. )
#ifdef _NA_003923_20120130_GUILD_RENEWAL
enum eGUILD_DUTY
{
    eGUILD_DUTY_NONE,
    eGUILD_DUTY_MASTER,	                // ��� ������
    eGUILD_DUTY_SUBMASTER,	            // ���� ������
    eGUILD_DUTY_STRATEGIST,	            // ����[������]     : ��� ������ -> X
    eGUILD_DUTY_COMMANDER,	            // ������           : ��� ������ -> X
    eGUILD_DUTY_LEADER,                 // ���� ����        : ��� ������ -> NEW
    eGUILD_DUTY_CAMP_LEADER,	        // �Ʒ�(ķ��)����   : ��� ������ -> X
    eGUILD_DUTY_REGULAR_SOLDIER,	    // �ܿ�(������)     : ��� ������ -> �Ϲ� ����
    eGUILD_DUTY_CAMP_SOLDIER,	        // �Ʒ�(ķ��)��     : ��� ������ -> X
    eGUILD_DUTY_MAX,
};
#else
enum eGUILD_DUTY
{
    eGUILD_DUTY_NONE			= 0,
    eGUILD_DUTY_MASTER			= 1,	// ��� ������
    eGUILD_DUTY_SUBMASTER		= 2,	// ���� ������
    eGUILD_DUTY_STRATEGIST		= 3,	// ����[������]
    eGUILD_DUTY_COMMANDER		= 4,	// ������
    eGUILD_DUTY_CAMP_LEADER		= 5,	// �Ʒ�(ķ��)����
    eGUILD_DUTY_REGULAR_SOLDIER = 6,	// �ܿ�(������)
    eGUILD_DUTY_CAMP_SOLDIER	= 7,	// �Ʒ�(ķ��)��
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

// ��� Ŭ����
enum eGUILD_CLASS
{
    eGUILD_CLASS_GENERAL,			// �Ϲ� ���
    eGUILD_CLASS_MERCHANT,			// ���� ����
    eGUILD_CLASS_STOCK,				// �ֽ� ����
    eGUILD_CLASS_GAIDENCE,			// ���̴���
    eGUILD_CLASS_SEED_GAIDENCE,		// �õ� ���̴���
    eGUILD_CLASS_DIVINE_CRUSADER,	// ����� ũ�缼�̴�
    eGUILD_CLASS_ASSASSIN,			// �ϻ��
    eGUILD_CLASS_REUNION,			// �����Ͽ�
};

// ��� �ɹ��� ����
// *** �� ������ DB�� ����ȴ�. ���� �̰��� ������ ���� ��ũ��Ʈ�� DB�� ����� �Ѵ�. ***
enum eGUILD_RIGHTS
{
    eGUILD_RIGHTS_JOIN				    = 1,							//����(�ʴ�)
    eGUILD_RIGHTS_WITHDRAW			    = 2,							//Ż�� 
    eGUILD_RIGHTS_DESTROY			    = 3,							//��ü 
    eGUILD_RIGHTS_KICK				    = 4,							//���� 
    eGUILD_RIGHTS_RELATION			    = 5,							//����
    eGUILD_RIGHTS_RANKUP			    = 6,							//��ũ��
    eGUILD_RIGHTS_NOTICE			    = 7,							//��� ���� 
    eGUILD_RIGHTS_CORPS_NOTICE		    = 8,							//���� ���� 
    eGUILD_RIGHTS_RESERVED_NOTICE	    = 9,							//���� ����
    eGUILD_RIGHTS_DONATEUP			    = 10,							//UP ����
    eGUILD_RIGHTS_UP2GP				    = 11,							//UP->GP ��ȯ
    eGUILD_RIGHTS_USE_UPGP			    = 12,							//UP/GP����Ʈ ��� 
    eGUILD_RIGHTS_CHANGE_DUTY		    = 13,							//��å ����
    eGUILD_RIGHTS_DELEGATION_RIGHT	    = 14,							//���� ����
    eGUILD_RIGHTS_WAREHOUSE_SAVE	    = 15,							//��� â�� ���� 
    eGUILD_RIGHTS_WAREHOUSE_OUT	        = 16,							//��� â�� �ⳳ

    eGUILD_RIGHTS_WAREHOUSE_LOG		    = 17,							//��� â�� �̿볻�� ����
    eGUILD_RIGHTS_GUILD_WAR			    = 18,							//�����
    eGUILD_RIGHTS_SEIGE_WARFARE		    = 19,							//������
    eGUILD_RIGHTS_GUILD_MARK		    = 20,							//��� ��ũ
    eGUILD_RIGHTS_CREATE_CORPS		    = 21,							//����â��
    eGUILD_RIGHTS_DISMISS_CORPS		    = 22,							//�����ػ�
    eGUILD_RIGHTS_CREATE_CAMP		    = 23,							//��� ����ķ�� â�� 
    eGUILD_RIGHTS_DISMISS_CAMP		    = 24,							//��� ����ķ�� �ػ�
    eGUILD_RIGHTS_ITEM_BUY			    = 25,							//��� �������� ���� (���ο��)
    eGUILD_RIGHTS_ITEM_EQUIP		    = 26,							//��� �������� ���(���ο�����)	
    eGUILD_RIGHTS_STYLE_ACQUIRE		    = 27,							//��彺Ÿ�� ȹ��
    eGUILD_RIGHTS_STYLE_LEVEL_UP	    = 28,							//��彺Ÿ�� ������ 
    eGUILD_RIGHTS_GUILD_STYLE_USE	    = 29,							//��彺Ÿ�� ��� 
    eGUILD_RIGHTS_CORPS_STYLE_USE	    = 30,							//���ܽ�Ÿ�� ��� 
    eGUILD_RIGHTS_CLASS_CHANGE		    = 31,							//���Ŭ���� ü����
    eGUILD_RIGHTS_CONQUEST_JOIN		    = 32,							//��� ������Ʈ ���� 
    eGUILD_RIGHTS_CHANGE_MASTER		    = 33,							//����� ����(����)
    eGUILD_RIGHTS_VIEW_RIGHT		    = 34,							//���� ���� ����
    eGUILD_RIGHTS_CHANGE_CORPS		    = 35,							//���ܰ� �̵�
#ifdef _NA_003923_20120130_GUILD_RENEWAL
    eGUILD_RIGHTS_GUILD_EQUIPMENT       = 36,                           //��� �ü� ������
    eGUILD_RIGHTS_ACTIVATION_EQUIPMENT  = 37,                           //��� �ü� Ȱ��ȭ
#endif
    eGUILD_RIGHTS_MAX,
};

// ��� ���� �з�
enum eGUILD_NOTICE
{
    eGUILD_NOTICE_GUILD = 1,		// ��� ����
    eGUILD_NOTICE_CORPS,			// ���� ����
    eGUILD_NOTICE_CAMP,				// ���� ����
};

// ��� ����
enum eGUILD_RELATION
{
    eGUILD_RELATION_NONE,				    // �ʱ�ȭ
    eGUILD_RELATION_ALLIENCE,			    // ����
    eGUILD_RELATION_BOTH_HOSTILITY,		    // �ֹ�����
    eGUILD_RELATION_PROCLAIM_HOSTILITY,	    // ���� ����
    eGUILD_RELATION_ONE_SIDE_HOSTILITY,     // �Ϲ��� ����
    eGUILD_RELATION_CANCEL_BOTH_HOSTILITY,  // �ֹ� ���� ���
};

enum ePROCLAIM_GUILD
{
    NONE_PROCLAIM_GUILD,
    PROCLAIM_GUILD,  //���� ������ ���
    ONESIDE_GUILD,  // ���� ���� ���� ���
};

//���� Ÿ��
enum eGUILD_CORPS_TYPE
{
    eGUILD_GUILD,
    eGUILD_CORPS,
    eGUILD_CAMP,
};

// ��� ���
enum eGUILD_ANSWER
{
    eGUILD_ANSWER_YES = 1,
    eGUILD_ANSWER_NO,
};

//��� ���Ƽ Ÿ��
enum eGUILD_PENALTY
{
    eGUILD_WITHDRAW = 1,
    eGUILD_KICK,
    eGUILD_DESTROY,
};

//��� DelChk
enum eGUILD_DELCHK
{
    eGUILD_DELCHK_NORMAL = 0,
    eGUILD_DELCHK_DESTROY,
    eGUILD_DELCHK_DESTROY_WAIT,
};

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

enum eGUILD_JOIN_REQUEST
{
    eGUILD_JOIN_REQUEST_OK = 0, //���Կ�û ��ư����
    eGUILD_JOIN_REQUEST_CANCEL, //������� ��ư����
};

enum eGUILD_JOIN_REJECT_TYPE
{
    eGUILD_JOIN_REJECT_TYPE_NORMAL = 0,  //���� ������ ���� ����
    eGUILD_JOIN_REJECT_TYPE_TIMEOUT,     //��� �ð� �ʰ��� ���� ����
    eGUILD_JOIN_REJECT_TYPE_FAILED,      //�ٸ� ��忡 ���� �Ǵ� ��� ������ ���� ����
};

enum FACILITY_UPGRADE_ITEM_TYPE
{
    FUI_ONLY_GUILDPOINT = 0,             // ��� �ü��� ��� �������θ� ���׷��̵�
    FUI_ONLY_ITEM,                       // ��� �ü��� ���������θ� ���׷��̵�
    FUI_GUILDPOINTANDITEM,               // ��� �ü��� ��� ���ΰ� ���������� ���׷��̵�
    FUI_MAX,
};
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

enum eJION_TOP_STATE
{
    eJION_TOP_STATE_NONE		= -1,
    eJION_TOP_STATE_ADMISSION,				//	�����㰡 ����
    eJION_TOP_STATE_NONADMISSION,			//	������� ����
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
    eMAPOBJECT_TYPE = 0,			//param1�� �ε��� // ���� 3�̸� ��������Ʈ.
    eMAPOBJECT_MONSTER = 1,         //param2�� �ε��� // ��ġ�� ���� �ڵ�(NPCCode)
    eMAPOBJECT_USE_ALPHA = 2,		//param3�� �ε��� // ���� ���� ��� ����
    eMAPOBJECT_LINKED_MONSTER_REGEN_TYPE = 3, //param4�� �ε��� 
                                              // 2�� : ����� ���Ͱ� ������� �̸� ���� ���� �ʴ´�.
                                              // Ʈ���Ÿ� ���� ���� (REGEN_TYPE_TRIGGER) �ܿ��� �������� �ʴ´�.

    eMAPOBJECT_MAX = 4,				//< 4������ �Ѵ�.
};

// GM ���ѵ�� ����
enum eGM_DB_GRADE
{
    eGM_GRADE_BASE		= 100,			//< ��� GM�� �ƴ�(����)
    eGM_GRADE_FIRST,					//< �⺻ GM����(������ ���ѵ��� WOPS ����)
    eGM_GRADE_SECOND,					//< 2�ܰ�
    eGM_GRADE_THIRD,					//< 3�ܰ�
    eGM_GRADE_MAX,						//< �굵 GM�� �ƴ�(�񱳿�)
    eGM_GRADE_VIRTUAL	= 200,			//< ���� ���� : sts=200 �� �Ѿ��
};

// ī�� ����
enum eCHAO_STATE
{
    eCHAO_STATE_NORMAL,			// �Ϲ�
    eCHAO_STATE_PREV_CHAO,		// ���� ī��
    eCHAO_STATE_CHAO,			// ī��
};

// ��ƼŻ�� ����
enum eREASON_LEAVE_PARTY
{
    eREASON_LEAVE_PARTY_INTEND = 0,         // ���������� �ǵ��ؼ� ��������
    eREASON_LEAVE_PARTY_BAN,                // ���� ��������
    eREASON_LEAVE_PARTY_ABNORMAL,           // ������ ����
    eREASON_LEAVE_PARTY_NOREASON,	        // ��������(�������� ���̵��� ��Ƽ �缳���� ���� ��ƼŻ��)
    eREASON_LEAVE_PARTY_BY_GUILDRELATION,   // ��Ƽ ������ ���� ���踦 ���� ��� ����� ���� Ż��
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    eREASON_LEAVE_PARTY_NOT_MATCH_LEVEL,    // ��Ƽ�� �̼� ���� ���ǿ� �������� ���� ����
    eREASON_LEAVE_PARTY_NOT_CLEAR_MISSION,  // ���� �̼��� Ŭ���� ���� ���� ����
    eREASON_LEAVE_PARTY_NOT_CLEAR_QUEST,    // ���� ����Ʈ�� Ŭ�������� ���� ����
};

enum ePORTAL_TYPE
{ 
    ePORTAL_TYPE_GM,	
    ePORTAL_TYPE_REVENGE, 
    ePORTAL_TYPE_ITEM,
    ePORTAL_TYPE_ITEM_EX,	//��������Ʈ����Ʈ ������
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
    eROOM_SPECIAL_PCBANG_SUPER_ROOM,	// PC�� ���۹�
    eROOM_SPECIAL_ITEM1_SUPER_ROOM,		// ���۹���1 ���������� ���� ���۹�
};
#endif

//---------------------------------------------
//���� ����Ʈ NPC �̿��
//---------------------------------------------
enum eVENDOR_LIST_PAGE_KIND
{
    PAGE_REQUIRE_KIND_NONE,							//�ʱⰪ
    PAGE_REQUIRE_KIND_ALL,							//Ŭ���̾�Ʈ�� Page ��û�� �� ���� - ��ü
    PAGE_REQUIRE_KIND_TITLE,						//�˻��� �� Define ���			   - �����̸�
    PAGE_REQUIRE_KIND_ID,							//�˻��� �� Define ���			   - ID
    PAGE_REQUIRE_KIND_ITEM,							//�˻��� �� Define ���			   - ������ �̸�
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

//�ν��Ͻ� ����
enum eINSTANCE_DUNGEON_KIND
{	// sMAPINFO->byMType ���� Ȯ���
    //	__NA001226_INSTANCE_DUNGEON_EXTENTION__�� ���� �߰��Ǵ� Ÿ�Ե�...
    //	���� ����� MapInfoParser.IndunRule����

    //////////////////////////////////////////////////////////////////////////
    //		(+)MissionRanking (point)
    //		(-)RankingTimer
    INDUN_KIND_TOP_OF_TRIAL		=  0,		// Į���� (������ ž)
    //////////////////////////////////////////////////////////////////////////
    //		(-)MissionRanking
    //		(-)RankingTimer
    //		(+)change-up, class change... (Ȥ 200���� ü�������� �����, ���� ���� �ٸ���, ������ ������ ���� ���� ��.)
    INDUN_KIND_100LEVEL_QUEST	=  1,		// 100���� ����Ʈ �̺�Ʈ ��	__NA000844_080324_100LEVEL_CHAGNEUP_REBUILDING__
    //////////////////////////////////////////////////////////////////////////
    //		(+)MissionRanking <- special
    //		(-)RankingTimer
    //		(+)static dungeon control (������ ������ ������ �� 1���� �����ϴ� �δ�)
    //		(+)use portal enter method
    //		(+)variation logic control rules
    INDUN_KIND_SS_QUEST			=  2,		// Ÿ���� ���	__NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__
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
    INDUN_KIND_MAYA_SCROLL		=  5,		//(lst1024)�߱� ������ ���� �Ұ� ��( ����ġ ȹ��� )
    //////////////////////////////////////////////////////////////////////////
    
    INDUN_KIND_CHAOS_ZONE   	=  6,

    INDUN_KIND_MAX				,
};

struct eZONE_RULESET
{
    enum eRules
    {
        ZONE_RULE_EMPTY								= 0,		// �ʱⰪ, �Ǵ� �δ��� �ƴ� ��� ��
        //////////////////////////////////////////////////////////////////////////
        //	reserved - �ٸ� ���� ���� ���� ��Ʈ���� �䱸�� ��� Mkind�� ���� �и��ǰ�, bMtype�� ���� rule�� ����ȴ�?
        //	ex:
        //		ZONE_RULE_MISSION_RANKING_POINT		= 1<< 0,
        //////////////////////////////////////////////////////////////////////////
        //	for instance dungeon control
        ZONE_RULE_INDUN_SHOW_THE_FLOOR				= 1<< 0,	// �� ���� ó�� (��ž ���)
        ZONE_RULE_OPTION_ROOM_BY_PORTAL             = 1<< 1,    // Portal����� ���� �ɼ�, ex) Portal Mission���� ��ŷ �̵��� ó��
#ifdef _NA_008012_20150130_RANKING_SYSTEM
        ZONE_RULE_INDUN_RANKING_POINT				= 1<< 5,	// �̼Ƿ�ŷ ����, ����Ʈ���
        ZONE_RULE_INDUN_RANKING_TIME_ATTACK			= 1<< 6,	// �̼Ƿ�ŷ ����, Ÿ�Ӿ���
        ZONE_RULE_INDUN_RANKING_TIMER				= 1<< 7,	// �̼Ƿ�ŷ ����, Ŭ���̾�Ʈ Ÿ�̸� ���̱�
#endif // _NA_008012_20150130_RANKING_SYSTEM
        ZONE_RULE_INDUN_QUEST						= 1<<10,	// ����Ʈ�� ���� �δ�, (ü������)
        ZONE_RULE_INDUN_CANT_EQUIP_WEAPON_AND_BLOCK	= 1<<15,	// Ư�� ó��, ������/�� ���ݹ��� ���� �Ұ� ���
        ZONE_RULE_INDUN_SINGLE_PLAYING_MODE			= 1<<16,	// �Ѹ� ���� ����
        ZONE_RULE_INDUN_SHADOW_NOT_PLAYING_MODE		= 1<<17,	// �Ѹ� ���� �Ұ�(������)	//(lst1024) �߱� ������ ���� �Ұ� ��
        ZONE_RULE_INDUN_STATIC_DUNGEON				= 1<<30,	// ���� �ð��� ���������� �ϳ��� �����ϴ� Ư�� �δ�
        //////////////////////////////////////////////////////////////////////////
        //	MAX_BIT_SET							= 1<<31,
    };

    eRules						rules;
};

enum eSCRIPT_CODE
{
    // ��ũ��Ʈ ����/ Ŭ�� �뿪��..
    // 10-199 ������ : ���� �� ����Ŭ�� ���뽺ũ��Ʈ
    // 200-299 ������ : Ŭ������ ��ũ��Ʈ
    // �ݵ�� ���� ���ؼ� ���� �ֽʽÿ�. �ؿܹ��� �ֶ������ ����� �ش� ������ ��ũ��Ʈ�� �ε��ϰ� �˴ϴ�.
    // �߰��� ��ȣ�� Ȯ���ؾ� �ϴ� ��� SCRIPT_LIST_SIZE ���� �÷� �ֽʽÿ�.

    SCRIPT_EMPTY						=	0,					// Control Flag
    SCRIPT_SCRIPT_ALL					=	1,					// Control Flag
    SCRIPT_NOTIFY_SIGNAL				=	2,					// Control Flag
    SCRIPT_CONTROL_FLAG_UPPERBOUNDARY	=	10,					// Control Flag
    SCRIPT_LIST_SIZE					=	300,				// COUNT Flag

//////////////////////////////////////////////////////////////////////////
//	������ ����Ŭ�� ���� ��ũ��Ʈ
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
    SCRIPT_EXCHANGE_REWARD_INFO			=	89, //< [�Ϻ�/�븸] __NA_001027_20080423_RANDOM_REWARD_EVENT_
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
    SCRIPT_ABUSING_OPTION_TXT			=  109,		//< SolarAuth ���� ���� ����	//__NA000000_ENABLE_MASTER_SERVER_SCRIPT_LOADING__
    SCRIPT_COLLECT                      =  110, //< __NA001283_20090221_COLLECTION_PARSER
    SCRIPT_GROUPINFO                    =  111, //�׷� �α� �ý��� �⺻ ���� ��ũ��Ʈ__KR_001355_20090608_GROUPINGSYSTEM_DATA
    SCRIPT_CHAOSZONE_INFO               =  112, //< ī������ ����, _NA_008334_20150608_SONNENSCHEIN 
    SCRIPT_RIDING_INFO                  =  113, //< __NA1390_1391_090915_RIDING_SYSTEM_ITEM_PART__
    SCRIPT_DOMINATIONINFO               =  114, //< _KR_001385_20090907_DOMINATION_PARSER
    SCRIPT_DOMINATION_EVENT_INFO        =  115,
    SCRIPT_ACCUMULATE_POINT             =  116, //< _NA_1419_20100121_ACCUMULATE_POINT
    SCRIPT_PERKINFO                     =  117, //< _NA002217_100728_EP2_PERK_SYSTEM_
    SCRIPT_CUBE_COMPOSE                 =  118, // ť�� �ռ� ��ũ��Ʈ _NA_002253_20100811_CUBE_SYSTEM
    SCRIPT_CUBE_DECOMPOSE               =  119, // ť�� ���� ��ũ��Ʈ _NA_002253_20100811_CUBE_SYSTEM
    SCRIPT_SOCKET_ITEM_RATIO_INFO       =  120, //< _NA_0_20100727_SOCKET_SYSTEM_PARSER
    SCRIPT_ZARD_COMPOSE_INFO            =  121, //< _NA_0_20100727_SOCKET_SYSTEM_PARSER
    SCRIPT_SKILL_INVEN_STORE            =  122, //< _NA_0_20100901_SKILLTREE_SYSTEM (renumbering)
    SCRIPT_ACHIEVEMENT_INFO             =  123, //<-- C_NA_0_20100520_ACSYSTEM_CHANGED
    SCRIPT_CASHCATEGORY                 =  124, //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    SCRIPT_GMRIGHTS                     =  125, //_NA000000_GM_COMMAND_CHANGE_CONCEPT_
    SCRIPT_ZARD_OPTION_INFO             =  126, // ����Ʈ ����� �����ϴ� ��� �ɼ� ��ũ��Ʈ _NA_001699_20110104_SOCKET_OPTION_POINT
    SCRIPT_ZARD_POINT_INFO              =  127, // ��� ����Ʈ ���� ��ũ��Ʈ _NA_001699_20110104_SOCKET_OPTION_POINT
    SCRIPT_MAX_CHAR_LIST                =  128, // ĳ���� �ִ� ��� ���� ��ũ��Ʈ _NA_002050_20110216_ADD_GENDER
    SCRIPT_BATTLEZONE_TREE              =  129, // ��Ʋ�� ������ Ʈ�� ��ũ��Ʈ _NA_20110630_BATTLE_ZONE_RENEWAL
    SCRIPT_FAME_LIST                    =  130, // ���ý��� �� ��ũ��Ʈ _NA_003027_20111013_HONOR_SYSTEM
    SCRIPT_REPUTE_LIST                  =  131, // ���ý��� ���� ��ũ��Ʈ _NA_003027_20111013_HONOR_SYSTEM
    SCRIPT_TITLE_LIST                   =  132, // ���ý��� Īȣ ��ũ��Ʈ _NA_003027_20111013_HONOR_SYSTEM
    SCRIPT_ITEM_SCORE_COMMON            =  133, // ��� �� ���� ��ũ��Ʈ _NA_002935_20110704_ITEM_SCORE
    SCRIPT_NPCREPLACE_INFO              =  134, // _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    SCRIPT_RANDOM_ITEM_INFO             =  135, // ���� ������ ���� ��ũ��Ʈ _NA_003740_20111122_RANDOM_ITEM
    SCRIPT_RANDOM_ITEM_VALUE            =  136, // ���� ������ �� ��ũ��Ʈ _NA_003740_20111122_RANDOM_ITEM
    SCRIPT_RANDOM_ITEM_OPTION_TYPE      =  137, // ���� ������ �ɼ� ���� ��ũ��Ʈ _NA_003740_20111122_RANDOM_ITEM
    SCRIPT_RANDOM_ITEM_KEY              =  138, // ���� ������ Ű ��ũ��Ʈ _NA_003740_20111122_RANDOM_ITEM
    SCRIPT_CURRENCY_INFO                =  139, // ȭ�� ����Ʈ ��ũ��Ʈ _NA004034_20120102_POINT_WALLET_SYSTEM
    SCRIPT_GUILDINFO                    =  140, // ��� �⺻ ���� _NA_003923_20120130_GUILD_RENEWAL
    SCRIPT_RESOURCE_VERSION             =  141, // ���ҽ����� üũ ��ũ��Ʈ _RU_0000_20120312_RUSSIA_CHECK_RESOURCE_VERSION_SCRIPT
    SCRIPT_CHAOSZONE_TREE               =  142, // ī������ Ʈ�� ��ũ��Ʈ _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND (1504���� Ŭ������ ��ũ��Ʈ)
    SCRIPT_BATTLE_GROUND_INFO           =  143, // ���� ��ũ��Ʈ _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SCRIPT_BATTLE_GROUND_FLAG_INFO      =  144, // ���� ��ũ��Ʈ _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SCRIPT_BATTLE_GROUND_TIME_INFO      =  145, // ���� ��ũ��Ʈ _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    SCRIPT_GOLDRUSH_INFO                =  146, // ��巯�� ��ũ��Ʈ _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    SCRIPT_GOLDRUSH_MONSTERPOINT        =  147,  // ��巯�� ��ũ��Ʈ _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    SCRIPT_MONSTER_BOOK                 =  148, // ���͵��� ��ũ��Ʈ _NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SCRIPT_GUILD_FACILITY_INFO          =  150, // ���ü� //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    SCRIPT_ATTENDANCE                   =  160, // �⼮ ��ũ��Ʈ _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SCRIPT_SUN_RANKING                  =  161, // �� ��ŷ ��ũ��Ʈ _NA_008012_20150204_SUN_RANKING_SYSTEM
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#if defined(_NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM) || defined(_NA_008124_20150313_AWAKENING_SYSTEM)
	SCRIPT_AWAKENING_INFO               =  162, // ���� ��ũ��Ʈ _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM // _NA_008124_20150313_AWAKENING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    SCRIPT_NOTIFICATION_INFO            =  163, // �˸� �ý��� ��ũ��Ʈ _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    SCRIPT_SONNENSCHEIN_REWARDINFO      = 164,  // �ҳٻ��� ��������
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    SCRIPT_MINIGAME                     =  165, // �̴� ���� �ý��� ��ũ��Ʈ _NA_008404_20150712_MINIGAME_SYSTEM
#endif // _NA_008404_20150712_MINIGAME_SYSTEM
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	SCRIPT_ONEDAYSBUILDMISSION          =  166, // ���� ���̼� ��ũ��Ʈ _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#endif // _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    //----------------------------------------------------------------------------------------------
    SCRIPT_TEST							=  199, //__NA_000000_20071123_NEW_SCRIPT_PARSING_SYSTEM
    //----------------------------------------------------------------------------------------------

//==================================================================================================
//	Ŭ���̾�Ʈ ���� ��ũ��Ʈ
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

    SCRIPT_TEXTURE_LIST_INFO			=	250,	//!< �ؽ��� �ִϸ��̼�
    SCRIPT_TEXTURE_ANIMATION_INFO	    =	251,	//!< �ؽ��� �ִϸ��̼�

    SCRIPT_CREATECHAR_CUSTOMCAMERA_INFO	=	252,	//!< ĳ���� ����ȭ�� ī�޶� ������
    SCRIPT_LOADINGTEXTURE_INFO	        =	253,	//!< �ε� �̹��� ����Ʈ
    SCRIPT_CHARACTER_CREATION_INFO      =   254,    //!< �����߰�/ĳ���� ��������
    SCRIPT_COLOR_INFO                   =   255,    //!< ���� ���� 

    SCRIPT_TREE_CONTROL                 =   256,    //!< Ʈ����Ʈ�� ����
    SCRIPT_TREE_CONTROL_ICON            =   257,    //!< Ʈ����Ʈ�� ���� ������ �̹���
    
    SCRIPT_TOTAL_GUIDE                  =   258, // ���� ����� ���̵� ��ũ��Ʈ _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_PAGE             =   259, // ���� ����� ���̵� ���� ������ ��ũ��Ʈ _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_NOTICE           =   260, // ���� ����� ���̵� �˸�â ��ũ��Ʈ _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_MESSAGE          =   261, // ���� ����� ���̵� �޼��� ��ũ��Ʈ _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_IMAGE            =   262, // ���� ����� ���̵� �޼��� ��ũ��Ʈ _NA_002521_20110613_TOTAL_GUIDE_SYSTEM
    SCRIPT_TOTAL_GUIDE_TREE_CONTROL     =   263, // ���� ����� ���̵� Ʈ����Ʈ�� ����
    SCRIPT_HOT_KEY_INFO                 =   264, // ����Ű �⺻���� ����

    SCRIPT_SKILLEVENTEFFECT_INFO        =   265, // ��ų �̺�Ʈ ����Ʈ ���� (Ŭ���)	
	SCRIPT_MISSIONHELP_INFO				=   266, // �̼� ���� ����
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
	SCRIPT_COSTUMEITEM_INFO				=	267, // �ڽ�Ƭ ������ ���� (Ŭ���)
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION
	SCRIPT_ENDURANCE_INFO				=   268,// ��� ������ ���� UI ��ũ��Ʈ

    SCRIPT_OPTION_CATEGORY_INFO         =   269, // �ɼ�â�� �׸�����
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SCRIPT_GUILD_FACILITY_IMAGE_INFO    =   270,  // ��� �ü� �̹��� ����
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SCRIPT_MONSTER_BOOK_DROP_ORDER_INFO =   271, // ���͵��� ��ũ��Ʈ _NA_007086_20140318_MONSTERBOOK
    SCRIPT_MONSTER_BOOK_DROP_ITEM       =   272, // ���͵��� ��ũ��Ʈ _NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	SCRIPT_STATUE_INFO                  =   273, // ���� ����
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    SCRIPT_SPA_ZONE_INFO                =   274, // ��õ ��Ÿ�� ����
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    SCRIPT_GUILDMISSIONTREE             =   275, // ���̼� Ʈ������
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
    eINTERCEPTION_TRADE,		//�ŷ� ����
    eINTERCEPTION_WHISPER,	//�Ӹ� ����
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
    eATTR_EQUIP_TYPE_LEVEL		=	0,							// ��ȿ���� �ʴ� �Ӽ�
    eATTR_EQUIP_TYPE_STR		=	1,							// ��ȿ���� �ʴ� �Ӽ�
    eATTR_EQUIP_TYPE_DEX		=	2,							// ��ȿ���� �ʴ� �Ӽ�
    eATTR_EQUIP_TYPE_VIT		=	3,							// ��ȿ���� �ʴ� �Ӽ�
    eATTR_EQUIP_TYPE_INT		=	4,							// ��ȿ���� �ʴ� �Ӽ�
    eATTR_EQUIP_TYPE_SPR		=	5,							// ��ȿ���� �ʴ� �Ӽ�
    eATTR_EQUIP_TYPE_SKILL1		=	6,							// ��ȿ���� �ʴ� �Ӽ�
    eATTR_EQUIP_TYPE_SKILL2		=	7,							// ��ȿ���� �ʴ� �Ӽ�	
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

enum ePORTAL_MOVE_TYPE	// �̵�Ÿ��( 0 : �ʵ��̵�, 1 : ���� NPC �̵�, 2 : ���� ����Ʈ 3: �������) // MapEnterancePortal.txt �� �̵�Ÿ�� 
{
    ePORTAL_MOVE_FIELD = 0,
    ePORTAL_MOVE_NPC,
    ePORTAL_MOVE_WAYPOINT,
    ePORTAL_MOVE_AIRSHIP,
};

// ���� �󿡼� �ֺ��� Ư�� ���� ������ ���� �ʿ䰡 ���� ��... �ߺ� ���� ����
enum eSPECIAL_RENDER_EFFECT
{
    eSPECIAL_RENDER_EFFECT_NONE					= 0,
    eSPECIAL_RENDER_EFFECT_CHANGEofCLASS_1st	= 1<<0,
    //
    eSPECIAL_RENDER_EFFECT_SOLO_BADGE			= 1<<10,
    eSPECIAL_RENDER_EFFECT_COUPLE_BADGE			= 1<<11,
    eSPECIAL_RENDER_EFFECT_2nd_EFFECT_ITEM		= 1<<12,
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_01		= 1<<13,	// �� �߱��� �߰� (WAVERIX)(080715)
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_02		= 1<<14,
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_03		= 1<<15,
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_04		= 1<<16,
    eSPECIAL_RENDER_EFFECT_EFFECT_ITEM_05		= 1<<17,
};

//	(Waverix)(NOTE)(071125)(HARDCODE)
//
//	-- ���� ���忡���� Ư�� Item�� ���õ� ȿ�� ���� ���θ� �Ǵ��ϱ� ����� ������ ����
//	(�������� ���־� ���� ȿ���� Ŭ�󿡼��� ����Ѵ�.)
//	��ũ��Ʈ�� �ذ��Ҷ� ġ��, �ᱹ ��ũ��Ʈ �� �߰� �Ǵ� ���־� ���� ȿ���� ������ �˾ƾ� �Ѵ�.
//	(RENDER ������ BRD����)
//	�׸���, �� Ÿ�Ե� ������ �˾ƾ� �Ѵ�. �̷� ���� ������ ��Ȳ ����.
//	-- (071125) �� ����. ũ�������� �̺�Ʈ�� ���� Ư���� ó���� �䱸�ǰ� �ִ�. ��ȹ ��ü�� ������ ������ ��
//	�ϸ鼭�� �񶧸��� �̹��� ���̰� �����ϸ鼭 �߻��ϰ� �ִ� ������� �� �� �ִ�. ��... �̰͵��� ��� ������ ��
//	�ִ� ������ ������ ������ ���ߴٰ� �߱��Ѵٸ� �� �� ����������...
//	�������ν�� �ٸ� ��Ȳ�� �������� �ʰ� ������, ���� ���� ������ ó���� ���� ���,
//	�װͰ� ���� ��� �����ϴ� ���� �� ����� ������ ���� �Ŷ� ����Ѵ�.
//	-- �ϴ� �Ʒ��� ���� Ư���� ���־� Ÿ�Կ� ���� ó�� �˻��� ���� hard-code�� �Է��� �д�.
//	eSPECIAL_RENDER_EFFECT ������ ���� ���̸�, ���� �����ϴ� �������� ������.
enum eSPECIAL_RENDER_EFFECT_ADDIN_INFO_IN_SERVER
{
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_SOLO_BADGE		= 40198,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_COUPLE_BADGE		= 40199,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_2nd_EFFECT_ITEM	= 40211,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_01	=  9014,	// �� �߱��� �߰� (WAVERIX)(080715)
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_02	=  9015,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_03	=  9016,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_04	=  9017,
    eSPECIAL_RENDER_EFFECT_ADDIN_INFO_EFFECT_ITEM_05	=  9018,
};

enum GuildWarehouseLogType
{
    ITEM_IN,    //������ �԰�
    ITEM_OUT, //������ ���
    MONEY_IN, //�� �Ա�
    MONEY_OUT, //�� ���
};

enum eGUILDWAREHOUSE_UPDATE_FACTOR
{
    UPDATE_FACTOR_COMMON,				// ��� â�� ������Ʈ�� ������ ��ġ�� �ʴ� ������Ʈ
    UPDATE_FACTOR_CHAR_UPDATE,		//ĳ������ �ֱ����� ������Ʈ
    UPDATE_FACTOR_LOGOUT,				//ĳ���� �α׾ƿ�
    UPDATE_FACTOR_TIEM_OUT,				//��� â�� ���½ð� Ÿ�Ӿƿ�
    UPDATE_FACTOR_NORMAL_CLOSE,		//�������� â�� �ݱ�
    UPDATE_COMPLETE,
};


// T_EventUser DB Table �ٸ������� ����ϱ� ���� �����ڵ�
enum eEVENT_MODE_MULTIUSE
{
    eEVENT_MULTIUSE_START				= 10000,
    eEVENT_MULTIUSE_SSQ_WINNER_REWARD	,		// ���� ����					10001
    eEVENT_MULTIUSE_SSQ_QUEST_FAIL_INFO	,		// ����Ʈ ���� ���� ��������	10002
    eEVENT_MULTIUSE_SSQ_REFUND			,		// ����� ȯ��					10003
};


// __NA_S00015_20080828_NEW_HOPE_SHADOW
//�׾������� ���� 
enum eCHAR_DEAD_TYPE
{
    eCHAR_DEAD_NOT_DEAD			= 0,			//��������.
    eCHAR_DEAD_NORMAL			= 1,			//�Ϲ������� ����
    eCHAR_DEAD_SUCTION			= 2,			//�׾ HP/MP ����.
    eCHAR_DEAD_SUMMON			= 3,			//�׾ ��ȯ�Ǿ� ����� ����.
};

enum eSUMMON_TYPE
{
    eSUMMON_NORMAL				= 0,			//�Ϲݼ�ȯ
    eSUMMON_DEAD_MONSTER		= 1,			//�������ͼ�ȯ
    eSUMMON_CRYSTALWARP         = 2,            // ũ����Ż ���� //_NA001385_20090924_DOMINATION_SKILL
    eSUMMON_IMMOVABLE           = 3,            // ������ ��ȯ�� //_NA_007667_20141001_WITCHBLADE_SKILL

    eSUMMON_MAX
};

//__NA001338_090519_TUTORIAL_FILED_TYPE_CHANGE__
//  (WAVERIX) (NOTE) ������ �ð��� �帣�鼭 ���� ������ �ʴ� ��������� �����ϰ� �Ѵ�.
//  MapCode���� ��������, �׷��� ������ ��� ó���ϱ⿡�� �������� �κ��� ����.
//  �׷��� �Ϻ� ����ó���� ���ؼ� ����ϴµ�, ã�Ƶ��� ������ �����ϹǷ� ������ ���� �Ѵ�.
//  #1) 20206 (Ʃ�丮�� �ʵ�)�� ���
//
struct HardCode
{
    enum eMapCodes{
        MAPCODE_for_TRINGGEL       = 10001,
        MAPCODE_for_VALDCHEN       = 10002,
        MAPCODE_for_GM             = 10005,    // GM ��(�̱״Ͻ�)   //__NA_0_20090714_POTAL_ITEM_DONOT_MOVE_GMMAP
        MAPCODE_for_TUTORIAL_FIELD = 20206,    //(090520) ���ҵ� ����
		MAPCODE_for_SONNENSCHEIN   = 43,	   // �ҳٻ���
		MAPCODE_for_CROSSINGSPACEOFTIME = 46,  // �ð��� ��������
    };
    enum eItems {
        ITEMCODE_of_ETHERIA = 4983, // _NA000000_100427_ITEM_CUSTOMIZING_CONTENTS_
    };
};

//__KR_001355_20090608_GROUPINGSYSTEM_DATA
//�׷� �ý��ۿ��� ���� �ڽ��� ������ �ִ� �׷��� ������ ����
enum eGROUP_STAT
{
    //�׷쳻 ���� �ڽŰ� �׷���鰣�� ���踦 �˱� ���� STAT
    GROUP_LEVEL_HIGH				= 1,	//�� �׷��� ���� �����̴�.
    GROUP_LEVEL_LOW					= 2,	//�� �׷��� ���� �����̴�.
    GROUP_LEVEL_OUT					= 3,	//�� �׷��� Ż�� �ߴ�.
    GROUP_LEVEL_HIGH_OVERDATE       = 4,    //�� ���� �׷� �ɹ��� �������� 1������ �Ѿ���.

    //GameServer���� �ɸ����� �׷� ���¸� �˱� ���� STAT
    GROUP_HAVE_NONE					= 10,	//�׷��� ������ ���� �ʴ�.
    GROUP_HAVE_ONLINE_HIGH			= 11,	//���� �ɹ��� ������ �ִ�.(���� ���´� �¶���)
    GROUP_HAVE_OFFLINE_HIGH			= 12,	//���� �ɹ��� ������ �ִ�.(���� ���´� ��������)
    GROUP_HAVE_ONLINE_LOW			= 13,	//���� �ɹ��� ������ �ִ�.(���� ���´� �¶���)
    GROUP_HAVE_OFFLINE_LOW			= 14,	//���� �ɹ��� ������ �ִ�.(���� ���´� ��������)

    GROUP_STAT_ERROR                = 1000, //�׷� �ɹ� ������ �߸� ���� �Ǿ��� ���
};

//__NA_0_20090818_SYSTEM_MEMO_SUPPORT
// �ý��� �޸� Ÿ��
enum eSYSTEM_MEMO_TYPE
{
    NORMAL_MEMO = 0,
    SYSMEMO_LOTTO_WIN = 1,          // �ζ� ��÷ �ȳ� ����: SYSMEMO_LOTTO_WIN|EntryIndex|DrawNum|Rank|Prize
    SYSMEMO_GROUP_MGR = 2,          // �׷� ����ġ �ȳ�
    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
    SYSMEMO_FRIENDINVITATION_INVATES_TART		= 3,          // �ڽ��� ģ����õ�� ģ���� ĳ���͸� ���� ģ���߰� �̺�Ʈ �����Ѵ�.
    SYSMEMO_FRIENDINVITATION_INVATE_END			= 4,          // �ڽ��� ģ����õ�� ĳ���Ͱ� �̺�Ʈ ���᷹������ �������Ǿ� �̺�Ʈ �����Ѵ�.
    SYSMEMO_FRIENDINVITATION_INVATE_PRESENT		= 5,		  // �ڽ��� ģ����õ�� ĳ���Ͱ� �̺�Ʈ ������ �޼��Ͽ� ������ �����Ѵ�.
    SYSMEMO_FRIENDINVITATION_BEINVATED_START	= 6,          // ģ����õ ���� ������ ĳ���͸� ���� ģ���߰� �̺�Ʈ �����Ѵ�.
    SYSMEMO_FRIENDINVITATION_BEINVATED_END		= 7,          // ģ����õ ���� ĳ���Ͱ� �̺�Ʈ ���᷹������ �������Ǿ� �̺�Ʈ �����Ѵ�.
    SYSMEMO_FRIENDINVITATION_BEINVATED_PRESENT	= 8,          // ģ����õ ���� ĳ���Ͱ� �̺�Ʈ ������ �޼��Ͽ� ������ �����Ѵ�.	
    //}
    SYSMEMO_DOMINATION_RETURN_MONEY = 9,                      //���� ������ ���� ��ſ� ���� ���� ��� ���޵Ǵ� ����
    SYSMEMO_DOMINATION_REWARD_MONEY = 10,                     //���� ���������� ��� ��忡�� ��� �ݾ��� �Ϻθ� ����

    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION //���ο� �ý��� ���� Ÿ�� �߰�(��/�� ����)
    SYSMEMO_LOTTO_WIN_NEW = 11,
    SYSMEMO_DOMINATION_RETURN_MONEY_NEW = 12,                     
    SYSMEMO_DOMINATION_REWARD_MONEY_NEW = 13,
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    SYSMEMO_WORLD_RANKING_GRADE = 14,
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SYSMEMO_GUILD_JOIN_REQUEST = 15,                          // ��� ���� ��û �˸� �޼���
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    SYSMEMO_SUN_RANKING_SETTLE_RESULT   = 16,                 // �㷩ŷ ���� ��� �˸� �޼���
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
    SYSMEMO_MAX,
};

//_NA001385_20090924_DOMINATION_EVENT_CONTROLL
enum eDOMINATION_EVENT_STATE
{
    DOMINATION_EVENT_AUCTION_START = 0,         // ������ ���� ����
    DOMINATION_EVENT_AUCTION_END,               // ������ ���� ����
    DOMINATION_EVENT_SELECT_ATTACKGUILD,        // ������ ���� ��� ���� �̺�Ʈ
    DOMINATION_EVENT_SELECT_ATTACKGUILD_END,    // ���� ��� ���� ���� �̺�Ʈ ����
    DOMINATION_EVENT_SYSTEM_RESET,              // ������ ������ ���õ� ��� �ý��� �ʱ�ȭ �ð�
    DOMINATION_EVENT_WAIT,                      // ������ ������ ������ �� �̺�Ʈ���� ��� �ð�
    DOMINATION_EVENT_START,                     // ������ ����
    DOMINATION_EVENT_END,                       // ������ ����
    DOMINATION_EVENT_AUCTION_START2,            // ������ ������ ��� �����
    DOMINATION_EVENT_MAX                        // MAX ��
}; 

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
//���������� ���� ��� �ý����� ������� �����Ƿ� ��ũ��Ʈ���� �����ϰ� �Ʒ��� ���� ����� ó���Ѵ�.
//���� ������ ��� �����ؾ������� ����ġ���� ������ �������� ��ġ�̴�.
enum eDOMINATION_EVENT_TIME
{
    DOMINATION_EVENT_AUCTION_START_TIME = 0,
    DOMINATION_EVENT_AUCTION_END_TIME = 1,
    DOMINATION_EVENT_SELECT_ATTACKGUILD_TIME = 300,
    DOMINATION_EVENT_SELECT_ATTACKGUILD_END_TIME = 500,
    DOMINATION_EVENT_AUCTION_START2_TIME = 240000,
};

//���ο� ����� ������ ������ ���迡 ���� dominationInfo.txt�� Į���� �߰��ϰ� �Ʒ��� ���� ����
enum eDOMINATION_TYPE//ENTER_CONDITION_TYPE
{
    DOMINATION_TYPE_NORMAL = 0,     //�⺻ ������       //��尡 �־�� ���� ����
    DOMINATION_TYPE_MONDSCHIEN = 1, //��Ʈ���� ������   //���� ���� ������ �־�� ���� ����
};
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

enum eDOMINATION_EVENT_TYPE
{
    DOMINATION_INIT = 0,
    DOMINATION_INFO_SET,
    DOMINATION_DEFFENSEWALL_SET,        // ��� �� ��
    DOMINATION_SUCCESS_DEFENCE,         // ��� ����
    DOMINATION_SUCCESS_OFFENCE,         // ���� ���� (���� �ٲ�)
    DOMINATION_DISGUILD_SET,            // ���� ��� ����
    DOMINATION_GM_OFFENCE_GUILD_SET,    // gm ��ɾ ���� ���� ��� ����
    DOMINATION_GM_DEFENCE_GUILD_SET,    // gm ��ɾ ���� ��� ��� ����
    DOMINATION_ADD_DEFFENSEWALL_POINT,  // ������ �溮 ����Ʈ ����
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    DOMINATION_TURN_OVER,               // (��Ʈ����) ���� ����, ���� ����
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
}; 

enum eDOMINATION_ADD_GUILD_INFO_TYPE
{
    DOMINATION_JOIN_COUNT       = 1<<0,       // ������ ���� Ƚ�� �˸�
    DOMINATION_DEFENSE_COUNT    = 1<<1,       // ������ ��� Ƚ�� �˸�
    DOMINATION_HUNTING_COUNT    = 1<<2,       // ������ ���� ��� Ƚ�� �˸�
    DOMINATION_MAP_COUNT        = 1<<3,       // ������ ���� ���� Ƚ�� �˸�
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
// ������ ���� ������ ������� ǥ��
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
    SONNENSCHEIN_EVENT_STATE_NONE = 0,          // �Ϲݻ���
    SONNENSCHEIN_EVENT_STATE_WAIT,      // �ʱ�ȭ, �غ�, ���������
    SONNENSCHEIN_EVENT_STATE_START,         // ����, ����, ���ΰ���
    SONNENSCHEIN_EVENT_STATE_END,           // ���� (�ð��� ����)
    SONNENSCHEIN_EVENT_STATE_STAMP_COMPLETE, // ���οϷ� (��ǥ�޼����� ���� ����)
    SONNENSCHEIN_EVENT_STATE_DESTROY,       // �Ҹ���
};

// _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
enum eGUILDMISSION_STATE
{
	eGUILDMISSION_NONE = 0,								// �⺻��
	eGUILDMISSION_ENTERWAIT,							// ������ ������ ��ٸ��� ����
	eGUILDMISSION_MATCHED,								// ���� ���� ����
	eGUILDMISSION_MATCHEDEND,							// ���� ���� �Ϸ�
	eGUILDMISSION_RESULT,								// ��� ����
	eGUILDMISSION_END,									// ���� ����
	eGUILDMISSION_KICK_USER,							// ���� ���� ���� 
	eGUILDMISSION_DESTROY,								// �� �ı�
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
#define FRIENDINVITATION_RECALL_MSGBOX_MAXTIME 15000 //ģ�� ��ȯ��û�� �Ҷ� ������ �޽����ڽ��� ������� �������� �ð�

// _NA_0_20100722_AUTO_SYSTEM
enum eAUTO_HUNTING_TYPE
{
    eAUTO_TYPE_INVALID = 0,
    eAUTO_RANGE_FIELD,              // ��ɼ���_����ü
    eAUTO_RANGE_PARTY,              // ��ɼ���_��Ƽ���ֺ�
    eAUTO_RANGE_USER,               // ��ɼ���_Ư������(�ڽ� �ֺ�)
    eAUTO_RANGE_BOSS,               // ��ɼ���_�������
    eAUTO_PICKUP_ALL,               // �ݱ⼳��_��������
    eAUTO_PICKUP_NORMAL,            // �ݱ⼳��_�Ϲݾ�����
    eAUTO_PICKUP_QUEST,             // �ݱ⼳��_����Ʈ������
    eAUTO_PICKUP_STONE,             // �ݱ⼳��_���������
    eAUTO_PICKUP_WEAPON,            // �ݱ⼳��_���������
    eAUTO_PICKUP_ARMOR,             // �ݱ⼳��_��������
    eAUTO_PICKUP_AC,                // �ݱ⼳��_AC������
    eAUTO_PICKUP_HEIM,              // �ݱ⼳��_����
    eAUTO_PICKUP_ITEMTYPE,          // �ݱ⼳��_Ư��������
    eAUTO_CURE_HP,                  // ȸ������_HP
    eAUTO_CURE_MPSP,                // ȸ������_MPSP
    eAUTO_CURE_INSTANT,             // ȸ������_���ȸ��
    eAUTO_CURE_PET_FEED,            // ȸ������_���(���� ����)
    eAUTO_CURE_SITDOWN_HP,          // ȸ������_�޽�(�ɱ�, �ش� HP �����ϰ��)
    eAUTO_CURE_SITDOWN_MP,          // ȸ������_�޽�(�ɱ�, �ش� MP �����ϰ��)
    eAUTO_REBIRTH_VILIAGE,          // �ڵ���Ȱ_������Ȱ
    eAUTO_REBIRTH_WAITING,          // �ڵ���Ȱ_��Ȱ���
    eAUTO_REBIRTH,                  // �ڵ���Ȱ_���ڸ� ��Ȱ
    eAUTO_RETURNSCROLL_INVEN,       // �����̵�_�κ��丮
    eAUTO_RETURNSCROLL_POTION,      // �����̵�_ȸ������
    eAUTO_RETURNSCROLL_PET_FEED,    // �����̵�_���
    eAUTO_RETURNSCROLL_DURATION,    // �����̵�_������
    eAUTO_SELL_ALL,                 // �ڵ��Ǹ�_��������             
    eAUTO_SELL_NORMAL,              // �ڵ��Ǹ�_�Ϲݾ�����             
    eAUTO_SELL_QUEST,               // �ڵ��Ǹ�_����Ʈ������           
    eAUTO_SELL_STONE,               // �ڵ��Ǹ�_���������
    eAUTO_SELL_WEAPON,              // �ڵ��Ǹ�_���������
    eAUTO_SELL_ARMOR,               // �ڵ��Ǹ�_��������
    eAUTO_SELL_AC,                  // �ڵ��Ǹ�_AC������
    eAUTO_BUY_RETURNSCROLL,         // �ڵ�����_��ȯ��ũ��
    eAUTO_BUY_PET_FEED,             // �ڵ�����_�Ϲݻ��
    eAUTO_BUY_DURATION,             // �ڵ�����_��������ü����
    eAUTO_BUY_POTION_LEVEL,         // �ڵ�����_ȸ������
    eAUTO_BUY_POTION_COUNT,         // �ڵ�����_ȸ������
    eAUTO_COMMUNITY_PARTY_JOIN,     // Ŀ�´�Ƽ_��Ƽ����
    eAUTO_COMMUNITY_PARTY_BLOCK,    // Ŀ�´�Ƽ_��Ƽ�ź�
    eAUTO_COMMUNITY_SAVE_CHAT,      // Ŀ�´�Ƽ_ä�ó�������
    eAUTO_COMMUNITY_SHOUT,          // Ŀ�´�Ƽ_��ġ��
    eAUTO_COMMUNITY_CHANNEL,        // Ŀ�´�Ƽ_ä������

    //! 2011.9.16 / i4u4me /  
    eAUTO_RETURNSCROLL_RETURNFIELD, // �����̵�_�̵��� ���� ��ġ�� �ٽ� �ʵ�� //!<44

    eAUTO_TYPE_MAX,        
};

////////////////////////////////////////////////////////////////////////////////////////////////////


#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM

// ��Ƽ ���� ��� ���
struct PartyFindingListConst
{
    enum EnumValue
    {
        kMinLength = 0, // �ּ� ��Ƽ ���� �� 
        kMaxLength = 100 // �ִ� ��Ƽ ���� ��
    };

    typedef BYTE ValueType;
    
    static ValueType GetValue(const EnumValue enum_value)
    {
        //assert(enum_value >= kMinLength && enum_value <= kMaxLength);
        return static_cast<ValueType>(enum_value);
    }

}; // PartyFindingListConst

// ��Ƽ ���� ���� ���
struct PartyFindingTitleConst
{
    enum
    {
        kMinLength = 5, // �ּ� ���ڼ�
        kMaxLength = 30 // �ִ� ���ڼ�
    };

    // ���� ����
    enum { kBufferLength = kMaxLength + 1 };

}; // PartyFindingTitleConst

// ��Ƽ ���� ���� ���
struct PartyFindingGoalConst
{    
    enum EnumValue
    {
        kMinBound = -1, 
        kMission, // �̼�
        kQuest, // ����Ʈ
        kAC, // AC
        kCursedTower, // ���ֹ��� ž
        kEtc, // ��Ÿ
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

// ��Ƽ ���� �ο��� ���
struct PartyFindingNumOfMemberConst
{    
    enum EnumValue
    {
        kMin = 2, // �ּ� �ο�
        kMax = 10 // �ִ� �ο�
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

// ��Ƽ ���� ���� ���� ���
struct PartyFindingResponseTypeConst
{
    enum EnumValue
    {
        kMinBound = -1, 
        kUnknown, // �� �� ���� ����
        kRegister, // ��Ƽ ���� ���
        kUpdate, // ��Ƽ ���� ����
        kGetList, // ��Ƽ ���� ��� ���
        kJoin, // ��Ƽ ���� ����
        kCancel, // ��Ƽ ���� ���
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

// ĳ���� �ܸ� ���� ���� ���
struct ChangeCharacterAppearanceTypeConst
{
    enum EnumValue
    {
        kNone = 0x00, // ���� ����
        kFace = 0x01, // ��
        kHair = 0x02, // ���
        kHeight = 0x04, // Ű
        kGender = 0x08, // ����
        kAll = kFace|kHair|kHeight|kGender // ���
    };

    typedef BYTE ValueType;

    // ������ ���� ���� ����� ���� �������� ��ȯ�Ѵ�.
    inline static ValueType EnumToValue(const EnumValue enum_value)
    {
        return static_cast<ValueType>(enum_value);
    }

    // ��� ���� �Ǵ��� �˻��Ѵ�.
    inline static bool IsChangeAll(const ValueType change_type_value)
    {
        return (change_type_value == kAll);
    }

    // ���� ���� �Ǵ��� �˻��Ѵ�.
    inline static bool IsChangeFace(const ValueType change_type_value)
    {
        return ((change_type_value & kFace) == kFace);
    }

    // �� ���� �Ǵ��� �˻��Ѵ�.
    inline static bool IsChangeHair(const ValueType change_type_value)
    {
        return ((change_type_value & kHair) == kHair);
    }

    // Ű�� ���� �Ǵ��� �˻��Ѵ�.
    inline static bool IsChangeHeight(const ValueType change_type_value)
    {
        return ((change_type_value & kHeight) == kHeight);
    }

    // ������ ���� �Ǵ��� �˻��Ѵ�.
    inline static bool IsChangeGender(const ValueType change_type_value)
    {
        return ((change_type_value & kGender) == kGender);
    }
}; // ChangeCharacterAppearanceTypeConst

////////////////////////////////////////////////////////////////////////////////////////////////////

enum NPC_SHOP_TYPE
{
    SHOP_TYPE_OF_NARMAL = 1,    //�Ϲ� ����
    SHOP_TYPE_OF_ITEM,          //������ ����
    SHOP_TYPE_OF_ACCUMULATE,    //���� ����Ʈ ����
    SHOP_TYPE_OF_CURRENCY,      //ȭ�� ����Ʈ ����
    SHOP_TYPE_MAX,
};

enum ePARTY_TYPE
{
    ePARTY_FIELD,
    ePARTY_ROOM,
    ePARTY_INSTANCE,
    ePARTY_MAX
};

//�ſ� �߿� : BattlezoneMissiontree��ũ��Ʈ Kind ������ �������� ����
//�̼� Ÿ���� �߰� �Ǹ� �ݵ�� �Ʒ� �׸��߰� �Ǿ�� �Ѵ�.
enum PartyZoneType{
    PartyOfNone = 1,                //�⺻
    PartyOfMission = 2,             //�̼�
    PartyOfTopOfTrialByCash = 3,    //��ž ĳ�� ������
    PartyOfTopOfTrialByNormal = 4,  //��ž �⺻ 
    PartyOfPvPByPersonal = 5,       //PVP������
    PartyOfPvPByTeam = 6,           //PVP����
    PartyOfEvent = 7,               //�̺�Ʈ �̼�
    PartyOfSSQ = 8,                 //Ÿ���� ���
    PartyOfHardcore = 9,            //�ϵ��ھ�
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    PartyOfChaosZone = 10,          //ī������
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
    PartyOfTopOfTrialLight_normal = 11, // ���ֹ���ž ���ǽ÷�
    PartyOfTopOfTrialLight_cash = 12, // ���ֹ���ž ���ǽ÷�(ĳ��������)
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
    kPlayerStateMatch, // ���� �������
    kPlayerStateDeath, // ���� ����
    kPlayerStateStandbyResurection, // ��Ȱ������
    kPlayerStateStandbyResurectionRefuse, // ��Ȱ��� �ź� ����
};
enum BattleGroundStrongpointState
{
    kStrongpointStateNone = 0,      // �߸� ����
    kStrongpointStateWait,          // ��� ����(����)
    kStrongpointStateWaitAttack,    // ��� ����(����)
    kStrongpointStateConquer,       // ���ɻ���
    kStrongpointStateDefence,       // ���ɹ��
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
// ��巯�ÿ��� �������Ʈ�� ���� ��޼���
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
    eATTENDANCE_INFO_NOT_ATTEND = 0,    // �⼮ ����
    eATTENDANCE_INFO_ATTENDED,          // �⼮ ��
    eATTENDANCE_INFO_ALEADY_GOT_REWARD, // ���� ����
    eATTENDANCE_INFO_MAX,
};
enum eATTENDANCE_TYPE
{
    eATTENDANCE_CALENDAR = 0,       // �޷���
    eATTENDANCE_SUPERPOSITION,      // ��ø��
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

//���� : ���α� �߰��ÿ��� �ݵ�� GuildInGameLog::_GetLogCategory()�� �ݵ�� �߰�
enum GuildInGameLogType{
    //GuildGrowthLog
    GuildCreate,		    //��� â��
    GuildLevelUp,	        //��� ������
    GuildLevelDown,         //��� ���� �ٿ�
    GuildEquipmentApply,    //��� �ü� Ȱ��ȭ
    GuildEquipmentLevelup,  //��� �ü� ���� ��
    GuildEquipmentLevelDown,//��� �ü� ���� �ٿ�
    GuildGivePoint,         //��� ����Ʈ ���
    GuildChangeName,        //��� �̸� ����
    GuildRegisterMark,      //��� ��ũ ���
    GuildRegisterAnnounce,  //��� ���� ���
    GuildRegisterIntroduce, //��� �Ұ� ���

    //GuildMemberLog
    GuildMemberJoin,	    //����
    GuildMemberKick,	    //����
    GuildMemberOut,	        //Ż��
    GuildDutyChange,	    //��å ����
    GuildChangeMaster,      //���� ����

    //GuildRelationLog
    GuildRelationAllienceSet,   //��� ���� ����
    GuildRelationAllienceCancle,//��� ���� ����
    GuildSendHostilitySet,  //��� ���� ���� ����
    GuildRecvHostilitySet,  //��� ���� ���� ����
    GuildBothHostilitySet,  //��� ���� ���� ����
    GuildHostilityCancel,   //��� ���� ���� ����

    //GuildDominationLog
    GuildSelectedDomiAttack,//������ ���� ��� ����
    GuildNotifyDomiAttack,  //�� ������ ���� ��尡 ����
    GuildSuccessDomiAttack, //������ ���� ����
    GuildSuccessDomiDefense,//������ ���� ����
    GuildAuctionBatting,    //������ ��� ����
    GuildFailAuction,       //������ ���� ����
    GuildPenaltyTime,       // ������ �г�Ƽ //_NA_000000_20130429_DOMINATION_PENALTY

    GuildInGameLogMax,
};

enum ObtainType
{
    kObtainType_Disable = 0,    //��Ƽ������ȹ�� ��Ȱ��
    kObtainType_Enable = 1,     //��Ƽ������ȹ�� Ȱ��
};

enum ToggleItemType
{
    kToggleItem_Badge,
    kToggleItem_PCBang,
    kToggleItemMax,
};

//_NA_007667_20141001_WITCHBLADE_SKILL
// ��ų �ߵ� ���� �˻� (SkillInfo Ability Option2)
enum eSKILL_CONDITION
{
    eSKILL_CONDITION_NONE = 0,
    eSKILL_CONDITION_BELOW_HP = 1,    // Ư�� HP�� '����'�� ���
    
    eSKILL_CONDITION_MAX
};

//_NA_007667_20141001_WITCHBLADE_SKILL
enum eSKILL_UPDATE_CONDITION
{
    eSKILL_UPDATE_CONDITION_RANDOM = 0,     //npcinfo�� SkillUpdateTime���� ������
    eSKILL_UPDATE_CONDITION_FIXED = 1,      //npcinfo�� SkillUpdateTime���� ������

    eSKILL_UPDATE_CONDITION_MAX
};

// _NA_007196_MODIFY_INVITATIONFRIEND_REWARD_LEVELINFO
// ģ���ʴ� ���� ���޵Ǵ� ��������
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
    ePEAKTIME_PRENOTIFY = 0,    // ������ �����˸�
    ePEAKTIME_START,            // ��ũŸ���̺�Ʈ ����
    ePEAKTIME_END,              // �̺�Ʈ ����
    ePEAKTIME_MAX
};

//_NA_008012_20150204_SUN_RANKING_SYSTEM
/* ���� - �Ʒ� ������ DB�� ����Ǵ� Ÿ���̹Ƿ� ������� �ʾƾ��Ѵ�. �߰��Ǵ� Ÿ���� END���� �߰��Ǿ���Ѵ�*/
enum eSUN_RANKING_TYPE
{
	eSUN_RANKING_TYPE_START = 0,
	eSUN_RANKING_TYPE_BEST_OF_BEST = eSUN_RANKING_TYPE_START,    // Best of Best
	eSUN_RANKING_TYPE_ELETE_WARRIOR,   // ����Ʈ ����
	eSUN_RANKING_TYPE_LEVEL_KING,      // ������
	eSUN_RANKING_TYPE_LEGEND_EQUIP,    // �������� ���
	eSUN_RANKING_TYPE_STRONG_ONE_SHOT, // ������ �ѹ�
	eSUN_RANKING_TYPE_BEST_ATTACKER,   // �ְ��� ���ݼ�
	eSUN_RANKING_TYPE_MASTER_OF_HEAL,  // ġ���� ����
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
    eSUN_RANKING_SETTLE_MONTH = 0,  // ���� ����
    eSUN_RANKING_SETTLE_WEEK,       // �ְ� ����
};
enum eSUN_RANKING_STATE
{
    eSUN_RANKING_STATE_INIT = 0,            // �ʱ�ȭ
    eSUN_RANKING_STATE_LOAD,                // �ε�
    eSUN_RANKING_STATE_RUN,                 // ���� ��
    eSUN_RANKING_STATE_SETTLE_WAIT,         // ���� ��
    eSUN_RANKING_STATE_SEND_MEMO,           // �޸� ����
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
    eNotificationEnchant,       // ��æƮ
    eNotificationAwakening,     // ����
    eNotificationDrop,          // ���
    eNotificationCraft,         // ����
    eNotificationEvolution,     // ��ȭ
    eNotificationBox,           // ����
    eNotificationAcReward,      // AC����
    eNotificationMissionReward, // �̼Ǻ���
	eNotificationBingoReward,   // ������
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
    eMINIGAME_BINGO_STATE_EMPTY = 0,     // �� ĭ
    eMINIGAME_BINGO_STATE_FILLED,        // ä���� ĭ
    eMINIGAME_BINGO_STATE_FREE,          // ���� ĭ
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
