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

// ��þƮ�� ���� ���ݷ�, ���� ����
extern DWORD CalcAttackPower(BYTE Enchant, LEVELTYPE ItemLV);
extern DWORD CalcPhyDef(DWORD PhyDef, BYTE Enchant, LEVELTYPE ItemLV);

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
extern DWORD CalcAwakeningWeapon(const CODETYPE item_code, const BYTE awakening_grade);
extern DWORD CalcAwakeningArmor(const CODETYPE item_code, const BYTE awakening_grade);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

// ������ ���� ����
//extern float CalcWeaponDura(DWORD MonsterDef, DWORD WeaponMinDamage, DWORD WeaponMaxDamage);
//extern float CalcArmorDura(DWORD MonMinAttackPower, DWORD MonMaxAttackPower, DWORD ItemDef);
// ������ �� ������ ���� ����
//extern DWORD CalcPriceForDura(DWORD ItemPrice, DURATYPE ItemDura);


// ���� ��ġ
// ��þƮ + ��ũ
// ����, ���� ���� ���� ��ġ
extern WORD CalcLimitStatWeapon(WORD ItemLimitStr, BYTE Enchant, eRANK Rank, LEVELTYPE ItemLV, float fLimitRatio);
extern WORD CalcLimitStatArmor(WORD ItemLimitStr, BYTE Enchant, eRANK Rank, LEVELTYPE ItemLV, float fLimitRatio);

// �Ǽ����� ���� ���Ѽ�ġ
extern WORD CalcLimitLVAccessory(WORD ItemLimitEqLevel, eRANK Rank);

//// UNUSED: Divine Item ��ġ
//// ��
//extern WORD CalcPhyDefForDivineArmor(WORD wPhyDef, LEVELTYPE level);

// ������
extern DWORD CalcDecreaseItemDuraValue(DWORD decreaseItemDuraRatio);

}; //end of namespace 'ns_formula'

#endif //PROGRAMCOMMON_ITEMFORMULA_H