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

	m_byGMGrade = 0;			//	GM ���
	m_bInvisible = false;		//	����
	//m_bHero=FALSE;
	m_iValkyrieShotAnimVariation = 0;

	m_bHideHelmet = FALSE;
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    m_bHideWing = FALSE;
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	m_bInvincible = FALSE; // ����
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

        ////	���� �ʱ�ȭ( IDLE ���� )
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
    appearInfo.Job = dwID;		// �ϴ� ID�� ĳ���� ������ ����. 

    CreateCharacterVariation(&appearInfo, TRUE);
    InitContainer();

	//	���� �ʱ�ȭ( IDLE ���� )
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

	//	GM ����
	m_byGMGrade				= 0;			// GM ���
	m_bInvisible			= FALSE;		// ����

	// PVP ����
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

	//�������׸�ź ����Ʈ ��������
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
/** ��Ȳ�� ���� �ɼ°��� �����ϱ�������..�����Ѵ�
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
        //! ĳ���� ��ü ����
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
/**  ���̵�����üũ:�� Flag������ Ż���� Ÿ���Ÿ�� �����Ѵ�.
*/
BOOL Player::IsRiding()
{
    //���̵���������
    if(m_bRiding && m_pRider) 
    {
        //�κ�ų� �����ε��϶��� ���̵����¾ƴ��Ƿΰ���
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
/**  ���̴� refresh
*/
void Player::RefreshRider()
{
    if( IsRiding() == TRUE && IsUseDelayLoading() == false )
    {
        m_pRiderContainer->RefreshApperance();
        m_pRider->EnableFunction(TRUE);

#ifdef _NA_007732_20141024_RIDER_MOVESPEED_SYNC
        m_pRiderContainer->RefreshRiderSpeed(); // Ż�� �̼� ����
#endif //_NA_007732_20141024_RIDER_MOVESPEED_SYNC
    }
}
//------------------------------------------------------------------------------
/**  ���̴� ��þƮȿ��
*/
char Player::GetRiderKindCode()
{
	return m_pRider->GetAniMapping();
}
//------------------------------------------------------------------------------
/**  ���̴� ��þƮȿ��
*/
void Player::RiderSubtype(int nPartIdx,int nSubType)
{
	if(IsRiding())
	{
		m_pRider->SetSubtype(nPartIdx,nSubType);
	}
}
//------------------------------------------------------------------------------
/**  ���̴� �������̱�
*/
void Player::AttachRiderPart(int nIdx, DWORD dwItemCode)
{
	if(IsRiding())
	{
		m_pRider->AttachRiderPart(nIdx,dwItemCode);
	}
}
//------------------------------------------------------------------------------
/**  ���̴� ��������
*/
void Player::DetachRiderPart(int nIdx)
{
    if(IsRiding())
    {
        m_pRider->DetachRiderPart(nIdx);
    }
}
//------------------------------------------------------------------------------
/**  ���̴� ����
*/
void Player::CreateRider(DWORD dwID,BOOL bEffect)
{
    m_nRiderIdx = 0;

	//���̵����¶�� ����Ż���� �����Ѵ�
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

		//�������ж��
		if(m_bRiding == FALSE)
		{
			SAFE_DELETE( m_pRider );
		}
        else
        {
            m_nRiderIdx= dwID;  
            m_dwStandAnimDelay = 0;
            //�븻���͸�z���� 1.0f�� �����ϴ� ������ Rider���� �䰪�� �����Ƿ� SetRotation�ϴµ�
            //�̰��� ��������̾ƴҰ�� ���� ���Լ� ȣ��������� ���� ����� �ȵǾ��ִ� 0���ͷ��ʱ�ȭ�Ǿ�����
            SetVector(&m_vNormal,0.0f,0.0f,1.0f);
        }
	}
}
//------------------------------------------------------------------------------
/** ���̴� ����
*/
void Player::DestroyRider()
{
	if(m_pRider)
	{
        //�����̳ʵ� �����Ѵ�.
        if(m_pRiderContainer)
        {
            m_pRiderContainer->Clear();
        }
		
        GetBasePart()->SetRotationByMatrix(NULL);

        GetBasePart()->UpdateTransform();

        m_pRider->Destroy();
		SAFE_DELETE( m_pRider );
		m_bRiding = FALSE;

        // ���� ���/���� ���϶� Ż���� ������� �ִϸ��̼� �ٷ� �ٲ� �ش�.
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
		//ĳ���� ������ġ�� ���̴����� �������ش�.
		WzVector vPos=GetBasePart()->GetPosition();
		m_pRider->SetPosition(&vPos);
	}
	return bRet;
}
//------------------------------------------------------------------------------
/** ���̴��� �ִϸ��̼��� �ٷ� ������� �÷��̾�� ��������ش�(��ġ.ȸ��)
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
   
    //�����̵��ÿ��� ����������Ѵ�.
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
    //���� �ִ� ���¿��� �̵��� ���� �ִϸ��̼��� Ư��ó��
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
/** ���� ����ִ� ���Ⱑ(ĵ������Ʈ)�������� üũ�Ѵ�
���� ��ũ��Ʈ�������� ���밡ġ����� �ϵ��ڵ��Ƿ� ����
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

            // �̽�ƽ ����
            if( item_slot_ref.GetImageCode() == 50495 )
                return TRUE;
            if( item_slot_ref.GetImageCode() == 50496 )
                return TRUE;

            // ����̵� ~ ���� �ű�Ŭ���� �뿪
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

	// �̰� �� �̷������� �ߴ��� ���� �ϴ� ��� ����ǵ��� ������ 
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

	//���� ��ȯ�Ȼ��¶��
	if (ClientPart.m_bSummoned)
	{
		SCItemSlot ItemSlot(ClientPart.m_PetSlotStream);
		
		if (ItemSlot.IsContainValidItem())
		{
			CreatePetSummon(ItemSlot.GetImageItemInfo()->m_wSkillCode);
		}
	}

	// ��Ţ�����ؼ� ����� �����ش�.
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
        //���� ��ȿ���� üũ
        SCItemSlot* item_slot = GetEquipmentContainer()->GetItemSlot(equip_pos);
        if (item_slot == NULL)
        {
            continue;
        }
 
        // �ش� ������ Ȱ��ȭ �Ǿ� �ִ��� üũ
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

    // ��Ţ������ ��� �޾Ҵ�.
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
        // ĳ���� �ִϸ��̼ǿ� ���缭 ���⸦ �ִϸ��̼� �����ش�
        // ������ ShowCrossbow ���� ���� �ִϸ��̼��� �����ϹǷ� ����

        if (GetWeaponKind() != eWEAPONTYPE_ONEHANDCROSSBOW )
        {
            // ����̵��� N203 (����IDLE) �� ����
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
�ش� ����� ��Ÿ�ϰ� Ÿ���� �´� ���� �Ƶڸ� ��´�.

*/
WzID Player::GetCurrentAttackAnimation()
{
	WzID animationID = StrToWzID("A001");
	DWORD dwStyle = GetCurrentAttackStyle();
	//Todo:��Ÿ���� ������� 2Ÿ�� �Ѿ���� �ȵȴ�.
	DWORD sequence=GetAttackSequence();
	DWORD weapon  =GetWeaponKind();

	BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo( (SLOTCODE)dwStyle );
    // �Ѽռ����� �ƴϸ�
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
���� �������� ����� nomal style ���¸� ������.
����� ���� ���� ���� ��Ÿ���� ����� üũ �Ҽ� ����...�ϵ� �ڵ� ���Ҽ� ����!

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

	// Ư�� �ڼ�
	if(IsRiding())
	{
		//���ν����ش�.
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



	// �⺻�ڼ�
	WzID animationID = 0;	

	char szAniCode[5];
	// ���⿡���� �⺻�ڼ��� �ٲ�ϴ�.

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = '0';
	szAniCode[2] = '0';
	szAniCode[3] = '1';
	szAniCode[1] = GetWeaponAnimationCode();


	// ���ý��ĵ�� �ڵ� ���� 3 �⺻�� 1�̴�.
	if (m_iAttackStandTime > 0 )			
	{
		// ���������� IDLE
		if (GetClass() == eCHAR_VALKYRIE) 
		{
			// ��Ű�� ���ݽô� Idle���°� �ƴϴ� ���ݸ���� �����ش�.
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

		// ������������ IDLE
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
	// ���⿡���� �⺻�ڼ��� �ٲ�ϴ�.

    if (GetHideSkill())
    {
        // ���̵� ������ �ִ��ڵ�
        if (GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            // ���Ⱑ ����϶��� �ִ��ڵ�
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
	// ���⿡���� �⺻�ڼ��� �ٲ�ϴ�.

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
�ɱ� ���ϸ� ��´�.
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

            // ���ð� �ο�
            SetSpaSittingAnimationDelay();
            return animation_id;
        }
    }
  #else
    if (IsInSpaTile())
    {
        // ���ð� �ο�
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
        //! �ִ� ����
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
�ɱ� ���� ���ϸ� ��´�.
*/
WzID Player::GetCurrentSittingAnimation()
{
	if(IsRiding())
	{
		return GlobalFunc::GetPlayerAnimationByRiding(eANI_SIT_IDLE,GetRiderKindCode());
	}
    if (is_spread_wings())
    {
        // �ִ� ����
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
�Ͼ�� ���ϸ� ��´�.
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
        // �ִ� ����
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
/**  �����:������ �÷��̾� ������ �����Ѵ�.
*/
BOOL Player::CreatePetSummon(WORD wPetIndex)
{	
	//safe_code:Ȥ server���� BRD�� �ι������������ ��� 
	if (this->IsPetReturn())
	{
		DeletePetSummon();
	}

    m_PetInfo.m_bPetSummoned = TRUE;

	sPet_Script_Info* pet_script_ptr =	PetInfoParser::Instance()->GetPetInfo(wPetIndex);

	if (!pet_script_ptr)
	{
		assert(!"�̷� ���ε����� �����ϴ�.");	 
		return FALSE;
	}
	
    if(GENERALPARAM->IsNet())
    {
	    m_PetInfo.m_dwPetObjKey = g_ObjectManager.GenerateKeyByPet(PET_OBJECT);
    }
    else
    {
        //! �̱۸��
        m_PetInfo.m_dwPetObjKey = g_ObjectManager.GenerateKeyAtSingleMode(PET_OBJECT);
    }

	m_PetInfo.m_wPetIndex = wPetIndex;
	
	Pet* pet_ptr = static_cast<Pet*>(g_ObjectManager.Add(m_PetInfo.m_dwPetObjKey, 
		                                                 PET_OBJECT,
		                                                 pet_script_ptr->m_MonsterCode,
		                                                 FALSE));

	if (!pet_ptr)
	{
		assert(!"�������Ʈ �����Ұ�");
		return FALSE;
	}

	pet_ptr->InitPetMonster(this->GetObjectKey(),wPetIndex);

    return TRUE;
}
//------------------------------------------------------------------------------
/** ������
*/
void Player::DeletePetSummon()
{
	Pet* pet_info = static_cast<Pet*>(g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey));
	
	//safecode:������ȿ����üũ
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
/**	 ����� ����(���¿����� ����Ʈ���)
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
/**	 �÷��̾� �������Ʈ
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
/**	 �÷��̾� �������Ʈ
*/
void Player::SetPetName(TCHAR* pszName)
{
	if (!pszName)
    {
        return;
    }

	StrnCopy(m_PetInfo.PLAYER_.m_sPetName ,pszName, MAX_PET_NAME_LENGTH);
	
	Pet* pPet = (Pet*)g_ObjectManager.GetObject(m_PetInfo.m_dwPetObjKey);

	//safecode:������ȿ����üũ
	if (pPet && pPet->IsKindOfObject(PET_OBJECT))
	{
		pPet->RefreshPetName();
	}
}





//------------------------------------------------------------------------------
/**  ���̵�����϶� ��Ƽ������ ���� ��������
*/
void Player::RefreshHideState(BOOL bClear)
{
	// �ڽ��� �н�
	if( IsHero() )
		return;

    //���� �����츸 üũ
    if( GetClass() != eCHAR_SHADOW )
    {
        return;
    }

    // �������ΰ�쿡�� 
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

		// ����ΰ� ������ ��ų�� ������ ������ ��� ����ü�� �������ϰ� ǥ���Ѵ�
        if( g_pHero->IsDetectingHide() == TRUE )
        {
            IsHalfHide = TRUE;
        }

		// �÷��̾ ��ũ�������Ʈ ��ų�� �ɷ��ִٸ�, �� ����ü�� �������ϰ� ǥ���Ѵ�
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
/**  ���̵� ����(��Ƽ or ���������� ���)
*/
BYTE Player::GetHideCondition()
{
    //0.�⺻���Ƿ� Hero��������
    BYTE byHideState  = IsHero() ? (m_sHideInfo.HALF_HIDE):(m_sHideInfo.ALL_HIDE);

    //1.��Ƽ�������üũ(��Ƽ����ǰ�� ������)
    uiPartyMan* partyMan = static_cast<uiPartyMan *>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));
    
    if (partyMan && partyMan->IsPartyMember(GetObjectKey()))
    {
        byHideState = m_sHideInfo.HALF_HIDE;
    }
    
    
    //2.������������ �����������(������)
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
        // ���� ���̸� ������ȭ
        if (IsHero() == false)
        {
            if (this->GetTeam() == g_pHero->GetTeam())
            {
                byHideState = m_sHideInfo.HALF_HIDE;
            }
        }
        
        return byHideState; // �Ʒ� ������ ���� ������ ��� �ұ�.. �ϴ� �׳� ����
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

    //3.������������ ��� ĳ��������������¸������Ѵ�.
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
/**  ĳ���͸� ���̵��Ų��
	 0:�����̵����   1:�������   2:���������(�ϴ� ���ΰ��϶�)
*/
void Player::SetHideSkill(BOOL bFlag)
{	
    if( (GetClass()!=eCHAR_SHADOW && GetClass() != eCHAR_MYSTIC) && bFlag )
    {
        //safecode: �����찡 �ƴ�ĳ������ ���̵尡 �Դ�
        assert(!"���������̵�����:SetHideSkill");
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
			//�׸��ھ���	
			SetShadowDraw(FALSE);
		}
		else
		{
			SetShow(FALSE);
			
            //���ΰ��� this Ÿ�����ϰ��ִٸ� Ǯ��
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
			//����
			if(!GetShow() || m_sHideInfo.byHideState==m_sHideInfo.HALF_HIDE)
			{
				SetFadeIn(1500);
			}

            SetShow(TRUE);
		}
	}
    
    UpdateItemEffectPos(this); // SetShow() �� ��ġ ����

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
/**  ���� ������ (����� ���̵�� ����)
*/
void Player::SetBattleGroundHideMode(bool is_hide)
{	
    if (is_hide)
    {
        BattleGroundHideMode hide_mode = CheckBattleGroundHideMode(); // ���������� ������������ üũ
        set_battle_ground_hide_mode(hide_mode);

        if (battle_ground_hide_mode() == BattleGroundHideMode_Half)
        {
            //�׸��ھ���	
            SetShadowDraw(FALSE);
        }
        else
        {
            SetShow(FALSE);

            //���ΰ��� this Ÿ�����ϰ��ִٸ� Ǯ��
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
            //����
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
            // �� ����ȭ�� ���̵带 �ᵵ ������ �� �ϴ�. (�÷��̾� ������¿� ���� ����ǹǷ�)
            BYTE hide_state = static_cast<BYTE>(battle_ground_hide_mode());
            pet_pointer->SetStatePetHide(hide_state);
        }
    }
}
//------------------------------------------------------------------------------
/**  ���� ������ üũ (���̵�� �ٸ����� ������ų�� ������ �ʴ´ٴ� ��)
*/
BattleGroundHideMode Player::CheckBattleGroundHideMode()
{
    //0.�⺻���Ƿ� Hero��������
    BattleGroundHideMode hide_mode = IsHero() ? BattleGroundHideMode_Half : BattleGroundHideMode_Full;
    
    //1.��Ƽ�������üũ(��Ƽ����ǰ�� ������)
    //uiPartyMan* party_manager = static_cast<uiPartyMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_PARTY));

    //if (party_manager && party_manager->IsPartyMember(GetObjectKey()))
    //{
    //    hide_mode = BattleGroundHideMode_Half;
    //}

    //2.��������� ���� ������(������)
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    // ��巯���� ����� ���� ���̵� ���� ���� ���
    if (GlobalFunc::IsBattleGroundScene() == TRUE || 
        GlobalFunc::IsGoldRushScene() == TRUE)    
#else
    if (GlobalFunc::IsBattleGroundScene() == TRUE)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    {
        // ���� ���̸� ������ȭ
        if (IsHero() == false)
        {
            //if (this->GetTeam() == g_pHero->GetTeam())
            // this->GetTeam()���� ���ϱ⿣ ���� �� Ű�� ������ �ȵ� ������
            //
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            // ������ �ƴ϶�� ��巯���̴�.
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

    ////3.������������ ��� ĳ��������������¸������Ѵ�.
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
/**  ���� ������ ����
*/
void Player::RefreshBattleGroundHideMode()
{
    // ����� ���� ����
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    // ��巯���� ���� ������ �߰�
    if (GlobalFunc::IsBattleGroundScene() == FALSE && 
        GlobalFunc::IsGoldRushScene() == FALSE)
#else
    if (GlobalFunc::IsBattleGroundScene() == FALSE)
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    {
        return;
    }

    // �ڽ��� �н�
    if (IsHero())
    {
        return;
    }

    // �������ΰ�쿡�� 
    if (battle_ground_hide_mode() != BattleGroundHideMode_None)
    {
        if (g_pHero && g_pHero->GetTeam() == this->GetTeam()) // ���� ���̸�
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
/**	���̵� ����Ʈ(start)
*/
void Player::StartExtraEffect()
{
    //��������¸�����
    if ((m_sHideInfo.byHideState == m_sHideInfo.HALF_HIDE) ||
        (battle_ground_hide_mode() == BattleGroundHideMode_Half))
    {

        //�ڽ��� ���̵�ÿ� ����ȿ��
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
/**	���̵� ����Ʈ(end)
*/
void Player::EndExtraEffect()
{
    //��������¸�����
    if ((m_sHideInfo.byHideState == m_sHideInfo.HALF_HIDE) ||
        (battle_ground_hide_mode() == BattleGroundHideMode_Half))
    {
        //�����·� �ٽ� �ǵ�����.
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
/**	Ŭ���� üũ(������ ���� �����츸)
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
            //�����ܻ���� ������.
            static DWORD dwCopytime = 0;

            //150������ ����
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
// 0905��Ŷ���� ��
//==================================================
//------------------------------------------------------------------------------
/**  �� ���� ����
*/
void Player::SetPlayerPetInfo( PLAYER_RENDER_INFO_EX *pPlayerInfo)
{
    if (!pPlayerInfo)
    {
        return;
    }

	//��ȯ���¶�� �����Ѵ�
	if (pPlayerInfo->is_summoned_pet)
	{
		CreatePetSummon(pPlayerInfo->pet_index);
		SetPetAttributes(0,TRUE,pPlayerInfo->pet_fullness);	//������
		SetPetAttributes(0,FALSE,pPlayerInfo->pet_intimacy);	//ģ�е�

		ZeroMemory(m_PetInfo.PLAYER_.m_sPetName, sizeof(m_PetInfo.PLAYER_.m_sPetName));

		//������� �����Ѵ�
		if (pPlayerInfo->pet_name[0] != '\0')
		{
			SetPetNameUse(1);				//���������
			SetPetName(pPlayerInfo->pet_name);
		}
		else
		{
			SetPetNameUse(0);				//����Ӿ���
		}
	}

    m_PetInfo.m_bPetSummoned = pPlayerInfo->is_summoned_pet;
}
//------------------------------------------------------------------------------
/** �÷��̾� ��������
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
        DWORD collect_progress_time = 0; // ä�� ����ð� (��Ŷ�� �����ð����� ��)
        
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
       //���̴� �ڵ带�����Ѵ�.
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
            //eCHAO_STATE_NORMAL:�Ϲ�, eCHAO_STATE_PREV_CHAO:���� ī��, eCHAO_STATE_CHAO:ī��
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


	//Todo:���⼭ üũ������ Player�� ����������
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
            //safecode: �����찡 �ƴ�ĳ������ ���̵尡 �Դ�
            if(this->GetClass()!=eCHAR_SHADOW)
            {
                assert(!"������ ���̵�����:SetPlayerInfo");
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
        //safecode: �����찡 �ƴ�ĳ������ ���̵尡 �Դ�
        if(this->GetClass()!=eCHAR_SHADOW)
        {
            assert(!"������ ���̵�����:SetPlayerInfo");
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

	// ���� ���ݿ� ���� ó���� Ȯ���� ���ش�.
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
		m_iValkyrieShotDelay = 500;	// �ѵ�� �ִ� �ð�.
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
	// ��� ���ɻ���!
	if( g_HeroInput.IsIdleState() || GetCurState() == STATE::KEYBOARDMOVE || GetCurState() == STATE::MOVE )
	{
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/** �� ����
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
/** ���̴� ����
*/
void Player::RefreshAppearRider()
{
    if (IsRiding() == FALSE)
    {
        return;
    }

    //���̴� ��ü�� �Ҵ�Ǹ� �������Ҵ��Ѵ�
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
			RenderTextOnHead(_T("����ī��"),m_RenderinfoColor,y,TRUE);

		}

		if(this->IsAttackBeginPlayer())
		{

			y+=1;
			RenderTextOnHead(_T("������"),m_RenderinfoColor,y,TRUE);
		}


		if(this->GetChaoState()==eCHAO_STATE_CHAO)
		{

			y+=1;
			RenderTextOnHead(_T("ī��"),m_RenderinfoColor,y,TRUE);

		}
	}

	if (GetClass() == eCHAR_VALKYRIE)
	{
		ProcessValkyrieShot(dwTick);
	}

	//�ð��� ������ �ڵ����� ���ش�.
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

	// �̵����϶��� 0.5�ʸ��� ���������´�.
	// Ű�����̵��ÿ��� ȸ���� ����
	// 0x00000033�� ȸ���� ������ ��� Ű���� �̵� flag��
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

    // ����Ʈ ���� ���� - �̵� �� ������
	if (false == domination_effect_list_.empty())
	{
        // ���̰� ���
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
                        // �� ����Ʈ�� 35�� �������� ����
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
			// �������� ����~
			else
			{
				// ���� ����� ó���ϰ�
				ProcessAttackResult(pAction->ATTACK.dwAttackSerial,TRUE);
				// ��ƿ�~
				SetCurrentAction(&idleaction);
				SetNextState(STATE::IDLE,g_CurTime);					
				return 0;
			}
		}
		break;

		// �̵�~
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
			SetAngle(pAction->SPECIALMOVE.fAngle,TRUE);// Ư�� �̵��� �ٷ� ���´�.
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
			// �������� ����~
			else
			{
				// ���� ����� ó���ϰ�
				ProcessAttackResult(pAction->SKILL.dwAttackSerial,TRUE);
				// ��ƿ�~
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
			SetDestWarp(vPos);//������ ��ǥ�� ���� �ѵ� �ڷ���Ʈ ������Ʈ ����� ���� 
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
			assert(!"Character::DoNextAction() ó������ ���� ť ���");
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
        //	���� ���� �ʴ� ��Ÿ�� �Դϴ�
        g_InterfaceManager.GetInterfaceString( eST_NOT_EXIST_STYLE, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return FALSE;
    }

    //ĳ���� üũ
    BASE_PLAYERINFO* bp = GetCharInfo();
    if(bp==NULL)
    {
        //	��Ÿ���� ������ ĳ���� ������ �����ϴ�.
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
        //	�ش� ĳ���Ϳ� ������ �� ���� ��Ÿ���Դϴ�.
        g_InterfaceManager.GetInterfaceString( eST_NOT_APPLY_STYLE, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);

        return FALSE;
    }

    //���� üũ 
    if(GetWeaponKind()!=pInfo->m_iWeaponDefine)
    {
        //	����� ��Ÿ���� ��ġ���� �ʽ��ϴ�.
        g_InterfaceManager.GetInterfaceString( eST_NOT_STYLE_FOR_WEAPON, szMessage, INTERFACE_STRING_LENGTH );

        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, szMessage);
        return FALSE;
    }

    return TRUE;
}


//------------------------------------------------------------------------------
/**
Todo:��Ŷ ������ ���� �������� �ѹ� �� �˻� �ϴ°� ���ڴ�.
,��Ÿ�� ���� ��Ŷ������ ���� ���� �ٲܰ�� ������ ���� �Ұ�.
,���� ������ ����� ��Ÿ���� ��ġ ���� ���� ���� ��Ÿ�� ���� ���·� ��ȯ �Ѵ�.(��Ÿ�� ������ ���� �����Ҷ�)
�����ϴ� ���߿� �ٲٸ� ���� ���� Think.Ÿ�� ���δ�.��Ʈ ī��Ʈ�� �ʱ�ȭ �ؾ� �ϳ�....

*/
BOOL Player::SetCurrentAttackStyle(DWORD dwStyle)
{
    if( IsMyHero() == TRUE )
    {
        // �ڱ� ��Ÿ���� ���߿� �ٲ�� �޺��ʱ�ȭ 
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
���� �Ϸ��� ��Ÿ���� ���� ���� ����� ���� ���� �˻�  
*/
BOOL Player::CheckAttackStyle(DWORD dwStyle)
{
	BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo((SLOTCODE)dwStyle);
	//���� ���� �ʴ� ��Ÿ��
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
���� �Ϸ��� ���� Ÿ�԰� ��Ÿ���� �˻� �Ѵ�.
*/
BOOL Player::CheckStyleForWeapon(DWORD dwWeaponType)
{
	DWORD dwStyle =this->GetCurrentAttackStyle();

	BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo((SLOTCODE)dwStyle);
	//��Ÿ�� ������ ����.
	if(pInfo==NULL)
	{
		this->SetCurrentNomalAttackStyle(dwWeaponType);//��Ÿ�� ���� ����
		return FALSE;
	}
	else if(pInfo->m_iWeaponDefine!=dwWeaponType)	  //����� ��Ÿ���� �ȸ´ٸ� 
	{
		this->SetCurrentNomalAttackStyle(dwWeaponType);//��Ÿ�� ���� ����
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

			// Ÿ�÷��̾�� ������ ���� ������� ������.
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
���� ������ ���⸦ ���������� �ֵθ��� ���� ���������� ��û�Ÿ����ϰ�
�������� �ֵθ��� �������� ��û�Ÿ��� �ϴ� ��ƾ�� �����Ǿ�����
������ ���⸦ �ֵθ��� ���⿡ ���缭 bRight���ڰ� ����
*/
void Player::PlayDamageAnimation(BOOL bRight,BOOL bCritical)
{
	// �÷��̾�� �̵����̰ų� Idle�����϶���
	// �´� ������ ���
	WzID damageAnimation = 0;
	BOOL bAniCritcal = FALSE;

    // Ż���� ź ���¿����� ũ��Ƽ�� �ִϸ��̼��� ������� �ʴ´�.
    // ũ��Ƽ�� ������ �ִϸ��̼��� ��� ��ü�� ���� �ִϸ��̼��� �ֱ� ������
    // Ż���� ź ���¿��� �ִϸ��̼� �����ϴ� ���� Ż�� ���� �Ͼ�� �ȴ�.
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
	//�������� ũ�Ⱑ MAX_CHARNAME_LENGTH+1������ �������� �ι��ڷ� ä������
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

	// ���⿡���� �⺻�ڼ��� �ٲ�ϴ�.
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
			assert(pItemInfo && "������ ���� ID�� �߸��Ǿ����ϴ�.");
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

    // ���� �ʹ� ������ ����
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
/**	���� SD
*/
DWORD Player::GetSD()
{
	return GetCharInfo()->m_iSD;
}
//------------------------------------------------------------------------------
/** �ִ� SP
*/
DWORD Player::GetMaxSD()
{
	return GetCharInfo()->m_iMaxSD;	
}
//------------------------------------------------------------------------------
/**	SP ����
*/
void Player::SetSD(DWORD dwSD)
{
	this->GetCharInfo()->m_iSD = dwSD;
	Character::SetSD(dwSD);
}
//------------------------------------------------------------------------------
/** �ִ� SP ����
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
        // ���̵� ������ �ִ��ڵ�
        if (GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            // ���Ⱑ ����϶��� �ִ��ڵ�
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
        // ���̵� ������ �ִ��ڵ�
        if (GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            // ���Ⱑ ����϶��� �ִ��ڵ�
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
        // ���̵� ������ �ִ��ڵ�
        if (GetWeaponKind() == eWEAPONTYPE_DAGGER)
        {
            // ���Ⱑ ����϶��� �ִ��ڵ�
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
Todo:���⿡ ���� ������ �߻�ü ���� ó�� 
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

        DWORD MoveTime = max(200, (DWORD)(800.f * fDistance / GetAttackRange())); // �����Ÿ��� 2.5�ʿ� �����ϴ� �ӵ�


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

		DWORD MoveTime = max(200, (DWORD)(800.f * fDistance / GetAttackRange())); // �����Ÿ��� 2.5�ʿ� �����ϴ� �ӵ�


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

        DWORD MoveTime = max(200, (DWORD)(733.0f * fDistance / GetAttackRange())); // �����Ÿ��� 2.5�ʿ� �����ϴ� �ӵ�

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
			eColorType = eDMGPOP_COLOR_CRITICAL; // �÷��̾�� ������ ���������� �߰���.
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

	// ����γ�?
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
		// �̵���
	case STATE::MOVE:
	case STATE::KEYBOARDMOVE:
		{		
			szAniCode[3] = '3';
			szAniCode[3] += m_iValkyrieShotAnimVariation;
		}
		break;
		// �Ϲ�
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
		// ���ý��ĵ� ������ ���������� �ִϸ��̼Ǽ����� �ٽ����ش�.
		AnimID.id = GetCurrentIdleAnimation();

        //! ���� ����(idle/move) ������ �ٸ� ��� üũ
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
						// TODO: gs_bug_fix 09.07: �ϴ� ���������س����� ���� �����������ϸ鼭 ����..
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

	// �ΰ� ȿ�� ����
	BOOL bCritical = dwAdditionalEffect & (ATTACK_ADDITIONAL_EFFECT_CRITICAL);

#ifdef _NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    if (GetHideSkill() != HIDE_SKILL_INFO::ALL_HIDE)
#endif //_NA_007113_20140324_SHADOW_SKILL_BALLANCE_RENEWAL
    {
        // �´� ����Ʈ�� ���´�
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

	//���� ǥ�� 
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

	// �÷��̾�� �̵����̰ų� Idle�����϶���
	// �´� ������ ���
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
            // ������ ������� ���� ����Ʈ�ε� ����Ʈ�� ���°�� ����
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
//    // ������ ��뿡 ���� ��ȸ�� ����Ʈ�� �����ϴ� �Լ�
//    // ��������Ʈ�� �� �ڵ��� ������ ��ü�� �����Ƿ� ���⿡�� �ٷ������ �ʴ´�
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
//	// ������ ������ Ÿ���� �ִ� �������� ��� �ڵ� ó�� 
//	if( pItemInfo->m_byEffectPosition == eSKILLEFFECT_POS_LRHAND )
//	{
//        // ��տ��� 2�� �����Ѵ�
//
//		WzVector vPos = GetSkillEffectPos(eSKILLEFFECT_POS_RHAND );
//		HANDLE hHandle = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, vPos, GetObjectKey());
//
//        // ��������Ʈ�� �ڵ��� ������ ��ü�� ���⶧���� �ٷ� �����Ѵ�
//		pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
//		if (pBody && pBody->IsLoop())
//		{
//			g_pEffectManager->RemoveEffectPtr(hHandle);
//		}
//
//		vPos = GetSkillEffectPos(eSKILLEFFECT_POS_LHAND );
//		hHandle = GlobalFunc::CreateEffectBeUpToOption(pItemInfo->m_dwEffectCode, vPos, GetObjectKey());
//
//        // ��������Ʈ�� �ڵ��� ������ ��ü�� ���⶧���� �ٷ� �����Ѵ�
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
//        // ��������Ʈ�� �ڵ��� ������ ��ü�� ���⶧���� �ٷ� �����Ѵ�
//		pBody = g_pEffectManager->GetEffectPtrHandle(hHandle);
//		if (pBody && pBody->IsLoop())
//		{
//			g_pEffectManager->RemoveEffectPtr(hHandle);
//		}
//	}	
//
//	return TRUE;
//}

//Todo:���� ������ �������� ��� �´�.
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
	//Todo:������ ����Ʈ ���� �۾� 	
    return RemoveItemEffectByItemCode( dwItemCode );
}

void Player::OnLevelUp()
{
    //	%s���� ������ �ϼ̽��ϴ�
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

    if (dwLevel % 3 == 0) //3���� ���� ��ų����Ʈ�� ������ ����
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

	// ���� ���� ����
	if( pBaseStylenfo->m_iAttackRate )
		SkillCalc.AddAttr( eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO, VALUE_TYPE_VALUE, pBaseStylenfo->m_iAttackRate );

	// ��� ���� ����
	if( pBaseStylenfo->m_iAvoidRate )
		SkillCalc.AddAttr( eATTR_PHYSICAL_ATTACK_BLOCK_RATIO, VALUE_TYPE_VALUE, pBaseStylenfo->m_iAvoidRate );

	// ���� ����
	if( pBaseStylenfo->m_iAttackSpeed )
		SkillCalc.AddAttr( eATTR_ATTACK_SPEED, VALUE_TYPE_VALUE, pBaseStylenfo->m_iAttackSpeed );

	// ���� ����
	if( pBaseStylenfo->m_iBonusDefence )
		SkillCalc.AddAttr( eATTR_OPTION_ALL_DEFENSE_POWER, VALUE_TYPE_VALUE, pBaseStylenfo->m_iBonusDefence );

	// ũ��Ƽ�� Ȯ�� ����
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
		if ( IsShowName() == TRUE ) // ĳ�� ���ý� ( etc. �������� �� m_bShowName �ϱ�??)
			renderInfo_InCharacterSenen();
		else
			renderInfo_InBattleScene();
	}

	Character::RenderInfo();
}


float Player::GetAttackRange( void )
{
	const float PUNCH_ATTACK_RANGE = 2.0f;

	// ���⿡���� �⺻�ڼ��� �ٲ�ϴ�.
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
            // ��Ȱ��ȭ�� ���� ����� �߰�
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
			Sprintf(szDebugMsg, _T("������ ���� ID�� �߸��Ǿ����ϴ�[%d]"), rSlot.GetCode());
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

	if(IsTransForm())//���� ���϶�
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
    // ���ð� ����
    if (current_ani_progress_rate() >= 100)
    {
        // ���ð� �ο�
        SetSpaSittingAnimationDelay();
    }

    // ������ ���
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
                // ��õ �ɱ� IDLE �ִϸ��̼�
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
            // ��õ �ɱ� IDLE �ִϸ��̼�
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
        // �ɱ�IDLE �ִϸ��̼� �ٲٴ� Ÿ��
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
        // �ɱ� �ִϸ��̼� �ٲٴ� Ÿ��
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

	// ���� ���ݿ� ���� ó���� Ȯ���� ���ش�.
	// �̰� ���� �ؾ����� �Ǳ����̵�� -> �ٿ�
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
	// Ű�����̵�������
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
			//ȭ��Ÿ��
			Character *pTarget = (Character *)GetTargetObject();
			if (pTarget)
			{
				WzVector vDest,vTemp;
				float fArrowAngleZ,fArrowAngleX;

				vTemp=pTarget->GetVisiblePos();
				vTemp.z+=pTarget->GetArrowTargetHeight();

				vDest = vTemp-GetArrowBonePos(iShotCount);//���� ��ġ���� ���� ��ġ�� �������.
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

				fArrowAngleX = -atan2(vDest.z,fLength);			//���� �������� �ƴ� ���� �������� ����
				fArrowAngleZ = Math_GetAngleFromVector(&vDest); //���� �������� �ƴ� ���� �������� ����

				vDest=pTarget->GetVisiblePos();
				vDest.z+=pTarget->GetArrowTargetHeight();

				HANDLE handle;

				handle=GlobalFunc::CreateEffectBeUpToOption(StrToWzID("A041"),vDest,GetObjectKey());//!loop ����Ʈ ���� ����

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

	// ���ݰ����̶� ���������̸� ���ݺҰ�.
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
			// Ǫ���� ���� 
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
	// ĳ���� ���ý� 
	if ( IsShowName() == TRUE )
	{
		//	�ش� �÷��̾ GM ĳ�����ΰ�?
		if( IsGM() )
		{
			//	���� GM ĳ���� �ΰ�?
			if( g_pHero && g_pHero->IsGM() )
			{
				//	���� GM �̸� �÷��̾� ������ ������Ʈ Ű�� ǥ��
				if( g_pHero->GetAccountState() )
				{
					//	��ī��Ʈ�� �������� ������
					Sprintf(szResult, _T("%ld %s[%d]"), GetCharInfo()->m_LV, player_name, GetObjectKey());
				}
				else
				{
					Sprintf(szResult, _T("%s"), player_name);
				}
			}
			else
			{
				//	GM ĳ���ʹ� ������ ǥ������ �ʴ´�
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
		// ���콺 �ö󰣳� ��´�.
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
					// ĳ������ ���¸� ���´�. ���λ������� ��, 1:1 �ŷ� ��.....
					_fillRenderinfoState();	

					//	���� GM ĳ���� �ΰ�?
					if( g_pHero && g_pHero->IsGM() )
					{
						//	���� GM �̸� �÷��̾� ������ ������Ʈ Ű�� ǥ��
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
						//	GM ĳ���ʹ� ������ ǥ������ �ʴ´�
						if (IsPlayerHaveRenderinfoState())
						{
							Sprintf(szResult, _T("%s %s"), player_name, m_szCharState);
						}
						else
						{
							Sprintf(szResult, _T("%s"), player_name);
						}
					}
					//	GM ĳ���ʹ� �ٸ������� ���
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
    // �ٸ� �÷��̾� �̸� ���̱Ⱑ on�̰� Hero�� �ƴ� �� ���δ�.
    if (GAMEOPTION->GetShowPlayerName() && 
        (g_pHero && g_pHero->GetObjectKey() != GetObjectKey()))
    {
        return TRUE;
    }
    // �ٸ� �÷��̾� �̸� ���̱Ⱑ off�̰� ���콺 ������ �Ǿ� �ְų� Hero�� ���� ���δ�.
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
		//	[1:1 �ŷ�/��ȯ] 
		g_InterfaceManager.GetInterfaceString( eST_PLAYER_BEHAVE_TRADE_STATE, m_szCharState, INTERFACE_STRING_LENGTH );
	}
	else if (this->m_BehaveState == PLAYER_BEHAVE_NPCSHOP_STATE)
	{
		//	[���� ������] 
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
	
	//���� ������ ���� ���� ���� ���� ����� ���װ� �ִ�.ó�� ���� ��ȿ���� ���� ��ǥ�� �´ٸ� ���� ���� �ȵ�
	if (m_dwCurTileAtt & (PTA_NO_WALK))
	{
		rt=FALSE;
	}


	if(!rt&&m_iOldTile>=0)//������ ������ ���� ������
	{		
		g_pSunTerrain->PathExp_Reset( m_pPathExplorer, vOldPos,iOldTile);//������ ���϶� ���� ��ǥ�� ����
		AfterRefreshTileAtt();	
		m_dwCurTileAtt=m_dwOldTileAtt;
			
	}
	
	return rt;
}

void Player::BeforeRefreshTileAtt()
{
	//Ű���� �̵� �̿ܿ� ������ ������ ����
	m_iOldTile=m_pPathExplorer->GetTile();

	//���� �Ӽ��� �ִٸ� ���� �Ӽ� ���� ������ ���� ���� 
	
	m_dwOldTileAtt=m_dwCurTileAtt;

}

void Player::AfterRefreshTileAtt()
{
	m_iCurTile=m_pPathExplorer->GetTile();

	//Ÿ���� �ٲ���ų� �ʱ�ȭ�� �ȵǾ������ 
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
        // �ӽ� GM ���� Ǯ��
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
		//���������� ������������������ TextOnHead Color �ٲ۴�.
		DWORD dwAtt = GetCurrentTileAtt(FALSE);
		if(BIT_CHECK(dwAtt, PTA_CONFLICT_TILE) || BIT_CHECK(dwAtt, PTA_FREE_CONFLICT_TILE))
		{
			m_RenderinfoColor = c_EnemyTextColor;
		}
#ifdef _GS_GSP_PVP_ENEMY_COLOR
        else if(GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_PVP)
        {
            //PKŸ���� �ƴ�����  PVP��� �����Ƿ�
            m_RenderinfoColor = c_EnemyTextColor;
        }
#endif//_GS_GSP_PVP_ENEMY_COLOR
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
        else if(GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_FREEPVP)
        {
            //PKŸ���� �ƴ�����  PVP��� �����Ƿ�
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
			RenderTextOnHead(_T("����ī��"),m_RenderinfoColor,y,TRUE);

		}

		if(this->IsAttackBeginPlayer())
		{

			y+=1;
			RenderTextOnHead(_T("������"),m_RenderinfoColor,y,TRUE);
		}


		if(this->GetChaoState()==eCHAO_STATE_CHAO)
		{

			y+=1;
			RenderTextOnHead(_T("ī��"),m_RenderinfoColor,y,TRUE);

		}
	}
	//ĳ���� ���þ��� �ƴҰ��
	if( IsShowName() == FALSE)
	{
		// ���λ��� ���� ����
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
	// 2006-10-12 24:03 ( �д��� )
	if( _tcslen(m_tszDelDate) != 16 )
	{
		assert("RealTime Char del input string != 16");
		return 0;
	}
	TCHAR szNumber[5];
	tm endTM;
	// ��
	StrnCopy(szNumber , m_tszDelDate , 4);
	szNumber[4] = NULL;
	endTM.tm_year = _ttoi(szNumber) - 1900;
	// ��
	StrnCopy(szNumber, &m_tszDelDate[5], 2);
	szNumber[2] = NULL;
	endTM.tm_mon = _ttoi(szNumber) - 1;
	// ��
	StrnCopy(szNumber , &m_tszDelDate[8] , 2);
	szNumber[2] = NULL;
	endTM.tm_mday = _ttoi(szNumber) + CHAR_DEL_DAYS_OVER_40_LEVEL ;

	// ��
	StrnCopy(szNumber , &m_tszDelDate[11] , 2);
	szNumber[2] = NULL;
	endTM.tm_hour = _ttoi(szNumber);
	//��
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
    // 1�� ���� ū��
    if( RemainTotalTime >= 86400)
    {
        day = RemainTotalTime / 86400;
        remainder = RemainTotalTime % 86400;
    }
    // 1�ð� ���� ū��
    if( RemainTotalTime >= 3600 )
    {
        hour = remainder / 3600;
        remainder = RemainTotalTime % 3600;
    }
    // �� ���� �ΰ�
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
		// 5038 ���� �����
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
			// ĳ������ ���¸� ���´�.
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
		Snprintf(pszOutString,MAX_CLASSNAME_LENGTH, _T("�׽�Ʈ ĳ��"));
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
		//	����Ŀ
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
		//	����
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_DRAGON, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

	case eCHAREX_SHADOW_100LEVEL:
	case eCHAR_SHADOW:
		//	�ε���
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_SHADOW, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

	case eCHAREX_VALKYRIE_100LEVEL:
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_EX_NAME_VALKYRIE, pszOutString, MAX_CLASSNAME_LENGTH );
		break;
	case eCHAR_VALKYRIE:
		//	��Ű��
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_VALKYRIE, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

	case eCHAREX_ELEMENTALIST_100LEVEL:
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_EX_NAME_MAGICIAN, pszOutString, MAX_CLASSNAME_LENGTH );
		break;
	case eCHAR_MAGICIAN:
		//	������Ż ����Ʈ
		g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_MAGICIAN, pszOutString, MAX_CLASSNAME_LENGTH );
		break;

    case eCHAREX_MYSTIC_100LEVEL:
    case eCHAR_MYSTIC:
        // �̽�ƽ //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_MYSTIC, pszOutString, MAX_CLASSNAME_LENGTH );
        break; 
    case eCHAREX_HELLROID_100LEVEL:
    case eCHAR_HELLROID:
        // ����̵� //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_HELLROID, pszOutString, MAX_CLASSNAME_LENGTH );
        break;
    case eCHAREX_WITCHBLADE_100LEVEL:
    case eCHAR_WITCHBLADE:
        // ��ġ���̵� //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        g_InterfaceManager.GetInterfaceString( eST_CHARACTER_CLASS_NAME_WITCHBLADE, pszOutString, MAX_CLASSNAME_LENGTH );
        break;
	default:
		{
			Snprintf(pszOutString,MAX_CLASSNAME_LENGTH, _T("���ǵ�������"));
			assert(! "Ŭ�������� �߸� ���� ");
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

	// ��ȭ�� ǥ���ϰ� �ִ°�� �н�
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
        //    //! ���� �����̸� ���� ���� �÷� �ش�.
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

		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint((TCHAR*)player_name, lstrlen(player_name), &textSizeName);// �۷ι� ���������� ���Ƽ� error�߻� (TCHAR*)�ʿ� ���� ���� ���

		SIZE BigTextSize = textsizeGuild.cx > textSizeName.cx ? textsizeGuild : textSizeName;
	

		RECT rtGuildMark;
		//��帶ũ �׵θ� �̹����� ����� Ŀ���鼭
		//�¿��Ī�� ���� ��帶ũ�� ��ġ�� �������� �ణ �о���.
		rtGuildMark.left = pt2.x - 24 - (BigTextSize.cx / 2) - 10;
		rtGuildMark.top = pt2.y - 12 - 1;
		rtGuildMark.right = pt2.x - (BigTextSize.cx / 2) - 10;
		rtGuildMark.bottom = pt2.y + 12 - 1;

        eGUILD_MARK_RELATION eRelation = eGUILD_MARK_RELATION_EMPTY;
        BASE_GuildMarkInfo* pMarkInfo_Relation = NULL;


#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        if (false)
        {
            // �׳� ����������
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
                        //��� ��޸� üũ �Ѵ�.(���� ������ üũ ����)
                        if (GuildRightInfoParser::Instance()->CheckGradeForRight(eGUILD_RIGHTS_RELATION, 
                            m_GuildRenderInfo.m_GuildGrade))
                        {
                            //3���� �̻��� ��� ������ ���
                            eRelation = eGUILD_MARK_RELATION_BLUE;
                            pMarkInfo_Relation = GuildMarkInfoParser::Instance()->GetGuildMarkInfo(eGUILD_MARK_RELATION_BLUE);
                        }
                        else
                        {
                            //3���� ������ ��� ��帶ũ�� �������� ���
                            if (IsExistGuildMark())
                            {
                                if ((g_pHero->GetGuildRenderInfo().m_GuildGuid == m_GuildRenderInfo.m_GuildGuid)) //Hero�� ���� ����
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
                        if((g_pHero->GetGuildRenderInfo().m_GuildGuid == m_GuildRenderInfo.m_GuildGuid)) //Hero�� ���� ����
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
                            if((g_pHero->GetGuildRenderInfo().m_GuildGuid == m_GuildRenderInfo.m_GuildGuid)) //Hero�� ���� ����
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
            // ���� �� ��ũ ��� (��帶ũ ���)
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
                    if (g_pHero && IsPartyMember()) // �� ���õǱ� ����  ��Ƽ���̸� ����ũ �������
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
                    //�ܰ��� �巯����
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
            //�ܰ��� �Ⱥ��̰� (ũ��)
#else
            //�ܰ��� �巯����
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
            right_rect.left = pt2.x + (BigTextSize.cx / 2) + 9/*����*/;
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
				right_rect.left = pt2.x + (BigTextSize.cx / 2) + 9/*����*/;
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
            right_rect.left = pt2.x + (BigTextSize.cx / 2) + 9/*����*/;
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

	//���׸� ������, ���⿡ ���� ����Ʈ��  ��� ���´�.
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

	//���⿡ ���� ���׸�����Ʈ��ũ��Ʈ �������ٸ� ����
	if (!pInfo) 
    {
        return;
    }

	
	//����ó������: Ư������� �ΰ��� �ٿ��ߵȴ�(������ ���,�巡�ﳪ��Ʈ�� ��Į)
	
	
	//�⺻������ ����� 1��	
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

	
	//���������鼭(���ʼ�,�����ʼ�)
	for(int i=0;i<nWeaponNum;++i)
	{
        if (i >= (int)GetAttachmentCount())
        {
            break;
        }

		//STEP1:��������� ������������
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


		//STEP:�Ѿ�� ���ڰ������� ����Ʈ����
		if( bShotEffect )
		{
			//�߻� ����Ʈ
			m_hEtherWeaponEffect[i][2] = GlobalFunc::CreateEffectBeUpToOption(pInfo->dwEffectCode[ETHER_EFFECT_SHOT],
				pWeaponUnitDraw->GetBonePos(pBoneInfo->m_iNode));
		}
		else
		{
			if( !bFillupEffect) continue;
			
			int BulletIndex = GetEtherBulletEffectIndex();
			//���� ���� ����Ʈ(����)	
			m_hEtherWeaponEffect[i][0] = GlobalFunc::CreateEffectBeUpToOption(pInfo->dwEffectCode[BulletIndex], 
				pWeaponUnitDraw->GetBonePos(pBoneInfo->m_iNode));

			//�����ü�������Ʈ(�ܹ߿�)
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

	//��ũ��Ʈ ���������� ����
	if (!pInfo) 
    {
        return;
    }
	
	int nWeaponNum = 1; //�⺻������ ����� 1��

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
            //�ι�������ϰ�
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
	// ��ȯ���� ������ ����
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
			pUnitDraw->SetSubType(1); // ���� Ÿ�� �ٲ��ش�.
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

// ���ɻ��� ���� ���� �� ��帶������ ��� ����Ʈ ����
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

        // �渶�� ��� ����Ʈ ����
        // Ÿ���� ����� ����Ʈ ������ ���� �ʴ´�
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
    // ����Ʈ�� �����Ǿ� �ִٸ� ����
    RemoveDominationStateEffect();
    
    // �ڽ��� ����UI(zonemark) ������Ʈ
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
    bool area_guild = false; // ��� ��� �� ���

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

        // �渶 �ٴ�����Ʈ ���� �ڵ� ����
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

        // ���ư��� ����Ʈ ����
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
                // GUILD_RENDER_INFO ������ Ŭ���̾�Ʈ���� ī���Ͽ� �����Ѵ�.
                // PLAYER_GUILD_RENDER_INFO ������ ��ŭ�� ī��
                // DOMINATION_STATE_TOTAL_INFO ������ SetDominationStateInfo() �Լ��� ���� ����
                memcpy(&m_GuildRenderInfo, pGuildInfo, sizeof(m_GuildRenderInfo));

                SetGuildName(m_GuildRenderInfo.m_tszGuildName, MAX_GUILDNAME_LENGTH);
                pInfo->m_GuildGuid = m_GuildRenderInfo.m_GuildGuid;

                CheckDominationRelation(); // ������ �ȿ����� ���Ͱ��� ����
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
	// ����Ʈ ���� ���� - �̵� �� ������
	if (false == player_ranking_grade_effect_list_.empty())
	{
		// ���̰� ���
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

// ����ȭ�� ������ ����� ���ΰ�. (�ε��� �� �� �÷��̾�)
BOOL Player::IsSimpleRenderMode()
{
	// �÷��̾��� ��쳪������ true, �ε��� �Ϸ�Ǿ�߸� true �׷��� ������ false
	// �������� true

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
    //! ���Կ� �ִ� ���뵵 ������ش�. ���� �������� �ϳ��� ���� ����
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
                //! space key ���������� ������� ����
                player_ani = spread_player_ani;
                return move_other_motion;
            }
        }
        else
        {
            //���� �ִ� ���¿��� space key�� ��������. �ٸ� �ִϸ��̼�
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
        // ����
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

    // ��ȭ�� ǥ���ϰ� �ִ°�� �н�
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
        //    //! ���� �����̸� ���� ���� �÷� �ش�.
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

        g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint((TCHAR*)player_name, lstrlen(player_name), &textSizeName);// �۷ι� ���������� ���Ƽ� error�߻� (TCHAR*)�ʿ� ���� ���� ���

        SIZE BigTextSize = textsizeGuild.cx > textSizeName.cx ? textsizeGuild : textSizeName;

        g_pSunRenderer->x_pManagerTextOut->EndUseFont();

        //------------------------------------------------------------------------------ 
        //! �ֻ��� ���
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

        // �������̸� TRUE, �������̸� FALSE
        enum {DEFENDER = 0, ATTACKER = 1};
        BOOL hero_section = ATTACKER;  // ����� ��
        BOOL player_section = ATTACKER; // �÷��̾� ��

        // ��.���� ���� �Ǵ�
        {
            uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
            if (ui_domination_manager)
            {
                // ���� ������ ����������� Ȯ��
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
                // ��������� ��Ʈ������ �ʵ尡 �������� �ʾ� ��Ʈ���� ���� ���� link_map_code�� ã�Ƽ� ���� �ʰ� ������ code�� �����Ѵ�.
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
                
                // ������ ����Ʈ�� �ִ� �������
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

        if (hero_section == DEFENDER) // ����ΰ� ������ �϶�
        {
            if (player_section == DEFENDER) // �÷��̾ ������ �̸� �Ʊ�.
            {
                m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_ALLIENCE;
            }
            else
            {
                m_GuildRenderInfo.m_RelationType = eGUILD_RELATION_BOTH_HOSTILITY;
            }
        }
        else // ����ΰ� ������ �϶�
        {
            GUILD_RELATION* guild_relation = GuildSystem::GetGuildSystemData().GetGuildRelation(m_GuildRenderInfo.m_GuildGuid);
            if (guild_relation != NULL)
            {
                if ((player_section == ATTACKER) &&
                    (guild_relation->m_eRelationType == eGUILD_RELATION_ALLIENCE)) // �����̸鼭, ������ �̸� �Ʊ�
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

    // �ڿ� ����    
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
    
    // ���⼭ �������� �Ǵ��ؼ� �ѷ�����.
    // ���� ���̸� ����    
    domination_info_ptr = domination_manager_ptr->GetDominationGuildInfo();
    if (domination_info_ptr && (domination_info_ptr->m_GuildGuid == this->GetPlayerGuildID())
        || (domination_manager_ptr->IsDefenceGuildAlliance(this->GetGuildRenderInfo().m_GuildGuid)))
    {
        g_InterfaceManager.RenderRectWithTextureAndZTest(rect, depth, 1, 12);
    }
    else // �������̸� Į�� �������.
    {
        g_InterfaceManager.RenderRectWithTextureAndZTest(rect, depth, 0, 12);
    }
}
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL
