#pragma once
#ifndef SOLARTRIGGER_ACTION_INFO_H
#define SOLARTRIGGER_ACTION_INFO_H
//==================================================================================================
/// ActionInfo's class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2004. 12. 29
    @remark
        - �׼� Ʈ����(ActionInfo class)�� ��ӹ��� ���� �׼� Ʈ���� Ÿ�� Ŭ������ ����
    @note
        - 
    @history 
        - ~2011. 07. 20
        - 2011. 07. 20, waverix, code improvement
*/
//==================================================================================================
#include "ConditionInfo.h"
#include "TriggerCommon.h"

//-----------------------------------------------------------------------
//
// �ֻ��� Ŭ����
//
//-----------------------------------------------------------------------
class ActionInfo
{
public:
    ActionInfo(eACTION_TYPE wType);
    virtual ~ActionInfo();

    eACTION_TYPE TriggerType() const;
private:
    eACTION_TYPE trigger_type_;
};

inline ActionInfo::ActionInfo(eACTION_TYPE trigger_type)
    : trigger_type_(trigger_type)
{
}

inline ActionInfo::~ActionInfo()
{
}

inline eACTION_TYPE ActionInfo::TriggerType() const
{
    return trigger_type_;
}

//-----------------------------------------------------------------------
//
// ���� Ŭ���� ���� ���ǹ�
//
//-----------------------------------------------------------------------
#define BEGIN_DECL_ACTION(ConcreteAction) \
    class ConcreteAction##_INFO : public ActionInfo \
    { \
    public: \
        ConcreteAction##_INFO() : ActionInfo(e##ConcreteAction) {} \
        virtual ~ConcreteAction##_INFO() {}

#define END_DECL_ACTION \
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
    CANMOVE,
    CANNOTMOVE,
};      //< ACTION_CHANGE_PATHTILE�� Attribute �Ӽ� ���� 

}; //end of namespace

/*
BEGIN_DECL_ACTION(ACTION_CHANGE_SWITCH)
    __PROPERTY(BYTE, SwitchID)
    __PROPERTY(INT, Value)
END_DECL_ACTION;
*/
BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECTANI)
    __PROPERTY(INT, ObjectID);
    __PROPERTY(DWORD, WzAniID);
    __PROPERTY(BYTE, Direction);// ����(������ 0, ������ 1) 
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_CHANGE_PATHTILE)
    __PROPERTY(INT, TileID);
    __PROPERTY(BYTE, Attribute);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECTSTATE)
public:enum { APPEAR, DISAPPEAR, REMOVE, };
    __PROPERTY(INT, ObjectID);
    __PROPERTY(BYTE, State);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_PLAY_EVENT)
    __PROPERTY(INT, EventID);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_SET_ISATTACK)
public:
    enum { 
        TARGET_TYPE_USER = 0,
        TARGET_TYPE_MONSTER, 
        TARGET_TYPE_ALL,
        CAN_NOT_ATTACK = 0,
        CAN_ATTACK
    };
    __PROPERTY(BYTE, TargetType);// ���ݴ�� ����(0 : ����, 1: ����, 2 : �Ѵ�)
    __PROPERTY(BYTE, IsAttack);  // ���� ���� ����(0 : ���ݺҰ���, 1 : ���ݰ���)

END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_PORTAL_PLAYER)
public:
    enum {
        _MAX_MAP_NAME_LENGTH = 128, 
        RELATEDVILLAGE  = 0, 
        VILLAGECODE     = 1,
        FIELDCODE       = 2,
        TARGET_RETURN   = 3, //__NA001310_BATTLEZONE_PORTAL_RETURN_TARGET_MOVE_SUPPORT__
    };
    __PROPERTY(INT, MapCode);
    __PROPERTY(INT, AreaID);
    __PROPERTY(BYTE, IsParty);  // �̵���ų ��� (0 : ���� 1: ��Ƽ)
    __PROPERTY(INT, Type);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_SET_OBJECTTHRUST)
    __PROPERTY(BYTE, State);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_APPLY_DAMAGE)
public:
    enum {
        TARGET_FIELD    = 0,
        TARGET_PERSONAL = 1,
    };

    __PROPERTY(BYTE, DamageType);
    __PROPERTY(BYTE, TargetType);
    __PROPERTY(FLOAT, Damage);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_REFLECT_DAMAGE)
    __PROPERTY(INT, ObjectID);
    __PROPERTY(FLOAT, Damage);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER)
    __PROPERTY(INT, MonsterCode);
    __PROPERTY(INT, AreaID);
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(INT, CreateCount);           // ���� ���� ����
    __PROPERTY(INT, DelayTime);         // ���� �ð� ����
    __PROPERTY(INT, LoopCount);         // ���� ���� ȸ��
    __PROPERTY(BYTE, CreateDestroy);    // ���� ����/����
    __PROPERTY(INT, Type);                  // Ÿ�� (���� �� �����̳�)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_DISPLAY_MSGBOX)
    enum eENUM
    {
        TARGET_PERSONAL         = 0,
        TARGET_FIELD            = 1,
        TARGET_OFFENCE          = 2,
        TARGET_DEFENCE          = 3,
    };

    __PROPERTY(INT, TextID);
    __PROPERTY(BYTE, Target);       // 0:���� 1:��ü 2:���� 3:����
    __PROPERTY(INT, DisplayWindow); // ����� â
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_REMOVE_ITEM)
    __PROPERTY(INT, ItemCode);
    __PROPERTY(INT, ItemNum);   // ������ ����
END_DECL_ACTION;

/*
BEGIN_DECL_ACTION(ACTION_PRESERVER)
END_DECL_ACTION;
*/

BEGIN_DECL_ACTION(ACTION_REWARD_PLAYER)
public:enum { INVENTORY=0, DROPFIELD=1, };
    __PROPERTY(INT, ItemCode);
    __PROPERTY(INT, Count);
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(BYTE, IsDrop);   // �ִ¹��(0 : �κ��� �־,1 �ٴڿ� ����)
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_CLEAR_MISSION)
    __PROPERTY(BYTE, ResultType);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_PORTAL_RANDOM)
public: enum { _MAX_AREA_ID_NUM = 8, };
    __PROPERTY(INT, MapID);
    __ARRAY_PROPERTY(INT, _MAX_AREA_ID_NUM, AreaID);
    __PROPERTY(BYTE, Random);               // ���� ����������
    __PROPERTY(BYTE, Target);               // (0: ���� 1: ��Ƽ)
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_NPC_RANDOM_AREA_MOVE)
public: enum { _MAX_AREA_ID_NUM = 8, };
    __ARRAY_PROPERTY(INT, _MAX_AREA_ID_NUM, AreaID);
    __PROPERTY(BYTE, MoveOrWarp);           // (0: move 1: warp)
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_ACTIVATE_TRIGGER)
    __PROPERTY(DWORD, WzTriggerID);
    __PROPERTY(BYTE, Flag); // 0 : Ȱ��, 1 : ��Ȱ��
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_AREA_DAMAGE)
    __PROPERTY(DWORD, ObjectIndex);
    __PROPERTY(FLOAT, Range);
    __PROPERTY(BYTE, Target);       // ��ǥ (����/��Ƽ)
    __PROPERTY(BYTE, DamageType);   // ������ Ÿ��
    __PROPERTY(FLOAT, Damage);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_OPERATE_SWITCH)
    __PROPERTY(BYTE, SwitchID);
    __PROPERTY(BYTE, OperationType);
    __PROPERTY(INT, Argument);  // �Ű�����
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_OPERATE_SWITCH_EX)
	__PROPERTY(BYTE, Target);               // (0: ���� 1: ��Ƽ)
	__PROPERTY(BYTE, SwitchID);
	__PROPERTY(BYTE, OperationType);
	__PROPERTY(INT, Argument);  // �Ű�����
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_OPERATE_VARIABLE)
    __PROPERTY(INT, VarID);
    __PROPERTY(BYTE, OperationType);
    __PROPERTY(INT, Argument);  // �Ű�����
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ROAD_CONNECT); // �� ������ ����/���� �Ѵ�
    __PROPERTY(INT, AreaIndex);
    __PROPERTY(BYTE, CanGo);    // �� �� �ִ�=0, ����=1
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CREATE_DIR_MONSTER)
    __PROPERTY(INT, MonsterCode);
    __PROPERTY(INT, AreaID);
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(INT, CreateCount);   // ���� ���� ����
    __PROPERTY(INT, DelayTime);     // ���� �ð� ����
    __PROPERTY(INT, LoopCount);     // ���� ���� ȸ��
    __PROPERTY(FLOAT, DirX);        // x����
    __PROPERTY(FLOAT, DirY);        // y����
    __PROPERTY(FLOAT, DirZ);        // z����
    __PROPERTY(INT, Type);          // Type[0]
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CHANGE_LIGHT)
    __PROPERTY(INT, LightIndex);// �� �ε����� ����Ʈ��
    __PROPERTY(INT, DelayTime); // �̸�ŭ�� �ð� �Ŀ�
    __PROPERTY(BYTE, Status);   // 0:���� 1:�Ҵ� 2:����Ѵ�
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECT_TYPE)
    __PROPERTY(INT, ObjectID);
    __PROPERTY(INT, DelayTime);
    __PROPERTY(INT, ObjectType);
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_QUEST_SATISFACTION)
    __PROPERTY(ULONG, QuestID);
    __PROPERTY(LONG, ScriptCode);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_QUEST_GRANT)
    __PROPERTY(ULONG, QuestID);
    __PROPERTY(BYTE, PartyState);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_QUEST_SETUP)
    __PROPERTY(ULONG, QuestID);
    __PROPERTY(BYTE, Completed);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_USE_CATEGORY)
    enum { _MAX_USE_CATEGORY_NAME = 128 };
    __PROPERTY(INT, FieldID);
    __ARRAY_PROPERTY(TCHAR, _MAX_USE_CATEGORY_NAME, CategoryName)
END_DECL_ACTION;

// ������ �����ϴ� �׼� Ʈ����
//BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER)
//__END_DECL_ACTION

BEGIN_DECL_ACTION(ACTION_HOLD_CHARACTER)
    __PROPERTY(BYTE, Who);
    __PROPERTY(INT, HoldingTime);
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER_GROUP)
    __PROPERTY(INT, GroupID);
    __PROPERTY(INT, AreaID);
    __PROPERTY(BYTE, PartyState);
    __PROPERTY(INT, GroupCount);
END_DECL_ACTION;

// <THINKING & WARNING><Waverix>
//  ��Ÿ��/�����
//  ��Ÿ����/�������
//  --> �� �ΰ����� ���� �ݴ� ������ �ش� �Ӽ��� ǥ���ϰ� �ȴ�. ������ ��
//  ShowOrHide�� ��Ī ������ �ʿ䰡 �ִ�.

// ���� ��Ÿ��/����� (���������� ���� ����/���ſ� ����)
BEGIN_DECL_ACTION(ACTION_SHOW_OR_HIDE_UNIT)
    enum { SHOW=0,HIDE=1,};
    __PROPERTY(WzID, MonsterUnitID);
    __PROPERTY(INT, IsHide); //0: ��Ÿ����, 1:�������
END_DECL_ACTION;


BEGIN_DECL_ACTION(ACTION_SHOW_GUIDE_MSGBOX)
    __PROPERTY(LONG, HelpIndex);
END_DECL_ACTION;

// �޽��� �ڽ��� ��Ÿ��/�����
BEGIN_DECL_ACTION(ACTION_SHOW_OR_HIDE_MSGBOX)
    enum { SHOW=1,HIDE=0,};
    __PROPERTY(INT, TextID);
    __PROPERTY(BYTE, IsParty);
    __PROPERTY(INT, DisplayWindow); // ����� â
    __PROPERTY(INT, IsShow);        //1: ��Ÿ����, 0:�������
END_DECL_ACTION;


// ���� ���� ����
BEGIN_DECL_ACTION(ACTION_RANDOM_MONSTER_SPAWN)
    __PROPERTY(INT, MonsterID_1);           // 1-5 ����
    __PROPERTY(INT, MonsterRate_1);     // 1-5 ���� ����Ȯ��
    __PROPERTY(INT, MonsterID_2);
    __PROPERTY(INT, MonsterRate_2);
    __PROPERTY(INT, MonsterID_3);
    __PROPERTY(INT, MonsterRate_3);
    __PROPERTY(INT, MonsterID_4);
    __PROPERTY(INT, MonsterRate_4);
    __PROPERTY(INT, MonsterID_5);
    __PROPERTY(INT, MonsterRate_5);
    __PROPERTY(INT, AreaID);                // ���� ���� ����
    __PROPERTY(BYTE, IsParty);          // ���� ? ��Ƽ ?
    __PROPERTY(INT, MinCreateMonsterCnt);   // �ּ� ���� ���� ����
    __PROPERTY(INT, MaxCreateMonsterCnt);   // �ּ� ���� ���� ����
    __PROPERTY(INT, RegenTime);         // ���� �ð� ����
    __PROPERTY(INT, MaxRegenCount);     // ���� �� ���� ȸ��
    __PROPERTY(FLOAT, DirX);                // x����
    __PROPERTY(FLOAT, DirY);                // y����
    __PROPERTY(FLOAT, DirZ);                // z����
END_DECL_ACTION;

//#ifdef __NA_000104_20070123_TOP_OF_TRIAL
BEGIN_DECL_ACTION(ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR)
    __PROPERTY(INT, Floor); // ������ ž �� ��
END_DECL_ACTION;
//#endif //

//////////////////////////////////////////////////////////////////////////
BEGIN_DECL_ACTION(ACTION_RANDOM_MAP_MOVE)   // ���� �� �̵�
public:
    enum eENUM
    {
        _MAX_FIELDCODE_ID       = 10,       // �ʵ� �ڵ� �迭 ũ��
        RNDMODE_RANDOM          = 0,
        RNDMODE_SEQUENCIAL      = 1,
        TARGET_PERSONAL         = 0,
        TARGET_PARTY            = 1,
        RETMAP_VILLAGE          = 0,
        RETMAP_STARTINGVILLAGE  = 1,
        RETMAP_BATTLEMAP        = 2,
    };
    __ARRAY_PROPERTY(INT, _MAX_FIELDCODE_ID, FieldID);
    __PROPERTY(BYTE, Random);               // ����, ����������
    __PROPERTY(WzID, AreaID);               // ���� ID
    __PROPERTY(BYTE, Target);               // (0: ���� 1: ��Ƽ)
    __PROPERTY(INT, Type);              // (0: ����, 1:�ͼӸ���(?), 2:��Ʋ��
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_RANDOM_AREA_MOVE) // ������ ���� �̵� ��Ų��
public:
    enum eENUM
    {
        _MAX_AREA_ID            = 8,
        TARGET_PERSONAL         = 0,
        TARGET_PARTY            = 1,
        PROC_EACH               = 0,
        PROC_SAMENESS           = 1,
    };
    __ARRAY_PROPERTY(INT, _MAX_AREA_ID, AreaIDX);   // �̵���ų ���� IDs
    __PROPERTY(BYTE, Target);   // (0: ���� 1: ��Ƽ)
    __PROPERTY(BYTE, ProcType); // (0: ����, 1: ����)
END_DECL_ACTION;
//////////////////////////////////////////////////////////////////////////


BEGIN_DECL_ACTION(ACTION_REMOVE_MONSTER)// ����.
    __PROPERTY(INT, MonsterCode);       // �����ڵ�
    __PROPERTY(BYTE, IsExpItemDrop);    // ����ġ�׾����۵巴 ���� (0: ���Ѵ�. 1: �Ѵ�)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ADD_EXP)
    __PROPERTY(BYTE, IsParty);  // �����̳� ��Ƽ�� (0 : ����, 1:��Ƽ)
    __PROPERTY(DWORD, Exp);     // Exp �ִ� ����ġ.
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_MONSTER_STAT_UP_OR_DOWN)
public: 
    enum
    {
        eOPERATION_MINUS = 0,                   // "-"
        eOPERATION_PLUS  = 1,                   // "+"
    };

     __PROPERTY(INT, MonsterCode);  // �����ڵ�
     __PROPERTY(WORD, AbilityID);   // �����Ƽ ID
     __PROPERTY(int, AbilityParameter1); // Ÿ�Լ��� : 0. ??, 1. ��ġ, 2. ����ġ ��� ����, 3. �ִ�ġ ��� ����, 4. ��� ����, 5. ��ġ�� �������� ���(�̵��ӵ���)
     __PROPERTY(int, AbilityParameter2); // ��ġ/������
     __PROPERTY(BYTE, OperationType);   // ������ 1 : + , 0 : - 
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_RANDUM_TRIGER_ON_OR_OFF)
public:
    enum 
    { 
        _TRIGGER_STATE_OFF  = 0,
        _TRIGGER_STATE_ON   = 1,
        _MAX_TRIGGER_ID     = 8 
    };      

    __ARRAY_PROPERTY(DWORD, _MAX_TRIGGER_ID, WzTriggerID);
    __PROPERTY(BYTE, TriggerState);

END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_PLAY_MUSIC)
public: 
    enum
    {
        TARGET_PERSONAL         = 0,
        TARGET_FIELD            = 1,
    };

    __PROPERTY(BYTE, Target);                   // (0: ���� 1: �ʵ���ü)
    __PROPERTY(INT, MusCode);                   // (�����ڵ�)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_DOWN_OBJETC_HP)
public:                                             // �� ������Ʈ�� HP�� %�� ��´�. (map_object �� ����� NPC)
    __PROPERTY(DWORD, dwObjKey);                    // mapobjectkey
    __PROPERTY(BYTE,  btHPPercentage);          // �϶� ��ġ
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CHANGE_ATTACK_KIND)      // ����. 
public:                                             // Ÿ���� �ڽ��� ���ݰ���[�Ұ���] �ϰ� �����. (map_object �� ����� NPC)
    enum
    {
        ATTACK_IMPOSSIBILITY = 0,
        ATTACK_POSSIBILITY   = 1,
    };

    __PROPERTY(DWORD, dwObjKey);                    // mapobjectkey
    __PROPERTY(BYTE,  btPossibility);               // 0 : ���� �Ұ�, 1 : ���� ����
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ACTIVE_STAMP)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ATTACK_OBJECTKIND)       // ACTION_CHANGE_ATTACK_KIND Ȯ��
public:                                             //[����/����/������Ʈ]�� ���� �� �� [�ְ�/����] �����.
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(BYTE,  byAttack);                    // eACTION_ATTACK_TYPE
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_ACTIVE_SKILL_OBJECTKIND)
public:                                             //[����/����/������Ʈ]�� [��ųID]��ų�� �ߵ���Ų��.
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(DWORD, dwSkillCode);             // �ߵ� ��ų �ڵ�
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_SHOW_MESSAGEBOX_OBJECTKIND)
public:                                             //[����/����/������Ʈ]�� ��ġ�� [�޽����ڵ�]�޽����� �����ش�.
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(DWORD, dwMessageCode);               // ����� �޽����ڵ�
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_SET_ATTACK_STATE_OBJECTKIND)
public:                                             //[����/����]�� [������Ʈ]�� [����/����]�Ѵ�.
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(BYTE,  byDestObjectType);            // ��� eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwDestObjectKey);         // ��� Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(BYTE,  byAttack);                    // eACTION_ATTACK_TYPE
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_DESTROY_OBJECTKIND)      // ACTION_REMOVE_MONSTER Ȯ��
public:                                             // [����/����/������Ʈ]�� �����ϸ� ������ [����/������]�Ѵ�.
    enum
    {
        NOT_REWARD = 0,
        REWARD     = 1,
    };

    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(BYTE,  byReward);                    // ����ġ�׾����۵巴 ���� (0: ���Ѵ�. 1: �Ѵ�)
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER_LINKED_MAPOBJECT)     
public:                                            
    __PROPERTY(DWORD,  dwObjectKey);               // ������ƮID
END_DECL_ACTION;

BEGIN_DECL_ACTION(ACTION_CONTROL_TIMER)
public:   
    __PROPERTY(DWORD, dwTime);               // �ð� (ms), ������ �ʱ�ð�, �ı��� ���� �ð�(delay)
    __PROPERTY(BYTE,  byControlType);          // ���� eTIMER_CONTROL_TYPE
END_DECL_ACTION;                                // (0:������ ����(����), 1:������ ����(����), 2:�ı�)

//_NA_0_20100705_DOMINATION_EVENT
BEGIN_DECL_ACTION(ACTION_DO_COMMAND)
public:   
    enum
    {
        END_REGEN_EVENT = 0,
        MAX_COMMAND_TYPE,
    };

    __PROPERTY(BYTE,  byCommandType);          // �޽��� ���� 
END_DECL_ACTION;                               

// added by _NA_0_20100817_HELLON_LAST_TRIGGER
BEGIN_DECL_ACTION(ACTION_MOVE_CONTROL)     
public:   
    __PROPERTY(BYTE,  byObjectType);                // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(BYTE,  byMove);                  // eACTION_MOVE_TYPE
END_DECL_ACTION;                               

// added by _NA_0_20100817_HELLON_LAST_TRIGGER
BEGIN_DECL_ACTION(ACTION_ANIMATION_CONTROL)     
public:   
    enum
    {
        LOOP = 0,
        ONCE = 1,
    };

    __PROPERTY(BYTE,  byObjectType);        // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);         // Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(DWORD, dwAnimationCode1);    // ���� �ڵ�1
    __PROPERTY(BYTE,  byLoop1);             //���� �ڵ�1 �� (����/1ȸ) ����       
    __PROPERTY(DWORD, dwAnimationCode2);    // ���� �ڵ�2
    __PROPERTY(BYTE,  byLoop2);             //���� �ڵ�2 �� (����/1ȸ) ����      
END_DECL_ACTION;

// _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
// ����/����/������Ʈ���� �������� �����Ѵ�.
BEGIN_DECL_ACTION(ACTION_NPC_APPLY_DAMAGE)
public:
    __PROPERTY(BYTE, ObjectType); // ��ü ����
    __PROPERTY(DWORD, ObjectKey); // ��ü ������ ���� ����
    __PROPERTY(INT, Damage); // ������ ������
END_DECL_ACTION;

//_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
//�̼ǳ� ä�� Ʈ����
BEGIN_DECL_ACTION(ACTION_COMPLETE_COLLECTION)
public:
    enum 
    { 
        kMaxCollectObject = 10,
    };      
    __ARRAY_PROPERTY(CODETYPE, kMaxCollectObject, ObjectKey); // ��ü ������ ���� ����
    __PROPERTY(BYTE, EnableType); // Ȱ��ȭ����(0:��Ȱ��, 1:Ȱ��)
END_DECL_ACTION;

// _NA_000000_20130104_ADD_ACTION_TRIGGER_APPLY_HPMPSD
// [����/��ü]���� [HP/MP/SD]�� [��ġ]��ŭ [����/����]�� ����
BEGIN_DECL_ACTION(ACTION_APPLY_HPMPSD)
public:
    enum
    {
        kPerson = 0,
        kField = 1,
        kTeam = 2
    };
    enum
    {
        kHP = 0,
        KMP = 1,
        KSD = 2
    };
    enum
    {
        kIncrease = 0,
        kDecrease = 1
    };
    __PROPERTY(BYTE, TargetType); // TargetType
    __PROPERTY(int, TeamKey); // ����/����
    __PROPERTY(BYTE, PointType); // HP/MP/SD
    __PROPERTY(DWORD, Figure);   // ��ġ
    __PROPERTY(BYTE, ApplyType); // ����/����
END_DECL_ACTION;

// _NA_006658_20130422_ADD_ACTION_TRIGGER_CHANGE_OBJECT_HP
// [ObjectKey]������Ʈ�� HP�� [����/��ġ] [����]��ŭ [������/�ø���]
BEGIN_DECL_ACTION(ACTION_CHANGE_OBJECT_HP)
public:
    enum
    {
        kPersentage = 0,
        kValue = 1,
    };
    enum
    {
        kDecrease = 0,
        kIncrease = 1
    };
    __PROPERTY(DWORD, Objectkey);   // ��� ������ƮŰ
    __PROPERTY(BYTE, CalcType);     // ����/��ġ
    __PROPERTY(DWORD, Figure);      // ��
    __PROPERTY(BYTE, ApplyType);    // ��������
END_DECL_ACTION;

// _NA_006619_20130422_ADD_ACTION_TRIGGER_CHANGE_RADAR_MAP
// ���̴���(�̴ϸ�)�� �����Ѵ�.
BEGIN_DECL_ACTION(ACTION_CHANGE_RADAR_MAP)  
public:
    __PROPERTY(WzID, SubMapID); // �����ID
END_DECL_ACTION;


// ����� �������� �ִ� �÷��̾�� ���¸� ���ӽð����� �ο��Ѵ�
BEGIN_DECL_ACTION(ACTION_ATTACH_STATE_RANGE)  
public:
    __PROPERTY(BYTE,  byObjectType);            // eOBJECTCODE_TYPE
    __PROPERTY(DWORD, dwObjectKey);             // Ÿ�Կ� ���� ��ȭ.
    __PROPERTY(FLOAT, fRange);                  // ����
    __PROPERTY(DWORD, dwStateCode);             // �ο��� �����ڵ�
    __PROPERTY(DWORD, dwTimeDuration);          // ������ ���ӽð�
    __PROPERTY(BYTE,  byApplyType);             // 0:�ο��Ѵ�, 1:�����Ѵ�
END_DECL_ACTION;

// [��Ƽ/����]�� HP�� [����/��ġ] [����]��ŭ [������/�ø���]
BEGIN_DECL_ACTION(ACTION_CHANGE_PLAYER_HP)  
public:
    enum
    {
        kTargetParty = 0,
        kTargetPerson,
    };
    enum
    {
        kPersentage = 0,
        kValue = 1,
    };
    enum
    {
        kDecrease = 0,
        kIncrease = 1
    };
    __PROPERTY(BYTE, TargetType);   // Ÿ��Ÿ��
    __PROPERTY(BYTE, CalcType);     // ����/��ġ figure�� Ÿ��
    __PROPERTY(DWORD, Figure);      // ����/��
    __PROPERTY(BYTE, ApplyType);    // ��������
END_DECL_ACTION;


#endif //SOLARTRIGGER_ACTION_INFO_H