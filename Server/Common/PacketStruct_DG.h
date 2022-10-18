#ifndef __PACKETSTRUCT_DG_H__
#define __PACKETSTRUCT_DG_H__

#include <Struct.h>
#include <PacketStruct.h>
#include <SCSlotHeader.h>
#include <Protocol_DG.h>
#include <ServerStruct.h>
#include <QuestDefine.h>

#include <PacketStruct_AG.h>

#include <RankingStruct.h>
#include <CGuid.h> // error C2065: 'GUID_NULL'
#include <atltime.h>
//
//	<WARNING>
//		Header 파일은 현재 파일의 Pack, Warning, align등의 정보설정 이후 포함하게 되면 망가집니다.
//		Header file에서는 자신이 정의하는 것에만 자신이 설정한 옵션이 적용될 수 있도록 처리해 주세요.
//
#include "SunEventInfoParser.h"
#pragma pack(push,1)

/*
/// 가변 패킷 정의
/// category : DG_CHARINFO
/// protocol : DG_CHARINFO_CHARLIST_ACKV
< 패킷 순서 & 사이즈 >
BYTE	category;
BYTE	protocol;
DWORD	dwObjectKey;
*/

#define PACKET_DG_OPEN( c, p )	struct MSG_##p : public MSG_BASE_FORWARD { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_DG_CLOSE		};

#define PACKET_DG_INHERIT_OPEN( c, p, b )	struct MSG_##p : public b { MSG_##p(){ m_byCategory = c; m_byProtocol = p; }
#define PACKET_DG_INHERIT_CLOSE		};


//--------------------------------------------------------------------------------------------------------
//
// DB에 기록하기 위한 프로토콜 ---------------------------- START
//
//--------------------------------------------------------------------------------------------------------
// _NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
PACKET_DG_OPEN(DG_CONNECTION, DG_CONNECTION_DBCONNECT_UPDATE_CMD)
    int db_connector_index_;
PACKET_DG_CLOSE;

PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_RECOVER_SYN )
	enum		{ MAX_RECOVER_INFO_NUM = 150, };
	WORD		m_byCount;
	struct RECOVER_INFO{
		DWORD	m_dwKey;
		BYTE	m_bySelectedCharSlotIndex;
		DWORD	m_dwCharGUID;
	}m_RECOVER_INFO[MAX_RECOVER_INFO_NUM];
	int GetSize()
	{
		return (sizeof(MSG_DG_CONNECTION_RECOVER_SYN) - sizeof(RECOVER_INFO)*(MAX_RECOVER_INFO_NUM-m_byCount));
	}
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_RECOVER_ACK )
	enum		{ MAX_RECOVER_INFO_NUM = 150, };
	WORD		m_byCount;
	struct RECOVER_INFO{
		DWORD	m_dwKey;
	}m_RECOVER_INFO[MAX_RECOVER_INFO_NUM];
	int GetSize()
	{
		return (sizeof(MSG_DG_CONNECTION_RECOVER_ACK) - sizeof(RECOVER_INFO)*(MAX_RECOVER_INFO_NUM-m_byCount));
	}
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_RECOVER_NAK )
	enum		{ MAX_RECOVER_INFO_NUM = 150, };
	WORD		m_byCount;
	struct RECOVER_INFO{
		DWORD	m_dwKey;
	}m_RECOVER_INFO[MAX_RECOVER_INFO_NUM];
	int GetSize()
	{
		return (sizeof(MSG_DG_CONNECTION_RECOVER_NAK) - sizeof(RECOVER_INFO)*(MAX_RECOVER_INFO_NUM-m_byCount));
	}
PACKET_DG_CLOSE



PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_CONNECT_SYN )
//	BYTE		m_bySelectedCharSlotIndex;
	CHARGUID	m_CharGuid;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_CONNECT_ACK )
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_CONNECT_NAK )
	BYTE m_byErrorCode;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_ENTER_SYN )
	//BYTE		m_bySelectedCharSlotIndex;
	CHARGUID	m_CharGuid;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_ENTER_ACK )
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_ENTER_NAK )
	BYTE m_byErrorCode;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_LEAVE_SYN )
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_LEAVE_ACK )
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_LEAVE_NAK )
	BYTE m_byErrorCode;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CONNECTION, DG_CONNECTION_DISCONNECT_CMD )
PACKET_DG_CLOSE

//
//PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_ALL_REQ_SYN )
//	enum { JOIN_VILLAGE=0, OTHER=1, };
//	BYTE		m_bySelectedCharSlotIndex;
//	BYTE		m_byType;
//	CHARGUID	m_CharGuid;
//#ifdef _WHATTHEHELL_EVENTTIME_20060811
//	BYTE		m_IsPCRoom;		// 0: 일반유저 1: PC방 유저
//#endif
//PACKET_DG_CLOSE
//
//PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_ALL_REQ_ACK )
//	enum { JOIN_VILLAGE=0, OTHER=1, };
//	BYTE m_byType;
//PACKET_DG_CLOSE
//
//PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_ALL_REQ_NAK )
//	enum { JOIN_VILLAGE=0, OTHER=1, };
//	BYTE m_byType;
//	DWORD m_byErrorCode;
//PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_CHAR_ITEM_CMD )
	BYTE				m_ChannelID;
	BASE_PLAYERINFO		m_CharacterInfo; // NOTE: f101021.2L, contains char_guid
	_ITEM_TOTAL_INFO	m_ItemInfo;
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize + m_ItemInfo.GetSize());
    }
PACKET_DG_CLOSE
#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
    PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD )
    DWORD char_guid_; // f101021.2L
    BYTE				m_ChannelID;        
    REPURCHASITEM_TOTAL_INFO m_RepurchasItem;
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_RepurchasItem);
        return static_cast<uint16_t>(kMinSize + m_RepurchasItem.GetSize());
    }
PACKET_DG_CLOSE
#endif
//_KR_0_20091021_FIRENDINVITATION_EXTEND
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_INVITATEDFRIEND_CMD )
    DWORD char_guid_; // f101021.2L
    BYTE				        channel_id_;            
    CHAR_INVITATEDFRIEND_INFOs  friend_list_;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(friend_list_);
        return static_cast<uint16_t>(kMinSize + friend_list_.GetSize());
    }
PACKET_DG_CLOSE

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_CHAR_ACHIEVEMENT_CMD )
    DWORD char_guid_; // CHANGES: f110311.4L, changes update info. since f101021.2L
    BYTE channel_id_;
	Character_Achievements	achievement_info_;
	
	uint16_t GetSize() const
	{
        const size_t kMinSize = sizeof(*this) - sizeof(achievement_info_);
        return static_cast<uint16_t>(kMinSize + achievement_info_.GetSize());
	}
PACKET_DG_CLOSE	
#endif//C_NA_0_20100520_ACSYSTEM_CHANGED

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_SKILL_COOL_TIME_CMD )
    DWORD char_guid_; // f101021.2L
    BYTE				m_ChannelID;
    SkillCoolTime	skill_cool_time;
    uint16_t GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(skill_cool_time);
        return static_cast<uint16_t>(kMinSize + skill_cool_time.GetSize());
    }
PACKET_DG_CLOSE
#endif

PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_SKILL_CMD )
    DWORD char_guid_; // f101021.2L
	BYTE				m_ChannelID;
	SKILL_TOTAL_INFO	m_SkillInfo;
	uint16_t GetSize() const
	{
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_SkillInfo);
        return static_cast<uint16_t>(kMinSize + m_SkillInfo.GetSize());
	}
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_QUICK_CMD )
    DWORD char_guid_; // f101021.2L
	BYTE				m_ChannelID;
	QUICK_TOTAL_INFO	m_QuickInfo;
	uint16_t GetSize() const
	{
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_QuickInfo);
        return static_cast<uint16_t>(kMinSize + m_QuickInfo.GetSize());
	}
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_STYLE_CMD )
    DWORD char_guid_; // f101021.2L
	BYTE				m_ChannelID;
	STYLE_TOTAL_INFO	m_StyleInfo;
	uint16_t GetSize() const
	{
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_StyleInfo);
        return static_cast<uint16_t>(kMinSize + m_StyleInfo.GetSize());
	}
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_QUEST_CMD )
    DWORD char_guid_; // f101021.2L
	BYTE				m_ChannelID;
	BYTE				m_pProgrQStream[MAX_PROGR_QUESTSTREAM_SIZE];
	BYTE				m_pComplQStream[MAX_COMPL_QUESTSTREAM_SIZE];
	BYTE				m_pMissionStream[MAX_MISSIONSTREAM_SIZE];
  #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
	BYTE				m_pACStream[MAX_ACSTREAM_SIZE];
  #endif
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_WAREHOUSE_CMD )
    DWORD char_guid_; // f101021.2L
	BYTE					m_ChannelID;
	MONEY					m_WarehouseMoney;
	_WAREHOUSE_TOTAL_INFO	m_ItemInfo;
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize + m_ItemInfo.GetSize());
    }
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_EXTRA_CMD )
    DWORD char_guid_; // f101021.2L
	BYTE					m_ChannelID;
	time_t					m_LoginTime_t;
	STATE_GM_INFO			m_GMStateInfo;
	STATE_DETAIL_TOTAL_INFO m_TotalInfo;

    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<uint16_t>(kMinSize + m_TotalInfo.GetSize());
    }
PACKET_DG_CLOSE;

// CHANGES: f110426.2L, changes to forwarding message to make successful replication process.
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_CONNECT_CHAR_CMD )
    CHARGUID char_guid;
    int session_index;
    MSG_AG_ZONE_ROOM_JOIN_SYN forward_msg;
    //
    uint16_t GetSize() const { return sizeof(*this); };
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_DISCONNECT_CHAR_CMD )
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_DISCONNECT_CMD )
	BYTE		m_ChannelID;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_QUERY_SYN )
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_QUERY_ACK )
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_QUERY_NAK )
	DWORD		m_dwErrorCode;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_UPDATE_SYN )
	BYTE		m_ChannelID;
    CHARGUID    char_guid_; // CHANGES: f110412.2L
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_UPDATE_ACK )
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_UPDATE_NAK )
	DWORD m_dwErrorCode;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_CHARINFO, DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN )
	enum eCONTROL { SERIALIZE_LOCKING = 0,	SERIALIZE_UNLOCKING = 1,
								  DBUPDATE_LOCKING = 2,	DBUPDATE_UNLOCKING = 3, 
					              SERIALIZE_UNLOCKING_WITH_GUILDWAREHOUSE=  4,
	};

    BYTE m_byTransControl;
    BYTE m_ChannelID; // NOTE: rarely usage
    DWORD char_guid_; // CHANGES: f110412.2L, added fields
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_CHARINFO, DG_CHARINFO_SKILL_LIST_CMD)
PACKET_DG_CLOSE;

PACKET_DG_OPEN(DG_CHARINFO, DG_CHARINFO_SKILL_PERK_CMD)
    DWORD char_guid_;
    PerkDataList container_;
    uint16_t GetSize() const {
        const int kMinSize = sizeof(*this) - sizeof(container_);
        return static_cast<uint16_t>(kMinSize + container_.GetSize());
    }
PACKET_DG_CLOSE;

PACKET_DG_OPEN(DG_CHARINFO, DG_CHARINFO_SKILL_PERK_CONTROL_CMD)
    PerkControl header;
    PerkControlData data;
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(data);
        return static_cast<uint16_t>(kMinSize + data.GetSize());
    }
PACKET_DG_CLOSE;


PACKET_DG_OPEN(DG_CHARINFO, DG_CHARNAME_CHANGE_CMD)
    CHARGUID char_guid;
    SLOTIDX slot_index;
    POSTYPE item_pos;
PACKET_DG_CLOSE;

PACKET_DG_OPEN(DG_CHARINFO, DG_CHARNAME_CHANGE_ACK)
    CHARGUID char_guid;
    SLOTIDX slot_index;
    POSTYPE item_pos;
    BYTE result;
PACKET_DG_CLOSE;

PACKET_DG_OPEN(DG_CHARINFO, DG_CHARACTER_EXTRA_INFO_CMD)
#ifdef _NA_006372_20130108_BADGE_ITEM
    ToggleItemInfos toggle_item_info_;
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundRecordInfos battle_ground_record_infos_;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    GoldRushRecordInfos goldrush_record_infos_;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    CTime daily_quest_reset_item_use_time;
#endif //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    MiniGameBingoInfos minigame_bingo_infos;
#endif //_NA_008404_20150712_MINIGAME_SYSTEM
PACKET_DG_CLOSE

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
PACKET_DG_OPEN(DG_CHARINFO, DG_USER_EXTRA_INFO_CMD)
    // UserExtraInfo 추가시 수정
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    AttendanceInfo attendance_infos;
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
PACKET_DG_CLOSE
#endif // _NA_007932_20150106_USER_EXTRA_INFO

#ifdef _NA_007086_20140318_MONSTERBOOK
PACKET_DG_OPEN(DG_CHARINFO, DG_CHARACTER_MONSTERBOOK_CMD)
    BYTE packet_type;
PACKET_DG_CLOSE

enum eDG_CHARACTER_MONSTERBOOK_CMD
{
    DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD = 1, // D -> G
    DG_CHARACTER_MONSTERBOOK_ADD_CMD, // G -> D
    DG_CHARACTER_MONSTERBOOK_ADD_ACK, // D -> G
};

struct MSG_DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD : public MSG_DG_CHARACTER_MONSTERBOOK_CMD
{
    MonsterBook monster_book;

    MSG_DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD()
    {
        packet_type = DG_CHARACTER_MONSTERBOOK_SERIALIZE_CMD;
        monster_book.clear();
    }
};

struct MSG_DG_CHARACTER_MONSTERBOOK_ADD_CMD : public MSG_DG_CHARACTER_MONSTERBOOK_CMD
{
    DWORD monster_code;

    MSG_DG_CHARACTER_MONSTERBOOK_ADD_CMD()
    {
        packet_type = DG_CHARACTER_MONSTERBOOK_ADD_CMD;
        monster_code = 0;
    }
};

struct MSG_DG_CHARACTER_MONSTERBOOK_ADD_ACK : public MSG_DG_CHARACTER_MONSTERBOOK_CMD
{
    DWORD monster_code;

    MSG_DG_CHARACTER_MONSTERBOOK_ADD_ACK()
    {
        packet_type = DG_CHARACTER_MONSTERBOOK_ADD_ACK;
        monster_code = 0;
    }
};
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_UPDATE_CMD)
    SunRankingPlayerInfo info;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_UPDATE_ACK)
    SunRankingPlayerInfo info;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_SELECT_SYN)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_SELECT_ACK)
    enum { MAX_SUN_RANKING_PLAYER_INFO = 100, };
    int count;
    SunRankingPlayerInfo infos[MAX_SUN_RANKING_PLAYER_INFO];
    void init()
    {
        for (int i = 0; i < MAX_SUN_RANKING_PLAYER_INFO; ++i)
        {
            infos[i].init();
        }
        count = 0;
    }
    int total_msg_count;    // 전체 메세지 개수
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_SELECT_SYN)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_SELECT_ACK)
    enum { MAX_SUN_RANKING_GRADE_INFO = 100, };
    int count;
    SunRankingGradeInfo infos[MAX_SUN_RANKING_GRADE_INFO];
    void init()
    {
        for (int i = 0; i < MAX_SUN_RANKING_GRADE_INFO; ++i)
        {
            infos[i].init();
        }
        count = 0;
    }
    int total_msg_count;    // 전체 메세지 개수
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_STATUE_SELECT_SYN)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_STATUE_SELECT_ACK)
    StatueInfo        statue_info;
    int               count;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_STATUE_ITEM_SLOT_INFO_INSERT_SYN)
    eSTATUE_TYPE statue_type;
    eSTATUE_INDEX statue_index;
    CHARGUID char_guid;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_STATUE_ITEM_SLOT_INFO_INSERT_ACK)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_STATUE_ITEM_SLOT_INFO_SELECT_SYN)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_STATUE_CHAR_INSERT_SYN)
    eSTATUE_TYPE statue_type;
    eSTATUE_INDEX statue_index;
    CHARGUID char_guid;
    SCORE    score;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_STATUE_CHAR_SELECT_SYN)
    CHARGUID          char_guid;
    _EQUIP_TOTAL_INFO equip_info;
    eSTATUE_TYPE      statue_type;
    eSTATUE_INDEX     statue_index;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_START_SYN)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_START_CMD)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_COMPLETE_SYN)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_COMPLETE_CMD)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_SYN)
    int      grade_boundary [eSUN_RANKING_TYPE_END][eSUN_RANKING_GRADE_END];
    CHARGUID statue_char_guid;
    SCORE    statue_char_equip_score;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_SELECT_COMPLETE_CMD)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_SELECT_COMPLETE_CMD)
PACKET_DG_CLOSE
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
//--------------------------------------------------------------------------------------------------------
//
// DB에 기록하기 위한 프로토콜 ---------------------------- END
//
//--------------------------------------------------------------------------------------------------------


PACKET_DG_OPEN( DG_ITEM, DG_ITEM_SERIAL_SYN )
	SLOTIDX		m_SlotIdx;
	POSTYPE		m_ItemPos;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_ITEM, DG_ITEM_SERIAL_ACK )
	SLOTIDX		m_SlotIdx;
	POSTYPE		m_ItemPos;
	DBSERIAL	m_DBSerial;
	BYTE		m_Type;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_ITEM, DG_ITEM_SERIAL_NAK )
	SLOTIDX		m_SlotIdx;
	POSTYPE		m_ItemPos;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_ITEM, DG_ITEM_INVENTORY_FLAG_STREAM_NTF )
	DWORD	m_dwFlagStream;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_ITEM, DG_ITEM_INVENTORY_FLAG_STREAM_CMD )
	DWORD	m_dwFlagStream;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_START_SYN )
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_START_ACK )
	enum { PASSWORD_LENGTH = 8, };
	CHAR		m_WarehousePwd[PASSWORD_LENGTH];
	MONEY m_WarehouseMoney;
	_WAREHOUSE_TOTAL_INFO m_ItemInfo;
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize + m_ItemInfo.GetSize());
    };
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_START_NAK )
	DWORD m_dwErrorCode;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_END_SYN )
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_END_ACK )
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_END_NAK )
	DWORD m_dwErrorCode;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_FLAG_STREAM_NTF )
	DWORD	m_dwFlagStream;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_FLAG_STREAM_CMD )
	DWORD					m_dwFlagStream;
	sSSN_ENCODED_STREAM		m_EncodedSSN;
PACKET_DG_CLOSE

//////////길드 창고 프로토콜//////////
PACKET_DG_OPEN(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_CMD)
    DWORD char_guid_; // f101021.2L
	BYTE m_ChannelID;
	GUILDGUID m_GuildGuid;
	MONEY m_WarehouseMoney;
	eGUILDWAREHOUSE_UPDATE_FACTOR m_UpdateFactor;
	_GUILDWAREHOUSE_TOTAL_INFO m_ItemInfo;
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_ItemInfo);
        return static_cast<uint16_t>(kMinSize + m_ItemInfo.GetSize());
    };
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAREHOUSE, DG_GUILD_WAREHOUSE_CLOSE_SYN )
	GUILDGUID m_GuildGuid;
PACKET_DG_CLOSE 

PACKET_DG_OPEN( DG_WAREHOUSE, DG_GUILD_WAREHOUSE_CLOSE_ACK )
	GUILDGUID m_GuildGuid;
PACKET_DG_CLOSE 

PACKET_DG_OPEN( DG_WAREHOUSE, DG_GUILD_WAREHOUSE_CLOSE_NAK )
	BYTE m_byErrorCode;
	GUILDGUID m_GuildGuid;
PACKET_DG_CLOSE 

PACKET_DG_OPEN( DG_WAREHOUSE, DG_GUILD_WAREHOUSELOG_UPDATE_SYN )
	MONEY m_Money;
	WORD m_ItemNum;
	WORD m_LimitedIndex;
	BYTE m_bInOutState;
	SLOTCODE m_ItemCode;
	GUILDGUID m_GuildGuid;
	TCHAR m_CharName[MAX_CHARNAME_LENGTH+1];
PACKET_DG_CLOSE 

PACKET_DG_OPEN( DG_WAREHOUSE, DG_GUILD_WAREHOUSE_TAPEX_SYN )
	GUILDGUID m_GuildGuid;
	CHARGUID m_CharGuid;
	BYTE  m_TabCount;
	SLOTIDX m_SlotIndex;
	POSTYPE m_SlotPos;
PACKET_DG_CLOSE 

PACKET_DG_OPEN( DG_WAREHOUSE, DG_GUILD_WAREHOUSE_TAPEX_ACK )
	GUILDGUID m_GuildGuid;
	BYTE  m_TabCount;
	SLOTIDX m_SlotIndex;
	POSTYPE m_SlotPos;
PACKET_DG_CLOSE 

PACKET_DG_OPEN( DG_WAREHOUSE, DG_GUILD_WAREHOUSE_TAPEX_NAK )
	BYTE m_bErrorCode;
PACKET_DG_CLOSE 

//=========================================================================================
// 창고, 인벤토리 잠금 (패스워드)관련
//----------------------------------------------------------------------------------------
PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_PWD_SETTING_ACK )
	enum eRET_CODE
	{
		RET_FAIL			= 0,	//
		RET_SUCCESS 		= 1,	//
		RET_START_FAIL		= 2,	// 질의조차 못한 상태로 게임서버는 로컬 비번 재설정 안한다.
	};
	enum
	{
		PWD_INVENTORY = 1,
		PWD_WAREHOUSE = 2,
	};
	enum { PASSWORD_LENGTH = 8, };
	// <FIELD>
	BYTE					m_byRetCode;	// RET_START_FAIL이면 이하 데이터 무시
	BYTE					m_byTypeCode;
	CHAR					m_Password[PASSWORD_LENGTH];
	//
	// <METHODs>
	inline BYTE&			TypeCode() { return m_byTypeCode; }
	inline BYTE&			ReturnCode() { return m_byRetCode; }
PACKET_DG_CLOSE


//----------------------------------------------------------------------------------------
PACKET_DG_OPEN( DG_WAREHOUSE, DG_WAREHOUSE_PWD_SETTING_SYN )
	enum
	{
		PWD_INVENTORY = 1,
		PWD_WAREHOUSE = 2,
	};
	// <FIELD>
	BYTE		m_PwdType;
	CHAR 		m_STREAM[MAX_WAREHOUSE_INVENTORY_PWD_LENGTH];

	//
	// <METHODs>
	inline BYTE&			GetTypeCode() { return m_PwdType; }
	inline WORD				GetSize()	{ return sizeof(*this); }

	inline VOID				Set( CHAR* pszInput ) { memcpy( m_STREAM, pszInput, sizeof( m_STREAM ) ); }
	inline CHAR*			Get() { return m_STREAM; }
PACKET_DG_CLOSE



PACKET_DG_OPEN( DG_OTHER, DG_OTHER_EXIST_CHARNAME_SYN )
	BYTE m_Type;		//< enum eCHARNAME_QUERY_ACTION_TYPE
	TCHAR m_tszCharName[MAX_CHARNAME_LENGTH+1];
	// (WAVERIX) 문제 생긴 코드이며, 임시 주석처리
	//int GetSize()
	//{
	//	return sizeof(*this)-(MAX_CHARNAME_LENGTH-(int)strlen(m_tszCharName));
	//}
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_OTHER, DG_OTHER_EXIST_CHARNAME_ACK )
	BYTE m_Type;		//< enum eCHARNAME_QUERY_ACTION_TYPE
	USERGUID m_UserGuid;
	CHARGUID m_CharGuid;
	TCHAR m_tszCharName[MAX_CHARNAME_LENGTH+1];
    
    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    TCHAR guild_name_[MAX_GUILDNAME_LENGTH+1];
    int character_class_;
    int character_level_;
    #endif
	// (WAVERIX) 문제 생긴 코드이며, 임시 주석처리
	//int GetSize()
	//{
	//	return sizeof(*this)-(MAX_CHARNAME_LENGTH-(int)strlen(m_tszCharName));
	//}
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_OTHER, DG_OTHER_EXIST_CHARNAME_NAK )
	BYTE m_Type;		//< enum eCHARNAME_QUERY_ACTION_TYPE
	BYTE m_ErrorCode;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_OTHER, DG_OTHER_SHOP_RECV_GIFT_NTF )
	USERGUID m_UserGuid;
	CHARGUID m_CharGuid;
PACKET_DG_CLOSE

#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
PACKET_DG_OPEN( DG_OTHER, DG_OTHER_FATIGUE_CMD )
	WORD	m_wPlayTime;
	WORD	m_wOfflineTime;
PACKET_DG_CLOSE
#endif  //__CN_OO0510__FATIGUE_MATCHING_UPDATE

//__NA_001359_20090619_HEIM_LOTTO_SYSTEM
// 하임 행운 복권 응모 정보 (로깅 목적)
PACKET_DG_OPEN( DG_OTHER, DG_OTHER_HEIM_LOTTO_PICK_NTF )
    CHARGUID iCharGuid;     // 캐릭터 ID
    WORD wDrawNum;          // 회차 번호
    BYTE bIsAuto;           // 자동 여부
    BYTE bPickedNum[4];     // 응모 번호
PACKET_DG_CLOSE

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
// 적립포인트의 계산을 위한 현재의 년도/요일 정보
// 현재 년도 또는 요일이 변경시 통보된다.
PACKET_DG_OPEN( DG_OTHER, DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF )
    WORD    wYear;      // 현재 년도
    WORD    wDayOfWeek; // 현재 요일
    DWORD   dwSecond;   // 현재 시간(초단위)
PACKET_DG_CLOSE
#endif

// 아이템 합성 또는 분해 내역 삽입
PACKET_DG_OPEN(DG_OTHER, DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD)
    typedef WORD MessageSize;
    ItemCompositionOrDecompositionHistory new_composition_or_decomposition_history; // 삽입할 합성 또는 분해 내역
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_DG_CLOSE // DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD
// 아이템 합성 또는 분해 내역 얻어오기
PACKET_DG_OPEN(DG_OTHER, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN)
    typedef ItemCompositionOrDecompositionHistory::RequestType RequestType;
    typedef WORD MessageSize;
    RequestType request_type; // 요청 종류
    bool is_composition; // 합성 여부
    void Init()
    {
        request_type = ItemCompositionOrDecompositionHistory::kUndefinedRequest;
        is_composition = true;
    }
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_DG_CLOSE // DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN
//
PACKET_DG_OPEN(DG_OTHER, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR)
    typedef WORD MessageSize;
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_DG_CLOSE // DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR
//
PACKET_DG_OPEN(DG_OTHER, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK)
    enum { kMaxHistoryListSize = ItemCompositionOrDecompositionHistory::kMaxHistories };
    typedef int ResultCode;    
    typedef BYTE HistoryListSize;
    typedef WORD MessageSize;
    ResultCode result_code; // 요청 처리 결과
    HistoryListSize history_list_size; // 내역 개수
    ItemCompositionOrDecompositionHistory history_list[kMaxHistoryListSize]; // 내역
    void Init()
    {
        result_code = RC::RC_ITEM_FAILED_TO_GET_COMPOSITION_OR_DECOMPOSITION_HISTORIES;
        history_list_size = 0;
    }
    MessageSize GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(history_list);
        return static_cast<MessageSize>(kMinSize + sizeof(history_list[0]) * history_list_size);
    }
PACKET_DG_CLOSE // DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK
// 아이템 합성 또는 분해 내역 수정
PACKET_DG_OPEN(DG_OTHER, DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD)
    enum { kMaxConditionListSize = ItemCompositionOrDecompositionHistory::kMaxHistories };
    typedef BYTE ConditionListSize;
    typedef ItemCompositionOrDecompositionHistory::ModifyCondition ModifyCondition;
    typedef WORD MessageSize;
    ConditionListSize condition_list_size;
    ModifyCondition modify_condition_list[kMaxConditionListSize];
    void Init()
    {
        condition_list_size = 0;
        ::ZeroMemory(modify_condition_list, sizeof(modify_condition_list));
    }
    MessageSize GetSize() const
    {
        const size_t kMinSize = sizeof(*this) - sizeof(modify_condition_list);
        return static_cast<MessageSize>(kMinSize +
            sizeof(modify_condition_list[0]) * condition_list_size);
    }
PACKET_DG_CLOSE // DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD

// 캐릭터 슬롯 확장 요청
PACKET_DG_OPEN(DG_OTHER, DG_OTHER_EXTEND_CHAR_SLOT_SYN)
    typedef WORD MessageSize;
    POSTYPE extend_item_position; // 슬롯 확장 아이템을 사용했다면 인벤토리 위치
    void Init()
    {
        extend_item_position = INVALID_SLOT_POS;
    }
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_DG_CLOSE
//
PACKET_DG_OPEN(DG_OTHER, DG_OTHER_EXTEND_CHAR_SLOT_DBR)
    typedef WORD MessageSize;
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_DG_CLOSE
// 캐릭터 슬롯 확장 응답
PACKET_DG_OPEN(DG_OTHER, DG_OTHER_EXTEND_CHAR_SLOT_ACK)
    enum
    { 
        kSuccess = 0, // 성공
        kSlotIsFull = 1, // 이미 사용 가능한 모든 슬롯 확장
        kEtcError = 2 // 기타 시스템 오류
    };
    typedef int ResultCode;
    typedef WORD MessageSize;
    ResultCode result_code; // 요청 처리 결과
    POSTYPE extend_item_position; // 슬롯 확장 아이템을 사용했다면 인벤토리 위치
    void Init()
    {
        result_code = kEtcError;
        extend_item_position = INVALID_SLOT_POS;
    }
    MessageSize GetSize() const
    {
        return static_cast<MessageSize>(sizeof(*this));
    }
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_SELECT_SYN )
//	USERGUID	m_UserGuid;
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_SELECT_ACK )
	EVENT_ITEM_TOTAL_INFO_EX m_TotalInfo;
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_TotalInfo);
        return static_cast<uint16_t>(kMinSize + m_TotalInfo.GetSize());
    };
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_SELECT_NAK )
	BYTE		m_byErrorCode;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_EVENT, DG_EVENT_UPDATE_SYN )
	//USERGUID	m_UserGuid;
	DWORD		m_ItemSeq;
	POSTYPE		m_ItemPos;
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_UPDATE_ACK )
	POSTYPE		m_ItemPos;
PACKET_DG_CLOSE
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_UPDATE_NAK )
	BYTE		m_byErrorCode;
PACKET_DG_CLOSE

// 오픈베타용 에테르 이벤트 등록상태 확인
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_ITEM_STATUS_SYN )
	CHARGUID	m_CharGUID;
PACKET_DG_CLOSE

// 오픈베타용 에테르 이벤트 등록상태 확인 성공
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_ITEM_STATUS_ACK )
	WORD			m_wEventNum;
	EVENT_STATUS_EX	m_EventStatus[_MAX_EVENT_ROW_NUM];
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_EventStatus);
        return static_cast<uint16_t>(kMinSize + sizeof(m_EventStatus[0]) * m_wEventNum);
    }
PACKET_DG_CLOSE

// 오픈베타용 에테르 이벤트 등록상태 확인 실패
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_ITEM_STATUS_NAK )
	BYTE		m_byErrorCode;
PACKET_DG_CLOSE

// 오픈베타용 에테르 이벤트 등록
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_ITEM_REG_SYN )
	DWORD				m_dwCharGUID;
	EVENT_STATUS_EX		m_EventStatus;
PACKET_DG_CLOSE

// 오픈베타용 에테르 이벤트 등록 성공
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_ITEM_REG_ACK )
	DWORD				m_dwEventID;
PACKET_DG_CLOSE

// 오픈베타용 에테르 이벤트 등록 실패
PACKET_DG_OPEN( DG_EVENT, DG_EVENT_ITEM_REG_NAK )
	DWORD				m_dwEventID;
	BYTE				m_byErrorCode;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_USETIME_CMD )
	INT					m_EventUseTime;			// 이벤트 관련 누적 시간(분 단위)
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_QUERY_EXIST_SYN )
PACKET_DG_CLOSE

PACKET_DG_INHERIT_OPEN( DG_EVENT, DG_EVENT_QUERY_EXIST_ACK, MSG_CG_EVENT_QUERY_EXIST_ACK )
PACKET_DG_INHERIT_CLOSE


PACKET_DG_OPEN( DG_EVENT, DG_EVENT_STATUS_UPDATE_SYN )
	enum eSTATUS { CONTINUING = 0, COMPLETED = 1, };
	EVENT_STATUS_EX		m_EventStatus;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_STATUS_UPDATE_ACK )
	// 성공이든 실패든 트랜잭션 푼다.
	DWORD				m_EventID;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_EVENT, DG_EVENT_SSQ_REWARD_EXIST_CMD )	// D->G (보상 정보 있을 경우)
	DWORD							m_CharGuid;
	SSQ_RESERVED_REWARD_INFO		m_RewardInfo;
	inline	WORD					GetSize() const { return sizeof(*this); }
PACKET_DG_CLOSE

PACKET_DG_INHERIT_OPEN( DG_EVENT, DG_EVENT_SSQ_REWARD_UPDATE_SYN, MSG_DG_EVENT_SSQ_REWARD_EXIST_CMD )	// G->D
PACKET_DG_INHERIT_CLOSE

PACKET_DG_INHERIT_OPEN( DG_EVENT, DG_EVENT_SSQ_REWARD_UPDATE_ACK, MSG_DG_EVENT_SSQ_REWARD_UPDATE_SYN )	// G->D
	BOOLEAN							m_UpdateResult;
PACKET_DG_INHERIT_CLOSE

//_KR_0_20091021_FIRENDINVITATION_EXTEND //프로토콜 선언 = { DG_EVENT_INVITATEFRIEND~ }

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLSELECT_SYN )    
    CHARGUID    CharGUID;//내 GUUID
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLSELECT_NAK )    
    CHARGUID    CharGUID;//내 GUUID
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK )
    CHARGUID    CharGUID;//내 GUUID
    BYTE        Count;
    struct INVITATEFRIENDRECALL_INFO{
        TCHAR	    tszRecallTime[MAX_MIDDATETIME_SIZE + 1];	//최근 소환 시간
        CHARGUID	FriendGUID;                                 //친구 GUID
    } m_Recall_Info[MAX_FRIENDINVITATE_REWARD_NUM];

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_Recall_Info);
        return static_cast<uint16_t>(kMinSize + sizeof(m_Recall_Info[0]) * Count);
    }
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLUPDATE_CMD )    
    CHARGUID    CharGUID;//내 GUUID
    CHARGUID    FriendGUID;//친구 GUID
    DWORD64     RecallTime;//소환 시간
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_INVITATEFRIEND_REWARDUPDATE_SYN )
    CHARGUID    CharGUID;//내 GUUID
    CHARGUID    FriendGUID;//친구 GUID
    LEVELTYPE   ReWardType;//보상 타입
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_INVITATEFRIEND_REWARDUPDATE_ACK )
    CHARGUID    CharGUID;//내 GUUID
    CHARGUID    FriendGUID;//친구 GUID
    LEVELTYPE   ReWardType;//보상 타입
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_EVENT, DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK )
    enum    {DB_FAIL = 1, DBPROXY_FAIL = 2, RECVREWARDED = 3, NOTOBTAININVEN = 4};
    BYTE	m_dwErrorCode;
PACKET_DG_CLOSE

#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
//점령전 경매 보상 정보 및 삭제 요청
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_DOMINATION_REWARD_AND_DEL_SYN)
    MEMOGUID memo_guid_;//메모 Guid
    eSYSTEM_MEMO_TYPE reward_type_;//보상 타입
    MAPCODE reward_area_mapcode_;//경매 지역 맵 코드
    CHARGUID recv_char_guid_;//보상 받는 케릭터 GUID
PACKET_DG_CLOSE
//점령전 경매 보상 정보 응답
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_DOMINATION_REWARD_AND_DEL_ACK)
    MEMOGUID memo_guid_;//메모 Guid
    INT32 result_value_;//보상 요청 DB결과 값
    MAPCODE db_result_map_code_;//DB상에 저장된 MapCode
    MAPCODE request_map_code_;//요청한 MapCode
    eSYSTEM_MEMO_TYPE db_result_reward_type_;//DB에 저장된 보상 타입
    eSYSTEM_MEMO_TYPE request_reward_type_;//보상 타입
    CHARGUID db_result_char_guid_;//DB요청 결과 값에 존재하는 케릭터 GUID
    CHARGUID request_char_guid_;//보상 받는 케릭터 GUID
    ULONGLONG tender_cost_;//경매 입찰 금액
PACKET_DG_CLOSE
//하임 로또 당첨 정보 및 해당 데이터 삭제 요청
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_LOTTO_REWARD_AND_DEL_SYN)
    MEMOGUID memo_guid_;//메모Guid
    CHARGUID request_char_guid_;//요청자 캐릭터 ID
    INT64 lotto_entry_index_;//삭제할 쪽지의 EntryIdx
PACKET_DG_CLOSE
//하임 로또 당첨 정보 응답
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_LOTTO_REWARD_AND_DEL_ACK)
    MEMOGUID memo_guid_;//메모Guid
    INT32 result_value_;//0: 성공, 2:레코드없음, 1:기타오류
    CHARGUID request_char_guid_;//요청자 캐릭터 ID
    CHARGUID db_result_char_guid_;//응모 건의 캐릭터 ID
    INT64 db_result_prize_money_;//상금
PACKET_DG_CLOSE
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN)
    MAPCODE map_code_;//요청한 MapCode
    eSYSTEM_MEMO_TYPE reward_type_;//보상 타입
    CHARGUID char_guid_;//보상 받는 케릭터 GUID
    ULONGLONG tender_cost_;//경매 입찰 금액
PACKET_DG_CLOSE
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC

//_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
//해당 아이템이 존재 하는지 확인
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_SYSTEMMEMO_ITEM_CHECK_SYN)
    CHARGUID char_guid_;
    MEMOGUID memo_guid_;
    //_NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
    TCHAR memo_contents_[MAX_MEMO_LENGTH + 1];//게임 로그를 위한 자료로 사용
PACKET_DG_CLOSE
//아이템 보상확인후 요청
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK)
    bool db_result_value_;
    MEMOGUID memo_guid_;
    CHARGUID char_guid_;
    eSYSTEM_MEMO_TYPE memo_type_;
    SLOTCODE reward_item_code_;
    ULONGLONG reward_item_num_;
    //_NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
    TCHAR memo_contents_[MAX_MEMO_LENGTH + 1];//게임 로그를 위한 자료로 사용
PACKET_DG_CLOSE
//시스템 쪽지 보상 아이템 정보 업데이트
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_CMD)
    MEMOGUID memo_guid_;
    CHARGUID char_guid_;
    bool read_stat_;
    SLOTCODE reward_item_code_;
    ULONGLONG reward_item_num_;
PACKET_DG_CLOSE

//!~ _NA_000000_20140120_SMART_NPC_SYSTEM
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_SMARTNPC_SYSTEM)
    DWORD CID;
    DWORD NID;
    DWORD EID;
    DWORD STEP;
    BYTE packet_type;
PACKET_DG_CLOSE
//~! _NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
PACKET_DG_OPEN(DG_EVENT, DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_SYN)
    GUILDGUID guild_guid;
PACKET_DG_CLOSE
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

//_NA_007919_20141211_ADD_GMCOMMAND_EVENT
PACKET_DG_OPEN(DG_EVENT, DG_GMEVENT_PEAKTIME_REGISTER_SYN)
    EVENT_INFO register_event;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_EVENT, DG_GMEVENT_PEAKTIME_REGISTER_ACK)
    EVENT_INFO register_event;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_EVENT, DG_GMEVENT_PEAKTIME_TERMINATE_SYN)
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_EVENT, DG_GMEVENT_PEAKTIME_TERMINATE_ACK)
PACKET_DG_CLOSE
#ifdef _NA_008012_20150130_RANKING_SYSTEM
//------------------------------------------------------------------------------
// 미션 랭킹 관련
PACKET_DG_OPEN( DG_MISSION, DG_MISSION_RECORD_SELECT_CMD )
	enum { MAX_NUM = MAX_MISSION_NUM, };
	BYTE					m_byCount;
	MISSION_RECORD_INFO		m_MissionRecord[MAX_NUM];

    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(m_MissionRecord);
        return static_cast<uint16_t>(kMinSize + sizeof(m_MissionRecord[0]) * m_byCount);
    }
PACKET_DG_CLOSE
//
//PACKET_DG_OPEN( DG_MISSION, DG_MISSION_CLEAR_UPDATE_SYN )
//	enum { MAX_MEMBER_NUM = MAX_PARTYMEMBER_NUM, };
//	MISSION_RECORD_INFO			m_Record;
//	//LEVELTYPE					m_TotalLV;				//< 따로 계산
//	BYTE						m_byCount;
//	MISSION_RANK_MEMBER_INFO	m_MemberInfo[MAX_MEMBER_NUM];
//	int GetSize()
//	{
//		return (sizeof(*this) - (MAX_MEMBER_NUM-m_byCount*sizeof(MISSION_RANK_MEMBER_INFO)));
//	}
//PACKET_DG_CLOSE
//
//PACKET_DG_OPEN( DG_MISSION, DG_MISSION_CLEAR_UPDATE_ACK )
//PACKET_DG_CLOSE

//PACKET_DG_OPEN( DG_MISSION, DG_MISSION_CLEAR_UPDATE_NAK )
//	BYTE						m_byErrorCode;
//PACKET_DG_CLOSE

//PACKET_DG_OPEN( DG_MISSION, DG_MISSION_RECORD_UPDATE_SYN )
//	MISSION_RECORD_INFO			m_Record;
//PACKET_DG_CLOSE
//
//PACKET_DG_OPEN( DG_MISSION, DG_MISSION_RECORD_UPDATE_ACK )
//PACKET_DG_CLOSE
//
//PACKET_DG_OPEN( DG_MISSION, DG_MISSION_RECORD_UPDATE_NAK )
//	BYTE						m_byErrorCode;
//PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_MISSION, DG_MISSION_RANKING_POINT_REQ_SYN )
	_MISSION_RECORD_INFO		RecordInfo;
	_MISSION_RANK_MEMBER_INFO   MemberInfo;
	DWORD						SettledStartDate;		//< 정산 시작날짜
	BYTE						Period;				//< 주기
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_MISSION, DG_MISSION_RANKING_POINT_REQ_ACK )
	st_RANKING_POINT	Point;	
PACKET_DG_CLOSE
#endif // _NA_008012_20150130_RANKING_SYSTEM

PACKET_DG_OPEN( DG_WAYPOINT, DG_WAYPOINT_INFO_LOAD_SYN )
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAYPOINT, DG_WAYPOINT_INFO_LOAD_ACK )
	WAYPOINT_TOTAL_INFO m_Info;
	int GetSize() 
	{
		return sizeof(MSG_BASE_FORWARD) + m_Info.GetSize();
	}
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAYPOINT, DG_WAYPOINT_INFO_LOAD_NAK )
	DWORD						m_dwErrorCode;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_WAYPOINT, DG_WAYPOINT_INFO_SAVE_SYN )
	WPINDEX						m_MapCode;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAYPOINT, DG_WAYPOINT_INFO_SAVE_ACK )
	WPINDEX						m_MapCode;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAYPOINT, DG_WAYPOINT_INFO_SAVE_NAK )
	DWORD						m_dwErrorCode;
PACKET_DG_CLOSE

	//개인 웨이 포인트 관련
PACKET_DG_OPEN( DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_LOAD_SYN )
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_LOAD_ACK )
    CHAR_WAYPOINT_TOTAL_INFO m_Info;
    uint16_t GetSize() const 
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_Info);
        return static_cast<uint16_t>(kMinSize + m_Info.GetSize());
    }
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_SAVE_SYN )
MAPCODE						m_MapCode;
SHORT						m_X;		
SHORT						m_Y;		
SHORT						m_Z;		
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_SAVE_ACK )
WAYPOINTSLOT m_Info;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_DEL_SYN )
DWORD						m_IdxWayPoint;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_DEL_ACK )
DWORD						m_IdxWayPoint;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_NAK )
DWORD						m_dwErrorCode;
PACKET_DG_CLOSE

//채집물 리스트
PACKET_DG_OPEN( DG_COLLECT, DG_COLLECT_LIST_LOAD_SYN )
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_COLLECT, DG_COLLECT_LIST_LOAD_ACK  )
    COLLECTED_TOTAL_INFO m_Info;
    uint16_t GetSize() const
    {
        // (f100527.3L)
        const int kMinSize = sizeof(*this) - sizeof(m_Info);
        return static_cast<uint16_t>(kMinSize + m_Info.GetSize());
    }
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_COLLECT, DG_COLLECT_ADD_SYN )
WzID	m_CollectionID;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_COLLECT, DG_COLLECT_ADD_ACK )
WzID	m_CollectionID;
WORD	m_wCount;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_COLLECT, DG_COLLECT_DEL_SYN )
WzID	m_CollectionID;
PACKET_DG_CLOSE

PACKET_DG_OPEN( DG_COLLECT, DG_COLLECT_DEL_ACK )
WzID	m_CollectionID;
PACKET_DG_CLOSE


PACKET_DG_OPEN( DG_COLLECT, DG_COLLECT_NAK )
DWORD	m_dwErrorCode;
PACKET_DG_CLOSE

#ifdef _NA_003027_20111013_HONOR_SYSTEM
PACKET_DG_OPEN(DG_CHARINFO, DG_CHARINFO_HONORSYSTEM)
    enum eHonorSystemPacketTypeDG
    {
        DG_HONORSYSTEM_TITLES = 1,
        DG_HONORSYSTEM_FAME_REPUTE,
        DG_HONORSYSTEM_DAILYQUEST,
    };

    BYTE packet_type;
PACKET_DG_CLOSE

//DG_HONORSYSTEM_TITLES
struct MSG_DG_HONORSYSTEM_TITLES : MSG_DG_CHARINFO_HONORSYSTEM
{
    BYTE sub_type;

    MSG_DG_HONORSYSTEM_TITLES()
    {
        packet_type = DG_HONORSYSTEM_TITLES;
    }
};

struct MSG_DG_HONORTITLES_ADD_REMOVE_CMD : MSG_DG_HONORSYSTEM_TITLES
{
    WORD honor_title_index;
};

struct MSG_DG_HONORTITLES_ADD_REMOVE_ACK : MSG_DG_HONORSYSTEM_TITLES
{
    BYTE result;
    WORD honor_title_index;
};

struct MSG_DG_HONORTITLES_SELECT_SYN : MSG_DG_HONORSYSTEM_TITLES
{
    MSG_DG_HONORTITLES_SELECT_SYN()
    {
        sub_type = DG_HONORSYSTEM_TITLES_SELECT_SYN;
    }
};

struct MSG_DG_HONORTITLES_SELECT_ACK : MSG_DG_HONORSYSTEM_TITLES
{
    HonorTitleTable titles;

    MSG_DG_HONORTITLES_SELECT_ACK()
    {
        sub_type = DG_HONORSYSTEM_TITLES_SELECT_ACK;
        memset(&titles, 0, sizeof(titles));
    }
    // CHANGES: f110829.4L
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(titles);
        return static_cast<uint16_t>(kMinSize + titles.GetSize());
    };
};

//DG_HONORSYSTEM_FAME_REPUTE
struct MSG_DG_HONORSYSTEM_FAME_REPUTE : MSG_DG_CHARINFO_HONORSYSTEM
{
    BYTE sub_type;

    MSG_DG_HONORSYSTEM_FAME_REPUTE()
    {
        packet_type = DG_HONORSYSTEM_FAME_REPUTE;
    }
};

struct MSG_DG_FAME_REPUTE_SELECT_SYN : MSG_DG_HONORSYSTEM_FAME_REPUTE
{
    MSG_DG_FAME_REPUTE_SELECT_SYN()
    {
        sub_type = DG_HONORSYSTEM_FAME_REPUTE_SELECT_SYN;
    }
};

struct MSG_DG_FAME_REPUTE_SELECT_ACK : MSG_DG_HONORSYSTEM_FAME_REPUTE
{
    HonorPointTable honor_point_table;

    MSG_DG_FAME_REPUTE_SELECT_ACK()
    {
        sub_type = DG_HONORSYSTEM_FAME_REPUTE_SELECT_ACK;
        memset(&honor_point_table, 0, sizeof(honor_point_table));
    }
    // CHANGES: f110829.4L
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(honor_point_table);
        return static_cast<uint16_t>(kMinSize + honor_point_table.GetSize());
    };
};

struct MSG_DG_HONORSYSTEM_FAME_REPUTE_CHANGE_CMD : MSG_DG_HONORSYSTEM_FAME_REPUTE
{
    BYTE type_;
    WORD code_;
    DWORD point_;

    MSG_DG_HONORSYSTEM_FAME_REPUTE_CHANGE_CMD(BYTE type, WORD code, DWORD point)
    {
        sub_type = DG_HONORSYSTEM_FAME_REPUTE_CHANGE_CMD;
        type_ = type, code_ = code, point_ = point;
    }
};

//DG_HONORSYSTEM_DAILYQUEST
struct MSG_DG_HONORSYSTEM_DAILYQUEST : MSG_DG_CHARINFO_HONORSYSTEM
{
    BYTE sub_type;

    MSG_DG_HONORSYSTEM_DAILYQUEST()
    {
        packet_type = DG_HONORSYSTEM_DAILYQUEST;
    }
};

struct MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_SYN : MSG_DG_HONORSYSTEM_DAILYQUEST
{
    MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_SYN()
    {
        sub_type = DG_HONORSYSTEM_DAILYQUEST_SELECT_SYN;
    }
};

struct MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_ACK : MSG_DG_HONORSYSTEM_DAILYQUEST
{
    WORD count_;
    QUEST_DAILY_INFO daily_quest_info_[MAX_DAILYQUEST_INFO_COUNT];

    MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_ACK()
    {
        sub_type = DG_HONORSYSTEM_DAILYQUEST_SELECT_ACK;
        count_ = 0;
        memset(&daily_quest_info_, 0, sizeof(daily_quest_info_));
    }

    // CHANGES: f110829.4L
    uint16_t GetSize() const {
        const size_t kMinSize = sizeof(*this) - sizeof(daily_quest_info_);
        return static_cast<uint16_t>(kMinSize + sizeof(daily_quest_info_[0]) * count_);
    };
};

struct MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN : MSG_DG_HONORSYSTEM_DAILYQUEST
{
    QUEST_DAILY_INFO quest_data;
    MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN()
    {
        sub_type = DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN;
    }
};
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
PACKET_DG_OPEN(DG_CHARINFO, DG_CHARINFO_POINTWALLET_SYSTEM)
    BYTE packet_type;
PACKET_DG_CLOSE
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
PACKET_DG_OPEN(DG_NOTIFICATION, DG_NOTIFICATION_ITEM_SYN)
    NotificationInfo info;
PACKET_DG_CLOSE

PACKET_DG_OPEN(DG_NOTIFICATION, DG_NOTIFICATION_CLEAR_SYN)
PACKET_DG_CLOSE
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#pragma pack(pop)

#endif	// __PACKETSTRUCT_DM_H__