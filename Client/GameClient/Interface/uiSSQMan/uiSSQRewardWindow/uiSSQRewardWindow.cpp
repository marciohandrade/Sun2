#include "SunClientPrecompiledHeader.h"
#include "uissqrewardwindow.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"

WzID uiSSQRewardWindow::m_wzID[MAX_SSQ_REWARD_WINDOW] =
{
	StrToWzID("ST01"),
	StrToWzID("ST14"),
	StrToWzID("ST15"),
	StrToWzID("ST16"),
	StrToWzID("ST17"),
	StrToWzID("B004"),
	StrToWzID("B001"),
};


uiSSQRewardWindow::uiSSQRewardWindow( InterfaceManager * pUIMan )
:uiBase( pUIMan )
{
}

uiSSQRewardWindow::~uiSSQRewardWindow(void)
{
}

void uiSSQRewardWindow::Init( CDrawBase * pDrawBase )
{
}

void uiSSQRewardWindow::Release()
{

}

void uiSSQRewardWindow::Process( DWORD dwTick )
{

}

void uiSSQRewardWindow::OnShowWindow( BOOL val )
{
	if( val )
	{
		MoveCenter();
		SetRewardInfoToUI();
	}
}

void uiSSQRewardWindow::SetRewardInfoToUI( void )
{
	TCHAR szTemp[INTERFACE_STRING_LENGTH +1] = { '\0', };
	TCHAR szText[MAX_PATH + 1] = { '\0', };
	CCtrlStaticWZ * pPoint		= (CCtrlStaticWZ *)getControl( ACQUIREMENT_POINT_STATIC ); 
	CCtrlStaticWZ * pExp		= (CCtrlStaticWZ *)getControl( REWARD_EXP_STATIC ); 
	CCtrlStaticWZ * pMonKill	= (CCtrlStaticWZ *)getControl( MONSTER_KILL_COUNT_STATIC ); 
	CCtrlStaticWZ * pPlayerKill = (CCtrlStaticWZ *)getControl( PLAYER_KILL_COUNT_STATIC ); 

	CCtrlStaticWZ * pNotice		= (CCtrlStaticWZ *)getControl( NOTICE_STATIC ); 

	int TextNum = 0;
	if( m_Type == REWARD_WIN )
		TextNum = 1771;
	else
		TextNum = 1770;

	g_InterfaceManager.GetInterfaceString( TextNum, szTemp );

	GlobalFunc::SetCaption( pNotice, szTemp );

	Sprintf( szText, "%d", m_RewardInfo.m_Point );
	GlobalFunc::SetCaption( pPoint, szText );
	Sprintf( szText, "%d", m_RewardInfo.m_Exp );
	GlobalFunc::SetCaption( pExp, szText );
	Sprintf( szText, "%d", m_RewardInfo.m_MonsterKillNum );
	GlobalFunc::SetCaption( pMonKill, szText );
	Sprintf( szText, "%d", m_RewardInfo.m_PlayerKillNum );
	GlobalFunc::SetCaption( pPlayerKill, szText );
}

void uiSSQRewardWindow::MessageProc( SI_MESSAGE * pMessage )
{
	switch( pMessage->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch( getControlIDToPos( pMessage->dwCtrlID ) )
			{
			case EXIT_BUTTON:
			case CONFIRM_BUTTON:
				{
					ShowWindow( FALSE );
				}
				break;
			}
		}
		break;
	}
}

