#include "StdAfx.h"
#include ".\MissionBattleResult.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

MissionBattleResult::MissionBattleResult(void)
{
}

MissionBattleResult::~MissionBattleResult(void)
{
}

void MissionBattleResult::update_team(GameBattleResult::BattlePlayer* battle_player)
{
    battle_player->team = 0;
}
