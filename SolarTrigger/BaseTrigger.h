#ifndef _SOLARTRIGGER_BASETRIGGER_H
#define _SOLARTRIGGER_BASETRIGGER_H
#pragma once

//==================================================================================================
//  BaseTrigger class
/**
    @author	Cho Jae Sik < jscho@webzen.com >
    @since	2007. 3. 28
    @remark
    - Trigger�� ��� ����� ������ ���� �⺻ Ŭ����.
    @note
    - 
    @history 
    - 
    - 2007. 10. xx  waverix, rearrange
    - 2009. 06. 13  waverix, rearrange
*/
//==================================================================================================

#include "TriggerCommon.h"
#include "TriggerInfo.h"

class ConditionInfo;
class ActionInfo;
class IConditionTrigger;
class IActionTrigger;
class TriggerManager;
class BaseTrigger;
struct TRIGGER_MSG;

typedef STLX_VECTOR<IConditionTrigger*> CONDITION_TRIGGER_LIST;
typedef STLX_VECTOR<IActionTrigger*>    ACTION_TRIGGER_LIST;

namespace ns_trigger {
;

struct ActiveTriggerLink : public util::__LList<ActiveTriggerLink>
{
    BaseTrigger* trigger; // debug link
    int ordered_index; // nth order in trigger list
};

};

//==================================================================================================
//
class BaseTrigger
{
public:
    BaseTrigger();
    virtual ~BaseTrigger();

public:
    enum eTRIGGER_STATE
    {
        INACTIVATE_STATE,   // �ʱ�ȭ�� �Ǿ����� ���� Ʈ���Ű� Active���� �ʾҴ�.
        DONTUPDATE_STATE,   // ������Ʈ ���� �ʴ´�.
        UPDATE_STATE,       // ������Ʈ �Ѵ�.
        ACTIONUPDATE_STATE, // Action Update�� �Ѵ�.
        TERMINATING_STATE,  // Action�� ����
    };

    //__NA000000_090613_TRIGGER_LOGIC_REARRANGE__
    // (WAVERIX) (090621) (NOTE) �ش� Ʈ���Ÿ� �׳� ��ȹ ������� ���۽����� ��,
    //ó����(��� + ��Ŷ + a)�� �����ϴ� ������ �ذ��ϱ� ����. Ư�� ��ʸ� �ذ��ϴ� ���·� ����.
    enum eRuntimeConfig
    {
        eRuntimeConfig_Null         = 0,
        eRuntimeConfig_Loop         = 1 << 0,   // Attribute: Loop
        eRuntimeConfig_Or           = 1 << 1,   // Attribute: Or
        eRuntimeConfig_Area         = 1 << 2,   // Condition Trigger 'Area'
        eRuntimeConfig_Timer        = 1 << 3,   // Condition Trigger 'Timer'
        eRuntimeConfig_EventOneTime = eRuntimeConfig_Loop | eRuntimeConfig_Area,
    };

    void Init(TriggerManager* pTriggerManager, TriggerInfo* pInfo);
    virtual BOOL Update();

    // ���� : Active�ÿ� OnInit(), DeactiveȤ�� Destroy�ÿ� OnRelease()
    virtual void OnInit();
    virtual void OnRelease();

    const WzID TriggerID() const;
    eTRIGGER_STATE GetTriggerState();

    void SetActive(BOOL bActive);
    bool IsActive() const;
    bool IsNextActive() const;
    bool IsLoop() const;

    void OnMsg(TRIGGER_MSG* trigger_msg);
    bool Execute();
    // CHANGES: f110720.3L, prevent the corrupted list container state
    void RequestDeletion();
    bool CheckRequestedDeletion() const;

    DWORD GetClickedPlayerKey();
    void SetClickedPlayerKey(DWORD player_key);
    DWORD GetObjectKey();
    void SetObjectKey(DWORD object_key);
    bool IsAlwaysTrigger() const;
    void NeedActionByRuntimeConfig();

    //_DH_TRIGGER_LOG_MESSAGE_IN_TRIGGER
    const TCHAR* _DisplayTriggerInfo(TCHAR* const making_msg) const;

private:
    void Release(); // changes to private accessibility
    bool _ConditionCheck();
    void _ConditionReset();
    void _Action();
    TriggerInfo* _getInfo();
    void _setInfo(TriggerInfo* pInfo);

protected:
    virtual void _createCondition(ConditionInfo* condition_info) = 0;
    virtual void _createAction(ActionInfo* action_info) = 0;
    virtual void _destroyCondition(IConditionTrigger* condition_trigger) = 0;
    virtual void _destroyAction(IActionTrigger* action_trigger) = 0;
    virtual void _OnAction() = 0;

private:
    ns_trigger::ActiveTriggerLink active_link_;
protected:
    TriggerManager* m_pTriggerManager;
    TriggerInfo*    m_pTriggerInfo;
    bool activated_;      //< ������ Ȱ��ȭ ���� (WAVERIX) (090621) (CHANGES) BOOL->BOOLEAN
    BYTE runtime_config_;  // Ʈ���� ���� ����: __NA000000_090613_TRIGGER_LOGIC_REARRANGE__
    bool need_action_by_runtime_config_;
    bool deletion_requested_; // CHANGES: f110720.3L
    eTRIGGER_STATE m_state;
    CONDITION_TRIGGER_LIST m_listConditionTrigger;
    ACTION_TRIGGER_LIST    m_listActionTrigger;

    // Ʈ���� �������� �ʿ��� �ɹ�, (WAVERIX) (090621) (CHANGES) ~ClickedUserKey -> ~ClickedPlayerKey
    DWORD clicked_player_key_;
    DWORD object_key_;
    //
    friend class TriggerManager;
    __DISABLE_COPY(BaseTrigger);
};

//==================================================================================================

inline BOOL BaseTrigger::Update()
{
    return false;
}

inline void BaseTrigger::OnRelease()
{
}

inline const WzID BaseTrigger::TriggerID() const
{
    return m_pTriggerInfo->TriggerID();
}

inline BaseTrigger::eTRIGGER_STATE BaseTrigger::GetTriggerState()
{
    return m_state;
}

inline void BaseTrigger::SetActive(BOOL activated)
{
    activated_ = (activated != false);
}

inline bool BaseTrigger::IsActive() const
{
    return activated_;
}

inline bool BaseTrigger::IsNextActive() const
{
    return m_pTriggerInfo->IsNextActive();
}

inline bool BaseTrigger::IsLoop() const
{
    return m_pTriggerInfo->IsLoop();
}

inline DWORD BaseTrigger::GetClickedPlayerKey()
{
    return clicked_player_key_;
}

inline void BaseTrigger::SetClickedPlayerKey(DWORD player_key)
{
    clicked_player_key_ = player_key;
}

inline DWORD BaseTrigger::GetObjectKey()
{
    return object_key_;
}

inline void BaseTrigger::SetObjectKey(DWORD object_key)
{
    object_key_ = object_key;
}

inline bool BaseTrigger::IsAlwaysTrigger() const
{
    return m_pTriggerInfo->IsAlways();
}

inline void BaseTrigger::NeedActionByRuntimeConfig()
{
    need_action_by_runtime_config_ = true;
}

// CHANGES: f110720.3L, prevent the corrupted list container state
inline void BaseTrigger::RequestDeletion()
{
    deletion_requested_ = true;
};

inline bool BaseTrigger::CheckRequestedDeletion() const
{
    return deletion_requested_;
};

inline TriggerInfo* BaseTrigger::_getInfo()
{
    return m_pTriggerInfo;
}

inline void BaseTrigger::_setInfo(TriggerInfo* trigger_info)
{
    m_pTriggerInfo = trigger_info;
}

#endif //_SOLARTRIGGER_BASETRIGGER_H
