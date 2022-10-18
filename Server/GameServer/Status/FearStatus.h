#ifndef	__FEARSTATUS_H__
#define __FEARSTATUS_H__

#include "AbilityStatus.h"

// 피어 상태
class FearStatus : public AbilityStatus
{
public:		FearStatus() {}
			~FearStatus() {}
public:
	virtual	VOID			Start();
	virtual	VOID			Execute() {}

	virtual	BOOL			Update( DWORD curTime );
	virtual	VOID			End();
protected:	VOID			RunAway();

private:
	static const DWORD		UPDATE_INTERVAL	= 1000;
private:	ITimerBase		m_UpdateTick;

private:	WzVector		m_wvRotatedVector;
			WzVector		m_vTargetPos;
			BYTE			m_bTouchedSomething;
};


#endif


