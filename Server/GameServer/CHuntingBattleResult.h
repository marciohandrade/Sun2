#ifndef SUN_GAMESERVER_CHUNTINGBATTLERESULT_H_
#define SUN_GAMESERVER_CHUNTINGBATTLERESULT_H_


#if _MSC_VER > 1000
#pragma once
#endif

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT

#include "GameBattleResult.h"

class CHuntingManager;
class CHuntingBattleResult : public GameBattleResult
{
public:
    CHuntingBattleResult(void);
    virtual ~CHuntingBattleResult(void);

    void set_competition_manager(CHuntingManager* competition_manager);
    
    virtual bool SendBattleResult(Player* player, int team_flag = GameBattleResult::HOME_TEAM);

protected:
    virtual void update_team(GameBattleResult::BattlePlayer* battle_player);

    CHuntingManager*    competition_manager_;
};

inline void CHuntingBattleResult::set_competition_manager(CHuntingManager* competition_manager)
{
    competition_manager_ = competition_manager;
}



#endif //SUN_GAMESERVER_CHUNTINGBATTLERESULT_H_
