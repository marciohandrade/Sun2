#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "GameBattleResult.h"
class ChaosZoneManager;
class BattleGround;

class BattleGroundBattleResult : public GameBattleResult
{
public:
    BattleGroundBattleResult() : battle_ground_(NULL)
    {
    }
    virtual ~BattleGroundBattleResult()
    {
    }
    virtual bool SendBattleResult(Player* player, int team_flag = GameBattleResult::HOME_TEAM);
    inline void SetBattleGround(BattleGround* const battle_ground) { battle_ground_ = battle_ground; }

protected:
    virtual void update_team(GameBattleResult::BattlePlayer* battle_player);
private:
    BattleGround* battle_ground_;
};

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND