#pragma once
#ifndef PROGRAMCOMMON_PERKDATA_CONTAINER_H
#define PROGRAMCOMMON_PERKDATA_CONTAINER_H

#include <StructInPacket.h>

//==================================================================================================

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_

#include "Attribute.h"

class SCPerkSlotContainer;

namespace nsSkill {
;
class PerkManager;

//==================================================================================================
// PerkSlot Container placement
// <-     14    ->|
// [00][01]...[13]|

struct PerkDataNode : public PerkData
{
    const PerkInfo* perk_info_;
    POSTYPE pos_in_container_;
    //
};

struct IPerkControlEvent
{
    virtual void OnChangeAttributes() = 0;
    virtual void OnChangeSlotContainer() = 0;
    virtual void OnLevelup() = 0;
#ifdef _NA_001990_ACSYSTEM_ADD
    virtual void OnLevelup(const PerkID& perk_id) = 0;
#endif //
};

struct IPerkControlMessenger
{
    virtual SCPerkSlotContainer* GetPerkSlotContainer() = 0;
    virtual Attributes* GetOwnerAttributes() = 0;
    virtual IPerkControlEvent* GetPerkControlEvent() = 0;
};

//==================================================================================================

class PerkDataContainer
{
    union ServerStorage;
    struct TouchList {
        enum { None = 0, Selected, Skipped, Counts };
        typedef uint8_t check_type;
        // NOTE: f110602.3L, added to write a game log
        struct ChangedInfo {
            int perk_group;
            int64_t prev_exp, next_exp;
        };
        //
        check_type touches[PerkDataList::kNumberOfActiveSlots];
    #ifdef _SERVER
        ChangedInfo changed_infos[PerkDataList::kNumberOfActiveSlots];
    #endif
    };
public:
    typedef STLX_HASH_MAP<ulong, PerkDataNode> PerkTable;
    //
  #ifdef _SERVER
    static const bool kServerLogic = true;
  #else
    static const bool kServerLogic = false;
  #endif
    static const bool kClientLogic = !kServerLogic;
    struct ActiveSlotPresenter {
        uint8_t number_of_overlaps_[PerkDataList::kNumberOfActiveSlots];
        const PerkInfo* perk_infos_[PerkDataList::kNumberOfActiveSlots];
    };
    //
    PerkDataContainer();
    virtual ~PerkDataContainer();
    void Clear();
    void set_pass_key(DWORD pass_key);
    //
    const PerkTable& GetPerkTable() const;
    const PerkDataNode* FindPerkData(const PerkID& perk_id) const;
    // D->G or G->C update stream
    void StorePerk(const PerkDataList& perk_data_list, IPerkControlMessenger* perk_messenger);
    // G->C send stream
    void LoadPerk(PerkDataList* perk_data_list) const;
    // register a cash perk slot using a cash item
    bool RequestPlants(PerkControl* req_info, const POSTYPE from_inven_item_pos,
                       IPerkControlMessenger* const perk_messenger);
    // unregister a registered cash perk so that a cash perk move to item in inventory
    bool RequestUproot(PerkControl* req_info,
                       const PerkID& uprooting_perk_id, const POSTYPE to_inven_empty_pos,
                       IPerkControlMessenger* const perk_messenger);
    // move to an active slot so that change to active
    bool RequestRegister(PerkControl* req_info,
                         const PerkID& from_perk_id, const PerkID& to_perk_id,
                         IPerkControlMessenger* const perk_messenger);
    // move from an active slot so that change to inactive
    bool RequestUnregister(PerkControl* req_info, const PerkID& perk_id,
                           IPerkControlMessenger* const perk_messenger);
    // NOTE: f100902.5L, this is a single side command by server
    // if a cash perk containing the expiration date is expired.
    // but, the result_code value of a req_info is not established
    // because a reason of deletion may have several situations.
    bool RequestDeletion(PerkControl* req_info, const PerkID& perk_id,
                         IPerkControlMessenger* const perk_messenger);
    //
    bool ProcessReplyMessage(const PerkControl& reply_header, PerkControlData* data,
                             IPerkControlMessenger* perk_messenger);
    //
    bool ProcessEventMessage(const PerkControl& update_header, PerkStatusData* data,
                             IPerkControlMessenger* perk_messenger);
    //
#if SUN_CODE_BACKUP // NOTE: f110413.2L
    const ActiveSlotPresenter& GetActiveSlotPresenter() const;
#endif
    // NOTE: f101029.1L, a check routine for a client whether a player request already a perk process
    bool IsRequestTransaction() const;
    // NOTE: f101026.2L, add a factility for single attribute application on first enter field event
    bool ApplyAttributes(IPerkControlMessenger* perk_messenger);
private:
    // the result_index have a range eActiveGroup01 with eActiveGroup10.
    // if a result is fails, the result is 0.
    // the plan rule is below.
    //  a default perk can select multiple to regist to active slot.
    //bool AllocFromFreeActiveSlot(PerkID* alloced_active_id, const PerkDataNode* perk_data_node);
    bool AllocActiveSlot(const PerkID& active_slot_id, const PerkDataNode* perk_data_node);
    bool FreeActiveSlot(const PerkID& alloced_active_id, const PerkDataNode* test_node);
    bool IsActivatedSlot(PerkID* one_of_selected_activated_slot_id,
                         const PerkDataNode* perk_data_node) const;
    // the result_index have a range 0 with kNumberOfCashSlots.
    // if a result is fails, the result is kNumberOfCashSlots.
    // the major premise is below.
    //  you should be find a same group perk before calling a this method.
    //  a cash perk using a perk item must be have the unique group code,
    // because the key of a perk table is managed by a group code.
    int AllocFromFreeCashSlot(const PerkDataNode* perk_cash_node);
    bool FreeCashSlot(const PerkDataNode* free_cash_node);
    int GetIndexInCashSlot(const PerkDataNode* perk_data_node) const;
    // NOTE: the perk system experience rule, add only battle experience value.
    // CHANGES: f101108.3L, add a function related to level control by GM commands
    bool ModifyExpOrLevel(int changed_value, bool is_level, TouchList* result_list);
    //
    // (NOTE) if the value of a 'is_add' is false, do batched process subtract attributes
    void BatchCalculateAttrActivePerks(bool is_add, IPerkControlMessenger* const perk_messenger);
    //
    bool ProcessPlants(const PerkControl& reply_header, PerkControlData* data,
                       IPerkControlMessenger* perk_messenger);
    bool ProcessUproot(const PerkControl& reply_header, PerkControlData* data,
                       IPerkControlMessenger* perk_messenger);
    bool ProcessRegister(const PerkControl& reply_header, PerkControlData* data,
                         IPerkControlMessenger* perk_messenger);
    bool ProcessUnregister(const PerkControl& reply_header, PerkControlData* data,
                           IPerkControlMessenger* perk_messenger);
    bool ProcessDelete(const PerkControl& reply_header, PerkControlData* data,
                       IPerkControlMessenger* perk_messenger);
    bool ProcessEvent(const PerkControl& reply_header, PerkStatusData* data,
                      IPerkControlMessenger* perk_messenger);
    //
    PerkTable perk_table_;
    struct InterData {
        DWORD pass_key_; // use char_guid in server, client...
        int number_of_aligned_cashes_;
        int number_of_aligned_actives_;
        const PerkDataNode* cash_slots_[PerkDataList::kNumberOfCashSlots];
        const PerkDataNode* active_slots_[PerkDataList::kNumberOfActiveSlots];
        // to support an empty slot transaction related to a plants action
        //int plants_trans_;
        // to support a multi-purpose operation like a targeting active position register operation
        PerkControl request_info_;
    #if SUN_CODE_BACKUP
        // NOTE: f110413.2L, removed the unused codes that is to support client presenting data, but
        // the client has other implementation.
        ActiveSlotPresenter active_slot_presenter_;
    #endif
    } inter_;
    //
    union ServerStorage {
        struct {
            PerkDataNode* uprooting_node;
            PerkDataNode* active_node_if_registered;
        } uproot;
        struct {
            PerkDataNode* registering_node;
            PerkDataNode* active_node;
        } regist;
        struct {
            PerkDataNode* active_node_for_unregist;
            PerkDataNode* unregistering_node;
        } unregist;
        struct {
            PerkDataNode* deleting_node;
            PerkDataNode* active_node_if_registered;
        } deletion;
    } server_data_;
    friend class PerkManager;
};

//==================================================================================================
// implements
inline void PerkDataContainer::set_pass_key(DWORD pass_key) {
    inter_.pass_key_ = pass_key;
}

inline const PerkDataContainer::PerkTable& PerkDataContainer::GetPerkTable() const {
    return perk_table_;
}

inline const PerkDataNode* PerkDataContainer::FindPerkData(const PerkID& perk_id) const
{
    PerkTable::const_iterator found = perk_table_.find(perk_id.group_);
    if (found == perk_table_.end()) {
        return NULL;
    }
    //
    const PerkDataNode* control = &found->second;
    return control;
}

#if SUN_CODE_BACKUP // NOTE: f110413.2L
inline const PerkDataContainer::ActiveSlotPresenter&
PerkDataContainer::GetActiveSlotPresenter() const
{
    return inter_.active_slot_presenter_;
}
#endif

// NOTE: f101029.1L, a check routine for a client whether a player request already a perk process
inline bool PerkDataContainer::IsRequestTransaction() const
{
    return (inter_.request_info_.request != inter_.request_info_.eReq_None);
}

//--------------------------------------------------------------------------------------------------

}; //end of namespace

#endif //_NA002217_100728_EP2_PERK_SYSTEM_
//==================================================================================================

#endif //PROGRAMCOMMON_PERKDATA_CONTAINER_H
