#pragma once

class Fatigue
{
public:
	Fatigue(void);
	~Fatigue(void);

protected:
	int		m_nLimitedTime;				// 플레이 제한시간.(단위 : 분)

public:
	BOOL	IsOverPlayingTime( int nPlayingTime );							// 제한시간을 넘겼는지 체크.
	void	SetLimitedTime( int nLimitedTime );
	int		GetLimitedTime(){ return m_nLimitedTime; }						
};
