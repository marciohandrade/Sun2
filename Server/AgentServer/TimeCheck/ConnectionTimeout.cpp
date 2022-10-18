#include "stdafx.h"
#include ".\connectiontimeout.h"
#include "TimeKeeper.h"
#include "ConnectionTimeoutFactory.h"

ConnectionTimeout::ConnectionTimeout()
{
}

ConnectionTimeout::~ConnectionTimeout()
{
}

VOID ConnectionTimeout::OnStart()
{

}

VOID ConnectionTimeout::OnExpired()
{
	// 1. OverTime
	// 2. TimeKeeper���� �����Ѵ�.
	// 3. Factory�� ��ȯ

	g_TimeKeeper.KillTimer( GetAuthUserID() );
	g_ConnTimeoutFactory.FreeConnection( this );
}