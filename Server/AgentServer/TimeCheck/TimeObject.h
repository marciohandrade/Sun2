
#pragma once

#include <ITimeout.h>
#include <Timer.h>

class TimeObject
{
	enum eTIME_STATE { TS_NONE, TS_START, TS_END, TS_STOP, };
	friend class TimeKeeper;
public:
	TimeObject();
	virtual ~TimeObject();

	virtual VOID		OnStart(){}
	virtual VOID		OnExpired(){}
	
	VOID				Start( DWORD dwIntervalTime );
	VOID				Stop();
	VOID				Resume();
	VOID				Reset();
	VOID				ForceEnd();

	
private:
	VOID				_calcElapsedTime();
	VOID				_update();

	DWORD				m_dwIntervalTime;
	DWORD				m_dwOldTime;
	DWORD				m_dwElapsedTime;
	eTIME_STATE			m_eTimeState;
};
