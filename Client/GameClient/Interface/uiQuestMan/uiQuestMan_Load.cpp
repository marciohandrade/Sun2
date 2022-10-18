#include "SunClientPrecompiledHeader.h"	 
#include "uiQuestMan.h"
#include "uiQuestCommonForward/uiQuestCommonForward.h"
#include "uiQuestReward/uiQuestReward.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#include "uiQuestListMission/uiQuestListMission.h"
#include "uiQuestTimeInfo/uiQuestTimeInfo.h"
#include "uiQuestAutoAcceptAlram/uiQuestAutoAcceptAlram.h"
#include"uiTimeQuest/uiTimeQuest.h"
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
#include "uiQuestList/uiQuestList.h"
#endif//_NA_004644_20110506_QUEST_NAVIGATION

const DWORD IM_QUEST_MANAGER::QUEST_COMMON_FORWARD	= StrToWzID("0831");
const DWORD IM_QUEST_MANAGER::QUEST_REWARD			= StrToWzID("0832");
const DWORD IM_QUEST_MANAGER::QUEST_LIST_DLG		= StrToWzID("0839");
const DWORD IM_QUEST_MANAGER::QUEST_LIST_MISSION	= StrToWzID("0833");
const DWORD IM_QUEST_MANAGER::QUEST_TIME_INFO		= StrToWzID("0836");
const DWORD IM_QUEST_MANAGER::QUEST_AUTO_ACCETP		= StrToWzID("0837");
const DWORD IM_QUEST_MANAGER::QUEST_TIME_QUEST		= StrToWzID("0907");
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
const DWORD IM_QUEST_MANAGER::QUEST_LIST	= StrToWzID("8312");
#endif//_NA_004644_20110506_QUEST_NAVIGATION
//-------------------------------------------------------------------------------------------
/**
*/
BOOL uiQuestMan::_LoadUI()
{
	m_pUIQuestCommonForward = CreateUIDialog<uiQuestCommonForward>( IM_QUEST_MANAGER::QUEST_COMMON_FORWARD, 
																	"Data\\Interface\\83_1_N_Main_Common.iwz", this );
							  
	m_pUIQuestReward		= CreateUIDialog<uiQuestReward>( IM_QUEST_MANAGER::QUEST_REWARD, 
																	"Data\\Interface\\83_2_N_QuestReward.iwz", this );

	m_pUIQuestListDlg	= CreateUIDialog<uiQuestListDlg>( IM_QUEST_MANAGER::QUEST_LIST_DLG, 
																	"Data\\Interface\\83_9_P_Missionlist.iwz", this );

	m_pUITimeQuest			= CreateUIDialog<uiTimeQuest>( IM_QUEST_MANAGER::QUEST_TIME_QUEST, 
																  "Data\\Interface\\90_7_timer.iwz", this );

	m_puiQuestListMission	= CreateUIDialog<uiQuestListMission>( IM_QUEST_MANAGER::QUEST_LIST_MISSION, 
		"Data\\Interface\\83_3_P_Questdetail.iwz", this );

	m_pUITimeInfo			= CreateUIDialog<uiQuestTimeInfo>( IM_QUEST_MANAGER::QUEST_TIME_INFO, 
																	"Data\\Interface\\83_6_Q_Timer.iwz", this );

	m_pUIAutoAcceptAlram	= CreateUIDialog<uiQuestAutoAcceptAlram>( IM_QUEST_MANAGER::QUEST_AUTO_ACCETP, 
																	"Data\\Interface\\83_7_AutoQuestpopup.iwz", this );
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
    ui_quest_list_ptr_ = CreateUIDialog<uiQuestList>(IM_QUEST_MANAGER::QUEST_LIST, 
        "Data\\Interface\\83_12_QuestList.iwz", this);
#endif//_NA_004644_20110506_QUEST_NAVIGATION
	return TRUE;
}