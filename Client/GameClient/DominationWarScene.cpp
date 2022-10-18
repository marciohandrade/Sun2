
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "DominationWarScene.h"

#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiDominationTimer/uiDominationTimer.h"

#include "BGMSoundInfoParser.h"
#include "Map.h"
#include "GlobalFunc.h"

#include "uiBattleScoreMan\uiBattleScoreMan.h"
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
#include "uiDominationStamp/uiDominationStamp.h"
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

DominationWarScene g_DominationWarScene;

DominationWarScene::DominationWarScene()
{
	m_SceneType = SCENE_TYPE_DOMINATION;
}

DominationWarScene::~DominationWarScene()
{
}

BOOL DominationWarScene::Init()
{
	if ( NULL == g_pMap )
		return FALSE;

	BOOL bReturn;

	m_bDeadFlag				= FALSE;
	m_bPendingResurrection	= FALSE;
	m_dwRenderTime			= 0;

	bReturn = BattleScene::Init();
	
	GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_DOMINATION_FIELD );

#if defined(_NA_007115_20140313_MONDSCHIEN_RENEWAL) && !defined(_NA_008757_20160329_DOMINATION_GRAPHIC_CONTROL)
    // 게임 옵션 조정 하지 않음.
#else
	//m_bOldTileBlendingVal = GAMEOPTION->IsTileBlending();
	//GAMEOPTION->SetTileBlending(false);

    m_iOldEffectLevel = GAMEOPTION->GetGameOption().GetOptionValue( nOptionType::EFFECT_QUALITY );
	
	g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\DeleteEffectResource.txt");
	g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource3.txt");

	g_pEffectManager->LoadEffectFromFile("Data\\Effect\\D_EffectResource.txt", "Data\\Effect\\", EELT_ADD, 0);

	m_bOldShadowVal = GAMEOPTION->GetShadowOn();
	GAMEOPTION->SetShadowOn(FALSE);

	g_pMap->SetVisibleRange(70);
	g_pMap->SetAlphaRange(60);

	m_fOldCullingRange	= GAMEOPTION->GetCullLength();
	GAMEOPTION->SetCullLength(20.f);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	uiBattleScoreMan* battle_score_manager = 
        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	if (battle_score_manager)
	{
		if (battle_score_manager && 
			(battle_score_manager->GetIsInit() == false))
		{
			battle_score_manager->SetMode(uiBattleScoreMan::eBTS_DOMINATION);
		}
	}		

	return bReturn;
}

BOOL DominationWarScene::Process( DWORD dwTick )
{
	BOOL bReturn;

	bReturn = BattleScene::Process(dwTick);

	SendHeroSectorIndex();

	return bReturn;
}

void DominationWarScene::Release()
{
	FieldScene::Release();

#if defined(_NA_007115_20140313_MONDSCHIEN_RENEWAL) && !defined(_NA_008757_20160329_DOMINATION_GRAPHIC_CONTROL)
    // 게임 옵션 조정하지 않음.    
#else
	//GAMEOPTION->SetTileBlending(m_bOldTileBlendingVal);

	if(3 == m_iOldEffectLevel)
	{
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0);

		g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource1.txt");
		g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource2.txt");
		g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource3.txt");
	}
	else if(2 == m_iOldEffectLevel)
	{
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0);
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource1.txt","Data\\Effect\\", EELT_ADD, 1);

		g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource2.txt");
		g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource3.txt");
	}
	else if(1 == m_iOldEffectLevel)
	{
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0);
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource1.txt","Data\\Effect\\", EELT_ADD, 1);
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource2.txt","Data\\Effect\\", EELT_ADD, 2);

		g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource3.txt");

	}
	else if(0 == m_iOldEffectLevel)
	{
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource0.txt","Data\\Effect\\", EELT_ADD, 0);
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource1.txt","Data\\Effect\\", EELT_ADD, 1);
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource2.txt","Data\\Effect\\", EELT_ADD, 2);
		g_pEffectManager->LoadEffectFromFile("Data\\Effect\\effectresource3.txt","Data\\Effect\\", EELT_ADD, 3);
	}

	g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\D_EffectResource.txt");

	// 이펙트 기본 레벨 설정
	g_pEffectManager->SetBaseEffectLevel( 3 - m_iOldEffectLevel );


	GAMEOPTION->SetShadowOn(m_bOldShadowVal);

	GAMEOPTION->SetCullLength(m_fOldCullingRange);
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	uiBattleScoreMan* battle_score_manager = 
        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	if (battle_score_manager)
	{
		if (battle_score_manager &&
			(battle_score_manager->GetIsInit() == false))
		{
			battle_score_manager->Reset();
		}
	}		
}

void DominationWarScene::ParsePacket( MSG_BASE * pMsg )
{
	FieldScene::ParsePacket(pMsg);
}

RELATIONKIND::TYPE DominationWarScene::GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInDominationWar(pObject, bForceEnemy);
}

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
BOOL DominationWarScene::PostInterfaceRender( DWORD tick )
{
    BOOL result = BattleScene::PostInterfaceRender(tick);

    uiDominationStamp* domination_stamp_ptr = GET_CAST_DIALOG(uiDominationStamp, 
        IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_STAMP_DIALOG);
    if (domination_stamp_ptr)
    {
        domination_stamp_ptr->RenderStampGuildText();
    }

    return result;
}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL