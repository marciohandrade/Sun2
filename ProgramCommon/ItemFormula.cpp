#include "stdafx.h"
#include "./ItemFormula.h"

#include "NumericValues.h"

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
#include "AwakeningInfoParser.h"
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

//==================================================================================================

namespace ns_formula {
;

// internal declarations
static int _GetDivValue(LEVELTYPE ItemLV);
static WORD _CalcLimitStatIncreaseValue(BYTE Enchant, int DivideValue, eRANK Rank);

//--------------------------------------------------------------------------------------------------

DWORD ns_formula::CalcAttackPower(BYTE Enchant, LEVELTYPE ItemLV)
{
    return GetWeaponEnchantWeightForEnchant(Enchant) +
           Enchant * GetWeaponEnchantWeightForLV(ItemLV);
}

DWORD ns_formula::CalcPhyDef(DWORD PhyDef, BYTE Enchant, LEVELTYPE ItemLV)
{
    return PhyDef + (GetArmorEnchantWeightForEnchant(Enchant) +
                     Enchant * GetArmorEnchantWeightForLV(ItemLV));
}

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
DWORD ns_formula::CalcAwakeningWeapon(const CODETYPE item_code, const BYTE awakening_grade)
{
    return AwakeningInfoParser::Instance()->GetOptionValue(item_code, awakening_grade);
}

DWORD ns_formula::CalcAwakeningArmor(const CODETYPE item_code, const BYTE awakening_grade)
{
    return AwakeningInfoParser::Instance()->GetOptionValue(item_code, awakening_grade);
}
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

static int ns_formula::_GetDivValue(LEVELTYPE ItemLV)
{
    // 공식 적용시 Item 레벨이 18이상이 되어서는 안된다.
    // 현 공식에서는 30이상값을 쓸수 없다.
    // while(ItemLV >= 18)  ItemLV -= 11;

    LEVELTYPE conv_lv = ItemLV;
    switch (ItemLV)
    {
    case 13:
        conv_lv = 1;
    	break;
    case 14:
        conv_lv = 2;
        break;
    case 15:
        conv_lv = 3;
        break;
    case 16:
        conv_lv = 4;
        break;
    case 17:
        conv_lv = 5;
        break;
    case 18:
        conv_lv = 6;
        break;
    case 22:
        conv_lv = 21;
        break;
    case 23:
        conv_lv = 21;
        break;
    case 24:
        conv_lv = 23;
        break;
    case 33:
        conv_lv = 31;
        break;
    case 34:
        conv_lv = 35;
        break;
    }
    if (conv_lv >= 18) 
    {
        conv_lv -= 11;
    }
    return 2 * (3 - conv_lv / 6);
}

static WORD ns_formula::_CalcLimitStatIncreaseValue(BYTE Enchant, int DivideValue, eRANK Rank)
{
    return (Enchant * (Enchant + 1) / DivideValue + Enchant) + GetLimitValueForRank(Rank);
}

// (lst1024)(080904) 기존 계산식에 변경값을 적용하려 했으나 기획에서 변경값 계산에 어려움이 있어
// 팀장님과 협의 기획 원하는대로 하기로함
// 기존 상승폭(_CalcLimitStatIncreaseValue) 자체를 스크립트값(fLimitRatio) 으로 제어
// 2중으로 계산이 되는 구조.

WORD ns_formula::CalcLimitStatWeapon(WORD ItemLimitStr, BYTE Enchant, eRANK Rank, LEVELTYPE ItemLV,
                                     float fLimitRatio)
{
#ifdef _NA_006363_20130103_NOT_CALC_ENCHANT_STAT
    return ItemLimitStr; // 0인챈일 때의 값을 유지
#endif //_NA_006363_20130103_NOT_CALC_ENCHANT_STAT

    //
    int  DivideValue        = _GetDivValue(ItemLV);
#ifdef _NA_003966_20111227_ADD_ENCHANT
    if (Enchant > 12)
    {
        // 기획팀 요청에 따라 12 인첸트 이상은 12 인첸트로 취급.
        Enchant = 12;
    }
#endif // _NA_003966_20111227_ADD_ENCHANT
    WORD wCurIncreaseValue  = _CalcLimitStatIncreaseValue(Enchant, DivideValue, Rank);

    // (lst1024)(090223) Enchant가 0일때는 Enchant-1 == 255값으로 계산됨. 중간 계산이 필요 없음.
    if (0==Enchant) return ItemLimitStr + wCurIncreaseValue;

    //.//

    WORD wBefIncreaseValue  = _CalcLimitStatIncreaseValue(Enchant-1, DivideValue, Rank);

    return ItemLimitStr + wBefIncreaseValue +
           (WORD)((float)(wCurIncreaseValue - wBefIncreaseValue) * fLimitRatio);
}

WORD ns_formula::CalcLimitStatArmor(WORD ItemLimitStr, BYTE Enchant, eRANK Rank, LEVELTYPE ItemLV,
                                    float fLimitRatio)
{
    return CalcLimitStatWeapon(ItemLimitStr, Enchant, Rank, ItemLV, fLimitRatio);
}

WORD ns_formula::CalcLimitLVAccessory(WORD ItemLimitEqLevel, eRANK Rank)
{
    return (ItemLimitEqLevel+Rank*3);
}

// UNUSED:
//WORD ns_formula::CalcPhyDefForDivineArmor(WORD wPhyDef, LEVELTYPE ItemLV)
//{
//    return ((wPhyDef*11/8)+((ItemLV+53)/6));
//}

DWORD ns_formula::CalcDecreaseItemDuraValue(DWORD decreaseItemDuraRatio)
{
    DWORD dwDecreaseDuraVal = random(1, 5);
    if ((DWORD)(random(0, 100)) <= decreaseItemDuraRatio)
    {
        dwDecreaseDuraVal /= 2;
    }

    return dwDecreaseDuraVal;
}

}; //end of namespace 'ns_formula'

//==================================================================================================
