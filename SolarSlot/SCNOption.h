#ifndef _SOLAR_SLOT_SCNOPTION_H
#define _SOLAR_SLOT_SCNOPTION_H
#pragma once

//==================================================================================================
/// ������ NOption ���� Ŭ����
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2006. 3. 17
    @remark
    - ���̳ʸ� Noption �ɼ� �κ��� Decode�� ������ ��Ÿ��
    - SCItemSlot�� �ɹ��� ����
    - ���������� eATTR_TYPE�� �ִ밪�� ũ�⶧���� ���� ��ũ��Ʈ���� ������� �ʰ�, 
    ���� AttrIndex�� eATTR_TYPE�� �����Ͽ� ���� �������� ����� ���Ѵ�.
    (sNOPTION_ATTR_VALUE�� �ɹ� m_AttrIndex -> m_AttrType���� �������� �Ѵ�. Decode()��)
    @usage
    - �Ӽ� ���� ���� üũ �� : 
    GetNOptionAttrIndex() == 0 �Ǵ�
    GetNOptionAttr() == eATTR_TYPE_INVALID �Ǵ�
    //GetNOptionAttrValue() == 0 �̸�
    �Ӽ��� �������� �ʴ´�.
    @history 
    - 09.09.06 | waverix | code rearrangement
*/
//==================================================================================================

#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"
#include <ExtraStoneOptionInfoParser.h>

class BitStream;

//==================================================================================================
//
//==================================================================================================
// (DESC) NOption�� ExtraStone Option�� ���õ� �����̴�. �̸� ������� ������ �ȵ�����... �׷���.
class SCNOption
{
    void Clear();

    //void Encode(BitStream& IN bitstream) const;
    //void Decode(WORD item_type, LEVELTYPE item_level, BitStream& OUT bitstream);

    //void Encode(OPTIONPART_VER5* OUT dest) const;
    //void Decode(WORD item_type, LEVELTYPE item_level, const OPTIONPART_VER5* IN src);
private:
    void SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sLimited& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info);

    void GetInfo(nsSlot::SerializeItemOption::sEquip* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sLimited* dest) const;
    void GetInfo(nsSlot::SerializeItemOption::sFate* dest) const;

    void SetNOptionAttr(int iAttrValueIndex, const sEXTRASTONE_OPT_INFO* pOption);
    void SetNOptionAttr(WORD item_type, LEVELTYPE item_level, int iAttrValueIndex);

    void _SetInfo(ulong attr_index, const BASE_ITEMINFO* item_info);
    void _GetInfo(ulong* attr_index) const;
    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_AttrIndex = 3;
    //----------------------------------------------------------------------------------------------
    int attr_index_;           // 3: NumberOfBits_AttrIndex
    eATTR_TYPE attr_type_;
    int attr_value_index_;
    int attr_value_;
    const sEXTRASTONE_OPT_INFO* n_option_;

    friend class SCItemSlot;
};

//--------------------------------------------------------------------------------------------------

inline void
SCNOption::Clear()
{
    BOOST_STATIC_ASSERT(eATTR_TYPE_INVALID == 0); //attr_type_ = eATTR_TYPE_INVALID
    ZeroMemory(this, sizeof(*this));
}

inline void
SCNOption::_SetInfo(ulong attr_index, const BASE_ITEMINFO* item_info) {
    Clear();
    SLOT_BITBOUND_SET(attr_index, NumberOfBits_AttrIndex);
    SetNOptionAttr(item_info->m_wType, item_info->m_LV, attr_index);
}

inline void
SCNOption::_GetInfo(ulong* attr_index) const {
    *attr_index = attr_value_index_;
    SLOT_BITBOUND_SET(*attr_index, NumberOfBits_AttrIndex);
}

inline void
SCNOption::SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NOption, item_info);
}

inline void
SCNOption::SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NOption, item_info);
}

inline void
SCNOption::SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NOption, item_info);
}

inline void
SCNOption::SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NOption, item_info);
}

inline void
SCNOption::SetInfo(const nsSlot::SerializeItemOption::sLimited& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NOption, item_info);
}

inline void
SCNOption::SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NOption, item_info);
}

inline void
SCNOption::GetInfo(nsSlot::SerializeItemOption::sEquip* dest) const {
    _GetInfo(&dest->NOption);
}

inline void
SCNOption::GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest) const {
    _GetInfo(&dest->NOption);
}

inline void
SCNOption::GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest) const {
    _GetInfo(&dest->NOption);
}

inline void
SCNOption::GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest) const {
    _GetInfo(&dest->NOption);
}

inline void
SCNOption::GetInfo(nsSlot::SerializeItemOption::sLimited* dest) const {
    _GetInfo(&dest->NOption);
}

inline void
SCNOption::GetInfo(nsSlot::SerializeItemOption::sFate* dest) const {
    _GetInfo(&dest->NOption);
}




//
//==================================================================================================


#endif //_SOLAR_SLOT_SCNOPTION_H
