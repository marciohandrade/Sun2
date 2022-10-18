#ifndef N_TIMER_H
#define N_TIMER_H
//------------------------------------------------------------------------------
/**
    @brief 타이머

    @author
    - 지완 

    @since
    - 2005.6.30
    @remarks
    - 작업 중!
*/

class nTimer 
{
public:
	nTimer();
	~nTimer();

public:
    void        Start(const float dwTime);
	void	    Stop();
	void	    Reset();
    bool	    IsTime(); 
	bool	    IsEnable();
	void	    SetEnable(bool enable);
	float	    Get();
	float	    Signal(float dwTime);
	float	    GetEndTime();

private:
	void        Clear();

private:	
	bool	    m_enable;
	bool        m_init;
    float	    m_dwStartTime;
	float	    m_dwEndTime;
};

//------------------------------------------------------------------------------
/**
    생성자 
*/
inline nTimer::nTimer():m_xEnable(true) 
{
    Clear(); 
    m_init = true; 
}

//------------------------------------------------------------------------------
/**
    소멸자 
*/
inline ~nTimer() 
{
    Clear();
    m_init = false;
}

//------------------------------------------------------------------------------
/**
    타이머 리셋 
*/
inline void	nTimer::Reset() 
{
    Stop();
}

//------------------------------------------------------------------------------
/**
    정해진 시간이 다됐나 검사 
*/
inline bool	nTimer::IsTime()
{
/*    if(m_enable) 
        return (m_kernelServer->GetTimeServer()->GetTime()>= m_dwEndTime); 
    else */
        return false;
}

//------------------------------------------------------------------------------
/**
    타이머 활성화 검사 
*/
inline bool	nTimer::IsEnable() 
{
    return m_enable;
}

//------------------------------------------------------------------------------
/**
    타이머 스타트 
*/
inline void nTimer::Start(float dwTime)
{
	if(!m_enable)
		return;
	
    if(m_dwStartTime)
		return;

	//m_dwStartTime = m_kernelServer->GetTimeServer()->GetTime();
	m_dwEndTime = m_dwStartTime + dwTime;
}

//------------------------------------------------------------------------------
/**
    타이머 정지 
*/
inline void nTimer::Stop()
{
	if(!m_enable)
		return;

	Clear();
}


//------------------------------------------------------------------------------
/**
    활성화 세팅 
*/
inline void	nTimer::SetEnable(bool enable) 
{
    m_enable = enable;
}

//------------------------------------------------------------------------------
/**
    시작시간으로 부터 흐른 시간 
*/
inline float nTimer::Get() 
{
    //return (m_kernelServer->GetTimeServer()->GetTime() - m_dwStartTime);

    return 0.0f;
}

//------------------------------------------------------------------------------
/**
    dwTime 마다 신호를 발생 시킴 
*/
inline float nTimer::Signal(float dwTime) 
{
    float rt=(Get()/dwTime);
    rt=(DWORD)rt%2;

    return rt;
}

//------------------------------------------------------------------------------
/**
    소멸 시간을 구함 
*/
inline float nTimer::GetEndTime() 
{
    return (m_dwEndTime - m_dwStartTime);
}
}

//------------------------------------------------------------------------------
/**
    데이터 초기화 
*/
inline void nTimer::Clear()
{
    m_dwStartTime = 0;
    m_dwEndTime = 0;
}

#endif