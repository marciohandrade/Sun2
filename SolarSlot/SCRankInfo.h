#ifndef _SOLAR_SLOT_SCRANKINFO_H
#define _SOLAR_SLOT_SCRANKINFO_H
#pragma once

//==================================================================================================
/// 아이템 랭크 옵션 정보 클래스
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2006. 1. 9
    @remark
    - 아이템 옵션에서 랭크부분을 따로 떼어내서 클래스화 시킴
    - 바이너리 랭크 옵션 부분을 Decode한 정보를 나타냄
    - SCItemSlot의 맴버로 사용됨
    - 아이템쪽은 eATTR_TYPE의 최대값이 크기때문에 직접 스크립트에서 사용하지 않고, 
    따로 AttrIndex로 eATTR_TYPE에 대응하여 값을 가져오는 방식을 취한다.
    (sRANK_ATTR_VALUE의 맴버 m_AttrIndex -> m_AttrType으로 컨버팅을 한다. Decode()시)
    @note
    - sRANK_ATTR_VALUE의 배열 RANK_E값은 사용되지 않음
    @history 
    - 09.09.06 | waverix | code rearrangement
    @usage
    - ex)
        1. RANK_E 아이템을 RANK_D로 랭크업 했을 때
        SetRankAttr(RANK_D, eATTR_TYPE, 3);
        2. RANK_D 아이템을 RANK_C로 랭크업 했을 때
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
    // 실제 Slot에서 사용하는 정보
    // 현재 아이템이 가진 옵션 정보의 누적값
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


    // (ADD-IN) 주어진 랭크가 정상이 아닌 경우라면, empty value로 채워진 RANK_E { 0, }을 반환한다.
    const AttrValues& GetRankAttrBlock(eRANK rank) const;
    static BOOLEAN IsAcceptableRank(int rank);


    // (NOTE) RankUp시 기존 로직 : SetRank(cur_rank + 1); SetRankAttr(cur_rank + 1, rank_option)
    //        RankDown시         : DelRankAttr(cur_rank);
    // - 상기 로직대로일 경우는 외부에서 사용할 때, 졸~라 주의해야 한다. stack형태로 처리하는게 더
    // 나을 거라 생각되지만... 중간의 랭크 정보를 바꿀 수도 있는 사례는 충분히 있다. SCItemSlot의
    // Rank관련 함수의 수정을 고려하자.
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
    // (WAVERIX) (CRITICAL NOTE) bitstream으로 사용하는 비트 수는 9이다.
    // OPTIONPART::RankOptions : 7로 정의되어 있음.
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

// (ADD-IN) 주어진 랭크가 정상이 아닌 경우라면, empty value로 채워진 RANK_E { 0, }을 반환한다.
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
