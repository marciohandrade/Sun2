#include "SunClientPrecompiledHeader.h"

#include "fieldscene.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
#include "MapInfoParser.h"
#include "Hero.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
#include "uiBattlezone2Man/uiBattlezone2Man.h"
#endif //_NA_000000_20121210_REFACTORY_GAME_USER
#include "ObjectManager.h"

FieldScene		g_FieldScene;

FieldScene::FieldScene(void)
{
	m_SceneType = SCENE_TYPE_FIELD;
}

FieldScene::~FieldScene(void)
{
}

BOOL FieldScene::Init( void )
{
	m_bDeadFlag				= FALSE;
	m_bPendingResurrection	= FALSE;
	m_dwRenderTime			= 0;

	BattleScene::Init();

	if ( g_pMap )
	{
		GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_FIELD );
	}

	uiDominationTowerMan* pDominationMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
	if (pDominationMan && false == pDominationMan->IsShowDominationObject())
	{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        pDominationMan->set_domination_object_show(FALSE);
#else
		Object* pObject = g_ObjectManager.GetObject(pDominationMan->GetDominationObject());
		if (pObject)
		{
			pObject->SetShow(FALSE);
		}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
	}

#ifdef _NA_000000_20121210_REFACTORY_GAME_USER
	//이전씬
	SCENE_TYPE postSceneType = GameFramework::GetPostSceneType();

	if(postSceneType == SCENE_TYPE_MISSION )
	{
		uiBattlezone2Man* ui_battlezone2_manager_ = GET_DIALOG_MANAGER(uiBattlezone2Man, UIMAN_BATTLE_ZONE_MANAGER);

		if (ui_battlezone2_manager_ != NULL)
		{
			BYTE isStateBattleZone2 = ui_battlezone2_manager_->battlezone_room_state();

			//현재 배틀존 입장 상태가 로비 상태와 같다면
			if(isStateBattleZone2 == uiBattlezone2Man::ROOM_STATE_LOBBY)
			{
				ui_battlezone2_manager_->ShowBattlezoneDialog(uiBattlezone2Man::BATTLEZONE2_DIALOG_LOBBY,true);
			}
		}
		
	}
#endif //_NA_000000_20121210_REFACTORY_GAME_USER

	return TRUE;
}


BOOL FieldScene::Process( DWORD dwTick )
{
	BOOL bRet = BattleScene::Process( dwTick );

	TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
	if(m_dwRenderTime < 10000)
	{
		m_dwRenderTime += dwTick;
		const sMAPINFO * pMapInfo = MapInfoParser::Instance()->FindMapInfo(g_pMap->GetMapID()); 
		if(pMapInfo)
		{
			g_InterfaceManager.GetInterfaceString(pMapInfo->dwFNCode, szMessage, INTERFACE_STRING_LENGTH);
			g_InterfaceManager.ShowMissionAreaInfo(szMessage,10000, NULL);
		}
	}

    int i;
    for (i = 0; i < 10; ++i)
    {
        TCHAR   szText[INTERFACE_STRING_LENGTH];

        Sprintf(szText, _T("PRT%d"), i);
        this->CheckHeroInShortcutBattleArea(szText, StrToWzID(szText));
        Sprintf(szText, _T("OUT%d"), i);
        this->CheckHeroInShortcutBattleArea(szText, StrToWzID(szText));
		Sprintf(szText, _T("MP0%d"), i);
		this->CheckHeroInShortcutBattleArea(szText, StrToWzID(szText));	
    }

	if (g_pHero)
	{
		if (!m_bReadyDeadFlag && !m_bDeadFlag && g_pHero->IsDead())
		{
			//튜토리얼필드(에소드마을)에서는 사망시 팝업이 뜨지않는다
			if (!g_pMap->IsEsodeMap())
			{
				m_bDeadFlag = TRUE;
				this->verifyDeadMessageBox();
			}
			
		}
	}
	SendHeroSectorIndex();
	return bRet;
}


//-------------------------------------------------------------------------------------------
/**
*/
void 
FieldScene::Release( void )
{
	BattleScene::Release();

	this->m_bDeadFlag = FALSE;
}


//-------------------------------------------------------------------------------------------
/**
*/
void	
FieldScene::verifyDeadMessageBox()
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
    {
		//pSystemMan->Sys_Confirm( szMessage, GlobalFunc::DeadConfirmInField, szTitle, szSubject, 887, 900);
        pSystemMan->Sys_ConfirmCloseButton(szMessage, GlobalFunc::DeadConfirmInField, szTitle, szSubject, 887, 900);
    }
}

//-------------------------------------------------------------------------------------------
/**
*/

RELATIONKIND::TYPE  FieldScene::GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInField( pObject, bForceEnemy );
}