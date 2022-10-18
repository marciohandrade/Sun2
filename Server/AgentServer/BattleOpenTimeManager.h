#pragma once
#ifndef AGENTSERVER_BATTLE_OPEN_TIME_MANAGER_H
#define AGENTSERVER_BATTLE_OPEN_TIME_MANAGER_H

#include <CurseTowerTimeParser.h>

struct MSG_CG_ETC_GET_BATTLE_OPEN_TIME_ACK;

#if __ITIMERBASE_VERSION_CRTL >= 0x0120
//==================================================================================================

class BattleOpenTimeManager
{
public:
    static const ulong kUpdateInterval = 5 * 1000;
    BattleOpenTimeManager();
    ~BattleOpenTimeManager();

    bool IsOpenDungeon(uint16_t index);
    void Update(const util::TimeInfoPerFrame& time_info);
    //
    bool is_loaded() const;
    // making message: find closely first event and calculation
    bool SetupMessage(const util::TimeInfoPerFrame& time_info,
                      MSG_CG_ETC_GET_BATTLE_OPEN_TIME_ACK* const msg_ack) const;
private:
    void SyncParser();
    void ExecuteNotifier();
    //
    bool loaded_;
    util::ITimerBase update_timer_;
    ns_func::BattleTimeNode current_mode_;
};

extern BattleOpenTimeManager g_BattleOpenTimeMng;

inline bool BattleOpenTimeManager::is_loaded() const
{
    return loaded_;
}

//==================================================================================================
#else // __ITIMERBASE_VERSION_CRTL < 0x0120
//==================================================================================================

struct st_BattleOpenTimeNode;

class BattleOpenTimeManager
{
public:

	BattleOpenTimeManager();
	~BattleOpenTimeManager();

	VOID					BattleJoinTimeUpdate();
	VOID					RegisterTimeUpdate();
	BOOL					TimeCheckRegisterNode(st_BattleOpenTimeNode *pNode);
	BOOL					IsOpenDungeon(WORD wIndex);
};

extern BattleOpenTimeManager g_BattleOpenTimeMng;

//==================================================================================================
#endif //__ITIMERBASE_VERSION_CRTL

#endif //AGENTSERVER_BATTLE_OPEN_TIME_MANAGER_H