/*!	
 *	\file		ssqscene.cpp file
 *	\brief		ssqscene 관련 클래스 구현 부
 *	\author		김경표 (lammy@webzen.co.kr)
 *	\version	2009/01/12	김경표 - doxygen command 작성
 */
#include "SunClientPrecompiledHeader.h"
#include "SceneBase.h"
#include "SsqScene.h"
#include "GameFramework.h"
#include "Player.h"
#include "Monster.h"
#include "mapeditscene.h"
#include "TerrainObject.h"
#include "varpacket.h"
#include "Camera.h"
#include "ItemManager.h"
#include "HotKeyManager.h"
#include "HeroActionInput.h"
#include "Battle.h"
#include "SoundBGM.h"
#include "SolarDialog.h"
#include "LoadScene.h"
#include "Map.h"
#include "GameFunc.h"
#include "ObjectManager.h"
#include "RewardDialog.h"
#include "Hero.h"
#include "InterfaceManager.h"
#include "Clone.h"
#include "CloneManager.h"
#include "ProjectileManagerEx.h"
#include "ChatDialog.h"
#include "HeroGaugeDialog.h"
#include "TriggerParser.h"
#include "HelpMessageParser.h"
#include "Message.h"
#include "MapUnitCacher.h"
#include "NPCInfoParser.h"
#include "SkillEffectInfoParser.h"
#include "MouseHandler.h"
#include "SystemDialog.h"
#include "EquipmentContainer.h"
#include "SkillInfoParser.h"
#include "CommonCharacterFormula.h"
#include "Mouse.h"
#include "ItemInfoParser.h"
#include "PlayerAttributes.h"
#include <SCItemSlot.h>
#include "VillageScene.h"
#include "NPC.h"
#include "EventInfoParser.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiGuildCreate/uiGuildCreate.h"
#include "uiGuildWindow/uiGuildWindow.h"
#include "uiGuildMenu/uiGuildMenu.h"
#include "uiGuildMain/uiGuildMain.h"
#include "uiItemCompositeMan/uiItemCompositeMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiChaoSystemMan/uiChaoSystemMan.h"
#include "TargetDialog.h"
#include "PlayerMonster.h"
#include "tiredsystemdialog.h"
#include "uiMissionRelayMan/uiMissionRelayMan.h"
#include "MissionRelay.h"
#include "BattleLobby.h"
#include "Global.h"
#include "BGMSoundInfoParser.h"
#include "MouseCursor.h"
#include "KeyQueueManager.h"
#include "StateInfoParser.h"
#include "RadarInfoParser.h"
#include "radardialog.h"
#include "radarplusdialog.h"
#include "RadarFullDialog.h"
#include "CastDialog.h"
#include "CastLeaveFieldDialog.h"
#include "uiHelpMan/uiHelpMan.h"
#include "MapInfoParser.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiQuestMan/uiQuestMan.h"
#include "QuestManager_Concrete.h"
#include "MissionManager_Concrete.h"
#include "uiPartyMan/uiPartyMan.h"
#include "ResourceManager.h"
#include "CursorChangeTransact.h"
#include "Application.h"
#include "GlobalFunc.h"
#include "SkillEffectInfoParser.h"
#include "MissionScene.h"
#include "AppearanceManager.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "InstanceDungeonScene.h"
#include "uiSystemMan/uiSystemMan.h"
#include "uiLoginMan/uiLoginMan.h"
#include "AreaTileLoadHelper.h"
#include "OptimizationManager.h"
#include "uiSSQMan/uiSSQManager.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiMenuMan/uiMenuMan.h"
#include "MainDialog.h"

///////////////////////////////////////////////////////////////

SsqScene		g_SsqScene;
static int      mode = 0;

SsqScene::SsqScene(void) 
{
	m_SceneType = SCENE_TYPE_SSQ;
}

SsqScene::~SsqScene(void)
{
}

WzVector SsqScene::GetRandomPosition()
{
	int iTileNum=g_pSunTerrain->x_pPathFinder->GetNumPathTile();
	
	return g_pSunTerrain->GetRandomPosInTile(rand()%iTileNum);
}

BOOL SsqScene::Init( void )
{
	BattleScene::Init();

	if ( g_pMap )
	{
		GlobalFunc::PLAY_BGM(MAP_TYPE_DEFAULT, g_pMap->GetCurrentFieldID(), eZONETYPE_INSTANCE );
	}

	m_dwThunderboltTime=5000+rand()%5000;


	m_fOldBlurRatio = g_pSunRenderer->x_pManagerPostEffect->GetMotionBlurRatio();
	g_pSunRenderer->x_pManagerPostEffect->SetMotionBlurRatio(0.5f);


	m_dwEarthQuakeTime=9000;
	m_dwOverCoatingTime=0;

	if( GENERALPARAM->IsNet() == FALSE )
	{
		if( m_iTerrainObjIndexArray.Size() )
		{
			m_pTerrain = (TerrainObject *)g_ObjectManager.GetObject(m_iTerrainObjIndexArray[0]);
			m_iTerrainObjIndexArray.Erase(0);
		}

		// 임시로 버서커에게 칼을 들려준다.
		if (g_pHero->GetClass() == eCHAR_BERSERKER) 
		{
			SCItemSlot ItemSlot;
			ItemSlot.SetCode(1);

			EquipmentContainer* pContainer = static_cast<EquipmentContainer*>(g_pHero->GetSlotContainer(SI_EQUIPMENT));
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

            ItemSlot.SetCode(51000); // 		
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

			ItemSlot.SetCode(51028); // 		
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

			ItemSlot.SetCode(51002); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

			ItemSlot.SetCode(51003); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

			ItemSlot.SetCode(51004); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

			ItemSlot.SetCode(51005); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

			ItemSlot.SetCode(51006); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);	

			ItemSlot.SetCode(60000); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_RING1, ItemSlot);	

			ItemSlot.SetCode(51001); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	

            g_pHero->RefreshEquipData();
			g_pHero->SetCurrentAttackStyle(eSTYLE_ROLLANDS_DOWN);

		}

		// 드래곤도 칼주자
		if (g_pHero->GetClass() == eCHAR_DRAGON) 
		{
			SCItemSlot ItemSlot;
			ItemSlot.SetCode(31);

			EquipmentContainer* pContainer = static_cast<EquipmentContainer*>(g_pHero->GetSlotContainer(SI_EQUIPMENT));
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);			


			ItemSlot.SetCode(51007); // 		
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

			ItemSlot.SetCode(51029); // 		
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

			ItemSlot.SetCode(51008); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	


			ItemSlot.SetCode(51009); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

			ItemSlot.SetCode(51010); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

			ItemSlot.SetCode(51011); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

			ItemSlot.SetCode(51012); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

			ItemSlot.SetCode(51013); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);	
            
            g_pHero->RefreshEquipData();

			g_pHero->SetCurrentAttackStyle(eSTYLE_HACKSTER_KNOCKBACK_TWOHAND);

		}
		// 발키리 석궁주기
		if (g_pHero->GetClass() == eCHAR_VALKYRIE) 
		{

			SCItemSlot ItemSlot;
			//ItemSlot.SetCode(91); // 석궁
			ItemSlot.SetCode(112); // 에테르

			EquipmentContainer* pContainer = static_cast<EquipmentContainer*>(g_pHero->GetSlotContainer(SI_EQUIPMENT));
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);	

			ItemSlot.SetCode(51021); // 		
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

			ItemSlot.SetCode(51031); // 		
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

			ItemSlot.SetCode(51022); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	


			ItemSlot.SetCode(51023); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

			ItemSlot.SetCode(51024); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

			ItemSlot.SetCode(51025); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

			ItemSlot.SetCode(51026); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

			ItemSlot.SetCode(51027); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);	

            g_pHero->RefreshEquipData();
			g_pHero->SetCurrentAttackStyle(eSTYLE_ETHER_NORMAL);
		}

		if (g_pHero->GetClass() == eCHAR_MAGICIAN) 
		{
			SCItemSlot ItemSlot;
			ItemSlot.SetCode(154);
			g_pHero->SetCurrentNomalAttackStyle(eWEAPONTYPE_ORB);

			EquipmentContainer* pContainer = static_cast<EquipmentContainer*>(g_pHero->GetSlotContainer(SI_EQUIPMENT));
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);		

			ItemSlot.SetCode(51014); // 		
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

			ItemSlot.SetCode(51030); // 		
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

			ItemSlot.SetCode(51015); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	


			ItemSlot.SetCode(51016); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

			ItemSlot.SetCode(51017); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

			ItemSlot.SetCode(51018); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

			ItemSlot.SetCode(51019); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

			ItemSlot.SetCode(51020); // 
			pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);

            g_pHero->RefreshEquipData();
			ItemSlot.SetCode(138); // 
			SCSlotContainer*inven_container = g_pHero->GetSlotContainer(SI_INVENTORY);
			inven_container->InsertSlot(0, ItemSlot);

		}
	}

	WzVector vPos;
	SetVector(&vPos, 199.0f, 201.0f, 0.0f);

    m_hDoomEffect = g_pEffectManager->CreateEffect(StrToWzID("O026"), vPos);
	
	g_OptimizationManager.SetPerformanceMode(TRUE);

	GET_DIALOG(IM_SSQ_MANAGER::SSQ_SYSTEMTIME)->ShowInterface(TRUE);
	uiSSQSystemWindow* pDlg = GET_CAST_DIALOG(uiSSQSystemWindow, IM_SSQ_MANAGER::SSQ_SYSTEMTIME);
	if (pDlg)
	{
		pDlg->SetStart(FALSE);
		pDlg->SetStartCount(0);
	}

	GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG)->ShowDialog(FALSE);
	GET_DIALOG(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)->ShowDialog(FALSE);

	m_bGambleStart = FALSE;
	g_bObserverMode = FALSE;

	MainDialog* pMainDlg = GET_CAST_DIALOG(MainDialog, IM_MENU_MANAGER::MAIN_DLG);

	if (pMainDlg)
	{
		pMainDlg->ShowObserverButton(FALSE);
	}


	//////////////////////////////////////////////////////////////////////////
    m_iOldEffectLevel = GAMEOPTION->GetGameOption().GetOptionValue(nOptionType::EFFECT_QUALITY);

	g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\DeleteEffectResource.txt");
	g_pEffectManager->UnLoadEffectFromFile("Data\\Effect\\effectresource3.txt");

	m_bOldShadowVal = GAMEOPTION->GetShadowOn();
	GAMEOPTION->SetShadowOn(FALSE);

	//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

void SsqScene::DeleteDoomEffect()
{
	g_pEffectManager->RemoveEffectPtr( m_hDoomEffect );
}


BOOL SsqScene::Process( DWORD dwTick )
{
	BOOL bRet = BattleScene::Process( dwTick );

	if( m_bGambleStart == TRUE )
	{
		m_dwGambleTime += dwTick;

		if( m_dwGambleTime >= 5000 )
		{
			m_dwGambleTime = 0;
			GET_DIALOG( IM_SSQ_MANAGER::SSQ_DICE_WINDOW )->ShowInterface( TRUE );
			m_bGambleStart = FALSE;
		}
	}

	m_dwOverCoatingTime+=dwTick;

	if ( GENERALPARAM->IsNet() ==FALSE)
	{

		if(m_dwOverCoatingTime>=60000)
		{
			CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(m_hDoomEffect);
			if(pBody)
			{
				BOOL rt=pBody->IsShow();
				pBody->SetShow(FALSE);
			}

		}
	}

	return bRet;
}

void SsqScene::CreateThunderBolt( const WzVector & pos )
{
	AddBattleTimeSerial();

	SetBrightDelay( 100 );

	WzVector vTarget = pos;
	vTarget.z = 20.0f;

	GameFunc::PickTerrainMesh( vTarget, vTarget );


    cProjectileChaseTarget* pProjectile = new cProjectileChaseTarget();
    pProjectile->SetTargetPosition( vTarget);
    pProjectile->SetLifeTime( 3000 );
    pProjectile->SetActiveDelayTime( 300 );
    pProjectile->CreateEffect( StrToWzID("0025"), 0, 0 );
    g_ProjectileManagerEx.InsertProjectile( pProjectile );
}

void SsqScene::ProcessDamage( const DAMAGE_INFO & DamageInfo, DWORD dwSkillCode )
{
	Character * pChar =  (Character *)g_ObjectManager.GetObject( DamageInfo.m_dwTargetKey );

	if( pChar == NULL )
		return;

	AddBattleTimeSerial();

	pChar->DoPeriodicDamage( DamageInfo.m_wDamage, DamageInfo.m_dwTargetKey, 
							 DamageInfo.m_dwTargetHP, GetBattleTimeSerial(), dwSkillCode );
}

TerrainObject * SsqScene::FindTerrainObject( int index )
{
	for( int i = 0; i < m_iTerrainObjIndexArray.Size(); i++ )
	{
		TerrainObject * pTerrain = (TerrainObject*)g_ObjectManager.GetObject( m_iTerrainObjIndexArray[i] );
		
		if( pTerrain == NULL )
			continue;

		if( pTerrain->GetID() == index )
			return pTerrain;
	}

	return NULL;
}

void SsqScene::AddEarthQuakeIndex( int index )
{
	m_EarthQuakeVec.push_back( index );

	TerrainObject * pTerrain = FindTerrainObject( index );
	if( pTerrain == NULL )
		return;

	pTerrain->SetNextState( STATE::EARTH_QUAKE, g_CurTime );
	pTerrain->CommitChangeState(g_CurTime);
}

void SsqScene::ProcessEarthQuake()
{
	for( size_t i = 0; i < m_EarthQuakeVec.size(); i++ )
	{
		DWORD index = m_EarthQuakeVec[i];

		TerrainObject * pTerrain = FindTerrainObject( index );

		if( pTerrain )
		{
			if( pTerrain->GetCurState() == STATE::EARTH_QUAKE )
			{
				pTerrain->SetFallDown(TRUE);
			}
		}
	}

	m_EarthQuakeVec.clear();
}

//----------------------------------------------------------------------------
/**
*/
void  
SsqScene::AddTerrainObjIndex(int iIndex)
{
	m_iTerrainObjIndexArray.Append(iIndex);
}

void 
SsqScene::Release( void )
{
	BattleScene::Release();
	m_iTerrainObjIndexArray.Clear();
	g_Camera.EndVibration();


	g_pSunRenderer->x_pManagerPostEffect->SetMotionBlurRatio(m_fOldBlurRatio);
	g_pSunRenderer->x_pManagerPostEffect->TurnOffPostEffect(PET_MOTION_BLUR);

	GET_DIALOG(IM_SSQ_MANAGER::SSQ_SYSTEMTIME)->ShowInterface(FALSE);

	MainDialog* pMainDlg = GET_CAST_DIALOG(MainDialog, IM_MENU_MANAGER::MAIN_DLG);

	if (pMainDlg)
	{
		pMainDlg->ShowObserverButton(FALSE);
	}

	g_bObserverMode = FALSE;
	g_IsBattle		= FALSE;

	//////////////////////////////////////////////////////////////////////////
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

	// 이펙트 기본 레벨 설정
	g_pEffectManager->SetBaseEffectLevel( 3 - m_iOldEffectLevel );


	GAMEOPTION->SetShadowOn(m_bOldShadowVal);
	//////////////////////////////////////////////////////////////////////////
}

void SsqScene::ProcessKeyInput_Hero( DWORD dwTick )
{
	BattleScene::ProcessKeyInput_Hero( dwTick );

#ifdef	_DEBUG
	//if(g_Input.GetState(DIK_F1, KS_UP))
	//{		
 //       PLAYER_ACTION action;
 //       action.ActionID		= ACTION_KNOCKBACK;
 //       action.dwTimeSerial = GetBattleTimeSerial();
 //       action.KNOCKBACK.fInitialSpeed = g_KnockBackInitialSpeed;

	//	action.KNOCKBACK.vCurPos.wvPos = g_pHero->GetPosition();
	//	action.KNOCKBACK.vCurPos.wTile =-1;

	//	action.KNOCKBACK.vDestPos.wvPos    = g_pHero->GetPosition() + ( g_pHero->GetDirection() * -5.0f );
	//	action.KNOCKBACK.vDestPos.wTile    = -1;

 //       action.KNOCKBACK.dwEndTime   = g_CurTime + 10000;
	//	g_pHero->PutAction(&action);
	//}

	if(g_Input.GetState(DIK_F2, KS_UP))
	{
		uiSSQSystemWindow * pDlg = GET_CAST_DIALOG( uiSSQSystemWindow, IM_SSQ_MANAGER::SSQ_SYSTEMTIME );

		if( pDlg )
		{
			pDlg->ClearCount();
			pDlg->SetStart( TRUE );
			pDlg->SetStartCount( 10 );
		}
	}

	if(g_Input.GetState(DIK_F3, KS_UP))
	{
		uiSSQRewardWindow * pDlg = GET_CAST_DIALOG( uiSSQRewardWindow, IM_SSQ_MANAGER::SSQ_REWARD_WINDOW );

		if( pDlg )
		{
			SSQ_REWARD_INFO Info( 100, 100, 100, 100 );

			if( pDlg )
			{
				pDlg->SetType( REWARD_WIN );
				pDlg->SetRewardInfo( Info );
				pDlg->ShowInterface( TRUE );
			}
		}
	}

	if( g_Input.GetState( DIK_F4, KS_UP ) )
	{
		static BOOL bShow = FALSE;
		bShow = !bShow;
		GET_DIALOG( IM_SSQ_MANAGER::SSQ_DICE_WINDOW )->ShowInterface( bShow );
	}

	if( g_Input.GetState( DIK_F5, KS_UP ) )
	{
		g_SoundBGM.Stop();
		g_SoundBGM.Play( 29 );
	}

	if( g_Input.GetState( DIK_F6, KS_UP ) )
	{
		WzVector vPos = g_pHero->GetPosition();

		vPos.z = 20.0f;

		WzVector vTarget;

		float ft;
		WzRay	ray;
		ray.m_wvOrigin = vPos;
		ray.m_wvOrigin.x += 0.5f;
		ray.m_wvOrigin.z += 3.0f;

		SetVector( &ray.m_wvDirection, 0.0f, 0.0f, -1.0f );

		int result = g_pSunTerrain->PickTerrainMesh( &ray, &ft );

		if( result < 0 )
		{

		}
		else
		{
			vTarget = ray.m_wvOrigin + ray.m_wvDirection * ft;
		}
	}

	if( g_Input.GetState( DIK_F7, KS_UP ) )
	{
		g_bObserverMode = !g_bObserverMode;
	}

	if( g_Input.GetState( DIK_F8, KS_UP ) )
	{
		SetShowInterfaceForObserverMode( FALSE );
	}

#endif
}

RELATIONKIND::TYPE SsqScene::GetRelationKindOfCharacter( Character* pObject, BOOL bForceEnemy )
{
	return GameFunc::GetRelationKindOfPlayerInSSQZone( pObject, bForceEnemy );
}

void SsqScene::Func_SendPacket_()
{
	MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN sync;
	GlobalFunc::SendPacketToGameServer( &sync, sizeof(sync) );
}

void	SsqScene::Func_OnRecv_( const MSG_CG_ETC_GET_BATTLE_OPEN_TIME_ACK * pMsg )
{
	const nsSSQ::SSQTicketInfo & rInfo = pMsg->m_SSQTicketInfo;
	m_SSQTicketTimeInfoCtrl.QueryResult( rInfo );
}

bool	SsqScene::Func_Output_with_internal_timer(TCHAR * szMessage, bool is_only_time /* = false */)
{
	// client timer "cycle 1sec"

	//	#1 - GetStatus()는 GetTicketTimeInfo()의 호출뒤 상태가 바뀔 수 있으므로 먼저 호출해 둘 것.
	const SYSTEMTIME & rCurrentTimeInfo = m_SSQTicketTimeInfoCtrl.GetTicketTimeInfo();
	// 두가지 정의된 타입 외 나머지 타입 'eTM_None', 'eTM_Transaction'상태에서는 항상 0:0:0의 결과를 보일 것이다.

	//	#2 - Query IO
	Func_CSQuery_Check();

	typedef nsSSQ::SSQTicketTimeInfo	SSQTI;

	//	#3 - Message Processing
	const SSQTI::eTM current_state = m_SSQTicketTimeInfoCtrl.GetStatus();
    CHAR buffer[1024] = {0,}; // temp for sample

	if( current_state == SSQTI::eTM_EnterableTimeout )
	{
		_snprintf( buffer, sizeof(buffer)
            , g_InterfaceManager.GetInterfaceString((is_only_time)?2472:11352)//2472 (시작 %d시간 %d분전), 11352 (입장까지 남은시간: %d시간 %d분 %d초)
			, rCurrentTimeInfo.wHour
			, rCurrentTimeInfo.wMinute+((is_only_time)?1:0)
			, rCurrentTimeInfo.wSecond
			);
	}
	else// if( current_state == SSQTI::eTM_CloseTimeout )
	{
		_snprintf( buffer, sizeof(buffer)
            , g_InterfaceManager.GetInterfaceString((is_only_time)?2471:11353)//2471 (입장가능), 11353 (입장종료까지 남은시간: %d분 %d초)
            , rCurrentTimeInfo.wMinute
			, rCurrentTimeInfo.wSecond
			);
	}

    if (is_only_time && current_state == SSQTI::eTM_EnterableTimeout && rCurrentTimeInfo.wHour < 1)
    {		
        _snprintf( buffer, sizeof(buffer)
            , g_InterfaceManager.GetInterfaceString(2473)//2473 (시작 %d분전)
		    , (rCurrentTimeInfo.wMinute+1));
    }
	//	make_string
	_tcscpy( szMessage, buffer );

    if (current_state == SSQTI::eTM_CloseTimeout)
    {
        return true;//입장가능
    }
    else
    {
        return false;//입장불가능
    }
}

void	SsqScene::Func_CSQuery_Check()
{
	typedef nsSSQ::SSQTicketTimeInfo	SSQTI;
	const SSQTI::eTM current_state = m_SSQTicketTimeInfoCtrl.GetStatus();
	switch( current_state )
	{
	case SSQTI::eTM_None:
		//	need send packet "MSG_CG_ETC_GET_BATTLE_OPEN_TIME_SYN"
		Func_SendPacket_();
		m_SSQTicketTimeInfoCtrl.SetStatus_Queried();	// change to 'eTM_Transaction'
		break;
	case SSQTI::eTM_Transaction:
		break;
	}
}

bool SsqScene::is_ssq_state()
{
    typedef nsSSQ::SSQTicketTimeInfo SSQTI;

    const SYSTEMTIME & rCurrentTimeInfo = m_SSQTicketTimeInfoCtrl.GetTicketTimeInfo();
    typedef nsSSQ::SSQTicketTimeInfo	SSQTI;

    //	#3 - Message Processing
    const SSQTI::eTM current_state = m_SSQTicketTimeInfoCtrl.GetStatus();

    if (current_state == SSQTI::eTM_EnterableTimeout || current_state == SSQTI::eTM_CloseTimeout)
    {
        if (rCurrentTimeInfo.wHour <= 0)
        {
            if (rCurrentTimeInfo.wMinute <= 4)
            {
                return true;
            }
            else if (rCurrentTimeInfo.wMinute == 5 && rCurrentTimeInfo.wSecond <= 0)
            {
                return true;
            }            
        }
    }
    return false;
    /*const SSQTI::eTM current_state = m_SSQTicketTimeInfoCtrl.GetStatus();
    return (current_state == SSQTI::eTM_EnterableTimeout);*/
}

void SsqScene::GetSSQTimeInfo(OUT SYSTEMTIME& system_time, OUT nsSSQ::SSQTicketTimeInfo::eTM& current_state)
{
    system_time = m_SSQTicketTimeInfoCtrl.GetTicketTimeInfo();
    current_state = m_SSQTicketTimeInfoCtrl.GetStatus();
    return;
}