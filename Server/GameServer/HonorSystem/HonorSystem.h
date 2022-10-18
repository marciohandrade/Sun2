#pragma once
#ifndef _GAMESERVER_HONORSYSTEM_HONORSYSTEM_H
#define _GAMESERVER_HONORSYSTEM_HONORSYSTEM_H
#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "QuestInfoManagerEx.h"
#include <CGuid.h>
#include <atltime.h>

class TodayQuestSystem
{
    struct QUEST_TODAY_QUEST_INFO_EX : QUEST_TODAY_QUEST_INFO
    {
        CTime next_update_time;
    };

    typedef STLX_MAP<WORD, QUEST_TODAY_QUEST_INFO_EX> TodayQuestData;
    TodayQuestData today_quest_data;

public:
    TodayQuestSystem();
    ~TodayQuestSystem();
    const QUEST_TODAY_QUEST_INFO_EX* GetTodayQuest(WORD group) const;
    bool IsTodayQuest(WORD group, WORD code) const;
    CTime GetNextUpdateTime(WORD qcode) const;
    bool UpdateTodayQuest(bool force = false);
    void Serialize(QUEST_TODAY_QUEST_INFO* _quest_info, WORD& _count);

    __DISABLE_COPY(TodayQuestSystem);
};

class HonorSystem : public util::Singleton<HonorSystem>
{
    static const ulong kUpdateInterval = 60 * 1000;
    util::ITimerBase update_timer_;
    TodayQuestSystem today_quests_;

public:
    HonorSystem();
    ~HonorSystem();

    WORD GetFameReputeGrade(BYTE type, WORD code, DWORD point);

    void ParseCGPacket(PVOID packet);

    void ParseCGPacketTitle(PVOID packet);
    void OnPACKET_CG_HONORSYSTEM_TITLE_CHANGE_CMD(PVOID packet);

    void ParseDGPacket(PVOID packet);
    void ParseDGPacketTitle(PVOID packet);
    void ParseDGPacketFameRepute(PVOID packet);
    void ParseDGPacketDailyQuest(PVOID packet);

    void Update();

    void ChoiceTodayQuest(bool force = false);
    bool IsTodayQuest(WORD group, WORD code);

    //
    __DISABLE_COPY(HonorSystem);
};

#endif // _NA_003027_20111013_HONOR_SYSTEM
#endif //_GAMESERVER_HONORSYSTEM_HONORSYSTEM_H