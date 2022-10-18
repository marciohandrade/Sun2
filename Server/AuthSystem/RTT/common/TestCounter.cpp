#include "stdafx.h"

#include <eXtreme/threading/Mutex.h>
#include "TestCounter.h"

#define  Guard( x )			eXtreme::Guard	< eXtreme::Mutex >	guard( x )

TestCounter::TestCounter()
{
}

TestCounter::~TestCounter()
{
}

TestCounter::Counter* TestCounter::Get( DWORD subjectID )
{
	Guard( m_mutex );
	{
		MapCounter::iterator iter = m_mapCounter.find( subjectID );
		if( iter == m_mapCounter.end() )
			return NULL;

		return iter->second;
	}
}

TestCounter::Counter* TestCounter::Add( DWORD subjectID )
{
	Guard( m_mutex );
	{
		MapCounter::iterator iter = m_mapCounter.find( subjectID );
		if( iter != m_mapCounter.end() )
			return NULL;

		Counter* counter = new Counter();
		m_mapCounter[subjectID] = counter;

		return counter;
	}
}

void TestCounter::Remove( DWORD subjectID )
{
	Guard( m_mutex );
	{
		MapCounter::iterator iter = m_mapCounter.find( subjectID );
		if( iter == m_mapCounter.end() )
			return;

		Counter* counter = iter->second;
		delete counter;

		m_mapCounter.erase( iter );
	}
}

void TestCounter::RemoveAll()
{
	Guard( m_mutex );
	{
		Counter*				counter = NULL;

		for(MapCounter::iterator iter = m_mapCounter.begin() ; iter != m_mapCounter.end() ; iter++ )
		{
			counter = iter->second;
			delete counter;
		}

		m_mapCounter.clear();
	}
}

void TestCounter::Inc( DWORD subjectID, DWORD val )
{
	Counter*	counter = Get( subjectID );

	if( counter != 0 )
		counter->Inc( val );
}

void TestCounter::Dec( DWORD subjectID, DWORD val )
{
	Counter*	counter = Get( subjectID );

	if( counter != 0 )
		counter->Dec( val );
}



TestCounter::Counter::Counter()
	: m_tickStart( 0 ), m_tickStop( 0 ), m_value( 0 )
{
}

TestCounter::Counter::~Counter()
{
}

void TestCounter::Counter::Start()
{
	m_tickStart	= ::GetTickCount();
	m_tickStop	= 0;
}

void TestCounter::Counter::Stop()
{
	m_tickStop = ::GetTickCount();
}

void TestCounter::Counter::Inc( DWORD val )
{
	if( ( DWORD )( m_value + val ) < m_value )
		m_value = 0xFFFFFFFF;
	else
		m_value += val;
}

void TestCounter::Counter::Dec( DWORD val )
{
	if( ( DWORD )( m_value - val ) > m_value )
		m_value = 0;
	else
		m_value -= val;
}

DWORD TestCounter::Counter::GetCount()
{
	return m_value;
}

DWORD TestCounter::Counter::GetTimeGap()
{
	if( m_tickStop == 0 )
		return ( ::GetTickCount() - m_tickStart );

	return ( m_tickStop - m_tickStart );
}