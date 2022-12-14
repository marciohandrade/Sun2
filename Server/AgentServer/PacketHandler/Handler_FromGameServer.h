#pragma once

#include <PacketStruct_AG.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class GameServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_AG_DECL(fnName)		\
	static VOID On##fnName(GameServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

#define HANDLER_AG_IMPL(fnName)		\
	VOID Handler_AG::On##fnName(GameServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize)

//-------------------------------------------------------------------------------------------------

class Handler_AG
{
public:
	static VOID		RegisterHandler_ALL(PACKET_HANDLER_DELEGATOR pInstance);
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_SYNC();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_ZONE();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_STATUS();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_GUILD();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_MISSION();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_GM();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_CHUNTING();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_CHAO();
	static sPACKET_HANDLER_INFO*	GetHandlerInfo_AG_ITEM();
public:
	//------------------------------------------------------------------------------
	// <PACKET HANDLERs>
	// Declaration & Implementation
	//
    HANDLER_AG_DECL(AG_CONNECTION_GROUND_CMD);
	HANDLER_AG_DECL(AG_CONNECTION_UNREGISTER_CMD);
	HANDLER_AG_DECL(AG_CONNECTION_DISCONNECT_SERVER_CMD);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    HANDLER_AG_DECL(AG_CONNECTION_SERVER_SELECT_CONTROL_ACK);
#endif

	HANDLER_AG_DECL(AG_SYNC_PLAYER_ENTER_ACK);
	HANDLER_AG_DECL(AG_SYNC_PLAYER_ENTER_NAK);

	HANDLER_AG_DECL(AG_SYNC_PLAYER_WARP_ACK);
	HANDLER_AG_DECL(AG_SYNC_PLAYER_WARP_NAK);

	HANDLER_AG_DECL(AG_CHARINFO_CHAR_CMD);

	HANDLER_AG_DECL(AG_CHARINFO_FIELD_POS_FOR_RETURN_PARTY_CMD);
	//HANDLER_AG_DECL(AG_CONNECTION_UNREGISTER_ACK);
	//HANDLER_AG_DECL(AG_CONNECTION_UNREGISTER_NAK);

	HANDLER_AG_DECL(AG_PARTY_FIELD_AGENT_INVITE_RESPONSE_NAK);
	HANDLER_AG_DECL(AG_PARTY_BATTLE_AGENT_JOINROOM_CMD);
	HANDLER_AG_DECL(AG_PARTY_BATTLE_AGENT_INVITE_REQUEST_SYN);
	HANDLER_AG_DECL(AG_PARTY_BATTLE_AGENT_CHANGE_MASTER_CMD);
    //__NA001336_090514_PARTY_NAVI_SUPPORT__
    HANDLER_AG_DECL(AG_PARTY_NAVI_INFO_BRD);

	HANDLER_AG_DECL(AG_ZONE_ROOM_JOIN_ACK);
	HANDLER_AG_DECL(AG_ZONE_ROOM_JOIN_NAK);

	HANDLER_AG_DECL(AG_ZONE_ROOM_LEAVE_ACK);
	HANDLER_AG_DECL(AG_ZONE_ROOM_LEAVE_NAK);

	HANDLER_AG_DECL(AG_ZONE_MISSION_LEAVE_CMD);
	HANDLER_AG_DECL(AG_ZONE_CHUNTING_LEAVE_CMD);
	HANDLER_AG_DECL(AG_ZONE_CHUNTING_COMPETITION_END_CMD);

	HANDLER_AG_DECL(AG_ZONE_PORTAL_FIELD_MOVE_ACK);
	HANDLER_AG_DECL(AG_ZONE_PORTAL_FIELD_MOVE_NAK);

	HANDLER_AG_DECL(AG_ZONE_RETURN_VILLAGE_ACK);
	HANDLER_AG_DECL(AG_ZONE_RETURN_VILLAGE_NAK);

	HANDLER_AG_DECL(AG_ZONE_RETURN_TO_ROOM_INFO_ACK);

	HANDLER_AG_DECL(AG_STATUS_LEVEL_UP_CMD);
	HANDLER_AG_DECL(AG_STATUS_MAXHP_CHANGE_CMD);
	HANDLER_AG_DECL(AG_STATUS_HP_CHANGE_CMD);
    HANDLER_AG_DECL(AG_STATUS_MAXSD_CHANGE_CMD);
    //
	HANDLER_AG_DECL(AG_STATUS_DEAD_CMD);
	HANDLER_AG_DECL(AG_STATUS_RESURRECTION_CMD);
	HANDLER_AG_DECL(AG_STATUS_MP_CHANGE_CMD);
	HANDLER_AG_DECL(AG_STATUS_MAXMP_CHANGE_CMD);
	HANDLER_AG_DECL(AG_STATUS_CHANGE_STATE_CMD);
	HANDLER_AG_DECL(AG_STATUS_PLAYING_TUTORIAL_CMD);
    //__NA001336_090514_PARTY_NAVI_SUPPORT__
    HANDLER_AG_DECL(AG_STATUS_CHAR_STAT_CHANGE_CMD);
    HANDLER_AG_DECL(AG_STATUS_GROUP_EXP_ACQUISITION_CMD);
    //__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
    HANDLER_AG_DECL(AG_STATUS_HEIM_ACQUISITION_NTF);
    HANDLER_AG_DECL(AG_STATUS_HEIM_CONSUMPTION_NTF);
    HANDLER_AG_DECL(AG_STATUS_LOTTO_REWARD_ACK);
#ifdef _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    HANDLER_AG_DECL( AG_STATUS_LOTTO_TICKET_PURCHASE_ACK );
#endif // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
	HANDLER_AG_DECL(AG_GUILD_CREATE_ACK);
	HANDLER_AG_DECL(AG_GUILD_DESTROY_ACK);

	HANDLER_AG_DECL(AG_GUILD_JOIN_SUCCESS_CMD);
	HANDLER_AG_DECL(AG_GUILD_WITHDRAW_ACK);

	HANDLER_AG_DECL(AG_GUILD_KICKPLAYER_ACK);

	HANDLER_AG_DECL(AG_MISSION_STREAM_CMD);
	HANDLER_AG_DECL(AG_MISSION_CLEAR_CMD);

	HANDLER_AG_DECL(AG_CHUNTING_WIN_CMD);
	HANDLER_AG_DECL(AG_CHUNTING_START_COMPETITION_CMD);

	HANDLER_AG_DECL(AG_GM_WHISPER_SET_CMD);
	HANDLER_AG_DECL(AG_GM_USERCHAT_BLOCK_CMD);

	HANDLER_AG_DECL(AG_GM_CHAR_INFO_CMD);
	HANDLER_AG_DECL(AG_GM_CHAR_INFO_ACK);

	HANDLER_AG_DECL(AG_GM_ROOM_INFO_CMD);

	HANDLER_AG_DECL(AG_GM_CHANNEL_INFO_CMD);
	HANDLER_AG_DECL(AG_GM_CHANGE_ROOMTITLE_CMD);

	HANDLER_AG_DECL(AG_GM_FORCE_DISCONNECT_CMD);
	HANDLER_AG_DECL(AG_GM_SERVER_SHUTDOWN_CMD);

	HANDLER_AG_DECL(AG_GM_GO_CMD);
	HANDLER_AG_DECL(AG_GM_TRACKING_CMD);
	HANDLER_AG_DECL(AG_GM_TRACKING_ACK);
	HANDLER_AG_DECL(AG_GM_USER_SUMMON_CMD);
	HANDLER_AG_DECL(AG_GM_USER_MOVE_CMD);
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM = {
    HANDLER_AG_DECL(AG_GM_LOTTO_NUM_CMD);
    HANDLER_AG_DECL(AG_GM_LOTTO_SETTLE_CMD);
    HANDLER_AG_DECL(AG_GM_LOTTO_FUND_CMD);
    //}
    HANDLER_AG_DECL(AG_GM_DOMINATION_INIT_CMD);
	// AG_CHAO
	HANDLER_AG_DECL(AG_CHAO_REQUEST_REVENGEINFO_SYN);
	HANDLER_AG_DECL(AG_CHAO_REMOVE_REVENGE_CHAR_SYN);
	HANDLER_AG_DECL(AG_CHAO_ADD_REVENGE_CHAR_SYN);
	HANDLER_AG_DECL(AG_CHAO_CHANGE_CHAO_STATE_CMD);

	HANDLER_AG_DECL(AG_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD);

	HANDLER_AG_DECL(AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_ACK);
	HANDLER_AG_DECL(AG_ZONE_CAN_CREATE_INSTANCE_DUNGEON_NAK);

	HANDLER_AG_DECL(AG_ITEM_USE_DECREE_ITEM_ACK);
	HANDLER_AG_DECL(AG_ITEM_USE_DECREE_ITEM_NAK);
#ifdef _NA_20101011_HEIM_LOTTO_CASH_TICKET
    HANDLER_AG_DECL(AG_ITEM_USE_HEIM_LOTTO_TICKET_SYN);
#endif

	HANDLER_AG_DECL(AG_STATUS_INTERCEPTION_SYN);
	HANDLER_AG_DECL(AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN);
	HANDLER_AG_DECL(AG_GM_NOTICE_CMD);
	HANDLER_AG_DECL(AG_ZONE_CAN_JOIN_100LEVEL_QUEST_MISSION_ACK);
	HANDLER_AG_DECL(AG_EVENT_BUFF_EVENT_TIME_CONTROL_SYN);

	HANDLER_AG_DECL(AG_ZONE_SSQ_JOIN_INFO_ACK);
	HANDLER_AG_DECL(AG_ZONE_SSQ_JOIN_ACK);
	HANDLER_AG_DECL(AG_ZONE_SSQ_ROOM_CTRL_CMD);
	HANDLER_AG_DECL(AG_ZONE_SSQ_ROOM_INFO_BRD);
    //{__NA1279_1281_090209_TIMELINE_QUEST_BASED_MISSION__
    HANDLER_AG_DECL(AG_ZONE_QUEST_PORTAL_REQUEST_SYN);
    HANDLER_AG_DECL(AG_ZONE_QUEST_PORTAL_ENTER_ACK);
    HANDLER_AG_DECL(AG_ZONE_QUEST_PORTAL_CONTROL_CMD);
    //}
    HANDLER_AG_DECL(AG_PARTY_JOIN_ACK);
	HANDLER_AG_DECL(AG_PARTY_JOIN_NAK);
	HANDLER_AG_DECL(AG_PARTY_LEAVE_BY_GUILDRELATION_SYN);
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    HANDLER_AG_DECL(AG_PARTY_CAN_CHANGE_OPTION_ACK);
    HANDLER_AG_DECL(AG_PARTY_CAN_CREATE_ROOM_ACK);
    HANDLER_AG_DECL(AG_PARTY_READY_STATE_ACK);
    HANDLER_AG_DECL(AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK);
    HANDLER_AG_DECL(AG_PARTY_CAN_JOIN_ROOM_ACK);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    HANDLER_AG_DECL(AG_CHAOS_ZONE_PACKET);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    //{__NA001281_BATTLEZONE_ENTER_CONSTRAINTS_CHANGE_IN_LOBBY__
    HANDLER_AG_DECL(AG_ZONE_CAN_CREATE_ROOM_ACK);
    HANDLER_AG_DECL(AG_ZONE_LOBBY_CREATE_ACK);
    HANDLER_AG_DECL(AG_ZONE_LOBBY_JOIN_ACK);
    //}
    //_NA_000038_20100316_VIEW_TO_JOINABLE_ROOM_IN_LOBBY
    // (ADD)(20100318)(hukim)(REQ_0038) AG_ZONE_CHECK_MISSION_ROOM_LIST_ACK 
    // ?????????????? ???? ???? ???? ?????? ?????? ?????? ???????? ???????????? ???????? 
    // ?????? ?????? ????
    HANDLER_AG_DECL(AG_ZONE_CHECK_MISSION_ROOM_LIST_ACK);

    HANDLER_AG_DECL(AG_ZONE_DOMINATION_FIELD_INFO_ACK);
    HANDLER_AG_DECL(AG_ZONE_DOMINATION_FIELD_INFO_NAK);
    HANDLER_AG_DECL(AG_ZONE_DOMINATION_REQ_KICK_CMD);
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
    HANDLER_AG_DECL(AG_EVENT_SYSTEMMEMO_SEND_CMD);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    HANDLER_AG_DECL(AG_EVENT_TODAYQUEST_INFO_BRD);
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    HANDLER_AG_DECL(AG_GM_GUILD_RANKING_REFRESH);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#ifdef _NA_006413_20130201_PREMIUMSERVICE_ITEM
    HANDLER_AG_DECL(AG_CHARINFO_PREMIUMSERVICE_BILL_CMD);
#endif //_NA_006413_20130201_PREMIUMSERVICE_ITEM
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
    HANDLER_AG_DECL(AG_GM_BATTLE_GROUND_CMD);
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    HANDLER_AG_DECL(AG_ZONE_DOMINATION_NOTIFIER_BRD);
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    HANDLER_AG_DECL(AG_ZONE_SPA_ENTER_ACK);
    HANDLER_AG_DECL(AG_ZONE_SPA_LEAVE_ACK);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_008334_20150608_SONNENSCHEIN
    HANDLER_AG_DECL(AG_GM_SONNENSCHEIN_CMD);
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	HANDLER_AG_DECL(AG_GM_ONEDAYSGUILDMISSION_CMD);
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
};













