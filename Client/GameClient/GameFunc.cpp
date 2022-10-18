//------------------------------------------------------------------------------
//  GameFunc.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "globalfunc.h"
#include "Object.h"
#include "character.h"
#include "Hero.h"
#include "gamefunc.h"
#include "ObjectManager.h"
#include "Map.h"
#include "Monster.h"
#include "WeaponSoundInfoParser.h"
#include "MouseCursor.h"
#include "HeroActionInput.h"
#include "MouseHandler.h"
#include "mapObject.h"
#include "OptimizationManager.h"
#include "ShopDialog.h"
#include "GmManager.h"
#include "StyleInfoParser.h"
#include "ChatDialog.h"
#include "uiVendorBuy/uiVendorBuy.h"
#include "uiTrade/uiTrade.h"
#include "uiTradeMan/uiTradeMan.h"
#include "uiPartyLeavePopup/uiPartyLeavePopup.h"
#include "uiVendorMan/uiVendorMan_def.h"
#include "uiVendorSell/uiVendorSell.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemFunction.h"
    #include "GuildSystem/uiGuildMan.h"
    #include "GuildSystem/uiGuildMenu.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
    #include "uiGuildMenu/uiGuildMenu.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "quickslot.h"
#include "Map.h"
#include "Camera.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDef.h"
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "MouseCursor.h"
#include "HeroVoiceInfoParser.h"
#include "SoundEffect.h"
#include "NPCPortalDialog.h"
#include "quickcontainer.h"
#include "uiHelpMan/uiHelpMan.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "ProjectileManagerEx.h"
#include "SkillInfoParser.h"
#include "SkillEffectInfoParser.h"
#include "SelectCompositeMenu.h"
#include "uiQuestMan/uiQuestMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "NPCInfoParser.h"
#include "uiEventMan/uiEventMan.h"
#include "uiEventWindow/uiEventWindow.h"
#include "QuestManager_Concrete.h"
#include "CompositeNPCInfoParser.h"
#include "uiitemcompositeman/uiItemCompositeMan.h"
#include "uiItemTotalComposite/uiItemTotalComposite.h"
#include "uiBattleZoneReadyCHuntingConfig/uiBattleZoneReadyCHuntingConfig.h"
#include "CursorChangeTransact.h"
#include "GameFramework.h"
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h"
#include "uiMissionRankList2/uiMissionRankList2.h"
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#include "uiMissionRankList/uiMissionRankList.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#include "uiVendorList/uiVendorList.h"
#include "ItemManager.h"
#include "uiCashItemShopMan/uiCashItemShopMan.h"
#include "uiCashItemShopNpcPopup/uiCashItemShopNpcPopup.h"
#include "PlayerMonster.h"
#include "uiCashItemShopPresentDlg/uiCashItemShopPresentDlg.h"
#include "uiCashItemShopPresentCheck/uiCashItemShopPresentCheck.h"
#include "uiCashItemShopPresentComplete/uiCashItemShopPresentComplete.h"
#include "uiCashItemShopPackageDlg/uiCashItemShopPackageDlg.h"
#include "uiCashItemShopPackageCheck/uiCashItemShopPackageCheck.h"
#include "uicashItemShopPackageComplete/uicashItemShopPackageComplete.h"
#include "uiPasswordDlg/uiPasswordDlg.h"
#include "EquipmentContainer.h"

#include "uiContinentMapEP2/ContinentMapEp2Dialog.h"

#include "ContinentMapDlg.h"
#include "uiRadarMan/uiRadarMan.h"

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
#include "uiGuildEventDialog/uiGuildEventDialog.h"
#endif

#include "NPC.h"

#include "ui_XMas_DonationConfirm/ui_XMas_DonationConfirm.h"
#include "ui_XMas_Confirm/ui_XMas_Confirm.h"
#include "ui_XMas_SantaDlg/ui_XMas_SantaDlg.h"
#include "SunEventInfoParser.h"
#include "RewardInfoList.h"
#include "ItemInfoParser.h"
#include "uiSystemMan/uiSystemMan.h"

#ifdef _YJ_FISHING_SYSTEM_000933
#include "uiFishingShop/uiFishingShop.h"
#endif //_YJ_FISHING_SYSTEM_000933

#include "uiQuestCommonForward/uiQuestCommonForward.h"
#include "uiBattleZoneMan/uiBattleZoneMan_def.h"
#include "NpcImageInfo.h"
#include "uiBuffEvent/uiBuffEvent.h"

#include "uiLottoMan/uiLottoMan.h"
#include "uiLottoWindow/uiLottoWindow.h"

#include "PKScriptParser.h"

#include "uiEventFriendWindow.h"
#include "uiCharCustomizeMan/uiCharCustomizeMan.h"
#include "uiCharCustomizeDlg/uiCharCustomizeDlg.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "OptionListParser.h"

#include "Define.ShopServer.h"

#ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
#include "../Interface/uiVendorMan/uiVendorSearch/uiVendorSearch.h"
#endif //_NA_005988_20120901_VENDOR_SEARCH_SYSTEM
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
  #include "uiDominationTowerMan/uiDominationTowerMan.h"
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "PacketStruct_CG_SmartNPC.h"
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StatuePlayer.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

eZONETYPE GameFunc::ConvertServerZoneType(SCENE_TYPE eSceneType)
{
	//매칭 되는 타입이 없을 경우 서버에서 디폴트로 사용 된다.
	eZONETYPE eServerSceneType=eZONETYPE_MAX;

	switch(eSceneType)
	{
	case SCENE_TYPE_LOBBY:
		{
			eServerSceneType=eZONETYPE_LOBBY;
		}
		break;

	case SCENE_TYPE_VILLAGE:
		{
			eServerSceneType=eZONETYPE_VILLAGE;
		}
		break;

	case SCENE_TYPE_CHARSELECT:
		{
			eServerSceneType=eZONETYPE_CHARSELECT;
		}
		break;

	case SCENE_TYPE_MISSION:
		{
			eServerSceneType=eZONETYPE_MISSION;
		}
		break;

	case SCENE_TYPE_PVP:
		{
			eServerSceneType=eZONETYPE_PVP;
		}
		break;

	case SCENE_TYPE_EVENT:
		{
			eServerSceneType=eZONETYPE_EVENT;
		}
		break;

	case SCENE_TYPE_FIELD:
		{
			eServerSceneType=eZONETYPE_FIELD;
		}
		break;

	case SCENE_TYPE_CHUNTING:
		{
			eServerSceneType=eZONETYPE_CHUNTING;
		}
		break;

	case SCENE_TYPE_INSTANCE_DUNGEON:
	case SCENE_TYPE_DOMINATION:
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case SCENE_TYPE_SONNENSCHEIN:
#endif //_NA_008334_20150608_SONNENSCHEIN

		{
			eServerSceneType=eZONETYPE_INSTANCE;
		}
		break;


	}

	return eServerSceneType;
}

//------------------------------------------------------------------------------
/**
*/
Character* GameFunc::GetPickedCharacter(WzRay *pRay,DWORD dwPickingOption)
{
	if (g_pMap) 
	{		
		if (VectorLength2(&pRay->m_wvDirection) >= 0.04f) 
		{
			return g_pMap->GetPickedCharacter(pRay,dwPickingOption);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
Item * GameFunc::GetPickedItem(WzRay* pRay)
{
	if (g_pMap)
	{	
		if (VectorLength2(&pRay->m_wvDirection) >= 0.04f) 
		{
			return g_pMap->GetPickedItem(pRay);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}


//----------------------------------------------------------------------------
/**
*/
MapObject *		
GameFunc::GetPickedMapUnitObject(WzRay* pRay)
{
	if (g_pMap)
	{	
		if (VectorLength2(&pRay->m_wvDirection) >= 0.04f) 
		{
			return g_pMap->GetPickedUnitMapObject(pRay);
		}
		else
		{
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL GameFunc::GetClickedPosition(WzVector* OUT pPosition)
{
	// mouse ray를 잘못넣어서 뻑나지 않도록.
	if (VectorLength2(&g_wrMouseRay.m_wvDirection) <= 0.04f)
	{
		return FALSE;
	}

	float fT;
	

	if(SCENE_TYPE_DOMINATION == GameFramework::GetCurScene()
#ifdef _NA_008334_20150608_SONNENSCHEIN
        || SCENE_TYPE_SONNENSCHEIN == GameFramework::GetCurScene()
#endif //_NA_008334_20150608_SONNENSCHEIN
        )
	{
		int iTile = g_pSunTerrain->PickPathTile(&g_wrMouseRay, &fT);
		if(0 > iTile)
			return FALSE;
	}
	else
	{
		WzVector wvNormal;
		int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &g_wrMouseRay, &fT ,TRUE,TMGO_HAZE, &wvNormal);
		if( iTerrainMeshFocussing < 0 )
		{
			return FALSE;
		}	
	}

	*pPosition = g_wrMouseRay.m_wvOrigin + g_wrMouseRay.m_wvDirection * fT;

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/** 길찾기 위치에서 실제 포지션을 얻어온다.
*/
BOOL GameFunc::GetVisiblePosition(WzVector * IN OUT pInOutVisiblePos )
{
    // 타겟 위치 알기
    BOOL bRet = FALSE;
    float fT = 0.f;

    if ( !g_pMap || !pInOutVisiblePos )
    {
        return FALSE;
    }

    int iMesh = 
        g_pMap->GetWorldBase()->PathExp_GetMeshToStand(*pInOutVisiblePos, &fT, 10.f);
    
    //딛을수 있는 곳인가
    if ( iMesh >= 0 && fT )
    {
        pInOutVisiblePos->z -= fT;
    }
    
    return bRet;
}

RELATIONKIND::TYPE GameFunc::GetGuildRelationShip( Player * pPlayer, BOOL bForceEnemy )
{
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
    if( pPlayer->GetGuildRenderInfo().m_RelationType == eGUILD_RELATION_BOTH_HOSTILITY )
    {
        return RELATIONKIND::Enemy;
    }
#else
    if( pPlayer->GetGuildRenderInfo().m_RelationType == eGUILD_RELATION_BOTH_HOSTILITY ||
        pPlayer->GetGuildRenderInfo().m_RelationType == eGUILD_RELATION_PROCLAIM_HOSTILITY )
    {
        return  RELATIONKIND::Enemy;
    }
    else if( pPlayer->GetGuildRenderInfo().m_RelationType == eGUILD_RELATION_ONE_SIDE_HOSTILITY )
    {
        if( bForceEnemy )
            return RELATIONKIND::Enemy;
        else
            return RELATIONKIND::Friend;
    }
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL	
	else if( pPlayer->GetGuildRenderInfo().m_RelationType == eGUILD_RELATION_ALLIENCE )
	{
		return  RELATIONKIND::Friend;
	}

	return RELATIONKIND::None;
}


BOOL GameFunc::IsFriendShip( Player * pPlayer )
{
	uiPartyMan * partyMan = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );

	if (partyMan)
	{
		if( partyMan->IsPartyMember( pPlayer->GetObjectKey() ) )
			return TRUE;
	}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
    {
        return TRUE;
    }
#else
	uiGuildMan * guildMan = GET_DIALOG_MANAGER( uiGuildMan, UIMAN_GUILD );

	if(guildMan)
	{
		if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
			return TRUE;

	}
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

	return FALSE;
}

BOOL GameFunc::GetCheckPlayerOnTile( DWORD dwPlayerTile, DWORD dwHeroTile, DWORD dwTileAttr )
{
	if( BIT_CHECK( dwPlayerTile, dwTileAttr ) && BIT_CHECK( dwHeroTile, dwTileAttr ) )
	{
		return TRUE;
	}

	return FALSE;
}

RELATIONKIND::TYPE	GameFunc::GetRelationOfMonster( Monster * pMonster, BOOL bForceEnemy )
{
#ifdef _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
    if (g_pHero != NULL && pMonster != NULL)
    {
        DWORD dwMonsterAtt = pMonster->GetCurrentTileAtt(FALSE);
        DWORD dwHeroAtt = g_pHero->GetCurrentTileAtt(FALSE);

        if (BIT_CHECK(dwHeroAtt,PTA_SAFETY_ZONE) || BIT_CHECK(dwMonsterAtt, PTA_SAFETY_ZONE))
        {
            return RELATIONKIND::Friend;
        }
    }
#endif //_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_    
    // _NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    BASE_NPCINFO* npc_info = pMonster->GetMonsterInfo();
    if (npc_info != NULL)
    {
        if ((npc_info->m_byGrade == eNPC_FRIEND_MAPOBJECT_NPC) ||
            (npc_info->m_byGrade == eNPC_FRIEND_MONSTER))
        {
            return RELATIONKIND::Friend;
        }        
    }

	if( pMonster->GetMonsterType() == MONSTER_TYPE_TOTEM )
		return RELATIONKIND::None;
	
	if(MONSTER_TYPE_SETTING_MAPOBJECT == pMonster->GetMonsterType()) //설치형 몬스터라면
	{
		Object* pTemp = g_ObjectManager.GetObject(MAP_OBJECT_KEY + pMonster->GetMapObjectKey());
		if(pTemp && pTemp->IsKindOfObject(MAP_OBJECT))
		{
			//해당 설치형 몬스터와 엮여있는 맵오브젝트가 때릴수 없는 상태라면
			MapObject* pMapObj = (MapObject*)pTemp;
			if(false == pMapObj->GetbCanHit())
				return RELATIONKIND::None;
		}
	}

	if( pMonster->GetParentId() )
	{
		Player * pPlayer = (Player *)g_ObjectManager.GetObject( pMonster->GetParentId() );

		if( pPlayer )//주인이 있다면 주인과의 관계를 본다.
		{
			return GameFunc::GetRelationOfPlayer(pPlayer, bForceEnemy);
		}
	}
	else if( (g_pHero->GetGuildRenderInfo().m_GuildGuid!=INVALID_GUILDGUID) && g_pHero->GetGuildRenderInfo().m_GuildGuid == pMonster->GetMasterGuildID())
	{
		return RELATIONKIND::Friend;
	}
	else
	{
		return RELATIONKIND::Enemy;
	}

	return RELATIONKIND::None;
}


/*
 *	GameFunc::GetRelationOfPlayer() 함수는 타일별 룰을 통한 플레이어간의 관계를 얻기 위해서 사용된다.
 *  현재 GetRelationKindOfPlayerInField()			- FiledScene
 *	     GetRelationKindOfPlayerInDominationWar()	- DominationWarScene
 *		 GetRelationOfMonster()
 *		 함수에서 호출되거 있으며 GetRelationOfMonster() 에서는 소환수의 주인과의 관계를 알기위해 호출됨
 */
RELATIONKIND::TYPE	GameFunc::GetRelationOfPlayer( Player * pPlayer, BOOL bForceEnemy )
{
#ifdef _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
	// 자신이거나 상대가 개인상점 상태일 경우 
	if( g_pHero && g_pHero->GetObjectKey() == pPlayer->GetObjectKey() )
		return RELATIONKIND::Friend;
	ePLAYER_BEHAVE_STATE behaveState = pPlayer->GetBeHaveState();
	if( behaveState == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE )
		return RELATIONKIND::Vendor;

	// PKScript 정보
	PKOptionInfo& PKInfo = PKScriptParser::Instance()->GetPKOptionInfo();
	
	// PK룰 객체 생성
	static PKRuleForTile PKRuleforTile;

	// 각플레이어 속성으로 체크로 적용할 룰 선택 & 관계를 얻음
	bool bForce = (TRUE == bForceEnemy);
	RELATIONKIND::TYPE relationType = PKRuleforTile.CheckRelationTileAttr(pPlayer, &PKInfo, bForce);
	
	return relationType;
#else
	if( g_pHero && g_pHero->GetObjectKey() == pPlayer->GetObjectKey() )
		return RELATIONKIND::Friend;

	ePLAYER_BEHAVE_STATE behaveState = pPlayer->GetBeHaveState();

	if( behaveState == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE )
		return RELATIONKIND::Vendor;

	if( !g_isPkServer )
		return RELATIONKIND::Friend;

	DWORD dwPlayerAtt	= pPlayer->GetCurrentTileAtt(FALSE);
	DWORD dwHeroAtt		= g_pHero->GetCurrentTileAtt(FALSE);


	PKOptionInfo& PKInfo = PKScriptParser::Instance()->GetPKOptionInfo();
	BYTE IgnorePKTile = PKInfo.m_IgnorePKTile;

	if (bForceEnemy && GetCheckPlayerOnTile(dwPlayerAtt, dwHeroAtt, PTA_FREE_CONFLICT_TILE) && !(IgnorePKTile&PKOptionInfo::ignore_FreePart)) 
	{
		return RELATIONKIND::Enemy;
	}

	if (IsFriendShip(pPlayer))
    {
		return RELATIONKIND::Friend;
    }

#ifdef __LTJ_RELATION_OF_PLAYER_BUGFIX
	// no operation
#else //__LTJ_RELATION_OF_PLAYER_BUGFIX
	RELATIONKIND::TYPE state = GetGuildRelationShip( pPlayer, bForceEnemy );

	if( state != RELATIONKIND::None )
		return state;

	if( bForceEnemy )
		return RELATIONKIND::Enemy;
#endif //__LTJ_RELATION_OF_PLAYER_BUGFIX

	if( GetCheckPlayerOnTile( dwPlayerAtt, dwHeroAtt, PTA_FREE_CONFLICT_TILE ) && !(IgnorePKTile&PKOptionInfo::ignore_FreePart) ) 
	{
		return RELATIONKIND::Enemy;
	}
	else if( GetCheckPlayerOnTile( dwPlayerAtt, dwHeroAtt, PTA_CONFLICT_TILE ) && !(IgnorePKTile&PKOptionInfo::ignore_NomalPart) ) 
	{
		return RELATIONKIND::Enemy;
	}
	else if( GetCheckPlayerOnTile( dwPlayerAtt, dwHeroAtt, PTA_PK_TILE ) && !(IgnorePKTile&PKOptionInfo::ignore_BoundaryPart) ) 
	{
#ifdef __LTJ_RELATION_OF_PLAYER_BUGFIX

		RELATIONKIND::TYPE state = GetGuildRelationShip( pPlayer, bForceEnemy );

		if( state != RELATIONKIND::None )
			return state;

#endif //__LTJ_RELATION_OF_PLAYER_BUGFIX
		if( g_pHero->GetLevel() < PKInfo.m_PKAllowableLV || pPlayer->GetLevel() < PKInfo.m_PKProtectLV )
		{
			return RELATIONKIND::Friend;
		}

#ifdef _CHINA
		if( pPlayer->GetChaoState() == eCHAO_STATE_CHAO )
#else
		if( pPlayer->GetChaoState() == eCHAO_STATE_CHAO || pPlayer->IsAttackBeginPlayer() )
#endif
		{
			return RELATIONKIND::Enemy;
		}

#ifdef __LTJ_RELATION_OF_PLAYER_BUGFIX
		if( bForceEnemy )
			return RELATIONKIND::Enemy;
#endif //__LTJ_RELATION_OF_PLAYER_BUGFIX

		return RELATIONKIND::Friend;
	}
	else if (GetCheckPlayerOnTile( dwPlayerAtt, dwHeroAtt, PTA_DOMINATION_WAR_TILE ))
	{
		if(pPlayer->IsExistGuild())
		{
			if( eGUILD_RELATION_ALLIENCE == pPlayer->GetGuildRenderInfo().m_RelationType )
				return RELATIONKIND::Friend;
			else if(eGUILD_RELATION_NONE == pPlayer->GetGuildRenderInfo().m_RelationType )
				return RELATIONKIND::None;
			else
				return RELATIONKIND::Enemy;
		}
	}
	else
	{
		RELATIONKIND::TYPE state = GetGuildRelationShip( pPlayer, bForceEnemy );

		if( state != RELATIONKIND::None )
			return state;
		else
			return RELATIONKIND::Friend;

	}

	return RELATIONKIND::None;
#endif //_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_	
}

//------------------------------------------------------------------------------
/**
	YH2DO 나중에는 NPC나 파티설정등이 추가 되면 체계적으로 정리해야 함
*/
RELATIONKIND::TYPE GameFunc::GetRelationKindOfHero(Character* pObject,BOOL bForceEnemy)
{
    if (pObject->IsKindOfObject(PLAYER_OBJECT))
	{
		Player *pPlayer = (Player *)pObject;

		ePLAYER_BEHAVE_STATE behaveState = pPlayer->GetBeHaveState();

		if (behaveState == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE)
		{
			return RELATIONKIND::Vendor;
		}

		if (g_pHero) 
		{
			if(pPlayer->GetObjectKey() == g_pHero->GetObjectKey())//이거 먼저 검사해야하지 않나?
			{
				return RELATIONKIND::Friend;
			}			
		}

		if ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_PVP )
		{
			uiPartyMan* partyMan =
				static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

			if(partyMan)
			{
				if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
				{
					return RELATIONKIND::Friend;
				}
				else
					return RELATIONKIND::Enemy;
			}
			else
				return RELATIONKIND::Enemy;
		}
		else if ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ )
		{
				return RELATIONKIND::Enemy;
		}
		else if( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE)
		{
			if(!g_isPkServer)
				return RELATIONKIND::Friend;
			DWORD dwPlayerAtt=pPlayer->GetCurrentTileAtt(FALSE);
			DWORD dwHeroAtt=g_pHero->GetCurrentTileAtt(FALSE);

			//Todo:공격할수 있는 유저 레벨이 20 공격 받는 유저 레벨이 20일때 공격 
			if(BIT_CHECK(dwPlayerAtt,PTA_PK_TILE)&&BIT_CHECK(dwHeroAtt,PTA_PK_TILE))
			{
				if(g_pHero->GetLevel()<20||pPlayer->GetLevel()<20)
				{
					return RELATIONKIND::Friend;
				}
			}


			
			if(BIT_CHECK(dwPlayerAtt,PTA_FREE_CONFLICT_TILE)&&BIT_CHECK(dwHeroAtt,PTA_FREE_CONFLICT_TILE))//자유 분쟁지역일때
			{
				return RELATIONKIND::Enemy;

			}//일반 분쟁 지역일때
			else if(BIT_CHECK(dwPlayerAtt,PTA_CONFLICT_TILE)&&BIT_CHECK(dwHeroAtt,PTA_CONFLICT_TILE))
			{
				uiPartyMan* partyMan =
					static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
				if (partyMan)
				{
					if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
						return  RELATIONKIND::Friend;

				}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                {
                    return RELATIONKIND::Friend;
                }
#else
                uiGuildMan* guildMan =
                    static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                if(guildMan)
                {
                    if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                        return  RELATIONKIND::Friend;
                }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER                

				return RELATIONKIND::Enemy;
			}
			else if(BIT_CHECK(dwPlayerAtt,PTA_PK_TILE)&&BIT_CHECK(dwHeroAtt,PTA_PK_TILE))
			{
				uiPartyMan* partyMan =
					static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
				if (partyMan)
				{
					if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
						return  RELATIONKIND::Friend;
				}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                {
                    return RELATIONKIND::Friend;
                }
#else
                uiGuildMan* guildMan =
                    static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                if(guildMan)
                {
                    if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                        return  RELATIONKIND::Friend;
                }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

				//카오 상태지만 길드원이나 파티원은 친구
				if(pPlayer->GetChaoState()==eCHAO_STATE_CHAO || pPlayer->IsAttackBeginPlayer())
				{
					return RELATIONKIND::Enemy;
				}
			
				//Todo:강제 공격
				if(bForceEnemy)
				{
					return RELATIONKIND::Enemy;
				}
				else
					return RELATIONKIND::Friend;
				
			}
			else
			{
				return RELATIONKIND::Friend;
			}

			return RELATIONKIND::Friend;

		}
		else if( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FIELD)
		{

			if(!g_isPkServer)
				return RELATIONKIND::Friend;
			DWORD dwPlayerAtt=pPlayer->GetCurrentTileAtt(FALSE);
			DWORD dwHeroAtt=g_pHero->GetCurrentTileAtt(FALSE);

			if(BIT_CHECK(dwPlayerAtt,PTA_FREE_CONFLICT_TILE)&&BIT_CHECK(dwHeroAtt,PTA_FREE_CONFLICT_TILE))
			{

				if(bForceEnemy)
					return RELATIONKIND::Enemy;

				uiPartyMan* partyMan =
					static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
				if (partyMan)
				{
					if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
						return  RELATIONKIND::Friend;
				}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                {
                    return RELATIONKIND::Friend;
                }
#else
                uiGuildMan* guildMan =
                    static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                if(guildMan)
                {
                    if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                        return  RELATIONKIND::Friend;
                }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

				return RELATIONKIND::Enemy;
			}
			else if(BIT_CHECK(dwPlayerAtt,PTA_CONFLICT_TILE)&&BIT_CHECK(dwHeroAtt,PTA_CONFLICT_TILE))
			{
				uiPartyMan* partyMan =
					static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
				if (partyMan)
				{
					if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
						return  RELATIONKIND::Friend;

				}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                {
                    return RELATIONKIND::Friend;
                }
#else
                uiGuildMan* guildMan =
                    static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                if(guildMan)
                {
                    if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                        return  RELATIONKIND::Friend;
                }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

				return RELATIONKIND::Enemy;
			}
			else if(BIT_CHECK(dwPlayerAtt,PTA_PK_TILE)&&BIT_CHECK(dwHeroAtt,PTA_PK_TILE))
			{
				if(pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_BOTH_HOSTILITY||
					pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_PROCLAIM_HOSTILITY)
				{
					return  RELATIONKIND::Enemy;
				}
				else if(pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_ONE_SIDE_HOSTILITY)
				{
					if(bForceEnemy)
						return RELATIONKIND::Enemy;
					else
						return RELATIONKIND::Friend;
				}
				else if(pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_ALLIENCE)
				{
					return  RELATIONKIND::Friend;
				}

				// 길드원이나 파티원은 친구
				uiPartyMan* partyMan =
					static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
				if (partyMan)
				{
					if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
						return  RELATIONKIND::Friend;
				}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                {
                    return RELATIONKIND::Friend;
                }
#else
                uiGuildMan* guildMan =
                    static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                if(guildMan)
                {
                    if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                        return  RELATIONKIND::Friend;
                }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

				//레벨이 카오나 ctrl보다 우선순위가 높아야한다.
				if(BIT_CHECK(dwPlayerAtt,PTA_PK_TILE)&&BIT_CHECK(dwHeroAtt,PTA_PK_TILE))
				{
					if(g_pHero->GetLevel()<20||pPlayer->GetLevel()<20)
					{
						return RELATIONKIND::Friend;
					}
				}

#ifdef _CHINA
				if(pPlayer->GetChaoState()==eCHAO_STATE_CHAO)
#else
				if(pPlayer->GetChaoState()==eCHAO_STATE_CHAO || pPlayer->IsAttackBeginPlayer())
#endif//_CHINA:중국에서는 프리카오시 자동타켓딩되지않는다.(0814시요청) 
				
				{
					return RELATIONKIND::Enemy;
				}

				//Todo:강제 공격
				if(bForceEnemy)
				{
					return RELATIONKIND::Enemy;
				}
				else
					return RELATIONKIND::Friend;
			}
			else
			{

				if(pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_BOTH_HOSTILITY||
					pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_PROCLAIM_HOSTILITY)
				{
					return  RELATIONKIND::Enemy;
				}
				else if(pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_ONE_SIDE_HOSTILITY)
				{
					if(bForceEnemy)
						return RELATIONKIND::Enemy;
					else
						return RELATIONKIND::Friend;
				}
				else if(pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_ALLIENCE)
				{
					return  RELATIONKIND::Friend;
				}

				return RELATIONKIND::Friend;
			}
		}
		else if ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING )
		{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
			uiBattleZoneReadyCHuntingConfig* cfg = GET_CAST_DIALOG( uiBattleZoneReadyCHuntingConfig,
				                                   IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG );

			assert (cfg);
			if (cfg->GetCfgs().compModeType == eCOMPETITION_MODE_SURVIVAL ||
                // FIXME: 2006년 5월 6일 - 타임 어택에선 서로 공격 할 수 없다.
				// cfg->GetCfgs().compModeType == eCOMPETITION_MODE_TIME_ATTACK ||
				cfg->GetCfgs().compModeType == eCOMPETITION_MODE_DEATH_MATCH) //pvp모드 일때
			{
				//Todo: 파티가 다르면 적,파티가 같으면 프렌드  
				uiPartyMan* partyMan =
					static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
				
				if(partyMan)
				{
					if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
					{
						return RELATIONKIND::Friend;
					}
					else
						return RELATIONKIND::Enemy;
				}
				else
					return RELATIONKIND::Enemy;
 			}
            // FIXME: 2006년 5월 6일 - 타임 어택에선 서로 공격할 수 없으므로 Friend로 설정
            else
            {
                return RELATIONKIND::Friend;
            }
#else 
            return RELATIONKIND::Friend;
#endif //#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
		}
		else
		{
			return RELATIONKIND::Friend;
		}

	}
	else if (pObject->IsKindOfObject(MONSTER_OBJECT))//Todo:주인과의 관계로 변경
	{
		Monster *pMonster = (Monster *)pObject;

		if(pMonster->GetMonsterType()==MONSTER_TYPE_TOTEM)
		{
			return RELATIONKIND::None;
		}
		else if(pMonster->GetParentId())
		{
			Player * pPlayer = (Player *)g_ObjectManager.GetObject( pMonster->GetParentId() );

			if(pPlayer)//주인이 있다면 주인과의 관계를 본다.
			{
				ePLAYER_BEHAVE_STATE behaveState = pPlayer->GetBeHaveState();

				if (behaveState == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE)
				{
					return RELATIONKIND::Vendor;
				}

				if(pPlayer->GetObjectKey() == g_pHero->GetObjectKey())//이거 먼저 검사해야하지 않나?
				{
					return RELATIONKIND::Friend;
				}				


				if ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_PVP )
				{
					uiPartyMan* partyMan =
						static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

					if(partyMan)
					{
						if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
						{
							return RELATIONKIND::Friend;
						}
						else
							return RELATIONKIND::Enemy;
					}
					else
						return RELATIONKIND::Enemy;
				}
				else if( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_VILLAGE)//&&!pPlayer->GetGMGrade()
				{
					if(!g_isPkServer)
						return RELATIONKIND::Friend;

					DWORD dwPlayerAtt=pPlayer->GetCurrentTileAtt(FALSE);
					DWORD dwHeroAtt=g_pHero->GetCurrentTileAtt(FALSE);

					if(BIT_CHECK(dwPlayerAtt,PTA_PK_TILE)&&BIT_CHECK(dwHeroAtt,PTA_PK_TILE))
					{
						if(g_pHero->GetLevel()<20||pPlayer->GetLevel()<20)
						{
							return RELATIONKIND::Friend;
						}
					}



					if(BIT_CHECK(dwPlayerAtt,PTA_FREE_CONFLICT_TILE)&&BIT_CHECK(dwHeroAtt,PTA_FREE_CONFLICT_TILE))
					{
						if( bForceEnemy )
							return RELATIONKIND::Enemy;

						if( IsFriendShip( pPlayer ) )
							return RELATIONKIND::Friend;
						else
							return RELATIONKIND::Enemy;
					}
					else if(BIT_CHECK(dwPlayerAtt,PTA_CONFLICT_TILE)&&BIT_CHECK(dwHeroAtt,PTA_CONFLICT_TILE))
					{
						uiPartyMan* partyMan =
							static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
						if (partyMan)
						{
							if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
								return  RELATIONKIND::Friend;

						}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                        {
                            return RELATIONKIND::Friend;
                        }
#else
                        uiGuildMan* guildMan =
                            static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                        if(guildMan)
                        {
                            if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                                return  RELATIONKIND::Friend;
                        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

						return RELATIONKIND::Enemy;
					}
					else if(BIT_CHECK(dwPlayerAtt,PTA_PK_TILE)&&BIT_CHECK(dwHeroAtt,PTA_PK_TILE))
					{
						uiPartyMan* partyMan =
							static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
						if (partyMan)
						{
							if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
								return  RELATIONKIND::Friend;
						}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                        {
                            return RELATIONKIND::Friend;
                        }
#else
                        uiGuildMan* guildMan =
                            static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                        if(guildMan)
                        {
                            if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                                return  RELATIONKIND::Friend;
                        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

						if(pPlayer->GetChaoState()==eCHAO_STATE_CHAO || pPlayer->IsAttackBeginPlayer())
						{
							return RELATIONKIND::Enemy;
						}

						//Todo:강제 공격
						if(bForceEnemy)
						{
							return RELATIONKIND::Enemy;
						}
						else
							return RELATIONKIND::Friend;
					}
					else
					{
						return RELATIONKIND::Friend;
					}

				}
				else if( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FIELD)
				{
					if(!g_isPkServer)
						return RELATIONKIND::Friend;

					if(pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_BOTH_HOSTILITY||
						pPlayer->GetGuildRenderInfo().m_RelationType==eGUILD_RELATION_PROCLAIM_HOSTILITY)
					{
						return  RELATIONKIND::Enemy;
					}
					else if( pPlayer->GetGuildRenderInfo().m_RelationType == eGUILD_RELATION_ALLIENCE )
					{
						if( bForceEnemy )
							return RELATIONKIND::Enemy;

						return  RELATIONKIND::Friend;
					}

				
					DWORD dwPlayerAtt=pPlayer->GetCurrentTileAtt(FALSE);
					DWORD dwHeroAtt=g_pHero->GetCurrentTileAtt(FALSE);

					if( BIT_CHECK(dwPlayerAtt,PTA_PK_TILE)&&BIT_CHECK(dwHeroAtt,PTA_PK_TILE) )
					{
						if( g_pHero->GetLevel() < 20 || pPlayer->GetLevel() < 20 )
						{
							return RELATIONKIND::Friend;
						}			
					}

					if(BIT_CHECK(dwPlayerAtt,PTA_FREE_CONFLICT_TILE)&&BIT_CHECK(dwHeroAtt,PTA_FREE_CONFLICT_TILE))
					{
						if( bForceEnemy )
							return RELATIONKIND::Enemy;

						if( IsFriendShip( pPlayer ) )
							return RELATIONKIND::Friend;
						else
							return RELATIONKIND::Enemy;
					}
					else if(BIT_CHECK(dwPlayerAtt,PTA_CONFLICT_TILE)&&BIT_CHECK(dwHeroAtt,PTA_CONFLICT_TILE))
					{
						uiPartyMan* partyMan =
							static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
						if (partyMan)
						{
							if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
								return  RELATIONKIND::Friend;

						}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                        {
                            return RELATIONKIND::Friend;
                        }
#else
                        uiGuildMan* guildMan =
                            static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                        if(guildMan)
                        {
                            if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                                return  RELATIONKIND::Friend;
                        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

						return RELATIONKIND::Enemy;
					}
					else if(BIT_CHECK(dwPlayerAtt,PTA_PK_TILE)&&BIT_CHECK(dwHeroAtt,PTA_PK_TILE))
					{
						uiPartyMan* partyMan =
							static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
						if (partyMan)
						{
							if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
								return  RELATIONKIND::Friend;
						}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                        if (GuildSystem::Function::IsGuildMember(pPlayer->GetGuildName()) == true)
                        {
                            return RELATIONKIND::Friend;
                        }
#else
                        uiGuildMan* guildMan =
                            static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                        if(guildMan)
                        {
                            if(guildMan->IsGuildMember(pPlayer->GetGuildName()))
                                return  RELATIONKIND::Friend;
                        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

						if(pPlayer->GetChaoState()==eCHAO_STATE_CHAO || pPlayer->IsAttackBeginPlayer())
						{
							return RELATIONKIND::Enemy;
						}

						//Todo:강제 공격
						if(bForceEnemy)
						{
							return RELATIONKIND::Enemy;
						}
						else
							return RELATIONKIND::Friend;
					}
					else 
					{
						return RELATIONKIND::Friend;
					}
				}
				else if ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING )
				{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
					uiBattleZoneReadyCHuntingConfig* cfg = GET_CAST_DIALOG( uiBattleZoneReadyCHuntingConfig,
  														   IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG );

					assert (cfg);
					if (cfg->GetCfgs().compModeType == eCOMPETITION_MODE_SURVIVAL ||
						cfg->GetCfgs().compModeType == eCOMPETITION_MODE_TIME_ATTACK ||
						cfg->GetCfgs().compModeType == eCOMPETITION_MODE_DEATH_MATCH)
					{
						//Todo: 파티가 다르면 적,파티가 같으면 프렌드  
						uiPartyMan* partyMan =
							static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

						if(partyMan)
						{
							if(partyMan->IsPartyMember(pPlayer->GetObjectKey()))
							{
								return RELATIONKIND::Friend;
							}
							else
								return RELATIONKIND::Enemy;
						}
						else
							return RELATIONKIND::Enemy;


					}

#else
                    return RELATIONKIND::Enemy;
#endif //#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
				}
				else
				{
					return RELATIONKIND::Friend;
				}

			}
			else
				return RELATIONKIND::Enemy;

		}
		else
			return RELATIONKIND::Enemy;

	}
	else if(pObject->IsKindOfObject( PET_OBJECT))
	{
		return RELATIONKIND::None;
	}
	else if (pObject->IsKindOfObject( NPC_OBJECT) || pObject->IsKindOfObject( LOTTO_NPC_OBJECT))
	{
		return RELATIONKIND::Assistant;
	}
    else if (pObject->IsKindOfObject( MERCHANT_OBJECT ))
    {
        return RELATIONKIND::Vendor;
    }
   
	return RELATIONKIND::None;
}
//#endif

//------------------------------------------------------------------------------
/**
*/
DWORD GameFunc::GetAttackTargetWithinRange(WzVector vPos,float fRange,DWORD *pObjectKeys,DWORD exception)
{
	DWORD ret = 0;
	Object_Map_Itr itr = g_ObjectManager.GetBegin();

	for (; itr != g_ObjectManager.GetEnd(); itr++)
	{
		Object * pObject = static_cast<Object *>(itr->second);
		if (!pObject)
		{
			continue;
		}

		if (pObject->IsKindOfObject(MONSTER_OBJECT ))
		{
			Monster *pMonster = static_cast<Monster *>(itr->second);

			if (pMonster->GetObjectKey() == exception)
			{
				continue;
			}

			WzVector vDistance = vPos - pMonster->GetPosition();			
			float fLength = VectorLength2(&vDistance);
			if (fLength <= fRange * fRange) 
			{
				vDistance.z = 0.0f;
				if (pMonster->CanBeAttackTarget())
				{
					pObjectKeys[ret] = pMonster->GetObjectKey();
					ret++;
					if (ret >= 12) break;
				}
			}
		}
	}

	return ret;
}

//------------------------------------------------------------------------------
/**
*/
DWORD GameFunc::GetAttackTargetWithinLine(WzVector vPos,WzVector vDest,float fRange,DWORD *pObjectKeys,DWORD exception)
{
	DWORD ret = 0;
	Object_Map_Itr itr = g_ObjectManager.GetBegin();

	// 사각형의 축 두개를 만든다.
	vPos.z = 0.0f;
	vDest.z = 0.0f;

	WzVector vDiff = vPos - vDest;
	WzVector vCenter = (vPos + vDest) * 0.5f;

	// 두좌표가 차이가없으면 타겟이 없음을 리턴
	if (vDiff.x == 0 && vDiff.y == 0)
	{
		return 0;
	}

	float fLength = VectorLength(&vDiff);

	VectorNormalize(&vDiff,&vDiff);

	float fAxis1X = vDiff.x;
	float fAxis1Y = vDiff.y;

	float fAxis2X = vDiff.y;
	float fAxis2Y = -vDiff.x;


	for (; itr != g_ObjectManager.GetEnd(); itr++)
	{
		Object * pObject = static_cast<Object *>(itr->second);
		if (!pObject)
		{
			continue;
		}

		if (pObject->IsKindOfObject(MONSTER_OBJECT))
		{
			Monster *pMonster = static_cast<Monster *>(itr->second);

			if (pMonster->GetObjectKey() == exception)
			{
				continue;
			}

			if (pMonster->CanBeAttackTarget())
			{

				WzVector vOffset = pMonster->GetPosition() - vCenter;			

				// 축변환~
				float fNewAxisX = -(vOffset.x * fAxis2Y + vOffset.y * -fAxis1Y);
				float fNewAxisY = -(vOffset.x * -fAxis2X + vOffset.y * fAxis1X);

				float fDet = fAxis1X * fAxis2Y - fAxis1Y * fAxis2X;

				// 변환된 축좌표가 사각형안에 들어가는지 체크
				if (fabs(fNewAxisX) <= fLength * 0.5f + 1.0f)
				{
					if (fabs(fNewAxisY) <= fRange )
					{
						pObjectKeys[ret] = pMonster->GetObjectKey();
						ret++;

						if (ret >= 12) 
							break;
					}
				}
			}
		}
	}

	return ret;
}



//------------------------------------------------------------------------------
/**
*/
BOOL GameFunc::IsMove(WzVector vPos,WzVector vDest,BOOL bCheckMakePath,DWORD dwMaxNode)
{
	assert(g_pPathExplorer);//초기 시작시 생성함으로 널일이유가 없겠지만..
	assert(g_pMap);

	if(!g_pPathExplorer || !g_pMap)
		return FALSE;

	g_pPathExplorer->Stop();

	g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vPos, -1);

	// 대상 Tile 얻기	
	int iTileFocussing = -1;
	iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vDest, NULL, -1.0f, 100.0f );

	if( iTileFocussing < 0 )
	{
		return FALSE;
	}

	// 갈수 없는 타일로 가는 명령은 무시 
	if (g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iTileFocussing) & (PTA_ONLY_JUMP |PTA_NO_WALK))
	{
		return FALSE;
	}

	if (vPos == vDest)
    {
		return TRUE;
    }

	if (bCheckMakePath) //이동경로까지 검사 할경우 
	{
		// 디폴트 이동 제약 타일
	    WORD wAttr =  PTA_ONLY_JUMP | PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE;
				
		ENUM_FINDPATH_RESULT rt = g_pSunTerrain->PathExp_FindPath(g_pPathExplorer, vDest, iTileFocussing, wAttr, dwMaxNode);

		if (rt != EFR_SUCCESS)
		{			
			return FALSE;
		}
	}

	return TRUE;
}


void ClampAngle(float* pAngle)
{
	float fNewAngle = *pAngle;
	fNewAngle = fmod(fNewAngle,WZ_PI * 2.0f);

	if (fNewAngle < 0.0f)
	{
		fNewAngle += WZ_PI * 2.0f;
	}

	*pAngle = fNewAngle;
}

void RenderOutLinedText(TCHAR *pText,RECT *prc,WzColor color, BYTE alpha/* = 255*/)
{
	if(prc)
	{
		RECT rc = *prc;
		BOOL bTest = g_pSunRenderer->IsEnableZTest();
		g_pSunRenderer->SetZBufferTest(FALSE);

		WzColor wzColor = WzColor_RGBA(0,0,0,alpha);

		g_pSunRenderer->x_pManagerTextOut->DrawText(pText,
			&rc,
			color,
			WzColor_RGBA(0,0,0,0),
			TP_HLEFT,
			ETS_OUTLINE,
			wzColor);

		g_pSunRenderer->SetZBufferTest(bTest);
	}

}
//Todo:현재 감마에 적용 
void SetGamma(DWORD dwRamp)
{
	dwRamp=min(255,dwRamp);
	dwRamp=max(0,dwRamp);
	float fPercent = (float)dwRamp / 255;

	WORD wSred[256];
	WORD wSgreen[256];
	WORD wSblue[256];


	for(int i=0;i<256;i++)
	{
		wSred[i] =WORD(g_D3dGammaRamp.red[i] * fPercent);
		wSred[i]=min(65535,wSred[i]);

		wSgreen[i] = WORD(g_D3dGammaRamp.green[i] * fPercent);
		wSgreen[i]=min(65535,wSgreen[i]);

		wSblue[i] = WORD(g_D3dGammaRamp.blue[i] * fPercent);
		wSblue[i]=min(65535,wSblue[i]);

	}

	g_pSunRenderer->SetGammaRamp(wSred,wSgreen,wSblue);
}

DWORD	GetGamma()
{

	return 255;
}



void RenderShadowedText(TCHAR *pText,RECT *prc,WzColor color)
{
	if(prc)
	{
		RECT rc = *prc;
		BOOL bTest = g_pSunRenderer->IsEnableZTest();
		g_pSunRenderer->SetZBufferTest(FALSE);

		g_pSunRenderer->x_pManagerTextOut->DrawText(pText,
			&rc,
			color,
			WzColor_RGBA(0,0,0,0),
			TP_HLEFT,
			ETS_SHADOW
			);

		g_pSunRenderer->SetZBufferTest(bTest);
	}

}

void RenderBlockedText(TCHAR *pText,RECT *prc,WzColor color,WzColor BackColor)
{
	if(prc)
	{
		RECT rc = *prc;
		BOOL bTest = g_pSunRenderer->IsEnableZTest();
		g_pSunRenderer->SetZBufferTest(FALSE);

		SIZE szBox;
		RECT rcBox;

		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(pText,_tcslen(pText),&szBox);

		rcBox.left = rc.left - 1;
		rcBox.right = rc.left + szBox.cx + 1;
		rcBox.top = rc.top;
		rcBox.bottom = rc.top + szBox.cy + 1;	

		g_InterfaceManager.RenderRect( rcBox, BackColor );

		g_pSunRenderer->x_pManagerTextOut->DrawText(pText,rc.left,rc.top,color);
		g_pSunRenderer->SetZBufferTest(bTest);
	}

}



void RenderBlockedTextWithDepthTest(LPCTSTR pText,RECT *prc,WzColor color,WzColor BackColor,float fDepthTest, bool bPlayer /*= false*/)
{
	if(prc)
	{
		RECT rc = *prc;
		BOOL bTest = g_pSunRenderer->IsEnableZTest();
		g_pSunRenderer->SetZBufferTest(TRUE);
	 	
		if(false == bPlayer)
		{
			SIZE szBox;
			RECT rcBox;

			g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint((TCHAR*)pText,_tcslen(pText),&szBox); // 글로벌 엔진버전이 낮아서 error발생 (TCHAR*)필요 추후 제거 요망

			rcBox.left = rc.left - 1;
			rcBox.right = rc.left + szBox.cx + 1;
			rcBox.top = rc.top;

			rcBox.bottom = rc.top + szBox.cy + 1;	

			g_InterfaceManager.RenderRectWithZTest(rcBox,BackColor,fDepthTest);
		}

		g_pSunRenderer->x_pManagerTextOut->BeginSpriteTextWithDepth(fDepthTest);
		g_pSunRenderer->x_pManagerTextOut->DrawText((TCHAR*)pText,rc.left,rc.top,color); // 글로벌 엔진버전이 낮아서 error발생 (TCHAR*)필요 추후 제거 요망
		g_pSunRenderer->x_pManagerTextOut->EndSpriteTextWithDepth();
		g_pSunRenderer->SetZBufferTest(bTest);
	}
}



eWEAPONSOUNDKIND GameFunc::GetWeaponSoundKind(eWEAPONTYPE eType)
{
	switch(eType)
	{
	case eWEAPONTYPE_TWOHANDAXE:
	case eWEAPONTYPE_TWOHANDSWORD:
		return eWEAPONSOUNDKIND_TWOHANDED;
	case eWEAPONTYPE_ONEHANDSWORD:
		return eWEAPONSOUNDKIND_ONEHANDSWORD;
	case eWEAPONTYPE_SPEAR:
		return eWEAPONSOUNDKIND_SPEAR;
	case eWEAPONTYPE_ONEHANDCROSSBOW:
		return eWEAPONSOUNDKIND_ARROW;
	case eWEAPONTYPE_ETHERWEAPON:
		return eWEAPONSOUNDKIND_ETHERBULLET;
	case eWEAPONTYPE_STAFF:
		return eWEAPONSOUNDKIND_STAFF;
	case eWEAPONTYPE_DAGGER:
		return eWEAPONSOUNDKIND_DAGGER;
	case eWEAPONTYPE_WHIP:
		return eWEAPONSOUNDKIND_WHIP;
	case eWEAPONTYPE_ORB:
		return eWEAPONSOUNDKIND_MAGICBULLET;
        //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eWEAPONTYPE_GUNTLET:
        return eWEAPONSOUNDKIND_GUNTLET;
    case eWEAPONTYPE_POLEARM:
        return eWEAPONSOUNDKIND_POLEARM;
        //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eWEAPONTYPE_HELLROID_1:
        return eWEAPONSOUNDKIND_HELLROIDWEAPON_1;
        //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eWEAPONTYPE_ARCBLADE:
        return eWEAPONSOUNDKIND_ARCBLADE;
	default:
	case eWEAPONTYPE_PUNCH:
#ifdef _YJ_FISHING_SYSTEM_000933
	case eWEAPONTYPE_FISHING_POLE:
#endif //_YJ_FISHING_SYSTEM_000933
		{
			return eWEAPONSOUNDKIND_UNDEFINED;
		}
		break;
	}
}

BOOL GameFunc::GetBonePos(CWzUnitDraw *pSkeleton, int index, WzVector *pVector)
{
	if (pSkeleton && pVector && pSkeleton->GetWzUnit()) 
    {
		WzMatrix * pBoneMatrix = pSkeleton->GetTransformBuffer();

		if (pBoneMatrix) 
        {
			HANDLE handle = pSkeleton->GetWzUnit()->GetWzdHandle();

			if (handle != INVALID_HANDLE_VALUE) 
            {
				CWzdBase * pBase = g_pSunRenderer->x_pManagerWzdLoader->GetWzd(handle);

				if ( pBase ) 
                {
					if (index >= 0 && index < pBase->GetNumNode())
                    {
                        pVector->x = pBoneMatrix[index]._41;
						pVector->y = pBoneMatrix[index]._42;
						pVector->z = pBoneMatrix[index]._43;

						return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

BOOL GameFunc::GetBoneMatrix( CWzUnitDraw* pSkeleton, int index, WzMatrix* pMatrix )
{
	if( pSkeleton && pMatrix )
	{
		MatrixIdentity( pMatrix );

		if( WzMatrix* pBoneMatrix = pSkeleton->GetTransformBuffer() )
		{
			if( index >= 0 && index < pSkeleton->GetTransformBufferSize() )
			{
				*pMatrix = pBoneMatrix[index];
				return TRUE;
			}
		}
	}
	return FALSE;
}


BOOL GameFunc::UpdateCursorByPickedObject(Object * pObject)
{
	if (!g_pHero)
		return FALSE;

	if (MouseHandler::Instance()->IsExistItemAtHand())
	{
		return FALSE;
	}

	if (!pObject)
	{
		return FALSE;
	}
	
	if (pObject->IsKindOfObject(MAP_OBJECT))
	{
		MapObject * pMapObject = (MapObject *)pObject;

		if(FALSE == pMapObject->GetShow())
			return FALSE;

		if (pMapObject->CheckTriggerActive())
		{
			if (pMapObject->IsTriggerOperating())
			{
				CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Operating_switch_Highlight );
			}
			else
			{
				CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Operating_switch );
			}

			return TRUE;
		}
		else if(pMapObject->IsWayPointObj() 
			|| pMapObject->IsDominationObject()
            || pMapObject->IsAirShipObject())
		{
			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Conversation );
			return TRUE;

		}
		else if(INVALID_WZID != pMapObject->GetCollectionID())
		{
			CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_PickAxe );
			return TRUE;
		}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        else if (pMapObject->IsBattleGroundResurrectionObject())
        {
            if (ChaosSystem::GetBattleGroundData().player_state() == kPlayerStateStandbyResurectionRefuse)
            {
                CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Operating_switch );
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	}
	else if (pObject->IsKindOfObject(ITEM_OBJECT))
	{
		CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Open_Box );
		return TRUE;
	}
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	else if (pObject->IsKindOfObject(STATUE_PLAYER_OBJECT))
	{
		CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Conversation );
		return TRUE;
	}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	else
	{
		Character * pPickedCharacter = static_cast<Character *>(pObject);
		assert(pPickedCharacter);
		switch( GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pPickedCharacter, g_HeroInput.lsForceAttack() ) )
		{
		case RELATIONKIND::Enemy:
			{
				if (g_HeroInput.IsAttackSkillArea())
				{
					CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Magic_Scope_Highlight );
				}
				else
				{
					if (g_pHero->GetTargetObject() && 
						g_pHero->GetTargetObject() == pObject)
					{
						CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Attack_Target_Highlight );
					}
					else
					{
						CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Attack_Target );
					}
				}

				return TRUE;
			}
			break;

		case RELATIONKIND::Friend:
			{
				return FALSE;
			}
			break;

		case RELATIONKIND::Vendor:
		case RELATIONKIND::Assistant:
			{
				NPC*  pNPC = (NPC*)pObject;
				switch(pNPC->GetExtraInfo()->m_eNPCTYPE)
				{
				case eNPC_TYPE_EVENT_DONATION:
					{
						if (g_pHero->GetTargetObject() && 
							g_pHero->GetTargetObject() == pObject)
						{
							CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Donation_Highlight);
						}
						else
						{
							CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Donation);
						}
					}
					break;

				case eNPC_TYPE_EVENT_BUFF:
					{
						if (g_pHero->GetTargetObject() && 
							g_pHero->GetTargetObject() == pObject)
						{
							CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Operating_switch_Highlight);
						}
						else
						{
							CursorChangeTransact::Instance()->ChangeTranaction(MouseCursor::eMouseCursor_Operating_switch);
						}
					}
					break;
				
				default:
					{
						CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Conversation );
					}
					break;
				}

				return TRUE;
			}
			break;

		case RELATIONKIND::None:
		default:
			{
				return FALSE;
			}
			break;
		}
	}

	return TRUE;
}

SolarDialog* GameFunc::NPCMeetWithForward(MAPID MapID, FIELDID FieldID, CODETYPE NpcCode, DWORD dwNPCKey /*= 0*/)
{
	uiQuestMan * pQuestMan = 
		(uiQuestMan * )g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUEST);

	if ( pQuestMan )
	{
		return pQuestMan->OpenForwardWindow(MapID, FieldID, NpcCode, dwNPCKey);   
	}

	return NULL;
}


SolarDialog *		
GameFunc::OpenNPCMeetDialog(MAPID MapID, FIELDID FieldID, CODETYPE NpcCode)
{
	EXTRA_NPCINFO_SET * pInfoSet =  
		NPCInfoParser::Instance()->GetExtraInfoSetFromFieldID( (MAPCODE)MapID , (FIELDID)FieldID);

	vector<EXTRA_NPCINFO*>::iterator itr;

	EXTRA_NPCINFO * pInfo = NULL;
	EXTRA_NPCINFO * pFindInfo = NULL;

	for ( itr = pInfoSet->GetBegin(); itr != pInfoSet->GetEnd(); itr ++)
	{
		pInfo = *itr;
		if ( pInfo->m_NPCCODE == NpcCode )
		{
			pFindInfo = pInfo;
			break;
		}
	}

	
	if (pFindInfo)
	{
		switch (pFindInfo->m_eNPCTYPE)
		{
		case eNPC_TYPE_GUARD:
			{
				uiQuestCommonForward* pQuestCommonForward = GET_CAST_DIALOG( uiQuestCommonForward, IM_QUEST_MANAGER::QUEST_COMMON_FORWARD );

				if(pQuestCommonForward)
				{
					pQuestCommonForward->ShowInterfaceWithSound(TRUE);

					keyMsg msg;
					ZeroMemory(&msg,sizeof(msg));
					msg.dwType = eDoType_dialog_Quest_NPC;
					msg.wParam = QUEST_WINDOW_NPC;
					msg.DoSomething = GlobalFunc::DoShowQuestWindow;
					g_KeyQueueManager.PushBack(msg);

					return (SolarDialog*)pQuestCommonForward;
				}
			}
			break;
		case eNPC_TYPE_MAKE_ZONE:
			{
				// 미션 보상을 받지 않은 상태이면.
				if (BattleScene::m_bMissionReward)
				{
					TCHAR szText[512];

					const int CANNOT_ENTER_BATTLEZONE_CODE = 5368;

					g_InterfaceManager.GetInterfaceString(CANNOT_ENTER_BATTLEZONE_CODE,szText,512);

					GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_TOP_SHOW,szText);
					return NULL;

				}

                g_InterfaceManager.HideAllWindows();
				g_HeroInput.SetCurrentTarget(0);

				// 배틀존 리스트 UI 띄우기
				uiBase* pUIBase = GET_DIALOG( IM_BATTLEZONE_MANAGER::BATTLEZONE_LIST );

                if (pUIBase)
                {
					if(g_pMap->GetOriginWeather())
					{
						g_pMap->SetRenderWeather(FALSE);
					}

                    BOOL bShow = !pUIBase->IsVisible();
                    pUIBase->ShowInterfaceWithSound(bShow);
                }

				return (SolarDialog *)pUIBase;
			}
			break;

		case eNPC_TYPE_BANK://1
			{
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
                uiBankDialog *pBankDlg = GET_CAST_DIALOG(uiBankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#else
                BankDialog *pBankDlg = GET_CAST_DIALOG(BankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
				
				if (pBankDlg && !pBankDlg->IsVisibleWindow())
				{
					pBankDlg->SendWareHouseStart();
					return (SolarDialog*)pBankDlg;
				}

				return pBankDlg;
			}
			break;

		case eNPC_TYPE_STORE:
			{
				ShopDialog * pShopDlg = (ShopDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SHOP);
				if ( pShopDlg && !pShopDlg->IsVisibleWindow() )
				{
					pShopDlg->SetShopID( NpcCode );
					g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_SHOP, TRUE);	
					return (SolarDialog *)pShopDlg;
				}
			}
			break;

        case eNPC_TYPE_GUILD:
            {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
                uiGuildMenu* guild_menu = GET_CAST_DIALOG(uiGuildMenu, uiGuildMenu::kDialog_ID);
                if (guild_menu != NULL)
                {
                    guild_menu->ShowInterfaceWithSound(TRUE);
					return (SolarDialog *)guild_menu;
                }
#else
                // 길드 메뉴
                uiGuildMan* guildMan =
                    static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

                uiBase * guildMenu = GET_DIALOG( IM_GUILD_MANAGER::GUILD_MENU );

                if (guildMan)
                {
                    if (guildMenu)
                    {
                        guildMenu->ShowInterfaceWithSound(TRUE);
                        return (SolarDialog *)guildMenu;
                    }
                }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
            }
            break;

		case eNPC_TYPE_EVENT:
			{
				// 이벤트 메뉴
				uiEventMan* eventMan = static_cast<uiEventMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_EVENT));
				
				uiEventWindow * eventDlg = GET_CAST_DIALOG( uiEventWindow, IM_EVENT_MANAGER::EVENT_WINDOW );

				if(eventMan && eventDlg)
				{
					eventMan->SendEventTimePeriod();
					return (SolarDialog *)eventDlg;
				}
			}
			break;

#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
		case eNPC_TYPE_GUILD_EVENT:
			{		
				uiEventMan* eventMan = static_cast<uiEventMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_EVENT));
				
				uiGuildEventDialog * pGuildEvent = GET_CAST_DIALOG( uiGuildEventDialog, IM_EVENT_MANAGER::GUILD_EVENT );

				if( eventMan && pGuildEvent )
				{
					pGuildEvent->ShowInterface(TRUE);
					return (SolarDialog *)pGuildEvent;
				}
			}
			break;
#endif

#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
		case eNPC_TYPE_MISSION_RANK:
			{
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
                uiMissionRankMan2* rank_manager_ptr2 =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );
                uiBase* rank_list_ptr2 = GET_DIALOG( IM_MISSION_RANK_MANAGER::MissionRankList );

                if ( rank_manager_ptr2 && rank_list_ptr2 )
                {
                    rank_manager_ptr2->ShowRankList( TRUE );

                    return static_cast<SolarDialog*>(rank_list_ptr2);
                }
#else
				uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

				uiBase * pRankMenu = GET_DIALOG( IM_MISSION_RANK_MANAGER::RankMenu );

				if( pRankMan && pRankMenu )
				{
					pRankMan->ShowRankMenu( TRUE );

                    return (SolarDialog *)pRankMenu;					
				}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
			}
			break;
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM


		case eNPC_TYPE_POINT_RANK:
			{
				// 포인트랭킹 NPC	
#if !defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
				uiMissionRankMan* pRankMan =  GET_DIALOG_MANAGER( uiMissionRankMan, UIMAN_MISSION_RANK );

				uiMissionRankList* pRankList =  GET_CAST_DIALOG( uiMissionRankList, IM_MISSION_RANK_MANAGER::MissionRankList );

				if(pRankMan && pRankList)
				{
					pRankMan->ShowRankList(eRANKING_TYPE_POINT);
					return (SolarDialog *)pRankList;
				}
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM				
			}
			break;


		case eNPC_TYPE_VENDOR_LIST:
			{
// #ifdef _NA_005988_20120901_VENDOR_SEARCH_SYSTEM
// 				uiVendorSearch * pVendorSearch = GET_CAST_DIALOG( uiVendorSearch, IM_VENDOR_MANAGER::VENDOR_SEARCH );
// 				if( pVendorSearch && !pVendorSearch->IsVisibleWindow())
// 				{
// 					g_InterfaceManager.ShowDialog(IM_VENDOR_MANAGER::VENDOR_SEARCH, TRUE);
// 
// 					return (SolarDialog *) pVendorSearch;
// 				}
// #else
				uiVendorList * pVendorList = GET_CAST_DIALOG( uiVendorList, IM_VENDOR_MANAGER::VENDOR_LIST );
				if( pVendorList )
				{
					pVendorList->NET_SEND_CG_VENDOR_LIST_START_SYN();
					return (SolarDialog *) pVendorList;
				}
// #endif
			}
			break;

		case eNPC_TYPE_ITEM_MIX:
			{
#if ITEM_COMPOSITE_APPLY
				BOOL	bTotalComposite = FALSE;
				BASE_CompositeNPCInfo * pInfo = 
					CompositeNPCInfoParser::Instance()->GetCompositeNPCInfo(NpcCode);
				for (int i = 0; i < MAX_COMPOSE_PARAM; ++i)
				{
					if(pInfo->m_dwParams[i] == eITEMCOPOSITE_TOTAL)
					{
						bTotalComposite = TRUE;
					}
				}
				if(bTotalComposite)
				{
					uiItemCompositeMan * pCompositeMan = 
						(uiItemCompositeMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_ITEM_COMPOSITE);

					
					uiItemTotalcomposite * pTotalComposite = GET_CAST_DIALOG( uiItemTotalcomposite, IM_ITEM_COMPOSITE_MANAGER::ITEM_TOTAL_COMPOSITE );
					if( pCompositeMan )
					{
						if(!pTotalComposite->IsVisible())
						{
							pCompositeMan->ShowItemComposite( ITEM_COMPOSITE_CATEGORY_TOTAL_COMPOSITE,
								(eITEMCOMPOSITECODE) pTotalComposite->GetCurrentSelectItemCode());

							return (SolarDialog *)pTotalComposite;
						}
					}
				}
				else
				{
					SelectCompositeMenu * pDlg = 
						(SelectCompositeMenu *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SELECT_COMPOSITE_MENU);

					if (pDlg && !pDlg->IsVisibleWindow())
					{
						pDlg->SetOpenNPC(NpcCode);
						g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_SELECT_COMPOSITE_MENU, TRUE);									

						return (SolarDialog *)pDlg;
					}
				}
#endif
			}
			break;

	case eNPC_TYPE_VILLAGE_PORTAL:
		{
			SolarDialog * pDlg = 
				g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_NPC_PORTAL);

			if (pDlg && !pDlg->IsVisibleWindow())
			{
				NPCPortalDialog * pNPCPortalDlg = (NPCPortalDialog *)pDlg;

				pNPCPortalDlg->SetCurVillageCode(
					NpcCode, 
					(MAPCODE)g_pMap->GetMapID()
					);
				g_InterfaceManager.ShowDialog(InterfaceManager::DIALOG_NPC_PORTAL, TRUE);

				return (SolarDialog *)pDlg;
			}
		}
		break;

	case eNPC_TYPE_QUEST:
		{
			return NULL;			
		}
		break;

	case eNPC_TYPE_EVENT_DONATION: //기부관련
		{
			ui_XMas_DonationConfirm * pDonationConfirm = GET_CAST_DIALOG( ui_XMas_DonationConfirm, IM_EVENT_MANAGER::EVENT_X_MAS_DONATION_CONFIRM );

			if(pDonationConfirm)
			{
				pDonationConfirm->ShowInterface(TRUE);
				return (SolarDialog*)pDonationConfirm;
			}
		}
		break;

	case eNPC_TYPE_EVENT_PERIOD_ITEM: //기간제 아이템 관련
		{
			DWORD dwRewardCode = 0;
			DWORD dwItemCode[2] = {0,0};
			EVENT_INFO_VEC* pEventInfoVec = SunEventInfoParser::Instance()->FindEventInfoFromNpcType(eNPC_TYPE_EVENT_PERIOD_ITEM);
			//해당이벤트가 현재 맵상에 무조건 "1개" 라고 가정하자.
			if(NULL != pEventInfoVec)
			{
				EVENT_INFO* pEventInfo = pEventInfoVec->back();

				if(pEventInfo)
				{
					dwRewardCode = pEventInfo->m_dwRewardInfo[0];
					RewardInfo* pRewardInfo = RewardInfoList::Instance()->FindRewardInfo(dwRewardCode);

					if(pRewardInfo)
					{
						dwItemCode[0] = pRewardInfo->GetTypeCode(0);
						dwItemCode[1] = pRewardInfo->GetTypeCode(1);
					}
				}
			}

			SCSlotContainer *pContainer = g_pHero->GetSlotContainer(SI_INVENTORY);
			POSTYPE start = 0;
			POSTYPE total = 0;
            if (pContainer)
            {
                total = pContainer->GetMaxSlotNum();
            }

			BOOL	bVal = FALSE;

			for (POSTYPE i = start; i < total; ++i)
			{
				if ( !pContainer->IsEmpty(i) )
				{
					SCSlot& rSlot = pContainer->GetSlot(i);
					BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo( rSlot.GetCode() );
					if(pItemInfo)
					{
						if(pItemInfo->m_Code == dwItemCode[0] || pItemInfo->m_Code == dwItemCode[1])
						{
							bVal = TRUE;
							break;
						}
						else
						{
							bVal = FALSE;
						}
					}		
				}
			}

			if(bVal)
			{
				uiSystemMan* pSysMan = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
				if(pSysMan)
				{
					TCHAR szMessage[INTERFACE_STRING_LENGTH + 1] = {0,};
					//203101	뱃지는 쓸만한가? 큰맘 먹고 준 선물이니 아껴쓰라구
					g_InterfaceManager.GetInterfaceString(203101, szMessage, INTERFACE_STRING_LENGTH);
					pSysMan->Sys_Verify(szMessage);
				}

				//등록은 하되 보여주진 않는다.
				ui_XMas_Confirm* pXMasConfirm = GET_CAST_DIALOG( ui_XMas_Confirm, IM_EVENT_MANAGER::EVENT_X_MAS_CONFIRM );
				

				if(pXMasConfirm)
				{
					pXMasConfirm->SetDlgType(ui_XMas_Confirm::eXMAS_DLG_TYPE_PERIOD_ITEM);
					pXMasConfirm->ShowInterface(FALSE);
					return (SolarDialog*)pXMasConfirm;
				}
			}
			else
			{
				ui_XMas_Confirm * pXMasConfirm = GET_CAST_DIALOG( ui_XMas_Confirm, IM_EVENT_MANAGER::EVENT_X_MAS_CONFIRM );

				if(pXMasConfirm)
				{
					pXMasConfirm->SetDlgType(ui_XMas_Confirm::eXMAS_DLG_TYPE_PERIOD_ITEM);
					pXMasConfirm->ShowInterface(TRUE);
					return (SolarDialog*)pXMasConfirm;
				}
			}
		}
		break;

	case eNPC_TYPE_EVENT_SANTA://산타관련
		{
			ui_XMas_SantaDlg* pSantaDlg = GET_CAST_DIALOG( ui_XMas_SantaDlg, IM_EVENT_MANAGER::EVENT_X_MAS_SANTA_DLG );
			if(pSantaDlg)
			{
				pSantaDlg->ShowInterface(TRUE);
				return (SolarDialog*)pSantaDlg;
			}
		}
		break;

	case eNPC_TYPE_EVENT_BUFF:
		{
            Object_Map_Itr found_itr = g_ObjectManager.GetBegin();
            Object_Map_Itr end_itr = g_ObjectManager.GetEnd();
            for ( ; found_itr != end_itr; ++found_itr)
            {

                Object* pObject = found_itr->second;
                if (pObject == NULL)
                {
                    continue;
                }

                eOBJECT_TYPE eObjectType = pObject->GetSCObject().GetObjectType();
                if (eObjectType != NPC_OBJECT)
                {
                    continue;
                }

                NPC* pNPC = static_cast<NPC*>(pObject);
				if (pNPC)
				{
					if (pFindInfo->m_NPCCODE == pNPC->GetExtraInfo()->m_NPCCODE)
					{
						uiBuffEvent* pDlg = GET_CAST_DIALOG(uiBuffEvent, IM_EVENT_MANAGER::EVENT_BUFF);
						if (pDlg)
						{
							pDlg->ShowInterface(TRUE);
						}

						return (SolarDialog*)pDlg;
					}
				}
			}

			return NULL;
		}
		break;

#ifdef _YJ_FISHING_SYSTEM_000933
	case eNPC_TYPE_FISHING:
		{
            uiFishingShop* pFishingShop = (uiFishingShop*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_FISHING_SHOP);
			if(pFishingShop)
			{
				pFishingShop->ShowInterface(TRUE);
				return (SolarDialog*)pFishingShop;
			}
		}
		break;
#endif //_YJ_FISHING_SYSTEM_000933

    case eNPC_TYPE_AIRSHIP_MOVE:
        {
            int continent_number = 1;
            if(g_pHero && g_pMap)
            {
                const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo((MAPCODE)g_pMap->GetMapID());
                if(map_info)
                {
                    continent_number = map_info->ContienetNumber;
                }
            }
            ContinentEp2MapDlg* continent_map_ep2_dialog = 
                GET_CAST_DIALOG(ContinentEp2MapDlg, IM_RADAR_MANAGER::CONTINENT_MAP_EP2_DLG);

            ContinentMapDlg* continent_map = GET_CAST_DIALOG(ContinentMapDlg, 
                IM_RADAR_MANAGER::CONTINENT_MAP_DLG);

            if (continent_number == 1 && continent_map != NULL)
            {
                continent_map->ShowInterface(TRUE);
                continent_map->SetAirShipMode();
            }
            else if (continent_number == 2 && continent_map_ep2_dialog != NULL)
            {
                continent_map_ep2_dialog->ShowInterface(TRUE);
                continent_map_ep2_dialog->SetAirShipMode();
            }
        }
        break;
	case eNPC_TYPE_LOTTO:
		{
#ifdef _DH_UNUSE_LOTTO
#else
			uiLottoWindow* pLottoWindow= GET_CAST_DIALOG( uiLottoWindow, IM_LOTTO_MANAGER::LOTTO_WINDOW_DLG );

			if(pLottoWindow)
			{
				if(pLottoWindow->IsVisible())
				{
					break;
				}
			}
			//나의 로또응모정보를 요청한다
			uiLottoMan* pLottoMan = GET_DIALOG_MANAGER(uiLottoMan, UIMAN_LOTTO_MAN);
 
			if(pLottoMan && pLottoMan->GetWaitPacket()==FALSE)
			{
				pLottoMan->SEND_LOTTO_INFO_SYN();
			}
#endif//_DH_UNUSE_LOTTO
		}
		break;

	case eNPC_TYPE_EVENT_FRIENDINVITATION:
		{
			uiEventMan* eventMan = static_cast<uiEventMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_EVENT));
			uiEventFriend * eventDlg = GET_CAST_DIALOG( uiEventFriend, IM_EVENT_MANAGER::EVENT_FRIEND );

			if(eventMan && eventDlg && !eventDlg->IsVisible())
			{
				eventMan->SendEventFriend();
				return (SolarDialog *)eventDlg;
			}
		}
		break;
#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
    case eNPC_TYPE_SMARTNPC:
        {
            // 지능형 엔피씨일 경우 패킷 전송
            MSG_CG_NPCDIALOG_OPERATION_CMD send_packet(g_pHero->GetObjectKey(), NpcCode, 0, 0);
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
        }
        break;
#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
		default:
			break;
		}
	}

	return NULL;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL GameFunc::CheckHeroInAreaForQuest(DWORD areaID)
{
    if (g_pMap == NULL || g_pHero == NULL)
    {
        return FALSE;
    }

    eTRIGGER_STATE	TrigerState, HeroTrigerState;

    WzSpecialArea* pAreaInfo = g_pMap->GetMapArea(areaID);
    if (pAreaInfo == NULL)
    {
        return FALSE;
    }

    TrigerState = TRIGGER_STATE_LEAVE_AREA;

    switch (pAreaInfo->m_wbvRegion.m_eBvType)
    {
    case BVT_AABB:
        {
            if (TRUE == Contain_AABB_Vertex(&pAreaInfo->m_wbvRegion.m_aabb, g_pHero->GetPosition()))			
            {
                TrigerState = TRIGGER_STATE_ENTER_AREA;			
            }
        }
        break;
    case BVT_PATH_TILE_INDEX:
        {
            int iTile = g_pHero->GetPathExplorer()->GetTile();
            for (int i = 0; i < static_cast<int>(pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount); ++i)
            {
                if (iTile == pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData[i])
                {
                    TrigerState = TRIGGER_STATE_ENTER_AREA;
                    break;
                }
            }
        }
        break;
    case BVT_SPHERE:
        {
            if (TRUE == Contain_Sphere_Vertex(&pAreaInfo->m_wbvRegion.m_BvSphere, g_pHero->GetPosition()))			
            {
                TrigerState = TRIGGER_STATE_ENTER_AREA;			
            }
        }
        break;
    default:
        {
            return FALSE;
        }
    }

    HeroTrigerState = g_pHero->GetTriggerState(areaID);
    if (HeroTrigerState != TrigerState)
    {
        // 들어갔을땐 언제나 패킷을 보낸다!!!
        if (TrigerState == TRIGGER_STATE_ENTER_AREA)
        {
            //< 플레이어가 들어왔다.
            //	Quest Event
            QUEST_AREA_MSG QuestAreaMsg;
            QuestAreaMsg.m_AreaID = areaID;
            g_pQMGR_Real->Event(QUEST_EVENT_AREA, &QuestAreaMsg);
        }	        
        g_pHero->SetTriggerState( areaID,TrigerState );
    }	

    if (TrigerState == TRIGGER_STATE_ENTER_AREA)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL					
GameFunc::IsHeroInAreaForQuest(DWORD areaID)
{
	if ( !g_pMap || !g_pHero )
	{
		return FALSE;
	}

	WzSpecialArea *pAreaInfo = g_pMap->GetMapArea( areaID );
	if ( !pAreaInfo )
	{
		return FALSE;
	}

	BOOL bRet = FALSE;

	if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_AABB )
	{

		if( TRUE == Contain_AABB_Vertex( &pAreaInfo->m_wbvRegion.m_aabb, g_pHero->GetPosition() ) )			
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}	
	else 
	if( pAreaInfo->m_wbvRegion.m_eBvType == BVT_PATH_TILE_INDEX )
	{
		int		iTile = g_pHero->GetPathExplorer()->GetTile();

		for( int i = 0 ; i < int(pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_nCount) ; ++i )
		{
			if( iTile == pAreaInfo->m_wbvRegion.m_BvPathTileIndex.m_pdwData[i] )
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	return FALSE;
}

void GameFunc::GetPlayerSkill(int charType,Character *pChar,Character * pTarget,PLAYER_ACTION &action)
{
	action.ActionID = ACTION_SKILL;				
	action.SKILL.dwTargetID = pTarget->GetObjectKey();
	action.SKILL.dwAttackSerial = 0;
	action.SKILL.vCurPos.wvPos =pChar->GetPosition();					
	action.SKILL.vCurPos.wTile = -1;
	action.SKILL.vDestPos.wvPos =pChar->GetPosition();


	if (charType == eCHAR_BERSERKER) 
	{
		action.SKILL.vDestPos.wvPos =pTarget->GetPosition();//태클 

		action.SKILL.dwSkillID= 2061;
	}
	else if (charType == eCHAR_DRAGON) 
	{
		action.SKILL.dwSkillID= 4061;

		action.SKILL.vDestPos.wvPos =pTarget->GetPosition();//일루젼댄스
	}
	else if (charType == eCHAR_VALKYRIE) 
	{
		action.SKILL.dwSkillID= 7130;

	}
	else if (charType == eCHAR_ELEMENTALIST) 
	{
		action.SKILL.dwSkillID= 10262;
		action.SKILL.vTargePos=pTarget->GetPosition();//화이어스피어

	}
}


Player * GameFunc::SpawnPlayer(BYTE bCharType)
{
	if(g_pHero)
	{
		
		static int iCount=0;
		iCount++;
		char temp[256];
		sprintf(temp,"%d플레이어",iCount);
		g_InterfaceManager.ShowMessageBox(temp,5000,NULL,0);
		Player *pPlayer = (Player *)g_ObjectManager.Add(
			g_ObjectManager.GenerateKeyAtSingleMode(PLAYER_OBJECT), 
			PLAYER_OBJECT,
			bCharType,						
			FALSE);

		PLAYER_RENDER_INFO_EX info;
		ZeroMemory(&info,sizeof(PLAYER_RENDER_INFO_EX));
		info.char_class=bCharType;
		pPlayer->SetPlayerInfo(&info);
		pPlayer->Rotate(0.0f);		
		pPlayer->SetHP(100);
		pPlayer->SetMP(100);
		pPlayer->SetPartVariation(PLAYER_VARIATION_HAIR,rand()%4+1);
		pPlayer->SetPartVariation(PLAYER_VARIATION_FACE,rand()%4+1);
        pPlayer->RefreshAppearRender();
		WzVector wvPos = g_pHero->GetVisiblePos();
		

		if(rand()%2)
			pPlayer->SetLevel(19);
		else
			pPlayer->SetLevel(20);

		pPlayer->SetName(temp);
		pPlayer->SetPosition( wvPos );


		if(pPlayer)
		{
            EquipmentContainer* pContainer = static_cast<EquipmentContainer *>(pPlayer->GetSlotContainer(SI_EQUIPMENT));
			SCItemSlot ItemSlot;
			ALL_ARMOR_CODE temp;

			if(pContainer)
			{
				g_OptimizationManager.GetAllPartInfo(pPlayer->GetClass(),temp);

				ItemSlot.SetCode((WORD)temp.dwArmorCode); // 		
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

				ItemSlot.SetCode((WORD)temp.dwProtectorCode); // 		
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

				ItemSlot.SetCode((WORD)temp.dwHelmetCode); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	


				ItemSlot.SetCode((WORD)temp.dwPantsCode); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

				ItemSlot.SetCode((WORD)temp.dwBootsCode); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

				ItemSlot.SetCode((WORD)temp.dwGloveCode); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

				ItemSlot.SetCode((WORD)temp.dwBeltCode); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

				ItemSlot.SetCode((WORD)temp.dwShirtsCode); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);
			}

            pPlayer->RefreshAppearRender();
			pPlayer->SetNextState(STATE::IDLE,g_CurTime);	
			pPlayer->CommitChangeState(g_CurTime);	
            pPlayer->SetMoveSpeedRatio(100);
			
			return pPlayer;
		}


	}

	return NULL;
}

void GameFunc::SpawnFullsetPlayer(BYTE bCharType , BYTE bySetType)
{
	if(g_pHero)
	{

		static int iCount=0;
		iCount++;
		char temp[256];
		sprintf(temp,"%d마리",iCount);
		g_InterfaceManager.ShowMessageBox(temp,5000,NULL,0);
		Player *pPlayer = (Player *)g_ObjectManager.Add(
			g_ObjectManager.GenerateKeyAtSingleMode(PLAYER_OBJECT), 
			PLAYER_OBJECT,
			eCHAR_BERSERKER,						
			FALSE);

		PLAYER_RENDER_INFO_EX info;
		ZeroMemory(&info,sizeof(PLAYER_RENDER_INFO_EX));
		info.char_class=eCHAR_BERSERKER;

		pPlayer->SetPlayerInfo(&info);
		pPlayer->Rotate(0.0f);		
		pPlayer->SetHP(100);
		pPlayer->SetMP(100);
		pPlayer->SetPartVariation(PLAYER_VARIATION_HAIR,rand()%4+1);
		pPlayer->SetPartVariation(PLAYER_VARIATION_FACE,rand()%4+1);
		WzVector wvPos = g_pHero->GetVisiblePos();


		pPlayer->SetPosition( wvPos );


		if(pPlayer)
		{
            EquipmentContainer* pContainer = static_cast<EquipmentContainer *>(pPlayer->GetSlotContainer(SI_EQUIPMENT));
			SCItemSlot ItemSlot;

			if(pContainer)
			{
				ItemSlot.SetCode(bySetType);
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_WEAPON1, ItemSlot);

				ItemSlot.SetCode(1000 + bySetType); // 		
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_ARMOR, ItemSlot);	

				ItemSlot.SetCode(1050 + bySetType); // 		
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PROTECTOR, ItemSlot);	

				ItemSlot.SetCode(1100 + bySetType); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_HELMET, ItemSlot);	


				ItemSlot.SetCode(1150 + bySetType); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_PANTS, ItemSlot);	

				ItemSlot.SetCode(1200 + bySetType); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BOOTS, ItemSlot);	

				ItemSlot.SetCode(1250 + bySetType); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_GLOVE, ItemSlot);	

				ItemSlot.SetCode(1300 + bySetType); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_BELT, ItemSlot);	

				ItemSlot.SetCode(1350 + bySetType); // 
				pContainer->JustInsertSlot(EquipmentContainer::EQUIP_SHIRTS, ItemSlot);
			}

            pPlayer->RefreshAppearRender();
			pPlayer->SetNextState(STATE::IDLE,g_CurTime);	
			pPlayer->CommitChangeState(g_CurTime);	

			pPlayer->SetMoveSpeedRatio(100);
			pPlayer->SetAttackSpeedRatio(100);
		}


	}
}

void GameFunc::SetPlayerMonsterAttackStyle(PlayerMonster* player_monster)
{
    DWORD weapon_style = player_monster->GetWeaponKind();
    BYTE class_code = player_monster->GetClass();
    
    DWORD style_code;
    player_monster->SetCurrentNomalAttackStyle(weapon_style);
    style_code = player_monster->GetCurrentAttackStyle();
    player_monster->SetCurrentAttackStyle(style_code, FALSE);
}

DWORD GameFunc::GetRandomStyle(DWORD dwWeaponStyle,BYTE bClassCode)
{
	DWORD dwStyle=0;
	int iRand=0;
	iRand=rand()%3;

	switch(dwWeaponStyle)
	{
	case eWEAPONTYPE_TWOHANDAXE:
		{
			if(iRand==0)
				dwStyle=eSTYLE_FUSS_BREAKER;		     
			else if(iRand==1)
				dwStyle=eSTYLE_HACKSTER_KNOCKBACK_AXE;
			else
				dwStyle=eSTYLE_ROLLANDS_DOWN;	     
		}
		break;

	case eWEAPONTYPE_TWOHANDSWORD:
		{
			if(iRand==0)
				dwStyle=eSTYLE_BALTURS_DEFENCE;		     
			else if(iRand==1)
				dwStyle=eSTYLE_HACKSTER_KNOCKBACK_TWOHAND;
			else
				dwStyle=eSTYLE_RUDBIGS_STUN;	
		}
		break;


	case eWEAPONTYPE_ONEHANDSWORD:
		{
	             
			if(iRand==0)
				dwStyle=eSTYLE_RESTROS_SPEED;		     
			else if(iRand==1)
				dwStyle=eSTYLE_TORES_CRITICAL;
			else
				dwStyle=eSTYLE_ELEMEOUS_KNOCKBACK_ONEHAND;	

		}
		break;

	case eWEAPONTYPE_SPEAR:
		{
			if(iRand==0)
				dwStyle=eSTYLE_ALRESTER_DAMAGE;		     
			else if(iRand==1)
				dwStyle=eSTYLE_ELEMEOUS_KNOCKBACK_LANCE;
			else
				dwStyle=eSTYLE_FLABIOUS_PIERCING;	
		}
		break;

	case eWEAPONTYPE_ONEHANDCROSSBOW:
		{
			dwStyle=eSTYLE_ONEHANDCROSSBOW_NORMAL;
		}
		break;

	case eWEAPONTYPE_ETHERWEAPON:
		{
			dwStyle=eSTYLE_ETHER_NORMAL;
		}
		break;


	case eWEAPONTYPE_STAFF:
		{
			dwStyle=eSTYLE_STAFF_NORMAL;
		}
		break;

	case eWEAPONTYPE_ORB:
		{
			dwStyle=eSTYLE_ORB_NORMAL;
		}
		break;						

	case eWEAPONTYPE_DAGGER:
		{
			dwStyle=eSTYLE_SHADOW_DAGGER_NORMAL;
		}
		break;

	case eWEAPONTYPE_WHIP:
		{
			dwStyle=eSTYLE_SHADOW_WHIP_NORMAL;
		}
		break;
	case eWEAPONTYPE_PUNCH:
		{
			if(bClassCode==eCHAR_BERSERKER)
			{
				dwStyle=eSTYLE_BERSERKER_PUNCH;
			}
			else if(bClassCode==eCHAR_DRAGON)
			{
				dwStyle=eSTYLE_DRAGON_PUNCH;
			}
			else if(bClassCode==eCHAR_SHADOW)
			{
				dwStyle=eSTYLE_SHADOW_PUNCH;
			}
			else if(bClassCode==eCHAR_VALKYRIE)
			{
				dwStyle=eSTYLE_VALKYRIE_PUNCH;
			}
			else if(bClassCode==eCHAR_MAGICIAN)
			{
				dwStyle=eSTYLE_MAGICIAN_PUNCH;
			}	
		}
		break;	
	}

	return dwStyle;
}

Monster* GameFunc::SpawnMonster(CODETYPE MonsterCode, size_t MonsterCount /* = 1 */, WzVector* wzVec /* = NULL*/, DWORD dwMaster /* = 0*/)
{
    if (!g_pHero)
    {
        return NULL;
    }

	BASE_NPCINFO* pNpcInfo = NPCInfoParser::Instance()->GetNPCInfo(MonsterCode);

    WzVector wvPos;
	if (NULL == wzVec)
    {
		wvPos = g_pHero->GetVisiblePos();
    }
	else 
    {
		wvPos = *wzVec;
    }

	Monster* pMonster = NULL;

	
	if (pNpcInfo && pNpcInfo->m_wNpcImageIndex)
	{
		BASE_NPCImageInfo* pNpcImageInfo = NPCImageInfoParser::Instance()->GetNpcImageInfo(pNpcInfo->m_wNpcImageIndex);

		if (pNpcImageInfo)
		{
			PlayerMonster* pPlayerMonster = (PlayerMonster*)g_ObjectManager.Add(g_ObjectManager.GenerateKeyAtSingleMode(MONSTER_OBJECT), SSQMONSTER_OBJECT, pNpcImageInfo->bClassCode, FALSE);
            
            // 저탑 보스 등으로 등장하는 플레이어형몬스터의 경우 MONSTER_OBJECT 세팅
			pPlayerMonster->SetMonsterInfo(pNpcInfo);

			//PLAYER_RENDER_INFO_EX info;
			//ZeroMemory(&info, sizeof(PLAYER_RENDER_INFO_EX));
			//info.char_class = pNpcImageInfo->bClassCode;
			//info.level = 10;
			//info.hp = 100;
			//info.max_hp = 200;
			//info.move_speed_ratio = 100;
			//info.attack_speed_ratio = 100;

			//pPlayerMonster->SetPlayerInfo(&info);
			srand(MonsterCode);
			if (pPlayerMonster)
			{
                // 외형세팅
                pPlayerMonster->SetPartVariation(PLAYER_VARIATION_HAIR, pNpcImageInfo->hairImageID);
                pPlayerMonster->SetPartVariation(PLAYER_VARIATION_FACE, pNpcImageInfo->faceImageID);
                BOOL is_hide_helmet = (pNpcImageInfo->showhelmet == 1) ? false : true;
                pPlayerMonster->HideHelmet(is_hide_helmet, FALSE);

                NPC::InitNpcImageInfo(pPlayerMonster, pPlayerMonster->GetMonsterInfo());
                GameFunc::SetPlayerMonsterAttackStyle(pPlayerMonster);

                
                pPlayerMonster->RefreshEquipData();
				pPlayerMonster->SetNextState(STATE::IDLE, g_CurTime);	
				pPlayerMonster->CommitChangeState(g_CurTime);	

				pPlayerMonster->SetMoveSpeedRatio(100);
				pPlayerMonster->SetAttackSpeedModifier(100);

				pPlayerMonster->SetPosition(wvPos);
				tempId = pPlayerMonster->GetObjectKey();
				m_dwTestMonsterId = tempId;

			}

			return pPlayerMonster;
		}
		else
        {
			pMonster = (Monster*)g_ObjectManager.Add(g_ObjectManager.GenerateKeyAtSingleMode(MONSTER_OBJECT), MONSTER_OBJECT, MonsterCode);
        }

	}
	else
	{	
		pMonster = (Monster*)g_ObjectManager.Add(g_ObjectManager.GenerateKeyAtSingleMode(MONSTER_OBJECT), MONSTER_OBJECT, MonsterCode);
		PLAYER_ACTION idleaction;

		if (pNpcInfo)
		{
			idleaction.SPAWN.dwEndTime = pNpcInfo->m_dwSpawnTime + g_CurTime;
		}

		pMonster->SetCurrentAction(&idleaction);
		pMonster->SetNextState(STATE::SPAWN,g_CurTime);		
		pMonster->SetAttackSpeedRatio(100);

		WzVector vTemp;
		vTemp.x = 56.0f;
		vTemp.y = 57.0f;
		vTemp.z = 0.0f;
		pMonster->SetDirection(vTemp);
		pMonster->SetHP(pMonster->GetMonsterInfo()->m_dwMaxHP);


	}

	if (pMonster)
	{
		pMonster->SetPosition(wvPos);
		
		if (g_pHero && (eNPC_CRYSTAL_WARP == pMonster->GetMonsterInfo()->m_byGrade))
		{
			if (RELATIONKIND::Enemy == GameFramework::GetCurrentScene()->GetRelationKindOfCharacter(pMonster, g_HeroInput.lsForceAttack()))
			{
				//빨강
				pMonster->SetParentId(0, 0, TRUE);
				pMonster->CreateCrystalWarpEffect(StrToWzID("O078"));
			}
			else
			{
				//파랑
				pMonster->SetParentId(0, 0, FALSE);
				pMonster->CreateCrystalWarpEffect(StrToWzID("O081"));
			}

			pMonster->SetMasterGuildID(g_pHero->GetPlayerGuildID());
			g_pHero->SetCrystalWarpKey(pMonster->GetObjectKey());
		}

		tempId = pMonster->GetObjectKey();
		m_dwTestMonsterId = tempId;
	}

	return pMonster;
}

WzVector GameFunc::FindStandPosition(WzVector vSrc,WzVector vDest)
{
	WzVector vTemp;
	float fRate=1.0f;

	//최대 10회까지만 찾는다
	for(int i=0;i<10;i++)
	{
		vTemp =  vSrc* (1.0f - fRate) +  vDest* (fRate);

		if(g_pSunTerrain)
		{
			int iTileFocussing = -1;
			float fT=0.0f;
			iTileFocussing = g_pSunTerrain->x_pPathFinder->GetAllowTileToStand( vTemp, &fT, -1.0f, 10.0f );

			if(iTileFocussing >=0)
			{

				if(g_pPathExplorer)
				{
					g_pPathExplorer->Stop();
					g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vTemp, -1);
					return g_pPathExplorer->GetVisiblePos();

				}
				else
				{
					vTemp.z-=fT;
					return vTemp;
				}
			}
		}

		fRate-=0.1f;
	}

	return vSrc;
}

//-------------------------------------------------------------------------------------------
/** 특정 위치에서 특정 범위안에 있는 이동가능한 타일의 위치를 얻어온다.
*/
BOOL GameFunc::GetValidPosForRange(WzVector & rwzPos, WzVector & OUT rOutwzReturn, float fRangeLength /* = 10.f */ )
{   
    if (fRangeLength <= 0)
    {
        return FALSE;
    }


	BOOL bRand = FALSE;
	if ( 1 == (rand() % 2) )
	{
		bRand = TRUE;	
	}

	float fStartAngle = 0.f;
	
	if (bRand)
	{
		fStartAngle = WZ_PI;	
	}

    for ( float fRange = fRangeLength; fRange > 0.f; fRange -= 5.0f)
    {		
        for ( float fAngle = fStartAngle; fAngle < 2 * WZ_PI; fAngle += (WZ_PI / 6.f) )
        {
            rOutwzReturn = rwzPos;
            
            rOutwzReturn.x += sinf(fAngle) * fRangeLength;
            rOutwzReturn.y += cosf(fAngle) * fRangeLength;
            rOutwzReturn.z += 3.0f;

            int iTileFocussing = -1;
            float fT=0.0f;
            iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( rOutwzReturn, &fT, -1.0f, 10.0f );

            if(iTileFocussing >= 0)
            {
                rOutwzReturn.z -= fT;
            }

            if( IsMove(rwzPos,rOutwzReturn) )
            {
                return TRUE;
            }                    
        }
    }

    return FALSE;
}

BOOL GameFunc::PickTerrainMesh( const WzVector & IN vOrigin,WzVector & OUT vPickPos, BYTE CheckType, float height )
{	
	WzRay ray;
	ray.m_wvOrigin = vOrigin;
	ray.m_wvOrigin.z += height;
	ray.m_wvDirection.x = 0.0f;
	ray.m_wvDirection.y = 0.0f;
	ray.m_wvDirection.z = -1.0f;

	float		fT;
	BOOL        bFind=TRUE;
	int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT, TRUE, CheckType);
	if( iTerrainMeshFocussing < 0 )
	{
		ray.m_wvOrigin = vOrigin;
		ray.m_wvOrigin.z += height;
		ray.m_wvDirection.x = 0.0f;
		ray.m_wvDirection.y = 0.0f;
		ray.m_wvDirection.z = +1.0f;

		if( iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh( &ray, &fT, FALSE, CheckType ) >=0 )
		{
			vPickPos = ray.m_wvOrigin + ray.m_wvDirection * fT;

		}
		else 
		{
			vPickPos=vOrigin;
			bFind=FALSE;
		}

	}
	else
	{
		vPickPos = ray.m_wvOrigin + ray.m_wvDirection * fT;
	}		

	return bFind;
}

BOOL                 
GameFunc::GetValidPosForRange(WzVector & IN rwzPos, float IN fAngle, WzVector & OUT rOutwzReturn, float fRangeLength /*= 10.f*/)
{
	if (fRangeLength <= 0)
	{
		return FALSE;
	}

	WzVector wvDir;

	int iCount = int(360 / 15);
	float fAngleBasic = fAngle;

	for ( float fRange = fRangeLength; fRange > 0.f; fRange -= 3.0f)
	{				
		fAngle = fAngleBasic;
		for ( int i = 0; i < iCount; ++i )
		{
			fAngle = fAngle + i * (15 * WZ_PI / 180.f);

			wvDir.x = cosf(fAngle);
			wvDir.y = sinf(fAngle);
			wvDir.z = 0.f;

			rOutwzReturn = rwzPos + wvDir * fRange;
			rOutwzReturn.z += 3.f;

			int iTileFocussing = -1;
			float fT = 0.0f;
			iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( rOutwzReturn, &fT, -1.0f, 10.0f );

			if(iTileFocussing >= 0)
			{
				rOutwzReturn.z -= fT;
			}

			if( IsMove(rwzPos,rOutwzReturn) )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

float LerfFloat(float fNumber,float fNumber2,float fRate)
{
	if (fRate >= 1.0f)
	{
		fRate = 1.0f;
	}

	if (fRate < 0.0f)
	{
		fRate = 0.0f;
	}

	float fRet = fNumber * (1.0f - fRate) + fNumber2 * fRate;
	return fRet;
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInField( Character* pObject, BOOL bForceEnemy )
{
	if( pObject->IsKindOfObject( PLAYER_OBJECT ) )
	{
		if( !g_isPkServer )
			return RELATIONKIND::Friend;

		Player * pPlayer = (Player *)pObject;
		//에소드는 필드지만 마을로 간주되기에  PK막는다
		if(g_pMap && g_pMap->IsEsodeMap())
		{	
			return RELATIONKIND::Friend;
		}
		return GetRelationOfPlayer( pPlayer, bForceEnemy );

	}
	else if( pObject->IsKindOfObject( MONSTER_OBJECT ) )
	{
		Monster * pMonster = (Monster *)pObject;

		return GetRelationOfMonster( pMonster, bForceEnemy );		
	}
	else if(pObject->IsKindOfObject( PET_OBJECT))
	{
		return RELATIONKIND::None;
	}
	else if (pObject->IsKindOfObject( NPC_OBJECT) || pObject->IsKindOfObject( LOTTO_NPC_OBJECT))
	{
		return RELATIONKIND::Assistant;
	}
    else if (pObject->IsKindOfObject( MERCHANT_OBJECT ))
    {
        return RELATIONKIND::Vendor;
    }

	return RELATIONKIND::None;
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInVillage( Character* pObject, BOOL bForceEnemy )
{
	if( pObject->IsKindOfObject( PLAYER_OBJECT ) )
	{
		Player * pPlayer = (Player *)pObject;

		if( g_pHero && g_pHero->GetObjectKey() == pPlayer->GetObjectKey() )
			return RELATIONKIND::Friend;

		ePLAYER_BEHAVE_STATE behaveState = pPlayer->GetBeHaveState();

		if( behaveState == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE )
			return RELATIONKIND::Vendor;

		return RELATIONKIND::Friend;
	}
	else if( pObject->IsKindOfObject( MONSTER_OBJECT ) )
	{
		Monster * pMonster = (Monster *)pObject;

		return GetRelationOfMonster( pMonster, bForceEnemy );

	}
	else if(pObject->IsKindOfObject( PET_OBJECT))
	{
		return RELATIONKIND::None;
	}
	else if (pObject->IsKindOfObject( NPC_OBJECT) || pObject->IsKindOfObject( LOTTO_NPC_OBJECT))
	{
		return RELATIONKIND::Assistant;
	}
    else if (pObject->IsKindOfObject( MERCHANT_OBJECT ))
    {
        return RELATIONKIND::Vendor;
    }

	return RELATIONKIND::Friend;
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInPVPZone( Character* pObject,BOOL bForceEnemy )
{
	if (pObject->IsKindOfObject(PLAYER_OBJECT))
	{
		if( g_pHero && g_pHero->GetObjectKey() == pObject->GetObjectKey() )
        {
			return RELATIONKIND::Friend;
        }

        if ((pObject->GetObjectKey() == EQUIPMENT_CHARACTER_KEY) || 
            (pObject->GetObjectKey() == CUSTOMIZE_CHARACTER_KEY))
        {
            return RELATIONKIND::Friend;
        }

		if (GlobalFunc::IsParty(pObject->GetObjectKey()) == TRUE)
        {
			return RELATIONKIND::Friend;
        }
		else
        {
			return RELATIONKIND::Enemy;
        }
	}
	else if( pObject->IsKindOfObject( MONSTER_OBJECT ) )
	{
		Monster * pMon= (Monster *)pObject;

		if( g_pHero &&  g_pHero->GetObjectKey()==pMon->GetParentId())
		{	
			return RELATIONKIND::Friend;
		}
		return RELATIONKIND::Enemy;
	}
	else if(pObject->IsKindOfObject( PET_OBJECT))
	{
		return RELATIONKIND::None;
	}
	else if (pObject->IsKindOfObject( NPC_OBJECT) || pObject->IsKindOfObject( LOTTO_NPC_OBJECT))
	{
		return RELATIONKIND::Assistant;
	}
    else if (pObject->IsKindOfObject( MERCHANT_OBJECT ))
    {
        return RELATIONKIND::Vendor;
    }

	return RELATIONKIND::None;
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInSSQZone( Character* pObject,BOOL bForceEnemy )
{
	if( g_pHero && g_pHero->GetObjectKey() == pObject->GetObjectKey() )
		return RELATIONKIND::Friend;

	if( pObject->IsKindOfObject( MAP_OBJECT ) )
		return RELATIONKIND::None;

	if( pObject->IsKindOfObject( MONSTER_OBJECT ) )
	{
		Monster *pMonster = (Monster *)pObject;

		if( pMonster->GetMonsterType() == MONSTER_TYPE_TOTEM )
		{
			return RELATIONKIND::None;
		}

		Player * pPlayer = (Player *)g_ObjectManager.GetObject( pMonster->GetParentId() );

		if( pPlayer )
		{
			if( pPlayer->GetObjectKey() == g_pHero->GetObjectKey() )
			{
				return RELATIONKIND::Friend;
			}	
			else
			{
				return RELATIONKIND::Enemy;
			}
		}
		else
		{
			return RELATIONKIND::Enemy;
		}

	}

	return RELATIONKIND::Enemy;
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInHuntingZone( Character* pObject,BOOL bForceEnemy )
{
	if( pObject->IsKindOfObject( PLAYER_OBJECT ) )
	{
		if( g_pHero && g_pHero->GetObjectKey() == pObject->GetObjectKey() )
			return RELATIONKIND::Friend;

#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
		uiBattleZoneReadyCHuntingConfig * cfg = GET_CAST_DIALOG( uiBattleZoneReadyCHuntingConfig,
												IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG );

		assert (cfg);

		if( cfg->GetCfgs().compModeType == eCOMPETITION_MODE_SURVIVAL ||
			cfg->GetCfgs().compModeType == eCOMPETITION_MODE_DEATH_MATCH )
		{

			if( GlobalFunc::IsParty( pObject->GetObjectKey() ) )
				return RELATIONKIND::Friend;
			else
				return RELATIONKIND::Enemy;
		}
		else
		{
			return RELATIONKIND::Friend;
		}
#else
        return RELATIONKIND::Friend;
#endif // !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
	}
	else if( pObject->IsKindOfObject( MONSTER_OBJECT ) )
	{
		Monster *pMonster = (Monster *)pObject;

		if( pMonster->GetMonsterType() == MONSTER_TYPE_TOTEM )
		{
			return RELATIONKIND::None;
		}
		else if( pMonster->GetParentId() )
		{
			Player * pPlayer = (Player *)g_ObjectManager.GetObject( pMonster->GetParentId() );

			if( pPlayer )//주인이 있다면 주인과의 관계를 본다.
			{
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)

				uiBattleZoneReadyCHuntingConfig * cfg = GET_CAST_DIALOG( uiBattleZoneReadyCHuntingConfig,
  														IM_BATTLEZONE_MANAGER::BATTLEZONE_READY_CHUNTING_CFG );
				assert (cfg);

				if (cfg->GetCfgs().compModeType == eCOMPETITION_MODE_SURVIVAL ||
					cfg->GetCfgs().compModeType == eCOMPETITION_MODE_TIME_ATTACK ||
					cfg->GetCfgs().compModeType == eCOMPETITION_MODE_DEATH_MATCH)
				{
					if( GlobalFunc::IsParty( pPlayer->GetObjectKey() ) )
						return RELATIONKIND::Friend;
					else
						return RELATIONKIND::Enemy;
				}
				else
				{
					return RELATIONKIND::Friend;
				}
#else
                return RELATIONKIND::Friend;
#endif//#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
			}
		}

		return RELATIONKIND::Enemy;
	}
	else if(pObject->IsKindOfObject( PET_OBJECT))
	{
		return RELATIONKIND::None;
	}
	else if (pObject->IsKindOfObject( NPC_OBJECT) || pObject->IsKindOfObject( LOTTO_NPC_OBJECT))
	{
		return RELATIONKIND::Assistant;
	}
    else if (pObject->IsKindOfObject( MERCHANT_OBJECT ))
    {
        return RELATIONKIND::Vendor;
    }

	return RELATIONKIND::None;	
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInMissionZone( Character* pObject,BOOL bForceEnemy )
{
	if( pObject->IsKindOfObject( PLAYER_OBJECT ) )
	{
		return RELATIONKIND::Friend;
	}
	else if( pObject->IsKindOfObject( MONSTER_OBJECT ) )
	{
		Monster * pMonster = (Monster *)pObject;

		return GetRelationOfMonster( pMonster, bForceEnemy );		
	}
	else if(pObject->IsKindOfObject( PET_OBJECT))
	{
		return RELATIONKIND::None;
	}
	else if (pObject->IsKindOfObject( NPC_OBJECT) || pObject->IsKindOfObject( LOTTO_NPC_OBJECT))
	{
		return RELATIONKIND::Assistant;
	}
    else if (pObject->IsKindOfObject( MERCHANT_OBJECT ))
    {
        return RELATIONKIND::Vendor;
    }

	return RELATIONKIND::None;	
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInInstanceDungeon( Character* pObject,BOOL bForceEnemy )
{
	return GetRelationKindOfPlayerInMissionZone( pObject,bForceEnemy );
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInDominationWar( Character* pObject, BOOL bForceEnemy /*= FALSE*/ )
{
	if(pObject->IsKindOfObject( PLAYER_OBJECT ))
	{
		Player* pPlayer = (Player*)pObject;
		if (pPlayer->IsExistGuild())
		{
			return GetRelationOfPlayer(pPlayer, bForceEnemy);
		}
        else
        {
            return RELATIONKIND::Enemy;
        }
	}
	else if(pObject->IsKindOfObject( MONSTER_OBJECT ))
	{
		Monster * pMonster = (Monster *)pObject;

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        // 기본적으론 보통 피아식별과 같지만, 수성 오브젝트를 수성연합에게도 아군으로 인식하게 한다.
        // 자신이 수성 연합이라면
        uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
        if (ui_domination_manager && 
            ui_domination_manager->IsDefenceGuildAlliance(g_pHero->GetGuildRenderInfo().m_GuildGuid))
        {
            // 수성 오브젝트는 아군이다.
            DOMINATION_GUILD_INFO* domination_info = ui_domination_manager->GetDominationGuildInfo();
            if (domination_info->m_GuildGuid == pMonster->GetMasterGuildID())
            {
                return RELATIONKIND::Friend;
            }
        }
        
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

		return GetRelationOfMonster( pMonster, bForceEnemy );	
	}
#ifdef _NA_008243_20150422_DOMINATION_SCENE_OPEN_NPC_STORE
	else if (pObject->IsKindOfObject( NPC_OBJECT) || pObject->IsKindOfObject( LOTTO_NPC_OBJECT))
	{
		return RELATIONKIND::Assistant;
	}
#endif // _NA_008243_20150422_DOMINATION_SCENE_OPEN_NPC_STORE

	return RELATIONKIND::None;
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInBattleGround(Character* pObject, BOOL bForceEnemy /*= FALSE*/)
{
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (g_pHero == NULL)
    {
        return RELATIONKIND::None;
    }
    
    if (pObject->IsKindOfObject(PLAYER_OBJECT))
    {
        if (g_pHero->GetObjectKey() == pObject->GetObjectKey())
        {
            return RELATIONKIND::Friend;
        }

        Player* player = static_cast<Player*>(pObject);
        if (player->GetTeam() == INVALID_TEAM)
        {
            // 팀 설정이 안되있으면 전장인포에서 가져와서 저장한다.
            WORD team_code = ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(player->GetName());
            if (team_code != 0)
            {
                player->SetTeam(team_code);
            }
        }
        
        if (player->GetTeam() <= 0)
        {
            if (GlobalFunc::IsParty(player->GetObjectKey()) == TRUE)
            {
                return RELATIONKIND::Friend;
            }
            else
            {
                return RELATIONKIND::None;
            }
        }
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        else if (player->GetTeam() == kObserve)
        {
            return RELATIONKIND::None;
        }
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        else if (g_pHero->GetTeam() == player->GetTeam())
        {
            return RELATIONKIND::Friend;
        }
        else
        {
            return RELATIONKIND::Enemy;
        }
    }
    else if (pObject->IsKindOfObject(MONSTER_OBJECT))
    {
        Monster* monster = static_cast<Monster*>(pObject);

        if (monster->GetMonsterType() == MONSTER_TYPE_TOTEM)
        {
            return RELATIONKIND::None;
        }
        else if (monster->GetParentId())
        {
            Player* player = static_cast<Player*>(g_ObjectManager.GetObject(monster->GetParentId()));
            if (player != NULL)//주인이 있다면 주인과의 관계를 본다.
            {
                if (player->GetTeam() == INVALID_TEAM)
                {
                    // 팀 설정이 안되있으면 전장인포에서 가져와서 저장한다.
                    WORD team_code = ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(player->GetName());
                    player->SetTeam(team_code);
                }

                if (player->GetTeam() <= 0)
                {
                    return RELATIONKIND::None;
                }
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
                else if (player->GetTeam() == kObserve)
                {
                    return RELATIONKIND::None;
                }
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
                else if (g_pHero->GetTeam() == player->GetTeam())
                {
                    return RELATIONKIND::Friend;
                }
                else
                {
                    return RELATIONKIND::Enemy;
                }
            }
        }

        return RELATIONKIND::Enemy;
    }
    else if (pObject->IsKindOfObject(PET_OBJECT))
    {
        return RELATIONKIND::None;
    }
    else if (pObject->IsKindOfObject(NPC_OBJECT) || pObject->IsKindOfObject(LOTTO_NPC_OBJECT))
    {
        return RELATIONKIND::Assistant;
    }
    else if (pObject->IsKindOfObject(MERCHANT_OBJECT))
    {
        return RELATIONKIND::Vendor;
    }

#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    return RELATIONKIND::None;
}

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInGoldRush(Character* pObject, BOOL bForceEnemy /*= FALSE*/)
{
    if (g_pHero == NULL)
    {
        return RELATIONKIND::None;
    }

    if (pObject->IsKindOfObject(PLAYER_OBJECT))
    {
        if (g_pHero->GetObjectKey() == pObject->GetObjectKey())
        {
            return RELATIONKIND::Friend;
        }

        Player* player = static_cast<Player*>(pObject);
        if (player->GetTeam() == INVALID_TEAM)
        {
            // 팀 설정이 안되있으면 골드러시인포에서 가져와서 저장한다.
            WORD team_code = ChaosSystem::GetGoldRushData().GetGoldRushTeamForPlayerName(player->GetName());
            if (team_code != 0)
            {
                player->SetTeam(team_code);
            }
        }

        if (player->GetTeam() <= 0)
        {
            if (GlobalFunc::IsParty(player->GetObjectKey()) == TRUE)
            {
                return RELATIONKIND::Friend;
            }
            else
            {
                return RELATIONKIND::None;
            }
        }
        else if (player->GetTeam() == kObserve)
        {
            return RELATIONKIND::None;
        }
        else if (g_pHero->GetTeam() == player->GetTeam())
        {
            return RELATIONKIND::Friend;
        }
        else
        {
            return RELATIONKIND::Enemy;
        }
    }
    else if (pObject->IsKindOfObject(MONSTER_OBJECT))
    {
        Monster* monster = static_cast<Monster*>(pObject);

        if (monster->GetMonsterType() == MONSTER_TYPE_TOTEM)
        {
            return RELATIONKIND::None;
        }
        else if (monster->GetParentId())
        {
            Player* player = static_cast<Player*>(g_ObjectManager.GetObject(monster->GetParentId()));
            if (player != NULL)//주인이 있다면 주인과의 관계를 본다.
            {
                if (player->GetTeam() == INVALID_TEAM)
                {
                    // 팀 설정이 안되있으면 골드러시인포에서 가져와서 저장한다.
                    WORD team_code = ChaosSystem::GetGoldRushData().GetGoldRushTeamForPlayerName(player->GetName());
                    player->SetTeam(team_code);
                }

                if (player->GetTeam() <= 0)
                {
                    return RELATIONKIND::None;
                }
                else if (player->GetTeam() == kObserve)
                {
                    return RELATIONKIND::None;
                }
                else if (g_pHero->GetTeam() == player->GetTeam())
                {
                    return RELATIONKIND::Friend;
                }
                else
                {
                    return RELATIONKIND::Enemy;
                }
            }
        }

        return RELATIONKIND::Enemy;
    }
    else if (pObject->IsKindOfObject(PET_OBJECT))
    {
        return RELATIONKIND::None;
    }
    else if (pObject->IsKindOfObject(NPC_OBJECT) || pObject->IsKindOfObject(LOTTO_NPC_OBJECT))
    {
        return RELATIONKIND::Assistant;
    }
    else if (pObject->IsKindOfObject(MERCHANT_OBJECT))
    {
        return RELATIONKIND::Vendor;
    }

    return RELATIONKIND::None;
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInFreePVP(Character* pObject, BOOL bForceEnemy /*= FALSE*/)
{
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    if (g_pHero == NULL)
    {
        return RELATIONKIND::None;
    }

    // 인벤 캐릭터와 상대방 정보보기 캐릭터가 미니맵에 나오는 현상 수정
    if ((pObject->GetObjectKey() > ONLYCLIENT_OBJECT_KEY) &&
        (pObject->GetObjectKey() < MAX_OBJECT_KEY))
    {
        return RELATIONKIND::None;
    }

    if (pObject->IsKindOfObject(PLAYER_OBJECT))
    {
        if (g_pHero && g_pHero->GetObjectKey() == pObject->GetObjectKey())
        {
            return RELATIONKIND::Friend;
        }

        if (GlobalFunc::IsParty(pObject->GetObjectKey()) == TRUE)
        {
            return RELATIONKIND::Friend;
        }
        else
        {
            return RELATIONKIND::Enemy;
        }
    }
    else if (pObject->IsKindOfObject(MONSTER_OBJECT))
    {
        Monster* monster = static_cast<Monster*>(pObject);

        if (g_pHero && g_pHero->GetObjectKey() == monster->GetParentId())
        {	
            return RELATIONKIND::Friend;
        }
        else if (GlobalFunc::IsParty(monster->GetParentId()))
        {
            return RELATIONKIND::Friend;
        }
        return RELATIONKIND::Enemy;
    }
    else if (pObject->IsKindOfObject(PET_OBJECT))
    {
        return RELATIONKIND::None;
    }
    else if (pObject->IsKindOfObject(NPC_OBJECT) || pObject->IsKindOfObject(LOTTO_NPC_OBJECT))
    {
        return RELATIONKIND::Assistant;
    }
    else if (pObject->IsKindOfObject(MERCHANT_OBJECT))
    {
        return RELATIONKIND::Vendor;
    }
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    return RELATIONKIND::None;
}

RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInSpa(Character* pObject, BOOL bForceEnemy /*= FALSE*/)
{
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    if (g_pHero == NULL)
    {
        return RELATIONKIND::None;
    }

    // 인벤 캐릭터와 상대방 정보보기 캐릭터가 미니맵에 나오는 현상 수정
    if ((pObject->GetObjectKey() > ONLYCLIENT_OBJECT_KEY) &&
        (pObject->GetObjectKey() < MAX_OBJECT_KEY))
    {
        return RELATIONKIND::None;
    }

    if (pObject->IsKindOfObject(PLAYER_OBJECT))
    {
        return RELATIONKIND::Friend;
    }
    else if (pObject->IsKindOfObject(MONSTER_OBJECT))
    {
        return RELATIONKIND::None;
    }
    else if (pObject->IsKindOfObject(PET_OBJECT))
    {
        return RELATIONKIND::None;
    }
    else if (pObject->IsKindOfObject(NPC_OBJECT) || pObject->IsKindOfObject(LOTTO_NPC_OBJECT))
    {
        return RELATIONKIND::Assistant;
    }
    else if (pObject->IsKindOfObject(MERCHANT_OBJECT))
    {
        return RELATIONKIND::Vendor;
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
    return RELATIONKIND::None;
}

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
RELATIONKIND::TYPE	GameFunc::GetRelationKindOfPlayerInGuildMission(Character* pObject, BOOL bForceEnemy /*= FALSE*/)
{
    if (g_pHero == NULL)
    {
        return RELATIONKIND::None;
    }

    // 인벤 캐릭터와 상대방 정보보기 캐릭터가 미니맵에 나오는 현상 수정
    if ((pObject->GetObjectKey() > ONLYCLIENT_OBJECT_KEY) &&
        (pObject->GetObjectKey() < MAX_OBJECT_KEY))
    {
        return RELATIONKIND::None;
    }

    if (pObject->IsKindOfObject(PLAYER_OBJECT))
    {
        return RELATIONKIND::Friend;
    }
    else if (pObject->IsKindOfObject(MONSTER_OBJECT))
    {
        Monster* monster = static_cast<Monster*>(pObject);

        return GetRelationOfMonster( monster, bForceEnemy );		
    }
    else if (pObject->IsKindOfObject(PET_OBJECT))
    {
        return RELATIONKIND::None;
    }
    else if (pObject->IsKindOfObject(NPC_OBJECT) || pObject->IsKindOfObject(LOTTO_NPC_OBJECT))
    {
        return RELATIONKIND::Assistant;
    }
    else if (pObject->IsKindOfObject(MERCHANT_OBJECT))
    {
        return RELATIONKIND::Vendor;
    }

    return RELATIONKIND::None;
}
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
//--------------------------------------------------------------------------------------------------
/**  
*/
bool GameFunc::IsCheckItem(SCSlot& slot)
{
    SCItemSlot& item_slot = static_cast<SCItemSlot&>(slot);
    if (item_slot.IsContainValidItem())
    {
        if (item_slot.IsEtheriaItem())
        {
            return false;
        }

        if (item_slot.GetImageCode() != item_slot.GetCode())
        {
            return true;
        }

        if (item_slot.IsShellItem())
        {
            return true;
        }
    }
    return false;
}
//--------------------------------------------------------------------------------------------------
/**  
*/
CODETYPE GameFunc::GetImagecode(SCItemSlot* item_slot, CODETYPE base_code)
{
    if (item_slot &&
        item_slot->IsContainValidItem())
    {
        //에테리아 조합아이템의경우
        if (item_slot->GetImageCode() != base_code)
        {
            return item_slot->GetImageCode();
        }
        else if (item_slot->IsLimited())
        {
            return item_slot->GetLimitedIndex();
        }
    }

    return base_code;
}
//--------------------------------------------------------------------------------------------------
/**  
*/
BOOL GameFunc::IsLimitedItem(SCItemSlot* item_slot, CODETYPE base_code)
{
    if (item_slot &&
        item_slot->IsContainValidItem())
    {
        if (item_slot->GetImageCode() != base_code)
        {
            return FALSE;
        }
        else if (item_slot->IsLimited())
        {
            return TRUE;
        }
    }
    return FALSE;
}
//--------------------------------------------------------------------------------------------------
/** 아이템커스터마이징 활성화 여부
*/
bool GameFunc::IsActiveCharCustomize(POSTYPE pos)
{
    uiCharCustomizeDlg* custom_dlg = GET_CAST_DIALOG(uiCharCustomizeDlg,
                                                     IM_CHAR_CUSTOMIZE_MGR::CHAR_CUSTOMIZE_DLG);
    
    if (custom_dlg == NULL)
    {
        return false;
    }

    return custom_dlg->IsActiveSlot(pos);
}
//--------------------------------------------------------------------------------------------------
/** 아이템슬롯, 아이템인포가 유효해야 네임을 얻을수있다.
*/
void GameFunc::GetRealItemName(SCItemSlot* slot_item, 
                               const BASE_ITEMINFO* item_info, 
                               TCHAR* name , 
                               int size)
{
    if (item_info == NULL)
    {
        return;
    }
    
    int string_code =item_info->m_NCode;
    // 소켓(쟈드 아이템)
    if(item_info->m_wType == eITEMTYPE_SOCKET_ZARD && item_info->zard_grade > 0)
    {
        // 12119 : 하급, 12120 중급 12121 상급 12122 최상급
        TCHAR temp_name[128] = {0,};
        TCHAR copy_name[MAX_ITEMNAME_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(string_code, copy_name, size - 1);// 아이템 이름
        
        string_code = 12118 + item_info->zard_grade;
        g_InterfaceManager.GetInterfaceString(string_code, temp_name, size - 1);// 등급 추가
        _snprintf(name, size - 1, "%s %s", temp_name, copy_name);
        return;
    }

    if ((slot_item != NULL) &&
        (slot_item->IsContainValidItem() == TRUE))
    {
        // 무기에테리아일땐 등급표시
        BOOL is_grade_show = (item_info->IsWeapon() ||
            item_info->IsArmor());

        if (slot_item->IsEtheriaItem())
        {
            if (is_grade_show)
            {
                WORD item_level = slot_item->GetItemInfo()->m_LV;
                string_code = GetItemGradeStringCode(name, item_level, item_info->sub_type_detailed);
                if (string_code == kNormalItemResultStringCode)
                {
                    return; // 0 이면 일반 아이템
                }
/*
                if (item_level < 17)
                {
                    TCHAR grade[128] ={0,};
                    // 30422 %d 등급
                    g_InterfaceManager.GetInterfaceString(30422, grade, 127);
                    _snprintf(name, size -1 , grade, item_level);
                    return;
                }//얼티메이트
                else if (item_level == 17)
                {
                    string_code = 30424;
                }//엘리트 1등급
                else if (item_level == 21)
                {
                    // 30428 : 타락한 엘리트1등급
                    string_code = (item_info->m_wSubType == 3) ? (30428):(30423);
                }//엘리트 1.5등급
                else if (item_level == 25)
                {
                    string_code = 30425;
                }//엘리트 2등급
                else if (item_level == 26)
                {
                    string_code = 30426;
                }*/
            }

        }//에테리아 조합아이템을 경우 이미지네임을 따라간다
        else if(slot_item->GetImageItemInfo()->m_Code != item_info->m_Code)
        {
            string_code = slot_item->GetImageItemInfo()->m_NCode;
        }
    }
    g_InterfaceManager.GetInterfaceString(string_code, name, size - 1);

}
//--------------------------------------------------------------------------------------------------
/** 커스텀 아이템일경우 추가스트링을 붙여준다
*/
void GameFunc::GetCustomizeInfo(SCItemSlot* slot_item, TCHAR* name)
{
    if (slot_item == NULL)
    {
        return;
    }

    if (slot_item->IsContainValidItem() == FALSE)
    {
        return;
    }

    if (slot_item->IsShellItem() ||
        slot_item->IsEtheriaItem())
    {
        TCHAR add_string[INTERFACE_STRING_LENGTH] = {0,};
        int string_code = slot_item->IsShellItem() ? (30472) : (30421);
        // 30421: 에테리아 
        // 30427: 껍데기
        g_InterfaceManager.GetInterfaceString(string_code, add_string, INTERFACE_STRING_LENGTH);
        _tcscat(name, _T(" "));
        _tcscat(name, add_string);
    }
}

void GameFunc::GetItemName(SCItemSlot* slot_item, 
                           const BASE_ITEMINFO* item_info, 
                           TCHAR* name , 
                           int size)
{
    TCHAR first_name[INTERFACE_STRING_LENGTH] = {0, };
    TCHAR second_name[INTERFACE_STRING_LENGTH] = {0, };

    GameFunc::GetRealItemName(slot_item, item_info, first_name, INTERFACE_STRING_LENGTH);

    if (item_info->IsWeapon() ||
        item_info->IsArmor())
    {
        g_InterfaceManager.GetInterfaceString(item_info->m_SCNCode, second_name, MAX_PATH);
#ifdef _RUSSIA
        Snprintf(name, (size - 1), "%s %s", second_name, first_name);
#else
        Snprintf(name, (size - 1), "%s %s", first_name, second_name);
#endif //_RUSSIA
    }
    else
    {
        Snprintf(name, (size - 1), "%s", first_name);
    }
    GameFunc::GetCustomizeInfo(slot_item, name);
}

void GameFunc::GetItemDescription(TCHAR* desc_string, SCItemSlot* item_slot, const BASE_ITEMINFO* item_info)
{
    if ((desc_string == NULL) ||
        (item_info == NULL))
    {
        return;
    }

    g_InterfaceManager.GetInterfaceString(item_info->m_TDNCode, desc_string, INTERFACE_STRING_LENGTH);

    if (item_info->m_dwExp > 0)
    {
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
        Snprintf(buffer_string, INTERFACE_STRING_LENGTH - 1, desc_string, item_info->m_dwExp);
        strcpy(desc_string, buffer_string);
    }
    else if ((item_slot != NULL) &&
             (item_slot->IsZardItem() == true))
    {
        SOCKETINDEX zard_option_index = item_slot->GetExtendData();
        if (zard_option_index == SocketOptionParser::kInvalidSocketOptionIndex)
        {
            const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();
            zard_option_index = zard_option_parser->GetFixedZardOption(item_slot->GetCode());// 옵션이 하나뿐인 쟈드 옵션을 얻어와서 세팅
        }

        if (zard_option_index != SocketOptionParser::kInvalidSocketOptionIndex)
        {
            bool is_check = false;

            const SocketOptionParser* const zard_option_parser = SocketOptionParser::Instance();
            const sSOCKET_OPTION* const zard_option = 
                zard_option_parser->FindSocketOption(zard_option_index);

            int socket_option_name_code = 0;
            const int zard_options_max = sSOCKET_OPTION::OptionPointInfo::kMaxListSize;
            if (zard_option != NULL)
            {
                TCHAR socket_buffer_string[zard_options_max][INTERFACE_STRING_LENGTH] = {0,};
                TCHAR socket_options_string[INTERFACE_STRING_LENGTH] = {0};
                TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR option_format_string[INTERFACE_STRING_LENGTH] = {0,};
                // 12118: [%s %d 포인트]
                g_InterfaceManager.GetInterfaceString(12118, option_format_string);

                // 소켓 옵션의 각 옵션 포인트에 대해
                for (sSOCKET_OPTION::OptionPointInfo::ListSizeType i = 0; 
                    i < zard_option->option_point_list_size; ++i)
                {
                    socket_buffer_string[i][0] = '\0';//초기화 ""
                    // 포인트 정보가 유효한지 검사한다.
                    const sSOCKET_OPTION::OptionPointInfo* const option_point_info = zard_option->GetPointInfo(i);
                    if (option_point_info == NULL)
                    {
                        continue;
                    }

                    // 옵션 정보가 유효한지 검사한다.
                    const ZardOptionInfo* const zard_option_info = option_point_info->zard_option;
                    if (zard_option_info == NULL || 
                        zard_option_info->attr_option_code == eATTR_TYPE_INVALID || 
                        zard_option_info->item_option_code == eATTR_TYPE_INVALID)
                    {
                        continue;
                    }

                    const BASE_OptionList* const zard_option_desc = OptionListParser::Instance()->GetOptionList(zard_option_info->item_option_code);
                    if (zard_option_desc == NULL)
                    {
                        continue;
                    }
                    // %s에 들어갈 옵션 종류 이름
                    socket_option_name_code = zard_option_desc->mdwNameCode;
                    //option_point_info->option_point; // %d에 들어갈 옵션 포인트
                    TCHAR option_string[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR comma_string[INTERFACE_STRING_LENGTH] = {0,};
                    
                    g_InterfaceManager.GetInterfaceString(socket_option_name_code, option_string);
                    sprintf(comma_string, "%c%s", (is_check && i < (zard_options_max - 1))?',':' ', option_format_string);
                    sprintf(socket_buffer_string[i], comma_string, option_string, option_point_info->option_point);
                    is_check = true;  
                }

                if (is_check == false)
                {
                }
                else
                {
                    sprintf(socket_options_string, "%s%s%s%s%s", socket_buffer_string[0], socket_buffer_string[1],
                        socket_buffer_string[2],socket_buffer_string[3],socket_buffer_string[4]);
                    // 12161: 무기나 방어구의 소켓에 %s를 부여합니다.<더블클릭으로 사용>
                    g_InterfaceManager.GetInterfaceString(12161, buffer_string);
                    sprintf(desc_string, buffer_string, socket_options_string);
                }
            }
        }
        else
        {
            // 상점 아이템인데 옵션이 여러개라서 메시지를 다르게 출력하는 경우를 여기에 구현하면 된다.
            const BASE_ITEMINFO* const zard = item_slot->GetItemInfo();
            if (zard != NULL)
            {
                const BASE_OptionList* const zard_option_desc = OptionListParser::Instance()->GetOptionList((DWORD)zard->zard_item_option_kind);
                if (zard_option_desc != NULL)
                {
                    TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR option_string[INTERFACE_STRING_LENGTH] = {0,};
                    TCHAR socket_options_string[INTERFACE_STRING_LENGTH] = {0};
                    int socket_option_name_code = 0;
                    // %s에 들어갈 옵션 종류 이름
                    socket_option_name_code = zard_option_desc->mdwNameCode;
                    g_InterfaceManager.GetInterfaceString(socket_option_name_code, option_string);
                    // 12169: [%s]
                    g_InterfaceManager.GetInterfaceString(12169, buffer_string);
                    sprintf(socket_options_string, buffer_string, option_string);
                    
                    // 12161: 무기나 방어구의 소켓에 %s를 부여합니다.<더블클릭으로 사용>
                    g_InterfaceManager.GetInterfaceString(12161, buffer_string);
                    sprintf(desc_string, buffer_string, socket_options_string);
                }
            }
        }
    }
    else if ((item_info != NULL) &&
        (item_info->m_wType == eITEMTYPE_SOCKET_COMPOSE_ADDITIVE))
    {
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR option_string[INTERFACE_STRING_LENGTH] = {0,};
        sprintf(buffer_string, desc_string, (int)(item_info->zard_success_per/10));
        strcpy(desc_string, buffer_string);
    }
    else if (item_info->m_wType == eITEMTYPE_SOCKETEXTRACT)//소켓 제거 아이템
    {
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
        DWORD string_code = 0;
        float zard_success_per = item_info->zard_success_per;//쟈드 회수 확률
        zard_success_per /= 10;
        if (zard_success_per > 0)
        {
            string_code = 159049; // 무기나 방어구의 소켓에 장착되어 있는 쟈드를 빼냅니다. %d%%의 확률로 쟈드가 온전히 분리됩니다.
            g_InterfaceManager.GetInterfaceString(string_code, buffer_string);
            sprintf(desc_string, buffer_string, (int)zard_success_per);
        }
        else
        {
            string_code = 159022; // 무기나 방어구의 소켓에 장착되어 있는 쟈드를 빼냅니다. 쟈드는 소멸됩니다.
            g_InterfaceManager.GetInterfaceString(string_code, desc_string);
        }
    }
    else if (item_info->m_wType == eITEMTYPE_DOMINATION_BUFF)
    {
        // ... SkillCode - Ability1Parameter1/Ability1Parameter2
        SkillScriptInfo* skill_info = SkillInfoParser::Instance()->GetSkillInfo(item_info->m_wSkillCode);
        BASE_ABILITYINFO* ability_info = NULL;
        if (skill_info != NULL)
        {
            ability_info = skill_info->GetAbilityInfoByIndex(0);
        }
        if (ability_info != NULL)
        {
            TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};

            if (ability_info->m_iParam[0] == VALUE_TYPE_VALUE)
            {
                sprintf(buffer_string, desc_string, ability_info->m_iParam[1]);
            }
            else
            {
                sprintf(buffer_string, desc_string, (ability_info->m_iParam[1] / 10));
            }
            strcpy(desc_string, buffer_string);
        }
    }
}

#ifdef _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
// 안전지대 추가 및 코드 정리
// 타일 속성별 룰 테이블
//  안전지대              경계지역                   분쟁지역            자유분쟁              점령전
//     x      Ctrl(파티,길드 및 20렙이하 불가)  O(파티,길드원 불가)   O,Ctrl(파티,길드원)   길드관계(공/방)
//                    길드관계 우선
// *서로 다른 속성타일에 있을 경우 공격불가
RELATIONKIND::TYPE PKRuleForTile::PKTile(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForceEnemy)
{
	// 경계지역 - 길드관계가 최우선
	RELATIONKIND::TYPE state = GameFunc::GetGuildRelationShip( pPlayer, bForceEnemy );
	if( state != RELATIONKIND::None )
		return state;

	// 파티원이나 길드원일 경우 Ctrl키를 눌러도 공격 불가
	if( GameFunc::IsFriendShip( pPlayer ) )
		return RELATIONKIND::Friend;

	// 20렙 미만이면 Ctrl키를 눌러도 공격 불가(레벨은 스크립트로 제어)
	if( g_pHero->GetLevel() < pPKInfo->m_PKAllowableLV || pPlayer->GetLevel() < pPKInfo->m_PKProtectLV )
	{
		return RELATIONKIND::Friend;
	}
	
	// 카오 및 보라돌이 처리
#ifdef _CHINA
	if( pPlayer->GetChaoState() == eCHAO_STATE_CHAO )
#else
	if( pPlayer->GetChaoState() == eCHAO_STATE_CHAO || pPlayer->IsAttackBeginPlayer() )
#endif
	{
		return RELATIONKIND::Enemy;
	}

	// 이외의 상황에서 Ctrl키 누름 - 공격
	if (bForceEnemy)
	{
		return RELATIONKIND::Enemy;
	}
	return RELATIONKIND::Friend;
}
RELATIONKIND::TYPE PKRuleForTile::ConflictTile(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForceEnemy)
{
	// 파티원이나 길드원일 경우 Ctrl키를 눌러도 공격 불가
	if(GameFunc::IsFriendShip( pPlayer ))
		return RELATIONKIND::Friend;

	return RELATIONKIND::Enemy;
}
RELATIONKIND::TYPE PKRuleForTile::FreeConflictTile(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForceEnemy)
{
	// 파티원이나 길드원일 경우 Ctrl키를 누르면 공격가능(누르지 않으면 공격불가)
	if(GameFunc::IsFriendShip( pPlayer ) && false == bForceEnemy)
		return RELATIONKIND::Friend;

	return RELATIONKIND::Enemy;
}
RELATIONKIND::TYPE PKRuleForTile::DominationTile(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForceEnemy)
{
	// 점령전 플레이어 체크
	if(pPlayer->IsExistGuild())
	{
		if( eGUILD_RELATION_ALLIENCE == pPlayer->GetGuildRenderInfo().m_RelationType )
			return RELATIONKIND::Friend;
		else if(eGUILD_RELATION_NONE == pPlayer->GetGuildRenderInfo().m_RelationType )
			return RELATIONKIND::None;
		else
			return RELATIONKIND::Enemy;
	}			
	return RELATIONKIND::None;
}
RELATIONKIND::TYPE PKRuleForTile::CheckRelationTileAttr(Player* pPlayer, PKOptionInfo* pPKInfo, bool bForce)
{
	if (NULL == g_pHero)
	{
		return RELATIONKIND::Friend;
	}

	// 자신과 상대 플레이어의 타일 속성을 가져온다.
	DWORD dwHeroAtt		= g_pHero->GetCurrentTileAtt(FALSE);
	DWORD dwPlayerAtt	= pPlayer->GetCurrentTileAtt(FALSE);

	if (GameFunc::GetCheckPlayerOnTile(dwHeroAtt, dwPlayerAtt, PTA_PK_TILE) && !(pPKInfo->m_IgnorePKTile&PKOptionInfo::ignore_BoundaryPart))
	{
		return PKTile(pPlayer, pPKInfo, bForce);
	}
	if (GameFunc::GetCheckPlayerOnTile(dwHeroAtt, dwPlayerAtt, PTA_CONFLICT_TILE) && !(pPKInfo->m_IgnorePKTile&PKOptionInfo::ignore_NomalPart))
	{
		return ConflictTile(pPlayer, pPKInfo, bForce);
	}			
	if (GameFunc::GetCheckPlayerOnTile(dwHeroAtt, dwPlayerAtt, PTA_FREE_CONFLICT_TILE) && !(pPKInfo->m_IgnorePKTile&PKOptionInfo::ignore_FreePart))
	{
		return FreeConflictTile(pPlayer, pPKInfo, bForce);
	}
	if (GameFunc::GetCheckPlayerOnTile(dwHeroAtt, dwPlayerAtt, PTA_DOMINATION_WAR_TILE))
	{
		return DominationTile(pPlayer, pPKInfo, bForce);
	}
	if (BIT_CHECK(dwHeroAtt,PTA_SAFETY_ZONE) || BIT_CHECK(dwPlayerAtt, PTA_SAFETY_ZONE))
	{
		return RELATIONKIND::Friend;
	}
	return RELATIONKIND::Friend;			
}
#endif //_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_

int GameFunc::CheckLODLevelByPlayerCount(int player_count, int option_lod_level)
{
    const int lod_count[4] = {17,20,32,35};
    static int lod_of_player_count = 0;

    int lod_level = 2;
    for (int i = 1; i < 4; i += 2)
    {
        if (player_count <= lod_count[i])
        {
            int temp_Level = (i >> 1);
            if ((lod_of_player_count == (temp_Level + 1)) && 
                (player_count > lod_count[i - 1]))
            {
                lod_level = lod_of_player_count;
            }
            else
            {
                lod_level = temp_Level;
            }
            break;
        }
    }
    lod_level = max(lod_level, option_lod_level);
    lod_of_player_count = lod_level;

    return lod_level;
}

void GameFunc::ApplyLODDistance(int lod_level)
{
    switch (lod_level)
    {
    case eOPTION_LEVEL_HIGH:
        {
            float fLODDistance[4] = {20.0f, 21.0f, 30.0f, 31.0f};
            g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD(fLODDistance, 4);	
        }
        break;
    case eOPTION_LEVEL_MIDDLE:
        {
            float fLODDistance[4] = {15.0f, 16.0f, 20.0f, 21.0f};
            g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD(fLODDistance, 4);	
        }
        break;
    case eOPTION_LEVEL_LOW:
        {
            float fLODDistance[4] = {10.0f, 11.0f, 15.0f, 16.0f};		
            g_pSunRenderer->x_pManagerUnitLOD->SetDistanceOfLOD(fLODDistance, 4);	
        }
        break;
    }
}

void GameFunc::ApplyEffectLODDistance(int lod_level)
{
    if( g_pEffectManager != NULL )
    {
        switch (lod_level)
        {
        case eOPTION_LEVEL_HIGH:
            {
                g_pEffectManager->ClearEffectLODParams();
                g_pEffectManager->AddEffectLODParam(30.0f, 40);
                g_pEffectManager->AddEffectLODParam(20.0f, 60);
                g_pEffectManager->AddEffectLODParam(15.0f, 70);
                g_pEffectManager->AddEffectLODParam(10.0f, 80);
            }
            break;
        case eOPTION_LEVEL_MIDDLE:
            {
                g_pEffectManager->ClearEffectLODParams();
                g_pEffectManager->AddEffectLODParam(30.0f, 20);
                g_pEffectManager->AddEffectLODParam(20.0f, 40);
                g_pEffectManager->AddEffectLODParam(15.0f, 60);
                g_pEffectManager->AddEffectLODParam(10.0f, 80);
            }
            break;
        case eOPTION_LEVEL_LOW:
            {
                g_pEffectManager->ClearEffectLODParams();
                g_pEffectManager->AddEffectLODParam(30.0f, 10);
                g_pEffectManager->AddEffectLODParam(20.0f, 20);
                g_pEffectManager->AddEffectLODParam(15.0f, 30);
                g_pEffectManager->AddEffectLODParam(10.0f, 50);
            }
            break;
        }
    }
}

int GameFunc::GetNearestTile(const WzVector& pos)
{
    float backup_length = 10000000.0f;

    int tile_no = -1;

    int tile_num = g_pSunTerrain->x_pPathFinder->GetNumPathTile();
    for (int i = 0; i < tile_num; ++i)
    {
        WzVector v = g_pSunTerrain->x_pPathFinder->GetNearestPointToTarget(i, pos);
        WzVector gab = v - pos;
        float length = VectorLength(&gab);
        if (length < backup_length)
        {
            tile_no = i;
            backup_length = length;
        }
    }

    return tile_no;
}

bool GameFunc::FindMoveableNearestTile( const WzVector& StartPosition, WzVector& DestPosition, int& tile_number )
{
    int StartTileIndex = g_pSunTerrain->x_pPathFinder->GetTileToStand( StartPosition );
    if( StartTileIndex == -1 )
    {
        return false;
    }

    bool empty_tile = false;
    int DestTileIndex = g_pSunTerrain->x_pPathFinder->GetTileToStand( DestPosition );
    if( DestTileIndex == -1 )
    {
        empty_tile = true;
        DestTileIndex = GetNearestTile( DestPosition, PTA_ONLY_JUMP | PTA_NO_WALK );
        if( DestTileIndex == -1 )
        {
            return false;
        }
    }

#ifdef _NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX
    bool block_tile = false;
    if (g_pSunTerrain->x_pPathFinder->GetAttribute(DestTileIndex) & (PTA_ONLY_JUMP | PTA_NO_WALK))
    {
        // 갈수 없는 타일을 가려고 할 경우
        block_tile = true;
    }
#endif //_NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX


    WzVector start = StartPosition;
    WzVector dest = DestPosition;
    int num_result;
    int result[1024];
    ENUM_FINDPATH_RESULT Result = g_pSunTerrain->x_pPathFinder->FindPath( StartTileIndex, DestTileIndex, &start, &dest,
                                    result, 1024, &num_result,
                                    0, 200000 );

    if( Result == EFR_SUCCESS )
    {
        for( int i=num_result-1; i>=0; --i )
        {
            WORD attrib = g_pSunTerrain->x_pPathFinder->GetAttribute( result[i] );
            if( attrib & PTA_NO_WALK )
            {
                continue;
            }

            if( attrib & PTA_ONLY_JUMP )
            {
                continue;
            }

            tile_number = result[i];

#ifdef _NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX
            if (empty_tile == true || block_tile == true)
            {
                // 목적지를 목적타일의 중심으로 바꿔줌
                // 위의 empty_tile은 GetNearestPointToTarget()에서 나온 좌표가 tile_number 타일을 벗어난 값이 나오는 경우가 종종있어서 오동작함
                // 어쨋든 block_tile 타일로 가려고 한 경우는 여기서 처리
                WzVector out_vec[3];
                WzVector center_vec;
                g_pSunTerrain->x_pPathFinder->GetTileVertexInfo(out_vec, tile_number);

                center_vec = out_vec[0] + out_vec[1] + out_vec[2];

                center_vec.x	*= ( 1.0f / 3.0f);
                center_vec.y	*= ( 1.0f / 3.0f);
                center_vec.z	*= ( 1.0f / 3.0f);

                DestPosition = center_vec;
            }
#else
            if( empty_tile == true )
            {
                DestPosition = g_pSunTerrain->x_pPathFinder->GetNearestPointToTarget(tile_number, DestPosition);
            }
#endif //_NA_008721_20160308_AUTOMOVE_TO_PORTAL_BUG_FIX

            return true;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
int GameFunc::GetNearestTile(const WzVector& pos, WORD restrict_tile_attribute)
{
    WORD attribute = 0;
    int tile_no = g_pSunTerrain->x_pPathFinder->GetTileToStand(pos, NULL, -1.0f, -1.0f, &attribute);
    if (tile_no >= 0)
    {
        if ((attribute & restrict_tile_attribute) == FALSE)
        {
            return tile_no;
        }
    }

    float backup_length = 10000000.0f;

    tile_no = -1;

    int tile_num = g_pSunTerrain->x_pPathFinder->GetNumPathTile();
    for (int i = 0; i < tile_num; ++i)
    {
        if (g_pSunTerrain->x_pPathFinder->CheckAttribute(i, restrict_tile_attribute) == TRUE)
        {
            continue;
        }

        WzVector v = g_pSunTerrain->x_pPathFinder->GetNearestPointToTarget(i, pos);
        WzVector gab = v - pos;
        float length = VectorLength(&gab);
        if (length < backup_length)
        {
            tile_no = i;
            backup_length = length;
        }
    }

    return tile_no;
}
//------------------------------------------------------------------------------
MapObject* GameFunc::GetWaypoint()
{
    Object_Map::iterator begin = g_ObjectManager.GetBegin();
    Object_Map::iterator end = g_ObjectManager.GetEnd();
    for (; begin != end; ++ begin)
    {
        Object* object = begin->second;
        if ((object == NULL) || (object->IsKindOfObject(MAP_OBJECT) == FALSE))
        {
            continue;
        }

        MapObject* map_object = static_cast<MapObject*>(object);

        if (map_object->IsWayPointObj() == TRUE)
        {
            return map_object;
        }
    }

    return NULL;
}

NPC* GameFunc::GetNearestShopNpcByItem(SLOTCODE item_code)
{
    std::list<NPC*> npc_list;

    {
        // 상점 npc 리스트 만듬
        Object_Map::iterator begin = g_ObjectManager.GetBegin();
        Object_Map::iterator end = g_ObjectManager.GetEnd();
        for (; begin != end; ++ begin)
        {
            Object* object = begin->second;
            if ((object == NULL) || (object->IsKindOfObject(NPC_OBJECT) == FALSE))
            {
                continue;
            }

            EXTRA_NPCINFO* extra_info = static_cast<NPC*>(object)->GetExtraInfo();
            if (extra_info->m_eNPCTYPE != eNPC_TYPE_STORE)
            {
                continue;
            }

            if ( ShopInfoParser::Instance()->HasSellItemByItemCode( extra_info->m_NPCCODE, item_code ) == true )
            {
                npc_list.push_back(static_cast<NPC*>(object));
            }
        }
    }


    //가장 가까운 npc
    NPC* nearest_npc = NULL;

    {
        std::list<NPC*>::iterator begin = npc_list.begin();
        std::list<NPC*>::iterator end = npc_list.end();

        for (; begin != end; ++ begin)
        {
            if (nearest_npc == NULL)
            {
                nearest_npc = *begin;
            }
            else
            {
                WzVector gab1 = nearest_npc->GetPosition() - g_pHero->GetPosition();
                WzVector gab2 = (*begin)->GetPosition() - g_pHero->GetPosition();

                if (VectorLength(&gab1) > VectorLength(&gab2))
                {
                    nearest_npc = *begin;
                }
            }
        }
    }

    return nearest_npc;

}

//------------------------------------------------------------------------------
NPC* GameFunc::GetNearestShopNpc(eITEMTYPE sell_item_type)
{
    std::list<NPC*> npc_list;
    npc_list.clear();

    {
        // 상점 npc 리스트 만듬
        Object_Map::iterator begin = g_ObjectManager.GetBegin();
        Object_Map::iterator end = g_ObjectManager.GetEnd();
        for (; begin != end; ++ begin)
        {
            Object* object = begin->second;
            if ((object == NULL) || (object->IsKindOfObject(NPC_OBJECT) == FALSE))
            {
                continue;
            }

            EXTRA_NPCINFO* extra_info = static_cast<NPC*>(object)->GetExtraInfo();
            if (extra_info->m_eNPCTYPE != eNPC_TYPE_STORE)
            {
                continue;
            }

            if ( ShopInfoParser::Instance()->HasSellItemByItemType( extra_info->m_NPCCODE, sell_item_type ) == true )
            {
                npc_list.push_back(static_cast<NPC*>(object));
            }
        }
    }

    //가장 가까운 npc
    NPC* nearest_npc = NULL;

    {
        std::list<NPC*>::iterator begin = npc_list.begin();
        std::list<NPC*>::iterator end = npc_list.end();

        for (; begin != end; ++ begin)
        {
            if (nearest_npc == NULL)
            {
                nearest_npc = *begin;
            }
            else
            {
                WzVector gab1 = nearest_npc->GetPosition() - g_pHero->GetPosition();
                WzVector gab2 = (*begin)->GetPosition() - g_pHero->GetPosition();

                if (VectorLength(&gab1) > VectorLength(&gab2))
                {
                    nearest_npc = *begin;
                }
            }
        }
    }

    return nearest_npc;
}

//------------------------------------------------------------------------------
int GameFunc::GetMonstetListTargetPlayer(OUT vector<DWORD>& monster_vector, DWORD object_key)
{
    Object_Map::iterator begin = g_ObjectManager.GetBegin();
    Object_Map::iterator end = g_ObjectManager.GetEnd();
    for (; begin != end; ++ begin)
    {
        Object* object = begin->second;
        if ((object == NULL) || (object->IsKindOfObject(MONSTER_OBJECT) == FALSE))
        {
            continue;
        }

        Monster* monster = static_cast<Monster*>(object);

        if (monster->GetTargetID() == object_key)
        {
            monster_vector.push_back(monster->GetObjectKey());
        }
    }
    
    return static_cast<int>(monster_vector.size());
}
//------------------------------------------------------------------------------ 
int GameFunc::GetInterpolationValue(int small_value, int big_value, float ratio)
{
    return (int)(small_value + ((big_value - small_value) * ratio));
}
//------------------------------------------------------------------------------ 
float GameFunc::GetInterpolationValue(float small_value, float big_value, float ratio)
{
    return (small_value + ((big_value - small_value) * ratio));
}
//------------------------------------------------------------------------------ 
RECT GameFunc::GetInterpolationValue(RECT& small_value, RECT& big_value, float ratio)
{
    RECT v;
    v.left = static_cast<LONG>(small_value.left + ((big_value.left - small_value.left) * ratio));
    v.top = static_cast<LONG>(small_value.top + ((big_value.top - small_value.top) * ratio));
    v.right = static_cast<LONG>(small_value.right + ((big_value.right - small_value.right) * ratio));
    v.bottom = static_cast<LONG>(small_value.bottom + ((big_value.bottom - small_value.bottom) * ratio));

    return v;
}
//------------------------------------------------------------------------------ 
bool GameFunc::IsPossibleStand(WzVector& pos, float height, WORD restrict_tile_attribute)
{
    WzRay ray;
    ray.m_wvOrigin = pos;
    ray.m_wvOrigin.z += height;
    ray.m_wvDirection.x =  0.0f;
    ray.m_wvDirection.y =  0.0f;
    ray.m_wvDirection.z = -1.0f;

    bool is_find = true;
    float fT;
    if (g_pSunTerrain->PickTerrainMesh(&ray, &fT ,TRUE, TMGO_NO_COLLISION) < 0)
    {
        ray.m_wvOrigin.z += height;
        ray.m_wvDirection.x = 0.0f;
        ray.m_wvDirection.y = 0.0f;
        ray.m_wvDirection.z = 1.0f;

        if(g_pSunTerrain->PickTerrainMesh(&ray, &fT , FALSE,TMGO_NO_COLLISION) < 0)
        {
            is_find = false;
        }
    }

    if (is_find)
    {
        WzVector dest_pos = ray.m_wvOrigin + ray.m_wvDirection * fT;
        g_pPathExplorer->Stop();

        g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &dest_pos, -1);

        int tile_no = g_pPathExplorer->GetTile();
        if ((tile_no >= 0) && 
            ((g_pSunTerrain->x_pPathFinder->GetAttribute(tile_no) & restrict_tile_attribute) == false))
        {
            return true;
        }
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool GameFunc::IsIntersect(WzVector& scr, WzVector& dst)
{
    const float HEIGHT = 2.8f;

    WzRay ray;
    WzVector vDistance = dst - scr;
    float fDistance = VectorLength(&vDistance);
    ray.m_wvOrigin = scr;
    ray.m_wvOrigin.z +=0.2f;
    ray.m_wvDirection = dst - ray.m_wvOrigin;

    VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);

    if (g_pMap->IntersectTree_Ray_Bv(&ray,fDistance) == TRUE)
    {
        return true;
    }

    WzVector vObjPosition;

    if (g_pMap->IntersectMapObject_Ray_Bv(&ray, fDistance, HEIGHT, vObjPosition) == TRUE)
    {
        return true;
    }

    float fT;

    WzVector tempDest = dst;
    WzVector tempSrc = scr;

    tempSrc.z +=2.5f;
    tempDest.z = tempSrc.z;

    vDistance = tempDest - tempSrc;
    fDistance = VectorLength(&vDistance);
    ray.m_wvOrigin = tempSrc;
    ray.m_wvDirection = tempDest - ray.m_wvOrigin;
    VectorNormalize(&ray.m_wvDirection,&ray.m_wvDirection);

    //Todo:뒷면 지형 체크 옵션 바꾸기 
    int iTerrainMeshFocussing = g_pSunTerrain->PickTerrainMesh(&ray, &fT, FALSE,TMGO_NO_COLLISION);
    if (iTerrainMeshFocussing >= 0 && fT <= fDistance)
    {
        //지형과 충돌
        return true;
    }

    return false;
}

//------------------------------------------------------------------------------ 
BodyPart GameFunc::EquipToBodyPart(eEQUIPCONTAINER_POS equip_pos/*item_info->m_wEqPos*/)
{
    BodyPart bodypart_index = BP_COUNT;
    switch (equip_pos)
    {
    case eEQUIPCONTAINER_WEAPON: bodypart_index = BP_WEAPON;break;
    case eEQUIPCONTAINER_BOOTS: bodypart_index = BP_BOOTS;break;
    case eEQUIPCONTAINER_PANTS: bodypart_index = BP_PANTS;break;
    case eEQUIPCONTAINER_ARMOR: bodypart_index = BP_ARMOR;break;
    case eEQUIPCONTAINER_HELMET:bodypart_index = BP_HELM; break;
    case eEQUIPCONTAINER_GLOVE: bodypart_index = BP_GLOVE; break;  
    case eEQUIPCONTAINER_SACCESSORY1: bodypart_index = BP_SPECIAL_ACCESSORY1; break;  
    case eEQUIPCONTAINER_SACCESSORY2: bodypart_index = BP_SPECIAL_ACCESSORY2; break;  
    case eEQUIPCONTAINER_SACCESSORY3: bodypart_index = BP_SPECIAL_ACCESSORY3; break;  
    case eEQUIPCONTAINER_PROTECTOR:
    case eEQUIPCONTAINER_BELT:
    case eEQUIPCONTAINER_SHIRTS:
    case eEQUIPCONTAINER_RING1:
    case eEQUIPCONTAINER_RING2:
    case eEQUIPCONTAINER_NECKLACE:
    default: 
        break; //return false;
    }

    return bodypart_index;
}
//------------------------------------------------------------------------------ 
int GameFunc::GetItemGradeStringCode(TCHAR *item_name, WORD item_level, ItemSubTypeDetail item_subtype)
{
    int string_code = 0;

    switch (item_subtype.value)
    {
    case ItemSubTypeDetail::kSuperJapan:
        string_code = 30424; // 30424 얼티메이트 (일본 최강)
        break;
    case ItemSubTypeDetail::kElite1_0st:
        string_code = 30423; // 30423 엘리트 1차
        break;
    case ItemSubTypeDetail::kCurruptTemple:
        string_code = 30428; // 30428 저주받은 엘리트 1차
        break;
    case ItemSubTypeDetail::kElite1_5st:
        string_code = 30425; // 30425 엘리트 1.5차
        break;
    case ItemSubTypeDetail::kElite2_0st:
        string_code = 30426; // 30426 엘리트 2차
        break;
	case ItemSubTypeDetail::kElite2_5st:
		string_code = 30434; // - 엘리트 2.5차
		break;
	case ItemSubTypeDetail::kElite3_0st:
		string_code = 30435; // - 엘리트 3차
		break;
    case ItemSubTypeDetail::kElite3_5st:
        string_code = 30451; // - 엘리트 3.5차
        break;
	case ItemSubTypeDetail::kElite4_0st:
		string_code = 30452; // - 엘리트 4차
		break;
    case ItemSubTypeDetail::kUnique1_0st:
        string_code = 30429; // - 유니크 1차
        break;
    case ItemSubTypeDetail::kUnique1_5st:
        string_code = 30453; // - 유니크 1.5차
        break;
    case ItemSubTypeDetail::kUnique2_0st:
        string_code = 30430; // - 유니크 2차
        break;
    case ItemSubTypeDetail::kUnique2_5st:
        string_code = 30454; // - 유니크 2.5차
        break;
	case ItemSubTypeDetail::kFigter_Elite2_0st:
		string_code = 30433; // - 투사 엘리트 2차
		break;
	case ItemSubTypeDetail::kFigter_Elite3_0st:
		string_code = 30436; // - 투사 엘리트 3차
		break;

    default:   // kItemLevelNormal
        {
            TCHAR grade[INTERFACE_STRING_LENGTH] ={0,};
            // 30422 %d 등급, (item_level)
            g_InterfaceManager.GetInterfaceString(30422, grade, 127);
            _snprintf(item_name, INTERFACE_STRING_LENGTH-1, grade, item_level);
        }
        break;
    }
    return string_code;
}


////------------------------------------------------------------------------------ 
// 남과장님 요청한 예제 함수
////------------------------------------------------------------------------------ 
//------------------------------------------------------------------------------ 
bool GameFunc::Tile_CrossCheck(WzVector *pwvFrom, WzVector *pwvTo, WzVector *pwvEdge1, WzVector *pwvEdge2, float *pfDist, WzVector* pwvCross)
{
    Wz2DLine w2l, w2lEdge;
    As2D_GetLine(&w2l, pwvFrom, pwvTo);
    As2D_GetLine(&w2lEdge, pwvEdge1, pwvEdge2);

    // 교차점 얻기
    WzVector wvCross;
    As2D_GetCrossPointOfTwoLine(&wvCross, &w2l, &w2lEdge);

    // 두 Edge 사이를 통과 하고 있는가?
    float fVectorDist[2];
    fVectorDist[0] = As2D_Distance_Line_Vertex(&w2l, *pwvEdge1);
    fVectorDist[1] = As2D_Distance_Line_Vertex(&w2l, *pwvEdge2);
    fVectorDist[0] = (fabs(fVectorDist[0])>0.0001f)?fVectorDist[0]:0.0f;
    fVectorDist[1] = (fabs(fVectorDist[1])>0.0001f)?fVectorDist[1]:0.0f;

    if (fabs(fVectorDist[0]) > WZ_EPSILON && fabs(fVectorDist[1]) > WZ_EPSILON && 0.0f < (fVectorDist[0] * fVectorDist[1]))
    {
        //두 Edge가 직선의 Nomal 방향(또는 Nomal 반대 방향)에 같이 위치 하고 있어 교차점이 없다.
        return false;
    }

    fVectorDist[0] = As2D_Distance_Line_Vertex(&w2lEdge, *pwvTo);
    if (0.0f < fVectorDist[0])
    {
        //pwvFrom 과 pwvTo가 모두 w2lEdge의 노말 방향에 위치 하고 있어 교차점이 없다.
        return false;
    }

    WzVector wvDIst = wvCross - *pwvFrom;
    *pfDist = VectorLength(&wvDIst);

    if (pwvCross)
    {
        *pwvCross = wvCross;
    }

    return true;
}
//------------------------------------------------------------------------------
//! Returns:   bool     true : 삼각형 접선에 충돌
//! Qualifier: 진행방향에 따른 삼각형 선분 방향 구하는 함수
//! Parameter: WzVector * triangle_vector_list : 삼각형 3점 배열 3개짜리
//! Parameter: WzVector & from : 캐릭터 이동 시작 위치
//! Parameter: WzVector & to : 이후 캐릭터 이동 될 위치
//! Parameter: OUT WzVector & out_dir : 선분 방향
//! Parameter: OUT float & dot_product_value : dot product 한 값.
//------------------------------------------------------------------------------
bool GameFunc::GetTangentDirection(WzVector* triangle_vector_list, 
                                   WzVector& from, 
                                   WzVector& to, 
                                   OUT WzVector& out_dir, 
                                   OUT float& dot_product_value)
{
    if (triangle_vector_list == NULL)
    {
        return false;
    }

    WzVector start_pos, end_pos, cross_pos;
    float cross_dist = 1000000000.0f;


    float temp_dist = 0.0f;
    WzVector temp_cross;

    bool is_col = false;
    for(int i = 0; i < 3; ++i)
    {
        WzVector line0 = triangle_vector_list[i];
        WzVector line1 = triangle_vector_list[( i + 1) % 3];

        if (Tile_CrossCheck(&from, &to, &line0, &line1, &temp_dist, &temp_cross) == true)
        {
            if ((is_col == false) || (temp_dist < cross_dist))
            {
                start_pos = line0;
                end_pos = line1;
                cross_pos = temp_cross;
                cross_dist = temp_dist;

                is_col = true;
            }
        }
    }

    if (is_col == true)
    {
        WzVector move_dir = to - from;
        WzVector line_dir = end_pos - start_pos;

        WzVector player_dir;
        WzVector tangent_dir;
        VectorNormalize(&player_dir, &move_dir);
        VectorNormalize(&tangent_dir, &line_dir);

        dot_product_value = VectorDotProduct(&player_dir, &tangent_dir);
        if (dot_product_value > 0 && dot_product_value <= 1)
        {
            //예각이니깐 
            out_dir = line_dir;
        }
        else if (dot_product_value < 0 && dot_product_value >= -1)
        {
            //둔각이니깐
            out_dir = -line_dir;
        }
        else
        {
            //나머진 90.
            out_dir.x = 0;
            out_dir.y = 0;
            out_dir.z = 0;
        }

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------ 
void GameFunc::ExamplePlayerNextDirection(Player* player, DWORD elapse_tick)
{ 
    //! 초기 값
    float fMoveSpeed = player->GetMoveSpeed() * (float)elapse_tick;
    WzVector offset = player->GetDirection() *  fMoveSpeed;
    WzVector from = player->GetPosition();
    WzVector to = from + offset;

    //------------------------------------------------------------------------------ 
    //! 타일의 3점을 얻어 온다.
    WzVector out_vec[3];
    g_pSunTerrain->x_pPathFinder->GetTileVertexInfo(out_vec, player->GetPathExplorer()->GetTile());

    //------------------------------------------------------------------------------ 
    //! 캐릭터 이동 방향으로 걸린 삼각형의 선분을 구해옴
    WzVector move_dir;
    float dot_product_value = 0.0f;
    if (GameFunc::GetTangentDirection(out_vec, from, to, OUT move_dir, OUT dot_product_value) == true)
    {
        //move_dir
        //확인 사살(실제 값이 맞는지 확인
        WzVector temp_move_dir = to - from;

        WzVector player_dir;
        WzVector tangent_dir;
        VectorNormalize(&player_dir, &temp_move_dir);
        VectorNormalize(&tangent_dir, &move_dir);
        float temp = VectorDotProduct(&player_dir, &tangent_dir);
    }
}
//------------------------------------------------------------------------------ 
Player*  GameFunc::MakeBattleZonePlayer(PLAYER_RENDER_INFO_EX& render_info_ex, 
                                     ETC_STATE_TOTAL_INFO* state_total_info,
                                     VENDOR_RENDER_INFO* vendor_render_info,
                                     EQUIP_ITEM_BASEINFO* equip_item_info,
                                     sMSG_RENDER_EFFECT_ITEM_LIST* effect_item_info)
{
    DWORD object_key = g_ObjectManager.GenerateKeyByBattleZonePlayer(PLAYER_OBJECT);
    if (object_key <= 0)
    {
        return NULL;
    }
    if (g_ObjectManager.GetObject(object_key) != NULL)
    {
        return NULL;
    }

    render_info_ex.hp = 1;
    render_info_ex.is_riding = 0;
    render_info_ex.spread_wings = 0;
    render_info_ex.char_condition = 0;

    ClassCodeEx class_code_ex(render_info_ex.char_class, render_info_ex.gender);
    Player* player = (Player*)g_ObjectManager.Add(object_key, PLAYER_OBJECT, class_code_ex.class_code, FALSE);
    if (player == NULL)
    {
        return NULL;
    }

    player->HideHelmet(render_info_ex.helmet != 0, FALSE);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    player->HideWing(render_info_ex.wing != 0, FALSE);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    player->HideCostume(render_info_ex.costume);
#endif
    player->SetRandomAngle();
    player->SetPlayerInfo(&render_info_ex);
    player->SetCurrentAttackStyle(render_info_ex.selected_style_code);
    player->SetHeightVariation(2);


    for( int a = 0; a < state_total_info->m_Count; ++a )
    {
        ETC_STATE_INFO& state_info = state_total_info->m_Slot[a];
        SKILL_EFFECT* skill_effect = SKILL_EFFECT::CreateStatusEffect(state_info.m_wStateCode, state_info.m_iLeavedTime);
        
        player->AddSkillEffect(skill_effect);
    }

    if (vendor_render_info != NULL)
    {
        // 벤드 정보
        player->SetMarketTitle(vendor_render_info->m_tszVendorTitle, vendor_render_info->m_byTitleLength);
    }

    if (equip_item_info != NULL)
    {
        //이큅정보
        EQUIP_ITEM_BASEINFO::data_type* item_slot = (EQUIP_ITEM_BASEINFO::data_type*)((BYTE*)(equip_item_info) + sizeof(EQUIP_ITEM_BASEINFO));
        player->SetEquipItemBaseInfo(equip_item_info->m_Count, item_slot);
    }

    RESOURCECODE resource_code = 0;
    DWORD skill_code = 0; 
    eTRANSFORM_TYPE transform_type = eTRANSFORM_TYPE_NONE;
    if (GameFunc::IsTransform(render_info_ex, resource_code, skill_code, transform_type))
    {
        player->GetTransformData().SetTransFormType(transform_type);
        player->GetTransformData().SetTransFormResourceCode(resource_code);
        player->SetTransSkillCode(skill_code);
        player->ForceTransFrom();
    }
    
    if (effect_item_info != NULL)
    {
        //이펙트 정보
        player->RefreshRenderEffectItem(player, &(effect_item_info->m_ItemCodes[0]), effect_item_info->m_ItemCount);
    }

    player->RefreshAppearRender();

    // 기본 에니메이션
    player->SetColor(WzColor_RGBA(255,255,255,255));
    player->SetFreeMove(TRUE); 
    player->ProcessAnimation(0); 
    player->SetAnimation("N002", TRUE);

    return player;
}
//------------------------------------------------------------------------------ 
Player* GameFunc::MakePlayer(const DWORD object_key, Player* base_player, bool is_with_equip)
{
    if (object_key <= 0 || base_player == NULL)
    {
        return NULL;
    }
    if (g_ObjectManager.GetObject(object_key) != NULL)
    {
        return false;
    }

    Player* player = (Player*)g_ObjectManager.Add(object_key, PLAYER_OBJECT, base_player->GetClassCode(), FALSE);
    if (player != NULL)
    {
        CLIENT_CHARACTER_PART character_part;
        ZeroMemory(&character_part,sizeof(character_part));
        character_part.m_byClass = base_player->GetClass();
        character_part.m_bySlot = 0;
        character_part.m_byFace = base_player->GetPartVariation(PLAYER_VARIATION_FACE);
        character_part.m_byHair = base_player->GetPartVariation(PLAYER_VARIATION_HAIR);

#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        player->SetHideHelmet(base_player->IsHideHelmet() );
        player->SetHideWing(base_player->IsHideWing() );
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        player->SetHideCostume(base_player->isHideCostume());
#endif
        player->SetClientChar(character_part); 
        player->SetAnimation("N002", TRUE);
        player->SetColor(WzColor_RGBA(255,255,255,255));
        player->SetFreeMove(TRUE); 
        player->ProcessAnimation(0); 
        player->SetName(base_player->GetName());
        player->SetLevel(base_player->GetLevel());
        player->SetHeightVariation(2);
#ifdef _NA_004035_20120227_COSTUME_ITEM
        player->SetCostumeItemCode(base_player->GetCostumeItemCode());
#endif//_NA_004035_20120227_COSTUME_ITEM
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
		player->SetDecorateCostumeItemCode(base_player->GetDecorateCostumeItemCode());
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

        if (is_with_equip)
        {
            EQUIP_ITEM_INFO out;
            base_player->GetEquipItemTotalInfoByCheckValidItem(OUT out);
            player->SetEquipItemTotalInfo(out);
        }
        player->RefreshAppearRender();
    }

    return player;
}
//------------------------------------------------------------------------------ 
bool GameFunc::IsTransform(PLAYER_RENDER_INFO_EX& render_info_ex,
                           OUT RESOURCECODE& resource_code,
                           OUT DWORD& skill_code, 
                           OUT eTRANSFORM_TYPE& transform_type)
{
    for (int j = 0; j <  render_info_ex.state_total_info.m_Count; ++j)
    {
        STATE_INFO& state_info = render_info_ex.state_total_info.m_Slot[j];

        SkillScriptInfo* skill_script_Info = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)state_info.m_SkillCode);
        if (skill_script_Info == NULL)
        {
            continue;
        }

        BASE_ABILITYINFO* ability_info = skill_script_Info->GetAbilityInfo(eABILITY_DRAGON_TRANSFORMATION1);
        if (ability_info != NULL)
        {
            skill_code=state_info.m_SkillCode;
            resource_code = ability_info->m_iOption1;
            transform_type= static_cast<eTRANSFORM_TYPE>(ability_info->m_iOption2);
            return true;
        }
    }
    return false;
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
StatuePlayer* GameFunc::MakeStatuePlayer( const WzVector& position, const WzVector& direction, const float& scale_value, StatueInfo statue_info )
{
	// index 범위 : 0 ~ 11
	if (statue_info.statue_index < 0 || statue_info.statue_index > 11)
	{
		return NULL;
	}

	CLIENT_CHARACTER_PART client_character_part;
	ZeroMemory(&client_character_part, sizeof(client_character_part));
	client_character_part = statue_info.client_character_part;

	StatuePlayer* statue_player = static_cast<StatuePlayer*>(g_ObjectManager.Add(STATUE_OBJECT_CHARACTER_KEY1 + statue_info.statue_index,
		STATUE_PLAYER_OBJECT,
		client_character_part.m_byClass,
		FALSE));

	statue_player->SetClass((eCHAR_TYPE)client_character_part.m_byClass);
	statue_player->SetGender((GenderType::Value)client_character_part.gender);
	statue_player->RefreshApperBronzePlayer();

	// 외형세팅	
	statue_player->SetPartVariation(PLAYER_VARIATION_HAIR, client_character_part.m_byHair);
	statue_player->SetPartVariation(PLAYER_VARIATION_FACE, client_character_part.m_byFace);

	statue_player->SetClientChar(client_character_part);
	statue_player->HideHelmet(false, FALSE);
	statue_player->SetShow(TRUE);
	statue_player->SetHideWing(TRUE);

	EquipmentContainer* equipment_container = statue_player->GetEquipmentContainer();
	EquipmentContainer* statue_equip_container = statue_player->get_statue_equip_container_();

	SCItemSlot* item_slot;
	SCItemSlot copy_slot;
	for ( int i = EquipmentContainer::EQUIP_WEAPON1; i <= EquipmentContainer::EQUIP_SHIRTS; ++i )
	{
		item_slot = equipment_container->GetItemSlot(i);
		if ( item_slot )
		{
			copy_slot.Copy(*item_slot);
			copy_slot.SetEnchant(0);
			statue_equip_container->JustInsertSlot(i, copy_slot);
		}
	}

	statue_player->set_equip_container_(statue_player->get_statue_equip_container_());
    statue_player->RefreshAppearRender();

	statue_player->SetScale(scale_value);
	statue_player->SetVisiblePos(position.x, position.y, position.z);
	statue_player->SetPosition(position);
	statue_player->SetDirection(direction);

	statue_player->SetName(client_character_part.m_tszCharName);
	statue_player->SetShowName(TRUE);

	StatueGuildInfo statue_guild_info;
	ZeroMemory(&statue_guild_info, sizeof(statue_guild_info));
	StrNCpy(statue_guild_info.guild_name, statue_info.guild_name, MAX_GUILDNAME_LENGTH-1);
	statue_guild_info.guild_mark_pattern_idx = statue_info.pattern_mark_index;
	statue_guild_info.guild_mark_background_idx = statue_info.background_mark_index;
	statue_player->set_statue_guild_info(statue_guild_info);

	// 기본자세
	WzID animationID = 0;

	char animation_code[5];
	// 무기에따라서 기본자세가 바뀝니다.

	ZeroMemory(animation_code,sizeof(animation_code));

	animation_code[0] = 'N';
	animation_code[1] = '0';
	animation_code[2] = '3';
	animation_code[3] = '0';
	animation_code[1] = statue_player->GetWeaponAnimationCode();

	statue_player->set_stand_animation_id_(animation_code);

	statue_player->SetItemScore((DWORD)statue_info.total_equip_score);

	PLAYER_ACTION action;
	ZeroMemory(&action, sizeof(action));
	action.ActionID = ACTION_STATUE_STAND;
	statue_player->DoAction(&action);

	return statue_player;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

int GameFunc::GetDayOfTheWeek(int year, int month, int day)
{
    // return -> 0:일요일 1:월요일, 2:화요일, .. , 6:토요일, -1:날짜x
    if (!(year > 0 && month > 0 && month < 13 && day > 0))
    {
        return -1;
    }

    int last_day = GetLastDayOfTheMonth(year, month);
    
    if (last_day < 0 || day > last_day)
    {
        return -1;
    }

    // 요일 구하기 (제라의공식)
    if (month < 3)
    {
        --year;
        month += 12;
    }

    return (21*(year/100)/4 + 5*(year%100)/4 + 26*(month+1)/10 + day - 1) % 7;
}

int GameFunc::GetLastDayOfTheMonth(int year, int month)
{
    if (month > 12)
    {
        return -1;
    }

    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // 윤달 계산
    if (month == 2 && (!(year%4) && (year%100) || !(year%400)))
    {
        return 29;
    }
    
    return days[month-1];

}