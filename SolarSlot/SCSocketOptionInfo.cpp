#include "StdAfx.h"
#include "./SCSocketOptionInfo.h"
#include <SocketOptionParser.h>

//==================================================================================================
//

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT

void SCSocketOptionInfo::_SetInfo(ulong socket_identify_type, ulong number_of_sockets, 
                                  const ulong socket_index[SOCKET_MAX], 
                                  const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    Clear();

    SLOT_BITBOUND_SET(socket_identify_type, NumberOfBits_SocketIdentifyType);
    SLOT_BITBOUND_SET(number_of_sockets, NumberOfBits_NumberOfSockets);
    socket_identify_type_ = static_cast<BOOLEAN>(socket_identify_type);
    number_of_sockets_ = static_cast<BYTE>(number_of_sockets);

    BOOST_STATIC_ASSERT(SOCKET_1 == 0);
    for(BYTE i = SOCKET_1; i < SOCKET_MAX; ++i)
    {
        SetSocketOption(static_cast<eSOCKET>(i), static_cast<SOCKETINDEX>(socket_index[i]));
    }
}

void SCSocketOptionInfo::_GetInfo(ulong* socket_identify_type, ulong* number_of_sockets, 
                                  ulong socket_index[SOCKET_MAX])
{
    SLOT_BITBOUND_SET_1B(socket_identify_type_, NumberOfBits_SocketIdentifyType);
    SLOT_BITBOUND_SET_1B(number_of_sockets_, NumberOfBits_NumberOfSockets);
    *socket_identify_type = socket_identify_type_;
    *number_of_sockets = number_of_sockets_;

    BOOST_STATIC_ASSERT(SOCKET_1 == 0);
    for(BYTE i = SOCKET_1; i < SOCKET_MAX; ++i) 
    {
        SLOT_BITBOUND_SET(socket_attributes_[i].first, NumberOfBits_SocketIndex);
        socket_index[i] = socket_attributes_[i].first;
    }
}

void SCSocketOptionInfo::SetSocketOption(eSOCKET sock, SOCKETINDEX index)
{
    SLOT_BITBOUND_SET(index, NumberOfBits_SocketIndex);

    SocketOptionParser* const parser = SocketOptionParser::Instance();
    AttrValues* socket_attribute = &socket_attributes_[sock];
    socket_attribute->first = index;
    socket_attribute->second = parser->FindSocketOption(index);
    //socket_attributes_[sock] = make_pair(index, parser->FindSocketOption(index));
}

BOOLEAN SCSocketOptionInfo::CopySocketInfo(const SCSocketOptionInfo& sock_info)
{
    (*this) = sock_info; // copy

    SLOT_BITBOUND_SET_1B(socket_identify_type_, NumberOfBits_SocketIdentifyType);
    SLOT_BITBOUND_SET_1B(number_of_sockets_, NumberOfBits_NumberOfSockets);

    BOOST_STATIC_ASSERT(SOCKET_1 == 0);
    for(BYTE i = SOCKET_1; i < SOCKET_MAX; ++i)
    {
        AttrValues& dest = socket_attributes_[i];
        SLOT_BITBOUND_SET(dest.first, NumberOfBits_SocketIndex);
    }

    return true;
}

#endif //_NA_0_20100727_SOCKET_SYSTEM_CONVERT
