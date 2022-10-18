#include "stdafx.h"
#include "./NumericValues.h"

//==================================================================================================
// @history
//  - ~10.12.20
//  - 10.12.20, waverix, moved macro logic in header to code file to support multi-version
//                       fixed invalid output string meta formats
//  - 11.02.07, waverix, added logging facility
//
//==================================================================================================

// DONE: do add logger for quantitative analysis
#ifdef _SERVER
namespace ns_formula {
;

static void FormulaLogger(const ulong error_index,
                          const char* method, const int line_no,
                          const char* meta_format, int param);

enum {
    eCounter_Begin = __COUNTER__,
};

#define COUNTER_INCREMENT_INDEX()   (__COUNTER__ - eCounter_Begin)

#define FORMULA_ERROR_LOG(meta_format, arg) \
    FormulaLogger(COUNTER_INCREMENT_INDEX(), __FUNCTION__, __LINE__, meta_format, arg)

}; //end of namespace
//==================================================================================================
#else
//==================================================================================================
#define FORMULA_ERROR_LOG(meta_format, arg)

#endif

//==================================================================================================
namespace ns_formula {
;

// 아이템 가격 관련
float ns_formula::GetPriceWeightForEnchant(const uint8_t enchant)
{
    switch (enchant)
    {
    case 0: return 1.0f;
    case 1: return 1.5f;
    case 2: return 2.25f;
    case 3: return 3.38f;
    case 4: return 5.06f;
    case 5: return 7.59f;
    case 6: return 11.39f;
    case 7: return 17.09f;
    case 8: return 25.63f;
    case 9: return 38.44f;
    case 10: return 57.67f;
    case 11: return 86.50f;
    case 12: return 129.75f;
    case 13: return 194.63f;
    case 14: return 251.45f;
    case 15: return 337.18f;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", enchant);
    return 0.3f;
}

float ns_formula::GetPriceWeightForRank(eRANK param)
{
    switch (param)
    {
    case RANK_E:    return 1.0f;
    case RANK_D:    return 1.7f;
    case RANK_C:    return 2.89f;
    case RANK_B:    return 4.91f;
    case RANK_MA:   return 8.35f;
    case RANK_A:    return 14.20f;
    case RANK_PA:   return 24.14f;
    case RANK_MS:   return 41.03f;
    case RANK_S:    return 69.76f;
    case RANK_PS:   return 118.59f;
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 1.0f;
}

// 인챈트시 소모비용 책정용
float ns_formula::GetPriceWeightForEnchantFare(const uint8_t enchant)
{
    switch (enchant)
    {
    case 0: return 1.0f;
    case 1: return 1.4f;
    case 2: return 1.96f;
    case 3: return 2.74f;
    case 4: return 3.84f;
    case 5: return 5.38f;
    case 6: return 7.53f;
    case 7: return 10.54f;
    case 8: return 14.76f;
    case 9: return 20.66f;
    case 10: return 28.93f;
    case 11: return 40.50f;
    case 12: return 56.69f;
    case 13: return 56.69f;
    case 14: return 56.69f;
    case 15: return 56.69f;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", enchant);
    return 0.3f;
}

float ns_formula::GetPriceWeightForRankFare(eRANK param)
{
    switch (param)
    {
    case RANK_E:    return 1.0f;
    case RANK_D:    return 1.5f;
    case RANK_C:    return 2.0f;
    case RANK_B:    return 2.5f;
    case RANK_MA:   return 3.0f;
    case RANK_A:    return 3.5f;
    case RANK_PA:   return 4.0f;
    case RANK_MS:   return 4.5f;
    case RANK_S:    return 5.0f;
    case RANK_PS:   return 6.0f;
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 1.0f;
}

// 2006.4.22 : 태현변경요청
int ns_formula::GetLimitValueForRank(BYTE param)
{
    switch (param)
    {
    case RANK_E:  return 0;
    case RANK_D:  return 0;
    case RANK_C:  return 0;
    case RANK_B:  return 0;
    case RANK_MA: return 0;
    case RANK_A:  return 0;
    case RANK_PA: return 0;
    case RANK_MS: return 0;
    case RANK_S:  return 0;
    case RANK_PS: return 0;
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 0;
}

//---------------------------------------------------------------------------------------------------------
// 인첸트 관련

int ns_formula::GetWeaponEnchantWeightForEnchant(const uint8_t enchant)
{
    switch (enchant)
    {
    case 0: return 0;
    case 1: return 0;
    case 2: return 0;
    case 3: return 1;
    case 4: return 2;
    case 5: return 3;
    case 6: return 5;
    case 7: return 7;
    case 8: return 10;
    case 9: return 13;
    case 10: return 18;
    case 11: return 24;
    case 12: return 31;
    case 13: return 43;
    case 14: return 60;
    case 15: return 82;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", enchant);
    return 0;
}

int ns_formula::GetWeaponEnchantWeightForLV(LEVELTYPE param)
{    
    // changed rules by _NA_002128_20110228_MODIFY_ITEM_LEVEL
    if (param < ItemCustomLevel::kMinLV || param > ItemCustomLevel::kMaxLV)
    {
        FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
        enum { kDefaultWeight = 2 };
        return kDefaultWeight;
    }
    static const int weight_map[ItemCustomLevel::kMaxLV] = 
        /*1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39*/
        { 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 };
    return weight_map[param-1];
}

int ns_formula::GetArmorEnchantWeightForEnchant(const uint8_t enchant)
{
    switch (enchant)
    {
    case 0: return 0;
    case 1: return 0;
    case 2: return 0;
    case 3: return 1;
    case 4: return 2;
    case 5: return 3;
    case 6: return 5;
    case 7: return 7;
    case 8: return 10;
    case 9: return 13;
    case 10: return 18;
    case 11: return 24;
    case 12: return 31;
    case 13: return 43;
    case 14: return 60;
    case 15: return 82;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", enchant);
    return 0;
}

int ns_formula::GetArmorEnchantWeightForLV(LEVELTYPE param)
{
    // changed rules by _NA_002128_20110228_MODIFY_ITEM_LEVEL
    if (param < ItemCustomLevel::kMinLV || param > ItemCustomLevel::kMaxLV)
    {
        FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
        enum { kDefaultWeight = 1 };
        return kDefaultWeight;
    }
    static const int weight_map[ItemCustomLevel::kMaxLV] = 
        /*1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31 32 33 34 35 36 37 38 39*/
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    return weight_map[param-1];
}

//---------------------------------------------------------------------------------------------------------
// 내구도 관련 : 인첸트 누적 내구도
uint8_t ns_formula::GetDuraEnchantWeightForEnchant(const uint8_t enchant)
{
    switch (enchant)
    {
    case 0: return 0;
    case 1: return 0;
    case 2: return 0;
    case 3: return 1;
    case 4: return 2;
    case 5: return 3;
    case 6: return 5;
    case 7: return 7;
    case 8: return 9;
    case 9: return 12;
    case 10: return 15;
    case 11: return 18;
    case 12: return 22;
    case 13: return 22;
    case 14: return 22;
    case 15: return 22;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", enchant);
    return 0;
}

//---------------------------------------------------------------------------------------------------------
// 인첸트, 랭크 제한수치 관련
//////////////////////////////////////////////////////////////////////////
// 무기 아이템의 제한수치
//////////////////////////////////////////////////////////////////////////
//int GetLimitStrForWeapon(eCHAR_TYPE param, eITEMTYPE type)
//int GetLimitDexForWeapon(eCHAR_TYPE param, eITEMTYPE type)
//int GetLimitIntForWeapon(eCHAR_TYPE param, eITEMTYPE type)


//////////////////////////////////////////////////////////////////////////
// 방어구 아이템의 제한수치
//////////////////////////////////////////////////////////////////////////

//_NA_0_20081015_EXP_LEVEL_RATIO
float ns_formula::GetExpLevelRatioKorea(int param)
{
    switch (param)
    {
    case 30:        return 0.01f;
    case 29:        return 0.01f;
    case 28:        return 0.01f;
    case 27:        return 0.12f;
    case 26:        return 0.12f;
    case 25:        return 0.12f;
    case 24:        return 0.23f;
    case 23:        return 0.23f;
    case 22:        return 0.23f;
    case 21:        return 0.34f;
    case 20:        return 0.34f;
    case 19:        return 0.34f;
    case 18:        return 0.45f;
    case 17:        return 0.45f;
    case 16:        return 0.45f;
    case 15:        return 0.56f;
    case 14:        return 0.56f;
    case 13:        return 0.56f;
    case 12:        return 0.67f;
    case 11:        return 0.67f;
    case 10:        return 0.67f;
    case 9:         return 0.78f;
    case 8:         return 0.78f;
    case 7:         return 0.78f;
    case 6:         return 0.89f;
    case 5:         return 0.89f;
    case 4:         return 0.89f;
    case 3:         return 1.00f;
    case 2:         return 1.00f;
    case 1:         return 1.00f;
    case 0:         return 1.00f;
    case -1:        return 1.00f;
    case -2:        return 1.00f;
    case -3:        return 1.00f;
    case -4:        return 0.89f;
    case -5:        return 0.89f;
    case -6:        return 0.89f;
    case -7:        return 0.79f;
    case -8:        return 0.79f;
    case -9:        return 0.79f;
    case -10:       return 0.68f;
    case -11:       return 0.68f;
    case -12:       return 0.68f;
    case -13:       return 0.57f;
    case -14:       return 0.57f;
    case -15:       return 0.57f;
    case -16:       return 0.47f;
    case -17:       return 0.47f;
    case -18:       return 0.47f;
    case -19:       return 0.36f;
    case -20:       return 0.36f;
    case -21:       return 0.36f;
    case -22:       return 0.25f;
    case -23:       return 0.25f;
    case -24:       return 0.25f;
    case -25:       return 0.14f;
    case -26:       return 0.14f;
    case -27:       return 0.14f;
    case -28:       return 0.04f;
    case -29:       return 0.04f;
    case -30:       return 0.04f;
    };

    return 0.00f;
}

//_NA_0_20081015_EXP_LEVEL_RATIO
float ns_formula::GetExpLevelRatioChina(int param)
{
    if (param >= 20 || param <= -20)
        return 0.01f;

    switch (param)
    {
    case 30:        return 0.01f;
    case 29:        return 0.01f;
    case 28:        return 0.01f;
    case 27:        return 0.12f;
    case 26:        return 0.12f;
    case 25:        return 0.12f;
    case 24:        return 0.23f;
    case 23:        return 0.23f;
    case 22:        return 0.23f;
    case 21:        return 0.34f;
    case 20:        return 0.34f;
    case 19:        return 0.34f;
    case 18:        return 0.45f;
    case 17:        return 0.45f;
    case 16:        return 0.45f;
    case 15:        return 0.56f;
    case 14:        return 0.56f;
    case 13:        return 0.56f;
    case 12:        return 0.67f;
    case 11:        return 0.67f;
    case 10:        return 0.67f;
    case 9:         return 0.78f;
    case 8:         return 0.78f;
    case 7:         return 0.78f;
    case 6:         return 0.89f;
    case 5:         return 0.89f;
    case 4:         return 0.89f;
    case 3:         return 1.00f;
    case 2:         return 1.00f;
    case 1:         return 1.00f;
    case 0:         return 1.00f;
    case -1:        return 1.00f;
    case -2:        return 1.00f;
    case -3:        return 1.00f;
    case -4:        return 0.89f;
    case -5:        return 0.89f;
    case -6:        return 0.89f;
    case -7:        return 0.79f;
    case -8:        return 0.79f;
    case -9:        return 0.79f;
    case -10:       return 0.68f;
    case -11:       return 0.68f;
    case -12:       return 0.68f;
    case -13:       return 0.57f;
    case -14:       return 0.57f;
    case -15:       return 0.57f;
    case -16:       return 0.47f;
    case -17:       return 0.47f;
    case -18:       return 0.47f;
    case -19:       return 0.36f;
    case -20:       return 0.36f;
    case -21:       return 0.36f;
    case -22:       return 0.25f;
    case -23:       return 0.25f;
    case -24:       return 0.25f;
    case -25:       return 0.14f;
    case -26:       return 0.14f;
    case -27:       return 0.14f;
    case -28:       return 0.04f;
    case -29:       return 0.04f;
    case -30:       return 0.04f;
    };

    return 0.00f;
}

// Exp 조정%값
float ns_formula::GetExpLevelRatio(int param)
{
    switch (param)
    {
    case 1:     return 1.0f;
    case 2:     return 1.0f;
    case 3:     return 0.95f;
    case 4:     return 0.95f;
    case 5:     return 0.9f;
    case 6:     return 0.9f;
    case 7:     return 0.85f;
    case 8:     return 0.85f;
    case 9:     return 0.8f;
    case 10:    return 0.8f;
    case 11:    return 0.75f;
    case 12:    return 0.75f;
    case 13:    return 0.7f;
    case 14:    return 0.7f;
    case 15:    return 0.6f;
    case 16:    return 0.5f;
    case 17:    return 0.4f;
    case 18:    return 0.3f;
    case 19:    return 0.2f;
    };

    if (param <= 0) return 1.0f;
    else            return 0.1f;
}

// 파티원 평균레벨에 대한 내레벨 비중
float ns_formula::GetPartyLevelExpRatio(int param)
{
    switch (param)
    {
    case 30:        return 1.5f;
    case 29:        return 1.5f;
    case 28:        return 1.5f;
    case 27:        return 1.45f;
    case 26:        return 1.45f;
    case 25:        return 1.45f;
    case 24:        return 1.4f;
    case 23:        return 1.4f;
    case 22:        return 1.4f;
    case 21:        return 1.35f;
    case 20:        return 1.35f;
    case 19:        return 1.35f;
    case 18:        return 1.3f;
    case 17:        return 1.3f;
    case 16:        return 1.3f;
    case 15:        return 1.25f;
    case 14:        return 1.25f;
    case 13:        return 1.25f;
    case 12:        return 1.2f;
    case 11:        return 1.2f;
    case 10:        return 1.2f;
    case 9:         return 1.15f;
    case 8:         return 1.15f;
    case 7:         return 1.15f;
    case 6:         return 1.1f;
    case 5:         return 1.1f;
    case 4:         return 1.1f;
    case 3:         return 1.05f;
    case 2:         return 1.05f;
    case 1:         return 1.05f;
    case 0:         return 1.0f;
    case -1:        return 0.95f;
    case -2:        return 0.95f;
    case -3:        return 0.95f;
    case -4:        return 0.9f;
    case -5:        return 0.9f;
    case -6:        return 0.9f;
    case -7:        return 0.85f;
    case -8:        return 0.85f;
    case -9:        return 0.85f;
    case -10:       return 0.8f;
    case -11:       return 0.8f;
    case -12:       return 0.8f;
    case -13:       return 0.75f;
    case -14:       return 0.75f;
    case -15:       return 0.75f;
    case -16:       return 0.7f;
    case -17:       return 0.7f;
    case -18:       return 0.7f;
    case -19:       return 0.65f;
    case -20:       return 0.65f;
    case -21:       return 0.65f;
    case -22:       return 0.6f;
    case -23:       return 0.6f;
    case -24:       return 0.6f;
    case -25:       return 0.55f;
    case -26:       return 0.55f;
    case -27:       return 0.55f;
    case -28:       return 0.5f;
    case -29:       return 0.5f;
    case -30:       return 0.5f;
    };

    if (param >= 30)    return 1.5f;
    else                return 0.5f;
}

// Exp 추가%값(필드)
float ns_formula::GetFieldExpPartyBonusFactor(DWORD num_party_members)
{
#ifdef _NA_003481_20111026_EXP_PARTYBONUS_CHANGE
    // 파티 인원수에 따른 경험치 증가율 변경 - arycoat 2011.10.26
    switch (num_party_members)
    {
        case 1:     return 1.00f;
        case 2:     return 1.15f;
        case 3:     return 1.31f;
        case 4:     return 1.48f;
        case 5:     return 1.66f;
        case 6:     return 1.85f;
        case 7:     return 2.05f;
        case 8:     return 2.26f;
        case 9:     return 2.48f;
        case 10:    return 2.71f;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", num_party_members);
    return 1.00f;
#else
#if defined(_CHINA)
    switch (num_party_members)
    {
    case 1:     return 1.8f;
    case 2:     return 3.024f;
    case 3:     return 4.236f;
    case 4:     return 5.928f;
    case 5:     return 8.304f;
    };
    return 8.304f;
#elif defined(_JAPAN)
    switch (num_party_members)
    {
    case 1:     return 1.00f;
    case 2:     return 1.15f;
    case 3:     return 1.30f;
    case 4:     return 1.45f;
    case 5:     return 1.60f;
    case 6:     return 1.75f;
    case 7:     return 1.90f;
    case 8:     return 2.05f;
    case 9:     return 2.20f;
    case 10:    return 2.35f;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", num_party_members);
    return 1.00f;
#else
    switch (num_party_members)
    {
    case 1:     return 1.00f;
    case 2:     return 1.18f;
    case 3:     return 1.36f;
    case 4:     return 1.54f;
    case 5:     return 1.72f;
    case 6:     return 1.90f;
    case 7:     return 2.08f;
    case 8:     return 2.26f;
    case 9:     return 2.44f;
    case 10:    return 2.62f;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", num_party_members);
    return 1.00f;
#endif
#endif //_NA_003481_20111026_EXP_PARTYBONUS_CHANGE
}

// Exp 추가%값(방)
float ns_formula::GetRoomExpPartyBonusFactor(DWORD num_party_members)
{
#ifdef _NA_003481_20111026_EXP_PARTYBONUS_CHANGE
    // 파티 인원수에 따른 경험치 증가율 변경 - arycoat 2011.10.26
    switch (num_party_members)
    {
        case 1:     return 2.05f;
        case 2:     return 2.15f;
        case 3:     return 2.26f;
        case 4:     return 2.38f;
        case 5:     return 2.60f;
        case 6:     return 2.83f;
        case 7:     return 3.07f;
        case 8:     return 3.32f;
        case 9:     return 3.67f;
        case 10:    return 4.03f;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", num_party_members);
    return 2.05f;
#else 
#if defined(_CHINA)
    switch (num_party_members)
    {
    case 1:     return 1.0f;
    case 2:     return 1.35f;
    case 3:     return 1.6f;
    case 4:     return 1.75f;
    case 5:     return 1.85f;
    case 6:     return 1.85f;
    case 7:     return 1.85f;
    case 8:     return 1.85f;
    case 9:     return 1.85f;
    case 10:    return 1.85f;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", num_party_members);
    return 1.00f;
#else
    // changed rules by _NA_0_20110103_EP2_BATTLEZONE_EXP_PARTY_BONUS_ADJUSTED
    switch (num_party_members)
    {
    case 1:     return 2.05f;
    case 2:     return 2.12f;
    case 3:     return 2.20f;
    case 4:     return 2.29f;
    case 5:     return 2.39f;
    case 6:     return 2.50f;
    case 7:     return 2.62f;
    case 8:     return 2.75f;
    case 9:     return 2.89f;
    case 10:    return 3.05f;
    };
    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", num_party_members);
    return 2.05f;
#endif
#endif //_NA_003481_20111026_EXP_PARTYBONUS_CHANGE
}

// CHANGES: f110414.4L, apply HEIM weight value about heim based on number of party members.
// input: number of party members
float ns_formula::GetRoomHeimPartyBonusFactor(int param)
{
    BOOST_STATIC_ASSERT(MAX_PARTYMEMBER_NUM == 10);

#ifdef _NA_0_20120102_CHANGE_PARTY_HEIM_WEIGHT
    float weight = 0.3f;
    if ((param >= 1) && (param <= MAX_PARTYMEMBER_NUM))
        weight *= param;
    else
        FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return weight;
#endif

    switch (param)
    {
    case 1:
        return 1.0f;
    case 2:
        return 1.1f;
    case 3:
        return 1.2f;
    case 4:
        return 1.3f;
    case 5:
        return 1.5f;
    case 6:
        return 1.7f;
    case 7:
        return 1.9f;
    case 8:
        return 2.2f;
    case 9:
        return 2.5f;
    case 10:
        return 3.0f;
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 1.0f;
}

float ns_formula::GetRoomHeimPartyBonusFactorType2(int param)
{
    switch (param)
    {
    case 1:
        return 1.0f;
    case 2:
        return 1.1f;
    case 3:
        return 1.2f;
    case 4:
        return 1.3f;
    case 5:
        return 1.5f;
    case 6:
        return 1.7f;
    case 7:
        return 1.9f;
    case 8:
        return 2.2f;
    case 9:
        return 2.5f;
    case 10:
        return 3.0f;
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 1.0f;
}

// 헌팅 옵션%값
float ns_formula::GetRoomBonusTypeRatio(BYTE param)
{
    switch (param)
    {
    case 0:     return 1.0f;        // 아이템 우선
    case 1:     return 1.1f;        // 경험치 우선
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 1.0f;
}

// 파티원 수에 따른 추가 공격력
float ns_formula::GetBonusAttackPower(int param)
{
    switch (param)
    {
    case 1:     return 0;
    case 2:     return 1.6f; // 2.0f -> 1.6f
    case 3:     return 1.6f; // 2.0f -> 1.6f
    case 4:     return 1.6f; // 2.0f -> 1.6f
    case 5:     return 1.6f; // 2.0f -> 1.6f
    case 6:     return 1.6f; // 2.0f -> 1.6f
    case 7:     return 1.6f; // 2.0f -> 1.6f
    case 8:     return 1.6f; // 2.0f -> 1.6f
    case 9:     return 1.6f; // 2.0f -> 1.6f
    case 10:    return 1.6f; // 2.0f -> 1.6f
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 0.0f;
}

// 파티원 수에 따른 추가 공격력 비율값
float ns_formula::GetBonusAttPowerRatio(int param)
{
    switch (param)
    {
    case 1:     return 0.044f; // 0.055f -> 0.044f
    case 2:     return 0.048f; // 0.06f -> 0.048f
    case 3:     return 0.052f; // 0.065f -> 0.052f
    case 4:     return 0.056f; // 0.07f -> 0.056f
    case 5:     return 0.06f;  // 0.075f -> 0.06f
    case 6:     return 0.064f; // 0.08f -> 0.064f
    case 7:     return 0.068f; // 0.085f -> 0.068f
    case 8:     return 0.072f; // 0.09f -> 0.072f
    case 9:     return 0.076f; // 0.095f -> 0.076f
    case 10:    return 0.08f; // 0.01f -> 0.08f
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 0.044f; // 0.055f -> 0.044f
}

// 상태적용 확률계산 시 몬스터등급에 따른 상수
float ns_formula::GetStatusRatioAsNPCGrade(int param)
{
    switch (param)
    {
    case eNPC_GENERAL:          return 1.0f;
    case eNPC_ELITE:            return 0.8f;
    case eNPC_LEADER:           return 1.0f;
    case eNPC_MIDDLEBOSS:       return 0.6f;
    case eNPC_BOSS:             return 0.4f;
    case eNPC_MERCHANT_NPC:     return 0.1f;
    case eNPC_SUMMON_NPC:       return 0.1f;
    case eNPC_MAPOBJECT_NPC:    return 0.1f;
    case eNPC_CRYSTAL_WARP:     return 0.1f;
    case eNPC_DOMINATION_MAPOBJECT_NPC: return 0.1f;
    case eNPC_LUCKY_MONSTER:    return 0;
    // NOTE: f110719.4L, added an unmanaged default value;
    case eNPC_TOTEM:			return 1.0f;
    case eNPC_SSQ_MONSTER:		return 1.0f;
    case eNPC_FRIEND_MAPOBJECT_NPC: return 1.0f;
    case eNPC_FRIEND_MONSTER:   return 1.0f;
    case eNPC_MOVE_SYNC_MERCHANT:   return 1.0f;
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 1.0f;
}

// 상태에 따른 캐릭터 기본이동속도
float ns_formula::GetBaseMoveSpeedAsState(int param)
{
    switch (param)
    {
    case CMS_WALK:              return 1.2f * SPEED_MULTIPLIER;
    case CMS_RUN:               return 5.0f * SPEED_MULTIPLIER;
    case CMS_SWIPE:             return 10.0f * SPEED_MULTIPLIER;
    case CMS_KNOCKBACK:         return 6.0f * SPEED_MULTIPLIER;
    case CMS_KNOCKBACK2:        return 15.0f * SPEED_MULTIPLIER; //__NA001048_080514_APPEND_ABILITY_KNOCKBACK2__
    case CMS_KNOCKBACK_DOWN:    return 12.0f * SPEED_MULTIPLIER;
    case CMS_SIDESTEP:          return 4.0f * SPEED_MULTIPLIER;
    case CMS_BACKSTEP:          return 3.0f * SPEED_MULTIPLIER;
    case CMS_MONSTER_ATTACKJUMP:return 9.0f * SPEED_MULTIPLIER;
    case CMS_LUCKY_MONSTER_RUNAWAY: return 15.0f * SPEED_MULTIPLIER;
    case CMS_TUMBLING_FRONT:    return 10.0f * SPEED_MULTIPLIER;
    case CMS_TUMBLING_LEFT:     return 10.0f * SPEED_MULTIPLIER;
    case CMS_TUMBLING_RIGHT:    return 10.0f * SPEED_MULTIPLIER;
    case CMS_TUMBLING_BACK:     return 10.0f * SPEED_MULTIPLIER;
    case CMS_SHOULDER_CHARGE:   return 10.0f * SPEED_MULTIPLIER;
    case CMS_SLIDING:           return 10.0f * SPEED_MULTIPLIER;
    case CMS_TELEPORT:          return 10.0f * SPEED_MULTIPLIER;
    case CMS_STOP:              return 0;
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 5.0f * SPEED_MULTIPLIER;
}

// 상태에 따른 캐릭터 이동시간(특수이동 : 캐릭터 애니에따라 시간이 다름)
WORD ns_formula::GetMoveTimeAsState(int param)
{
    switch (param)
    {
    case CMS_TUMBLING_FRONT:    return 300;
    case CMS_TUMBLING_LEFT:     return 300;
    case CMS_TUMBLING_RIGHT:    return 300;
    case CMS_SHOULDER_CHARGE:   return 300;
    case CMS_SLIDING:           return 300;
    case CMS_TELEPORT:          return 300;
    case CMS_TUMBLING_BACK:     return 300;
    };

    FORMULA_ERROR_LOG("|["__FUNCTION__"]|[%u]", param);
    return 433;
}


//==================================================================================================
//==================================================================================================
// WARNING: the below code block must be located last position

#ifdef _SERVER
enum {
    eCounter_Counts = COUNTER_INCREMENT_INDEX() + 1,
};

struct LocalModuleCounterField
{
    ulong milestone;
    ulong counter;
};

static LocalModuleCounterField logging_counters[eCounter_Counts] = { 0, };

static void FormulaLogger(const ulong error_index,
                          const char* method, const int line_no,
                          const char* meta_format, int param)
{
    if (_countof(logging_counters) <= error_index) {
        ASSERT(!"Index out of ranges");
        return;
    }
    LocalModuleCounterField* const counter_record = &logging_counters[error_index];
    if (counter_record->milestone == 0) {
        counter_record->milestone = (1 << 0);
    };

    ++counter_record->counter;
    if (counter_record->milestone > counter_record->counter) {
        return;
    }

    counter_record->milestone <<= 1;
    //
    char buffer[512];
    _snprintf(buffer, _countof(buffer), meta_format, param);
    buffer[_countof(buffer) - 1] = '\0';
    //
    SUNLOG(eCRITICAL_LOG, "|MilestoneError(%8u) '%s' --- [%u] in '%s'|",
           counter_record->counter, buffer, line_no, method);
}
#endif //

}; //end of namespace 'ns_formula'
//==================================================================================================
