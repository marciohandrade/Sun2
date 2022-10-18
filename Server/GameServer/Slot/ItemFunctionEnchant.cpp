#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "EnchantParser.h"
#include "BetaKeyImpl.h"

#include "InventorySlotContainer.h"
#include "SlotManager.h"

#include "ChaoState.h"
#include "AchievementManager.h"

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#include "GameZone.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

//==================================================================================================
namespace nsSlot {
;
//--------------------------------------------------------------------------------------------------
// @history
//  09/06/25|~~~
//  09/07/16|shogen|enchant facility uniformity|<- �����ϱ� �ϴ� �ɱ�?
//  09/10/22|namka199|rank facility improvement|(WARNING) ��þƮ �Ͽ�ȭ �۾��� �Ϻ� ������ ��.
//  10/01/28|waverix|code arrangement
//                  |delete macros  '__NA_676_PK_SYSTEM_IMPROVEMENT'
//                  |               '__NA_977_PREVENTION_PARTY_RETURN_ABUSE'
//                  |               '__NA_1339_ENCHANT_RENEWAL_EX'
//                  |               '__NA_1168_ADD_CONTENTS_ADD_LOG'
//                  |               '__BUG_35162_COMPOSE_BUG_BY_FULL_IVEN'
//                  |               '__CN_1194__ELITE_1_5_ITEM'
//                  |               '__MOD_CRYSTALIZE_MAX_CREATE_ITEM_NUM'
//  10/02/23|hukim|remove warnings level 4... etc...
//                |(waverix: i think this changes have fragmentary knowledge)
//  10/02/24|waverix|rollback to revision 46727|
//                  |������, �ٽ� �� �� ����� �غ��ñ���. ���� ��� �����ϴ� ���� ���� ������,
//                  warning level 4��� �༮�� ������ ���� ������ ������ ������ �ɷ� �����˴ϴ�.
//  11/06/11|waverix|show the below description about the changes
//--------------------------------------------------------------------------------------------------

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionEnchant) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================


// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT
nsSlot::ItemFunctionEnchant::ChkBlockEnchant() const
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_ENCHANT)) {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (!root_->player_->GetChaoState().CanUseFuction(CHAO_USE_ENCHANT)) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    if (root_->player_->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING) {
        return RC::RC_ITEM_INVALIDSTATEOFPLAYER;
    }
    if (root_->player_->GetLevel() < LIMIT_LEVEL_ENCHANT) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CURR_LEVEL;
    }
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT
nsSlot::ItemFunctionEnchant::ChkSlotEnchant(SCItemSlot& target_slot) const
{
    if (target_slot.CanUseMixture(MIXTURE_ENCHANT) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }
    if (target_slot.GetEnchant() >= MAX_ENCHANT_GRADE) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionEnchant::ChkSuccessRate(
    const uint8_t enchant, 
    const int enchant_grade,
    bool is_elite, 
    bool is_unique, 
    const eENCHANT_OPTION enchant_option
) const {
    __UNUSED(enchant);
    // ����Ʈ �������� 50%Ȯ���� ���õȴ�.
    if (is_elite) {
        return (enchant_option & eENCHANT_50PER) ?  RC::RC_ITEM_SUCCESS
                                                 :  RC::RC_ITEM_ENCHANT_INVALID_RATE_INDEX;
    }
    // ����ũ �������� 25%Ȯ���� ���õȴ�.
    if (is_unique) {
        return (enchant_option & eENCHANT_25PER) ?  RC::RC_ITEM_SUCCESS
                                                 :  RC::RC_ITEM_ENCHANT_INVALID_RATE_INDEX;
    }
    // �Ϲ� �������� ĳ�� ������ ��� Ȯ�� ��� ����.
    if (enchant_option & eENCHANT_CASH) {
        return RC::RC_ITEM_SUCCESS;
    }

    // �Ϲ� ������ ��
    switch (enchant_grade)
    {
    case kEnchantLow:
        // ������ ��æƮ�ΰ�� 100%Ȯ���� ���ð���.
        if ((enchant_option & eENCHANT_100PER) == 0) {
            return RC::RC_ITEM_ENCHANT_INVALID_RATE_INDEX;
        }
        break;
    case kEnchantMiddle:
        // �߷��� ��æƮ�ΰ�� 100%Ȯ�� ���úҰ�.
        if (enchant_option & eENCHANT_100PER) {
            return RC::RC_ITEM_ENCHANT_INVALID_RATE_INDEX;
        }
        break;
    case kEnchantHigh:
        // ���� ��æƮ�� ��� 75%, 100%Ȯ�� ���úҰ���.
        // (CHANGES) (100131) (WAVERIX) fix up invalid check condition that \
        // ratio selector have only one
        if ((enchant_option & eENCHANT_100PER) ||
            (enchant_option & eENCHANT_75PER))
        {
            return RC::RC_ITEM_ENCHANT_INVALID_RATE_INDEX;
        }
        break;
#ifdef _NA_003966_20111227_ADD_ENCHANT
    case kEnchantHighest:
        // �ֻ��� ��þƮ 75%, 100% ���� �Ұ�
        if ((enchant_option & eENCHANT_100PER) || (enchant_option & eENCHANT_75PER)) 
        {
            return RC::RC_ITEM_ENCHANT_INVALID_RATE_INDEX;
        }
        break;
#endif
    default:
        return RC::RC_ITEM_ENCHANT_INVALID_RATE_INDEX;
    }

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionEnchant::ChkEnchantCost(
    const MATERIALS_FOR_LEVEL* enchant_info,
    const MONEY enchant_fare, 
    const eENCHANT_OPTION enchant_option
) const {
    // ��þƮ ����� �ִ��� üũ
    if (root_->player_->GetMoney() < enchant_fare) {
        return RC::RC_ITEM_INSUFFICIENT_MONEY;
    }
    // �Ҹ��� ��ᰡ �ִ��� üũ
    const sFARE_RATIO* fare_ratio = enchant_info->GetFareRatio(enchant_option);
    if (FlowControl::FCAssert(fare_ratio != NULL) == false) {
        return RC::RC_ITEM_ENCHANT_INVALID_ITEMLEVEL;
    }
    const double material_fare = fare_ratio->m_MaterialFare;

    const EnchantParser* const enchant_parser = EnchantParser::Instance();
    for (int i = 0; i < MATERIALS_FOR_LEVEL::kMaxMaterialNumber; ++i)
    {
        const sMATERIALS* materials = enchant_info->GetMaterials(i);
        if (materials == NULL) {
            continue;
        }
        POSTYPE number_of_material_items = \
            enchant_parser->CalcMaterialNum(materials->m_MaterialItemNum, material_fare);

        ns_functor::EmptyItemCheck empty_checker(\
            materials->m_MaterialItemCode, number_of_material_items);
        if (root_->ForeachSlot(SI_INVENTORY, &empty_checker) == false) {
            return RC::RC_ITEM_ENCHANT_HAVENOTMETERIALS;
        }
    }

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT
nsSlot::ItemFunctionEnchant::DeleteEnchantCost(const MATERIALS_FOR_LEVEL* enchant_info,
                               MONEY enchant_fare, eENCHANT_OPTION enchant_option)
{   
    // ��� ���ֱ�                                assert(enchant_option) ?
    double material_fare = enchant_info->GetFareRatio(enchant_option)->m_MaterialFare;
    const EnchantParser* const enchant_parser = EnchantParser::Instance();
    for (POSTYPE i = 0; i < MATERIALS_FOR_LEVEL::kMaxMaterialNumber; ++i)
    {
        const sMATERIALS* materials = enchant_info->GetMaterials(i);
        if (materials == NULL) {
            continue;
        }
        POSTYPE number_of_material_items = \
            enchant_parser->CalcMaterialNum(materials->m_MaterialItemNum, material_fare);

        ns_functor::DeleteItem delete_operator(\
            materials->m_MaterialItemCode, number_of_material_items);
        BOOL foreach_result = root_->ForeachSlot(SI_INVENTORY, &delete_operator);
        ASSERT(foreach_result);
    }

    // ��� �Ҹ�
    BOOL minus_result = root_->player_->MinusMoney(enchant_fare);
    ASSERT(minus_result);

    GAMELOG->LogMoney(MONEY_ENCHANT, root_->player_, enchant_fare);
    g_pGameServer->NotifyHeimConsumption(root_->player_->GetCharGuid(),
                                         eHEIM_CONSUMPTION_ENCHANT,
                                         enchant_fare);

    return RC::RC_ITEM_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionEnchant::Enchant(
    const POSTYPE target_pos, const POSTYPE down_guard_item_pos, const eENCHANT_OPTION enchant_option)
{   
    const RC::eITEM_RESULT check_block_enchant = ChkBlockEnchant();
    if (RC::RC_ITEM_SUCCESS != check_block_enchant) 
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|��þƮ ����� ����� �� �����ϴ�.|ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            root_->player_->GetCharName()
        );
        return check_block_enchant;
    }
    if (root_->ValidPos(SI_INVENTORY, target_pos) == false) 
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|��þƮ ��� ������ ��ġ�� �ùٸ��� �ʽ��ϴ�.|ItemPosition = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            target_pos, 
            root_->player_->GetCharName()
        );
        return RC::RC_ITEM_INVALIDPOS;
    }

    SCSlotContainer* const inventory = root_->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& target_slot = static_cast<SCItemSlot&>(inventory->GetSlot(target_pos));    
    const RC::eITEM_RESULT check_slot_enchant = ChkSlotEnchant(target_slot);
    if (check_slot_enchant != RC::RC_ITEM_SUCCESS) 
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|��þƮ �Ҽ� ���� ������ �Դϴ�.|ItemPosition = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            target_pos, 
            root_->player_->GetCharName()
        );
        return check_slot_enchant;
    }

    const uint8_t enchant = target_slot.GetEnchant();
    const BASE_ITEMINFO* const target_item_info = target_slot.GetItemInfo();
    const bool is_elite  = target_item_info->IsElite();
    const bool is_unique = target_item_info->IsUnique();
    
    const EnchantParser* const enchant_parser = EnchantParser::Instance();
    const eENCHANT_GRADE enchant_grade = enchant_parser->GetEnchantGrade(enchant);
    const RC::eITEM_RESULT check_success_rate = 
        ChkSuccessRate(enchant, enchant_grade, is_elite, is_unique, enchant_option);
    if (check_success_rate != RC::RC_ITEM_SUCCESS) 
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|��þƮ Ȯ���� �ùٸ��� �ʽ��ϴ�.|ItemPosition = %d, EnchantGrade = %d, EnchantOption = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            target_pos, 
            enchant_grade, 
            enchant_option, 
            root_->player_->GetCharName()
        );
        return check_success_rate;
    }

    const LEVELTYPE item_level = target_item_info->m_LV;
    const MATERIALS_FOR_LEVEL* const enchant_info = enchant_parser->GetEnchantMatInfo(
        item_level, enchant, target_item_info->IsWeapon(), target_slot.IsLimited(), enchant_option);
    if (enchant_info == NULL) 
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|��þƮ ������ ã�� �� �����ϴ�.|ItemPosition = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            target_pos, 
            root_->player_->GetCharName()
        );
        return RC::RC_ITEM_ENCHANT_INVALID_ITEMLEVEL;
    }
    
    // ��æƮ ���� ���
    // NOTE: f080903.x, lst1024, Ŭ����� ��갪�� 1 ������ �߻� �ڷ����� float���� �ٲٰ� (+ 0.5f + 0.001f) �� ����
    // CHANGES: f100130.x, waverix, changes to more generic code using high-level rounding
    MONEY enchant_fare = 0;
    //{
    //    double formula_calc = static_cast<double>(target_slot.GetPriceForUpGradeFare());
    //    ;      formula_calc *= item_level;
    //    ;      formula_calc *= enchant_info->GetFareRatio(enchant_option)->m_HaimFare;
    //    //assertion : enchant����� 42���� �Ѿ�� �ȵȴ�.
    //    enchant_fare = util::LL_Round(formula_calc);
    //};
    {
        enum { kPercent = 100 };
        enum { kHalfPercent = kPercent / 2 };
        const double haim_fare = enchant_info->GetFareRatio(enchant_option)->m_HaimFare * kPercent;
        const int conv_haim_fare = util::LL_Round(haim_fare);
#ifdef _NA_006589_20120415_ENCHANT_PRICE_MODIFY
        LEVELTYPE touch_item_level = target_item_info->m_LV;
        if (touch_item_level > 22) {
            touch_item_level = 23;
        }
        else if (touch_item_level > 12) {
            touch_item_level = 13;
        }
        else {
            touch_item_level = 3;
        }
        enchant_fare = target_slot.GetPriceForUpGradeFare() * touch_item_level * conv_haim_fare;
#else
        enchant_fare = target_slot.GetPriceForUpGradeFare() * item_level * conv_haim_fare;
#endif // _NA_006589_20120415_ENCHANT_PRICE_MODIFY
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        enchant_fare = root_->player_->GetAttr().GetValueAppliedAttr(enchant_fare, eATTR_ENCHANT_COST_RATIO);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        const bool is_round = (enchant_fare % kPercent) >= kHalfPercent;
        enchant_fare /= 100;
        if (is_round == true)
        {
            enchant_fare++;
        }
    }
    const RC::eITEM_RESULT check_enchant_cost = 
        ChkEnchantCost(enchant_info, enchant_fare, enchant_option);
    if (check_enchant_cost != RC::RC_ITEM_SUCCESS) 
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|��þƮ ��� �˻翡 ���� �߽��ϴ�.|ItemPosition = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            target_pos, 
            root_->player_->GetCharName()
        );
        return check_enchant_cost;
    }

    // �ٿ� ���� ������ �˻縦 �Ѵ�.
    bool use_down_guard = false;
    if (down_guard_item_pos != INVALID_SLOT_POS)
    {
        if (enchant_info->IsDownGrade() == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|��þƮ �ٿ��� ������ �������� �ƴմϴ�.|ItemCode = %d, ĳ�����̸� = %s|"), 
                __FUNCTION__, 
                target_item_info->m_Code, 
                root_->player_->GetCharName()
            );
            return RC::RC_ITEM_INVALID_TYPE;
        }
        if (inventory->ValidPos(down_guard_item_pos) == false)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|��þƮ �ٿ� ���� ������ ��ġ�� �ùٸ��� �ʽ��ϴ�.|ItemPosition = %d, ĳ�����̸� = %s|"), 
                __FUNCTION__, 
                down_guard_item_pos, 
                root_->player_->GetCharName()
            );
            return RC::RC_ITEM_INVALIDPOS;
        }
        const SCItemSlot* const down_guard_item = 
            static_cast<const SCItemSlot*>(inventory->GetValidSlotPtr(NULL, down_guard_item_pos));
        if (down_guard_item == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|��þƮ �ٿ� ���� ������ ��ġ�� �ùٸ��� �ʽ��ϴ�.|ItemPosition = %d, ĳ�����̸� = %s|"), 
                __FUNCTION__, 
                down_guard_item_pos, 
                root_->player_->GetCharName()
            );
            return RC::RC_ITEM_INVALIDPOS;
        }
        const BASE_ITEMINFO* const down_guard_item_info = down_guard_item->GetItemInfo(); 
        if (down_guard_item_info->m_wType != eITEMTYPE_ENCHANT)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|��þƮ �ٿ� ���� ������ Ÿ���� �ùٸ��� �ʽ��ϴ�.|ItemType = %d, ĳ�����̸� = %s|"), 
                __FUNCTION__, 
                down_guard_item_info->m_wType, 
                root_->player_->GetCharName()
            );
            return RC::RC_ITEM_INVALID_TYPE;
        }
        if (down_guard_item_info->m_wSubType != enchant_info->GetDownGradeType())
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|��þƮ �ٿ� ���� ������ ��� ����� �ùٸ��� �ʽ��ϴ�.|SubType = %d, ĳ�����̸� = %s|"), 
                __FUNCTION__, 
                down_guard_item_info->m_wSubType, 
                root_->player_->GetCharName()
            );
            return RC::RC_ITEM_INVALID_TYPE;
        }
        const RC::eITEM_RESULT using_result = root_->UseItemByType(
            eITEMTYPE_ENCHANT, SI_INVENTORY, down_guard_item_pos, true, __FUNCTION__);
        if (using_result != RC::RC_ITEM_SUCCESS)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|��þƮ �ٿ� ���� ������ ��뿡 ���� �߽��ϴ�.|UsingResult = %d, ItemCode = %d, ĳ�����̸� = %s|"), 
                __FUNCTION__, 
                using_result, 
                down_guard_item_info->m_Code, 
                root_->player_->GetCharName()
            );
            return using_result;
        }
        use_down_guard = true;
    }

    const RC::eITEM_RESULT check_delete_cost = 
        DeleteEnchantCost(enchant_info, enchant_fare, enchant_option);
    if (check_delete_cost != RC::RC_ITEM_SUCCESS) 
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|��þƮ ��� ������ ���� �߽��ϴ�.|ItemPosition = %d, ĳ�����̸� = %s|"), 
            __FUNCTION__, 
            target_pos, 
            root_->player_->GetCharName()
            );
        return check_delete_cost;
    }
    
    // ��þƮ Ȯ�� ó��
    BOOL success = TRUE;
    if (enchant_parser->IsOption100Per(enchant_option) == false)
    {
        int succeed_ratio = enchant_parser->PercentOptionToInt(enchant_option);
        succeed_ratio += root_->player_->GetAttr().GetIncreaseEnchantRatio();
#ifdef _NA_000000_20130612_RANDOMIZER_CHANGE
        RandomManager* const random_manager = root_->player_->GetRandomManager();
        int random_result = random_manager->randomizer_enchant_.Rand();
        success = (random_result <= succeed_ratio) ? TRUE : FALSE;
#else
        success = random_percent(succeed_ratio);
#endif //_NA_000000_20130612_RANDOMIZER_CHANGE
    }
#ifdef _NA_20100307_BETAKEY_SYSTEM
    enchant_fare = BetaKeyImpl::GetEnchantMoney(
        root_->player_->GetBetaKeyInfo(), enchant_info->GetBetaKeyInfo(), enchant_fare);
#endif

    return success ? ApplyEnchantSuccess(target_slot, enchant_info, enchant_fare, enchant_option)
                   : ApplyEnchantFail(target_slot, enchant_info, enchant_fare, enchant_option, use_down_guard);
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionEnchant::ApplyEnchantSuccess(
    SCItemSlot& target_slot, const MATERIALS_FOR_LEVEL* enchant_info,
    MONEY enchant_fare, eENCHANT_OPTION enchant_option)
{   
    const uint8_t new_enchant = target_slot.GetEnchant()+1;
    target_slot.SetEnchantUpdateDura(new_enchant);
    root_->SendItemChange(target_slot);
    LogEnchant(ITEM_ENCHANT_SUCCESS, target_slot, enchant_info, enchant_fare, enchant_option);

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    if (NotificationInfoParser::Instance()->IsNotificationItem(
        target_slot, eNotificationEnchant) == true)
    {
        MSG_DG_NOTIFICATION_ITEM_SYN msg;
        msg.info.SetInfo(root_->player_->GetCharName(),
            (root_->player_->GetGameZonePtr() != NULL) ? root_->player_->GetGameZonePtr()->GetMapCode() : 0,
            GetTickCount(), eNotificationEnchant, target_slot.GetItemCode(), target_slot.GetEnchant());
        g_pGameServer->SendToSessionType(GAME_DBPROXY, &msg, sizeof(MSG_DG_NOTIFICATION_ITEM_SYN));
    }
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    
    return RC::RC_ITEM_ENCHANT_SUCCESS;
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
RC::eITEM_RESULT nsSlot::ItemFunctionEnchant::ApplyEnchantFail(
    SCItemSlot& target_slot, 
    const MATERIALS_FOR_LEVEL* enchant_info,
    const MONEY enchant_fare, 
    const eENCHANT_OPTION enchant_option, 
    const bool use_down_guard
) {
    BYTE down_level = 0;

    //��þƮ ���� ������ ���� �ٿ� �ڵ�ĸ ����
    if (enchant_info->IsDownGrade() && (use_down_guard == false))
    {
        //������ ��þƮ �ٿ� ����
        BOOL downed = true;
        if (enchant_info->IsDownGrade100Per() == false)
        {
            BYTE down_ratio = enchant_info->GetDownPer();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            // ��æƮ ���� �� ������ �Ҹ�, �ٿ� ���� ��Ʈ����Ʈ ����
            int prevent_ratio = root_->player_->GetAttr().GetEnchantPreventDestroyNDowngradeItemRatio();
            if (prevent_ratio > 0 && random_percent(prevent_ratio))
            {
                // ��þƮ�� ���������� ������ �Ҹ� �� �ٿ� ���� �ߵ�
                LogEnchant(ITEM_ENCHANT_FAILED_PREVENT, target_slot, enchant_info,
                    enchant_fare, enchant_option, down_level);
                return RC::RC_ITEM_ENCHANT_FAILED_AND_PREVENT;
            }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            downed = random_percent(down_ratio);
        }

        if (downed)
        {
            const BYTE down_grade_type = enchant_info->GetDownGradeType();
            switch (down_grade_type)
            {
            case 1: //�Ҹ�Ÿ���� ��þƮ �ٿ��̸�..
                {           
                    down_level = enchant_info->GetDownLV();
                    int enchant = static_cast<int>(target_slot.GetEnchant()) - down_level;
                    if (FlowControl::FCAssert(enchant >= 0))
                    {
                        //(lst1024)(080918)(2447)��æƮ�� �������� �ٽ� ���� ���ش�.
                        target_slot.SetEnchantUpdateDura(enchant);
                        root_->SendItemChange(target_slot);
                    }
                    LogEnchant(ITEM_ENCHANT_FAILED_DOWN, target_slot, enchant_info,
                               enchant_fare, enchant_option, down_level);
                    return RC::RC_ITEM_ENCHANT_FAILED_AND_ENCHNAT_DOWN;
                }
                break;
            case 2: //�Ҹ�Ÿ���� ������ �Ҹ��̸�..
                {
                    root_->Delete(target_slot.GetPos());
                    LogEnchant(ITEM_ENCHANT_FAILED_DESTROY, target_slot, enchant_info,
                               enchant_fare, enchant_option, down_level);
                    return RC::RC_ITEM_ENCHANT_FAILED_AND_DESTROY;
                }
                break;
            }
        }
    }

    // ��þƮ ���� �α����� 
    LogEnchant(ITEM_ENCHANT_FAILED, target_slot, enchant_info,
               enchant_fare, enchant_option, down_level);
    return RC::RC_ITEM_ENCHANT_FAILED;
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
void nsSlot::ItemFunctionEnchant::LogEnchant(
    int iLogType,
    SCItemSlot& target_slot, const MATERIALS_FOR_LEVEL* enchant_info,
    MONEY enchant_fare, eENCHANT_OPTION enchant_option, BYTE byDownLV)
{
    // (CHANGES) (WAVERIX) change vector control to more cheap routine
    //�α������� ����� ���� ��þƮ ���� vector
    static STLX_VECTOR<COMPOSITEMATERIAL> consumed_materials;
    consumed_materials.resize(0);

    double material_fare = enchant_info->GetFareRatio(enchant_option)->m_MaterialFare;
    EnchantParser* const enchant_parser = EnchantParser::Instance();
    for (POSTYPE i = 0; i < MATERIALS_FOR_LEVEL::kMaxMaterialNumber; ++i)
    {
        const sMATERIALS* materials = enchant_info->GetMaterials(i);
        if (materials == NULL) {
            continue;
        }
        COMPOSITEMATERIAL info = { 0, };
        info.MaterialCode = materials->m_MaterialItemCode;
        info.MaterialNum  = enchant_parser->CalcMaterialNum(materials->m_MaterialItemNum, material_fare);
        consumed_materials.push_back(info);
    }

    // ��þƮ �α� 
    GAMELOG->LogEnchant(iLogType, root_->player_, target_slot, enchant_fare, consumed_materials);
}

// CHANGES: f110611.1L, moved from ItemManager_Composite.cpp
// ����Ʈ�� ���� ���
RC::eITEM_RESULT nsSlot::ItemFunctionEnchant::UseExtraStone(
    POSTYPE extra_stone_pos, POSTYPE target_pos, BYTE& OUT value_index_out)
{
    if (root_->IsRestrictFunction(eFUNCTIONALRESTRICT_USE_EXTRASTONE)) {
        return RC::RC_ITEM_UNUSABLE_FUNCTION;
    }
    if (root_->player_->GetChaoState().IsChaoState()) {
        return RC::RC_ITEM_UNABLE_FUNCTION_FOR_CHAOSTATE;
    }
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (root_->ValidPos(SI_INVENTORY, extra_stone_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    if (root_->ValidPos(SI_INVENTORY, target_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    //----------------------------------------------------------------------------------------------
    SCSlotContainer* inventory = root_->GetItemSlotContainer(SI_INVENTORY);
    SCItemSlot& target_item = static_cast<SCItemSlot&>(inventory->GetSlot(target_pos));
    // ����, ��, �Ǽ�����, Ư�� �Ǽ�����
    if (target_item.GetItemRuleInfo().IsEnableExtraOption() == false) {
        return RC::RC_ITEM_INVALID_VALUE;
    };
    //----------------------------------------------------------------------------------------------
    const BASE_ITEMINFO* target_item_info = target_item.GetItemInfo();
    if (target_item.CanUseMixture(MIXTURE_EXTRASTONE) == false) {
        return RC::RC_ITEM_NOT_MIXTURE;
    }
    ItemCompositeParser* const composite_parser = ItemCompositeParser::Instance();
    const sITEMCOMPOSITE* composite = \
        composite_parser->GetCompositeInfo(eITEMCOPOSITE_EXTRASTONE_USE);
    if (composite == NULL) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    //----------------------------------------------------------------------------------------------
    SCItemSlot& extra_stone_item = static_cast<SCItemSlot&>(inventory->GetSlot(extra_stone_pos));
    // ����Ʈ�� ���� ���� üũ
    const SLOTCODE extra_stone_item_code = extra_stone_item.GetItemCode();
    if (extra_stone_item_code != composite->m_sCompositeMaterials[0].m_dwCompositeMaterialCode)
    {
        return RC::RC_ITEM_INVALID_VALUE;
    }

    const sEXTRASTONE_OPT_INFO* stone_option = ExtraStoneOptionInfoParser::Instance()->
        GetExtraStoneOptionInfo(target_item_info->m_wType);
    if (stone_option == NULL) {
        return RC::RC_ITEM_INVALID_VALUE;
    }
    if (stone_option->m_ItemLV != 0 && stone_option->m_ItemLV != target_item_info->m_LV) {
        return RC::RC_ITEM_INVALID_VALUE;
    }

    int value_index = target_item.GetNOptionValueIndex();
    if (value_index >= sEXTRASTONE_OPT_INFO::MAX_VALUE_NUM - 1) {
        return RC::RC_ITEM_INVALID_VALUE;
    }
    ++value_index;
    target_item.SetNOptionAttr(value_index);
    // impilicit value_index value range < 255
    value_index_out = value_index;

    GAMELOG->LogItem(ITEM_USE_EXTRASTONE, root_->player_, &target_item);

    // updates extra stone item: delete or discount
    // assert stone_remains != 0
    DURATYPE stone_remains = extra_stone_item.GetNum();
    if (stone_remains == 1) {
        inventory->DeleteSlot(extra_stone_pos, NULL);
    }
    else if (stone_remains > 1) {
        inventory->UpdateSlot(extra_stone_pos, -1);
    }

    return RC::RC_ITEM_SUCCESS;
}
