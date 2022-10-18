#include "SunClientPrecompiledHeader.h"
#include "BattleScene.h"
#include "missionscene.h"
#include "InterfaceManager.h"
#include "Map.h"
#include "Hero.h"
#include "BGMSoundInfoParser.h"

#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM

#include "uiHelpMan/uiHelpMan.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"
#include "uiMissionObject/uiMissionObject.h"
#include "uiMissionObjectMan/uiMissionObjectMan.h"

MissionScene		g_MissionScene;

MissionScene::MissionScene(void) 
{
	m_SceneType = SCENE_TYPE_MISSION;
}

MissionScene::~MissionScene(void)
{
}

void MissionScene::Activate( void )
{
	BattleScene::Activate();
}


BOOL MissionScene::Init( void )
{
	BattleScene::Init();

	if ( g_pMap )
		GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_MISSION );
	
	m_bDeadFlag = FALSE;

	uiBattleScoreMan* battle_score_manager_ptr = 
        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	if (battle_score_manager_ptr && 
		(battle_score_manager_ptr->GetIsInit() == false))
	{
		battle_score_manager_ptr->SetMode(uiBattleScoreMan::eBTS_MISSION);
		battle_score_manager_ptr->InitInfo();
	}

    uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));
    if (helpMan)
    {
        helpMan->UpdateHelpMission();
    }	

	return TRUE;
}

void MissionScene::InitMissionTime()
{
	m_bLeave_Party_Ban	= false;

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2* rank_manager2_ptr =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );

    if ( rank_manager2_ptr )
    {
        rank_manager2_ptr->InitMission();
    }
#else
	uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

	if(pRankMan)
	{
		pRankMan->InitMission();
	}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

	uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));
	if (helpMan)
	{
		helpMan->ClearHelpMission();
	}		
}


BOOL MissionScene::Process( DWORD dwTick )
{
	BOOL bRet = BattleScene::Process(dwTick);

	if (g_pHero)
	{
		if (!m_bDeadFlag && g_pHero->IsDead())
		{
			m_bDeadFlag = TRUE;
			verifyDeadMessageBox();
		}
	}	

	return bRet;
}


BOOL MissionScene::Render( DWORD dwTick )
{
	BattleScene::Render(dwTick);

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2* rank_manager2_ptr =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );

    if ( rank_manager2_ptr )
    {
        rank_manager2_ptr->DisplayTime();
    }
#else
	uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

	if( pRankMan )
	{
		pRankMan->DisplayTime();
	}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

	return TRUE;
}
  
//-------------------------------------------------------------------------------------------
/**
*/
void MissionScene::Release( void )
{
	BattleScene::Release();

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    uiMissionRankMan2* rank_manager2_ptr =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );

    if ( rank_manager2_ptr )
    {
        rank_manager2_ptr->CloseMissionRunningDlg();
    }
#else
	uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

	if(pRankMan)
	{
		pRankMan->CloseMissionRunningDlg();
	}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM

	uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));
	if (helpMan)
	{
		helpMan->HideHelpMission();
	}		
}

//-------------------------------------------------------------------------------------------
/**
*/
void MissionScene::verifyDeadMessageBox()
{
	TCHAR szMessage[INTERFACE_STRING_LENGTH], szText[512];
	TCHAR szTitle[INTERFACE_STRING_LENGTH];
	TCHAR szSubject[INTERFACE_STRING_LENGTH];

	ZeroMemory(szMessage, sizeof(szMessage));
	ZeroMemory(szText, sizeof(szText));

	g_InterfaceManager.GetInterfaceString(890, szMessage, INTERFACE_STRING_LENGTH);

	g_InterfaceManager.GetInterfaceString(891, szText, 512);
	StrCpy(szTitle, szText);

	g_InterfaceManager.GetInterfaceString(892, szText, 512);
	StrCpy(szSubject, szText);

	uiSystemMan * pSystemMan = (uiSystemMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
	if(pSystemMan)
		pSystemMan->Sys_Verify(szMessage, GlobalFunc::DeadVerifyInMission, szTitle, szSubject);

}

RELATIONKIND::TYPE MissionScene::GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInMissionZone( pObject, bForceEnemy );
}

//----------------------------------------------------------------------------
/**
*/
void MissionScene::ProcessKeyInput()
{
	BattleScene::ProcessKeyInput();
}