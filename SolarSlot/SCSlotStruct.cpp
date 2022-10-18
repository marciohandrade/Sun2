#include "stdafx.h"
#include "./SCSlotStruct.h"

//==================================================================================================

eCHAR_TYPE
BASE_ITEMINFO::GetFirstAvailableClass() const
{
    if(m_wEqClass1)
        return eCHAR_BERSERKER;

    if(m_wEqClass2)
        return eCHAR_DRAGON;

    if(m_wEqClass3)
        return eCHAR_SHADOW;

    if(m_wEqClass4)
        return eCHAR_VALKYRIE;

    if(m_wEqClass5)
        return eCHAR_ELEMENTALIST;

    // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    if (m_wEqClass6)
        return eCHAR_MYSTIC;

    // _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if (m_wEqClass7)
        return eCHAR_HELLROID;

    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (m_wEqClass8)
        return eCHAR_WITCHBLADE;

    // 없으면 그냥 버서커
    return eCHAR_BERSERKER;
}


#if !defined(_SERVER) // Client Code
bool
BASE_ITEMINFO::CanRankUpItem() const
{
    //서버팀 사용하지 말것!
    if(!IsWeapon() && !IsArmor() && !IsAccessory())
        return false;

    if(IsChargeItem())
        return false;

    if(IsElite())
        return false;

    if(IsUnique())
        return false;

    return true;
}
#endif

//==================================================================================================
// NOTE: internal utilities
//
// NOTE: f110414.x, declared by hukim, 아이템 계열 상세 타입 (prototype:GetSubTypeDetail())
// CHANGES: f110414.1L, call this after loading default script data
void BASE_ITEMINFO::_UpdateSubTypeDetailedValue()
{
    sub_type_detailed.value = ItemSubTypeDetail::kInvalid;
    //
    if (m_wSubType == eITEMSUBTYPE_ELITE_SSQ) // 타락한 사원
    {
        switch (m_LV)
        {
        case ItemCustomLevel::kCurruptTempleLV:
            sub_type_detailed.value = ItemSubTypeDetail::kCurruptTemple;
            return;
		case ItemCustomLevel::kFighter_Elite2_0LV://투사의 엘리트 2차
			sub_type_detailed.value = ItemSubTypeDetail::kFigter_Elite2_0st;
			return;
		case ItemCustomLevel::kFighter_Elite3_0LV:
			sub_type_detailed.value = ItemSubTypeDetail::kFigter_Elite3_0st;
			return;
        default:
            return;
        }
    }
    else if (m_wSubType == eITEMSUBTYPE_ELITE) // 엘리트
    {
        switch (m_LV)
        {
        case ItemCustomLevel::kElite1_0stLV:
            sub_type_detailed.value = ItemSubTypeDetail::kElite1_0st;
            return;
        case ItemCustomLevel::kElite1_5stLV:
            sub_type_detailed.value = ItemSubTypeDetail::kElite1_5st;
            return;
        case ItemCustomLevel::kElite2_0stLV:
            sub_type_detailed.value = ItemSubTypeDetail::kElite2_0st;
            return;
		case ItemCustomLevel::kElite2_5stLV:
			sub_type_detailed.value = ItemSubTypeDetail::kElite2_5st;
			return;
		case ItemCustomLevel::kElite3_0stLV:
			sub_type_detailed.value = ItemSubTypeDetail::kElite3_0st;
			return;
        case ItemCustomLevel::kElite3_5stLV:
            sub_type_detailed.value = ItemSubTypeDetail::kElite3_5st;
            return;
		case ItemCustomLevel::kElite4_0stLV:
			sub_type_detailed.value = ItemSubTypeDetail::kElite4_0st;
			return;
        default:
            return;
        }
    }
    else if (IsUnique()) // 유니크
    {
        switch (m_LV)
        {
        case ItemCustomLevel::kUnique1_0stLV:
            sub_type_detailed.value = ItemSubTypeDetail::kUnique1_0st;
            return;
        case ItemCustomLevel::kUnique1_5stLV:
            sub_type_detailed.value = ItemSubTypeDetail::kUnique1_5st;
            return;
        case ItemCustomLevel::kUnique2_0stLV:
            sub_type_detailed.value = ItemSubTypeDetail::kUnique2_0st;
            return;
        case ItemCustomLevel::kUnique2_5stLV:
            sub_type_detailed.value = ItemSubTypeDetail::kUnique2_5st;
            return;
        default:
            return;
        }
    }
    else if (m_wSubType == eITEMSUBTYPE_FATE) // 럭키
    {
        sub_type_detailed.value = ItemSubTypeDetail::kFate;
        return;
    }
    // CHANGES: f110413.4L, added type about rare items
    else if (m_wSubType == eITEMSUBTYPE_RARE)
    {
        if (m_LV < ItemCustomLevel::kMinLV || m_LV > ItemCustomLevel::kGeneralMaxLV) {
            return;
        }
        sub_type_detailed.value = ItemSubTypeDetail::kRare;
        return;
    }
    else if (m_wSubType == eITEMSUBTYPE_GENERAL) // 일반
    {
        if (m_LV > ItemCustomLevel::kGeneralMaxLV) {
            return;
        }
        if (IsWeapon() || IsArmor())
        {
            if (m_LV == ItemCustomLevel::kSuperJapanLV) { // 일본 최강
                sub_type_detailed.value = ItemSubTypeDetail::kSuperJapan;
                return;
            }
        }
        sub_type_detailed.value = ItemSubTypeDetail::kGeneral;
        return;
    };
    //
    sub_type_detailed.value = ItemSubTypeDetail::kInvalid;
    //
}

#ifdef  _NA_000000_20130521_DAILY_QUEST_RESET_ITEM
bool BASE_ITEMINFO::IsDailyQeustResetItem() const
{
    return ((eITEMTYPE)m_wType == eITEMTYPE_DAILYQUEST_RESET);
}
#endif

#ifdef _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE
bool BASE_ITEMINFO::IsImmediatelyUseItem() const
{
    return (m_ChargeSubType == eCHARGE_SUB_IMMEDIATELY_USE
        || m_ChargeSubType == eCHARGE_SUB_IMMEDIATELY_USE_AND_DELETE);
}
#endif // _NA_008440_20150818_ADD_CHARGE_SUB_IMMEDIATELY_USE