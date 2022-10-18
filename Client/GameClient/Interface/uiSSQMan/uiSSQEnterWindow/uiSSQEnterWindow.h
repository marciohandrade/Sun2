#pragma once

#include "uibase.h"
#include "ssq/SSQInfoParser.h"

enum SSQ_ENTER_WINDOW
{
	CREATE_PORTAL_BUTTON,
	CANCEL_BUTTON,
	NOTICE_STATIC,
	SEL_LINE1_PICTURE,
	SEL_LINE2_PICTURE,
	SEL_LINE3_PICTURE,
	SEL_LINE4_PICTURE,
	SEL_LINE5_PICTURE,
	SEL_LINE6_PICTURE,

	ENTER_LEVEL1_STATIC,
	ENTER_COST1_STATIC,
	ENTER_LEVEL2_STATIC,
	ENTER_COST2_STATIC,
	ENTER_LEVEL3_STATIC,
	ENTER_COST3_STATIC,
	ENTER_LEVEL4_STATIC,
	ENTER_COST4_STATIC,
	ENTER_LEVEL5_STATIC,
	ENTER_COST5_STATIC,
	ENTER_LEVEL6_STATIC,
	ENTER_COST6_STATIC,

	EXIT_BUTTON,

	MAX_SSQ_ENTER_WINDOW
};


//------------------------------------------------------------------------------
//! 타락의 사원 입장 팝업
//! 리소스 파일 : 76_4_SSQSystem.iwz
//------------------------------------------------------------------------------
class uiSSQEnterWindow : public uiBase
{

public:
	uiSSQEnterWindow( InterfaceManager * pUIMan );
	~uiSSQEnterWindow( void );

	virtual void Init( CDrawBase * pDrawBase );
	virtual void Release();
	virtual void Process( DWORD dwTick );	
	virtual void OnShowWindow( BOOL val );
	virtual void MessageProc( SI_MESSAGE * pMessage );

	
private:

	SSQInfo::INDEX_T CanOpenPortal();
	void SendCreatePortal();

	void SetSelLine( int index );
	void SetLevelCostString( void );
	void SetControl( int index, WORD MinLevel, WORD MaxLevel, MONEY cost );

	void ClearLine( void );

	CControlWZ * getControl(POSTYPE AtPos)
	{
		assert( AtPos < MAX_SSQ_ENTER_WINDOW );
		return GetControlWZ( m_wzID[AtPos] );
	}

	POSTYPE getControlIDToPos(WzID wzID)
	{
		for( int i = 0; i < MAX_SSQ_ENTER_WINDOW; ++i)
		{
			if( m_wzID[i] == wzID )
				return i;
		}

		return MAX_SSQ_ENTER_WINDOW;
	}


private:
	static WzID		m_wzID[MAX_SSQ_ENTER_WINDOW];
	CControlWZ *	m_pCreatePortalButton;
};


