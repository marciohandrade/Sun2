#pragma once

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include "GameBattleResult.h"

class OneDaysGuildMission;

class OneDaysGuildMissionResult : public GameBattleResult
{
public:
	OneDaysGuildMissionResult() : onedaysguildmission_(NULL) {}
	virtual ~OneDaysGuildMissionResult() {}

	virtual void Leave(Player* player, bool adnormal_disconnect);

	virtual bool SendBattleResult(Player* player, int team_flag = GameBattleResult::HOME_TEAM);
	inline void SetOneDayGuildMissin(OneDaysGuildMission* const onedaysguildmission) { onedaysguildmission_ = onedaysguildmission;}

protected:
	virtual void update_team(GameBattleResult::BattlePlayer* battle_player);

private:
	OneDaysGuildMission* onedaysguildmission_;
};

#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)