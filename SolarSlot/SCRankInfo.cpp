#include "StdAfx.h"
#include ".\scrankinfo.h"
#include <RankOptionParser.h>
#include <BitStream.hpp>

//==================================================================================================
//
__forceinline void
SCRankInfo::_SetRankAttr(SCRankInfo::AttrValues* OUT dest, eRANK rank, const sRANK_OPTION* rank_option)
{
    dest->attr_index_ = rank_option->m_iOptionIndex;
    if(rank_option->m_iOptionIndex < _countof(g_ITEM_OPTION))
        dest->attr_type_ = g_ITEM_OPTION[rank_option->m_iOptionIndex];
    else
        dest->attr_type_ = eATTR_TYPE_INVALID;
    dest->attr_value_ = rank_option->m_Value[rank];
    dest->rank_option_ = rank_option;
}

void
SCRankInfo::_SetInfo(ulong cur_rank, const ulong attr_index[RANK_MAX], const BASE_ITEMINFO* item_info)
{
    Clear();
    SLOT_BITBOUND_SET(cur_rank, NumberOfBits_Rank);

    const WORD item_type = item_info->m_wType;
    RankOptionParser* const pRankOptionParser = RankOptionParser::Instance();
    const int upperbound = cur_rank + 1;
    for(int rank = RANK_D; rank < upperbound; ++rank) {
        AttrValues& dest = rank_attributes_[rank];
        dest.attr_index_ = attr_index[rank];
        if(dest.attr_index_ == 0)
            break;

        current_rank_ = static_cast<uint8_t>(rank);

        const sRANK_OPTION* rank_option =
            pRankOptionParser->GetRankOption(item_type, dest.attr_index_);
        if(rank_option != 0)
            _SetRankAttr(&dest, eRANK(rank), rank_option);
    }
}

void
SCRankInfo::_GetInfo(ulong* rank, ulong rank_attr[RANK_MAX])
{
    SLOT_BITBOUND_SET_1B(current_rank_, NumberOfBits_Rank);
    *rank = current_rank_;

    const int upperbound = *rank + 1;
    for(int rank = RANK_D; rank < upperbound; ++rank) {
        rank_attr[rank] = rank_attributes_[rank].attr_index_;
    }
}

void
SCRankInfo::SetRankAttr(eRANK rank, const sRANK_OPTION* rank_option)
{
    // external interface
    BOOLEAN set_rank_attr_operation_result = rank_option != 0 &&
                                             (RANK_E < rank && rank < RANK_MAX);

    // TODO(WAVERIX) (WARNING) #1 ���� ��ũ�� �����ϴ� ���ΰ�, ���纸�� ���� ��ũ�� �����ϴ� ���ΰ�?
    if(FlowControl::FCAssert(set_rank_attr_operation_result != false)) {
        _SetRankAttr(&rank_attributes_[rank], rank, rank_option);
    }
}

void
SCRankInfo::DelRankAttr(eRANK rank)
{
    // TODO(WAVERIX) (WARNING) #1 Rank down�� ��Ȳ�� �ǰ�? assert ��Ȳ�� �ƴ� �� �ִ�. �� �� ������ �� ��.
    //                         #2 ���� ������ ���� �� ����... �Էµ� rank�� ���� ��ũ��� ������ �ִ°�?
    if(FlowControl::FCAssert(RANK_E < rank && rank < RANK_MAX)) {
        ASSERT(current_rank_ == rank);
        ZeroMemory(&rank_attributes_[rank], sizeof(rank_attributes_[rank]));
        if(current_rank_)
            current_rank_ = eRANK(current_rank_ - 1);
    }
}

