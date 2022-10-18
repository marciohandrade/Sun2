#pragma once
#ifndef GAMESERVER_NPC_HEARTBEAT_H
#define GAMESERVER_NPC_HEARTBEAT_H

namespace nsAI {
;
class NpcHeartbeat;
//==================================================================================================
//
// NOTE: this object support a milestone continual progressing timer for npc
// attributes = { automatic reset, enable instantly time expiration, automatic start }
// WARNING: this object can't support always running system,
// but the usage of short time interval enable to support similar system.
class HeartbeatTimer
{
public:
    static const ulong kHeartbeatInterval = 128; // ms
    HeartbeatTimer();
    void Clear();
    bool SetTimer(ulong interval);
    bool IsExpired() const;

private:
    ulong expiration_tick_;
    ulong interval_;
    //
    friend class nsAI::NpcHeartbeat;
    __DISABLE_COPY(HeartbeatTimer);
};

//==================================================================================================
// NOTE: this class support the generic npc ai timer such as Wander, and etc...
class HeartbeatStaticTimer
{
public:
    static const ulong kHeartbeatInterval = nsAI::HeartbeatTimer::kHeartbeatInterval;
    enum eStaticTimer {
        eStaticTimer_None = 0,
        eStaticTimer_MinSyncInterval,
        eStaticTimer_Idle,
        eStaticTimer_Search,
        eStaticTimer_Track,
        eStaticTimer_ReturnCheck,
        eStaticTimer_Thrust,
        eStaticTimer_Retreat,
        eStaticTimer_FallApart,
        eStaticTimer_Blind,
        //
        eStaticTimer_Counts
    };
    //
    HeartbeatStaticTimer();
    void Clear();
    bool SetTimer(eStaticTimer timer_type, ulong custom_interval);
    bool IsExpired() const;

private:
    uint8_t timer_type_;
    uint8_t __padding0;
    uint16_t interval_multiplier_; // base_interval * [interval_multiplier_]
    mutable uint32_t latest_checked_index_;
    //
    friend class nsAI::NpcHeartbeat;
    __DISABLE_COPY(HeartbeatStaticTimer);
};

//==================================================================================================
//
class NpcHeartbeat
{
public:
    static const uint64_t kHeartbeatInterval = HeartbeatTimer::kHeartbeatInterval;
    static bool OnAiScriptLoaded();
    // CHANGES: f110316.3L, changes fixed interval to statistics
    static bool Update(const util::ServerTimeInfoPerFrame* time_info_per_frame);
private:
    struct StaticTimerData {
        uint8_t timer_type;
        uint32_t current_index;
        ulong interval;
        uint64_t expiration_aged_ticks;
    };
    NpcHeartbeat();
    ~NpcHeartbeat();

    static NpcHeartbeat* Instance();
    static NpcHeartbeat* CreateInstance();
    bool ConfigureStaticTimer(nsAI::HeartbeatStaticTimer* const timer,
        HeartbeatStaticTimer::eStaticTimer timer_type, ulong custom_interval);
    bool IsExpiredStaticTimer(const nsAI::HeartbeatStaticTimer* const timer) const;
    //
    uint64_t current_aged_ticks_;
    uint64_t timeout_aged_ticks_;
    bool ai_script_loaded_;
    StaticTimerData static_timer_data_[HeartbeatStaticTimer::eStaticTimer_Counts];
    //
    static NpcHeartbeat* instance_static_;
    //
    friend class nsAI::HeartbeatTimer;
    friend class nsAI::HeartbeatStaticTimer;
    __DISABLE_COPY(NpcHeartbeat);
};

//==================================================================================================
// solid probability
// 25.00%
struct Npc25PercentageRegulator {
public:
    Npc25PercentageRegulator() : increase_counter_(0) {}
    bool Hit();
private:
    ulong increase_counter_;
};
// 50.00%
struct Npc50PercentageRegulator {
public:
    Npc50PercentageRegulator() : increase_counter_(0) {}
    bool Hit();
private:
    ulong increase_counter_;
};
// 75.00%
struct Npc75PercentageRegulator {
public:
    Npc75PercentageRegulator() : increase_counter_(0) {}
    bool Hit();
private:
    ulong increase_counter_;
};

}; //end of namespace
//==================================================================================================
// implements

inline nsAI::HeartbeatTimer::HeartbeatTimer()
    : expiration_tick_(ULONG_MAX)
    , interval_(0)
{
}

inline void nsAI::HeartbeatTimer::Clear() {
    expiration_tick_ = ULONG_MAX;
    interval_ = 0;
};

inline nsAI::HeartbeatStaticTimer::HeartbeatStaticTimer()
    : timer_type_(eStaticTimer_None)
    , latest_checked_index_(0)
{
}

inline void nsAI::HeartbeatStaticTimer::Clear() {
    timer_type_ = eStaticTimer_None;
    latest_checked_index_ = 0;
};

inline nsAI::NpcHeartbeat* nsAI::NpcHeartbeat::Instance()
{
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    return CreateInstance();
};

inline bool nsAI::Npc25PercentageRegulator::Hit() {
    ulong counter = ++increase_counter_;
    return ((counter & 0x3) == 0x0);
};

inline bool nsAI::Npc50PercentageRegulator::Hit() {
    ulong counter = ++increase_counter_;
    return ((counter & 0x1) != 0x0);
};

inline bool nsAI::Npc75PercentageRegulator::Hit() {
    ulong counter = ++increase_counter_;
    return ((counter & 0x3) != 0x0);
};

#endif //GAMESERVER_NPC_HEARTBEAT_H