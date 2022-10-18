//------------------------------------------------------------------------------
//  PlayerMonster.cpp
//  (C) 2008 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "varpacket.h"
#include "ItemInfoParser.h"
#include "Player.h"
#include "Monster.h"
#include "PlayerMonster.h"
#include "Map.h"
#include "EquipmentContainer.h"
#include "ItemManager.h"
#include "AppearanceManager.h"
#include "ObjectManager.h"
#include "ProjectileManagerEx.h"
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
#include <NPCAttributes.h>
#include "CharacterScene.h"
#include "uiPartyMan/uiPartyMan.h"
#include "RewardInfoList.h"
#include "SunEventInfoParser.h"
#include "GameConst_Color.h"
#include "DelayLoadManager.h"
#include "ResourceManager.h"

using namespace util;

extern DWORD g_CurTime;

//------------------------------------------------------------------------------
/**
*/
PlayerMonster::PlayerMonster( void )
{
//	m_pEquipContainer	= NULL;
	m_eAttackSequence	= eATTACK_SEQUENCE_THIRD;

	SetObjectType(SSQMONSTER_OBJECT);	

	m_dwValkyrieShotSerial = 0;
	m_iCurTile=m_iOldTile=-1;    
	m_dwCurTileAtt=m_dwOldTileAtt=0;
	m_bValkyrieReload = FALSE;
	m_bInvisible = false;		//	����
	m_iValkyrieShotAnimVariation = 0;
	m_bHideHelmet = FALSE;
#ifdef _NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
    m_bHideWing = FALSE;
#endif//_NA_000000_20120420_EQUIPMENTS_VISIBLE_WING
	m_bInvincible = FALSE; // ����
	m_bCasting	  = FALSE;	
	m_iInvicibleTime =0;
	m_dwKeyboardMoveTime=0;
	m_fScale=1.0f;

	m_wAttSpeedRatio = 100;
	m_wMoveSpeedRatio = 100;
	m_iJumpFlag=0;
}

//------------------------------------------------------------------------------
/**
*/
PlayerMonster::~PlayerMonster( void )
{
    DestroyCharacterVariation();
}

//------------------------------------------------------------------------------
/**
*/
WzVector PlayerMonster::GetHeadBonePos()
{
	return GetSpecialBonePos(StrToWzID("HEAD"));
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetKeyboardMoveTime(DWORD dwTime)
{
	m_dwKeyboardMoveTime = dwTime;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::AddKeyboardMoveTime(DWORD dwDelta)
{
	m_dwKeyboardMoveTime += dwDelta;
}

//------------------------------------------------------------------------------
/**
*/
DWORD PlayerMonster::GetKeyboardMoveTime()
{
	return m_dwKeyboardMoveTime;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetMonsterInfo(BASE_NPCINFO *pInfo)
{
	if (pInfo) 
	{
		m_NPCInfo = *pInfo;

		if(m_NPCInfo.m_wSize / 100.0f>=0.0f)
			m_fScale = (float) m_NPCInfo.m_wSize / 100.0f; 
		SetScale(m_fScale);

        // pInfo->m_byGrade �� eNPC_SSQ_MONSTER(11) �� �ƴϸ� MONSTER_OBJECT ���� ����Ѵ�.
        // ���ֹ���ž�� ���� ������ ������ �÷��̾������Ͱ��� ��츦 
        // Ÿ���� ����� �÷��̾��� ���Ϳ� �����ϱ� ���Ͽ� MONSTER_OBJECT ���� �����Ͽ� ����Ѵ�.
        if (pInfo->m_byGrade != eNPC_SSQ_MONSTER)
        {
            SetObjectType(MONSTER_OBJECT);
        }
	}

	TCHAR	szString[INTERFACE_STRING_LENGTH] = {0,};

	g_InterfaceManager.GetInterfaceString(
		pInfo->m_NCode, 
		szString, 
		MAX_MONSTERNAME_LENGTH);

#ifdef _DEV_VER
    //#ifdef _SCJ_SHOW_NPC_ID
    if (CGeneralGameParam::Instance()->IsShowNpcId())
    {
        TCHAR buffer_string[INTERFACE_STRING_LENGTH] ={0,};          
        StrnCopy(buffer_string, szString, INTERFACE_STRING_LENGTH-1); 
        Snprintf(szString, INTERFACE_STRING_LENGTH-1, _T("%s <NPCID:%ld>"),
            buffer_string, pInfo->m_MonsterCode);
    }
    //#endif //_SCJ_SHOW_NPC_ID
#endif //#ifdef _DEV_VER

	SetName( szString );

	m_pNPCAttriburtes = new NPCAttributes;
	m_pNPCAttriburtes->SetOwner( &m_scNPCAtt);   
	m_pNPCAttriburtes->SetCharInfo(pInfo,0,0,0);
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetClass(eCHAR_TYPE eType)
{
    Monster::SetClass(eType);
}	

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::Create( DWORD dwObjectKey, DWORD dwID)
{
	SetObjectKey( dwObjectKey );
	m_dwParentId=0;
	m_dwTransFormCode=0;
	m_iAlarmVoice = 0;
	m_iWaitSoundDelay = 0;
	m_bNoCull = TRUE;
	m_eMonsterType = MONSTER_TYPE_GENERAL;
	m_iMonsterGaugeShowDelay = 0;
	m_iComboCount = 0;
	m_iComboDelay=0;

	//GetCharInfo()->m_byClassCode = (BYTE)dwID;
	//GetCharInfo()->SetChangeOfClass((eCHAREX_TYPE)dwID);

	m_fLandFrame=0.0f;

	APPEARANCEINFO appearInfo;
	ZeroMemory(&appearInfo,sizeof(appearInfo));
	appearInfo.ObjectType = APPEARANCEINFO::PLAYER;	
	appearInfo.Job = dwID;		// �ϴ� ID�� ĳ���� ������ ����. 

	BOOL rt = Character::Create(dwObjectKey, dwID);
	assert(rt);

    CreateCharacterVariation(&appearInfo, TRUE);
    
    InitContainer();

    m_iValkyrieShotDelay	= 0;
	m_iValkyrieShotCount	= 0;
	m_iValkyrieShotRemain	= 0;
	m_bSpecialMoveFlag=0;
	m_dwValkyrieShotSerial	= 0;	
	m_dwOldAttackStyle		= NONE_ID;
	m_dwDustTime			= 0;
	m_iValkyrieBulletCount = 20;
	m_iValkyrieShotAnimSet = 0;
	m_iValkyrieShotAnimCount = 0;
	m_bInvisible			= FALSE;

	m_dwShieldTime=0;
	m_bStandUp=FALSE;
	m_dwTransSkillCode=0;

    InitState(STATE::IDLE,g_CurTime);

    //InitAttachEffectForSetItem();

    InitMeshHeight();

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
    playermonster_costume_item_ = 0;
#endif //_NA_008185_20150402_COSTUME_ITEM_FOR_NPC

	return TRUE;
}

void PlayerMonster::SetPolymorph( BOOL Active )
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
		// ���� �ʱ�ȭ
		APPEARANCEINFO appearInfo;
		ZeroMemory(&appearInfo,sizeof(appearInfo));
		appearInfo.ObjectType = APPEARANCEINFO::PLAYER;
		appearInfo.Job = GetClass();

		InitRender(BP_COUNT, BP_BONE, &appearInfo, AppearanceManager::kAppearanceAttachIndex_Count, FALSE);
		RefreshAppearRender(TRUE);


		// NpcInfo�κ��� Scale�� �缳��
		SetMonsterInfo(GetMonsterInfo());

		//SetScale(m_fScale);
		SetIdleState();

		GlobalFunc::CreateEffectBeUpToOption( StrToWzID("KC05"), GetVisiblePos() );
	}
}

//------------------------------------------------------------------------------
/**
*/
WzVector PlayerMonster::GetTrailBonePos(int index, int start)
{
	return Object::GetTrailBonePos(index,start);
}

//------------------------------------------------------------------------------
/**
*/
WzID PlayerMonster::GetTrailEffectCode()
{
	if(GameFramework::GetCurrentScene()&&GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_SSQ)
		return Object::GetTrailEffectCode();

	return Monster::GetTrailEffectCode();
}

//------------------------------------------------------------------------------
/**
*/
DWORD PlayerMonster::IncreaseComboCount()
{
	m_iComboCount++;//�޺��� ���� 

	DWORD dwCurStyle = GetCurrentAttackStyle();//���ݽ�Ÿ���� ���� 

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

	if(IsRangedNormalAttack())
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
		if (m_iComboCount >= 3)
		{
			m_iComboCount = 0;
		}
	}

	return m_iComboCount;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetComboDelay(int combodelay)
{
	m_iComboDelay = combodelay;
}

//------------------------------------------------------------------------------
/**
*/
int PlayerMonster::GetComboDelay()
{
	return m_iComboDelay;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::Destroy( void )
{
	ReleaseContainer();

    Monster::Destroy();
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::Render( void )
{
	if( m_bObserverMode )
		return TRUE;

	if ( GetShow() == TRUE )
	{		
		if (this->m_bInvincible) 
		{
			SetColor( 154, 154, 154,100);

		}
	
		return Character::Render();	
	}
	else
	{
		return TRUE;
	}	
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::ApplyTerrainColor(BOOL bApply)
{
	if (!m_bInvincible) 
	{
		Character::ApplyTerrainColor(bApply);
	}
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::InitContainer()
{
    EquipmentContainer* equipment_container = new EquipmentContainer();
    equipment_container->SetOwnerKey(GetObjectKey());
    SetEquipmentContainer(equipment_container);
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::ReleaseContainer()
{
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container != NULL)
    {
        equipment_container->Release();	
        delete equipment_container;
        SetEquipmentContainer(NULL);
    }
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::ResetMoveFlag()
{
	m_iMoveFlag = 0;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::AddMoveFlag(int MoveFlag)
{	
	BIT_ADD(m_iMoveFlag,MoveFlag);
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::RemoveMoveFlag(int MoveFlag)
{
	BIT_REMOVE(m_iMoveFlag,MoveFlag);
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::CheckMoveFlag(int MoveFlag)
{
	return BIT_CHECK(m_iMoveFlag,MoveFlag);
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetMoveFlag(int MoveFlag)
{
	m_iMoveFlag = MoveFlag;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::RefreshEquipData()
{
    RefreshAppearRender();
    
}

//------------------------------------------------------------------------------
/**
*/
int PlayerMonster::GetMoveFlag()
{
	return m_iMoveFlag;
}

//------------------------------------------------------------------------------
/**
*/
float PlayerMonster::ProcessAnimation(DWORD dwTick,WzEvent *pEvent,BOOL bIgnoreEvent)
{	
	ProcessStandAnimationDelay(dwTick);
	return Monster::ProcessAnimation(dwTick,pEvent,bIgnoreEvent);
}

//------------------------------------------------------------------------------
/**
*/
WzID PlayerMonster::GetCurrentAttackAnimation()
{
	WzID animationID = StrToWzID("A001");
	DWORD dwStyle = GetCurrentAttackStyle();
	DWORD sequence=GetAttackSequence();
	DWORD weapon  =GetWeaponKind();

	BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo( (SLOTCODE)dwStyle );

	//safecode
	if(!pInfo)
	{
		WRITELOG(eLOG_LEVEL_ERR, c_wOUTPUT_LOG_FILE | c_wOUTPUT_LOG_CENTER, _T("BASE_STYLEINFO *pInfo is NULL-StyleCode[%d]"), dwStyle);
		assert( !"BASE_STYLEINFO *pInfo is NULL");
		return animationID;
	}

	// �Ѽռ����� �ƴϸ�
	if (weapon != eWEAPONTYPE_ONEHANDCROSSBOW)
	{
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

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::RefreshPunchStyle()
{
	switch(GetClass())
	{
	case eCHAR_BERSERKER:
		m_dwAttackStyle=eSTYLE_BERSERKER_PUNCH;
		break;
	case eCHAR_DRAGON:
		m_dwAttackStyle=eSTYLE_DRAGON_PUNCH;
		break;
	case eCHAR_SHADOW:
		m_dwAttackStyle=eSTYLE_SHADOW_PUNCH;
		break;
	case eCHAR_VALKYRIE:
		m_dwAttackStyle=eSTYLE_VALKYRIE_PUNCH;
		break;
	case eCHAR_ELEMENTALIST:
		m_dwAttackStyle=eSTYLE_MAGICIAN_PUNCH;
        break;
    case eCHAR_MYSTIC: //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        m_dwAttackStyle=eSTYLE_MYSTIC_PUNCH;
		break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        m_dwAttackStyle=eSTYLE_HELLROID_PUNCH;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        m_dwAttackStyle=eSTYLE_WITCHBLADE_PUNCH;
        
        break;
	}
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetCurrentNomalAttackStyle(DWORD dwWeaponType)
{
	switch(dwWeaponType)
	{
	case eWEAPONTYPE_TWOHANDAXE://����Ŀ 
		{
			m_dwAttackStyle=eSTYLE_TWOHANDAXE_NORMAL;
		}
		break;

	case eWEAPONTYPE_TWOHANDSWORD:
		{
			m_dwAttackStyle=eSTYLE_TWOHANDSWORD_NORMAL;
		}
		break;

	case eWEAPONTYPE_ONEHANDSWORD://���� 
		{
			m_dwAttackStyle=eSTYLE_ONEHANDSWORD_NORMAL;
		}
		break;
	case eWEAPONTYPE_SPEAR:
		{
			m_dwAttackStyle=eSTYLE_SPEAR_NORMAL;
		}
		break;

	case eWEAPONTYPE_DAGGER://������
		{
			m_dwAttackStyle=eSTYLE_SHADOW_DAGGER_NORMAL;
		}
		break;

	case eWEAPONTYPE_WHIP:
		{
			m_dwAttackStyle=eSTYLE_SHADOW_WHIP_NORMAL;
		}
		break;

    case eWEAPONTYPE_ONEHANDCROSSBOW://��Ű��
        {
            m_dwAttackStyle=eSTYLE_ONEHANDCROSSBOW_NORMAL;
        }
        break;
    case eWEAPONTYPE_ETHERWEAPON:
        {
            m_dwAttackStyle=eSTYLE_ETHER_NORMAL;
        }
        break;

	case eWEAPONTYPE_STAFF://���� 
		{
			m_dwAttackStyle=eSTYLE_STAFF_NORMAL;
		}
		break;

	case eWEAPONTYPE_ORB:
		{
			m_dwAttackStyle=eSTYLE_ORB_NORMAL;
		}
		break;

        //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eWEAPONTYPE_GUNTLET:
        {
            m_dwAttackStyle=eSTYLE_MYSTIC_GUNTLET_NORMAL;
        }
        break;
    case eWEAPONTYPE_POLEARM:
        {
            m_dwAttackStyle=eSTYLE_MYSTIC_POLEARM_NORMAL;
        }
        break;
        //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eWEAPONTYPE_HELLROID_1:
        {
            m_dwAttackStyle=eSTYLE_HELLROID_WEAPON1;
        }
        break;
        //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eWEAPONTYPE_ARCBLADE:
        {
            m_dwAttackStyle=eSTYLE_WITCHBLADE_ARCBLADE;
        }
        break;
	case eWEAPONTYPE_PUNCH:
		{
			RefreshPunchStyle();
		}
		break;
	}
}

//------------------------------------------------------------------------------
/**
*/
WzID PlayerMonster::GetCurrentIdleAnimation()
{
	WzID animationID = 0;	

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = '0';
	szAniCode[2] = '0';
	szAniCode[3] = '1';

	szAniCode[1] = GetWeaponAnimationCode();

	if (IsFainting())
	{
		szAniCode[0] = 'F';

		szAniCode[3] = '4';
	}
	else 
	{
		// ���ý��ĵ�� �ڵ� ���� 3 �⺻�� 1�̴�.
		if (m_iAttackStandTime > 0 )
		{
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
			if (m_dwStandAnimDelay) 
			{
				szAniCode[3] = '1';			
			}
			else
			{
				szAniCode[3] = '2';
			}
		}
	}

	animationID = StrToWzID(szAniCode);

	return animationID;
}

WzID  PlayerMonster::GetCurrentReviveAnimation()
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
WzID PlayerMonster::GetCurrentMoveAnimation()
{
	WzID animationID = 0;	
	char szAniCode[5];
	ZeroMemory(szAniCode,sizeof(szAniCode));

	if (GetMoveState() == CMS_WALK) 
	{
		szAniCode[0] = 'N';
		szAniCode[1] = GetWeaponAnimationCode();	
		szAniCode[2] = '0';
		szAniCode[3] = '6';
	}
	else
	{
		szAniCode[0] = 'N';
		szAniCode[1] = GetWeaponAnimationCode();	
		szAniCode[2] = '0';
		szAniCode[3] = '7';
	}


	animationID = StrToWzID(szAniCode);
	return animationID;	
}

//------------------------------------------------------------------------------
/**
*/
WzID PlayerMonster::GetCurrentBlockAnimation()
{
	WzID animationID = 0;	
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
WzID PlayerMonster::GetCurrentDeathAnimation()
{
	WzID animationID = 0;
	int i=0;
	while(1)
	{
		if(i==10)
			animationID = StrToWzID("D001");	

		int iType=rand()%3;

		if(iType==0)
			animationID = StrToWzID("D001");	

		else if(iType==1)
			animationID = StrToWzID("D002");	

		else if(iType==2)
			animationID = StrToWzID("D003");	

		CWzUnit* pUnit=GetBasePart()->GetWzUnit();

		if(!pUnit)
			return StrToWzID("D001");

		WzAnimationInfo *pAnimationInfo =pUnit->GetAnimationInfo(animationID);

		if(!pAnimationInfo)
		{
			return StrToWzID("D001");
		}

		if(pAnimationInfo->m_wiAction==animationID)
		{
			break;
		}
		i++;
	}

	return animationID;
}

//------------------------------------------------------------------------------
/**
*/
WzID PlayerMonster::GetCurrentDamageAnimation(BOOL bRight,BOOL bCritical)
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
WzID PlayerMonster::GetCurrentSitAnimation()
{
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
*/
WzID PlayerMonster::GetCurrentSittingAnimation()
{
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
*/
WzID PlayerMonster::GetCurrentStandUpAnimation()
{
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
void PlayerMonster::CreateDamageEffect(BOOL bCritical,BOOL bRight,DWORD dwAttackObjId)
{
	Monster::CreateDamageEffect(bCritical,bRight,dwAttackObjId);	
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetOldMoveFlag(int iOldMoveFlag)
{
	m_iOldMoveFlag = iOldMoveFlag;
}

//------------------------------------------------------------------------------
/**
*/
int PlayerMonster::GetOldMoveFlag()
{
	return m_iOldMoveFlag;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetOldKeyboardDirection(WzVector *pVector)
{
	m_vOldDirection = *pVector;
}

//------------------------------------------------------------------------------
/**
*/
WzVector PlayerMonster::GetOldKeyboardDirection()
{
	return m_vOldDirection;
}
//
////------------------------------------------------------------------------------
///** �÷��̾� ���� ��������
//*/
//void PlayerMonster::SetPlayerInfo(PLAYER_RENDER_INFO_EX* pPlayerInfo)
//{
//	ZeroMemory(GetCharInfo(),sizeof(BASE_PLAYERINFO));
//
//	GetCharInfo()->m_byClassCode	= pPlayerInfo->char_class;
//	GetCharInfo()->SetChangeOfClass( (eCHAREX_TYPE) pPlayerInfo->char_class);
//	GetCharInfo()->m_LV				= pPlayerInfo->level;
//	m_dwAttackStyle					= pPlayerInfo->selected_style_code;
//	m_wMoveSpeedRatio               = pPlayerInfo->move_speed_ratio;
//	m_wAttSpeedRatio	            = pPlayerInfo->attack_speed_ratio;
//	m_pPlayerAttriburtes->SetCharInfo( m_PlayerData.GetCharacterInfo(), 0, 0, 0 );
//	SetPartVariation(PLAYER_VARIATION_HAIR,pPlayerInfo->hair_style);
//	SetPartVariation(PLAYER_VARIATION_FACE,pPlayerInfo->face);
//	SetHeightVariation(pPlayerInfo->height);
//
//	SetStateTotalInfo(pPlayerInfo->state_total_info);
//}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::ValkyrieShot(DWORD dwAttackSerial)
{
	HideAttachment(FALSE);
	SetAttackStandTime(ATTACK_STAND_TIME);
	m_fValkyrieShotSpeed = GetAttackSpeedModifier();
	SetCurrentBlendAnimation(GetValkyrieShotAnimation(),GetShotBlendBone(),m_fValkyrieShotSpeed,0);
	OpenCrossbow();

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
		m_iValkyrieShotDelay = 500;	
	}
	m_iValkyrieShotDelayTime = m_iValkyrieShotDelay;
	m_iValkyrieShotCount = 0;
	m_iValkyrieShotRemain = 2;

	m_bValkyrieReload = FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::CanValkyrieAction()
{
	// ��� ���ɻ���!
	if(  GetCurState() == STATE::KEYBOARDMOVE || GetCurState() == STATE::MOVE )
	{
		return TRUE;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::Process(DWORD dwTick)
{
	g_dwCharProcessCount++;
	ProcessComboDelay(dwTick);
	BeforeRefreshTileAtt();

	if (GetClass() == eCHAR_VALKYRIE)
	{
		ProcessValkyrieShot(dwTick);
	}

	//�ð��� ������ �ڵ����� ���ش�.
	if(m_bInvincible&&m_iInvicibleTime)
	{
		m_iInvicibleTime-=dwTick;

		if(m_iInvicibleTime<=0)
		{
			m_iInvicibleTime=0;
			m_bInvincible=FALSE;
		}
	}

	m_dwShieldTime-=dwTick;
	m_dwShieldTime=max(m_dwShieldTime,0);

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

	BOOL bRet = Monster::Process(dwTick);
	AfterRefreshTileAtt();
	return bRet;
}

//-------------------------------------------------------------------------------------------
/**
*/
void PlayerMonster::ProcessComboDelay(DWORD dwTick)
{
	if (m_iComboDelay > 0)
	{
		if (GetCurState() != STATE::ATTACK)
		{
			m_iComboDelay -= dwTick;
		}
		if (m_iComboDelay <= 0)
		{
			m_iComboDelay = 0;
			m_iComboCount = 0;
			SetComboCount(0);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
int  PlayerMonster::GetComboCount()
{
	return m_iComboCount;
}

//------------------------------------------------------------------------------
/**
*/
void  PlayerMonster::SetComboCount(int iCount)
{
	m_iComboCount = iCount;
}

//------------------------------------------------------------------------------
/**
*/
int PlayerMonster::DoAction(PLAYER_ACTION *pAction)
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

				switch (GetComboCount())
				{
				case 0:
					SetAttackSequence(eATTACK_SEQUENCE_FIRST);
					break;
				case 1:
					SetAttackSequence(eATTACK_SEQUENCE_SECOND);
					break;
				case 2:
					SetAttackSequence(eATTACK_SEQUENCE_THIRD);
					break;
				}

				SetNextActionAttackSerial(pAction->ATTACK.dwAttackSerial);
				SetNextState(STATE::ATTACK,g_CurTime);
				SetComboCount(IncreaseComboCount());

				if(GetClass()==eCHAR_BERSERKER||GetClass()==eCHAR_DRAGON)
				{
					if(GetAttackSequence()==eATTACK_SEQUENCE_FIRST)
					{
						srand(pAction->ATTACK.dwAttackSerial);
                        GameFunc::SetPlayerMonsterAttackStyle(this);
					}
				}

				return 1;
			}
			else
			{
				ProcessAttackResult(pAction->ATTACK.dwAttackSerial,TRUE);
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
			
		}
		return 1;


	case ACTION_FALLDOWN:
		{
			StopMove();
			SetNextState(STATE::FALL_DOWN,g_CurTime);
		}
		return 1;

	case ACTION_AIR_FALLDOWN:
		{
			StopMove();
			SetNextState( STATE::AIR_FALL_DOWN, g_CurTime );
		}
		return 1;

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
			GameFunc::PickTerrainMesh(pAction->KEYBAORDJUMP.vDest,this->vVisibleDest);
			this->vSrc = this->GetVisiblePos();
			StopMove();	
			SetNextState(STATE::KEYBOARDJUMP,g_CurTime);
			
		}
		return 1;


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
			else
			{
				ProcessAttackResult(pAction->SKILL.dwAttackSerial,TRUE);
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
			SetDirection(vPos, TRUE);
		}
		break;

	case ACTION_KNOCKBACK:
		{
			StopMove();		
			SetNextState(STATE::DAMAGE,g_CurTime);	
		}
		return 1;

	case ACTION_KNOCKBACK2:
		{
			StopMove();		
			SetNextState(STATE::KNOCKBACK2,g_CurTime);	
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
		return 1;

	case ACTION_DOWN:
		{
			StopMove();	
			SetNextState(STATE::DOWN,g_CurTime);
		}
		return 1;

	case ACTION_GETUP:
		{
			StopMove();	
			SetNextState(STATE::GETUP,g_CurTime);
		}
		return 1;

	case ACTION_SIT:
		{
			StopMove();	
			SetNextState(STATE::SIT,g_CurTime);
		}
		return 1;

	case ACTION_EVENTJUMP:
		{
			StopMove();	
			SetNextState(STATE::EVENTJUMP,g_CurTime);
		}
		return 1;

	case ACTION_STOP:
		{
			StopMove();	
			SetPosition(pAction->STOP.vCurPos);
			SetNextState(STATE::IDLE,g_CurTime);
		}
		return 1;

	case ACTION_NPCMEET:
		{
			SetNextState(STATE::NPC_RESPONSE_STATE, g_CurTime);
			
		}
		return 1;

	case ACTION_VENDORMEET:
		{
		}
		return 1;
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
int PlayerMonster::GetValkyrieShotDelay()
{
	return m_iValkyrieShotDelay;
}


//------------------------------------------------------------------------------
/**
*/
DWORD PlayerMonster::GetCurrentAttackStyle()
{
	return m_dwAttackStyle;
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::SetCurrentAttackStyle(DWORD dwStyle,BOOL bCheck)
{
	m_dwAttackStyle = dwStyle;
	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::CheckAttackStyle(DWORD dwStyle)
{
	BASE_STYLEINFO *pInfo = SkillInfoParser::Instance()->GetStyleInfo((SLOTCODE)dwStyle);
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
BOOL PlayerMonster::CheckStyleForWeapon(DWORD dwWeaponType)
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
WzVector PlayerMonster::GetWeaponBonePos(int index)
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
void PlayerMonster::ProcessValkyrieShot(DWORD dwTick)
{
	const float fTrailHeight = 0.03f;
	const DWORD dwShotLifeTime = 500;

	const float fCrossBowShotFrame1 = m_iValkyrieShotDelayTime - ((2.8f * 33) / m_fValkyrieShotSpeed);
	const float fCrossBowShotFrame2 = m_iValkyrieShotDelayTime - ((5.3f * 33) / m_fValkyrieShotSpeed);

	const int VALKYRIE_SHOT_SOUND1 = 408;
	const int VALKYRIE_SHOT_SOUND2 = 409;

	if (m_iValkyrieShotDelay > 0) 
	{

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

//------------------------------------------------------------------------------
/**
*/
DWORD PlayerMonster::GetValkyrieShotSerial()
{
	return m_dwValkyrieShotSerial;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::PlayDamageAnimation(BOOL bRight,BOOL bCritical)
{
	// �÷��̾�� �̵����̰ų� Idle�����϶���
	// �´� ������ ���
	WzID damageAnimation = 0;
	BOOL bAniCritcal = FALSE;
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
void PlayerMonster::PlayDamageSound(eWEAPONSOUNDKIND eSoundKind)
{
    SOUND_INFO_CODE SoundInfoCode;
    SoundInfoCode.wCharacterCode = static_cast<WORD>(GetClass());
    SoundInfoCode.bWeaponType = static_cast<BYTE>(eSoundKind);
    SoundInfoCode.bGenderType = static_cast<BYTE>(ResourceCode::CheckValidGender(GetClass(), GetGender()));

	BASE_CharSoundInfo *pInfo = CharSoundInfoParser::Instance()->GetCharSoundInfo(SoundInfoCode.dwSoundCode);
	BASE_WeaponSoundInfo *pWeaponSoundInfo = WeaponSoundInfoParser::Instance()->GetWeaponSoundInfo(eSoundKind);

	assert(eSoundKind >= 0 && eSoundKind < eWEAPONSOUNDKIND_MAX);
	if (eSoundKind < 0 || eSoundKind >= eWEAPONSOUNDKIND_MAX)
	{
		return;
	}

	int iDamageVoice = 0;
	int iImpactSound = 0;
	int iWeaponSound = 0;

	if (pInfo != NULL)
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
int PlayerMonster::GetShotBlendBone()
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

//------------------------------------------------------------------------------
/**
*/
BASE_ITEMINFO* PlayerMonster::GetWeaponInfo()
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

//------------------------------------------------------------------------------
/**
*/
float PlayerMonster::GetMoveSpeedModifier()
{
	float fMoveSpeedModifier = m_wMoveSpeedRatio / 100.0f;
	return fMoveSpeedModifier;
}

//------------------------------------------------------------------------------
/**
*/
float PlayerMonster::GetAttackSpeedModifier()
{
	const float fWeaponMultiplier = 0.67f;
	float fFinalMultiplier = fWeaponMultiplier * m_wAttSpeedRatio / 100.0f;
	return fFinalMultiplier;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetExp(EXPTYPE exp)
{
	//this->GetCharInfo()->m_i64Exp = exp;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetLevel(LEVELTYPE LV)
{
	//this->GetCharInfo()->m_LV = LV;
}


//------------------------------------------------------------------------------
/**
*/
EXPTYPE PlayerMonster::GetExp()
{
    return 0;
	//return this->GetCharInfo()->m_i64Exp;
}


//------------------------------------------------------------------------------
/**
*/
LEVELTYPE PlayerMonster::GetLevel()
{
    return 0;
	//return GetCharInfo()->m_LV;
}

//------------------------------------------------------------------------------
/**
*/
EXPTYPE PlayerMonster::GetNextExp()
{
	return 0;
}

//------------------------------------------------------------------------------
/**
*/
EXPTYPE PlayerMonster::GetAccumExp(LEVELTYPE LV)
{
	BASE_ACCUM_EXP_INFO * pAccumExpInfo =  (BASE_ACCUM_EXP_INFO *)ExpValueInfoParser::Instance()->GetAccumExpInfo(LV);		
	if( !pAccumExpInfo )
	{	
		return 0;
	}
	return pAccumExpInfo->m_AccumExp;
}

//------------------------------------------------------------------------------
/**
*/
float   PlayerMonster::GetLandFrame()
{
	return m_fLandFrame;
}

//------------------------------------------------------------------------------
/**
*/
void    PlayerMonster::SetLandFrame(float fFrame)
{
	m_fLandFrame=fFrame;
}

//------------------------------------------------------------------------------
/**
*/
WzID	PlayerMonster::GetCurrentLeftTurnAnimation()
{
	char szAniCode[5];
	ZeroMemory(szAniCode,sizeof(szAniCode));
	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '2';

	return StrToWzID(szAniCode);

}

//------------------------------------------------------------------------------
/**
*/
WzID	PlayerMonster::GetCurrentRightTurnAnimation()
{

	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '3';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID	PlayerMonster::GetCurrentLeftStepAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '0';
	szAniCode[3] = '9';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID	PlayerMonster::GetCurrentRightStepAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '0';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID	PlayerMonster::GetCurrentBackStepAnimation()
{
	char szAniCode[5];

	ZeroMemory(szAniCode,sizeof(szAniCode));

	szAniCode[0] = 'N';
	szAniCode[1] = GetWeaponAnimationCode();
	szAniCode[2] = '1';
	szAniCode[3] = '1';

	return StrToWzID(szAniCode);
}

//------------------------------------------------------------------------------
/**
*/
WzID	PlayerMonster::GetCurrentSpecialRightStepAnimation()
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
WzID	PlayerMonster::GetCurrentSpecialFrontStepAnimation()
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
WzID	PlayerMonster::GetCurrentSpecialBackStepAnimation()
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
WzID	PlayerMonster::GetCurrentSpecialLeftStepAnimation()
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
WzID PlayerMonster::GetCurrentJumpStartAnimation()
{
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
WzID PlayerMonster::GetCurrentJumpKeepAnimation()
{
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
WzID PlayerMonster::GetCurrentJumpEndAnimation()
{
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
WzID PlayerMonster::GetCurrentJumpLandAnimation()
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
WzID PlayerMonster::GetCurrentJumpStunAnimation()
{
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
WzID PlayerMonster::GetCurrentJumpStunKeepAnimation()
{
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
WzID PlayerMonster::GetCurrentJumpStunEndAnimation()
{
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
WzID PlayerMonster::GetCurrentJumpEndRunAnimation()
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
void PlayerMonster::UpdateActiveSkillEffectAttr(SkillAttrCalculator &SkillCalc)
{
    std::vector<SKILL_EFFECT*>& array = GetSkillEffectArray();

    for( size_t i=0;i!=array.size();++i)
    {
        SKILL_EFFECT* pEffect = array[i];

		if( SkillScriptInfo* pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)pEffect->dwSkillID) )
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


//------------------------------------------------------------------------------
/**
// �÷��̾�� ����
Todo:���⿡ ���� ������ �߻�ü ���� ó�� 
*/
DWORD PlayerMonster::CreateProjectile(Character *pTarget,int iArrowBone)
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

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::CreatePopupMesh(
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
		FALSE
		);
}

//------------------------------------------------------------------------------
/**
*/
void  PlayerMonster::OpenCrossbow()
{
	if (GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW) 
	{
		SetAttachmentAnimation(0,StrToWzID("N002"),FALSE);
		SetAttachmentAnimation(1,StrToWzID("N002"),FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void  PlayerMonster::CloseCrossbow()
{
	if (GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW) 
	{
		SetAttachmentAnimation(0,StrToWzID("N001"),FALSE);
		SetAttachmentAnimation(1,StrToWzID("N001"),FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
void  PlayerMonster::ShotCrossbow()
{
	if (GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW) 
	{
		SetAttachmentAnimation(0,StrToWzID("N003"),FALSE);
		SetAttachmentAnimation(1,StrToWzID("N003"),FALSE);
	}
}

//------------------------------------------------------------------------------
/**
*/
WzID PlayerMonster::GetValkyrieShotAnimation()
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

//------------------------------------------------------------------------------
/**
*/
WzID PlayerMonster::GetValkyrieReloadAnimation()
{
	return StrToWzID("A211");
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetIdleAnimation()
{
	tagWZID AnimID;

	if(!IsBlock())
	{
		AnimID.id = GetCurrentIdleAnimation();
		if (AnimID.id != GetCurrentAnimation())
		{
			if (AnimID.code[3] == '2')
			{
				SetAnimation( AnimID.id, FALSE );
			}
			else
			{
				SetAnimation( AnimID.id, TRUE );
			}

			GetBasePart()->SetInterpolationFrame(6);
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::OnAttack(DWORD dwSkillCode)
{
	Character::OnAttack(dwSkillCode);

#ifndef _PERF_DEBUG
	BASE_SKILLINFO *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)dwSkillCode);

	if(pInfo)
	{
		switch (pInfo->m_SkillClassCode)//��ų�� ���� ����Ʈ ó�� 
		{
		case eSKILL_WIND_SHIELD:
			{
				if(m_dwShieldTime==0)
				{
					m_dwShieldTime=1000;

                    cTargetPosition Position;
                    Position.SetTargetVisiblePosition( GetObjectKey(), TRUE );
                    Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

                    g_ProjectileManagerEx.CreateEffect( GetObjectKey(), StrToWzID("K370"), 3000, Position );
				}
			}
			break;

		case eSKILL_ELECTRIC_FIELD:
			{
				if(m_dwShieldTime==0)
				{
					m_dwShieldTime=1000;

                    cTargetPosition Position;
                    Position.SetTargetVisiblePosition( GetObjectKey(), TRUE );
                    Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

                    g_ProjectileManagerEx.CreateEffect( GetObjectKey(), StrToWzID("K381"), 3000, Position );

				}
			}
			break;
		}
	}

#endif
}

//------------------------------------------------------------------------------
/**
*/

void PlayerMonster::OnDamage(const DAMAGE_PRAM &rDamage)
{
	Character::OnDamage(rDamage);

	DWORD dwAdditionalEffect = rDamage.pResult->GetTargetData().GetAdditionalEffect();

	SetAttackStandTime(ATTACK_STAND_TIME);
	BOOL bCritical = dwAdditionalEffect & (ATTACK_ADDITIONAL_EFFECT_CRITICAL);

	// �´� ����Ʈ�� ���´�
	CreateDamageEffect(bCritical,rDamage.bRight,rDamage.dwObjKey);	

	if ( !(rand() % 3) )
	{
		PlayDamageSound(rDamage.pResult->GetWeaponSound());														
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
        std::vector<SKILL_EFFECT*>& array = GetSkillEffectArray();

        DWORD dwState=0;
        for( size_t i=0;i!=array.size();++i)
        {
            SKILL_EFFECT* pEffect = array[i];
            if( pEffect->dwAbilityID == eABILITY_MAGIC_SHIELD )
            {
                m_dwShieldTime = 300;

                cTargetPosition Position;
                Position.SetTargetVisiblePosition( GetObjectKey(), TRUE );
                Position.SetOffsetPosition( 0.0f, 0.0f, 0.95f );

                g_ProjectileManagerEx.CreateEffect( GetObjectKey(), StrToWzID("K361"), 3000, Position );

                break;
            }
		}
	}
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::PlayEvadeAnimation()
{
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::PlayBlockAnimation() 
{
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


//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::AnimateUnit()
{
	Character::AnimateUnit();
}

//------------------------------------------------------------------------------
/**
*/
WzVector PlayerMonster::GetArrowBonePos(int iAttachIndex)
{
	// Player::GetArrowBonePos�� ����.

	if (!IsAnimated())
	{
		AnimateUnit();
	}

    if (iAttachIndex >= 0 && iAttachIndex < (int)GetAttachmentCount())
    {
	    CWzUnitDraw* pSkeleton = GetAttachment(iAttachIndex).GetUnitDraw();
	    if ((pSkeleton != NULL) && (pSkeleton->GetWzUnit() != NULL)) 
	    {
		    if( WzBoneInfo* pInfo = pSkeleton->GetWzUnit()->GetBoneInfo( StrToWzID("ARRW") ) )
		    {
			    WzVector vRet;
			    if ( GameFunc::GetBonePos( pSkeleton, pInfo->m_iNode, &vRet) == TRUE )
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

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::RenderInfo()
{
	Monster::RenderInfo();
}

//------------------------------------------------------------------------------
/**
*/
float PlayerMonster::GetAttackRange( void )
{
	const float PUNCH_ATTACK_RANGE = 2.0f;

	// ���⿡���� �⺻�ڼ��� �ٲ�ϴ�.
    EquipmentContainer* equipment_container = GetEquipmentContainer();
    if (equipment_container == NULL)
    {
        return PUNCH_ATTACK_RANGE;
    }

    SCSlot& rSlot = equipment_container->GetSlot(EquipmentContainer::EQUIP_WEAPON1);

	//float fBonus = (float)m_pPlayerAttriburtes->GetNormalRangeBonus() / 10.0f;

	if (rSlot.GetCode() == 0)
	{
		return PUNCH_ATTACK_RANGE;
	}
	else
	{
		BASE_ITEMINFO *pItemInfo = ItemInfoParser::Instance()->GetItemInfo(rSlot.GetCode());
		if(pItemInfo)
		{
			return (float)(pItemInfo->m_fAttRange );
		}
		else
		{
#ifdef _DEV_VER
			TCHAR szDebugMsg[1024] = {0,};
			Sprintf(szDebugMsg, _T("������ ���� ID�� �߸��Ǿ����ϴ�[%d]"), rSlot.GetCode());
			GlobalFunc::OutputMsg_SysMsg(szDebugMsg);
#endif
			return PUNCH_ATTACK_RANGE;
		}
	}	
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::IsRangedNormalAttack()
{
	switch (GetWeaponKind())
	{
	case eWEAPONTYPE_ONEHANDCROSSBOW:
	case eWEAPONTYPE_ETHERWEAPON:
	case eWEAPONTYPE_ORB:
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

//------------------------------------------------------------------------------
/**
*/
float PlayerMonster::GetMeshHeight()
{
    float mesh_height = m_fMeshHeight;

    if (GetImageClass() == eCHAR_VALKYRIE)
    {
        mesh_height *= 0.9f;
    }

    return mesh_height;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::OnFootStep(BOOL b2D /* = FALSE */)
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

	switch (GetClass())
	{
	case eCHAR_BERSERKER:
	case eCHAR_DRAGON:
	case eCHAR_SHADOW:
		bMan = TRUE;
		break;

	default:
		bMan = FALSE;
		break;
	}

	DWORD dwVariation = rand() % MAX_FOOTSTEP_VARIATION;

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

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::ProcessStandAnimationDelay(DWORD dwTick)
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
int PlayerMonster::GetBulletCount()
{
	return m_iValkyrieBulletCount;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetBulletCount(int iCount)
{
	m_iValkyrieBulletCount = iCount;

}

//------------------------------------------------------------------------------
/**
*/
int PlayerMonster::DecreaseBulletCount()
{
	m_iValkyrieBulletCount--;

	return m_iValkyrieBulletCount;
}

//------------------------------------------------------------------------------
/**
*/
void  PlayerMonster::ValkyrieReload( BOOL bAnimation )
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

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetAttackSpeedRatio(int iSpeed)
{
	m_wAttSpeedRatio = iSpeed;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetMoveSpeedRatio(int iSpeed)
{
	m_wMoveSpeedRatio = iSpeed;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::LaunchEtherWeapon()
{
	if (GetWeaponKind() == eWEAPONTYPE_ETHERWEAPON) 
	{
		SetAttachmentAnimation(0,StrToWzID("N001"),FALSE);		
	}
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::OnShotStart()
{
	OpenCrossbow();
	LaunchEtherWeapon();
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::OnShotEnd()
{
	CloseCrossbow();
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::OnParalyzed()
{
	// Ű�����̵�������
	ResetMoveFlag();
	Character::OnParalyzed();
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::OnShotFire(int iShotCount)
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

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::CanBeAttackTarget()
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

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetPartVariation(int iPart,int iVariation)
{
    if (iPart < 0 || iPart >= PLAYER_VARIATION_MAX)
    {
        return;
    }

    //switch (iPart)
    //{
    //case PLAYER_VARIATION_HAIR:
    //    GetCharInfo()->m_byHair = iVariation;
    //    break;
    //case PLAYER_VARIATION_FACE:
    //    GetCharInfo()->m_byFace = iVariation;
    //    break;
    //}

    Character::SetPartVariation(iPart, iVariation);
}

//------------------------------------------------------------------------------
/**
*/
WzID	PlayerMonster::GetCurrentCamAnimation()
{
	return StrToWzID("0000");
}

//------------------------------------------------------------------------------
/**
*/
int PlayerMonster::GetShotCount()
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
void PlayerMonster::ResetJumpFlag()
{
	m_iJumpFlag = 0;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::AddJumpFlag(int JumpFlag)
{	
	BIT_ADD(m_iJumpFlag,JumpFlag);
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::RemoveJumpFlag(int JumpFlag)
{
	BIT_REMOVE(m_iJumpFlag,JumpFlag);
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::CheckJumpFlag(int JumpFlag)
{
	return BIT_CHECK(m_iJumpFlag,JumpFlag);
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetJumpFlag(int JumpFlag)
{
	m_iJumpFlag = JumpFlag;
}

//------------------------------------------------------------------------------
/**
*/
int PlayerMonster::GetJumpFlag()
{
	return m_iJumpFlag;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::SetOldJumpFlag(int iOldJumpFlag)
{
	m_iOldJumpFlag = iOldJumpFlag;
}

//------------------------------------------------------------------------------
/**
*/
int PlayerMonster::GetOldJumpFlag()
{
	return m_iOldJumpFlag;
}

//------------------------------------------------------------------------------
/**
*/
BOOL  PlayerMonster::IsFlyFinishWeapon()
{
	switch(GetWeaponKind())
	{
	case eWEAPONTYPE_TWOHANDSWORD:
	case eWEAPONTYPE_TWOHANDAXE:
	case eWEAPONTYPE_ONEHANDSWORD:
	case eWEAPONTYPE_SPEAR:
	case eWEAPONTYPE_ETHERWEAPON:
		return TRUE;
		break;
	}

	return FALSE;
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::IsShowRenderinfo()
{
	// �̸� ���κ���
	if( GAMEOPTION->GetShowPlayerName() ||									// �̸� ���κ���
		( g_HeroInput.GetPickedObjectKey() == GetObjectKey() ) )			// ���콺 ����
	{
		return TRUE;
	}	


	return FALSE;
}


//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::SetPosition( const HONGPOS& Pos)
{
	return SetPosition( Pos.wvPos, Pos.wTile);
}

//------------------------------------------------------------------------------
/**
*/
BOOL PlayerMonster::SetPosition( const WzVector& wvPos ,int iTileIndex)
{
	WzVector vOldPos = m_pPathExplorer->GetPos();

	AfterRefreshTileAtt();


	BOOL rt = Character::SetPosition( wvPos, iTileIndex );

	
	//���� ������ ���� ���� ���� ���� ����� ���װ� �ִ�.ó�� ���� ��ȿ���� ���� ��ǥ�� �´ٸ� ���� ���� �ȵ�
	if (m_dwCurTileAtt & (PTA_NO_WALK))
	{
		rt=FALSE;
	}

	if ( !GENERALPARAM->IsNet() )
	{
		rt=TRUE;
	}


	if(!rt && (m_iOldTile >= 0))
	{
		rt=g_pSunTerrain->PathExp_Reset( m_pPathExplorer, vOldPos);
		
	}

	AfterRefreshTileAtt();


	return rt;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::BeforeRefreshTileAtt()
{
	//Ű���� �̵� �̿ܿ� ������ ������ ����
	m_iOldTile=m_pPathExplorer->GetTile();
	//���� �Ӽ��� �ִٸ� ���� �Ӽ� ���� ������ ���� ���� 
	m_dwOldTileAtt=m_dwCurTileAtt;
}

//------------------------------------------------------------------------------
/**
*/
void PlayerMonster::AfterRefreshTileAtt()
{
	m_iCurTile=m_pPathExplorer->GetTile();
	//Ÿ���� �ٲ���ų� �ʱ�ȭ�� �ȵǾ������ 
	if((m_iOldTile!=m_iCurTile)||m_iCurTile<0)
		m_dwCurTileAtt=GetCurrentTileAtt(TRUE);
}

#ifdef _NA_008185_20150402_COSTUME_ITEM_FOR_NPC
void PlayerMonster::SetCostumeItemCode( SLOTCODE item_code )
{
    playermonster_costume_item_ = item_code;
}

SLOTCODE PlayerMonster::GetCostumeItemCode()
{
    return playermonster_costume_item_;
}
#endif//_NA_008185_20150402_COSTUME_ITEM_FOR_NPC
#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
void PlayerMonster::SetDecorateCostumeItemCode( SLOTCODE item_code )
{
	playermonster_decorate_costume_item_ = item_code;
}

SLOTCODE PlayerMonster::GetDecorateCostumeItemCode()
{
	return playermonster_decorate_costume_item_;
}
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME