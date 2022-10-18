#ifndef __TRIGGER_HEADER_H__
#define __TRIGGER_HEADER_H__

#pragma once

#include <Struct.h>

#pragma pack(push,1)

enum eTRIGGER_TYPE
{
	eTRIGGER_CONDITION							= 0,	// �����
	eTRIGGER_ACTION								= 1,	// �׼�
};

// ���� Ʈ���� Ÿ��  => 3DEngine������ Ʈ���� ������ �۾��� �̹� �Ʒ��� ���� ���̵�� �Ǿ� �ִ� ����(2005.4.12)
enum eTRIGGER_CONDITION_TYPE
{
	eTRIGGER_ALWAYS_CONDITION					= 0,	// �׻�
	eTRIGGER_COMPARE_SWITCH_CONDITION			= 1,	// ����ġ ��
	eTRIGGER_CLICK_OBJECT_CONDITION             = 2,	// ������Ʈ Ŭ��
	eTRIGGER_DAMAGE_OBJECT_CONDITION            = 3,	// ������Ʈ ������ ����
	eTRIGGER_COUNT_NPCKILL_CONDITION            = 4,	// ���� ��� ������
	eTRIGGER_PASS_TIME_CONDITION                = 5,	// �����ð��� �帧
	eTRIGGER_ENTER_AREA_CONDITION               = 6,	// Ư�� ������ ��
	eTRIGGER_OBTAIN_ITEM_CONDITION              = 7,	// ������ ȹ��
	eTRIGGER_MULTI_SWITCH_CONDITION             = 8,	// ���� ����ġ ��
	eTRIGGER_MISSION_POINT_CONDITION            = 9,	// �̼� ����Ʈ ����
	eTRIGGER_VARIABLE_COMPARE_CONDITION         = 10,   // ������
	eTRIGGER_TEST_QUEST_FLAG_CONDITION          = 11,   // ����Ʈ �Ϸ� üũ
	eTRIGGER_USER_CONDITION                     = 12,   // ���� ���� ����
	eTRIGGER_MONSTER_KILL	                    = 13,   // ���� ������ ������
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    eTRIGGER_NPC_ENTER_AREA_CONDITION           = 14,   // NPC�� Ư�� ������ ���ٸ�
    //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    eTRIGGER_COMPLETE_COLLECTION_CONDITION           = 15,   // �̼ǳ� ä�� Ʈ����
};

// ���� Ʈ���� Ÿ�� => 3DEngine������ Ʈ���� ������ �۾��� �̹� �Ʒ��� ���� ���̵�� �Ǿ� �ִ� ����(2005.4.12)
enum eTRIGGER_ACTION_TYPE
{
	eTRIGGER_CHANGE_SWITCH_ACTION               = 1000,	// ����ġ ����
	eTRIGGER_CHANGE_OBJECTANI_ACTION            = 1001,	// ������Ʈ �ִϸ��̼� ����
	eTRIGGER_CHANGE_PATHTILE_ACTION             = 1002,	// ��ã�� Ÿ�� �Ӽ� ����
	eTRIGGER_CHANGE_OBJECTSTATE_ACTION          = 1003,	// ������Ʈ ���� ����
	eTRIGGER_PLAY_EVENT_ACTION                  = 1004,	// �̺�Ʈ �÷���
	eTRIGGER_SET_ISATTACK_ACTION                = 1005,	// ���� ����/�Ұ� ����
	eTRIGGER_PORTAL_PLAYER_ACTION               = 1006,	// �÷��̾� ��Ż
	eTRIGGER_SET_OBJECTTHRUST_ACTION            = 1007,	// ������Ʈ �б� ����
	eTRIGGER_APPLY_DAMAGE_ACTION                = 1008,	// ������ ����
	eTRIGGER_REFLECT_DAMAGE_ACTION              = 1009,	// ������ �ݻ�
	eTRIGGER_CREATE_MONSTER_ACTION              = 1010,	// ���� ����
	eTRIGGER_DISPLAY_MSGBOX_ACTION              = 1011,	// �޼��� �ڽ� ���
	eTRIGGER_REMOVE_ITEM_ACTION                 = 1012,	// �κ��丮 ������ ����
	eTRIGGER_PRESERVER_ACTION                   = 1013,	// ����
	eTRIGGER_REWARD_PLAYER_ACTION               = 1014,	// �÷��̾� ���� 
	eTRIGGER_CLEAR_MISSION_ACTION               = 1015,	// �̼� ���
	eTRIGGER_PORTAL_RANDOM_ACTION               = 1016,	// ���� ��Ż
	eTRIGGER_ACTIVATE_TRIGGER_ACTION            = 1017,	// Ʈ���� Ȱ�� ��Ȱ��
	eTRIGGER_AREA_DAMAGE_ACTION                 = 1018,	// ����������
	eTRIGGER_OPERATE_SWITCH_ACTION              = 1019,	// ����ġ ����
	eTRIGGER_OPERATE_VARIABLE_ACTION            = 1020,	// ���� ����
	eTRIGGER_AREA_CHANGE_PATHTILE_ACTION        = 1021,	// ���� Ÿ�� �Ӽ����� 
	eTRIGGER_CREATE_MONSTER_WITH_DIRECTION      = 1022,	// ���� ���� (����)
	eTRIGGER_CHANGE_LIGHT                       = 1023,	// ���� ���� ����
	eTRIGGER_CHANGE_OBJECT_TYPE                 = 1024,	// ������Ʈ ���¸� �ٲ۴�.
	eTRIGGER_RANDOMMAP_MOVE                     = 1025, // ������ �̵�
	eTRIGGER_DROP_TREASURE                      = 1026, // ���� ������
	eTRIGGER_QUEST_CONDITION_COMPLETE_ACTION    = 1027, // �� ���� ����
	eTRIGGER_GIVE_QUEST_ACTION                  = 1028, // �� �ֱ�
	eTRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION     = 1029, // ����Ʈ ���� ����
	eTRIGGER_USER_ACTION                        = 1030, // ���� �׼�
	eTRIGGER_USE_CATEGORY_ACTION                = 1031, // ī�װ� ���
	eTRIGGER_SET_CANNOT_MOVE_ACTION             = 1032, // �̵� �Ұ� ����
	eTRIGGER_CREATE_MONSTER_GROUP_ACTION        = 1033, // ���� �׷� ����
	eTRIGGER_NOTICE_MESSAGE_BOX_ACTION          = 1034, // �ȳ� �޽��� �ڽ�
	eTRIGGER_SHOW_MONSTERUNIT_ACTION			= 1035, // ���� ��Ÿ�� / �����
	eTRIGGER_SHOW_MESSAGE_BOX_ACTION			= 1036, // �޽��� �ڽ��� ��Ÿ���� / ������� �Ѵ�.
	eTRIGGER_SKILL_GENERATE_ACTION				= 1037, // ��ų�߻� Ʈ����.
	eTRIGGER_TERRITORY_MOVE_ACTION				= 1038, // ���� �̵� Ʈ����.
	eTRIGGER_MON_RANDOM_SPAWN_ACTION			= 1039, // ���� ���� ����.
	eTRIGGER_SET_LAYER							= 1040, // [?]���� �����ش�.
};


// ���� ����
enum eTRIGGER_OPERATION_TYPE
{
	eTRIGGER_OPERATION_EQUAL = 0,				// "=="
	eTRIGGER_OPERATION_MORE,					// ">"
	eTRIGGER_OPERATION_LESS,					// "<"
	eTRIGGER_OPERATION_LESSTHAN,				// "<="
	eTRIGGER_OPERATION_MORETHAN,				// ">="
	eTRIGGER_OPERATION_NOTEQUAL,				// "!="
};

#define	INVALID_SWITCH_ID_VALUE			255

//////////////////////////////////////////////////////////////////////////
// Trigger Struct Define Start
//////////////////////////////////////////////////////////////////////////

struct TRIGGER_TYPE
{
	WORD				m_wTriggerType;		// Ʈ���� ����
	int                 m_iIDIndex;
};

// ���� Ʈ���� : ������
struct TRIGGER_ALWAYS_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_ALWAYS_CONDITION() { m_wTriggerType = eTRIGGER_ALWAYS_CONDITION; }
};

// ���� Ʈ���� : ����ġ ��			=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_COMPARE_SWITCH_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_COMPARE_SWITCH_CONDITION() { m_wTriggerType = eTRIGGER_COMPARE_SWITCH_CONDITION; }
	BYTE				m_bySwitchID;			// ����ġ ���̵�
	BYTE				m_byOperationType;		// ��������
	int					m_iValue;				// ��
};

// ���� Ʈ���� : ������Ʈ ����
struct TRIGGER_CLICK_OBJECT_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_CLICK_OBJECT_CONDITION() { m_wTriggerType = eTRIGGER_CLICK_OBJECT_CONDITION; }
	int					m_iObjectID;			// ������Ʈ ���̵�
};

// ���� Ʈ���� : ������Ʈ ������ ����
struct TRIGGER_DAMAGE_OBJECT_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_DAMAGE_OBJECT_CONDITION() { m_wTriggerType = eTRIGGER_DAMAGE_OBJECT_CONDITION; }
	int					m_iObjectID;			// ������Ʈ ���̵�
	float				m_fDamage;				// ������
};

// ���� Ʈ���� : ���� ���� ���� ���� ����
struct TRIGGER_COUNT_NPCKILL_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_COUNT_NPCKILL_CONDITION() { m_wTriggerType = eTRIGGER_COUNT_NPCKILL_CONDITION; }
	int					m_iAreaID;				// ���� ���̵�
	int					m_iMonsterType;			// ���� ����
	BYTE				m_byIsParty;			// ��Ƽ����(���� 0, ��Ƽ 1)
	int					m_iKillCount;			// ���� ��� ������
	BYTE                m_byMethod;             // ��� (0 : �Ϲ� 1 : ����)
};

// ���� Ʈ���� : �����ð� �帧			=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_PASS_TIME_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_PASS_TIME_CONDITION() { m_wTriggerType = eTRIGGER_PASS_TIME_CONDITION; }
	int					m_iPassTime;				// �帣�� �ð�
};

// ���� Ʈ���� : ������ ��
struct TRIGGER_ENTER_AREA_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_ENTER_AREA_CONDITION() { m_wTriggerType = eTRIGGER_ENTER_AREA_CONDITION; }
	int					m_iAreaID;				// ���� ���̵�
	int					m_iEnterCount;			// �� ��ü�� ��
	BYTE                m_bIsParty;             // ��Ƽ? ����?
};

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
// NPC�� Ư�� ������ ���ٸ�
struct TRIGGER_NPC_ENTER_AREA_CONDITION : public TRIGGER_TYPE 
{
    TRIGGER_NPC_ENTER_AREA_CONDITION() : area_id(0), npc_unit_id(0)
    {
        m_wTriggerType = eTRIGGER_NPC_ENTER_AREA_CONDITION;
    }
    CODETYPE area_id;
    CODETYPE npc_unit_id;
};

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
struct TRIGGER_COMPLETE_COLLECTION_CONDITION : public TRIGGER_TYPE 
{
    TRIGGER_COMPLETE_COLLECTION_CONDITION() : object_key(0)
    {
        m_wTriggerType = eTRIGGER_COMPLETE_COLLECTION_CONDITION;
    }
    CODETYPE object_key;
};

// ���� Ʈ���� : ������ ȹ��			=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_OBTAIN_ITEM_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_OBTAIN_ITEM_CONDITION() { m_wTriggerType = eTRIGGER_OBTAIN_ITEM_CONDITION; }
	int					m_iItemCode;			// ������ �ڵ�
	int                 m_iNumber;
};

// ���� Ʈ���� : ���� ����ġ
struct TRIGGER_MULTI_SWITCH_CONDITION : public TRIGGER_TYPE
{
	enum { _MAX_SWITCH_ID_NUM = 10	};
	TRIGGER_MULTI_SWITCH_CONDITION() { m_wTriggerType = eTRIGGER_MULTI_SWITCH_CONDITION; }

	BYTE				m_byaSwitchID[_MAX_SWITCH_ID_NUM];	// ����ġ ���̵��
	BYTE				m_byOperationType1;					// ��������1
	int					m_iValue1;							// ��1
	BYTE				m_byOperationType2;					// ��������2
	int					m_iValue2;							// ��2
};

// ���� Ʈ���� : �̼� ����Ʈ ����		=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_MISSION_POINT_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_MISSION_POINT_CONDITION() { m_wTriggerType = eTRIGGER_MISSION_POINT_CONDITION; }
	//int					m_iAreaID;				// ���� ���̵�
	int					m_iMissionPoint;		// ������ų �̼� ����Ʈ
	BYTE                m_bCondition;
};

// ���� Ʈ���� : ���� ��
struct TRIGGER_VARIABLE_COMPARE_CONDITION : public TRIGGER_TYPE
{
	TRIGGER_VARIABLE_COMPARE_CONDITION() { m_wTriggerType = eTRIGGER_VARIABLE_COMPARE_CONDITION; }
	DWORD               m_dwVariableID;
	BYTE                m_bOperator;
	int                 m_iParam;
};

struct TRIGGER_TEST_QUEST_FLAG_CONDITION   : public TRIGGER_TYPE
{
	TRIGGER_TEST_QUEST_FLAG_CONDITION() { m_wTriggerType = eTRIGGER_TEST_QUEST_FLAG_CONDITION; }
	ULONG              m_ulQuestID;
	BYTE               m_lScriptCode;
	

};

struct TRIGGER_USER_CONDITION              : public TRIGGER_TYPE
{
	TRIGGER_USER_CONDITION() { m_wTriggerType = eTRIGGER_USER_CONDITION; }
	int               m_iUser;	
};

struct TRIGGER_MONSTER_KILL              : public TRIGGER_TYPE
{
	TRIGGER_MONSTER_KILL()	{ m_wTriggerType = eTRIGGER_MONSTER_KILL; }
	WzID			m_wID;			//	��� ������ WzID
};


////////////////////////////////////////////////////////////////////////////////////////

// ���� Ʈ���� : ����ġ ���� ����		=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_CHANGE_SWITCH_ACTION : public TRIGGER_TYPE
{
	TRIGGER_CHANGE_SWITCH_ACTION() { m_wTriggerType = eTRIGGER_CHANGE_SWITCH_ACTION; }
	BYTE				m_bySwitchID;			// ����ġ ���̵�
	int					m_iValue;				// ��
};

// ���� Ʈ���� : ������Ʈ �ִϸ��̼� ����
struct TRIGGER_CHANGE_OBJECTANI_ACTION : public TRIGGER_TYPE
{
	enum eORDER { RIGHT = 0, REVERSE = 1, };
	TRIGGER_CHANGE_OBJECTANI_ACTION() { m_wTriggerType = eTRIGGER_CHANGE_OBJECTANI_ACTION; }
	int					m_iObjectID;			// ������Ʈ ���̵�
	DWORD				m_dwAniID;				// �ִϸ��̼� ���̵�
	BYTE				m_byDirection;			// ����(������ 0, ������ 1) 
};

// ���� Ʈ���� : Ÿ�ϼӼ� ����
struct TRIGGER_CHANGE_PATHTILE_ACTION : public TRIGGER_TYPE
{
	enum eFLAG { ENABLE = 0, DISABLE = 1 };
	TRIGGER_CHANGE_PATHTILE_ACTION() { m_wTriggerType = eTRIGGER_CHANGE_PATHTILE_ACTION; }
	int					m_iTileID;				// Ÿ�� ���̵�
	BYTE				m_byAttribute;			// �Ӽ�
};

// ���� Ʈ���� : ������Ʈ ���� ����		=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_CHANGE_OBJECTSTATE_ACTION : public TRIGGER_TYPE
{
	enum eSTATE { APPEAR = 0, DISAPPEAR = 1, REMOVE = 2 };
	TRIGGER_CHANGE_OBJECTSTATE_ACTION() { m_wTriggerType = eTRIGGER_CHANGE_OBJECTSTATE_ACTION; }
	int					m_iObjectID;			// ������Ʈ ���̵�
	BYTE				m_byState;				// ����
};

// ���� Ʈ���� : �̺�Ʈ �÷���
struct TRIGGER_PLAY_EVENT_ACTION : public TRIGGER_TYPE
{
	TRIGGER_PLAY_EVENT_ACTION() { m_wTriggerType = eTRIGGER_PLAY_EVENT_ACTION; }
	int					m_iEventID;				// �̺�Ʈ ���̵�
};

// ���� Ʈ���� : ���ݿ��� ����			=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_SET_ISATTACK_ACTION : public TRIGGER_TYPE
{
	TRIGGER_SET_ISATTACK_ACTION() { m_wTriggerType = eTRIGGER_SET_ISATTACK_ACTION; }
	BYTE				m_byTargetType;			// ���ݴ�� ����
	BYTE				m_byIsAttack;			// ���� ���� ����
};

// ���� Ʈ���� : �÷��̾� ��Ż
struct TRIGGER_PORTAL_PLAYER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_PORTAL_PLAYER_ACTION() { m_wTriggerType = eTRIGGER_PORTAL_PLAYER_ACTION; }
	int                 m_iMapIndex;            // �ʹ�ȣ
	int					m_iAreaID;				// ���� ���̵�
	BYTE                m_byTarget;             // �̵���ų ��� (0 : ���� 1: ��Ƽ)
	int                 m_iType;           
};

// ���� Ʈ���� : ������Ʈ �б⼳��		=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_SET_OBJECTTHRUST_ACTION : public TRIGGER_TYPE
{
	TRIGGER_SET_OBJECTTHRUST_ACTION() { m_wTriggerType = eTRIGGER_SET_OBJECTTHRUST_ACTION; }
	BYTE				m_byState;
};

// ���� Ʈ���� : ������ ����			=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_APPLY_DAMAGE_ACTION : public TRIGGER_TYPE
{
	TRIGGER_APPLY_DAMAGE_ACTION() { m_wTriggerType = eTRIGGER_APPLY_DAMAGE_ACTION; }
	BYTE				m_byDamageType;
	BYTE				m_byTargetType;
	float				m_fDamage;
};

// ���� Ʈ���� : ������ �ݻ�
struct TRIGGER_REFLECT_DAMAGE_ACTION : public TRIGGER_TYPE
{
	TRIGGER_REFLECT_DAMAGE_ACTION() { m_wTriggerType = eTRIGGER_REFLECT_DAMAGE_ACTION; }
	int					m_iObjectID;
	float				m_fDamage;				// ������
};


// ���� Ʈ���� : ���� ����
struct TRIGGER_CREATE_MONSTER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_CREATE_MONSTER_ACTION() { m_wTriggerType = eTRIGGER_CREATE_MONSTER_ACTION; }
	int					m_iMonsterCode;			// ���� �ڵ�
	int					m_iAreaID;				// ���� ���̵�
	BYTE				m_byIsParty;			// ��Ƽ����
	int					m_iCreateCount;			// ���� ���� ����
	int					m_iDelayTime;			// ���� ������ �ð�
	int					m_iLoopCount;			// ���� ���� ȸ��
    BYTE                m_byCreateDelete;       // ����/����
	int					m_iType;				// Ÿ��
	
};

// ���� Ʈ���� : �޼��� �ڽ� ���
struct TRIGGER_DISPLAY_MSGBOX_ACTION : public TRIGGER_TYPE
{
	TRIGGER_DISPLAY_MSGBOX_ACTION() { m_wTriggerType = eTRIGGER_DISPLAY_MSGBOX_ACTION; }
	int					m_iTextID;				// �ؽ�Ʈ ���̵�
	BYTE				m_byIsParty;			// ��Ƽ����
	int					m_iDisplayWindow;		// ����� â
};

// ���� Ʈ���� : �κ��丮 ������ ����		=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_REMOVE_ITEM_ACTION : public TRIGGER_TYPE
{
	TRIGGER_REMOVE_ITEM_ACTION() { m_wTriggerType = eTRIGGER_REMOVE_ITEM_ACTION; }
	int                m_iItemID;
	//BYTE               m_byAll; // (0: ������ �ϳ���,1: ������ ����)
	int                m_iNumber; // ����
};

// ���� Ʈ���� : ����				=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_PRESERVER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_PRESERVER_ACTION() { m_wTriggerType = eTRIGGER_PRESERVER_ACTION; }
};

// ���� Ʈ���� : �÷��̾� ����			=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_REWARD_PLAYER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_REWARD_PLAYER_ACTION() { m_wTriggerType = eTRIGGER_REWARD_PLAYER_ACTION; }
	int					m_iItemCode;			// ������ �ڵ�
	int                 m_iItemCount;           // ������ ����
	BYTE				m_byIsParty;			// ��Ƽ����(���� 0, ��Ƽ 1)
	BYTE                m_byDrop;         // �ִ¹��(0 : �κ��� �־,1 �ٴڿ� ����)
};

// ���� Ʈ���� : �̼� ���
struct TRIGGER_CLEAR_MISSION_ACTION : public TRIGGER_TYPE
{
	TRIGGER_CLEAR_MISSION_ACTION() { m_wTriggerType = eTRIGGER_CLEAR_MISSION_ACTION; }
	BYTE				m_byResultType;
};

// ���� Ʈ���� : ���� ��Ż			=> ���� ������ �ʿ� ����(2005.04.12)
struct TRIGGER_PORTAL_RANDOM_ACTION : public TRIGGER_TYPE
{
	enum { _MAX_AREA_ID_NUM = 8	};
	TRIGGER_PORTAL_RANDOM_ACTION() { m_wTriggerType = eTRIGGER_PORTAL_RANDOM_ACTION; }
	int                 m_iMapID;
	int                 m_iAreaID[_MAX_AREA_ID_NUM];
	BYTE                m_byRandom; // ���� ����������
	BYTE                m_byTarget; // (0: ���� 1: ��Ƽ)
};

// ���� Ʈ���� : Ʈ���� Ȱ��/��Ȱ��
struct TRIGGER_ACTIVATE_TRIGGER_ACTION : public TRIGGER_TYPE
{
	TRIGGER_ACTIVATE_TRIGGER_ACTION() { m_wTriggerType = eTRIGGER_ACTIVATE_TRIGGER_ACTION; }
	DWORD m_dwTriggerID; // Ʈ���� ���̵�
	BYTE  m_byFlag; // Ȱ��/��Ȱ��

};

// ���� Ʈ���� : ���� ������ �ο�
struct TRIGGER_AREA_DAMAGE_ACTION : public TRIGGER_TYPE
{
	TRIGGER_AREA_DAMAGE_ACTION() { m_wTriggerType = eTRIGGER_AREA_DAMAGE_ACTION; }
	int   m_iObjectIndex; // ������Ʈ ID
	float m_fRange; // ����
	BYTE  m_byTarget;   // ��ǥ (����/��Ƽ)
	BYTE  m_byDamageType; // ������ Ÿ��
	float m_fDamage; // ������

};

// ���� Ʈ���� : ����ġ ���� ����
struct TRIGGER_OPERATE_SWITCH_ACTION : public TRIGGER_TYPE
{
	TRIGGER_OPERATE_SWITCH_ACTION() { m_wTriggerType = eTRIGGER_OPERATE_SWITCH_ACTION; }
	BYTE  m_bySwitchID; // ����ġ���̵�
	BYTE  m_byOperation; // ��������
	int   m_iArgument; // �Ű�����

};

// ���� Ʈ���� : ����ġ ���� ����
struct TRIGGER_OPERATE_VARIABLE_ACTION : public TRIGGER_TYPE
{
	TRIGGER_OPERATE_VARIABLE_ACTION() { m_wTriggerType = eTRIGGER_OPERATE_VARIABLE_ACTION; }
	DWORD  m_dwVariableID; // �������̵�
	BYTE  m_byOperation; // ��������
	int   m_iArgument; // �Ű�����

};


// ���� Ʈ���� : ���� Ÿ�� �Ӽ����� 
struct TRIGGER_AREA_CHANGE_PATHTILE_ACTION  : public TRIGGER_TYPE
{
	TRIGGER_AREA_CHANGE_PATHTILE_ACTION () { m_wTriggerType = eTRIGGER_AREA_CHANGE_PATHTILE_ACTION; }
	int   m_iAreaID;  //  �������̵�
	BYTE  m_byCanWalk; // �������� ����
	

};

// ���� Ʈ���� : ���� ���� (����)
struct TRIGGER_CREATE_MONSTER_WITH_DIRECTION : public TRIGGER_TYPE
{
	TRIGGER_CREATE_MONSTER_WITH_DIRECTION() { m_wTriggerType = eTRIGGER_CREATE_MONSTER_WITH_DIRECTION; }
	int m_iMonsterID; // ������
	int m_iAreaID;
	BYTE m_byParty;
	int m_iMonsterCount;
	int m_iTimeInverval;
	int m_iTimeCount;
	WzVector m_vDir;
	int m_iType;

	
};

// ���� Ʈ���� : ���� ���� ����
struct TRIGGER_CHANGE_LIGHT : public TRIGGER_TYPE
{
	enum eSTATE { OFF = 0, ON = 1, TOGGLE = 2, }; // 0:���� 1:�Ҵ� 2:����Ѵ�
	TRIGGER_CHANGE_LIGHT() { m_wTriggerType = eTRIGGER_CHANGE_LIGHT; }
	int   m_iLightIndex;
	int   m_iTime;
	BYTE  m_byCondition;

};

// ���� Ʈ���� : ������Ʈ ���¸� �ٲ۴�.
struct TRIGGER_CHANGE_OBJECT_TYPE : public TRIGGER_TYPE
{
	TRIGGER_CHANGE_OBJECT_TYPE() { m_wTriggerType = eTRIGGER_CHANGE_OBJECT_TYPE; }
	int m_iObjectID;
	int m_iTime;
	int m_iType;
	

};

struct TRIGGER_RANDOMMAP_MOVE : public TRIGGER_TYPE
{
	TRIGGER_RANDOMMAP_MOVE() { m_wTriggerType = eTRIGGER_RANDOMMAP_MOVE; }
	int   m_iMapIndex[10];
	BYTE  m_bMethod; // ���� / ����
	WzID  m_AreaID; // �������̵�
	BYTE  m_bParty; // ���� / ��Ƽ
	int   m_iType; // Ÿ��

};

struct TRIGGER_DROP_TREASURE : public TRIGGER_TYPE
{
	TRIGGER_DROP_TREASURE() { m_wTriggerType = eTRIGGER_DROP_TREASURE; }
	int   m_iType;
	BYTE  m_bParty;
	BYTE  m_bInventory;

};

struct TRIGGER_QUEST_CONDITION_COMPLETE_ACTION  : public TRIGGER_TYPE  // �� ���� ����
{
	TRIGGER_QUEST_CONDITION_COMPLETE_ACTION() { m_wTriggerType = eTRIGGER_QUEST_CONDITION_COMPLETE_ACTION; }
	ULONG              m_ulQuestID;
	long               m_lScriptCode;

};

struct TRIGGER_GIVE_QUEST_ACTION                : public TRIGGER_TYPE  // �� �ֱ�
{
	TRIGGER_GIVE_QUEST_ACTION() { m_wTriggerType = eTRIGGER_GIVE_QUEST_ACTION; }
	ULONG              m_ulQuestID;
	BYTE               m_byParty; // 0 : ���� 1 : ��Ƽ
};

struct TRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION   : public TRIGGER_TYPE  // ����Ʈ ���� ����
{
	TRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION() { m_wTriggerType = eTRIGGER_SET_QUEST_COMPLETE_FLAG_ACTION; }

	ULONG              m_ulQuestID;
	BYTE               m_byFailed; // 0 : �Ϸ� 1 : ����

};

struct TRIGGER_USER_ACTION                      : public TRIGGER_TYPE  // ���� �׼�
{
	TRIGGER_USER_ACTION()                     { m_wTriggerType = eTRIGGER_USER_ACTION; }

	int               m_iUserID;
};
struct TRIGGER_USE_CATEGORY_ACTION              : public TRIGGER_TYPE  // ī�װ� ���
{
	TRIGGER_USE_CATEGORY_ACTION() { m_wTriggerType = eTRIGGER_USE_CATEGORY_ACTION; }

	int               m_iFieldID;
	char             m_szCategory[128];
};
struct TRIGGER_SET_CANNOT_MOVE_ACTION           : public TRIGGER_TYPE  // �̵� �Ұ� ����
{
	TRIGGER_SET_CANNOT_MOVE_ACTION() { m_wTriggerType = eTRIGGER_SET_CANNOT_MOVE_ACTION; }

	BYTE              m_byTarget; // 0 : ���� 1 : �� 2 : �Ѵ�
	int               m_iTime;

};
struct TRIGGER_CREATE_MONSTER_GROUP_ACTION      : public TRIGGER_TYPE  // ���� �׷� ����
{
	TRIGGER_CREATE_MONSTER_GROUP_ACTION() { m_wTriggerType = eTRIGGER_CREATE_MONSTER_GROUP_ACTION; }

	int               m_iGroupCode;
	int               m_iAreaID;
	BYTE              m_byParty; // 0 : ���� 1 : ��Ƽ
	int               m_iNumber;
};

struct TRIGGER_NOTICE_MESSAGE_BOX_ACTION        : public TRIGGER_TYPE  // �ȳ� �޽��� �ڽ�
{
	TRIGGER_NOTICE_MESSAGE_BOX_ACTION() { m_wTriggerType = eTRIGGER_NOTICE_MESSAGE_BOX_ACTION; }
	long               m_lScriptCode;
};

struct TRIGGER_SHOW_MONSTERUNIT_ACTION          : public TRIGGER_TYPE  // ���� ���� ��Ÿ���� �����
{
	TRIGGER_SHOW_MONSTERUNIT_ACTION() { m_wTriggerType = eTRIGGER_SHOW_MONSTERUNIT_ACTION; }
	int                m_iUnitID;
};

struct TRIGGER_SHOW_MESSAGE_BOX_ACTION          : public TRIGGER_TYPE  // �޽��� ��Ÿ���� / �������
{
	TRIGGER_SHOW_MESSAGE_BOX_ACTION() { m_wTriggerType = eTRIGGER_SHOW_MESSAGE_BOX_ACTION; }
	int					m_iTextID;				// �ؽ�Ʈ ���̵�
	BYTE				m_byIsParty;			// ��Ƽ����
	int					m_iDisplayWindow;		// ����� â
	int					m_iDisplaySwitch;		// ������ ���ΰ� �����ΰ�?
};

struct TRIGGER_SKILL_GENERATE_ACTION			: public TRIGGER_TYPE
{
	TRIGGER_SKILL_GENERATE_ACTION() {m_wTriggerType = eTRIGGER_SKILL_GENERATE_ACTION; }
	WzID m_wzObject;
	int m_iSkill;
};

struct TRIGGER_TERRITORY_MOVE_ACTION			: public TRIGGER_TYPE
{
	TRIGGER_TERRITORY_MOVE_ACTION() { m_wTriggerType = eTRIGGER_TERRITORY_MOVE_ACTION; }
	int m_iArea[8];
	BYTE m_btWho;
	BYTE m_btRandom;
};

struct TRIGGER_MON_RANDOM_SPAWN_ACTION			: public TRIGGER_TYPE
{
	TRIGGER_MON_RANDOM_SPAWN_ACTION() { m_wTriggerType = eTRIGGER_MON_RANDOM_SPAWN_ACTION; }
	int		 m_iMonID[5];
	int		 m_iMonRate[5];

	int		 m_iArea;
	BYTE	 m_btWho;
	int		 m_iMonMinNum;
	int		 m_iMonMaxNum;
	int		 m_iTime;
	int		 m_iRegenNum;
	WzVector m_wvDir;
};

struct TRIGGER_SET_LAYER			: public TRIGGER_TYPE
{
	TRIGGER_SET_LAYER() { m_wTriggerType = eTRIGGER_SET_LAYER; }
	int					m_iLayer;				// �� ��
};

//////////////////////////////////////////////////////////////////////////
// Trigger  Struct Define End
//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#endif // __TRIGGER_HEADER_H__
