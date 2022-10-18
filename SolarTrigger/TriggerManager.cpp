#include "stdafx.h"
#include "./TriggerManager.h"
#include "BaseTrigger.h"

TriggerManager::TriggerManager()
{
    trigger_searcher_.Initialize(30);
    util::LList::Init(&active_trigger_list_);
}

TriggerManager::~TriggerManager()
{
}

void TriggerManager::Release()
{
    FOREACH_CONTAINER(const TRIGGER_LIST::value_type& node, m_listTrigger, TRIGGER_LIST)
    {
        BaseTrigger* trigger = node.second;
        assert(trigger != NULL);
        trigger->Release();
        FreeTrigger(trigger);
    };

    m_listTrigger.clear();
    ASSERT(util::LList::IsEmpty(&active_trigger_list_));
    //active_trigger_list_.clear();
    trigger_area_manager_.Release();
    trigger_searcher_.clear();
}

bool TriggerManager::InsertTrigger(BaseTrigger* trigger)
{
    if (trigger == NULL) {
        return false;
    };
    const WzID trigger_id = trigger->TriggerID();
    if (BaseTrigger* found = FindTrigger(trigger_id)) {
        return false;
    };

    ns_trigger::TriggerListNode node = { 0, };
    node.first = trigger_id;
    node.second = trigger;
    m_listTrigger.push_back(node);
    trigger_searcher_.insert(trigger_id, trigger);

    return true;
}

BaseTrigger* TriggerManager::FindNextTrigger(WzID current_trigger_id)
{
    BaseTrigger* trigger = this->FindTrigger(current_trigger_id);
    if (trigger == NULL) {
        return NULL;
    };
    int current_index = trigger->active_link_.ordered_index;
    const int max_numbers = static_cast<int>(m_listTrigger.size());
    if ((current_index < 0) || (current_index + 1 >= max_numbers)) {
        return NULL;
    };
    const ns_trigger::TriggerListNode& node = m_listTrigger[current_index + 1];
    return node.second;
}

bool TriggerManager::CreateArea(int area_id, TriggerAreaLocalStorage* area_item)
{
    return trigger_area_manager_.InsertNewArea(area_id, area_item);
}

bool TriggerManager::OnLeaveArea(TRIGGER_LEAVE_AREA* trigger_msg)
{
    bool succeeded = trigger_area_manager_.LeaveArea(trigger_msg);

    if (succeeded) {
        OnMsg(trigger_msg);
    };

    return succeeded;
}

bool TriggerManager::OnEnterArea(TRIGGER_ENTER_AREA* trigger_msg)
{
    bool succeeded = trigger_area_manager_.EnterArea(trigger_msg);

    if (succeeded) {
        OnMsg(trigger_msg);
    }

    return succeeded;
}

DWORD TriggerManager::GetPlayerNumForAreaID(int area_id) const
{
    return trigger_area_manager_.GetPlayerNumber(area_id);
}

bool TriggerManager::Disconnected(TriggerPlayerLink* const link)
{
    return TriggerArea::ForceLeave(&trigger_area_manager_, link);
}

bool TriggerManager::InsertActiveTrigger(WzID trigger_id)
{
    //1. 현재 Active Trigger에는 존재하지 말아야 하며..
    //2. 트리거 목록에는 있어야 한다.

    BaseTrigger* trigger = FindTrigger(trigger_id);
    if (trigger == NULL) {
        return false;
    };
    if (trigger->activated_ != false) {
        return false;
    };

    trigger->OnInit();

    util::LList::Delete(&trigger->active_link_);
    util::LList::AddPrev(&trigger->active_link_, &this->active_trigger_list_);
    trigger->SetActive(true);

    return true;
}

// CHANGES: f110720.3L, prevent the corrupted list container state
void TriggerManager::RequestRemoveActiveTrigger(WzID trigger_id) const
{
    BaseTrigger* trigger = this->FindActiveTrigger(trigger_id);
    if (trigger == NULL) {
        return;
    };
    ASSERT(util::LList::IsEmpty(&trigger->active_link_) == false);
    trigger->RequestDeletion();
};

// WARNING: f110721.2L, this method must be used in GameDominationField.
// side-effect: it call the Trigger::Release(triggers cleared)
void TriggerManager::RemoveAllActiveTrigger()
{
    ns_trigger::ActiveTriggerLink* it = this->active_trigger_list_.next;
    const ns_trigger::ActiveTriggerLink* const end = \
        static_cast<ns_trigger::ActiveTriggerLink*>(&this->active_trigger_list_);
    while (it != end)
    {
        assert(CONTAINING_RECORD(it, BaseTrigger, active_link_) == it->trigger);
        BaseTrigger* trigger = it->trigger;
        it = it->next;
        trigger->Release();
        trigger->SetActive(false);
    };
}

bool TriggerManager::ActiveNextTrigger(WzID current_trigger_id)
{
    BaseTrigger* next_trigger = FindNextTrigger(current_trigger_id);
    if (next_trigger)
    {
        // 개발에서 로그가 많이 남아 주석으로 막았습니다. [5/4/2010 lst1024]
        //SUNLOG(eDEV_LOG, "[TriggerManagerEx::Update] Active Next Trigger, ID = %u ", pNextTrigger->TriggerID());
        return InsertActiveTrigger(next_trigger->TriggerID());
    }

    return false;
}


void TriggerManager::OnMsg(TRIGGER_MSG* trigger_msg)
{
    if (trigger_msg == NULL || util::LList::IsEmpty(&active_trigger_list_)) {
        return;
    };
    // CHANGES: f110720.3L, prevent the corrupted list container state
    ns_trigger::ActiveTriggerLink* it = this->active_trigger_list_.next;
    const ns_trigger::ActiveTriggerLink* const end = \
        static_cast<ns_trigger::ActiveTriggerLink*>(&this->active_trigger_list_);
    for (; it != end; it = it->next)
    {
        assert(CONTAINING_RECORD(it, BaseTrigger, active_link_) == it->trigger);
        BaseTrigger* trigger = it->trigger;
        // pre-check
        if (trigger->CheckRequestedDeletion()) {
            continue;
        };
        //
        trigger->OnMsg(trigger_msg);
    };
}





