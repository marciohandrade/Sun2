#ifndef _SOLAR_SLOT_SCPERKSLOT_H
#define _SOLAR_SLOT_SCPERKSLOT_H
#pragma once

//==================================================================================================
/// SCPerkSlot class
/**
    @author waverix
    @since  2010.08.09
    @remark
    - the slot class for a perk control
    @note
    - 
    @history 
    - 10.08.09|waverix|created
*/
//==================================================================================================

#include "./SCSlot.h"
#include "./SCSlotStruct.h"

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_

class SCPerkSlotContainer;

//==================================================================================================

class SCPerkSlot : public SCSlot
{
public:
    SCPerkSlot();
    virtual ~SCPerkSlot();
    //
    // SCSlot interfaces
    //
    virtual void Clear();
    virtual void Copy(SCSlot& IN slot);
    //virtual void Copy(BitStream& IN stream);
    //virtual void CopyOut(BitStream& OUT stream);
    //virtual void Copy(BitStream& IN stream, eSLOT_SERIALIZE slot_serialize_type);
    //virtual void CopyOut(BitStream& OUT stream, eSLOT_SERIALIZE slot_serialize_type);
    //// common
    virtual eSlotType GetSlotType() const; // slot type based on object
    //virtual void SetSlotType(eSlotType slot_type);
    ////
    //virtual SLOTCODE GetCode() const; // get code id is contained in slot
    //// (CHANGES) (f100307.1) fix up unexpected polimorphous method, change CODETYPE to SLOTCODE
    //virtual void SetCode(SLOTCODE code);
    //// to support slot identity
    //virtual DBSERIAL GetSerial() const;
    //virtual void SetSerial(DBSERIAL serial);
    //// to support item slot identity in world database, use server only
    //virtual DBWLDSERIAL GetWorldSerial() const;
    ////virtual DURATYPE GetDura() const; - remove since 2006/1/16
    ////virtual void SetDura(DURATYPE duration); - remove since 2006/1/16
    //virtual DURATYPE GetNum() const;
    //virtual void SetNum(DURATYPE number);
    //// notifier for post action in slot container
    //virtual void OnInsert();
    //// notifier for post action in slot container
    //virtual void OnDelete();
    //
    //  non virtual interfaces
    //
    SCPerkSlot(const PerkData& stream);
    void Copy(const PerkData& IN stream);
    void CopyOut(PerkData& OUT stream);

    const PerkInfo* perk_info() const;
    bool is_active_slot() const;
    bool LinkSlot(const SCPerkSlot* link_slot);
    bool Unlink();
    const SCPerkSlot* GetLinkSlot() const;
    const PerkID& GetPerkID() const;
    const DWORD64& GetPerkExp() const;

private:
    struct SlotData {
        const PerkInfo* perk_info;
        PerkData perk_data;
        bool is_active_slot;
        const SCPerkSlot* link_slot;
    } slot_data_;
    //
    friend class SCPerkSlotContainer;
};

//==================================================================================================
// implements

inline SCPerkSlot::SCPerkSlot(const PerkData& stream)
{
    Copy(stream);
}

inline void SCPerkSlot::CopyOut(PerkData& OUT stream)
{
    stream = slot_data_.perk_data;
}

inline void SCPerkSlot::Clear()
{
    SCSlot::Clear();
    ZeroMemory(&slot_data_, sizeof(slot_data_));
}

inline void SCPerkSlot::Copy(SCSlot& IN slot)
{
    const SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(slot);
    slot_data_ = perk_slot.slot_data_;
    //
    SCSlot::Copy(slot);
}

inline /*virtual*/ eSlotType SCPerkSlot::GetSlotType() const
{
    return ST_STYLE_QUICK;
}

inline const PerkInfo* SCPerkSlot::perk_info() const
{
    return slot_data_.perk_info;
}

inline bool SCPerkSlot::is_active_slot() const
{
    return slot_data_.is_active_slot;
}

inline const SCPerkSlot* SCPerkSlot::GetLinkSlot() const
{
    if (slot_data_.is_active_slot) {
        return slot_data_.link_slot;
    }
    return NULL;
}

inline const PerkID& SCPerkSlot::GetPerkID() const
{
    return slot_data_.perk_data.perk_id_;
}

inline const DWORD64& SCPerkSlot::GetPerkExp() const
{
    return slot_data_.perk_data.exp_;
}

#endif //_NA002217_100728_EP2_PERK_SYSTEM_

#endif //_SOLAR_SLOT_SCQUICKSTYLESLOT_H
