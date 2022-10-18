#pragma once
#ifndef SOLARTRIGGER_CONDITION_INFO_H
#define SOLARTRIGGER_CONDITION_INFO_H
//==================================================================================================
/// ConditionInfo's class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2004. 12. 29
	@remark
		- ����� Ʈ����(ConditionInfo class)�� ��ӹ��� ���� ����� Ʈ���� Ÿ�� Ŭ������ ����
	@note
		- 
	@history
        - ~2011. 07. 20
        - 2011. 07. 20, waverix, code improvement
*/
//==================================================================================================
#include "TriggerCommon.h"

//-----------------------------------------------------------------------
//
// �ֻ��� Ŭ����
//
//-----------------------------------------------------------------------
class ConditionInfo
{
public:
    ConditionInfo(eCONDITION_TYPE trigger_type);
    virtual ~ConditionInfo();

    eCONDITION_TYPE TriggerType() const;
private:
    eCONDITION_TYPE trigger_type_;
};

inline ConditionInfo::ConditionInfo(eCONDITION_TYPE trigger_type)
    : trigger_type_(trigger_type)
{
}

inline ConditionInfo::~ConditionInfo()
{
}

inline eCONDITION_TYPE ConditionInfo::TriggerType() const
{
    return trigger_type_;
}

//-----------------------------------------------------------------------
//
// ���� Ŭ���� ���� ���ǹ�
//
//-----------------------------------------------------------------------
#define BEGIN_DECL_CONDITION(ConcreteCondition) \
    class ConcreteCondition##_INFO : public ConditionInfo \
    { \
    public: \
        ConcreteCondition##_INFO() : ConditionInfo(e##ConcreteCondition) {} \
        virtual ~ConcreteCondition##_INFO() {}

#define END_DECL_CONDITION \
    };

//-----------------------------------------------------------------------
//
// ���� Ŭ����
//
// Warnings :   ObjectID�� INT
//              ItemCode�� INT
// 
//-----------------------------------------------------------------------

namespace ns_trigger {
;

enum 
{
    PERSONAL = 0,
    PARTY = 1,
	ZONE = 2,
};
enum // NOTE: f110721.2L, unused type
{
    METHOD_GENERAL      = 0,
    METHOD_ACCUMULATE   = 1,
};

}; //end of namespace

BEGIN_DECL_CONDITION(CONDITION_ALWAYS)
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_COMPARE_SWITCH)
    __PROPERTY(BYTE, SwitchID);
    __PROPERTY(BYTE, OperationType);
    __PROPERTY(INT, Value);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_COMPARE_SWITCH_EX)
	__PROPERTY(BYTE, Target);
	__PROPERTY(BYTE, SwitchID);
	__PROPERTY(BYTE, OperationType);
	__PROPERTY(INT, Value);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_CLICK_OBJECT)
    __PROPERTY(INT, ObjectID);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_DAMAGE_OBJECT)
    __PROPERTY(INT, ObjectID);
    __PROPERTY(FLOAT, Damage);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_COUNT_NPCKILL)
    __PROPERTY(INT, AreaID);
    __PROPERTY(INT, MonsterCode);   //< �� ���� �� Code
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(INT, KillCount);     //< ���
    __PROPERTY(BYTE, Method);       //< ��� = { �Ϲ� , ���� }
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_PASS_TIME)
    __PROPERTY(INT, PassTime);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_ENTER_AREA)
    __PROPERTY(INT, AreaID);
    __PROPERTY(INT, PlayerCount);
    __PROPERTY(BYTE, IsParty);
END_DECL_CONDITION;

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
// NPC�� Ư�� ������ ���� ��
BEGIN_DECL_CONDITION(CONDITION_NPC_ENTER_AREA)
    __PROPERTY(CODETYPE, AreaID);
    __PROPERTY(CODETYPE, NPCUnitID);
END_DECL_CONDITION;

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
BEGIN_DECL_CONDITION(CONDITION_COMPLETE_COLLECTION)
public:
    __PROPERTY(CODETYPE, ObjectKey); // ��ü ������ ���� ����
END_DECL_CONDITION;

//_NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
BEGIN_DECL_CONDITION(CONDITION_SERVER_EVENT)
public:
    __PROPERTY(int, EventID); // �̺�Ʈ ������ ���� ����
END_DECL_CONDITION;

// _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
BEGIN_DECL_CONDITION(CONDITION_CHECK_TEAM)
public:
    __PROPERTY(int, TeamKey); // �̺�Ʈ ������ ���� ����
END_DECL_CONDITION;



BEGIN_DECL_CONDITION(CONDITION_HAVE_ITEM)
    __PROPERTY(INT, ItemCode);
    __PROPERTY(INT, ItemNum);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_MULTI_SWITCH)
public:  enum { _MAX_SWITCH_ID_NUM = 10 };
    __ARRAY_PROPERTY(BYTE, _MAX_SWITCH_ID_NUM, SwitchID);
    __PROPERTY(BYTE, OperationType1);
    __PROPERTY(INT, Value1);
    __PROPERTY(BYTE, OperationType2);
    __PROPERTY(INT, Value2);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_MISSION_POINT)
    __PROPERTY(INT, MissionPoint);  // �� ����Ʈ����
    __PROPERTY(BYTE, Flag);         // 0:�̻� 1:���� 2:����
END_DECL_CONDITION;

// [����IDX] �� [��] [int]�� �����ϸ�
BEGIN_DECL_CONDITION(CONDITION_COMPARE_VARIABLE)
    __PROPERTY(INT, VarID);
    __PROPERTY(BYTE, OperationType);
    __PROPERTY(INT, Value);
END_DECL_CONDITION;


// Quest �Ϸ� üũ Ʈ����, �ִ°����� Ȯ���� ��
BEGIN_DECL_CONDITION(CONDITION_QUEST_PROGRESS)
enum ePROGRESS { IN_PROGRESS, COMPLETED, DONTHAVE };
    __PROPERTY(/*QCODE*/ ULONG, QuestID);// Quest ID
    __PROPERTY(BYTE, Progress);          // (������, �Ϸ���)
END_DECL_CONDITION;

// ���� Ŭ���� ����(���� Ʈ����)
// [USER CLASS ID] �̸�, //0:���, 1:����Ŀ, 2:�巡��, 3:������, 4:��Ű��, 5:������Ż����Ʈ
BEGIN_DECL_CONDITION(CONDITION_CHECK_CLASS_ID)
    __PROPERTY(INT, ClassID);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_NPCKILL_FOR_UNITID)
    __PROPERTY(WzID, MonsterUnitID);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_LIVE_PARTY_MEMBER)
public:
    enum{ MEMBER_DIE = 0, MEMBER_LIVE = 1 };
    __PROPERTY(BYTE, LiveFlag); // 0:��� 1:����
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_CHECK_OBJECT_HP)
public:
    __PROPERTY(DWORD, dwObjKey);    // mapobject
    __PROPERTY(BYTE,  btHPPercentage);
END_DECL_CONDITION;


BEGIN_DECL_CONDITION(CONDITION_DESTROY_OBJECT)
public:
    __PROPERTY(DWORD, dwObjKey);
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_CHECK_OBJECTKIND_HP)
public:
    __PROPERTY(BYTE,  byObjectType);    // ����� Ÿ��(����/����/������Ʈ);
    __PROPERTY(DWORD, dwObjectKey);     // ��� ��ü�� Ű��
    __PROPERTY(BYTE,  btHPPercentage);  // HP ����
END_DECL_CONDITION;

BEGIN_DECL_CONDITION(CONDITION_MONSTER_ENTER_AREA)
public:
    __PROPERTY(int, AreaID);    // ����ID
    __PROPERTY(int, MonsterCode);  // �����ڵ�
    END_DECL_CONDITION;

#endif //SOLARTRIGGER_CONDITION_INFO_H