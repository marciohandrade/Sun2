//------------------------------------------------------------------------------
//  Hero.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "hero.h"
#include "EquipmentContainer.h"
#include "HeroEquipmentContainer.h"
#include "SummonSkillContainer.h"
#include "SCSlotContainer.h"
#include "InterfaceManager.h"
#include "QuickContainer.h"
#include "StyleContainer.h"
#include "InventoryDialog.h"
#include "RadarInfoParser.h"
#include "RadarDialog.h"
#include "RadarPlusDialog.h"
#include "RadarFullDialog.h"
#include "ChatDialog.h"
#include "ObjectManager.h"
#include "ShopDialog.h"
#include "ItemManager.h"
#include "HeroActionInput.h"
#include <SkillInfoParser.h>
#include <PlayerAttributes.h>
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDef.h"
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include <ItemAttrCalculator.h>
#include "EquipmentContainer.h"
#include "ItemInfoParser.h"
#include <SkillSlot.h>
#include <SCItemSlot.h>
#include "hotkeymanager.h"
#include "StyleInfoParser.h"
#include "SkillAttrCalculator.h"
#include "SkillMain.h"
#include "uiSkillMan/uiNewSkillMan.h"
#include "uiPerkDialog/uiPerkDialog.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "Char_SkillDialog.h"
#include "QuickSlot.h"
#include "StyleSlot.h"

#ifdef _NA_000000_20130114_RENEWER_UI
#include "HeroGaugeDialog2.h"
#include "uiHelmetShow.h"
#else
#include "HeroGaugeDialog.h"
#endif

#include "EventInventoryDialog.h"
#include "SoundEffect.h"
#include "QuestManager_Concrete.h"
#include "Interface/uiPartyMan/uiPartyMan.h"
#include "ProjectileManagerEx.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "GlobalFunc.h"
#include "AppearanceManager.h"
#include "uiChaoSystemMan/uiChaoSystemMan.h"
#include "BitStream.hpp"
#include "Camera.h"
#include "Monster.h"
#include "CastLeaveFieldDialog.h"
#include "Map.h"
#include "GameOptionParser.h"
#include "ssqScene.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "GuildSystem/uiGuildWareHouse.h"
#else
    #include "uiGuildWareHouse/uiGuildWareHouse.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "Pet.h"
#include "PetInfoParser.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiVendorMan/uiVendorMan_def.h"
#include "uiRadarMan/uiRadarMan.h"
#include "uiPetMan/uiPetMan.h"
#include "uiPetNameSetting/uiPetNameSetting.h"
#include "OptionGameDialog.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "PKScriptParser.h"
#include "Rider.h"
#include "RiderInventoryDialog.h"
#include "DominationInfoParser.h"

#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiDominationZoneMarkDlg/uiDominationZoneMarkDlg.h"
#include "ExtendInventoryDialog.h"

#ifdef _YMS_AUTOBOT_GAME
    #include "Interface/uiAutobot/uiAutobotConditionBattle.h"
#endif //_YMS_AUTOBOT_GAME

#include "PacketRequestManager.h"
#include "uiSkillTree/uiSkillTree.h"

#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"

#include "KBMoveCheckManagerInClient.h"
#include "HeroAttributes.h"
#include "HeroData.h"
#include "CooltimeManager.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildSystem/GuildSystemData.h"
#include "GuildFacilityInfoParser.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#include "uiFPGaugeMan/uiFPGaugeMan.h"
#include "uiFPGaugeDialog/uiFPGaugeDialog.h"
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

extern DWORD g_CurTime;

//------------------------------------------------------------------------------
/**
*/
Hero::Hero(void)
	:	m_pInventoryContainer ( NULL ),
		m_pQuickContainer ( NULL ),
		m_pStyleContainer(NULL),
		m_pSummonContainer(NULL),
        m_pHeroEquipContainer(NULL),
		m_dwPacketStatus( 0 )
{
	
	m_pPlayerAttriburtes	= NULL;
	
	m_eTriggerState = TRIGGER_STATE_LEAVE_AREA;

	InitTriggerState();
	InitShortcutState();

	m_bShowFullMap = false;

	m_iHeroTurnState = eHERO_NOT_TURN;

	m_bAccountState = FALSE;
	m_bIsScreenShotMode = FALSE;
	m_iCanNotMoveTime=0;
	m_bForceAttack=FALSE;
	m_bCompleteTransForm=FALSE;
	//m_bHero=TRUE;

   // // 쿨타임 타이머 해쉬 테이블 생성
   //COOLTIME_MAP* pCoolTimeMap = SkillInfoParser::Instance()->GetSkillCoolTimeMap();
   //this->GetSCCharacter().InitializeHashTable((DWORD)pCoolTimeMap->size());

   SetLockSkill(FALSE);

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
   m_ePCBangStatus = CHANGE_PCROOM_STATUS::eStatus_None;
#endif

   memset(&m_RiderInfo,0,sizeof(m_RiderInfo));

   for (int i = 0; i < CRYSTAL_WARP_OBJ_MAX_CNT; ++i)
   {
	   m_dwCrystalWarpObjKeys[i] = 0;
   }
   
   //CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD
   FixedPortalState.portal_key = 0;
   FixedPortalState.open_state = MSG_CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD::eOpenState_None;
   FixedPortalState.portal_lifetime = 0;

   //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
   KBMove.ang = 0;
   SetVector(&KBMove.pos, 0, 0, 0);
   KBMove.sent_tick = 0;
   KBMove.last_sent_move_flag = 0;
   KBMove.is_first_kbmove = false;
   KBMove.debug_kbmove_skip_count = 0;
   KBMove.debug_kbmove_gab = 0.0f;
   KBMove.debug_move_speed = 0.0f;

   //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
   PacketNak.delay_tick = 0;
   PacketNak.is_recv_nak = false;


   is_waiting_invite_group_ = false;

}


//------------------------------------------------------------------------------
/**
*/
Hero::~Hero(void)
{
}


BOOL Hero::SendUnTransFormPacket()
{
	if(IsTransForm()&&!BIT_CHECK(m_dwPacketStatus, PACKET_STATUS__END_FORMATION ))
	{
		if( GENERALPARAM->IsNet()==FALSE)
		{
			RemoveSkillEffectWithStatus(eCHAR_STATE_TRANSFORMATION);
			SetTransSkillCode(0);

			PLAYER_ACTION action;
			ZeroMemory(&action,sizeof(action));
			action.ActionID = ACTION_TRANSFORM;	
			action.TRANSFORM.appearKind=APPEARANCEINFO::PLAYER;
			action.TRANSFORM.rcCode=0;
			action.TRANSFORM.transFormProcess=TRANSFORM_PROCESS_UNTRANSFORM;
			action.TRANSFORM.transFormType=eTRANSFORM_TYPE_NONE;

			g_HeroInput.SetTransFormAction(&action);
			UpdateSkillAttr();	
		}


		if( GENERALPARAM->IsNet())
			BIT_ADD(m_dwPacketStatus, PACKET_STATUS__END_FORMATION );

		MSG_CG_STATUS_END_DRAGON_FORMATION_SYN packet;
		packet.m_byCategory=CG_STATUS;
		packet.m_byProtocol=CG_STATUS_END_DRAGON_FORMATION_SYN;
		return SendPacket( &packet, sizeof(packet) );

	}

	return FALSE;
}

void Hero::HeroTransForm()
{
	g_InterfaceManager.UpdateMyHeadTexture();

	if(m_pStyleContainer)
		m_pStyleContainer->ShowWindow(FALSE); 


	if (m_pHeroEquipContainer)
	{
		m_pHeroEquipContainer->TransForm();
	}

    xGetSkillData()->BlockSkill(GetTransformData().GetTransFormType());

    if( uiSkillMan* skill_main = static_cast<uiSkillMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SKILL)) )
    {
        skill_main->skill_tree()->RefreshUI();
    }

    xGetSkillData()->ResetTreeData();

	if (MouseHandler::Instance()->IsExistItemAtHand())
	{
		MouseHandler::Instance()->RollbackItemAtHand();
	}

	m_bCompleteTransForm=TRUE;
}

void Hero::TransForm()
{
	Player::TransForm();

	if(GetTransformData().GetTransFormProcessType()==TRANSFORM_PROCESS_TRANSFORM)
	{
		HeroTransForm();
	}

    UpdateSkillAttr();
}



//------------------------------------------------------------------------------
/**
*/
void Hero::UnTransForm()
{
	Player::UnTransForm();

	if(GetTransformData().GetTransFormProcessType()==TRANSFORM_PROCESS_UNTRANSFORM)
	{
		g_InterfaceManager.UpdateMyHeadTexture();
		StyleContainer *pStyleContainer= (StyleContainer * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_STYLE_CONTAINER);

		if(m_pStyleContainer)
		{
			if(GetClass()==eCHAR_BERSERKER||GetClass()==eCHAR_DRAGON)
				m_pStyleContainer->ShowWindow(TRUE);
		}

		if (m_pHeroEquipContainer)
		{
			m_pHeroEquipContainer->UnTransForm();
		}


        xGetSkillData()->UnBlockSkill();

        if( uiSkillMan* skill_main = static_cast<uiSkillMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_SKILL)) )
        {
            skill_main->skill_tree()->RefreshUI();
        }

        xGetSkillData()->ResetTreeData();

        if (MouseHandler::Instance()->IsExistItemAtHand())
		{
			MouseHandler::Instance()->RollbackItemAtHand();
		}

		m_bCompleteTransForm=FALSE;
	}

    UpdateSkillAttr();
}
//------------------------------------------------------------------------------
/**
*/
BOOL Hero::Create( DWORD dwObjectKey, DWORD dwID )
{
    m_pPlayerAttriburtes = new HeroAttributes();
    m_pPlayerAttriburtes->SetOwner( &m_PlayerData );
    m_pPlayerAttriburtes->SetCharInfo( GetCharInfo(), 0, 0, 0 );
    
	Player::Create(dwObjectKey, dwID);
	
    InitializeStateForHero();

	// 아직 구현되지않았다.
	// 생성 / 해제만 하고 있는 상황
	m_iCanNotMoveTime=0;
	m_iComboCount = 0;
	m_bCustomAttackSpeed = FALSE;
	m_fCustomAttackSpeed = 1.0f;
	m_bCannotMove = FALSE;
	m_bCustomMoveSpeed = FALSE;
	m_fCustomMoveSpeed = 1.0f;

	//	Tab 버튼으로 타겟팅하기 위해 타겟팅 가능한 주변 몬스터 관리 리스트
	m_vectorTargetMonster.clear();
	m_CurTargetInfo.dwObjectKey = 0;
	m_CurTargetInfo.fDistance = 0.0f;
	m_CurTargetInfo.wTargetCheck = 0;

	m_CurTargetInfo.wTargetCount = 0;


	m_bShowFullMap = false;

	//GetSCCharacter().SetAllCoolTime( FALSE, 0 );
	//GetSCCharacter().InitializeCooltime();

	//GetSCPlayer().SetItemCoolTime( FALSE, 0 );
	//GetSCPlayer().InitializeItemCooltime();

	m_bCameraAlpha = TRUE;
	m_bNoCull = TRUE;

    xGetSkillData()->SetHero( this );

#ifdef _NA_004035_20120227_COSTUME_ITEM
    SetActivedCostumeItemSlotIndex(INVALID_SLOT_POS);
#endif//_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	SetActivedDecorateCostumeItemSlotIndex(INVALID_SLOT_POS);
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Hero::Destroy()
{
    if( SkillMain* instance = xGetSkillData() )
    {
        instance->SetHero( NULL );
    }

    //GetSCCharacter().FreeAllCoolTime( FALSE );
    //GetSCPlayer().FreeItemCoolTime( FALSE );

	//	Tab 버튼으로 타겟팅하기 위해 타겟팅 가능한 주변 몬스터 관리 리스트
	//	포인터가 아닌 변수 전달 방식으로 관리하므로 그냥 clear() 만 하면 된다
	m_vectorTargetMonster.clear();

	ItemManager::Instance()->ClearUpdateItemSlot();
    ItemManager::Instance()->ClearOwnerList();
	Player::Destroy();

    if( m_pPlayerAttriburtes != NULL )
    {
        delete m_pPlayerAttriburtes;
        m_pPlayerAttriburtes = NULL;
    }
}

//------------------------------------------------------------------------------
/**
*/
SCSlotContainer* Hero::GetSlotContainer( SLOTIDX slotIdx )
{
	switch( slotIdx )
	{
	case SI_INVENTORY:
		{ 
			return m_pInventoryContainer;
		}
		break;
	case SI_EQUIPMENT:
		{
			return GetEquipmentContainer();
		}
		break;
	case SI_STYLE:
		{
			return m_pStyleContainer;
		}
		break;
	case SI_QUICK:
		{
			return m_pQuickContainer;
		}
		break;
    case SI_VENDOR_SELL:
        {
			return GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SELL)->GetSlotContainer();
        }
        break;
    case SI_SKILL:
        {
            return g_InterfaceManager.GetUISkillSlotContainer();
        }
        break;
	case SI_WAREHOUSE:
		{
			return m_pBankContainer;
		}
		break;
	case SI_NPCSTORE:
		{
			SCSlotContainer * pSlotContainer = NULL;
			ShopDialog * pDlg = 
			static_cast<ShopDialog *>(g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SHOP ));
			
			pSlotContainer = static_cast<SCSlotContainer *>(pDlg);
			assert(pSlotContainer);
			return pSlotContainer;
		}
		break;

	case SI_GUILDWAREHOUSE:
		{
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
            DWORD dialog_id = uiGuildWareHouse::kDialog_ID;
#else
            DWORD dialog_id = IM_GUILD_MANAGER::GUILD_WAREHOUSE;
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER			
            uiGuildWareHouse* pSlotContainer = GET_CAST_DIALOG(uiGuildWareHouse, dialog_id);
            return static_cast<SCSlotContainer*>(pSlotContainer);
		}
		break;


    case SI_RIDER:
        {
            return m_pRiderContainer;
        }
        break;

#ifdef _YMS_AUTOBOT_GAME
    case SI_AUTOMATIC_INFO:
        {
            uiAutobotConditionBattle* dialog = GET_CAST_DIALOG(uiAutobotConditionBattle, uiAutobotConditionBattle::kDialog_ID);
            if (dialog)
            {
                return dialog->GetSlotContainer();
            }
        }
        break;
#endif //_YMS_AUTOBOT_GAME
	}
	return NULL;
}


//----------------------------------------------------------------------------
/**
*/
QuickContainer*	Hero::GetQuickContainer()
{
	return (QuickContainer *)GetSlotContainer(SI_QUICK);
}


//----------------------------------------------------------------------------
/**
*/
HeroEquipmentContainer* Hero::GetHeroEquipContainer()
{
	return this->m_pHeroEquipContainer;
}


//------------------------------------------------------------------------------
/**
*/
BOOL Hero::Process( DWORD dwTick )
{
	if(m_bCannotMove)
	{
		m_iCanNotMoveTime+=dwTick;
		//10초이상 응답이 없으면 움직일수 있게 해주자 
		if(m_iCanNotMoveTime>=10000)
		{
			SetCannotMoveFlag(FALSE);
		}
	}
    //------------------------------------------------------------------------------ 
    //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
    //! 5초에 한번씩 메모리 테스트
    {
        static DWORD prev_tick = clock_function::GetTickCount();
        DWORD cur_tick = clock_function::GetTickCount();

        if ((cur_tick - prev_tick) > (5 * 1000))
        {
            GetPlayerAttribute()->CheckMemoryTest();
            prev_tick = cur_tick;
        }
    }
    if (PacketNak.is_recv_nak == true)
    {
        //!초면 풀리게 한다.
        if ((clock_function::GetTickCount() - PacketNak.delay_tick > 1000))
        {
            PacketNak.is_recv_nak = false;
        }
    }
    //------------------------------------------------------------------------------ 


	if (m_bCameraAlpha)
	{

		float fRange = 0.0f;
		fRange = g_Camera.GetCurrentCameraFocusRange();

		if (fRange >= 1.6f)
		{
			float fDelta=1.5f*dwTick;
			m_fAlpha+=fDelta;	
			m_fAlpha=min(m_fAlpha,255.0f);
		}
		else 
		{
			m_fAlpha = 0.0f;
		}
	}

    BOOL rt = Player::Process( dwTick );

	ItemManager::Instance()->UpdateItemSlot();

    // 맵 위치 계산
    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager != NULL)
    {
        ui_radar_manager->SetHeroPosition(GetVisiblePos());
    }

	ShowConflictMessage();

	ActionCrystalWarp();

#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
    UpdateActivedComboSkill();
#endif

#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
    // 매크로 시스템 Process()
    xGetHeroData()->macro_system()->Process(dwTick);
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

	return rt;
}

//////////////////////////////////////////////////////////////////////////
void Hero::ActionCrystalWarp()
{
	if(0 == m_dwCrystalWarpObjKeys[0] && 0 == m_dwCrystalWarpObjKeys[1])
		return;

	for(int i = 0; i < CRYSTAL_WARP_OBJ_MAX_CNT; ++i)
	{
		//선택된 크리스탈 워프 오브젝트의 근처로 갔을경우 워프
		if(g_HeroInput.GetCurrentTarget() != m_dwCrystalWarpObjKeys[i])
			continue;

		Object* pObject = g_ObjectManager.GetObject(m_dwCrystalWarpObjKeys[i]);
		if(pObject)
		{
			WzVector wvLen = pObject->GetPosition() - this->GetPosition();
			float fLen = VectorLength(&wvLen);
			if(2.f <= fLen)
				continue;

			MSG_CG_CRYSTALWARP_COMMAND_SYN SendPacket;
			SendPacket.m_wvPos = c_ZeroVector;
			SendPacket.m_dwCrystalWarpObjKey = pObject->GetObjectKey();
			SendPacket.m_byCommand = eCRYSTALWARP_COMMAND_EXECUTE;

			this->SendPacket( &SendPacket, sizeof(SendPacket) );
			g_HeroInput.SetCurrentTarget(0);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void Hero::ShowConflictMessage()
{
	PKOptionInfo& PKInfo = PKScriptParser::Instance()->GetPKOptionInfo();
	BYTE IgnorePKTile = PKInfo.m_IgnorePKTile;

	//PK 가능 지역에서 나갔을때
	if (!(IgnorePKTile&PKOptionInfo::ignore_BoundaryPart) && BIT_CHECK(m_dwOldTileAtt,PTA_PK_TILE) && !BIT_CHECK(m_dwCurTileAtt,PTA_PK_TILE))
	{
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(eST_PK_ENABLE_WARNNING_OUT,szMessage,INTERFACE_STRING_LENGTH);//경계지역에서 벗어 났습니다.
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}

	//분쟁 지역에서 나갔을때 
	if (!(IgnorePKTile&PKOptionInfo::ignore_NomalPart) && BIT_CHECK(m_dwOldTileAtt,PTA_CONFLICT_TILE) && !BIT_CHECK(m_dwCurTileAtt,PTA_CONFLICT_TILE))
	{
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(eST_PK_ZONE_WARNNING_OUT,szMessage,INTERFACE_STRING_LENGTH);//분쟁 지역에에서 벗어 났습니다.
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}

	//자유 분쟁 지역에서 나갔을때
	if (!(IgnorePKTile&PKOptionInfo::ignore_FreePart) && BIT_CHECK(m_dwOldTileAtt,PTA_FREE_CONFLICT_TILE) && !BIT_CHECK(m_dwCurTileAtt,PTA_FREE_CONFLICT_TILE))
	{
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(eST_PK_FREE_ZONE_WARNNING_OUT,szMessage,INTERFACE_STRING_LENGTH);//자유 분쟁 지역에서 벗어 났습니다.
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}

	////////////////////
#ifdef _NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_
	//안전지대에 진입했을때
	if(BIT_CHECK(m_dwCurTileAtt,PTA_SAFETY_ZONE)&&!BIT_CHECK(m_dwOldTileAtt,PTA_SAFETY_ZONE))
	{
		WzColor color =WzColor_RGBA( 0, 255, 0, 255 );
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(6380,szMessage,INTERFACE_STRING_LENGTH);//경계지역에 진입 하였습니다.
		g_InterfaceManager.ShowMessageBox(szMessage,5000,&color, 0);
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}
#endif //_NA001415_091123_CHANGE_MOB_AI_IN_SAFETY_AREA_

	//PK 가능 지역에 진입했을때
	if (!(IgnorePKTile&PKOptionInfo::ignore_BoundaryPart) && BIT_CHECK(m_dwCurTileAtt,PTA_PK_TILE) && !BIT_CHECK(m_dwOldTileAtt,PTA_PK_TILE))
	{
		WzColor color = WzColor_RGBA(251, 218, 54, 255);
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(eST_PK_ENABLE_WARNNING, szMessage, INTERFACE_STRING_LENGTH);//경계지역에 진입 하였습니다.
		g_InterfaceManager.ShowMessageBox(szMessage, 5000, &color, 0);

		g_InterfaceManager.GetInterfaceString(eST_PK_ENABLE_WARNNING_IN, szMessage, INTERFACE_STRING_LENGTH);// 경계 지역에 진입 하였습니다.
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

	}

	//분쟁 지역에 진입 했을때 
	if (!(IgnorePKTile & PKOptionInfo::ignore_NomalPart) && BIT_CHECK(m_dwCurTileAtt, PTA_CONFLICT_TILE) && !BIT_CHECK(m_dwOldTileAtt, PTA_CONFLICT_TILE))
	{
		WzColor color = WzColor_RGBA(255, 128, 0, 255);
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(eST_PK_ZONE_WARNNING, szMessage, INTERFACE_STRING_LENGTH);//주의! 분쟁 지역에 직입하였습니다.
		g_InterfaceManager.ShowMessageBox(szMessage, 5000, &color, 0);

		g_InterfaceManager.GetInterfaceString(eST_PK_ZONE_WARNNING_IN, szMessage, INTERFACE_STRING_LENGTH);//분쟁 지역에 진입 하였습니다.
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}

	//자유 분쟁 지역에 진입 했을때 
	if (!(IgnorePKTile & PKOptionInfo::ignore_FreePart) && BIT_CHECK(m_dwCurTileAtt, PTA_FREE_CONFLICT_TILE) && !BIT_CHECK(m_dwOldTileAtt, PTA_FREE_CONFLICT_TILE))
	{
		WzColor color = WzColor_RGBA(255, 0, 0, 255);
		TCHAR	szMessage[INTERFACE_STRING_LENGTH];
		g_InterfaceManager.GetInterfaceString(eST_PK_FREE_ZONE_WARNNING, szMessage, INTERFACE_STRING_LENGTH);//주의! 자유 분쟁 지역에 직입하였습니다.
		g_InterfaceManager.ShowMessageBox(szMessage, 5000, &color, 0);

		g_InterfaceManager.GetInterfaceString(eST_PK_FREE_ZONE_WARNNING_IN, szMessage, INTERFACE_STRING_LENGTH);//자유 분쟁 지역에 진입 하였습니다.
		GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
	}	
}

//------------------------------------------------------------------------------
/**
*/
BOOL Hero::SendPacket( void *pBuf, int iLength )
{
	MSG_BASE_FORWARD *	p = (MSG_BASE_FORWARD *)pBuf;

	return GlobalFunc::SendPacket( CI_GAMESERVERIDX, pBuf, iLength ) ? TRUE : FALSE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL Hero::SendStopMovePacket()
{
#ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
    if ((PacketNak.is_recv_nak == false) && (BattleScene::IsWaitEnterSync() == FALSE))
    {
        ulong next_stop_serial_key = 0;
        if (KBMoveCheckManagerInClient::Instance()->IssueNextStopSerial(&next_stop_serial_key) == true)
        {
            MSG_CG_SYNC_STOP_SYN StopSyn;
            StopSyn.wvCurPos = GetPosition();
            StopSyn.issued_stop_serial = next_stop_serial_key;
            StopSyn.client_test_result = \
                nsSync::MemoryHackMonitor::Instance()->GetNextClientTestSerial();
            return SendPacket( &StopSyn, sizeof(StopSyn) );
        }
        WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "KBMoveCheckManagerInClient::Instance()->IssueNextStopSerial(&next_stop_serial_key) == false");
    }
    return false;
#else

    MSG_CG_SYNC_STOP_SYN StopSyn;
    StopSyn.m_byCategory = CG_SYNC;
    StopSyn.m_byProtocol = CG_SYNC_STOP_SYN;
    StopSyn.wvCurPos = GetPosition();

	#ifdef _POSITION_DEBUG
	TCHAR temp[512] = {0,};
	Snprintf( temp, 512-1, _T("서버로 STOP 패킷 보냄:%f,%f,%f"), StopSyn.wvCurPos.x, StopSyn.wvCurPos.y, StopSyn.wvCurPos.z);
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp);
	#endif

    return SendPacket( &StopSyn, sizeof(StopSyn) );
#endif //_NA000000_SPEED_HACK_PREVENTION_SUPPORT

}
//------------------------------------------------------------------------------ 
bool Hero::SendStandUp()
{
    MSG_CG_STATUS_CONDITION_CHANGE_SYN syn;
    syn.m_byCategory = CG_STATUS;
    syn.m_byProtocol = CG_STATUS_CONDITION_CHANGE_SYN;      
    syn.m_byConditionType = eCHAR_CONDITION_STANDUP;

    return (SendPacket(&syn, sizeof(syn)) == TRUE);
}


//------------------------------------------------------------------------------
/**
*/
BOOL Hero::SetPosition( const WzVector& wvPos ,int iTileIndex)
{
	return Player::SetPosition(wvPos,iTileIndex);
}


//------------------------------------------------------------------------------
/**
*/
MONEY Hero::GetMoney( void )
{
	assert(GetCharInfo());
	return GetCharInfo()->m_Money;
}


//------------------------------------------------------------------------------
/**
*/
void Hero::SetMoney(MONEY Money)
{
	assert(GetCharInfo());
	GetCharInfo()->m_Money = Money;


	if ( g_pQMGR_Real )
	{
		g_pQMGR_Real->Event(QUEST_EVENT_MONEY, NULL);
		g_InterfaceManager.UpdateQuestInfo();
	}	

}

//------------------------------------------------------------------------------
/**
*/
void Hero::InitContainer()
{
    BASE_PLAYERINFO* bp = this->GetCharInfo();

	char pszFileName[MAX_PATH] = {0,};

	_snprintf (pszFileName, MAX_PATH-1, "Data\\Interface\\41_1_Item_Slot_Inventory(Common).iwz");

    if (!g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_EQUIP))
    {
        m_pHeroEquipContainer = new HeroEquipmentContainer();
		m_pHeroEquipContainer->SetInventoryDialog( GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG ) );
        if( !g_InterfaceManager.LoadInterface( 
            m_pHeroEquipContainer, 
            pszFileName, 
            StrToWzID("0410"), 
            FALSE ) )
        {

        }
        m_pHeroEquipContainer->SetDialogKey( StrToWzID("0410") );
        m_pHeroEquipContainer->SetExtendInventory(
            static_cast<ExtendInventoryDialog*>(GET_DIALOG(IM_INVENTORY_MANAGER::EXTEND_INVEN_DLG))); 
    }

    /// g_InterfaceManager에서 가져 온것은 포인터를 반환하거나 삭제 할 필요 없다.
	SetEquipmentContainer(m_pHeroEquipContainer);
	GetEquipmentContainer()->SetOwnerKey(GetObjectKey());
	m_pInventoryContainer = static_cast<InventoryDialog *>( GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG ) ); 
	m_pQuickContainer = static_cast<QuickContainer *>(g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_QUICK ));
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
	m_pSummonContainer = static_cast<SummonSkillContainer *>(g_InterfaceManager.GetDialog( InterfaceManager::DIALOG_SUMMON_SKILL_CONTAINER ));
#endif

    RiderInventoryDialog* pDlg = GET_CAST_DIALOG(RiderInventoryDialog, IM_INVENTORY_MANAGER::RIDERINVEN_DLG);
    if (pDlg)
    {
       m_pRiderContainer = pDlg->GetSlotContainer(); 
       m_pRiderContainer->SetOwnerKey(GetObjectKey());
    }

#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    uiBankDialog *pBankDlg = GET_CAST_DIALOG(uiBankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#else
    BankDialog *pBankDlg = GET_CAST_DIALOG(BankDialog, IM_BANK_MANAGER::BANK_DIALOG);
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM	
	m_pBankContainer = static_cast<SCSlotContainer *>(pBankDlg);

	InventoryDialog *pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );

	EventInventoryDialog *pEventDlg = GET_CAST_DIALOG( EventInventoryDialog, IM_INVENTORY_MANAGER::EVENTINVEN_DLG );
    ExtendInventoryDialog* pExtendDlg = GET_CAST_DIALOG(ExtendInventoryDialog, IM_INVENTORY_MANAGER::EXTEND_INVEN_DLG);

    if (m_pHeroEquipContainer)
    {
        m_pHeroEquipContainer->InitItemRender();
    }

    if (m_pQuickContainer)
    {
	    m_pQuickContainer->InitItemRender();
    }

    if (m_pStyleContainer)
    {
	    m_pStyleContainer->InitItemRender();
    }

    if (m_pSummonContainer)
    {
	    m_pSummonContainer->InitItemRender();
    }

    if (pInvenDlg)
    {
	    pInvenDlg->InitRenderer();
        if (pExtendDlg)
        {
            pExtendDlg->InitRenderer();
        }
    }

    if (pBankDlg)
    {
	    pBankDlg->InitContainer();
    }

    if (pEventDlg)
    {
	    pEventDlg->InitRenderer();
    }
}


//------------------------------------------------------------------------------
/**
*/
void Hero::ReleaseContainer()
{
    QuickContainer* quick_container = (QuickContainer*)g_pHero->GetQuickContainer();
    if (quick_container)
    {
        quick_container->set_is_item_move(true);
    }

#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    uiBankDialog* pBankDlg = static_cast<uiBankDialog*>(ItemManager::Instance()->GetContainer(SI_WAREHOUSE));
#else
    BankDialog* pBankDlg = static_cast<BankDialog*>(ItemManager::Instance()->GetContainer(SI_WAREHOUSE));
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
	if (pBankDlg)
	{
		pBankDlg->ReleaseContainer();
	}

	EventInventoryDialog *pEventDlg = GET_CAST_DIALOG( EventInventoryDialog, IM_INVENTORY_MANAGER::EVENTINVEN_DLG );
    if (pEventDlg)
    {
	    pEventDlg->FlushUnitDraw();
    }

	static_cast<InventoryDialog *>(ItemManager::Instance()->GetContainer(SI_INVENTORY))->FlushUnitDraw();
	static_cast<HeroEquipmentContainer *>(ItemManager::Instance()->GetContainer(SI_EQUIPMENT))->FlushUnitDraw();
	static_cast<QuickContainer *>(ItemManager::Instance()->GetContainer(SI_QUICK))->FlushUnitDraw();

    if (m_pStyleContainer)
    {
	    m_pStyleContainer->FlushUnitDraw();
    }

    if (m_pSummonContainer)
    {
	    m_pSummonContainer->FlushUnitDraw();
    }

	if (GetEquipmentContainer())
	{
		SetEquipmentContainer(NULL);
        g_InterfaceManager.DeleteInterface(StrToWzID("0410"));
	}

    if (m_pHeroEquipContainer)
    {
        m_pHeroEquipContainer = NULL;
    }
	
	if (m_pQuickContainer)
	{
		m_pQuickContainer = NULL;
	}

	if (m_pStyleContainer)
	{
		m_pStyleContainer = NULL;
	}	

	if (m_pInventoryContainer)
	{
		m_pInventoryContainer = NULL;
	}

    if (quick_container)
    {
        quick_container->set_is_item_move(false);
    }
}

//------------------------------------------------------------------------------
/**
*/
int Hero::GetComboCount()
{
	return m_iComboCount;
}

//------------------------------------------------------------------------------
/**
*/
void Hero::SetComboCount(int iCount)
{
	m_iComboCount = iCount;
}
//------------------------------------------------------------------------------
/** 오직 인벤(보조인벤제외)토리의 사용가능슬롯수만 리턴한다 
*/
int	Hero::GetEmptyUseSlot()
{
	InventoryDialog *pInvenDlg = (InventoryDialog*)(m_pInventoryContainer);

	if(pInvenDlg)
	{
        // SCSlotContaine::GetEmptySlotNum() 변경됨
        // 이제부터 해당 함수의 리턴값을 그대로 사용하면 됨
		return static_cast<int>(m_pInventoryContainer->GetEmptySlotNum());
	}
	return 0;
}
//------------------------------------------------------------------------------
/**
*/
int Hero::DoAction(PLAYER_ACTION *pAction)
{
	SetCurrentAction(pAction);

	switch(pAction->ActionID)
	{
	case ACTION_ATTACK:
		{
			
			Character *pTarget = (Character *)g_ObjectManager.GetObject(pAction->ATTACK.dwTargetID);
			if (pTarget)
			{

#ifdef _YJ_PK_OPTION_001120
				if(TRUE == GAMEOPTION->GetPKWithoutCtrlKey())
				{
					m_bForceAttack=TRUE;
				}
				else
				{
					m_bForceAttack=pAction->ATTACK.bForceAttack;
				}
#else //_YJ_PK_OPTION_001120
				m_bForceAttack=pAction->ATTACK.bForceAttack;
#endif //_YJ_PK_OPTION_001120
				SetTargetID(pTarget->GetObjectKey());
				SetNextState(STATE::ATTACK,g_CurTime);
				
			}
			// 목표가 없으면 IDLE로
			else
			{

				m_bForceAttack=FALSE;

				SetTargetID(0);
				SetNextState(STATE::IDLE,g_CurTime);
			}
		}
		break;

	case ACTION_SKILL:
		{
			Character *pTarget = (Character *)g_ObjectManager.GetObject(pAction->SKILL.dwTargetID);
			StopMove();
			BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(pAction->SKILL.dwSkillID);
			
			// 3번은 자기 자리에서 바로 사용하는 스킬임
			if (pInfo->m_byTarget != SKILL_TARGET_ME&&pInfo->m_byTarget != SKILL_TARGET_AREA&&pInfo->m_byTarget != SKILL_TARGET_SUMMON &&pInfo->m_byTarget != SKILL_TARGET_AREA_ENEMY_CORPSE) 
			{
				if (pTarget)
				{
#ifdef _YJ_PK_OPTION_001120
					if(TRUE == GAMEOPTION->GetPKWithoutCtrlKey())
					{
						m_bForceAttack=TRUE;
					}
					else
					{
						m_bForceAttack=pAction->SKILL.bForceAttack;
					}
#else //_YJ_PK_OPTION_001120
					m_bForceAttack=pAction->SKILL.bForceAttack;
#endif //_YJ_PK_OPTION_001120
			
					// 시작좌표를 현재 좌표로 덮어쓴다!
					// Hero의 가장큰 차이점이다.
					pAction->SKILL.vCurPos.wvPos = GetPosition();
					pAction->SKILL.vCurPos.wTile = -1;
					SetCurrentAction(pAction);
					SetTargetID(pAction->SKILL.dwTargetID);
					SetTargetID(pAction->SKILL.dwTargetID);
					UseSkill(pAction->SKILL.dwSkillID);

					//	적에게 스킬을 사용하면 자동 공격 켜짐

					if ((pInfo->m_byTarget == SKILL_TARGET_ENEMY)
						|| (pInfo->m_byTarget == SKILL_TARGET_REACHABLE_ENEMY)
						|| (pInfo->m_byTarget == SKILL_TARGET_ENEMY_PLAYER)
						|| (pInfo->m_byTarget == SKILL_TARGET_ENEMY_AND_ME)
						)
					{
                        if (g_pHero)
                        {

#ifdef _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
                            if( (CGameOption::Instance()->GetSkillAfterAutoMove()) && 
                                (FALSE == pInfo->m_btAutoAttack)) //m_btAutoAttack가 FALSE이면 자동공격, TRUE면 자동공격X
#else
                            if( (g_pHero->GetClass() != eCHAR_ELEMENTALIST) && 
                                (FALSE == pInfo->m_btAutoAttack)) //m_btAutoAttack가 FALSE이면 자동공격, TRUE면 자동공격X
#endif _NA_000000_20140408_SkILL_USE_AFTER_AUTOATTACK_OPTION
                            {
								g_HeroInput.SetAutoAttack( TRUE );
						    }
                        }
					}

				}
				// 목표가 없으면 IDLE로
				else
				{

					m_bForceAttack=FALSE;

					SetTargetID(0);
					SetNextState(STATE::IDLE,g_CurTime);

				}
			}
			else
			{

				m_bForceAttack=pAction->SKILL.bForceAttack;

				pAction->SKILL.vCurPos.wvPos = GetPosition();
				pAction->SKILL.vCurPos.wTile = -1;
				SetCurrentAction(pAction);
				SetTargetID(0);
				SetTargetID(pAction->SKILL.dwTargetID);
				UseSkill(pAction->SKILL.dwSkillID);
			}
		}
		break;

	//지완 추가 
	case ACTION_KNOCKBACK:
		{
			StopMove();		
			SetNextState(STATE::DAMAGE,g_CurTime);	
		}
		break;

	case ACTION_KNOCKBACK2:
		{
			StopMove();
			SetNextState(STATE::KNOCKBACK2,g_CurTime);
		}	
		break;


	case ACTION_TOAIR:
		{
			StopMove();		
			//TOAIR_INFO info;
			//info.bGroundHit = FALSE;
			//info.fDownForceLimit = pAction->TOAIR.fDownForceLimit;
			//info.fGravity = pAction->TOAIR.fGravity;
			//info.fHeightLimit = pAction->TOAIR.fHeightLimit;
			//info.fUpperForce = pAction->TOAIR.fUpperForce;
			//info.iBoundCount = 0;
			//info.bForceDown= pAction->TOAIR.bForceDown;
			//ToAirInfo(&info);
			SetNextState(STATE::AIR,g_CurTime);	
			
		}
		break;

	case ACTION_DOWN:
		{
			StopMove();	
			SetNextState(STATE::DOWN,g_CurTime);
		
		}
		break;

	case ACTION_GETUP:
		{
			StopMove();	
			SetNextState(STATE::GETUP,g_CurTime);
			
		}
		break;

	case ACTION_SPECIAL_MOVE:
		{
			StopMove();	
			SetSpecialMoveFlag(pAction->SPECIALMOVE.bType);
			SetNextState(STATE::SPECIALMOVE,g_CurTime);
		}
		break;

	case ACTION_FALLDOWN:
		{
			StopMove();
			SetNextState(STATE::FALL_DOWN,g_CurTime);

			return 1;
		}
		break;

	case ACTION_AIR_FALLDOWN:
		{
			StopMove();
			SetNextState( STATE::AIR_FALL_DOWN, g_CurTime );

			return 1;
		}
		break;

	case ACTION_KEYBOARDJUMP:
		{
			StopMove();	
			SetNextState(STATE::KEYBOARDJUMP,g_CurTime);
			return 1;
		}
		break;


	case ACTION_TRANSFORM:
		{
			StopMove();

			GetTransformData().SetTransFormResourceCode(pAction->TRANSFORM.rcCode);
			GetTransformData().SetTransFormType(pAction->TRANSFORM.transFormType);
			GetTransformData().SetTransFormProcessType(pAction->TRANSFORM.transFormProcess);
			GetTransformData().SetAppearanceKind(pAction->TRANSFORM.appearKind);

			m_bCompleteTransForm=FALSE;
			SetNextState(STATE::TRANSFORM,g_CurTime);
			return 1;
		}
		break;

	case ACTION_FIRECRACKER:
		{
			StopMove();
			SetNextState(STATE::FIRECRACKER, g_CurTime);
			return 1;
		}
		break;


	case ACTION_COLLECTION_OBJECT:
		{
			if(STATE::COLLECT != GetCurState())
			{
				StopMove();
				SetNextState(STATE::COLLECT, g_CurTime);
                CommitChangeState(g_CurTime);
			}
			return 1;
		}
		break;

	case ACTION_SUMMON_RIDER:
		{
			if(STATE::SUMMON_RIDER != GetCurState())
			{
				StopMove();
                SendStopMovePacket();
				SetNextState(STATE::SUMMON_RIDER, g_CurTime);
			}
			return 1;
		}
		break;

	case ACTION_RIDER_TRUN:
		{
			if(STATE::RIDER_TURN != GetCurState())
			{
				SetNextState(STATE::RIDER_TURN, g_CurTime);
			}
			return 1;
		}
		break;


    case ACTION_ETHERIA_EXTRACT:
       {
           if(STATE::ETHERIA_EXTRACT != GetCurState())
           {
               SetNextState(STATE::ETHERIA_EXTRACT, g_CurTime);
           }
           return 1;
       }
       break;

    case ACTION_WINGS_SPREAD:
        {
            if(STATE::WINGS_SPREAD != GetCurState())
            {
                StopMove();
                SetNextState(STATE::WINGS_SPREAD, g_CurTime);
            }
            return 1;
        }
        break;
    case ACTION_WINGS_FURL:
        {
            if(STATE::WINGS_FURL != GetCurState())
            {
                StopMove();
                SetNextState(STATE::WINGS_FURL, g_CurTime);
            }
            return 1;
        }
    case ACTION_RIDER_SPREAD:
        {
            if(STATE::RIDER_SPREAD != GetCurState())
            {
                StopMove();
                SetNextState(STATE::RIDER_SPREAD, g_CurTime);
            }
            return 1;
        }
        break;
    case ACTION_RIDER_FURL:
        {
            if(STATE::RIDER_FURL != GetCurState())
            {
                StopMove();
                SetNextState(STATE::RIDER_FURL, g_CurTime);
            }
            return 1;
        }        
        break;

	default:
		assert(!"Hero::DoNextAction() 처리되지 않은 큐 명령");
		break;
	}

	return 1;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Hero::IsCancelTransForm(DWORD dwSkillClassCode)
{
	if(IsTransForm())//변신중에 변신 스킬 사용은 해제 
	{
		if(dwSkillClassCode==eSKILL_DRAGON_ARM_TRANSFORM)
		{
            switch (GetCurState())
            {
            case STATE::IDLE:
            case STATE::SIT:
            case STATE::EMOTICON:
            case STATE::MOVE:
                SendUnTransFormPacket();
                g_SkillQueue.Pop();
            }

            return TRUE;
		}
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Hero::IsCompleteTransForm()
{
	if(IsTransForm()&&m_bCompleteTransForm)
		return TRUE;

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Hero::CheckQueueActionCondition()
{
	if (m_ActionQueue.size() == 0)
	{
		return FALSE;
	}

	PLAYER_ACTION action =  m_ActionQueue.front();
	switch (action.ActionID)
	{
	case ACTION_KNOCKBACK2:
	case ACTION_TOAIR:
	case ACTION_KNOCKBACK:
	case ACTION_DOWN:
		return FALSE;

	case ACTION_ATTACK:
		{
			if( Character *pTarget = (Character *)g_ObjectManager.GetObject(action.ATTACK.dwTargetID) )
			{
				WzVector vDiff = pTarget->GetPosition() - GetPosition();
                float fDistance = VectorLength(&vDiff);

				float fActionLength = g_HeroInput.GetAttackActionLength();

				if ( fDistance <= fActionLength + pTarget->GetBodyRadius())
				{					
					return TRUE;
				}	
			}
			return FALSE;
		}
		break;

	case ACTION_SKILL:
		{
			//BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(action.SKILL.dwSkillID);
			//assert(pInfo);

			if( Character* pTarget = (Character*)g_ObjectManager.GetObject(action.SKILL.dwTargetID) )
			{
				WzVector vDiff = pTarget->GetPosition() - GetPosition();
				float fDistance = VectorLength(&vDiff);

				if (fDistance <= g_HeroInput.GetActionLength(action) + pTarget->GetBodyRadius())
				{					
					return TRUE;
				}	
			}
			return FALSE;
		}
		break;

	case ACTION_FIRECRACKER:
		{
			return TRUE;
		}
		break;

	case ACTION_RIDER_TRUN:
	case ACTION_SUMMON_RIDER:
    case ACTION_WINGS_SPREAD:
    case ACTION_WINGS_FURL:
    case ACTION_RIDER_SPREAD:
    case ACTION_RIDER_FURL:
		{
			return TRUE;
		}
		break;


    case ACTION_ETHERIA_EXTRACT:
        {
           return TRUE;
        }
        break;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
float Hero::GetProcessMultiplier()
{
	return 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
void Hero::SetKeyboardMovePacketSendDuration(DWORD dwDuration)
{
	m_dwKeyboardMovePacketSendDuration = dwDuration;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Hero::GetKeyboardMovePacketSendDuration()
{
	return m_dwKeyboardMovePacketSendDuration;
}

//------------------------------------------------------------------------------
/**
*/
void Hero::SetKeyboardMovePacketSendTime(DWORD dwTime)
{
	m_dwKeyboardMovePacketSendTime = dwTime;
}

//------------------------------------------------------------------------------
/**
*/
void Hero::AddKeyboardMovePacketSendTime(DWORD dwTime)
{
	m_dwKeyboardMovePacketSendTime += dwTime;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Hero::GetKeyboardMovePacketSendTime()
{
	return m_dwKeyboardMovePacketSendTime;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Hero::ProcessNextAction()
{	
	if (GetActionQueueCount()) 
	{
		if (CheckQueueActionCondition())
		{
			DoNextAction();							
			return TRUE;
		}
		else
		{
			SetNextState(STATE::IDLE,g_CurTime);
			return TRUE;
		}
	}

	BOOL bRet = g_HeroInput.ProcessQueueAction();
	if (bRet)
	{
		return TRUE;
	}

	return FALSE;
}

void Hero::OnDead()
{
	Character::OnDead();

    uiGuideSystemMan* guide_manager =
        static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

    if (guide_manager)
    {
        guide_manager->CheckHelpMessage(TotalGuideNotice::kDead);
    }
    
    g_SkillQueue.ClearAll();
    g_HeroInput.SetAreaSkill(FALSE);

	if( g_InterfaceManager.GetShowFlag() == FALSE )
	{
		g_InterfaceManager.ShowInterface(TRUE);
	}

    if (GameFramework::GetCurrentScene())
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		TCHAR szResultInfo[INTERFACE_STRING_LENGTH] = {0,};

		//튜토리얼맵(에소드마을)일때는 자동부활한다.
		if ((SCENE_TYPE_VILLAGE == GameFramework::GetCurrentScene()->GetType()) || g_pMap->IsEsodeMap())	
		{
			StartLeaveFieldCastTimer( eDoType_Delay_Spawn_Village, DEALY_SPAWN_VILLAGE);
			g_InterfaceManager.GetInterfaceString(86005,szMessage,INTERFACE_STRING_LENGTH);

			Sprintf( szResultInfo,szMessage,DEALY_SPAWN_VILLAGE/1000);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT , szResultInfo );
        }
        
        //#define _NA_000000_121015_LEAVE_CANCEL_TO_REVIVE_RE_CASTING
        CastLeaveFieldDialog* cast_dialog = 
            (CastLeaveFieldDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_DELAY_CASTING);

        // 떠나기 캐스팅이 아니었을때

        if ((cast_dialog->GetDoType() != eDoType_Delay_Leave_Domination) &&
            (cast_dialog->GetDoType() != eDoType_ServerSelectStandingBy) &&
            (cast_dialog->GetDoType() != eDoType_CharacterSelectStandingBy)
#ifdef _NA_000000_20120527_CHAOS_ZONE
            && (cast_dialog->GetDoType() != eDoType_Delay_Leave_ChaosZone)
#endif //_NA_000000_20120527_CHAOS_ZONE
            )
        {
            //Scene에 맞춰 부활 캐스팅 시전
            GlobalFunc::ReviveCastFromSceneType();


//#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        }
	}
}

//------------------------------------------------------------------------------
/**
	유저로 부터 입력을 받을수 없는 상태인가.
*/
BOOL Hero::IsLockInput()
{
	if ( IsDead() == TRUE )
	{
		return TRUE;
	}

	if( CannotAction() == TRUE )
	{
		return TRUE;
	}

	if( GetCurState()==STATE::DAMAGE||
		GetCurState()==STATE::GETUP||
		GetCurState()==STATE::DOWN)
	{
		return TRUE;
	}
	if (this->GetBeHaveState() == PLAYER_BEHAVE_TRADE_STATE)
	{
		return TRUE;
	}

    if (this->GetBeHaveState() == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE ||
        this->GetBeHaveState() == PLAYER_BEHAVE_VENDOR_OBSERVER_STATE)
    {
        return TRUE;
    }

    if (this->GetBeHaveState() == PLAYER_BEHAVE_PARTY_THINKING_JOIN ||
        this->GetBeHaveState() == PLAYER_BEHAVE_PARTY_THINKING_LEAVE)
    {
        return TRUE;
    }

	if(this->GetBeHaveState() == PLAYER_BEHAVE_SYSTEM_LOCK)
	{
		return TRUE;
	}

	if ( is_waiting_invite_group_ == true )
    {
        return TRUE;
    }

	if(this->GetBeHaveState() == PLAYER_BEHAVE_BANK_STATE)
	{
		return TRUE;
	}

    if (this->GetBeHaveState() == PLAYER_BEHAVE_SOCKET_COMPOSITE_STATE)
    {
        return TRUE;
    }

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    if (battle_ground_hide_mode() != BattleGroundHideMode_None)
    {
        return TRUE;
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	return FALSE;
}

//------------------------------------------------------------------------------
/**
	TRUE 이면 상태를 변화 할수 있다.
*/
BOOL Hero::CanDoQueueAction(PLAYER_ACTION *pAction)
{
	switch (GetCurState())
	{
	case STATE::IDLE:
	case STATE::SIT:
	case STATE::EMOTICON:
	case STATE::MOVE:
		return TRUE;

	case STATE::ATTACK:
		if (pAction->ActionID == ACTION_ATTACK || 
			pAction->ActionID == ACTION_SKILL)
		{
			if (g_HeroInput.CanDoNextAttack()) 
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}


//------------------------------------------------------------------------------
/**	에테르정보 초기화
*/
void Hero::SetEtherActive(BYTE byIndex)
{
	Player::SetEtherActive(byIndex);
	m_EtherInfo.byEtherIndex = byIndex;
}

//------------------------------------------------------------------------------
/** 에테르정보 초기화
*/
void Hero::ClearEthreInfo()
{ 
	memset(&m_EtherInfo, 0, sizeof(m_EtherInfo));	
}

//------------------------------------------------------------------------------
/** 에테르탄환 락,언락 해주는 함수
*/
void Hero::SetEtherItemLock(BOOL bZoneMove, BOOL bLock)
{	
	//장전아니라면 호출될이유없음
	if (!m_EtherInfo.byEtherIndex)
    {
        return;
    }
	
	//장전아이템을 얻어온다 
	SCItemSlot& rInvenItemSlot = (SCItemSlot&)(ItemManager::Instance()->GetContainer(SI_INVENTORY))->GetSlot(m_EtherInfo.byEtherChargePos); 
	SCItemSlot& rEquipItemSlot = (SCItemSlot&)(ItemManager::Instance()->GetContainer(SI_EQUIPMENT))->GetSlot(EquipmentContainer::EQUIP_WEAPON1);
	
	//락상태일때는 무조건 락!  아닐시에는 클리어 or 언락
	if (bLock)
	{
		rInvenItemSlot.GetLaxLock()->EnterLock();
		rEquipItemSlot.GetLaxLock()->EnterLock();
	}
	else
	{
		if (bZoneMove)
		{
			rInvenItemSlot.GetLaxLock()->Clear();
			rEquipItemSlot.GetLaxLock()->Clear();
		}
		else
		{
			rInvenItemSlot.GetLaxLock()->LeaveLock();
			rEquipItemSlot.GetLaxLock()->LeaveLock();
		}
	}
}



//------------------------------------------------------------------------------
/**
	주인공의 펫을 셋팅한다.
*/
void Hero::SetHeroPetInfo( BASE_PLAYERINFO* pPlayerInfo)
{
    if (!pPlayerInfo)
    {
        return;
    }

	if (pPlayerInfo->m_bSummonedPet)
	{
		SCItemSlot & rItemSlot = (SCItemSlot &)m_pInventoryContainer->GetSlot(pPlayerInfo->m_PetItemPos);
		if ((rItemSlot.IsContainValidItem()) && 
            (CreatePetSummon(rItemSlot.GetImageItemInfo()->m_wSkillCode)))
		{
			UpdatePetInfo(pPlayerInfo->m_PetItemPos);

			rItemSlot.StartQuantity();
		}
	}
    
    m_PetInfo.m_bPetSummoned = pPlayerInfo->m_bSummonedPet;
}
//------------------------------------------------------------------------------
/** 
펫속성 이펙트(친밀도,포만감)정보를 갱신
*/
void Hero::DeletePetSummon()
{
	uiPetNameSetting* pet_name_dialog = GET_CAST_DIALOG(uiPetNameSetting, IM_PET_MANAGER::PET_NAME_DLG);
	
    //펫해제되면 네임카드창도 자동닫힌다.
	if (pet_name_dialog)
	{
		if (pet_name_dialog->IsVisible())
        {
			pet_name_dialog->ShowInterface(FALSE);
        }
	}

	Player::DeletePetSummon();

}
//------------------------------------------------------------------------------
/** 
	펫슬롯 재지정(펫아이템소환시) 및 펫정보+펫옵션갱신
*/
void Hero::UpdatePetInfo(POSTYPE pos)
{
	m_PetInfo.HERO_.m_PetItemPos = pos;
	Pet* pPet = static_cast<Pet*>(g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey));
	
	if (pPet)
	{
		SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(m_pInventoryContainer->GetSlot(pos));
		//펫에게도 정보 갱신
		if (rItemSlot.IsPetItem())
		{
			SCPetInfo* pPetInfo  = rItemSlot.GetPetInfo();
			if (pPetInfo)
			{
				pPet->SetFullness(pPetInfo->GetFullness());
				pPet->SetIntimacy(pPetInfo->GetIntimacy());
			}
		}

		pPet->RefreshPetName();
		pPet->UpdatePetAttributes();
		UpdateSkillAttr();
	}
	
}
//------------------------------------------------------------------------------
/** 
	 펫슬롯정보를 업데이트한다.( 펫속성 -IN-> 펫슬롯 )
	 주인공은 아이템슬롯정보를 참조해서 친밀도와 포만감정보를 얻는다
	 bIsFullness: 1이면 포만감 0이면 친밀도
*/
void Hero::SetPetAttributes( POSTYPE pos, BYTE bIsFullness , BYTE byValue)
{
	SCItemSlot & rItemSlot = static_cast<SCItemSlot &>(m_pInventoryContainer->GetSlot(pos));
	
	if(rItemSlot.IsPetItem())
	{
		SCPetInfo  * pPetInfo  = rItemSlot.GetPetInfo();

		if(!pPetInfo) return;

		//아이템정보갱신
		((bIsFullness) ? ( pPetInfo->SetFullness(byValue) ):( pPetInfo->SetIntimacy(byValue) ));
		
		//펫정보갱신+옵션적용
		UpdatePetInfo(pos);
	}
}
//------------------------------------------------------------------------------
/** 
	펫에게 사료를 먹인다.
*/
void Hero::UsePetFeed()
{
	Pet* pPet  = static_cast<Pet*>(g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey));

	if(pPet)
	{
		pPet->PetEatAction();	
	}
}

void Hero::UpdateBadgeItemInfo(SkillAttrCalculator& SkillCalc)
{
    //if( xGetHeroData()->GetBadgeItemData().IsActive() )
    //{
    //    POSTYPE slot = xGetHeroData()->GetBadgeItemData().GetItemPosition();
    //    if( slot == INVALID_SLOT_POS )
    //        return;

    //    if( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( slot ) )
    //    {
    //        if( const BASE_ITEMINFO* item_info = item->GetItemInfo() )
    //        {
    //            for (int option_index = 0; option_index < BASE_ITEMINFO::_MAX_OPTION_NUM; ++option_index)
    //            {
    //                if ((item_info->m_wOptionIndex[option_index] != 0) && 
    //                    (item_info->m_iOptionValue[option_index] != 0) &&
    //                    (item_info->m_wOptionIndex[option_index] < _countof(g_ITEM_OPTION)))
    //                {
    //                    SkillCalc.AddAttr(g_ITEM_OPTION[item_info->m_wOptionIndex[option_index]],
    //                        item_info->m_byOptionType[option_index], item_info->m_iOptionValue[option_index]);
    //                }
    //            }

    //            if( GetPCBangMode() != 0 )
    //            {
    //                for (int option_index = 0; option_index < BASE_ITEMINFO::_MAX_OPTION_NUM; ++option_index)
    //                {
    //                    if ((item_info->m_byPCOptionType[option_index] != 0) && 
    //                        (item_info->m_iPCOptionValue[option_index] != 0) &&
    //                        (item_info->m_wPCOptionIndex[option_index] < _countof(g_ITEM_OPTION)))
    //                    {
    //                        SkillCalc.AddAttr(
    //                            g_ITEM_OPTION[item_info->m_wPCOptionIndex[option_index]],
    //                            item_info->m_byPCOptionType[option_index], 
    //                            item_info->m_iPCOptionValue[option_index]);
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}
}

#ifdef _NA_004035_20120227_COSTUME_ITEM
void Hero::UpdateCostumeInfo(SkillAttrCalculator& SkillCalc)
{
    POSTYPE slot = GetActivedCostumeItemSlotIndex();
    if( slot == INVALID_SLOT_POS )
        return;

    if( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( slot ) )
    {
        if( const BASE_ITEMINFO* item_info = item->GetItemInfo() )
        {

            for (int option_index = 0; option_index < BASE_ITEMINFO::_MAX_OPTION_NUM; ++option_index)
            {
                if ((item_info->m_wOptionIndex[option_index] != 0) && 
                    (item_info->m_iOptionValue[option_index] != 0) &&
                    (item_info->m_wOptionIndex[option_index] < _countof(g_ITEM_OPTION)))
                {
                    SkillCalc.AddAttr(g_ITEM_OPTION[item_info->m_wOptionIndex[option_index]],
                        item_info->m_byOptionType[option_index], item_info->m_iOptionValue[option_index]);
                }
            }

            if( GetPCBangMode() != 0 )
            {
                for (int option_index = 0; option_index < BASE_ITEMINFO::_MAX_OPTION_NUM; ++option_index)
                {
                    if ((item_info->m_byPCOptionType[option_index] != 0) && 
                        (item_info->m_iPCOptionValue[option_index] != 0) &&
                        (item_info->m_wPCOptionIndex[option_index] < _countof(g_ITEM_OPTION)))
                    {
                        SkillCalc.AddAttr(
                            g_ITEM_OPTION[item_info->m_wPCOptionIndex[option_index]],
                            item_info->m_byPCOptionType[option_index], 
                            item_info->m_iPCOptionValue[option_index]);
                    }
                }
            }
        }
    }
}

#endif //_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
void Hero::UpdateDecorateCostumeInfo( SkillAttrCalculator& SkillCalc )
{
	POSTYPE slot = GetActivedDecorateCostumeItemSlotIndex();
	if( slot == INVALID_SLOT_POS )
		return;

	if( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( slot ) )
	{
		if( const BASE_ITEMINFO* item_info = item->GetItemInfo() )
		{

			for (int option_index = 0; option_index < BASE_ITEMINFO::_MAX_OPTION_NUM; ++option_index)
			{
				if ((item_info->m_wOptionIndex[option_index] != 0) && 
					(item_info->m_iOptionValue[option_index] != 0) &&
					(item_info->m_wOptionIndex[option_index] < _countof(g_ITEM_OPTION)))
				{
					SkillCalc.AddAttr(g_ITEM_OPTION[item_info->m_wOptionIndex[option_index]],
						item_info->m_byOptionType[option_index], item_info->m_iOptionValue[option_index]);
				}
			}

			if( GetPCBangMode() != 0 )
			{
				for (int option_index = 0; option_index < BASE_ITEMINFO::_MAX_OPTION_NUM; ++option_index)
				{
					if ((item_info->m_byPCOptionType[option_index] != 0) && 
						(item_info->m_iPCOptionValue[option_index] != 0) &&
						(item_info->m_wPCOptionIndex[option_index] < _countof(g_ITEM_OPTION)))
					{
						SkillCalc.AddAttr(
							g_ITEM_OPTION[item_info->m_wPCOptionIndex[option_index]],
							item_info->m_byPCOptionType[option_index], 
							item_info->m_iPCOptionValue[option_index]);
					}
				}
			}
		}
	}
}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

//------------------------------------------------------------------------------
/** 
	펫 스킬옵션적용 
*/
void Hero::UpdatePetSkillAttr(SkillAttrCalculator &SkillCalc)
{
	if (!m_PetInfo.m_bPetSummoned)
    {
        return;
    }
	
	//1.기본옵션적용	
    SCItemSlot& item_slot = static_cast<SCItemSlot &>(m_pInventoryContainer->GetSlot(m_PetInfo.HERO_.m_PetItemPos));

	const BASE_ITEMINFO* item_info = item_slot.GetItemInfo(); 

	if (!item_info)
    {
        return;
    }

    if (!item_slot.IsContainValidItem())
    {
        return;
    }

	for (int option_index = 0; option_index < BASE_ITEMINFO::_MAX_OPTION_NUM; ++option_index)
	{
		if ((item_info->m_wOptionIndex[option_index] != 0) && 
			(item_info->m_iOptionValue[option_index] != 0) &&
			(item_info->m_wOptionIndex[option_index] < _countof(g_ITEM_OPTION)))
		{
			SkillCalc.AddAttr(g_ITEM_OPTION[item_info->m_wOptionIndex[option_index]],
				            item_info->m_byOptionType[option_index], item_info->m_iOptionValue[option_index]);
		}
	}
	
	SCPetInfo* pet_Info = item_slot.GetPetInfo();
	
	if (!pet_Info)
    {
        return; 
    }

	//2.추가옵션적용(친밀도 MAX시)	
#ifdef __CN_PET_OPTION_APPLY_FLAG
	sPet_Script_Info* pet_script_info = PetInfoParser::Instance()->GetPetInfo(item_info->m_wSkillCode);

	if (!pet_script_info)
    {
        return;
    }

	int level = PetInfoParser::Instance()->GetOptionLevel(GetPetIndex(),pet_Info->GetIntimacy());
		
    // 누적옵션 없을시에는 일반적용
    if (pet_script_info->m_eApplyOptionFlag== sPet_Script_Info::ePET_OPTION_FLAG_NONE)
    {
        if (pet_Info->GetIntimacy()>=60)
        {
            int level_index =MAX_PET_OPTION_EXTENT_COUNT-2;

            if (pet_Info->GetIntimacy()==100)
            {
                level_index =MAX_PET_OPTION_EXTENT_COUNT-1;
            }

            for (int option_index = 0; option_index < MAX_PET_OPTION_COUNT; ++option_index)
            {
                if ( pet_script_info->m_PetOptionInfo[level_index].m_wOptionKind[option_index] != 0	&&
                    pet_script_info->m_PetOptionInfo[level_index].m_byValueType[option_index] != 0  &&
                    pet_script_info->m_PetOptionInfo[level_index].m_wOptionKind[option_index] < _countof(g_ITEM_OPTION) )
                {
                    SkillCalc.AddAttr(g_ITEM_OPTION[pet_script_info->m_PetOptionInfo[level_index].m_wOptionKind[option_index]],
                        pet_script_info->m_PetOptionInfo[level_index].m_byValueType[option_index],
                        pet_script_info->m_PetOptionInfo[level_index].m_iValue[option_index]);
                }
            }
        }
    }
    else
    {   
        for (int level_count = 0; level_count <= level; ++level_count)
        {
            if (!pet_script_info->CanUseOption(level_count))
            {
                continue;
            }

            for (INT option_index = 0; option_index < MAX_PET_OPTION_COUNT; ++option_index)
            {
                if( (pet_script_info->m_PetOptionInfo[level_count].m_wOptionKind[option_index]) != 0 &&
                    (pet_script_info->m_PetOptionInfo[level_count].m_byValueType[option_index]) != 0 &&
                    (pet_script_info->m_PetOptionInfo[level_count].m_wOptionKind[option_index]) < _countof(g_ITEM_OPTION) )
                {
                    SkillCalc.AddAttr(g_ITEM_OPTION[pet_script_info->m_PetOptionInfo[level_count].m_wOptionKind[option_index]],
                        pet_script_info->m_PetOptionInfo[level_count].m_byValueType[option_index],
                        pet_script_info->m_PetOptionInfo[level_count].m_iValue[option_index]);
                }
            }
        }
    
    }
#else
	if (pet_Info->GetIntimacy()>=60)
	{
		sPet_Script_Info* pet_script_info = PetInfoParser::Instance()->GetPetInfo(item_info->m_wSkillCode);
	
		if (!pet_script_info)
        {
            return;
        }

		int level_index =MAX_PET_OPTION_EXTENT_COUNT-2;

		if (pet_Info->GetIntimacy()==100)
		{
			level_index =MAX_PET_OPTION_EXTENT_COUNT-1;
		}

		for (int option_index = 0; option_index < MAX_PET_OPTION_COUNT; ++option_index)
		{
			if ( pet_script_info->m_PetOptionInfo[level_index].m_wOptionKind[option_index] != 0	&&
				pet_script_info->m_PetOptionInfo[level_index].m_byValueType[option_index] != 0  &&
				pet_script_info->m_PetOptionInfo[level_index].m_wOptionKind[option_index] < _countof(g_ITEM_OPTION) )
			{
				SkillCalc.AddAttr(g_ITEM_OPTION[pet_script_info->m_PetOptionInfo[level_index].m_wOptionKind[option_index]],
					              pet_script_info->m_PetOptionInfo[level_index].m_byValueType[option_index],
					              pet_script_info->m_PetOptionInfo[level_index].m_iValue[option_index]);
			}
		}
	}
#endif//__CN_PET_OPTION_APPLY_FLAG
}	

void Hero::RefreshEquipData()
{
    UpdateItemAttrCalculator();

    UpdateSkillAttr();

    GetPlayerAttribute()->Update();

    RefreshAppearRender();

    if (IsEtherDeviecBulletChargeing())
    {
        CreateEtherWeaponEffect(0, TRUE);
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
void Hero::UpdateItemAttrCalculator()
{
    if (GetEquipmentContainer() == NULL)
    {
        return; 
    }	

    EquipmentContainer* container_ptr = GetEquipmentContainer();

    ItemAttrCalculator itemCalc((*GetPlayerAttribute()), container_ptr);
    itemCalc.EquipAll(!!g_pHero->GetPCBangMode(), false, g_pHero->GetLevel());

    g_pHero->GetPlayerAttribute()->Update();
}

void Hero::UpdateActiveSkillEffectAttr(SkillAttrCalculator &SkillCalc)
{
    std::vector<SKILL_EFFECT*>& array = GetSkillEffectArray();

    for( size_t i=0;i!=array.size();++i)
    {
        SKILL_EFFECT* pEffect = array[i];

        if( SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)pEffect->dwSkillID) )
        {
            if (pEffect->AbilityInfo.m_wAbilityID == pEffect->dwAbilityID)
            {
                eATTR_TYPE eType = pEffect->AbilityInfo.GetAttrType();

                if( eType == eATTR_CUR_HP )
                {
                    ;
                }
                else if( eType == eATTR_CUR_MP )
                {
                    ;
                }
                else if( eType != eATTR_TYPE_INVALID )
                {
                    int iAddValue = 
                        (int)SkillCalc.AddAttr( 
                        eType, 
                        pEffect->AbilityInfo.m_iParam[0], 
                        pEffect->AbilityInfo.m_iParam[1] );	
                }
            }			
        }
    }
}

int Hero::GetItemScore()
{
    if( EquipmentContainer* equipment_container = GetEquipmentContainer() )
    {
        return equipment_container->GetEquipmentManageInfo()->item_score_info.GetTotalScore();
    }

    return -1;
}

void Hero::ItemAttrCalculatorByItemslot(SCSlot& item_slot, bool is_equip, POSTYPE position)
{
    EquipmentContainer* container_ptr = GetEquipmentContainer();
    if( container_ptr == NULL )
    {
        return;
    }

    SCItemSlot& slot = (SCItemSlot&)item_slot;
    ItemAttrCalculator itemCalc((*GetPlayerAttribute()), container_ptr);
    BOOL bFullSetItem = FALSE;
    if (is_equip == true)
    {
        itemCalc.Equip2(slot, (GetPCBangMode()!=FALSE), false, GetLevel());
    }
    else
    {
        itemCalc.Unequip2(&slot, position, (GetPCBangMode()!=FALSE), false, GetLevel());
    }

    if (g_pHero != NULL)
    {
        g_pHero->GetPlayerAttribute()->Update();
    }
}


//------------------------------------------------------------------------------
/** 라이더 인벤및 파츠갱신
*/
void Hero::RefreshRider()
{
    Player::RefreshRider();
    
    RiderInventoryDialog *pInven= (RiderInventoryDialog *)m_pRiderContainer;
    if(pInven && IsRiding())
    {
       pInven->CopyHeroRider(m_pRider);
    }
}

//------------------------------------------------------------------------------
/** 주인공 라이더가 만들어질때는 인벤도체크
*/
void Hero::CreateRider(DWORD dwID,BOOL bEffect)
{
	Player::CreateRider(dwID,bEffect);
	ShowRiderInven(TRUE);
}
//------------------------------------------------------------------------------
/** 라이더 해제
*/
void Hero::DestroyRider()
{
    RiderInventoryDialog *pInven= (RiderInventoryDialog *)m_pRiderContainer;
    ShowRiderInven(FALSE);
    if(pInven && IsRiding())
    {
        pInven->CopyHeroRider(NULL);
    }

    Player::DestroyRider();
}
//------------------------------------------------------------------------------
/** 라이더 인벤열기
*/
void Hero::ShowRiderInven(BOOL bOepn)
{
  RiderInventoryDialog *pDlg = GET_CAST_DIALOG( RiderInventoryDialog, IM_INVENTORY_MANAGER::RIDERINVEN_DLG );

  if(!pDlg)
  {
	  return;
  }

  if(bOepn)
  {
	//인벤토리가 열렸을때만 열어준다.
	InventoryDialog * pInvenDlg = GET_CAST_DIALOG( InventoryDialog, IM_INVENTORY_MANAGER::INVEN_DLG );
    if(pInvenDlg && pInvenDlg->IsVisibleWindow())
	{
       pDlg->ShowInterface(TRUE);
	}
    else
    {
        ExtendInventoryDialog* pExtendDlg = GET_CAST_DIALOG(ExtendInventoryDialog, IM_INVENTORY_MANAGER::EXTEND_INVEN_DLG);
        if ((pExtendDlg != NULL) && pExtendDlg->IsVisibleWindow())
        {
            pDlg->ShowInterface(TRUE);
        }
    }
  }
  else
  {
	pDlg->ShowInterface(FALSE);
  }
  
}

//------------------------------------------------------------------------------
/** 라이더 소환해제패킷보내기
*/
void Hero::SendRiderReleasePacket(bool is_battle)
{
    if(IsRiding())
    {
        if (rider() && rider()->is_spread())
        {
            // 탈것이 날고 있으면 착륙하라고 보낸다.
            g_pHero->SendWingsControlByRider(false, false);
        }
        
        MSG_CG_ITEM_RIDER_CONTROL_SYN SendMsg;
        
        if (true == is_battle)
        {
            SendMsg.header.request = SendMsg.header.eReq_SummonBattleRelease;
        }
        else
        {
            SendMsg.header.request = SendMsg.header.eReq_SummonRelease;
        }
        
        SendMsg.header.args.summon.slot_pos = m_RiderInfo.m_RiderItemPos;

        GlobalFunc::SendPacket(CI_GAMESERVERIDX, &SendMsg, sizeof(SendMsg) );
    }
}
//------------------------------------------------------------------------------ 
bool Hero::SendWingsControl(eSlotContainerIndex container_index, POSTYPE slot_at, bool is_spread)
{
    if (PacketRequestManager::Instance()->is_sent(CG_SYNC, CG_SYNC_WINGS_CONTROL_SYN) == true)
    {
        return false;
    }

    MSG_CG_SYNC_WINGS_CONTROL_SYN syn;
    if (is_spread)
    {
        syn.header.request = WingControl::eReq_SpreadWings;
        syn.header.args.spread.wings_pos = slot_at;
        syn.header.args.spread.wings_container = container_index;
    }
    else
    {
        syn.header.request = WingControl::eReq_FurlWings;
        syn.header.args.furl.wings_pos = slot_at;
        syn.header.args.furl.wings_container = container_index;
    }

    bool result = (SendPacket(&syn, sizeof(syn)) == TRUE);

    if (result)
    {
        MSG_CG_SYNC_WINGS_CONTROL_SYN* syn = new MSG_CG_SYNC_WINGS_CONTROL_SYN;
        MSG_CG_SYNC_WINGS_CONTROL_CMD* ack = new MSG_CG_SYNC_WINGS_CONTROL_CMD;
        MSG_CG_SYNC_WINGS_CONTROL_CMD* nak = new MSG_CG_SYNC_WINGS_CONTROL_CMD;

        PacketRequestManager::Instance()->PushPacket(syn, ack, nak,
                                                    sizeof(MSG_CG_SYNC_WINGS_CONTROL_SYN), 
                                                    PACKET_WAITING_MILLISECOND,
                                                    false);
    }
    
    return result;
}
//------------------------------------------------------------------------------ 
bool Hero::SendWingsControlByAccessory(bool is_spread, bool is_force_send)
{
    if ((is_spread_wings() == is_spread) && (is_force_send == false))
    {
        return true;
    }

    POSTYPE at_pos;
    if (is_wing_equip(&at_pos) == true)
    {
        return SendWingsControl(SI_EQUIPMENT, at_pos, is_spread);
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool Hero::SendWingsControlByRider(bool is_spread, bool is_force_send)
{
    if (IsRiding() == FALSE)
    {
        return false;
    }
    
    if (rider() && (rider()->is_spread() == is_spread) && (is_force_send == false))
    {
        return true;
    }

    return SendWingsControl(SI_INVENTORY, m_RiderInfo.m_RiderItemPos, is_spread);
}
//------------------------------------------------------------------------------
/**
	베틀 씬에 저장해놓은 히어로 정보를 세팅하는 함수 

	- y2jinc
*/
void Hero::SetHeroInfo( const HERO_INFO * pHeroInfo )
{
    SetCharInfo( &pHeroInfo->m_CharInfo );

    BASE_PLAYERINFO* pPlayerInfo = GetCharInfo();

	//m_scPlayerAtt.SetCharInfo( &pHeroInfo->m_CharInfo );

	SetName(pPlayerInfo->m_tszCharName);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    SetTitleName(pPlayerInfo->honor_title_);
#endif //_NA_003027_20111013_HONOR_SYSTEM

	m_PlayerMakeType = pHeroInfo->m_MakeType;
    SetCurrentAttackStyle( pHeroInfo->m_CharInfo.m_iSelectStyleCode );
	
    //hp,mp 서버에서만 쓴다.
	m_pPlayerAttriburtes->SetCharInfo( GetCharInfo(),0,0,0);

	SetPartVariation(PLAYER_VARIATION_HAIR,pHeroInfo->m_CharInfo.m_byHair);
	SetPartVariation(PLAYER_VARIATION_FACE,pHeroInfo->m_CharInfo.m_byFace);

	HideHelmet( pHeroInfo->m_bHideHelmet ,FALSE);
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    HideWing(pHeroInfo->m_bHideWing ,FALSE);
    GAMEOPTION->SetShowWing(pHeroInfo->m_bHideWing);
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	//gs_bug_fix:옵션창과 서버투구정보불일치

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
    HideCostume(pHeroInfo->m_bHideCostume,FALSE);
    GAMEOPTION->SetShowCostume(pHeroInfo->m_bHideCostume);
#endif
	
#ifdef _NA_000000_20130114_RENEWER_UI
    uiHelmetShow* ui_helmet_show_dlg_ptr = GET_CAST_DIALOG( uiHelmetShow, IM_INVENTORY_MANAGER::HELMET_UI );
    if ( ui_helmet_show_dlg_ptr )
    {
        ui_helmet_show_dlg_ptr->set_check_show_helmet_state(!pHeroInfo->m_bHideHelmet);
    }
#else
	OptionGameDialog * pOptionDlg= GET_CAST_DIALOG( OptionGameDialog, IM_OPTION_MENU_MANAGER::OPTION_GAME_DIALOG );
	if(pOptionDlg)
	{
		pOptionDlg->SetHelmetInfo(!pHeroInfo->m_bHideHelmet);
	}
#endif

	SetObserverMode( pHeroInfo->m_bObserverMode );
	SetGMGrade( pHeroInfo->m_byGMGrade );
	SetPCBangMode( pHeroInfo->m_byPCBang );
	SetBlockPCBang( pHeroInfo->m_byBlockPCBang );
    
    const GUILD_RENDER_INFO* guild_render_info = &(pHeroInfo->m_GuildRenderInfo);
    m_bExistGuild = guild_render_info->IsExist();
    if (m_bExistGuild)
    {
        SetGuildRenderInfo(guild_render_info);
    }
    // 자신의 인장UI(zonemark) 업데이트
    SetDominationStateInfo(&guild_render_info->m_stateTotalInfo);
    uiDominationTowerMan::RefreshZoneMarkInfo();

	BOOL bSkillQueueing = GAMEOPTION->GetSkillQueueing();
	GAMEOPTION->SetSkillQueueing(bSkillQueueing);
	
	// 순서에 유의!!
	// SKILL
	SetSkillTotalInfo( pHeroInfo->m_SkillTotalInfo );
    // EQUIP
	SetEquipItemTotalInfo( pHeroInfo->m_EquiptItemInfo );
	// INVENTORY + TEMPINVENTORY

	SetInventoryTotalInfo( pHeroInfo->m_pItemStream, pHeroInfo->m_wStreamLength );

    //인벤셋팅후에 체크한다.
    ItemManager::Instance()->CheckRiderSummon();

    // 비행모드 초기화
    if (rider())
    {
        rider()->set_spread(false);
    }
    set_spread_wings(false);


	// QUICK 
	SetQuickTotalInfo( pHeroInfo->m_QuickTotalInfo );

	SetHeightVariation(pHeroInfo->m_CharInfo.m_byHeight);

	SetHP(pHeroInfo->m_CharInfo.m_iHP);
	SetMP(pHeroInfo->m_CharInfo.m_iMP);
	SetSD(pHeroInfo->m_CharInfo.m_iSD);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    SetFP(pHeroInfo->m_CharInfo.cur_FP_);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

#ifdef _PCBANG_POLICY_CHANGE_POST_EXPIRED
	SetPCRoomStatus((CHANGE_PCROOM_STATUS::eStatus)pHeroInfo->m_bPCBangStatus);
#endif //_PCBANG_POLICY_CHANGE_POST_EXPIRED

	// 죽은 상태로 다른곳에서 리젠되는일은 없다.
	// 현재 서버로부터 정보가 오지않으므로 일단 1로 세팅
	if (GetHP() == 0)
	{
		SetHP(1);
	}

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    SetSpaItemCode(pHeroInfo->m_spa_item_code);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

	// 이큅관련해서 제대로 입혀준다.
	RefreshAppearRender();

	InventoryDialog* pIventoryDlg = (InventoryDialog*)ItemManager::Instance()->GetItemContainerDialog(SI_INVENTORY);

	pIventoryDlg->ReFreshRenderSlot();
	pIventoryDlg->UpdateQuickPos();

    RefreshBadgeItemEffect();
    RefreshPCBangItemEffect();
    RefreshInvenItemEffect();

    SetHeroPetInfo(pPlayerInfo);

    //에테르 5차개선안
 	  this->ClearEthreInfo();
	  m_EtherInfo.byEtherIndex		= pPlayerInfo->m_bActiveEtherDevice;
	  m_EtherInfo.byEtherChargePos	= pPlayerInfo->m_byEtherBulletPos;
 	
	 if (m_EtherInfo.byEtherIndex)
	 {
		SetEtherActive(m_EtherInfo.byEtherIndex);
		SetEtherItemLock(TRUE, TRUE);
		CreateEtherWeaponEffect(false, TRUE);
	 }


    m_pQuickContainer->ReFreshRenderSlot();

#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
    m_pSummonContainer->ReFreshRenderSlot();
#endif

	UpdateSkillAttr();

	m_bHPInfo = TRUE;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    m_bSDInfo = TRUE;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

    // 캐릭터가 생성되는 단계에서 FightEnergy 만큼의 분노버프를 추가해준다
    SetFightingEnergy( pHeroInfo->m_FightEnergy );
    {
        int count = 0;
        while( count < GetFightingEnergy() )
        {
            if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateStatusEffect( 307, 0 ) )
            {
                g_pHero->AddSkillEffect( pEffect );
            }
            ++count;
        }
    }
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	const CLIENT_PLAYER_RANKING_GRADE_INFO* temp_client_player_ranking_grade_info = 
		&pHeroInfo->client_player_ranking_grade_info_;
	if ( temp_client_player_ranking_grade_info )
	{
		SetClientPlayerRankingGradeInfo(temp_client_player_ranking_grade_info);
	}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
}

VOID Hero::GetInventoryTotalInfo(  BYTE * pItemSteam, WORD & wSize )
{
	if(NULL == pItemSteam)
		return;

	SCItemSlotContainer * pItemContainer = (SCItemSlotContainer *)GetSlotContainer(SI_INVENTORY);

	if (pItemContainer)
	{
		wSize = sizeof(RENDER_ITEMSTREAMEX) * (MAX_INVENTORY_SLOT_NUM);

		BitStream stream(
			pItemSteam, 
			wSize, 
			TRUE);
		
		pItemContainer->Serialize( stream, eSLOT_SERIALIZE_CLIENT_LOAD );

		wSize = stream.GetNumberOfBytesUsed();
	}
}

//------------------------------------------------------------------------------
/**
*/
VOID Hero::GetQuickTotalInfo( QUICK_TOTAL_INFO & OUT rQuickInfo )
{
	QUICKSLOT * pQuickSlot = rQuickInfo.m_Slot; 

	rQuickInfo.m_Count = 0;
	
    if (!m_pQuickContainer)
    {
        return;
    }

	POSTYPE start = 0;
	POSTYPE total = MAX_QUICK_SLOT_NUM;

	for (POSTYPE i = start; i < total; ++i)
	{
		if ( !m_pQuickContainer->IsEmpty(i) )
		{
			pQuickSlot[rQuickInfo.m_Count].m_Pos = i;
			((QuickSlot &)m_pQuickContainer->GetSlot(i)).CopyOut( pQuickSlot[rQuickInfo.m_Count].m_Stream );

			++rQuickInfo.m_Count;			
		}
	}
}


//------------------------------------------------------------------------------
/** 게임도중에 특정이유로 인해서 인벤토리 사이즈가 변경된경우 사용.
*/
VOID Hero::ResizeInventoryTotalInfo(BYTE byExtraInventoryTabCount)
{
	if (m_pInventoryContainer)
	{
		GetCharInfo()->m_ExtraInventoryTabCount = byExtraInventoryTabCount;
#ifndef __NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
        m_pInventoryContainer->Resize( MAX_INVENTORY_SLOT_PER_TAB*(MAX_INVENTORY_TAB_NUM + GetCharInfo()->m_ExtraInventoryTabCount) );
#endif
	}
}

//------------------------------------------------------------------------------
/**
*/
VOID Hero::SetInventoryTotalInfo( const INVENTORY_TOTAL_INFO & ItemInfo )
{
	const ITEMSLOTEX * pSlot = ItemInfo.m_Slot;
	POSTYPE start = 0;
	POSTYPE total = ItemInfo.m_InvenCount;

    if (m_pInventoryContainer)
    {
	    POSTYPE MaxInvenCnt = m_pInventoryContainer->GetMaxSlotNum();
	    for(POSTYPE i = start; i < MaxInvenCnt; ++i)
	    {
		    if(!m_pInventoryContainer->IsEmpty(i))
			    m_pInventoryContainer->DeleteSlot(i, NULL);
	    }

	    for(POSTYPE i = start; i < total; ++i)
	    {
		    SCItemSlot InvenSlot(pSlot[i].m_Stream);

		    assert(pSlot[i].m_Pos < m_pInventoryContainer->GetMaxSlotNum());
		    if (pSlot[i].m_Pos < m_pInventoryContainer->GetMaxSlotNum())
		    {
			    m_pInventoryContainer->InsertSlot(pSlot[i].m_Pos, InvenSlot);
    	    }		
	    }
    }
}

//------------------------------------------------------------------------------
/**
*/
VOID Hero::SetInventoryTotalInfo( const BYTE * pItemStream, WORD wSize )
{
	BitStream stream( (BYTE*)pItemStream, wSize, FALSE );
	assert(m_pInventoryContainer);
	if ( m_pInventoryContainer )
	{
	#ifndef __NA_000678_20071023_MEMORY_OPTIMIZATION_SLOTCONTAINER
		m_pInventoryContainer->Resize( MAX_INVENTORY_SLOT_PER_TAB*(MAX_INVENTORY_TAB_NUM +GetCharInfo()->m_ExtraInventoryTabCount) );
	#endif		
		((SCItemSlotContainer *)m_pInventoryContainer)->Serialize( stream, eSLOT_SERIALIZE_CLIENT_SAVE );
	}	
}


//------------------------------------------------------------------------------
/**
*/
VOID Hero::SetQuickTotalInfo( const QUICK_TOTAL_INFO & rQuickInfo )
{
    //! 2011.6.2 / i4u4me / 스킬 트리에 정보 재 셋팅 
    uiSkillMan* ui_skill_man = GET_DIALOG_MANAGER(uiSkillMan, UIMAN_SKILL);
    if (ui_skill_man != NULL)
    {
        xGetSkillData()->ResetTreeData();
        ui_skill_man->skill_tree()->RefreshSkillSlot();
    }

	const QUICKSLOT * pSlot = rQuickInfo.m_Slot;
	POSTYPE start = 0;
	POSTYPE total = rQuickInfo.m_Count;
	for(POSTYPE i=start;i<total;++i)
	{
		QuickSlot TempQuickSlot(pSlot[i].m_Stream);
		
		switch (TempQuickSlot.GetOrgSlotIndex())
		{
		case SI_EQUIPMENT:
			_asm nop;
		case SI_INVENTORY:
			{

				ItemManager::Instance()->QuickLinkItem(
					TempQuickSlot.GetOrgSlotIndex(), 
					TempQuickSlot.GetOrgPos(), 
					pSlot[i].m_Pos, 
					TempQuickSlot.GetOrgCode());
			}
			break;

		case SI_SKILL:
			{
				ItemManager::Instance()->QuickLinkSkill(
					TempQuickSlot.GetOrgSlotIndex(), 
					TempQuickSlot.GetOrgCode(), 
					pSlot[i].m_Pos);
			}
			break;

		default:
			assert(0);
		break;
		}	
	}

	ItemManager::Instance()->UpdateQuickInfo();
	QuickContainer * pQuickContainer = g_pHero->GetQuickContainer();
	if (pQuickContainer)
	{
		pQuickContainer->ReFreshRenderSlot();
	}	
}


//------------------------------------------------------------------------------
/**
*/
VOID Hero::SetStyleTotalInfo( const STYLE_TOTAL_INFO & IN rStyleInfo )
{
	const STYLESLOT * pSlot = rStyleInfo.m_Slot;
	POSTYPE start = 0;
	POSTYPE total = rStyleInfo.m_Count;
	for(POSTYPE i=start;i<total;++i)
	{		
		ItemManager::Instance()->StyleLink(
			pSlot[i].m_Stream.Part.Code, 
			pSlot[i].m_Pos);
	}
}

//------------------------------------------------------------------------------
/**
*/
VOID Hero::UpdateTotalItemDate()
{
}


//------------------------------------------------------------------------------
/**
*/
VOID Hero::SetSkillTotalInfo( const SKILL_TOTAL_INFO & IN SkillInfo )
{
    g_InterfaceManager.SetSkillInfos(SkillInfo);

    UpdateSkillAttr();
}
//------------------------------------------------------------------------------
/**
	
*/
BOOL Hero::CanUseStyle()
{
	if(IsTransForm())
		return FALSE;

	if( GetClass() == eCHAR_BERSERKER || GetClass() == eCHAR_DRAGON)
	{
		return TRUE;
	}		

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Hero::GetWeaponKind()
{
	HeroEquipmentContainer * pEquipContainer = (HeroEquipmentContainer *)ItemManager::Instance()->GetItemContainerDialog(SI_EQUIPMENT);

	if(NULL == pEquipContainer)
	{
		assert(pEquipContainer && "pEquipContainer == NULL");
		return eWEAPONTYPE_INVALID;
	}

	SCSlot & rSlot = pEquipContainer->GetSlot(EquipmentContainer::EQUIP_WEAPON1);

	// 빈손이면~
	if (!rSlot.GetCode())
	{
		return eWEAPONTYPE_PUNCH;
	}
	else
	{
		BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rSlot.GetCode());
		
		if(pItemInfo)
		{
            // 비활성화에 대한 고려를 추가
            ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = GetEquipmentContainer()->GetEquipmentManageInfo();
            if (equip_slot_manage_info && 
                equip_slot_manage_info->slots[EquipmentContainer::EQUIP_WEAPON1].activated == false)
            {
                return eWEAPONTYPE_PUNCH;
            }

			return pItemInfo->m_wType;
		}
		else
		{
#ifdef _DEV_VER
			TCHAR szDebugMsg[1024] = {0,};
			Sprintf(szDebugMsg, _T("장착된 무기 ID가 잘못되었습니다[%d]"), rSlot.GetCode());
			GlobalFunc::OutputMsg_SysMsg(szDebugMsg);
#endif
			return eWEAPONTYPE_PUNCH;
		}
		
	}
}



//----------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseSkill(SLOTCODE skillID,BOOL bShowFailMessage)
{
	if(IsLockSkill())
		return FALSE;

    //소환수가 있고 
	if(GetSummonId()!=NONE_ID)
	{
		Monster *pMon=(Monster *)g_ObjectManager.GetObject(GetSummonId());
		if(pMon)
		{
			BASE_NPCINFO *pInfo=pMon->GetMonsterInfo();
			if(pInfo)
			{
				for(int i=0;i<BASE_NPCINFO::MAX_SKILL_NUM;i++)
				{
					BASE_SKILLINFO *pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(pInfo->m_wSkillCode[i]);
					BASE_SKILLINFO *pSkillInfo2 = SkillInfoParser::Instance()->GetSkillInfo(skillID);

					if(pSkillInfo&&pSkillInfo2&&pSkillInfo->m_SkillClassCode==pSkillInfo2->m_SkillClassCode)
					{
						return pMon->CanUseSkill(skillID,bShowFailMessage);
					}
				}
			}
		}
	}

	//BOOL bCheck = FALSE;
	BOOL bCheck = CanUseSkillType(skillID, bShowFailMessage);
	if (bCheck == FALSE)
	{
		Monster *pMon=(Monster *)g_ObjectManager.GetObject(GetSummonId());
		if(pMon)
		{
			BASE_NPCINFO *pInfo=pMon->GetMonsterInfo();
			if(pInfo)
			{
				for(int i=0;i<BASE_NPCINFO::MAX_SKILL_NUM;i++)
				{
					if(pInfo->m_wSkillCode[i]==skillID)
					{
						return pMon->CanUseSkill(skillID,bShowFailMessage);
					}
				}
			}
		}

        return FALSE;
	}

	//if( CanUseSkillType(skillID, bShowFailMessage) == FALSE )
	//	return FALSE;

	if( GENERALPARAM->IsNet()==FALSE)
		return TRUE;
		
	if( CanUseSkillLimitWeapon(skillID, bShowFailMessage) == FALSE )
		return FALSE;

	if( CanUseSkillLimitLevel( skillID, bShowFailMessage) == FALSE )
		return FALSE;

	if( CanUseSkillLimitMP(skillID, bShowFailMessage) == FALSE )
		return FALSE;
	
	if( CanUseSkillLimitHP(skillID, bShowFailMessage) == FALSE )
		return FALSE;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if ( CanUseSkillLimitFP(skillID, bShowFailMessage) == FALSE )
        return FALSE;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	
	if( (eACTION_CRYSTAL_WARP_DESTROY + eACTION_START_SKILL_CODE) == skillID )
	{
		if( CanUseSkillLimitCoolTime(eACTION_CRYSTAL_WARP_CREATE + eACTION_START_SKILL_CODE, bShowFailMessage) == FALSE )
			return FALSE;
	}
	else
	{
		if( CanUseSkillLimitCoolTime(skillID, bShowFailMessage) == FALSE )
			return FALSE;
	}

	if( CanUseSkillSlotState(skillID, bShowFailMessage) == FALSE )
		return FALSE;

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    if (GlobalFunc::IsSpaScene() == TRUE)
    {
        // 70534 스킬을 사용할 수 없는 장소입니다.
        GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString(70534));
        return FALSE;
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

	return TRUE;
}

BOOL
Hero::CanUseSkillWithoutCooltime(SLOTCODE skillID, BOOL bShowFailMessage /*= FALSE*/, bool is_check_monster)
{
    //------------------------------------------------------------------------------ 
    if (is_check_monster == true)
    {
        //소환수가 있고 
        if(GetSummonId()!=NONE_ID)
        {
            Monster *pMon=(Monster *)g_ObjectManager.GetObject(GetSummonId());
            if(pMon)
            {
                BASE_NPCINFO *pInfo=pMon->GetMonsterInfo();
                if(pInfo)
                {
                    for(int i=0;i<BASE_NPCINFO::MAX_SKILL_NUM;i++)
                    {
                        BASE_SKILLINFO *pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(pInfo->m_wSkillCode[i]);
                        BASE_SKILLINFO *pSkillInfo2 = SkillInfoParser::Instance()->GetSkillInfo(skillID);

                        if(pSkillInfo&&pSkillInfo2&&pSkillInfo->m_SkillClassCode==pSkillInfo2->m_SkillClassCode)
                        {
                            return pMon->CanUseSkillWithoutCooltime(skillID,bShowFailMessage);
                        }
                    }
                }
            }
        }

        BOOL bCheck = CanUseSkillType(skillID, bShowFailMessage);
        if (bCheck == FALSE)
        {
            Monster *pMon=(Monster *)g_ObjectManager.GetObject(GetSummonId());
            if(pMon)
            {
                BASE_NPCINFO *pInfo=pMon->GetMonsterInfo();
                if(pInfo)
                {
                    for(int i=0;i<BASE_NPCINFO::MAX_SKILL_NUM;i++)
                    {
                        if(pInfo->m_wSkillCode[i]==skillID)
                        {
                            return pMon->CanUseSkillWithoutCooltime(skillID,bShowFailMessage);
                        }
                    }
                }
            }

            return FALSE;
        }
    } //if (is_check_monster == true)
    //------------------------------------------------------------------------------ 


    //------------------------------------------------------------------------------ 
	BOOL bCheck = FALSE;
	
	bCheck = CanUseSkillType(skillID, bShowFailMessage);
	if ( !bCheck )
	{
		return FALSE;
	}

	if( GENERALPARAM->IsNet()==FALSE)
		return TRUE;

	
	bCheck = CanUseSkillLimitWeapon(skillID, bShowFailMessage);
	if ( !bCheck )
	{
		return FALSE;
	}

	bCheck = CanUseSkillLimitLevel(skillID, bShowFailMessage);
	if ( !bCheck )
	{
		return FALSE;
	}

	bCheck = CanUseSkillLimitMP(skillID, bShowFailMessage);
	if ( !bCheck )
	{
		return FALSE;
	}
	
	bCheck = CanUseSkillLimitHP(skillID, bShowFailMessage);
	if ( !bCheck )
	{
		return FALSE;
	}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    bCheck = CanUseSkillLimitFP(skillID, bShowFailMessage);
    if ( !bCheck )
    {
        return FALSE;
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	bCheck = CanUseSkillSlotState(skillID, bShowFailMessage);
	if ( !bCheck )
	{
		return FALSE;
	}

	return TRUE;
}
//----------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseSkillLimitMP(SLOTCODE skillID, BOOL bShowFailMessage /*= FALSE*/)
{
	TCHAR szFailMessage[512];

	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(skillID);
	assert(pInfo);

	if (!pInfo)
	{
		return FALSE;
	}


	if(!m_pPlayerAttriburtes)
		return FALSE;

	int iValue=m_pPlayerAttriburtes->GetMPSpendIncrease();
	float fRatio = float(m_pPlayerAttriburtes->GetRatioMPSpendIncrease())/100.0f;

	// 쿨타임이 아직 안지났으면 못쓴다~

	int iMPSend = int( pInfo->m_wMPSpend * (1.0f+fRatio) + iValue );
	iMPSend=max(0,iMPSend);

	if(GetMP()<DWORD(iMPSend))
	{
		if (bShowFailMessage)
		{
			if ((GetClass() == eCHAR_BERSERKER) || (GetClass() == eCHAR_DRAGON))
			{			
				g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NOT_ENOUGH_SP,szFailMessage,INTERFACE_STRING_LENGTH);
			}
			else
			{
				g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NOT_ENOUGH_MP,szFailMessage,INTERFACE_STRING_LENGTH);

			}
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);

			g_HeroInput.PlayNotEnoughManaSound();
		}

		return FALSE;
	}


	return TRUE;
}
//----------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseSkillLimitLevel(SLOTCODE skillID, BOOL bShowFailMessage /*= FALSE*/)
{
	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(skillID);
	assert(pInfo);
	if (!pInfo)
	{
		return FALSE;
	}

	if (pInfo->m_wRequireLv > GetCharInfo()->m_LV)
	{

		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------------------
bool Hero::CanUseACItem(const BASE_ITEMINFO *pItemInfo)
{
    return true;
}
//------------------------------------------------------------------------------
BOOL Hero::CanUseItem(const BASE_ITEMINFO *pItemInfo,BOOL bShowFailMessage)
{
	if (!pItemInfo)
	{
		return FALSE;
	}

	if (pItemInfo->m_wACCode && pItemInfo->m_wType == eITEMTYPE_AC )
	{
        if (CanUseACItem(pItemInfo) == false)
        {
            TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
            //5124	이 AC 아이템은 더 이상 사용할 수 없습니다.
            g_InterfaceManager.GetInterfaceString(5124 , szMessage ,INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            return FALSE;
        }
	}

	util::Timer *pTimer = xGetCooltimeManager()->GetItemCooltime(pItemInfo->m_wCoolTimeType);

	// 타이머가 없으면 쿨타임 적용 할필요 없는 아이템이다.
	if (!pTimer) 
		return TRUE;

	if (!pTimer->IsExpired(FALSE))
	{
		if (bShowFailMessage)
		{
			TCHAR szFailMessage[INTERFACE_STRING_LENGTH];
			g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_COOLTIME_NOT_EXPIRED,szFailMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);			
		}
		return FALSE;
	}
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    else
    {
        if (pItemInfo->m_byWasteType == eITEMWASTE_HPPOTION || 
            pItemInfo->m_byWasteType == eITEMWASTE_MPPOTION || 
            pItemInfo->m_byWasteType == eITEMWASTE_HPMP_POTION) // 포션기능
        {
            // 쿨타임 시작
            // 툴팁에서도 적용시켜 줘야 한다.
            if (g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_RECOVER_POTION_COOLTIME_RATIO) != 0)
            {
                xGetCooltimeManager()->StartItemCooltime(pItemInfo->m_wCoolTimeType, 
                    0, 
                    g_pHero->GetPlayerAttribute()->GetAttrValue(eATTR_RECOVER_POTION_COOLTIME_RATIO));
            }
        }
    }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

	return TRUE;
}

BOOL Hero::CanUseChangeGenderItem()
{
    // 코스튬 아이템이 장착중이면 성별을 변경할 수 없다.
    if( GetCostumeItemCode() != 0 )
    {
        return false;
    }

    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return FALSE;
    }

    POSTYPE container_size = equipment_container->GetEmptySlotNum();
    for (POSTYPE slot_index = 0; slot_index < container_size; ++slot_index)
    {
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(equipment_container->GetSlot(slot_index));
        if (item_slot.IsContainValidItem() == FALSE)
        {
            continue;
        }

        if (item_slot.GetCode() != 0)
        {
            return false;
        }
    }
    return true;
}

BOOL CheckSkillClassDefine( DWORD SkillClassDefine, eCHAR_TYPE CharacterClass, eTRANSFORM_TYPE TransformType )
{
    if( SkillClassDefine == 0 )
        return TRUE;

    if( CharacterClass == eCHAR_DRAGON )
    {
        if( TransformType == eTRANSFORM_TYPE_NONE )
        {
#ifdef _NA_0_20120104_DRAGON_COMMON_SKILL
            if( SkillClassDefine == eDRAGON_COMMON_SKILL_1stTRANS )
                return TRUE;
            if( SkillClassDefine == eDRAGON_COMMON_SKILL_2ndTRANS )
                return TRUE;
#endif // _NA_0_20120104_DRAGON_COMMON_SKILL
            if( SkillClassDefine == CharacterClass )
                return TRUE;
            if( SkillClassDefine - 10 == CharacterClass )
                return TRUE;
        }
        else if( TransformType == eTRANSFORM_TYPE_DRAGON1 )
        {
#ifdef _NA_0_20120104_DRAGON_COMMON_SKILL
            if( SkillClassDefine == eDRAGON_COMMON_SKILL_1stTRANS )
                return TRUE;
#endif //_NA_0_20120104_DRAGON_COMMON_SKILL
            if( SkillClassDefine == eTRANSFORM_TYPE_DRAGON1 )
                return TRUE;
        }
        else if( TransformType == eTRANSFORM_TYPE_DRAGON2 )
        {
#ifdef _NA_0_20120104_DRAGON_COMMON_SKILL
            if( SkillClassDefine == eDRAGON_COMMON_SKILL_2ndTRANS )
                return TRUE;
#endif //_NA_0_20120104_DRAGON_COMMON_SKILL
            if( SkillClassDefine == eTRANSFORM_TYPE_DRAGON2 )
                return TRUE;
        }
    }
    else if( SkillClassDefine >= 10 && SkillClassDefine < 50 )    // 체인지업은 eCHAR_TYPE 보다 10 크다
    {
        if( SkillClassDefine - 10 == CharacterClass )
            return TRUE;
    }
    else if( SkillClassDefine < 50 )
    {
        if( SkillClassDefine == CharacterClass )
            return TRUE;
    }

    return FALSE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseSkillType(SLOTCODE skillID,BOOL bShowFailMessage /*= FALSE*/)
{
	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(skillID);
	assert(pInfo);
	if (!pInfo)
	{
		return FALSE;
	}

	if(GetSummonId()!=NONE_ID)
	{
		Monster *pMon=(Monster *)g_ObjectManager.GetObject(GetSummonId());

		if(pMon)
		{
			BASE_NPCINFO *pNpcInfo=pMon->GetMonsterInfo();
			if (pNpcInfo)
			{
				if(pNpcInfo->m_wClass==pInfo->m_dwClassDefine)
					return TRUE;
			}
		}
	}

    if( CheckSkillClassDefine( pInfo->m_dwClassDefine, GetClass(), GetTransformData().GetTransFormType() ) == FALSE )
        return FALSE;

    return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseEmotion(SLOTCODE skillID)
{

	if(IsTransForm())
	{
		return FALSE;
	}

	return	TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseAction(SLOTCODE skillID)
{
    DWORD action_code = skillID - eACTION_START_SKILL_CODE;

    if (IsTransForm() == TRUE)
    {
        switch (action_code)
        {
        case eACTION_SIT_AND_STAND:
        case eACTION_VENDOR:
            return FALSE;
        }
    }

    return	TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseSkillSlotState(SLOTCODE skillID, BOOL bShowFailMessage /*= FALSE*/)
{
    if (!GENERALPARAM->IsNet())
    {
        return TRUE;
    }


    if( SkillData* skill_data = xGetSkillData()->GetSkillDataSet().GetSkillDataBySkillCode( skillID ) )
    {
        if( skill_data->IsActiveState() == true )
        {
            return TRUE;
        }
    }

    if( SkillData* skill_data = xGetSkillData()->GetSkillDataSetForTree().GetSkillDataBySkillCode( skillID ) )
    {
        if( skill_data->IsActiveState() == true )
        {
            return TRUE;
        }
    }

    if (skillID >= eACTION_START_SKILL_CODE)
    {
        // 액션에 등록된 스킬 - 해당스킬은 상태체크하지 않고 통과시킨다.
        // 액션스킬은 상태처리를 하지 않음 [3/31/2011 iforall]
        return TRUE;
    }

    // 초기화나 기타 이유에 따라 현재는 사용 될수 없는 스킬이당.
    if (bShowFailMessage)
    {			
        GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( 5073 ) );
    }

    return FALSE;

}


//----------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseSkillLimitWeapon(SLOTCODE skillID,BOOL bShowFailMessage /*= FALSE*/)
{
	TCHAR szFailMessage[512];
	TCHAR szTemp[512];
	TCHAR szText[512];

	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(skillID);
	assert(pInfo);
	if (!pInfo)
	{
		return FALSE;
	}

	// 모든무기가 다 사용할수 있는 스킬이 아니라면!
	if (pInfo->m_iWeaponDefine[0] != -1)
	{
		// 무기 사용조건을 체크하여 사용할수없으면 리턴
		const int	iWeaponName = 30000;
		BOOL bPass = FALSE;
		DWORD dwCurWeaponKind = GetWeaponKind();
		// 무기가 없으면 FALSE;
		if (dwCurWeaponKind == 0)
		{
			if (bShowFailMessage)
			{
				g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NOT_PROPER_WEAPON,szTemp,INTERFACE_STRING_LENGTH);
				g_InterfaceManager.GetInterfaceString( (iWeaponName + pInfo->m_iWeaponDefine[0]), szText, INTERFACE_STRING_LENGTH );
				Sprintf( szFailMessage, szTemp, szText );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);
			}

			return FALSE;
		}

		for (int a = 0; a < 4; a++)
		{
			if (dwCurWeaponKind == pInfo->m_iWeaponDefine[a])
			{
				bPass = TRUE;
				break;
			}


		}

		if (!bPass)
		{
			if (bShowFailMessage)
			{
				g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NOT_PROPER_WEAPON,szTemp,INTERFACE_STRING_LENGTH);
				g_InterfaceManager.GetInterfaceString( (iWeaponName + pInfo->m_iWeaponDefine[0]), szText, INTERFACE_STRING_LENGTH );
				Sprintf( szFailMessage, szTemp, szText );
				GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);
			}
			return FALSE;
		}
	}

	return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
BOOL Hero::CanUseSkillLimitFP( SLOTCODE skillID,BOOL bShowFailMessage /*= FALSE*/ )
{    
    TCHAR fail_message[INTERFACE_STRING_LENGTH];

    BASE_SKILLINFO* base_skill_info_ptr = SkillInfoParser::Instance()->GetSkillInfo(skillID);
    assert(base_skill_info_ptr);

    if (!base_skill_info_ptr)
    {
        return FALSE;
    }

    // 여기다가 어빌리티 348에 대한 처리를 해주어야 할 것 같다.
    SkillScriptInfo* skill_info_ptr = SkillInfoParser::Instance()->GetSkillInfo(static_cast<SLOTCODE>(skillID));
    BASE_ABILITYINFO* ability_info = NULL;
    if ( skill_info_ptr )
        ability_info = skill_info_ptr->GetAbilityInfoByIndex(0);
    
    if ( ability_info && 
        (ability_info->m_wAbilityID == eABILITY_FP_CHANGE || ability_info->m_wAbilityID == eABILITY_FP_TOGGLE)
        && ability_info->m_iParam[1] != 0)
    {
        int need_fp = ability_info->m_iParam[1]; // 소모 FP
        need_fp = static_cast<int>(this->GetFP()) + need_fp;
        if (need_fp < 0)
        {
            TCHAR skill_name[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(skill_info_ptr->m_dwSkillNcode, skill_name, INTERFACE_STRING_LENGTH);
            
            if ( bShowFailMessage )
            {
                // 2924 : FP가 부족합니다.
                g_InterfaceManager.GetInterfaceStringFormat(fail_message,
                    INTERFACE_STRING_LENGTH,
                    2924,
                    skill_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, fail_message);

                g_HeroInput.PlayNotEnoughFPSound();
            }

            return FALSE;
        }
    }    

    return TRUE;
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
//-------------------------------------------------------------------------------------------
/**
*/
BOOL Hero::CanUseStyleLimitWeapon(SLOTCODE skillID, BOOL bShowFailMessage)
{
	BASE_STYLEINFO * pInfo = SkillInfoParser::Instance()->GetStyleInfo( skillID );
	assert(pInfo);
	if (!pInfo)
	{
		return FALSE;
	}

	DWORD dwCurWeaponKind = GetWeaponKind();

	if ( dwCurWeaponKind == pInfo->m_iWeaponDefine )
	{
		return TRUE;
	}

	return FALSE;
}

//----------------------------------------------------------------------------
/**
*/
int Hero::AddSkillEffect(SKILL_EFFECT *pEffect)
{
	int Result = Player::AddSkillEffect(pEffect);

    UpdateSkillAttr();

    if (Result != 0)
    {
        //하이드예약상태가 아닐때만:마을에 입장,포탈이동시 하이드상태로 변경해준다.
        if (!GetHideReserve() && (pEffect->dwAbilityID == eABILITY_HIDE))
        {
            SetHideSkill(TRUE);
        }
    }

	return Result;
}

BOOL Hero::DeleteSkillEffect(int iEffectID)
{
    BOOL Result = Player::DeleteSkillEffect(iEffectID);

    UpdateSkillAttr();	
    return Result;
}

//----------------------------------------------------------------------------
/**
*/
eTRIGGER_STATE Hero::GetTriggerState( DWORD iAreaID  ) 
{
	TRIGGERSTATE_MAP::iterator iter;

	iter = m_MapTriggerState.find(iAreaID);

	
	if (iter == m_MapTriggerState.end())
	{
		return TRIGGER_STATE_NONE;
	}

	return (iter->second); 
}


//----------------------------------------------------------------------------
/**
*/
void Hero::SetTriggerState( DWORD iAreaID,eTRIGGER_STATE TriggerState ) 
{
	TRIGGERSTATE_MAP::iterator iter;
	iter = m_MapTriggerState.find(iAreaID);
	
	if (iter != m_MapTriggerState.end())
	{
		iter->second = TriggerState;
	}
	// 없으면 추가.
	else
	{

		m_MapTriggerState.insert(TRIGGERSTATE_PAIR(iAreaID,TriggerState));
	}
}


//----------------------------------------------------------------------------
/**
*/
eSHORTCUT_AREA_STATE Hero::GetShortcutState( DWORD dwAreaID  ) 
{
	SHORTCUT_AREA_STATE_MAP::iterator iter;	
	iter = this->m_MapShorcutAreaState.find(dwAreaID);
	
	if (iter == m_MapShorcutAreaState.end())
	{
		return SHORTCUT_AREA_STATE_NONE;
	}

	return (iter->second); 
}


//----------------------------------------------------------------------------
/**
*/
void						
Hero::SetShortcutState( DWORD dwAreaID, eSHORTCUT_AREA_STATE ShortcutState )
{
	SHORTCUT_AREA_STATE_MAP::iterator iter;
	iter = this->m_MapShorcutAreaState.find(dwAreaID);
	
	if (iter != m_MapShorcutAreaState.end())
	{
		iter->second = ShortcutState;
	}
	// 없으면 추가.
	else
	{
		m_MapShorcutAreaState.insert(SHORTCUT_AREA_STATE_PAIR(dwAreaID,ShortcutState));
	}
}
	
void Hero::InitTriggerState()
{
	m_MapTriggerState.clear();
}

void Hero::InitShortcutState()
{
	m_MapShorcutAreaState.clear();
}

float Hero::GetAttackSpeedModifier()
{

#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
    const float fWeaponMultiplier = 0.67f;

    if (m_bCustomAttackSpeed)
    {
        return m_fCustomAttackSpeed;
    }
    else
    {
        return Player::GetAttackSpeedModifier();
    }

#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
    const float fWeaponMultiplier = 0.67f;

    if (m_bCustomAttackSpeed)
    {
        return m_fCustomAttackSpeed;
    }
    else
    {
        return ((float)m_pPlayerAttriburtes->GetAttSpeedRatio() / 100.0f) * fWeaponMultiplier;		
    }

#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
}

float Hero::GetMoveSpeedModifier()
{
    //GM속도무시한다 무조건 라이더(서버가원함)
    if (IsRiding())
    {
        return GetPlayerAttribute()->GetRiderSpeedRatio()/100.0f;
    }

	if (m_bCustomMoveSpeed)
	{
		return m_fCustomMoveSpeed;
	}

#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
    return Player::GetMoveSpeedModifier();

#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER

#ifdef _NA_000000_20120410_SPEEDHACK
    _asm int 3;
#else 
    // 값이 너무 작으면 보정
    if (m_pPlayerAttriburtes->GetMoveSpeedRatio() <= 0.0f)
    {
        return 0.0f;
    }
    else if (m_pPlayerAttriburtes->GetMoveSpeedRatio() < 1.0f)
    {
        return 0.01f;
    }

    return ((float)m_pPlayerAttriburtes->GetMoveSpeedRatio() / 100.0f);
#endif //_NA_000000_20120410_SPEEDHACK

#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER


}

DWORD Hero::GetSD()
{
	return Player::GetSD();
}

DWORD Hero::GetMaxSD()
{
	return m_pPlayerAttriburtes->GetMaxSD();
}


DWORD Hero::GetMaxHP()
{
	return m_pPlayerAttriburtes->GetMaxHP();
}


DWORD Hero::GetMaxMP()
{
	return m_pPlayerAttriburtes->GetMaxMP();
}

void Hero::SetCustomAttackSpeed(float fSpeed)
{
	m_bCustomAttackSpeed = TRUE;
	m_fCustomAttackSpeed = fSpeed;

}

void Hero::SetCustomMoveSpeed(BOOL bUse,float fSpeed)
{
	m_bCustomMoveSpeed = bUse;
	m_fCustomMoveSpeed = fSpeed;
}

void Hero::SetCannotMoveFlag(BOOL bFlag)
{
	m_bCannotMove = bFlag;

	m_iCanNotMoveTime=0;
}

BOOL Hero::GetCannotMoveFlag()
{
	return m_bCannotMove;
}

BYTE Hero::GetPCBangMode()
{
    BYTE mode = Player::GetPCBangMode();
    if( mode != 0 ) 
        return mode;

    if( xGetHeroData()->GetPCBangItemData().IsActive() )
        return 1;

    return 0;
}


void Hero::UpdateSkillAttr()
{
	if (!GetPlayerAttribute())
	{
		return;
	}

	SkillAttrCalculator SkillCalc( *GetPlayerAttribute() );

	SkillCalc.Clear();
	
	UpdateActiveSkillEffectAttr(SkillCalc);
	GetPlayerAttribute()->Update();

	if (GetTransSkillCode())
    {
		SkillCalc.ApplyDragonFormation(TRUE, (SLOTCODE)m_dwTransSkillCode, 0);
    }

	if (GetEthreActive() && (m_EtherInfo.iValue > 0))
	{
		SkillCalc.AddAttr(eATTR_ATTACK_SPEED, VALUE_TYPE_PERCENT_PER_CUR, m_EtherInfo.iValue);
	}

	UpdatePetSkillAttr(SkillCalc);

#ifdef _NA_004035_20120227_COSTUME_ITEM
    UpdateCostumeInfo(SkillCalc);
#endif //_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	UpdateDecorateCostumeInfo(SkillCalc);
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

    UpdateBadgeItemInfo(SkillCalc);

	UpdateDominationRewardAttr(SkillCalc);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    UpdateGuildFacilityAttr(SkillCalc);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    uiPerkDialog* perk_dialog = GET_CAST_DIALOG(uiPerkDialog, IM_SKILL_MANAGER::SKILL_PERK_DLG);
    if (perk_dialog != NULL)
    {
        perk_dialog->UpdatePerkAttribute();
    }
	GetPlayerAttribute()->Update();

    //------------------------------------------------------------------------------ 
    //! 인벤을 돌면서 능력치를 주는 아이템일경우 능력치를 더해준다.
    if (m_pInventoryContainer != NULL)
    {
        POSTYPE total = m_pInventoryContainer->GetMaxSlotNum();

        for (POSTYPE i = 0; i < total; ++i)
        {
            if (m_pInventoryContainer->IsEmpty(i) == TRUE)
            {
                continue;
            }

            SCSlot& slot = m_pInventoryContainer->GetSlot(i);
            if (slot.GetSlotType() != ST_ITEM)
            {
                continue;
            }

            SCItemSlot& item_slot = static_cast<SCItemSlot&>(slot);
            if (item_slot.GetItemInfo()->IsExistInvenOptions() == false)
            {
                continue;
            }

            const bool pc_room_status = (GetPCBangMode() != FALSE);
            PlayerAttributes& player_attr = (*GetPlayerAttribute());
            InvenItemOptionCalculator::ApplyInvenItemOption(player_attr, item_slot, 1, pc_room_status, false);
        }
    }

    if( xGetHeroData()->GetBadgeItemData().IsActive() )
    {
        POSTYPE slot = xGetHeroData()->GetBadgeItemData().GetItemPosition();
        if( slot != INVALID_SLOT_POS )
        {
            if( SCItemSlot* item = ItemManager::Instance()->GetInventoryItem( slot ) )
            {
                InvenItemOptionCalculator::ApplyInvenItemOption((*GetPlayerAttribute()), *item, 1, GetPCBangMode() != 0, false );
            }
        }
    }

    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();

}

void Hero::UpdateDominationRewardAttr(SkillAttrCalculator &SkillCalc)
{
	// 점령전 보상옵션 적용
	uiDominationZoneMarkDlg* pDlg = GET_CAST_DIALOG(uiDominationZoneMarkDlg, IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_ZONEMARK_DIALOG);
	if (pDlg && g_pMap)
	{
        // 선택된 옵션 정보에 따라 옵션적용
        const DominationInfoParser::SelectedRewardOptions& selected_option_info = 
            pDlg->GetSelectedRewardOptionInfo();
        DominationInfoParser::SelectedRewardOptions::const_iterator begin_itr = selected_option_info.begin();
        DominationInfoParser::SelectedRewardOptions::const_iterator end_itr = selected_option_info.end();
        for ( ; begin_itr != end_itr; ++begin_itr)
        {
            const DominationInfoParser::SelectedRewardOption& selected_option = begin_itr->second;
            eATTR_TYPE attr_type = g_ITEM_OPTION[selected_option.option_info->m_wOptionKind];
            SkillCalc.AddAttr(attr_type, 
                selected_option.option_info->m_byOptionType, 
                selected_option.option_info->m_iOptionValue);
        }
	}
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
void Hero::UpdateGuildFacilityAttr( SkillAttrCalculator& skill_calc )
{
    // 패시브 적용
    GuildSystem::GuildSystemData::GuildFacilityContainer& guild_facility_container = 
        GuildSystem::GetGuildSystemData().guild_facility_container();
    GuildSystem::GuildSystemData::GuildFacilityContainer::iterator iter;
    for (iter = guild_facility_container.begin(); iter != guild_facility_container.end(); ++iter)
    {
        if (iter->second.facility_level_ > 0)
        {
            const FACILITY_INFO* facility_parser_info = 
                GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(iter->second.facility_code_, iter->second.facility_level_);
            if (facility_parser_info == NULL)
                continue;

            if (facility_parser_info->passive_option.option_type > eATTR_TYPE_INVALID && 
                facility_parser_info->passive_option.option_type < _countof(g_ITEM_OPTION) )
            {
                skill_calc.AddAttr(g_ITEM_OPTION[facility_parser_info->passive_option.option_type], 
                    facility_parser_info->passive_option.option_value_type, facility_parser_info->passive_option.option_value);
            }
        }
    }
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

void Hero::OnDamage(const DAMAGE_PRAM &rDamage)
{
#ifdef _NA_000000_20130114_RENEWER_UI
#else
	HeroGaugeDialog *pHeroDlg = (HeroGaugeDialog *)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_HEROGAUGE);
	if (pHeroDlg)
	{
		pHeroDlg->MakeHitEffect();
	}
#endif

	Player::OnDamage(rDamage);
}

//----------------------------------------------------------------------------
/**
*/
void Hero::OnLevelUp()
{
    Player::OnLevelUp();

    uiCharacterStatusMan::RefreshSimulationPlayerAttributes();
	
	// 레벨업 메세지
    TCHAR szMessage[MAX_MESSAGE_LENGTH];
	g_InterfaceManager.GetInterfaceString( eST_HERO_LEVEL_UP, szMessage, MAX_MESSAGE_LENGTH );
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

    // 5027 축하합니다! 최고 레벨에 도달하였습니다.
	if( g_pHero && GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st() == g_pHero->GetLevel() )
	{
		g_InterfaceManager.GetInterfaceString(5027, szMessage , INTERFACE_STRING_LENGTH);
		GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_TOP_SHOW | eOUTPUT_MSG_CHAT, szMessage );
	}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    // no operation
#else
    OutputMsg_ForceWithDrawCampSoldier(dwLevel);
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER    

	g_SoundEffect.PlayUI(80);

}

//------------------------------------------------------------------------------
/**
*/
int Hero::GetBonusSkillCooltime() 
{
    return m_pPlayerAttriburtes->GetItemBonusSkillCoolTime(); 
}

//------------------------------------------------------------------------------
/**
*/
int Hero::GetRatioBonusSkillCooltime() 
{
    return m_pPlayerAttriburtes->GetRatioBonusSkillCoolTime(); 
}

//----------------------------------------------------------------------------
/** 아이템 사용과 쿨타임 리셋
*/
void						
Hero::OnUseItemWithCoolTime(CODETYPE ItemCode)
{
	if (!g_pHero)
	{
		return;
	}

	OnUseItem(ItemCode);

	BASE_ITEMINFO* pItemInfo = (BASE_ITEMINFO*)ItemInfoParser::Instance()->GetItemInfo(ItemCode);
	if (pItemInfo)
	{	
		util::Timer* pTimer = xGetCooltimeManager()->GetItemCooltime(pItemInfo->m_wCoolTimeType);
		if (pTimer && (ItemCode != 63038))
		{
			pTimer->Reset();
		}
	}
}


BOOL Hero::OnUseItem(DWORD dwItemCode)
{
	//Todo:아이템 읽어서 인포 타입으로 관리 

	if( GENERALPARAM->IsNet()==FALSE)
	{
		if(dwItemCode==SSQ_PORTAL)
		{
            g_ProjectileManagerEx.CreatePortalForHero();
		}

		if( dwItemCode == 63014 )
		{
			static DWORD dwObjectKey = 250019;
			TOTEMNPC_RENDER_INFO RenderInfo;
			RenderInfo.m_dwObjectKey = dwObjectKey++;
			RenderInfo.m_dwMonsterCode = 64520;
			RenderInfo.m_ItemCode = 63014;
			RenderInfo.m_wvPos = GetVisiblePos();

			g_SsqScene.AddTotemMonsterWithPacket( &RenderInfo, 100 );
		}
	}

	return Player::OnUseItem(dwItemCode);
}

BOOL Hero::Render()
{
	if( BattleScene::GetScreenShotState() == eSCREEN_SHOT_NOT_UI_AND_HERO )
	{
		return TRUE;
	}
	
	return Player::Render();
}

void Hero::RenderInfo()
{
	// Player가 아니라 Object 맞음 위에 이름 안찍기위해 일부러 한것임
	// 카오 일땐 

	if (!IsGM() && (IsChao() || IsAttackBeginPlayer()) || GAMEOPTION->GetShowMyRenderInfo())
    {
		Player::RenderInfo();
    }
	else
    {
		Object::RenderInfo();
    }
	
}

void Hero::OnFootStepPlaySound()
{

}

void Hero::OnWaterWave()
{
	Character::OnWaterWave();
}

void Hero::OnWaterEllipse()
{
	Character::OnWaterEllipse();
}

void Hero::OnFootStep(BOOL b2D /*= FALSE*/)
{
	if (BIT_CHECK(m_dwCurTileAtt, PTA_PLAYER_FALLDOWN_TILE))
    {
		return;
    }

	if (m_bObserverMode)
	{
		return;
	}

	WzVector vPos = GetVisiblePos();

	// 지형속성에 따라 이펙트를 뿌려준다
	switch (m_dwTerrainAttribute)
	{
	default:
	case eTERRAIN_ATTRIBUTE_DIRT:
		{
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A000"), vPos, GetObjectKey());
		}
		break;


	case eTERRAIN_ATTRIBUTE_GRASS:
		{
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A042"), vPos, GetObjectKey());
		}
		break;


	case eTERRAIN_ATTRIBUTE_WATER:
		{
			float fRand = 0.0f;

			GlobalFunc::CreateEffectBeUpToOption( StrToWzID("A002"), vPos, GetObjectKey() );

		}
		break;

	case eTERRAIN_ATTRIBUTE_SNOW:
		{
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A003"), vPos, GetObjectKey());
		}
		break;

	case eTERRAIN_ATTRIBUTE_DESERT:
		{
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A001"), vPos, GetObjectKey());
		}
		break;

	}

	Player::OnFootStep(FALSE);
}

void Hero::SetMaxSD(DWORD dwMaxSD)
{
	// 주인공같은경우는 UpdateAttr에서 자동으로 해준다
	// 고로 아무것도 해주지 말아야한다!
}

void Hero::SetSD(DWORD dwSD)
{
	Player::SetSD(dwSD);
}


void Hero::SetMaxHP( DWORD iHP )
{
	// 주인공같은경우는 UpdateAttr에서 자동으로 해준다
   // 고로 아무것도 해주지 말아야한다!
	return;
}
void Hero::SetMaxMP( DWORD iMP )
{
    // 주인공같은경우는 UpdateAttr에서 자동으로 해준다
   // 고로 아무것도 해주지 말아야한다!
	return;
}
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
void Hero::SetMaxFP( const PLAYERFP& max_fp )
{
    Player::SetMaxFP(max_fp);
}
void Hero::SetFP( const PLAYERFP& fp )
{
    Player::SetFP(fp);
    uiFPGaugeDialog* fp_gauge = GET_CAST_DIALOG(uiFPGaugeDialog, IM_FP_GAUGE_MANAGER::FP_GAUGE_DIALOG);
    if (fp_gauge)
    {
        fp_gauge->SetFPGauge(fp);
    }
}

PLAYERFP Hero::GetFP()
{
    return Player::GetFP();
}

PLAYERFP Hero::GetMaxFP()
{
    return Player::GetMaxFP();
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
void Hero::SetAttackSpeedRatio(int iSpeed)
{
   // 주인공같은경우는 UpdateAttr에서 자동으로 해준다
   // 고로 아무것도 해주지 말아야한다!
	return;
}
void Hero::SetMoveSpeedRatio(int iSpeed)
{
	// 주인공같은경우는 UpdateAttr에서 자동으로 해준다
   // 고로 아무것도 해주지 말아야한다!
	return;
}
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER

//------------------------------------------------------------------------------
#ifdef _NA_000000_20120410_SPEEDHACK
void Hero::SetMoveSpeed(float fMoveSpeed)
{
    _asm int 3;
    move_speed_ = fMoveSpeed;
}
//------------------------------------------------------------------------------
float	Hero::GetMoveSpeed(void)
{
    _asm int 3;
}
#endif //_NA_000000_20120410_SPEEDHACK
//------------------------------------------------------------------------------
BOOL Hero::CanEquipItem()
{
    if ( !g_pHero )
    {
        return FALSE;
    }

	switch (g_pHero->GetCurState())
	{
	case STATE::IDLE:
	case STATE::SIT:
	case STATE::EMOTICON:
	case STATE::MOVE:
	case STATE::KEYBOARDMOVE:
		break;
	default:
		{
			return FALSE;
		}
		break;
	}

	if (m_iValkyrieShotDelay > 0)
	{
		return FALSE;
	}

	if (m_iAttackStandTime > 0)
	{
		return FALSE;
	}

	return TRUE;
}



void Hero::SetHP(DWORD dwHP)
{
	Player::SetHP(dwHP);

#ifdef _NA_000000_20130114_RENEWER_UI
	HeroGaugeDialog2* hero_gauge =(HeroGaugeDialog2*)(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_HEROGAUGE));
	if (hero_gauge != NULL)
	{
		hero_gauge->set_hp_prev_percent((float)GetHP() / (float)GetMaxHP() * 100.0f);
	}
#else
    HeroGaugeDialog* hero_gauge =(HeroGaugeDialog*)(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_HEROGAUGE));
    if (hero_gauge != NULL)
    {
        hero_gauge->set_hp_prev_percent((float)GetHP() / (float)GetMaxHP() * 100.0f);
    }
#endif
}

void Hero::SetMP(DWORD dwHP)
{
	Player::SetMP(dwHP);
}

void Hero::SetHPInfo(HPINFO *pInfo)
{
    // 히어로는 세팅할 필요없다.
}



void Hero::SetAccountState( BOOL bState )
{
	m_bAccountState = bState;
}

BOOL Hero::GetAccountState( void )
{
	return m_bAccountState;
}

void  Hero::HideHelmet(BOOL bFlag,BOOL bRefresh)
{

	Player *pPlayer = (Player *)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
	if (pPlayer) 
	{
		pPlayer->HideHelmet(bFlag,bRefresh);
	}

	Player::HideHelmet(bFlag,bRefresh);
}

#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
void  Hero::HideWing(BOOL bFlag, BOOL bRefresh)
{
    Player *pPlayer = (Player *)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
    if (pPlayer) 
    {
        pPlayer->HideWing(bFlag,bRefresh);
    }

    Player::HideWing(bFlag,bRefresh);
}
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
void Hero::HideCostume( BOOL bFlag,BOOL bRefresh /*= TRUE*/ )
{
    Player *pPlayer = (Player *)g_ObjectManager.GetObject(EQUIPMENT_CHARACTER_KEY);
    if (pPlayer) 
    {
        pPlayer->HideCostume(bFlag,bRefresh);
    }

    Player::HideCostume(bFlag,bRefresh);
}
#endif

void Hero::SetChaoState(BYTE bState)
{
	Player::SetChaoState( bState );
}


BOOL Hero::IsKeboardMoving()
{
	if (BIT_CHECK(m_iMoveFlag,KEYBOARDMOVE_FOWARD)||
		BIT_CHECK(m_iMoveFlag,KEYBOARDMOVE_BACKWARD)||
		BIT_CHECK(m_iMoveFlag,KEYBOARDMOVE_LEFTSTEP)||
		BIT_CHECK(m_iMoveFlag,KEYBOARDMOVE_RIGHTSTEP))
	{
		return TRUE;
	}

	return FALSE;
}

BOOL Hero::IsOldKeboardMoving()
{
	if (BIT_CHECK(m_iOldMoveFlag,KEYBOARDMOVE_FOWARD)||
		BIT_CHECK(m_iOldMoveFlag,KEYBOARDMOVE_BACKWARD)||
		BIT_CHECK(m_iOldMoveFlag,KEYBOARDMOVE_LEFTSTEP)||
		BIT_CHECK(m_iOldMoveFlag,KEYBOARDMOVE_RIGHTSTEP))
	{
		return TRUE;
	}

	return FALSE;
}


void Hero::RefreshAppearRender(BOOL refresh_bone /*= FALSE*/)
{
	Player::RefreshAppearRender(refresh_bone);
	g_InterfaceManager.UpdateMyHeadTexture();
}


BOOL Hero::IsParty( void )
{
	uiPartyMan * partyMan = static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

#ifdef _DH_BATTLEZONE2_
    return (partyMan && (partyMan->GetPartyNum() > 0));
#else
	return ( partyMan && ( partyMan->GetPartyNum() > 1 ) );
#endif//_DH_BATTLEZONE2_
}

void	Hero::SetFreeInitState( BYTE count )
{
    GetCharInfo()->m_byFreeInitCount = count;
}

BYTE	Hero::GetFreeInitState()
{
    return GetCharInfo()->m_byFreeInitCount;
}


BOOL Hero::IsReachMaxLevel()
{
	if( GetCharInfo()->GetChangeOfClass() >= eCHAREX_BERSERKER_100LEVEL)
	{
		//전직 후 최고 레벨에 도달 했는가?
		if( GetLevel() >= GameOptionParser::Instance()->GetMaxLevelChangeOfClass1st())
			return TRUE;
	}
	else
	{
		//전직 하기 전 최고 레벨에 도달 했는가?
		if( GetLevel() >= GameOptionParser::Instance()->GetMaxLevel())
			return TRUE;
	}

	return FALSE;
}

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#else
void Hero::OutputMsg_ForceWithDrawCampSoldier(DWORD dwLevel)
{
	// 훈련병이 29 30 레벨 도달시  훈련병 탈퇴 메세지 출력.
	if( g_pHero->IsExistGuild() && (dwLevel == 29 || dwLevel == 30) )
	{
		uiGuildMan * pGuildMan =  (uiGuildMan *)g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);
		if( pGuildMan )
		{
			stGUILD_MEMBER_INFO * pHeroInfo = pGuildMan->GetGuildMemberInfo(g_pHero->GetName());
			if( pHeroInfo )
			{
				if( pHeroInfo->m_eGuildDuty == eGUILD_DUTY_CAMP_SOLDIER )
				{
					//7198 30레벨이 되면 육성 캠프 졸업이 가능합니다. 31레벨을 달성하면 길드에서 강제탈퇴 됩니다.
					GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, g_InterfaceManager.GetInterfaceString(7198) );
				}
			}
		}
	}
}
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER

bool Hero::IsCrystalWarpKey(DWORD dwKey)
{
	if(dwKey == m_dwCrystalWarpObjKeys[0] || dwKey == m_dwCrystalWarpObjKeys[1])
	{
		return true;
	}

	return false;
}

void Hero::SetCrystalWarpKey(DWORD dwKey, bool bAdd /*= true*/)
{
	if(true == bAdd)
	{
		if(0 == m_dwCrystalWarpObjKeys[0])
			m_dwCrystalWarpObjKeys[0] = dwKey;
		else if(0 == m_dwCrystalWarpObjKeys[1])
			m_dwCrystalWarpObjKeys[1] = dwKey;
		else
		{
			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "SetCrystalWarpKey Add 이상");
		}
	}
	else if(false == bAdd)
	{
		if(dwKey == m_dwCrystalWarpObjKeys[0])
			m_dwCrystalWarpObjKeys[0] = 0;
		else if(dwKey == m_dwCrystalWarpObjKeys[1])
			m_dwCrystalWarpObjKeys[1] = 0;
	}
}

//------------------------------------------------------------------------------ 
bool Hero::SendZONE_EX_QUEST_PORTAL_ENTER_SYN()
{
    MSG_CG_ZONE_EX_QUEST_PORTAL_WITHDRAW_MEMBER_SYN syn;

    syn.portal_key = FixedPortalState.portal_key; 
 
    bool is_res = (SendPacket(&syn, sizeof(syn)) == TRUE);

    FixedPortalState.portal_key = 0;
    FixedPortalState.open_state = MSG_CG_ZONE_EX_QUEST_PORTAL_FIXEDPORTAL_STATEINFO_CMD::eOpenState_None;
    return is_res;
}

#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT

void Hero::ClearActivedComboSkill()
{
    actived_combo_skill_array_.clear();
}

void Hero::AddActivedComboSkill( int skill_class_code, DWORD duration )
{
    if( skill_class_code == 0 )
        return;

    if( duration == 0 )
        return;

//#ifdef _DEBUG
//    // 테스트용은 시간을 10배
//    static int test_value = 10;
//    duration *= 10;
//#endif

    ActivedComboSkillData data;
    data.actived_skill_class_code_ = skill_class_code;
    data.end_time_ = GetCurrentTime() + duration;
    actived_combo_skill_array_.push_back(data);
}

void Hero::UpdateActivedComboSkill()
{
    DWORD current_time = GetCurrentTime();
    std::vector<ActivedComboSkillData>::iterator it = actived_combo_skill_array_.begin();
    while( it != actived_combo_skill_array_.end() )
    {
        if( current_time >= it->end_time_ )
        {
            it = actived_combo_skill_array_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

BOOL Hero::IsActivedComboSkill( SLOTCODE SkillClassCode )
{
    for( size_t i=0;i!=actived_combo_skill_array_.size();++i)
    {
        if( actived_combo_skill_array_[i].actived_skill_class_code_ == SkillClassCode )
            return TRUE;
    }

    return FALSE;
}
#endif

#ifdef _NA_004035_20120227_COSTUME_ITEM
POSTYPE Hero::GetActivedCostumeItemSlotIndex()
{
    return GetCharInfo()->costume_item_pos_;
}

void Hero::SetActivedCostumeItemSlotIndex(POSTYPE inventory_position)
{
    GetCharInfo()->costume_item_pos_ = inventory_position;
}
#endif//_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
POSTYPE Hero::GetActivedDecorateCostumeItemSlotIndex()
{
	return GetCharInfo()->decorate_costume_item_pos_;
}

void Hero::SetActivedDecorateCostumeItemSlotIndex( POSTYPE inventory_position )
{
	GetCharInfo()->decorate_costume_item_pos_ = inventory_position;
}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

void Hero::RefreshBadgeItemEffect()
{
    if( xGetHeroData()->GetBadgeItemData().IsActive()
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        && (GlobalFunc::IsSpaScene() == FALSE)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        )
    {
        if( BASE_ITEMINFO* pIteminfo = ItemInfoParser::Instance()->GetItemInfo(xGetHeroData()->GetBadgeItemData().GetItemCode() ) )
        {
            AddItemEffect( this, pIteminfo, eITEM_EFFECT_TYPE_BADGE );
        }
    }
    else 
    {
        RemoveItemEffect( eITEM_EFFECT_TYPE_BADGE );
    }
}

void Hero::RefreshPCBangItemEffect()
{

}

void Hero::RefreshInvenItemEffect()
{
    if( m_pInventoryContainer == NULL )
    {
        assert(!"Hero::RefreshInvenItemEffect()");
        return;
    }

    POSTYPE start = 0;
    POSTYPE total = m_pInventoryContainer->GetMaxSlotNum();

    int iCurPos = 0;
    for (POSTYPE i = start; i < total; ++i)
    {
        if ( !m_pInventoryContainer->IsEmpty(i) )
        {
            SCSlot& rSlot = m_pInventoryContainer->GetSlot(i);
            if( BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo( rSlot.GetCode() ) )
            {
                if( pItemInfo->IsExistInvenOptions() )
                {
                    g_pHero->AddItemEffect( this, pItemInfo, eITEM_EFFECT_TYPE_INVENTORY );
                }
            }		
        }
    }
}

BYTE Hero::GetAttackPropensity()
{
    BYTE Result = ATTACK_PROPENSITY_NORMAL;

    //pk가능 씬이면
    if(GameFramework::GetCurrentScene() && GameFramework::GetCurrentScene()->IsEnablePkScene() )
    {
        Result = IsForceAttack() ? ATTACK_PROPENSITY_FORCE : ATTACK_PROPENSITY_NORMAL;

        if( Character* pTarget = (Character*)GetTargetObject() )
        {
            // 몬스터를 공격할 경우에는 강제공격을 해제시켜준다
            if( pTarget->IsKindOfObject(MONSTER_OBJECT))
            {
                Monster* pMonster = (Monster*)pTarget;

                Player* pPlayer=(Player*)g_ObjectManager.GetObject(pMonster->GetParentId());
                if( pPlayer == NULL )
                {
                    Result = ATTACK_PROPENSITY_NORMAL;
                }
            }
        }
    }

    return Result;
}

BOOL Hero::SendUseSkillPacket( DWORD SkillCode )
{
    MSG_CG_SKILL_ACTION_SYN	Packet;
    Packet.m_byCategory = CG_SKILL;
    Packet.m_byProtocol = CG_SKILL_ACTION_SYN;		
    Packet.m_SkillCode = (SLOTCODE)SkillCode;

    Packet.m_byAttackPropensity = GetAttackPropensity();
    Packet.m_wvCurPos = GetPosition();		// 2015.12.16 언제부터인지 쓰레기 값을 보내고 있었음 관련 버그가 생겨서 수정 [SUNJP-1770]
    Packet.m_wvDestPos = GetPosition();		// m_wvDestPos 는 스킬사용하는 위치로 사용됨.. 변수명에 속지말것

    Packet.m_dwClientSerial = GetNextAttackSerial();

    // 인자가 NULL이면 기본적인 인자 계산식을 사용.


    SkillScriptInfo* pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(SkillCode);
    if( pSkillInfo == NULL )
        return FALSE;


    // 타겟 없음
    if (pSkillInfo->m_byAttRangeform == SKILL_TARGET_NONE)
    {

        Packet.m_dwMainTargetKey = GetObjectKey();
        Packet.m_wvMainTargetPos = GetPosition();
    }
    else
    {
        if (pSkillInfo->m_byTarget == SKILL_TARGET_ME)
        {
            Packet.m_dwMainTargetKey = GetObjectKey();
            Packet.m_wvMainTargetPos = GetPosition();
        }
        else if( pSkillInfo->m_byTarget == SKILL_TARGET_AREA)
        {
            Packet.m_dwMainTargetKey = GetObjectKey();
            Packet.m_wvMainTargetPos = GetCurrentAction()->SKILL.vTargePos;
        }
        else if (pSkillInfo->m_byTarget == SKILL_TARGET_SUMMON || pSkillInfo->m_byTarget == SKILL_TARGET_AREA_ENEMY_CORPSE)
        {
            Packet.m_dwMainTargetKey = GetObjectKey();
            Packet.m_wvMainTargetPos = GetPosition();
        }
        else
        {
            if( Character* pTarget = (Character*)GetTargetObject() )
            {
                Packet.m_dwMainTargetKey = pTarget->GetObjectKey();					
                Packet.m_wvMainTargetPos = pTarget->GetPosition();

                if (pSkillInfo->m_byAttRangeform == SRF_PIERCE)
                {
                    // 그냥 관통형스킬일 경우 목표 좌표를 넣어준다.
                    WzVector gab = pTarget->GetPosition() - GetPosition();
                    WzVector vDir;
                    VectorNormalize(&vDir, &gab);

                    Packet.m_wvMainTargetPos = GetPosition() + vDir * (pSkillInfo->m_wSkillRange / 10.0f);
                }
            }
            else
            {
                // 타겟형 스킬인데 타겟이 없다
                return FALSE;
            }
        }
    }

    SendRiderReleasePacket();
    SendWingsControlByAccessory(false, false);

    Packet.Encode();
    if( FALSE == SendPacket( &Packet, sizeof(Packet) ) )
    {
        assert(!"스킬 패킷 전송 실패");
        return FALSE;
    }

#ifdef _NA_20120426_DISPLAY_SKILL_COMBO_ICON_EFFECT
    ClearActivedComboSkill();
#endif

    SetCurrentAttackSerial(GetNextAttackSerial());

    return TRUE;

}

