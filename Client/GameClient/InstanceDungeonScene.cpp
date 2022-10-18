//------------------------------------------------------------------------------
// InstanceDegeonScene.cpp
//
//	(C) 2007 KTW
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "InstanceDungeonScene.h"
#include "ChatDialog.h"
#include "Player.h"
#include "ObjectManager.h"
#include "heroActioninput.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#include "Application.h"
#include "OptimizationManager.h"
#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
#else
  #include "RadarDialog.h"
  #include "RadarPlusDialog.h"
  #include "uiRadarMan/uiRadarMan.h"
  #include "InDunOptionParser.h"
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY

#include "uiBattleScoreMan/uiBattleScoreMan.h"

InstanceDungeonScene	g_InstanceDungeonScene;

//----------------------------------------------------------------------------
/**
*/
InstanceDungeonScene::InstanceDungeonScene(void)
{
	m_SceneType = SCENE_TYPE_INSTANCE_DUNGEON;
	m_byJoinState = eJION_TOP_STATE_NONE;
	m_dwStartCheckTime = 0;
	m_dwStartProcessTick = 0;

}



//----------------------------------------------------------------------------
/**
*/
InstanceDungeonScene::~InstanceDungeonScene(void)
{
}


//----------------------------------------------------------------------------
/** 초기화, 로딩
*/
BOOL InstanceDungeonScene::Init( void )
{
	BattleScene::Init();

	GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_INSTANCE );

	m_packetStatus = 0;

	g_OptimizationManager.SetPerformanceMode(TRUE);

    uiBattleScoreMan* battle_score_manager_ptr = 
        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
    if (battle_score_manager_ptr && battle_score_manager_ptr->GetIsInit()==false)
    {
        battle_score_manager_ptr->SetMode(uiBattleScoreMan::eBTS_SSQ);
        battle_score_manager_ptr->InitInfo();
    }

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
InstanceDungeonScene::Process( DWORD dwTick )
{
	processInfo(dwTick);

	return BattleScene::Process( dwTick );
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
InstanceDungeonScene::Render( DWORD dwTick )
{
	BOOL bRet = BattleScene::Render( dwTick );
	
	DisplayTime();
		
	return bRet;
}
//----------------------------------------------------------------------------
/** 
*/
void
InstanceDungeonScene::DisplayTime()
{
	//몬트샤인지역아니라면 리턴
	if(!g_pMap) return ;

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
	if(GET_MISSION_RANK_MANAGER()->GetRankingType(g_pMap->GetMapID()) != eRANKING_TYPE_TIME_ATTACK) return;
	
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    static uiMissionRankMan2* rank_manager_ptr2 = NULL;

    if ( !rank_manager_ptr2 )
    {
        rank_manager_ptr2 = GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );
    }

    if ( !rank_manager_ptr2 ) return;

    rank_manager_ptr2->DisplayTime();
#else
	static uiMissionRankMan* pRankMan = NULL;

	//static으로 한번만 참조하자
	if(!pRankMan)
	{
		pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );
	}

	if(!pRankMan) return;

	pRankMan->DisplayTime();
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
	
}
//----------------------------------------------------------------------------
/**
*/
void 
InstanceDungeonScene::Release( void )
{
	BattleScene::Release();

	g_OptimizationManager.SetPerformanceMode(FALSE);

	g_HeroInput.SetCannotAttack(FALSE);
    g_HeroInput.SetAutoAttack(FALSE);
}

#ifdef _NA_007477_20140805_RADAR_RAYER_MODIFY
#else
void InstanceDungeonScene::SetCurLayer(int iCurLayer)
{
	RadarDialog * pRadarDlg = GET_CAST_DIALOG(RadarDialog, IM_RADAR_MANAGER::RADAR_DIALOG);
	if(pRadarDlg)
	{
		pRadarDlg->SetCurLayer(iCurLayer);
	}

	RadarPlusDialog * pRadarPlusDlg = GET_CAST_DIALOG(RadarPlusDialog, IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
	if(pRadarPlusDlg)
	{
		pRadarPlusDlg->SetCurLayer(iCurLayer);
	}
}
//----------------------------------------------------------------------------
/**
*/
void InstanceDungeonScene::initRadarLayer(MAPCODE MapCode)
{
	int iMaxLayer = 1;

	INDUN_OPTION* pOption = InDunOptionParser::Instance()->GetInDunOption(MapCode);
	if (pOption)
    {
		iMaxLayer = pOption->mdwLayer;
    }


	RadarDialog* pRadarDlg = GET_CAST_DIALOG(RadarDialog, IM_RADAR_MANAGER::RADAR_DIALOG);
	if (pRadarDlg)
	{
		pRadarDlg->SetMaxLayer(iMaxLayer);
		pRadarDlg->SetCurLayer();
	}

	RadarPlusDialog* pRadarPlusDlg = GET_CAST_DIALOG(RadarPlusDialog, IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);
	if (pRadarPlusDlg)
	{
		pRadarPlusDlg->SetMaxLayer(iMaxLayer);
		pRadarPlusDlg->SetCurLayer();
	}
}
//----------------------------------------------------------------------------
/**
*/
VOID			
InstanceDungeonScene::initInstanceDungeon( MAPCODE MapCode )
{
	initRadarLayer( MapCode );
}
#endif //_NA_007477_20140805_RADAR_RAYER_MODIFY
//----------------------------------------------------------------------------
/** 
*/
VOID			
InstanceDungeonScene::processInfo( DWORD dwTick )
{
}

//----------------------------------------------------------------------------
/**
*/
void InstanceDungeonScene::ProcessKeyInput()
{
	BattleScene::ProcessKeyInput();
}

void InstanceDungeonScene::Send_MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN()
{
	MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN sync;
	GlobalFunc::SendPacket(CI_GAMESERVERIDX, &sync, sizeof (sync));
}

RELATIONKIND::TYPE InstanceDungeonScene::GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInInstanceDungeon( pObject, bForceEnemy );
}
