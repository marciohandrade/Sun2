#include "stdafx.h"
#include "./TriggerInfo.h"

#include <ProgramCommon/WzArchive.h>
#include <Misc.h>

#include "ActionInfo.h"
#include "ConditionInfo.h"
#include "TriggerInfoManager.h"

#include <fstream>

//==================================================================================================

#define TYPESWITCH_BEGIN(s)         sSWITCH(s)

#define TYPE_BEGIN(c)               CASE(e##c)                                    \
                                        c##_INFO* partial_info = TAllocNew(c##_INFO);
#define __SET(dataType, v)              dataType _value##v;                         \
                                        archive >> _value##v;                      \
                                        partial_info->Set##v(_value##v);
#define __ARRAY_SET(dataType, num , v)  dataType _value##v;                         \
                                        for (WORD i=0;i<num;++i) {                   \
                                            archive >> _value##v;                  \
                                            partial_info->Set##v(i, _value##v);          \
                                        }
#define TYPE_END                        return partial_info;
#define TYPESWICH_END               eSWITCH

//==================================================================================================

TriggerInfo::TriggerInfo()
{
}

TriggerInfo::~TriggerInfo()
{
    Release();
}

void TriggerInfo::Release()
{
    FOREACH_CONTAINER(const ns_trigger::CONDITION_INFO_LIST::value_type& info_node,
                      condition_info_list_, ns_trigger::CONDITION_INFO_LIST)
    {
        TAllocDelete(ConditionInfo, info_node);
    }

    FOREACH_CONTAINER(const ns_trigger::ACTION_INFO_LIST::value_type& info_node,
                      action_info_list_, ns_trigger::ACTION_INFO_LIST)
    {
        TAllocDelete(ActionInfo, info_node);
    }
    condition_info_list_.clear();
    action_info_list_.clear();
}

void TriggerInfo::Load(CWzArchive& IN archive, TriggerInfoManager* trigger_info_manager)
{
    ASSERT(condition_info_list_.empty());
    ASSERT(action_info_list_.empty());

    INT iIDTriggerListIndex = 0;
    if (159 < archive.GetVersion())
    {
        archive >> iIDTriggerListIndex;
    #ifdef _DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER
        set_condition_index(iIDTriggerListIndex);
    #endif
    }

    // ���̵� + �Ӽ�
    archive >> trigger_id_;
    archive >> trigger_attributes_;

    ASSERT(trigger_attributes_ < 16);

    // ����� ����
    WORD wNumberOfConditions = 0;
    archive >> wNumberOfConditions;

    INT iLastIDConditionTriggerIndex = 0;
    if (159 < archive.GetVersion())
    {
        archive >> iLastIDConditionTriggerIndex;
    }

    always_trigger_ = false;

    // ����� Ÿ�Ժ��� ����
    WORD wConditionType = 0;
    INT iConditionIDIndex = 0;
    for (WORD i = 0; i < wNumberOfConditions; ++i)
    {
        archive >> wConditionType;
        ASSERT(eCONDITION_ALWAYS <= wConditionType && wConditionType < eCONDITION_MAX);

        if (159 < archive.GetVersion()) {
            archive >> iConditionIDIndex;
        };
        ConditionInfo* pConditionInfo = _ParseCondition(archive, wConditionType, trigger_info_manager);
        if (eCONDITION_ALWAYS == wConditionType) {
            always_trigger_ = true;
        };
        ASSERT(pConditionInfo);
        condition_info_list_.push_back(pConditionInfo); 
    }
    

    // �׼� Ʈ���� ����
    WORD wNumberOfActions = 0;
    archive >> wNumberOfActions;

    INT iLastIDActionTriggerIndex = 0;
    if (159 < archive.GetVersion())
    {
        archive >> iLastIDActionTriggerIndex;
    }

    // �׼� Ÿ�Ժ��� ����
    WORD wActionType = 0;
    INT iActionIDIndex;
    for (WORD i = 0; i < wNumberOfActions; ++i)
    {
        archive >> wActionType;
        ASSERT(eACTION_CHANGE_OBJECTANI <= wActionType && wActionType < eACTION_MAX);

        if (159 < archive.GetVersion()) {
            archive >> iActionIDIndex;
        };
        ActionInfo* pActionInfo = _ParseAction(archive, wActionType, trigger_info_manager);
        ASSERT(pActionInfo);
        action_info_list_.push_back(pActionInfo); 
    }

#ifdef _HARDMODE_TRIGGER_PRINT_
    /*;{
        std::ofstream out_trigger("TRIGGER.txt", std::ios::app);
        CHAR buffer[512] = { 0, };
        sprintf(buffer, "TRIGGER_DO_ACTION: ID(%u) ī�װ�(%s)\n", TriggerID(), CategoryName());
        out_trigger << buffer;
    };*/
    
    FOREACH_CONTAINER(const ns_trigger::CONDITION_INFO_LIST::value_type& rNode,
        condition_info_list_, ns_trigger::CONDITION_INFO_LIST)
    {
        ConditionInfo* pConditionInfo = rNode;
        eCONDITION_TYPE condition_type = pConditionInfo->TriggerType();

        switch(condition_type)
        {
            case eCONDITION_COUNT_NPCKILL:
                {
                    CONDITION_COUNT_NPCKILL_INFO* condition_info = \
                        static_cast<CONDITION_COUNT_NPCKILL_INFO*>(pConditionInfo);

                    ;{
                        std::ofstream out_trigger("TRIGGER.txt", std::ios::app);
                        CHAR buffer[512] = { 0, };
                        sprintf(buffer, "%d\t%d\t%d\t;CONDITION_COUNT_NPCKILL\n", \
                            condition_info->GetMonsterCode(), 0, 0);
                        out_trigger << buffer;
                    };
                } break;
        }
    }

    FOREACH_CONTAINER(const ns_trigger::ACTION_INFO_LIST::value_type& rNode,
        action_info_list_, ns_trigger::ACTION_INFO_LIST)
    {
        ActionInfo* pActionInfo = rNode;
        eACTION_TYPE action_type = pActionInfo->TriggerType();
        
        switch(action_type)
        {
            case eACTION_CREATE_MONSTER:
                {
                    ACTION_CREATE_MONSTER_INFO* action_info = \
                        static_cast<ACTION_CREATE_MONSTER_INFO*>(pActionInfo);

                    ;{
                        std::ofstream out_trigger("TRIGGER.txt", std::ios::app);
                        CHAR buffer[512] = { 0, };
                        sprintf(buffer, "%d\t%d\t%d\t;ACTION_CREATE_MONSTER_INFO\n", \
                            action_info->GetMonsterCode(), 0, 0);
                        out_trigger << buffer;
                    };
                } break;
            case eACTION_CREATE_DIR_MONSTER:
                {
                    ACTION_CREATE_DIR_MONSTER_INFO* action_info = \
                        static_cast<ACTION_CREATE_DIR_MONSTER_INFO*>(pActionInfo);

                    ;{
                        std::ofstream out_trigger("TRIGGER.txt", std::ios::app);
                        CHAR buffer[512] = { 0, };
                        sprintf(buffer, "%d\t%d\t%d\t;ACTION_CREATE_DIR_MONSTER\n", \
                            action_info->GetMonsterCode(), 0, 0);
                        out_trigger << buffer;
                    };
                } break;
        }
    }
#endif //_HARDMODE_TRIGGER_PRINT_
}

ConditionInfo* TriggerInfo::_ParseCondition(CWzArchive& IN archive,
    WORD condition_type, TriggerInfoManager* trigger_info_manager)
{
    TYPESWITCH_BEGIN(condition_type)

    TYPE_BEGIN(CONDITION_ALWAYS)
    TYPE_END    

    TYPE_BEGIN(CONDITION_COMPARE_SWITCH)
        __SET(BYTE, SwitchID)
        __SET(BYTE, OperationType)
        __SET(INT, Value)
    TYPE_END

	TYPE_BEGIN(CONDITION_COMPARE_SWITCH_EX)
		__SET(BYTE, Target)
		__SET(BYTE, SwitchID)
		__SET(BYTE, OperationType)
		__SET(INT, Value)
	TYPE_END

    TYPE_BEGIN(CONDITION_CLICK_OBJECT)
        __SET(INT, ObjectID)
    TYPE_END

    TYPE_BEGIN(CONDITION_DAMAGE_OBJECT)
        __SET(INT, ObjectID)
        __SET(FLOAT, Damage)
    TYPE_END

    TYPE_BEGIN(CONDITION_COUNT_NPCKILL)
        __SET(INT, AreaID)
        __SET(INT, MonsterCode)               //< �� ���� �� Code
        __SET(BYTE, IsParty)
//      if (_valueIsParty == PERSONAL) _asm int 3;
        __SET(INT, KillCount)             //< ���
        __SET(BYTE, Method)               //< ��� = {�Ϲ�, ����}
#if defined(_SERVER)
        // (NOTE) ���� �ڵ忡 ���ؼ��� ���� �ϰڴ�
        if (trigger_info_manager) {
            trigger_info_manager->NotifyTriggerRegisterNpcKill(\
                static_cast<MONSTERCODE>(partial_info->GetMonsterCode()));
        }
#endif
    TYPE_END

    TYPE_BEGIN(CONDITION_PASS_TIME)
        __SET(INT, PassTime)
    TYPE_END

    TYPE_BEGIN(CONDITION_ENTER_AREA)
        __SET(INT, AreaID)
        __SET(INT, PlayerCount)
        __SET(BYTE, IsParty)
    TYPE_END

    TYPE_BEGIN(CONDITION_HAVE_ITEM)
        __SET(INT, ItemCode)
        __SET(INT, ItemNum)   // ���� �ִ� ������ ����
    TYPE_END

    TYPE_BEGIN(CONDITION_MULTI_SWITCH)
        __ARRAY_SET(BYTE, CONDITION_MULTI_SWITCH_INFO::_MAX_SWITCH_ID_NUM, SwitchID)
        __SET(BYTE, OperationType1)
        __SET(INT, Value1)
        __SET(BYTE, OperationType2)
        __SET(INT, Value2)
    TYPE_END

    TYPE_BEGIN(CONDITION_MISSION_POINT)
        __SET(INT, MissionPoint)      // �� ����Ʈ����
        __SET(BYTE, Flag)             // 0:�̻� 1:���� 2:����
    TYPE_END

    TYPE_BEGIN(CONDITION_COMPARE_VARIABLE)
        __SET(INT, VarID)
        __SET(BYTE, OperationType)
        __SET(INT, Value)
    TYPE_END

    TYPE_BEGIN(CONDITION_QUEST_PROGRESS)
        __SET(/*QCODE*/ ULONG, QuestID)   // Quest ID
        __SET(BYTE, Progress)         // (������, �Ϸ���)
    TYPE_END

    TYPE_BEGIN(CONDITION_CHECK_CLASS_ID)
        __SET(INT, ClassID)
    TYPE_END

    TYPE_BEGIN(CONDITION_NPCKILL_FOR_UNITID)
        __SET(WzID, MonsterUnitID)
#if defined(_SERVER)
        if (trigger_info_manager) {
            trigger_info_manager->NotifyTriggerRegisterUnitKill(partial_info->GetMonsterUnitID());
        };
#endif
    TYPE_END

    TYPE_BEGIN(CONDITION_LIVE_PARTY_MEMBER)
        __SET(BYTE, LiveFlag)             // 0:��� 1:����
    TYPE_END

    TYPE_BEGIN(CONDITION_CHECK_OBJECT_HP)
        __SET(DWORD, dwObjKey)           
        __SET(BYTE,  btHPPercentage)           
    TYPE_END

    TYPE_BEGIN(CONDITION_DESTROY_OBJECT)
        __SET(DWORD, dwObjKey)           
    TYPE_END

    TYPE_BEGIN(CONDITION_CHECK_OBJECTKIND_HP)
    __SET(BYTE,  byObjectType)           // ����� Ÿ��(����/����/������Ʈ)
    __SET(DWORD, dwObjectKey)            // ��� ��ü�� Ű��
    __SET(BYTE,  btHPPercentage)         // HP ����    
    TYPE_END

    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    TYPE_BEGIN(CONDITION_NPC_ENTER_AREA)
        __SET(CODETYPE, AreaID)
        __SET(CODETYPE, NPCUnitID)
    TYPE_END

    // _NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    TYPE_BEGIN(CONDITION_COMPLETE_COLLECTION)
        __SET(DWORD, ObjectKey)
    TYPE_END

    // _NA_000000_20130103_ADD_SERVER_EVENT_TRIGGER
    TYPE_BEGIN(CONDITION_SERVER_EVENT)
    __SET(int, EventID)
    TYPE_END

    // _NA_000000_20130104_ADD_CONDITION_TRIGGER_CHECK_TEAM
    TYPE_BEGIN(CONDITION_CHECK_TEAM)
    __SET(int, TeamKey)
    TYPE_END

    TYPE_BEGIN(CONDITION_MONSTER_ENTER_AREA)
    __SET(int, AreaID)
    __SET(int, MonsterCode)
    TYPE_END


    TYPESWICH_END

    return NULL;
}

ActionInfo* TriggerInfo::_ParseAction(CWzArchive& IN archive,
    WORD action_type, TriggerInfoManager* trigger_info_manager)
{
    __UNUSED(trigger_info_manager);
    TYPESWITCH_BEGIN(action_type)

        TYPE_BEGIN(ACTION_CHANGE_OBJECTANI)
            __SET(INT, ObjectID)
            __SET(DWORD, WzAniID)
            __SET(BYTE, Direction)        // ����(������ 0, ������ 1) 
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_PATHTILE)
            __SET(INT, TileID)
            __SET(BYTE, Attribute)
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_OBJECTSTATE)
            __SET(INT, ObjectID)
            __SET(BYTE, State)
        TYPE_END

        TYPE_BEGIN(ACTION_PLAY_EVENT)
            __SET(INT, EventID)
        TYPE_END

        TYPE_BEGIN(ACTION_SET_ISATTACK)
            __SET(BYTE, TargetType)       // ���ݴ�� ����(0 : ����, 1 : ����, 2 : �Ѵ�)
            __SET(BYTE, IsAttack)         // ���� ���� ����(0 : ���ݰ���, 1 : ���ݺҰ�)
        TYPE_END

        TYPE_BEGIN(ACTION_PORTAL_PLAYER)
            __SET(INT, MapCode)
            __SET(INT, AreaID)
            __SET(BYTE, IsParty)          // �̵���ų ��� (0 : ���� 1: ��Ƽ)
            __SET(INT, Type)
        TYPE_END

        TYPE_BEGIN(ACTION_SET_OBJECTTHRUST)
            __SET(BYTE, State)
        TYPE_END

        TYPE_BEGIN(ACTION_APPLY_DAMAGE)
            __SET(BYTE, DamageType)
            __SET(BYTE, TargetType)
            __SET(FLOAT, Damage)
        TYPE_END

        TYPE_BEGIN(ACTION_REFLECT_DAMAGE)
            __SET(INT, ObjectID)
            __SET(FLOAT, Damage)
        TYPE_END

        //TYPE_BEGIN(ACTION_CREATE_MONSTER)
        //  __SET(INT, MonsterCode)
        //  __SET(INT, AreaID)
        //  __SET(BYTE, IsParty)
        //  __SET(INT, CreateCount)           // ���� ���� ����
        //  __SET(INT, DelayTime)             // ���� �ð� ����
        //  __SET(INT, LoopCount)             // ���� ���� ȸ��
        //  __SET(BYTE, CreateDestroy)        // ���� ����/����
        //  __SET(INT, Type)                  // Ÿ�� (���� �� �����̳�)
        //TYPE_END
        }
        break;

        case(eACTION_CREATE_MONSTER):
        {
            ACTION_CREATE_MONSTER_INFO* partial_info = TAllocNew(ACTION_CREATE_MONSTER_INFO);

            //__SET(INT, MonsterCode)
            INT _valueMonsterCode;
            archive >> _valueMonsterCode;
            partial_info->SetMonsterCode(_valueMonsterCode);
            //__SET(INT, AreaID)
            INT _valueAreaID;
            archive >> _valueAreaID;
            partial_info->SetAreaID(_valueAreaID);
            //__SET(BYTE, IsParty)
            BYTE _valueIsParty;
            archive >> _valueIsParty;
            partial_info->SetIsParty(_valueIsParty);
            //__SET(INT, CreateCount)         // ���� ���� ����
            INT _valueCreateCount;
            archive >> _valueCreateCount;
            partial_info->SetCreateCount(_valueCreateCount);
            //__SET(INT, DelayTime)               // ���� �ð� ����
            INT _valueDelayTime;
            archive >> _valueDelayTime;
            partial_info->SetDelayTime(_valueDelayTime);
            //__SET(INT, LoopCount)               // ���� ���� ȸ��
            INT _valueLoopCount;
            archive >> _valueLoopCount;
            partial_info->SetLoopCount(_valueLoopCount);
            //__SET(BYTE, CreateDestroy)      // ���� ����/����
            BYTE _valueCreateDestroy;
            archive >> _valueCreateDestroy;
            partial_info->SetCreateDestroy(_valueCreateDestroy);
            //__SET(INT, Type)                    // Ÿ�� (���� �� �����̳�)
            INT _valueType;
            archive >> _valueType;
            partial_info->SetType(_valueType);

            return partial_info;
        //}
        //break;

        TYPE_BEGIN(ACTION_DISPLAY_MSGBOX)
            __SET(INT, TextID)
            __SET(BYTE, Target)           // 0:���� 1:��ü 2:���� 3:����
            __SET(INT, DisplayWindow)     // ����� â
        TYPE_END

        TYPE_BEGIN(ACTION_REMOVE_ITEM)
            __SET(INT, ItemCode)              
            __SET(INT, ItemNum)               // ������ ����
        TYPE_END

        TYPE_BEGIN(ACTION_REWARD_PLAYER)
            __SET(INT, ItemCode)
            __SET(INT, Count)
            __SET(BYTE, IsParty)
            __SET(BYTE, IsDrop)                // �ִ¹��(0 : �κ��� �־,1 �ٴڿ� ����)
        TYPE_END

        TYPE_BEGIN(ACTION_CLEAR_MISSION)
            __SET(BYTE, ResultType)
        TYPE_END

        TYPE_BEGIN(ACTION_PORTAL_RANDOM)
            __SET(INT, MapID)
            __ARRAY_SET(INT, ACTION_PORTAL_RANDOM_INFO::_MAX_AREA_ID_NUM, AreaID)
            __SET(BYTE, Random)               // ���� ����������
            __SET(BYTE, Target)               // (0: ���� 1: ��Ƽ)
        TYPE_END

        TYPE_BEGIN(ACTION_NPC_RANDOM_AREA_MOVE)
            __ARRAY_SET(INT, ACTION_NPC_RANDOM_AREA_MOVE_INFO::_MAX_AREA_ID_NUM, AreaID)
            __SET(BYTE, MoveOrWarp)           // (0: move 1: warp)
        TYPE_END

        TYPE_BEGIN(ACTION_ACTIVATE_TRIGGER)
            __SET(DWORD, WzTriggerID)
            __SET(BYTE, Flag)             // 0 : Ȱ��, 1 : ��Ȱ��
        TYPE_END

        TYPE_BEGIN(ACTION_AREA_DAMAGE)
            __SET(DWORD, ObjectIndex)     
            __SET(FLOAT, Range)
            __SET(BYTE, Target)               // ��ǥ (����/��Ƽ)       
            __SET(BYTE, DamageType)           // ������ Ÿ��
            __SET(FLOAT, Damage)
        TYPE_END

        TYPE_BEGIN(ACTION_OPERATE_SWITCH)
            __SET(BYTE, SwitchID)
            __SET(BYTE, OperationType)
            __SET(INT, Argument)              // �Ű�����
        TYPE_END

		TYPE_BEGIN(ACTION_OPERATE_SWITCH_EX)
			__SET(BYTE, Target)               // (0: ���� 1: ��Ƽ)
			__SET(BYTE, SwitchID)
			__SET(BYTE, OperationType)
			__SET(INT, Argument)              // �Ű�����
		TYPE_END

        TYPE_BEGIN(ACTION_OPERATE_VARIABLE)
            __SET(INT, VarID)
            __SET(BYTE, OperationType)
            __SET(INT, Argument)              // �Ű�����
        TYPE_END

        TYPE_BEGIN(ACTION_ROAD_CONNECT)       // �� ������ ����/���� �Ѵ�
            __SET(INT, AreaIndex)
            __SET(BYTE, CanGo)                // �� �� �ִ�=0, ����=1
        TYPE_END

        TYPE_BEGIN(ACTION_CREATE_DIR_MONSTER) // ���� �����Ѵ�(����)
            __SET(INT, MonsterCode)
            __SET(INT, AreaID)
            __SET(BYTE, IsParty)
            __SET(INT, CreateCount)           // ���� ���� ����
            __SET(INT, DelayTime)         // ���� �ð� ����
            __SET(INT, LoopCount)         // ���� ���� ȸ��
            __SET(FLOAT, DirX)            // x����
            __SET(FLOAT, DirY)            // y����
            __SET(FLOAT, DirZ)            // z����
            __SET(INT, Type)              // Type[0]
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_LIGHT)   // ����Ʈ ������ �����Ѵ�
            __SET(INT, LightIndex)        // �� �ε����� ����Ʈ��
            __SET(INT, DelayTime)         // �̸�ŭ�� �ð� �Ŀ�
            __SET(BYTE, Status)           // 0:���� 1:�Ҵ� 2:����Ѵ�
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_OBJECT_TYPE) // ������Ʈ ���¸� �ٲ۴� [Ÿ��]
            __SET(INT, ObjectID)
            __SET(INT, DelayTime)
            __SET(INT, ObjectType)
        TYPE_END

        TYPE_BEGIN(ACTION_QUEST_SATISFACTION)
            __SET(ULONG, QuestID)
            __SET(LONG, ScriptCode)
        TYPE_END

        TYPE_BEGIN(ACTION_QUEST_GRANT)
            __SET(ULONG, QuestID)
            __SET(BYTE, PartyState)
        TYPE_END

        TYPE_BEGIN(ACTION_QUEST_SETUP)
            __SET(ULONG, QuestID)
            __SET(BYTE, Completed)
        TYPE_END

        TYPE_BEGIN(ACTION_USE_CATEGORY)
            __SET(INT, FieldID)
            __ARRAY_SET(TCHAR, ACTION_USE_CATEGORY_INFO::_MAX_USE_CATEGORY_NAME, CategoryName)
        TYPE_END

        // ������ �����ϴ� �׼� Ʈ����
        //BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER)
        //__END_DECL_ACTION

        TYPE_BEGIN(ACTION_CREATE_MONSTER_GROUP)
            __SET(INT, GroupID)
            __SET(INT, AreaID)
            __SET(BYTE, PartyState)
            __SET(INT, GroupCount)
        TYPE_END

        // ���� ��Ÿ��/����� (���������� ���� ����/���ſ� ����)
        TYPE_BEGIN(ACTION_SHOW_OR_HIDE_UNIT)
            __SET(WzID, MonsterUnitID)
            __SET(INT, IsHide)        //0: ��Ÿ����, 1:�������
        TYPE_END

        TYPE_BEGIN(ACTION_SHOW_GUIDE_MSGBOX)
            __SET(LONG, HelpIndex)
        TYPE_END

        // �޽��� �ڽ��� ��Ÿ��/�����
        TYPE_BEGIN(ACTION_SHOW_OR_HIDE_MSGBOX)
            __SET(INT, TextID)
            __SET(BYTE, IsParty)
            __SET(INT, DisplayWindow)     // ����� â
            __SET(INT, IsShow)        //1: ��Ÿ����, 0:�������
        TYPE_END

        TYPE_BEGIN(ACTION_RANDOM_MONSTER_SPAWN)
            __SET(INT, MonsterID_1);          // 1-5�� ���� ID
            __SET(INT, MonsterRate_1);        // 1-5�� ���� ����Ȯ��
            __SET(INT, MonsterID_2);
            __SET(INT, MonsterRate_2);
            __SET(INT, MonsterID_3);
            __SET(INT, MonsterRate_3);
            __SET(INT, MonsterID_4);
            __SET(INT, MonsterRate_4);
            __SET(INT, MonsterID_5);
            __SET(INT, MonsterRate_5);        
            __SET(INT, AreaID);               // ���� ���� ����
            __SET(BYTE, IsParty);             // ���� ? ��Ƽ ?
            __SET(INT, MinCreateMonsterCnt);  // �ּ� ���� ���� ����
            __SET(INT, MaxCreateMonsterCnt);  // �ּ� ���� ���� ����
            __SET(INT, RegenTime)             // ���� �ð� ����
            __SET(INT, MaxRegenCount)         // ���� �� ���� ȸ��
            __SET(FLOAT, DirX)                // x����
            __SET(FLOAT, DirY)                // y����
            __SET(FLOAT, DirZ)                // z����
        TYPE_END

        TYPE_BEGIN(ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR)
            __SET(INT, Floor);            // 1-5�� ���� ID
        TYPE_END

        TYPE_BEGIN(ACTION_RANDOM_MAP_MOVE)
            __ARRAY_SET(INT, ACTION_RANDOM_MAP_MOVE_INFO::_MAX_FIELDCODE_ID, FieldID);
            __SET(BYTE, Random);              // ����, ����������
            __SET(WzID, AreaID);              // ���� ID
            __SET(BYTE, Target);              // (0: ���� 1: ��Ƽ)
            __SET(INT, Type);             // (0: ����, 1:�ͼӸ���(?), 2:��Ʋ��
        TYPE_END

        TYPE_BEGIN(ACTION_RANDOM_AREA_MOVE)
            __ARRAY_SET(INT, ACTION_RANDOM_AREA_MOVE_INFO::_MAX_AREA_ID, AreaIDX);    // �̵���ų ���� IDs
            __SET(BYTE, Target);                      // (0: ���� 1: ��Ƽ)
            __SET(BYTE, ProcType);                    // (0: ����, 1: ����)
        TYPE_END

        TYPE_BEGIN(ACTION_REMOVE_MONSTER)
            __SET(INT, MonsterCode);      // �����ڵ�
            __SET(BYTE, IsExpItemDrop);   // ����ġ�׾����۵巴 ���� (0: ���Ѵ�. 1: �Ѵ�)
        TYPE_END

        TYPE_BEGIN(ACTION_ADD_EXP)
            __SET(BYTE, IsParty);             // �����̳� ��Ƽ�� (0 : ����, 1:��Ƽ)
            __SET(DWORD, Exp);                // Exp �ִ� ����ġ.
        TYPE_END

        TYPE_BEGIN(ACTION_MONSTER_STAT_UP_OR_DOWN)
        __SET(INT, MonsterCode)           // �����ڵ�
        __SET(WORD, AbilityID)            // �����Ƽ ID
        __SET(int, AbilityParameter1)     // Ÿ�Լ���     1. ��ġ, 2. ����ġ ��� ����, 3. �ִ�ġ ��� ����
        __SET(int, AbilityParameter2)     // ��ġ/������
        __SET(BYTE, OperationType)        // ������ 1 : eOPERATION_PLUS(+) , 0 : eOPERATION_MINUS(-) 
        TYPE_END

        TYPE_BEGIN(ACTION_RANDUM_TRIGER_ON_OR_OFF)
            __ARRAY_SET(DWORD, ACTION_RANDUM_TRIGER_ON_OR_OFF_INFO::_MAX_TRIGGER_ID, WzTriggerID)
            __SET(BYTE, TriggerState) 
        TYPE_END

        TYPE_BEGIN(ACTION_PLAY_MUSIC)
        __SET(BYTE, Target);                      // (0: ���� 1: �ʵ���ü)
        __SET(INT, MusCode);                      // (�����ڵ�)
        TYPE_END

        TYPE_BEGIN(ACTION_DOWN_OBJETC_HP)
            __SET(DWORD, dwObjKey);					// mapobjectkey
            __SET(BYTE,  btHPPercentage);			    // �϶� ��ġ
        TYPE_END
        TYPE_BEGIN(ACTION_CHANGE_ATTACK_KIND)
            __SET(DWORD, dwObjKey);					// mapobjectkey
            __SET(BYTE,  btPossibility);			    // 0 : ���� �Ұ�, 1 : ���� ����
        TYPE_END

        TYPE_BEGIN(ACTION_ACTIVE_STAMP)
        TYPE_END

        TYPE_BEGIN(ACTION_ATTACK_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // Ÿ�Կ� ���� ��ȭ.
            __SET(BYTE,  byAttack);			        // eACTION_ATTACK_TYPE
        TYPE_END

        TYPE_BEGIN(ACTION_ACTIVE_SKILL_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // Ÿ�Կ� ���� ��ȭ.
            __SET(DWORD, dwSkillCode);			    // �ߵ� ��ų �ڵ�
        TYPE_END

        TYPE_BEGIN(ACTION_SHOW_MESSAGEBOX_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // Ÿ�Կ� ���� ��ȭ.
            __SET(DWORD, dwMessageCode);			    // ����� �޽����ڵ�
        TYPE_END

        TYPE_BEGIN(ACTION_SET_ATTACK_STATE_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // Ÿ�Կ� ���� ��ȭ.
            __SET(BYTE,  byDestObjectType);			// ��� eOBJECTCODE_TYPE
            __SET(DWORD, dwDestObjectKey);			// ��� Ÿ�Կ� ���� ��ȭ.
            __SET(BYTE,  byAttack);			        // eACTION_ATTACK_TYPE
        TYPE_END

        TYPE_BEGIN(ACTION_DESTROY_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // Ÿ�Կ� ���� ��ȭ.
            __SET(BYTE,  byReward);			        // ����ġ�׾����۵巴 ���� (0: ���Ѵ�. 1: �Ѵ�)
        TYPE_END

        TYPE_BEGIN(ACTION_CREATE_MONSTER_LINKED_MAPOBJECT)
            __SET(DWORD,  dwObjectKey);				
        TYPE_END

        TYPE_BEGIN(ACTION_CONTROL_TIMER)
            __SET(DWORD, dwTime);                     // �ð� (ms), ������ �ʱ�ð�, �ı��� ���� �ð�(delay)
            __SET(BYTE,  byControlType);		        // ���� eTIMER_CONTROL_TYPE
        TYPE_END                                        // (0:������ ����(����), 1:������ ����(����), 2:�ı�)
        //_NA_0_20100705_DOMINATION_EVENT
        TYPE_BEGIN(ACTION_DO_COMMAND)
            __SET(BYTE,  byCommandType);		        // �޽��� ���� 
        TYPE_END                                        
        TYPE_BEGIN(ACTION_MOVE_CONTROL)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // Ÿ�Կ� ���� ��ȭ.
            __SET(BYTE,  byMove);			        // eACTION_MOVE_TYPE
        TYPE_END                                        
        
        TYPE_BEGIN(ACTION_ANIMATION_CONTROL)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // Ÿ�Կ� ���� ��ȭ.
            __SET(DWORD, dwAnimationCode1);	        // ���� �ڵ�1
            __SET(BYTE,  byLoop1);                    //���� �ڵ�1 �� (����/1ȸ) ����       
            __SET(DWORD, dwAnimationCode2);	        // ���� �ڵ�2
            __SET(BYTE,  byLoop2);                    //���� �ڵ�2 �� (����/1ȸ) ����      
        TYPE_END                                        
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        TYPE_BEGIN(ACTION_NPC_APPLY_DAMAGE)
            __SET(BYTE, ObjectType); // ��ü ����
            __SET(DWORD, ObjectKey); // ��ü ������ ���� ����
            __SET(INT, Damage); // ������ ������
        TYPE_END
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        TYPE_BEGIN(ACTION_COMPLETE_COLLECTION)
            __ARRAY_SET(CODETYPE, ACTION_COMPLETE_COLLECTION_INFO::kMaxCollectObject, ObjectKey);  // ä��ID
            __SET(BYTE, EnableType); // ��ü ����
        TYPE_END

        TYPE_BEGIN(ACTION_APPLY_HPMPSD)
            __SET(BYTE, TargetType); // TargetType
            __SET(int, TeamKey); // ����/����
            __SET(BYTE, PointType); // HP/MP/SD
            __SET(DWORD, Figure);   // ��ġ
            __SET(BYTE, ApplyType); // ����/����
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_OBJECT_HP)
            __SET(DWORD, Objectkey);   // ��� ������ƮŰ
            __SET(BYTE, CalcType);     // ����/��ġ
            __SET(DWORD, Figure);      // ��
            __SET(BYTE, ApplyType);    // ��������
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_RADAR_MAP)
            __SET(WzID, SubMapID); // �����ID
        TYPE_END
        
        TYPE_BEGIN(ACTION_ATTACH_STATE_RANGE)
            __SET(BYTE,  byObjectType); // eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey); // Ÿ�Կ� ���� ��ȭ.
            __SET(FLOAT, fRange); // ����
            __SET(DWORD, dwStateCode); // �ο��� �����ڵ�
            __SET(DWORD, dwTimeDuration); // ������ ���ӽð�
            __SET(BYTE, byApplyType); // 0:�ο��Ѵ�, 1:�����Ѵ�
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_PLAYER_HP)
            __SET(BYTE, TargetType);   // ��Ƽ/����
            __SET(BYTE, CalcType);     // ����/��ġ
            __SET(DWORD, Figure);      // ��
            __SET(BYTE, ApplyType);    // ��������
        TYPE_END
    TYPESWICH_END

    return NULL;
}
