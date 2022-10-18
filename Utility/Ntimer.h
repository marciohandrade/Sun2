#ifndef N_TIMER_H
#define N_TIMER_H
//------------------------------------------------------------------------------
/**
    @brief Ÿ�̸�

    @author
    - ���� 

    @since
    - 2005.6.30
    @remarks
    - �۾� ��!
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
    ������ 
*/
inline nTimer::nTimer():m_xEnable(true) 
{
    Clear(); 
    m_init = true; 
}

//------------------------------------------------------------------------------
/**
    �Ҹ��� 
*/
inline ~nTimer() 
{
    Clear();
    m_init = false;
}

//------------------------------------------------------------------------------
/**
    Ÿ�̸� ���� 
*/
inline void	nTimer::Reset() 
{
    Stop();
}

//------------------------------------------------------------------------------
/**
    ������ �ð��� �ٵƳ� �˻� 
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
    Ÿ�̸� Ȱ��ȭ �˻� 
*/
inline bool	nTimer::IsEnable() 
{
    return m_enable;
}

//------------------------------------------------------------------------------
/**
    Ÿ�̸� ��ŸƮ 
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
    Ÿ�̸� ���� 
*/
inline void nTimer::Stop()
{
	if(!m_enable)
		return;

	Clear();
}


//------------------------------------------------------------------------------
/**
    Ȱ��ȭ ���� 
*/
inline void	nTimer::SetEnable(bool enable) 
{
    m_enable = enable;
}

//------------------------------------------------------------------------------
/**
    ���۽ð����� ���� �帥 �ð� 
*/
inline float nTimer::Get() 
{
    //return (m_kernelServer->GetTimeServer()->GetTime() - m_dwStartTime);

    return 0.0f;
}

//------------------------------------------------------------------------------
/**
    dwTime ���� ��ȣ�� �߻� ��Ŵ 
*/
inline float nTimer::Signal(float dwTime) 
{
    float rt=(Get()/dwTime);
    rt=(DWORD)rt%2;

    return rt;
}

//------------------------------------------------------------------------------
/**
    �Ҹ� �ð��� ���� 
*/
inline float nTimer::GetEndTime() 
{
    return (m_dwEndTime - m_dwStartTime);
}
}

//------------------------------------------------------------------------------
/**
    ������ �ʱ�ȭ 
*/
inline void nTimer::Clear()
{
    m_dwStartTime = 0;
    m_dwEndTime = 0;
}

#endif