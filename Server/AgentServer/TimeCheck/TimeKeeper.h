#ifndef __TIME_KEEPER_H__
#define __TIME_KEEPER_H__

#pragma once

#include <SolarHashTable.h>

class TimeObject;
class ConnectionTimeout;

class TimeKeeper
{
public:
	TimeKeeper();
	~TimeKeeper();
	VOID										InitTimeKeeper( DWORD dwHashSize );
	VOID										ReleaseTimeKeeper();

	VOID										PushTimer( DWORD dwKey, TimeObject * pObj );
	VOID										KillTimer( DWORD dwKey, TimeObject ** ppObj = NULL );

	TimeObject *								PeekTimer( DWORD dwKey );

	// 인증을 위해 하나더 키를 추가함
	TimeObject *								PeekTimerByUserGUID( DWORD dwUserGUID );

	VOID										Update();
	
private:
	util::SolarHashTable<TimeObject *>			m_TimeObjectHash;
	util::SolarHashTable<TimeObject *>			m_TimeObjectHashByUserGUID;
};

extern TimeKeeper g_TimeKeeper;

#endif // __TIME_KEEPER_H__