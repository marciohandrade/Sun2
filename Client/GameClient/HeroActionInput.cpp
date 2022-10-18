//------------------------------------------------------------------------------
//  HeroActionInput.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "HeroActionInput.h"
#include "Hero.h"
#include "CooltimeManager.h"
#include "GameFunc.h"
#include "Monster.h"
#include "Character.h"
#include "ObjectManager.h"
#include "TerrainObject.h"
#include "KeyQueueManager.h"
#include "SkillQueueManager.h"
#include "Item.h"


#include "InterfaceManager.h"
#include "ItemManager.h"
#include "item.h"
#include "MouseHandler.h"
#include <SkillInfoParser.h>
#include "NPC.h"
#include "NPCVoiceInfoParser.h"
#include "uitargetman/uitargetman.h"
#include "TargetDialog.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"
#include "SummonTargetDialog.h"
#include "SummonSkillContainer.h"

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
#else
    #include "uiGuildMan/uiGuildMan.h"
    #include "uiGuildMenu/uiGuildMenu.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "quickslot.h"
#include "Map.h"
#include "Camera.h"
#ifdef _NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
    #include "uiBankMan/uiBankDialog.h"
#else
    #include "BankDialog.h"
#endif //_NA_003923_20120130_GUILD_RENEWAL_MOD_WAREHOUSE_SLOT_NUM
#include "MouseCursor.h"
#include "HeroVoiceInfoParser.h"
#include "SoundEffect.h"
#include "NPCPortalDialog.h"
#include "uiAssistMan/uiAssistMan.h"
#include "SkillMain.h"
#include "quickcontainer.h"
#include "uiPartyMember/uiPartyMember.h"
#include "uiHelpMan/uiHelpMan.h"
#include "uiVendorMan/uiVendorMan.h"

#include "SelectCompositeMenu.h"

#include "uiPartyMan/uiPartyMan.h"
#include "CursorChangeTransact.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include "uiSystemMan/uiSystemMan.h"

#include "MapObject.h"
#include "uiWayPointMan/uiWayPointMan.h"
#include "MapEditScene.h"
#include "uiQuickContainerMan/uiQuickContainerMan.h"
#include "uiCommunityDlg/uiCommunityDlg.h"
#include "ChatPersonalOption.h"

#include "uibankman/uibankman.h"
#include "uiRadarMan/uiRadarMan.h"
#include "RadarFullDialog.h"
#include <ssq/SSQInfoStruct.h>
#include "castleavefielddialog.h"

#include "uiChatMan/uiChatMan_def.h"
#include "uiOptionMenuMan/uiOptionMenuMan.h"
#include "Interface/uiPetMan/uiPetMan.h"
#include "Interface/uiPetGauge/uiPetGauge.h"

#include "Clone.h"
#include "CloneManager.h"

#include "uiDominationTowerMan/uiDominationTowerMan.h"

#include "CollectionParser.h"
#include "State/StateCollect.h"

#include "uiBattleScoreMan/uiBattleScoreMan.h"
#include "uipetman\uipetman.h"
#include "uiRadarMan/uiRadarMan.h"
#include "ContinentMapDlg.h"

#include "uiContinentMapEP2\ContinentMapEp2Dialog.h"

#ifdef _YMS_AUTOBOT_GAME
    #include "Autobot/Autobot.h"
#endif //_YMS_AUTOBOT_GAME
#include "uiInventoryMan/uiInventoryMan.h"
#include "ExtendInventoryDialog.h"

#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "uiSkillTree/uiSkillTree.h"

#ifdef _NA_20100307_BETAKEY_SYSTEM
    #include "uiLoginMan/uiLoginMan.h"
    #include "BetaKeyImpl.h"
    #include "NPCInfoParser.h"
#endif//_NA_20100307_BETAKEY_SYSTEM

#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "Chaossystem/ChaosSystemPacketFunction.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "Rider.h"
#if defined (_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
#include "uiMissionRankMan2/uiMissionRankMan2.h" 
#else
#include "uiMissionRankMan/uiMissionRankMan.h"
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
#include "uiSkillMan/uiNewSkillMan.h"
#include "uiPerkDialog/uiPerkDialog.h"
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM

#ifdef _NA_000000_20130114_RENEWER_UI
	#include "uiOptionMan.h"
	#include "uiOptionView.h"
#endif
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
#include "DominationInfoParser.h"
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

#include "State/SkillState.h"
#include "State/SkillStateHelroidSpin.h"
#include "InventoryDialog.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StatuePlayer.h"
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE


extern DWORD g_CurTime;
HeroActionInput g_HeroInput;

const DWORD VALKYRIE_SHOT_DELAY_MARGIN = 35;
const DWORD UPDATE_MOVE_POSITION_TIME = 500;
#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
BOOL HeroActionInput::bIsRotateL = FALSE;
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
//------------------------------------------------------------------------------
/**
*/

BOOL GetSidePosNoNearTileCheck( Object *pTarget, float fAngle, WzVector *vDest , float add_distance)
{
    WzVector vTarget = pTarget->GetPosition();
    WzVector vPos = g_pHero->GetPosition();
    WzVector vDir = (vPos - vTarget);

    vDir.z = 0.0f;

    VectorNormalize( &vDir, &vDir );

    // Ÿ�ٿ��� �ణ �տ� �ڸ��� �����ش�.
    WzVector vNewDir;

    vNewDir.x = vDir.x * cos(fAngle) + vDir.y * -sin(fAngle);
    vNewDir.y = vDir.x * sin(fAngle) + vDir.y * cos(fAngle);
    vNewDir.z = 0.0f;

    WzVector vNewPos;
    vNewPos = vTarget + (vNewDir * add_distance);

    g_pSunTerrain->PathExp_Reset( g_pPathExplorer, &vNewPos, -1 );  
    g_pPathExplorer->Stop();
    float length = VectorLength(&vNewPos);
    WzVector long_offset_vector = vNewDir * length * -1;
    g_pSunTerrain->PathExp_Thrust(g_pPathExplorer, &long_offset_vector, PTA_NO_WALK);
    g_pSunTerrain->PathExp_Progress(g_pPathExplorer, 1000000, NULL);
    int iTile = g_pPathExplorer->GetTile();
    if( iTile == -1 )
    {
        return FALSE;
    }

    if( !g_pMap )
        return FALSE;

    if( g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute( iTile ) & ( PTA_ONLY_JUMP | PTA_NO_WALK ) )
    {
        return FALSE;
    }

    *vDest = vNewPos; 

    return TRUE;
}

bool _SortItemByDistance(GET_ITEM& item_ref_left, GET_ITEM& item_ref_right)
{
    return (item_ref_left.fDistance < item_ref_right.fDistance);
}


////////////////////////////////////////////////////////////////////////////////////
PickingData::PickingData()
{

}

void PickingData::Init()
{
    m_dwPickedTarget = 0;
    m_bCanMoveToPickedPos = FALSE;
    m_bIgnorePlayer = FALSE;
    m_iPickedTile = -1;
}

void PickingData::ProcessPicking()
{
    m_dwPickedTarget = 0;

    if (!g_pMap)
    {
        return;
    }

    if (VectorLength(&g_wrMouseRay.m_wvDirection) <= 0.2f) 
    {
        return;
    }

#ifndef _DEBUG
    SUNPROCESSPROFILE_BEGIN(_T("CheckMovable"));
    m_bCanMoveToPickedPos = FALSE;
    ZeroMemory(&m_vPickedPos,sizeof(m_vPickedPos));
    m_iPickedTile = -1;

    BOOL bPickTerrain = GameFunc::GetClickedPosition(&m_vPickedPos);
    if (bPickTerrain)
    {
        m_iPickedTile = g_pSunTerrain->x_pPathFinder->GetTileToStand( m_vPickedPos, NULL, -1.0f, 100.0f );

        if (m_iPickedTile != -1)
        {
            if (!(g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(m_iPickedTile) & ( PTA_ONLY_JUMP |PTA_NO_WALK)))
            {          
                m_bCanMoveToPickedPos = TRUE;
            }
        }
    }

    SUNPROCESSPROFILE_END( _T("CheckMovable") );
#else
    m_iPickedTile = -1;
    m_bCanMoveToPickedPos = TRUE;
#endif

    DWORD dwPickingOption = PICKING_IGNORE_CORPSE;

#ifdef _DH_BATTLEZONE2_
    m_bIgnorePlayer = (CGameOption::Instance()->GetPickingFriend() == FALSE);
#endif //_DH_BATTLEZONE2_
    if (m_bIgnorePlayer)
    {
        dwPickingOption |= PICKING_IGNORE_FRIEND;
    }

    if ( !g_Input.GetInterfaceInputMouseFlag() )
    {
        Character* pPickedObj = GameFunc::GetPickedCharacter(&g_wrMouseRay,dwPickingOption);
        if (pPickedObj)
        {
            m_dwPickedTarget = pPickedObj->GetObjectKey();

#ifdef _NA_000000_20140423_SUMMON_BACKGROUND_PICKING
            // ��ȯ���� �ƴ϶�� �ٷ� ����,  ��ȯ���̸� ������,�ʿ�����Ʈ üũ�� �Ѿ
            // �ؿ����� �ش� ������ m_dwPickedTarget�� �ڿ������� ������
            if (pPickedObj->IsKindOfObject(MONSTER_OBJECT) == FALSE)
            {
                return;
            }
            Monster* monster = static_cast<Monster*>(pPickedObj);
            if (monster && monster->IsShadowSummon() == FALSE)
            {
                return;
            }
#else
            return;
#endif //_NA_000000_20140423_SUMMON_BACKGROUND_PICKING
        }

        Item* pItem = GameFunc::GetPickedItem(&g_wrMouseRay);
        if (pItem)
        {
            m_dwPickedTarget = pItem->GetObjectKey();
            return;
        }

        MapObject* pMapObject = GameFunc::GetPickedMapUnitObject(&g_wrMouseRay);
        if (pMapObject)
        {          
#ifdef __NA_0_20080627_MAPOBJECT_PICKING_CHANGE
            m_dwPickedTarget = pMapObject->GetObjectKey();
#else
            // ���� Show�� �Ǿ��ִ� ������Ʈ�� Ŭ�������ϴ�.
            if (pMapObject->IsPickable()) 
            {
                m_dwPickedTarget = pMapObject->GetObjectKey();     
            }
#endif//__NA_0_20080627_MAPOBJECT_PICKING_CHANGE

            return;
        }
    }
}

Character* PickingData::GetPickedCharacter()
{
    Object *pObject = g_ObjectManager.GetObject(m_dwPickedTarget);
    if (pObject) 
    {
        if(pObject->IsKindOfObject(PET_OBJECT))
        {
            return NULL;
        }

        if (pObject->IsKindOfObject(CHARACTER_OBJECT))
        {
            return (Character *)pObject;
        }

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		if (pObject->IsKindOfObject(STATUE_PLAYER_OBJECT))
		{
			return static_cast<Character*>(pObject);
		}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
    }

    return NULL;
    
}

Item* PickingData::GetPickedItem()
{
    Object *pObject = g_ObjectManager.GetObject(m_dwPickedTarget);
    if (pObject) 
    {
        if (pObject->IsKindOfObject(ITEM_OBJECT))
        {
            return (Item *)pObject;
        }
    }

    return NULL;
    
}

MapObject* PickingData::GetPickedMapObject()
{
    Object *pObject = g_ObjectManager.GetObject(m_dwPickedTarget);
    if (pObject) 
    {
        if (pObject->IsKindOfObject(MAP_OBJECT))
        {
            return (MapObject *)pObject;
        }
    }

    return NULL;
}


Object* PickingData::GetPickedObject()
{
    Object *pObject = g_ObjectManager.GetObject(m_dwPickedTarget);

    return pObject;   
}

DWORD PickingData::GetPickedObjectKey()
{
    return m_dwPickedTarget;
}


////////////////////////////////////////////////////////////////////////////////////
HeroActionInput::HeroActionInput(void)
{
    m_TargetAreaSkillID = 0;
    m_ShowTargetAreaPointer= FALSE;

    SetCurrentTarget(0);
    ZeroMemory(&m_QueueAction,sizeof(m_QueueAction));   
    
    m_QueueAction.ActionID = ACTION_IDLE;
    m_iComboCount = 0;
    m_bAutoAttack = FALSE;  
    m_bCanDoNextAttack = FALSE;

    m_bAutoRun = FALSE;

    m_bIsHeroKeyboardMove = FALSE;
    m_bKeyBoardMoveCheck = FALSE;
    m_bKeyboardQueueAttack = FALSE;
    m_iKeyboardQueueAttackDelay = 0;

    m_MousePoint.x = 0;
    m_MousePoint.y = 0;
    m_bIsMousePointHide = FALSE;

    m_iUIVoiceDelay = 0;
    m_dwUIVoice = 0;
    m_iMoveDelay=0;

    
    ResetSpecialMove();

    m_bCannotAttack = FALSE;
    m_bSpecialMove=FALSE;
    m_bFollowPlayer = FALSE;
    m_dwFollowPlayerKey = 0;
    m_TargetType = RELATIONKIND::None;
    m_OldReason=0;
    m_bFixTarget = FALSE;
    m_bForwardTargeting = TRUE;
    m_bCanInputMode = TRUE;

    set_jump_key_pressed(false);

    PickingData::Init();

    m_IsClickedTargetArea = FALSE;

}

void HeroActionInput::Init()
{
    m_bShowPetInfo=TRUE;    
    m_bSpecialMove=FALSE;
    m_iUIVoiceDelay = 0;
    m_dwUIVoice = 0;
    m_iMoveDelay=0;
    //m_dwPickedTarget = 0;
    g_SkillQueue.ClearAll();

    m_dwStartPickPosTime=0;

    m_bCannotAttack = FALSE;
    m_iMoveToActionTime=0;
    m_bCanInputMode = TRUE;
    m_bShowPartyMemberOnly=FALSE;
    m_TransFormActionQueue.Clear();
    m_dwClickedNpcCode = 0;

#ifdef PERF_AUTO_MOVE_TEST
    m_bAutoMove=FALSE;
    m_vSrcPos=c_ZeroVector;
    m_vDestPos=c_ZeroVector;
    m_bSwitch=FALSE;
#endif 

#ifdef MAP_EDIT_MODE
    m_dwEditMapObjKey=0;
#endif 
    
    m_bCameraBaseMove = TRUE;

    PickingData::Init();
}


void HeroActionInput::SetTransFormAction(PLAYER_ACTION *pAction)
{
    m_TransFormActionQueue.PushBack(*pAction);
}

//------------------------------------------------------------------------------
/**
*/
HeroActionInput::~HeroActionInput(void)
{
    m_TransFormActionQueue.Clear();
}


//------------------------------------------------------------------------------
/**
*/
void HeroActionInput::Process(DWORD dwTick)
{
    if (GlobalFunc::BeforeLoadingInputLock())
    {
        ProcessKeyboardMove();
        return;
    }

    PickingForShowMonsterName();

    // Voice Delay�� ��Ȳ���� ������� ������ ó���Ǿ����
    ProcessVoiceDelay(dwTick);
    
    if (!g_pHero) return;

    m_iMoveToActionTime-=dwTick;
    m_iMoveToActionTime=max(0,m_iMoveToActionTime);


    // ������ �Կ��� ���� ī�޶�
    if( GENERALPARAM->IsFreeCameraMode() )
    {
        if( GENERALPARAM->GetFreeCameraState() )
        {
            return;
        }
    }

    if ( g_pMap && !g_pMap->IsRender())
    {
        return;
    }

    // �޺� ó�� (�����ð� ���� �Է��� �ٽ� �ȵ����� 1Ÿ�� ó���ϴ� �κ�)
    ProcessComboDelay(dwTick);

    // Ÿ��â ���¸� ó��
    ProcessTargeting();

    // ��ŷ ������ ����
    PickingData::ProcessPicking();

    // ��ŷ������ �̿��� Ŀ���� ����
    ProcessCursor();

    // ����������ϋ��� Ű�Է� ó��
    ProcessObserverModeKey(dwTick);

    //�׾��ų�,����,�Ͼ��,�ٿ�,����߿��� �Է� ���´�.
    if (g_pHero)
    {
		if( CannotMoveState() == TRUE )
        {          
            g_pHero->ResetMoveFlag();
            SetAutoRun(FALSE);
            SetFollowState( FALSE );
        }

        if(g_pHero->IsLockInput())
        {
            if(!MouseCursor::Instance()->IsShowCursor())
            {
                this->m_bIsMousePointHide = FALSE;
                SetCursorPos( m_MousePoint.x, m_MousePoint.y );
                MouseCursor::Instance()->ShowCursor( TRUE );
            }
            //�⺻����Űó�����Ѵ�(������+�̴ϸ�+��ũ����+�����)
            ProcessBaseOptionKey();
            // Ű�����̵� ����
            return;
        }
    }

    if( IsShootingMode() == FALSE )
    {
        // �Ϲ�ĳ����

        g_pHero->LockOn(0);

        ProcessAutoAttack();
        ProcessKeyboardAttack();

        //Todo:�̵��� ��ųť�� �ʱ�ȭ �Ѵ�
        if(m_iMoveDelay<=0)
        {
            m_bIsHeroKeyboardMove = ProcessKeyboardMove();
        }
        
        ProcessMouse();

        // Ÿ�� ��ý�Ʈ!!
        ProcessAssistTarget();
        ProcessQueueAction();

        // ���� ��
        // Todo:�̵��� ��ųť�� �ʱ�ȭ �Ѵ�
        if( m_iMoveDelay <= 0 && m_bAutoRun )
        {
            InitTerm();

            if ( IsIdleState() || g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
            }
            g_pHero->AddMoveFlag( KEYBOARDMOVE_FOWARD );
        }
    }
    else
    {
        // ��Ű��

        ProcessAutoAttack();
        //Todo:�̵��� ��ųť�� �ʱ�ȭ �Ѵ�
        if(m_iMoveDelay<=0)
        {
            m_bIsHeroKeyboardMove = ProcessKeyboardMove();
        }

        ProcessValkyrieMouse();

        // Ÿ�� ��ý�Ʈ!!
        ProcessAssistTarget();
        ProcessQueueAction();

        // ���� ��
        //Todo:�̵��� ��ųť�� �ʱ�ȭ �Ѵ�
        if( m_iMoveDelay<=0&&m_bAutoRun )
        {
            InitTerm();

            if (IsIdleState() || g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
            }
            g_pHero->AddMoveFlag( KEYBOARDMOVE_FOWARD );
        }

        // ������ 45���̻� �������� Ÿ�� Ǯ��
        if (GetCurrentTarget())
        {
            Character *pTarget = (Character *)g_ObjectManager.GetObject( this->GetCurrentTarget() );
            if (pTarget)
            {
                WzVector vTarget = pTarget->GetPosition();
                WzVector vPos = g_pHero->GetPosition();
                WzVector vDiff = vTarget - vPos;
                float fTargetAngle = Math_GetAngleFromVector(&vDiff);
                float fAngle = g_pHero->GetAngle();

                float fDiff = abs(fTargetAngle - fAngle);
                float fAnotherDiff = WZ_PI * 2.0f - fDiff;

                // ������ 45���̻�Ǹ� ���� Ǯ��
                if (min(fDiff,fAnotherDiff) > WZ_PI / 4.0f )
                {
                    SetAutoAttack(FALSE);
                    g_pHero->LockOn( 0 );
                }
                else
                {
                    g_pHero->LockOn( this->GetCurrentTarget() );
                }
            }
        }
    }  

    
    WzVector pickPos;

    //�������� ������  
    if( m_ShowTargetAreaPointer == FALSE || GameFunc::GetClickedPosition(&pickPos) == FALSE )
    {
        g_InterfaceManager.SetAreaPointer(m_ShowTargetAreaPointer,NULL,0);
        g_InterfaceManager.SetAreaTargetPointerTexture( g_InterfaceManager.GetAreaTargetTextureHandle( g_pHero->GetClass() ) );
    }
    else
    {
#ifdef _YMS_AUTOBOT_GAME
        Autobot::Instance()->GetAreaSkillClickPosition((SLOTCODE)m_TargetAreaSkillID, pickPos);
#endif //_YMS_AUTOBOT_GAME

        WzColor color = WzColor_RGBA(255, 0,0, 255);

        BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)m_TargetAreaSkillID);

        if(pInfo)
        {
            float len = GetSkillRange( pInfo );

            WzVector vDistance = pickPos - g_pHero->GetPosition();
            float fDistance = VectorLength(&vDistance);

            if( fDistance <= len)
            {
                color = WzColor_RGBA(255,255,255, 255);
            }

            if (pInfo->m_SkillClassCode == eSKILL_CREATE_CRYSTAL_WARP)
            {
                if( GameFunc::IsMove(g_pHero->GetPosition(),m_PickingPosition) == FALSE )
                {
                    color = WzColor_RGBA(255, 0,0, 255);
                }
            }

            //Todo:�Ÿ��� �ʹ� �ָ� ��� �ȵǰ�
            m_PickingPosition = pickPos; 

            if(pInfo->m_wSkillArea)
                g_InterfaceManager.SetAreaPointer(m_ShowTargetAreaPointer,&m_PickingPosition,(float)pInfo->m_wSkillArea * 0.1f,color);       
            else
                g_InterfaceManager.SetAreaPointer(m_ShowTargetAreaPointer,&m_PickingPosition,0.0f); 

            g_InterfaceManager.SetAreaTargetPointerTexture( g_InterfaceManager.GetAreaTargetTextureHandle( g_pHero->GetClass() ) );

        }
    }

    ProcessOptionKey();

    m_iMoveDelay -= (short)dwTick;
    m_iMoveDelay = max( 0, m_iMoveDelay);

    // �÷��̾� ���󰡱�
    ProcessFollow();
}

float HeroActionInput::GetSkillRange( BASE_SKILLINFO* skill_info )
{
    if( skill_info == NULL )
        return 0.0f;

    float range = (float)skill_info->m_wSkillRange * 0.1f;

    // ��ų ��Ÿ� ���� ������ ã�´�
    if( SKILL_EFFECT* effect = g_pHero->FindSkillEffect( eCHAR_STATE_INCREASE_SKILLRANGE ) )
    {
        if( effect->dwSkillID != 0 )
        {
            float range_bonus = (float)effect->AbilityInfo.m_iParam[1] * 0.1f * 0.01f;
            range = range + range * range_bonus;
        }
    }

    return range;
}

//------------------------------------------------------------------------------
/**
    ���� ���� �� npc,item �� ��Ÿ ��ȿ �Ÿ� üũ 
*/
float HeroActionInput::GetActionLength(PLAYER_ACTION action)
{
    float fLength = 0.0f;
    switch (action.ActionID)
    {
    case ACTION_ATTACK:
        {
            fLength = GetAttackActionLength();
        }
        break;
    case ACTION_PLAYERMEET:
        {
            if(g_pHero->IsCrystalWarpKey(action.PLAYERMEET.dwTargetID))
            {
                fLength = 2.0f;
            }
            else
            {
                fLength = DISTANCE_NPC_MEET * 0.9f; 
            }
        }
        break;
    case ACTION_NPCMEET:
    case ACTION_VENDORMEET:
        {
            fLength = DISTANCE_NPC_MEET * 0.9f; 
        }
        break;
    case ACTION_USE_OBJECT:
        {
            fLength = DISTANCE_NPC_MEET;
        }
        break;
    case ACTION_COLLECTION_OBJECT:
        {
            fLength = 3.0f;
        }
        break;

    case ACTION_DOMINATION_OBJECT:
    case ACTION_WAYPOINT_MEET:
    case ACTION_AIRSHIP_MEET:
        {
            fLength = DISTANCE_NPC_MEET; 
        }
        break;

    case ACTION_GETITEM:
        {
            return DISTANCE_NPC_MEET; 
        }
        break;

    case ACTION_VALKYRIESHOT:
        {
            fLength = GetAttackActionLength();
        }
        break;

    case ACTION_SKILL:
        {
            BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)action.SKILL.dwSkillID);
            assert(pInfo);

            if(pInfo)
            {
                // �ڽ� ���� ��ų�̸�
                if (pInfo->m_byTarget == SKILL_TARGET_ME)
                {
                    fLength = 9.999999e+29f;;
                }
                else
                {
                    fLength = GetSkillRange(pInfo);
                    //if (pInfo->IsRangedSkill())
                    //{
                    //    fLength = (float)(pInfo->m_wSkillRange + g_pHero->GetPlayerAttribute()->GetSkillRangeBonus()) / 10.0f;               
                    //}
                    //else
                    //{
                    //    fLength =  (float)pInfo->m_wSkillRange / 10.0f;
                    //}
                }
            }
        }
        break;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	case ACTION_STATUE_PLAYER_MEET:
		{
			fLength = 18.0f;
		}
		break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
    }  
    return fLength;
}

////------------------------------------------------------------------------------
///**
//*/
BOOL  HeroActionInput::ChoaCanUse()
{
    //�������� ���� ó�� ���شٰ� �� �޼��� ���� �� 
    return TRUE;
}



//------------------------------------------------------------------------------
/**
    NPC ���ɱ�
*/
BOOL HeroActionInput::ProcessQueueNPCMeet()
{  
    NPC * pNPC = (NPC *)g_ObjectManager.GetObject(m_QueueAction.NPCMEET.dwNPCKey); 

#ifdef _NA_20100307_BETAKEY_SYSTEM
    if (pNPC && IsUseableNPCBetakey(pNPC->GetNpcInfo()) == FALSE)
    {
        return FALSE;
    }
#endif//_NA_20100307_BETAKEY_SYSTEM

    // NPC�� ����� �ٰ������� ���Ǵ�.
    if (pNPC && g_pHero)
    {
        m_dwClickedNpcCode = pNPC->GetExtraInfo()->m_NPCCODE;
        WzVector wzVec1 = pNPC->GetPosition() - g_pHero->GetPosition();
        WzVector wzVec2;
        wzVec2 = -wzVec1;
        wzVec2.z = 0.0f;
        float fLength = VectorLength(&wzVec1);

        if (fLength <= GetActionLength(m_QueueAction))
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }

            SolarDialog * pDlg = NULL;
            NPC::SetNpcVoiceIndex(rand() % MAX_NPC_VOICE_SAMPLE);

            if (g_pMap)
            {
                pDlg = GameFunc::NPCMeetWithForward(
                    g_pMap->GetMapID(),
                    g_pMap->GetCurrentFieldID(),
                    pNPC->GetMonsterInfo()->m_MonsterCode,
                    pNPC->GetObjectKey());
            }      

            if ( pDlg )
            {
                pNPC->SetOpenDialog(pDlg);
                pDlg->SetOwnerKey(pNPC->GetObjectKey());
            }

            if ( !(wzVec2.x == 0.0f && wzVec2.y == 0.0f) )
            {
                float fAngle = Math_GetAngleFromVector(&wzVec2);
                if (pNPC->GetExtraInfo() && 
                    EXTRA_NPCINFO::eROTATE_ROTATE == pNPC->GetExtraInfo()->m_eRotateType )
                {
                    pNPC->SetAngle(fAngle);
                }              
            }

            if(g_pHero && pNPC)
                g_pHero->SetTargetID(pNPC->GetObjectKey());

            pNPC->DoAction(&m_QueueAction); 
            pNPC->CommitChangeState(g_CurTime);
            pNPC->PlayMeetSound();

            return TRUE;
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
NPC ���ɱ�
*/
BOOL HeroActionInput::ProcessQueueVendorMeet()
{
    if(!ChoaCanUse())
        return FALSE;

    Player * pPlayer = (Player *)g_ObjectManager.GetObject(m_QueueAction.VENDORMEET.dwTargetID); 
    // NPC�� ����� �ٰ������� ���Ǵ�.
    if (pPlayer && g_pHero)
    {
        WzVector wzVec1 = pPlayer->GetPosition() - g_pHero->GetPosition();
        WzVector wzVec2;
        wzVec2 = -wzVec1;
        wzVec2.z = 0.0f;
        float fLength = VectorLength(&wzVec1);

        if (fLength <= GetActionLength(m_QueueAction))
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }

            uiVendorBuy* pVendorBuy = GET_CAST_DIALOG( uiVendorBuy, IM_VENDOR_MANAGER::VENDOR_BUY );

            if(pVendorBuy)
                pVendorBuy->QueryView( pPlayer->GetObjectKey() );

            if ( !(wzVec2.x == 0.0f && wzVec2.y == 0.0f) )
            {
                float fAngle = Math_GetAngleFromVector(&wzVec2);
                pPlayer->SetAngle(fAngle);
            }

            pPlayer->DoAction(&m_QueueAction); 
            pPlayer->CommitChangeState(g_CurTime);

            return TRUE;
        }
    }

    return FALSE;
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
BOOL HeroActionInput::ProcessQueueStatuePlayerMeet()
{
	StatuePlayer* statue_player = static_cast<StatuePlayer*>(g_ObjectManager.GetObject(m_QueueAction.STATUEMEET.dwStatuePlayerKey)); 

	// ���� ����� �ٰ������� ���Ǵ�.
	if (statue_player && g_pHero)
	{
		WzVector wzVec1 = statue_player->GetPosition() - g_pHero->GetPosition();
		WzVector wzVec2;
		wzVec2 = -wzVec1;
		wzVec2.z = 0.0f;
		float fLength = VectorLength(&wzVec1);

		if (fLength <= GetActionLength(m_QueueAction))
		{
			uiSUNRankingMan* ui_sun_ranking_manager = GET_DIALOG_MANAGER(uiSUNRankingMan, UIMAN_SUNRANKING);
			if ( ui_sun_ranking_manager )
			{
				ui_sun_ranking_manager->ShowSUNRankingStatueInfo(statue_player->GetObjectKey());
			}

			return TRUE;
		}
	}

	return FALSE;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

BOOL HeroActionInput::ProcessQueueCollectObject()
{
    MapObject * pObject = (MapObject *)g_ObjectManager.GetObject(m_QueueAction.COLLECTOBJECT.dwTargetID); 
    if(!pObject || !g_pHero)
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "[Null] TargetID: %d", m_QueueAction.COLLECTOBJECT.dwTargetID);
#endif //_DEV_VER
        return FALSE;
    }

    if(INVALID_WZID == pObject->GetCollectionID())
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "ProcessQueueCollectObject()::INVALID_WZID == pObject->GetCollectionID()");
#endif //_DEV_VER
        return FALSE;
    }

    float fDistance = VectorLength( & (pObject->GetPosition() - g_pHero->GetVisiblePos()) );
    if (fDistance <= GetActionLength(m_QueueAction))
    {
        //Ż��ź���¿����� �ڵ�����
        g_pHero->SendRiderReleasePacket(false);
        g_pHero->SendWingsControlByAccessory(false, false);

        // �̵����̾����� �����.
        if (g_pHero->GetCurState() == STATE::MOVE || g_pHero->GetCurState() == STATE::KEYBOARDMOVE)
        {
            g_pHero->StopMove();
            g_pHero->SendStopMovePacket();
            return FALSE;
        }

        if(STATE::COLLECT != g_pHero->GetCurState())
        {
            StateCollect::SendCollectActionSyn(pObject->GetObjectKey(), g_pHero->GetVisiblePos());

#ifdef _NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT
            // syn ������ �ش� objectkey�� �̸� �ѹ� ����
            if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_MISSION))
            {
                g_pHero->SetCollectObjKey(pObject->GetObjectKey());
            }
#endif //_NA_006744_20130531_COLLECT_NAK_TO_REMOVE_COLLECT
        }

        return TRUE;
    }

    return FALSE;
}

BOOL HeroActionInput::ProcessQueueUseObject()
{
    MapObject * pObject = (MapObject *)g_ObjectManager.GetObject(m_QueueAction.USEOBJECT.dwTargetID); 

    // �÷��̾ ����� �ٰ������� ����.
    if (pObject && g_pHero)
    {
        if(FALSE == pObject->CheckTriggerActive())
            return FALSE;

       WzBoundingVolume *bv = pObject->GetBoundingVolume();
       float fMargin = 0.0f;
       if (bv)
       {
           switch(bv->m_eBvType)
           {
           case BVT_AABB:
               {
                   WzVector diff = bv->m_aabb.m_wvMax - bv->m_aabb.m_wvMin;
                   fMargin = max(diff.x,diff.y) / 2.0f;
               }
               break;
           case BVT_SPHERE:
               {
                   fMargin = bv->m_BvSphere.m_fRadius;
               }
               break;                 
           }
       }

        const float ACTIVATE_TRIGGER_RANGE = 4.0f;

        WzColor  Color;
        // ���� MapObject GetPosition �½��ϴ� MapObject�� VisiblePos �Ⱦ��ϴ� �ٲ��� ������!!
        float fDistance = VectorLength( & (pObject->GetPosition() - g_pHero->GetVisiblePos()) );
        fDistance -= fMargin;

        if (fDistance <= GetActionLength(m_QueueAction))
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE || g_pHero->GetCurState() == STATE::KEYBOARDMOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }

            MSG_CG_TRIGGER_CLICK_OBJECT_SYN     SendPacket;
            SendPacket.m_byCategory           = CG_TRIGGER;
            SendPacket.m_byProtocol           = CG_TRIGGER_CLICK_OBJECT_SYN;
            SendPacket.m_dwClickedObjectKey = pObject->GetTriggerID();

            if( FALSE == GlobalFunc::SendPacket( CI_GAMESERVERIDX, &SendPacket, sizeof(MSG_CG_TRIGGER_CLICK_OBJECT_SYN) ) )
            {
                // Send Packet ���� ����
                Color = WzColor_RGBA( 255, 80, 80, 255 );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,_T("MSG_CG_TRIGGER_CLICK_OBJECT_SYN - BattleScene"));

            }

            return TRUE;
        }
    }

    return FALSE;
}


//------------------------------------------------------------------------------
/**
�÷��̾�� ����
*/
BOOL HeroActionInput::ProcessQueuePlayerMeet()
{
    Player * pPlayer = (Player *)g_ObjectManager.GetObject(m_QueueAction.PLAYERMEET.dwTargetID); 

    // �÷��̾ ����� �ٰ������� ����.
    if (pPlayer && g_pHero)
    {
        WzVector wzVec1 = pPlayer->GetPosition() - g_pHero->GetPosition();
        WzVector wzVec2;
        wzVec2 = -wzVec1;
        wzVec2.z = 0.0f;
        float fLength = VectorLength(&wzVec1);

        if (fLength <= GetActionLength(m_QueueAction))
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }

            if (MouseHandler::Instance()->IsExistItemAtHand())
            {
                uiTradeMan* tradeMan =
                    static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
                assert (tradeMan);

                if (tradeMan && FALSE == GlobalFunc::IsSSQScene())
                {
                    tradeMan->Ask(pPlayer->GetName());
                    tradeMan->SetPutOneStarting();
                }
            }

            return TRUE;
        }
    }

    return FALSE;
}


BOOL HeroActionInput::ProcessQueuePlayerFollow()
{
    Player * pPlayer = (Player *)g_ObjectManager.GetObject(m_QueueAction.PLAYERMEET.dwTargetID); 

    // �÷��̾ ����� �ٰ������� ����.
    if (pPlayer && g_pHero)
    {
        WzVector wzVec1 = pPlayer->GetPosition() - g_pHero->GetPosition();
        WzVector wzVec2;
        wzVec2 = -wzVec1;
        wzVec2.z = 0.0f;
        float fLength = VectorLength(&wzVec1);

        if (fLength <= DISTANCE_FOLLOW_PLAYER )
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }

            return TRUE;
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
��������Ʈ�� ����
*/
BOOL HeroActionInput::ProcessQueueWayPointMeet()
{
    MapObject * pObject = (MapObject *)g_ObjectManager.GetObject(m_QueueAction.WAYPOINTMEET.dwTargetID); 
 
    // ����� �ٰ������� ����.
    if (pObject && g_pHero)
    {
        if(!pObject->IsWayPointObj())
            return FALSE;

        WzVector wzVec1 = pObject->GetPosition() - g_pHero->GetPosition();
        float fLength = VectorLength(&wzVec1);

        if (fLength <= GetActionLength(m_QueueAction))
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }          
            else
            {
                // ��������Ʈ ���̺�
                uiWayPointMan* waypointMan = GET_DIALOG_MANAGER(uiWayPointMan, UIMAN_WAYPOINT);
                if(waypointMan)
                {
                    waypointMan->RqstWayPointSave(m_QueueAction.WAYPOINTMEET.dwTargetID);
                }

                return TRUE;
            }
        }
    }

    return FALSE;
}
//------------------------------------------------------------------------------
/**
    �����������Ʈ�� �̵�
*/
bool HeroActionInput::ProcessQueueAirShipMeet()
{
    MapObject* object = static_cast<MapObject *>(g_ObjectManager.GetObject(m_QueueAction.AIRSHIP.target_object)); 

    // ����� �ٰ������� ����.
    if (object &&
        object->IsAirShipObject())
    {
        WzVector distance = object->GetPosition() - g_pHero->GetPosition();
        float length = VectorLength(&distance);

        if (length <= GetActionLength(m_QueueAction))
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }          
            else
            {
                uiAssistMan* assist_man = static_cast<uiAssistMan*>(
                            g_InterfaceManager.GetUserInterfaceManager(UIMAN_ASSIST));  

                if (assist_man)
                {

                    sMAPPORTAL_HASH* hash_table =  MapInfoParser::Instance()->GetMapPortalHashTable();
                    sMAPPORTAL_HASH::const_iterator itr = hash_table->begin();

                    while (itr != hash_table->end())
                    {
                        sMAP_ENTERENCE_PORTAL* data = itr->second;// (*itr);

                        if (data && 
                           (data->bMoveType == ePORTAL_MOVE_AIRSHIP) &&
                           (g_pMap->GetCurrentFieldID() == data->wFieldFrom))
                        {
                            assist_man->ShowAskAirShipPortal(data->wIndex, false, m_QueueAction.AIRSHIP.target_object);
                            return true;
                        }
                        ++itr;
                    }
                }
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::ProcessQueueDominationObject()
{
    MapObject * pObject = (MapObject *)g_ObjectManager.GetObject(m_QueueAction.DOMINATIONOBJECT.dwTargetID); 

    // ����� �ٰ������� ����.
    if (pObject && g_pHero)
    {
        if(!pObject->IsDominationObject())
            return FALSE;

        WzVector wzVec1 = pObject->GetPosition() - g_pHero->GetPosition();
        WzVector wzVec2;
        wzVec2 = -wzVec1;
        wzVec2.z = 0.0f;
        float fLength = VectorLength(&wzVec1);

        if (fLength <= GetActionLength(m_QueueAction))
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }          
            else
            {
                uiDominationTowerMan* pUIMan = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
                if (pUIMan)
                {
                    pUIMan->SEND_DOMINATION_NOTICE_SYN(m_QueueAction.DOMINATIONOBJECT.dwTargetID);
                }
    
                return TRUE;
            }
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
    ������ �ݱ�
*/
BOOL HeroActionInput::ProcessQueueGetItem()
{
    Item * pItem = (Item *)g_ObjectManager.GetObject(m_QueueAction.GETITEM.dwItemKey); 

    // �����ۿ� ����� �ٰ����� �ݴ´�
    if (pItem)
    {
        WzVector wzVec1 = pItem->GetPosition() - g_pHero->GetPosition();
        WzVector wzVec2;
        wzVec2 = -wzVec1;
        wzVec2.z = 0.0f;

        float fLength = VectorLength(&wzVec1);

        if (fLength <= DISTANCE_SPACE_GET_ITEM)
        {
            // �̵����̾����� �����.
            if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }

            if (uiAreaConquestMan::CanPickACItem(pItem, true) == false)
            {
                return FALSE;
            }
            ItemManager::Instance()->SendPickUpItemObject(pItem);
            pItem->SetSendPickPacket(TRUE);

            return TRUE;   
        }
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/**
    ��Ÿ���� ���� ��Ÿ�ϰ� �ٸ��� ȣ�� 
*/
void HeroActionInput::SetComboReset()
{
    m_iComboCount = 0;
    m_iComboDelay = 0;

    if(g_pHero)
        g_pHero->SetComboCount(0);
    
    SetAutoRun(FALSE);
    SetFollowState( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::ProcessQueueAttack()
{
    Character * pEnemy = (Character *)g_ObjectManager.GetObject(m_QueueAction.ATTACK.dwTargetID);

    if (pEnemy && g_pHero)  
    {
        if (!pEnemy->CanBeAttackTarget())//Ÿ������ �����ɼ� ���ٸ� 
        {
            return FALSE;
        }

        WzVector wzVec1 = pEnemy->GetPosition() - g_pHero->GetPosition();
        float fLength = VectorLength(&wzVec1);//���� ���� �Ÿ��� ���Ѵ�

        if ( fLength <= g_HeroInput.GetAttackActionLength() + pEnemy->GetBodyRadius() )//���� ���� �ȿ� �ִٸ� 
        {
            if (g_pHero->CanDoQueueAction(&m_QueueAction))// ���¸� ��ȭ �Ҽ� �ֳ�(������ �̵� ����)
            {
                g_pHero->StopMove();          // ���� ��ȭ�� �̵����� 
                g_pHero->SetTargetID(pEnemy->GetObjectKey());// Ÿ�� ���� 
                g_pHero->SetNextState(STATE::ATTACK,g_CurTime);//���� ���� ���� 
                g_pHero->SetCurrentAction(&m_QueueAction);
                
                return TRUE;
            }
        }
        else
        {
        }
    }
    else
    {
        return FALSE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
DWORD HeroActionInput::IncreaseComboCount()
{
    m_iComboCount++;//�޺��� ���� 

    DWORD dwCurStyle = g_pHero->GetCurrentAttackStyle();//���ݽ�Ÿ���� ���� 

    BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo((SLOTCODE)dwCurStyle);

    // ��ġ�� ���Ÿ� ������ �޺�����
    
    if(pInfo==NULL)
    {
        if (m_iComboCount >= 2)//�⺻ ���� 2Ÿ
        {
            m_iComboCount = 0;
        }
        return m_iComboCount;
    }
    
    if(g_pHero->IsRangedNormalAttack())
    {
        if (m_iComboCount >= 2)//�⺻ ���� 2Ÿ
        {
            m_iComboCount = 0;
        }
    }
    else if(pInfo->m_byStyleCheck==FALSE)//��Ÿ�� ���� 2Ÿ 
    {
        if (m_iComboCount >= 2)
        {
            m_iComboCount = 0;

        }
    }
    // ��Ÿ���� ������ 3��Ÿ~
    else
    {
        if (m_iComboCount >= g_pHero->NormalAttack.combo_max_count)
        {
            m_iComboCount = 0;
        }
    }

    return m_iComboCount;
}

//------------------------------------------------------------------------------
/**
*/
DWORD HeroActionInput::GetComboCount()
{
    return m_iComboCount;
}

//----------------------------------------------------------------------------
/**
*/
void HeroActionInput::ClearPickingTarget()
{
    PickingData::Init();
    //this->m_dwPickedTarget = 0;
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::ProcessSpecialMove()
{
    if(g_pHero)
    {
        if (g_pHero->CanDoQueueAction(&m_QueueAction)) 
        {
            // ���������߿��� ����
            if (IsShootingMode())
            {
                if (g_pHero->GetValkyrieShotDelay() > 30)
                {
                    return FALSE;
                }
            }

            g_pHero->DoAction(&m_QueueAction);
        
            //���䷱ ����.
            if( m_bAutoRun == TRUE )
            {
                SetAutoRun(FALSE);
                SetFollowState( FALSE );
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }


            return TRUE;
        }
    }

    return FALSE;
}
    
//------------------------------------------------------------------------------
/**
Ű ť �޴����� ���� ������ �׻� ���Լ� ��� �ϼ���!
*/
void HeroActionInput::SetAreaSkill(BOOL bShow)
{
    if( bShow == TRUE )
    {
        if( m_ShowTargetAreaPointer != bShow )
        {
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType = eDoType_Area_Pointer;
            msg.DoSomething = GlobalFunc::DoAreaSkill;
            g_KeyQueueManager.PushBack(msg);
        }
    }
    else
    {
        if( m_ShowTargetAreaPointer != bShow )
        {
            g_KeyQueueManager.DeleteMsg(eDoType_Area_Pointer);
        }
    }

    m_ShowTargetAreaPointer = bShow;
}

BOOL HeroActionInput::OnSkill(SLOTCODE dwSkillid)
{
    BOOL rt=FALSE;
    if(!IsAckBeginVendor())
    {
        BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillid);

        if(!pInfo)
            return FALSE;

        switch(pInfo->m_bySkillUserType)
        {
        case eSKILL_USER_PLAYER:
            rt = InsertSkill(dwSkillid);
            break;

        case eSKILL_USER_SUMMONED:
            rt = UseSummonSkill(dwSkillid);
            break;

        case eSKILL_USER_ACTION:
            rt = ProcessUserAction( dwSkillid );
            break;

        case eSKILL_USER_EMOTICON:

            //------------------------------------------------------------------------------
            // ���� : define _YMS_CANNOT_EMOTION
            // ���� : if ���� ProcessUserEmoticon() �Լ� ���� �̵�
            // ���� : �ߺ��� �ڵ� ����
            //       : ProcessUserEmoticon() �Լ� �ȿ��� false �� �ɼ� �ִµ� ������� 
            //         ProcessUserEmoticon() ����� ������� stop ��Ŷ�� �����Ե�
            //if(g_pHero->IsMove())
            //    g_pHero->SendStopMovePacket();
            //------------------------------------------------------------------------------
            rt = ProcessUserEmoticon( dwSkillid );

            break;

        default:
            assert(!"����Ҽ����� ���� Ÿ��");
            break;
        }
    }

    return rt;
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::UseSummonSkill(SLOTCODE dwSkillid)
{
    if(!g_pHero)
        return FALSE;

    DWORD dwId=g_pHero->GetSummonId();
    DWORD dwAtt=g_pHero->GetCurrentTileAtt(FALSE);

    if(dwId==NONE_ID)
    {
        // ���͸� ��ȯ�ϼ���.
        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

        const int   c_iSUMMON_MONSTER = 70425;
        g_InterfaceManager.GetInterfaceString( c_iSUMMON_MONSTER, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage); 
        return FALSE;
    }

    Monster *pMon=(Monster *)g_ObjectManager.GetObject(dwId);
    DWORD dwTargetId=GetCurrentTarget();

    BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillid);
    assert(pInfo);
    if (!pInfo)
    {
        return FALSE;
    }
    
    if(!pMon)
    {
        return FALSE;
    }

    if(!pMon->IsKindOfObject(MONSTER_OBJECT))
    {
        return FALSE;
    }

    if(pInfo->m_byTarget==SKILL_TARGET_ME||pInfo->m_byTarget==SKILL_TARGET_REACHABLE_ME)
    {
        dwTargetId=dwId;//�ڽ��� ���̵� �־� �ش�.
    }

    Character *pTarget=(Character *)g_ObjectManager.GetObject(dwTargetId);

    if(pInfo->m_byTarget!=SKILL_TARGET_ME&&pInfo->m_byTarget!=SKILL_TARGET_REACHABLE_ME)
    {
        int iResult = 0; //�ֽ����ߴ��� ��ü������ ������Ѻ���
        if (!CanBeSkillTarget(dwSkillid,pTarget,lsForceAttack(),iResult))
        {              
            TCHAR szFailMessage[512];
            //������Ǿȵ������� default������
            int iStringCode = ((iResult == 0) ? (eST_SKILL_FAIL_NOT_PROPER_TARGET):(iResult));
            g_InterfaceManager.GetInterfaceString(iStringCode,szFailMessage,MAX_PATH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);  
            return FALSE;
        }
    }

    if (pTarget&&pMon&&pMon->CanUseSkill(dwSkillid,TRUE))
    {
        MSG_CG_SUMMON_SKILL_ACTION_SYN  Packet;
        Packet.m_byCategory = CG_SUMMON;
        Packet.m_byProtocol = CG_SUMMON_SKILL_ACTION_SYN;     
        Packet.m_dwSummonedObjKey=dwId; 
        Packet.m_SkillCode = (SLOTCODE)dwSkillid;       
        //Packet.m_dwClientSerial=pMon->GetCurrentAttackSerial();   
        Packet.m_dwClientSerial=pMon->GetNextAttackSerial();
        Packet.m_wvCurPos=pMon->GetPosition();          
        Packet.m_wvDestPos=pMon->GetPosition();     
        Packet.m_dwMainTargetKey=dwTargetId; 
        Packet.m_wvMainTargetPos=pTarget->GetPosition();


        Packet.m_byAttackPropensity=lsForceAttack();           // �������� ����


        //�����̰� ���� ���� ������ ����
        if((GameFramework::GetCurrentScene()&&
            GameFramework::GetCurrentScene()->IsEnablePkScene()==FALSE)||!g_isPkServer)
        {
            Packet.m_byAttackPropensity=ATTACK_PROPENSITY_NORMAL;
        }

        if(g_pHero->SendPacket( &Packet, sizeof(MSG_CG_SUMMON_SKILL_ACTION_SYN) )==FALSE )
        {
            return FALSE;
        }

        return TRUE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::InsertSkill(SLOTCODE dwSkillid)
{
    if(!g_pHero)
        return FALSE;

    // ��ų�� �߰��ҋ�
    SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillid);
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
#else
	// ���� ����Ϸ��� ��ų�� ��Ű���� ���潺 ����̰�, ���潺 ����� �̹� �������϶� ����ϱ� ���� �ϵ��ڵ�
    if( SKILL_EFFECT* pSkillEffect = g_pHero->FindSkillEffect( (eCHAR_STATE_TYPE)121 ) )
    {
        // ���潺 ����� ����(121�� ����) �� ������ ���¸� ����ϱ� ���� ��Ŷ�� ������

        if ( pInfo && pInfo->m_SkillClassCode == eSKILL_DEFENCE_MOTION )
        {
            MSG_CG_STATUS_DELETE_CHAR_STATE_SYN send_packet;
            send_packet.m_eCharStateType = (eCHAR_STATE_TYPE)121;
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

            return FALSE;
        }
    }
#endif // _NA_008472_20150903_IMPROVED_BALANCE

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    // ������ ��ų�� ����Ϸ��� �Ҷ�
    // ����Ϸ��� ������ ��ų�� ���°� �̹� �ִٸ� ���¸� ����ϰ�, ������ �����Ѵ�
    if( pInfo->IsAuraSkill() == true )
    {
        if( g_pHero->CanUseSkillLimitCoolTime( pInfo->m_SkillCode, FALSE ) == TRUE )
        {
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            eCHAR_STATE_TYPE StateType = g_pHero->FindToggleSkillState( pInfo->m_SkillClassCode );
#else
            eCHAR_STATE_TYPE StateType = g_pHero->FindAuraSkillState( pInfo->m_SkillClassCode );
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            if( StateType != eCHAR_STATE_INVALID )
            {
                MSG_CG_STATUS_DELETE_CHAR_STATE_SYN send_packet;
                send_packet.m_eCharStateType = StateType;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

                return FALSE;
            }

#ifdef _NA_008252_20150427_AURASKILL_PACKET_PENDING
            if (CannotMoveState() == FALSE)
            {
                if ((g_pHero->ToggleSkill::GetPacketPending() == FALSE) &&
                    (g_pHero->CanUseSkill(pInfo->m_SkillCode, TRUE) == TRUE))
                {
                    g_pHero->ToggleSkill::SetPacketPending();
                    g_pHero->SendUseSkillPacket( pInfo->m_SkillCode );
                }
            }
            else
            {
                // 70535 �̻���¿� �ɸ� ���¿����� �ش� ��ų�� ����� �� �����ϴ�.
                TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceString(70535, string, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, string);
            }
#else
            g_pHero->SendUseSkillPacket( pInfo->m_SkillCode );
#endif //_NA_008252_20150427_AURASKILL_PACKET_PENDING
        }
        return TRUE;
    }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if ( pInfo->IsToggleSkill() == true )
    {
        if (pInfo->IsPassiveSkill() == true) // ��� �нú� ��ų�� ��� �� �� ����.
            return FALSE;

        if ( g_pHero->CanUseSkillLimitCoolTime( pInfo->m_SkillCode, FALSE ) == TRUE )
        {
            eCHAR_STATE_TYPE StateType = g_pHero->FindToggleSkillState( pInfo->m_SkillClassCode );
            if ( StateType != eCHAR_STATE_INVALID )
            {
                MSG_CG_STATUS_DELETE_CHAR_STATE_SYN send_packet;
                send_packet.m_eCharStateType = StateType;
                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

                return FALSE;
            }

#ifdef _NA_008252_20150427_AURASKILL_PACKET_PENDING
            if (CannotMoveState() == FALSE)
            {
                if ((g_pHero->ToggleSkill::GetPacketPending() == FALSE) &&
                    (g_pHero->CanUseSkill(pInfo->m_SkillCode, TRUE) == TRUE))
                {
                    g_pHero->ToggleSkill::SetPacketPending();
                    g_pHero->SendUseSkillPacket( pInfo->m_SkillCode );
                }
            }
            else
            {
                // 70535 �̻���¿� �ɸ� ���¿����� �ش� ��ų�� ����� �� �����ϴ�.
                TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceString(70535, string, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, string);
            }
            return TRUE;
#else //
            if (g_pHero->CanUseSkill(pInfo->m_SkillCode, TRUE) == TRUE)
            {
                g_pHero->SendUseSkillPacket( pInfo->m_SkillCode );
                return TRUE;
            }
#endif //_NA_008252_20150427_AURASKILL_PACKET_PENDING
        }
        else
        {
            eCHAR_STATE_TYPE StateType = g_pHero->FindToggleSkillState( pInfo->m_SkillClassCode );
            if ( StateType != eCHAR_STATE_INVALID )
            {
                MSG_CG_STATUS_DELETE_CHAR_STATE_SYN send_packet;
                send_packet.m_eCharStateType = StateType;

                GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));

                return FALSE;
            }
        }
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    if(GAMEOPTION->GetSkillQueueing()==FALSE)
    {
        if(g_pHero->GetCurState()==STATE::SKILL||g_pHero->GetNextState()==STATE::SKILL)
            return FALSE;
    }

#ifdef SKILL_EDIT
    // no operation
#else
    if (!g_pHero->CanUseSkillSlotState(dwSkillid, TRUE))
    {
        return FALSE;
    }
#endif

    if(NULL == pInfo)
    {
#ifdef _DEV_VER
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Invalid SkillInfo:%d", dwSkillid);
#endif //_DEV_VER
        return FALSE;
    }
    
    if (pInfo->m_byTarget != SKILL_TARGET_ME&&pInfo->m_byTarget != SKILL_TARGET_AREA&&pInfo->m_byTarget != SKILL_TARGET_SUMMON &&pInfo->m_byTarget != SKILL_TARGET_AREA_ENEMY_CORPSE) 
    {
        if( false == CanAttackInDominationWar() )
            return FALSE;
    }

    BOOL rt=g_SkillQueue.PushBack(dwSkillid,lsForceAttack());

    if(!rt)//������ ��带 ���� �߰� 
    {
        if(m_ShowTargetAreaPointer) //���� �����Ȱ� ���� �����̾����� 
        {
            m_ShowTargetAreaPointer=FALSE;
            g_KeyQueueManager.DeleteMsg(eDoType_Area_Pointer);
        }
    }

    return rt;
}


BOOL HeroActionInput::ProcessTargetClick( Character *pTarget, BOOL bAutoAttack )
{
    // Ÿ���� �ִ�. �׼�!
    if( IsShootingMode() == FALSE )
    {
        // �Ϲ� Melee ���� Ÿ��
        if( FALSE == OnLClickedCharacter( pTarget, GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTarget,lsForceAttack() ), false ) )
        {
            // ����
            return FALSE;
        }
        g_InterfaceManager.SetMovePointer( FALSE, NULL );
        SetAutoRun(FALSE);
        SetFollowState( FALSE );

        if( m_TargetType == RELATIONKIND::Enemy )
        {
            SetAutoAttack( bAutoAttack );
        }
    }
    else
    {
        if(g_pHero && (g_pHero->IsMoving() == TRUE))
        {
            // 45�� ���� �˻�
            WzVector vTarget = pTarget->GetPosition();
            WzVector vPos = g_pHero->GetPosition();
            WzVector vDiff = vTarget - vPos;
            float fTargetAngle = Math_GetAngleFromVector(&vDiff);
            float fAngle = g_pHero->GetAngle();

            float fDiff = abs(fTargetAngle - fAngle);
            float fAnotherDiff = WZ_PI * 2.0f - fDiff;

            // ������ 45���̻�Ǹ� ���� Ǯ��
            if (min(fDiff,fAnotherDiff) > WZ_PI / 4.0f )
            {
                SetAutoAttack(FALSE);
                g_pHero->LockOn( 0 );
            }
            else
            {
                g_pHero->LockOn( this->GetCurrentTarget() );

                // ���� ������ ���� Ÿ��
                if( FALSE == OnValkyrieLClickedCharacter(pTarget) )
                {
                    // ����
                    return FALSE;
                }
            }
        }
        else
        {
            // ���� ������ ���� Ÿ��
            if( FALSE == OnValkyrieLClickedCharacter(pTarget) )
            {
                // ����
                return FALSE;
            }
        }

        g_InterfaceManager.SetMovePointer( FALSE, NULL );
        SetAutoRun(FALSE);
        SetFollowState( FALSE );
        if( m_TargetType == RELATIONKIND::Enemy )
        {
            SetAutoAttack( bAutoAttack );
        }
    }

    return TRUE;
}

BOOL HeroActionInput::ProcessUserAction( SLOTCODE dwSkillid )
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if( g_pHero == NULL )
    {
        return FALSE;
    }

    if(!g_pHero->CanUseAction(dwSkillid))
    {
        //�����߻��Ұ�
        TCHAR szMessage[MAX_MESSAGE_LENGTH];
        g_InterfaceManager.GetInterfaceString(5139, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

        return FALSE;
    }

#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    if (dwSkillid == 35023)
    {
        // ���� ��Ŷ�� ��ٸ��� �ִ��� �Ǵ��ؾ� �Ѵ�.
        uiPerkDialog* perk_dialog_ptr = static_cast<uiPerkDialog*>(GET_DIALOG( IM_SKILL_MANAGER::SKILL_PERK_DLG ));
        if ( perk_dialog_ptr )
        {
            if (perk_dialog_ptr->get_is_wait_packet_())
                return FALSE;
        }
        SkillScriptInfo* skill_script_info_ptr = 
            SkillInfoParser::Instance()->GetSkillInfo(static_cast<SLOTCODE>(dwSkillid));
        util::Timer* timer_ptr = xGetCooltimeManager()->GetSkillCooltime(dwSkillid);
        if (!timer_ptr) return FALSE;

        // ��Ÿ���� ���� ���������� ������.
        if (!timer_ptr->IsExpired(FALSE))
        {
            TCHAR message_string[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(
                eST_SKILL_FAIL_COOLTIME_NOT_EXPIRED,
                message_string,
                INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,message_string);
            return FALSE;
        }
        else
        {
            // ��Ÿ�� ����
            // ��Ÿ�ӿ� �������̳� �ٸ� ���ǵ��� �ݿ����� �ʰ� ���밪���� �ݿ�
            xGetCooltimeManager()->StartSkillCooltime( skill_script_info_ptr->m_SkillCode, 
                                                       0, 
                                                       0 );
        }
    }
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM


    if( GetFollowState() )
    {
        SetFollowState( FALSE );
    }

    DWORD dwActionCode = dwSkillid - eACTION_START_SKILL_CODE;
    switch( dwActionCode )
    {

    //--------------------------------------------------------------------------------------
    //     �÷��̾� ���� �׼�
    //--------------------------------------------------------------------------------------

    case eACTION_AUTO_ATTACK:
        {
            Object   *pObj = g_ObjectManager.GetObject( GetCurrentTarget() );
            if( pObj )
            {
                Character   *pChar = (Character *)pObj;
                if( GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChar, lsForceAttack() ) == RELATIONKIND::Enemy)
                {
                    if( ProcessTargetClick( pChar ) == FALSE )
                    {
                        return FALSE;
                    }
                }
                else
                {
                    // ������ �� ���� ����Դϴ�.
                    g_InterfaceManager.GetInterfaceString( eST_CANNOT_ATTACK_TARGET, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                }
            }
            else
            {
                // ���õ� �÷��̾ �����ϴ�.
                g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            }
        }

        break;

    case eACTION_PICK_ITEM:
		// �������� ����~
        if( FALSE == OnSpacePushItem() )
        {
            // ����
            return FALSE;
        }
        break;

    case eACTION_FOLLOW_PLAYER:
        {
            DWORD dwTarget = GetCurrentTarget();

            if( dwTarget )
            {
                SetFollowState( TRUE, dwTarget );
            }
            else
            {
                // ���õ� �÷��̾ �����ϴ�.
                g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            }
        }
        break;

    case eACTION_SIT_AND_STAND:
        {
            if( g_pHero->GetCurState() == STATE::SIT )
            {
                MSG_CG_STATUS_CONDITION_CHANGE_SYN Packet;
                Packet.m_byCategory = CG_STATUS;
                Packet.m_byProtocol = CG_STATUS_CONDITION_CHANGE_SYN;      
                Packet.m_byConditionType = eCHAR_CONDITION_STANDUP;

                if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_STATUS_CONDITION_CHANGE_SYN) ) )
                {
                    assert(!"���� ��Ŷ ���� ����");
                    return FALSE;
                }

                GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CENTER_SHOW, _T("���� ��Ŷ ���ȴ�!"));
            }
            else
            {
                if( CanSitState() )
                {
                    MSG_CG_STATUS_CONDITION_CHANGE_SYN Packet;
                    Packet.m_byCategory = CG_STATUS;
                    Packet.m_byProtocol = CG_STATUS_CONDITION_CHANGE_SYN;      
                    Packet.m_byConditionType = eCHAR_CONDITION_SITDOWN;

                    if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_STATUS_CONDITION_CHANGE_SYN) ) )
                    {
                        assert(!"�ɱ� ��Ŷ ���� ����");
                        return FALSE;
                    }

                    GlobalFunc::DebugOutputMsg( eOUTPUT_MSG_CENTER_SHOW, _T("�ɱ� ��Ŷ ���ȴ�!"));

#ifdef _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
                    g_pHero->set_sitting_packet_delay(TRUE);
#endif //_NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
                }
            }
        }
        break;

    case eACTION_TRADE:
        {
            if(g_pHero && (GetCurrentTarget() == g_pHero->GetObjectKey()))
            {
                // �ڽſ��Դ� �ŷ��� ��û�� �� �����ϴ�. : 5370
                g_InterfaceManager.GetInterfaceString(eST_CANNOT_TRADE_MYSELF , szMessage , INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice (eOUTPUT_MSG_CHAT, szMessage);
                break;
            }
            else
            {
                if (GlobalFunc::IsSSQScene())
                {
                    break;
                }

                uiTradeMan* tradeMan = static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
                assert (tradeMan);

                if (tradeMan)
                {
                    Object * pObj = g_ObjectManager.GetObject( GetCurrentTarget() );
                    
                    if( pObj)
                    {
                        if( pObj->IsKindOfObject( PLAYER_OBJECT ) )
                        {
                            Player * player = (Player *) pObj;

                            if( player->IsDead() )
                            {
                                g_InterfaceManager.GetInterfaceString( 6340, szMessage, INTERFACE_STRING_LENGTH );
                                GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szMessage );
                                break;
                            }
                        }

                        tradeMan->Ask( pObj->GetName() );
                    }
                    else
                    {
                        // ���õ� �÷��̾ �����ϴ�.
                        g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                    }
                }
                else
                {
                    // /�ŷ� [���� ���̵�]
                    g_InterfaceManager.GetInterfaceString( eST_GM_TRADE_HELP, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                }
            }
        }
        break;

    case eACTION_VENDOR:
        {
            if (GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE )
            {
                if(g_pHero->IsRiding())
                {
                    g_InterfaceManager.GetInterfaceString( 6439, szMessage);
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                    break;
                }
                else
                {
                    if ( g_InterfaceManager.IsShowDialog( IM_VENDOR_MANAGER::VENDOR_SELL ) == FALSE
                        && (g_InterfaceManager.IsShowDialog(IM_BANK_MANAGER::BANK_DIALOG) == FALSE) )
                    {
                        GET_DIALOG( IM_VENDOR_MANAGER::VENDOR_SELL )->ShowWindow(TRUE);
                    }
                }
                
            }
            else
            {
                // ���� ���� ������ ���������� �����մϴ�.
                const int   c_iPRIVATE_TRADE = 70399;
                g_InterfaceManager.GetInterfaceString( c_iPRIVATE_TRADE, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            }
        }

        break;

    case eACTION_GUILD_INVITE:
        {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
            GuildSystem::Function::InviteTargetPlayer();
#else
            uiGuildMan* guildMan =
                static_cast<uiGuildMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD));

            if (guildMan)
            {
                guildMan->Invite(NULL);
            }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        }
        break;

    case eACTION_PARTY_LEAVE:
        {
            if ( g_pHero->IsParty() )
            {
                uiPartyLeavePopup * partyLeavePopup = GET_CAST_DIALOG( uiPartyLeavePopup, IM_PARTY_MANAGER::PARTY_LEAVE_POPUP );
                                
                if( partyLeavePopup )
                {
                    if( ( GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE ) ||
                        ( GameFramework::GetCurScene() == SCENE_TYPE_FIELD ) )
                        partyLeavePopup->LeaveParty();
                    else
                        partyLeavePopup->ShowInterface(TRUE);
                }
            }
            else
            {
                // ��Ƽ�� ���� ���� �ʽ��ϴ�.
                g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_PARTY, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            }

        }
        break;

    case eACTION_NORMAL_ATTACK:
        {
            Object   *pObj = g_ObjectManager.GetObject( GetCurrentTarget() );
            if( pObj )
            {
                Character   *pChar = (Character *)pObj;
                if( GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChar, lsForceAttack() ) == RELATIONKIND::Enemy)
                {
                    if( ProcessTargetClick( pChar, FALSE ) == FALSE )
                    {
                        return FALSE;
                    }
                }
                else
                {
                    // ������ �� ���� ����Դϴ�.
                    g_InterfaceManager.GetInterfaceString( eST_CANNOT_ATTACK_TARGET, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                }
            }
            else
            {
                // ���õ� �÷��̾ �����ϴ�.
                g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            }
        }

        break;

    case eACTION_FRIEND_ADD:
        {
            if (GlobalFunc::IsSSQScene())
            {
                break;
            }
            Object   *pObj = g_ObjectManager.GetObject( GetCurrentTarget() );
            if( pObj )
            {
                Character   *pChar = (Character *)pObj;

                if(pObj->IsKindOfObject(PLAYER_OBJECT))//������ ��� �߰� �Ҽ� �ִ�.
                {
                    _CallBack_Friend_Add(true, pChar->GetName());
                }
                else
                {
                    // ģ���� �߰��� �� ���� ����Դϴ�.
                    g_InterfaceManager.GetInterfaceString( eST_CANNOT_ADD_FRIEND, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                }
            }
            else
            {
                // ���õ� �÷��̾ �����ϴ�.
                g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            }

        }
        break;



    case eACTION_PARTY_MEMBER_SUMMON://��Ƽ���� �ɹ��� ��ȯ �Ҷ�
        {
            BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillid);
            util::Timer *pTimer =NULL;

            if(pInfo)
            {
                pTimer = xGetCooltimeManager()->GetSkillCooltime(pInfo->m_SkillCode);
            }


            if ( pTimer&&g_pHero&&!g_pHero->CanUseSkillLimitCoolTime(dwSkillid,TRUE) )
            {
                break;
            }

            if( pTimer&&!GENERALPARAM->IsNet() )
                pTimer->Reset();


#ifdef SKILL_EDIT
            // no operation
#else
            if (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_CHUNTING)
            {
                g_InterfaceManager.GetInterfaceString( eST_PARTY_SUMMON_MSG, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

                break;
            }
#endif
    
            uiPartyMan* partyMan =
                static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
            //��Ƽ �޴��� ���

            if(partyMan&&partyMan->IsLeader())
            {
                //

                Character *pTarget = (Character *)g_ObjectManager.GetObject(g_HeroInput.GetCurrentTarget());

                if(pTarget)
                {
                    if(pTarget->GetObjectKey()!=g_pHero->GetObjectKey()&&partyMan->IsPartyMember(pTarget->GetObjectKey()))
                    {
                        MSG_CG_PARTY_MEMBER_SUMMON_REQ_SYN sync;
                        sync.m_byCategory = CG_PARTY;
                        sync.m_byProtocol = CG_PARTY_MEMBER_SUMMON_REQ_SYN;
                        sync.m_dwObjKey=pTarget->GetObjectKey();
                        g_pHero->SendPacket(&sync,sizeof(sync));
                    
                        if(pTimer)
                            pTimer->Reset();
                    }
                    else
                    {
                        // ���õ� �÷��̾ �����ϴ�.
                        g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

                    }
                }
                else
                {
                    // ���õ� �÷��̾ �����ϴ�.
                    g_InterfaceManager.GetInterfaceString( eST_NOT_SELECTED_PLAYER, szMessage, INTERFACE_STRING_LENGTH );
                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

                }
            }
            else                                  
            {
                // 65002 ��Ƽ���� �ƴմϴ�.
                g_InterfaceManager.GetInterfaceString( 65002 , szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

            }
        }
        break;


    case eACTION_PARTY_MEMBER_WARP://����� ��Ƽ�忡�� ����
        {
            BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillid);
            util::Timer *pTimer =NULL;

            if(pInfo)
            {
                pTimer = xGetCooltimeManager()->GetSkillCooltime(pInfo->m_SkillCode);
            }


            if ( pTimer&&g_pHero&&!g_pHero->CanUseSkillLimitCoolTime(dwSkillid,TRUE) )
            {
                g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_COOLTIME_NOT_EXPIRED, szMessage, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
                break;
            }

            if( pTimer&&!GENERALPARAM->IsNet() )
                pTimer->Reset();


#ifdef SKILL_EDIT
            // no operation
#else
            if ((GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_CHUNTING)
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
                //&& (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_DOMINATION)
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
                )
            {
                g_InterfaceManager.GetInterfaceString( eST_USE_ONLY_HUNTTING, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);

                break;
            }
#endif
            uiPartyMan* partyMan =
                static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
            //��Ƽ�� �ϰ�� 

            if (partyMan &&
                (partyMan->GetPartyNum() >= 2) &&
                (partyMan->IsLeader() == FALSE))
            {
                uicPartyMemberInfoPlus *pInfo =
                    const_cast<uicPartyMemberInfoPlus *>(partyMan->GetPartyLeader());

                if (pInfo)
                {
                    //�ڽ��� �����Ѱ� �ƴϰ� ��Ƽ�� �ϰ�� CG_PARTY_MEMBER_WARP_REQ_SYN ������
                    _PARTY_MEMBER_INFO memInfo = pInfo->Get();
                    if ((memInfo.m_dwMemberKey != g_pHero->GetObjectKey()) && partyMan->IsPartyMember(memInfo.m_dwMemberKey))
                    {
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
                        if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_DOMINATION)
                        {
                            // ���������� ��Ƽ���� �Ʊ��� �� ����ϴ�
                            Player *party_leader = static_cast<Player*>(g_ObjectManager.GetObject(memInfo.m_dwMemberKey));
                            if (party_leader)
                            {
                                //RELATIONKIND::TYPE type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter();
                                if (party_leader->GetGuildRenderInfo().m_RelationType != eGUILD_RELATION_ALLIENCE)
                                {
                                    // 6173 ���Ͱ��谡 �ƴմϴ�.
                                    g_InterfaceManager.GetInterfaceString(6173, szMessage, INTERFACE_STRING_LENGTH);
                                    GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
                                    break;
                                }
                            }
                        }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
                        MSG_CG_PARTY_MEMBER_WARP_REQ_SYN sync;
                        sync.m_byCategory = CG_PARTY;
                        sync.m_byProtocol = CG_PARTY_MEMBER_WARP_REQ_SYN;

                        g_pHero->SendPacket(&sync, sizeof(sync));
                        if (pTimer)
                        {
                            pTimer->Reset();
                        }
                    }
                    else
                    {
                        g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NOT_PROPER_TARGET, szMessage, INTERFACE_STRING_LENGTH);
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                    }
                }
            }
            else
            {
                g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NOT_PROPER_TARGET, szMessage, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }  
        }
        break;



    //case 13:    // ��ý�Ʈ
    case eACTION_ASSIST:    // ��ý�Ʈ
        {
            TargetDialog* targetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
            
            if (!targetDlg)
                break;

            uiPartyMan* partyMan =
                static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
            if(partyMan)
            {
                if (partyMan->IsPartyMember(targetDlg->GetTargetKey()))
                {
                    Character*  pChar = (Character *)g_ObjectManager.GetObject(targetDlg->GetTargetKey());
                    if (!pChar)
                        break;

                    Character* pTargetChar = (Character *)pChar->GetTargetObject();
                    if (!pTargetChar)
                        break;

                    Monster *pMonster = (Monster *)g_ObjectManager.GetObject(pTargetChar->GetObjectKey());
                    if (pMonster)
                    {
                        // �ƹ���
                        const int   c_iPartyMemberName = 70395;
                        g_InterfaceManager.GetInterfaceString( c_iPartyMemberName, szMessage, INTERFACE_STRING_LENGTH );
                        util::_tstring strPartyMemberName = szMessage;

                        // �����ΰ�
                        const int   c_iTargetMonsterName = 70396;
                        g_InterfaceManager.GetInterfaceString( c_iTargetMonsterName, szMessage, INTERFACE_STRING_LENGTH );
                        util::_tstring strTargetMonsterName = szMessage;

                        strTargetMonsterName = pMonster->GetName();
                        strPartyMemberName = pChar->GetName();

                        // 5922  '%s'�� Ÿ���� ��ý�Ʈ�մϴ�.
                        TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
                        g_InterfaceManager.GetInterfaceString(5922, szTemp, INTERFACE_STRING_LENGTH);
                        Sprintf (szMessage, szTemp, strPartyMemberName.c_str());
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

                        g_HeroInput.SetCurrentTarget(pTargetChar->GetObjectKey());
                    }
                }
            }
        }
        break;

    //case 14:    // ��ý�Ʈ + ����
    case eACTION_ASSIST_AND_ATTACK:    // ��ý�Ʈ + ����
        {
            TargetDialog* targetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
            
            if (!targetDlg)
                break;

            uiPartyMan* partyMan =
                static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
            if(partyMan)
            {
                if (partyMan->IsPartyMember(targetDlg->GetTargetKey()))
                {
                    Character*  pChar = (Character *)g_ObjectManager.GetObject(targetDlg->GetTargetKey());
                    if (!pChar)
                        break;

                    Character* pTargetChar = (Character *)pChar->GetTargetObject();
                    if (!pTargetChar)
                        break;

                    Monster *pMonster = (Monster *)g_ObjectManager.GetObject(pTargetChar->GetObjectKey());
                    if (pMonster)
                    {
                        // �ƹ���
                        const int   c_iPartyMemberName = 70395;
                        g_InterfaceManager.GetInterfaceString( c_iPartyMemberName, szMessage, INTERFACE_STRING_LENGTH );
                        util::_tstring strPartyMemberName = szMessage;

                        // �����ΰ�
                        const int   c_iTargetMonsterName = 70396;
                        g_InterfaceManager.GetInterfaceString( c_iTargetMonsterName, szMessage, INTERFACE_STRING_LENGTH );
                        util::_tstring strTargetMonsterName = szMessage;

                        strTargetMonsterName = pMonster->GetName();
                        strPartyMemberName = pChar->GetName();

                        // 5922  '%s'�� Ÿ���� ��ý�Ʈ�մϴ�.
                        TCHAR szTemp[INTERFACE_STRING_LENGTH] = {0,};
                        g_InterfaceManager.GetInterfaceString(5922, szTemp, INTERFACE_STRING_LENGTH);
                        Sprintf (szMessage, szTemp, strPartyMemberName.c_str());
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

                        RELATIONKIND::TYPE RelationType = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTargetChar, lsForceAttack() );
                        g_HeroInput.OnLClickedCharacter( pTargetChar, RelationType, false );
                    }
                }
            }
        }
        break;

    case eACTION_TRADE_IN_CHAT_DLG: //15
        {
            uiTradeMan* tradeMan = static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
            assert (tradeMan);

            if (tradeMan && FALSE == GlobalFunc::IsSSQScene())
            {
                ChatPersonalOption* pDlg = GET_CAST_DIALOG(ChatPersonalOption, IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION);//(ChatPersonalOption*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT_PERSONAL_OPTION_DIALOG);
                if(pDlg && pDlg->IsVisibleWindow())
                {
                    TCHAR szVal[INTERFACE_STRING_LENGTH+1] = {0,};
                    pDlg->GetClickedCharName(szVal);
                    if('0' != szVal[0])
                        tradeMan->Ask(szVal);
                }
            }
        }
        break;

    case eACTION_FRIEND_ADD_IN_CHAT_DLG: //16
        {
            ChatPersonalOption* pDlg = GET_CAST_DIALOG(ChatPersonalOption, IM_CHAT_MANAGER::CHAT_PERSONAL_OPTION);//ChatPersonalOption* pDlg = (ChatPersonalOption*)g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CHAT_PERSONAL_OPTION_DIALOG);
            if(pDlg && pDlg->IsVisibleWindow() && FALSE == GlobalFunc::IsSSQScene())
            {
                TCHAR szVal[INTERFACE_STRING_LENGTH+1] = {0,};
                pDlg->GetClickedCharName(szVal);
                if('0' != szVal[0])
                    _CallBack_Friend_Add( true, szVal);
            }
        }
        break;

        
    // ���׸��� ���� �׼�
    case eACTION_ETHERIA_EXTRACT:
    {
    }
    break;

    //�ϳ��� ��ư���� Create/Destroy
    case eACTION_CRYSTAL_WARP_CREATE: //17
    case eACTION_CRYSTAL_WARP_DESTROY: //18
        {
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
            //3341 ���������� ��ȯ�� �� ���� �����Դϴ�.
            TCHAR message[INTERFACE_STRING_LENGTH + 1] = {0,};
            g_InterfaceManager.GetInterfaceString(3341, message);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message);
            break;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

            if(NULL == g_pHero)
                break;

            Object* pObject = g_ObjectManager.GetObject(GetCurrentTarget());
            if(pObject && pObject->IsKindOfObject(MONSTER_OBJECT))
            {
                Monster* pMonster = (Monster*)pObject;
                //�������� ������Ʈ�� ���õǾ� �ִٸ� ������Ʈ ����
                if(eNPC_CRYSTAL_WARP == pMonster->GetMonsterInfo()->m_byGrade)
                {
                    if( (g_pHero->GetGuildRenderInfo().m_GuildGuid!=INVALID_GUILDGUID) && 
                        g_pHero->GetPlayerGuildID() != pMonster->GetMasterGuildID())
                    {
#ifdef _DEV_VER
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, "Hero::GuildID:%d, Crystal::GuildID:%d", g_pHero->GetPlayerGuildID(), pMonster->GetMasterGuildID());
#endif
                        break;
                    }

                    if(RELATIONKIND::Friend == GameFramework::GetCurrentScene()->GetRelationKindOfCharacter((Character*)pMonster, lsForceAttack()))
                    {
                        InsertSkill(eACTION_CRYSTAL_WARP_DESTROY + eACTION_START_SKILL_CODE);
                    }
                }
                else
                {
                    InsertSkill(eACTION_CRYSTAL_WARP_CREATE + eACTION_START_SKILL_CODE);
                }

            }
            else
            {
                InsertSkill(eACTION_CRYSTAL_WARP_CREATE + eACTION_START_SKILL_CODE);
            }

        }
        break;

    case eACTION_IDENTIFY_SOCKET:
        {
            MouseHandler::Instance()->SetSocketIdentifyState();
            //	6483:���� ����� Ȯ���ϰ� ���� �������� Ŭ���ϼ���
            g_InterfaceManager.GetInterfaceString(6483, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            MouseHandler::Instance()->SetPendingUseChargeItemPos(INVALID_POSTYPE_VALUE);
            MouseHandler::Instance()->RollbackItemAtHand();
        }
        break;
#ifdef _NA_008016_20150202_OLD_RANKING_SYSTEM
#if defined(_NA_006802_20130704_REORGANIZATION_RANK_SYSTEM)
    case eACTION_RANKING_SYSTEM_LOAD:
        {
            // ��ŷ �ý����� ���⼭ �ҷ��´�.
            uiMissionRankMan2* rank_manager_ptr2 =  GET_DIALOG_MANAGER( uiMissionRankMan2, UIMAN_MISSION_RANK );

            uiBase* rank_list_ptr2 = GET_DIALOG( IM_MISSION_RANK_MANAGER::MissionRankList );

            if ( rank_manager_ptr2 && rank_list_ptr2 )
            {
                rank_manager_ptr2->ShowRankList( TRUE );
            }
        }
        break;
#endif // _NA_006802_20130704_REORGANIZATION_RANK_SYSTEM
#endif // _NA_008016_20150202_OLD_RANKING_SYSTEM
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    case eACTION_PERK_GROUP_CHANGE:
        {
            // Ŀ���� �нú긦 �׷����� ��ü �մϴ�.
            uiPerkDialog* perk_dialog_ptr = static_cast<uiPerkDialog*>(GET_DIALOG( IM_SKILL_MANAGER::SKILL_PERK_DLG ));
            if ( perk_dialog_ptr )
            {
                perk_dialog_ptr->PerkGroupChange_On();
            }
        }
        break;
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    case eACTION_ITEM_CRYSTALIZATION:
        {
			InventoryDialog* inventory_dialog_ptr = static_cast<InventoryDialog*>(GET_DIALOG( IM_INVENTORY_MANAGER::INVEN_DLG ));

			if ( inventory_dialog_ptr )
			{
				inventory_dialog_ptr->HideWindowProcess();
			}

            MouseHandler::Instance()->SetCrystalizationState();
            // 2165 : ����ȭ �� �������� Ŭ���ϼ���
            g_InterfaceManager.GetInterfaceString(2165, szMessage);

            if ( inventory_dialog_ptr )
            {
                inventory_dialog_ptr->ImpossibleCrystalizationItemIconDeActiveRender();
            }

            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            MouseHandler::Instance()->SetPendingUseChargeItemPos(INVALID_POSTYPE_VALUE);
            MouseHandler::Instance()->RollbackItemAtHand();
        }
        break;
    default:
        return FALSE;
        break;
    }

    return TRUE;
}


//------------------------------------------------------------------------------
// Method : ProcessUserEmoticon
// Returns :BOOL
// Parameter : SLOTCODE dwSkillid
// [2010-1-26 by i4u4me] ����  
//  1. �Լ� ���� �̵�
//             if(g_pHero->IsMove())
//                 g_pHero->SendStopMovePacket();
//  2. �Լ��� ���� ȣ�� �Լ��� ���� �ٲ� return false �Ҽ� �ִ� �κ� ���� ó���� ��� ����
//  3. assert ==> SUN_ASSERT �� ����
//------------------------------------------------------------------------------
BOOL HeroActionInput::ProcessUserEmoticon(SLOTCODE dwSkillid)
{
    if (g_pHero == NULL)
    {
        return FALSE;
    }

    if ((g_pHero->IsRiding()) || 
        (g_pHero->GetCurState() == STATE::SUMMON_RIDER) || 
        (g_pHero->GetCurState() == STATE::WINGS_SPREAD) || 
        (g_pHero->is_spread_wings()) ||
        (g_pHero->IsMove()))
    {
        //6439 �ش� ����� ����Ҽ������ϴ�.
        TCHAR szMessage[INTERFACE_STRING_LENGTH] = {NULL,};
        g_InterfaceManager.GetInterfaceString(6439, szMessage);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

        return FALSE;
    }

    if(g_pHero->IsTransForm())
    {
        //�����߻��Ұ�
        TCHAR szMessage[MAX_MESSAGE_LENGTH] = {0,};
        g_InterfaceManager.GetInterfaceString(5139, szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

        return FALSE;
    }

    // ���� ǥ���� �Ҽ� ���� ���������� üũ�ؼ�
    if (g_pHero->CannotAction())
    {
        return FALSE;
    }

    if (GetFollowState())
    {
        SetFollowState(FALSE);
    }

    if (GENERALPARAM->IsNet()==FALSE)
    {
        BASE_SKILLINFO* pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillid);
        if(pInfo)
        {
            g_pHero->SetEmotion(pInfo->m_wzidAnimationCode[0].id);
            g_pHero->SetNextState(STATE::EMOTICON, g_CurTime);
        }
    }
    else
    {
        //�̸���߿��� �ٸ� �̸���� ��Ŷ�� ������ �ʴ´�.
        if(g_pHero->GetCurState() == STATE::EMOTICON)
        {
            return FALSE;
        }

        MSG_CG_STATUS_EMOTION_SYN SendPacket;
        ZeroMemory(&SendPacket,sizeof(SendPacket));
        
        SendPacket.m_byCategory = CG_STATUS;
        SendPacket.m_byProtocol = CG_STATUS_EMOTION_SYN;
        SendPacket.m_byEmotionType   = BYTE(dwSkillid - eEMOTION_START_CODE);
        if (FALSE == g_pHero->SendPacket(&SendPacket, sizeof(MSG_CG_STATUS_EMOTION_SYN)))
        {
            SUN_ASSERT(!"�̸�� ��Ŷ ���� ����");
            return FALSE;
        }

    }

    return TRUE;
}


//------------------------------------------------------------------------------
/**
    Todo:��ų ť�� �հ� �ִٸ� ó�� ��� ���ǿ� ���� �ϸ� ť�� ����.
*/
BOOL HeroActionInput::ProcessQueueSkill()
{
	if( g_pHero == NULL )
		return FALSE;

#ifdef _DEBUG
    int SkillQueueCount = g_SkillQueue.Size();
    SkillQueueCount = SkillQueueCount;
#endif //_DEBUG

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    DWORD dwSkillID=g_SkillQueue.GetFrontSkillID();
    BOOL  bForceAttack=g_SkillQueue.GetFrontForceAttack();

    // ���� ��ų ������̸� ��ٸ���.
    if( g_pHero->GetCurState() == STATE::SKILL)
    {
        return FALSE;
    }

    SetAutoRun(FALSE); //��ų ��� �ϸ� ���䷱ ���� 
    SetFollowState( FALSE );

    BASE_SKILLINFO* pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillID);
    if( pSkillInfo == NULL )
    {
        g_SkillQueue.Pop();	// ť���� �����ϰ� ������
        return TRUE;
    }

    //���� ��� �ΰ�? ���� ��ų �������� ���� 
    if(g_pHero->IsCancelTransForm(pSkillInfo->m_SkillClassCode))
    {
        return TRUE;
    }

	// Ÿ������ ��ų�ΰ�� Ÿ������ ���� ����
    if( pSkillInfo->m_byTarget == SKILL_TARGET_AREA || pSkillInfo->m_byTarget == SKILL_TARGET_REACHABLE_ME )
    {
        if( m_ShowTargetAreaPointer == FALSE )
        {
            m_TargetAreaSkillID = (SLOTCODE)dwSkillID;
            m_IsClickedTargetArea = FALSE;

            // ���� ������ų!! ������ Ŭ���ϼ���!!
            const int c_iClickArea = 70269;
            g_InterfaceManager.GetInterfaceString( c_iClickArea, szMessage, INTERFACE_STRING_LENGTH );
            g_InterfaceManager.ShowMessageBox(szMessage, 3000, NULL, 0);

            SetAreaSkill(TRUE);

        }
    }

	// Ÿ������ ����ΰ��, ��ŷ���� ������ ������ FALSE
    if( m_ShowTargetAreaPointer == TRUE || m_IsClickedTargetArea == TRUE )
    {
        if( UseAreaSkill() == FALSE)
        {
            SetAreaSkill(FALSE);
            m_IsClickedTargetArea = FALSE;
            g_SkillQueue.Pop();
            return TRUE;
        }
    }
    else
    {
        if( UseSkill(dwSkillID,bForceAttack) == FALSE )
        {
            SetAreaSkill(FALSE);
            g_SkillQueue.Pop();
            return TRUE;
        }
    }

    if(m_QueueAction.ActionID!=ACTION_SKILL)
        return FALSE;

	if(m_QueueAction.SKILL.dwSkillID != dwSkillID )
		return FALSE;

    if (g_pHero->CanDoQueueAction(&m_QueueAction)) 
    {
        BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)m_QueueAction.SKILL.dwSkillID);


        // ��ų�� ����� �⺻���� ������ �ȵǸ� ����
        if (pInfo==NULL||!g_pHero->CanUseSkill(m_QueueAction.SKILL.dwSkillID))
        {
            SetAreaSkill(FALSE);
            g_SkillQueue.Pop();
            return TRUE;
        }

        
        util::Timer *pTimer = NULL;
        if(   (eACTION_CRYSTAL_WARP_DESTROY + eACTION_START_SKILL_CODE) == pInfo->m_SkillCode )
        {
            pTimer = xGetCooltimeManager()->GetSkillCooltime(eACTION_CRYSTAL_WARP_CREATE + eACTION_START_SKILL_CODE);
        }
        else
        {
            pTimer = xGetCooltimeManager()->GetSkillCooltime(pInfo->m_SkillCode);
        }

        if (!pTimer) 
        {
            SetAreaSkill(FALSE);
            g_SkillQueue.Pop();
            return TRUE;
        }

        // ���������߿��� ��ų ����
        if (IsShootingMode()) 
        {
            if (g_pHero->GetValkyrieShotDelay() > 30)
            {
                return FALSE;//�����̴� ��ٸ���
            }
        }

        // ���ڸ����� �ٷ� ����ϴ� ��ų�ϰ��,Ư�� ������ Ŭ���ؼ� ����ϴ� ��� 
        if (pInfo->m_byTarget == SKILL_TARGET_ME||pInfo->m_byTarget == SKILL_TARGET_SUMMON || pInfo->m_byTarget == SKILL_TARGET_AREA_ENEMY_CORPSE)
        {  
            g_pHero->DoAction(&m_QueueAction);
            SetAreaSkill(FALSE);
            g_SkillQueue.Pop();
            return TRUE;
        }
        else if(pInfo->m_byTarget ==SKILL_TARGET_AREA)
        {
            PLAYER_ACTION skillaction;
            ZeroMemory(&skillaction,sizeof(skillaction));
            skillaction.ActionID = ACTION_SKILL;
            skillaction.SKILL.dwSkillID = m_TargetAreaSkillID;
            skillaction.SKILL.dwTargetID = this->GetCurrentTarget();
            skillaction.SKILL.vTargePos = m_TargetAreaPosition;
            skillaction.SKILL.bForceAttack=bForceAttack;

            m_QueueAction = skillaction;
            float fLength = GetActionLength(m_QueueAction);

            WzVector vTargetPos = m_TargetAreaPosition;
            WzVector vDistance = vTargetPos - g_pHero->GetPosition();
            float fDistance = VectorLength(&vDistance);

            if (fDistance <= fLength)
            {
                g_pHero->DoAction(&m_QueueAction);
                SetAreaSkill(FALSE);
                m_IsClickedTargetArea = FALSE;

                g_SkillQueue.Pop();
                return TRUE;
            }

            return FALSE;

        }
        else if(pInfo->m_byTarget ==SKILL_TARGET_FRIEND_CORPSE)//��Ƽ���� ��ü�϶�
        {
            float fLength = GetActionLength(m_QueueAction);
            Character* pChr = (Character *)g_ObjectManager.GetObject(m_QueueAction.SKILL.dwTargetID);

            if (pChr)
            {
                WzVector vTargetPos = pChr->GetPosition();
                WzVector vDistance = vTargetPos - g_pHero->GetPosition();
                float fDistance = VectorLength(&vDistance);

                if (fDistance <= fLength)
                {
                    g_pHero->DoAction(&m_QueueAction);
                    SetAreaSkill(FALSE);
                    g_SkillQueue.Pop();
                    return TRUE;
                }
            }
        }
        else if(pInfo->m_byTarget ==SKILL_TARGET_ENEMY_CORPSE)//������ü�϶�
        {
            float fLength = GetActionLength(m_QueueAction);
            Character* pChr = (Character *)g_ObjectManager.GetObject(m_QueueAction.SKILL.dwTargetID);

            if (pChr)
            {
                WzVector vTargetPos = pChr->GetPosition();
                WzVector vDistance = vTargetPos - g_pHero->GetPosition();
                float fDistance = VectorLength(&vDistance);

                if (fDistance <= fLength)
                {
                    g_pHero->DoAction(&m_QueueAction);
                    SetAreaSkill(FALSE);
                    g_SkillQueue.Pop();
                    return TRUE;
                }
            }
        }

        // ��ǥ�� �ִ� ���
        else
        {
            float fLength = GetActionLength(m_QueueAction);
            Character* pChr = (Character *)g_ObjectManager.GetObject(m_QueueAction.SKILL.dwTargetID);

            if (pChr&&!pChr->CanBeAttackTarget())
            {
                SetAreaSkill(FALSE);
                g_SkillQueue.Pop();
                return TRUE;
            }

            if (pChr)
            {
                WzVector vTargetPos = pChr->GetPosition();
                WzVector vDistance = vTargetPos - g_pHero->GetPosition();
                float fDistance = VectorLength(&vDistance);

        
                if (fDistance <= fLength)
                {

                    //// ��ǥ ���� ������ �̵��ؾ��ϴ� ��ų�ϰ�� SKILLMOVE ��ǥ�� ������ش�.
                    //// ������ �ӽ÷� �ϵ��ڵ�
                    //if (pInfo->m_SkillClassCode == eSKILL_ASSAULT )
                    //{                  

                    //    WzVector vDest;
                    //    
                    //    // �ʹ� �ָ� ����
                    //    // 1.0f�� �ӽ÷� �־���� ��
                    //    if (fDistance > 1.0f)
                    //    {
                    //        WzVector vDirection;

                    //        VectorNormalize(&vDirection,&vDistance);
                    //    
                    //        vDest = vTargetPos + (vDirection * -0.9f);

                    //    }
                    //    else 
                    //    {
                    //        vDest = g_pHero->GetPosition();
                    //    }

                    //    m_QueueAction.SKILL.vDestPos.wvPos = vDest;
                    //    m_QueueAction.SKILL.vDestPos.wTile = -1;
                    //    m_QueueAction.SKILL.bForceAttack=bForceAttack;
                    //}

                    //if (pInfo->m_SkillClassCode == eSKILL_EXPLOZEN_ATTACK )
                    //{                  

                    //    WzVector vDest;

                    //    // �ʹ� �ָ� ����
                    //    // 1.0f�� �ӽ÷� �־���� ��
                    //    if (fDistance > 1.0f)
                    //    {
                    //        WzVector vDirection;

                    //        VectorNormalize(&vDirection,&vDistance);

                    //        vDest = vTargetPos + (vDirection * -0.9f);

                    //    }
                    //    else 
                    //    {
                    //        vDest = g_pHero->GetPosition();
                    //    }

                    //    m_QueueAction.SKILL.vDestPos.wvPos = vDest;
                    //    m_QueueAction.SKILL.vDestPos.wTile = -1;
                    //    m_QueueAction.SKILL.bForceAttack=bForceAttack;
                    //}

                    //if (pInfo->m_SkillClassCode == eSKILL_MACHSLASH)
                    //{                  

                    //    WzVector vDest;
                    //    
                    //    WzVector vDirection;

                    //    VectorNormalize(&vDirection,&vDistance);

                    //    float fLen=pInfo->m_wSkillRange/10.0f;

                    //    fLen+=1.0f;

                    //    vDest = g_pHero->GetPosition() + (vDirection *fLen); //��Ÿ� ��ŭ �̵��ض�.

                    //    
                    //    m_QueueAction.SKILL.vDestPos.wvPos = vDest;
                    //    m_QueueAction.SKILL.vDestPos.wTile = -1;
                    //    m_QueueAction.SKILL.bForceAttack=bForceAttack;

                    //}


      //              if (/*pInfo->m_SkillClassCode == eSKILL_DARK_STUN || */
						///*pInfo->m_SkillClassCode == eSKILL_DARK_BREAK || */
						//pInfo->m_SkillClassCode == eSKILL_TACKLE || 
						///*pInfo->m_SkillClassCode == eSKILL_CHARGE_ATTACK */
						////|| pInfo->m_SkillClassCode == eSKILL_DASH 
						//)
      //              {                  
      //                  WzVector vDest;
      //                  WzVector vDirection;
      //                  WzVector vOffset;

      //                  VectorNormalize(&vDirection,&vDistance);
      //                  vOffset = vDirection * 2.f;
      //                  vDest = g_pHero->GetPosition() + (vDistance - vOffset);

      //                  m_QueueAction.SKILL.vDestPos.wvPos = vDest;
      //                  m_QueueAction.SKILL.vDestPos.wTile = -1;
      //                  m_QueueAction.SKILL.bForceAttack=bForceAttack;
      //              }

                    g_pHero->DoAction(&m_QueueAction);
                    SetAreaSkill(FALSE);
                    g_SkillQueue.Pop();
                    return TRUE;
                }
            }
            else
            {
                return TRUE;
            }
        }
    }
    
    return FALSE;
}

//------------------------------------------------------------------------------
/**
    Ŭ���� ���̵�� ���� �κ��� ��ų �ڵ带 ��´�. 
*/
DWORD HeroActionInput::GetSkillCode(DWORD dwClassId,BOOL & bActive,float &fDistance)
{
	if( SkillData* skill_data = xGetSkillData()->GetSkillDataBySkillClassCode(dwClassId) )
    {
        if( SkillScriptInfo* skill_info = skill_data->GetCurSkillInfo() )
        {
			if( skill_data->GetState() == SLOT_RENDER_STATE_ACTIVATED)
			{
				bActive = TRUE;
			}
			else
			{
				bActive = FALSE; 
			}			

            fDistance = GetSkillRange(skill_info);
			//fDistance = static_cast<float>(skill_info->m_wSkillRange / 10.0f);

			return skill_info->m_SkillCode;
		}
    }

    return 0;
}

//------------------------------------------------------------------------------
/**
    ��Ű�� Ű���� �̵� 
*/
BOOL HeroActionInput::ProcessValkyrieKeyboardMove()
{
    // ���� ���� �÷��װ� ���� ���·� �����Ϸ� �پ�� ��, Ű���� �̵��� �����ٸ� üũ�ϱ� ���� �÷���
    m_bKeyBoardMoveCheck = FALSE;

    DWORD dwCurTick = g_pSunRenderer->x_Clock.GetCurrentTick();


    m_KeyboardState.UpdateSpecialMove2( dwCurTick );


    Character* pChar = (Character*)g_ObjectManager.GetObject( GetCurrentTarget() );

    BOOL bRange = FALSE;
    if( pChar != NULL )
    {
        WzVector vPos = g_pHero->GetPosition();
        WzVector vDest = pChar->GetPosition();
        WzVector vDiff = vDest - vPos;
		float fLength = VectorLength(&vDiff);

        if( fLength <= GetAttackActionLength() + pChar->GetBodyRadius() )
        {
            bRange = TRUE;
        }
    }

    BOOL bUse = TRUE;

#ifdef _NA_000000_20160315_ETHERWEAPON_SPECIALMOVE
#else
    if (g_pHero->GetWeaponKind() == eWEAPONTYPE_ETHERWEAPON) 
    {
        bUse=FALSE;
    }
#endif //_NA_000000_20160315_ETHERWEAPON_SPECIALMOVE

    RELATIONKIND::TYPE RelationType = RELATIONKIND::None;
    if( pChar )
    {
        RelationType = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChar, lsForceAttack() );
    }

    //�������̰ų� ������ ���ɼ��� ������ Ư�� �̵��̴�.
    if (bUse &&
        ((g_pHero->GetCurState() == STATE::ATTACK) || (g_pHero->GetNextState() == STATE::ATTACK) ||
        (m_bAutoAttack && pChar && (RelationType == RELATIONKIND::Enemy) 
        && (pChar->IsDead() == FALSE) && bRange)))
    {

        if( m_KeyboardState.GetSpecialMoveLeftStep() )
        {
            CancelSkillQueue();

            m_QueueAction.ActionID = ACTION_SPECIAL_MOVE;
            m_QueueAction.SPECIALMOVE.bType = CMS_TUMBLING_LEFT;

            return FALSE;
        }
        else if( m_KeyboardState.GetSpecialMoveRightStep() )
        {
            CancelSkillQueue();

            m_QueueAction.ActionID = ACTION_SPECIAL_MOVE;
            m_QueueAction.SPECIALMOVE.bType = CMS_TUMBLING_RIGHT;

            return FALSE;
        }
        else if( m_KeyboardState.GetSpecialMoveFoward() )
        {
            CancelSkillQueue();
            m_QueueAction.ActionID = ACTION_SPECIAL_MOVE;
            m_QueueAction.SPECIALMOVE.bType = CMS_TUMBLING_FRONT;

            return FALSE;
        }  
        else if( m_KeyboardState.GetSpecialMoveBackword() )
        {
            CancelSkillQueue();
            m_QueueAction.ActionID = ACTION_SPECIAL_MOVE;
            m_QueueAction.SPECIALMOVE.bType = CMS_TUMBLING_BACK;
            return FALSE;
        }


        if( m_KeyboardState.GetLeftTurnPressed() == TRUE || 
            m_KeyboardState.GetRightTurnPressed() == TRUE )
        {
            SetAutoAttack(FALSE);
            g_pHero->StopMove();
            g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
        }

        return FALSE;
    }


    if( m_QueueAction.ActionID == ACTION_SPECIAL_MOVE )
        return TRUE;

    if( GetAutoRun() == TRUE )
    {
        if( m_KeyboardState.GetFowardPressed() == TRUE || m_KeyboardState.GetBackwardPressed() == TRUE )
        {
            SetAutoRun(FALSE);
        }
    }

    if( m_KeyboardState.IsKeyboardPressed() )
    {
        //SetAutoAttack(FALSE);
        SetFollowState(FALSE);

        // ��� ����� �۾� ���.         
        InitTerm();

        CancelSkillQueue();

        m_bKeyBoardMoveCheck = TRUE;

        if (IsIdleState() || g_pHero->GetCurState() == STATE::MOVE )
        {
            g_pHero->StopMove();
            g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
        }
        else if(STATE::COLLECT == g_pHero->GetCurState())
        {
            MapObject* pMapObj = (MapObject*)g_ObjectManager.GetObject(g_pHero->GetCollectObjKey());
            if(pMapObj)
            {
                TCHAR* debug_message = NULL;
#ifdef _DEV_VER
                debug_message = _T("MSG_CG_COLLECT_CANCEL_SYN[Move]");
#endif 
                StateCollect::SendCollectCancelSyn(debug_message);
            }

            g_pHero->StopMove();
            g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
        }


        if( m_KeyboardState.GetLeftTurnPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_LEFTTURN);
        }
        else if( m_KeyboardState.GetRightTurnPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_RIGHTTURN);
        }

        if( m_KeyboardState.GetFowardPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_FOWARD);
        }
        else if( m_KeyboardState.GetBackwardPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_BACKWARD);
        }

#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
        if (bIsRotateL)
        {
            // ���콺 Ŭ������ ȭ�� ���� ���� + ����or���� �̵�,  �¿�Ű�� ����
            if (g_pHero->CheckMoveFlag(KEYBOARDMOVE_FOWARD) ||
                g_pHero->CheckMoveFlag(KEYBOARDMOVE_BACKWARD))
            {
                g_pHero->RemoveMoveFlag(KEYBOARDMOVE_LEFTTURN);
                g_pHero->RemoveMoveFlag(KEYBOARDMOVE_RIGHTTURN);
            }
        }
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX

        if (!g_bObserverMode)
        {
            if( m_KeyboardState.GetRightStepPressed() )
            {
                g_pHero->AddMoveFlag(KEYBOARDMOVE_RIGHTSTEP);
            }
            else if( m_KeyboardState.GetLeftStepPressed() )
            {
                g_pHero->AddMoveFlag(KEYBOARDMOVE_LEFTSTEP);
            }
        }


        return TRUE;
    }

    return FALSE;
}


BOOL HeroActionInput::ProcessDefaultKeyboardMove()
{
    // ���� ���� �÷��װ� ���� ���·� �����Ϸ� �پ�� ��, Ű���� �̵��� �����ٸ� üũ�ϱ� ���� �÷���
    m_bKeyBoardMoveCheck = FALSE;



    // �ڵ��޸������϶� ���� ������ ������ ���
    if( GetAutoRun() == TRUE )
    {
        if( m_KeyboardState.GetFowardPressed() || m_KeyboardState.GetBackwardPressed() )
        {
            SetAutoRun(FALSE);
        }
    }


    // Ű���� �̵��� �Ѱ��
    if( m_KeyboardState.IsKeyboardPressed() )
    {
        // ���󰡱�, �ڵ������� ���
        SetFollowState(FALSE);
        SetAutoAttack(FALSE);

        // ��� ����� �۾� ���.         
        InitTerm();

        CancelSkillQueue();

        m_bKeyBoardMoveCheck = TRUE;

        if (IsIdleState() || g_pHero->GetCurState() == STATE::MOVE )
        {
            if(g_pHero->IsMoving())
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }

            g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
        }
        else if( StateSkill* pSkillState = g_pHero->GetCurSkillState() )
        {
            if( pSkillState->GetSkillClassCode() == eSKILL_HP_SUCTION ||
                pSkillState->GetSkillClassCode() == eSKILL_MP_SUCTION )
            {
                g_pHero->SendStopMovePacket();
                g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
            }
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
            else if( pSkillState->GetSkillClassCode() == eSKILL_HELLROID_1015 )
            {
                // ����̵� ����
                SkillStateHellroidSpin* pState = (SkillStateHellroidSpin*)pSkillState;

                if( m_KeyboardState.GetLeftTurnPressed() )
                {
                    g_pHero->AddMoveFlag(KEYBOARDMOVE_LEFTTURN);
                }
                else if( m_KeyboardState.GetRightTurnPressed() )
                {
                    g_pHero->AddMoveFlag(KEYBOARDMOVE_RIGHTTURN);
                }

                if( m_KeyboardState.GetFowardPressed() )
                {
                    g_pHero->AddMoveFlag(KEYBOARDMOVE_FOWARD);
                }
                else if( m_KeyboardState.GetBackwardPressed() )
                {
                    g_pHero->AddMoveFlag(KEYBOARDMOVE_BACKWARD);
                }

#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
                if (bIsRotateL)
                {
                    // ���콺 Ŭ������ ȭ�� ���� ���� + ����or���� �̵�,  �¿�Ű�� ����
                    if (g_pHero->CheckMoveFlag(KEYBOARDMOVE_FOWARD) ||
                        g_pHero->CheckMoveFlag(KEYBOARDMOVE_BACKWARD))
                    {
                        g_pHero->RemoveMoveFlag(KEYBOARDMOVE_LEFTTURN);
                        g_pHero->RemoveMoveFlag(KEYBOARDMOVE_RIGHTTURN);
                    }
                }
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX

                if ( g_bObserverMode == FALSE )
                {
                    if( m_KeyboardState.GetRightStepPressed() )
                    {
                        g_pHero->AddMoveFlag(KEYBOARDMOVE_RIGHTSTEP);
                    }
                    else if( m_KeyboardState.GetLeftStepPressed() )
                    {
                        g_pHero->AddMoveFlag(KEYBOARDMOVE_LEFTSTEP);
                    }
                }

                pState->SetHeroKeyboardMove();
                return TRUE;
            }
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        }
        else if(STATE::COLLECT == g_pHero->GetCurState())
        {
            MapObject* pMapObj = (MapObject*)g_ObjectManager.GetObject(g_pHero->GetCollectObjKey());
            if(pMapObj)
            {
                TCHAR* debug_message = NULL;
#ifdef _DEV_VER
                debug_message = _T("MSG_CG_COLLECT_CANCEL_SYN[Move]");
#endif 
                StateCollect::SendCollectCancelSyn(debug_message);
            }

            g_pHero->StopMove();
            g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
        }


        if( m_KeyboardState.GetLeftTurnPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_LEFTTURN);
        }
        else if( m_KeyboardState.GetRightTurnPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_RIGHTTURN);
        }

        if( m_KeyboardState.GetFowardPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_FOWARD);
        }
        else if( m_KeyboardState.GetBackwardPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_BACKWARD);
        }

#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
        if (bIsRotateL)
        {
            // ���콺 Ŭ������ ȭ�� ���� ���� + ����or���� �̵�,  �¿�Ű�� ����
            if (g_pHero->CheckMoveFlag(KEYBOARDMOVE_FOWARD) ||
                g_pHero->CheckMoveFlag(KEYBOARDMOVE_BACKWARD))
            {
                g_pHero->RemoveMoveFlag(KEYBOARDMOVE_LEFTTURN);
                g_pHero->RemoveMoveFlag(KEYBOARDMOVE_RIGHTTURN);
            }
        }
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX

        if ( g_bObserverMode == FALSE )
        {
            if( m_KeyboardState.GetRightStepPressed() )
            {
                g_pHero->AddMoveFlag(KEYBOARDMOVE_RIGHTSTEP);
            }
            else if( m_KeyboardState.GetLeftStepPressed() )
            {
                g_pHero->AddMoveFlag(KEYBOARDMOVE_LEFTSTEP);
            }
        }

        return TRUE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
    ������Ż����Ʈ Ű���� �̵� 
*/
BOOL HeroActionInput::ProcessElementalistKeyboardMove()
{
    // ���� ���� �÷��װ� ���� ���·� �����Ϸ� �پ�� ��, Ű���� �̵��� �����ٸ� üũ�ϱ� ���� �÷���
    m_bKeyBoardMoveCheck = FALSE;

    DWORD dwCurTick = g_pSunRenderer->x_Clock.GetCurrentTick();

    m_KeyboardState.UpdateSpecialMove( dwCurTick );


    // �����̵�
    if( m_KeyboardState.GetSpecialMoveLeftStep() )
    {
        WzVector vFoward = g_pHero->GetDirection();

        WzVector vLeft;
        vLeft.x = -vFoward.y;
        vLeft.y = vFoward.x;
        vLeft.z = 0.0f;
        ProcessTeleportMoveSkill(vLeft);

        return FALSE;
    }
    else if( m_KeyboardState.GetSpecialMoveRightStep() )
    {
        WzVector vFoward = g_pHero->GetDirection();

        WzVector vRight;
        vRight.x = vFoward.y;
        vRight.y = -vFoward.x;
        vRight.z = 0.0f;
        ProcessTeleportMoveSkill(vRight);

        return FALSE;
    }
    else if( m_KeyboardState.GetSpecialMoveFoward() )
    {
        WzVector vFoward = g_pHero->GetDirection();
        ProcessTeleportMoveSkill(vFoward);

        return FALSE;
    }  
    else if( m_KeyboardState.GetSpecialMoveBackword() )
    {
        WzVector vFoward = g_pHero->GetDirection();
        ProcessTeleportMoveSkill(-vFoward);

        return FALSE;
    }  


    // �ڵ��޸������϶� ���� ������ ������ ���
    if( GetAutoRun() == TRUE )
    {
        if( m_KeyboardState.GetFowardPressed() || m_KeyboardState.GetBackwardPressed() )
        {
            SetAutoRun(FALSE);
        }
    }

    // Ű���� �̵��� �Ѱ��
    if( m_KeyboardState.IsKeyboardPressed() )
    {
        // ���󰡱�, �ڵ������� ���
        SetFollowState(FALSE);
        SetAutoAttack(FALSE);

        // ��� ����� �۾� ���.         
        InitTerm();
        
        CancelSkillQueue();

        m_bKeyBoardMoveCheck = TRUE;

        if (IsIdleState() || g_pHero->GetCurState() == STATE::MOVE )
        {
            g_pHero->StopMove();
            g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
        }
        else if(STATE::COLLECT == g_pHero->GetCurState())
        {
            MapObject* pMapObj = (MapObject*)g_ObjectManager.GetObject(g_pHero->GetCollectObjKey());
            if(pMapObj)
            {
                TCHAR* debug_message = NULL;
#ifdef _DEV_VER
                debug_message = _T("MSG_CG_COLLECT_CANCEL_SYN[Move]");
#endif 
                StateCollect::SendCollectCancelSyn(debug_message);
            }

            g_pHero->StopMove();
            g_pHero->SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
        }

        if( m_KeyboardState.GetLeftTurnPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_LEFTTURN);
        }
        else if( m_KeyboardState.GetRightTurnPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_RIGHTTURN);
        }

        if( m_KeyboardState.GetFowardPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_FOWARD);
        }
        else if( m_KeyboardState.GetBackwardPressed() )
        {
            g_pHero->AddMoveFlag(KEYBOARDMOVE_BACKWARD);
        }

#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
        if (bIsRotateL)
        {
            // ���콺 Ŭ������ ȭ�� ���� ���� + ����or���� �̵�,  �¿�Ű�� ����
            if (g_pHero->CheckMoveFlag(KEYBOARDMOVE_FOWARD) ||
                g_pHero->CheckMoveFlag(KEYBOARDMOVE_BACKWARD))
            {
                g_pHero->RemoveMoveFlag(KEYBOARDMOVE_LEFTTURN);
                g_pHero->RemoveMoveFlag(KEYBOARDMOVE_RIGHTTURN);
            }
        }
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX

        if ( g_bObserverMode == FALSE )
        {
            if( m_KeyboardState.GetRightStepPressed() )
            {
                g_pHero->AddMoveFlag(KEYBOARDMOVE_RIGHTSTEP);
            }
            else if( m_KeyboardState.GetLeftStepPressed() )
            {
                g_pHero->AddMoveFlag(KEYBOARDMOVE_LEFTSTEP);
            }
        }

        return TRUE;
    }

    return FALSE;
}

void HeroActionInput::ProcessTeleportMoveSkill(WzVector teleport_direction)
{
	if( g_pHero->IsPolymorph() == TRUE )
		return;

    SetAutoRun(FALSE);
    SetFollowState( FALSE );
    SetAutoAttack(FALSE);

    DWORD move_skill_class_code = 0;

    if( g_pHero->GetClass() == eCHAR_ELEMENTALIST )
    {
        move_skill_class_code = eSKILL_TELEPORT;
    }
    else if( g_pHero->GetClass() == eCHAR_MYSTIC )
    {
        move_skill_class_code = eSKILL_MYSTIC_921;
    }

    BOOL is_active = FALSE;
    float teleport_distance;

    DWORD skill_id = GetSkillCode(move_skill_class_code, is_active, teleport_distance);

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {'\0',};

    if (skill_id != 0)
    {
        if (is_active == TRUE)
        {
            // 1. ��ų�Ÿ���ŭ �о �� �� �ִ��� üũ(�о �� �� �ִٸ� �ٸ� üũ�� �ʿ����)
            // 2. �о �� �� ���ٸ�, PTA_ONLY_JUMP, PTA_NO_WALK, PTA_EXTRA_TILE ���� Ÿ�������� �Ͽ� ��� Ž��
            
            WzVector source_position = g_pHero->GetPosition();
            WzVector destination_position = source_position + (teleport_direction * teleport_distance * 0.85f);
            static int spline_num = 10;
            destination_position = GetCanTeleportPosition(destination_position, spline_num);
            destination_position.z += 1.0f;
            //m_PickingPosition = destination_position;

            if ((VectorLength(&destination_position) == 0.0f) || 
                (abs(destination_position.z - source_position.z) > 10.0f))
            {
                // �ڷ���Ʈ�� �̵��� �� ���� �����Դϴ�.
                g_InterfaceManager.GetInterfaceString(eST_NOT_USE_TELEPORT_SKILL, szMessage, INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
                return;
            }

            //m_TargetAreaSkillID = (SLOTCODE)skill_id;
            m_bSpecialMove = TRUE;

            UseSpecialMoveSkill( skill_id, destination_position);
        }
        else
        {
            // �ڷ���Ʈ ��ų�� ����� �մϴ�.
            static DWORD ms_LastOutputTime = 0;
            DWORD CurrentTime = GetCurrentTime();

            // 2�� �̳����� ǥ������ �ʴ´�
            if( ms_LastOutputTime == 0 || ms_LastOutputTime + 2000 < CurrentTime )
            {
                int message_code = move_skill_class_code == eSKILL_TELEPORT ? eST_NOT_LEARN_TELEPORT : eST_NOT_LEARN_BLINK;
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, g_InterfaceManager.GetInterfaceString( message_code ) );

                ms_LastOutputTime = CurrentTime;
            }
        }
    }
    else
    {
        // �ڷ���Ʈ ��ų�� �����ϴ�.
        g_InterfaceManager.GetInterfaceString(eST_NOT_EXIST_TELEPORT_SKILL, szMessage, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
    }
}

//------------------------------------------------------------------------------
/**
    �̵� ������ ��ġ�� ��´�.
*/
WzVector HeroActionInput::GetCanTeleportPosition(WzVector vDest, int iSplineNum/* = 10*/)
{
    iSplineNum = max(iSplineNum, 5);
    iSplineNum = min(iSplineNum, 100);

    WzVector vSrc = g_pHero->GetPosition();

    WzVector vTemp;
    float fRate = 0.0f;
    float fRateModify = 1.0f / static_cast<float>(iSplineNum);
    int iTileFocussing = -1;
    float fDistance = 0.0f;

    //�ִ� 10ȸ������ ã�´�
    
    for (int i = 0; i < 10; ++i)
    {
        vTemp = vDest * (1.0f - fRate) + vSrc * (fRate);

        iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(vTemp, &fDistance, 10.0f, 10.0f);
        if(iTileFocussing >= 0)
        {
            vTemp.z -= fDistance;

            g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vSrc, -1);
            
            // ��ǥ��ġ���� �о����� ����
            WzVector wzThrustDirection = vDest - vSrc;
            ENUM_FINDPATH_RESULT eFindpathResult =
                g_pSunTerrain->PathExp_Thrust(g_pPathExplorer, 
                &wzThrustDirection, 
                PTA_ONLY_JUMP | PTA_NO_WALK);

            if (eFindpathResult == EFR_SUCCESS)
            {
                g_pSunTerrain->PathExp_Progress(g_pPathExplorer, 1000000);
                vTemp = g_pPathExplorer->GetPos();

                return vTemp;
            }

            // ���� ��ǥ��ġ���� ���Ž���Ͽ� üũ
            WORD wTileAttribute = g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iTileFocussing);
            if (wTileAttribute & (PTA_ONLY_JUMP | PTA_NO_WALK))
            {
                // ��ǥ��ġ�� �� �� ���� Ÿ���̸� �ȵ�
            }
            else
            {
                // �ڷ���Ʈ�� ��� Ž���� ��� PTA_EXTRA_TILE Ÿ�� �Ӽ��� �����Ѵ�.
                // ���� ��� ���� ��� Ÿ�ϼӼ��� �ο��Ͽ� ��/������ �ڷ���Ʈ�ϴ� ���� ����
                wTileAttribute = PTA_ONLY_JUMP | PTA_NO_WALK | PTA_EXTRA_TILE;
                ENUM_FINDPATH_RESULT eFindpathResult = 
                g_pSunTerrain->PathExp_FindPath(g_pPathExplorer, 
                    vTemp, 
                    iTileFocussing, 
                    wTileAttribute, 
                    MAX_NODE_FOR_PATHFIND);

                if (eFindpathResult == EFR_SUCCESS)
                {
                    return vTemp;
                }
            }

            // ������� ���� �ش���ġ�� �ڷ���Ʈ�� �� ����, ��ǥ��ġ Ž��
        }

        fRate += fRateModify;

        iTileFocussing = -1;
        fDistance = 0.0f;
    }

    // ��ǥ��ġ�� ã�� �� ������ �о������ ������.
    g_pSunTerrain->PathExp_Reset(g_pPathExplorer, &vSrc, -1);
    WzVector wzThrustDirection = vDest - vSrc;
    g_pSunTerrain->PathExp_Thrust(g_pPathExplorer,&wzThrustDirection, PTA_NO_WALK);
    g_pSunTerrain->PathExp_Progress(g_pPathExplorer, 1000000);
    vDest = g_pPathExplorer->GetPos();

    return vDest;
}


//------------------------------------------------------------------------------
/**
    ĳ���� ���� ó�� 
*/
BOOL HeroActionInput::ProcessKeyboardJump()
{
    if( CannotMoveState() == TRUE )
    {
        return FALSE;
    }

	if( g_pHero->IsGravityZoneState() || g_pHero->IsPolymorph() )
	{
		return FALSE;
	}

#ifdef _YJ_FISHING_SYSTEM_000933
    if( g_InterfaceManager.IsShowDialog(InterfaceManager::DIALOG_FISHING_ACTION) == TRUE )
    {
        //������ �����.
    }
    else
    {
        if ((g_Input.IsPressAnyExtKey() == FALSE) && 
            (g_Input.GetState(DIK_SPACE, KS_KEY) != FALSE))
        {
            set_jump_key_pressed(true);
        }
    }
#else //_YJ_FISHING_SYSTEM_000933
        if (is_spread_wing_key_pressed() == true)
        {
            if (ProcessSpreadWing() == TRUE)
            {
                return TRUE;
            }            
        }

        if ((g_pHero->is_spread_wings() == true) || 
            ((g_pHero->IsRiding() == TRUE) && 
            (g_pHero->rider() != NULL) && 
            (g_pHero->rider()->is_spread() == true)))
        {
            // ������ ��� �ְų�, ���� Ż���� Ÿ�� ���� ������ ������ �ȵ�
            return FALSE;
        }

        //////////////////////////////////////////////////////////////////////////

#endif //_YJ_FISHING_SYSTEM_000933
    

    //if (bJumpPressed) 
    if (is_jump_key_pressed() == true)
    {
        // ��� ����� �۾� ���.         
        InitTerm();
        
        CancelSkillQueue();

        SetFollowState( FALSE );
        SetAutoAttack(FALSE);

        //���� �߿��� ���� �ȵȴ�.
        //if (bJumpPressed&&(IsIdleState() || g_pHero->GetCurState() == STATE::MOVE  || g_pHero->GetCurState() == STATE::KEYBOARDMOVE))
        if (IsIdleState() || g_pHero->GetCurState() == STATE::MOVE  || g_pHero->GetCurState() == STATE::KEYBOARDMOVE)
        {
            if( m_KeyboardState.GetFowardPressed() )
            {
                g_pHero->m_JumpData.m_JumpFlag.AddForward();
            }
            else if( m_KeyboardState.GetBackwardPressed() )
            {
				g_pHero->m_JumpData.m_JumpFlag.AddBackward();
            }

            if( m_KeyboardState.GetRightStepPressed() )
            {
				g_pHero->m_JumpData.m_JumpFlag.AddRightStep();
            }
            else if( m_KeyboardState.GetLeftStepPressed() )
            {
				g_pHero->m_JumpData.m_JumpFlag.AddLeftStep();
            }

            if(g_pHero->m_JumpData.m_JumpFlag.GetMoveFlag() == 0 )
            {
                if(m_bAutoRun||g_pHero->IsMoving())
                {
					g_pHero->m_JumpData.m_JumpFlag.AddForward();
                }
            }
            //�ɱ� �߿� ���� ������ Ǭ��.
            if(g_pHero->GetCurState() == STATE::SIT)
            {
                MSG_CG_STATUS_CONDITION_CHANGE_SYN Packet;
                Packet.m_byCategory = CG_STATUS;
                Packet.m_byProtocol = CG_STATUS_CONDITION_CHANGE_SYN;      
                Packet.m_byConditionType = eCHAR_CONDITION_STANDUP;

                if( FALSE == g_pHero->SendPacket( &Packet, sizeof(MSG_CG_STATUS_CONDITION_CHANGE_SYN) ) )
                {
                    assert(!"���� ��Ŷ ���� ����");
                    return FALSE;
                }
            }

            g_pHero->SetNextState(STATE::KEYBOARDJUMP,g_CurTime);
            return TRUE;
        }      
    }

    return FALSE;
}

BOOL HeroActionInput::ProcessSpreadWing()
{
    if ((g_pHero->IsTransForm() == FALSE) && 
        (g_pHero->IsRiding() == TRUE) && 
        (g_pHero->rider() != NULL) && 
        (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_SSQ)
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        && (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_SPA)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        )
    {
        StateBase* rider_spread_state = g_pHero->GetStateInstance(STATE::RIDER_SPREAD);
        if ((g_pHero->rider()->is_spread() == true) && 
            (g_pHero->GetCurState() != STATE::RIDER_FURL) && 
            (g_pHero->GetNextState() != STATE::RIDER_FURL))
        {
            g_pHero->SetNextState(STATE::RIDER_FURL, g_CurTime);
        }
        else if ( g_pHero->rider() && 
			(g_pHero->rider()->is_spread() == false) &&
            (g_pHero->rider()->is_possible_fly() == true) && 
            (g_pHero->GetCurState() != STATE::RIDER_SPREAD) && 
            (g_pHero->GetNextState() != STATE::RIDER_SPREAD) && 
            (rider_spread_state != NULL) && 
            (rider_spread_state->CanTransite(g_pHero->GetCurStateInstance()) == true))
        {
            g_pHero->SetNextState(STATE::RIDER_SPREAD, g_CurTime);
            return TRUE;
        }
    }
    else if ((g_pHero->IsTransForm() == FALSE) && 
        (g_pHero->IsRiding() == FALSE) && 
        (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_SSQ)
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
        && (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_SPA)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
        )
    {
        StateBase* wing_summon_state = g_pHero->GetStateInstance(STATE::WINGS_SPREAD);

        if ((g_pHero->is_spread_wings() == true) && 
            (g_pHero->GetCurState() != STATE::WINGS_FURL) && 
            (g_pHero->GetNextState() != STATE::WINGS_FURL))
        {
            g_pHero->SetNextState(STATE::WINGS_FURL, g_CurTime);
        }
        else if ((g_pHero->is_wing_equip(NULL) == true) && 
            (g_pHero->is_spread_wings() == false) && 
            (g_pHero->GetCurState() != STATE::WINGS_SPREAD) && 
            (g_pHero->GetNextState() != STATE::WINGS_SPREAD) && 
            (wing_summon_state != NULL) && 
            (wing_summon_state->CanTransite(g_pHero->GetCurStateInstance()) == true))
        {
            if (g_pHero->GetCurState() == STATE::SIT) 
            {
                g_pHero->SendStandUp();
            }
            g_pHero->SetNextState(STATE::WINGS_SPREAD, g_CurTime);
            return TRUE;
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
    2006.1.15 Ư�� �̵� ���� ���� ����-���� 
*/
BOOL HeroActionInput::ProcessKeyboardMove()
{
    // �ƹ�Ÿ�� ������ ó������
    // ���� ���ʿ����� g_pHero�� �������������� üũ���ʿ����.
    if( g_pHero == NULL )
    {
        return FALSE;
    }

    if( BattleScene::IsWaitEnterSync() == TRUE )
    {
        //! Player_enter ������ ���� ��������(�ε�����)������ Ű���� ó�� ���� �ʴ´�.
        SetAutoRun(FALSE);
        SetFollowState( FALSE );

        return FALSE;
    }


    eCHAR_TYPE charType = g_pHero->GetClass();

    if( ProcessKeyboardJump() )
    {
        return TRUE;
    }


    g_pHero->ResetMoveFlag();

    if( CannotMoveState() == TRUE )
    {
        SetAutoRun(FALSE);
        SetFollowState( FALSE );

        return FALSE;
    }


    if( charType == eCHAR_VALKYRIE )
    {
        return ProcessValkyrieKeyboardMove();
    }
    else if( charType == eCHAR_ELEMENTALIST || charType == eCHAR_MYSTIC )
    {
        return ProcessElementalistKeyboardMove();
    }
    else
    {
        return ProcessDefaultKeyboardMove();
    }

    return FALSE;

}


//------------------------------------------------------------------------------
/**
*/
VOID HeroActionInput::InitTerm()
{
    ZeroMemory(&m_QueueAction,sizeof(m_QueueAction));   
    m_QueueAction.ActionID = ACTION_IDLE;
    if( IsShootingMode() == FALSE )
    {
        SetAutoAttack(FALSE);  
    }

    m_bKeyboardQueueAttack = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void HeroActionInput::PickingForShowMonsterName()
{
    if( GAMEOPTION->GetDisplayMonterInfo() == FALSE )
        return;

    if (VectorLength2(&g_wrMouseRay.m_wvDirection) <= 0.04f)
        return;

    Character* pPickedObj = GetPickedCharacter();

    g_pCurrentPickedObject = pPickedObj;

    if( pPickedObj && ( pPickedObj->GetObjectKey() != g_pHero->GetObjectKey() ) )
    {
        Object * pTargetObject = g_ObjectManager.GetObject( pPickedObj->GetObjectKey() );
        Object * pCurrentTargetObj = g_ObjectManager.GetObject( GetCurrentTarget() );

        if( pTargetObject )
        {
            if( pTargetObject->IsKindOfObject( MONSTER_OBJECT ) )
            {
                Monster * pMonster = static_cast<Monster *>(pTargetObject);

                if( pCurrentTargetObj == pTargetObject )
                {
                    pMonster->SetShowName( FALSE );
                    return;
                }

                pMonster->SetShowName( TRUE );
            }
        }
    }
    else
    {
        g_pCurrentPickedObject = NULL;
    }
}


//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::ProcessValkyrieMouse()
{
    if (!g_pHero)
    {
        return FALSE;
    }

    if( g_Input.GetState( MOUSE_LBUTTON, IKS_MOUSE_DOWN ))
    {
        GetCursorPos( &m_MousePoint );
    }
    if( g_Input.GetState( MOUSE_RBUTTON, IKS_MOUSE_DOWN ))
    {
        GetCursorPos( &m_MousePoint );
    }

    static   BOOL  bIsLButtonDown = FALSE;
    static   BOOL  bIsRButtonDown = FALSE;

#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
#else
    static   BOOL  bIsRotateL = FALSE;
#endif //#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
    static   BOOL  bIsRotateR = FALSE;

    // ���ΰ� ȸ�� ���� �ʱ�ȭ
    g_pHero->SetHeroTurnState( g_Camera.GetCameraTurnState() );

    if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_DOWN ))
    {
        bIsRotateL = FALSE;
        bIsLButtonDown = TRUE;
        GetCursorPos( &m_MousePoint );
    }
    else if ( bIsRotateL == FALSE && 
              g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_KEY) && 
              g_Camera.IsRotate() == TRUE )
    {
        // ó�� ȸ�� ����
        bIsRotateL = TRUE;
        if( m_bAutoAttack == FALSE && g_pHero->IsDead() == FALSE )
        {
            if( g_pHero->GetCurState() == STATE::IDLE || g_pHero->GetCurState() == STATE::KEYBOARDMOVE )
            {
                // ���ΰ� ȸ�� ���� ����
                g_pHero->SetHeroTurnState( g_Camera.GetCameraTurnState() );

                g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), FALSE );
            }
        }

        // ó�� Ŭ���� ���� ���콺 ��ǥ ����
        if( m_bIsMousePointHide == FALSE )
        {
            // ���콺 ȸ���� ���� ���콺 �����͸� �����.
            m_bIsMousePointHide = TRUE;
        }
    }
    else if (bIsRotateL == TRUE && 
             g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_KEY) && 
             g_Camera.IsRotate() == TRUE )
    {
        // ��� ȸ�� ��
        if( m_bAutoAttack == FALSE && g_pHero->IsDead() == FALSE )
        {
            if( g_pHero->GetCurState() == STATE::IDLE || g_pHero->GetCurState() == STATE::KEYBOARDMOVE )
            {
                // ���ΰ� ȸ�� ���� ����
                g_pHero->SetHeroTurnState( g_Camera.GetCameraTurnState() );

                g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), FALSE );
            }
        }
    }


    if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_DOWN ))
    {
        bIsRotateR = FALSE;
        bIsRButtonDown = TRUE;
        GetCursorPos( &m_MousePoint );
    }
    else if ( bIsRotateR == FALSE && g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_KEY) && g_Camera.IsRotate() == TRUE )
    {
        // ó�� ȸ�� ����
        bIsRotateR = TRUE;

        // ó�� Ŭ���� ���� ���콺 ��ǥ ����
        if( m_bIsMousePointHide == FALSE )
        {
            // ���콺 ȸ���� ���� ���콺 �����͸� �����.
            m_bIsMousePointHide = TRUE;
        }
    }
    else if (bIsRotateR == TRUE && 
             g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_KEY) && 
             g_Camera.IsRotate() == TRUE )
    {
        // ��� ȸ����
    }


    // ���콺 ������ ǥ��
    if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_UP ))
    {
        if( bIsLButtonDown == TRUE || m_bIsMousePointHide )
        {
            m_bIsMousePointHide = FALSE;
        }
    }
    else if( g_Input.GetState( MOUSE_LBUTTON, IKS_MOUSE_UP ))
    {
        if( bIsLButtonDown == TRUE || m_bIsMousePointHide )
        {
            m_bIsMousePointHide = FALSE;
        }
    }

    if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_UP ))
    {
        if( bIsRButtonDown == TRUE || m_bIsMousePointHide )
        {
            m_bIsMousePointHide = FALSE;
            SetCursorPos( m_MousePoint.x, m_MousePoint.y );
            MouseCursor::Instance()->ShowCursor( TRUE );
        }
    }
    else if( g_Input.GetState( MOUSE_RBUTTON, IKS_MOUSE_UP ))
    {
        if( bIsRButtonDown == TRUE || m_bIsMousePointHide )
        {
            m_bIsMousePointHide = FALSE;
        }
    }



    // �� ��ư ���ÿ� �����°� ����.
    if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_UP ))
    {
        if( bIsLButtonDown == TRUE )
        {
            bIsLButtonDown = FALSE;

            if( bIsRotateL == FALSE )
            {
                // ���콺 Ŭ��
                WzVector ClickedPosition;
                SetAutoRun(FALSE);
                SetFollowState( FALSE );
                SetAutoAttack(FALSE);

                InitTerm();

                if (!g_pHero->CanValkyrieAction())
                {
                    return FALSE;
                }

                Character* pPickedObj = GetPickedCharacter();
                if(pPickedObj)
                {
                    if (OnValkyrieLClickedCharacter(pPickedObj))
                    {
                        g_InterfaceManager.SetMovePointer( FALSE, NULL );
                        return TRUE;
                    }
                }

                g_pHero->LockOn(0);

                Item * pPickItem = GetPickedItem();
                if (pPickItem)
                {
                    // �������� ���� ���ڸ� Ŭ�� �����ϴ�~
                    if (!pPickItem->IsOpened() && MouseHandler::Instance()->IsExistItemAtHand() == FALSE )
                    {
                        if (OnLClickedItem(pPickItem))
                        {
                            return TRUE;
                        }
                    }
                }


                if (GameFunc::GetClickedPosition(&ClickedPosition))
                {
                    if (OnLClickedPosition(&ClickedPosition))
                    {
#ifdef PERF_AUTO_MOVE_TEST
                        if(m_bAutoMove)
                        {
                            m_vDestPos=g_pHero->GetVisiblePos();
                            m_vSrcPos=ClickedPosition;       
                        }
#endif 

                        return TRUE;
                    }

                }
                else
                {
                    return TRUE;
                }


            }
            else
            {
                bIsRotateL = FALSE;
            }
        }  // if( bIsLButtonDown == TRUE )
    }

    if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_UP ))
    {
        if( bIsRotateR == FALSE )
        {
            Character* pPickedObj = GetPickedCharacter();
            if(pPickedObj && (pPickedObj->GetObjectKey() != g_pHero->GetObjectKey()) )
            {
                SetCurrentTarget( pPickedObj->GetObjectKey() );
                if (g_pHero->GetCurState() == STATE::IDLE)
                {
                    g_pHero->SetDirection(pPickedObj->GetPosition());
                }
            }
            else
            {
                SetAutoAttack(FALSE);

                SetCurrentTarget( 0 );

                if( g_pHero->GetCurState() != STATE::MOVE && g_pHero->GetCurState() != STATE::KEYBOARDMOVE && g_pHero->GetCurState() != STATE::SPECIALMOVE&& g_pHero->GetCurState() != STATE::KEYBOARDJUMP)
                {
                    PLAYER_ACTION action;
                    action.ActionID = ACTION_IDLE;
                    m_QueueAction = action;
                    g_pHero->SetNextState(STATE::IDLE,g_CurTime);
                }
            }

            // Ÿ�� ���̾�α� ����
            ProcessTargeting();
        }
        else
        {
            bIsRotateR = FALSE;
        }
    }

    return FALSE;  
}

BOOL HeroActionInput::ProcessMouse()
{
    // �ƹ�Ÿ�� ������ ó������
    // ���� ���ʿ����� g_pHero�� �������������� üũ���ʿ����.
    if (!g_pHero)
    {
        return FALSE;
    }
    
    if(GetCurrentTarget())
    {
        Object *pObj=g_ObjectManager.GetObject(GetCurrentTarget());

        if(pObj)
        {
            if(pObj->IsKindOfObject(CHARACTER_OBJECT))
            {
                Character *pChr=(Character *)pObj;

                if(pChr->IsObserverMode())
                    this->SetCurrentTarget(0);
            }
        }
    }

    m_bPicking = FALSE;

    if( g_Input.GetState( MOUSE_LBUTTON, IKS_MOUSE_DOWN )||g_Input.GetState( MOUSE_RBUTTON, IKS_MOUSE_DOWN ))
    {
        GetCursorPos( &m_MousePoint );
    }

    static   BOOL  bIsLButtonDown = FALSE;
    static   BOOL  bIsRButtonDown = FALSE;

#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
#else
    //static   BOOL  bIsRotateL = FALSE;
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
    static   BOOL  bIsRotateR = FALSE;

    // ���ΰ� ȸ�� ���� �ʱ�ȭ
    g_pHero->SetHeroTurnState( g_Camera.GetCameraTurnState() );

    if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_DOWN ))
    {
        bIsRotateL = FALSE;
        bIsLButtonDown = TRUE;
        GetCursorPos( &m_MousePoint );
    }
    else if ( bIsRotateL == FALSE && 
              g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_KEY) && 
              g_Camera.IsRotate() == TRUE )
    {
        // ó�� ȸ�� ����
        bIsRotateL = TRUE;
        if( m_bAutoAttack == FALSE && 
            IsShootingMode() == FALSE && 
            g_pHero->IsDead() == FALSE )
        {

            if( IsIdleState() || g_pHero->GetCurState() == STATE::KEYBOARDMOVE || g_pHero->GetCurState() == STATE::MOVE)
            {
                if(IsIdleState() && g_pHero->IsRiding())
                {   
                    if(g_pHero->GetCurState()==STATE::RIDER_TURN) 
                    {
                        g_pHero->SetNextState(STATE::IDLE,g_CurTime);
                    }
                }
                else
                {
                    // ���ΰ� ȸ�� ���� ����
                    g_pHero->SetHeroTurnState( g_Camera.GetCameraTurnState() );
                    g_pHero->StopMove();
                    g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), TRUE );
                    if (g_pHero->GetCurState() == STATE::MOVE)//�̵��߿��� ��ũ ���߱�
                    {
                        g_pHero->SendStopMovePacket();
                    }
                }
            }
            else if( g_pHero->GetCurState() == STATE::KEYBOARDJUMP)
            {
                g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), TRUE );
            }
        }

        // ó�� Ŭ���� ���� ���콺 ��ǥ ����
        if( m_bIsMousePointHide == FALSE )
        {
            MouseCursor::Instance()->ShowCursor( FALSE );
            // ���콺 ȸ���� ���� ���콺 �����͸� �����.
            m_bIsMousePointHide = TRUE;
        }
    }
    else if ( bIsRotateL == TRUE && 
            g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_KEY) && 
            g_Camera.IsRotate() == TRUE)
    {
        // ��� ȸ�� ��
        if( m_bAutoAttack == FALSE && IsShootingMode() == FALSE && g_pHero->IsDead() == FALSE )
        {
            if( (IsIdleState() && !g_pHero->IsRiding()) || g_pHero->GetCurState() == STATE::KEYBOARDMOVE)
            {
                // ���ΰ� ȸ�� ���� ����
                g_pHero->SetHeroTurnState( g_Camera.GetCameraTurnState() );
 
                g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom());
                
            }
            else if( g_pHero->GetCurState() == STATE::KEYBOARDJUMP)
            {
                g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), TRUE );
            }
        } 
    }
    else
    {
        if(g_InterfaceManager.IsLockDlg())
        {
            //����ó��:���ɷ����� �޽����������Ѵ� ���콺�� �Ⱥ��ϼ����ֱ⿡ Ǯ���ش�
            if(!MouseCursor::Instance()->IsShowCursor())
            {
                MouseCursor::Instance()->ShowCursor(TRUE);
                m_bIsMousePointHide = TRUE;
            }
        }
    }

    if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_DOWN ))
    {
        bIsRotateR = FALSE;
        bIsRButtonDown = TRUE;
        GetCursorPos( &m_MousePoint );
    }
    else if (bIsRotateR == FALSE && g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_KEY) && g_Camera.IsRotate() == TRUE )
    {
        // ó�� ȸ�� ����
        bIsRotateR = TRUE;

        // ó�� Ŭ���� ���� ���콺 ��ǥ ����
        if( m_bIsMousePointHide == FALSE )
        {
            MouseCursor::Instance()->ShowCursor( FALSE );
            // ���콺 ȸ���� ���� ���콺 �����͸� �����.
            m_bIsMousePointHide = TRUE;
        }
    }
    else if (bIsRotateR == TRUE && g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_KEY) && g_Camera.IsRotate() == TRUE )
    {
        // ��� ȸ�� ��
    }

    if (GlobalFunc::BeforeLoadingInputLock())
    {
        return TRUE;
    }


    // ���콺 ������ ǥ��
    if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_UP ))
    {
        if( bIsLButtonDown == TRUE || m_bIsMousePointHide )
        {
            m_bIsMousePointHide = FALSE;
            SetCursorPos( m_MousePoint.x, m_MousePoint.y );
            MouseCursor::Instance()->ShowCursor( TRUE );
        }
    }
    else if( g_Input.GetState( MOUSE_LBUTTON, IKS_MOUSE_UP ))
    {
        if( bIsLButtonDown == TRUE || m_bIsMousePointHide )
        {
            m_bIsMousePointHide = FALSE;
            SetCursorPos( m_MousePoint.x, m_MousePoint.y );
            MouseCursor::Instance()->ShowCursor( TRUE );
        }
    }

    if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_UP ))
    {
        if( bIsRButtonDown == TRUE || m_bIsMousePointHide )
        {
            m_bIsMousePointHide = FALSE;
            SetCursorPos( m_MousePoint.x, m_MousePoint.y );
            MouseCursor::Instance()->ShowCursor( TRUE );
        }
    }
    else if( g_Input.GetState( MOUSE_RBUTTON, IKS_MOUSE_UP ))
    {
        if( bIsRButtonDown == TRUE || m_bIsMousePointHide )
        {
            m_bIsMousePointHide = FALSE;
            SetCursorPos( m_MousePoint.x, m_MousePoint.y );
            MouseCursor::Instance()->ShowCursor( TRUE );
        }
    }

    if( g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_UP ))
    {
        if( bIsRButtonDown )
        {
            bIsRButtonDown = FALSE;


            if( bIsRotateR == FALSE )
            {
                Character* pPickedObj = GetPickedCharacter();

                if(pPickedObj && (pPickedObj->GetObjectKey() != g_pHero->GetObjectKey()) )
                {
                    SetCurrentTarget( pPickedObj->GetObjectKey() );
                    if( g_pHero->GetCurState() == STATE::IDLE )
                    {
                        g_pHero->SetDirection(pPickedObj->GetPosition());
                    }

                    RELATIONKIND::TYPE RelationType = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pPickedObj, lsForceAttack() );
                    if( RelationType == RELATIONKIND::Assistant)
                    {
                        uiHelpMan* helpMan =
                                    static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

                        if (helpMan)
                        {
                            helpMan->CheckHelpMessage(eHK_CLICK_NPC);
                        }
                        uiGuideSystemMan* guide_manager =
                            static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

                        if (guide_manager)
                        {
                            NPC* npc = static_cast<NPC*>(pPickedObj);
                            DWORD monster_code = npc->GetMonsterInfo()->m_MonsterCode;
                            guide_manager->CheckHelpMessage(TotalGuideNotice::kNpcClick, monster_code);
                        }
                    }
                }
                else
                {
                    SetAutoAttack(FALSE);
                    SetCurrentTarget( 0 );
                    if( g_pHero->GetCurState() != STATE::MOVE && 
                        g_pHero->GetCurState() != STATE::KEYBOARDMOVE &&
                        g_pHero->GetCurState() != STATE::SPECIALMOVE &&
                        g_pHero->GetCurState() != STATE::SIT &&
                        g_pHero->GetCurState() != STATE::KEYBOARDJUMP &&
                        g_pHero->GetCurState() != STATE::EMOTICON
                        && g_pHero->GetCurState() != STATE::COLLECT 
                        && g_pHero->GetCurState() != STATE::SKILL 
#ifdef _YJ_FISHING_SYSTEM_000933
                        && g_pHero->GetCurState() != STATE::FISHING_THROW 
                        && g_pHero->GetCurState() != STATE::FISHING_STAND 
                        && g_pHero->GetCurState() != STATE::FISHING_BITE 
                        && g_pHero->GetCurState() != STATE::FISHING_SUCCESS 
                        && g_pHero->GetCurState() != STATE::FISHING_FAILED
                        
#endif //_YJ_FISHING_SYSTEM_000933

                        && (g_pHero->GetCurState() != STATE::SUMMON_RIDER)
                        ) 
                    {
                        PLAYER_ACTION action;
                        action.ActionID = ACTION_IDLE;
                        m_QueueAction = action;
                        g_pHero->SetNextState(STATE::IDLE,g_CurTime);
                    }
                }
                Item * pPickItem = GetPickedItem();
                if (pPickItem)
                {
                    // �������� ���� ���ڸ� Ŭ�� �����ϴ�~
                    if (!pPickItem->IsOpened() && MouseHandler::Instance()->IsExistItemAtHand() == FALSE )
                    {
                        if (OnLClickedItem(pPickItem))
                        {
                            return TRUE;
                        }
                    }
                }

                // Ÿ�� ���̾�α� ����
                ProcessTargeting();

#ifdef MAP_EDIT_MODE
                MapObject *pMapObject = GetPickedMapObject();

                if (pMapObject)
                {

                    if (OnRClickedMapObject(pMapObject))
                    {
                        return TRUE;
                    }
                    else if(OnRClickedTile())
                    {
                        return TRUE;
                    }
                    
                }
                else if(OnRClickedTile())
                {
                    return TRUE;
                }
#endif
            }
            else
            {
                bIsRotateR = FALSE;
            }
        }
    }

    // ������ �����°͸� �ٸ��� ó����.
    if (MouseHandler::Instance()->IsExistItemAtHand() && 
        ((!g_Input.GetMouseChange() && g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_UP ) ) ||
         (g_Input.GetMouseChange() && g_Input.GetState( MOUSE_RBUTTON, KS_MOUSE_UP )) )
        ) 
    {
        WzVector ClickedPosition;
        if( GameFunc::GetClickedPosition(&ClickedPosition) == FALSE )
            return TRUE;

        if (OnLClickedPositionWithItem(&ClickedPosition))
        {
            return TRUE;
        }              
    }
    else
    // �� ��ư ���ÿ� �����°� ����. ���� ��ư -> ����
    if( g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_UP ))
    {
        if( bIsLButtonDown == TRUE )
        {
            bIsLButtonDown = FALSE;

            if( bIsRotateL == FALSE )
            {              
                // ���콺 Ŭ��
                SetAutoAttack(FALSE);
                SetAutoRun(FALSE);
                SetFollowState( FALSE );

                Character* pPickedObj = GetPickedCharacter();

                // Ű���� �̵��ÿ� ���� ����
                if( m_bIsHeroKeyboardMove )
                {
                    if( pPickedObj )
                    {
                        SetCurrentTarget( pPickedObj->GetObjectKey() );
                    }

                    return FALSE;
                }

                InitTerm();//m_QueueAction�� idle ���·� ���� 

                // �������� ���°� �ƴѰ�쿡��
                if( m_ShowTargetAreaPointer == FALSE )
                {
                    if(pPickedObj)
                    {
                        RELATIONKIND::TYPE RelationType = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pPickedObj, lsForceAttack() );
                        if( OnLClickedCharacter( pPickedObj, RelationType, false ) )
                        {
                            g_InterfaceManager.SetMovePointer( FALSE, NULL );
                            return TRUE;
                        }
                    }

                    // �̰ſ� ���ؼ� �� �����غ���. 
                    Item * pPickItem = GetPickedItem();
                    if (pPickItem)
                    {
                        // �������� ���� ���ڸ� Ŭ�� �����ϴ�~
                        if (!pPickItem->IsOpened() && MouseHandler::Instance()->IsExistItemAtHand() == FALSE )
                        {
                            if (OnLClickedItem(pPickItem))
                            {
                                return TRUE;
                            }
                        }
                    }

                    MapObject *pMapObject = GetPickedMapObject();

                    if (pMapObject && pMapObject->GetShow())
                    {
                        if(STATE::COLLECT != g_pHero->GetCurState())
                        {
                            if(TRUE == OnLClickedMapObject(pMapObject)) //UP
                            {
                                return TRUE;
                            }
                        }
                    }
                }

                WzVector ClickedPosition;
                if( GameFunc::GetClickedPosition(&ClickedPosition) == FALSE )
                    return TRUE;

                if (OnLClickedPosition(&ClickedPosition))
                {
#ifdef PERF_AUTO_MOVE_TEST
                    if(m_bAutoMove)
                    {
                        m_vSrcPos=g_pHero->GetVisiblePos();
                        m_vDestPos=ClickedPosition;      
                    }
#endif 
                    return TRUE;
                }
            }
            else
            {
                // ī�޶� ȸ��
                bIsRotateL = FALSE;
            }

        }  // if( bIsLButtonDown == TRUE )
#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
        else if ((bIsLButtonDown == FALSE) &&
            (bIsRotateL == TRUE))
        {
            // Ŭ�� �ٿ��� �����µ� ȸ���� ���ִ� ���?
            {
                // [SUNJP-1711] 2016.1.22 / naklu / �̵� Ű ������ ���������� ������� �ʴ� ���� ����
                // ui ��ġ�̵� �巡�׷� ȭ�� ������ ���콺�� ���� ��� bIsRotateL �� �����µ�
                // �ٽ� ȭ�� ���η� ���콺 �̵��ؼ� Ŭ�� ���� ���� ���, bIsRotateL �� �Ȳ����� ���콺 ���� ���� ����
                bIsRotateL = FALSE;
            }
        }
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
    }
    else if(g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_DOWN ))
    {
        if(STATE::COLLECT != g_pHero->GetCurState())
        {
            MapObject *pMapObject = GetPickedMapObject();
            if(pMapObject
               && (pMapObject->IsDominationObject() == FALSE))
            {
                if(TRUE == OnLClickedMapObject(pMapObject, false))
                {
                    g_pHero->SetDirection( pMapObject->GetVisiblePos() );
                    return TRUE;
                }
            }
        }
    }
#ifdef _NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
    else if( g_Input.GetState( MOUSE_LBUTTON, IKS_MOUSE_UP ))
    {
        // ���콺�� ui������ �� �� ȭ����� ���°� ��Ǯ���� ���� ����
        bIsRotateL = FALSE;
        bIsLButtonDown = FALSE;
    }
#endif //_NA_000000_20150609_CHARACTER_ROTATE_RUNNING_BUG_FIX
    return FALSE;

}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::ProcessMouseWhenKeyboardMove()
{
    // Ŭ���ϸ� ĳ���͸� ��󺻴�.
    if(g_Input.GetState( MOUSE_LBUTTON, KS_MOUSE_DOWN))
    {
        Character* pPickedObj = GetPickedCharacter();
        if(pPickedObj && (pPickedObj->GetObjectKey() != g_pHero->GetObjectKey()) )
        {
            SetCurrentTarget(pPickedObj->GetObjectKey());
            return TRUE;
        }
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
    ���콺 ��ư�� ���� ĳ���͸� ��ŷ ������ 
*/
BOOL HeroActionInput::OnLClickedCharacter(Character* pTarget, RELATIONKIND::TYPE relationKind, bool move_nearest)
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if(m_iMoveDelay>0)
    {
        return FALSE;
    }

    if(!pTarget)
        return FALSE;

    SetCurrentTarget(pTarget->GetObjectKey());

    if(!g_pHero)
        return FALSE;

    float fDistance;
    PLAYER_ACTION action;
    WzVector vPlayerPos = g_pHero->GetPosition();
    WzVector vOffset;
    WzVector vTargetPos;

    ZeroMemory(&action,sizeof(action));
    action.ActionID = ACTION_IDLE;//�׼� ���� �ʱ�ȭ   

#ifdef _NA_007113_SHADOW_PATCH_UNTARGET_MONSTER_DIE

    // �ҿ� ��Ʈ���� �������쿡�� ��ü�� Ÿ�����ϵ��� �Ѵ�
    if (relationKind == RELATIONKIND::Enemy )
    {
        if( pTarget->GetCurState() == STATE::DEATH )
        {
            if( xGetSkillData()->GetSkillDataBySkillClassCode( eSKILL_SOUL_CONTROL ) != NULL )
            {
                SetCurrentTarget( pTarget->GetObjectKey() );
                return TRUE;
            }
        }
    }
#endif //_NA_007113_SHADOW_PATCH_UNTARGET_MONSTER_DIE

    
    // �ؾ��� �ൿ�� �����Ѵ�!
    if (relationKind == RELATIONKIND::Enemy)
    {
        if (!pTarget->CanBeAttackTarget())//Ÿ���� �ɼ� �ִ� �����ΰ� 
        {
            return FALSE;
        }

        if (CannotAttack(pTarget))
        {
            return FALSE;
        }

        if( false == CanAttackInDominationWar() )
            return FALSE;

        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_ATTACK;                //����            
        action.ATTACK.dwTargetID = pTarget->GetObjectKey();//���  
        action.ATTACK.bForceAttack =lsForceAttack();

        // �ڵ� ���� ��
        
        SetAutoAttack(TRUE);
        SetCurrentTarget( pTarget->GetObjectKey() );


        vTargetPos = pTarget->GetPosition();
    }
    else if (relationKind == RELATIONKIND::Assistant)
    {  
        if (g_pHero)
        {
            if(!ChoaCanUse())
            {
                // ī�� ���¿����� �̿��� �������� �ʽ��ϴ�. 5375
                g_InterfaceManager.GetInterfaceString(eST_CHAO_IS_INAVALIBLE_STATUS , szMessage , INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }

    
            if ( g_pHero->GetBeHaveState() == PLAYER_BEHAVE_IDLE_STATE )
            {
                if ((pTarget->IsKindOfObject(NPC_OBJECT) == TRUE) && 
                    (pTarget->IsKindOfObject(MONSTER_OBJECT) == FALSE))
                {
                    NPC* pNPC = static_cast<NPC*>(pTarget);
                    if ((pNPC != NULL) && 
                        (pNPC->GetExtraInfo()->m_eNPCTYPE == eNPC_TYPE_PROTECTS))
                    {
                        // �ش� NPCŸ���� �÷��̾��� Ŭ���� �������� �ʰ�, ������ŷ�� ���� �ʴ´�.
                        return TRUE;
                    }
                }
                
                vTargetPos = pTarget->GetPosition();
                ZeroMemory(&action,sizeof(action));
                action.ActionID = ACTION_NPCMEET;                      
                action.NPCMEET.dwNPCKey = pTarget->GetObjectKey();   
                // �ڵ� ���� ����
                SetAutoAttack(FALSE);
            }

            uiHelpMan* helpMan =
                static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

            if (helpMan)
            {
                helpMan->CheckHelpMessage(eHK_CLICK_NPC);
            }
            uiGuideSystemMan* guide_manager =
                static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

            if (guide_manager)
            {
                NPC* npc = static_cast<NPC*>(pTarget);
                DWORD monster_code = npc->GetMonsterInfo()->m_MonsterCode;
                guide_manager->CheckHelpMessage(TotalGuideNotice::kNpcClick, monster_code);
            }
        }
    }
    else if (relationKind == RELATIONKIND::Vendor)
    {
        if(g_pHero)
        {
            if(!ChoaCanUse())
            {
                // ī�� ���¿����� �̿��� �������� �ʽ��ϴ�. 5375
                g_InterfaceManager.GetInterfaceString(eST_CHAO_IS_INAVALIBLE_STATUS , szMessage , INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }

            vTargetPos = pTarget->GetPosition();
            ZeroMemory(&action,sizeof(action));
            action.ActionID = ACTION_VENDORMEET;                      
            action.VENDORMEET.dwTargetID = pTarget->GetObjectKey();        
            // �ڵ� ���� ����
            SetAutoAttack(FALSE);

            if( pTarget->IsKindOfObject(PLAYER_OBJECT))
            {
                Player * pSaler = (Player * ) pTarget;
                if (pSaler)
                {
                    switch (pSaler->GetBeHaveState())
                    {
                    case PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE:
                        {
                            uiVendorMan * puiVendorMan = GET_DIALOG_MANAGER(uiVendorMan, UIMAN_VENDOR);

                            if (puiVendorMan)
                            {
                                puiVendorMan->SetPendingSaler( pTarget->GetObjectKey() );
                            }
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }
    else if (relationKind == RELATIONKIND::Friend)
    {
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		if (pTarget->IsKindOfObject(STATUE_PLAYER_OBJECT))
		{
			if (g_pHero)
			{
				if ( g_pHero->GetBeHaveState() == PLAYER_BEHAVE_IDLE_STATE )
				{	
					StatuePlayer* bronze_player = static_cast<StatuePlayer*>(pTarget);

					vTargetPos = pTarget->GetPosition();
					ZeroMemory(&action,sizeof(action));
					action.ActionID = ACTION_STATUE_PLAYER_MEET;
					action.STATUEMEET.dwStatuePlayerKey = pTarget->GetObjectKey();
					// �ڵ� ���� ����
					SetAutoAttack(FALSE);
				}
			}
		}
		else
		{
			vTargetPos = pTarget->GetPosition();
			ZeroMemory(&action,sizeof(action));
			action.ActionID = ACTION_PLAYERMEET;
			action.PLAYERMEET.dwTargetID = pTarget->GetObjectKey();
			// �ڵ� ���� ����
			SetAutoAttack(FALSE);
		}
#else
        vTargetPos = pTarget->GetPosition();
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_PLAYERMEET;                      
        action.PLAYERMEET.dwTargetID = pTarget->GetObjectKey();
        // �ڵ� ���� ����
        SetAutoAttack(FALSE);
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
    }


    // �ؾ����ൿ�� ������ ����ģ��.
    if (action.ActionID == ACTION_IDLE)
    {
        return FALSE;
    }

    vOffset = vTargetPos - vPlayerPos;
    fDistance = VectorLength( &vOffset );

    m_QueueAction = action;
    

	// ������ ��� BodyRadius �� ���ش�
	if( action.ActionID == ACTION_ATTACK )
	{
		fDistance -= pTarget->GetBodyRadius();
	}

    // �Ÿ��� �ʹ��ָ� �̵�~
    if( fDistance > GetActionLength(m_QueueAction) )
    {

        g_pHero->SetStartMoveTime( g_CurTime );
        if( MoveToAction(vTargetPos, move_nearest) == FALSE )
        {
            // Ÿ���� NPC�ϰ��� ���� �ٽ� �ѹ� �� ���
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
			if (relationKind == RELATIONKIND::Assistant || pTarget->GetObjectType() == STATUE_PLAYER_OBJECT )
#else
            if (relationKind == RELATIONKIND::Assistant)
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
            {
                // NPC�� �� ������ ���� �� �ִ� �ִ� �Ÿ����� �ణ �� �� ��ġ�� �Ѱ��༭ �ٽ� �ѹ� �� ���
                WzVector vDir = g_pHero->GetPosition() - vTargetPos;
                VectorNormalize( &vDir, &vDir );

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
				WzVector TargetPos;
				if ( pTarget->GetObjectType() == STATUE_PLAYER_OBJECT )
				{
					const float legnth_statue = 18.0f;
					TargetPos = vTargetPos + ( vDir * (legnth_statue * DISTANCE_NPC_MEET_EXT) );
				}
				else
				{
					// NPC�� ���� �� �ִ� �Ÿ�( DISTANCE_NPC_MEET - 6���� )�� 70%
					// �� ���� NPC�� ���� �� �ִ� �Ÿ����� 30% �Ÿ���ŭ �� �ٰ����ٴ� �ǹ��̴�.
					TargetPos = vTargetPos + ( vDir * (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET_EXT) );
				}
#else
                WzVector TargetPos = vTargetPos + ( vDir * (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET_EXT) );
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
                CPathExplorer *pPE = NULL;
                pPE = ( CPathExplorer* )Func_ContactTerrain( CT_NEW_PATH_EXPLORER, 0 );

                if( pPE )
                {
                    g_pSunTerrain->PathExp_Reset( pPE, &TargetPos, -1);

                    if( MoveToAction(TargetPos, move_nearest ) == FALSE )
                    {
                        if(pPE)
                        {
                            // ��ã�� ��� �ı�
                            Func_ContactTerrain( CT_DELETE_PATH_EXPLORER, pPE );

                            pPE = NULL;
                        }
                        return TRUE;
                    }

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
					if ( pTarget->GetObjectType() == STATUE_PLAYER_OBJECT )
					{
						m_QueueAction.ActionID = ACTION_STATUE_PLAYER_MEET;
					}
					else
					{
						m_QueueAction.ActionID = ACTION_NPCMEET;
					}
#else
                    m_QueueAction.ActionID = ACTION_NPCMEET;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

                    if(pPE)
                    {
                        // ��ã�� ��� �ı�
                        Func_ContactTerrain( CT_DELETE_PATH_EXPLORER, pPE );

                        pPE = NULL;
                    }
                }
            }
        }
        return TRUE;       
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/**
    ��� ������Ʈ�� �̵��� �� �׼��� ���Ѵ�.
*/
BOOL HeroActionInput::MoveToMapObjectWithAction(MapObject* pObj, bool bKeyUp )
{
    if( pObj == NULL || g_pHero == NULL )
        return FALSE;

#ifdef _NA_000000_20160308_ATTACK_RATE_RISE_BUG
	if( IsIdleState() == FALSE && g_pHero->GetCurState() != STATE::MOVE )
	{
		return FALSE;
	}
#endif // _NA_000000_20160308_ATTACK_RATE_RISE_BUG

    // 1. �׼� ����

    PLAYER_ACTION action;
    ZeroMemory(&action,sizeof(action));

    if(pObj->IsTriggerObj())
    {
        action.ActionID = ACTION_USE_OBJECT;
        action.USEOBJECT.dwTargetID = pObj->GetObjectKey();
    }
    else if(pObj->IsWayPointObj())
    {
        action.ActionID = ACTION_WAYPOINT_MEET;
        action.WAYPOINTMEET.dwTargetID = pObj->GetObjectKey();

        g_SoundEffect.PlayUI( UI_SOUND_DEFAULT_CLICK );
    }
    else if (pObj->IsAirShipObject())
    {
        action.ActionID = ACTION_AIRSHIP_MEET;
        action.AIRSHIP.target_object = pObj->GetObjectKey();

        g_SoundEffect.PlayUI(UI_SOUND_DEFAULT_CLICK);
    }
    else if(INVALID_WZID != pObj->GetCollectionID())
    {
        ////Ż��ź���¿����� �ڵ�����
        //g_pHero->SendRiderReleasePacket(false);
        //g_pHero->SendWingsControlByAccessory(false, false);

        const CollectionInfo* pCI = CollectionParser::Instance()->FindCollectionInfo(pObj->GetCollectionID());
        if(NULL != pCI)
        {
            const DWORD collect_condition = pCI->m_Condition[CollectionInfo::STAMP];

            if (((collect_condition == CollectionInfo::OBJ_COLLECT) && (true == bKeyUp)) || 
                ((collect_condition == CollectionInfo::OBJ_STAMP) && (false == bKeyUp)) || 
                ((collect_condition == CollectionInfo::OBJ_STAMP_USE_GUILD_MASTER) && (false == bKeyUp)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                || ((collect_condition == CollectionInfo::OBJ_BATTLE_GROUND_STRONGPOINT) && (true == bKeyUp))
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_008334_20150608_SONNENSCHEIN
                || ((collect_condition == CollectionInfo::OBJ_SONNENSCHEIN_STAMP) && (false == bKeyUp))
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
                || ((collect_condition == CollectionInfo::OBJ_ONEDYSGUILDMISSION_STAMP) && (true == bKeyUp))
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
                )
                )
            {
                action.ActionID = ACTION_COLLECTION_OBJECT;
                action.COLLECTOBJECT.dwTargetID = pObj->GetObjectKey();
            }
        }
    }
    else if (pObj->IsDominationObject())
    {
        action.ActionID = ACTION_DOMINATION_OBJECT;
        action.DOMINATIONOBJECT.dwTargetID = pObj->GetObjectKey();
    }
    else
    {
        // �̵�
        if( MoveToPosition( pObj->GetPosition(), true ) )
        {
            // ������ ���� ����� �̵� ��Ŷ ������.
            MSG_CG_SYNC_MOVE_SYN SendPacket;
            ZeroMemory(&SendPacket,sizeof(SendPacket));
            SendPacket.m_byCategory = CG_SYNC;
            SendPacket.m_byProtocol = CG_SYNC_MOVE_SYN;


            // MakePathPacket�� �����ߴٸ� �ٷ� ���� �����̴� Ư���� �����ʿ����.
            if( g_pHero->MakePathPacket( &SendPacket ) == FALSE)
            {
                assert(!"������ ���� �̵���Ʈ�� ����Ҽ� �����ϴ�");
            }
            else
            {

                SendPacket.Encode();

                g_pHero->SendPacket( &SendPacket, SendPacket.GetPacketSize() );
            }

#ifdef _POSITION_DEBUG
            TCHAR temp[512];
            Sprintf( temp, _T("������ move ��Ŷ ����:%f,%f,%f"),SendPacket.wvCurPos.x, SendPacket.wvCurPos.y, SendPacket.wvCurPos.z);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp);
#endif

            return TRUE;
        }

        return FALSE;
    }


    // �ڵ� ���� ����
    SetAutoAttack( FALSE );


    WzVector vOffset = pObj->GetPosition() - g_pHero->GetPosition();
    float fDistance = VectorLength( &vOffset );

    m_QueueAction = action;

    // �Ÿ��� �ʹ��ָ� �̵�~
    if( fDistance > GetActionLength( m_QueueAction ) )
    {  
        // �̵��� ����������
        if( MoveToAction(pObj->GetPosition(), false) == FALSE )
        {
            // ��ó�ζ� �̵��õ�!
                WzVector vDest2;
                float fAngle = 0.0f;
                while( fAngle <= WZ_PI * 2.0f )
                {
                    for (float add_distance = 1.0f; add_distance <= 2.5f; add_distance += 0.5f)
                    {
                        if( GetSidePosNoNearTileCheck( pObj, fAngle, &vDest2 ,add_distance) )
                        {
                            if( vDest2 != pObj->GetPosition() )
                            {
                                if( MoveToAction( vDest2, false ) == TRUE )
                                {
                                    m_QueueAction = action;
                                    return TRUE;
                                }
                            }
                        }
                    }
                    fAngle += (WZ_PI / 6.0f);
                }

            if( g_pHero )
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
            }
        }
        else
        {
            return TRUE;       
        }
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
    ���콺 ��ư�� ���� ĳ���͸� ��ŷ ������ 
*/
BOOL HeroActionInput::OnLClickedMapObject( MapObject* pObj , bool bKeyUp /*= true*/)
{
    if( m_iMoveDelay > 0 )
    {
        return FALSE;
    }

    if( pObj == NULL )
    {
        return FALSE;
    }

    if( pObj->IsAirShipObject() 
        || pObj->IsTriggerObj() 
        || pObj->IsWayPointObj() 
        || pObj->GetCollectionID() != INVALID_WZID 
        || pObj->IsDominationObject()
        )
    {

        return MoveToMapObjectWithAction( pObj, bKeyUp );
    }

    return FALSE;
}

#ifdef MAP_EDIT_MODE
//------------------------------------------------------------------------------
/**
    ���� ������Ʈ edit��  
*/
BOOL HeroActionInput::OnRClickedMapObject(MapObject* pObj)
{
    if (g_Input.GetState(DIK_LCONTROL, KS_KEY))
    {
        if (pObj && pObj->IsTerrain())
        {
            m_dwEditMapObjKey=pObj->GetObjectKey();
            g_mapEditScene.SetSelectObject(m_dwEditMapObjKey);
            return TRUE;
        }
    }

    return FALSE;
}

void HeroActionInput::SetEditMapObjKey(DWORD dwKey)
{
    m_dwEditMapObjKey=dwKey;
}

TerrainObject *HeroActionInput::GetTargetTerrainObject()
{
    Object *pObj=g_ObjectManager.GetObject(m_dwEditMapObjKey);       

    if(pObj&&pObj->IsKindOfObject(MAP_OBJECT))
    {
        MapObject *pMapObj=(MapObject *)pObj;
        if(pMapObj->IsTerrain())
        {
            return (TerrainObject *)pMapObj;
        }  
    }

    return NULL;
}

BOOL HeroActionInput::OnRClickedTile()
{
    // mouse ray�� �߸��־ ������ �ʵ���.
    if (VectorLength2(&g_wrMouseRay.m_wvDirection) <= 0.04f)
    {
        return FALSE;
    }

    WzVector vPos;

    BOOL bPickTerrain = GameFunc::GetClickedPosition(&vPos);
    if (bPickTerrain)
    {
        int iTile = g_pSunTerrain->x_pPathFinder->GetTileToStand( vPos, NULL, -1.0f, 100.0f );
        
        if(iTile!=-1 && iTile<g_pSunTerrain->x_pPathFinder->GetNumPathTile())
        {
            TerrainObject *pTerrainObj=GetTargetTerrainObject();       

            if(pTerrainObj)
            {
                if(!pTerrainObj->AddTileIndex(iTile))
                {
                    pTerrainObj->DeleteTileIndex(iTile);
                }

                return TRUE;
            }
        }

    }
    

    return FALSE;;
}

DWORD HeroActionInput::GetEditMapObjKey()
{
    return m_dwEditMapObjKey;
}

#endif 

//------------------------------------------------------------------------------
/**
    ���콺 ��ư�� ���� ĳ���͸� ��ŷ ������ 
*/
BOOL HeroActionInput::OnShiftLClickedCharacter(Character* pObj, RELATIONKIND::TYPE relationKind)
{

    if(!pObj)
        return FALSE;

    SetCurrentTarget(pObj->GetObjectKey());

    if (g_pHero) 
    {
        if (g_pHero->GetShow()) 
        {
            GET_DIALOG(IM_TARGET_MANAGER::TARGET_DIALOG)->ShowWindow(TRUE);
        }
    }

    return TRUE;
    

}

BOOL HeroActionInput::OnLDragedCharacter(Character* pObj, RELATIONKIND::TYPE relationKind)
{
    if(!pObj)
        return FALSE;

    if(!g_pHero)
        return FALSE;

    if (relationKind == RELATIONKIND::Enemy)
    {
        float fDistance;
        PLAYER_ACTION action;
        WzVector vPlayerPos = g_pHero->GetPosition();
        WzVector vOffset;
        WzVector vTargetPos;

        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_IDLE;//�׼� ���� �ʱ�ȭ   

        SetCurrentTarget(pObj->GetObjectKey());

        if (g_pHero) 
        {
            if (g_pHero->GetShow()) 
            {
                GET_DIALOG(IM_TARGET_MANAGER::TARGET_DIALOG)->ShowWindow(TRUE);
            }
        }

        if (!pObj->CanBeAttackTarget())//Ÿ���� �ɼ� �ִ� �����ΰ� 
        {
            return FALSE;
        }

        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_ATTACK;                //����            
        action.ATTACK.dwTargetID = pObj->GetObjectKey();//���  
        action.ATTACK.bForceAttack =lsForceAttack();

        
        vTargetPos = pObj->GetPosition();

        vOffset = vTargetPos - vPlayerPos;
        fDistance = VectorLength( &vOffset );

        m_QueueAction = action;

        // �Ÿ��� �ʹ��ָ� �̵�~
        if( fDistance > GetActionLength(m_QueueAction) )
        {
            MoveToAction(vTargetPos, false);
            return TRUE;       
        }

        return TRUE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnSpacePushItem( BOOL bAllPickup)
{
    if(m_iMoveDelay>0)
    {
        return FALSE;
    }

    Item            *pObj;
    Object          *pObject;
    SCObject        SCObj;
    GET_ITEM_vector vectorItem;
    GET_ITEM_vector vector_Item_another;
    GET_ITEM_vector* vector_Item_pointer;
    // ��ü ������Ʈ ��, �����۸� �̾Ƴ��� �Ÿ� ��� �� �ӽ� ����Ʈ�� ���
    Object_Map_Itr itr = g_ObjectManager.GetBegin();
    for( itr = g_ObjectManager.GetBegin() ; itr != g_ObjectManager.GetEnd() ; ++itr )
    {
        pObject = static_cast<Object *>(itr->second);
        if (0 == pObject)
        {
            assert(!"Object Is NULL!!");
            return FALSE;
        }

        // �������ΰ�?
        SCObj = (itr->second)->GetSCObject();
        if( SCObj.GetObjectType() == ITEM_OBJECT )
        {
            pObj = (Item *)pObject;

#ifdef _YMS_AUTOBOT_GAME
            if (Autobot::Instance()->IsPossiblePickup(*pObj) == false)
            {
                continue;
            }
#endif //_YMS_AUTOBOT_GAME

            // �������� ���� ���ڸ� Ŭ�� �����ϴ�~
            if( pObj->IsOpened() == TRUE )
            {
                continue;
            }

            // ���� ���� �� �ִ� �Ÿ��� �ִ��� �˻�
            // �ʹ� �ָ� �˻� ��󿡼� ����
            float fDistance;
            WzVector vPlayerPos = g_pHero->GetPosition();
            WzVector vTargetPos = pObj->GetPosition();
            WzVector vOffset = vTargetPos - vPlayerPos;
            fDistance = VectorLength( &vOffset );
            // �Ÿ��� �󸶷� �� ������ Ȯ���ϰ� ������ 
            if( fDistance <= DISTANCE_SPACE_GET_ITEM )
            {
                // ���� �� �ְԲ� ������
                GET_ITEM Getitem;
                Getitem.dwObjectKey = pObj->GetObjectKey();
                Getitem.fDistance = fDistance;
                bool is_can_get;
                if (pObj->GetItemType() == ITEM_RENDER_INFO::eFIELDITEM_MONEY)
                {
                    is_can_get = GlobalFunc::CanGetHaim(pObj);
                }
                else
                {
                    is_can_get = GlobalFunc::CanGetItem(pObj);
                }

                if (is_can_get)
                {
                    vectorItem.push_back(Getitem);
                }
                else
                {
                    vector_Item_another.push_back(Getitem);
                }
            }
        }
    }

    if( bAllPickup )
    {
        //���� : �� ����� �������� �𸣰����� ������ ������ ����ݱ��� ������ ������� ��� �ݵ��� �ص�        
        vector_Item_pointer = &vectorItem;  
        vector_Item_pointer->insert(vector_Item_pointer->end(),vector_Item_another.begin(),vector_Item_another.end());

        GET_ITEM_vector_Itr itr = vector_Item_pointer->begin();
        for( ; itr != vector_Item_pointer->end() ; ++itr)
        {
            DWORD dwItemKey = (*itr).dwObjectKey;
            pObj = (Item *) g_ObjectManager.GetObject(dwItemKey);
            if( pObj)
            {
                if( pObj->GetSendPickPacket() == FALSE)
                {
                    // �������� �԰ڴٰ� ��û�ϴ� �׼� ���
                    PLAYER_ACTION action;
                    ZeroMemory( &action, sizeof(action) );

                    // �ؾ��� �ൿ�� �����Ѵ�!
                    // �Ծ����
                    action.ActionID = ACTION_GETITEM;
                    action.GETITEM.dwItemKey = dwItemKey;
                    m_QueueAction = action;
                    if( ProcessQueueGetItem() )
                    {
                        m_QueueAction.ActionID = ACTION_IDLE; // �������� ����
                    }
                }
            }
        }
        return TRUE;
    }
    else
    {
        // ����Ʈ�� ��ϵ� �����۵��� ���� ���� �� �ִ� �ݰ� �ȿ� �ִ� �͵��̴�
        // �� �߿��� ���� ����� ���� �Դ´�.
        // ���� ��, �԰ڴٰ� ������ ��û�� �������̸� �н�~
        
        // ����� �Ÿ��� ����
       //naklu//���� ��������� �ϳ� �̻� �ִ��� ���� vectoritem �����Ϳ� ��������̳� ��������� ����
        bool is_can_pick = false;
        if (vectorItem.size() >= 1)
        {
            vector_Item_pointer = &vectorItem;
            is_can_pick = true;
        }
        else
        {
            vector_Item_pointer = &vector_Item_another;
        }
        std::sort(vector_Item_pointer->begin(), vector_Item_pointer->end(), _SortItemByDistance);
        GET_ITEM_vector_Itr item_itr = vector_Item_pointer->begin();
        GET_ITEM_vector_Itr item_itr_end = vector_Item_pointer->end();

        if (is_can_pick == false)
        {
            if (vector_Item_pointer->size() >= 1)
            {
                DWORD item_key = (*item_itr).dwObjectKey;
                Item *temp_item = static_cast<Item *>(g_ObjectManager.GetObject(item_key));
                temp_item->InitState(STATE::ITEM_DROP_STATE, g_CurTime);
            }
            // 5690 : �ֺ��� �������� ���� �������� �����ϴ�.
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(5690, message, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, message );

            return TRUE;
        }

        for ( ;item_itr != item_itr_end; ++item_itr)
        {
            GET_ITEM& item_ref = (*item_itr);
            pObj = static_cast<Item*>(g_ObjectManager.GetObject(item_ref.dwObjectKey));
            if ((pObj != NULL) && 
                (pObj->GetSendPickPacket() == FALSE))
            {
                // �԰ڴٰ� ��û�� �� ���� �������� �ݴ´�.
                ItemManager::Instance()->SendPickUpItemObject(pObj);
                pObj->SetSendPickPacket(TRUE);
                break;
            }
        }

        //GET_ITEM Min = { 0, 9999.0f };
        //GET_ITEM_vector_Itr it, HeroIt;
        //bool  bPass = false;
        //for( it = vectorItem.begin() ; it != vectorItem.end() ; ++it )
        //{
        //    pObj = (Item *)g_ObjectManager.GetObject( (*it).dwObjectKey );
        //    if( pObj )
        //    {
        //        if( pObj->GetSendPickPacket() == FALSE )
        //        {
        //            // �԰ڴٰ� ��û�� �� ���� �������̴�. ���� �Ÿ��� ����� ���� �����
        //            if( Min.fDistance > (*it).fDistance )
        //            {
        //                Min.dwObjectKey = (*it).dwObjectKey;
        //                Min.fDistance = (*it).fDistance;
        //            }
        //        }
        //    }
        //}

        //// �̹��� �԰ڴٰ� ��û�ϴ� ������ ���( �ߺ� ��û�� ���ϱ� ���� )
        //pObj = (Item *)g_ObjectManager.GetObject( Min.dwObjectKey );
        //if( pObj )
        //{
        //    // �������� �԰ڴٰ� ��û�ϴ� �׼� ���
        //    PLAYER_ACTION action;
        //    ZeroMemory( &action, sizeof(action) );

        //    // �ؾ��� �ൿ�� �����Ѵ�!
        //    // �Ծ����
        //    action.ActionID = ACTION_GETITEM;
        //    action.GETITEM.dwItemKey = Min.dwObjectKey;
        //    m_QueueAction = action;
        //    if( ProcessQueueGetItem() )
        //    {
        //        m_QueueAction.ActionID = ACTION_IDLE; // �������� ����  
        //        return TRUE;
        //    }
        //}
    }


    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void HeroActionInput::OnSearchTarget(SEARCH_TARGET_TYPE search_type)
{
    BOOL bUpdateTargetList = FALSE;

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {'\0', };

    // ����ε�/ȭ��Ʈ ����ε忡���� ���Ұ�
    if( g_pHero->IsBlind() || g_pHero->IsBlind2() )
        return;

    // Ÿ���� ��Ʈ�� ���� ��� ����( Ÿ���� ����Ʈ ���� )
    // �� �Լ��� �����ϸ� Ű�˻� �� �ʿ� ����. �ؼ��� �ȵȴ�.
    if (UpdateTargetList(15.0f, m_bForwardTargeting ,TRUE) == TRUE)
    {
        bUpdateTargetList = TRUE;
    }
    else if (UpdateTargetList(20.0f, FALSE ,TRUE) == TRUE)
    {
        bUpdateTargetList = TRUE;
    }
    else if (UpdateTargetList(DISTANCE_TAB_TARGET_MONSTER, FALSE ,TRUE) == TRUE)
    {
        bUpdateTargetList = TRUE;
    }
    else
    {
        if (GameFramework::GetCurrentScene()->GetType() != SCENE_TYPE_VILLAGE)
        {
            // �ڵ� Ÿ���� ���� �ȿ� ���Ͱ� �������� �ʽ��ϴ�.
            g_InterfaceManager.GetInterfaceString(eST_NOT_EXIST_TARGET, szMessage, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        }
    }

    if (bUpdateTargetList == TRUE)
    {
        // ���� ����� �� Ÿ����
        if (search_type == SEARCH_TARGET_NEAREST)
        {
            if (m_bFixTarget)
            {
                if (FALSE == SetNearestTarget())
                {
                    // ����( ���� ���� ó���� �� ������? )
                }
            }
            else
            {
                if (FALSE == OnNextTarget())
                {
                    // ����( ���� ���� ó���� �� ������? )
                }
            }
        }
        else if (search_type == SEARCH_TARGET_NEXT)
        {
            if (FALSE == OnNextTarget())
            {
                // ����( ���� ���� ó���� �� ������? )
            }
        }
        else if (search_type == SEARCH_TARGET_PREVIOUS)
        {
            if (FALSE == OnPrevTarget())
            {
                // ����( ���� ���� ó���� �� ������? )
            }
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::UpdateTargetList( float fLimitMonsterDistance, BOOL bCheckCameraAngle,BOOL bChekChaoState)
{
    if(!g_pHero)
        return FALSE;

    TAB_TARGET_MONSTER_vector_Itr   Check_it;
    g_pHero->m_vectorTargetMonster.clear();

    Object                       *pObject;
    SCObject                 SCObj;
    Character                   *pChar;
    // ��ü ������Ʈ ��, ���͸� �̾Ƴ��� �Ÿ� ��� �� �ӽ� ����Ʈ�� ���
    Object_Map_Itr itr = g_ObjectManager.GetBegin();
    for( itr = g_ObjectManager.GetBegin() ; itr != g_ObjectManager.GetEnd() ; ++itr )
    {
        pObject = static_cast<Object *>(itr->second);
        if (NULL == pObject)
        {
            assert(!"Object Is NULL!!");
            return FALSE;
        }

        if(!pObject->IsKindOfObject(CHARACTER_OBJECT) )
        {
            continue;
        }

        // ���ΰ�?
        pChar = (Character *)pObject;
        RELATIONKIND::TYPE RelationType = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChar, lsForceAttack() );
        if( RELATIONKIND::Enemy == RelationType )
        {
            
            // ��� �ִ� �� ���
            if( pChar->IsDead() == TRUE )
            {
                continue;
            }

#ifdef _YMS_AUTOBOT_GAME
            if (Autobot::Instance()->IsPossibleTarget(pChar->GetObjectKey()) == false)
            {
                continue;
            }
#endif //_YMS_AUTOBOT_GAME


            if(pChar->IsObserverMode())
            {
                continue;
            }

            // ���̵� ��ų������̶�� Ÿ���úҰ�
            if(pChar->GetHideSkill()==HIDE_SKILL_INFO::ALL_HIDE)
            {
                continue;
            }

            // ���� Ÿ���� �� �� �ִ� �Ÿ����� �˻�
            // �ʹ� �ָ� �˻� ��󿡼� ����
            float fMinAngle = pChar->GetAngle();
            float fDistance;
            WzVector vPlayerPos = g_pHero->GetPosition();
            WzVector vTargetPos = pChar->GetPosition();
            WzVector vOffset = vTargetPos - vPlayerPos;
            fDistance = VectorLength( &vOffset );
            if( fDistance <= fLimitMonsterDistance &&fabs(vOffset.z)<10.0f)
            {
                // Ÿ���� �� �� �ִ� �Ÿ��̴�
                // �Ÿ� �켱 �ɼ����� �Ǿ� ������ ī�޶� ���� �ȵ�����
                if( bCheckCameraAngle )
                {
                    WzVector vOffset = g_pHero->GetPosition() - vTargetPos; 
                    WzVector vCamDirection = g_pHero->GetPosition() - g_Camera.GetCameraPosition();
                    float fDotResult = VectorDotProduct(&vCamDirection,&vOffset); 
                    if(fDotResult > 0)
                    {
                        continue;
                    }
                }

                TAB_TARGET_MONSTER  TAB_TargetMonster;
                TAB_TargetMonster.dwObjectKey = pChar->GetObjectKey();
                TAB_TargetMonster.fAngle = fMinAngle;
                TAB_TargetMonster.fDistance = fDistance;
                TAB_TargetMonster.wTargetCount = 0;
                TAB_TargetMonster.wTargetCheck = 1;
                g_pHero->m_vectorTargetMonster.push_back( TAB_TargetMonster );
            }
        }
    }
    if( g_pHero->m_vectorTargetMonster.empty() )
    {
        return FALSE;
    }

    // �Ÿ��� ���� ����� ������ ����
    sort( g_pHero->m_vectorTargetMonster.begin(), g_pHero->m_vectorTargetMonster.end() );


    return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnTabPushTarget( void )
{
    if(!g_pHero)
        return FALSE;

    if( g_pHero->m_vectorTargetMonster.empty() == true )
    {
        // �����.
        return TRUE;
    }

    g_pHero->m_CurTargetInfo.wTargetCount = 0;

    // ����Ʈ�� ��ϵ� ���͵��� Ÿ������ �� �ִ� �ݰ� �ȿ� �ִ� �͵��̴�
    // �� �߿��� ���� ����� ���� Ÿ�����Ѵ�.
    TAB_TARGET_MONSTER  Min = { 0, 99999.0f, 0, 0 };
    TAB_TARGET_MONSTER_vector_Itr it;
    int       iCount = 0;
    for( it = g_pHero->m_vectorTargetMonster.begin() ; it != g_pHero->m_vectorTargetMonster.end() ; ++it )
    {
        // �Ÿ��� ���� ����� ���� �����
        if( Min.fDistance > (*it).fDistance )
        {
            Min.dwObjectKey = (*it).dwObjectKey;
            Min.fDistance = (*it).fDistance;

            g_pHero->m_CurTargetInfo.wTargetCount = iCount;
        }

        ++iCount;
    }

    // Ÿ�� ����
    SetCurrentTarget( Min.dwObjectKey );
    g_pHero->m_CurTargetInfo.dwObjectKey = Min.dwObjectKey;
    g_pHero->m_CurTargetInfo.fDistance = Min.fDistance;


    return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnNextTarget( void )
{
    if(!g_pHero)
        return FALSE;

    if( g_pHero->m_vectorTargetMonster.empty() == true )
    {
        // �����.
        return TRUE;
    }

    // ����Ʈ�� ��ϵ� ���͵��� Ÿ������ �� �ִ� �ݰ� �ȿ� �ִ� �͵��̴�
    // �� �߿��� �����̼� ��Ų��.( ���� Ÿ�� )
    TAB_TARGET_MONSTER_vector_Itr it;
    
    int       iCount = 0;
    BOOL  bFind = FALSE;
    
    for( it = g_pHero->m_vectorTargetMonster.begin() ; it != g_pHero->m_vectorTargetMonster.end() ; ++it )
    {
        // �Ÿ��� ���� ����� ���� �����
        if( g_pHero->m_CurTargetInfo.dwObjectKey == (*it).dwObjectKey )
        {
            ++it;
            if( it == g_pHero->m_vectorTargetMonster.end() )
            {
                it = g_pHero->m_vectorTargetMonster.begin();
            }

            g_pHero->m_CurTargetInfo.dwObjectKey = (*it).dwObjectKey;
            g_pHero->m_CurTargetInfo.fDistance = (*it).fDistance;
            g_pHero->m_CurTargetInfo.wTargetCheck = 0;
            g_pHero->m_CurTargetInfo.wTargetCount = iCount;

            bFind = TRUE;
            break;
        }
        ++iCount;
    }

    if( bFind == FALSE )
    {
        it = g_pHero->m_vectorTargetMonster.begin();
        g_pHero->m_CurTargetInfo.dwObjectKey = (*it).dwObjectKey;
        g_pHero->m_CurTargetInfo.fDistance = (*it).fDistance;
        g_pHero->m_CurTargetInfo.wTargetCheck = 0;
        g_pHero->m_CurTargetInfo.wTargetCount = -1;
    }

    // Ÿ�� ����
    SetCurrentTarget( g_pHero->m_CurTargetInfo.dwObjectKey );


    return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnPrevTarget( void )
{
    if(!g_pHero)
        return FALSE;

    if( g_pHero->m_vectorTargetMonster.empty() == true )
    {
        // �����.
        return TRUE;
    }


    // ����Ʈ�� ��ϵ� ���͵��� Ÿ������ �� �ִ� �ݰ� �ȿ� �ִ� �͵��̴�
    // �� �߿��� �����̼� ��Ų��.( ���� Ÿ�� )
    TAB_TARGET_MONSTER_vector_Itr it;
    int       iCount = 0;
    BOOL  bFind = FALSE;
    for( it = g_pHero->m_vectorTargetMonster.begin() ; it != g_pHero->m_vectorTargetMonster.end() ; ++it )
    {
        // �Ÿ��� ���� ����� ���� �����
        if( g_pHero->m_CurTargetInfo.dwObjectKey == (*it).dwObjectKey )
        {
            if( it != g_pHero->m_vectorTargetMonster.begin() )
            {
                --it;
            }
            else
            {
                it = g_pHero->m_vectorTargetMonster.end();
                --it;
            }
            g_pHero->m_CurTargetInfo.dwObjectKey = (*it).dwObjectKey;
            g_pHero->m_CurTargetInfo.fDistance = (*it).fDistance;
            g_pHero->m_CurTargetInfo.wTargetCheck = 0;
            g_pHero->m_CurTargetInfo.wTargetCount = iCount - 1;

            bFind = TRUE;
            break;
        }

        ++iCount;
    }

    if( bFind == FALSE )
    {
        it = g_pHero->m_vectorTargetMonster.end();
        --it;
        g_pHero->m_CurTargetInfo.dwObjectKey = (*it).dwObjectKey;
        g_pHero->m_CurTargetInfo.fDistance = (*it).fDistance;
        g_pHero->m_CurTargetInfo.wTargetCheck = 0;
        g_pHero->m_CurTargetInfo.wTargetCount = -1;
    }

    // Ÿ�� ����
    SetCurrentTarget( g_pHero->m_CurTargetInfo.dwObjectKey );

    return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnLClickedItem(Item* pObj)
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if(m_iMoveDelay>0)
    {
        return FALSE;
    }

    if(!pObj)
        return FALSE;

    if (pObj->IsOpened())
    {
        return TRUE;
    }
    
    bool is_can_get;
    if (pObj->GetItemType() == ITEM_RENDER_INFO::eFIELDITEM_MONEY)
    {
        is_can_get = GlobalFunc::CanGetHaim(pObj);
    }
    else
    {
        is_can_get = GlobalFunc::CanGetItem(pObj);
    }

    if (is_can_get == false)
    {
        if (uiAreaConquestMan::CanPickACItem(pObj,true) == true)
        {
            // �����ۿ� ���� �������� �����ϴ�.
            g_InterfaceManager.GetInterfaceString( eST_NOT_OWNER, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
        }
        pObj->InitState(STATE::ITEM_DROP_STATE, g_CurTime);
        return TRUE;
    }
    
    if(!g_pHero)
        return FALSE;

    float fDistance;
    PLAYER_ACTION action;
    WzVector vPlayerPos = g_pHero->GetPosition();
    WzVector vOffset;
    WzVector vTargetPos;

    ZeroMemory(&action,sizeof(action));
    action.ActionID = ACTION_IDLE;

    // �ؾ��� �ൿ�� �����Ѵ�!
    // �Ծ����
    action.ActionID = ACTION_GETITEM;
    action.GETITEM.dwItemKey = pObj->GetObjectKey();
    
    // �ؾ����ൿ�� ������ ����ģ��.
    if (action.ActionID == ACTION_IDLE)
    {
        return FALSE;
    }

    vTargetPos = pObj->GetPosition();
    vOffset = vTargetPos - vPlayerPos;
    fDistance = VectorLength( &vOffset );
    m_QueueAction = action;
    
    // �Ÿ��� �ʹ��ָ� �̵�~
    if( fDistance > GetActionLength(m_QueueAction) )
    {
        MoveToAction(vTargetPos, false);
        return TRUE;
    }
    
    return TRUE;
}

//------------------------------------------------------------------------------
/**
    Todo:�̵��� ��ųť�� �׻� ����.
*/
BOOL HeroActionInput::OnLClickedPosition(WzVector* pClickedPosition)
{
    if( g_pHero == NULL )
    {
        return FALSE;
    }

    if( CannotMoveState() )
    {
        return FALSE;
    }

    // �̵��ϱ� ������ �ð� ����
    Object   *pObj = g_ObjectManager.GetObject( GetCurrentTarget() );
    if( pObj )
    {
        g_pHero->SetStartMoveTime( g_CurTime );
    }
    
    if(pClickedPosition)
    {
        WzVector OldPosition=g_pHero->GetTarGetPosition();

        WzVector vTemp=*pClickedPosition-OldPosition;

        float fLen=VectorLength(&vTemp);

        //0.5�� �ȿ� ����� ��ǥ�� ���� ���� 
        if(fLen<=1.0f&&g_CurTime-m_dwStartPickPosTime<=UPDATE_MOVE_POSITION_TIME)
        {
            return FALSE;
        }
    }

#ifdef _NA_000000_20151125_MOUSECLICK_AREA_SELECT_CANCLE
    // ���콺 Ŭ������ ��ų ť�� Ǯ���� ����
#else
    if(!m_ShowTargetAreaPointer)
        g_SkillQueue.ClearAll();
#endif //_NA_000000_20151125_MOUSECLICK_AREA_SELECT_CANCLE

    if( IsIdleState() || g_pHero->GetCurState() == STATE::MOVE ) 
    {
        // �����̵� ó��   CTRL+MOUSE_L_BUTTON
        if (GENERALPARAM->GetSpecialMode())
        {
            WzVector vPos=*pClickedPosition;
            vPos.z+=2.0f;

            int iTile = 0;

            iTile = g_pSunTerrain->x_pPathFinder->GetAllowTileToStand( vPos, NULL, -1.0f, 100.0f );

            if(iTile >= 0)
            {
                if (GENERALPARAM->IsNet())
                {
                    if (g_Input.GetState(DIK_LCONTROL, KS_KEY))
                    {
                        MSG_CG_MAP_TELEPORT_SYN SendPacket;
                        SendPacket.m_byCategory = CG_MAP;
                        SendPacket.m_byProtocol = CG_MAP_TELEPORT_SYN;
                        SendPacket.vPos = *pClickedPosition;
                        g_pHero->SendPacket( &SendPacket, sizeof(SendPacket) );
                        m_dwStartPickPosTime=g_CurTime;

                        return TRUE;
                    }
                }
                else
                {
                    if (g_Input.GetState(DIK_LCONTROL, KS_KEY))
                    {
                        m_dwStartPickPosTime=g_CurTime;
                        g_pHero->SetPosition(*pClickedPosition);
                        return TRUE;
                    }
                }
            }
        }

        if(m_ShowTargetAreaPointer)
        {
            m_bPicking = TRUE;
            m_IsClickedTargetArea = TRUE;
            m_TargetAreaPosition = m_PickingPosition;
            return TRUE;
        }

        if(m_iMoveDelay>0)
        {
            return FALSE;
        }

        // ���� ���� �õ�!!
        if(g_pHero->SetNextState( STATE::MOVE,g_CurTime ) )
        {
            // ����!!
            // ���ݸ�ǥ�� ����
            g_pHero->SetTargetID( 0 );
            m_dwStartPickPosTime=g_CurTime;

            WzVector vRet;
            vRet = *pClickedPosition;
            FindNearesetPathTile(pClickedPosition,&vRet);
            // �̵��õ�!!
            // ����!
            if (g_pHero->Move(g_pHero->GetPosition(),vRet,CMS_RUN,FALSE) == TRUE)
            {
                // ������ ���� ����� �̵� ��Ŷ ������.
                MSG_CG_SYNC_MOVE_SYN    SendPacket;
                ZeroMemory(&SendPacket,sizeof(SendPacket));
                SendPacket.m_byCategory = CG_SYNC;
                SendPacket.m_byProtocol = CG_SYNC_MOVE_SYN;

                // MakePathPacket�� �����ߴٸ� �ٷ� ���� �����̴� Ư���� �����ʿ����.
                if( g_pHero->MakePathPacket( &SendPacket ) == FALSE)
                {
                    assert(!"������ ���� �̵���Ʈ�� ����Ҽ� �����ϴ�");
                }


                SendPacket.Encode();

                g_pHero->SendPacket( &SendPacket, SendPacket.GetPacketSize() );
                g_InterfaceManager.SetMovePointer(TRUE,pClickedPosition);

                
#ifdef _POSITION_DEBUG
                TCHAR temp[512];
                Sprintf( temp, _T("������ move ��Ŷ ����:%f,%f,%f"),SendPacket.wvCurPos.x, SendPacket.wvCurPos.y, SendPacket.wvCurPos.z);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp);
#endif

                
                return TRUE;   
            }
            else
            {
                if (g_pHero->GetCurState()==STATE::MOVE)
                {                  
                    // STOP
                    g_pHero->SendStopMovePacket();
                }

                // ���н�
                g_pHero->SetNextState(STATE::IDLE,g_CurTime);
                return TRUE;
            }
        }
    }

    return FALSE;
}

BOOL  
HeroActionInput::OnLClickedPositionWithItem(WzVector* pClickedPosition)
{
    if(m_iMoveDelay>0)
    {
        return FALSE;
    }

    // �����ִ� ���� ��������� �̵����·� �ٲ��ش�.
    // ��� Tile ���
    WzVector wvTarget = *pClickedPosition;
    // ��ã�� Ÿ���� ������� �Է� ����
    if (g_pSunTerrain->x_pPathFinder->GetTileToStand( wvTarget, NULL, -1.0f, 100.0f ) < 0)
    {
        return FALSE;
    }

    if (MouseHandler::Instance()->IsExistItemAtHand())
    {
        if (MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx == SI_STYLE)
        {
            ItemManager::Instance()->SendQuickStyleRemoveMsg(
                MouseHandler::Instance()->GetMouseItem()->m_fromPos);

            return TRUE;           
        }
        else if (MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx == SI_QUICK)
        {
            ItemManager::Instance()->SendQuickItemRemoveMsg(
                MouseHandler::Instance()->GetMouseItem()->m_fromPos);

            return TRUE;
        }
        else if (MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx == SI_TRADE)
        {
            uiTradeMan* tradeMan =
                static_cast<uiTradeMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_TRADE));
            if (tradeMan)
            {
                tradeMan->TEST_Get(MouseHandler::Instance()->GetMouseItem()->m_fromPos);
            }
            return TRUE;
        }
        else if (MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx == SI_INVENTORY)
        {
            if (MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit)
            {
                if (MouseHandler::Instance()->GetMouseItem()->m_pRenderUnit->m_isDiv)
                {
                }
                else
                {
                    ItemManager::Instance()->SendDropItemMsg(
                        MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx,  
                        MouseHandler::Instance()->GetMouseItem()->m_fromPos );     
                }
                
            }
            else
            {
                ItemManager::Instance()->SendDropItemMsg(
                    MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx,  
                    MouseHandler::Instance()->GetMouseItem()->m_fromPos );     
            }

            return TRUE;
        }
        else if (MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx == SI_EQUIPMENT)
        {
            BOOL bUnEquip = ItemManager::Instance()->CanMoveUnEquipItem(
                MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx, 
                MouseHandler::Instance()->GetMouseItem()->m_fromPos, 
                TRUE);

            if (!bUnEquip)
            {
                MouseHandler::Instance()->RollbackItemAtHand();
                return FALSE;
            }

            // Ŭ���̾�Ʈ������ ������ �ŷ� Ÿ�Կ� ���� üũ ����
            ItemManager::Instance()->SendDropItemMsg(
                MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx,  
                MouseHandler::Instance()->GetMouseItem()->m_fromPos );
            return TRUE;
        }
        else if (MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx == SI_AUTOMATIC_INFO)
        {
            SLOTIDX slot_idx = MouseHandler::Instance()->GetMouseItem()->m_fromSlotIdx;
            POSTYPE from_pos = MouseHandler::Instance()->GetMouseItem()->m_fromPos;

            ItemManager::Instance()->GetContainer(slot_idx)->DeleteSlot(from_pos, NULL);
            MouseHandler::Instance()->RemoveItemAtHand();
            return TRUE;
        }
        else
        {
            MouseHandler::Instance()->RollbackItemAtHand();
            return TRUE;
        }

    }

    return FALSE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnRClickedCharacter(Character* pObj, RELATIONKIND::TYPE relationKind)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnRClickedPosition(WzVector* pClickedPosition)
{
    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void HeroActionInput::ProcessAutoAttack()
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if(!g_pHero)
        return;

    if( g_pHero->IsDead() && m_bAutoAttack )
    {
        // ���ΰ��� �׾���.
        SetAutoAttack(FALSE);

        SetCurrentTarget( 0 );
        return;
    }

    switch (g_pHero->GetCurState())
    {
    case STATE::SKILL:
    case STATE::DAMAGE:
    case STATE::KNOCKBACK2:
    case STATE::DOWN:
    case STATE::AIR:
    case STATE::DEATH:
    case STATE::SIT:
    case STATE::SPECIALMOVE:
    case STATE::KEYBOARDJUMP:
    case STATE::TRANSFORM:
        {
            return;
        }
        break;
    }
    
    if (m_bAutoAttack && m_ShowTargetAreaPointer == FALSE )
    {
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        WzVector vTargetPos;

        DWORD dwTarget = GetCurrentTarget();
        Character *pChr = (Character *)g_ObjectManager.GetObject(dwTarget);
        if (pChr)
        {

            RELATIONKIND::TYPE RelationType = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChr, lsForceAttack() );
            if( RelationType != RELATIONKIND::Enemy )
            {
                SetAutoAttack(FALSE);
                return;  
            }

            if (pChr->IsDead())
            {
                SetAutoAttack(FALSE);
                return;
            }

            if (CannotAttack(pChr))
            {
                SetAutoAttack(FALSE);
                return;
            }


            // �÷��̾ ���� �϶� 0.5�ʸ��� �� ��ġ ����
            if (GAMEOPTION->GetAutoMove())
            {
                if( g_pHero->IsMoving() && g_pHero->GetCurState() == STATE::MOVE )
                {
                    if( (g_CurTime - g_pHero->GetStartMoveTime()) >= UPDATE_MOVE_POSITION_TIME )
                    {
                        g_pHero->SetStartMoveTime( g_CurTime );

                        DWORD dwTarget = GetCurrentTarget();
                        Character *pChr = (Character *)g_ObjectManager.GetObject(dwTarget);
                        if (pChr)
                        {
                            vTargetPos = pChr->GetPosition();
                            WzVector vDistance = vTargetPos - g_pHero->GetPosition();
                            float fDistance = VectorLength(&vDistance);

                            if( fDistance > GetActionLength(m_QueueAction) + g_pHero->GetBodyRadius() )
                            {
                                action.ActionID = ACTION_ATTACK;
                                action.ATTACK.dwTargetID = GetCurrentTarget();
                                action.ATTACK.bForceAttack =lsForceAttack();

                                m_QueueAction = action;
                                
                                if (MoveToAction(vTargetPos, false) == FALSE)
                                {
                                    // ��ó�ζ� �̵��õ�!
                                    // ��ó�ζ� �̵��õ�!
                                    WzVector vSrc=g_pHero->GetPosition();
                                    MoveToAction(pChr->GetNearTargetPos(vSrc), false);       

                                }

                                return;
                            }
                        }
                    }
                }
            }


            if( m_QueueAction.ActionID != ACTION_IDLE ) // �������� ����  
            {
                return;
            }

            if( IsShootingMode() == FALSE )
            {
                if( GetCurrentTarget() )
                {
                    action.ActionID = ACTION_ATTACK;
                    action.ATTACK.dwTargetID = GetCurrentTarget();
                    action.ATTACK.bForceAttack =lsForceAttack();
                    m_QueueAction = action;

                    // �Ÿ��� �ʹ��ָ� �̵�~
                    if (g_pHero->IsRangedNormalAttack())
                    {
                        vTargetPos = pChr->GetPosition();
                    }
                    else
                    {
                        vTargetPos = pChr->GetPosition();
                    }
                    WzVector vDistance = vTargetPos - g_pHero->GetPosition();
                    float fDistance = VectorLength(&vDistance);

                    if (GAMEOPTION->GetAutoMove())
                    {
                        if( fDistance > GetActionLength(m_QueueAction) + pChr->GetBodyRadius() )
                        {
                            if(g_pHero->GetCurState() == STATE::SPECIALMOVE||g_pHero->GetPreState() == STATE::SPECIALMOVE)//Ư�� �̵��߿� ��Ÿ� ����� ������� ��� 
                            {
                                SetAutoAttack( FALSE );
                            }
                            else
                            {

                                g_pHero->SetStartMoveTime( g_CurTime );
                                    
                                if (MoveToAction(vTargetPos, false) == FALSE)
                                {
                                    // ��ó�ζ� �̵��õ�!
                                    WzVector vSrc=g_pHero->GetPosition();
                                    MoveToAction(pChr->GetNearTargetPos(vSrc), false);       
                                }

                            }

                            return;
                        }
                    }
                    else
                    {
                        // ���ݽ� �ڵ��̵��� ���� ��� �Ÿ��� �ָ� ���ݸ���� ��ҽ�Ų��.
                        if( fDistance > GetActionLength(m_QueueAction) )
                        {
                            SetAutoAttack( FALSE );

                            // Ÿ���� ��Ÿ����� ���� �����ϴ�.
                            g_InterfaceManager.GetInterfaceString( eST_SKILL_FAIL_NOT_ENOUGH_DISTANCE, szMessage, INTERFACE_STRING_LENGTH );
                            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                            return;
                        }

                    }

                    return;
                }
            }
            else
            {
                if (!pChr->CanBeAttackTarget())
                {
                    return;
                }

                // ������ ��Ŭ��
                if ( this->GetCurrentTarget() == pChr->GetObjectKey())
                {
                    // �����̰� �������� �����. �����
                    if (g_pHero->GetValkyrieShotDelay() > VALKYRIE_SHOT_DELAY_MARGIN)
                    {
                        return;
                    }

                    // �Ÿ��� �� �־ �����.
                    WzVector vDiff = pChr->GetPosition() - g_pHero->GetPosition();
					float fDistance = VectorLength(&vDiff);

                    if ( fDistance >= (g_pHero->GetAttackRange() + pChr->GetBodyRadius()))
                    {
                        if( g_pHero->IsMoving() == FALSE )
                        {
                            // �Ÿ��� �ʹ��ָ� �̵�~
                            action.ActionID = ACTION_IDLE;
                            action.ATTACK.dwTargetID = GetCurrentTarget();
                            m_QueueAction = action;

                            if (GAMEOPTION->GetAutoMove())
                            {
                                g_pHero->SetStartMoveTime( g_CurTime );

                                if (MoveToAction(pChr->GetPosition(), false) == FALSE)
                                {
                                    // ��ó�ζ� �̵��õ�!
                                    WzVector vSrc=g_pHero->GetPosition();
                                    MoveToAction(pChr->GetNearTargetPos(vSrc), false);       
                                    
                                }
                            }
                        }
                        else if( m_bKeyBoardMoveCheck == TRUE )
                        {
                            return;
                        }

                        return;
                    }

                    if( g_pHero->IsMoving() )
                    {
                        // STOP
                        g_pHero->StopMove();
                        g_pHero->SendStopMovePacket();
                    }

                    ValkyrieShot();

                    return;
                }
            }
        }
        SetAutoAttack(FALSE);

    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnValkyrieLClickedCharacter(Character* pObj)
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if(!pObj)
        return FALSE;

    SetCurrentTarget(pObj->GetObjectKey());

    if(!g_pHero)
        return FALSE;

    if (g_pHero->GetShow()) 
    {
        GET_DIALOG(IM_TARGET_MANAGER::TARGET_DIALOG)->ShowWindow(TRUE);
    }
    

    float fDistance = 0.0f;
    PLAYER_ACTION action;
    WzVector vPlayerPos = g_pHero->GetPosition();
    WzVector vOffset;
    WzVector vTargetPos;

    ZeroMemory(&action,sizeof(action));
    action.ActionID = ACTION_IDLE;//�׼� ���� �ʱ�ȭ   


    RELATIONKIND::TYPE relationKind = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pObj, lsForceAttack() );

    // ���� ���� ����

    if (g_pHero->GetCurState() == STATE::SKILL)
    {
        return TRUE;
    }

    if (relationKind == RELATIONKIND::Enemy)
    {
        if (!pObj->CanBeAttackTarget())
        {
            return FALSE;
        }

        if (CannotAttack(pObj))
        {
            return FALSE;
        }

        // ������ ��Ŭ��

        SetCurrentTarget(pObj->GetObjectKey());

        if( IsIdleState() ) 
        {
            g_pHero->SetDirection(pObj->GetPosition(), TRUE);
        }
        else if (g_pHero->GetCurState() == STATE::MOVE)
        {
            g_pHero->StopMove();
            g_pHero->SendStopMovePacket();
            g_pHero->SetDirection( pObj->GetPosition(), TRUE);                
        }

        // ����!
        SetAutoAttack(TRUE);
        SetCurrentTarget( pObj->GetObjectKey() );


        // �����̰� �������� �����. �����
        if (g_pHero->GetValkyrieShotDelay() > VALKYRIE_SHOT_DELAY_MARGIN)
        {
            return TRUE;
        }
        // �Ÿ��� �� �־ �����.
        WzVector vPos = g_pHero->GetPosition();
        WzVector vDest = pObj->GetPosition();
        WzVector vDiff = vDest - vPos;

		float fLength = VectorLength(&vDiff);

        if (GAMEOPTION->GetAutoMove())
        {
            if( fLength > GetAttackActionLength() + pObj->GetBodyRadius() )
            {
                // �Ÿ��� �ʹ��ָ� �̵�~
                action.ActionID = ACTION_IDLE;
                action.ATTACK.dwTargetID = GetCurrentTarget();
                m_QueueAction = action;

                g_pHero->SetStartMoveTime( g_CurTime );

                MoveToAction(vDest, false);
                return TRUE;
            }
        }
        else
        {
            // ���ݽ� �ڵ��̵��� ���� ��� �Ÿ��� �ָ� ���ݸ���� ��ҽ�Ų��.
            if( fLength > GetAttackActionLength() + pObj->GetBodyRadius())
            {
                SetAutoAttack( FALSE );
                action.ActionID = ACTION_IDLE;
                action.ATTACK.dwTargetID = GetCurrentTarget();
                m_QueueAction = action;

                // Ÿ���� ��Ÿ����� ���� �����ϴ�.
                g_InterfaceManager.GetInterfaceString( eST_SKILL_FAIL_NOT_ENOUGH_DISTANCE, szMessage, INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                return TRUE;
            }
        }

        if( g_pHero->IsMoving() )
        {
            // STOP
            g_pHero->StopMove();
            g_pHero->SendStopMovePacket();
        }

        ValkyrieShot();
        return TRUE;
        
    }
    else if (relationKind == RELATIONKIND::Assistant)
    {      
        vTargetPos = pObj->GetPosition();
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_NPCMEET;                      
        action.NPCMEET.dwNPCKey = pObj->GetObjectKey();   
        WzVector vDistance = vTargetPos - g_pHero->GetPosition();
        fDistance = VectorLength(&vDistance);

        SetAutoAttack(FALSE);

    }
    else if (relationKind == RELATIONKIND::Vendor)
    {      
        vTargetPos = pObj->GetPosition();
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_VENDORMEET;                      
        action.VENDORMEET.dwTargetID = pObj->GetObjectKey();    
        WzVector vDistance = vTargetPos - g_pHero->GetPosition();
        fDistance = VectorLength(&vDistance);

        SetAutoAttack(FALSE);
    }
    else if (relationKind == RELATIONKIND::Friend)
    {
        vTargetPos = pObj->GetPosition();
        ZeroMemory(&action,sizeof(action));
        action.ActionID = ACTION_PLAYERMEET;                      
        action.PLAYERMEET.dwTargetID = pObj->GetObjectKey();

        // �ڵ� ���� ����
        SetAutoAttack(FALSE);
    }


    // �ؾ����ൿ�� ������ ����ģ��.
    if (action.ActionID == ACTION_IDLE)
    {
        return FALSE;
    }

    vOffset = vTargetPos - vPlayerPos;
    fDistance = VectorLength( &vOffset );

	if( action.ActionID == ACTION_ATTACK )
	{
		fDistance -= pObj->GetBodyRadius();
	}
    
	m_QueueAction = action;


    // �Ÿ��� �ʹ��ָ� �̵�~
    if( fDistance > GetActionLength(m_QueueAction) )
    {
        g_pHero->SetStartMoveTime( g_CurTime );

        if( MoveToAction(vTargetPos, false) == FALSE )
        {
            // Ÿ���� NPC�ϰ�츸 �ٽ� �ѹ� �� ���
            if (relationKind == RELATIONKIND::Assistant)
            {
                // NPC�� �� ������ ���� �� �ִ� �ִ� �Ÿ����� �ణ �� �� ��ġ�� �Ѱ��༭ �ٽ� �ѹ� �� ���
                WzVector vDir = g_pHero->GetPosition() - vTargetPos;
                VectorNormalize( &vDir, &vDir );

                // NPC�� ���� �� �ִ� �Ÿ�( DISTANCE_NPC_MEET - 6���� )�� 70%
                // �� ���� NPC�� ���� �� �ִ� �Ÿ����� 30% �Ÿ���ŭ �� �ٰ����ٴ� �ǹ��̴�.
                WzVector TargetPos = vTargetPos + ( vDir * (DISTANCE_NPC_MEET * DISTANCE_NPC_MEET_EXT) );

                CPathExplorer *pPE = NULL;
                pPE = ( CPathExplorer* )Func_ContactTerrain( CT_NEW_PATH_EXPLORER, 0 );

                if( pPE )
                {
                    g_pSunTerrain->PathExp_Reset( pPE, &TargetPos, -1);

                    if( MoveToAction(TargetPos, false) == FALSE )
                    {
                        if(pPE)
                        {
                            // ��ã�� ��� �ı�
                            Func_ContactTerrain( CT_DELETE_PATH_EXPLORER, pPE );

                            pPE = NULL;
                        }
                        return TRUE;
                    }

                    m_QueueAction.ActionID = ACTION_NPCMEET;

                    if(pPE)
                    {
                        // ��ã�� ��� �ı�
                        Func_ContactTerrain( CT_DELETE_PATH_EXPLORER, pPE );

                        pPE = NULL;
                    }
                }
            }
        }
        return TRUE;       
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::OnValkyrieLDragedCharacter(Character* pObj)
{
    if(!pObj)
        return FALSE;

    if(!g_pHero)
        return FALSE;

    RELATIONKIND::TYPE relationKind = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pObj, lsForceAttack() );
    // ���� ���� ����
    if (relationKind == RELATIONKIND::Enemy)
    {
        if (!pObj->CanBeAttackTarget())
        {
            return FALSE;
        }
        // ������ ��Ŭ��
        if ( this->GetCurrentTarget() == pObj->GetObjectKey())
        {
            // �����̰� �������� �����. �����
            if (g_pHero->GetValkyrieShotDelay() > VALKYRIE_SHOT_DELAY_MARGIN)
            {
                return TRUE;
            }
            // �Ÿ��� �� �־ �����.
            WzVector vPos = g_pHero->GetPosition();
            WzVector vDest = pObj->GetPosition();
            WzVector vDiff = vDest - vPos;

            if (VectorLength(&vDiff) > GetAttackActionLength())
            {
                return FALSE;
            }



            ValkyrieShot();
            return TRUE;
        }
        else
        {
            SetCurrentTarget(pObj->GetObjectKey());
            if (g_pHero->GetCurState() == STATE::IDLE) 
            {
                g_pHero->SetDirection(pObj->GetPosition());
            }
            else if (g_pHero->GetCurState() == STATE::MOVE)
            {
                g_pHero->StopMove();
                g_pHero->SendStopMovePacket();
                g_pHero->SetDirection(pObj->GetPosition());
            }
            return TRUE;
        }
    }
    

    return FALSE;
}


BOOL HeroActionInput::ProcessQueueTransForm()
{
    if(!g_pHero)
        return FALSE;

    PLAYER_ACTION action=m_TransFormActionQueue[0];

    if(g_pHero->GetCurState()==STATE::TRANSFORM)
    {
        if(g_pHero->GetTransformData().GetTransFormType()==action.TRANSFORM.transFormType)
        {
            m_TransFormActionQueue.Erase(0);
            return TRUE;
        }          
    }
	else if( g_pHero->GetCurState() == STATE::REVIVE )
	{
		m_TransFormActionQueue.Erase(0);
		return TRUE;
	}
    else 
    {
        g_pHero->DoAction(&action);

    }
        
    return FALSE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::ProcessQueueAction()
{
    //Todo: ��ų ť�� �˻� �հ� ��尡 �ִٸ� ��ų ���μ����� �����Ѵ�.
    BOOL bRet = FALSE;

    if(!g_pHero)
        return FALSE;

    if(g_pHero->IsDead())
    {
        g_SkillQueue.ClearAll();
    }

    if(m_TransFormActionQueue.Size()>0)//���� ���� ������ �ִٸ� ���� ����
    {
        bRet = ProcessQueueTransForm();
    }
    else 
    
    if(g_SkillQueue.Size()>0)    //�״��� ��ų 
    {
        bRet = ProcessQueueSkill();
    }
    else                              //�Ϲ� ���� �͵� 
    {
        switch(m_QueueAction.ActionID)
        {
        case ACTION_SPECIAL_MOVE:
            bRet = ProcessSpecialMove();
            break;

        case ACTION_ATTACK:
            bRet = ProcessQueueAttack();
            break;

        case ACTION_NPCMEET:
            bRet = ProcessQueueNPCMeet();
            break;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
		case ACTION_STATUE_PLAYER_MEET:
			bRet = ProcessQueueStatuePlayerMeet();
			break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
        case ACTION_VENDORMEET:
            bRet = ProcessQueueVendorMeet();
            break;
        case ACTION_PLAYERMEET:
            bRet = ProcessQueuePlayerMeet();
            break;
        case ACTION_USE_OBJECT:
            bRet = ProcessQueueUseObject();
            break;       
        case ACTION_COLLECTION_OBJECT:
            bRet = ProcessQueueCollectObject();
            break;   
        case ACTION_GETITEM:
            bRet = ProcessQueueGetItem();
            break;
        case ACTION_VALKYRIESHOT:
            bRet = ProcessQueueShot();
            break;
        case ACTION_PLAYERFOLLOW:
            bRet = ProcessQueuePlayerFollow();
            break;
        case ACTION_WAYPOINT_MEET:
            bRet = ProcessQueueWayPointMeet();
            break;
        case ACTION_AIRSHIP_MEET:
            bRet = ProcessQueueAirShipMeet();
            break;
        case ACTION_DOMINATION_OBJECT:
            bRet = ProcessQueueDominationObject();
            break;           
        }

    }
    

    if (bRet)
    {
        m_QueueAction.ActionID = ACTION_IDLE; // �������� ����  
        return TRUE;
    }
    else
    {
        //���� ���°� �⺻�̰� �̵��� �ؾ� �ϰų�,���� �̵����϶� �� Action���� ���� �Ǳ⸦ ��ٸ� 
        if ( ( IsIdleState() && g_pHero->GetNextState() == STATE::MOVE) || g_pHero->GetCurState() == STATE::MOVE )
        {
            return FALSE;
        }
        //�̹� ������ �ϰ� �ְ� ���� ���۵� �����ϰ�� ProcessQueueAttack ���� ��ٸ�
        if (g_pHero->GetCurState() == STATE::ATTACK && m_QueueAction.ActionID == ACTION_ATTACK)
        {
            return FALSE;
        }
        // �����߿� ��ų�� �޺��� �������� ����
        if (g_pHero->GetCurState() == STATE::ATTACK && m_QueueAction.ActionID == ACTION_SKILL)
        {
            return FALSE;
        }

        // �����߿� ��ų�� �޺��� �������� ����
        if (g_pHero->GetCurState() == STATE::ATTACK && m_QueueAction.ActionID == ACTION_SPECIAL_MOVE)
        {
            return FALSE;
        }

        // ��Ű�� �� �߿� ��ų�� �޺��� ����������
        if (g_pHero->GetValkyrieShotDelay() > 30 && m_QueueAction.ActionID == ACTION_SKILL)
        {
            return FALSE;
        }

        // ��Ű�� �� �߿� Ư���̵��̸� ��ٸ���.
        if (g_pHero->GetValkyrieShotDelay() > 30 && m_QueueAction.ActionID == ACTION_SPECIAL_MOVE)
        {
            return FALSE;
        }

        
        // �׿ܿ��� ���� ������ ��ȿȭ
        m_QueueAction.ActionID = ACTION_IDLE;
        return FALSE;
    }
}

//------------------------------------------------------------------------------
/**
*/
void HeroActionInput::SetComboDelay(int combodelay)
{
    m_iComboDelay = combodelay;
}

//------------------------------------------------------------------------------
/**
*/
int HeroActionInput::GetComboDelay()
{
    return m_iComboDelay;
}



//------------------------------------------------------------------------------
/**
*/
DWORD HeroActionInput::GetCurrentTarget()
{
    return m_dwSelectedTarget;
}


//------------------------------------------------------------------------------
/**
    ��ǥ ��ġ�� �̵��Ѵ�.
*/
BOOL HeroActionInput::MoveToPosition( const WzVector& DestPosition, bool move_nearest )
{
    if( IsIdleState() || g_pHero->GetCurState() == STATE::MOVE )
    {
        STATE::TYPE eOldNextState = g_pHero->GetNextState();

        if( g_pHero->SetNextState( STATE::MOVE, g_CurTime ) )
        {
			if( CannotMoveState() == FALSE )
			{
                if( g_pHero->Move( g_pHero->GetPosition(), DestPosition, CMS_RUN, FALSE, -1, -1 ) )
                {
                    return TRUE;
                }
                else if( move_nearest == true )
                {
                    // ���� �ִµ� ������ ����
                    int tile_number;
                    WzVector position = DestPosition;
                    if( GameFunc::FindMoveableNearestTile( g_pHero->GetPosition(), position, tile_number ) )
                    {
                        if( g_pHero->Move( g_pHero->GetPosition(), position, CMS_RUN, FALSE, -1, tile_number ) )
                        {
                            return TRUE;
                        }
                    }
                }
            }

            // �̵� ���, ������ ���·� �ǵ�����. Ű���� �̵����̾����� ��� Ű�����̵��� �����ش�.
            if (eOldNextState != STATE::KEYBOARDMOVE)
            {
                g_pHero->SetNextState( STATE::IDLE,g_CurTime );
            }
            else
            {
                g_pHero->SetNextState( eOldNextState,g_CurTime );
            }
        }
    }

    return FALSE;
}


//------------------------------------------------------------------------------
/**
    Queue�� �׼��� ���� �̵��Ѵ�.
*/
BOOL HeroActionInput::MoveToAction(WzVector vDest, bool move_nearest)
{
    // �ڵ��̵������ ����������
    if (!GAMEOPTION->GetAutoMove())
    {
        // �����ൿ�� ���ؼ��� �̵����� �ʴ´�!
        if (m_QueueAction.ActionID == ACTION_ATTACK)
        {
            return FALSE;
        }

    }

    if(!g_pHero)
    {
        return FALSE;
    }

    if( MoveToPosition( vDest, move_nearest ) )
    {
        if (m_QueueAction.ActionID == ACTION_ATTACK && !g_pHero->IsRangedNormalAttack())
        {
            MSG_CG_SYNC_TARGET_MOVE_SYN SendPacket;

            ZeroMemory(&SendPacket,sizeof(SendPacket));
            SendPacket.m_byCategory = CG_SYNC;
            SendPacket.m_byProtocol = CG_SYNC_TARGET_MOVE_SYN;
            SendPacket.m_dwTargetKey = m_QueueAction.ATTACK.dwTargetID;

            if( g_pHero->MakeTargetPathPacket( &SendPacket ) == FALSE)
            {
                assert(!"������ ���� �̵���Ʈ�� ����Ҽ� �����ϴ�");
            }
            else
            {
                g_pHero->SendPacket( &SendPacket, SendPacket.GetPacketSize() );
            }
        }
        else
        {
            // ������ ���� ����� �̵� ��Ŷ ������.
            MSG_CG_SYNC_MOVE_SYN    SendPacket;
            ZeroMemory(&SendPacket,sizeof(SendPacket));
            SendPacket.m_byCategory = CG_SYNC;
            SendPacket.m_byProtocol = CG_SYNC_MOVE_SYN;


            // MakePathPacket�� �����ߴٸ� �ٷ� ���� �����̴� Ư���� �����ʿ����.
            if( g_pHero->MakePathPacket( &SendPacket ) == FALSE)
            {
                assert(!"������ ���� �̵���Ʈ�� ����Ҽ� �����ϴ�");
            }
            else
            {

                SendPacket.Encode();

                g_pHero->SendPacket( &SendPacket, SendPacket.GetPacketSize() );
            }

#ifdef _POSITION_DEBUG
            TCHAR temp[512];
            Sprintf( temp, _T("������ move ��Ŷ ����:%f,%f,%f"),SendPacket.wvCurPos.x, SendPacket.wvCurPos.y, SendPacket.wvCurPos.z);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp);
#endif

        }

        return TRUE;
    }
    else
    {
        m_QueueAction.ActionID = ACTION_IDLE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::IsShootingMode()
{
            
    if (g_pHero && (g_pHero->GetWeaponKind() == eITEMTYPE_ONEHANDCROSSBOW))
    {
        //return TRUE;
    }
    

    return FALSE;  
}

//------------------------------------------------------------------------------
/**
*/
void HeroActionInput::SetCurrentTarget(DWORD dwID,BOOL bCheck)
{
    m_dwSelectedTarget = dwID;
    Object* pObj = g_ObjectManager.GetObject( m_dwSelectedTarget );

    if(pObj && pObj->IsKindOfObject(CHARACTER_OBJECT)) 
    {
        Character *pChar = (Character *)pObj;
        if( pChar )
        {
            m_TargetType = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChar, lsForceAttack() );
        }
        else
        {
            m_TargetType = RELATIONKIND::None;
        }
    }
    else
    {
        m_TargetType = RELATIONKIND::None;
    }

    if( g_pHero )
    {
        g_pHero->m_CurTargetInfo.dwObjectKey = dwID;

        if( IsShootingMode() )
        {
            g_pHero->LockOn( m_dwSelectedTarget );
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::ProcessQueueShot()
{
    Character * pEnemy = (Character *)g_ObjectManager.GetObject(m_QueueAction.VALKYRIESHOT.dwTargetID);

    if (pEnemy) 
    {
        if (!pEnemy->CanBeAttackTarget())
        {
            return FALSE;
        }

        if(!g_pHero)
            return FALSE;

        WzVector wzVec1 = pEnemy->GetPosition() - g_pHero->GetPosition();
        float fLength = VectorLength(&wzVec1);

        if (fLength <= g_pHero->GetAttackRange())
        {
            if (g_pHero->CanDoQueueAction(&m_QueueAction))
            {
                g_pHero->StopMove();
                g_pHero->SetComboCount(m_iComboCount);
                g_pHero->SetTargetID(pEnemy->GetObjectKey());
                g_pHero->SetNextState(STATE::ATTACK,g_CurTime);
                
                m_iComboCount++;

                DWORD dwCurStyle = g_pHero->GetCurrentAttackStyle();

                // �⺻������ ������ 2Ÿ
                if (dwCurStyle == 9999) 
                {
                    if (m_iComboCount >= 2)
                    {
                        m_iComboCount = 0;
                    }
                }
                // ��Ÿ���� ������ 3��Ÿ~
                else
                {
                    if (m_iComboCount >= 3)
                    {
                        m_iComboCount = 0;
                    }
                }
                return TRUE;
            }          
        }
    }
    else
    {
        return FALSE;
    }

    return FALSE;
}

void HeroActionInput::SetCanDoNextAttack(BOOL bFlag)
{
    m_bCanDoNextAttack = bFlag;
    
}
BOOL HeroActionInput::CanDoNextAttack()
{
    return m_bCanDoNextAttack;
}

PLAYER_ACTION HeroActionInput::GetQueueAction()
{
    return m_QueueAction;
}

BOOL HeroActionInput::UseSpecialMoveSkill( DWORD SkillCode, const WzVector& TargetPosition )
{
    if(!g_pHero)
        return FALSE;

    if( CannotMoveState() == TRUE )
    {
        return FALSE;
    }

    if( g_pHero->CanUseSkill( (SLOTCODE)SkillCode, TRUE ) == FALSE )
    {
        return FALSE;
    }

    if( BASE_SKILLINFO* pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)SkillCode) )
    {
        float len = GetSkillRange(pInfo);

        WzVector vDistance = TargetPosition - g_pHero->GetPosition();
        float fDistance = VectorLength(&vDistance);

        if( fDistance > len)
        {
            return FALSE;
        }
        
        PLAYER_ACTION skillaction;
        ZeroMemory(&skillaction,sizeof(skillaction));
        skillaction.ActionID = ACTION_SKILL;
        skillaction.SKILL.dwSkillID = (SLOTCODE)SkillCode;
        skillaction.SKILL.dwTargetID = this->GetCurrentTarget();
        skillaction.SKILL.vTargePos = TargetPosition;

        m_QueueAction = skillaction;

        if (g_pHero->CanDoQueueAction(&m_QueueAction)) 
        {
            // ���������߿��� ����
            if (IsShootingMode())
            {
                if (g_pHero->GetValkyrieShotDelay() > 30)
                {
                    return FALSE;
                }
            }

            g_pHero->DoAction(&m_QueueAction);
        }

        return TRUE;
    }

    return FALSE;
}

BOOL HeroActionInput::UseAreaSkill()
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

    if( g_pHero == NULL )
        return FALSE;

    if( m_IsClickedTargetArea == FALSE )
        return TRUE;

    //if(!m_bPicking)    //��ŷ �Ҷ����� ��ٸ���
    //    return  TRUE;

    BASE_SKILLINFO* pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)m_TargetAreaSkillID);
    if( pInfo == NULL )
    {
        return FALSE;
    }

    // ��ų�� ����� �⺻���� ������ �ȵǸ� ����� ����Ѵ�
    if (!g_pHero->CanUseSkill(m_TargetAreaSkillID,TRUE))
    {
        return FALSE;
    }

    // Ư�� ��ų�� �̵��� �Ұ����ϰų� �̵��� �Ұ��� �� ��ġ�δ� ������� ���Ѵ�
    if( eSKILL_TELEPORT == pInfo->m_SkillClassCode ||
		eSKILL_TELEPORT2 == pInfo->m_SkillClassCode ||
		eSKILL_CREATE_CRYSTAL_WARP == pInfo->m_SkillClassCode ||
		eSKILL_MYSTIC_915 == pInfo->m_SkillClassCode ||	// ������Ʈ��
		eSKILL_MYSTIC_921 == pInfo->m_SkillClassCode // �̽�ƽ��ũ
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        || eSKILL_WITCHBLADE_1124 == pInfo->m_SkillClassCode // ƼŸ�Ͼ�
        || eSKILL_WITCHBLADE_1127 == pInfo->m_SkillClassCode // �꽬
        || eSKILL_WITCHBLADE_1130 == pInfo->m_SkillClassCode // ��������
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		)
    {
		if( CannotMoveState() == TRUE )
        {
            return FALSE;
        }

        if( GameFunc::IsMove(g_pHero->GetPosition(), m_TargetAreaPosition ) == FALSE )
        {
            m_IsClickedTargetArea = FALSE;
            // 70266 : ���� �Ҽ� ���� ���� �Դϴ�.
            g_InterfaceManager.GetInterfaceString( 70266, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            return TRUE;
        }
    }
    else
    {
        // ��� Tile ���  
        int iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( m_TargetAreaPosition, NULL, -1.0f, 10.0f );
        if( iTileFocussing < 0 )
        {
            // ���� �Ҽ� ���� ���� �Դϴ�.
            const int   c_iInvalidArea = 70266;
            g_InterfaceManager.GetInterfaceString( c_iInvalidArea, szMessage, INTERFACE_STRING_LENGTH );
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            m_IsClickedTargetArea = FALSE;

            return TRUE;
        }
    }

    // ��ų �Ÿ��� �˻�
    float len = GetSkillRange(pInfo);

    WzVector vDistance = m_TargetAreaPosition - g_pHero->GetPosition();
    float fDistance = VectorLength(&vDistance);

    if( fDistance > len)
    {
        static BOOL s_flag = TRUE;
        if (GAMEOPTION->GetAutoMove() == TRUE && s_flag == TRUE )
        {
            if( m_iMoveToActionTime<=0)
            {
                if( MoveToAction(m_TargetAreaPosition, false))
                {
                    m_iMoveToActionTime=1000;
                    return TRUE;
                }
                else 
                {
                    // ���� �Ÿ��� ���� �����ϴ�.
                    const int   c_iInvalidDistance = 70267;
                    g_InterfaceManager.GetInterfaceString( c_iInvalidDistance, szMessage, INTERFACE_STRING_LENGTH );
                    g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );
                    return TRUE;

                    //WzVector vSrc=g_pHero->GetPosition();
                    //MoveToAction(pTarget->GetNearTargetPos(vSrc),-1,false);       
                }
            }
        }
        else
        {
            // ���� �Ÿ��� ���� �����ϴ�.
            const int   c_iInvalidDistance = 70267;
            g_InterfaceManager.GetInterfaceString( c_iInvalidDistance, szMessage, INTERFACE_STRING_LENGTH );
            g_InterfaceManager.ShowMessageBox( szMessage, 2000, NULL, 0 );
            return TRUE;
        }

        return TRUE;
    }
    else
    {
        // �����Ÿ� ���� ���� ���

        PLAYER_ACTION skillaction;
        ZeroMemory(&skillaction,sizeof(skillaction));
        skillaction.ActionID = ACTION_SKILL;
        skillaction.SKILL.dwSkillID = m_TargetAreaSkillID;
        skillaction.SKILL.dwTargetID = this->GetCurrentTarget();
        skillaction.SKILL.vTargePos = m_TargetAreaPosition;
        skillaction.SKILL.bForceAttack=lsForceAttack();
        
        m_QueueAction = skillaction;
    }
        
    return TRUE;
}


BOOL HeroActionInput::UseSkill(DWORD skillid,BOOL bForceAttack)
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)skillid);
    assert(pInfo);
    if (!pInfo)
    {
        return FALSE;
    }

    if (g_pHero)
    {
        if (g_pHero->IsLockInput())
        {
            return FALSE;
        }

		if( g_pHero->IsPolymorph() )
		{
			return FALSE;
		}
    }

    // ���� ������ �޸��� ������ ��ų�� ����Ǵ� IDLE ���·� �ٲ�����
    if( m_bAutoRun == TRUE )
    {
        if( g_pHero )
        {
            SetAutoRun(FALSE);
            SetFollowState( FALSE );
            g_pHero->StopMove();
            g_pHero->SendStopMovePacket();
        }
    }

    if(!g_pHero)
        return FALSE;

    switch(pInfo->m_byTarget)
    {
        case SKILL_TARGET_REACHABLE_ENEMY:
        case SKILL_TARGET_REACHABLE_FRIEND:
        case SKILL_TARGET_REACHABLE_ME:
            {
				if( CannotMoveState() == TRUE )
                {
                    return FALSE;
                }
            }
            break;

    }

    // �ڷ���Ʈ�� �̵��Ұ��� ����
    if (pInfo->m_SkillClassCode == eSKILL_TELEPORT||pInfo->m_SkillClassCode == eSKILL_TELEPORT2)
    {
		if( CannotMoveState() == TRUE )
        {
            return FALSE;
        }
        
    }

    DWORD dwTargetID = GetCurrentTarget();

    // Ÿ�� ��������� �Ʊ��ϋ� ���� ��ǥ�� ������ �ڵ����� �ڽſ��� Ÿ��
    if (pInfo->m_byTarget == SKILL_TARGET_FRIEND)
    {
        if (dwTargetID == 0)
        {
            dwTargetID = g_pHero->GetObjectKey();

        }
    }
    else if(pInfo->m_byTarget == SKILL_TARGET_SUMMON && pInfo->m_byTarget == SKILL_TARGET_AREA_ENEMY_CORPSE)
    {
        dwTargetID =0;
    }

    Character *pTarget = NULL;
    pTarget = (Character *)g_ObjectManager.GetObject(dwTargetID);


#ifdef _NA_0_20120612_SKILL_DARK_BREAK_IGNORE_PLAYER_TARGET
    // �÷��̾��� ��쿡�� ��ũ�극��ũ�� Ÿ���� ���� �ʴ´�
    if( pTarget != NULL && skillid == eSKILL_DARK_BREAK )
    {
        if( pTarget->IsKindOfObject(PLAYER_OBJECT) )
        {
            TCHAR szFailMessage[512] = {0,};
            ZeroMemory(&szFailMessage,sizeof(szFailMessage));

            g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NO_TARGET,szFailMessage,MAX_PATH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);

            PlayNoTargetSound();

            return FALSE;
        }
    }
#endif // _NA_0_20120612_SKILL_DARK_BREAK_IGNORE_PLAYER_TARGET

    if (CannotAttack(pTarget))
    {
        return FALSE;
    }


    // ��ų�� ����� �⺻���� ������ �ȵǸ� ����
    if (!g_pHero->CanUseSkill((SLOTCODE)skillid,TRUE))
    {        
        return FALSE;
    }


    // �巡�� Eye���� Nonstop ��ų������� ���������� Action�� ������ �ʴ´� ��Ŷ�� �ٷ�����!
    if (pInfo->IsNonStopSkill())
    {
        MSG_CG_SKILL_ACTION_SYN   Packet;
        Packet.m_byCategory = CG_SKILL;
        Packet.m_byProtocol = CG_SKILL_ACTION_SYN;      
        Packet.m_SkillCode = (SLOTCODE)skillid;
                
        Packet.m_wvCurPos = g_pHero->GetPosition(); 
        Packet.m_wvDestPos = g_pHero->GetPosition();       
        Packet.m_dwClientSerial = g_pHero->GetCurrentAttackSerial();

        Packet.m_byAttackPropensity = bForceAttack;

        DWORD dwAtt=g_pHero->GetCurrentTileAtt(FALSE);

        //�����̰� ���� ���� ������ ����
        if((GameFramework::GetCurrentScene() && 
            GameFramework::GetCurrentScene()->IsEnablePkScene()==FALSE)||!g_isPkServer)
        {
            Packet.m_byAttackPropensity=ATTACK_PROPENSITY_NORMAL;
        }


        // Ÿ�� ����
        if (pInfo->m_byAttRangeform == SKILL_TARGET_NONE)
        {
            
            Packet.m_dwMainTargetKey = g_pHero->GetObjectKey();
            Packet.m_wvMainTargetPos = g_pHero->GetPosition();
        }
        else
        {
            if (pInfo->m_byTarget == SKILL_TARGET_ME)
            {
                Packet.m_dwMainTargetKey = g_pHero->GetObjectKey();
                Packet.m_wvMainTargetPos = g_pHero->GetPosition();
            }
            else if( pInfo->m_byTarget ==SKILL_TARGET_AREA)//���� ���� ��ų�� ��ŷ ��ǥ �־��ش�.
            {
                assert("Target ������ų�� Nonstop��ų�� �ɼ� �����ϴ�!");
                
            }
            else
            {
                
                if (pTarget)
                {

                    Packet.m_dwMainTargetKey = pTarget->GetObjectKey();                 
                    Packet.m_wvMainTargetPos = pTarget->GetPosition();
                }
                else
                {
                    return FALSE;
                }
                            
                if (pInfo->m_byAttRangeform == 5)
                {
                    // �׳� ��������ų�� ��� ��ǥ ��ǥ�� �־��ش�.

                    WzVector vDir = g_pHero->GetDirection();
                    WzVector vDest = g_pHero->GetPosition() + vDir * GetSkillRange(pInfo);

                    Packet.m_wvMainTargetPos = vDest;

                }
            }
        }


        Packet.Encode();

        g_pHero->SendPacket(&Packet,sizeof(MSG_CG_SKILL_ACTION_SYN));  
        return FALSE;
    }


    TCHAR szFailMessage[512] = {0,};
    ZeroMemory(&szFailMessage,sizeof(szFailMessage));

    PLAYER_ACTION skillaction;
    ZeroMemory(&skillaction,sizeof(skillaction));
    skillaction.ActionID = ACTION_SKILL;
    skillaction.SKILL.dwSkillID = (SLOTCODE)skillid;
    skillaction.SKILL.dwTargetID = dwTargetID;
    skillaction.SKILL.bForceAttack=bForceAttack;

    // �ڽ��� Ÿ���� ��ų�̸� �ٷ� ��� �����ϴ�!
    if (pInfo->m_byTarget == SKILL_TARGET_ME||
        pInfo->m_byTarget == SKILL_TARGET_SUMMON||
        pInfo->m_byTarget == SKILL_TARGET_AREA_ENEMY_CORPSE)
    {
        m_QueueAction = skillaction;
        return TRUE;
    }
    else
    {
        if (!pTarget) 
        {              
            g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NO_TARGET,szFailMessage,MAX_PATH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);

            PlayNoTargetSound();
            
            return FALSE;
        }
   
        
        // ��ǥ�� �������� ������ ����
        int iResult = 0; //�ֽ����ߴ��� ��ü������ ������Ѻ���
        if (!CanBeSkillTarget(skillid,pTarget,bForceAttack,iResult))
        {              
            //������Ǿȵ������� default������
            int iStringCode = ((iResult == 0) ? (eST_SKILL_FAIL_NOT_PROPER_TARGET):(iResult));
            g_InterfaceManager.GetInterfaceString(iStringCode,szFailMessage,MAX_PATH);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);  

            
            return FALSE;
        }

        m_QueueAction = skillaction;

        WzVector vDest = pTarget->GetPosition();
        WzVector vDistance = vDest - g_pHero->GetPosition();

        float fDistance = VectorLength(&vDistance);



        // �Ÿ��� �ʹ��ָ� �̵�~

        if( fDistance > GetActionLength(m_QueueAction) )
        {
            if (GAMEOPTION->GetAutoMove())
            {
                if(m_iMoveToActionTime<=0)
                {
                    if(MoveToAction(vDest, false))
                    {
                        m_iMoveToActionTime=1000;
                    }
                    else 
                    {
                        WzVector vSrc=g_pHero->GetPosition();
                        MoveToAction(pTarget->GetNearTargetPos(vSrc), false);       
                    }
                }
            }
            else
            {
                // �Ÿ��� �ʹ� �ٴϴ�.
                g_InterfaceManager.GetInterfaceString(eST_TOO_FAR,szMessage,INTERFACE_STRING_LENGTH);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );

                PlayTargetTooFarSound();
                return FALSE;

            }
        
        }
        return TRUE;    
        
    }

    return FALSE;
}

BOOL    HeroActionInput::CanBeSkillTarget(DWORD dwSkillID,Character *pTarget,BOOL bForceAttack,int &iResult)
{

    if (!pTarget)
    {
        return FALSE;
    }

    BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillID);
    assert(pInfo);
    if (!pInfo)
    {
        return FALSE;
    }

    switch(pInfo->m_byTarget)
    {
        case SKILL_TARGET_ENEMY_PLAYER:  // �� �÷��̾ ���� ����
            {
                // �Ϲ����� ��ų�� ��ǥ�� �ɼ��ִ��� Ȯ���Ѵ�.
                if (!pTarget->CanBeAttackTarget())
                {
                    return FALSE;
                }

                if( pTarget->IsKindOfObject(PLAYER_OBJECT) )
                {
                    RELATIONKIND::TYPE type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTarget, bForceAttack );
                    if (type == RELATIONKIND::Enemy)
                    {
                        return TRUE;
                    }
                    else
                    {
                        return FALSE;
                    }
                }
                else
                {
                    return FALSE;
                }
            }


        case SKILL_TARGET_ENEMY    :
        case SKILL_TARGET_REACHABLE_ENEMY  :
        case SKILL_TARGET_ENEMY_AND_ME:
            {
                // �Ϲ����� ��ų�� ��ǥ�� �ɼ��ִ��� Ȯ���Ѵ�.
                if (!pTarget->CanBeAttackTarget())
                {
                    return FALSE;
                }

                // ���� ��쿡�� ����
                RELATIONKIND::TYPE type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTarget, bForceAttack );
                if (type == RELATIONKIND::Enemy)
                {
                    return TRUE;
                }
                else
                {
                    return FALSE;
                }



            }
            break;
        case SKILL_TARGET_FRIEND   :
        case SKILL_TARGET_REACHABLE_FRIEND :
            {
                // �Ϲ����� ��ų�� ��ǥ�� �ɼ��ִ��� Ȯ���Ѵ�.
                if (!pTarget->CanBeAttackTarget())
                {
                    return FALSE;
                }

                // �Ʊ��� ��쿡�� ����
                RELATIONKIND::TYPE type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTarget, bForceAttack );
                if (type == RELATIONKIND::Friend)
                {
                    if(pTarget->IsKindOfObject(MONSTER_OBJECT))
                    {
                        Monster* pMon = (Monster*)pTarget;
                        if( MONSTER_TYPE_SETTING_MAPOBJECT == pMon->GetMonsterType() )
                            return FALSE;
                        else
                            return TRUE;
                    }
                    else
                    {
                        return TRUE;
                    }
                }
                else
                {
                    return FALSE;
                }
            }
            break;       
        case SKILL_TARGET_FRIEND_CORPSE :
            {
                // ��ü�� �ƴϸ� ����
                if (!pTarget->IsDead())
                {
                    return FALSE;
                }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL

                // ��Ƽ���� �ƴϸ� ����
                uiPartyMan* partyMan  = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
                if (partyMan->IsPartyMember(pTarget->GetObjectKey()) == FALSE )
                {
                    return FALSE;
                }

                // �Ʊ��÷��̾ �ƴϸ� ����
                RELATIONKIND::TYPE type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTarget, bForceAttack );
                if (type != RELATIONKIND::Friend)
                {
                    return FALSE;
                }
#else
                // ��Ƽ���� �ƴϸ� ����
                uiPartyMan* partyMan  = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
                if (partyMan->IsPartyMember(pTarget->GetObjectKey()) == FALSE )
                {
                    return FALSE;
                }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

                return TRUE;
            }
            break;       
        case SKILL_TARGET_ME       :
        case SKILL_TARGET_REACHABLE_ME:
        case SKILL_TARGET_AREA     :
            {
                assert(!"CanBeSkillTarget() �ڽ� �Ǵ� ������ ��ǥ�� ��� ��ų�� �� �Լ��� ����ϸ�ȵ�!");
                return FALSE;
            }
            break;

        case SKILL_TARGET_ENEMY_CORPSE:
            {
                // ������ü�� �ƴϸ� ����
                if (!pTarget->IsDead())
                {
                    return FALSE;
                }
                RELATIONKIND::TYPE type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pTarget, bForceAttack );
                if(type == RELATIONKIND::Enemy)
                {
                    //�ҿ���Ʈ�ѿ��ѿ����� ó��
                    //���Ĵٸ���ų�� SKILL_TARGET_ENEMY_CORPSE���� ���δٸ� �̰����� ó���ϼ���
                    return CheckSoulControl(pTarget,dwSkillID ,iResult);
                }  
                else
                {
                    return FALSE;
                }
            }


        default:
            {
                assert(!"CanBeSkillTarget() ó������ ���� ��ǥ���� ���");
                return FALSE;
            }
            break;

    }

    return FALSE;

}

//------------------------------------------------------------------------------
/** �ҿ���Ʈ�� ����üũ(Ŭ�󿡼� �̸�üũ�ؼ� ������)
    iResult���� ���п����� ��Ʈ���ڵ带 �������ش�.
*/
BOOL HeroActionInput::CheckSoulControl(Character* pTarget,DWORD dwSkillID,int &iResult)
{
    if(!pTarget->IsKindOfObject(MONSTER_OBJECT))
    {
        return FALSE;
    }
    Monster *pMon = static_cast<Monster*>(pTarget);
        
    BYTE byGrade = pMon->GetMonsterInfo()->m_byGrade;
    
    //������, ��Ű, ĸƾ�� ��ȯ�� �Ұ��� �ϴ�.
#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    if (byGrade == 9 || byGrade == 7 || byGrade == 8)
#else
    if( byGrade == 5 || byGrade == 9 || byGrade == 7 || byGrade == 4 || byGrade == 8)
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    {
        //5631:�ش���ʹ� ��ȯ�Ҽ������ϴ�.
        iResult = 5631;
        return FALSE;
    }
    
    SkillScriptInfo* skill_info = (SkillScriptInfo*)SkillInfoParser::Instance()->GetSkillInfo((WORD)dwSkillID); 

    if (!skill_info)
    {
        return FALSE; 
    }

    BASE_ABILITYINFO* skill_ablity = skill_info->GetAbilityInfo( eABILITY_SUMMON_DEAD_MONSTER ); 

    if ((skill_ablity) && (g_pHero->GetSummonSize() >= skill_ablity->m_iParam[0])) 
    {
        //5632:���̻� ��ȯ�Ҽ������ϴ�. 
        iResult = 5632; 
        return FALSE; 
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**

    INPUT ASSIST TARGET


    * �켱 ���� ��� ���� (Primary Targetting)
        - Shift + �� Ŭ�� : ��� ���� + ��Ƽ���� ��븦 ���� �̵��Ͽ� ����
        - Shift + �� Ŭ�� : ��� ���� + ��Ƽ���� ��� Ÿ����
    
    * ���� Ÿ����
        - ��ý�Ʈ ��ư (�Ǵ� ctrl + tab)�� Ŭ���ϸ� ��Ƽ���� ������ ��� �Ǵ� �ٸ� ��Ƽ���� ���� (Ÿ����) ���� ����� �ڵ����� Ÿ�����Ѵ�.

    * ��Ƽ�� ���� ��� Ÿ���� ('��' �� ����) (Tanker assist)
        - ��ý�Ʈ ��ư (shift + tab)�� Ŭ���ϸ� ��Ƽ���� '�켱 ���� ��� ���� - ��Ƽ�� ����' ����� ����Ͽ� ������ ���� Ÿ�����Ѵ�.
        - ��Ƽ���� ������ ���� ���� ��쿡�� ����� �������� �ʴ´�. = Ÿ���� �ǰų� �������� �ʴ´�.


*/
#define INP_AT_PRIMARY_TARGETTING_TYPE_A    (g_Input.GetState(DIK_LSHIFT, KS_DOWN) && g_Input.GetState(MOUSE_LBUTTON, KS_MOUSE_DOWN))
#define INP_AT_PRIMARY_TARGETTING_TYPE_B    (g_Input.GetState(DIK_LSHIFT, KS_DOWN) && g_Input.GetState(MOUSE_RBUTTON, KS_MOUSE_DOWN))
#define INP_AT_FREE_TARGETTING              (g_Input.GetState(DIK_LCONTROL, KS_DOWN) && g_Input.GetState(DIK_TAB, KS_DOWN) )
#define INP_AT_TANKER_ASSIST             (g_Input.GetState(DIK_LSHIFT, KS_DOWN) && g_Input.GetState(DIK_TAB, KS_DOWN) )

void
HeroActionInput::ProcessAssistTarget()
{
}
//------------------------------------------------------------------------------
/**
    �⺻���� �ɼ�Ű��
*/
void HeroActionInput::ProcessBaseOptionKey()
{
    //����Ű ����â(HŰ)
    if (g_Input.IsPressAnyExtKey() == FALSE && g_Input.GetState(DIK_H, KS_DOWN))
    {
#ifdef MAP_EDIT_MODE
        // no operation
#else

#endif   
    }

    // �̱۸��� ���� �̵� �� ��ų ���� ���
    if ((GENERALPARAM->IsNet()==FALSE) && GENERALPARAM->GetSpecialMode())
    {
        if( g_Input.IsPressAnyExtKey() == FALSE  && g_Input.GetState(DIK_P, KS_DOWN ))
        {
            static BOOL bHelpToggle = FALSE;

            bHelpToggle ^= TRUE;

            uiHelpMan* helpMan = static_cast<uiHelpMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_HELP));

            if(helpMan)
                helpMan->ShowHelpMonsterInfo(bHelpToggle);
        }
    }
}
//------------------------------------------------------------------------------
/**
    �ɼ�Ű ó��
*/
void HeroActionInput::ProcessOptionKey()
{
    
    if(!g_pHero)return;

    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};

//#ifdef _DH_BATTLEZONE2_
//#else
//#ifndef _JAPAN
//    if (g_Input.GetState(DIK_F11, KS_DOWN) && g_Input.IsPressAnyExtKey() == FALSE)
//    {
//        m_bIgnorePlayer ^= TRUE;
//    }
//#endif
//#endif//_DH_BATTLEZONE2_

    if( g_Input.GetState( DIK_RALT, KS_UP ) )
    {
        if ( GAMEOPTION->GetNotifyDropItem() )
        {
            g_InterfaceManager.SetDiplayDropItem( TRUE ); 
        }
        else
        {
            g_InterfaceManager.SetShowItemName(!g_InterfaceManager.ShowItemName());
        }

    }

    ProcessBaseOptionKey();

}

void HeroActionInput::ToggleChangeSlotSize()
{
    uiQuickContainerMan* pContainerMan = 
        static_cast<uiQuickContainerMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUICKCONTAINER));
    assert(pContainerMan);
    pContainerMan->ToggleChangeSlotSize();
}

void HeroActionInput::PressQuickSlotKey()
{
    uiQuickContainerMan* pContainerMan = 
        static_cast<uiQuickContainerMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_QUICKCONTAINER));
    assert(pContainerMan);

    if (pContainerMan)
    {
        pContainerMan->ToggleSlot();
    }
}


float HeroActionInput::GetAttackActionLength()
{

    // �̲����� ���� �Ÿ�
    const float SWIPE_RANGE = 0.2f;
    // �ٰŸ������ ���Ÿ� ���⸦ �����Ѵ�!!!
    // �ٰŸ�
    if (!g_pHero->IsRangedNormalAttack())
    {
        return g_pHero->GetAttackRange() + SWIPE_RANGE;
    }
    else
    {
        return g_pHero->GetAttackRange();
    }
    

}

void HeroActionInput::ProcessComboDelay(DWORD dwTick)
{
    if (m_iComboDelay > 0)
    {
        if(g_pHero)
        {
            if (g_pHero->GetCurState() != STATE::ATTACK)
            {
                m_iComboDelay -= dwTick;
            }
            if (m_iComboDelay <= 0)
            {
                m_iComboDelay = 0;
                m_iComboCount = 0;
                g_pHero->SetComboCount(0);
            }
        }
    }

    if (m_iKeyboardQueueAttackDelay > 0)
    {
        m_iKeyboardQueueAttackDelay -= dwTick;

        if (m_iKeyboardQueueAttackDelay <= 0)
        {
            m_iKeyboardQueueAttackDelay = 0;
            m_bKeyboardQueueAttack = FALSE;
        }
    }
}

void HeroActionInput::ProcessTargeting()
{
    // Ÿ���� �������� �ʰų� �׾����� Ǯ����.
    if ( GetCurrentTarget())
    {
        Character *pTarget = (Character *)g_ObjectManager.GetObject( this->GetCurrentTarget());
        if (!pTarget)
        {
            uiPartyMan* partyMan  = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );
            assert(partyMan);

            if(partyMan->IsPartyMember(this->GetCurrentTarget())==FALSE)
                SetCurrentTarget(0);
        }
        else 
        {
            if (g_pHero->IsBlind())
            {
                SetCurrentTarget(0);
            }

            if (pTarget->IsKindOfObject(MONSTER_OBJECT)) 
            {
                Monster* pMonster = (Monster*)pTarget;

                if (pTarget->GetCurState() == STATE::DEATH)
                {
                    bool can_target_death_monster = false;

                    if( g_pHero->GetClass() == eCHAR_SHADOW )
                    {
                        if( SkillData* pSkillData = xGetSkillData()->GetSkillDataBySkillClassCode( eSKILL_SOUL_CONTROL ) )
                        {
                            if( pSkillData->IsActive() == true )
                            {
                                can_target_death_monster = true;
                            }
                        }
                    }

                    if( can_target_death_monster == false )
                    {
                        SetCurrentTarget(0);
                    }

                    ////������� ��ü�� Ÿ�����Ҽ��ִ�
                    //if (g_pHero->GetClass() != eCHAR_SHADOW)
                    //{
                    //    SetCurrentTarget(0);
                    //}

                    if (eNPC_DOMINATION_MAPOBJECT_NPC == pMonster->GetMonsterInfo()->m_byGrade)
                    {
                        SetCurrentTarget(0);
                    }
            
                }

                if (pMonster->GetMonsterInfo() && (pMonster->GetMonsterInfo()->m_byGrade == 10))
                {
                    SetCurrentTarget(0);
                }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                if (pMonster->IsWitchBladeSummon() == TRUE)
                {
                    SetCurrentTarget(0);
                }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

            }
            else if (pTarget->IsKindOfObject(PLAYER_OBJECT))
            {
                //���̵�Ȱ��� Ÿ���Ҽ�����.
                if (pTarget->GetHideSkill() == HIDE_SKILL_INFO::ALL_HIDE)
                    SetCurrentTarget(0);
            }

        }
    }

    if (GetCurrentTarget())
    {
        Object * pObj = g_ObjectManager.GetObject( GetCurrentTarget() );

        BOOL bShow = FALSE;

        if(pObj)
        {
            if( pObj->IsKindOfObject( PLAYER_OBJECT ) || pObj->IsKindOfObject( MONSTER_OBJECT ) )
            {
                bShow = TRUE;
            }
        }
        else
        {
            uiPartyMan* partyMan  = GET_DIALOG_MANAGER( uiPartyMan, UIMAN_PARTY );
            assert(partyMan);
            //��Ƽ���� ������Ʈ�� ��� ���â�� ���� Ÿ�����Ҽ��ִ�   
            if(partyMan->IsPartyMember(this->GetCurrentTarget()))
            {
                bShow = TRUE;
            }
        }

        if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ)
        {
            bShow = FALSE;
        }

        //Ÿ����â �����ֱ�
        if (bShow)
        {
            TargetDialog* pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);

            if (pTargetDlg)
            {
                if (pTargetDlg->GetTargetKey() != GetCurrentTarget())
                {
                    pTargetDlg->SetTargetKey(this->GetCurrentTarget());
                    
                    //��������̺�Ʈ������ Ÿ��â��������!
                    if (g_pMap->IsSnowManEventMap())
                    {
                        bShow = FALSE;
                    }

                    pTargetDlg->ShowWindow(bShow);      
    
                }
            }
        }
    }
    else
    {
        TargetDialog* pTargetDlg = GET_CAST_DIALOG(TargetDialog, IM_TARGET_MANAGER::TARGET_DIALOG);
        if (pTargetDlg)
        {
            SetAutoAttack(FALSE);
            
            pTargetDlg->SetTargetKey(0);

            if (pTargetDlg->IsVisibleWindow())
            {
                pTargetDlg->ShowWindow(FALSE);
            }
        }
    }
}


//----------------------------------------------------------------------------
/**
*/
void HeroActionInput::ProcessCursor()
{
    if ( IsAttackSkillArea())
    {
        CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Magic_Scope );
        return;
    }

    if ( !g_Input.GetInterfaceInputMouseFlag() )
    {
        if( Object* pPickedObject = GetPickedObject() )
        {
            BOOL bRet = GameFunc::UpdateCursorByPickedObject( pPickedObject );
            if ( bRet )
            {
                return;
            }
            else
            {
                CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
            }
        }
        else
        {
            if ( CanMoveToPickedPos() == FALSE )
            {
                CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_DO_NOT_GO_TILE );
            }
            else
            {
                CursorChangeTransact::Instance()->ChangeTranaction( MouseCursor::eMouseCursor_Cursor_Cancle );
            }
        }
    }
}

void HeroActionInput::SetEnableInput(BOOL bFlag)
{
    this->m_bEnableInput = bFlag;
}

BOOL HeroActionInput::IsEnableInput()
{
    return this->m_bEnableInput;
}

void HeroActionInput::ValkyrieShot()
{
    if (g_pHero) 
    {
        if(g_pHero->GetCurState()!=STATE::SPECIALMOVE
            && g_pHero->GetNextState()!=STATE::SPECIALMOVE
            && g_SkillQueue.Size()==0)
        {
            if (g_pHero->GetBulletCount() > 0)
            {
                g_pHero->DecreaseBulletCount();

                MSG_CG_BATTLE_VKR_ATTACK_SYN packet;
                packet.dwTargetKey =  this->GetCurrentTarget();
                packet.dwClientSerial = g_pHero->GetNextAttackSerial();
                g_pHero->SendPacket(&packet,sizeof(packet));
                g_pHero->AddNextAttackSerial();
                g_pHero->ValkyrieShot(packet.dwClientSerial);
            }
            // ���ε��ϻ�
            else
            {
                ValkyrieReload();         

            }
        }
    }
}

void HeroActionInput::ValkyrieReload()
{
    if (!IsShootingMode())
    {
        return;
    }

    if(!g_pHero)
        return;

    BOOL  PlayAnimation = TRUE;
    switch(g_pHero->GetCurState())
    {
    case STATE::SIT:
    case STATE::EMOTICON:
        PlayAnimation = FALSE;
        break;

    case STATE::MOVE:
    case STATE::IDLE:
    case STATE::KEYBOARDMOVE:
        PlayAnimation = TRUE;
        break;

    default:
        return;
        break;
    }

    if (g_pHero->GetValkyrieShotDelay() > (VALKYRIE_SHOT_DELAY_MARGIN - 5))
    {
        return;
    }

    MSG_CG_BATTLE_VKR_RELOAD_SYN packet;
    g_pHero->SendPacket(&packet,sizeof(packet));

    g_pHero->ValkyrieReload( PlayAnimation );
    g_pHero->SetBulletCount(20);

}

BOOL HeroActionInput::IsAutoAttackMode( void )
{
    return m_bAutoAttack;
}

DWORD HeroActionInput::FindNearestTarget()
{
    DWORD dwTarget = 0;

    DWORD dwPickingOption = PICKING_IGNORE_CORPSE;

//#ifdef _DH_BATTLEZONE2_
//    m_bIgnorePlayer = (CGameOption::Instance()->GetPickingFriend() == FALSE);
//#endif //_DH_BATTLEZONE2_
    BOOL IgnorePlayer = (CGameOption::Instance()->GetPickingFriend() == FALSE);
    if (IgnorePlayer)
    {
        dwPickingOption |= PICKING_IGNORE_FRIEND;
    }

    WzRay ray;
    ray.m_wvOrigin = g_pHero->GetVisiblePos();
    ray.m_wvOrigin.z += 1;
    ray.m_wvDirection = g_pHero->GetDirection();

    if (g_pMap)
    {
        Character *pChr = g_pMap->GetPickedCharacter(&ray,dwPickingOption);

        if (pChr)
        {
            dwTarget = pChr->GetObjectKey();
        }
    }

    return dwTarget;
}

BOOL HeroActionInput::AttackNearestTarget()
{
    if(!g_pHero)
        return FALSE;

    if( g_pHero->IsDead() && m_bAutoAttack )
    {      
        return FALSE;
    }
    
    Object *pTarget =g_ObjectManager.GetObject(FindNearestTarget());


    if (CannotAttack(pTarget))
    {
        return FALSE;
    }

    switch(g_pHero->GetCurState())
    {
    case STATE::IDLE:
    case STATE::SIT:
    case STATE::EMOTICON:
    case STATE::MOVE:
    case STATE::KEYBOARDMOVE:
        break;
    case STATE::ATTACK:
        {
            m_bKeyboardQueueAttack = TRUE;
            m_iKeyboardQueueAttackDelay = 100;
            return FALSE;
        }
        break;
    default:
        return FALSE;
        break;

    }
    
    if ( m_ShowTargetAreaPointer == FALSE )
    {
        PLAYER_ACTION action;
        ZeroMemory(&action,sizeof(action));
        
        DWORD dwTarget = FindNearestTarget();                         
        if( dwTarget )
        {
            action.ActionID = ACTION_ATTACK;
            action.ATTACK.dwTargetID = dwTarget;
            action.ATTACK.bForceAttack =lsForceAttack();
            m_QueueAction = action;     
            m_bKeyboardQueueAttack = FALSE;

            Character *pTarget = (Character *)g_ObjectManager.GetObject(dwTarget);
            if(pTarget)
            {
                WzVector vTarget = pTarget->GetPosition();
                WzVector vOffset = g_pHero->GetPosition() - vTarget;
                float fDistance = VectorLength( &vOffset );

                // �Ÿ��� �ʹ��ָ� �̵�~
                if( fDistance > GetActionLength(m_QueueAction) )
                {
                    MoveToAction(vTarget, false);                    
                }

                return TRUE;
            }
        }                  
    }

    return FALSE;
}

void HeroActionInput::ProcessObserverModeKey(DWORD dwTick)
{
    using namespace nsSSQ;
    using namespace nsSSQ::Observer;

    if (g_bObserverMode == FALSE)
    {
        return;
    }

    static DWORD Tick = 0;
    static BOOL bCheck   = FALSE;
    BOOL   bPress       = FALSE;

    Tick += dwTick;

    if (bCheck && (Tick >= 3000))
    {
        Tick = 0;
        bCheck = FALSE;
    }

    if(bCheck == TRUE)
    {
        return;
    }

    if (g_Input.GetState(DIK_E, KS_UP))
    {
        g_ObserverKey = Info::eKeyCmd_E;
        bPress = TRUE;
    }
    else if (g_Input.GetState(DIK_Q, KS_UP))
    {
        g_ObserverKey = Info::eKeyCmd_Q;
        bPress = TRUE;

    }
    else if (g_Input.GetState(DIK_W, KS_UP))
    {
        g_ObserverKey = Info::eKeyCmd_W;
        bPress = TRUE;
    }

    if (bPress)
    {      
        StartLeaveFieldCastTimer(eDoType_ObserverMode_ChangePos, 3000);  
        bCheck = TRUE;
        Tick = 0;
    }
}




void    HeroActionInput::ProcessKeyboardAttack()
{
    if (GENERALPARAM->GetSpecialMode() == eSPECIAL_MODE_STEP3)
    {
        if( IsShootingMode() == FALSE )
        {
            if( m_bKeyboardQueueAttack )
            {
                if (AttackNearestTarget())
                {
                    return;
                }
            }

            if (g_Input.GetState(DIK_SPACE, KS_DOWN ))
            {
                if (AttackNearestTarget())
                {
                    return;
                }
            }
        }
    }
}

void    HeroActionInput::PlayLimitUseSound()
{
    if( GAMEOPTION->GetSystemVoice() == FALSE )
    {
        return;
    }

    // �ش������� �����̰� ��ӵǰ������� �÷�����������
    if (m_dwUIVoice == UI_VOICE_LIMIT_USE)
    {
        return;
    }

    m_dwUIVoice = UI_VOICE_LIMIT_USE;
    m_iUIVoiceDelay = UI_VOICE_DELAY;

    DWORD dwClass = 0;
    if (g_pHero != NULL)
    {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        dwClass = HeroVoiceInfoParser::GetVoiceInfoKeyCode(g_pHero->GetClass(), g_pHero->GetGender());
#else
        dwClass = g_pHero->GetClass();
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }

    BASE_HeroVoiceInfo *pInfo = HeroVoiceInfoParser::Instance()->GetHeroVoiceInfo(dwClass);

    if (pInfo)
    {
        g_SoundEffect.PlayUI(pInfo->m_LimitUse.dwVoiceID);
    }
}
void    HeroActionInput::PlayNoTargetSound()
{
    if( GAMEOPTION->GetSystemVoice() == FALSE )
    {
        return;
    }

    if (m_dwUIVoice == UI_VOICE_NO_TARGET)
    {
        return;
    }

    m_dwUIVoice = UI_VOICE_NO_TARGET;
    m_iUIVoiceDelay = UI_VOICE_DELAY;
    
    DWORD dwClass = 0;
    if (g_pHero)
    {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        dwClass = HeroVoiceInfoParser::GetVoiceInfoKeyCode(g_pHero->GetClass(), g_pHero->GetGender());
#else
        dwClass = g_pHero->GetClass();
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }
    
    BASE_HeroVoiceInfo *pInfo = HeroVoiceInfoParser::Instance()->GetHeroVoiceInfo(dwClass);

    if (pInfo)
    {
        if (rand() & 1)
        {
            g_SoundEffect.PlayUI(pInfo->m_NoTarget1.dwVoiceID);
        }
        else
        {
            g_SoundEffect.PlayUI(pInfo->m_NoTarget1.dwVoiceID);
        }
    }
}

void    HeroActionInput::PlayTargetTooFarSound()
{
    if( GAMEOPTION->GetSystemVoice() == FALSE )
    {
        return;
    }

    if (m_dwUIVoice == UI_VOICE_TARGET_TOO_FAR)
    {
        return;
    }

    m_dwUIVoice = UI_VOICE_TARGET_TOO_FAR;
    m_iUIVoiceDelay = UI_VOICE_DELAY;

    DWORD dwClass = 0;
    if (g_pHero)
    {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        dwClass = HeroVoiceInfoParser::GetVoiceInfoKeyCode(g_pHero->GetClass(), g_pHero->GetGender());
#else
        dwClass = g_pHero->GetClass();
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }

    BASE_HeroVoiceInfo *pInfo = HeroVoiceInfoParser::Instance()->GetHeroVoiceInfo(dwClass);

    if (pInfo)
    {
        g_SoundEffect.PlayUI(pInfo->m_FarTarget1.dwVoiceID);
    }
}

void    HeroActionInput::PlayNotEnoughMoneySound()
{
    if( GAMEOPTION->GetSystemVoice() == FALSE )
    {
        return;
    }

    if (m_dwUIVoice == UI_VOICE_NOT_ENOUGH_MONEY)
    {
        return;
    }

    m_dwUIVoice = UI_VOICE_NOT_ENOUGH_MONEY;
    m_iUIVoiceDelay = UI_VOICE_DELAY;

    DWORD dwClass = 0;
    if (g_pHero)
    {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        dwClass = HeroVoiceInfoParser::GetVoiceInfoKeyCode(g_pHero->GetClass(), g_pHero->GetGender());
#else
        dwClass = g_pHero->GetClass();
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }
    
    BASE_HeroVoiceInfo *pInfo = HeroVoiceInfoParser::Instance()->GetHeroVoiceInfo(dwClass);

    if (pInfo)
    {
        g_SoundEffect.PlayUI(pInfo->m_LackMon.dwVoiceID);
    }
}

void HeroActionInput::PlayNotEnoughManaSound()
{
    if( GAMEOPTION->GetSystemVoice() == FALSE )
    {
        return;
    }

    if (m_dwUIVoice == UI_VOICE_NOT_ENOUGH_MANA)
    {
        return;
    }

    m_dwUIVoice = UI_VOICE_NOT_ENOUGH_MANA;
    m_iUIVoiceDelay = UI_VOICE_DELAY;

    DWORD dwClass = 0;
    if (g_pHero)
    {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        dwClass = HeroVoiceInfoParser::GetVoiceInfoKeyCode(g_pHero->GetClass(), g_pHero->GetGender());
#else
        dwClass = g_pHero->GetClass();
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }
    
    BASE_HeroVoiceInfo *pInfo = 
        HeroVoiceInfoParser::Instance()->GetHeroVoiceInfo(dwClass);
    if (pInfo)
    {
        switch (dwClass)
        {
            case eCHAR_BERSERKER:
            case eCHAR_DRAGON:
                g_SoundEffect.PlayUI(pInfo->m_LackSP.dwVoiceID);
                break;

            default:
                g_SoundEffect.PlayUI(pInfo->m_LackMP.dwVoiceID);
                break;
        }
    }
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
void HeroActionInput::PlayNotEnoughFPSound()
{
    if( GAMEOPTION->GetSystemVoice() == FALSE )
    {
        return;
    }

    if (m_dwUIVoice == UI_VOICE_NOT_ENOUGH_FP )
    {
        return;
    }

    m_dwUIVoice = UI_VOICE_NOT_ENOUGH_FP;
    m_iUIVoiceDelay = UI_VOICE_DELAY;

    DWORD dwClass = 0;
    if (g_pHero)
    {
        dwClass = HeroVoiceInfoParser::GetVoiceInfoKeyCode(g_pHero->GetClass(), g_pHero->GetGender());
    }

    BASE_HeroVoiceInfo *pInfo = 
        HeroVoiceInfoParser::Instance()->GetHeroVoiceInfo(dwClass);
    if (pInfo)
    {
        g_SoundEffect.PlayUI(pInfo->lack_fp_.dwVoiceID);
    }
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

void HeroActionInput::PlayNotEnoughPointSound()
{
    if( GAMEOPTION->GetSystemVoice() == FALSE )
    {
        return;
    }

    if (m_dwUIVoice == UI_VOICE_NOT_ENOUGH_POINT )
    {
        return;
    }

    m_dwUIVoice = UI_VOICE_NOT_ENOUGH_POINT;
    m_iUIVoiceDelay = UI_VOICE_DELAY;

    DWORD dwClass = 0;
    if (g_pHero)
    {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        dwClass = HeroVoiceInfoParser::GetVoiceInfoKeyCode(g_pHero->GetClass(), g_pHero->GetGender());
#else
        dwClass = g_pHero->GetClass();
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }
    
    BASE_HeroVoiceInfo *pInfo = 
        HeroVoiceInfoParser::Instance()->GetHeroVoiceInfo(dwClass);
    if (pInfo)
    {
        g_SoundEffect.PlayUI(pInfo->m_LackPoint.dwVoiceID);
    }
}

void    HeroActionInput::PlayNoRepairableSound()
{
    if( GAMEOPTION->GetSystemVoice() == FALSE )
    {
        return;
    }

    if (m_dwUIVoice == UI_VOICE_NO_REPAIRABLE)
    {
        return;
    }

    m_dwUIVoice = UI_VOICE_NO_REPAIRABLE;
    m_iUIVoiceDelay = UI_VOICE_DELAY;

    DWORD dwClass = 0;
    if (g_pHero)
    {
#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
        dwClass = HeroVoiceInfoParser::GetVoiceInfoKeyCode(g_pHero->GetClass(), g_pHero->GetGender());
#else
        dwClass = g_pHero->GetClass();
#endif //_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
    }
    
    BASE_HeroVoiceInfo *pInfo = 
        HeroVoiceInfoParser::Instance()->GetHeroVoiceInfo(dwClass);
    if (pInfo)
    {
        g_SoundEffect.PlayUI(pInfo->m_NOREPItem.dwVoiceID);
    }
}

void HeroActionInput::ProcessVoiceDelay(DWORD dwTick)
{
    if (m_iUIVoiceDelay > 0)
    {
        m_iUIVoiceDelay -= dwTick;
        if (m_iUIVoiceDelay <= 0)
        {
            m_iUIVoiceDelay = 0;
            m_dwUIVoice = 0;
        }
    }
}

BOOL HeroActionInput::CannotAttack(Object* pTargetObject)
{
    if( g_pHero == NULL )
        return TRUE;

    if( g_pHero->IsImpossible() )
        return TRUE;

    if( pTargetObject != NULL )
    {
        if( pTargetObject->IsKindOfObject(MONSTER_OBJECT) )
        {
            Monster* pMonster = (Monster*)pTargetObject;
            if(MONSTER_TYPE_SETTING_MAPOBJECT == pMonster->GetMonsterType()) //��ġ�� ���Ͷ��
            {
                Object* pTemp = g_ObjectManager.GetObject(MAP_OBJECT_KEY + pMonster->GetMapObjectKey());
                if(pTemp && pTemp->IsKindOfObject(MAP_OBJECT))
                {
                    //�ش� ��ġ�� ���Ϳ� �����ִ� �ʿ�����Ʈ�� ������ ���� ���¶��
                    MapObject* pMapObj = (MapObject*)pTemp;
                    if(pMapObj->GetbCanHit() == false )
                        return TRUE;
                }
            }
        }
        else if( pTargetObject->IsKindOfObject(PLAYER_OBJECT) == FALSE )
        {
            return FALSE;
        }
    }

    return m_bCannotAttack;
}

bool HeroActionInput::CanAttackInDominationWar()
{
    Object* pObj = g_ObjectManager.GetObject(GetCurrentTarget());
    if(NULL == pObj)
        return true;

    if (SCENE_TYPE_DOMINATION == GameFramework::GetCurrentScene()->GetType()
#ifdef _NA_008334_20150608_SONNENSCHEIN
        || SCENE_TYPE_SONNENSCHEIN == GameFramework::GetCurrentScene()->GetType()
#endif //_NA_008334_20150608_SONNENSCHEIN
        )
    {
        if(STATE::KEYBOARDJUMP == g_pHero->GetCurState())
            return false;

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        const sDOMINATION_INFO* domination_info =
            DominationInfoParser::Instance()->GetDominationInfo(g_pMap->GetCurrentFieldID());
        if (domination_info && domination_info->m_DominationType == DOMINATION_TYPE_MONDSCHIEN)
        {
            // ��Ʈ���� �� ��쿡��
            if(TRUE == pObj->IsKindOfObject(MONSTER_OBJECT))
            {
                Monster* pMon = (Monster*)pObj;
                //eNPC_DOMINATION_MAPOBJECT_NPC�� ������ Ÿ������ �־ ���ݰ����ϴ�.
                if( eNPC_DOMINATION_MAPOBJECT_NPC != pMon->GetMonsterInfo()->m_byGrade )
                {
                    if(FALSE == GameFunc::IsMove(g_pHero->GetPosition(), pObj->GetPosition(), TRUE, 200/*MAX_TILE_NODE_CNT_IN_DOMINATION*/))
                    {
                        return false;
                    }  
                }
            }
            else if(TRUE == pObj->IsKindOfObject(PLAYER_OBJECT))
            {
                float height = pObj->GetVisiblePos().z - g_pHero->GetVisiblePos().z;
                if(!(-MAX_ATTACK_HEIGHT_IN_DOMINATION > height || MAX_ATTACK_HEIGHT_IN_DOMINATION < height))
                {
                    if(FALSE == GameFunc::IsMove(g_pHero->GetPosition(), pObj->GetPosition(), TRUE, 200/*MAX_TILE_NODE_CNT_IN_DOMINATION*/))
                    {
                        return false;
                    }
                }
            }
        }
        else
        {
            // �Ϲ� ������
            //���� ������ �� ��� 4.5M �̻�/���� �� ��� �������� ���Ѵ�.
            float fHeight = pObj->GetVisiblePos().z - g_pHero->GetVisiblePos().z;
            if(-MAX_ATTACK_HEIGHT_IN_DOMINATION > fHeight || MAX_ATTACK_HEIGHT_IN_DOMINATION < fHeight)
                return false;

            if(TRUE == pObj->IsKindOfObject(MONSTER_OBJECT))
            {
                Monster* pMon = (Monster*)pObj;
                //eNPC_DOMINATION_MAPOBJECT_NPC�� ������ Ÿ������ �־ ���ݰ����ϴ�.
                if( eNPC_DOMINATION_MAPOBJECT_NPC != pMon->GetMonsterInfo()->m_byGrade )
                {
                    if(FALSE == GameFunc::IsMove(g_pHero->GetPosition(), pObj->GetPosition(), TRUE, 200/*MAX_TILE_NODE_CNT_IN_DOMINATION*/))
                    {
                        return false;
                    }
                }
            }
            else if(TRUE == pObj->IsKindOfObject(PLAYER_OBJECT))
            {            
                if(FALSE == GameFunc::IsMove(g_pHero->GetPosition(), pObj->GetPosition(), TRUE, 200/*MAX_TILE_NODE_CNT_IN_DOMINATION*/))
                {
                    return false;
                }
            }
        }
        
#else
        //���� ������ �� ��� 4.5M �̻�/���� �� ��� �������� ���Ѵ�.
        float fHeight = pObj->GetVisiblePos().z - g_pHero->GetVisiblePos().z;
        if(-MAX_ATTACK_HEIGHT_IN_DOMINATION > fHeight || MAX_ATTACK_HEIGHT_IN_DOMINATION < fHeight)
            return false;

        if(TRUE == pObj->IsKindOfObject(MONSTER_OBJECT))
        {
            Monster* pMon = (Monster*)pObj;
            //eNPC_DOMINATION_MAPOBJECT_NPC�� ������ Ÿ������ �־ ���ݰ����ϴ�.
            if( eNPC_DOMINATION_MAPOBJECT_NPC != pMon->GetMonsterInfo()->m_byGrade )
            {
                if(FALSE == GameFunc::IsMove(g_pHero->GetPosition(), pObj->GetPosition(), TRUE, 200/*MAX_TILE_NODE_CNT_IN_DOMINATION*/))
                {
                    return false;
                }  
            }
        }
        else if(TRUE == pObj->IsKindOfObject(PLAYER_OBJECT))
        {
            if(FALSE == GameFunc::IsMove(g_pHero->GetPosition(), pObj->GetPosition(), TRUE, 200/*MAX_TILE_NODE_CNT_IN_DOMINATION*/))
            {
                return false;
            }
        }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
    }

    return true;
}

BOOL HeroActionInput::SetFollowState( BOOL bFlag, DWORD dwPlayerKey )
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR   szText[INTERFACE_STRING_LENGTH] = {0,};

    BOOL  preState = m_bFollowPlayer;
    m_bFollowPlayer = bFlag;
    if( preState && m_bFollowPlayer == FALSE )
    {
        // ����ٴϱ⸦ ����ϴ�.
        g_InterfaceManager.GetInterfaceString( eST_SET_FOLLOWPLAYER_STOP, szMessage, INTERFACE_STRING_LENGTH );
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
    }

    if( dwPlayerKey )
    {
        Object   *pObj = g_ObjectManager.GetObject( dwPlayerKey );
        if( pObj )
        {
            Character   *pChar = (Character *)pObj;
            RELATIONKIND::TYPE type = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( pChar, lsForceAttack() );
            if( type == RELATIONKIND::Friend )
            {
                m_dwFollowPlayerKey = dwPlayerKey;
                m_dwFollowPlayerTime = g_CurTime;
                SetCurrentTarget( m_dwFollowPlayerKey );

                // %s ���� ����ٴմϴ�.
                g_InterfaceManager.GetInterfaceString( eST_SET_FOLLOWPLAYER_START, szText, INTERFACE_STRING_LENGTH );
                Sprintf( szMessage, szText, pChar->GetName() );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
            }
            else
            {
                // ����ٴ� �� ���� ����Դϴ�.
                g_InterfaceManager.GetInterfaceString( eST_CANNOT_FOLLOW_OBJECT, szText, INTERFACE_STRING_LENGTH );
                Sprintf( szMessage, szText, pChar->GetName() );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage );
                m_bFollowPlayer = FALSE;

                return FALSE;
            }
            
        }
    }

    return TRUE;
}

void HeroActionInput::ProcessFollow( void )
{
    if( g_pHero && GetFollowState() )
    {
        if( (g_CurTime - m_dwFollowPlayerTime) >= FOLLOW_DELAY_TIME )
        {
            m_dwFollowPlayerTime = g_CurTime;

            Character   *pChar = (Character *)g_ObjectManager.GetObject( GetFollowPlayer() );
            if( pChar )
            {
                PLAYER_ACTION action;
                ZeroMemory(&action,sizeof(action));
                action.ActionID = ACTION_PLAYERFOLLOW;
                action.PLAYERMEET.dwTargetID = pChar->GetObjectKey();
                m_QueueAction = action;

                // �ڵ� ���� ����
                SetAutoAttack(FALSE);
                SetAutoRun(FALSE);

                // �Ÿ��� �ʹ��ָ� �̵�~
                WzVector vTargetPos = pChar->GetPosition();
                WzVector vOffset = g_pHero->GetPosition() - vTargetPos;
                float fDistance = VectorLength( &vOffset );
                if( fDistance > DISTANCE_FOLLOW_PLAYER )
                {
                    MoveToAction(vTargetPos, false);
                }
            }
        }
    }
}

void HeroActionInput::SetHeroTurnState()
{
    if(g_pHero)
    {
        if( g_pHero->GetCurState() == STATE::IDLE || g_pHero->GetCurState() == STATE::KEYBOARDMOVE )
        {
            // ���ΰ� ȸ�� ���� ����
            g_pHero->SetHeroTurnState( g_Camera.GetCameraTurnState() );

            g_pHero->SetDirection( g_Camera.GetCameraTo(), g_Camera.GetCameraFrom(), FALSE );
        }
    }
}

BOOL HeroActionInput::IsActionState()
{
    if(!g_pHero)
        return FALSE;

    if(g_pHero->GetCurState() != STATE::MOVE && 
        g_pHero->GetCurState() != STATE::KEYBOARDMOVE&&
        g_pHero->GetCurState() != STATE::SPECIALMOVE&&
        g_pHero->GetCurState() != STATE::KEYBOARDJUMP&&
        g_pHero->GetCurState() != STATE::SIT &&
        g_pHero->GetCurState() != STATE::EMOTICON) return TRUE;
    else
        return FALSE;
}

BOOL HeroActionInput::CanSitState()
{
    if (!g_pHero)
    {
        return FALSE;
    }

	if( g_pHero->IsPolymorph() == TRUE )
	{
		return FALSE;
	}

    return BOOL((g_pHero->GetCurState() != STATE::SIT) && 
                (g_pHero->IsRiding() == FALSE ) &&
                (g_pHero->is_spread_wings() == false) &&
                (g_pHero->GetCurState() != STATE::SUMMON_RIDER) &&
                (g_pHero->GetCurState() != STATE::ETHERIA_EXTRACT) && 
                (g_pHero->GetCurState() != STATE::ATTACK) &&
                (g_pHero->GetCurState() != STATE::MOVE) &&     
                (g_pHero->GetCurState() != STATE::KEYBOARDMOVE) &&
                (g_pHero->GetCurState() != STATE::COLLECT) &&
                (FALSE == IsAutoAttackMode()));
}


void HeroActionInput::SetFixTarget(BOOL bFixTarget)
{
    m_bFixTarget = bFixTarget;
}

BOOL HeroActionInput::SetNearestTarget()
{
    if(!g_pHero)
        return FALSE;

    if( g_pHero->m_vectorTargetMonster.empty() == true )
    {
        // �����.
        return TRUE;
    }

    // ����Ʈ�� ��ϵ� ���͵��� Ÿ������ �� �ִ� �ݰ� �ȿ� �ִ� �͵��̴�
    // �� �߿��� �����̼� ��Ų��.( ���� Ÿ�� )
    TAB_TARGET_MONSTER_vector_Itr it;
    
    int       iCount = 0;
    BOOL  bFind = FALSE; 

    TAB_TARGET_MONSTER_vector_Itr near_pos = g_pHero->m_vectorTargetMonster.begin();
    g_pHero->m_CurTargetInfo.dwObjectKey = (*near_pos).dwObjectKey;
    g_pHero->m_CurTargetInfo.fDistance = (*near_pos).fDistance;
    g_pHero->m_CurTargetInfo.wTargetCheck = 0;
    g_pHero->m_CurTargetInfo.wTargetCount = iCount;
    
    if( bFind == FALSE )
    {
        it = g_pHero->m_vectorTargetMonster.begin();
        g_pHero->m_CurTargetInfo.dwObjectKey = (*it).dwObjectKey;
        g_pHero->m_CurTargetInfo.fDistance = (*it).fDistance;
        g_pHero->m_CurTargetInfo.wTargetCheck = 0;
        g_pHero->m_CurTargetInfo.wTargetCount = -1;
    }

    // Ÿ�� ����
    SetCurrentTarget( g_pHero->m_CurTargetInfo.dwObjectKey );

    return TRUE;
}

BOOL HeroActionInput::GetFixTarget()
{
    return m_bFixTarget;
}




void HeroActionInput::SetForwardTargeting(BOOL val)
{
    m_bForwardTargeting = val;
}

BOOL HeroActionInput::GetForwardTargeting()
{
    return m_bForwardTargeting;
}


BOOL HeroActionInput::CanGetItem(DWORD dwObjectKey)
{
    // ������ üũ ����
    return TRUE;
}

//-------------------------------------------------------------------------------------------
/**
*/
BOOL HeroActionInput::CannotMoveState() const
{
    if ( !g_pHero )
    {
        return TRUE;
    }

	if ( g_pHero->CannotMove() == TRUE )
	{
		return TRUE;
	}

    if ( g_pHero->GetCurState() == STATE::DOWN )
    {
        return TRUE;
    }

    if ( g_pHero->GetCurState() == STATE::SIT && g_pHero->GetSitState() == false )
    {
        return TRUE;
    }    

#ifdef _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
    if (g_pHero->sitting_packet_delay() == TRUE)
    {
        return TRUE;
    }
#endif //_NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG

    return FALSE;
}

void HeroActionInput::ShowCanNotMoveReason(DWORD dwReason)
{
    TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
    if(dwReason!=m_OldReason)
    {
        switch(dwReason)
        {
        case eCHAR_STATE_SLEEP:
            {
                // ���� �����Դϴ�. 5521
                g_InterfaceManager.GetInterfaceString(eST_CHAR_STATE_SLIP , szMessage ,INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }
            break;

        case eCHAR_STATE_STUN:
            {
                // ���� �����Դϴ�. 5522
                g_InterfaceManager.GetInterfaceString(eST_CHAR_STATE_STUN , szMessage ,INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage); 
            }
            break;

        case eCHAR_STATE_FROZEN:
            {
                // ������ �����Դϴ�. 5523
                g_InterfaceManager.GetInterfaceString(eST_CHAR_STATE_FROZEN , szMessage ,INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);   
            }
            break;

        case eCHAR_STATE_DELAY:
            {
                // ������ �����Դϴ�. 5524
                g_InterfaceManager.GetInterfaceString(eST_CHAR_STATE_DELAY , szMessage ,INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);   
            }
            break;

        case eCHAR_STATE_STONE:
            {
                // ���� �����Դϴ�. 5525
                g_InterfaceManager.GetInterfaceString(eST_CHAR_STATE_STONE , szMessage ,INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);   
            }
            break;

        case eCHAR_STATE_SUMMON:
        case eCHAR_STATE_HOLDING: 
            {
                // Ȧ�� �����Դϴ�. 5526
                g_InterfaceManager.GetInterfaceString(eST_CHAR_STATE_HOLDING , szMessage ,INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
            }
                break;

        default:
            {
                // �����Ƽ ��Ÿ������ �̵��Ұ� 5527
                g_InterfaceManager.GetInterfaceString(eST_CHAR_STATE_ABILITY_ETC_RESON , szMessage ,INTERFACE_STRING_LENGTH );
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szMessage);
            }
            break;
        }
    }

    m_OldReason=dwReason;
}

BOOL HeroActionInput::FindNearesetPathTile(WzVector *pClickedPos,WzVector *pRet)
{
    int iTileFocussing = -1;

    // ��� Tile ���      
    iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( *pClickedPos, NULL, -1.0f, 100.0f ); 

    if(!g_pMap)
        return FALSE;

    if( iTileFocussing >= 0 )
    {
        WORD attribute = g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute( iTileFocussing );

        WORD condition = PTA_ONLY_JUMP | PTA_NO_WALK;

        if( !( attribute & condition ) )
        {
            *pRet = *pClickedPos;
            return TRUE;
        }

    }


    

    WzVector vPos = g_pHero->GetPosition();

    g_pMap->GetWorldBase()->PathExp_Reset(g_pPathExplorer, &vPos, -1);

    if (g_pPathExplorer->GetTile() == -1)
    {
        return FALSE;
    }

    WzVector vOffset = *pClickedPos - vPos;
    
    WzVector oldPos=g_pPathExplorer->GetPos();

    
    g_pSunTerrain->PathExp_Thrust(g_pPathExplorer,&vOffset,PTA_NO_WALK);


    g_pSunTerrain->PathExp_Progress(g_pPathExplorer, 1000000,NULL);

    WzVector curPos=g_pPathExplorer->GetPos();

    if (curPos!=oldPos )
    {
        WzVector vDiff = curPos- oldPos;
        float    fDiffLength = VectorLength(&vDiff);

        if(fDiffLength>=1.0f)
        {
            *pRet = g_pPathExplorer->GetPos();
            return TRUE;
        }
    }
    
    return FALSE;
}

void HeroActionInput::SetCameraBaseMove(BOOL val)
{
    m_bCameraBaseMove = val;
}

BOOL HeroActionInput::GetCameraBaseMove()
{
    return m_bCameraBaseMove;
}

//------------------------------------------------------------------------------
BOOL HeroActionInput::ToggleShowPetInfo(void)
{
    uiPetGauge* pet_dialog = GET_CAST_DIALOG(uiPetGauge, IM_PET_MANAGER::PET_GAUGE_DLG);
    if (pet_dialog)
    {
        pet_dialog->NextShowType();
    }
    return TRUE;
}
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
void HeroActionInput::ForceAreaSkillPosition(DWORD skill_code, WzVector target_position)
{
    // �������� ������ �н�
    m_TargetAreaSkillID = static_cast<SLOTCODE>(skill_code);
    m_ShowTargetAreaPointer = TRUE;
    m_IsClickedTargetArea = TRUE;
    m_TargetAreaPosition = target_position;
}
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM
//------------------------------------------------------------------------------
/**
	��Ƽ����� Ŭ�������� Ÿ�ϰ�����
*/
void HeroActionInput::ClickPartyMember(DWORD dwObjKey)
{
	g_pHero->m_CurTargetInfo.dwObjectKey = dwObjKey;		
	m_dwSelectedTarget= dwObjKey;					
	m_TargetType = RELATIONKIND::Friend;			
}

BOOL HeroActionInput::IsIdleState( void )
{
	if( g_pHero )
	{
		return ( g_pHero->GetCurState() == STATE::IDLE || 
			g_pHero->GetCurState() == STATE::SIT  || 
			g_pHero->GetCurState() == STATE::RIDER_TURN  || 	
			g_pHero->GetCurState() == STATE::EMOTICON );
	}

	return FALSE;
}

#ifdef _NA_20100307_BETAKEY_SYSTEM
BOOL HeroActionInput::IsUseableNPCBetakey(BASE_NPCINFO* pNPCInfo)
{
    if (pNPCInfo)
    {
        uiLoginMan* login_manager = static_cast<uiLoginMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_LOGIN));
        if (login_manager)
        {
            if (login_manager->CanAbilityContent(pNPCInfo->m_BeytaKey))
            {
                return TRUE;
            }
        }
        
    }
    return FALSE;
}
#endif//_NA_20100307_BETAKEY_SYSTEM

BOOL HeroActionInput::lsForceAttack()
{
    #ifdef _YJ_PK_OPTION_001120
    if (GAMEOPTION->GetPKWithoutCtrlKey() == TRUE )
    {
        return ATTACK_PROPENSITY_FORCE;
    }
    #endif //_YJ_PK_OPTION_001120

	return ATTACK_PROPENSITY_NORMAL;
}

void HeroActionInput::CancelSkillQueue()
{
#ifdef _NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL
    g_SkillQueue.SkillQueueMoveCancle();
    m_IsClickedTargetArea = FALSE;

#else
    g_SkillQueue.ClearAll();
    SetAutoTarget(FALSE);
#endif //_NA_004923_120618_SKILL_QUEUE_MOVE_CANCEL
}
