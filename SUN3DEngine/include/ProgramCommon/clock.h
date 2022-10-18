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
    // ������/�Ҹ���
    CClock( void );
    ~CClock( void );

    // �⺻���� �� ����
    void AutoMeasure( DWORD dwMillisecond = 0 );

    // ���� ƽ ��� (���� : ƽ)
    LONGLONG GetCustomTick( void ) const;
    // ���� �ð� ��� (���� : �и���)
    DWORD GetTickCount( void ) const;

    // ���� ������ �ð� ��� (���� : �и���)
    DWORD GetCurrentTick( void ) const;

    // �и��ʴ� ƽ �� (���� : ƽ)
    LONGLONG GetCustomTickPerMillisecond( void ) const;

    // ƽ�� �и��ʷ� ȯ��
    DWORD CustomTick2Millisecond( LONGLONG llTick ) const;
    // �и��ʸ� ƽ���� ȯ��
    LONGLONG Millisecond2CustomTick( DWORD dwTick ) const;

    // �ʴ� ������ ����
    BOOL SetFramePerSecond( int iMaxFramePerSecond, 
                            BOOL bEnableRenderOnly = FALSE, 
                            int iMinFramePerSecond = 10,
                            BOOL bFixedFrame = FALSE );

    // �ʴ� ������ �� (�ִ� �������� ���)
    int GetFramePerSecond( void ) const;
    // ���� ������ ��
    long GetFrameCount( void ) const;

    // Move, Render ���� (�� ������ ȣ��)
    WORD CheckFrameSkipping( DWORD* pdwTick );

    // ������ Move �ð� ��� (���� : �и���)
    DWORD GetFrameMoveTick( void );
    // ������ Render �ð� ��� (���� : �и���)
    DWORD GetRenderTick( void );
    
    // ������ ������ ƽ �缳��
    void Reset( void );

    // ��� ������ �ð� ���
    DWORD GetAvgFrameTime( void ) const;
    // ��� ������ �ð� ���
    DWORD GetAvgRenderTime( void ) const;

    // �ܺ� ����Ÿ�̸� �Լ������� ����
    void SetClockFunction(FUNC_QueryPerformance pFuncPerformanceCount, 
                          FUNC_QueryPerformance pFuncPerformanceFrequency, 
                          FUNC_GetTickCount pFuncGetTickCount);
    // QueryPerformanceCounter �����Լ�
    BOOL QueryPerformanceCounterSecure(LARGE_INTEGER* pPerformanceCount) const;
    // QueryPerformanceFrequency �����Լ�
    BOOL QueryPerformanceFrequencySecure(LARGE_INTEGER* pPerformanceFrequency) const;
    // GetTickCount �����Լ�
    DWORD GetTickCountSecure() const;
    // Ÿ�̸��Լ� �����ڵ� ���
    int GetErrorCode();

    FUNC_QueryPerformance GetFuncPerformanceCount();
    FUNC_QueryPerformance GetFuncPerformanceFrequency();
    FUNC_GetTickCount GetFuncGetTickCount();
    

private:
    // From���� To������ ���� ƽ ��� (To - From)
    DWORD GetPassTick( DWORD dwTickTo, DWORD dwTickFrom ) const;

private:
    enum
    {
        TICK_MAXVALUE = 0xffffffff 
    };

    LONGLONG m_llTicksPerMsec;      // �и��ʴ� ƽ �� (���� : ƽ)

    LONGLONG m_llTicksPerFrame_Max; // �ִ� �������϶� �����Ӵ� ƽ �� (���� : ƽ)
    LONGLONG m_llTicksPerFrame_Min; // �ּ� �������϶� �����Ӵ� ƽ �� (���� : ƽ)

    int m_iMaxFramePerSec;          // �ʴ� �ִ� ������ �� (��� ������ ��)
    int m_iMinFramePerSec;          // �ʴ� �ּ� ������ ��

    LONGLONG m_llLatestFrameTick;   // ������ ������ ƽ (���� : ƽ)

    DWORD m_dwBaseTime;             // Base �ð� (���� : �и���)
    DWORD m_dwLatestFrameMoveTime;  // ������ Move �ð� (���� : �и���)
    DWORD m_dwLatestRenderTime;     // ������ Render �ð� (���� : �и���)

    long m_lFrameCount;             // ���� ������ ��

    float m_fAvgFrameTime;          // ��� ������ �ð� (���� : �и���)
    float m_fAvgRenderTime;         // ��� ������ �ð� (���� : �и���)

    FUNC_QueryPerformance m_pQueryPerformanceCounter;
    FUNC_QueryPerformance m_QueryPerformanceFrequency;
    FUNC_GetTickCount m_pGetRickCount;

    static int s_iErrorCode;
};

//------------------------------------------------------------------------------
/**
    ���� ƽ ��� (���� : ƽ)
*/
inline LONGLONG CClock::GetCustomTick( void ) const
{
    LARGE_INTEGER liCounter;
    QueryPerformanceCounterSecure(&liCounter);

    return ( (UINT64)liCounter.QuadPart );
}

//------------------------------------------------------------------------------
/**
    ���� ƽ ��� (���� : �и���)
*/
inline DWORD CClock::GetTickCount( void ) const
{
    return ( (DWORD)( GetCustomTick() / m_llTicksPerMsec ) );
}

//------------------------------------------------------------------------------
/**
    ����(������) ������ �ð� ��� - �� �����ӳ� ���� (���� : �и���) 
*/
inline DWORD CClock::GetCurrentTick( void ) const
{
    return ( (DWORD)( m_llLatestFrameTick / m_llTicksPerMsec ) );
}

//------------------------------------------------------------------------------
/**
    �и��ʴ� ƽ �� (���� : ƽ)
*/
inline LONGLONG CClock::GetCustomTickPerMillisecond( void ) const
{
    return m_llTicksPerMsec; 
}

//------------------------------------------------------------------------------
/**
    ƽ�� �и��ʷ� ȯ�� (���� : �и���)
*/
inline DWORD CClock::CustomTick2Millisecond( LONGLONG llTick ) const
{
    return ( ( DWORD)( llTick / m_llTicksPerMsec ) );
}

//------------------------------------------------------------------------------
/**
    �и��ʸ� ƽ���� ȯ�� (���� : ƽ)
*/
inline LONGLONG CClock::Millisecond2CustomTick( DWORD dwTick ) const
{
    return ( dwTick * m_llTicksPerMsec );
}

//------------------------------------------------------------------------------
/**
    �ʴ� �ִ�(���) ������ �� ���
*/
inline int CClock::GetFramePerSecond( void ) const
{
    return m_iMaxFramePerSec;
}

//------------------------------------------------------------------------------
/**
    ���� ������ �� ���
*/
inline long CClock::GetFrameCount( void ) const
{
    return m_lFrameCount;
}

//------------------------------------------------------------------------------
/**
    From���� To������ ���� ƽ ���
*/
inline DWORD CClock::GetPassTick( DWORD dwTickTo, DWORD dwTickFrom ) const
{
    return ( ( dwTickTo < dwTickFrom ) ? TICK_MAXVALUE - ( dwTickFrom - dwTickTo ) : dwTickTo - dwTickFrom );
}

//------------------------------------------------------------------------------
/**
    ������ Move �ð� ��� (���� : �и���)
*/
inline DWORD CClock::GetFrameMoveTick( void )
{
    return ( m_dwLatestFrameMoveTime - m_dwBaseTime );
}

//------------------------------------------------------------------------------
/**
    ������ Render �ð� ��� (���� : �и���)
*/
inline DWORD CClock::GetRenderTick( void )
{
    return ( m_dwLatestRenderTime - m_dwBaseTime );
}

//------------------------------------------------------------------------------
/**
    ������ ������ ƽ �缳��
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