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

// ItemSubType �����۰�ȭ ����Ʈ
// �⺻ ��þƮ (0~6)  ���� 0    ����� 4
// �⺻ ��þƮ (7~9)  ���� 1    ����� 5
// �⺻ ��þƮ (10~11)���� 2    ����� 6
// �⺻ ��þƮ (12~14)���� 3    ����� 7
// �⺻ ��þƮ (15~15)���� 8    ����� 9

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
        // �ֻ���� ������� �����ϰ� ���� ȿ���� ����
        // ���ҽ� ����� �ʹ� ���ٳ� ������.
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
/** �Ǽ��縮 ���ε��� ���
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
/** �Ǽ��縮 ���̴� Ÿ�Ծ��
*/
static eATTACHTYPE GetAttachType(DWORD item_code)
{
   BASE_ITEMINFO* item_info_ptr = ItemInfoParser::Instance()->GetItemInfo(item_code);

   //���� ���� �ٴϴ°� �ƴϸ� ������ ���� �Ѵ�.
   if(item_info_ptr &&
      (item_info_ptr->m_bEquipRot == false))
   {
        return ATTACHTYPE_INTERPOLATION;
   }
   
   return ATTACHTYPE_GENERAL;
}
//-------------------------------------------------------------------------------------------
/** �Ǽ��縮 �⺻�������� �����Ѵ�
*/
static void SetBaseAccessoryInfo(EQUIPAPPEARINFO& equip_info, AttachmentInfo& attach_info,
                                 Character* character_ptr,bool attach_bone)
{
    ResourceManager* resource_instance = ResourceManager::Instance();

    // ���ҽ��ڵ�
    RESOURCECODE resource_code = resource_instance->GetItemSkinResourceCode( equip_info.dwItemID, character_ptr->GetGender());
    
    attach_info.SetResourceID(resource_code);

    // ����Ÿ��
    attach_info.SetSubType( GetItemSubType(equip_info.iEnchant, equip_info.bDivine) );

    // �����
    attach_info.SetSpecularVariation( equip_info.bDivine );

    // ���ε���
    attach_info.SetBoneIndex( GetAccessoryBoneIndex(character_ptr,equip_info.dwItemID, attach_bone) );

    attach_info.SetItemCode(equip_info.dwItemID);
}

//-------------------------------------------------------------------------------------------
/** �ִϸ��̼� �ڵ� ���
*/
static WzID GetAccessoryAnimation(int parts_index)
{
    //N001 : ������, �ð����
    //N002 : ������, �ð�ݴ����
    //N003 : ū��, �ð����
    //N004 : ū��, �ð�ݴ����

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
/** ������ �����ϴ��� üũ(�����Ѵٸ�����)
*/
static bool IsExgistParts(CWzUnitDraw* part_ptr, DWORD skin_code)
{
    //���Ƴ������ ������ ���� ������ �ٸ���
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
	// 0���� ���� ���̴�.
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


	//	���� ������ �ε� & ����
	for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
	{
        if( pParts[i] != NULL )
        {
            ResourceManager::Instance()->FreeResourceUnitDraw(pParts[i]);
            pParts[i] = NULL;
        }


		assert(code && "���� �̹����ڵ尡 0�Դϴ�.");
		pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code, pAppearInfo->texture_rate);

		assert(pParts[i] &&"���� �̹��� ������ �����ϴ�");
	}
}

//------------------------------------------------------------------------------
/** ���� ĳ���Ѵ�
*/
void AppearanceManager::SetPetAppearance(APPEARANCEINFO* appear_ptr,Object* object_ptr)
{
    ResourceManager* resource_instance = ResourceManager::Instance();
 

	CWzUnitDraw** ppRenderPart = object_ptr->GetParts();

    //	���� ������ �ε� & ����
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

	//	���� ������ �ε� & ����
	for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
	{
		RESOURCECODE code = ResourceManager::Instance()->GetNPCSkinResourceCode( pAppearInfo->pMonsterInfo->m_MonsterCode );

        if( pParts[i] != NULL )
        {
            ResourceManager::Instance()->FreeResourceUnitDraw(pParts[i]);
            pParts[i] = NULL;
        }

		assert(code && "���� �̹����ڵ尡 0�Դϴ�.");
		pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);

		assert(pParts[i] && "���� �̹��� ������ �����ϴ�");
	}
}

//------------------------------------------------------------------------------
/**
*/
void AppearanceManager::SetItemAppearance( APPEARANCEINFO* pAppearInfo, Object *pObject)
{
	// E3 ������ ���ҽ��� ���ɵ��� ���ؼ� Ǯ��. 
	CWzUnitDraw **pParts = pObject->GetParts();
	RESOURCECODE code;

	//	������ ���� 
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
			// ������
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
			// ��.
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
//	TODO: ..�̳��� ���������̴�. 
//*/
//void AppearanceManager::SetSkyBoxAppearance(
//				APPEARANCEINFO* pAppearInfo,
//				Object *pObject,
//				BOOL bRefreshBone )
//{
//	CWzUnitDraw **pParts = pObject->GetParts();
//	//	���� ������ �ε� & ����
//	for( size_t i = 0 ; i < pObject->GetPartCount(); ++i )
//	{
//		RESOURCECODE code = ResourceManager::Instance()->GetTempResourceCode(pAppearInfo->SkyBoxNum );
//		pParts[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
//	}
//}
//
//
////------------------------------------------------------------------------------
///**  ��� �ǰ� ���� ���� �Լ� ����� ����.
//*/
//void AppearanceManager::SetItemMoneyAppearance(
//							APPEARANCEINFO * pAppearInfo,
//							Object * pObject,
//							BOOL bRefreshBone )
//{
//	CWzUnitDraw** pParts = pObject->GetParts();
//	RESOURCECODE code = 40000;
//
//	//	������ ���� 
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

	//	�������Ʈ ���� 
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

    //	���� ������ �ε� & ����
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
	//	���� ������ �ε� & ����
	for( size_t i = 0 ; i < pObject->GetPartCount() ; ++i )
	{
		RESOURCECODE code = ResourceCode::GetCinematicCameraInCharacterScene() + ENGINE_RESOURCE_ID_OFFSET ;
		ppRenderPart[i] = ResourceManager::Instance()->AllocResourceUnitDraw(code);
	}
}

void AppearanceManager::_SetCustumBodyPart( Object* pObject, DWORD CustomCode, float texture_rate )
{
    RESOURCECODE skin_code = CustomCode;

    //������ü�Ѵ�(��ü���ʿ������ continue)
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
        // �̹� ���� ��Ų
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

    // ��Ų�� �ڵ带 �������Ѵ�
    if( parts_index == BP_HAIR || parts_index == BP_FACE )
    {
        // Item ID�� Enchant�� �Ӹ� ���� Set���� ����Ͽ� ����.
        skin_code = ResourceManager::Instance()->GetCharacterPartSkinResourceCode(
            player_ptr->GetClass(), 
            (BodyPart)parts_index, 
            equip_info.dwItemID, 
            equip_info.iEnchant, 
            character_gender);
    }
    else
    {
        //�Ϲ����� ������
        skin_code = ResourceManager::Instance()->GetItemSkinResourceCode(equip_info.dwItemID,
            player_ptr->GetClass(),
            parts_index,
            character_gender);

        sub_type = GetItemSubType(equip_info.iEnchant, equip_info.bDivine);

        // ������� ����ŧ���������� �������Ƿ� �ٲ��༭ �ٸ������� �ش�
        use_specular_effect = equip_info.bDivine;
    }

    CWzUnitDraw** player_parts = pObject->GetParts();

    // ���ҽ� �Ŵ����� ���� ���ҽ��ڵ尪�� ��ȿ�Ҷ��� ���������ϴµ�
    // ������ �ִ� �ڵ��� ������ ���ϰ� �����ڵ��� �������ҽ��� �����ְ� �����Ѵ�
    if (skin_code != 0) 
    {
        if (IsExgistParts(player_parts[parts_index], skin_code) == FALSE )
        {
            // ������ü�Ѵ�
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
                // ����Ÿ���� �ٸ��� ��ȿ������ �����ʾ����� ���⿡ �ɼǿ����� �ٸ� ����Ÿ���� �������ش�.
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
/** ĳ���� ��������(kBeginBodyParts ~ kEndBodyParts)���鼭 ������������ �������¿����� ����
    �ϰ� ������ ���Ȳ(dwItemID==0)������ �⺻������ �������ش� ���� ���� Ŭ������ �����ʴ�
    ��� �������ϰ�� ����Ʈ�� �������ش�
*/
void AppearanceManager::_SetPlayerBodyAppearance(APPEARANCEINFO * pAppearInfo, Object * pObject)
{
    ResourceManager* resource_instance = ResourceManager::Instance();

    CWzUnitDraw** player_parts = pObject->GetParts();
    GenderType::Value character_gender = static_cast<Character*>(pObject)->GetGender();

    // ����ؾ��� resourceID�� üũ���� �ٲ���ϸ� �������� �����ϰ� �ٲߴϴ�.
    for (int parts_index = kBeginBodyParts; parts_index < kEndBodyParts; ++parts_index)
    {
        //safe_code: �迭�ε���üũ
        if (parts_index >= (int)pObject->GetPartCount())
        {
            continue;
        }

        EQUIPAPPEARINFO& equip_info =  pAppearInfo->Equipment[parts_index];
        if (equip_info.dwItemID != 0)
        {
            // ������������ ���� �˻�
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
/** ���� ������ ũ�� �Ѽ�.����� ���� ���������� �Ѽ��϶��� ���������� DeleteAttach���ְ�
    ����϶��� SetAttach(0) , SetAttach(1)���Ƿ� ���⸦�������ش�
        
    (������:0),index(�޼�:1) - (2,3,4)�Ǽ��縮

*/
void AppearanceManager::_SetPlayerWeaponAppearance(APPEARANCEINFO * pAppearInfo, Object * pObject)
{
    Character* character_ptr = static_cast<Character*>(pObject);

    EQUIPAPPEARINFO& weapon_info = pAppearInfo->Equipment[BP_WEAPON];

    //WzMatrix offset_matrix;
    //ZeroMemory(&offset_matrix ,sizeof(offset_matrix));

    RESOURCECODE resource_code = 0;
    GenderType::Value character_gender = character_ptr->GetGender();
    
    //�������� �����Ҷ�
    if (weapon_info.dwItemID != 0)
    {
        resource_code = ResourceManager::Instance()->GetItemSkinResourceCode(weapon_info.dwItemID, (eCHAR_TYPE)pAppearInfo->Job, 0, character_gender);
    }

    AttachmentInfo attach_info;

    // ����������������(����Ŀ,�ٸ���Ż����Ʈ�� �������Ѽ���)
    bool one_hand_weapon = true;

    eCHAR_TYPE class_type = static_cast<eCHAR_TYPE>(pAppearInfo->Job);
    switch(class_type)
    {
    case eCHAR_DRAGON:
        {   
            //��Į�� ��쿡�� ��տ��ܴ�
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
        //�������� ����� ��������
        pObject->DeleteAttachment(1);
    }
    else
    {
        //�������(�������� ��������(1))
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

        //����ó��:��Ű�� ���� Ȱ�ݾ���
        if (attach_success &&
           (pAppearInfo->Job == eCHAR_VALKYRIE))
        {
            character_ptr->CloseCrossbow();
        }
    }
}
//-------------------------------------------------------------------------------------------
/** �� 5���� AttachMent���ִµ� ���� �Ǽ��縮 �ε�����(2,3,4)�̰� ������ (0,1)�̴�
*/
void AppearanceManager::_SetPlayerSpecialAccessoryAppearance(APPEARANCEINFO* pAppearInfo, 
                                                             Object* pObject)
{	
    //Ÿ���� ��������� �Ǽ��縮 ��Ƽ����
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
	�Ʒ��� �ϵ��ڵ��� �������ڵ��� ��� ���.�Ķ�.���� ���ǵ��Ƿ� ���λ����� �Ӷ�
    �۾����� �ٲ�µ�..���� �ϵ��ڵ��Ǿ��ִ»�����
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
    case 49518://!< ���� �߱����߰�
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
                // ��/������ ������ ������ �������� üũ
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
            //�ִϸ��̼� ���̵� ���
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

        // ĳ���Ϳ� ��������
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