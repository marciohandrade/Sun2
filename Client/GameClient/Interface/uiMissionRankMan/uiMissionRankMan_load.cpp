#include "SunClientPrecompiledHeader.h"
#if !defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan.h"
#include "uiMissionRankTimer/uiMissionRankTimer.h"
#include "uiMissionRankEnding/uiMissionRankEnding.h"
#include "uiMissionRankList/uiMissionRankList.h"
#include "uiMissionRankGamble/uiMissionRankGamble.h"
#include "uiRankMenu/uiRankMenu.h"

#include "interfacemanager.h"

const DWORD IM_MISSION_RANK_MANAGER::RankMenu		 = StrToWzID("0906");
const DWORD IM_MISSION_RANK_MANAGER::MissionRankList = StrToWzID("0903");
const DWORD IM_MISSION_RANK_MANAGER::PointRankList	 = StrToWzID("0904");
const DWORD IM_MISSION_RANK_MANAGER::RankTimer		 = StrToWzID("0900");
const DWORD IM_MISSION_RANK_MANAGER::RankEnding		 = StrToWzID("0901");
const DWORD IM_MISSION_RANK_MANAGER::RankGamble		 = StrToWzID("0902");

BOOL uiMissionRankMan::_LoadUI()
{
	m_pUIPointList	= CreateUIDialog<uiMissionRankList>( IM_MISSION_RANK_MANAGER::PointRankList, "Data\\Interface\\90_4_Lank_List.iwz", this );
	m_pUIRankMenu	= CreateUIDialog<uiRankMenu>( IM_MISSION_RANK_MANAGER::RankMenu, "Data\\Interface\\90_6_Rank_menu.iwz", this );
	m_pUIRankTimer	= CreateUIDialog<uiMissionRankTimer>( IM_MISSION_RANK_MANAGER::RankTimer, "Data\\Interface\\90_timer.iwz", this );
	m_pUIRankEnding	= CreateUIDialog<uiMissionRankEnding>( IM_MISSION_RANK_MANAGER::RankEnding, "Data\\Interface\\90_1_Mission_End.iwz", this );
	m_pUIRankList	= CreateUIDialog<uiMissionRankList>( IM_MISSION_RANK_MANAGER::MissionRankList, "Data\\Interface\\90_3_Lank_List.iwz", this );
	m_pUIRankGamble	= CreateUIDialog<uiMissionRankGamble>( IM_MISSION_RANK_MANAGER::RankGamble, "Data\\Interface\\90_2_Lank_gamble.iwz", this );

	return TRUE;
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

