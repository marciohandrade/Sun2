#include "StdAfx.h"
#include ".\PVPBattleResult.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

#include "PVPInfo.h"

PVPBattleResult::PVPBattleResult(void)
{
}

PVPBattleResult::~PVPBattleResult(void)
{
}

void PVPBattleResult::update_team(GameBattleResult::BattlePlayer* battle_player)
{
    Player* battle_find_player = PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
    if (battle_find_player != NULL)
    {
        battle_player->team = battle_find_player->GetPVPInfo().GetTeam();
    }
    //battle_player->team = battle_player->player->GetPVPInfo().GetTeam();

}
