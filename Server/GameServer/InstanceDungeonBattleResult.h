#ifndef SUN_GAMESERVER_INSTANCEDUNGEONBATTLERESULT_H_
#define SUN_GAMESERVER_INSTANCEDUNGEONBATTLERESULT_H_


#if _MSC_VER > 1000
#pragma once
#endif

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT

#include "MissionBattleResult.h"

class InstanceDungeonBattleResult : public MissionBattleResult
{
public:
    InstanceDungeonBattleResult(void);
    virtual ~InstanceDungeonBattleResult(void);

protected:
    virtual void update_team(GameBattleResult::BattlePlayer* battle_player);
};


#endif //SUN_GAMESERVER_INSTANCEDUNGEONBATTLERESULT_H_
