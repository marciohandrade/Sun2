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
	WORD		m_wDownTimeAfterThrust;		// �и��Ⱑ ���� �Ŀ� �ٿ�Ǵ� �ð�
};


