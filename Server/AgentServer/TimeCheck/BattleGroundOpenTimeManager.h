#pragma once
#ifndef AGENTSERVER_BATTLE_GROUND_OPEN_TIME_MANAGER_H
#define AGENTSERVER_BATTLE_GROUND_OPEN_TIME_MANAGER_H

#include <Singleton.h>
#include <BattleGroundTimeInfoParser.h>
#include <ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h>

struct MSG_CG_ETC_GET_BATTLE_GROUND_OPEN_TIME_ACK;

class BattleGroundOpenTimeManager : public util::Singleton<BattleGroundOpenTimeManager> 
{
public:
    static const ulong kUpdateInterval = 5 * 1000;
    BattleGroundOpenTimeManager();
    ~BattleGroundOpenTimeManager();

    bool IsOpenBattleGround();
    void Update(const util::TimeInfoPerFrame& time_info);
    //
    // making message: find closely first event and calculation
    bool SetupMessage(const util::TimeInfoPerFrame& time_info,
                      MSG_CG_BATTLE_GROUND_GET_OPEN_TIME_ACK* const msg_ack) const;
private:
    void ExecuteNotifier();
    //
    bool loaded_;
    util::ITimerBase update_timer_;
    ns_func::BattleGroundTimeNode current_mode_;
};
#endif //AGENTSERVER_BATTLE_GROUND_OPEN_TIME_MANAGER_H