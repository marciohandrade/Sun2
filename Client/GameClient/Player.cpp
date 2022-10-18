//------------------------------------------------------------------------------
//  Player.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "varpacket.h"
#include "ItemInfoParser.h"
#include "Player.h"
#include "Map.h"
#include "EquipmentContainer.h"
#include "ItemManager.h"
#include "AppearanceManager.h"
#include "ObjectManager.h"
#include "HeroActionInput.h"
#include "soundeffect.h"
#include "CommonCharacterFormula.h"
#include <SCItemSlot.h>
#include "InterfaceManager.h"
#include "StyleInfoParser.h"
#include <PlayerAttributes.h>
#include "ItemAttrCalculator.h"
#include "SkillAttrCalculator.h"
#include "SkillInfoParser.h"
#include "ChatMsg.h"
#include "ChatDialog.h"
#include "Hero.h"
#include "HeroEquipmentContainer.h"
#include "CharSoundInfoParser.h"
#include "WeaponSoundInfoParser.h"
#include "AppearanceResourceCode.h"
#include "ProjectileInfoParser.h"
#include "HeroHeightInfoParser.h"
#include "SetItemOptionInfoParser.h"
#include "globalfunc.h"
#include "GameFramework.h"
#include "BattleScene.h"
#include <Timer.h>
#include "ExpValueInfoParser.h"
#include "CharacterScene.h"
#include "uiPartyMan/uiPartyMan.h"
#include "RewardInfoList.h"
#include "SunEventInfoParser.h"
#include "GuildMarkInfoParser.h"
#include "uiGuildMan/uiGuildMan.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "EtherWeaponEffectInfo.h"
#include "Pet.h"
#include "PetInfoParser.h"
#include "Monster.h"
#include "GuildRightInfoParser.h"
#include "Clone.h"
#include "CloneManager.h"
#include "Rider.h"
#include "RiderContainer.h"
#include "DelayLoadManager.h"
#include "Camera.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "TotalGuideParser.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "TitleListParser.h"
#include "HonorSystem/HonorSystem.h"
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GuildSystem/GuildSystemFunction.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
  #include "GuildSystem/GuildSystemData.h"
  #include "DominationInfoParser.h"
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

float fTest = 0.f;

#include "GameConst_Color.h"
#include "LoadScene.h"
#include "ResourceManager.h"
#include "RidingInfoParser.h"
#include "State/StateKeyboardJump.h"
#include "NumericValues.h"
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
#include "DiceCastManager.h"
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
#include "HeroAttributes.h"
#include "ProjectileManagerEx.h"
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
#include "CostumeItemInfoParser.h"
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "CollectionParser.h"
#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#include "SpaZoneInfoParser.h"
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

using namespace util;

extern DWORD g_CurTime;




//------------------------------------------------------------------------------
/**
*/
Player::Player( void ):
    m_EtherIndex(0),
    receive_equip_info_(false),
    m_pRider(NULL),
    is_riding_jump_(false),
    m_bRiding(FALSE),
    m_pRiderContainer(NULL),
    m_bRiderJumpFlag(FALSE)
{
	m_eAttackSequence	= eATTACK_SEQUENCE_THIRD;

    SetObjectType(PLAYER_OBJECT);	
	m_dwValkyrieShotSerial = 0;

	m_iCurTile=m_iOldTile=-1;    
	m_dwCurTileAtt=m_dwOldTileAtt=0;

	m_bValkyrieReload = FALSE;

	m_byGMGrade = 0;			//	GM 등급
	m_bInvisible = false;		//	투명
	//m_bHero=FALSE;
	m_iValkyrieShotAnimVariation = 0;

	m_bHideHelmet = FALSE;
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    m_bHideWing = FALSE;
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	m_bInvincible = FALSE; // 무적
	m_bCasting	  = FALSE;	
	m_iInvicibleTime =0;


	m_bAttackBeginPlayer=FALSE;
	m_dwKeyboardMoveTime=0;

	m_wAttSpeedRatio = 100;
	m_wMoveSpeedRatio = 100;

	m_bHPInfo = FALSE;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    m_bSDInfo = FALSE;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
	m_bExistGuild = 0;

	ZeroMemory(&m_GuildRenderInfo, sizeof(m_GuildRenderInfo));

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	is_client_player_ranking_grade_ = FALSE;

	ZeroMemory(&client_player_ranking_grade_info_, sizeof(client_player_ranking_grade_info_));
	client_player_ranking_grade_info_.ranking_type = eSUN_RANKING_TYPE_END;
	client_player_ranking_grade_info_.grade = eSUN_RANKING_GRADE_NONE;
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

	m_JumpData.m_JumpFlag.Reset();


	memset(m_szCharState , 0 , sizeof(m_szCharState) );

	memset(m_tszDelDate , 0, sizeof(m_tszDelDate) );
	m_DelChk = 0;

	for (int col = 0; col < 2; ++col)
    {
		for (int row = 0; row < 3; ++row)
		{	
			m_hEtherWeaponEffect[col][row] = INVALID_HANDLE_VALUE;
		}
    }

	//m_hSummonIntensifyEffect = INVALID_HANDLE_VALUE;

	m_dwCollectObjKey = 0;

	m_wiPreAniID = INVALID_WZID;

	m_btIdleAni = 0;
	SetbtCriAttkAni(0);


    is_spread_wings_ = false;

    m_hCollectStartEffect = INVALID_HANDLE_VALUE;
    m_hDominationStampEffect = INVALID_HANDLE_VALUE;

    m_PlayerData.Initialize();

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    battle_ground_hide_mode_ = BattleGroundHideMode_None;
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    m_ItemScore = 0;
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE

}



//------------------------------------------------------------------------------
/**
*/
Player::~Player( void )
{
    m_PlayerData.Release();

    RemoveDominationStateEffect();	
    DestroyCharacterVariation();
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	RemovePlayerRankingEffect();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
}

//------------------------------------------------------------------------------
/**
*/
WzVector Player::GetHeadBonePos()
{
	return GetSpecialBonePos(StrToWzID("HEAD"));
}

//------------------------------------------------------------------------------
/**
*/
void Player::SetKeyboardMoveTime(DWORD dwTime)
{
	m_dwKeyboardMoveTime=dwTime;
}

//------------------------------------------------------------------------------
/**
*/
void Player::AddKeyboardMoveTime(DWORD dwDelta)
{
	m_dwKeyboardMoveTime+=dwDelta;
}

//------------------------------------------------------------------------------
/**
*/
DWORD Player::GetKeyboardMoveTime()
{
	return m_dwKeyboardMoveTime;
}

//------------------------------------------------------------------------------
/**
*/
void Player::TransForm()
{
	if( GetTransformData().GetTransFormProcessType() == TRANSFORM_PROCESS_TRANSFORM )
	{
        //RestoreFullsetResource();

		APPEARANCEINFO appearInfo;
		ZeroMemory(&appearInfo,sizeof(appearInfo));
		appearInfo.ObjectType = (APPEARANCEINFO::eAPPEARKIND)GetTransformData().GetAppearanceKind();	
		appearInfo.resourceCode = GetTransformData().GetTransFormResourceCode();
		InitRender(1,0,&appearInfo,0,FALSE);

		if(GetTransformData().GetTransFormType()==eTRANSFORM_TYPE_DRAGON1)
			this->SetCurrentAttackStyle(eSTYLE_DRAGON_TRANSFORM1);
		else if(GetTransformData().GetTransFormType()==eTRANSFORM_TYPE_DRAGON2)
			this->SetCurrentAttackStyle(eSTYLE_DRAGON_TRANSFORM2);

		if( GetCharInfo()->GetChangeOfClass() > 10 )
		{
			GetCharInfo()->SetChangeOfClass( (eCHAREX_TYPE) (GetTransformData().GetTransFormType() + 10));
		}

#ifdef _NA_008536_20151023_ELITE4TH_ADD
		if (this->GetClass() == eCHAR_DRAGON)
		{
			RefreshFullsetItemInfo( this );
		}
#endif // _NA_008536_20151023_ELITE4TH_ADD
	}
}
//------------------------------------------------------------------------------
/**
*/
void Player::UnTransForm()
{
    if(GetTransformData().GetTransFormProcessType()==TRANSFORM_PROCESS_UNTRANSFORM)
    {	
        APPEARANCEINFO info;
        ZeroMemory(&info,sizeof(info));
        info.Job = GetClass();
        info.ObjectType = (APPEARANCEINFO::eAPPEARKIND)GetTransformData().GetAppearanceKind();	
        InitRender(BP_COUNT,BP_BONE,&info, AppearanceManager::kAppearanceAttachIndex_Count, FALSE);
        RefreshAppearRender();

        SetTransSkillCode(0);
        GetTransformData().SetTransFormType(eTRANSFORM_TYPE_NONE);

        CreateEtherWeaponEffect(false,true);

        if( GetCharInfo()->GetChangeOfClass() > 10 )
        {
            GetCharInfo()->SetChangeOfClass( eCHAREX_TYPE( GetClass() + 10));
        }

    }
}

void Player::TransformGenderType()
{
    SetAnimation("NULL");

    APPEARANCEINFO appearInfo;
    ZeroMemory(&appearInfo,sizeof(appearInfo));
    appearInfo.ObjectType = APPEARANCEINFO::PLAYER;
    appearInfo.Job = GetClass();
    InitRender(BP_COUNT,BP_BONE, &appearInfo, AppearanceManager::kAppearanceAttachIndex_Count, TRUE);

    RefreshAppearRender();

    CreateEtherWeaponEffect(false,true);

    ProcessNextAction();
}


void Player::SetPolymorph( BOOL Active )
{
    if( m_IsPolymorphApperance == Active )
        return;

    m_IsPolymorphApperance = Active;

    if( m_IsPolymorphApperance == TRUE )
    {
		if( IsMyHero() == TRUE )
		{
			g_HeroInput.SetAutoAttack(FALSE);
			g_HeroInput.SetFollowState(FALSE);
		}

		//OnParalyzed();
		//SetVisiblePos( GetPosition() );

        APPEARANCEINFO appearInfo;
        ZeroMemory(&appearInfo,sizeof(appearInfo));
        appearInfo.ObjectType = APPEARANCEINFO::POLOMORPH;
        //appearInfo.pMonsterInfo = GetMonsterInfo();

        InitRender( MP_COUNT, MP_BONE, &appearInfo, 0, FALSE );
        //appearInfo.Job = GetClass();
        //InitRender(BP_COUNT, BP_BONE, &appearInfo, AppearanceManager::kAppearanceAttachIndex_Count, TRUE);
		RefreshRender( &appearInfo, TRUE );

		SetIdleState();

		
        //InitState(STATE::IDLE,g_CurTime);

		GlobalFunc::CreateEffectBeUpToOption( StrToWzID("KC05"), GetVisiblePos() );
    }
    else
    {

        APPEARANCEINFO appearInfo;
        ZeroMemory(&appearInfo,sizeof(appearInfo));
        appearInfo.ObjectType = APPEARANCEINFO::PLAYER;	
        appearInfo.Job = GetClass();

        InitRender( BP_COUNT, BP_BONE, &appearInfo, AppearanceManager::kAppearanceAttachIndex_Count, FALSE );
		RefreshAppearRender(TRUE);

		SetIdleState();

		GlobalFunc::CreateEffectBeUpToOption( StrToWzID("KC05"), GetVisiblePos() );

        ////	상태 초기화( IDLE 상태 )
        //InitState(STATE::IDLE,g_CurTime);
    }
}

void Player::ForceUnTransForm()
{
	GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_UNTRANSFORM);
	GetTransformData().SetAppearanceKind(APPEARANCEINFO::PLAYER);

	GetTransformData().SetTransFormType(eTRANSFORM_TYPE_NONE);
	UnTransForm();
	GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_NONE);

}

void Player::ForceTransFrom()
{
	GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_TRANSFORM);
	GetTransformData().SetAppearanceKind(APPEARANCEINFO::PLAYERTRANSFORM);

	TransForm();

	GetTransformData().SetTransFormProcessType(TRANSFORM_PROCESS_NONE);
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentTransFormAnimation()
{
	WzID animationID = 0;	
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	if(GetTransformData().GetTransFormProcessType()==TRANSFORM_PROCESS_TRANSFORM)
	{
		szAniCode[0] = 'T';
		szAniCode[1] = '0';
		szAniCode[2] = '0';
		szAniCode[3] = '1';

	}
	else
	{
		szAniCode[0] = 'T';
		szAniCode[1] = '0';
		szAniCode[2] = '0';
		szAniCode[3] = '2';
	}

	animationID = StrToWzID(szAniCode);
	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Player::Create( DWORD dwObjectKey, DWORD dwID)
{
	SetObjectKey( dwObjectKey );

    cPlayerItemEffect::Initialize();

#ifdef _NA_004035_20120227_COSTUME_ITEM
    GetCharInfo()->is_equipped_costume_ = false;
    GetCharInfo()->costume_item_pos_ = INVALID_SLOT_POS;
    GetCharInfo()->costume_item_code_ = 0;
#endif //_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	GetCharInfo()->is_equipped_decorate_costume_ = false;
	GetCharInfo()->decorate_costume_item_pos_ = INVALID_SLOT_POS;
	GetCharInfo()->decorate_costume_item_code_ = 0;
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

    ClassCodeEx class_code;
    class_code.class_code = dwID;

    GetCharInfo()->m_byClassCode = static_cast<BYTE>(class_code.class_code);
    GetCharInfo()->SetChangeOfClass(static_cast<eCHAREX_TYPE>(class_code.class_code));

    //m_fLandFrame=0.0f;
    BOOL rt = Character::Create(dwObjectKey, dwID);
    assert(rt);

    APPEARANCEINFO appearInfo;
    ZeroMemory(&appearInfo,sizeof(appearInfo));
    appearInfo.ObjectType = APPEARANCEINFO::PLAYER;	
    appearInfo.Job = dwID;		// 일단 ID를 캐릭터 종류로 쓴다. 

    CreateCharacterVariation(&appearInfo, TRUE);
    InitContainer();

	//	상태 초기화( IDLE 상태 )
	InitState(STATE::IDLE,g_CurTime);

	m_iValkyrieShotDelay	= 0;
	m_iValkyrieShotCount	= 0;
	m_iValkyrieShotRemain	= 0;
	m_bSpecialMoveFlag=0;
	m_dwValkyrieShotSerial	= 0;	
	m_dwDustTime			= 0;
	m_iValkyrieBulletCount = 20;

	m_iValkyrieShotAnimSet = 0;
	m_iValkyrieShotAnimCount = 0;

	//	GM 관련
	m_byGMGrade				= 0;			// GM 등급
	m_bInvisible			= FALSE;		// 무적

	// PVP 관련
	m_iTeam					= INVALID_TEAM;

	m_bHPInfo = FALSE;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    m_bSDInfo = FALSE;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

	SetBeHaveState( PLAYER_BEHAVE_IDLE_STATE );	
	m_dwShieldTime=0;
	m_bStandUp=FALSE;

	m_dwTransSkillCode=0;
	m_bAttackBeginPlayer=FALSE;


	//InitAttachEffectForSetItem();

	m_dwCollectStartTime = 0;

    InitMeshHeight();

	return TRUE;
}

void Player::SetTransSkillCode(DWORD dwSkillCode)
{
	m_dwTransSkillCode = dwSkillCode;

	//기존에테르탄 이펙트 전부제거
	if (IsEtherDeviecBulletChargeing())
	{	
		CreateEtherWeaponEffect(false, TRUE);
	}

}

DWORD  Player::GetTransSkillCode()
{
	return m_dwTransSkillCode;
}
//------------------------------------------------------------------------------
/**
*/
void Player::Destroy( void )
{
	DestroyRider();

	ReleaseContainer();
	Character::Destroy();


    cPlayerItemEffect::Destroy();
    DestoryEtherWeaponEffect();
}

void Player::SetClass(eCHAR_TYPE character_class)
{
    Character::SetClass(character_class);
    GetCharInfo()->m_byClassCode = static_cast<BYTE>(character_class);
}

void Player::SetGender(GenderType::Value gender)
{
    Character::SetGender(gender);
    GetCharInfo()->gender = static_cast<BYTE>(gender);
}

//------------------------------------------------------------------------------
/** 상황에 따라서 옵셋값을 변경하구싶을때..기입한다
*/
float Player::GetHeightOffset()
{
    if (GetBasePart() == NULL)
    {
        return 0.0f;
    }

    float height_offset = 0.0f;

    if (IsRiding()) 
    {
        //! 캐릭터 하체 높이
        float kSitHeight= 1.0f;
		switch( GetImageClass() )
		{
		case eCHAR_BERSERKER:		kSitHeight = 1.0f; break;
		case eCHAR_DRAGON:			kSitHeight = 1.0f; break;
		case eCHAR_SHADOW:			kSitHeight = 1.0f; break;
		case eCHAR_VALKYRIE:		kSitHeight = 1.0f; break;
		case eCHAR_ELEMENTALIST:	kSitHeight = 1.0f; break;
		case eCHAR_MYSTIC:			kSitHeight = 0.8f; break;
        case eCHAR_HELLROID:        kSitHeight = 1.0f; break;   //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eCHAR_WITCHBLADE:      kSitHeight = 1.0f; break;   //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		}

        float riding_Offset = fabs(GetVisiblePos().z - m_pRider->GetLinkPos()->z);
        height_offset +=(riding_Offset - kSitHeight);
        //height_offset +=(riding_Offset * 0.8f);
    }
    if (is_spread_wings())
    {
        float kFloorToBodyBone_Length = 1.5f;

		switch( GetImageClass() )
		{
		case eCHAR_BERSERKER:		kFloorToBodyBone_Length = 1.5f; break;
		case eCHAR_DRAGON:			kFloorToBodyBone_Length = 1.4f; break;
		case eCHAR_SHADOW:			kFloorToBodyBone_Length = 1.4f; break;
		case eCHAR_VALKYRIE:		kFloorToBodyBone_Length = 1.25f; break;
		case eCHAR_ELEMENTALIST:	kFloorToBodyBone_Length = 1.25f; break;
		case eCHAR_MYSTIC:			kFloorToBodyBone_Length = 1.0f; break;
        case eCHAR_HELLROID:        kFloorToBodyBone_Length = 1.4f; break;  //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        case eCHAR_WITCHBLADE:      kFloorToBodyBone_Length = 1.25f; break; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		}

        WzVector pos;
        if (GetBodyBonePos(&pos))
        {
            float body_bone_offset = fabs(GetVisiblePos().z - pos.z) - kFloorToBodyBone_Length;
            height_offset += body_bone_offset;
        }
        else
        {
            height_offset += kFloorToBodyBone_Length;
        }
    }
    
    return height_offset;
}


//------------------------------------------------------------------------------
/**  라이딩상태체크:이 Flag에따라서 탈것을 타고안타고를 가늠한다.
*/
BOOL Player::IsRiding()
{
    //라이딩상태인지
    if(m_bRiding && m_pRider) 
    {
        //로비거나 지연로딩일때는 라이딩상태아님의로간주
        if (BattleScene::IsLobby())
        {
            GetBasePart()->SetRotationByMatrix(NULL);
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}
//------------------------------------------------------------------------------
/**  라이더 refresh
*/
void Player::RefreshRider()
{
    if( IsRiding() == TRUE && IsUseDelayLoading() == false )
    {
        m_pRiderContainer->RefreshApperance();
        m_pRider->EnableFunction(TRUE);

#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
        m_pRiderContainer->RefreshRiderSpeed(); // 탈것 이속 갱신
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC
    }
}
//------------------------------------------------------------------------------
/**  라이더 인첸트효과
*/
char Player::GetRiderKindCode()
{
	return m_pRider->GetAniMapping();
}
//------------------------------------------------------------------------------
/**  라이더 인첸트효과
*/
void Player::RiderSubtype(int nPartIdx,int nSubType)
{
	if(IsRiding())
	{
		m_pRider->SetSubtype(nPartIdx,nSubType);
	}
}
//------------------------------------------------------------------------------
/**  라이더 파츠붙이기
*/
void Player::AttachRiderPart(int nIdx, DWORD dwItemCode)
{
	if(IsRiding())
	{
		m_pRider->AttachRiderPart(nIdx,dwItemCode);
	}
}
//------------------------------------------------------------------------------
/**  라이더 파츠제거
*/
void Player::DetachRiderPart(int nIdx)
{
    if(IsRiding())
    {
        m_pRider->DetachRiderPart(nIdx);
    }
}
//------------------------------------------------------------------------------
/**  라이더 생성
*/
void Player::CreateRider(DWORD dwID,BOOL bEffect)
{
    m_nRiderIdx = 0;

	//라이딩상태라면 기존탈것을 해제한다
	if(IsRiding())
	{
		m_pRider->Destroy();
		SAFE_DELETE( m_pRider );
	}

	m_pRider  = new Rider;

    if(m_pRider)
	{
        SetRindingState(TRUE);

        m_bRiding = m_pRider->Create(dwID, this, bEffect);

		//생성실패라면
		if(m_bRiding == FALSE)
		{
			SAFE_DELETE( m_pRider );
		}
        else
        {
            m_nRiderIdx= dwID;  
            m_dwStandAnimDelay = 0;
            //노말벡터를z값을 1.0f로 강제하는 이유는 Rider에서 요값을 기준의로 SetRotation하는데
            //이값이 정상셋팅이아닐경우 현재 요함수 호출시점에는 값이 제대로 안되어있다 0벡터로초기화되어있음
            SetVector(&m_vNormal,0.0f,0.0f,1.0f);
        }
	}
}
//------------------------------------------------------------------------------
/** 라이더 해제
*/
void Player::DestroyRider()
{
	if(m_pRider)
	{
        //컨테이너도 제거한다.
        if(m_pRiderContainer)
        {
            m_pRiderContainer->Clear();
        }
		
        GetBasePart()->SetRotationByMatrix(NULL);

        GetBasePart()->UpdateTransform();

        m_pRider->Destroy();
		SAFE_DELETE( m_pRider );
		m_bRiding = FALSE;

        // 날개 펴기/접기 중일때 탈것이 사라지면 애니메이션 바로 바꿔 준다.
        if ((GetCurState() == STATE::RIDER_SPREAD) || (GetCurState() == STATE::RIDER_FURL))
        {
            if(GetNextState() == STATE::NONE)
            {
                SetNextState(STATE::IDLE, g_CurTime);
            }
            CommitChangeState(g_CurTime);
        }
	}
}
//------------------------------------------------------------------------------
/** 
*/
BOOL Player::ProcessInterpolation( DWORD dwTick ,float fInterpolateRatio)
{
	BOOL bRet = Character::ProcessInterpolation(dwTick,fInterpolateRatio);

	if(IsRiding())
	{
		//캐릭의 실제위치를 라이더에게 셋팅해준다.
		WzVector vPos=GetBasePart()->GetPosition();
		m_pRider->SetPosition(&vPos);
	}
	return bRet;
}
//------------------------------------------------------------------------------
/** 라이더의 애니메이션후 바로 결과값을 플레이어에게 적용시켜준다(위치.회전)
*/
void Player::AnimateRider()
{
	m_pRider->AnimateUnit(this);

    m_pRider->Transform(GetBasePart());
}
//------------------------------------------------------------------------------
/** 
*/
BOOL Player::ProcessRider(DWORD dwTick)
{
   
    //자유이동시에는 기울기적용안한다.
    if(m_bRiderJumpFlag || ( rider()&& rider()->is_spread()))
    {
        m_pRider->SetRotation( &c_UpVector, GetVisibleAngleZ() );
    }
    else
    {
        m_pRider->SetRotation( &m_vNormal, GetVisibleAngleZ() );
    }

	m_pRider->Process(dwTick, this);

	return TRUE;
}
//------------------------------------------------------------------------------ 
void Player::SetAnimation(const char* pAnimation, BOOL bLoop)
{
    Player::SetPlayerAnimation(StrToWzID(pAnimation), eANI_IDLE, bLoop);
}
//------------------------------------------------------------------------------ 
void Player::SetAnimation(WzID idAnimation, BOOL bLoop)
{
    Player::SetPlayerAnimation(idAnimation, eANI_IDLE, bLoop);
}
//------------------------------------------------------------------------------
void Player::SetPlayerAnimation(WzID wzID, ePLAYER_ANI eState, BOOL bLoop)
{
    //날고 있는 상태에서 이동에 대한 애니메이션은 특별처리
    BOOL bRestart = TRUE;
    if ((eState == eANI_SPREAD_OTHERMOTION_MOVE) || (eState == eANI_SPREAD_OTHERMOTION_IDLE))
    {
        bLoop = FALSE; 
        bRestart = FALSE;
    }
    //if ((wzID == GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_SPREAD_OTHERMOTION_MOVE)) || 
    //    ((IsRiding() == TRUE) && (wzID == GlobalFunc::GetPlayerAnimationByRiding(eANI_SPREAD_OTHERMOTION_MOVE, GetRiderKindCode()))))
    //{
    //    bLoop = FALSE; 
    //    bRestart = FALSE;
    //    eState = eANI_SPREAD_OTHERMOTION_MOVE;
    //}
    //else if ((wzID == GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_SPREAD_OTHERMOTION_IDLE)) || 
    //    ((IsRiding() == TRUE) && (wzID == GlobalFunc::GetPlayerAnimationByRiding(eANI_SPREAD_OTHERMOTION_IDLE, GetRiderKindCode()))))
    //{
    //    bLoop = FALSE; 
    //    bRestart = FALSE;
    //    eState = eANI_SPREAD_OTHERMOTION_IDLE;
    //}

    Object::SetAnimation(wzID, bLoop, bRestart);

	if (IsRiding())
	{
        WzID ani_id = GlobalFunc::GetRiderAnimationByPlayerAni(eState, rider()->is_spread());
		m_pRider->SetAnimation(ani_id, bLoop, bRestart);
	}
    
    int attach_index = 0;
    if (is_attached_wing_accessory(&attach_index) && (attach_index < (int)GetAttachmentCount()))
    {
        WzID ani_id = GlobalFunc::GetWingsAnimationByPlayerAni(eState, is_spread_wings());
        const ObjectAttachment& object_attachment = GetAttachment(attach_index);
        ((ObjectAttachment*)(&object_attachment))->SetAnimation(ani_id, bLoop, bRestart);
    }
}
//------------------------------------------------------------------------------
/**
*/
void Player::RenderRider()
{
	m_pRider->SetColor(GetColor());
	m_pRider->Render();	
}


//------------------------------------------------------------------------------
/** 현재 들고있는 무기가(캔디이펙트)무기인지 체크한다
현재 스크립트연동없이 범용가치가없어서 하드코딩의로 적용
*/
BOOL Player::IsEventWeapon()
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container != NULL)
    {
        SCSlot& slot_ref = equipment_container->GetSlot(EquipmentContainer::EQUIP_WEAPON1);
        SCItemSlot& item_slot_ref = static_cast<SCItemSlot&>(slot_ref);

        if (item_slot_ref.IsContainValidItem())
        {
            if ((item_slot_ref.GetImageCode() >= 50310) && 
                (item_slot_ref.GetImageCode() <= 50319))
            {
                return TRUE;
            }

            // 미스틱 무기
            if( item_slot_ref.GetImageCode() == 50495 )
                return TRUE;
            if( item_slot_ref.GetImageCode() == 50496 )
                return TRUE;

            // 헬로이드 ~ 이후 신규클래스 대역
            if ((item_slot_ref.GetImageCode() >= 50533) && 
                (item_slot_ref.GetImageCode() <= 50543))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Player::Render( void )
{
	if (m_bObserverMode)
    {
		return TRUE;
    }

	if ( GetShow() == TRUE )
	{		

		if (this->m_bInvincible) 
		{
			SetColor( 154, 154, 154,100);

		}

		BOOL bRet = Character::Render();	

		if (IsRiding() &&
            bRet)
		{
			RenderRider();
		}


		return bRet;	
	}
	else
	{
		return TRUE;
	}	
}

void Player::ApplyTerrainColor(BOOL bApply)
{
	Character::ApplyTerrainColor(bApply);

	// 이거 왜 이런식으로 했는지 몰라도 일단 모두 적용되도록 수정함 
	//if (!m_bInvincible) 
	//{
	//	Character::ApplyTerrainColor(TRUE);


	//	if((this == g_pHero) || IsRiding())
	//	{
	//		Character::ApplyTerrainColor(TRUE);
	//	}
	//	else
	//	{
	//		Character::ApplyTerrainColor(FALSE);
	//	}
	//}
}

//------------------------------------------------------------------------------
/**
*/
SCSlotContainer * Player::GetSlotContainer( SLOTIDX slotIdx )
{
	switch( slotIdx )
	{
	case SI_EQUIPMENT:
		{
			return GetEquipmentContainer();
		}
		break;

    case SI_RIDER:
        {
            return m_pRiderContainer;    
        }
        break;

	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void Player::InitContainer()
{
    if (GetEquipmentContainer() == NULL)
    {
        EquipmentContainer* equipment_container = new EquipmentContainer();
        equipment_container->SetOwnerKey(GetObjectKey());
        SetEquipmentContainer(equipment_container);
    }

    m_pRiderContainer = new RiderContainer();
    m_pRiderContainer->SetOwnerKey(GetObjectKey());
}

//------------------------------------------------------------------------------
/**
*/
void Player::ReleaseContainer()
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container != NULL)
    {
        equipment_container->Release();	
        delete equipment_container;
        SetEquipmentContainer(NULL);
    }

    SAFE_RELEASENDELETE(m_pRiderContainer);
}


//------------------------------------------------------------------------------
/**
*/
void Player::AnimateUnit()
{
	if(IsRiding())
	{
		AnimateRider();
	}

	Character::AnimateUnit();
    UpdateEtherWeaponEffectPos();
    UpdateItemEffectPos(this);
}

//------------------------------------------------------------------------------
/**
*/
void Player::ResetMoveFlag()
{
//#ifdef _DEBUG
//    if( m_iMoveFlag != 0 )
//    {
//        char temp[1024];
//        wsprintf(temp,"ResetMoveFlag\n");
//        OutputDebugStr(temp);
//    }
//#endif 

	m_iMoveFlag = 0;
}

//------------------------------------------------------------------------------
/**
*/
void Player::AddMoveFlag(int MoveFlag)
{	
	BIT_ADD(m_iMoveFlag,MoveFlag);
}

//------------------------------------------------------------------------------
/**
*/
void Player::RemoveMoveFlag(int MoveFlag)
{
	BIT_REMOVE(m_iMoveFlag,MoveFlag);
}

//------------------------------------------------------------------------------
/**
*/
BOOL Player::CheckMoveFlag(int MoveFlag)
{
	return BIT_CHECK(m_iMoveFlag,MoveFlag);
}

//------------------------------------------------------------------------------
/**
*/
void Player::SetMoveFlag(int MoveFlag)
{
	m_iMoveFlag = MoveFlag;
}

#ifdef _NA_004035_20120227_COSTUME_ITEM
void Player::SetCostumeItemCode( SLOTCODE item_code )
{
    GetCharInfo()->costume_item_code_ = item_code;
}

SLOTCODE Player::GetCostumeItemCode()
{
    return GetCharInfo()->costume_item_code_;
}
#endif//_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
void Player::SetDecorateCostumeItemCode( SLOTCODE item_code )
{
	GetCharInfo()->decorate_costume_item_code_ = item_code;
}

SLOTCODE Player::GetDecorateCostumeItemCode()
{
	return GetCharInfo()->decorate_costume_item_code_;
}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
void Player::SetSpaItemCode( SLOTCODE item_code )
{
    spa_item_code_ = item_code;
}

SLOTCODE Player::GetSpaItemCode()
{
    return spa_item_code_;
}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
//------------------------------------------------------------------------------
/**
*/
void Player::SetClientChar(const CLIENT_CHARACTER_PART& ClientPart)
{
	GetCharInfo()->m_bySlot			= ClientPart.m_bySlot;
	GetCharInfo()->m_byClassCode	= ClientPart.m_byClass;
    GetCharInfo()->gender           = ClientPart.gender;
	GetCharInfo()->m_LV				= ClientPart.m_LV;
	SetName( ClientPart.m_tszCharName);
	GetCharInfo()->m_iRegion		= ClientPart.m_iRegion;
	GetCharInfo()->SetChangeOfClass( (eCHAREX_TYPE)ClientPart.m_ChangeOfClassStep);

#ifdef _NA_004035_20120227_COSTUME_ITEM
    GetCharInfo()->is_equipped_costume_ = ClientPart.is_equipped_costume;
    GetCharInfo()->costume_item_pos_ = ClientPart.costume_item_pos;
    GetCharInfo()->costume_item_code_ = ClientPart.costume_item_code;
#endif//_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	GetCharInfo()->is_equipped_decorate_costume_ = ClientPart.is_equipped_decorate_costume;
	GetCharInfo()->decorate_costume_item_pos_ = ClientPart.decorate_costume_item_pos;
	GetCharInfo()->decorate_costume_item_code_ = ClientPart.decorate_costume_item_code;
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

	SetPartVariation(PLAYER_VARIATION_HAIR,ClientPart.m_byHair);
	SetPartVariation(PLAYER_VARIATION_FACE,ClientPart.m_byFace);
	SetHeightVariation(ClientPart.m_byHeight);

	SetEquipItemTotalInfo( ClientPart.m_EquipItemInfo );

	m_DelChk = ClientPart.m_DelChk;
	StrnCopy(m_tszDelDate, ClientPart.m_tszDelDate , MAX_TIMEDATA_SIZE);
	m_tszDelDate[MAX_TIMEDATA_SIZE] = NULL;

	//펫이 소환된상태라면
	if (ClientPart.m_bSummoned)
	{
		SCItemSlot ItemSlot(ClientPart.m_PetSlotStream);
		
		if (ItemSlot.IsContainValidItem())
		{
			CreatePetSummon(ItemSlot.GetImageItemInfo()->m_wSkillCode);
		}
	}

	// 이큅관련해서 제대로 입혀준다.
    //RefreshEquipData();
	RefreshAppearRender();

    if (IsEtherDeviecBulletChargeing())
    {
        CreateEtherWeaponEffect(0, TRUE);
    }
}

//------------------------------------------------------------------------------ 
void Player::GetEquipItemTotalInfoByCheckValidItem(EQUIP_ITEM_INFO& OUT equip_item_info)
{
    ITEMSLOTEX* pItemSlot = equip_item_info.m_Slot; 
    equip_item_info.m_Count = 0;
 
    if (GetEquipmentContainer() == NULL)
    {
        return;
    }
 
    ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = GetEquipmentContainer()->GetEquipmentManageInfo();
    for (int equip_pos = 0; equip_pos < GetEquipmentContainer()->GetMaxSlotNum(); ++equip_pos)
    {
        //슬롯 유효한지 체크
        SCItemSlot* item_slot = GetEquipmentContainer()->GetItemSlot(equip_pos);
        if (item_slot == NULL)
        {
            continue;
        }
 
        // 해당 슬롯이 활성화 되어 있는지 체크
        ns_formula::EquipSlotsManageInfo::Node* equip_node = equip_slot_manage_info->slots;
        if (equip_node[item_slot->GetPos()].activated == false)
        {
            continue;
        }
 
        pItemSlot[equip_item_info.m_Count].m_Pos = equip_pos;
        item_slot->CopyOut(pItemSlot[equip_item_info.m_Count].m_Stream);
        ++equip_item_info.m_Count;			
    }
}
//------------------------------------------------------------------------------ 
VOID Player::GetEquipItemTotalInfo( EQUIP_ITEM_INFO & OUT rItemInfo )
{
	ITEMSLOTEX * pItemSlot = rItemInfo.m_Slot; 

	rItemInfo.m_Count = 0;

    if (GetEquipmentContainer() == NULL)
        return;

	POSTYPE start = 0;
	POSTYPE total = MAX_EQUIPMENT_SLOT_NUM;

	for (POSTYPE i = start; i < total; ++i)
	{
		if (GetEquipmentContainer()->IsEmpty(i) == FALSE)
		{
			pItemSlot[rItemInfo.m_Count].m_Pos= i;
			((SCItemSlot &)GetEquipmentContainer()->GetSlot(i)).CopyOut( pItemSlot[rItemInfo.m_Count].m_Stream );

			++rItemInfo.m_Count;			
		}
	}
}


VOID Player::SetEquipItemTotalInfo( const EQUIP_ITEM_INFO & IN rItemInfo )
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return;
    }

    const ITEMSLOTEX* equip_slot_ptr = rItemInfo.m_Slot;

    for(POSTYPE index = 0; index < rItemInfo.m_Count; ++index)
    {
        SCItemSlot insert_slot(equip_slot_ptr[index].m_Stream);

        equipment_container->InsertSlot(equip_slot_ptr[index].m_Pos, insert_slot);
    }
}

VOID Player::SetEquipItemBaseInfo( POSTYPE total, EQUIP_ITEM_BASEINFO::data_type* IN pSlot )
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return;
    }

    for (POSTYPE index = 0; index < total; ++index)
    {
        SCItemSlot insert_slot(pSlot[index].m_Stream);

        equipment_container->JustInsertSlot(pSlot[index].m_Pos, insert_slot);
    }
   
    RefreshAppearRender();

    if (IsEtherDeviecBulletChargeing())
    {
        CreateEtherWeaponEffect(0, TRUE);
    }

    // 이큅정보를 모두 받았다.
    set_receive_equip_info(true);
}

//------------------------------------------------------------------------------
/**
*/
int Player::GetMoveFlag()
{
	return m_iMoveFlag;
}

//------------------------------------------------------------------------------
/**
*/
float Player::ProcessAnimation(DWORD dwTick,WzEvent *pEvent,BOOL bIgnoreEvent)
{	
	ProcessStandAnimationDelay(dwTick);

	if(m_wiPreAniID != GetCurrentAnimation())
	{
        // 캐릭터 애니메이션에 맞춰서 무기를 애니메이션 시켜준다
        // 석궁은 ShowCrossbow 에서 직접 애니메이션을 제어하므로 제외

        if (GetWeaponKind() != eWEAPONTYPE_ONEHANDCROSSBOW )
        {
            // 헬로이드의 N203 (전투IDLE) 만 루프
            if( GetWeaponKind() == eWEAPONTYPE_HELLROID_1 && GetCurrentAnimation() == StrToWzID("N203") )
            {
                SetAttachmentAnimation(0, GetCurrentAnimation(), TRUE);
            }
            else
            {
                SetAttachmentAnimation(0, GetCurrentAnimation(), FALSE);
            }
        }
	}

	m_wiPreAniID = GetCurrentAnimation();
		
	float fRet = Character::ProcessAnimation(dwTick,pEvent,bIgnoreEvent);

	if(IsRiding())
	{
		ProcessRider(dwTick);
        fRet = rider()->current_ani_progress_rate();
	}

	return fRet;

}

//------------------------------------------------------------------------------
/**
*/
float Player::GetAngleForMoveDirection()
{
	WzVector vPos			= GetPosition();
	WzVector vFoward		= GetDirection();
	WzVector vRight,vLeft;
	WzVector vDir;
	SetVector( &vDir,0.0f, 0.0f, 0.0f );

	SetVector( &vRight, vFoward.y, -vFoward.x, 0.0f );
	SetVector( &vLeft,  -vFoward.y, vFoward.x, 0.0f );

	if( GetMoveFlag() & KEYBOARDMOVE_FOWARD )
	{
		vDir =  vDir + vFoward;
	}
	else if( GetMoveFlag() & KEYBOARDMOVE_BACKWARD )
	{
		vDir =  vDir - vFoward;
	}

	if( GetMoveFlag() & KEYBOARDMOVE_RIGHTSTEP )
	{
		vDir =  vDir + vRight;
	}
	else if( GetMoveFlag() & KEYBOARDMOVE_LEFTSTEP )
	{
		vDir =  vDir + vLeft;		
	}

	return Math_GetAngleFromVector( &vDir );
}

//------------------------------------------------------------------------------
/**
해당 무기와 스타일과 타수에 맞는 에니 아뒤를 얻는다.

*/
WzID Player::GetCurrentAttackAnimation()
{
	WzID animationID = StrToWzID("A001");
	DWORD dwStyle = GetCurrentAttackStyle();
	//Todo:스타일이 없을경우 2타를 넘어가서는 안된다.
	DWORD sequence=GetAttackSequence();
	DWORD weapon  =GetWeaponKind();

	BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo( (SLOTCODE)dwStyle );
    // 한손석궁이 아니면
    if (weapon != eWEAPONTYPE_ONEHANDCROSSBOW)
    {
#ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
        animationID = GetNormalAttackAniCode(pInfo, static_cast<eATTACK_SEQUENCE>(sequence), NormalAttack.ani_type);
#else
        switch(sequence)
        {
        case eATTACK_SEQUENCE_FIRST:
            animationID = pInfo->m_wzidCodeFirst.id;
            break;
        case eATTACK_SEQUENCE_SECOND:
            animationID = pInfo->m_wzidCodeSecond.id;
            break;
        case eATTACK_SEQUENCE_THIRD:
            animationID = pInfo->m_wzidCodeThird.id;
            break;
        }
#endif //_NA003109_STYLE_PRESENTATION_IMPROVEMENT

    }
    else
    {
        if (m_iValkyrieShotAnimSet)
        {
            switch(m_iValkyrieShotAnimCount)
            {
            case 0:
            case 1:
            case 2:
                animationID = pInfo->m_wzidCodeFirst.id;
                break;
            default:
                animationID = pInfo->m_wzidCodeSecond.id;
                break;				
            }
        }
        else
        {
            switch(m_iValkyrieShotAnimCount)
            {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                animationID = pInfo->m_wzidCodeSecond.id;
                break;
            default:
                animationID = pInfo->m_wzidCodeFirst.id;
                break;				
            }
        }

        m_iValkyrieShotAnimCount++;

        if (m_iValkyrieShotAnimCount >= 8)
        {
            m_iValkyrieShotAnimSet = rand() % 2;

            m_iValkyrieShotAnimCount = 0;
        }
    }		

	return animationID;
}


void Player::RefreshPunchStyle()
{
	switch(GetClass())
	{
	case eCHAR_BERSERKER:   SetCurrentAttackStyle(eSTYLE_BERSERKER_PUNCH); break;
	case eCHAR_DRAGON:      SetCurrentAttackStyle(eSTYLE_DRAGON_PUNCH);    break;
	case eCHAR_SHADOW:      SetCurrentAttackStyle(eSTYLE_SHADOW_PUNCH);	break;
	case eCHAR_VALKYRIE:	SetCurrentAttackStyle(eSTYLE_VALKYRIE_PUNCH);	break;
	case eCHAR_ELEMENTALIST:SetCurrentAttackStyle(eSTYLE_MAGICIAN_PUNCH);	break;
    case eCHAR_MYSTIC:      SetCurrentAttackStyle(eSTYLE_MYSTIC_PUNCH);    break;
	}
}

//------------------------------------------------------------------------------
/**
현재 장착중인 무기로 nomal style 상태를 가린다.
무기로 현재 착용 중인 스타일의 노멀을 체크 할수 없다...하드 코딩 피할수 없다!

*/
void Player::SetCurrentNomalAttackStyle(DWORD dwWeaponType)
{
	switch(dwWeaponType)
	{
    case eWEAPONTYPE_TWOHANDAXE:            SetCurrentAttackStyle(eSTYLE_TWOHANDAXE_NORMAL);       break;
    case eWEAPONTYPE_TWOHANDSWORD:          SetCurrentAttackStyle(eSTYLE_TWOHANDSWORD_NORMAL);     break;
    case eWEAPONTYPE_ONEHANDSWORD:          SetCurrentAttackStyle(eSTYLE_ONEHANDSWORD_NORMAL);     break;
    case eWEAPONTYPE_SPEAR:                 SetCurrentAttackStyle(eSTYLE_SPEAR_NORMAL);            break;
    case eWEAPONTYPE_DAGGER:                SetCurrentAttackStyle(eSTYLE_SHADOW_DAGGER_NORMAL);    break;
    case eWEAPONTYPE_WHIP:                  SetCurrentAttackStyle(eSTYLE_SHADOW_WHIP_NORMAL);      break;
    case eWEAPONTYPE_STAFF:                 SetCurrentAttackStyle(eSTYLE_STAFF_NORMAL);            break;
    case eWEAPONTYPE_ORB:                   SetCurrentAttackStyle(eSTYLE_ORB_NORMAL);              break;
    case eWEAPONTYPE_GUNTLET:               SetCurrentAttackStyle(eSTYLE_MYSTIC_GUNTLET_NORMAL);   break;
    case eWEAPONTYPE_POLEARM:               SetCurrentAttackStyle(eSTYLE_MYSTIC_POLEARM_NORMAL);   break;

#ifdef _YJ_FISHING_SYSTEM_000933
	case eWEAPONTYPE_FISHING_POLE:          RefreshPunchStyle();    break;
#endif //_YJ_FISHING_SYSTEM_000933
	case eWEAPONTYPE_PUNCH:                 RefreshPunchStyle();    break;
	}
}


//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentIdleAnimation()
{
	if(IsPolymorph())
	{
		return GlobalFunc::GetAnimationByPolymorph(eANI_IDLE);
	}

	// 특수 자세
	if(IsRiding())
	{
		//맵핑시켜준다.
		if(m_dwStandAnimDelay) 
		{
            m_bIdleFlag = (rand()%2==1);

			m_btIdleAni += 1;
			if(2 < m_btIdleAni)
				m_btIdleAni = 0;
		}
		else
		{
            return GlobalFunc::GetPlayerAnimationByRiding((m_bIdleFlag) ? (eANI_IDLE):(eANI_IDLE2),GetRiderKindCode());
		}

		return GlobalFunc::GetPlayerAnimationByRiding(eANI_STAND,GetRiderKindCode());
	}

    if (is_spread_wings())
    {
        if (m_dwStandAnimDelay)
        {
            return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_STAND);
        }
        else
        {
            return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_IDLE);
        }
    }

	bool is_idle_animation = false;

	if (IsFainting())
	{
		return StrToWzID("F004");
	}

#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
	m_IsCostumIdleAnimation = FALSE;
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION



	// 기본자세
	WzID animationID = 0;	

	char szAniCode[5];
	// 무기에따라서 기본자세가 바뀝니다.

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = '0';
	szAniCode[2] = '0';
	szAniCode[3] = '1';
	szAniCode[1] = GetWeaponAnimationCode();


	// 어택스탠드는 코드 끝이 3 기본은 1이다.
	if (m_iAttackStandTime > 0 )			
	{
		// 전투상태의 IDLE
		if (GetClass() == eCHAR_VALKYRIE) 
		{
			// 발키리 공격시는 Idle상태가 아니다 공격모션을 취해준다.
			if (m_iValkyrieShotDelay > 0) 
			{
				return StrToWzID("A201");
			}
			else
			{
				szAniCode[3] = '3';
			}
		}
		else
		{
			szAniCode[3] = '3';
		}
	}
	else 
	{

		// 비저누상태의 IDLE
		if (m_dwStandAnimDelay) 
		{
			szAniCode[3] = '1';		
			m_btIdleAni += 1;
			if(2 < m_btIdleAni)
				m_btIdleAni = 0;
		}
		else
		{
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
			if( IsTransForm() == FALSE )
			{
				SLOTCODE item_code = GetCostumeItemCode();
				if( item_code != 0 )
				{
					if( cCostumeItemInfo* pCostumeItemInfo = cCostumeItemInfoParser::Instance()->GetInfo( item_code ) )
					{
						m_IsCostumIdleAnimation = TRUE;
						return pCostumeItemInfo->GetIdleAnimation( GetGender() == GenderType::kMale );
					}
				}
			}
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION

			if( 2 > m_btIdleAni)
			{
				szAniCode[3] = '2';
			}
			else
			{

				if ((eCHAR_SHADOW == GetClass()) ||   //_YJ_SHADOW_SKILL_S00013
                    (eCHAR_HELLROID == GetClass()) || //_NA_000000_20130812_NEW_CHARACTER_HELLROID
					(eCHAR_MYSTIC == GetClass()) ||   //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                    (eCHAR_WITCHBLADE == GetClass())) //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
				{
					szAniCode[2] = '5';
					szAniCode[3] = '2';
				}
				else
				{
					szAniCode[3] = '2';
				}
			}
		}
	}





	animationID = StrToWzID(szAniCode);

	return animationID;
}

WzID  Player::GetCurrentReviveAnimation()
{
	WzID animationID = 0;	
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'R';
	szAniCode[1] = '0';
	szAniCode[2] = '0';
	szAniCode[3] = '1';

	animationID = StrToWzID(szAniCode);
	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentMoveAnimation()
{
	if(IsPolymorph())
	{
		return GlobalFunc::GetAnimationByPolymorph(eANI_RUN);
	}
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_RUN, GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_RUN);
    }

	WzID animationID = 0;
	// 무기에따라서 기본자세가 바뀝니다.

    if (GetHideSkill())
    {
        // 하이드 상태의 애니코드
        if (GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            // 무기가 대거일때의 애니코드
            animationID = StrToWzID("S721");
        }
        else
        {
            animationID = StrToWzID("S007");
        }
    }
    else
    {
        char szAniCode[5];
        ZeroMemory(szAniCode,sizeof(szAniCode));

        szAniCode[0] = 'N';
        szAniCode[1] = GetWeaponAnimationCode();
        szAniCode[2] = '0';
        szAniCode[3] = '7';
        
        animationID = StrToWzID(szAniCode);
    }

	return animationID;	
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentBlockAnimation()
{
	WzID animationID = 0;	
	// 무기에따라서 기본자세가 바뀝니다.

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'M';
	szAniCode[1] = '0';
	szAniCode[2] = '0';
	szAniCode[3] = '1';
	szAniCode[1] = GetWeaponAnimationCode();

	animationID = StrToWzID(szAniCode);

	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentDeathAnimation()
{
	TCHAR szWzID[4] = {0,};
	szWzID[0] = 'D';
	szWzID[1] = GetWeaponAnimationCode();
	szWzID[2] = '0';
	int iRan = rand()%3;
	if(0 == iRan)
		szWzID[3] = '1';
	else if(1 == iRan)
		szWzID[3] = '2';
	else if(2 == iRan)
		szWzID[3] = '3';

	WzID aniID = StrToWzID(szWzID);

	CWzUnit* pUnit=GetBasePart()->GetWzUnit();
	if(!pUnit)
	{
		szWzID[3] = '1';
		aniID = StrToWzID(szWzID);
		return aniID;
	}

	WzAnimationInfo *pAnimationInfo =pUnit->GetAnimationInfo(aniID);
	if(!pAnimationInfo)
	{
		szWzID[3] = '1';
		aniID = StrToWzID(szWzID);
		return aniID;
	}

	if(pAnimationInfo->m_wiAction != aniID)
	{
		szWzID[3] = '1';
		aniID = StrToWzID(szWzID);
		return aniID;
	}

	return aniID;
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical)
{
	WzID animationID = 0;	


	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'H';
	szAniCode[1] = GetWeaponAnimationCode();

	if (!bRight)
	{
		szAniCode[2] = 'R';
	}
	else
	{
		szAniCode[2] = 'L';
	}

	if (bCritical)
	{
		szAniCode[3] = '2';
	}
	else
	{
		szAniCode[3] = '1';
	}

	animationID = StrToWzID(szAniCode);

	return animationID;
}

//------------------------------------------------------------------------------
/**
앉기 에니를 얻는다.
*/
WzID Player::GetCurrentSitAnimation()
{
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
  #ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    if (IsInSpaTile_Change_SitIdle())
    {
        WzID animation_id = 0;
        SpaZoneInfo* spa_info = SpaZoneInfoParser::Instance()->GetSpaZoneInfo(g_pMap->GetMapID());
        if (spa_info && (spa_info->special_animation[0].id != 0))
        {
            animation_id = spa_info->special_animation[0].id;

            // 대기시간 부여
            SetSpaSittingAnimationDelay();
            return animation_id;
        }
    }
  #else
    if (IsInSpaTile())
    {
        // 대기시간 부여
        SetSpaSittingAnimationDelay();
        return StrToWzID("N021");
    }
  #endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_SIT,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        //! 애니 없음
        //return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_SIT);
    }

	WzID animationID = 0;	

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '0';	
	szAniCode[3] = '4';

	animationID = StrToWzID(szAniCode);

	return animationID;
}

//------------------------------------------------------------------------------
/**
앉기 유지 에니를 얻는다.
*/
WzID Player::GetCurrentSittingAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_SIT_IDLE,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        // 애니 없음
        //return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_SIT_IDLE);
    }

	WzID animationID = 0;	

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '0';
	szAniCode[3] = '5';

	animationID = StrToWzID(szAniCode);

	return animationID;
}


//------------------------------------------------------------------------------
/**
일어서기 에니를 얻는다.
*/
WzID Player::GetCurrentStandUpAnimation()
{
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
  #ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    if (IsInSpaTile_Change_SitIdle())
    {
        WzID animation_id = 0;
        SpaZoneInfo* spa_info = SpaZoneInfoParser::Instance()->GetSpaZoneInfo(g_pMap->GetMapID());
        if (spa_info && (spa_info->special_animation[3].id != 0))
        {
            animation_id = spa_info->special_animation[3].id;

            return animation_id;
        }
    }
  #else
    if (IsInSpaTile())
    {
        return StrToWzID("N023");
    }
  #endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_GETUP,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        // 애니 없음
        //return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_GETUP);
    }

	WzID animationID = 0;	

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] =  GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '6';

	animationID = StrToWzID(szAniCode);

	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
void Player::CreateDamageEffect(BOOL bCritical,BOOL bRight,DWORD dwAttackObjId)
{
	Character::CreateDamageEffect(bCritical,bRight,dwAttackObjId);	
}

//------------------------------------------------------------------------------
/**
*/
void Player::SetOldMoveFlag(int iOldMoveFlag)
{
	m_iOldMoveFlag = iOldMoveFlag;
}

//------------------------------------------------------------------------------
/**
*/
int Player::GetOldMoveFlag()
{
	return m_iOldMoveFlag;
}

//------------------------------------------------------------------------------
/**
*/
void Player::SetOldKeyboardDirection(WzVector *pVector)
{
	m_vOldDirection = *pVector;
}

//------------------------------------------------------------------------------
/**
*/
WzVector Player::GetOldKeyboardDirection()
{
	return m_vOldDirection;
}

//------------------------------------------------------------------------------
/**
*/
void Player::SetCondition(BYTE byCondition)
{
	Character::SetCondition(byCondition);

	switch(byCondition)
	{
	case eCHAR_CONDITION_STANDUP:
		{
			if( GetCurState()==STATE::SIT )
			{
				SetStandUp(TRUE);
			}
		}
		break;

	case eCHAR_CONDITION_SITDOWN:
		{
			SetNextState(STATE::SIT,g_CurTime);		

#ifdef _NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
            if (g_pHero && IsHero() == true)
            {
                g_pHero->set_sitting_packet_delay(FALSE);
            }
#endif //_NA_008546_20151029_SITTING_PACKET_DELAY_POSITION_SYNC_BUG
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**  펫생성:생성시 플레이어 정보와 결합한다.
*/
BOOL Player::CreatePetSummon(WORD wPetIndex)
{	
	//safe_code:혹 server에서 BRD가 두번날라왔을때를 대비 
	if (this->IsPetReturn())
	{
		DeletePetSummon();
	}

    m_PetInfo.m_bPetSummoned = TRUE;

	sPet_Script_Info* pet_script_ptr =	PetInfoParser::Instance()->GetPetInfo(wPetIndex);

	if (!pet_script_ptr)
	{
		assert(!"이런 펫인덱스는 없습니다.");	 
		return FALSE;
	}
	
    if(GENERALPARAM->IsNet())
    {
	    m_PetInfo.m_dwPetObjKey = g_ObjectManager.GenerateKeyByPet(PET_OBJECT);
    }
    else
    {
        //! 싱글모드
        m_PetInfo.m_dwPetObjKey = g_ObjectManager.GenerateKeyAtSingleMode(PET_OBJECT);
    }

	m_PetInfo.m_wPetIndex = wPetIndex;
	
	Pet* pet_ptr = static_cast<Pet*>(g_ObjectManager.Add(m_PetInfo.m_dwPetObjKey, 
		                                                 PET_OBJECT,
		                                                 pet_script_ptr->m_MonsterCode,
		                                                 FALSE));

	if (!pet_ptr)
	{
		assert(!"펫오브젝트 생성불가");
		return FALSE;
	}

	pet_ptr->InitPetMonster(this->GetObjectKey(),wPetIndex);

    return TRUE;
}
//------------------------------------------------------------------------------
/** 펫제거
*/
void Player::DeletePetSummon()
{
	Pet* pet_info = static_cast<Pet*>(g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey));
	
	//safecode:펫이유효한지체크
    if (!pet_info)
    {
        return;
    }

	if (pet_info->IsKindOfObject(PET_OBJECT))
	{
		pet_info->DestroyPetEffect();

		g_ObjectManager.Delete(m_PetInfo.m_dwPetObjKey);

		m_PetInfo.m_bPetSummoned  = FALSE;
	}
}
//------------------------------------------------------------------------------
/**	 펫상태 설정(상태에따른 이펙트출력)
*/
void Player::SetPetEmotion(ePET_EFFECT eState)
{
	if(!m_PetInfo.m_bPetSummoned) return;

	Pet* pPet =   (Pet*)g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey);

	if(pPet && pPet->IsKindOfObject(PET_OBJECT))
	{
		pPet->CreateInstanceEffect(eState);
	}
}
//------------------------------------------------------------------------------
/**	 플레이어 펫업데이트
*/
void Player::SetPetAttributes(POSTYPE pos, BYTE bIsFullness, BYTE byValue)
{
	if (bIsFullness)
	{
		m_PetInfo.PLAYER_.m_byPetFullness = byValue;
	}
	else
	{
		m_PetInfo.PLAYER_.m_byPetIntimacy = byValue;
	}

	Pet* pPet =  (Pet*)g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey);

	if (pPet && pPet->IsKindOfObject(PET_OBJECT))
	{
		((bIsFullness) ? (pPet->SetFullness(byValue)) : (pPet->SetIntimacy(byValue)));

		pPet->UpdatePetAttributes(FALSE);

	}


}

//------------------------------------------------------------------------------
/**	 플레이어 펫업데이트
*/
void Player::SetPetName(TCHAR* pszName)
{
	if (!pszName)
    {
        return;
    }

	StrnCopy(m_PetInfo.PLAYER_.m_sPetName ,pszName, MAX_PET_NAME_LENGTH);
	
	Pet* pPet = (Pet*)g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey);

	//safecode:펫이유효한지체크
	if (pPet && pPet->IsKindOfObject(PET_OBJECT))
	{
		pPet->RefreshPetName();
	}
}





//------------------------------------------------------------------------------
/**  하이드상태일때 파티정보에 따라서 정보갱신
*/
void Player::RefreshHideState(BOOL bClear)
{
	// 자신은 패스
	if( IsHero() )
		return;

    //현재 쉐도우만 체크
    if( GetClass() != eCHAR_SHADOW )
    {
        return;
    }

    // 은신중인경우에만 
    if( m_sHideInfo.NONE_HIDE != m_sHideInfo.byHideState )
    {
        BOOL IsHalfHide = FALSE;

        if( uiPartyMan* partyMan = static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY)) )
        {
            if( partyMan->IsPartyMember( GetObjectKey() ) == true )
                IsHalfHide = TRUE;
        }

        if( bClear )
        {
            IsHalfHide = FALSE;
        }

		// 히어로가 디텍팅 스킬을 가지고 있으면 모든 은신체를 반투명하게 표시한다
        if( g_pHero->IsDetectingHide() == TRUE )
        {
            IsHalfHide = TRUE;
        }

		// 플레이어가 다크오브라이트 스킬에 걸려있다면, 그 은신체는 반투명하게 표시한다
		if( IsDarkofLightZoneState() == TRUE )
		{
			IsHalfHide = TRUE;
		}

        if( IsHalfHide == TRUE )
        {
            m_sHideInfo.byHideState;
            m_sHideInfo.byHideState=m_sHideInfo.HALF_HIDE;
            SetShow(TRUE);
            SetShadowDraw(FALSE);
        }
        else
        {
            m_sHideInfo.byHideState=m_sHideInfo.ALL_HIDE;
            SetShow(FALSE);
        }
    }


    if(IsPetReturn())
    {
        if( Pet* pPet = (Pet*) g_ObjectManager.GetObject(GetPetObjectKey()) )
        {
            pPet->SetStatePetHide(m_sHideInfo.byHideState);
        }
    }
}
//------------------------------------------------------------------------------
/**  하이드 조건(파티 or 점령전에서 길드)
*/
BYTE Player::GetHideCondition()
{
    //0.기본적의로 Hero만반투명
    BYTE byHideState  = IsHero() ? (m_sHideInfo.HALF_HIDE):(m_sHideInfo.ALL_HIDE);

    //1.파티멤버인지체크(파티멤버의경우 반투명)
    uiPartyMan* partyMan = static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    
    if (partyMan && partyMan->IsPartyMember(GetObjectKey()))
    {
        byHideState = m_sHideInfo.HALF_HIDE;
    }
    
    
    //2.점령전씬에서 같은길드인지(반투명)
    if(SCENE_TYPE_DOMINATION == GameFramework::GetCurScene()
#ifdef _NA_008334_20150608_SONNENSCHEIN
        || SCENE_TYPE_SONNENSCHEIN == GameFramework::GetCurScene()
#endif //_NA_008334_20150608_SONNENSCHEIN
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        || SCENE_TYPE_GUILDMISSION == GameFramework::GetCurScene()
#endif //_NA_008543_20151027_ONE_DAYS_GUILD_MISSION
        )
    {
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
        if (GuildSystem::Function::IsGuildMember(GetGuildName()) == true)
        {
            byHideState = m_sHideInfo.HALF_HIDE;
        }
#else
        uiGuildMan* pGuildMan = (uiGuildMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUILD);

        if(pGuildMan && pGuildMan->IsGuildMember(GetGuildName()))
        {
            byHideState = m_sHideInfo.HALF_HIDE;
        }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    else if (GlobalFunc::IsBattleGroundScene() == TRUE || 
             GlobalFunc::IsGoldRushScene() == TRUE)
#else
    else if (GlobalFunc::IsBattleGroundScene() == TRUE)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    {
        // 같은 팀이면 반투명화
        if (IsHero() == false)
        {
            if (this->GetTeam() == g_pHero->GetTeam())
            {
                byHideState = m_sHideInfo.HALF_HIDE;
            }
        }
        
        return byHideState; // 아래 레벨에 따른 보임은 어떻게 할까.. 일단 그냥 리턴
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    //3.같은쉐도우의 경우 캐릭레벨에따라상태를변경한다.
    if( IsHero() == false )
    {
		if( GetClass() == eCHAR_SHADOW )
		{
			if( g_pHero->GetClass() == eCHAR_SHADOW && GetLevel()<=g_pHero->GetLevel())
			{
				byHideState = m_sHideInfo.HALF_HIDE;
			}
		}
    }

    return byHideState;
}
//------------------------------------------------------------------------------
/**  캐릭터를 하이드시킨다
	 0:비하이드상태   1:투명상태   2:반투명상태(일단 주인공일때)
*/
void Player::SetHideSkill(BOOL bFlag)
{	
    if( (GetClass()!=eCHAR_SHADOW && GetClass() != eCHAR_MYSTIC) && bFlag )
    {
        //safecode: 쉐도우가 아닌캐릭에게 하이드가 왔다
        assert(!"비정상하이드정보:SetHideSkill");
        return;
    }

	if(bFlag)
	{
		m_sHideInfo.byHideState = GetHideCondition();

        if ( GetClass() == eCHAR_MYSTIC )
        {
            m_sHideInfo.byHideState = m_sHideInfo.ALL_HIDE;
        }

		if(m_sHideInfo.byHideState==m_sHideInfo.HALF_HIDE)
		{
			//그림자없음	
			SetShadowDraw(FALSE);
		}
		else
		{
			SetShow(FALSE);
			
            //주인공이 this 타켓팅하고있다면 풀자
			if(g_HeroInput.GetCurrentTarget()==GetObjectKey())
            {
				g_HeroInput.SetCurrentTarget(0);
            }
		}
	}
	else
	{
		SetShadowDraw(TRUE);
		
        m_sHideInfo.byHideReserve = FALSE;
		m_sHideInfo.byHideState = m_sHideInfo.NONE_HIDE;	

		if(!IsObserverMode())
		{
			//연출
			if(!GetShow() || m_sHideInfo.byHideState==m_sHideInfo.HALF_HIDE)
			{
				SetFadeIn(1500);
			}

            SetShow(TRUE);
		}
	}
    
    UpdateItemEffectPos(this); // SetShow() 후 위치 갱신

	Pet* pPet = NULL;
	if(IsPetReturn())
	{
		pPet = (Pet*) g_ObjectManager.GetObject(GetPetObjectKey());
		if(pPet)
		{
			pPet->SetStatePetHide(m_sHideInfo.byHideState);
		}
	}
}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//------------------------------------------------------------------------------
/**  전장 투명모드 (기능은 하이드와 같다)
*/
void Player::SetBattleGroundHideMode(bool is_hide)
{	
    if (is_hide)
    {
        BattleGroundHideMode hide_mode = CheckBattleGroundHideMode(); // 투명모드인지 반투명모드인지 체크
        set_battle_ground_hide_mode(hide_mode);

        if (battle_ground_hide_mode() == BattleGroundHideMode_Half)
        {
            //그림자없음	
            SetShadowDraw(FALSE);
        }
        else
        {
            SetShow(FALSE);

            //주인공이 this 타켓팅하고있다면 풀자
            if(g_HeroInput.GetCurrentTarget() == GetObjectKey())
            {
                g_HeroInput.SetCurrentTarget(0);
            }
        }
    }
    else
    {
        SetShadowDraw(TRUE);

        set_battle_ground_hide_mode(BattleGroundHideMode_None);

        if(!IsObserverMode())
        {
            //연출
            if(!GetShow() || (battle_ground_hide_mode() == BattleGroundHideMode_Half))
            {
                SetFadeIn(1500);
            }

            SetShow(TRUE);
        }
    }

    Pet* pet_pointer = NULL;
    if (IsPetReturn())
    {
        pet_pointer = static_cast<Pet*>(g_ObjectManager.GetObject(GetPetObjectKey()));
        if (pet_pointer)
        {
            // 펫 투명화는 하이드를 써도 무관할 듯 하다. (플레이어 투명상태에 따라 적용되므로)
            BYTE hide_state = static_cast<BYTE>(battle_ground_hide_mode());
            pet_pointer->SetStatePetHide(hide_state);
        }
    }
}
//------------------------------------------------------------------------------
/**  전장 투명모드 체크 (하이드와 다른점은 해제스킬이 통하지 않는다는 점)
*/
BattleGroundHideMode Player::CheckBattleGroundHideMode()
{
    //0.기본적의로 Hero만반투명
    BattleGroundHideMode hide_mode = IsHero() ? BattleGroundHideMode_Half : BattleGroundHideMode_Full;
    
    //1.파티멤버인지체크(파티멤버의경우 반투명)
    //uiPartyMan* party_manager = static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

    //if (party_manager && party_manager->IsPartyMember(GetObjectKey()))
    //{
    //    hide_mode = BattleGroundHideMode_Half;
    //}

    //2.전장씬에서 같은 팀인지(반투명)
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    // 골드러쉬도 전장과 같이 하이드 상태 같이 사용
    if (GlobalFunc::IsBattleGroundScene() == TRUE || 
        GlobalFunc::IsGoldRushScene() == TRUE)    
#else
    if (GlobalFunc::IsBattleGroundScene() == TRUE)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    {
        // 같은 팀이면 반투명화
        if (IsHero() == false)
        {
            //if (this->GetTeam() == g_pHero->GetTeam())
            // this->GetTeam()으로 비교하기엔 아직 팀 키가 설정이 안된 상태임
            //
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            // 전장이 아니라면 골드러쉬이다.
            int player_team = GlobalFunc::IsBattleGroundScene() ? 
                ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(this->GetName()) : 
                ChaosSystem::GetGoldRushData().GetGoldRushTeamForPlayerName(this->GetName());
#else
            int player_team =
                ChaosSystem::GetBattleGroundData().GetBattleGroundTeamForPlayerName(this->GetName());
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            if (player_team == g_pHero->GetTeam())
            {
                hide_mode = BattleGroundHideMode_Half;
            }
        }
    }

    ////3.같은쉐도우의 경우 캐릭레벨에따라상태를변경한다.
    //if( IsHero() == false )
    //{
    //    if( GetClass() == eCHAR_SHADOW )
    //    {
    //        if( g_pHero->GetClass() == eCHAR_SHADOW && GetLevel()<=g_pHero->GetLevel())
    //        {
    //            byHideState = m_sHideInfo.HALF_HIDE;
    //        }
    //    }
    //}

    return hide_mode;
}
//------------------------------------------------------------------------------
/**  전장 투명모드 갱신
*/
void Player::RefreshBattleGroundHideMode()
{
    // 현재는 전장 전용
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    // 골드러쉬에 대한 내용을 추가
    if (GlobalFunc::IsBattleGroundScene() == FALSE && 
        GlobalFunc::IsGoldRushScene() == FALSE)
#else
    if (GlobalFunc::IsBattleGroundScene() == FALSE)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    {
        return;
    }

    // 자신은 패스
    if (IsHero())
    {
        return;
    }

    // 은신중인경우에만 
    if (battle_ground_hide_mode() != BattleGroundHideMode_None)
    {
        if (g_pHero && g_pHero->GetTeam() == this->GetTeam()) // 같은 팀이면
        {
            set_battle_ground_hide_mode(BattleGroundHideMode_Half);
            SetShow(TRUE);
            SetShadowDraw(FALSE);
        }
        else
        {
            set_battle_ground_hide_mode(BattleGroundHideMode_Full);
            SetShow(FALSE);
        }
    }

    if (IsPetReturn())
    {
        Pet* pet = static_cast<Pet*>(g_ObjectManager.GetObject(GetPetObjectKey()));
        if (pet)
        {
            BYTE hide_mode = static_cast<BYTE>(battle_ground_hide_mode());
            pet->SetStatePetHide(hide_mode);
        }
    }
}

#endif _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
//------------------------------------------------------------------------------
/**	하이드 이펙트(start)
*/
void Player::StartExtraEffect()
{
    //반투명상태만적용
    if ((m_sHideInfo.byHideState == m_sHideInfo.HALF_HIDE) ||
        (battle_ground_hide_mode() == BattleGroundHideMode_Half))
    {

        //자신이 하이드시엔 굴절효과
        CWzUnitDraw *pParts = GetBasePart();

        if(pParts)
        {
            m_sHideInfo.eBlendMode	    = g_pSunRenderer->GetAlphaBlendMode();	 
            m_sHideInfo.eRenderType	    = pParts->GetRenderType();	 
            m_sHideInfo.fRefractAlpha   = pParts->GetRefractionAlpha();	 
            m_sHideInfo.fRefractRate	= pParts->GetRefractionRate();	 

            pParts->SetRefractionRate(0.1f);		
            pParts->SetRefractionAlpha(0.1f);
            pParts->SetRenderType(AR_REFRACTION_CHAOS);

            WzColor color = GetColor();

            int iAlpha= 100;

            if(m_bCameraAlpha && m_fAlpha<100.f)
            {
                iAlpha = (int)m_fAlpha;
            }

            SetColor(Red_WzColor(color),Green_WzColor(color),Blue_WzColor(color),iAlpha);
            g_pSunRenderer->SetAlphaBlendMode( AB_LIGHTING, FALSE );

        }
    }
}

//------------------------------------------------------------------------------
/**	하이드 이펙트(end)
*/
void Player::EndExtraEffect()
{
    //반투명상태만적용
    if ((m_sHideInfo.byHideState == m_sHideInfo.HALF_HIDE) ||
        (battle_ground_hide_mode() == BattleGroundHideMode_Half))
    {
        //본상태로 다시 되돌린다.
        CWzUnitDraw *pParts = GetBasePart();

        if(pParts)
        {
            pParts->SetRefractionRate(m_sHideInfo.fRefractRate);		
            pParts->SetRefractionAlpha(m_sHideInfo.fRefractAlpha);
            pParts->SetRenderType(m_sHideInfo.eRenderType);
            g_pSunRenderer->SetAlphaBlendMode( m_sHideInfo.eBlendMode, FALSE );
        }
    }
}

//------------------------------------------------------------------------------
/**	클래스 체크(러쉬는 오직 쉐도우만)
*/
void Player::ProcessRushClone()
{
	if(this->GetClass()!=eCHAR_SHADOW || m_sHideInfo.byHideState) return;
	
	BOOL bRet = FALSE; 

	switch(GetCurState()) 
	{
	case STATE::IDLE:
	case STATE::DEATH:
	case STATE::SKILL:
		return;
	}

    std::vector<SKILL_EFFECT*>& array = GetSkillEffectArray();

    for( size_t i=0;i!=array.size();++i)
    {
        SKILL_EFFECT* pEffect = array[i];

        if (pEffect->dwStatusID == eCHAR_STATE_FETTER)  
            return;

        if (pEffect->dwStatusID == eCHAR_STATE_RUSH)
        {
            //러쉬잔상들을 만들어낸다.
            static DWORD dwCopytime = 0;

            //150단위로 생성
            if(g_CurTime-dwCopytime>240)
            {
                CloneEffect(400,1000,1);	
                dwCopytime = g_CurTime;
            }
            return;
        }
    }
}


//==================================================
// 0905패킷변경 후
//==================================================
//------------------------------------------------------------------------------
/**  펫 정보 셋팅
*/
void Player::SetPlayerPetInfo( PLAYER_RENDER_INFO_EX *pPlayerInfo)
{
    if (!pPlayerInfo)
    {
        return;
    }

	//소환상태라면 생성한다
	if (pPlayerInfo->is_summoned_pet)
	{
		CreatePetSummon(pPlayerInfo->pet_index);
		SetPetAttributes(0,TRUE,pPlayerInfo->pet_fullness);	//포만감
		SetPetAttributes(0,FALSE,pPlayerInfo->pet_intimacy);	//친밀도

		ZeroMemory(m_PetInfo.PLAYER_.m_sPetName, sizeof(m_PetInfo.PLAYER_.m_sPetName));

		//펫네임이 존재한다
		if (pPlayerInfo->pet_name[0] != '\0')
		{
			SetPetNameUse(1);				//펫네임있음
			SetPetName(pPlayerInfo->pet_name);
		}
		else
		{
			SetPetNameUse(0);				//펫네임없음
		}
	}

    m_PetInfo.m_bPetSummoned = pPlayerInfo->is_summoned_pet;
}
//------------------------------------------------------------------------------
/** 플레이어 랜더정보
*/
void Player::SetPlayerInfo(PLAYER_RENDER_INFO_EX* pPlayerInfo)
{
	ZeroMemory(GetCharInfo(),sizeof(BASE_PLAYERINFO)); 

    DelayLoadManager::Instance()->InsertPlayer(GetObjectKey());

	GetCharInfo()->m_byClassCode	= pPlayerInfo->char_class;
    GetCharInfo()->gender	= pPlayerInfo->gender;

	if( pPlayerInfo->GetChangeOfClassSteps() == eSPECIAL_RENDER_EFFECT_CHANGEofCLASS_1st )
		GetCharInfo()->SetChangeOfClass( (eCHAREX_TYPE) (pPlayerInfo->char_class +10) );
	else
		GetCharInfo()->SetChangeOfClass( (eCHAREX_TYPE) pPlayerInfo->char_class);

	
	GetCharInfo()->m_LV				= pPlayerInfo->level;
	GetCharInfo()->m_iHP 			= pPlayerInfo->hp;
	GetCharInfo()->m_iMaxHP			= pPlayerInfo->max_hp;
    SetCurrentAttackStyle( pPlayerInfo->selected_style_code );
	m_wAttSpeedRatio	            = pPlayerInfo->attack_speed_ratio;
	m_bHPInfo = TRUE;
	if(TRUE == pPlayerInfo->collection_render_info.m_bUseCollectRanderInfo)
	{
        DWORD collect_progress_time = 0; // 채집 진행시간 (패킷은 남은시간으로 옴)
        
        const CollectionInfo* collection_info =
            CollectionParser::Instance()->FindCollectionInfo(pPlayerInfo->collection_render_info.m_CollectionID);
        if (collection_info)
        {
            collect_progress_time =
                collection_info->m_dwCollectingTime - pPlayerInfo->collection_render_info.m_dwRemainTime;
        }

		DoCollect(pPlayerInfo->collection_render_info.m_dwObjectKey
			, pPlayerInfo->collection_render_info.m_CollectionID
			, collect_progress_time);
	}
	
	SetRindingState(pPlayerInfo->is_riding);

	if (m_pRiderContainer && pPlayerInfo->is_riding)
	{
       //라이더 코드를복사한다.
       SCItemSlot ItemSlot;
       ItemSlot.Copy(pPlayerInfo->rider_item_slot);
       DWORD dwRiderCode = m_pRiderContainer->CopyItem(ItemSlot);
       
       if(dwRiderCode!=0)
       {
           CreateRider(dwRiderCode);
       }
	}

    if (rider() != NULL)
    {
        set_spread_wings(false);
        rider()->set_spread(pPlayerInfo->spread_wings != 0);
    }
    else
    {
        set_spread_wings((pPlayerInfo->spread_wings != 0));
    }

	SetName(pPlayerInfo->char_name);
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    SetTitleName(pPlayerInfo->honor_title);
#endif //_NA_003027_20111013_HONOR_SYSTEM
	m_wMoveSpeedRatio               = pPlayerInfo->move_speed_ratio;
	m_bAttackBeginPlayer			= pPlayerInfo->boradori;
	GetCharInfo()->m_ChaoState		= pPlayerInfo->chao_state;
	m_EtherIndex					= pPlayerInfo->ether_device_index;

	SetGMGrade(pPlayerInfo->gm_grade);
	SetBeHaveState((ePLAYER_BEHAVE_STATE)pPlayerInfo->behave_state);

	SetPCBangMode(pPlayerInfo->pcroom);
	
	SetHP( GetCharInfo()->m_iHP );
	SetCondition(pPlayerInfo->char_condition);
	BOOL	bIsHero = FALSE;
	if( g_pHero )
	{
		bIsHero = ( g_pHero->GetObjectKey() == this->GetObjectKey() );
            uiGuideSystemMan* guide_manager =
            static_cast<uiGuideSystemMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_GUIDE_SYSTEM));

        if (bIsHero && guide_manager)
        {
            //eCHAO_STATE_NORMAL:일반, eCHAO_STATE_PREV_CHAO:프리 카오, eCHAO_STATE_CHAO:카오
            switch (pPlayerInfo->chao_state)
            {
            case eCHAO_STATE_PREV_CHAO:
                guide_manager->CheckHelpMessage(TotalGuideNotice::kFreeChaos);
                break;
            case eCHAO_STATE_CHAO:
                guide_manager->CheckHelpMessage(TotalGuideNotice::kChaos);
                break;
            }
        }
	}


	//Todo:여기서 체크를하자 Player의 투명정보를
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    switch (pPlayerInfo->transparent)
    {
    case 3:
        {
            SetBattleGroundHideMode(true);
        }
        break;
    case 2:
        {
#ifdef	_DEV_VER
            //safecode: 쉐도우가 아닌캐릭에게 하이드가 왔다
            if(this->GetClass()!=eCHAR_SHADOW)
            {
                assert(!"비정상 하이드정보:SetPlayerInfo");
            }
#endif//_DEV_VER
            SetHideSkill(TRUE);
        }
        break;
    default:
        {
            this->SetObserverMode( pPlayerInfo->transparent, bIsHero );
        }
        break;
    }
#else //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
	if (pPlayerInfo->transparent==2)
	{
#ifdef	_DEV_VER
        //safecode: 쉐도우가 아닌캐릭에게 하이드가 왔다
        if(this->GetClass()!=eCHAR_SHADOW)
        {
            assert(!"비정상 하이드정보:SetPlayerInfo");
        }
#endif//_DEV_VER
		SetHideSkill(TRUE);
	}
	else
	{
		this->SetObserverMode( pPlayerInfo->transparent, bIsHero );
	}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	//m_pPlayerAttriburtes->SetCharInfo( GetCharInfo(),0,0,0);

	SetPartVariation(PLAYER_VARIATION_HAIR,pPlayerInfo->hair_style);
	SetPartVariation(PLAYER_VARIATION_FACE,pPlayerInfo->face);
	SetHeightVariation(pPlayerInfo->height);

	SetStateTotalInfo(pPlayerInfo->state_total_info);

	SetPlayerPetInfo(pPlayerInfo);

#ifdef _NA_004035_20120227_COSTUME_ITEM
    SetCostumeItemCode( pPlayerInfo->is_equipped_costume ? pPlayerInfo->costume_item_code : 0 );
#endif//_NA_004035_20120227_COSTUME_ITEM

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	SetDecorateCostumeItemCode( pPlayerInfo->is_equipped_decorate_costume ? pPlayerInfo->decorate_costume_item_code : 0);
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
    SetSpaItemCode(pPlayerInfo->spa_ticket_code);
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

#ifdef _NA_000000_20130206_BROADCAST_ITEM_SCORE
    m_ItemScore = (int)pPlayerInfo->total_item_score_;
#endif // _NA_000000_20130206_BROADCAST_ITEM_SCORE
}


void Player::DoCollect(DWORD dwTargetKey, DWORD dwCollectionID, DWORD dwTime)
{
	PLAYER_ACTION action;
	ZeroMemory(&action,sizeof(action));
	action.ActionID = ACTION_COLLECTION_OBJECT;
	action.COLLECTOBJECT.dwTargetID = dwTargetKey;
    
    Object* map_object = g_ObjectManager.GetObject(dwTargetKey);
    if (map_object != NULL)
    {
        this->SetDirection(map_object->GetVisiblePos());
    }

	SetCollectObjKey(dwTargetKey);
	SetCollectID(dwCollectionID);
	SetCollectStartTime(dwTime);
	DoAction(&action);

#ifdef _DEV_VER
	GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, _T("Player::DoCollect() %s,%d"), GetName(), dwTime);
#endif 
}

//------------------------------------------------------------------------------
/**
*/
void Player::ValkyrieShot(DWORD dwAttackSerial)
{
	HideAttachment(FALSE);
	SetAttackStandTime(ATTACK_STAND_TIME);

	m_fValkyrieShotSpeed = GetAttackSpeedModifier();

	SetCurrentBlendAnimation(GetValkyrieShotAnimation(),GetShotBlendBone(),m_fValkyrieShotSpeed,0);
	OpenCrossbow();

	// 이전 공격에 대한 처리를 확실히 해준다.
	if (m_dwValkyrieShotSerial)	
	{
		ProcessAttackResult(m_dwValkyrieShotSerial,TRUE);
		m_dwValkyrieShotSerial = 0;
	}

	WzAnimationInfo *pAniInfo = NULL;
    if (GetBasePart()->GetWzUnit() != NULL)
    {
        pAniInfo = GetBasePart()->GetWzUnit()->GetAnimationInfo(GetValkyrieShotAnimation());
    }

	m_dwValkyrieShotSerial = dwAttackSerial;

	if( pAniInfo )
	{
		m_iValkyrieShotDelay = (int)((float)pAniInfo->m_nMaxFrame * 33.0f / m_fValkyrieShotSpeed);
	}
	else
	{
		m_iValkyrieShotDelay = 500;	// 총들고 있는 시간.
	}
	m_iValkyrieShotDelayTime = m_iValkyrieShotDelay;
	m_iValkyrieShotCount = 0;
	m_iValkyrieShotRemain = 2;

	m_bValkyrieReload = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Player::CanValkyrieAction()
{
	// 사격 가능상태!
	if( g_HeroInput.IsIdleState() || GetCurState() == STATE::KEYBOARDMOVE || GetCurState() == STATE::MOVE )
	{
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/** 펫 갱신
*/
void Player::RefreshAppearPet()
{
    if (IsPetReturn() == FALSE)
    {   
        return;
    }

    Pet* pet_info = static_cast<Pet*>(g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey));

    if (pet_info)
    {
        pet_info->RefreshApperPet(this);
    }
}
//------------------------------------------------------------------------------
/** 라이더 갱신
*/
void Player::RefreshAppearRider()
{
    if (IsRiding() == FALSE)
    {
        return;
    }

    //라이더 몸체가 할당되면 파츠도할당한다
    if (m_pRider->RefreshApperRider(this))
    {
        RefreshRider();
    }
}
//------------------------------------------------------------------------------
/**
*/
BOOL Player::Process(DWORD dwTick)
{
	g_dwCharProcessCount++;

	if (GENERALPARAM->GetSpecialMode() && GENERALPARAM->IsNet()==FALSE && g_bPlayerAutoMove)
	{
		if((g_pHero != this) && (this->GetCurState() == STATE::IDLE) && (this->IsMove() == FALSE) && ((rand()%700) == 1))
		{
			float fAngle = (rand() % 628) / 100.0f;
			ClampAngle(&fAngle);
			WzVector vDirection;
			vDirection.x = sin(fAngle);
			vDirection.y = -cos(fAngle);
			vDirection.z = 0.0f;

			float fRate=5.0f;
			fRate+=rand()%30;

			vDirection*=fRate;
			WzVector vPos=g_pHero->GetPosition()+vDirection;

			vPos.z+=3.0;

			int iTileFocussing = -1;
			float fT=0.0f;
			iTileFocussing = g_pSunTerrain->x_pPathFinder->GetTileToStand( vPos, &fT, -1.0f, 10.0f );

			if(iTileFocussing >= 0)
			{
				vPos.z-=fT;
				PLAYER_ACTION action;
				ZeroMemory(&action,sizeof(action));
				action.ActionID = ACTION_MOVE;
				action.MOVE.byState =CMS_RUN ;
				action.MOVE.bThrust = FALSE;
				action.MOVE.vCurPos.wvPos = this->GetPosition();
				action.MOVE.vCurPos.wTile = -1;
				action.MOVE.vDestPos.wvPos = vPos;
				action.MOVE.vDestPos.wTile = -1;
				PutAction(&action);	

			}
		}
	}

	BeforeRefreshTileAtt();

	if (GENERALPARAM->GetSpecialMode())
	{
		int y=0;
		if(this->GetChaoState()==eCHAO_STATE_PREV_CHAO)
		{

			y+=1;
			RenderTextOnHead(_T("프리카오"),m_RenderinfoColor,y,TRUE);

		}

		if(this->IsAttackBeginPlayer())
		{

			y+=1;
			RenderTextOnHead(_T("보라돌이"),m_RenderinfoColor,y,TRUE);
		}


		if(this->GetChaoState()==eCHAO_STATE_CHAO)
		{

			y+=1;
			RenderTextOnHead(_T("카오"),m_RenderinfoColor,y,TRUE);

		}
	}

	if (GetClass() == eCHAR_VALKYRIE)
	{
		ProcessValkyrieShot(dwTick);
	}

	//시간이 지나면 자동으로 꺼준다.
	if(m_bInvincible && m_iInvicibleTime)
	{
		m_iInvicibleTime -= dwTick;

		if(m_iInvicibleTime <= 0)
		{
			m_iInvicibleTime=0;
			m_bInvincible=FALSE;
		}
	}

	m_dwShieldTime-=dwTick;
	m_dwShieldTime=max(m_dwShieldTime,0);

	// 이동중일때는 0.5초마다 먼지가나온다.
	// 키보드이동시에는 회전은 제외
	// 0x00000033은 회전을 제외한 모든 키보드 이동 flag임
	if ( GetCurState() == STATE::MOVE || 
		(GetCurState() == STATE::KEYBOARDMOVE && GetMoveFlag() & KEYBOARDMOVE_ISMOVING) ) 
	{
		m_dwDustTime += dwTick;
	}
	else
	{
		m_dwDustTime = 0;
	}

	if (m_dwDustTime >= 500)
	{
		m_dwDustTime = 0;
	}

	BOOL bRet = Character::Process(dwTick);

	ProcessSummonIntensifyEffect();

	AfterRefreshTileAtt();

    // 이펙트 높이 설정 - 이동 및 점프시
	if (false == domination_effect_list_.empty())
	{
        // 높이값 계산
        WzVector world_pos;
        WzMatrix position_matrix, rotation_matrix, temp_matrix;
        WzVector picked_position;
        float rotation_angle = 0.0f;
        GameFunc::PickTerrainMesh(GetVisiblePos(), picked_position, TMGO_NO_COLLISION, 0.5f);
        MatrixIdentity(&temp_matrix);														
        VectorTransformCoord(&world_pos, &picked_position, &temp_matrix);		
        MatrixTranslation(&position_matrix, world_pos.x, world_pos.y, world_pos.z);

		BYTE hide_condition = GetHideSkill();
		bool is_show_effect = (GetShow() &&
                               IsShowByGameOption() &&
                               hide_condition != HIDE_SKILL_INFO::ALL_HIDE &&
                               BattleScene::GetScreenShotState() != eSCREEN_SHOT_NOT_UI_AND_HERO);
		std::list<HANDLE>::iterator itr = domination_effect_list_.begin();
		std::list<HANDLE>::iterator itr_end = domination_effect_list_.end();
		CEffectBody* effectbody = NULL;
		for ( ; itr != itr_end; ++itr)
		{
			if (INVALID_HANDLE_VALUE != (*itr))
			{
				effectbody = g_pEffectManager->GetEffectPtrHandle((*itr));
				if (INVALID_HANDLE_VALUE != effectbody && NULL != effectbody)
				{
					WzID effect_id = effectbody->GetBodyID();
                    if (effect_id == StrToWzID("I710"))
                    {
                        // 각 이팩트르 35도 간격으로 돌림
                        rotation_angle += PI * (35.0f / 180.0f);
                        MatrixRotationZ(&rotation_matrix, rotation_angle);
                    }
                    else
                    {
                        MatrixIdentity(&rotation_matrix);
                    }
					effectbody->SetShow(is_show_effect);
					effectbody->SetResultMatrix(&position_matrix, &rotation_matrix, NULL);
				}
			}
		}
	}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
	UpdatePlayerRankingEffect();
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

	return bRet;
}

//------------------------------------------------------------------------------
/**
*/
int Player::DoAction(PLAYER_ACTION *pAction)
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


	case ACTION_KEYBOARDJUMP:
		{
#ifdef _NA_20120220_JUMP_RENEWAL
            GameFunc::PickTerrainMesh(pAction->KEYBAORDJUMP.vDest,m_JumpData.m_vJumpTargetPosition);
            m_JumpData.m_JumpFlag.SetMoveFlag( pAction->KEYBAORDJUMP.iJumpFlag );
            m_JumpData.m_vJumpStartPosition = GetVisiblePos();
#else
            GameFunc::PickTerrainMesh(pAction->KEYBAORDJUMP.vDest, m_JumpData.m_JumpTargetPosition );

            m_JumpData.m_JumpStartPosition = GetVisiblePos();
#endif

			StopMove();	
			SetNextState(STATE::KEYBOARDJUMP,g_CurTime);
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

	case ACTION_TELEPORT:
		{

			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("k470"),GetVisiblePos(),GetObjectKey());

			StopMove();			
			WzVector vPos=pAction->TELEPORT.vDest;
			SetDestWarp(vPos);//워프할 좌표를 세팅 한뒤 텔레포트 스테이트 종료시 빼자 
			SetPosition(vPos);
			SetDirection( vPos, TRUE);
		}
		break;

	case ACTION_KNOCKBACK:
		{
			StopMove();		
			SetNextState(STATE::DAMAGE,g_CurTime);	
			return 1;
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
			return 1;
		}
		break;

	case ACTION_DOWN:
		{
			StopMove();	
			SetNextState(STATE::DOWN,g_CurTime);
			return 1;
		}
		break;

	case ACTION_GETUP:
		{
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
			SetNextState(STATE::NPC_RESPONSE_STATE, g_CurTime);
			return 1;
		}
		break;

	case ACTION_VENDORMEET:
		{
			
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
				SetNextState(STATE::SUMMON_RIDER, g_CurTime);
			}
			return 1;
		}
		break;

    case ACTION_ETHERIA_EXTRACT:
        {
            if (STATE::ETHERIA_EXTRACT != GetCurState())
            {
                StopMove();
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
        break;
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
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	case ACTION_STATUE_STAND:
		{
			if (STATE::STATUE_STAND != GetCurState())
			{
				StopMove();
				SetNextState(STATE::STATUE_STAND, g_CurTime);
			}
			return 1;
		}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
	default:
		{
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
int Player::GetValkyrieShotDelay()
{
	return m_iValkyrieShotDelay;
}


//------------------------------------------------------------------------------
/**
*/
DWORD Player::GetCurrentAttackStyle()
{
	return GetCharInfo()->m_iSelectStyleCode;
}

//------------------------------------------------------------------------------
/**
*/
BOOL Player::CheckChangeAttackStyle( DWORD dwStyle )
{
    TCHAR	szMessage[INTERFACE_STRING_LENGTH];

    BASE_STYLEINFO* pInfo = SkillInfoParser::Instance()->GetStyleInfo((SLOTCODE)dwStyle);
    if(pInfo==NULL)
    {
        //	존재 하지 않는 스타일 입니다
        g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_STYLE, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return FALSE;
    }

    //캐릭터 체크
    BASE_PLAYERINFO* bp = GetCharInfo();
    if(bp==NULL)
    {
        //	스타일을 적용할 캐릭터 정보가 없습니다.
        g_InterfaceManager.GetInterfaceString( eST_NOT_CHARACTER_INFO_FOR_STYLE, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

        return FALSE;
    }

    if( IsTransForm() && GetTransformData().GetTransFormType() != pInfo->m_byClassDefine )
    {
        g_InterfaceManager.GetInterfaceString( eST_NOT_APPLY_STYLE, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

        return FALSE;

    }
    else if(bp->m_byClassCode!=pInfo->m_byClassDefine)
    {
        //	해당 캐릭터에 적용할 수 없는 스타일입니다.
        g_InterfaceManager.GetInterfaceString( eST_NOT_APPLY_STYLE, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

        return FALSE;
    }

    //무기 체크 
    if(GetWeaponKind()!=pInfo->m_iWeaponDefine)
    {
        //	무기와 스타일이 일치하지 않습니다.
        g_InterfaceManager.GetInterfaceString( eST_NOT_STYLE_FOR_WEAPON, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/**
Todo:패킷 보낼때 적용 가능한지 한번 더 검사 하는게 좋겠다.
,스타일 변경 패킷날리고 무기 장착 바꿀경우 서버와 협의 할것.
,현재 장착된 무기와 스타일이 일치 하지 않을 경우는 스타일 없음 상태로 전환 한다.(스타일 장착후 무기 변동할때)
공격하는 도중에 바꾸면 어케 할지 Think.타수 꼬인다.히트 카운트를 초기화 해야 하나....

*/
BOOL Player::SetCurrentAttackStyle(DWORD dwStyle)
{
    if( IsMyHero() == TRUE )
    {
        // 자기 스타일이 도중에 바뀌면 콤보초기화 
	    if( GetCurrentAttackStyle() != dwStyle )
        {
		    g_HeroInput.SetComboReset();
        }
	}

    GetCharInfo()->m_iSelectStyleCode = dwStyle; 

	return TRUE;
}

//------------------------------------------------------------------------------
/**
적용 하려는 스타일이 현재 장착 무기와 적합 한지 검사  
*/
BOOL Player::CheckAttackStyle(DWORD dwStyle)
{
	BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo((SLOTCODE)dwStyle);
	//존재 하지 않는 스타일
	if(pInfo==NULL)
	{
		return FALSE;
	}
	else if(pInfo->m_iWeaponDefine==GetWeaponKind())
	{
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
장착 하려는 무기 타입과 스타일을 검사 한다.
*/
BOOL Player::CheckStyleForWeapon(DWORD dwWeaponType)
{
	DWORD dwStyle =this->GetCurrentAttackStyle();

	BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo((SLOTCODE)dwStyle);
	//스타일 정보가 없다.
	if(pInfo==NULL)
	{
		this->SetCurrentNomalAttackStyle(dwWeaponType);//스타일 없음 설정
		return FALSE;
	}
	else if(pInfo->m_iWeaponDefine!=dwWeaponType)	  //무기와 스타일이 안맞다면 
	{
		this->SetCurrentNomalAttackStyle(dwWeaponType);//스타일 없음 설정
		return FALSE;
	}

	return TRUE;
}


//------------------------------------------------------------------------------
/**
*/
WzVector Player::GetWeaponBonePos(int index)
{
	int boneindex = GetWeaponBone(index);
	WzVector ret;
	ZeroMemory(&ret,sizeof(ret));

	ret = GetBonePos(boneindex);

	return ret;
}

//------------------------------------------------------------------------------
/**
*/
void Player::ProcessValkyrieShot(DWORD dwTick)
{
	const float fTrailHeight = 0.03f;
	const DWORD dwShotLifeTime = 500;

	const float fCrossBowShotFrame1 = m_iValkyrieShotDelayTime - ((2.8f * 33) / m_fValkyrieShotSpeed);
	const float fCrossBowShotFrame2 = m_iValkyrieShotDelayTime - ((5.3f * 33) / m_fValkyrieShotSpeed);

	const int VALKYRIE_SHOT_SOUND1 = 408;
	const int VALKYRIE_SHOT_SOUND2 = 409;

	if (m_iValkyrieShotDelay > 0) {

		m_iValkyrieShotDelay -= dwTick;

		if (m_iValkyrieShotDelay < fCrossBowShotFrame1)
		{
			if (m_iValkyrieShotRemain >= 2)
			{
				g_SoundEffect.Play(VALKYRIE_SHOT_SOUND1,&GetVisiblePos());


				WzVector StartPosition = GetArrowBonePos(0);

				cProjectileUnit* pProjectile = new cProjectileUnit();
				pProjectile->SetLifeTime( 500 );
				pProjectile->SetArrowStartPosition( StartPosition );

				if( Character* pTarget = (Character*)g_ObjectManager.GetObject(GetLockedTarget()) )
                    pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
				else
					pProjectile->SetArrowTargetPosition( StartPosition + GetDirection() * 4.0f );

				pProjectile->SetUnitCode( eProjectile_SRC_ARROW );
				pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.03f, 0.8f, 100, 150, 255 );
				pProjectile->SetHitData( GetObjectKey(), m_dwValkyrieShotSerial, 0, FALSE, 0, FALSE );

				ShotCrossbow();

				m_iValkyrieShotRemain--;
			}
		}

		if (m_iValkyrieShotDelay < fCrossBowShotFrame2)
		{
			if (m_iValkyrieShotRemain >= 1)
			{
				g_SoundEffect.Play(VALKYRIE_SHOT_SOUND2,&GetVisiblePos());

				WzVector StartPosition = GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW ? GetArrowBonePos(1) : GetArrowBonePos(0);

				cProjectileUnit* pProjectile = new cProjectileUnit();
				pProjectile->SetLifeTime( 500 );
				pProjectile->SetArrowStartPosition( StartPosition );

				if( Character* pTarget = (Character*)g_ObjectManager.GetObject(GetLockedTarget()) )
					pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
				else
					pProjectile->SetArrowTargetPosition( StartPosition + GetDirection() * 4.0f );

				pProjectile->SetUnitCode( eProjectile_SRC_ARROW );
				pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.03f, 0.8f, 100, 150, 255 );
				pProjectile->SetHitData( GetObjectKey(), m_dwValkyrieShotSerial, 0, FALSE, 0, FALSE );


				ShotCrossbow();

				m_iValkyrieShotRemain--;
			}

		}

		if (m_iValkyrieShotDelay <= 0)
		{
			m_iValkyrieShotDelay = 0;
			m_dwValkyrieShotSerial = 0;
			CloseCrossbow();

			// 타플레이어는 공격후 고개를 원래대로 돌린다.
			if (g_pHero && g_pHero->GetObjectKey() != GetObjectKey())
			{
				LockOn(0);
			}
		}
	}
}
//
//#ifdef __NA_0_20080415_PROCESS_END_PROECSS
//int	Player::OnEndProcess( DWORD dwTick )
//{
//    std::vector<sAttachEffectForSetItem>* effect_attach_info_ptr = GetFullsetEffectAttachInfo();
//    if (effect_attach_info_ptr != NULL)
//    {
//        std::vector<sAttachEffectForSetItem>& effect_attach_info_ref = (*effect_attach_info_ptr);
//        int iSize = effect_attach_info_ref.size();
//        for( int i = 0; i < iSize; ++i )
//        {
//            if (effect_attach_info_ref[i].m_hEffectHandle == INVALID_HANDLE_VALUE)
//            {
//                continue;
//            }
//
//
//            CEffectBody *pBody = 
//                g_pEffectManager->GetEffectPtrHandle(effect_attach_info_ref[i].m_hEffectHandle);
//            if(pBody)
//            {
//                if(GetShow()&&Alpha_WzColor(GetColor())>0.0f&&GetCurShowFlag())
//                    pBody->SetShow(TRUE);
//                else
//                    pBody->SetShow(FALSE);
//            }
//        }
//    }
//
//	for (int a = 0; a < eITEM_EFFECT_TYPE_MAX; a++)
//	{
//		ITEM_EFFECT_PARAM effect_param;
//        GetItemEffectParam(a, effect_param);
//
//		if (effect_param.hEffect != INVALID_HANDLE_VALUE)
//		{
//			CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(effect_param.hEffect);
//			if(pBody)
//			{
//				if(GetShow()&&Alpha_WzColor(GetColor())>0.0f&&GetCurShowFlag())
//					pBody->SetShow(TRUE);
//				else
//					pBody->SetShow(FALSE);
//			}
//		}
//	}
//
//
//    for( int col =0; col < 2; ++col)
//    {
//	    for( int row =0; row < 3; ++row)
//	    {
//		    if( m_hEtherWeaponEffect[col][row] != INVALID_HANDLE_VALUE )
//		    {
//			    CEffectBody * pBody = g_pEffectManager->GetEffectPtrHandle(m_hEtherWeaponEffect[col][row]);
//			    if( !pBody) continue;
//    			
//			    if(GetShow()&&Alpha_WzColor(GetColor())>0.0f&&GetCurShowFlag())
//				    pBody->SetShow(TRUE);
//			    else
//				    pBody->SetShow(FALSE);
//		    }
//	    }
//    }
//
//
//	return 0;
//}
//#endif 

//------------------------------------------------------------------------------
/**
*/
DWORD Player::GetValkyrieShotSerial()
{
	return m_dwValkyrieShotSerial;
}

//------------------------------------------------------------------------------
/**
현재 상대방이 무기를 오른쪽으로 휘두르면 몸을 오른쪽으로 휘청거리게하고
왼쪽으로 휘두르면 왼쪽으로 휘청거리게 하는 루틴이 구현되어있음
상대방이 무기를 휘두르는 방향에 맞춰서 bRight인자가 들어옴
*/
void Player::PlayDamageAnimation(BOOL bRight,BOOL bCritical)
{
	// 플레이어는 이동중이거나 Idle상태일때만
	// 맞는 동작을 출력
	WzID damageAnimation = 0;
	BOOL bAniCritcal = FALSE;

    // 탈것을 탄 상태에서는 크리티컬 애니매이션을 사용하지 않는다.
    // 크리티컬 데미지 애니매이션의 경우 하체에 대한 애니매이션이 있기 때문에
    // 탈것을 탄 상태에서 애니매이션 블렌딩하는 동안 탈것 위로 일어서게 된다.
    //bCritical = (bCritical && (IsRiding() == FALSE)) ? TRUE : FALSE;
    if (IsRiding() || is_spread_wings())
    {
        bCritical = FALSE;
    }
    //else
    //{
    //    bCritical = TRUE;
    //}

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
void Player::PlayDamageSound(eWEAPONSOUNDKIND eSoundKind)
{
	assert(eSoundKind >= 0 && eSoundKind < eWEAPONSOUNDKIND_MAX);
	if (eSoundKind < 0 || eSoundKind >= eWEAPONSOUNDKIND_MAX)
	{
		return;
	}

    SOUND_INFO_CODE SoundInfoCode;
    SoundInfoCode.wCharacterCode = static_cast<WORD>(GetClass());
    SoundInfoCode.bWeaponType = static_cast<BYTE>(eSoundKind);
    SoundInfoCode.bGenderType = static_cast<BYTE>(ResourceCode::CheckValidGender(GetClass(), GetGender()));

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
void Player::SetName( const tstring& Name )
{
	memcpy( GetCharInfo()->m_tszCharName, Name.c_str(), MAX_CHARNAME_LENGTH );
	//서버에서 크기가 MAX_CHARNAME_LENGTH+1임으로 마지막을 널문자로 채우라고함
	GetCharInfo()->m_tszCharName[MAX_CHARNAME_LENGTH]=0;

	Object::SetName( Name );
}


PLAYER_MAKE_TYPE Player::GetPlayerType() const
{
	return m_PlayerMakeType;
}

void Player::SetPlayerType(PLAYER_MAKE_TYPE Type)
{
	m_PlayerMakeType = Type;
}

int Player::GetShotBlendBone()
{
	switch(GetClass())
	{
	case eCHAR_VALKYRIE:
		{
            if (GetBasePart()->GetWzUnit() != NULL)
            {
                WzBoneInfo *pInfo = GetBasePart()->GetWzUnit()->GetBoneInfo(StrToWzID("BLND"));
                if (pInfo) 
                {
                    return pInfo->m_iNode;
                }
                else
                {
                    return 5;
                }
            }
			else
			{
				return 5;
			}
		}			
	}

	return 0;
}

void Player::SetWeaponKind(DWORD dwWeaponKind)
{
    m_PlayerData.SetWeaponKind(dwWeaponKind);
}

BASE_ITEMINFO* Player::GetWeaponInfo()
{	
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return NULL;
    }

	// 무기에따라서 기본자세가 바뀝니다.
	SCSlot & rSlot = equipment_container->GetSlot(EquipmentContainer::EQUIP_WEAPON1);
	if (rSlot.GetCode() == 0)
	{
		return NULL;
	}
	else
	{
		BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rSlot.GetCode());
		if(!pItemInfo)
		{
			WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_CENTER, "Invalid Weapon ID [%d]", rSlot.GetCode());
			assert(pItemInfo && "장착된 무기 ID가 잘못되었습니다.");
		}
		
		return pItemInfo;
	}	
}

float Player::GetMoveSpeedModifier()
{
#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
    if (m_pRiderContainer && IsRiding())
    {
        return (m_pRiderContainer->get_rider_speed()/100.0f);
    }
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC

    // 값이 너무 작으면 보정
    if (m_wMoveSpeedRatio <= 0.0f)
    {
        return 0.0f;
    }
    else if (m_wMoveSpeedRatio < 1.0f)
    {
        return 0.01f;
    }

    float fMoveSpeedModifier = m_wMoveSpeedRatio / 100.0f;

	return fMoveSpeedModifier;
}

float Player::GetAttackSpeedModifier()
{
	const float fWeaponMultiplier = 0.67f;

	float fFinalMultiplier = fWeaponMultiplier * m_wAttSpeedRatio / 100.0f;

	return fFinalMultiplier;
}

eCHAREX_TYPE Player::GetClassExType()
{
	if (!this->GetCharInfo())
	{
		return  (eCHAREX_TYPE) eCHAR_TYPE_MAX;
	}

	return static_cast<eCHAREX_TYPE>(this->GetCharInfo()->GetChangeOfClass());
}

void Player::SetExp(EXPTYPE exp)
{
	this->GetCharInfo()->m_i64Exp = exp;
}


void Player::SetLevel(LEVELTYPE LV)
{
	this->GetCharInfo()->m_LV = LV;
}

void Player::SetHP(DWORD dwHP )
{
	this->GetCharInfo()->m_iHP = dwHP;

	Character::SetHP(dwHP);
}

void Player::SetMP(DWORD dwMP )
{
	this->GetCharInfo()->m_iMP = dwMP;

	Character::SetMP(dwMP);
}


//------------------------------------------------------------------------------
/**	현재 SD
*/
DWORD Player::GetSD()
{
	return GetCharInfo()->m_iSD;
}
//------------------------------------------------------------------------------
/** 최대 SP
*/
DWORD Player::GetMaxSD()
{
	return GetCharInfo()->m_iMaxSD;	
}
//------------------------------------------------------------------------------
/**	SP 셋팅
*/
void Player::SetSD(DWORD dwSD)
{
	this->GetCharInfo()->m_iSD = dwSD;
	Character::SetSD(dwSD);
}
//------------------------------------------------------------------------------
/** 최대 SP 셋팅
*/
void Player::SetMaxSD(DWORD dwMaxSD)
{
	this->GetCharInfo()->m_iMaxSD = dwMaxSD;
	Character::SetMaxSD(dwMaxSD);
}	

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
PLAYERFP Player::GetFP()
{
    return this->GetCharInfo()->cur_FP_;
}

PLAYERFP Player::GetMaxFP()
{
    return this->GetCharInfo()->max_FP_;
}

void Player::SetMaxFP( const PLAYERFP& max_fp )
{
    this->GetCharInfo()->max_FP_ = max_fp;
}

void Player::SetFP( const PLAYERFP& fp )
{
    this->GetCharInfo()->cur_FP_ = fp;
    if (this->GetCharInfo()->cur_FP_ >= 10.0f)
    {
        this->CreateFPFullEffect(this);
    }
    else
    {
        this->DestroyFPFullEffect();
    }
}
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

EXPTYPE Player::GetExp()
{
	return this->GetCharInfo()->m_i64Exp;
}


LEVELTYPE Player::GetLevel()
{
	return GetCharInfo()->m_LV;
}


DWORD Player::GetHP()
{
	return GetCharInfo()->m_iHP;
}

DWORD Player::GetMP()
{
	return GetCharInfo()->m_iMP;
}


DWORD Player::GetMaxHP()
{
	return GetCharInfo()->m_iMaxHP;
}
//------------------------------------------------------------------------------
DWORD Player::GetMaxMP()
{
	return GetCharInfo()->m_iMaxMP;
}
//------------------------------------------------------------------------------
float Player::GetHP_Percent()
{
    if (GetCharInfo()->m_iMaxHP > 0)
    {
        return (static_cast<float>(GetCharInfo()->m_iHP) / static_cast<float>(GetCharInfo()->m_iMaxHP) * 100.0f);
    }
    return 0.0f;
}
//------------------------------------------------------------------------------
float Player::GetMP_Percent()
{
    if (GetCharInfo()->m_iMaxMP > 0)
    {
        return (static_cast<float>(GetCharInfo()->m_iMP) / static_cast<float>(GetCharInfo()->m_iMaxMP) * 100.0f);
    }
    return 0.0f;
}
//------------------------------------------------------------------------------
EXPTYPE Player::GetNextExp()
{
	return GetAccumExp(GetLevel()+1);	
}

EXPTYPE Player::GetAccumExp(LEVELTYPE LV)
{
	BASE_ACCUM_EXP_INFO * pAccumExpInfo =  (BASE_ACCUM_EXP_INFO *)ExpValueInfoParser::Instance()->GetAccumExpInfo(LV);		
	if( !pAccumExpInfo )
	{	
		return 0;
	}
	return pAccumExpInfo->m_AccumExp;
}

WzID	Player::GetCurrentLeftTurnAnimation()
{
	if(IsPolymorph())
	{
		return GlobalFunc::GetAnimationByPolymorph(eANI_L_ROTATE);
	}
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_L_ROTATE,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_L_ROTATE);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '2';

	return StrToWzID(szAniCode);

}
WzID	Player::GetCurrentRightTurnAnimation()
{
	if(IsPolymorph())
	{
		return GlobalFunc::GetAnimationByPolymorph(eANI_L_ROTATE);
	}
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_R_ROTATE,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_R_ROTATE);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '3';

	return StrToWzID(szAniCode);
}

WzID	Player::GetCurrentLeftStepAnimation()
{
	if(IsPolymorph())
	{
		return GlobalFunc::GetAnimationByPolymorph(eANI_L_MOVE);
	}
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_L_MOVE,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_L_MOVE);
    }

    WzID animationID = 0;

    if (GetHideSkill())
    {
        // 하이드 상태의 애니코드
        if (GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            // 무기가 대거일때의 애니코드
            animationID = StrToWzID("S722");
        }
        else
        {
            animationID = StrToWzID("S009");
        }
    }
    else
    {
        char szAniCode[5];
        ZeroMemory(szAniCode,sizeof(szAniCode));

        szAniCode[0] = 'N';
        szAniCode[1] = GetWeaponAnimationCode();
        szAniCode[2] = '0';
        szAniCode[3] = '9';

        animationID = StrToWzID(szAniCode);
    }

	return animationID;
}

WzID	Player::GetCurrentRightStepAnimation()
{
	if(IsPolymorph())
	{
		return GlobalFunc::GetAnimationByPolymorph(eANI_R_MOVE);
	}
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_R_MOVE,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_R_MOVE);
    }

    WzID animationID = 0;

    if (GetHideSkill())
    {
        // 하이드 상태의 애니코드
        if (GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            // 무기가 대거일때의 애니코드
            animationID = StrToWzID("S723");
        }
        else
        {
            animationID = StrToWzID("S010");
        }
    }
    else
    {
        char szAniCode[5];
        ZeroMemory(szAniCode,sizeof(szAniCode));

        szAniCode[0] = 'N';
        szAniCode[1] = GetWeaponAnimationCode();
        szAniCode[2] = '1';
        szAniCode[3] = '0';

        animationID = StrToWzID(szAniCode);
    }

	return animationID;
}

WzID	Player::GetCurrentBackStepAnimation()
{
	if(IsPolymorph())
	{
		return GlobalFunc::GetAnimationByPolymorph(eANI_B_MOVE);
	}
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_B_MOVE,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_B_MOVE);
    }

    WzID animationID = 0;

    if (GetHideSkill())
    {
        // 하이드 상태의 애니코드
        if (GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            // 무기가 대거일때의 애니코드
            animationID = StrToWzID("S724");
        }
        else
        {
            animationID = StrToWzID("S011");
        }
    }
    else
    {
        char szAniCode[5];
        ZeroMemory(szAniCode,sizeof(szAniCode));

        szAniCode[0] = 'N';
        szAniCode[1] = GetWeaponAnimationCode();
        szAniCode[2] = '1';
        szAniCode[3] = '1';

        animationID = StrToWzID(szAniCode);
    }

	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID	Player::GetCurrentSpecialRightStepAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '2';
	szAniCode[3] = '0';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID	Player::GetCurrentSpecialFrontStepAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] =  GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '7';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID	Player::GetCurrentSpecialBackStepAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] =  GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '8';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID	Player::GetCurrentSpecialLeftStepAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '9';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentJumpStartAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_JUMP_START,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_JUMP_START);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'J';
	szAniCode[1] = GetWeaponAnimationCode();

	szAniCode[2] = '0';
	szAniCode[3] = '1';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentJumpKeepAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_JUMP_LOOP,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_JUMP_LOOP);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'J';
	szAniCode[1] = GetWeaponAnimationCode();

	szAniCode[2] = '0';
	szAniCode[3] = '2';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentJumpEndAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_JUMP_END,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_JUMP_END);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'J';
	szAniCode[1] = GetWeaponAnimationCode();

	szAniCode[2] = '0';
	szAniCode[3] = '3';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentJumpLandAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_JUMP_LAND,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_JUMP_LAND);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'J';
	szAniCode[1] = GetWeaponAnimationCode();

	szAniCode[2] = '0';
	szAniCode[3] = '4';

	return StrToWzID(szAniCode);
}



//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentJumpStunAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_HJUMP_END,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_HJUMP_END);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'J';
	szAniCode[1] = GetWeaponAnimationCode();

	szAniCode[2] = '0';
	szAniCode[3] = '5';


	return StrToWzID(szAniCode);
}


//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentJumpStunKeepAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_HJUMP_END_LOOP,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_HJUMP_END_LOOP);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'J';
	szAniCode[1] = GetWeaponAnimationCode();

	szAniCode[2] = '0';
	szAniCode[3] = '6';

	return StrToWzID(szAniCode);
}



//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentJumpStunEndAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_HJUMP_GETUP,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        return GlobalFunc::GetPlayerAnimationBySpreadWings(eANI_HJUMP_GETUP);
    }

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'J';
	szAniCode[1] = GetWeaponAnimationCode();

	szAniCode[2] = '0';
	szAniCode[3] = '7';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentJumpEndRunAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'J';
	
	szAniCode[1] = GetWeaponAnimationCode();

	szAniCode[2] = '0';
	szAniCode[3] = '4';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID Player::GetCurrentKnockBack2Animation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));
	
	szAniCode[0] = 'H';
	szAniCode[1] = 'S';
	szAniCode[2] = GetWeaponAnimationCode();
	szAniCode[3] = '5';
	
	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
Todo:무기에 따라서 나가는 발사체 종류 처리 
*/
DWORD Player::CreateProjectile(Character *pTarget,int iArrowBone)
{
    if( pTarget == NULL )
        return 0;

	DWORD dwProjectileCode = 0;

	BASE_ITEMINFO* pWeaponInfo = GetWeaponInfo();

	if (pWeaponInfo)
	{
		dwProjectileCode = pWeaponInfo->m_wMissile;
	}

	BASE_ProjectileInfo* pInfo = ProjectileInfoParser::Instance()->GetProjectileInfo(dwProjectileCode);

    WzVector StartPosition = GetArrowBonePos(iArrowBone);
    WzVector TargetPosition;


    if( pTarget != NULL )
    {
        TargetPosition = pTarget->GetArrowTargetPos();
    }
    else
    {
        TargetPosition = StartPosition + (GetDirection() * 4.0f);
    }

    DWORD ProjectileKey = 0;

    DWORD OwnerObjectKey = GetObjectKey();
    if( pInfo )
    {
        cProjectileUnit* pProjectile = new cProjectileUnit();
        pProjectile->SetArrowStartPosition( StartPosition );
        pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
        pProjectile->SetLifeTime( 333 );
        pProjectile->CreateEffect( pInfo->m_wzidFlyingEffectID.id, GetObjectKey(), pTarget->GetObjectKey() );
        pProjectile->SetHitData( GetObjectKey(), GetCurrentAttackSerial(), pInfo->m_wzidHitEffectID.id, FALSE, 0, TRUE );
        if( pInfo->m_bMakeTrail == TRUE )
        {
            pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, 150, 200, 255 );
        }

        g_ProjectileManagerEx.InsertProjectile( pProjectile );

        ProjectileKey = pProjectile->GetKey();
    }
    else if( GetWeaponKind() == eITEMTYPE_ORB )
    {
        WzVector temp = TargetPosition - StartPosition;
        float fDistance = VectorLength(&temp);

        DWORD MoveTime = max(200, (DWORD)(800.f * fDistance / GetAttackRange())); // 사정거리에 2.5초에 도달하는 속도


        cProjectileUnit* pProjectile = new cProjectileUnit();
        pProjectile->SetArrowStartPosition( StartPosition );
        pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
        pProjectile->SetLifeTime( MoveTime );
        pProjectile->CreateEffect( StrToWzID("E056"), GetObjectKey(), pTarget->GetObjectKey() );
        pProjectile->SetHitData( GetObjectKey(), GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );

        ProjectileKey = pProjectile->GetKey();
    }
#ifdef _NA_008740_20160321_MAGICIAN_STAFF_CREATE_PROJECTILE
	else if( GetWeaponKind() == eITEMTYPE_STAFF )
	{
		WzVector temp = TargetPosition - StartPosition;
		float fDistance = VectorLength(&temp);

		DWORD MoveTime = max(200, (DWORD)(800.f * fDistance / GetAttackRange())); // 사정거리에 2.5초에 도달하는 속도


		cProjectileUnit* pProjectile = new cProjectileUnit();
		pProjectile->SetArrowStartPosition( StartPosition );
		pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
		pProjectile->SetLifeTime( MoveTime );
		pProjectile->CreateEffect( StrToWzID("E056"), GetObjectKey(), pTarget->GetObjectKey() );
		pProjectile->SetHitData( GetObjectKey(), GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
		g_ProjectileManagerEx.InsertProjectile( pProjectile );

		ProjectileKey = pProjectile->GetKey();
	}
#endif // _NA_008740_20160321_MAGICIAN_STAFF_CREATE_PROJECTILE
    else if( GetWeaponKind()== eITEMTYPE_ETHERWEAPON )
    {
        WzVector temp = TargetPosition - StartPosition;
        float fDistance = VectorLength(&temp);

        DWORD MoveTime = max(200, (DWORD)(733.0f * fDistance / GetAttackRange())); // 사정거리에 2.5초에 도달하는 속도

        cProjectileUnit* pProjectile = new cProjectileUnit();
        pProjectile->SetArrowStartPosition( StartPosition );
        pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
        pProjectile->SetLifeTime( MoveTime );
        pProjectile->CreateEffect( StrToWzID("A511"), GetObjectKey(), pTarget->GetObjectKey() );
        pProjectile->SetHitData( GetObjectKey(), GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );

        ProjectileKey = pProjectile->GetKey();

    }
    else
    {

        cProjectileUnit* pProjectile = new cProjectileUnit();
        pProjectile->SetArrowStartPosition( StartPosition );
        pProjectile->SetArrowTargetObjectKey( pTarget->GetObjectKey(), TRUE );
        pProjectile->SetLifeTime( 100 );
        pProjectile->CreateEffect( StrToWzID("A041"), GetObjectKey(), pTarget->GetObjectKey() );
        pProjectile->SetUnitCode( eProjectile_SRC_ARROW );
        pProjectile->CreateTrail( g_ProjectileManagerEx.GetTrailTexture(), 0.04f, 0.8f, 150, 200, 255 );
        pProjectile->SetHitData( GetObjectKey(), GetCurrentAttackSerial(), 0, FALSE, 0, TRUE );
        g_ProjectileManagerEx.InsertProjectile( pProjectile );

        ProjectileKey = pProjectile->GetKey();

    }

    return ProjectileKey;
}


void Player::CreatePopupMesh(
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

	eDMGPOP_TARGET_TYPE eTargetType = eDMGPOP_TARGET_PLAYER;
	eDMGPOP_COLOR_TYPE eColorType = eDMGPOP_COLOR_GENERAL;		

	switch( eKind )
	{
	case eDAMAGEPOPUP_ENEMYHPDAMAGE:
	case eDAMAGEPOPUP_ENEMYMPDAMAGE:
	case eDAMAGEPOPUP_PLAYERHPDAMAGE:
	case eDAMAGEPOPUP_PLAYERMPDAMAGE:
		{
			eColorType = eDMGPOP_COLOR_CRITICAL; // 플레이어는 무조건 붉은색으로 뜨게함.
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

	// 히어로냐?
	if (g_pHero)
	{
		bHero = BOOL( g_pHero == this );
	}	

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
		fHeight, 
		bHero
		);
}

void  Player::OpenCrossbow()
{
	if (GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW) {
		SetAttachmentAnimation(0,StrToWzID("N002"),FALSE);
		SetAttachmentAnimation(1,StrToWzID("N002"),FALSE);
	}
}

void  Player::CloseCrossbow()
{
	if (GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW) 
	{
		SetAttachmentAnimation(0,StrToWzID("N001"),FALSE);
		SetAttachmentAnimation(1,StrToWzID("N001"),FALSE);
	}
}

void  Player::ShotCrossbow()
{
	if (GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW) 
	{
		SetAttachmentAnimation(0,StrToWzID("N003"),FALSE);
		SetAttachmentAnimation(1,StrToWzID("N003"),FALSE);
	}
}

WzID Player::GetValkyrieShotAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'A';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '0';

	switch (GetCurState()) 
	{
		// 이동중
	case STATE::MOVE:
	case STATE::KEYBOARDMOVE:
		{		
			szAniCode[3] = '3';
			szAniCode[3] += m_iValkyrieShotAnimVariation;
		}
		break;
		// 일반
	default:
		{		
			szAniCode[3] = '1';
			szAniCode[3] += m_iValkyrieShotAnimVariation;
		}
		break;	
	}

	return StrToWzID(szAniCode);
}

WzID Player::GetValkyrieReloadAnimation()
{
	return StrToWzID("A211");
}

void Player::SetIdleAnimation()
{
	tagWZID AnimID;

	if(!IsBlock())
	{
		// 어택스탠드 때문에 지속적으로 애니메이션세팅을 다시해준다.
		AnimID.id = GetCurrentIdleAnimation();

        //! 날기 상태(idle/move) 있을대 다른 모션 체크
        ePLAYER_ANI spread_player_ani = eANI_IDLE;
        AnimID.id = GetCurrentSpreadOtherAnimation(AnimID.id, spread_player_ani);

		if (AnimID.id != GetCurrentAnimation())
		{
#ifdef _NA_COSTUMEITEM_IDLE_ANIMATION
			if( m_IsCostumIdleAnimation == TRUE )
			{
				SetPlayerAnimation(AnimID.id, eANI_IDLE, FALSE );
			}
			else
			{
#else
			{
#endif //_NA_COSTUMEITEM_IDLE_ANIMATION
				if (spread_player_ani == eANI_SPREAD_OTHERMOTION_IDLE)
				{ 
					SetPlayerAnimation(AnimID.id, spread_player_ani, FALSE);
				} 
				else 
				{
					if (AnimID.code[3] == '2')
					{ 
						    SetPlayerAnimation(AnimID.id, eANI_IDLE, FALSE);
                        }
					else if (AnimID.code[3] == '3')
					{ 
						// TODO: gs_bug_fix 09.07: 일단 수정를위해나가고 추후 디파인제거하면서 정리..
						if (IsRiding())
						{
							SetPlayerAnimation(AnimID.id, eANI_IDLE2, FALSE);
						}
						else
						{
							SetPlayerAnimation(AnimID.id, eANI_IDLE2, TRUE);
						}
					} 
					else 
					{ 
						SetPlayerAnimation(AnimID.id,eANI_STAND,TRUE);
					} 
				} 
			}

			if (GetBasePart())
			{
				GetBasePart()->SetInterpolationFrame(6);
			}
		}
	}
}


//----------------------------------------------------------------------------
/**
*/
void Player::SetBeHaveState(ePLAYER_BEHAVE_STATE eBeHaveState)
{
	this->m_BehaveState = eBeHaveState;
}


//----------------------------------------------------------------------------
/**
*/
ePLAYER_BEHAVE_STATE		
Player::GetBeHaveState()
{
	return this->m_BehaveState;
}

void Player::OnAttack(DWORD dwSkillCode)
{
	Character::OnAttack(dwSkillCode);

#ifdef _PERF_DEBUG
	return;
#endif

	if( BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillCode) )
	{
        if( pInfo->m_SkillClassCode == eSKILL_WIND_SHIELD )
        {
			if( m_dwShieldTime == 0 )
			{
				m_dwShieldTime = 1000;

                cTargetPosition Position;
                Position.SetTargetVisiblePosition( GetObjectKey(), TRUE );
                Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

                g_ProjectileManagerEx.CreateEffect( GetObjectKey(), StrToWzID("K370"), 3000, Position );
			}
        }
        else if( pInfo->m_SkillClassCode == eSKILL_ELECTRIC_FIELD )
        {
			if( m_dwShieldTime == 0 )
			{
				m_dwShieldTime = 1000;

                cTargetPosition Position;
                Position.SetTargetVisiblePosition( GetObjectKey(), TRUE );
                Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

                g_ProjectileManagerEx.CreateEffect( GetObjectKey(), StrToWzID("K381"), 3000, Position );
			}
        }
	}
}

void Player::OnDamage(const DAMAGE_PRAM &rDamage)
{
	DWORD dwAdditionalEffect = rDamage.pResult->GetTargetData().GetAdditionalEffect();
	
	Character::OnDamage(rDamage);
	SetAttackStandTime(ATTACK_STAND_TIME);

	// 부가 효과 적용
	BOOL bCritical = dwAdditionalEffect & (ATTACK_ADDITIONAL_EFFECT_CRITICAL);

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    if (GetHideSkill() != HIDE_SKILL_INFO::ALL_HIDE)
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    {
        // 맞는 이펙트도 나온다
        CreateDamageEffect(bCritical,rDamage.bRight,rDamage.dwObjKey);	

        if ( !(rand() % 3) )
        {
            PlayDamageSound(rDamage.pResult->GetWeaponSound());														
        }
    }
	switch (GetCurState())
	{
		case STATE::DEATH:
			break;

		default:
			if ( !(dwAdditionalEffect & ATTACK_ADDITIONAL_EFFECT_DONOTPLAYANIMATION) )
			{
				PlayDamageAnimation(rDamage.bRight,bCritical);							
			}
			break;
	}


	if(GetCurState()==STATE::SIT || GetCurState()==STATE::EMOTICON)
	{
		SetNextState(STATE::IDLE,g_CurTime);	
	}

	//쉴드 표시 
	if (m_dwShieldTime == 0)
	{
        DWORD dwState = 0;

        std::vector<SKILL_EFFECT*>& array = GetSkillEffectArray();

        for( size_t i=0;i!=array.size();++i)
        {
            SKILL_EFFECT* pEffect = array[i];

            if( pEffect->dwAbilityID == eABILITY_MAGIC_SHIELD )
            {
				m_dwShieldTime = 300;

                cTargetPosition Position;
                Position.SetTargetVisiblePosition( GetObjectKey(), TRUE );
                //Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

                g_ProjectileManagerEx.CreateEffect( GetObjectKey(), StrToWzID("K361"), 3000, Position );

                break;
            }
		}
	}
}

void Player::PlayEvadeAnimation()
{
	
}
void Player::PlayBlockAnimation() 
{
	if(IsRiding())
	{
		return;
	}
    if (is_spread_wings())
    {
        return;
    }

	WzID blockanim = GetCurrentBlockAnimation();
	assert(blockanim);

	// 플레이어는 이동중이거나 Idle상태일때만
	// 맞는 동작을 출력
	switch(GetCurState())
	{
	case STATE::IDLE:
		this->SetAnimation(blockanim,FALSE);
		break;
	}			
}


BOOL Player::ProcessSkillEffect(DWORD dwTick)
{
    std::vector<SKILL_EFFECT*>& array = GetSkillEffectArray();

    for( size_t i=0;i!=array.size();++i)
    {
        SKILL_EFFECT* pEffect = array[i];

		pEffect->Process(this,dwTick);

		if ( SkillScriptInfo * pSkillInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)pEffect->dwSkillID) )
		{
            // 아이템 사용으로 생긴 이펙트인데 이펙트가 없는경우 생성
			if( IsExistItemEffect(pSkillInfo->m_dwItemCode) == FALSE )
				OnUseItem(pSkillInfo->m_dwItemCode);
		}
	}

	UpdateSkillEffectPos();

    UpdateShieldEffect();
    
	return TRUE;
}

//
//BOOL Player::CreateWasteItemEffect(CODETYPE code)
//{
//    // 아이템 사용에 의한 일회성 이펙트를 생성하는 함수
//    // 루프이펙트는 그 핸들을 관리할 주체가 없으므로 여기에서 다루어지지 않는다
//
//	BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(code);
//
//    if( pItemInfo == NULL )
//	{
//		return FALSE;
//	}
//
//    if (pItemInfo->m_dwEffectCode == Null_Value)
//    {
//        return FALSE;
//    }
//
//    if( pItemInfo->m_wType == eITEMTYPE_FIRECRACKER ||
//        pItemInfo->m_wType == eITEMTYPE_POLLEN ||
//        pItemInfo->m_wType == eITEMTYPE_TORCH ||
//        pItemInfo->m_wType == eITEMTYPE_MINE )
//    {
//        return TRUE;
//    }
//
//
//	CEffectBody* pBody = NULL;
//
//	// 루프나 라이프 타임이 있는 아이템의 경우 핸들 처리 
//	if( pItemInfo->m_byEffectPosition == eSKILLEFFECT_POS_LRHAND )
//	{
//        // 양손에서 2개 생성한다
//
//		WzVector vPos = GetSkillEffectPos(eSKILLEFFECT_POS_RHAND );
//		HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, vPos, GetObjectKey());
//
//        // 루프이펙트는 핸들을 관리할 주체가 없기때문에 바로 삭제한다
//		pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
//		if (pBody && pBody->IsLoop())
//		{
//			g_pEffectManager->RemoveEffectPtr(hHandle);
//		}
//
//		vPos = GetSkillEffectPos(eSKILLEFFECT_POS_LHAND );
//		hHandle = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, vPos, GetObjectKey());
//
//        // 루프이펙트는 핸들을 관리할 주체가 없기때문에 바로 삭제한다
//		pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
//		if (pBody && pBody->IsLoop())
//		{
//			g_pEffectManager->RemoveEffectPtr(hHandle);
//		}
//	}
//	else
//	{
//		WzVector vPos = GetSkillEffectPos( (eSKILLEFFECT_POS)pItemInfo->m_byEffectPosition );
//		HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, vPos, GetObjectKey());
//
//        // 루프이펙트는 핸들을 관리할 주체가 없기때문에 바로 삭제한다
//		pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
//		if (pBody && pBody->IsLoop())
//		{
//			g_pEffectManager->RemoveEffectPtr(hHandle);
//		}
//	}	
//
//	return TRUE;
//}

//Todo:장착 가능한 아이템이 들어 온다.
BOOL Player::OnUseItem(DWORD dwItemCode)
{
	BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo(dwItemCode);
	if (!pItemInfo)
	{
		return FALSE;
	}

	if (pItemInfo->m_dwEffectCode==Null_Value)
	{
		return FALSE;
	}

    RemoveItemEffectByItemCode( dwItemCode );
    AddItemEffect( this, pItemInfo );


	return TRUE;
}

BOOL Player::OnUnUseItem(DWORD dwItemCode)
{
	//Todo:아이템 이펙트 삭제 작업 	
    return RemoveItemEffectByItemCode( dwItemCode );
}

void Player::OnLevelUp()
{
    //	%s님이 레벨업 하셨습니다
    TCHAR szResult[MAX_MESSAGE_LENGTH];
    TCHAR szMessage[MAX_MESSAGE_LENGTH];

    g_InterfaceManager.GetInterfaceString( eST_PLAYER_LEVEL_UP, szMessage, MAX_MESSAGE_LENGTH );
    Sprintf(szResult, szMessage, GetName());

    if( GlobalFunc::IsParty( GetObjectKey() ) )
    {
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szResult, CHAT_TYPE_SYSTEM );
	}

#ifdef _NA_20111213_SKILLTREE_VISUAL_RENEWAL2
    CreateEffectAtHead(StrToWzID("A741"),0.0f);
#else

    if (dwLevel % 3 == 0) //3레벨 마다 스킬포인트가 증가를 강조
    {
        CreateEffectAtHead(StrToWzID("A741"),0.0f);
    }
    else
    {
        CreateEffectAtHead(StrToWzID("A015"),0.0f);
    }
#endif //_NA_20120509_LEVELUP_EFFECT

    if( IsMyHero() == FALSE )
    {
		WzVector vPos = GetPosition();
		g_SoundEffect.Play(666,&vPos);
	}
}


void Player::ApplyStyleBuff( DWORD dwStyleCode ,SkillAttrCalculator &SkillCalc)
{
	BASE_STYLEINFO *pBaseStylenfo = SkillInfoParser::Instance()->GetStyleInfo( (SLOTCODE)dwStyleCode );
	if( !pBaseStylenfo )	return;

	// 공격 성공 증가
	if( pBaseStylenfo->m_iAttackRate )
		SkillCalc.AddAttr( eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO, VALUE_TYPE_VALUE, pBaseStylenfo->m_iAttackRate );

	// 블록 성공 증가
	if( pBaseStylenfo->m_iAvoidRate )
		SkillCalc.AddAttr( eATTR_PHYSICAL_ATTACK_BLOCK_RATIO, VALUE_TYPE_VALUE, pBaseStylenfo->m_iAvoidRate );

	// 공속 증가
	if( pBaseStylenfo->m_iAttackSpeed )
		SkillCalc.AddAttr( eATTR_ATTACK_SPEED, VALUE_TYPE_VALUE, pBaseStylenfo->m_iAttackSpeed );

	// 방어력 증가
	if( pBaseStylenfo->m_iBonusDefence )
		SkillCalc.AddAttr( eATTR_OPTION_ALL_DEFENSE_POWER, VALUE_TYPE_VALUE, pBaseStylenfo->m_iBonusDefence );

	// 크리티컬 확률 증가
	if( pBaseStylenfo->m_wCriticalBonus )
		SkillCalc.AddAttr( eATTR_ADD_ALL_CRITICAL_RATIO, VALUE_TYPE_VALUE, pBaseStylenfo->m_wCriticalBonus );
}

WzVector Player::GetArrowBonePos(int iAttachIndex)
{
	if (!IsAnimated())
	{
		AnimateUnit();
	}

    if ((iAttachIndex >= 0) && (iAttachIndex < (int)GetAttachmentCount()))
    {
	    CWzUnitDraw* pSkeleton = GetAttachment(iAttachIndex).GetUnitDraw();
	    if ((pSkeleton != NULL) && (pSkeleton->GetWzUnit() != NULL)) 
	    {
		    if (WzBoneInfo* pInfo = pSkeleton->GetWzUnit()->GetBoneInfo(StrToWzID("ARRW")))
		    {
			    WzVector vRet;
			    if (GameFunc::GetBonePos( pSkeleton, pInfo->m_iNode, &vRet) == TRUE)
			    {
				    return vRet;
			    }
		    }
	    }
    }
    else
    {
        assert(iAttachIndex >= 0 && iAttachIndex < (int)GetAttachmentCount() );
    }

	return GetVisiblePos();
}

void Player::RenderInfo()
{

	if(g_HeroInput.IsShowPartyMemberOnly())
	{
		uiPartyMan* partyMan =
			static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

		if(partyMan&&partyMan->IsPartyMember(GetObjectKey())==FALSE)
		{
			return Character::RenderInfo();
		}
	}

	if( IsGM() )
	{
		_renderinfo_GM();
	}
	else
	{
		if ( IsShowName() == TRUE ) // 캐릭 선택시 ( etc. 변수명이 왜 m_bShowName 일까??)
			renderInfo_InCharacterSenen();
		else
			renderInfo_InBattleScene();
	}

	Character::RenderInfo();
}


float Player::GetAttackRange( void )
{
	const float PUNCH_ATTACK_RANGE = 2.0f;

	// 무기에따라서 기본자세가 바뀝니다.
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return PUNCH_ATTACK_RANGE;
    }

	SCSlot & rSlot = equipment_container->GetSlot(EquipmentContainer::EQUIP_WEAPON1);

	//float fBonus = (float)m_pPlayerAttriburtes->GetNormalRangeBonus() / 10.0f;

	if (rSlot.GetCode() == 0)
	{
		return PUNCH_ATTACK_RANGE;
	}
	else
	{
		BASE_ITEMINFO* pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rSlot.GetCode());
		if(pItemInfo)
		{
            // 비활성화에 대한 고려를 추가
            if (TRUE == this->IsMyHero())
            {
                ns_formula::EquipSlotsManageInfo* equip_slot_manage_info = g_pHero->GetEquipmentContainer()->GetEquipmentManageInfo();
                if (equip_slot_manage_info->slots[EquipmentContainer::EQUIP_WEAPON1].activated == false)
                {
                    return PUNCH_ATTACK_RANGE;
                }
            }

			return (float)(pItemInfo->m_fAttRange );
		}
		else
		{
#ifdef _DEV_VER
			TCHAR szDebugMsg[1024] = {0,};
			Sprintf(szDebugMsg, _T("장착된 무기 ID가 잘못되었습니다[%d]"), rSlot.GetCode());
			GlobalFunc::OutputMsg_SysMsg(szDebugMsg);
#endif
			return PUNCH_ATTACK_RANGE ;
		}
	}	
}

BOOL Player::IsRangedNormalAttack()
{
	switch (GetWeaponKind())
	{
	case eWEAPONTYPE_ONEHANDCROSSBOW:
	case eWEAPONTYPE_ETHERWEAPON:
	case eWEAPONTYPE_ORB:
#ifdef _NA_008740_20160321_MAGICIAN_STAFF_CREATE_PROJECTILE
	case eWEAPONTYPE_STAFF:
#endif // _NA_008740_20160321_MAGICIAN_STAFF_CREATE_PROJECTILE
		{
			return TRUE;		
		}
		break;
	default:
		{
			return FALSE;
		}
		break;	
	}
}

float Player::GetMeshHeight()
{
	BASE_HeroHeightInfo *pInfo = HeroHeightInfoParser::Instance()->GetHeroHeightInfo(GetImageClass());
    int height_variation = GetHeightVariation();
	assert(height_variation >= 0 && height_variation < MAX_HEIGHT_VARIATION);

	float fScale = 1.0f;

	if (pInfo)
	{
		fScale = pInfo->m_fHeight[height_variation];
	}

    float height_offset = GetHeightOffset();

	if(IsTransForm())//변신 중일때
	{
		return 2.2f * fScale + height_offset;
	}

	switch(GetImageClass())
	{
	case eCHAR_BERSERKER:
		return 2.2f * fScale + height_offset;
    case eCHAR_DRAGON:
		return 2.0f * fScale + height_offset;
    case eCHAR_SHADOW:
		return 2.0f * fScale + height_offset;
    case eCHAR_VALKYRIE:
		return 1.75f * fScale + height_offset;
	case eCHAR_MAGICIAN:
		return 1.75f * fScale + height_offset;
    case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return 1.4f * fScale + height_offset;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return 2.0f * fScale + height_offset;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return 1.75f * fScale + height_offset;
	}

	return m_fMeshHeight;
}

void Player::OnFootStep(BOOL b2D /* = FALSE */)
{
	if (BIT_CHECK(m_dwCurTileAtt, PTA_PLAYER_FALLDOWN_TILE))
    {
		return;
    }

	if (m_bObserverMode)
	{
		return;
	}

	const int MAX_FOOTSTEP_VARIATION = 4;
	WzVector vPos = GetVisiblePos();

    BOOL bMan;

    if (GetGender() == GenderType::kMale)
    {
        bMan = TRUE;
    }
    else
    {
        bMan = FALSE;
    }

	DWORD dwVariation =  rand() % MAX_FOOTSTEP_VARIATION;

	switch (m_dwTerrainAttribute)
	{
	case eTERRAIN_ATTRIBUTE_DIRT:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2100 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2100 + dwVariation, &vPos);
				}				
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2000 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2000 + dwVariation, &vPos);
				}				
			}
		}
		break;
	case eTERRAIN_ATTRIBUTE_STONE:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2124 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2124 + dwVariation, &vPos);
				}
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2024 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2024 + dwVariation, &vPos);
				}

			}
		}
		break;
	case eTERRAIN_ATTRIBUTE_PEBBLE:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2108 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2108 + dwVariation, &vPos);
				}
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2008 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2008 + dwVariation, &vPos);
				}				
			}
		}
		break;

	case eTERRAIN_ATTRIBUTE_GRASS:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2104 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2104 + dwVariation, &vPos);
				}				
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2004 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2004 + dwVariation, &vPos);
				}				
			}
		}
		break;

	case eTERRAIN_ATTRIBUTE_METAL:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2112 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2112 + dwVariation, &vPos);
				}
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2012 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2012 + dwVariation, &vPos);
				}
			}
		}
		break;

	case eTERRAIN_ATTRIBUTE_WATER:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2128 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2128 + dwVariation, &vPos);
				}
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2028 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2028 + dwVariation, &vPos);
				}
			}
		}
		break;

	case eTERRAIN_ATTRIBUTE_MUD:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2116 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2116 + dwVariation, &vPos);
				}
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2016 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2016 + dwVariation, &vPos);
				}
			}
		}
		break;

	case eTERRAIN_ATTRIBUTE_WOOD:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2132 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2132 + dwVariation, &vPos);
				}
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2032 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2032 + dwVariation, &vPos);
				}
			}
		}
		break;

	case eTERRAIN_ATTRIBUTE_SNOW:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2120 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2120 + dwVariation, &vPos);
				}
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2020 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2020 + dwVariation, &vPos);
				}
			}
		}
		break;

	case eTERRAIN_ATTRIBUTE_DESERT:
		{
			if (bMan) 
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2136 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2136 + dwVariation, &vPos);
				}
			}
			else
			{
				if (b2D)
				{
					g_SoundEffect.PlayUI(2036 + dwVariation);
				}
				else
				{
					g_SoundEffect.Play(2036 + dwVariation, &vPos);
				}
			}
		}
		break;	
	}	
}


void Player::ProcessStandAnimationDelay(DWORD dwTick)
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

#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
void Player::SetSpaSittingAnimation(DWORD dwTick)
{
    // 대기시간 갱신
    if (current_ani_progress_rate() >= 100)
    {
        // 대기시간 부여
        SetSpaSittingAnimationDelay();
    }

    // 딜레이 계산
    if (spa_sit_idle_ani_delay_ < dwTick)
    {
        spa_sit_idle_ani_delay_ = 0;
    }
    else 
    {
        spa_sit_idle_ani_delay_ -= dwTick;
    }


#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    WzID animation_id = 0;
    SpaZoneInfo* spa_info = SpaZoneInfoParser::Instance()->GetSpaZoneInfo(g_pMap->GetMapID());

    if (spa_info && spa_sit_idle_ani_delay_ == 0)
    {
        if (spa_info->special_animation[2].id != 0)
        {
            animation_id = spa_info->special_animation[2].id;

            if (animation_id != GetCurrentAnimation())
            {
                // 온천 앉기 IDLE 애니메이션
                SetPlayerAnimation(animation_id,eANI_SIT_IDLE,FALSE);
            }
        }
    }
    else
    {
        if (spa_info->special_animation[1].id != 0)
        {
            animation_id = spa_info->special_animation[1].id;
            SetPlayerAnimation(animation_id, eANI_SIT_IDLE,TRUE);
        }
    }
#else
    if (spa_sit_idle_ani_delay_ == 0)
    {
        if ((StrToWzID("N024")) != GetCurrentAnimation())
        {
            // 온천 앉기 IDLE 애니메이션
            SetPlayerAnimation(StrToWzID("N024"),eANI_SIT_IDLE,FALSE);
        }
    }
    else
    {
        SetPlayerAnimation(StrToWzID("N022"),eANI_SIT_IDLE,TRUE);
    }
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
}

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
BOOL Player::IsInSpaTile_Change_SitIdle()
{
    if ((GlobalFunc::IsSpaScene() == TRUE) &&
        (BIT_CHECK(m_dwCurTileAtt, PTA_EXTRA_TILE)))
    {
        // 앉기IDLE 애니메이션 바꾸는 타입
        SpaZoneInfoParser::SpaAnimationStyle style = SpaZoneInfoParser::Instance()->GetSpaAnimationStyle(g_pMap->GetMapID());
        if (style == SpaZoneInfoParser::AnimationStyle_SitIdle)
        {
            return TRUE;
        }
    }

    return FALSE;
}
BOOL Player::IsInSpaTile_Change_Sit()
{
    if ((GlobalFunc::IsSpaScene() == TRUE) &&
        (BIT_CHECK(m_dwCurTileAtt, PTA_EXTRA_TILE)))
    {
        // 앉기 애니메이션 바꾸는 타입
        SpaZoneInfoParser::SpaAnimationStyle style =
            SpaZoneInfoParser::Instance()->GetSpaAnimationStyle(g_pMap->GetMapID());
        if (style == SpaZoneInfoParser::AnimationStyle_Sit)
        {
            return TRUE;
        }
    }

    return FALSE;
}
#else
BOOL Player::IsInSpaTile()
{
    if ((GlobalFunc::IsSpaScene() == TRUE) &&
        (BIT_CHECK(m_dwCurTileAtt, PTA_EXTRA_TILE)))
    {
        return TRUE;
    }

    return FALSE;
}
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

void Player::SetSpaSittingAnimationDelay()
{
    const int STAND_ANIMATION_MINIMUM_DELAY = 16000;
    const int STAND_ANIMATION_DELAY_VARIATION = 10000;

    spa_sit_idle_ani_delay_ = 
        STAND_ANIMATION_MINIMUM_DELAY + rand() % STAND_ANIMATION_DELAY_VARIATION;

}
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM

int Player::GetBulletCount()
{
	return m_iValkyrieBulletCount;
}

void Player::SetBulletCount(int iCount)
{
	m_iValkyrieBulletCount = iCount;

}

int Player::DecreaseBulletCount()
{
	m_iValkyrieBulletCount--;

	return m_iValkyrieBulletCount;
}

void  Player::ValkyrieReload( BOOL bAnimation )
{
	m_iValkyrieShotAnimVariation = rand() % 2;

	WzID ReloadAnimID = GetValkyrieReloadAnimation();
	WzAnimationInfo *pAniInfo = NULL;
    if (GetBasePart()->GetWzUnit() != NULL)
    {
        pAniInfo = GetBasePart()->GetWzUnit()->GetAnimationInfo(ReloadAnimID);
    }

	m_iValkyrieShotDelay = 300;

	// 이전 공격에 대한 처리를 확실히 해준다.
	// 이걸 과연 해야할지 의구심이든다 -> 꾸오
	if (m_dwValkyrieShotSerial)	
	{
		ProcessAttackResult(m_dwValkyrieShotSerial,TRUE);
		m_dwValkyrieShotSerial = 0;
	}

	if (pAniInfo)
	{
		m_iValkyrieShotDelay = (int)((float)pAniInfo->m_nMaxFrame * 33.0f); 
	}

	if( bAnimation )
	{
		SetCurrentBlendAnimation(ReloadAnimID,GetShotBlendBone());
	}

	m_bValkyrieReload = TRUE;
}

void Player::SetMaxHP( DWORD iHP )
{
	GetCharInfo()->m_iMaxHP = iHP;
}

void Player::SetMaxMP( DWORD iMP )
{
	GetCharInfo()->m_iMaxMP = iMP;
}

void Player::SetAttackSpeedRatio(int iSpeed)
{
	m_wAttSpeedRatio = iSpeed;
}

void Player::SetMoveSpeedRatio(int iSpeed)
{
	m_wMoveSpeedRatio = iSpeed;
}

void Player::LaunchEtherWeapon()
{
	if (GetWeaponKind() == eWEAPONTYPE_ETHERWEAPON) 
	{
		SetAttachmentAnimation(0,StrToWzID("N001"),FALSE);		
	}
}

void Player::OnShotStart()
{
	OpenCrossbow();
	LaunchEtherWeapon();
}

void Player::OnShotEnd()
{
	CloseCrossbow();
}

void Player::OnParalyzed()
{
	// 키보드이동도멈춤
	ResetMoveFlag();
	Character::OnParalyzed();
}

void Player::OnShotFire(int iShotCount)
{
	switch(GetWeaponKind())
	{
	case eWEAPONTYPE_ONEHANDCROSSBOW:
		{

			GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A509"),GetArrowBonePos(iShotCount),GetObjectKey());


			SetAttachmentAnimation(iShotCount,StrToWzID("N001"),FALSE);
			//화살타격
			Character *pTarget = (Character *)GetTargetObject();
			if (pTarget)
			{
				WzVector vDest,vTemp;
				float fArrowAngleZ,fArrowAngleX;

				vTemp=pTarget->GetVisiblePos();
				vTemp.z+=pTarget->GetArrowTargetHeight();

				vDest = vTemp-GetArrowBonePos(iShotCount);//이전 위치에서 현재 위치로 계산하자.
				vTemp = vDest;

				float fLength;

				if (vTemp.x == 0.0f && vTemp.y == 0.0f)
				{
					fLength = 0.0f;
				}
				else
				{
					fLength = VectorLength(&vTemp);
				}

				fArrowAngleX = -atan2(vDest.z,fLength);			//최종 목적지가 아닌 현재 목적지로 꺽자
				fArrowAngleZ = Math_GetAngleFromVector(&vDest); //최종 목적지가 아닌 현재 목적지로 꺽자

				vDest=pTarget->GetVisiblePos();
				vDest.z+=pTarget->GetArrowTargetHeight();

				HANDLE handle;

				handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A041"),vDest,GetObjectKey());//!loop 이펙트 쓰지 말것

				CEffectBody *pBody = g_pEffectManager->GetEffectPtrHandle(handle);
				if(pBody)
				{
					WzVector vRot;
					vRot.x = fArrowAngleX;
					vRot.y = 0.0f;
					vRot.z = fArrowAngleZ;
					WzVector vScale;
					vScale.x = 1.0f;
					vScale.y = 1.0f;
					vScale.z = 1.0f;

					pBody->SetResultMatrix(vDest,vRot,vScale);
				}
			}
		}
		break;
	case eWEAPONTYPE_ETHERWEAPON:
		{
		}
		break;
	}
}

BOOL Player::CanBeAttackTarget()
{
	BOOL bFlag = Character::CanBeAttackTarget();

	// 공격가능이라도 무적상태이면 공격불가.
	if (bFlag)
	{
		if (IsInvincible())
		{
			bFlag = FALSE;
		}
	}

	return bFlag;
}

void Player::SetPartVariation(int iPart,int iVariation)
{
	if (iPart < 0 || iPart >= PLAYER_VARIATION_MAX)
	{
		return;
	}

	switch (iPart)
	{
	case PLAYER_VARIATION_HAIR:
		GetCharInfo()->m_byHair = iVariation;
		break;
	case PLAYER_VARIATION_FACE:
		GetCharInfo()->m_byFace = iVariation;
		break;
	}

    Character::SetPartVariation(iPart, iVariation);
}

void Player::SetHeightVariation(int iIndex)
{
    Character::SetHeightVariation(iIndex);
	
	GetCharInfo()->m_byHeight = GetHeightVariation();
}

BOOL Player::HasHPInfo()
{
	return m_bHPInfo;
}

#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
BOOL Player::HasSDInfo()
{
    return m_bSDInfo;
}
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

void Player::SetHPInfo(HPINFO *pInfo)
{
	Character::SetHPInfo(pInfo);

	GetCharInfo()->m_LV	    = (int)pInfo->m_LV;	

	GetCharInfo()->m_iHP 	= (int)pInfo->m_HP;
	GetCharInfo()->m_iMaxHP	= (int)pInfo->m_MaxHP;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    GetCharInfo()->m_iSD	= (int)pInfo->m_SD;
    GetCharInfo()->m_iMaxSD	= (int)pInfo->m_MaxSD;
    SetSD(pInfo->m_SD);
    m_bSDInfo = TRUE;
#endif //_NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
	SetHP( pInfo->m_HP );	

	m_bHPInfo = TRUE;
}

WzID	Player::GetCurrentCamAnimation()
{
	return StrToWzID("0000");
}

int Player::GetShotCount()
{
	if (GetWeaponKind() == eITEMTYPE_ONEHANDCROSSBOW)
	{
		return 2;
	}
	else
	{
		return 1;
	}
}

//------------------------------------------------------------------------------
/**
*/

TCHAR*  Player::GetGuildName()
{
	return m_GuildRenderInfo.m_tszGuildName;
}

WzColor  Player::GetEstablisherColor()
{
	int iItemCode;
	WzColor color = c_VendorColor;

    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return color;
    }

	const int ACCESSORY_COUNT = 3;
	for (int a = 0; a < ACCESSORY_COUNT; a++)
	{
		SCSlot& rSlot = equipment_container->GetSlot(EquipmentContainer::EQUIP_SACCESSORY1  + a);

		iItemCode = rSlot.GetCode();

		switch(iItemCode)
		{
			// 푸른색 간판 
		case 3024:
        case 49518:
			color = WzColor_RGBA(0,198,255,255);
			break;	
		case 3025:
        case 49519:
			color = WzColor_RGBA(255,238,0,255);
			break;	
		case 3026:
        case 49520:
			color = WzColor_RGBA(240,0,255,255);
			break;	
		case 3027:
        case 49521:
			color = WzColor_RGBA(0,255,73,255);
			break;	
		case 3028:
        case 49522:
			color = WzColor_RGBA(241,48,0,255);
			break;

		}
	}	

	return color;	
}

void Player::SetMarketTitle(const char * pszName, int iLen)
{
	TCHAR szTemp[256];
	
	ZeroMemory(szTemp,sizeof(szTemp));
	StrnCopy(szTemp,pszName,iLen);
	szTemp[iLen] = 0;

	m_strMarketTitle = util::_tstring(szTemp);
	
}

TCHAR* Player::GetMarketTitle() const
{
	return const_cast<TCHAR *>( m_strMarketTitle.c_str() );
}


BOOL  Player::IsFlyFinishWeapon()
{
	switch(GetWeaponKind())
	{
	case eWEAPONTYPE_TWOHANDSWORD:
	case eWEAPONTYPE_TWOHANDAXE:
	case eWEAPONTYPE_ONEHANDSWORD:
	case eWEAPONTYPE_SPEAR:
	case eWEAPONTYPE_ETHERWEAPON:
    case eWEAPONTYPE_GUNTLET: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eWEAPONTYPE_POLEARM: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eWEAPONTYPE_HELLROID_1: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eWEAPONTYPE_ARCBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
		return TRUE;
		break;
	}

	return FALSE;
}


void Player::_renderinfo_GM()
{
    TCHAR szResult[MAX_PATH] = {'\0', };
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    const TCHAR* player_name = GetNameWithTitle();
#else
    const TCHAR* player_name = GetName();
#endif //_NA_003027_20111013_HONOR_SYSTEM    
	// 캐릭터 선택시 
	if ( IsShowName() == TRUE )
	{
		//	해당 플레이어가 GM 캐릭터인가?
		if( IsGM() )
		{
			//	내가 GM 캐릭터 인가?
			if( g_pHero && g_pHero->IsGM() )
			{
				//	내가 GM 이면 플레이어 레벨과 오브젝트 키를 표시
				if( g_pHero->GetAccountState() )
				{
					//	어카운트를 켰을때만 보여줌
					Sprintf(szResult, _T("%ld %s[%d]"), GetCharInfo()->m_LV, player_name, GetObjectKey());
				}
				else
				{
					Sprintf(szResult, _T("%s"), player_name);
				}
			}
			else
			{
				//	GM 캐릭터는 레벨을 표시하지 않는다
				Sprintf(szResult, _T("%s"), player_name);
			}
			m_RenderinfoColor = c_GMTextColor;
			
			RenderTextOnHeadGuild(GetGuildName(), m_RenderinfoColor, -2);           

			RenderTextOnHead(szResult,m_RenderinfoColor,0);
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
            RenderDiceOnHead();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
		}
	}
	else
	{
		// 마우스 올라간놈만 찍는다.
		if( IsShowRenderinfo() )   
		{
			if( IsGM() )
			{
				if( IsShowVedorTitle() && this->m_BehaveState == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE)
				{
					_renderinfo_VendorTitle();
				}
				else
				{
					// 캐릭터의 상태를 얻어온다. 개인상점개설 中, 1:1 거래 中.....
					_fillRenderinfoState();	

					//	내가 GM 캐릭터 인가?
					if( g_pHero && g_pHero->IsGM() )
					{
						//	내가 GM 이면 플레이어 레벨과 오브젝트 키를 표시
						if( g_pHero->GetAccountState() )
						{
							Sprintf(szResult, _T("%ld %s[%d] %s"), GetCharInfo()->m_LV, player_name, GetObjectKey(), m_szCharState);
						}
						else
						{
							if (IsPlayerHaveRenderinfoState() )
							{
								Sprintf(szResult, _T("%s %s"), player_name, m_szCharState);
							}
							else
							{
								Sprintf(szResult, _T("%s"), player_name);
							}
						}
					}
					else
					{
						//	GM 캐릭터는 레벨을 표시하지 않는다
						if (IsPlayerHaveRenderinfoState())
						{
							Sprintf(szResult, _T("%s %s"), player_name, m_szCharState);
						}
						else
						{
							Sprintf(szResult, _T("%s"), player_name);
						}
					}
					//	GM 캐릭터는 다른색으로 출력
					m_RenderinfoColor  = c_GMTextColor;
					selectRenderinfo_Color();
					
					RenderTextOnHeadGuild(GetGuildName(), m_RenderinfoColor, -2);           
					RenderTextOnHead(szResult,m_RenderinfoColor,0);
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
                    RenderDiceOnHead();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
				}

			}
		}
	}
}

BOOL Player::IsShowRenderinfo()
{
    // 다른 플레이어 이름 보이기가 on이고 Hero가 아닐 때 보인다.
    if (GAMEOPTION->GetShowPlayerName() && 
        (g_pHero && g_pHero->GetObjectKey() != GetObjectKey()))
    {
        return TRUE;
    }
    // 다른 플레이어 이름 보이기가 off이고 마우스 오버가 되어 있거나 Hero일 때는 보인다.
    else if (g_HeroInput.GetPickedObjectKey() == GetObjectKey()
        || (g_pHero && g_pHero->GetObjectKey() == GetObjectKey()))
    {
        return TRUE;
    }

	if( IsAttackBeginPlayer()||IsChao())	
	{
		return TRUE;
	}		

	return FALSE;
}

void Player::_fillRenderinfoState()
{
	m_bRenderinfoState = TRUE;
	if (this->m_BehaveState == PLAYER_BEHAVE_TRADE_STATE)
	{
		//	[1:1 거래/교환] 
		g_InterfaceManager.GetInterfaceString( eST_PLAYER_BEHAVE_TRADE_STATE, m_szCharState, INTERFACE_STRING_LENGTH );
	}
	else if (this->m_BehaveState == PLAYER_BEHAVE_NPCSHOP_STATE)
	{
		//	[상점 업무중] 
		g_InterfaceManager.GetInterfaceString( eST_PLAYER_BEHAVE_NPCSHOP_STATE, m_szCharState, INTERFACE_STRING_LENGTH );
	}
	else
	{
		Sprintf (m_szCharState, _T(""));
		m_bRenderinfoState =  FALSE;
	}
}


//------------------------------------------------------------------------------
/**
*/
BOOL Player::SetPosition( const HONGPOS& Pos)
{
	return SetPosition( Pos.wvPos, Pos.wTile);
}

//------------------------------------------------------------------------------
/**
*/
BOOL Player::SetPosition( const WzVector& wvPos ,int iTileIndex)
{
	WzVector vOldPos=m_pPathExplorer->GetPos();
	int iOldTile=m_pPathExplorer->GetTile();
	
   	BeforeRefreshTileAtt();

	BOOL rt= Character::SetPosition(wvPos,iTileIndex);

	AfterRefreshTileAtt();

	if(rt)
	{
		ShowConflictMessage();
	}
	
	//가끔 서버로 부터 갈수 없는 곳에 세우는 버그가 있다.처음 부터 유효하지 않은 좌표가 온다면 현재 복원 안됨
	if (m_dwCurTileAtt & (PTA_NO_WALK))
	{
		rt=FALSE;
	}


	if(!rt&&m_iOldTile>=0)//포지션 설정에 실패 했을때
	{		
		g_pSunTerrain->PathExp_Reset( m_pPathExplorer, vOldPos,iOldTile);//못가는 곳일때 이전 좌표로 돌림
		AfterRefreshTileAtt();	
		m_dwCurTileAtt=m_dwOldTileAtt;
			
	}
	
	return rt;
}

void Player::BeforeRefreshTileAtt()
{
	//키보드 이동 이외에 워프성 포지션 세팅
	m_iOldTile=m_pPathExplorer->GetTile();

	//현재 속성이 있다면 현재 속성 대입 없으면 새로 구함 
	
	m_dwOldTileAtt=m_dwCurTileAtt;

}

void Player::AfterRefreshTileAtt()
{
	m_iCurTile=m_pPathExplorer->GetTile();

	//타일이 바뀌었거나 초기화가 안되었을경우 
	if((m_iOldTile!=m_iCurTile)||m_iCurTile<0)
		m_dwCurTileAtt=GetCurrentTileAtt(TRUE);
}

void Player::selectRenderinfo_Color()
{
	if( IsGM() )
	{
		m_RenderinfoColor = c_GMTextColor;
	}
	else
	{
		m_RenderinfoColor = c_TextColor;
	}

	RELATIONKIND::TYPE relationKind = GameFramework::GetCurrentScene()->GetRelationKindOfCharacter( (Character *)this,g_HeroInput.lsForceAttack() );
	if (this->m_BehaveState != PLAYER_BEHAVE_IDLE_STATE)
	{
		if( this->m_BehaveState != PLAYER_BEHAVE_VENDOR_OBSERVER_STATE)
			m_RenderinfoColor = c_NoIdleTextColor;
	}

	if ( GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_CHUNTING )
	{
		uiPartyMan* partyMan =
			static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

		if(partyMan)
		{
			if(partyMan->IsPartyMember(GetObjectKey()) == FALSE)
			{
				if (relationKind == RELATIONKIND::Friend)
				{
					m_RenderinfoColor = c_TimeAttackTextColor;
				}
			}
		}		
	}
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    else if (GlobalFunc::IsBattleGroundScene() ||
             GlobalFunc::IsGoldRushScene())
#else
    else if (GlobalFunc::IsBattleGroundScene())
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    {
        if (battle_ground_hide_mode() != BattleGroundHideMode_None)
        {
            m_RenderinfoColor = c_NoIdleTextColor;
        }
        // 임시 GM 색상 풀기
        else if (relationKind == RELATIONKIND::Enemy)
        {
            m_RenderinfoColor = c_EnemyTextColor;
        }
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        else if (relationKind == RELATIONKIND::None)
        {
            // no op
        }
#endif //_NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
        else
        {
            m_RenderinfoColor = c_TextColor;
        }
        //
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
	if (relationKind == RELATIONKIND::Enemy)
	{
		//분쟁지역과 자유분쟁지역에서만 TextOnHead Color 바꾼다.
		DWORD dwAtt = GetCurrentTileAtt(FALSE);
		if(BIT_CHECK(dwAtt, PTA_CONFLICT_TILE) || BIT_CHECK(dwAtt, PTA_FREE_CONFLICT_TILE))
		{
			m_RenderinfoColor = c_EnemyTextColor;
		}
#ifdef _GS_GSP_PVP_ENEMY_COLOR
        else if(GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_PVP)
        {
            //PK타일은 아니지만  PVP라면 적색의로
            m_RenderinfoColor = c_EnemyTextColor;
        }
#endif//_GS_GSP_PVP_ENEMY_COLOR
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        else if(GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FREEPVP)
        {
            //PK타일은 아니지만  PVP라면 적색의로
            m_RenderinfoColor = c_EnemyTextColor;
        }
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
	}

	if (this->IsDead())
	{                 
		m_RenderinfoColor = c_DeadTextColor;
	}
	if( ! IsGM() )
	{
		if( this->GetChaoState()==eCHAO_STATE_PREV_CHAO )
			m_RenderinfoColor= c_FreeChaoColor;

		if ( this->IsAttackBeginPlayer() )
			m_RenderinfoColor= c_BoraDoriColor;

		if( this->GetChaoState()==eCHAO_STATE_CHAO )
			m_RenderinfoColor=c_ChaoColor;
	}

	if (GENERALPARAM->GetSpecialMode())
	{
		int y=0;
		if(this->GetChaoState()==eCHAO_STATE_PREV_CHAO)
		{

			y+=1;
			RenderTextOnHead(_T("프리카오"),m_RenderinfoColor,y,TRUE);

		}

		if(this->IsAttackBeginPlayer())
		{

			y+=1;
			RenderTextOnHead(_T("보라돌이"),m_RenderinfoColor,y,TRUE);
		}


		if(this->GetChaoState()==eCHAO_STATE_CHAO)
		{

			y+=1;
			RenderTextOnHead(_T("카오"),m_RenderinfoColor,y,TRUE);

		}
	}
	//캐릭터 선택씬이 아닐경우
	if( IsShowName() == FALSE)
	{
		// 개인상점 색깔 변경
		if (m_BehaveState == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE)
		{
			m_RenderinfoColor = GetEstablisherColor();
		}
	}

	BOOL bVirtualID = GetGMGrade() == 4;
	if( bVirtualID)
	{
		m_RenderinfoColor = c_VirtualIDColor;
	}
}


int Player::GetCharDelRemainTime()
{
	// 2006-10-12 24:03 ( 분단위 )
	if( _tcslen(m_tszDelDate) != 16 )
	{
		assert("RealTime Char del input string != 16");
		return 0;
	}
	TCHAR szNumber[5];
	tm endTM;
	// 년
	StrnCopy(szNumber , m_tszDelDate , 4);
	szNumber[4] = NULL;
	endTM.tm_year = _ttoi(szNumber) - 1900;
	// 월
	StrnCopy(szNumber, &m_tszDelDate[5], 2);
	szNumber[2] = NULL;
	endTM.tm_mon = _ttoi(szNumber) - 1;
	// 일
	StrnCopy(szNumber , &m_tszDelDate[8] , 2);
	szNumber[2] = NULL;
	endTM.tm_mday = _ttoi(szNumber) + CHAR_DEL_DAYS_OVER_40_LEVEL ;

	// 시
	StrnCopy(szNumber , &m_tszDelDate[11] , 2);
	szNumber[2] = NULL;
	endTM.tm_hour = _ttoi(szNumber);
	//분
	StrnCopy(szNumber, &m_tszDelDate[14], 2);
	szNumber[2] = NULL;
	endTM.tm_min = _ttoi(szNumber);

	endTM.tm_sec = 0;

	time_t curTime , endTime = mktime(&endTM);
	util::TimeSync::time(&curTime);
	return (int) difftime(endTime , curTime );
}

TCHAR* Player::GetCharDelRemainTimeString(TCHAR* string)
{
    int RemainTotalTime = GetCharDelRemainTime();
    int day = 0 , hour = 0 , minute = 0, second = 0;

    int remainder = RemainTotalTime;
    // 1일 보다 큰가
    if( RemainTotalTime >= 86400)
    {
        day = RemainTotalTime / 86400;
        remainder = RemainTotalTime % 86400;
    }
    // 1시간 보다 큰가
    if( RemainTotalTime >= 3600 )
    {
        hour = remainder / 3600;
        remainder = RemainTotalTime % 3600;
    }
    // 분 단위 인가
    minute = remainder / 60;

    second = remainder % 60;

    Snprintf(string , (INTERFACE_STRING_LENGTH -1), "[%s%d:%s%d:%s%d:%s%d]", 
        (day<10)?"0":"", day, (hour<10)?"0":"", hour, (minute<10)?"0":"", minute, (second<10)?"0":"", second);

    return string;
}

void Player::_renderinfo_Remain_Char_Del()
{
	if( m_DelChk > 0 )
	{
		TCHAR szMessage[INTERFACE_STRING_LENGTH] = {0,};
		// 5038 삭제 대기중
        TCHAR szMessage1[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR time_string[INTERFACE_STRING_LENGTH] = {0,};

        GetCharDelRemainTimeString(time_string);
        g_InterfaceManager.GetInterfaceString(5038 , szMessage1 ,INTERFACE_STRING_LENGTH);
        Snprintf(szMessage, INTERFACE_STRING_LENGTH, "%s %s", szMessage1, time_string);

		RenderTextOnHead(szMessage , WzColor_RGBA(220,0,0,255),1, FALSE, true);
	}
}

BOOL Player::IsShowVedorTitle()
{
	BOOL bOnlyMouseOver = ( !GAMEOPTION->GetShowPlayerName() && ( g_HeroInput.GetPickedObjectKey() == GetObjectKey() ));
	if( GAMEOPTION->GetDrawMarketTitle()  ||  bOnlyMouseOver )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void Player::_renderinfo_VendorTitle()
{
	TCHAR szResult[INTERFACE_STRING_LENGTH] = {0,};
#ifdef _NA_003027_20111013_HONOR_SYSTEM
    const TCHAR* player_name = GetNameWithTitle();
#else
    const TCHAR* player_name = GetName();
#endif //_NA_003027_20111013_HONOR_SYSTEM 
    if( IsExistGuild() && GAMEOPTION->GetShowGuildName() )
	{
		Snprintf(szResult , INTERFACE_STRING_LENGTH -1 , _T("< %s > %s") , GetGuildName() , player_name);
	}
	else
	{
		Snprintf(szResult ,INTERFACE_STRING_LENGTH -1, _T("%s") , player_name);
	}

	selectRenderinfo_Color();
	RenderTextOnHead(szResult , m_RenderinfoColor , 0);
	memset(szResult , 0 , sizeof(szResult));
	Snprintf(szResult , INTERFACE_STRING_LENGTH -1 , _T("%s") ,GetMarketTitle() );
	RenderTextOnHead(szResult , m_RenderinfoColor , -1);
}
void Player::renderInfo_InCharacterSenen()
{
    _renderinfo_Remain_Char_Del();
}

void Player::renderInfo_InBattleScene()
{
	TCHAR szResult[INTERFACE_STRING_LENGTH] ={0,};
	if( IsShowRenderinfo() )
	{
		if( IsShowVedorTitle() && this->m_BehaveState == PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE )
		{
			_renderinfo_VendorTitle();
		}
		else
		{
			// 캐릭터의 상태를 얻어온다.
#ifdef _NA_003027_20111013_HONOR_SYSTEM
            const TCHAR* player_name = GetNameWithTitle();
#else
            const TCHAR* player_name = GetName();
#endif //_NA_003027_20111013_HONOR_SYSTEM 

			_fillRenderinfoState(); 
			if (IsPlayerHaveRenderinfoState() )
				Sprintf(szResult, _T("%s %s"), player_name, m_szCharState);
			else
				Sprintf(szResult, _T("%s"), player_name);
			selectRenderinfo_Color();
			renderInfo_Guild();
			RenderTextOnHead(szResult,m_RenderinfoColor,0);
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
            RenderDiceOnHead();
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
		}
	}
}

void Player::renderInfo_Guild()
{
	if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ)
#ifdef _NA_004756_120504_SHOW_SSQ_PLAYER_NAME
        && ((g_pHero->IsGM() == FALSE) || (GENERALPARAM->IsShowSSQPlayerName() == FALSE))
#endif //_NA_004756_120504_SHOW_SSQ_PLAYER_NAME
        )
    {
		return;
    }


	RenderTextOnHeadGuild(GetGuildName(), m_RenderinfoColor, -2); 
}

BOOL Player::IsGM()
{
	if( m_byGMGrade > 0  && m_byGMGrade < 4 )
	{
		return TRUE;
	}

	return FALSE;
}

void Player::GetClassExName(OUT TCHAR * pszOutString)
{
	if(FALSE == GENERALPARAM->IsNet())
	{
		Snprintf(pszOutString,MAX_CLASSNAME_LENGTH, _T("테스트 캐릭"));
		return;
	}

	if( !GetCharInfo())
		return;

	switch ( GetCharInfo()->GetChangeOfClass() )
	{
	case eCHAREX_BERSERKER_100LEVEL:
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_EX_NAME_BERSERKER, pszOutString, MAX_CLASSNAME_LENGTH );
		break;
	case eCHAR_BERSERKER:
		//	버서커
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_BERSERKER, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

	case eCHAREX_DRAGON_KNIGHT_100LEVEL:
	case eCHAREX_DRAGON_1stTRANS_100LEVEL:
	case eCHAREX_DRAGON_2ndTRANS_100LEVEL:
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_EX_NAME_DRAGON, pszOutString, MAX_CLASSNAME_LENGTH );
		break;
	case eCHAR_DRAGON:
	case eCHAREX_DRAGON_1stTRANS:
	case eCHAREX_DRAGON_2ndTRANS:
		//	드라곤
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_DRAGON, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

	case eCHAREX_SHADOW_100LEVEL:
	case eCHAR_SHADOW:
		//	셰도우
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_SHADOW, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

	case eCHAREX_VALKYRIE_100LEVEL:
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_EX_NAME_VALKYRIE, pszOutString, MAX_CLASSNAME_LENGTH );
		break;
	case eCHAR_VALKYRIE:
		//	발키리
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_VALKYRIE, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

	case eCHAREX_ELEMENTALIST_100LEVEL:
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_EX_NAME_MAGICIAN, pszOutString, MAX_CLASSNAME_LENGTH );
		break;
	case eCHAR_MAGICIAN:
		//	엘리멘탈 리스트
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_MAGICIAN, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

    case eCHAREX_MYSTIC_100LEVEL:
    case eCHAR_MYSTIC:
        // 미스틱 //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_MYSTIC, pszOutString, MAX_CLASSNAME_LENGTH );
        break; 
    case eCHAREX_HELLROID_100LEVEL:
    case eCHAR_HELLROID:
        // 헬로이드 //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_HELLROID, pszOutString, MAX_CLASSNAME_LENGTH );
        break;
    case eCHAREX_WITCHBLADE_100LEVEL:
    case eCHAR_WITCHBLADE:
        // 위치블레이드 //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_WITCHBLADE, pszOutString, MAX_CLASSNAME_LENGTH );
        break;
	default:
		{
			Snprintf(pszOutString,MAX_CLASSNAME_LENGTH, _T("정의되지않음"));
			assert(! "클레스값이 잘못 들어옴 ");
		}
		break;
	}
}




void Player::RenderTextOnHeadGuild(TCHAR *szString, WzColor color, int yoffset)
{
	if ((GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ)
#ifdef _NA_004756_120504_SHOW_SSQ_PLAYER_NAME
        && ((g_pHero->IsGM() == FALSE) || (GENERALPARAM->IsShowSSQPlayerName() == FALSE))
#endif //_NA_004756_120504_SHOW_SSQ_PLAYER_NAME
        )
    {
		return;
    }

	if (SCENE_TYPE_CHARSELECT == GameFramework::GetCurScene())
    {
		return;
    }

	if (SCENE_TYPE_NONE == GameFramework::GetCurScene())
    {
		return;
    }

	// 대화를 표시하고 있는경우 패스
	if ( GetChatDisplayTime() )	
	{
		return;
	}

	if (m_bObserverMode)
	{
		return;
	}

	if (!GAMEOPTION->GetShowGuildName())
    {
		return;
    }

	TCHAR szGuildString[INTERFACE_STRING_LENGTH] = { 0,};
	Snprintf (szGuildString,INTERFACE_STRING_LENGTH-1, _T("< %s >"), szString);

	// fix
	yoffset = -1;

	if (g_InterfaceManager.GetShowFlag()) 
	{

		WzVector wzVisualPos = GetVisiblePos();
		WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
		g_pSunRenderer->Set2DMode( TRUE );
		g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m217") );

		WzVector wzPos = GetPosition();				
		wzVisualPos.z += GetMeshHeight();
        wzVisualPos.z += GetHeight();
		wzVisualPos.z += HEIGHT_GAP;

        //if (is_spread_wings())
        //{
        //    //! 날개 상태이면 위로 좀더 올려 준다.
        //    wzVisualPos.z += 1.4f;
        //}

		POINT pt2;
		float fDepth;
		g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos , &fDepth );


		
		SIZE textsizeGuild, textSizeName;
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szGuildString,lstrlen(szGuildString),&textsizeGuild);
		
		RECT rc;
		rc.left = pt2.x - (textsizeGuild.cx / 2);
		rc.top = pt2.y + (yoffset * textsizeGuild.cy ) - 2 ;
		rc.right = rc.left + textsizeGuild.cx;
		rc.bottom = rc.top + 50;
		WzColor BackColor = WzColor_RGBA(0,0,0,100);	
		
#ifdef _NA_003027_20111013_HONOR_SYSTEM
        const TCHAR* player_name = GetNameWithTitle();
#else
        const TCHAR* player_name = GetName();
#endif //_NA_003027_20111013_HONOR_SYSTEM 

		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint((TCHAR*)player_name, lstrlen(player_name), &textSizeName);// 글로벌 엔진버전이 낮아서 error발생 (TCHAR*)필요 추후 제거 요망

		SIZE BigTextSize = textsizeGuild.cx > textSizeName.cx ? textsizeGuild : textSizeName;
	

		RECT rtGuildMark;
		//길드마크 테두리 이미지의 사이즈가 커지면서
		//좌우대칭을 위해 길드마크의 위치를 좌측으로 약간 밀었다.
		rtGuildMark.left = pt2.x - 24 - (BigTextSize.cx / 2) - 10;
		rtGuildMark.top = pt2.y - 12 - 1;
		rtGuildMark.right = pt2.x - (BigTextSize.cx / 2) - 10;
		rtGuildMark.bottom = pt2.y + 12 - 1;

        eGUILD_MARK_RELATION eRelation = eGUILD_MARK_RELATION_EMPTY;
        BASE_GuildMarkInfo* pMarkInfo_Relation = NULL;


#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        if (false)
        {
            // 그냥 에러방지용
        }
#else
        if ((SCENE_TYPE_DOMINATION == GameFramework::GetCurScene()) &&
            (IsExistGuild()) &&
            (g_pHero->GetGuildRenderInfo().m_GuildGuid != m_GuildRenderInfo.m_GuildGuid))
        {
            eRelation = eGUILD_MARK_RELATION_RED;
            pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_RED);
        }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        else if (SCENE_TYPE_BATTLEGROUND == GameFramework::GetCurScene() ||
                 SCENE_TYPE_GOLDRUSH == GameFramework::GetCurScene())
#else
        else if (SCENE_TYPE_BATTLEGROUND == GameFramework::GetCurScene())
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        {
            //if (this->GetTeam() == INVALID_TEAM)
            {
                eRelation = eGUILD_MARK_RELATION_EMPTY;
                pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_EMPTY);
            }
            /*else if (g_pHero->GetTeam() == this->GetTeam())
            {
                eRelation = eGUILD_MARK_RELATION_BLUE;
                pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
            }
            else
            {
                eRelation = eGUILD_MARK_RELATION_RED;
                pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_RED);
            }*/
        }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        else if (SCENE_TYPE_FREEPVP == GameFramework::GetCurScene())
        {
            eRelation = eGUILD_MARK_RELATION_EMPTY;
            pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_EMPTY);
        }
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        else
        {
            switch (m_GuildRenderInfo.m_RelationType)
            {
            case eGUILD_RELATION_ALLIENCE:
                {
                    if (g_pHero)
                    {
                        //길드 등급만 체크 한다.(개인 직위는 체크 생략)
                        if (GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_RELATION, 
                            m_GuildRenderInfo.m_GuildGrade))
                        {
                            //3레벨 이상일 경우 무조건 출력
                            eRelation = eGUILD_MARK_RELATION_BLUE;
                            pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
                        }
                        else
                        {
                            //3레벨 이하일 경우 길드마크가 있을때만 출력
                            if (IsExistGuildMark())
                            {
                                if ((g_pHero->GetGuildRenderInfo().m_GuildGuid == m_GuildRenderInfo.m_GuildGuid)) //Hero와 같은 길드다
                                {
                                    eRelation = eGUILD_MARK_RELATION_BLUE;
                                    pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
                                }
                                else
                                {
                                    eRelation = eGUILD_MARK_RELATION_EMPTY;
                                    pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_EMPTY);
                                }
                            }
                        }
                    }
                }
                break;
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
            case eGUILD_RELATION_BOTH_HOSTILITY:        
                {
                    eRelation = eGUILD_MARK_RELATION_RED;
                    pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_RED);
                }
                break;
            case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
            case eGUILD_RELATION_PROCLAIM_HOSTILITY:
                {
                    eRelation = eGUILD_MARK_RELATION_ORANGE;
                    pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_ORANGE);
                }
                break;
#else 
            case eGUILD_RELATION_BOTH_HOSTILITY:
            case eGUILD_RELATION_PROCLAIM_HOSTILITY:
                {
                    eRelation = eGUILD_MARK_RELATION_RED;
                    pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_RED);
                }
                break;
            case eGUILD_RELATION_ONE_SIDE_HOSTILITY:
                {
                    eRelation = eGUILD_MARK_RELATION_ORANGE;
                    pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_ORANGE);
                }
                break;
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL		

            case eGUILD_RELATION_NONE:
            default:
                {
                    if (GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_RELATION, 
                        m_GuildRenderInfo.m_GuildGrade))
                    {
                        if((g_pHero->GetGuildRenderInfo().m_GuildGuid == m_GuildRenderInfo.m_GuildGuid)) //Hero와 같은 길드다
                        {
                            eRelation = eGUILD_MARK_RELATION_BLUE;
                            pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
                        }
                        else
                        {
                            eRelation = eGUILD_MARK_RELATION_EMPTY;
                            pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_EMPTY);
                        }
                    }
                    else
                    {
                        if( IsExistGuildMark() && g_pHero)
                        {
                            if((g_pHero->GetGuildRenderInfo().m_GuildGuid == m_GuildRenderInfo.m_GuildGuid)) //Hero와 같은 길드다
                            {
                                eRelation = eGUILD_MARK_RELATION_BLUE;
                                pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
                            }
                            else
                            {
                                eRelation = eGUILD_MARK_RELATION_EMPTY;
                                pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_EMPTY);
                            }
                        }
                    }
                }
                break;
            }
        }

		if(pMarkInfo_Relation)
		{
			g_InterfaceManager.RenderGuildMarkBackGround(rtGuildMark, fDepth, eRelation);
			g_InterfaceManager.RenderCharNameBackGround(rtGuildMark.right, rtGuildMark.top, BigTextSize.cx, rtGuildMark.bottom - rtGuildMark.top, fDepth, eRelation, true, true);
		}
		else
		{
			if(TRUE == IsExistGuild())
				g_InterfaceManager.RenderCharNameBackGround(rtGuildMark.right, rtGuildMark.top, BigTextSize.cx, rtGuildMark.bottom - rtGuildMark.top, fDepth, eRelation, true, false);
			else
				g_InterfaceManager.RenderCharNameBackGround(rtGuildMark.right, rtGuildMark.top, BigTextSize.cx, rtGuildMark.bottom - rtGuildMark.top, fDepth, eRelation, false, false);
		}

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        if (GlobalFunc::IsBattleGroundScene() == TRUE || 
            GlobalFunc::IsGoldRushScene() == TRUE)
#else
        if (GlobalFunc::IsBattleGroundScene() == TRUE)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        {
            DWORD pattern_index = 0;
            // 전장 팀 마크 출력 (길드마크 대신)
            switch (GetTeam())
            {
            case kRed:
                {
                    pattern_index = 285;
                }
                break;
            case kBlue:
                {
                    pattern_index = 286;
                }
                break;
            default:
                {
                    if (g_pHero && IsPartyMember()) // 팀 세팅되기 전에  파티원이면 팀마크 띄워주자
                    {
                        if (g_pHero->GetTeam() == kRed)
                        {
                            pattern_index = 285;
                        }
                        else
                        {
                            pattern_index = 286;
                        }
                    }
                }
                break;
            }

            if (pattern_index != 0)
            {
                BASE_GuildMarkInfo* mark_info = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(pattern_index);
                if (mark_info)
                {
                    g_InterfaceManager.RenderRectWithTextureAndZTest(rtGuildMark, fDepth, mark_info->iRow, mark_info->iColumn);
                }
            }
            else
            {
                if(  IsExistGuildMark() )
                {
                    //외곽선 드러나게
                    rtGuildMark.bottom -= 3;
                    rtGuildMark.top	   += 3;
                    rtGuildMark.left   += 3;
                    rtGuildMark.right  -= 3;

                    BASE_GuildMarkInfo * pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(m_GuildRenderInfo.m_BackGroundIndex);
                    if( pMarkInfo)
                    {
                        g_InterfaceManager.RenderRectWithTextureAndZTest(rtGuildMark,fDepth, pMarkInfo->iRow, pMarkInfo->iColumn);
                    }

                    pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(m_GuildRenderInfo.m_PatternIndex);
                    if( pMarkInfo )
                    {
                        g_InterfaceManager.RenderRectWithTextureAndZTest(rtGuildMark,fDepth, pMarkInfo->iRow, pMarkInfo->iColumn);
                    }
                }
            }

        }
        else
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
		if(  IsExistGuildMark() )
		{
#ifdef _NA_000000_20130828_GUILDMARK_SIZE_MODIFY
            //외각선 안보이게 (크게)
#else
            //외곽선 드러나게
			rtGuildMark.bottom -= 3;
			rtGuildMark.top	   += 3;
			rtGuildMark.left   += 3;
			rtGuildMark.right  -= 3;
#endif //_NA_000000_20130828_GUILDMARK_SIZE_MODIFY

			BASE_GuildMarkInfo * pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(m_GuildRenderInfo.m_BackGroundIndex);
			if( pMarkInfo)
			{
				g_InterfaceManager.RenderRectWithTextureAndZTest(rtGuildMark,fDepth, pMarkInfo->iRow, pMarkInfo->iColumn);
			}

			pMarkInfo = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(m_GuildRenderInfo.m_PatternIndex);
			if( pMarkInfo )
			{
				g_InterfaceManager.RenderRectWithTextureAndZTest(rtGuildMark,fDepth, pMarkInfo->iRow, pMarkInfo->iColumn);
			}
		}

		if (this->IsExistGuild())
			RenderBlockedTextWithDepthTest(szGuildString,&rc,color,BackColor,fDepth, true);

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH        
        if (SCENE_TYPE_GOLDRUSH == GameFramework::GetCurScene())
        {
            RECT right_rect;
            right_rect.left = pt2.x + (BigTextSize.cx / 2) + 9/*여분*/;
            right_rect.right = pt2.x + 24 + (BigTextSize.cx / 2) + 9;
            right_rect.top = rtGuildMark.top;
            right_rect.bottom = rtGuildMark.bottom;
            
            RenderGoldMarkOnHead(right_rect, fDepth, eRelation);
        }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
		if ( SCENE_TYPE_GOLDRUSH != GameFramework::GetCurScene() && SCENE_TYPE_DOMINATION != GameFramework::GetCurScene()
#ifdef _NA_008334_20150608_SONNENSCHEIN
            && SCENE_TYPE_SONNENSCHEIN != GameFramework::GetCurScene()
#endif //_NA_008334_20150608_SONNENSCHEIN
            )
		{
			if ( GetClientPlayerRankingGradeInfo().grade != eSUN_RANKING_GRADE_NONE )
			{
				RECT right_rect;
				right_rect.left = pt2.x + (BigTextSize.cx / 2) + 9/*여분*/;
				right_rect.right = pt2.x + 24 + (BigTextSize.cx / 2) + 9;
				right_rect.top = rtGuildMark.top;
				right_rect.bottom = rtGuildMark.bottom;

				RenderRankingGradeOnHead(right_rect, fDepth, eRelation);
			}
		}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        if((SCENE_TYPE_DOMINATION == GameFramework::GetCurScene()
#ifdef _NA_008334_20150608_SONNENSCHEIN
            || SCENE_TYPE_SONNENSCHEIN == GameFramework::GetCurScene()
#endif //_NA_008334_20150608_SONNENSCHEIN
            )
            && this->IsExistGuild())
        {            
            RECT right_rect;
            right_rect.left = pt2.x + (BigTextSize.cx / 2) + 9/*여분*/;
            right_rect.right = pt2.x + 24 + (BigTextSize.cx / 2) + 9;
            right_rect.top = rtGuildMark.top;
            right_rect.bottom = rtGuildMark.bottom;

            RenderDominationMarkOnHead(right_rect, fDepth, eRelation);
        }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

		g_pSunRenderer->x_pManagerTextOut->EndUseFont();

		g_pSunRenderer->Set2DMode(FALSE);
		g_pSunRenderer->SetColor(oldcolor);
	}
}

#ifdef _NA_003027_20111013_HONOR_SYSTEM
void Player::SetTitleName(WORD title_code)
{
    bool is_success = false;

    BASE_PLAYERINFO* base_player_info = GetCharInfo();
    if (base_player_info != NULL)
    {
        base_player_info->honor_title_ = title_code;
    }

    if (title_code != 0)
    {
        TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(title_code);
        if (title_info != NULL)
        {
            const TCHAR* empty_char = _T("  ");
            TCHAR title_name[INTERFACE_STRING_LENGTH] = {'\0', };
            g_InterfaceManager.GetInterfaceString(title_info->title_name_code, title_name);

            if (title_info->title_display_type == TitleListParser::kPrefixTitle)
            {
                name_with_title_ = title_name;
                name_with_title_ += empty_char;
                name_with_title_ += GetName();

                is_success = true;
            }
            else if (title_info->title_display_type == TitleListParser::kPostfixTitle)
            {
                name_with_title_ = GetName();
                name_with_title_ += empty_char;
                name_with_title_ += title_name;

                is_success = true;
            }
        }
    }
    
    if (is_success == false)
    {
        name_with_title_ = GetName();
    }
}

WORD Player::GetTitleCode()
{
    WORD title_code = 0;
    
    BASE_PLAYERINFO* base_player_info = GetCharInfo();
    if (base_player_info != NULL)
    {
        title_code = base_player_info->honor_title_;
    }

    return title_code;
}

#endif //_NA_003027_20111013_HONOR_SYSTEM

void Player::CreateEtherWeaponEffect(bool bShotEffect, bool bFillupEffect)
{
	if (GlobalFunc::IsSSQScene())
    {
		return;
    }

	//에테르 충전시, 무기에 루프 이펙트가  계속 나온다.
	if (!bShotEffect)
    {
		DestoryEtherWeaponEffect();
    }

	BOOL IsTransfom = GetTransSkillCode();

	if ( GetAttachmentCount() == 0 )
	{
		if (!IsTransfom)
        {
            return;
        }
	}

	DWORD dwWeaponKind = GetWeaponKind();
	
	if (IsTransfom)
    {
		dwWeaponKind = eWEAPONTYPE_ONEHANDSWORD;
    }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    BASE_EtherWeaponEffect* pInfo = EtherWeaponEffectInfoParser::Instance()->GetEtherWeponEffectInfo(MAKEDWORD(dwWeaponKind, ETHER_HAND_FIRST));
#else
	BASE_EtherWeaponEffect* pInfo = EtherWeaponEffectInfoParser::Instance()->GetEtherWeponEffectInfo(dwWeaponKind);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	//무기에 대한 에테르이펙트스크립트 정보없다면 리턴
	if (!pInfo) 
    {
        return;
    }

	
	//예외처리구문: 특정무기는 두개를 붙여야된다(쉐도우 대거,드래곤나이트의 쌍칼)
	
	
	//기본적으로 무기는 1개	
	int nWeaponNum = 1; 

		switch(dwWeaponKind)
		{
		case eWEAPONTYPE_ONEHANDCROSSBOW:
		case eWEAPONTYPE_ONEHANDSWORD:
		case eWEAPONTYPE_DAGGER:
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        case eWEAPONTYPE_ARCBLADE:
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
			{
				nWeaponNum = 2;
			}
			break;
		}

	
	//루프를돌면서(왼쪽손,오른쪽손)
	for(int i=0;i<nWeaponNum;++i)
	{
        if (i >= (int)GetAttachmentCount())
        {
            break;
        }

		//STEP1:무기부착및 무기유닛정보
		CWzUnitDraw * pWeaponUnitDraw = NULL;

		pWeaponUnitDraw = (IsTransfom) ? (GetBasePart()) : (GetAttachment(i).GetUnitDraw());
		
		if(IsTransfom)
		{
			pWeaponUnitDraw = GetBasePart();
		}

		if( ! pWeaponUnitDraw)	return;


		CWzUnit * pWzUnit = pWeaponUnitDraw->GetWzUnit();
		if( !pWzUnit)return;

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        pInfo = EtherWeaponEffectInfoParser::Instance()->GetEtherWeponEffectInfo(MAKEDWORD(dwWeaponKind, (i + 1)));
        if (!pInfo)
        {
            pInfo = EtherWeaponEffectInfoParser::Instance()->GetEtherWeponEffectInfo(MAKEDWORD(dwWeaponKind, ETHER_HAND_FIRST));
        }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

		WzBoneInfo * pBoneInfo = pWzUnit->GetBoneInfo(pInfo->dwTargetBone);
		
		if(IsTransfom)
		{
			WzID wzID  = i ? (StrToWzID("LHND")):(StrToWzID("RHND"));
			pWzUnit->GetBoneInfo(wzID);
		}

		if( !pBoneInfo) return;


		//STEP:넘어온 인자값에따라 이펙트셋팅
		if( bShotEffect )
		{
			//발사 이펙트
			m_hEtherWeaponEffect[i][2] = GlobalFunc::CreateEffectBeUpToOption(pInfo->dwEffectCode[ETHER_EFFECT_SHOT],
				pWeaponUnitDraw->GetBonePos(pBoneInfo->m_iNode));
		}
		else
		{
			if( !bFillupEffect) continue;
			
			int BulletIndex = GetEtherBulletEffectIndex();
			//충전 지속 이펙트(루프)	
			m_hEtherWeaponEffect[i][0] = GlobalFunc::CreateEffectBeUpToOption(pInfo->dwEffectCode[BulletIndex], 
				pWeaponUnitDraw->GetBonePos(pBoneInfo->m_iNode));

			//충전시순간이펙트(단발용)
			m_hEtherWeaponEffect[i][1] = GlobalFunc::CreateEffectBeUpToOption(pInfo->dwEffectCode[ETHER_EFFECT_CHARGEING], 
				pWeaponUnitDraw->GetBonePos(pBoneInfo->m_iNode));
		}

		UpdateEtherWeaponEffectPos();
	}
}

void Player::UpdateEtherWeaponEffectPos()
{
	if (GlobalFunc::IsSSQScene())
    {
		return;
    }

	if (!IsEtherDeviecBulletChargeing())
	{
		DestoryEtherWeaponEffect();
		return;
	}

	BOOL IsTransfom = GetTransSkillCode();

	if ( GetAttachmentCount() == 0 )
	{
		if (!IsTransfom)
        {
            return; 
        }
	}

	DWORD dwWeaponKind = GetWeaponKind();

	if (IsTransfom)
	{
		dwWeaponKind = eWEAPONTYPE_ONEHANDSWORD;
	}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    BASE_EtherWeaponEffect* pInfo = EtherWeaponEffectInfoParser::Instance()->GetEtherWeponEffectInfo(MAKEDWORD(dwWeaponKind, ETHER_HAND_FIRST));
#else
	BASE_EtherWeaponEffect* pInfo = EtherWeaponEffectInfoParser::Instance()->GetEtherWeponEffectInfo(dwWeaponKind);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

	//스크립트 정보없으면 리턴
	if (!pInfo) 
    {
        return;
    }
	
	int nWeaponNum = 1; //기본적으로 무기는 1개

	switch (dwWeaponKind)
	{
	case eWEAPONTYPE_ONEHANDCROSSBOW:
	case eWEAPONTYPE_ONEHANDSWORD:
	case eWEAPONTYPE_DAGGER:
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eWEAPONTYPE_ARCBLADE:
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
			{
				nWeaponNum = 2;
			}
			break;
	}

	
	for (int i = 0; i < nWeaponNum; ++i)
	{
        if (i >= (int)GetAttachmentCount())
        {
            break;
        }

		CWzUnitDraw* pWeaponUnitDraw = NULL;
		
		pWeaponUnitDraw = (IsTransfom) ? (GetBasePart()) : (GetAttachment(i).GetUnitDraw());

		if (!pWeaponUnitDraw)
        {
            return;
        }

		CWzUnit* pWzUnit = pWeaponUnitDraw->GetWzUnit();
		if (!pWzUnit)
        {
            return;
        }

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        pInfo = EtherWeaponEffectInfoParser::Instance()->GetEtherWeponEffectInfo(MAKEDWORD(dwWeaponKind, (i + 1)));
        if (!pInfo)
        {
            pInfo = EtherWeaponEffectInfoParser::Instance()->GetEtherWeponEffectInfo(MAKEDWORD(dwWeaponKind, ETHER_HAND_FIRST));
        }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

		WzBoneInfo* pBoneInfo = pWzUnit->GetBoneInfo(pInfo->dwTargetBone);
		
		if (IsTransfom)
		{
			WzID wzID  = i ? (StrToWzID("LHND")) : (StrToWzID("RHND"));
			pBoneInfo = pWzUnit->GetBoneInfo(wzID);	
		}

		if (pBoneInfo)
		{
			WzMatrix* pwzmTransform = pWeaponUnitDraw->GetTransformBuffer();
			if (!pwzmTransform)
            {
				return;
            }

			WzMatrix wmOffsetRot, wmRot, wmPosMat, wmScale;

			MatrixIdentity(&wmOffsetRot);

			//Rotation
			WzVector vRot;
			SetVector(&vRot, n_deg2rad(pInfo->wvOffsetRot.x), n_deg2rad(pInfo->wvOffsetRot.y), n_deg2rad(pInfo->wvOffsetRot.z));
			MatrixRotationZYX(&wmOffsetRot, &vRot);
			MatrixMultiply(&wmRot, &wmOffsetRot, &pwzmTransform[pBoneInfo->m_iNode]);

			//Position
			WzVector vOffsetPos;
			VectorTransformCoord(&vOffsetPos, &pInfo->wvOffsetPos, &wmRot);
			MatrixTranslation(&wmPosMat, vOffsetPos.x, vOffsetPos.y, vOffsetPos.z);
            //두번적용못하게
			wmRot._41=0;
            wmRot._42=0;
            wmRot._43=0; 
	
			if (IsTransfom)
			{
				WzVector vScale;
				SetVector(&vScale, 0.5f, 1.f, 0.3f);
				MatrixScaling(&wmScale, vScale.x, vScale.y, vScale.z);
			}
			else
            {
				MatrixScaling(&wmScale, pInfo->wvScale.x ,pInfo->wvScale.y, pInfo->wvScale.z);
            }


			for (int row = 0; row < 3; ++row)
			{
				if (m_hEtherWeaponEffect[i][row] == INVALID_HANDLE_VALUE)
                {
					continue;
                }

				CEffectBody* pBody = g_pEffectManager->GetEffectPtrHandle(m_hEtherWeaponEffect[i][row]);
				if (!pBody)
                {
					continue;
                }

				pBody->SetResultMatrix(&wmPosMat, &wmRot, &wmScale); 
			}

		}

	}

	
	
}

void Player::DestoryEtherWeaponEffect()
{
	for(int i=0;i<2;++i)
	{
		if( m_hEtherWeaponEffect[i][0] != INVALID_HANDLE_VALUE )
		{
			g_pEffectManager->RemoveEffectPtr(m_hEtherWeaponEffect[i][0]);
			m_hEtherWeaponEffect[i][0] = INVALID_HANDLE_VALUE;
		}

	}
}

bool Player::IsEtherDeviecBulletChargeing()
{
	return ((m_EtherIndex == 0) ? false : true);
}

int Player::GetEtherBulletEffectIndex()
{
	int index = ETHER_EFFECT_METAL_BULLET;
	int iBulletIndex = m_EtherIndex;

	switch (iBulletIndex)
	{
	case 1:
		{
			index = ETHER_EFFECT_METAL_BULLET;
		}
		break;

	case 2:
		{
			index = ETHER_EFFECT_ETHER_BULLET;
		}
		break;

	case 3:
		{
			index = ETHER_EFFECT_MAGIC_BULLET;
		}
		break;
	}

	return index;

}


BOOL Player::IsSummonIntensify()
{
    return FindSkillEffect( eCHAR_STATE_INTENSIFY_SUMMON ) != NULL;
}

void Player::ProcessSummonIntensifyEffect()
{
	// 소환수가 없으면 리턴
	if( GetSummonId() == NONE_ID || ! IsSummonIntensify() )
	{
		//if( m_hSummonIntensifyEffect != INVALID_HANDLE_VALUE)
		//{
		//	g_pEffectManager->RemoveEffectPtr(m_hSummonIntensifyEffect);
		//	m_hSummonIntensifyEffect = INVALID_HANDLE_VALUE;
		//}
		Monster *pMon=(Monster *)g_ObjectManager.GetObject(GetSummonId());
		if( pMon )
		{
			CWzUnitDraw * pUnitDraw = pMon->GetBasePart();
			if( pUnitDraw && pUnitDraw->GetSubType() != 0)
			{
				pUnitDraw->SetSubType(0);
			}

            pMon->DestorySummonIntensifyEffect();
		}

		return;
	}

	Monster *pMon=(Monster *)g_ObjectManager.GetObject(GetSummonId());
	if( !pMon)
		return;

	CWzUnitDraw * pUnitDraw = pMon->GetBasePart();
	if( pUnitDraw )
	{
		if( pUnitDraw->GetSubTypeNum() > 1 && pUnitDraw->GetSubType() != 1)
			pUnitDraw->SetSubType(1); // 서브 타입 바꿔준다.
	}

	BASE_NPCINFO *pNpcInfo=pMon->GetMonsterInfo();
	
	if( !pNpcInfo )
		return;


	WzVector wvPos, wvScale,wvRot;
	wvScale.x = 1.0f;
	wvScale.y = 1.0f;
	wvScale.z = 1.0f;

	wvRot.x = 0.0f;
	wvRot.y = 0.0f;
	wvRot.z = 0.0f;
	switch(pNpcInfo->m_wClass)
	{
	case 50:
		{
			wvPos = pMon->GetBonePos(3);
			wvScale.x = 0.3f;
			wvScale.y = 0.3f;
			wvScale.z = 0.3f;
		}
		break;
	case 51:
		{
			wvPos = pMon->GetBonePos(4);
		}
		break;
	case 52:
		{
			wvPos = pMon->GetBonePos(5);
		}
		break;
	default:
		{
			wvPos = pMon->GetBonePos(0);
		}
		break;
	}

	//if( m_hSummonIntensifyEffect == INVALID_HANDLE_VALUE )
	//{		
	//	m_hSummonIntensifyEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("K735"), pMon->GetBonePos(0));
	//}
	//CEffectBody * pBody = g_pEffectManager->GetEffectPtrHandle(m_hSummonIntensifyEffect);

    HANDLE effect_handle = pMon->CreateSummonIntensifyEffect(StrToWzID("K735"));
    CEffectBody * pBody = g_pEffectManager->GetEffectPtrHandle(effect_handle);
	if( pBody )
	{
		pBody->SetResultMatrix(wvPos,wvRot,wvScale);
	}
}


bool Player::IsShowByGameOption()
{
	if ( GameFramework::GetCurScene() != SCENE_TYPE_VILLAGE || g_pHero->GetObjectKey() == GetObjectKey() )
	{
		return true;
	}

	if (PLAYER_BEHAVE_VENDOR_ESTABLISHER_STATE == m_BehaveState)
	{
		if ( FALSE == GAMEOPTION->GetShowVendors() ) 
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else if ( FALSE == GAMEOPTION->GetShowPlayers() )
	{
		return false;
	}
	return true;
}

// 점령상태 정보 세팅 및 길드마스터의 경우 이펙트 생성
void Player::SetDominationStateInfo(const DOMINATION_STATE_TOTAL_INFO* domination_state_info)
{
    if (domination_state_info)
    {
        domination_state_info_.clear();
        domination_state_info_.reserve(domination_state_info->m_Count);
        for (int index = 0; index < domination_state_info->m_Count; ++index)
        {
            domination_state_info_.push_back(domination_state_info->m_Slot[index].m_wDominationCode);
        }

        // 길마의 경우 이펙트 생성
        // 타락한 사원은 이펙트 생성을 하지 않는다
        size_t size_of_info = domination_state_info_.size();
        BOOL is_ssq_scene = GlobalFunc::IsSSQScene();
        if ((size_of_info > 0) &&
            (GlobalFunc::IsSSQScene() == FALSE)
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
            && (GlobalFunc::IsBattleGroundScene() == FALSE)
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
            && (GlobalFunc::IsFreePVPScene() == FALSE)
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            && (GlobalFunc::IsGoldRushScene() == FALSE)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
            && (GlobalFunc::IsMondschienMap() == FALSE)
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
#ifdef _NA_008069_20150210_ADD_SPA_SYSTEM
            && (GlobalFunc::IsSpaScene() == FALSE)
#endif //_NA_008069_20150210_ADD_SPA_SYSTEM
            )
        {
            CreateDominationGuildRewardEffect(m_GuildRenderInfo.m_GuildDuty);
        }
    }
}

void Player::ClearDominationStateInfo()
{
    domination_state_info_.clear();
    // 이펙트가 생성되어 있다면 해제
    RemoveDominationStateEffect();
    
    // 자신의 인장UI(zonemark) 업데이트
    if (g_pHero && (g_pHero->GetObjectKey() == GetObjectKey()))
    {                        
        uiDominationTowerMan::RefreshZoneMarkInfo();
    }
}

void Player::CreateDominationGuildRewardEffect(DWORD guild_duty)
{
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
    RemoveDominationStateEffect();
#endif //_NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

    size_t size_of_info = domination_state_info_.size();
    size_t size_of_effect = domination_effect_list_.size();
    HANDLE hEffect = INVALID_HANDLE_VALUE;
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    bool area_guild = false; // 대륙 길드 일 경우

    STLX_VECTOR<MAPCODE>::iterator iter = domination_state_info_.begin();
    for ( ; iter != domination_state_info_.end(); ++iter)
    {
        const sDOMINATION_INFO* domination_info_ptr = 
            DominationInfoParser::Instance()->GetDominationInfo((*iter));
        if (domination_info_ptr && domination_info_ptr->m_DominationType == DOMINATION_TYPE_MONDSCHIEN)
        {
            area_guild = true;
            break;
        }
    }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
    
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
#else
    if (size_of_effect != 0 &&
        size_of_info > (size_of_effect - 1))
    {
        RemoveDominationStateEffect();
        size_of_effect = 0;
    }
#endif //_NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

    if (size_of_effect == 0)
    {
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        if (area_guild == true)
        {
            if (guild_duty == eGUILD_DUTY_MASTER)
            {
                hEffect = g_pEffectManager->CreateEffect(StrToWzID("I885"), GetVisiblePos());
            }
            else
            {
                hEffect = g_pEffectManager->CreateEffect(StrToWzID("I886"), GetVisiblePos());
            }
        }
        else 
        {
            if (guild_duty == eGUILD_DUTY_MASTER)
            {
                hEffect = g_pEffectManager->CreateEffect(StrToWzID("I705"), GetVisiblePos());
            }
            else
            {
                hEffect = g_pEffectManager->CreateEffect(StrToWzID("I708"), GetVisiblePos());
            }
        }
#else
        if (guild_duty == eGUILD_DUTY_MASTER)
        {
            hEffect = g_pEffectManager->CreateEffect(StrToWzID("I705"), GetVisiblePos());
        }
        else
        {
            hEffect = g_pEffectManager->CreateEffect(StrToWzID("I708"), GetVisiblePos());
        }
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

        // 길마 바닥이펙트 생성 핸들 저장
        if (hEffect != INVALID_HANDLE_VALUE)
        {
            domination_effect_list_.push_back(hEffect);
        }
        size_of_effect++;
    }
    
    if (guild_duty == eGUILD_DUTY_MASTER)
    {
        for (size_t count = (size_of_effect - 1) ; count < size_of_info; ++count)
        {        
            hEffect = g_pEffectManager->CreateEffect(StrToWzID("I710"), GetVisiblePos());
            if (hEffect != INVALID_HANDLE_VALUE)
            {
                domination_effect_list_.push_back(hEffect);
            }
            size_of_effect++;
        }

        // 돌아가는 이펙트 줄임
        for (size_t count = (size_of_info + 1); count < size_of_effect; ++count)
        {
            hEffect = domination_effect_list_.back();
            if (hEffect != INVALID_HANDLE_VALUE)
            {
                g_pEffectManager->RemoveEffectPtr(hEffect);
            }
            domination_effect_list_.pop_back();
        }
    }
}

void Player::RemoveDominationStateEffect()
{
    if (false == domination_effect_list_.empty())
    {
        std::list<HANDLE>::iterator itr = domination_effect_list_.begin();
        std::list<HANDLE>::iterator itr_end = domination_effect_list_.end();
        for ( ; itr != itr_end; ++itr)
        {
            if (INVALID_HANDLE_VALUE != (*itr))
            {
                g_pEffectManager->RemoveEffectPtr((*itr));
            }
        }
        domination_effect_list_.clear();
    }
}

void Player::SetGuildRenderInfo( const GUILD_RENDER_INFO* pGuildInfo )
{
    if (pGuildInfo)
    {
        m_bExistGuild = pGuildInfo->IsExist();
        if (m_bExistGuild)
        {
            BASE_PLAYERINFO* pInfo = GetCharInfo();

            if (pInfo)
            {
                // GUILD_RENDER_INFO 정보를 클라이언트에서 카피하여 관리한다.
                // PLAYER_GUILD_RENDER_INFO 사이즈 만큼만 카피
                // DOMINATION_STATE_TOTAL_INFO 정보는 SetDominationStateInfo() 함수로 따로 세팅
                memcpy(&m_GuildRenderInfo, pGuildInfo, sizeof(m_GuildRenderInfo));

                SetGuildName(m_GuildRenderInfo.m_tszGuildName, MAX_GUILDNAME_LENGTH);
                pInfo->m_GuildGuid = m_GuildRenderInfo.m_GuildGuid;

                CheckDominationRelation(); // 점령전 안에서의 동맹관게 설정
            }
        }
        else
        {
            ClearGuildRenderInfo();
        }
    }
}

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
void Player::SetClientPlayerRankingGradeInfo( const CLIENT_PLAYER_RANKING_GRADE_INFO* client_player_ranking_grade_info )
{
	if ( client_player_ranking_grade_info == NULL )
	{
		return;
	}
	is_client_player_ranking_grade_ = client_player_ranking_grade_info->IsExist();
	if ( is_client_player_ranking_grade_ )
	{
		CopyMemory(&client_player_ranking_grade_info_, client_player_ranking_grade_info, sizeof(*client_player_ranking_grade_info));
		CreatePlayerRankingEffect();
	}
	else
	{
		ClearClientPlayerRankingGradeInfo();
		RemovePlayerRankingEffect();
	}

}

void Player::RemovePlayerRankingEffect()
{
	if (false == player_ranking_grade_effect_list_.empty())
	{
		std::list<HANDLE>::iterator itr = player_ranking_grade_effect_list_.begin();
		std::list<HANDLE>::iterator itr_end = player_ranking_grade_effect_list_.end();
		for ( ; itr != itr_end; ++itr)
		{
			if (INVALID_HANDLE_VALUE != (*itr))
			{
				g_pEffectManager->RemoveEffectPtr((*itr));
			}
		}
		player_ranking_grade_effect_list_.clear();
	}
}

void Player::CreatePlayerRankingEffect()
{
	RemovePlayerRankingEffect();

	if ( client_player_ranking_grade_info_.ranking_type != eSUN_RANKING_TYPE_BEST_OF_BEST || 
		 client_player_ranking_grade_info_.grade < eSUN_RANKING_GRADE_0 || 
		 client_player_ranking_grade_info_.grade > eSUN_RANKING_GRADE_2 )
	{
		return;
	}

	size_t size_of_effect = player_ranking_grade_effect_list_.size();
	HANDLE hEffect = INVALID_HANDLE_VALUE;

	if (size_of_effect == 0)
	{
		switch ( client_player_ranking_grade_info_.grade )
		{
		case eSUN_RANKING_GRADE_0:
			{
				hEffect = g_pEffectManager->CreateEffect(StrToWzID("AB30"), GetVisiblePos());
			}
			break;
		case eSUN_RANKING_GRADE_1:
			{
				hEffect = g_pEffectManager->CreateEffect(StrToWzID("AB40"), GetVisiblePos());
			}
			break;
		case eSUN_RANKING_GRADE_2:
			{
				hEffect = g_pEffectManager->CreateEffect(StrToWzID("AB35"), GetVisiblePos());
			}
			break;
		}
		
		if (hEffect != INVALID_HANDLE_VALUE)
		{
			player_ranking_grade_effect_list_.push_back(hEffect);
		}
		size_of_effect++;
	}
}

void Player::UpdatePlayerRankingEffect()
{
	// 이펙트 높이 설정 - 이동 및 점프시
	if (false == player_ranking_grade_effect_list_.empty())
	{
		// 높이값 계산
		WzVector world_pos;
		WzMatrix position_matrix, rotation_matrix, temp_matrix;
		WzVector result_position;
		float rotation_angle = 0.0f;
		result_position = GetVisiblePos();
		MatrixIdentity(&temp_matrix);														
		VectorTransformCoord(&world_pos, &result_position, &temp_matrix);		
		MatrixTranslation(&position_matrix, world_pos.x, world_pos.y, world_pos.z);

		BYTE hide_condition = GetHideSkill();
		bool is_show_effect = (GetShow() &&
			IsShowByGameOption() &&
			hide_condition != HIDE_SKILL_INFO::ALL_HIDE &&
			BattleScene::GetScreenShotState() != eSCREEN_SHOT_NOT_UI_AND_HERO);
		std::list<HANDLE>::iterator itr = player_ranking_grade_effect_list_.begin();
		std::list<HANDLE>::iterator itr_end = player_ranking_grade_effect_list_.end();
		CEffectBody* effectbody = NULL;
		for ( ; itr != itr_end; ++itr)
		{
			if (INVALID_HANDLE_VALUE != (*itr))
			{
				effectbody = g_pEffectManager->GetEffectPtrHandle((*itr));
				if (INVALID_HANDLE_VALUE != effectbody && NULL != effectbody)
				{
					MatrixIdentity(&rotation_matrix);
					effectbody->SetShow(is_show_effect);
					effectbody->SetResultMatrix(&position_matrix, &rotation_matrix, NULL);
				}
			}
		}
	}
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

// 간략화한 랜더를 사용할 것인가. (로딩이 덜 된 플레이어)
BOOL Player::IsSimpleRenderMode()
{
	// 플레이어인 경우나머지는 true, 로딩이 완료되어야만 true 그렇지 않으면 false
	// 나머지는 true

	if( IsKindOfObject(PLAYER_OBJECT) == TRUE )
	{
		if ( IsUseDelayLoading() == true && IsCompletelyLoaded() == false )
		{
			return true;
		}
	}

    return false;
}
//------------------------------------------------------------------------------ 
float Player::GetJumpHeight()
{
    float height = 3.0f;

    if (IsRiding())
    {
        const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(GetRiderIdx());

        if (rider_info)
        {
            height = rider_info->set_jump_ * 0.01f;
        }
    }
    if (is_spread_wings())
    {
        height = 1.0f;;
    }

    return height;
}
//------------------------------------------------------------------------------ 
float Player::GetJumpDistance()
{
    float ratio = 1.0f;

    if (IsRiding())
    {
        const sRiderInfo* rider_info = RidingInfoParser::Instance()->GetInfo(GetRiderIdx());

        if (rider_info)
        {
            ratio = rider_info->set_hangtime_ * 0.01f;
        }
    }


#ifdef _NA_000000_20120410_SPEEDHACK
    float jump_dist = 0.0f;
    if (this == g_pHero)
    {
#ifdef _NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        float move_speed_modifier = GetMoveSpeedModifier();
#else //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        float move_speed_modifier = 0.0f;
        if (g_pHero->IsRiding() || g_pHero->m_bCustomMoveSpeed)
        {
            move_speed_modifier =  GetMoveSpeedModifier();
        }
        else
        {
            HeroAttributes* const hero_attr = (HeroAttributes*)(g_pHero->GetPlayerAttribute());
            move_speed_modifier = hero_attr->move_speed_modifier_;
        }
#endif //_NA_000000_20131022_RECIEVE_SPEED_FROM_SERVER
        jump_dist = move_speed_modifier * 4.5f * (g_pHero->move_speed_ / GetBaseMoveSpeedAsState(CMS_RUN)) * ratio;
    }
    else
    {
        jump_dist = GetMoveSpeedModifier() * 4.5f * (GetMoveSpeed() / GetBaseMoveSpeedAsState(CMS_RUN)) * ratio;
    }
#else
    float jump_dist = GetMoveSpeedModifier() * 4.5f * (GetMoveSpeed() / GetBaseMoveSpeedAsState(CMS_RUN)) * ratio;
#endif //_NA_000000_20120410_SPEEDHACK



    jump_dist = min(jump_dist, 1.5f);
    jump_dist = max(jump_dist, 9.0f);


    return jump_dist;
}
#ifdef _NA_20120220_JUMP_RENEWAL
#else
//------------------------------------------------------------------------------ 
bool Player::CalcJumpTime(float move_length)
{
    const DWORD kMaxJumpTime = 1000;

    float ratio = move_length / GetJumpDistance();
    m_JumpData.m_dwHorizonMoveTime = DWORD(kMaxJumpTime * ratio);
    m_JumpData.m_dwHorizonMoveTime = min(m_JumpData.m_dwHorizonMoveTime, kMaxJumpTime);

    m_JumpData.m_dwVerticalMoveTime = kMaxJumpTime;

    if ((is_spread_wings() == true) ||
        ((rider() != NULL) && (rider()->is_spread() == true)))
    {
        //ratio = min(ratio, 1.0f);
        //dwHorizonMoveTime = (DWORD)(dwVerticalMoveTime * 0.8f);;
        m_JumpData.m_dwVerticalMoveTime = 1;//(DWORD)(dwVerticalMoveTime * 1.0f * ratio);
    }

    return true;
}
#endif
//------------------------------------------------------------------------------ 
void Player::set_spread_wings(bool val)
{
    //------------------------------------------------------------------------------
    //! 슬롯에 있는 내용도 토글해준다. 날개 아이템은 하나만 장착 가능
    POSTYPE at_pos = 0;
    if (is_wing_equip(&at_pos) == true)
    {
        SCSlot& slot = GetEquipmentContainer()->GetSlot(at_pos);
        if (slot.GetSlotType() == ST_ITEM)
        {
            SCItemSlot& item_slot = static_cast<SCItemSlot&>(slot);
            if (item_slot.GetItemInfo()->m_wType == eITEMTYPE_WINGS)
            {
                item_slot.SetUserBlocked(val);
            }
        }
    }

    //
    bool backup_value = is_spread_wings_;
    is_spread_wings_ = val;

    if (is_spread_wings_ != backup_value)
    {
        RefreshAppearRender();
    }
}

//------------------------------------------------------------------------------ 
float Player::GetBodyRadius()
{
	return 0.0f;
}

//------------------------------------------------------------------------------ 
bool Player::is_wing_equip(OUT POSTYPE* at_pos)
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return false;
    }

    for (POSTYPE i = EquipmentContainer::EQUIP_SACCESSORY1; i <= EquipmentContainer::EQUIP_SACCESSORY3; ++i)
    {
        SCItemSlot& item_slot = static_cast<SCItemSlot&>(equipment_container->GetSlot(i));

        if (item_slot.GetItemInfo()->m_wType == eITEMTYPE_WINGS)
        {
            if (at_pos != NULL)
            {
                *at_pos = i;
            }
            return true;
        } 
    } 

    return false;
}
//------------------------------------------------------------------------------ 
bool Player::is_attached_wing_accessory(OUT int* attach_index)
{
    for (int i = AppearanceManager::kAppearanceAttachIndex_Accessory1; i <= AppearanceManager::kAppearanceAttachIndex_Accessory3; ++i)
    {
        if (i >= (int)GetAttachmentCount())
        {
            return false;
        }
        const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(GetAttachmentInfo(i).GetItemCode());
        if (item_info != NULL)
        {
            if (item_info->m_wType == eITEMTYPE_WINGS)
            {
                if (attach_index != NULL)
                {
                    *attach_index = i;
                }
                return true;
            }
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
WzID Player::GetCurrentRiderSpreadAnimation()
{
    if (IsRiding() == TRUE)
    {
        return GlobalFunc::GetPlayerAnimationByRiding(eANI_RIDER_SPREAD, GetRiderKindCode());
    }
    return 0;
}
//------------------------------------------------------------------------------ 
WzID Player::GetCurrentRiderFureAnimation()
{
    if (IsRiding() == TRUE)
    {
        return GlobalFunc::GetPlayerAnimationByRiding(eANI_RIDER_FURL, GetRiderKindCode());
    }
    return 0;
}
//------------------------------------------------------------------------------ 
WzID Player::GetCurrentSpreadOtherAnimation(WzID default_ani_id, INOUT ePLAYER_ANI& player_ani)
{
    if (this != g_pHero)
    {
        return default_ani_id;
    }

    WzID move_other_motion = 0;
    ePLAYER_ANI spread_player_ani = eANI_STAND;
    if (player_ani == eANI_RUN)
    {
        spread_player_ani = eANI_SPREAD_OTHERMOTION_MOVE;
    }
    else
    {
        spread_player_ani = eANI_SPREAD_OTHERMOTION_IDLE;
    }

    if ((IsRiding() == TRUE) && (rider() != NULL) && rider()->is_spread())
    {
        move_other_motion = GlobalFunc::GetPlayerAnimationByRiding(spread_player_ani, GetRiderKindCode());
    }
    else if (is_spread_wings())
    {
        move_other_motion = GlobalFunc::GetPlayerAnimationBySpreadWings(spread_player_ani);
    }

    if (move_other_motion > 0)
    {
        if (GetCurrentAnimation() == move_other_motion)
        {
            if (current_ani_progress_rate() < 100.0f)
            {
                //! space key 눌렀을때의 모션으로 유지
                player_ani = spread_player_ani;
                return move_other_motion;
            }
        }
        else
        {
            //날고 있는 상태에서 space key를 눌렀으면. 다른 애니메이션
            if (g_HeroInput.is_jump_key_pressed())
            {
                player_ani = spread_player_ani;
                return move_other_motion;
            }
        }
    }

    return default_ani_id;
}


//------------------------------------------------------------------------------ 
void Player::CreateDominationStampEffect()
{
    if (m_hDominationStampEffect == INVALID_HANDLE_VALUE)
    {
        const float kEffectPositionOffset = 1.3f;
        WzVector effect_position = GetHeadBonePos();
        effect_position.z += kEffectPositionOffset;
        // 생성
        m_hDominationStampEffect = GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A771"), effect_position);
    }
}

//------------------------------------------------------------------------------ 
void Player::DestroyDominationStampEffect()
{
    if (m_hDominationStampEffect != INVALID_HANDLE_VALUE)
    {
        g_pEffectManager->RemoveEffectPtr(m_hDominationStampEffect);
        m_hDominationStampEffect = INVALID_HANDLE_VALUE;
    }
}

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
void Player::RenderDiceOnHead()
{
    if (GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ)
    {
        return;
    }

    if (SCENE_TYPE_CHARSELECT == GameFramework::GetCurScene())
    {
        return;
    }

    if (SCENE_TYPE_NONE == GameFramework::GetCurScene())
    {
        return;
    }

    // 대화를 표시하고 있는경우 패스
    if ( GetChatDisplayTime() )	
    {
        return;
    }

    if (m_bObserverMode)
    {
        return;
    }

    TCHAR szGuildString[INTERFACE_STRING_LENGTH] = { 0,};
    Snprintf (szGuildString,INTERFACE_STRING_LENGTH-1, _T("< %s >"), GetGuildName());

    if (g_InterfaceManager.GetShowFlag()) 
    {

        WzVector wzVisualPos = GetVisiblePos();
        WzColor oldcolor = g_pSunRenderer->GetDiffuseColor();
        g_pSunRenderer->Set2DMode( TRUE );
        g_pSunRenderer->x_pManagerTextOut->StartToUseFont( StrToWzID("m217") );

        WzVector wzPos = GetPosition();
        wzVisualPos.z += GetMeshHeight();
        wzVisualPos.z += GetHeight();
        wzVisualPos.z += HEIGHT_GAP;

        //if (is_spread_wings())
        //{
        //    //! 날개 상태이면 위로 좀더 올려 준다.
        //    wzVisualPos.z += 1.4f;
        //}

        POINT pt2;
        float fDepth;
        g_pSunRenderer->GetScreenPos( &pt2, &wzVisualPos , &fDepth );

        SIZE textsizeGuild, textSizeName;
        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint(szGuildString,lstrlen(szGuildString),&textsizeGuild);

#ifdef _NA_003027_20111013_HONOR_SYSTEM
        const TCHAR* player_name = GetNameWithTitle();
#else
        const TCHAR* player_name = GetName();
#endif //_NA_003027_20111013_HONOR_SYSTEM 

        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint((TCHAR*)player_name, lstrlen(player_name), &textSizeName);// 글로벌 엔진버전이 낮아서 error발생 (TCHAR*)필요 추후 제거 요망

        SIZE BigTextSize = textsizeGuild.cx > textSizeName.cx ? textsizeGuild : textSizeName;

        g_pSunRenderer->x_pManagerTextOut->EndUseFont();

        //------------------------------------------------------------------------------ 
        //! 주사위 출력
        {
            if (g_InterfaceManager.GetDiceCastManager()->IsDiceDraw(GetObjectKey()) == true)
            {
                uiPartyMan* ui_party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
                if (ui_party_manager != NULL && ui_party_manager->IsParty() == true)
                {
                    int party_index = ui_party_manager->GetPartyIndexByObjectKey(GetObjectKey());
                    if (party_index >= 0)
                    {
                        HANDLE tex_handle = g_InterfaceManager.GetPartyDiceTexture(party_index);
                        St_TextureContainer* tex_info = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(tex_handle);
                        if (tex_info != NULL)
                        {
                            RECT dice_rect;
                            dice_rect.left = pt2.x + (BigTextSize.cx / 2) + 5;
                            dice_rect.right = pt2.x + (BigTextSize.cx / 2)+ tex_info->m_nWidth + 5;
                            dice_rect.top = pt2.y - (tex_info->m_nHeight / 2) - 2;
                            dice_rect.bottom = pt2.y + (tex_info->m_nHeight / 2) - 2;

                            g_InterfaceManager.RenderRectWithTextureAndZTest(
                                dice_rect,
                                fDepth,
                                g_InterfaceManager.GetPartyDiceTexture(party_index));
                        }
                    }
                }
            }
        }
        //------------------------------------------------------------------------------ 

        g_pSunRenderer->Set2DMode(FALSE);
        g_pSunRenderer->SetColor(oldcolor);
    }

}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

void Player::CheckDominationRelation()
{
    if(SCENE_TYPE_DOMINATION == GameFramework::GetCurScene()
#ifdef _NA_008334_20150608_SONNENSCHEIN
        || SCENE_TYPE_SONNENSCHEIN == GameFramework::GetCurScene()
#endif //_NA_008334_20150608_SONNENSCHEIN
        )
    {
#if defined(_NA_006826_20130722_DOMINATION_RENEWAL) && defined(_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER)
        if (IsExistGuild() && 
            g_pHero->GetGuildRenderInfo().m_GuildGuid == m_GuildRenderInfo.m_GuildGuid)
        {
            m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_ALLIENCE;
            return;
        }

        // 공격측이면 TRUE, 수성측이면 FALSE
        enum {DEFENDER = 0, ATTACKER = 1};
        BOOL hero_section = ATTACKER;  // 히어로 편
        BOOL player_section = ATTACKER; // 플레이어 편

        // 공.수성 진영 판단
        {
            uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
            if (ui_domination_manager)
            {
                // 현재 지역의 수성길드인지 확인
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                // 현재까지는 몬트샤인의 필드가 존재하지 않아 몬트샤인 맵일 경우는 link_map_code를 찾아서 받지 않고 원래의 code로 적용한다.
                MAPCODE link_map_code = 0;
                if (GlobalFunc::IsMondschienMap() == TRUE)
                {
                    const sDOMINATION_INFO* domination_info_ptr = 
                        DominationInfoParser::Instance()->GetDominationInfo(g_pMap->GetCurrentFieldID());
                    if (domination_info_ptr)
                    {
                        link_map_code = domination_info_ptr->m_DominationMapCode;
                    }
                }
                else
                {
                    link_map_code = DominationInfoParser::Instance()->GetDominationLinkMapCode(g_pMap->GetCurrentFieldID());
                }
#else
                MAPCODE link_map_code = DominationInfoParser::Instance()->GetDominationLinkMapCode(g_pMap->GetCurrentFieldID());
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
                DOMINATIONGUILDMAP::iterator found_itr = ui_domination_manager->GetDominationGuildMap().find(link_map_code);
                if (found_itr != ui_domination_manager->GetDominationGuildMap().end())
                {
                    DOMINATION_GUILD_INFO* guild_info = &found_itr->second;
                    if (guild_info)
                    {
                        if (guild_info->m_GuildGuid == g_pHero->GetGuildRenderInfo().m_GuildGuid)
                        {
                            hero_section = DEFENDER;
                        }
                        if (guild_info->m_GuildGuid == m_GuildRenderInfo.m_GuildGuid)
                        {
                            player_section = DEFENDER;
                        }
                    }
                }
                
                // 수성측 리스트에 있는 길드인지
                if ((hero_section != DEFENDER) &&
                    (ui_domination_manager->IsDefenceGuildAlliance(g_pHero->GetGuildRenderInfo().m_GuildGuid)))
                {
                    hero_section = DEFENDER;
                }
                if ((player_section != DEFENDER) &&
                    (ui_domination_manager->IsDefenceGuildAlliance(m_GuildRenderInfo.m_GuildGuid)))
                {
                    player_section = DEFENDER;
                }
            }
        }

        if (hero_section == DEFENDER) // 히어로가 수성측 일때
        {
            if (player_section == DEFENDER) // 플레이어가 수성측 이면 아군.
            {
                m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_ALLIENCE;
            }
            else
            {
                m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_BOTH_HOSTILITY;
            }
        }
        else // 히어로가 공성측 일때
        {
            GUILD_RELATION* guild_relation = GuildSystem::GetGuildSystemData().GetGuildRelation(m_GuildRenderInfo.m_GuildGuid);
            if (guild_relation != NULL)
            {
                if ((player_section == ATTACKER) &&
                    (guild_relation->m_eRelationType == eGUILD_RELATION_ALLIENCE)) // 동맹이면서, 공격측 이면 아군
                {
                    m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_ALLIENCE;
                }
                else
                {
                    m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_BOTH_HOSTILITY;
                }
            }
            else
            {
                m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_BOTH_HOSTILITY;
            }
        }
#else
        if(IsExistGuild() && 
            g_pHero->GetGuildRenderInfo().m_GuildGuid != m_GuildRenderInfo.m_GuildGuid)
        {
            m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_BOTH_HOSTILITY;
        }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
    }    
}

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
void Player::RenderGoldMarkOnHead( RECT rc, float fDepth, eGUILD_MARK_RELATION eRelation)
{
    g_InterfaceManager.RenderGuildMarkBackGround(rc, fDepth, eRelation);

    // 뒤에 바탕    
    g_InterfaceManager.RenderRectWithTextureAndZTest(rc, fDepth, 0, 11);
    int gold_mark_index = -1;
    gold_mark_index = ChaosSystem::GetGoldRushData().get_gold_grade(this->GetName());    
    gold_mark_index = g_InterfaceManager.get_gold_rush_mark_index(gold_mark_index);
    if (gold_mark_index != -1)
    {
        BASE_GuildMarkInfo* guild_mark_info_ptr = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(gold_mark_index);
        if (guild_mark_info_ptr)
        {
            g_InterfaceManager.RenderRectWithTextureAndZTest(rc, fDepth, guild_mark_info_ptr->iRow, guild_mark_info_ptr->iColumn);
        }
    }
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE
void Player::RenderRankingGradeOnHead( RECT rc, float fDepth, eGUILD_MARK_RELATION relation )
{
	g_InterfaceManager.RenderGuildMarkBackGround(rc, fDepth, relation);

	int mark_index = g_InterfaceManager.get_ranking_mark_index(GetClientPlayerRankingGradeInfo().ranking_type, GetClientPlayerRankingGradeInfo().grade);
	if ( mark_index != 0 )
	{
		BASE_GuildMarkInfo* guild_mark_info_ptr = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(mark_index);
		if (guild_mark_info_ptr)
		{
			g_InterfaceManager.RenderRectWithTextureAndZTest(rc, fDepth, guild_mark_info_ptr->iRow, guild_mark_info_ptr->iColumn);
		}
	}
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_GRADE

#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
void Player::RenderDominationMarkOnHead( RECT rect, float depth, eGUILD_MARK_RELATION eRelation )
{
    DOMINATION_GUILD_INFO* domination_info_ptr = NULL;
    uiDominationTowerMan* domination_manager_ptr = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (NULL == domination_manager_ptr)
    {
        return;
    }

    g_InterfaceManager.RenderGuildMarkBackGround(rect, depth, eRelation);
    
    // 여기서 공수성을 판단해서 뿌려주자.
    // 수성 측이면 방패    
    domination_info_ptr = domination_manager_ptr->GetDominationGuildInfo();
    if (domination_info_ptr && (domination_info_ptr->m_GuildGuid == this->GetPlayerGuildID())
        || (domination_manager_ptr->IsDefenceGuildAlliance(this->GetGuildRenderInfo().m_GuildGuid)))
    {
        g_InterfaceManager.RenderRectWithTextureAndZTest(rect, depth, 1, 12);
    }
    else // 공성측이면 칼을 띄워주자.
    {
        g_InterfaceManager.RenderRectWithTextureAndZTest(rect, depth, 0, 12);
    }
}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
