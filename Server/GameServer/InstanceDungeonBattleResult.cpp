#include "StdAfx.h"
#include ".\InstanceDungeonBattleResult.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <World/ChaosZoneSystem/ChaosZoneManager.h>
#endif

InstanceDungeonBattleResult::InstanceDungeonBattleResult(void)
{
}

InstanceDungeonBattleResult::~InstanceDungeonBattleResult(void)
{
}

void InstanceDungeonBattleResult::update_team(GameBattleResult::BattlePlayer* battle_player)
{
    battle_player->team = 0;
}
