////////////////////////////////////////////////////////////////////////////////
///
/// @file   ImplementUtil.cpp
/// @author hukim
/// @brief  [2010/10/13 by hukim] This is created.\n
/// @history
///     - ~10.12.20
///     - 10.12.20, waverix, CHANGES: f101220.1L
///
////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "./ImplementUtil.h" // header

namespace util { namespace internal {
;

//==================================================================================================

Player* IsExistInField(DWORD user_key)
{
    const PlayerManager* const player_manager = PlayerManager::Instance();
    if (Player* requester = player_manager->FindPlayerByUserKey(user_key))
    {
        if (requester->GetField() == NULL) {
            return NULL;
        }
        //
        return requester;
    }
    return NULL;
}

//==================================================================================================
// 요청자 상태에 대해 기본적인 검사를 한다.
bool CheckRequesterState(
    Player* const requester, const TCHAR* called_function, int& OUT result_code)
{
    // 인벤토리가 잠겼는지 검사한다.
    if (!(requester->GetWarehouseInventoryProtector()).INVENTORY().IsUsable())
    {
        result_code = RC::RC_ITEM_INVENTORY_NEED_PASSWORD;
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|인벤토리가 잠겨 있습니다.|캐릭터이름 = %s|"), 
               called_function, 
               requester->GetCharName());
        return false;
    }

    // 요청자 상태를 검사한다.
    if (requester->GetBehave() == PLAYER_BEHAVE_UNEXPECTED_WAITING) 
    {
        result_code = RC::RC_ITEM_INVALIDSTATEOFPLAYER;
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|행동상태가 옳바르지 않습니다.|행동상태 = %d, 캐릭터이름 = %s|"), 
               called_function, 
               requester->GetBehave(), 
               requester->GetCharName());
        return false;
    }

    // 아이템 슬롯 상태를 검사한다.
    ItemManager* const item_manager = requester->GetItemManager();
    if (!item_manager->ValidState())
    {
        result_code = RC::RC_ITEM_INVALIDSTATE;
        SUNLOG(eCRITICAL_LOG, 
               _T("|%s|아이템 슬롯 상태가 옳바르지 않습니다.|캐릭터이름 = %s|"), 
               called_function, 
               requester->GetCharName());
        return false;
    }

    return true;
}

//==================================================================================================
// 아이템 합성 또는 분해 보상

// 인스턴스를 생성한다.
ItemCompositionOrDecompositionReward::RewardInterface*
ItemCompositionOrDecompositionReward::RewardInterface::CreateInstance(const RewardType reward_type) 
{
    switch (reward_type)
    {
    case kItemCodeReward:
        return new ItemCodeReward();
    case kRewardCodeReward:
        return new RewardCodeReward();
    default:
        assert(false);
        break;
    }
    return NULL;
}

ItemCompositionOrDecompositionReward::ItemCodeReward::ItemCodeReward()
    : number_of_kinds_(0), is_use_ig_type_(false), is_use_zard_option_(false)
{
    ::ZeroMemory(item_code_list_, sizeof(item_code_list_));
    ::ZeroMemory(item_num_list_, sizeof(item_num_list_));
    ::ZeroMemory(ig_type_list_, sizeof(ig_type_list_));
    ::ZeroMemory(zard_option_list_, sizeof(zard_option_list_));
}

bool ItemCompositionOrDecompositionReward::ItemCodeReward::Add(const ItemReward& item_reward)
{
    if (item_reward.item_code == kUnused || item_reward.number_of_item < 1)
    {
        return false;
    }
    if (number_of_kinds_ >= kMaxKindOfReward)
    {
        return false;
    }
    // 이미 같은 아이템이 보상으로 설정되었으면 개수만 증가 시킨다.
    for (DURATYPE same_reward_index = 0; 
        same_reward_index < number_of_kinds_; 
        ++same_reward_index)
    {
        const RewardItemCode same_reward_item_code = item_code_list_[same_reward_index];
        if (same_reward_item_code == item_reward.item_code)
        {
            ++(item_num_list_[same_reward_index]);
            return true;
        }
    }
    item_code_list_[number_of_kinds_] = item_reward.item_code;
    item_num_list_[number_of_kinds_] = item_reward.number_of_item;
    if (item_reward.ig_type != KindOfItemType::Default)
    {
        ig_type_list_[number_of_kinds_] = item_reward.ig_type;
        is_use_ig_type_ = true;
    }
    if (item_reward.zard_option != kUnused)
    {
        zard_option_list_[number_of_kinds_] = item_reward.zard_option;
        is_use_zard_option_ = true;
    }
    ++number_of_kinds_;
    return true;
}

RC::eITEM_RESULT
ItemCompositionOrDecompositionReward::ItemCodeReward::ObtainToInventory(\
    const int log_type, INVENTORY_TOTAL_INFO& OUT added_inventory_items)
{
    if (number_of_kinds_ == 0)
    {
        return RC::RC_ITEM_SUCCESS;
    }
    ItemManager* const item_manager = requester_->GetItemManager();
    if (item_manager->ValidState() == FALSE)
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    SCSlotContainer* const inventory = item_manager->GetItemSlotContainer(SI_INVENTORY);
    if (inventory->GetEmptySlotNum() < number_of_kinds_) 
    {
        return RC::RC_ITEM_HAVENOTSPACE;
    }
    nsSlot::ItemQuery item_query;
    if (!nsSlot::ItemQuery::MakeQueryInverseSelect(&item_query, requester_, SI_INVENTORY)) 
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    // NOTE: f110530.6L, 'obtain_result' naming is good that this word is not long winded.
    // furthermore, it is judge by inference
    // that the result of the obtain process of the item composition
    // because this function is the item composition is already self-described.
    // but, according to coding style custom, it is also acceptable to use the name of 'item_result'
    const RC::eITEM_RESULT obtain_result = item_manager->ObtainAllKindOfItemShared(
        &item_query,
        number_of_kinds_,
        item_code_list_, 
        item_num_list_,
        NULL,
        (is_use_ig_type_ ? ig_type_list_ : NULL), 
        NULL, 
        NULL, 
        (is_use_zard_option_ ? zard_option_list_ : NULL));
    if (obtain_result != RC::RC_ITEM_SUCCESS) 
    {
        return obtain_result;
    }
    item_query.MakeSerializedStream(&added_inventory_items);
    // 로그를 기록한다.
    if (log_type != kDoNotUseLog)
    {
        const nsSlot::ItemQuery::ResultInfo& query_results = item_query.result_info();
        const int max_number_of_results = _countof(query_results.nodes);
        if (query_results.number_of_results > max_number_of_results)
        {
            return RC::RC_ITEM_SUCCESS;
        }
        int number_of_remain_results = query_results.number_of_results;
        for (int i = 0; i < max_number_of_results; ++i)
        {
            if (number_of_remain_results == 0)
            {
                break;
            }
            const nsSlot::ItemQuery::ResultInfo::Node& query_result = query_results.nodes[i];
            if (query_result.container_item_code == 0 || query_result.number_of_selected_items == 0)
            {
                continue;
            }
            if (query_result.update_guide & query_result.eUpdateGuide_Insert)
            {
                // 인벤에 새로 생성된 아이템만 로그를 남긴다.
                const SCItemSlot& item_slot = 
                    static_cast<const SCItemSlot&>(inventory->GetSlot(query_result.position));
                GAMELOG->LogItem(log_type, requester_, &item_slot, 0, item_slot.GetNum());
            }
            --number_of_remain_results;
        }
    }
    return RC::RC_ITEM_SUCCESS;
}

bool ItemCompositionOrDecompositionReward::ItemCodeReward::AddToHistory(\
    ItemCompositionOrDecompositionHistory& OUT history) const
{
    for (POSTYPE i = 0; i < number_of_kinds_; ++i)
    {
        ItemCompositionOrDecompositionHistory::UsedItem used_item;
        {
            used_item.is_material = false;
            used_item.item_code = item_code_list_[i];
            used_item.number_of_item = item_num_list_[i];
            used_item.ig_type = ig_type_list_[i];
        }
        if (!history.AddUsedItem(used_item))
        {
            return false;
        }
    }
    return true;
}

bool ItemCompositionOrDecompositionReward::RewardCodeReward::Add(const RewardCode reward_code)
{
    if (reward_code == kUnused)
    {
        return false;
    }
    reward_code_ = reward_code;
    return true;
}

RC::eITEM_RESULT ItemCompositionOrDecompositionReward::RewardCodeReward::ObtainToInventory(\
    const int log_type, INVENTORY_TOTAL_INFO& OUT added_inventory_items)
{
    __UNUSED(log_type);
    if (reward_code_ == kUnused)
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    const RC::eREWARD_RESULT reward_result = g_RewardManager.RewardItem(
        RewardManager::eREWARD_COMPOSE, 
        reward_code_, 
        0, 
        requester_, 
        added_inventory_items);
    if (reward_result != RC::RC_REWARD_SUCCESS)
    {
        return RC::RC_ITEM_HAVENOTSPACE;
    }
    g_RewardManager.GetRecentObtainedResult(obtained_result_);
    return RC::RC_ITEM_SUCCESS;
}

bool ItemCompositionOrDecompositionReward::RewardCodeReward::AddToHistory(\
    ItemCompositionOrDecompositionHistory& OUT history) const
{
    for (POSTYPE i = 0; i < obtained_result_.number_of_kinds; ++i)
    {
        ItemCompositionOrDecompositionHistory::UsedItem used_item;
        {
            used_item.is_material = false;
            used_item.item_code = obtained_result_.item_code_list[i];
            used_item.number_of_item = obtained_result_.item_num_list[i];
            used_item.ig_type = obtained_result_.kind_of_item_list[i];
        }
        if (!history.AddUsedItem(used_item))
        {
            return false;
        }
    }
    return true;
}

// 초기화 한다.
bool ItemCompositionOrDecompositionReward::Init(Player* const requester, const RewardType reward_type)
{
    if (requester == NULL || reward_type == kUndefinedReward)
    {
        return false;
    }
    RewardInterface* const new_reward = RewardInterface::CreateInstance(reward_type);
    if (new_reward == NULL)
    {
        return false;
    }
    reward_.reset(new_reward);
    reward_->Init(requester);
    reward_type_ = reward_type;
    return true;
}

// 아이템 코드 보상을 추가한다.
bool ItemCompositionOrDecompositionReward::AddItemReward(const ItemReward& item_reward)
{
    if (reward_type_ != kItemCodeReward)
    {
        return false;
    }
    ItemCodeReward* const item_code_reward = reward_->GetConcrete<ItemCodeReward>();
    if (item_code_reward == NULL)
    {
        return false;
    }
    return item_code_reward->Add(item_reward);
}

// 보상 코드 보상을 추가한다.
bool ItemCompositionOrDecompositionReward::AddReward(const RewardCode reward_code)
{
    if (reward_type_ != kRewardCodeReward)
    {
        return false;
    }
    RewardCodeReward* const reward_code_reward = reward_->GetConcrete<RewardCodeReward>();
    if (reward_code_reward == NULL)
    {
        return false;
    }
    return reward_code_reward->Add(reward_code);
}

// 인벤토리에 보상을 얻는다.
RC::eITEM_RESULT ItemCompositionOrDecompositionReward::ObtainRewardToInventory(\
    const int log_type, INVENTORY_TOTAL_INFO& OUT added_inventory_items)
{
    added_inventory_items.m_InvenCount = 0;
    added_inventory_items.m_TmpInvenCount = 0;
    if (!reward_)
    {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    return reward_->ObtainToInventory(log_type, added_inventory_items);
}

// 합성 또는 분해 내역에 보상 정보를 추가한다.
bool ItemCompositionOrDecompositionReward::AddToHistory(\
    ItemCompositionOrDecompositionHistory& OUT history) const
{
    if (!reward_)
    {
        return false;
    }
    return reward_->AddToHistory(history);
}

// 인벤토리로 얻은 보상이 있는지 검사한다.
bool ItemCompositionOrDecompositionReward::IsExistObtainedToInventory() const
{
    if (!reward_)
    {
        return false;
    }
    return reward_->IsExistObtainedToInventory();
}

//==================================================================================================

}}; //end of namespace util::internal