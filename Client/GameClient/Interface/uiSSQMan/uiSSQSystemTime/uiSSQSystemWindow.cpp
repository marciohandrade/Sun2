#include "SunClientPrecompiledHeader.h"
#include "uissqsystemwindow.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "SoundEffect.h"

WzID uiSSQSystemWindow::m_wzID[MAX_SSQ_SYSTEM_WINDOW] =
{
	StrToWzID("PI00"),
	StrToWzID("PI01"),
	StrToWzID("PI02"),
	StrToWzID("PI03"),
	StrToWzID("PI04"),
	StrToWzID("ST01"),
	StrToWzID("ST03"),
};

uiSSQSystemWindow::uiSSQSystemWindow( InterfaceManager * pUIMan )
:uiBase( pUIMan )
,m_dwMin( 0 )
,m_dwSec( 0 )
,m_StartCount( 0 )
,m_BarCount( 5 )
,m_bStart( FALSE )
{
}


uiSSQSystemWindow::~uiSSQSystemWindow(void)
{
}

void uiSSQSystemWindow::Init( CDrawBase * pDrawBase )
{
	DisplayRemainTime( 0, 0 );
	SetRemainUser( 1, 100 );
}

void uiSSQSystemWindow::Release()
{

}

void uiSSQSystemWindow::Process( DWORD dwTick )
{
	static DWORD Tick = 0;

	Tick += dwTick;

	if( Tick >= 1000 )
	{
		Tick = 0;
		
		if( m_dwSec )
			--m_dwSec;
	
		DisplayRemainTime( m_dwMin, m_dwSec );

		ProcessStartCount();
	}
}

void uiSSQSystemWindow::OnShowWindow( BOOL val )
{
	if( val )
	{
		ClearCount();
		SetRemainTime( 0, 0 );
	}
	else
	{

	}

}

void uiSSQSystemWindow::MessageProc( SI_MESSAGE * pMessage )
{

}

void uiSSQSystemWindow::SetTimeCount( int count )
{
	CControlWZ * pControl = getControl( count );

	if( pControl )
	{
		pControl->ShowWindowWZ( WZ_SHOW );
	}

	g_SoundEffect.PlayUI( 568 + count );
}

void uiSSQSystemWindow::ClearCount()
{
	for( int i = 0; i < REMAIN_USER_STATIC; ++i )
	{
		CControlWZ * pControl = getControl( i );
		if( pControl )
		{
			pControl->ShowWindowWZ( WZ_HIDE );
		}
	}
}

void uiSSQSystemWindow::DisplayRemainTime(  DWORD min, DWORD sec )
{
	TCHAR szTime[MAX_PATH + 1];
	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1];

	g_InterfaceManager.GetInterfaceString( 1778, szTemp );

	Sprintf( szTime, "%d'  %d\"", min, sec );

	CCtrlStaticWZ * pControl = (CCtrlStaticWZ*)getControl( REMAIN_TIME_STATIC );

	GlobalFunc::SetCaption( pControl, szTime );
}

void uiSSQSystemWindow::SetRemainTime( DWORD min, DWORD sec )
{
	m_dwMin = min;
	m_dwSec = sec;
}

void uiSSQSystemWindow::SetRemainUser( int user, int total )
{
	TCHAR szUser[MAX_PATH + 1];
	TCHAR szTemp[INTERFACE_STRING_LENGTH + 1];

	g_InterfaceManager.GetInterfaceString( 1777, szTemp );

	Sprintf( szUser, szTemp, user, total );

	CCtrlStaticWZ * pControl = (CCtrlStaticWZ*)getControl( REMAIN_USER_STATIC );

	GlobalFunc::SetCaption( pControl, szUser );
	
}

void uiSSQSystemWindow::ProcessStartCount()
{	
	if( m_StartCount == 0 )
		return;
	
	m_StartCount--;

	if( m_StartCount > 5 )
	{
		TCHAR szText[INTERFACE_STRING_LENGTH + 1];
		TCHAR szMessage[INTERFACE_STRING_LENGTH + 1];
		g_InterfaceManager.GetInterfaceString( 1768, szText );
		Sprintf( szMessage, szText, m_StartCount );
		GlobalFunc::default_OutputMsg( eOUTPUT_MSG_CHAT, szMessage );
	}
	else
	{
		SetTimeCount( m_BarCount - m_StartCount );
	}
}
