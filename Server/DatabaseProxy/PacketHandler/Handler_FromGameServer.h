#ifndef __HANDLER_DG_CHARINFO_H__
#define __HANDLER_DG_CHARINFO_H__

#pragma once

#include "./PacketHandler.h"

#define Handler_FromGameServer_DECL( p )	static VOID On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_FromGameServer_IMPL( p )	VOID Handler_FromGameServer::On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

#include <PacketStruct.h>

class ChildServerSession;

class Handler_FromGameServer
{
public:
	Handler_FromGameServer(){}
	~Handler_FromGameServer(){}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_FromGameServer_DECL(DG_CONNECTION_GROUND_CMD);
    //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
    Handler_FromGameServer_DECL(DG_CONNECTION_DBCONNECT_UPDATE_CMD);
	// 시리얼 발급
	Handler_FromGameServer_DECL(DG_ITEM_SERIAL_SYN);
	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------
	// Base.cpp
	// 유저 생성
	// 업데이트 주기로 정보가 왔을때
	Handler_FromGameServer_DECL(DG_CHARINFO_CHAR_ITEM_CMD);
#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
    Handler_FromGameServer_DECL(DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD);
#endif //_NA_20100106_REPURCHAS_BUGFIX_2496
    //_KR_0_20091021_FIRENDINVITATION_EXTEND
    Handler_FromGameServer_DECL(DG_CHARINFO_INVITATEDFRIEND_CMD);
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	Handler_FromGameServer_DECL(DG_CHARINFO_CHAR_ACHIEVEMENT_CMD);
#endif //C_NA_0_20100520_ACSYSTEM_CHANGED
#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    Handler_FromGameServer_DECL(DG_CHARINFO_SKILL_COOL_TIME_CMD);
#endif
	Handler_FromGameServer_DECL(DG_CHARINFO_SKILL_CMD);
	Handler_FromGameServer_DECL(DG_CHARINFO_QUICK_CMD);
	Handler_FromGameServer_DECL(DG_CHARINFO_STYLE_CMD);
	Handler_FromGameServer_DECL(DG_CHARINFO_QUEST_CMD);
	Handler_FromGameServer_DECL(DG_CHARINFO_WAREHOUSE_CMD);
	Handler_FromGameServer_DECL(DG_CHARINFO_EXTRA_CMD);

	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------
	// 캐릭터 접속/접속해제
	Handler_FromGameServer_DECL(DG_CHARINFO_CONNECT_CHAR_CMD);
	Handler_FromGameServer_DECL(DG_CHARINFO_DISCONNECT_CHAR_CMD);
	// 이미 한상태이기 때문에 안함!
	//Handler_FromGameServer_DECL(DG_CHARINFO_DISCONNECT_CHAR_CMD_DBR);

	// 유저 접속 해제
	Handler_FromGameServer_DECL(DG_CHARINFO_DISCONNECT_CMD);

	// 정보 조회 요청
	Handler_FromGameServer_DECL(DG_CHARINFO_QUERY_SYN);

	// 정보 저장 요청
	Handler_FromGameServer_DECL(DG_CHARINFO_UPDATE_SYN);
	Handler_FromGameServer_DECL(DG_CHARINFO_UPDATE_DBR);
    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    Handler_FromGameServer_DECL(DG_CHARINFO_UPDATE_INVEN_DBR);

	Handler_FromGameServer_DECL(DG_WAREHOUSE_UPDATE_INNER_DBR);

	Handler_FromGameServer_DECL(DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR);

	Handler_FromGameServer_DECL(DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN);
    Handler_FromGameServer_DECL(DG_CHARINFO_CHARLISTREQ_INTER_ITEMSLOTINFO_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_CHARLISTREQ_INTER_SLOTINFO_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_CHARLISTREQ_INTER_PERK_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_SKILL_LIST_CMD);
    Handler_FromGameServer_DECL(DG_CHARINFO_SKILL_PERK_CMD);
    Handler_FromGameServer_DECL(DG_CHARINFO_SKILL_PERK_CONTROL_CMD);
    Handler_FromGameServer_DECL(DG_CHARINFO_UPDATE_INTER_QUEST_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_UPDATE_INTER_BATTLERECORDS_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_CHARLISTREQ_INTER_CHAR_STATE_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_CHARLISTREQ_INTER_MISSION_DBR);
	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
	Handler_FromGameServer_DECL(DG_CHARINFO_SELECT_ACHIEVEMENT_DBR);
	Handler_FromGameServer_DECL(DG_CHARINFO_UPDATE_ACHIEVEMENT_DBR);
#endif


	// Warehouse.cpp
	// 창고
	Handler_FromGameServer_DECL(DG_WAREHOUSE_START_SYN);
    Handler_FromGameServer_DECL(DG_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR);
    Handler_FromGameServer_DECL(DG_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR);
	Handler_FromGameServer_DECL(DG_WAREHOUSE_START_DBR);
	Handler_FromGameServer_DECL(DG_WAREHOUSE_END_SYN);

	Handler_FromGameServer_DECL(DG_ITEM_INVENTORY_FLAG_STREAM_NTF);
	//Handler_FromGameServer_DECL(DG_ITEM_INVENTORY_PWD_SETTING_SYN);
	Handler_FromGameServer_DECL(DG_WAREHOUSE_FLAG_STREAM_NTF);
	Handler_FromGameServer_DECL(DG_WAREHOUSE_PWD_SETTING_SYN);

	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------

	Handler_FromGameServer_DECL( DG_OTHER_EXIST_CHARNAME_SYN );
	Handler_FromGameServer_DECL( DG_OTHER_EXIST_CHARNAME_DBR );
	Handler_FromGameServer_DECL( DG_OTHER_SHOP_RECV_GIFT_NTF );
#ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
	Handler_FromGameServer_DECL(DG_OTHER_FATIGUE_CMD);
#endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE
	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------
	// Event.cpp
	Handler_FromGameServer_DECL( DG_EVENT_SELECT_SYN );
	Handler_FromGameServer_DECL( DG_EVENT_SELECT_DBR );
	Handler_FromGameServer_DECL( DG_EVENT_UPDATE_SYN );
	Handler_FromGameServer_DECL( DG_EVENT_UPDATE_DBR );

	Handler_FromGameServer_DECL( DG_EVENT_ITEM_STATUS_SYN );
	Handler_FromGameServer_DECL( DG_EVENT_ITEM_STATUS_DBR );
	Handler_FromGameServer_DECL( DG_EVENT_ITEM_REG_SYN );
	Handler_FromGameServer_DECL( DG_EVENT_ITEM_REG_DBR );

	Handler_FromGameServer_DECL( DG_EVENT_QUERY_EXIST_SYN );
	Handler_FromGameServer_DECL( DG_EVENT_QUERY_EXIST_DBR );

	Handler_FromGameServer_DECL( DG_EVENT_STATUS_UPDATE_SYN );
	Handler_FromGameServer_DECL( DG_EVENT_STATUS_UPDATE_DBR );

	Handler_FromGameServer_DECL( DG_EVENT_SSQ_REWARD_UPDATE_SYN );
	Handler_FromGameServer_DECL( DG_EVENT_SSQ_REWARD_UPDATE_DBR );
    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
    Handler_FromGameServer_DECL( DG_EVENT_INVITATEFRIEND_REWARDUPDATE_SYN );
    Handler_FromGameServer_DECL( DG_EVENT_INVITATEFRIEND_REWARDUPDATE_DBR );
    Handler_FromGameServer_DECL( DG_EVENT_INVITATEFRIEND_RECALLSELECT_SYN );    
    Handler_FromGameServer_DECL( DG_EVENT_INVITATEFRIEND_RECALLSELECT_DBR );    
    Handler_FromGameServer_DECL( DG_EVENT_INVITATEFRIEND_RECALLUPDATE_CMD );    
    Handler_FromGameServer_DECL( DG_EVENT_INVITATEFRIEND_RECALLUPDATE_DBR );        
    //}
#ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
    Handler_FromGameServer_DECL( DG_EVENT_DOMINATION_REWARD_AND_DEL_SYN );   
    Handler_FromGameServer_DECL( DG_EVENT_DOMINATION_REWARD_AND_DEL_DBR );   
    Handler_FromGameServer_DECL( DG_EVENT_LOTTO_REWARD_AND_DEL_SYN );   
    Handler_FromGameServer_DECL( DG_EVENT_LOTTO_REWARD_AND_DEL_DBR );   
    Handler_FromGameServer_DECL( DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN );
#endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
    // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
    Handler_FromGameServer_DECL( DG_EVENT_SYSTEMMEMO_ITEM_CHECK_SYN );
    Handler_FromGameServer_DECL( DG_EVENT_SYSTEMMEMO_ITEM_CHECK_DBR );
    Handler_FromGameServer_DECL( DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_CMD );
    Handler_FromGameServer_DECL( DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_DBR );
    // }
    //_NA_20100604_BETTER_DOMINATION_MANAGER
    Handler_FromGameServer_DECL( DG_EVENT_DOMINATION_UPDATE_GUILDINFO_SYN );   
    Handler_FromGameServer_DECL( DG_EVENT_DOMINATION_UPDATE_GUILDINFO_DBR );   
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    Handler_FromGameServer_DECL( DG_EVENT_SMARTNPC_SYSTEM );
    Handler_FromGameServer_DECL( DG_EVENT_SET_ATTENDANCE_DBR );
    Handler_FromGameServer_DECL( DG_EVENT_GET_ATTENDANCE_DBR );
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    Handler_FromGameServer_DECL( DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_SYN );
    Handler_FromGameServer_DECL( DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_DBR );
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
    Handler_FromGameServer_DECL(DG_GMEVENT_PEAKTIME_REGISTER_SYN);
    Handler_FromGameServer_DECL(DG_GMEVENT_PEAKTIME_TERMINATE_SYN);
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
	//------------------------------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------------------------------------
	// 웨이 포인트 관련
	Handler_FromGameServer_DECL(DG_WAYPOINT_INFO_LOAD_SYN);
	Handler_FromGameServer_DECL(DG_WAYPOINT_INFO_LOAD_DBR);
	Handler_FromGameServer_DECL(DG_WAYPOINT_INFO_SAVE_SYN);
	Handler_FromGameServer_DECL(DG_WAYPOINT_INFO_SAVE_DBR);
	// 개인 웨이 포인트 관련
	Handler_FromGameServer_DECL(DG_CHAR_WAYPOINT_INFO_LOAD_SYN);
	Handler_FromGameServer_DECL(DG_CHAR_WAYPOINT_INFO_LOAD_DBR);
	Handler_FromGameServer_DECL(DG_CHAR_WAYPOINT_INFO_SAVE_SYN);
	Handler_FromGameServer_DECL(DG_CHAR_WAYPOINT_INFO_SAVE_DBR);
	Handler_FromGameServer_DECL(DG_CHAR_WAYPOINT_INFO_DEL_SYN);
	Handler_FromGameServer_DECL(DG_CHAR_WAYPOINT_INFO_DEL_DBR);
#ifdef _NA_008012_20150130_RANKING_SYSTEM
	//------------------------------------------------------------------------------------------------------------
	// 랭킹 포인트 관련
	Handler_FromGameServer_DECL(DG_MISSION_RANKING_POINT_REQ_SYN);
	Handler_FromGameServer_DECL(DG_MISSION_RANKING_POINT_REQ_DBR);
#endif // _NA_008012_20150130_RANKING_SYSTEM

	Handler_FromGameServer_DECL(DG_GUILD_WAREHOUSE_CMD);
	Handler_FromGameServer_DECL(DG_GUILD_WAREHOUSELOG_UPDATE_SYN );
	Handler_FromGameServer_DECL(DG_GUILD_WAREHOUSELOG_UPDATE_DBR );
	Handler_FromGameServer_DECL(DG_GUILD_WAREHOUSE_UPDATE_INNER_DBR);
    Handler_FromGameServer_DECL(DG_GUILD_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR);
	Handler_FromGameServer_DECL(DG_GUILD_WAREHOUSE_TAPEX_SYN);
	Handler_FromGameServer_DECL(DG_GUILD_WAREHOUSE_TAPEX_DBR);
	Handler_FromGameServer_DECL(DG_GUILD_WAREHOUSE_CLOSE_SYN);
    //{__NA001283_20090227_COLLECTION_LIST
	Handler_FromGameServer_DECL(DG_COLLECT_LIST_LOAD_SYN);
	Handler_FromGameServer_DECL(DG_COLLECT_LIST_LOAD_DBR);
	Handler_FromGameServer_DECL(DG_COLLECT_ADD_SYN);
	Handler_FromGameServer_DECL(DG_COLLECT_ADD_DBR);
	Handler_FromGameServer_DECL(DG_COLLECT_DEL_SYN);
	Handler_FromGameServer_DECL(DG_COLLECT_DEL_DBR);
    //}
    Handler_FromGameServer_DECL(DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD);
    Handler_FromGameServer_DECL(DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_DBR);
    Handler_FromGameServer_DECL(DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN);
    Handler_FromGameServer_DECL(DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR);
    Handler_FromGameServer_DECL(DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD);
    Handler_FromGameServer_DECL(DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR);

    Handler_FromGameServer_DECL(DG_OTHER_EXTEND_CHAR_SLOT_SYN);
    Handler_FromGameServer_DECL(DG_OTHER_EXTEND_CHAR_SLOT_DBR);

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    Handler_FromGameServer_DECL(DG_CHARINFO_HONORSYSTEM);   // G -> D
    Handler_FromGameServer_DECL(DG_HONORSYSTEM_TITLES);     // DG_CHARINFO_HONORSYSTEM | DG_HONORSYSTEM_TITLES
    Handler_FromGameServer_DECL(DG_CHARINFO_HONORTITLES_SELECT_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_HONORTITLES_ADD_REMOVE_DBR);
    Handler_FromGameServer_DECL(DG_HONORSYSTEM_FAME_REPUTE);
    Handler_FromGameServer_DECL(DG_CHARINFO_FAMEREPUTE_SELECT_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_FAMEREPUTE_CHANGE_DBR);
    Handler_FromGameServer_DECL(DG_HONORSYSTEM_DAILYQUEST);
    Handler_FromGameServer_DECL(DG_CHARINFO_DAILYQUEST_SELECT_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_DAILYQUEST_UPDATE_DBR);
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    Handler_FromGameServer_DECL(DG_CHARINFO_POINTWALLET_SYSTEM);
    Handler_FromGameServer_DECL(DG_CHARINFO_POINTWALLET_SELECT_DBR);
    Handler_FromGameServer_DECL(DG_CHARINFO_POINTWALLET_UPDATE_DBR);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
    Handler_FromGameServer_DECL(DG_CHARINFO_POINTWALLET_GUILDCOIN_DONATION_DBR);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
    Handler_FromGameServer_DECL(DG_CHARNAME_CHANGE_CMD);
    Handler_FromGameServer_DECL(DG_CHARNAME_CHANGE_DBR);
    //!~ _NA_006372_20130108_BADGE_ITEM
    Handler_FromGameServer_DECL(DG_CHARACTER_EXTRA_INFO_CMD); 
    Handler_FromGameServer_DECL(DG_CHARACTER_EXTRA_INFO_DBR);
    //~! _NA_006372_20130108_BADGE_ITEM
#ifdef _NA_007932_20150106_USER_EXTRA_INFO
    Handler_FromGameServer_DECL(DG_USER_EXTRA_INFO_CMD);
    Handler_FromGameServer_DECL(DG_USER_EXTRA_INFO_DBR);
#endif // _NA_007932_20150106_USER_EXTRA_INFO
#ifdef _NA_007086_20140318_MONSTERBOOK
    Handler_FromGameServer_DECL(DG_CHARACTER_MONSTERBOOK_CMD);
    Handler_FromGameServer_DECL(DG_CHARACTER_MONSTERBOOK_DBR);
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    Handler_FromGameServer_DECL(DG_SUN_RANKING_UPDATE_CMD);
    Handler_FromGameServer_DECL(DG_SUN_RANKING_UPDATE_DBR);

    Handler_FromGameServer_DECL(DG_SUN_RANKING_SELECT_SYN);
    Handler_FromGameServer_DECL(DG_SUN_RANKING_SELECT_DBR);

    Handler_FromGameServer_DECL(DG_SUN_RANKING_INIT_SYN);
    Handler_FromGameServer_DECL(DG_SUN_RANKING_INIT_DBR);

    Handler_FromGameServer_DECL(DG_SUN_RANKING_GRADE_INSERT_SYN);
    Handler_FromGameServer_DECL(DG_SUN_RANKING_GRADE_INSERT_DBR);

    Handler_FromGameServer_DECL(DG_SUN_RANKING_GRADE_INIT_SYN);
    Handler_FromGameServer_DECL(DG_SUN_RANKING_GRADE_INIT_DBR);

    Handler_FromGameServer_DECL(DG_SUN_RANKING_GRADE_SELECT_SYN);
    Handler_FromGameServer_DECL(DG_SUN_RANKING_GRADE_SELECT_DBR);

    Handler_FromGameServer_DECL(DG_STATUE_SELECT_SYN);

    Handler_FromGameServer_DECL(DG_STATUE_CHAR_INSERT_SYN);
    Handler_FromGameServer_DECL(DG_STATUE_CHAR_INSERT_DBR);

    Handler_FromGameServer_DECL(DG_STATUE_ITEM_SLOT_INFO_INSERT_SYN);
    Handler_FromGameServer_DECL(DG_STATUE_ITEM_SLOT_INFO_INSERT_DBR);

    Handler_FromGameServer_DECL(DG_STATUE_ITEM_SLOT_INFO_SELECT_SYN);
    Handler_FromGameServer_DECL(DG_STATUE_ITEM_SLOT_INFO_SELECT_DBR);

    Handler_FromGameServer_DECL(DG_STATUE_CHAR_SELECT_SYN);
    Handler_FromGameServer_DECL(DG_STATUE_CHAR_SELECT_DBR);

    Handler_FromGameServer_DECL(DG_SUN_RANKING_SETTLE_START_SYN);

    Handler_FromGameServer_DECL(DG_SUN_RANKING_SETTLE_COMPLETE_SYN);

    Handler_FromGameServer_DECL(DG_SUN_RANKING_SETTLE_SYN);
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    Handler_FromGameServer_DECL(DG_NOTIFICATION_ITEM_SYN);
    Handler_FromGameServer_DECL(DG_NOTIFICATION_CLEAR_SYN);
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
};


#endif // __HANDLER_DG_CHARINFO_H__