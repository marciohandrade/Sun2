#pragma once
#ifndef PROGRAMCOMMON_NUMERIC_VALUES_H
#define PROGRAMCOMMON_NUMERIC_VALUES_H

#include "ItemOptionHeader.h"

//==================================================================================================
// @history
//  - ~10.12.20
//  - 10.12.20, waverix, moved macro logic in header to code file to support multi-version
//                       remove unused section
//
//==================================================================================================

//==================================================================================================
//
// ��ũ��Ʈ �����!
//
namespace ns_formula {
;

// ������ ���� ����
extern float GetPriceWeightForEnchant(const uint8_t enchant);
extern float GetPriceWeightForRank(eRANK);
// ��æƮ�� �Ҹ��� å����
extern float GetPriceWeightForEnchantFare(const uint8_t enchant);
extern float GetPriceWeightForRankFare(eRANK);
// 2006.4.22 : ���������û
extern int GetLimitValueForRank(BYTE);
//---------------------------------------------------------------------------------------------------------
// ��þƮ ����

extern int GetWeaponEnchantWeightForEnchant(const uint8_t enchant);
extern int GetWeaponEnchantWeightForLV(LEVELTYPE);
extern int GetArmorEnchantWeightForEnchant(const uint8_t enchant);
extern int GetArmorEnchantWeightForLV(LEVELTYPE);
//---------------------------------------------------------------------------------------------------------
// ������ ���� : ��þƮ ���� ������
extern uint8_t GetDuraEnchantWeightForEnchant(const uint8_t enchant);
//---------------------------------------------------------------------------------------------------------
// ��þƮ, ��ũ ���Ѽ�ġ ����
//////////////////////////////////////////////////////////////////////////
// ���� �������� ���Ѽ�ġ
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// �� �������� ���Ѽ�ġ
//////////////////////////////////////////////////////////////////////////
//_NA_0_20081015_EXP_LEVEL_RATIO
extern float GetExpLevelRatioChina(int);
extern float GetExpLevelRatioKorea(int);
// Exp ����%��
extern float GetExpLevelRatio(int);
// ��Ƽ�� ��շ����� ���� ������ ����
extern float GetPartyLevelExpRatio(int);
// Exp �߰�%��(�ʵ�)
extern float GetFieldExpPartyBonusFactor(DWORD);
// Exp �߰�%��(��)
extern float GetRoomExpPartyBonusFactor(DWORD);
// CHANGES: f110414.4L, apply HEIM weight value about heim based on number of party members.
// input: number of party members
extern float GetRoomHeimPartyBonusFactor(int);
extern float GetRoomHeimPartyBonusFactorType2(int);
// ���� �ɼ�%��
extern float GetRoomBonusTypeRatio(BYTE);
// ��Ƽ�� ���� ���� �߰� ���ݷ�
extern float GetBonusAttackPower(int);
// ��Ƽ�� ���� ���� �߰� ���ݷ� ������
extern float GetBonusAttPowerRatio(int);
// �������� Ȯ����� �� ���͵�޿� ���� ���
extern float GetStatusRatioAsNPCGrade(int);
// ���¿� ���� ĳ���� �⺻�̵��ӵ�
extern float GetBaseMoveSpeedAsState(int);
// ���¿� ���� ĳ���� �̵��ð�(Ư���̵� : ĳ���� �ִϿ����� �ð��� �ٸ�)
extern WORD GetMoveTimeAsState(int);

}; //end of namespace 'ns_formula'
//==================================================================================================

#endif //PROGRAMCOMMON_NUMERIC_VALUES_H