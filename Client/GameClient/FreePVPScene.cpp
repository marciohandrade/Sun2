#include "SunClientPrecompiledHeader.h"

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include "FreePVPScene.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "GlobalFunc.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
///////////////////////////////////////////////////////////////

FreePVPScene g_FreePVPScene;

//-------------------------------------------------------------------------------------------
FreePVPScene::FreePVPScene() 
{
    m_SceneType = SCENE_TYPE_FREEPVP;
}

//-------------------------------------------------------------------------------------------
FreePVPScene::~FreePVPScene()
{
}

//-------------------------------------------------------------------------------------------
BOOL FreePVPScene::Init()
{
    BattleScene::Init();

    if (g_pMap)
    {
        GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_INSTANCE);
    }

//#ifdef _NA_0_20091118_GAMEROOM_BATTLE_RESULT //elflee
//    uiBattleScoreMan* battle_score_manager_ptr = 
//        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
//    if (battle_score_manager_ptr)
//    {
//        if (battle_score_manager_ptr && 
//            (battle_score_manager_ptr->GetIsInit() == false))
//        {
//            battle_score_manager_ptr->SetMode(uiBattleScoreMan::eBTS_BATTLE_GROUND);
//        }
//    }		
//#endif//_NA_0_20091118_GAMEROOM_BATTLE_RESULT


    //if( GENERALPARAM->IsNet() == FALSE )
    //{
    //}

    //GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->ShowDialog(FALSE);
    //GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->ShowDialog(FALSE);

    //g_bObserverMode = FALSE;

    /*MainDialog* pMainDlg = GET_CAST_DIALOG(MainDialog, IM_MENU_MANAGER::MAIN_DLG);

    if (pMainDlg)
    {
    pMainDlg->ShowObserverButton(FALSE);
    }*/

    return TRUE;
}

//-------------------------------------------------------------------------------------------

BOOL FreePVPScene::Process( DWORD dwTick )
{

    BOOL bRet = BattleScene::Process( dwTick );

    return bRet;
}

//-------------------------------------------------------------------------------------------
void FreePVPScene::Release( void )
{
    BattleScene::Release();

    /*MainDialog* pMainDlg = GET_CAST_DIALOG(MainDialog, IM_MENU_MANAGER::MAIN_DLG);

    if (pMainDlg)
    {
    pMainDlg->ShowObserverButton(FALSE);
    }*/

    //g_bObserverMode = FALSE;
    //g_IsBattle		= FALSE;  (채팅을 막네)

    ChaosSystem::GetFreePVPData().Clear();

}

//-------------------------------------------------------------------------------------------
void FreePVPScene::ProcessKeyInput_Hero(DWORD tick)
{
    BattleScene::ProcessKeyInput_Hero(tick);

#ifdef _DEBUG
#endif //_DEBUG
}

//-------------------------------------------------------------------------------------------
RELATIONKIND::TYPE FreePVPScene::GetRelationKindOfCharacter(Character* object, BOOL force_enemy)
{
    return GameFunc::GetRelationKindOfPlayerInFreePVP(object, force_enemy);
}
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
