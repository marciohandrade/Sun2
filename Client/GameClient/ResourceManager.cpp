#include "SunClientPrecompiledHeader.h"

#include "resourcemanager.h"
#include "ItemUnitCacher.h"
#include "MapUnitCacher.h"
#include "MapInfoParser.h"
#include "ItemInfoParser.h"
#include "NPCInfoParser.h"
#include "skillinfoparser.h"
#include "styleinfoparser.h"
#include "HeroVariationInfoParser.h"
#include "GraphicResourceList.h"
#include "GameFrameWork.h"
#include "LoadScene.h"
#include "GlobalFunc.h"
#include "AppearanceResourceCode.h"


#include "LimitedItemInfoParser.h"


const DWORD ENGINE_RESOURCE_ID_OFFSET = 100000;
const DWORD SCRIPT_RESOURCE_ID_OFFSET = 8000000;

ResourceManager::ResourceManager()
{
	m_pResourceCacher = NULL;
}

ResourceManager::~ResourceManager()
{
	assert( NULL == m_pResourceCacher );
}

VOID ResourceManager::Init()
{
	assert( NULL == m_pResourceCacher );

	
	m_pResourceCacher	= new ResourceCacher;
	m_pResourceCacher->Init();


}

void ResourceManager::LastFree()
{
	
}


VOID ResourceManager::Release()
{
	SAFE_DELETE( m_pResourceCacher );
}
	

void  ResourceManager::IncreaseResourceRefCount(RESOURCECODE code)
{
	if( RESOURCE_INFO *pInfo = GraphicResourceList::Instance()->GetItemInfo((CODETYPE)(code - ENGINE_RESOURCE_ID_OFFSET)) )
	{
		pInfo->dwTotalRefCount++;
	}
}

CWzUnitDraw * ResourceManager::AllocResourceUnitDraw( RESOURCECODE code ,float fRequestRateResizeTexture, bool bFreeAll /*=true*/, bool is_npc)
{	
	assert(code >= ENGINE_RESOURCE_ID_OFFSET);

	IncreaseResourceRefCount(code);
	
	CWzUnit *pUnit = m_pResourceCacher->GetResourceWzUnit( code ,fRequestRateResizeTexture, false, bFreeAll, is_npc);
	if (pUnit)
    {	
        if( CWzUnitDraw* pUnitDraw = (CWzUnitDraw *)Func_ContactWzd( CW_NEW_WZUNITDRAW, 0 ) )
		{
			pUnitDraw->SetWzUnit( pUnit , FALSE);
			return pUnitDraw;
		}
	}

	return NULL;
	
}

CWzUnit * ResourceManager::AllocResourceUnit( RESOURCECODE code)
{
	assert(code >= ENGINE_RESOURCE_ID_OFFSET);

	IncreaseResourceRefCount(code);

	if( CWzUnit* pUnit = m_pResourceCacher->GetResourceWzUnit( code ,-1.0f, false, true) )
	{	
		return pUnit;
	}
	return NULL;
}

VOID ResourceManager::FreeResourceUnit( RESOURCECODE code)
{
	m_pResourceCacher->ReleaseResourceWzUnit(code);
}

VOID ResourceManager::PreLoadResource( RESOURCECODE code ,float fRequestRateResizeTexture)
{	
	if( RESOURCE_INFO* pInfo = GraphicResourceList::Instance()->GetItemInfo((CODETYPE)(code - ENGINE_RESOURCE_ID_OFFSET)) )
	{
		if(0 == pInfo->dwTotalRefCount)
			pInfo->dwTotalRefCount = 1;
	}

	m_pResourceCacher->GetResourceWzUnit( code ,fRequestRateResizeTexture, true);
}


VOID ResourceManager::FreeResourceUnitDraw( CWzUnitDraw * pUnitDraw )
{
    if (pUnitDraw && pUnitDraw->GetWzUnit())
    {
	    RESOURCECODE code = pUnitDraw->GetWzUnit()->GetType();
	    g_pSunRenderer->DestroyResourceForWzUnitDraw(pUnitDraw);
        Func_ContactWzd( CW_DELETE_WZUNITDRAW, pUnitDraw );		
	    m_pResourceCacher->ReleaseResourceWzUnit(code);
    }
}

VOID ResourceManager::FreeResourceUnitDrawVer2( CWzUnitDraw * pUnitDraw, bool bForceRemove/* =false  */)
{
    if (pUnitDraw && pUnitDraw->GetWzUnit())
    {
        RESOURCECODE code = pUnitDraw->GetWzUnit()->GetType();
        g_pSunRenderer->DestroyResourceForWzUnitDraw(pUnitDraw);
        Func_ContactWzd( CW_DELETE_WZUNITDRAW, pUnitDraw );		
        
        //-ck- LRU 정책등은 무시되며 무조건 즉시 삭제된다.
        if (bForceRemove)
	    {
            m_pResourceCacher->SetUnitForceRemove(code);
	    }

        m_pResourceCacher->ReleaseResourceWzUnit(code);
    }
}

RESOURCECODE ResourceManager::GetItemSkinResourceCode(CODETYPE itemcode)
{
    // 실제코드
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(itemcode);
    assert(item_info);
    if (item_info == NULL)
    {
        return 0;
    }

    return (RESOURCECODE)(item_info->m_wWICode + ENGINE_RESOURCE_ID_OFFSET);
}


RESOURCECODE  ResourceManager::GetItemSlotImageResourceCode(CODETYPE itemcode, BOOL bIsLimited/* = FALSE*/)
{
	RESOURCECODE rtCode = 0xFFFFFFFF;
	if(bIsLimited)
	{
		sLIMITED_ITEM_INFO	*pInfo 
			= LimitedItemInfoParser::Instance()->GetLimitedItemInfo(itemcode);
		assert(pInfo);
		if (!pInfo) return 0xFFFFFFFF;
		rtCode = (RESOURCECODE)pInfo->m_dwInvenImage;
	}
	else
	{
		BASE_ITEMINFO *pInfo = ItemInfoParser::Instance()->GetItemInfo(itemcode);
		assert(pInfo);

		if (!pInfo) return 0xFFFFFFFF;

		rtCode = (RESOURCECODE)pInfo->m_wVICode;
	}
	return rtCode;
}

RESOURCECODE ResourceManager::GetItemSkillSlotImageResourceCode(CODETYPE skillcode)
{
	ROOT_SKILLINFO * pInfo = SkillInfoParser::Instance()->GetInfo((SLOTCODE)skillcode);
	
	if (pInfo) 
	{
		if (pInfo->IsSkill())
		{
			SkillScriptInfo *pSkillInfo = (SkillScriptInfo *)pInfo;
			return pSkillInfo->m_dwSkillIconcode;		

		}
		else
		{
			BASE_STYLEINFO *pStyleInfo = (BASE_STYLEINFO *)pInfo;
			return (RESOURCECODE)pStyleInfo->m_dwStyleImage;

		}
		
	}
	else 
	{
		return (RESOURCECODE)0xFFFFFFFF;
	}
}

RESOURCECODE   ResourceManager::GetSkillTransFormImageResourceCode(CODETYPE skillcode)
{
	SkillScriptInfo *pInfo = SkillInfoParser::Instance()->GetSkillInfo((SLOTCODE)skillcode);
	if(pInfo)
	{
		BASE_ABILITYINFO * pAbility =pInfo->GetAbilityInfo( eABILITY_DRAGON_TRANSFORMATION1 );

		if(pAbility)
		{
			return pAbility->m_iOption1;
		}
	}

	return 0;
}

RESOURCECODE   ResourceManager::GetNPCSkinResourceCode(CODETYPE monstercode)
{
	// 실제코드
	BASE_NPCINFO *pInfo = NPCInfoParser::Instance()->GetNPCInfo(monstercode);
	assert(pInfo);

	// 임시코드
	// 이미지없는놈을 하급보초로 만들어버린다.
	if ( !pInfo || !pInfo->m_dwICode)
	{
        return 41001 + ENGINE_RESOURCE_ID_OFFSET;
	}

	return pInfo->m_dwICode + ENGINE_RESOURCE_ID_OFFSET;
}
RESOURCECODE   ResourceManager::GetTempResourceCode(CODETYPE itemcode)
{
	return (itemcode | RESOURCE_TEMP);
}

void ResourceManager::Process(DWORD dwTick)
{
	if (m_pResourceCacher) 
	{
		m_pResourceCacher->Process(dwTick);
	}
}

int  ResourceManager::GetLoadedResourceCount()
{
	if (m_pResourceCacher) 
	{
		return m_pResourceCacher->GetLoadedResourceCount();
	}

	return 0;
}


int ResourceManager::GetWaitForFreeResourceCount()
{
	if (m_pResourceCacher) 
	{
		return m_pResourceCacher->GetWaitForFreeResourceCount();
	}

	return 0;
}

void ResourceManager::FreeAllCachedData()
{
	if (m_pResourceCacher) 
	{
		m_pResourceCacher->ReleaseAllCachedUnit();
	}
}

void ResourceManager::FreeMostData()
{
	if (m_pResourceCacher) 
	{
		m_pResourceCacher->ReleaseMostUnit();
	}
}

RESOURCECODE ResourceManager::GetItemSkinResourceCode(CODETYPE item_code, 
                                     eCHAR_TYPE class_type, 
                                     int body_part, 
                                     GenderType::Value class_gender)
{
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    assert(item_info);
    if (item_info == NULL)
    {
        return 0;
    }

    // 정확한 성별이 아니면 각 클래스의 기본 성별로 설정
    class_gender = ResourceCode::CheckValidGender(class_type, class_gender);

    RESOURCECODE resource_code = 0;

    if (GlobalFunc::IsSSQScene())
    {
        if (item_info->IsWeapon())
        {
            eWEAPONTYPE weapon_type = static_cast<eWEAPONTYPE>(item_info->m_wType);
            resource_code = ResourceCode::GetWeaponCodeForTemple(weapon_type);
        }
        else
        {
            resource_code = ResourceCode::GetPartCodeForTemple(
                class_type, 
                class_gender, 
                static_cast<BodyPart>(body_part));
        }
        if (resource_code == 0)
        {
            return 0;
        }
    }
    else
    {
        resource_code = ResourceCode::GetItemSkinImageCodeByGender(item_info, class_gender);
    }

    return (resource_code + ENGINE_RESOURCE_ID_OFFSET);
}
RESOURCECODE ResourceManager::GetItemSkinResourceCode(CODETYPE item_code, GenderType::Value class_gender)
{
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    assert(item_info);
    if (item_info == NULL)
    {
        return 0;
    }

    RESOURCECODE resource_code = ResourceCode::GetItemSkinImageCodeByGender(item_info, class_gender);
    return (resource_code + ENGINE_RESOURCE_ID_OFFSET);
}

// 캐릭터 뼈대 리소스 코드를 얻어옵니다.
RESOURCECODE ResourceManager::GetCharacterBoneResourceCode(eCHAR_TYPE class_type, GenderType::Value class_gender)
{
    RESOURCECODE bone_resource_code = ResourceCode::GetBaseSkinOfClass(class_type, class_gender);

    return (bone_resource_code + ENGINE_RESOURCE_ID_OFFSET);
}

// 캐릭터 기본 스킨 코드를 얻어옵니다.
RESOURCECODE ResourceManager::GetDefaultSkinCode(eCHAR_TYPE class_type, BodyPart body_part, GenderType::Value class_gender)
{
    // 정확한 성별이 아니면 각 클래스의 기본 성별로 설정
    class_gender = ResourceCode::CheckValidGender(class_type, class_gender);

    RESOURCECODE skin_resource_code = 0;
    if (GlobalFunc::IsSSQScene())
    {
        skin_resource_code = ResourceCode::GetPartCodeForTemple(class_type, class_gender, body_part);
        if (skin_resource_code == 0)
        {
            return 0;
        }
        return skin_resource_code + ENGINE_RESOURCE_ID_OFFSET;
    }

    int offset_code = 0;
    switch (body_part)
    {
    case BP_ARMOR:
        offset_code = 1;
        break;

    case BP_PANTS:
        offset_code = 3;
        break;

    case BP_BOOTS:
        offset_code = 4;
        break;

    case BP_GLOVE:
        offset_code = 5;
        break;

    default:
        return 0;
        break;
    }

    skin_resource_code = ResourceCode::GetBasePartCodeOfClass(class_type, class_gender);
    return (skin_resource_code + offset_code + ENGINE_RESOURCE_ID_OFFSET);
}

// 헤어 및 페이스 스킨코드를 얻어옵니다.
RESOURCECODE ResourceManager::GetCharacterPartSkinResourceCode(eCHAR_TYPE class_type, 
                                              BodyPart body_part, 
                                              int variation_code, 
                                              int variation_set, 
                                              GenderType::Value class_gender)
{
    // 정확한 성별이 아니면 각 클래스의 기본 성별로 설정
    class_gender = ResourceCode::CheckValidGender(class_type, class_gender);
    eCHAR_TYPE image_class_type = ResourceCode::GetImageClassType(class_type, class_gender);

    HERO_VARIATION_PART variation_id;
    variation_id.byClass = image_class_type;
    variation_id.byPart = body_part;
    variation_id.wPartID = variation_code;

    BASE_HeroVariationInfo* variation_info = HeroVariationInfoParser::Instance()->GetHeroVariationInfo(variation_id.dwID);

    if (variation_info != NULL) 
    {
        RESOURCECODE part_resource_code = variation_info->m_dResourceID[variation_set];
        return part_resource_code + ENGINE_RESOURCE_ID_OFFSET;
    }
    else 
    {
        return 0;
    }
}
                                              
float ResourceManager::GetWeaponScale(eCHAR_TYPE class_type, GenderType::Value class_gender)
{
    float weapon_scale = 1.0f;

    class_gender = ResourceCode::CheckValidGender(class_type, class_gender);

    // 추가 성별 클래스 무기 사이즈 
    if ((class_type == eCHAR_BERSERKER) && 
        (class_gender == GenderType::kFemale))
    {
        // 버서커클래스이며 성별이 여성이면 무기크기를 조정
        weapon_scale = 0.9f;
    }
    else if ((class_type == eCHAR_VALKYRIE) && 
        (class_gender == GenderType::kMale))
    {
        // 발키리클래스이며 성별이 남성이면 무기크기를 조정
        weapon_scale = 1.2f;
    }
    return weapon_scale;
}