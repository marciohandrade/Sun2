#ifndef _SOLAR_SLOT_SCPETINFO_H
#define _SOLAR_SLOT_SCPETINFO_H
#pragma once

//==================================================================================================
/// 아이템 Pet 정보 클래스
/**
    @author Kim Chang Hyun < shogen@webzen.co.kr >
    @since  2008. 2. 18
    @remark
    - Socket Option 부분과 같이 사용함
    @history 
    - 09.09.06 | waverix | code rearrangement
*/
//==================================================================================================

#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"

class BitStream;

//==================================================================================================
//
struct SCPetInfo
{
    void Clear();

    //void Encode(BitStream& IN bitstream) const;
    //void Decode(const WORD item_type, BitStream& OUT bitstream);

    //void Encode(OPTIONPART_VER5* OUT dest) const;
    //void Decode(const WORD item_type, const OPTIONPART_VER5* IN src);
    //-----------------------------------------------
    // old version corresponding interfaces
    void SetFullness(int ivalue) const;
    void SetIntimacy(int ivalue) const;

    BYTE GetFullness() const { return fullness_; }
    BYTE GetIntimacy() const { return intimacy_; }

    //-----------------------------------------------
    //__NA_1131_PET_NAME_SETTING       // 기존 10으로 설정되어 있으므로...
    void SetPetName(const char pet_name[MAX_PET_NAME_LENGTH]) const;
    const char* GetPetName() const;
    BOOLEAN IsSetPetName() const;   //__NA_1131_PET_NAME_SETTING

private:
    void SetInfo(nsSlot::SerializeItemOption::sPet* src, const BASE_ITEMINFO* item_info);
    void GetInfo(nsSlot::SerializeItemOption::sPet* dest);

    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_Fullness = 8;
    static const DWORD NumberOfBits_Intimacy = 8;
    static const DWORD NumberOfBits_SetPetName = 1;
    static const DWORD NumberOfLength_PetName = MAX_PET_NAME_LENGTH; // for serialize
    //----------------------------------------------------------------------------------------------
    BYTE fullness_; // 8: 포만감
    BYTE intimacy_; // 8: 친밀도
    // __NA_1131_PET_NAME_SETTING
    char pet_name_[MAX_PET_NAME_LENGTH + 1];

    friend class SCItemSlot;
};

//--------------------------------------------------------------------------------------------------

inline void
SCPetInfo::Clear() {
    ZeroMemory(this, sizeof(*this));
}

inline BOOLEAN
SCPetInfo::IsSetPetName() const {
    return pet_name_[0] != 0;
}

inline void
SCPetInfo::SetFullness(int ivalue) const
{
    // n < 0, 100 < n   - bound check
    if(ivalue <  0)
        ivalue = 0;
    else if(   100 < ivalue)
        ivalue = 100;

    const_cast<BYTE&>(fullness_) = BYTE(ivalue);
}

inline void
SCPetInfo::SetIntimacy(int ivalue) const
{
    // n < 0, 100 < n   - bound check
    if(ivalue <  0)
        ivalue = 0;
    else if(   100 < ivalue)
        ivalue = 100;

    const_cast<BYTE&>(intimacy_) = BYTE(ivalue);
}

inline const char*
SCPetInfo::GetPetName() const {
    return pet_name_;
}
//
//==================================================================================================

#endif //_SOLAR_SLOT_SCPETINFO_H
