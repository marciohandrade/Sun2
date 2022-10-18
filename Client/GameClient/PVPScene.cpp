//------------------------------------------------------------------------------
// PVPScene.cpp
//
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "pvpscene.h"
#include "PVPManager.h"
#include "ChatDialog.h"
#include "Hero.h"
#include "ObjectManager.h"
#include "heroActioninput.h"
#include "Map.h"
#include "BGMSoundInfoParser.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiBattleScoreMan\uiBattleScoreMan.h"

PVPScene		g_PVPScene;

//----------------------------------------------------------------------------
/**
*/
PVPScene::PVPScene(void)
{
	m_SceneType = SCENE_TYPE_PVP;
}



//----------------------------------------------------------------------------
/**
*/
PVPScene::~PVPScene(void)
{
}


//----------------------------------------------------------------------------
/** 초기화, 로딩
*/
BOOL 
PVPScene::Init( void )
{
	BattleScene::Init();

	GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_PVP );

	m_packetStatus = 0;
	m_pPressAnykeyDelay = new util::Timer;
	m_pStartMatchDelay	= new util::Timer;
	m_bCheckPressAnyKey = FALSE;

	this->m_PVPManager.Init();

	uiBattleScoreMan* battle_score_manager_ptr = 
        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	if (battle_score_manager_ptr)
	{
		if (battle_score_manager_ptr && 
			(battle_score_manager_ptr->GetIsInit() == false))
		{
			switch (battle_score_manager_ptr->GetHeroTeam())
			{
			case 1:	//팀전
			case 2:
			{
				battle_score_manager_ptr->SetMode(uiBattleScoreMan::eBTS_PVP_TEAM);
			}
			break;
			default:// PVP 개인전
			{
				battle_score_manager_ptr->SetMode(uiBattleScoreMan::eBTS_PVP_PRIVATE);
			}
			break;
			}
		}
	}		

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
PVPScene::Process( DWORD dwTick )
{
	processInfo(dwTick);

	if ( TRUE == m_bCheckPressAnyKey )
	{
		if ( m_pPressAnykeyDelay->IsExpired() )
		{
			TCHAR	szMessage[INTERFACE_STRING_LENGTH];
			//	키 입력이 없어 자동으로 마을로 돌아갑니다.
			const int	c_iReturnVillage = 5605;
			g_InterfaceManager.GetInterfaceString( c_iReturnVillage, szMessage, INTERFACE_STRING_LENGTH );
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

			MSG_CG_ZONE_PVP_LEAVE_SYN SendPacket;
			GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));

			m_bCheckPressAnyKey = FALSE;
		}
	}
	return BattleScene::Process( dwTick );
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
PVPScene::Render( DWORD dwTick )
{
	return BattleScene::Render( dwTick );
}


//----------------------------------------------------------------------------
/**
*/
void 
PVPScene::Release( void )
{
	BattleScene::Release();

	SAFE_DELETE(m_pStartMatchDelay);
	SAFE_DELETE(m_pPressAnykeyDelay);

	this->m_PVPManager.RemoveAll();

	g_HeroInput.SetCannotAttack(FALSE);

	uiBattleScoreMan* battle_score_manager_ptr =
        static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
	if (battle_score_manager_ptr)
	{
		battle_score_manager_ptr->Reset();
	}
}


//----------------------------------------------------------------------------
/**
*/
VOID			
PVPScene::initPVP()
{
}


//----------------------------------------------------------------------------
/** 
*/
VOID			
PVPScene::processInfo( DWORD dwTick )
{
}


//----------------------------------------------------------------------------
/**
*/
VOID			
PVPScene::showPVPInfoDialog()
{
        uiBattleScoreMan* battle_score_manager_ptr = 
            static_cast<uiBattleScoreMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_BATTLE_SCORE));
		if (battle_score_manager_ptr)
		{
            battle_score_manager_ptr->set_toggle_ui_open(true);
			battle_score_manager_ptr->SendPacketBattleResultSYN();
		}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
PVPScene::updatePVPInfo()
{
}

//----------------------------------------------------------------------------
/**
*/
VOID			
PVPScene::InvincibleToAllUser()
{
	PVP_PLAYER_INFO_ITR itr = this->m_PVPManager.GetBegin();
	
	while ( itr != this->m_PVPManager.GetEnd() )
	{
		PVPPlayerInfo * pInfo = itr->second;

		Player * pPlayer = (Player *)g_ObjectManager.GetObject( pInfo->dwPlayerKey );
		if ( pPlayer )
		{
			pPlayer->SetInvincible(TRUE);

			if ( g_pHero && 
				 g_pHero->GetObjectKey() == pPlayer->GetObjectKey() )
			{
				g_HeroInput.SetCannotAttack(TRUE);
			}
		}

		itr++;
	}
}

//----------------------------------------------------------------------------
/**
*/
VOID			
PVPScene::UnInvincibleToAllUser()
{
	PVP_PLAYER_INFO_ITR itr = this->m_PVPManager.GetBegin();
	
	while ( itr != this->m_PVPManager.GetEnd() )
	{
		PVPPlayerInfo * pInfo = itr->second;

		Player * pPlayer = (Player *)g_ObjectManager.GetObject( pInfo->dwPlayerKey );
		if ( pPlayer )
		{
			pPlayer->SetInvincible(FALSE);

			if ( g_pHero && 
				 g_pHero->GetObjectKey() == pPlayer->GetObjectKey() )
			{
				g_HeroInput.SetCannotAttack(FALSE);
                g_HeroInput.SetAutoAttack(FALSE);
			}
		}

		itr++;
	}
}

//----------------------------------------------------------------------------
/**
*/
void	
PVPScene::ProcessKeyInput()
{
	BattleScene::ProcessKeyInput();

	if ( TRUE == m_bCheckPressAnyKey )
	{
		if ( TRUE == g_Input.IsPressAnyKey() )
		{
			if ( !BIT_CHECK(this->m_packetStatus, PVP_PACKET_STATUS_PRESS_KEY) )
			{
				BIT_ADD(this->m_packetStatus, PVP_PACKET_STATUS_PRESS_KEY);

				MSG_CG_PVP_PRESS_KEY_SYN SendPacket;
				GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(SendPacket));
			}
		}
	}
}

RELATIONKIND::TYPE PVPScene::GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInPVPZone( pObject, bForceEnemy );	
}
