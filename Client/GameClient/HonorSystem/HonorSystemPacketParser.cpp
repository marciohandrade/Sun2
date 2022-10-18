#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "../ProgramCommon/HonorSystem/HonorSystemPacket.h"
#include "HonorSystem/HonorSystem.h"
#include "HonorSystem/HonorSystemPacketParser.h"
#include "HonorSystem/HonorSystemData.h"

namespace HonorSystem{

namespace HonorSystemPacketParser
{
    void ParsePacketTitleSystem(MSG_CG_HONORSYSTEM_PACKET* recv_packet)
    {
        CG_HONORSYSTEM_TITLE_PACKET* title_packet = static_cast<CG_HONORSYSTEM_TITLE_PACKET*>(recv_packet);
        switch (title_packet->packet_type2)
        {
        case CG_HONORSYSTEM_TITLE_CHANGE_BRD:
            {
                PACKET_CG_HONORSYSTEM_TITLE_CHANGE_BRD* title_change_brd = static_cast<PACKET_CG_HONORSYSTEM_TITLE_CHANGE_BRD*>(title_packet);
                HonorSystem::HonorSystemFunction::SetTitleName(title_change_brd->player_key_, title_change_brd->honor_title_index_);
                HonorSystem::HonorSystemFunction::OnChangeTitle(title_change_brd->honor_title_index_);
            }
            break;
        case CG_HONORSYSTEM_TITLE_ADD_ACK:
            {
                PACKET_CG_HONORSYSTEM_TITLE_ADD_ACK* title_add_ack = static_cast<PACKET_CG_HONORSYSTEM_TITLE_ADD_ACK*>(title_packet);
                HonorSystem::GetHonorSystemData().AddTitleCode(title_add_ack->honor_title_index_);
                HonorSystem::HonorSystemFunction::OnAddTitle();
            }
            break;
        case CG_HONORSYSTEM_TITLE_REMOVE_ACK:
            {
                PACKET_CG_HONORSYSTEM_TITLE_REMOVE_ACK* title_remove_ack = static_cast<PACKET_CG_HONORSYSTEM_TITLE_REMOVE_ACK*>(title_packet);
                HonorSystem::GetHonorSystemData().RemoveTitleCode(title_remove_ack->honor_title_index_);
                HonorSystem::HonorSystemFunction::OnRemoveTitle();
            }
            break;
        case CG_HONORSYSTEM_TITLE_LIST_ACK:
            {
                PACKET_CG_HONORSYSTEM_TITLE_LIST_ACK* title_list_ack = static_cast<PACKET_CG_HONORSYSTEM_TITLE_LIST_ACK*>(title_packet);
                HonorSystem::GetHonorSystemData().RecvTitleList(title_list_ack->titles_);
            }
            break;

        case CG_HONORSYSTEM_TITLE_NAK:
            {
                // .. errorcode 贸府
            }
            break;
        default:
            {
            }
        }
    }

    void ParsePacketFameReputeSystem(MSG_CG_HONORSYSTEM_PACKET* recv_packet)
    {
        CG_HONORSYSTEM_FAMEREPUTE_PACKET* fame_repute_packet = static_cast<CG_HONORSYSTEM_FAMEREPUTE_PACKET*>(recv_packet);
        switch (fame_repute_packet->packet_type2)
        {
        case CG_HONORSYSTEM_FAMEREPUTE_POINT_TABLE_ACK:
            {
                PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_TABLE_ACK* honor_point_ack = static_cast<PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_TABLE_ACK*>(fame_repute_packet);
                HonorSystem::GetHonorSystemData().RecvFameReputePointTable(honor_point_ack->honor_point_table_);
            }
            break;
        case CG_HONORSYSTEM_FAMEREPUTE_POINT_CHANGE_CMD:
            {
                PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_CHANGE_CMD* point_change_cmd = static_cast<PACKET_CG_HONORSYSTEM_FAMEREPUTE_POINT_CHANGE_CMD*>(fame_repute_packet);
                if (point_change_cmd->type_ == HONOR_POINT_INFO::FAME)
                {
                    HonorSystem::HonorSystemFunction::OnChangeFamePoint(point_change_cmd->code_, point_change_cmd->point_);
                    HonorSystem::GetHonorSystemData().ChangeFamePoint(point_change_cmd->code_, point_change_cmd->point_);
                }
                else if (point_change_cmd->type_ == HONOR_POINT_INFO::REPUTE)
                {
                    HonorSystem::HonorSystemFunction::OnChangeReputePoint(point_change_cmd->code_, point_change_cmd->point_);
                    HonorSystem::GetHonorSystemData().ChangeReputePoint(point_change_cmd->code_, point_change_cmd->point_);
                }
            }
            break;

        case CG_HONORSYSTEM_FAMEREPUTE_NAK:
            {
                // .. errorcode 贸府
            }
            break;
        default:
            {
            }
        }
    }

    void ParsePacketHonorSystem(MSG_BASE* recv_packet)
    {
        MSG_CG_HONORSYSTEM_PACKET* honor_system_packet = static_cast<MSG_CG_HONORSYSTEM_PACKET*>(recv_packet);
        switch (honor_system_packet->packet_type)
        {
        case CG_HONORSYSTEM_TITLE:
            {
                ParsePacketTitleSystem(honor_system_packet);
            }
            break;
        case CG_HONORSYSTEM_FAMEREPUTE:
            {
                ParsePacketFameReputeSystem(honor_system_packet);
            }
            break;
        default:
            {
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////


};//HonorSystemPacketParser

};//namespace HonorSystem

#endif //_NA_003027_20111013_HONOR_SYSTEM
