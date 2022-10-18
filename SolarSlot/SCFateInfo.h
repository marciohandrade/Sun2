#ifndef _SOLAR_SLOT_SCFATEINFO_H
#define _SOLAR_SLOT_SCFATEINFO_H
#pragma once

//==================================================================================================
//  @history
//  - 090906 | waverix | change block
//
#include "./SCSlotStruct.h"
#include <FateItemInfoParser.h>

class BitStream;

//==================================================================================================
class SCFateInfo
{
    void Clear();

    //void Encode(BitStream& IN bitstream) const;
    //void Decode(BitStream& OUT bitstream);

    //void Encode(OPTIONPART_VER5* OUT dest) const;
    //void Decode(const OPTIONPART_VER5* IN src);
private:
    void SetInfo(nsSlot::SerializeItemOption::sFate* src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sFate* dest);

    BOOLEAN CalFate(SLOTCODE item_code, DWORD fate_index);
    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_Ratio         =  9;
    static const DWORD NumberOfBits_MaxDuration   =  9;
    static const DWORD NumberOfBits_Identify      =  1;
    static const DWORD NumberOfBits_Period        = 14;
    //----------------------------------------------------------------------------------------------
    WORD ratio_;            // 9: [0, 512) 공격력(방어구는 방어력) 비율
    DURATYPE max_duration_; // 9: [0, 256) Max 내구도
    BOOLEAN identify_;      // 1: 아이템을 인증했나?
    WORD period_;           //14: 아이템 기간(고정 : 인증 후 발생) 

    friend class SCItemSlot;
};

//--------------------------------------------------------------------------------------------------

inline void
SCFateInfo::Clear() {
    ZeroMemory(this, sizeof(*this));
}

//==================================================================================================

#endif //_SOLAR_SLOT_SCFATEINFO_H
