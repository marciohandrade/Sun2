#ifndef SUN_GAMESERVER_DOMINATIONBATTLERESULT_H_
#define SUN_GAMESERVER_DOMINATIONBATTLERESULT_H_


#if _MSC_VER > 1000
#pragma once
#endif


//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

#include "GameBattleResult.h"

class DominationBattleResult : public GameBattleResult
{
public:
    DominationBattleResult(void);
    virtual ~DominationBattleResult(void);

    virtual bool Enter(Player* player);
    virtual void Leave(Player* player, bool abnormal_disconnect);
    virtual bool SendBattleResult(Player* player, int team_flag = GameBattleResult::HOME_TEAM);

#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
    void ProcessSunRanking();
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
};

#endif //SUN_GAMESERVER_DOMINATIONBATTLERESULT_H_
