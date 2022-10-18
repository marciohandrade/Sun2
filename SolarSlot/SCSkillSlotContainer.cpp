#include "StdAfx.h"
#include ".\scskillslotcontainer.h"
#include ".\SCSkillSlot.h"
#include <SkillInfoParser.h>

SCSkillSlotContainer::SCSkillSlotContainer()
{
}

SCSkillSlotContainer::~SCSkillSlotContainer()
{
    skill_slots_.clear();
}

SCSlot* SCSkillSlotContainer::CreateSlot()
{
    return new SCSkillSlot;
}

void SCSkillSlotContainer::Init(POSTYPE max_slot_size, SLOTIDX slot_index)
{
    SCSlotContainer::Init(max_slot_size, slot_index);
    skill_slots_.clear();
}

void SCSkillSlotContainer::InsertSlot(POSTYPE at_pos, SKILLSTREAM& stream)
{
    SCSlot* const slot = GetValidSlotPtr(NULL, at_pos);
    if (slot == NULL) 
    {
        return;
    }
    if (m_nSlotNum >= GetMaxSlotNum()) 
    {
        return;
    }
    if (IsEmpty(at_pos) == false) 
    {
        return;
    }
    ++m_nSlotNum; // pre-increment before insert
    SCSkillSlot* skill_slot = static_cast<SCSkillSlot*>(slot);
    skill_slot->Copy(stream);
    skill_slot->SetPos(at_pos);
    skill_slot->SetSlotIdx(GetSlotIdx());
    skill_slot->OnInsert();
    this->OnInsert(*skill_slot);

    const SLOTCODE code = stream.Part.wCode;
    skill_slots_[code] = skill_slot;
}

RC::eSLOT_INSERT_RESULT SCSkillSlotContainer::InsertSlot(POSTYPE at_pos, SCSlot& IN slot)
{
    const RC::eSLOT_INSERT_RESULT result = SCSlotContainer::InsertSlot(at_pos, slot);
    if (result == RC::RC_INSERT_SLOT_SUCCESS)
    {
        SCSkillSlot& new_slot = static_cast<SCSkillSlot&>(GetSlot(at_pos));
        skill_slots_[new_slot.GetCode()] = &new_slot;
    }
    return result;
}

BOOL SCSkillSlotContainer::DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete)
{
    if (ValidPos(at_pos) == false) 
    {
        return false;
    }
    
    const SCSkillSlot& skill_slot = static_cast<SCSkillSlot&>(GetSlot(at_pos));
    const SLOTCODE skill_code = skill_slot.GetCode();
    
    if (SCSlotContainer::DeleteSlot(at_pos, slot_clone_before_delete))
    {
        skill_slots_.erase(skill_code);
        return true;
    }
    return false;
}

void SCSkillSlotContainer::ClearAll()
{
    SCSlotContainer::ClearAll();
    skill_slots_.clear();
}

SCSkillSlot* SCSkillSlotContainer::GetSkillSlotByClass(const SLOTCODE skill_class_code) const
{
    const SkillInfoParser* const skill_parser = SkillInfoParser::Instance();
    for (SkillSlots::const_iterator it = skill_slots_.begin(); it != skill_slots_.end(); ++it)
    {
        const SLOTCODE skill_code = it->first;
        const ROOT_SKILLINFO* const skill_info = skill_parser->GetInfo(skill_code);
        if (skill_info != NULL)
        {
            if (skill_info->m_SkillClassCode == skill_class_code)
            {
                return it->second;
            }
        }
    }
    return NULL;
}

// (f100311.2) moved from header file
SCSkillSlot* SCSkillSlotContainer::GetSkillSlotByCode(const SLOTCODE skill_code) const
{
    SkillSlots::const_iterator it = skill_slots_.find(skill_code);
    if (it != skill_slots_.end())
    {
        return it->second;
    }
    return NULL;
}

void SCSkillSlotContainer::SerializeSkillStream(
    POSTYPE pos, SKILLSTREAM* IN skill_stream, eSERIALIZE serialize_type)
{
    if (serialize_type == SERIALIZE_LOAD)
    {
        SCSlot* const slot = GetValidSlotPtr(NULL, pos);
        if (!FlowControl::FCAssert(slot != NULL)) 
        {
            ZeroMemory(skill_stream, sizeof(*skill_stream)); // no ctor stream structure
            return;
        }
        // with empty slot
        SCSkillSlot* const skill_slot = static_cast<SCSkillSlot*>(slot);
        skill_slot->CopyOut(*skill_stream);
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        InsertSlot(pos, *skill_stream);
    }
    // impossible unreached point
    ASSERT(serialize_type == SERIALIZE_LOAD || serialize_type == SERIALIZE_STORE);
}

void SCSkillSlotContainer::SerializeSkillStreamAll(
    SKILLSTREAM* IN skill_stream, WORD stream_size, eSERIALIZE serialize_type)
{
    const POSTYPE upperbound = GetMaxSlotNum();
    if (serialize_type == SERIALIZE_LOAD)
    {
        if (!FlowControl::FCAssert(
            IsBadWritePtr(skill_stream, stream_size) == false &&
            (stream_size / sizeof(*skill_stream) >= upperbound)))
        {
            __debugbreak();
        }
        ZeroMemory(skill_stream, stream_size);
        for (POSTYPE pos = 0; pos < upperbound; ++pos)
        {
            if (IsEmpty(pos) == false) 
            {
                SerializeSkillStream(pos, &skill_stream[pos], serialize_type);
            }
        }
        return;
    }
    else if (serialize_type == SERIALIZE_STORE)
    {
        if (!FlowControl::FCAssert(IsBadReadPtr(skill_stream, stream_size) == false)) 
        {
            __debugbreak();
        }
        for (POSTYPE pos = 0; pos < upperbound; ++pos) 
        {
            SerializeSkillStream(pos, &skill_stream[pos], serialize_type);
        }
        return;
    }
    // impossible unreached point
    ASSERT(serialize_type == SERIALIZE_LOAD || serialize_type == SERIALIZE_STORE);
}

//_NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
void SCSkillSlotContainer::Copy(SCSkillSlotContainer* INOUT target) const
{
    target->ClearAll();
    target->Init(GetMaxSlotNum(), GetSlotIdx());

    for (SkillSlots::const_iterator it = skill_slots_.begin(); it != skill_slots_.end(); ++it)
    {
        SCSkillSlot* const skill_slot = it->second;
        target->InsertSlot(skill_slot->GetPos(), *skill_slot);
    }
}