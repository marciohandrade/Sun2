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


//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 夥檜傘葬 賅菊 寡ん 衛縑 в蹂и 睡碟
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

typedef BYTE				NUMTYPE;			///< 偃熱
typedef BYTE				DURATYPE;			///< 頂掘溘
typedef BYTE				POSTYPE;			///< 路塭
const POSTYPE				INVALID_POSTYPE_VALUE = 255;

typedef INT64				DBSERIAL;			///< DB 衛葬橡

typedef DWORD64				DBWLDSERIAL;

typedef WORD				SLOTCODE;			///< 蝸煜 囀萄
typedef DWORD				CODETYPE;			///< 路塭
typedef WORD				KEYTYPE;			///< 陝謙 酈菟 顫殮	<= ZONEKEY嫌 翕橾п撿 и棻.
typedef WORD				LEVELTYPE;			///< item, character level type
typedef BYTE				SLOTIDX;			///< slot index
typedef int					FIELDID;
typedef ULONGLONG			MONEY;				///< 絲
typedef DWORD				CASH;				///< 議蓮
typedef WORD				MAPCODE;			///< 裘囀萄
typedef WORD				FIELDCODE;			///< в萄囀萄
typedef WORD				ZONEKEY;			///< 襄酈

typedef INT64				EXPTYPE;			///< 唳я纂 顫殮
typedef WORD                PORTALKEY;
const PORTALKEY             INVALID_PORTAL_KEY = 0;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// ぬ韃辨!!! 譆瞳�� 辨 typedef歜 : client煎 爾頂朝 ぬ韃縑憮虜 餌辨夥塋
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
typedef int DAMAGETYPE; // extended by _NA001605_EP2_DAMAGETYPE_EXTENTION_

typedef short               TILEINDEX;          ///< ぬ韃辨!!! 顫橾檣策蝶
typedef WORD				MONSTERCODE;		///< 跨蝶攪囀萄
typedef	UINT				PLAYERHP;			///< Ы溯檜橫HP
typedef WORD				MPTYPE;				///< MP 顫殮
//__NA_001290_20090525_SHIELD_SYSTEM
typedef WORD				PLAYERSD;			///< Ы溯檜橫SD
//_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
typedef FLOAT               PLAYERFP;           ///< 嬪纂綰溯檜萄 FP
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define DAMAGETYPE_MAX  (1000000) // expanded by _NA001605_EP2_DAMAGETYPE_EXTENTION_

//薄Щ 婦溼 
#define MAX_JUMP_NODE 30000
#define MIN_JUMP_NODE 200
/// [2012-5-31 by i4u4me] 贗塭 薄Щ 憲堅葬闈 滲唳戲煎 譆渠 繩Щ剪葬 滲唳
#define MAX_JUMP_DISTANCE			(12)			// 譆渠 薄Щ煎 檜翕й 熱 氈朝 剪葬
//#define MAX_JUMP_DISTANCE			(15)			// 譆渠 薄Щ煎 檜翕й 熱 氈朝 剪葬

#define MAX_ALLPLAYERS_PACKET_SIZE	( 1500 )	// 輿滲 Ы溯檜橫 薑爾 瞪歎衛 譆渠 ぬ韃餌檜鍔

#define MAX_ITEMNAME_LENGTH			( 64 )		// 嬴檜蠱 檜葷 望檜
#define MAX_SKILLNAME_LENGTH		( 64 )		// 蝶鑒 檜葷 望檜
#define	MAX_MAPNAME_LENGTH			( 64 )		// 裘檜葷 望檜
enum { MAX_ABILITY_NUM = 5 }; // 蝶鑒ж釭渡 譆渠 橫網楝じ(蝶鑒�膩�) 偃熱
#define MAX_SUMMONED_NUM			( 10 )		// и廓縑 模�秘Ж� 譆渠葆葬熱
#define MAX_AREA_ID_LENGTH			( 4 )		// Area 艙羲 ID 望檜

// (NOTE) an unused section since the 0816 version
// removed by __NA_963_LOG_LIB_VERSION_REFACTORING

#define	MAX_STATEINFOSTREAM_SIZE			( 1024 )	// 議葛攪 鼻鷓薑爾
#define MAX_DBSAVE_STATEINFO_NUM			( 40 )		// DB縑 盪濰腎朝 譆渠 鼻鷓偃熱

#ifdef _NA_001990_ACSYSTEM_ADD
	//_NA_007422_20140716_MODIFY_MAX_ACHIEVEMENT_COUNT
    #define MAX_ACHIEVEMENT_COUNT				(1000)   //譆渠 機瞳 
    #define MAX_ACHIEVEMENT_REWARD				(2)     //機瞳 爾鼻 偃熱
    #define MAX_ACHIEVEMENT_OBJECT				(10)    //機瞳 婁營 偃熱
    #define ACHIEVEMENT_UINT_SIZE				(23)    //機瞳 ж釭曖 望檜
    #define MAX_ACHIEVEMENT_PARENT_CODE         (4)     //摹чAC囀萄 譆渠偎熱
#else
    #define MAX_ACHIEVEMENT_COUNT				(300)   //譆渠 機瞳 
    #define MAX_ACHIEVEMENT_REWARD				(2)     //機瞳 爾鼻 偃熱
    #define MAX_ACHIEVEMENT_OBJECT				(8)     //機瞳 婁營 偃熱
    #define ACHIEVEMENT_UINT_SIZE				(19)    //機瞳 ж釭曖 望檜
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
#define MAX_EVENT_INVENTORY_SLOT_NUM	(5*5)			//< 檜漸お 檣漸饜葬 蝸煜 偃熱 иむ檜雖 5*5 = 25

#define MAX_INVENTORY_TAB_NUM			(5)				//< 檣漸饜葬 晦獄 龔 偃熱
#define MAX_CHARGE_INVENTORY_TAB_NUM	(5)				//< 檣漸饜葬 晦獄 龔 偃熱
#define MAX_INVENTORY_TAB_TOTAL_NUM		(MAX_INVENTORY_TAB_NUM+MAX_CHARGE_INVENTORY_TAB_NUM)
#define MAX_INVENTORY_SLOT_PER_TAB		(15)			//< 檣漸饜葬 龔渡 蝸煜 偃熱
enum { MAX_PANDORA_SLOT_NUM	= 8 }; // っ紫塭 鼻濠 蝸煜 偃熱
#define MAX_CHANGE_ITEMSTAT_SLOT_NUM	(4)				//< 嬴檜蠱 蝶邐 滲�� 蝸煜 偃熱

#define MAX_WAREHOUSE_INVENTORY_PWD_LENGTH	(8)			// 璽堅 檣漸饜葬 廓�� 

#define MAX_SSN_STREAM_LENGTH				(10)		// Social Society Number (10濠 梱雖虜... 望檜 望橫紫 鼠衛)
#ifdef _KR_0_20120207_CRYPT_SHA256
#define MAX_SSN_ENCODED_STREAM_LENGTH		(65)
#else
#define MAX_SSN_ENCODED_STREAM_LENGTH		(16)
#endif

//啪歜憮幗縑憮虜 餌辨ж朝 蝸煜
#define MAX_TRADE_SLOT_NUM				(20)

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 鼻薄 葬蝶お 婦溼
#define MAX_ONLOOKER_VENDOR_NUM			(100)			//鼻薄跡煙 NPC 餌辨濠 POOL曖 Init size(棺橫陳 熱 氈擠)
#define COUNT_SEARCH_CONTAINER_INIT		(10)			//鼻薄跡煙 匐儀衛 漱攪曖 Init size(棺橫陳 熱 氈擠)
#define COUNT_VENDOR_IN_PAGE			(10)			//鼻薄跡煙 иむ檜雖縑 菟橫陞 葬蝶お 熱
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

#define TEMP_DBSERIAL_VALUE				(0)
#define DBSERIAL_MASK_VALUE				(0xFFFFFFFFFFFFFFFF)
#define QUICK_SLOT_NULL_VALUE			(255)
#define INVALID_SLOT_POS				(255)
#define MAX_LEVEL						( 100 )		// 譆渠 溯漣(離�� 譆渠 溯漣擎 夥莞 熱 氈棻堅 л!!)

#define MIN_PENALTY_LEVEL			    ( 11 )      // 譆模 ぬ割じ 瞳辨 溯漣
#ifdef _NA_003966_20111227_ADD_ENCHANT
enum { MAX_ENCHANT_GRADE = 15 };
#else
enum { MAX_ENCHANT_GRADE = 12 };
#endif
enum { MAX_AWAKENING_GRADE = 9, };                  // 譆渠 陝撩 溯漣 // _NA_008124_20150313_AWAKENING_SYSTEM
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

// 嘐暮 婦溼
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

#define BEGIN_MISSION_MAP				1		//嘐暮 裘囀萄  衛濛
#define BEGIN_CHUNTING_MAP				40001	//Cьた 裘囀萄 衛濛
#define BEGIN_PVP_MAP					20001	//PVP 裘囀萄   衛濛	

//嘐暮 萵溯檜 婦溼.
#define MISSION_RELAY_NOTICE_TIME		21000

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 溯漣滌 晦棟 薯и 婦溼
//#define	CREATE_GUILD_LIMIT_LEVEL		(20)
#define LIMIT_LEVEL_WARHOUSE			(6)
#define LIMIT_LEVEL_RANKUP				(7)
#define LIMIT_LEVEL_ENCHANT				(8)
#define LIMIT_LEVEL_ACCESSORY			(15)
#define LIMIT_LEVEL_SACCESSORY			(30)

#ifdef _NA_007408_20140711_MODIFY_CRYSTALLIZE
// 唸薑�� 溯漣薯и 餉薯
#else
#define LIMIT_LEVEL_CRYSTALIZE			(9)
#endif //_NA_007408_20140711_MODIFY_CRYSTALLIZE
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

/// 億煎 薑曖
#define MAX_EXP_POTION_VALUE		( 10000000 ) //唳я纂 僭擒戲煎 螢萵 熱 氈朝 譆渠 唳я纂
#define DISTANCE_SPACE_GET_ITEM		7.0f	// 蝶む檜蝶煎 詳擊 熱 氈朝 嬴檜蠱擎 10 嘐攪檜頂 氈朝 嬴檜蠱

#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    enum { MAX_SKILL_LEVEL = 10 }; // 譆渠 蝶鑒 溯漣
    enum { MAX_SKILL_OVER_LEVEL = 20 }; // 譆渠 蝶鑒 螃幗 溯漣
#else
    #ifdef _NA_0_20100901_SKILLTREE_SYSTEM
        enum { MAX_SKILL_LEVEL = 5 }; // 譆渠 蝶鑒 溯漣
        enum { MAX_SKILL_OVER_LEVEL = 10 }; // 譆渠 蝶鑒 螃幗 溯漣
    #else
        enum { MAX_SKILL_LEVEL = 10 }; // 譆渠 蝶鑒 溯漣
        enum { MAX_SKILL_OVER_LEVEL = 20 }; // 譆渠 蝶鑒 螃幗 溯漣
    #endif
#endif

// 檜葷 望檜 薑曖
#define MAX_CLIENT_CHARNAME_LENGTH	( MAX_CHARNAME_LENGTH + 1 )	 // 贗塭檜樹お縑 爾頂朝 ぬ韃檜釭 贗塭檜樹お縑憮 議葛攪 檜葷擊 氬晦嬪и 蝶お葭 望檜.
#define MAX_CLASSNAME_LENGTH		( 16 )
#define MAX_TITLEID_LENGTH			( 16 )		// 議葛攪檜葷檜諼曖 か熱 蘆��
#define MAX_SHOPNAME_LENGTH			( 32 )		// 鼻薄 檜葷 望檜

#define MAX_SMALLDATETIME_SIZE		( 16 )		// "2006-10-12 24:03" ( 碟欽嬪 )
#define MAX_MIDDATETIME_SIZE		( 19 )		// "2006-10-12 24:03:00" ( 蟾欽嬪 )
#define MAX_DATETIME_SIZE			( 23 )		// "2006-10-12 24:03:00.123" ( 塵葬蟾欽嬪 )

#ifdef _INTERNATIONAL_UI
 #if defined (_GLOBAL) || defined (_RUSSIA)
	#define MAX_ROOMTITLE_LENGTH		( 31 )
 #else 
	#define MAX_ROOMTITLE_LENGTH		( 29 )
 #endif//磁嘐朝 30濠 斜諼朝 28濠煎薯и
#else
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    #define MAX_ROOMTITLE_LENGTH		( 31 )
#endif//_INTERNATIONAL_UI(艙僥擎 30濠煎薯им棲棻.)

#define MAX_ROOMPASSWORD_LENGTH		( 7 )		// 寞 綠塵廓�� 譆渠 望檜
#define	MAX_VENDOR_TITLE_LENGTH		(80)		//< 偃檣鼻薄 顫檜ぎ 望檜
#define	MAX_VENDOR_SEARCH_LENGTH	(20)		//< 匐儀橫 望檜
#define VENDOR_ITEMS_PER_PAGE       (12)        // 偃檣鼻薄 匐儀唸婁 む檜雖渡 嬴檜蠱 偃熱

#define MAX_CHAT_LEN				(100)
#define MAX_NOTICE_LEN				(1024)
#define MAX_SHOUT_LEN				(255)

// 偃熱 薑曖
#define MAX_MONSTER_NUM				( 10000 )	// 啪歜 瞪羹曖 跨蝶攪 熱
#define MAX_PLAYER_NUM				( 1000 )	// 啪歜 瞪羹曖 譆渠 翕蕾 議葛攪 熱
#define MAX_FIELDITEM_NUM			( 2000 )	// 啪歜 瞪羹曖 嬴檜蠱 熱
#define MAX_TARGET_NUM				( 32 )		// 彰嬪奢問衛 譆渠奢問 陛棟и 議葛攪 熱
enum { MAX_FIGHTING_ENERGY_NUM = 5 }; // 譆渠 醱瞪衛鑒 熱 氈朝 癱晦 偃熱
#define MAX_EFFECT_COUNT			( 10 )		// 蝶鑒衛瞪衛 譆渠 檜めお 偃熱

#define MAX_PARTYMEMBER_NUM			( 10 )		// だじ錳曖 譆渠 偃熱
#define MAX_FIELD_PARTYMEMBER_NUM	( 10 )		// в萄縑憮 だじ錳曖 譆渠 偃熱

#ifdef _NA_002050_20110216_ADD_GENDER
    #define MAX_CHARACTER_LIST_NUM (10) // и 憮幗捱縑憮 議葛攪 儅撩й 熱 氈朝 譆渠 偃熱
    #define MAX_CHARACTER_SLOT_NUM (10) // и 憮幗捱縑憮 議葛攪 儅撩й 熱 氈朝 譆渠 偃熱
    // 議葛攪 蝸煜 偃熱 婦溼 撲薑 (滲唳 衛 營網萄 в蹂)
    struct MaxCharListConfig
    {
        enum { kDefaultSlotSize = 6 }; // 晦獄 議葛攪 蝸煜 偃熱
        enum { kMaxSlotSize = MAX_CHARACTER_LIST_NUM }; // 譆渠 議葛攪 蝸煜 偃熱
        enum { kUseExtension = 1 }; // 議葛攪 蝸煜 �挫� 餌辨 罹睡 (0, 1)
        enum { kSlotSizePerExtension = 1 }; // �挫� 衛紫衛 棺橫釭朝 議葛攪 蝸煜 偃熱
    };
#else
    #define MAX_CHARACTER_LIST_NUM (6) // и 憮幗捱縑憮 議葛攪 儅撩й 熱 氈朝 譆渠 偃熱
    #define MAX_CHARACTER_SLOT_NUM (6) // и 憮幗捱縑憮 議葛攪 儅撩й 熱 氈朝 譆渠 偃熱
#endif // _NA_002050_20110216_ADD_GENDER

#define VILLAGE_SECTOR_SIZE			40			// 葆擊裘曖 播攪 觼晦(箔んお 籀葬蒂 嬪и и偃曖 播攪 觼晦(薑餌陝⑽))
#define FIELD_SECTOR_SIZE			40			// в萄曖 播攪 觼晦
#define ROOM_SECTOR_SIZE			40			// 寡ぎ瑛曖 播攪 觼晦
#define DOMINATION_SECTOR_SIZE		40			// 雖羲薄滄瞪曖 播攪 觼晦

#define PLAYER_MOVE_UPDATE_TICK		(500)		// Ы溯檜橫曖 檜翕 機等檜お Tick
#define SERVER_UPDATE_TICK			(20)		// 憮幗曖 瞪羹 Update Tick


#define DISTANCE_NPC_MEET			6.0f		// 6 嘐攪檜頂 npc諦 蕾摹 й熱 氈朝 嬪纂煎 獄棻.
#define JUMP_DAMAGE_HEIGHT			8.0f		// 8 嘐攪 睡攪 薄Щ 等嘐雖 瞳辨
#define JUMP_METER_PER_DAMAGE		60.0f		// 嘐攪渡 等嘐雖

#define MAX_IPADDRESS_SIZE					15

#define DEFAULT_ADJUST_LIMIT			(3.0f)							// GetTileToStand曖 檣濠高

// 擁棲詭檜暮 Щ溯歜滌 衛除
// 縛霞縑憮 DEF_NUM_FRAMES_PER_MS 曖 高擎 0.03f 
#define DEF_NUM_FRAMES_PER_MS (30)
#define SPEED_MULTIPLIER				(0.001f)	// 議葛攪 蝶Я萄 啗熱
#define MAX_MAP_NUM						(100)		// World.txt曖 Row曖 偃熱

//偃檣鼻薄 熱熱猿
#define TRADE_COMMISSION_RATE			(0.05f)		//< 5%

#if NATION_BUILD_VERSION >= 1002
#define CHAR_DESTROY_LEVEL				(70)		//< 議葛攪 諫瞪 餉薯ж朝 溯漣 (70)
#else
#define CHAR_DESTROY_LEVEL				(40)		//< 議葛攪 諫瞪 餉薯ж朝 溯漣 (40)
#endif

// GM 貲滄曖 MAX Define 高
#define GM_MAX_CREATE_ITEM_NUM			( 1000)
#define GM_MAX_CREATE_MONEY				( 1000000000)

#define GM_MAX_STAT_UP_VALUE			( 100)
#define GM_MAX_SKILLPOINT_UP_VALUE		( 100)
#define	MAX_RANK_OPTION					( 49)
#define	MAX_ITEM_GRADE					( 1)		//⑷營朝 0擎 橾奩, 1擎 蛤夥檣嬴檜蠱檜棻. 蹺�� 2朝 嶸棲觼 嬴檜蠱檜 腆匙檜棻.

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// PVP婦溼 衛除 裁溯檜
#define PVP_PRESS_KEY_TIMEOUT_TIME		(30*1000)
#define PVP_PRESS_KEY_LEAVE_TIME		(PVP_PRESS_KEY_TIMEOUT_TIME - (10*1000))	// 20 蟾 翕寰 擬港橈戲賊 葆擊煎 給嬴陛朝 ぬ韃陳葡
#define PVP_MATCHLESSS_DELAY_TIME		(10*1000)
#define PVP_REVIVAL_DELAY_TIME			(20*1000)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 唳爵ьた 婦溼 衛除 裁溯檜
#define CHUNTING_TIMEATTACK_EXPIRE_TIME				(20*60*1000)	// 顫歜橫鷗 嶸雖衛除
#define CHUNTING_SURVIVAL_EXPIRE_TIME				(20*60*1000)	// 憮幗檜弊 嶸雖衛除
#define CHUNTING_INIT_POTION_NUM					(40)			// 蟾晦ん暮 偃熱
#define CHUNTING_MAX_MEMBER_NUM						(10)			// 譆渠 詹幗熱
#define CHUNTING_RESURRECTION_DELAY_TIME			(5*1000)        // 睡�� 裁溯檜
#define CHUNTING_INTERVAL_START_READY				(20*1000)       // 顫 だじ 殮撩 �� 啪歜 衛濛梱雖 裁溯檜
#define CHUNTING_TIMEATTACK_EXPIRE_TIME_GUILD_PVP	(10*60*1000)
#define CHUNTING_SURVIVAL_EXPIRE_TIME_GUILD_PVP		(10*60*1000)

#define SKILL_COOLTIME_MARGIN (500) // Server: 贗塭爾棻朝 籐顫歜擊 濛啪 濩嬴憮 籐顫歜縑 曖и 蝶鑒 褒ぬ徽擊 雪醮遽棻.
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 望萄 婦溼
#define	MAX_GUILDMARKSTREAM_SIZE		(3072)
#define	MAX_GUILDSTYLESTREAM_SIZE		(8)

#define	MAX_GUILDNOTICE_LENGTH			(768)
#define MAX_GUILD_INTRODUCTION_LENGTH   (768)

#define	CREATE_GUILD_LIMIT_LEVEL		(20)			//<< 望萄璽撲 溯漣薯и
#define GUILD_UP2GP_RATE				(1000000)		//<< 望萄曖 UP:GP 滲�� 綠徽

#ifdef _NA_007239_20140521_BUG_GUILDRELATION_COUNT
#define MAX_ALLIENCE_GUILD_NUM			(20)			// 翕裊 望萄 譆渠熱
#define MAX_HOSTILITY_GUILD_NUM			(20)				// 瞳渠 望萄 譆渠熱 
#define MAX_ONE_SIDE_HOSTILITY_NUM		(100)			// 橾寞瞳 瞳渠摹ん 望萄 譆渠熱
#else // 望萄葬景橡 檜瞪-檜��
#define MAX_GUILD_RELATION_NUM			(10)			// и 望萄渡 婦啗 望萄 譆渠熱
#define MAX_ALLIENCE_GUILD_NUM			(10)			// 翕裊 望萄 譆渠熱
#define MAX_HOSTILITY_GUILD_NUM			(5)				// 瞳渠 望萄 譆渠熱 
#define MAX_ONE_SIDE_HOSTILITY_NUM		(100)			// 橾寞瞳 瞳渠摹ん 望萄 譆渠熱
#endif //_NA_007239_20140521_BUG_GUILDRELATION_COUNT

#define MAX_GUILD_MEMBER_NUM            (200)			//<< ⑷營 嫡擊 熱 氈朝 譆渠 望萄錳 熱
#define	MAX_GUILD_CORPS_MEMEBER_NUM     (60)			//<< 望萄 捱欽檜 嫡擊 熱 氈朝 譆渠 望萄錳 熱

#define MAX_GUILD_CORPS_NUM			(8)				// 望萄渡 譆渠 捱欽熱(霜й捱欽 んл)

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#define MAX_GUILD_JOIN_REQUEST_NUM      (30)            // 望萄 陛殮 蹂羶 跡煙 譆渠熱
#define MAX_GUILD_JOIN_REQUEST_TIME_MAX (24*60*60)      // 望萄 陛殮 蹂羶 嶸雖 衛除(欽嬪 蟾)
#define MAX_GUILD_JOIN_REQUEST_PLAYER_NUM (5)           // Ы溯檜橫陛 褐羶й 熱 氈朝 譆渠 陛殮 蹂羶 熱

#define MAX_GUILD_FACILITY              (8)             // 望萄 衛撲 譆渠熱
#define MAX_GUILD_FACILITY_OPTION       (2)             // 望萄 衛撲 褫暮 譆渠熱

typedef BYTE FCODE;                                     //望萄 衛撲 謙盟
typedef int  FLEVEL;                                    //望萄 衛撲 溯漣
typedef BYTE ATYPE;                                     //望萄 �側瘓� 謙盟
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 顫塊и 餌錳
#define VERSION_AREA_TILE				( 1)            //WATだ橾 幗瞪  

typedef INT					UPTYPE;
typedef INT					GPTYPE;
typedef INT					GUILDGUID;
typedef INT					GUILDMARKIDX;		///< 裘囀萄
typedef	INT					CORPSGUID;			
typedef	INT					CAMPGUID;
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
typedef INT					CHARGUID;
typedef INT					USERGUID;
typedef BYTE				MISSIONNO;			//< MAX_MISSION_NUM蒂 剩雖 彊朝棻.
typedef int					RANKINGKEY;			//< 楨韁 橾溼 廓�� 
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
typedef BYTE                RANKINGID;          //< 裘囀萄曖 醞犒戲煎 檣и 堅嶸 酈高 蹺陛.
typedef BYTE                RANKINGTYPE;         //< 裘囀萄陛 偽堅 楨韁 顫殮 棻蒂 熱 氈晦 陽僥縑 蹺陛.
typedef WORD               RANKING;            // 楨韁 牖嬪 ル衛 顫殮
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
typedef WORD				RANKINGNO;			//< 楨韁 謙盟滌 廓�� == MAPCODE
typedef	INT					CORPSGUID;			//< 望萄 捱欽 GUID
typedef INT                 GAMECHUUSN;
typedef UINT64              BETAKEY;
#ifdef _NA_0_20100819_AGGRO_RENEWAL
typedef DWORD               BATTLEPOINT;
#else
typedef WORD                BATTLEPOINT;
#endif
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
typedef INT64               SCORE;              //< 楨韁 薄熱
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

/// о鼻 顫殮擊 檜匙擊 餌辨ж晦煎 л!!!!! 濠猿⑽(DATATYPE) 鱔橾
typedef WORD				MAPID;				///< server identification
typedef int					SOUND_INDEX;		///< sound index : it's same sound handle
typedef DWORD				PLAYERKEY;

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// WAYPOINT 婦溼
typedef WORD				WPINDEX;			//< ⑷營 DB朝 int煎 腎橫氈擠
#ifdef _NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT
  #define MAX_CHAR_WAYPOINT_NUM		(15)			// 偃檣 錚檜 ん檣お 譆渠 偎熱
#else
  #define MAX_CHAR_WAYPOINT_NUM		(10)			// 偃檣 錚檜 ん檣お 譆渠 偎熱
#endif //_NA_007942_20150113_MODIFY_RUNE_STONE_WAYPOINT_MAXCOUNT
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 瓣餵 婦溼
#define MAX_COLLECTION_LIST_NUM	(255)			//< 蝶觼董お曖 譆渠 偎熱 (瓣餵 陛棟и 瓣餵僭 謙盟)
#define MAX_FIELD_COLLECTION_NUM (255)			//< в萄縑 襄營 ж朝 譆渠 偎熱

#define MAX_COLLECT_DISTANCE_OBJ	(10)		//< 瓣餵僭婁 瓣餵 陛棟и 譆渠 剪葬		(瓣餵僭婁 剪葬離)
#define MAX_COLLECT_DISTANCE_CHAR	(5)			//< 馨葛攪陛 爾鼻 嫡擊熱 氈朝 譆渠 剪葬	(衛濛衛諦 謙猿衛 剪葬離)

//{__NA001283_20090303_COLLECTION_MAPOBJECTID
// 裘螃粽薛お 酈朝 儅撩衛 20000廓 渠蒂 餌辨 頂睡瞳戲煎 餌辨腎貊,
// お葬剪縑憮朝 1廓睡攪 衛濛腎朝 廓�ㄧ� 餌辨, 
// お葬剪薹婁 翱婦脹 煎霜縑憮朝 お葬剪 廓�ㄦ� 滲唳 в蹂ж棻.

// MapObjectKey朝 (200000~)高擊 餌辨ж貊, TriggerObjectKey朝 (1~)高擊 餌辨и棻.
//ж嬪酈(1~)蒂 鼻嬪酈(200000~)煎 滲��
#define Conv_MapObjectKey(Key)	(MAP_OBJECT_KEY>Key?Key+MAP_OBJECT_KEY:Key)

//鼻嬪酈(200000~)蒂 ж嬪酈(1~)煎 滲��
#define Conv_TriggerObjectKey(Key)	(MAP_OBJECT_KEY<=Key?Key-MAP_OBJECT_KEY:Key)
//}

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//斜瑜 裕晦 衛蝶蠱 '__KR_001355_20090608_GROUPINGSYSTEM_DATA'
#define MAX_GROUP_MEMBER_LOW_MAX (5)        //偃檣檜 陛韓熱 氈朝 譆渠 ж嬪 跡煙 偃熱
#define MAX_GROUP_MEMBER_HIGH_MAX (1)       //偃檣檜 陛韓熱 氈朝 譆渠 鼻嬪 跡煙 偃熱
#define MAX_GROUP_MEMBER (MAX_GROUP_MEMBER_LOW_MAX + MAX_GROUP_MEMBER_HIGH_MAX)//斜瑜識 檣錳熱
#define MAX_DATE_GROUPBONUS (7)             //斜瑜唳я纂 爾傘蝶蒂 嫡晦 嬪п 營蕾п撿й 陳熱
#define HIGH_MEMBER_LEVEL (80)              //鼻嬪 詹幗 溯漣, 醞措 幗瞪縑憮虜 餌辨
#define LOW_MEMBER_LEVEL (10)               //ж嬪 詹幗 溯漣, 醞措 幗瞪縑憮虜 餌辨
#if defined (_CHINA)
    #define MAX_GROUP_EXP (10000000)	            //螃Щ塭檣 譆渠 援瞳 唳я纂
#else
    #define MAX_GROUP_EXP (3000000)	            //螃Щ塭檣 譆渠 援瞳 唳я纂
#endif

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

typedef DWORD REGENID;
typedef DWORD UNITGROUPID;

// 檣隸 婦溼п憮 Щ煎饜屬縑憮 餌辨ж朝高
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
const int IP_MAX_LEN			= 32;	// NOTE: f110923.1L, replace with MAX_IP_STRING_LEN. 嶸棲囀萄蒂 渠綠п憮 Size蒂 棺葡(16->32)
const int MAX_IP_LENGTH         = 16;   // NOTE: f110923.1L, does not use anymore, 憧 憮幗諦 蜃蹺晦 嬪п 蹺陛
const int AUTH_SERIAL_KEY_LEN	= 32;	// 蕾樓 衛葬橡 酈(檣隸)

const int MAX_WORLD_NAME_LEN	= 32;	// 錯萄檜葷 望檜
const int MAX_CHANNEL_NAME_LEN  = 32;   // 瓣割檜葷 望檜

const DWORD HEARTBEAT_TICK      = 20000;
#if USING_GAMEGUARD
const DWORD GAME_GUARD_CHECK_TICK = 5000; // 5蟾縑 и 廓噶 啪歜陛萄 羹觼
#endif // USING_GAMEGUARD

#ifdef _KR_0_20120207_CRYPT_SHA256
    enum { kCryptStringSize = 65 };
#else
    enum { kCryptStringSize = 16 };
#endif

//煎斜だ橾曖 唳煎 譆渠 望檜
#define		MAX_LOGPATH_LEN			256
//橾薑 旎擋 檜鼻 剪楚衛 煎斜 陴梯(1橘)
#define		LOG_TRADE_OVER_MONEY	100000000
//嘐暮寞 儅撩衛 溯漣 離檜高.
#define		MISSION_LEVEL_JOIN_OFFSET	25

#define		HUNTING_JOIN_LEVEL_OFFSET	15

const BYTE MAX_VALKYRIE_RELOAD_COUNT = 20;	// 嫦酈葬 鼠晦 譆渠 濰瞪偃熱

#ifdef _NA_000094_20100622_ENHANCED_USER_BLOCK_LIST
    #define MAX_FRIEND_STAT_NUM	200 // и ぬ韃縑 菟橫陞 熱 氈朝 譆渠 耀掘 璋濠
    #define MAX_BLOCK_STAT_NUM 200 // и ぬ韃縑 菟橫陞 熱 氈朝 譆渠 離欽 璋濠
    #define MAX_FRIENDBLOCK_STAT_NUM 400 // и ぬ韃縑 菟橫陞 熱 氈朝 耀掘/離欽 んл 譆渠 璋濠
#else
    #define MAX_FRIEND_STAT_NUM	90 // и ぬ韃縑 菟橫陞 熱 氈朝 譆渠 耀掘 璋濠
    #define MAX_BLOCK_STAT_NUM 90 // и ぬ韃縑 菟橫陞 熱 氈朝 譆渠 離欽 璋濠
    #define MAX_FRIENDBLOCK_STAT_NUM 190 // и ぬ韃縑 菟橫陞 熱 氈朝 耀掘/離欽 んл 譆渠 璋濠
#endif //_NA_000094_20100622_ENHANCED_USER_BLOCK_LIST

#define MAX_REVENGE_STAT_NUM 10 // и ぬ韃縑 菟橫陞 熱 氈朝 譆渠 葬漸雖 渠鼻 璋濠
#define		MAX_FRIENDINVITATE_REWARD_NUM	MAX_FRIEND_STAT_NUM * 4 // и ぬ韃縑 菟橫陞 熱 氈朝 耀掘 爾鼻 跡煙 薑爾
//_NA_0_20100527_ADD_CHAR_RELATION_MEMO
#define     MAX_FRIEND_MEMO_LENGHT      114     // 鼻撮 薑爾璽曖 詭賅 餌檜鍔

#define __AUTH_TEST__

/*薑葬脹 蛤だ檣*/#define REMOVE_ATTACK_MOVE              // 2006 .2 .18 / 夢遽鷓 / 奢問檜翕 薯剪

// 檜漸お 婦溼 高
#define		_MAX_EVENT_REGITEM			5		// и 檜漸お縑 蛔煙й 熱 氈朝 譆渠 嬴檜蠱 偃熱

// NOTE: increased by _KR_0_20091021_FIRENDINVITATION_EXTEND //耀掘蟾渠 檜漸お朝 爾鼻 檜 4陛雖 顫殮檜渡.
#define		_MAX_EVENT_REWARD			4		// и 檜漸お縑憮 輿朝 爾鼻 陛雖 熱 

#define		_MAX_EVENT_ROW_NUM			100		// ⑷營 熱ч醞檣 檜漸お 譆渠 偃熱

#define		MAX_AKPATTERN_SIZE				1024	// 嬴該韃 ぬ欐 等檜攪 譆渠 觼晦

#define MAX_MEMO_NUM			200		// 譆渠 薹雖 偃熱
#define MAX_MEMO_PAGENUM		20		// и む檜雖(ぬ韃 欽嬪)縑 爾頂雖朝 譆渠 薹雖 偃熱(識 200偃賊 20*10 = 200檜嘎煎 10廓 瞪歎л)
#define MAX_MEMO_LENGTH			200		// 褒薯 望檜(BYTE)
#define MAX_MEMO_STRLENGTH		201		// NULLんл
#define MAX_SUBJECT_WLENGTH		10		// 褒薯 望檜(wchar_t)
#define MAX_SUBJECT_LENGTH		20		// 褒薯 望檜(BYTE)
#define MAX_SUBJECT_STRLENGTH	21		// NULLんл
typedef ULONGLONG					MEMOGUID;

// 瓣割瓣た寞 譆渠 殮濰 檣錳(define 橈歷棻)
#if defined (_RUSSIA)
#define	MAX_CHATZONE_USER			500
#else
#define	MAX_CHATZONE_USER			255
#endif
// Null 僥濠蒂 DWORD ⑽戲煎 滲⑽и 高 
#define Null_Value 1819047246

// 檣隸憮幗縑憮 贗塭檜樹お 蕾樓謙猿 蹂羶й 陽 檜嶸 Ы楚斜 蹺陛-璽⑷噢陛 雖錶憮 棻衛 蹺陛. ぬ纂 釭陞 陽 define ヴ溥撿 л
#define _AUTH_USERDISCONN_CODE

#define	MAX_VERSION_SIZE			64		// 蝶觼董お釭 陝謙 幗瞪曖 餌檜鍔

//觼楚蓮葬んお 婦溼 高菟.
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
#define MAX_ITEM_OPTION_KIND_NUM			128     // 餉薯 _NA_000251_20100727_SOCKET_SYSTEM

#define SKILLCODE_INVALID					0
#define SKILLCODE_NORMAL_ATTACK				65500
#define SKILLCODE_FIGHINGENERGY				2101		// 蝶Я葩葬離雖 蝶鑒囀萄

#define MAX_GUILD_DELEGATE_RIGHT			6			//__NA_000596_20070821_GUILD_CLASS_SYSTEM


#define INFINITE_ITEM_DAY					10000

#define HEIGHT_OF_STORY						20			// и 類曖 堪檜

 ///////////////////////////////////////////望萄 ///////////////////////////////////////////
#define INVALID_GUILDGUID	(0)
#define MAX_REPURCHASE_ITEM_NUM	(5)                             // 營掘衙 嬴檜蠱 跡煙曖 譆渠 偎熱
#define GUILDWAREHOUSE_USING_LEVEL	(1)                         // 望萄 璽堅蒂 餌辨й 熱 橈朝 溯漣

#ifdef _NA_003923_20120130_GUILD_RENEWAL
    #define GUILDWAREHOUSE_SLOTEX_GUILD_GRADE       (3)         // �挫� 蝸煜擊 餌辨й 熱 橈朝 望萄 蛔晝
    #define GUILDWAREHOUSE_SLOTEXPOS	            (25)        // 望萄 溯漣 3檜鼻睡攪 餌辨陛棟и 蝸煜 嬪纂 廓��
    #define GUILDWAREHOUSE_SLOTEXPOS_BY_CASHITEM	(50)	    // 望萄 溯漣 議蓮 嬴檜蠱擊 餌辨л戲煎賦 餌辨 陛棟и 蝸煜 嬪纂 廓��
    #define MAX_GUILDWAREHOUSE_SLOT_PER_TAB         (25)        //  望萄 璽堅 龔渡 蝸煜 偃熱
#else
    #define GUILDWAREHOUSE_SLOTEX_GUILD_GRADE       (2)         // �挫� 蝸煜擊 餌辨й 熱 橈朝 望萄 蛔晝
    #define GUILDWAREHOUSE_SLOTEXPOS	            (24)        // 望萄 溯漣 2檜鼻睡攪 餌辨陛棟и 蝸煜 嬪纂 廓��
    #define GUILDWAREHOUSE_SLOTEXPOS_BY_CASHITEM	(48)	    // 望萄 溯漣 議蓮 嬴檜蠱擊 餌辨л戲煎賦 餌辨 陛棟и 蝸煜 嬪纂 廓��
    #define MAX_GUILDWAREHOUSE_SLOT_PER_TAB         (24)        //  望萄 璽堅 龔渡 蝸煜 偃熱
#endif //_NA_003923_20120130_GUILD_RENEWAL

#define MAX_GUILDWAREHOUSE_TAB_NUM (1)                       // 望萄 璽堅 晦獄 龔 偃熱
#define MAX_GRADE_GUILDWAREHOUSE_TAB_NUM	 (1)        // 望萄 璽堅 望萄 蛔晝檜 3檜 脾戲煎賦 餌辨 陛棟и 譆渠 龔 偃熱
#define MAX_CHARGE_GUILDWAREHOUSE_TAB_NUM	 (1)        // 望萄 璽堅 議蓮蠱戲煎 餌辨 陛棟и 譆渠 龔 偃熱
#define MAX_GUILDWAREHOUSE_TAB_TOTAL_NUM (3)           // 望萄 璽堅縑憮 餌辨陛棟и 譆渠 龔 偃熱

#define MAX_GUILDWAREHOUSE_LOGLIST_NUM	(200)
#define MAX_GUILDWAREHOUSE_LOGPAGE_NUM	(20)

#define GUILD_DESTROY_WAIT_TIME	(3*24*60)

#define GUILDGRADE_AT_GUILDWAREHOUSE_CREATE (2)         //望萄 璽堅蒂 儅撩й 熱 氈朝 Guild Grade
#define GUILDWAREHOUSE_TAB_COUNT_AT_GUILD_CREATE (1)    //望萄 璽堅 儅撩衛縑 輿橫雖朝 璽堅 龔 偃熱
#define GRADE_AT_GUILD_CREATE (1)                       //望萄 儅撩衛 晦獄 Guild Grade

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
#define	MAX_GUILDNAME_LENGTH		( 16 )		// 望萄貲 譆渠 望檜
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
////////////////////////////////////////////雖羲薄滄瞪////////////////////////////////////////
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
// 棻擠 ж歜 煎傳 撲薑 о跡菟擎 贗塭/憮幗/DB ⑺曖 橈檜朝 滲唳 碳陛
#define LOTTO_MIN_NUMBER            1   // 摹鷗 陛棟и 譆模 璋濠
#define LOTTO_MAX_NUMBER            25  // 摹鷗 陛棟и 譆渠 璋濠
#define LOTTO_NUM_PICKS             4   // 擬賅/蹺繩衛 賃 偃曖 璋濠蒂 摹鷗й 匙檣陛
#define LOTTO_NUM_RECENT_DRAWS      2   // 譆斬 賃 �蜃鷩� ж歜 煎傳 等檜攪蒂 棻瑙 匙檣陛 ( >= 2)
#define LOTTO_MAX_NOTICE            10  // 譆斬 賃 偃曖 渡繩 奢雖 詭衛雖蒂 爾婦й 匙檣陛

#if defined (_GLOBAL) || defined (_RUSSIA)
#define LOTTO_MAX_TICKETS           30   // и 議葛攪陛 爾嶸й 熱 氈朝 犒掏 辨雖曖 譆渠 偎熱
#else
#define LOTTO_MAX_TICKETS           5   // и 議葛攪陛 爾嶸й 熱 氈朝 犒掏 辨雖曖 譆渠 偎熱
#endif

// 憮幗難 む檜雖 籀葬蒂 嬪и 鼻熱
#define LOTTO_NUM_WINNERS_PER_PAGE  200 // 渡繩濠 跡煙 摹鷗衛 む檜雖渡 溯囀萄 熱
// }

#define CHANNEL_DOMINATION_WAR 0

//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
//雖羲 薄滄瞪 衛蝶蠱
#define MAX_DOMINATION_ACTIONLIST (10)      // 唳衙璽縑 ル衛腆 望萄曖 葬蝶お 璋濠   
#define MAX_DOMINATION_MAP (100)            // 雖羲 薄滄瞪擊 衛濛й 裘曖 瞪羹 偎熱
#define MAX_DOMINATION_AUCTION (100)         // и雖羲 唳衙縑 殮雞й 熱 氈朝 望萄曖 偎熱
#define MAX_DOMINATION_DEFENSEWALL_NUM (3)  // 雖羲薄滄瞪 寞漁 偎熱
#define DOMINATION_DEFFENSWALL_POINT (5)    // ⑷營 5ん檣お陛 MAX檜雖虜 蹺�� 滲唳檜 陛棟ж棻.
// WARNING: to gamelulu, only you need test value (3)!, you must be restore to valid value (20)
#define MAX_DOMINATION_MEMBER (20)          // 薄滄瞪縑 霤罹й 熱 氈朝 詹幗 譆渠高
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收

#define CONDITION_DELEY             (1000)  // 鐘蛤暮 お葬剪 機等檜お 顫歜

#if defined (_RUSSIA)
    #define GIFT_REQ_USERLEVEL          (20)    //楝衛嬴 摹僭ж晦 陛棟 Level
#else
    #define GIFT_REQ_USERLEVEL          (25)    //磁嘐 摹僭ж晦 陛棟 Level
#endif


#ifdef _CHINA  //_NA_001290_20090525_MODIFY_CALC_SD
#define SDINCREASE_RATIO            (3.0f)  // SP 啗骯辨 隸陛 綠徽纂
#else
//youngmoon #2996 SD 奢衝 滲唳
//#define SDINCREASE_RATIO            (8.0f)  
#define SDINCREASE_RATIO            (6.0f)  
#endif

#define STATE_BATTLE_TIME           (5000)  // 寡ぎ 鼻鷓 雖樓 衛除
//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
#define MAX_SYSTEM_MEMO_LIST        (200)   // и廓縑 爾鳥 熱 氈朝 衛蝶蠱 詭賅 偎熱
#else
#define MAX_SYSTEM_MEMO_LIST        (100)   // и廓縑 爾鳥 熱 氈朝 衛蝶蠱 詭賅 偎熱
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR

//_NA_0_20100901_SKILLTREE_SYSTEM
#ifdef _NA_004530_20120315_1202_SKILL_RENEWAL
    enum { CHARACTER_SKILL_LEVEL_RATIO = 1 }; // 溯漣 1渡 1ん檣お
    enum { REQUIRE_USED_SKILL_POINT_FOR_NEXT_LINE = 20 };
#else
    enum { CHARACTER_SKILL_LEVEL_RATIO = 3 }; // 議葛攪溯漣 3縑 蝶鑒溯漣 1
    enum { REQUIRE_USED_SKILL_POINT_FOR_NEXT_LINE = 5 }; // 棻擠 塭檣曖 蝶鑒擊 櫛�鰼熏序� 餌辨脹 蝶鑒 ん檣お 綠徽
#endif

//_NA_20110630_BATTLE_ZONE_RENEWAL
#define MAX_MISSION_COUNT           (100)   //雖薑й 熱 氈朝 嘐暮 譆渠 高
#define MAX_PARTYROOM_LIST          (40)    //寡ぎ襄 葬蝶お縑 ル衛腎朝 だじ瑛 葬蝶お 譆渠高
#define MAX_PARTYROOM_SEARCH_LIST   (100)   //匐儀唸婁 葬蝶お 譆渠 高

#define MAX_REQUIRE_ITEM            (10)     //蹂掘 嬴檜蠱 謙盟高 (蝶觼董お曖 蹂掘 偎熱諦 橾纂п撿л)
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
// 議衛 憧 婦溼
//收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收
#define GIFT_MESSAGE_LENGTH 201
#define BUY_STORAGE_ITEM_PER_PAGE 30
#define GIFT_STORAGE_ITEM_PER_PAGE 30
#define SHOP_MAX_ITEM_BUY_COUNT 30
#define SHOP_MAX_ITEM_GIFT_COUNT 30
#define SHOP_MAX_CASH_DETAIL_COUNT 10
#define SHOP_MAX_ITEM_PROPERTY_COUNT 10

// 憧 啪歜 囀萄.
#ifdef _KOREA
    #define SHOP_GAMECODE 113 
    #define SHOP_SALESZONE 329
    #define SHOP_SERVERTYPE 0   // 檣啪歜 ん檣お 嶸⑽滌 雖晝 ぷ撫お 褻��(InquireInGamePointValue() 縑憮 餌辨). 賬縑憮朝 餌辨ж雖 彊擠.
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

