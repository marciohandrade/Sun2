#ifndef _SOLAR_SLOT_SCQUICKSTYLESLOT_H
#define _SOLAR_SLOT_SCQUICKSTYLESLOT_H
#pragma once

//==================================================================================================
/// SCQuickStyleSlot class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - 퀵 스타일을 나타내는 슬롯 클래스
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

class SCQuickStyleSlot : public SCSlot
{
public:
    SCQuickStyleSlot(void);
    virtual ~SCQuickStyleSlot(void);
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
    // SCQuickStyleSlot non virtual interfaces
    //
    SCQuickStyleSlot(const STYLESTREAM& stream);
    void Copy(const STYLESTREAM& IN stream);
    void CopyOut(STYLESTREAM& OUT stream);
    // is empty = { style_stream_.Part.Code == 0 }
    bool IsEmptySlot() const; // f100826.7L, added
    SLOTCODE GetOrgCode() const;
    void SetOrgCode(SLOTCODE code);
    //
private:
    STYLESTREAM style_stream_;
};

//==================================================================================================

// moved from impl file (cpp)

inline SCQuickStyleSlot::SCQuickStyleSlot(const STYLESTREAM& stream)
{
    Copy(stream);
}

inline void SCQuickStyleSlot::Copy(const STYLESTREAM& IN stream)
{
    style_stream_ = stream;
}

inline void SCQuickStyleSlot::CopyOut(STYLESTREAM& OUT stream)
{
    stream = style_stream_;
}

inline void SCQuickStyleSlot::Clear()
{
    SCSlot::Clear();
    ZeroMemory(&style_stream_, sizeof(style_stream_));
}

inline void SCQuickStyleSlot::Copy(SCSlot& IN slot)
{
    SCSlot::Copy(slot);
    SCQuickStyleSlot& style_slot = static_cast<SCQuickStyleSlot&>(slot);
    style_stream_ = style_slot.style_stream_;
}

inline /*virtual*/ eSlotType SCQuickStyleSlot::GetSlotType() const
{
    return ST_STYLE_QUICK;
}

inline bool SCQuickStyleSlot::IsEmptySlot() const
{   // f100826.7L, added
    return style_stream_.Part.Code == 0;
}

inline SLOTCODE SCQuickStyleSlot::GetOrgCode() const
{
    return style_stream_.Part.Code;
}

inline void SCQuickStyleSlot::SetOrgCode(SLOTCODE code)
{
    style_stream_.Part.Code = code;
}

#endif //_SOLAR_SLOT_SCQUICKSTYLESLOT_H
