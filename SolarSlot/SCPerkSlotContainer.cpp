#include "stdafx.h"
#include "./SCPerkSlotContainer.h"
#include <algorithm>

#include "./SCPerkSlot.h"

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_

#include <SkillSystem/PerkInfoParser.h>
#include <SkillSystem/PerkDataContainer.h>
#include <Attribute.h>

//==================================================================================================

namespace nsSlot {
;
/*
// NOTE, test code
struct PerkControlMessengerInSlot : public nsSkill::IPerkControlMessenger
{
    PerkControlMessengerInSlot(SCPerkSlotContainer* perk_slot_container)
        : perk_slot_container_(perk_slot_container),
          attributes_(perk_slot_container ? perk_slot_container->inter_.attributes : NULL),
          perk_control_event_(NULL)
    {}
    virtual SCPerkSlotContainer* GetPerkSlotContainer() const {
        return perk_slot_container_;
    }
    virtual Attributes* GetOwnerAttributes() const {
        return attributes_;
    }
    virtual nsSkill::IPerkControlEvent* GetPerkControlEvent() const {
        return perk_control_event_;
    }
    //
    SCPerkSlotContainer* perk_slot_container_;
    Attributes* attributes_;
    nsSkill::IPerkControlEvent* perk_control_event_;
};
*/
}; //end of namespace

//==================================================================================================

SCPerkSlotContainer::SCPerkSlotContainer()
    : perk_data_container_(NULL)
{
    ZeroMemory(&inter_, sizeof(inter_));
}

SCPerkSlotContainer::~SCPerkSlotContainer()
{
    SAFE_DELETE(perk_data_container_);
}

//==================================================================================================

void SCPerkSlotContainer::Init(POSTYPE, SLOTIDX)
{
    assert(!"blocked");
    __debugbreak();
}

//prevent an interface relate to a bitstream
void SCPerkSlotContainer::Serialize(BitStream&, eSLOT_SERIALIZE)
{
    assert(!"blocked");
    __debugbreak();
}

//prevent an interface relate to a bitstream
void SCPerkSlotContainer::Serialize(POSTYPE, BitStream&, eSLOT_SERIALIZE)
{
    assert(!"blocked");
    __debugbreak();
}

//==================================================================================================

SCSlot* SCPerkSlotContainer::CreateSlot()
{
    return new SCPerkSlot;
}

void SCPerkSlotContainer::Release()
{
    SCSlotContainer::Release();
    SAFE_DELETE(perk_data_container_);
    ZeroMemory(&inter_, sizeof(inter_));
}

void SCPerkSlotContainer::ClearAll()
{
    SCSlotContainer::ClearAll();
    if (perk_data_container_ != NULL)
    {
        perk_data_container_->Clear();
    }    
    ZeroMemory(&inter_, sizeof(inter_));
}
/*
void SCPerkSlotContainer::InsertSlot(POSTYPE at_pos, STYLESTREAM& stream)
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
*/

BOOL SCPerkSlotContainer::ValidState()
{
    return false;
}

BOOL SCPerkSlotContainer::IsLocked(POSTYPE at_pos)
{
    __UNUSED(at_pos);
    return false;
}

void SCPerkSlotContainer::SetLock(POSTYPE at_pos, BOOL val)
{
    __UNUSED((at_pos, val));
}

BOOL SCPerkSlotContainer::IsEmpty(POSTYPE at_pos)
{
    SCSlot* base_slot = GetValidSlotPtr(NULL, at_pos);
    if (base_slot == NULL) {
        // problem point... traditional custom is required the result is true
        return true;
    }
    const PerkID& perk_id = static_cast<SCPerkSlot*>(base_slot)->slot_data_.perk_data.perk_id_;
    if (perk_id.group_ == 0) { //...correct?
        return true;
    }
    return false;
}

namespace nsSlot {
;
struct ApplyResult {
    ApplyResult(RC::eITEM_RESULT* result)
        : result_(RC::RC_ITEM_SUCCESS),
            result_link_(result)
    {}
    ~ApplyResult()
    {
        if (result_link_) {
            *result_link_ = result_;
        }
    }
    //
    RC::eITEM_RESULT result_;
    RC::eITEM_RESULT* result_link_;
};

}; //end of namespace

SCPerkSlot* SCPerkSlotContainer::GetActivePartSlot(RC::eITEM_RESULT* result,
                                                   POSTYPE active_slot_pos, bool with_unlink) const
{
    nsSlot::ApplyResult apply_result(result);
    const size_t kNumberOfActiveSlots = _countof(PerkDataList::kActiveSlotPositions);
    if (active_slot_pos < PerkDataList::kActiveSlotPositions[0] ||
        active_slot_pos > PerkDataList::kActiveSlotPositions[kNumberOfActiveSlots - 1])
    {
        apply_result.result_ = RC::RC_ITEM_INVALIDPOS;
        return NULL;
    }
    //
    const POSTYPE* const end = &PerkDataList::kActiveSlotPositions[kNumberOfActiveSlots];
    const POSTYPE* found_index = std::find(PerkDataList::kActiveSlotPositions, end, active_slot_pos);
    if (found_index == end) {
        apply_result.result_ = RC::RC_ITEM_INVALIDPOS;
        return NULL;
    }
    //
    SCSlot* base_slot = GetValidSlotPtr(NULL, active_slot_pos);
    if (base_slot == NULL) {
        apply_result.result_ = RC::RC_ITEM_INVALIDPOS;
        return NULL;
    }
    SCPerkSlot* perk_slot = static_cast<SCPerkSlot*>(base_slot);
    const SCPerkSlot::SlotData& slot_data = perk_slot->slot_data_;
    assert(slot_data.is_active_slot);
    if (with_unlink == false && slot_data.link_slot == NULL) {
        apply_result.result_ = RC::RC_ITEM_INVALIDPOS;
        return NULL;
    }

    return perk_slot;
}

SCPerkSlot* SCPerkSlotContainer::GetRegisterablePartSlot(RC::eITEM_RESULT* result,
                                                         POSTYPE inactive_slot_pos,
                                                         bool with_empty) const
{
    nsSlot::ApplyResult apply_result(result);
    BOOST_STATIC_ASSERT(kNumberOfDefaultSlots == _countof(PerkDataList::kDefaultSlotPositions) &&
                        kNumberOfCashSlots == _countof(PerkDataList::kCashSlotPositions));
    //
    bool in_range_default = (inactive_slot_pos >= PerkDataList::kDefaultSlotPositions[0] &&
        inactive_slot_pos <= PerkDataList::kDefaultSlotPositions[kNumberOfDefaultSlots - 1]);
    //
    bool in_range_cash = !in_range_default;
    if (in_range_default == false) {
        in_range_cash = (inactive_slot_pos >= PerkDataList::kCashSlotPositions[0] &&
            inactive_slot_pos <= PerkDataList::kCashSlotPositions[kNumberOfCashSlots - 1]);
    }
    if (in_range_default == false && in_range_cash == false) {
        apply_result.result_ = RC::RC_ITEM_INVALIDPOS;
        return NULL;
    }
    //
    const POSTYPE* found_index = NULL;
    if (in_range_default)
    {
        const POSTYPE* const end = &PerkDataList::kDefaultSlotPositions[kNumberOfDefaultSlots];
        found_index = std::find(PerkDataList::kDefaultSlotPositions, end, inactive_slot_pos);
        if (found_index == end) {
            found_index = NULL;
        }
    }
    else //in_range_cash
    {
        const POSTYPE* const end = &PerkDataList::kCashSlotPositions[kNumberOfCashSlots];
        found_index = std::find(PerkDataList::kCashSlotPositions, end, inactive_slot_pos);
        if (found_index == end) {
            found_index = NULL;
        }
    }
    if (found_index == NULL) {
        apply_result.result_ = RC::RC_ITEM_INVALIDPOS;
        return NULL;
    }
    //
    SCSlot* base_slot = GetValidSlotPtr(NULL, inactive_slot_pos);
    if (base_slot == NULL) {
        apply_result.result_ = RC::RC_ITEM_INVALIDPOS;
        return NULL;
    }
    SCPerkSlot* perk_slot = static_cast<SCPerkSlot*>(base_slot);
    const SCPerkSlot::SlotData& slot_data = perk_slot->slot_data_;
    if (with_empty == false && slot_data.perk_info == 0) {
        apply_result.result_ = RC::RC_ITEM_INVALIDPOS;
        return NULL;
    }
    
    return perk_slot;
}

//==================================================================================================

void SCPerkSlotContainer::StorePerk(const PerkDataList& perk_data_list, DWORD char_key)
{
    if (perk_data_container_ == NULL) {
        perk_data_container_ = new nsSkill::PerkDataContainer;
        ZeroMemory(&inter_, sizeof(inter_));
    }
    //
    //nsSlot::PerkControlMessengerInSlot perk_data_control_event(this);
    //
    perk_data_container_->StorePerk(perk_data_list, messenger_);
    perk_data_container_->set_pass_key(char_key);
}

void SCPerkSlotContainer::SetSlotPosByTypes(int number_of_default,
                                            int number_of_cash,
                                            int number_of_active)
{
    // NOTE: removed by changes because of using a fixed size slot container
    __UNUSED((number_of_default, number_of_cash, number_of_active));
    assert(number_of_default <= this->kNumberOfDefaultSlots &&
           number_of_cash <= this->kNumberOfCashSlots &&
           number_of_active <= this->kNumberOfActiveSlots);
    inter_.slot_pos_by_types[0] = PerkDataList::kDefaultSlotPositions[0];
    inter_.slot_pos_by_types[1] = PerkDataList::kCashSlotPositions[0];
    inter_.slot_pos_by_types[2] = PerkDataList::kActiveSlotPositions[0];

    inter_.number_of_slots_by_types[0] = _countof(PerkDataList::kDefaultSlotPositions);
    inter_.number_of_slots_by_types[1] = _countof(PerkDataList::kCashSlotPositions);
    inter_.number_of_slots_by_types[2] = _countof(PerkDataList::kActiveSlotPositions);
/*
    // NOTE, removed by changes because of using a fixed size slot container
    inter_.slot_pos_by_types[0] = 0;
    inter_.slot_pos_by_types[1] = static_cast<POSTYPE>(this->kNumberOfDefaultSlots);
    inter_.slot_pos_by_types[2] = \
        static_cast<POSTYPE>(this->kNumberOfDefaultSlots + this->kNumberOfCashSlots);
    inter_.number_of_slots_by_types[0] = static_cast<POSTYPE>(this->kNumberOfDefaultSlots);
    inter_.number_of_slots_by_types[1] = static_cast<POSTYPE>(this->kNumberOfCashSlots);
    inter_.number_of_slots_by_types[2] = static_cast<POSTYPE>(this->kNumberOfActiveSlots);
*/
    this->Resize(static_cast<POSTYPE>(this->kMaxNumberOfFixedSlots));
}

// NOTE: f101029.1L, a check routine for a client whether a player request already a perk process
bool SCPerkSlotContainer::IsRequestTransaction() const
{
    if (perk_data_container_ == NULL) {
        return true;
    }
    return perk_data_container_->IsRequestTransaction();
}

//==================================================================================================

// register a cash perk slot using a cash item
bool SCPerkSlotContainer::RequestPlants(PerkControl* req_info, const POSTYPE from_inven_item_pos)
{
    req_info->Clear();
    req_info->request = req_info->eReq_Plants;
    //
    //nsSlot::PerkControlMessengerInSlot perk_messenger(this);
    bool result_inter_op = perk_data_container_->RequestPlants(\
        req_info, from_inven_item_pos, messenger_);
    if (result_inter_op == false) {
        return false;
    }
    return true;
}

// reserved, ... change to a cash item?
bool SCPerkSlotContainer::RequestUproot(PerkControl* req_info,
                                        const POSTYPE from_perk_slot_pos,
                                        const POSTYPE to_inven_empty_pos)
{
    req_info->Clear();
    req_info->request = req_info->eReq_Uproot;
    //
    if (ValidPos(from_perk_slot_pos) == false || IsEmpty(from_perk_slot_pos)) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_INVALIDPOS;
        return false;
    };
    const SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(GetSlot(from_perk_slot_pos));
    const SCPerkSlot::SlotData& slot_data = perk_slot.slot_data_;
    if (slot_data.perk_info == NULL) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = static_cast<uint16_t>(
            slot_data.is_active_slot ? RC::RC_ITEM_INVALIDPOS : RC::RC_ITEM_EMPTYSLOT);
        return false;
    }
    //
    //nsSlot::PerkControlMessengerInSlot perk_messenger(this);
    bool result_inter_op = perk_data_container_->RequestUproot(\
        req_info, slot_data.perk_data.perk_id_, to_inven_empty_pos, messenger_);
    if (result_inter_op == false) {
        return false;
    }
    return true;
}

// move to an active slot so that change to active
bool SCPerkSlotContainer::RequestRegister(PerkControl* req_info,
                                          POSTYPE from_slot_pos, POSTYPE to_active_slot_pos)
{
    req_info->Clear();
    req_info->request = req_info->eReq_Register;
    // constants
    const bool kFindOptionExistOnly = false; // default, cash slots
    const bool kFindOptionWithUnlink = true; // active slots
    //
    RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;
    // a field of 'from_slot_pos' convers indexes 0 through kNumberOfDefaultSlots(15) or
    //                            convers indexes 20 through kCashSlotPositions(3) or
    const SCPerkSlot* from_perk_slot = \
        this->GetRegisterablePartSlot(&result, from_slot_pos, kFindOptionExistOnly);
    if (from_perk_slot == NULL)
    {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = static_cast<uint16_t>(result);
        return false;
    };
    //
    const SCPerkSlot* to_perk_slot = \
        this->GetActivePartSlot(&result, to_active_slot_pos, kFindOptionWithUnlink);
    if (to_perk_slot == NULL)
    {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = static_cast<uint16_t>(result);
        return false;
    };

    const SCPerkSlot::SlotData& from_slot_data = from_perk_slot->slot_data_;
    const SCPerkSlot::SlotData& to_slot_data = to_perk_slot->slot_data_;
    /*
    const SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(GetSlot(from_slot_pos));
    const SCPerkSlot::SlotData& slot_data = perk_slot.slot_data_;
    if (slot_data.perk_info == NULL) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = slot_data.is_active_slot ? RC::RC_ITEM_INVALIDPOS
                                                         : RC::RC_ITEM_EMPTYSLOT;
        return false;
    }
    */
    //nsSlot::PerkControlMessengerInSlot perk_messenger(this);
    bool result_inter_op = perk_data_container_->RequestRegister(\
        req_info, from_slot_data.perk_data.perk_id_, to_slot_data.perk_data.perk_id_, messenger_);
    if (result_inter_op == false) {
        return false;
    }
    return true;
}

// move from an active slot so that change to inactive
bool SCPerkSlotContainer::RequestUnregister(PerkControl* req_info, POSTYPE active_slot_pos)
{
    const bool kFindOptionLinkOnly = false; // active slots
    ;{
        req_info->Clear();
        req_info->request = req_info->eReq_Unregister;
    };
    RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;
    const SCPerkSlot* from_perk_slot = \
        this->GetActivePartSlot(&result, active_slot_pos, kFindOptionLinkOnly);
    if (from_perk_slot == NULL)
    {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = static_cast<uint16_t>(result);
        return false;
    };
    const SCPerkSlot::SlotData& slot_data = from_perk_slot->slot_data_;
    //nsSlot::PerkControlMessengerInSlot perk_messenger(this);
    bool result_inter_op = perk_data_container_->RequestUnregister(\
        req_info, slot_data.perk_data.perk_id_, messenger_);
    if (result_inter_op == false) {
        return false;
    }
    return true;
}

bool SCPerkSlotContainer::ProcessReplyMessage(const PerkControl& header,
                                              const PerkControlData& input_data)
{
    PerkControlData data = input_data;
    bool success = false;
    switch (header.request)
    {
    case header.eReq_Register:
        {
            //nsSlot::PerkControlMessengerInSlot perk_messenger(this);
            success = perk_data_container_->ProcessReplyMessage(header, &data, messenger_);
            ASSERT(success);
        };
        break;
    case header.eReq_Delete:
        {
            success = perk_data_container_->ProcessReplyMessage(header, &data, messenger_);
            ASSERT(success);
        }
        break;
    case header.eReq_Unregister:
        {
            //nsSlot::PerkControlMessengerInSlot perk_messenger(this);
            success = perk_data_container_->ProcessReplyMessage(header, &data, messenger_);
            ASSERT(success);
        };
        break;
    case header.eReq_Plants:
        {
            // NOTE: delete the pointed item in an inventory
            success = perk_data_container_->ProcessReplyMessage(header, &data, messenger_);
        };
        break;
    case header.eReq_Uproot:
        {
            success = perk_data_container_->ProcessReplyMessage(header, &data, messenger_);
            // NOTE: a making item is inserted in an inventory that is extrected from a perk slot
            // WARNING : a server is not how to the item code to make an item from a perk info
        };
        break;
    }
    return success;
}

bool SCPerkSlotContainer::ProcessEventMessage(const PerkControl& header,
                                              const PerkStatusData& input_data)
{
    PerkStatusData data = input_data;
    bool success = false;
    switch (header.request)
    {
    case header.eReq_Status:
        {
            success = perk_data_container_->ProcessEventMessage(header, &data, messenger_);
        };
        break;
    };
    return success;
}

// NOTE: f101026.2L, add a factility for single attribute application on first enter field event
bool SCPerkSlotContainer::OnEnterField()
{
    return perk_data_container_->ApplyAttributes(messenger_);
}

//==================================================================================================

#endif //_NA002217_100728_EP2_PERK_SYSTEM_
