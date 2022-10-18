#include "StdAfx.h"
#include ".\enchantparser.h"
#include <ItemOptionHeader.h>
#include <iostream>

EnchantParser::EnchantParser()
{
}

EnchantParser::~EnchantParser()
{
}

BOOL EnchantParser::LoadScript(eSCRIPT_CODE code, BOOL is_reload)
{
    const bool reload = (is_reload == TRUE);
    if (code == SCRIPT_ENCHANT)
    {
        if (!FlowControl::FCAssert(LoadScript(reload)))
        {
            return FALSE;
        }
        return TRUE;
    }
    return FALSE;
}

eENCHANT_GRADE EnchantParser::GetEnchantGrade(const uint8_t enchant) const
{
    const uint32_t grade = static_cast<uint32_t>(kEnchantLV0 << enchant);
    if (kEnchantLow & grade)
    {
        return kEnchantLow;
    }        
    if (kEnchantMiddle & grade)
    {
        return kEnchantMiddle;
    }        
    if (kEnchantHigh & grade)
    {
        return kEnchantHigh;
    }
    if (kEnchantHighest & grade)
    {
        return kEnchantHighest;
    }
    return kEnchantErrLV;
}

POSTYPE EnchantParser::CalcMaterialNum(
    const uint16_t material_number, const double material_fare) const
{
    const POSTYPE cost = static_cast<POSTYPE>(material_number * material_fare);
    return cost ? cost : 1;
}

int	EnchantParser::PercentOptionToInt(const eENCHANT_OPTION option) const
{
    if (option & eENCHANT_100PER)
    {
        return 100;
    }
    if (option & eENCHANT_75PER)
    {
        return 75;
    }
    if (option & eENCHANT_50PER)
    {
        return 50;
    }
    if (option & eENCHANT_25PER)
    {
        return 25;
    }
    return 0;
}

const MATERIALS_FOR_LEVEL* EnchantParser::GetEnchantMatInfo( 
    const LEVELTYPE level, 
    const uint8_t enchant, 
    const bool is_weapon, 
    const bool is_limited, 
    const eENCHANT_OPTION option) const
{
    const Materials& materials = is_weapon ? weapon_materials_ : armor_materials_;
    Materials::const_iterator it = materials.find(level);
    if (it == materials.end())
    {
        return NULL;
    }
    const Material& material = it->second;
    if (enchant >= _countof(material.normals))
    {
        return NULL;
    }
    if (option & eENCHANT_CASH)
    {
        return is_limited ? &(material.cash_limiteds[enchant]) : &(material.cash_normals[enchant]);
    }
    else
    {
        return is_limited ? &(material.limiteds[enchant]) : &(material.normals[enchant]);
    }
    return NULL;
}
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
const MATERIALS_FOR_LEVEL* EnchantParser::GetEnchantInfo(const bool is_weapon, 
                                                            const LEVELTYPE item_level,
                                                            const uint8_t enchant) const
{
    const int temp_enchant = static_cast<int>(enchant)-1;

    const Materials& materials = is_weapon ? weapon_materials_ : armor_materials_;
    Materials::const_iterator it = materials.find(item_level);
    if (it == materials.end())
    {
        return NULL;
    }
    const Material& material = it->second;
    if (temp_enchant >= _countof(material.normals))
    {
        return NULL;
    }

    return &(material.normals[temp_enchant]);
}
#endif // _NA_006731_20130521_ENCHANT_ADD_OPTION

float EnchantParser::GetLimitRatio( 
    const LEVELTYPE level, 
    const uint8_t enchant, 
    const bool is_weapon, 
    const bool is_limited) const
{
    const int temp_enchant = static_cast<int>(enchant)-1;
    if (temp_enchant < 0 || temp_enchant >= MAX_ENCHANT_GRADE)
    {
        return 1;
    }
    const MATERIALS_FOR_LEVEL* const material_info = 
        GetEnchantMatInfo(level, static_cast<uint8_t>(temp_enchant), is_weapon, is_limited);
    if (material_info && material_info->IsLimitRatio())
    {
        return material_info->GetLimitRatio();
    }        
    return 1;
}

bool EnchantParser::IsValid(
    const int row, 
    const int code, 
    const uint8_t item_type, 
    const MATERIALS_FOR_LEVEL& materal_info) const
{
    if (code == 0 || code > kMaxCode)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            _T("|%s|Code가 올바르지 않습니다.|줄 = %d, Code = %d"), 
            __FUNCTION__, 
            row, 
            code 
        );
        assert(false);
        return false;
    }
    if (item_type >= Material::kMaxItemType)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            _T("|%s|ItemType이 올바르지 않습니다.|줄 = %d, Code = %d, ItemType = %d"), 
            __FUNCTION__, 
            row, 
            code, 
            item_type 
        );
        assert(false);
        return false;
    }
    if (materal_info.enchant_ > MAX_ENCHANT_GRADE)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            _T("|%s|Enchant가 올바르지 않습니다.|줄 = %d, Code = %d, Enchant = %d"), 
            __FUNCTION__, 
            row, 
            code, 
            materal_info.enchant_ 
        );
        assert(false);
        return false;
    }
    for (int i = 0; i < MATERIALS_FOR_LEVEL::kMaxFareNumber; ++i)
    {
        if (materal_info.fare_ratios_[i].m_HaimFare < 0.001)
        {
            SUNLOG( 
                eCRITICAL_LOG, 
                _T("|%s|HaimFare가 올바르지 않습니다.|줄 = %d, Code = %d, Index = %d, HaimFare = %f"), 
                __FUNCTION__, 
                row, 
                code, 
                i+1, 
                materal_info.fare_ratios_[i].m_HaimFare
            );
            assert(false);
            return false;
        }
        if (materal_info.fare_ratios_[i].m_MaterialFare < 0.001)
        {
            SUNLOG( 
                eCRITICAL_LOG, 
                _T("|%s|MaterialFare가 올바르지 않습니다.|줄 = %d, Code = %d, Index = %d, MaterialFare = %f"), 
                __FUNCTION__, 
                row, 
                code, 
                i+1, 
                materal_info.fare_ratios_[i].m_MaterialFare
            );
            assert(false);
            return false;
        }
    }
    if (materal_info.down_grade_.percent > 100)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            _T("|%s|DownPer가 올바르지 않습니다.|줄 = %d, Code = %d, DownPer = %d"), 
            __FUNCTION__, 
            row, 
            code, 
            materal_info.down_grade_.percent
        );
        assert(false);
        return false;
    }
    if (materal_info.down_grade_.level > MAX_ENCHANT_GRADE)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            _T("|%s|DownLV가 올바르지 않습니다.|줄 = %d, Code = %d, DownLV = %d"), 
            __FUNCTION__, 
            row, 
            code, 
            materal_info.down_grade_.level
        );
        assert(false);
        return false;
    }
    if (fabs(materal_info.limit_ratio_.ratio) < 0.001)
    {
        SUNLOG( 
            eCRITICAL_LOG, 
            _T("|%s|LimitRatio가 올바르지 않습니다.|줄 = %d, Code = %d, LimitRatio = %f"), 
            __FUNCTION__, 
            row, 
            code, 
            fabs(materal_info.limit_ratio_.ratio)
        );
        assert(false);
        return false;
    }
    return true;
}

bool EnchantParser::LoadScript(const bool reload)
{
    if (reload)
    {
        weapon_materials_.clear();
        armor_materials_.clear();
    }

    enum { kMaxFieldName = 40 };
    char field_name[kMaxFieldName] = {0, };
    
    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        const int code = GetDataInt(_T("bCode"), row);
        const uint8_t item_type = GetDataBYTE(_T("bItemType"), row);
        const LEVELTYPE item_level = GetDataWORD(_T("bItemLV"), row);

        MATERIALS_FOR_LEVEL temp_materal_info;
        temp_materal_info.enchant_ = GetDataBYTE(_T("bEnchant"), row);

        for (int i = 0; i < MATERIALS_FOR_LEVEL::kMaxMaterialNumber; ++i)
        {
            _snprintf(field_name, MAX_FIELD_NAME_SIZE, "wMaterial%d", i+1);
            temp_materal_info.materials_[i].m_MaterialItemCode = GetDataWORD(field_name, row);
            
            _snprintf( field_name, MAX_FIELD_NAME_SIZE, "wMaterial%dNum", i+1);
            temp_materal_info.materials_[i].m_MaterialItemNum = GetDataWORD(field_name, row);
        }
        for (int i = 0 ; i < MATERIALS_FOR_LEVEL::kMaxFareNumber ; ++i)
        {
            _snprintf(field_name, MAX_FIELD_NAME_SIZE, "HaimFare%d", i+1);
            temp_materal_info.fare_ratios_[i].m_HaimFare = GetDataFloat(field_name, row);
            
            _snprintf(field_name, MAX_FIELD_NAME_SIZE, "MaterialFare%d", i+1);
            temp_materal_info.fare_ratios_[i].m_MaterialFare = GetDataFloat(field_name, row);
        }
        temp_materal_info.down_grade_.grade = GetDataBYTE(_T("bDownGrade"), row);
        temp_materal_info.down_grade_.percent = GetDataBYTE(_T("byDownPer"), row);
        temp_materal_info.down_grade_.level = GetDataBYTE(_T("byDownLV"), row);
        temp_materal_info.limit_ratio_.is_limit_ratio = (GetDataBYTE(_T("bLimitRatio"), row) == 1);
        temp_materal_info.limit_ratio_.ratio = GetDataFloat(_T("dLimitRatio"), row);
#ifdef _NA_20100307_BETAKEY_SYSTEM
        temp_materal_info.betakey_.m_betakey = GetDataInt("BetaKeyCode", row);
        temp_materal_info.betakey_.m_pay_ratio = GetDataFloat("PayApp", row);
#endif
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
        char szName[32] = {0,};
        for (int op = 0; op < MATERIALS_FOR_LEVEL::kMaxAddOptionNumber; ++op)
        {
            _snprintf(szName, 32, "AddOptionKind%d", op + 1);
            temp_materal_info.add_option_kind_[op] = GetDataWORD(szName, row);
            _snprintf(szName, 32, "AddOptionType%d", op + 1);
            temp_materal_info.add_option_type_[op] = GetDataBYTE(szName, row);
            _snprintf(szName, 32, "AddOptionValue%d", op + 1);
            temp_materal_info.add_option_value_[op] = GetDataInt(szName, row);
        }
#endif // _NA_006731_20130521_ENCHANT_ADD_OPTION

        if (IsValid(row, code, item_type, temp_materal_info) == false)
        {
            return false;
        }

        Materials& materials = (item_type == Material::kWeapon) ? weapon_materials_ : armor_materials_;
        Materials::iterator it = materials.find(item_level);
        if (it == materials.end())
        {
            Material new_material;
            new_material.item_type = item_type;
            new_material.item_level = item_level;
            materials[item_level] = new_material;
        }
        Material& material = materials[item_level];
        MATERIALS_FOR_LEVEL* material_info(NULL);
        {
            const int enchant = static_cast<int>(temp_materal_info.enchant_)-1;
            if (code < kMinLimitedCode)
            {
                material_info = &material.normals[enchant];
            }
            else if (code < kMinCashNormalCode)
            {
                material_info = &material.limiteds[enchant];
            }
            else if (code < kMinCashLimitedCode)
            {
                material_info = &material.cash_normals[enchant];
            }
            else
            {
                material_info = &material.cash_limiteds[enchant];
            }
        }
        if (material_info == NULL)
        {
            return false;
        }
        *material_info = temp_materal_info;
    }
    return true;
}