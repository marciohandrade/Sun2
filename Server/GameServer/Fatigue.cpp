#include "StdAfx.h"
#include ".\fatigue.h"

Fatigue::Fatigue(void)
{
	m_nLimitedTime = 0;
}

Fatigue::~Fatigue(void)
{
}

BOOL	Fatigue::IsOverPlayingTime( int nPlayingTime )		// ���ѽð��� �Ѱ���� üũ.
{
	if( m_nLimitedTime == 0 )	//���� ������ �Ѱ�ð��� 0�̸� �� �̻� �������� �ʴ°ɷ� üũ�Ѵ�.
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