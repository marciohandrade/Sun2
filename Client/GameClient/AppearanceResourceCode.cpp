#include "SunClientPrecompiledHeader.h"
#include "AppearanceResourceCode.h"
#include "ItemInfoParser.h"

namespace
{
    const int kNumberOfClass = (eCHAR_TYPE_MAX - 1);
    const int kNumberOfBodyParts = BP_COUNT;
    const int kGenderMax = GenderType::kFemale;

    // 각 클래스의 기본 성별
    const GenderType::Value kBaseGenderOfClass[kNumberOfClass] = 
    {
        GenderType::kMale,
        GenderType::kMale,
        GenderType::kMale,
        GenderType::kFemale,
        GenderType::kFemale,
        GenderType::kFemale, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        GenderType::kMale,   //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        GenderType::kFemale, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };

    // 각 클래스/성별에 대한 외형클래스(*)
    const eCHAR_TYPE kImageClassType[kNumberOfClass][kGenderMax] = 
    {
        {eCHAR_BERSERKER, eCHAR_VALKYRIE}, 
        {eCHAR_DRAGON, eCHAR_VALKYRIE}, 
        {eCHAR_SHADOW, eCHAR_ELEMENTALIST}, 
        {eCHAR_DRAGON, eCHAR_VALKYRIE}, 
        {eCHAR_SHADOW, eCHAR_ELEMENTALIST},
        {eCHAR_MYSTIC, eCHAR_MYSTIC}, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {eCHAR_HELLROID, eCHAR_HELLROID}, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {eCHAR_WITCHBLADE, eCHAR_WITCHBLADE}, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };

    // 기본 베이스 리소스코드
    const DWORD kClassSkinCode[kNumberOfClass][kGenderMax] = 
    {
        {40001, 40006}, // 버서커 남 여  eCHAR_BERSERKER
        {40002, 40007}, // 드래곤 남 여  eCHAR_DRAGON
        {40003, 40008}, // 섀도우 남 여  eCHAR_SHADOW
        {40009, 40004}, // 발키리 남 여  eCHAR_VALKYRIE
        {40010, 40005},  // 엘리   남 여  eCHAR_ELEMENTALIST
        {40011, 40012},  // 미스틱 남 여  eCHAR_MYSTIC //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {40013, 40014}, // 헬로이드 남 여 eCHAR_HELLROID //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {40015, 40016}, // 위블 남 여    eCHAR_WITCHBLADE //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };

    // 기본파츠외형정보 GetDefaultSkinCode
    const DWORD kPartCodeOfBaseSkin[kNumberOfClass][kGenderMax] = 
    {
        {60001, 60025}, // 버서커 남 여  eCHAR_BERSERKER
        {60009, 60025}, // 드래곤 남 여  eCHAR_DRAGON
        {60017, 60033}, // 섀도우 남 여  eCHAR_SHADOW
        {60009, 60025}, // 발키리 남 여  eCHAR_VALKYRIE
        {60017, 60033}, // 엘리   남 여  eCHAR_ELEMENTALIST
        {60121, 60121},  // 미스틱 남 여  eCHAR_MYSTIC //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        {60129, 60129}, // 헬로이드 남 여 eCHAR_HELLROID //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        {60137, 60137}, // 위블 남 여    eCHAR_WITCHBLADE //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };

#ifdef _NA_008633_20160104_ADD_DECORATECOSTUME
	// 타락한 사원 장비 리소스 코드
	const DWORD kPartCodeForTemple[kNumberOfClass][kNumberOfBodyParts] = 
	{
		0, 0, 0, 11540, 11541, 11543, 11542, 11544, 0, 0, 0, 0, 0,  
		0, 0, 0, 11545, 11546, 11548, 11547, 11549, 0, 0, 0, 0, 0,  
		0, 0, 0, 11550, 11551, 11552, 11553, 11554, 0, 0, 0, 0, 0,  
		0, 0, 0, 11555, 11556, 11558, 11557, 11559, 0, 0, 0, 0, 0,  
		0, 0, 0, 11560, 11561, 11563, 11562, 11564, 0, 0, 0, 0, 0,  
		0, 0, 0, 11613, 11614, 11616, 11615, 11617, 0, 0, 0, 0, 0,  //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
		0, 0, 0, 14708, 14709, 14710, 14711, 14712, 0, 0, 0, 0, 0,  //_NA_000000_20130812_NEW_CHARACTER_HELLROID
		0, 0, 0, 14883, 14884, 14885, 14886, 14887, 0, 0, 0, 0, 0,  //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
	};
#else
    // 타락한 사원 장비 리소스 코드
    const DWORD kPartCodeForTemple[kNumberOfClass][kNumberOfBodyParts] = 
    {
        0, 0, 0, 11540, 11541, 11543, 11542, 11544, 0, 0, 0, 0, 
        0, 0, 0, 11545, 11546, 11548, 11547, 11549, 0, 0, 0, 0, 
        0, 0, 0, 11550, 11551, 11552, 11553, 11554, 0, 0, 0, 0, 
        0, 0, 0, 11555, 11556, 11558, 11557, 11559, 0, 0, 0, 0, 
        0, 0, 0, 11560, 11561, 11563, 11562, 11564, 0, 0, 0, 0, 
        0, 0, 0, 11613, 11614, 11616, 11615, 11617, 0, 0, 0, 0, //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        0, 0, 0, 14708, 14709, 14710, 14711, 14712, 0, 0, 0, 0, //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        0, 0, 0, 14883, 14884, 14885, 14886, 14887, 0, 0, 0, 0, //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };
#endif // _NA_008633_20160104_ADD_DECORATECOSTUME

    // 타락한 사원 무기 리소스 코드
    const DWORD kWeaponCodeForTemple[eWEAPONTYPE_MAX] = 
    {
        0,      //eWEAPONTYPE_INVALID
        11565,  //eWEAPONTYPE_TWOHANDAXE
        11566,  //eWEAPONTYPE_TWOHANDSWORD
        11567,  //eWEAPONTYPE_ONEHANDSWORD
        11568,  //eWEAPONTYPE_SPEAR
        11569,  //eWEAPONTYPE_DAGGER
        11570,  //eWEAPONTYPE_WHIP
        11571,  //eWEAPONTYPE_ONEHANDCROSSBOW
        11572,  //eWEAPONTYPE_ETHERWEAPON
        0,      //eWEAPONTYPE_SCIMITAR
        11573,  //eWEAPONTYPE_STAFF
        11574,  //eWEAPONTYPE_ORB
        0,      //eWEAPONTYPE_FISHING_POLE
        0,      //eWEAPONTYPE_PUNCH
        11618,  //eWEAPONTYPE_GUNTLET //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        11619,  //eWEAPONTYPE_POLEARM //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
        1080,  //eWEAPONTYPE_HELLROID_1 //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        3111,  //eWEAPONTYPE_ARCBLADE //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE 
        // 3112
    };

    const DWORD kCinematicCameraInCharacterScene = 888888;
}

int ResourceCode::GetNumberOfClass()
{
    return kNumberOfClass;
}

int ResourceCode::GetNumberOfParts()
{
    return BP_COUNT;
}

GenderType::Value ResourceCode::GetBaseGenderOfClass(eCHAR_TYPE class_type)
{
#if defined(_JAPAN)
    if ((class_type <= eCHAR_NONE) || (class_type >= eCHAR_TYPE_MAX))
    {
        class_type = eCHAR_ELEMENTALIST; // 배열 범위 초과 시 디폴트를 가장 많은 엘리로 함
    }
#endif

    return kBaseGenderOfClass[class_type - 1];
}

eCHAR_TYPE ResourceCode::GetSecondSkinClassType(eCHAR_TYPE base_class)
{
    return kImageClassType[base_class-1][1];
}

eCHAR_TYPE ResourceCode::GetImageClassType(eCHAR_TYPE class_type, GenderType::Value class_gender)
{
#if defined(_JAPAN)
    if ((class_type <= eCHAR_NONE) || (class_type >= eCHAR_TYPE_MAX))
    {
        class_type = eCHAR_ELEMENTALIST; // 배열 범위 초과 시 디폴트를 가장 흔한 엘리로 함
    }
#endif

    class_gender = CheckValidGender(class_type, class_gender);

    return kImageClassType[class_type - 1][class_gender - 1];
}

DWORD ResourceCode::GetBaseSkinOfClass(eCHAR_TYPE class_type, GenderType::Value class_gender)
{
#if defined(_JAPAN)
    if ((class_type <= eCHAR_NONE) || (class_type >= eCHAR_TYPE_MAX))
    {
        class_type = eCHAR_ELEMENTALIST; // 배열 범위 초과 시 디폴트를 가장 흔한 엘리로 함
    }
#endif

    class_gender = CheckValidGender(class_type, class_gender);

    return kClassSkinCode[class_type - 1][class_gender - 1];
}

DWORD ResourceCode::GetBasePartCodeOfClass( eCHAR_TYPE class_type, GenderType::Value class_gender)
{
#if defined(_JAPAN)
    if ((class_type <= eCHAR_NONE) || (class_type >= eCHAR_TYPE_MAX))
    {
        class_type = eCHAR_ELEMENTALIST; // 배열 범위 초과 시 디폴트를 가장 흔한 엘리로 함
    }
#endif

    class_gender = CheckValidGender(class_type, class_gender);

    return kPartCodeOfBaseSkin[class_type - 1][class_gender - 1];
}

DWORD ResourceCode::GetPartCodeForTemple(eCHAR_TYPE class_type, GenderType::Value class_gender, BodyPart body_part)
{
#if defined(_JAPAN)
    if ((class_type <= eCHAR_NONE) || (class_type >= eCHAR_TYPE_MAX))
    {
        class_type = eCHAR_ELEMENTALIST; // 배열 범위 초과 시 디폴트를 가장 흔한 엘리로 함
    }
#endif

    class_gender = CheckValidGender(class_type, class_gender);
    class_type = GetImageClassType(class_type, class_gender);

    return kPartCodeForTemple [class_type -1][body_part];
}

DWORD ResourceCode::GetWeaponCodeForTemple(eWEAPONTYPE weapon_type)
{
    return kWeaponCodeForTemple[weapon_type];
}

GenderType::Value ResourceCode::CheckValidGender(eCHAR_TYPE class_type, GenderType::Value class_gender)
{
    if ((class_gender != GenderType::kMale) && 
        (class_gender != GenderType::kFemale))
    {
        class_gender = GetBaseGenderOfClass(class_type);
    }
    return class_gender;
}

DWORD ResourceCode::GetItemSkinImageCodeByGender(BASE_ITEMINFO* item_info, GenderType::Value class_gender)
{
    RESOURCECODE resource_code = 0;

    if (class_gender == GenderType::kMale)
    {
        resource_code = item_info->m_wWICode;
    }
    else if (class_gender == GenderType::kFemale)
    {
        resource_code = item_info->m_wWICode2;
    }
    
    return resource_code;
}

DWORD ResourceCode::GetCinematicCameraInCharacterScene()
{
    return kCinematicCameraInCharacterScene;
}