#include "stdafx.h"
#include "./NewCharacterItemProvider.h"

#include <SCItemSlot.h>

#include <NewCharItemInfoParser.h>
#include <ItemInfoParser.h>
#include <SunEventInfoParser.h>
// (f100713.2L) import a drop information to support a changed new char information table
#include <ItemTypeList.h>

//==================================================================================================

namespace nsSlot {
;

NewCharacterItemProvider* NewCharacterItemProvider::instance_static_ = NULL;

const NewCharacterItemProvider* NewCharacterItemProvider::CreateInstance()
{
    static struct AutoClosure {
        AutoClosure(NewCharacterItemProvider** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        NewCharacterItemProvider** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    NewCharacterItemProvider* provider = new NewCharacterItemProvider;
    //
    if (provider->BuildTable() == false) {
        assert(!"can't building table because inordered sequence."
               "should be loaded a NewCharacterItemInfoParser first");
        return NULL;
    }
    instance_static_ = provider;
    //
    return instance_static_;
}

NewCharacterItemProvider::NewCharacterItemProvider()
    : number_of_total_items_(0),
      total_items_(NULL)
{
    ZeroMemory(character_items_info_, sizeof(character_items_info_));
}

NewCharacterItemProvider::~NewCharacterItemProvider()
{
    if (total_items_) {
        ::free(total_items_);
        total_items_ = NULL;
    }
}

//==================================================================================================
//, NewCharItemInfoParser::GetTable();
bool NewCharacterItemProvider::BuildTable()
{
    NewCharItemInfoParser* const new_char_info_parser = NewCharItemInfoParser::Instance();
    const NEW_CHAR_ITEM_INFO_MAP& char_items = new_char_info_parser->GetTable();
    size_t number_of_total_items = char_items.size();
    if (number_of_total_items == 0) {
        return false;
    };
    const size_t kMaxBufferSize = sizeof(*character_items_info_->items_) * number_of_total_items;
    ;{
        number_of_total_items_ = number_of_total_items;
        void* ptr = ::malloc(kMaxBufferSize);
        ZeroMemory(ptr, kMaxBufferSize);
        total_items_ = reinterpret_cast<ITEMSTREAMEX*>(ptr);
    };
    ZeroMemory(character_items_info_, sizeof(character_items_info_));
    SCItemSlot item_slot;
    // #1. make an inventory stream with items
    ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    ItemTypeList* const item_type_list = ItemTypeList::Instance();
    ITEMSTREAMEX* dest_item_it = total_items_;
    //
    for (uint8_t char_type = eCHAR_BERSERKER;
         char_type < _countof(character_items_info_);
         ++char_type)
    {
        // NOTE: eCHAR_BERSERKER == 1
        Node* const char_node = &character_items_info_[char_type];
        char_node->number_of_items_ = 0;
        char_node->items_ = dest_item_it;
        //
        NEW_CHAR_ITEM_INFO_MAP::_Paircc range = char_items.equal_range(char_type);
        NEW_CHAR_ITEM_INFO_MAP::const_iterator item_it = range.first,
                                               item_end = range.second;
        for ( ; item_it != item_end; ++item_it)
        {
            const NEW_CHAR_ITEM_INFO* char_item_info = item_it->second;
            if (char_item_info == NULL) {
                continue;
            }
            const BASE_ITEMINFO* item_info = item_parser->GetItemInfo(char_item_info->m_wItemCode);
            if (item_info == NULL) {
                continue;
            }
            //
            item_slot.Clear();
            item_slot.SetSerial(TEMP_DBSERIAL_VALUE);
            item_slot.SetCode(char_item_info->m_wItemCode);
            item_slot.SetSlotIdx(SI_INVENTORY);
            //
            // 기간제 아이템의 경우 기간을 넣어준다.
            switch (item_info->m_ChargeSubType)
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
                // 60분 적용(초단위다.. 스크립트는 분단위다.)
                item_slot.SetDateTime(char_item_info->m_dwCasheApplyTime * 60);
                break;
            }
            // select the usage method of m_byCount whether an inputed item is an overlapped item
            const bool use_type_index = item_info->IsWeapon() || item_info->IsArmor();
            const bool is_overlapped_item = (item_slot.IsOverlap() != false);
            const bool use_method_number = (use_type_index == false);
            //                        //should be change to the int type
            int reserved_value = char_item_info->m_ReserveValue;
            if (use_method_number && (reserved_value <= 0 ||
                reserved_value > GM_MAX_CREATE_ITEM_NUM))
            {
                continue;
            }
            //_NA000000_100714_NEW_CHARITEM_EXTENDED_FIELD_
            // the reserved_value field is used by a item type index
            // if an is_overlapped_item equals false and a reserved_value not equals 1
            if (is_overlapped_item == false && use_method_number == false)
            {
                int item_type_index = reserved_value;
                reserved_value = 1;
                const ItemType* item_type = item_type_list->FindItemType(item_type_index);
                if (item_type == NULL) {
                    continue;
                }
                const BYTE enchant = item_type->GetEnchant();
                const KindOfItemType::value_type item_type_kind = item_type->GetItemType();
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
                const BYTE number_of_socket = item_type->GetRank();
                if (number_of_socket != 0) {
                    item_slot.CreateSocketItem(number_of_socket, false);
                }
#else
                // don't use rankup
#endif // _NA_000251_20100727_SOCKET_SYSTEM
                const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
                if (item_rule_info.IsEnableEnchant()) {
                    item_slot.SetEnchant(enchant);
                }
                nsSlot::ItemTypeChanger::ChangeItemByType(&item_slot, item_type_kind);
            }
            if (is_overlapped_item == false) {
                reserved_value = 1;
            }
            item_slot.SetNum(static_cast<POSTYPE>(reserved_value));
            item_slot.SetPos(static_cast<POSTYPE>(char_node->number_of_items_));

            item_slot.CopyOut(*dest_item_it++);
            ++char_node->number_of_items_;
        }; //end for range loop
        //
    } //end for 'char'
    //
    return true;
}

//==================================================================================================

int NewCharacterItemProvider::MakeStream(eCHAR_TYPE char_type,
                                         ITEMSLOTEX_INSERVER* const slot_array) const
{
    // CHANGES: f101006.4L, renewals.
    // this provide default items in the inventory for a creating character.
    int index = static_cast<int>(char_type);
    if (index <= 0 || index >= eCHAR_TYPE_MAX) {
        return 0;
    };
    // assert( this logic enabled operate in inventory streams )
    ITEMSLOTEX_INSERVER* slot_it = slot_array;

    //
    const Node* char_items_info = &character_items_info_[index];
    assert(char_items_info->number_of_items_ <= MAX_INVENTORY_SLOT_NUM);
    
    const ITEMSTREAMEX* item_it = char_items_info->items_,
                      * const item_end = char_items_info->items_ + char_items_info->number_of_items_;
    for (int pos = 0; item_it != item_end; ++pos, ++item_it, ++slot_it)
    {
        slot_it->m_Pos = static_cast<POSTYPE>(pos);
        slot_it->m_Stream = *item_it;
    };
    //
    int number_of_aligned = static_cast<int>(slot_it - slot_array);
    return number_of_aligned;
}


}; //end of namespace