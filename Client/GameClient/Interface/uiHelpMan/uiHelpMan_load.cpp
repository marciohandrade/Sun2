#include "SunClientPrecompiledHeader.h"
#include "uiHelpMan.h"
#include "uiHelpMenu/uiHelpMenu.h"
#include "uiHelpWindow/uiHelpWindow.h"
#include "uiHelpAnnounceWindow/uiHelpAnnounceWindow.h"
#include "interfacemanager.h"
#include "uiHelpMission/uiHelpMission.h"
#include "uiHelpMonsterInfo/uiHelpMonsterInfo.h"

#include "uiHelpMissionTimer/uiHelpMissionTimer.h"


const DWORD IM_HELP_MANAGER::HELP_MENU_DLG		= StrToWzID("0490");
const DWORD IM_HELP_MANAGER::HELP_WINDOW_DLG	= StrToWzID("0459");
const DWORD IM_HELP_MANAGER::HELP_ANNOUNCE_DLG	= StrToWzID("0430");
const DWORD IM_HELP_MANAGER::HELP_MONSTER_DLG	= StrToWzID("0459");
const DWORD IM_HELP_MANAGER::HELP_MISSION_DLG	= StrToWzID("0501");
const DWORD IM_HELP_MANAGER::HELP_MISSION_TIMER_DLG	= StrToWzID("0503");


const char  c_strDlgFname_HelpMenu[]			= ("Data\\Interface\\49_helpbox.iwz");
const char  c_strDlgFname_HelpWindow[]			= ("Data\\Interface\\45_9_Helpmessage.iwz");
const char  c_strDlgFname_HelpAnnounce[]		= ("Data\\Interface\\43_Announce_Window.iwz");
const char  c_strDlgFname_HelpMission[]			= ("Data\\Interface\\50_1_Misson_Directing.iwz");
const char  c_strDlgFname_HelpMissionTimer[]    = ("Data\\Interface\\50_3_mission_timer.iwz");

//------------------------------------------------------------------------------
/**
*/
BOOL uiHelpMan::_LoadUI()
{
	//HELP_MENU_DLG
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	m_pUIHelpMenu			= CreateUIDialog<uiHelpMenu>( IM_HELP_MANAGER::HELP_MENU_DLG, c_strDlgFname_HelpMenu, this );
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

	//HELP_WINDOW_DLG
	m_pUIHelpWindow			= CreateUIDialog<uiHelpWindow>( IM_HELP_MANAGER::HELP_WINDOW_DLG, c_strDlgFname_HelpWindow, this );

	//HELP_ANNOUNCE_DLG	
	m_pUIHelpAnnounceWindow = CreateUIDialog<uiHelpAnnounceWindow>( IM_HELP_MANAGER::HELP_ANNOUNCE_DLG, c_strDlgFname_HelpAnnounce, this );

	//HELP_MONSTER_DLG	// 싱글모드시 몬스터 이동 및 스킬 보기 기능
	m_pUIHelpMonsterInfo  = CreateUIDialog<uiHelpMonsterInfo>(IM_HELP_MANAGER::HELP_MONSTER_DLG,c_strDlgFname_HelpWindow,this );

	//HELP_MISSION_DLG	
	m_pUIHelpMission		= CreateUIDialog<uiHelpMission>( IM_HELP_MANAGER::HELP_MISSION_DLG, c_strDlgFname_HelpMission, this );
    help_mission_timer_ = CreateUIDialog<uiHelpMissionTimer>(IM_HELP_MANAGER::HELP_MISSION_TIMER_DLG, 
                                                             c_strDlgFname_HelpMissionTimer, this);

	return TRUE;
}








