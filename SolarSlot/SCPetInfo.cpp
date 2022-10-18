#include "Stdafx.h"
#include ".\SCPetInfo.h"
#include <BitStream.hpp>

//==================================================================================================
//
void
SCPetInfo::SetInfo(nsSlot::SerializeItemOption::sPet* src, const BASE_ITEMINFO* item_info)
{
    Clear();
    if(!FlowControl::FCAssert(item_info->m_wType == eITEMTYPE_PET))
        return;

    SLOT_BITBOUND_SET(src->Fullness, SCPetInfo::NumberOfBits_Fullness);
    SLOT_BITBOUND_SET(src->Intimacy, SCPetInfo::NumberOfBits_Intimacy);

    fullness_ = BYTE(src->Fullness);
    intimacy_ = BYTE(src->Intimacy);

    BOOST_STATIC_ASSERT(MAX_PET_NAME_LENGTH == 10);
    SLOT_BITBOUND_SET(src->IsSetPetName, SCPetInfo::NumberOfBits_SetPetName);
    if(src->IsSetPetName) {
        CopyMemory(pet_name_, src->Name, MAX_PET_NAME_LENGTH);
    }
}

void
SCPetInfo::GetInfo(nsSlot::SerializeItemOption::sPet* dest)
{
    SLOT_BITBOUND_SET(fullness_, SCPetInfo::NumberOfBits_Fullness);
    SLOT_BITBOUND_SET(intimacy_, SCPetInfo::NumberOfBits_Intimacy);

    dest->Fullness = fullness_;
    dest->Intimacy = intimacy_;
    dest->IsSetPetName = pet_name_[0] != 0;
    if(dest->IsSetPetName) {
        CopyMemory(dest->Name, pet_name_, MAX_PET_NAME_LENGTH);
    }
}

void
SCPetInfo::SetPetName(const char pet_name[MAX_PET_NAME_LENGTH]) const
{
    SCPetInfo* this_ = const_cast<SCPetInfo*>(this);
    BOOST_STATIC_ASSERT(_countof(pet_name_) == MAX_PET_NAME_LENGTH + 1);
    //                                  기존에 10으로 사용되던 것이기에...
    strncpy(this_->pet_name_, pet_name, _countof(this_->pet_name_) - 1);
    this_->pet_name_[_countof(this_->pet_name_) - 1] = '\0';
}

//
//==================================================================================================
//==================================================================================================
//==================================================================================================
