#include "StdAfx.h"
#include ".\AccumulatePointMatchDate.h"
#include "DBProxyServerEx.h"
#include <PacketStruct_DG.h>
#include "Sessions/GameServerSession.h"

#ifdef _NA_1419_20100121_ACCUMULATE_POINT

AccumulatePointMatchDate::AccumulatePointMatchDate()
{
    SYSTEMTIME localtime;
    GetLocalTime(&localtime);
    MatchDate(localtime);
    expire_timer_.SetTimer(10000);   // 10초마다 실행
}

AccumulatePointMatchDate::~AccumulatePointMatchDate()
{
}

void AccumulatePointMatchDate::Update()
{
    if (expire_timer_.IsExpired(true) == false) {
        return;
    };

    SYSTEMTIME localtime;
    MSG_DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF message;

    GetLocalTime(&localtime);
    ;{
        MatchDate(localtime);
        message.wYear = year_;
        message.wDayOfWeek = dayofweek_;
        message.dwSecond = secondofday_;
    };
    g_DBProxyServer.SendToSessionType(GAME_SERVER, &message, sizeof(message));
}

void AccumulatePointMatchDate::SendMatchDate(GameServerSession* game_server)
{
    MSG_DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF message;
    message.wYear = year_;
    message.wDayOfWeek = dayofweek_;
    message.dwSecond = secondofday_;
    game_server->Send((BYTE*)&message, sizeof(MSG_DG_OTHER_ACCUMULATEPOINT_MATCHDATE_NTF));
}

void AccumulatePointMatchDate::ApplyCurrentYear(BASE_PLAYERINFO* player_info)
{
    if (player_info->m_wAccumulateYear != year_)
    {
        if (player_info->m_wAccumulateYear == 0)
            player_info->m_wAccumulatePoint = 0;

        player_info->m_wAccumulateYear = year_;
        player_info->m_dwAccumulateTime = 0;
    }
}

void AccumulatePointMatchDate::MatchDate(SYSTEMTIME& localtime)
{
    year_ = localtime.wYear;
    dayofweek_ = localtime.wDayOfWeek;
    secondofday_ = localtime.wHour * 3600 + localtime.wMinute * 60 + localtime.wSecond;
}

#endif//_NA_1419_20100121_ACCUMULATE_POINT