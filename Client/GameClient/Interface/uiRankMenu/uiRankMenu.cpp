#include "SunClientPrecompiledHeader.h"
#if !defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uirankmenu.h"
#include "uiMissionRankMan/uiMissionRankMan.h"
#include "GlobalFunc.h"

WzID uiRankMenu::m_wzId[MAX_UI] =
{
	StrToWzID("BT00"),
	StrToWzID("B002"),
	StrToWzID("B999")
};

uiRankMenu::uiRankMenu( InterfaceManager *pUIMan )
{
}

uiRankMenu::~uiRankMenu(void)
{
}

void uiRankMenu::Init(CDrawBase* pDrawBase)
{
#ifdef _RU_110420_POINT_RANKING_BLOCK
    CControlWZ* point_ranking_button = getControl(POINT_RANK_LIST_BUTTON);
    if (point_ranking_button != NULL)
    {
        point_ranking_button->ShowWindowWZ(WZ_HIDE);
    }
#endif //_RU_110420_POINT_RANKING_BLOCK
}

void uiRankMenu::Release()
{

}

void uiRankMenu::OnShowWindow( BOOL val )
{
	if( val )
	{
		MoveCenter();

		keyMsg msg;
		ZeroMemory(&msg,sizeof(msg));
		msg.dwType = eDoType_dialog_mission_rank_menu;
		msg.wParam = IM_MISSION_RANK_MANAGER::RankMenu;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_dialog_mission_rank_menu);
	}
}

void uiRankMenu::MessageProc( SI_MESSAGE * pMessage )
{
	uiMissionRankMan * pRankMan = GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

	if( pRankMan == NULL )
		return;
    
	switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONCLICK:
		{
			switch( getCtrlIDToPos( pMessage->dwCtrlID ) )
			{
			case MISSION_RANK_LIST_BUTTON:
				{
					pRankMan->ShowRankList( eRANKING_TYPE_TIME_ATTACK );					
					ShowDialog( FALSE );
				}
				break;
			case POINT_RANK_LIST_BUTTON:
				{
					pRankMan->ShowRankList( eRANKING_TYPE_POINT );
					ShowDialog( FALSE );
				}
				break;
			case EXIT_BUTTON:
				{
					ShowDialog( FALSE );
				}
				break;
			}
		}
		break;
	}

}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
