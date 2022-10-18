// InventorySlotContainer.cpp: implementation of the CInventorySlotContainer class.
//
//==================================================================================================
#include "stdafx.h"
#include "InventorySlotContainer.h"
#include "Player.h"
#include "QuickManager.h"
#include "QuickSlotContainer.h"
#include <SCQuickSlot.h>
#include "ItemManager.h"
#include <PacketStruct_AG.h>
#include "ItemInfoParser.h"
#include <ItemAttrCalculator.h>
#include <AppliedItemDependencyChain.h>
#include <boost/static_assert.hpp>

//==================================================================================================
// Construction/Destruction

CInventorySlotContainer::CInventorySlotContainer()
    : m_pPlayer(NULL),
      m_TabNum(0)
{
    // (CHANGES) change input value of MAX_INVENTORY_SLOT_NUM to default size that is 75
    // changes real slot_list size by called Resize, to save memory, to use slot_list iterating
    const POSTYPE kDefaultInventorySize = MAX_INVENTORY_TAB_NUM * MAX_INVENTORY_SLOT_PER_TAB;
    Init(kDefaultInventorySize, SI_INVENTORY);
    ZeroMemory(m_InsertRuleBaseSlotList, sizeof(m_InsertRuleBaseSlotList));
}

CInventorySlotContainer::~CInventorySlotContainer()
{
    ClearConfiguredSlotList();
}

//==================================================================================================
//  <CInventorySlotContainer::OptionEffectOperator>

void
CInventorySlotContainer::ClearAll()
{
    SCItemSlotContainer::ClearAll();
    m_OptionEffectItems.clear();
}


BOOL
CInventorySlotContainer::CheckPrevInsert(SCItemSlot& rInsertSlot)
{
    const BASE_ITEMINFO* const pItemInfo = rInsertSlot.GetItemInfo();
    //if(FALSE == FlowControl::FCAssert(pItemInfo))
    //    return false;

    const int add_num = rInsertSlot.GetNum();

    // AppliedItemDependencyChain 검사 - Exclusive Item
    AppliedItemDependencyChain* pAIDChain = AppliedItemDependencyChain::Instance();
    if(pAIDChain->IsAcceptableInsert(m_pPlayer, rInsertSlot, add_num))
        return true;

    return false;
}

//==================================================================================================
//

RC::eSLOT_INSERT_RESULT
CInventorySlotContainer::InsertSlot(POSTYPE at_pos, SCSlot& IN rSlot)
{
    SCItemSlot& rSrcSlot = (SCItemSlot&)rSlot;

    if(FALSE == CheckPrevInsert(rSrcSlot))
        return RC::RC_INSERT_SLOT_FAILED;

    RC::eSLOT_INSERT_RESULT rt = SCSlotContainer::InsertSlot(at_pos, rSlot);
    if(RC::RC_INSERT_SLOT_SUCCESS == rt)
    {
        SCItemSlot& rCurSlot = (SCItemSlot&)GetSlot(at_pos);

        SCQuickSlotContainer* quick_container = m_pPlayer->GetQuickManager()->GetQuickSlotContainer();
        // for debug section
        SLOTIDX in_slot_idx = rSrcSlot.GetSlotIdx();
        POSTYPE in_slot_pos = rSrcSlot.GetPos();
        SLOTCODE in_slot_code = rSrcSlot.GetItemCode();

        quick_container->UpdateQuickSlot(in_slot_idx, in_slot_pos, in_slot_code,
                                         SI_INVENTORY, at_pos, rCurSlot.GetItemCode());

        //m_pPlayer->GetQuickManager()->GetQuickSlotContainer()->UpdateQuickSlot(rCurSlot.GetSlotIdx(), rCurSlot.GetPos(), rCurSlot.GetItemCode(), 
        //  SI_INVENTORY, at_pos, rCurSlot.GetItemCode());
        if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
                m_pPlayer->GetItemManager()->GetUpdaterImpl())
        {
            item_updater_impl->AddSerialCode(eSERIAL_INVENTORY, GetSlotIdx(),
                                             rCurSlot.GetPos(), rCurSlot.GetWorldSerial());
        }
    }

    return rt;
}

BOOL
CInventorySlotContainer::DeleteSlot(POSTYPE at_pos, SCSlot * pSlotOut)
{
    SCItemSlot temp_delete_item_slot;

    if(SCSlotContainer::DeleteSlot(at_pos, &temp_delete_item_slot) == 0)
        return false;

    // (NOTE) 미묘한 인터페이스 타입 문제가 항상 도처에 깔린 편이다.
    // 그저 문제 없이 구현하고, 문제가 생겨도 바로 알 수 있기를 기원할 뿐이다.
    if(pSlotOut)
        static_cast<SCItemSlot*>(pSlotOut)->Copy(temp_delete_item_slot);

    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->RemoveSerialCode(GetSlotIdx(), at_pos, temp_delete_item_slot.GetWorldSerial());
    };
    //_NA_004035_20120227_COSTUME_ITEM 
    // 착용 해제..
    if (const BASE_ITEMINFO* item_info = temp_delete_item_slot.GetItemInfo())
    {
        if (eITEMTYPE(item_info->m_wType) == eITEMTYPE_COSTUME)
        {
            if (m_pPlayer->GetIsEquippedCostume() == true
                && m_pPlayer->GetCostumeItemPos() == temp_delete_item_slot.GetPos()
                && m_pPlayer->GetCostumeItemCode() == temp_delete_item_slot.GetItemCode())
            {
                m_pPlayer->GetCostumeHandler().Equipped(temp_delete_item_slot, false);
            }
        }
#ifdef _NA_008610_20160104_ADD_DECORATECOSTUME
        else if (eITEMTYPE(item_info->m_wType) == eITEMTYPE_COSTUME_DECORATE)
        {
            if (m_pPlayer->GetIsEquippedDecoCostume() == true
                && m_pPlayer->GetDecoCostumeItemPos() == temp_delete_item_slot.GetPos()
                && m_pPlayer->GetDecoCostumeItemCode() == temp_delete_item_slot.GetItemCode())
            {
                m_pPlayer->GetCostumeHandler().Equipped(temp_delete_item_slot, false);
            }
        }
#endif //_NA_008610_20160104_ADD_DECORATECOSTUME
    }
    return TRUE;
}

void
CInventorySlotContainer::OnInsert(SCSlot& IN rSlot)
{
    const BASE_ITEMINFO* item_info = ((SCItemSlot&)rSlot).GetItemInfo(); //for debug
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->AddExpirationItem((SCItemSlot &)rSlot);
    }
    int changed_num = rSlot.GetNum();
    OnUpdate(rSlot, changed_num);
}

void
CInventorySlotContainer::OnDelete(SCSlot& IN rSlot)
{
    const BASE_ITEMINFO* item_info = ((SCItemSlot&)rSlot).GetItemInfo(); //for debug
    if (nsSlot::ItemManagerUpdateImpl* const item_updater_impl =
        m_pPlayer->GetItemManager()->GetUpdaterImpl())
    {
        item_updater_impl->RemoveExpirationItem((SCItemSlot &)rSlot);
    }
    int changed_num = rSlot.GetNum();
    OnUpdate(rSlot, -changed_num);
}

void CInventorySlotContainer::ApplyEffect(SCItemSlot& item_slot,
    const int changed_num, const int changed_total_count)
{
#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
    const bool pc_room_status = (m_pPlayer->CanApplyPCBangBenefit(true) != false);
    InvenItemOptionCalculator::ApplyInvenItemOption(m_pPlayer->GetPlayerAttr(),
        item_slot, changed_num, pc_room_status, true);

    if (AppliedItemDependencyChain* dependency_chain = AppliedItemDependencyChain::Instance()) {
        dependency_chain->CheckAndApplyEffect(
            m_pPlayer, item_slot, (changed_num >= 0), changed_total_count);
    };
    return;
#else
    int loopCount = changed_num >= 0 ? changed_num : -changed_num;

    const BOOL bPCBang = m_pPlayer->CanApplyPCBangBenefit(TRUE);
    ItemAttrCalculator itemCalc(m_pPlayer->GetPlayerAttr(), *this);
    itemCalc.SetCalcOption(FALSE);

    BOOLEAN is_inserted = changed_num > 0;
    for( ; loopCount; --loopCount)
    {
        BOOL bDummy;
        if(is_inserted)
            itemCalc.Equip(item_slot, bPCBang, TRUE, bDummy);
        else
            itemCalc.UnEquip(&item_slot, bPCBang, TRUE, bDummy);
    }

    AppliedItemDependencyChain* dependency_chain = AppliedItemDependencyChain::Instance();
    dependency_chain->CheckAndApplyEffect(m_pPlayer, item_slot, is_inserted, changed_total_count);
#endif
}

void
CInventorySlotContainer::OnUpdate(SCSlot& IN rSlot, int added_num)
{
    SCItemSlot& rItemSlot = static_cast<SCItemSlot &>(rSlot);
    const BASE_ITEMINFO* pItemInfo = rItemSlot.GetItemInfo();

    //{__NA_0_CASH_ITEM_RENEWAL_SUPERMASTER
    if (pItemInfo->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX &&
        rItemSlot.CanEquip() == false)
    {
        //활성화 된 아이템만 소유 상태를 증가한다.
        if(added_num > 0)
        {
            if(!rItemSlot.IsProgressTime())
                return;
        }
        //만료기간이 끝났거나 활성화 된 아이템만 소유 상태를 감소한다.
        else
        {
            if(!rItemSlot.IsExpiredTime() && !rItemSlot.IsProgressTime())
                return;
        }
    }
    //}__NA_0_CASH_ITEM_RENEWAL_SUPERMASTER
    if (rItemSlot.IsUserBlocked())
    {
        if (rItemSlot.IsRiderItem()) {
            if(m_pPlayer->Rider.SyncNotifyRiderSummon(rItemSlot, m_pPlayer) == false) {
                rItemSlot.RiderSummonStatus(false);
            };
        }
    };
    // Agent에서 소유상태를 알아야되는 아이템
    ItemOwnFlag& rOwnFlag = m_pPlayer->GetItemOwnFlag();
    const eITEMTYPE item_type = (eITEMTYPE)pItemInfo->m_wType;
    if(rOwnFlag.IsSendToAgentServer(item_type))
    {
        if(added_num > 0)
            rOwnFlag.IncItemInfo(item_type, abs(added_num));
        else
            rOwnFlag.DecItemInfo(item_type, abs(added_num));

        MSG_AG_ITEM_CHANGE_SPECIAL_ITEM_OWN_CMD cmdMsg;
        cmdMsg.m_Apply = added_num > 0;
        cmdMsg.m_ItemType = pItemInfo->m_wType;
        cmdMsg.m_Count = abs(added_num);
        m_pPlayer->SendPacket(&cmdMsg, sizeof(cmdMsg));
    }

    if(added_num == 0)
        return;

    BOOST_STATIC_ASSERT(0 < BASE_ITEMINFO::_MAX_OPTION_NUM &&
                            BASE_ITEMINFO::_MAX_OPTION_NUM <= sizeof(DWORD64));
    typedef util::SEQ_BYTE_MASK<eITEM_EFFECT_ON_INVEN, BASE_ITEMINFO::_MAX_OPTION_NUM> \
        OPTION_EFFECT_INVEN_MASK;

    const BOOLEAN isOptionEffectItem =
        ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_INVEN_MASK::MASK, pItemInfo->m_byOptionExEffect) ||
        ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_INVEN_MASK::MASK, pItemInfo->m_byPCOptionExEffect);

    if(isOptionEffectItem == 0)
        return;

    // 실제 옵션적용에 영향을 주는 아이템개수 구하기
    int& OUT out_effect_items_num = m_OptionEffectItems[ pItemInfo->m_Code ];
    int already_applied_num = min(pItemInfo->m_byDupEquipNum, out_effect_items_num);
    int need_apply_num = min(out_effect_items_num + added_num, pItemInfo->m_byDupEquipNum);
    if(need_apply_num < 0)
    {
        // 가지고 있는 개수보다 더 많이 줄어들수는 없다.
        SUNLOG(eCRITICAL_LOG, "[CInventorySlotContainer::OnUpdate] ResultApplingCount(%u)",
               need_apply_num);
        need_apply_num = 0;
    }

    int change_num = need_apply_num - already_applied_num;
    // 변경된 개수만큼 아이템 옵션을 적용한다.
    if(change_num)
        ApplyEffect(rItemSlot, change_num, out_effect_items_num + added_num);
    out_effect_items_num += added_num;
}

void
CInventorySlotContainer::SetValidState(BOOL v)
{
    BASE_PLAYERINFO* base_char_info = m_pPlayer->GetCharInfo();
    base_char_info->m_iInventoryLock = v;
    SCItemSlotContainer::SetValidState(v);
}

BOOL
CInventorySlotContainer::ValidState()
{
    // (BUGFIX) (091202) (WOPS:5334) 인벤잠금 상태에서 아이템 제거 가능한 특수 처리(PK드랍)
    ItemManager::TransactionClosure::value_type transaction =
        m_pPlayer->GetItemManager()->GetCurrentTransactionAction();
    if(transaction == ItemManager::TransactionClosure::eTransType_None) {
        ProtectorInventory& rPROTECT = m_pPlayer->GetWarehouseInventoryProtector().INVENTORY();
        if(!rPROTECT.IsUsable())
            return false;
    }

    if(m_pPlayer->GetCharInfo()->m_iInventoryLock)
        return false;
    else
        return SCItemSlotContainer::ValidState();
}


void
CInventorySlotContainer::UpdateAttr()
{
    //{__NA_0_CASH_ITEM_RENEWAL_SUPERMASTER
    // 버그 수정 해줌.
    ItemOwnFlag& rOwnFlag = m_pPlayer->GetItemOwnFlag();
    rOwnFlag.Init();
    //}
    m_OptionEffectItems.clear();

    const POSTYPE upperbound = GetMaxSlotNum();
    for(POSTYPE i = 0; i < upperbound; ++i)
    {
        if(IsEmpty(i))
            continue;

        SCItemSlot& rItemSlot = static_cast<SCItemSlot&>(GetSlot(i));
        if(CheckPrevInsert(rItemSlot))
            OnUpdate(rItemSlot, rItemSlot.GetNum());
    }
}


// implemented by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_

//--------------------------------------------------------------------------------------------------

namespace nsSlot {

static const struct InventoryConfigMatchNode
{
    nsSlot::InventoryConfig::eInsertRule kRule;
    int kIndex;
} kInventoryConfigMatchNodeList[nsSlot::InventoryConfig::kMaxNumberOfInsertRules] =
{
    { nsSlot::InventoryConfig::eInsertRule_Equip, 0 },
    { nsSlot::InventoryConfig::eInsertRule_Quest, 1 },
    { nsSlot::InventoryConfig::eInsertRule_AC, 2 },
    { nsSlot::InventoryConfig::eInsertRule_Material, 3 },
    { nsSlot::InventoryConfig::eInsertRule_Normal, 4 },
    { nsSlot::InventoryConfig::eInsertRule_Waste, 5 },
    { nsSlot::InventoryConfig::eInsertRule_All, 6 },
};

// (Ref100328) slot linear list array order based on the policy
// all configuration use a default SlotList of the SCSlotContainer
static const nsSlot::InventoryConfig::SetType
kSlotOrderList[nsSlot::InventoryConfig::kMaxNumberOfInsertRules - 1] =
{
    nsSlot::InventoryConfig::eInsertRule_Equip,
    nsSlot::InventoryConfig::eInsertRule_Quest,
    nsSlot::InventoryConfig::eInsertRule_AC,
    nsSlot::InventoryConfig::eInsertRule_Material,
    nsSlot::InventoryConfig::eInsertRule_Normal,
    nsSlot::InventoryConfig::eInsertRule_Waste,
    //InventoryConfig::eInsertRule_All,
};

}; //end of namespace

//--------------------------------------------------------------------------------------------------
/*
const CInventorySlotContainer::SLOT_LINEAR_LIST&
CInventorySlotContainer::GetConfiguredSlotList(const nsSlot::InventoryConfig::eInsertRule rule) const
{
    using nsSlot::InventoryConfig;
    // (Ref100328) slot linear list array based on the policy
    if (rule == InventoryConfig::eInsertRule_All ||
        rule == static_cast<InventoryConfig::eInsertRule>(0))
    {
        // policy for all
        return GetSlotList();
    }
#ifdef _DEBUG
    // require one field set
    switch (rule)
    {
    case InventoryConfig::eInsertRule_Equip:
    case InventoryConfig::eInsertRule_Quest:
    case InventoryConfig::eInsertRule_AC:
    case InventoryConfig::eInsertRule_Material:
    case InventoryConfig::eInsertRule_Normal:
    case InventoryConfig::eInsertRule_Waste:
        break;
    default:
        assert(!"Unexpected input argument, must be input the value of the eInsertRule");
        return GetSlotList();
    }
#endif

    const nsSlot::InventoryConfigMatchNode* it = nsSlot::kInventoryConfigMatchNodeList,
                                          * end = &nsSlot::kInventoryConfigMatchNodeList[\
                                                _countof(nsSlot::kInventoryConfigMatchNodeList)];
    for (; it != end; ++it)
    {
        if (it->kRule != rule) {
            continue;
        }
        if (SLOT_LINEAR_LIST* selected_list = m_InsertRuleBaseSlotList[it->kIndex]) {
            return *selected_list;
        }
        break;
    }
    const SLOT_LINEAR_LIST& default_slot_list = GetSlotList();
    return default_slot_list;
}
*/
// f100904.4L, The interface to get sorted slot list start/end offset by configurations
void CInventorySlotContainer::GetConfiguredSlotList(const nsSlot::InventoryConfig::eInsertRule rule,
                                                    nsSlot::SearchOffset* result_offset_info) const
{
    using namespace nsSlot;
    // (Ref100328) slot linear list array based on the policy
    if (rule == InventoryConfig::eInsertRule_All ||
        rule == static_cast<InventoryConfig::eInsertRule>(0))
    {
        // policy for all
        *result_offset_info = insert_rule_list_offset_[_countof(insert_rule_list_offset_)];
        return;
    }
#ifdef _DEBUG
    // require one field set
    switch (rule)
    {
    case InventoryConfig::eInsertRule_Equip:
    case InventoryConfig::eInsertRule_Quest:
    case InventoryConfig::eInsertRule_AC:
    case InventoryConfig::eInsertRule_Material:
    case InventoryConfig::eInsertRule_Normal:
    case InventoryConfig::eInsertRule_Waste:
        break;
    default:
        assert(!"Unexpected input argument, must be input the value of the eInsertRule");
        *result_offset_info = insert_rule_list_offset_[_countof(insert_rule_list_offset_)];
        return;
    }
#endif

    const nsSlot::InventoryConfigMatchNode* it = nsSlot::kInventoryConfigMatchNodeList,
                                          * end = &nsSlot::kInventoryConfigMatchNodeList[\
                                                _countof(nsSlot::kInventoryConfigMatchNodeList)];
    for (; it != end; ++it)
    {
        if (it->kRule != rule) {
            continue;
        }
        *result_offset_info = insert_rule_list_offset_[it->kIndex];
        return;
    }
    *result_offset_info = insert_rule_list_offset_[_countof(insert_rule_list_offset_)];
    return;
}

namespace nsSlot {
;

struct SetTypeSortedArray {
    int tab_index;
    InventoryConfig::SetType config;
    int mode;
};

}; //end of namespace

// update all inventory insert rule configures
void CInventorySlotContainer::UpdateInventoryConfiguration(const nsSlot::InventoryConfig& input)
{
    using nsSlot::InventoryConfig;
    using nsSlot::SetTypeSortedArray;
    // generally, inputed data structure byte alignment, packet structure, is not be in accord with \
    // the best guide of system architecture to support hardware high performance.
    // additionally, below the data structure has expand slot check field using migration
    struct InventoryConfigControl : public nsSlot::InventoryConfig
    {
        bool checked[kMaxNumberOfConfigurations];
        void ClearHistory()
        {
            BOOST_STATIC_ASSERT(_countof(configures) == _countof(checked));
            ZeroMemory(checked, sizeof(checked));
        }
        void Initialize(nsSlot::InventoryConfig* modified, const nsSlot::InventoryConfig& input)
        {
            ZeroMemory(this, sizeof(*this));
            InventoryConfig::SetType* dest = modified->configures;
            InventoryConfig::SetType* it = configures,
                                    * end = &configures[_countof(configures)];
            const InventoryConfig::SetType* src_it = input.configures;
            for ( ; it != end; ++it, ++src_it, ++dest)
            {
                if (*src_it == 0) {
                    *it = InventoryConfig::eInsertRule_All;
                }
                else {
                    *it = (*src_it) & InventoryConfig::eInsertRule_All;
                }
                *dest = *it;
            }
        };
    } configure;
    // set up 'configure'
    configure.Initialize(&m_pPlayer->GetCharInfo()->m_InventoryConfig, input);

    // tab based working storage
    SetTypeSortedArray set_type_sorted_array[_countof(configure.configures)];
    // mask mode operator
    struct MaskModeOperator {
        // (CHANGES) fix up a sorting condition which is a different result
        // related to the 'all' checked state
        // [0] : and mode (multi-bits except all)
        // [1] : or (all)
        // [2] : or (others)
        __forceinline bool setup_config(int mode, InventoryConfig::SetType config_condition)
        {
            if (mode >= 2) {
                return true;
            }
            bool all_configuration = config_condition == InventoryConfig::eInsertRule_All;
            switch (mode)
            {
            case 0:
                if (all_configuration == false && (config_condition & origin_mask)) {
                    return true;
                }
                break;
            case 1:
                if (all_configuration) {
                    return true;
                }
                break;
            }
            return false;
        }
        InventoryConfig::SetType origin_mask;
    };
    //
    const SLOT_LINEAR_LIST& original_list = GetSlotList();
    const int current_max_number_of_tabs = original_list.size() / MAX_INVENTORY_SLOT_PER_TAB;
    // terrible... time complexity O(n^2)... specification = { n * 2 * n + (n * n) }
    for (int order_index = 0; order_index < _countof(nsSlot::kSlotOrderList); ++order_index)
    {
        ZeroMemory(set_type_sorted_array, sizeof(set_type_sorted_array));
        SetTypeSortedArray* dest_it = set_type_sorted_array;
        //
        configure.ClearHistory();
        MaskModeOperator mask_mode = { nsSlot::kSlotOrderList[order_index] };
        for (int mode_index = 0; mode_index < 3; ++mode_index)
        {
            for (int config_index = 0;
                 (config_index < current_max_number_of_tabs &&
                  config_index < _countof(configure.configures));
                 ++config_index)
            {
                if (configure.checked[config_index] != false) {
                    continue;
                }
                InventoryConfig::SetType* const config = &configure.configures[config_index];
                if (mask_mode.setup_config(mode_index, *config) &&
                    configure.checked[config_index] == false)
                {
                    dest_it->tab_index = static_cast<int>(config - configure.configures);
                    dest_it->config = *config;
                    dest_it->mode = mode_index; // f100904.4L, to support two phase group sorting
                    ++dest_it;
                    configure.checked[config_index] = true;
                }
            }
        }
        //
        SLOT_LINEAR_LIST* slot_of_slot_list = m_InsertRuleBaseSlotList[order_index];
        if (slot_of_slot_list == NULL)
        {
            slot_of_slot_list = TAllocNew(SLOT_LINEAR_LIST);
            m_InsertRuleBaseSlotList[order_index] = slot_of_slot_list;
        }
        slot_of_slot_list->reserve(current_max_number_of_tabs * MAX_INVENTORY_SLOT_PER_TAB);
        slot_of_slot_list->resize(0);

        // copy range
        dest_it = set_type_sorted_array;
        int number_of_sort_groups[3] = { 0, };
        for (int index = 0;
             (index < current_max_number_of_tabs &&
              dest_it != &set_type_sorted_array[_countof(set_type_sorted_array)]);
             ++index, ++dest_it)
        {
            SLOT_LINEAR_LIST::const_iterator offset_it = original_list.begin();
            offset_it += static_cast<int>(dest_it->tab_index * MAX_INVENTORY_SLOT_PER_TAB);
            // can't use std::copy because of danger, instead of ...what is the replacement?
            for (int loop = MAX_INVENTORY_SLOT_PER_TAB; loop; --loop, ++offset_it) {
                slot_of_slot_list->push_back(*offset_it);
            }
            // unused field (dest_it->config)...
            ++number_of_sort_groups[dest_it->mode];
        }
        // f100904.4L, prepare range iterators to support a six phase inversed select operation
        if (nsSlot::SearchOffset* search_node = &insert_rule_list_offset_[order_index])
        {
            SLOT_LINEAR_LIST::const_iterator list_it = slot_of_slot_list->begin(),
                                             list_end = slot_of_slot_list->end();
            {
                for (SLOT_LINEAR_LIST::const_iterator* offset_it = search_node->search_it;
                    offset_it != &search_node->search_it[_countof(search_node->search_it)];
                    ++offset_it)
                {
                    *offset_it = list_end;
                }
            }
            {
                for (SLOT_LINEAR_LIST::const_iterator* offset_it = search_node->search_end;
                    offset_it != &search_node->search_end[_countof(search_node->search_end)];
                    ++offset_it)
                {
                    *offset_it = list_end;
                }
            }
            //
            {
                SLOT_LINEAR_LIST::const_iterator offset_it = list_it;
                if (number_of_sort_groups[0])
                {
                    search_node->search_it[1] =
                        search_node->search_it[0] = offset_it;
                    offset_it += number_of_sort_groups[0] * MAX_INVENTORY_SLOT_PER_TAB;
                    search_node->search_end[1] =
                        search_node->search_end[0] = offset_it;
                }
                if (number_of_sort_groups[1])
                {
                    search_node->search_it[3] =
                        search_node->search_it[2] = offset_it;
                    offset_it += number_of_sort_groups[1] * MAX_INVENTORY_SLOT_PER_TAB;
                    search_node->search_end[3] =
                        search_node->search_end[2] = offset_it;
                }
                if (number_of_sort_groups[2])
                {
                    search_node->search_it[5] =
                        search_node->search_it[4] = offset_it;
                    offset_it += number_of_sort_groups[2] * MAX_INVENTORY_SLOT_PER_TAB;
                    search_node->search_end[5] =
                        search_node->search_end[4] = offset_it;
                }
            }
        };
    };
    //last section, f100904.4L, 'all' configuration has not sorting policy except an overlapping case
    if (nsSlot::SearchOffset* search_node = \
        &insert_rule_list_offset_[nsSlot::InventoryConfigSwitcher::eSwitch_All])
    {
        SLOT_LINEAR_LIST::const_iterator list_it = original_list.begin(),
                                         list_end = original_list.end();
        {
            for (SLOT_LINEAR_LIST::const_iterator* offset_it = search_node->search_it;
                offset_it != &search_node->search_it[_countof(search_node->search_it)];
                ++offset_it)
            {
                *offset_it = list_it;
            }
        }
        {
            for (SLOT_LINEAR_LIST::const_iterator* offset_it = search_node->search_end;
                offset_it != &search_node->search_end[_countof(search_node->search_end)];
                ++offset_it)
            {
                *offset_it = list_end;
            }
        }
    }
}

void CInventorySlotContainer::ClearConfiguredSlotList()
{
    for (SLOT_LINEAR_LIST** it = m_InsertRuleBaseSlotList;
         it != &m_InsertRuleBaseSlotList[_countof(m_InsertRuleBaseSlotList)];
         ++it)
    {
        SLOT_LINEAR_LIST* slot_list = *it;
        if (slot_list) {
            TAllocDelete(SLOT_LINEAR_LIST, slot_list);
        }
    }
    ZeroMemory(m_InsertRuleBaseSlotList, sizeof(m_InsertRuleBaseSlotList));
}

