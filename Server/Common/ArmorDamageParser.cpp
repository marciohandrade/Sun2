#include "StdAfx.h"
#include ".\armordamageparser.h"
#include <SolarFileReader.h>

ArmorDamageParser::ArmorDamageParser(void) : armor_damage_table_(NULL)
{
}

ArmorDamageParser::~ArmorDamageParser(void)
{
}

VOID ArmorDamageParser::Init(DWORD pool_size)
{
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    armor_damage_table_ = new ARMOR_DAMAGEINFO_HASH;
    armor_damage_table_->Initialize(pool_size);
#endif
}

VOID ArmorDamageParser::Release()
{
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    unload();
    SAFE_DELETE(armor_damage_table_);
#endif
}

VOID ArmorDamageParser::unload()
{
    if (armor_damage_table_ != NULL)
    {
        ARMOR_DAMAGE_INFO* armor_damage_info = NULL;
        ARMOR_DAMAGEINFO_ITER pos = armor_damage_table_->begin();
        ARMOR_DAMAGEINFO_ITER end_pos = armor_damage_table_->end();
        for (; pos != end_pos; ++pos)
        {
            armor_damage_info = *pos;
            SAFE_DELETE(armor_damage_info);
        }
        armor_damage_table_->RemoveAll();
    }
}

BOOL ArmorDamageParser::load(BOOL is_reload)
{
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    const int row_size = GetRowSize();
    for (int row = 0; row < row_size; ++row)
    {
        // (CHANGES)(20100309)(hukim) armor_damage_table_에 키로 삽입하기 때문에 index는 사용되지 않
        // 기 때문에 주석처리, 디버깅 이상의 의미도 없고 연산 낭비이다.

        //const WORD index = GetDataWORD("Index", row);
        const BYTE weapon_type = GetDataBYTE("WeaponType", row);
        const BYTE armor_type = GetDataBYTE("ArmorType", row);

        const WORD armor_damage_info_key = make_key(armor_type, weapon_type);
        ARMOR_DAMAGE_INFO* armor_damage_info = armor_damage_table_->GetData(armor_damage_info_key);
        if (armor_damage_info == NULL)
        {
            armor_damage_info = new ARMOR_DAMAGE_INFO;
            armor_damage_table_->Add(armor_damage_info, armor_damage_info_key);
        }
        else
        {
            if (is_reload == FALSE)
            {
                SUNLOG(eCRITICAL_LOG, _T("Script File Data Error, ArmorType = %d, AttackType = %d"), 
                       armor_type, weapon_type );
                ASSERT(!"데이터에 오류가 있습니다.");
            }
        }       

        armor_damage_info->byWeaponType = weapon_type;
        armor_damage_info->byArmorType = armor_type;
        armor_damage_info->Damage = GetDataWORD("DamageRatio", row);

        if (armor_damage_info->Damage == 0)
        {
            SUNLOG(eCRITICAL_LOG, _T(" ArmorTypeDamage is 0 "));
        }
    }
#endif
    return TRUE;
}

DAMAGETYPE ArmorDamageParser::FindDamageRatio(BYTE armor_type, BYTE attack_type) const
{
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    WORD armor_damage_info_key = make_key(armor_type, attack_type);
    ARMOR_DAMAGEINFO_ITER pos = armor_damage_table_->find(armor_damage_info_key);
    ARMOR_DAMAGE_INFO* armor_damage_info = *pos;
    if (armor_damage_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("ArmorTypeDamage is not Exist! armor_type[%d] attack_type[%d]\n"), 
               armor_type, attack_type); 
    }
    else
    {
        return armor_damage_info->Damage;
    }
#endif
    return 100;
}

//----------------------------------------------------------------------------
/**
wArmorType | wAttackType |
8bit | 8bit |
*/
WORD ArmorDamageParser::make_key(BYTE armor_type, BYTE attack_type) const
{
    return WORD((WORD(armor_type) << 8) | WORD(attack_type));
}

BOOL ArmorDamageParser::LoadScript(eSCRIPT_CODE script_code, BOOL is_reload)
{
    switch(script_code)
    {
    case SCRIPT_ARMORTYPEDAMAGEINFO:
        return load(is_reload);
    };
    return FALSE;
}
