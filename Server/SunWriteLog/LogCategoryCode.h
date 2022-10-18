#pragma  once

enum eLOG_CATEGORY
{
	LOG_SESSION		=	0x00010001,		//���ӷα�
	LOG_ITEM		=	0x00020001,		//�����۷α�
	LOG_EXP			=	0x00030001,		//����ġ
	LOG_ACTION		=	0x00040001,		//�׼�
	LOG_SKILL		=   0x00050001,		//��ų
	LOG_SNAPSHOT	=   0x00060001,		//������
	LOG_CHAT		=   0x00070001,		//ä�� �α�
	LOG_EVENT		=	0x00090001,		//�̺�Ʈ
	LOG_ADMIN		=   0x00100001,		//GM(���) �α�
	LOG_MONEY		=	0x00200001,		//���α�
	LOG_GUILD		=	0x00300001,		//���α�
    LOG_ACCUMULATEPOINT = 0x00400001,		//��������Ʈ�α�
};


/// ���ӻ����ڵ�
enum eLOG_SESSION
{
	SESSION_LOGIN = 1000,				// �α���
	SESSION_LOGOUT,						// �α׾ƿ�(����)
	SESSION_CHAR_CONNECT,				// ĳ���� ����
	SESSION_CHAR_DISCONNECT,			// ĳ���� ���� ����
	SESSION_WORLD_LOGIN,				// ���� �α��� : ���� ��� ����
	SESSION_WORLD_LOGOUT,				// ���� �α׾ƿ� : ���� ��� ����
    // _NA_20100802_SPEED_HACK_DETECTION
    SESSION_SPEED_HACK_SUSPECT,         // ���ǵ��� ��� �ǽ�
    SESSION_SPEED_HASK_SUSPECT_DISCONN, // ���ǵ��� ��� �ǽɿ� ���� ���� ���� ����
    // _NA_007750_20141113_WZDAT_MODIFY_LOG
    SESSION_CHANNEL_INFO,               // ä�� ����(������, ä��ID)
    //_NA_008506_20150923_LOG_LOGOUT_IN_GAMESERVER
    SESSION_LOGOUT_GAMESERVER,          // �α׾ƿ�(�α׾ƿ� ��, ��/��ġ���� ����)
};

enum eLOG_SESSION_LOGOUT_TYPE
{
	eLOGOUT_NORMAL,
	eLOGOUT_ABNORMAL,
	eLOGOUT_HACK,
	eLOGOUT_EXPIRED,
};

/// �����۷α��ڵ�
enum eLOG_ITEM
{
	ITEM_MONSTER_DROP = 2000,			// ���� ������ ���
	ITEM_MAKE_SERIAL,					// ������ �ø��� �߱޽�

	ITEM_USE,							// ������ ���
	ITEM_PICK,							// ������ ����
	ITEM_DROP,							// ������ �ٴڿ� ����.
	ITEM_DELETE,						// ������ ����
	ITEM_DIVIDE,						// ������ ������
	ITEM_COMBINE,						// ������ ��ġ��
	ITEM_EXCHANGE,						// ������ ��ȯ

	ITEM_NPC_BUY,						// NPC���Լ� ����
	ITEM_NPC_SELL,						// NPC���� �ȱ�

	ITEM_TRADE_START,					// �ŷ� ����
	ITEM_TRADE_CANCEL,					// �ŷ� ���
	ITEM_TRADE_GIVE,					// �ŷ� �ֱ�
	ITEM_TRADE_RECEIVE,					// �ŷ� �ޱ� 

	ITEM_VENDOR_START,					// ���� ���� ����
	ITEM_VENDOR_PRICE,					// ���� ���� ���� ����
	ITEM_VENDOR_END,					// ���� ���� ���
	ITEM_VENDOR_VIEW,					// ���� ���� ��������
	ITEM_VENDOR_BUY,					// ���λ��� ���
	ITEM_VENDOR_SELL,					// ���λ��� �ȱ�

	ITEM_WAREHOUSE_GET,					// â���� ã��
	ITEM_WAREHOUSE_PUT,					// â�� �ñ��
	ITEM_WAREHOUSE_INFO,				// â�� ������ ����

	ITEM_ENCHANT_FAILED,				// ��þƮ ���� + ��� ����
	ITEM_ENCHANT_SUCCESS,				// ��þƮ ���� + ��� ����

	ITEM_RANKUP_BEFORE,					// ��ũ�� �� ����
	ITEM_RANKUP_AFTER,					// ��ũ�� �� ����

	ITEM_COMPOSE_SUCCESS,
	ITEM_COMPOSE_FAILED,

	ITEM_CRYSTALIZE_SUCCESS,
	ITEM_CRYSTALIZE_FAILED,

	ITEM_USE_EXTRASTONE,				// ����Ʈ�� ���� ���

	ITEM_SOCKET_ADD,					// ���� ���� �ձ�(���������ȵ�)
	ITEM_SOCKETOPT_FILL,				// ���� �ɼ� ä���
	ITEM_SOCKETOPT_EXTRACT,				// ���� �ɼ� �����

	ITEM_QUEST_OBTAIN,					// ����Ʈ ������ ���� ������
	ITEM_QUEST_DELETE,					// ����Ʈ ���� ������

	ITEM_REWARD_QUEST,					// ����Ʈ ����
	ITEM_REWARD_AC,						// AC ����
	ITEM_REWARD_MISSION,				// �̼� ����
	ITEM_REWARD_EVENT,					// �̺�Ʈ ����
	ITEM_REWARD_COMPETITION,			// �������� ����

	ITEM_DURA,							//< ������ ������ ����	

	// 2006�� 5�� 31�� ������ ���� 4:14:05
	ITEM_EVENT_ROCKET,					//< ����Ʈ���͸� �̺�Ʈ

	ITEM_EVENT_ETHER_REG,				//< ���׸� ��ǰ ���� �̺�Ʈ
	ITEM_EVENT_ETHER_REG_FAILED,		//< ���׸� ��ǰ ���� �̺�Ʈ ���н� ������ ������
	ITEM_EVENT_ETHER_DELETE,			//< ���׸� ������ ��ȯ �̺�Ʈ �� ������ ������

	ITEM_VENDOR_VIEW_END,				//< ���λ�����������

	// 20060717 ���� 12:06:10
	ITEM_AC_ITEM_USE,					//< AC������ ���

	ITEM_PK_DROP,						//< PK�� ���� ������ ���

	ITEM_OVERLAPPED_ITEM,				//< �������� �����ۿ� ���� �α�
	ITEM_DELETE_CAUSE_DURA,				//< ������0�̾ ������ ������ �α�

	ITEM_REWARD_RANKING,				//< �̼� ��ŷ �α�

	// 20061212 ���� 10:17:17
	ITEM_CHARGE_BUY_REQUEST,			//< ĳ�þ����� ���� ��û
	ITEM_CHARGE_BUY_SUCCESS,			//< ĳ�þ����� ���� ����
	ITEM_CHARGE_BUY_FAILED,				//< ĳ�þ����� ���� ����

	ITEM_DELETE_REVENGE_PORTAL,			//< ������ �� ����
	ITEM_DELETE_ITEM_PORTAL,			//< ��ǥ�̵� ������ ����
	ITEM_DELETE_PROTECT_PKDROP,			//< PKDROP ������ ����

	ITEM_CHARGE_EXPIRED,				//< ĳ�þ����� ��ȿ�Ⱓ ����
	ITEM_CHARGE_FIRST_EQUIP,			//< ó�� �����Ͽ� ��ȿ�Ⱓ�� ������

	ITEM_CHARGE_SEND_GIFT_SUCCESS,		//< ĳ�þ����� ���� ������ ����
	ITEM_CHARGE_SEND_GIFT_FAILED,		//< ĳ�þ����� ���� ������ ����

	ITEM_CHARGE_ACTION_RECV_GIFT_SUCCESS,	//< ĳ�þ����� ���� ���� �ޱ� ����
	ITEM_CHARGE_ACTION_RECV_GIFT_FAILED,	//< ĳ�þ����� ���� ���� �ޱ� ����

	// 20070618 ���� 03:13:27
	ITEM_TEMP_INVENTORY_GET,				//< ���� â���� ã��
	ITEM_DUPLICATE_SERIAL,					//< ������ �ø��� �ߺ�

	ITEM_RANKDOWN_BEFORE,					//< ��ũ�ٿ� �� ����
	ITEM_RANKDOWN_AFTER,					//< ��ũ�ٿ� �� ����

	ITEM_IDENTIFY_LUCKY_ITEM,				//< ��Ű ������ ����

	ITEM_REWARD_SSQ_WINNER,					//< Ÿ���� ��� ���� ����
	ITEM_REWARD_SSQ_BATTLE_PRIZE,			//< Ÿ���� ��� ������ ��ǰ

	ITEM_GUILD_WAREHOUSE_GET,					// ��� â���� ã��
	ITEM_GUILD_WAREHOUSE_PUT,					// ��� â�� �ñ��

	ITEM_ENCHANT_FAILED_DOWN,				// ��þƮ ����_���� �ٿ� + ��� ����

	// 20090130 �߰�.
	ITEM_GET_BY_LOTTERY,					// �ζǳ� �ǵ����� ���ڿ� ���� ������ ����.
	
	ITEM_SSQ_ENTRANCE_ITEM_REFUND,			//< Ÿ���� ��� ���� ������ ȯ��

	ITEM_IDENTIFY_POSSESSION_ITEM,			//< �ͼ� ������ ����
	
	ITEM_ENCHANT_FAILED_DESTROY,			//< ��þƮ ����_������ �ı� + ��� ����

    ITEM_NPC_REPURCHASE,					//< NPC���Լ� �籸��
    //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
    ITEM_CHANGED_TO_SHELL,                  //< ���׸��ư� ����� ������
    ITEM_COMBINED_WITH_ETHERIA,             //< ���׸��ƿ� ���յ� ������
    //_NA000541_100901_ITEM_CUSTOMIZING_CONTENTS_USING_VARIOUS_ETHEREALIZER_
    ITEM_CHANGED_TO_ETHERIA,                //< ���׸��Ʒ� ��ȯ�� ������
    // _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
    ITEM_DOMINATION_AUCTION_REWARD_ITEM_BY_SYSTEM_MEMO, //������ ������ ����:2084 
    ITEM_DOMINATION_AUCTION_RETURN_ITEM_BY_SYSTEM_MEMO, //������ ������ ȯ��:2085
    ITEM_DEFAULT_SYSTEM_MEMO_REWARD,                    //�⺻���� �ý��� �޸�, ������ ������ ���

    ITEM_DOMINATION_AUCTION_APPLY,                      //������ ������ ������ ��� ���

    // _NA_20110303_ADD_TYPE_OF_NPCSHOP
    ITEM_BUY_NPC_SHOP_TO_ADDITIONAL_TYPE_,              //�߰� ���� Ÿ�� ������ ����

    ITEM_CUBE_COMPOSE_MATERIAL, // ť�� �ռ��� ���� ���
    ITEM_CUBE_COMPOSE_REWARD, // ť�� �ռ����� ���� ����
    ITEM_CUBE_DECOMPOSE_MATERIAL, // ť�� ���ؿ� ���� ���
    ITEM_CUBE_DECOMPOSE_REWARD, // ť�� ���ط� ���� ����
    ITEM_ZARD_COMPOSE_MATERIAL, // ��� �ռ��� ���� ���
    ITEM_ZARD_COMPOSE_REWARD, // ��� �ռ����� ���� ����
    // NOTE: f110602.3L, player was basically rewarded for his particifation.
    ITEM_REWARD_SSQ_DEFAULT_PRIZE,
    
    ITEM_DOMINATION_AUCTION_DUP_APPLY,                //������ ��� �ߺ� ���� ������(������ǥ) ���

    // _NA_003740_20111122_RANDOM_ITEM
    ITEM_RANDOMIZE_OPTION, // ������ �ɼ� ����ȭ
	
    // _NA_004035_20120227_COSTUME_ITEM
    ITEM_USE_COSTUME, // �ڽ�Ƭ ������ ���
    ITEM_USE_TOGGLE, // ���������� ��� //_NA_006372_20130108_BADGE_ITEM

    ITEM_SMARTNPC_GIVE,						// SMARTNPC���� ���� ������
    ITEM_SMARTNPC_ROB,						// SMARTNPC���� �� ������

    // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ITEM_USE_GUILD_FACILITY_UPGRADE,        // ��� �ü� ���׷��̵�� ������ ���
    ITEM_USE_GUILD_FACILITY_ACTIVE,         // ��� �ü� Ȱ��ȭ�� ������ ���

    ITEM_ENCHANT_FAILED_PREVENT,            // ��æƮ�� ���������� ������ �Ҹ� �� �ٿ� ����

    // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    ITEM_REWARD_ATTENDANCE,                 // �⼮ ����

    // _NA_008124_20150313_AWAKENING_SYSTEM
    ITEM_AWAKENING_SUCCESS,                 // ���� ����
    ITEM_AWAKENING_FAILED,                  // ���� ����
    ITEM_AWAKENING_DELETE_MATERIAL,         // ���� ��� ����
    ITEM_EVOLUTION_SUCCESS,                 // ��ȭ ����
    ITEM_EVOLUTION_FAILED,                  // ��ȭ ����
    ITEM_EVOLUTION_DELETE_EQUIPMENT,        // ��ȭ ��� ����

    //_NA_008404_20150712_MINIGAME_SYSTEM
    ITEM_REWARD_MINIGAME_BINGO,             // �̴ϰ���/���� ����

    ITEM_MAX,
};

enum eLOG_MONEY
{
	MONEY_TOTAL = 9000,					//< �ֱ����� + �� �α�

	MONEY_MONSTER_DROP,
	MONEY_MONSTER_JACKPOT_DROP,

	MONEY_PICK,
	MONEY_DROP,

	MONEY_NPC_BUY,
	MONEY_NPC_SELL,

	MONEY_WAREHOUSE_PUT,
	MONEY_WAREHOUSE_GET,

	MONEY_ENCHANT,
	MONEY_RANKUP,
	MONEY_COMPOSE,
	MONEY_SOCKETOPT_EXTRACT,

	MONEY_TRADE,
	MONEY_VENDOR,						//< 2006/5/30 18:29:00  ���� ������-_-;;

	// ����Ʈ ����, AC����, �̼� ����
	MONEY_REWARD_QUEST,
	MONEY_REWARD_AC,
	MONEY_REWARD_MISSION,
	MONEY_REWARD_COMPETITION,

	MONEY_GREATE_MONEY,					//< ���� �����ݾ� �̻��� ���

	MONEY_TOTALWAREHOUSE,				// â�� ��ȭ��

	// 2006/5/30 18:29:00 �߰�
	MONEY_VENDOR_BUY,					//< �����ڰ� ���λ������� �������� �� ����
	MONEY_VENDOR_SELL,					//< �Ǹ��ڰ� ���λ������� �������� �� ����

	MONEY_TRADE_PUT_PRICE,				//< �ŷ��� �ִ� ���� ����
	MONEY_TRADE_GET_PRICE,				//< �ŷ��� ���� ���� ����

	// 20061212 ���� 10:17:17
	MONEY_CHARGE_BUY_BY_HIME,					//< ĳ�þ����� �������� ����
	MONEY_CHARGE_RECOVER_FOR_BUY_BY_HIME,		//< ĳ�þ����� �������� ���� -> ���н� ����
	MONEY_CHARGE_BUY_BY_CASH,					//< ĳ�þ����� �������� ����

	MONEY_CHARGE_GIFT_BY_HIME,					//< ĳ�þ����� �������� ���� 
	MONEY_CHARGE_RECOVER_FOR_GIFT_BY_HIME,		//< ĳ�þ����� �������� ���� -> ���н� ����

	// 20070619 ���� 17:50
	MONEY_SPEND_REPAIR,							//< ��������� ���� �Һ�
	MONEY_SPEND_WAYPOINT,						//< ��������Ʈ�� ���� �Һ�

	MONEY_REWARD_SSQ_GAMBLE,					//< Ÿ���� ��� ���� ���� ����

	MONEY_GUILD_WAREHOUSE_PUT,				// ��� â�� �� ����ֱ�
	MONEY_GUILD_WAREHOUSE_GET,				// ��� â�� �� ������

    MONEY_SPEND_SSQ_ADMISSION_FEE,      //< Ÿ���� ��� ����� ���� �Һ�

    // __NA_001359_20090619_HEIM_LOTTO_SYSTEM
    MONEY_LOTTO_REWARD,                         //< ���� �ζ� ��� ȹ��

    // __NA_0_20090820_SSQ_FEE_REFUND_LOG
    MONEY_REFUND_SSQ_ADMISSION_FEE,             //< Ÿ���� ��� ����� ���� ȯ��

    //_KR_001385_20091211_DOMINATION_GAMELOG_MONEY
    MONEY_DOMINATION_ACTIONMONEY_RETURN,        //���� ������ ��� �ݾ� ȯ�� ����:9038
    MONEY_DOMINATION_ACTIONMONEY_REWARD,        //���� ������ ��� �ݾ� ���� ����:9039
    MONEY_DOMINATION_ACTIONMONEY_APPLY,         //���� ������ ��� �ݾ� ���� ����:9040

    // _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    MONEY_LOTTO_TICKET_PURCHASE,                //< ���� ��� ���ǿ��� ���� ����

    // _NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG
    MONEY_GROUPSYSTEM_GETMONEY,                 //�׷� �ý��ۿ��� ���� ����� ���� �޴� ���� �Ӵ�
    
    // _NA_20100901_ADD_GAMELOG_FOR_SYSTEM_MEMO_TYPE
    MONEY_DEFAULT_SYSTEM_MEMO_REWARD,           //�⺻���� �ý��� �޸��� ���� ������ ���:9043

    // __NA_20101209_ADD_GAMELOG_FOR_HEIMLOTTO
    MONEY_HEIM_LOTTO_SETTLE_RESULT,             //���ӷζ� ��� ���(�ο��� ���� ���)

	MONEY_MAX,
};


///	����ġ�α��ڵ�
enum eLOG_EXPERIENCE
{
	EXP_LEVELUP = 3000,			//< ���� ��
	EXP_DOWN,					//< ����ġ �ٿ�
	EXP_DIVIDECNT,				//< �������� n/10 ����
	EXP_QUEST,					//< ����Ʈ ���� ����ġ
	EXP_COMPETITION,			//< �������� ���� ����ġ
	EXP_AC,
	EXP_MISSION,
    // _NA_20100423_GROUPSYSTEM_MONEY_AND_EXP_GAMELOG
    EXP_GROUPSYSTEM,            //< �׷� �ý��� ���� ���� ����� ���� �޴� ����ġ �α�
    // NOTE: f110602.3L, perk experience & changed level log
    EXP_PERK,
    EXP_PERK_LEVEL,
    //
};


/// ��ų �α� �ڵ�
enum eLOG_SKILL
{
	SKL_USE_SKILL = 5000
};


/// ������� �ǹ��ִ� Action �α� �ڵ�
enum eLOG_ACTION
{
	ACT_CREATE_CHAR = 4000,		// ĳ���� ����
	ACT_DELETE_CHAR,			// ĳ���� ����
	ACT_DEAD_CHAR,				// ĳ���� ���
	ACT_REBIRTH_CHAR,			// ĳ���� ��Ȱ
	ACT_PVP_START,				// PVP�õ�
	ACT_USE_STAT,				// ���� ���
	ACT_ACQUIRE_SKILL,			// ��ų ȹ��
	ACT_SKILLLV_UP,				// ��ų���� ��
	ACT_MOVE_ROOM,				// ������ �̵�(�������� ������, �濡�� ������)
	ACT_MOVE_FIELD,				// �ʵ�� �̵�(�������� �ʵ��, �ʵ忡�� ������)
	ACT_INVENLOGIN,				// �α��� �Ҷ� �κ��丮, ���λ����� �ִ� ������ ����
	ACT_INVENLOGOUT,			// �α׾ƿ��� �κ�/���λ����� �ִ� �ƾ��� ����
	ACT_PVPKILL,				// �ٸ� ���� ����
	ACT_PVPDIE,					// ������ ����
	ACT_GETTAX,					// ���� ȹ��

	ACT_PK_KILL,				// PK�� �ٸ� ���� ����
	ACT_PK_DIE,					// PK�� ������ ����
	ACT_PK_CHANGE_CHAOSTATE,	// ī������ ����

	ACT_MISSION_RANKING_SUCCESS,// �̼� ��ŷ ���� ���
	ACT_MISSION_RANKING_FAIL,	// �̼� ��ŷ ���� ���

	ACT_TRACK_REVENGE,			// ������ ��� ����
	ACT_PORTAL_BY_ITEM,			// ��ǥ�̵� �������� �̿��� ��Ż

	ACT_SKILLLV_DOWN,			//��ų ���� �ٿ�
	ACT_SKILL_UNLEARN,			//��ų UNLEARN
	ACT_DECREASE_STAT,			//���� ����
	ACT_INIT_STAT_ALL,			//���� �ʱ�ȭ
	ACT_INIT_BASIC_STAT_ALL,	//�⺻���� �ʱ�ȭ
	ACT_INIT_EXPERTY_STAT_ALL,	//���õ� ���� �ʱ�ȭ
	ACT_INIT_PASSIVE_SKILL_ALL,	//�нú� ��ų �ʱ�ȭ

	ACT_KILL_SPECIAL_MONSTER,	//�߿� ���� ����

	ACT_FATIGUE_PLAY_TIME,		//�Ƿε�
	ACT__RANKING_POINT,			//��ŷ ����Ʈ

	ACT_TROUBLED_KILL,			// �������� �ٸ����� ����
	ACT_TROUBLED_DIE,			// �������� ������ ����

	ACT_HOSTILITY_GUILD_KILL,	// ������ �ٸ����� ����
	ACT_HOSTILITY_GUILD_DIE,	// ������ ������ ����

	ACT_CHARACTER_CHANGE_UP,	// ĳ���� ü���� ��
	ACT_JOIN_MAP,				// �� ����
	ACT_QUEST_ACCEPT,			// ����Ʈ ����
	ACT_QUEST_REWARD,			// ����Ʈ ����
	ACT_QUEST_ABANDON,			// ����Ʈ ����

    // __NA_001359_20090619_HEIM_LOTTO_SYSTEM
    ACT_HEIM_LOTTO_PICK,        // ���� ��� ���� ����

    //_NA001385_20090924_DOMINATION_GAMELOG_ACTION
    ACT_DOMINATION_FIELD_JOIN,          //���� ������ ����
    ACT_DOMINATION_FIELD_LEAVE,         //���� ������ ����
    ACT_DOMINATION_DESTROY_OBJECT,      //���� ������ ��� Ÿ�� �ı�(����,���׸����)
    ACT_DOMINATION_SET_GATE,            //���� ������ ���� �� ����
    ACT_DOMINATION_STAMP_START,         //���� ������ ���� ����
    ACT_DOMINATION_STAMP_COMPLETE,      //���� ������ ���� �Ϸ�
    ACT_DOMINATION_SUCCESS_DEFENCE,     //���� ������ ���� ����
    ACT_DOMINATION_SUCCESS_OFFENCE,     //���� ������ ���� ����
    ACT_DOMINATION_KILL,                //���� ���������� �ٸ� ���� ����
    ACT_DOMINATION_DIE,                 //���� ���������� ������ ����
    ACT_DOMINATION_CRYSTALWARP_CREATE,  //���� ������ �������� ��ȯ
    ACT_DOMINATION_CRYSTALWARP_DESTROY, //���� ������ �������� ����
    ACT_DOMINATION_CRYSTALWARP_DEAD,    //���� ������ �������� �ı�(�����, �ð� �ʰ�)

    ACT_FRIENDINVITATION_RECALL,        //ģ���ʴ� �̺�Ʈ�� ���� ��ȯ ���� �α�

    ACT_DOMINATION_REWORD_EVENT_START,  //���� ������ ���� �̺�Ʈ ����(��Ű ���� ��ȯ Ʈ����)

    ACT_DOMINATION_SET_ATTACK_GUILD,    //���� ������ ���� ��� ���� �̺�Ʈ

	ACT_ACHIEVEMENT_VALUE_CHANGED,      //������ AC ������ �����

    // _NA_1798_20110114_SYSTEM_MEMO_ADDITIONAL_LOGGING
    ACT_SYSTEM_MEMO_RECEIVE,            //�ý��� ���� ����
    ACT_SYSTEM_MEMO_DELETE,             //�ý��� ���� ���� (����� ���ۿ� ����)
    ACT_ALL_MEMO_DELETE,                //��ü ���� �ϰ� ���� (����� ���ۿ� ����)

    // _NA_20110216_INCREASE_DOMINATION_DEFFENSEWALL_POINT
    ACT_DOMINATION_ADD_DEFFENSEWALL_POINT, // ������ �溮 ����Ʈ �߰�

    ACT_DOMINATION_FIELD_JOIN_COUNT,    //������ ���� �ο� ī��Ʈ

    POINTWALLET_DEPOSIT_BY_USING_ITEMS,     // �������� ����Ͽ� �����ϱ�
    POINTWALLET_WITHDRAW_BY_BUYING_ITEMS_ON_MARKET, // ����Ʈ�������� ����Ʈ�� ������ ����

    ACT_MISSION_CLEAR,                  // �̼� Ŭ���� �α�
    ACT_CHARACTER_NAME_CHANGE,          // ĳ���� ����
    
    // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    ACT_BATTLE_GROUND_ENTER, //���� ����
    ACT_BATTLE_GROUND_LEAVE, //���� ����
    ACT_BATTLE_GROUND_KILL, //���� ų
    ACT_BATTLE_GROUND_DEATH, //���� ����
    ACT_BATTLE_GROUND_REWARD, //���� ����
    ACT_BATTLE_GROUND_STRONGPOINT_ATTACK, //���� ���� ����
    ACT_BATTLE_GROUND_STRONGPOINT_DEFENSE, //���� ���� ���
    ACT_BATTLE_GROUND_WIN, //���� ��
    ACT_BATTLE_GROUND_LOSE, //���� ��
    ACT_BATTLE_GROUND_DRAW, //���� ��
    ACT_BATTLE_GROUND_DIS, //���� Dis

    //_NA_007237_20140519_GAMELOG_DONATION_GUILDCOIN
    ACT_GUILD_DONATION_GUILDCOIN,    // ������� ���

    // _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
    // GoldRush
    ACT_GOLDRUSH_MONSTERKILL,       // ��巯�� / ����ų
    ACT_GOLDRUSH_GOLDPOINT_GET,     // ��巯�� / �������Ʈ ȹ��
    ACT_GOLDRUSH_GOLDPOINT_LOSE,    // ��巯�� / �������Ʈ �ս�
    ACT_GOLDRUSH_BATTLERESULT,      // ��巯�� / ����������� ���

    // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ACT_GUILD_FACILITY_ACTIVE,      // ���ü� / Ȱ��ȭ

    // _NA_007086_20140318_MONSTERBOOK
    ACT_MONSTERBOOK_REGISTER,       // ���͵��� / ������ ���͵��

    // _NA_007750_20141113_WZDAT_MODIFY_LOG
    ACT_TOTAL_DROP_HEIM_FROM_MONSTER,   // WzDat / Ư�� �ʳ����� ���ͷκ��� ����� ���ӷ�
    ACT_MISSION_CLEAR_TRIGGER,          // WzDat / �̼� ���� Ʈ���� �ߵ���

    // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
    ACT_ATTENDANCE_ATTENDED,            // �⼮ / �⼮��
    ACT_ATTENDANCE_GOT_REWARD,          // �⼮ / �������        

    // _NA_008299_20150519_MAX_DAMAGE_LOG
    ACT_MAX_DAMAGE,                     // �ִ뵥���� / �ִ뵥���� �߻�

    // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    ACT_NOTIFICATION_MESSAGE,           // �˸��޼��� / �˸��޼��� �߻�

    //_NA_008334_20150608_SONNENSCHEIN
    ACT_SONNENSCHEIN_ENTER = 4092,      // �ҳٻ��� / ����
    ACT_SONNENSCHEIN_LEAVE,             // �ҳٻ��� / ����
    ACT_SONNENSCHEIN_DOMINATE_SUCCESS,  // �ҳٻ��� / ����(����)����

	// _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	ACT_ONE_DAYS_GUILD_MISSION_ENTER = 4095,		// ���� ���̼� / ����
	ACT_ONE_DAYS_GUILD_MISSION_LEAVE,				// ���� ���̼� / ����
	ACT_ONE_DAYS_GUILD_MISSION_DOMINATE_SUCCESS,	// ���� ���̼� / ���� ����

    ACT_MAX
};

enum eLOG_CHAT
{
	CHAT_NORMAL = 6000,			// �Ϲ� ä��
	CHAT_SHOUT,					// ��ġ�� ä��
	CHAT_PARTY,					// ��Ƽ ä��
	CHAT_GUILD,					// ��� ä��
	CHAT_PRIVATE,				// �ӼӸ�
	CHAT_FRIEND,				// ģ��ä��
	CHAT_CHANNEL,				// ä��ä��
};


enum eLOG_SNAPSHOT
{
	SNAPSHOT_REGULAR = 7000,		// �ֱ����� ������ �α�
	SNAPSHOT_LOGININVEN,			// �α��� �Ҷ� �κ��丮, â�� �ִ� ������ ����
	SNAPSHOT_LOGOUTINVEN,			// �α׾ƿ� �Ҷ� �κ��丮, â�� �ִ� ������ ����
	SNAPSHOT_WAREHOUSE_OEPN,		// â�� ���� â���� ������ ����
	SNAPSHOT_WAREHOUSE_CLOSE,		// â�� ������ â���� ������ ����
	SNAPSHOT_WAREHOUSE_OPEN_INVEN,	// â�� ���� �κ��� ������ ����
	SNAPSHOT_WAREHOUSE_CLOSE_INVEN,	// â�� ������ �κ��� ������ ����

	SNAPSHOT_MAX,
};

enum eLOG_GM
{
	GM_WHISPER = 8000,				// �ӼӸ�
	GM_ENTRANCE,					// ����
	GM_TRANSPARENT,					// ����
	GM_INVINCIBILITY,				// ����
	GM_DISCONNECT,					// ��������
	GM_CHARACTER_INFO,				// ĳ���� ����
	GM_ROOMINFO,					// ������
	GM_ROOMLIST,					// �渮��Ʈ
	GM_SPEED,						// �ӵ�
	GM_REBIRTH,						// ��Ȱ
	GM_WORLD_NOTICE,				// ����_����
	GM_CHANNEL_NOTICE,				// ä��_����
	GM_ZONE_NOTICE,			    	// ����_����
	GM_REMOVALITEM,					// ����������
	GM_REMOVALMONSTER,				// ��������
	GM_RECOVERY,					// ȸ��
	GM_LEVELUP,						// ������
	GM_STATPOINTUP,					// ���ݾ�
	GM_SKILLPOINTUP,				// ��ų����Ʈ��	
	GM_CREATEMONSTER,				// ���ͼ�ȯ
	GM_REMOVAL,						// ����
	GM_CREATEITEM,					// ����
	GM_CREATEHEIM,					// ����
	GM_KILL,						// ų
	GM_SERVERSHUTDOWN,				//< ���� ����
	//GM_CHUNTING_CLEAR,			//< �������� ���� �ʱ�ȭ
	//GM_CHUNTING_INCWIN,			//< �������� ����
	GM_ACQUISITION_ACCMULATE_POINT,	// GM ��ɾ ���� ��������Ʈ ȹ��
	GM_CONSUMPTION_ACCMULATE_POINT,	// GM ��ɾ ���� ��������Ʈ ����
	GM_ACQUISITION_ACCMULATE_POINT_BY_QUEST,	// GM ��ɾ ���� ��������Ʈ ����
};

enum eLOG_GUILD
{
	GUILD_LOG_START = 10000, //������, MAP �ε����� ����� ���� ����ǥ��

	GLD_CRTE,				//��� â��
	GLD_DTRY_WAIT,	//�ػ� ���
	GLD_DTRY_CACL,	//�ػ� ��û ���
	GLD_DTRY_CPLT,	//�ػ� �Ϸ�
	
	//����
	GLD_CORP_CRTE,	//â��
	GLD_CORP_DTRY,	//�ػ�

	//��� ���
	GLD_MEM_JOIN,		//����
	GLD_MEM_KICK,		//����
	GLD_MEM_OUT,		//Ż��
	
	// ķ��
	GLD_CAMP_CRT,		//â��
	GLD_CAMP_JOIN,		//����
	GLD_CAMP_KICK,		//����
	GLD_CAMP_OUT,		//Ż��
	GLD_CAMP_DTRY,	//�ػ�
	
	//������
	GLD_REL_SET,		//���輳��
	GLD_REL_CACL,		//��������

	//����
	GULD_LEV_UP,		//��巩ũ��

	//��å
	GLD_DUTY_CHG,	//����

	//����
	GLD_RIGT_DLGT,	//����

	//UP
	GLD_UP_GIVE,	//��� UP��

    // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    //��� ���� ��û
    GLD_JOIN_REQUEST,       //��� ���� ��û
    //��� �ü�
    GLD_FACILITY_UPGRADE,   //��� �ü� ���׷��̵�
    GLD_FACILITY_DOWNGRADE, //��� �ü� �ٿ�׷��̵�

	GUILD_LOG_MAX, //������, MAP �ε����� ����� ���� ��ǥ��
};

enum eLOG_ACCUMULATEPOINT
{
    ACCUM_CHANGE_YEAR,          // ���ӽð� ���� �⵵
    ACCUM_ACQUISITION_TIME,     // ���� �ð��� ���� ��������Ʈ ����
    ACCUM_ACQUISITION_QUEST,    // ����Ʈ�� ���� ��������Ʈ ����
    ACCUM_CONSUMPTION_ROULETTE, // ������ ���� �귿���ӿ� ���� ��������Ʈ ���.
};