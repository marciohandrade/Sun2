// clock.h

#ifndef _PROGRAMCOMMON_WZCLOCK_H_
#define _PROGRAMCOMMON_WZCLOCK_H_

#include "wztypedef.h"

#define CFS_MOVE    (0x0001)
#define CFS_RENDER  (0x0002)

typedef BOOL (__stdcall *FUNC_QueryPerformance)(LARGE_INTEGER*, int*);
typedef unsigned long (__stdcall *FUNC_GetTickCount)(int*);

//------------------------------------------------------------------------------
/**
    @class CClock
*/
class CClock
{
public:
    // 생성자/소멸자
    CClock( void );
    ~CClock( void );

    // 기본적인 값 측정
    void AutoMeasure( DWORD dwMillisecond = 0 );

    // 현재 틱 얻기 (단위 : 틱)
    LONGLONG GetCustomTick( void ) const;
    // 현재 시간 얻기 (단위 : 밀리초)
    DWORD GetTickCount( void ) const;

    // 현재 프레임 시간 얻기 (단위 : 밀리초)
    DWORD GetCurrentTick( void ) const;

    // 밀리초당 틱 수 (단위 : 틱)
    LONGLONG GetCustomTickPerMillisecond( void ) const;

    // 틱을 밀리초로 환산
    DWORD CustomTick2Millisecond( LONGLONG llTick ) const;
    // 밀리초를 틱으로 환산
    LONGLONG Millisecond2CustomTick( DWORD dwTick ) const;

    // 초당 프레임 설정
    BOOL SetFramePerSecond( int iMaxFramePerSecond, 
                            BOOL bEnableRenderOnly = FALSE, 
                            int iMinFramePerSecond = 10,
                            BOOL bFixedFrame = FALSE );

    // 초당 프레임 수 (최대 프레임인 경우)
    int GetFramePerSecond( void ) const;
    // 누적 프레임 수
    long GetFrameCount( void ) const;

    // Move, Render 여부 (매 프레임 호출)
    WORD CheckFrameSkipping( DWORD* pdwTick );

    // 마지막 Move 시간 얻기 (단위 : 밀리초)
    DWORD GetFrameMoveTick( void );
    // 마지막 Render 시간 얻기 (단위 : 밀리초)
    DWORD GetRenderTick( void );
    
    // 마지막 프레임 틱 재설정
    void Reset( void );

    // 평균 프레임 시간 얻기
    DWORD GetAvgFrameTime( void ) const;
    // 평균 렌더링 시간 얻기
    DWORD GetAvgRenderTime( void ) const;

    // 외부 보안타이머 함수포인터 세팅
    void SetClockFunction(FUNC_QueryPerformance pFuncPerformanceCount, 
                          FUNC_QueryPerformance pFuncPerformanceFrequency, 
                          FUNC_GetTickCount pFuncGetTickCount);
    // QueryPerformanceCounter 랩핑함수
    BOOL QueryPerformanceCounterSecure(LARGE_INTEGER* pPerformanceCount) const;
    // QueryPerformanceFrequency 랩핑함수
    BOOL QueryPerformanceFrequencySecure(LARGE_INTEGER* pPerformanceFrequency) const;
    // GetTickCount 랩핑함수
    DWORD GetTickCountSecure() const;
    // 타이머함수 에러코드 얻기
    int GetErrorCode();

    FUNC_QueryPerformance GetFuncPerformanceCount();
    FUNC_QueryPerformance GetFuncPerformanceFrequency();
    FUNC_GetTickCount GetFuncGetTickCount();
    

private:
    // From에서 To까지의 지난 틱 얻기 (To - From)
    DWORD GetPassTick( DWORD dwTickTo, DWORD dwTickFrom ) const;

private:
    enum
    {
        TICK_MAXVALUE = 0xffffffff 
    };

    LONGLONG m_llTicksPerMsec;      // 밀리초당 틱 수 (단위 : 틱)

    LONGLONG m_llTicksPerFrame_Max; // 최대 프레임일때 프레임당 틱 수 (단위 : 틱)
    LONGLONG m_llTicksPerFrame_Min; // 최소 프레임일때 프레임당 틱 수 (단위 : 틱)

    int m_iMaxFramePerSec;          // 초당 최대 프레임 수 (희망 프레임 수)
    int m_iMinFramePerSec;          // 초당 최소 프레임 수

    LONGLONG m_llLatestFrameTick;   // 마지막 프레임 틱 (단위 : 틱)

    DWORD m_dwBaseTime;             // Base 시간 (단위 : 밀리초)
    DWORD m_dwLatestFrameMoveTime;  // 마지막 Move 시간 (단위 : 밀리초)
    DWORD m_dwLatestRenderTime;     // 마지막 Render 시간 (단위 : 밀리초)

    long m_lFrameCount;             // 누적 프레임 수

    float m_fAvgFrameTime;          // 평균 프레임 시간 (단위 : 밀리초)
    float m_fAvgRenderTime;         // 평균 렌더링 시간 (단위 : 밀리초)

    FUNC_QueryPerformance m_pQueryPerformanceCounter;
    FUNC_QueryPerformance m_QueryPerformanceFrequency;
    FUNC_GetTickCount m_pGetRickCount;

    static int s_iErrorCode;
};

//------------------------------------------------------------------------------
/**
    현재 틱 얻기 (단위 : 틱)
*/
inline LONGLONG CClock::GetCustomTick( void ) const
{
    LARGE_INTEGER liCounter;
    QueryPerformanceCounterSecure(&liCounter);

    return ( (UINT64)liCounter.QuadPart );
}

//------------------------------------------------------------------------------
/**
    현재 틱 얻기 (단위 : 밀리초)
*/
inline DWORD CClock::GetTickCount( void ) const
{
    return ( (DWORD)( GetCustomTick() / m_llTicksPerMsec ) );
}

//------------------------------------------------------------------------------
/**
    현재(마지막) 프레임 시간 얻기 - 한 프레임내 동일 (단위 : 밀리초) 
*/
inline DWORD CClock::GetCurrentTick( void ) const
{
    return ( (DWORD)( m_llLatestFrameTick / m_llTicksPerMsec ) );
}

//------------------------------------------------------------------------------
/**
    밀리초당 틱 수 (단위 : 틱)
*/
inline LONGLONG CClock::GetCustomTickPerMillisecond( void ) const
{
    return m_llTicksPerMsec; 
}

//------------------------------------------------------------------------------
/**
    틱을 밀리초로 환산 (단위 : 밀리초)
*/
inline DWORD CClock::CustomTick2Millisecond( LONGLONG llTick ) const
{
    return ( ( DWORD)( llTick / m_llTicksPerMsec ) );
}

//------------------------------------------------------------------------------
/**
    밀리초를 틱으로 환산 (단위 : 틱)
*/
inline LONGLONG CClock::Millisecond2CustomTick( DWORD dwTick ) const
{
    return ( dwTick * m_llTicksPerMsec );
}

//------------------------------------------------------------------------------
/**
    초당 최대(희망) 프레임 수 얻기
*/
inline int CClock::GetFramePerSecond( void ) const
{
    return m_iMaxFramePerSec;
}

//------------------------------------------------------------------------------
/**
    누적 프레임 수 얻기
*/
inline long CClock::GetFrameCount( void ) const
{
    return m_lFrameCount;
}

//------------------------------------------------------------------------------
/**
    From에서 To까지의 지난 틱 얻기
*/
inline DWORD CClock::GetPassTick( DWORD dwTickTo, DWORD dwTickFrom ) const
{
    return ( ( dwTickTo < dwTickFrom ) ? TICK_MAXVALUE - ( dwTickFrom - dwTickTo ) : dwTickTo - dwTickFrom );
}

//------------------------------------------------------------------------------
/**
    마지막 Move 시간 얻기 (단위 : 밀리초)
*/
inline DWORD CClock::GetFrameMoveTick( void )
{
    return ( m_dwLatestFrameMoveTime - m_dwBaseTime );
}

//------------------------------------------------------------------------------
/**
    마지막 Render 시간 얻기 (단위 : 밀리초)
*/
inline DWORD CClock::GetRenderTick( void )
{
    return ( m_dwLatestRenderTime - m_dwBaseTime );
}

//------------------------------------------------------------------------------
/**
    마지막 프레임 틱 재설정
*/
inline void CClock::Reset( void )
{
    m_llLatestFrameTick = GetCustomTick();
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD CClock::GetAvgFrameTime( void ) const
{
    return (DWORD)m_fAvgFrameTime;
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD CClock::GetAvgRenderTime( void ) const
{
    return (DWORD)m_fAvgRenderTime;
}

inline void CClock::SetClockFunction(FUNC_QueryPerformance pFuncPerformanceCount, 
                                     FUNC_QueryPerformance pFuncPerformanceFrequency, 
                                     FUNC_GetTickCount pFuncGetTickCount)
{
    m_pQueryPerformanceCounter = pFuncPerformanceCount;
    m_QueryPerformanceFrequency = pFuncPerformanceFrequency;
    m_pGetRickCount = pFuncGetTickCount;

    AutoMeasure();
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CClock::QueryPerformanceCounterSecure(LARGE_INTEGER *pPerformanceCount) const
{
    if (m_pQueryPerformanceCounter != NULL)
    {
        return m_pQueryPerformanceCounter(pPerformanceCount, &s_iErrorCode);
    }
    return ::QueryPerformanceCounter(pPerformanceCount);
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL CClock::QueryPerformanceFrequencySecure(LARGE_INTEGER *pPerformanceFrequency) const
{
    if (m_QueryPerformanceFrequency != NULL)
    {
        return m_QueryPerformanceFrequency(pPerformanceFrequency, &s_iErrorCode);
    }
    return ::QueryPerformanceFrequency(pPerformanceFrequency);
}

//------------------------------------------------------------------------------
/**
*/
inline DWORD CClock::GetTickCountSecure() const
{
    if (m_pGetRickCount != NULL)
    {
        return m_pGetRickCount(&s_iErrorCode);
    }
    return ::GetTickCount();
}

//------------------------------------------------------------------------------
/**
*/
inline int CClock::GetErrorCode()
{
    return s_iErrorCode;
}

//------------------------------------------------------------------------------
/**
*/
inline FUNC_QueryPerformance CClock::GetFuncPerformanceCount()
{
    return m_pQueryPerformanceCounter;
}

//------------------------------------------------------------------------------
/**
*/
inline FUNC_QueryPerformance CClock::GetFuncPerformanceFrequency()
{
    return m_QueryPerformanceFrequency;
}

//------------------------------------------------------------------------------
/**
*/
inline FUNC_GetTickCount CClock::GetFuncGetTickCount()
{
    return m_pGetRickCount;
}

#endif // _PROGRAMCOMMON_WZCLOCK_H_