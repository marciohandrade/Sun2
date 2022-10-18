#ifndef _SOLAR_SLOT_SCQUICKSLOT_H
#define _SOLAR_SLOT_SCQUICKSLOT_H
#pragma once

//==================================================================================================
/// SCQuickSlot
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - ÄüÃ¢¿¡ µé¾î°¡´Â Äü ½½·Ô Å¬·¡½º
    @note
    - 
    @history 
    - 09.09.06|waverix|code rearrangement
    - 10.03.07|waverix|code rearrangement to support item sorting has indirect layer
*/
//==================================================================================================

#include "./SCSlot.h"
#include "./SCSlotStruct.h"

//==================================================================================================

class SCQuickSlot : public SCSlot
{
public:
    SCQuickSlot();
    virtual ~SCQuickSlot();
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
    SCQuickSlot(const QUICKSTREAM& stream);
    void Copy(const QUICKSTREAM& IN stream);
    void CopyOut(QUICKSTREAM& OUT stream);
    // is empty = { quick_stream_.Part.SlotIdx == 0 } ... not code, slot index
    bool IsEmptySlot() const; // f100826.7L, added
    // (CHANGES) (f100307.5) change to const method
    POSTYPE GetOrgPos() const;
    SLOTCODE GetOrgCode() const;
    SLOTIDX GetOrgSlotIndex() const;
    //
    void SetOrgPos(POSTYPE pos);
    void SetOrgCode(SLOTCODE code);
    void SetOrgSlotIndex(SLOTIDX slot_idx);
private:
    QUICKSTREAM quick_stream_;
};

//==================================================================================================

inline SCQuickSlot::SCQuickSlot(void)
{   // prevent a garbage data from generating after it call a constructor
    ZeroMemory(&quick_stream_, sizeof(quick_stream_));
}

inline SCQuickSlot::~SCQuickSlot(void)
{
}

inline SCQuickSlot::SCQuickSlot(const QUICKSTREAM& quick_stream)
{
    Copy(quick_stream);
}

inline void SCQuickSlot::Copy(const QUICKSTREAM& IN quick_stream)
{
    quick_stream_ = quick_stream;
}

inline void SCQuickSlot::CopyOut(QUICKSTREAM& OUT quick_stream)
{
    quick_stream = quick_stream_;
}

inline /*virtual*/ void SCQuickSlot::Clear()
{
    SCSlot::Clear();
    ZeroMemory(&quick_stream_, sizeof(quick_stream_));
}

inline /*virtual*/ void SCQuickSlot::Copy(SCSlot& IN slot)
{
    SCSlot::Copy(slot);
    SCQuickSlot& quick_slot = static_cast<SCQuickSlot&>(slot);
    quick_stream_ = quick_slot.quick_stream_;
}

inline /*virtual*/ eSlotType SCQuickSlot::GetSlotType() const {
    return ST_QUICK;
}

// f100826.7L, added
// is empty = { quick_stream_.Part.SlotIdx == 0 } ... not code, slot index
inline bool SCQuickSlot::IsEmptySlot() const
{
    return quick_stream_.Part.SlotIdx == 0;
};

inline POSTYPE SCQuickSlot::GetOrgPos() const
{
    return quick_stream_.Part.Pos;
}

inline SLOTCODE SCQuickSlot::GetOrgCode() const
{
    return quick_stream_.Part.Code;
}

inline SLOTIDX SCQuickSlot::GetOrgSlotIndex() const
{
    return quick_stream_.Part.SlotIdx;
}

inline void SCQuickSlot::SetOrgPos(POSTYPE pos)
{
    quick_stream_.Part.Pos = pos;
}

inline void SCQuickSlot::SetOrgCode(SLOTCODE code)
{
    quick_stream_.Part.Code = code;
}

inline void SCQuickSlot::SetOrgSlotIndex(SLOTIDX slot_index)
{
    quick_stream_.Part.SlotIdx = slot_index;
}

#endif //_SOLAR_SLOT_SCQUICKSLOT_H
