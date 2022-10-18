#include "StdAfx.h"
#include ".\fatigue.h"

Fatigue::Fatigue(void)
{
	m_nLimitedTime = 0;
}

Fatigue::~Fatigue(void)
{
}

BOOL	Fatigue::IsOverPlayingTime( int nPlayingTime )		// 제한시간을 넘겼는지 체크.
{
	if( m_nLimitedTime == 0 )	//만일 설정된 한계시간이 0이면 더 이상 적용하지 않는걸로 체크한다.
		return FALSE;

	if( m_nLimitedTime <= nPlayingTime )
		return TRUE;
	else 
		return FALSE;
}

void	Fatigue::SetLimitedTime( int nLimitedTime )
{
	if( nLimitedTime < 0 )
		nLimitedTime = 0;

	m_nLimitedTime = nLimitedTime; 
}