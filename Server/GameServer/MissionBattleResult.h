#ifndef SUN_GAMESERVER_MISSIONBATTLERESULT_H_
#define SUN_GAMESERVER_MISSIONBATTLERESULT_H_


#if _MSC_VER > 1000
#pragma once
#endif

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

#include "GameBattleResult.h"

class MissionBattleResult : public GameBattleResult
{
public:
    MissionBattleResult(void);
    virtual ~MissionBattleResult(void);

protected:
    virtual void update_team(GameBattleResult::BattlePlayer* battle_player);
};


#endif //SUN_GAMESERVER_MISSIONBATTLERESULT_H_
