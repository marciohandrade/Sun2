#ifndef __UTILITY_ITIMEOUT_H_
#define __UTILITY_ITIMEOUT_H_
#pragma once

//==================================================================================================
// version control
#define __ITIMERBASE_VERSION_CRTL	(0x0120)
//==================================================================================================

//==================================================================================================
//==================================================================================================
//==================================================================================================
#if __ITIMERBASE_VERSION_CRTL < 0x0120
//==================================================================================================
//==================================================================================================
//==================================================================================================


namespace util { namespace nsOldVer {
#pragma pack(push)
#pragma pack()

//==================================================================================================
// <TIMEOUT OBJECT>
//   단순 Timeout 처리를 목적으로 한다.
//
// <Example>
//	ITimeout timeout;
//	timeout.IsExpired()									==> 이 시점에서는 항상 FALSE (무한값으로 설정)
//	timeout.SetNextTimeout( GetTickCount() + 3000 )		==> 3초 뒤 IsExpired() == TRUE
//	timeout.SetNextTimeoutByCurTickBased( 3000 )		==> 3초 뒤 IsExpired() == TRUE
//	timeout.Instance()									==> 바로 IsExpired() 해도 TRUE
//	timeout.Disabled()									==> 시스템 점검 기간내 항상 IsExpired() == FALSE
class ITimeout_v01xx
{
public:
	static const DWORD	UNLIMITED = (DWORD)(-1);
	static const DWORD	INSTANCE = 0;

	ITimeout_v01xx() : m_dwTimeoutTick(UNLIMITED) {}
	//================================================================================
	// <INTERFACEs>
public:
	inline BOOL			IsExpired()
	{
		DWORD dwCurTick = GetTickCount();
		if( !(dwCurTick < TimeoutTick()) )
			return TRUE;
		return FALSE;
	}

	inline BOOL			IsDisabled()	{ return (BOOL)( ITimeout_v01xx::UNLIMITED == TimeoutTick() ); }
	inline VOID			Disabled()		{ TimeoutTick() = ITimeout_v01xx::UNLIMITED; }
	inline VOID			Instance()		{ TimeoutTick() = ITimeout_v01xx::INSTANCE; }

	inline VOID			SetNextTimeout( DWORD dwTimeoutTick ) { m_dwTimeoutTick = dwTimeoutTick; }
	inline VOID			SetNextTimeoutByCurTickBased( DWORD dwTimeoutTick ) { m_dwTimeoutTick = GetTickCount() + dwTimeoutTick; }

	inline DWORD&		TimeoutTick() { return m_dwTimeoutTick; }
private:
	DWORD				m_dwTimeoutTick;
};



//==================================================================================================
// <TIMER OBJECT> 9Byte
//		기존 사용 중인 Timer Refactoring 중...
//		이곳 저곳에 흩어져 있는 다양한 형태의 Timer들도 정리할 예정
class ITimerBase_v01xx : public ITimeout_v01xx
{
public:
	enum eTIMER_CONTROL {
#if __ITIMERBASE_VERSION_CRTL == 0x0100
		CLEAR = 0x00000000, START = 0x40000000, STOP = 0x80000000, CONTROL_MASK = 0xC0000000,
#elif __ITIMERBASE_VERSION_CRTL == 0x0110
		START = 0x1, STOP = 0x0/*Toggle START*/, CONTROL_MASK = 0x1,
#endif
	};
	enum eTIMER_MODE {
#if __ITIMERBASE_VERSION_CRTL == 0x0100
		NONE = 0x00000000, ONETIME = 0x10000000, LOOP = 0x20000000, MODE_MASK = 0x30000000,
#elif __ITIMERBASE_VERSION_CRTL == 0x0110
		LOOP = 0x2, ONETIME = 0x0/*Toggle LOOP*/, MODE_MASK = 0x2,
#endif
	};
#if __ITIMERBASE_VERSION_CRTL == 0x0100
	static const DWORD INTERVAL_FIELD = 0x0FFFFFFF;			//	268435455
	static const DWORD CONTROL_FIELD = 0xF0000000;
#elif __ITIMERBASE_VERSION_CRTL == 0x0110
	static const DWORD CONTROL_FIELD	= CONTROL_MASK|MODE_MASK;
	static const DWORD INTERVAL_FIELD	= ~CONTROL_FIELD;	// 4294967292
#endif

	ITimerBase_v01xx() : ITimeout_v01xx(), m_Interval(0) { ClearTimer(); }
	//================================================================================
	// <INTERFACEs>
public:
	inline VOID			ClearTimer()
	{
		Disabled();
		SetInterval( 0 );
		ClearControlField();
	}

	inline VOID			ResetTimer()
	{
		if( LOOP == GetTimerMode() )
		{
			SetNextTimeoutByCurTickBased( GetInterval() );
			return;
		}
		//case ONETIME:
		//case NONE:
		if( START == GetTimerControl() )
		{
			Disabled();
			SetTimerControl( STOP );
		}
		else
		{
			SetNextTimeoutByCurTickBased( GetInterval() );
			SetTimerControl( START );
		}
	}

	inline BOOL			Configure( eTIMER_MODE eMode, DWORD dwInterval, BOOL bStart = TRUE )
	{
		switch( eMode )
		{
		case ONETIME:
		case LOOP:
			SetInterval( dwInterval );
			SetTimerMode( eMode );
			SetTimerControl( STOP );
			if( bStart )	StartTimer();
			else			StopTimer();

			return TRUE;
		};
		ClearTimer();
		return FALSE;
	}

	inline VOID			SetTimer( DWORD dwIntervalTime )	// ITimer Correspond Interface : auto LOOP timer
	{
		Configure( ITimerBase_v01xx::LOOP, dwIntervalTime );
	}

	inline BOOL			IsStarted() { return (BOOL)(START == GetTimerControl()); }
	inline BOOL			IsStopped() { return (BOOL)(STOP == GetTimerControl()); }

	inline BOOL			StartTimer()
	{
		if( IsStopped() )
		{
			ResetTimer();
			SetTimerControl( START );
			return TRUE;
		}
		return FALSE;
	}

	inline VOID			StopTimer() { ResetTimer(); SetTimerControl( STOP ); }

	inline BOOL			ResumeTimer()
	{
		eTIMER_MODE eMode = GetTimerMode();
		if( LOOP == eMode )
			return StartTimer();
		StopTimer();
		return FALSE;
	}

	inline eTIMER_MODE		GetTimerMode() { return (eTIMER_MODE)(MODE_MASK&m_Interval); }
	inline eTIMER_CONTROL	GetTimerControl() { return (eTIMER_CONTROL)(CONTROL_MASK&m_Interval); }

	inline BOOL				IsExpired()
	{
		BOOL bRet = (!IsStopped() && ITimeout_v01xx::IsExpired());
		if( bRet )
			ResetTimer();

		return bRet;
	}

	inline DWORD			GetInterval() { return INTERVAL_FIELD&m_Interval ; }
	inline VOID				SetInterval( DWORD interval )
	{
#if __ITIMERBASE_VERSION_CRTL == 0x0100
		m_Interval = (CONTROL_FIELD&m_Interval) | (INTERVAL_FIELD&interval) ;
#elif __ITIMERBASE_VERSION_CRTL == 0x0110
		const DWORD interval_ip = interval & (~CONTROL_FIELD);
		m_Interval = (CONTROL_FIELD&m_Interval) | ( interval&CONTROL_FIELD ? (interval_ip)+(CONTROL_FIELD+1) : (interval_ip) ) ;
#endif
	}

	//================================================================================
	// <INTERFACEs><INTERNAL>
protected:
	inline VOID				SetTimerMode( eTIMER_MODE eMode )
	{ m_Interval = (m_Interval&CONTROL_MASK) | (eMode&MODE_MASK) | GetInterval(); }
	inline VOID				SetTimerControl( eTIMER_CONTROL eControl )
	{ m_Interval = (eControl&CONTROL_MASK) | (m_Interval&MODE_MASK) | GetInterval(); }

	inline VOID				ClearControlField() { m_Interval = GetInterval(); }
private:
	DWORD					m_Interval;
};

#pragma pack(pop)
}}; //end of namespace


//==================================================================================================
//==================================================================================================
//==================================================================================================
#elif __ITIMERBASE_VERSION_CRTL == 0x0120
//==================================================================================================
//==================================================================================================
//==================================================================================================


namespace util { namespace internal {
#pragma pack(push)
#pragma pack()

//------------------------------------------------------------------------------
//  ()
//    - 09/12/12 : migration (version 0x0120->0x0121)
//  @name  TimeBaseInfo
//  @code  
struct TimeBaseInfo
{
    enum {
    };
    enum eCtrl {
        eCtrl_Start = 0x1, // opp. Stop
        eCtrl_Stop  = 0x0,
        eCtrl_Mask  = 0x1,
    };
    enum eMode {
        eMode_None        = 0,
        eMode_ManualReset = 0,
        eMode_AutoReset   = 2, // opp. ManualReset
        eMode_Mask        = 2,
    };

    static const ulong Version        = 0x0121;
    static const ulong ControlField   = eCtrl_Mask | eMode_Mask ;
    static const ulong IntervalField  = ~ControlField;
};

#pragma pack(pop)
}}; //end of namespace

//==============================================================================
//  @WxNote (util::ITimeout)
//  @Author  : Waverix
//  @Date  : 2008-12-04
//  @Reference
//  @History
//    - 07/08/12 : version 0100 update
//    - 08/07/09 : version 0110 update, timeout interval bitfield modification
//for ITimerBase
//    - 08/12/04 : migration (ITimeout -> Timeout) and revision
//    - 09/11/02 : change timeout_tick_ time accuracy (4ms>error)
//    - 09/12/12 : migrate Timeout to ITimeout for SUN utility (version 0x0120->0x0121)
//==============================================================================
namespace util {
#pragma pack(push)
#pragma pack()

//------------------------------------------------------------------------------
//  ()
//  @name  ITimeout
//  @brief
//    elapsed timer by 'IsExpired'
//  @code  
class ITimeout : public internal::TimeBaseInfo
{
public:
    static const ulong Version    = 0x0121;
    static const ulong UNLIMITED  = ULONG_MAX;
    static const ulong INSTANCE   = 0;

    ITimeout();
    //  (Check)
    bool IsDisabled() const;
    bool IsExpired() const;
    //  (Control Action)
    void Disabled();
    void Instance();

    //  (timeout value control)
    void SetNextTimeout(ulong next_timeout);
    void SetNextTimeoutByCurTickBased(ulong interval);

    ulong GetTimeoutTicks() const;

private:
    // spec. timeout_tick_count = timeout_tick_ << 2
    //       timeout_tick = (GetTickCount() + next_interval) >> 2;
    ulong timeout_tick_;
};

//==============================================================================

inline
ITimeout::ITimeout()
    : timeout_tick_(UNLIMITED)
{
}

//  (Check)
inline bool
ITimeout::IsDisabled() const {
    return UNLIMITED == timeout_tick_;
}

inline bool
ITimeout::IsExpired() const {
    ulong current = GetTickCount();
    current >>= 2;
    if (timeout_tick_ < current) {
        return true;
    }
    return false;
}

//  (Control Action)
inline void
ITimeout::Disabled() {
    timeout_tick_ = UNLIMITED;
}

inline void
ITimeout::Instance() {
    timeout_tick_ = INSTANCE;
}

//  (timeout value control)
inline void
ITimeout::SetNextTimeout(ulong next_timeout) {
    //if ((next_timeout & 3) == next_timeout)
    //  next_timeout = 4;
    next_timeout >>= 2;

    timeout_tick_ = next_timeout;
}

inline void
ITimeout::SetNextTimeoutByCurTickBased(ulong interval) {
    if ((interval & 3) == interval) {
        interval = 4;
    }
    interval >>= 2;

    ulong current = GetTickCount();
    timeout_tick_ = (current >> 2) + interval;
}

inline ulong
ITimeout::GetTimeoutTicks() const {
    return timeout_tick_ << 2;
}

#pragma pack(pop)
}; //end of namespace

//==============================================================================
//  @WxNote (util::ITimerBase)
//  @Author  : waverix
//  @Date  : 2008-12-04
//  @Reference
//  @History
//    - 07/08/12 : version 0100 update
//    - 08/07/09 : version 0110 update, timeout interval bitfield modification for ITimerBase
//    - 08/12/04 : migration (ITimerBase -> TimerBase) and revision
//    - 09/11/02 : update to v0120, GetTickCount overflow tolerant algorithm
//    - 09/12/12 : migrate TimerBase to ITimerBase for SUN utility (version 0x0120->0x0121)
//==============================================================================
namespace util {
#pragma pack(push)
#pragma pack()

//------------------------------------------------------------------------------
//  ()
//  @name  ITimerBase
//  spec:  8 bytes object
//  @brief
//    elapsed timer by 'IsExpired'
//  @accuracy
//    min: 4ms
//  @code  
class ITimerBase : public internal::TimeBaseInfo
{
public:
    static const ulong Version = 0x0121;

    ITimerBase();
    void Clear();
    void Reset() const;
    bool SetTimer(ulong interval, bool start = true, bool auto_reset = true);
    // CHANGES: f110216.4L, add the interface to support cool time concept.
    void ChangeToInstantExpiration();

    bool IsAutoReset() const;
    bool IsStarted() const;
    bool IsStopped() const;
    bool Start();
    bool Stop();
    bool Resume();

    bool IsExpired() const;
    ulong GetRemainTicks() const;
    ulong GetTicks() const;
    ulong GetTimeoutTicks() const;
    ulong GetInterval() const;
    // interfaces to support a game timer
    float GetProgressRatio() const;
    ulong GetRemaindTime() const;
    ulong GetProcessTime() const;

    void SetInterval(ulong interval);

protected:
    void ClearControlField();

private:
    #pragma warning(push)
    #pragma warning(disable:4201)
    union {
        ulong data_;
        struct {
            ulong is_start_ : 1;
            ulong is_auto_  : 1;
            // interval section : data >> 2
            // interval : ((data >> 2) << 4) or (interval_ << 2)
            ulong interval_ : 30;
        };
    };
    #pragma warning(pop)
    ulong checked_;
};

//==============================================================================

inline
ITimerBase::ITimerBase()
    : data_(0)
    , checked_(0)
{
}

inline bool ITimerBase::IsAutoReset() const {
    return (is_auto_ != 0);
}

inline bool
ITimerBase::IsStarted() const {
    return is_start_;
}

inline bool
ITimerBase::IsStopped() const {
    return is_start_ == 0;
}

inline ulong
ITimerBase::GetTicks() const {
    return checked_;
}

inline ulong
ITimerBase::GetTimeoutTicks() const {
    const ulong next_timeout = checked_ + interval_;
    ulong modified = next_timeout << 2;
    if ((modified >> 2) == next_timeout) {
        return modified;
    }
    return ULONG_MAX;
}

inline ulong
ITimerBase::GetInterval() const {
    ulong interval = interval_;
    return interval << 2;
}

inline void
ITimerBase::SetInterval(ulong interval) {
    if ((interval & 3) == interval) {
        interval = 4;
    }
    interval >>= 2;
    interval_ = interval;
}

// interfaces to support a game timer
inline ulong ITimerBase::GetRemaindTime() const {
    return GetRemainTicks();
}

inline void
ITimerBase::Clear()
{  //  don't action anymore
    data_ = 0; // -> is_start_ == 0, is_auto_ == 0
    checked_ = 0;
};

inline void
ITimerBase::Reset() const
{
    //  LOOP 모드일 경우 다음 타임아웃 설정,
    //  INSTANCE일 경우는 START 여부에 따라서 달리 설정됨.
    //  eMode_AutoReset ? set next_timeout
    //  instance -> variation setup by Start state
    if (is_auto_ || is_start_ == 0) {
        ulong current = GetTickCount();
        const_cast<ITimerBase*>(this)->checked_ = current >> 2;
    }
    else if (is_start_ != 0) {
        const_cast<ITimerBase*>(this)->is_start_ = 0;
    }
}

// CHANGES: f110216.4L, add the interface to support cool time concept.
inline void ITimerBase::ChangeToInstantExpiration()
{
    checked_ = 0;
}

inline bool
ITimerBase::Start()
{
    if (is_start_) {
        return false;
    }
    Reset();
    is_start_ = true;
    return true;
}

inline bool
ITimerBase::Stop()
{
    Reset();
    is_start_ = false;
    return true;
}

inline bool
ITimerBase::Resume()
{
    if (is_auto_) {
        return Start();
    }
    Stop();
    return false;
}

inline void
ITimerBase::ClearControlField() {
    data_ = (data_ >> 2) << 2;
}

#pragma pack(pop)
}; //end of namespace

//==================================================================================================
//==================================================================================================
//==================================================================================================
#endif //__ITIMERBASE_VERSION_CRTL == 0x0120
//==================================================================================================
//==================================================================================================
//==================================================================================================







//==================================================================================================
//==================================================================================================
//==================================================================================================

#if __ITIMERBASE_VERSION_CRTL < 0x0120
    typedef util::nsOldVer::ITimeout_v01xx    ITimeout;
    typedef util::nsOldVer::ITimerBase_v01xx  ITimerBase;
#elif __ITIMERBASE_VERSION_CRTL == 0x0120
    using util::ITimeout;
    using util::ITimerBase;
#endif

#endif //__UTILITY_ITIMEOUT_H_


