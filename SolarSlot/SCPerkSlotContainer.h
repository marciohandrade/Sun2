#pragma once
#ifndef _SOLAR_SLOT_SCPERKSLOTCONTAINER_H
#define _SOLAR_SLOT_SCPERKSLOTCONTAINER_H

//==================================================================================================
/// SCPerkSlotContainer class
/**
    @author waverix
    @since  2010.08.09
    @remark
    - the container class for a perk group control
    @note
    - 
    @history 
    - 10.08.09|waverix|created, this is a slot manipulator for client
*/
//==================================================================================================

#include "./SCSlotStruct.h"
#include "./SCSlotContainer.h"
#include <StructInPacket.h>

class SCPerkSlot;

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_

//==================================================================================================
class Attributes;
//
namespace nsSkill {
;
class PerkDataContainer;
struct IPerkControlEvent;
struct IPerkControlMessenger;
//
}; //end of namespace
//
namespace nsSlot {
;
struct PerkControlMessengerInSlot;
//
}; //end of namespace
//
//==================================================================================================
//
class SCPerkSlotContainer : public SCSlotContainer
{
public:
    static const int kNumberOfDefaultSlots = PerkDataList::kNumberOfDefaultSlots;
    static const int kNumberOfCashSlots = PerkDataList::kNumberOfCashSlots;
    static const int kNumberOfActiveSlots = PerkDataList::kNumberOfActiveSlots;
    static const int kMaxNumberOfNodes = PerkDataList::kMaxNumberOfNodes;
    static const int kMaxNumberOfFixedSlots = PerkDataList::kMaxNumberOfFixedSlots;
    //
    SCPerkSlotContainer();
    virtual ~SCPerkSlotContainer();

    virtual eSlotType GetSlotType() const { return ST_STYLE_QUICK; }
    virtual SCSlot* CreateSlot();
    //
    virtual BOOL ValidState();
    virtual BOOL IsLocked(POSTYPE at_pos);
    virtual void SetLock(POSTYPE at_pos, BOOL val);
    virtual BOOL IsEmpty(POSTYPE at_pos);
    //
    virtual void Release();
    virtual void ClearAll();
    //
    // (NOTE) (ref100809.1L) specification
    // { [00]=default, [11]=cash, [22]=active }
    // |[22][22][22][22]|   |[22]|
    // |-------------------------|
    // |[00][00][00][00][00]|[11]|
    // |[00][00][00][00][00]|[11]|
    // |[00][00][00][00]    |[11]|
    // |-------------------------|
    // index : reference PerkDataList::k(Default, Cash, Active)SlotPositions
    //   default = [00, 19] (20 slots)
    //   cash    = [20, 23] ( 4 slots)
    //   active  = [30, 40] ( 5 slots)
    // |[30][31][32][33]|   |[39]|
    // |-------------------------|
    // |[00][01][02][03][04]|[20]|
    // |[05][06][07][08][09]|[21]|
    // |[10][11][12][13]    |[22]|
    // |-------------------------|
    POSTYPE GetDefaultSlotPos(POSTYPE* number = NULL) const;
    POSTYPE GetCashSlotPos(POSTYPE* number = NULL) const;
    POSTYPE GetActiveSlotPos(POSTYPE* number = NULL) const;
    //
    SCPerkSlot* GetActivePartSlot(RC::eITEM_RESULT* result,
                                  POSTYPE active_slot_pos, bool with_unlink) const;
    SCPerkSlot* GetRegisterablePartSlot(RC::eITEM_RESULT* result,
                                        POSTYPE inactive_slot_pos, bool with_empty) const;
    //
    const nsSkill::PerkDataContainer* perk_data_container() const;
    void RegisterPerkControlMessenger(nsSkill::IPerkControlMessenger* messenger);
    //
    void StorePerk(const PerkDataList& perk_data_list, DWORD char_key);
    // register a cash perk slot using a cash item
    bool RequestPlants(PerkControl* req_info, const POSTYPE from_inven_item_pos);
    // reserved, ... change to a cash item?
    bool RequestUproot(PerkControl* req_info,
                       const POSTYPE from_perk_slot_pos, const POSTYPE to_inven_empty_pos);
    // move to an active slot so that change to active
    bool RequestRegister(PerkControl* req_info, POSTYPE from_slot_pos, POSTYPE to_active_slot_pos);
    // move from an active slot so that change to inactive
    bool RequestUnregister(PerkControl* req_info, POSTYPE active_slot_pos);
    //
    bool ProcessReplyMessage(const PerkControl& reply_header, const PerkControlData& data);
    bool ProcessEventMessage(const PerkControl& update_header, const PerkStatusData& data);
    // NOTE: f101026.2L, add a factility for single attribute application on first enter field event
    bool OnEnterField();
    // NOTE: f101029.1L, a check routine for a client whether a player request already a perk process
    bool SCPerkSlotContainer::IsRequestTransaction() const;
private:
    //
    void SetSlotPosByTypes(int number_of_default, int number_of_cash, int number_of_active);
    virtual void Init(POSTYPE max_slot_size, SLOTIDX slot_index);
    //prevent an interface relate to a bitstream
    virtual void Serialize(BitStream& bitstream, eSLOT_SERIALIZE serialize_type);
    virtual void Serialize(POSTYPE pos, BitStream& bitstream, eSLOT_SERIALIZE serialize_type);
    //
    nsSkill::PerkDataContainer* perk_data_container_;
    nsSkill::IPerkControlMessenger* messenger_;
    struct {
        POSTYPE slot_pos_by_types[3]; // (ref100809.1L) { 0=default, 1=cash, 2=active }
        POSTYPE number_of_slots_by_types[3];
    } inter_;
    //
    friend class nsSkill::PerkDataContainer;
    friend struct nsSlot::PerkControlMessengerInSlot;
};

//==================================================================================================
// implements

inline const nsSkill::PerkDataContainer* SCPerkSlotContainer::perk_data_container() const {
    return perk_data_container_;
}

inline void
SCPerkSlotContainer::RegisterPerkControlMessenger(nsSkill::IPerkControlMessenger* messenger)
{
    messenger_ = messenger;
}

inline POSTYPE SCPerkSlotContainer::GetDefaultSlotPos(POSTYPE* number) const
{
    if (number) {
        *number = inter_.number_of_slots_by_types[0];
    }
    return inter_.slot_pos_by_types[0];
}

inline POSTYPE SCPerkSlotContainer::GetCashSlotPos(POSTYPE* number) const
{
    if (number) {
        *number = inter_.number_of_slots_by_types[1];
    }
    return inter_.slot_pos_by_types[1];
}

inline POSTYPE SCPerkSlotContainer::GetActiveSlotPos(POSTYPE* number) const
{
    if (number) {
        *number = inter_.number_of_slots_by_types[2];
    }
    return inter_.slot_pos_by_types[2];
}

//==================================================================================================

#endif //_NA002217_100728_EP2_PERK_SYSTEM_

#endif //_SOLAR_SLOT_SCPERKSLOTCONTAINER_H
