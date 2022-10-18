//------------------------------------------------------------------------------
//  AppearanceManager.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "Object.h"
#include "character.h"
#include "player.h"
#include "item.h"
#include "appearancemanager.h"
#include "ResourceManager.h"
#include "AppearanceResourceCode.h"
#include "MapResMgr.h"
#include "ItemInfoParser.h"
#include "AttachmentOffsetInfoParser.h"
#include "ItemManager.h"
#include "GlobalFunc.h"
#include "SceneBase.h"
#include "LoadScene.h"

const int MONEY_RESOURCE_CODE = 34007 + ENGINE_RESOURCE_ID_OFFSET;
const int ITEMBOX_RESOURCE_CODE = 39999 + ENGINE_RESOURCE_ID_OFFSET;
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
const int DICEBOX_RESOURCE_CODE = 39939 + ENGINE_RESOURCE_ID_OFFSET;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

//-------------------------------------------------------------------------------------------
/**
*/

// ItemSubType 아이템강화 이펙트
// 기본 인첸트 (0~6)  까지 0    디바인 4
// 기본 인첸트 (7~9)  까지 1    디바인 5
// 기본 인첸트 (10~11)까지 2    디바인 6
// 기본 인첸트 (12~14)까지 3    디바인 7
// 기본 인첸트 (15~15)까지 8    디바인 9

static int GetItemSubType(int enchant,BOOL divine)
{
    int sub_type;

    if (enchant <= 6)
    {
        sub_type = 0;
    }
    else if (enchant <= 9)	
    {
        sub_type = 1;
    }
    else if (enchant <= 11)
    {
        sub_type = 2;
    }
    else if (enchant <= 14 )
    {
        sub_type = 3;
    }
    else
    {
        // 최상급은 디바인을 무시하고 단일 효과로 적용
        // 리소스 사용이 너무 많다나 뭐래나.
        return 8;
        //if( divine )
        //{
        //    return 9;
        //}
        //else 
        //{
        //    return 8;
        //}
    }

    if (divine)
    {
        sub_type += 4;
    }

    return sub_type;
}
//-------------------------------------------------------------------------------------------
/** 악세사리 본인덱스 얻기
*/
static int GetAccessoryBoneIndex(Character* character_ptr, DWORD item_code, bool attach_bone)
{
    int bone_index = -1;
    BASE_ITEMINFO* item_info_ptr = ItemInfoParser::Instance()->GetItemInfo(item_code);

    if (item_info_ptr)
    {
        bone_index = character_ptr->GetSpecialAccessoryBone(item_info_ptr->m_dwEquipBoneID);
    }
    else
    {
        if (attach_bone)
        {
            bone_index = character_ptr->GetSpecialAccessoryBone(StrToWzID("ACC1"));
        }
    }
    
    return bone_index;
}
//-------------------------------------------------------------------------------------------
/** 악세사리 붙이는 타입얻기
*/
static eATTACHTYPE GetAttachType(DWORD item_code)
{
   BASE_ITEMINFO* item_info_ptr = ItemInfoParser::Instance()->GetItemInfo(item_code);

   //본을 따라 다니는게 아니면 무조건 보간 한다.
   if(item_info_ptr &&
      (item_info_ptr->m_bEquipRot == false))
   {
        return ATTACHTYPE_INTERPOLATION;
   }
   
   return ATTACHTYPE_GENERAL;
}
//-------------------------------------------------------------------------------------------
/** 악세사리 기본정보들을 셋팅한다
*/
static void SetBaseAccessoryInfo(EQUIPAPPEARINFO& equip_info, AttachmentInfo& attach_info,
                                 Character* character_ptr,bool attach_bone)
{
    ResourceManager* resource_instance = ResourceManager::Instance();

    // 리소스코드
    RESOURCECODE resource_code = resource_instance->GetItemSkinResourceCode( equip_info.dwItemID, character_ptr->GetGender());
    
    attach_info.SetResourceID(resource_code);

    // 서브타입
    attach_info.SetSubType( GetItemSubType(equip_info.iEnchant, equip_info.bDivine) );

    // 디바인
    attach_info.SetSpecularVariation( equip_info.bDivine );

    // 본인덱스
    attach_info.SetBoneIndex( GetAccessoryBoneIndex(character_ptr,equip_info.dwItemID, attach_bone) );

    attach_info.SetItemCode(equip_info.dwItemID);
}

//-------------------------------------------------------------------------------------------
/** 애니메이션 코드 얻기
*/
static WzID GetAccessoryAnimation(int parts_index)
{
    //N001 : 작은원, 시계방향
    //N002 : 작은원, 시계반대방향
    //N003 : 큰원, 시계방향
    //N004 : 큰원, 시계반대방향

    WzID animation_id = StrToWzID("N004");

    switch(parts_index)
    {
    case BP_SPECIAL_ACCESSORY1:
        {
            animation_id = StrToWzID("N003");
        }
        break;
    case BP_SPECIAL_ACCESSORY2:
        {
            animation_id = StrToWzID("N002");
        }
        break;
    case BP_SPECIAL_ACCESSORY3:
        {
            animation_id = StrToWzID("N001");
        }
        break;
    }
    
    return animation_id;
}
//-------------------------------------------------------------------------------------------
/** 파츠가 존재하는지 체크(존재한다면제거)
*/
static bool IsExgistParts(CWzUnitDraw* part_ptr, DWORD skin_code)
{
    //갈아끼어야할 파츠랑 현재 파츠랑 다르다
    if (part_ptr &&
        part_ptr->GetWzUnit() && 
        part_ptr->GetWzUnit()->GetType() != skin_code)
    {
        ResourceManager::Instance()->FreeResourceUnitDraw(part_ptr);
        part_ptr = NULL;
    }

    if (part_ptr)
    {
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------
/**
*/
static void SetDefaultOption(Object* object, int parts_index)
{
    RENDER_PART_OPTION option;
    option.Initialize();
    object->SetRenderPartOption( (size_t)parts_index, option);
}
//------------------------------------------------------------------------------
/**
*/
AppearanceManager::AppearanceManager(void)
{
}

//------------------------------------------------------------------------------
/**
*/
AppearanceManager::~AppearanceManager(void)
{
}


//------------------------------------------------------------------------------
/**
*/
void AppearanceManager::SetAppearance(
	APPEARANCEINFO * pAppearInfo, 
	Object * pObject,
	BOOL bRefreshBone,
	BOOL bMakeBoneOnly)
{

    switch(pAppearInfo->ObjectType)
    {
	case APPEARANCEINFO::PLAYERTRANSFORM:
		SetPlayerTransFormAppearance(pAppearInfo, pObject, bRefreshBone);
		break;

    case APPEARANCEINFO::PLAYER:
        SetPlayerAppearance(pAppearInfo, pObject, bRefreshBone, bMakeBoneOnly);
        break;

    case APPEARANCEINFO::MONSTER:
        SetMonsterAppearance(pAppearInfo, pObject, bRefreshBone);
        break;
    
    case APPEARANCEINFO::PET:
        SetPetAppearance(pAppearInfo, pObject);
        break;

    case APPEARANCEINFO::ITEM:
        SetItemAppearance(pAppearInfo, pObject );
        break;
    
    //case APPEARANCEINFO::SKYBOX:
    //    SetSkyBoxAppearance(pAppearInfo, pObject, bRefreshBone);
    //    break;

    case APPEARANCEINFO::MAPOBJECT:
        SetMapObjectAppearance(pAppearInfo, pObject, bRefreshBone);
        break;

    case APPEARANCEINFO::CAMERA:
        SetCameraObjectAppearance(pAppearInfo, pObject, bRefreshBone);
        break;

    case APPEARANCEINFO::POLOMORPH:
        SetPolymorphApperance( pObject, bRefreshBone );
        break;

    default:
        assert(0);
        break;
    }
}



//------------------------------------------------------------------------------
/**
*/
void AppearanceManager::SetPlayerAppearance( APPEARANCEINFO* pAppearInfo, Object *pObject, BOOL bRefreshBone, BOOL bMakeBoneOnly)
{
	int PartsCount = pObject->GetPartCount();	
	CWzUnitDraw **pParts = pObject->GetParts();
	// 0번은 메인 본이다.
	RESOURCECODE skinCode;

	if (bRefreshBone) 
	{
        eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(pAppearInfo->Job);
        GenderType::Value character_gender = static_cast<Character*>(pObject)->GetGender();
        skinCode = ResourceManager::Instance()->GetCharacterBoneResourceCode(class_type, character_gender);
		CWzUnitDraw* pDraw = ResourceManager::Instance()->AllocResourceUnitDraw(skinCode, pAppearInfo->texture_rate, 
                                                               false);
		assert(pDraw);
		pParts[0] = pDraw;
	}

	if (bMakeBoneOnly)
	{
		return;
    }

    _SetPlayerBodyAppearance(pAppearInfo, pObject);
    _SetPlayerWeaponAppearance(pAppearInfo, pObject);
    _SetPlayerSpecialAccessoryAppearance(pAppearInfo, pObject);
}

//------------------------------------------------------------------------------
/**
*/
void AppearanceManager::SetPlayerTransFormAppearance( APPEARANCEINFO* pAppearInfo, Object *pObject, BOOL bRefreshBone)
{
	CWzUnitDraw **pParts = pObject->GetParts();
	RESOURCECODE code = pAppearInfo->resourceCode;

	code += ENGINE_RESOURCE_ID_OFFSET;


	//	변신 데이터 로딩 & 생성
	for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
	{
        if( pParts[i] != NULL )
        {
            ResourceManager::Instance()->FreeResourceUnitDraw(pParts[i]);
            pParts[i] = NULL;
        }


		assert(code && "변신 이미지코드가 0입니다.");
		pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code, pAppearInfo->texture_rate);

		assert(pParts[i] &&"변신 이미지 파일이 없습니다");
	}
}

//------------------------------------------------------------------------------
/** 펫은 캐싱한다
*/
void AppearanceManager::SetPetAppearance(APPEARANCEINFO* appear_ptr,Object* object_ptr)
{
    ResourceManager* resource_instance = ResourceManager::Instance();
 

	CWzUnitDraw** ppRenderPart = object_ptr->GetParts();

    //	몬스터 데이터 로딩 & 생성
    for ( size_t i = 0; i < object_ptr->GetPartCount(); ++i)
    {
        RESOURCECODE code = resource_instance->GetNPCSkinResourceCode(appear_ptr->pMonsterInfo->m_MonsterCode);

        if( ppRenderPart[i] != NULL )
        {
            ResourceManager::Instance()->FreeResourceUnitDraw(ppRenderPart[i]);
            ppRenderPart[i] = NULL;
        }

        ppRenderPart[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code, -1.0f, false);
    }
}

//------------------------------------------------------------------------------
/**
*/
void AppearanceManager::SetMonsterAppearance( APPEARANCEINFO* pAppearInfo, Object *pObject, BOOL bRefreshBone )
{
	CWzUnitDraw **pParts = pObject->GetParts();

	//	몬스터 데이터 로딩 & 생성
	for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
	{
		RESOURCECODE code = ResourceManager::Instance()->GetNPCSkinResourceCode( pAppearInfo->pMonsterInfo->m_MonsterCode );

        if( pParts[i] != NULL )
        {
            ResourceManager::Instance()->FreeResourceUnitDraw(pParts[i]);
            pParts[i] = NULL;
        }

		assert(code && "몬스터 이미지코드가 0입니다.");
		pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);

		assert(pParts[i] && "몬스터 이미지 파일이 없습니다");
	}
}

//------------------------------------------------------------------------------
/**
*/
void AppearanceManager::SetItemAppearance( APPEARANCEINFO* pAppearInfo, Object *pObject)
{
	// E3 나가고 리소스가 어케될지 정해서 풀자. 
	CWzUnitDraw **pParts = pObject->GetParts();
	RESOURCECODE code;

	//	아이템 생성 
	for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
	{
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
        if (pAppearInfo->is_dice_item != 0)
        {
            code = DICEBOX_RESOURCE_CODE;
        }
        else
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
		if (pAppearInfo->dwMoney == 0 )
		{
			// 아이템
			BASE_ITEMINFO * pItemInfo = 
				ItemInfoParser::Instance()->GetItemInfo( pAppearInfo->ItemCode );
			if ( pItemInfo )
			{
				code = pItemInfo->m_wDICode +  ENGINE_RESOURCE_ID_OFFSET;
			}
			else
			{
				code = ITEMBOX_RESOURCE_CODE; 
			}
		}
		else
		{
			// 돈.
			code = MONEY_RESOURCE_CODE;
		}

        if( pParts[i] != NULL )
        {
            ResourceManager::Instance()->FreeResourceUnitDraw(pParts[i]);
            pParts[i] = NULL;
        }
	
		pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
	}
}
////------------------------------------------------------------------------------
///**
//	TODO: ..이놈은 없어질것이다. 
//*/
//void AppearanceManager::SetSkyBoxAppearance(
//				APPEARANCEINFO* pAppearInfo,
//				Object *pObject,
//				BOOL bRefreshBone )
//{
//	CWzUnitDraw **pParts = pObject->GetParts();
//	//	몬스터 데이터 로딩 & 생성
//	for( size_t i = 0 ; i < pObject->GetPartCount(); ++i )
//	{
//		RESOURCECODE code = ResourceManager::Instance()->GetTempResourceCode(pAppearInfo->SkyBoxNum );
//		pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
//	}
//}
//
//
////------------------------------------------------------------------------------
///**  사용 되고 있지 않은 함수 사라질 예정.
//*/
//void AppearanceManager::SetItemMoneyAppearance(
//							APPEARANCEINFO * pAppearInfo,
//							Object * pObject,
//							BOOL bRefreshBone )
//{
//	CWzUnitDraw** pParts = pObject->GetParts();
//	RESOURCECODE code = 40000;
//
//	//	아이템 생성 
//	for( size_t i = 0 ; i < pObject->GetPartCount(); ++i )
//	{
//		code = 40000;
//		pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
//		pParts[i]->SetScale(1.5f, 1.5f, 1.5f);
//	}
//}

//-------------------------------------------------------------------------------------------
/**
*/
void AppearanceManager::SetMapObjectAppearance( APPEARANCEINFO* pAppearInfo, Object *pObject, BOOL bRefreshBone/* = FALSE*/)
{
	CWzUnitDraw **ppRenderPart = pObject->GetParts();

	//	멥오브젝트 생성 
	for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
	{		
		RESOURCECODE code = pAppearInfo->MapObjectID;		
		ppRenderPart[i] = MAPRESMGR()->AddDraw(code);
		assert(ppRenderPart[i]);
		
		if(ppRenderPart[i])
		{
			ppRenderPart[i]->EnableUpdateLight(FALSE);
			ppRenderPart[i]->SetScale( pAppearInfo->wzScale );
			ppRenderPart[i]->SetRotation(pAppearInfo->wzRot);
		}
	}

	pObject->SetColor(pAppearInfo->wzColor);
}

void AppearanceManager::SetPolymorphApperance( Object *pObject, BOOL bRefreshBone /*= FALSE*/ )
{
    static DWORD s_PolymorphCode = 15914;

    CWzUnitDraw **pParts = pObject->GetParts();

    //	몬스터 데이터 로딩 & 생성
    for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
    {
        pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(s_PolymorphCode + ENGINE_RESOURCE_ID_OFFSET);
    }
}

//-------------------------------------------------------------------------------------------
/**
*/
void AppearanceManager::SetCameraObjectAppearance( APPEARANCEINFO* pAppearInfo, Object *pObject, BOOL bRefreshBone)
{	
	CWzUnitDraw **ppRenderPart = pObject->GetParts();
	//	몬스터 데이터 로딩 & 생성
	for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
	{
		RESOURCECODE code = ResourceCode::GetCinematicCameraInCharacterScene() + ENGINE_RESOURCE_ID_OFFSET ;
		ppRenderPart[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
	}
}

void AppearanceManager::_SetCustumBodyPart( Object* pObject, DWORD CustomCode, float texture_rate )
{
    RESOURCECODE skin_code = CustomCode;

    //파츠교체한다(교체할필요없으면 continue)
    CWzUnitDraw** player_parts = pObject->GetParts();
    if (IsExgistParts(player_parts[BP_ARMOR], skin_code) == FALSE )
    {
        if( CWzUnitDraw* part_unit = ResourceManager::Instance()->AllocResourceUnitDraw(skin_code, texture_rate, false) )
        {
            player_parts[BP_ARMOR] = part_unit;
            SetDefaultOption( pObject, BP_ARMOR );
        }
    }
}

void AppearanceManager::_SetDefaultPart( Object* pObject, eCHAR_TYPE Job, BodyPart parts_index, float texture_rate, GenderType::Value character_gender )
{

    RESOURCECODE skin_code = ResourceManager::Instance()->GetDefaultSkinCode(Job, parts_index, character_gender);

    CWzUnitDraw** player_parts = pObject->GetParts();
    if (skin_code != 0)
    {
        // 이미 같은 스킨
        if (IsExgistParts(player_parts[parts_index], skin_code) == FALSE )
        {
            if( CWzUnitDraw* part_unit = ResourceManager::Instance()->AllocResourceUnitDraw(skin_code,  texture_rate, false) )
            {
                player_parts[parts_index] = part_unit;
                SetDefaultOption(pObject, parts_index);
            }
        }
    }
    else if (player_parts[parts_index])
    {
        ResourceManager::Instance()->FreeResourceUnitDraw(player_parts[parts_index]);
        player_parts[parts_index] = NULL;					
    }

}

void AppearanceManager::_SetEquipItemPart( Object* pObject, const EQUIPAPPEARINFO& equip_info, BodyPart parts_index, float texture_rate, GenderType::Value character_gender )
{
    Player* player_ptr = static_cast<Player *>(pObject);

    

    RESOURCECODE skin_code = 0;
    int sub_type = 0;
    BOOL use_specular_effect = FALSE;

    // 스킨의 코드를 먼저구한다
    if( parts_index == BP_HAIR || parts_index == BP_FACE )
    {
        // Item ID와 Enchant를 머리 모양과 Set으로 대신하여 쓴다.
        skin_code = ResourceManager::Instance()->GetCharacterPartSkinResourceCode(
            player_ptr->GetClass(), 
            (BodyPart)parts_index, 
            equip_info.dwItemID, 
            equip_info.iEnchant, 
            character_gender);
    }
    else
    {
        //일반적인 파츠들
        skin_code = ResourceManager::Instance()->GetItemSkinResourceCode(equip_info.dwItemID,
            player_ptr->GetClass(),
            parts_index,
            character_gender);

        sub_type = GetItemSubType(equip_info.iEnchant, equip_info.bDivine);

        // 디바인은 스펙큘러강도값을 지속적의로 바꿔줘서 다른느낌을 준다
        use_specular_effect = equip_info.bDivine;
    }

    CWzUnitDraw** player_parts = pObject->GetParts();

    // 리소스 매니저를 통해 리소스코드값이 유효할때만 파츠셋팅하는데
    // 기존에 있던 코드라면 셋팅을 안하고 없던코드라면 기존리소스를 없애주고 셋팅한다
    if (skin_code != 0) 
    {
        if (IsExgistParts(player_parts[parts_index], skin_code) == FALSE )
        {
            // 파츠교체한다
            bool is_monster = false;
            if (pObject->GetObjectType() == MONSTER_OBJECT)
            {
                is_monster = true;
            }
            if( CWzUnitDraw* new_part_unit = ResourceManager::Instance()->AllocResourceUnitDraw(skin_code, texture_rate, false, is_monster) )
            {
                player_parts[parts_index] = new_part_unit;
            }
        }

        CWzUnitDraw* part_unit = player_parts[parts_index];
        if( part_unit )
        {
            if( part_unit->GetWzUnit() != NULL )
            {
                // 서브타입이 다르고 유효범위를 넘지않았을때 무기에 옵션에따라 다른 서브타입을 적용해준다.
                if ((part_unit->GetSubType() != sub_type) && 
                    (sub_type < part_unit->GetWzUnit()->GetNumSubType()))
                {
                    part_unit->SetSubType(sub_type);
                }		
            }
        }


        RENDER_PART_OPTION option;
        option.bSpecularVariation = use_specular_effect;
        option.bCustomDiffuse = FALSE;
        pObject->SetRenderPartOption(parts_index, option);
    }
    else if (player_parts[parts_index])
    {
        ResourceManager::Instance()->FreeResourceUnitDraw(player_parts[parts_index]);
        player_parts[parts_index] = NULL;					
    }
}

//-------------------------------------------------------------------------------------------
/** 캐릭터 파츠셋팅(kBeginBodyParts ~ kEndBodyParts)돌면서 기존장착되지 않은상태에서만 장착
    하고 슬롯이 빈상황(dwItemID==0)에서는 기본파츠라도 장착해준다 또한 현재 클래스에 맞지않는
    장비 아이템일경우 디폴트로 장착해준다
*/
void AppearanceManager::_SetPlayerBodyAppearance(APPEARANCEINFO * pAppearInfo, Object * pObject)
{
    ResourceManager* resource_instance = ResourceManager::Instance();

    CWzUnitDraw** player_parts = pObject->GetParts();
    GenderType::Value character_gender = static_cast<Character*>(pObject)->GetGender();

    // 사용해야할 resourceID를 체크한후 바꿔야하면 기존것을 해제하고 바꿉니다.
    for (int parts_index = kBeginBodyParts; parts_index < kEndBodyParts; ++parts_index)
    {
        //safe_code: 배열인덱스체크
        if (parts_index >= (int)pObject->GetPartCount())
        {
            continue;
        }

        EQUIPAPPEARINFO& equip_info =  pAppearInfo->Equipment[parts_index];
        if (equip_info.dwItemID != 0)
        {
            // 장착가능한지 먼저 검사
            if( BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(equip_info.dwItemID) )
            {
                if( item_info->IsArmor() && ItemManager::Instance()->CanEquipClass((Player*)pObject, item_info) == FALSE)
                {
                    _SetDefaultPart( pObject, (eCHAR_TYPE)pAppearInfo->Job, (BodyPart)parts_index, pAppearInfo->texture_rate, character_gender );
                    continue;
                }
				else if( GlobalFunc::IsSSQScene() )
				{
					_SetDefaultPart( pObject, (eCHAR_TYPE)pAppearInfo->Job, (BodyPart)parts_index, pAppearInfo->texture_rate, character_gender );
					continue;
				}
            }

            _SetEquipItemPart( pObject, equip_info, (BodyPart)parts_index, pAppearInfo->texture_rate, character_gender );
        }
        else
        {
            _SetDefaultPart( pObject, (eCHAR_TYPE)pAppearInfo->Job, (BodyPart)parts_index, pAppearInfo->texture_rate, character_gender );
        }
    }    
}
//-------------------------------------------------------------------------------------------
/** 무기 장착은 크게 한손.양손의 경우로 나뉘어진다 한손일때는 나머지손을 DeleteAttach해주고
    양손일때는 SetAttach(0) , SetAttach(1)식의로 무기를장착해준다
        
    (오른손:0),index(왼손:1) - (2,3,4)악세사리

*/
void AppearanceManager::_SetPlayerWeaponAppearance(APPEARANCEINFO * pAppearInfo, Object * pObject)
{
    Character* character_ptr = static_cast<Character*>(pObject);

    EQUIPAPPEARINFO& weapon_info = pAppearInfo->Equipment[BP_WEAPON];

    //WzMatrix offset_matrix;
    //ZeroMemory(&offset_matrix ,sizeof(offset_matrix));

    RESOURCECODE resource_code = 0;
    GenderType::Value character_gender = character_ptr->GetGender();
    
    //아이템이 존재할때
    if (weapon_info.dwItemID != 0)
    {
        resource_code = ResourceManager::Instance()->GetItemSkinResourceCode(weapon_info.dwItemID, (eCHAR_TYPE)pAppearInfo->Job, 0, character_gender);
    }

    AttachmentInfo attach_info;

    // 한쪽장착무기인지(버서커,앨리멘탈리스트는 무조건한손임)
    bool one_hand_weapon = true;

    eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(pAppearInfo->Job);
    switch(class_type)
    {
    case eCHAR_DRAGON:
        {   
            //쌍칼일 경우에는 양손에단다
            if (character_ptr->GetWeaponKind() == eWEAPONTYPE_ONEHANDSWORD)
            {
                one_hand_weapon = false;
            }
        }
        break;
    case eCHAR_VALKYRIE:
        {
            if (character_ptr->GetWeaponKind() == eWEAPONTYPE_ONEHANDCROSSBOW)
            {
                one_hand_weapon = false;
            }
        }
        break;
    case eCHAR_SHADOW:
        {
            if (character_ptr->GetWeaponKind() == eWEAPONTYPE_DAGGER)
            {
                one_hand_weapon = false;
            }
        }
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        {
            if (character_ptr->GetWeaponKind() == eWEAPONTYPE_ARCBLADE)
            {
                one_hand_weapon = false;
            }
        }
        break;
    }

    attach_info.SetResourceID( resource_code );
    attach_info.SetBoneIndex( character_ptr->GetWeaponBone(0) );
    attach_info.SetSubType( GetItemSubType(weapon_info.iEnchant, weapon_info.bDivine) );
    attach_info.SetSpecularVariation( weapon_info.bDivine );
    float weapon_scale = ResourceManager::Instance()->GetWeaponScale(class_type, character_gender);
    attach_info.SetScale(weapon_scale);
    pObject->SetAttachment(0, attach_info, pAppearInfo->texture_rate);

    if (one_hand_weapon)
    {
        //나머지손 무기는 제거해줌
        pObject->DeleteAttachment(1);
    }
    else
    {
        //양손장착(나머지손 장착해줌(1))
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if (character_ptr->GetWeaponKind() == eWEAPONTYPE_ARCBLADE)
        {
            if (weapon_info.dwItemID != 0)
            {
                BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(weapon_info.dwItemID);
                if (item_info != NULL)
                {
                    resource_code = item_info->m_wWICode + ENGINE_RESOURCE_ID_OFFSET;
                    attach_info.SetResourceID( resource_code );
                }
            }
        }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        attach_info.SetBoneIndex( character_ptr->GetWeaponBone(1) );
        BOOL attach_success = pObject->SetAttachment( 1, attach_info, pAppearInfo->texture_rate);

        //예외처리:발키리 최초 활닫아줌
        if (attach_success &&
           (pAppearInfo->Job == eCHAR_VALKYRIE))
        {
            character_ptr->CloseCrossbow();
        }
    }
}
//-------------------------------------------------------------------------------------------
/** 총 5개의 AttachMent가있는데 이중 악세사리 인덱스는(2,3,4)이고 웨폰은 (0,1)이다
*/
void AppearanceManager::_SetPlayerSpecialAccessoryAppearance(APPEARANCEINFO* pAppearInfo, 
                                                             Object* pObject)
{	
    //타락한 사원에서는 악세사리 셋티없음
	if (GlobalFunc::IsSSQScene())
    {
		return;
    }

    Character* character_ptr = static_cast<Character*>(pObject);

    WzMatrix mat;
    ZeroMemory( &mat ,sizeof(mat));

    ApplySpecialAccessoryAppearance(pAppearInfo, character_ptr);
}
//-------------------------------------------------------------------------------------------
/**
	아래의 하드코딩된 아이템코드의 경우 노란.파란.빨간 간판등의로 개인상점을 켤때
    글씨색이 바뀌는데..현재 하드코딩되어있는상태임
//*/
bool 
AppearanceManager::InvalidAccessory(DWORD item_code)
{
    switch(item_code)
    {
    case 0:
    case 3024:
    case 3025:
    case 3026:
    case 3027:
    case 3028:
    case 49518://!< 이하 중국용추가
    case 49519:
    case 49520:
    case 49521:
    case 49522:
        return false;
    default:
        break;
    }

    return true;
}

//------------------------------------------------------------------------------ 
void AppearanceManager::ApplySpecialAccessoryAppearance(APPEARANCEINFO* appear_info, Character* character_ptr)
{
    
    typedef int BoneIndex;
    typedef int ItemLevel;
    typedef std::pair<BoneIndex, ItemLevel> AttachCheckInfo;
    typedef STLX_MAP<int, AttachCheckInfo> ApplyInfoMap;

    const int kAccessoryMax = 
        (kAppearanceAttachIndex_Accessory3 - kAppearanceAttachIndex_Accessory1) + 1;

    ApplyInfoMap apply_info_map;
    for (int accessory_index = 0; accessory_index < kAccessoryMax; ++accessory_index)
    {
        BodyPart body_part = static_cast<BodyPart>(accessory_index + BP_SPECIAL_ACCESSORY1);
        EQUIPAPPEARINFO& equip_appear_info = appear_info->Equipment[body_part];

        if (InvalidAccessory(equip_appear_info.dwItemID) == true)
        {
            const BASE_ITEMINFO* item_info = 
                ItemInfoParser::Instance()->GetItemInfo(equip_appear_info.dwItemID);
            if (item_info != NULL)
            {
                // 본/레벨을 따져서 실제로 적용할지 체크
                int bone_index = GetAccessoryBoneIndex(character_ptr, 
                    equip_appear_info.dwItemID, 
                    true);

                bool is_apply = true;
                if (bone_index != -1)
                {
                    ApplyInfoMap::iterator begin_itr = apply_info_map.begin();
                    ApplyInfoMap::iterator end_itr = apply_info_map.end();
                    for ( ; begin_itr != end_itr; ++begin_itr)
                    {
                        AttachCheckInfo& attach_check_info = begin_itr->second;
                        if (attach_check_info.first == bone_index)
                        {
                            if (attach_check_info.second >= item_info->m_LV)
                            {
                                is_apply = false;
                            }
                            else
                            {
                                apply_info_map.erase(begin_itr);
                            }

                            break;
                        }
                    }
                }

                if (is_apply == true)
                {
                    AttachCheckInfo attach_check_info;
                    attach_check_info.first = bone_index;
                    attach_check_info.second = item_info->m_LV;
                    apply_info_map.insert(ApplyInfoMap::value_type(accessory_index, attach_check_info));
                }
            }
        }

        character_ptr->DeleteAttachment(accessory_index + kAppearanceAttachIndex_Accessory1);
    }

    ApplyInfoMap::iterator begin_itr = apply_info_map.begin();
    ApplyInfoMap::iterator end_itr = apply_info_map.end();
    for ( ; begin_itr != end_itr; ++begin_itr)
    {
        BodyPart body_part = static_cast<BodyPart>(begin_itr->first + BP_SPECIAL_ACCESSORY1);
        int appearance_attach_index = (begin_itr->first) + kAppearanceAttachIndex_Accessory1;
        
        WzID animation_id = 0;
        AttachmentInfo attach_info;
        EQUIPAPPEARINFO& equip_info = appear_info->Equipment[body_part];

        SetBaseAccessoryInfo(equip_info, attach_info, character_ptr, true);

        if (appear_info->Job == eCHAR_VALKYRIE)
        {
            animation_id = StrToWzID("N001");
            attach_info.SetAttachType( GetAttachType(equip_info.dwItemID) );
        }

        if (attach_info.GetBoneIndex() == -1)
        {
            //애니메이션 아이디 얻기
            animation_id = GetAccessoryAnimation(body_part);
            WzVector offset_vector;
            offset_vector.x = 0.0f;
            offset_vector.y = 0.0f;
            offset_vector.z = 1.0f;
            attach_info.SetOffset(offset_vector);
        }
        else
        {
            const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(equip_info.dwItemID);
            if ((item_info != NULL) && (item_info->m_wType == eITEMTYPE_WINGS))
            {
                WzVector pos;
                pos.x = 0; pos.y = 0; pos.z = 0; 
                if (appear_info->Equipment[BP_ARMOR].dwItemID == 0)
                {
                    GetAccessoryPosition(pos, equip_info.dwItemID);
                }
                attach_info.SetOffset(pos);

                if (appearance_attach_index < (int)character_ptr->GetAttachmentCount())
                {
                    ((AttachmentInfo&)(character_ptr->GetAttachmentInfo(appearance_attach_index))).SetOffset(pos);
                }
            }
            else
            {
                WzVector pos;
                if (GetAccessoryPosition(pos, appear_info->Equipment[BP_GLOVE].dwItemID) == true)
                {
                    attach_info.SetOffset(pos);
                }
            }

            WzMatrix rot;
            if (GetAccessoryRotation(rot, equip_info.dwItemID) == true)
            {
                attach_info.SetRotateMatrix(rot);
            }
        }

        // 캐릭터에 장착해줌
        BOOL attach_success = 
            character_ptr->SetAttachment(appearance_attach_index, attach_info, appear_info->texture_rate);
        if (attach_success != FALSE)
        {
            character_ptr->SetAttachmentAnimation(appearance_attach_index, animation_id, TRUE);
        }
    }
}

//------------------------------------------------------------------------------ 
bool AppearanceManager::GetAccessoryPosition(OUT WzMatrix& mx_pos, DWORD item_code)
{
    BASE_AttachmentOffsetInfo* attachment_info = AttachmentOffsetInfoParser::Instance()->GetAttachmentOffsetInfo(item_code);
    if (attachment_info == NULL)
    {
        return false;
    }
    if ((attachment_info->pos_x_ == 0.0f) && 
        (attachment_info->pos_y_ == 0.0f) && 
        (attachment_info->pos_z_ == 0.0f))
    {
        return false;
    }

    MatrixTranslation(&mx_pos, attachment_info->pos_x_, attachment_info->pos_y_, attachment_info->pos_z_);

    return true;
}
//------------------------------------------------------------------------------ 
bool AppearanceManager::GetAccessoryPosition(OUT WzVector& pos, DWORD item_code)
{
    BASE_AttachmentOffsetInfo* attachment_info = AttachmentOffsetInfoParser::Instance()->GetAttachmentOffsetInfo(item_code);
    if (attachment_info == NULL)
    {
        return false;
    }
    if ((attachment_info->pos_x_ == 0.0f) && 
        (attachment_info->pos_y_ == 0.0f) && 
        (attachment_info->pos_z_ == 0.0f))
    {
        return false;
    }

    pos.x = attachment_info->pos_x_;
    pos.y = attachment_info->pos_y_;
    pos.z = attachment_info->pos_z_;

    return true;
}
//------------------------------------------------------------------------------ 
bool AppearanceManager::GetAccessoryRotation(OUT WzMatrix& mx_rot, DWORD item_code)
{
    BASE_AttachmentOffsetInfo* attachment_info = AttachmentOffsetInfoParser::Instance()->GetAttachmentOffsetInfo(item_code);
    if (attachment_info == NULL)
    {
        return false;
    }
    if ((attachment_info->rot_x_ == 0.0f) && 
        (attachment_info->rot_y_ == 0.0f) && 
        (attachment_info->rot_z_ == 0.0f))
    {
        return false;
    }

    WzVector v;
    v.x = n_deg2rad(attachment_info->rot_x_);
    v.y = n_deg2rad(attachment_info->rot_y_);
    v.z = n_deg2rad(attachment_info->rot_z_);

    MatrixRotationZYX(&mx_rot, &v);

    return true;
}