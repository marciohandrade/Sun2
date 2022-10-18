//------------------------------------------------------------------------------
//  Monster.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "varpacket.h"
#include "Monster.h"
#include "MonsterGauge.h"
#include <NPCInfoParser.h>
#include "AppearanceManager.h"
#include "InterfaceManager.h"
#include "SoundEffect.h"
#include "HeroActionInput.h"
#include "SkillInfoParser.h"
#include "StateInfoParser.h"
#include "Hero.h"
#include "MapObject.h"
#include "CharSoundInfoParser.h"
#include "WeaponSoundInfoParser.h"
#include "ProjectileManagerEx.h"
#include "ProjectileInfoParser.h"
#include "MonsterVoiceInfoParser.h"
#include "uiSummonTargetMan/uiSummonTargetMan.h"
#include "SummonTargetDialog.h"
#include "SummonSkillContainer.h"
#include "SkillSlot.h"
#include "GlobalFunc.h"
#include "GameConst_Color.h"
#include "ObjectManager.h"
#include <NPCAttributes.h>
#include "SkillAttrCalculator.h"
#include "CloneManager.h"
#include "Clone.h"
#include "SceneBase.h"

extern DWORD g_CurTime;

#define	MONSTER_HP_GAUGE_BAR_WIDTH		119	
#define	MONSTER_HP_GAUGE_BAR_HEIGHT		5
#define	MONSTER_HP_LINE_WIDTH			125
#define	MONSTER_HP_LINE_HEIGHT			11

//------------------------------------------------------------------------------
/**
*/
Monster::Monster(void)
: m_IsShowName(FALSE),
m_bSingleMove(FALSE),
m_bShadowSummon(FALSE),
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
m_bWitchBladeSummon(FALSE),
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
m_bMissedSummoner(FALSE),
m_hSummonTarget(INVALID_HANDLE_VALUE),
m_dwShowNameTick(0) 
{
	m_fMoveSpeed		= 0.001f;

	SetObjectType(MONSTER_OBJECT);

	m_fAttackSpeedModifier = 1.0f;
	m_fMoveSpeedModifier = 1.0f;
	m_DelaySkillSerial.clear();
	m_eMonsterType = MONSTER_TYPE_GENERAL;	
	m_dwTransFormCode = 0;
	m_hRangeEffect = INVALID_HANDLE_VALUE;
	m_pNPCAttriburtes = NULL;
	m_hCrystalWarpEffect = INVALID_HANDLE_VALUE;
	m_bCreateDeadBody = false;
    set_stand_state(kStand_State);
    m_hSummonIntensifyEffect = INVALID_HANDLE_VALUE;

}

//------------------------------------------------------------------------------
/**
*/
Monster::~Monster( void )
{
}

//------------------------------------------------------------------------------
/**
	NPC 코드로 변신 한다.
*/
void Monster::SetTransFormNpcCode(DWORD dwCode)
{
	m_dwTransFormCode=dwCode;
}

void Monster::TransForm()
{
	// 시체 스르륵 사라지기

	BASE_NPCINFO* pInfo=NPCInfoParser::Instance()->GetNPCInfo( m_dwTransFormCode );
	if( pInfo == NULL )
		return;

	//-----------------------------------------------------------------------	
	// 몬스터 정보 연결
	//-----------------------------------------------------------------------
	SetMonsterInfo( NPCInfoParser::Instance()->GetNPCInfo( m_dwTransFormCode ) );
	APPEARANCEINFO appearInfo;
	ZeroMemory(&appearInfo,sizeof(appearInfo));
	appearInfo.ObjectType = APPEARANCEINFO::MONSTER;
	appearInfo.pMonsterInfo = GetMonsterInfo();

	InitRender(MP_COUNT,MP_BONE,&appearInfo,0,FALSE);

	
	if( WzBoundingVolume* pBV = GetPickingBoundingVolume(0) )
	{
		if (pBV->m_eBvType == BVT_AABB)
		{
			float fHeight = pBV->m_aabb.m_wvMax.z - GetVisiblePos().z;
			SetMeshHeight(fHeight+0.1f);
		}
	}
	else if( WzBoundingVolume* pBV = GetBoundingVolume() )
	{
		if (pBV->m_eBvType == BVT_AABB)
		{
			float fHeight = (pBV->m_aabb.m_wvMax.z - pBV->m_aabb.m_wvMin.z);
			SetMeshHeight(fHeight+0.2f);
		}
	}

	TCHAR szString[INTERFACE_STRING_LENGTH];
	g_InterfaceManager.GetInterfaceString( appearInfo.pMonsterInfo->m_NCode, szString, MAX_MONSTERNAME_LENGTH);
	StrCopy( appearInfo.pMonsterInfo->m_pszName, szString );
	SetName( appearInfo.pMonsterInfo->m_pszName );


	if (m_pNPCAttriburtes)
	{
		delete m_pNPCAttriburtes;
		m_pNPCAttriburtes = NULL;
	}

	m_pNPCAttriburtes = new NPCAttributes;
	m_pNPCAttriburtes->SetOwner( &m_scNPCAtt); 
	m_pNPCAttriburtes->SetCharInfo(pInfo,0,0,0);

	m_dwHideTime = 1000;	//감추기를 사용할 경우 감추기가 끝난후 자동으로 fadeIn됨 
}

void Monster::SetPolymorph( BOOL Active )
{
    if( m_IsPolymorphApperance == Active )
        return;

    m_IsPolymorphApperance = Active;

    if( m_IsPolymorphApperance == TRUE )
    {
        APPEARANCEINFO appearInfo;
        ZeroMemory(&appearInfo,sizeof(appearInfo));
        appearInfo.ObjectType = APPEARANCEINFO::POLOMORPH;
        //appearInfo.pMonsterInfo = GetMonsterInfo();

        InitRender( MP_COUNT, MP_BONE, &appearInfo, 0, FALSE );
        //appearInfo.Job = GetClass();
        //InitRender(BP_COUNT, BP_BONE, &appearInfo, AppearanceManager::kAppearanceAttachIndex_Count, TRUE);

		SetIdleState();

		GlobalFunc::CreateEffectBeUpToOption( StrToWzID("KC05"), GetVisiblePos() );
    }
    else
    {
        // 파츠 초기화
        APPEARANCEINFO appearInfo;
        ZeroMemory(&appearInfo,sizeof(appearInfo));
        appearInfo.ObjectType = APPEARANCEINFO::MONSTER;
        appearInfo.pMonsterInfo = GetMonsterInfo();
        InitRender(MP_COUNT,MP_BONE,&appearInfo,0,FALSE);

		SetScale(m_fScale);
		SetIdleState();

		GlobalFunc::CreateEffectBeUpToOption( StrToWzID("KC05"), GetVisiblePos() );
    }
}


//------------------------------------------------------------------------------
/**
*/
BOOL Monster::Create( DWORD dwObjectKey, DWORD dwID )
{
	m_eMonsterType = MONSTER_TYPE_GENERAL;

	SetObjectKey( dwObjectKey );

    BOOL rt = Character::Create(dwObjectKey, dwID);
    assert(rt);

    // 몬스터 정보 설정
    SetMonsterInfo( NPCInfoParser::Instance()->GetNPCInfo( dwID ) );

    // 파츠 초기화
	APPEARANCEINFO appearInfo;
	ZeroMemory(&appearInfo,sizeof(appearInfo));
	appearInfo.ObjectType = APPEARANCEINFO::MONSTER;
	appearInfo.pMonsterInfo = GetMonsterInfo();
    InitRender(MP_COUNT,MP_BONE,&appearInfo,0,FALSE);

	//	상태 초기화( IDLE 상태 )
	InitState(STATE::IDLE,g_CurTime);
	m_iMonsterGaugeShowDelay = 0;
	
	TCHAR	szString[INTERFACE_STRING_LENGTH];

	g_InterfaceManager.GetInterfaceString(
		appearInfo.pMonsterInfo->m_NCode, 
		szString, 
		MAX_MONSTERNAME_LENGTH);

#ifdef _DEV_VER
    //#ifdef _SCJ_SHOW_NPC_ID
    if (CGeneralGameParam::Instance()->IsShowNpcId())
    {
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
        StrnCopy(buffer_string, szString, INTERFACE_STRING_LENGTH-1); 
        Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
            buffer_string, appearInfo.pMonsterInfo->m_MonsterCode);
    }
    //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER

	StrCopy( appearInfo.pMonsterInfo->m_pszName, szString );

	SetName( appearInfo.pMonsterInfo->m_pszName );

	m_dwParentId=0;
	m_dwTransFormCode=0;
	m_iAlarmVoice = 0;
	m_iWaitSoundDelay = 0;

	// 몬스터는 그냥 안쪽 보이삼
	m_bNoCull = TRUE;

#ifdef _POSITION_DEBUG
	Sleep(500);
#endif

	BASE_NPCINFO *pInfo=NPCInfoParser::Instance()->GetNPCInfo( dwID );

	if(pInfo)
	{
		m_pNPCAttriburtes = new NPCAttributes;
		m_pNPCAttriburtes->SetOwner( &m_scNPCAtt); 
		m_pNPCAttriburtes->SetCharInfo(pInfo,0,0,0);
	}

	SetIdleAniID();

	m_dwMasterGuildID = 0;
	SetMapObjectKey(0);
	m_bGetOriginPos = false;

    SetScale(m_fScale);

    InitMeshHeight();

    return TRUE;
}

void Monster::CreateCrystalWarpEffect(DWORD dwEffectID)
{
	if(eNPC_CRYSTAL_WARP == this->GetMonsterInfo()->m_byGrade)
	{
		m_hCrystalWarpEffect = GlobalFunc::CreateEffectBeUpToOption(dwEffectID,this->GetPosition());
	}
}

void Monster::DestoryCrystalWarpEffect()
{
	if(m_hCrystalWarpEffect != INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hCrystalWarpEffect);
		m_hCrystalWarpEffect = INVALID_HANDLE_VALUE;
	}
}

//------------------------------------------------------------------------------ 
WzVector Monster::GetSkillEffectScale(SKILL_EFFECT* skill_effect)
{
    if ((skill_effect != NULL) && 
        (skill_effect->dwStatusID >= eCHAR_STATE_HPRATE_INCREASE13) && 
        (skill_effect->dwStatusID <= eCHAR_STATE_MAGICDEFENSE15)) 
    {
        //! 피의성전 버프 아이템은 이펙트 하나로 스케일을 다르게 사용
        //! 스크립트로 안되어서 하드코등
        WzVector scale = c_UnitVector;
        if (GetMonsterInfo()->m_MonsterCode == 60007) //피의결정체
        {
            if (GetCurrentAnimation() == StrToWzID("A002"))
            {
                scale.z = 0.87f;
            }
            else if (GetCurrentAnimation() == StrToWzID("A003"))
            {
                scale.x = 1.1f;
                scale.y = 1.1f;
                scale.z = 0.7f;
            }
        }
        else if (GetMonsterInfo()->m_MonsterCode == 60006) //피의고리
        {
            scale.x = 0.67f;
            scale.y = 0.67f;
            scale.z = 0.67f;

            if (GetCurrentAnimation() == StrToWzID("A002"))
            {
                scale.z = 0.56f;
            }
            else if (GetCurrentAnimation() == StrToWzID("A003"))
            {
                scale.x = 0.8f;
                scale.y = 0.8f;
                scale.z = 0.4f;
            }
        }

        return scale;
    }

    return c_UnitVector;
}

//------------------------------------------------------------------------------
/** GS_BUG_FIX:서버에서 뷰포트작업에의해 셔먼이오고 플레이어가옴
*/
void
Monster::SetMissedSummoner()
{
	m_bMissedSummoner = TRUE;
}

//------------------------------------------------------------------------------
/** GS_BUG_FIX:주인이 없을때는 Process에서 지속적으로 찾아야한다.
*/
void
Monster::FindMissedSumoner()
{	
	  Player* pPlayer = (Player*)g_ObjectManager.GetObject(m_dwParentId);
		
	  if (!pPlayer) 
      {
          return;
      }
		
	  //주인이찾았다(입장했을때)
	  pPlayer->AddSummonId(GetObjectKey());
	  m_bMissedSummoner = FALSE;
}


//------------------------------------------------------------------------------
/**
*/
void 
Monster::SetParentId(const DWORD& dwId, const BYTE& byCommand, BOOL bHideDialog)
{
	m_dwParentId = dwId;
	
	//섀도우셔먼이면 타겟창안띄움
    //위치블레이드 고정 소환 수 이어도 타겟창안띄움
	if (bHideDialog)
    {
        return;
    }

	//Todo:위치 안좋다. 다른데로 옮길것 -지완
	if(g_pHero && g_pHero->GetObjectKey() == m_dwParentId)
	{
		SummonTargetDialog * pSummonTargetDlg = GET_CAST_DIALOG(SummonTargetDialog, IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG);

		if (pSummonTargetDlg)
		{
			pSummonTargetDlg->SetTargetKey( GetObjectKey() );
			pSummonTargetDlg->ShowWindow(TRUE);
			pSummonTargetDlg->ResetOhterCheckButton(byCommand);	
		}
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
		SummonSkillContainer * pSummonSkillContainer = (SummonSkillContainer * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SUMMON_SKILL_CONTAINER);
		
		if(pSummonSkillContainer)
		{
			pSummonSkillContainer->SetTargetKey( GetObjectKey() );
			pSummonSkillContainer->ShowWindow(TRUE);

			//SCSkillSlot skillSlot;
			//skillSlot.SetCode(154);//스킬 코드 

			BASE_NPCINFO *pInfo = NPCInfoParser::Instance()->GetNPCInfo(m_NPCInfo.m_MonsterCode);
			
			if(pInfo)
			{
				for(int i=0;i<BASE_NPCINFO::MAX_SKILL_NUM;i++)
				{

					if(i>=pSummonSkillContainer->GetMaxSlotNum())
						break;

					SkillSlot skillSlot;
					
					if(pInfo->m_wSkillCode[i] != 0)
					{
						skillSlot.SetSlotType(ST_SKILL);
						skillSlot.SetCode(pInfo->m_wSkillCode[i]);

						pSummonSkillContainer->InsertSlot(i,skillSlot);
					}
                    else
                    {
                        pSummonSkillContainer->DeleteSlot(i, NULL);
                    }

				}
			}
		}
#endif
	}
}

//------------------------------------------------------------------------------
/**
*/
void Monster::Destroy( void )
{
	//Todo:위치 안좋다. 다른데로 옮길것 -지완  
	if(m_dwParentId)
	{
		Object *pObject=g_ObjectManager.GetObject(m_dwParentId);
		if(pObject&&pObject->IsKindOfObject(PLAYER_OBJECT))
		{
			Player *pPlayer=(Player*)pObject;

		
			pPlayer->RemoveSummonId(this->GetObjectKey());

			if(pPlayer==g_pHero&&pPlayer->GetSummonId()==NONE_ID)//소환한 놈이 한명도 없다면 
			{
				SummonTargetDialog * pSummonTargetDlg = GET_CAST_DIALOG(SummonTargetDialog, IM_SUMMON_TARGET_MANAGER::SUMMON_TARGET_DIALOG);
				if( pSummonTargetDlg )
				{
					pSummonTargetDlg->SetTargetKey( 0 );
					pSummonTargetDlg->ShowWindow(FALSE);
				}
#ifdef _NA_000000_20130521_NOT_SUMMON_SKILL
#else
				SummonSkillContainer * pSummonSkillContainer = (SummonSkillContainer * )g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_SUMMON_SKILL_CONTAINER);

				if(pSummonSkillContainer)
				{
					pSummonSkillContainer->SetTargetKey( 0 );
					pSummonSkillContainer->ShowWindow(FALSE);
					pSummonSkillContainer->Clear();
				}
#endif
			}

		}
	}

	//------------------------------------------
	// 몬스터 게이치 창 해제
	//------------------------------------------
	Character::Destroy();
	m_DelaySkillSerial.clear();

	if (m_pNPCAttriburtes)
	{
		delete m_pNPCAttriburtes;
		m_pNPCAttriburtes = NULL;
	}

	if(m_hRangeEffect!=INVALID_HANDLE_VALUE&&g_pEffectManager)
	{
		g_pEffectManager->RemoveEffectPtr(m_hRangeEffect);
	}
	//소울컨트롤된 몬스터의 타겟팅이펙트
	if(m_hSummonTarget!=INVALID_HANDLE_VALUE)
	{
		g_pEffectManager->RemoveEffectPtr(m_hSummonTarget);
		m_hSummonTarget = INVALID_HANDLE_VALUE;
	}

	if(g_pHero)
	{
		if((g_pHero->GetGuildRenderInfo().m_GuildGuid!=INVALID_GUILDGUID) 
			&& (g_pHero->GetPlayerGuildID() == this->GetMasterGuildID())
			&& (eNPC_CRYSTAL_WARP == this->GetMonsterInfo()->m_byGrade))
		{
			g_pHero->SetCrystalWarpKey(this->GetObjectKey(), false);
		}
	}

	DestoryCrystalWarpEffect();
    DestorySummonIntensifyEffect();
}
//------------------------------------------------------------------------------
/**
*/
BOOL Monster::Render( void )
{
	if ( GetShow() == FALSE ) return FALSE;	

	g_pSunRenderer->SetColor(255,255,255,255);

#ifdef _NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE
    if (m_bShadowSummon)
    {
        CWzUnit* pUnit = GetBasePart()->GetWzUnit();
        if (pUnit)
        {
            int num = pUnit->GetSubType(0)->GetNumRenderInfo();

            CWzArray<int> mesh_numbers;

            // 원본 보존
            for (int i = 0; i < num; ++i)
            {
                mesh_numbers.Add(pUnit->GetSubType(0)->GetRenderInfo(i)->m_iMesh);
                pUnit->GetSubType(0)->GetRenderInfo(i)->m_iMesh = -2;               //#define WRIM_NONE   (-2)
            }

            // 0번째 렌더인포만 소환수 전용으로 활성화
            BYTE old_set = pUnit->GetSubType(0)->GetRenderInfo(0)->m_bySet;
            pUnit->GetSubType(0)->GetRenderInfo(0)->m_iMesh = -1;               //#define WRIM_ALL    (-1)
            pUnit->GetSubType(0)->GetRenderInfo(0)->m_bySet = 99;               //99 소환수 전용 렌더

            BOOL result = Character::Render();

            // 원본 복구
            pUnit->GetSubType(0)->GetRenderInfo(0)->m_bySet = old_set;
            for (int i = 0; i < num; ++i)
            {
                pUnit->GetSubType(0)->GetRenderInfo(i)->m_iMesh = mesh_numbers.Get(i);
            }

            mesh_numbers.Clear();

            return result;
        }
    }
    return Character::Render();
#else
    return Character::Render();
#endif //_NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE
}

//------------------------------------------------------------------------------
/**
*/
int Monster::DoAction(PLAYER_ACTION *pAction)
{
	switch(pAction->ActionID)
	{
	case ACTION_TRANSFORM:
		{
			m_dwTransFormCode=pAction->TRANSFORM.rcCode;//몬스터 변신 코드는 NPC코드 이다.
			GetTransformData().SetTransFormType(pAction->TRANSFORM.transFormType);
			SetNextState(STATE::TRANSFORM,g_CurTime);
		}
		return 1;

	case ACTION_FALLDOWN:
	case ACTION_AIR_FALLDOWN:
		{
			if( m_eMonsterType == MONSTER_TYPE_TOTEM )
			{
				if( m_hRangeEffect != INVALID_HANDLE_VALUE && g_pEffectManager )
				{
					g_pEffectManager->RemoveEffectPtr( m_hRangeEffect );
				}
			}
		}
		break;
	}

	return Character::DoAction(pAction);
}

//------------------------------------------------------------------------------
/** 섀도우 소환몬스터는 색을 달리한다.
*/
void Monster::OnShadowSummon()
{	
	m_bShadowSummon = TRUE;
    
    GlobalFunc::CreateEffectBeUpToOption(StrToWzID("K787"), GetPosition());

	InitSummonTarget();
}

void Monster::StartExtraEffect()
{
	if (!m_bShadowSummon) 
    {
        return; 
    }

#ifdef _NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE
#else
	SetColor(WzColor_RGBA(70, 0, 0, 255));
#endif //_NA_000000_20140410_SHADOW_SUMMON_APPEARANCE_CHANGE
}


//------------------------------------------------------------------------------
/** 타겟팅 랜더링
*/
void Monster::UpdateSummonTargeting()
{
	if(m_hSummonTarget==INVALID_HANDLE_VALUE) return; 
	
	CEffectBody* pEffectBody = g_pEffectManager->GetEffectPtrHandle(m_hSummonTarget); 

	if(!pEffectBody) return;
	
	pEffectBody->SetResultMatrix(GetVisiblePos(),c_ZeroVector,m_vTargetScale);
}
//------------------------------------------------------------------------------
/** 타겟팅초기화(이펙트생성및 크기 셋팅)
*/
void Monster::InitSummonTarget()
{
	if(!g_pHero) return;

	//주인공이 소환한몬스터만 타겟이펙트 띄운다.
    if(g_pHero->GetObjectKey() != m_dwParentId) return;

	//소울컨트롤된 몬스터의 타겟팅이펙트
	m_hSummonTarget	 = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("K788"),GetVisiblePos());

	if(m_hSummonTarget==INVALID_HANDLE_VALUE) return; 

	CEffectBody* pEffectBody = g_pEffectManager->GetEffectPtrHandle(m_hSummonTarget); 

	if(!pEffectBody) return;

	//피킹볼륨
	WzBoundingVolume *pVolume  = this->GetPickingBoundingVolume(0);	

	//피킹볼륨이 없다면 바운딩볼륨으로
	if(!pVolume)
		pVolume  = this->GetBoundingVolume();

	SetVector(&m_vTargetScale,0.7f,0.7f,0.7f);
	
	//볼륨박스대각길이를 기준으로함
	if(pVolume)
	{
		WzVector vLength = pVolume->m_aabb.m_wvMax-pVolume->m_aabb.m_wvMin;
		vLength.z = 0.f;
		float fBoxLength = VectorLength(&vLength)*0.3f;

		if(fBoxLength >= 1.f)
			m_vTargetScale*=fBoxLength;
	}
	
	pEffectBody->SetResultMatrix(GetVisiblePos(),c_ZeroVector,m_vTargetScale);
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetHP( DWORD iHP )
{
	Character::SetHP( iHP );	

    if (IsDead() == TRUE)
    {
        DestorySummonIntensifyEffect();
    }
}

void Monster::SetScale(float fScale)
{
	Object::SetScale(fScale);
	m_fScale=fScale;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Monster::SetPosition( const WzVector& wvPos ,int iTileIndex)
{
    if ( Character::SetPosition( wvPos, iTileIndex) == FALSE )
		return FALSE;

    m_dwCurTileAtt = Object::GetCurrentTileAtt(TRUE);
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetMonsterInfo(BASE_NPCINFO *pInfo)
{
	assert(pInfo);

	if (pInfo) 
	{
		m_NPCInfo = *pInfo;

		m_fScale = (float) m_NPCInfo.m_wSize / 100.0f; 
		SetScale(m_fScale);

        if (IsCentipedeMonster() == true)
        {
            set_stand_state(kBurrow_State);
        }
	}
}
//------------------------------------------------------------------------------
/**
*/
void Monster::SetCondition(BYTE byCondition)
{
	Character::SetCondition(byCondition);
}
//------------------------------------------------------------------------------
/**
*/
BASE_NPCINFO* Monster::GetMonsterInfo()
{
	return &m_NPCInfo;
}

//------------------------------------------------------------------------------
/**
*/
NPCAttributes*	Monster::GetNpcAttribute()
{
	return m_pNPCAttriburtes;
}

//------------------------------------------------------------------------------
/**
*/
WzID Monster::GetCurrentAttackAnimation()
{
	WzID animationID;

	if( GetAttackSequence() == 1 )
	{
	   animationID = StrToWzID("A001");	   
	}
	else
	{
	   animationID = StrToWzID("A002");
	   WzAnimationInfo * pInfo = GetAnimationInfo( animationID );
	   //인포가 없다면 A002를 사용 함
	   if( pInfo&&pInfo->m_wiAction != animationID )
	   {
		   animationID = StrToWzID("A001");
	   }

	   // 원거리몬스터이고 발사체가 없다면 1번 동작만 한다.
	   if (GetMonsterInfo()->m_fAttRange >= 7.0f)
	   {
	
	   }
	}

	 return animationID;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetIdleAniID( DWORD dwStand, DWORD dwIdle, DWORD dwAttackStand )
{
	m_dwStandAniID			= dwStand;
	m_dwIdleAniID			= dwIdle;
	m_dwAttackStandAniID	= dwAttackStand;
}

WzID Monster::GetCurrentIdleAnimation()
{
	WzID animationID = 0;
    
    if( IsFainting() )
    {
		animationID = StrToWzID("F004");	
    }
    else if ( m_iAttackStandTime > 0 )
	{
		animationID = m_dwAttackStandAniID;
	}
	else 
	{
		if( m_dwStandAnimDelay )
		{
			animationID = m_dwStandAniID;
		}
		else
		{
			animationID = m_dwIdleAniID;
		}
	}

	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID Monster::GetCurrentMoveAnimation()
{
	WzID animationID = 0;

	if( IsPolymorph() == TRUE )
	{
		return GlobalFunc::GetAnimationByPolymorph(eANI_RUN);
	}

	if (GetMoveState() == CMS_WALK) 
	{
		animationID = StrToWzID("N004");	
	}
	else
	{
		animationID = StrToWzID("N005");	
	}
	return animationID;
}

//-------------------------------------------------------------------------------------------
/**
*/
WzID Monster::GetCurrentRunAwayAnimation()
{
    return StrToWzID("R001");	
}

//------------------------------------------------------------------------------
/**
*/
WzID Monster::GetCurrentDeathAnimation()
{
	WzID animationID = 0;

	animationID = StrToWzID("D001");
	
	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID Monster::GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical)
{
	WzID animationID = 0;

	if (bCritical)
	{
		if (bRight) {
			animationID = StrToWzID("H0L2");	
		}
		else
		{
			animationID = StrToWzID("H0R2");	
		}

	}
	else {
		if (bRight) {
			animationID = StrToWzID("H0L1");	
		}
		else
		{
			animationID = StrToWzID("H0R1");	
		}
	}
	
	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::PlayDamageSound(eWEAPONSOUNDKIND eSoundKind)
{
	assert(eSoundKind >= 0 && eSoundKind < eWEAPONSOUNDKIND_MAX);
  
	if (eSoundKind < 0 || eSoundKind >= eWEAPONSOUNDKIND_MAX)
	{
		return;
	}

    SOUND_INFO_CODE SoundInfoCode;
    SoundInfoCode.wCharacterCode = static_cast<WORD>(GetMonsterInfo()->m_dwICode);
    SoundInfoCode.bWeaponType = static_cast<BYTE>(eSoundKind);
	
	BASE_CharSoundInfo *pInfo = CharSoundInfoParser::Instance()->GetCharSoundInfo(SoundInfoCode.dwSoundCode);
	BASE_WeaponSoundInfo *pWeaponSoundInfo = WeaponSoundInfoParser::Instance()->GetWeaponSoundInfo(eSoundKind);

	int iDamageVoice = 0;
	int iImpactSound = 0;
	int iWeaponSound = 0;

	if (pInfo)
	{
        iDamageVoice = pInfo->SoundInfo.dwMoanSound[rand() % MAX_SOUND_VARIATION];
        iImpactSound = pInfo->SoundInfo.dwArmorSound[rand() % MAX_SOUND_VARIATION];
	}

	if (pWeaponSoundInfo)
	{
		iWeaponSound = pWeaponSoundInfo->m_dWeaponSound[rand() % MAX_SOUND_VARIATION];
	}

	if (iDamageVoice)
	{
		g_SoundEffect.Play(iDamageVoice,&GetPosition());	
	}

	if (iImpactSound)
	{
		g_SoundEffect.Play(iImpactSound,&GetPosition());	
	}

	if (iWeaponSound)
	{
		g_SoundEffect.Play(iWeaponSound,&GetPosition());	
	}
}

//------------------------------------------------------------------------------
/**
*/
void Monster::CreateDamageEffect(BOOL bCritical,BOOL bRight,DWORD dwAttackObjId)
{	
	Character::CreateDamageEffect(bCritical,bRight,dwAttackObjId);
	
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetMonsterGaugeShowDelay(int iDelay)
{
	m_iMonsterGaugeShowDelay = iDelay;
}

void Monster::SetShowName( BOOL b )
{ 
	m_IsShowName = b;
	m_dwShowNameTick = 0; 
}


#ifdef __NA_0_20080415_PROCESS_END_PROECSS
int	Monster::OnEndProcess( DWORD dwTick )
{
	CEffectBody * pBody = g_pEffectManager->GetEffectPtrHandle(m_hRangeEffect);
	if( pBody)
	{
		if(GetShow()&&Alpha_WzColor(GetColor())>0.0f&&GetCurShowFlag())
			pBody->SetShow(TRUE);
		else
			pBody->SetShow(FALSE);

	}
	return 0;
}
#endif 


//------------------------------------------------------------------------------
/**
*/
BOOL Monster::Process(DWORD dwTick)
{
	if (m_IsShowName)
    {
		m_dwShowNameTick += dwTick; 
    }
	else
    {
		m_dwShowNameTick = 0;
    }

	if (m_dwShowNameTick >= 1000)
    {
		m_IsShowName = FALSE;
    }

    ProcessVoice(dwTick);
	ProcessStandAnimationDelay(dwTick);

	if (m_bMissedSummoner)
	{
		//GS_BUG_FIX:주인이 없다 찾아야한다.
		FindMissedSumoner();
	}

	m_iMonsterGaugeShowDelay -= dwTick;
	if (m_iMonsterGaugeShowDelay < 0)
	{
		m_iMonsterGaugeShowDelay = 0;
	}

	BOOL bRet = Character::Process(dwTick);

	if ((GENERALPARAM->IsNet()==FALSE) && GENERALPARAM->GetSpecialMode())
	{
        // 싱글모드시 몬스터 이동 및 스킬 보기 기능
		ProcessSingleMove();
	}

	WzBoundingVolume * pBV =GetPickingBoundingVolume(0);

	if(pBV)
	{
		if (pBV->m_eBvType == BVT_AABB)
		{
			float fHeight =pBV->m_aabb.m_wvMax.z - GetVisiblePos().z;
			SetMeshHeight(fHeight+0.2f);
		}
	}


	if(m_bShadowSummon)
	{
		UpdateSummonTargeting();
	}

	SetTriggerMapObjectAnimation();

	//////////////////////////////////////////////////////////////////////////
	//밀렸던 거리만큼 천천히 돌아온다
	if(MONSTER_TYPE_SETTING_MAPOBJECT == GetMonsterType())
	{
		if(m_bGetOriginPos)
		{
			WzVector wvLen = m_wvOriginPos - this->GetPosition();
			if( 0.001f < VectorLength2(&wvLen) )
			{
				wvLen =  wvLen*(0.005f*dwTick);
				WzVector wvPos = this->GetPosition() + wvLen;
				SetPosition(wvPos);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return bRet;
}

float Monster::ProcessAnimation(DWORD dwTick,WzEvent *pEvent,BOOL bIgnoreEvent)
{
	if(true == m_bCreateDeadBody)
	{
		dwTick = 10000;
		bIgnoreEvent = true;
	}

    float animation_progress = Character::ProcessAnimation(dwTick, pEvent, bIgnoreEvent);

    if ((IsCentipedeMonster() == true) && 
        (animation_progress >= 100.0f))
    {
        ProcessAnimationForCentipedeMonster();
    }

	return animation_progress;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::RenderMonsterDetailInfo()
{
	if (g_Input.GetState(DIK_V, KS_KEY) ) 
	{
		RenderMonsterStatus();
	
		//------- 몬스터 정보	
		g_pSunRenderer->Set2DMode( TRUE );
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

		TCHAR monsterinfo[256] = {0,};
		TCHAR monsterinfo2[256] = {0,};
		TCHAR monsterinfo3[256] = {0,};
        TCHAR monsterinfo4[256] = {0,};

		WzVector wzPos = GetPosition();
		WzVector wzVisualPos = GetVisiblePos();

		wzVisualPos.z += 2.3f;

		POINT pt2;
		g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos );

		Snprintf(monsterinfo,  256-1, _T("ObjKey[%ld], CODE[%ld]"), GetObjectKey(), GetMonsterInfo()->m_MonsterCode);
		Snprintf(monsterinfo2, 256-1, _T("AQ : %d AR : %d"), GetActionQueueCount(),GetAttackResultListSize());
		Snprintf(monsterinfo3, 256-1, _T("Blending Count : %d"), GetBasePart()->GetBlendAniCount());
        Snprintf(monsterinfo4, 256-1, _T("CurHP/MaxHP:%d/%d"), GetHP(),GetMaxHP());
        

		WzColor color;

		color = WzColor_RGBA(255,255,255,255);

		RECT rc,rc2,rc3,rc4;

		rc.left = pt2.x - 60;
		rc.top = pt2.y;
		rc.right = rc.left + 280;
		rc.bottom = rc.top + 30;

		rc2 = rc;
		rc.top -= 15;
		rc.bottom -= 15;

		rc3 = rc;
		rc3.top -= 15;
		rc3.bottom -= 15;

        rc4 = rc3;
        rc4.top -= 15;
        rc4.bottom -= 15;

		g_pSunRenderer->x_pManagerTextOut->DrawText(monsterinfo,&rc,color);
		g_pSunRenderer->x_pManagerTextOut->DrawText(monsterinfo2,&rc2,color);
		g_pSunRenderer->x_pManagerTextOut->DrawText(monsterinfo3,&rc3,color);
        g_pSunRenderer->x_pManagerTextOut->DrawText(monsterinfo4,&rc4,color);

		g_pSunRenderer->Set2DMode( FALSE );
	}
	
}

//------------------------------------------------------------------------------
/**
*/
WzVector Monster::GetArrowBonePos(int index)
{
	WzVector ret = GetVisiblePos();
	int boneindex = -1;

	if (GetBasePart()) 
    {
		if (GetBasePart()->GetWzUnit()) 
        {
			WzBoneInfo *pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(StrToWzID("ARRW"));

			if (pInfo)
            {
				boneindex = pInfo->m_iNode;
			}
		}
	}

	if (boneindex != -1)
	{
		ret = GetBonePos(boneindex);		
	}

	return ret;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::PlayDamageAnimation(BOOL bRight,BOOL bCritical)
{
	WzID damageAnimation = 0;
	BOOL bAniCritcal = FALSE;

#ifdef _NA_008235_BOSS_MONSTER_CRITICAL_DAMAGE_ANIMATION_IGNORE
    if (GetMonsterInfo()->m_byGrade == eNPC_BOSS ||
        GetMonsterInfo()->m_byGrade == eNPC_MIDDLEBOSS)
    {
        // 보스,캡틴의 경우 큰경직 삭제, 작은 경직을 크리+확률로 발생시킴
        if (bCritical && (rand() % 3 == 0))
        {
            damageAnimation = GetCurrentDamageAnimation(bRight,FALSE);
            SetCurrentBlendAnimation(damageAnimation,0,1.0f,-1,TRUE);
        }

        return;
    }
#endif //_NA_008235_BOSS_MONSTER_CRITICAL_DAMAGE_ANIMATION_IGNORE
	if (bCritical)
	{
		if (rand() % 2)
		{
			damageAnimation = GetCurrentDamageAnimation(bRight,TRUE);
			bAniCritcal = TRUE;
		}
		else
		{
			damageAnimation = GetCurrentDamageAnimation(bRight,FALSE);
		}
	}
	else
	{
		damageAnimation = GetCurrentDamageAnimation(bRight,FALSE);
	}

	assert(damageAnimation);

	if (bAniCritcal)
	{
		SetCurrentBlendAnimation(damageAnimation,0,1.0f,-1,FALSE);
	}
	else
	{
		SetCurrentBlendAnimation(damageAnimation,0,1.0f,-1,TRUE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetName( const tstring& Name )
{
	StrnCopy( m_NPCInfo.m_pszName, Name.c_str(), _countof(m_NPCInfo.m_pszName) );
    m_NPCInfo.m_pszName[_countof(m_NPCInfo.m_pszName) - 1] = '\0';

    Character::SetName(Name);
}

//------------------------------------------------------------------------------
void Monster::RestoreObjectName()
{
    g_InterfaceManager.GetInterfaceString(m_NPCInfo.m_NCode, m_NPCInfo.m_pszName, MAX_MONSTERNAME_LENGTH);

    SetName(m_NPCInfo.m_pszName);
}



//------------------------------------------------------------------------------
/**
*/
void  Monster::ChangeHPRatio(float fRatio)
{
	BASE_NPCINFO *pInfo = NPCInfoParser::Instance()->GetNPCInfo(m_NPCInfo.m_MonsterCode);
	assert(pInfo);

	if(pInfo)
	{
		DWORD dwMaxHP = (DWORD)( pInfo->m_dwMaxHP * fRatio );		
		DWORD dwCurHP = 0;
		BOOL bChangeHP = FALSE;;
		if (m_dwHP > 0) 
		{
			bChangeHP = TRUE;
		}

		m_NPCInfo.m_dwMaxHP = dwMaxHP;

		if (bChangeHP)
		{
			SetHP(dwMaxHP);
		}		
	}
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetExp(EXPTYPE exp)
{
	assert(!"제대로 정의를 해주시기 바랍니다. 제가 일단 안넣었습니다. !! - 재영");
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetLevel(LEVELTYPE LV)
{
	assert(!"제대로 정의를 해주시기 바랍니다. 제가 일단 안넣었습니다. !! - 재영");
}

//------------------------------------------------------------------------------
/**
*/
LEVELTYPE Monster::GetLevel()
{
	return m_NPCInfo.m_Level;
}
LEVELTYPE Monster::GetDisplayLevel()
{
	return m_NPCInfo.m_DisplayLevel;
}

//------------------------------------------------------------------------------
/**
*/
EXPTYPE Monster::GetExp()
{
	assert(!"제대로 정의를 해주시기 바랍니다. 제가 일단 안넣었습니다. !! - 재영");
	return 0;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Monster::GetMaxHP()
{
	return m_NPCInfo.m_dwMaxHP;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Monster::GetMaxMP()
{
	return m_NPCInfo.m_dwMaxMP;
}

//------------------------------------------------------------------------------
/**
*/
EXPTYPE Monster::GetNextExp()
{
	assert(!"제대로 정의를 해주시기 바랍니다. 제가 일단 안넣었습니다. !! - 재영");

	return 0;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::RenderMonsterStatus()
{
	if (!g_InterfaceManager.GetShowFlag())
	{
		return;		
	}

	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m216") );

	char monsterinfo[256] = {0,};

	WzVector wzPos = GetPosition();
	WzVector wzVisualPos = GetVisiblePos();

	wzVisualPos.z += 2.0f;

	POINT pt2;
	g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos );

	WzColor color;
	color = WzColor_RGBA(255,255,255,255);


	RECT rc;
	rc.left = pt2.x - 60;
	rc.top = pt2.y;
	rc.right = rc.left + 280;
	rc.bottom = rc.top + 30;

    std::vector<SKILL_EFFECT*>& array = GetSkillEffectArray();
    for( size_t i=0;i!=array.size();++i)
    {
        SKILL_EFFECT* pEffect = array[i];

		_snprintf(monsterinfo, 256-1,"%s", pEffect->GetStateName() );
		g_pSunRenderer->x_pManagerTextOut->DrawTextA(monsterinfo,&rc,WzColor_RGBA(255,255,255,255));

		rc.top += 20;
		rc.bottom += 20;
	}
    
	g_pSunRenderer->Set2DMode( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void Monster::ProcessStandAnimationDelay(DWORD dwTick)
{
	if (m_dwStandAnimDelay < dwTick)
	{
		m_dwStandAnimDelay = 0;
	}
	else 
    {
		m_dwStandAnimDelay -= dwTick;
	}
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetIdleAnimation()
{
	if(MONSTER_TYPE_SETTING_MAPOBJECT == GetMonsterType())
		return;

	if( !IsBlock() )
	{
		// 어택스탠드 때문에 지속적으로 애니메이션세팅을 다시해준다.
        WzID idle_animation_id = GetCurrentIdleAnimation();
        WzID current_animation_id = GetCurrentAnimation();

        if (idle_animation_id != current_animation_id)
		{
            if(idle_animation_id == StrToWzID("N001"))
            {
                SetAnimation(idle_animation_id, FALSE);
			}
            else
            {
                SetAnimation(idle_animation_id, TRUE);
            }
        }
    }
}

//트리거 오브젝트에게 영향을 받는다.
void Monster::SetTriggerMapObjectAnimation()
{
	if(0 == this->GetMapObjectKey())
		return;

	MapObject* pMapObject = (MapObject*)g_ObjectManager.GetObject(MAP_OBJECT_KEY + this->GetMapObjectKey());
	if(pMapObject)
	{
        WzID animation_of_mapobject = pMapObject->GetCurrentTriggerAnimation();
        if((animation_of_mapobject != 0) && 
            (animation_of_mapobject != GetCurrentAnimation()))
        {
            // 맵 오브젝트의 D000(오브젝트의 IDLE)계열의 애니메이션은 루프로 돌린다.
            BOOL is_loop = (animation_of_mapobject == StrToWzID("D000")) ? TRUE : FALSE;

            SetAnimation(pMapObject->GetCurrentTriggerAnimation(), is_loop);
        }
	}
}

//------------------------------------------------------------------------------
/**
*/

void Monster::OnDamage(const  DAMAGE_PRAM &rDamage)
{
	
	DWORD dwAdditionalEffect		= rDamage.pResult->GetTargetData().GetAdditionalEffect();
	
	const float VIBRATE_FORCE = 0.05f;
	
	Character::OnDamage(rDamage);

	SetAttackStandTime(ATTACK_STAND_TIME);

	// 부가 효과 적용

	BOOL bCritical = dwAdditionalEffect & (ATTACK_ADDITIONAL_EFFECT_CRITICAL);

	// 맞는 소리 낸다.
	PlayDamageSound(rDamage.pResult->GetWeaponSound());
	// 맞는 이펙트도 나온다
	CreateDamageEffect(bCritical,rDamage.bRight,rDamage.dwObjKey);
	
	// 밀기류의 스킬이 아니면 맞는 애니메이션을 플레이한다.
	if ( !(dwAdditionalEffect & (ATTACK_ADDITIONAL_EFFECT_KNOCKBACK | ATTACK_ADDITIONAL_EFFECT_KNOCKBACK3 | ATTACK_ADDITIONAL_EFFECT_KNOCKBACK2|ATTACK_ADDITIONAL_EFFECT_DOWN | ATTACK_ADDITIONAL_EFFECT_AIR)) ) 
	{
		switch (GetCurState())
		{
		default:
			{
				if ( !(dwAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_DONOTPLAYANIMATION) )
				{
					PlayDamageAnimation(rDamage.dwObjKey,bCritical);
				}

#ifdef VIBRATE_CHARACTER
				WzVector vDir = GetDirection();
				WzVector vTemp;

				vTemp.x = vDir.x; 
				vTemp.y = vDir.y; 
				vTemp.z = 0.0f;
				Vibrate(vTemp,VIBRATE_FORCE);
#endif
			}
			break;
		case STATE::DEATH:
		case STATE::DOWN:
        case STATE::ATTACKJUMP:
		    {
                // 이 동작중엔 들어가지말아라.!!
			}
			break;
		}		
	}


	if(g_pHero->GetObjectKey() == rDamage.dwObjKey)
	{
		if(MONSTER_TYPE_SETTING_MAPOBJECT == GetMonsterType())
		{
			//////////////////////////////////////////////////////////////////////////
			//마땅히 원래 위치를 얻어올때가 없어 여기서 얻는다
			if(false == m_bGetOriginPos)
			{
				m_wvOriginPos = GetPosition();
				m_bGetOriginPos = true;
			}
			//////////////////////////////////////////////////////////////////////////

			//////////////////////////////////////////////////////////////////////////
			//적정 거리 만큼 민다
			WzVector wvLen = this->GetPosition() - g_pHero->GetPosition();
			WzVector wvResult;
			VectorNormalize(&wvResult, &wvLen);
			wvResult = wvResult*0.05f;
			WzVector wvPos = this->GetPosition() + wvResult;
			SetPosition(wvPos);
			//////////////////////////////////////////////////////////////////////////
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
WzID Monster::GetCurrentBlockAnimation()
{
	
	static const WzID AnimID  = StrToWzID("M001");

	return AnimID;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::PlayBlockAnimation() 
{
	if(MONSTER_TYPE_SETTING_MAPOBJECT == GetMonsterType())
		return;

	// 25% 확률로 블럭애니를한다.
	if (rand() & 2)
	{
		return;
	}

	WzID blockanim = GetCurrentBlockAnimation();
	assert(blockanim);
	
	switch(GetCurState())
	{
	case STATE::IDLE:
		this->SetAnimation(blockanim,FALSE);
		break;

		break;
	}			
}


//------------------------------------------------------------------------------
/**
*/
void Monster::PlayEvadeAnimation()
{

}

//------------------------------------------------------------------------------
/**
*/
float Monster::GetAttackSpeedModifier()
{	
	return m_fAttackSpeedModifier;
}

//------------------------------------------------------------------------------
/**
*/
float Monster::GetMoveSpeedModifier()
{
	return m_fMoveSpeedModifier;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Monster::CannotAir()
{
	return ( Character::CannotAir() || MONSTER_TYPE_SETTING_MAPOBJECT == m_eMonsterType );
}



//------------------------------------------------------------------------------
/**
*/
void Monster::RenderInfo()
{
    if (GENERALPARAM->GetSpecialMode()) 
    {
		RenderMonsterDetailInfo();
	}
	const WzColor GMTextColor = WzColor_RGBA(255, 0, 255, 255); 	

	//	해당 플레이어가 GM 캐릭터인가?
	if (g_pHero && g_pHero->IsGM())
	{
		TCHAR szResult[MAX_PATH];
		//	내가 GM 이면 플레이어 레벨과 오브젝트 키를 표시
		if (g_pHero->GetAccountState())
		{
			Sprintf(szResult, _T("%ld [%d]"), this->GetLevel(), GetObjectKey());
			RenderTextOnHead(szResult, GMTextColor, 0);
		}		
	}

	if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_DOMINATION
#ifdef _NA_008334_20150608_SONNENSCHEIN
        || GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SONNENSCHEIN
#endif //_NA_008334_20150608_SONNENSCHEIN
        )
	{
		if (RELATIONKIND::Enemy == GameFramework::GetCurrentScene()->GetRelationKindOfCharacter(this, false))
        {
			m_IsShowName = FALSE;
        }
	}

	if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ)
    {
		m_IsShowName = FALSE;
    }

    bool is_show_gauge = false;
	BOOL bExceptMonsterType = GetMonsterInfo() && GetMonsterInfo()->m_byGrade == 10;
	if (GAMEOPTION->GetDisplayMonterInfo() && m_IsShowName && !bExceptMonsterType)
	{
		if (g_pCurrentPickedObject != NULL)
		{
			if (g_pCurrentPickedObject != this)
			{
				m_IsShowName = FALSE;
			}
			else if (g_InterfaceManager.GetShowFlag())
			{

#ifdef _DH_NPC_GROUP_NAME
                if (GetMonsterInfo()->group_name != 0)
                {
                    TCHAR temp_string[256];
                    g_InterfaceManager.GetInterfaceString(GetMonsterInfo()->group_name, temp_string, 256);
                    RenderTextOnHeadGroup(temp_string, GetColorText(GetDisplayLevel(), g_pHero->GetLevel()), -37);
                }
#endif//_DH_NPC_GROUP_NAME 

				TCHAR szResult[MAX_PATH];

                if (m_bShadowSummon)
                {
                    //몬스터이름재정의 :속박된 + 몬스터이름
                    // 1724 속박된
                    Sprintf(szResult, _T("[%ld] %s %s"),
                        GetDisplayLevel(), g_InterfaceManager.GetInterfaceString(1724), GetName());
                }
                else
                {
                    Sprintf(szResult, _T("[%ld] %s"), GetDisplayLevel(), GetName());
                }
				RenderTextOnHead(szResult, GetColorText(GetDisplayLevel(), g_pHero->GetLevel()), 0);
                is_show_gauge = true;
			}
		}
	}

    if ((GetMonsterInfo()->m_byGrade == eNPC_FRIEND_MAPOBJECT_NPC) || 
        (GetMonsterInfo()->m_byGrade == eNPC_FRIEND_MONSTER))
    {
        is_show_gauge = true;
    }

    if (is_show_gauge == true)
    {
        RenderHPGuage();
    }

	Object::RenderInfo();
}

void Monster::RenderHPGuage( void )
{
	WzVector wzVisualPos = GetVisiblePos();
	wzVisualPos.z += GetMeshHeight();
	wzVisualPos.z += HEIGHT_GAP;
	
	float fDepth;
	POINT pt2;
	g_pSunRenderer->Set2DMode( TRUE );
	g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos, &fDepth );		

	RECT rc;
	rc.left = pt2.x - ( MONSTER_HP_LINE_WIDTH / 2);
	rc.top = pt2.y - 8;//12;//+ 20 - 22;
	rc.right = rc.left + MONSTER_HP_LINE_WIDTH;
	rc.bottom = rc.top + 17;

	DWORD HPersent = 0;
	if( GetMaxHP() )
	{
		HPersent = static_cast<DWORD>((static_cast<float>(GetHP()) / GetMaxHP()) * 100);
	}
	else
	{
		assert(!"GetMaxHP() == 0");
	}

	HPersent=min(100,HPersent);
	DWORD HPWidth = ( MONSTER_HP_GAUGE_BAR_WIDTH * HPersent ) / 100;

	ENUM_CULL_FACE      eCullFaceMode = g_pSunRenderer->GetCullFaceMode();	
	BOOL bZWrite = g_pSunRenderer->IsEnableZWrite();
	BOOL bTest = g_pSunRenderer->IsEnableZTest();
	g_pSunRenderer->SetZBufferTest( TRUE );
	g_pSunRenderer->SetZBufferWrite(TRUE);

    float gauge_src_offset = 0.0f;
    if ((GetMonsterInfo()->m_byGrade == eNPC_FRIEND_MAPOBJECT_NPC) || 
        (GetMonsterInfo()->m_byGrade == eNPC_FRIEND_MONSTER))
    {
        gauge_src_offset += 6.0f;
    }

	g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hMonsterGauageTexture, (float)rc.left, (float)rc.top, MONSTER_HP_LINE_WIDTH, MONSTER_HP_LINE_HEIGHT,
								   0.0f, 2.0f, MONSTER_HP_LINE_WIDTH, MONSTER_HP_LINE_HEIGHT, 0, fDepth );

	g_pSunRenderer->RenderTexture( g_InterfaceManager.m_hMonsterGauageTexture, (float)rc.left + 3, (float)rc.top + 3, (float)HPWidth, MONSTER_HP_GAUGE_BAR_HEIGHT,
								   3.0f, 14.0f + gauge_src_offset, MONSTER_HP_GAUGE_BAR_WIDTH, MONSTER_HP_GAUGE_BAR_HEIGHT, 0, fDepth );

	g_pSunRenderer->SetZBufferTest( bTest );
	g_pSunRenderer->SetCullFaceMode( eCullFaceMode);
	g_pSunRenderer->SetZBufferWrite( bZWrite );	
	g_pSunRenderer->Set2DMode( FALSE );
}

DWORD Monster::GetColorText(int monsterLv, int HeroLv)
{
	DWORD color;

    switch (GetMyResist())
    {
    case NpcElementWeakness::eElementWeakness_Fire:
        color = RGBATOARGB(255, 29, 75, 255);
        break;
    case NpcElementWeakness::eElementWeakness_Water:
        color = RGBATOARGB(108, 165, 250, 255);
        break;
    case NpcElementWeakness::eElementWeakness_Wind:
        color = RGBATOARGB(79, 210, 226, 255);
        break;
    case NpcElementWeakness::eElementWeakness_Earth:
        color = RGBATOARGB(252, 176, 86, 255);
        break;
    case NpcElementWeakness::eElementWeakness_Dark:
        color = RGBATOARGB(171, 153, 209, 255);
        break;
        //case Monster::kElement_Common:
    default:
        color = RGBATOARGB(189, 198, 198, 255);
        break;
    }
    return color;
}


//------------------------------------------------------------------------------
/**
*/
BOOL Monster::IsRangedNormalAttack()
{
	const float RANGED_THRESHOLD = 6.5f;
	if (GetMonsterInfo()->m_fAttRange >= RANGED_THRESHOLD)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


//------------------------------------------------------------------------------
/**
*/
float Monster::GetMeshHeight()
{
	return m_fMeshHeight;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetAttackSpeedModifier(float fAttSpeed)
{
	m_fAttackSpeedModifier = fAttSpeed;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetMaxHP( DWORD iHP )
{
	m_NPCInfo.m_dwMaxHP = iHP;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetMaxMP( DWORD iMP )
{
	m_NPCInfo.m_dwMaxMP = iMP;
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetAttackSpeedRatio(int iSpeed)
{
   SetAttackSpeedModifier( iSpeed /100.f );				
}

//------------------------------------------------------------------------------
/**
*/
void Monster::SetMoveSpeedRatio(int iSpeed)
{	
   SetMoveSpeedModifier( iSpeed /100.f );
}

//------------------------------------------------------------------------------
/**
*/
WzVector Monster::GetWeaponBonePos(int index)
{
	int boneindex=0;

    if (GetBasePart()->GetWzUnit() != NULL)
    {
	    if(index)
	    {
		    WzBoneInfo * pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(StrToWzID("WEA1"));
		    if (pInfo) 
		    {
			    boneindex= pInfo->m_iNode;
		    }
	    }
	    else
	    {
		    WzBoneInfo * pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(StrToWzID("WEA2"));
		    if (pInfo) 
		    {
			    boneindex= pInfo->m_iNode;
		    }
	    }
    }

	WzVector ret;
	ZeroMemory(&ret,sizeof(ret));

	ret = GetBonePos(boneindex);

	return ret;
}

//----------------------------------------------------------------------------
/**
*/
DWORD Monster::CreateProjectile(Character *pTarget, int iArrowBone)
{
    if( pTarget == NULL )
        return 0;

    DWORD dwProjectileCode = GetAttackSequence() == 1 ? m_NPCInfo.m_wProjectileCode : m_NPCInfo.m_wProjectileCode2;

    BASE_ProjectileInfo* pInfo = ProjectileInfoParser::Instance()->GetProjectileInfo(dwProjectileCode);
    if( pInfo == NULL )
        return 0;


    WzVector StartPosition = GetArrowBonePos(iArrowBone);
    WzVector TargetPosition = pTarget->GetArrowTargetPos();

    WzVector Distance = TargetPosition - StartPosition;
    float fDistance = VectorLength(&Distance);

    DWORD MoveTime = 100;
    if( fDistance <= 0.0f)
    {
        MoveTime = 100;
    }
    else
    {
        MoveTime = max(100, min(500, (DWORD)(fDistance * 50.f) ) );
    }

    cProjectileUnit* pProjectile = new cProjectileUnit();
    pProjectile->SetArrowStartPosition( StartPosition );
    pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
    pProjectile->SetLifeTime( MoveTime );

	if( pInfo->m_dwImageID > 0 )
	{
		pProjectile->SetUnitCode( (eProjectile_SRC)pInfo->m_dwImageID );
	}

	if( pInfo->m_wzidFlyingEffectID.id != StrToWzID("NULL") )
	{
		pProjectile->CreateEffect( pInfo->m_wzidFlyingEffectID.id, GetObjectKey(), pTarget->GetObjectKey() );
	}

    pProjectile->SetHitData( GetObjectKey(), GetCurrentAttackSerial(), pInfo->m_wzidHitEffectID.id, FALSE, 0, TRUE );
    if( pInfo->m_bMakeTrail == TRUE )
    {
        pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, 150, 200, 255 );
    }
    g_ProjectileManagerEx.InsertProjectile( pProjectile );

    return pProjectile->GetKey();
}

void Monster::CreatePopupMesh(
							 CODETYPE SkillCode,
							 INT64 iNumber,
							 eDAMAGEPOPUP_KIND eKind,
							 WzColor color,
							 BOOL bCritical,
							 BOOL bDoubleDamage,
							 float fMultiplier /*= 1.0f*/,
							 int iSecondary /*= -1*/,
							 WzColor secondarycol /*= WzColor_RGBA(0,0,0,0)*/,
							 float fHeight /*= 0.0f*/,
							 BOOL bHero /*= FALSE*/
							 )
{
	BOOL bBySkill = (BOOL)SkillCode;

	eDMGPOP_TARGET_TYPE eTargetType = eDMGPOP_TARGET_MONSTER;
	eDMGPOP_COLOR_TYPE eColorType = eDMGPOP_COLOR_GENERAL;		

	switch( eKind )
	{
	case eDAMAGEPOPUP_ENEMYHPDAMAGE:
	case eDAMAGEPOPUP_ENEMYMPDAMAGE:
	case eDAMAGEPOPUP_PLAYERHPDAMAGE:
	case eDAMAGEPOPUP_PLAYERMPDAMAGE:
		{
			if ( bCritical || bDoubleDamage)
			{
				eColorType = (bBySkill)? eDMGPOP_COLOR_SKILL : eDMGPOP_COLOR_CRITICAL;
			}
			else
			{
				eColorType = (bBySkill)? eDMGPOP_COLOR_SKILL : eDMGPOP_COLOR_GENERAL;
			}
		}		
		break;
	case eDAMAGEPOPUP_HEAL:
		eColorType = eDMGPOP_COLOR_HEAL;	
		break;
	case eDAMAGEPOPUP_EXP:
		eColorType = eDMGPOP_COLOR_EXP;		
		break;
	case eDAMAGEPOPUP_MISS:
		eColorType = eDMGPOP_COLOR_MISS;		
		break;
	case eDAMAGEPOPUP_BLOCK:
		eColorType = eDMGPOP_COLOR_BLOCK;		
		break;
	default:
		break;
	}

	color = c_PopupColorInfo[(int)eTargetType][(int)eColorType];

	Character::CreatePopupMesh(
		SkillCode, 
		iNumber, 
		eKind, 
		color, 
		bCritical, 
		bDoubleDamage,
		fMultiplier,
		iSecondary,
		secondarycol,
		fHeight
		);
}


void Monster::AddDelaySkillSerial(DWORD dwDelaySkill)
{
	m_DelaySkillSerial.push_back(dwDelaySkill);
}


DWORD Monster::GetDelaySkillSerialCount()
{
	return m_DelaySkillSerial.size();
}

DWORD Monster::GetDelaySkillSerial()
{
	if (m_DelaySkillSerial.size()) 
    {
		DWORD ret = m_DelaySkillSerial.front();
		m_DelaySkillSerial.pop_front();
		return ret;
	}

	return 0;
}

const float MONSTER_VOICE_RANGE = 10.0f * 10.0f;

//------------------------------------------------------------------------------
/**
*/
void Monster::PlayIdleVoice()
{
	BASE_MonsterVoiceInfo *pInfo = MonsterVoiceInfoParser::Instance()->GetMonsterVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (pInfo)
	{
		int iVoiceIndex = rand() % MAX_MONSTER_IDLE_VOICE;
		
		TCHAR szText[512];
		ZeroMemory(szText,512);
		DWORD dwTextID = pInfo->m_Idle[iVoiceIndex].dwTextID;

		if (dwTextID) {
			g_InterfaceManager.GetInterfaceString(dwTextID,szText,512);
			AddChatMessage(szText);
		}

		StopVoice();

		
		if (GetRangeWithHero() <= MONSTER_VOICE_RANGE) 
		{
			m_iVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_Idle[iVoiceIndex].dwVoiceID,NULL);
		}
		return;
	};

}

//------------------------------------------------------------------------------
/**
*/
void	Monster::PlayBattleVoice()
{
	BASE_MonsterVoiceInfo *pInfo = MonsterVoiceInfoParser::Instance()->GetMonsterVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (pInfo)
	{
		int iVoiceIndex = rand() % MAX_MONSTER_BATTLE_VOICE;
		
		TCHAR szText[512];
		ZeroMemory(szText,512);
		DWORD dwTextID = pInfo->m_Battle[iVoiceIndex].dwTextID;

		if (dwTextID) 
        {
			g_InterfaceManager.GetInterfaceString(dwTextID,szText,512);
			AddChatMessage(szText);
		}

		StopVoice();

		if (GetRangeWithHero() <= MONSTER_VOICE_RANGE) 
		{	
			m_iVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_Battle[iVoiceIndex].dwVoiceID,NULL);
		}

		return;
	};
}

//----------------------------------------------------------------------------
/**
*/
void Monster::PlaySkillVoice()
{
	BASE_MonsterVoiceInfo *pInfo = MonsterVoiceInfoParser::Instance()->GetMonsterVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (pInfo)
	{
		int iVoiceIndex = rand() % MAX_MONSTER_SKILL_VOICE;
		
		TCHAR szText[512];
		ZeroMemory(szText,512);
		DWORD dwTextID = pInfo->m_Skill[iVoiceIndex].dwTextID;

		if (dwTextID) 
        {
			g_InterfaceManager.GetInterfaceString(dwTextID,szText,512);
			AddChatMessage(szText);
		}

		StopVoice();

		if (GetRangeWithHero() <= MONSTER_VOICE_RANGE) 
		{	
			m_iVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_Skill[iVoiceIndex].dwVoiceID,NULL);
		}

		return;
	};
}

//------------------------------------------------------------------------------
/**
*/
void Monster::PlayDieVoice()
{
	BASE_MonsterVoiceInfo *pInfo = MonsterVoiceInfoParser::Instance()->GetMonsterVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (pInfo)
	{
		int iVoiceIndex = rand() % MAX_MONSTER_DIE_VOICE;
		
		TCHAR szText[512];
		ZeroMemory(szText,512);
		DWORD dwTextID = pInfo->m_Die[iVoiceIndex].dwTextID;

		if (dwTextID) 
        {
			g_InterfaceManager.GetInterfaceString(dwTextID,szText,512);
			AddChatMessage(szText);
		}

		StopVoice();

		if (GetRangeWithHero() <= MONSTER_VOICE_RANGE) 
		{	
			m_iVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_Die[iVoiceIndex].dwVoiceID,NULL);
		}

		return;
	};
}

//----------------------------------------------------------------------------
/**
*/
void Monster::PlayStateVoice()
{
	BASE_MonsterVoiceInfo *pInfo = MonsterVoiceInfoParser::Instance()->GetMonsterVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (pInfo)
	{
		int iVoiceIndex = rand() % MAX_MONSTER_STATE_VOICE;
		
		TCHAR szText[512];
		ZeroMemory(szText,512);
		DWORD dwTextID = pInfo->m_State[iVoiceIndex].dwTextID;

		if (dwTextID) 
        {
			g_InterfaceManager.GetInterfaceString(dwTextID,szText,512);
			AddChatMessage(szText);
		}

		StopVoice();

		if (GetRangeWithHero() <= MONSTER_VOICE_RANGE) 
		{	
			m_iVoiceHandle = g_SoundEffect.PlayVoice(pInfo->m_State[iVoiceIndex].dwVoiceID,NULL);
		}
		return;
	}
}

//----------------------------------------------------------------------------
/**
*/
void  Monster::OnUseSkill()
{
	PlaySkillVoice();
	Character::OnUseSkill();
}

//----------------------------------------------------------------------------
/**
*/
void Monster::OnDead()
{
	PlayDieVoice();
	DestoryEffect();
	Character::OnDead();
}

//----------------------------------------------------------------------------
/**
*/
BOOL Monster::CanUseSkill(SLOTCODE skillID,BOOL bShowFailMessage)
{
	BOOL bCheck = FALSE;

	bCheck = CanUseSkillLimitMP(skillID, bShowFailMessage);
	if ( !bCheck )
	{		
		return FALSE;
	}

	bCheck = CanUseSkillLimitHP(skillID, bShowFailMessage);
	if ( !bCheck )
		return FALSE;

	if( CanUseSkillLimitCoolTime(skillID, bShowFailMessage) == FALSE )
		return FALSE;

	return TRUE;
}
//----------------------------------------------------------------------------
BOOL Monster::CanUseSkillWithoutCooltime(SLOTCODE skillID,BOOL bShowFailMessage)
{
	BOOL bCheck = FALSE;

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
	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
BOOL Monster::CanUseSkillLimitClassDefine(SLOTCODE skillID,BOOL bShowFailMessage /*= FALSE*/)
{
	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)skillID);
	assert(pInfo);
	if (!pInfo)
	{
		return FALSE;
	}

	BASE_NPCINFO *pNpcInfo=GetMonsterInfo();
	assert(pNpcInfo);
	if (!pNpcInfo)
	{
		return FALSE;
	}

	if(pInfo->m_dwClassDefine!=pNpcInfo->m_MonsterCode)
	{
		return FALSE;
	}

	return TRUE;
}

//----------------------------------------------------------------------------
/**
*/
void Monster::OnStartRun()
{
	if (m_iAlarmVoice == 0)
	{
		PlayBattleVoice();
		m_iAlarmVoice = 1;
	}
	Character::OnStartRun();
}

//----------------------------------------------------------------------------
/**
*/
void Monster::ProcessVoice(DWORD dwTick)
{
	m_iWaitSoundDelay -= dwTick;

	if (m_iWaitSoundDelay < 0)
	{
		m_iWaitSoundDelay = 0;
	}

	if (m_iWaitSoundDelay)
	{
		return;
	}

	switch (GetCurState())
	{
	case STATE::IDLE:
		{
			break;
		}
	case STATE::MOVE:
		{			
			// 뛸때는 안함
			if (m_bMoveState == CMS_RUN)
			{
				return;
			}
			break;
		}
	default:
		{
			return;
		}
		break;
	}
	
	if (!g_pHero)
	{
		return;
	}

	BASE_MonsterVoiceInfo *pInfo = MonsterVoiceInfoParser::Instance()->GetMonsterVoiceInfo(m_NPCInfo.m_MonsterCode);

	if (!pInfo)
	{
		return;
	}

	WzVector vDistance;

	vDistance = GetPosition() - g_pHero->GetPosition();

	const float fSightRange = 30.0f;
	const int WAIT_SOUND_DELAY = 60000; // 1분 

	// 시야 안에들어왔으면!
	if ( VectorLength2(&vDistance) <= fSightRange * fSightRange )
	{

		if (m_iWaitSoundDelay == 0) {
			PlayIdleVoice();
			m_iWaitSoundDelay = WAIT_SOUND_DELAY; 
		
		}
	}

	return;
}

//----------------------------------------------------------------------------
/**
*/
WzID Monster::GetCurrentCamAnimation()
{
	return StrToWzID("N001");
}


//-------------------------------------------------------------------------------------------
/**
*/
WzID        
Monster::GetCurrentAttackJumpAnimation()
{
    return StrToWzID("N006");
}

//----------------------------------------------------------------------------
/**
*/
void Monster::CreateEffect()
{

	WzVector pos=GetVisiblePos();

	pos.z+=0.1f;


	switch(m_NPCInfo.m_wClass)
	{
	case eNPC_CLASS_LIFE_SOUL:
		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("J390"),pos,m_dwParentId);		
		break;

	case eNPC_CLASS_WAR_BATTLER:
		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("J340"),GetVisiblePos(),m_dwParentId);		
		break;

	case eNPC_CLASS_DESTROYER:
		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("J460"),GetVisiblePos(),m_dwParentId);		
		break;
	}

}

//----------------------------------------------------------------------------
/**
*/
void Monster::DestoryEffect()
{
	WzVector pos=GetVisiblePos();

	switch(m_NPCInfo.m_wClass)
	{
	case eNPC_CLASS_DESTROYER:
		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("J464"),pos,m_dwParentId);		
		break;
	}

}

WzID Monster::GetTrailEffectCode()
{
	return StrToWzID("aarx");
}

//----------------------------------------------------------------------------
/**
	몬스터는 통메쉬에서 찾아야 한다.
*/

WzVector Monster::GetTrailBonePos(int index,int start)
{
	WzVector vRet;
	ZeroMemory(&vRet,sizeof(vRet));

	index=min(index,MAX_TRAIL-1);
	index=max(0,index);


	int iAttachIndex = GetTrailEffectAttachmentIndex(index);

	iAttachIndex=min(iAttachIndex, (int)GetAttachmentCount()-1);
	iAttachIndex=max(0,iAttachIndex);

	int startboneindex = -1;
	int endboneindex = -1;	

	int boneindex = -1;
	

	if(GetBasePart())
	{
		CWzUnit *pUnit = GetBasePart()->GetWzUnit();

		if (pUnit) 
		{		 
			WzBoneInfo *pInfo = pUnit->GetBoneInfo(StrToWzID("EFF1"));
			if (pInfo)
			{
				startboneindex = pInfo->m_iNode;
			}

			pInfo = pUnit->GetBoneInfo(StrToWzID("EFF2"));
			if (pInfo)
			{
				endboneindex = pInfo->m_iNode;
			}

			int boneindex;
			if (start) 
			{
				boneindex = startboneindex;
			}
			else
			{
				boneindex = endboneindex;
			}

			if (boneindex != -1)
				vRet =GetBonePos(boneindex);		
		}

	}

	return vRet;
}
//----------------------------------------------------------------------------
/**
	싱글모드에서 스스로 움직인다.
*/
void Monster::ProcessSingleMove()
{
	if(m_bSingleMove && this->GetCurState()==STATE::IDLE&&this->IsMove()==FALSE) 
	{
		float fAngle = (rand() % 628) / 100.0f;
		ClampAngle(&fAngle);
		WzVector vDirection;
		vDirection.x = sin(fAngle);
		vDirection.y = -cos(fAngle);
		vDirection.z = 0.0f;

		float fRate=5.0f;
		fRate+=rand()%20;

		vDirection*=fRate;
		WzVector vPos=g_pHero->GetPosition()+vDirection;

		vPos.z+=3.0;

		int iTileFocussing = -1;
		float fT=0.0f;
		iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vPos, &fT, -1.0f, 10.0f );

		if(iTileFocussing >=0)
		{
			vPos.z-=fT;
			PLAYER_ACTION action;
			ZeroMemory(&action,sizeof(action));
			action.ActionID = ACTION_MOVE;
			action.MOVE.byState =CMS_WALK ;
			action.MOVE.bThrust = FALSE;
			action.MOVE.vCurPos.wvPos = this->GetPosition();
			action.MOVE.vCurPos.wTile = -1;
			action.MOVE.vDestPos.wvPos = vPos;
			action.MOVE.vDestPos.wTile = -1;
			PutAction(&action);	

		}
	}
}
//------------------------------------------------------------------------------ 
void Monster::DestorySummonIntensifyEffect()
{
    if (m_hSummonIntensifyEffect != INVALID_HANDLE_VALUE)
    {
        g_pEffectManager->RemoveEffectPtr(m_hSummonIntensifyEffect);
    }
    m_hSummonIntensifyEffect = INVALID_HANDLE_VALUE;
}
//------------------------------------------------------------------------------ 
HANDLE Monster::CreateSummonIntensifyEffect(DWORD effect_id)
{
    if (m_hSummonIntensifyEffect == INVALID_HANDLE_VALUE)
    {
        if (IsDead() == FALSE)
        {
            m_hSummonIntensifyEffect = GlobalFunc::CreateEffectBeUpToOption(effect_id, GetBonePos(0));
        }
    }

    return m_hSummonIntensifyEffect;
}

void Monster::SetAnimation(WzID animation_id, BOOL is_loop, BOOL bRestart/* = TRUE*/)
{
    if (IsCentipedeMonster() == true)
    {
        SetAnimationForCentipedeMonster(animation_id, is_loop, bRestart);
    }
    else
    {
        Object::SetAnimation(animation_id, is_loop, bRestart);
    }    
}

void Monster::SetAnimationForCentipedeMonster(WzID animation_id, BOOL is_loop, BOOL bRestart/* = TRUE*/)
{
    // Death 관련 애니매이션은 바로 적용해준다.
    if ((animation_id == StrToWzID("D001")) || 
        (animation_id == StrToWzID("D002")))
    {
        set_stand_state(kStand_State);
        Object::SetAnimation(animation_id, is_loop, bRestart);
        return;
    }

    bool is_burrow_state_animation = true;
    if ((animation_id != m_dwStandAniID) &&
        (animation_id != m_dwIdleAniID) && 
        (animation_id != StrToWzID("N004")))
    {
        is_burrow_state_animation = false;
    }

    switch (stand_state())
    {
    case kStand_State:
        {
            // 지면 위에 서있는 상태
            if (is_burrow_state_animation == true)
            {
                animation_id = StrToWzID("N007");
                is_loop = FALSE;
                set_stand_state(kStand_Down_State);
            }
            Object::SetAnimation(animation_id, is_loop, bRestart);
        }
        break;
    case kStand_Down_State:
        {
            // 지면 속으로 파고드는 상태
            if (is_burrow_state_animation == false)
            {
                animation_id = StrToWzID("N006");
                is_loop = FALSE;
                set_stand_state(kStand_Up_State);
                Object::SetAnimation(animation_id, is_loop, bRestart);
            }
        }
        break;
    case kStand_Up_State:
        {
            // 지면 위로 올라서는 상태 - 처리 없음
        }
        break;
    case kBurrow_State:
        {
            // 지면 속에 파고든 상태
            if (is_burrow_state_animation == false)
            {
                animation_id = StrToWzID("N006");
                is_loop = FALSE;
                set_stand_state(kStand_Up_State);
                m_iAttackStandTime = ATTACK_STAND_TIME;
            }
            Object::SetAnimation(animation_id, is_loop, bRestart);
        }
        break;
    }
}

void Monster::ProcessAnimationForCentipedeMonster()
{
    switch (stand_state())
    {
    case kStand_Down_State:
        {
            // 지면 속으로 파고드는 상태
            Object::SetAnimation(m_dwStandAniID, TRUE);
            set_stand_state(kBurrow_State);
            stand_state_ = kBurrow_State;
        }
        break;
    case kStand_Up_State:
        {
            // 지면 위로 올라서는 상태
            Object::SetAnimation(m_dwAttackStandAniID, TRUE);
            set_stand_state(kStand_State);
            m_iAttackStandTime = ATTACK_STAND_TIME;
        }
        break;
    }
}

float Monster::GetBodyRadius()
{
	if( BASE_NPCINFO* pMonsterInfo = GetMonsterInfo() )
        return pMonsterInfo->m_fBodyRadius;

	return 0.0f;	// default
}

void Monster::StartDeathMove()
{
    if( m_DeathSkillCode == 0 )
        return;

    SkillScriptInfo* pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo(m_DeathSkillCode);
    if( pSkillInfo == NULL )
        return;

#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if( pSkillInfo->m_dwClassDefine != eCHAR_HELLROID )
        return;

    m_IsDeathMove = TRUE;

    static float s_move_distance = -2.0f;

    WzVector CurrentPosition = GetPosition();
    m_DeathMoveTargetPosition = CurrentPosition + GetDirection() * s_move_distance;

    Move( CurrentPosition, m_DeathMoveTargetPosition, FALSE, FALSE );
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
}


BOOL Monster::ProcessDeathMove(DWORD dwTick)
{
    if( m_IsDeathMove == TRUE )
    {
        static float s_move_speed = 10.0f;

        float fMoveSpeed = s_move_speed * SPEED_MULTIPLIER * (float)dwTick;
        if( g_pSunTerrain->PathExp_Progress(GetPathExplorer(), fMoveSpeed, NULL ) == FALSE )
        {
            m_IsDeathMove = FALSE;
        }

        return TRUE;
    }

    return FALSE;
}

void Monster::EndDeathMove()
{

}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
void Monster::OnWitchBladeImmoveSummon()
{
    m_bWitchBladeSummon = TRUE;    

    // 나중에 소환된 몬스터가에 연출 할 상황이 있을 때 밑에 구현해 준다.
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE