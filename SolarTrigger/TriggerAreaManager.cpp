#include "StdAfx.h"
#include ".\triggerareamanager.h"

//==================================================================================================

TriggerAreaManager::TriggerAreaManager()
{
}

TriggerAreaManager::~TriggerAreaManager()
{
}

void TriggerAreaManager::Init()
{
}

void TriggerAreaManager::Release()
{
    FOREACH_CONTAINER(const TRIGGER_AREA_MAP::value_type& node, trigger_areas_, TRIGGER_AREA_MAP)
    {
        TriggerArea* trigger_area = node.second;
        if (trigger_area != NULL) {
            TAllocDelete(TriggerArea, trigger_area);
        }
    }
    trigger_areas_.clear();
}

bool TriggerAreaManager::EnterArea(TRIGGER_ENTER_AREA* msg)
{
    return TriggerArea::EnterArea(this, msg);
}

bool TriggerAreaManager::LeaveArea(TRIGGER_LEAVE_AREA* msg)
{
    return TriggerArea::LeaveArea(this, msg);
}

DWORD TriggerAreaManager::GetPlayerNumber(int area_id) const
{
    TriggerArea* trigger_area = FindArea(area_id);
    if (trigger_area) {
        return trigger_area->GetPlayerNumber();
    };
    return 0;
}

bool TriggerAreaManager::InsertNewArea(int area_id, TriggerAreaLocalStorage* area_item)
{
    TriggerArea* trigger_area = FindArea(area_id);
    if (trigger_area == NULL)
    {
        trigger_area = TAllocNew(TriggerArea)(area_id);
        trigger_areas_.insert(std::make_pair(area_id, trigger_area));
    }
    // (WAVERIX) (090904) (BUG-FIX) 하나의 영역당 이벤트 관리에서 n개의 공간이 존재해도 notify list는
    // 하나만 존재하게 되어 버리는 문제 수정 (객체 생성 다음 라인에서 목록 등록하던 문제 수정)
    trigger_area->InsertAreaCondition(area_item);
    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

TriggerAreaManagerForNpc::TriggerAreaManagerForNpc()
{
}

TriggerAreaManagerForNpc::~TriggerAreaManagerForNpc()
{
}

void TriggerAreaManagerForNpc::Init()
{
}

void TriggerAreaManagerForNpc::Release()
{
    FOREACH_CONTAINER(const TRIGGER_AREA_MAP::value_type& node, trigger_areas_, TRIGGER_AREA_MAP)
    {
        TriggerAreaForNpc* trigger_area = node.second;
        if (trigger_area != NULL) {
            TAllocDelete(TriggerAreaForNpc, trigger_area);
        }
    }
    trigger_areas_.clear();
}

bool TriggerAreaManagerForNpc::EnterArea(TRIGGER_NPC_ENTER_AREA* msg)
{
    return TriggerAreaForNpc::EnterArea(this, msg);
}

bool TriggerAreaManagerForNpc::LeaveArea(TRIGGER_NPC_LEAVE_AREA* msg)
{
    return TriggerAreaForNpc::LeaveArea(this, msg);
}

DWORD TriggerAreaManagerForNpc::GetNpcNumber(int area_id)
{
    TriggerAreaForNpc* trigger_area = FindArea(area_id);
    if (trigger_area) {
        return trigger_area->GetNpcNumber();
    };
    return 0;
}

bool TriggerAreaManagerForNpc::InsertNewArea(int area_id, TriggerAreaLocalStorageForNpc* area_item)
{
    TriggerAreaForNpc* trigger_area = FindArea(area_id);
    if (trigger_area == NULL)
    {
        trigger_area = TAllocNew(TriggerAreaForNpc)(area_id);
        trigger_areas_.insert(std::make_pair(area_id, trigger_area));
    }
    // (WAVERIX) (090904) (BUG-FIX) 하나의 영역당 이벤트 관리에서 n개의 공간이 존재해도 notify list는
    // 하나만 존재하게 되어 버리는 문제 수정 (객체 생성 다음 라인에서 목록 등록하던 문제 수정)
    trigger_area->InsertAreaCondition(area_item);
    return true;
}

