#ifndef _SERVER_COMMON_PACKETSTRUCT_CHAOSZONESYSTEM_H_
#define _SERVER_COMMON_PACKETSTRUCT_CHAOSZONESYSTEM_H_

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "PacketStruct_AG.h"
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>

//AG AgentServer <-> GameServer
//--------------------------------------------------------------------------------------------------
// BATTLE_GROUND
//--------------------------------------------------------------------------------------------------
enum eBATTLEGROUND_PACKET_TYPE_AG
{
    AG_BATTLE_GROUND_ENTER_TEAM_CMD = 0,
    AG_BATTLE_GROUND_LEAVE_CMD,
    AG_BATTLE_GROUND_CAN_RESERVATION_SYN,
    AG_BATTLE_GROUND_CAN_RESERVATION_ACK,
    AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD,
};

struct MSG_AG_BATTLE_GROUND_PACKET : MSG_AG_CHAOS_ZONE_PACKET
{
    eBATTLEGROUND_PACKET_TYPE_AG packet_type;
    MSG_AG_BATTLE_GROUND_PACKET()
    {
        category_type = kCategoryBattleGround;
    }
};

struct MSG_AG_BATTLE_GROUND_ENTER_TEAM_CMD : MSG_AG_BATTLE_GROUND_PACKET
{
    KEYTYPE zone_key;
    ChaosZoneTeam team_key;
    BYTE user_count;		// 입장하려는 플레이어 수
    DWORD user_key_list[10];

    MSG_AG_BATTLE_GROUND_ENTER_TEAM_CMD()
    {
        packet_type = AG_BATTLE_GROUND_ENTER_TEAM_CMD;
        zone_key = 0;
        team_key = kChaosZoneTeamNone;
        user_count = 0;
        ZeroMemory(user_key_list, sizeof(user_key_list));
    }
};

struct MSG_AG_BATTLE_GROUND_LEAVE_CMD : MSG_AG_BATTLE_GROUND_PACKET
{
    ZONEKEY zone_key;
    MSG_AG_BATTLE_GROUND_LEAVE_CMD()
    {
        packet_type = AG_BATTLE_GROUND_LEAVE_CMD;
        zone_key = 0;
    }
};
struct MSG_AG_BATTLE_GROUND_CAN_RESERVATION_SYN : MSG_AG_BATTLE_GROUND_PACKET
{
    MAPCODE map_code;
    bool is_healer;
    MSG_AG_BATTLE_GROUND_CAN_RESERVATION_SYN()
    {
        packet_type = AG_BATTLE_GROUND_CAN_RESERVATION_SYN;
        map_code = 0;
        is_healer = false;
    }
};
struct MSG_AG_BATTLE_GROUND_CAN_RESERVATION_ACK : MSG_AG_BATTLE_GROUND_PACKET
{
    MAPCODE map_code;
    RC::eBATTLE_GROUND_RESULT result_code;
    DWORD equip_item_score;             //>>장비평점
    eCHAR_TYPE class_code;              //>>클래스 코드 
    bool is_healer;                     //>>힐러여부
    MSG_AG_BATTLE_GROUND_CAN_RESERVATION_ACK()
    {       
        map_code = 0;
        equip_item_score = 0;
        class_code = eCHAR_NONE;
        is_healer = false;
        packet_type = AG_BATTLE_GROUND_CAN_RESERVATION_ACK;
    }
};

struct MSG_AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD : MSG_AG_BATTLE_GROUND_PACKET
{
    ePLAYER_BEHAVE_STATE behave_sate;
    MSG_AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD()
    {
        packet_type = AG_BATTLE_GROUND_STATUS_CHANGE_STATE_CMD;
        behave_sate = PLAYER_BEHAVE_IDLE_STATE;
    }
};


//--------------------------------------------------------------------------------------------------
// FREE_PVP 
//--------------------------------------------------------------------------------------------------
enum eFREEPVP_PACKET_TYPE_AG
{
    AG_FREE_PVP_LEAVE_ROOM_CMD,
    AG_FREE_PVP_ACTIVE_CMD,
};

struct MSG_AG_FREE_PVP_PACKET : MSG_AG_CHAOS_ZONE_PACKET
{
    eFREEPVP_PACKET_TYPE_AG packet_type;
    MSG_AG_FREE_PVP_PACKET()
    {
        category_type = kCategoryFreePVP;
    }
};

struct MSG_AG_FREE_PVP_LEAVE_ROOM_CMD : MSG_AG_FREE_PVP_PACKET
{
    ZONEKEY zone_key;
    MSG_AG_FREE_PVP_LEAVE_ROOM_CMD()
    {
        packet_type = AG_FREE_PVP_LEAVE_ROOM_CMD;
        zone_key = 0;
    }
};

struct MSG_AG_FREE_PVP_ACTIVE_CMD : MSG_AG_FREE_PVP_PACKET
{
    bool is_free_pvp_active;
    MSG_AG_FREE_PVP_ACTIVE_CMD()
    {
        packet_type = AG_FREE_PVP_ACTIVE_CMD;
        is_free_pvp_active = false;
    }
};
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
//--------------------------------------------------------------------------------------------------
// GOLDRUSH
//--------------------------------------------------------------------------------------------------
enum eGOLDRUSH_PACKET_TYPE_AG
{
    AG_GOLDRUSH_ENTER_TEAM_CMD = 0,
    AG_GOLDRUSH_LEAVE_CMD,

    AG_GOLDRUSH_CAN_RESERVATION_SYN,
    AG_GOLDRUSH_CAN_RESERVATION_ACK,

    AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD,
};

struct MSG_AG_GOLDRUSH_PACKET : MSG_AG_CHAOS_ZONE_PACKET
{
    eGOLDRUSH_PACKET_TYPE_AG packet_type;
    MSG_AG_GOLDRUSH_PACKET()
    {
        category_type = eCHAOSZONE_CATEGORY_GOLDRUSH;
    }
};

struct MSG_AG_GOLDRUSH_CAN_RESERVATION_SYN : MSG_AG_GOLDRUSH_PACKET
{
    MAPCODE map_code;
    bool is_healer;
    MSG_AG_GOLDRUSH_CAN_RESERVATION_SYN()
    {
        packet_type = AG_GOLDRUSH_CAN_RESERVATION_SYN;
        map_code = 0;
        is_healer = false;
    }
};
struct MSG_AG_GOLDRUSH_CAN_RESERVATION_ACK : MSG_AG_GOLDRUSH_PACKET
{
    MAPCODE map_code;
    RC::eBATTLE_GROUND_RESULT result_code;
    DWORD equip_item_score;             //>>장비평점
    eCHAR_TYPE class_code;              //>>클래스 코드 
    bool is_healer;                     //>>힐러여부
    MSG_AG_GOLDRUSH_CAN_RESERVATION_ACK()
    {       
        map_code = 0;
        equip_item_score = 0;
        class_code = eCHAR_NONE;
        is_healer = false;
        packet_type = AG_GOLDRUSH_CAN_RESERVATION_ACK;
    }
};
struct MSG_AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD : MSG_AG_GOLDRUSH_PACKET
{
    ePLAYER_BEHAVE_STATE behave_sate;
    MSG_AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD()
    {
        packet_type = AG_GOLDRUSH_STATUS_CHANGE_STATE_CMD;
        behave_sate = PLAYER_BEHAVE_IDLE_STATE;
    }
};

struct MSG_AG_GOLDRUSH_LEAVE_CMD : MSG_AG_GOLDRUSH_PACKET
{
    ZONEKEY zone_key;
    MSG_AG_GOLDRUSH_LEAVE_CMD()
    {
        packet_type = AG_GOLDRUSH_LEAVE_CMD;
        zone_key = 0;
    }
};

struct MSG_AG_GOLDRUSH_ENTER_TEAM_CMD : MSG_AG_GOLDRUSH_PACKET
{
    KEYTYPE zone_key;
    ChaosZoneTeam team_key;
    BYTE user_count;		// 입장하려는 플레이어 수
    DWORD user_key_list[10];

    MSG_AG_GOLDRUSH_ENTER_TEAM_CMD()
    {
        packet_type = AG_GOLDRUSH_ENTER_TEAM_CMD;
        zone_key = 0;
        team_key = kChaosZoneTeamNone;
        user_count = 0;
        ZeroMemory(user_key_list, sizeof(user_key_list));
    }
};
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_008334_20150608_SONNENSCHEIN
//--------------------------------------------------------------------------------------------------
// SONNENSCHEIN
//--------------------------------------------------------------------------------------------------
enum eSONNENSCHEIN_PACKET_TYPE_AG
{
    AG_SONNENSCHEIN_REQUEST_GUID_SYN = 0,
    AG_SONNENSCHEIN_REQUEST_GUID_ACK,

    AG_SONNENSCHEIN_KICKUSER_CMD,
    AG_SONNENSCHEIN_ROOM_DESTROY_CMD,
    AG_SONNENSCHEIN_STAMP_COMPLETE_CMD,

    AG_SONNENSCHEIN_ENTER_CHECK_SYN,
    AG_SONNENSCHEIN_ENTER_CHECK_ACK,
};

struct MSG_AG_SONNENSCHEIN_PACKET : MSG_AG_CHAOS_ZONE_PACKET
{
    eSONNENSCHEIN_PACKET_TYPE_AG packet_type;
    MSG_AG_SONNENSCHEIN_PACKET()
    {
        category_type = eCHAOSZONE_CATEGORY_SONNENSCHEIN;
    }
};

struct MSG_AG_SONNENSCHEIN_REQUEST_GUID_SYN : MSG_AG_SONNENSCHEIN_PACKET
{
    //생성된 sonnenschein 룸키
    KEYTYPE room_key;

    MSG_AG_SONNENSCHEIN_REQUEST_GUID_SYN()
    {
        packet_type = AG_SONNENSCHEIN_REQUEST_GUID_SYN;
        room_key = 0;
    }
};

struct MSG_AG_SONNENSCHEIN_REQUEST_GUID_ACK : MSG_AG_SONNENSCHEIN_PACKET
{
    //생성된 sonnenschein 룸키
    KEYTYPE room_key;
    CHARGUID creater_char_guid_;
    GUILDGUID creater_guild_guid_;

    MSG_AG_SONNENSCHEIN_REQUEST_GUID_ACK()
    {
        packet_type = AG_SONNENSCHEIN_REQUEST_GUID_ACK;
        room_key = 0;
        creater_char_guid_ = 0;
        creater_guild_guid_ = 0;
    }
};

struct MSG_AG_SONNENSCHEIN_KICKUSER_CMD : MSG_AG_SONNENSCHEIN_PACKET
{
    //room_key가 0이면 모든 길드대상
    //room_key 값이 존재하면 특정 room 대상으로 한다
    KEYTYPE room_key;

    MSG_AG_SONNENSCHEIN_KICKUSER_CMD()
    {
        room_key = 0;
        packet_type = AG_SONNENSCHEIN_KICKUSER_CMD;
    }
};

struct MSG_AG_SONNENSCHEIN_ROOM_DESTROY_CMD : MSG_AG_SONNENSCHEIN_PACKET
{
    KEYTYPE room_key;
    MSG_AG_SONNENSCHEIN_ROOM_DESTROY_CMD()
    {
        room_key = 0;
        packet_type = AG_SONNENSCHEIN_ROOM_DESTROY_CMD;
    }
};

struct MSG_AG_SONNENSCHEIN_STAMP_COMPLETE_CMD : MSG_AG_SONNENSCHEIN_PACKET
{
    KEYTYPE room_key;
    MSG_AG_SONNENSCHEIN_STAMP_COMPLETE_CMD()
    {
        room_key = 0;
        packet_type = AG_SONNENSCHEIN_STAMP_COMPLETE_CMD;
    }
};

struct MSG_AG_SONNENSCHEIN_ENTER_CHECK_SYN : MSG_AG_SONNENSCHEIN_PACKET
{
    MSG_AG_SONNENSCHEIN_ENTER_CHECK_SYN()
    {
        packet_type = AG_SONNENSCHEIN_ENTER_CHECK_SYN;
        entrance_item_code = 0;
        map_code = 0;
    }
    SLOTCODE entrance_item_code;
    MAPCODE map_code;
};

struct MSG_AG_SONNENSCHEIN_ENTER_CHECK_ACK : MSG_AG_SONNENSCHEIN_PACKET
{
    MSG_AG_SONNENSCHEIN_ENTER_CHECK_ACK()
    {
        packet_type = AG_SONNENSCHEIN_ENTER_CHECK_ACK;
        result_code = RC::RC_SONNENSCHEIN_FAILED;
    }

    RC::eSONNENSCHEIN_RESULT result_code;
};
#endif  //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
//--------------------------------------------------------------------------------------------------
// ONEDAYSGUILDMISSION
//--------------------------------------------------------------------------------------------------
enum eONEDAYSGUILDMISSION_PACKET_TYPE_AG	
{
	AG_ONEDAYSGUILDMISSION_KICKUSER_CMD = 0,
	AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD,
	AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD,

	AG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN,
	AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK,

	AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD,
};

struct MSG_AG_ONEDAYSGUILDMISSION_PACKET : MSG_AG_CHAOS_ZONE_PACKET
{
	eONEDAYSGUILDMISSION_PACKET_TYPE_AG packet_type;
	MSG_AG_ONEDAYSGUILDMISSION_PACKET()
	{
		category_type = eCHAOSZONE_CATEGORY_ONEDAYSGUILDMISSION;
	}
};

struct MSG_AG_ONEDAYSGUILDMISSION_KICKUSER_CMD : MSG_AG_ONEDAYSGUILDMISSION_PACKET
{
	//room_key가 0이면 모든 길드대상
	//room_key 값이 존재하면 특정 room 대상으로 한다
	KEYTYPE room_key;

	MSG_AG_ONEDAYSGUILDMISSION_KICKUSER_CMD()
	{
		room_key = 0;
		packet_type = AG_ONEDAYSGUILDMISSION_KICKUSER_CMD;
	}
};

struct MSG_AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD : MSG_AG_ONEDAYSGUILDMISSION_PACKET
{
	KEYTYPE room_key;
	MSG_AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD()
	{
		room_key = 0;
		packet_type = AG_ONEDAYSGUILDMISSION_ROOM_DESTROY_CMD;
	}
};

struct MSG_AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD : MSG_AG_ONEDAYSGUILDMISSION_PACKET
{
	ZONEKEY zone_key;
	MSG_AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD()
	{
		zone_key = 0;
		packet_type = AG_ONEDAYSGUILDMISSION_LEAVE_ROOM_CMD;
	}
};

struct MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN : MSG_AG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN()
	{
		packet_type = AG_ONEDAYSGUILDMISSION_ENTER_CHECK_SYN;
		entrance_item_code = 0;
		map_code = 0;
		pass_require_item_check = FALSE;
	}
	SLOTCODE entrance_item_code;
	MAPCODE map_code;
	BOOL pass_require_item_check;
};

struct MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK : MSG_AG_ONEDAYSGUILDMISSION_PACKET
{
	MSG_AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK()
	{
		packet_type = AG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK;
		result_code = RC::RC_ONEDAYSGUILDMISSION_FAILED;
	}

	RC::eONEDAYSGUILDMISSION_RESULT result_code;
};

struct MSG_AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD : MSG_AG_ONEDAYSGUILDMISSION_PACKET
{
	KEYTYPE room_key_;
	BYTE event_state_;
	DWORD remain_time_; //밀리세컨드
	MSG_AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD()
	{
		packet_type = AG_ONEDAYSGUILDMISSION_TIME_STATE_CMD;
		room_key_ = 0;
		event_state_ = 0;
		remain_time_ = 0;
	}
};

#endif  //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

#endif //_SERVER_COMMON_PACKETSTRUCT_CHAOSZONESYSTEM_H_