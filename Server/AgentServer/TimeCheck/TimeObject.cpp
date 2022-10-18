#include "stdafx.h"
#include ".\timeobject.h"

TimeObject::TimeObject()
	:	m_eTimeState ( TS_NONE ),
		m_dwIntervalTime ( 0 ),
		m_dwOldTime ( 0 ),
		m_dwElapsedTime ( 0 )
{

}

TimeObject::~TimeObject()
{

}

VOID TimeObject::Start( DWORD dwIntervalTime )
{
	m_dwIntervalTime	= dwIntervalTime;								// ex)5000ms
	Reset();
}

VOID TimeObject::Stop()
{
	m_eTimeState		= TS_STOP;
	_calcElapsedTime();
}

VOID TimeObject::Resume()
{
	m_eTimeState		= TS_START;
	m_dwOldTime			= GetTickCount();
}

VOID TimeObject::Reset()
{
	m_dwOldTime			= GetTickCount();
	m_dwElapsedTime		= 0;
	m_eTimeState		= TS_START;
	OnStart();
}

VOID TimeObject::ForceEnd()
{
	// 바로 리스트에서 삭제한다.
	m_eTimeState = TS_END;
	OnExpired();
}

VOID TimeObject::_calcElapsedTime()
{
	m_dwElapsedTime = ( GetTickCount() - m_dwOldTime );
}

VOID TimeObject::_update()
{
	if( m_eTimeState == TS_START )
	{
		_calcElapsedTime();
		if( m_dwElapsedTime >= m_dwIntervalTime )
		{
			m_eTimeState = TS_END;
			OnExpired();
		}
	}
}
