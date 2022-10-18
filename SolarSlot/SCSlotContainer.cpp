#include "stdafx.h"
#include "./SCSlotContainer.h"
#include "./SCSlotStruct.h"
//
#include <BitStream.hpp>

//==================================================================================================

SCSlotContainer::SCSlotContainer()
    : integrity_reference_(slot_index_),
      invalid_slot_(NULL),
      slot_index_(SI_MAX),
      m_nSlotNum(0),
      max_slot_num_(0)  // add init since 100307
      //m_nResizeSlotNum(0)// removed
{
}

SCSlotContainer::~SCSlotContainer()
{
    // changed by _SOLAR_SLOT_INTEGRITY_CONTAINER_PROCESS_
    ReleaseSlots(); // changes Release to ReleaseSlots, to solve polimorphism problem
    SAFE_DELETE(invalid_slot_);
}

void
SCSlotContainer::Release()
{
    ReleaseSlots();
}

// (f100307.8) add 'ReleaseSlots' to solve can't call state by polymorphism problem
// must be call in release condition state
void
SCSlotContainer::ReleaseSlots()
{
    FOREACH_CONTAINER(SLOT_LINEAR_LIST::value_type node, slot_list_, SLOT_LINEAR_LIST)
    {
        delete node.slot;
    }
    slot_list_.resize(0);
    m_nSlotNum = 0;
    max_slot_num_ = 0;
    //m_nResizeSlotNum = 0;
}

void
SCSlotContainer::ClearAll()
{
    m_nSlotNum = 0;
    // this logic has so re-use that \
    // { slot_index_, max_slot_num_, max_slot_num_, m_nResizeSlotNum } are necessary
    FOREACH_CONTAINER(SLOT_LINEAR_LIST::value_type node, slot_list_, SLOT_LINEAR_LIST)
    {   // node = SCSlot*
        //if (node == NULL) { // preemptive logic to support slot factory
        //    continue;
        //}
        node.slot->Clear();
    }
    if (invalid_slot_ != NULL) {
        invalid_slot_->Clear(); // to support prevent crash fault by invalid reference in client
    }
}

void
SCSlotContainer::Init(POSTYPE max_slot_size, SLOTIDX slot_index)
{
    ClearAll(); //side-effect : m_nSlotNum = 0
    slot_index_ = slot_index; //SetSlotIdx(slot_index);
    max_slot_num_ = max_slot_size;
    //m_nResizeSlotNum = max_slot_size;
    //
    slot_list_.reserve(max_slot_size);
    Resize(max_slot_size);
    // (f100308.3) ... what to do invalid_slot_?
}

void
SCSlotContainer::SetMaxSlotNum(POSTYPE max_slot_size) 
{
    // (CHANGES) (f100329.5L) change concept. change real list size on call SetMaxSlotNum
    // prevent invalid iterate by slot_list
    const SLOT_LINEAR_LIST::size_type original_size = slot_list_.size();
    const SLOT_LINEAR_LIST::size_type changed_size = max_slot_size;
    if (changed_size == original_size) {
        return;
    }
    if (changed_size < original_size) //reduce size
    {
        for (SLOT_LINEAR_LIST::size_type loop = original_size - changed_size; loop; --loop)
        {
            const SlotNode& node = slot_list_.back();
            delete node.slot;
            slot_list_.pop_back();
        }
    }
    else //if (changed_size > original_size)
    {
        for (SLOT_LINEAR_LIST::size_type pos = original_size; pos < changed_size; ++pos)
        {
            SlotNode slot_node = { 0, };
            slot_node.pos = static_cast<POSTYPE>(pos);
            slot_node.slot = CreateSlot();
            slot_node.slot->Clear();
            slot_list_.push_back(slot_node);
        }
    }

    // custom code
    max_slot_num_ = max_slot_size;
}

void
SCSlotContainer::Resize(POSTYPE max_slot_size)
{
    // CHANGES: move main logic to SetMaxSlotNum, because of similar to SetMaxSlotNum and \
    // to support direct iterating operation for slot_list_
    SetMaxSlotNum(max_slot_size);
}

// (f100308.2)
SCSlot* SCSlotContainer::GetValidSlotPtr(RC::eSLOT_INSERT_RESULT* result, POSTYPE at_pos) const
{
    // ValidPos(at_pos) == false
    if (at_pos >= max_slot_num_)
    {
        if (result != NULL) {
            *result = RC::RC_INSERT_SLOT_AT_POS_OVER;
        }
        return NULL;
    }
    // to get SCSlot derived object
    const SlotNode& slot_node = slot_list_[at_pos];
    assert(slot_node.slot != NULL);
    if (result != NULL) {
        *result = RC::RC_INSERT_SLOT_SUCCESS;
    }
    return slot_node.slot;
}

// pSlot for not allocated in heap
RC::eSLOT_INSERT_RESULT
SCSlotContainer::InsertSlot(POSTYPE at_pos, SCSlot& IN inputed_slot)
{
    // (f100308.2) change logic to solve GetSlot problem
    RC::eSLOT_INSERT_RESULT result = RC::RC_INSERT_SLOT_SUCCESS;
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(&result, at_pos);
    if (slot == NULL) {
        return result;
    }
    if (m_nSlotNum >= max_slot_num_) {
        return RC::RC_INSERT_SLOT_AT_POS_OVER;
    }
    // desired empty slot
    if (IsEmpty(at_pos) == false) {
        return RC::RC_INSERT_SLOT_AT_POS_EMPTY;
    }
    //
    ++m_nSlotNum; // pre-increment before insert
    slot->Copy(inputed_slot);
    slot->SetPos(at_pos);
    slot->SetSlotIdx(slot_index_);
    slot->OnInsert();
    this->OnInsert(*slot); // on event handling for slot container
    //
    return RC::RC_INSERT_SLOT_SUCCESS;
}

BOOL
SCSlotContainer::DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete)
{
    // (f100308.2) change logic to solve GetSlot problem
    if (this->m_nSlotNum == 0) { // is empty container?
        return false;
    }
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    //              desired not empty slot
    if (slot == NULL || IsEmpty(at_pos)) {
        return false;
    }

    slot->OnDelete();
    this->OnDelete(*slot); // on event handling for slot container

    // align info
    if (slot_clone_before_delete)
    {
        // (NOTE) need more detailed analysis
        if (FlowControl::FCAssert(slot->GetSlotType() ==
            slot_clone_before_delete->GetSlotType()))
        {
            slot_clone_before_delete->Copy(*slot);
        }
        else
        {
            slot_clone_before_delete->Clear();
        }
    }

    // post process
    slot->Clear();
    --m_nSlotNum; // pre-decrement after delete

    return true;
}

void
SCSlotContainer::UpdateSlot(POSTYPE at_pos, SCSlot& IN inputed_slot)
{
    // (f100308.2) change logic to solve GetSlot problem
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    if (FlowControl::FCAssert(slot != NULL) == false) {
        return;
    }
    // desired not empty slot
    if (IsEmpty(at_pos)) {
        return;
    }
    //
    slot->Copy(inputed_slot);
    slot->SetPos(at_pos);
    slot->SetSlotIdx(slot_index_);
    //
    return;
}

void
SCSlotContainer::UpdateSlot(POSTYPE at_pos, int changed_num)
{
    // (f100308.2) change logic to solve GetSlot problem
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    if (FlowControl::FCAssert(slot != NULL) == false) {
        return;
    }
    // desired not empty slot
    if (IsEmpty(at_pos)) {
        return;
    }
    //
    int calculated = static_cast<int>(slot->GetNum()) + changed_num;
    if (calculated < 0 || calculated > INVALID_POSTYPE_VALUE) {
        return; // problem point... pre-condition check is important.
    }
    // (WARNING) if current GetNum() == 0 then occurred problem... need more thinking.
    slot->SetNum(static_cast<POSTYPE>(calculated));
    this->OnUpdate(*slot, changed_num);
}

SCSlot&
SCSlotContainer::GetSlot(POSTYPE at_pos)
{
    // (f100308.2) change logic to solve GetSlot problem
    // slot_in_container, and replace variable 'm_nResizeSlotNum' to 'max_slot_num_'

    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    // empty slot support

    if (FlowControl::FCAssert(slot != NULL) == false)
    {
        if (invalid_slot_ == NULL) {
            invalid_slot_ = CreateSlot();
        }
        // invalid slot specification concept have \
        // to support empty and invalid position in the slot container.
        invalid_slot_->Clear();
        invalid_slot_->SetSlotIdx(this->slot_index_);
        invalid_slot_->SetPos(INVALID_POSTYPE_VALUE);
        return *invalid_slot_;
    }
    // success
    return *slot;
}

BOOL
SCSlotContainer::GetEmptyPos(POSTYPE& empty_pos_result)
{
    SLOT_LINEAR_LIST::const_iterator it = slot_list_.begin(),
                                     end = slot_list_.end();
    for (int pos = 0; it != end; ++it, ++pos)
    {
        const SlotNode& slot_node = *it;
        if (slot_node.slot->GetCode() == 0) { // is empty
            empty_pos_result = static_cast<POSTYPE>(pos);
            return true;
        }
    }
    return false;
}

BOOL
SCSlotContainer::IsEmpty(POSTYPE at_pos)
{
    // (f100308.2) change logic to solve GetSlot problem
    // (WARNING) can't assured slot integrity
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(NULL, at_pos);
    if (slot == NULL) {
        // problem point... traditional custom is required the result is true
        return true;
    }
    if (slot->GetCode() == 0) {
        return true;
    }
    return false;
}

//bitstream
void
SCSlotContainer::Serialize(BitStream& bitstream, eSLOT_SERIALIZE serialize_type)
{
    // specification
    // |num:8| scslot derived instance | ...
    //       |pos:8|~~ variable size ~~|
    if (serialize_type & eSLOT_SERIALIZE_LOAD)
    {
        POSTYPE number_of_slots = GetSlotNum();
        bitstream.Write(&number_of_slots, 8);
        const POSTYPE max_num_slots = GetMaxSlotNum();
        for (POSTYPE pos = 0; pos < max_num_slots; ++pos)
        {
            if (IsEmpty(pos) == false)
            {
                bitstream.Write(&pos, 8);
                Serialize(pos, bitstream, serialize_type);
            }
        }
    }
    else if (serialize_type & eSLOT_SERIALIZE_SAVE)
    {
        POSTYPE max_slot_num = 0;
        bitstream.Read(&max_slot_num, 8);

        POSTYPE pos_in_stream = 0;
        for (int loop = max_slot_num; loop; --loop)
        {
            bitstream.Read(&pos_in_stream, 8);
            Serialize(pos_in_stream, bitstream, serialize_type);
        }
    }
}

void
SCSlotContainer::Serialize(POSTYPE pos, BitStream& bitstream, eSLOT_SERIALIZE serialize_type)
{
    // (f100308.2) change logic to solve GetSlot problem
    RC::eSLOT_INSERT_RESULT result = RC::RC_INSERT_SLOT_SUCCESS;
    // slot_in_container
    SCSlot* slot = GetValidSlotPtr(&result, pos);
    if (slot == NULL) {
        return;
    }
    if (serialize_type & eSLOT_SERIALIZE_LOAD)
    {
        slot->CopyOut(bitstream, serialize_type);
    }
    else if (serialize_type & eSLOT_SERIALIZE_SAVE)
    {
        ++m_nSlotNum;
        slot->Copy(bitstream, serialize_type);
        slot->SetPos(pos);
        slot->SetSlotIdx(this->slot_index_);
        slot->OnInsert();
        this->OnInsert(*slot);
    }
}

