#pragma once
#ifndef PROGRAMCOMMON_ITEMFORMULA_H
#define PROGRAMCOMMON_ITEMFORMULA_H

#include "ItemOptionHeader.h"

//==================================================================================================
// @history
//  - ~10.12.20
//  - 10.12.20, waverix, remove unused section
//
//==================================================================================================

namespace ns_formula {
;

// 인첸트에 따른 공격력, 방어력 공식
extern DWORD CalcAttackPower(BYTE Enchant, LEVELTYPE ItemLV);
extern DWORD CalcPhyDef(DWORD PhyDef, BYTE Enchant, LEVELTYPE ItemLV);

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
extern DWORD CalcAwakeningWeapon(const CODETYPE item_code, const BYTE awakening_grade);
extern DWORD CalcAwakeningArmor(const CODETYPE item_code, const BYTE awakening_grade);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

// 내구도 감소 공식
//extern float CalcWeaponDura(DWORD MonsterDef, DWORD WeaponMinDamage, DWORD WeaponMaxDamage);
//extern float CalcArmorDura(DWORD MonMinAttackPower, DWORD MonMaxAttackPower, DWORD ItemDef);
// 내구도 당 아이템 수리 공식
//extern DWORD CalcPriceForDura(DWORD ItemPrice, DURATYPE ItemDura);


// 제한 수치
// 인첸트 + 랭크
// 무기, 방어구에 대한 제한 수치
extern WORD CalcLimitStatWeapon(WORD ItemLimitStr, BYTE Enchant, eRANK Rank, LEVELTYPE ItemLV, float fLimitRatio);
extern WORD CalcLimitStatArmor(WORD ItemLimitStr, BYTE Enchant, eRANK Rank, LEVELTYPE ItemLV, float fLimitRatio);

// 악세서리 레벨 제한수치
extern WORD CalcLimitLVAccessory(WORD ItemLimitEqLevel, eRANK Rank);

//// UNUSED: Divine Item 수치
//// 방어구
//extern WORD CalcPhyDefForDivineArmor(WORD wPhyDef, LEVELTYPE level);

// 내구도
extern DWORD CalcDecreaseItemDuraValue(DWORD decreaseItemDuraRatio);

}; //end of namespace 'ns_formula'

#endif //PROGRAMCOMMON_ITEMFORMULA_H