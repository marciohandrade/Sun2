#ifndef _SOLAR_SLOT_SCSKILLSLOT_H
#define _SOLAR_SLOT_SCSKILLSLOT_H
#pragma once

//==================================================================================================
/// SCSkillSlot class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - 스킬 슬롯을 나타내는 클래스
    @note
    - 
    @history 
    - 09.09.06|waverix|code rearrangement
    - 10.03.07|waverix|code rearrangement to support item sorting has indirect layer
                       (f100307.3, 4) enforce validation check
*/
//==================================================================================================

#include <Struct.h>
#include "./SCSlot.h"
#include "./SCSlotStruct.h"

class SkillScriptInfo;

//==================================================================================================
//
class SCSkillSlot : public SCSlot
{
public:
    SCSkillSlot(void);
    virtual ~SCSkillSlot(void);
    //
    // SCSlot interfaces
    //
    virtual void Clear();
    virtual void Copy(SCSlot& IN slot);
    //virtual void Copy(BitStream& IN stream);
    //virtual void CopyOut(BitStream& OUT stream);
    //virtual void Copy(BitStream& IN stream, eSLOT_SERIALIZE slot_serialize_type);
    //virtual void CopyOut(BitStream& OUT stream, eSLOT_SERIALIZE slot_serialize_type);
    // common
    virtual eSlotType GetSlotType() const; // slot type based on object
    virtual void SetSlotType(eSlotType slot_type);
    //
    virtual SLOTCODE GetCode() const; // get code id is contained in slot
    virtual void SetCode(SLOTCODE code);
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
    // SCSkillSlot non virtual interfaces
    //
    SCSkillSlot(const SKILLSTREAM& skill_stream);
    void Copy(const SKILLSTREAM& IN skill_stream);
    void CopyOut(SKILLSTREAM& OUT skill_stream);
    // (CHANGES) change to const and bool that all version supported
    bool IsStyle() const;
    bool IsSKill() const;
    const BASE_STYLEINFO* GetStyleInfo() const;
    const SkillScriptInfo* GetSkillInfo() const;
    // (CHANGES) (f100307.4) add validation check tool
    bool IsContainValidInfo() const;
private:
    void _SetSkillInfo();

    static const SkillScriptInfo& Null_SkillInfo;
    SKILLSTREAM skill_stream_;
    const ROOT_SKILLINFO* root_skill_info_;
};

//==================================================================================================

inline SCSkillSlot::SCSkillSlot(const SKILLSTREAM& skill_stream)
{
    skill_stream_ = skill_stream;
    _SetSkillInfo();
}

inline /*virtual*/ void SCSkillSlot::Copy(SCSlot& IN slot)
{
    SCSlot::Copy(slot);
    SCSkillSlot& skill_slot = static_cast<SCSkillSlot&>(slot);
    skill_stream_ = skill_slot.skill_stream_;
    root_skill_info_ = skill_slot.root_skill_info_;
}

inline void SCSkillSlot::Copy(const SKILLSTREAM& skill_stream)
{
    skill_stream_ = skill_stream;
    _SetSkillInfo();
}

inline void SCSkillSlot::CopyOut(SKILLSTREAM& OUT skill_stream)
{
    skill_stream = skill_stream_;
}

inline /*virtual*/ void SCSkillSlot::Clear()
{
    SCSlot::Clear();
    ZeroMemory(&skill_stream_, sizeof(skill_stream_));
    root_skill_info_ = NULL;
}

inline /*virtual*/ eSlotType SCSkillSlot::GetSlotType() const
{
    return ST_SKILL;
}

inline /*virtual*/ SLOTCODE SCSkillSlot::GetCode() const
{
    return skill_stream_.Part.wCode;
}

inline /*virtual*/ void SCSkillSlot::SetSlotType(eSlotType slot_type)
{
    __UNUSED(slot_type);
}

// (CHANGES) (f100307.4) add validation check tool
inline bool SCSkillSlot::IsContainValidInfo() const
{
    return root_skill_info_ != NULL && reinterpret_cast<const char*>(root_skill_info_) !=
                                       reinterpret_cast<const char*>(&Null_SkillInfo);
}

#endif //_SOLAR_SLOT_SCSKILLSLOT_H
