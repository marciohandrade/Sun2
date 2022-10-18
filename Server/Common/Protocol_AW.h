#pragma once

// CHANGES: f110914.1L, change the category index, based on _NA000000_CW_PROTOCOL_UNIFICATION
/// ī�װ�
enum eAW_CATEGORY
{
    AW_CONNECTION           = CATEGORY_GENERATOR_213,
    AW_USER_STATUS          = CATEGORY_GENERATOR_214,
    AW_PARTY                = CATEGORY_GENERATOR_215,
    AW_GM                   = CATEGORY_GENERATOR_216,
    AW_CHAO                 = CATEGORY_GENERATOR_217,
    AW_ITEM                 = CATEGORY_GENERATOR_218,
    //__NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__ : Ÿ���� ���
    // (�ּ� ó���Ǿ� �ִ� �� �� Ȱ��ȭ ���� �������� ���ݵ��� �ʵ��� �ڷ� �̵� AW_MAX�� �����Ǵ� �� ������)
    AW_OPERATION            = CATEGORY_GENERATOR_219,
    AW_FRIENDINVITATE       = CATEGORY_GENERATOR_220, //_KR_0_20091021_FIRENDINVITATION_EXTEND
    //
    AW_MAX                  = CATEGORY_GENERATOR_211,
};

// used by _KR_0_20091021_FIRENDINVITATION_EXTEND
enum eAW_FRIENDINVITATE_PROTOCOL		//ģ�� �ʴ� �̺�Ʈ
{
	AW_FRIENDINVITATE_INVITATELIST_CMD		=1,	//W->A ���� �ʴ��� ĳ���� ���	
    AW_FRIEND_INVITATIONR_INSERT_CMD,           //D->A->W ��õ�� ģ���� ĳ���� ����������
};

/// ��������
enum eAW_CONNECTION_PROTOCOL
{
	AW_AGENTSERVER_INFO_SYN		= 1,

	AW_PREPARE_NEW_USER_SYN,
	AW_PREPARE_NEW_USER_ACK,
	AW_PREPARE_NEW_USER_NAK,

	AW_ABUSING_USER_VERIFY_QUERY_CMD,
    AW_CONNECTION_GROUND_CMD, // NOTE: f110617.1L
    AW_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN, // NOTE: f110727.1L
};

enum eAW_USER_STATUS_PROTOCOL
{
	AW_USER_SET_CHARNAME_SYN	= 1,
	AW_USER_UNSET_CHARNAME_SYN,
	AW_USER_ENTER_ZONE_SYN,
	AW_USER_LEAVE_ZONE_SYN,
	AW_USER_LOGOUT,
	AW_USER_RESTORE_SYN,				// ���� ������ ��� ���� ��û
	AW_USER_RESTORE_CMD,				// ���� ���ӵ� ����� ���
	AW_USER_LEVELUP_CMD,				// ����� ������
	AW_USER_STATUS_INTERCEPTION_SYN,
	AW_USER_STATUS_INTERCEPTION_ACK,
	AW_USER_STATUS_INTERCEPTION_NAK,
    //__NA_0_20090627_HEIM_ACQUISITION_S2S_NOTIFICATION
    AW_USER_STATUS_HEIM_ACQUISITION_NTF, // ���� ȹ�� ����
    //__NA_0_20090627_HEIM_CONSUMPTION_S2S_NOTIFICATION
    AW_USER_STATUS_HEIM_CONSUMPTION_NTF, // ���� �Һ� ����
    //__KR_001355_20090608_GROUPINGSYSTEM_GW
    AW_STATUS_GROUP_SET_MEMBERINFO_CMD,    //�׷� ��� ���� ����
    AW_STATUS_GROUP_EXP_ACQUISITION_CMD,    //����ġ ȹ�� ����
    AW_STATUS_GROUP_ADDEXP_CMD,             //����ġ �߰� ��û
    AW_STATUS_GROUP_ADDMONEY_CMD,           //���� �߰� ��û
    AW_STATUS_GROUP_START_CMD,              //�׷� �ý��� ���� ����
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM
    AW_USER_LOTTO_REWARD_SYN,               // ���� �ζ� ��� ���� ��û
    AW_USER_LOTTO_REWARD_ACK,               // ���� �ζ� ��� ���� ����
    //__NA_001359_20090619_HEIM_LOTTO_SYSTEM && _NA_0_20100115_HEIM_LOTTO_TICKET_PURCHASE
    AW_USER_LOTTO_TICKET_PURCHASE_SYN,      // ���� �ζ� ���ǿ��� ���� ���� ��û
    AW_USER_LOTTO_TICKET_PURCHASE_ACK,      // ���� �ζ� ���ǿ��� ���� ���� ����
    //_PCBANG_POLICY_CHANGE_POST_EXPIRED
    AW_USER_STATUS_PCBANG_BILLING_NTF,      // PC�� ���� ���� ��ȯ
    //_KR_001385_20091202_DOMINATION_RETURN_HEIM
    AW_USER_DOMINATION_RETURNHIEM_CMD,      // ���� ������ ��� �ݾ� ���� �ޱ�
    //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION 
    AW_USER_SYSTEMMEMO_SEND_CMD,            // �ý��� ���� ��û
    //_NA002676_WORLD_SERVER_RENEWAL
    AW_USER_STADING_POSITION_CMD,
};

enum eAW_PARTY_PROTOCOL
{
	AW_PARTY_MAKE_SYN	= 1,			// ��Ƽ ����
	AW_PARTY_JOIN_MEMBER_SYN,			// ��Ƽ ����
	AW_PARTY_WITHDRAW_MEMBER_SYN,		// ��Ƽ Ż��
	AW_PARTY_DESTROY_SYN,				// ��Ƽ ��ü
    AW_PARTY_CHANGE_MASTER_SYN,         // ��Ƽ�� ����
    AW_PARTY_FINDING_JOIN_SYN,          // ��Ƽ ������ ���� ��Ƽ ���� ��û
    AW_PARTY_FINDING_JOIN_RESULT_ACK,   // ��Ƽ ������ ���� ��Ƽ ���� ��û�� ���� ó�� ��� ����
    AW_PARTY_ROOM_KEY_SYN,              // ��Ʋ�� ��Ƽ�� ���ȣ ����
};

enum eAW_GM_PROTOCOL
{
	AW_GM_WHISPER_SET_CMD	= 1,		// �Ӹ� ��/��
	AW_GM_USERCHAT_BLOCK_CMD,			// ä�� ����/���
	AW_GM_NOTICE_CMD,					// ����

    // __NA_001359_20090619_HEIM_LOTTO_SYSTEM
    AW_GM_LOTTO_NUM_CMD,                // ���� �ζ� ���� ȸ�� ��÷ ��ȣ ���̱� ���
    AW_GM_LOTTO_SETTLE_CMD,             // ���� �ζ� ���� ��÷ ���
    AW_GM_LOTTO_FUND_CMD,               // ���� �ζ� ��� ���� ���
};

enum eAW_CHAO_PROTOCOL
{
	AW_CHAO_REQUEST_REVENGEINFO_SYN = 1,	// ������ ���� ��û(A->W)
	AW_CHAO_REQUEST_REVENGEINFO_ACK,		// ������ ����(W->A)
	AW_CHAO_REQUEST_REVENGEINFO_NAK,		// ������ ���� ���� ����(W->A)

	AW_CHAO_REMOVE_REVENGE_CHAR_CMD,		// ������ ��� ����(Ŭ���̾�Ʈ�� �������� ��)(W->A)

	AW_CHAO_REMOVE_REVENGE_CHAR_SYN,		// ������ ��� �Ѹ� ����(PK�� ���� �������� ��)(A->W)
	AW_CHAO_REMOVE_REVENGE_CHAR_ACK,		// ������ ��� �Ѹ� ���� ����(W->A)
	AW_CHAO_REMOVE_REVENGE_CHAR_NAK,		// ������ ��� �Ѹ� ���� ����(W->A)

	AW_CHAO_ADD_REVENGE_CHAR_SYN,			// ������ ��� �߰� ��û(A->W)
	AW_CHAO_ADD_REVENGE_CHAR_ACK,			// ������ ��� �߰� ����(W->A)
	AW_CHAO_ADD_REVENGE_CHAR_NAK,			// ������ ��� �߰� ����(W->A)
};

enum eAW_ITEM_PROTOCOL
{
	AW_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD = 1,
	AW_ITEM_CHANGE_SPECIAL_ITEM_SYN,			// ���� ������ ���� ��û
	AW_ITEM_SPECIAL_ITEM_CLEAR_CMD,
    AW_ITEM_USE_DECREE_ITEM_SYN,				// �ع決�� ���� ���[�߱����� 1ȸ ���]	
	AW_ITEM_USE_DECREE_ITEM_ACK,
	AW_ITEM_USE_DECREE_ITEM_NAK,

    // _NA_20101011_HEIM_LOTTO_CASH_TICKET
    AW_ITEM_USE_HEIM_LOTTO_TICKET_SYN,      // ������� ĳ�� ����� ��� ��û (A->W)
    AW_ITEM_USE_HEIM_LOTTO_TICKET_ACK,      // ������� ĳ�� ����� ��� ���� (W->A)
};


// USERCOUNT_SYN�� ���� ���� ��ü���� �����ϱ�� �����ϰ�, USERMONEY_SYN�� GM���� �Ű���
/*
enum eAW_USER_OPERATION_PROTOCOL
{
	AW_USERCOUNT_SYN	= 1,
	AW_USERMONEY_SYN,					// ���� �� ������ ��
};
*/

enum eAW_OPERATION_PROTOCOL
{
	AW_OPERATION_ROOM_CTRL_CMD		,	//__NA000941_080204_TASK_SURVIVAL_SOLOING_QUEST_SYSTEM__ : Ÿ���� ���
};
