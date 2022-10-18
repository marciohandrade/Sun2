#ifndef _SOLAR_SLOT_SCSOCKETOPTIONINFO_H
#define _SOLAR_SLOT_SCSOCKETOPTIONINFO_H
#pragma once


#include "ItemOptionHeader.h"
#include "SCSlotStruct.h"
#include <SocketOptionParser.h>

class BitStream;

//==================================================================================================
//

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
class SCSocketOptionInfo
{
    friend class SCItemSlot;
    // NOTE: initialize routines of the SCItemSlot have clear memory
    // so that the below structure with std::pair is not neat style
    // although std::pair with clear memory is stable.
    // f :db에 존재하는 인덱스 값
    // s :스크립트 매칭 값 (NULL 가능)
    struct AttrValues {
        SOCKETINDEX first;
        const sSOCKET_OPTION* second;
    };
    //typedef std::pair<SOCKETINDEX, const sSOCKET_OPTION*> AttrValues; // f :db에 존재하는 인덱스 값
    //                                                                  // s :스크립트 매칭 값 (NULL 가능)
private:
    void SetInfo(const nsSlot::SerializeItemOption::sEquip& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sEquipTime& src, const BASE_ITEMINFO* item_info);
    //void SetInfo(nsSlot::SerializeItemOption::sEquipEtc& src, const BASE_ITEMINFO* item_info);
    //void SetInfo(nsSlot::SerializeItemOption::sEquipEtcTime& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sLimited& src, const BASE_ITEMINFO* item_info);
    void SetInfo(const nsSlot::SerializeItemOption::sFate& src, const BASE_ITEMINFO* item_info);

    void GetInfo(nsSlot::SerializeItemOption::sEquip* dest);
    void GetInfo(nsSlot::SerializeItemOption::sEquipTime* dest);
    //void GetInfo(nsSlot::SerializeItemOption::sEquipEtc* dest);
    //void GetInfo(nsSlot::SerializeItemOption::sEquipEtcTime* dest);
    void GetInfo(nsSlot::SerializeItemOption::sLimited* dest);
    void GetInfo(nsSlot::SerializeItemOption::sFate* dest);

    void Clear();

    BOOLEAN CopySocketInfo(const SCSocketOptionInfo& sock_info);
    void SetSocketOption(eSOCKET sock, SOCKETINDEX index);
    void _SetInfo(ulong socket_identify_type, ulong number_of_sockets, 
                  const ulong socket_index[SOCKET_MAX], const BASE_ITEMINFO* item_info);

    const sSOCKET_OPTION* GetSocketOption(eSOCKET sock) const;
    void _GetInfo(ulong* socket_identify_type, ulong* number_of_sockets, ulong socket_index[SOCKET_MAX]);


private:
    //----------------------------------------------------------------------------------------------
    static const DWORD NumberOfBits_SocketIdentifyType = 2;    
    static const DWORD NumberOfBits_NumberOfSockets = 3;
    static const DWORD NumberOfBits_SocketIndex = 10;
    //----------------------------------------------------------------------------------------------

    BYTE socket_identify_type_;
    BYTE number_of_sockets_;
    AttrValues socket_attributes_[SOCKET_MAX];
};

//--------------------------------------------------------------------------------------------------
// inline function implementation
#define _SOLAR_SLOT_SCSOCKETOPTIONINFO_INL
#include "./SCSocketOptionInfo.inl"
#undef  _SOLAR_SLOT_SCSOCKETOPTIONINFO_INL

//--------------------------------------------------------------------------------------------------
#endif //_NA_0_20100727_SOCKET_SYSTEM_CONVERT

#endif //_SOLAR_SLOT_SCSOCKETINFO_H
