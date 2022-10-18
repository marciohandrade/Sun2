#ifndef _SOLAR_SLOT_SCSOCKETINFO_H
#define _SOLAR_SLOT_SCSOCKETINFO_H
#pragma once

//==================================================================================================
/// ������ ���� ���� Ŭ����
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2006. 1. 9
    @remark
    - ���̳ʸ� ���� �ɼ� �κ��� Decode�� ������ ��Ÿ��
    - SCItemSlot�� �ɹ��� ����
    - ���������� eATTR_TYPE�� �ִ밪�� ũ�⶧���� ���� ��ũ��Ʈ���� ������� �ʰ�, 
    ���� AttrIndex�� eATTR_TYPE�� �����Ͽ� ���� �������� ����� ���Ѵ�.
    (sRANK_ATTR_VALUE�� �ɹ� m_AttrIndex -> m_AttrType���� �������� �Ѵ�. Decode()��)
    @note
    - m_AttrIndex == 0          : �Ӽ��� ����
    - 1 <= m_AttrIndex <= 31    : �ɼ��� �ε���

    - eSOCKET_LEVEL�� ���� ���� SocketOption1,2,3�� ���� +0(�ϱ�),+31(���)�� ���ش�.
    - �Ӽ� ���� ���� üũ �� : 
        GetSocketAttrIndex(sock) == 0 �Ǵ�
        GetSocketAttr(sock) == eATTR_TYPE_INVALID �Ǵ�
        //GetSocketAttrValue(sock) == 0 �̸�
        �Ӽ��� �������� �ʴ´�.
    @history 
    - 09.09.06 | waverix | code rearrangement
*/
//==================================================================================================

#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"
#include <SocketOptionParser.h>

class BitStream;

//==================================================================================================
//
class SCSocketInfo
{
    // ���� Slot���� ����ϴ� ����
    // ���� �������� ���� �ɼ� ������ ������
    struct AttrValues {
        eSOCKET_LEVEL socket_level_;
        int attr_index_;
        eATTR_TYPE attr_type_;
        int attr_value_;
        const sSOCKET_OPTION* socket_option_;
    };

    SCSocketInfo& GetEditableObject() const { return const_cast<SCSocketInfo&>(*this); }
    void Clear();

    //void Encode(BitStream& IN bitstream) const;
    //void Decode(BitStream& OUT bitstream);

    //void Encode(const WORD sub_type, OPTIONPART_VER5* OUT dest) const;
    //void Decode(const WORD sub_type, const OPTIONPART_VER5* IN src);

    const AttrValues& GetSocketAttrBlock(eSOCKET sock) const;

private:
    void SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* item_info);
    //void SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* item_info);
    //void SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sLimited& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info);

    void GetInfo(nsSlot::SerializeItemOption::sEquip* dest);
    void GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest);
    //void GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest);
    //void GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest);
    void GetInfo(nsSlot::SerializeItemOption::sLimited* dest);
    void GetInfo(nsSlot::SerializeItemOption::sFate* dest);

    //BYTE GetSocketNum() const { return number_of_sockets_; }
    //void SetSocketNum(BYTE num) { number_of_sockets_ = num; }
    void SetSocketAttr(eSOCKET sock, eSOCKET_LEVEL level, const sSOCKET_OPTION* socket_option);
    BOOLEAN CopySocketInfo(const SCSocketInfo& sock_info);

    void _SetInfo(ulong number_of_sockets, const ulong sockets[SOCKET_MAX], const BASE_ITEMINFO* item_info);
    void _GetInfo(ulong* number_of_sockets, ulong sockets[SOCKET_MAX]);

    int SocketIndexToBinary(const AttrValues* node) const;
    BOOLEAN BinaryToSocketIndex(int IN binary_index, int& OUT attr_index, eSOCKET_LEVEL& OUT level);

    static void _SetSocketAttr(SCSocketInfo::AttrValues* OUT dest, eSOCKET_LEVEL level,
                               const sSOCKET_OPTION* socket_option);
    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_NumberOfSockets = 2;
    static const DWORD NumberOfBits_SocketIndex = 8;
    //----------------------------------------------------------------------------------------------
    BYTE number_of_sockets_;
    AttrValues socket_attributes_[SOCKET_MAX];
    AttrValues dummy_;

    friend class SCItemSlot;
};

//--------------------------------------------------------------------------------------------------

inline void
SCSocketInfo::Clear() {
    ZeroMemory(this, sizeof(*this));
}

//inline void
//SCSocketInfo::_SetInfo(ulong attr_index, const BASE_ITEMINFO* item_info) {
//    Clear();
//    SLOT_BITBOUND_SET(attr_index, NumberOfBits_AttrIndex);
//    SetNOptionAttr(item_info->m_wType, item_info->m_LV, attr_index);
//}
//
//inline void
//SCSocketInfo::_GetInfo(ulong* attr_index) const {
//    *attr_index = attr_index_;
//    SLOT_BITBOUND_SET(*attr_index, NumberOfBits_AttrIndex);
//}

inline void
SCSocketInfo::SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NumberOfSockets, src.SocketOption, item_info);
}

inline void
SCSocketInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NumberOfSockets, src.SocketOption, item_info);
}

//inline void
//SCSocketInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* item_info) {
//    _SetInfo(src.NumberOfSockets, src.SocketOption, item_info);
//}
//
//inline void
//SCSocketInfo::SetInfo(const nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* item_info) {
//    _SetInfo(src.NumberOfSockets, src.SocketOption, item_info);
//}

inline void
SCSocketInfo::SetInfo(const nsSlot::SerializeItemOption::sLimited& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NumberOfSockets, src.SocketOption, item_info);
}

inline void
SCSocketInfo::SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info) {
    _SetInfo(src.NumberOfSockets, src.SocketOption, item_info);
}

inline void
SCSocketInfo::GetInfo(nsSlot::SerializeItemOption::sEquip* dest) {
    _GetInfo(&dest->NumberOfSockets, dest->SocketOption);
}

inline void
SCSocketInfo::GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest) {
    _GetInfo(&dest->NumberOfSockets, dest->SocketOption);
}

//inline void
//SCSocketInfo::GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest) {
//    _GetInfo(&dest->NumberOfSockets, dest->SocketOption);
//}
//
//inline void
//SCSocketInfo::GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest) {
//    _GetInfo(&dest->NumberOfSockets, dest->SocketOption);
//}

inline void
SCSocketInfo::GetInfo(nsSlot::SerializeItemOption::sLimited* dest) {
    _GetInfo(&dest->NumberOfSockets, dest->SocketOption);
}

inline void
SCSocketInfo::GetInfo(nsSlot::SerializeItemOption::sFate* dest) {
    _GetInfo(&dest->NumberOfSockets, dest->SocketOption);
}

inline const SCSocketInfo::AttrValues&
SCSocketInfo::GetSocketAttrBlock(eSOCKET sock) const {
    if(FlowControl::FCAssert(sock < SOCKET_MAX && sock < number_of_sockets_)) {
        return socket_attributes_[sock];
    }
    return dummy_;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
//

#endif //_SOLAR_SLOT_SCSOCKETINFO_H
