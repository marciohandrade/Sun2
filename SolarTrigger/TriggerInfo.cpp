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

    // 아이디 + 속성
    archive >> trigger_id_;
    archive >> trigger_attributes_;

    ASSERT(trigger_attributes_ < 16);

    // 컨디션 개수
    WORD wNumberOfConditions = 0;
    archive >> wNumberOfConditions;

    INT iLastIDConditionTriggerIndex = 0;
    if (159 < archive.GetVersion())
    {
        archive >> iLastIDConditionTriggerIndex;
    }

    always_trigger_ = false;

    // 컨디션 타입별로 읽음
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
    

    // 액션 트리거 개수
    WORD wNumberOfActions = 0;
    archive >> wNumberOfActions;

    INT iLastIDActionTriggerIndex = 0;
    if (159 < archive.GetVersion())
    {
        archive >> iLastIDActionTriggerIndex;
    }

    // 액션 타입별로 읽음
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
        sprintf(buffer, "TRIGGER_DO_ACTION: ID(%u) 카테고리(%s)\n", TriggerID(), CategoryName());
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
        __SET(INT, MonsterCode)               //< 몹 종류 몹 Code
        __SET(BYTE, IsParty)
//      if (_valueIsParty == PERSONAL) _asm int 3;
        __SET(INT, KillCount)             //< 몇마리
        __SET(BYTE, Method)               //< 방법 = {일반, 누적}
#if defined(_SERVER)
        // (NOTE) 몬스터 코드에 대해서는 툴만 믿겠다
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
        __SET(INT, ItemNum)   // 갖고 있는 아이템 개수
    TYPE_END

    TYPE_BEGIN(CONDITION_MULTI_SWITCH)
        __ARRAY_SET(BYTE, CONDITION_MULTI_SWITCH_INFO::_MAX_SWITCH_ID_NUM, SwitchID)
        __SET(BYTE, OperationType1)
        __SET(INT, Value1)
        __SET(BYTE, OperationType2)
        __SET(INT, Value2)
    TYPE_END

    TYPE_BEGIN(CONDITION_MISSION_POINT)
        __SET(INT, MissionPoint)      // 이 포인트보다
        __SET(BYTE, Flag)             // 0:이상 1:이하 2:동등
    TYPE_END

    TYPE_BEGIN(CONDITION_COMPARE_VARIABLE)
        __SET(INT, VarID)
        __SET(BYTE, OperationType)
        __SET(INT, Value)
    TYPE_END

    TYPE_BEGIN(CONDITION_QUEST_PROGRESS)
        __SET(/*QCODE*/ ULONG, QuestID)   // Quest ID
        __SET(BYTE, Progress)         // (진행중, 완료한)
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
        __SET(BYTE, LiveFlag)             // 0:사망 1:생존
    TYPE_END

    TYPE_BEGIN(CONDITION_CHECK_OBJECT_HP)
        __SET(DWORD, dwObjKey)           
        __SET(BYTE,  btHPPercentage)           
    TYPE_END

    TYPE_BEGIN(CONDITION_DESTROY_OBJECT)
        __SET(DWORD, dwObjKey)           
    TYPE_END

    TYPE_BEGIN(CONDITION_CHECK_OBJECTKIND_HP)
    __SET(BYTE,  byObjectType)           // 대상의 타입(몬스터/유닛/오브젝트)
    __SET(DWORD, dwObjectKey)            // 대상 객체의 키값
    __SET(BYTE,  btHPPercentage)         // HP 조건    
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
            __SET(BYTE, Direction)        // 방향(정방향 0, 역방향 1) 
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
            __SET(BYTE, TargetType)       // 공격대상 종류(0 : 유저, 1 : 몬스터, 2 : 둘다)
            __SET(BYTE, IsAttack)         // 공격 가능 여부(0 : 공격가능, 1 : 공격불가)
        TYPE_END

        TYPE_BEGIN(ACTION_PORTAL_PLAYER)
            __SET(INT, MapCode)
            __SET(INT, AreaID)
            __SET(BYTE, IsParty)          // 이동시킬 대상 (0 : 개인 1: 파티)
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
        //  __SET(INT, CreateCount)           // 몬스터 생성 갯수
        //  __SET(INT, DelayTime)             // 생성 시간 간격
        //  __SET(INT, LoopCount)             // 몬스터 생성 회수
        //  __SET(BYTE, CreateDestroy)        // 몬스터 생성/제거
        //  __SET(INT, Type)                  // 타입 (리더 냐 꼬붕이냐)
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
            //__SET(INT, CreateCount)         // 몬스터 생성 갯수
            INT _valueCreateCount;
            archive >> _valueCreateCount;
            partial_info->SetCreateCount(_valueCreateCount);
            //__SET(INT, DelayTime)               // 생성 시간 간격
            INT _valueDelayTime;
            archive >> _valueDelayTime;
            partial_info->SetDelayTime(_valueDelayTime);
            //__SET(INT, LoopCount)               // 몬스터 생성 회수
            INT _valueLoopCount;
            archive >> _valueLoopCount;
            partial_info->SetLoopCount(_valueLoopCount);
            //__SET(BYTE, CreateDestroy)      // 몬스터 생성/제거
            BYTE _valueCreateDestroy;
            archive >> _valueCreateDestroy;
            partial_info->SetCreateDestroy(_valueCreateDestroy);
            //__SET(INT, Type)                    // 타입 (리더 냐 꼬붕이냐)
            INT _valueType;
            archive >> _valueType;
            partial_info->SetType(_valueType);

            return partial_info;
        //}
        //break;

        TYPE_BEGIN(ACTION_DISPLAY_MSGBOX)
            __SET(INT, TextID)
            __SET(BYTE, Target)           // 0:개인 1:전체 2:공격 3:수비
            __SET(INT, DisplayWindow)     // 출력할 창
        TYPE_END

        TYPE_BEGIN(ACTION_REMOVE_ITEM)
            __SET(INT, ItemCode)              
            __SET(INT, ItemNum)               // 아이템 개수
        TYPE_END

        TYPE_BEGIN(ACTION_REWARD_PLAYER)
            __SET(INT, ItemCode)
            __SET(INT, Count)
            __SET(BYTE, IsParty)
            __SET(BYTE, IsDrop)                // 주는방식(0 : 인벤에 넣어서,1 바닥에 떨굼)
        TYPE_END

        TYPE_BEGIN(ACTION_CLEAR_MISSION)
            __SET(BYTE, ResultType)
        TYPE_END

        TYPE_BEGIN(ACTION_PORTAL_RANDOM)
            __SET(INT, MapID)
            __ARRAY_SET(INT, ACTION_PORTAL_RANDOM_INFO::_MAX_AREA_ID_NUM, AreaID)
            __SET(BYTE, Random)               // 랜덤 순차적으로
            __SET(BYTE, Target)               // (0: 개인 1: 파티)
        TYPE_END

        TYPE_BEGIN(ACTION_NPC_RANDOM_AREA_MOVE)
            __ARRAY_SET(INT, ACTION_NPC_RANDOM_AREA_MOVE_INFO::_MAX_AREA_ID_NUM, AreaID)
            __SET(BYTE, MoveOrWarp)           // (0: move 1: warp)
        TYPE_END

        TYPE_BEGIN(ACTION_ACTIVATE_TRIGGER)
            __SET(DWORD, WzTriggerID)
            __SET(BYTE, Flag)             // 0 : 활성, 1 : 비활성
        TYPE_END

        TYPE_BEGIN(ACTION_AREA_DAMAGE)
            __SET(DWORD, ObjectIndex)     
            __SET(FLOAT, Range)
            __SET(BYTE, Target)               // 목표 (개인/파티)       
            __SET(BYTE, DamageType)           // 데미지 타입
            __SET(FLOAT, Damage)
        TYPE_END

        TYPE_BEGIN(ACTION_OPERATE_SWITCH)
            __SET(BYTE, SwitchID)
            __SET(BYTE, OperationType)
            __SET(INT, Argument)              // 매개변수
        TYPE_END

		TYPE_BEGIN(ACTION_OPERATE_SWITCH_EX)
			__SET(BYTE, Target)               // (0: 개인 1: 파티)
			__SET(BYTE, SwitchID)
			__SET(BYTE, OperationType)
			__SET(INT, Argument)              // 매개변수
		TYPE_END

        TYPE_BEGIN(ACTION_OPERATE_VARIABLE)
            __SET(INT, VarID)
            __SET(BYTE, OperationType)
            __SET(INT, Argument)              // 매개변수
        TYPE_END

        TYPE_BEGIN(ACTION_ROAD_CONNECT)       // 길 연결을 설정/해제 한다
            __SET(INT, AreaIndex)
            __SET(BYTE, CanGo)                // 갈 수 있다=0, 없다=1
        TYPE_END

        TYPE_BEGIN(ACTION_CREATE_DIR_MONSTER) // 몬스터 생성한다(방향)
            __SET(INT, MonsterCode)
            __SET(INT, AreaID)
            __SET(BYTE, IsParty)
            __SET(INT, CreateCount)           // 몬스터 생성 갯수
            __SET(INT, DelayTime)         // 생성 시간 간격
            __SET(INT, LoopCount)         // 몬스터 생성 회수
            __SET(FLOAT, DirX)            // x방향
            __SET(FLOAT, DirY)            // y방향
            __SET(FLOAT, DirZ)            // z방향
            __SET(INT, Type)              // Type[0]
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_LIGHT)   // 라이트 설정을 변경한다
            __SET(INT, LightIndex)        // 이 인덱스의 라이트를
            __SET(INT, DelayTime)         // 이만큼의 시간 후에
            __SET(BYTE, Status)           // 0:끈다 1:켠다 2:토글한다
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_OBJECT_TYPE) // 오브젝트 상태를 바꾼다 [타입]
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

        // 기존에 존재하는 액션 트리거
        //BEGIN_DECL_ACTION(ACTION_CREATE_MONSTER)
        //__END_DECL_ACTION

        TYPE_BEGIN(ACTION_CREATE_MONSTER_GROUP)
            __SET(INT, GroupID)
            __SET(INT, AreaID)
            __SET(BYTE, PartyState)
            __SET(INT, GroupCount)
        TYPE_END

        // 유닛 나타남/사라짐 (서버에서는 몬스터 생성/제거와 동일)
        TYPE_BEGIN(ACTION_SHOW_OR_HIDE_UNIT)
            __SET(WzID, MonsterUnitID)
            __SET(INT, IsHide)        //0: 나타나게, 1:사라지게
        TYPE_END

        TYPE_BEGIN(ACTION_SHOW_GUIDE_MSGBOX)
            __SET(LONG, HelpIndex)
        TYPE_END

        // 메시지 박스의 나타남/사라짐
        TYPE_BEGIN(ACTION_SHOW_OR_HIDE_MSGBOX)
            __SET(INT, TextID)
            __SET(BYTE, IsParty)
            __SET(INT, DisplayWindow)     // 출력할 창
            __SET(INT, IsShow)        //1: 나타나게, 0:사라지게
        TYPE_END

        TYPE_BEGIN(ACTION_RANDOM_MONSTER_SPAWN)
            __SET(INT, MonsterID_1);          // 1-5번 몬스터 ID
            __SET(INT, MonsterRate_1);        // 1-5번 몬스터 생성확률
            __SET(INT, MonsterID_2);
            __SET(INT, MonsterRate_2);
            __SET(INT, MonsterID_3);
            __SET(INT, MonsterRate_3);
            __SET(INT, MonsterID_4);
            __SET(INT, MonsterRate_4);
            __SET(INT, MonsterID_5);
            __SET(INT, MonsterRate_5);        
            __SET(INT, AreaID);               // 몬스터 생성 지역
            __SET(BYTE, IsParty);             // 개인 ? 파티 ?
            __SET(INT, MinCreateMonsterCnt);  // 최소 몬스터 생성 갯수
            __SET(INT, MaxCreateMonsterCnt);  // 최소 몬스터 생성 갯수
            __SET(INT, RegenTime)             // 리젠 시간 간격
            __SET(INT, MaxRegenCount)         // 몬스터 총 리젠 회수
            __SET(FLOAT, DirX)                // x방향
            __SET(FLOAT, DirY)                // y방향
            __SET(FLOAT, DirZ)                // z방향
        TYPE_END

        TYPE_BEGIN(ACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR)
            __SET(INT, Floor);            // 1-5번 몬스터 ID
        TYPE_END

        TYPE_BEGIN(ACTION_RANDOM_MAP_MOVE)
            __ARRAY_SET(INT, ACTION_RANDOM_MAP_MOVE_INFO::_MAX_FIELDCODE_ID, FieldID);
            __SET(BYTE, Random);              // 랜덤, 순차적으로
            __SET(WzID, AreaID);              // 영역 ID
            __SET(BYTE, Target);              // (0: 개인 1: 파티)
            __SET(INT, Type);             // (0: 마을, 1:귀속마을(?), 2:배틀맵
        TYPE_END

        TYPE_BEGIN(ACTION_RANDOM_AREA_MOVE)
            __ARRAY_SET(INT, ACTION_RANDOM_AREA_MOVE_INFO::_MAX_AREA_ID, AreaIDX);    // 이동시킬 영역 IDs
            __SET(BYTE, Target);                      // (0: 개인 1: 파티)
            __SET(BYTE, ProcType);                    // (0: 개별, 1: 동일)
        TYPE_END

        TYPE_BEGIN(ACTION_REMOVE_MONSTER)
            __SET(INT, MonsterCode);      // 몬스터코드
            __SET(BYTE, IsExpItemDrop);   // 경험치및아이템드럽 지급 (0: 안한다. 1: 한다)
        TYPE_END

        TYPE_BEGIN(ACTION_ADD_EXP)
            __SET(BYTE, IsParty);             // 개인이냐 파티냐 (0 : 개인, 1:파티)
            __SET(DWORD, Exp);                // Exp 주는 경험치.
        TYPE_END

        TYPE_BEGIN(ACTION_MONSTER_STAT_UP_OR_DOWN)
        __SET(INT, MonsterCode)           // 몬스터코드
        __SET(WORD, AbilityID)            // 어빌리티 ID
        __SET(int, AbilityParameter1)     // 타입설정     1. 수치, 2. 현재치 대비 비율, 3. 최대치 대비 비율
        __SET(int, AbilityParameter2)     // 수치/비율값
        __SET(BYTE, OperationType)        // 연산자 1 : eOPERATION_PLUS(+) , 0 : eOPERATION_MINUS(-) 
        TYPE_END

        TYPE_BEGIN(ACTION_RANDUM_TRIGER_ON_OR_OFF)
            __ARRAY_SET(DWORD, ACTION_RANDUM_TRIGER_ON_OR_OFF_INFO::_MAX_TRIGGER_ID, WzTriggerID)
            __SET(BYTE, TriggerState) 
        TYPE_END

        TYPE_BEGIN(ACTION_PLAY_MUSIC)
        __SET(BYTE, Target);                      // (0: 개인 1: 필드전체)
        __SET(INT, MusCode);                      // (뮤직코드)
        TYPE_END

        TYPE_BEGIN(ACTION_DOWN_OBJETC_HP)
            __SET(DWORD, dwObjKey);					// mapobjectkey
            __SET(BYTE,  btHPPercentage);			    // 하락 수치
        TYPE_END
        TYPE_BEGIN(ACTION_CHANGE_ATTACK_KIND)
            __SET(DWORD, dwObjKey);					// mapobjectkey
            __SET(BYTE,  btPossibility);			    // 0 : 공격 불가, 1 : 공격 가능
        TYPE_END

        TYPE_BEGIN(ACTION_ACTIVE_STAMP)
        TYPE_END

        TYPE_BEGIN(ACTION_ATTACK_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // 타입에 따라 변화.
            __SET(BYTE,  byAttack);			        // eACTION_ATTACK_TYPE
        TYPE_END

        TYPE_BEGIN(ACTION_ACTIVE_SKILL_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // 타입에 따라 변화.
            __SET(DWORD, dwSkillCode);			    // 발동 스킬 코드
        TYPE_END

        TYPE_BEGIN(ACTION_SHOW_MESSAGEBOX_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // 타입에 따라 변화.
            __SET(DWORD, dwMessageCode);			    // 출력할 메시지코드
        TYPE_END

        TYPE_BEGIN(ACTION_SET_ATTACK_STATE_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // 타입에 따라 변화.
            __SET(BYTE,  byDestObjectType);			// 대상 eOBJECTCODE_TYPE
            __SET(DWORD, dwDestObjectKey);			// 대상 타입에 따라 변화.
            __SET(BYTE,  byAttack);			        // eACTION_ATTACK_TYPE
        TYPE_END

        TYPE_BEGIN(ACTION_DESTROY_OBJECTKIND)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // 타입에 따라 변화.
            __SET(BYTE,  byReward);			        // 경험치및아이템드럽 지급 (0: 안한다. 1: 한다)
        TYPE_END

        TYPE_BEGIN(ACTION_CREATE_MONSTER_LINKED_MAPOBJECT)
            __SET(DWORD,  dwObjectKey);				
        TYPE_END

        TYPE_BEGIN(ACTION_CONTROL_TIMER)
            __SET(DWORD, dwTime);                     // 시간 (ms), 생성시 초기시간, 파괴시 유지 시간(delay)
            __SET(BYTE,  byControlType);		        // 변동 eTIMER_CONTROL_TYPE
        TYPE_END                                        // (0:역방향 생성(감소), 1:정방향 생성(증가), 2:파괴)
        //_NA_0_20100705_DOMINATION_EVENT
        TYPE_BEGIN(ACTION_DO_COMMAND)
            __SET(BYTE,  byCommandType);		        // 메시지 종류 
        TYPE_END                                        
        TYPE_BEGIN(ACTION_MOVE_CONTROL)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // 타입에 따라 변화.
            __SET(BYTE,  byMove);			        // eACTION_MOVE_TYPE
        TYPE_END                                        
        
        TYPE_BEGIN(ACTION_ANIMATION_CONTROL)
            __SET(BYTE,  byObjectType);				// eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey);			    // 타입에 따라 변화.
            __SET(DWORD, dwAnimationCode1);	        // 동작 코드1
            __SET(BYTE,  byLoop1);                    //동작 코드1 의 (루프/1회) 여부       
            __SET(DWORD, dwAnimationCode2);	        // 동작 코드2
            __SET(BYTE,  byLoop2);                    //동작 코드2 의 (루프/1회) 여부      
        TYPE_END                                        
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        TYPE_BEGIN(ACTION_NPC_APPLY_DAMAGE)
            __SET(BYTE, ObjectType); // 객체 종류
            __SET(DWORD, ObjectKey); // 객체 종류에 따라 변경
            __SET(INT, Damage); // 고정형 데미지
        TYPE_END
        // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
        TYPE_BEGIN(ACTION_COMPLETE_COLLECTION)
            __ARRAY_SET(CODETYPE, ACTION_COMPLETE_COLLECTION_INFO::kMaxCollectObject, ObjectKey);  // 채집ID
            __SET(BYTE, EnableType); // 객체 종류
        TYPE_END

        TYPE_BEGIN(ACTION_APPLY_HPMPSD)
            __SET(BYTE, TargetType); // TargetType
            __SET(int, TeamKey); // 증가/감소
            __SET(BYTE, PointType); // HP/MP/SD
            __SET(DWORD, Figure);   // 수치
            __SET(BYTE, ApplyType); // 증가/감소
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_OBJECT_HP)
            __SET(DWORD, Objectkey);   // 대상 오브젝트키
            __SET(BYTE, CalcType);     // 비율/수치
            __SET(DWORD, Figure);      // 값
            __SET(BYTE, ApplyType);    // 증감여부
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_RADAR_MAP)
            __SET(WzID, SubMapID); // 서브맵ID
        TYPE_END
        
        TYPE_BEGIN(ACTION_ATTACH_STATE_RANGE)
            __SET(BYTE,  byObjectType); // eOBJECTCODE_TYPE
            __SET(DWORD, dwObjectKey); // 타입에 따라 변화.
            __SET(FLOAT, fRange); // 범위
            __SET(DWORD, dwStateCode); // 부여할 상태코드
            __SET(DWORD, dwTimeDuration); // 상태의 지속시간
            __SET(BYTE, byApplyType); // 0:부여한다, 1:제거한다
        TYPE_END

        TYPE_BEGIN(ACTION_CHANGE_PLAYER_HP)
            __SET(BYTE, TargetType);   // 파티/개인
            __SET(BYTE, CalcType);     // 비율/수치
            __SET(DWORD, Figure);      // 값
            __SET(BYTE, ApplyType);    // 증감여부
        TYPE_END
    TYPESWICH_END

    return NULL;
}
