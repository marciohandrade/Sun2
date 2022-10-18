#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "ChaosSystem/ChaosSystemPacketParser.h"
#include "ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"


namespace ChaosSystem
{
    namespace ParserPacket
    {
        void ParsePacketChaosSystem(MSG_BASE* recv_packet)
        {
            MSG_CG_BATTLE_GROUND_PACKET* const recv_msg = static_cast<MSG_CG_BATTLE_GROUND_PACKET*>(recv_packet);
            switch (recv_msg->category_type)
            {
            case kCategoryZone:
                {
                    ParsePacketChaosSystemZone(recv_packet); //zone 관련
                }
                break;
            case kCategoryBattleGround:
                {
                    ParsePacketChaosSystemBattleGround(recv_packet); // 전장
                }
                break;
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
            case kCategoryFreePVP:
                {
                    ParsePacketChaosSystemFreePVP(recv_packet); // 자유전투지역
                }
                break;
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
                //case...
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            case eCHAOSZONE_CATEGORY_GOLDRUSH:
                {
                    ParsePacketChaosSystemGoldRush(recv_packet); // 골드 러쉬
                }
                break;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
            case eCHAOSZONE_CATEGORY_SONNENSCHEIN:
                {
                    ParsePacketChaosSystemSonnenSchein(recv_packet); // 소넨샤인
                }
                break;
#endif  //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            case eCHAOSZONE_CATEGORY_ONEDAYSGUILDMISSION:
                {
                    ParsePacketChaosSystemOneDaysGuildMission(recv_packet); // 길드일일미션
                }
                break;
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
            }
        }

        void ParsePacketChaosSystemForWorld(MSG_BASE* recv_packet)
        {
        }

        // zone 이동 관련 패킷
        void ParsePacketChaosSystemZone(MSG_BASE* recv_packet)
        {
            MSG_CG_CHAOS_ZONE_ZONE* const recv_msg = static_cast<MSG_CG_CHAOS_ZONE_ZONE*>(recv_packet);
            switch(recv_msg->packet_type)
            {
            case CG_CHAOS_ZONE_ZONE_JOIN_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_CHAOS_ZONE_ZONE_JOIN_ACK(recv_packet);
                }
                break;
                //case CG_CHAOS_ZONE_ZONE_LEAVE_ACK:
            }
        }

        // 전장 패킷
        void ParsePacketChaosSystemBattleGround(MSG_BASE* recv_packet)
        {
            MSG_CG_BATTLE_GROUND_PACKET* const recv_msg = static_cast<MSG_CG_BATTLE_GROUND_PACKET*>(recv_packet);
            switch(recv_msg->packet_type)
            {
            case CG_BATTLE_GROUND_RESERVATION_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_RESERVATION_ACK(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_RESERVATION_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_RESERVATION_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_RESERVATION_CANCEL_ACK(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_MATCHING_RESULT_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_MATCHING_RESULT_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_TEAM_INFO_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_TEAM_INFO_CMD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_TEAM_INFO_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_TEAM_INFO_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_STATE_RESURECTION_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STATE_RESURECTION_CMD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_STATE_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_STRONGPOINT_VICTORY_POINT_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_ABNORMAL_EXIT_USER_RETURN_SYN(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_FINISH_RESULT_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_FINISH_RESULT_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_TIME_INFO_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_TIME_INFO_CMD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_GET_OPEN_TIME_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_BATTLE_STATE_NOTIFY_BRD(recv_packet);
                }
                break;
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
            case CG_BATTLE_GROUND_INITIATE_VOTE_NAK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_INITIATE_VOTE_NAK(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_INITIATE_VOTE_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_INITIATE_VOTE_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_VOTE_STATUS_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_VOTE_STATUS_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_VOTE_RESULT_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_VOTE_RESULT_BRD(recv_packet);
                }
                break;
            case CG_BATTLE_GROUND_VANP_PLAYER_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_BATTLE_GROUND_VANP_PLAYER_CMD(recv_packet);
                }
                break;
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
            }
        }// 전장 패킷

        // 자유전투지역 패킷
        void ParsePacketChaosSystemFreePVP(MSG_BASE* recv_packet)
        {
            MSG_CG_FREE_PVP_PACKET* const recv_msg = static_cast<MSG_CG_FREE_PVP_PACKET*>(recv_packet);
            switch(recv_msg->packet_type)
            {
            case CG_FREE_PVP_ROOM_LIST_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_FREE_PVP_ROOM_LIST_ACK(recv_packet);
                }
                break;
            case CG_FREE_PVP_CREATE_ROOM_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_FREE_PVP_CREATE_ROOM_ACK(recv_packet);
                }
                break;
            case CG_FREE_PVP_JOIN_ROOM_COUNT_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_FREE_PVP_JOIN_ROOM_COUNT_CMD(recv_packet);
                }
                break;
            case CG_FREE_PVP_JOIN_ROOM_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_FREE_PVP_JOIN_ROOM_ACK(recv_packet);
                }
                break;
            }
        }// 자유전투지역 패킷

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        // 골드러쉬 패킷
        void ParsePacketChaosSystemGoldRush( MSG_BASE* recv_packet )
        {
            MSG_CG_GOLDRUSH_PACKET* const recv_msg = static_cast<MSG_CG_GOLDRUSH_PACKET*>(recv_packet);
            switch(recv_msg->packet_type)
            {
            case CG_GOLDRUSH_RESERVATION_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_RESERVATION_ACK(recv_packet);
                }
                break;
            case CG_GOLDRUSH_RESERVATION_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_RESERVATION_BRD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_RESERVATION_CANCEL_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_RESERVATION_CANCEL_ACK(recv_packet);
                }
                break;            
            case CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_ABNORMAL_EXIT_USER_RETURN_SYN(recv_packet);
                }
                break;
            case CG_GOLDRUSH_INITIATE_VOTE_NAK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_INITIATE_VOTE_NAK(recv_packet);
                }
                break;
            case CG_GOLDRUSH_INITIATE_VOTE_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_INITIATE_VOTE_BRD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_VOTE_STATUS_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_VOTE_STATUS_BRD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_VOTE_RESULT_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_VOTE_RESULT_BRD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_VANP_PLAYER_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_VANP_PLAYER_CMD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_TEAM_INFO_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_TEAM_INFO_CMD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_TEAM_INFO_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_TEAM_INFO_BRD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_TOTAL_TEAM_POINT_BRD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_STATE_RESURECTION_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_STATE_RESURECTION_CMD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_FINISH_RESULT_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_FINISH_RESULT_BRD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_TIME_INFO_CMD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_TIME_INFO_CMD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_BATTLE_STATE_NOTIFY_BRD(recv_packet);
                }
                break;
            case CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_GOLDRUSH_MONSTERKILL_NOTIFY_BRD(recv_packet);
                }
                break;
            }
        } // 골드 러쉬 패킷
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        
        // 소넨샤인 패킷
#ifdef _NA_008334_20150608_SONNENSCHEIN
        void ParsePacketChaosSystemSonnenSchein(MSG_BASE* recv_packet)
        {
            MSG_CG_SONNENSCHEIN_PACKET* const recv_msg = static_cast<MSG_CG_SONNENSCHEIN_PACKET*>(recv_packet);
            switch(recv_msg->packet_type)
            {
            case CG_SONNENSCHEIN_CREATE_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_SONNENSCHEIN_CREATE_ACK(recv_packet);
                }
                break;
            case CG_SONNENSCHEIN_ENTER_CHECK_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_SONNENSCHEIN_ENTER_CHECK_ACK(recv_packet);
                }
                break;
            case CG_SONNENSCHEIN_BEGIN_STAMP_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_SONNENSCHEIN_BEGIN_STAMP_BRD(recv_packet);
                }
                break;
            case CG_SONNENSCHEIN_CANCEL_STAMP_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_SONNENSCHEIN_CANCEL_STAMP_BRD(recv_packet);
                }
                break;
            case CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_SONNENSCHEIN_ENTERPLAYER_LIST_ACK(recv_packet);
                }
                break;
            }
        }
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        void ParsePacketChaosSystemOneDaysGuildMission(MSG_BASE* recv_packet)
        {
            MSG_CG_ONEDAYSGUILDMISSION_PACKET* const recv_msg = static_cast<MSG_CG_ONEDAYSGUILDMISSION_PACKET*>(recv_packet);
            switch(recv_msg->packet_type)
            {
            case CG_ONEDAYSGUILDMISSION_CREATE_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_ONEDAYSGUILDMISSION_CREATE_ACK(recv_packet);
                }
                break;
            case CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_ONEDAYSGUILDMISSION_ENTER_CHECK_ACK(recv_packet);
                }
                break;
            case CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_ONEDAYSGUILDMISSION_BEGIN_STAMP_BRD(recv_packet);
                }
                break;
            case CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_ONEDAYSGUILDMISSION_CANCEL_STAMP_BRD(recv_packet);
                }
                break;
            case CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK:
                {
                    ChaosSystem::PacketFunction::RECV_CG_ONEDAYSGUILDMISSION_ENTERPLAYER_LIST_ACK(recv_packet);
                }
                break;
            case CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD:
                {
                    ChaosSystem::PacketFunction::RECV_CG_ONEDAYSGUILDMISSION_TIME_STATE_INFO_BRD(recv_packet);
                }
                break;
            }
        }
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
    };
};
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND