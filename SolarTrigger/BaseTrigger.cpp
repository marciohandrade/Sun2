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

    // ������Ʈ ����!
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
    // ù��°�� ���� : for always condition
    // ��Ȱ��ȭ Ʈ������ ���� always�� �ִ� �׼��� ����1ȸ ������ �����ϰ� �ȴ�.
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
            m_state = ACTIONUPDATE_STATE; // (WAVERIX) (090622) (WARNING) ��������...
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
    // ������Ʈ ����!
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
        // ����üũ�� �����ϸ� ��ü ���� ���� üũ
        if (condition_trigger->OnMsg(trigger_msg))
        {
            // ���� �̺�Ʈ �޼����� �����Ǿ���.
            // ��ü ���� üũ
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
    case eCONDITION_ALWAYS:			        sprintf(message, "�׻�\n");
        break;
    case eCONDITION_COMPARE_SWITCH:		    sprintf(message, "����ġ ��\n");
        break;
    case eCONDITION_CLICK_OBJECT:		    sprintf(message, "������Ʈ Ŭ��\n");
        break;
    case eCONDITION_DAMAGE_OBJECT:		    sprintf(message, "������Ʈ ������ ����\n");
        break;
    case eCONDITION_COUNT_NPCKILL:		    sprintf(message, "���� ��� ������\n");
        break;
    case eCONDITION_PASS_TIME:		        sprintf(message, "�����ð��� �帧\n");
        break;
    case eCONDITION_ENTER_AREA:			    sprintf(message, "Ư�� ������ ��\n");
        break;
    case eCONDITION_HAVE_ITEM:			    sprintf(message, "�������� �����ϰ� �ִ�\n");
        break;
    case eCONDITION_MULTI_SWITCH:		    sprintf(message, "���� ����ġ ��\n");
        break;
    case eCONDITION_MISSION_POINT:		    sprintf(message, "�̼� ����Ʈ ����\n");
        break;
    case eCONDITION_COMPARE_VARIABLE:	    sprintf(message, "���� ��\n");
        break;
    case eCONDITION_QUEST_PROGRESS:	        sprintf(message, "Quest �Ϸ� üũ Ʈ����\n");
        break;
    case eCONDITION_CHECK_CLASS_ID:		    sprintf(message, "���� Ŭ���� ����(���� Ʈ����)\n");
        break;
    case eCONDITION_NPCKILL_FOR_UNITID:	    sprintf(message, "���� ������ ������\n");
        break;
    case eCONDITION_LIVE_PARTY_MEMBER:	    sprintf(message, "��Ƽ�� ����/��� ���¶��\n");
        break;
    case eCONDITION_CHECK_OBJECT_HP:        sprintf(message, "Mapobject�� HP��ġ�� %�����ϰ��\n");
        break;
    case eCONDITION_DESTROY_OBJECT:   	    sprintf(message, "mapobject�� �ı� �Ǿ��ٸ�.\n");
        break;
    case eCONDITION_CHECK_OBJECTKIND_HP:    sprintf(message, "[UnitID,Mapobject]�� HP��ġ�� %�����ϰ��\n");
        break;
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    case eCONDITION_NPC_ENTER_AREA:         sprintf(message, "NPC�� Ư�� ������ ��\n");
        break;
    case eCONDITION_SERVER_EVENT:           sprintf(message, "���� �κ�Ʈ�� �߻���\n");
        break;
    case eCONDITION_CHECK_TEAM:             sprintf(message, "�ش� ���̸�\n");
        break;
    case eCONDITION_COMPARE_SWITCH_EX:      sprintf(message, "����ġ ��(Ȯ����)\n");
        break;
    case eCONDITION_MONSTER_ENTER_AREA:      sprintf(message, "���Ͱ� Ư�� ������ ��\n");
        break;

    case eCONDITION_MAX:				    sprintf(message, "���� ��������� ����\n");
        break;
    default:
        sprintf(message, "�˼����� ����� [%d]\n", condition_type);
        break;
    }
    return message;
};

static TCHAR* util::GetActionTypeString(WORD condition_type, TCHAR* message)
{
    switch(condition_type)
    {
    case eACTION_CHANGE_OBJECTANI:          sprintf(message, "������Ʈ �ִϸ��̼� ����\n");
        break;
    case eACTION_CHANGE_PATHTILE:           sprintf(message, "��ã�� Ÿ�� �Ӽ� ����   \n");
        break;
    case eACTION_CHANGE_OBJECTSTATE:        sprintf(message, "������Ʈ ���� ����\n");
        break;
    case eACTION_PLAY_EVENT:                sprintf(message, "�̺�Ʈ �÷���\n");
        break;
    case eACTION_SET_ISATTACK:              sprintf(message, "���� ����/�Ұ� ����\n");
        break;
    case eACTION_PORTAL_PLAYER:             sprintf(message, "�÷��̾� ��Ż\n");
        break;
    case eACTION_SET_OBJECTTHRUST:          sprintf(message, "������Ʈ �б� ����\n");
        break;
    case eACTION_APPLY_DAMAGE:              sprintf(message, "������ ����\n");
        break;
    case eACTION_REFLECT_DAMAGE:            sprintf(message, "������ �ݻ�\n");
        break;
    case eACTION_CREATE_MONSTER:            sprintf(message, "���� ����\n");
        break;
    case eACTION_DISPLAY_MSGBOX:            sprintf(message, "�޼��� �ڽ� ���\n");
        break;
    case eACTION_REMOVE_ITEM:               sprintf(message, "�κ��丮 ������ ����\n");
        break;
    case eACTION_REWARD_PLAYER:             sprintf(message, "�÷��̾� ����\n");
        break;
    case eACTION_CLEAR_MISSION:             sprintf(message, "�̼� ���\n");
        break;
    case eACTION_PORTAL_RANDOM:             sprintf(message, "���� ��Ż\n");
        break;
    case eACTION_NPC_RANDOM_AREA_MOVE:      sprintf(message, "NPC ���� ���� �̵�\n");
        break;
    case eACTION_ACTIVATE_TRIGGER:          sprintf(message, "Ʈ���� Ȱ�� ��Ȱ��\n");
        break;
    case eACTION_AREA_DAMAGE:               sprintf(message, "����������\n");
        break;
    case eACTION_OPERATE_SWITCH:            sprintf(message, "����ġ ����\n");
        break;
    case eACTION_OPERATE_VARIABLE:          sprintf(message, "����ġ ���� ����\n");
        break;
    case eACTION_ROAD_CONNECT:              sprintf(message, "�� ������ ����/�����Ѵ�\n");
        break;
    case eACTION_CREATE_DIR_MONSTER:        sprintf(message, "���͸� �����Ѵ�(����)\n");
        break;
    case eACTION_CHANGE_LIGHT:              sprintf(message, "����Ʈ ������ �����Ѵ�\n");
        break;
    case eACTION_CHANGE_OBJECT_TYPE:        sprintf(message, "������Ʈ ���¸� �ٲ۴�\n");
        break;
    case eACTION_RANDOM_MAP_MOVE:           sprintf(message, "���� �� �̵�\n");
        break;
    case eACTION_DROP_TRESUREBOX:           sprintf(message, "���� ���� ����߸���\n");
        break;
    case eACTION_QUEST_SATISFACTION:        sprintf(message, "����Ʈ ���� ����\n");
        break;
    case eACTION_QUEST_GRANT:               sprintf(message, "����Ʈ �ο�\n");
        break;
    case eACTION_QUEST_SETUP:               sprintf(message, "����Ʈ �Ϸ�/���� ����\n");
        break;
    case eACTION_USE_CATEGORY:              sprintf(message, "(�ʵ�)�� (ī�װ�)�� ����Ѵ�\n");
        break;
    case eACTION_HOLD_CHARACTER:            sprintf(message, "(Character)�� �̵��� (�ð�)���� ���´�\n");
        break;
    case eACTION_CREATE_MONSTER_GROUP:      sprintf(message, "���ͱ׷� ����\n");
        break;
    case eACTION_SHOW_GUIDE_MSGBOX:         sprintf(message, "�ȳ��޽����ڽ� ���\n");
        break;
    case eACTION_SHOW_OR_HIDE_UNIT:         sprintf(message, "���� ��Ÿ��/�����\n");
        break;
    case eACTION_SHOW_OR_HIDE_MSGBOX:       sprintf(message, "�޽��� �ڽ��� ��Ÿ��/�����\n");
        break;
    case eACTION_RANDOM_AREA_MOVE:          sprintf(message, "������ ���� �̵� ��Ų��\n");
        break;
    case eACTION_RANDOM_MONSTER_SPAWN:      sprintf(message, "���� ���� ����\n");
        break;
    case eACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR: sprintf(message, "eACTION_TOWER_OF_TRIAL_SHOW_THE_FLOOR\n");
        break;
    case eACTION_REMOVE_MONSTER:            sprintf(message, "���͸� ����\n");
        break;
    case eACTION_ADD_EXP:                   sprintf(message, "����ġ �߰�\n");
        break;
    case eACTION_MONSTER_STAT_UP_OR_DOWN:   sprintf(message, "������ �ɷ�ġ�� �ϳ��� ����/���� ��Ų��.\n");
        break;
    case eACTION_RANDUM_TRIGER_ON_OR_OFF:   sprintf(message, "�ش� Ʈ������ �ϳ��� on/off ��Ų��.\n");
        break;
    case eACTION_PLAY_MUSIC:                sprintf(message, "���� ������ ����Ѵ�.\n");
        break;
    case eACTION_DOWN_OBJETC_HP:            sprintf(message, "�� ������Ʈ�� HP�� ��´�.\n");
        break;
    case eACTION_CHANGE_ATTACK_KIND:        sprintf(message, "�ڽ��� ���ݰ���[�Ұ���] �ϰ� �����.\n");
        break;
    case eACTION_ACTIVE_STAMP:              sprintf(message, "���ΰ��� ���·� ����� eCONDITION_DESTROY_OBJECT ���� �Ͻ� ����\n");
        break;
    case eACTION_ATTACK_OBJECTKIND:         sprintf(message, "[����/����/������Ʈ]�� ���� �� �� [�ְ�/����] �����\n");
        break;
    case eACTION_ACTIVE_SKILL_OBJECTKIND:   sprintf(message, "[����/����/������Ʈ]�� [��ųID]��ų�� �ߵ���Ų��.\n");
        break;
    case eACTION_SHOW_MESSAGEBOX_OBJECTKIND: sprintf(message, "[����/����/������Ʈ]�� ��ġ�� [�޽����ڵ�]�޽����� �����ش�.\n");
        break;
    case eACTION_SET_ATTACK_STATE_OBJECTKIND: sprintf(message, "[����/����]�� [����]�� [����/����]�Ѵ�.\n");
        break;
    case eACTION_DESTROY_OBJECTKIND:        sprintf(message, "[����/����/������Ʈ]�� �����ϸ� ������ [����/������]�Ѵ�.\n");
        break;
    case eACTION_CREATE_MONSTER_LINKED_MAPOBJECT: sprintf(message, "[������Ʈ]�� �����Ѵ�.MapOjbect���� NPC");
        break;
    case eACTION_CONTROL_TIMER:             sprintf(message, "�ð踦 �����ش�\n");
        break;
    case eACTION_DO_COMMAND:                sprintf(message, "Ÿ�Կ� �ش� �ϴ� ������ ����(Ʈ���Ÿ� ���� �޽��� ����)\n");
        break;
    case eACTION_MOVE_CONTROL:              sprintf(message, "�̵��� �����Ѵ�\n");
        break;
    case eACTION_ANIMATION_CONTROL:         sprintf(message, "�ִϸ��̼��� �����Ѵ�\n");
        break;
    // _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER
    case eACTION_NPC_APPLY_DAMAGE:
        sprintf(message, _T("����/����/������Ʈ���� �������� �����Ѵ�.\n"));
        break;

        //_NA_004777_20120428_ADD_COMPLETE_COLLECTION_TRIGGER
    case eACTION_COMPLETE_COLLECTION:
        sprintf(message, _T("ä���� �Ϸ�Ǹ� ������ �����Ѵ�.(Ʈ���� ����)\n"));
        break;
    case eACTION_APPLY_HPMPSD:
        sprintf(message, _T("HP/MP(SP)/SD�� ������Ų��.\n"));
        break;
    case eACTION_CHANGE_OBJECT_HP:
        sprintf(message, _T("[ObjectKey]������Ʈ�� HP�� [����/��ġ] [����]��ŭ [������/�ø���]\n"));
        break;
    case eACTION_CHANGE_RADAR_MAP:
        sprintf(message, _T("���̴���(�̴ϸ�)�� �����Ѵ�.\n"));
        break;
    case eACTION_ATTACH_STATE_RANGE:
        sprintf(message, _T("����� �������� �ִ� �÷��̾�� ���¸� ���ӽð����� �ο��Ѵ�.\n"));
        break;

    case eACTION_CHANGE_PLAYER_HP:
        sprintf(message, _T("[��Ƽ/����]�� HP�� [����/��ġ] [����]��ŭ [������/�ø���]\n"));
        break;

    case eACTION_MAX:                       sprintf(message, "�ִ밪 ���� ��������� ����\n");
        break;
    default:
        sprintf(message, "�˼����� �׼� [%d]\n", condition_type);
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
    sprintf(making_msg, _T("TRIGGER_DO_ACTION: ID(%u) �׷�(%s), ī�װ�(%s), �����(%d)\n"), 
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

