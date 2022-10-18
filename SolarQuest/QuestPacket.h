#ifndef _SOLAR_QUEST_QUESTPACKET_H
#define _SOLAR_QUEST_QUESTPACKET_H

#include <atltime.h>
#include "Protocol_CG.h"
#include "QuestStruct.h"

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#pragma pack(push,1)

enum eCG_QUEST_DAILYQUEST_PACKET
{
    CG_QUEST_DAILYQUEST_INFO = 1,
};

//////////////////////////////////////////////////////////////////////////
//
struct MSG_CG_QUEST_DAILYQUEST_INFO_PACKET : MSG_CG_QUEST_DAILYQUEST_PACKET
{
    BYTE packet_type2;

    MSG_CG_QUEST_DAILYQUEST_INFO_PACKET()
    {
        packet_type = CG_QUEST_DAILYQUEST_INFO;
    }
};

enum eCG_QUEST_DAILYQUEST_INFO_PACKET
{
    CG_QUEST_DAILYQUEST_INFO_ALL_ACK = 1,
    CG_QUEST_DAILYQUEST_INFO_CMD,
    CG_QUEST_TODAYQUEST_INFO_CMD,
    CG_QUEST_DAILYQUEST_RESET_ACK,
};

struct MSG_CG_QUEST_DAILYQUEST_INFO_ALL_ACK : MSG_CG_QUEST_DAILYQUEST_INFO_PACKET
{
    WORD count_;
    QUEST_DAILY_INFO daily_quest_data[MAX_DAILYQUEST_INFO_COUNT];

    MSG_CG_QUEST_DAILYQUEST_INFO_ALL_ACK()
    {
        packet_type2 = CG_QUEST_DAILYQUEST_INFO_ALL_ACK;
        count_ = 0;
        memset(daily_quest_data, 0, sizeof(daily_quest_data));
    }
};

struct MSG_CG_QUEST_DAILYQUEST_INFO_CMD : MSG_CG_QUEST_DAILYQUEST_INFO_PACKET
{
    QUEST_DAILY_INFO daily_quest_info;

    MSG_CG_QUEST_DAILYQUEST_INFO_CMD()
    {
        packet_type2 = CG_QUEST_DAILYQUEST_INFO_CMD;
        memset(&daily_quest_info, 0, sizeof(daily_quest_info));
    }
};

struct MSG_CG_QUEST_TODAYQUEST_INFO_CMD : MSG_CG_QUEST_DAILYQUEST_INFO_PACKET
{
    WORD count_;
    QUEST_TODAY_QUEST_INFO today_quest_info[MAX_TODAYQUEST_COUNT];

    MSG_CG_QUEST_TODAYQUEST_INFO_CMD()
    {
        packet_type2 = CG_QUEST_TODAYQUEST_INFO_CMD;
        count_ = 0;
        memset(&today_quest_info, 0, sizeof(today_quest_info));
    }
};

struct MSG_CG_QUEST_DAILYQUEST_RESET_ACK : MSG_CG_QUEST_DAILYQUEST_INFO_PACKET
{
    CTime item_use_time;

    MSG_CG_QUEST_DAILYQUEST_RESET_ACK()
    {
        packet_type2 = CG_QUEST_DAILYQUEST_RESET_ACK;
        item_use_time = 0;
    }
};
#pragma pack(pop)

#endif //_NA_003027_20111013_HONOR_SYSTEM
#endif //_SOLAR_QUEST_QUESTPACKET_H