#include "stdafx.h"
#include ".\SCFateInfo.h"
#include <BitStream.hpp>

#include <ItemInfoParser.h>


void
SCFateInfo::SetInfo(nsSlot::SerializeItemOption::sFate* src, const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    Clear();
    SLOT_BITBOUND_SET(src->AtkDefRatio, SCFateInfo::NumberOfBits_Ratio);
    SLOT_BITBOUND_SET(src->Duration,    SCFateInfo::NumberOfBits_MaxDuration);
    SLOT_BITBOUND_SET(src->IsIdentify,  SCFateInfo::NumberOfBits_Identify);
    SLOT_BITBOUND_SET(src->Period,      SCFateInfo::NumberOfBits_Period);

    ratio_        = WORD(src->AtkDefRatio);
    max_duration_ = DURATYPE(src->Duration);
    identify_     = !!src->IsIdentify;
    period_       = WORD(src->Period);
}

void
SCFateInfo::GetInfo(nsSlot::SerializeItemOption::sFate* dest)
{
    SLOT_BITBOUND_SET(ratio_,        SCFateInfo::NumberOfBits_Ratio);
    SLOT_BITBOUND_SET(max_duration_, SCFateInfo::NumberOfBits_MaxDuration);
    SLOT_BITBOUND_SET(identify_,     SCFateInfo::NumberOfBits_Identify);
    SLOT_BITBOUND_SET(period_,       SCFateInfo::NumberOfBits_Period);

    dest->AtkDefRatio = ratio_;
    dest->Duration    = max_duration_;
    dest->IsIdentify  = identify_;
    dest->Period      = period_;
}

BOOLEAN
SCFateInfo::CalFate(SLOTCODE item_code, DWORD fate_index)
{
    const BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if(pItemInfo == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|can't find item info, code=%d|"),
               __FUNCTION__, item_code);
        ASSERT(false);
        return false;
    }   

    // 이미 감정 했음.
    if(identify_ != 0)
        return false;

    FateItemInfoParser* const pFateItemInfoParser = FateItemInfoParser::Instance();
    ushort random_value = 0;
    {
        random_value = pFateItemInfoParser->GetRandomkeyFromFateDuraRatio(fate_index);
        ulong max_duration = ulong(pItemInfo->m_Dura * random_value / 100.0f);
        ASSERT(SAFE_NUMERIC_TYPECAST(int, max_duration, DURATYPE));
        max_duration_ = DURATYPE(max_duration);
    };
    {
        random_value = pFateItemInfoParser->GetRandomkeyFromFateAtkDefRatio(fate_index);
        ratio_ = random_value;
    };

    identify_ = true;

    return true;
}

