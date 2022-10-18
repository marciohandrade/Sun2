#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneSystem/AgentChaosZoneManager.h>
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>
#include <ChaosZoneSystem/BattleGroundManager.h>
#include <ChaosZoneSystem/FreePVPManager.h>

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include <ChaosZoneSystem/SonnenScheinManager.h>
#endif //_NA_008334_20150608_SONNENSCHEIN

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include <ChaosZoneSystem/OneDaysGuildMissionManager.h>
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

//--------------------------------------------------------------------------------------------------
// C -> A -> G
//--------------------------------------------------------------------------------------------------
void AgentChaosZoneManager::ParseCGPacket(User* user, MSG_BASE* packet, WORD size)
{
    MSG_CG_CHAOS_ZONE_PACKET* recv_packet = static_cast<MSG_CG_CHAOS_ZONE_PACKET*>(packet);

    ChaosZoneCategory category_type = static_cast<ChaosZoneCategory>(recv_packet->category_type);
    switch(category_type)
    {
    case kCategoryZone:
        {
        }
        break;

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    case kCategoryBattleGround:
    case eCHAOSZONE_CATEGORY_GOLDRUSH:
        {
            BattleGroundManager::Instance()->ProcessPacket_CG(category_type, user, packet, size);
        } break;
#else
    case kCategoryBattleGround:
        {
            BattleGroundManager::Instance()->GetCGPacketHandler().OnCG_ProcessPacket(user, packet, size);
        } break;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
       
    case kCategoryFreePVP:
        {
            FreePVPManager::Instance()->GetCGPacketHandler().OnCG_ProcessPacket(user, packet, size);
        } break;
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case eCHAOSZONE_CATEGORY_SONNENSCHEIN:
        {
            SonnenScheinManager::Instance()->ProcessPacket_CG(user, packet, size);
        }break;
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	case eCHAOSZONE_CATEGORY_ONEDAYSGUILDMISSION:
		{
			OneDaysGuildMissionManager::Instance()->ProcessPacket_CG(user, packet, size);
		}break;
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
    }
}

//--------------------------------------------------------------------------------------------------
// G -> A
//--------------------------------------------------------------------------------------------------
void AgentChaosZoneManager::ParseAGPacket(GameServerSession* server_session, MSG_BASE* packet, WORD size)
{
    MSG_CG_CHAOS_ZONE_PACKET* recv_packet = static_cast<MSG_CG_CHAOS_ZONE_PACKET*>(packet);
    ChaosZoneCategory category_type = static_cast<ChaosZoneCategory>(recv_packet->category_type);
    switch(category_type)
    {
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    case kCategoryBattleGround:
    case eCHAOSZONE_CATEGORY_GOLDRUSH:
        {
            BattleGroundManager::Instance()->ProcessPacket_AG(category_type, server_session, packet, size);
        } break;
#else
    case kCategoryBattleGround:
        {
            BattleGroundManager::Instance()->GetAGPacketHandler().OnAG_ProcessPacket(server_session, packet, size);
        } break;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

    case kCategoryFreePVP:
        {
            FreePVPManager::Instance()->GetAGPacketHandler().OnAG_ProcessPacket(server_session, packet, size);
        }
        break;
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case eCHAOSZONE_CATEGORY_SONNENSCHEIN:
        {
            SonnenScheinManager::Instance()->ProcessPacket_AG(server_session, packet, size);
        }
        break;
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	case eCHAOSZONE_CATEGORY_ONEDAYSGUILDMISSION:
		{
			OneDaysGuildMissionManager::Instance()->ProcessPacket_AG(server_session, packet, size);
		}
		break;
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
    }
}
#endif  // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

