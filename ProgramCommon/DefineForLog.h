#ifndef __PROGRAMCOMMON_DEFINE_FOR_LOG_H
#define __PROGRAMCOMMON_DEFINE_FOR_LOG_H
#pragma once

#define	MAX_MONSTERNAME_LENGTH		( 40 )		// 몬스터 이름 길이
#define	MAX_CHARNAME_LENGTH			( 16 )		// 캐릭터 이름 -> 서버쪽에서 인증쪽에도 보내고 있으므로 수정시 서버측에 통보바람!!
#define MAX_TIMEDATA_SIZE			( 20 )		// DB의 datetime타입의 길이
#ifdef _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
#define MAX_GUILDLOG_COLUMNSIZE     ( 20 )      //DB 길드로그 컬럼3,4는 nvarchar[20]
#endif //_NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG

#define	MAX_GUILDNAME_LENGTH		( 16 )		// 길드명 최대 길이

#ifdef _SERVER
	#define MAX_CHATMSG_SIZE			( 255 )		// 채팅 최대 메세지 사이즈.
#endif


#define MAX_AC_SECTION_NUM_V2					(6)
#define MAX_AC_SECTION_NUM_V1					(6)
#define MAX_AC_SECTION_NUM_V0					(6)

#ifdef __NA_0_AC_KEY_DIRECT_REGISTER
    #define AC_STREAM_SIZE_LAST_VERSION  (2)
    #define MAX_AC_SECTION_NUM           MAX_AC_SECTION_NUM_V2
#else
    #define AC_STREAM_SIZE_LAST_VERSION  (1)
    #define MAX_AC_SECTION_NUM           MAX_AC_SECTION_NUM_V1
#endif  

#define MAX_ACSTREAM_SIZE                (MAX_AC_SECTION_NUM * 30) // sizeof(ACSTREAM)*MAX_AC_SECTION_NUM

#ifdef _NA_000000_20150511_QUEST_BUG
const int MAX_PROGR_QUESTSTREAM_SIZE	= 650;
#else
const int MAX_PROGR_QUESTSTREAM_SIZE	= 550;
#endif //_NA_000000_20150511_QUEST_BUG

#ifdef _NA_004157_20120409_QUEST_BANDWITH_4000
    const int MAX_COMPL_QUESTSTREAM_SIZE	= 4001;
#else
    const int MAX_COMPL_QUESTSTREAM_SIZE	= 3001;
#endif
#define QUEST_STREAM_SIZE_LAST_VERSION			(1)

enum { MAX_ADDINFO_SIZE = 128 };
#define	MAX_BASE_PLAYER_INFO_SIZE	( 283 )

enum { MAX_EQUIPMENT_SLOT_NUM_V1 = 16 };
enum { MAX_EQUIPMENT_SLOT_NUM_V2 = 19 };
enum { MAX_EQUIPMENT_SLOT_NUM_V3 = 21 };
enum { MAX_EQUIPMENT_SLOT_NUM_V4 = 24 };
enum { MAX_EQUIPMENT_SLOT_NUM_V5 = 24 };
enum { MAX_EQUIPMENT_SLOT_NUM_V6 = 24 }; //_NA_20090925_ITEMSTREAM_INIT_RESERVED
enum { MAX_EQUIPMENT_SLOT_NUM_V7 = 24 }; //__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__
enum { MAX_EQUIPMENT_SLOT_NUM_V8 = 24 }; //_NA002237_100802_EP2_SOCKET_SYSTEM_DB_
//#define MAX_EQUIPMENT_SLOT_NUM_Vx   (24) //_NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_

//#if   defined(_NA000109_100427_SOLAR_SLOT_UPDATE_TO_VER8_)  // v8
//    #define ITEM_STREAM_SIZE_LAST_VERSION    (8)    //100508 reserved, can't extend size anymore
//    #define MAX_ITEMPARTEX_SIZE              (37)
//    #define MAX_EQUIPMENT_SLOT_NUM           MAX_EQUIPMENT_SLOT_NUM_V8
#if defined(_NA002237_100802_EP2_SOCKET_SYSTEM_DB_)
    enum { ITEM_STREAM_SIZE_LAST_VERSION = 8 };    //아이템 스트림 변경
  #ifdef _NA_003740_20111122_RANDOM_ITEM
    enum { MAX_ITEMPARTEX_SIZE = (11 + 34) }; // 11 + 34 = 45
  #else
    enum { MAX_ITEMPARTEX_SIZE = (11 + 24) }; // 11 + 24 = 35
  #endif
    enum { MAX_EQUIPMENT_SLOT_NUM = MAX_EQUIPMENT_SLOT_NUM_V8 };
#else //__NA000000_090906_SOLAR_SLOT_UPDATE_VER6__   // v7    
    enum { ITEM_STREAM_SIZE_LAST_VERSION = 7 };    //아이템 스트림 변경
    enum { MAX_ITEMPARTEX_SIZE = 35 };
    enum { MAX_EQUIPMENT_SLOT_NUM = MAX_EQUIPMENT_SLOT_NUM_V7 };
#endif


#define MAX_INVENTORY_SLOT_NUM						(75*2)
#define MAX_TEMPINVENTORY_SLOT_NUM					(20)
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #define MAX_WAREHOUSE_SLOT_NUM						(5*5*5)
    #define MAX_GUILD_WAREHOUSE_SLOT_NUM				(5*5*3)
#else
    #define MAX_WAREHOUSE_SLOT_NUM						(100)
    #define MAX_GUILD_WAREHOUSE_SLOT_NUM				(6*4*3)
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM

#define MAX_PC_ROOM_EQUIPMENT_SLOT_NUM				(3)			// PC방 슬롯 개수
#define MAX_CHARGE_EQUIPMENT_SLOT_NUM				(2)			// 장착창 유료 캐쉬슬롯 개수

/////////////////////////////////////////////////컨테이너 사이즈////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX_INVENTORYITEMSTREAM_SIZE		( MAX_INVENTORY_SLOT_NUM * MAX_ITEMPARTEX_SIZE )		// 인벤토리 DB GameField Size MAX_INVENTORY_SLOT_NUM(75*2) * sizeof(ITEMPARTEX)
#define MAX_TEMPINVENTORYITEMSTREAM_SIZE	( MAX_TEMPINVENTORY_SLOT_NUM * MAX_ITEMPARTEX_SIZE )	// 임시인벤토리 DB GameField Size MAX_TEMPINVENTORY_SLOT_NUM(20) * sizeof(ITEMPARTEX)
#define MAX_WAREHOUSESTREAM_SIZE			( MAX_WAREHOUSE_SLOT_NUM * MAX_ITEMPARTEX_SIZE )		// 창고 DB GameField Size MAX_WAREHOUSE_SLOT_NUM(100) * sizeof(ITEMPARTEX)	
#define MAX_EQUIPITEMSTREAM_SIZE			( MAX_EQUIPMENT_SLOT_NUM * MAX_ITEMPARTEX_SIZE )		// 장착창 DB GameField Size
#define MAX_GUILDWAREHOUSESTREAM_SIZE		( MAX_GUILD_WAREHOUSE_SLOT_NUM * MAX_ITEMPARTEX_SIZE )	// 장착창 DB GameField Size
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_SKILLSTREAM_SIZE				( 200 )		// 스킬 DB GameField Size
#define MAX_QUICKSTREAM_SIZE				( 192 )		// 퀵정보 DB GameField Size
#define MAX_STYLESTREAM_SIZE				( 8 )		// 스타일 4*2
#define MAX_MISSIONSTREAM_SIZE				( 4 * MAX_MISSION_NUM ) // sizeof(MISSIONPART) * MAX_MISSION_NUM

//_NA_0_20110329_GAMEOPTION_SETTING
#define MAX_GAMEOPTIONSTREAM_SIZE           ( 1024 )

//_NA_0_20110308_SKILL_COOL_TIME_STORAGE
enum { MAX_SKILL_COUNT = 36 }; // 캐릭터가 가지는 최대 스킬 갯수

#endif //__PROGRAMCOMMON_DEFINE_FOR_LOG_H
