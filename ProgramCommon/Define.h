#ifndef __DEFINE_H__
#define __DEFINE_H__

#include "Version.h"
#include "Define.Build.h"
#include "DefineForLog.h"

#if defined(__KR) || defined(_KOREA)
	#define GENERAL_SHOUT_LIMIT_LEVEL	1
#elif defined(__TW) || defined(_TAIWAN)
	#define GENERAL_SHOUT_LIMIT_LEVEL	20
#elif defined(__CN) || defined(_CHINA)
	#define GENERAL_SHOUT_LIMIT_LEVEL	15
#elif defined(__JP) || defined(_JAPAN)
	#define GENERAL_SHOUT_LIMIT_LEVEL	1
#endif


//������������������������������������������������������������������������������
// ���̳ʸ� ��� ���� �ÿ� �ʿ��� �κ�
//������������������������������������������������������������������������������

typedef BYTE				NUMTYPE;			///< ����
typedef BYTE				DURATYPE;			///< ������
typedef BYTE				POSTYPE;			///< ����
const POSTYPE				INVALID_POSTYPE_VALUE = 255;

typedef INT64				DBSERIAL;			///< DB �ø���

typedef DWORD64				DBWLDSERIAL;

typedef WORD				SLOTCODE;			///< ���� �ڵ�
typedef DWORD				CODETYPE;			///< ����
typedef WORD				KEYTYPE;			///< ���� Ű�� Ÿ��	<= ZONEKEY�� �����ؾ� �Ѵ�.
typedef WORD				LEVELTYPE;			///< item, character level type
typedef BYTE				SLOTIDX;			///< slot index
typedef int					FIELDID;
typedef ULONGLONG			MONEY;				///< ��
typedef DWORD				CASH;				///< ĳ��
typedef WORD				MAPCODE;			///< ���ڵ�
typedef WORD				FIELDCODE;			///< �ʵ��ڵ�
typedef WORD				ZONEKEY;			///< ��Ű

typedef INT64				EXPTYPE;			///< ����ġ Ÿ��
typedef WORD                PORTALKEY;
const PORTALKEY             INVALID_PORTAL_KEY = 0;

//������������������������������������������������������������������������������
// ��Ŷ��!!! ����ȭ �� typedef�� : client�� ������ ��Ŷ������ ���ٶ�
//������������������������������������������������������������������������������
typedef int DAMAGETYPE; // extended by _NA001605_EP2_DAMAGETYPE_EXTENTION_

typedef short               TILEINDEX;          ///< ��Ŷ��!!! Ÿ���ε���
typedef WORD				MONSTERCODE;		///< �����ڵ�
typedef	UINT				PLAYERHP;			///< �÷��̾�HP
typedef WORD				MPTYPE;				///< MP Ÿ��
//__NA_001290_20090525_SHIELD_SYSTEM
typedef WORD				PLAYERSD;			///< �÷��̾�SD
//_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
typedef FLOAT               PLAYERFP;           ///< ��ġ���̵� FP
//������������������������������������������������������������������������������
#define DAMAGETYPE_MAX  (1000000) // expanded by _NA001605_EP2_DAMAGETYPE_EXTENTION_

//���� ���� 
#define MAX_JUMP_NODE 30000
#define MIN_JUMP_NODE 200
/// [2012-5-31 by i4u4me] Ŭ�� ���� �˰��� �������� �ִ� ÷���Ÿ� ����
#define MAX_JUMP_DISTANCE			(12)			// �ִ� ������ �̵��� �� �ִ� �Ÿ�
//#define MAX_JUMP_DISTANCE			(15)			// �ִ� ������ �̵��� �� �ִ� �Ÿ�

#define MAX_ALLPLAYERS_PACKET_SIZE	( 1500 )	// �ֺ� �÷��̾� ���� ���۽� �ִ� ��Ŷ������

#define MAX_ITEMNAME_LENGTH			( 64 )		// ������ �̸� ����
#define MAX_SKILLNAME_LENGTH		( 64 )		// ��ų �̸� ����
#define	MAX_MAPNAME_LENGTH			( 64 )		// ���̸� ����
enum { MAX_ABILITY_NUM = 5 }; // ��ų�ϳ��� �ִ� �����Ƽ(��ųȿ��) ����
#define MAX_SUMMONED_NUM			( 10 )		// �ѹ��� ��ȯ�Ǵ� �ִ븶����
#define MAX_AREA_ID_LENGTH			( 4 )		// Area ���� ID ����

// (NOTE) an unused section since the 0816 version
// removed by __NA_963_LOG_LIB_VERSION_REFACTORING

#define	MAX_STATEINFOSTREAM_SIZE			( 1024 )	// ĳ���� ��������
#define MAX_DBSAVE_STATEINFO_NUM			( 40 )		// DB�� ����Ǵ� �ִ� ���°���

#ifdef _NA_001990_ACSYSTEM_ADD
	//_NA_007422_20140716_MODIFY_MAX_ACHIEVEMENT_COUNT
    #define MAX_ACHIEVEMENT_COUNT				(1000)   //�ִ� ���� 
    #define MAX_ACHIEVEMENT_REWARD				(2)     //���� ���� ����
    #define MAX_ACHIEVEMENT_OBJECT				(10)    //���� ���� ����
    #define ACHIEVEMENT_UINT_SIZE				(23)    //���� �ϳ��� ����
    #define MAX_ACHIEVEMENT_PARENT_CODE         (4)     //����AC�ڵ� �ִ밹��
#else
    #define MAX_ACHIEVEMENT_COUNT				(300)   //�ִ� ���� 
    #define MAX_ACHIEVEMENT_REWARD				(2)     //���� ���� ����
    #define MAX_ACHIEVEMENT_OBJECT				(8)     //���� ���� ����
    #define ACHIEVEMENT_UINT_SIZE				(19)    //���� �ϳ��� ����
#endif

#define MAX_SKILL_SLOT_NUM				(100)

#define MAX_VENDOR_SLOT_NUM				(130) //extended by __NA000920_20080306_NPCSHOP_ITEM_SELL_HISTORY (from 125)
#ifdef _NA_0_20101124_VENDOR_SLOT_COUNT_INCREASE
    #define MAX_PERSONAL_VENDOR_SLOT_NUM	(40)
#else
    #define MAX_PERSONAL_VENDOR_SLOT_NUM	(24)
#endif
#define MAX_TOTAL_SLOT_NUM				(MAX_INVENTORY_SLOT_NUM+MAX_EQUIPMENT_SLOT_NUM+MAX_SKILL_SLOT_NUM+MAX_WAREHOUSE_SLOT_NUM+MAX_TEMPINVENTORY_SLOT_NUM)
#define MAX_QUICK_SLOT_NUM				(48)
#define MAX_STYLE_SLOT_NUM				(4)
#define MAX_EVENT_INVENTORY_SLOT_NUM	(5*5)			//< �̺�Ʈ �κ��丮 ���� ���� �������� 5*5 = 25

#define MAX_INVENTORY_TAB_NUM			(5)				//< �κ��丮 �⺻ �� ����
#define MAX_CHARGE_INVENTORY_TAB_NUM	(5)				//< �κ��丮 �⺻ �� ����
#define MAX_INVENTORY_TAB_TOTAL_NUM		(MAX_INVENTORY_TAB_NUM+MAX_CHARGE_INVENTORY_TAB_NUM)
#define MAX_INVENTORY_SLOT_PER_TAB		(15)			//< �κ��丮 �Ǵ� ���� ����
enum { MAX_PANDORA_SLOT_NUM	= 8 }; // �ǵ��� ���� ���� ����
#define MAX_CHANGE_ITEMSTAT_SLOT_NUM	(4)				//< ������ ���� ��ȯ ���� ����

#define MAX_WAREHOUSE_INVENTORY_PWD_LENGTH	(8)			// â�� �κ��丮 ��ȣ 

#define MAX_SSN_STREAM_LENGTH				(10)		// Social Society Number (10�� ������... ���� �� ����)
#ifdef _KR_0_20120207_CRYPT_SHA256
#define MAX_SSN_ENCODED_STREAM_LENGTH		(65)
#else
#define MAX_SSN_ENCODED_STREAM_LENGTH		(16)
#endif

//���Ӽ��������� ����ϴ� ����
#define MAX_TRADE_SLOT_NUM				(20)

//������������������������������������������������������������������������������
// ���� ����Ʈ ����
#define MAX_ONLOOKER_VENDOR_NUM			(100)			//������� NPC ����� POOL�� Init size(�þ �� ����)
#define COUNT_SEARCH_CONTAINER_INIT		(10)			//������� �˻��� ������ Init size(�þ �� ����)
#define COUNT_VENDOR_IN_PAGE			(10)			//������� ���������� �� ����Ʈ ��
//������������������������������������������������������������������������������

#define TEMP_DBSERIAL_VALUE				(0)
#define DBSERIAL_MASK_VALUE				(0xFFFFFFFFFFFFFFFF)
#define QUICK_SLOT_NULL_VALUE			(255)
#define INVALID_SLOT_POS				(255)
#define MAX_LEVEL						( 100 )		// �ִ� ����(���� �ִ� ������ �ٲ� �� �ִٰ� ��!!)

#define MIN_PENALTY_LEVEL			    ( 11 )      // �ּ� �г�Ƽ ���� ����
#ifdef _NA_003966_20111227_ADD_ENCHANT
enum { MAX_ENCHANT_GRADE = 15 };
#else
enum { MAX_ENCHANT_GRADE = 12 };
#endif
enum { MAX_AWAKENING_GRADE = 9, };                  // �ִ� ���� ���� // _NA_008124_20150313_AWAKENING_SYSTEM
//������������������������������������������������������������������������������
//������������������������������������������������������������������������������

// �̼� ����
#ifdef _NA_1388_20101207_EP2_INCREASED_MISSION_MAX
    #ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
        #define MAX_MISSION_NUM                 100
    #else
        #define MAX_MISSION_NUM                 50
    #endif
#else
	#define MAX_MISSION_NUM					32
#endif
#define MAX_HIDDEN_MISSION_NUM			16
#define MAX_MAPCODE_NUM					0xFFFF

#define BEGIN_MISSION_MAP				1		//�̼� ���ڵ�  ����
#define BEGIN_CHUNTING_MAP				40001	//C���� ���ڵ� ����
#define BEGIN_PVP_MAP					20001	//PVP ���ڵ�   ����	

//�̼� ������ ����.
#define MISSION_RELAY_NOTICE_TIME		21000

//������������������������������������������������������������������������������
// ������ ��� ���� ����
//#define	CREATE_GUILD_LIMIT_LEVEL		(20)
#define LIMIT_LEVEL_WARHOUSE			(6)
#define LIMIT_LEVEL_RANKUP				(7)
#define LIMIT_LEVEL_ENCHANT				(8)
#define LIMIT_LEVEL_ACCESSORY			(15)
#define LIMIT_LEVEL_SACCESSORY			(30)

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
// ����ȭ �������� ����
#else
#define LIMIT_LEVEL_CRYSTALIZE			(9)
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE
//������������������������������������������������������������������������������

/// ���� ����
#define MAX_EXP_POTION_VALUE		( 10000000 ) //����ġ �������� �ø� �� �ִ� �ִ� ����ġ
#define DISTANCE_SPACE_GET_ITEM		7.0f	// �����̽��� ���� �� �ִ� �������� 10 �����̳� �ִ� ������

#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    enum { MAX_SKILL_LEVEL = 10 }; // �ִ� ��ų ����
    enum { MAX_SKILL_OVER_LEVEL = 20 }; // �ִ� ��ų ���� ����
#else
    #ifdef _NA_0_20100901_SKILLTREE_SYSTEM
        enum { MAX_SKILL_LEVEL = 5 }; // �ִ� ��ų ����
        enum { MAX_SKILL_OVER_LEVEL = 10 }; // �ִ� ��ų ���� ����
    #else
        enum { MAX_SKILL_LEVEL = 10 }; // �ִ� ��ų ����
        enum { MAX_SKILL_OVER_LEVEL = 20 }; // �ִ� ��ų ���� ����
    #endif
#endif

// �̸� ���� ����
#define MAX_CLIENT_CHARNAME_LENGTH	( MAX_CHARNAME_LENGTH + 1 )	 // Ŭ���̾�Ʈ�� ������ ��Ŷ�̳� Ŭ���̾�Ʈ���� ĳ���� �̸��� ������� ��Ʈ�� ����.
#define MAX_CLASSNAME_LENGTH		( 16 )
#define MAX_TITLEID_LENGTH			( 16 )		// ĳ�����̸��̿��� Ư�� Īȣ
#define MAX_SHOPNAME_LENGTH			( 32 )		// ���� �̸� ����

#define MAX_SMALLDATETIME_SIZE		( 16 )		// "2006-10-12 24:03" ( �д��� )
#define MAX_MIDDATETIME_SIZE		( 19 )		// "2006-10-12 24:03:00" ( �ʴ��� )
#define MAX_DATETIME_SIZE			( 23 )		// "2006-10-12 24:03:00.123" ( �и��ʴ��� )

#ifdef _INTERNATIONAL_UI
 #if defined (_GLOBAL) || defined (_RUSSIA)
	#define MAX_ROOMTITLE_LENGTH		( 31 )
 #else 
	#define MAX_ROOMTITLE_LENGTH		( 29 )
 #endif//�Ϲ̴� 30�� �׿ܴ� 28�ڷ�����
#else
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    #define MAX_ROOMTITLE_LENGTH		( 31 )
#endif//_INTERNATIONAL_UI(������ 30�ڷ������մϴ�.)

#define MAX_ROOMPASSWORD_LENGTH		( 7 )		// �� ��й�ȣ �ִ� ����
#define	MAX_VENDOR_TITLE_LENGTH		(80)		//< ���λ��� Ÿ��Ʋ ����
#define	MAX_VENDOR_SEARCH_LENGTH	(20)		//< �˻��� ����
#define VENDOR_ITEMS_PER_PAGE       (12)        // ���λ��� �˻���� �������� ������ ����

#define MAX_CHAT_LEN				(100)
#define MAX_NOTICE_LEN				(1024)
#define MAX_SHOUT_LEN				(255)

// ���� ����
#define MAX_MONSTER_NUM				( 10000 )	// ���� ��ü�� ���� ��
#define MAX_PLAYER_NUM				( 1000 )	// ���� ��ü�� �ִ� ���� ĳ���� ��
#define MAX_FIELDITEM_NUM			( 2000 )	// ���� ��ü�� ������ ��
#define MAX_TARGET_NUM				( 32 )		// �������ݽ� �ִ���� ������ ĳ���� ��
enum { MAX_FIGHTING_ENERGY_NUM = 5 }; // �ִ� ������ų �� �ִ� ���� ����
#define MAX_EFFECT_COUNT			( 10 )		// ��ų������ �ִ� ����Ʈ ����

#define MAX_PARTYMEMBER_NUM			( 10 )		// ��Ƽ���� �ִ� ����
#define MAX_FIELD_PARTYMEMBER_NUM	( 10 )		// �ʵ忡�� ��Ƽ���� �ִ� ����

#ifdef _NA_002050_20110216_ADD_GENDER
    #define MAX_CHARACTER_LIST_NUM (10) // �� ���������� ĳ���� ������ �� �ִ� �ִ� ����
    #define MAX_CHARACTER_SLOT_NUM (10) // �� ���������� ĳ���� ������ �� �ִ� �ִ� ����
    // ĳ���� ���� ���� ���� ���� (���� �� ����� �ʿ�)
    struct MaxCharListConfig
    {
        enum { kDefaultSlotSize = 6 }; // �⺻ ĳ���� ���� ����
        enum { kMaxSlotSize = MAX_CHARACTER_LIST_NUM }; // �ִ� ĳ���� ���� ����
        enum { kUseExtension = 1 }; // ĳ���� ���� Ȯ�� ��� ���� (0, 1)
        enum { kSlotSizePerExtension = 1 }; // Ȯ�� �õ��� �þ�� ĳ���� ���� ����
    };
#else
    #define MAX_CHARACTER_LIST_NUM (6) // �� ���������� ĳ���� ������ �� �ִ� �ִ� ����
    #define MAX_CHARACTER_SLOT_NUM (6) // �� ���������� ĳ���� ������ �� �ִ� �ִ� ����
#endif // _NA_002050_20110216_ADD_GENDER

#define VILLAGE_SECTOR_SIZE			40			// �������� ���� ũ��(����Ʈ ó���� ���� �Ѱ��� ���� ũ��(���簢��))
#define FIELD_SECTOR_SIZE			40			// �ʵ��� ���� ũ��
#define ROOM_SECTOR_SIZE			40			// ��Ʋ���� ���� ũ��
#define DOMINATION_SECTOR_SIZE		40			// ������������ ���� ũ��

#define PLAYER_MOVE_UPDATE_TICK		(500)		// �÷��̾��� �̵� ������Ʈ Tick
#define SERVER_UPDATE_TICK			(20)		// ������ ��ü Update Tick


#define DISTANCE_NPC_MEET			6.0f		// 6 �����̳� npc�� ���� �Ҽ� �ִ� ��ġ�� ����.
#define JUMP_DAMAGE_HEIGHT			8.0f		// 8 ���� ���� ���� ������ ����
#define JUMP_METER_PER_DAMAGE		60.0f		// ���ʹ� ������

#define MAX_IPADDRESS_SIZE					15

#define DEFAULT_ADJUST_LIMIT			(3.0f)							// GetTileToStand�� ���ڰ�

// �ִϸ��̼� �����Ӻ� �ð�
// �������� DEF_NUM_FRAMES_PER_MS �� ���� 0.03f 
#define DEF_NUM_FRAMES_PER_MS (30)
#define SPEED_MULTIPLIER				(0.001f)	// ĳ���� ���ǵ� ���
#define MAX_MAP_NUM						(100)		// World.txt�� Row�� ����

//���λ��� ������
#define TRADE_COMMISSION_RATE			(0.05f)		//< 5%

#if NATION_BUILD_VERSION >= 1002
#define CHAR_DESTROY_LEVEL				(70)		//< ĳ���� ���� �����ϴ� ���� (70)
#else
#define CHAR_DESTROY_LEVEL				(40)		//< ĳ���� ���� �����ϴ� ���� (40)
#endif

// GM ����� MAX Define ��
#define GM_MAX_CREATE_ITEM_NUM			( 1000)
#define GM_MAX_CREATE_MONEY				( 1000000000)

#define GM_MAX_STAT_UP_VALUE			( 100)
#define GM_MAX_SKILLPOINT_UP_VALUE		( 100)
#define	MAX_RANK_OPTION					( 49)
#define	MAX_ITEM_GRADE					( 1)		//����� 0�� �Ϲ�, 1�� ����ξ������̴�. ���� 2�� ����ũ �������� �ɰ��̴�.

//������������������������������������������������������������������������������
// PVP���� �ð� ������
#define PVP_PRESS_KEY_TIMEOUT_TIME		(30*1000)
#define PVP_PRESS_KEY_LEAVE_TIME		(PVP_PRESS_KEY_TIMEOUT_TIME - (10*1000))	// 20 �� ���� ��������� ������ ���ư��� ��Ŷ����
#define PVP_MATCHLESSS_DELAY_TIME		(10*1000)
#define PVP_REVIVAL_DELAY_TIME			(20*1000)
//������������������������������������������������������������������������������

//������������������������������������������������������������������������������
// �������� ���� �ð� ������
#define CHUNTING_TIMEATTACK_EXPIRE_TIME				(20*60*1000)	// Ÿ�Ӿ��� �����ð�
#define CHUNTING_SURVIVAL_EXPIRE_TIME				(20*60*1000)	// �����̹� �����ð�
#define CHUNTING_INIT_POTION_NUM					(40)			// �ʱ����� ����
#define CHUNTING_MAX_MEMBER_NUM						(10)			// �ִ� �����
#define CHUNTING_RESURRECTION_DELAY_TIME			(5*1000)        // ��Ȱ ������
#define CHUNTING_INTERVAL_START_READY				(20*1000)       // Ÿ ��Ƽ �Լ� �� ���� ���۱��� ������
#define CHUNTING_TIMEATTACK_EXPIRE_TIME_GUILD_PVP	(10*60*1000)
#define CHUNTING_SURVIVAL_EXPIRE_TIME_GUILD_PVP		(10*60*1000)

#define SKILL_COOLTIME_MARGIN (500) // Server: Ŭ�󺸴ٴ� ��Ÿ���� �۰� ��Ƽ� ��Ÿ�ӿ� ���� ��ų �������� �����ش�.
//������������������������������������������������������������������������������
// ��� ����
#define	MAX_GUILDMARKSTREAM_SIZE		(3072)
#define	MAX_GUILDSTYLESTREAM_SIZE		(8)

#define	MAX_GUILDNOTICE_LENGTH			(768)
#define MAX_GUILD_INTRODUCTION_LENGTH   (768)

#define	CREATE_GUILD_LIMIT_LEVEL		(20)			//<< ���â�� ��������
#define GUILD_UP2GP_RATE				(1000000)		//<< ����� UP:GP ��ȯ ����

#ifdef _NA_007239_20140521_BUG_GUILDRELATION_COUNT
#define MAX_ALLIENCE_GUILD_NUM			(20)			// ���� ��� �ִ��
#define MAX_HOSTILITY_GUILD_NUM			(20)				// ���� ��� �ִ�� 
#define MAX_ONE_SIDE_HOSTILITY_NUM		(100)			// �Ϲ��� ���뼱�� ��� �ִ��
#else // ��帮���� ����-����
#define MAX_GUILD_RELATION_NUM			(10)			// �� ���� ���� ��� �ִ��
#define MAX_ALLIENCE_GUILD_NUM			(10)			// ���� ��� �ִ��
#define MAX_HOSTILITY_GUILD_NUM			(5)				// ���� ��� �ִ�� 
#define MAX_ONE_SIDE_HOSTILITY_NUM		(100)			// �Ϲ��� ���뼱�� ��� �ִ��
#endif //_NA_007239_20140521_BUG_GUILDRELATION_COUNT

#define MAX_GUILD_MEMBER_NUM            (200)			//<< ���� ���� �� �ִ� �ִ� ���� ��
#define	MAX_GUILD_CORPS_MEMEBER_NUM     (60)			//<< ��� ������ ���� �� �ִ� �ִ� ���� ��

#define MAX_GUILD_CORPS_NUM			(8)				// ���� �ִ� ���ܼ�(���ұ��� ����)

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#define MAX_GUILD_JOIN_REQUEST_NUM      (30)            // ��� ���� ��û ��� �ִ��
#define MAX_GUILD_JOIN_REQUEST_TIME_MAX (24*60*60)      // ��� ���� ��û ���� �ð�(���� ��)
#define MAX_GUILD_JOIN_REQUEST_PLAYER_NUM (5)           // �÷��̾ ��û�� �� �ִ� �ִ� ���� ��û ��

#define MAX_GUILD_FACILITY              (8)             // ��� �ü� �ִ��
#define MAX_GUILD_FACILITY_OPTION       (2)             // ��� �ü� �ɼ� �ִ��

typedef BYTE FCODE;                                     //��� �ü� ����
typedef int  FLEVEL;                                    //��� �ü� ����
typedef BYTE ATYPE;                                     //��� Ȱ��ȭ ����
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//������������������������������������������������������������������������������
// Ÿ���� ���
#define VERSION_AREA_TILE				( 1)            //WAT���� ����  

typedef INT					UPTYPE;
typedef INT					GPTYPE;
typedef INT					GUILDGUID;
typedef INT					GUILDMARKIDX;		///< ���ڵ�
typedef	INT					CORPSGUID;			
typedef	INT					CAMPGUID;
//������������������������������������������������������������������������������
typedef INT					CHARGUID;
typedef INT					USERGUID;
typedef BYTE				MISSIONNO;			//< MAX_MISSION_NUM�� ���� �ʴ´�.
typedef int					RANKINGKEY;			//< ��ŷ �Ϸ� ��ȣ 
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
typedef BYTE                RANKINGID;          //< ���ڵ��� �ߺ����� ���� ���� Ű�� �߰�.
typedef BYTE                RANKINGTYPE;         //< ���ڵ尡 ���� ��ŷ Ÿ�� �ٸ� �� �ֱ� ������ �߰�.
typedef WORD               RANKING;            // ��ŷ ���� ǥ�� Ÿ��
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
typedef WORD				RANKINGNO;			//< ��ŷ ������ ��ȣ == MAPCODE
typedef	INT					CORPSGUID;			//< ��� ���� GUID
typedef INT                 GAMECHUUSN;
typedef UINT64              BETAKEY;
#ifdef _NA_0_20100819_AGGRO_RENEWAL
typedef DWORD               BATTLEPOINT;
#else
typedef WORD                BATTLEPOINT;
#endif
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
typedef INT64               SCORE;              //< ��ŷ ����
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

/// �׻� Ÿ���� �̰��� ����ϱ�� ��!!!!! �ڷ���(DATATYPE) ����
typedef WORD				MAPID;				///< server identification
typedef int					SOUND_INDEX;		///< sound index : it's same sound handle
typedef DWORD				PLAYERKEY;

//������������������������������������������������������������������������������
// WAYPOINT ����
typedef WORD				WPINDEX;			//< ���� DB�� int�� �Ǿ�����
#ifdef _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT
  #define MAX_CHAR_WAYPOINT_NUM		(15)			// ���� ���� ����Ʈ �ִ� ����
#else
  #define MAX_CHAR_WAYPOINT_NUM		(10)			// ���� ���� ����Ʈ �ִ� ����
#endif //_NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT
//������������������������������������������������������������������������������
// ä�� ����
#define MAX_COLLECTION_LIST_NUM	(255)			//< ��ũ��Ʈ�� �ִ� ���� (ä�� ������ ä���� ����)
#define MAX_FIELD_COLLECTION_NUM (255)			//< �ʵ忡 ���� �ϴ� �ִ� ����

#define MAX_COLLECT_DISTANCE_OBJ	(10)		//< ä������ ä�� ������ �ִ� �Ÿ�		(ä������ �Ÿ���)
#define MAX_COLLECT_DISTANCE_CHAR	(5)			//< �ɸ��Ͱ� ���� ������ �ִ� �ִ� �Ÿ�	(���۽ÿ� ����� �Ÿ���)

//{__NA001283_20090303_COLLECTION_MAPOBJECTID
// �ʿ�����Ʈ Ű�� ������ 20000�� �븦 ��� ���������� ���Ǹ�,
// Ʈ���ſ����� 1������ ���۵Ǵ� ��ȣ�� ���, 
// Ʈ�����ʰ� ������ ���������� Ʈ���� ��ȣ�� ���� �ʿ��ϴ�.

// MapObjectKey�� (200000~)���� ����ϸ�, TriggerObjectKey�� (1~)���� ����Ѵ�.
//����Ű(1~)�� ����Ű(200000~)�� ��ȯ
#define Conv_MapObjectKey(Key)	(MAP_OBJECT_KEY>Key?Key+MAP_OBJECT_KEY:Key)

//����Ű(200000~)�� ����Ű(1~)�� ��ȯ
#define Conv_TriggerObjectKey(Key)	(MAP_OBJECT_KEY<=Key?Key-MAP_OBJECT_KEY:Key)
//}

//������������������������������������������������������������������������������
//�׷� �α� �ý��� '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
#define MAX_GROUP_MEMBER_LOW_MAX (5)        //������ ������ �ִ� �ִ� ���� ��� ����
#define MAX_GROUP_MEMBER_HIGH_MAX (1)       //������ ������ �ִ� �ִ� ���� ��� ����
#define MAX_GROUP_MEMBER (MAX_GROUP_MEMBER_LOW_MAX + MAX_GROUP_MEMBER_HIGH_MAX)//�׷��� �ο���
#define MAX_DATE_GROUPBONUS (7)             //�׷����ġ ���ʽ��� �ޱ� ���� �����ؾ��� ����
#define HIGH_MEMBER_LEVEL (80)              //���� ��� ����, �߱� ���������� ���
#define LOW_MEMBER_LEVEL (10)               //���� ��� ����, �߱� ���������� ���
#if defined (_CHINA)
    #define MAX_GROUP_EXP (10000000)	            //�������� �ִ� ���� ����ġ
#else
    #define MAX_GROUP_EXP (3000000)	            //�������� �ִ� ���� ����ġ
#endif

//������������������������������������������������������������������������������

typedef DWORD REGENID;
typedef DWORD UNITGROUPID;

// ���� �����ؼ� �������ݿ��� ����ϴ°�
const int GAMECHU_USN_MAX       = 10;
const int INFO_MAX_LEN			= 64;

#if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__)
    const int ID_MAX_LEN        = 50;
#else
    const int ID_MAX_LEN        = 24;
#endif

#ifndef CHECK_ID_LENGTH_BUFFER
    #if defined(__CH1305_090303_ACCOUNT_ID_LENGTH_EXTENTION__) || defined(_ACCOUNT_ID_LENGTH_EXTENTION_)
        #define CHECK_ID_LENGTH_BUFFER(member)  \
            BOOST_STATIC_ASSERT(static_cast<size_t>(sizeof(member)/sizeof(member[0])) == (ID_MAX_LEN + 1) && \
                                ID_MAX_LEN == 50)
    #else
        #define CHECK_ID_LENGTH_BUFFER(member)  \
            BOOST_STATIC_ASSERT(static_cast<size_t>(sizeof(member)/sizeof(member[0])) >= ID_MAX_LEN && \
                                ID_MAX_LEN == 24)
    #endif
#endif


const int PASSWD_MAX_LEN		= 24;
#define MAX_IP_STRING_LEN		15  // NOTE: f110923.1L, moved from serverstruct.h
const int IP_MAX_LEN			= 32;	// NOTE: f110923.1L, replace with MAX_IP_STRING_LEN. �����ڵ带 ����ؼ� Size�� �ø�(16->32)
const int MAX_IP_LENGTH         = 16;   // NOTE: f110923.1L, does not use anymore, �� ������ ���߱� ���� �߰�
const int AUTH_SERIAL_KEY_LEN	= 32;	// ���� �ø��� Ű(����)

const int MAX_WORLD_NAME_LEN	= 32;	// �����̸� ����
const int MAX_CHANNEL_NAME_LEN  = 32;   // ä���̸� ����

const DWORD HEARTBEAT_TICK      = 20000;
#if USING_GAMEGUARD
const DWORD GAME_GUARD_CHECK_TICK = 5000; // 5�ʿ� �� ���� ���Ӱ��� üũ
#endif // USING_GAMEGUARD

#ifdef _KR_0_20120207_CRYPT_SHA256
    enum { kCryptStringSize = 65 };
#else
    enum { kCryptStringSize = 16 };
#endif

//�α������� ��� �ִ� ����
#define		MAX_LOGPATH_LEN			256
//���� �ݾ� �̻� �ŷ��� �α� ����(1��)
#define		LOG_TRADE_OVER_MONEY	100000000
//�̼ǹ� ������ ���� ���̰�.
#define		MISSION_LEVEL_JOIN_OFFSET	25

#define		HUNTING_JOIN_LEVEL_OFFSET	15

const BYTE MAX_VALKYRIE_RELOAD_COUNT = 20;	// ��Ű�� ���� �ִ� ��������

#ifdef _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST
    #define MAX_FRIEND_STAT_NUM	200 // �� ��Ŷ�� �� �� �ִ� �ִ� ģ�� ����
    #define MAX_BLOCK_STAT_NUM 200 // �� ��Ŷ�� �� �� �ִ� �ִ� ���� ����
    #define MAX_FRIENDBLOCK_STAT_NUM 400 // �� ��Ŷ�� �� �� �ִ� ģ��/���� ���� �ִ� ����
#else
    #define MAX_FRIEND_STAT_NUM	90 // �� ��Ŷ�� �� �� �ִ� �ִ� ģ�� ����
    #define MAX_BLOCK_STAT_NUM 90 // �� ��Ŷ�� �� �� �ִ� �ִ� ���� ����
    #define MAX_FRIENDBLOCK_STAT_NUM 190 // �� ��Ŷ�� �� �� �ִ� ģ��/���� ���� �ִ� ����
#endif //_NA_000094_20100622_ENHANCED_USER_BLOCK_LIST

#define MAX_REVENGE_STAT_NUM 10 // �� ��Ŷ�� �� �� �ִ� �ִ� ������ ��� ����
#define		MAX_FRIENDINVITATE_REWARD_NUM	MAX_FRIEND_STAT_NUM * 4 // �� ��Ŷ�� �� �� �ִ� ģ�� ���� ��� ����
//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
#define     MAX_FRIEND_MEMO_LENGHT      114     // �� ����â�� �޸� ������

#define __AUTH_TEST__

/*������ ������*/#define REMOVE_ATTACK_MOVE              // 2006 .2 .18 / ������ / �����̵� ����

// �̺�Ʈ ���� ��
#define		_MAX_EVENT_REGITEM			5		// �� �̺�Ʈ�� ����� �� �ִ� �ִ� ������ ����

// NOTE: increased by _KR_0_20091021_FIRENDINVITATION_EXTEND //ģ���ʴ� �̺�Ʈ�� ���� �� 4���� Ÿ���̴�.
#define		_MAX_EVENT_REWARD			4		// �� �̺�Ʈ���� �ִ� ���� ���� �� 

#define		_MAX_EVENT_ROW_NUM			100		// ���� �������� �̺�Ʈ �ִ� ����

#define		MAX_AKPATTERN_SIZE				1024	// �Ƹ�Ŷ ���� ������ �ִ� ũ��

#define MAX_MEMO_NUM			200		// �ִ� ���� ����
#define MAX_MEMO_PAGENUM		20		// �� ������(��Ŷ ����)�� �������� �ִ� ���� ����(�� 200���� 20*10 = 200�̹Ƿ� 10�� ������)
#define MAX_MEMO_LENGTH			200		// ���� ����(BYTE)
#define MAX_MEMO_STRLENGTH		201		// NULL����
#define MAX_SUBJECT_WLENGTH		10		// ���� ����(wchar_t)
#define MAX_SUBJECT_LENGTH		20		// ���� ����(BYTE)
#define MAX_SUBJECT_STRLENGTH	21		// NULL����
typedef ULONGLONG					MEMOGUID;

// ä��ä�ù� �ִ� ���� �ο�(define ������)
#if defined (_RUSSIA)
#define	MAX_CHATZONE_USER			500
#else
#define	MAX_CHATZONE_USER			255
#endif
// Null ���ڸ� DWORD ������ ������ �� 
#define Null_Value 1819047246

// ������������ Ŭ���̾�Ʈ �������� ��û�� �� ���� �÷��� �߰�-â������ ������ �ٽ� �߰�. ��ġ ���� �� define Ǯ���� ��
#define _AUTH_USERDISCONN_CODE

#define	MAX_VERSION_SIZE			64		// ��ũ��Ʈ�� ���� ������ ������

//ũ��������Ʈ ���� ����.
#define	MAX_MODULE_NAME_SIZE		64
#define MAX_BUILD_VERSION_SIZE		8
#define	MAX_OS_NAME_SIZE			64
#define	MAX_PROCESS_NAME_SIZE		100
#define	MAX_PHYSICAL_MEMORY_SIZE	32
#define	MAX_DIRECTX_NAME_SIZE		64
#define	MAX_DESCRIPT_SIZE			64
#define	MAX_DISPLAY_MEMORY_SIZE		32
#define	MAX_DRIVER_VERSION_SIZE		32

#define __NA_515_20070608_JUMP_DAMAGE

#define MAX_SOCKET_OPTION_INDEX			    1024    
#define MAX_ITEM_OPTION_KIND_NUM			128     // ���� _NA_000251_20100727_SOCKET_SYSTEM

#define SKILLCODE_INVALID					0
#define SKILLCODE_NORMAL_ATTACK				65500
#define SKILLCODE_FIGHINGENERGY				2101		// ���Ǹ������� ��ų�ڵ�

#define MAX_GUILD_DELEGATE_RIGHT			6			//__NA_000596_20070821_GUILD_CLASS_SYSTEM


#define INFINITE_ITEM_DAY					10000

#define HEIGHT_OF_STORY						20			// �� ���� ����

 ///////////////////////////////////////////��� ///////////////////////////////////////////
#define INVALID_GUILDGUID	(0)
#define MAX_REPURCHASE_ITEM_NUM	(5)                             // �籸�� ������ ����� �ִ� ����
#define GUILDWAREHOUSE_USING_LEVEL	(1)                         // ��� â�� ����� �� ���� ����

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    #define GUILDWAREHOUSE_SLOTEX_GUILD_GRADE       (3)         // Ȯ�� ������ ����� �� ���� ��� ���
    #define GUILDWAREHOUSE_SLOTEXPOS	            (25)        // ��� ���� 3�̻���� ��밡���� ���� ��ġ ��ȣ
    #define GUILDWAREHOUSE_SLOTEXPOS_BY_CASHITEM	(50)	    // ��� ���� ĳ�� �������� ��������ν� ��� ������ ���� ��ġ ��ȣ
    #define MAX_GUILDWAREHOUSE_SLOT_PER_TAB         (25)        //  ��� â�� �Ǵ� ���� ����
#else
    #define GUILDWAREHOUSE_SLOTEX_GUILD_GRADE       (2)         // Ȯ�� ������ ����� �� ���� ��� ���
    #define GUILDWAREHOUSE_SLOTEXPOS	            (24)        // ��� ���� 2�̻���� ��밡���� ���� ��ġ ��ȣ
    #define GUILDWAREHOUSE_SLOTEXPOS_BY_CASHITEM	(48)	    // ��� ���� ĳ�� �������� ��������ν� ��� ������ ���� ��ġ ��ȣ
    #define MAX_GUILDWAREHOUSE_SLOT_PER_TAB         (24)        //  ��� â�� �Ǵ� ���� ����
#endif //_NA_003923_20120130_GUILD_RENEWAL

#define MAX_GUILDWAREHOUSE_TAB_NUM (1)                       // ��� â�� �⺻ �� ����
#define MAX_GRADE_GUILDWAREHOUSE_TAB_NUM	 (1)        // ��� â�� ��� ����� 3�� �����ν� ��� ������ �ִ� �� ����
#define MAX_CHARGE_GUILDWAREHOUSE_TAB_NUM	 (1)        // ��� â�� ĳ�������� ��� ������ �ִ� �� ����
#define MAX_GUILDWAREHOUSE_TAB_TOTAL_NUM (3)           // ��� â���� ��밡���� �ִ� �� ����

#define MAX_GUILDWAREHOUSE_LOGLIST_NUM	(200)
#define MAX_GUILDWAREHOUSE_LOGPAGE_NUM	(20)

#define GUILD_DESTROY_WAIT_TIME	(3*24*60)

#define GUILDGRADE_AT_GUILDWAREHOUSE_CREATE (2)         //��� â�� ������ �� �ִ� Guild Grade
#define GUILDWAREHOUSE_TAB_COUNT_AT_GUILD_CREATE (1)    //��� â�� �����ÿ� �־����� â�� �� ����
#define GRADE_AT_GUILD_CREATE (1)                       //��� ������ �⺻ Guild Grade

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
#define	MAX_GUILDNAME_LENGTH		( 16 )		// ���� �ִ� ����
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
////////////////////////////////////////////����������////////////////////////////////////////
#define MAX_TILE_NODE_CNT_IN_DOMINATION 50   
#define MAX_ATTACK_HEIGHT_IN_DOMINATION 4.5f
///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////BIT CONTROL///////////////////////////
#define BIT_RESET(val) (val = 0)
#define BIT_ON(val,bit) (val |= bit)
#define BIT_OFF(val,bit) (val &= (~bit))
#define IS_BIT_ON(val,bit) ((val & bit) == bit)
////////////////////////////////////////////////////////////////////////

#define PACKET_PTR_CONVERT(type, obj, Value)	\
	type* obj = static_cast<type*>(Value);

#define		_MAKESTR(s)		#s
#define		MAKESTR(s)		_MAKESTR(s)

enum { MAX_PET_NAME_LENGTH = 10 }; //__NA_1131_PET_NAME_SETTING

// used by __NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
// ���� ���� �ζ� ���� �׸���� Ŭ��/����/DB ���� ���̴� ���� �Ұ�
#define LOTTO_MIN_NUMBER            1   // ���� ������ �ּ� ����
#define LOTTO_MAX_NUMBER            25  // ���� ������ �ִ� ����
#define LOTTO_NUM_PICKS             4   // ����/��÷�� �� ���� ���ڸ� ������ ���ΰ�
#define LOTTO_NUM_RECENT_DRAWS      2   // �ֱ� �� ȸ���� ���� �ζ� �����͸� �ٷ� ���ΰ� ( >= 2)
#define LOTTO_MAX_NOTICE            10  // �ֱ� �� ���� ��÷ ���� �޽����� ������ ���ΰ�

#if defined (_GLOBAL) || defined (_RUSSIA)
#define LOTTO_MAX_TICKETS           30   // �� ĳ���Ͱ� ������ �� �ִ� ���� ������ �ִ� ����
#else
#define LOTTO_MAX_TICKETS           5   // �� ĳ���Ͱ� ������ �� �ִ� ���� ������ �ִ� ����
#endif

// ������ ������ ó���� ���� ���
#define LOTTO_NUM_WINNERS_PER_PAGE  200 // ��÷�� ��� ���ý� �������� ���ڵ� ��
// }

#define CHANNEL_DOMINATION_WAR 0

//������������������������������������������������������������������������������
//���� ������ �ý���
#define MAX_DOMINATION_ACTIONLIST (10)      // ���â�� ǥ�õ� ����� ����Ʈ ����   
#define MAX_DOMINATION_MAP (100)            // ���� �������� ������ ���� ��ü ����
#define MAX_DOMINATION_AUCTION (100)         // ������ ��ſ� ������ �� �ִ� ����� ����
#define MAX_DOMINATION_DEFENSEWALL_NUM (3)  // ���������� �溮 ����
#define DOMINATION_DEFFENSWALL_POINT (5)    // ���� 5����Ʈ�� MAX������ ���� ������ �����ϴ�.
// WARNING: to gamelulu, only you need test value (3)!, you must be restore to valid value (20)
#define MAX_DOMINATION_MEMBER (20)          // �������� ������ �� �ִ� ��� �ִ밪
//������������������������������������������������������������������������������

#define CONDITION_DELEY             (1000)  // ����� Ʈ���� ������Ʈ Ÿ��

#if defined (_RUSSIA)
    #define GIFT_REQ_USERLEVEL          (20)    //���þ� �����ϱ� ���� Level
#else
    #define GIFT_REQ_USERLEVEL          (25)    //�Ϲ� �����ϱ� ���� Level
#endif


#ifdef _CHINA  //_NA_001290_20090525_MODIFY_CALC_SD
#define SDINCREASE_RATIO            (3.0f)  // SP ���� ���� ����ġ
#else
//youngmoon #2996 SD ���� ����
//#define SDINCREASE_RATIO            (8.0f)  
#define SDINCREASE_RATIO            (6.0f)  
#endif

#define STATE_BATTLE_TIME           (5000)  // ��Ʋ ���� ���� �ð�
//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#define MAX_SYSTEM_MEMO_LIST        (200)   // �ѹ��� ���� �� �ִ� �ý��� �޸� ����
#else
#define MAX_SYSTEM_MEMO_LIST        (100)   // �ѹ��� ���� �� �ִ� �ý��� �޸� ����
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

//_NA_0_20100901_SKILLTREE_SYSTEM
#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    enum { CHARACTER_SKILL_LEVEL_RATIO = 1 }; // ���� 1�� 1����Ʈ
    enum { REQUIRE_USED_SKILL_POINT_FOR_NEXT_LINE = 20 };
#else
    enum { CHARACTER_SKILL_LEVEL_RATIO = 3 }; // ĳ���ͷ��� 3�� ��ų���� 1
    enum { REQUIRE_USED_SKILL_POINT_FOR_NEXT_LINE = 5 }; // ���� ������ ��ų�� ���������� ���� ��ų ����Ʈ ����
#endif

//_NA_20110630_BATTLE_ZONE_RENEWAL
#define MAX_MISSION_COUNT           (100)   //������ �� �ִ� �̼� �ִ� ��
#define MAX_PARTYROOM_LIST          (40)    //��Ʋ�� ����Ʈ�� ǥ�õǴ� ��Ƽ�� ����Ʈ �ִ밪
#define MAX_PARTYROOM_SEARCH_LIST   (100)   //�˻���� ����Ʈ �ִ� ��

#define MAX_REQUIRE_ITEM            (10)     //�䱸 ������ ������ (��ũ��Ʈ�� �䱸 ������ ��ġ�ؾ���)
//������������������������������������������������������������������������������
// ĳ�� �� ����
//������������������������������������������������������������������������������
#define GIFT_MESSAGE_LENGTH 201
#define BUY_STORAGE_ITEM_PER_PAGE 30
#define GIFT_STORAGE_ITEM_PER_PAGE 30
#define SHOP_MAX_ITEM_BUY_COUNT 30
#define SHOP_MAX_ITEM_GIFT_COUNT 30
#define SHOP_MAX_CASH_DETAIL_COUNT 10
#define SHOP_MAX_ITEM_PROPERTY_COUNT 10

// �� ���� �ڵ�.
#ifdef _KOREA
    #define SHOP_GAMECODE 113 
    #define SHOP_SALESZONE 329
    #define SHOP_SERVERTYPE 0   // �ΰ��� ����Ʈ ������ ���� �ۼ�Ʈ ��ȸ(InquireInGamePointValue() ���� ���). �㿡���� ������� ����.
#elif defined(_CHINA)
    #define SHOP_GAMECODE 862
    #define SHOP_SALESZONE 860
    #define SHOP_SERVERTYPE 0
#elif defined(_JAPAN)
    #define SHOP_GAMECODE 844
    #define SHOP_SALESZONE 847
    #define SHOP_SERVERTYPE 0
#elif defined(_GLOBAL)
    #define SHOP_GAMECODE 503
    #define SHOP_SALESZONE 505
    #define SHOP_SERVERTYPE 687
#elif defined(_RUSSIA)
    #define SHOP_GAMECODE 856
    #define SHOP_SALESZONE 853
    #define SHOP_SERVERTYPE 852
#endif


//_NA_003027_20111013_HONOR_SYSTEM
#define MAX_HONORTITLE_COUNT        (300)
#define MAX_HONOR_POINT_INFO_COUNT  (200)
#define MAX_DAILYQUEST_INFO_COUNT   (300)
#define MAX_DAILYQUEST_PROGRESS_COUNT (24)
#define MAX_GUILD_DAILYQUEST_PROGRESS_COUNT (5)

#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
#define MAX_CHALLENGE_DAILYQUEST_PROGRESS_COUNT (1000)
#endif //_NA_007407_20140710_CHALLENGE_DAILY_QUEST

//_NA004034_20120102_POINT_WALLET_SYSTEM
#define MAX_POINTWALLET_COUNT       (100)

//_NA_003923_20120130_GUILD_RENEWAL
#define MAX_IN_GAME_GUILD_LOG       (100)
#define MAX_IN_GAME_LOG_PARAM_STR   (20)

// _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#define MAX_NOTIFICATION_COUNT      (50)

// _NA_008404_20150712_MINIGAME_SYSTEM
#define MINIGAME_BINGO_WIDTH        (5)
#define MINIGAME_BINGO_HEIGHT       (5)
#define MINIGAME_BINGO_LENGTH       (MINIGAME_BINGO_WIDTH * MINIGAME_BINGO_HEIGHT)
#define MINIGAME_BINGO_MAX_REWARD_COUNT (12)

#endif // __DEFINE_H__

