#include "stdafx.h"
#include ".\timekeeper.h"
#include "TimeObject.h"
#include "ConnectionTimeout.h"

TimeKeeper g_TimeKeeper;

TimeKeeper::TimeKeeper()
{
	
}
		
TimeKeeper::~TimeKeeper()
{
	
}

VOID TimeKeeper::InitTimeKeeper( DWORD dwHashSize )
{
	m_TimeObjectHash.Initialize( dwHashSize );
	m_TimeObjectHashByUserGUID.Initialize( dwHashSize );
}
VOID TimeKeeper::ReleaseTimeKeeper()
{
	// Object반환하자!
	
	m_TimeObjectHash.RemoveAll();
	m_TimeObjectHashByUserGUID.RemoveAll();
}

VOID TimeKeeper::Update()
{
	m_TimeObjectHash.SetFirst();
	while (TimeObject* pObj = m_TimeObjectHash.GetNext() )
	{
		pObj->_update();
	}
}

VOID TimeKeeper::PushTimer( DWORD dwKey, TimeObject * pObj )
{
	ASSERT(pObj);
	// 임시
	ConnectionTimeout * pTimeout = (ConnectionTimeout *)pObj;

	m_TimeObjectHash.Add( pObj, dwKey );
	m_TimeObjectHashByUserGUID.Add( pObj, pTimeout->GetUserGUID() );
}

VOID TimeKeeper::KillTimer( DWORD dwKey, TimeObject ** ppObj )
{
	TimeObject * pObject = m_TimeObjectHash.GetData( dwKey );
	if( ppObj ) *ppObj = pObject;

	ConnectionTimeout * pTimeout = (ConnectionTimeout *)pObject;

	m_TimeObjectHash.Remove( dwKey );
	if(pTimeout) m_TimeObjectHashByUserGUID.Remove(pTimeout->GetUserGUID());
	
}

TimeObject * TimeKeeper::PeekTimer( DWORD dwKey )
{
	return m_TimeObjectHash.GetData( dwKey );
}

TimeObject * TimeKeeper::PeekTimerByUserGUID( DWORD dwUserGUID )
{
	return m_TimeObjectHashByUserGUID.GetData( dwUserGUID );
}