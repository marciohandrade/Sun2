#include "stdafx.h"
#include ".\Handler_FromGameServer.h"
#include <PacketStruct_DG.h>
#include <Macro.h>
#include "Sessions/ChildServerSession.h"
#include "DBUser.h"
#include "DBUserManager.h"
#include "Sessions/GameServerSession.h"
#include <SCSlot.h>
#include <SCSlotContainer.h>
#include "DBProxyServerEx.h"
#include "QueryObjects/GameServerQuery.h"
#include "Services/DBSerialManager.h"
#include <WayPoint.h>
#include <ResultCode.h>
#include <time.h>
#include <SolarDateTime.h>
#include "CollectedList.h"
#include "ProxyNodes/DBQueryProxyManager.h"
#include "QueryObjects/DBHandler.h"
#include "PacketHandler/Handler_FromGameServer.Composition.h"
#ifdef _USE_MAX_CHAR_LIST_PARSER
#include <MaxCharListParser.h>
#endif

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
    #include "Services/SkillCoolTimeStorage.h"
#endif
#include "PacketStruct_PointSystem.h"

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
#include "ProxyNodes/DBQueryProxyItemSlotNode.h"
#include "Services/SunRankingSettle.h"
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "PacketStruct_DW.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

//==================================================================================================
// CHANGES: f110518.2L
const nsPacket::PacketHandlerInfo Handler_FromGameServer::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), false, phase, Handler_FromGameServer::On##p }
#define HANDLER_NODE_INFO_DBR(c, p, phase) { MAKEWORD(c, p), true, phase, Handler_FromGameServer::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, false, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(DG_CONNECTION, DG_CONNECTION_GROUND_CMD, PHASE_UNLIMITED),
        //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
        HANDLER_NODE_INFO(DG_CONNECTION, DG_CONNECTION_DBCONNECT_UPDATE_CMD, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_WAREHOUSE,DG_WAREHOUSE_START_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAREHOUSE,DG_WAREHOUSE_START_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAREHOUSE, DG_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAREHOUSE, DG_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_WAREHOUSE,DG_WAREHOUSE_END_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAREHOUSE,DG_WAREHOUSE_UPDATE_INNER_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_ITEM, DG_ITEM_INVENTORY_FLAG_STREAM_NTF, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO(DG_ITEM, DG_ITEM_INVENTORY_PWD_SETTING_SYN, PHASE_UNLIMITED), //여기에 포함되었다.
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_WAREHOUSE_FLAG_STREAM_NTF, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_WAREHOUSE_PWD_SETTING_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_ITEM,DG_ITEM_SERIAL_SYN, PHASE_UNLIMITED),
        //HANDLER_NODE_INFO_DBR(DG_MISSION, DG_MISSION_RECORD_SELECT_DBR, PHASE_UNLIMITED),

#ifdef _NA_008012_20150130_RANKING_SYSTEM
        HANDLER_NODE_INFO(DG_MISSION,DG_MISSION_RANKING_POINT_REQ_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_MISSION,DG_MISSION_RANKING_POINT_REQ_DBR, PHASE_UNLIMITED),
#endif // _NA_008012_20150130_RANKING_SYSTEM

        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_CHAR_ITEM_CMD, PHASE_UNLIMITED),
    #ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD, PHASE_UNLIMITED),
    #endif //_NA_20100106_REPURCHAS_BUGFIX_2496
        //_KR_0_20091021_FIRENDINVITATION_EXTEND
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_INVITATEDFRIEND_CMD, PHASE_UNLIMITED),
    #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_CHAR_ACHIEVEMENT_CMD, PHASE_UNLIMITED),
    #endif //C_NA_0_20100520_ACSYSTEM_CHANGED
    #ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_SKILL_COOL_TIME_CMD, PHASE_UNLIMITED),
    #endif
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_SKILL_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_QUICK_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_STYLE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_QUEST_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_WAREHOUSE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_EXTRA_CMD, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_CONNECT_CHAR_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_DISCONNECT_CHAR_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_DISCONNECT_CMD, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_QUERY_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO,DG_CHARINFO_UPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO,DG_CHARINFO_UPDATE_DBR, PHASE_UNLIMITED),
        //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_UPDATE_INVEN_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_ITEMSLOTINFO_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_SLOTINFO_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_PERK_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_SKILL_LIST_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_SKILL_PERK_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_SKILL_PERK_CONTROL_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_UPDATE_INTER_QUEST_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_UPDATE_INTER_BATTLERECORDS_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_CHAR_STATE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_MISSION_DBR, PHASE_UNLIMITED),

    #ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_SELECT_ACHIEVEMENT_DBR, PHASE_UNLIMITED),  
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_UPDATE_ACHIEVEMENT_DBR, PHASE_UNLIMITED),  
    #endif

        HANDLER_NODE_INFO(DG_EVENT,DG_EVENT_SELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT,DG_EVENT_SELECT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_EVENT,DG_EVENT_UPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT,DG_EVENT_UPDATE_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_EVENT,DG_EVENT_ITEM_STATUS_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT,DG_EVENT_ITEM_STATUS_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_EVENT,DG_EVENT_ITEM_REG_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT,DG_EVENT_ITEM_REG_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_QUERY_EXIST_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_QUERY_EXIST_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_STATUS_UPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_STATUS_UPDATE_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SSQ_REWARD_UPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_SSQ_REWARD_UPDATE_DBR, PHASE_UNLIMITED),
        //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_INVITATEFRIEND_REWARDUPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_INVITATEFRIEND_REWARDUPDATE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLSELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLSELECT_DBR, PHASE_UNLIMITED),    
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLUPDATE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_INVITATEFRIEND_RECALLUPDATE_DBR, PHASE_UNLIMITED),    
        //}
    #ifdef _NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_DOMINATION_REWARD_AND_DEL_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_DOMINATION_REWARD_AND_DEL_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_LOTTO_REWARD_AND_DEL_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_LOTTO_REWARD_AND_DEL_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN, PHASE_UNLIMITED),
    #endif//_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
        // added by _NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SYSTEMMEMO_ITEM_CHECK_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_SYSTEMMEMO_ITEM_CHECK_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_DBR, PHASE_UNLIMITED),
        // }
        HANDLER_NODE_INFO(DG_OTHER,DG_OTHER_EXIST_CHARNAME_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_OTHER,DG_OTHER_EXIST_CHARNAME_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_OTHER,DG_OTHER_SHOP_RECV_GIFT_NTF, PHASE_UNLIMITED),
    #ifdef __CN_OO0510__FATIGUE_MATCHING_UPDATE
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_FATIGUE_CMD, PHASE_UNLIMITED),
    #endif //__CN_OO0510__FATIGUE_MATCHING_UPDATE
        HANDLER_NODE_INFO(DG_WAYPOINT,DG_WAYPOINT_INFO_LOAD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAYPOINT,DG_WAYPOINT_INFO_LOAD_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_WAYPOINT,DG_WAYPOINT_INFO_SAVE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAYPOINT,DG_WAYPOINT_INFO_SAVE_DBR, PHASE_UNLIMITED),
        //{개인 웨이 포인트 관련
        HANDLER_NODE_INFO(DG_WAYPOINT,DG_CHAR_WAYPOINT_INFO_LOAD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAYPOINT,DG_CHAR_WAYPOINT_INFO_LOAD_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_WAYPOINT,DG_CHAR_WAYPOINT_INFO_SAVE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAYPOINT,DG_CHAR_WAYPOINT_INFO_SAVE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_WAYPOINT,DG_CHAR_WAYPOINT_INFO_DEL_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAYPOINT,DG_CHAR_WAYPOINT_INFO_DEL_DBR, PHASE_UNLIMITED),
        //}

        HANDLER_NODE_INFO(DG_WAREHOUSE,DG_GUILD_WAREHOUSE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_WAREHOUSE,DG_GUILD_WAREHOUSELOG_UPDATE_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAREHOUSE,DG_GUILD_WAREHOUSELOG_UPDATE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_TAPEX_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_TAPEX_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_UPDATE_INNER_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_CLOSE_SYN, PHASE_UNLIMITED),
        //{__NA001283_20090227_COLLECTION_LIST
        // 채집물 리스트
        HANDLER_NODE_INFO(DG_COLLECT,DG_COLLECT_LIST_LOAD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_COLLECT,DG_COLLECT_LIST_LOAD_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_COLLECT,DG_COLLECT_ADD_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_COLLECT,DG_COLLECT_ADD_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_COLLECT,DG_COLLECT_DEL_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_COLLECT,DG_COLLECT_DEL_DBR, PHASE_UNLIMITED),
        //}
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_OTHER, DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_OTHER, DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_OTHER, DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_OTHER, DG_OTHER_EXTEND_CHAR_SLOT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_OTHER, DG_OTHER_EXTEND_CHAR_SLOT_DBR, PHASE_UNLIMITED),

#ifdef _NA_003027_20111013_HONOR_SYSTEM
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_HONORSYSTEM, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_HONORTITLES_SELECT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_HONORTITLES_ADD_REMOVE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_FAMEREPUTE_SELECT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_FAMEREPUTE_CHANGE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_DAILYQUEST_SELECT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_DAILYQUEST_UPDATE_DBR, PHASE_UNLIMITED),
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARINFO_POINTWALLET_SYSTEM, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_POINTWALLET_SELECT_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_POINTWALLET_UPDATE_DBR, PHASE_UNLIMITED),
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARINFO_POINTWALLET_GUILDCOIN_DONATION_DBR, PHASE_UNLIMITED),
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARNAME_CHANGE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARNAME_CHANGE_DBR, PHASE_UNLIMITED),

        //!~ _NA_006372_20130108_BADGE_ITEM
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARACTER_EXTRA_INFO_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARACTER_EXTRA_INFO_DBR, PHASE_UNLIMITED),
        //~! _NA_006372_20130108_BADGE_ITEM
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_SMARTNPC_SYSTEM, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_SET_ATTENDANCE_DBR, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_GET_ATTENDANCE_DBR, PHASE_UNLIMITED),
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        HANDLER_NODE_INFO(DG_EVENT, DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_EVENT, DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_DBR, PHASE_UNLIMITED),
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007086_20140318_MONSTERBOOK
        HANDLER_NODE_INFO(DG_CHARINFO, DG_CHARACTER_MONSTERBOOK_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_CHARACTER_MONSTERBOOK_DBR, PHASE_UNLIMITED),
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
        HANDLER_NODE_INFO(DG_CHARINFO, DG_USER_EXTRA_INFO_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_CHARINFO, DG_USER_EXTRA_INFO_DBR, PHASE_UNLIMITED),
#endif //_NA_007932_20150106_USER_EXTRA_INFO

#ifdef _NA_007919_20141211_ADD_GMCOMMAND_EVENT
        HANDLER_NODE_INFO(DG_EVENT, DG_GMEVENT_PEAKTIME_REGISTER_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_EVENT, DG_GMEVENT_PEAKTIME_TERMINATE_SYN, PHASE_UNLIMITED),
#endif //_NA_007919_20141211_ADD_GMCOMMAND_EVENT

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_UPDATE_CMD, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_SUN_RANKING_UPDATE_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_SELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_SUN_RANKING_SELECT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_SUN_RANKING_INIT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_INSERT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_INIT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_SELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_SUN_RANKING_GRADE_SELECT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_STATUE_SELECT_SYN, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_STATUE_CHAR_INSERT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_STATUE_CHAR_INSERT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_STATUE_ITEM_SLOT_INFO_SELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_STATUE_ITEM_SLOT_INFO_SELECT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_STATUE_ITEM_SLOT_INFO_INSERT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_STATUE_ITEM_SLOT_INFO_INSERT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_STATUE_CHAR_SELECT_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO_DBR(DG_SUN_RANKING, DG_STATUE_CHAR_SELECT_DBR, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_START_SYN, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_COMPLETE_SYN, PHASE_UNLIMITED),

        HANDLER_NODE_INFO(DG_SUN_RANKING, DG_SUN_RANKING_SETTLE_SYN, PHASE_UNLIMITED),
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        HANDLER_NODE_INFO(DG_NOTIFICATION, DG_NOTIFICATION_ITEM_SYN, PHASE_UNLIMITED),
        HANDLER_NODE_INFO(DG_NOTIFICATION, DG_NOTIFICATION_CLEAR_SYN, PHASE_UNLIMITED),
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        //
        HANDLER_NODE_INFO_EOR() // end of records
    };
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

Handler_FromGameServer_IMPL(DG_CONNECTION_GROUND_CMD)
{
    __UNUSED((pServerSession, pMsg, wSize));
}

Handler_FromGameServer_IMPL(DG_CONNECTION_DBCONNECT_UPDATE_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CONNECTION_DBCONNECT_UPDATE_CMD* recv_msg = \
        static_cast<MSG_DG_CONNECTION_DBCONNECT_UPDATE_CMD*>(pMsg);
    //
    const DWORD db_unit_id = pServerSession->GetDBUnitID();
    g_DBHandler.UpdateDBConnectorIndex(db_unit_id, recv_msg->db_connector_index_);
}

Handler_FromGameServer_IMPL(DG_CHARINFO_CHAR_ITEM_CMD)
{
    __TOUCHED((pServerSession, wSize));
    // (f100527.3L) fit in a coding style
    MSG_DG_CHARINFO_CHAR_ITEM_CMD* recv_msg = static_cast<MSG_DG_CHARINFO_CHAR_ITEM_CMD*>(pMsg);
    const DWORD user_key = recv_msg->m_dwKey;
#ifdef __NA_PACKETFLOW_TEST    
    SUNLOG(eFULL_LOG, "Handler_FromGameServer_IMPL(DG_CHARINFO_CHAR_ITEM_CMD)");
#endif

    const CHARGUID char_guid = recv_msg->m_CharacterInfo.m_CharGuid;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_key, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%d) in user (%u)|"),
               __FUNCTION__, char_guid, user_key);
        return;
    };
    ASSERT(db_user->IsTransactionSerialize());
    db_character->SerializeCharInfo(recv_msg->m_CharacterInfo, SERIALIZE_STORE);
    //

    _ITEM_TOTAL_INFO* const item_block = &recv_msg->m_ItemInfo;
    //
    int offset = 0; // (f100527.3L) change type
    POSTYPE sum_of_node = item_block->m_EquipCount;
    db_character->SerializeItemStreamStore(SI_EQUIPMENT, \
                                           &item_block->m_Slot[offset], sum_of_node);
    //
    offset += sum_of_node; // change offset
    sum_of_node = item_block->m_InvenCount;
    db_character->SerializeItemStreamStore(SI_INVENTORY, \
                                           &item_block->m_Slot[offset], sum_of_node);
    //
    offset += sum_of_node;
    // CHANGES: f100928.2L, remove a tempinventory store routine
    // NOTE: f110429.4L, a dangling user occur possibility
    // when he moves a field server to a battle server
    db_user->UpdateLatestUpdateTick();
}

Handler_FromGameServer_IMPL(DG_CHARINFO_CHARLISTREQ_INTER_ITEMSLOTINFO_DBR)
{
    __TOUCHED((pServerSession, wSize));

    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    const nsQuery::ProxyTransaction transaction = { query_result->transaction_type() };
    const DWORD user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();

    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };
    //
    if (db_query_proxy_manager->DBResult_Char_Transaction(\
            *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }

    // success

RELEASE_RESOURCE:
    switch (transaction.value_)
    {
    case transaction.eTrans_ItemSlotInfo_Select: //S_ItemSlotInfo_Select
        Query_ItemSlotInfo_Select::FREE(static_cast<Query_ItemSlotInfo_Select*>(query_result));
        break;
    case transaction.eTrans_ItemSlotInfo_Update: //S_ItemSlotInfo_Update
        Query_ItemSlotInfo_Update::FREE(static_cast<Query_ItemSlotInfo_Update*>(query_result));
        break;
    }
}

Handler_FromGameServer_IMPL(DG_CHARINFO_CHARLISTREQ_INTER_SLOTINFO_DBR)
{
    __TOUCHED((pServerSession, wSize));

    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    const nsQuery::ProxyTransaction transaction = { query_result->transaction_type() };
    const DWORD user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();

    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };
    //
    if (db_query_proxy_manager->DBResult_Char_Transaction(\
            *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }

    // success

RELEASE_RESOURCE:
    switch (transaction.value_)
    {
    case transaction.eTrans_SlotInfo_Select: //S_SlotInfo_Select
        Query_SlotInfo_Select::FREE(static_cast<Query_SlotInfo_Select*>(query_result));
        break;
    case transaction.eTrans_SlotInfo_Insert: //S_SlotInfo_Insert
        Query_SlotInfo_Insert::FREE(static_cast<Query_SlotInfo_Insert*>(query_result));
        break;
    case transaction.eTrans_SlotInfo_Update: //S_SlotInfo_Update
        Query_SlotInfo_Update::FREE(static_cast<Query_SlotInfo_Update*>(query_result));
        break;
    }
}

Handler_FromGameServer_IMPL(DG_CHARINFO_CHARLISTREQ_INTER_PERK_DBR)
{
    __TOUCHED((pServerSession, wSize));

    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    const nsQuery::ProxyTransaction transaction = { query_result->transaction_type() };
    const DWORD user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();

    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
            __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };
    //
    if (db_query_proxy_manager->DBResult_Char_Transaction(\
            *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }

    // success

RELEASE_RESOURCE:
    switch (transaction.value_)
    {
    case transaction.eTrans_Perk_Select: //S_Perk_Select
        Query_Perk_Select::FREE(static_cast<Query_Perk_Select*>(query_result));
        break;
    case transaction.eTrans_Perk_Insert: //S_Perk_Insert
        Query_Perk_Insert::FREE(static_cast<Query_Perk_Insert*>(query_result));
        break;
    case transaction.eTrans_Perk_Delete: //S_Perk_Delete
        Query_Perk_Delete::FREE(static_cast<Query_Perk_Delete*>(query_result));
        break;
    case transaction.eTrans_Perk_Update: //S_Perk_Update
        Query_Perk_Update::FREE(static_cast<Query_Perk_Update*>(query_result));
        break;
    }
}

Handler_FromGameServer_IMPL(DG_CHARINFO_SKILL_LIST_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
}

Handler_FromGameServer_IMPL(DG_CHARINFO_SKILL_PERK_CMD)
{
    __TOUCHED((pServerSession, wSize));

    const MSG_DG_CHARINFO_SKILL_PERK_CMD* recv_msg = static_cast<MSG_DG_CHARINFO_SKILL_PERK_CMD*>(pMsg);
    const DWORD user_guid = recv_msg->m_dwKey;
    const DWORD char_guid = recv_msg->char_guid_;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%d) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    //
    nsQuery::DBQueryProxyCharNode* char_node = db_character->db_query_node();
    char_node->StorePerk(recv_msg->container_);
}

Handler_FromGameServer_IMPL(DG_CHARINFO_SKILL_PERK_CONTROL_CMD)
{
    __TOUCHED((pServerSession, wSize));

    const MSG_DG_CHARINFO_SKILL_PERK_CONTROL_CMD* recv_msg = \
        static_cast<MSG_DG_CHARINFO_SKILL_PERK_CONTROL_CMD*>(pMsg);
    const DWORD user_guid = recv_msg->m_dwKey;
    const PerkControl& header = recv_msg->header;
    const DWORD char_guid = header.char_guid;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%d) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    MSG_DG_CHARINFO_SKILL_PERK_CONTROL_CMD msg_notify;
    //
    nsQuery::DBQueryProxyCharNode* char_node = db_character->db_query_node();
    char_node->ProcessPerkOperation(header, &msg_notify.header, &msg_notify.data); // + data
    db_user->SendPacketToGameServer(&msg_notify, msg_notify.GetSize());
}

Handler_FromGameServer_IMPL(DG_CHARINFO_UPDATE_INTER_QUEST_DBR)
{
    __TOUCHED((pServerSession, wSize));

    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    const nsQuery::ProxyTransaction transaction = { query_result->transaction_type() };
    const DWORD user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();

    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };
    //
    if (db_query_proxy_manager->DBResult_Char_Transaction(\
            *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }

    // success

RELEASE_RESOURCE:
    switch (transaction.value_)
    {
    case transaction.eTrans_Quest_Select: //S_Perk_Select
        Query_Quest_Select::FREE(static_cast<Query_Quest_Select*>(query_result));
        break;
    case transaction.eTrans_Quest_Update: //S_Perk_Update
        Query_Quest_Update::FREE(static_cast<Query_Quest_Update*>(query_result));
        break;
    }
}

Handler_FromGameServer_IMPL(DG_CHARINFO_UPDATE_INTER_BATTLERECORDS_DBR)
{
    __TOUCHED((pServerSession, wSize));

    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    const nsQuery::ProxyTransaction transaction = { query_result->transaction_type() };
    const DWORD user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();

    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };
    //
    if (db_query_proxy_manager->DBResult_Char_Transaction(\
            *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }

    // success

RELEASE_RESOURCE:
    switch (transaction.value_)
    {
    case transaction.eTrans_BattleRecords_Select: //S_BattleRecords_Select
        Query_BattleRecords_Select::FREE(static_cast<Query_BattleRecords_Select*>(query_result));
        break;
    case transaction.eTrans_BattleRecords_Update: //S_BattleRecords_Update
        Query_BattleRecords_Update::FREE(static_cast<Query_BattleRecords_Update*>(query_result));
        break;
    }
}

Handler_FromGameServer_IMPL(DG_CHARINFO_CHARLISTREQ_INTER_CHAR_STATE_DBR)
{
    __TOUCHED((pServerSession, wSize));

    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    const nsQuery::ProxyTransaction transaction = { query_result->transaction_type() };
    const DWORD user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();

    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };
    //
    if (db_query_proxy_manager->DBResult_Char_Transaction(\
            *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }

    // success

RELEASE_RESOURCE:
    switch (transaction.value_)
    {
    case transaction.eTrans_Char_State_Select: //S_Char_State_Select
        Query_Char_State_Select::FREE(static_cast<Query_Char_State_Select*>(query_result));
        break;
    case transaction.eTrans_Char_State_Insert: //S_Char_State_Insert
        Query_Char_State_Insert::FREE(static_cast<Query_Char_State_Insert*>(query_result));
        break;
    case transaction.eTrans_Char_State_Delete: //S_Char_State_Delete
        Query_Char_State_Delete::FREE(static_cast<Query_Char_State_Delete*>(query_result));
        break;
    case transaction.eTrans_Char_State_Update: //S_Char_State_Update
        Query_Char_State_Update::FREE(static_cast<Query_Char_State_Update*>(query_result));
        break;
    }
}

Handler_FromGameServer_IMPL(DG_CHARINFO_CHARLISTREQ_INTER_MISSION_DBR)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    const nsQuery::ProxyTransaction transaction = { query_result->transaction_type() };
    const DWORD user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();

    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };
    //
    if (db_query_proxy_manager->DBResult_Char_Transaction(\
            *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }

    // success

RELEASE_RESOURCE:
    switch (transaction.value_)
    {
    case transaction.eTrans_MissionInfo_Select: // S_MissionInfo_Select
        Query_MissionInfo_Select::FREE(static_cast<Query_MissionInfo_Select*>(query_result));
        break;
    case transaction.eTrans_MissionInfo_Insert: // S_MissionInfo_Insert
        Query_MissionInfo_Insert::FREE(static_cast<Query_MissionInfo_Insert*>(query_result));
        break;
    case transaction.eTrans_MissionInfo_Update: // S_MissionInfo_Update
        Query_MissionInfo_Update::FREE(static_cast<Query_MissionInfo_Update*>(query_result));
        break;
    }
}


#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
Handler_FromGameServer_IMPL(DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD * pRecvMsg = (MSG_DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD *)pMsg;
    DWORD UserKey = pRecvMsg->m_dwKey;

    DBUser * pUser = g_DBUserManager.FindUser( UserKey );
    if( !pUser )
    {
        SUNLOG(eFULL_LOG,  "[(DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD)] [UserGuid:%u][ChannelID:%u]", UserKey, pRecvMsg->m_ChannelID );
        return;
    }

    if( !pUser->GetConnectedChar() )
    {
        SUNLOG(eFULL_LOG,  "[(DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD)] [UserGuid:%u]NotChar", UserKey );
        return;
    }

    if( pUser->GetChannelID() != pRecvMsg->m_ChannelID )
    {
        SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD] UserGuid(%u) Invalid ChannelID(%d/%d)", UserKey, pRecvMsg->m_ChannelID, pUser->GetChannelID() );
        return;
    }

    pUser->GetConnectedChar()->SerializeRepurchasItemInfo(pRecvMsg->m_RepurchasItem, SERIALIZE_STORE);
}
#endif //_NA_20100106_REPURCHAS_BUGFIX_2496

// implemented by _KR_0_20091021_FIRENDINVITATION_EXTEND
Handler_FromGameServer_IMPL(DG_CHARINFO_INVITATEDFRIEND_CMD)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DG_CHARINFO_INVITATEDFRIEND_CMD * pRecvMsg = (MSG_DG_CHARINFO_INVITATEDFRIEND_CMD *)pMsg;
    DWORD UserKey = pRecvMsg->m_dwKey;

    DBUser * pUser = g_DBUserManager.FindUser( UserKey );
    if( !pUser )
    {
        SUNLOG(eFULL_LOG,  "[(DG_CHARINFO_INVITATEDFRIEND_CMD)] [UserGuid:%u][ChannelID:%u]", UserKey, pRecvMsg->channel_id_ );
        return;
    }

    if( !pUser->GetConnectedChar() )
    {
        SUNLOG(eFULL_LOG,  "[(DG_CHARINFO_INVITATEDFRIEND_CMD)] [UserGuid:%u]NotChar", UserKey );
        return;
    }

    if( pUser->GetChannelID() != pRecvMsg->channel_id_ )
    {
        SUNLOG( eCRITICAL_LOG, "[DG_CHARINFO_INVITATEDFRIEND_CMD] UserGuid(%u) Invalid ChannelID(%d/%d)", UserKey, pRecvMsg->channel_id_, pUser->GetChannelID() );
        return;
    }

    pUser->GetConnectedChar()->SerializeFriendListInfo(pRecvMsg->friend_list_, SERIALIZE_STORE);
}

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
Handler_FromGameServer_IMPL(DG_CHARINFO_CHAR_ACHIEVEMENT_CMD)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD * pRecvMsg = (MSG_DG_CHARINFO_CHAR_ACHIEVEMENT_CMD *)pMsg;
	DWORD user_guid = pRecvMsg->m_dwKey;
    DWORD char_guid = pRecvMsg->char_guid_;
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
            __FUNCTION__, char_guid, user_guid);
        return;
    };
    //
	db_character->SerializeAchievementInfo(pRecvMsg->achievement_info_, SERIALIZE_STORE);
}
#endif

#ifdef _NA_0_20110308_SKILL_COOL_TIME_STORAGE
Handler_FromGameServer_IMPL(DG_CHARINFO_SKILL_COOL_TIME_CMD)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_DG_CHARINFO_SKILL_COOL_TIME_CMD* recv_msg = (MSG_DG_CHARINFO_SKILL_COOL_TIME_CMD*)pMsg;
    DWORD user_guid = recv_msg->m_dwKey;
    DWORD char_guid = recv_msg->char_guid_;
    //
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    //
    SkillCoolTimeStorage::Instance()->SaveSkillCoolTime(char_guid, &(recv_msg->skill_cool_time));
}
#endif

Handler_FromGameServer_IMPL(DG_CHARINFO_SKILL_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_SKILL_CMD* recv_msg = static_cast<MSG_DG_CHARINFO_SKILL_CMD*>(pMsg);
    const DWORD user_guid = recv_msg->m_dwKey;
    const DWORD char_guid = recv_msg->char_guid_;
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    // not check channel comparision
    ;{
        nsQuery::DBQueryProxyCharNode* db_query_node = db_character->db_query_node();
        db_query_node->StoreSkill(recv_msg->m_SkillInfo);
    };
}

Handler_FromGameServer_IMPL(DG_CHARINFO_QUICK_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_QUICK_CMD* recv_msg = static_cast<MSG_DG_CHARINFO_QUICK_CMD*>(pMsg);
    const DWORD user_guid = recv_msg->m_dwKey;
    const DWORD char_guid = recv_msg->char_guid_;
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    // not check channel comparision
    ;{
        nsQuery::DBQueryProxyCharNode* db_query_node = db_character->db_query_node();
        db_query_node->StoreQuick(recv_msg->m_QuickInfo);
    };
}

Handler_FromGameServer_IMPL(DG_CHARINFO_STYLE_CMD)
{
    __TOUCHED((pServerSession, wSize));
    __TOUCHED((pMsg));
    ASSERT(!"don't support anymore");
    return;
}

Handler_FromGameServer_IMPL(DG_CHARINFO_QUEST_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_QUEST_CMD* recv_msg = static_cast<MSG_DG_CHARINFO_QUEST_CMD*>(pMsg);
    const DWORD user_guid = recv_msg->m_dwKey;
    const DWORD char_guid = recv_msg->char_guid_;
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    // not check channel comparision
    if (nsQuery::DBQueryProxyCharNode* const query_char_node = db_character->db_query_node())
    {
        query_char_node->StoreQuest(*recv_msg);
        query_char_node->StoreMission(*recv_msg);
    };
  #if !defined(C_NA_0_20100520_ACSYSTEM_CHANGED)
    db_character->SerializeACStream(const_cast<BYTE*>(recv_msg->m_pACStream),
                                    MAX_ACSTREAM_SIZE, SERIALIZE_STORE);
  #endif
}

Handler_FromGameServer_IMPL(DG_CHARINFO_WAREHOUSE_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_WAREHOUSE_CMD* recv_msg = \
        static_cast<MSG_DG_CHARINFO_WAREHOUSE_CMD*>(pMsg);
    const DWORD user_guid = recv_msg->m_dwKey;
    //const DWORD char_guid = recv_msg->char_guid_;
    DBUser* db_user = NULL;
    //DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, &db_user);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find user (%u)|"),
               __FUNCTION__, user_guid);
        return;
    };
   // not check character & channel comparision
    // f101009.2L
    const _WAREHOUSE_TOTAL_INFO* const warehouse_item_block = &recv_msg->m_ItemInfo;
    nsQuery::DBQueryProxyUserNode* db_query_node = db_user->db_query_node();
    bool success = db_query_node->StoreItemSlotInfo(SI_WAREHOUSE,
        warehouse_item_block->m_Slot, warehouse_item_block->m_Count);
    db_user->SetWarehouseMoney(recv_msg->m_WarehouseMoney);
    db_user->SetNeedWarehouseToSave(true);
}

Handler_FromGameServer_IMPL(DG_CHARINFO_EXTRA_CMD)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DG_CHARINFO_EXTRA_CMD* recv_msg = static_cast<MSG_DG_CHARINFO_EXTRA_CMD*>(pMsg);
    const DWORD user_guid = recv_msg->m_dwKey;
    const DWORD char_guid = recv_msg->char_guid_;
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        return;
    };
    // not check channel comparision
    db_character->SerializeStateInfo(const_cast<STATE_DETAIL_TOTAL_INFO&>(recv_msg->m_TotalInfo),
                                     const_cast<STATE_GM_INFO&>(recv_msg->m_GMStateInfo),
                                     SERIALIZE_STORE);
    db_character->SetLoginTime(recv_msg->m_LoginTime_t);
}

Handler_FromGameServer_IMPL(DG_ITEM_SERIAL_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_ITEM_SERIAL_SYN * pRecvMsg = (MSG_DG_ITEM_SERIAL_SYN *)pMsg;
	MSG_DG_ITEM_SERIAL_ACK msg;
	msg.m_dwKey		= pRecvMsg->m_dwKey;
	msg.m_SlotIdx	= pRecvMsg->m_SlotIdx;
	msg.m_ItemPos	= pRecvMsg->m_ItemPos;
	msg.m_Type		= g_DBProxyServer.GetItemWorldID();

	msg.m_DBSerial	= g_pDBSerialManager->AllocSerial();
	if( msg.m_DBSerial != 0 )
	{
		//SUNLOG(eFULL_LOG,  "[g_DBProxyServer.]: 아이템 시리얼 g_DBProxyServer.(%d)OK", msg.m_DBSerial );
		/*SUNLOG( eFULL_LOG, "[아이템시리얼발급]:[%s],%u,%u,%u,%u", 
					GetServerType2String(pServerSession->GetServerType(), 
					msg.m_dwKey, msg.m_SlotIdx, msg.m_ItemPos, msg.m_DBSerial );*/
		pServerSession->Send( (BYTE *)&msg, sizeof(msg) );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[아이템시리얼발급오류]:[%s] DBSerial is 0!", GetServerType2String(pServerSession->GetServerType()) );

		MSG_DG_ITEM_SERIAL_NAK nmsg;
		nmsg.m_dwKey	= pRecvMsg->m_dwKey;
		nmsg.m_SlotIdx	= pRecvMsg->m_SlotIdx;
		nmsg.m_ItemPos	= pRecvMsg->m_ItemPos;
		pServerSession->Send( (BYTE *)&nmsg, sizeof(nmsg) );
	}
}

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
Handler_FromGameServer_IMPL( DG_OTHER_EXIST_CHARNAME_SYN )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DG_OTHER_EXIST_CHARNAME_SYN* pRecvMsg = (MSG_DG_OTHER_EXIST_CHARNAME_SYN*)pMsg;
    DWORD userKey = pRecvMsg->m_dwKey;
    DBUser* pUser = g_DBUserManager.FindUser(userKey);
    if(!pUser)
    {
        SUNLOG(eMIDDLE_LOG, "[DG_OTHER_EXIST_CHARNAME_SYN]:[ID:%u]:DBUserNotFound", userKey);
        return;
    }
    if(!pUser->GetConnectedChar())
    {
        SUNLOG(eMIDDLE_LOG, "[DG_OTHER_EXIST_CHARNAME_SYN]:[ID:%u]:DBCharNotFound", userKey);
        return;
    }

    STRING_SAFER_CHAR_NAME szCharName(pRecvMsg->m_tszCharName);

    STRING_SAFER_QUERY64 szQueryBuff;
    szQueryBuff.MakeString("{call S_Char_Exist('%s',?,?,?,?)}", (TCHAR*)szCharName);

    Query_Character_Exist* pQuery = Query_Character_Exist::ALLOC();
    pQuery->Clear();
    pQuery->SetQuery(szQueryBuff);
    pQuery->SetUserKey(pRecvMsg->m_dwKey);
    pQuery->SetIndex(MAKEDWORD((WORD)DG_OTHER, (WORD)DG_OTHER_EXIST_CHARNAME_DBR));
    pQuery->SetVoidObject(pServerSession);
    pQuery->SetParamCharName(pRecvMsg->m_tszCharName);
    pServerSession->DBQuery(pQuery);
}

Handler_FromGameServer_IMPL( DG_OTHER_EXIST_CHARNAME_DBR )
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* pResult	= (MSG_DBPROXY_RESULT *)pMsg;
    Query_Character_Exist* pQResult		= (Query_Character_Exist *)pResult->m_pData;
    int  ResultNum					= pQResult->GetResultRowNum();
    DWORD UserKey					= pQResult->GetUserKey();

    DBUser* pUser = g_DBUserManager.FindUser(UserKey);
    if(!pUser) 
    {
        SUNLOG(eFULL_LOG, "[DG_OTHER_EXIST_CHARNAME_DBR][UGuid:%u]DBUserNotFound", UserKey);
        Query_Character_Exist::FREE(pQResult); pQResult = NULL;
        return;
    }
    if(!pUser->GetConnectedChar())
    {
        SUNLOG(eFULL_LOG, "[DG_OTHER_EXIST_CHARNAME_DBR]:[ID:%u]DBCharNotFound", UserKey);
        Query_Character_Exist::FREE(pQResult); pQResult = NULL;
        return;
    }
    if(!pQResult->ResultSuccess())
    {
        MSG_DG_OTHER_EXIST_CHARNAME_NAK msg;
        msg.m_dwKey = UserKey;
        pServerSession->SendPacket(&msg, sizeof(msg));
        Query_Character_Exist::FREE(pQResult); pQResult = NULL;
        return;
    }

    MSG_DG_OTHER_EXIST_CHARNAME_ACK msg;
    const uint count = _countof(msg.m_tszCharName);
    msg.m_dwKey = UserKey;
    msg.m_UserGuid = pQResult->pParam[0].m_UserGuid;
    msg.m_CharGuid = pQResult->pParam[0].m_CharGuid;
    msg.character_class_ = pQResult->pParam[0].m_CharClass;
    msg.character_level_ = pQResult->pParam[0].m_CharLevel;
    _tcsncpy(msg.m_tszCharName, pQResult->GetParamCharName(), count);
    msg.m_tszCharName[count - 1] = _T('\0');
    pServerSession->SendPacket(&msg, sizeof(msg));

    Query_Character_Exist::FREE(pQResult); pQResult = NULL;
}
#else //_NA_0_20110613_GAME_SHOP_RENEWAL
Handler_FromGameServer_IMPL( DG_OTHER_EXIST_CHARNAME_SYN )
{
    __TOUCHED((pServerSession, wSize));
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    __TOUCHED((pMsg));
    ASSERT(!"function not supported!");
#else //!__NA000000_SHOP_USERID_50_PASSING_PROC__
	MSG_DG_OTHER_EXIST_CHARNAME_SYN* pRecvMsg = (MSG_DG_OTHER_EXIST_CHARNAME_SYN*)pMsg;
	DWORD userKey = pRecvMsg->m_dwKey;
	DBUser* pUser = g_DBUserManager.FindUser(userKey);
	if(!pUser)
	{
		SUNLOG(eMIDDLE_LOG, "[DG_OTHER_EXIST_CHARNAME_SYN]:[ID:%u]:DBUserNotFound", userKey);
		return;
	}
	if(!pUser->GetConnectedChar())
	{
		SUNLOG(eMIDDLE_LOG, "[DG_OTHER_EXIST_CHARNAME_SYN]:[ID:%u]:DBCharNotFound", userKey);
		return;
	}

	STRING_SAFER_CHAR_NAME szCharName(pRecvMsg->m_tszCharName);

	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString("{call S_Char_Exist('%s',?,?)}", (TCHAR*)szCharName);

	Query_Char_Exist* pQuery = Query_Char_Exist::ALLOC();
    pQuery->Clear();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey(pRecvMsg->m_dwKey);
	pQuery->SetIndex(MAKEDWORD((WORD)DG_OTHER, (WORD)DG_OTHER_EXIST_CHARNAME_DBR));
    pQuery->SetVoidObject(pServerSession);
    pQuery->SetType(pRecvMsg->m_Type);
	pQuery->SetParamCharName(pRecvMsg->m_tszCharName);
	pServerSession->DBQuery(pQuery);
#endif //!__NA000000_SHOP_USERID_50_PASSING_PROC__
}

Handler_FromGameServer_IMPL( DG_OTHER_EXIST_CHARNAME_DBR )
{
    __TOUCHED((pServerSession, wSize));
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    __TOUCHED((pMsg));
    ASSERT(!"function not supported!");
#else //!__NA000000_SHOP_USERID_50_PASSING_PROC__
	MSG_DBPROXY_RESULT* pResult	= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Char_Exist* pQResult		= (Query_Char_Exist *)pResult->m_pData;
	int  ResultNum					= pQResult->GetResultRowNum();
	DWORD UserKey					= pQResult->GetUserKey();

	DBUser* pUser = g_DBUserManager.FindUser(UserKey);
	if(!pUser) 
	{
		SUNLOG(eFULL_LOG, "[DG_OTHER_EXIST_CHARNAME_DBR][UGuid:%u]DBUserNotFound", UserKey);
		Query_Char_Exist::FREE(pQResult); pQResult = NULL;
		return;
	}
	if(!pUser->GetConnectedChar())
	{
		SUNLOG(eFULL_LOG, "[DG_OTHER_EXIST_CHARNAME_DBR]:[ID:%u]DBCharNotFound", UserKey);
		Query_Char_Exist::FREE(pQResult); pQResult = NULL;
		return;
	}
	if(!pQResult->ResultSuccess())
	{
		MSG_DG_OTHER_EXIST_CHARNAME_NAK msg;
		msg.m_dwKey = UserKey;
		msg.m_Type = pQResult->GetType();
		pServerSession->SendPacket(&msg, sizeof(msg));
		Query_Char_Exist::FREE(pQResult); pQResult = NULL;
		return;
	}

	MSG_DG_OTHER_EXIST_CHARNAME_ACK msg;
	msg.m_dwKey = UserKey;
	msg.m_Type = pQResult->GetType();
	msg.m_UserGuid = pQResult->GetResultUserGuid();
	msg.m_CharGuid = pQResult->GetResultCharGuid();
    _tcsncpy(msg.m_tszCharName, pQResult->GetParamCharName(), _countof(msg.m_tszCharName));
    msg.m_tszCharName[_countof(msg.m_tszCharName) - 1] = _T('\0');
	pServerSession->SendPacket(&msg, sizeof(msg));

	Query_Char_Exist::FREE(pQResult); pQResult = NULL;
#endif //!__NA000000_SHOP_USERID_50_PASSING_PROC__
}
#endif //_NA_0_20110613_GAME_SHOP_RENEWAL

Handler_FromGameServer_IMPL( DG_OTHER_SHOP_RECV_GIFT_NTF )
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_OTHER_SHOP_RECV_GIFT_NTF * pRecvMsg = (MSG_DG_OTHER_SHOP_RECV_GIFT_NTF*)pMsg;
	// UserKey == UserKey이라는 가정
	DWORD UserKey = pRecvMsg->m_UserGuid;
	DBUser * pTargetUser = g_DBUserManager.FindUser( UserKey );
	if( !pTargetUser )
	{
		SUNLOG( eFULL_LOG, "MSG_DG_OTHER_SHOP_RECV_GIFT_NTF:User[U:%u,C:%u] is off-line", 
			pRecvMsg->m_UserGuid, pRecvMsg->m_CharGuid );
		return;
	}

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
	if( !pTargetUser->GetConnectedChar() || 
		pTargetUser->GetConnectedChar()->GetCharGuid() != pRecvMsg->m_CharGuid )
	{
		SUNLOG( eFULL_LOG, "MSG_DG_OTHER_SHOP_RECV_GIFT_NTF:Char[U:%u,C:%u] is off-line", 
			pRecvMsg->m_UserGuid, pRecvMsg->m_CharGuid );
		return;
	}
#endif

	if( pTargetUser->GetServerSession() )
	{
		MSG_DG_OTHER_SHOP_RECV_GIFT_NTF ntf;
		ntf.m_dwKey = UserKey;
		ntf.m_UserGuid = pRecvMsg->m_UserGuid;
		ntf.m_CharGuid = pRecvMsg->m_CharGuid;
		pTargetUser->GetServerSession()->SendPacket( &ntf, sizeof(ntf) );
	}
}

Handler_FromGameServer_IMPL(DG_WAYPOINT_INFO_LOAD_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_WAYPOINT_INFO_LOAD_SYN * pRecvMsg = (MSG_DG_WAYPOINT_INFO_LOAD_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG( eMIDDLE_LOG, "[DG_WAYPOINT_INFO_LOAD_SYN]:[ID:%u]:DBUserNotFound", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG( eMIDDLE_LOG, "[DG_WAYPOINT_INFO_LOAD_SYN]:[ID:%u]:DBCharNotFound", UserKey );
		return;
	}
	

	if( pUser->GetConnectedChar()->GetWayPoint()->IsLoaded() )
	{
		MSG_DG_WAYPOINT_INFO_LOAD_ACK msg;
		msg.m_dwKey = UserKey;
		pUser->GetConnectedChar()->GetWayPoint()->Serialize( msg.m_Info, SERIALIZE_LOAD );
		pServerSession->SendPacket( &msg, sizeof(msg) );
		return;
	}
	else
	{
		if( 0 != pUser->GetConnectedChar()->GetWayPoint()->GetCount() )
		{
			// 우선 임시로 막음!, 나중에 버그 잡으삼.
			// SUNLOG( eFULL_LOG, "[DBP]웨이포인트 개수가 0이 아니다.\n" );
		}
		pUser->GetConnectedChar()->GetWayPoint()->Clear();
	}

	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "S_WP_Select %d", pUser->GetConnectedChar()->GetCharGuid() );

	Query_WayPoint_Select* pQuery = Query_WayPoint_Select::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetVoidObject( pServerSession );
	pUser->DBQuery(DG_WAYPOINT, DG_WAYPOINT_INFO_LOAD_DBR, pQuery);

}

Handler_FromGameServer_IMPL(DG_WAYPOINT_INFO_LOAD_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_WayPoint_Select* pQResult	= (Query_WayPoint_Select *)pResult->m_pData;
	int  ResultNum					= pQResult->GetResultRowNum();
	DWORD UserKey					= pQResult->GetUserKey();

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
	{
		SUNLOG( eFULL_LOG, "[DG_WAYPOINT_INFO_LOAD_DBR][UGuid:%u]WPLoad후바로나감", UserKey );
		Query_WayPoint_Select::FREE(pQResult); pQResult = NULL;
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_WAYPOINT_INFO_LOAD_SYN]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		Query_WayPoint_Select::FREE(pQResult); pQResult = NULL;
		return;
	}

	for( int i = 0 ; i < ResultNum ; ++i )
	{
		BOOL rt = pUser->GetConnectedChar()->GetWayPoint()->Insert(pQResult->pResult[i].m_MapCode);
		ASSERT( rt && "WP중복삽입 안됨" );
	}
	pUser->GetConnectedChar()->GetWayPoint()->SetLoaded(true);

	MSG_DG_WAYPOINT_INFO_LOAD_ACK msg;
	msg.m_dwKey = UserKey;
	pUser->GetConnectedChar()->GetWayPoint()->Serialize( msg.m_Info, SERIALIZE_LOAD );
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_WayPoint_Select::FREE(pQResult); pQResult = NULL;
}

Handler_FromGameServer_IMPL(DG_WAYPOINT_INFO_SAVE_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_WAYPOINT_INFO_SAVE_SYN * pRecvMsg = (MSG_DG_WAYPOINT_INFO_SAVE_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[DG_WAYPOINT_INFO_SAVE_SYN]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_WAYPOINT_INFO_SAVE_SYN]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		return;
	}

	if( pUser->GetConnectedChar()->GetWayPoint()->IsExist(pRecvMsg->m_MapCode) )
	{
		MSG_DG_WAYPOINT_INFO_SAVE_NAK msg;
		msg.m_dwKey			= UserKey;
		msg.m_dwErrorCode	= RC::RC_WP_ALREADY_SAVE_MAPCODE;
		pServerSession->SendPacket( &msg, sizeof(msg) );

		SUNLOG(eFULL_LOG, "[DG_WAYPOINT_INFO_SAVE_SYN]:[C:%s]중복WPIndex:%d", pUser->GetConnectedChar()->GetCharName(), pRecvMsg->m_MapCode );
		return;
	}

	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_WP_Insert(%d,%d)}", pUser->GetConnectedChar()->GetCharGuid(), pRecvMsg->m_MapCode );

	Query_WayPoint_Insert * pQuery = Query_WayPoint_Insert::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_WAYPOINT, (WORD)DG_WAYPOINT_INFO_SAVE_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pQuery->SetMapCode( pRecvMsg->m_MapCode );

	pUser->DBQuery(DG_WAYPOINT, DG_WAYPOINT_INFO_SAVE_DBR, pQuery);
}

Handler_FromGameServer_IMPL(DG_WAYPOINT_INFO_SAVE_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_WayPoint_Insert * pQResult	= (Query_WayPoint_Insert *)pResult->m_pData;
	BYTE  ResultNum						= (BYTE)pQResult->GetResultRowNum();
	DWORD UserKey						= pQResult->GetUserKey();

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
	{
		SUNLOG( eFULL_LOG, "[DG_WAYPOINT_INFO_SAVE_DBR][UGuid:%u]WPSave후바로나감", UserKey );
		Query_WayPoint_Insert::FREE(pQResult); pQResult = NULL;
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_WAYPOINT_INFO_SAVE_DBR]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		Query_WayPoint_Insert::FREE(pQResult); pQResult = NULL;
		return;
	}

	if( !pQResult->ResultSuccess() )
	{
		MSG_DG_WAYPOINT_INFO_SAVE_NAK msg;
		msg.m_dwKey			= pQResult->GetUserKey();
		msg.m_dwErrorCode	= RC::RC_WP_ALREADY_SAVE_MAPCODE;
		pServerSession->SendPacket( &msg, sizeof(msg) );
		// 임시 주석 처리
		//SUNLOG( eCRITICAL_LOG, "[DG_WAYPOINT_INFO_SAVE_DBR][오류]%s", pQResult->GetQuery() );
		Query_WayPoint_Insert::FREE(pQResult); pQResult = NULL;
		return;
	}

	//
	BOOL rt = pUser->GetConnectedChar()->GetWayPoint()->Insert( pQResult->GetMapCode() );
	ASSERT( rt && "[DG_WAYPOINT_INFO_SAVE_DBR]WP중복삽입불가" );

	MSG_DG_WAYPOINT_INFO_SAVE_ACK msg;
	msg.m_dwKey = pQResult->GetUserKey();
	msg.m_MapCode = pQResult->GetMapCode();
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_WayPoint_Insert::FREE(pQResult); pQResult = NULL;
}


//{__NA001164_20081024_CHAR_WAY_POINT_SET_DB_PACKET

Handler_FromGameServer_IMPL(DG_CHAR_WAYPOINT_INFO_LOAD_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_CHAR_WAYPOINT_INFO_LOAD_SYN * pRecvMsg = (MSG_DG_CHAR_WAYPOINT_INFO_LOAD_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG( eMIDDLE_LOG, "[DG_CHAR_WAYPOINT_INFO_LOAD_SYN]:[ID:%u]:DBUserNotFound", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG( eMIDDLE_LOG, "[DG_CHAR_WAYPOINT_INFO_LOAD_SYN]:[ID:%u]:DBCharNotFound", UserKey );
		return;
	}


	if( pUser->GetConnectedChar()->GetCharWayPoint()->IsLoaded() )
	{
		MSG_DG_CHAR_WAYPOINT_INFO_LOAD_ACK msg;
		msg.m_dwKey = UserKey;
		pUser->GetConnectedChar()->GetCharWayPoint()->Serialize( msg.m_Info, SERIALIZE_LOAD );
		pServerSession->SendPacket( &msg, sizeof(msg) );
		return;
	}
	else
	{
		if( 0 != pUser->GetConnectedChar()->GetCharWayPoint()->GetCount() )
		{
			 SUNLOG( eFULL_LOG, "[DBP]개인 웨이포인트 개수가 0이 아니다.\n" );
		}
		pUser->GetConnectedChar()->GetCharWayPoint()->Clear();
	}

	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "S_WayPoint_Select %d", pUser->GetConnectedChar()->GetCharGuid() );

	Query_Char_WayPoint_Select* pQuery = Query_Char_WayPoint_Select::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_WAYPOINT, (WORD)DG_CHAR_WAYPOINT_INFO_LOAD_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pUser->DBQuery(DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_LOAD_DBR, pQuery);

}

Handler_FromGameServer_IMPL(DG_CHAR_WAYPOINT_INFO_LOAD_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult			 = (MSG_DBPROXY_RESULT *)pMsg;
	Query_Char_WayPoint_Select* pQResult = (Query_Char_WayPoint_Select *)pResult->m_pData;
	int  ResultNum						 = pQResult->GetResultRowNum();
	DWORD UserKey						 = pQResult->GetUserKey();

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
	{
		SUNLOG( eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_LOAD_DBR][UGuid:%u]CWPLoad후바로나감", UserKey );
		Query_Char_WayPoint_Select::FREE(pQResult); pQResult = NULL;
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_LOAD_DBR]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		Query_Char_WayPoint_Select::FREE(pQResult); pQResult = NULL;
		return;
	}

	for( int i = 0 ; i < ResultNum ; ++i )
	{
		BOOL rt = pUser->GetConnectedChar()->GetCharWayPoint()->Insert( *((WAYPOINTSLOT*)&pQResult->pResult[i]) );
		ASSERT( rt && "CWP중복삽입 안됨" );
	}
	pUser->GetConnectedChar()->GetCharWayPoint()->SetLoaded(true);

	MSG_DG_CHAR_WAYPOINT_INFO_LOAD_ACK msg;
	msg.m_dwKey = UserKey;
	pUser->GetConnectedChar()->GetCharWayPoint()->Serialize( msg.m_Info, SERIALIZE_LOAD );
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Char_WayPoint_Select::FREE(pQResult); pQResult = NULL;
}

Handler_FromGameServer_IMPL(DG_CHAR_WAYPOINT_INFO_SAVE_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_CHAR_WAYPOINT_INFO_SAVE_SYN * pRecvMsg = (MSG_DG_CHAR_WAYPOINT_INFO_SAVE_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_SAVE_SYN]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_SAVE_SYN]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		return;
	}

	// 동일 좌표가 있을시 NAK? 아니면 동일 좌표도 가능?

	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_WayPoint_Insert(%d,%u,%d,%d,%d)}", 
							pUser->GetConnectedChar()->GetCharGuid(), pRecvMsg->m_MapCode, pRecvMsg->m_X, pRecvMsg->m_Y, pRecvMsg->m_Z );

	Query_Char_WayPoint_Insert * pQuery = Query_Char_WayPoint_Insert::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_WAYPOINT, (WORD)DG_CHAR_WAYPOINT_INFO_SAVE_DBR ) );
	pQuery->SetVoidObject( pServerSession );

	pQuery->SetMapCode( pRecvMsg->m_MapCode );
	pQuery->SetX( pRecvMsg->m_X );
	pQuery->SetY( pRecvMsg->m_Y );
	pQuery->SetZ( pRecvMsg->m_Z );

	pUser->DBQuery(DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_SAVE_DBR, pQuery);
}

Handler_FromGameServer_IMPL(DG_CHAR_WAYPOINT_INFO_SAVE_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult			= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Char_WayPoint_Insert * pQResult	= (Query_Char_WayPoint_Insert *)pResult->m_pData;
	BYTE  ResultNum							= (BYTE)pQResult->GetResultRowNum();
	DWORD UserKey							= pQResult->GetUserKey();

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
	{
		SUNLOG( eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_SAVE_DBR][UGuid:%u]CWPSave후바로나감", UserKey );
		Query_Char_WayPoint_Insert::FREE(pQResult); pQResult = NULL;
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_SAVE_DBR]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		Query_Char_WayPoint_Insert::FREE(pQResult); pQResult = NULL;
		return;
	}

	if( !pQResult->ResultSuccess() )
	{
		MSG_DG_CHAR_WAYPOINT_INFO_NAK msg;
		msg.m_dwKey			= pQResult->GetUserKey();
		msg.m_dwErrorCode	= RC::RC_WP_CANNOT_SAVE_FROM_DB;
		pServerSession->SendPacket( &msg, sizeof(msg) );
	
		SUNLOG( eCRITICAL_LOG, "[DG_CHAR_WAYPOINT_INFO_SAVE_DBR][오류]%s", pQResult->GetQuery() );
		Query_Char_WayPoint_Insert::FREE(pQResult); pQResult = NULL;
		return;
	}

	//
	WAYPOINTSLOT stWayPoint;
	stWayPoint.m_IdxWayPoint = pQResult->pResult[0].m_IdxWayPoint;
	stWayPoint.m_MapCode	 = pQResult->GetMapCode();
	stWayPoint.m_X			 = pQResult->GetX();
	stWayPoint.m_Y			 = pQResult->GetY();
	stWayPoint.m_Z			 = pQResult->GetZ();

	BOOL rt = pUser->GetConnectedChar()->GetCharWayPoint()->Insert( stWayPoint );
	ASSERT( rt && "[MSG_DG_CHAR_WAYPOINT_INFO_SAVE_NAK]CWP중복삽입불가" );

	MSG_DG_CHAR_WAYPOINT_INFO_SAVE_ACK msg;
	msg.m_dwKey = pQResult->GetUserKey();
	msg.m_Info  = stWayPoint;
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Char_WayPoint_Insert::FREE(pQResult); pQResult = NULL;
}

Handler_FromGameServer_IMPL(DG_CHAR_WAYPOINT_INFO_DEL_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_CHAR_WAYPOINT_INFO_DEL_SYN * pRecvMsg = (MSG_DG_CHAR_WAYPOINT_INFO_DEL_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_DEL_SYN]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_DEL_SYN]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		return;
	}

	if( !pUser->GetConnectedChar()->GetCharWayPoint()->IsExist(pRecvMsg->m_IdxWayPoint) )
	{
		MSG_DG_CHAR_WAYPOINT_INFO_NAK msg;
		msg.m_dwKey			= UserKey;
		msg.m_dwErrorCode	= RC::RC_WP_CANNOT_FIND_WP_IDX;
		pServerSession->SendPacket( &msg, sizeof(msg) );

		SUNLOG(eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_DEL_SYN]:[C:%s]중복CWPIndex:%d", pUser->GetConnectedChar()->GetCharName(), pRecvMsg->m_IdxWayPoint );
		return;
	}

	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_WayPoint_Delete(%d,%d)}", pUser->GetConnectedChar()->GetCharGuid(), pRecvMsg->m_IdxWayPoint );

	Query_Char_WayPoint_Delete * pQuery = Query_Char_WayPoint_Delete::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_WAYPOINT, (WORD)DG_CHAR_WAYPOINT_INFO_DEL_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pQuery->SetIdxWayPoint( pRecvMsg->m_IdxWayPoint );

	pUser->DBQuery(DG_WAYPOINT, DG_CHAR_WAYPOINT_INFO_DEL_DBR, pQuery);
}

Handler_FromGameServer_IMPL(DG_CHAR_WAYPOINT_INFO_DEL_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult			= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Char_WayPoint_Delete * pQResult	= (Query_Char_WayPoint_Delete *)pResult->m_pData;
	BYTE  ResultNum							= (BYTE)pQResult->GetResultRowNum();
	DWORD UserKey							= pQResult->GetUserKey();

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
	{
		SUNLOG( eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_DEL_DBR][UGuid:%u]CWPSave후바로나감", UserKey );
		Query_Char_WayPoint_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_CHAR_WAYPOINT_INFO_DEL_DBR]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		Query_Char_WayPoint_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}

	if( !pQResult->ResultSuccess() )
	{
		MSG_DG_CHAR_WAYPOINT_INFO_NAK msg;
		msg.m_dwKey			= pQResult->GetUserKey();
		msg.m_dwErrorCode	= RC::RC_WP_CANNOT_DELETE_FROM_DB;
		pServerSession->SendPacket( &msg, sizeof(msg) );
		
		SUNLOG( eCRITICAL_LOG, "[DG_CHAR_WAYPOINT_INFO_DEL_DBR][오류]%s", pQResult->GetQuery() );
		Query_Char_WayPoint_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}

	BOOL rt = pUser->GetConnectedChar()->GetCharWayPoint()->Delete( pQResult->GetIdxWayPoint() );
	ASSERT( rt && "[DG_CHAR_WAYPOINT_INFO_DEL_DBR]해당 웨이 포인트가 존재하지 않는다" );

	MSG_DG_CHAR_WAYPOINT_INFO_DEL_ACK msg;
	msg.m_dwKey = pQResult->GetUserKey();
	msg.m_IdxWayPoint  = pQResult->GetIdxWayPoint();
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Char_WayPoint_Delete::FREE(pQResult); pQResult = NULL;
}

//}__NA001164_20081024_CHAR_WAY_POINT_SET_DB_PACKET

Handler_FromGameServer_IMPL(DG_WAREHOUSE_FLAG_STREAM_NTF)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_WAREHOUSE_FLAG_STREAM_NTF* pRecvMsg = (MSG_DG_WAREHOUSE_FLAG_STREAM_NTF*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_WAREHOUSE_FLAG_STREAM_NTF]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_WAREHOUSE_FLAG_STREAM_NTF]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		return;
	}
//printf( "DEBUG ->DB WARE_NTF %08X\n", pRecvMsg->m_dwFlagStream );
	pUser->WarehouseFlagStream() = pRecvMsg->m_dwFlagStream;
}


Handler_FromGameServer_IMPL(DG_WAREHOUSE_PWD_SETTING_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_WAREHOUSE_PWD_SETTING_SYN* pRecvMsg = (MSG_DG_WAREHOUSE_PWD_SETTING_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser* pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_WAREHOUSE_FLAG_STREAM_NTF]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}

	DBCharacter* pCharacter = pUser->GetConnectedChar();
	if( !pCharacter )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_WAREHOUSE_FLAG_STREAM_NTF]:[ID:%u]:ID에 맞는 DBChar가 없습니다.", UserKey );
		return;
	}

	// (WAVERIX_TEST) 구현부가 빠져 있다. 잊지 말고 처리할 것
	BOOL bProcess = TRUE;
	BYTE byTypeCode = pRecvMsg->GetTypeCode();
	sPASSWORD_BLOCK_IN_DBPROXY* pPwdBlock = g_DBProxyServer.AllocTempPasswordBlock();
	if( !pPwdBlock
		|| (pUser->PasswordTransaction() != DBUser::ePWD_NO_TRANS)
		)
	{
		MSG_DG_WAREHOUSE_PWD_SETTING_ACK msgACK;
		msgACK.m_byRetCode = MSG_DG_WAREHOUSE_PWD_SETTING_ACK::RET_START_FAIL;
		pUser->SendPacketToGameServer( &msgACK, sizeof(msgACK) );

		SUNLOG( eCRITICAL_LOG, "[DG_WAREHOUSE_PWD_SETTING_SYN]Can't Alloc TempNode" );
		return;
	}
	pPwdBlock->Set( pRecvMsg->Get() );

	if( MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_INVENTORY == byTypeCode)
	{
		pUser->PasswordTransaction() = DBUser::ePWD_TRANS_INVENTORY;
		pPwdBlock->Type() = sPASSWORD_BLOCK_IN_DBPROXY::INVENTORY;
		pCharacter->SetTempInventoryPassword( pPwdBlock );
		//pCharacter->SetInventoryPassword( pRecvMsg->Get() );
	}
	else if( MSG_DG_WAREHOUSE_PWD_SETTING_SYN::PWD_WAREHOUSE == byTypeCode)
	{
		pUser->PasswordTransaction() = DBUser::ePWD_TRANS_WAREHOUSE;
		pPwdBlock->Type() = sPASSWORD_BLOCK_IN_DBPROXY::WAREHOUSE;
		pUser->SetTempWarehousePassword( pPwdBlock );
		//pUser->SetWarehousePassword( pRecvMsg->Get() );
	}
	else
		bProcess = FALSE;

	if( bProcess )
	{
		STRING_SAFER_QUERY64 szQueryBuff;
		szQueryBuff.MakeString(
			"{?=call S_PassWD_Update (%d,%d,%d,'%s')}",
			byTypeCode, pUser->GetUserGuid(), pUser->GetConnectedChar()->GetCharGuid(), pRecvMsg->Get() );

		Query_Password_Update* pQuery = Query_Password_Update::ALLOC();
		pQuery->SetQuery(szQueryBuff);
		pQuery->SetUserKey( pRecvMsg->m_dwKey );
		//pQuery->SetIndex( MAKEDWORD( (WORD)DG_CHARINFO, (WORD)DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR ) );
		pQuery->SetVoidObject( pServerSession );

		pUser->DBQuery(DG_CHARINFO, DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR, pQuery);
	}
}

Handler_FromGameServer_IMPL(DG_ITEM_INVENTORY_FLAG_STREAM_NTF)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_WAREHOUSE_FLAG_STREAM_NTF* pRecvMsg = (MSG_DG_WAREHOUSE_FLAG_STREAM_NTF*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser* pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_WAREHOUSE_FLAG_STREAM_NTF]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}

	DBCharacter* pCharacter = pUser->GetConnectedChar();
	if( !pCharacter )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_WAREHOUSE_FLAG_STREAM_NTF]:[ID:%u]:ID에 맞는 DBCharacter가 없습니다.", UserKey );
		return;
	}
//printf( "DEBUG ->DB INVEN_NTF [%d] %08X\n", pServerSession->GetSessionIndex(), pRecvMsg->m_dwFlagStream );
	pCharacter->InventoryFlagStream() = pRecvMsg->m_dwFlagStream;
}

#ifdef _NA_008012_20150130_RANKING_SYSTEM
Handler_FromGameServer_IMPL(DG_MISSION_RANKING_POINT_REQ_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_MISSION_RANKING_POINT_REQ_SYN* pRecvMsg = (MSG_DG_MISSION_RANKING_POINT_REQ_SYN*)pMsg;	

	static char buff[128];
	BYTE m_Count = 1;			//랭킹포인트는 개인이므로 무조건 1
	std::string query_string  = "{?=call S_Ranking_Insert_MissionType2(?," ;
	query_string = query_string+_itoa(pRecvMsg->RecordInfo.m_MissionNo, buff, 10 )+",";
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    query_string = query_string+_itoa(pRecvMsg->RecordInfo.ranking_type, buff, 10 )+",";
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	query_string = query_string+_itoa(pRecvMsg->RecordInfo.m_ClearTime, buff, 10 )+",";
	query_string = query_string+_itoa(pRecvMsg->MemberInfo.m_LV, buff, 10 )+",";
	query_string = query_string+_itoa(m_Count, buff, 10 )+",";
	query_string = query_string+_i64toa(pRecvMsg->RecordInfo.m_RegisterTime, buff, 10 )+",'";
	util::GetSQLDateTime_YYYYMMDDHHMMSSNNN( pRecvMsg->RecordInfo.m_RegisterTime, buff );	
	query_string = query_string+buff+"'";	
	query_string += ",";

	query_string = query_string + _ultoa( pRecvMsg->Period, buff, 10 )+",";
	query_string = query_string + _ultoa( pRecvMsg->SettledStartDate, buff, 10 )+",";	

	query_string = query_string+_itoa(pRecvMsg->MemberInfo.m_UserGuid, buff, 10 )+",'";
	query_string = query_string+pRecvMsg->MemberInfo.m_tszCharName+"',";
	query_string = query_string+_itoa(pRecvMsg->MemberInfo.m_LV, buff, 10 )+",";
	query_string = query_string+_itoa(pRecvMsg->MemberInfo.m_Class, buff, 10 )+",'";
	query_string = query_string+pRecvMsg->MemberInfo.m_tszGuildName+"'";
	
	query_string += ")}";

	SUNLOG( eDEV_LOG, "[Handler_FromGameServer_IMPL(DG_MISSION_RANKING_POINT_REQ_SYN)] Query = %s", query_string.c_str() );

	
	//NAK는 필요 없을걸? 하는것도 없는데...
	//DISPMSG( query_string.c_str() );
	if( query_string.size() >= Query::MAX_QUERY_LENGTH ) 
	{
		SUNLOG(eFULL_LOG, "DG_MISSION_RANKING_POINT_REQ_SYN - DB LENGTH OVERFLOW" );
		return;
	}

	Query_Ranking_Select_Insert* pQuery = Query_Ranking_Select_Insert::ALLOC();
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	pQuery->SetMissionNo( pRecvMsg->RecordInfo.m_MissionNo );
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    pQuery->SetRankingType(pRecvMsg->RecordInfo.ranking_type);
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	pQuery->SetQuery(query_string.c_str());
	pQuery->SetIndex( MAKEDWORD( (WORD)DG_MISSION, (WORD)DG_MISSION_RANKING_POINT_REQ_DBR ) );
	pQuery->SetVoidObject( pServerSession );
    // NOTE: this query has high loads...
	pServerSession->DBQuery( pQuery );	
}

Handler_FromGameServer_IMPL(DG_MISSION_RANKING_POINT_REQ_DBR)
{	
	__TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult				= (MSG_DBPROXY_RESULT*)pMsg;
	Query_Ranking_Select_Insert* pQResult	= (Query_Ranking_Select_Insert*)pResult->m_pData;

	if( !pQResult->ResultSuccess() )
	{
		SUNLOG( eCRITICAL_LOG, "[DG_MISSION_RANKING_POINT_REQ_DBR][오류]:%s", pQResult->GetQuery() );
		Query_Ranking_Select_Insert::FREE(pQResult); pQResult = NULL;

		return;
	}

	MSG_DG_MISSION_RANKING_POINT_REQ_ACK msg;
	msg.m_dwKey				= pQResult->GetUserKey();
	msg.Point.Point			= pQResult->pResult[0].m_Point;
	msg.Point.RankingNum	= pQResult->GetMissionNo();
	msg.Point.UserGUID		= pQResult->pResult[0].m_UserGUID;
	msg.Point.DBKey			= pQResult->pParam[0].m_Key;
#ifdef _NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
    msg.Point.ranking_type  = pQResult->GetRankingType();
#endif //_NA_006801_20130814_RENEWAL_RANKINGSYSTEM_MINOR
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Ranking_Select_Insert::FREE(pQResult); pQResult = NULL;
}
#endif // _NA_008012_20150130_RANKING_SYSTEM

//{__NA001283_20090227_COLLECTION_LIST
Handler_FromGameServer_IMPL(DG_COLLECT_LIST_LOAD_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_COLLECT_LIST_LOAD_SYN * pRecvMsg = (MSG_DG_COLLECT_LIST_LOAD_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG( eMIDDLE_LOG, "[DG_COLLECT_LIST_LOAD_SYN]:[ID:%u]:DBUserNotFound", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG( eMIDDLE_LOG, "[DG_COLLECT_LIST_LOAD_SYN]:[ID:%u]:접속중 캐릭 없음", UserKey );
		return;
	}


	if( pUser->GetConnectedChar()->GetCollectedList()->IsLoaded() )
	{
		MSG_DG_COLLECT_LIST_LOAD_ACK msg;
		msg.m_dwKey = UserKey;
		pUser->GetConnectedChar()->GetCollectedList()->SerializeStream( msg.m_Info, SERIALIZE_LOAD );
		pServerSession->SendPacket( &msg, sizeof(msg) );
		return;
	}

	STRING_SAFER_QUERY64 szQueryBuff;
	szQueryBuff.MakeString( "S_Collection_Select %d", pUser->GetConnectedChar()->GetCharGuid() );

	Query_Collected_Select* pQuery = Query_Collected_Select::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_COLLECT, (WORD)DG_COLLECT_LIST_LOAD_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pUser->DBQuery(DG_COLLECT, DG_COLLECT_LIST_LOAD_DBR, pQuery);

}

Handler_FromGameServer_IMPL(DG_COLLECT_LIST_LOAD_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT* pResult		 = (MSG_DBPROXY_RESULT *)pMsg;
	Query_Collected_Select* pQResult = (Query_Collected_Select *)pResult->m_pData;
	int  ResultNum					 = pQResult->GetResultRowNum();
	DWORD UserKey					 = pQResult->GetUserKey();

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
	{
		SUNLOG( eFULL_LOG, "[DG_COLLECT_LIST_LOAD_DBR][UGuid:%u]Load후바로나감", UserKey );
		Query_Collected_Select::FREE(pQResult); pQResult = NULL;
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_COLLECT_LIST_LOAD_DBR]:[ID:%u]:접속중 캐릭터가 없습니다.", UserKey );
		Query_Collected_Select::FREE(pQResult); pQResult = NULL;
		return;
	}


	pUser->GetConnectedChar()->GetCollectedList()->Clear();

	for( int i = 0 ; i < ResultNum ; ++i )
	{
		pUser->GetConnectedChar()->GetCollectedList()->Add( StrToWzID(pQResult->pResult[i].m_CollectionID), pQResult->pResult[i].m_Count );
	}
	pUser->GetConnectedChar()->GetCollectedList()->SetIsLoad(true);

	MSG_DG_COLLECT_LIST_LOAD_ACK msg;
	msg.m_dwKey = UserKey;
	pUser->GetConnectedChar()->GetCollectedList()->SerializeStream( msg.m_Info, SERIALIZE_LOAD );
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Collected_Select::FREE(pQResult); pQResult = NULL;
}


Handler_FromGameServer_IMPL(DG_COLLECT_ADD_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_COLLECT_ADD_SYN * pRecvMsg = (MSG_DG_COLLECT_ADD_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_COLLECT_ADD_SYN]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_COLLECT_ADD_SYN]:[ID:%u]:접속중인 캐릭터가 없습니다.", UserKey );
		return;
	}

	if( false == pUser->GetConnectedChar()->GetCollectedList()->IsLoaded() )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_COLLECT_ADD_SYN]:[ID:%u]아직 채집물리스트를 로드 하지 않았다.", UserKey );
		return;
	}

	TCHAR Wzid[5] = {0,};
	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_Collection_Insert(%d,'%s')}", pUser->GetConnectedChar()->GetCharGuid(), WzIDToString(Wzid,pRecvMsg->m_CollectionID) );

	Query_Collected_Insert* pQuery = Query_Collected_Insert::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_COLLECT, (WORD)DG_COLLECT_ADD_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pQuery->SetCollectionID(pRecvMsg->m_CollectionID);

	pUser->DBQuery(DG_COLLECT, DG_COLLECT_ADD_DBR, pQuery);
}

Handler_FromGameServer_IMPL(DG_COLLECT_ADD_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT * pResult		= (MSG_DBPROXY_RESULT *)pMsg;
	Query_Collected_Insert * pQResult	= (Query_Collected_Insert *)pResult->m_pData;
	BYTE  ResultNum						= (BYTE)pQResult->GetResultRowNum();

    const DWORD UserKey		    = pQResult->GetUserKey();
    const WzID CollectionID     = pQResult->GetCollectionID();
    const BOOL bResultSuccess   = pQResult->ResultSuccess();

    // (JAYKANG) (090826) (BUG-FIX)
    if( !bResultSuccess )
    {
        MSG_DG_COLLECT_NAK msg;
        msg.m_dwKey		  = UserKey;
        msg.m_dwErrorCode = RC::RC_COLLECT_FAIL_DB_ADD;
        pServerSession->SendPacket( &msg, sizeof(msg) );

        SUNLOG( eCRITICAL_LOG, "[DG_COLLECT_ADD_DBR][오류]%s, Result[%d]", pQResult->GetQuery(), pQResult->GetResult() );
        Query_Collected_Insert::FREE(pQResult); pQResult = NULL;
        return;
    }
    Query_Collected_Insert::FREE(pQResult); pQResult = NULL;

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
    {
        SUNLOG( eFULL_LOG, "[DG_COLLECT_ADD_DBR][UGuid:%u]Add후바로나감", UserKey );
        return;
    }

    DBCharacter* pDBCharacter = pUser->GetConnectedChar();
	if( !pDBCharacter )
    {
        SUNLOG(eFULL_LOG, "[DG_COLLECT_ADD_DBR]:[ID:%u]:접속중 캐릭터 없음.", UserKey );
        return;
    }

    CollectedList* pCollectedList = pDBCharacter->GetCollectedList();
    if( !pCollectedList )
    {
        SUNLOG( eCRITICAL_LOG, "[DG_COLLECT_ADD_DBR][오류] NOT CollectedList" );
        return;
    }
 
    //.//

	pCollectedList->Add( CollectionID );

	MSG_DG_COLLECT_ADD_ACK msg;
	msg.m_dwKey = UserKey;
	msg.m_CollectionID = CollectionID;
    msg.m_wCount = pCollectedList->GetCount(CollectionID);
	pServerSession->SendPacket( &msg, sizeof(msg) );
}


Handler_FromGameServer_IMPL(DG_COLLECT_DEL_SYN)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DG_COLLECT_DEL_SYN * pRecvMsg = (MSG_DG_COLLECT_DEL_SYN*)pMsg;
	DWORD UserKey = pRecvMsg->m_dwKey;
	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_COLLECT_DEL_SYN]:[ID:%u]:ID에 맞는 DBUser가 없습니다.", UserKey );
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_COLLECT_DEL_SYN]:[ID:%u]:접속중인 캐릭터가 없습니다.", UserKey );
		return;
	}

	if( false == pUser->GetConnectedChar()->GetCollectedList()->IsLoaded() )
	{
		SUNLOG(eFULL_LOG, "[MSG_DG_COLLECT_DEL_SYN]:[ID:%u]아직 채집물리스트를 로드 하지 않았다.", UserKey );
		return;
	}

	TCHAR Wzid[5] = {0,};
	STRING_SAFER_QUERY128 szQueryBuff;
	szQueryBuff.MakeString( "{?=call S_Collection_Delete(%d,'%s')}", pUser->GetConnectedChar()->GetCharGuid(), WzIDToString(Wzid,pRecvMsg->m_CollectionID) );

	Query_Collected_Delete* pQuery = Query_Collected_Delete::ALLOC();
	pQuery->SetQuery(szQueryBuff);
	pQuery->SetUserKey( pRecvMsg->m_dwKey );
	//pQuery->SetIndex( MAKEDWORD( (WORD)DG_COLLECT, (WORD)DG_COLLECT_DEL_DBR ) );
	pQuery->SetVoidObject( pServerSession );
	pQuery->SetCollectionID(pRecvMsg->m_CollectionID);

	pUser->DBQuery(DG_COLLECT, DG_COLLECT_DEL_DBR, pQuery);
}

Handler_FromGameServer_IMPL(DG_COLLECT_DEL_DBR)
{
    __TOUCHED((pServerSession, wSize));
	MSG_DBPROXY_RESULT*		pResult		= (MSG_DBPROXY_RESULT*)pMsg;
	Query_Collected_Delete* pQResult	= (Query_Collected_Delete*)pResult->m_pData;
	BYTE  ResultNum						= (BYTE)pQResult->GetResultRowNum();
	DWORD UserKey						= pQResult->GetUserKey();

	DBUser * pUser = g_DBUserManager.FindUser( UserKey );
	if( !pUser ) 
	{
		SUNLOG( eFULL_LOG, "[DG_COLLECT_DEL_DBR][UGuid:%u]Add후바로나감", UserKey );
		Query_Collected_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}
	if( !pUser->GetConnectedChar() )
	{
		SUNLOG(eFULL_LOG, "[DG_COLLECT_DEL_DBR]:[ID:%u]:접속중 캐릭터 없음.", UserKey );
		Query_Collected_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}

	if( !pQResult->ResultSuccess() )
	{
		MSG_DG_COLLECT_NAK msg;
		msg.m_dwKey		  = pQResult->GetUserKey();
		msg.m_dwErrorCode = RC::RC_COLLECT_FAIL_DB_DEL;
		pServerSession->SendPacket( &msg, sizeof(msg) );

		SUNLOG( eCRITICAL_LOG, "[RC_COLLECT_FAIL_DB_DEL][오류]%s, Result[%d]", pQResult->GetQuery(), pQResult->GetResult() );
		Query_Collected_Delete::FREE(pQResult); pQResult = NULL;
		return;
	}

	//
	pUser->GetConnectedChar()->GetCollectedList()->Delete( pQResult->GetCollectionID() );

	MSG_DG_COLLECT_DEL_ACK msg;
	msg.m_dwKey = pQResult->GetUserKey();
	msg.m_CollectionID = pQResult->GetCollectionID();
	pServerSession->SendPacket( &msg, sizeof(msg) );

	Query_Collected_Delete::FREE(pQResult); pQResult = NULL;
}

//}__NA001283_20090227_COLLECTION_LIST

#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
Handler_FromGameServer_IMPL(DG_CHARINFO_SELECT_ACHIEVEMENT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* db_result	= (MSG_DBPROXY_RESULT *)pMsg;
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(db_result->m_pData);
    //Query_Char_Achievement_Select* pQResult = (Query_Char_Achievement_Select *)db_result->m_pData;

    const USERGUID user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();

    RC::eCHAR_RESULT error_code = RC::RC_CHAR_SUCCESS;

    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };

    if(db_query_proxy_manager->DBResult_Char_Transaction(\
        *query_result, db_user, db_character) == false)
    {
        assert(error_code == RC::RC_CHAR_SUCCESS);
    }
    // success
    assert(error_code == RC::RC_CHAR_SUCCESS);

RELEASE_RESOURCE:
    Query_Char_Achievement_Select::FREE(static_cast<Query_Char_Achievement_Select*>(query_result));
}

Handler_FromGameServer_IMPL(DG_CHARINFO_UPDATE_ACHIEVEMENT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* dbproxy_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        reinterpret_cast<QueryForCharTransaction*>(dbproxy_result->m_pData);
    //
    const USERGUID user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();
    //
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
               __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };

    if (db_query_proxy_manager->DBResult_Char_Transaction(\
        *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }
    // success

RELEASE_RESOURCE:
    Query_Char_Achievement_Update::FREE(static_cast<Query_Char_Achievement_Update*>(query_result));
}

#endif
Handler_FromGameServer_IMPL(DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD)
{
    CompositionOrDecompositionHandler handler;
    handler.HandleInsertHistory(pServerSession, pMsg, wSize);
}
Handler_FromGameServer_IMPL(DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_DBR)
{
    CompositionOrDecompositionHandler handler;
    handler.HandleInsertHistoryDBR(pServerSession, pMsg, wSize);
}
Handler_FromGameServer_IMPL(DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN)
{
    CompositionOrDecompositionHandler handler;
    handler.HandleSelectHistories(pServerSession, pMsg, wSize);
}
Handler_FromGameServer_IMPL(DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR)
{
    CompositionOrDecompositionHandler handler;
    handler.HandleSelectHistoriesDBR(pServerSession, pMsg, wSize);
}
Handler_FromGameServer_IMPL(DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD)
{
    CompositionOrDecompositionHandler handler;
    handler.HandleUpdateHistories(pServerSession, pMsg, wSize);
}
Handler_FromGameServer_IMPL(DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR)
{
    CompositionOrDecompositionHandler handler;
    handler.HandleUpdateHistoriesDBR(pServerSession, pMsg, wSize);
}

Handler_FromGameServer_IMPL(DG_OTHER_EXTEND_CHAR_SLOT_SYN)
{
    const MSG_DG_OTHER_EXTEND_CHAR_SLOT_SYN* const request = 
        static_cast<const MSG_DG_OTHER_EXTEND_CHAR_SLOT_SYN* const>(pMsg);
    if (request->GetSize() != wSize) 
    {
        return;
    }

    // 요청자를 검사한다.
    const DWORD user_key = pMsg->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (requester == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
        if (requester->GetConnectedChar() == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|접속중인 캐릭터가 존재하지 않습니다.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
    }

    // 쿼리를 실행하기전에 사전 검사한다.
    if (requester->GetCharSlotSize() == MaxCharListConfig::kMaxSlotSize)
    {
        MSG_DG_OTHER_EXTEND_CHAR_SLOT_ACK response;
        {
            response.Init();
            response.result_code = MSG_DG_OTHER_EXTEND_CHAR_SLOT_ACK::kSlotIsFull;
            response.extend_item_position = request->extend_item_position;
        }
        requester->SendPacketToGameServer(&response, response.GetSize());
        return;
    }

    // 쿼리 문자열을 만든다.
    STRING_SAFER_QUERY128 query_string;
    query_string.MakeString(
        _T("{?=call S_ExtendCharSlot(%u, %u, %u, %u)}"), 
        requester->GetUserGuid(), 
#ifdef _USE_MAX_CHAR_LIST_PARSER
        MaxCharListParser::Instance()->GetDefaultSlotSize(), 
        MaxCharListParser::Instance()->GetMaxSlotSize(), 
        MaxCharListParser::Instance()->GetSlotSizePerExtension()
#else
        MaxCharListConfig::kDefaultSlotSize, 
        MaxCharListConfig::kMaxSlotSize, 
        MaxCharListConfig::kSlotSizePerExtension
#endif
    );
   
    // 쿼리를 실행한다.
    ExtendCharSlotQuery* const query = ExtendCharSlotQuery::ALLOC();
    {
        query->SetUserKey(user_key);
        query->SetQuery(query_string);
        query->SetVoidObject(pServerSession);
        query->SetExtendItemPosition(request->extend_item_position);
    }
    requester->DBQuery(DG_OTHER, DG_OTHER_EXTEND_CHAR_SLOT_DBR, query);
}

Handler_FromGameServer_IMPL(DG_OTHER_EXTEND_CHAR_SLOT_DBR)
{
    __UNUSED(pServerSession);
    __UNUSED(wSize);

    const MSG_DBPROXY_RESULT* const db_result = static_cast<const MSG_DBPROXY_RESULT* const>(pMsg);
    ExtendCharSlotQuery* query = static_cast<ExtendCharSlotQuery*>(db_result->m_pData);

    // 쿼리 해제를 보장한다.
    struct QueryAutoReleaser
    {
        QueryAutoReleaser(ExtendCharSlotQuery* query) : query_(query)
        {
        }
        ~QueryAutoReleaser()
        {
            ExtendCharSlotQuery::FREE(query_);
        }
        ExtendCharSlotQuery* query_;
    } query_auto_releaser(query);

    // 요청자를 검사한다.
    const DWORD user_key = query->GetUserKey();
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (requester == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
        if (requester->GetConnectedChar() == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|접속중인 캐릭터가 존재하지 않습니다.|UserKey = %u|"), 
                   __FUNCTION__, 
                   user_key);
            return;
        }
    }

    // 결과를 검사한다.
    const MSG_DG_OTHER_EXTEND_CHAR_SLOT_ACK::ResultCode result_code = query->GetResult();
    if (result_code != MSG_DG_OTHER_EXTEND_CHAR_SLOT_ACK::kSuccess)
    {
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|쿼리 실행이 실패 했습니다.|UserKey = %u, Result = %u, Query = %s|"), 
               __FUNCTION__, 
               user_key, 
               result_code, 
               query->GetQuery());
    }

    // 쿼리 실행에 성공했으므로 새로운 슬롯 크기를 설정한다.
    // 경계값은 이전에 이미 검사된다.
    const BYTE new_slot_size = 
        requester->GetCharSlotSize() + MaxCharListConfig::kSlotSizePerExtension;
    requester->SetCharSlotSize(new_slot_size);

    // 응답한다.
    MSG_DG_OTHER_EXTEND_CHAR_SLOT_ACK response;
    {
        response.Init();
        response.result_code = result_code;
        response.extend_item_position = query->GetExtendItemPosition();
    }
    requester->SendPacketToGameServer(&response, response.GetSize());
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
Handler_FromGameServer_IMPL(DG_CHARINFO_HONORTITLES_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_HonorTitle_Select* query_result = static_cast<Query_HonorTitle_Select*>(result->m_pData);

    if( query_result->IsError() || query_result->parameters.result_ != 0)
    {
        //something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
        Query_HonorTitle_Select::FREE(query_result);

        return;
    }

    int result_count = query_result->GetResultRowNum();
    if (FlowControl::FCAssert(result_count <= MAX_HONORTITLE_COUNT) == false) {
        result_count = MAX_HONORTITLE_COUNT;
    };

    MSG_DG_HONORTITLES_SELECT_ACK msg;
    msg.m_dwKey = query_result->GetUserKey();
    msg.titles.count_ = result_count;
    BOOST_STATIC_ASSERT(sizeof(msg.titles.title_) == sizeof(query_result->result_records_) &&
                        _countof(msg.titles.title_) == _countof(query_result->result_records_));
    CopyMemory(&msg.titles.title_, query_result->result_records_,
               sizeof(msg.titles.title_[0]) * result_count);
    pServerSession->SendPacket(&msg, msg.GetSize());

    Query_HonorTitle_Select::FREE(query_result);
}

// GameServer -> DBProxyServer
Handler_FromGameServer_IMPL(DG_CHARINFO_HONORSYSTEM)
{
    const MSG_DG_CHARINFO_HONORSYSTEM* const request = 
        static_cast<const MSG_DG_CHARINFO_HONORSYSTEM* const>(pMsg);

    switch(request->packet_type)
    {
    case request->DG_HONORSYSTEM_TITLES:
        OnDG_HONORSYSTEM_TITLES(pServerSession, pMsg, wSize);
        break;
    case request->DG_HONORSYSTEM_FAME_REPUTE:
        OnDG_HONORSYSTEM_FAME_REPUTE(pServerSession, pMsg, wSize);
        break;
    case request->DG_HONORSYSTEM_DAILYQUEST:
        OnDG_HONORSYSTEM_DAILYQUEST(pServerSession, pMsg, wSize);
        break;
    default:
        ASSERT(!"received an unexpected message");
        break;
    }
}

Handler_FromGameServer_IMPL(DG_HONORSYSTEM_TITLES)
{
    const MSG_DG_HONORSYSTEM_TITLES* const request = 
        static_cast<const MSG_DG_HONORSYSTEM_TITLES* const>(pMsg);

    // 요청자를 검사한다.
    const DWORD user_key = pMsg->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (requester == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }
        if (requester->GetConnectedChar() == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|접속중인 캐릭터가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }
    }

    switch(request->sub_type)
    {
    case DG_HONORSYSTEM_TITLES_SELECT_SYN:
        {
            const MSG_DG_HONORTITLES_SELECT_SYN* cmd_packet = \
                static_cast<const MSG_DG_HONORTITLES_SELECT_SYN* const>(request);

            STRING_SAFER_QUERY64 query_string;
            query_string.MakeString(_T("{? = call S_HonorTitles_Select (?)}"));

            Query_HonorTitle_Select* query = Query_HonorTitle_Select::ALLOC();
            query->parameters.char_guid_ = requester->GetConnectedChar()->GetCharGuid();
            query->SetUserKey(user_key);
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);

            requester->DBQuery(DG_CHARINFO, DG_CHARINFO_HONORTITLES_SELECT_DBR, query);
        } break;
    case DG_HONORSYSTEM_TITLES_ADD_SYN:
        {
            const MSG_DG_HONORTITLES_ADD_REMOVE_CMD* cmd_packet = \
                static_cast<const MSG_DG_HONORTITLES_ADD_REMOVE_CMD* const>(request);

            STRING_SAFER_QUERY128 query_string;
            query_string.MakeString(_T("{?=call S_HonorTitles_Add(?, ?)}"));

            Query_HonorTitle_AddOrRemove* const query = Query_HonorTitle_AddOrRemove::ALLOC();
            query->sub_type = DG_HONORSYSTEM_TITLES_ADD_SYN;
            query->parameters.char_guid = requester->GetConnectedChar()->GetCharGuid();
            query->parameters.title_index = cmd_packet->honor_title_index;
            query->SetUserKey(user_key);
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);

            requester->DBQuery(DG_CHARINFO, DG_CHARINFO_HONORTITLES_ADD_REMOVE_DBR, query);
        } break;
    case DG_HONORSYSTEM_TITLES_REMOVE_SYN:
        {
            const MSG_DG_HONORTITLES_ADD_REMOVE_CMD* cmd_packet = \
                static_cast<const MSG_DG_HONORTITLES_ADD_REMOVE_CMD* const>(request);

            // 쿼리 문자열을 만든다.
            STRING_SAFER_QUERY128 query_string;
            query_string.MakeString(_T("{?=call S_HonorTitles_Remove(?, ?)}"));

            // 쿼리를 실행한다.
            Query_HonorTitle_AddOrRemove* const query = Query_HonorTitle_AddOrRemove::ALLOC();
            query->sub_type = DG_HONORSYSTEM_TITLES_REMOVE_SYN;
            query->parameters.char_guid = requester->GetConnectedChar()->GetCharGuid();
            query->parameters.title_index = cmd_packet->honor_title_index;
            query->SetUserKey(user_key);
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);

            requester->DBQuery(DG_CHARINFO, DG_CHARINFO_HONORTITLES_ADD_REMOVE_DBR, query);
        } break;
    default:
        return;
    }    
}

Handler_FromGameServer_IMPL(DG_CHARINFO_HONORTITLES_ADD_REMOVE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_HonorTitle_AddOrRemove* query_result = static_cast<Query_HonorTitle_AddOrRemove*>(result->m_pData);

    if( query_result->IsError() /*|| query_result->parameters.result != 0*/)
    {
        //something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
        Query_HonorTitle_AddOrRemove::FREE(query_result);

        return;
    }

    MSG_DG_HONORTITLES_ADD_REMOVE_ACK msg;
    msg.m_dwKey = query_result->GetUserKey();
    msg.sub_type = query_result->sub_type;
    msg.result = query_result->parameters.result;
    msg.honor_title_index = query_result->parameters.title_index;
    pServerSession->SendPacket( &msg, sizeof(msg) );

    Query_HonorTitle_AddOrRemove::FREE(query_result); query_result = NULL;
}

Handler_FromGameServer_IMPL(DG_HONORSYSTEM_FAME_REPUTE)
{
    const MSG_DG_HONORSYSTEM_FAME_REPUTE* const request = 
        static_cast<const MSG_DG_HONORSYSTEM_FAME_REPUTE* const>(pMsg);

    // 요청자를 검사한다.
    const DWORD user_key = pMsg->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (requester == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }
        if (requester->GetConnectedChar() == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|접속중인 캐릭터가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }
    }

    switch(request->sub_type)
    {
        case DG_HONORSYSTEM_FAME_REPUTE_SELECT_SYN:
            {
                const MSG_DG_FAME_REPUTE_SELECT_SYN* cmd_packet = \
                    static_cast<const MSG_DG_FAME_REPUTE_SELECT_SYN* const>(request);

                STRING_SAFER_QUERY64 query_string;
                query_string.MakeString(_T("{? = call S_HonorFameRepute_Select (?)}"));

                Query_FameRepute_Select* query = Query_FameRepute_Select::ALLOC();
                query->parameters.char_guid_ = requester->GetConnectedChar()->GetCharGuid();
                query->SetUserKey(user_key);
                query->SetQuery(query_string);
                query->SetVoidObject(pServerSession);

                requester->DBQuery(DG_CHARINFO, DG_CHARINFO_FAMEREPUTE_SELECT_DBR, query);
            } break;
        case DG_HONORSYSTEM_FAME_REPUTE_CHANGE_CMD:
            {
                const MSG_DG_HONORSYSTEM_FAME_REPUTE_CHANGE_CMD* cmd = \
                    static_cast<const MSG_DG_HONORSYSTEM_FAME_REPUTE_CHANGE_CMD* const>(request);

                STRING_SAFER_QUERY64 query_string;
                query_string.MakeString(_T("{? = call S_HonorFameRepute_Set (?, ?, ?, ?)}"));

                Query_FameRepute_Set* query = Query_FameRepute_Set::ALLOC();
                query->parameters.char_guid_ = requester->GetConnectedChar()->GetCharGuid();
                query->parameters.type_ = cmd->type_;
                query->parameters.code_ = cmd->code_;
                query->parameters.point_ = cmd->point_;
                query->SetUserKey(user_key);
                query->SetQuery(query_string);
                query->SetVoidObject(pServerSession);

                requester->DBQuery(DG_CHARINFO, DG_CHARINFO_FAMEREPUTE_CHANGE_DBR, query);
            } break;
        default:
            return;
    }    
}

Handler_FromGameServer_IMPL(DG_CHARINFO_FAMEREPUTE_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_FameRepute_Select* query_result = static_cast<Query_FameRepute_Select*>(result->m_pData);

    if( query_result->IsError() || query_result->parameters.result_ != 0)
    {
        //something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
        Query_FameRepute_Select::FREE(query_result);

        return;
    }

    int result_count = query_result->GetResultRowNum();

    MSG_DG_FAME_REPUTE_SELECT_ACK msg;
    msg.m_dwKey = query_result->GetUserKey();
    msg.honor_point_table.count_ = result_count;
    memcpy(&msg.honor_point_table.point_table_, query_result->result_records_, sizeof(HONOR_POINT_INFO)*result_count);
    pServerSession->SendPacket( &msg, sizeof(msg) );

    Query_FameRepute_Select::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_CHARINFO_FAMEREPUTE_CHANGE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_FameRepute_Set* query_result = static_cast<Query_FameRepute_Set*>(result->m_pData);

    if( query_result->IsError() || query_result->parameters.result_ != 0)
    {
        //something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
        Query_FameRepute_Set::FREE(query_result);

        return;
    }

    // nothing to return...
    
    Query_FameRepute_Set::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_HONORSYSTEM_DAILYQUEST)
{
    const MSG_DG_HONORSYSTEM_DAILYQUEST* const request = \
        static_cast<const MSG_DG_HONORSYSTEM_DAILYQUEST* const>(pMsg);

    // 요청자를 검사한다.
    const DWORD user_key = pMsg->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (requester == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }
        if (requester->GetConnectedChar() == NULL)
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|접속중인 캐릭터가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }
    }

    switch(request->sub_type)
    {
        case DG_HONORSYSTEM_DAILYQUEST_SELECT_SYN:
            {
                const MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_SYN* cmd_packet = \
                    static_cast<const MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_SYN* const>(request);

                STRING_SAFER_QUERY64 query_string;
                query_string.MakeString(_T("{? = call S_DailyQuest_Select (?)}"));

                Query_DailyQuest_Select* query = Query_DailyQuest_Select::ALLOC();
                query->parameters.char_guid_ = requester->GetConnectedChar()->GetCharGuid();
                query->SetUserKey(user_key);
                query->SetQuery(query_string);
                query->SetVoidObject(pServerSession);

                requester->DBQuery(DG_CHARINFO, DG_CHARINFO_DAILYQUEST_SELECT_DBR, query);
            } break;
        case DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN:
            {
                const MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN* cmd_packet = \
                    static_cast<const MSG_DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN* const>(request);

                STRING_SAFER_QUERY64 query_string;
                query_string.MakeString(_T("{? = call S_DailyQuest_Update (?,?,?,?,?)}"));

                Query_DailyQuest_Update* query = Query_DailyQuest_Update::ALLOC();
                query->parameters.char_guid_ = requester->GetConnectedChar()->GetCharGuid();
                query->SetUserKey(user_key);
                query->SetQuery(query_string);
                query->SetVoidObject(pServerSession);
                query->parameters.code_ = cmd_packet->quest_data.code_;
                query->parameters.type_ = cmd_packet->quest_data.status_;

                util::GetSQLDateTime_YYYYMMDDHHMMSSNNN( \
                    cmd_packet->quest_data.start_datetime_*1000, query->parameters.startdatetime);
                util::GetSQLDateTime_YYYYMMDDHHMMSSNNN( \
                    cmd_packet->quest_data.end_datetime_*1000, query->parameters.enddatetime);

                requester->DBQuery(DG_CHARINFO, DG_CHARINFO_DAILYQUEST_UPDATE_DBR, query);
            } break;
        default:
            return;
    }  
}

Handler_FromGameServer_IMPL(DG_CHARINFO_DAILYQUEST_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_DailyQuest_Select* query_result = static_cast<Query_DailyQuest_Select*>(result->m_pData);

    if( query_result->IsError() || query_result->parameters.result_ != 0)
    {
        //something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
        Query_DailyQuest_Select::FREE(query_result);

        return;
    }

    MSG_DG_HONORSYSTEM_DAILYQUEST_SELECT_ACK msg;
    msg.m_dwKey = query_result->GetUserKey();
    int result_count = query_result->GetResultRowNum();
    if (FlowControl::FCAssert(result_count <= _countof(msg.daily_quest_info_)) == false) {
        result_count = _countof(msg.daily_quest_info_); // check query object array size
    };
    msg.count_ = static_cast<uint16_t>(result_count);
    for(int i = 0; i < result_count; ++i)
    {
        if(i == MAX_DAILYQUEST_INFO_COUNT)
            break;

        msg.daily_quest_info_[i].code_      = query_result->result_records_[i].code;
        msg.daily_quest_info_[i].status_    = query_result->result_records_[i].status;

        tm s_tm, e_tm;
        sscanf(query_result->result_records_[i].startdatetime, "%4d-%2d-%2d %2d:%2d:%2d",
            &s_tm.tm_year, &s_tm.tm_mon, &s_tm.tm_mday, &s_tm.tm_hour, &s_tm.tm_min, &s_tm.tm_sec);
        sscanf(query_result->result_records_[i].enddatetime, "%4d-%2d-%2d %2d:%2d:%2d",
            &e_tm.tm_year, &e_tm.tm_mon, &e_tm.tm_mday, &e_tm.tm_hour, &e_tm.tm_min, &e_tm.tm_sec);

        util::GetDateTime_YYYYMMDDHHMMSS(s_tm.tm_year, s_tm.tm_mon, s_tm.tm_mday, s_tm.tm_hour, s_tm.tm_min, s_tm.tm_sec, 
            msg.daily_quest_info_[i].start_datetime_);
        util::GetDateTime_YYYYMMDDHHMMSS(e_tm.tm_year, e_tm.tm_mon, e_tm.tm_mday, e_tm.tm_hour, e_tm.tm_min, e_tm.tm_sec,
            msg.daily_quest_info_[i].end_datetime_);
    }
    pServerSession->SendPacket(&msg, msg.GetSize());

    Query_DailyQuest_Select::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_CHARINFO_DAILYQUEST_UPDATE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_DailyQuest_Update* query_result = static_cast<Query_DailyQuest_Update*>(result->m_pData);

    if( query_result->IsError() || query_result->parameters.result_ != 0)
    {
        //something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
        Query_DailyQuest_Update::FREE(query_result);

        return;
    }

    // nothing to return...

    Query_DailyQuest_Update::FREE(query_result);
}
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
//gameserver -> dbproxyserver
Handler_FromGameServer_IMPL(DG_CHARINFO_POINTWALLET_SYSTEM)
{
    const MSG_DG_CHARINFO_POINTWALLET_SYSTEM* const request = 
        static_cast<const MSG_DG_CHARINFO_POINTWALLET_SYSTEM* const>(pMsg);

    // 요청자를 검사한다.
    const DWORD user_key = request->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (requester == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }

        if (requester->GetConnectedChar() == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|접속중인 캐릭터가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }
    }

    switch(request->packet_type)
    {
        case DG_POINTWALLET_SELECT_SYN: 
        {
            const MSG_DG_POINTWALLET_SELECT_SYN* packet = \
                static_cast<MSG_DG_POINTWALLET_SELECT_SYN*>(pMsg);

            STRING_SAFER_QUERY64 query_string;
            query_string.MakeString(_T("{? = call S_PointWallet_Select (?)}"));

            Query_PointWallet_Select* query = Query_PointWallet_Select::ALLOC();
            query->SetUserKey(user_key);
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);

            query->parameters.char_guid_ = requester->GetConnectedChar()->GetCharGuid();

            requester->DBQuery(DG_CHARINFO, DG_CHARINFO_POINTWALLET_SELECT_DBR, query);
        } break;

        case DG_POINTWALLET_CHANGE_SYN: 
        {
            const MSG_DG_POINTWALLET_CHANGE_SYN* packet = \
                static_cast<MSG_DG_POINTWALLET_CHANGE_SYN*>(pMsg);

            STRING_SAFER_QUERY64 query_string;
            query_string.MakeString(_T("{? = call S_PointWallet_Update (?, ?, ?)}"));

            Query_PointWallet_Update* query = Query_PointWallet_Update::ALLOC();
            query->SetUserKey(user_key);
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);

            query->parameters.char_guid_ = requester->GetConnectedChar()->GetCharGuid();
            query->parameters.code_  = packet->code_;
            query->parameters.point_ = packet->point_;

            requester->DBQuery(DG_CHARINFO, DG_CHARINFO_POINTWALLET_UPDATE_DBR, query);
        } break;

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
        case DG_POINTWALLET_GUILDPOINT_DONATION_SYN:
        {
            const MSG_DG_GUILDPOINT_DONATION_SYN* packet = \
                static_cast<MSG_DG_GUILDPOINT_DONATION_SYN*>(pMsg);

            STRING_SAFER_QUERY128 query_string;
#ifdef _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA 
            query_string.MakeString(_T("{? = call S_GuildPoint_Donation (%d, %d, %d, %d, %d, ?)}"),
#else
            query_string.MakeString(_T("{? = call S_GuildPoint_Donation (%d, %d, %d, %d, ?)}"),
#endif // _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA 
                requester->GetConnectedChar()->GetCharGuid(),
                packet->guild_guid_,
                packet->guild_point_code_,
                packet->donation_item_count_
#ifdef _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA 
                ,packet->max_guild_coin_donation_);
#else
                );
#endif // _NA_006032_20120823_GUILD_COIN_COUNT_OF_DAY_APPLY_SCRIPT_DATA 

            Query_GuildPoint_Donation* query = Query_GuildPoint_Donation::ALLOC();
            query->SetUserKey(user_key);
            query->SetQuery(query_string);
            query->SetVoidObject(pServerSession);
            query->donation_item_count_ = packet->donation_item_count_;

            requester->DBQuery(DG_CHARINFO, DG_CHARINFO_POINTWALLET_GUILDCOIN_DONATION_DBR, query);
        } break;
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
        default: {
            ASSERT(!"received an unexpected message");
        } break;
    }
}

Handler_FromGameServer_IMPL(DG_CHARINFO_POINTWALLET_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_PointWallet_Select* query_result = static_cast<Query_PointWallet_Select*>(result->m_pData);

    if( query_result->IsError() || query_result->parameters.result_ != 0)
    {
        // something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
    }
    else
    {
        // success...

        MSG_DG_POINTWALLET_SELECT_ACK msg;
        msg.m_dwKey = query_result->GetUserKey();
        int result_count = query_result->GetResultRowNum();
        if (FlowControl::FCAssert(result_count <= _countof(msg.pointwallet_data)) == false) {
            result_count = _countof(msg.pointwallet_data); // check query object array size
        };
        msg.count_ = static_cast<uint16_t>(result_count);
        for(int i = 0; i < result_count; ++i)
        {
            if(i == MAX_POINTWALLET_COUNT)
                break;

            msg.pointwallet_data[i].code_   = query_result->result_records_[i].code;
            msg.pointwallet_data[i].point_  = query_result->result_records_[i].point;
        }
        pServerSession->SendPacket(&msg, msg.GetSize());
    }

    Query_PointWallet_Select::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_CHARINFO_POINTWALLET_UPDATE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_PointWallet_Update* query_result = static_cast<Query_PointWallet_Update*>(result->m_pData);

    if( query_result->IsError() || query_result->parameters.result_ != 0)
    {
        // something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
    }
    else
    {
        // success...

        // nothing to return...
    }

    Query_PointWallet_Update::FREE(query_result);
}

#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
Handler_FromGameServer_IMPL(DG_CHARINFO_POINTWALLET_GUILDCOIN_DONATION_DBR)
{
    __TOUCHED((pServerSession, wSize));
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_GuildPoint_Donation* query_result = static_cast<Query_GuildPoint_Donation*>(result->m_pData);

    if (query_result->IsError())
    {
        // something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
    }
    else
    {
        MSG_DG_GUILDPOINT_DONATION_ACK msg;
        msg.m_dwKey = query_result->GetUserKey();
        msg.donation_count_of_day_ = query_result->parameters.donation_count_of_day_;
        msg.donation_item_count_ = query_result->donation_item_count_;
        pServerSession->SendPacket(&msg, sizeof(msg));
    }

    Query_GuildPoint_Donation::FREE(query_result);
}
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM 

Handler_FromGameServer_IMPL(DG_CHARNAME_CHANGE_CMD)
{
    const MSG_DG_CHARNAME_CHANGE_CMD* const request = 
        static_cast<const MSG_DG_CHARNAME_CHANGE_CMD* const>(pMsg);

    // 요청자를 검사한다.
    const DWORD user_key = request->m_dwKey;
    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    {
        if (requester == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|요청자가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }

        if (requester->GetConnectedChar() == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, 
                _T("|%s|접속중인 캐릭터가 존재하지 않습니다.|UserKey = %u|"), 
                __FUNCTION__, 
                user_key);
            return;
        }
    }

    STRING_SAFER_QUERY64 query_string;
    query_string.MakeString(_T("{? = call S_CharacterNameChange_Request (?)}"));

    Query_Character_NameChange_Request* query = Query_Character_NameChange_Request::ALLOC();
    query->SetUserKey(user_key);
    query->SetQuery(query_string);
    query->SetVoidObject(pServerSession);

    query->parameters.char_guid_ = requester->GetConnectedChar()->GetCharGuid();
    query->slot_index = request->slot_index;
    query->item_pos   = request->item_pos;

    requester->DBQuery(DG_CHARINFO, DG_CHARNAME_CHANGE_DBR, query);
}

Handler_FromGameServer_IMPL(DG_CHARNAME_CHANGE_DBR)
{
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    Query_Character_NameChange_Request* query_result = \
        static_cast<Query_Character_NameChange_Request*>(result->m_pData);

    if (query_result->IsError())
    {
        // something error...
        SUNLOG(eCRITICAL_LOG, _T("[%s][%s][UserKey = %u]"), 
            __FUNCTION__, query_result->GetErrorMsg(), query_result->GetUserKey());
    }
    else
    {
        MSG_DG_CHARNAME_CHANGE_ACK msg;
        msg.m_dwKey = query_result->GetUserKey();
        msg.char_guid = query_result->parameters.char_guid_;
        msg.slot_index = query_result->slot_index;
        msg.item_pos = query_result->item_pos;
        msg.result = query_result->parameters.result_;
        pServerSession->SendPacket(&msg, sizeof(msg));
    }

    Query_Character_NameChange_Request::FREE(query_result);
}

//!~ _NA_006372_20130108_BADGE_ITEM
Handler_FromGameServer_IMPL(DG_CHARACTER_EXTRA_INFO_CMD)
{
    const MSG_DG_CHARACTER_EXTRA_INFO_CMD* const recv_msg = \
        static_cast<MSG_DG_CHARACTER_EXTRA_INFO_CMD*>(pMsg);
    const DWORD user_key = recv_msg->m_dwKey;

    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    if (requester == NULL) {
        return;
    }

    DBCharacter* character = requester->GetConnectedChar();
    if (character == NULL) {
        return;
    }

#ifdef _NA_006372_20130108_BADGE_ITEM
    memcpy(&character->GetToggleItemInfo(), &recv_msg->toggle_item_info_, \
        sizeof(recv_msg->toggle_item_info_));
#endif //_NA_006372_20130108_BADGE_ITEM
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    memcpy(&character->GetBattleGroundRecordInfos(), &recv_msg->battle_ground_record_infos_, \
        sizeof(recv_msg->battle_ground_record_infos_));
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    memcpy(&character->GetGoldRushRecordInfos(), &recv_msg->goldrush_record_infos_, \
        sizeof(recv_msg->goldrush_record_infos_));
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
    character->SetDailyQuestResetTime(recv_msg->daily_quest_reset_item_use_time);
#endif //_NA_000000_20130521_DAILY_QUEST_RESET_ITEM
#ifdef _NA_008404_20150712_MINIGAME_SYSTEM
    memcpy(&character->GetMiniGameBingoInfos(), &recv_msg->minigame_bingo_infos, \
        sizeof(recv_msg->minigame_bingo_infos));
#endif //_NA_008404_20150712_MINIGAME_SYSTEM

    character->SetExtraInfoModified(true);
}

template<class QueryForCharTransaction_T>
void DBResultCharacterTransaction(MSG_BASE * pMsg)
{
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        static_cast<QueryForCharTransaction*>(result->m_pData);
    //
    const USERGUID user_guid = query_result->GetUserKey();
    const CHARGUID char_guid = query_result->char_guid();
    //
    DBUser* db_user = NULL;
    DBCharacter* db_character = NULL;
    nsQuery::DBQueryProxyManager* const db_query_proxy_manager = \
        nsQuery::DBQueryProxyManager::Instance();
    nsQuery::DBQueryProxyManager::eFindResult find_result = \
        db_query_proxy_manager->FindUserWithPendings(user_guid, char_guid, &db_user, &db_character);
    if (find_result == db_query_proxy_manager->eFindResult_NotFound) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Can't find a character(%u) in user (%u)|"),
            __FUNCTION__, char_guid, user_guid);
        goto RELEASE_RESOURCE;
    };

    if (db_query_proxy_manager->DBResult_Char_Transaction(\
        *query_result, db_user, db_character) == false)
    {
        goto RELEASE_RESOURCE;
    }
    // success

RELEASE_RESOURCE:
    QueryForCharTransaction_T::FREE(static_cast<QueryForCharTransaction_T*>(query_result));
}

Handler_FromGameServer_IMPL(DG_CHARACTER_EXTRA_INFO_DBR)
{
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        static_cast<QueryForCharTransaction*>(result->m_pData);

    switch(query_result->transaction_type())
    {
        case nsQuery::ProxyTransaction::eTrans_CharacterExtraInfo_Select:
            DBResultCharacterTransaction<Query_CharacterExtraInfo_Select>(pMsg);
            break;
        case nsQuery::ProxyTransaction::eTrans_CharacterExtraInfo_Update:
            DBResultCharacterTransaction<Query_CharacterExtraInfo_Update>(pMsg);
            break;
    }
}
//~! _NA_006372_20130108_BADGE_ITEM

#ifdef _NA_007086_20140318_MONSTERBOOK
Handler_FromGameServer_IMPL(DG_CHARACTER_MONSTERBOOK_CMD)
{
    const MSG_DG_CHARACTER_MONSTERBOOK_CMD* const recv_msg = \
        static_cast<MSG_DG_CHARACTER_MONSTERBOOK_CMD*>(pMsg);
    const DWORD user_key = recv_msg->m_dwKey;

    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    if (requester == NULL) {
        return;
    }

    DBCharacter* character = requester->GetConnectedChar();
    if (character == NULL) {
        return;
    }

    switch(recv_msg->packet_type)
    {
        case DG_CHARACTER_MONSTERBOOK_ADD_CMD:
        {
            const MSG_DG_CHARACTER_MONSTERBOOK_ADD_CMD* const packet = \
                static_cast<MSG_DG_CHARACTER_MONSTERBOOK_ADD_CMD*>(pMsg);

            DWORD char_guid = requester->GetConnectedChar()->GetCharGuid();
            //
            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(_T("{?=call S_CharacterMonsterBook_Update (?, ?)}"));

            Query_CharacterMonsterBook_Update* query = Query_CharacterMonsterBook_Update::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_key);
            query->SetVoidObject(pServerSession);
            query->set_char_guid(char_guid);
            // setup parameters
            query->parameters_.char_guid_ = char_guid;
            query->parameters_.monster_code_ = packet->monster_code;
            //
            requester->DBQuery(DG_CHARINFO, DG_CHARACTER_MONSTERBOOK_DBR, query);
        } break;
    }
}

Handler_FromGameServer_IMPL(DG_CHARACTER_MONSTERBOOK_DBR)
{
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        static_cast<QueryForCharTransaction*>(result->m_pData);

    switch(query_result->transaction_type())
    {
        case nsQuery::ProxyTransaction::eTrans_CharacterMonsterBook_Select:
            DBResultCharacterTransaction<Query_CharacterMonsterBook_Select>(pMsg);
            break;
        case nsQuery::ProxyTransaction::eTrans_CharacterMonsterBook_Update:
            DBResultCharacterTransaction<Query_CharacterMonsterBook_Update>(pMsg);
            break;
    }
}
#endif //_NA_007086_20140318_MONSTERBOOK

#ifdef _NA_007932_20150106_USER_EXTRA_INFO
Handler_FromGameServer_IMPL(DG_USER_EXTRA_INFO_CMD)
{
    const MSG_DG_USER_EXTRA_INFO_CMD* const recv_msg = \
        static_cast<MSG_DG_USER_EXTRA_INFO_CMD*>(pMsg);
    const DWORD user_key = recv_msg->m_dwKey;

    DBUser* const requester = g_DBUserManager.FindUser(user_key);
    if (requester == NULL) {
        return;
    }

    // UserExtraInfo 추가시 수정
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    const DBUser::CHAR_HASH& char_list = requester->GetCharHash();
    FOREACH_CONTAINER(const DBUser::CHAR_HASH::value_type& node, char_list, DBUser::CHAR_HASH)
    {
        DBCharacter* db_character = node.second;
        memcpy(&db_character->GetAttendanceInfos(), &recv_msg->attendance_infos, \
            sizeof(recv_msg->attendance_infos));
    }
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM

    DBCharacter* character = requester->GetConnectedChar();
    if (character == NULL) {
        return;
    }

    character->SetUserExtraInfoModify(true);
}
Handler_FromGameServer_IMPL(DG_USER_EXTRA_INFO_DBR)
{
    MSG_DBPROXY_RESULT* result	= static_cast<MSG_DBPROXY_RESULT*>(pMsg);
    QueryForCharTransaction* query_result = \
        static_cast<QueryForCharTransaction*>(result->m_pData);

    switch(query_result->transaction_type())
    {
    case nsQuery::ProxyTransaction::eTrans_UserExtraInfo_Select:
        DBResultCharacterTransaction<Query_UserExtraInfo_Select>(pMsg);
        break;
    case nsQuery::ProxyTransaction::eTrans_UserExtraInfo_Update:
        DBResultCharacterTransaction<Query_UserExtraInfo_Update>(pMsg);
        break;
    }
}
#endif // _NA_007932_20150106_USER_EXTRA_INFO

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM

Handler_FromGameServer_IMPL(DG_SUN_RANKING_UPDATE_CMD)
{
    const MSG_DG_SUN_RANKING_UPDATE_CMD* const recv_msg = \
        static_cast<MSG_DG_SUN_RANKING_UPDATE_CMD*>(pMsg);

    const int max_buffer_size = 511; // 변경시 저장프로시저 및 아래 쿼리 버퍼 함께 변경해야함
    char buffer[max_buffer_size + 1] = {0, };
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        const int max_temp_buffer_size = 63;
        char temp_buffer[max_temp_buffer_size + 1] = {0, };
        _sntprintf(temp_buffer, max_temp_buffer_size, "%d,%I64d/", type, recv_msg->info.score[type]);
        strcat(buffer, temp_buffer);
    }

    STRING_SAFER_QUERY1024 query_buffer;

    query_buffer.MakeString("{?=call S_SunRanking_Updates(%d, '%s')}", 
        recv_msg->info.char_guid, buffer);


    Query_SunRanking_Update* query = Query_SunRanking_Update::ALLOC();

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_SUN_RANKING_UPDATE_DBR));
    query->SetVoidObject(pServerSession);
    query->SetRankInfo(recv_msg->info);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_UPDATE_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_SunRanking_Update* query_result = \
        static_cast<Query_SunRanking_Update*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_SunRanking_Update::FREE(query_result);
        return;
    }

    MSG_DG_SUN_RANKING_UPDATE_ACK ack_msg;
    ack_msg.info = query_result->GetRankInfo();

    g_DBProxyServer.SendToSessionType(GAME_SERVER, &ack_msg, sizeof(ack_msg));


    Query_SunRanking_Update::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_SELECT_SYN)
{
    const MSG_DG_SUN_RANKING_SELECT_SYN* const recv_msg = \
        static_cast<MSG_DG_SUN_RANKING_SELECT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buffer;

    query_buffer.MakeString("{?=call S_SunRanking_Select}");

    Query_SunRanking_Select* query = Query_SunRanking_Select::ALLOC();

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_SUN_RANKING_SELECT_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_SunRanking_Select* query_result = \
        static_cast<Query_SunRanking_Select*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_SunRanking_Select::FREE(query_result);
        return;
    }
    query_result->GetLastColumnInfo();

    std::map<CHARGUID, SunRankingPlayerInfo>& infos = query_result->GetInfos();
    std::map<CHARGUID, SunRankingPlayerInfo>::iterator cur_iter = infos.begin();
    std::map<CHARGUID, SunRankingPlayerInfo>::iterator end_iter = infos.end();
    int count = 0;
    MSG_DG_SUN_RANKING_SELECT_ACK ack_msg;
    ack_msg.total_msg_count = ((infos.size() - 1) / MSG_DG_SUN_RANKING_SELECT_ACK::MAX_SUN_RANKING_PLAYER_INFO) + 1;
    ack_msg.init();

    int send_packet_count = 0;

    if (infos.size() == 0)
    {
        ack_msg.total_msg_count = 1;
        pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
        send_packet_count++;
    }

    for (; cur_iter != end_iter; ++cur_iter)
    {
        ack_msg.infos[count] = cur_iter->second;
        if ((count + 1) % MSG_DG_SUN_RANKING_SELECT_ACK::MAX_SUN_RANKING_PLAYER_INFO == 0)
        {
            ack_msg.count = count + 1;
            pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
            send_packet_count++;

            ack_msg.init();
            count = 0;
        }
        else
        {
            ++count;
        }
    }

    if (count != 0)
    {
        ack_msg.count = count;
        pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
        send_packet_count++;
    }

    MSG_DG_SUN_RANKING_SELECT_COMPLETE_CMD cmd_msg;
    pServerSession->SendPacket(&cmd_msg, sizeof(cmd_msg));

    SUNLOG(eCRITICAL_LOG, "[SunRanking] - Ranking Select State : %d / %d - %d",
        send_packet_count,
        (infos.size() / MSG_DG_SUN_RANKING_SELECT_ACK::MAX_SUN_RANKING_PLAYER_INFO) + 1,
        infos.size());

    Query_SunRanking_Select::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_INIT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_SunRankingGrade_Init* query_result = \
        static_cast<Query_SunRankingGrade_Init*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_SunRankingGrade_Init::FREE(query_result);
        return;
    }

    SunRankingSettle::Instance()->NewStatueChar();

    Query_SunRankingGrade_Init::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_GRADE_INSERT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_SunRankingGrade_Insert* query_result = \
        static_cast<Query_SunRankingGrade_Insert*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_SunRankingGrade_Insert::FREE(query_result);
        return;
    }

    SunRankingSettle::Instance()->GetLoadControl().SetLoaded(eSUN_RANKING_GRADE_END * eSUN_RANKING_TYPE_END);
    SunRankingSettle::Instance()->NextGradeSettle(query_result->GetSunRankingType());

    Query_SunRankingGrade_Insert::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_GRADE_INIT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_SunRankingGrade_Init* query_result = \
        static_cast<Query_SunRankingGrade_Init*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_SunRankingGrade_Init::FREE(query_result);
        return;
    }

    SunRankingSettle::Instance()->SetState(eSUN_RANKING_SETTLE_STATE_SETTLE_GRADE);
    for (eSUN_RANKING_TYPE type = eSUN_RANKING_TYPE_START; type < eSUN_RANKING_TYPE_END; ++type)
    {
        SunRankingSettle::Instance()->NextGradeSettle(type);
    }

    Query_SunRankingGrade_Init::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_GRADE_SELECT_SYN)
{
    const MSG_DG_SUN_RANKING_GRADE_SELECT_SYN* const recv_msg = \
        static_cast<MSG_DG_SUN_RANKING_GRADE_SELECT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buffer;

    query_buffer.MakeString("{?=call S_SunRankingGrade_Select}");

    Query_SunRankingGrade_Select* query = Query_SunRankingGrade_Select::ALLOC();

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_SUN_RANKING_GRADE_SELECT_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_GRADE_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_SunRankingGrade_Select* query_result = \
        static_cast<Query_SunRankingGrade_Select*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_SunRankingGrade_Select::FREE(query_result);
        return;
    }
    query_result->GetLastColumnInfo();

    std::map<std::pair<CHARGUID, int>, SunRankingGradeInfo>& infos = query_result->GetInfos();
    std::map<std::pair<CHARGUID, int>, SunRankingGradeInfo>::iterator cur_iter = infos.begin();
    std::map<std::pair<CHARGUID, int>, SunRankingGradeInfo>::iterator end_iter = infos.end();
    int count = 0;
    MSG_DG_SUN_RANKING_GRADE_SELECT_ACK ack_msg;
    ack_msg.total_msg_count = ((infos.size() - 1) / MSG_DG_SUN_RANKING_GRADE_SELECT_ACK::MAX_SUN_RANKING_GRADE_INFO) + 1;
    ack_msg.init();

    int send_packet_count = 0;

    if (infos.size() == 0)
    {
        ack_msg.total_msg_count = 1;
        pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
        send_packet_count++;
    }

    for (; cur_iter != end_iter; ++cur_iter)
    {
        ack_msg.infos[count] = cur_iter->second;
        if ((count + 1) % MSG_DG_SUN_RANKING_GRADE_SELECT_ACK::MAX_SUN_RANKING_GRADE_INFO == 0)
        {
            ack_msg.count = count + 1;
            pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
            send_packet_count++;

            ack_msg.init();
            count = 0;
        }
        else
        {
            ++count;
        }
    }

    if (count != 0)
    {
        ack_msg.count = count;
        pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));
        send_packet_count++;
    }

    MSG_DG_SUN_RANKING_GRADE_SELECT_COMPLETE_CMD cmd_msg;
    pServerSession->SendPacket(&cmd_msg, sizeof(cmd_msg));

    SUNLOG(eCRITICAL_LOG, "[SunRanking] - Grade Select State : %d / %d - %d",
        send_packet_count, 
        (infos.size() / MSG_DG_SUN_RANKING_GRADE_SELECT_ACK::MAX_SUN_RANKING_GRADE_INFO) + 1,
        infos.size());

    Query_SunRankingGrade_Select::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_STATUE_ITEM_SLOT_INFO_INSERT_SYN)
{
    const MSG_DG_STATUE_ITEM_SLOT_INFO_INSERT_SYN* const recv_msg = \
        static_cast<MSG_DG_STATUE_ITEM_SLOT_INFO_INSERT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buffer;

    query_buffer.MakeString("{?=call S_StatueItemSlotInfo_Insert(%d, %d, %d)}",
        recv_msg->statue_type, recv_msg->statue_index, recv_msg->char_guid);

    Query_StatueItemSlotInfo_Insert* query = Query_StatueItemSlotInfo_Insert::ALLOC();

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_STATUE_ITEM_SLOT_INFO_INSERT_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_STATUE_ITEM_SLOT_INFO_INSERT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_StatueItemSlotInfo_Insert* query_result = \
        static_cast<Query_StatueItemSlotInfo_Insert*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_StatueItemSlotInfo_Insert::FREE(query_result);
        return;
    }

    Query_StatueItemSlotInfo_Insert::FREE(query_result);

    SunRankingSettle::Instance()->Complete();
}

Handler_FromGameServer_IMPL(DG_STATUE_SELECT_SYN)
{
    // 죄송합니다 시간이 없습니다 추후 리팩토링
    const MSG_DG_STATUE_SELECT_SYN* const recv_msg = \
        static_cast<MSG_DG_STATUE_SELECT_SYN*>(pMsg);

    MSG_DG_STATUE_ITEM_SLOT_INFO_SELECT_SYN msg;
    OnDG_STATUE_ITEM_SLOT_INFO_SELECT_SYN(pServerSession, &msg, sizeof(msg));
}

Handler_FromGameServer_IMPL(DG_STATUE_ITEM_SLOT_INFO_SELECT_SYN)
{
    const MSG_DG_STATUE_ITEM_SLOT_INFO_SELECT_SYN* const recv_msg = \
        static_cast<MSG_DG_STATUE_ITEM_SLOT_INFO_SELECT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buffer;

    query_buffer.MakeString("{?=call S_StatueItemSlotInfo_Select}");

    Query_StatueItemSlotInfo_Select* query = Query_StatueItemSlotInfo_Select::ALLOC();

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_STATUE_ITEM_SLOT_INFO_SELECT_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_STATUE_ITEM_SLOT_INFO_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_StatueItemSlotInfo_Select* query_result = \
        static_cast<Query_StatueItemSlotInfo_Select*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_StatueItemSlotInfo_Select::FREE(query_result);
        return;
    }

    nsQuery::ItemSlotUpdateNode* update_node_target = NULL;
    nsQuery::EquipSlotUpdateTable equip_table;
    CHARGUID char_guid = 0;
    const POSTYPE max_number_of_slots = equip_table.kMaxNumberOfSlots;
    {
        update_node_target = equip_table.records_;
        equip_table._OperateAll(equip_table.eOper_ClearAllToDefault |
            equip_table.eOper_CopyAllLatestDataToCommitedData,
            equip_table.records_,
            equip_table.kMaxNumberOfSlots,
            equip_table.kSlotIndex);

        int number_of_rows = query_result->GetResultRowNum();
        assert(number_of_rows <= max_number_of_slots);
        ;   number_of_rows = min(number_of_rows, max_number_of_slots);
        //
        for (int i = 0; i < number_of_rows; ++i)
        {
            const Query_StatueItemSlotInfo_Select::sQueryResult& record = query_result->result_records_[i];
            char_guid = record.char_guid;
            //!~
            if (record.pos_ <= 0 || record.pos_ > max_number_of_slots) {
                SUNLOG(eCRITICAL_LOG, "[%s|%d] record.pos_(%d) >= max_number_of_slots(%d)", 
                    __FUNCTION__, __LINE__, record.pos_, max_number_of_slots);
                continue;
            }
            //!~
            nsQuery::ItemSlotUpdateNode* const update_node = &update_node_target[record.pos_ - 1];
            // CHANGES: f101116.2L, fixed a problem. even though need update
            // if an inputed record is converted, it is not updated.
            nsQuery::ItemSlotVersionChecker::ConvertItem(record,
                &update_node->commited_data_, &update_node->latest_data_);
            //
            ITEMPART item_part = { 0, };
            if (nsQuery::ItemSlotResultRecord* latest_data = &update_node->latest_data_)
            {
                item_part.m_WorldSerial = latest_data->serial_;
                // NOTE: special operation. make to marked serial key.
                nsQuery::ItemSlotVersionChecker::SetWorldType(&item_part); // assert(db_user)
                latest_data->serial_ = item_part.m_WorldSerial;
            }
        }
    }

    _EQUIP_TOTAL_INFO equip_block; // & msg 처리
    ITEMSLOTEX_INSERVER* data_list = equip_block.m_Slot;
    POSTYPE result_number_of_slots = 0;
    {
        ITEMSLOTEX_INSERVER* data_it = data_list;
        const nsQuery::ItemSlotUpdateNode* update_it = update_node_target,
            * const update_end = update_node_target + max_number_of_slots;
        for (int pos = 0; update_it != update_end; ++update_it, ++pos)
        {
            const nsQuery::ItemSlotResultRecord& latest_data = update_it->latest_data_;
            if (latest_data.code_ == 0) {
                continue;
            }
            ITEMPART item_part = { 0, };
            item_part.m_WorldSerial = latest_data.serial_;
            item_part.wCode = static_cast<SLOTCODE>(latest_data.code_);
            item_part.byDura = static_cast<DURATYPE>(latest_data.dura_or_num_);
            //
            assert(latest_data.pos_ == pos + 1);
            data_it->m_Pos = static_cast<POSTYPE>(pos);
            data_it->m_Stream.Part = item_part;
            CopyMemory(&data_it->m_Stream.OptionPart, latest_data.option_stream_,
                sizeof(data_it->m_Stream.OptionPart));
            //
            ++data_it;
        }
        //
        result_number_of_slots = static_cast<POSTYPE>(data_it - data_list);
    }
    equip_block.m_Count = result_number_of_slots;

    MSG_DG_STATUE_CHAR_SELECT_SYN char_select_syn_msg;
    char_select_syn_msg.statue_index = static_cast<eSTATUE_INDEX>(query_result->result_records_[0].statue_index);
    char_select_syn_msg.statue_type = static_cast<eSTATUE_TYPE>(query_result->result_records_[0].statue_type);
    char_select_syn_msg.char_guid = char_guid;
    char_select_syn_msg.equip_info = equip_block;
    OnDG_STATUE_CHAR_SELECT_SYN(pServerSession, &char_select_syn_msg, sizeof(char_select_syn_msg));
    Query_StatueItemSlotInfo_Select::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_STATUE_CHAR_SELECT_SYN)
{
    const MSG_DG_STATUE_CHAR_SELECT_SYN* const recv_msg = \
        static_cast<MSG_DG_STATUE_CHAR_SELECT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buffer;

    query_buffer.MakeString("{? = call S_Statue_Char_Select (%d)}", recv_msg->char_guid);

    Query_Statue_Char_Select* query = Query_Statue_Char_Select::ALLOC();

    query->equip_info = recv_msg->equip_info;
    query->statue_type = recv_msg->statue_type;
    query->statue_index = recv_msg->statue_index;
    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_STATUE_CHAR_SELECT_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_STATUE_CHAR_SELECT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_Statue_Char_Select* query_result = \
        static_cast<Query_Statue_Char_Select*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_Statue_Char_Select::FREE(query_result);
        return;
    }
    
    MSG_DG_STATUE_SELECT_ACK ack_msg;
    ack_msg.count = query_result->GetResultRowNum();

    StatueInfo* statue_info = &ack_msg.statue_info;
    statue_info->clear();
    statue_info->statue_index = query_result->statue_index;
    statue_info->statue_type = query_result->statue_type;

    CLIENT_CHARACTER_PART* client_character_part = &statue_info->client_character_part;
    _EQUIP_TOTAL_INFO& equip_info = query_result->equip_info;

    const int number_of_records = query_result->GetResultRowNum();
    if (number_of_records > 0)
    {
        Query_Statue_Char_Select::sQUERY_RESULT* pQuerySrcInfo = \
            &(query_result->pResult[0]);
        pQuerySrcInfo->SetSafeBound(); // f101007.4L

        /************************************************************************/
        /* Query -> CLIENT_CHARACTER_PART                                       */
        /************************************************************************/
        client_character_part->m_bySlot = static_cast<uint8_t>(pQuerySrcInfo->m_bySlot);
        client_character_part->m_byClass = pQuerySrcInfo->m_byClass;
        BOOST_STATIC_ASSERT(
            sizeof(client_character_part->m_tszCharName) == sizeof(pQuerySrcInfo->result_info_char_name_) &&
            _countof(client_character_part->m_tszCharName) == _countof(pQuerySrcInfo->result_info_char_name_));

        int len = _sntprintf(client_character_part->m_tszCharName, _countof(client_character_part->m_tszCharName),
            "%s", pQuerySrcInfo->result_info_char_name_);
        assert(len > 0);
        client_character_part->m_tszCharName[_countof(client_character_part->m_tszCharName) - 1] = _T('\0');
        client_character_part->m_byHeight = pQuerySrcInfo->m_byHeight;
        client_character_part->m_byFace = pQuerySrcInfo->m_byFace;
        client_character_part->m_byHair = pQuerySrcInfo->m_byHair;
        client_character_part->gender = pQuerySrcInfo->gender;
        client_character_part->costume_item_pos = pQuerySrcInfo->costume_item_pos;
        client_character_part->costume_item_code = pQuerySrcInfo->costume_item_code;
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        client_character_part->decorate_costume_item_pos = pQuerySrcInfo->deco_costume_item_pos;
        client_character_part->decorate_costume_item_code = pQuerySrcInfo->deco_costume_item_code;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

        BOOST_STATIC_ASSERT(_countof(client_character_part->m_EquipItemInfo.m_Slot) == _countof(equip_info.m_Slot));
        int number_of_items = min(equip_info.m_Count, MAX_EQUIPMENT_SLOT_NUM);
        EQUIP_ITEM_INFO::SLOT_TYPE* dest_it = client_character_part->m_EquipItemInfo.m_Slot;
        const _EQUIP_TOTAL_INFO::SLOT_TYPE* src_it = equip_info.m_Slot;
        const _EQUIP_TOTAL_INFO::SLOT_TYPE* src_end = &equip_info.m_Slot[number_of_items];
        for (; src_it != src_end; ++src_it, ++dest_it)
        {
            dest_it->m_Pos = src_it->m_Pos;
            CopyStream(dest_it->m_Stream, src_it->m_Stream);
        };
        client_character_part->m_EquipItemInfo.m_Count = static_cast<POSTYPE>(number_of_items);
        client_character_part->m_bSummoned = FALSE;
        client_character_part->is_equipped_costume = FALSE;
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        client_character_part->is_equipped_decorate_costume = FALSE;
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME

        /************************************************************************/
        /* Query -> StatueInfo                                                  */
        /************************************************************************/
        statue_info->pattern_mark_index = pQuerySrcInfo->m_PatternMarkIndex;
        statue_info->background_mark_index = pQuerySrcInfo->m_BackGroundMarkIndex;
        BOOST_STATIC_ASSERT(
            (sizeof(statue_info->guild_name) == sizeof(pQuerySrcInfo->result_info_guild_name_)) 
            && (_countof(statue_info->guild_name) ==MAX_GUILDNAME_LENGTH + 1));
        strncpy(statue_info->guild_name, pQuerySrcInfo->result_info_guild_name_,
            _countof(statue_info->guild_name));
        statue_info->guild_name[_countof(statue_info->guild_name) - 1] = '\0';
        statue_info->total_equip_score = pQuerySrcInfo->m_total_equip_score;
    }

    pServerSession->SendPacket(&ack_msg, sizeof(ack_msg));

    Query_Statue_Char_Select::FREE(query_result);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_SETTLE_START_SYN)
{
    const MSG_DG_SUN_RANKING_SETTLE_START_SYN* const recv_msg = \
        static_cast<MSG_DG_SUN_RANKING_SETTLE_START_SYN*>(pMsg);

    MSG_DG_SUN_RANKING_SETTLE_START_CMD cmd_msg;
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &cmd_msg, sizeof(cmd_msg));
}


Handler_FromGameServer_IMPL(DG_SUN_RANKING_SETTLE_SYN)
{
    const MSG_DG_SUN_RANKING_SETTLE_SYN* const recv_msg = \
        static_cast<MSG_DG_SUN_RANKING_SETTLE_SYN*>(pMsg);

    SunRankingSettle::Instance()->SetStatueInfo(recv_msg->statue_char_guid, recv_msg->statue_char_equip_score);
    SunRankingSettle::Instance()->SetGradeBoundary(recv_msg->grade_boundary);
    SunRankingSettle::Instance()->InitGrade(pServerSession);
}

Handler_FromGameServer_IMPL(DG_SUN_RANKING_SETTLE_COMPLETE_SYN)
{
    const MSG_DG_SUN_RANKING_SETTLE_COMPLETE_SYN* const recv_msg = \
        static_cast<MSG_DG_SUN_RANKING_SETTLE_COMPLETE_SYN*>(pMsg);
    
    MSG_DG_SUN_RANKING_SETTLE_COMPLETE_CMD cmd_msg;
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &cmd_msg, sizeof(cmd_msg));
}


Handler_FromGameServer_IMPL(DG_STATUE_CHAR_INSERT_SYN)
{
    const MSG_DG_STATUE_CHAR_INSERT_SYN* const recv_msg = \
        static_cast<MSG_DG_STATUE_CHAR_INSERT_SYN*>(pMsg);

    STRING_SAFER_QUERY1024 query_buffer;

    query_buffer.MakeString("{?=call S_Statue_Char_Insert (%d,%d,%d,%I64d) }",
        recv_msg->statue_type, recv_msg->statue_index, recv_msg->char_guid, recv_msg->score);

    Query_StatueCharInfo_Select* query = Query_StatueCharInfo_Select::ALLOC();

    query->SetQuery(query_buffer);
    query->SetIndex(MAKEDWORD((WORD)DG_SUN_RANKING, (WORD)DG_STATUE_CHAR_INSERT_DBR));
    query->SetVoidObject(pServerSession);
    pServerSession->DBQuery(query);
}

Handler_FromGameServer_IMPL(DG_STATUE_CHAR_INSERT_DBR)
{
    __TOUCHED((pServerSession, wSize));
    const MSG_DBPROXY_RESULT* db_result = static_cast<MSG_DBPROXY_RESULT*>(pMsg);

    Query_StatueCharInfo_Select* query_result = \
        static_cast<Query_StatueCharInfo_Select*>(db_result->m_pData);
    if(query_result->IsError())
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"](%s):%s",
            query_result->GetQuery(), query_result->GetErrorMsg());
        Query_StatueCharInfo_Select::FREE(query_result);
        return;
    }

    SunRankingSettle::Instance()->NewStatueCharItem();

    Query_StatueCharInfo_Select::FREE(query_result);
}
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
Handler_FromGameServer_IMPL(DG_NOTIFICATION_ITEM_SYN)
{
    const MSG_DG_NOTIFICATION_ITEM_SYN* const recv_msg = \
        static_cast<MSG_DG_NOTIFICATION_ITEM_SYN*>(pMsg);

    MSG_DW_NOTIFICATION_ITEM_SYN send_msg;
    send_msg.info = recv_msg->info;
    g_DBProxyServer.SendToWorldServer(&send_msg, sizeof(MSG_DW_NOTIFICATION_ITEM_SYN));
}

Handler_FromGameServer_IMPL(DG_NOTIFICATION_CLEAR_SYN)
{
    const MSG_DG_NOTIFICATION_CLEAR_SYN* const recv_msg = \
        static_cast<MSG_DG_NOTIFICATION_CLEAR_SYN*>(pMsg);

    MSG_DW_NOTIFICATION_CLEAR_SYN send_msg;
    g_DBProxyServer.SendToWorldServer(&send_msg, sizeof(send_msg));
}
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM