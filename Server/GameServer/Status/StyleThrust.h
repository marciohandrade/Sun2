#pragma once

#include "EtcStatus.h"


class StyleThrust : public EtcStatus
{
public:
	StyleThrust() :m_wDownTimeAfterThrust(0) {}
	~StyleThrust() {}

	virtual VOID		Init( Character *pOwner, WORD wStateID, int iApplicationTime, int iPeriod );

	virtual VOID		Start();
	virtual VOID		End();
	virtual VOID		SetDownTime( WORD wDwonTime )	{ m_wDownTimeAfterThrust = wDwonTime; }

private:
	WORD		m_wDownTimeAfterThrust;		// 밀리기가 끝난 후에 다운되는 시간
};


