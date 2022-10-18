#pragma once
#ifndef UTILITY_TIMER_H
#define UTILITY_TIMER_H

#include <ITimeout.h>

//==================================================================================================
/// Timer 클래스
/**
	@history
			- 2005.2.28 : [taiyo] Timer based time 으로 바꾸었다!
			- 2007.7.19 : [taiyo] IfcTimer, SimpleTimer 구현, 추후 Timer를 IfcTimer를 상속받게 처리할것임
            - 2010.11.01 : waverix|MESSAGE=the ITimerBase(v2xxx) utility replaced
            all existing timers (IfcTimer, SimpleTimer, Timer)
*/
//==================================================================================================

namespace util {
;

#if __ITIMERBASE_VERSION_CRTL == 0x0120

//--------------------------------------------------------------------------------------------------
//
class SimpleTimer
{
public:
    void Clear();
    void Reset(ulong interval);
    void SetTimer(ulong interval);
    bool IsExpired() const;
    ulong GetExpireTime();
    ulong GetRemainedTime();
    //void IncreasingExpireTime(ulong add_expire_time);
private:
    util::ITimerBase timer_base_;
};

//--------------------------------------------------------------------------------------------------
//
inline void SimpleTimer::Clear()
{
    timer_base_.Stop();
};

inline void SimpleTimer::Reset(ulong interval)
{
    timer_base_.SetTimer(interval, true, false);
}

inline void SimpleTimer::SetTimer(ulong interval)
{
    timer_base_.SetTimer(interval, true, false);
};

inline bool SimpleTimer::IsExpired() const
{
    return timer_base_.IsExpired();
}

inline ulong SimpleTimer::GetExpireTime()
{
    return timer_base_.GetTimeoutTicks();
}

inline ulong SimpleTimer::GetRemainedTime()
{
    return timer_base_.GetRemainTicks();
}

//==================================================================================================

#elif __ITIMERBASE_VERSION_CRTL < 0x0120

class IfcTimer
{
public:
	IfcTimer(){}
	virtual ~IfcTimer(){}
	
	virtual void Clear()							{}
	virtual void Reset( DWORD /*dwIntervalTime = 0*/ )	{}
	virtual void SetTimer( DWORD /*dwIntervalTime*/ )	{}
	// manual mode
	virtual bool IsExpired( bool /*bReset = true*/ )	{ return false; }
	// auto mode
	virtual bool IsExpiredAuto( bool /*bReset = true*/ ){ return false; }
	virtual DWORD GetExpireTime()					{ return 0; }
	virtual DWORD GetIntervalTime()					{ return 0; }
	virtual DWORD GetRemainedime()					{ return 0; }
	virtual DWORD GetProcessTime()					{ return 0; }
	virtual void IncreasingExpireTime( DWORD /*dwAddExpireTime*/ ) {}
	// range : [0,1]
	virtual float GetProgressRatio()				{ return 0.f; }

};

class SimpleTimer : public IfcTimer
{
public:
	SimpleTimer():m_dwExpireTime(0){}
	virtual ~SimpleTimer(){}

	virtual void Clear()
	{
		m_dwExpireTime=0;
	}
	virtual void Reset( DWORD dwIntervalTime )
	{
		m_dwExpireTime = GetTickCount() + dwIntervalTime;
	}

	virtual void SetTimer( DWORD dwIntervalTime )
	{
		Reset( dwIntervalTime );
	}
	// only manual mode
	virtual bool IsExpired( bool bReset = false/*not used*/ )
	{
		bReset;
		if( m_dwExpireTime && GetTickCount() >= m_dwExpireTime ) 
		{
			m_dwExpireTime=0;
			return true;
		}
		else 
			return false;
	}

	virtual DWORD GetExpireTime()	{ return m_dwExpireTime; }
	virtual DWORD GetRemainedTime()  
	{
		DWORD dwRemainedTime = 0;
		if( m_dwExpireTime > (dwRemainedTime=GetTickCount()) )
			return ( m_dwExpireTime- dwRemainedTime );
		else
			return 0;
	}
	virtual void IncreasingExpireTime( DWORD dwAddExpireTime )
	{
		m_dwExpireTime += dwAddExpireTime;
	}
private:
	DWORD m_dwExpireTime;
};

#endif //__ITIMERBASE_VERSION_CRTL

//==================================================================================================
// NOTE: this timer was tuned game logic cool-time requirements of game contents
// so that I can't change to 'ITimerBase::Version = 0x0120' because of InitCoolTime.
// CHANGES: f101101.1L, code arrangment
class Timer
{
public: 
    Timer();
    ~Timer();

public:
    void Clear();
    void SetTimer(const DWORD interval);
    DWORD GetBaseIntervalTime() const;
    DWORD GetIntervalTime() const;
    void SetBonusIntervalTime(int bonus_interval);
    // base_interval_ 시간 간격을 그대로 유지하고 다시 타이머를 돌린다.
    void Reset(/*base_interval_*/);
    // 타이머를 resume
    void EnableCheckTime();
    // 타이머를 중지한다.
    void DisableCheckTime();
    //void IncreasingExpireTime(DWORD dwAddExpireTime);
    // 만료타임이 되기까지의 현재 진행률
    float GetProgressRatio() const;
    // 완료되기 까지 남은 시간 (단위 ms)
    DWORD GetRemainedTime() const;
    // 진행된 시간
    DWORD GetProcessTime() const;
    // problem : 이 함수를 가끔 호출할 경우 IsExpired() == true가 되는 경우가 생긴다.
    // 왜나면 1 Tick이라도 오차가 생기지 않기 위해 IsExpired()호출한 시간을 적용하지 않는다.
    // Therefore, 이럴경우 IsExpired() == true된 후에 Reset()를 한번 호출해 준다!
    bool IsExpired(BOOL need_reset = true);
    // 한번만 타이머를 돌리기 위해
    bool IsExpiredManual(BOOL need_reset = true);
    void InitCoolTime(DWORD next_expire_ticks = 0);
    DWORD GetExpireTime() const;

private:
    DWORD expire_time_;
    DWORD base_interval_;
    bool  is_check_expire_;
    int   bonus_interval_;
};

inline Timer::Timer()
{
    ZeroMemory(this, sizeof(*this));
}

inline Timer::~Timer() {
}

inline void Timer::Clear()
{
    ZeroMemory(this, sizeof(*this));
}

inline void Timer::SetTimer(const DWORD interval)
{
    base_interval_ = interval;
    bonus_interval_ = 0;
    Reset();
}

inline DWORD Timer::GetBaseIntervalTime() const
{
    return base_interval_;
}

inline DWORD Timer::GetIntervalTime() const
{ 
    int interval = max(0, (int)base_interval_ + bonus_interval_);
    return interval;
}

inline void Timer::SetBonusIntervalTime(int bonus_interval)
{
    bonus_interval_ = bonus_interval;
}

// base_interval_ 시간 간격을 그대로 유지하고 다시 타이머를 돌린다.
inline void Timer::Reset(/*base_interval_*/)
{
    expire_time_ = GetTickCount() + GetIntervalTime();
    EnableCheckTime();
}

// 타이머를 resume
inline void Timer::EnableCheckTime()
{
    is_check_expire_ = true;
}

// 타이머를 중지한다.
inline void Timer::DisableCheckTime()
{
    is_check_expire_ = false;
}

//inline void Timer::IncreasingExpireTime(DWORD dwAddExpireTime)
//{
//    expire_time_ += dwAddExpireTime;
//}

// 만료타임이 되기까지의 현재 진행률
inline float Timer::GetProgressRatio() const
{
    int process_ticks = GetTickCount() - (expire_time_ - GetIntervalTime());
    return min(1.0f, (float)process_ticks / GetIntervalTime());
}

// 완료되기 까지 남은 시간 (단위 ms)
inline DWORD Timer::GetRemainedTime() const
{
    DWORD remained_ticks = 0;
    if (is_check_expire_ && expire_time_ > (remained_ticks = GetTickCount())) {
        return (expire_time_ - remained_ticks);
    }
    else {
        return 0;
    }
}

// 진행된 시간
inline DWORD Timer::GetProcessTime() const
{
    int process_ticks = GetTickCount() - (expire_time_ - GetIntervalTime());
    if (process_ticks >= 0) {
        return process_ticks;
    }
    else {
        return 0;
    }
}

// problem : 이 함수를 가끔 호출할 경우 IsExpired() == true가 되는 경우가 생긴다.
// 왜나면 1 Tick이라도 오차가 생기지 않기 위해 IsExpired()호출한 시간을 적용하지 않는다.
// Therefore, 이럴경우 IsExpired() == true된 후에 Reset()를 한번 호출해 준다!
inline bool Timer::IsExpired(BOOL need_reset)
{
    DWORD cur_ticks;
    if (is_check_expire_ && (cur_ticks = GetTickCount()) >= expire_time_) 
    {
        if (need_reset) {
            expire_time_ = cur_ticks + base_interval_;
        }
        return true;
    }
    else
    {
        return false;
    }
}

// 한번만 타이머를 돌리기 위해
inline bool Timer::IsExpiredManual(BOOL need_reset)
{
    DWORD cur_ticks;
    if (is_check_expire_ && (cur_ticks = GetTickCount()) >= expire_time_) 
    {
        if (need_reset) {
            expire_time_ = cur_ticks + base_interval_;
        }
        DisableCheckTime();
        return true;
    }
    else
    {
        return false;
    }
}

inline void Timer::InitCoolTime(DWORD next_expire_ticks)
{
    expire_time_ = next_expire_ticks;
}

inline DWORD Timer::GetExpireTime() const
{
    return expire_time_;
}


}//End of namespace util

#endif //UTILITY_TIMER_H

