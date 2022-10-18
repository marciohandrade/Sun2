#include "stdafx.h"
#include "./ItemManager_ItemFunction.h"
#include "ItemManager.h"

#include "LotteryInfoParser.h"
#include "ItemTypeList.h"
#include "DropRatioInfoList.h"

#include "InventorySlotContainer.h"
#include "AchievementManager.h"

#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "NotificationInfoParser.h"
#include "GameZone.h"
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM

//==================================================================================================
namespace nsSlot {
;

// NOTE: sub-manager is implemented to decompose ItemManager.
// this object has not extra members and virtual function table except the instance of ItemManager.
BOOST_STATIC_ASSERT(sizeof(nsSlot::ItemFunctionLottery) == sizeof(ItemManager*) * 2);

}; //end of namespace

//==================================================================================================

// CHANGES: f110611.1L, moved from ItemManager_Use.cpp
// 고정 타입 아이템 생성
// 예외조건은 위에서 다 처리하였다.
SCItemSlot* nsSlot::ItemFunctionLottery::MakeItemByLotteryInfo(
    const MiniSlotInfo& mini_slot_info, BOOL random, SCItemSlot* const working_slot)
{
    static SCItemSlot item_slot_static;
    SCItemSlot& making_item_slot = working_slot ? *working_slot : item_slot_static;
    making_item_slot.Clear(); // should be first call before a 'SetCode' call
    //
    bool is_overlapping_item = false;
    DWORD number_of_items = 0;
    DWORD item_type_index = 0;
    SLOTCODE item_code = 0;

    //---------------------------------------------------------------------------------------------
    if (random)
    {
        DropRatioInfoList* const drop_parser = DropRatioInfoList::Instance();
        DropObj* drop_object = NULL;
        for (int retry_count = 5; retry_count > 0; --retry_count)
        {
            drop_object = drop_parser->PopRandomItemTypeIndex(mini_slot_info.Index);
            if (drop_object == NULL || (drop_object->GetType() != DROP_OBJ_ITEM)) {
                drop_object = NULL;
                continue;
            };
            break;
        }
        if (drop_object == NULL)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|Msg=can't find the drop info (%d)"),
                   mini_slot_info.Index);
            return NULL;
        };
        // success
        DropItemObj* drop_item_obj = static_cast<DropItemObj*>(drop_object);
        // WARNING: ... problem point
        item_code = static_cast<SLOTCODE>(drop_item_obj->GetItemCode());
        making_item_slot.SetCode(item_code);
        // a default value of 'GetNum' is 1.
        is_overlapping_item = (making_item_slot.IsOverlap() != false);
        if (is_overlapping_item) {
            number_of_items = drop_item_obj->GetReserved();
            item_type_index = 0;
        }
        else {
            number_of_items = 1;
            item_type_index = drop_item_obj->GetReserved();
        };

        // added logic to support _NA_0_20110318_ITEM_EXPIRATION_PERIOD
        const BYTE group_code = drop_item_obj->GetGroupCode();
        const bool apply_time = ((group_code & (DROP_GROUP::LIMIT_DATE|DROP_GROUP::LIMIT_TIME)) != 0);
        if (apply_time)
        {
            const nsSlot::ItemRuleInfo& drop_item_rule_info = making_item_slot.GetItemRuleInfo();
            if (drop_item_rule_info.is_valid_item == 0) 
            {
                SUNLOG(eCRITICAL_LOG, TEXT("[ItemFunctionLottery]drop_item_code|%u|Try to invalid item drop"), item_code);
                return NULL;
            };

            if (making_item_slot.ChangeItemToUsingTimer() == false)
            {
                SUNLOG(eCRITICAL_LOG, _T("[ItemFunctionLottery]ItemCode|%u|failed apply timer to drop item"),
                    item_code);
                return NULL;
            };

            DWORD drop_item_reserved = drop_item_obj->GetReserved();
            switch (drop_item_rule_info.charge_sub_type)
            {
            case eCHARGE_SUB_FIXED_AMOUNT:
            case eCHARGE_SUB_FIXED_AMOUNT_EX:
            case eCHARGE_SUB_FIXED_QUANTITY:
            case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
            case eCHARGE_SUB_FATE:
            case eCHARGE_SUB_COORDINATE_EX:
            case eCHARGE_SUB_RIDER_WITH_PART:
            case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
            case eCHARGE_SUB_FIXED_AMOUNT_EX2:
                if (group_code & DROP_GROUP::LIMIT_TIME)
                {
                    // 분단위 적용
                    making_item_slot.SetDateTime(drop_item_reserved * 60);
                }
                else if (group_code & DROP_GROUP::LIMIT_DATE)
                {
                    //날짜 YYYYMMDDHHMMSS 값을 맞추기 위해 1000000을 곱한다.
                    DWORD64 d = (DWORD64)drop_item_reserved*1000000;
                    making_item_slot.SetFixedDateTime(d);
                };
                // CHANGES: f110603.4L, set default value about drop_item_reserved
                drop_item_reserved = 0;
                break;
            }
        }; // apply time
    }
    else
    {   
        if (0 == mini_slot_info.Index)
        {
            // 랜덤이 아닐때는 item_code Index는 0이 될 수 없다. 
            // 스크립트 로드할 때 체크한다. 0이면 크리티컬 에러
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=invalid input value 'Index(%d)'|"),
                   mini_slot_info.Index);
            return NULL;
        }

        assert(SAFE_NUMERIC_TYPECAST(DWORD, mini_slot_info.Index, SLOTCODE));
        item_code = static_cast<SLOTCODE>(mini_slot_info.Index);
        making_item_slot.SetCode(item_code);
        //
        is_overlapping_item = (making_item_slot.IsOverlap() != false);
        if (is_overlapping_item) {
            number_of_items = mini_slot_info.ItemTypeIndex;
            item_type_index = 0;
        }
        else {
            number_of_items = 1;
            item_type_index = mini_slot_info.ItemTypeIndex;
        };
    };
    //---------------------------------------------------------------------------------------------
    if (is_overlapping_item)
    {
        making_item_slot.SetNum(static_cast<DURATYPE>(number_of_items));
    }
    else if (const ItemType* item_type = ItemTypeList::Instance()->FindItemType(item_type_index))
    {
        // default : SetNum(1)
        const nsSlot::ItemRuleInfo& item_rule_info = making_item_slot.GetItemRuleInfo();
        if (item_rule_info.IsEnableEnchant()) {
            making_item_slot.SetEnchant(item_type->GetEnchant());
        };
    #ifdef _NA_000251_20100727_SOCKET_SYSTEM
        // EP2에서는 아이템 타입 정보의 랭크 항목을 소켓 개수로 사용한다.
        const BYTE number_of_socket = item_type->GetRank();
        if (number_of_socket != 0 && item_rule_info.IsEnableSocket())
        {
            if (making_item_slot.CreateSocketItem(number_of_socket, false) == false)
            {
                SUNLOG(eCRITICAL_LOG, 
                        _T("|%s|소켓 개수 설정에 실패했습니다.|아이템코드 = %u, 소켓개수 = &u"), 
                        __FUNCTION__, 
                        item_code, 
                        number_of_socket);
            }
        };
    #else
        if (item_rule_info.IsEnableRank())
        {
            making_item_slot.SetRank(RANK_E);
            BYTE rank = item_type->GetRank();
            for (INT RankIndex = RANK_E ; RankIndex < rank ; ++RankIndex)
                ItemManager::ItemRankUp(&making_item_slot);
        };
    #endif // _NA_000251_20100727_SOCKET_SYSTEM
        nsSlot::ItemTypeChanger::ChangeItemByType(&making_item_slot, item_type->GetItemType());
    }
    else
    {
        // is_overlapping_item == false && can't find 'ItemType'
        // default item
    };

    return &making_item_slot;
}

// CHANGES: f110614.3L, logic arrangement, f110611.1L, moved from ItemManager_Use.cpp
//판도라의 상자를 연다.
RC::eITEM_RESULT nsSlot::ItemFunctionLottery::PandoraBoxOpen(
    POSTYPE lottery_item_pos, ITEMSTREAMEX* const result_stream)
{
    if (root_->player_->GetBehave() != PLAYER_BEHAVE_IDLE_STATE) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    if (root_->ValidPos(SI_INVENTORY, lottery_item_pos) == false) {
        return RC::RC_ITEM_INVALIDPOS;  
    };
    CInventorySlotContainer* const inventory = \
        static_cast<CInventorySlotContainer*>(root_->GetItemSlotContainer(SI_INVENTORY));
    SCItemSlot& lottery_item = static_cast<SCItemSlot&>(inventory->GetSlot(lottery_item_pos));
    if (const nsSlot::ItemRuleInfo* item_rule_info = &lottery_item.GetItemRuleInfo()) {
        if (item_rule_info->is_valid_item == false ||
            item_rule_info->is_lottery == false)
        {
            return RC::RC_ITEM_LOTTO_NOLOTTOITEM;
        };
    };

#ifdef _NA_0_20110803_LOTTO_ITEM_LEVEL_LIMIT
    LEVELTYPE level = lottery_item.GetItemInfo()->m_LimitUseLevel;
    if (level != 0 && root_->player_->GetLevel() < level)
        return RC::RC_ITEM_CANNOT_USE_PLAYER_LEVEL;
#endif

    SCLottoInfo* const lottery_info = lottery_item.GetLottoInfo();
    // #1. check remain slot... pass
    //if (inventory->GetEmptySlotNum() < MAX_PANDORA_SLOT_NUM) {
    //    return RC::RC_ITEM_LOTTO_NEED_EMPTY_SLOT;
    //};
    // #2. if the lottery item don't identificaiton, do identity
    if (lottery_info->IsIdentity() == false)
    {
        // item_info->m_MaterialCode -> link -> lotteryinfo.lottery_index
        const SLOTCODE material_code = lottery_item.GetItemInfo()->m_MaterialCode;
        const LotteryIndexGroup* lottery_group = \
            LotteryInfoParser::Instance()->GetLotteryIndexGroup(material_code);
        if (lottery_group == NULL)
        {
            SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't find lottery index(%d)|"),
                   __FUNCTION__, material_code);
            return RC::RC_ITEM_LOTTO_IDENTITY_FAIL;
        };
        lottery_info->SetLottoIndex(material_code);
        if (lottery_info->Identity() == false) {
            return RC::RC_ITEM_LOTTO_IDENTITY_FAIL;
        };
    };
    //
    if (lottery_item.IsPandoraOpen())
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=is it sync problem? open failed|"), __FUNCTION__);
        return RC::RC_ITEM_LOTTO_OPEN_FAIL;
    };
    // success
    root_->player_->SetBehave(PLAYER_BEHAVE_DIALOG_OPEN_STATE);
    lottery_info->SetOpenStatus(true);
    if (result_stream) {
        lottery_item.CopyOut(*result_stream);
    };
    //
    return RC::RC_ITEM_LOTTO_OPEN_SUCCESS;
}

// CHANGES: f110614.3L, logic arrangement, f110611.1L, moved from ItemManager_Use.cpp
// 판도라의 상자를 닫을 때. 
RC::eITEM_RESULT nsSlot::ItemFunctionLottery::PandoraBoxClose(POSTYPE lottery_item_pos)
{
    if (root_->player_->GetBehave() != PLAYER_BEHAVE_DIALOG_OPEN_STATE) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    if (root_->ValidPos(SI_INVENTORY, lottery_item_pos, false) == false) {
        return RC::RC_ITEM_INVALIDPOS;  
    };
    CInventorySlotContainer* const inventory = \
        static_cast<CInventorySlotContainer*>(root_->GetItemSlotContainer(SI_INVENTORY));
    SCItemSlot& lottery_item = static_cast<SCItemSlot&>(inventory->GetSlot(lottery_item_pos));
    if (lottery_item.GetItemCode() == 0) {
        // NOTE: the pointed lottery item can be already deleted when all lottery slot empty.
        root_->player_->SetBehave(PLAYER_BEHAVE_IDLE_STATE);
        return RC::RC_ITEM_PANDORABOX_CLOSE_SUCCESS;
    };
    //----------------------------------------------------------------------------------------------
    if (const nsSlot::ItemRuleInfo* item_rule_info = &lottery_item.GetItemRuleInfo()) {
        if (item_rule_info->is_valid_item == false ||
            item_rule_info->is_lottery == false)
        {
            return RC::RC_ITEM_LOTTO_NOLOTTOITEM;
        };
    };
    SCLottoInfo* const lottery_info = lottery_item.GetLottoInfo();
    // NOTE: the state of identification is already processed after the lottery item opened.
    // if the below condition is fails, that is the unexpected state.
    if (lottery_info->IsIdentity() == false) {
        return RC::RC_ITEM_PANDORABOX_CLOSE_FAIL;
    };
    if (lottery_item.IsPandoraOpen() == false) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=is it sync problem? open failed|"), __FUNCTION__);
        return RC::RC_ITEM_PANDORABOX_CLOSE_FAIL;
    };
    // success process
    root_->player_->SetBehave(PLAYER_BEHAVE_IDLE_STATE);
    lottery_info->SetOpenStatus(false);

    return RC::RC_ITEM_PANDORABOX_CLOSE_SUCCESS;
}

// CHANGES: f110613.3L, added the lottery renewal logic
RC::eITEM_RESULT nsSlot::ItemFunctionLottery::ExtractLotteryItems(bool moved_all,
    POSTYPE lottery_item_pos, POSTYPE slot_index_in_lottery, const POSTYPE empty_slot_count,
    ConcretizationResult* const concretization_result)
{
    ;{
        concretization_result->result_value = concretization_result->eResult_None;
        ZeroMemory(concretization_result->changed_lottery_item_stream,
                   sizeof(*concretization_result->changed_lottery_item_stream));
        concretization_result->total_info->m_InvenCount = 0;
        concretization_result->total_info->m_TmpInvenCount = 0;
    };
    //----------------------------------------------------------------------------------------------
    if (root_->ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    if (root_->ValidPos(SI_INVENTORY, lottery_item_pos, true) == false) {
        return RC::RC_ITEM_INVALIDPOS;
    };
    //----------------------------------------------------------------------------------------------
    CInventorySlotContainer* const inventory = static_cast<CInventorySlotContainer*>(
        root_->GetItemSlotContainer(SI_INVENTORY));
    //----------------------------------------------------------------------------------------------
    SCItemSlot& lottery_item = static_cast<SCItemSlot&>(inventory->GetSlot(lottery_item_pos));
    if (const nsSlot::ItemRuleInfo* item_rule_info = &lottery_item.GetItemRuleInfo())
    {
        if (item_rule_info->is_valid_item == false ||
            item_rule_info->is_lottery == false)
        {
            return RC::RC_ITEM_INVALIDSTATE;
        }
    };
    if (lottery_item.IsPandoraOpen() == false) {
        return RC::RC_ITEM_LOTTO_NEED_OPEN_STATE;
    };
    //
    SCLottoInfo* const lottery_info = lottery_item.GetLottoInfo();
    if (lottery_info->IsIdentity() == false) {
        return RC::RC_ITEM_LOTTO_NEED_IDENTYFY_LOTTO_ITEM;
    };

    if( empty_slot_count < lottery_info->GetCount() )
    {
        PandoraBoxClose(lottery_item_pos);
        return RC::RC_ITEM_HAVENOTSPACE;
    }

    //----------------------------------------------------------------------------------------------
    // lottery_item -> inventory, concretization
    RC::eITEM_RESULT result = RC::RC_ITEM_SUCCESS;
    if (lottery_info->GetCount() > 0) {
        result = this->AcquireItemsFromLottery(\
            moved_all, lottery_item, slot_index_in_lottery, concretization_result);
    }
    else {
        // delete
    };
    //----------------------------------------------------------------------------------------------
    if ((result != RC::RC_ITEM_SUCCESS) &&
        (result != RC::RC_ITEM_LOTTO_CONCRETIZE_SUCCESS))
    {
        return result;
    };
    //
    if (lottery_info->GetCount() > 0)
    {   // success process, concretization
        lottery_item.CopyOut(*concretization_result->changed_lottery_item_stream);
        return result;
    }
    else
    {
        // NOTE: result process with the lottery item deletion.
        // CHANGES: f110610.3L, fixed invalid item logging problem that the item is already deleted.
        // Custom comments: 현재 판도라 박스의 아이템 개수가 0이면... 복권을 삭제한다.
        SCItemSlot deleted_item_for_log;
        bool success = inventory->DeleteSlot(lottery_item_pos, &deleted_item_for_log) != false;
        if (FlowControl::FCAssert(success))
        {   // success process
            concretization_result->result_value |=
                concretization_result->eResult_RemoveLotteryItem;
            GAMELOG->LogItem(ITEM_DELETE, root_->player_, &deleted_item_for_log);
            return result;
        };
    };
    ASSERT(!"we must accomplish to success");
    // FAILED process
    result = RC::RC_ITEM_FAILED;
    if (INVENTORY_TOTAL_INFO* const total_info = concretization_result->total_info)
    {
        // NOTE: waverix hopes not to enter at this point.
        // CHANGES: f110610.3L, fixed deleting only one item
        // even though acquired items may grater than one.
        const INVENTORY_TOTAL_INFO::SLOT_TYPE* it = total_info->m_Slot,
            * const end = &total_info->m_Slot[total_info->m_InvenCount];
        for (; it != end; ++it)
        {
            if (inventory->DeleteSlot(it->m_Pos, NULL) != RC::RC_ITEM_SUCCESS)
            {
                SLOTCODE item_code = it->m_Stream.Part.wCode;
                SUNLOG(eCRITICAL_LOG,
                       _T("|[%s]|Msg=failed to rollback rewarded item(%d)|CharGuid=%d"),
                       __FUNCTION__, item_code, root_->player_->GetCharGuid());
            };
        };
    };

    return result;
}

//==================================================================================================

namespace nsSlot { namespace internal {
;

struct WorkingLotteryInfoNode
{
    POSTYPE slot_index_in_lottery;
    const MiniSlotInfo* mini_slot_info;
    const SCLottoInfo::MiniSlot* mini_slot; // selected node in SCItemSlot::LottoInfo
};

struct WorkingLotteryItemSlot
{
    uint8_t stream[sizeof(SCItemSlot)];
    SCItemSlot* CreateInstance() {
        return new (stream) SCItemSlot; // SCItemSlot need not to delete instance
    };
    SCItemSlot* GetInstance() {
        return reinterpret_cast<SCItemSlot*>(stream);
    };
};

}}; //end of namespace

//==================================================================================================
// CHANGES: f110613.3L, added the lottery renewal logic
RC::eITEM_RESULT nsSlot::ItemFunctionLottery::AcquireItemsFromLottery(
    bool moved_all, SCItemSlot& lottery_item, POSTYPE slot_index_in_lottery,
    ConcretizationResult* const concretization_result)
{
    //
    SCLottoInfo* lottery_info = lottery_item.GetLottoInfo();
    if (FlowControl::FCAssert(lottery_info->GetCount() > 0) == false) {
        return RC::RC_ITEM_SUCCESS; // for the empty lottery item deletion
    };
    const LotteryIndexGroup* lottery_index_group = NULL;
    if (LotteryInfoParser* lottery_parser = LotteryInfoParser::Instance())
    {
        const uint16_t lottery_index = lottery_info->GetLottoIndex();
        lottery_index_group = lottery_parser->GetLotteryIndexGroup(lottery_index);
        if (lottery_index_group == NULL)
        {
            SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=can't find lottery index(%d)|"),
                   lottery_index);
            return RC::RC_ITEM_LOTTO_INVALID_LOTTOSCRIPT_INDEX;
        }
    };

    nsSlot::internal::WorkingLotteryItemSlot working_slots[MAX_PANDORA_SLOT_NUM];
    nsSlot::internal::WorkingLotteryInfoNode working_infos[MAX_PANDORA_SLOT_NUM] = { 0, };
    int number_of_processing = 0;
    if (moved_all)
    {
        for (uint8_t index = 0; index < _countof(working_infos); ++index)
        {
            const SCLottoInfo::MiniSlot* const mini_slot = &lottery_info->GetMiniSlot(index);
            if (mini_slot->ExistItem == 0) {
                continue;
            };
            // this mini_slot can handle the invalid lotto slot that is filled in zero.
            // be basically selected zero index(1st).
            const MiniSlotInfo* mini_slot_info =
                lottery_index_group->GetMiniSlotInfo(mini_slot->InfoPos);
            if (mini_slot_info == NULL) {
                continue;
            };
            nsSlot::internal::WorkingLotteryInfoNode* info_node = &working_infos[number_of_processing];
            ;{
                info_node->slot_index_in_lottery = index;
                info_node->mini_slot = mini_slot;
                info_node->mini_slot_info = mini_slot_info;
            };
            SCItemSlot* making_slot = working_slots[number_of_processing].CreateInstance();
            making_slot = MakeItemByLotteryInfo(*info_node->mini_slot_info,
                info_node->mini_slot->Random, making_slot);
            if (making_slot == NULL) {
                continue;
            };
            ++number_of_processing;
        };
    }
    else
    {
        const SCLottoInfo::MiniSlot* const mini_slot = \
            &lottery_info->GetMiniSlot(slot_index_in_lottery);
        if (mini_slot->ExistItem == 0) {
            return RC::RC_ITEM_NOTEXIST_ITEM;
        };
        const MiniSlotInfo* mini_slot_info =
            lottery_index_group->GetMiniSlotInfo(mini_slot->InfoPos);
        if (mini_slot_info == NULL) {
            return RC::RC_ITEM_NOTEXIST_ITEM;
        };
        nsSlot::internal::WorkingLotteryInfoNode* info_node = &working_infos[number_of_processing];
        ;{
            info_node->slot_index_in_lottery = slot_index_in_lottery;
            info_node->mini_slot = mini_slot;
            info_node->mini_slot_info = mini_slot_info;
        };
        SCItemSlot* making_slot = working_slots[number_of_processing].CreateInstance();
        making_slot = MakeItemByLotteryInfo(*info_node->mini_slot_info,
            info_node->mini_slot->Random, making_slot);
        if (making_slot != NULL) {
            ++number_of_processing;
        };
    };
    if (number_of_processing == 0) {
        return RC::RC_ITEM_NOTEXIST_ITEM;
    };
    //----------------------------------------------------------------------------------------------
    nsSlot::ItemQuery select_suplus_query;
    if (nsSlot::ItemQuery::MakeQueryInverseSelect(&select_suplus_query,
            root_->player_, SI_INVENTORY) == false)
    {
        return RC::RC_ITEM_INVALIDSTATE;
    };

    for (uint8_t index = 0; index < number_of_processing; ++index)
    {
        SCItemSlot* making_slot = working_slots[index].GetInstance();

        //bool is_overlapping_item = (making_slot->IsOverlap() != false);
        const KindOfItemTag& lotto_tag = lottery_item.GetOptionTag();
        // f100910.1L, f101101.2L, a making_item can inherit an own original tradable attribute
        // by 'SetCode' event, though a parent item (it is a pandora box) has't a tradable attribute
        if (lotto_tag.value == lotto_tag.Tag_TradeSkip) {
            making_slot->CopyFromInheritableOption(lottery_item);
        };
        DURATYPE obtainable_number_of_items = making_slot->GetNum();
        select_suplus_query.AddArg_ItemSlot(*making_slot, obtainable_number_of_items, true);
    };
    const ItemQuery::QueryInfo& query_info = select_suplus_query.query_info();
    if (query_info.number_of_queries == 0) {
        return RC::RC_ITEM_NOTEXIST_ITEM;
    };
    //----------------------------------------------------------------------------------------------
    select_suplus_query.ChangeToFindEmptySlotOnlyMode();
    const nsSlot::ItemQueryResult result = root_->Query(&select_suplus_query);
    if (result.result_of_inverse_select == 0) {
        return RC::RC_ITEM_HAVENOTSPACE;
    };
    for (int i = 0; i < query_info.number_of_queries; ++i)
    {
        const nsSlot::ItemQuery::QueryInfo::Node& node = query_info.nodes[i];
        if (node.number_of_items != node.sum_of_items) {
            return RC::RC_ITEM_HAVENOTSPACE;
        };
    };
    for (int i = 0; i < query_info.number_of_queries; ++i)
    {
        const nsSlot::ItemQuery::QueryInfo::Node& node = query_info.nodes[i];
        lottery_info->ClearMiniSlot(working_infos[i].slot_index_in_lottery);
        // 판도라의 상자를 이용하여 아이템을 얻은 로그.
        GAMELOG->LogItemEx(ITEM_GET_BY_LOTTERY, root_->player_, *node.item_slot, lottery_item);
#ifdef _NA_001990_ACSYSTEM_ADD
        if (node.item_info->m_wACCode)
        {
            root_->player_->GetACManager()->ProcessItemObtain(
                node.item_info->m_Code, node.item_info->m_wACCode, 
                node.item_info->m_byACReferenceID, node.number_of_items);
        };
#endif //_NA_001990_ACSYSTEM_ADD
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
        if (NotificationInfoParser::Instance()->IsNotificationItem(
            *node.item_slot, eNotificationBox) == true)
        {
            MSG_DG_NOTIFICATION_ITEM_SYN msg;
            msg.info.SetInfo(root_->player_->GetCharName(),
                (root_->player_->GetGameZonePtr() != NULL) ? root_->player_->GetGameZonePtr()->GetMapCode() : 0,
                GetTickCount(), eNotificationBox, node.item_slot->GetItemCode());
            g_pGameServer->SendToSessionType(GAME_DBPROXY, &msg, sizeof(MSG_DG_NOTIFICATION_ITEM_SYN));
        }
#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
    };
    root_->ObtainAllKindOfItemByQuery(select_suplus_query);
    select_suplus_query.MakeSerializedStream(concretization_result->total_info);

    return RC::RC_ITEM_LOTTO_CONCRETIZE_SUCCESS;
}

//==================================================================================================
