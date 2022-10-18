#include "stdafx.h"
#include ".\SCPerkSlot.h"

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_
#include <SkillSystem/PerkInfoParser.h>

//==================================================================================================

SCPerkSlot::SCPerkSlot()
{
    ZeroMemory(&slot_data_, sizeof(slot_data_));
}

SCPerkSlot::~SCPerkSlot()
{
}

void SCPerkSlot::Copy(const PerkData& IN stream)
{
    ZeroMemory(&slot_data_, sizeof(slot_data_));
    //
    const PerkID& perk_id = stream.perk_id_;
    if (perk_id.group_ == 0) {
        return;
    };
    slot_data_.perk_data = stream;
    const bool is_default_perk_index = (perk_id.group_ < PerkDataList::eActiveGroup01);
    if (is_default_perk_index == false) {
        slot_data_.perk_info = NULL;
        slot_data_.is_active_slot = true;
        return;
    }
    //
    PerkInfoParser* const perk_parser = PerkInfoParser::Instance();
    const PerkInfo* perk_info = perk_parser->FindPerkInfo(perk_id);
    assert(perk_info);
    slot_data_.perk_info = perk_info;
}

bool SCPerkSlot::LinkSlot(const SCPerkSlot* link_slot)
{
    if (!FlowControl::FCAssert(link_slot)) {
        return false;
    }
    if (slot_data_.is_active_slot == false) {
        return false;
    }
    slot_data_.link_slot = link_slot;
    return true;
}

bool SCPerkSlot::Unlink()
{
    if (slot_data_.is_active_slot == false) {
        return false;
    }
    slot_data_.link_slot = NULL;
    return true;
}

//==================================================================================================

// moved to header
/*
void
SCPerkSlot::CopyOut(STYLESTREAM& OUT stream)
{
    CopyMemory(&stream, &style_stream_, sizeof(stream));
}

void
SCPerkSlot::Clear()
{
    SCSlot::Clear();
    ZeroMemory(&style_stream_, sizeof(style_stream_));
}

void
SCPerkSlot::Copy(SCSlot& IN slot)
{
    SCSlot::Copy(slot);
    SCPerkSlot& style_slot = (SCPerkSlot &)slot;
    CopyMemory(&style_stream_, &style_slot.style_stream_, sizeof(style_stream_));
}
*/

//==================================================================================================

#endif //_NA002217_100728_EP2_PERK_SYSTEM_
