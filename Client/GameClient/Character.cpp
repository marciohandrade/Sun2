//------------------------------------------------------------------------------
//  Character.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"

#include <PlayerAttributes.h>
#include "Character.h"
#include "ObjectManager.h"
#include "InterfaceManager.h"
#include "Player.h"
#include "Monster.h"
#include "NPC.h"
#include <SCItemSlot.h>
#include "Map.h"

#include "globalfunc.h"                  


#include "CollectionParser.h"
#include "State/StateCollect.h"
#include "Hero.h"
#include "CooltimeManager.h"
#include "Gamefunc.h"
#include "ChatDialog.h"
#include "SkillInfoParser.h"
#include "ProjectileManagerEx.h"
#include "HeroActionInput.h"
#include "SoundEffect.h"
#include "ResourceManager.h"
#include "HeadMeshInfoParser.h"
#include "StateInfoParser.h"
#include "SkillEffectInfoParser.h"
#include "Camera.h"
#include "GameConst_Color.h"
#include "Map.h"
#include "SceneBase.h"
#include "Clone.h"
#include "CloneManager.h"
#include "Application.h"
#include "uiGuildMan/uiGuildMan.h"
#include "State/SkillStateFactory.h"
#include "EquipmentContainer.h"
#include "ItemInfoParser.h"
#include "LoadScene.h"
#include "ItemAttrCalculator.h"
#include "ItemManager.h"
#include "AppearanceResourceCode.h"

#include "HeroEquipmentContainer.h"
#include "uiPartyMan/uiPartyMan.h"
#include "SkillEffectFunc.h"

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
  #include "SpaZoneInfoParser.h"
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

extern DWORD g_CurTime;
extern float g_fUpperForce;
extern float g_fGravity;

//------------------------------------------------------------------------------
/**
*/
Character::Character():
m_dwTargetObjectKey     ( 0),
m_fMoveSpeed		    ( 0.1f),
m_fAttackRange		    ( 4.0f),
m_fReachNPCRange	    ( 5.f),
m_dwHP                  ( 1),
m_iAttackStandTime      ( 0),
m_dwMoveTarget          ( 0),
m_dwLatestAttack        ( 0),
m_dwLatestQueueAttack   ( 0),	
m_fSpinForce            ( 0.0f),
m_dwTerrainAttribute    ( 0),
m_fMeshHeight           ( 1.0f),
m_dwNextActionAttackSerial ( 0),
m_iEmoteIcon            ( 0),
m_iEmoteIconDelay       ( 0),
m_iDeathTimer           ( 0),
m_dwAttackEffectTime    ( 0),    
m_CurrentEmotionAnimationID ( 0),
m_iTextFadeCurDelay     ( 0),
m_bFade                 ( FALSE),
m_bNoCull               ( FALSE),
m_bCameraAlpha          ( FALSE),
m_bVibrateMove          ( FALSE),
m_fVibrateCurrent       ( 0.0f),
m_bDeathTimer           ( FALSE),
m_pEmoteDraw            ( NULL),
m_fAlpha				( 255.0f),
m_bShield               ( FALSE),    
m_dwHideTime            (0),
m_bHide                 (FALSE),
m_dwCurColorTime        (0), 
m_nSeekNodeCount		(20000),
#ifdef _YJ_FISHING_SYSTEM_000933
m_wiCurFishingAniID		( 0),
#endif //_YJ_FISHING_SYSTEM_000933

m_reserveHP             ( 0),
m_dwSD					( 0),
m_hWaterEllipse         ( INVALID_HANDLE_VALUE )

{
    m_IsPolymorphApperance = FALSE;
    m_OldTerrainColor=WzColor_RGBA(255,255,255,255);      
    
	m_listTargeter.clear();

	m_vNotProcessedList.clear();

	m_wvOldVisiblePos.x = 0.0f;
	m_wvOldVisiblePos.y = 0.0f;
	m_wvOldVisiblePos.z = 0.0f;

	m_fHeight = 0.0f;
	m_byFloor = 0;
	m_FreeMove = FALSE;
	m_ForceTurn = FORCE_TURN_NONE;

	m_bShowName = FALSE;
	m_fVisibleAngleZ = 0.0f;
	m_fVisibleAngleModifier = 0.0f;


	m_TransformData.Init();

    m_ReflectDamageList.clear();

    NormalAttack.ani_type = 0;
    NormalAttack.combo_max_count = 3;
}

void Character::SetShow(BOOL bFlag)
{
    Object::SetShow(bFlag);

    ShowFullsetEffect(bFlag);
    RefreshFullsetEffect(this);
}

void Character::SetAngle(float fAngleX, float fAngleY, float fAngleZ)
{
	Object::SetAngle( fAngleX, fAngleY, fAngleZ );

	m_fVisibleAngleZ = fAngleZ;
}

void Character::Rotate(float fAngle, BOOL bSetVisibleAngle /* = FALSE */ )
{
	Object::Rotate( fAngle, bSetVisibleAngle );

	if( bSetVisibleAngle == TRUE )
	{
		m_fVisibleAngleZ = GetAngle();
	}
}

void Character::SetAngle( float fAngle, BOOL bSetVisibleAngle )
{
	Object::SetAngle( fAngle, bSetVisibleAngle );

	if( bSetVisibleAngle == TRUE )
	{
		m_fVisibleAngleZ = fAngle;
	}
}


//------------------------------------------------------------------------------
/**
	원거리 공격 
*/
DWORD Character::CreateProjectile(Character *pTarget,int iArrowBone)
{
	cProjectileUnit* pProjectile = new cProjectileUnit();
	pProjectile->SetLifeTime( 300 );
	pProjectile->SetArrowStartPosition( GetArrowBonePos(0) );

	if( Character* pTarget = (Character*)GetTargetObject() )
	{
		pProjectile->SetArrowTargetObjectKey(pTarget->GetObjectKey(), TRUE );
	}
	else
	{
		pProjectile->SetArrowTargetPosition( GetArrowBonePos(0) + GetDirection() * 4.0f );
	}

	pProjectile->SetUnitCode( eProjectile_SRC_ARROW );
	pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, 100, 150, 255 );
	pProjectile->SetHitData( GetObjectKey(), GetCurrentAttackSerial(), 0, FALSE, 0, FALSE );

	g_ProjectileManagerEx.InsertProjectile( pProjectile );

	return pProjectile->GetKey();
}

//-------------------------------------------------------------------------------------------
/**
*/
void Character::SetStateTotalInfo( STATE_TOTAL_INFO & rStateTotalInfo )
{
    DeleteAllSkillEffect();

    for ( int i = 0; i < rStateTotalInfo.m_Count; ++i )
    {
        STATE_INFO * pStateInfo = &rStateTotalInfo.m_Slot[i];
        if (NULL == pStateInfo) 
        {
            ASSERT(0);
            continue;
        }

        SkillScriptInfo * pSkillInfo = 
            SkillInfoParser::Instance()->GetSkillInfo(pStateInfo->m_SkillCode);
        if (!pSkillInfo)
        {
            ASSERT(0);
            continue;
        }

        BASE_ABILITYINFO * pAbilityInfo = 
            pSkillInfo->GetAbilityInfoByIndex(pStateInfo->m_byAbilityIndex);
        if ( !pAbilityInfo )
        {
            ASSERT(0);
            continue;
        }

        int remain_time;
#ifdef __NA_0_20080618_STATE_INFO_SIZE_DECREASE
        if( pStateInfo->m_wLeavedTime == (WORD)-1 )
            remain_time = -1;
        else
            remain_time = pStateInfo->m_wLeavedTime * SEC_PER_TICK;
#else
        remain_time = pStateInfo->m_iLeavedTime * SEC_PER_TICK;
#endif


        if( SKILL_EFFECT* pEffect = SKILL_EFFECT::CreateSkillStatusEffect( pAbilityInfo->m_wStateID, pStateInfo->m_SkillCode, remain_time, pAbilityInfo ) )
        {
            pEffect->CreateEffect( this, 0 );
            AddSkillEffect(pEffect);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetCondition(BYTE byCondition)
{
	m_byCondition = byCondition;
}

//------------------------------------------------------------------------------
/**
*/
Character::~Character()
{
	m_listTargeter.clear();

	m_vNotProcessedList.clear();

	if( m_hWaterEllipse != INVALID_HANDLE_VALUE )
	{
		g_pEffectManager->RemoveEffectPtr( m_hWaterEllipse );
		m_hWaterEllipse = INVALID_HANDLE_VALUE;
	}

    // 캐릭터가 사라질때 가지고 있는 액션은 모두 처리 한다.
    ProcessAllRemainActionResult();
}



//------------------------------------------------------------------------------
/**
*/
BOOL Character::Create( DWORD dwObjectKey, DWORD dwID )
{
    if (!Object::Create(dwObjectKey, dwID))
	{
		return FALSE;
	}

	////if(m_transFormResourceCode)
	////	SetAppearanceKind(APPEARANCEINFO::PLAYERTRANSFORM);
	//m_TransformData.m_TransFormType = eTRANSFORM_TYPE_NONE;
	//m_TransformData.m_OldTransFormType = m_TransFormType;

	m_TransformData.Init();


	SetVector(&m_wvOldVisiblePos,0.0f,0.0f,0.0f);
	SetAngle(0.0f,TRUE);

	m_fVisibleAngleModifier = 0.0f;
	m_dwLockedTargetID      = 0;
	m_bLocked               = FALSE;
	m_bLockedPosition       = FALSE;

	SetVector(&m_vNormal,0.0f,0.0f,0.0f);
	SetVector(&m_wvInterpolateOffset,0.0f,0.0f,0.0f);
	SetVector(&m_vJumpFrom,0.0f,0.0f,0.0f);

	m_bJump                 = FALSE;
	m_dwLatestAttack        = 0;
	m_dwAttackSerial        = 1;
	m_dwNextAttackSerial    = 1;

	m_bAttackMove = FALSE;
	m_bNextMove   = FALSE;

	ZeroMemory(&m_vAttackStartPos,sizeof(m_vAttackStartPos));
	ZeroMemory(&m_vAttackEndPos,sizeof(m_vAttackEndPos));
	
	m_bMoveState = CMS_RUN;
	m_dwHitPause = 0;
	m_fVibrateForce = 0.0f;
	m_dwHP      = 1;
	m_iFightingEnergy = 0;
	m_iEmoteIcon = 0;

	// 사운드
	m_iVoiceHandle  = -1;
	m_iVoiceKind    = 0;

	EmptyAttackResultList();
	m_ActionQueue.empty();
	//InitializeState();

	PLAYER_ACTION idleaction;
	idleaction.ActionID = ACTION_IDLE;
	idleaction.IDLE.vCurPos = GetPosition();
	SetCurrentAction(&idleaction);

	m_SkillEffectArray.clear();
	m_vNearPos=c_ZeroVector;

	m_dwSpecialMoveCurTime=0;
	m_bSpecialMoveMove=FALSE;
	m_bSitting=FALSE;
#ifdef _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
    sitting_packet_delay_ = FALSE;
#endif //_NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
	m_dwAttackEffectTime=0;
	m_fAlpha= 255.0f;

	EnableCustomLight( TRUE );

	m_bObserverMode = FALSE;        // 옵저버 모드
	m_dwHideTime = 0;
	m_bHide = FALSE;

    m_ReflectDamageList.clear();

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    DestroyViceSwordWeaponEffect();
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
void Character::InitMeshHeight()
{
    if( WzBoundingVolume* pBV = GetBoundingVolume() )
    {
        if (pBV->m_eBvType == BVT_AABB)
        {
            // 공중에 떠있는 몹들때문에 maxz로 대신 사용
            float fHeight = (pBV->m_aabb.m_wvMax.z - pBV->m_aabb.m_wvMin.z);

            SetMeshHeight(fHeight);
        }
    }
}

//------------------------------------------------------------------------------
/**
*/
void Character::Destroy()
{
    ReleaseFullsetEffect();

	FreeEmoteIcon();
	ReleaseMoveTarget();

	if (g_pMap && g_pMap->IsLoaded())
	{
		RemoveObjectToOctree();
	}

	DeleteAllSkillEffect();

	m_listDamageEffectHndl.clear();

	Object::Destroy();

    ReleaseShieldEffect();

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    DestroyViceSwordWeaponEffect();
    cPlayerFPFullEffect::DestroyFPFullEffect();
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}

//------------------------------------------------------------------------------
/**
	타겟의 위치가 설수 없는 타일일 경우 근처 포지션을 구해 준다. 
	Todo:vTemp가 갈수 없는 곳일 경우 방향 값을 회전 시키며 구하는 로직이 필요 

*/
WzVector Character::GetNearTargetPos(WzVector vSrc)
{
	if(g_pSunTerrain&&g_pPathExplorer&&
	   m_vNearPos==c_ZeroVector)
	{
        // 이동 가능한 근접 위치 탐색
        WzVector target_vector =GetPosition(); //타겟 위치
        WzVector direct_vector = vSrc - target_vector;  //타겟의 방향

        VectorNormalize(&direct_vector, &direct_vector);

        WzVector check_vector; //체크 지점

        //원형으로 체크 할 때 순서 조절
        WzVector modified_dir; // 수정된 방향
        static float angle_turn[8] = {0.0f, D3DX_PI*1/4, -D3DX_PI*1/4, D3DX_PI*1/2,
            -D3DX_PI*1/2, D3DX_PI*3/4, -D3DX_PI*3/4, D3DX_PI};

        for (int index = 0; index < 8; ++index)
        {
            for (float add_distance = 1.0f; add_distance <= 3.0f; add_distance += 0.5f)
            {
                // 원형으로 방향 체크
                float theta = angle_turn[index];
                modified_dir.x = cosf(theta)*direct_vector.x + sinf(theta)*direct_vector.y;
                modified_dir.y = -1*sinf(theta)*direct_vector.x + cosf(theta)*direct_vector.y;
                modified_dir.z = direct_vector.z;

                check_vector = target_vector + (modified_dir * add_distance);
                g_pSunTerrain->PathExp_Reset(g_pPathExplorer,&check_vector,-1);

                g_pPathExplorer->Stop();

                float length = VectorLength(&check_vector);

                WzVector long_offset_vector = modified_dir*length*-1;

                g_pSunTerrain->PathExp_Thrust(g_pPathExplorer,&long_offset_vector, PTA_NO_WALK);

                g_pSunTerrain->PathExp_Progress(g_pPathExplorer,1000000,NULL);

                WzVector new_pos_vector = g_pPathExplorer->GetPos();

                // 이동 가능한 지역인지 체크
                int tile_number = g_pSunTerrain->x_pPathFinder->GetTileToStand(new_pos_vector, NULL, -1.0f, 10.0f );
                if (tile_number < 0)
                {
                    continue;
                }
                if (g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(tile_number) & PTA_NO_WALK)
                {
                    continue;
                }

                WzVector length_vector = new_pos_vector - m_vNearPos;

                length = VectorLength(&length_vector);
                //짧게 덜컥 거리는거 방지 
                if(length < 0.3f)
                {
                    continue;
                }
                m_vNearPos = new_pos_vector;
                return m_vNearPos;
            }
        }
    }

	return m_vNearPos;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::Process(DWORD dwTick)
{
	Object::Process(dwTick);
	DWORD processTick;

#ifdef _YJ_MOD_UNIT_PROCESS
	if( IsEnableDisplay() )
#endif //_YJ_MOD_UNIT_PROCESS
	{
		SUNPROCESSPROFILE_BEGIN(_T("ProcessCharacterEtc"));
		ProcessSkillEffect(dwTick);
		ProcessEmoteDelay(dwTick);
		ProcessRushClone();

		if (m_iAttackStandTime)
		{
			m_iAttackStandTime -= dwTick;
			if (m_iAttackStandTime < 0)
			{
				m_iAttackStandTime = 0;
			}
		}

		if(m_dwHideTime>dwTick)
		{
			m_dwHideTime-=dwTick;		
			m_bHide=TRUE;
		}
		else
		{
			if(m_bHide==TRUE)
			{
				SetFadeIn(1000);											
			}
			m_bHide=FALSE;
			m_dwHideTime=0;
		}
		SUNPROCESSPROFILE_END(_T("ProcessCharacterEtc"));
	} //if(GetVisible())

	if(IsMove())	//움직이는 놈들은 갱신 해준다.
		m_vNearPos=c_ZeroVector;

	m_dwAttackEffectTime-= dwTick;
	m_dwAttackEffectTime=max(m_dwAttackEffectTime,0);

	if (m_bFade)
	{
		m_iFadeCurDelay += dwTick;

		if (m_iFadeCurDelay >= m_iFadeTotDelay)
		{
			m_bFade = FALSE;
		}
	}


	float fProcessMultiplier = GetProcessMultiplier();
	// 캐스팅 오차를 줄이자
	if (fProcessMultiplier <= 1.0f)
	{
		processTick = dwTick;
	}
	else
	{
		processTick = (DWORD)((float)dwTick * GetProcessMultiplier());
	}

#ifdef _YJ_MOD_UNIT_PROCESS
	if( IsEnableDisplay() )
	{
		SUNPROCESSPROFILE_BEGIN(_T("ProcessCharacterState"));
		StateProcess( g_CurTime, processTick );
		SUNPROCESSPROFILE_END(_T("ProcessCharacterState"));
	}
	else
	{
		SUNPROCESSPROFILE_BEGIN(_T("CharacterOnlyMove"));
		if(ACTION_MOVE == GetCurrentAction()->ActionID || ACTION_ATTACK == GetCurrentAction()->ActionID)
			StateProcess( g_CurTime, processTick );
		else
			ProcessInterpolation(dwTick);
		SUNPROCESSPROFILE_END(_T("CharacterOnlyMove"));
	}
#else
	StateProcess( g_CurTime, processTick );
#endif //_YJ_MOD_UNIT_PROCESS

	ProcessInterpolationAttachment( dwTick );
	
	return TRUE;
}
  
//------------------------------------------------------------------------------
/**
*/
void Character::ProcessTextFade( DWORD dwTick )
{
	return;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::MakePathPacket( MSG_CG_SYNC_MOVE_SYN *pPacket)
{
	if( pPacket == NULL )
	{
		return FALSE;
	}

	if (!m_pPathExplorer->IsMoving())
	{
		return FALSE;
	}

	WzVector vCurPos = m_pPathExplorer->GetPos();
	WzVector vTarget = m_pPathExplorer->GetTargetPos();


	pPacket->m_byCategory = CG_SYNC;
	pPacket->m_byProtocol = CG_SYNC_MOVE_SYN;
	//	이동 데이터 얻기


#if WZENGINEVER >= 470
    int iPath[_MAX_PATHROUTE_SIZE];
    pPacket->m_byNumRoute = m_pPathExplorer->GetTileRoute( iPath, _MAX_PATHROUTE_SIZE );		

    for (int a = 0; a < pPacket->m_byNumRoute; a++)
    {
        pPacket->m_iRoute[a] = iPath[a];
    }

#else
    // 임시로 예외처리를 해 두었으나, 향후 제거되어야 할 코드
    int iPath[MAX_PATH_ROUTE > _MAX_PATHROUTE_SIZE ? MAX_PATH_ROUTE : _MAX_PATHROUTE_SIZE];
    pPacket->m_byNumRoute = m_pPathExplorer->GetTileRoute( iPath );

    for (int a = 0; a < pPacket->m_byNumRoute && a < _MAX_PATHROUTE_SIZE; a++)
    {
        pPacket->m_iRoute[a] = iPath[a];
    }
#endif

	pPacket->wvCurPos = vCurPos;
	pPacket->wvDestPos = vTarget;

	m_vLastSend = vTarget;

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::MakeTargetPathPacket( MSG_CG_SYNC_TARGET_MOVE_SYN *pPacket )
{
	if( pPacket == NULL )
	{
		return FALSE;
	}

	if (!m_pPathExplorer->IsMoving())
	{
		return FALSE;
	}

	WzVector vCurPos = m_pPathExplorer->GetPos();
	WzVector vTarget = m_pPathExplorer->GetTargetPos();

	pPacket->m_byCategory = CG_SYNC;
	pPacket->m_byProtocol = CG_SYNC_TARGET_MOVE_SYN;
	//	이동 데이터 얻기

#if WZENGINEVER >= 470
    int iPath[_MAX_PATHROUTE_SIZE];
	pPacket->m_byNumRoute = m_pPathExplorer->GetTileRoute( iPath, _MAX_PATHROUTE_SIZE );		

    for (int a = 0; a < (int)(pPacket->m_byNumRoute); a++)
    {
        pPacket->m_iRoute[a] = iPath[a];
    }

#else
    // 임시로 예외처리를 해 두었으나, 향후 제거되어야 할 코드
    int iPath[MAX_PATH_ROUTE > _MAX_PATHROUTE_SIZE ? MAX_PATH_ROUTE : _MAX_PATHROUTE_SIZE];
    pPacket->m_byNumRoute = m_pPathExplorer->GetTileRoute( iPath );

	for (int a = 0; a < (int)(pPacket->m_byNumRoute) && a < _MAX_PATHROUTE_SIZE; a++)
	{
		pPacket->m_iRoute[a] = iPath[a];
	}
#endif
	
	pPacket->wvCurPos = vCurPos;
	pPacket->wvDestPos = vTarget;

	m_vLastSend = vTarget;

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Character::StopMove( void )
{
    if( m_pPathExplorer->IsMoving() )
    {
        m_pPathExplorer->Stop();
    }
}

//------------------------------------------------------------------------------
BOOL Character::IsMyHero()
{
    if( g_pHero != NULL )
    {
        if( g_pHero->GetObjectKey() == GetObjectKey() )
            return TRUE;
    }

    return FALSE;
}

//------------------------------------------------------------------------------
BOOL Character::IsPartyMember()
{
	return GlobalFunc::IsParty( GetObjectKey() );
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsMoving( void )
{
	return m_pPathExplorer->IsMoving();
}

//------------------------------------------------------------------------------
/**
*/
inline BOOL Outside_Plane_AABB( WzPlane *pwp, BvAABB *pbaabb)
{
	const WzVector* pwvSrcMin = &pbaabb->m_wvMin;
	const WzVector* pwvSrcMax = &pbaabb->m_wvMax;		

	static WzVector wvMin;
	wvMin.x = ( pwp->m_wvNormal.x < 0.0f) ? pwvSrcMax->x : pwvSrcMin->x;
	wvMin.y = ( pwp->m_wvNormal.y < 0.0f) ? pwvSrcMax->y : pwvSrcMin->y;
	wvMin.z = ( pwp->m_wvNormal.z < 0.0f) ? pwvSrcMax->z : pwvSrcMin->z;

	// 면과의 관계 검사
	if( Distance_Plane_Vertex( pwp, wvMin) > 0.0f)
	{
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::Render( void )
{

    // GetTerrainMesh가 너무느려서 Debug에선 사용하지 않는다.
    
    WzBoundingVolume *pVolume = GetBoundingVolume();
    if (pVolume && !m_bNoCull)
    {
        WzBoundingVolume bvVolume = *pVolume;
        St_Camera *pCameraInfo = g_pSunRenderer->GetCameraInfo();

        if (pCameraInfo)
        {
            WzPlane nearplane = pCameraInfo->m_wfFrustum.m_wpPlanes[FP_NEAR];

            nearplane.m_wvNormal = -nearplane.m_wvNormal;

            WzVector vPos = 
                (pCameraInfo->m_wvDir * pCameraInfo->m_fNear) + pCameraInfo->m_wvFrom;
            if (Contain_Bv_Vertex(&bvVolume,vPos))
            {
                return FALSE;
            }
        }
    }

    // 이부분은 정확한 의미를 파악해서 작성하자
    // 내가 보기엔 선택 / 피킹시에는 알파를 적용 안할려고 그런것같은데..
    BOOL bBright = FALSE;
    BOOL bRet = FALSE;
    if (g_HeroInput.GetPickedObjectKey() == GetObjectKey())
    {
        bBright = TRUE;
    }
             
    if (g_HeroInput.GetCurrentTarget() == GetObjectKey())
    {
        bBright = TRUE;
    }

	SUNPROFILE_BEGIN( _T("CharApplyTerrainColor") );
	ApplyTerrainColor(TRUE);	
	SUNPROFILE_END( _T("CharApplyTerrainColor") );

	//현재 히어로만 처리 된다.
    if (m_bCameraAlpha)
    {
        WzColor color = GetColor();
        int iAlpha=(int)m_fAlpha;
        SetColor(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),iAlpha);
    }


	if(m_bHide)
	{
		WzColor color = GetColor();
		SetColor(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),0);
	}
	
    WzColor oldcolor = g_pSunRenderer->GetAmbientColor();

    if (g_pMap)
    {
        if (g_pMap->IsApplyAnotherLight())
        {
            g_pSunRenderer->SetColor(g_pMap->GetCharacterAmbientColor(),WRIC_AMBIENT);
        }
    }

	bool IsHero = false;
	if(g_pHero)
	{
		if(GetObjectKey() == g_pHero->GetObjectKey())
		{
			IsHero = true;
		}
	}

	if(IsHero && m_fAlpha <= 0.0f) // 알파값이 0.0보다 작고 Hero이면 그리지 않는다.
	{
		_asm nop;
	}
	else
	{
		StartExtraEffect();

		bRet = Object::Render();	
	   
		EndExtraEffect();
	}

    if (m_pEmoteDraw)
    {
        WzVector vPos = GetVisiblePos();

        vPos.z += GetMeshHeight() + 0.2f;
        m_pEmoteDraw->SetPosition(vPos,FALSE);
        g_pSunRenderer->AnimateUnit(m_pEmoteDraw);
        g_pSunRenderer->RenderUnit(m_pEmoteDraw);
    }


    g_pSunRenderer->SetColor(oldcolor,WRIC_AMBIENT);
				
	return bRet;
    
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::RenderOutline( const WzColor& color , float fThickness )
{
	// GetTerrainMesh가 너무느려서 Debug에선 사용하지 않는다.

    if ( m_bFade || m_bHide || m_bObserverMode )
    {
        return FALSE;
    }

	if ( WzBoundingVolume* pVolume = GetBoundingVolume() )
	{
		WzBoundingVolume bvVolume = *pVolume;

		if( St_Camera* pCameraInfo = g_pSunRenderer->GetCameraInfo() )
		{
			WzVector vPos = (pCameraInfo->m_wvDir * pCameraInfo->m_fNear) + pCameraInfo->m_wvFrom;

			GlobalFunc::ShrinkBoundingVolume(&bvVolume,0.6f);

			if ( Contain_Bv_Vertex( &bvVolume, vPos) )
			{
				return TRUE;
			}
        }
	}
	
	return Object::RenderOutline(color,fThickness);

}

//------------------------------------------------------------------------------
/**
*/
void Character::SetFadeIn(int iDelay)
{
	if (iDelay >= 0)
	{
		m_iFadeCurDelay = 0;
		m_iFadeTotDelay = iDelay;
		m_bFade = TRUE;
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetAttackRange( float fRange )
{
	assert(fRange >= 0.0f);
	m_fAttackRange = fRange;

}

//------------------------------------------------------------------------------
/**
*/
float Character::ProcessAnimation(DWORD dwTick,WzEvent *pEvent,BOOL bIgnoreEvent)
{	
	CWzUnitDraw *pDraw = GetBasePart();
	if (pDraw)
	{
		// 캐릭터는 VisibleAngleZ를 사용
        pDraw->SetRotation( GetAngleX() ,0.0f, m_fVisibleAngleZ, FALSE);
	}

	float fProgressRate;

	if (m_pEmoteDraw)
	{
		g_pSunRenderer->ProgressUnitFrame(m_pEmoteDraw,(float)dwTick);
	}

	if( m_dwHitPause == 0 ) 
	{
		fProgressRate = Object::ProcessAnimation(dwTick,pEvent,bIgnoreEvent);
	}
	else
	{
		if (m_dwHitPause <= dwTick)
		{
			m_dwHitPause = 0;
		}
		else
		{
			m_dwHitPause -= dwTick;
		}

		fProgressRate = Object::ProcessAnimation(1,pEvent,bIgnoreEvent);
	}

	return fProgressRate;
}

//------------------------------------------------------------------------------
/**
*/
void Character::LockOn(DWORD dwObjectKey)
{
	// 나중에 변수를 하나로!
	if (dwObjectKey)
	{
		m_dwLockedTargetID = dwObjectKey;
		m_bLocked = TRUE;
	}
	else
	{
		m_dwLockedTargetID = 0;
		m_bLocked = FALSE;
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::LockOnPosition(BOOL bFlag,WzVector vVector)
{
	m_bLockedPosition = bFlag;
	if (m_bLockedPosition)
	{
		m_vLockedPosition = vVector;
	}
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetLockedTarget()
{
	return m_dwLockedTargetID;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::UseSkill( int SkillID )
{
	DWORD dwObjectType = 0;

	BIT_ADD(dwObjectType,GetSCObject().GetObjectType());

	StateBase* pSkill = g_SkillStateFactory.CreateState( SkillID, dwObjectType );
	if ( pSkill == NULL )
	{
		return FALSE;
	}

	SetNextSkillState(pSkill);
	SetNextState(STATE::SKILL,g_CurTime);	

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Character::UpdateOctree()
{
	UpdateOctree( FALSE );
}

//------------------------------------------------------------------------------
/**
*/
void Character::UpdateOctree( BOOL bForce )
{
	if( m_wvOldVisiblePos != GetVisiblePos() || 
		GetOctIndex() == INVALID_WZOCTINDEX || 
		bForce )
	{
		m_wvOldVisiblePos = GetVisiblePos();

		if (g_pMap->IsLoaded())
		{
			AddObjectToOctree();
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetVisibleAngleModifer(float fAngle)
{
	ClampAngle(&fAngle);

	m_fVisibleAngleModifier = fAngle;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::ProcessInterpolation(DWORD dwTick,float fInterpolateRatio)
{
    WzVector vTempPos;  

#ifdef _PERF_DEBUG
	vTempPos = m_pPathExplorer->GetPos();
#else

	assert(m_pPathExplorer->GetVisiblePos().x == m_pPathExplorer->GetPos().x);
	assert(m_pPathExplorer->GetVisiblePos().y == m_pPathExplorer->GetPos().y);

	vTempPos = m_pPathExplorer->GetVisiblePos() + m_wvInterpolateOffset;

	const float INTERPOLATE_EPSILON = 1.0e-04f;
	const float INTERPOLATE_RATIO = 0.92f;

	if (fabs(m_wvInterpolateOffset.x) < INTERPOLATE_EPSILON)
	{
		m_wvInterpolateOffset.x = 0.0f;
	}
	else
	{
		m_wvInterpolateOffset.x *= fInterpolateRatio;
	}

	if (fabs(m_wvInterpolateOffset.y) < INTERPOLATE_EPSILON)
	{
		m_wvInterpolateOffset.y = 0.0f;
	}
	else
	{
		m_wvInterpolateOffset.y *= fInterpolateRatio;
	}

	if (fabs(m_wvInterpolateOffset.z) < INTERPOLATE_EPSILON)
	{
		m_wvInterpolateOffset.z = 0.0f;
	}
	else
	{
		m_wvInterpolateOffset.z *= fInterpolateRatio;
	}

#endif // _PERF_DEBUG
	
    if( IsFreeMove() == FALSE )
    {
		// 지형 높이에 상관 없이 정해진 높이 값으로 움직인다.
		SetVisiblePos( vTempPos );
//        m_wvVisiblePos = vTempPos;
    }

	const float fVibrateSpeed = 0.0150f;
	const float fEpsilon = 0.00005f;
	const float fRate = -0.90f;

	WzVector vRealVisiblePos = GetVisiblePos();

	// 떨기

	if (m_bVibrateMove) 
    {
		if (m_fVibrateForce != 0.0f)
		{
			if ( m_fVibrateCurrent < m_fVibrateForce)
			{
				m_fVibrateCurrent += (fVibrateSpeed * dwTick);

				if (m_fVibrateCurrent >= m_fVibrateForce)
				{
					m_fVibrateCurrent = m_fVibrateForce;
					m_fVibrateForce *= fRate;
				}
			}
			else
			{
				m_fVibrateCurrent -= (fVibrateSpeed * dwTick);

				if (m_fVibrateCurrent <= m_fVibrateForce)
				{
					m_fVibrateCurrent = m_fVibrateForce;
					m_fVibrateForce *= fRate;
				}
			}

			if (fabs(m_fVibrateForce) <= fEpsilon)
			{
				m_fVibrateForce = 0.0f;
				m_bVibrateMove = FALSE;
				m_fVibrateCurrent = 0.0f;
			}

			vRealVisiblePos += m_vVibrateDirection * m_fVibrateCurrent;
		}
		else
		{
			m_bVibrateMove = FALSE;
		}
	}
	
	// 띄우기
	vRealVisiblePos.z += m_fHeight;
	
	GetBasePart()->SetPosition(vRealVisiblePos);
	
	// renderpart의 형태에 따라 octindex 변경
	UpdateOctree();

	const float fTurnSpeed = WZ_PI / 4.0f; // 15도
	float fDestViewAngle = GetAngle() + m_fVisibleAngleModifier;
	if (fDestViewAngle >= WZ_PI * 2.0f)
	{
		fDestViewAngle -= WZ_PI * 2.0f;
	}
	else if (fDestViewAngle < 0.0f)
	{
		fDestViewAngle += WZ_PI * 2.0f;
	}
	
    ClampAngle(&fDestViewAngle);

	if (fDestViewAngle != m_fVisibleAngleZ) 
	{
		// 각도를 더하는게 빠른지 빼는게 빠른지 판단한다.
		float fLeftDiff; // 이쪽이 더하기
		float fRightDiff; // 빼기

		if ( fDestViewAngle > m_fVisibleAngleZ)
		{
			fLeftDiff = fDestViewAngle - m_fVisibleAngleZ;
		}
		else
		{
			fLeftDiff = fDestViewAngle - m_fVisibleAngleZ + WZ_PI * 2.0f;
		}

		// 반대쪽 회전각도와의 합은 360도
		fRightDiff = WZ_PI * 2.0f - fLeftDiff;
		// 강제 방향조정.
		switch(m_ForceTurn)
		{
		case FORCE_TURN_RIGHT:
			fLeftDiff = 99999.9f;
			break;
		case FORCE_TURN_LEFT:
			fRightDiff = 99999.9f;
			break;
		}

		if ( fLeftDiff <= fRightDiff )
		{
			if (fLeftDiff > fTurnSpeed)
			{
				m_fVisibleAngleZ += fTurnSpeed;
				ClampAngle(&m_fVisibleAngleZ);
			}
			else
			{
				m_fVisibleAngleZ = fDestViewAngle;
				m_ForceTurn = FORCE_TURN_NONE;
			}
		}
		else
		{
			if (fRightDiff > fTurnSpeed)
			{
				m_fVisibleAngleZ -= fTurnSpeed;
				ClampAngle(&m_fVisibleAngleZ);
			}
			else
			{
				m_fVisibleAngleZ = fDestViewAngle;
				m_ForceTurn = FORCE_TURN_NONE;
			}
		}
	}

	if (m_fSpinForce > 0.0f)
	{
		m_fSpinForce *= 0.8f;
		if (m_fSpinForce <= 0.05f)		
		{
			m_fSpinForce = 0.0f;
		}
		m_fVisibleAngleZ += m_fSpinForce;
		ClampAngle(&m_fVisibleAngleZ);
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetInterpolatePosition()
{
	if (GENERALPARAM->IsInterpolatePosition()) 
	{
		if (m_pPathExplorer->GetTerrainMesh() != -1)
		{
			WzVector vCurrentPos = m_pPathExplorer->GetVisiblePos();

			m_wvInterpolateOffset =  GetVisiblePos() - vCurrentPos;

			//보간 거리가 3미터가 넘어갈때 리셋한다
			if( VectorLength( &m_wvInterpolateOffset ) > 3.0f )
			{
				SetVector(&m_wvInterpolateOffset,0.f,0.f,0.f);
			}
		}
	}
}
//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsJumping()
{
	return m_bJump;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetJumpFlag(BOOL bFlag)
{
	m_bJump = bFlag;
}
//------------------------------------------------------------------------------
/**
*/
void Character::SetJumpInfo(WzVector *pvJumpFrom,WzVector *pvJumpTo)
{
	m_vJumpFrom = *pvJumpFrom;
	m_vJumpTo = *pvJumpTo;
	WzVector vDistance;
	vDistance = m_vJumpTo - m_vJumpFrom;
	m_fJumpDistance = VectorLength(&vDistance);
}

//------------------------------------------------------------------------------
/**
*/
void Character::GetJumpInfo(WzVector *pvJumpFrom,WzVector *pvJumpTo,float *pDistance)
{
	*pvJumpFrom = m_vJumpFrom;
	*pvJumpTo = m_vJumpTo;
	*pDistance = m_fJumpDistance;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetHP( DWORD dwHP ) 
{
	m_dwHP = dwHP; 
}

void  Character::SetHPInfo(HPINFO *pInfo)
{
	// 내용이 없는게 정상
	return;	
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetHP( void ) 
{
	return m_dwHP; 
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetMP( DWORD dwMP ) 
{
	m_dwMP = dwMP; 
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetMP( void )  
{
	return m_dwMP; 
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetReserveHP( DWORD val )
{
	m_reserveHP = val;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetReserveHP( void )
{
	return m_reserveHP;
}


//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetLatestAttack()
{
	return m_dwLatestAttack;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetLatestAttack(DWORD dwTimeSerial)
{
	if (dwTimeSerial > m_dwLatestAttack)
	{
		m_dwLatestAttack = dwTimeSerial;
	}
}

DWORD Character::GetLatestQueueAttack()
{
	return m_dwLatestQueueAttack;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetLatestQueueAttack(DWORD dwTimeSerial)
{
	if (dwTimeSerial > m_dwLatestQueueAttack)
	{
		m_dwLatestQueueAttack = dwTimeSerial;
	}
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetCurrentAttackSerial()
{
	return m_dwAttackSerial;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetCurrentAttackSerial(DWORD dwAttackSerial)
{
	m_dwAttackSerial = dwAttackSerial;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetNextAttackSerial()
{
	return m_dwNextAttackSerial;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetNextAttackSerial(DWORD dwSerial)
{
	m_dwNextAttackSerial = dwSerial;
}

//------------------------------------------------------------------------------
/**
*/
void Character::AddNextAttackSerial()
{
	m_dwNextAttackSerial++;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetTargetID(DWORD dwObjectKey)
{
	m_dwTargetObjectKey = dwObjectKey;
}

//------------------------------------------------------------------------------
/**
*/
Object* Character::GetTargetObject()
{
	return g_ObjectManager.GetObject(m_dwTargetObjectKey);
}

//------------------------------------------------------------------------------
/**
    공격 결과를 실행 하기 위해 
*/
int Character::AddAttackResult(ATTACK_RESULT *pattresult)
{	
	m_AttackResultList.push_back(*pattresult);
	return m_AttackResultList.size();

}

//------------------------------------------------------------------------------
/**
*/
int Character::GetAttackResultListSize()
{
	return m_AttackResultList.size();
}

//------------------------------------------------------------------------------
/**
*/
void Character::EmptyAttackResultList()
{
	m_AttackResultList.clear();
}

void Character::_ProcessRemainAttackResult( DWORD attack_serail, BOOL is_right, BOOL is_additional_effect )
{
    ATTACK_RESULT_LIST::iterator itr(m_AttackResultList.begin());
    while(itr != m_AttackResultList.end())
    {
        ATTACK_RESULT* attack_result = &(*itr);

        ATTACK_RESULT_LIST::iterator tempitr = itr;
        ++itr;

        Character* target_character = NULL;
        if (attack_result != NULL)
        {
            target_character = g_ObjectManager.GetCharacter(attack_result->GetTargetData().GetTargetObjectKey() );
        }

        // 자기 어택씨리얼보다 작은거는 무조건 처리한다
        if (attack_result->GetAttackSerial() < attack_serail)
        {
            if (target_character != NULL)
            {
                target_character->ProcessReflectDamage();
            }

            _ProcessAttackResult( attack_result, is_right, is_additional_effect);
            m_AttackResultList.erase(tempitr);
        }
    }
}

BOOL Character::ProcessAttackResult_Group(DWORD dwAttackSerial, BOOL bRight, BOOL bAddtionalEffect )
{
    _ProcessRemainAttackResult( dwAttackSerial, bRight, bAddtionalEffect );

    BOOL bResult = FALSE;
    BOOL first_group_start = TRUE;
    DWORD current_group_id = GROUP_ID_NONE;//그룹 없음 이다.0번은 

    ATTACK_RESULT_LIST::iterator itr(m_AttackResultList.begin());
    while(itr != m_AttackResultList.end())
    {
        ATTACK_RESULT* attack_result = &(*itr);
        ATTACK_RESULT_LIST::iterator tempitr = itr;
        ++itr;

        // 찾았으면!
        if (tempitr->GetAttackSerial() == dwAttackSerial)
        {
            // AttackSerial이 같고 히트수가 여러개 일경우에 동일한 AttackSeral에 그룹아이디가 1,2,3,4.. 순으로 등록되어있다
            // 첫번째 히트에 그룹1, 두번째 히트에 그룹2를 처리하기 위한 코드

            if( attack_result->GetGroupID() != GROUP_ID_NONE )
            {
                if( first_group_start  ) 
                {
                    current_group_id = attack_result->GetGroupID();   // 이번에 처리할 그룹아이디를 구하는과정
                    first_group_start = FALSE;
                }

                // 그룹 삭제에서 그룹 아이디가 없다면 삭제 안된다.
                if( attack_result->GetGroupID() == current_group_id )
                {
                    if( Character* target_character = g_ObjectManager.GetCharacter(attack_result->GetTargetData().GetTargetObjectKey()) )
                    {
                        target_character->ProcessReflectDamage();
                    }

                    bResult = TRUE;
                    _ProcessAttackResult( attack_result, bRight, bAddtionalEffect );
                    m_AttackResultList.erase(tempitr);
                }
            }
        }
    }

    //! 반사계열 모두 처리 처리
    ProcessReflectDamage();

    return bResult;	
}

BOOL Character::ProcessAttackResult_Target(DWORD dwAttackSerial, DWORD target_object_key, BOOL bRight, BOOL bAddtionalEffect )
{
    _ProcessRemainAttackResult( dwAttackSerial, bRight, bAddtionalEffect );

    if( target_object_key == 0xFFFFFFFF )
        return FALSE;

    BOOL first_group_start = TRUE;
    DWORD current_group_id = GROUP_ID_NONE;//그룹 없음 이다.0번은 

    ATTACK_RESULT_LIST::iterator itr(m_AttackResultList.begin());
    while(itr != m_AttackResultList.end())
    {
        ATTACK_RESULT* attack_result = &(*itr);
        ATTACK_RESULT_LIST::iterator tempitr = itr;
        ++itr;


        // 찾았으면!
        if (attack_result->GetAttackSerial() == dwAttackSerial)
        {
            if(attack_result->GetTargetData().GetTargetObjectKey() == target_object_key )
            {
                if( Character* target_character = g_ObjectManager.GetCharacter(attack_result->GetTargetData().GetTargetObjectKey()) )
                {
                    target_character->ProcessReflectDamage();
                }

                m_AttackResultList.erase(tempitr);
                break;
            }
        }
    }

    //! 반사계열 모두 처리 처리
    ProcessReflectDamage();

    return FALSE;	
}

BOOL Character::ProcessAttackResult(DWORD dwAttackSerial, BOOL bAll, BOOL bRight, BOOL bAddtionalEffect )
{
    _ProcessRemainAttackResult( dwAttackSerial, bRight, bAddtionalEffect );

    ATTACK_RESULT_LIST::iterator itr(m_AttackResultList.begin());
    while(itr != m_AttackResultList.end())
    {
        ATTACK_RESULT* attack_result = &(*itr);
        ATTACK_RESULT_LIST::iterator tempitr = itr;
        ++itr;


        // 찾았으면!
        if (attack_result->GetAttackSerial() == dwAttackSerial)
        {
            if( Character* target_character = g_ObjectManager.GetCharacter(attack_result->GetTargetData().GetTargetObjectKey()) )
            {
                target_character->ProcessReflectDamage();
            }

            _ProcessAttackResult( attack_result, bRight, bAddtionalEffect);

            m_AttackResultList.erase(tempitr);

            if ( bAll == FALSE ) //모두 지울게 아니라면 리턴 
            {
                ProcessReflectDamage();
                return TRUE;
            }		
        }
    }

    //! 반사계열 모두 처리 처리
    ProcessReflectDamage();

    return FALSE;
}
//------------------------------------------------------------------------------ 
void Character::ProcessReflectDamage()
{
    ATTACK_RESULT_LIST::iterator begin = m_ReflectDamageList.begin();
    ATTACK_RESULT_LIST::iterator end = m_ReflectDamageList.end();
    for (; begin != end; ++begin)
    {
        _ProcessAttackResult(&(*begin));
    }
    m_ReflectDamageList.clear();
}
//------------------------------------------------------------------------------
/**
*/
BOOL Character::_ProcessAttackResult(ATTACK_RESULT *pResult,BOOL bRight,BOOL bAddtionalEffect)
{
    if ( !g_pHero )
    {
        return TRUE;
    }

    if ( Character* target_character = (Character *)g_ObjectManager.GetObject(pResult->GetTargetData().GetTargetObjectKey()) )
    {	
		BOOL bShowDamageMesh = FALSE;

		switch (GAMEOPTION->GetDamageOutput())
		{
		case eODO_NONE:
			break;
		case eODO_SHOWMINE:
			{
                if ((g_pHero->GetObjectKey() == GetObjectKey()) || 
                    (g_pHero->GetObjectKey() == target_character->GetObjectKey()))
                {
                    bShowDamageMesh = TRUE;
                }
                else if (target_character->IsKindOfObject(MONSTER_OBJECT) == TRUE)
                {
                    Monster* monster = static_cast<Monster*>(target_character);
                    if (monster != NULL)
                    {
                        if ((monster->GetMonsterInfo()->m_byGrade == eNPC_FRIEND_MAPOBJECT_NPC) || 
                            (monster->GetMonsterInfo()->m_byGrade == eNPC_FRIEND_MONSTER))
                        {
                            bShowDamageMesh = TRUE;
                        }
                    }
                }
                else if ((target_character->IsKindOfObject(NPC_OBJECT) == TRUE) && 
                         (target_character->IsKindOfObject(MONSTER_OBJECT) == FALSE))
                {
                    NPC* npc = static_cast<NPC*>(target_character);
                    if (npc != NULL)
                    {
                        if (npc->GetExtraInfo()->m_eNPCTYPE == eNPC_TYPE_PROTECTS)
                        {
                            bShowDamageMesh = TRUE;
                        }
                    }
                }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                // 나만의 옵션에서 소환수의 의한 표시를 보여 줄 수 있도록 한다.
                if ( g_pHero && g_pHero->FindSummon(this->GetObjectKey()) == TRUE)
                {
                    bShowDamageMesh = TRUE;
                }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
			}
			break;
		case eODO_SHOWALL:
			{
				bShowDamageMesh = TRUE;
			}
			break;
		}

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
        if (target_character && target_character->GetHideSkill() == HIDE_SKILL_INFO::ALL_HIDE)
        {
            // 하이드 상태면 데미지 수치 안띄움
            bShowDamageMesh = FALSE;
        }
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

		// 실질적 체력 처리
        if ( pResult->GetTimeSerial() >= target_character->GetLatestAttack() )
        {
            // 미스가 난건 등록 안한다!
            if ( pResult->GetTargetData().IsMiss() == FALSE || pResult->GetTargetData().IsForceSetLastestAttack() == TRUE ) 
            {				
				target_character->SetLatestAttack( pResult->GetTimeSerial());				
            }

            // 힐은 HP 설정을 여기서 하지않음
            if ( pResult->GetTargetData().IsHeal() == FALSE )
            {
                // 피까고
			    if ( pResult->GetTargetData().IsNoDamage() == false ) 
                {
                    if ((pResult->GetTargetData().IsHeal() == FALSE) && 
                        (pResult->GetTargetData().IsLive() > 0) && 
                        //(pChr->IsKindOfObject(MONSTER_OBJECT) == TRUE) && 
                        (target_character->GetHP() == 0))
                    {
                        //GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, "hp 에러! 죽었다 살아남");
                        //WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_SYSCHAT, "hp 에러! 죽었다 살아남");
                    }
                    else
                    {
                        if( pResult->GetTargetData().GetTargetHP() == 0 )
                        {
                            target_character->SetDeathSkillCode( pResult->GetSkillCode() );
                        }

                        target_character->SetHP(pResult->GetTargetData().GetTargetHP() );
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
                        target_character->SetSD(pResult->GetTargetData().GetTargetSD());
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
                    }
			    }
            }
        }



		// 비주얼적인 처리
		if (pResult->GetTargetData().IsNoDamage() )
		{
			bShowDamageMesh = FALSE;			
		}

		if (bShowDamageMesh && pResult->GetTargetData().IsHeal() == FALSE )
		{
			//공격자는 플레이어.주인공.플레이어몬스터일경우
			if(IsKindOfObject(PLAYER_OBJECT) || IsKindOfObject(SSQMONSTER_OBJECT))
			{
				//피격자는 쉴드버프가 있을때 필드이펙트를 터뜨린다(무조건)
				//자해공격:공격자와 데미지받는사람이 같을경우는 제외
				if(target_character->GetObjectKey()!=GetObjectKey() && target_character->IsShield())
				{
                    GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A671"),
                        target_character->GetBonePos(target_character->GetBoneIndex(eSKILLEFFECT_POS_BODY)),pResult->GetTargetData().GetTargetObjectKey());
				}
			}
		}

		// 미스난 공격은 Miss만 띄우고 끝!
		if (pResult->GetTargetData().IsMiss())
		{
			if (!pResult->GetTargetData().IsNoDamage())
			{
				target_character->OnMiss(bShowDamageMesh);
			}

			//원하는 타수에 빼기 위해 기본은 true이다.
			if(bAddtionalEffect)
			{
				DWORD TargetAdditionalEffect = pResult->GetTargetData().GetAdditionalEffect();

				// 큐행동에 관련된것을 따로 씨리얼을 세팅한다.
				if ((	TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_KNOCKBACK)
					|| (TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_AIR)
					|| (TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_DOWN)
					|| (TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_KNOCKBACK2)
					|| (TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_KNOCKBACK3)
					)
				{
					target_character->SetLatestQueueAttack(pResult->GetTimeSerial());
				}
			}
			
            return TRUE;
		}

		// 적대적결과 일때는 맞는 애니메이션을!
		if (!pResult->GetTargetData().IsHeal()) 
		{                
			if (!pResult->GetTargetData().IsNoDamage() ) 
            {
				DAMAGE_PRAM DamageParam(pResult,bRight,GetObjectKey());
				target_character->OnDamage(DamageParam);
				OnAttack(pResult->GetSkillCode());					
			}

			//원하는 타수에 빼기 위해 기본은 true이다.
			if(bAddtionalEffect)
			{
				DWORD TargetAdditionalEffect = pResult->GetTargetData().GetAdditionalEffect();

				// 큐행동에 관련된것을 따로 씨리얼을 세팅한다.
				if ((	TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_KNOCKBACK)
					|| (TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_AIR)
					|| (TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_DOWN)
					|| (TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_KNOCKBACK2)
					|| (TargetAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_KNOCKBACK3)
					)
				{
					target_character->SetLatestQueueAttack(pResult->GetTimeSerial());
				}
			}
		}

		BOOL bCritical = pResult->GetTargetData().CheckAdditionalEffect(ATTACK_ADDITIONAL_EFFECT_CRITICAL) ? TRUE : FALSE;
		BOOL bDoubleDamage = pResult->GetTargetData().CheckAdditionalEffect(ATTACK_ADDITIONAL_EFFECT_DOUBLE) ? TRUE : FALSE;

		WzColor damagecolor;
		eDAMAGEPOPUP_KIND eKind;
		float fMultiplier = 1.0f;     

		if (target_character->IsKindOfObject(MONSTER_OBJECT))
		{
			if (!pResult->GetTargetData().IsHeal() ) 
			{
				if (GetObjectKey() == g_pHero->GetObjectKey()) 
				{
					damagecolor = c_Hero2MonsterHPDamageColor;
				}
				else
				{
					damagecolor = c_MonsterHPDamageColor;
				}
				eKind = eDAMAGEPOPUP_ENEMYHPDAMAGE;
			}
			else
			{
				damagecolor = c_MonsterHPHealColor;
				eKind = eDAMAGEPOPUP_HEAL;
			}
		}
		else
		{
			if (!pResult->GetTargetData().IsHeal() ) 
			{
				if (target_character->GetObjectKey() == g_pHero->GetObjectKey()) 
				{
					damagecolor = c_HeroHPDamageColor;
					fMultiplier = 0.9f;
				}
				else
				{
					damagecolor = c_PlayerHPDamageColor;
				}

				eKind = eDAMAGEPOPUP_PLAYERHPDAMAGE;
			}
			else
			{
				damagecolor = c_PlayerHPHealColor;
				eKind = eDAMAGEPOPUP_HEAL;
			}
		}  


		if (!pResult->GetTargetData().IsNoDamage()) 
        {
			if (bShowDamageMesh) 
            {		
				//콤보카운트가 있을때
				if (pResult->GetTargetData().GetComboCount() != 0)
				{
					fMultiplier += (pResult->GetTargetData().GetComboCount() * 0.3f);
				}

				target_character->CreatePopupMesh( pResult->GetSkillCode(), pResult->GetTargetData().GetDamage(), eKind, damagecolor,bCritical, bDoubleDamage, fMultiplier, FALSE);
			}
		}
		else
		{
			__asm nop;
		}

		if(target_character->IsKindOfObject(PLAYER_OBJECT))
		{
			Player* pTargetPlayer = (Player*)target_character;
			if ((pTargetPlayer->GetObjectKey() == g_pHero->GetObjectKey()) &&
                (0 != pTargetPlayer->GetCollectObjKey()) &&
                (pResult->GetTargetData().IsHeal() == FALSE))
			{
				const CollectionInfo* pCI = CollectionParser::Instance()->FindCollectionInfo(pTargetPlayer->GetCollectID());
				if (NULL != pCI &&
                    ((CollectionInfo::OBJ_COLLECT == pCI->m_Condition[CollectionInfo::STAMP])
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                        || (CollectionInfo::OBJ_BATTLE_GROUND_STRONGPOINT == pCI->m_Condition[CollectionInfo::STAMP])
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                    ))
				{
                    TCHAR* debug_message = NULL;
#ifdef _DEV_VER
                    debug_message = _T("MSG_CG_COLLECT_CANCEL_SYN[Damage]");
#endif 
                    StateCollect::SendCollectCancelSyn(debug_message);
				}
			}
		}
    }

    return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BYTE Character::GetMoveState()
{
	return m_bMoveState;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetMoveState(BYTE state)
{
	m_bMoveState = state;
}

//------------------------------------------------------------------------------
/**
	주의!
	STATE::MOVE와 이 함수는 직접적인 관련이 없습니다!!
	이 함수는 PathExp에 명령을 내리는 역할일 뿐입니다.
	적이 맞아서 뒤로 밀리는 처리에도 이 함수를 호출할수있습니다.
*/
BOOL Character::Move(WzVector vCurPos,WzVector vDestPos,BYTE byState,BOOL bThrust,int iCurTileNum,int iDestTileNum)
{
	m_pPathExplorer->Stop();

	if (m_pPathExplorer->GetTile() == -1)
	{
		return FALSE;
	}

	SetMoveState(byState);

	if (vCurPos.x == vDestPos.x && vCurPos.y == vDestPos.y) return FALSE;
	
	if (!bThrust) 
	{	
		int iTileFocussing = -1;

		// 대상 Tile 얻기	
		if (iDestTileNum != -1)
		{
			iTileFocussing = iDestTileNum;
		}
		else
		{
            float ft = 0.0f;
			iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vDestPos, &ft, -1.0f, 10.0f );
            vDestPos.z -= ft;
		}

		if( iTileFocussing < 0 )
		{
			return FALSE;
		}
		
	
						
		// 갈수 없는 타일로 가는 명령은 무시
		if (g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iTileFocussing) & ( PTA_ONLY_JUMP |PTA_NO_WALK))
		{
			return FALSE;
		}
		
		// 디폴트 이동 제약 타일  

		WORD wAttr = PTA_ONLY_JUMP | PTA_NO_WALK | PTA_PLAYER_FALLDOWN_TILE;
		
		// Monster인 경우 점프타일 위를 걸을 수 있게한다.
		if (IsKindOfObject(MONSTER_OBJECT))
        {
            wAttr &= (~PTA_ONLY_JUMP);
        }

		//const int SEEK_NODE = 500;
		//	길찾기
		//  일단 모두 점프할수있다.
		//  속성따라 다름

		SUNPROCESSPROFILE_BEGIN(_T("PathFinding"));
		ENUM_FINDPATH_RESULT rt=g_pSunTerrain->PathExp_FindPath( m_pPathExplorer, vDestPos, iTileFocussing, wAttr, GetSeekNodeCount());

		if(rt==EFR_FAILURE||rt==EFR_SAME_POSITION)
		{			
			SUNPROCESSPROFILE_END(_T("PathFinding"));
			return FALSE;
		}
		SUNPROCESSPROFILE_END(_T("PathFinding"));
	}
	else
	{
		WzVector vDist;	
		vDist = vDestPos - vCurPos;
		// 디폴트 이동 제약 타일
		WORD wAttr =  PTA_ONLY_JUMP |PTA_NO_WALK;
		// Monster인 경우 점프타일 위를 걸을 수 있게한다.
		if( IsKindOfObject(MONSTER_OBJECT) )
		{
			wAttr &= (~PTA_ONLY_JUMP);
		}
		g_pSunTerrain->PathExp_Thrust( m_pPathExplorer,&vDist,wAttr);

	}

	m_vTargetPosition = vDestPos;

	return TRUE;	
}


//------------------------------------------------------------------------------
/**
*/
int Character::EmptyActionQueue()
{
	while( !m_ActionQueue.empty() )
	{
		m_ActionQueue.pop();
	}
	
	return 1;
}


//------------------------------------------------------------------------------
/**
*/
int Character::GetActionQueueCount()
{
	return m_ActionQueue.size();
}

//------------------------------------------------------------------------------
/**
*/
int Character::PutAction(PLAYER_ACTION *pAction)
{
	m_ActionQueue.push(*pAction);
	// 큐에 너무많은 명령이 쌓이지는 않는지 체크
	if (m_ActionQueue.size() > 150)
	{
		WRITELOG(eLOG_LEVEL_DBG, c_wOUTPUT_LOG_CENTER, _T("m_ActionQueue.size() > 150"));
	}
	return m_ActionQueue.size();
}


//------------------------------------------------------------------------------
/**
*/
int Character::DoAction(PLAYER_ACTION *pAction)
{
    SetCurrentAction(pAction);

	PLAYER_ACTION idleaction;
	idleaction.ActionID = ACTION_IDLE;
	idleaction.IDLE.vCurPos = GetPosition();

	switch(pAction->ActionID)
	{
	case ACTION_ATTACK:
		{
			Character *pTarget = (Character *)g_ObjectManager.GetObject(pAction->ATTACK.dwTargetID);
			StopMove();			
			if (pTarget)
			{
				if (pAction->ATTACK.bAttackMove)
				{
					SetPosition(pAction->ATTACK.vCurPos);
					SetAttackMovePath(pAction->ATTACK.vCurPos,pAction->ATTACK.vDestPos);
					SetAttackMove(TRUE);
					
				}
				else 
                {
                    SetAttackMove(FALSE);
					SetPosition(pAction->ATTACK.vCurPos);
				}

				SetInterpolatePosition();

				SetTargetID(pTarget->GetObjectKey());				
				SetAttackSequence((eATTACK_SEQUENCE)pAction->ATTACK.byAttackType);

#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
                NormalAttack.ani_type = pAction->ATTACK.style_presentation;
#endif //_NA003109_STYLE_PRESENTATION_IMPROVEMENT

				SetNextActionAttackSerial(pAction->ATTACK.dwAttackSerial);

				SetNextState(STATE::ATTACK,g_CurTime);
				return 1;
			}
			// 때릴놈이 없네~
			else
			{
				// 때린 결과만 처리하고
				ProcessAttackResult(pAction->ATTACK.dwAttackSerial,TRUE);
				// 놀아용~
				SetCurrentAction(&idleaction);
				SetNextState(STATE::IDLE,g_CurTime);					
				return 0;
			}
		}
		break;


	case ACTION_TRANSFORM:
		{
			StopMove();

			GetTransformData().SetTransFormResourceCode(pAction->TRANSFORM.rcCode);
			GetTransformData().SetTransFormType(pAction->TRANSFORM.transFormType);
			GetTransformData().SetTransFormProcessType(pAction->TRANSFORM.transFormProcess);
			GetTransformData().SetAppearanceKind(pAction->TRANSFORM.appearKind);

			SetNextState(STATE::TRANSFORM,g_CurTime);
			return 1;
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

	// 이동~
	case ACTION_MOVE:
		{
			StopMove();
			
			SetNextMove(pAction->MOVE.vCurPos.wvPos,pAction->MOVE.vDestPos.wvPos,pAction->MOVE.byState,pAction->MOVE.bThrust,pAction->MOVE.vCurPos.wTile,pAction->MOVE.vDestPos.wTile);		
			SetPosition(pAction->MOVE.vCurPos.wvPos,pAction->MOVE.vCurPos.wTile);
			SetInterpolatePosition();
			SetNextState(STATE::MOVE,g_CurTime);				
			return 1;
		}
		break;

	case ACTION_KEYBOARDMOVE:
		{
			StopMove();			
			SetPosition(pAction->KEYBOARDMOVE.vKeyboardCurPos);
			SetInterpolatePosition();
			SetAngle(pAction->KEYBOARDMOVE.fKeyboardAngle);
			SetMoveFlag(pAction->KEYBOARDMOVE.byKeyboardState);			
			SetNextState(STATE::KEYBOARDMOVE,g_CurTime);
			return 1;
		}
		break;

	case ACTION_SPECIAL_MOVE:
		{			
			StopMove();	
			SetPosition(pAction->SPECIALMOVE.vCurPos);
			SetInterpolatePosition();
			SetAngle(pAction->SPECIALMOVE.fAngle,TRUE);// 특수 이동은 바로 꺽는다.
			SetSpecialMoveFlag(pAction->SPECIALMOVE.bType);
			SetNextState(STATE::SPECIALMOVE,g_CurTime);
			return 1;
		}
		break;

	case ACTION_SKILL:
		{
			Character *pTarget = (Character *)g_ObjectManager.GetObject(pAction->SKILL.dwTargetID);
			StopMove();			
			if (pTarget)
			{
				SetCurrentAction(pAction);
				SetNextActionAttackSerial(pAction->ATTACK.dwAttackSerial);
				SetTargetID(pAction->SKILL.dwTargetID);
				SetDirection( pTarget->GetVisiblePos() );
				UseSkill(pAction->SKILL.dwSkillID);
				return 1;
			}
			// 때릴놈이 없네~
			else
			{
				// 때린 결과만 처리하고
				ProcessAttackResult(pAction->SKILL.dwAttackSerial,TRUE);
				// 놀아용~
				SetNextState(STATE::IDLE,g_CurTime);	
				SetCurrentAction(&idleaction);
				return 0;
			}
		}
		break;

	case ACTION_KNOCKBACK:
		{
			// 액션큐를 사용하지 않음
			StopMove();		
			SetNextState(STATE::DAMAGE,g_CurTime);	
			return 1;
		}
		break;

	case ACTION_TOAIR:
		{
			// 액션큐를 사용하지 않음
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
			return 1;
		}
		break;

	case ACTION_DOWN:
		{
			// 액션큐를 사용하지 않음
			StopMove();	
			SetNextState(STATE::DOWN,g_CurTime);
            return 1;
		}
		break;

	case ACTION_GETUP:
		{
			// 액션큐를 사용하지 않음
			StopMove();	
			SetNextState(STATE::GETUP,g_CurTime);
            return 1;
		}
		break;

	case ACTION_SIT:
		{
			StopMove();	
			SetNextState(STATE::SIT,g_CurTime);
			return 1;
		}
		break;

	case ACTION_EVENTJUMP:
		{
			StopMove();	
			SetNextState(STATE::EVENTJUMP,g_CurTime);
            return 1;
		}
		break;

	case ACTION_STOP:
		{
			StopMove();	
			SetPosition(pAction->STOP.vCurPos);
			SetNextState(STATE::IDLE,g_CurTime);
            return 1;
		}
		break;

	case ACTION_NPCMEET:
		{
			if(STATE::NPC_SPECIAL_ACT_LOOP_STATE != GetCurState())
				SetNextState(STATE::NPC_RESPONSE_STATE, g_CurTime);

            return 1;
		}
		break;

    case ACTION_VENDORMEET:
        {
        
            return 1;
        }
        break;


    case ACTION_ATTACKJUMP:
        {
            StopMove();			

            SetNextActionAttackSerial(pAction->ATTACKJUMP.dwAttackSerial);
            SetDirection( pAction->ATTACKJUMP.vDestPos.wvPos );

            SetNextState(STATE::ATTACKJUMP,g_CurTime);

            return 1;
        }
        break;


    case ACTION_RUNAWAY:
        {
            StopMove();

            SetNextMove(pAction->MOVE.vCurPos.wvPos,pAction->MOVE.vDestPos.wvPos,pAction->MOVE.byState,pAction->MOVE.bThrust,pAction->MOVE.vCurPos.wTile,pAction->MOVE.vDestPos.wTile);		
            SetPosition(pAction->MOVE.vCurPos.wvPos,pAction->MOVE.vCurPos.wTile);
            SetInterpolatePosition();
            SetNextState(STATE::RUNAWAY,g_CurTime);				
            return 1;
        }
        break;

	case ACTION_TELEPORT:
		{
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("k470"),GetVisiblePos(),GetObjectKey());
			StopMove();			
			WzVector vPos=pAction->TELEPORT.vDest;
			SetDestWarp(vPos);
			SetPosition(vPos);
			SetDirection(vPos, TRUE);
		}
		break;
    case ACTION_TRIGGER_ANIMATION_CONTROL:
        {
            if (pAction->TRIGGERANIMATIONCONTROL.start_animation_id != 0)
            {
                StopMove();
                SetNextState(STATE::TRIGGER_ANIMATION_CONTROL, g_CurTime);
                return 1;
            }            
        }
        break;
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_BRONZE
	case ACTION_BRONZE_STAND:
		{
			StopMove();
			SetNextState(STATE::BRONZE_STAND, g_CurTime);
			return 1;
		}
		break;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_BRONZE

	default:
		{
			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("[%d] Character::DoNextAction() 처리되지 않은 큐 명령"), pAction->ActionID);
			assert(!"Character::DoNextAction() 처리되지 않은 큐 명령");
			break;
		}
	}

	SetCurrentAction(&idleaction);
	SetNextState(STATE::IDLE,g_CurTime);				

	return 0;
}

//------------------------------------------------------------------------------
/**
*/
int Character::DoNextAction()
{
	PLAYER_ACTION action;

	while( !m_ActionQueue.empty() )
	{
		action = m_ActionQueue.front();
		m_ActionQueue.pop();
		// 액션을 실행하는데 성공했으면 리턴
		// 실패시에는 그 다음액션을 시도.
		if (DoAction(&action))
		{
			return 1;	
		}			
	}

	SetNextState(STATE::IDLE,g_CurTime);
	return 0;
}


//------------------------------------------------------------------------------
/**
*/
BOOL Character::CheckQueueActionCondition()
{
	assert(m_ActionQueue.size() != 0);
	// 일반 캐릭터는 다음 Action이 Damage 액션이 아닌이상 무조건 성공합니다.
	// Damage Action은 캐릭터의 데미지가 처리된후에만 발동합니다
	switch(m_ActionQueue.front().ActionID)
	{
	case ACTION_TOAIR:
	case ACTION_KNOCKBACK:
	case ACTION_DOWN:
	case ACTION_KNOCKBACK2:
		return FALSE;
	}
	
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsMove()
{
	return BOOL( m_pPathExplorer && m_pPathExplorer->IsMoving() );
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsTransFormation()
{
    return FindSkillEffect( eCHAR_STATE_TRANSFORMATION ) != NULL;
}
//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsStun()
{
    return FindSkillEffect( eCHAR_STATE_STUN ) != NULL 
		|| FindSkillEffect( eCHAR_STATE_STUN2 ) != NULL;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsSleep()
{
    return FindSkillEffect( eCHAR_STATE_SLEEP ) != NULL;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsFrozen()
{
    return FindSkillEffect( eCHAR_STATE_FROZEN ) != NULL;
}

BOOL Character::IsPolymorph()
{
	return FindSkillEffect( eCHAR_STATE_POLYMORPH ) != NULL;
}

//#ifdef _DEBUG
//    static DWORD s_TestObjectKey = 0;
//
//    if( GetObjectKey() == s_TestObjectKey )
//        return TRUE;
//#endif
//
//    return FALSE;
//}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsBlind2()
{
#ifdef _DEBUG
    if( FindSkillEffect( (eCHAR_STATE_TYPE)972 ) != NULL )
        return TRUE;
#endif
    return FindSkillEffect( (eCHAR_STATE_TYPE)eCHAR_STATE_WHITE_BLIND ) != NULL;    // 화이트블라인드
}

BOOL Character::IsDarkofLightZoneState()
{
    return FindSkillEffect( (eCHAR_STATE_TYPE)eCHAR_STATE_DARK_OF_LIGHT_ZONE ) != NULL; // 
}

BOOL Character::IsGravityZoneState()
{
	return FindSkillEffect( (eCHAR_STATE_TYPE)eCHAR_STATE_GRAVITY_ZONE ) != NULL; // 
}

BOOL Character::IsDetectingHide()
{
	return FindSkillEffect( (eCHAR_STATE_TYPE)eCHAR_STATE_DETECTING_HIDE ) != NULL; // 
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsBlind()
{
    return FindSkillEffect( eCHAR_STATE_BLIND ) != NULL;
}
//------------------------------------------------------------------------------
bool Character::IsSteel()
{
    return FindSkillEffect( eCHAR_STATE_STEEL ) != NULL;
        /*
            || FindSkillEffect( eCHAR_STATE_STEEL2 ) != NULL
            || FindSkillEffect( eCHAR_STATE_STEEL3 ) != NULL
            || FindSkillEffect( eCHAR_STATE_STEEL4 ) != NULL
            || FindSkillEffect( eCHAR_STATE_STEEL5 ) != NULL;*/
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsFainting()
{
    return FindSkillEffect( eCHAR_STATE_SLEEP ) != NULL 
        || FindSkillEffect( eCHAR_STATE_STUN ) != NULL
        || FindSkillEffect( eCHAR_STATE_STUN2 ) != NULL;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsBlock()
{
	if(this->GetCurrentAnimation()==this->GetCurrentBlockAnimation())
	{
		float fFrame = GetTotalFrame() - 1.0f;
		float fRate = (fFrame) ? ((GetCurrentFrame() * 100.0f) / fFrame) : 100;

		if(fRate<100)
			return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL  Character::CannotAir()
{
    switch (GetCurState())
    {
    case STATE::SKILL: 
    case STATE::KEYBOARDJUMP:
    case STATE::ATTACKJUMP:
	case STATE::TRANSFORM:
	case STATE::FALL_DOWN:
	case STATE::COLLECT:
		{    
			return TRUE;
			break;
		}
    }

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::CannotMove()
{
    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
		SKILL_EFFECT* pEffect = m_SkillEffectArray[i];

		switch (pEffect->dwStatusID )
		{
		case eCHAR_STATE_DOWN:		// 다운 
		case eCHAR_STATE_FEAR:		// 공포
		case eCHAR_STATE_SLEEP:		// 수면
		case eCHAR_STATE_STUN:		// 스턴
		case eCHAR_STATE_STUN2:		// 스턴2
		case eCHAR_STATE_FROZEN:	// 프로즌
		case eCHAR_STATE_DELAY:		// 딜레이?
		case eCHAR_STATE_STONE:		// 석화
		case eCHAR_STATE_SUMMON:	// 소환중
		case eCHAR_STATE_MP_FEAR2:	// 피어2
		case eCHAR_STATE_THRUST:	// 밀리기
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
        case eCHAR_STATE_UPPERDOWN:
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_STATE_CHARMED:
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		//case eCHAR_STATE_POLYMORPH:

		case eCHAR_STATE_HOLDING:	// 홀딩 (이동불가)
			{
				return TRUE;
			}
			break;
		}				
	}

    return FALSE;
}

//------------------------------------------------------------------------------
BOOL Character::CannotAction()
{
	for( size_t i=0;i!=m_SkillEffectArray.size();++i)
	{
		SKILL_EFFECT* pEffect = m_SkillEffectArray[i];

		switch (pEffect->dwStatusID )
		{
		case eCHAR_STATE_DOWN:		// 다운 
		case eCHAR_STATE_FEAR:		// 공포
		case eCHAR_STATE_SLEEP:		// 수면
		case eCHAR_STATE_STUN:		// 스턴
		case eCHAR_STATE_STUN2:		// 스턴2
		case eCHAR_STATE_FROZEN:	// 프로즌
		case eCHAR_STATE_DELAY:		// 딜레이
		case eCHAR_STATE_STONE:		// 석화
		case eCHAR_STATE_SUMMON:	// 소환중
		case eCHAR_STATE_MP_FEAR2:	// 피어2
		case eCHAR_STATE_THRUST:	// 밀리기
		case eCHAR_STATE_FATAL_POINT:	// 페이탈포인트 (홀딩+스턴)
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
        case eCHAR_STATE_UPPERDOWN:
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eCHAR_STATE_CHARMED:
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

		//case eCHAR_STATE_POLYMORPH:	// 폴리모프 (이동가능)
			{
				return TRUE;
			}
			break;
		}				
	}
	return FALSE;
}

BOOL Character::IsImpossible()
{
    return FindSkillEffect( eCHAR_STATE_ATTACK_IMPOSSIBLE ) != NULL;
}


//------------------------------------------------------------------------------
/**  클론 이펙트(클론복사잔상)
*/
void Character::CloneEffect(DWORD LiveTime, DWORD DisappearTime,BYTE byStyle)
{
	DWORD dwCloneKey;
	dwCloneKey = g_CloneManager.CreateClone(this);
	Clone *pClone = g_CloneManager.GetClone(dwCloneKey);
	if(!pClone) return;
	
	switch(byStyle)
	{	
	case 0://하이드
		{
			pClone->SetColor(WzColor_RGBA(255,255,255,120));
		}
		break;
	case 1: //러쉬
		{
			pClone->SetAlphaBlend(AB_ALPHA);
			pClone->SetColor(WzColor_RGBA(125,125,125,125));
		}
		break;
	}
	pClone->SetAnimation(this->GetCurrentAnimation());
	pClone->SetPosition(this->GetPosition());
	pClone->SetLifeTime( LiveTime );	
	pClone->SetDisappear( DisappearTime );
	pClone->SetCurrentFrame(this->GetCurrentFrame());
}
//------------------------------------------------------------------------------
/**
*/
BOOL Character::CanBeAttackTarget()
{
	switch(GetCurState()) 
	{
	case STATE::DEATH:		
	//case STATE::JUMP:
	//case STATE::AIR:
	case STATE::EVENTJUMP:
		return FALSE;		
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetNextMove(WzVector vCurPos,WzVector vDestPos,BYTE byState,BOOL bThrust,int iCurIndex,int iDestIndex)
{
	m_vNextMoveCurPos = vCurPos;
	m_vNextMoveDestPos = vDestPos;
	m_bNextMoveState = byState;
	m_bNextMoveThrust = bThrust;
	m_iNextMoveCurTile = iCurIndex;
	m_iNextMoveDestTile = iDestIndex;
	m_bNextMove = TRUE;
}

//------------------------------------------------------------------------------
/**
*/
void Character::StartNextMove()
{
	if (m_bNextMove)
	{
		if (!m_bNextMoveThrust) 
		{
			Move(m_vNextMoveCurPos,m_vNextMoveDestPos,m_bNextMoveState,m_bNextMoveThrust,m_iNextMoveCurTile,m_iNextMoveDestTile);
		}
		else
		{
			Move(m_vNextMoveCurPos,m_vNextMoveDestPos,m_bNextMoveState,m_bNextMoveThrust,m_iNextMoveCurTile,-1);

		}
		m_bNextMove = FALSE;
	}
	else
	{
		if (GetSCObject().GetObjectType() == MONSTER_OBJECT) 
		{
			__asm nop;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetAttackMove(BOOL bMove)
{
	m_bAttackMove = bMove;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetAttackMovePath(HONGPOS vCurPos,HONGPOS vDestPos)
{
	m_vAttackStartPos = vCurPos;
	m_vAttackEndPos = vDestPos;	
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetHitPause(DWORD dwPause)
{
	m_dwHitPause = dwPause;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetHitPause()
{
	return m_dwHitPause;
}

//------------------------------------------------------------------------------
/**
*/
void Character::Vibrate(WzVector vDirection,float fForce)
{
#ifdef VIBRATE_CHARACTER

	m_vVibrateDirection = vDirection;
	m_fVibrateForce = fForce;
	m_fVibrateCurrent = 0.0f;
	m_bVibrateMove = TRUE;

#endif	
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetMoveSpeed( float fMoveSpeed )
{
	m_fMoveSpeed = fMoveSpeed;
}

//------------------------------------------------------------------------------
/**
*/
float Character::GetMoveSpeed( void )
{
	return m_fMoveSpeed;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetCurrentAction(PLAYER_ACTION *pAction)
{
	m_CurAction = *pAction;
}


//------------------------------------------------------------------------------
/**
*/
PLAYER_ACTION* Character::GetCurrentAction()
{
	return &m_CurAction;
}

PLAYER_ACTION* Character::GetNextAction()
{
	if( GetActionQueueCount() )
	{
		return &m_ActionQueue.front();
	}
	else
	{
		return NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::CommonStateProcess()
{	
	if (GetActionQueueCount())
	{
		PLAYER_ACTION NextAction = m_ActionQueue.front();

		switch (NextAction.ActionID)
		{
		case ACTION_KNOCKBACK2:
		case ACTION_KNOCKBACK:
		case ACTION_TOAIR:
		case ACTION_DOWN:
			if ( GetLatestQueueAttack() >= NextAction.dwTimeSerial)
			{
				DoNextAction();
				return TRUE;
			}
			break;
        case ACTION_ETHERIA_EXTRACT:
        case ACTION_SUMMON_RIDER:
		case ACTION_FALLDOWN:
		case ACTION_KEYBOARDJUMP:
		case ACTION_TRANSFORM:
        case ACTION_WINGS_SPREAD:
        case ACTION_WINGS_FURL:
        case ACTION_RIDER_SPREAD:
        case ACTION_RIDER_FURL:
			{
                if( m_dwHP == 0 && NextAction.ActionID == ACTION_TRANSFORM )
                    break;

				DoNextAction();
				return TRUE;
			}
			break;
		}
	}
	// 피없으면 바로 죽으셈
	if (m_dwHP <=0 )
	{
		int iDestTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand(GetPosition(), NULL, -1.0f, 100.0f);
		if (iDestTileFocussing >= 0)
		{
			WORD Attribute = g_pMap->GetWorldBase()->x_pPathFinder->GetAttribute(iDestTileFocussing);


			if (Attribute & (PTA_PLAYER_FALLDOWN_TILE))
			{
				SetNextState(STATE::FALL_DOWN, g_CurTime);		
				return TRUE;
			}
		}
        
        SetNextState(STATE::DEATH, g_CurTime);
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
float Character::GetProcessMultiplier()
{
	const float fMaxMultiplier = 5.0f;	
	// 큐가 하나만 쌓여있어도 꽤나 처리가 느리다는뜻이다!!
	const int   iTolerance = 1;
	float fMultiplier = 1.0f;
	int count = GetActionQueueCount();
	if (count >= iTolerance)
	{
		while(count > 0)
		{
			fMultiplier *= 1.1f;
			if (fMultiplier > fMaxMultiplier)
			{
				break;
			}
			count--;
		}
	}

	if (fMultiplier > fMaxMultiplier)
	{
		fMultiplier = fMaxMultiplier;
	}

	return fMultiplier;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetHitCount(int count)
{
	m_iHitCount = count;
}

//------------------------------------------------------------------------------
/**
*/
int Character::GetHitCount()
{
	return m_iHitCount;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::ProcessNextAction()
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
	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void Character::ProcessAllRemainActionResult()
{
	PLAYER_ACTION action;

	while( !m_ActionQueue.empty() )
	{
		action = m_ActionQueue.front();
		m_ActionQueue.pop();

		switch(action.ActionID)
		{
		case ACTION_ATTACK:
			{
				ProcessAttackResult(action.ATTACK.dwAttackSerial,TRUE);				
			}
            break;

		case ACTION_SKILL:
			{
				ProcessAttackResult(action.SKILL.dwAttackSerial,TRUE);				
			}
            break;

        case ACTION_ATTACKJUMP:
            {
                ProcessAttackResult(action.ATTACKJUMP.dwAttackSerial,TRUE);
            }
            break;

		}						
	}

	ATTACK_RESULT_LIST::iterator itr;
    itr = m_AttackResultList.begin();
	
	while(itr != m_AttackResultList.end())
    {
		ATTACK_RESULT ret = *itr;
		_ProcessAttackResult(&ret);     
		itr++;
    }

	m_AttackResultList.clear();
	
    //! 반사계열 처리
    ProcessReflectDamage();
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetAttackSequence( eATTACK_SEQUENCE sequence ) 
{
	m_eAttackSequence = sequence; 
}

//------------------------------------------------------------------------------
/**
*/
eATTACK_SEQUENCE Character::GetAttackSequence( void ) 
{
	return m_eAttackSequence; 
}

//------------------------------------------------------------------------------
/**
*/
void Character::HitSpin(float fForce,BOOL bRight)
{
	m_fSpinForce = fForce;
}


//------------------------------------------------------------------------------
/**
*/
void Character::PlayDamageSound(eWEAPONSOUNDKIND eSoundKind)
{
	;
}

//------------------------------------------------------------------------------
/**
*/
void Character::CreateDamageEffect(BOOL bCritical,BOOL bRight,DWORD dwAttackObjId)
{
	WzVector wvEffectPos = GetArrowTargetPos();
    wvEffectPos.z += GetHeight();
	float fHeight = GetArrowTargetHeight();

	HANDLE hSparkEffect     = INVALID_HANDLE_VALUE;
	HANDLE hBloodEffect	    = INVALID_HANDLE_VALUE;
	WzID wiDamageEffectID	= INVALID_WZID;
	WzID wiCDamageEffectID  = INVALID_WZID;
    
    BOOL bEventEffect = FALSE;

    Object* pChr = g_ObjectManager.GetObject(dwAttackObjId);

    if(pChr && pChr->IsKindOfObject(PLAYER_OBJECT))
    {
        Player *pPlayer = static_cast<Player*>(pChr);

        //맞은데미지가 이벤트웨폰(사탕)이였다면 이펙트를 터뜨린다.
        if(pPlayer && this!=pPlayer && pPlayer->IsEventWeapon())
        {
            bEventEffect = TRUE;
        }
    }

#ifdef _PERF_DEBUG
	return;
#endif

    if (bEventEffect)
    {
        GlobalFunc::CreateEffectBeUpToOption(StrToWzID("J760"),wvEffectPos,dwAttackObjId,GetObjectKey());
    }
    else
    {
		//////////////////////////////////////////////////////////////////////////
		// 한 캐릭터에 너무 많은 데미지이펙트가 붙는걸 방지하기 위한 코드
		std::list<HANDLE>::iterator iter    = m_listDamageEffectHndl.begin();
		std::list<HANDLE>::iterator iterEnd	= m_listDamageEffectHndl.end();
		int iDamageEftCnt = 0;
		while(iterEnd != iter)
		{
			HANDLE hHndl = (*iter);
			if (NULL != g_pEffectManager->GetEffectPtrHandle(hHndl))
			{
				++iDamageEftCnt;
				++iter;
			}
			else
			{
				iter = m_listDamageEffectHndl.erase(iter);
			}
		}
		
		if(SCENE_TYPE_DOMINATION == GameFramework::GetCurScene()
#ifdef _NA_008334_20150608_SONNENSCHEIN
            || SCENE_TYPE_SONNENSCHEIN == GameFramework::GetCurScene()
#endif //_NA_008334_20150608_SONNENSCHEIN
            )
		{
			if(MAX_DAMAGE_EFFECT_CNT_IN_DOMINATION < iDamageEftCnt)
				return;
		}
		else
		{
			if (MAX_DAMAGE_EFFECT_CNT < iDamageEftCnt)
				return;
		}
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		//스크립트에 타격 이펙트가 있는지 체크(있으면 있는걸로 없으면 기본으로)
		if (TRUE == this->IsKindOfObject(MONSTER_OBJECT))
		{
			Monster* pMon = (Monster*)this;
			wiDamageEffectID  = pMon->GetMonsterInfo()->m_wiDamageEffectID;
			wiCDamageEffectID = pMon->GetMonsterInfo()->m_wiCDamageEffectID;
		}

        // 타격효과
        if (bCritical) 
        {
			if (INVALID_WZID == wiCDamageEffectID)
			{
				Object* pObj = g_ObjectManager.GetObject(dwAttackObjId);
				if (pObj)
				{
					if (TRUE == pObj->IsKindOfObject(PLAYER_OBJECT))
					{
						Player* pPlayer = (Player*)pObj;
						switch (pPlayer->GetClass())
						{
						case eCHAR_SHADOW:
							// 스파크
							hSparkEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A650"),wvEffectPos,dwAttackObjId,GetObjectKey());
							break;

						default:
							// 스파크
							hSparkEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A008"),wvEffectPos,dwAttackObjId,GetObjectKey());
							break;
						}
					}
					else
					{
						// 스파크
						hSparkEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A008"),wvEffectPos,dwAttackObjId,GetObjectKey());
					}

					// 피
					hBloodEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A005"),wvEffectPos,dwAttackObjId,GetObjectKey());
				}
			}
			else
			{
				hSparkEffect = GlobalFunc::CreateEffectBeUpToOption(wiCDamageEffectID,wvEffectPos,dwAttackObjId,GetObjectKey());
			}
        }
        else
        {
			if (INVALID_WZID == wiDamageEffectID)
			{
				// 피
				hBloodEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A004"),wvEffectPos,dwAttackObjId,GetObjectKey()); 
				// 스파크
				hSparkEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A007"),wvEffectPos,dwAttackObjId,GetObjectKey());
			}
			else
			{
				hSparkEffect = GlobalFunc::CreateEffectBeUpToOption(wiDamageEffectID,wvEffectPos,dwAttackObjId,GetObjectKey());
			}

        }
    }

	if (INVALID_HANDLE_VALUE != hSparkEffect)
    {
		m_listDamageEffectHndl.push_back(hSparkEffect);
    }

	WzVector vBaseScale;
	vBaseScale.x = 1.0f;
	vBaseScale.y = 1.0f;
	vBaseScale.z = 1.0f;

//! 2011.5.3 / i4u4me / 캐릭팀(조흥래팀장)에서 타격시 딜레이 주지 않게 요청
//! 관련 작업 undef 함, 효과는 유지
  //#ifdef _YJ_MOD_BLOW_MOTION_001188
	CEffectBody* pBodyB = g_pEffectManager->GetEffectPtrHandle(hBloodEffect);
	CEffectBody* pBodyS = g_pEffectManager->GetEffectPtrHandle(hSparkEffect);

	float fDestAngle = WZ_PI / 4.0f;
	WzVector vRot;
	WzVector vScaleBlood, vScaleSpark;

	vRot.x = 0.0f;
	vRot.y = 0.0f;
	vRot.z = 0.f;

	if (bRight)
	{
		vRot.z -= fDestAngle;
	}
	else
	{
		vRot.z += fDestAngle;
	}

	if (4.0f < fHeight)
	{
		SetVector(&vScaleBlood, 4.0f, 4.0f, 4.0f);
		SetVector(&vScaleSpark, 2.f, 2.f, 2.f);
	}
	else if (3.5f < fHeight)
	{
		SetVector(&vScaleBlood, 3.5f, 3.5f, 3.5f);
		SetVector(&vScaleSpark, 1.8f, 1.8f, 1.8f);
	}
	else if (3.0f < fHeight)
	{
		SetVector(&vScaleBlood, 3.0f, 3.0f, 3.0f);
		SetVector(&vScaleSpark, 1.4f, 1.4f, 1.4f);
	}
	else if (2.5f < fHeight)
	{
		SetVector(&vScaleBlood, 2.5f, 2.5f, 2.5f);
		SetVector(&vScaleSpark, 1.2f, 1.2f, 1.2f);
	}
	else
	{
		SetVector(&vScaleBlood, 2.f, 2.f, 2.f);
		SetVector(&vScaleSpark, 1.f, 1.f, 1.f);
	}

	if (pBodyB)
    {
		pBodyB->SetResultMatrix(wvEffectPos,vRot,vScaleBlood);
    }

	if (pBodyS)
	{
        if (INVALID_WZID == wiDamageEffectID)
        {
			pBodyS->SetResultMatrix(wvEffectPos,vRot,vScaleSpark);
        }
		else
		{
			vRot.x = 0.0f;
			vRot.y = 0.0f;
			vRot.z = GetAngle();
			pBodyS->SetResultMatrix(wvEffectPos,vRot,vBaseScale);
		}
	}
  //#endif //_YJ_MOD_BLOW_MOTION_001188
	//////////////////////////////////////////////////////////////////////////
}

//------------------------------------------------------------------------------
/**
	죽었음
*/
BOOL Character::IsDead()   
{
    return (m_dwHP == 0) ? TRUE : FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void  Character::SetAttackStandTime(int iTime)
{
	m_iAttackStandTime = iTime;
}

//------------------------------------------------------------------------------
/**
*/
float Character::GetAttackSpeedModifier()
{
	return 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
float Character::GetMoveSpeedModifier()
{
	return 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::OnUseItem(DWORD dwItemCode)
{

	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL Character::OnUnUseItem(DWORD dwItemCode)
{

	return TRUE;
}

////------------------------------------------------------------------------------ 
//HANDLE Character::AddStateEffect(WzID state_effect_id, DWORD target_objectkey, SKILL_EFFECT* skill_effect)
//{
//    if ((state_effect_id) > 0 && (skill_effect != NULL))
//    {
//        return GlobalFunc::CreateEffectBeUpToOption(state_effect_id, 
//                                                    GetSkillEffectPos(skill_effect), 
//                                                    GetSkillEffectRotate(skill_effect),
//                                                    GetSkillEffectScale(skill_effect),
//                                                    target_objectkey, 
//                                                    GetObjectKey());
//    }
//    return INVALID_HANDLE_VALUE;
//}

#include <algorithm>
bool _Sort_Effect( SKILL_EFFECT* pLeft, SKILL_EFFECT* pRight )
{
#ifdef _NA_006819_20130719_STATE_ICON_RENDER
    static int sort_order[] = { 5,      //eSTATE_KIND_DEBUFF = 0,
                                3,      //eSTATE_KIND_ITEM = 1,
                                1,      //eSTATE_KIND_CASH = 2,
                                4,      //eSTATE_KIND_ETC = 3, 
                                0,      //eSTATE_KIND_SHIELD = 4,
                                2,      //eSTATE_KIND_SKILL = 5,
                                999};   //eSTATE_KIND_MAX = 6,     // 낮은값이 앞에오도록 정렬됨
    // 캐릭터스킬,쉴드,캐쉬,아이템,기타,디버프 순서
#else // _NA_006819_20130719_STATE_ICON_RENDER
    //eSTATE_KIND_DEBUFF = 0,
    //eSTATE_KIND_SKILL = 1,
    //eSTATE_KIND_CASH = 2,
    //eSTATE_KIND_ETC = 3, //기타
    //eSTATE_KIND_SHIELD =4,//쉴드 최우선
    //eSTATE_KIND_MAX = 5,
    static int sort_order[] = {4,2,1,3,0,999};
    // 쉴드,캐쉬,스킬,기타,디버프 순서

#endif // _NA_006819_20130719_STATE_ICON_RENDER
    
    if( sort_order[pLeft->GetIconKind()] < sort_order[pRight->GetIconKind()] )
        return true;

    if( sort_order[pLeft->GetIconKind()] == sort_order[pRight->GetIconKind()] )
        return pLeft->iRemainTime < pRight->iRemainTime;

    return false;
};

void Character::OnAddState( eCHAR_STATE_TYPE StateType )
{
    //쉴드체크
	if( StateType == eCHAR_STATE_ETC_EXIST_SHELD_POINT)
    {
        m_bShield = TRUE;
    }
	else if( StateType == eCHAR_STATE_DETECTING_HIDE )
    {
        if( GetObjectKey() == g_pHero->GetObjectKey() )
        {
            g_ObjectManager.RefreshPlayerHideState();
        }
    }
	else if( StateType == eCHAR_STATE_DARK_OF_LIGHT_ZONE )
	{
		if( IsKindOfObject(PLAYER_OBJECT) )
		{
			Player* pPlayer = (Player*)this;
			pPlayer->RefreshHideState(FALSE);
		}
	}
    else if( StateType == eCHAR_STATE_POLYMORPH )
    {
        SetPolymorph(TRUE);
    }
    else if( StateType == eCHAR_STATE_BLIND || StateType == eCHAR_STATE_WHITE_BLIND )
    {
        if( GetTargetObject() != NULL )
        {
            SetTargetID(0);
        }

        if( IsMyHero() == TRUE )
        {
            g_HeroInput.SetCurrentTarget(0);
        }
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    else if( StateType == eCHAR_STATE_VICE_SWORD )
    {
        CreateViceSwordWeaponEffect();
        if (g_pHero == this)
        {
            SCItemSlot& rEquipItemSlot = 
                static_cast<SCItemSlot&>(ItemManager::Instance()->GetContainer(SI_EQUIPMENT)->GetSlot(EquipmentContainer::EQUIP_WEAPON1));
            rEquipItemSlot.GetLaxLock()->EnterLock();
        }
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    if( m_bShield)
    {
        ReleaseShieldEffect();
    }

//#ifdef _DEBUG
//    if( BASE_STATEINFO* pStateInfo = StateInfoParser::Instance()->GetStateInfo( StateType ) )
//    {
//        char temp[1024];
//        wsprintf( temp, "AddState : %s [%d]%s\n", GetName(), (int)StateType, pStateInfo->m_szStateName );
//        OutputDebugStr( temp );
//    }
//#endif

}

void Character::OnRemoveState( eCHAR_STATE_TYPE StateType, BOOL IsDestroy )
{
//#ifdef _DEBUG
//    if( BASE_STATEINFO* pStateInfo = StateInfoParser::Instance()->GetStateInfo( StateType ) )
//    {
//        char temp[1024];
//        wsprintf( temp, "RemoveState : %s [%d]%s\n", GetName(), (int)StateType, pStateInfo->m_szStateName );
//        OutputDebugStr( temp );
//    }
//#endif

    //쉴드체크
    if( StateType == eCHAR_STATE_ETC_EXIST_SHELD_POINT)
    {
        m_bShield = FALSE;
    }
    else if( StateType == eCHAR_STATE_DETECTING_HIDE )
    {
        if( IsMyHero() == TRUE )
        {
            g_ObjectManager.RefreshPlayerHideState();
        }
    }
	else if( StateType == eCHAR_STATE_DARK_OF_LIGHT_ZONE )
	{
		if( IsKindOfObject(PLAYER_OBJECT) )
		{
			Player* pPlayer = (Player*)this;
			pPlayer->RefreshHideState(FALSE);
		}
	}
    else if( StateType == eCHAR_STATE_POLYMORPH )
    {
        SetPolymorph( FALSE );
    }

    //TODO: 스테터스해제시 예외처리해야될구문

    //상태해제
    switch(StateType)
    {
    case eCHAR_STATE_SEALING:
    case eCHAR_STATE_CONFUSE:
        {
            if( IsMyHero() == TRUE )
            {
                g_pHero->SetLockSkill(FALSE);
            }
        }
        break;
    case eCHAR_STATE_TRANSPARENT:
        {
            SetHideSkill(FALSE);
        }
        break;

    case eCHAR_STATE_ETC_EXIST_SHELD_POINT:
        {
			if( IsDestroy == FALSE )
			{
				GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A670"),GetBodyPos(),GetObjectKey());
			}
        }
        break;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_STATE_VICE_SWORD:
        {
            DestroyViceSwordWeaponEffect();
            if (g_pHero == this)
            {
                if (ItemManager::Instance()->GetContainer(SI_EQUIPMENT))
                {
                    SCItemSlot& rEquipItemSlot = 
                        static_cast<SCItemSlot&>(ItemManager::Instance()->GetContainer(SI_EQUIPMENT)->GetSlot(EquipmentContainer::EQUIP_WEAPON1));
                    rEquipItemSlot.GetLaxLock()->LeaveLock();
                }
            }
        }
        break;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }


    CreateShieldEffect();

}


//------------------------------------------------------------------------------
/**
*/
int Character::AddSkillEffect(SKILL_EFFECT *pEffect)
{
	static int s_iEffectID = 1;
	
	if( pEffect == NULL )
		return 1;

	pEffect->iEffectID = s_iEffectID;
    
    m_SkillEffectArray.push_back(pEffect);

    std::sort(m_SkillEffectArray.begin(), m_SkillEffectArray.end(), _Sort_Effect );

	s_iEffectID++;

	// 절대로 0은 만들지 않는다.
	if (s_iEffectID == 0)
	{
		s_iEffectID++;
	}

    OnAddState( (eCHAR_STATE_TYPE)pEffect->dwStatusID );

	return pEffect->iEffectID;

}

SKILL_EFFECT* Character::FindSkillEffect( eCHAR_STATE_TYPE StateType )
{
    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
        SKILL_EFFECT* pEffect = m_SkillEffectArray[i];
        if( pEffect->dwStatusID == StateType )
        {
            return pEffect;
        }
    }

    return NULL;
}

//------------------------------------------------------------------------------
/**
*/
SKILL_EFFECT* Character::GetSkillEffect(int iEffectID)
{
    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
        SKILL_EFFECT* pEffect = m_SkillEffectArray[i];
        if (pEffect->iEffectID == iEffectID)
        {
            return pEffect;
        }
    }

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void  Character::DeleteAllSkillEffect()
{
    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
		SKILL_EFFECT* pEffect = m_SkillEffectArray[i];
        eCHAR_STATE_TYPE StateType = (eCHAR_STATE_TYPE)pEffect->dwStatusID;

        // 아이템 사용에 의한 버프가 제거되는 경우에, 해당 아이템의 이펙트가 있다면 함께 제거
		if( SkillScriptInfo * pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)pEffect->dwSkillID) )
        {
			RemoveItemEffectByItemCode(pSkillInfo->m_dwItemCode);
		}
        delete pEffect;

		OnRemoveState(StateType, TRUE);
	}
    m_SkillEffectArray.clear();
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::DeleteSkillEffect(int iEffectID)
{
    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
		SKILL_EFFECT * pEffect = m_SkillEffectArray[i];

		if (pEffect->iEffectID == iEffectID)
		{
            eCHAR_STATE_TYPE StateType = (eCHAR_STATE_TYPE)pEffect->dwStatusID;

            // 아이템 사용에 의한 버프가 제거되는 경우에, 해당 아이템의 이펙트가 있다면 함께 제거
			if( SkillScriptInfo* pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)pEffect->dwSkillID) )
			{
				RemoveItemEffectByItemCode( pSkillInfo->m_dwItemCode );
			}

#ifdef _DEBUG
			BASE_STATEINFO *pInfo = StateInfoParser::Instance()->GetStateInfo(pEffect->dwStatusID);
			TCHAR szBuff[1024] = {0,};
			if(pInfo)
				Snprintf(szBuff, 1023, "\nDeleteSkillEffect(%s): %d\n\r", pInfo->m_szStateName, pEffect->dwStatusID);
			else
				Snprintf(szBuff, 1023, "\npInfo==NULL!!!! DeleteSkillEffect: %d\n\r", pEffect->dwStatusID);

			GlobalFunc::OutputMsg_Notice( eOUTPUT_MSG_CHAT, szBuff );
#endif

			delete pEffect;

            m_SkillEffectArray.erase( m_SkillEffectArray.begin() + i );

			OnRemoveState( StateType );

			return TRUE;
		}
	}
	
	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::ProcessSkillEffect(DWORD dwTick)
{
    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
        SKILL_EFFECT* pEffect = m_SkillEffectArray[i];
        pEffect->Process( this, dwTick );
    }
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
#else
    // 이펙트가 본보다 늦게 따라오는 현상이 있어 AnimateUnit() 뒤로 옴김
    UpdateSkillEffectPos();
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    UpdateShieldEffect();

	return TRUE;
}
//------------------------------------------------------------------------------
/**
*/
void Character::UpdateSkillEffectPos()
{
    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
		SKILL_EFFECT *pEffect = m_SkillEffectArray[i];
        pEffect->UpdateEffect( this );
    }
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::RemoveSkillEffectWithStatus(eCHAR_STATE_TYPE StateType)
{
	int iSkillEffectID = -1;
	BOOL bForceDestroy = TRUE;

    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
		SKILL_EFFECT* pEffect = m_SkillEffectArray[i];
		if (pEffect->dwStatusID == StateType)
		{
			iSkillEffectID = pEffect->iEffectID;
			break;
		}
	}


	// Status 에 해당하는effect를 찾았으면 삭제.
	if (iSkillEffectID >= 0)
	{
		return DeleteSkillEffect(iSkillEffectID);
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
void Character::DoPeriodicDamage(DAMAGETYPE wDamage,DWORD dwTargetKey,DWORD dwTargetHP,DWORD timeSerial,DWORD dwSkillCode)
{
    Character* target_character = g_ObjectManager.GetCharacter(dwTargetKey);
    if (target_character != NULL)
    {
        target_character->ProcessReflectDamage();
    }

	ATTACK_RESULT attret;
	ZeroMemory(&attret,sizeof(attret));

	attret.SetSkillCode(dwSkillCode);
	attret.SetTimeSerial(timeSerial);

	attret.GetTargetData().SetTargetObjectKey( dwTargetKey );
	attret.GetTargetData().SetTargetHP( dwTargetHP );

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    if (target_character && target_character->IsKindOfObject(PLAYER_OBJECT))
    {
        attret.GetTargetData().SetTargetSD(target_character->GetSD());    
    }
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

	attret.GetTargetData().SetDamage( wDamage );
	attret.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DONOTPLAYANIMATION );

	_ProcessAttackResult(&attret);	
}
//------------------------------------------------------------------------------ 
void Character::AddReflectDamage(DAMAGETYPE damage, DWORD target_object_key, DWORD target_hp, DWORD time_serial, DWORD skill_code)
{
    ATTACK_RESULT attret;
    ZeroMemory(&attret, sizeof(attret));

	attret.SetSkillCode(skill_code);
    attret.SetTimeSerial(time_serial);
	attret.GetTargetData().SetTargetObjectKey( target_object_key );
	attret.GetTargetData().SetTargetHP( target_hp );
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    Character* target_character = g_ObjectManager.GetCharacter(target_object_key);
    if (target_character && target_character->IsKindOfObject(PLAYER_OBJECT))
    {
        attret.GetTargetData().SetTargetSD(target_character->GetSD());    
    }
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
	attret.GetTargetData().SetDamage( damage );
	attret.GetTargetData().AddAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_DONOTPLAYANIMATION );

    m_ReflectDamageList.push_back(attret);
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetFightingEnergy(int count)
{
	m_iFightingEnergy = count;
}

//------------------------------------------------------------------------------
/**
*/
int Character::GetFightingEnergy()
{
	return m_iFightingEnergy;
}


//------------------------------------------------------------------------------
/**
*/
void Character::SetIdleAnimation()
{
	WzID animID;
	WzID currentanimID;

	currentanimID = GetCurrentAnimation();

	// 어택스탠드 때문에 지속적으로 애니메이션세팅을 다시해준다.
	animID = GetCurrentIdleAnimation();

	if( animID != currentanimID )
	{
		SetAnimation( animID, TRUE );		

		// 0이 아니었으면 Blend
		if (currentanimID != 0) 
        {
			GetBasePart()->SetInterpolationFrame(6);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetMoveAnimation()
{
	WzID animID;

	// 어택스탠드 때문에 지속적으로 애니메이션세팅을 다시해준다.
	animID = GetCurrentMoveAnimation();
	if (animID != GetCurrentAnimation())
	{
		SetAnimation( animID, TRUE );		
	}
}



//-------------------------------------------------------------------------------------------
/**
*/
void Character::SetRunAwayAnimation()
{
    WzID animID;

    // 어택스탠드 때문에 지속적으로 애니메이션세팅을 다시해준다.
    animID = GetCurrentRunAwayAnimation();
    if (animID != GetCurrentAnimation())
    {
		SetAnimation( animID, TRUE );		
    };
}



//------------------------------------------------------------------------------
/**
*/
BOOL Character::GetAttackTargetList(DWORD dwAttackSerial,int *pNum,DWORD *pTarget, int target_buffer_size)
{
	ATTACK_RESULT_LIST::iterator itr,tempitr;
    itr = m_AttackResultList.begin();

	BOOL bFind = FALSE;	
	int iCount = 0;	

	while(itr != m_AttackResultList.end())
    {
        if (itr->GetAttackSerial() == dwAttackSerial)
        {
			pTarget[iCount] = itr->GetTargetData().GetTargetObjectKey();
			iCount++;	

            if (iCount >= target_buffer_size - 1)
            {
                break;
            }
        }

		itr++;
    }

	*pNum = iCount;

	return (iCount ? TRUE : FALSE);
}

//------------------------------------------------------------------------------
/**
*/
void Character::OnStartMove()
{
	;
}

//------------------------------------------------------------------------------
/**
*/
void Character::OnStartRun()
{
	;
}

//------------------------------------------------------------------------------
/**
*/
void Character::OnAttack(DWORD dwSkillCode)
{

}

//------------------------------------------------------------------------------
/**
*/
void Character::OnDamage(const DAMAGE_PRAM &rDamage)
{		
	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)rDamage.pResult->GetSkillCode());
	if(pInfo)
	{
        SkillEffectFunc::CreateSkillEventEffect( pInfo->m_SkillClassCode, this, cSkillEventEffectInfoData::EVENT_TYPE_DAMAGE );
	}

	WzVector wvEffectPos = GetArrowTargetPos();
    wvEffectPos.z += GetHeight();

#ifndef _PERF_DEBUG

	if(pInfo)
	{
		switch (pInfo->m_SkillClassCode)//스킬에 따른 이펙트 처리 
		{
		case eSKILL_FIRE_ARROW://불계열
		case eSKILL_FIRE_BALL:
		case eSKILL_FIRE_SPEAR:
		case eSKILL_ETHER_BLASE:
		case eSKILL_HEAL_MAGIC_ARRAY:
		case eSKILL_STAR_FALL:
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("F020"),wvEffectPos,rDamage.dwObjKey,GetObjectKey());
			break;

		case eSKILL_BEAST_MASTER_DEADLY_STRIKE:
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("F020"),wvEffectPos,rDamage.dwObjKey,GetObjectKey());
			// 스파크
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A004"),wvEffectPos,rDamage.dwObjKey,GetObjectKey());		
			// 피
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A007"),wvEffectPos,rDamage.dwObjKey,GetObjectKey());
			// 백쇼크
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A009"),wvEffectPos,rDamage.dwObjKey,GetObjectKey());
			break;

		case eSKILL_ICE_DAGGER://얼음 계열 
		case eSKILL_ICE_DAGGER2:
		case eSKILL_ICE_BLAST:
		case eSKILL_ICE_BOLT:
		case eSKILL_PRESSING_WALL:
		case eSKILL_FROZON_SHIELD:
		case eSKILL_FROZEN_ARROW:
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("F030"),wvEffectPos,rDamage.dwObjKey,GetObjectKey());
			break;

		case eSKILL_INQUIRY:
		case eSKILL_WIND_BOLT: //라이팅 계열
		case eSKILL_HEAVY_SHOT:
		case eSKILL_LIGHTING:
		case eSKILL_SHOCKWAVE:
		case eSKILL_LIGHTING_WAVE:
		case eSKILL_ELECTRIC_FIELD:
		case eSKILL_WIND_SHIELD:
		case eSKILL_ARCHBISHOP_OF_DARKNESS_DOOM:
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("F040"),wvEffectPos,rDamage.dwObjKey,GetObjectKey());
			break;

		case eSKILL_POISON_THORN: //독 계열
		case eSKILL_SONY_VINE:
		case eSKILL_POISON_RAIN:
		case eSKILL_POWER_WAVE: //?
			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("F011"),wvEffectPos,rDamage.dwObjKey,GetObjectKey());
			break;

		}
	}
#endif


	WzVector vScale = c_UnitVector;

 	if ( rDamage.pResult->GetTargetData().CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_CRITICAL) == FALSE )
	{
		SetVector(&vScale, 0.5f, 0.5f, 0.5f);
	}
 
	if ( rDamage.pResult->GetTargetData().CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_METAL ) == TRUE )
	{
		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E265"), wvEffectPos, vScale);
	}

	if ( rDamage.pResult->GetTargetData().CheckAdditionalEffect( ATTACK_ADDITIONAL_EFFECT_ETHERDEVICE_ETHER ) == TRUE )
	{
		GlobalFunc::CreateEffectBeUpToOption(StrToWzID("E270"), wvEffectPos, vScale);
	}


}


//-------------------------------------------------------------------------------------------
/**
*/
WzID Character::GetCurrentRunAwayAnimation()
{
    return 0;
}


//------------------------------------------------------------------------------
/**
*/
WzID Character::GetCurrentReviveAnimation()
{
	return 0;
}


//------------------------------------------------------------------------------
/**
*/
WzID Character::GetCurrentTransFormAnimation()
{
	WzID animationID = 0;	
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	{
		szAniCode[0] = 'T';
		szAniCode[1] = '0';
		szAniCode[2] = '0';
		szAniCode[3] = '1';
	}


	animationID = StrToWzID(szAniCode);
	return animationID;
}


//------------------------------------------------------------------------------
/**
*/
WzID Character::GetCurrentBlockAnimation()
{
	return 0;
}

//------------------------------------------------------------------------------
/**
*/
WzID Character::GetCurrentCamAnimation()
{
	return 0;
}


//------------------------------------------------------------------------------
/**
*/
WzID Character::GetCurrentAttackJumpAnimation()
{
    return 0;
}

//------------------------------------------------------------------------------
/**
*/
WzID Character::GetCurrentKnockBack2Animation()
{
	return 0;
}

//------------------------------------------------------------------------------
/**
*/
void Character::OnMiss(BOOL bCreateDamageMesh)
{
	SetAttackStandTime(ATTACK_STAND_TIME);
	
	WzColor popupcolor;

	if (IsKindOfObject(MONSTER_OBJECT))
	{
		popupcolor = c_MonsterHPDamageColor;	
	}
	else
	{
		popupcolor = c_PlayerHPDamageColor;		
	}

	if (bCreateDamageMesh) 
    {		
		CreatePopupMesh(0, 0, eDAMAGEPOPUP_BLOCK, popupcolor, FALSE, FALSE);
	}
    
    PlayBlockAnimation();	
}

//------------------------------------------------------------------------------
/**
*/
void  Character::OnUseSkill()
{
	return;
}

//------------------------------------------------------------------------------
/**
*/
void  Character::OnShotFire(int iShotCount)
{
	return;
}

//------------------------------------------------------------------------------
/**
*/
void  Character::OnShotEnd()
{
	return;
}

//------------------------------------------------------------------------------
/**
*/
void Character::RenderTextOnHead(TCHAR *szString,WzColor color,int yoffset,BOOL bForceRender, bool bIsCharacterScene)
{
	if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ)
#ifdef _NA_004756_120504_SHOW_SSQ_PLAYER_NAME
        && ((g_pHero->IsGM() == FALSE) || (GENERALPARAM->IsShowSSQPlayerName() == FALSE))
#endif //_NA_004756_120504_SHOW_SSQ_PLAYER_NAME
        )
    {
		return;
    }

	// 강제로 랜더링 하는 경우가 아니라면, 대화를 표시하고 있는경우 패스
	if ( GetChatDisplayTime() && !bForceRender)
	{
		return;
	}

	if (m_bObserverMode)
	{
		return;
	}


	if (g_InterfaceManager.GetShowFlag() || bForceRender) 
    {
		float fDepth;

		WzVector wzVisualPos = GetVisiblePos();
		WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
		g_pSunRenderer->Set2DMode(TRUE);
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont(StrToWzID("m217"));
		
		//WzVector wzPos = GetPosition();				
		wzVisualPos.z += GetMeshHeight();
		wzVisualPos.z += HEIGHT_GAP;

        //if (is_spread_wings())
        //{
        //    //! 날개 상태이면 위로 좀더 올려 준다.
        //    wzVisualPos.z += 1.4f;
        //}
		
		POINT pt2;
		g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos ,&fDepth);		
	
		RECT rc;
		SIZE textsize;

		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szString, lstrlen(szString), &textsize);

		rc.left = pt2.x - (textsize.cx / 2);
		if (IsKindOfObject(MONSTER_OBJECT))
			rc.top = pt2.y + (yoffset * textsize.cy) - 22;
		else
			rc.top = pt2.y + (yoffset * textsize.cy)-1;
		
		if (bIsCharacterScene && (rc.top < 0))
			rc.top = (yoffset * textsize.cy);

		rc.right = rc.left + textsize.cx;
		rc.bottom = rc.top + 50;

		WzColor BackColor = WzColor_RGBA(0,0,0,100);		

		
		if( m_bShowName)
			RenderBlockedText(szString,&rc,color,BackColor);
		else
		{
			if(TRUE == this->IsKindOfObject(PLAYER_OBJECT))
            {
                Player* pPlayer = (Player*)this;
                if( pPlayer->IsShowVedorTitle() && pPlayer->GetBeHaveState() == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE )
                {
                    RenderBlockedTextWithDepthTest(szString,&rc,color,BackColor,fDepth,false);
                }
                else
                {
                    RenderBlockedTextWithDepthTest(szString,&rc,color,BackColor,fDepth,true);		
                }
            }
			else
            {
				RenderBlockedTextWithDepthTest(szString,&rc,color,BackColor,fDepth,false);	
            }
	
		}


		g_pSunRenderer->x_pManagerTextOut->EndUseFont();
		
		g_pSunRenderer->Set2DMode( FALSE );
		g_pSunRenderer->SetColor(oldcolor);
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::CreateEffectAtHead(WzID effectID,float fHeight)
{
	WzVector wvEffectPos = GetVisiblePos();
	wvEffectPos.z += fHeight;
	wvEffectPos.z += m_fHeight;

	GlobalFunc::CreateEffectBeUpToOption(effectID,wvEffectPos,GetObjectKey());
}


//------------------------------------------------------------------------------
/**
*/
void Character::RenderInfo()
{
	Object::RenderInfo();
}

//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsRangedNormalAttack()
{
	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
WzVector Character::GetArrowBonePos(int index)
{
	return GetVisiblePos();
}

//------------------------------------------------------------------------------
/**
*/
WzVector Character::GetHandBonePos(int index)
{
	WzVector ret = GetVisiblePos();
	int boneindex = -1;

	if (!IsAnimated())
	{
		AnimateUnit();
	}

	if (GetBasePart()) 
    {
		if (GetBasePart()->GetWzUnit()) 
        {
			WzBoneInfo *pInfo;
			if(index)//오른손
			{
				pInfo= GetBasePart()->GetWzUnit()->GetBoneInfo(StrToWzID("RHND"));
			}
			else
			{
				pInfo= GetBasePart()->GetWzUnit()->GetBoneInfo(StrToWzID("LHND"));
			}
		
			if (pInfo) 
            {
				boneindex = pInfo->m_iNode;
			}
		}
	}

	if (boneindex != -1)
	{
		CWzUnitDraw *pSkeleton = GetBasePart();
		if (pSkeleton) 
		{
			WzMatrix *pBoneMatrix = pSkeleton->GetTransformBuffer();
			if (pBoneMatrix) 
			{			
                WzVector vPos;vPos.x = vPos.y = vPos.z = 0.0f;
                GameFunc::GetBonePos( GetBasePart(), boneindex, &vPos);

                ret.x = vPos.x;
                ret.y = vPos.y;
                ret.z = vPos.z;
			}
		}
	}

	return ret;
}

WzVector Character::GetWeaponBonePos(int index)
{
	WzVector ret = GetVisiblePos();
	int boneindex = -1;

	if (!IsAnimated())
	{
		AnimateUnit();
	}

	if (GetBasePart()) 
    {
		if (GetBasePart()->GetWzUnit()) 
        {
			WzBoneInfo *pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(StrToWzID("ARRW"));//일반적인 무기본은 고정하고 쓰자.

			if (pInfo) 
            {
				boneindex = pInfo->m_iNode;
			}
		}
	}

	if (boneindex != -1)
	{
		CWzUnitDraw *pSkeleton = GetBasePart();
		if (pSkeleton) 
		{
			WzMatrix *pBoneMatrix = pSkeleton->GetTransformBuffer();
			if (pBoneMatrix) 
			{	
                WzVector vPos;vPos.x = vPos.y = vPos.z = 0.0f;	
                GameFunc::GetBonePos( GetBasePart(), boneindex, &vPos);

                ret.x = vPos.x;
                ret.y = vPos.y;
                ret.z = vPos.z;
			}
		}
	}

	return ret;
}

//------------------------------------------------------------------------------ 
bool Character::GetBodyBonePos(OUT WzVector* pos)
{
    int boneindex = -1;

    if (GetBasePart() && GetBasePart()->GetWzUnit()) 
    {
        WzID wiBodyID = StrToWzID("BODY");
        WzBoneInfo *pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(wiBodyID);

        if ((pInfo != NULL) && (pInfo->m_wiBone == wiBodyID))
        {
            boneindex = pInfo->m_iNode;
        }
    } 

    if ((boneindex != -1) && (GetBasePart() != NULL) && (GetBasePart()->GetTransformBuffer() != NULL))
    {
        if (pos != NULL)
        {
            
            WzVector vPos;vPos.x = vPos.y = vPos.z = 0.0f;
            if(GameFunc::GetBonePos( GetBasePart(), boneindex, &vPos) == FALSE )
            {
                return false;
            }

            (*pos).x = vPos.x;
            (*pos).y = vPos.y;
            (*pos).z = vPos.z;
        }

        return true;
    }

    return false;
}
//------------------------------------------------------------------------------ 
WzVector Character::GetBodyPos()
{
	WzVector ret;
    if (GetBodyBonePos(&ret) == false)
    {
        // 본이 없을때는 현재 위치에서 메쉬 높이의 1/2 만큼 올린다.
        ret = GetVisiblePos();
        ret.z += (GetMeshHeight() * 0.5f);
    }

	return ret;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetTerrainAttribute()
{
	return m_dwTerrainAttribute;
}



//------------------------------------------------------------------------------
/**
*/
void Character::OnFootStep(BOOL b2D /*=FASLE*/)
{
	Object::OnFootStep();
}

//------------------------------------------------------------------------------ 
float Character::GetWaterEffectSizeFromBoundingVolume()
{
    WzBoundingVolume* bv = GetBoundingVolume();
    if (bv != NULL)
    {
        if (bv->m_eBvType == BVT_AABB)
        {
            WzVector diff = bv->m_aabb.m_wvMax - bv->m_aabb.m_wvMin;
            diff.z = 0;

            return VectorLength(&diff);
        }
        else if (bv->m_eBvType == BVT_SPHERE)
        {
            return bv->m_BvSphere.m_fRadius * 2.0f;
        }
    }
    return 1.0f;
}

//------------------------------------------------------------------------------
/**
*/
float Character::GetAngleForMoveDirection()
{
	return Math_GetAngleFromVector( &GetDirection() );
}

void Character::OnWaterWave()
{
	if( m_dwTerrainAttribute == eTERRAIN_ATTRIBUTE_WATER )
	{
		WzVector vPos = GetWaterTerrainPos();

		HANDLE hWaterWave = GlobalFunc::CreateEffectBeUpToOption( StrToWzID("I690"), vPos, GetObjectKey() );

		CEffectBody * pBody =  g_pEffectManager->GetEffectPtrHandle( hWaterWave );

		if( pBody )
		{
			WzVector vRot;
			vRot.x = 0.0f;
			vRot.z = GetAngleForMoveDirection();
			vRot.y = 0.0f;

			WzVector vScale;
            float effect_scale = GetWaterEffectSizeFromBoundingVolume();
			SetVector(&vScale, effect_scale, effect_scale, effect_scale);

			pBody->SetResultMatrix( vPos, vRot, vScale );
		}
	}
}

WzVector Character::GetWaterTerrainPos()
{
	WzVector vPos = GetVisiblePos();
	WzVector vTemp;
	SetVector( &vTemp, 0.0f,  0.0f,  0.0f );

	GameFunc::PickTerrainMesh( vPos, vTemp, TMGO_SPECULAR, GetMeshHeight() );

	return vTemp;
}

void Character::OnWaterEllipse()
{
	if( m_dwTerrainAttribute == eTERRAIN_ATTRIBUTE_WATER )
	{
		WzVector vPos = GetWaterTerrainPos();

		CEffectBody * pBody = g_pEffectManager->GetEffectPtrHandle( m_hWaterEllipse );

		if( pBody == NULL )
		{
			m_hWaterEllipse = GlobalFunc::CreateEffectBeUpToOption( StrToWzID("I691"), vPos, GetObjectKey() );
			pBody = g_pEffectManager->GetEffectPtrHandle( m_hWaterEllipse );
		}

		if(pBody)
		{
			WzVector vRot, vScale;

            float effect_scale = GetWaterEffectSizeFromBoundingVolume();
			SetVector( &vRot, 0.0f, 0.0f, 0.0f );
            SetVector(&vScale, effect_scale, effect_scale, effect_scale);

			pBody->SetResultMatrix( vPos, vRot, vScale );

			if( GetCurState() == STATE::KEYBOARDJUMP )
			{
				pBody->SetShow( FALSE );
			}
			else
			{
				pBody->SetShow( TRUE );
			}
		}
	}
	else
	{
		CEffectBody * pBody = g_pEffectManager->GetEffectPtrHandle( m_hWaterEllipse );

		if( pBody )
		{
			WzVector vPos = GetWaterTerrainPos();

			WzVector vRot, vScale;
			SetVector( &vRot, 0.0f, 0.0f, 0.0f );
			SetVector( &vScale, 1.0f, 1.0f, 1.0f );

			pBody->SetResultMatrix( vPos, vRot, vScale );
			pBody->SetShow( FALSE );
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::CreatePopupMesh(
								CODETYPE SkillCode,
                                INT64 iNumber,
                                eDAMAGEPOPUP_KIND eKind,
                                WzColor color,
                                BOOL bCritical,
								BOOL bDoubleDamage,
                                float fMultiplier,
                                int iSecondary,
                                WzColor secondarycol,
                                float fHeight,
								BOOL bHero /*= FALSE*/
								)
{
	DAMAGE_POPUP popup;
	popup.dTime = 0;
	popup.vPos = GetVisiblePos();
	float fMeshHeight;
	fMeshHeight = GetMeshHeight();

	WzVector wzScale = {1.f, 1.f, 1.f};    
	// 데미지가 너무 높은곳에서 나와서 안보이는일이 없도록함
	if (fMeshHeight >= 3.5f)
	{
		fMeshHeight = 3.5f;
	}
	else
	{
		GetScale(&wzScale);
	}
	//* wzScale.z
	popup.vPos.z += fMeshHeight ;
	popup.vPos.z += fHeight;
	popup.SkillCode = SkillCode;
	popup.bHero = bHero;
	popup.iNumber = iNumber;
	popup.iNumber2 = iSecondary;
	popup.Color = color;
	popup.Color2 = secondarycol;
	popup.eKind = eKind;
	popup.bCritical = bCritical;
	popup.fMultiplier = fMultiplier;
	popup.bDoubleDamage = bDoubleDamage;
	g_InterfaceManager.AddDamageList(popup);
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetMeshHeight(float fMeshHeight)
{
	m_fMeshHeight = fMeshHeight;	
}

//------------------------------------------------------------------------------
/**
*/
float Character::GetMeshHeight()
{
	return m_fMeshHeight;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetStandAnimationDelay()
{
    const int STAND_ANIMATION_MINIMUM_DELAY = 8000;
    const int STAND_ANIMATION_DELAY_VARIATION = 7000;

    m_dwStandAnimDelay = 
        STAND_ANIMATION_MINIMUM_DELAY + rand() % STAND_ANIMATION_DELAY_VARIATION;
}

int Character::GetSkillEffectCount(DWORD status_code)
{
    int count = 0;

    for( size_t i=0;i!=m_SkillEffectArray.size();++i)
    {
        SKILL_EFFECT* effect = m_SkillEffectArray[i];
        if( effect->dwStatusID == status_code )
        {
            ++count;
        }
    }

    return count;
}

//------------------------------------------------------------------------------
/**
*/
float Character::GetArrowTargetHeight()
{
	return (GetMeshHeight() * 3.0f / 5.0f);
}

//------------------------------------------------------------------------------
/**
*/
WzVector Character::GetArrowTargetPos()
{
    return GetBodyPos();
}

//------------------------------------------------------------------------------
/**
*/
void Character::OnShotStart()
{
	return;
}

//------------------------------------------------------------------------------
/**
*/
int Character::GetBoneIndex(BYTE byType)
{
	int boneindex = -1;

	if (!IsAnimated())
	{
		AnimateUnit();
	}

	WzID id=0;

	switch (byType)
	{
	case eSKILLEFFECT_POS_HEAD:
		{
			id = StrToWzID("HEAD");
		}
		break;

	case eSKILLEFFECT_POS_RHAND:
		{
			id=StrToWzID("RHND");
		}
		break;

	case eSKILLEFFECT_POS_LHAND:
		{
			id=StrToWzID("LHND");
		}
		break;

	case eSKILLEFFECT_POS_WEAPON:
		{
			id=StrToWzID("ARRW");
		}
		break;

	case eSKILLEFFECT_POS_BODY:
		{
			id=StrToWzID("BODY");
		}
		break;

	case eSKILLEFFECT_POS_BOTTOM:
		{
			id=StrToWzID("BTTM");
		}
		break;

	case eSKILLEFFECT_POS_BUST:
		{
			id=StrToWzID("BUST");
		}
		break;

    case eSKILLEFFECT_ONLY_POS:
        {
            return -1;
        }
        break;

	}


	if (GetBasePart()) 
	{
		if (GetBasePart()->GetWzUnit()) 
		{
			WzBoneInfo *pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(id);//일반적인 무기본은 고정하고 쓰자.

			if (pInfo) 
			{
				boneindex = pInfo->m_iNode;
			}
		}
	}
	
	return boneindex;
}
//------------------------------------------------------------------------------
int Character::GetBoneIndexByWzID(WzID bone_id)
{
	if (!IsAnimated())
	{
		AnimateUnit();
	}

	if (GetBasePart() && GetBasePart()->GetWzUnit())
	{
        WzBoneInfo *pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(bone_id);

        if (pInfo) 
        {
            return pInfo->m_iNode;
        }
	}
	
	return -1;
}
//------------------------------------------------------------------------------ 
WzVector Character::GetSkillEffectRotate(SKILL_EFFECT* skill_effect)
{
    if (skill_effect->bApplyParentRotate)
    {
        WzVector rotate;
        rotate.y = 0.0f;
        rotate.z = GetAngle(); 
        rotate.x = GetAngleX();

        return rotate;
    }
    return c_ZeroVector;
}
//------------------------------------------------------------------------------ 
WzVector Character::GetSkillEffectScale(SKILL_EFFECT* skill_effect)
{
    if (skill_effect->bApplyParentScale)
    {
        return GetScale();
    }
    return c_UnitVector;
}
//------------------------------------------------------------------------------ 
WzVector Character::GetSkillEffectPos(SKILL_EFFECT* skill_effect)
{
    return (GetSkillEffectPos(skill_effect->effect_position_type, skill_effect->effect_position_bone_index ) + skill_effect->effect_position_offset);
}

//------------------------------------------------------------------------------
/**
*/
WzVector Character::GetSkillEffectPos( eSKILLEFFECT_POS position_type, int bone_index )
{
    if( position_type == eSKILLEFFECT_POS_BONE_INDEX )
        return GetBonePos( bone_index );

	switch (position_type)
	{
	default:
	case eSKILLEFFECT_POS_HEAD:
		{
			WzVector vPos = GetVisiblePos();
			float fMeshHeight = GetMeshHeight();

			vPos.z += fMeshHeight;

			return vPos;
		}
		break;

	case eSKILLEFFECT_TARGET_POS:
		{
			Character *pTarget = (Character *)GetTargetObject();
	
			if(pTarget)
				return pTarget->GetVisiblePos();
			else 
				return GetVisiblePos();
		}
		break;

	case eSKILLEFFECT_POS_BODY:
		{
			return GetBodyPos();
		}
		break;

	//case eSKILLEFFECT_ONLY_POS:
	case eSKILLEFFECT_POS_FOOT:
		{
			return GetVisiblePos();
		}
		break;

	case eSKILLEFFECT_POS_RHAND:
		{
			return GetHandBonePos(1);
		}
		break;

	case eSKILLEFFECT_POS_LHAND:
		{
			return GetHandBonePos(0);
		}
		break;

	case eSKILLEFFECT_POS_WEAPON:
		{
			return GetWeaponBonePos(0);
		}
		break;

    case eSKILLEFFECT_ONLY_POS:
        {
            WzVector picked_position;
            GameFunc::PickTerrainMesh(GetVisiblePos(), picked_position, TMGO_NO_COLLISION, 0.5f);
            return picked_position;
        }
        break;
    case eSKILLEFFECT_POS_NONE:
        {
            return GetVisiblePos();
        }
        break;
	}
}


//------------------------------------------------------------------------------
/**
*/
void Character::AddTargetingCount(DWORD dwTargeter)
{
	list<DWORD>::iterator iter;

	iter = m_listTargeter.begin();

	for (;iter != m_listTargeter.end(); iter++)
	{
		if (dwTargeter == *iter)
		{
			return;
		}
	}

	m_listTargeter.push_back(dwTargeter);
}

//------------------------------------------------------------------------------
/**
*/
void Character::RemoveTargetingCount(DWORD dwTargeter)
{
	list<DWORD>::iterator iter;

	iter = m_listTargeter.begin();

	for (;iter != m_listTargeter.end(); iter++)
	{
		if (dwTargeter == *iter)
		{
			m_listTargeter.erase(iter);
			break;
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
int Character::GetTargetingCount()
{
	return m_listTargeter.size();
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetMoveTarget(DWORD dwMoveTarget)
{
	m_dwMoveTarget = dwMoveTarget;

}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetMoveTarget()
{
	return m_dwMoveTarget;

}

//------------------------------------------------------------------------------
/**
*/
void Character::OnDead()
{
	ReleaseMoveTarget();

#ifdef _NA_007113_SHADOW_PATCH_UNTARGET_MONSTER_DIE
    ReleaseHeroTarget();
#endif //_NA_007113_SHADOW_PATCH_UNTARGET_MONSTER_DIE
}

void Character::ReleaseHeroTarget()
{
    DWORD TargetObjectKey = g_HeroInput.GetCurrentTarget();
    if( GetObjectKey() == TargetObjectKey )
    {
        g_HeroInput.SetCurrentTarget(0);
    }
}

//------------------------------------------------------------------------------
/**
*/
void Character::ReleaseMoveTarget()
{
	if (m_dwMoveTarget) 
    {
		Character *pChr = (Character *)g_ObjectManager.GetObject(m_dwMoveTarget);
		if (pChr)
		{
			pChr->RemoveTargetingCount(GetObjectKey());
		}

		m_dwMoveTarget = 0;
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetStartMoveTime( DWORD dwTime )
{
	m_dwStartMoveTime = dwTime;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetStartMoveTime( void )
{
	return m_dwStartMoveTime;
}

//------------------------------------------------------------------------------
/**
*/
void Character::OnParalyzed()
{
	StopMove();
}


//------------------------------------------------------------------------------
/**
*/
BOOL Character::IsInNotprocessedList(DWORD dwAttackSerial)
{
	
	return FALSE;

}

//------------------------------------------------------------------------------
/**
*/
void Character::RemoveNotprocessedList(DWORD dwAttackSerial)
{


}

//------------------------------------------------------------------------------
/**
*/
void Character::AddNotprocessedList(DWORD dwAttackSerial)
{
	m_vNotProcessedList.push_back(dwAttackSerial);

}


//------------------------------------------------------------------------------
/**
*/
void Character::SetNextActionAttackSerial(DWORD dwAttackSerial)
{
	m_dwNextActionAttackSerial = dwAttackSerial;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Character::GetNextActionAttackSerial()
{
	return m_dwNextActionAttackSerial;
}

//------------------------------------------------------------------------------
/**
*/
WzVector Character::GetHeadBonePos()
{
	return GetBonePos(7);
}

//------------------------------------------------------------------------------
/**
*/
void Character::ApplyTerrainColor(BOOL bApply)
{
#ifdef _PERF_DEBUG
	return;
#endif

	int iAlpha = 255;


	if (m_bFade)
	{
		float fAlpha= (float)m_iFadeCurDelay / (float)m_iFadeTotDelay;
		fAlpha = 255.0f * fAlpha;
		iAlpha=(int)fAlpha;
	}

	iAlpha=min(iAlpha,255);

	if (m_pPathExplorer && bApply)
	{
		if (m_pPathExplorer->GetTerrainMesh() != -1)
		{
			if (g_pMap) 
            {
				if (g_pMap->IsLoaded())
				{
					int iR,iG,iB;
					float fT = 10.0f;						

					m_vNormal = c_ZeroVector;

#if( WZENGINEVER >= 290 )
                    // 캐릭터가 딛고 있는 메쉬를 찾는다.
                    // 캐릭터의 (위치+키높이)에서 아래방향으로 지형메쉬를 찾음.
                    WzVector visible_position = GetVisiblePos();
                    float character_height = GetMeshHeight();
                    if (character_height < 1.0f)
                    {
                        character_height = 1.0f;
                    }
                    int mesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand(visible_position, &fT, 5.0f,0,&m_vNormal, character_height);
#else
                    int mesh = g_pMap->GetWorldBase()->PathExp_GetMeshToStand(m_pPathExplorer->GetPos(), &fT, 5.0f,0,&m_vNormal);
#endif

					int meshtexture = -1;
                    if (mesh != -1)
                    {
                        meshtexture = g_pMap->GetWorldBase()->GetTextureIndexOfTerrainMesh(mesh);
                    }

					if (meshtexture != -1) {

						m_dwTerrainAttribute = g_pMap->GetWorldBase()->GetTerrainMeshUserOption(meshtexture);

					}

					if ( mesh < 0 || fT > 5.0f)
					{	
						// 밑에 없거나 거리가 5m 이상인 경우 무시
						
					}
					else 
					{

						float fWeight = GRAPHICPARAM->GetTerrainColorRate();

						WzColor color = g_pMap->GetWorldBase()->GetTerrainLight(m_pPathExplorer->GetPos(),mesh);
		

						float degree = acos(m_vNormal.z);
						degree=n_rad2deg(degree);
						
						if(degree<20.0f)
						{
							iR = int( Red_WzColor(color) * fWeight + (float)255 * (1.0f - fWeight) );
							iG = int( Green_WzColor(color) * fWeight + (float)255 * (1.0f - fWeight) );
							iB = int( Blue_WzColor(color) * fWeight + (float)255 * (1.0f - fWeight) );
							SetColor(iR,iG,iB,iAlpha);
						}
					}
					
				}
			}
		}
	}

	//서있는 곳이 없거나 길찾기가 없을 서있는 면이 90도 보다 클경우 칼라가 없을수도  있다.
	WzColor LastColor = GetColor();
	SetColor(Red_WzColor(LastColor),Green_WzColor(LastColor),Blue_WzColor(LastColor),iAlpha);

}


//-------------------------------------------------------------------------------------------
/**
*/
int Character::GetEmoticonIcon() const
{
	return m_iEmoteIcon;
}

//------------------------------------------------------------------------------
/**
*/
void Character::ShowEmoticonIcon(int iEmote,int iDelay)
{
	m_iEmoteIconDelay = iDelay;

	if (0 == iEmote && m_pEmoteDraw)
	{
		m_iEmoteIcon = iEmote;
		ResourceManager::Instance()->FreeResourceUnitDraw(m_pEmoteDraw);
		m_pEmoteDraw = NULL;
	}
	else
	if (m_iEmoteIcon != iEmote)
	{
		m_iEmoteIcon = iEmote;
		if (m_iEmoteIcon)
		{
			BASE_HeadMeshInfo *pInfo = HeadMeshInfoParser::Instance()->GetHeadMeshInfo(m_iEmoteIcon);

			if (pInfo)
			{
				if (m_pEmoteDraw)
				{
					ResourceManager::Instance()->FreeResourceUnitDraw(m_pEmoteDraw);
					m_pEmoteDraw = NULL;
				}

				m_pEmoteDraw = ResourceManager::Instance()->AllocResourceUnitDraw(pInfo->m_dwIcon + ENGINE_RESOURCE_ID_OFFSET);

                if (m_pEmoteDraw->GetWzUnit() != NULL)
                {
				    int iAniCnt = m_pEmoteDraw->GetWzUnit()->GetCountAnimationInfo();
				    if (iAniCnt > 0)
				    {
					    WzAnimationInfo * pwzAniInfo = m_pEmoteDraw->GetWzUnit()->GetAnimationInfoByIndex(0);

					    if (pwzAniInfo) 
					    {
						    pwzAniInfo->m_wiAction;
						    m_pEmoteDraw->SetCurrentAnimation( pwzAniInfo->m_wiAction, TRUE );
					    }
				    }
                }
			}
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::ProcessEmoteDelay(DWORD dwTick)
{
	if (m_iEmoteIconDelay > 0 ) 
	{
		m_iEmoteIconDelay -= dwTick;

		if (m_iEmoteIconDelay <= 0)
		{
			FreeEmoteIcon();			
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::FreeEmoteIcon()
{
	m_iEmoteIcon = 0;
	m_iEmoteIconDelay = 0;

	if (m_pEmoteDraw)
	{
		ResourceManager::Instance()->FreeResourceUnitDraw(m_pEmoteDraw);
		m_pEmoteDraw = NULL;
	}
}

//------------------------------------------------------------------------------
/**
*/
void Character::StopVoice()
{
	if (m_iVoiceHandle != -1)
	{
		g_SoundEffect.StopVoice(m_iVoiceHandle);
		m_iVoiceHandle = -1;
	}
}

//----------------------------------------------------------------------------
/**
*/
BOOL Character::CanUseSkillLimitMP(SLOTCODE skillID, BOOL bShowFailMessage /*= FALSE*/)
{
	TCHAR szFailMessage[INTERFACE_STRING_LENGTH];

	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(skillID);
	assert(pInfo);

	if (!pInfo)
	{
		return FALSE;
	}

	if (GetMP() < pInfo->m_wMPSpend)
	{
		if (bShowFailMessage)
		{
			g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NOT_ENOUGH_MP,szFailMessage,INTERFACE_STRING_LENGTH);
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
BOOL Character::CanUseSkillLimitHP(SLOTCODE skillID, BOOL bShowFailMessage /*= FALSE*/)
{
	TCHAR szFailMessage[INTERFACE_STRING_LENGTH];

	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(skillID);
	assert(pInfo);

	if (!pInfo)
	{
		return FALSE;
	}

    // 여기다가 어빌리티 345에 대한 처리를 해주어야 할 것 같다.
    SkillScriptInfo* skill_info_ptr = SkillInfoParser::Instance()->GetSkillInfo(static_cast<SLOTCODE>(skillID));
    BASE_ABILITYINFO* ability_info = NULL;
    if ( skill_info_ptr )
        ability_info = skill_info_ptr->GetAbilityInfoByIndex(0);
    
    if ( ability_info && ability_info->m_wAbilityID == eABILITY_VARIATION_PUREVALUE_HP
        && this == g_pHero)
    {
        int need_hp = ability_info->m_iParam[1]; // 소모 HP        
        switch ( ability_info->m_iParam[0] )
        {
        case VALUE_TYPE_VALUE:
            {
                // 소모 HP그대로 적용
            }
            break;
        case VALUE_TYPE_PERCENT_PER_CUR:
            {
                need_hp = static_cast<int>(this->GetHP() * (need_hp / 1000.0f));
            }
            break;
        case VALUE_TYPE_PERCENT_PER_MAX:
            {
                need_hp = static_cast<int>(this->GetMaxHP() * (need_hp / 1000.0f));
            }
            break;
        case VALUE_TYPE_RATIO_VALUE:
            {                
                // VALUE_TYPE_RATIO_VALUE 주로 데미지 같은데서 사용.
                assert(!"345(eABILITY_VARIATION_PUREVALUE_HP) 부적절한 타입");
            }
            break;
        default:
            {
                // no operator                
            }
            break;
        }
        need_hp = this->GetHP() + need_hp;
        if (need_hp <= 0)
        {
            TCHAR skill_name[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(skill_info_ptr->m_dwSkillNcode, skill_name, INTERFACE_STRING_LENGTH);

            //7019	%s 스킬을 사용하기엔 체력이 너무 적습니다.
            if ( bShowFailMessage )
            {
                g_InterfaceManager.GetInterfaceStringFormat(szFailMessage,
                    INTERFACE_STRING_LENGTH,
                    7019,
                    skill_name);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szFailMessage);
            }

            return FALSE;
        }
    }

	// HP제한 체크
	if (GetHP() < pInfo->m_wHPSpend)
	{
		if (bShowFailMessage)
		{
			g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_NOT_ENOUGH_HP,szFailMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);
		}

		return FALSE;
	}
    

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
BOOL						
Character::CanUseSkillLimitCoolTime(SLOTCODE skillID, BOOL bShowFailMessage /*= FALSE*/)
{
	TCHAR szFailMessage[INTERFACE_STRING_LENGTH];

	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo(skillID);
	assert(pInfo);
	if (!pInfo)
	{
		return FALSE;
	}

    if ( !g_pHero )
    {
        return FALSE;
    }

	util::Timer *pTimer = xGetCooltimeManager()->GetSkillCooltime(skillID);
	if (!pTimer) return FALSE;

	// 쿨타임이 아직 안지났으면 못쓴다~
	if (!pTimer->IsExpired(FALSE))
	{
		if (bShowFailMessage)
		{
			g_InterfaceManager.GetInterfaceString(eST_SKILL_FAIL_COOLTIME_NOT_EXPIRED,szFailMessage,INTERFACE_STRING_LENGTH);
			GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT,szFailMessage);			
		}

		return FALSE;
	}

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
float Character::GetRangeWithHero()
{
	float fRet = 99999999.9f;

	if (g_pHero)
	{
		WzVector vDiff = GetPosition() - g_pHero->GetPosition();

		fRet = VectorLength2(&vDiff);
	}
	
	return fRet;
}

//------------------------------------------------------------------------------
/**
*/
int Character::GetShotCount()
{
	return 1;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetNoCull(BOOL bFlag)
{
	m_bNoCull = bFlag;
}

//------------------------------------------------------------------------------
/**
*/
void Character::SetCurrentMoveSpeed()
{
	
}


void Character::SetObserverMode( BYTE byObserverMode, BOOL IsHero )
{
	m_bObserverMode = byObserverMode;

	if( IsHero )
	{
		if( m_bObserverMode )
		{
			
			SetShow( FALSE );
		}
		else
		{
			
			SetShow( TRUE );
		}
	}
	else
	{
		if( m_bObserverMode )
		{
			SetShow( FALSE );
		}
		else
		{
			SetShow( TRUE );
		}
	}
}


BOOL Character::IsObserverMode( void )
{
	return m_bObserverMode;
}


//------------------------------------------------------------------------------
/**
*/

DWORD Character::GetCurrentTileAtt( BOOL bForce )
{
	if( bForce )
	{
		DWORD dwAtt = Object::GetCurrentTileAtt();
		
		return dwAtt;
	}

	return m_dwCurTileAtt;
}

//------------------------------------------------------------------------------
/**
*/


DWORD ConvertServerToClientEffect(DWORD dwEffect)
{
	DWORD dwTempEffect=0;

	if(BIT_CHECK(dwEffect,SKILL_EFFECT_KNOCKBACK))
	{
		BIT_ADD(dwTempEffect,ATTACK_ADDITIONAL_EFFECT_KNOCKBACK);
	}

	if(BIT_CHECK(dwEffect,SKILL_EFFECT_KNOCKDOWN))
	{
		BIT_ADD(dwTempEffect,ATTACK_ADDITIONAL_EFFECT_DOWN);
	}
	
	
	if(BIT_CHECK(dwEffect,SKILL_EFFECT_STUN))
	{
		BIT_ADD(dwTempEffect,ATTACK_ADDITIONAL_EFFECT_STUN);

	}

	if(BIT_CHECK(dwEffect,SKILL_EFFECT_CRITICAL))
	{
		BIT_ADD(dwTempEffect,ATTACK_ADDITIONAL_EFFECT_CRITICAL);

	}

	return dwTempEffect;
}

//------------------------------------------------------------------------------
/**
*/
RESOURCECODE Character::sTransformData::GetTransFormResourceCode()
{
	if( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ )
		return 11575;
	else
		return m_TransFormResourceCode;
}

// [3/10/2011 iforall]
// Player, PlayerMonster 클래스에서 Appearance/Equip 관련 공통코드 가져옴
// 애니매이션/전투/액션 등에 관한 공통코드도 가져온 이후 분리작업 필요
void Character::AnimateUnit()
{
    Object::AnimateUnit();

    if( GlobalFunc::IsSSQScene() == FALSE )
        ProcessFullsetEffect(this);

#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
    // 이펙트가 본보다 늦게 따라오는 현상이 있어 AnimateUnit() 뒤로 옴김
    UpdateSkillEffectPos();
#endif //_NA_007294_20140613_DRAGONKNIGHT_RENEWAL
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (GetClass() == eCHAR_WITCHBLADE)
    {
        ProcessFPFullEffect(this);
        UpdateViceSwordWeaponEffect();
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}

float GetHeightInfo(eCHAR_TYPE class_type,int height_variation)
{
    float unit_scale = 1.0f;

    if (GlobalFunc::IsSSQScene())
    {
        return unit_scale;
    }

    HeroHeightInfoParser* parser_instance  = HeroHeightInfoParser::Instance();

    BASE_HeroHeightInfo* player_height_info = parser_instance->GetHeroHeightInfo(class_type);

    assert(height_variation >= 0 && height_variation < MAX_HEIGHT_VARIATION);

    if (player_height_info)
    {
        unit_scale = player_height_info->m_fHeight[height_variation];
    }

    return unit_scale;
}

BOOL Character::IsBodyOnlyRenderMode()
{
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    if ((GlobalFunc::IsSpaScene() == TRUE) &&
        (((GetObjectKey() > ONLYCLIENT_OBJECT_KEY) && (GetObjectKey() < MAX_OBJECT_KEY)) == false) && // 추가 생성된 객체 제외 (순수 사람만)
        (IsKindOfObject(PLAYER_OBJECT) == TRUE))
    {
        return TRUE;
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_004035_20120227_COSTUME_ITEM
    if( GetCostumeItemCode() != 0 && GameFramework::GetCurScene() != SCENE_TYPE_SSQ 
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        && isHideCostume() == false
#endif
        )
    {
        return TRUE;
    }
#endif //_NA_004035_20120227_COSTUME_ITEM
    return FALSE;
}

void Character::RefreshAppearRender(BOOL refresh_bone /*= FALSE*/)
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return;
    }

    if (IsUseDelayLoading() == true)
    {
        return;
    }

    APPEARANCEINFO parts_info;
    ZeroMemory(&parts_info, sizeof(parts_info));

    if (IsTransForm())
    {
        //용변신상태에서는 RefreshAppearRender 호출되는걸 막아줘야된다
        parts_info.ObjectType = APPEARANCEINFO::PLAYERTRANSFORM;	
        parts_info.resourceCode = GetTransformData().GetTransFormResourceCode();
        RefreshRender(&parts_info, refresh_bone);
        //SetIdleAnimation();
    }
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    else if ((GlobalFunc::IsSpaScene() == TRUE) &&
        (((GetObjectKey() > ONLYCLIENT_OBJECT_KEY) && (GetObjectKey() < MAX_OBJECT_KEY)) == false) && // 추가 생성된 객체 제외 (순수 사람만)
        (IsKindOfObject(PLAYER_OBJECT) == TRUE))
    {
        //온천외형
        RefreshSpaRender(refresh_bone);
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
#ifdef _NA_004035_20120227_COSTUME_ITEM
    else if( GetCostumeItemCode() != 0 && GameFramework::GetCurScene() != SCENE_TYPE_SSQ 
#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
        && isHideCostume() == false
#endif
        )
    {

        DWORD class_code = GetClass();
        parts_info.ObjectType = APPEARANCEINFO::PLAYER;
        parts_info.Job = class_code;

        if( const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo( (CODETYPE)GetCostumeItemCode() ) )
        {
            // 머리
            parts_info.Equipment[BP_HAIR].dwItemID = GetPartVariation(PLAYER_VARIATION_HAIR);
            parts_info.Equipment[BP_HAIR].iEnchant = GetGender() == GenderType::kFemale ? item_info->m_bHeadType1 : item_info->m_bHeadType;

            parts_info.Equipment[BP_FACE].dwItemID = GetPartVariation(PLAYER_VARIATION_FACE);

            // 방어구
            parts_info.Equipment[BP_ARMOR].dwItemID = GetCostumeItemCode();
        }

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
		if ( GetDecorateCostumeItemCode() != 0 )
		{
			parts_info.Equipment[BP_DECORATE_COSTUME].dwItemID = GetDecorateCostumeItemCode();
		}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

        // 무기
        int equip_pos = equipment_container->GetEquipPosByParts(BP_WEAPON);
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(equipment_container->GetSlot(equip_pos));
        if ( (TRUE == this->IsMyHero()) || (this->GetObjectKey() == EQUIPMENT_CHARACTER_KEY) )
        {
            ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = g_pHero->GetEquipmentContainer()->GetEquipmentManageInfo();
            ns_formula::EquipSlotsManageInfo::Node* it = equip_slot_manage_info->slots;
            if (equip_slot_manage_info->slots[equip_pos].activated == true) // 아이템이 비활성화인 경우 적용안함
            {
                SetEquipAppearInfo( &parts_info.Equipment[BP_WEAPON], item_slot );
            }
        }
        else
        {
            SetEquipAppearInfo( &parts_info.Equipment[BP_WEAPON], item_slot );
        }

        // 악세서리(날개)
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
        for( int i=BP_SPECIAL_ACCESSORY1;i<=BP_SPECIAL_ACCESSORY3;++i)
        {
            int equip_pos = equipment_container->GetEquipPosByParts(i);
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(equipment_container->GetSlot(equip_pos));            
            if ( is_spread_wings() == true && item_slot.IsWingItem() == true )
            {
                SetEquipAppearInfo(&parts_info.Equipment[i], item_slot);
            }
        }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING


        //파츠 갱신
        RefreshRender(&parts_info, refresh_bone);

        ReleaseFullsetEffect();

        class_code = GetImageClass();

        SetScale(GetHeightInfo(static_cast<eCHAR_TYPE>(class_code), GetHeightVariation()));
    }
#endif//_NA_004035_20120227_COSTUME_ITEM
    else
    {
        DWORD class_code = GetClass();
        parts_info.ObjectType = APPEARANCEINFO::PLAYER;
        parts_info.Job = class_code;

        for (int parts_position = (BP_BONE + 1); parts_position < BP_COUNT; ++parts_position)
        {
            int equip_pos = equipment_container->GetEquipPosByParts(parts_position);

            //외형에 영향을 주는 파츠가 아니다.
            if (equip_pos == EquipmentContainer::EQUIP_MAX)
            {
                continue;
            }

            // 슬롯이 비었을경우(헬멧은 예외처리된다)
            if (equipment_container->IsEmpty(equip_pos))
            {
                CheckEmptyHelmet(equip_pos, 
                    parts_info, 
                    GetPartVariation(PLAYER_VARIATION_HAIR), 
                    GetPartVariation(PLAYER_VARIATION_FACE));
                continue;
            }

            SCItemSlot& item_slot = static_cast<SCItemSlot&>(equipment_container->GetSlot(equip_pos));
    
            bool is_visible = false;

            if ( (TRUE == this->IsMyHero()) || (this->GetObjectKey() == EQUIPMENT_CHARACTER_KEY) )
            {
                ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = g_pHero->GetEquipmentContainer()->GetEquipmentManageInfo();
                ns_formula::EquipSlotsManageInfo::Node* it = equip_slot_manage_info->slots;
                if (equip_slot_manage_info->slots[equip_pos].activated == false) // 아이템이 비활성화인 경우 적용안함
                {
                    CheckEmptyHelmet(equip_pos, 
                        parts_info, 
                        GetPartVariation(PLAYER_VARIATION_HAIR), 
                        GetPartVariation(PLAYER_VARIATION_FACE));
                    continue;
                }
            }

            //헬멧일 경우는 예외처리
            if (equip_pos == EquipmentContainer::EQUIP_HELMET)
            {
                SetEquipAppearHelmet(parts_info, 
                    item_slot, 
                    GetPartVariation(PLAYER_VARIATION_HAIR), 
                    GetPartVariation(PLAYER_VARIATION_FACE), 
                    IsHideHelmet()
                    , GetGender()
                    );
            }
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
            else if (equip_pos >= EquipmentContainer::EQUIP_SACCESSORY1 && equip_pos <= EquipmentContainer::EQUIP_SACCESSORY3)
            {
                if (!is_spread_wings() && item_slot.IsWingItem() && IsHideWing())
                {
                    SetEquipAppearInfo(NULL, item_slot);
                }
                else
                {
                    SetEquipAppearInfo(&parts_info.Equipment[parts_position], item_slot);
                }
            }
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
            else
            {
                SetEquipAppearInfo(&parts_info.Equipment[parts_position], item_slot);
            }
        }

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
		if ( GetDecorateCostumeItemCode() != 0 && isHideCostume() == FALSE )
		{
			parts_info.Equipment[BP_DECORATE_COSTUME].dwItemID = GetDecorateCostumeItemCode();
		}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

        //파츠 갱신
        RefreshRender(&parts_info, refresh_bone);
        class_code = GetImageClass();
        
        SetScale(GetHeightInfo(static_cast<eCHAR_TYPE>(class_code), GetHeightVariation()));

        // 엘리트 아이템 연출관련
        if (GENERALPARAM->IsNet())
        {
            if( GlobalFunc::IsSSQScene() == FALSE )
            {
                RefreshFullsetItemInfo( this );
            }
        }
    }
}


void Character::SetPolymorph( BOOL Active )
{
    if( m_IsPolymorphApperance == Active )
        return;

    m_IsPolymorphApperance = Active;
}

void Character::CreateCharacterVariation(APPEARANCEINFO* appearance_info, BOOL only_bone)
{
    cCharacterData::Initialize();

    ClassCodeEx class_code;
    class_code.class_code = appearance_info->Job;
    appearance_info->Job = class_code.class_type;
    SetClass(static_cast<eCHAR_TYPE>(class_code.class_type));
    SetGender(static_cast<GenderType::Value>(class_code.gender));

    InitRender(BP_COUNT, BP_BONE, appearance_info, AppearanceManager::kAppearanceAttachIndex_Count, only_bone);
}


void Character::DestroyCharacterVariation()
{
    cCharacterData::Release();
}

SCSlotContainer* Character::GetSlotContainer( SLOTIDX slotIdx )
{
    switch( slotIdx )
    {
    case SI_EQUIPMENT:
        {
            return GetEquipmentContainer();
        }
        break;
    }

    return NULL;
}
//
//bool Character::IsSetitemEquipmentSlot(int equipment_slot_index)
//{
//    switch (equipment_slot_index)
//    {
//    case eEQUIPCONTAINER_ARMOR:
//    case eEQUIPCONTAINER_HELMET:
//    case eEQUIPCONTAINER_PANTS:
//    case eEQUIPCONTAINER_BOOTS:
//    case eEQUIPCONTAINER_GLOVE:
//        {
//            return true;
//        }        
//        break;
//    case eEQUIPCONTAINER_BELT:
//    case eEQUIPCONTAINER_PROTECTOR:
//    case eEQUIPCONTAINER_SHIRTS:
//        {
//            if (IsKindOfObject(PLAYER_OBJECT) != FALSE)
//            {
//                return true;
//            }
//
//            if (IsKindOfObject(NPC_OBJECT) != FALSE)
//            {
//                return false;
//            }
//        }
//        break;
//    }
//
//    return false;
//}
//
////-------------------------------------------------------------------------------------------
///** 세트 아이템을 모두 장착 하였느냐? ( 풀셋을 만족하였나? )
//GS_DESC:현재 동일종류의 아이템이여도 아머파츠와 다른파츠가 디바인이 다르다면 풀셋이 아니라
//고체크하고있는데 맞는지는 모르겠음
//*/
//eKIND_OF_EQUIPED_SETITEM Character::GetEquipedKindOfSetItem(OUT WORD& setitem_optioninfo_setgroupcode) //m_sFullSetEffectInfo를 셋팅해준다
//{
//    SCSlotContainer * pContainer = GetEquipmentContainer();
//    if (pContainer == NULL)
//    {
//        return eKIND_OF_EQUIPED_SETITEM_NONE;
//    }
//    eITEMSUBTYPE eSubType;
//    WORD		 SetCode;
//    BOOL bDivine = FALSE;
//
//    SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(eEQUIPCONTAINER_ARMOR);			
//
//    const BASE_ITEMINFO* pItemInfo = rItemSlot.GetImageItemInfo();
//
//    //슬롯유효검사
//    if (rItemSlot.IsContainValidItem() && NULL != pItemInfo )
//    {
//        eSubType = (eITEMSUBTYPE)pItemInfo->m_wSubType;
//        SetCode  = pItemInfo->set_group_code;
//    }
//    else
//    {
//        eSubType = eITEMSUBTYPE_GENERAL;
//        SetCode  = 0;
//    }
//
//    ClearFullsetEffectInfo();
//
//    eKIND_OF_EQUIPED_SETITEM eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_NONE;	
//
//    for ( int i = 0; i < pContainer->GetMaxSlotNum(); ++i )
//    {	
//        // 이놈들만 체크한다.
//        // 플레이어와 Monster/NPC 는 조건이 틀리다.
//        if (IsSetitemEquipmentSlot(i) == false)
//        {
//            continue;
//        }
//
//        if ( pContainer->IsEmpty(i) )
//        {
//            return eKIND_OF_EQUIPED_SETITEM_NONE;
//        }
//
//        SCItemSlot & rItemSlot = (SCItemSlot &)pContainer->GetSlot(i);	
//
//        if (GetEquipmentContainer()->GetEquipmentManageInfo()->slots[i].activated == false)
//        {
//            // 아이템이 비활성화인 경우 허용안함
//            return eKIND_OF_EQUIPED_SETITEM_NONE;
//        }
//
//        const BASE_ITEMINFO* pItemInfo = rItemSlot.GetImageItemInfo();
//        if ( !pItemInfo )
//        {
//            return eKIND_OF_EQUIPED_SETITEM_NONE;
//        }
//
//        // 이전의 값과 다른 셋인지 체크한다.
//        // eEQUIPCONTAINER_ARMOR와 나머지 장비들을 비교한다.
//        if ((eSubType != pItemInfo->m_wSubType) ||
//            (SetCode != pItemInfo->set_group_code))
//        {
//            return eKIND_OF_EQUIPED_SETITEM_NONE;
//        }
//    }
//
//    //풀셋으로 장착하고 있다면..
//    if( 0 != pItemInfo->GetApplySetItemOptionType())
//    {
//        SETITEM_OPTION_INFO* pSetItemOpt = SetItemOptionInfoParser::Instance()->Find(pItemInfo->set_group_code);
//        if(pSetItemOpt)
//        {
//            for(int i = 0; i < MAX_SETITEM_EFFECT_NUM; ++i)
//            {
//                if (INVALID_WZID != pSetItemOpt->m_dwEffectCode[i])
//                {
//                    sFullSetEffectInfo effect_info;
//                    effect_info.m_dwEffectID = pSetItemOpt->m_dwEffectCode[i][GetGender()];
//                    effect_info.m_btBoneIdx = pSetItemOpt->m_btBoneIdx[i][GetGender()];
//                    // 럭키아이템처럼 처리한다(bysubType)
//                    if (pSetItemOpt->m_byItemType == 5)
//                    {
//                        eSubType = eITEMSUBTYPE_FATE;
//                    }
//                    SetFullsetEffectInfo(i, effect_info);
//                }
//            }
//            setitem_optioninfo_setgroupcode = pSetItemOpt->m_wSetGroupCode;
//        }
//    }
//
//    switch ( eSubType )
//    {
//    case eITEMSUBTYPE_GENERAL:
//        {
//            eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_GENERAL;
//        }
//        break;
//
//    case eITEMSUBTYPE_ELITE_SSQ:
//    case eITEMSUBTYPE_ELITE:
//        {
//            eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_ELITE;
//        }
//        break;
//
//
//    case eITEMSUBTYPE_UNIQUE:
//        {
//            eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_UNIQUE;
//        }
//        break;
//
//    case eITEMSUBTYPE_FATE:
//        {
//            eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_LUCKY;
//        }
//        break;
//
//    default:
//        eKindofEquipedSet = eKIND_OF_EQUIPED_SETITEM_NONE;
//        break;
//    }			
//
//    return eKindofEquipedSet;
//}

////------------------------------------------------------------------------------
///**
//*/
//BOOL Character::GetAttachEffectForSetItemPos( Character* pCharacter, OUT WzVector& Attach_Position)
//{
//    int iBoneIndex = -1;
//
//    // 가슴 본을 구해온다.	
//    switch( pCharacter->GetClass() )
//    {
//    case eCHAR_BERSERKER:       iBoneIndex = 5;
//        break;
//    case eCHAR_DRAGON:          iBoneIndex = 5;
//        break;
//    case eCHAR_VALKYRIE:        iBoneIndex = 6;
//        break;
//    case eCHAR_ELEMENTALIST:    iBoneIndex = 6;	
//        break;
//    case eCHAR_SHADOW:          iBoneIndex = 6;
//        break;
//    case eCHAR_MYSTIC:          iBoneIndex = 5;
//        break;
//    default:
//        break;
//    }
//
//    Attach_Position = pCharacter->GetVisiblePos();
//    Attach_Position.z += pCharacter->GetHeight() / 2.0f;
//
//    return GameFunc::GetBonePos( GetBasePart(), iBoneIndex, &Attach_Position);
//}
//
////------------------------------------------------------------------------------
///**
//*/
//int	Character::GetAttachEffectForEliteItemBone()
//{
//    int iBoneIndex = -1;
//
//    // 가슴 본을 구해온다.	
//    switch (GetClass())
//    {
//    case eCHAR_BERSERKER:
//        iBoneIndex = 5;
//        break;
//    case eCHAR_DRAGON:
//        iBoneIndex = 5;
//        break;
//    case eCHAR_VALKYRIE:
//        iBoneIndex = 6;
//        break;
//    case eCHAR_ELEMENTALIST:
//        iBoneIndex = 6;	
//        break;
//    case eCHAR_SHADOW:
//        iBoneIndex = 6;
//        break;
//    case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
//        iBoneIndex = 5;
//        break;
//
//    default:
//        break;
//    }
//
//    return iBoneIndex;
//}

void Character::HideHelmet(BOOL is_hide, BOOL is_refresh/* = TRUE*/)
{
    if (IsHideHelmet() != is_hide) 
    {
        // 용변신 중에 헬멧보이기 옵션이 변경되었다고 하여 RefreshAppearRender() 을 호출할 필요가 없음
        // 변신중이 파츠를 안그리기 때문에 Refresh할 필요는 없으며 m_bHideHelmet 에 세팅해주는 것으로 변신해제시 옵션이 적용됨

        cCharacterData::SetHideHelmet(is_hide);

        if ((is_refresh == TRUE) && (IsTransForm() == FALSE)) 
        {
            RefreshAppearRender();
        }
    }
}
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
void Character::HideWing(BOOL is_hide, BOOL is_refresh/* = TRUE*/)
{
    if (IsHideWing() != is_hide) 
    {
        cCharacterData::SetHideWing( is_hide );

        if ((is_refresh == TRUE) && (IsTransForm() == FALSE)) 
        {
            RefreshAppearRender();
        }
    }
}
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING

#ifdef _NA_006683_20130502_EQUIPMENTS_VISIBLE_COSTUME
void Character::HideCostume( BOOL is_Hide,BOOL is_refresh /*= TRUE*/ )
{
    if (isHideCostume() != is_Hide) 
    {
        cCharacterData::SetHideCostume( is_Hide );

        if ((is_refresh == TRUE) && (IsTransForm() == FALSE)) 
        {
            RefreshAppearRender();
        }
    }
}
#endif

void Character::CreateShieldEffect()
{
    if( IsCharacterDataCreated() == false )
    {
        return;
    }

    //쉴드가 없어지면 쉴드깨진이펙트 출력해준다
    HANDLE effect_handle = GetDestroyShieldEffect();

    if( m_bShield == FALSE )
    {
        if( effect_handle == INVALID_HANDLE_VALUE )
        {
            effect_handle = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A701"), GetBodyPos(), GetObjectKey() );

            SetDestroyShieldEffect(effect_handle);
        }
    }
}

void Character::UpdateShieldEffect()
{
    if( m_bShield == FALSE )
        return;

    if( IsCharacterDataCreated() == false )
    {
        return;
    }

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    if (GetHideSkill() == HIDE_SKILL_INFO::ALL_HIDE)
    {
        // 하이드 상태면 안띄움
        return;
    }
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL

    //쉴드가있을때나 핸들이 없을땐리턴
    HANDLE effect_handle = GetDestroyShieldEffect();
    if( effect_handle == INVALID_HANDLE_VALUE )
        return;


    if( CEffectBody* effect_body = g_pEffectManager->GetEffectPtrHandle(effect_handle) )
    {
        effect_body->SetResultMatrix(GetBodyPos(),c_ZeroVector,c_UnitVector);
    }
}

void Character::ReleaseShieldEffect()
{
    if( IsCharacterDataCreated() == false )
    {
        return;
    }

    HANDLE effect_handle = GetDestroyShieldEffect();
    if (effect_handle != INVALID_HANDLE_VALUE )
    {
        g_pEffectManager->RemoveEffectPtr(effect_handle);

        SetDestroyShieldEffect(INVALID_HANDLE_VALUE);
    }
}

void Character::CheckEmptyHelmet(int equip_pos, APPEARANCEINFO& parts_info, int hair_variation, int face_variation)
{
    if (equip_pos == EquipmentContainer::EQUIP_HELMET &&
        GlobalFunc::IsSSQScene() == FALSE)
    {
        parts_info.Equipment[BP_HAIR].dwItemID = hair_variation;
        parts_info.Equipment[BP_FACE].dwItemID = face_variation;
    }
}

void Character::SetEquipAppearHelmet(APPEARANCEINFO& parts_info, SCItemSlot& item_slot, int hair_variation, int face_variation, BOOL hide_helmet, GenderType::Value gender )
{
    int hair_type = 0;
    
    bool draw_face = true;	
    bool draw_hair = true;	

    //헬멧 보이기 상태에서만 셋팅
    if (hide_helmet == FALSE)
    {
        SetEquipAppearInfo(&parts_info.Equipment[BP_HELM], item_slot);

        BASE_ITEMINFO* item_info_ptr = ItemInfoParser::Instance()->GetItemInfo(item_slot.GetImageCode());

        if (item_info_ptr)
        {
            int customize_type = 0;
            if (gender == GenderType::kMale)
            {
                hair_type = item_info_ptr->m_bHeadType;
                customize_type = item_info_ptr->m_wCustomize;
            }
            else
            {
                hair_type = item_info_ptr->m_bHeadType1;
                customize_type = item_info_ptr->m_wCustomize1;
            }

            if (customize_type == 1)
            {							
                //1일경우 얼굴.머리카락 랜더안함
                draw_face = false;	
                draw_hair = false;
            }
            else if (customize_type == 2)
            {
                // 해당 값이 2이면 머리카락 랜더안함
                draw_hair = false;
            }     
        }
    }

    //타락한 사원에서는 무조건 그림
    if (g_LoadScene.GetNextSceneAfterLoaded() == SCENE_TYPE_SSQ)
    {
        draw_hair = false;
        draw_face = false;
    }

    if (draw_hair == true)
    {
        parts_info.Equipment[BP_HAIR].dwItemID = hair_variation;
        parts_info.Equipment[BP_HAIR].iEnchant = hair_type;				
    }

    if (draw_face == true)
    {
        parts_info.Equipment[BP_FACE].dwItemID = face_variation;
    }
}

void Character::SetEquipAppearInfo(EQUIPAPPEARINFO* parts_info, SCItemSlot& item_slot)
{
    if (parts_info != NULL)
    {
        parts_info->dwItemID = item_slot.GetImageCode();
        parts_info->bDivine = item_slot.IsDivine();	
        parts_info->iEnchant = item_slot.GetEnchant();
    }
}

DWORD Character::GetWeaponKind()
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return eWEAPONTYPE_PUNCH;
    }

    // 무기에따라서 기본자세가 바뀝니다.
    SCSlot& rSlot = equipment_container->GetSlot(EquipmentContainer::EQUIP_WEAPON1);

    if (rSlot.GetCode() == 0)
    {
        return eWEAPONTYPE_PUNCH;
    }
    else
    {
        BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rSlot.GetCode());
        if(!pItemInfo)
        {
            WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_CENTER, "Invalid Weapon ID [%d]", rSlot.GetCode());
            assert(pItemInfo && "장착된 무기 ID가 잘못되었습니다.");
            return eWEAPONTYPE_PUNCH;
        }

        return pItemInfo->m_wType;
    }	
}

char Character::GetWeaponAnimationCode()
{
    const char kInvalid_Code = '0';
    if(IsTransForm())
    {
        return kInvalid_Code;
    }

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    if (GlobalFunc::IsSpaScene() == TRUE)
    {
        return kInvalid_Code;
    }
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

    DWORD weapon_kind = GetWeaponKind();


    switch(weapon_kind)
    {    
    case eWEAPONTYPE_TWOHANDAXE:
    case eWEAPONTYPE_STAFF:
        {
            return '1';
        }
        break;

    case eWEAPONTYPE_TWOHANDSWORD:
    case eWEAPONTYPE_ONEHANDSWORD:
    case eWEAPONTYPE_ONEHANDCROSSBOW:
    case eWEAPONTYPE_ORB:
    case eWEAPONTYPE_DAGGER:
    case eWEAPONTYPE_GUNTLET: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eWEAPONTYPE_HELLROID_1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eWEAPONTYPE_ARCBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {
            return '2';
        }
        break;

    case eWEAPONTYPE_SPEAR:
    case eWEAPONTYPE_ETHERWEAPON:
    case eWEAPONTYPE_WHIP:
    case eWEAPONTYPE_POLEARM: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {
            return '3';
        }
        break;

    case eWEAPONTYPE_PUNCH:
        {
            return '0';
        }
        break;

#ifdef _YJ_FISHING_SYSTEM_000933
    case eWEAPONTYPE_FISHING_POLE:
        {
            return kInvalid_Code;
        }
        break;
#endif //_YJ_FISHING_SYSTEM_000933

    default:
        {
            assert(!"무기 계열 Text 정보가 잘못되었습니다.");
            return '2';
        }
        break;
    }
}

int Character::GetWeaponBone(int index)
{
    if( IsCharacterDataCreated() == false )
    {
        return 0;
    }

    CWzUnit* unit_ptr = GetBasePart()->GetWzUnit();
    if (unit_ptr == NULL)
    {
        return 0;
    }

    // 클래스 및 무기에 따른 본이름
    WzBoneInfo* bone_Info = NULL;
    switch (GetWeaponKind())
    {
        // 'WEA1' only
    case eWEAPONTYPE_TWOHANDSWORD:
    case eWEAPONTYPE_GUNTLET: // 건틀릿은 오른손만 착용 //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eWEAPONTYPE_HELLROID_1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {
            bone_Info = unit_ptr->GetBoneInfo(StrToWzID("WEA1"));
        }
        break;

        // 'WEA1' & 'WEA2'
    case eWEAPONTYPE_ONEHANDSWORD:
    case eWEAPONTYPE_DAGGER:
    case eWEAPONTYPE_ONEHANDCROSSBOW:
    case eWEAPONTYPE_STAFF:
    case eWEAPONTYPE_ORB:
    case eWEAPONTYPE_ARCBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {
            if (index == 0)
            {
                bone_Info = unit_ptr->GetBoneInfo(StrToWzID("WEA1"));
            }
            else
            {
                bone_Info = unit_ptr->GetBoneInfo(StrToWzID("WEA2"));
            }
        }
        break;

        // 'WEA3' only
    case eWEAPONTYPE_TWOHANDAXE:
    case eWEAPONTYPE_SPEAR:
    case eWEAPONTYPE_WHIP:
    case eWEAPONTYPE_ETHERWEAPON:
    case eWEAPONTYPE_POLEARM: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {
            bone_Info = unit_ptr->GetBoneInfo(StrToWzID("WEA3"));
        }
        break;
    }

    if (bone_Info != NULL)
    {
        return bone_Info->m_iNode;
    }

    return 0;
}
// [3/10/2011 iforall]
//////////////////////////////////////////////////////////////////////////

int Character::GetSpecialAccessoryBone(WzID id)
{
    WzBoneInfo* pInfo = NULL;

    if (StrToWzID("Null") == id)
    {
        return -1;
    }

    if (GetBasePart()->GetWzUnit() != NULL)
    {
        pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(id);
        if (pInfo != NULL)
        {
            return pInfo->m_iNode;
        }
    }

    return -1;
}

#ifdef _DH_NPC_GROUP_NAME
void Character::RenderTextOnHeadGroup(TCHAR *szString, WzColor color, int yoffset)
{
    TCHAR szGuildString[INTERFACE_STRING_LENGTH] = { 0,};
    Snprintf (szGuildString,INTERFACE_STRING_LENGTH-1, _T("< %s >"), szString);

    if (GetChatDisplayTime())//채팅 중 랜더 안함
    {
        return;
    }

    if (g_InterfaceManager.GetShowFlag()) 
    {

        WzVector wzVisualPos = GetVisiblePos();
        WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
        g_pSunRenderer->Set2DMode( TRUE );
        g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m217") );

        WzVector wzPos = GetPosition();				
        wzVisualPos.z += GetMeshHeight();
        wzVisualPos.z += HEIGHT_GAP;

        POINT pt2;
        float fDepth;
        g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos , &fDepth );

        SIZE textsizeGuild, textSizeName;
        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szGuildString,lstrlen(szGuildString),&textsizeGuild);

        RECT rc;
        rc.left = pt2.x - (textsizeGuild.cx / 2);
        rc.top = pt2.y + yoffset - 2 ;
        rc.right = rc.left + textsizeGuild.cx;
        rc.bottom = rc.top + 50;
        WzColor BackColor = WzColor_RGBA(0,0,0,100);	

        const TCHAR* player_name = GetName();
        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint((TCHAR*)player_name, lstrlen(player_name), &textSizeName);// 글로벌 엔진버전이 낮아서 error발생 (TCHAR*)필요 추후 제거 요망
        SIZE BigTextSize = textsizeGuild.cx > textSizeName.cx ? textsizeGuild : textSizeName;

        //if (this->IsExistGuild())
        {
            RenderBlockedTextWithDepthTest(szGuildString,&rc,color,BackColor,fDepth);//배경 Rect그리기
            RenderBlockedTextWithDepthTest(szGuildString,&rc,color,BackColor,fDepth, true);
        }

        g_pSunRenderer->x_pManagerTextOut->EndUseFont();

        g_pSunRenderer->Set2DMode(FALSE);
        g_pSunRenderer->SetColor(oldcolor);
    }
}
#endif//_DH_NPC_GROUP_NAME
//------------------------------------------------------------------------------ 
WzID Character::GetNormalAttackAniCode(BASE_STYLEINFO* info, eATTACK_SEQUENCE attack_sequence, int ani_type)
{
    if ((GetClass() == eCHAR_BERSERKER) || 
        (GetClass() == eCHAR_DRAGON) || 
        (GetClass() == eCHAR_MYSTIC)) // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    {
        if (ani_type == 0)
        {
            //! 2타 애니메이션
            if (info->m_SkillCode == eSTYLE_TWOHANDSWORD_NORMAL)
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("A201");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("A203");
                }
            }
            else if (info->m_SkillCode == eSTYLE_TWOHANDAXE_NORMAL)
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("A101");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("A102");
                }
            }
            else if (info->m_SkillCode == eSTYLE_ONEHANDSWORD_NORMAL)
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("A201");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("A203");
                }
            }
            else if (info->m_SkillCode == eSTYLE_SPEAR_NORMAL)
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("A301");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("A303");
                }
            }
            else if (info->m_SkillCode == eSTYLE_MYSTIC_GUNTLET_NORMAL) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("A201");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("A203");
                }
            }
            else if (info->m_SkillCode == eSTYLE_MYSTIC_POLEARM_NORMAL) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("A301");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("A303");
                }
            }
        }
        else if (ani_type == 2)
        {
            //! 3타애니
            if (info->m_SkillCode == eSTYLE_TWOHANDSWORD_NORMAL)
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("S213");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("S215");
                case eATTACK_SEQUENCE_THIRD:    return StrToWzID("S217");
                }
            }
            else if (info->m_SkillCode == eSTYLE_TWOHANDAXE_NORMAL)
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("S113");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("S115");
                case eATTACK_SEQUENCE_THIRD:    return StrToWzID("S117");
                }
            }
            else if (info->m_SkillCode == eSTYLE_ONEHANDSWORD_NORMAL)
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("S213");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("S215");
                case eATTACK_SEQUENCE_THIRD:    return StrToWzID("S217");
                }
            }
            else if (info->m_SkillCode == eSTYLE_SPEAR_NORMAL)
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("S301");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("S303");
                case eATTACK_SEQUENCE_THIRD:    return StrToWzID("S305");
                }
            }
            else if (info->m_SkillCode == eSTYLE_MYSTIC_GUNTLET_NORMAL) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("S213");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("S215");
                case eATTACK_SEQUENCE_THIRD:    return StrToWzID("S217");
                }
            }
            else if (info->m_SkillCode == eSTYLE_MYSTIC_POLEARM_NORMAL) //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
            {
                switch(attack_sequence)
                {
                case eATTACK_SEQUENCE_FIRST:    return StrToWzID("S313");
                case eATTACK_SEQUENCE_SECOND:   return StrToWzID("S315");
                case eATTACK_SEQUENCE_THIRD:    return StrToWzID("S317");
                }
            }
        }
    }

    switch(attack_sequence)
    {
    case eATTACK_SEQUENCE_FIRST:    return info->m_wzidCodeFirst.id;
    case eATTACK_SEQUENCE_SECOND:   return info->m_wzidCodeSecond.id;
    case eATTACK_SEQUENCE_THIRD:    return info->m_wzidCodeThird.id;
    }

    return StrToWzID("A001");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 뒤로 밀리는 애니메이션과 함께 넉백
void Character::SetKnockBack( const WzVector& CurrentPosition, const WzVector& TargetPosition )
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (CheckDominationCollectState() == TRUE)
        return;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();

	// 액션 큐 무시, 현재 상태를 스킵하고 바로 변경
	SetNextState( STATE::KNOCKBACK, g_CurTime );
	CommitChangeState( g_CurTime );

	// 목표위치로 이동
	StopMove();
	Move( CurrentPosition, TargetPosition , CMS_WALK, TRUE, -1, -1 );
}

void Character::SetKnockBack2( const WzVector& CurrentPosition, const WzVector& TargetPosition )
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (CheckDominationCollectState() == TRUE)
        return;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();

	// 액션 큐 무시, 현재 상태를 스킵하고 바로 변경
	SetNextState( STATE::KNOCKBACK2, g_CurTime );
	CommitChangeState( g_CurTime );

	// 목표위치로 이동
	StopMove();
	Move( CurrentPosition, TargetPosition , CMS_WALK, TRUE, -1, -1 );
}

// 점프하면서 대상 위치로 튕귀며 이동된다 (이그니션의 풀링)
void Character::SetKnockBack3( const WzVector& CurrentPosition, const WzVector& TargetPosition )
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (CheckDominationCollectState() == TRUE)
        return;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	if (IsAbnormalityProtection() == TRUE)
	{
		return;
	}
#endif // _NA_008472_20150903_IMPROVED_BALANCE

	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();

	// 뛰우기 위한 값을 설정
	m_AirData.m_fDownForceLimit = -50.0f;
	m_AirData.m_fUpperForce = 25.0f;
	m_AirData.m_fGravity = 140.0f;
	m_AirData.m_fHighLimit = 5.0f;
	m_fHeight = 0.0f;

	// 액션 큐 무시하고 바로 변경
	SetNextState( STATE::KNOCKBACK3, g_CurTime );
	CommitChangeState( g_CurTime );

	// 목표위치로 이동
	StopMove();
	Move( CurrentPosition, TargetPosition , CMS_WALK, TRUE, -1, -1 );
}

void Character::SetDown( DWORD Duration )
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (CheckDominationCollectState() == TRUE)
        return;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();


	// 액션 큐 무시, 현재 상태를 스킵하고 바로 변경
	SetNextState( STATE::DOWN, g_CurTime );
	CommitChangeState( g_CurTime );


#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	// 일어나는 동작이 없어졌다.
#else
	// 일어나는 애니메이션의 시간을 미리 계산한 한다
	DWORD GetUpAnimationTime = 1000;

	WzID DownAnimationID;
	switch( GetWeaponAnimationCode() )
	{
	case '0':	DownAnimationID = StrToWzID("F003"); break;
	case '1':	DownAnimationID = StrToWzID("F103"); break;
	case '2':	DownAnimationID = StrToWzID("F203"); break;
	case '3':	DownAnimationID = StrToWzID("F303"); break;
	}

	if( WzAnimationInfo* pAnimationInfo = GetAnimationInfo(DownAnimationID) )
	{
		GetUpAnimationTime = pAnimationInfo->m_nMaxFrame * DEF_NUM_FRAMES_PER_MS;
	}
#endif // _NA_008472_20150903_IMPROVED_BALANCE

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	// 일어나는 시간이 없어졌다.
	m_DownTime = g_CurTime + Duration;
#else
	// 일어나는 시간만큼을 미리 뺴둔다. (넘어지는 시간, + 일어나는 시간을 합한것보다 다운 시간이 적다면 문제가 생기므로 최소 2초의 시간을 부여한다)
	m_DownTime = g_CurTime + max(2000, Duration - GetUpAnimationTime );
#endif // _NA_008472_20150903_IMPROVED_BALANCE

	// 목표위치로 이동
	StopMove();
	//Move( ResultData.GetCurrentPosition().wvPos, ResultData.GetDestPosition().wvPos, CMS_WALK, TRUE, -1, -1 );

}

// 다운과 넉백이 동시에 발생
void Character::SetKnockBackDown( const WzVector& CurrentPosition, const WzVector& TargetPosition, DWORD DownDuration )
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (CheckDominationCollectState() == TRUE)
        return;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	// 일단 다운으로 적용

	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();

	// 액션 큐 무시, 현재 상태를 스킵하고 바로 변경
	SetNextState( STATE::DOWN, g_CurTime );
	CommitChangeState( g_CurTime );


	// 일어나는 애니메이션의 시간을 미리 계산한 한다
	DWORD GetUpAnimationTime = 1000;

	WzID DownAnimationID;
	switch( GetWeaponAnimationCode() )
	{
	case '0':	DownAnimationID = StrToWzID("F003"); break;
	case '1':	DownAnimationID = StrToWzID("F103"); break;
	case '2':	DownAnimationID = StrToWzID("F203"); break;
	case '3':	DownAnimationID = StrToWzID("F303"); break;
	}

	if( WzAnimationInfo* pAnimationInfo = GetAnimationInfo(DownAnimationID) )
	{
		GetUpAnimationTime = pAnimationInfo->m_nMaxFrame * DEF_NUM_FRAMES_PER_MS;
	}

	// 일어나는 시간만큼을 미리 뺴둔다. (넘어지는 시간, + 일어나는 시간을 합한것보다 다운 시간이 적다면 문제가 생기므로 최소 2초의 시간을 부여한다)
	m_DownTime = g_CurTime + max(2000, DownDuration - GetUpAnimationTime );


	// 목표위치로 이동
	StopMove();
	Move( CurrentPosition, TargetPosition , CMS_WALK, TRUE, -1, -1 );
}

void Character::SetStun( const WzVector& CurrentPosition )
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (CheckDominationCollectState() == TRUE)
        return;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();

	// 액션 큐 무시, 현재 상태를 스킵하고 바로 변경
	SetNextState( STATE::IDLE, g_CurTime );
	CommitChangeState( g_CurTime );

	StopMove();
	SetPosition( CurrentPosition );
}

void Character::SetIdleState()
{
	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();

	// 액션 큐 무시, 현재 상태를 스킵하고 바로 변경
	SetNextState( STATE::IDLE, g_CurTime );
	CommitChangeState( g_CurTime );
}

void Character::SetAir( DWORD Duration )
{
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    if (CheckDominationCollectState() == TRUE)
        return;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	if (IsAbnormalityProtection() == TRUE)
	{
		return;
	}
#endif // _NA_008472_20150903_IMPROVED_BALANCE

	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();

	// 액션 큐 무시, 현재 상태를 스킵하고 바로 변경
	SetNextState( STATE::AIR, g_CurTime );
	CommitChangeState( g_CurTime );


	// 일어나는 애니메이션의 시간을 미리 계산한 한다
	static DWORD GetUpAnimationTime = 1000;

	WzID DownAnimationID;
	switch( GetWeaponAnimationCode() )
	{
	case '0':	DownAnimationID = StrToWzID("F003"); break;
	case '1':	DownAnimationID = StrToWzID("F103"); break;
	case '2':	DownAnimationID = StrToWzID("F203"); break;
	case '3':	DownAnimationID = StrToWzID("F303"); break;
	}

    static DWORD double_attack_duration = 2000;
    static DWORD double_attack_duration2 = 5000;
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
    static DWORD double_attack_duration3 = 2500;
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

	if( Duration == double_attack_duration )
	{
        m_AirData.m_MoveType = sAirData::MOVE_TYPE_NORMAL;
		m_AirData.m_fDownForceLimit = -40.0f;
		m_AirData.m_fGravity = 200.0f;
		m_AirData.m_fHighLimit = 3.4f;
		m_AirData.m_fUpperForce = 100.0f;
        m_AirData.m_NoDown = TRUE;
	}
	else if( Duration == double_attack_duration2 )	
	{
        static int s_mode = 1;


        m_AirData.m_MoveType = sAirData::MOVE_TYPE_AIRBLOW;

        static float fDownLimit = -20.0f;
        static float fGravity = 37.0f;
        static float fHeightLimit = 4.5f;
        static float fUpperForce = 22.0f;
        static DWORD DownTime = 1000;
        static float fMaxHeight = 1.3f;

        m_AirData.m_fDownForceLimit = fDownLimit;
        m_AirData.m_fGravity = fGravity;
        m_AirData.m_fHighLimit = fHeightLimit;
        m_AirData.m_fUpperForce = fUpperForce;
        m_AirData.m_NoDown = FALSE;
        m_AirData.m_DownTime = DownTime;
        m_AirData.m_MaxHeight = fMaxHeight;
        m_AirData.m_StartTime = GetCurrentTime();

        m_AirData.m_NoDown = FALSE;
	}
#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
    else if( Duration == double_attack_duration3 )
    {
        // 현재 사용하지는 않지만 혹시 몰라 추가.        
        m_AirData.m_MoveType = sAirData::MOVE_TYPE_AIRBLOW;

        static float fDownLimit1 = -20.0f;
        static float fGravity1 = 50.0f;
        static float fHeightLimit1 = 4.5f;
        static float fUpperForce1 = 22.0f;
        static DWORD DownTime1 = 500;
        static float fMaxHeight1 = 1.3f;

        m_AirData.m_fDownForceLimit = fDownLimit1;
        m_AirData.m_fGravity = fGravity1;
        m_AirData.m_fHighLimit = fHeightLimit1;
        m_AirData.m_fUpperForce = fUpperForce1;
        m_AirData.m_NoDown = FALSE;
        m_AirData.m_DownTime = DownTime1;
        m_AirData.m_MaxHeight = fMaxHeight1;
        m_AirData.m_StartTime = GetCurrentTime();

        m_AirData.m_NoDown = FALSE;
    }
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

	// 일어나는 시간만큼을 미리 뺴둔다. (넘어지는 시간, + 일어나는 시간을 합한것보다 다운 시간이 적다면 문제가 생기므로 최소 2초의 시간을 부여한다)
	// 체공시간은 일단 패스
	m_AirData.m_AirTime = g_CurTime + max(2000, Duration - GetUpAnimationTime );

	StopMove();
}

#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
void Character::SetAirBlowAir( DWORD Duration )
{    
    if (CheckDominationCollectState() == TRUE)
        return;

	// 모든 데미지를 처리한다
	ProcessAttackResult( GetCurrentAttackSerial(), TRUE );

	EmptyActionQueue();

	// 액션 큐 무시, 현재 상태를 스킵하고 바로 변경
	SetNextState( STATE::AIR, g_CurTime );
	//CommitChangeState( g_CurTime ); // 바로 액션을 하지 않는다.

	// 일어나는 애니메이션의 시간을 미리 계산한 한다
	static DWORD GetUpAnimationTime = 1000;

	WzID DownAnimationID;
	switch( GetWeaponAnimationCode() )
	{
	case '0':	DownAnimationID = StrToWzID("F003"); break;
	case '1':	DownAnimationID = StrToWzID("F103"); break;
	case '2':	DownAnimationID = StrToWzID("F203"); break;
	case '3':	DownAnimationID = StrToWzID("F303"); break;
	}

    m_AirData.m_MoveType = sAirData::MOVE_TYPE_AIRBLOW;

    const float fDownLimitAir = -20.0f;
    const float fGravityAir = 50.0f;
    const float fHeightLimitAir = 4.5f;
    const float fUpperForceAir = 22.0f;
    const DWORD DownTimeAir = 500;
    const float fMaxHeightAir = 1.3f;

    m_AirData.m_fDownForceLimit = fDownLimitAir;
    m_AirData.m_fGravity = fGravityAir;
    m_AirData.m_fHighLimit = fHeightLimitAir;
    m_AirData.m_fUpperForce = fUpperForceAir;
    m_AirData.m_NoDown = FALSE;
    m_AirData.m_DownTime = DownTimeAir;
    m_AirData.m_MaxHeight = fMaxHeightAir;
    m_AirData.m_StartTime = GetCurrentTime();

    m_AirData.m_NoDown = FALSE;

	// 일어나는 시간만큼을 미리 뺴둔다. (넘어지는 시간, + 일어나는 시간을 합한것보다 다운 시간이 적다면 문제가 생기므로 최소 2초의 시간을 부여한다)
	// 체공시간은 일단 패스
	m_AirData.m_AirTime = g_CurTime + max(2000, Duration - GetUpAnimationTime );

	StopMove();
}
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN

void Character::AddAirForce( float fAddUpperForce, float fHeightLimit )
{
	//m_AirData.m_fUpperForce += fAddUpperForce;
	//m_AirData.m_fHighLimit += fHeightLimit;

    //if( m_AirData.m_MoveType != 0 )
    //    return;

    static int s_mode = 2;

    static float fUpperForce = 10.0f;
    static float fStaticUpperForce = 10.0f;
    static float fStaticUpperForce2 = 5.0f;
    //static float fHeightLimit = 0.5f;

    if( s_mode == 0 )
    {
        m_AirData.m_fUpperForce += fUpperForce;


        if( m_AirData.m_fUpperForce >= fStaticUpperForce )
            m_AirData.m_fUpperForce = fStaticUpperForce;
    }
    else if( s_mode == 1 )
    {
        m_AirData.m_fUpperForce = fStaticUpperForce2;
    }
    else if( s_mode == 2 )
    {
        static float sf = 0.5f;
        m_AirData.m_fUpperForce += sf;

        static float sh = 0.1f;
        m_fHeight += sh;
    }

//    m_AirData.m_fHighLimit += fHeightLimit;

}

void Character::UpdateAir( DWORD TickTime )
{
    if( m_AirData.m_MoveType == sAirData::MOVE_TYPE_NORMAL )
    {
        static float s_speed_multiplier = 0.001f;
        static float s_height_multiplier = 0.001f;

	    m_AirData.m_fUpperForce -= (m_AirData.m_fGravity * TickTime * s_speed_multiplier);
	    if( m_AirData.m_fUpperForce < m_AirData.m_fDownForceLimit )
	    {
		    m_AirData.m_fUpperForce = m_AirData.m_fDownForceLimit;
	    }

	    m_fHeight += (m_AirData.m_fUpperForce * TickTime * s_height_multiplier);		

	    if( m_fHeight >= m_AirData.m_fHighLimit )
	    {
		    m_fHeight = m_AirData.m_fHighLimit;
	    }

	    if( m_fHeight < 0.0f )
		    m_fHeight = 0.0f;
    }
    else if( m_AirData.m_MoveType == sAirData::MOVE_TYPE_AIRBLOW )
    {
        static float s_speed_multiplier = 0.0007f;
        static float s_height_multiplier = 0.0008f;
        static float s_height_multiplier2 = 0.00025f;
        static float s_height_multiplier3 = 0.001f;


        DWORD CurrentTime = GetCurrentTime();
        if( m_AirData.m_StartTime + m_AirData.m_DownTime > CurrentTime )
        {
            // 이 구간은 떨어지지는 않는다
            m_AirData.m_fUpperForce -= (m_AirData.m_fGravity * TickTime * s_speed_multiplier);

            if( m_AirData.m_fUpperForce < 0.0f )
                m_AirData.m_fUpperForce = 0.0f;

            if( m_AirData.m_fUpperForce < m_AirData.m_fDownForceLimit )
            {
                m_AirData.m_fUpperForce = m_AirData.m_fDownForceLimit;
            }

            if( m_fHeight >= m_AirData.m_MaxHeight )
            {
                m_fHeight += (m_AirData.m_fUpperForce * TickTime * s_height_multiplier2);		
            }
            else
            {
                m_fHeight += (m_AirData.m_fUpperForce * TickTime * s_height_multiplier);		
            }


            if( m_fHeight >= m_AirData.m_fHighLimit )
            {
                m_fHeight = m_AirData.m_fHighLimit;
            }
        }
        else
        {
            m_AirData.m_fUpperForce -= (m_AirData.m_fGravity * TickTime * s_height_multiplier3);

            if( m_AirData.m_fUpperForce < m_AirData.m_fDownForceLimit )
            {
                m_AirData.m_fUpperForce = m_AirData.m_fDownForceLimit;
            }


            m_fHeight += (m_AirData.m_fUpperForce * TickTime * s_height_multiplier3);		

            if( m_fHeight >= m_AirData.m_fHighLimit )
            {
                m_fHeight = m_AirData.m_fHighLimit;
            }


            if( m_fHeight < 0.0f )
                m_fHeight = 0.0f;
        }
    }
}


void Character::SetFallDown( float fGravity, float fUpperForce )
{
	m_FallDownData.m_fGravity = fGravity;
	m_FallDownData.m_fUpperForce = fUpperForce;
	m_fHeight = 0.0f;
}

void Character::UpdateFallDown( DWORD TickTime )
{
	m_FallDownData.m_fUpperForce += m_FallDownData.m_fGravity * TickTime * SPEED_MULTIPLIER;

	float fDelta = m_FallDownData.m_fUpperForce * (float)TickTime * SPEED_MULTIPLIER;
	m_fHeight += fDelta;

	if( m_fHeight < -100.0f )
	{
		m_fHeight = -100.0f;
	}
}

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
BOOL Character::CheckDominationCollectState()
{
    if(GlobalFunc::IsDominationScene() == TRUE 
        && this->GetCurState() == STATE::COLLECT)
        return TRUE;

    return FALSE;
}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
void Character::CreateViceSwordWeaponEffect()
{
    if (is_vice_sword_effect_() == TRUE)
        return;

    DestroyViceSwordWeaponEffect();

    // 왼손, 오른손
    for (unsigned int i = 0; i < 2; ++i)
    {
        CWzUnitDraw* weapon_unit_draw_ptr = GetAttachment(i).GetUnitDraw();
        if (!weapon_unit_draw_ptr)
        {
            DestroyViceSwordWeaponEffect();
            continue;
        }

        CWzUnit* wz_unit_ptr = weapon_unit_draw_ptr->GetWzUnit();
        if( !wz_unit_ptr )
            return;

        WzBoneInfo* bone_info_ptr = NULL;
        if (i == 0) // 왼손
        {
            bone_info_ptr = wz_unit_ptr->GetBoneInfo(StrToWzID("WEA1"));
        }

        if (i == 1) // 오른손
        {
            bone_info_ptr = wz_unit_ptr->GetBoneInfo(StrToWzID("WEA2"));
        }

        if (!bone_info_ptr)
            continue;

        if (i == 0) // 왼손
        {
            vice_sword_effect_handle_[0] = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("KE46"), 
                weapon_unit_draw_ptr->GetBonePos(bone_info_ptr->m_iNode));

        }

        if (i == 1) // 오른손
        {
            vice_sword_effect_handle_[1] = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("KE45"), 
                weapon_unit_draw_ptr->GetBonePos(bone_info_ptr->m_iNode));
        }
    }

    UpdateViceSwordWeaponEffect();
}

void Character::DestroyViceSwordWeaponEffect()
{
    for(int i=0 ;i < 2; ++i)
    {
        if( vice_sword_effect_handle_[i] != INVALID_HANDLE_VALUE )
        {
            g_pEffectManager->RemoveEffectPtr(vice_sword_effect_handle_[i]);
            vice_sword_effect_handle_[i] = INVALID_HANDLE_VALUE;
        }
    }
}

void Character::UpdateViceSwordWeaponEffect()
{
    if (is_vice_sword_effect_() == FALSE)
    {
        return;
    }

    // 왼손, 오른손
    for (unsigned int i = 0; i < 2; ++i)
    {
        if (GetAttachmentCount() <= i)
        {
            return;
        }

        CWzUnitDraw* weapon_unit_draw_ptr = GetAttachment(i).GetUnitDraw();
        if (!weapon_unit_draw_ptr)
            continue;

        CWzUnit* wz_unit_ptr = weapon_unit_draw_ptr->GetWzUnit();
        if( !wz_unit_ptr )
        {
            DestroyViceSwordWeaponEffect();
            return;
        }

        WzBoneInfo* bone_info_ptr = NULL;
        if (i == 0) // 왼손
        {
            bone_info_ptr = wz_unit_ptr->GetBoneInfo(StrToWzID("WEA1"));
        }

        if (i == 1) // 오른손
        {
            bone_info_ptr = wz_unit_ptr->GetBoneInfo(StrToWzID("WEA2"));
        }

        if (!bone_info_ptr)
            continue;
        
        
        WzMatrix* pwzmTransform = weapon_unit_draw_ptr->GetTransformBuffer();
        if (!pwzmTransform)
        {
            return;
        }

        WzMatrix wmOffsetRot, wmRot, wmPosMat, wmScale;

        MatrixIdentity(&wmOffsetRot);

        //Rotation
        WzVector vRot;
        SetVector(&vRot, n_deg2rad(0.0f), n_deg2rad(0.0f), n_deg2rad(0.0f));
        MatrixRotationZYX(&wmOffsetRot, &vRot);
        MatrixMultiply(&wmRot, &wmOffsetRot, &pwzmTransform[bone_info_ptr->m_iNode]);

        //Position
        WzVector vOffsetPos;
        WzVector offset_pos;
        if (i == 0)
        {
            SetVector(&offset_pos, 0.0f, 0.0f, 0.0f);
        }
        if (i == 1)
        {
            SetVector(&offset_pos, -0.3f, 0.0f, 0.0f);
        }
        
        VectorTransformCoord(&vOffsetPos, &offset_pos, &wmRot);
        MatrixTranslation(&wmPosMat, vOffsetPos.x, vOffsetPos.y, vOffsetPos.z);
        //두번적용못하게
        wmRot._41=0;
        wmRot._42=0;
        wmRot._43=0;
        
        MatrixScaling(&wmScale, 1.0f ,1.0f, 1.0f);            
        
        if (vice_sword_effect_handle_[i] == INVALID_HANDLE_VALUE)
        {
            continue;
        }

        CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(vice_sword_effect_handle_[i]);
        if (!pBody)
        {
            continue;
        }

        pBody->SetResultMatrix(&wmPosMat, &wmRot, &wmScale);        
    }
}

BOOL Character::is_vice_sword_effect_()
{
    if (vice_sword_effect_handle_[0] != INVALID_HANDLE_VALUE && 
        vice_sword_effect_handle_[1] != INVALID_HANDLE_VALUE)
    {
        return TRUE;
    }

    return FALSE;
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
void Character::RefreshSpaRender(BOOL refresh_bone /*= FALSE*/)
{
    APPEARANCEINFO parts_info;
    ZeroMemory(&parts_info, sizeof(parts_info));

    DWORD class_code = GetClass();
    parts_info.ObjectType = APPEARANCEINFO::PLAYER;
    parts_info.Job = class_code;

    SLOTCODE spa_item_code = GetSpaItemCode();
    if (spa_item_code == 0)
    {
#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
        SpaZoneInfo* spa_info = SpaZoneInfoParser::Instance()->GetSpaZoneInfo(g_pMap->GetMapID());
        if (spa_info)
        {
            spa_item_code = static_cast<SLOTCODE>(spa_info->base_costume_code);
        }
#else
        spa_item_code = 7254; // 7254 기본 입장권
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    }

    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(spa_item_code);
    if (item_info && item_info->m_wType == eITEMTYPE_SPA_TICKET)
    {
        DWORD spa_costume_item = 0;
        switch (GetImageClass())
        {
        case eCHAR_BERSERKER:   spa_costume_item = item_info->m_wEqClass1; break;
        case eCHAR_DRAGON:      spa_costume_item = item_info->m_wEqClass2; break;
        case eCHAR_SHADOW:      spa_costume_item = item_info->m_wEqClass3; break;
        case eCHAR_VALKYRIE:    spa_costume_item = item_info->m_wEqClass4; break;
        case eCHAR_ELEMENTALIST:spa_costume_item = item_info->m_wEqClass5; break;
        case eCHAR_MYSTIC:      spa_costume_item = item_info->m_wEqClass6; break; // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        case eCHAR_HELLROID:    spa_costume_item = item_info->m_wEqClass7; break; // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eCHAR_WITCHBLADE:  spa_costume_item = item_info->m_wEqClass8; break; // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        }

        const BASE_ITEMINFO* costume_item_info = ItemInfoParser::Instance()->GetItemInfo(spa_costume_item);
        if (costume_item_info)
        {
            // 머리
#ifdef _NA_000000_20150616_SPA_COSTUME_SHOW_HAIR
            if (GetGender() == GenderType::kMale)
            {
                if (costume_item_info->m_wCustomize == 1 ||
                    costume_item_info->m_wCustomize == 2)
                {
                    parts_info.Equipment[BP_HAIR].dwItemID = 0;
                    parts_info.Equipment[BP_HAIR].iEnchant = 0;
                }
                else
                {
                    parts_info.Equipment[BP_HAIR].dwItemID = GetPartVariation(PLAYER_VARIATION_HAIR);
                    parts_info.Equipment[BP_HAIR].iEnchant = costume_item_info->m_bHeadType;
                }
            }
            else
            {
                if (costume_item_info->m_wCustomize1 == 1 ||
                    costume_item_info->m_wCustomize1 == 2)
                {
                    parts_info.Equipment[BP_HAIR].dwItemID = 0;
                    parts_info.Equipment[BP_HAIR].iEnchant = 0;
                }
                else
                {
                    parts_info.Equipment[BP_HAIR].dwItemID = GetPartVariation(PLAYER_VARIATION_HAIR);
                    parts_info.Equipment[BP_HAIR].iEnchant = costume_item_info->m_bHeadType1;
                }
            }
#else
            parts_info.Equipment[BP_HAIR].dwItemID = 0;
            parts_info.Equipment[BP_HAIR].iEnchant = 0;
#endif //_NA_000000_20150616_SPA_COSTUME_SHOW_HAIR

            parts_info.Equipment[BP_FACE].dwItemID = GetPartVariation(PLAYER_VARIATION_FACE);

            // 방어구
            parts_info.Equipment[BP_ARMOR].dwItemID = costume_item_info->m_Code;
        }
    }

    //파츠 갱신
    RefreshRender(&parts_info, refresh_bone);

    ReleaseFullsetEffect();

    class_code = GetImageClass();

    SetScale(GetHeightInfo(static_cast<eCHAR_TYPE>(class_code), GetHeightVariation()));
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
BOOL Character::IsAbnormalityProtection()
{
	// 디펜스모션 일 때 이상상태 보호.
	if (this->FindSkillEffect(eCHAR_STATE_PROTECTION))
	{
		return TRUE;
	}

	// 에너지석션 일 때 이상상태 보호.
	if (this->FindSkillEffect(eCHAR_STATE_HP_SUCTION))
	{
		return TRUE;
	}

	return FALSE;
}
#endif // _NA_008472_20150903_IMPROVED_BALANCE