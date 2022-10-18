#include "stdafx.h"
#include "Handler_FromClient.h"

#include "PacketHandler.Shared.h"
#include "Handler_FromGameServer.h"

#include "SolarDateTime.h"
#include "../SolarShop/ShopDefine.h" // (WAVERIX) (090816) (NOTE) ������ ������ ��.

#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
#include <ServerOptionParserEx.h>
#endif
#include "BattleOpenTimeManager.h"

#ifdef _NA_20100307_BETAKEY_SYSTEM
#include <BetaKeyImpl.h>
#endif

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
#include "Function/GameOptionManager.h"
#endif //_NA_0_20110329_GAMEOPTION_SETTING

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "AgentParty/AgentPartyManager.h"
#include "Party/PartyRoom.h"

#ifdef _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST
#include "AgentParty/PartyRoomFinder.h"
#endif  // _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosZoneSystem/BattleGroundManager.h"
#include <ChaosZoneSystem/BattleGroundMatchingSystem.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include <Function/GuildRanking/GuildRanking.h>

//��������������������������������������������������������������������������������������������������������������������������������������������������
//
//
#undef ADD_HANDLER_INFO_EOR
#undef ADD_HANDLER_INFO
#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO(c, p, accptablePHASE)		\
	{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_CG::On##p }

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND //���� �޼ҵ� ����

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo_Event()
{
    static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
    {
        ADD_HANDLER_INFO(CG_EVENT, CG_EVENT_INVITATEFRIEND_REWARD_LIST_SYN, PHASE_UNLIMITED),                

        ADD_HANDLER_INFO_EOR(),
    };

    return s_pHandlerInfo;
};

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo_CHARINFO()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(CG_CHARINFO, CG_CHARINFO_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CHARINFO, CG_CHARINFO_DESTROY_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CHARINFO, CG_CHARINFO_RECOVER_SYN, PHASE_UNLIMITED),
#ifdef __NA_001044_ADD_CUSTOM_UI
		ADD_HANDLER_INFO(CG_CHARINFO, CG_CHARINFO_CHARACTERID_CHECK_SYN, PHASE_UNLIMITED),
#endif
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo_SYNC()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(CG_SYNC, CG_SYNC_PLAYER_ENTER_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo_ZONE()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		// �̼� �κ� �� ����Ʈ ��û
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LIST_MISSIONLOBBY_SYN, PHASE_UNLIMITED),
		// ���� �κ� �� ����Ʈ ��û
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LIST_HUNTINGLOBBY_SYN, PHASE_UNLIMITED),	
		// ����� ���� �� ����Ʈ ��û
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LIST_HUNTING_SYN, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LIST_PVPLOBBY_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LIST_CHUNTINGLOBBY_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LIST_CHUNTING_SYN, PHASE_UNLIMITED),

		// Ư�� �� ����Ʈ ���ΰ�ħ
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LIST_REFRESH_SYN, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_SHORTCUT_SYN, PHASE_UNLIMITED),
		// ���̵�� �� ã��
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_FIND_ROOM_FROM_CHARID_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_FIND_ROOM_FROM_NO_SYN, PHASE_UNLIMITED),
		// ������ �κ�(����or�̼�)���� �����
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_CHUNTINGLOBBY_CREATE_SYN, PHASE_UNLIMITED),
		// �κ� ����
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_JOIN_SYN, PHASE_UNLIMITED),
		// [�������� ����]
		// �������� ���� ��ư�� ������.
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_READY_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_READY_CANCEL_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_REQUEST_INFO_SYN, PHASE_UNLIMITED),
		// [������ ����]
		// �ο��� ����
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_NONBLOCKSLOTNUM_SYN, PHASE_UNLIMITED),
		// �÷��̾� ����
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_KICKPLAYER_SYN, PHASE_UNLIMITED),
		// �κ��� ���� ���� �ٲ�
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_HUNTING_CONFIG_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_PVP_CONFIG_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_HUNTING_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_MISSION_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_PVP_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_CHUNTING_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_CHUNTING_MIGRATE_SYN, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_HUNTING_JOIN_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_MISSION_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_HUNTING_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_PVP_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_CHUNTING_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_HUNTING_RELAY_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_HUNTING_RELAY_JOIN_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_CHANGE_TEAM_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_LOBBY_CHANGE_MAP_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_PORTAL_FIELD_MOVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_RETURN_VILLAGE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_MISSION_RELAY_CREATE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_MISSION_RELAY_JOIN_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_REQUEST_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE, CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_LOBBY_NAME_CHANGE_SYN, PHASE_UNLIMITED),		
		ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_INFO_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_SSQ_JOIN_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_SSQ_ROOM_INFO_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_SSQ_OBSERVER_SYN, PHASE_UNLIMITED),
        //__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
        ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN, PHASE_UNLIMITED),
        //_NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
        ADD_HANDLER_INFO(CG_ZONE_EX, CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_SYN, PHASE_UNLIMITED),

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        ADD_HANDLER_INFO( CG_ZONE_EX, CG_ZONE_EX_SPA_ENTER_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO( CG_ZONE_EX, CG_ZONE_EX_SPA_LEAVE_SYN, PHASE_UNLIMITED),
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        //
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo_PARTY()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		// CG_PARTY
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_JOIN_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_JOIN_RESPONSE_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_JOIN_RESPONSE_NAK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_LEAVE_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_FORCED_EXPULSION_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_CHANGE_MASTER_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_DESTROY_SYN, PHASE_UNLIMITED),

		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_SELECT_TARGET_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_MINIMAP_POINTING_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_ITEM_DISTRIBUTION_CHANGE_SYN, PHASE_UNLIMITED),

		//ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_RETURN_G2C_LEADER_QUERY_CMD, PHASE_UNLIMITED),		// ��Ƽ�忡�� ��Ƽ�� �ʴ� ���� �޽���, ��Ƽ���� �ѹ� ��ҽ�
		//ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_RETURN_G2C_MEMBER_QUERY_CMD, PHASE_UNLIMITED),		// ��Ƽ������ �ʴ� �޽���
		ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_RETURN_C2G_MEMBER_QUERY_ACK, PHASE_UNLIMITED),		// ��Ƽ���� �ź� ����
        //_NA_20110630_BATTLE_ZONE_RENEWAL
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_CREATE_ROOM_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_CHECK_ROOM_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_BATTLE_ZONE_INFO_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_LEAVE_ROOM_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_ROOM_LIST_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_CHANGE_ROOM_OPTION_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_CHANGE_MAX_PLAYER_COUNT_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_JOIN_ROOM_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_STANDBY_ROOM_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_READY_STATE_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_ROOM_FORCE_LEAVE_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_ROOM_START_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_SELECT_TEAM_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_REQUEST_MEMBER_INFO_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_FIND_ROOM_BY_CHARID_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_STANDBY_PAUSE_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_REQUEST_RENDER_INFO_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_FIND_ROOM_BY_ROOMNUMBER_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_FIND_WAITING_ROOM_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_FIND_ROOM_BY_MISSION_NAME_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_INVITE_PARTY_ROOM_ANS, PHASE_UNLIMITED),
    #ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        ADD_HANDLER_INFO(CG_PARTY, CG_PARTY_RETURN_G2C_MEMBER_REMOVE_SYN, PHASE_UNLIMITED),
    #endif
    #ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        ADD_HANDLER_INFO(CG_PARTY, CG_CHAOS_ZONE_PACKET, PHASE_UNLIMITED),
    #endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo_GM()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		// GM ��ɾ�(ä������ ��û)
		ADD_HANDLER_INFO(CG_GM, CG_GM_CHANNELINFO_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_GM, CG_GM_WARP_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo_SECURE()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(CG_ARMORKIT, CG_ARMORKIT_FINDHACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CRYPTOGRAPHY, CG_CRYPTOGRAPHY_NAK, PHASE_UNLIMITED),
	#define ADD_HANDLER_INFO_MOLA(c, p, accptablePHASE)	\
		{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&Handler_CG::OnCG_SECURE_MOLACONTROL }

		//������������������������������������������������������������������������������
		ADD_HANDLER_INFO_MOLA(CG_SYNC, CG_SECURE_POLICY_PATTERN01_SIMPLEX_SNK_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_SYNC, CG_SECURE_POLICY_PATTERN02_SIMPLEX_SNK_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_ITEM, CG_SECURE_POLICY_PATTERN03_SIMPLEX_SNK_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_TRADE, CG_SECURE_POLICY_PATTERN04_SIMPLEX_SNK_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_VENDOR, CG_SECURE_POLICY_PATTERN05_QUERY_MOLA_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_WAREHOUSE, CG_SECURE_POLICY_PATTERN06_CONFIGURE_MOLA_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_PARTY, CG_SECURE_POLICY_PATTERN07_CONFIGURE_HEARTBEAT_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_STATUS, CG_SECURE_POLICY_PATTERN08_DUPLEX_SNK_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_CONNECTION, CG_SECURE_QUERY_BUILTIN_CHK_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_CONNECTION, CG_SECURE_QUERY_RESOURCE_CHK_T1_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_CHARINFO, CG_SECURE_QUERY_RESOURCE_CHK_T2_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_MAP, CG_SECURE_QUERY_RESOURCE_CHK_NTF1_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_BATTLE, CG_SECURE_QUERY_RESOURCE_CHK_T3_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_T4_ACK, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_MOLA(CG_STYLE, CG_SECURE_QUERY_RESOURCE_CHK_NTF2_SYN, PHASE_UNLIMITED),
    #if USING_HACKSHIELD
		ADD_HANDLER_INFO(CG_ARMORKIT, CG_HACKSHIELD_SERVER_EX_QUERY_ACK, PHASE_UNLIMITED),
    #elif USING_XIGNCODE
        ADD_HANDLER_INFO(CG_ARMORKIT, CG_XIGNCODE_SERVER_QUERY_ACK, PHASE_UNLIMITED),
    #elif USING_GAMEGUARD
        ADD_HANDLER_INFO(CG_ARMORKIT, CG_GAMEGUARD_SERVER_QUERY_ACK, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_ARMORKIT, CG_GAMEGUARD_HACKDATA_CMD, PHASE_UNLIMITED),
    #elif USING_FROST
    #endif

		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		// CG_CONNECTION
		ADD_HANDLER_INFO(CG_CONNECTION, CG_CONNECTION_BACKTOCHARSELECT_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CONNECTION, CG_CONNECTION_HEARTBEAT_SYN, PHASE_UNLIMITED),
#ifdef _NA_20100802_SPEED_HACK_DETECTION
        ADD_HANDLER_INFO(CG_CONNECTION, CG_CONNECTION_TICK_QUERY_ACK, PHASE_UNLIMITED),
#endif
		ADD_HANDLER_INFO(CG_CONNECTION, CG_CONNECTION_REENTERSERVER_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CONNECTION, CG_CONNECTION_ENTERVILLAGEREQ_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CONNECTION, CG_PREPARE_WORLD_CONNECT_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CONNECTION, CG_CONNECTION_NOTIFY_LOGOUT_SYN, PHASE_UNLIMITED),
		ADD_HANDLER_INFO(CG_CONNECTION, CG_CONNECTION_CRASH_REPORT_SYN, PHASE_UNLIMITED),
    #ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
        ADD_HANDLER_INFO(CG_CONNECTION, CG_CONNECTION_SERVER_SELECT_REQUEST_SYN, PHASE_UNLIMITED),
    #endif
    #ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
        ADD_HANDLER_INFO(CG_CONNECTION, CG_CHARNAME_CHANGE_SYN, PHASE_UNLIMITED),        
    #endif
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

sPACKET_HANDLER_INFO*	Handler_CG::GetHandlerInfo_ETC()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		ADD_HANDLER_INFO(CG_ETC, CG_ETC_GET_BATTLE_OPEN_TIME_SYN, PHASE_UNLIMITED),		

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
    #ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
        ADD_HANDLER_INFO(CG_SHOP, CG_SHOP_SEND_GIFT_SYN, PHASE_UNLIMITED),
    #endif
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef _NA_0_20110329_GAMEOPTION_SETTING
        ADD_HANDLER_INFO(CG_ETC, CG_ETC_GAMEOPTION_LOAD_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_ETC, CG_ETC_GAMEOPTION_SAVE_SYN, PHASE_UNLIMITED),
#endif //_NA_0_20110329_GAMEOPTION_SETTING
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        ADD_HANDLER_INFO(CG_ETC, CG_ETC_GUILD_RANKINGINFO_SYN, PHASE_UNLIMITED),
        ADD_HANDLER_INFO(CG_ETC, CG_ETC_GUILDINFO_SYN, PHASE_UNLIMITED),
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

        //_NA_000000_20120410_SPEEDHACK
        ADD_HANDLER_INFO(CG_ETC, CG_ETC_MEMORY_HACK_CMD, PHASE_UNLIMITED),

        ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
};

#pragma pack(push, 1)
struct AcceptCategoryNode
{
	BYTE			Category;
	BOOLEAN			Delimeter;
};
#pragma pack(pop)
#define ACCEPT_CATEGORY(category)		{ category, 1 },
#define ACCEPT_CATEGORY_EOR()			{ 0, 0 }

VOID	Handler_CG::RegisterHandler_ALL(PACKET_HANDLER_DELEGATOR pInstance)
{
    static const AcceptCategoryNode kAcceptFilters[] =
    {
        ACCEPT_CATEGORY(CG_CONNECTION   )
        ACCEPT_CATEGORY(CG_CHARINFO     )
        ACCEPT_CATEGORY(CG_MAP          )
        ACCEPT_CATEGORY(CG_SYNC         )
        ACCEPT_CATEGORY(CG_BATTLE       )
        ACCEPT_CATEGORY(CG_STYLE        )
        ACCEPT_CATEGORY(CG_SKILL        )
        ACCEPT_CATEGORY(CG_CONVERSATION )
        ACCEPT_CATEGORY(CG_ZONE         )
        ACCEPT_CATEGORY(CG_ITEM         )
        ACCEPT_CATEGORY(CG_TRADE        )
        ACCEPT_CATEGORY(CG_PARTY        )
        ACCEPT_CATEGORY(CG_TRIGGER      )
        ACCEPT_CATEGORY(CG_GM           )
        ACCEPT_CATEGORY(CG_ETC          )
        ACCEPT_CATEGORY(CG_STATUS       )
        ACCEPT_CATEGORY(CG_VENDOR       )
        ACCEPT_CATEGORY(CG_WAREHOUSE    )
        ACCEPT_CATEGORY(CG_PVP          )
        ACCEPT_CATEGORY(CG_SUMMON       )
        ACCEPT_CATEGORY(CG_GUILD        )
        ACCEPT_CATEGORY(CG_EVENT        )
        ACCEPT_CATEGORY(CG_QUEST        )
        ACCEPT_CATEGORY(CG_CRYPTOGRAPHY )
        ACCEPT_CATEGORY(CG_ARMORKIT     )
        ACCEPT_CATEGORY(CG_CHUNTING     )
        ACCEPT_CATEGORY(CG_AREACONQUEST )
        ACCEPT_CATEGORY(CG_MISSION      )
        ACCEPT_CATEGORY(CG_ZONE_EX      )
        ACCEPT_CATEGORY(CG_WAYPOINT     )
        ACCEPT_CATEGORY(CG_CHAO         )
        ACCEPT_CATEGORY(CG_SHOP         )
        ACCEPT_CATEGORY(CG_ZONE_EX      )
        ACCEPT_CATEGORY(CG_COLLECT      )
        //
        ACCEPT_CATEGORY_EOR()
    };
#ifdef _NA000000_CW_PROTOCOL_UNIFICATION
    static const AcceptCategoryNode kAcceptFilters_CW[] =
    {
        ACCEPT_CATEGORY(CW_CONNECTION   )
        ACCEPT_CATEGORY(CW_CHAT         )
        ACCEPT_CATEGORY(CW_GM           )
        ACCEPT_CATEGORY(CW_VIEWPORT     )
        ACCEPT_CATEGORY(CW_ARMORKIT     )
        ACCEPT_CATEGORY(CW_FRIEND       )
        ACCEPT_CATEGORY(CW_GUILD        )
        ACCEPT_CATEGORY(CW_CRYPTOGRAPHY )
        ACCEPT_CATEGORY(CW_COMMUNITY    )
        ACCEPT_CATEGORY(CW_REVENGE      )
        ACCEPT_CATEGORY(CW_MEMO         )
        ACCEPT_CATEGORY(CW_GROUP        )
        ACCEPT_CATEGORY(CW_LOTTO        )
        ACCEPT_CATEGORY(CW_PARTY        )
        //
        ACCEPT_CATEGORY_EOR()
    };
    BOOST_STATIC_ASSERT(CATEGORY_GENERATOR_065 == CG_MAX);
    BOOST_STATIC_ASSERT(CATEGORY_GENERATOR_165 == CG_MAX_PACK);
    for (const AcceptCategoryNode* node_it = kAcceptFilters; node_it->Delimeter; ++node_it) {
        pInstance->RegisterFowardHandlerGroupCG(UPT_USER, node_it->Category);
    };
    for (const AcceptCategoryNode* node_it = kAcceptFilters_CW; node_it->Delimeter; ++node_it) {
        pInstance->RegisterFowardHandlerGroupCW(UPT_USER, node_it->Category);
    };
#else
    BOOST_STATIC_ASSERT(CATEGORY_GENERATOR_036 == CG_MAX);
    BOOST_STATIC_ASSERT(CATEGORY_GENERATOR_136 == CG_MAX_PACK);
    for (const AcceptCategoryNode* node_it = kAcceptFilters; node_it->Delimeter; ++node_it) {
        pInstance->RegisterFowardHandlerGroupCG(UPT_USER, node_it->Category);
    };
#endif

	pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo());
	pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo_CHARINFO());
	pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo_SYNC());
	pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo_ZONE());
	pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo_PARTY());
	pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo_GM());
	pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo_SECURE());
	pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo_ETC());
    // added by _KR_0_20091021_FIRENDINVITATION_EXTEND //���� �޼ҵ� ���
    pInstance->RegisterHandlerCA_<UPT_USER>(GetHandlerInfo_Event());
#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    // Block - ���� �̰͵� ���� ����? �ƴ� ���ܻ����� ������ �̴�� ����...
    static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
    {
        { MAKEWORD(CG_SHOP, CG_SHOP_SEND_GIFT_RELAY_SYN), 
          PHASE_UNLIMITED, (PH_fnHandler)&PacketHandlerShared::OnUSER_EMPTY_HANDLER },
        ADD_HANDLER_INFO_EOR(),
    };
    pInstance->RegisterHandlerCA_<UPT_USER>(s_pHandlerInfo);
#endif
#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef __NA000000_081104_REGISTERED_PROTOCOL_NAMES__
	pInstance->RegisterHandlerNames();
#endif
}

//
//
//
//��������������������������������������������������������������������������������������������������������������������������������������������������
//
//
//

#include <PacketStruct_AG.h>
#include <PacketStruct_AD.h>
#include <PacketStruct_CG.h>
#include <PacketStruct_AZ.h>
#include <PacketStruct_AW.h>
#include <PacketStruct_AM.h>
#include <Protocol_AW.h>
#include <ResultCode.h>
#include <SolarLog.h>
#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>
#include <Zone/LobbyRoom.h>
#include <Zone/HuntingRoom.h>
#include <Zone/MissionRoom.h>
#include <Zone/CharSelect.h>
#include <Zone/PVPRoom.h>
#include <Zone/Village.h>
#include <TimeCheck/HeartbeatManager.h>
#include <AgentServer.h>
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>
#include <Zone/ZoneManager.h>
#include <Party/PartyManager.h>
#include "MapInfoParser.h"
#include <Zone/CHuntingRoom.h>
#include <Zone/HuntingField.h>
#include <Zone/PortalManager.h>

#include <ProhibitionWordParser.h>

#include <CurseTowerTimeParser.h>

#include <SSQ/SSQInfoParser.h>
#include <Zone/WarControlManager.h>

//��������������������������������������������������������������������������������������������������������������������������������������������������
//
//
//

// (WAVERIX) (REMARK) <070318>
//   Client handler���� ���캼 �ð��� ������.

HANDLER_CG_IMPL(CG_PREPARE_WORLD_CONNECT_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    ASSERT(!"does not support anymore");
    return;
#else
	MSG_CG_PREPARE_WORLD_CONNECT_SYN* pRecvMsg = (MSG_CG_PREPARE_WORLD_CONNECT_SYN *)pMsg;

	// ���� ������ ���� �������� �˻� �ʿ�
	WorldServerSession* pWorldServer = AgentServer::GetInstance()->SessionWorldServer();
	if(!(pWorldServer && pWorldServer->IsConnected()))
	{
		MSG_CG_PREPARE_WORLD_CONNECT_NAK nmsg;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	// ���� �����̹Ƿ� ���� ��Ŷ ����
	MSG_AW_USER_RESTORE_CMD	cmsg;

	cmsg.dwAuthUserID = pUser->GetAuthID();
	cmsg.dwUserGUID = pUser->GetUserGUID();
	cmsg.dwZoneKey = pUser->GetZoneKey();
	cmsg.byZoneType = pUser->GetZoneType();
	cmsg.m_dwKey = pUser->GetUserKey();
	cmsg.byGMGrade = pUser->GetSts();
    cmsg.m_CharClass = pUser->GetCharClass();

	if(cmsg.byZoneType != eZONETYPE_CHARSELECT)
	{
		cmsg.CharGuid = pUser->GetSelectedCharGuid();
		memset(cmsg.tszCharName, 0, sizeof(TCHAR)*MAX_CHARNAME_LENGTH);
		_tcsncpy(cmsg.tszCharName, pUser->GetSelectedCharName(), MAX_CHARNAME_LENGTH);
	}
	else
	{
		cmsg.CharGuid = 0;
		memset(cmsg.tszCharName, 0, sizeof(TCHAR)*MAX_CHARNAME_LENGTH);
	}

	// ��Ƽ ���ԵǾ� �ִ� ��� ��Ƽ ���� ����
	if(pUser->GetPlayerKey())
	{
		cmsg.wPartyKey = pUser->GetPartyState().GetPartyKey();
	}
	else
	{
		cmsg.wPartyKey = 0;
	}

	cmsg.dwPCRoomID = pUser->GetReservedValue();	// PC������ �ƴ��� ����
	cmsg.m_GuildGuid = pUser->GetSelectedCharGuildGuid();
    CHECK_ID_LENGTH_BUFFER(cmsg.ptszAccountID);
    _tcsncpy(cmsg.ptszAccountID, pUser->GetUserID(), _countof(cmsg.ptszAccountID));
    cmsg.ptszAccountID[_countof(cmsg.ptszAccountID) - 1] = '\0';
	memcpy(cmsg.pszClientIP, pUser->GetClientIP(), IP_MAX_LEN);
	cmsg.m_CharLevel = pUser->GetCharLV();
	pWorldServer->SendPacket(&cmsg, sizeof(MSG_AW_USER_RESTORE_CMD));

	SUNLOG(eFULL_LOG, "[Guid:%u] User Restore To World[ZT:%u][ZK:%u]", pUser->GetUserGUID(), cmsg.byZoneType, cmsg.dwZoneKey);
	/*
	// �������Դ� ���� ������ ����� �˷����~
	MSG_CG_PREPARE_WORLD_CONNECT_ACK amsg;
	amsg.m_byCategory = CG_CONNECTION;
	amsg.m_byProtocol = CG_PREPARE_WORLD_CONNECT_ACK;

	if(g_AgentServer.GetServerEnv()->bGetServerInfoFromFile == 1)
	{
	// ���Ͽ��� ���� �ּ�
	strncpy(amsg.szWorldServerIP, g_AgentServer.GetServerEnv()->pszWorldServerClientIP, IP_MAX_LEN);
	amsg.wWorldServerPort = g_AgentServer.GetServerEnv()->wWorldServerClientPort;
	}
	else
	{
	strncpy(amsg.szWorldServerIP, g_AgentServer.GetWorldServerClientIP().c_str(), IP_MAX_LEN);
	amsg.wWorldServerPort = g_AgentServer.GetWorldServerClientPort();
	}

	pUser->SendPacket(&amsg, sizeof(amsg));
	MessageOut(eFULL_LOG, "[Guid:%u] User Restore Msg To Client", pUser->GetUserGUID());
	*/

	/*
	MSG_AW_PREPARE_NEW_USER_SYN msg;
	msg.m_byCategory	= AW_CONNECTION;
	msg.m_byProtocol	= AW_PREPARE_NEW_USER_SYN;
	msg.m_dwKey			= pUser->GetUserKey();
	msg.dwAuthUserID	= pUser->GetAuthID();
	msg.dwUserGuid		= pUser->GetUserGUID();
	msg.byUserStatus	= pUser->GetZoneType();

	strncpy(msg.szClientIP, pUser->GetClientIP(), IP_MAX_LEN);
	if(strcmp(pUser->>GetSelectedCharName(), "") == 0)
	{
	strcpy(msg.szCharName, "");
	}
	else
	{
	strncpy(msg.szCharName, pUser->GetSelectedCharName(), MAX_CHARNAME_LENGTH);
	}

	//printf("Ŭ��κ��� CG_PREPARE_WORLD_CONNECT_SYN ����(%d)\n", pUser->GetUserKey());

	if(!pUser->SendToWorldServer(&msg, sizeof(msg)))
	{
	MSG_CG_PREPARE_WORLD_CONNECT_NAK nmsg;
	pUser->SendPacket(&nmsg, sizeof(nmsg));
	return;
	}
	*/
#endif //if !defined(_NA002676_WORLD_SERVER_RENEWAL)
}

HANDLER_CG_IMPL(CG_SYNC_PLAYER_ENTER_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_SYNC_PLAYER_ENTER_SYN* pRecvMsg = (MSG_CG_SYNC_PLAYER_ENTER_SYN *)pMsg;

	BOOL bIsValidSize = (BOOL) (sizeof(MSG_CG_SYNC_PLAYER_ENTER_SYN) == wSize);
	if(!bIsValidSize)
	{
		SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_SYNC_PLAYER_ENTER_SYN"));
		return;
	}

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_SYNC_PLAYER_ENTER_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	eZONE_STATE zsSTATE = pUser->GetZoneState();

	//	1)	ePRS_BEFORE_ENTER_LOBBY | ePRS_AT_LOBBY �� ��츦 �����ϰ� PLAYER_ENTER_SYN������ 
	//		STATUS::TRANSACTION �����̸�, QUERYBUILTIN�� Transaction�� �ذ�� ���¿��� �Ѵ�.
	//	2)	�� ������ ���� ���θ� ������ �׳� ��� ��Ų��.
	if(ePRS_BEFORE_ENTER_LOBBY != zsSTATE)
	{
		SecurePackNode4User& rSECURE = pUser->GetSecureNode();
		sSECURE_FLAG& rSECURE_FLAG = rSECURE.SECURE_FLAG();
		if(rSECURE_FLAG.QUERYBUILTIN.IsTransaction())
		{
			// Notify to MASTER
			SECURELOG(SECURELOG_USERINFO(pUser, eABUSE_LOGIC_VIOLATION, eABUSE_SUB_LOGIC_VIOLATION_ASSERTION_FLOW)
				, "Assertion Flow Violation : Transaction Tic-Toc { %u }", zsSTATE);
		}
	}

	switch(zsSTATE)
	{
	case ePRS_BEFORE_ENTER_VILLAGE:
	case ePRS_BEFORE_ENTER_FIELD:
	case ePRS_BEFORE_ENTER_LOBBY:
	case ePRS_BEFORE_ENTER_HUNTING:
	case ePRS_BEFORE_ENTER_MISSION:
	case ePRS_BEFORE_ENTER_CHUNTING:
	case ePRS_BEFORE_ENTER_PVP:
	case ePRS_BEFORE_ENTER_INSTNACE:
    case ePRS_BEFORE_ENTER_DOMINATION_FIELD:
    case ePRS_BEFORE_ENTER_SPA_FIELD:
        {
            // (f100623.1L) changed a data control by _NA000000_100623_CHECK_WMO_CHECKSUM_
            MSG_AG_SYNC_PLAYER_ENTER_SYN msg;
            ZeroMemory(&msg.checksum_list_, sizeof(msg.checksum_list_));
            msg.checksum_list_.checksum_map = pRecvMsg->m_dwCheckSum;
            msg.checksum_list_.checksum_wmo = pRecvMsg->checksum_wmo_;
            // NOTE:f100916.2L, to support the agent server dummy client
            msg.is_dummy_client_test_ = USING_DUMMY_CLIENT;
            msg.tail_guard_ = -1;
            // ���� ��Ŷ ����
            pUser->SendToLinkedServer(&msg, sizeof(msg));
        }
        break;
	case ePRS_AT_VILLAGE:
	case ePRS_AT_LOBBY:
	case ePRS_AT_HUNTING:
	case ePRS_AT_MISSION:
	case ePRS_AT_PVP:
	case ePRS_AT_CHUNTING:
	case ePRS_AT_INSTANCE:
    case ePRS_AT_DOMINATION_FIELD:
    case ePRS_AT_SPA_FIELD:
        {
            // (f100623.1L) changed a data control by _NA000000_100623_CHECK_WMO_CHECKSUM_
            // migrate to an other internal field in a same zone
            MSG_AG_SYNC_PLAYER_WARP_SYN msg;
            ZeroMemory(&msg.checksum_list_, sizeof(msg.checksum_list_));
            msg.checksum_list_.checksum_map = pRecvMsg->m_dwCheckSum;
            msg.checksum_list_.checksum_wmo = pRecvMsg->checksum_wmo_;
            // NOTE:f100916.2L, to support the agent server dummy client
            msg.is_dummy_client_test_ = USING_DUMMY_CLIENT;
            msg.tail_guard_ = -1;
            // ���� ��Ŷ ����
            pUser->SendToLinkedServer(&msg, sizeof(msg));
        }
        break;
	default:
		{
			MSG_CG_SYNC_PLAYER_ENTER_NAK nmsg;
			nmsg.m_dwErrorCode = 8787878;
			pUser->SendPacket(&nmsg, sizeof(nmsg));
			SUNLOG(eCRITICAL_LOG, "MSG_CG_SYNC_PLAYER_ENTER_SYN:[��%u������.]", pUser->GetZoneState());
			return;
		}
	}

    pUser->GetSecureNode().EnableHeartbeats(true);
}

HANDLER_CG_IMPL(CG_CONNECTION_REENTERSERVER_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CONNECTION_REENTERSERVER_SYN* pRecvMsg = (MSG_CG_CONNECTION_REENTERSERVER_SYN *)pMsg;

	BOOL bIsValidSize = (BOOL) ((WORD)sizeof(MSG_CG_CONNECTION_REENTERSERVER_SYN) == wSize);
	if(!bIsValidSize)
	{
		SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] CG_CONNECTION_REENTERSERVER_SYN"));
		return;
	}

    pUser->GetSecureNode().EnableHeartbeats(true);

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_CONNECTION_ENTERSERVER_NAK nmsg;
		nmsg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	pUser->BeginTransaction(TR_CHARACTER_LIST);

	MSG_AD_CHARINFO_CHARLISTREQ_SYN msg;
	msg.m_dwKey		= pUser->GetUserKey();
	msg.m_UserGuid	= pUser->GetUserGUID();
	msg.m_bIsPCRoom = !!pUser->GetReservedValue(); //< pc�� �������� üũ, (WAVERIX) (090820) (CHANGES)
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	msg.m_LoginTime = pUser->GetLoginTime_t();
	if((pUser->GetAge() >= 18) && (pUser->GetRealNameAuth() == 1))	
		msg.m_bFatigueUser = FALSE;
	else
		msg.m_bFatigueUser = TRUE;
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    msg.gamechu_usn_ = pUser->GetGameChuUSN();    
#endif
#ifdef _NA_20100307_BETAKEY_SYSTEM    
    msg.betakey_ = pUser->GetBetaKeyInfo()->GetBetaKey();
#endif

	msg.m_bBackToSelect = true;
	msg.m_ChannelID = AgentServer::GetInstance()->GetKey().GetChannelID();

	if(FALSE == pUser->SendToDBPServer(&msg, sizeof(msg)))
	{
		MSG_CG_CONNECTION_ENTERSERVER_NAK nmsg;
		nmsg.m_dwErrorCode = 77;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		SUNLOG(eFULL_LOG,  "[HANDLER_FROMCLIENT_IMPL(CG_CONNECTION_REENTERSERVER_SYN)] [U:%d]ĳ���͸���Ʈ���޾ƿ������ز��´�.77", pUser->GetUserKey());
		pUser->SetDisconnectCode(RC::RC_DISCONNECT_DBP_CONNETION_ERROR);
		pUser->DisconnectUser();

		pUser->EndTransaction();
		return;
	}
}

HANDLER_CG_IMPL(CG_CHARINFO_CREATE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CHARINFO_CREATE_SYN* pRecvMsg = (MSG_CG_CHARINFO_CREATE_SYN *)pMsg;

	BOOL bIsValidSize = (BOOL) (sizeof(MSG_CG_CHARINFO_CREATE_SYN) == wSize);
	if(!bIsValidSize)
	{
		SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] CHAR CREATE"));
		return;
	}
    // CHANGES: f110825.1L, restored to the original filter order.
    // the BetaKeyImpl that is logically the contents filter is not the basic object control logic.
    if (pUser->IsBeginTransaction())
    {
        MSG_CG_CHARINFO_CREATE_NAK msg_nak;
        msg_nak.m_dwErrorCode  = RC::RC_CHAR_CREATE_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
    // CHANGES: f110825.1L, prevent the unexpected packet flow
    if (pUser->GetZoneState() != ePRS_AT_CHARSELECT)
    {
        MSG_CG_CHARINFO_CREATE_NAK msg_nak;
        msg_nak.m_dwErrorCode = RC::RC_CHAR_CREATE_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
#if defined(_NA_20100307_BETAKEY_SYSTEM) && defined(_NA_002050_20110216_ADD_GENDER)    
    if(!BetaKeyImpl::CanCreateCharacter(pUser->GetBetaKeyInfo(), pRecvMsg->m_byClass, pRecvMsg->gender))
    {
        MSG_CG_CHARINFO_CREATE_NAK nmsg;
        nmsg.m_dwErrorCode = RC::RC_CHAR_CREATE_CANNOT_BETAKEY;
        pUser->SendPacket(&nmsg, sizeof(nmsg));
        return;
    }    
#endif

	const DWORD ACCEPTABLE_CHARMASK = (1<<eCHAR_BERSERKER) | 
        (1<<eCHAR_SHADOW) | 
        (1<<eCHAR_DRAGON) | 
        (1<<eCHAR_VALKYRIE) | 
        (1<<eCHAR_ELEMENTALIST) | 
        (1 << eCHAR_HELLROID) | //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        (1 << eCHAR_WITCHBLADE) |
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        (1 << eCHAR_MYSTIC); // _NA_004965_20120613_NEW_CHARACTER_MYSTIC

	DWORD dwClassBit = (1<<(DWORD)pRecvMsg->m_byClass);

	if(!(ACCEPTABLE_CHARMASK & dwClassBit)
		|| pUser->GetState() < CHARACTER_LIST_STATE 
		|| pUser->GetNumberOfCreatedChars() >= MAX_CHARACTER_SLOT_NUM)
	{
		MSG_CG_CHARINFO_CREATE_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_CREATE_INVALID_PARAM;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	util::cSTRING_REF strCharNameRD(pRecvMsg->m_pszCharName, sizeof(pRecvMsg->m_pszCharName));
	util::cSTRING_REF_RW_WITH_BUFFER<sizeof(TCHAR)*(MAX_CHARNAME_LENGTH+1)> strCharNameRW;

	strCharNameRW.CopyFrom(strCharNameRD);
	strCharNameRW.MakeSafeString();

	// �ѱ�2��, ���� 2���� �̻�
	// �����ڵ��϶� ����!!!! �ѱ���2�ڴ� 2�� �ν�
	if(strCharNameRW.GetSTRING_LENGTH() < 4)
	{
		MSG_CG_CHARINFO_CREATE_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_CREATE_MINIMUM_CHARACTER;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	//20070131_Ư������ ��뿡 ���� NAK
#ifdef _NA_0_20100929_LANGUAGE_OF_FILTER
    BYTE Language = ServerOptionParserEx::Instance()->GetServerOption().m_byLanguageOfFilter;
    if(!ProhibitionWordParser::Instance()->VerifyCharName(strCharNameRW, Language))
#else
    if(!ProhibitionWordParser::Instance()->VerifyCharName(strCharNameRW))
#endif
	{		
		//Ŭ���̾�Ʈ �޽��� �ʿ�
		MSG_CG_CHARINFO_CREATE_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_CREATE_CHARNAME_BAN_WORD;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}	

	MSG_AD_CHARINFO_CREATE_SYN msg;
	msg.m_dwKey			= pUser->GetUserKey();
	msg.m_UserGuid		= pUser->GetUserGUID();
	_tcsncpy(msg.m_tszUserID, pUser->GetUserID(), ID_MAX_LEN);
	msg.m_tszUserID[ID_MAX_LEN]='\0';
	msg.m_byClass		= pRecvMsg->m_byClass;
	_tcsncpy(msg.m_tszCharName, strCharNameRW.GetBUFFER(), MAX_CHARNAME_LENGTH);
	msg.m_tszCharName[MAX_CHARNAME_LENGTH]='\0';
	msg.m_byHeight		= pRecvMsg->m_byHeight;
	msg.m_byFace		= pRecvMsg->m_byFace;
	msg.m_byHair		= pRecvMsg->m_byHair;
#ifdef _NA_002050_20110216_ADD_GENDER
    msg.gender = pRecvMsg->gender;
#endif
	if(FALSE == pUser->SendToDBPServer(&msg, sizeof(msg)))
	{
		MSG_CG_CHARINFO_CREATE_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_CREATE_DISCONNECTED_DBPROXY;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		pUser->EndTransaction();
		return;
	}

	pUser->BeginTransaction(TR_CHARACTER_CREATION);
}

HANDLER_CG_IMPL(CG_CHARINFO_DESTROY_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CHARINFO_DESTROY_SYN* pRecvMsg = (MSG_CG_CHARINFO_DESTROY_SYN *)pMsg;

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_CHARINFO_DESTROY_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_DESTROY_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}
    // CHANGES: f110825.1L, prevent the unexpected packet flow
    if (pUser->GetZoneState() != ePRS_AT_CHARSELECT)
    {
        MSG_CG_CHARINFO_DESTROY_NAK msg_nak;
        msg_nak.m_dwErrorCode = RC::RC_CHAR_DESTROY_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };

	BYTE CharIndex = pRecvMsg->m_bySelectedCharSlotIndex;
	if(pUser->GetState() < CHARACTER_LIST_STATE ||
		pUser->GetNumberOfCreatedChars() == 0 ||
		CharIndex >= MAX_CHARACTER_SLOT_NUM ||
		!pUser->IsValidSlot(CharIndex))
	{
		MSG_CG_CHARINFO_DESTROY_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_DESTROY_INVALID_PARAM;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	if(pUser->IsCharDelete(CharIndex))
	{
		MSG_CG_CHARINFO_DESTROY_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_DESTROY_DESTROIED;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

#if !(defined(_USA) || defined(_GLOBAL) || defined(_RUSSIA))
    // NorthA, GSP version�� �ֹι�ȣ�� ������� �ʴ´�.
	// �ֹι�ȣ üũ
	if(!pUser->IsEqualLastSSN(pRecvMsg->m_LastSocialSecurityNumber))
	{
		MSG_CG_CHARINFO_DESTROY_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_DESTROY_INCORRECT_SSN;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}
#endif

	MSG_AD_CHARINFO_DESTROY_SYN msg;
	msg.m_dwKey = pUser->GetUserKey();
	msg.m_CharGuid	= pUser->GetCharGUID(CharIndex);
	if(FALSE == pUser->SendToDBPServer(&msg, sizeof(msg)))
	{
		MSG_CG_CHARINFO_DESTROY_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_DESTROY_DISCONNECTED_DBPROXY;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}
	pUser->BeginTransaction(TR_CHARACTER_DESTRUCTION);
}

HANDLER_CG_IMPL(CG_CHARINFO_RECOVER_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CHARINFO_RECOVER_SYN* pRecvMsg = (MSG_CG_CHARINFO_RECOVER_SYN *)pMsg;

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_CHARINFO_RECOVER_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_RECOVER_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}
    // CHANGES: f110825.1L, prevent the unexpected packet flow
    if (pUser->GetZoneState() != ePRS_AT_CHARSELECT)
    {
        MSG_CG_CHARINFO_RECOVER_NAK msg_nak;
        msg_nak.m_dwErrorCode = RC::RC_CHAR_RECOVER_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&msg_nak, sizeof(msg_nak));
        return;
    };
	BYTE CharIndex = pRecvMsg->m_bySelectedCharSlotIndex;
	if(pUser->GetState() < CHARACTER_LIST_STATE ||
		CharIndex >= MAX_CHARACTER_SLOT_NUM)
	{
		MSG_CG_CHARINFO_RECOVER_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_RECOVER_INVALID_PARAM;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	if(FALSE == pUser->IsCharDelete(CharIndex))
	{
		MSG_CG_CHARINFO_RECOVER_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_RECOVER_NOTDESTROYED;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	MSG_AD_CHARINFO_RECOVER_SYN msg;
	msg.m_dwKey = pUser->GetUserKey();
	msg.m_CharGuid	= pUser->GetCharGUID(CharIndex);
	if(FALSE == pUser->SendToDBPServer(&msg, sizeof(msg)))
	{
		MSG_CG_CHARINFO_RECOVER_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_RECOVER_DISCONNECTED_DBPROXY;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	pUser->BeginTransaction(TR_CHARACTER_RECOVERY);
}

#ifdef __NA_001044_ADD_CUSTOM_UI
HANDLER_CG_IMPL(CG_CHARINFO_CHARACTERID_CHECK_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CHARINFO_CHARACTERID_CHECK_SYN* pRecvMsg = (MSG_CG_CHARINFO_CHARACTERID_CHECK_SYN *)pMsg;

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_CHARINFO_CHARACTERID_CHECK_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_IDCHECK_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	MSG_AD_CHARINFO_CHECK_CHARACTERID_SYN msg_syn;
	msg_syn.m_dwKey = pUser->GetUserKey();
	strncpy(msg_syn.m_szCharName, pRecvMsg->m_szCharName, _countof(msg_syn.m_szCharName));
	msg_syn.m_szCharName[_countof(msg_syn.m_szCharName) - 1]= '\0';
	if(FALSE == pUser->SendToDBPServer(&msg_syn, sizeof(msg_syn)))
	{
		MSG_CG_CHARINFO_CHARACTERID_CHECK_NAK nmsg;
		nmsg.m_dwErrorCode	= RC::RC_CHAR_IDCHECK_DISCONNECTED_DBPROXY;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	pUser->BeginTransaction(TR_CHARACTER_IDCHECK);
}
#endif

HANDLER_CG_IMPL(CG_CONNECTION_BACKTOCHARSELECT_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CONNECTION_BACKTOCHARSELECT_SYN* pRecvMsg = (MSG_CG_CONNECTION_BACKTOCHARSELECT_SYN *)pMsg;

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_CONNECTION_BACKTOCHARSELECT_NAK nmsg;
		nmsg.m_dwErrorCode = 129;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

    // lobby, charselect ���´� ����.
    const DWORD blockFilter = (ePRS_AT_CHARSELECT | ePRS_AT_LOBBY) & ~ePRS_AT_ROOM;

    // (WAVERIX) (090518) (BUG-FIX) ĳ���� ����â ���ư��� ���� �˻� �̽� ����
    eZONE_STATE zoneState = pUser->GetZoneState();
    //if(!(pUser->GetZoneState() & ePRS_AT_ZONE))
    if((zoneState & ePRS_AT_ZONE) != ePRS_AT_ZONE || (zoneState & blockFilter) != 0)
	{
		MSG_CG_CONNECTION_BACKTOCHARSELECT_NAK nmsg;
		nmsg.m_dwErrorCode = 130;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

#ifdef _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    PartyRoomFinder* room_finder = party_manager->GetPartyRoomFinder();
    if (room_finder != NULL)
    {
        room_finder->DeleteAllCharGuidFromMissionList(pUser->GetSelectedCharGuid());
    }
#endif  // _NA_004707_20120416_1202_DELETE_RESERVATION_ROOM_LIST
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager->IsValidReservationMember(pUser->GetUserKey()))
    {
        BattleGroundMemberInfo* const member = \
            battle_ground_manager->FindBattleGroundMember(pUser->GetUserKey());
        if (member)
        {
            if (member->group_key == 0)
            {
                battle_ground_manager->RemoveMemberStandbyGroup(member);
                battle_ground_manager->RemoveReservationMember(pUser->GetUserKey());
            }
            else
            {
                battle_ground_manager->LeaveRoom(pUser, pUser->GetTRRoomKey());
            }
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	ZoneInterface* pZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
	if(!pZone) return;

	// �̼� ������ �ǵ������� ������ �ϸ� ������ ����
	if(pZone->GetUserNum() >= 2 && pZone->GetType() == eZONETYPE_MISSION)
	{
		MSG_AG_STATUS_DECREASE_DURA_CMD msg;
		pUser->SendToLinkedServer(&msg, sizeof(msg));
	}

	CharSelect* pCharSelect = g_ZoneManager.FindCharSelect(1);
	DWORD ToLinkServerIndex = pCharSelect != NULL ? pCharSelect->GetLinkedServerIndex() : 0;

	pZone->LeaveUser(pUser, eSEND_SYN, eZONETYPE_CHARSELECT, ToLinkServerIndex);

	pUser->BeginTransaction(TR_BACKTOSELECT);

	//	status chage (STAT_CRYPED && !STAT_CRYPTING) -> USERINIT
	//	������ ���� �ʰ� ���� ����.
	//	�Ǵ��ϱ� �ָ��� ��Ȳ�� �߻��Ѵ�. �����̰� �����ϴ� ���� ������ ��� ���°� �������� �ʴ´�.
	//	�ϴ� ���⿡ ���´ٴ� ���� �ּ��� ���������� �� ��Ȳ�̹Ƿ�, BUILTIN QUERY���´� �ƴϴ�.
	SecurePackNode4User& rSECURE = pUser->GetSecureNode();
	sSECURE_FLAG& rSECURE_FLAG = rSECURE.SECURE_FLAG();
	if(!rSECURE_FLAG.STATUS.IsTransaction())	// �̹� Ʈ����� ���¿� ���� �� �ִ°�?
		rSECURE_FLAG.STATUS.TransEnter();
}

HANDLER_CG_IMPL(CG_CONNECTION_NOTIFY_LOGOUT_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CONNECTION_NOTIFY_LOGOUT_SYN *pRecvMsg = (MSG_CG_CONNECTION_NOTIFY_LOGOUT_SYN*)pMsg;

	// Ŭ�� �����ư�� ������ �������� �õ��Ѵ�.
	pUser->SetLogOutWait();

	ZoneInterface* pZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
	if(!pZone) return;

	// �̼� ������ �ǵ������� ������ �ϸ� ������ ����
	if(pZone->GetUserNum() >= 2 && pZone->GetType() == eZONETYPE_MISSION)
	{
		MSG_AG_STATUS_DECREASE_DURA_CMD msg;
		pUser->SendToLinkedServer(&msg, sizeof(msg));
	}
}

HANDLER_CG_IMPL(CG_CONNECTION_ENTERVILLAGEREQ_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CONNECTION_ENTERVILLAGEREQ_SYN* pRecvMsg = (MSG_CG_CONNECTION_ENTERVILLAGEREQ_SYN *)pMsg;

	BOOL bIsValidSize = (BOOL) (sizeof(MSG_CG_CONNECTION_ENTERVILLAGEREQ_SYN) == wSize);
	if(!bIsValidSize)
	{
		SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] ENTERVILLAGEREQ"));
		return;
	}

	MSG_CG_CONNECTION_ENTERVILLAGEREQ_NAK nmsg;

	if(pUser->IsBeginTransaction())
	{
		nmsg.m_dwErrorCode	= RC::RC_CONNECTION_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	BYTE CharIndex = pRecvMsg->m_bySelectedCharIndex;
	if(pUser->GetState() < CHARACTER_LIST_STATE
		|| CharIndex >= MAX_CHARACTER_SLOT_NUM
		|| !pUser->IsValidSlot(CharIndex)
		)
	{
		nmsg.m_dwErrorCode	= RC::RC_CHARSELECT_OUTOFRANGEINDEX;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	if(pUser->IsCharDelete(CharIndex))
	{
		nmsg.m_dwErrorCode	= RC::RC_CONNECTION_DEL_CHAR;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	// ���õ� ĳ���� &
	// ��ũ �ʵ� ����
	if(!ServerSessionManager::Instance()->GetFieldServer())
	{
		nmsg.m_dwErrorCode	= RC::RC_CONNECTION_DONOTEXIST_LINKSERVER;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	if(pUser->GetZoneState() != ePRS_AT_CHARSELECT)
	{
		nmsg.m_dwErrorCode	= RC::RC_CONNECTION_INVALID_CHAR_STATE;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	// ����׿�!!!!!
	User* pPreUser = UserManager::Instance()->FindUserForCharName(pUser->GetSelectedCharName());
	if(pPreUser)
	{
		SUNLOG(eFULL_LOG,  "[HANDLER_FROMCLIENT_IMPL(CG_CONNECTION_ENTERVILLAGEREQ_SYN)] �� ĳ���� �̸�(%s)�������� �����Ѵ�.", pUser->GetSelectedCharName());
		pPreUser->SetDisconnectCode(RC::RC_DISCONNECT_DUPLICATE_CHARNAME);
		pPreUser->DisconnectUser();

		nmsg.m_dwErrorCode	= RC::RC_CONNECTION_DUPLICATED_CHAR_NAME;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		pUser->SetDisconnectCode(RC::RC_DISCONNECT_DUPLICATE_CHARNAME);
		pUser->DisconnectUser();
		return;
	}

	pUser->SelectChar(CharIndex);

	// ����� �������� üũ
	if(pUser->GetSelectedCharState() == DBCHAR_STATE_CHAR_BLOCK)
	{
		nmsg.m_dwErrorCode	= RC::RC_CONNECTION_CHAR_BLOCK;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	//������ �̵�? �ʵ�� �̵�..., �̵��� �� ����..
	eZONETYPE zoneType;
	sMAPINFO* pMapInfo = (sMAPINFO*)MapInfoParser::Instance()->FindMapInfo(pUser->GetSelectedCharMapCode());
	if(!pMapInfo)
	{
		SUNLOG(eFULL_LOG,  "[CG_CONNECTION_ENTERVILLAGEREQ_SYN]DB�� �ش���� �������� �ʽ��ϴ�.(%u)", pUser->GetSelectedCharMapCode());

		nmsg.m_dwErrorCode	= RC::RC_CHARSELECT_MAPCODE_WRONG;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

    MSG_GA_AUTH_PC_NAME_NTF loginNTF;
    loginNTF.m_dwAuthUserID = pUser->GetAuthID();

    CHECK_ID_LENGTH_BUFFER(loginNTF.m_szAccountID);
    strncpy(loginNTF.m_szAccountID, pUser->GetUserID(), _countof(loginNTF.m_szAccountID));
    loginNTF.m_szAccountID[_countof(loginNTF.m_szAccountID) - 1] = '\0';
    // NOTE: loginNTF.m_szPCName, not null-terminated string
    strncpy(loginNTF.m_szPCName, pUser->GetSelectedCharName(), _countof(loginNTF.m_szPCName));
  #ifdef _NA000000_091005_EXTENDED_LOG_
    loginNTF.m_CharClass = pUser->GetSelectedCharClass();
    loginNTF.m_CharLevel = pUser->GetSelectedCharLV();
  #endif
	AgentServer::GetInstance()->SendToAuthAgentServer((BYTE*)&loginNTF, (WORD)sizeof(loginNTF));

	// WorldServer�� ����
	MSG_AW_USER_SET_CHARNAME_SYN wmsg;
	wmsg.m_byCategory	= AW_USER_STATUS;
	wmsg.m_byProtocol	= AW_USER_SET_CHARNAME_SYN;
	wmsg.m_dwKey		= pUser->GetUserKey();
	wmsg.dwCharGuid		= pUser->GetCharGUID(CharIndex);
    wmsg.m_CharClass    = pUser->GetCharClass();
#ifdef _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY
    wmsg.gender         = pUser->GetSelectedCharGender();
#endif // _NA_006642_20120416_COMMUNITY_INFO_BUG_MODIFY

    strncpy(wmsg.szCharName, pUser->GetSelectedCharName(), _countof(wmsg.szCharName));
    wmsg.szCharName[_countof(wmsg.szCharName) - 1] = '\0';
	wmsg.m_GuildGuid		= pUser->GetSelectedCharGuildGuid();
	wmsg.m_CharLevel	= pUser->GetCharLV(CharIndex);
	pUser->SendToWorldServer(&wmsg, sizeof(wmsg));

    const MAPCODE entry_map_code = pUser->GetSelectedCharMapCode();

	ZoneInterface* pZone = (ZoneInterface*)g_ZoneManager.FindStaticZone(entry_map_code);
	if(!pZone)
	{	
		SUNLOG(eCRITICAL_LOG, "�����̳��ʵ尡�����ϴ�.[%d]", pUser->GetSelectedCharMapCode());
		return;
	}

    UserManager::Instance()->AddUserForSelectedCharInfoHashes(pUser);

	zoneType = (eZONETYPE)pMapInfo->GetZoneType4SharedCode();

	CharSelect* pCharSelect = g_ZoneManager.FindCharSelect(pUser->GetZoneKey());

	pCharSelect->LeaveUser(pUser, eSEND_NONE, zoneType, pZone->GetLinkedServerIndex());

    // NOTE: f110509.2L, the first enterable field that called by map matched with one field code
    // in the field server of the custom logic. so that the map code equals the field code
    // like the below logic.
    MAPCODE enter_field_code = entry_map_code;
#ifdef __NA002642_TUTORIAL_LOAD_BALANCE_SUPPORT
    // NOTE: f110509.2L, waverix, fields of the field server has been used the 1 to 1 matching method.
    // to exactly support load balancing for the tutorial map there are based on the custom logic
    // is difficult, therefore waverix use same entry field code in tutorial fields
    // such as mission sub-fields. the 20206(tutorial) map code is only used and that it has
    // a number of fields like 20206, 20207, 20208, and that.
    // In conculusion, from the client and agent viewpoint, there only control one map code, and
    // the field server control multi-fields in the map like a battle server.
    if (pMapInfo->byMKind == eZONETYPE_TUTORIAL_FIELD) {
        MapInfoParser* map_parser = MapInfoParser::Instance();
        enter_field_code = map_parser->GetRandomFieldCodeInZone(entry_map_code);
    };
#endif
	pUser->BeginTransaction(TR_JOIN_VILLAGE);
	pZone->ReadyUser(pUser, eSEND_SYN, enter_field_code, 0, NULL, true);

	pUser->BeginCharLoginTime();
	//ĳ���� ���� �α�
	GAMELOG->ConnectChar(pUser);
}

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
HANDLER_CG_IMPL(CG_CONNECTION_SERVER_SELECT_REQUEST_SYN)
{
    __TOUCHED((pMsg, wSize));
    const MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_SYN* pRecvMsg =
        static_cast<MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_SYN*>(pMsg);

    MSG_CG_CONNECTION_SERVER_SELECT_REQUEST_ACK msgACK;
    BOOLEAN bValidState = TRUE;
    BOOLEAN bStartTrans = pRecvMsg->m_bQueryType == pRecvMsg->eQuery_StartTransaction;
    if(bStartTrans)
    {
        if(bValidState && pUser->IsBeginTransaction() != 0)
            bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;

        // lobby, charselect ���´� ����.
        const DWORD blockFilter = (ePRS_AT_CHARSELECT | ePRS_AT_LOBBY) & ~ePRS_AT_ROOM;

        eZONE_STATE zoneState = pUser->GetZoneState();
        if(bValidState)
        {
            if((zoneState & ePRS_AT_ZONE) != ePRS_AT_ZONE || (zoneState & blockFilter) != 0)
                bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_INVALID_PLAYER_STATE;
        }
    }
    else
    {
        if(bValidState && pUser->IsBeginTransaction() == 0)
            bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_INVALID_PLAYER_STATE;
        if(bValidState && pUser->GetTransactionState() != TR_QUERY_SERVER_SELECT_TRANSACTION)
            bValidState = FALSE, msgACK.m_Result = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
    }

    // ���� ���� �ʱ�ȭ �ʿ� ���ɼ� Ÿ��...
    if(bValidState == 0)
    {
        pUser->SendPacket(&msgACK, msgACK.GetSize());
        return;
    }

    const nsPacket::ReAuthInfo& rReAuthInfo = pUser->GetReAuthInfo();
    if(bStartTrans)
        pUser->OnServerSelectQueryResult(rReAuthInfo.eReAuthSlot_AllSide,
                                        rReAuthInfo.eReAuthStep_Requested, 0);
    else
        pUser->OnServerSelectQueryResult(rReAuthInfo.eReAuthSlot_AllSide,
                                         rReAuthInfo.eReAuthStep_Canceled, 0);

    // Query to GameServer (Check Current State & Fix IdleState until Timeout)
    // Query to AuthServer (Get ReAuthToken)
    {
        MSG_AG_CONNECTION_SERVER_SELECT_CONTROL_SYN msgSYN;
        msgSYN.m_Control = bStartTrans ? msgSYN.eControl_StartTransaction
                                       : msgSYN.eControl_StopTransaction;
        pUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));
        if(bStartTrans)
            pUser->BeginTransaction(TR_QUERY_SERVER_SELECT_TRANSACTION);
        else
            pUser->EndTransaction();
    }
    {
        MSG_AU_AUTH_SERVER_SELECT_SYN msgSYN;
        msgSYN.m_AuthUserID = pUser->GetAuthID();
        msgSYN.m_Request = bStartTrans ? msgSYN.eRequest_QueryStart
                                       : msgSYN.eRequest_Cancel;
        AgentServer::GetInstance()->SendToAuthAgentServer((BYTE*)&msgSYN, sizeof(msgSYN));
    }

    if(bStartTrans == 0)
    {
        msgACK.m_Result = RC::RC_ROOM_SERVER_SELECT_TRANSACTION_CANCELED;
        pUser->SendPacket(&msgACK, msgACK.GetSize());
    }
}
#endif

#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE

HANDLER_CG_IMPL(CG_CHARNAME_CHANGE_SYN) 
{
    const MSG_CG_CHARNAME_CHANGE_SYN* pRecvMsg = 
        static_cast<MSG_CG_CHARNAME_CHANGE_SYN*>(pMsg);

    if(!pUser->IsValidSlot(pRecvMsg->slot_index_))
    {
        MSG_CG_CHARNAME_CHANGE_NAK nak;
        nak.slot_index_ = pRecvMsg->slot_index_;
        nak.result_ = MSG_CG_CHARNAME_CHANGE_NAK::INDEX_INVALID;
        pUser->SendPacket(&nak, sizeof(nak));
        return;
    }

    MSG_AD_CHARNAME_CHANGE_SYN msg;
    msg.m_dwKey = pUser->GetUserKey();
    msg.character_guid_ = pUser->GetCharGUID(pRecvMsg->slot_index_);
    msg.slot_index_ = pRecvMsg->slot_index_;
    _tcsncpy( msg.character_name_, pRecvMsg->character_name_, _countof(msg.character_name_) );
    msg.character_name_[MAX_CHARNAME_LENGTH] = '\0';

    if(FALSE == pUser->SendToDBPServer(&msg, sizeof(msg)))
    {
        MSG_CG_CHARNAME_CHANGE_NAK nak;
        nak.slot_index_ = pRecvMsg->slot_index_;
        nak.result_ = MSG_CG_CHARNAME_CHANGE_NAK::SEND_FAIL;
        pUser->SendPacket(&nak, sizeof(nak));
    }
}

#endif //_JP_20110222_INGAME_CHARNAME_CHANGE

// �̼� �κ� �� ����Ʈ ��û
HANDLER_CG_IMPL(CG_ZONE_LIST_MISSIONLOBBY_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LIST_MISSIONLOBBY_SYN* receive_message = static_cast<MSG_CG_ZONE_LIST_MISSIONLOBBY_SYN*>(pMsg);

    // (CHANGES)(20100318)(hukim)(REQ_0038) �� ����� �� ���� �������� �˻��� ��
    // ����� ����� �̼� �˻縦 �ϱ� ���� ���� ������ ��û�Ѵ�.
    
    MSG_AG_ZONE_CHECK_MISSION_ROOM_LIST_SYN msg_syn;
    g_ZoneManager.GetMissionLobbyRoomList(pUser, 
                                          receive_message->m_Count, 
                                          MSG_AG_ZONE_CHECK_MISSION_ROOM_LIST_SYN::kMaxRoomCount, 
                                          msg_syn.current_room_count_, 
                                          msg_syn.room_info_list_);
    pUser->SendToLinkedServer(&msg_syn, msg_syn.GetSize());
}

// ���� �κ� �� ����Ʈ ��û
HANDLER_CG_IMPL(CG_ZONE_LIST_HUNTINGLOBBY_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LIST_HUNTINGLOBBY_SYN* pRecvMsg = (MSG_CG_ZONE_LIST_HUNTINGLOBBY_SYN *)pMsg;

	MSG_CG_ZONE_LIST_HUNTINGLOBBY_ACK msg;
	g_ZoneManager.GetHuntingLobbyRoomList(pUser, pRecvMsg->m_Count, MSG_CG_ZONE_LIST_HUNTINGLOBBY_ACK::_MAX_ROOM_NUM, msg.m_Count, msg.m_RoomInfo);
	pUser->SendPacket(&msg, msg.GetSize());
}

// ����� ���� �� ����Ʈ ��û
HANDLER_CG_IMPL(CG_ZONE_LIST_HUNTING_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LIST_HUNTING_SYN* pRecvMsg = (MSG_CG_ZONE_LIST_HUNTING_SYN *)pMsg;

	MSG_CG_ZONE_LIST_HUNTING_ACK msg;
	g_ZoneManager.GetHuntingRoomList(pUser, pRecvMsg->m_Count, MSG_CG_ZONE_LIST_HUNTING_ACK::_MAX_ROOM_NUM, msg.m_Count, msg.m_RoomInfo);
	pUser->SendPacket(&msg, msg.GetSize());
}

// PVP�� ����Ʈ ��û
HANDLER_CG_IMPL(CG_ZONE_LIST_PVPLOBBY_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LIST_PVPLOBBY_SYN* pRecvMsg = (MSG_CG_ZONE_LIST_PVPLOBBY_SYN *)pMsg;

	MSG_CG_ZONE_LIST_PVPLOBBY_ACK msg;
	g_ZoneManager.GetPVPLobbyRoomList(pUser, pRecvMsg->m_Count, MSG_CG_ZONE_LIST_PVPLOBBY_ACK::_MAX_ROOM_NUM, msg.m_Count, msg.m_RoomInfo);
	pUser->SendPacket(&msg, msg.GetSize());
}

// �������ù� ����Ʈ ��û
HANDLER_CG_IMPL(CG_ZONE_LIST_CHUNTINGLOBBY_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LIST_CHUNTINGLOBBY_SYN* pRecvMsg = (MSG_CG_ZONE_LIST_CHUNTINGLOBBY_SYN*)pMsg;

	MSG_CG_ZONE_LIST_CHUNTINGLOBBY_ACK msg;
	g_ZoneManager.GetCHuntingLobbyRoomList(pUser, pRecvMsg->m_Count, MSG_CG_ZONE_LIST_CHUNTINGLOBBY_ACK::_MAX_ROOM_NUM, msg.m_Count, msg.m_RoomInfo);
	pUser->SendPacket(&msg, msg.GetSize());
}

HANDLER_CG_IMPL(CG_ZONE_LIST_CHUNTING_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LIST_CHUNTING_SYN* pRecvMsg = (MSG_CG_ZONE_LIST_CHUNTING_SYN*)pMsg;

	MSG_CG_ZONE_LIST_CHUNTING_ACK msg;
#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	g_ZoneManager.GetGuildPVPRoomList(pUser, pRecvMsg->m_Count, MSG_CG_ZONE_LIST_CHUNTING_ACK::_MAX_ROOM_NUM, msg.m_Count, msg.m_RoomInfo);
#else
	g_ZoneManager.GetCHuntingRoomList(pUser, pRecvMsg->m_Count, MSG_CG_ZONE_LIST_CHUNTING_ACK::_MAX_ROOM_NUM, msg.m_Count, msg.m_RoomInfo);
#endif
	pUser->SendPacket(&msg, msg.GetSize());
}

// Ư�� �� ����Ʈ ���ΰ�ħ
HANDLER_CG_IMPL(CG_ZONE_LIST_REFRESH_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LIST_REFRESH_SYN* pRecvMsg = (MSG_CG_ZONE_LIST_REFRESH_SYN *)pMsg;

	ZoneInterface* pZone = g_ZoneManager.FindZone(pRecvMsg->m_RoomKey);
	if(!pZone)
	{
		MSG_CG_ZONE_LIST_REFRESH_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_NOT_EXIST_ROOM;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}
	switch(pZone->GetType())
	{
	case eZONETYPE_LOBBY:
		{
			// ����or�̼�
			LobbyRoom* pRoom = (LobbyRoom *)pZone;
			switch(pRoom->GetLobbyType())
			{
			case eZONETYPE_HUNTING:
				{
					MSG_CG_ZONE_LIST_REFRESH_HUNTINGLOBBY_ACK amsg;
					pRoom->SerializeLoad(amsg.m_RoomInfo);
					pUser->SendPacket(&amsg, sizeof(amsg));
				}break;
			case eZONETYPE_MISSION:
				{
					MSG_CG_ZONE_LIST_REFRESH_MISSIONLOBBY_ACK amsg;
					pRoom->SerializeLoad(amsg.m_RoomInfo);
					pUser->SendPacket(&amsg, sizeof(amsg));
				}break;
			case eZONETYPE_PVP:
				{
					MSG_CG_ZONE_LIST_REFRESH_PVPLOBBY_ACK amsg;
					pRoom->SerializeLoad(amsg.m_RoomInfo);
					pUser->SendPacket(&amsg, sizeof(amsg));
				}break;
			case eZONETYPE_CHUNTING:
				{
					MSG_CG_ZONE_LIST_REFRESH_CHUNTINGLOBBY_ACK amsg;
					pRoom->SerializeLoad(amsg.m_RoomInfo);
					pUser->SendPacket(&amsg, sizeof(amsg));
				}break;
			default:
				{
					SUNLOG(eFULL_LOG, "�߸��� �κ�Ÿ�Դϴ�.\n");
				}
			}
		}
		break;
	case eZONETYPE_HUNTING:
		{
			HuntingRoom* pRoom = (HuntingRoom *)pZone;
			MSG_CG_ZONE_LIST_REFRESH_HUNTING_ACK amsg;
			pRoom->SerializeLoad(amsg.m_RoomInfo);
			pUser->SendPacket(&amsg, sizeof(amsg));
		}
		break;
	case eZONETYPE_CHUNTING:
		{
			CHuntingRoom* pRoom = (CHuntingRoom *)pZone;
			MSG_CG_ZONE_LIST_REFRESH_CHUNTING_ACK amsg;
			pRoom->SerializeLoad(amsg.m_RoomInfo);
			pUser->SendPacket(&amsg, sizeof(amsg));
		}
		break;
	default:
		{
			MSG_CG_ZONE_LIST_REFRESH_NAK msg;
			msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;
			pUser->SendPacket(&msg, sizeof(msg));
		}
	}
}

// �ٷΰ���
HANDLER_CG_IMPL(CG_ZONE_SHORTCUT_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_SHORTCUT_SYN* pRecvMsg = (MSG_CG_ZONE_SHORTCUT_SYN *)pMsg;

	RoomInterface* pRoom = g_ZoneManager.FindRecommendRoom(pUser, (eZONETYPE)pRecvMsg->m_byRoomType);
	if(!pRoom)
	{
		MSG_CG_ZONE_SHORTCUT_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_NOT_EXIST_ROOM;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	switch(pRoom->GetType())
	{
	case eZONETYPE_LOBBY:
		{
			LobbyRoom* pLobbyRoom = (LobbyRoom *)pRoom;
			switch(pLobbyRoom->GetLobbyType())
			{
			case eZONETYPE_HUNTING:
				{
					MSG_CG_ZONE_SHORTCUT_HUNTINGLOBBY_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_MISSION:
				{
					MSG_CG_ZONE_SHORTCUT_MISSIONLOBBY_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_PVP:
				{
					MSG_CG_ZONE_SHORTCUT_PVPLOBBY_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_CHUNTING:
				{
					MSG_CG_ZONE_SHORTCUT_CHUNTINGLOBBY_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			default:
				{
					SUNLOG(eFULL_LOG, "�߸��� �κ�Ÿ�Դϴ�.\n");
				}
			}
		}break;
	case eZONETYPE_HUNTING:
		{
			HuntingRoom* pHunting = (HuntingRoom *)pRoom; 
			MSG_CG_ZONE_SHORTCUT_HUNTING_ACK msg;
			pHunting->SerializeLoad(msg.m_RoomInfo);
			pUser->SendPacket(&msg, sizeof(msg));
		}break;
	default:
		{
			MSG_CG_ZONE_SHORTCUT_NAK msg;
			msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;
			pUser->SendPacket(&msg, sizeof(msg));
		}
	}
}

// ���̵�� �� ã��
HANDLER_CG_IMPL(CG_ZONE_FIND_ROOM_FROM_CHARID_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_SYN* pRecvMsg = (MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_SYN *)pMsg;

	TCHAR tszCharName[MAX_CHARNAME_LENGTH+1];
	memcpy(tszCharName, pRecvMsg->m_pszCharName, sizeof(TCHAR)*MAX_CHARNAME_LENGTH);
	tszCharName[MAX_CHARNAME_LENGTH] = '\0';
	User* pFindUser = UserManager::Instance()->FindUserForCharName(tszCharName);
	if(!pFindUser)
	{
		MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK msg;
		msg.m_dwErrorCode = 5601;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}
	ZoneInterface* pZone = g_ZoneManager.FindZone(pFindUser->GetZoneKey());
	if(!pZone)
	{
		MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK msg;
		msg.m_dwErrorCode = 5602;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}
	switch(pZone->GetType())
	{
	case eZONETYPE_LOBBY:
		{
			LobbyRoom* pLobbyRoom = (LobbyRoom *)pZone;
			switch(pLobbyRoom->GetLobbyType())
			{
			case eZONETYPE_HUNTING:
				{
					MSG_CG_ZONE_HUNTINGLOBBY_ROOM_FROM_CHARID_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_MISSION:
				{
					MSG_CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_PVP:
				{
					MSG_CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_CHUNTING:
				{
					MSG_CG_ZONE_CHUNTINGLOBBY_ROOM_FROM_CHARID_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			default:
				{
					SUNLOG(eFULL_LOG, "�߸��� �κ�Ÿ�Դϴ�.\n");
				}
			}
		}break;
	case eZONETYPE_HUNTING:
		{
			HuntingRoom* pHunting = (HuntingRoom *)pZone; 
			MSG_CG_ZONE_HUNTING_ROOM_FROM_CHARID_ACK msg;
			pHunting->SerializeLoad(msg.m_RoomInfo);
			pUser->SendPacket(&msg, sizeof(msg));
		}break;
	case eZONETYPE_CHUNTING:
		{
			CHuntingRoom* pCHunting = (CHuntingRoom *)pZone; 
			MSG_CG_ZONE_CHUNTING_ROOM_FROM_CHARID_ACK msg;
			pCHunting->SerializeLoad(msg.m_RoomInfo);
			pUser->SendPacket(&msg, sizeof(msg));
		}break;
	default:
		{
			MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK msg;
			msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;
			pUser->SendPacket(&msg, sizeof(msg));
		}
	}
}

// ���̵�� �� ã��
HANDLER_CG_IMPL(CG_ZONE_FIND_ROOM_FROM_NO_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_FIND_ROOM_FROM_NO_SYN* pRecvMsg = (MSG_CG_ZONE_FIND_ROOM_FROM_NO_SYN *)pMsg;

	ZoneInterface* pZone = g_ZoneManager.FindZone(pRecvMsg->m_RoomNo);
	if(!pZone)
	{
		MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK msg;
		msg.m_dwErrorCode = 5602;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}
	switch(pZone->GetType())
	{
	case eZONETYPE_LOBBY:
		{
			LobbyRoom* pLobbyRoom = (LobbyRoom *)pZone;
			switch(pLobbyRoom->GetLobbyType())
			{
			case eZONETYPE_HUNTING:
				{
					MSG_CG_ZONE_HUNTINGLOBBY_ROOM_FROM_CHARID_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_MISSION:
				{
					MSG_CG_ZONE_MISSIONLOBBY_ROOM_FROM_CHARID_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_PVP:
				{
					MSG_CG_ZONE_PVPLOBBY_ROOM_FROM_CHARID_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			case eZONETYPE_CHUNTING:
				{
					MSG_CG_ZONE_CHUNTINGLOBBY_ROOM_FROM_CHARID_ACK msg;
					pLobbyRoom->SerializeLoad(msg.m_RoomInfo);
					pUser->SendPacket(&msg, sizeof(msg));
				}break;
			default:
				{
					SUNLOG(eFULL_LOG, "�߸��� �κ�Ÿ�Դϴ�.\n");
				}
			}
		}break;
	case eZONETYPE_HUNTING:
		{
			HuntingRoom* pHunting = (HuntingRoom *)pZone; 
			MSG_CG_ZONE_HUNTING_ROOM_FROM_CHARID_ACK msg;
			pHunting->SerializeLoad(msg.m_RoomInfo);
			pUser->SendPacket(&msg, sizeof(msg));
		}break;
	case eZONETYPE_CHUNTING:
		{
			CHuntingRoom* pCHunting = (CHuntingRoom *)pZone; 
			MSG_CG_ZONE_CHUNTING_ROOM_FROM_CHARID_ACK msg;
			pCHunting->SerializeLoad(msg.m_RoomInfo);
			pUser->SendPacket(&msg, sizeof(msg));
		}break;
	default:
		{
			MSG_CG_ZONE_FIND_ROOM_FROM_CHARID_NAK msg;
			msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;
			pUser->SendPacket(&msg, sizeof(msg));
		}
	}
}

// ������ �κ�(����or�̼�)���� �����
HANDLER_CG_IMPL(CG_ZONE_LOBBY_CREATE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_CREATE_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_CREATE_SYN *)pMsg;

	MSG_CG_ZONE_LOBBY_CREATE_SYN pSafeBuffer[1];
	memcpy(pSafeBuffer, pRecvMsg, sizeof(MSG_CG_ZONE_LOBBY_CREATE_SYN));
	pRecvMsg = pSafeBuffer;
	pRecvMsg->Decode();

    if(pUser->IsBeginTransaction())
    {
        MSG_CG_ZONE_LOBBY_CREATE_NAK msg;
        msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&msg, sizeof(msg));
        return;
    }

    // (WAVERIX) (090829) (CHANGES) _PCBANG_POLICY_CHANGE_POST_EXPIRED
    BASE_ROOMINFO roomInfo_modified;
    g_ZoneManager.CheckRuleForRoom(pUser, pRecvMsg->m_RoomInfo, &roomInfo_modified);

    eZONETYPE zoneType = eZONETYPE(pRecvMsg->m_byRoomType);
    if(zoneType == eZONETYPE_MISSION)
    {
        MSG_AG_ZONE_LOBBY_CREATE_SYN msg;

        msg.m_MapCode = pRecvMsg->m_MapCode;
        msg.m_byMaxLimitUserNum = pRecvMsg->m_byMaxLimitUserNum;
        msg.m_byRoomType = pRecvMsg->m_byRoomType;      // eZONETYPE
        msg.m_byRoomPublic = pRecvMsg->m_byRoomPublic;  //< eZONEPUBLIC
        strncpy(msg.m_pszRoomTitle, pRecvMsg->m_pszRoomTitle, _countof(msg.m_pszRoomTitle));  // no null-check
        msg.m_RoomInfo = roomInfo_modified;
        if(msg.m_byRoomPublic != eZONEPUBLIC_PUBLIC)
            strncpy(msg.m_pszRoomPWD, pRecvMsg->m_pszRoomPWD, _countof(msg.m_pszRoomPWD));  // no null-check
        else
            ZeroMemory(msg.m_pszRoomPWD, sizeof(msg.m_pszRoomPWD));
        pUser->SendToLinkedServer(&msg, sizeof(msg));
    }
    else
    {
	    pUser->BeginTransaction(TR_CREATELOBBY);

	    LobbyRoom* pLobbyRoom = NULL;
	    BYTE byMaxLimitUserNum = pRecvMsg->m_byMaxLimitUserNum;

	    RC::eROOM_RESULT rt = g_ZoneManager.CreateLobbyRoom(pUser->GetZoneKey(), pUser, pRecvMsg->m_MapCode, 
		    (eZONETYPE)pRecvMsg->m_byRoomType, (eZONEPUBLIC)pRecvMsg->m_byRoomPublic, 
		    pRecvMsg->m_pszRoomTitle, pRecvMsg->m_pszRoomPWD, roomInfo_modified, byMaxLimitUserNum, 0, pLobbyRoom);

	    if(RC::RC_ROOM_SUCCESS == rt)
	    {
		    MSG_CG_ZONE_LOBBY_CREATE_ACK msg;
		    msg.m_LobbyKey = pLobbyRoom->GetKey();
		    pUser->SendPacket(&msg, sizeof(msg));
	    }
	    else
	    {
		    // �κ��������
		    MSG_CG_ZONE_LOBBY_CREATE_NAK msg;
		    msg.m_dwErrorCode = rt;
		    pUser->SendPacket(&msg, sizeof(msg));
	    }

	    pUser->EndTransaction();
    }
}

// �������� ���ηκ� ����
HANDLER_CG_IMPL(CG_ZONE_CHUNTINGLOBBY_CREATE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN* pRecvMsg = (MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN*)pMsg;

	MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_NAK msg;
	msg.m_byErrorCode = RC::RC_ROOM_CANNOT_JOIN_ROOMTYPE;
	pUser->SendPacket(&msg, sizeof(msg));
	return;
#if SUN_CODE_BACKUP
	MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN pSafeBuffer[1];
	memcpy(pSafeBuffer, pRecvMsg, sizeof(MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN));
	pRecvMsg = pSafeBuffer;
	//BOOL bIsValidSize = (BOOL) !(sizeof(MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN) < wSize);
	//if(!bIsValidSize)
	//{
	//	SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN"));
	//	return;
	//}
	pRecvMsg->Decode();

	// ��ũ��ų �������ù��� �ִ��� üũ
	CHuntingRoom *pCHunting = g_ZoneManager.FindCHunting(pRecvMsg->m_LinkHuntingKey);
	if(!pCHunting)
	{
		MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_INVALID_CHUNTING_LINK_KEY;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

    // (WAVERIX) (090829) (CHANGES) _PCBANG_POLICY_CHANGE_POST_EXPIRED
    BASE_ROOMINFO roomInfo_modified;
    g_ZoneManager.CheckRuleForRoom(pUser, pRecvMsg->m_RoomInfo, &roomInfo_modified);

	pUser->BeginTransaction(TR_CREATELOBBY);

	LobbyRoom *pLobbyRoom = NULL;

	RC::eROOM_RESULT rt = g_ZoneManager.CreateLobbyRoom(pUser->GetZoneKey(), pUser, pCHunting->GetMapCode(), 
		eZONETYPE_CHUNTING, (eZONEPUBLIC)pRecvMsg->m_byRoomPublic, pRecvMsg->m_pszRoomTitle, 
		pRecvMsg->m_pszRoomPWD, roomInfo_modified, 0, pRecvMsg->m_LinkHuntingKey, pLobbyRoom);

	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_ACK msg;
		msg.m_LobbyKey = pLobbyRoom->GetKey();
		msg.m_MapCode = pCHunting->GetMapCode();
		msg.m_AdditionalCHuntingInfo = *pCHunting->GetAdditionalCHuntingInfo();
		msg.m_NonBlockNum = pLobbyRoom->GetLimitMaxUserNum();
		pUser->SendPacket(&msg, sizeof(msg));
	}
	else
	{
		// �κ��������
		MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_NAK msg;
		msg.m_byErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}

	pUser->EndTransaction();
#endif //SUN_CODE_BACKUP
}

// �κ� ����
HANDLER_CG_IMPL(CG_ZONE_LOBBY_JOIN_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_JOIN_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_JOIN_SYN *)pMsg;

    LobbyRoom* pLobby = g_ZoneManager.FindLobby(pRecvMsg->m_LobbyKey);
    if(pLobby == 0)
    {
        MSG_CG_ZONE_LOBBY_JOIN_NAK msg;
        msg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTLOBBY;
        pUser->SendPacket(&msg, sizeof(msg));
        return;
    }

    if(pUser->IsBeginTransaction())
    {
        MSG_CG_ZONE_LOBBY_JOIN_NAK msg;
        msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&msg, sizeof(msg));
        return;
    }

    if(pLobby->GetLobbyType() == eZONETYPE_MISSION)
    {
        MSG_AG_ZONE_LOBBY_JOIN_SYN msg;
        msg.m_LobbyKey = pRecvMsg->m_LobbyKey;
        msg.m_byRoomPublic = pRecvMsg->m_byRoomPublic;
        if(msg.m_byRoomPublic != eZONEPUBLIC_PUBLIC)
            strncpy(msg.m_pszRoomPWD, pRecvMsg->m_pszRoomPWD, _countof(msg.m_pszRoomPWD));  // no null-check
        else
            ZeroMemory(msg.m_pszRoomPWD, sizeof(msg.m_pszRoomPWD));
        msg.m_MapCode = pLobby->GetMapCode();
        pUser->SendToLinkedServer(&msg, sizeof(msg));
    }
    else
    {
	    pUser->BeginTransaction(TR_JOINLOBBY);

	    CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		    MoveZonePolicy<MZTF_VILLAGE_TO_LOBBY>::AlignInfo(
			    pUser
			    //{		From		|			To		}
			    , eZONETYPE_VILLAGE,	eZONETYPE_LOBBY			//	eZONETYPE
			    , pUser->GetZoneKey(),	pRecvMsg->m_LobbyKey	//	ZoneKey
			);
	    RoomArgument args(
		    (eZONEPUBLIC)pRecvMsg->m_byRoomPublic
		    , ((pRecvMsg->m_byRoomPublic==eZONEPUBLIC_PRIVATE) ? pRecvMsg->m_pszRoomPWD : NULL)
		);
	    const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, args);
	    if(RC::RC_ROOM_SUCCESS == rt)
	    {
		    LobbyRoom* pLobby = g_ZoneManager.FindLobby(pRecvMsg->m_LobbyKey);
		    if(!pLobby)	return;

		    MSG_CG_ZONE_LOBBY_JOIN_ACK msg;
		    msg.m_dwMasterKey	= pLobby->GetMaster()->GetPlayerKey();
		    pUser->SendPacket(&msg, sizeof(msg));
	    }
	    else
	    {
		    // �κ��������
		    MSG_CG_ZONE_LOBBY_JOIN_NAK msg;
		    msg.m_dwErrorCode = rt;
		    pUser->SendPacket(&msg, sizeof(msg));
	    }

	    pUser->EndTransaction();
    }
}

// [�������� ����]
// �������� ���� ��ư�� ������.
HANDLER_CG_IMPL(CG_ZONE_LOBBY_READY_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_READY_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_READY_SYN *)pMsg;


	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_READY_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = g_ZoneManager.UserReadyInLobby(pUser, TRUE);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_LOBBY_READY_ACK amsg;
		pUser->SendPacket(&amsg, sizeof(amsg));

		LobbyRoom* pLobby = g_ZoneManager.FindLobby(pUser->GetZoneKey());
		if(!pLobby)	return;

		MSG_CG_ZONE_LOBBY_READY_BRD bmsg;
		bmsg.m_PlayerKey = pUser->GetPlayerKey();
		pLobby->SendToAll(&bmsg, sizeof(bmsg), pUser->GetUserKey());
	}
	else
	{
		MSG_CG_ZONE_LOBBY_READY_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

HANDLER_CG_IMPL(CG_ZONE_LOBBY_READY_CANCEL_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_READY_CANCEL_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_READY_CANCEL_SYN *)pMsg;


	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_READY_CANCEL_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = g_ZoneManager.UserReadyInLobby(pUser, FALSE);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_LOBBY_READY_CANCEL_ACK amsg;
		pUser->SendPacket(&amsg, sizeof(amsg));

		LobbyRoom* pLobby = g_ZoneManager.FindLobby(pUser->GetZoneKey());
		if(!pLobby)	return;

		MSG_CG_ZONE_LOBBY_READY_CANCEL_BRD bmsg;
		bmsg.m_PlayerKey = pUser->GetPlayerKey();
		pLobby->SendToAll(&bmsg, sizeof(bmsg), pUser->GetUserKey());
	}
	else
	{
		MSG_CG_ZONE_LOBBY_READY_CANCEL_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

// ������ �������� ��û�Ѵ�.
HANDLER_CG_IMPL(CG_ZONE_LOBBY_REQUEST_INFO_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_REQUEST_INFO_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_REQUEST_INFO_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_LOBBY_REQUEST_INFO_SYN)");
#endif


	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_REQUEST_INFO_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}
    // CHANGES: f110609.1L, f110608.6L, this changes responded to SendLobbyInfoForRequest method
    if (pUser->GetZoneType() == eZONETYPE_LOBBY)
    {
        LobbyRoom* const lobby = g_ZoneManager.FindLobby(pUser->GetZoneKey());
        if (lobby == NULL) {
            return;
        };
        if (lobby->SendLobbyInfoForRequest(pUser))
        {
            // success
            return;
        }
    };
    //
    MSG_CG_ZONE_LOBBY_REQUEST_INFO_NAK msg_nak;
    msg_nak.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;
    pUser->SendPacket(&msg_nak, sizeof(msg_nak));
}

// [������ ����]
// �ο��� ����
HANDLER_CG_IMPL(CG_ZONE_LOBBY_NONBLOCKSLOTNUM_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_SYN *)pMsg;

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = g_ZoneManager.SetNonblockSlotNumInLobby(pUser, pRecvMsg->m_NonBlockSlotNum);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_ACK amsg;
		pUser->SendPacket(&amsg, sizeof(amsg));

		LobbyRoom* pLobby = g_ZoneManager.FindLobby(pUser->GetZoneKey());
		if(!pLobby)	return;

		MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_BRD bmsg;
		bmsg.m_NonBlockSlotNum = pRecvMsg->m_NonBlockSlotNum;
		pLobby->SendToAll(&bmsg, sizeof(bmsg), pUser->GetUserKey());
	}
	else
	{
		MSG_CG_ZONE_LOBBY_NONBLOCKSLOTNUM_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

// �÷��̾� ����
HANDLER_CG_IMPL(CG_ZONE_LOBBY_KICKPLAYER_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_KICKPLAYER_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_KICKPLAYER_SYN *)pMsg;


	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_KICKPLAYER_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	User* pTargetUser = NULL;
	RC::eROOM_RESULT rt = g_ZoneManager.KickUserFromLobby(pUser, pRecvMsg->m_PlayerKey, pTargetUser);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_LOBBY_KICKPLAYER_ACK amsg;
		pUser->SendPacket(&amsg, sizeof(amsg));

		// �κ񿡼� ������ ��Ŷ ����
		MSG_CG_ZONE_LOBBY_LEAVE_ACK msg;
		pTargetUser->SendPacket(&msg, sizeof(msg));

	}
	else
	{
		MSG_CG_ZONE_LOBBY_KICKPLAYER_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

// �÷��̾� �� ����
HANDLER_CG_IMPL(CG_ZONE_LOBBY_CHANGE_TEAM_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_CHANGE_TEAM_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_CHANGE_TEAM_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_LOBBY_CHANGE_TEAM_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_CHANGE_TEAM_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = g_ZoneManager.ChangeTeamInLobby(pUser, pRecvMsg->m_Team);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_LOBBY_CHANGE_TEAM_ACK amsg;
		pUser->SendPacket(&amsg, sizeof(amsg));

		LobbyRoom* pLobby = g_ZoneManager.FindPVPLobby(pUser->GetZoneKey());
		if(!pLobby)	return;

		MSG_CG_ZONE_LOBBY_CHANGE_TEAM_BRD bmsg;
		bmsg.m_dwPlayerKey	= pUser->GetPlayerKey();
		bmsg.m_Team			= pRecvMsg->m_Team;
		pLobby->SendToAll(&bmsg, sizeof(bmsg), pUser->GetUserKey());
	}
	else
	{
		MSG_CG_ZONE_LOBBY_CHANGE_TEAM_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

// �� ����
HANDLER_CG_IMPL(CG_ZONE_LOBBY_CHANGE_MAP_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_CHANGE_MAP_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_CHANGE_MAP_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_LOBBY_CHANGE_MAP_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_CHANGE_MAP_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = g_ZoneManager.ChangeMapInLobby(pUser, pRecvMsg->m_MapCode);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_LOBBY_CHANGE_MAP_ACK amsg;
		pUser->SendPacket(&amsg, sizeof(amsg));

		LobbyRoom* pLobby = g_ZoneManager.FindLobby(pUser->GetZoneKey());
		if(!pLobby)	return;

		MSG_CG_ZONE_LOBBY_CHANGE_MAP_BRD bmsg;
		bmsg.m_MapCode	= pRecvMsg->m_MapCode;
		pLobby->SendToAll(&bmsg, sizeof(bmsg), pUser->GetUserKey());
	}
	else
	{
		MSG_CG_ZONE_LOBBY_CHANGE_MAP_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

// �κ��� ���� ���� ���� �ٲ�(�ʵ�, ���̵�, ���ʽ�)
HANDLER_CG_IMPL(CG_ZONE_LOBBY_HUNTING_CONFIG_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_HUNTING_CONFIG_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_HUNTING_CONFIG_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_LOBBY_HUNTING_CONFIG_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_HUNTING_CONFIG_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = g_ZoneManager.ConfigHuntingRoomInLobby(pUser, pRecvMsg->m_AdditionalInfo);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_LOBBY_HUNTING_CONFIG_ACK amsg;
		pUser->SendPacket(&amsg, sizeof(amsg));

		LobbyRoom* pLobby = g_ZoneManager.FindHuntingLobby(pUser->GetZoneKey());
		if(!pLobby)	return;

		MSG_CG_ZONE_LOBBY_HUNTING_CONFIG_BRD bmsg;
		bmsg.m_AdditionalInfo = pRecvMsg->m_AdditionalInfo;
		pLobby->SendToAll(&bmsg, sizeof(bmsg), pUser->GetUserKey());
	}
	else
	{
		MSG_CG_ZONE_LOBBY_HUNTING_CONFIG_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

// �κ��� PVP ���� ���� �ٲ�(�ʵ�, ��, ���)
HANDLER_CG_IMPL(CG_ZONE_LOBBY_PVP_CONFIG_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_PVP_CONFIG_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_PVP_CONFIG_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_LOBBY_PVP_CONFIG_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_PVP_CONFIG_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = g_ZoneManager.ConfigPVPRoomInLobby(pUser, pRecvMsg->m_AdditionalPVPInfo);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		LobbyRoom* pLobby = g_ZoneManager.FindPVPLobby(pUser->GetZoneKey());
		if(!pLobby)	return;

        MSG_CG_ZONE_LOBBY_PVP_CONFIG_BRD msg_brd;
        msg_brd.m_AdditionalPVPInfo = pRecvMsg->m_AdditionalPVPInfo;
        // NOTE: f110609.3L
        ns_functor::PvPLobbyChangedStatusNotificationFilter filter;
        msg_brd.m_Count = pLobby->GetListMemberInfo(
            filter.GetRedirectedArray(msg_brd.m_PlayerInfo), NULL);
        // CHANGES: f110608.6L, the broadcast changed from the each packing and sending
        // to the one-packing and reused sending approach.
        pLobby->SendToAll(&msg_brd, msg_brd.GetSize(), 0);
	}
	else
	{
		MSG_CG_ZONE_LOBBY_PVP_CONFIG_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

// �κ��� ���� ���� ���� �ٲ�(�ʵ�, ���̵�, ���ʽ�)
HANDLER_CG_IMPL(CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_LOBBY_CHUNTING_CONFIG_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = g_ZoneManager.ConfigCHuntingRoomInLobby(pUser, pRecvMsg->m_AdditionalInfo);
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_ACK amsg;
		pUser->SendPacket(&amsg, sizeof(amsg));

		LobbyRoom *pLobby = g_ZoneManager.FindCHuntingLobby(pUser->GetZoneKey());
		if(!pLobby)	return;

		MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_BRD bmsg;
		bmsg.m_AdditionalInfo = pRecvMsg->m_AdditionalInfo;
		pLobby->SendToAll(&bmsg, sizeof(bmsg), pUser->GetUserKey());
	}
	else
	{
		MSG_CG_ZONE_LOBBY_CHUNTING_CONFIG_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

HANDLER_CG_IMPL(CG_ZONE_HUNTING_CREATE_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
    // (WAVERIX) (090624) (CHANGES) Remove unused section 'Hunting'
    ASSERT("Function Not Suported!!");
    return;
}

HANDLER_CG_IMPL(CG_ZONE_MISSION_CREATE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_MISSION_CREATE_SYN* pRecvMsg = (MSG_CG_ZONE_MISSION_CREATE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_MISSION_CREATE_SYN)");
#endif

	BOOL bIsValidSize = (BOOL) (sizeof(MSG_CG_ZONE_MISSION_CREATE_SYN) == wSize);
	if(!bIsValidSize)
	{
		SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ZONE_MISSION_CREATE_SYN"));
		return;
	}

    MSG_CG_ZONE_MISSION_CREATE_NAK msgNak;
	if(pUser->IsBeginTransaction())
	{
		msgNak.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msgNak, sizeof(msgNak));
		return;
	}

    //{__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__
    KEYTYPE lobbyKey = pUser->GetZoneKey();
    LobbyRoom* pLobbyRoom = g_ZoneManager.FindMissionLobby(lobbyKey);
    if(pLobbyRoom == NULL)
    {
        msgNak.m_dwErrorCode = RC::RC_ROOM_INVALID_ROOM_TYPE;
        pUser->SendPacket(&msgNak, sizeof(msgNak));
        return;
    }

    typedef MSG_AG_ZONE_CAN_CREATE_ROOM_SYN SynPacket;
    MSG_AG_ZONE_CAN_CREATE_ROOM_SYN msg;
    msg.Clear();
    msg.m_CheckMethod = msg.eCheck_Members;
    msg.m_MapCode = pLobbyRoom->GetMapCode();//������ �̼� �ڵ�

    SynPacket::CheckArgMembers& rMemberList = msg.m_CheckArgMembers;
    rMemberList.partyKey = 0;
    rMemberList.roomKey = lobbyKey;

    DWORD memberNumber = 0;
    const ZoneInterface::ZONE_USER_LIST& rUserList = pLobbyRoom->GetUserList();
    FOREACH_CONTAINER(const ZoneInterface::ZONE_USER_LIST::value_type& rUser, rUserList,
                      ZoneInterface::ZONE_USER_LIST)
    {
        User* pMember = rUser.second;
        if(memberNumber >= _countof(rMemberList.members))
            break;
        SynPacket::MemberInfo& rMemberInfo = rMemberList.members[memberNumber++];
        rMemberInfo.objectKey = (WORD)pMember->GetPlayerKey();
        rMemberInfo.status = msg.eStatus_PartialOk;
    }

    pUser->SendToLinkedServer(&msg, msg.GetSize());
    //}__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__
}

// PVP ���� (START�� ����)
HANDLER_CG_IMPL(CG_ZONE_PVP_CREATE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_PVP_CREATE_SYN* pRecvMsg = (MSG_CG_ZONE_PVP_CREATE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_PVP_CREATE_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_PVP_CREATE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	pUser->BeginTransaction(TR_LEAVE_LOBBY_FOR_PVP);

	KEYTYPE LobbyKey = pUser->GetZoneKey();
	PVPRoom* pRoom = NULL;

	RC::eROOM_RESULT rt = g_ZoneManager.CreatePVPRoom(LobbyKey, pUser, pRoom);

	if(RC::RC_ROOM_SUCCESS == rt)
	{		
		// ������ CreatePVPRoom�ȿ��� �̹� Move �Ǿ���!
		pUser->SetTRRoomKey(pRoom->GetKey());

		LobbyRoom* pLobby = g_ZoneManager.FindLobby(LobbyKey);
		if(!pLobby)	return;

		// �κ��� ��������� �κ� ���� SYN, �̼� ���� SYN
		RoomMigrater<LobbyRoom, PVPRoom, TR_LEAVE_LOBBY_FOR_PVP> op(pLobby,pRoom);
		pLobby->Foreach(op);
	}
	else
	{
		pUser->EndTransaction();

		// �̼� ���� ����
		MSG_CG_ZONE_PVP_CREATE_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

// �������� ���� (START�� ����)
HANDLER_CG_IMPL(CG_ZONE_CHUNTING_CREATE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_CHUNTING_CREATE_SYN* pRecvMsg = (MSG_CG_ZONE_CHUNTING_CREATE_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_CHUNTING_CREATE_SYN)");
#endif

	BOOL bIsValidSize = (BOOL) (sizeof(MSG_CG_ZONE_CHUNTING_CREATE_SYN) == wSize);
	if(!bIsValidSize)
	{
		SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ZONE_CHUNTING_CREATE_SYN"));
		return;
	}

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_CHUNTING_CREATE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	KEYTYPE LobbyKey = pUser->GetZoneKey();
	CHuntingRoom* pCHuntingRoom = NULL;

	// ��Ī�� ���� �ִ��� ã�´�.
	if(g_ZoneManager.AutoMatching(LobbyKey, pUser, pCHuntingRoom))
	{
		// �κ񿡼� �������� ������ ��� ������ ��Ų��.
		RC::eROOM_RESULT rt = g_ZoneManager.MigrateCHuntingRoom(pUser, LobbyKey, pCHuntingRoom);

		if(RC::RC_ROOM_SUCCESS != rt)
		{		
			MSG_CG_ZONE_CHUNTING_CREATE_NAK msg;
			msg.m_byErrorCode = rt;
			pUser->SendPacket(&msg, sizeof(msg));
		}
	}
	// ��Ī ���� ������ ���� ���� �����Ѵ�.
	else
	{
		pUser->BeginTransaction(TR_LEAVE_LOBBY_FOR_CHUNTING);

		RC::eROOM_RESULT rt = g_ZoneManager.CreateCHuntingRoom(LobbyKey, pUser, pCHuntingRoom);

		if(RC::RC_ROOM_SUCCESS == rt)
		{		
			// ������ CreateCHuntingRoom�ȿ��� �̹� Move �Ǿ���!
			pUser->SetTRRoomKey(pCHuntingRoom->GetKey());

			LobbyRoom* pLobby = g_ZoneManager.FindLobby(LobbyKey);
			if(!pLobby)	return;

			// �κ��� ��������� �κ� ���� SYN, �������� ���� SYN
			RoomMigrater<LobbyRoom, CHuntingRoom, TR_LEAVE_LOBBY_FOR_CHUNTING> op(pLobby,pCHuntingRoom);
			pLobby->Foreach(op);
		}
		else
		{
			pUser->EndTransaction();

			// �������� ���� ����
			MSG_CG_ZONE_CHUNTING_CREATE_NAK msg;
			msg.m_byErrorCode = rt;
			pUser->SendPacket(&msg, sizeof(msg));
		}
	}
}

// �κ񿡼� ������������ ����
HANDLER_CG_IMPL(CG_ZONE_CHUNTING_MIGRATE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_CHUNTING_MIGRATE_SYN* pRecvMsg = (MSG_CG_ZONE_CHUNTING_MIGRATE_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_CHUNTING_MIGRATE_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_CHUNTING_JOIN_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	//#ifndef _SUSUNLOVE_AUTO_MATCHING_
	//	// �κ񿡼� �������� ������ ��� ������ ��Ų��.
	//	CHuntingRoom* pRoom = NULL;
	//	RC::eROOM_RESULT rt = g_ZoneManager.MigrateCHuntingRoom(pUser, pUser->GetZoneKey(), pRecvMsg->m_CHuntingKey, pRoom);
	//	
	//	if(RC::RC_ROOM_SUCCESS != rt)
	//	{		
	//		MSG_CG_ZONE_CHUNTING_JOIN_NAK msg;
	//		msg.m_byErrorCode = rt;
	//		pUser->SendPacket(&msg, sizeof(msg));
	//	}
	//#endif
}

// �������� �ٷ� �������� ����
HANDLER_CG_IMPL(CG_ZONE_HUNTING_JOIN_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
    // (WAVERIX) (090624) (CHANGES) Remove unused section 'Hunting'
    ASSERT("Function Not Suported!!");
    return;
//	MSG_CG_ZONE_HUNTING_JOIN_SYN* pRecvMsg = (MSG_CG_ZONE_HUNTING_JOIN_SYN *)pMsg;
//
//#ifdef __NA_PACKETFLOW_TEST
//	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_HUNTING_JOIN_SYN)");
//#endif
//
//
//	if(pUser->IsBeginTransaction())
//	{
//		MSG_CG_ZONE_HUNTING_JOIN_NAK msg;
//		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
//		pUser->SendPacket(&msg, sizeof(msg));
//		return;
//	}
//
//	//���� �ִ� ���� ������ �ƴϸ�.. NAK
//	if(pUser->GetZoneType() != eZONETYPE_VILLAGE)
//	{
//		MSG_CG_ZONE_HUNTING_JOIN_NAK msg;
//		msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;			//�߸��� �� ���� ��Ŷ ����..
//		pUser->SendPacket(&msg, sizeof(msg));
//		return;
//	}
//
//	pUser->BeginTransaction(TR_LEAVE_LOBBY_FOR_HUNTING);
//
//	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
//		MoveZonePolicy<MZTF_VILLAGE_TO_HUNTING>::AlignInfo(
//			pUser
//			//{		From		|			To		}
//			, eZONETYPE_VILLAGE,	eZONETYPE_HUNTING		//	eZONETYPE
//			, pUser->GetZoneKey(),	pRecvMsg->m_HuntingKey	//	ZoneKey
//			);
//	RoomArgument args(
//		(eZONEPUBLIC)pRecvMsg->m_byRoomPublic
//		, ((pRecvMsg->m_byRoomPublic==eZONEPUBLIC_PRIVATE) ? pRecvMsg->m_pszRoomPWD : NULL)
//		);
//	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, args);
//	if(RC::RC_ROOM_SUCCESS == rt)
//	{
//		pUser->SetTRRoomKey(pRecvMsg->m_HuntingKey);
//	}
//	else
//	{
//		pUser->EndTransaction();
//
//		// �κ��������
//		MSG_CG_ZONE_HUNTING_JOIN_NAK msg;
//		msg.m_dwErrorCode = rt;
//		pUser->SendPacket(&msg, sizeof(msg));
//	}
}

// �κ�->����
HANDLER_CG_IMPL(CG_ZONE_LOBBY_LEAVE_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_ZONE_LOBBY_LEAVE_SYN* pRecvMsg = (MSG_CG_ZONE_LOBBY_LEAVE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
    SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_LOBBY_LEAVE_SYN)");
#endif

    if(pUser->IsBeginTransaction())
    {
        MSG_CG_ZONE_LOBBY_LEAVE_NAK msg;
        msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&msg, sizeof(msg));
        return;
    }

    //���� �ִ� ���� �κ� �ƴϸ�.. NAK
    if(pUser->GetZoneType() != eZONETYPE_LOBBY)
    {
        MSG_CG_ZONE_LOBBY_LEAVE_NAK msg;
        msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;			//�߸��� �� ���� ��Ŷ ����..
        pUser->SendPacket(&msg, sizeof(msg));
        return;
    }

    const MAPCODE currentMapCode = pUser->GetSelectedCharMapCode();
    ZoneInterface* pZone = g_ZoneManager.FindStaticZone(currentMapCode);
    MSG_CG_ZONE_LOBBY_LEAVE_NAK msgNAK;

    if(pZone)
    {
        // (WAVERIX) (090520) (CHANGES) Ʃ�丮�� ���� �˻� ��� ����
        eZONETYPE staticZoneType = pZone->GetType();
        BOOLEAN isAcceptableStatus = (staticZoneType == eZONETYPE_VILLAGE || 
                                      currentMapCode == HardCode::MAPCODE_for_TUTORIAL_FIELD);

        if(isAcceptableStatus)
        {
            KEYTYPE toZoneKey = pZone->GetKey();

            CheckMoveZonePolicy* const pCheckMoveZonePolicy =
                MoveZonePolicy<MZTF_LOBBY_TO_VILLAGE>::AlignInfo(
                    pUser,
                    //{     From        |     To        }
                    eZONETYPE_LOBBY,     staticZoneType,// eZONETYPE
                    pUser->GetZoneKey(),   toZoneKey    // ZoneKey
                );
            const RC::eROOM_RESULT rt =
                g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
            if(RC::RC_ROOM_SUCCESS == rt)
            {
                //???? �̰� �����ؾ� �ǳ�??;;
                pUser->SetTRRoomKey(toZoneKey);

                // �κ񿡼� ������ ��Ŷ ����
                MSG_CG_ZONE_LOBBY_LEAVE_ACK msgACK;
                pUser->SendPacket(&msgACK, sizeof(msgACK));

                return;
            }
            else
                msgNAK.m_dwErrorCode = rt;
        }
        else
            msgNAK.m_dwErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
    }
    else
        msgNAK.m_dwErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;

    pUser->SendPacket(&msgNAK, sizeof(msgNAK));
    return;
}

HANDLER_CG_IMPL(CG_ZONE_MISSION_LEAVE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_MISSION_LEAVE_SYN* pRecvMsg = (MSG_CG_ZONE_MISSION_LEAVE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_MISSION_LEAVE_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_MISSION_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	// ���� �ִ� ���� �̼ǹ��� �ƴϸ�.. NAK
	if(pUser->GetZoneType() != eZONETYPE_MISSION)
	{
		MSG_CG_ZONE_MISSION_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;			//�߸��� �� ���� ��Ŷ ����..
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	ZoneInterface* pPrevZone = g_ZoneManager.FindStaticZone(pUser->GetSelectedCharMapCode());
	if(!pPrevZone)
	{
		MSG_CG_ZONE_MISSION_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

    // (History)
    //	(WAVERIX) (080626) (MEMORIAL) Ʃ�丮�󿡼� ������ �� �� ���� ��Ȳ ����

	const eZONETYPE prevZoneType = pPrevZone->GetType();
    const DWORD AcceptablePrevZoneType = 
        (1 << eZONETYPE_VILLAGE) | 
        (1 << eZONETYPE_FIELD) |
        (1 << eZONETYPE_DOMINATION_FIELD);

	BOOLEAN enableAccept = !!(AcceptablePrevZoneType & (1 << prevZoneType));
	ZoneInterface* pZone = NULL;

	if(enableAccept == 0 || !(pZone = g_ZoneManager.FindZone(pUser->GetZoneKey())))
	{
		MSG_CG_ZONE_MISSION_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTROOM;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	// �̼� ������ �ǵ������� ������ �ϸ� ������ ����
	if(pZone->GetUserNum() >= 2 && pRecvMsg->m_Reason == MSG_CG_ZONE_MISSION_LEAVE_SYN::REASON_INTEND)
	{
		// �ǵ����� ��� �������� ��´�.
		MSG_AG_STATUS_DECREASE_DURA_CMD msg;
		pUser->SendToLinkedServer(&msg, sizeof(msg));
	}

	WORD wFromMapCode = pZone->GetMapCode();

	//Village* pVillage = (Village*)pPrevZone;
    CheckMoveZonePolicy* const pCheckMoveZonePolicy =
        MoveZonePolicy<MZTF_ROOM_TO_STATIC>::AlignInfo(
            pUser,
            //{     From        |       To      }
            eZONETYPE_MISSION,	    prevZoneType,           //  eZONETYPE
            pUser->GetZoneKey(),    pPrevZone->GetKey());   //  ZoneKey
	RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		pUser->BeginTransaction(TR_LEAVE_MISSION_FOR_VILLAGE);

		pUser->SetTRRoomKey(pPrevZone->GetKey());
		// ������ �̵� �α�
		//if(eZONETYPE_VILLAGE == ztZONE)

		GAMELOG->MoveVillage(pUser, wFromMapCode, pUser->GetSelectedCharMapCode(), TRUE);

		// ������ �̵� �α� ��
	}
	else
	{
		MSG_CG_ZONE_MISSION_LEAVE_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

HANDLER_CG_IMPL(CG_ZONE_HUNTING_LEAVE_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
    // (WAVERIX) (090624) (CHANGES) Remove unused section 'Hunting'
    ASSERT("Function Not Suported!!");
    return;
//	MSG_CG_ZONE_HUNTING_LEAVE_SYN* pRecvMsg = (MSG_CG_ZONE_HUNTING_LEAVE_SYN *)pMsg;
//
//#ifdef __NA_PACKETFLOW_TEST
//	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_HUNTING_LEAVE_SYN)");
//#endif
//
//	if(pUser->IsBeginTransaction())
//	{
//		MSG_CG_ZONE_HUNTING_LEAVE_NAK msg;
//		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
//		pUser->SendPacket(&msg, sizeof(msg));
//		return;
//	}
//
//	//���� �ִ� ���� ���ù��� �ƴϸ�.. NAK
//	if(pUser->GetZoneType() != eZONETYPE_HUNTING)
//	{
//		MSG_CG_ZONE_HUNTING_LEAVE_NAK msg;
//		msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;			//�߸��� �� ���� ��Ŷ ����..
//		pUser->SendPacket(&msg, sizeof(msg));
//		return;
//	}
//
//	Village* pVillage = g_ZoneManager.FindVillage(pUser->GetSelectedCharMapCode());
//	if(!pVillage)
//	{
//		MSG_CG_ZONE_HUNTING_LEAVE_NAK msg;
//		msg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
//		pUser->SendPacket(&msg, sizeof(msg));
//		return;
//	}
//
//	WORD wFromMapCode = pVillage->GetMapCode();
//
//	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
//		MoveZonePolicy<MZTF_ROOM_TO_STATIC>::AlignInfo(
//			pUser
//			//{		From		|			To		}
//			, eZONETYPE_HUNTING,	eZONETYPE_VILLAGE		//	eZONETYPE
//			, pUser->GetZoneKey(),	pVillage->GetKey()		//	ZoneKey
//			);
//	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
//	if(RC::RC_ROOM_SUCCESS == rt)
//	{
//		pUser->BeginTransaction(TR_LEAVE_HUNTING_FOR_VILLAGE);
//		pUser->SetTRRoomKey(pVillage->GetKey());
//
//		// ������ �̵� �α�
//		GAMELOG->MoveVillage(pUser, wFromMapCode, pUser->GetSelectedCharMapCode(), TRUE);
//
//		// ������ �̵� �α� ��
//	}
//	else
//	{
//		MSG_CG_ZONE_HUNTING_LEAVE_NAK msg;
//		msg.m_dwErrorCode = rt;
//		pUser->SendPacket(&msg, sizeof(msg));
//	}
}

HANDLER_CG_IMPL(CG_ZONE_PVP_LEAVE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_PVP_LEAVE_SYN* pRecvMsg = (MSG_CG_ZONE_PVP_LEAVE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_PVP_LEAVE_SYN)");
#endif


	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_PVP_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	//���� �ִ� ���� PVP���� �ƴϸ�.. NAK
	if(pUser->GetZoneType() != eZONETYPE_PVP)
	{   // (WAVERIX) (090625) (CHANGES) PVP Leave ���н� �߸��� ��Ŷ ������ ��� ����
		//MSG_CG_ZONE_HUNTING_LEAVE_NAK msg;
        MSG_CG_ZONE_PVP_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;			//�߸��� �� ���� ��Ŷ ����..
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	Village* pVillage = g_ZoneManager.FindVillage(pUser->GetSelectedCharMapCode());
	if(!pVillage)
	{
		MSG_CG_ZONE_PVP_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_ROOM_TO_STATIC>::AlignInfo(
			pUser
			//{		From		|			To		}
			, eZONETYPE_PVP,		eZONETYPE_VILLAGE	//	eZONETYPE
			, pUser->GetZoneKey(),	pVillage->GetKey()	//	ZoneKey
			);
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		pUser->BeginTransaction(TR_LEAVE_PVP_FOR_VILLAGE);
		pUser->SetTRRoomKey(pVillage->GetKey());
	}
	else
	{
		MSG_CG_ZONE_PVP_LEAVE_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

HANDLER_CG_IMPL(CG_ZONE_CHUNTING_LEAVE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_CHUNTING_LEAVE_SYN* pRecvMsg = (MSG_CG_ZONE_CHUNTING_LEAVE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_CHUNTING_LEAVE_SYN)");
#endif


	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_CHUNTING_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	//���� �ִ� ���� �������ù��� �ƴϸ�.. NAK
	if(pUser->GetZoneType() != eZONETYPE_CHUNTING)
	{
		MSG_CG_ZONE_CHUNTING_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;			//�߸��� �� ���� ��Ŷ ����..
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	Village* pVillage = g_ZoneManager.FindVillage(pUser->GetSelectedCharMapCode());
	if(!pVillage)
	{
		MSG_CG_ZONE_CHUNTING_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	WORD wFromMapCode = pVillage->GetMapCode();

	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_CHUNTING_TO_VILLAGE>::AlignInfo(
			pUser
			//{		From		|			To		}
			, eZONETYPE_CHUNTING,	eZONETYPE_VILLAGE		//	eZONETYPE
			, pUser->GetZoneKey(),	pVillage->GetKey()		//	ZoneKey
			);
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());
	if(RC::RC_ROOM_SUCCESS == rt)
	{
		pUser->BeginTransaction(TR_LEAVE_CHUNTING_FOR_VILLAGE);
		pUser->SetTRRoomKey(pVillage->GetKey());

		// ������ �̵� �α�
		GAMELOG->MoveVillage(pUser, wFromMapCode, pUser->GetSelectedCharMapCode(), TRUE);

		// ������ �̵� �α� ��
	}
	else
	{
		MSG_CG_ZONE_CHUNTING_LEAVE_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

HANDLER_CG_IMPL(CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN* pRecvMsg = (MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_INSTANCE_DUNGEON_LEAVE_SYN)");
#endif

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	//���� �ִ� ���� �������ù��� �ƴϸ�.. NAK
	if(pUser->GetZoneType() != eZONETYPE_INSTANCE)
	{
		MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK msg;
		msg.m_dwErrorCode = RC::RC_ROOM_INVALIDROOMTYPE;			//�߸��� �� ���� ��Ŷ ����..
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	ZoneInterface* pPreZone = NULL;
	pPreZone = (ZoneInterface*)g_ZoneManager.FindVillage(pUser->GetSelectedCharMapCode());
	if(!pPreZone)
	{
		pPreZone = (ZoneInterface*)g_ZoneManager.FindHuntingField(pUser->GetSelectedCharMapCode());
		if(!pPreZone)
		{
			MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK msg;
			msg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
			pUser->SendPacket(&msg, sizeof(msg));
			return;
		}
	}

	
	WORD wFromMapCode = pPreZone->GetMapCode();
	
	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_ROOM_TO_STATIC>::AlignInfo(
			pUser
			//{		From		|			To		}
			, eZONETYPE_INSTANCE,	pPreZone->GetType()		//	eZONETYPE
			, pUser->GetZoneKey(),	pPreZone->GetKey()		//	ZoneKey
			);
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, RoomArgument::GetEmpty());

	if(RC::RC_ROOM_SUCCESS == rt)
	{
		pUser->BeginTransaction(TR_LEAVE_INSTANCE_DUNGEON);
		pUser->SetTRRoomKey(pPreZone->GetKey());

		// ������ �̵� �α�
	
		GAMELOG->MoveVillage(pUser, wFromMapCode, pUser->GetSelectedCharMapCode(), TRUE);
	
		// ������ �̵� �α� ��
	}
	else
	{
		MSG_CG_ZONE_INSTANCE_DUNGEON_LEAVE_NAK msg;
		msg.m_dwErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
	}
}

HANDLER_CG_IMPL(CG_ZONE_HUNTING_RELAY_CREATE_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
    // (WAVERIX) (090624) (CHANGES) Remove unused section 'Hunting'
    ASSERT("Function Not Suported!!");
    return;
}

// ���ÿ��� �������� ������ ����
HANDLER_CG_IMPL(CG_ZONE_HUNTING_RELAY_JOIN_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
    // (WAVERIX) (090624) (CHANGES) Remove unused section 'Hunting'
    ASSERT("Function Not Suported!!");
    return;
//	MSG_CG_ZONE_HUNTING_RELAY_JOIN_SYN* pRecvMsg = (MSG_CG_ZONE_HUNTING_RELAY_JOIN_SYN *)pMsg;
//
//#ifdef __NA_PACKETFLOW_TEST
//	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_HUNTING_RELAY_JOIN_SYN)");
//#endif
//
//
//	if(pUser->IsBeginTransaction())
//	{
//		MSG_CG_ZONE_HUNTING_RELAY_JOIN_NAK msg;
//		msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
//		pUser->SendPacket(&msg, sizeof(msg));
//		return;
//	}
//
//	// CanJoin�� �ϱ����� General���·� �����.
//	eROOMSTATE_TYPE OldRoomState = eROOMSTATE_NONE;
//	HuntingRoom* pHuntingRoom = g_ZoneManager.FindHunting(pRecvMsg->m_HuntingKey);
//	if(pHuntingRoom) 
//	{
//		OldRoomState = pHuntingRoom->GetRoomState();
//		pHuntingRoom->SetRoomState(eROOMSTATE_GENERNAL);
//	}
//
//	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
//		MoveZonePolicy<MZTF_HUNTING_TO_HUNTING>::AlignInfo(
//			pUser
//			//{		From		|			To		}
//			, eZONETYPE_HUNTING,	eZONETYPE_HUNTING		//	eZONETYPE
//			, pUser->GetZoneKey(),	pRecvMsg->m_HuntingKey	//	ZoneKey
//			);
//	RoomArgument args((eZONEPUBLIC)pRecvMsg->m_byRoomPublic, (pRecvMsg->m_byRoomPublic==eZONEPUBLIC_PUBLIC?NULL:pRecvMsg->m_pszRoomPWD));
//	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, args);
//	if(RC::RC_ROOM_SUCCESS == rt)
//	{
//		pUser->SetTRRoomKey(pRecvMsg->m_HuntingKey);
//		// ���忡��
//		MSG_CG_ZONE_HUNTING_RELAY_JOIN_ACK jmsg;
//		pUser->SendPacket(&jmsg, sizeof(jmsg));
//	}
//	else
//	{
//		// ������������
//		MSG_CG_ZONE_HUNTING_RELAY_JOIN_NAK msg;
//		msg.m_dwErrorCode = rt;
//		pUser->SendPacket(&msg, sizeof(msg));
//	}
//
//	if(pHuntingRoom) pHuntingRoom->SetRoomState(OldRoomState);
}


//��Ż�� �̿��Ͽ� �ʵ尣, ������ �̵�.
HANDLER_CG_IMPL(CG_ZONE_PORTAL_FIELD_MOVE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN* pRecvMsg = (MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN *)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_PORTAL_FIELD_MOVE_SYN)");
#endif

	BOOL bIsValidSize = (BOOL) (sizeof(MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN) == wSize);
	if(!bIsValidSize)
	{
		SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ZONE_PORTAL_FIELD_MOVE_SYN"));
		return;
	}
	WORD wPortalIndex = pRecvMsg->m_wIndex;

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    WORD party_key = pUser->GetPartyState().GetPartyKey();
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room && party_room->IsStartWaitingRoom() == true)
    {
        return;
    }

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	if(pUser->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
	{
		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        if(pUser->GetBehaveState() == PLAYER_BEHAVE_RESERVATION_BATTLE_GROUND_ZONE)
            msg.m_byErrorCode = RC::RC_ROOM_BATTLE_GROUND_RESERVATION;
        else
#endif
		msg.m_byErrorCode = RC::RC_ROOM_CANNOT_WAYPOINT_PLAYERSTATE;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}
	
	//1. ���� ��Ż�� FromField�ڵ带 ����.
	sMAP_ENTERENCE_PORTAL* pFromInfo = MapInfoParser::Instance()->GetMapPortal(wPortalIndex);
	if(!pFromInfo)
	{
		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_INVALID_MAPCODE;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	ZoneInterface* pFromZone = g_ZoneManager.FindStaticZone(pFromInfo->wWorldFrom);
	ZoneInterface* pToZone = g_ZoneManager.FindStaticZone(pFromInfo->wWorldTo);
	if(!pFromZone || !pToZone)
	{
		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
		msg.m_byErrorCode = RC::RC_ROOM_NOTEXISTVILLAGE;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	RC::eROOM_RESULT rt = pToZone->CanJoin(pUser, (eZONEPUBLIC)0, NULL);
	if(RC::RC_ROOM_SUCCESS != rt)
	{
		MSG_CG_ZONE_PORTAL_FIELD_MOVE_NAK msg;
		msg.m_byErrorCode = rt;
		pUser->SendPacket(&msg, sizeof(msg));
		return;
	}

	// ���� �������� �̼� �Ϸ� ���� üũ + �Ҹ� ��� ó��
	MSG_AG_ZONE_PORTAL_FIELD_MOVE_SYN msg;
	msg.m_wIndex = wPortalIndex;
	pUser->SendToLinkedServer(&msg, sizeof(msg));
}

//{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
HANDLER_CG_IMPL(CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN)
{
    __TOUCHED((pMsg, wSize));
    typedef MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN   RECEIVED_PACKET;
    const MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN* pRecvMsg =
        static_cast<MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN*>(pMsg);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (BattleGroundManager::Instance()->IsValidReservationMember(pUser->GetUserKey()))
    {
        MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK msg;
        msg.errorType = msg.eRESULT_ROOM;
        msg.errorCode = RC::RC_ROOM_BATTLE_GROUND_RESERVATION;
        pUser->SendPacket(&msg, sizeof(msg));
        return;
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    WORD party_key = pUser->GetPartyState().GetPartyKey();
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(party_key);
    if (party_room && party_room->IsStartWaitingRoom() == true)
    {
        return;
    }

    const RECEIVED_PACKET::eSYNC syncType = RECEIVED_PACKET::eSYNC(pRecvMsg->syncType);
    { // check integrity
        BOOLEAN requestCheck = syncType == pRecvMsg->eSYNC_TEMP_PORTAL ||
                               syncType == pRecvMsg->eSYNC_FIXED_PORTAL;

        if(requestCheck == FALSE || pRecvMsg->GetSize() != wSize)
            return;
    };

    const eZONE_STATE zoneState = pUser->GetZoneState();
    const BOOLEAN acceptableState =
        (ePRS_AT_ZONE & zoneState) && (ePRS_AT_CHARSELECT != zoneState);
    if(!acceptableState)
    {   // irregular
        return;
    }

    RC::eROOM_RESULT result = RC::RC_ROOM_SUCCESS;
    if(syncType == pRecvMsg->eSYNC_TEMP_PORTAL)
    {   // Sub Mission, etc...
        result = PortalManager::RequestEnterZoneTempPortal(pUser, pRecvMsg->portalKey);
    }

    if(syncType == pRecvMsg->eSYNC_FIXED_PORTAL)
    {   // Main Mission, etc...
        result = PortalManager::RequestEnterZoneFixedPortal(pUser,
                                                            pRecvMsg->mapEnterancePortalIndex);
    }

    if(result != RC::RC_ROOM_SUCCESS)
    {
        MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_NAK msg;
        msg.errorType = msg.eRESULT_ROOM;
        msg.errorCode = (WORD)result;
        pUser->SendPacket(&msg, sizeof(msg));
    }
}
//}__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__

//_NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
HANDLER_CG_IMPL(CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_SYN)
{
#if !defined(_NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION)
    __UNUSED((pUser, pMsg, wSize));
    ASSERT(!"function not supported in this version");
    return;
#endif
#ifdef _NA003129_QUEST_PORTAL_CONVENIENCE_EXTENTION
    const MSG_CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_SYN* const recv_msg = \
        static_cast<MSG_CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_SYN*>(pMsg);
    //
    MSG_CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_NAK msg_nak;
    if (PortalManager::RequestWithdrawPortalMember(pUser, &msg_nak.error_code) == false)
    {
        pUser->SendPacket(&msg_nak, sizeof(msg_nak));
    };
#endif
}

HANDLER_CG_IMPL(CG_GM_CHANNELINFO_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_GM_CHANNELINFO_SYN* pRecvMsg = (MSG_CG_GM_CHANNELINFO_SYN*)pMsg;


	MSG_CG_GM_CHANNELINFO_ACK AckMsg;
	AckMsg.m_dwChannelUserNum = UserManager::Instance()->GetNumberOfUsers();
	AckMsg.m_wHuntingRoomCount = (WORD)g_ZoneManager.GetNumberOfHuntingRooms();


	AckMsg.m_wMissionRoomCount = (WORD)g_ZoneManager.GetNumberOfMissionRooms();

	pUser->SendPacket(&AckMsg, sizeof(AckMsg));
}

HANDLER_CG_IMPL(CG_GM_WARP_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
	MSG_CG_GM_WARP_SYN* pRecvMsg = (MSG_CG_GM_WARP_SYN*)pMsg;

}

HANDLER_CG_IMPL(CG_ARMORKIT_FINDHACK)
{	//OnCG_ARMORKIT_FINDHACK_SYN(User *pUser, MSG_BASE *pMsg, WORD wSize);
    __TOUCHED((pMsg, wSize));
	MSG_CG_ARMORKIT_FINDHACK_SYN* pRecvMsg = (MSG_CG_ARMORKIT_FINDHACK_SYN*)pMsg;

	// ������ ���� �׳� ����
	MSG_AM_ARMORKIT_FINDHACK_SYN	sendMsg;

	//memset(sendMsg.szIP, 0, sizeof(sendMsg.szIP));
	memcpy(sendMsg.szIP, pUser->GetIP(), sizeof(sendMsg.szIP));
	sendMsg.HackType	= pRecvMsg->HackType;
	sendMsg.Size		= pRecvMsg->Size;
	sendMsg.dwGuid		= pUser->GetUserGUID();
	//memset(sendMsg.Buf, 0, sizeof(sendMsg.Buf));
	memcpy(sendMsg.Buf, pRecvMsg->Buf, sizeof(sendMsg.Buf));

	// �״�� ������
	AgentServer* pAgentServer = AgentServer::GetInstance();
	ServerSession* pMasterServer = pAgentServer->SessionMasterServer();
	if(!pMasterServer)
	{
		//MessageOut(eCRITICAL_LOG,  "ArmorKit FindHack Error - No Master(User %s)", pUser->GetCharName().c_str());
		return;
	}
	pMasterServer->Send((BYTE*)&sendMsg, sizeof(sendMsg));

	//MessageOut(eFULL_LOG,  "ArmorKit Find Hacking tool(User %u/%s)", pUser->GetGUID(), pUser->GetCharName().c_str());
}

HANDLER_CG_IMPL(CG_ZONE_RETURN_VILLAGE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_RETURN_VILLAGE_SYN* pRecvMsg = (MSG_CG_ZONE_RETURN_VILLAGE_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_RETURN_VILLAGE_SYN)");
#endif

	BOOL bIsValidSize = (BOOL) (sizeof(MSG_CG_ZONE_RETURN_VILLAGE_SYN) == wSize);
	if(!bIsValidSize)
	{
		SUNLOG(eDEV_LOG, _T("[RNDPROTOCOL_REPORT][SIZE] MSG_CG_ZONE_RETURN_VILLAGE_SYN"));
		return;
	}

	if(pUser->IsBeginTransaction())
	{
		MSG_CG_ZONE_RETURN_VILLAGE_NAK nmsg;
		nmsg.m_byErrorCode	= RC::RC_CONNECTION_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	if( pUser->GetZoneType() != eZONETYPE_FIELD &&
        pUser->GetZoneType() != eZONETYPE_DOMINATION_FIELD &&
        pUser->GetZoneType() != eZONETYPE_SPA_FIELD)    //_NA_008069_20150210_ADD_SPA_SYSTEM
	{
		MSG_CG_ZONE_RETURN_VILLAGE_NAK nmsg;
		nmsg.m_byErrorCode	= RC::RC_ROOM_INVALID_ROOM_TYPE;	// ���� ��Ÿ���� �ʵ尡 �ƴϴ�. ���� �����Ұ�.
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	// ���� �������� ���� ���� üũ.
	MSG_AG_ZONE_RETURN_VILLAGE_SYN msg;
	pUser->SendToLinkedServer(&msg, sizeof(msg));
}

HANDLER_CG_IMPL(CG_ZONE_MISSION_RELAY_CREATE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_MISSION_RELAY_CREATE_SYN* pRecvMsg = (MSG_CG_ZONE_MISSION_RELAY_CREATE_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_MISSION_RELAY_CREATE_SYN)");
#endif


	MSG_CG_ZONE_MISSION_RELAY_CREATE_NAK NakMsg;

	if(pUser->IsBeginTransaction())
	{
		NakMsg.m_ErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&NakMsg, sizeof(NakMsg));
		return;
	}

    // (WAVERIX) (090610) (BUG-FIX) AgentServer downfall problem, loop hole protector
    //MissionRoom* pMissionRoom = (MissionRoom*)g_ZoneManager.FindZone(pUser->GetZoneKey());
    MissionRoom* pMissionRoom = 0;
    {
        ZoneInterface* pZoneInterface = g_ZoneManager.FindZone(pUser->GetZoneKey());
        if(pZoneInterface && pZoneInterface->GetType() == eZONETYPE_MISSION)
            pMissionRoom = static_cast<MissionRoom*>(pZoneInterface);
    }

	if(pMissionRoom == 0)
	{
		NakMsg.m_ErrorCode = RC::RC_ROOM_NOTEXISTROOM;
		pUser->SendPacket(&NakMsg, sizeof(NakMsg));
		return;
	}

    PolicymentForRoom& rPolicyment = pMissionRoom->RoomPolicyment;
    if(rPolicyment.FIELD & rPolicyment.QUEST_PORTAL_CONTROL)
    {
        NakMsg.m_ErrorCode = RC::RC_ROOM_CANNOT_CREATE_RELAY_ROOM_BY_PORTAL_ENTERED;
        pUser->SendPacket(&NakMsg, sizeof(NakMsg));
        return;
    }

	LobbyRoom* pLobbyRoom = NULL;
	RC::eROOM_RESULT rt = g_ZoneManager.CreateRelayMissionLobbyRoom(pMissionRoom, pUser, pLobbyRoom);

	if(RC::RC_ROOM_SUCCESS == rt)
	{
		pUser->BeginTransaction(TR_LEAVE_MISSION_FOR_LOBBY);
		pUser->SetTRRoomKey(pLobbyRoom->GetKey());

		// �������� ���� �̼��� ���������
		MSG_CG_ZONE_MISSION_RELAY_CREATE_BRD BrdMsg;
		BrdMsg.m_LobbyKey = pLobbyRoom->GetKey();
		pMissionRoom->SendToAll(&BrdMsg, sizeof(BrdMsg), pUser->GetUserKey());
	}
	else
	{
		// �̼Ƿκ� ��������
		NakMsg.m_ErrorCode = rt;
		pUser->SendPacket(&NakMsg, sizeof(NakMsg));
	}
}

// ���ÿ��� �������� ������ ����
HANDLER_CG_IMPL(CG_ZONE_MISSION_RELAY_JOIN_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_MISSION_RELAY_JOIN_SYN* pRecvMsg = (MSG_CG_ZONE_MISSION_RELAY_JOIN_SYN*)pMsg;

#ifdef __NA_PACKETFLOW_TEST
	SUNLOG(eFULL_LOG, "HANDLER_FROMCLIENT_IMPL(CG_ZONE_MISSION_RELAY_JOIN_SYN)");
#endif


	MSG_CG_ZONE_LOBBY_JOIN_NAK NakMsg;

	if(pUser->IsBeginTransaction())
	{
		NakMsg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&NakMsg, sizeof(NakMsg));
		return;
	}

	MissionRoom *pMissionRoom = (MissionRoom*)g_ZoneManager.FindZone(pUser->GetZoneKey());
	if(!pMissionRoom)
	{
		NakMsg.m_dwErrorCode = RC::RC_ROOM_NOTEXISTROOM;
		pUser->SendPacket(&NakMsg, sizeof(NakMsg));
		return;
	}

	CheckMoveZonePolicy* const pCheckMoveZonePolicy =
		MoveZonePolicy<MZTF_ROOM_TO_LOBBY>::AlignInfo(
			pUser
			//{		From		|			To		}
			, eZONETYPE_MISSION,	eZONETYPE_LOBBY			//	eZONETYPE
			, pUser->GetZoneKey(),	pRecvMsg->m_LobbyKey	//	ZoneKey
			);
	RoomArgument args(pMissionRoom->GetZonePublic(), pMissionRoom->GetRoomPWD());
	const RC::eROOM_RESULT rt = g_ZoneManager.MoveZone(pCheckMoveZonePolicy, args);

	if(RC::RC_ROOM_SUCCESS == rt)
	{
		pUser->BeginTransaction(TR_LEAVE_MISSION_FOR_LOBBY);
		pUser->SetTRRoomKey(pRecvMsg->m_LobbyKey);
	}
	else
	{
		NakMsg.m_dwErrorCode = rt;
		pUser->SendPacket(&NakMsg, sizeof(NakMsg));
	}
}


//���� ���� ž
HANDLER_CG_IMPL(CG_ZONE_INSTANCE_DUNGEON_REQUEST_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_INSTANCE_DUNGEON_REQUEST_SYN* pRecvMsg = (MSG_CG_ZONE_INSTANCE_DUNGEON_REQUEST_SYN*)pMsg;

#ifdef _NA_LEAVE_PARTY_AT_USE_ITEM_TO_MISSION
    if (pUser->GetPartyState().IsPartyMember() == true)
    {
        AgentPartyManager * const party_manager = AgentPartyManager::Instance();
        RC::ePARTY_RESULT result_code;
        result_code = party_manager->LeaveParty(pUser, eREASON_LEAVE_PARTY_INTEND);

        if (result_code != RC::RC_PARTY_SUCCESS)
        {
            SUNLOG(eCRITICAL_LOG,
                __FUNCTION__" : Fail levave party : code : %d",
                result_code);
        }
    }
#endif

	if(pUser->GetLockItemPosForInstanceDungeon() != INVALID_POSTYPE_VALUE)
		return;

	pUser->SetLockItemPosForInstanceDungeon(pRecvMsg->m_ItemPos);

	MSG_AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_SYN msg;
	msg.m_ItemPos = pRecvMsg->m_ItemPos;
	pUser->SendToLinkedServer(&msg, sizeof(msg));
}

HANDLER_CG_IMPL(CG_CONNECTION_CRASH_REPORT_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_CONNECTION_CRASH_REPORT_SYN*	pRecvMsg = (MSG_CG_CONNECTION_CRASH_REPORT_SYN*)pMsg;
	//if(!pUser) return;	// �̰�... NULL�� �� �� ����. ---;;;
    // (CHANGES) (f100607.4L) fix up a buffer overrun problem
	//BuildVer
	TCHAR szBuildVer[MAX_BUILD_VERSION_SIZE+1];
	strncpy(szBuildVer, pRecvMsg->m_CrashInfo.m_szBuildVer, _countof(szBuildVer));
	szBuildVer[_countof(szBuildVer) - 1] = '\0';
	//ModuleName
	TCHAR szModuleName[MAX_MODULE_NAME_SIZE+1];
	strncpy(szModuleName, pRecvMsg->m_CrashInfo.m_szModuleName, _countof(szModuleName));
	szModuleName[_countof(szModuleName) - 1] = '\0';
	//OS
	TCHAR szOS[MAX_OS_NAME_SIZE+1];
	strncpy(szOS, pRecvMsg->m_ComputerInfo.m_szOS, _countof(szOS));
	szOS[_countof(szOS) - 1] = '\0';
	//Process
	TCHAR szProcess[MAX_PROCESS_NAME_SIZE+1];
	strncpy(szProcess, pRecvMsg->m_ComputerInfo.m_szProcess, _countof(szProcess));
	szProcess[_countof(szProcess) - 1] = '\0';
	//PhysicalMemory
	TCHAR szPhysicalMemory[MAX_PHYSICAL_MEMORY_SIZE+1];
	strncpy(szPhysicalMemory, pRecvMsg->m_ComputerInfo.m_szPhysicalMemory, _countof(szPhysicalMemory));
	szPhysicalMemory[_countof(szPhysicalMemory) - 1] = '\0';
	//DirectX
	TCHAR szDirectXVersion[MAX_DIRECTX_NAME_SIZE+1];
	strncpy(szDirectXVersion, pRecvMsg->m_ComputerInfo.m_szDirectXVersion, _countof(szDirectXVersion));
	szDirectXVersion[_countof(szDirectXVersion) - 1] = '\0';
	//Description
	TCHAR szDescription[MAX_DESCRIPT_SIZE+1];
	strncpy(szDescription, pRecvMsg->m_ComputerInfo.m_szDescription, _countof(szDescription));
	szDescription[_countof(szDescription) - 1] = '\0';
	//DisplayMemory
	TCHAR szDisplayMemory[MAX_DISPLAY_MEMORY_SIZE+1];
	strncpy(szDisplayMemory, pRecvMsg->m_ComputerInfo.m_szDisplayMemory, _countof(szDisplayMemory));
	szDisplayMemory[_countof(szDisplayMemory) - 1] = '\0';
	//Driver
	TCHAR szDriverVersion[MAX_DRIVER_VERSION_SIZE+1];
	strncpy(szDriverVersion, pRecvMsg->m_ComputerInfo.m_szDriverVersion, _countof(szDriverVersion));
	szDriverVersion[_countof(szDriverVersion) - 1] = '\0';

	SYSTEMTIME	time;
	util::YYYYMMDDHHMMSSToSYSTEMTIME(pRecvMsg->m_CrashInfo.m_dw64CrashTime, time);

	// ������ SecureLog�� ����Ѵ�.
	SECURELOG(SECURELOG_USERINFO(pUser, eABUSE_CRASH_REPORT, eABUSE_SUB_CRASH_REPORT),
			"CrashAddress = %u, "
			"ModuleStartAddress = %u, "
			"ModuleEndAddress = %u, "
			"ModuleCrashOffset = %u, "
			"ModuleName = %s, "
			"BuildVer = %s, "
			"CrashTime = %d/%d/%d-%d:%d:%d, "
			"OS = %s, "
			"Processor = %s, "
			"PhysicalMemory = %s , "
			"DirectXVersion = %s , "
			"Description = %s, "
			"DisplayMemory = %s, "
			"DriverVersion = %s ",
			pRecvMsg->m_CrashInfo.m_dwCrahAddress,
			pRecvMsg->m_CrashInfo.m_dwModuleStartAddress,
			pRecvMsg->m_CrashInfo.m_dwModuleEndAddress,
			pRecvMsg->m_CrashInfo.m_dwModuleCrashOffset,
			szModuleName,
			szBuildVer,
			time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond,
			szOS,
			szProcess,
			szPhysicalMemory,
			szDirectXVersion,
			szDescription,
			szDisplayMemory,
			szDriverVersion);
}

#include "Party\Party.h"
HANDLER_CG_IMPL(CG_ZONE_EX_LOBBY_NAME_CHANGE_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_SYN* pRecvMsg = (MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_SYN *)pMsg;

	if(sizeof(*pRecvMsg) != wSize)
		return;

	MSG_CG_ZONE_EX_LOBBY_NAME_CHANGE_NAK nmsg;

	ZoneInterface *pZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
	if(!pZone)
	{
		nmsg.m_dwErrorCode = RC::RC_ROOM_FAILED;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	if(pUser->IsBeginTransaction())
	{		
		nmsg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}	
	
	if (pZone->GetType() != eZONETYPE_LOBBY)
	{
		nmsg.m_dwErrorCode = RC::RC_ROOM_FAILED;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	RoomInterface* pRoom = (RoomInterface*)pZone;

	if(pRoom->GetMaster() != pUser)
	{
		nmsg.m_dwErrorCode = RC::RC_ROOM_ISNOTMATERPLAYER;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	if(((LobbyRoom*)pZone)->ChangeRoomTitle(pRecvMsg->m_pszRoomTitle) == FALSE)	
	{
		nmsg.m_dwErrorCode = RC::RC_ROOM_CANNOT_USE_ROOM_TITLE;
		pUser->SendPacket(&nmsg, sizeof(nmsg));
		return;
	}

	// PvP���� ��Ƽ�� �ƴϴ�
	ChangeRoomName Op(pRecvMsg->m_pszRoomTitle);
	pZone->Foreach(Op);
}

HANDLER_CG_IMPL(CG_ZONE_EX_TUTORIAL_FIELD_LEAVE_SYN)
{
    __TOUCHED((pMsg, wSize));
    // (WAVERIX) (090114) add ZoneType Check Constraints
    // case: �ǵ����� ���� ������ �ð� ���� 'ĳ���� ����'��ư����
    //(Ʃ�丮�� �ʵ� -> ĳ���� ����) ������ ���� 'Ʃ�丮�� ������'��ư�� ���� ���,
    //'Zone�� ���� ���, MapCode�� �����ؾ� �Ѵ�'��� �ܾ��� ������ ��Ȳ ����.
    ZoneInterface* zone = g_ZoneManager.FindZone(pUser->GetZoneKey());
    // old code "if(!zone)"
    //if(FALSE == (zone && zone->GetType() == eZONETYPE_FIELD))
    //    return;
    const sMAPINFO* map_info = 0;
    BOOLEAN isAcceptable = zone && !!(map_info = zone->GetMapInfo());
    if(isAcceptable == 0)
        return;

    eZONETYPE zoneType = eZONETYPE(map_info->byMKind);
    if(eZONETYPE_TUTORIAL_FIELD == zoneType)
    {   // SendTo GameServer, Action: Leave Tutorial Field, (090520 ����) �ش� ����� Ŭ�󿡼� ���ŵ�.
        MSG_AG_ZONE_TUTORIAL_SKIP_SYN msgSYN;
        pUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));
    }
}

HANDLER_CG_IMPL(CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_SYN)
{
    __TOUCHED((pMsg, wSize));
    // (WAVERIX) (090520) (NOTE) �� ����, �� �̻� ������ �ʴ� ����̴�.
	// ���۸����� ���, InitCharInfo�� ����ϸ� ���ٵ�... ��� 10001�� ���ϵǾ� ����. ���� ��û �ʿ�
	const MAPCODE HARDCODED_TUTORIAL_MISSION_MAPCODE	= 31;
	const DWORD CHAR_VILLAGE_TRINGGEL_BITMAP = (1 << eCHAR_BERSERKER | 1 << eCHAR_ELEMENTALIST);
	const DWORD CHAR_VILLAGE_VALDCHEN_BITMAP = (1 << eCHAR_DRAGON | 1 << eCHAR_VALKYRIE |
                                                1 << eCHAR_SHADOW);

	ZoneInterface* pZone = g_ZoneManager.FindZone(pUser->GetZoneKey());
	if(!pZone)
	{
		// ��� ó���� ��?
		return;
	}

	MAPCODE mapCode = pZone->GetMapCode();

	if(HARDCODED_TUTORIAL_MISSION_MAPCODE == mapCode)
	{
		// SendTo GameServer
		// Ʃ�丮�� �̼��� ������, ĳ���� ���۸����� �̵���Ų��.
		DWORD charClass = pUser->GetCharClass();
		charClass = 1 << charClass;

		MAPCODE returnMapCode
            = CHAR_VILLAGE_TRINGGEL_BITMAP & charClass ? HardCode::MAPCODE_for_TRINGGEL
            : CHAR_VILLAGE_VALDCHEN_BITMAP & charClass ? HardCode::MAPCODE_for_VALDCHEN
			: 0;

		if(returnMapCode)
		{
			//Ŭ���̾�Ʈ���� �� ��ȯ ���� ���ڵ带 �ذ��ؾ� �Ѵ�.
			pUser->SetSelectedCharMapCode(returnMapCode);
			MSG_CG_ZONE_EX_TUTORIAL_MISSION_LEAVE_CMD msgCMD;
			msgCMD.m_MapCode = returnMapCode;
			pUser->SendPacket(&msgCMD, sizeof(msgCMD));

			MSG_CG_ZONE_MISSION_LEAVE_SYN msgSYN;
			msgSYN.m_Reason = MSG_CG_ZONE_MISSION_LEAVE_SYN::REASON_EXPULSION;	// ���� ��带 �־� �� ��...
			OnCG_ZONE_MISSION_LEAVE_SYN(pUser, &msgSYN, sizeof(msgSYN));
		}
	}
}

HANDLER_CG_IMPL(CG_ETC_GET_BATTLE_OPEN_TIME_SYN)
{
    __TOUCHED((pMsg, wSize));
#if !defined(__000000_20080811_BUG_FIX_VIEW_JOIN_TOP_TIME)
	#error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

	//	(WAVERIX) (081016) (COMMENT) - '��ž+Ÿ��' ���� ��û�� �Ϳ� ���� ���� ��Ŷ���� ������

	MSG_CG_ETC_GET_BATTLE_OPEN_TIME_ACK msgACK;
	msgACK.m_eJoinState = eJION_TOP_STATE_NONE;
	msgACK.m_JoinOverTime = INFINITE;
	ZeroMemory(&msgACK.m_SSQTicketInfo, sizeof(msgACK.m_SSQTicketInfo));

	//	'��ž' control
	FLOWCONTROL_START()
    {
    #if __ITIMERBASE_VERSION_CRTL >= 0x0120
    //----------------------------------------------------------------------------------------------
        if (g_BattleOpenTimeMng.is_loaded() == false) {
            break;
        }
        util::TimeInfoPerFrame time_info;
        util::TimeSync::UpdateTimeInfoPerFrame(&time_info);
        g_BattleOpenTimeMng.SetupMessage(time_info, &msgACK);
    //----------------------------------------------------------------------------------------------
    #else // __ITIMERBASE_VERSION_CRTL < 0x0120
    //----------------------------------------------------------------------------------------------

		typedef CurseTowerTimeParser CTTParser;
		CurseTowerTimeParser* const pCurseTowerTimeParser = CurseTowerTimeParser::Instance();
		BATTLE_OPEN_TIME_LIST_HASH* const pListHash = pCurseTowerTimeParser->GetListHash();
		if(!pListHash)
			break;

		BATTLE_OPEN_TIME_LIST_HASH::const_iterator hash_it = pListHash->find(CTTParser::TIME_CHECK_CURSE_TOWER);
		if(hash_it == pListHash->end())
			break;

		st_BattleOpenTimeList* const pList = (*pListHash)[CTTParser::TIME_CHECK_CURSE_TOWER];
		if(!pList)
		{
			SUNLOG(eCRITICAL_LOG, "[HANDLER_CG_IMPL(CG_ETC_GET_BATTLE_OPEN_TIME_SYN)] st_BattleOpenTimeList is null ");
			break;
		}

		DWORD minV = INFINITE;
		BATTLE_OPEN_TIME_LIST::const_iterator it=pList->m_List.begin(), itend=pList->m_List.end();
		while(it!=itend)
		{
			typedef st_BattleOpenTimeNode stBOTN;
			stBOTN* const pBOTN = *it;
			const WORD nOpenState = pBOTN->OpenState;
			if(nOpenState == stBOTN::STATE_BEFORE_OPEN_5MIN
				|| nOpenState == stBOTN::STATE_BEFORE_OPEN_1MIN
				|| nOpenState == stBOTN::STATE_CLOSE
				)
			{
				minV = min(minV, pBOTN->dwOpenTick);
				msgACK.m_eJoinState	= eJION_TOP_STATE_NONADMISSION;
			}
			else
			{
				minV = pBOTN->dwCloseTick;
				msgACK.m_eJoinState	= eJION_TOP_STATE_ADMISSION;
				break;
			}

			++it;
		}

		if(INFINITE == minV)
		{
			ASSERT(FALSE);
			msgACK.m_eJoinState = eJION_TOP_STATE_NONE;
			break;
		}

		const DWORD curTick = GetTickCount();
		msgACK.m_JoinOverTime = minV - curTick;
    //----------------------------------------------------------------------------------------------
    #endif //__ITIMERBASE_VERSION_CRTL
    }
    FLOWCONTROL_END;

	//	'Ÿ��' control
	FLOWCONTROL_START()
    {

		WarControlManager* const pSSQControlManager = WarControlManager::Instance();
		msgACK.m_SSQTicketInfo = pSSQControlManager->GetTicketInfo();

    }
    FLOWCONTROL_END;

	pUser->SendPacket(&msgACK, sizeof(msgACK));

	return;
}

HANDLER_CG_IMPL(CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN)
{
    __TOUCHED((pMsg, wSize));
	MSG_CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN* pRecvMsg = (MSG_CG_ZONE_EX_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN*)pMsg;
	if(!pUser)
		return;	

	SUNLOG(eFULL_LOG, _T("%s\n"), __FUNCTION__);

	POSTYPE posOfItem = pRecvMsg->m_ItemPos;

	if(pUser->GetLockItemPosForInstanceDungeon() != INVALID_POSTYPE_VALUE)
		return;

	pUser->SetLockItemPosForInstanceDungeon(posOfItem);

	MSG_AG_ZONE_CAN_CREATE_100LEVEL_QUEST_MISSION_SYN msgSYN;
	msgSYN.m_ItemPos = posOfItem;
	pUser->SendToLinkedServer(&msgSYN, sizeof(msgSYN));	
}

HANDLER_CG_IMPL(CG_ZONE_EX_SSQ_JOIN_INFO_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv(pMsg, wSize, SSQSessionCmd::FromClient(pUser));
}

#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
extern void __WAVERIX_TESTUNIT_OnRecvCG_ZONE_EX_SSQ_JOIN_SYN(User* pUser, MAPCODE mapCode);
#endif

HANDLER_CG_IMPL(CG_ZONE_EX_SSQ_JOIN_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv(pMsg, wSize, SSQSessionCmd::FromClient(pUser));
}

HANDLER_CG_IMPL(CG_ZONE_EX_SSQ_ROOM_INFO_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv(pMsg, wSize, SSQSessionCmd::FromClient(pUser));
}

HANDLER_CG_IMPL(CG_ZONE_EX_SSQ_OBSERVER_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
	WarControlManager* const pSSQControlManager = WarControlManager::Instance();
	pSSQControlManager->OnRecv(pMsg, wSize, SSQSessionCmd::FromClient(pUser));
}

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
HANDLER_CG_IMPL(CG_ZONE_EX_SPA_ENTER_SYN)
{
    __TOUCHED((pUser, pMsg, wSize));
    MSG_CG_ZONE_EX_SPA_ENTER_SYN* recv_msg = static_cast<MSG_CG_ZONE_EX_SPA_ENTER_SYN*>(pMsg);

    if(pUser->IsBeginTransaction())
    {
        MSG_CG_ZONE_EX_SPA_ENTER_NAK nak_packet;
        nak_packet.error_code = RC::RC_SPA_USER_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&nak_packet, sizeof(nak_packet));
        return;
    }

    {
        MSG_AG_ZONE_SPA_ENTER_SYN syn_msg;
        syn_msg.item_code = recv_msg->item_code;
        pUser->SendToLinkedServer(&syn_msg, sizeof(syn_msg));
    }
}

HANDLER_CG_IMPL(CG_ZONE_EX_SPA_LEAVE_SYN)
{
    // ��õ���� [������] ���� ���
    __TOUCHED(pUser, pMsg, wSize);
    MSG_CG_ZONE_EX_SPA_LEAVE_SYN* recv_msg = static_cast<MSG_CG_ZONE_EX_SPA_LEAVE_SYN*>(pMsg);

    if (pUser->IsBeginTransaction())
    {
        MSG_CG_ZONE_EX_SPA_LEAVE_NAK nak_packet;
        nak_packet.error_code = RC::RC_SPA_USER_ALREADY_DOING_TRANSACTION;
        pUser->SendPacket(&nak_packet, sizeof(nak_packet));
        return;
    }

    if(pUser->GetZoneType() != eZONETYPE_SPA_FIELD)
    {
        MSG_CG_ZONE_EX_SPA_LEAVE_NAK nak_msg;
        nak_msg.error_code = RC::RC_SPA_INVALID_ZONE;
        pUser->SendPacket(&nak_msg, sizeof(nak_msg));
        return;
    }

    {
        MSG_AG_ZONE_SPA_LEAVE_SYN syn_packet;
        pUser->SendToLinkedServer(&syn_packet, sizeof(syn_packet));
    }
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND //�������� �ڵ鷯 ������
//ģ�� �ʴ� �̺�Ʈ�� ���� �������� ����� ��û�Ѵ�.
HANDLER_CG_IMPL(CG_EVENT_INVITATEFRIEND_REWARD_LIST_SYN)
{
    __TOUCHED((pMsg, wSize));
    SUNLOG(eFULL_LOG, "Recv CG_EVENT_INVITATEFRIEND_REWARD_LIST_SYN");

    MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN msg;
    
    msg.m_dwKey = pUser->GetUserKey();
    msg.CharGUID = pUser->GetSelectedCharGuid();
    
    if(!pUser->SendToDBPServer(&msg, sizeof(msg)))
    {
        MSG_CG_EVENT_INVITATEFRIEND_REWARD_LIST_NAK nmsg;
        nmsg.byErrorCode = 2;
        pUser->SendPacket(&nmsg, sizeof(nmsg));
        SUNLOG(eFULL_LOG,  "MSG_AD_EVENT_INVITATEFRIEND_REWARD_LIST_SYN Send Fail (%u, %u)", pUser->GetUserKey(), msg.CharGUID);
    }
}


#ifdef _NA_0_20110329_GAMEOPTION_SETTING
HANDLER_CG_IMPL( CG_ETC_GAMEOPTION_LOAD_SYN )
{
    MSG_CG_ETC_GAMEOPTION_LOAD_SYN* recv_message = 
        static_cast<MSG_CG_ETC_GAMEOPTION_LOAD_SYN*>(pMsg);
    GameOptionManager::Instance()->Load_GameOption(pUser, recv_message);
}
HANDLER_CG_IMPL( CG_ETC_GAMEOPTION_SAVE_SYN )
{
    MSG_CG_ETC_GAMEOPTION_SAVE_SYN* recv_message = 
        static_cast<MSG_CG_ETC_GAMEOPTION_SAVE_SYN*>(pMsg);
    GameOptionManager::Instance()->Save_GameOption(pUser, recv_message);
}
#endif //_NA_0_20110329_GAMEOPTION_SETTING


//_NA_000000_20120410_SPEEDHACK
HANDLER_CG_IMPL(CG_ETC_MEMORY_HACK_CMD)
{
    MSG_CG_ETC_MEMORY_HACK_CMD* recv_message = static_cast<MSG_CG_ETC_MEMORY_HACK_CMD*>(pMsg);
    if (pUser != NULL && recv_message != NULL)
    {
        SUNLOG(eCRITICAL_LOG, TEXT("Memory Hack | char_name = %s | char_level = %d"),
            pUser->GetSelectedCharName(),
            pUser->GetSelectedCharLV());
    }
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
HANDLER_CG_IMPL(CG_ETC_GUILD_RANKINGINFO_SYN)
{
    MSG_CG_ETC_GUILD_RANKINGINFO_SYN* packet = \
        static_cast<MSG_CG_ETC_GUILD_RANKINGINFO_SYN*>(pMsg);

    MSG_CG_ETC_GUILD_RANKINGINFO_ACK msg;
    msg.init();
    g_GuildRankingSystem.SerializeGuildRanking( \
        msg.guild_ranking_info_, \
        msg.guild_ranking_info_count_, \
        SERIALIZE_STORE);
    pUser->SendPacket(&msg, sizeof(msg));
}

HANDLER_CG_IMPL(CG_ETC_GUILDINFO_SYN)
{
    MSG_CG_ETC_GUILDINFO_SYN* packet = \
        static_cast<MSG_CG_ETC_GUILDINFO_SYN*>(pMsg);

    MSG_AD_ETC_GUILDINFO_SYN msg;
    msg.guid_ = packet->guid_;
    pUser->SendToDBPServer(&msg, sizeof(msg));
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
HANDLER_CG_IMPL(CG_SHOP_SEND_GIFT_SYN)
{
    __TOUCHED((pMsg, wSize));
    MSG_CG_SHOP_SEND_GIFT_SYN* pRecvMsg = static_cast<MSG_CG_SHOP_SEND_GIFT_SYN*>(pMsg);
    BYTE item_type = pRecvMsg->m_ItemType;
    if(wSize != sizeof(*pRecvMsg))  // no check 'item_type' -> unused value
        //|| (item_type != pRecvMsg->SINGLEITEM && item_type != pRecvMsg->PACKAGEITEM))
        return; //corrupt message, no answer

    BOOLEAN success = true;
    BYTE error_code = RC::RC_SHOP_SUCCESS;
    ShopUserQuerySlot& query_slot = pUser->GetShopUserQuerySlot();

    if(success && query_slot.IsTransaction() != 0)
    { success = false, error_code = RC::RC_SHOP_ALREADY_BUYING_TRANSACTION; }

#ifdef _USA
    // (CHANGES) (091202) add rule: �Ϲ��� ���, �����ϱ� �ڱ� ������ �ٸ� ĳ���Ϳ��Ե� ���� �� ���� �Ѵ�.
    char recv_char_name[MAX_CHARNAME_LENGTH + 1];
    CopyMemory(recv_char_name, pRecvMsg->m_tszCharName, sizeof(recv_char_name));
    recv_char_name[_countof(recv_char_name) - 1] = '\0';
    strupr(recv_char_name);

    char char_name_in_account[MAX_CHARNAME_LENGTH + 1];
    for(int i = 0; i < MAX_CHARACTER_SLOT_NUM; ++i) {
        if(pUser->IsValidSlot(i) == 0)
            continue;

        const char* char_name = pUser->GetCharName(i);
        strncpy(char_name_in_account, char_name, _countof(char_name_in_account));
        char_name_in_account[_countof(char_name_in_account) - 1] = '\0';
        strupr(char_name_in_account);

        if(strncmp(recv_char_name, char_name_in_account, _countof(recv_char_name)) == 0) {
            success = false, error_code = RC::RC_SHOP_CANNOT_SEND_GIFT_MYACCOUNT;
            break;
        }
    }
#endif

    if(success && query_slot.QueryStart(pRecvMsg) == 0)
    { success = false, error_code = RC::RC_SHOP_ALREADY_BUYING_TRANSACTION; }

    // fail process...
    if(success == false) {
        //query_slot.QueryEnd();
        MSG_CG_SHOP_SEND_GIFT_NAK msgNAK;
        // (NOTE) ������ ������ ���� Ʈ������� ������ ��� �������� �ʴ���,
        //�� �̻� �����ϱ� ����� �������� ���� �� �ִ�. ��ĳ �ұ�?
        msgNAK.m_byErrorCode = error_code;
        pUser->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

    // success status...
    // (current status list) #1 query slot alloced
    if(success) {
        MSG_SERVERCOMMON_QUERY_CHARINFO_SYN msgSYN;
        msgSYN.Clear();
        msgSYN.action_type_ = eCHARNAME_ACTION_SHOP_SEND_GIFT;

        const MSG_CG_SHOP_SEND_GIFT_SYN* backuped_msg = query_slot.GetMessage();
        QueryUserIdInfo& user_info = msgSYN.user_info_;
#ifdef _RU_20110801_SEND_GIFT_FRIEND_CHECK
        user_info.from_char_guid_ = pUser->GetSelectedCharGuid();
#endif
        user_info.requester_user_key_ = pUser->GetUserKey();
        //                            conversioned to uppercase
        strncpy(user_info.char_name_, backuped_msg->m_tszCharName, _countof(user_info.char_name_));
        user_info.char_name_[_countof(user_info.char_name_) - 1] = '\0';

        AgentServer* pAgentServer = AgentServer::GetInstance();
        pAgentServer->SendToDBPServer(&msgSYN, msgSYN.GetSize());
        return;
    }
}
#endif

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL