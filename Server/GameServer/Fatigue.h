#pragma once

class Fatigue
{
public:
	Fatigue(void);
	~Fatigue(void);

protected:
	int		m_nLimitedTime;				// �÷��� ���ѽð�.(���� : ��)

public:
	BOOL	IsOverPlayingTime( int nPlayingTime );							// ���ѽð��� �Ѱ���� üũ.
	void	SetLimitedTime( int nLimitedTime );
	int		GetLimitedTime(){ return m_nLimitedTime; }						
};
