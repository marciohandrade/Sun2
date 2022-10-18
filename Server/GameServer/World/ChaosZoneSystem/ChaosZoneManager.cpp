#include "StdAfx.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/ChaosZoneManager.h>
#include <World/ChaosZoneSystem/BattleGround.h>
#include <World/ChaosZoneSystem/FreePVP.h>
#include <GameDominationManager.h>
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include <World/ChaosZoneSystem/GoldRush.h>
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include <World/ChaosZoneSystem/SonnenSchein.h>
#include "GameGuild.h"
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include <World/ChaosZoneSystem/OneDaysGuildMission.h>
//#include "GameGuild.h"
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION

void ChaosZoneManager::Init(const ChaosZoneType chaos_zone_type)
{
    chaos_zone_type_ = chaos_zone_type;
    if (chaos_zone_ == NULL)
    {
        switch (chaos_zone_type_)
        {
        case kBattleGround:
            {
                chaos_zone_ = new BattleGround();
            } break;
        case kFreePVP:
            {
                chaos_zone_ = new FreePVP();
            } break;
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
        case eCHAOSZONE_TYPE_GOLDRUSH:
            {
                chaos_zone_ = new GoldRush();
            } break;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
        case eCHAOSZONE_TYPE_SONNENSCHEIN:
            {
                chaos_zone_ = new SonnenSchein();
            } break;
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
		case eCHAOSZONE_TYPE_ONEDAYSGUILDMISSION:
			{
				chaos_zone_ = new OneDaysGuildMission();
			} break;
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)
        }
    }

    if (chaos_zone_)
    {
        chaos_zone_->Init(instace_dungeon_);
    }
}

void ChaosZoneManager::Release()
{
    SAFE_RELEASENDELETE(chaos_zone_);
}

void ChaosZoneManager::Update()
{
    if (chaos_zone_)
    {
        chaos_zone_->Update();
    }
}

bool ChaosZoneManager::EnterPlayer(Player* const player)
{
    if (chaos_zone_ == NULL)
    {
        return false;
    }

    // 카오스존에서 인장효과 제거
    switch (chaos_zone_type_)
    {
    case kBattleGround:
    case kFreePVP:
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    case eCHAOSZONE_TYPE_GOLDRUSH:
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case eCHAOSZONE_TYPE_SONNENSCHEIN:
#endif //_NA_008334_20150608_SONNENSCHEIN
        {
#ifdef _NA_008334_20150608_SONNENSCHEIN
            GameGuild* player_guild = player->GetGuild();
            if (player_guild != NULL) {
                player_guild->ApplyGuildRewardOption(player, FALSE);
            }
#else
            GameDominationManager::Instance()->ApplyAllRewardOption(player, FALSE);
#endif //_NA_008334_20150608_SONNENSCHEIN

            GameDominationManager::Instance()->SendAreaInfoToPlayer(player, FALSE);
        }
        break;
    default:
        break;
    }
    
    return chaos_zone_->EnterPlayer(player);
}

void ChaosZoneManager::LeavePlayer(Player* const player)
{
    if (chaos_zone_ == NULL)
    {
        return;
    }

    // 카오스존에서 나갈 때 인장효과 활성화
    switch (chaos_zone_type_)
    {
    case kBattleGround:
    case kFreePVP:
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    case eCHAOSZONE_TYPE_GOLDRUSH:
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case eCHAOSZONE_TYPE_SONNENSCHEIN:
#endif //_NA_008334_20150608_SONNENSCHEIN
        {
#ifdef _NA_008334_20150608_SONNENSCHEIN
            GameGuild* player_guild = player->GetGuild();
            if (player_guild != NULL) {
                player_guild->ApplyGuildRewardOption(player);
            }
#else
            GameDominationManager::Instance()->ApplyAllRewardOption(player, TRUE);
#endif //_NA_008334_20150608_SONNENSCHEIN
            GameDominationManager::Instance()->SendAreaInfoToPlayer(player, TRUE);
        }
        break;
    default:
        break;
    }
    chaos_zone_->LeavePlayer(player);
}

void ChaosZoneManager::EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, 
                                                                     DWORD* user_key_list)
{
    if (chaos_zone_)
    {
        chaos_zone_->EnterTeam(team_key, player_count, user_key_list);
    }
}

eUSER_RELATION_TYPE ChaosZoneManager::IsFriendPVP(Player* attacker,Player* target)
{
    if (chaos_zone_)
        return chaos_zone_->IsFriendPVP(attacker, target);
    return USER_RELATION_NEUTRALIST;
}

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
void ChaosZoneManager::DeadCharacter( Character* const dead_character, Character* const kill_character )
{
    if (chaos_zone_ != NULL) {
        chaos_zone_->DeadCharacter(dead_character, kill_character);
    }
}

#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_008334_20150608_SONNENSCHEIN
RC::eSTATUS_RESULT ChaosZoneManager::ResurrectionAtSafezone( Player* player )
{
    if (chaos_zone_ == NULL) {
        return RC::RC_STATUS_RESURRECTION_FAIL;
    }

    return chaos_zone_->ResurrectionAtSafezone(player);
}
#endif //_NA_008334_20150608_SONNENSCHEIN

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
