#include "stdafx.h"
#include "./SCQuickStyleSlotContainer.h"
#include "./SCQuickStyleSlot.h"

//==================================================================================================

SCQuickStyleSlotContainer::SCQuickStyleSlotContainer(void)
{
}

SCQuickStyleSlotContainer::~SCQuickStyleSlotContainer(void)
{
}


SCSlot*
SCQuickStyleSlotContainer::CreateSlot()
{
    return new SCQuickStyleSlot;
}


void
SCQuickStyleSlotContainer::InsertSlot(POSTYPE at_pos, STYLESTREAM& stream)
{
    // slot_in_container
    SCSlot* base_slot = GetValidSlotPtr(NULL, at_pos);
    if (base_slot == NULL) {
        return;
    }
    if (GetEmptySlotNum() == 0) {
        return;
    }
    // desired empty slot
    if (IsEmpty(at_pos) == false) {
        return;
    }
    //
    ++m_nSlotNum;
    SCQuickStyleSlot* quick_style_slot = static_cast<SCQuickStyleSlot*>(base_slot);
    quick_style_slot->Copy(stream);
    quick_style_slot->SetPos(at_pos);
    quick_style_slot->SetSlotIdx(GetSlotIdx());
    quick_style_slot->OnInsert();
    //
    this->OnInsert(*quick_style_slot);
}

BOOL
SCQuickStyleSlotContainer::IsEmpty(POSTYPE at_pos)
{
    SCSlot* base_slot = GetValidSlotPtr(NULL, at_pos);
    if (base_slot == NULL) {
        // problem point... traditional custom is required the result is true
        return true;
    }
    if (static_cast<SCQuickStyleSlot*>(base_slot)->GetOrgCode() == 0) { //...correct?
        return true;
    }
    return false;
}


