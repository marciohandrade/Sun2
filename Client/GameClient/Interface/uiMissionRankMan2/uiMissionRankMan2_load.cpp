#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)

#include "uiMissionRankMan2.h"
#include "uiMissionRankTimer/uiMissionRankTimer.h"
#include "uiMissionRankEnding/uiMissionRankEnding.h"
#include "uiMissionRankList2/uiMissionRankList2.h"
#include "uiMissionRankGamble/uiMissionRankGamble.h"

#include "interfacemanager.h"

const DWORD IM_MISSION_RANK_MANAGER::MissionRankList = StrToWzID("0909");
const DWORD IM_MISSION_RANK_MANAGER::RankTimer		 = StrToWzID("0900");
const DWORD IM_MISSION_RANK_MANAGER::RankEnding		 = StrToWzID("0901");
const DWORD IM_MISSION_RANK_MANAGER::RankGamble		 = StrToWzID("0902");

BOOL uiMissionRankMan2::_LoadUI()
{    
    ui_rank_timer_ptr_	= CreateUIDialog<uiMissionRankTimer>( IM_MISSION_RANK_MANAGER::RankTimer, "Data\\Interface\\90_timer.iwz", this );
    ui_rank_ending_ptr_	= CreateUIDialog<uiMissionRankEnding>( IM_MISSION_RANK_MANAGER::RankEnding, "Data\\Interface\\90_1_Mission_End.iwz", this );
    ui_rank_list_ptr_	= CreateUIDialog<uiMissionRankList2>( IM_MISSION_RANK_MANAGER::MissionRankList, "Data\\Interface\\90_9_Lank_List.iwz", this );
    ui_rank_gamble_ptr_	= CreateUIDialog<uiMissionRankGamble>( IM_MISSION_RANK_MANAGER::RankGamble, "Data\\Interface\\90_2_Lank_gamble.iwz", this );

    return TRUE;
}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

