#include "SunClientPrecompiledHeader.h"
#include "PacketRequestManager.h"
#include "GlobalFunc.h"
#include "Hero.h"

//------------------------------------------------------------------------------
PacketRequestManager::PacketRequestManager()
{
    request_info_vector().clear();
} 
//------------------------------------------------------------------------------
PacketRequestManager::~PacketRequestManager()
{
    Release(); 
}
//------------------------------------------------------------------------------
void PacketRequestManager::Release()
{
    request_info_vector().clear();
}
//------------------------------------------------------------------------------
void PacketRequestManager::PushPacket(MSG_BASE* syn_packet, 
                                      MSG_BASE* ack_packet, 
                                      MSG_BASE* nak_packet, 
                                      int syn_packet_size, 
                                      DWORD wait_millisecond,
                                      bool is_request)
{
    PacketRequestInfo_Shared_Ptr info(new PacketRequestInfo(syn_packet, 
                                                            ack_packet, 
                                                            nak_packet,
                                                            syn_packet_size, 
                                                            clock_function::GetTickCount(), 
                                                            wait_millisecond,
                                                            is_request));
                                     

    request_info_vector().push_back(info);
}
//------------------------------------------------------------------------------
bool PacketRequestManager::is_sent(BYTE category, BYTE protocol)
{
    std::vector<PacketRequestInfo_Shared_Ptr>::iterator begin = request_info_vector().begin();
    std::vector<PacketRequestInfo_Shared_Ptr>::iterator end = request_info_vector().end();
    for (; begin != end; ++begin)
    {
        PacketRequestInfo* info = (*begin).get();

        if ((info->syn_packet()->m_byCategory == category) &&
            (info->syn_packet()->m_byProtocol == protocol))
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------
void PacketRequestManager::CheckRequestPacket()
{
    if (request_info_vector().size() <= 0)
    {
        return;
    }

    std::vector<PacketRequestInfo_Shared_Ptr>::iterator begin = request_info_vector().begin();
    for (; begin != request_info_vector().end();)
    {
        bool is_send = false;
        PacketRequestInfo* info = (*begin).get();

        if ((clock_function::GetTickCount() - info->send_millisecond()) > info->wait_millisecond())
        {
            if (info->is_request())
            {
                GlobalFunc::SendPacketToGameServer(info->syn_packet(), info->syn_packet_size());
                info->set_send_millisecond(clock_function::GetTickCount());
            }

            //! 한번만 더 보내고 지운다.
            is_send = true;
        }

        if (is_send)
        {
            begin = request_info_vector().erase(begin);
        }
        else
        {
            ++begin;
        }
    }
}
//------------------------------------------------------------------------------
void PacketRequestManager::NetworkProc(MSG_BASE * packet)
{
    if (request_info_vector().size() <= 0)
    {
        return;
    }

    std::vector<PacketRequestInfo_Shared_Ptr>::iterator begin = request_info_vector().begin();
    for (; begin != request_info_vector().end();)
    {
        bool is_same = false;
        PacketRequestInfo* info = (*begin).get();

        if (((info->ack_packet()->m_byCategory == packet->m_byCategory) && (info->ack_packet()->m_byProtocol == packet->m_byProtocol)) || 
            ((info->nak_packet()->m_byCategory == packet->m_byCategory) && (info->nak_packet()->m_byProtocol == packet->m_byProtocol)) )
        {
            if ((packet->m_byCategory == CG_QUEST) && (packet->m_byProtocol == CG_QUEST_COMPLETE_ACK))
            {
                is_same = ComparePacket_CG_QUEST_COMPLETE_ACK(info, static_cast<MSG_CG_QUEST_COMPLETE_ACK*>(packet));
            }
            else if ((packet->m_byCategory == CG_STATUS) && (packet->m_byProtocol == CG_STATUS_STAT_APPLY_ACK))
            {
                is_same = ComparePacket_CG_STATUS_STAT_APPLY_ACK(static_cast<MSG_CG_STATUS_STAT_APPLY_ACK*>(packet));
            }
            else if ((packet->m_byCategory == CG_SYNC) && (packet->m_byProtocol == CG_SYNC_WINGS_CONTROL_CMD))
            {
                is_same = ComparePacket_CG_SYNC_WINGS_CONTROL_CMD(static_cast<MSG_CG_SYNC_WINGS_CONTROL_CMD*>(packet));
            }
            else
            {
                is_same = true;
            }
        }

        if (is_same)
        {
            begin = request_info_vector().erase(begin);
        }
        else
        {
            ++begin;
        }
    }
}
//------------------------------------------------------------------------------
bool PacketRequestManager::ComparePacket_CG_QUEST_COMPLETE_ACK(PacketRequestInfo* info, MSG_CG_QUEST_COMPLETE_ACK* packet)
{
    return (((MSG_CG_QUEST_COMPLETE_ACK*)(info->ack_packet()))->m_QuestCode == packet->m_QuestCode);
}
//------------------------------------------------------------------------------ 
bool PacketRequestManager::ComparePacket_CG_SYNC_WINGS_CONTROL_CMD(MSG_CG_SYNC_WINGS_CONTROL_CMD* packet)
{
    if (packet->header.result != WingControl::eRet_Success)
    {
        //! 실패는 본인한데만 오기때문에 패킷을 받았다는 처리를 한다.
        return true;
    }
    if ((packet->data.data_type == WingControlData::eDataType_Actor) && 
        (g_pHero != NULL) &&
        (g_pHero->GetObjectKey() == packet->data.actor.requester))
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool PacketRequestManager::ComparePacket_CG_STATUS_STAT_APPLY_ACK(MSG_CG_STATUS_STAT_APPLY_ACK* packet)
{
    if (g_pHero == NULL)
    {
        return true;
    }
    return (packet->object_key == g_pHero->GetObjectKey());
}
