#pragma once

#include "uibase.h"

class uiSSQSystemWindow	: public uiBase
{
	enum SSQ_SYSTEM_WINDOW
	{
		TIME_COUNT1_PICTURE,
		TIME_COUNT2_PICTURE,
		TIME_COUNT3_PICTURE,
		TIME_COUNT4_PICTURE,
		TIME_COUNT5_PICTURE,
		REMAIN_USER_STATIC,
		REMAIN_TIME_STATIC,

		MAX_SSQ_SYSTEM_WINDOW,
	};

public:
	uiSSQSystemWindow( InterfaceManager * pUIMan );
	~uiSSQSystemWindow( void );

	virtual void Init( CDrawBase * pDrawBase );
	virtual void Release();
	virtual void Process( DWORD dwTick );	
	virtual void OnShowWindow( BOOL val );
	virtual void MessageProc( SI_MESSAGE * pMessage );

	void	SetTimeCount( int count );
	void	ClearCount( void );
	void	DisplayRemainTime(  DWORD min, DWORD sec );
	void	SetRemainTime(  DWORD min, DWORD sec );
	void	SetRemainUser( int user, int total );
	void	SetStartCount( BYTE count ) { m_StartCount = count; }
	void	ProcessStartCount( void );
	void	SetStart( BOOL b = FALSE ) { m_bStart = b; }

	CControlWZ * getControl(POSTYPE AtPos)
	{
		assert( AtPos < MAX_SSQ_SYSTEM_WINDOW );
		return GetControlWZ( m_wzID[AtPos] );
	}

	POSTYPE getControlIDToPos(WzID wzID)
	{
		for( int i = 0; i < MAX_SSQ_SYSTEM_WINDOW; ++i)
		{
			if( m_wzID[i] == wzID )
				return i;
		}

		return MAX_SSQ_SYSTEM_WINDOW;
	}

private:
	static WzID		m_wzID[MAX_SSQ_SYSTEM_WINDOW];
	DWORD			m_dwMin;
	DWORD			m_dwSec;
	BYTE			m_StartCount;
	BYTE			m_BarCount;
	BOOL			m_bStart;
};
