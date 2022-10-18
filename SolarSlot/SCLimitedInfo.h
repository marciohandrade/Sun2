#ifndef _SOLAR_SLOT_SCLIMITEDINFO_H
#define _SOLAR_SLOT_SCLIMITEDINFO_H
#pragma once

//==================================================================================================
/// 리미티드 아이템 정보 클래스
/**
    @author Kim Chang Hyun < shogen@webzen.co.kr >
    @since  2007. 7. 25
    @remark
    @history
    - 09.09.06 | waverix | code rearrangement
*/
//==================================================================================================

#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"

#include <LimitedItemInfoParser.h>
#include <LimitedOptionParser.h>

class BitStream;
struct sLIMITED_OPTION;

//==================================================================================================
//
class SCLimitedInfo
{
    // 실제 Slot에서 사용하는 정보
    // 현재 아이템이 가진 옵션 정보의 누적값
    struct AttrValues {
        int attr_index_;        // 7: NumberOfBits_AttrIndex
        eATTR_TYPE attr_type_;  // 6: AttrValueRange
        int attr_value_;
        const sLIMITED_OPTION* limited_option_;
    };

    void Clear();

private:
    /*
    // (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
    int GetOptAttrIndex(sLIMITED_NUM num) const; 
    int GetOptAttrValue(sLIMITED_NUM num) const;
    */

    //void Encode(BitStream& IN bitstream) const;
    //void Decode(WORD item_type, BitStream& OUT bitstream);

    //void Encode(OPTIONPART_VER5* OUT dest) const;
    //void Decode(WORD item_type, const OPTIONPART_VER5* IN src);

    void SetInfo(nsSlot::SerializeItemOption::sLimited* src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sLimited* dest) /*const*/;

    void CalLimited(WORD ItemCode);

    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_LimitedIndex          = 16;
    static const DWORD NumberOfBits_MaxDurationForRestore =  9;
    static const DWORD NumberOfBits_MaxDuration           =  9;
    static const DWORD NumberOfBits_NumberOfRepairs       =  4;
    static const DWORD NumberOfBits_DefenceRatio           =  9;
    static const DWORD NumberOfBits_LimitValueRatio       =  9;

    static const DWORD NumberOfBits_AttrIndex = 7; // in AttrValues
    static const DWORD NumberOfBits_AttrValue = 6; // in AttrValues

    //----------------------------------------------------------------------------------------------
    WORD limited_index_;                //16: NumberOfBits_LimitedIndex
    DURATYPE max_duration_for_restore_; // 9: 최대 내구도(최대 내구도 복구용)   NumberOfBits_MaxDurationForRestore
    DURATYPE max_duration_;             // 9: 최대 내구도(수리시 깍임)          NumberOfBits_MaxDuration
    BYTE number_of_repairs_;            // 4: 수리 횟수                         NumberOfBits_NumberOfRepairs
    int defence_ratio_;                  // 9: 공격력 비율(방어구는 방어력)      NumberOfBits_DamageRatio
    int limit_value_ratio_;             // 9: 제한 수치 비율                    NumberOfBits_LimitValueRatio
    // (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
    //AttrValues limited_attributes_[LIMITED_MAX];

    friend class SCItemSlot;
};

//--------------------------------------------------------------------------------------------------

inline void
SCLimitedInfo::Clear() {
    ZeroMemory(this, sizeof(*this));
}

/*
// (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
inline int
SCLimitedInfo::GetOptAttrIndex(sLIMITED_NUM num) const
{
    if(FlowControl::FCAssert(num < LIMITED_MAX))
        return limited_attributes_[num].attr_index_;
    return 0;
}

inline int
SCLimitedInfo::GetOptAttrValue(sLIMITED_NUM num) const
{
    if(FlowControl::FCAssert(num < LIMITED_MAX))
        return limited_attributes_[num].attr_value_;
    return 0;
}
*/

//
//==================================================================================================


#endif //_SOLAR_SLOT_SCLIMITEDINFO_H
