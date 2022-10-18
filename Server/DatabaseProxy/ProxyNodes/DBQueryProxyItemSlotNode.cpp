#include "stdafx.h"
#include "DBQueryProxyNode.h"


#include <SCItemSlot.h>
#include <VersionControl.h>
#include <ItemInfoParser.h>

#include "QueryObjects/DBHandler.h"

//==================================================================================================
#pragma warning(push, 4)
namespace nsQuery {
;

//==================================================================================================

void ItemSlotUpdateNode::UpdateAndCompareData()
{
    if (record_changed_ != this->eRecordChange_Default) {
        return;
    };

    bool changed = commited_data_.slot_type_ != latest_data_.slot_type_ ||
                   commited_data_.pos_ != latest_data_.pos_ ||
                   commited_data_.dura_or_num_ != latest_data_.dura_or_num_ ||
                   commited_data_.version_ != latest_data_.version_ ||
                   commited_data_.code_ != latest_data_.code_ ||
                   commited_data_.serial_ != latest_data_.serial_;
    if (changed == false)
    {
        changed = ::memcmp(commited_data_.option_stream_, latest_data_.option_stream_,
                           sizeof(commited_data_.option_stream_)) != 0;
    };
    if (changed) {
        record_changed_ = this->eRecordChange_Changed;
        return;
    }
    ; // no changed

    return;
}

bool ItemSlotVersionChecker::IsAcceptableItemSlot(SLOTIDX slot_type, ItemSlotVersionChecker* result)
{
    ZeroMemory(result, sizeof(*result));
    uint32_t making_mask = (1 << slot_type);
    result->is_char_slot_type_ = (kCharSlotTypeMask & making_mask) != 0;
    result->is_user_slot_type_ = (result->is_char_slot_type_ == false) &&
                                 (kUserSlotTypeMask & making_mask) != 0;
    result->is_unique_slot_type_ = (result->is_char_slot_type_ == false) &&
                                   (result->is_user_slot_type_ == false) &&
                                   (kUniqueSlotTypeMask & making_mask) != 0;

    return (result->is_char_slot_type_ ||
            result->is_user_slot_type_ ||
            result->is_unique_slot_type_);
}

void ItemSlotVersionChecker::SetWorldType(ITEMPART* item_part)
{
    // NOTE: make to clear stream because a game server may send the item stream with garbage data
    item_part->byReserved1 = 0;
    item_part->byReserved2 = 0;
    if (item_part->byType > 200) {
        item_part->byType = g_DBProxyServer.GetItemWorldID();
    }
}

bool ItemSlotVersionChecker::ConvertItem(const ItemSlotResultRecord& from_record,
                                         ItemSlotResultRecord* const original_record,
                                         ItemSlotResultRecord* const converted_record)
{
    //assert='a original_record and converted_record are initialized.'
    *original_record = from_record;
    //
    if (from_record.code_ == 0)
    {
        original_record->version_ = ITEM_STREAM_SIZE_LAST_VERSION; // for no update
        //
        converted_record->code_ = 0;
        converted_record->version_ = ITEM_STREAM_SIZE_LAST_VERSION;
        return true;
    };
    // NOTE: do clear stream if it can't find the item script
    // that is an item information extract from database.
    const ItemInfoParser* item_parser = ItemInfoParser::Instance();
    const BASE_ITEMINFO* item_info = item_parser->GetItemInfo(from_record.code_);
    if (item_info == NULL) {
        converted_record->code_ = 0;
        converted_record->version_ = ITEM_STREAM_SIZE_LAST_VERSION;
        return true;
    }
    ITEMPART item_part = { 0, };
    item_part.m_WorldSerial = from_record.serial_;
    item_part.wCode = static_cast<SLOTCODE>(from_record.code_);
    item_part.byDura = static_cast<DURATYPE>(from_record.dura_or_num_);
    //
    ItemSlotVersionChecker::SetWorldType(&item_part);
    if (from_record.version_ == ITEM_STREAM_SIZE_LAST_VERSION)
    {
        *converted_record = from_record;
        converted_record->serial_ = item_part.m_WorldSerial;
        return true;
    };
    //
    ITEMSTREAMEX result_stream;
    int converted_version = 0;
    ;{
        ITEMSTREAMEX item_stream;
        item_stream.Part = item_part;
        CopyMemory(&item_stream.OptionPart, from_record.option_stream_,
                   sizeof(item_stream.OptionPart));

        FlowControl::FCAssert(VersionControl::ConvertItem(&converted_version, &result_stream,
                                                          from_record.version_, &item_stream));
    };
    if (SLOTCODE item_code = result_stream.Part.wCode)
    {
        const ITEMPART& result_item_part = result_stream.Part;
        converted_record->dura_or_num_ = result_item_part.byDura;
        converted_record->version_ = static_cast<uint8_t>(converted_version);
        converted_record->code_ = result_item_part.wCode;
        converted_record->serial_ = result_item_part.m_WorldSerial;
        CopyMemory(converted_record->option_stream_, &result_stream.OptionPart,
                   sizeof(converted_record->option_stream_));
    }
    else
    {
        converted_record->code_ = 0;
        converted_record->version_ = ITEM_STREAM_SIZE_LAST_VERSION;
        return true;
    }
    //
    return true;
}

bool ItemSlotVersionChecker::ConvertItem(const ITEMSTREAMEX& from_stream,
                                         ItemSlotResultRecord* converted_record)
{
    ITEMPART item_part = from_stream.Part;
    SLOTCODE item_code = item_part.wCode;
    if (item_code == 0)
    {
        converted_record->code_ = 0;
        converted_record->version_ = ITEM_STREAM_SIZE_LAST_VERSION;
        return true;
    };
    ItemSlotVersionChecker::SetWorldType(&item_part);
    converted_record->dura_or_num_ = item_part.byDura;
    converted_record->version_ = ITEM_STREAM_SIZE_LAST_VERSION;
    converted_record->code_ = item_code; //item_part.wCode;
    converted_record->serial_ = item_part.m_WorldSerial;
    CopyMemory(converted_record->option_stream_, &from_stream.OptionPart,
               sizeof(converted_record->option_stream_));
    //
    return true;
}

void ItemSlotUpdateTableBase::_OperateAll(int operation,
                                          ItemSlotUpdateNode* record_array,
                                          size_t number_of_records,
                                          SLOTIDX slot_index)
{
    ItemSlotUpdateNode* update_it = record_array;
    const ItemSlotUpdateNode* const update_end = &record_array[number_of_records];
    for (int pos = 0; update_it != update_end; ++update_it, ++pos)
    {
        if (operation & eOper_ClearAllToDefault)
        {
            ItemSlotResultRecord* const record = &update_it->latest_data_;
            //
            ZeroMemory(record, sizeof(*record));
            record->slot_type_ = slot_index;
            record->pos_ = static_cast<POSTYPE>(pos + 1);
            //record->dura_or_num_ = 0;
            record->version_ = ITEM_STREAM_SIZE_LAST_VERSION;
            //record->code_ = 0;
            //record->serial_;
            //record->option_stream_;
        };
        if (operation & eOper_CopyAllLatestDataToCommitedData)
        {
            update_it->commited_data_ = update_it->latest_data_;
        }
    }
}

//==================================================================================================

bool DBQueryProxyItemSlotNode::StoreItemSlotInfo(SLOTIDX slot_type,
    const ITEMSLOTEX_INSERVER* data_list, const POSTYPE number_of_slots,
    ItemSlotUpdateNode* const update_node_target, const POSTYPE max_number_of_slots)
{
    __UNUSED(slot_type);
    //
    const ITEMSLOTEX_INSERVER* data_it = data_list,
                             * const data_end = data_list + number_of_slots;
    for ( ; data_it != data_end; ++data_it)
    {
        POSTYPE pos = data_it->m_Pos;
        if (pos >= max_number_of_slots) {
            continue;
        }
        ItemSlotUpdateNode* const update_node = &update_node_target[pos];
        ItemSlotResultRecord* const latest_data = &update_node->latest_data_;

        
        ITEMPART item_part = data_it->m_Stream.Part;
        // NOTE: special operation. make to marked serial key.
        ItemSlotVersionChecker::SetWorldType(&item_part); // assert(db_user)
        assert(latest_data->pos_ == pos + 1); // for db index
        latest_data->dura_or_num_ = item_part.byDura;
        latest_data->code_ = item_part.wCode;
        latest_data->serial_ = item_part.m_WorldSerial;
        BOOST_STATIC_ASSERT(sizeof(latest_data->option_stream_) ==
                            sizeof(data_it->m_Stream.OptionPart));
        CopyMemory(latest_data->option_stream_, &data_it->m_Stream.OptionPart,
                   sizeof(latest_data->option_stream_));
        update_node->UpdateAndCompareData();
    }
    return true;
}

bool DBQueryProxyItemSlotNode::LoadItemSlotInfo(SLOTIDX slot_type,
    ITEMSLOTEX_INSERVER* data_list, POSTYPE* result_number_of_slots,
    const ItemSlotUpdateNode* const update_node_target, const POSTYPE max_number_of_slots)
{
    __UNUSED(slot_type);
    ITEMSLOTEX_INSERVER* data_it = data_list;
    const ItemSlotUpdateNode* update_it = update_node_target,
                            * const update_end = update_node_target + max_number_of_slots;
    for (int pos = 0; update_it != update_end; ++update_it, ++pos)
    {
        const ItemSlotResultRecord& latest_data = update_it->latest_data_;
        if (latest_data.code_ == 0) {
            continue;
        }
        ITEMPART item_part = { 0, };
        item_part.m_WorldSerial = latest_data.serial_;
        item_part.wCode = static_cast<SLOTCODE>(latest_data.code_);
        item_part.byDura = static_cast<DURATYPE>(latest_data.dura_or_num_);
        //
        assert(latest_data.pos_ == pos + 1);
        data_it->m_Pos = static_cast<POSTYPE>(pos);
        data_it->m_Stream.Part = item_part;
        CopyMemory(&data_it->m_Stream.OptionPart, latest_data.option_stream_,
                   sizeof(data_it->m_Stream.OptionPart));
        //
        ++data_it;
    }
    //
    *result_number_of_slots = static_cast<POSTYPE>(data_it - data_list);
    return true;
}

bool DBQueryProxyItemSlotNode::DBResult_Select(const Query_ItemSlotInfo_Select& query_result,
                                               ItemSlotUpdateNode* const update_node_target,
                                               const POSTYPE max_number_of_slots)
{
    if (query_result.IsError()) {
        return false;
    }

    // NOTE: result of the Query_ItemSlotInfo_Select skipped an empty slot,
    // so that it is possible that a value of number_of_rows is zero.
    int number_of_rows = query_result.GetResultRowNum();
    assert(number_of_rows <= max_number_of_slots);
    ;   number_of_rows = min(number_of_rows, max_number_of_slots);
    //
    for (int i = 0; i < number_of_rows; ++i)
    {
        const Query_ItemSlotInfo_Select::sQueryResult& record = query_result.result_records_[i];
        //!~
        if (record.pos_ <= 0 || record.pos_ > max_number_of_slots) {
            SUNLOG(eCRITICAL_LOG, "[%s|%d] record.pos_(%d) >= max_number_of_slots(%d)", 
                __FUNCTION__, __LINE__, record.pos_, max_number_of_slots);
            continue;
        }
        //!~
        ItemSlotUpdateNode* const update_node = &update_node_target[record.pos_ - 1];
        // CHANGES: f101116.2L, fixed a problem. even though need update
        // if an inputed record is converted, it is not updated.
        ItemSlotVersionChecker::ConvertItem(record,
            &update_node->commited_data_, &update_node->latest_data_);
        //
        ITEMPART item_part = { 0, };
        if (ItemSlotResultRecord* latest_data = &update_node->latest_data_)
        {
            item_part.m_WorldSerial = latest_data->serial_;
            // NOTE: special operation. make to marked serial key.
            ItemSlotVersionChecker::SetWorldType(&item_part); // assert(db_user)
            latest_data->serial_ = item_part.m_WorldSerial;
        }
    }
    return true;
}

bool DBQueryProxyItemSlotNode::DBResult_Update(const Query_ItemSlotInfo_Update& query_result,
                                               ItemSlotUpdateNode* const update_node_target,
                                               const POSTYPE max_number_of_slots)
{
    __UNUSED(max_number_of_slots);
    const Query_ItemSlotInfo_Update::sQueryParam& parameters = query_result.parameters_[0];
    ItemSlotUpdateNode* const update_node = update_node_target + (parameters.pos_ - 1);
    ASSERT(update_node->query_ == &query_result &&
           update_node->record_changed_ == update_node->eRecordChange_Changed);
    update_node->record_changed_ = update_node->eRecordChange_Default;
    update_node->query_ = NULL; // external delete control
    //
    if (query_result.IsError()) {
        return false;
    }
    update_node->commited_data_ = parameters;
    //
    return true;
}

//==================================================================================================

// G->D update stream
void DBQueryProxyNode::CharacterNode::StoreItemSlotInfo(SLOTIDX slot_type,
                                                        const ITEMSLOTEX_INSERVER* data_list,
                                                        const POSTYPE number_of_slots)
{
    ItemSlotVersionChecker version_check;
    bool accept_version = ItemSlotVersionChecker::IsAcceptableItemSlot(slot_type, &version_check);
    if (FlowControl::FCAssert(accept_version) == false) {
        return;
    }
    ItemSlotUpdateNode* update_node_target = NULL;
    POSTYPE max_number_of_slots = 0;
    switch (slot_type)
    {
    case inven_table_.kSlotIndex:
        update_node_target = inven_table_.records_;
        max_number_of_slots = inven_table_.kMaxNumberOfSlots;
        inven_table_._OperateAll(inven_table_.eOper_ClearAllToDefault,
                                 inven_table_.records_,
                                 inven_table_.kMaxNumberOfSlots,
                                 inven_table_.kSlotIndex);
        break;
    case equip_table_.kSlotIndex:
        update_node_target = equip_table_.records_;
        max_number_of_slots = equip_table_.kMaxNumberOfSlots;
        equip_table_._OperateAll(equip_table_.eOper_ClearAllToDefault,
                                 equip_table_.records_,
                                 equip_table_.kMaxNumberOfSlots,
                                 equip_table_.kSlotIndex);
        break;
    default:
        __debugbreak();
        return;
    };
    //
    DBQueryProxyItemSlotNode::StoreItemSlotInfo(slot_type, data_list, number_of_slots,
                                                update_node_target, max_number_of_slots);
    //
    if (slot_type == inven_table_.kSlotIndex)
    {
        static SCItemSlot temp_item_slot;
        BASE_PLAYERINFO* player_info = db_character_->GetInfo();
        bool is_pet_summoned = (player_info->m_bSummonedPet != 0);
        switch (is_pet_summoned)
        {
        case true:
            {
                is_pet_summoned = false;
                if (player_info->m_PetItemPos >= inven_table_.kMaxNumberOfSlots) {
                    break;
                }
                const ItemSlotResultRecord* const pet_record = \
                    &inven_table_.records_[player_info->m_PetItemPos].latest_data_;
                if (pet_record->code_ == 0) {
                    break;
                }
                ZeroMemory(&pet_item_stream_, sizeof(pet_item_stream_));
                ITEMPART* item_part = &pet_item_stream_.Part;
                item_part->wCode = static_cast<SLOTCODE>(pet_record->code_);
                item_part->byDura = static_cast<DURATYPE>(pet_record->dura_or_num_);
                item_part->m_WorldSerial = pet_record->serial_;
                CopyMemory(&pet_item_stream_.OptionPart, pet_record->option_stream_,
                           sizeof(pet_item_stream_.OptionPart));

                SCItemSlot& pet_item = temp_item_slot;
                pet_item.Copy(pet_item_stream_);
                if (pet_item.IsPetItem() == false || pet_item.IsUserBlocked() == false) {
                    break;
                }
                is_pet_summoned = true;
            }
            break;
        };
        if (is_pet_summoned == false)
        {
            player_info->m_bSummonedPet = false;
            player_info->m_bMAXIntimacy = 0;
            player_info->m_PetItemPos = INVALID_POSTYPE_VALUE;
            ZeroMemory(&pet_item_stream_, sizeof(pet_item_stream_));
        }
    }
}

// D->G send stream
void DBQueryProxyNode::CharacterNode::LoadItemSlotInfo(SLOTIDX slot_type,
                                                       ITEMSLOTEX_INSERVER* data_list,
                                                       POSTYPE* result_number_of_slots) const
{
    *result_number_of_slots = 0;
    ItemSlotVersionChecker version_check;
    bool accept_version = ItemSlotVersionChecker::IsAcceptableItemSlot(slot_type, &version_check);
    if (FlowControl::FCAssert(accept_version) == false) {
        return;
    }
    const ItemSlotUpdateNode* update_node_target = NULL;
    POSTYPE max_number_of_slots = 0;
    switch (slot_type)
    {
    case inven_table_.kSlotIndex:
        update_node_target = inven_table_.records_;
        max_number_of_slots = inven_table_.kMaxNumberOfSlots;
        break;
    case equip_table_.kSlotIndex:
        update_node_target = equip_table_.records_;
        max_number_of_slots = equip_table_.kMaxNumberOfSlots;
        break;
    default:
        __debugbreak();
        return;
    };
    //
    DBQueryProxyItemSlotNode::LoadItemSlotInfo(slot_type, data_list, result_number_of_slots,
                                               update_node_target, max_number_of_slots);
}

// extra command
void DBQueryProxyNode::CharacterNode::LoadPetItemStream(ITEMSTREAMEX* result_pet_stream) const
{
    //assert(shared_.state & this->eState_Lo
    CopyMemory(result_pet_stream, &pet_item_stream_, sizeof(*result_pet_stream));
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_ItemSlotInfo_Select& query_result)
{
    // 1st load
    const Query_ItemSlotInfo_Select::sQueryParam& parameters = query_result.parameters_[0];
    const SLOTIDX slot_type = parameters.slot_type_;
    ItemSlotVersionChecker version_check;
    bool accept_version = ItemSlotVersionChecker::IsAcceptableItemSlot(slot_type, &version_check);
    if (FlowControl::FCAssert(accept_version) == false) {
        return false;
    }
    ItemSlotUpdateNode* update_node_target = NULL;
    POSTYPE max_number_of_slots = 0;
    switch (slot_type)
    {
    case inven_table_.kSlotIndex:
        update_node_target = inven_table_.records_;
        max_number_of_slots = inven_table_.kMaxNumberOfSlots;
        inven_table_._OperateAll(inven_table_.eOper_ClearAllToDefault |
                                 inven_table_.eOper_CopyAllLatestDataToCommitedData,
                                 inven_table_.records_,
                                 inven_table_.kMaxNumberOfSlots,
                                 inven_table_.kSlotIndex);
        break;
    case equip_table_.kSlotIndex:
        update_node_target = equip_table_.records_;
        max_number_of_slots = equip_table_.kMaxNumberOfSlots;
        equip_table_._OperateAll(equip_table_.eOper_ClearAllToDefault |
                                 equip_table_.eOper_CopyAllLatestDataToCommitedData,
                                 equip_table_.records_,
                                 equip_table_.kMaxNumberOfSlots,
                                 equip_table_.kSlotIndex);
        break;
    default:
        __debugbreak();
        return false;
    };
    //
    DBQueryProxyItemSlotNode::DBResult_Select(query_result,
                                              update_node_target, max_number_of_slots);

    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_ItemSlotInfo_Update& query_result)
{
    const Query_ItemSlotInfo_Update::sQueryParam& parameters = query_result.parameters_[0];
    const SLOTIDX slot_type = parameters.slot_type_;
    ItemSlotVersionChecker version_check;
    bool accept_version = ItemSlotVersionChecker::IsAcceptableItemSlot(slot_type, &version_check);
    if (FlowControl::FCAssert(accept_version) == false) {
        return false;
    }
    ItemSlotUpdateNode* update_node_target = NULL;
    POSTYPE max_number_of_slots = 0;
    switch (slot_type)
    {
    case inven_table_.kSlotIndex:
        update_node_target = inven_table_.records_;
        max_number_of_slots = inven_table_.kMaxNumberOfSlots;
        break;
    case equip_table_.kSlotIndex:
        update_node_target = equip_table_.records_;
        max_number_of_slots = equip_table_.kMaxNumberOfSlots;
        break;
    default:
        __debugbreak();
        return false;
    };
    if (FlowControl::FCAssert(parameters.pos_ <= max_number_of_slots) == false) {
        return false;
    }

    DBQueryProxyItemSlotNode::DBResult_Update(query_result,
                                              update_node_target, max_number_of_slots);

    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdateItemSlotInfo()
{
    const bool loaded_equip = (shared_.state & this->eState_Loaded_EquipInfo) != 0;
    const bool loaded_inven = (shared_.state & this->eState_Loaded_InvenInfo) != 0;
    if (loaded_equip == false && loaded_inven == false) {
        return true;
    }
    //
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL)
    {
        return true;    // °­Á¦..transaction success..
    }
    // CHANGES: f110628.1L, the load balance updating implemented. updater='db_user->DBQuery'
    const int number_of_pending_queries = \
        g_DBHandler.GetNumberOfDBConnectionQueuingQueries(db_user);
    if (number_of_pending_queries > g_DBHandler.kMaxNumberOfSpecializedConcurrentBusyUpdateQueries) {
        return false; // pendings
    };
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    //
    bool changed_events = false;
    STRING_SAFER_QUERY64 query_buffer;
    ItemSlotUpdateNode* update_nodes[2] = {
        loaded_equip ? equip_table_.records_ : NULL,
        loaded_inven ? inven_table_.records_ : NULL
    };
    ItemSlotUpdateNode* update_nodes_end[_countof(update_nodes)] = {
        loaded_equip ? &equip_table_.records_[_countof(equip_table_.records_)] : NULL,
        loaded_inven ? &inven_table_.records_[_countof(inven_table_.records_)] : NULL
    };
    //
    for (int update_node_index = 0; update_node_index < _countof(update_nodes); ++update_node_index)
    {
        ItemSlotUpdateNode* update_node_it = update_nodes[update_node_index],
                          * const update_node_end = update_nodes_end[update_node_index];
        if (update_node_it == NULL) {
            continue; //not loaded
        };
        for ( ; update_node_it != update_node_end; ++update_node_it)
        {
            if (update_node_it->query_ != NULL) {
                changed_events = true;
                continue; //in transaction
            }
            update_node_it->UpdateAndCompareData();
            if (update_node_it->record_changed_ == update_node_it->eRecordChange_Default) {
                continue;
            }
            //
            switch (update_node_it->record_changed_)
            {
            case update_node_it->eRecordChange_Changed: // update
                {
                    query_buffer.MakeString(\
                        _T("{?=call S_ItemSlotInfo_Update (?,?,?,?,?,?,?,?)}"));
                    Query_ItemSlotInfo_Update* const query = Query_ItemSlotInfo_Update::ALLOC();
                    query->SetQuery(query_buffer);
                    query->SetUserKey(user_guid);
                    query->set_char_guid(char_guid);
                    // setup parameters
                    Query_ItemSlotInfo_Update::sQueryParam* parameters = &query->parameters_[0];
                    static_cast<ItemSlotResultRecord&>(*parameters) = update_node_it->latest_data_;
                    parameters->guid_ = char_guid;
                    //
                    update_node_it->query_ = query;
                };
                break;
            }
            if (changed_events == false && update_node_it->query_) {
                changed_events = true;
            }
            if (update_node_it->query_)
            {
                db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_ITEMSLOTINFO_DBR,
                                 update_node_it->query_);
            }
        };
    }
    if (changed_events) {
        return false;
    }

    return true;
}

//==================================================================================================

}; //end of namespace


