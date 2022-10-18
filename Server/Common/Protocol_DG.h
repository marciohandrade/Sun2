

#ifndef __PROTOCOL_DG_H__
#define __PROTOCOL_DG_H__

//-------------------------------------------------------------------------------------------------------
// *  Prefix
//		C : Client
//		G : Game Server
//		M : Master Server
//		D : DBP Server
//
// * Postfix
//		SYN - Server�� ��û
//		ACK - ��û�� ���� ���
//		NAK - ��û�� ���� �ź�
//		CMD - Server�� ����
//		BRD - Server�� ����ȭ ���� ������ �˸�
//		DBR - DBProxy������ �����!
//
// * �������� Naming Rule
//		Prefix_ī�װ�����_�������ݸ�_Postfix
//		ex) CG_CONNECTION_REGISTER_SYN
//
//-------------------------------------------------------------------------------------------------------


// DBP Server <-> GameServer Protocol
enum eDG_CATEGORY
{
	DG_CONNECTION		= 1,
	DG_CHARINFO,
	DG_ITEM,
	DG_WAREHOUSE,
	DG_OTHER,
	DG_EVENT,
	DG_MISSION,
	DG_WAYPOINT,
	DG_COLLECT,
    DP_RANKING, // CHANGES: f110509.4L
    GP_RANKING, // CHANGES: f110509.4L
    DZ_GUILD, // CHANGES: f110518.2L
    GZ_GUILD, // CHANGES: f110518.2L
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    DG_SUN_RANKING,
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    DG_NOTIFICATION,
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    //
	DG_MAX,
};


// CONNECTION = 1,		// ���� ����
enum eDG_CONNECTION_PROTOCOL
{
	DG_CONNECTION_RECOVER_SYN,
	DG_CONNECTION_RECOVER_ACK,
	DG_CONNECTION_RECOVER_NAK,
		
	DG_CONNECTION_CONNECT_SYN,
	DG_CONNECTION_CONNECT_ACK,
	DG_CONNECTION_CONNECT_NAK,
	DG_CONNECTION_CONNECT_DBR,

	DG_CONNECTION_ENTER_SYN,
	DG_CONNECTION_ENTER_ACK,
	DG_CONNECTION_ENTER_NAK,
	DG_CONNECTION_ENTER_DBR,

	DG_CONNECTION_LEAVE_SYN,
	DG_CONNECTION_LEAVE_ACK,
	DG_CONNECTION_LEAVE_NAK,
	DG_CONNECTION_LEAVE_DBR,

	DG_CONNECTION_DISCONNECT_CMD,
	DG_CONNECTION_DISCONNECT_DBR,

    DG_CONNECTION_DBCONNECT_UPDATE_CMD, //_NA000000_DBQUERY_REVERSAL_STATE_PREVENTION
    DG_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
};

enum eDG_CHARINFO_PROTOCOL
{
	//DG_CHARINFO_ALL_REQ_SYN,
	//DG_CHARINFO_ALL_REQ_ACK,
	//DG_CHARINFO_ALL_REQ_NAK,
	//DG_CHARINFO_ALL_REQ_DBR,
	
	// DB�� ����ϱ� ���� ��������
	DG_CHARINFO_CHAR_ITEM_CMD,
	DG_CHARINFO_SKILL_CMD,
	DG_CHARINFO_QUICK_CMD,
	DG_CHARINFO_STYLE_CMD,
	DG_CHARINFO_QUEST_CMD,
	DG_CHARINFO_WAREHOUSE_CMD,
	DG_CHARINFO_EXTRA_CMD,

	DG_CHARINFO_CONNECT_CHAR_CMD,
	DG_CHARINFO_DISCONNECT_CHAR_CMD,
	DG_CHARINFO_DISCONNECT_CMD,

	DG_CHARINFO_QUERY_SYN,
	DG_CHARINFO_QUERY_ACK,
	DG_CHARINFO_QUERY_NAK,

	DG_CHARINFO_UPDATE_SYN,
	DG_CHARINFO_UPDATE_ACK,
	DG_CHARINFO_UPDATE_NAK,
	DG_CHARINFO_UPDATE_DBR,

	DG_CHARINFO_WAREHOUSE_INVENTORY_PWD_UPDATE_DBR,
	DG_CHARINFO_UPDATE_TRANSACTION_CONTROL_SYN,
    // _NA_20100106_REPURCHAS_BUGFIX_2496
    DG_CHARINFO_CHAR_REPURCHASE_ITEM_CMD,
    // added by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    DG_CHARINFO_UPDATE_INVEN_DBR,
    //
    DG_CHARINFO_INVITATEDFRIEND_CMD,
    DG_CHARINFO_CHARLISTREQ_INTER_ITEMSLOTINFO_DBR,
    DG_CHARINFO_CHARLISTREQ_INTER_SLOTINFO_DBR,
    DG_CHARINFO_CHARLISTREQ_INTER_PERK_DBR,
    DG_CHARINFO_SKILL_LIST_CMD, // streaming serialize
    DG_CHARINFO_SKILL_PERK_CMD, // streaming serialize
    DG_CHARINFO_SKILL_PERK_CONTROL_CMD, // perk operation
    //
    DG_CHARINFO_UPDATE_INTER_QUEST_DBR,
    DG_CHARINFO_UPDATE_INTER_BATTLERECORDS_DBR,
    DG_CHARINFO_CHARLISTREQ_INTER_CHAR_STATE_DBR,
    // C_NA_0_20100520_ACSYSTEM_CHANGED
	DG_CHARINFO_SELECT_ACHIEVEMENT_DBR,
	DG_CHARINFO_UPDATE_ACHIEVEMENT_DBR,
	DG_CHARINFO_CHAR_ACHIEVEMENT_CMD,
    DG_CHARINFO_CHARLISTREQ_INTER_MISSION_DBR,

    //_NA_0_20110308_SKILL_COOL_TIME_STORAGE
    DG_CHARINFO_SKILL_COOL_TIME_CMD,

    //_NA_003027_20111013_HONOR_SYSTEM
    DG_CHARINFO_HONORSYSTEM,
    DG_CHARINFO_HONORTITLES_SELECT_DBR,
    DG_CHARINFO_HONORTITLES_ADD_REMOVE_DBR,
    DG_CHARINFO_FAMEREPUTE_SELECT_DBR,
    DG_CHARINFO_FAMEREPUTE_CHANGE_DBR,
    DG_CHARINFO_DAILYQUEST_SELECT_DBR,
    DG_CHARINFO_DAILYQUEST_UPDATE_DBR,

    //_NA004034_20120102_POINT_WALLET_SYSTEM
    DG_CHARINFO_POINTWALLET_SYSTEM,
    DG_CHARINFO_POINTWALLET_SELECT_DBR,
    DG_CHARINFO_POINTWALLET_UPDATE_DBR,

    //_NA004034_20120102_GUILD_POINT_SYSTEM
    DG_CHARINFO_POINTWALLET_GUILDCOIN_DONATION_DBR,

    DG_CHARNAME_CHANGE_CMD,
    DG_CHARNAME_CHANGE_DBR,
    DG_CHARNAME_CHANGE_ACK,

    //!~ _NA_006372_20130108_BADGE_ITEM
    DG_CHARACTER_EXTRA_INFO_CMD,
    DG_CHARACTER_EXTRA_INFO_DBR,
    //~! _NA_006372_20130108_BADGE_ITEM

    //!~ _NA_007932_20150106_USER_EXTRA_INFO
    DG_USER_EXTRA_INFO_CMD,
    DG_USER_EXTRA_INFO_DBR,
    //~! _NA_007932_20150106_USER_EXTRA_INFO

    //!~ _NA_007086_20140318_MONSTERBOOK
    DG_CHARACTER_MONSTERBOOK_CMD,
    DG_CHARACTER_MONSTERBOOK_DBR,
    //~! _NA_007086_20140318_MONSTERBOOK
};
// OTHER, 	// ��Ÿ
enum eDG_ITEM_PROTOCOL
{
	DG_ITEM_SERIAL_SYN,
	DG_ITEM_SERIAL_ACK,
	DG_ITEM_SERIAL_NAK,

	DG_ITEM_INVENTORY_FLAG_STREAM_NTF,		// G->D ��Ʈ�� ���� (�ӽ� ����)
	DG_ITEM_INVENTORY_FLAG_STREAM_CMD,		// D->G ��Ʈ�� ����
	//DG_ITEM_INVENTORY_PWD_SETTING_SYN, --> DG_WAREHOUSE_PWD_SETTING_SYN �� �ش� ��ƾ�� ���ԵǾ� �ִ�.
	DG_ITEM_FATE_ITEM_IDENTIFY_SYN, // NOTE: f110428.5L, don't use anymore since 1102
	DG_ITEM_FATE_ITEM_IDENTIFY_ACK,
};

enum eDG_WAREHOUSE_PROTOCOL
{
	DG_WAREHOUSE_START_SYN,
	DG_WAREHOUSE_START_DBR,
	DG_WAREHOUSE_START_ACK,
	DG_WAREHOUSE_START_NAK,

	DG_WAREHOUSE_END_SYN,
	DG_WAREHOUSE_END_ACK,
	DG_WAREHOUSE_END_NAK,

    // WARNING: need revision base update
    DG_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR,
    DG_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR,
	DG_WAREHOUSE_UPDATE_INNER_DBR,

	DG_WAREHOUSE_FLAG_STREAM_NTF,		// G->D ��Ʈ�� ���� (�ӽ� ����)
	DG_WAREHOUSE_FLAG_STREAM_CMD,		// D->G ��Ʈ�� ����
	DG_WAREHOUSE_PWD_SETTING_SYN,
	DG_WAREHOUSE_PWD_SETTING_ACK,

	// ��� â��
	DG_GUILD_WAREHOUSE_CMD,
	DG_GUILD_WAREHOUSE_UPDATE_INNER_DBR,
    DG_GUILD_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR,

	DG_GUILD_WAREHOUSE_CLOSE_ACK,
	DG_GUILD_WAREHOUSE_CLOSE_NAK,
	DG_GUILD_WAREHOUSE_CLOSE_SYN,
	
	DG_GUILD_WAREHOUSELOG_UPDATE_SYN,
	DG_GUILD_WAREHOUSELOG_UPDATE_DBR,

	DG_GUILD_WAREHOUSE_TAPEX_SYN,
	DG_GUILD_WAREHOUSE_TAPEX_ACK,
	DG_GUILD_WAREHOUSE_TAPEX_NAK,
	DG_GUILD_WAREHOUSE_TAPEX_DBR,

	DG_GUILD_WAREHOUSE_FLAG_STREAM_NTF,		// G->D ��Ʈ�� ���� (�ӽ� ����)
	DG_GUILD_WAREHOUSE_FLAG_STREAM_CMD,	// D->G ��Ʈ�� ����
    //
};

enum eDG_OTHER_PROTOCOL
{
	DG_OTHER_EXIST_CHARNAME_SYN,
	DG_OTHER_EXIST_CHARNAME_DBR,
	DG_OTHER_EXIST_CHARNAME_ACK,
	DG_OTHER_EXIST_CHARNAME_NAK,

	DG_OTHER_SHOP_RECV_GIFT_NTF,	//< ���� World<->Game�� ������ �ɰ��, ���� ����!

    // __CN_OO0510__FATIGUE_MATCHING_UPDATE
	DG_OTHER_FATIGUE_CMD,

    // __NA_001359_20090619_HEIM_LOTTO_SYSTEM
    DG_OTHER_HEIM_LOTTO_PICK_NTF,   // ���� ��� ���� ���� ���� (�α� ����)

#ifdef _NA_1419_20100121_ACCUMULATE_POINT
    DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF, // ��������Ʈ �⵵�� ������ ����
#endif

    DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_CMD, // ������ �ռ� �Ǵ� ���� ���� ����
    DG_OTHER_INSERT_COMPOSITION_OR_DECOMPOSITION_HISTORY_DBR, 
    DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_SYN, // ������ �ռ� �Ǵ� ���� ������ ������
    DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR, 
    DG_OTHER_SELECT_COMPOSITION_OR_DECOMPOSITION_HISTORIES_ACK, 
    DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_CMD, // ������ �ռ� �Ǵ� ���� ������ ����
    DG_OTHER_UPDATE_COMPOSITION_OR_DECOMPOSITION_HISTORIES_DBR, 

    DG_OTHER_EXTEND_CHAR_SLOT_SYN, // ĳ���� ���� Ȯ��
    DG_OTHER_EXTEND_CHAR_SLOT_DBR, 
    DG_OTHER_EXTEND_CHAR_SLOT_ACK, 
};

enum eDG_EVENT_PROTOCOL
{
	DG_EVENT_SELECT_SYN,
	DG_EVENT_SELECT_DBR,
	DG_EVENT_SELECT_ACK,
	DG_EVENT_SELECT_NAK,

	DG_EVENT_UPDATE_SYN,					// Rocket Battery Table Update...-.- ��, ������ �� �� ����
	DG_EVENT_UPDATE_DBR,
	DG_EVENT_UPDATE_ACK,
	DG_EVENT_UPDATE_NAK,

	DG_EVENT_ITEM_STATUS_SYN,				// ���º�Ÿ�� ���׸� �̺�Ʈ ��ϻ��� Ȯ��
	DG_EVENT_ITEM_STATUS_DBR,
	DG_EVENT_ITEM_STATUS_ACK,
	DG_EVENT_ITEM_STATUS_NAK,

	DG_EVENT_ITEM_REG_SYN,					// �̺�Ʈ ������ ��� ��û
	DG_EVENT_ITEM_REG_DBR,
	DG_EVENT_ITEM_REG_ACK,
	DG_EVENT_ITEM_REG_NAK,

	DG_EVENT_USETIME_CMD,					// �̺�Ʈ ��� �ð� �˸�

	DG_EVENT_QUERY_EXIST_SYN	,
	DG_EVENT_QUERY_EXIST_DBR	,
	DG_EVENT_QUERY_EXIST_ACK	,

	DG_EVENT_STATUS_UPDATE_SYN	,
	DG_EVENT_STATUS_UPDATE_DBR	,
	DG_EVENT_STATUS_UPDATE_ACK	,

	//__NA001094_080904_TASK_SSQ_REWARD_PROCESS__
	DG_EVENT_SSQ_REWARD_EXIST_CMD	,	// D->G (���� ���� ���� ���)
	DG_EVENT_SSQ_REWARD_UPDATE_SYN	,	// G->D
	DG_EVENT_SSQ_REWARD_UPDATE_DBR	,	// DB RESULT
	DG_EVENT_SSQ_REWARD_UPDATE_ACK	,	// D->G

    //_KR_0_20091021_FIRENDINVITATION_EXTEND = {
    DG_EVENT_INVITATEFRIEND_REWARDUPDATE_SYN,
    DG_EVENT_INVITATEFRIEND_REWARDUPDATE_ACK,
    DG_EVENT_INVITATEFRIEND_REWARDUPDATE_NAK,
    DG_EVENT_INVITATEFRIEND_REWARDUPDATE_DBR,

    DG_EVENT_INVITATEFRIEND_RECALLSELECT_SYN,
    DG_EVENT_INVITATEFRIEND_RECALLSELECT_NAK,
    DG_EVENT_INVITATEFRIEND_RECALLSELECT_ACK,
    DG_EVENT_INVITATEFRIEND_RECALLSELECT_DBR,    

    DG_EVENT_INVITATEFRIEND_RECALLUPDATE_CMD,    
    DG_EVENT_INVITATEFRIEND_RECALLUPDATE_DBR,    
    //}
    //_NA_20100420_CHANGE_SYSTEM_MEMO_LOGIC
    DG_EVENT_DOMINATION_REWARD_AND_DEL_SYN,
    DG_EVENT_DOMINATION_REWARD_AND_DEL_ACK,
    DG_EVENT_DOMINATION_REWARD_AND_DEL_DBR,
    DG_EVENT_LOTTO_REWARD_AND_DEL_SYN,
    DG_EVENT_LOTTO_REWARD_AND_DEL_ACK,
    DG_EVENT_LOTTO_REWARD_AND_DEL_DBR,
    DG_EVENT_DOMINATION_ITEM_ROLLBACK_SYN,

    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION
    DG_EVENT_SYSTEMMEMO_ITEM_CHECK_SYN,
    DG_EVENT_SYSTEMMEMO_ITEM_CHECK_ACK,
    DG_EVENT_SYSTEMMEMO_ITEM_CHECK_DBR,
    DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_CMD,
    DG_EVENT_SYSTEMMEMO_ITEM_REWARD_UPDATE_DBR,

    //!~ _NA_000000_20140120_SMART_NPC_SYSTEM
    DG_EVENT_SMARTNPC_SYSTEM,
    DG_EVENT_SET_ATTENDANCE_DBR,
    DG_EVENT_GET_ATTENDANCE_DBR,
    //~! _NA_000000_20140120_SMART_NPC_SYSTEM

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_SYN,
    DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_DBR,
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    //_NA_007919_20141211_ADD_GMCOMMAND_EVENT
    DG_GMEVENT_PEAKTIME_REGISTER_SYN,
    DG_GMEVENT_PEAKTIME_REGISTER_ACK,

    DG_GMEVENT_PEAKTIME_TERMINATE_SYN,
    DG_GMEVENT_PEAKTIME_TERMINATE_ACK,
};

#ifdef _NA_008012_20150130_RANKING_SYSTEM
enum eDG_MISSION_PROTOCOL
{
	// �̼� ��ŷ ����
	DG_MISSION_RECORD_SELECT_DBR,			//< ������ �̼� ����� �����´�.
	DG_MISSION_RECORD_SELECT_CMD,

	//DG_MISSION_CLEAR_UPDATE_SYN,			//< ��Ƽ�� �̼� ��ŷ�� ����Ѵ�.
	//DG_MISSION_CLEAR_UPDATE_ACK,
	//DG_MISSION_CLEAR_UPDATE_NAK,
	//DG_MISSION_CLEAR_UPDATE_DBR,

	//DG_MISSION_RECORD_UPDATE_SYN,			//< ���� �̼� ����� �����Ѵ�.
	//DG_MISSION_RECORD_UPDATE_ACK,
	//DG_MISSION_RECORD_UPDATE_NAK,
	//DG_MISSION_RECORD_UPDATE_DBR,

	DG_MISSION_RANKING_POINT_REQ_SYN,		//< ������ ��ŷ ����Ʈ�� �����´�.
	DG_MISSION_RANKING_POINT_REQ_DBR,
	DG_MISSION_RANKING_POINT_REQ_ACK,
};
#endif // _NA_008012_20150130_RANKING_SYSTEM

enum eDG_WAYPOINT_PROTOCOL
{
	// �̼� ��ŷ ����
	DG_WAYPOINT_INFO_LOAD_SYN,
	DG_WAYPOINT_INFO_LOAD_ACK,
	DG_WAYPOINT_INFO_LOAD_NAK,
	DG_WAYPOINT_INFO_LOAD_DBR,

	DG_WAYPOINT_INFO_SAVE_SYN,
	DG_WAYPOINT_INFO_SAVE_ACK,
	DG_WAYPOINT_INFO_SAVE_NAK,
	DG_WAYPOINT_INFO_SAVE_DBR,
    //{__NA001164_20081024_CHAR_WAY_POINT_SET_DB_PACKET
	DG_CHAR_WAYPOINT_INFO_LOAD_SYN,
	DG_CHAR_WAYPOINT_INFO_LOAD_ACK,
	DG_CHAR_WAYPOINT_INFO_LOAD_DBR,

	DG_CHAR_WAYPOINT_INFO_SAVE_SYN,
	DG_CHAR_WAYPOINT_INFO_SAVE_ACK,
	DG_CHAR_WAYPOINT_INFO_SAVE_DBR,

	DG_CHAR_WAYPOINT_INFO_DEL_SYN,
	DG_CHAR_WAYPOINT_INFO_DEL_ACK,
	DG_CHAR_WAYPOINT_INFO_DEL_DBR,

	DG_CHAR_WAYPOINT_INFO_NAK,
    //}__NA001164_20081024_CHAR_WAY_POINT_SET_DB_PACKET
};

//__NA001283_20090227_COLLECTION_LIST
enum eDG_COLLECT_PROTOCOL
{
	DG_COLLECT_LIST_LOAD_SYN,
	DG_COLLECT_LIST_LOAD_ACK,
	DG_COLLECT_LIST_LOAD_DBR,

	DG_COLLECT_ADD_SYN,
	DG_COLLECT_ADD_ACK,
	DG_COLLECT_ADD_DBR,

	DG_COLLECT_DEL_SYN,
	DG_COLLECT_DEL_ACK,
	DG_COLLECT_DEL_DBR,

	DG_COLLECT_NAK,
};

// _NA002635_GP_DP_PROTOCOL_UINIFICATION

#ifdef _NA_008012_20150130_RANKING_SYSTEM
/// ��������
enum eDP_RANKING_PROTOCOL
{
	DP_RANKING_INFO_REQ_SYN,
	DP_RANKING_INFO_REQ_DBR,
	DP_RANKING_INFO_REQ_ACK,
	DP_RANKING_INFO_REQ_NAK,
	DP_RANKING_INFO_REQ_END_NTF,

	DP_RANKING_MISSION_CLEAR_SYN,
	DP_RANKING_MISSION_CLEAR_DBR,
	DP_RANKING_MISSION_CLEAR_ACK,
	DP_RANKING_MISSION_CLEAR_NAK,

	DP_RANKING_MISSION_REWARD_SYN,
	DP_RANKING_MISSION_REWARD_DBR,
	DP_RANKING_MISSION_REWARD_ACK,
	DP_RANKING_MISSION_REWARD_NAK,

	DP_RANKING_REGISTER_MISSION_POINT_SYN,
	DP_RANKING_REGISTER_MISSION_POINT_DBR,
	DP_RANKING_REGISTER_MISSION_POINT_ACK,
	DP_RANKING_REGISTER_MISSION_POINT_NAK,
};

enum eGP_RANKING_PROTOCOL
{
    GP_RANKING_SESSION_CONTROL_CMD, // NOTE: f110511.1L,

	GP_RANKING_INFO_REQ_SYN,
	GP_RANKING_INFO_REQ_ACK,
	GP_RANKING_INFO_REQ_NAK,
	GP_RANKING_INFO_REQ_END_NTF,

	GP_RANKING_MISSION_CLEAR_SYN,
	//GP_RANKING_MISSION_CLEAR_ACK,
	//GP_RANKING_MISSION_CLEAR_NAK,
	GP_RANKING_MISSION_SUCCESS_ACK,
	GP_RANKING_MISSION_SUCCESS_BRD,
	GP_RANKING_MISSION_FAILED_NAK,

	GP_RANKING_MISSION_REWARD_SYN,
	GP_RANKING_MISSION_REWARD_ACK,
	GP_RANKING_MISSION_REWARD_BRD, //< �߰� �۾� �ؾ���
	GP_RANKING_MISSION_REWARD_NAK,

	// ����� ��ŷ ����
	GP_RANKING_SETTLED_INFO_CMD,
	GP_RANKING_SETTLED_INFO_END_NTF,
	GP_RANKING_CHAGED_STATUS_NTF, //< ���� ���� �˸�

	// GM ��ŷ ���� ���� ��Ŷ
	GP_RANKING_SETTLE_CMD,

	GP_RANKING_REGISTER_MISSION_POINT_SYN,	

	GP_MISSION_RANKING_SETTLE_DATE_SYN,
	GP_MISSION_RANKING_SETTLE_DATE_ACK,	
};
#endif // _NA_008012_20150130_RANKING_SYSTEM

//_NA002635_GZ_DZ_PROTOCOL_UINIFICATION

enum eDZ_GUILD_PROTOCOL
{
	DZ_GUILD_SELECT_SYN,
	DZ_GUILD_SELECT_DBR,
	DZ_GUILD_SELECT_ACK,
	DZ_GUILD_SELECT_NAK,

	//DZ_GUILD_SELECT_GUILD_MEMBER_SYN,
	DZ_GUILD_SELECT_MEMBER_DBR,
	//DZ_GUILD_SELECT_GUILD_MEMBER_ACK,
	//DZ_GUILD_SELECT_GUILD_MEMBER_NAK,

	DZ_GUILD_CREATE_SYN,
	DZ_GUILD_CREATE_DBR,
	DZ_GUILD_CREATE_ACK,
	DZ_GUILD_CREATE_NAK,

	DZ_GUILD_DESTROY_SYN,
	DZ_GUILD_DESTROY_DBR,
	DZ_GUILD_DESTROY_ACK,
	DZ_GUILD_DESTROY_NAK,

	DZ_GUILD_DONATE_UP_SYN,
	DZ_GUILD_DONATE_UP_DBR,
	DZ_GUILD_DONATE_UP_ACK,
	DZ_GUILD_DONATE_UP_NAK,

	DZ_GUILD_CONVERT_UP2GP_SYN,
	DZ_GUILD_CONVERT_UP2GP_DBR,
	DZ_GUILD_CONVERT_UP2GP_ACK,
	DZ_GUILD_CONVERT_UP2GP_NAK,

	DZ_GUILD_CHANGE_NOTICE_SYN,
	DZ_GUILD_CHANGE_NOTICE_DBR,
	DZ_GUILD_CHANGE_NOTICE_ACK,
	DZ_GUILD_CHANGE_NOTICE_NAK,

	DZ_GUILD_JOIN_SYN,
	DZ_GUILD_JOIN_DBR,
	DZ_GUILD_JOIN_ACK,
	DZ_GUILD_JOIN_NAK,

	DZ_GUILD_WITHDRAW_SYN,
	DZ_GUILD_WITHDRAW_DBR,
	DZ_GUILD_WITHDRAW_ACK,
	DZ_GUILD_WITHDRAW_NAK,

	DZ_GUILD_KICKPLAYER_SYN,
	DZ_GUILD_KICKPLAYER_DBR,
	DZ_GUILD_KICKPLAYER_ACK,
	DZ_GUILD_KICKPLAYER_NAK,

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	DZ_GUILD_CHUNTING_PVP_RESULT_SYN,
	DZ_GUILD_CHUNTING_PVP_RESULT_DBR,
	DZ_GUILD_CHUNTING_PVP_RESULT_ACK,
#endif

	DZ_GUILD_CHANGE_DUTY_SYN,
	DZ_GUILD_CHANGE_DUTY_DBR,
	DZ_GUILD_CHANGE_DUTY_ACK,
	DZ_GUILD_CHANGE_DUTY_NAK,

	DZ_GUILD_RIGHT_DELEGATION_SYN,
	DZ_GUILD_RIGHT_DELEGATION_DBR,
	DZ_GUILD_RIGHT_DELEGATION_ACK,
	DZ_GUILD_RIGHT_DELEGATION_NAK,

	DZ_GUILD_CHANGE_MASTER_SYN,
	DZ_GUILD_CHANGE_MASTER_DBR,
	DZ_GUILD_CHANGE_MASTER_ACK,
	DZ_GUILD_CHANGE_MASTER_NAK,

	DZ_GUILD_CREATE_CORPS_SYN,
	DZ_GUILD_CREATE_CORPS_DBR,
	DZ_GUILD_CREATE_CORPS_ACK,
	DZ_GUILD_CREATE_CORPS_NAK,

	DZ_GUILD_DESTROY_CORPS_SYN,
	DZ_GUILD_DESTROY_CORPS_DBR,
	DZ_GUILD_DESTROY_CORPS_ACK,
	DZ_GUILD_DESTROY_CORPS_NAK,

	DZ_GUILD_CHANGE_COMMANDER_SYN,
	DZ_GUILD_CHANGE_COMMANDER_DBR,
	DZ_GUILD_CHANGE_COMMANDER_ACK,
	DZ_GUILD_CHANGE_COMMANDER_NAK,

	DZ_GUILD_CHANGE_CORPS_SYN,
	DZ_GUILD_CHANGE_CORPS_DBR,
	DZ_GUILD_CHANGE_CORPS_ACK,
	DZ_GUILD_CHANGE_CORPS_NAK,

	DZ_GUILD_CREATE_REAR_CAMP_SYN,
	DZ_GUILD_CREATE_REAR_CAMP_DBR,
	DZ_GUILD_CREATE_REAR_CAMP_ACK,
	DZ_GUILD_CREATE_REAR_CAMP_NAK,

	DZ_GUILD_DESTROY_REAR_CAMP_SYN,
	DZ_GUILD_DESTROY_REAR_CAMP_DBR,
	DZ_GUILD_DESTROY_REAR_CAMP_ACK,
	DZ_GUILD_DESTROY_REAR_CAMP_NAK,

	DZ_GUILD_LOGOUT_CHAR_CMD,

	DZ_GUILD_SELECT_CORPS_DBR,
	DZ_GUILD_SELECT_CORPS_NAK,

	DZ_GUILD_GM_GUILD_POINT_SYN,
	DZ_GUILD_GM_GUILD_POINT_DBR,
	DZ_GUILD_GM_GUILD_POINT_ACK,
	DZ_GUILD_GM_GUILD_POINT_NAK,

	DZ_GUILD_RELATION_SYN,
	DZ_GUILD_RELATION_DBR,
	DZ_GUILD_RELATION_ACK,
	DZ_GUILD_RELATION_NAK,	

	DZ_GUILD_RELATION_DELETE_SYN,
	DZ_GUILD_RELATION_DELETE_DBR,
	DZ_GUILD_RELATION_DELETE_ACK,
	DZ_GUILD_RELATION_DELETE_NAK,

	DZ_GUILD_RELATION_BOTH_AGAINST_SYN,	
	DZ_GUILD_RELATION_BOTH_AGAINST_ACK,
	DZ_GUILD_RELATION_BOTH_AGAINST_NAK,
	DZ_GUILD_RELATION_BOTH_AGAINST_DBR,

    DZ_GUILD_RELATION_INFO_SYN,
	DZ_GUILD_RELATION_INFO_ACK,
	DZ_GUILD_RELATION_INFO_NAK,
	DZ_GUILD_SELECT_RELATION_DBR,

	DZ_GUILD_DESTROY_COMPLETE_SYN,
	DZ_GUILD_DESTROY_COMPLETE_DBR,
	DZ_GUILD_DESTROY_COMPLETE_ACK,
	DZ_GUILD_DESTROY_COMPLETE_NAK,	

	DZ_GUILD_RECOVERY_GUILD_SYN,
	DZ_GUILD_RECOVERY_GUILD_ACK,
	DZ_GUILD_RECOVERY_GUILD_NAK,
	DZ_GUILD_RECOVERY_GUILD_DBR,

    DZ_GUILD_GUILDMARK_REGISTER_SYN,
	DZ_GUILD_GUILDMARK_REGISTER_ACK,
	DZ_GUILD_GUILDMARK_REGISTER_NAK,
	DZ_GUILD_GUILDMARK_REGISTER_DBR,

	DZ_GUILD_GUILDMARK_DELETE_SYN,
	DZ_GUILD_GUILDMARK_DELETE_ACK,
	DZ_GUILD_GUILDMARK_DELETE_NAK,
	DZ_GUILD_GUILDMARK_DELETE_DBR,

    DZ_GUILD_WAREHOUSE_OPEN_SYN,
	DZ_GUILD_WAREHOUSE_OPEN_ACK,
	DZ_GUILD_WAREHOUSE_OPEN_NAK,
	DZ_GUILD_WAREHOUSE_OPEN_DBR,

	DZ_GUILD_WAREHOUSE_CLOSE_SYN,
	DZ_GUILD_WAREHOUSE_CLOSE_ACK,
	DZ_GUILD_WAREHOUSE_CLOSE_NAK,

	DZ_GUILD_WAREHOUSE_MONEY_SYN,
	DZ_GUILD_WAREHOUSE_MONEY_ACK,
	DZ_GUILD_WAREHOUSE_MONEY_NAK,

	DZ_GUILD_WAREHOUSE_DBSAVE_SYN,
	DZ_GUILD_WAREHOUSE_DBSAVE_DBR,
	DZ_GUILD_WAREHOUSE_DBSAVE_ACK,
	DZ_GUILD_WAREHOUSE_LOG_DBSAVE_SYN,
	DZ_GUILD_WAREHOUSE_LOG_DBSAVE_DBR,

	DZ_GUILD_NAME_CHANGE_SYN,
	DZ_GUILD_NAME_CHANGE_ACK,
	DZ_GUILD_NAME_CHANGE_DBR,
	DZ_GUILD_NAME_CHANGE_NAK,

    //{ __KR_001385_20090907_DOMINATION_PACKET
    DZ_DOMINATION_INFO_SYN,
    DZ_DOMINATION_INFO_ACK,
    DZ_DOMINATION_INFO_DBR,

    DZ_DOMINATION_AUCTION_INFO_SYN,
    DZ_DOMINATION_AUCTION_INFO_ACK,
    DZ_DOMINATION_AUCTION_INFO_DBR,

    DZ_DOMINATION_UPDATE_INFO_SYN,
    DZ_DOMINATION_UPDATE_INFO_ACK,
    DZ_DOMINATION_UPDATE_INFO_DBR,

    DZ_DOMINATION_ACCEP_HEIM_SYN,
    DZ_DOMINATION_ACCEP_HEIM_ACK,
    DZ_DOMINATION_ACCEP_HEIM_DBR,

    DZ_DOMINATION_INIT_AUCTIONINFO_SYN,
    DZ_DOMINATION_INIT_AUCTIONINFO_ACK,
    DZ_DOMINATION_INIT_AUCTIONINFO_DBR,

    DZ_DOMINATION_UPDATE_RETURNHEIM_DBR,
    DZ_DOMINATION_DELETE_RETURNHEIM_DBR,
    //}
    //_NA_20100604_BETTER_DOMINATION_MANAGER
    DZ_DOMINATION_UPDATE_GUILDINFO_SYN,
    DZ_DOMINATION_UPDATE_GUILDINFO_ACK,
    DZ_DOMINATION_UPDATE_GUILDINFO_DBR,

    // _NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
    DZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD,              // GM ���ɿ� ���� ��� â�� ���� �ݱ�
    DZ_GUILD_WAREHOUSE_ITEMSLOTINFO_SELECT_INTER_DBR,

    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN,           // ������ �溮 ����Ʈ ���� ����
    DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK,
    DZ_DOMINATION_ADD_DEFFENSEWALL_POINT_DBR,

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    DZ_DOMINATION_MEMBER_JOIN_SYN,                      // ������ ��� ��û
    DZ_DOMINATION_MEMBER_JOIN_ACK,
    DZ_DOMINATION_MEMBER_JOIN_DBR,

    DZ_DOMINATION_MEMBER_LEAVE_SYN,                     // ������ ��� ��û ���
    DZ_DOMINATION_MEMBER_LEAVE_ACK,
    DZ_DOMINATION_MEMBER_LEAVE_DBR,

    DZ_DOMINATION_MEMBERLIST_REQ,                       // ������ ��� ����Ʈ ��û
    DZ_DOMINATION_MEMBERLIST_ANS,
    DZ_DOMINATION_MEMBERLIST_DBR,

    DZ_DOMINATION_MEMBERLIST_INIT_SYN,                  // ����Ʈ �ʱ�ȭ ��û
    DZ_DOMINATION_MEMBERLIST_INIT_ACK,
    DZ_DOMINATION_MEMBERLIST_INIT_DBR,

    //!~ _NA004034_20120102_GUILD_POINT_SYSTEM
    DZ_GUILDPOINT_SYSTEM,
    DZ_GUILDPOINT_SYSTEM_DBR,

    DZ_GUILD_INTRODUCTION_SAVE_SYN,
    DZ_GUILD_INTRODUCTION_SAVE_DBR,

    DZ_GUILD_INTRODUCTION_LOAD_SYN,
    DZ_GUILD_INTRODUCTION_LOAD_DBR,
    //~! _NA004034_20120102_GUILD_POINT_SYSTEM

    //_NA_003923_20120130_GUILD_RENEWAL
    DZ_GUILD_WRITE_IN_GAME_LOG_SYN,
    DZ_GUILD_WRITE_IN_GAME_LOG_ACK,
    DZ_GUILD_WRITE_IN_GAME_LOG_DBR,

    DZ_GUILD_IN_GAME_LOG_LIST_REQ,
    DZ_GUILD_IN_GAME_LOG_LIST_ANS,
    DZ_GUILD_IN_GAME_LOG_LIST_DBR,

    //!~ _NA_000000_20130429_DOMINATION_PENALTY
    DZ_DOMINATION_PENALTY_CMD,
    DZ_DOMINATION_PENALTY_DBR,
    //~! _NA_000000_20130429_DOMINATION_PENALTY

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    DZ_CONTINENT_LORD_INFO_SYN,
    DZ_CONTINENT_LORD_INFO_DBR,
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    DZ_GUILD_JOIN_REQUEST_INSERT_SYN,
    DZ_GUILD_JOIN_REQUEST_INSERT_DBR,

    DZ_GUILD_JOIN_REQUEST_DELETE_SYN,
    DZ_GUILD_JOIN_REQUEST_DELETE_DBR,

    DZ_GUILD_JOIN_REQUEST_SELECT_SYN,
    DZ_GUILD_JOIN_REQUEST_SELECT_DBR,

    DZ_GUILD_FACILITY_UPDATE_SYN,
    DZ_GUILD_FACILITY_UPDATE_DBR,

    DZ_GUILD_FACILITY_SELECT_SYN,
    DZ_GUILD_FACILITY_SELECT_DBR,

    DZ_GUILD_FACILITY_INIT_SYN,
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    //_NA_008334_20150608_SONNENSCHEIN
    DZ_GUILD_SONNENSCHEIN_INITIALIZE_SYN,
    DZ_GUILD_SONNENSCHEIN_INITIALIZE_DBR,
    DZ_GUILD_SONNENSCHEIN_INITIALIZE_ACK,

    DZ_GUILD_SONNENSCHEIN_UPDATE_SYN,
    DZ_GUILD_SONNENSCHEIN_UPDATE_DBR,
    DZ_GUILD_SONNENSCHEIN_UPDATE_ACK,
}; //end of 'eDZ_GUILD_PROTOCOL'

/// ��������
enum eGZ_GUILD_PROTOCOL
{
	GZ_GUILD_INFO_BRD,			//< ��弭������ ���Ӽ����� broadcast

	GZ_GUILD_SELECT_SYN,
	GZ_GUILD_SELECT_ACK,
	GZ_GUILD_SELECT_NAK,

	GZ_GUILD_JOIN_SYN,
	GZ_GUILD_JOIN_BRD,
	GZ_GUILD_JOIN_NAK,

	GZ_GUILD_CREATE_SYN,
	GZ_GUILD_CREATE_ACK,
	GZ_GUILD_CREATE_NAK,

	GZ_GUILD_DESTROY_SYN,
	GZ_GUILD_DESTROY_BRD,
	GZ_GUILD_DESTROY_NAK,

	GZ_GUILD_WITHDRAW_SYN,
	GZ_GUILD_WITHDRAW_BRD,
	GZ_GUILD_WITHDRAW_NAK,

	GZ_GUILD_DONATE_UP_SYN,			//< ��弭���� UP �峳
	GZ_GUILD_DONATE_UP_BRD,
	GZ_GUILD_DONATE_UP_NAK,

	GZ_GUILD_CONVERT_UP2GP_SYN,		//< UP->GP ��ȯ
	GZ_GUILD_CONVERT_UP2GP_BRD,
	GZ_GUILD_CONVERT_UP2GP_NAK,
	
	GZ_GUILD_RANKUP_BRD,
	

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
	GZ_GUILD_CHUNTING_PVP_RESULT_CMD,
	GZ_GUILD_CHUNTING_PVP_RESULT_BRD,
#endif

	GZ_GUILD_CREATE_CORPS_SYN,
	GZ_GUILD_CREATE_CORPS_BRD,
	GZ_GUILD_CREATE_CORPS_NAK,

	GZ_GUILD_DESTROY_CORPS_SYN,
	GZ_GUILD_DESTROY_CORPS_BRD,
	GZ_GUILD_DESTROY_CORPS_NAK,

	GZ_GUILD_EMPTY_COMMANDER_INFO_SYN,
	GZ_GUILD_EMPTY_COMMANDER_INFO_ACK,
	GZ_GUILD_EMPTY_COMMANDER_INFO_NAK,

	GZ_GUILD_CHANGE_COMMANDER_SYN,
	GZ_GUILD_CHANGE_COMMANDER_BRD,
	GZ_GUILD_CHANGE_COMMANDER_NAK,

	GZ_GUILD_CHANGE_CORPS_SYN,
	GZ_GUILD_CHANGE_CORPS_BRD,
	GZ_GUILD_CHANGE_CORPS_NAK,

	GZ_GUILD_CREATE_REAR_CAMP_SYN,
	GZ_GUILD_CREATE_REAR_CAMP_BRD,
	GZ_GUILD_CREATE_REAR_CAMP_NAK,

	GZ_GUILD_DESTROY_REAR_CAMP_SYN,
	GZ_GUILD_DESTROY_REAR_CAMP_BRD,
	GZ_GUILD_DESTROY_REAR_CAMP_NAK,

	GZ_GUILD_INVITE_SYN,
	GZ_GUILD_INVITE_ACK,
	GZ_GUILD_INVITE_NAK,

	GZ_GUILD_LOGIN_CHAR_SYN,
	GZ_GUILD_LOGIN_CHAR_BRD,
	GZ_GUILD_LOGIN_CHAR_NAK,

	GZ_GUILD_LOGOUT_CHAR_SYN,		
	GZ_GUILD_LOGOUT_CHAR_BRD,		

	GZ_GUILD_CHAR_LEVELUP_SYN,
	GZ_GUILD_CHAR_LEVELUP_BRD,

	GZ_GUILD_KICKPLAYER_SYN,
	GZ_GUILD_KICKPLAYER_NAK,
	GZ_GUILD_KICKPLAYER_BRD,

	GZ_GUILD_TOTALINFO_REQ_SYN,
	GZ_GUILD_TOTALINFO_REQ_ACK,
	GZ_GUILD_TOTALINFO_REQ_NAK,

	GZ_GUILD_GM_GUILD_POINT_SYN,			// GM���ɾ� �������Ʈ ����
	GZ_GUILD_GM_GUILD_POINT_BRD,
	GZ_GUILD_GM_GUILD_POINT_NAK,

	GZ_GUILD_CHANGE_CHAOSTATE_SYN,			//< ���� ī�� ���� ����
	GZ_GUILD_CHANGE_CHAOSTATE_BRD,			
	GZ_GUILD_CHANGE_CHAOSTATE_NAK,	

	GZ_GUILD_ENTER_SERVER_SYN,				// ������ �������� �̵��� ��.
	GZ_GUILD_ENTER_SERVER_ACK,				
	GZ_GUILD_ENTER_SERVER_NAK,		

	GZ_GUILD_PRE_LOGIN_CMD,					// ���� ���� �α���.

	GZ_GUILD_RELATION_SYN,			// ��� ����
	GZ_GUILD_RELATION_BRD,
	GZ_GUILD_RELATION_NAK,
	GZ_GUILD_BOTH_RELATION_SYN,
	GZ_GUILD_BOTH_RELATION_ACK,

	GZ_GUILD_RELATION_DELETE_SYN,	// ��� ���� ����
	GZ_GUILD_RELATION_DELETE_BRD,
	GZ_GUILD_RELATION_DELETE_NAK,

	GZ_GUILD_RELATION_BOTH_AGAINST_SYN,
	GZ_GUILD_RELATION_BOTH_AGAINST_BRD,
	GZ_GUILD_RELATION_BOTH_AGAINST_NAK,

    GZ_GUILD_RELATION_INFO_BRD,
	GZ_GUILD_RELATION_INFO_SYN,

    GZ_GUILD_CHANGE_DUTY_SYN,
	GZ_GUILD_CHANGE_DUTY_BRD,
	GZ_GUILD_CHANGE_DUTY_NAK,

	GZ_GUILD_RIGHT_DELEGATION_LIST_SYN,
	GZ_GUILD_RIGHT_DELEGATION_LIST_ACK,
	GZ_GUILD_RIGHT_DELEGATION_LIST_NAK,

	GZ_GUILD_RIGHT_DELEGATION_SYN,
	GZ_GUILD_RIGHT_DELEGATION_BRD,
	GZ_GUILD_RIGHT_DELEGATION_NAK,

	GZ_GUILD_CHANGE_MASTER_SYN,
	GZ_GUILD_CHANGE_MASTER_BRD,
	GZ_GUILD_CHANGE_MASTER_NAK,

	GZ_GUILD_DESTROY_WAIT_BRD,
	GZ_GUILD_RECOVERY_GUILD_SYN,
	GZ_GUILD_RECOVERY_GUILD_BRD,
	GZ_GUILD_RECOVERY_GUILD_NAK,

    GZ_GUILD_GUILDMARK_REGISTER_SYN,
	GZ_GUILD_GUILDMARK_REGISTER_BRD,
	GZ_GUILD_GUILDMARK_REGISTER_NAK,

	GZ_GUILD_GUILDMARK_DELETE_SYN,
	GZ_GUILD_GUILDMARK_DELETE_BRD,
	GZ_GUILD_GUILDMARK_DELETE_NAK,

    GZ_GUILD_WAREHOUSE_OPEN_SYN,
	GZ_GUILD_WAREHOUSE_OPEN_BRD,
	GZ_GUILD_WAREHOUSE_OPEN_ACK,
	GZ_GUILD_WAREHOUSE_OPEN_NAK,

	GZ_GUILD_WAREHOUSE_CLOSE_SYN,
	GZ_GUILD_WAREHOUSE_CLOSE_BRD,
	GZ_GUILD_WAREHOUSE_CLOSE_NAK,

	GZ_GUILD_WAREHOUSE_MONEY_SYN,
	GZ_GUILD_WAREHOUSE_MONEY_BRD,
	GZ_GUILD_WAREHOUSE_MONEY_NAK,

	GZ_GUILD_WAREHOUSE_DBSAVE_SYN,
	GZ_GUILD_WAREHOUSE_LOG_DBSAVE_SYN,

    // __NA000000_20081106_GUILD_NAME_CHANGE
	GZ_GUILD_NAME_CHANGE_SYN,
	GZ_GUILD_NAME_CHANGE_ACK,
	GZ_GUILD_NAME_CHANGE_BRD,
	GZ_GUILD_NAME_CHANGE_NAK,

    //_KR_001385_20091013_DOMINATION_BASICINFO_SETTING
    GZ_DOMINATION_INFO_SYN,             //���� ������ �⺻ ������ ���Ӽ����� ������.
    GZ_DOMINATION_INFO_ACK,
    GZ_DOMINATION_AUCTION_INFO_SYN,      //���� ������ ��� ������ ���� ������ ������.
    GZ_DOMINATION_AUCTION_INFO_ACK,
    GZ_DOMINATION_UPDATE_INFO_SYN,      //���� ������ �⺻ ������ Update�Ѵ�.
    GZ_DOMINATION_UPDATE_INFO_ACK,
    GZ_DOMINATION_ACCEP_HEIM_SYN,       //��ſ� ���������� DB�� ����.
    GZ_DOMINATION_ACCEP_HEIM_ACK,
    GZ_DOMINATION_INIT_ACTIONINFO_SYN,  //��� ������ ��� �ʱ�ȭ �Ѵ�.
    GZ_DOMINATION_INIT_ACTIONINFO_ACK,
    GZ_DOMINATION_RETURNHIME_SYN,       //��� �ݾ��� �����ش�.
    GZ_DOMINATION_RETURNHIME_ACK,
    GZ_DOMINATION_BATTING_HEIM_SYN,     //������ ���� �Ѵ�.
    GZ_DOMINATION_BATTING_HEIM_ACK,

    //_NA_20100604_BETTER_DOMINATION_MANAGER
    GZ_DOMINATION_UPDATE_GUILDINFO_SYN, //��������� ���� ������ ���� ����
    GZ_DOMINATION_UPDATE_GUILDINFO_ACK,

    // _NA_20100913_GUILD_WAREHOUSE_GM_COMMAND
    GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_CMD,          // GM ���ɿ� ���� ��� â�� ���� �ݱ�
    GZ_GUILD_GM_WAREHOUSE_FORCE_CLOSE_BRD,          // GM ���ɿ� ���� ��� â�� ���� �ݱ� ����

    //_NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_SYN,       // �溮 ����Ʈ ���� ��û
    GZ_DOMINATION_ADD_DEFFENSEWALL_POINT_ACK,

    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    GZ_DOMINATION_MEMBER_JOIN_SYN,                  // ������ �ɹ� ��û
    GZ_DOMINATION_MEMBER_JOIN_ACK,
    GZ_DOMINATION_MEMBER_LEAVE_SYN,                 // ������ �ɹ� ��û ���
    GZ_DOMINATION_MEMBER_LEAVE_ACK,
    GZ_DOMINATION_MEMBERLIST_REQ,                   // ������ �ɹ�����Ʈ ��û
    GZ_DOMINATION_MEMBERLIST_ANS,
    GZ_DOMINATION_MEMBERLIST_INIT_CMD,

    //_NA004034_20120102_GUILD_POINT_SYSTEM
    GZ_GUILDPOINTSYSTEM,

    //_NA_003923_20120130_GUILD_RENEWAL
    GZ_GUILD_IN_GAME_GUILD_LOG_LIST_REQ,
    GZ_GUILD_IN_GAME_GUILD_LOG_LIST_ANS,

    //_NA_004034_20120102_GUILD_INTRODUCTION
    GZ_GUILD_INTRODUCTION_SAVE_SYN,
    GZ_GUILD_INTRODUCTION_SAVE_ACK,

    GZ_GUILD_RENAME_CHAR_CMD,

    //_NA_000000_20130429_DOMINATION_PENALTY
    GZ_DOMINATION_PENALTY_CMD,

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    GZ_DOMINATION_CONTINENT_LORD_INFO_ACK,  //��� ���� ����
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //���� ��û ��� ����
    GZ_GUILD_JOIN_REQUEST_SYN,
    GZ_GUILD_JOIN_REQUEST_ACK,

    //���� ��û �ź�
    GZ_GUILD_JOIN_REQUEST_REJECT_SYN,
    GZ_GUILD_JOIN_REQUEST_REJECT_ACK,

    //��� ���� ���� ���ε�
    GZ_GUILD_RELOAD_SYN,
    GZ_GUILD_RELOAD_ACK,

    //��� �ü� ���׷��̵� ��û
    GZ_GUILD_FACILITY_UPGRADE_SYN,
    GZ_GUILD_FACILITY_UPGRADE_ACK,

    //��� �ü� ���� ��û
    GZ_GUILD_FACILITY_SELECT_SYN,
    GZ_GUILD_FACILITY_SELECT_ACK,
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    //_NA_008334_20150608_SONNENSCHEIN
    GZ_GUILD_SONNENSCHEIN_INITIALIZE_CMD,

    GZ_GUILD_SONNENSCHEIN_UPDATE_SYN,
    GZ_GUILD_SONNENSCHEIN_UPDATE_ACK,
    
}; //end of 'eGZ_GUILD_PROTOCOL'

enum eDG_HONORSYSTEM
{
    DG_HONORSYSTEM_TITLES_SELECT_SYN,
    DG_HONORSYSTEM_TITLES_SELECT_ACK,

    DG_HONORSYSTEM_TITLES_ADD_SYN,
    DG_HONORSYSTEM_TITLES_ADD_ACK = DG_HONORSYSTEM_TITLES_ADD_SYN,
    DG_HONORSYSTEM_TITLES_REMOVE_SYN,
    DG_HONORSYSTEM_TITLES_REMOVE_ACK = DG_HONORSYSTEM_TITLES_REMOVE_SYN,

    DG_HONORSYSTEM_FAME_REPUTE_SELECT_SYN,
    DG_HONORSYSTEM_FAME_REPUTE_SELECT_ACK,
    DG_HONORSYSTEM_FAME_REPUTE_CHANGE_CMD,

    DG_HONORSYSTEM_DAILYQUEST_SELECT_SYN,
    DG_HONORSYSTEM_DAILYQUEST_SELECT_ACK,
    DG_HONORSYSTEM_DAILYQUEST_UPDATE_SYN,
};

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
enum eDG_SUN_RANKING_PROTOCOL
{
    DG_SUN_RANKING_UPDATE_CMD,
    DG_SUN_RANKING_UPDATE_DBR,
    DG_SUN_RANKING_UPDATE_ACK,

    DG_SUN_RANKING_SELECT_SYN,
    DG_SUN_RANKING_SELECT_DBR,
    DG_SUN_RANKING_SELECT_ACK,

    DG_SUN_RANKING_INIT_DBR,
    DG_SUN_RANKING_GRADE_INSERT_DBR,
    DG_SUN_RANKING_GRADE_INIT_DBR,

    DG_SUN_RANKING_GRADE_SELECT_SYN,
    DG_SUN_RANKING_GRADE_SELECT_DBR,
    DG_SUN_RANKING_GRADE_SELECT_ACK,

    DG_STATUE_ITEM_SLOT_INFO_INSERT_SYN,
    DG_STATUE_ITEM_SLOT_INFO_INSERT_DBR,
    DG_STATUE_ITEM_SLOT_INFO_INSERT_ACK,

    DG_STATUE_SELECT_SYN,
    DG_STATUE_SELECT_ACK,

    DG_STATUE_ITEM_SLOT_INFO_SELECT_SYN,
    DG_STATUE_ITEM_SLOT_INFO_SELECT_DBR,

    DG_STATUE_CHAR_INSERT_SYN,
    DG_STATUE_CHAR_INSERT_DBR,

    DG_STATUE_CHAR_SELECT_SYN,
    DG_STATUE_CHAR_SELECT_DBR,

    DG_SUN_RANKING_SETTLE_START_SYN,
    DG_SUN_RANKING_SETTLE_START_CMD,

    DG_SUN_RANKING_SETTLE_COMPLETE_SYN,
    DG_SUN_RANKING_SETTLE_COMPLETE_CMD,

    DG_SUN_RANKING_SETTLE_SYN,

    DG_SUN_RANKING_SELECT_COMPLETE_CMD,
    DG_SUN_RANKING_GRADE_SELECT_COMPLETE_CMD,
};
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
enum eDG_NOTIFICATION_PROTOCOL
{
    DG_NOTIFICATION_ITEM_SYN,
    DG_NOTIFICATION_CLEAR_SYN,
};
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

#endif // __PROTOCOL_DG_H__

