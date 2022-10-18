#ifndef __ACHIEVEMENT_ACHIEVEMENTNOTIFY_H
#define __ACHIEVEMENT_ACHIEVEMENTNOTIFY_H
#pragma once

#include "ResultCode.h"

class AchievementNotify
{
public:
	virtual void OnValueChanged(WORD index, WORD location_code, BYTE object_index, 
								WORD previous_value, WORD current_value) = 0;
	virtual void Onreward(WORD index, WORD location_code, 
						  Achievement_ParserInfo_Reward  rewards_[MAX_ACHIEVEMENT_REWARD], 
						  bool is_reward, RC::eAC_RESULT rc) = 0;
#ifdef _SERVER
    virtual void OnCanCollectTreasureBox() = 0;
#endif
};

#endif //__ACHIEVEMENT_ACHIEVEMENTNOTIFY_H
