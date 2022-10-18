#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
#include "StatuePlayer.h"
#include "GlobalFunc.h"
#include "ObjectManager.h"
#include "Const.h"
#include "Monster.h"
#include "EquipmentContainer.h"
#include <SCItemSlot.h>
#include "uiSUNRankingMan/uiSUNRankingMan.h"
#include "uiSUNRankingMan/uiSUNRankingStatueInfo.h"
#include "Hero.h"

StatuePlayer::StatuePlayer(void) : 
proto_equip_container_(NULL),
statue_equip_container_(NULL)
{
	SetObjectType(STATUE_PLAYER_OBJECT);	
}

StatuePlayer::~StatuePlayer(void)
{
}

BOOL StatuePlayer::Create( DWORD object_key, DWORD id /*= 0 */ )
{
	SetObjectKey( object_key );

	BOOL result = Character::Create(object_key, id);
	cCharacterData::Initialize();

	InitContainer();

	// 상태 초기화 ( STATUE_STAND 상태 )
	InitState(STATE::STATUE_STAND, g_CurTime);

	return TRUE;
}

BOOL StatuePlayer::Process( DWORD tick )
{
	if ( GetBasePart() )
	{
		Character::Process(tick);
	}

	// 거리에 따라 열려있는 StatueInfoDialog를 닫아 준다.
	uiSUNRankingMan* ui_sun_ranking_manager = GET_DIALOG_MANAGER(uiSUNRankingMan, UIMAN_SUNRANKING);
	if ( ui_sun_ranking_manager )
	{
		uiSUNRankingStatueInfo* statue_info = ui_sun_ranking_manager->get_sun_ranking_statue_info_();
		if ( statue_info && 
			(GetObjectKey() == statue_info->get_statue_player_key_() ) && 
			statue_info->IsVisible() )
		{
			if ( NULL != g_pHero )
			{
				WzVector vDiff = GetPosition() - g_pHero->GetPosition();
				float distance = VectorLength( &vDiff );
				if ( distance > 18.0f )
				{
					statue_info->ShowInterface(FALSE);
				}
			}
		}
	}

	return TRUE;
}

void StatuePlayer::Destroy( void )
{
	Player::Destroy();
}

void StatuePlayer::RenderInfo()
{
	if (GetBasePart() == NULL)
	{
		return;
	}

	const WzColor TextColor = WzColor_RGBA(4,183,150,255);
	const float fShowInfoDistance = 80.0f * 80.0f;

	St_Camera * pCameraInfo = g_pSunRenderer->GetCameraInfo();
	WzVector vDistance = GetPosition() - pCameraInfo->m_wvFrom;

	if (VectorLength2(&vDistance) >= fShowInfoDistance)
	{
		return;		
	}

	Object::RenderInfo();
}

void StatuePlayer::AnimateUnit()
{
	if (GetBasePart() == NULL)
	{
		return;
	}

//	Character::AnimateUnit();

	Object::AnimateUnit();
}

BOOL StatuePlayer::Render()
{
	if ( GetShow() == FALSE )
		return FALSE;

	BOOL ret = Character::Render();

	return ret;
}

bool StatuePlayer::RefreshApperBronzePlayer()
{
	if (GetBasePart())
	{
		return false;
	}

	APPEARANCEINFO statue_player_appearance;
	ZeroMemory(&statue_player_appearance, sizeof(statue_player_appearance));
	statue_player_appearance.ObjectType = APPEARANCEINFO::PLAYER;
	statue_player_appearance.Job = GetClass();
	InitRender( BP_COUNT, BP_BONE, &statue_player_appearance, AppearanceManager::kAppearanceAttachIndex_Count, FALSE );
	RefreshAppearRender();

	return true;
}

void StatuePlayer::InitContainer()
{
	if (GetEquipmentContainer() == NULL)
	{
		EquipmentContainer* equipment_container = new EquipmentContainer();
		equipment_container->SetOwnerKey(GetObjectKey());
		SetEquipmentContainer(equipment_container);

		proto_equip_container_ = equipment_container;
		statue_equip_container_ = new EquipmentContainer();
		statue_equip_container_->SetOwnerKey(GetObjectKey());
	}
}

void StatuePlayer::ReleaseContainer()
{
	if (proto_equip_container_ != NULL)
	{
		proto_equip_container_->Release();	
		delete proto_equip_container_;
		proto_equip_container_ = NULL;
		SetEquipmentContainer(NULL);
	}

	if ( statue_equip_container_ != NULL )
	{
		statue_equip_container_->Release();
		delete statue_equip_container_;
		statue_equip_container_ = NULL;
		SetEquipmentContainer(NULL);
	}
}

void StatuePlayer::SetClass(eCHAR_TYPE character_class)
{
	Character::SetClass(character_class);
}

void StatuePlayer::set_equip_container_( EquipmentContainer* equip_container )
{
	if ( equip_container == NULL )
	{
		return;
	}

	SetEquipmentContainer(equip_container);
}

char StatuePlayer::GetWeaponAnimationCode()
{
	const char kInvalid_Code = '0';
	if(IsTransForm())
	{
		return kInvalid_Code;
	}

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

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
