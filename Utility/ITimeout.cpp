#include "UtilityCommon.h"
#include "./ITimeout.h"

//==================================================================================================
#if __ITIMERBASE_VERSION_CRTL == 0x0120

namespace util {
;
//
//
//------------------------------------------------------------------------------
//
//
bool
ITimerBase::SetTimer(ulong interval, bool start, bool auto_reset)
{
    Clear();
    // eMode_ManualReset | eMode_AutoReset
    SetInterval(interval);
    if (auto_reset) {
        is_auto_ = true;
    };
    if (start) {
        Start();
    }
    else {
        Stop();
    };
    return true;
}

/*
  thinking notes:
  current
  timeout
  interval

  //     timeout#2  current  timeout#1
  //---------|---------|---------|-------
  //
  timeout#2 - type overflow에 의해서 발생할 수 있다.
  expired
  current < timeout#1            X
  current + interval < timeout   X

  if (current > timeout) then expired
  if (current <= timeout) then
*/
bool
ITimerBase::IsExpired() const
{
    if (is_start_ == 0) {
        return false;
    }
    bool is_expired = false;

    ulong current = GetTickCount();
    ;     current >>= 2;
    ulong interval = interval_;
    ulong timeout = checked_ + interval;
    if (checked_ <= current)
    {
        if (timeout <= current) {
            is_expired = true;
        }
    }
    else
    {
        const ulong UpperLimit = (ULONG_MAX >> 2) + 1;
        current += UpperLimit;
        if (timeout <= current) {
            is_expired = true;
        }
    };
    if (is_expired) {
        Reset();
    }
    return is_expired;
}

ulong ITimerBase::GetRemainTicks() const
{
    if (is_start_ == 0) {
        return ULONG_MAX;
    }
    bool is_expired = false;

    ulong current = GetTickCount();
    ;     current >>= 2;
    ulong interval = interval_;
    ulong timeout = checked_ + interval;
    if (checked_ <= current)
    {
        if (timeout <= current) {
            is_expired = true;
        }
    }
    else
    {
        const ulong UpperLimit = (ULONG_MAX >> 2) + 1;
        current += UpperLimit;
        if (timeout <= current) {
            is_expired = true;
        }
    };
    if (is_expired) {
        return 0;
    }
    return static_cast<ulong>((timeout - current) << 2);
}

// interfaces to support a game timer
float ITimerBase::GetProgressRatio() const
{
    ulong interval = GetInterval();
    if (interval == 0) {
        return 1.0f;
    };
    ulong remains = GetRemainTicks();
    if (remains == ULONG_MAX) {
        return 0.0f;
    }
    if (remains == 0) {
        return 1.0f;
    };
    float progress = static_cast<float>(remains) / interval;
    ;     progress = 1.0f - progress;
    return progress;
}

ulong ITimerBase::GetProcessTime() const
{
    float progress = GetProgressRatio();
    if (progress <= 0.0f) {
        return 0;
    };
    ulong interval = GetInterval();
    if (progress >= 1.0f) {
        return interval;
    }
    progress *= interval;
    return static_cast<ulong>(progress);
}

}; //end of namespace

#endif //__ITIMERBASE_VERSION_CRTL == 0x0120

