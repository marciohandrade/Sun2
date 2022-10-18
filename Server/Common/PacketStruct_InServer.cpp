#include "stdafx.h"

//==================================================================================================
// @history
//  11.04.28, waverix, this file added to split implements from declaration sector

#include "PacketStruct_AD.h"

//==================================================================================================

uint16_t MSG_AD_CHARINFO_CREATE_ACK::ConvertForSend(SERVER_CHARACTER_PART& server_char_part)
{
    ArchiveOfStream archive(m_pBuffer, sizeof(m_pBuffer));
    BOOST_STATIC_ASSERT(sizeof(m_pBuffer) == sizeof(server_char_part));
    //write
    server_char_part.Output(archive);
    //
    const size_t kMinSize = sizeof(*this) - sizeof(m_pBuffer);
    return static_cast<uint16_t>(kMinSize + archive.GetSize());
}

void MSG_AD_CHARINFO_CREATE_ACK::ConvertForRecv(SERVER_CHARACTER_PART& server_char_part)
{
    ArchiveOfStream archive(m_pBuffer, sizeof(m_pBuffer));
    //read
    server_char_part.Input(archive);
}


uint16_t MSG_AD_CHARINFO_CHARLISTREQ_ACK::ConvertForSend(
    SERVER_CHARACTER_PART char_part_list[MAX_CHARACTER_LIST_NUM])
{
    ArchiveOfStream archive(m_pBuffer, sizeof(m_pBuffer));
    //write
    archive << m_byCount;
    for (BYTE i = 0; i < m_byCount; ++i) {
        char_part_list[i].Output(archive);
    };

    const size_t kMinSize = sizeof(*this) - sizeof(m_pBuffer);
    return static_cast<uint16_t>(kMinSize + archive.GetSize());
}

void MSG_AD_CHARINFO_CHARLISTREQ_ACK::ConvertForRecv(
    SERVER_CHARACTER_PART char_part_list[MAX_CHARACTER_LIST_NUM])
{
    ArchiveOfStream archive(m_pBuffer, sizeof(m_pBuffer));
    //read
    archive >> m_byCount;
    for (BYTE i = 0; i < m_byCount; ++i) {
        char_part_list[i].Input(archive);
    };
}


