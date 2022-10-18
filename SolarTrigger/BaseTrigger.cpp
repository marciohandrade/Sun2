#include "StdAfx.h"
#include ".\basetrigger.h"
#include "TriggerCommon.h"
#include "IConditionTrigger.h"
#include "IActionTrigger.h"
#include "ConditionInfo.h"
#include "ActionInfo.h"
#include "TriggerInfo.h"
#include "./TriggerManager.h"

#include <misc.h>

BaseTrigger::BaseTrigger()
    : m_pTriggerManager(NULL)
    , m_pTriggerInfo(NULL)
    , activated_(false)
    , runtime_config_(eRuntimeConfig_Null)
    , need_action_by_runtime_config_(false)
    , deletion_requested_(false)
    , m_state(INACTIVATE_STATE)
    , clicked_player_key_(0)
    , object_key_(0)
{
    ZeroMemory(&active_link_, sizeof(active_link_));
    util::LList::Init(&active_link_);
    active_link_.trigger = this;
    active_link_.ordered_index = -1;
}

BaseTrigger::~BaseTrigger()
{
    util::LList::Delete(&active_link_);
    active_link_.ordered_index = -1;
    //
    Release();
}

void BaseTrigger::Release()
{
    util::LList::Delete(&active_link_);
    active_link_.ordered_index = -1;
    //
    FOREACH_CONTAINER(const CONDITION_TRIGGER_LIST::value_type& node, m_listConditionTrigger,
                      CONDITION_TRIGGER_LIST)
    {
        IConditionTrigger* condition_trigger = node;
        _destroyCondition(condition_trigger);
    }

    FOREACH_CONTAINER(const ACTION_TRIGGER_LIST::value_type& node, m_listActionTrigger,
                      ACTION_TRIGGER_LIST)
    {
        IActionTrigger* action_trigger = node;
        _destroyAction(action_trigger);
    }
    m_listConditionTrigger.clear();
    m_listActionTrigger.clear();

    OnRelease();
}

void BaseTrigger::Init(TriggerManager* trigger_manager, TriggerInfo* trigger_info)
{
    m_pTriggerManager = trigger_manager;

    ASSERT(m_listConditionTrigger.empty());
    ASSERT(m_listActionTrigger.empty());

    if (const TRIGGER_LIST* trigger_list = &m_pTriggerManager->GetTriggerList()) {
        active_link_.ordered_index = static_cast<int>(trigger_list->size());
    };
    //__NA000000_090613_TRIGGER_LOGIC_REARRANGE__
    runtime_config_ = this->eRuntimeConfig_Null;
    need_action_by_runtime_config_ = false;
    deletion_requested_ = false;
    m_state = INACTIVATE_STATE;

    _setInfo(trigger_info);

    if (trigger_info->IsLoop()) {
        runtime_config_ |= eRuntimeConfig_Loop;
    };
    if (trigger_info->IsAND() == false) {
        runtime_config_ |= eRuntimeConfig_Or;
    };

    const ns_trigger::CONDITION_INFO_LIST& condition_info_list = \
        m_pTriggerInfo->GetConditionInfoList();
    m_listConditionTrigger.reserve(condition_info_list.size());
    FOREACH_CONTAINER(const ns_trigger::CONDITION_INFO_LIST::value_type& node,
                      condition_info_list, ns_trigger::CONDITION_INFO_LIST)
    {
        ConditionInfo* condition_info = node;
        _createCondition(condition_info);
    #if defined(_SERVER)
        switch (condition_info->TriggerType())
        {
        case eCONDITION_ENTER_AREA:     runtime_config_ |= eRuntimeConfig_Area; break;
        case eCONDITION_NPC_ENTER_AREA: runtime_config_ |= eRuntimeConfig_Area; break;
        case eCONDITION_PASS_TIME:      runtime_config_ |= eRuntimeConfig_Timer; break;
        };
    #endif
    };

    const ns_trigger::ACTION_INFO_LIST& action_info_list = m_pTriggerInfo->GetActionInfoList();
    m_listActionTrigger.reserve(action_info_list.size());
    FOREACH_CONTAINER(const ns_trigger::ACTION_INFO_LIST::value_type& node,
                      action_info_list, ns_trigger::ACTION_INFO_LIST)
    {
        ActionInfo* action_info = node;
        _createAction(action_info);
    };
}


void BaseTrigger::OnInit()
{
    need_action_by_runtime_config_ = false;
    deletion_requested_ = false;
    clicked_player_key_ = 0;
    object_key_ = 0;

    SetActive(_getInfo()->IsActive());

    // 업데이트 안함!
    m_state = DONTUPDATE_STATE;

    FOREACH_CONTAINER(const CONDITION_TRIGGER_LIST::value_type& node, m_listConditionTrigger,
                      CONDITION_TRIGGER_LIST)
    {
        IConditionTrigger* condition_trigger = node;
        condition_trigger->Clear();  //< satisfied_ = false;
        condition_trigger->OnInit();
        if (condition_trigger->NeedUpdate()) {
            m_state = UPDATE_STATE;
        };
    };
    // 첫번째는 실행 : for always condition
    // 비활성화 트리거의 경우라도 always에 있는 액션을 최초1회 무조건 실행하게 된다.
    Execute();
}

bool BaseTrigger::Execute()
{
#if defined(_SERVER)
    if (_ConditionCheck())
    {
        bool do_action = true;
        if (runtime_config_ == eRuntimeConfig_EventOneTime) {
            do_action = need_action_by_runtime_config_;
        };
        if (do_action) {
            _Action();
        }
        else {
            m_state = ACTIONUPDATE_STATE; // (WAVERIX) (090622) (WARNING) 상태제어...
        };
        return true;
    }
    else
    {
        _ConditionReset();
    }
    return false;
#else
    if (_ConditionCheck())
    {
        _Action();
        return true;
    }
    else
    {
        _ConditionReset();
    }

    return false;
#endif
}


bool BaseTrigger::_ConditionCheck()
{
    if (m_pTriggerInfo->IsAND())
    {
        FOREACH_CONTAINER(const CONDITION_TRIGGER_LIST::value_type& node, m_listConditionTrigger,
                          CONDITION_TRIGGER_LIST)
        {
            IConditionTrigger* condition_trigger = node;
            if (condition_trigger->IsSatisfied() == false) {
                return false;
            };
        };
        return true;
    }
    else // OR composed condtion
    {
        FOREACH_CONTAINER(const CONDITION_TRIGGER_LIST::value_type& node, m_listConditionTrigger,
                          CONDITION_TRIGGER_LIST)
        {
            IConditionTrigger* condition_trigger = node;
            if (condition_trigger->IsSatisfied() != false) {
                return true;
            };
        };
        return false;
    }
}

void BaseTrigger::_Action()
{
    _OnAction();

    FOREACH_CONTAINER(const ACTION_TRIGGER_LIST::value_type& node, m_listActionTrigger,
                      ACTION_TRIGGER_LIST)
    {
        IActionTrigger* action_trigger = node;
        action_trigger->OnAction();
    };

    m_state = ACTIONUPDATE_STATE;
}

void BaseTrigger::_ConditionReset()
{
    // 업데이트 안함!
    m_state = DONTUPDATE_STATE;
    FOREACH_CONTAINER(const CONDITION_TRIGGER_LIST::value_type& node, m_listConditionTrigger,
                      CONDITION_TRIGGER_LIST)
    {
        IConditionTrigger* condition_trigger = node;
        condition_trigger->OnReset();
        if (condition_trigger->NeedUpdate()) {
            m_state = UPDATE_STATE;
        };
    };
}

void BaseTrigger::OnMsg(TRIGGER_MSG* trigger_msg)
{
    FOREACH_CONTAINER(const CONDITION_TRIGGER_LIST::value_type& node, m_listConditionTrigger,
                      CONDITION_TRIGGER_LIST)
    {
        IConditionTrigger* condition_trigger = node;
        // 조건체크후 만족하면 전체 만족 여부 체크
        if (condition_trigger->OnMsg(trigger_msg))
        {
            // 현재 이벤트 메세지로 만족되었다.
            // 전체 만족 체크
            if (Execute()) {
                return;
            };
        }
    };
    return;
}

#ifdef _DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER

namespace util {
;

static TCHAR* GetConditionTypeString(WORD condition_type, TCHAR* message);
static TCHAR* GetActionTypeString(WORD condition_type, TCHAR* message);

};

static TCHAR* util::GetConditionTypeString(WORD condition_type, TCHAR* message)
{
    switch(condition_type)
    {
    case eCONDITION_ALWAYS:			        sprintf(message, "항상\n");
        break;
    case eCONDITION_COMPARE_SWITCH:		    sprintf(message, "스위치 비교\n");
        break;
    case eCONDITION_CLICK_OBJECT:		    sprintf(message, "오브젝트 클릭\n");
        break;
    case eCONDITION_DAMAGE_OBJECT:		    sprintf(message, "오브젝트 데미지 입음\n");
        break;
    case eCONDITION_COUNT_NPCKILL:		    sprintf(message, "몬스터 사냥 마리수\n");
        break;
    case eCONDITION_PASS_TIME:		        sprintf(message, "일정시간이 흐름\n");
        break;
    case eCONDITION_ENTER_AREA:			    sprintf(message, "특정 영역에 들어감\n");
        break;
    case eCONDITION_HAVE_ITEM:			    sprintf(message, "아이템을 소유하고 있다\n");
        break;
    case eCONDITION_MULTI_SWITCH:		    sprintf(message, "다중 스위치 비교\n");
        break;
    case eCONDITION_MISSION_POINT:		    sprintf(message, "미션 포인트 만족\n");
        break;
    case eCONDITION_COMPARE_VARIABLE:	    sprintf(message, "변수 비교\n");
        break;
    case eCONDITION_QUEST_PROGRESS:	        sprintf(message, "Quest 완료 체크 트리거\n");
        break;
    case eCONDITION_CHECK_CLASS_ID:		    sprintf(message, "유저 클래스 설정(의존 트리거)\n");
        break;
    case eCONDITION_NPCKILL_FOR_UNITID:	    sprintf(message, "몬스터 유닛을 잡으면\n");
        break;
    case eCONDITION_LIVE_PARTY_MEMBER:	    sprintf(message, "파티원 생존/사망 상태라면\n");
        break;
    case eCONDITION_CHECK_OBJECT_HP:        sprintf(message, "Mapobject의 HP수치가 %이하일경우\n");
        break;
    case eCONDITION_DESTROY_OBJECT:   	    sprintf(message, "mapobject가 파괴 되었다면.\n");
        break;
    case eCONDITION_CHECK_OBJECTKIND_HP:    sprintf(message, "[UnitID,Mapobject]의 HP수치가 %이하일경우\n");
        break;
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    case eCONDITION_NPC_ENTER_AREA:         sprintf(message, "NPC가 특정 영역에 들어감\n");
        break;
    case eCONDITION_SERVER_EVENT:           sprintf(message, "서버 인벤트가 발생함\n");
        break;
    case eCONDITION_CHECK_TEAM:             sprintf(message, "해당 팀이면\n");
        break;
    case eCONDITION_COMPARE_SWITCH_EX:      sprintf(message, "스위치 비교(확장형)\n");
        break;
    case eCONDITION_MONSTER_ENTER_AREA:      sprintf(message, "몬스터가 특정 영역에 들어감\n");
        break;

    case eCONDITION_MAX:				    sprintf(message, "에러 검출용으로 쓰임\n");
        break;
    default:
        sprintf(message, "알수없는 컨디션 [%d]\n", condition_type);
        break;
    }
    return message;
};

static TCHAR* util::GetActionTypeString(WORD condition_type, TCHAR* message)
{
    switch(condition_type)
    {
    case eACTION_CHANGE_OBJECTANI:          sprintf(message, "오브젝트 애니메이션 변경\n");
        break;
    case eACTION_CHANGE_PATHTILE:           sprintf(message, "길찾기 타일 속성 변경   \n");
        break;
    case eACTION_CHANGE_OBJECTSTATE:        sprintf(message, "오브젝트 상태 변경\n");
        break;
    case eACTION_PLAY_EVENT:                sprintf(message, "이벤트 플레이\n");
        break;
    case eACTION_SET_ISATTACK:              sprintf(message, "공격 가능/불가 설정\n");
        break;
    case eACTION_PORTAL_PLAYER:             sprintf(message, "플레이어 포탈\n");
        break;
    case eACTION_SET_OBJECTTHRUST:          sprintf(message, "오브젝트 밀기 설정\n");
        break;
    case eACTION_APPLY_DAMAGE:              sprintf(message, "데미지 적용\n");
        break;
    case eACTION_REFLECT_DAMAGE:            sprintf(message, "데미지 반사\n");
        break;
    case eACTION_CREATE_MONSTER:            sprintf(message, "몬스터 생성\n");
        break;
    case eACTION_DISPLAY_MSGBOX:            sprintf(message, "메세지 박스 출력\n");
        break;
    case eACTION_REMOVE_ITEM:               sprintf(message, "인벤토리 아이템 제거\n");
        break;
    case eACTION_REWARD_PLAYER:             sprintf(message, "플레이어 보상\n");
        break;
    case eACTION_CLEAR_MISSION:             sprintf(message, "미션 결과\n");
        break;
    case eACTION_PORTAL_RANDOM:             sprintf(message, "랜덤 포탈\n");
        break;
    case eACTION_NPC_RANDOM_AREA_MOVE:      sprintf(message, "NPC 랜덤 영역 이동\n");
        break;
    case eACTION_ACTIVATE_TRIGGER:          sprintf(message, "트리거 활성 비활성\n");
        break;
    case eACTION_AREA_DAMAGE:               sprintf(message, "범위데미지\n");
        break;
    case eACTION_OPERATE_SWITCH:            sprintf(message, "스위치 연산\n");
        break;
    case eACTION_OPERATE_VARIABLE:          sprintf(message, "스위치 설정 변경\n");
        break;
    case eACTION_ROAD_CONNECT:              sprintf(message, "길 연결을 설정/해제한다\n");
        break;
    case eACTION_CREATE_DIR_MONSTER:        sprintf(message, "몬스터를 생성한다(방향)\n");
        break;
    case eACTION_CHANGE_LIGHT:              sprintf(message, "라이트 설정을 변경한다\n");
        break;
    case eACTION_CHANGE_OBJECT_TYPE:        sprintf(message, "오브젝트 상태를 바꾼다\n");
        break;
    case eACTION_RANDOM_MAP_MOVE:           sprintf(message, "랜덤 맵 이동\n");
        break;
    case eACTION_DROP_TRESUREBOX:           sprintf(message, "보물 상자 떨어뜨리기\n");
        break;
    case eACTION_QUEST_SATISFACTION:        sprintf(message, "퀘스트 조건 만족\n");
        break;
    case eACTION_QUEST_GRANT:               sprintf(message, "퀘스트 부여\n");
        break;
    case eACTION_QUEST_SETUP:               sprintf(message, "퀘스트 완료/실패 설정\n");
        break;
    case eACTION_USE_CATEGORY:              sprintf(message, "(필드)의 (카테고리)를 사용한다\n");
        break;
    case eACTION_HOLD_CHARACTER:            sprintf(message, "(Character)의 이동을 (시간)동안 막는다\n");
        break;
    case eACTION_CREATE_MONSTER_GROUP:      sprintf(message, "몬스터그룹 생성\n");
        break;
    case eACTION_SHOW_GUIDE_MSGBOX:         sprintf(message, "안내메시지박스 출력\n");
        break;
    case eACTION_SHOW_OR_HIDE_UNIT:         sprintf(message, "유닛 나타남/사라짐\n");
        break;
    case eACTION_SHOW_OR_HIDE_MSGBOX:       sprintf(message, "메시지 박스의 나타남/사라짐\n");
        break;
    case eACTION_RANDOM_AREA_MOVE:          sprintf(message, "유저를 영역 이동 시킨다\n");
        break;
    case eACTION_RANDOM_MONSTER_SPAWN:      sprintf(message, "몬스터 랜덤 스폰\n");
        break;
    case eACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR: sprintf(message, "eACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR\n");
        break;
    case eACTION_REMOVE_MONSTER:            sprintf(message, "몬스터를 제거\n");
        break;
    case eACTION_ADD_EXP:                   sprintf(message, "경험치 추가\n");
        break;
    case eACTION_MONSTER_STAT_UP_OR_DOWN:   sprintf(message, "몬스터의 능력치중 하나를 증가/감소 시킨다.\n");
        break;
    case eACTION_RANDUM_TRIGER_ON_OR_OFF:   sprintf(message, "해당 트리거중 하나를 on/off 시킨다.\n");
        break;
    case eACTION_PLAY_MUSIC:                sprintf(message, "사운드 파일을 재생한다.\n");
        break;
    case eACTION_DOWN_OBJETC_HP:            sprintf(message, "맵 오브젝트의 HP를 깍는다.\n");
        break;
    case eACTION_CHANGE_ATTACK_KIND:        sprintf(message, "자신을 공격가능[불가능] 하게 만든다.\n");
        break;
    case eACTION_ACTIVE_STAMP:              sprintf(message, "각인가능 상태로 변경시 eCONDITION_DESTROY_OBJECT 상태 일시 동작\n");
        break;
    case eACTION_ATTACK_OBJECTKIND:         sprintf(message, "[몬스터/유닛/오브젝트]를 공격 할 수 [있게/없게] 만든다\n");
        break;
    case eACTION_ACTIVE_SKILL_OBJECTKIND:   sprintf(message, "[몬스터/유닛/오브젝트]가 [스킬ID]스킬을 발동시킨다.\n");
        break;
    case eACTION_SHOW_MESSAGEBOX_OBJECTKIND: sprintf(message, "[몬스터/유닛/오브젝트]의 위치에 [메시지코드]메시지를 보여준다.\n");
        break;
    case eACTION_SET_ATTACK_STATE_OBJECTKIND: sprintf(message, "[몬스터/유닛]가 [몬스터]를 [공격/멈춤]한다.\n");
        break;
    case eACTION_DESTROY_OBJECTKIND:        sprintf(message, "[몬스터/유닛/오브젝트]를 제거하며 보상을 [지급/미지급]한다.\n");
        break;
    case eACTION_CREATE_MONSTER_LINKED_MAPOBJECT: sprintf(message, "[오브젝트]를 생성한다.MapOjbect연계 NPC");
        break;
    case eACTION_CONTROL_TIMER:             sprintf(message, "시계를 보여준다\n");
        break;
    case eACTION_DO_COMMAND:                sprintf(message, "타입에 해당 하는 동작을 실행(트리거를 통한 메시지 전달)\n");
        break;
    case eACTION_MOVE_CONTROL:              sprintf(message, "이동을 제어한다\n");
        break;
    case eACTION_ANIMATION_CONTROL:         sprintf(message, "애니메이션을 제어한다\n");
        break;
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    case eACTION_NPC_APPLY_DAMAGE:
        sprintf(message, _T("몬스터/유닛/오브젝트에게 데미지를 적용한다.\n"));
        break;

        //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    case eACTION_COMPLETE_COLLECTION:
        sprintf(message, _T("채집이 완료되면 동작을 실행한다.(트리거 실행)\n"));
        break;
    case eACTION_APPLY_HPMPSD:
        sprintf(message, _T("HP/MP(SP)/SD를 증감시킨다.\n"));
        break;
    case eACTION_CHANGE_OBJECT_HP:
        sprintf(message, _T("[ObjectKey]오브젝트의 HP를 [비율/수치] [숫자]만큼 [내린다/올린다]\n"));
        break;
    case eACTION_CHANGE_RADAR_MAP:
        sprintf(message, _T("레이더맵(미니맵)을 변경한다.\n"));
        break;
    case eACTION_ATTACH_STATE_RANGE:
        sprintf(message, _T("대상의 범위내에 있는 플레이어에게 상태를 지속시간동안 부여한다.\n"));
        break;

    case eACTION_CHANGE_PLAYER_HP:
        sprintf(message, _T("[파티/개인]의 HP를 [비율/수치] [숫자]만큼 [내린다/올린다]\n"));
        break;

    case eACTION_MAX:                       sprintf(message, "최대값 에러 검출용으로 쓰임\n");
        break;
    default:
        sprintf(message, "알수없는 액션 [%d]\n", condition_type);
        break;
    }
    return message;
};
#endif //_DH_TRIGGER_LOG_MESSAGE

const TCHAR* BaseTrigger::_DisplayTriggerInfo(TCHAR* const making_msg) const
{
#if !defined(_DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER)
    return making_msg;
#endif
#if defined(_DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER)
    TCHAR temp[256] = {0,};
    sprintf(making_msg, _T("TRIGGER_DO_ACTION: ID(%u) 그룹(%s), 카테고리(%s), 컨디션(%d)\n"), 
        m_pTriggerInfo->TriggerID(), m_pTriggerInfo->group_name(), m_pTriggerInfo->CategoryName(), m_pTriggerInfo->condition_index());
    const ns_trigger::CONDITION_INFO_LIST& rConditions = m_pTriggerInfo->GetConditionInfoList();
    FOREACH_CONTAINER(const ns_trigger::CONDITION_INFO_LIST::value_type& rNode,
                      rConditions, ns_trigger::CONDITION_INFO_LIST)
    {
        ConditionInfo* pInfo = rNode;
        util::GetConditionTypeString(pInfo->TriggerType(), temp);
        strcat(making_msg, temp);
    }
    const ns_trigger::ACTION_INFO_LIST& rActions = m_pTriggerInfo->GetActionInfoList();
    FOREACH_CONTAINER(const ns_trigger::ACTION_INFO_LIST::value_type& rNode,
                      rActions, ns_trigger::ACTION_INFO_LIST)
    {
        ActionInfo* pInfo = rNode;
        util::GetActionTypeString(pInfo->TriggerType(), temp);
        strcat(making_msg, temp);
    }
    return making_msg;
#endif //_DH_TRIGGER_LOG_MESSAGE
}

