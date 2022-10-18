#include "SunClientPrecompiledHeader.h"
#include "CharacterData.h"
#include "AppearanceResourceCode.h"




cCharacterData::cCharacterData()
{
    m_EquipmentContainer = NULL;

    for (int index = 0; index < MAX_EQUIPMENT_SLOT_NUM; ++index)
    {
        m_RealEquipmentFlag[index] = false;
    }

    m_IsInitialized = false;
}

cCharacterData::~cCharacterData()
{
}

void cCharacterData::Initialize()
{
    m_IsInitialized = true;
}

void cCharacterData::Release()
{
}

eCHAR_TYPE cCharacterData::GetImageClass()
{
    return ResourceCode::GetImageClassType(GetClass(), GetGender());
}

DWORD cCharacterData::GetClassCode()
{
    ClassCodeEx class_code_ex;
    class_code_ex.class_type = GetClass();
    class_code_ex.gender = GetGender();

    return class_code_ex.class_code;
}


bool cCharacterData::GetRealEquipFlag(int index) const
{
    if (index < 0 || index >= MAX_EQUIPMENT_SLOT_NUM)
    {
        return false;
    }

    return m_RealEquipmentFlag[index];
}

void cCharacterData::SetRealEquipFlag(int index, bool equip_flag)
{
    if (index < 0 || index >= MAX_EQUIPMENT_SLOT_NUM)
    {
        return;
    }
    m_RealEquipmentFlag[index] = equip_flag;
}

float cCharacterData::GetHeightVariationValue()
{
    if( IsCharacterDataCreated() == FALSE )
    {
        return 1.0f;
    }

    BASE_HeroHeightInfo* height_info = HeroHeightInfoParser::Instance()->GetHeroHeightInfo(GetClass());
    int height_variation = GetHeightVariation();
    assert(height_variation >= 0 && height_variation < MAX_HEIGHT_VARIATION);

    float character_scale = 1.0f;

    if (height_info != NULL)
    {
        character_scale = height_info->m_fHeight[height_variation];
    }

    return character_scale;
}
