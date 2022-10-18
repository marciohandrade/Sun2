#ifndef _SOLAR_SLOT_SCSLOT_H
#define _SOLAR_SLOT_SCSLOT_H
#pragma once

//==================================================================================================
/// SCSlot class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - 모든 슬롯의 최상위 추상 클래스
    @note
    - 
    @history 
    - 2006/1/13 : 슬롯 개수 맴버 m_nNum SetNum(), GetNum() 추가 - 내구도와 별개처리위해
    - 2006/1/16 : GetDura(), SetDura() 를 없앰-> SCItemSlot에서 처리위해
    - 09.09.06|waverix|code rearrangement
    - 10.03.07|waverix|code rearrangement to support item sorting has indirect layer
*/
//==================================================================================================

#include "SCSlotHeader.h"

class BitStream;

//==================================================================================================
//
class SCSlot  
{
public:
    SCSlot();
    virtual ~SCSlot();

    virtual void Clear();
    virtual void Copy(SCSlot& IN slot);
    virtual void Copy(BitStream& IN stream);
    virtual void CopyOut(BitStream& OUT stream);
    virtual void Copy(BitStream& IN stream, eSLOT_SERIALIZE eType) { __UNUSED((&stream, eType)); }
    virtual void CopyOut(BitStream& OUT stream, eSLOT_SERIALIZE eType) { __UNUSED((&stream, eType)); }
    // common
    virtual eSlotType GetSlotType() const; // slot type based on object
    virtual void SetSlotType(eSlotType slot_type);
    //
    virtual SLOTCODE GetCode() const; // get code id is contained in slot
    // (CHANGES) (f100307.1) fix up unexpected polimorphous method, change CODETYPE to SLOTCODE
    virtual void SetCode(SLOTCODE code);
    // to support slot identity
    virtual DBSERIAL GetSerial() const;
    virtual void SetSerial(DBSERIAL serial);
    // to support item slot identity in world database, use server only
    virtual DBWLDSERIAL GetWorldSerial() const;
    //virtual DURATYPE GetDura() const; - remove since 2006/1/16
    //virtual void SetDura(DURATYPE duration); - remove since 2006/1/16
    virtual DURATYPE GetNum() const;
    virtual void SetNum(DURATYPE number);
    // notifier for post action in slot container
    virtual void OnInsert();
    // notifier for post action in slot container
    virtual void OnDelete();
    //-----------------------------------------
    // position in slot conainer by type
    POSTYPE GetPos() const;
    void SetPos(POSTYPE position);
    // slot container index by type
    SLOTIDX GetSlotIdx() const;
    void SetSlotIdx(SLOTIDX slot_index);
    //-----------------------------------------
private:
    SLOTIDX slot_index_;
    POSTYPE position_;
    DURATYPE number_;
};
//
//==================================================================================================
//
// (CHANGES) fix up invalid clear value that constructor is not matched 'clear' method
// (WARNING) check old logic irregulation
inline SCSlot::SCSlot()
    : position_(INVALID_POSTYPE_VALUE),
      number_(1),
      slot_index_(SI_MAX)
{
}

inline /*virtual*/ SCSlot::~SCSlot()
{
}

inline /*virtual*/ void SCSlot::Clear()
{
    slot_index_ = SI_MAX;
    position_ = INVALID_POSTYPE_VALUE;
    number_ = 1;
}

inline /*virtual*/ void SCSlot::Copy(SCSlot& IN slot)
{
    slot_index_ = slot.slot_index_;
    position_ = slot.position_;
    number_ = slot.number_;
}

inline /*virtual*/ void SCSlot::Copy(BitStream& IN stream)
{
    __UNUSED(&stream);
}

inline /*virtual*/ void SCSlot::CopyOut(BitStream& OUT stream)
{
    __UNUSED(&stream);
}

// link problem : not detach from header to solve COMDAT reference fail
//inline /*virtual*/ void Copy(BitStream& IN stream, eSLOT_SERIALIZE serialize_type)
//{
//    __UNUSED((&stream, serialize_type));
//}
//
//inline /*virtual*/ void CopyOut(BitStream& OUT stream, eSLOT_SERIALIZE serialize_type)
//{
//    __UNUSED((&stream, serialize_type));
//}

// common
inline /*virtual*/ eSlotType SCSlot::GetSlotType() const
{
    return ST_NONE;
}

inline /*virtual*/ SLOTCODE SCSlot::GetCode() const
{
    return 0;
}

inline /*virtual*/ void SCSlot::SetSlotType(eSlotType slot_type)
{
    __UNUSED(slot_type);
}

// (CHANGES) (f100307.1) fix up unexpected polimorphous method, change CODETYPE to SLOTCODE
inline /*virtual*/ void SCSlot::SetCode(SLOTCODE code)
{
    __UNUSED(code);
}

// to support slot identity
inline /*virtual*/ DBSERIAL SCSlot::GetSerial() const
{
    return 0;
}

// to support slot identity
inline /*virtual*/ void SCSlot::SetSerial(DBSERIAL serial)
{
    __UNUSED(serial);
}

// to support item slot identity in world database, use server only
inline /*virtual*/ DBWLDSERIAL SCSlot::GetWorldSerial() const
{
    return 0;
}

//inline /*virtual*/ DURATYPE SCSlot::GetDura() const { return 0; }
//inline /*virtual*/ void SCSlot::SetDura(DURATYPE duration) { __UNUSED(dura); }

inline /*virtual*/ DURATYPE SCSlot::GetNum() const
{
    return number_;
}

inline /*virtual*/ void SCSlot::SetNum(DURATYPE number)
{
    number_ = number;
}

// position in slot conainer by type
inline POSTYPE SCSlot::GetPos() const
{
    return position_;
}

// position in slot conainer by type
inline void SCSlot::SetPos(POSTYPE pos)
{
    position_ = pos;
}

// slot container index by type
inline SLOTIDX SCSlot::GetSlotIdx() const
{
    return slot_index_;
}

// slot container index by type
inline void SCSlot::SetSlotIdx(SLOTIDX slot_index)
{
    slot_index_ = slot_index;
}

// notifier for post action in slot container
inline /*virtual*/ void SCSlot::OnInsert()
{
}

// notifier for post action in slot container
inline /*virtual*/ void SCSlot::OnDelete()
{
}

#endif //_SOLAR_SLOT_SCSLOT_H
