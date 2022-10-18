#include "stdafx.h"
#include "./PacketStruct_CG.h"

// (CHANGES) (f100607.2L) split an implementation part from a declaration part

//==================================================================================================

uint16_t MSG_CG_CONNECTION_ENTERSERVER_ACK::ConvertForSend(
    CLIENT_CHARACTER_PART rpCharacterList[MAX_CHARACTER_LIST_NUM])
{
    // CHANGES: f110608.5L
    const size_t kMinSize = sizeof(*this) - sizeof(m_pBuffer);

    BOOST_STATIC_ASSERT(sizeof(MSG_CG_CONNECTION_ENTERSERVER_ACK) < USHRT_MAX);
    if (FlowControl::FCAssert(m_byCount <= MAX_CHARACTER_LIST_NUM) == false) {
        return static_cast<uint16_t>(kMinSize);
    };

    //write
    ArchiveOfStream archive(m_pBuffer, sizeof(m_pBuffer));
    CLIENT_CHARACTER_PART* part_it = &rpCharacterList[0];
    const CLIENT_CHARACTER_PART* const part_end = &rpCharacterList[m_byCount];
    for (; part_it != part_end; ++part_it)
    {
        part_it->Output(archive);
    };
    //
    const WORD tail_mark = kTailMark;
    archive.Write(&tail_mark, sizeof(tail_mark));
    //
    size_t message_size = kMinSize + archive.GetSize();
    return static_cast<uint16_t>(message_size);
}

bool MSG_CG_CONNECTION_ENTERSERVER_ACK::ConvertForRecv(
    CLIENT_CHARACTER_PART rpCharacterList[MAX_CHARACTER_LIST_NUM])
{
    ArchiveOfStream archive(m_pBuffer, sizeof(m_pBuffer));
    //read
    CLIENT_CHARACTER_PART* part_it = &rpCharacterList[0];
    const CLIENT_CHARACTER_PART* const part_end = &rpCharacterList[m_byCount];
    for (; part_it != part_end; ++part_it)
    {
        part_it->Input(archive);
    };

    WORD tail_mark = 0;
    archive.Read(&tail_mark, sizeof(tail_mark));
    if (FlowControl::FCAssert(tail_mark == kTailMark) == false) {
        return false;
    };
    return true;
}


int MSG_CG_CHARINFO_CREATE_ACK::ConvertForSend(CLIENT_CHARACTER_PART& rCharInfo)
{
    ArchiveOfStream archive(m_pBuffer, sizeof(CLIENT_CHARACTER_PART));
    //write
    rCharInfo.Output(archive);
    return sizeof(MSG_BASE_FORWARD)+archive.GetSize();
}
void MSG_CG_CHARINFO_CREATE_ACK::ConvertForRecv(CLIENT_CHARACTER_PART& rCharInfo)
{
    ArchiveOfStream archive(m_pBuffer, sizeof(CLIENT_CHARACTER_PART));
    //read
    rCharInfo.Input(archive);
}


// <임시버퍼로 사용할 타입>
//VENDOR_LIST_TOTAL_INFO& rVendorListInfo
uint16_t MSG_CG_VENDOR_REQUIRE_PAGE_ACK::ConvertforSend(const VENDOR_LIST_TOTAL_INFO& total_info)
{
    ArchiveOfStream archive(m_pBuffer, sizeof(m_pBuffer));
    //write
    total_info.Output(archive);

    const size_t kMinSize = sizeof(*this) - sizeof(m_pBuffer);
    return static_cast<uint16_t>(kMinSize + archive.GetSize());
}

void MSG_CG_VENDOR_REQUIRE_PAGE_ACK::ConvertForRecv(VENDOR_LIST_TOTAL_INFO& total_info) const
{
    ArchiveOfStream archive(const_cast<uint8_t*>(m_pBuffer), sizeof(m_pBuffer));
    //read
    total_info.Input(archive);
}

//==================================================================================================

void MSG_CG_ITEM_RIDER_CONTROL_SYN::Clear()
{
    ZeroMemory(this, sizeof(*this));
    new (this) MSG_CG_ITEM_RIDER_CONTROL_SYN;
}

void MSG_CG_ITEM_RIDER_CONTROL_BRD::Clear()
{
    ZeroMemory(this, sizeof(*this));
    new (this) MSG_CG_ITEM_RIDER_CONTROL_BRD;
}

void MSG_CG_ITEM_RIDER_CONTROL_CMD::Clear()
{
    ZeroMemory(this, sizeof(*this));
    new (this) MSG_CG_ITEM_RIDER_CONTROL_CMD;
}

void MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN::Clear()
{
    ZeroMemory(this, sizeof(*this));
    new (this) MSG_CG_ITEM_RIDER_CONTROL_PARTS_SYN;
}

void MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD::Clear()
{
    ZeroMemory(this, sizeof(*this));
    new (this) MSG_CG_ITEM_RIDER_CONTROL_PARTS_BRD;
}

void MSG_CG_ITEM_RIDER_CONTROL_PARTS_CMD::Clear()
{
    ZeroMemory(this, sizeof(*this));
    new (this) MSG_CG_ITEM_RIDER_CONTROL_PARTS_CMD;
}

void MSG_CG_ITEM_ETHERIA_CONTROL_SYN::Clear()
{
    ZeroMemory(this, sizeof(*this));
    new (this) MSG_CG_ITEM_ETHERIA_CONTROL_SYN;
}

void MSG_CG_ITEM_ETHERIA_CONTROL_ACK::Clear()
{
    ZeroMemory(this, sizeof(*this));
    new (this) MSG_CG_ITEM_ETHERIA_CONTROL_ACK;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
// the section for a packet randomizer
//
#pragma warning(push)
#pragma warning(disable : 4201 4307)

void MSG_CG_CONNECTION_ENTERSERVER_SYN::Encode()
{
    _Encode_T1();
    _Encode_T2();
    _Encode_T3();
}

void MSG_CG_CONNECTION_ENTERSERVER_SYN::Decode()
{
    _Decode_T1();
    _Decode_T2();
    _Decode_T3();
}

void MSG_CG_CONNECTION_ENTERSERVER_SYN::_Encode_T1()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (m_szSerialKey)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T4.h"
#   include "FI_RndProtocol/FI_ENCODE32_T5.h"
}

void MSG_CG_CONNECTION_ENTERSERVER_SYN::_Encode_T2()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_dwAuthID)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_04_LIST_T3.h"
#   include "FI_RndProtocol/FI_ENCODE04_T2.h"
}

void MSG_CG_CONNECTION_ENTERSERVER_SYN::_Encode_T3()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_dwAuthID)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T1.h"
#   include "FI_RndProtocol/FI_ENCODE32_T3.h"
}

void MSG_CG_CONNECTION_ENTERSERVER_SYN::_Decode_T1()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_dwAuthID)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T1.h"
#   include "FI_RndProtocol/FI_DECODE32_T3.h"
}
void MSG_CG_CONNECTION_ENTERSERVER_SYN::_Decode_T2()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_dwAuthID)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_04_LIST_T3.h"
#   include "FI_RndProtocol/FI_DECODE04_T2.h"
}
void MSG_CG_CONNECTION_ENTERSERVER_SYN::_Decode_T3()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (m_szSerialKey)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T4.h"
#   include "FI_RndProtocol/FI_DECODE32_T5.h"
}


void MSG_CG_CHARINFO_CHAR_ITEM_CMD::Encode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_CharacterInfo)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T5.h"
#   include "FI_RndProtocol/FI_ENCODE32_T4.h"
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_CharacterInfo)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T1.h"
#   include "FI_RndProtocol/FI_ENCODE08_T3.h"
}

void MSG_CG_CHARINFO_CHAR_ITEM_CMD::Decode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_CharacterInfo)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T1.h"
#   include "FI_RndProtocol/FI_DECODE08_T3.h"
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_CharacterInfo)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T5.h"
#   include "FI_RndProtocol/FI_DECODE32_T4.h"
}


void MSG_CG_SYNC_PLAYER_ENTER_ACK::Encode()
{
    _Encode_01();
    _Encode_02();
}

void MSG_CG_SYNC_PLAYER_ENTER_ACK::Decode()
{
    _Decode_01();
    _Decode_02();
}


void MSG_CG_SYNC_PLAYER_ENTER_ACK::_Encode_01()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_fPos[0])
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T4.h"
#   include "FI_RndProtocol/FI_ENCODE08_T5.h"
}

void MSG_CG_SYNC_PLAYER_ENTER_ACK::_Encode_02()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_fPos[1])
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T3.h"
#   include "FI_RndProtocol/FI_ENCODE08_T4.h"
}

void MSG_CG_SYNC_PLAYER_ENTER_ACK::_Decode_01()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_fPos[1])
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T3.h"
#   include "FI_RndProtocol/FI_DECODE08_T4.h"
}

void MSG_CG_SYNC_PLAYER_ENTER_ACK::_Decode_02()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_fPos[0])
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T4.h"
#   include "FI_RndProtocol/FI_DECODE08_T5.h"
}


void MSG_CG_SYNC_MOVE_SYN::Encode()
{
    _Encode_T1();
    _Encode_T2();
}

void MSG_CG_SYNC_MOVE_SYN::Decode()
{
    _Decode_T1();
    _Decode_T2();
}

void MSG_CG_SYNC_MOVE_SYN::_Encode_T1()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&wvDestPos)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T1.h"
#   include "FI_RndProtocol/FI_ENCODE08_T1.h"
}

void MSG_CG_SYNC_MOVE_SYN::_Encode_T2()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&wvCurPos)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T5.h"
#   include "FI_RndProtocol/FI_ENCODE16_T5.h"
}

void MSG_CG_SYNC_MOVE_SYN::_Decode_T1()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&wvCurPos)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T5.h"
#   include "FI_RndProtocol/FI_DECODE16_T5.h"
}
void MSG_CG_SYNC_MOVE_SYN::_Decode_T2()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&wvDestPos)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T1.h"
#   include "FI_RndProtocol/FI_DECODE08_T1.h"
}


void MSG_CG_SYNC_KBMOVE_SYN::Encode()
{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE    (&wvCurPos)
    #define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T2.h"
    #include "FI_RndProtocol/FI_ENCODE16_T4.h"
    };
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE    (&m_wAngle)
    #define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T5.h"
    #include "FI_RndProtocol/FI_ENCODE16_T3.h"
    };
#else
#   define BINGLEBANGLE_POINTER_VARIABLE    (((BYTE*)((&wvCurPos)+1))-3)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T2.h"
#   include "FI_RndProtocol/FI_ENCODE08_T4.h"
#endif
}

void MSG_CG_SYNC_KBMOVE_SYN::Decode()
{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE    (&m_wAngle)
    #define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T5.h"
    #include "FI_RndProtocol/FI_DECODE16_T3.h"
    };
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE    (&wvCurPos)
    #define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T2.h"
    #include "FI_RndProtocol/FI_DECODE16_T4.h"
    };
#else
#   define BINGLEBANGLE_POINTER_VARIABLE    (((BYTE*)((&wvCurPos)+1))-3)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T2.h"
#   include "FI_RndProtocol/FI_DECODE08_T4.h"
#endif
}

#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
void MSG_CG_SYNC_FAST_KBMOVE_SYN::Encode()
{
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE    (&m_wvCurPos)
    #define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T4.h"
    #include "FI_RndProtocol/FI_ENCODE16_T2.h"
    };
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE    (&m_wAngle)
    #define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T1.h"
    #include "FI_RndProtocol/FI_ENCODE16_T2.h"
    };
}

void MSG_CG_SYNC_FAST_KBMOVE_SYN::Decode()
{
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE    (&m_wAngle)
    #define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T1.h"
    #include "FI_RndProtocol/FI_DECODE16_T2.h"
    };
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE    (&m_wvCurPos)
    #define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_16_LIST_T4.h"
    #include "FI_RndProtocol/FI_DECODE16_T2.h"
    };
}
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT


void MSG_CG_BATTLE_PLAYER_ATTACK_SYN::Encode()
{
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE   (&dwClientSerial)
    #define BINGLEBANGLE_RND_LIST_FILE      "FI_RndProtocol/FI_32_LIST_T4.h"
    #include "FI_RndProtocol/FI_ENCODE32_T4.h"
    };
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE   (&dwTargetKey)
    #define BINGLEBANGLE_RND_LIST_FILE      "FI_RndProtocol/FI_16_LIST_T4.h"
    #include "FI_RndProtocol/FI_ENCODE16_T4.h"
    };
#endif
}

void MSG_CG_BATTLE_PLAYER_ATTACK_SYN::Decode()
{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE   (&dwTargetKey)
    #define BINGLEBANGLE_RND_LIST_FILE      "FI_RndProtocol/FI_16_LIST_T4.h"
    #include "FI_RndProtocol/FI_DECODE16_T4.h"
    };
#endif
    ;{
    #define BINGLEBANGLE_POINTER_VARIABLE   (&dwClientSerial)
    #define BINGLEBANGLE_RND_LIST_FILE      "FI_RndProtocol/FI_32_LIST_T4.h"
    #include "FI_RndProtocol/FI_DECODE32_T4.h"
    };
}

void MSG_CG_SKILL_ACTION_SYN::Encode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_dwClientSerial)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T3.h"
#   include "FI_RndProtocol/FI_ENCODE32_T2.h"
}

void MSG_CG_SKILL_ACTION_SYN::Decode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_dwClientSerial)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T3.h"
#   include "FI_RndProtocol/FI_DECODE32_T2.h"
}

void MSG_CG_ZONE_LOBBY_CREATE_SYN::Encode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_MapCode)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T2.h"
#   include "FI_RndProtocol/FI_ENCODE32_T5.h"
}

void MSG_CG_ZONE_LOBBY_CREATE_SYN::Decode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_MapCode)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T2.h"
#   include "FI_RndProtocol/FI_DECODE32_T5.h"
}

void MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN::Encode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_LinkHuntingKey)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T1.h"
#   include "FI_RndProtocol/FI_ENCODE32_T1.h"
}

void MSG_CG_ZONE_CHUNTINGLOBBY_CREATE_SYN::Decode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_LinkHuntingKey)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_32_LIST_T1.h"
#   include "FI_RndProtocol/FI_DECODE32_T1.h"
}

void MSG_CG_ITEM_SELL_SYN::Encode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (fixSIZE)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T3.h"
#   include "FI_RndProtocol/FI_ENCODE08_T1.h"
}

void MSG_CG_ITEM_SELL_SYN::Decode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (fixSIZE)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T3.h"
#   include "FI_RndProtocol/FI_DECODE08_T1.h"
}


void MSG_CG_TRIGGER_PORTAL_ACK::Encode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_MapCode)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T3.h"
#   include "FI_RndProtocol/FI_ENCODE08_T5.h"
}

void MSG_CG_TRIGGER_PORTAL_ACK::Decode()
{
#   define BINGLEBANGLE_POINTER_VARIABLE    (&m_MapCode)
#   define BINGLEBANGLE_RND_LIST_FILE       "FI_RndProtocol/FI_08_LIST_T3.h"
#   include "FI_RndProtocol/FI_DECODE08_T5.h"
}

#pragma warning(pop)
