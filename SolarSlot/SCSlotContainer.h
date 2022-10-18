#ifndef _SOLAR_SLOT_SCSLOTCONTAINER_H
#define _SOLAR_SLOT_SCSLOTCONTAINER_H
#pragma once

//==================================================================================================
/// SCSlot 컨테이너 클래스 : SCSlotContainer class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @note
    - Init()초기화, Release()릴리즈를 반드시 하라!
    - 컨테이너 슬롯의 개수는 고정적이다!
    @history
    - 2005.8.16 : SetLock()함수 추가 : 빈공간도 락을 하기 위해!
    - 09.09.06 | waverix | code rearrangement
    - 10.03.07|waverix|refactoring for enhancement of the functionality of slot container
                      |enforce integrity check, to solve client crash fault problem
                      |(f100307.6) changes to const method = \
                        { GetSlotNum, ValidPos, GetMaxSlotNum, GetEmptySlotNum, GetSlotIdx }
    -                 |(f100307.8) refactoring, to support safe pointer process \
                        on illegal life-time management
*/
//==================================================================================================

#include "ResultCode.h"
#include "./SCSlotHeader.h"
#include "./SCSlot.h"

//==================================================================================================
#define _SOLAR_SLOT_INTEGRITY_CONTAINER_PROCESS_
//|10/03/07|이은파|SlotContainer관련 정리, ref:@history in SCSlotContainer.h

class SCSlotContainer
{
public:
    struct SlotNode { POSTYPE pos; SCSlot* slot; };
    typedef STLX_VECTOR<SlotNode> SLOT_LINEAR_LIST; // exposed to access reference

    SCSlotContainer();
    virtual ~SCSlotContainer();
    // current container object integrity check utility
    // (f100308.1) to solve invalid slot container problem
    bool IntegrityObjectCheck() const;

    virtual void Init(POSTYPE max_slot_size, SLOTIDX slot_index);
    // (CHANGES) fix up critical problem that 'Release' polymorphism have too much,
    // (f100307.8) moved Release logic to ReleaseSlots:private, and notify all members
    virtual void Release();
    virtual void ClearAll();

    virtual eSlotType GetSlotType() const;
    virtual SCSlot* CreateSlot() = 0;

    virtual void OnInsert(SCSlot& IN slot);
    virtual void OnDelete(SCSlot& IN slot);
    virtual void OnUpdate(SCSlot& IN slot, int changed_num);

    //bitstream
    virtual void Serialize(BitStream& bitstream, eSLOT_SERIALIZE serialize_type);
    virtual void Serialize(POSTYPE pos, BitStream& bitstream, eSLOT_SERIALIZE serialize_type);

    virtual void UpdateSlot(POSTYPE at_pos, SCSlot& IN slot);
    virtual void UpdateSlot(POSTYPE at_pos, int changed_num);
    // insert slot in container
    virtual RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN slot);
    // delete slot from container,
    // output = slot_clone_before_delete : SCSlot*
    //  - copied slot containing original information
    virtual BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_clone_before_delete);
    // 왜 virtual 일까?
    virtual BOOL GetEmptyPos(POSTYPE& empty_pos_result);
    //
    // (NOTE) to acquire valid slot,
    //          composition set is (ValidPos(pos) && IsEmpty(pos) == false)
    // (NOTE) to acquire valid EMPTY slot,
    //          composition set is (ValidPos(pos) && IsEmpty(pos))
    bool ValidPos(POSTYPE at_pos) const; // (f100307.6) changes return type
    virtual BOOL IsEmpty(POSTYPE at_pos);
    // this function has many problem...
    SCSlot& GetSlot(POSTYPE at_pos);
    //
    POSTYPE GetSlotNum() const;
    // (CHANGES) (f100307.7) remove virtual, don't use polyporphism anywhere
    POSTYPE GetMaxSlotNum() const;
    void SetMaxSlotNum(POSTYPE max_slot_num);
    POSTYPE GetEmptySlotNum() const;
    // operate slot size extension
    void Resize(POSTYPE max_slot_size);
    //
    virtual BOOL ValidState();
    //
    virtual BOOL IsLocked(POSTYPE at_pos);
    virtual void SetLock(POSTYPE at_pos, BOOL val);
    //
    SLOTIDX GetSlotIdx() const;
    void SetSlotIdx(SLOTIDX slot_index);
    // (f100326.1) exposed to access reference, this will eventually replace ForEachSlot
    // (f100311.4) add getter for const slot list to support derived object to use for_each facilities
    const SLOT_LINEAR_LIST& GetSlotList() const;
    // for_each utility
    template<class SlotOperator>
    void ForEachSlot(SlotOperator& slot_operator);

    //typedef STLX_VECTOR<SCSlot*> SLOT_LINEAR_LIST;
    // (f100308.2) add 'GetSlot' concept method with integrity check.
    // if the function succeeds, the return value is nonzero.
    // if the function fails, occurred index of out range
    SCSlot* GetValidSlotPtr(RC::eSLOT_INSERT_RESULT* result, POSTYPE at_pos) const;
protected:
    // (f100326.1) exposed to access reference, this will eventually replace ForEachSlot
    // (f100311.4) add getter for const slot list to support derived object to use for_each facilities
    //const SLOT_LINEAR_LIST& GetSlotList() const;
private:
    // (f100307.8) add 'ReleaseSlots' to solve can't call state by polymorphism problem
    void ReleaseSlots();
    //
    //---- data fields -----------------------------------------------------------------------------
    const SLOTIDX& integrity_reference_; // (f100308.1)
    // (f100308.3) to support compatible 'GetSlot' reference.
    SCSlot* invalid_slot_;
    SLOT_LINEAR_LIST slot_list_;
    // align 1 serialize order
    SLOTIDX slot_index_;
    POSTYPE max_slot_num_; // 기획 내용에 따라 사용 가능한 최대 슬롯 개수
protected:
    POSTYPE m_nSlotNum; // can't change to private, because exist used case in derived object
    //POSTYPE m_nResizeSlotNum; // 초기에 생성한 슬롯 개수 <- (NOTE) unused ?
    __DISABLE_COPY(SCSlotContainer);
};

//==================================================================================================

// current container object integrity check utility
// (f100308.1) to solve invalid slot container problem
//      designed by _SOLAR_SLOT_INTEGRITY_CONTAINER_PROCESS_
inline bool SCSlotContainer::IntegrityObjectCheck() const
{
    return &integrity_reference_ == &slot_index_;
}

inline /*virtual*/ eSlotType SCSlotContainer::GetSlotType() const
{
    return ST_NONE;
}

//virtual SCSlot* SCSlotContainer::CreateSlot() = 0;

inline /*virtual*/ void SCSlotContainer::OnInsert(SCSlot& IN slot)
{
    __UNUSED(&slot);
}

inline /*virtual*/ void SCSlotContainer::OnDelete(SCSlot& IN slot)
{
    __UNUSED(&slot);
}

inline /*virtual*/ void SCSlotContainer::OnUpdate(SCSlot& IN slot, int changed_num)
{
    __UNUSED((&slot, changed_num));
}

inline POSTYPE SCSlotContainer::GetSlotNum() const
{
    return m_nSlotNum;
}

inline bool SCSlotContainer::ValidPos(POSTYPE at_pos) const
{
    return at_pos < max_slot_num_;
}

// (WARNING) (f100307.7) remove virtual, don't use polyporphism anywhere, make to const
inline POSTYPE SCSlotContainer::GetMaxSlotNum() const
{
    return max_slot_num_;
}

// (CHANGES) (f100329.5L) change concept. change real list size on call SetMaxSlotNum
//inline void SCSlotContainer::SetMaxSlotNum(POSTYPE max_slot_num) 
//{ 
//    max_slot_num_ = max_slot_num; 
//}

inline POSTYPE SCSlotContainer::GetEmptySlotNum() const
{
    // (CHANGES) add boundary checker
    if (FlowControl::FCAssert(max_slot_num_ >= m_nSlotNum)) {
        return max_slot_num_ - m_nSlotNum;
    }
    return 0;
}

inline /*virtual*/ BOOL SCSlotContainer::ValidState()
{
    // changes always true state to based on object integrity, since 100308
    // but... guess be occurred fault on calling point
    return IntegrityObjectCheck();
}

inline /*virtual*/ BOOL SCSlotContainer::IsLocked(POSTYPE at_pos)
{
    __UNUSED(at_pos);
    return false;
}

inline /*virtual*/ void SCSlotContainer::SetLock(POSTYPE at_pos, BOOL val)
{
    __UNUSED((at_pos, val));
}

inline SLOTIDX SCSlotContainer::GetSlotIdx() const
{
    return slot_index_;
}

inline void SCSlotContainer::SetSlotIdx(SLOTIDX slot_index)
{
    slot_index_ = slot_index;
}

// (f100311.4) add getter for const slot list to support derived object to use for_each facilities
inline const SCSlotContainer::SLOT_LINEAR_LIST&
SCSlotContainer::GetSlotList() const
{
    return slot_list_;
}

// for_each utility
template<class SlotOperator>
void
SCSlotContainer::ForEachSlot(SlotOperator& slot_operator)
{
    if (IntegrityObjectCheck() == false) {
        return;
    }
    if (slot_list_.empty()) {
        return;
    }
    SLOT_LINEAR_LIST::iterator it = slot_list_.begin(),
                               end = slot_list_.end();
    for (POSTYPE pos = 0; it != end; ++it, ++pos)
    {
        if (IsEmpty(pos)) {
            continue;
        }
        // slot_in_container
        const SlotNode& slot_node = *it;
        slot_operator(slot_node.slot);
    }
}

#endif //_SOLAR_SLOT_SCSLOTCONTAINER_H
