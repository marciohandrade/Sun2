#ifndef _SOLAR_SLOT_SCSKILLSLOTCONTAINER_H
#define _SOLAR_SLOT_SCSKILLSLOTCONTAINER_H
#pragma once

//==================================================================================================
/// SCSkillSlotContainer class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @remark
    - 스킬 슬롯을 조작하는 컨테이너 클래스
    @note
    - 
    @history 
    - 09.09.06|waverix|code rearrangement
    - 10.03.10|waverix|refactoring for enhancement of the functionality of slot container
                      |enforce integrity check, to solve client crash fault problem
*/
//==================================================================================================

#include "./SCSlotContainer.h"
#include "./SCSlotStruct.h"

class SCSkillSlot;

//==================================================================================================

class SCSkillSlotContainer : public SCSlotContainer
{
public:
    SCSkillSlotContainer();
    virtual ~SCSkillSlotContainer();

    virtual eSlotType GetSlotType() const
    {
        return ST_SKILL;
    }
    virtual SCSlot* CreateSlot();

    virtual void Init(POSTYPE max_slot_size, SLOTIDX slot_index);
    virtual void ClearAll();

    void InsertSlot(POSTYPE at_pos, SKILLSTREAM& stream);
    // insert slot in container
    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN slot);
    // delete slot from container,
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete);

    SCSkillSlot* GetSkillSlotByCode(const SLOTCODE skill_code) const; // move to code file
    SCSkillSlot* GetSkillSlotByClass(const SLOTCODE skill_class_code) const;

    void SerializeSkillStream(POSTYPE pos, SKILLSTREAM* IN skill_stream, eSERIALIZE serialize_type);
    void SerializeSkillStreamAll(SKILLSTREAM* IN skill_stream, WORD stream_size,
                                 eSERIALIZE serialize_type);

    // _NA_0_20110131_SKILLTREE_SYSTEM_RENEWAL
    void Copy(SCSkillSlotContainer* INOUT target) const;

protected:
    typedef STLX_HASH_MAP<SLOTCODE, SCSkillSlot*> SkillSlots;
    SkillSlots skill_slots_;
};

#endif //_SOLAR_SLOT_SCSKILLSLOTCONTAINER_H