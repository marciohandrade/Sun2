#include "Stdafx.h"
#include ".\SCLimitedInfo.h"

#include <BitStream.hpp>
#include <ItemInfoParser.h>

//#define CHECK_VALUE(Value, MinV, MaxV) \
//    if(Value < MinV || Value > MAxV) \
//    {                                               \
//        ASSERT(FALSE); \
//    }

void
SCLimitedInfo::SetInfo(nsSlot::SerializeItemOption::sLimited* src, const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    Clear();

    SLOT_BITBOUND_SET(src->Index,           NumberOfBits_LimitedIndex);
    SLOT_BITBOUND_SET(src->DurationForRestore, NumberOfBits_MaxDurationForRestore);
    SLOT_BITBOUND_SET(src->Duration,        NumberOfBits_MaxDuration);
    SLOT_BITBOUND_SET(src->NumberOfRepairs, NumberOfBits_NumberOfRepairs);
    SLOT_BITBOUND_SET(src->DefenceRatio,     NumberOfBits_DefenceRatio);
    SLOT_BITBOUND_SET(src->LimitValueRatio, NumberOfBits_LimitValueRatio);

    limited_index_     = WORD(src->Index);
    max_duration_for_restore_ = DURATYPE(src->DurationForRestore);
    max_duration_      = DURATYPE(src->Duration);
    number_of_repairs_ = BYTE(src->NumberOfRepairs);
    defence_ratio_     = src->DefenceRatio;
    limit_value_ratio_ = src->LimitValueRatio;

/*
    // (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
    BOOST_STATIC_ASSERT(LIMITED_MAX == 3 &&
                        LIMITED_MAX == _countof(limited_attributes_));

    for(int i = 0; i < _countof(limited_attributes_); ++i) {
        SLOT_BITBOUND_SET(src->LimitedOption[i].Option, NumberOfBits_AttrIndex);
        SLOT_BITBOUND_SET(src->LimitedOption[i].Value, NumberOfBits_AttrValue);
        limited_attributes_[i].attr_index_ = src->LimitedOption[i].Option;
        limited_attributes_[i].attr_value_ = src->LimitedOption[i].Value;
    }
*/
    //----------------------------------------------------------------------------------------------
    // modification
    defence_ratio_ -= 100;
    limit_value_ratio_ -= 100;
/*
    // (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
    LimitedOptionParser* const pLimitedOptionParser = LimitedOptionParser::Instance();
    for(int i = 0; i < _countof(limited_attributes_); ++i) {
        SCLimitedInfo::AttrValues& dest = limited_attributes_[i];

        const sLIMITED_OPTION* limited_option =
            pLimitedOptionParser->GetLimitedOptionByKind(dest.attr_index_);
        if(limited_option == 0)
            continue;

        // (NOTE) 아래와 같은 코드는... 스크립트가 달라지거나, g_ITEM_OPTION의 중간에 코드 삽입되는
        // 테러를 당했을 때? 스트림 망가졌을 때? ...hummm...
        dest.attr_index_ = limited_option->m_AttrIndex;
        // (BUG-FIX) sizeof -> _countof
        if(limited_option->m_AttrIndex < _countof(g_ITEM_OPTION))
            dest.attr_type_ = g_ITEM_OPTION[limited_option->m_AttrIndex];
        else
            dest.attr_type_ = eATTR_TYPE_INVALID;
        //dest.m_AttrValue <- 이미 설정되어 있다.
        dest.limited_option_ = limited_option;
    }
*/
}

void
SCLimitedInfo::GetInfo(nsSlot::SerializeItemOption::sLimited* dest) /*const*/
{
    //----------------------------------------------------------------------------------------------
    int defence_ratio_check_boundary = defence_ratio_ + 100;
    {   // damage ratio
        if(defence_ratio_check_boundary < 0 || 500 < defence_ratio_check_boundary) {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|ratio(%d) boundary overflow|"),
                   __FUNCTION__, defence_ratio_check_boundary);
            ASSERT(FALSE);
            // (ADD-IN) modify min-max
            if(defence_ratio_check_boundary < 0)
                defence_ratio_check_boundary = 0;
            if(500 < defence_ratio_check_boundary)
                defence_ratio_check_boundary = 500;
        }
    };
    int limit_value_ratio_check_boundary = limit_value_ratio_ + 100;
    {   // damage value
        if(limit_value_ratio_check_boundary < 0 || 500 < limit_value_ratio_check_boundary) {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|value ratio(%d) boundary overflow|"),
                   __FUNCTION__, limit_value_ratio_check_boundary);
            ASSERT(FALSE);
            // (ADD-IN) modify min-max
            if(limit_value_ratio_check_boundary < 0)
                limit_value_ratio_check_boundary = 0;
            if(500 < limit_value_ratio_check_boundary)
                limit_value_ratio_check_boundary = 500;
        }
    }
    //----------------------------------------------------------------------------------------------

    SLOT_BITBOUND_SET(limited_index_,       NumberOfBits_LimitedIndex);
    SLOT_BITBOUND_SET(max_duration_for_restore_, NumberOfBits_MaxDurationForRestore);
    SLOT_BITBOUND_SET(max_duration_,        NumberOfBits_MaxDuration);
    SLOT_BITBOUND_SET(number_of_repairs_,   NumberOfBits_NumberOfRepairs);
    SLOT_BITBOUND_SET(defence_ratio_check_boundary,       NumberOfBits_DefenceRatio);
    SLOT_BITBOUND_SET(limit_value_ratio_check_boundary,   NumberOfBits_LimitValueRatio);

    dest->Index              = limited_index_;
    dest->DurationForRestore = max_duration_for_restore_;
    dest->Duration           = max_duration_;
    dest->NumberOfRepairs    = number_of_repairs_;
    dest->DefenceRatio       = defence_ratio_check_boundary;
    dest->LimitValueRatio    = limit_value_ratio_check_boundary;

/*
    // (CHANGES) (UNUSED SECTION) (WAVERIX) - 기획 확인
    BOOST_STATIC_ASSERT(LIMITED_MAX == 3 &&
                        LIMITED_MAX == _countof(limited_attributes_));

    for(int i = 0; i < _countof(limited_attributes_); ++i) {
        SLOT_BITBOUND_SET(limited_attributes_[i].attr_index_, NumberOfBits_AttrIndex);
        SLOT_BITBOUND_SET(limited_attributes_[i].attr_value_, NumberOfBits_AttrValue);
        
        SLOT_BITBOUND_SET(dest->LimitedOption[i].Option, NumberOfBits_AttrIndex);
        SLOT_BITBOUND_SET(dest->LimitedOption[i].Value, NumberOfBits_AttrValue);
        dest->LimitedOption[i].Option = limited_attributes_[i].attr_index_;
        dest->LimitedOption[i].Value = limited_attributes_[i].attr_value_;
    }
*/
}


void
SCLimitedInfo::CalLimited(WORD item_code)
{
    if(limited_index_ == 0)
        return;

    LimitedItemInfoParser* const pLimitedItemInfoParser = LimitedItemInfoParser::Instance();
    const sLIMITED_ITEM_INFO* limited_info =
        pLimitedItemInfoParser->GetLimitedItemInfo(limited_index_);
    if(limited_info == 0)
    {
        ASSERT(FALSE);
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|can't find limited index(%d)|"),
               __FUNCTION__, limited_index_);
        return;
    }

    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if(item_info == 0)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|can't find item info(%d)|"),
               __FUNCTION__, item_code);
        ASSERT(FALSE);
        return;
    }   

    // (NOTE) limited item은 기존 아이템보다 내구도가 더 클 수 있는가?
    // 일단 DURATYPE : BYTE의 상한값을 처리하도록 하자.
    // calc - random value
    int random_value = pLimitedItemInfoParser->GetRandomkeyFromDuraRatio(limited_index_);

    DURATYPE item_duration = item_info->m_Dura; // overflow감안
    //iRandomValue / 100.0f * item_duration;
    int bound_check = item_duration + int(random_value * item_duration / 100.0f);
    if(!FlowControl::FCAssert(0 <= bound_check && bound_check <= DURATYPE(-1))) {
        // assert fail
        if(bound_check < 0)
            bound_check = 1; // min?
        else
            bound_check = DURATYPE(-1);
    }

    item_duration = DURATYPE(bound_check);
    max_duration_for_restore_ = item_duration;
    max_duration_ = item_duration;

    random_value = pLimitedItemInfoParser->GetRandomkeyFromLimitValueRatio(limited_index_);
    limit_value_ratio_ = random_value;

    random_value = pLimitedItemInfoParser->GetRandomkeyFromDamageRatio(limited_index_);
    defence_ratio_ = random_value;

    number_of_repairs_ = limited_info->m_byRepairLimite;
}

