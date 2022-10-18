#ifndef SUN_GAMESERVER_PVPBATTLERESULT_H_
#define SUN_GAMESERVER_PVPBATTLERESULT_H_


#if _MSC_VER > 1000
#pragma once
#endif

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

#include "GameBattleResult.h"

class PVPBattleResult : public GameBattleResult
{
public:
    PVPBattleResult(void);
    virtual ~PVPBattleResult(void);

protected:
    virtual void update_team(GameBattleResult::BattlePlayer* battle_player);
};

#endif //SUN_GAMESERVER_PVPBATTLERESULT_H_
