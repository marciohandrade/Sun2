#pragma once

enum READY_STATE
{
	READY_STATE_NONE,
	READY_STATE_NOT_READY,
	READY_STATE_READY,
};

class ReadyState
{
public:
	ReadyState();
	~ReadyState();

	VOID			Init( BYTE StateCount );
	READY_STATE		Update();

	VOID			SetReadyState( BYTE StateID , BOOL bFlag );

private:
	BOOL			IsReady( BYTE StateID );
	READY_STATE		IsAllReady();

private:
	BYTE			m_StateCount;
	BOOL *			m_pbStateArray;
	READY_STATE		m_ChangedState;
};

