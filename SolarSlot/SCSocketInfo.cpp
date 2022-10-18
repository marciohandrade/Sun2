#include "StdAfx.h"
#include ".\scsocketinfo.h"
#include <SocketOptionParser.h>
#include <BitStream.hpp>


//==================================================================================================
//

#ifdef _NA_0_20100727_SOCKET_SYSTEM_CONVERT
#else // 삭제

__forceinline void
SCSocketInfo::_SetSocketAttr(SCSocketInfo::AttrValues* OUT dest, eSOCKET_LEVEL level,
                             const sSOCKET_OPTION* socket_option)
{
    if(socket_option == 0) {
        ZeroMemory(dest, sizeof(*dest));
        return;
    }

    dest->socket_level_ = level;
    if(socket_option->m_AttrIndex < _countof(g_ITEM_OPTION))
        dest->attr_type_ = g_ITEM_OPTION[socket_option->m_AttrIndex];
    dest->attr_index_ = socket_option->m_AttrIndex;
    dest->attr_value_ = socket_option->m_iValue[level];
    dest->socket_option_ = socket_option;
}

void
SCSocketInfo::_SetInfo(ulong number_of_sockets, const ulong sockets[SOCKET_MAX],
                       const BASE_ITEMINFO* item_info)
{
    __UNUSED(item_info);
    Clear();
    SLOT_BITBOUND_SET(number_of_sockets, NumberOfBits_NumberOfSockets);
    number_of_sockets_ = BYTE(number_of_sockets);

    // need check boundary?
    SocketOptionParser* const pSocketOptionParser = SocketOptionParser::Instance();

    int attr_index = 0; // no-affect value, avoid warning
    eSOCKET_LEVEL sock_level = SOCKETLV_LOW; // no-affect value, avoid warning

    BOOST_STATIC_ASSERT(SOCKET_1 == 0);
    for(BYTE i = 0; i < number_of_sockets_; ++i) {
        int attr_index_org = sockets[i];

        if(BinaryToSocketIndex(attr_index_org, attr_index, sock_level) == 0)
            continue;

        const sSOCKET_OPTION* socket_option = pSocketOptionParser->GetSocketOption(attr_index);
        _SetSocketAttr(&socket_attributes_[i], sock_level, socket_option);
    }
}

void
SCSocketInfo::_GetInfo(ulong* number_of_sockets, ulong sockets[SOCKET_MAX])
{
    SLOT_BITBOUND_SET_1B(number_of_sockets_, NumberOfBits_NumberOfSockets);
    *number_of_sockets = number_of_sockets_;

    for(BYTE i = 0; i < number_of_sockets_; ++i) {
        int binary_value = SocketIndexToBinary(&socket_attributes_[i]);
        sockets[i] = binary_value;
    }
}


//--------------------------------
// VERSION 1 : 6bit
// 0    1   ~   31
// 32   33  ~   63

// VERSION 2 : 7bit
// 0    1   ~   63
// 64   65  ~   127
// 64 : Value Is Not Used
// Conversion Routine
// 0~31 -> just as it is
// 32~63 -> + 32 -> 64 ~ 95

//VERSION 3 : 8bit
// 0    1   ~   127
// 128  129 ~   255

#define ADD_VALUE 127//63//31
//--------------------------------

int
SCSocketInfo::SocketIndexToBinary(const AttrValues* node) const
{
    // (NOTE) 이런 변환이 난이도를 대폭 증가시킬 듯...
    // 일단 근본적인 해결 방안은 re-encoding... by type-based

    int attr_index = node->attr_index_;
    if(node->socket_level_ == SOCKETLV_LOW)
        return attr_index;
    else
        return attr_index + ADD_VALUE; //<- (TODO) 127인 이유를 찾아볼 것.
}

BOOLEAN
SCSocketInfo::BinaryToSocketIndex(int IN binary_index, int& OUT attr_index, eSOCKET_LEVEL& OUT level)
{
    if(binary_index == 0)
        return false;
    if(binary_index <= ADD_VALUE) {
        level = SOCKETLV_LOW;
        attr_index = binary_index;
    }
    else {
        //__NA_0_SOCKET_OPTION_BUG_RECOVERY
        // (WAVERIX) (NOTE) ...이해가 안되는 수치 및 보정이다. 이렇게 되는 원인 파악할 것.
        if(binary_index >= 192)
            binary_index -= 64;
        attr_index = binary_index - ADD_VALUE;
        level = SOCKETLV_HIGH;
    }

    return true;
}

void
SCSocketInfo::SetSocketAttr(eSOCKET sock, eSOCKET_LEVEL level, const sSOCKET_OPTION* socket_option)
{
    if(!FlowControl::FCAssert(sock < number_of_sockets_ && number_of_sockets_ <= SOCKET_MAX))
        return;

    _SetSocketAttr(&socket_attributes_[sock], level, socket_option);
    // TODO(WAVERIX) (WARNING) 옵션 제거시 socket level에 값을 설정하고 있었다... 없어도 되는지 여부 확인할 것.
    //m_SocketAttribute[sock].m_SocketLevel   = level;
}


BOOLEAN
SCSocketInfo::CopySocketInfo(const SCSocketInfo& sock_info)
{
    //Clear();
    (*this) = sock_info; // copy

    SLOT_BITBOUND_SET(number_of_sockets_, SCSocketInfo::NumberOfBits_NumberOfSockets);
    for(int i = 0; i < _countof(socket_attributes_); ++i) {
        SCSocketInfo::AttrValues& dest = socket_attributes_[i];
        SLOT_BITBOUND_SET(dest.attr_index_, SCSocketInfo::NumberOfBits_SocketIndex);
    }

    //nsSlot::SocketInfoSerializeStruct block = { 0, };
    //block.CopyFrom(sock_info);
    //block.ModifyVerification();
    //block.CopyTo(this);

    return true;
}

#endif //_NA_0_20100727_SOCKET_SYSTEM_CONVERT

//==================================================================================================
//==================================================================================================
//==================================================================================================
//

