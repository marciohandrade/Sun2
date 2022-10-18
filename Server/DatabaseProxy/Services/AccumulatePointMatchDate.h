#ifndef SUN_GAMESERVER_ACCUMULATEPOINTMATCHDATE_H_
#define SUN_GAMESERVER_ACCUMULATEPOINTMATCHDATE_H_


#if _MSC_VER > 1000
#pragma once
#endif

#include <Singleton.h>

class GameServerSession;
class AccumulatePointMatchDate : public util::Singleton<AccumulatePointMatchDate>
{
public:
	AccumulatePointMatchDate();
	~AccumulatePointMatchDate();

    void Update();
    void SendMatchDate(GameServerSession* game_server);
    void ApplyCurrentYear(BASE_PLAYERINFO* player_info);

private:
    void MatchDate(SYSTEMTIME& localtime);

    ushort  year_;
    ushort  dayofweek_;
    uint    secondofday_;
    util::Timer expire_timer_;
};


#endif