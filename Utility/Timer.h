#pragma once
#ifndef UTILITY_TIMER_H
#define UTILITY_TIMER_H

#include <ITimeout.h>

//==================================================================================================
/// Timer Ŭ����
/**
	@history
			- 2005.2.28 : [taiyo] Timer based time ���� �ٲپ���!
			- 2007.7.19 : [taiyo] IfcTimer, SimpleTimer ����, ���� Timer�� IfcTimer�� ��ӹް� ó���Ұ���
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
    // base_interval_ �ð� ������ �״�� �����ϰ� �ٽ� Ÿ�̸Ӹ� ������.
    void Reset(/*base_interval_*/);
    // Ÿ�̸Ӹ� resume
    void EnableCheckTime();
    // Ÿ�̸Ӹ� �����Ѵ�.
    void DisableCheckTime();
    //void IncreasingExpireTime(DWORD dwAddExpireTime);
    // ����Ÿ���� �Ǳ������ ���� �����
    float GetProgressRatio() const;
    // �Ϸ�Ǳ� ���� ���� �ð� (���� ms)
    DWORD GetRemainedTime() const;
    // ����� �ð�
    DWORD GetProcessTime() const;
    // problem : �� �Լ��� ���� ȣ���� ��� IsExpired() == true�� �Ǵ� ��찡 �����.
    // �ֳ��� 1 Tick�̶� ������ ������ �ʱ� ���� IsExpired()ȣ���� �ð��� �������� �ʴ´�.
    // Therefore, �̷���� IsExpired() == true�� �Ŀ� Reset()�� �ѹ� ȣ���� �ش�!
    bool IsExpired(BOOL need_reset = true);
    // �ѹ��� Ÿ�̸Ӹ� ������ ����
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

// base_interval_ �ð� ������ �״�� �����ϰ� �ٽ� Ÿ�̸Ӹ� ������.
inline void Timer::Reset(/*base_interval_*/)
{
    expire_time_ = GetTickCount() + GetIntervalTime();
    EnableCheckTime();
}

// Ÿ�̸Ӹ� resume
inline void Timer::EnableCheckTime()
{
    is_check_expire_ = true;
}

// Ÿ�̸Ӹ� �����Ѵ�.
inline void Timer::DisableCheckTime()
{
    is_check_expire_ = false;
}

//inline void Timer::IncreasingExpireTime(DWORD dwAddExpireTime)
//{
//    expire_time_ += dwAddExpireTime;
//}

// ����Ÿ���� �Ǳ������ ���� �����
inline float Timer::GetProgressRatio() const
{
    int process_ticks = GetTickCount() - (expire_time_ - GetIntervalTime());
    return min(1.0f, (float)process_ticks / GetIntervalTime());
}

// �Ϸ�Ǳ� ���� ���� �ð� (���� ms)
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

// ����� �ð�
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

// problem : �� �Լ��� ���� ȣ���� ��� IsExpired() == true�� �Ǵ� ��찡 �����.
// �ֳ��� 1 Tick�̶� ������ ������ �ʱ� ���� IsExpired()ȣ���� �ð��� �������� �ʴ´�.
// Therefore, �̷���� IsExpired() == true�� �Ŀ� Reset()�� �ѹ� ȣ���� �ش�!
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

// �ѹ��� Ÿ�̸Ӹ� ������ ����
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

