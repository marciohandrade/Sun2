#ifndef _SOLAR_SLOT_SCITEMSLOTCONTAINER_H
#define _SOLAR_SLOT_SCITEMSLOTCONTAINER_H
#pragma once

//==================================================================================================
/// SCItemSlot 컨테이너 클래스 : SCItemSlotContainer class
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2004. 12. 29
    @history 
    - 구조 수정 :   2005.7.30
        Item과 SKill을 Slot을 통해 공유해야 한다는 개념이 무색해 졌다. (공유가 필요 없음을 느꼈음)
    - 09.09.06|waverix|code rearrangement
    - 10.03.10|waverix|refactoring for enhancement of the functionality of slot container
                      |enforce integrity check, to solve client crash fault problem
    @note
    - Init()초기화, Release()릴리즈를 반드시 하라!
    - 컨테이너 슬롯의 개수는 고정적이다!
    - InsertSlot() 함수의 리턴값이 있는 이유, derived class에서 삽입할때 제한체크를 위해
    - 2006.10.27 : 최대 탭 17개 까지 추가 가능(POSTYPE->BYTE이기 때문)
*/
//==================================================================================================

#include "./SCSlotContainer.h"

struct BASE_ITEMINFO;
class SCItemSlot;
//==================================================================================================

class SCItemSlotContainer : public SCSlotContainer
{
public:
    SCItemSlotContainer(void);
    virtual ~SCItemSlotContainer(void);

    virtual eSlotType GetSlotType() const;
    virtual SCSlot* CreateSlot();

    virtual void ClearAll();
    virtual void Init(POSTYPE max_slot_size, SLOTIDX slot_index);

    virtual void UpdateSlot(POSTYPE at_pos, int changed_num);

    template< class STREAM_TYPE >
    void InsertSlotUsingStream(POSTYPE at_pos, STREAM_TYPE& stream);

    // (WARNING) The 'IsLocked' method is different from what 'SetLock' method has lock. \
    // 'IsLocked' is returns whether any multi-locks are locked.
    // (CHANGES) (f100311.4) add check the 'UserBlocked' related with summon
    virtual BOOL IsLocked(POSTYPE at_pos);
    // set up 'locked_' in item slot
    virtual void SetLock(POSTYPE at_pos, BOOL is_setup);

    // 아이템만의 SERIALIZE
    template< class STREAM_TYPE >
    void SerializeItemStream(POSTYPE pos, STREAM_TYPE* IN stream, eSERIALIZE serialize_type);
    template< class STREAM_TYPE >
    void SerializeItemStreamAll(STREAM_TYPE* INOUT stream, WORD size, eSERIALIZE serialize_type);

    /*
    virtual void Serialize(BitStream& bitstream, eSLOT_SERIALIZE eType);
    virtual void Serialize(POSTYPE pos, BitStream& bitstream, eSLOT_SERIALIZE eType);
    */

    virtual BOOL ValidState();
    virtual void SetValidState(BOOL settled);
    // implemented by _SOLAR_SLOT_INTEGRITY_CONTAINER_PROCESS_
    struct SameSetItemSlotsResult {
        WORD set_item_code;
        POSTYPE number_of_same_set_items;
    };
    SameSetItemSlotsResult GetSameSetItemSlots(SCItemSlot** result_of_set_items,
                                               const POSTYPE number_of_result_array,
                                               const BASE_ITEMINFO* related_reference_item,
                                               BOOL bEquip = TRUE);

	SameSetItemSlotsResult GetSameSetItemTotalSlots(SCItemSlot** result_of_set_items,
												const POSTYPE number_of_result_array,
												const BASE_ITEMINFO* related_reference_item,
												BOOL bEquip = TRUE);

    //_NA_0_20100517_ITEMSLOTCONTAINER_EQUIPGROUP_DETECTION
    // 주어진 참조 장착 위치와 같은 유형의 위치에 특정 아이템을 장비하고 있는지 검사
    // SolarSlot 기존 호출 관례에 따라 해당 아이템을 장비하고 있지 않다면 널 객체 참조 리턴
    const SCItemSlot& GetItemOfType(POSTYPE reference_pos, SLOTCODE item_code);

    bool ValidStateByConst() const
    {
        return valid_state_;
    }
private:
    bool valid_state_; // (f100310.1L) changes type
};

//==================================================================================================

inline /*virtual*/ eSlotType SCItemSlotContainer::GetSlotType() const
{
    return ST_ITEM;
}

inline /*virtual*/ BOOL SCItemSlotContainer::ValidState()
{
    return valid_state_;
}

inline /*virtual*/ void SCItemSlotContainer::SetValidState(BOOL settled)
{
    valid_state_ = !!settled;
}

//==================================================================================================

// template implement
#define __SOLAR_SLOT_SCITEMSLOT_CONTAINER_HPP__
#include "SCItemSlotContainer.hpp"
#undef __SOLAR_SLOT_SCITEMSLOT_CONTAINER_HPP__

#endif //_SOLAR_SLOT_SCITEMSLOTCONTAINER_H
