#include "stdafx.h"
#include "./SCLottoInfo.h"

#include "BitStream.hpp"

#include "SCItemSlot.h"
#include "LotteryInfoParser.h"

//==================================================================================================
//
const SCLottoInfo::MiniSlot
SCLottoInfo::NullMiniSlot = { 0, };

void
SCLottoInfo::SetInfo(nsSlot::SerializeItemOption::sLottery* src, const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    Clear();

    SLOT_BITBOUND_SET(src->Index, SCLottoInfo::NumberOfBits_LotteryIndex);
    SLOT_BITBOUND_SET(src->Identity, SCLottoInfo::NumberOfBits_IdentityStatus);
    SLOT_BITBOUND_SET(src->Count, SCLottoInfo::NumberOfBits_NumberOfMiniSlots);

    lottery_index_ = WORD(src->Index);
    identity_status_ = BOOLEAN(src->Identity);
    number_of_minislots_ = static_cast<uint8_t>(src->Count);

    for(int i = 0; i < _countof(mini_slots_); ++i) {
        SLOT_BITBOUND_SET(src->Slots[i].Random, NumberOfBits_MiniSlot_IsRandom);
        SLOT_BITBOUND_SET(src->Slots[i].Exist, NumberOfBits_MiniSlot_ExistItem);
        SLOT_BITBOUND_SET(src->Slots[i].Position, NumberOfBits_MiniSlot_InfoPos);
        SCLottoInfo::MiniSlot& dest = mini_slots_[i];
        dest.Random = BOOLEAN(src->Slots[i].Random);
        dest.ExistItem = BOOLEAN(src->Slots[i].Exist);
        dest.InfoPos = BYTE(src->Slots[i].Position);
    }

}

void
SCLottoInfo::GetInfo(nsSlot::SerializeItemOption::sLottery* dest)
{
    SLOT_BITBOUND_SET(lottery_index_,       NumberOfBits_LotteryIndex);
    SLOT_BITBOUND_SET(identity_status_,     NumberOfBits_IdentityStatus);
    SLOT_BITBOUND_SET(number_of_minislots_, NumberOfBits_NumberOfMiniSlots);

    dest->Index      = lottery_index_;
    dest->Identity   = identity_status_;
    dest->Count      = number_of_minislots_;

    for(int i = 0; i < _countof(mini_slots_); ++i) {
        SCLottoInfo::MiniSlot& src = mini_slots_[i];
        SLOT_BITBOUND_SET_1B(src.Random     , NumberOfBits_MiniSlot_IsRandom);
        SLOT_BITBOUND_SET_1B(src.ExistItem  , NumberOfBits_MiniSlot_ExistItem);
        SLOT_BITBOUND_SET_1B(src.InfoPos    , NumberOfBits_MiniSlot_InfoPos);

        dest->Slots[i].Random = src.Random;
        dest->Slots[i].Exist  = src.ExistItem;
        dest->Slots[i].Position = src.InfoPos;
    }
}

void
SCLottoInfo::ClearMiniSlot(POSTYPE pos)
{
    if(!FlowControl::FCAssert(number_of_minislots_ && ValidPosition(pos)))
        return;

    ZeroMemory(&mini_slots_[pos], sizeof(mini_slots_[pos]));
    if(number_of_minislots_)
        --number_of_minislots_;
}

void
SCLottoInfo::Move(POSTYPE from, POSTYPE to)
{
    if(FlowControl::FCAssert(ValidPosition(from) && ValidPosition(to))) {
        const MiniSlot temp = mini_slots_[from];
        mini_slots_[from] = mini_slots_[to];
        mini_slots_[to] = temp;
    }
}

// 스크립트에서 m_LottoIndex에 해당하는 정보를 랜덤으로 
// minislot에 설정한다.
BOOLEAN
SCLottoInfo::Identity()
{
    if(identity_status_)
        return false;

    // DONE: f110610.2L, waverix, 관련 로직을 파악할 필요 있다.
    // 일단 동일한 동작으로 처리하고, 문제점을 파악하자.
    //minislot_count 사용이 필요할 듯 한데, 로직이 좀 더 분석될 필요 있음.

    LotteryInfoParser* pLotteryInfoParser = LotteryInfoParser::Instance(); 
    const LotteryIndexGroup* lottery_index_group =
        pLotteryInfoParser->GetLotteryIndexGroup(lottery_index_);
    if(lottery_index_group != 0)
    {
        number_of_minislots_ = lottery_index_group->GetMiniSlotCount();
        ASSERT(0 < number_of_minislots_ && number_of_minislots_ <= MAX_PANDORA_SLOT_NUM);
        number_of_minislots_ = min(number_of_minislots_, MAX_PANDORA_SLOT_NUM);

        for(int i = 0; i < number_of_minislots_; ++i)
        {
            const MiniSlotInfo* mini_slot_info = lottery_index_group->GetMiniSlotInfo(i);
            assert(mini_slot_info != NULL);
            if (MiniSlot* dest = &mini_slots_[i])
            {
                dest->InfoPos = static_cast<uint8_t>(i);
                dest->ExistItem = true;
                dest->Random = mini_slot_info->Random != false;
            };
        }

        identity_status_ = true;
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|can't find lottery info(%d)|"),
               __FUNCTION__, lottery_index_);
        return FALSE;
    }

    return TRUE;
}


//==================================================================================================
//==================================================================================================
//==================================================================================================
//



