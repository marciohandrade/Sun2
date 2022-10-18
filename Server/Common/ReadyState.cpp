#include "StdAfx.h"
#include "ReadyState.h"

ReadyState::ReadyState()
{
	m_StateCount = 0;
	m_pbStateArray = NULL;
	m_ChangedState = READY_STATE_NONE;
}

ReadyState::~ReadyState()
{
	SAFE_DELETE_ARRAY( m_pbStateArray );
}

VOID ReadyState::Init( BYTE StateCount )
{
	m_StateCount = StateCount;
	m_pbStateArray = new BOOL[m_StateCount];
	ZeroMemory( m_pbStateArray, sizeof(BOOL) * m_StateCount );
}

READY_STATE ReadyState::Update()
{
	READY_STATE CurState = IsAllReady();

	if( m_ChangedState != CurState )
	{
		m_ChangedState = CurState;
		return m_ChangedState;
	}

	return READY_STATE_NONE;
}

VOID ReadyState::SetReadyState( BYTE StateID , BOOL bFlag )
{
	if( !m_pbStateArray )	return;

	m_pbStateArray[StateID] = bFlag;
}

BOOL ReadyState::IsReady( BYTE StateID )
{
	if( !m_pbStateArray )	return FALSE;

	return m_pbStateArray[StateID];
}

READY_STATE ReadyState::IsAllReady()
{
	if( !m_pbStateArray )	return READY_STATE_NONE;

	for( int i = 0; i < m_StateCount; ++i )
	{
		if( !IsReady( i ) )
			return READY_STATE_NOT_READY;
	}

	return READY_STATE_READY;
}

