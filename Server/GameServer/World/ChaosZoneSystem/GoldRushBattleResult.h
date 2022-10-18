#pragma once

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "GameBattleResult.h"
class GoldRush;

class GoldRushBattleResult : public GameBattleResult
{
public:
    GoldRushBattleResult() : goldrush_(NULL) {}
    virtual ~GoldRushBattleResult() {}

    virtual void Leave(Player* player, bool abnormal_disconnect);

    virtual bool SendBattleResult(Player* player, int team_flag = GameBattleResult::HOME_TEAM);
    inline void SetGoldRush(GoldRush* const goldrush) { goldrush_ = goldrush;}

#ifdef _JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT
    bool SendBattleResult_RegisterGameLog(Player* player, int team_flag = GameBattleResult::HOME_TEAM);
#endif //_JP_007387_20140702_GAMELOG_GOLDRUSH_GOLDPOINT

protected:
    virtual void update_team(GameBattleResult::BattlePlayer* battle_player);

private:
    GoldRush* goldrush_;
};
#endif ///_NA_007065_20140217_CHAOSZONE_GOLDRUSH