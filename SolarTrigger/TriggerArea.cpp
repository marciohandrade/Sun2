#include "stdafx.h"
#include "./TriggerArea.h"
#include "./TriggerAreaManager.h"

//==================================================================================================

void TriggerArea::Clear()
{
    TriggerPlayerLink* it = static_cast<TriggerPlayerLink*>(char_list_root_.next);
    while (it != &char_list_root_) {
        it = it->Delete();
    };
}

// (WAVERIX) (090831) (BUG-FIX) 트리거 관련
void TriggerArea::InsertAreaCondition(TriggerAreaLocalStorage* area_item)
{
    // (NOTE) 초기화시 한번 수행되는 영역이다.
    if (area_item == NULL) { // client?
        return;
    }
    area_item->Delete();
    area_item->last_updated_number_of_chars_ = 0;
    util::LList::AddNext(area_item, &area_conditions_);
}

bool TriggerArea::EnterArea(TriggerAreaManager* area_manager, const TRIGGER_ENTER_AREA* msg)
{
    TriggerPlayerLink* item = msg->m_pLink;
    // (WAVERIX) (090904) (BUG-FIX) 클라이언트 영역 이벤트 처리 순서 (enter->enter->leave) 역전 현상
    // 감안한 추가 처리
    if (item->object_key_)
    {
        TriggerArea* prev_trigger_area = area_manager->FindArea(item->area_id_);
        if (prev_trigger_area != NULL)
        {
            TriggerAreaLocalStorage* it = prev_trigger_area->area_conditions_.next;
            while (it != &prev_trigger_area->area_conditions_)
            {
                if (it->last_updated_number_of_chars_) {
                    --(it->last_updated_number_of_chars_);
                }
                it = it->next;
            }
        }
    }

    TriggerArea* trigger_area = area_manager->FindArea(msg->m_AreaID);
    if (trigger_area == NULL) {
        SUNLOG(eCRITICAL_LOG, "Can't Find Area %d", msg->m_AreaID); // where?
        return false;
    }
    //
    item->Delete();
    item->object_key_ = msg->m_PlayerKey;
    item->area_id_ = trigger_area->area_id_;
    util::LList::AddNext(item, &trigger_area->char_list_root_);
    //
    return true;
}

bool TriggerArea::LeaveArea(TriggerAreaManager* area_manager, const TRIGGER_LEAVE_AREA* msg)
{
    TriggerPlayerLink* item = msg->m_pLink;
    if (util::LList::IsEmpty(item)) {
        return true;
    }
    // Enter(1) -> Enter(2) -> Leave(1)의 형식으로 클라이언트 요청되는 사례가 있다. 
    // (WAVERIX) (090904) (BUG-FIX) Enter에서 Leave조건을 클리어 한다.
    const int area_id = msg->m_AreaID;
    if (item->area_id_ == area_id)
    {
        item->Delete();
        item->object_key_ = 0; //초기값을 명확히 구분할 필요 있다.
        item->area_id_ = 0;

        // (WAVERIX) (090831) (BUG-FIX) 트리거 관련
        //__NAM00000_090621_TRIGGER_LOGIC_REARRANGE2__
        TriggerArea* trigger_area = area_manager->FindArea(area_id);
        if (trigger_area != NULL)
        {
            TriggerAreaLocalStorage* it = trigger_area->area_conditions_.next;
            while (it != &trigger_area->area_conditions_)
            {
                if (it->last_updated_number_of_chars_) {
                    --(it->last_updated_number_of_chars_);
                }
                it = it->next;
            }
        }
    }
    return true;
}

bool TriggerArea::ForceLeave(TriggerAreaManager* area_manager, TriggerPlayerLink* link)
{
    //__NA000000_090613_TRIGGER_LOGIC_REARRANGE2__
    TriggerArea* trigger_area = area_manager->FindArea(link->area_id_);
    // (WAVERIX) (090831) (BUG-FIX) 트리거 관련
    if (trigger_area != NULL)
    {
        TriggerAreaLocalStorage* it = trigger_area->area_conditions_.next;
        while (it != &trigger_area->area_conditions_)
        {
            if (it->last_updated_number_of_chars_) {
                --(it->last_updated_number_of_chars_);
            }
            it = it->next;
        }
    }
    //
    link->object_key_ = 0;
    link->area_id_ = 0;
    link->Delete();
    //
    return true;
}

DWORD TriggerArea::GetPlayerNumber() const
{
    DWORD number_of_players = 0;
    TriggerPlayerLink* it = static_cast<TriggerPlayerLink*>(char_list_root_.next);
    for (; it != &char_list_root_; it = static_cast<TriggerPlayerLink*>(it->next)) {
        ++number_of_players;
    }
    return number_of_players;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
// NOTE: f110311.4L, _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE

void TriggerAreaForNpc::Clear()
{
    TriggerNpcLink* it = static_cast<TriggerNpcLink*>(char_list_root_.next);
    while (it != &char_list_root_) {
        it = it->Delete();
    };
}

// (WAVERIX) (090831) (BUG-FIX) 트리거 관련
void TriggerAreaForNpc::InsertAreaCondition(TriggerAreaLocalStorageForNpc* area_item)
{
    // (NOTE) 초기화시 한번 수행되는 영역이다.
    if (area_item == NULL) { // client?
        return;
    }
    area_item->Delete();
    area_item->last_updated_number_of_chars_ = 0;
    util::LList::AddNext(area_item, &area_conditions_);
}

bool TriggerAreaForNpc::EnterArea(TriggerAreaManagerForNpc* area_manager,
                                  const TRIGGER_NPC_ENTER_AREA* msg)
{
    TriggerNpcLink* item = msg->link;
    // (WAVERIX) (090904) (BUG-FIX) 클라이언트 영역 이벤트 처리 순서 (enter->enter->leave) 역전 현상
    // 감안한 추가 처리
    if (item->object_key_)
    {
        TriggerAreaForNpc* prev_trigger_area = area_manager->FindArea(item->area_id_);
        if (prev_trigger_area != NULL)
        {
            TriggerAreaLocalStorageForNpc* it = prev_trigger_area->area_conditions_.next;
            while (it != &prev_trigger_area->area_conditions_)
            {
                if (it->last_updated_number_of_chars_) {
                    --(it->last_updated_number_of_chars_);
                }
                it = it->next;
            }
        }
    }

    TriggerAreaForNpc* trigger_area = area_manager->FindArea(msg->area_id);
    if (trigger_area == NULL) {
        SUNLOG(eCRITICAL_LOG, "Can't Find Area %d", msg->area_id); // where?
        return false;
    }
    //
    item->Delete();
    item->object_key_ = msg->object_key;
    item->area_id_ = trigger_area->area_id_;
    util::LList::AddNext(item, &trigger_area->char_list_root_);
    //
    return true;
}

bool TriggerAreaForNpc::LeaveArea(TriggerAreaManagerForNpc* area_manager,
                                  const TRIGGER_NPC_LEAVE_AREA* msg)
{
    TriggerNpcLink* item = msg->link;
    if (util::LList::IsEmpty(item)) {
        return true;
    }
    // Enter(1) -> Enter(2) -> Leave(1)의 형식으로 클라이언트 요청되는 사례가 있다. 
    // (WAVERIX) (090904) (BUG-FIX) Enter에서 Leave조건을 클리어 한다.
    const int area_id = msg->area_id;
    if (item->area_id_ == area_id)
    {
        item->Delete();
        item->object_key_ = 0; //초기값을 명확히 구분할 필요 있다.
        item->area_id_ = 0;

        // (WAVERIX) (090831) (BUG-FIX) 트리거 관련
        //__NAM00000_090621_TRIGGER_LOGIC_REARRANGE2__
        TriggerAreaForNpc* trigger_area = area_manager->FindArea(area_id);
        if (trigger_area != NULL)
        {
            TriggerAreaLocalStorageForNpc* it = trigger_area->area_conditions_.next;
            while (it != &trigger_area->area_conditions_)
            {
                if (it->last_updated_number_of_chars_) {
                    --(it->last_updated_number_of_chars_);
                }
                it = it->next;
            }
        }
    }
    return true;
}

bool TriggerAreaForNpc::ForceLeave(TriggerAreaManagerForNpc* area_manager, TriggerNpcLink* link)
{
    //__NA000000_090613_TRIGGER_LOGIC_REARRANGE2__
    TriggerAreaForNpc* trigger_area = area_manager->FindArea(link->area_id_);
    // (WAVERIX) (090831) (BUG-FIX) 트리거 관련
    if (trigger_area != NULL)
    {
        TriggerAreaLocalStorageForNpc* it = trigger_area->area_conditions_.next;
        while (it != &trigger_area->area_conditions_)
        {
            if (it->last_updated_number_of_chars_) {
                --(it->last_updated_number_of_chars_);
            }
            it = it->next;
        }
    }
    //
    link->object_key_ = 0;
    link->area_id_ = 0;
    link->Delete();
    //
    return true;
}

DWORD TriggerAreaForNpc::GetNpcNumber() const
{
    DWORD number_of_chars = 0;
    TriggerNpcLink* it = static_cast<TriggerNpcLink*>(char_list_root_.next);
    for (; it != &char_list_root_; it = static_cast<TriggerNpcLink*>(it->next)) {
        ++number_of_chars;
    }
    return number_of_chars;
}

//==================================================================================================
