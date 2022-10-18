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
// 스크립트 상수들!
//
namespace ns_formula {
;

// 아이템 가격 관련
extern float GetPriceWeightForEnchant(const uint8_t enchant);
extern float GetPriceWeightForRank(eRANK);
// 인챈트시 소모비용 책정용
extern float GetPriceWeightForEnchantFare(const uint8_t enchant);
extern float GetPriceWeightForRankFare(eRANK);
// 2006.4.22 : 태현변경요청
extern int GetLimitValueForRank(BYTE);
//---------------------------------------------------------------------------------------------------------
// 인첸트 관련

extern int GetWeaponEnchantWeightForEnchant(const uint8_t enchant);
extern int GetWeaponEnchantWeightForLV(LEVELTYPE);
extern int GetArmorEnchantWeightForEnchant(const uint8_t enchant);
extern int GetArmorEnchantWeightForLV(LEVELTYPE);
//---------------------------------------------------------------------------------------------------------
// 내구도 관련 : 인첸트 누적 내구도
extern uint8_t GetDuraEnchantWeightForEnchant(const uint8_t enchant);
//---------------------------------------------------------------------------------------------------------
// 인첸트, 랭크 제한수치 관련
//////////////////////////////////////////////////////////////////////////
// 무기 아이템의 제한수치
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 방어구 아이템의 제한수치
//////////////////////////////////////////////////////////////////////////
//_NA_0_20081015_EXP_LEVEL_RATIO
extern float GetExpLevelRatioChina(int);
extern float GetExpLevelRatioKorea(int);
// Exp 조정%값
extern float GetExpLevelRatio(int);
// 파티원 평균레벨에 대한 내레벨 비중
extern float GetPartyLevelExpRatio(int);
// Exp 추가%값(필드)
extern float GetFieldExpPartyBonusFactor(DWORD);
// Exp 추가%값(방)
extern float GetRoomExpPartyBonusFactor(DWORD);
// CHANGES: f110414.4L, apply HEIM weight value about heim based on number of party members.
// input: number of party members
extern float GetRoomHeimPartyBonusFactor(int);
extern float GetRoomHeimPartyBonusFactorType2(int);
// 헌팅 옵션%값
extern float GetRoomBonusTypeRatio(BYTE);
// 파티원 수에 따른 추가 공격력
extern float GetBonusAttackPower(int);
// 파티원 수에 따른 추가 공격력 비율값
extern float GetBonusAttPowerRatio(int);
// 상태적용 확률계산 시 몬스터등급에 따른 상수
extern float GetStatusRatioAsNPCGrade(int);
// 상태에 따른 캐릭터 기본이동속도
extern float GetBaseMoveSpeedAsState(int);
// 상태에 따른 캐릭터 이동시간(특수이동 : 캐릭터 애니에따라 시간이 다름)
extern WORD GetMoveTimeAsState(int);

}; //end of namespace 'ns_formula'
//==================================================================================================

#endif //PROGRAMCOMMON_NUMERIC_VALUES_H