#ifndef _GameConst_h_
#define _GameConst_h_

/**
 *	@file		GameConst.h
 *	@brief		Ŭ���̾�Ʈ������ ����ϴ� ENUM ���� ��� ���°�.
 *  @author		���翵, ��â��
 *	@version	[07/14/2005] first release
 *	@version	[09/07/2005] ��ų ���� �÷����� �߰�
 */

//------------------------------------------------------------------------------
///**
//	Ŭ���̾�Ʈ������ ����ϴ� ENUM ���� ��� ���°�.
//    @author
//	    ���翵, ��â��
//    @since
//        [09/07/2005] ��ų ���� �÷����� �߰�
//    	[07/14/2005]
//    @remarks
//*/

#define INVALID_OBJECT_KEY              0xFFFFFFFF
#define INVALID_CLASS_CODE              0xFFFFFFFF
#define INVALID_SKILL_CLASS_CODE        0
#define INVALID_SKILL_CODE              0

//-----------------------------------------------------------------------------
// �������� ����
//-----------------------------------------------------------------------------
// �������� ��ų �÷�
const DWORD c_dwTOOLTIP_INFO_NAME                   = 0x00000001;
const DWORD c_dwTOOLTIP_INFO_NAME_DISABLE           = 0x00000002;
const DWORD c_dwTOOLTIP_INFO_REQUIRED_SKILLSTAT0    = 0x00000004;
const DWORD c_dwTOOLTIP_INFO_REQUIRED_SKILLSTAT1    = 0x00000008;
const DWORD c_dwTOOLTIP_INFO_REQUIRED_SKILLPOINT    = 0x00000010;
const DWORD c_dwTOOLTIP_INFO_NEXT_SKILLSTAT0        = 0x00000020;
const DWORD c_dwTOOLTIP_INFO_NEXT_SKILLSTAT1        = 0x00000040;
const DWORD c_dwTOOLTIP_INFO_NEXT_SKILLPOINT        = 0x00000080;
const DWORD c_dwTOOLTIP_INFO_EXT_EFFECT0            = 0x00000100;
const DWORD c_dwTOOLTIP_INFO_EXT_EFFECT1            = 0x00000200;
const DWORD c_dwTOOLTIP_INFO_DESC0                  = 0x00000400;
const DWORD c_dwTOOLTIP_INFO_DESC1                  = 0x00000800;
const DWORD c_dwTOOLTIP_INFO_REQUIRED_SKILLLEVEL    = 0x00001000;
const DWORD c_dwTOOLTIP_INFO_NEXT_SKILLLEVEL        = 0x00002000;
const DWORD c_dwTOOLTIP_INFO_MAX_SKILLLEVEL         = 0x00004000;
const DWORD c_dwTOOLTIP_INFO_SKILL_QUICKSLOT        = 0x00008000;
const DWORD c_dwTOOLTIP_INFO_ETC_SKILL		        = 0x00010000;	// �׼�,�̸�Ƽ�� ����

// �������� ��Ÿ�� �÷�
const DWORD c_dwTOOLTIP_STYLE_INFO_NAME                 = 0x00000001;
const DWORD c_dwTOOLTIP_STYLE_INFO_NAME_DISABLE         = 0x00000002;
const DWORD c_dwTOOLTIP_STYLE_INFO_REQUIRED_LEVEL       = 0x00000004;
const DWORD c_dwTOOLTIP_STYLE_INFO_REQUIRED_SKILLPOINT  = 0x00000008;
const DWORD c_dwTOOLTIP_STYLE_INFO_NEXT_LEVEL           = 0x00000010;
const DWORD c_dwTOOLTIP_STYLE_INFO_NEXT_SKILLPOINT      = 0x00000020;
const DWORD c_dwTOOLTIP_STYLE_INFO_EXT_EFFECT0          = 0x00000040;
const DWORD c_dwTOOLTIP_STYLE_INFO_EXT_EFFECT1          = 0x00000080;
const DWORD c_dwTOOLTIP_STYLE_INFO_DESC0                = 0x00000100;
const DWORD c_dwTOOLTIP_STYLE_INFO_DESC1                = 0x00000200;
const DWORD c_dwTOOLTIP_STYLE_INFO_QUICKSLOT            = 0x00000400;

// �������� ������ �÷�
const DWORD c_dwTOOLTIP_ITEM_INFO_NAME                      = 0x00000001;   // �̸�
const DWORD c_dwTOOLTIP_ITEM_INFO_TYPE                      = 0x00000002;   // ����
const DWORD c_dwTOOLTIP_ITEM_INFO_RANK						= 0x00000004;	// ��ũ
const DWORD c_dwTOOLTIP_ITEM_INFO_SOCKET					= 0x00000008;	// ����
const DWORD c_dwTOOLTIP_ITEM_INFO_ADDITIONAL_INFO           = 0x00000010;   // �߰� ����
const DWORD c_dwTOOLTIP_ITEM_INFO_ADINFO_PRICE              = 0x00000020;   // ����
const DWORD c_dwTOOLTIP_ITEM_INFO_ADINFO_PRICE_USED         = 0x00000040;   // �߰� ����
const DWORD c_dwTOOLTIP_ITEM_INFO_EQCLASS					= 0x00000080;	// Ŭ���� ����
const DWORD c_dwTOOLTIP_ITEM_INFO_PHY_ATTSPEED				= 0x00000100;	// ���� �ӵ�
const DWORD c_dwTOOLTIP_ITEM_INFO_DAMAGE					= 0x00000200;	// ���� ���ݷ�
const DWORD c_dwTOOLTIP_ITEM_INFO_MAGIC_DAMAGE				= 0x00000400;	// ���� ���ݷ�
const DWORD c_dwTOOLTIP_ITEM_INFO_DURABILITY				= 0x00000800;	// ������
const DWORD c_dwTOOLTIP_ITEM_INFO_DURABLE                   = 0x00001000;   // ����
const DWORD c_dwTOOLTIP_ITEM_INFO_PHY_DEFENSE				= 0x00002000;	// ���� ����
const DWORD c_dwTOOLTIP_ITEM_INFO_EQUIP_LIMIT				= 0x00004000;	// ������ ���� ����
const DWORD c_dwTOOLTIP_ITEM_INFO_OPTION					= 0x00008000;	// �ɼ�

// �������� �߰� ó�� ��Ÿ��
const DWORD c_dwTOOLTIP_EXTRA_INFO_DONOT_CHANGE_LINE		  = 0x80000000;   // ������ ����� ����� ���� ���ο� ����
const DWORD c_dwTOOLTIP_EXTRA_INFO_DT_LEFT					  = 0x40000000;   // �ý�Ʈ ���� ����
const DWORD c_dwTOOLTIP_EXTRA_INFO_DT_CENTER				  = 0x20000000;   // �ý�Ʈ ��� ����
const DWORD c_dwTOOLTIP_EXTRA_INFO_DT_RIGHT					  = 0x10000000;   // �ý�Ʈ ������ ����

const DWORD c_dwTOOLTIP_EXTRA_CELL1							  = 0x01000000;   // �� left +100px
const DWORD c_dwTOOLTIP_EXTRA_CELL2						      = 0x02000000;   // �� 1eft +125px
const DWORD c_dwTOOLTIP_EXTRA_CELL3							  = 0x04000000;   // �� left +150px

const DWORD c_dwTOOLTIP_EXTRA_INFO_FIXED_IMAGE		  = 0x00100000;   // ���� ��ǥ���� ��������� �׸� ��ǥ ����, ������ ũ�⿡ ������ ���� ����
const DWORD c_dwTOOLTIP_EXTRA_INFO_ABSOLUTE_IMAGE	  = 0x00200000;   // �׸� ��ǥ ���������� ����, ������ ũ�⿡ ������ ���� ����
const DWORD c_dwTOOLTIP_EXTRA_INFO_HANDLE_IMAGE	      = 0x00400000;   // �׸��� �ؽ����� ���ҽ� �ڵ�� ���´�.

enum LimitStatCompositeType
{
    kLimitStatCompositeNormal = 0,                //!< 
    kLimitStatCompositeRankup = 1,                //!< ��ũ��
    kLimitStatCompositeEnchant = 2,               //!< ����Ʈ
    kLimitStatCompositeItemPurchased = 100,     //< ������ �籸��
    kItemPurchasedByItem = 102, // ȭ��(������) ����
    kItemPurchasedByPoint = 103, // ���� ����Ʈ ����
    kItemPurchasedByCurrencyPoint = 104, // ȭ�� ����Ʈ ����:_NA004034_20120102_POINT_WALLET_SYSTEM
#ifdef _NA_002935_20110704_ITEM_SCORE
    kItemOtherEquipPcRoomEnable = 110, // �ٸ� ���� �������� â���� ������ �׸� �� ������ �� ����(param�� �뵵�� ���)
    kItemOtherEquipPcRoomDisable = 111, // RegisterTooltipItem()�� 99% ��ġ�ϴ� �Լ��� ������ ����� �����ϱ� ����
#endif //_NA_002935_20110704_ITEM_SCORE
};

//-----------------------------------------------------------------------------
//		Pack ������ ����
//-----------------------------------------------------------------------------

enum ePACK_DATA_FILE
{
	ePACK_SCRIPT_FILE			= 0,

	ePACK_NUMBER_OF_FILE
};

enum ePACK_FILE_NAME
{
	ePFN_NONE					= 0,
	ePFN_NOT_USE_PACKFILE		= 1,	

	ePFN_RESOURCE				= 8,
	ePFN_SYSTEM					= 9,

	ePFN_MAX,
};

//-----------------------------------------------------------------------------
// PVP ����
//-----------------------------------------------------------------------------
enum ePVP_MATCH_TYPE
{
	ePVP_MATCH_PRIVATE = 0,
	ePVP_MATCH_TEAM,
};

enum ePVPINFO_SORT_TYPE
{
	ePVPINFO_SORT_KILL_COUNT = 0,
	ePVPINFO_SORT_DEAD_COUNT,
	ePVPINFO_SORT_STRAIGHT_KILL,
	ePVPINFO_SORT_BEST_STRAIGHT_KILL,
	ePVPINFO_SORT_NAME,
};

//-----------------------------------------------------------------------------
// �����۰���
//-----------------------------------------------------------------------------

enum eNormalEnchantType
{
	LOW_ENCHANT,
	MIDDLE_ENCHANT,
};

//-----------------------------------------------------------------------------
// �������̽� �ɼ� ����
//-----------------------------------------------------------------------------
enum eOPTION_LEVEL
{
	eOPTION_LEVEL_HIGH			= 0,
	eOPTION_LEVEL_MIDDLE		= 1,
	eOPTION_LEVEL_LOW			= 2,

	eOPTION_LEVEL_MAX,
};

const float cEFFECT_DIST_WEIGHT_HIGH = 3.0f;
const float cEFFECT_DIST_WEIGHT_MIDDLE = 4.0f;
const float cEFFECT_DIST_WEIGHT_LOW = 5.0f;

//-----------------------------------------------------------------------------
// ����� ����Ʈ Ÿ��
//-----------------------------------------------------------------------------
enum ePET_EFFECT
{
	E_NONE =0,				//none
	E_WARP,					//����
	E_DESTORY,				//��ȯ����
	E_EAT,					//���̼���
	E_MIN_FULLNESS,			//�������϶�
	E_MAX_INTIMACY,			//ģ�е��ƽ�

	E_DECREASE_INTIMACY,	//ģ�е��϶�
	E_OWNER_DEATH,			//��������
	E_OWNER_LVUP,			//���η���
	E_BORING,				//5�д��		
	E_AUTO_PICK_ITEM,		//�������ڵ��ݱ�
};
//-----------------------------------------------------------------------------
// �߻�ü �ҽ� ����
//-----------------------------------------------------------------------------

enum eProjectile_SRC
{
    eProjectile_SRC_NONE = 0,
	eProjectile_SRC_ARROW =70001,
	eProjectile_SRC_THORN,
	eProjectile_SRC_THORN2,
	eProjectile_SRC_STONE,
	eProjectile_SRC_ESWORD,
	eProjectile_SRC_PHOENIX,
	eProjectile_SRC_WOLF,
	eProjectile_SRC_BEAR,
};
//-----------------------------------------------------------------------------
// Ż�� ���� ���ξִ�
//-----------------------------------------------------------------------------
enum ePLAYER_ANI
{
	eANI_STAND=0,		//���ĵ�
	eANI_IDLE,			//idle	
    eANI_IDLE2,			//idle2

	eANI_SIT,			//�ɱ�
	eANI_SIT_IDLE,		//�ɱ�idle
	eANI_GETUP,			//����

	eANI_RUN,			//�ɱ�

	eANI_R_MOVE,		//�������̵�
	eANI_L_MOVE,		//�����̵�
	eANI_B_MOVE,		//�ڷ��̵�

	eANI_R_ROTATE,		//��ȸ��
	eANI_L_ROTATE,		//��ȸ��

	eANI_JUMP_START,	//����
	eANI_JUMP_LOOP,		//������
	eANI_JUMP_END,		//����(���ڸ�)
	eANI_JUMP_LAND,		//����(�̵���)

	eANI_HJUMP_END,		//����������
	eANI_HJUMP_END_LOOP,//������������ ��ô��
	eANI_HJUMP_GETUP,	//������������ �Ͼ��	

    eANI_WINGS_SPREAD,  //!< ���� �
    eANI_WINGS_FURL,    //!< ���� ������

    eANI_RIDER_SPREAD,  //!< Ż���� �
    eANI_RIDER_FURL,    //!< Ż���� ������

    eANI_SPREAD_OTHERMOTION_MOVE,    //!< MOVE�� �ٸ����(SPACE KEY������ ���ϴ� ���)
    eANI_SPREAD_OTHERMOTION_IDLE,    //!< IDLE�� �ٸ����(SPACE KEY������ ���ϴ� ���)
};

//-----------------------------------------------------------------------------
// ���� �Ӽ� ����
//-----------------------------------------------------------------------------
enum eTERRAIN_ATTRIBUTE
{
	eTERRAIN_ATTRIBUTE_DIRT		= 0,
	eTERRAIN_ATTRIBUTE_STONE	= 1,
	eTERRAIN_ATTRIBUTE_PEBBLE	= 2, // �ڰ�
	eTERRAIN_ATTRIBUTE_GRASS	= 3, 
	eTERRAIN_ATTRIBUTE_METAL	= 4, 
	eTERRAIN_ATTRIBUTE_WATER	= 5, 
	eTERRAIN_ATTRIBUTE_MUD		= 6, 
	eTERRAIN_ATTRIBUTE_WOOD		= 7, 
	eTERRAIN_ATTRIBUTE_SNOW		= 8,
	eTERRAIN_ATTRIBUTE_DESERT	= 9
};

//-----------------------------------------------------------------------------
// �÷� ���̺�
//-----------------------------------------------------------------------------

// ��������� �ѹ��� ���ִ� ������ ��ȹ���� ID�� ����ϰ� �ϱ� ���ؼ��̴�.
// ! �߰��� + 1 �ؼ� �߰��Ѵ�.
// �߰��� ��������� �ȵ�.
enum eCOLOR_TABLE
{
	COLOR_DEFAULT					= 1,		// �Ϲ� ä�� / �Ϲ� TEXT COLOR
	COLOR_CHAT_WHISPER				= 2,		// �Ӹ�
	COLOR_CHAT_GUILD				= 3,		// ��� ä��
	COLOR_CHAT_PARTY				= 4,		// ��Ƽ ä��
	COLOR_CHAT_TRADE				= 5,		// �ŷ� ä��
	COLOR_CHAT_SYSTEM				= 6,		// �ý���
	COLOR_CHAT_GM					= 7,		// GM ��ȭ��
	COLOR_CHAT_GM_WHISPER			= 8,		// GM �Ӹ�
	COLOR_HELP						= 9,

	COLOR_GENERAL_PLAYER_NAME		= 10,		// �÷��̾� �̸�
	COLOR_GM_PLAYER_NAME			= 11,		// GM �÷��̾� �̸�
	COLOR_MONSTER_NAME				= 12,		// ���� �̸�

	COLOR_EXP						= 13,		// ����ġ
	COLOR_PLAYER_DAMAGE_HP			= 14,		// Ÿ ����
	COLOR_HERO_DAMAGE_HP			= 15,		// �ڽ�
	COLOR_PLAYER_HEAL_HP			= 16,		// �÷��̾� ��
	COLOR_MONSTER_HEAL_HP			= 17,		// ���� ��
	COLOR_DAMAGE_MP					= 18,
	COLOR_PLAYER_TO_MONSTER_DAMAGE	= 19,		// Ÿ�÷��̾ ���͸� �������� 
	COLOR_HERO_TO_MONSTER_DAMAGE	= 20,		// ���ΰ��� ���͸� ��������
	COLOR_HEAL_MP					= 21,		// MP ������
};

//-----------------------------------------------------------------------------
// ESC Ű ó�� ����
//-----------------------------------------------------------------------------
enum eDoType
{
	eDoType_System_Dialog,
	eDoType_Area_Pointer,
	eDoType_Target,
	eDoType_Auto_Attack,
	eDoType_Auto_Run,

	//
	eDoType_dialog_chat,
	eDoType_dialog_inventory,
	eDoType_dialog_charSkill,
	eDoType_dialog_Skill,
    eDoType_dialog_System_Menu,
	eDoType_dialog_Option_Menu,
	eDoType_dialog_Option_System,
	eDoType_dialog_Option_Game,
	eDoType_dialog_Option_KeyHelp,
	eDoType_dialog_Option_Sound,
	eDoType_dialog_Main_Option,
	eDoType_dialog_Friend,
	eDoType_dialog_Friend_Chat,


	eDoType_dialog_battle_create,
	eDoType_dialog_battle_List,
	eDoType_dialog_battle_Lobby,

	eDoType_dialog_bank,
	eDoType_dialog_shop,

	eDoType_dialog_enchant,
	eDoType_dialog_rankup,
	eDoType_dialog_item_selectmenu,
    eDoType_dialog_trade,
	eDoType_dialog_vendor_sell,
	eDoType_dialog_vendor_popup,
	eDoType_dialog_vendor_buy,
    eDoType_dialog_vendor_title_popup,
    eDoType_dialog_guild_menu,
    eDoType_dialog_guild_main,
    eDoType_dialog_guild_create,
    eDoType_dialog_guild_window,
    eDoType_dialog_guild_inputmessage,
	eDoType_dialog_party_option,
	eDoType_dialog_party_leave,
	eDoType_dialog_party_inviting,
	eDoType_dialog_new_enchant,

    eDoType_dialog_system_verify,

	eDoType_dialog_radar_full,
	eDoType_dialog_num_bank,
	eDoType_dialog_num,
	eDoType_dialog_num_trade,
    eDoType_dialog_battle_relay,
	eDoType_dialog_event_inventory,
    eDoType_cast_cancel,

	eDoType_Delay_Leave_Field,
	eDoType_Delay_Use_Return_Stone,
	eDoType_Delay_Spawn_Village,

	eDoType_ServerSelectStandingBy,
	eDoType_CharacterSelectStandingBy,

	eDoType_dialog_char_popup_menu,
	eDoType_dialog_help_window,
	eDoType_invoke_scene_event,
	eDoType_System_Message_Confrim,
	eDoType_System_Confrim,

	eDoType_dialog_event_window,
	eDoType_dialog_event_item_select_window,

#ifdef _KIKI_UI_WEB_DIALOG
	eDoType_dialog_web,
#endif
    eDoType_dialog_item_div,

	eDoType_dialog_Quest_Player,
	eDoType_dialog_Quest_NPC,

	eDoType_dialog_chunting_win,
	eDoType_dialog_chunting_score_board,

	eDoType_dialog_AreaConquest,

	eDoType_dialog_TotalComposite,

	eDoType_dialog_waypoint,

	eDoType_dialog_friend_info,

	eDoType_dialog_mission_rank_menu,
	eDoType_dialog_mission_rank_list,
	eDoType_dialog_mission_rank_gamble,
	eDoType_dialog_mission_rank_ending,
	eDoType_dialog_delete_cancel,
	eDoType_dialog_Socket_Remove_cancel,

#ifdef _ADD_CHARGE_ITEM_RANDOM_OPTION
	eDoType_dialog_Random_Option_cancel,
#endif

#ifdef _JBH_MOD_VENDOR_SELECT_BUY_NUM
	eDoType_dialog_vendor_buy_popup,
#endif

	eDoType_dialog_vendor_list,

	eDoType_dialog_community_dlg,
	eDoType_dialog_community_info,
	eDoType_dialog_community_chat,
	eDoType_dialog_community_read_mail,
	eDoType_dialog_community_write_mail,

	eDoType_dialog_cash_item_shop,
	eDoType_dialog_cash_item_shop_buy,
	eDoType_dialog_cash_item_shop_buy_check,
	eDoType_dialog_cash_item_shop_buy_complete,
	eDoType_dialog_cash_item_shop_cart,
	eDoType_dialog_cash_item_shop_npc_popup,
	eDoType_dialog_cash_item_shop_present,
	eDoType_dialog_cash_item_shop_present_check,
	eDoType_dialog_cash_item_shop_present_complete,
	eDoType_dialog_cash_item_shop_package,
	eDoType_dialog_cash_item_shop_package_check,
	eDoType_dialog_cash_item_shop_package_complete,
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
	eDoType_dialog_cash_item_shop_mileage,
	eDoType_dialog_cash_item_shop_mileage_buy,
	eDoType_dialog_cash_item_shop_mileage_check,
	eDoType_dialog_cash_item_shop_mileage_complete,
#endif	//	__NA_00387_20070605_SHOP_MILEAGE2
	eDoType_dialog_cash_item_shop_extension_buy,
	eDoType_dialog_cash_item_shop_extension_complete,
    eDoType_dialog_cash_item_purchase,
	eDoType_dialog_Tutorial_mission_invite,
	eDoType_dialog_Password,
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
	eDoType_dialog_GuildEventWindow,
#endif
	eDoType_Rank_Down_Cancle,
	eDoType_Repair_Limited_Cancle,
	eDoType_Guild_Alliance,
	eDoType_Guild_Hostility,
	eDoType_Guild_One_Side_Hostility,
	eDoType_Guild_Relation_ask,
	eDoType_Guild_Duty_Vest,
	eDoType_Guild_Right_Delegation,
	eDoType_Event_X_Mas_Confirm,
	eDotype_Event_X_Mas_SantaDlg,
	eDotype_Event_X_Mas_Donation_confirm,
	eDotype_Event_X_Mas_Donation_Reward,
	eDotype_Event_X_Mas_SoloCoupleReward,
	eDotype_Event_X_Mas_SantaReward,
	eDoType_Guild_WareHouse,
	eDoType_Guild_WareHouse_Right,
	eDoType_Guild_WareHouse_Log,
    eDoType_Lottery_OpenDlg,
	eDoType_Stat_Change_Item,
	eDoType_ObserverMode_Leave,
	eDoType_ObserverMode_ChangePos,
	eDoType_dialog_Personal_Waypoint,
	eDoType_dialog_Pet_NameSetting,

    eDoType_Cursor_Rider_Wax,

    eDoType_Etheria_Extract,
    eDoType_Etheria_Combine,
    eDoType_Char_Customize,

#ifdef _NA_0_20111012_ITEM_DATE_EXTEND
    eDoType_Extend_ItemDate,
#endif//_NA_0_20111012_ITEM_DATE_EXTEND

#ifdef _NA_003740_20111122_RANDOM_ITEM
    eDoType_Randomize_Item,
#endif//_NA_003740_20111122_RANDOM_ITEM

	eDoType_Dialog_Continent_Map_Dlg,
	eDoType_Dialog_Quest_List_Dlg,
	eDoType_Dialog_QuestInfo_Dlg,

	eDoType_Dialog_Lotto_Window_Dlg,
	eDoType_Dialog_Lotto_NumInput_Dlg,

#ifdef __NA_001358_ENCHANT_UNIFICATION
	eDoType_dialog_InstantEnchant_Cancle,
#endif //__NA_001358_ENCHANT_UNIFICATION

	eDoType_dialog_Domination_Main,
	eDoType_dialog_Domination_BettingList,
	eDoType_dialog_Domination_ApplyBetting,
	eDoType_dialog_Domination_ManagerDlg,
    eDoType_dialog_Domination_Member_Join,
    eDoType_dialog_Domination_Betting_List,
	eDoType_Delay_Leave_Domination,

	eDoType_Delay_Spawn_DominationWar,

    eDoType_Battle_Score,

    eDoType_dialog_itemshop_div,

    eDoType_Dialog_Event_Friend_Dlg,    //! ģ�� �ʴ� �̺�Ʈ
    eDoType_Dialog_Buffer_Event_Dlg,    //! ���� �̺�Ʈ

    eDoType_dialog_credit,
    eDoType_dialog_inventory_filter,
    eDoType_dialog_mission_object,
    eDoType_Dialog_Autobot, //!< ������ ȯ�� ���� ���̾�α�
    eDoType_dialog_Cube,
    eDoType_dialog_SocketComposite,
    eDoType_SocketIdentify,
    eDoType_SocketRecreate,
    eDoType_Crystalization,
    eDoType_AirShip_Comfirm,
#ifdef _DEV_VER
    eDoType_GM_Item_Window, // GM ������ ������
    eDoType_GM_Item_Option_Window, // GM ������ �ɼ� ������
    eDoType_GM_Item_Reset_Window, // GM ������ ���� ������
    eDoType_GM_Item_Menu_Window, // GM ������ �޴� ������
    eDoType_GM_Npc_Window, // GM ���ǽ� ������
#endif//_DH_GM_ITEM_WINDOW
    eDoType_GuideMessage_Dialog, 
    eDoType_GuideTutorial_Dialog,
    eDoType_GuideTutorial_Open_Dialog,
    eDoType_GuideInfomation_Dialog,
#ifdef _NA_002935_20110704_ITEM_SCORE
    eDoType_OtherPlayerEquipment_Dialog,
#endif //_NA_002935_20110704_ITEM_SCORE
    eDoType_HonorTitleList_Dialog, //_NA_003027_20111013_HONOR_SYSTEM
    
#ifdef _DH_BATTLEZONE2_
    eDoType_Battlezone2List_Dialog,
    eDoType_Battlezone2Lobby_Dialog,
#endif//_DH_BATTLEZONE2_
    eDoType_HotKeyDialog,

    // ī������
#ifdef _NA_000000_20120527_CHAOS_ZONE
    eDoType_Delay_Leave_ChaosZone,
#endif //_NA_000000_20120527_CHAOS_ZONE
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    eDoType_ChaoszoneDialog,
    eDoType_Delay_Spawn_BattleGround,
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    eDoType_Delay_Spawn_FreePVP,
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
    eDoType_VendorSearch,
#endif//#define _NA_000000_20120827_SEARCH_VENDOR

    eDoType_PersonalWarp,  
	eDoType_GeneralWarp,//�������ְ� ����

    eDoType_dialog_Option_System2,
    eDoType_dialog_Option_KeyBinding,
#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
    eDoType_Texture_Viewer_Search_Window,
    eDoType_Texture_Viewer_Texture_Window,
#endif
#endif
    eDoType_Accumulate_Dialog,
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    eDoType_SmartMenu_Dialog,
    eDoType_SmartReply_Dialog,
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    eDoType_Domination_Area_Owner_Tutorial_Dialog,
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

#ifdef _NA_007086_20140318_MONSTERBOOK
        eDoType_MonsterBook_View,
        eDoType_MonsterBook_DropList,
        //eDoType_MonsterBook_Extract,
#endif //_NA_007086_20140318_MONSTERBOOK
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
        eDoType_Attendance_Check,
#endif //_NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
		eDoType_SUNRanking_Dialog,
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		eDoType_SUNRanking_Statue_Info,
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        eDoType_Delay_SpaWarp_Dialog,
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
#ifdef _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
		eDoType_Equipment_Awakening_Dialog,
#endif // _NA_008121_20150311_EQUIPMENT_AWAKENING_SYSTEM
#ifdef _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
		eDoType_Equipment_Evolution_Dialog,
#endif // _NA_008121_20150318_EQUIPMENT_EVOLUTION_SYSTEM
#ifdef _NA_008405_20150716_MINIGAME_BINGO
		eDoType_Bingo_Dialog, 
#endif // _NA_008405_20150716_MINIGAME_BINGO
#ifdef _NA_008334_20150608_SONNENSCHEIN
        eDoType_Delay_Spawn_SonnenSchein,
#endif //_NA_008334_20150608_SONNENSCHEIN

    eDoType_MAX,
	//����! ���̳��� ������� �ڽ��� ���̵� ����Ѵ� 1500���� ���� 
	//�� ���� ����!
};

enum eSceneEvent
{
	eSceneEventSkip_EventScript,
};


enum eSKILLEFFECT_POS
{
	eSKILLEFFECT_POS_NONE = 0,
	eSKILLEFFECT_POS_HEAD = 1,
	eSKILLEFFECT_POS_BODY = 2,	
	eSKILLEFFECT_POS_FOOT = 3,
	eSKILLEFFECT_POS_RHAND = 4,
	eSKILLEFFECT_POS_LHAND = 5,
	eSKILLEFFECT_POS_WEAPON = 6,
	eSKILLEFFECT_POS_BONE_INDEX = 7,
	eSKILLEFFECT_POS_LRHAND = 8,
	eSKILLEFFECT_ONLY_POS = 9,  //! 2010.8.31 / i4u4me / ĳ�����ϰ� �����ϰ� �浹��ġ ���� ��� �����ص� �ٴ���ġ 
	eSKILLEFFECT_TARGET_POS = 10,
	eSKILLEFFECT_POS_BUST = 11,
	eSKILLEFFECT_POS_BOTTOM = 12,

};

//-----------------------------------------------------------------------------
// ����ȭ ������ ����Ʈ Ÿ�� 
//-----------------------------------------------------------------------------
enum eITEM_EFFECT_TYPE
{
	eITEM_EFFECT_TYPE_MIN   = 0,

	eITEM_EFFECT_TYPE_RHAND_EQUIP = 0,
	eITEM_EFFECT_TYPE_LHAND_EQUIP = 1,
	eITEM_EFFECT_TYPE_RHAND_WASTE = 2,
	eITEM_EFFECT_TYPE_LHAND_WASTE = 3,
	eITEM_EFFECT_TYPE_INVENTORY = 4,
    eITEM_EFFECT_TYPE_BADGE = 5,

	eITEM_EFFECT_TYPE_MAX
};


//-----------------------------------------------------------------------------
// ���콺 ���� ����
//-----------------------------------------------------------------------------
enum eMOUSE_OVER_TYPE
{
	eMOUSE_OVER_SLOT = 0,
	eMOUSE_OVER_SKILL,
    eMOUSE_OVER_STYLE,
    eMOUSE_OVER_QUEST,
	eMOUSE_OVER_MAX
};


//-----------------------------------------------------------------------------
// �޽��� ��� ����
//-----------------------------------------------------------------------------
enum eOUTPUT_MSG_TYPE
{
	eOUTPUT_MSG_TOP_SHOW	= (1<<0),
	eOUTPUT_MSG_CENTER_SHOW	= (1<<1),
	eOUTPUT_MSG_CHAT		= (1<<2),
	eOUTPUT_MSG_OUPUT_DEBUG	= (1<<3),
    eOUTPUT_MSG_LUA_CONSOLE = (1<<4),
};

enum eOUPUT_SYSTEM_TYPE
{
	eOUPUT_SYSTEM_VERIFY		= (1<<0),
	eOUPUT_SYSTEM_VERIFY_LOCK	= (1<<1),
	eOUPUT_SYSTEM_CONFIRM		= (1<<2),
	eOUPUT_SYSTEM_CONFIRM_LOCK	= (1<<3),
};

//-----------------------------------------------------------------------------
// ĳ���� �˾�â ����
//-----------------------------------------------------------------------------
enum eCHAR_POPUP_KIND
{
	eCPK_NONE,
	eCPK_NORMAL,
	eCPK_PARTY,
	eCPK_PARTY_MASTER,
 };

//-----------------------------------------------------------------------------
// ���� ����
//-----------------------------------------------------------------------------
enum eHELP_KIND
{
	eHK_CREATE_CHAR			=1,	//1. ĳ���� ����
	eHK_CLICK_NPC,				//2. NPC Ŭ��
	eHK_JOIN_BATTLE_ZONE,		//3. ��Ʋ�� ����
	eHK_EXP,					//4. ����ġ ����
	eHK_ITEM,					//5. ������ ����
	eHK_LEVEL_UP,				//6. ������
	eHK_HUNTING,				//7. ���ø� ���Խ�
	eHK_ETHERAIN,				//8. ���ڷ��� ���ԺҰ���
	eHK_AC,						//9. AC 5% ���޽� 
	eHK_GUILD,					//10. ���â�� �� 
	//eHK_CHUNTING,				//11. �������� Ÿ�� ������
	eHK_GM_MSG,					//11. GM �޽���
	eHK_QUEST,					//12. ����Ʈ
	eHK_QUEST_REWARD,			//13. Ư�� ����Ʈ����
	eHK_AC_REWARD,				//14. AC ���� (������)
};

//-----------------------------------------------------------------------------
// ���� �Ķ����
//-----------------------------------------------------------------------------
enum eHELP_PARAM
{
	eHP_ARMOR_OR_WEAPON		= 51,	//51: ���� �� ��
	eHP_ENCHANT_MATERIAL,			//52: ��æƮ ���
	eHP_AC_ITEM,					//53: AC������
	eHP_EVENT_ITEM,					//54: �̺�Ʈ������
	eHP_MATERIAL_ITEM,				//55: �Ϲ���� ������
	eHP_QUEST_ITEM,					//56: ����Ʈ ������
	eHP_CRYSTAL_ITEM,				//57: ����ü������
	eHP_LIMITED_ITEM,				//58: ����Ƽ�������

};

//-----------------------------------------------------------------------------
// ����ä�� ��� �ɼ�
//-----------------------------------------------------------------------------
enum eVOICE_CHAT_OPTION
{
	eVCO_ALLOW_ALL,		// ������
	eVCO_ALLOW_OPTION,	// ������ ���
	//eVCO_ALLOW_NOT,		// ������� ����
	
};
 
//-----------------------------------------------------------------------------
// ����ä�� ����
//-----------------------------------------------------------------------------
enum eVOICE_CHAT_STATUS
{
	eVCS_NONE,		// ������ȭ���� �ƴ�
	eVCS_NORMAL,	// 1:1 
	eVCS_PARTY,		// ��Ƽ
	eVCS_WAIT_VOICE_CHAT,	// 1:1 ������ȭ ��û�ϰ� �����
};

//-----------------------------------------------------------------------------
// �ŷ� Ÿ��
//-----------------------------------------------------------------------------
enum eITEM_TRADE_ACTION_TYPE
{
	eITA_TRADE	= 0,	// �ŷ�
	eITA_SELL,			// �Ǹ�
	eITA_DROP,			// ���
};


enum eATTACHTYPE 
{
	ATTACHTYPE_GENERAL = 0,
	ATTACHTYPE_INTERPOLATION,
};


enum eTRANSFORM_PROCESS
{
	TRANSFORM_PROCESS_NONE= 0,
	TRANSFORM_PROCESS_TRANSFORM,
	TRANSFORM_PROCESS_UNTRANSFORM,
};


enum OBJECT_SUB_TYPE
{
	OBJECT_SUB_TYPE_NONE     =0,
	OBJECT_SUB_TYPE_WAYPOINT = 3,
	OBJECT_SUB_TYPE_TERRAIN = 4,
	OBJECT_SUB_TYPE_TRANSPARENCY = 5,
	OBJECT_SUB_TYPE_DOMINATION_OBJECT = 6,
    OBJECT_SUB_TYPE_SHADOW_OBJECT = 7,
    OBJECT_SUB_TYPE_AIRSHIP = 8,
    OBJECT_SUB_TYPE_BATTLE_GROUND_STRONGPOINT = 9, //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    OBJECT_SUB_TYPE_BATTLE_GROUND_RESURRECTION = 10, //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
};

enum SCENE_TYPE
{
	SCENE_TYPE_NONE = 0,
	SCENE_TYPE_MOVIEPLAY	= 1,			
	SCENE_TYPE_START		= 2,			
	SCENE_TYPE_CHARSELECT	= 3,		
	SCENE_TYPE_LOGIN		= 4,
	SCENE_TYPE_LOAD			= 5,
	SCENE_TYPE_VILLAGE		= 6,	
	SCENE_TYPE_LOBBY		= 7,
	SCENE_TYPE_MISSION		= 8,
	
	SCENE_TYPE_PVP			= 10,  
	SCENE_TYPE_INSTANCE_DUNGEON	= 11, // �δ�(?)
	SCENE_TYPE_FIELD			= 12,
	SCENE_TYPE_CHUNTING			= 13,	// ����
	SCENE_TYPE_EVENT			= 14,
	SCENE_TYPE_TESTUI			= 15,
	SCENE_TYPE_RESOURCETEST		= 16,
	SCENE_TYPE_MAPEDIT			= 17,
	SCENE_TYPE_SSQ				= 18, // Ÿ���� ���
	SCENE_TYPE_DOMINATION		= 19, // ������
    SCENE_TYPE_BATTLEGROUND     = 20, // ����
    SCENE_TYPE_FREEPVP          = 21, //������������
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    SCENE_TYPE_GOLDRUSH         = 22, // ��� ����
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    SCENE_TYPE_SPA              = 23, // ��õ
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
#ifdef _NA_008334_20150608_SONNENSCHEIN
    SCENE_TYPE_SONNENSCHEIN     = 24, // ���PVE, �ҳٻ���
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    SCENE_TYPE_GUILDMISSION     = 25, // ���̼�
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

	MAX_SCENE_TYPE,
};

const DWORD c_dw_Special_Event_Num = 4; //Ư���� �̺�Ʈui�� �ʿ��� ���̽� Cnt

const DWORD c_dw_Donation_NPC_Type = 19;
const DWORD c_dw_SoloCouple_NPC_Type = 20;
const DWORD c_dw_Santa_NPC_Type = 21;
const DWORD c_dw_Buff_Event_NPC_Type = 22;

enum eITEM_CON_STATE
{
	eITEM_CON_STATE_NONE,
	eITEM_CON_STATE_EQUIP,
	eITEM_CON_STATE_INVEN,
};

enum eSPECIAL_TOOLTIP
{
	eSPECIAL_TOOLTIP_NONE = 0x00000000,
	eSPECIAL_TOOLTIP_NOMAL_ITEM = 0x00000001,
	eSPECIAL_TOOLTIP_LUCKY_ITEM = 0x00000002,

	eSPECIAL_TOOLTIP_RANDOM_ITEM = 0x00000004,

	eSPECIAL_TOOLTIP_ITEM_ON_TT_TEXT = 0x00000008,
    eSPECIAL_TOOLTIP_EXP_GAUGE = 0x00000010,
    eSPECIAL_TOOLTIP_IMAGE = 0x00000020,
};


enum eWAREHOUSE_TYPE
{
	eWAREHOUSE_TYPE_BANK,
	eWAREHOUSE_TYPE_GUILD,
	eWAREHOUSE_TYPE_MAX,
};


enum eGUILD_MARK_RELATION
{
	eGUILD_MARK_RELATION_EMPTY  = 61, //����X
	eGUILD_MARK_RELATION_RED    = 62, //���� ����,���뼱�� ����(Hero�� ���� ������)
	eGUILD_MARK_RELATION_BLUE   = 63, //���� ����
	eGUILD_MARK_RELATION_ORANGE = 64, //�Ϲ��� ���� ����(Hero�� ���� ���� ����)
};

#define		PLAYER_OBJECT_KEY_START		 0
#define		PLAYER_OBJECT_KEY_END		 50000	

enum eSTATE_KIND
{
#ifdef _NA_006819_20130719_STATE_ICON_RENDER
    eSTATE_KIND_DEBUFF = 0,
    eSTATE_KIND_ITEM = 1,
    eSTATE_KIND_CASH = 2,
    eSTATE_KIND_ETC = 3, //��Ÿ
    eSTATE_KIND_SHIELD = 4,//���� �ֿ켱
    eSTATE_KIND_SKILL = 5,
    eSTATE_KIND_MAX = 6,
#else // _NA_006819_20130719_STATE_ICON_RENDER
    eSTATE_KIND_DEBUFF = 0,
    eSTATE_KIND_SKILL = 1,
    eSTATE_KIND_CASH = 2,
    eSTATE_KIND_ETC = 3, //��Ÿ
    eSTATE_KIND_SHIELD = 4,//���� �ֿ켱
    eSTATE_KIND_MAX = 5,
#endif // _NA_006819_20130719_STATE_ICON_RENDER

};

const WORD c_wOUTPUT_LOG_FILE                   = 0x01;
const WORD c_wOUTPUT_LOG_CONSOLE                = 0x02;
const WORD c_wOUTPUT_LOG_SYSCHAT                = 0x04;
const WORD c_wOUTPUT_LOG_CENTER                 = 0x08;
const WORD c_wOUTPUT_LOG_SCREEN                 = 0x10;

enum eLOG_LEVEL
{
	eLOG_LEVEL_NONE = 0,	// �α׸� ������ ����
	eLOG_LEVEL_ERR = 1,		// ERROR
	eLOG_LEVEL_WAR = 2,		// WARNING
	eLOG_LEVEL_INF = 3,		// INFORMATION
	eLOG_LEVEL_DBG = 4,		// DEBUG MESSAGE
	eLOG_LEVEL_TRA = 5,		// TRACE MESSAGE
};

enum eSPECIAL_MODE			// eSPECIAL_MODE STEP �� �������� �� �پ��� ��� ����
{
	eSPECIAL_MODE_NONE = 0,		// ����� ��尡 �ƴϴ�.
	eSPECIAL_MODE_STEP1 = 1,	
	eSPECIAL_MODE_STEP2 = 2,	
	eSPECIAL_MODE_STEP3 = 3,	
	eSPECIAL_MODE_STEP4 = 4,	
};

enum LanguageType
{
    kLanguageInvalid    = -1,
    kLanguageGlobal     = 0,    // default �����(england, usa, global)
    kLanguageKorea,             // 
    kLanguageChina,             // cn
    kLanguageTaiwan,            // tw
    kLanguageJapan,             // jp
    kLanguageGermany,           // gm
    kLanguageRussia,            // ru
};

//------------------------------------------------------------------------------

enum PetGauge_ShowType
{
    kPetGaugeShowType_Head = 0,     //�� ����
    kPetGaugeShowType_Screen,
    kPetGaugeShowType_Mini,
    kPetGaugeShowType_Size
};

namespace ClassMarkInfo
{
    const DWORD kClassMarkResourceCode = 3000000000;

    const DWORD kClassMarkTextureX = 321;
    const DWORD kClassMarkTextureY = 156;
    const DWORD kClassMarkTextureWidth = 43;
    const DWORD kClassMarkTextureHeight = 43;
};

#ifdef _NA_002935_20110704_ITEM_SCORE
const int kItemScoreMax = 99999;
#endif //_NA_002935_20110704_ITEM_SCORE

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
const int kTempClassTypeMax = eCHAR_TYPE_MAX;
#else
// _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_TEMP
// UI���ҽ� �� ĳ���� ���� ������ �����Ͽ� eCHAR_TYPE_MAX �� ���� �ӽ� ����� [6/19/2012 iforall]
const int kTempClassTypeMax = eCHAR_ELEMENTALIST + 1;
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
enum BattleGroundHideMode
{
    BattleGroundHideMode_None,
    BattleGroundHideMode_Full,
    BattleGroundHideMode_Half,
};
enum battle_ground_object_type
{
    battle_ground_object_normal_waypoint = 0,
    battle_ground_object_red_waypoint,
    battle_ground_object_blue_waypoint,

    battle_ground_object_normal_flag,
    battle_ground_object_red_flag,
    battle_ground_object_blue_flag,
    battle_ground_object_half_blue_flag,
    battle_ground_object_half_red_flag,
    battle_ground_object_max
};
// ������ ����Ʈ ǥ���� �ִϸ��̼����� �ϰ� �ֱ� ������ �ִϸ��̼� ����� ��Ī�ؼ� ���
enum battle_ground_waypoint_animation
{
    battle_ground_normal_waypoint_whitefield,   // �� ���� �ִϸ��̼�
    battle_ground_normal_waypoint_whitefield_redconquering,
    battle_ground_normal_waypoint_redfield_redconquered,
    battle_ground_normal_waypoint_redfield,
    battle_ground_normal_waypoint_redfield_blueconquering,
    battle_ground_normal_waypoint_whitefield_blueconquering,
    battle_ground_normal_waypoint_bluefield_blueconquered,
    battle_ground_normal_waypoint_bluefield,
    battle_ground_normal_waypoint_bluefield_redconquering,

    battle_ground_red_waypoint_redfield,    // �� ���� �ִϸ��̼�
    battle_ground_red_waypoint_redfield_blueconquering,
    battle_ground_red_waypoint_redfield_redconquered,

    battle_ground_blue_waypoint_bluefield,  // �� ���� �ִϸ��̼�
    battle_ground_blue_waypoint_bluefield_redconquering,
    battle_ground_blue_waypoint_bluefield_blueconquerd,

    battle_ground_flag_change_animation, // ��� ���� �ִϸ��̼�

    battle_ground_waypoint_animation_max
};
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#endif // _GameConst_h_
