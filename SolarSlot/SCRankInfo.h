#ifndef _SOLAR_SLOT_SCRANKINFO_H
#define _SOLAR_SLOT_SCRANKINFO_H
#pragma once

//==================================================================================================
/// ������ ��ũ �ɼ� ���� Ŭ����
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2006. 1. 9
    @remark
    - ������ �ɼǿ��� ��ũ�κ��� ���� ����� Ŭ����ȭ ��Ŵ
    - ���̳ʸ� ��ũ �ɼ� �κ��� Decode�� ������ ��Ÿ��
    - SCItemSlot�� �ɹ��� ����
    - ���������� eATTR_TYPE�� �ִ밪�� ũ�⶧���� ���� ��ũ��Ʈ���� ������� �ʰ�, 
    ���� AttrIndex�� eATTR_TYPE�� �����Ͽ� ���� �������� ����� ���Ѵ�.
    (sRANK_ATTR_VALUE�� �ɹ� m_AttrIndex -> m_AttrType���� �������� �Ѵ�. Decode()��)
    @note
    - sRANK_ATTR_VALUE�� �迭 RANK_E���� ������ ����
    @history 
    - 09.09.06 | waverix | code rearrangement
    @usage
    - ex)
        1. RANK_E �������� RANK_D�� ��ũ�� ���� ��
        SetRankAttr(RANK_D, eATTR_TYPE, 3);
        2. RANK_D �������� RANK_C�� ��ũ�� ���� ��
        SetRankAttr(RANK_C, eATTR_TYPE, 2);
*/
//==================================================================================================

#include "ItemOptionHeader.h"
#include <RankOptionParser.h>
#include "SCSlotStruct.h"

class BitStream;

//==================================================================================================
//
class SCRankInfo
{
    // ���� Slot���� ����ϴ� ����
    // ���� �������� ���� �ɼ� ������ ������
    struct AttrValues {
        eRANK rank_;
        int attr_index_;
        eATTR_TYPE attr_type_;
        int attr_value_;
        const sRANK_OPTION* rank_option_;
    };

    // for editable object
    void Clear();

    //void Encode(BitStream& IN bitstream) const;
    //void Decode(const WORD item_type, BitStream& OUT bitstream);

    //void Encode(OPTIONPART_VER5* OUT dest) const;
    //void Decode(const WORD item_type, const OPTIONPART_VER5* IN src);
private:
    void SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* item_info);
    //void SetInfo(const nsSlot::SerializeItemOption::sLimited& src, const BASE_ITEMINFO* item_info);
    //void SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info);

    void GetInfo(nsSlot::SerializeItemOption::sEquip* dest);
    void GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest);
    void GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest);
    void GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest);
    void GetInfo(nsSlot::SerializeItemOption::sLimited* dest);
    //void GetInfo(nsSlot::SerializeItemOption::sFate* dest);


    // (ADD-IN) �־��� ��ũ�� ������ �ƴ� �����, empty value�� ä���� RANK_E { 0, }�� ��ȯ�Ѵ�.
    const AttrValues& GetRankAttrBlock(eRANK rank) const;
    static BOOLEAN IsAcceptableRank(int rank);


    // (NOTE) RankUp�� ���� ���� : SetRank(cur_rank + 1); SetRankAttr(cur_rank + 1, rank_option)
    //        RankDown��         : DelRankAttr(cur_rank);
    // - ��� ��������� ���� �ܺο��� ����� ��, ��~�� �����ؾ� �Ѵ�. stack���·� ó���ϴ°� ��
    // ���� �Ŷ� ����������... �߰��� ��ũ ������ �ٲ� ���� �ִ� ��ʴ� ����� �ִ�. SCItemSlot��
    // Rank���� �Լ��� ������ �������.
    // (PROPOSAL)
    //  UpdateRank  : setup random access rank [E, MAX) boundary
    //  RankUp      : current rank + 1
    //  RankDown    : current rank - 1
    void SetRankAttr(eRANK rank, const sRANK_OPTION* rank_option);
    void DelRankAttr(eRANK rank);

    //const sRANK_OPTION* GetRankAttrDesc(eRANK rank) const { return m_RankAttribute[rank].m_pRankOption; }

    //static eRANK_OPTION_ITEM_TYPE GetRankOptionItemType(DWORD ItemInfoType);
    void _SetInfo(ulong rank, const ulong attr_index[RANK_MAX], const BASE_ITEMINFO* item_info);
    void _GetInfo(ulong* rank, ulong attr_index[RANK_MAX]);

    static void _SetRankAttr(SCRankInfo::AttrValues* OUT dest, eRANK rank, const sRANK_OPTION* rank_option);
    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_Rank = 4;
    static const DWORD NumberOfBits_RankAttrIndex = 7;
    // (WAVERIX) (CRITICAL NOTE) bitstream���� ����ϴ� ��Ʈ ���� 9�̴�.
    // OPTIONPART::RankOptions : 7�� ���ǵǾ� ����.
    //----------------------------------------------------------------------------------------------
    BYTE current_rank_;
    AttrValues rank_attributes_[RANK_MAX];

    friend class SCItemSlot;
};

//--------------------------------------------------------------------------------------------------

inline void
SCRankInfo::Clear() {
    BOOST_STATIC_ASSERT(RANK_E == 0); // current_rank_, rank_... default RANK_E(grade: no rank)
    ZeroMemory(this, sizeof(*this));
}

// (ADD-IN) �־��� ��ũ�� ������ �ƴ� �����, empty value�� ä���� RANK_E { 0, }�� ��ȯ�Ѵ�.
inline const SCRankInfo::AttrValues&
SCRankInfo::GetRankAttrBlock(eRANK rank) const {      //[RANK_E, RANK_MAX)
    if(FlowControl::FCAssert(RANK_E <= rank && rank <= (eRANK)current_rank_)) {
        return rank_attributes_[rank];
    }
    return rank_attributes_[RANK_E];
}

inline BOOLEAN
SCRankInfo::IsAcceptableRank(int rank) {
    // Acceptalbe list
    // { RANK_D, RANK_C, RANK_B, RANK_MA, RANK_A, RANK_PA, RANK_MS, RANK_S, RANK_PS, }
    return RANK_D <= rank && rank < RANK_MAX;
}


inline void
SCRankInfo::SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.Rank, src.RankOption, item_info);
}

inline void
SCRankInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.Rank, src.RankOption, item_info);
}

inline void
SCRankInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.Rank, src.RankOption, item_info);
}

inline void
SCRankInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.Rank, src.RankOption, item_info);
}

//inline void
//SCRankInfo::SetInfo(const nsSlot::SerializeItemOption::sLimited& src, const BASE_ITEMINFO* item_info) {
//    _SetInfo(src.Rank, src.RankOption, item_info);
//}
//
//inline void
//SCRankInfo::SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info) {
//    _SetInfo(src.Rank, src.RankOption, item_info);
//}


inline void
SCRankInfo::GetInfo(nsSlot::SerializeItemOption::sEquip* dest) {
    _GetInfo(&dest->Rank, dest->RankOption);
}

inline void
SCRankInfo::GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest) {
    _GetInfo(&dest->Rank, dest->RankOption);
}

inline void
SCRankInfo::GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest) {
    _GetInfo(&dest->Rank, dest->RankOption);
}

inline void
SCRankInfo::GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest) {
    _GetInfo(&dest->Rank, dest->RankOption);
}

//inline void
//SCRankInfo::GetInfo(nsSlot::SerializeItemOption::sLimited* dest) {
//    _GetInfo(&dest->Rank, dest->RankOption);
//}
//
//inline void
//SCRankInfo::GetInfo(nsSlot::SerializeItemOption::sFate* dest) {
//    _GetInfo(&dest->Rank, dest->RankOption);
//}




//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //_SOLAR_SLOT_SCRANKINFO_H
