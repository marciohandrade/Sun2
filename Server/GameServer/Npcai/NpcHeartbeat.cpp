#include "stdafx.h"
#include "./NpcHeartbeat.h"

namespace nsAI {
;
#define HEARTBEAT_TICK_ALIGN(__interval) \
    (((__interval) + \
      ((nsAI::HeartbeatTimer::kHeartbeatInterval) - 1)) & \
       (~((nsAI::HeartbeatTimer::kHeartbeatInterval) - 1)))

#define HEARTBEAT_TICK_ALIGN64(__interval) \
    (((__interval) + \
      ((nsAI::NpcHeartbeat::kHeartbeatInterval) - 1)) & \
       (~((nsAI::NpcHeartbeat::kHeartbeatInterval) - 1)))


BOOST_STATIC_ASSERT(nsAI::HeartbeatTimer::kHeartbeatInterval == 128);

}; //end of namespace

//==================================================================================================

bool nsAI::HeartbeatTimer::SetTimer(ulong interval)
{
    nsAI::NpcHeartbeat* const heartbeat = nsAI::NpcHeartbeat::Instance();
    interval_ = HEARTBEAT_TICK_ALIGN(interval) >> 7;

    uint64_t approximated_current_tick = heartbeat->current_aged_ticks_;
    ;        approximated_current_tick = HEARTBEAT_TICK_ALIGN(approximated_current_tick) >> 7;
    expiration_tick_ = static_cast<ulong>(approximated_current_tick + interval_);
    return true;
};

bool nsAI::HeartbeatTimer::IsExpired() const
{
    nsAI::NpcHeartbeat* const heartbeat = nsAI::NpcHeartbeat::Instance();
    uint64_t approximated_current_tick = heartbeat->current_aged_ticks_;
    ;        approximated_current_tick = HEARTBEAT_TICK_ALIGN(approximated_current_tick) >> 7;
    // reversed period(6362days) = 49.7days * 128;
    if (expiration_tick_ > approximated_current_tick) {
        return false;
    };
    ulong realign_tick = static_cast<ulong>(approximated_current_tick + interval_);
    const_cast<ulong&>(expiration_tick_) = realign_tick;

    return true;
}

//==================================================================================================

bool nsAI::HeartbeatStaticTimer::SetTimer(eStaticTimer timer_type, ulong custom_interval)
{
    nsAI::NpcHeartbeat* const heartbeat = nsAI::NpcHeartbeat::Instance();
    return heartbeat->ConfigureStaticTimer(this, timer_type, custom_interval);
}

bool nsAI::HeartbeatStaticTimer::IsExpired() const
{
    if (timer_type_ == this->eStaticTimer_None) {
        return false;
    };
    nsAI::NpcHeartbeat* const heartbeat = nsAI::NpcHeartbeat::Instance();
    return heartbeat->IsExpiredStaticTimer(this);
}

//ulong nsAI::HeartbeatStaticTimer::GetTimeoutTicks() const {
//    return (0);
//};

//==================================================================================================

nsAI::NpcHeartbeat* nsAI::NpcHeartbeat::instance_static_ = NULL;

nsAI::NpcHeartbeat* nsAI::NpcHeartbeat::CreateInstance()
{
    static struct AutoClosure {
        AutoClosure(NpcHeartbeat** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        NpcHeartbeat** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    };
    NpcHeartbeat* provider = new NpcHeartbeat;
    //
    instance_static_ = provider;
    //
    return instance_static_;
};

//==================================================================================================

nsAI::NpcHeartbeat::NpcHeartbeat()
    : current_aged_ticks_(0)
    , timeout_aged_ticks_(0)
    , ai_script_loaded_(false)
{
    ZeroMemory(static_timer_data_, sizeof(static_timer_data_));
}

nsAI::NpcHeartbeat::~NpcHeartbeat()
{
}

bool nsAI::NpcHeartbeat::Update(const util::ServerTimeInfoPerFrame* time_info_per_frame)
{
    NpcHeartbeat* const this_ = Instance();
    const uint64_t ticks = this_->current_aged_ticks_ + time_info_per_frame->delta_tick_;
    this_->current_aged_ticks_ = ticks;
    if (ticks < this_->timeout_aged_ticks_) {
        return false;
    };
    uint64_t timeout = ticks + kHeartbeatInterval;
    ;        timeout = HEARTBEAT_TICK_ALIGN64(timeout);
    this_->timeout_aged_ticks_ = timeout;
    //
    if (this_->ai_script_loaded_ == false) {
        return true;
    };
    //
    StaticTimerData* timer_it = this_->static_timer_data_,
                   * const timer_end = &this_->static_timer_data_[\
                        _countof(this_->static_timer_data_)];
    for (; timer_it != timer_end; ++timer_it)
    {
        if (timer_it->interval == 0) {
            continue;
        };
        if (ticks < timer_it->expiration_aged_ticks) {
            continue;
        };
        uint64_t next_expiration = timeout + timer_it->interval;
        ulong diff = static_cast<ulong>(\
            (next_expiration - timer_it->expiration_aged_ticks) / timer_it->interval);
        timer_it->expiration_aged_ticks = next_expiration;
        timer_it->current_index += (diff + 1);;
    };
    //
    return true;
}

// CHANGES: f110809.1L
bool nsAI::NpcHeartbeat::OnAiScriptLoaded()
{
    BOOST_STATIC_ASSERT(HEARTBEAT_TICK_ALIGN(1000) == 1024);
    NpcHeartbeat* const this_ = Instance();
    //----------------------------------------------------------------------------------------------
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_MinSyncInterval])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_MinSyncInterval;
        static_timer_data->interval = HEARTBEAT_TICK_ALIGN(128);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_Idle])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_Idle;
        //static_timer_data->current_index = 0;
        static_timer_data->interval = HEARTBEAT_TICK_ALIGN(3000);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_Search])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_Search;
        //static_timer_data->current_index = 0;
        static_timer_data->interval = HEARTBEAT_TICK_ALIGN(2000);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_Track])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_Track;
        static_timer_data->interval = HEARTBEAT_TICK_ALIGN(256);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_ReturnCheck])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_ReturnCheck;
        static_timer_data->interval = HEARTBEAT_TICK_ALIGN(2000);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_Thrust])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_Thrust;
        static_timer_data->interval = HEARTBEAT_TICK_ALIGN(1500);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_Retreat])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_Retreat;
        static_timer_data->interval = HEARTBEAT_TICK_ALIGN(5000);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_FallApart])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_FallApart;
        static_timer_data->interval = HEARTBEAT_TICK_ALIGN(2000);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    if (StaticTimerData* static_timer_data = \
            &this_->static_timer_data_[nsAI::HeartbeatStaticTimer::eStaticTimer_Blind])
    {
        static_timer_data->timer_type = nsAI::HeartbeatStaticTimer::eStaticTimer_Blind;
        static_timer_data->interval = 2 * HEARTBEAT_TICK_ALIGN(2000);
        //static_timer_data->expiration_aged_ticks = 0;
    };
    //----------------------------------------------------------------------------------------------
    this_->ai_script_loaded_ = true;
    return true;
}

bool nsAI::NpcHeartbeat::ConfigureStaticTimer(
    nsAI::HeartbeatStaticTimer* const timer,
    HeartbeatStaticTimer::eStaticTimer timer_type,
    ulong custom_interval)
{
    ZeroMemory(timer, sizeof(timer));
    if (FlowControl::FCAssert(timer_type > timer->eStaticTimer_None &&
                              timer_type < timer->eStaticTimer_Counts) == false)
    {
        return false;
    };
    const StaticTimerData& static_timer_data = static_timer_data_[timer_type];
    timer->timer_type_ = static_cast<uint8_t>(timer_type);
    timer->latest_checked_index_ = static_timer_data.current_index;
    if (custom_interval)
    {
        // assertion(static_timer_data.interval != 0)
        if (custom_interval > static_timer_data.interval)
        {
            custom_interval = HEARTBEAT_TICK_ALIGN(custom_interval);
            ulong diff = custom_interval - static_timer_data.interval;
            timer->interval_multiplier_ = \
                static_cast<uint16_t>(diff / static_timer_data.interval);
        };
    };
    return true;
}

bool nsAI::NpcHeartbeat::IsExpiredStaticTimer(
    const nsAI::HeartbeatStaticTimer* const timer) const
{
    if (timer->timer_type_ <= timer->eStaticTimer_None ||
        timer->timer_type_ >= timer->eStaticTimer_Counts)
    {
        return false;
    };
    const StaticTimerData& static_timer_data = static_timer_data_[timer->timer_type_];
    if (timer->latest_checked_index_ >= static_timer_data.current_index) {
        return false;
    };
    if (ulong multiplier = timer->interval_multiplier_)
    {
        ulong diff = static_timer_data.current_index - timer->latest_checked_index_;
        if (multiplier > diff) {
            return false;
        };
    };
    timer->latest_checked_index_ = static_timer_data.current_index;
    return true;
}
