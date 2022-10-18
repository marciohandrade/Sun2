#ifndef _SOLAR_SLOT_SCSOCKETINFO_H
#define _SOLAR_SLOT_SCSOCKETINFO_H
#pragma once

//==================================================================================================
/// 아이템 소켓 정보 클래스
/**
    @author Kim Min Wook < taiyo@webzen.co.kr >
    @since  2006. 1. 9
    @remark
    - 바이너리 소켓 옵션 부분을 Decode한 정보를 나타냄
    - SCItemSlot의 맴버로 사용됨
    - 아이템쪽은 eATTR_TYPE의 최대값이 크기때문에 직접 스크립트에서 사용하지 않고, 
    따로 AttrIndex로 eATTR_TYPE에 대응하여 값을 가져오는 방식을 취한다.
    (sRANK_ATTR_VALUE의 맴버 m_AttrIndex -> m_AttrType으로 컨버팅을 한다. Decode()시)
    @note
    - m_AttrIndex == 0          : 속성이 없음
    - 1 <= m_AttrIndex <= 31    : 옵션의 인덱스

    - eSOCKET_LEVEL의 값에 따라 SocketOption1,2,3의 값을 +0(하급),+31(상급)을 해준다.
    - 속성 존재 여부 체크 시 : 
        GetSocketAttrIndex(sock) == 0 또는
        GetSocketAttr(sock) == eATTR_TYPE_INVALID 또는
        //GetSocketAttrValue(sock) == 0 이면
        속성은 존재하지 않는다.
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
    // 실제 Slot에서 사용하는 정보
    // 현재 아이템이 가진 옵션 정보의 누적값
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
