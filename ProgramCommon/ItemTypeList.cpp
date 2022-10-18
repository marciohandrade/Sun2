#include "stdafx.h"
#include "./ItemTypeList.h"
#include "./SCItemSlot.h"

//==================================================================================================

ItemTypeList* ItemTypeList::item_type_list_static_ = NULL;

ItemTypeList* ItemTypeList::CreateInstance()
{
    // (NOTE) thread-unsafe singleton
    item_type_list_static_ = new ItemTypeList;
    return item_type_list_static_;
}

void ItemTypeList::DestroyInstance()
{
    SAFE_DELETE(item_type_list_static_);
}

//==================================================================================================

KindOfItemType::value_type
nsSlot::ItemTypeChanger::GetKindOfItemType(const SCItemSlot& item_slot)
{
    const ItemTypeChanger* this_ = reinterpret_cast<ItemTypeChanger*>(NULL);
    __UNUSED(this_);
    const ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.is_valid_item == false) {
        return this_->Default;
    }
    if (item_rule_info.is_shell) {
        return this_->Shell;
    }
    const bool is_divine = (item_slot.IsDivine() != false);
    if (item_rule_info.is_etheria) {
        return is_divine ? this_->EtheriaWithDivine : this_->Etheria;
    }
    return is_divine ? this_->Divine : this_->Default;
}

//_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
bool nsSlot::ItemTypeChanger::IsEqualItemType(const SCItemSlot& item_slot,
                                              const ItemType* item_type)
{
    const ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.is_valid_item == false) {
        return false;
    }
    //
    const KindOfItemType::value_type item_type_value = item_type->GetItemType();
    const ItemTypeChanger* this_ = reinterpret_cast<ItemTypeChanger*>(NULL);
    __UNUSED(this_);

    //Shell타입 (스킨타입)의 일치성 검사
    /*
    if (item_type_value == this_->Shell) {
        return (item_rule_info.is_shell == true);
    }*/
    if ((item_type_value == this_->Shell) != (item_rule_info.is_shell)) {
        return false;
    }

    const bool divine_proc = item_type_value == this_->Divine ||
                             item_type_value == this_->EtheriaWithDivine;
    const bool etheria_proc = item_type_value == this_->Etheria ||
                              item_type_value == this_->EtheriaWithDivine;
    if (etheria_proc && item_rule_info.is_etheria == false) {
        return false;
    }
    const bool is_divine = (item_slot.IsDivine() != false);
    if (divine_proc && is_divine == false) {
        return false;
    }
    // CHANGES: f101029.3L, fixed an unhandled case. prevent unexpected success status
    // if the value of item_type_value belongs in the same category as a default item.
    if (divine_proc == false && is_divine) {
        return false;
    }
    if (etheria_proc == false && item_rule_info.is_etheria) {
        return false;
    }
    return true;
}

//_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
bool nsSlot::ItemTypeChanger::ChangeItemByType(SCItemSlot* item_slot,
                                               KindOfItemType::value_type item_type)
{
    if (item_type < 0 && item_type >= Counts) {
        return false;
    }
    const ItemTypeChanger* this_ = reinterpret_cast<ItemTypeChanger*>(NULL);
    __UNUSED(this_);
    if (item_type == this_->Default) {
        return true;
    };
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot->GetItemRuleInfo();
    RC::eITEM_RESULT item_result;
    if (item_type == this_->Shell) {
        if (item_slot->CanExtractToEtheria(&item_result) == false) {
            return false;
        }
        item_slot->ChangeToShellItem();
        return true;
    };
    //
    const bool divine_proc = item_type == this_->Divine ||
                             item_type == this_->EtheriaWithDivine;
    const bool etheria_proc = item_type == this_->Etheria ||
                              item_type == this_->EtheriaWithDivine;
    if (divine_proc) {
        if (item_rule_info.IsEnableDivine()) {
            item_slot->SetSet(true);
        }
    };
    //
    if (etheria_proc) {
        if (item_slot->CanExtractToEtheria(&item_result) == false) {
            return false;
        }
        item_slot->ChangeToEtheriaItem();
        return true;
    };
    return true;
}

// (f100728.1L) add a making item interface related to the item type information
bool nsSlot::ItemTypeChanger::MakeItemSlotByType(SCItemSlot* item_slot,
                                                 SLOTCODE item_code, DURATYPE duplicated_number,
                                                 const ItemType* item_type)
{
    item_slot->SetCode(item_code);
    const BASE_ITEMINFO* item_info = item_slot->GetItemInfo();
    //   for client check
    if (item_info == NULL || item_info->m_Code == 0) {
        return false;
    }
    if (duplicated_number != 1 && item_slot->IsOverlap() == false) {
        assert(duplicated_number == 1);
        duplicated_number = 1;
    }
    const DURATYPE max_duplicable_number = item_slot->GetMaxNum();
    if (duplicated_number > max_duplicable_number) {
        duplicated_number = max_duplicable_number;
    }
    bool success = true;
    if (item_type)
    {
        const nsSlot::ItemRuleInfo& item_rule_info = item_slot->GetItemRuleInfo();
        // (NOTE) no control a rankup attribute, will be a remove function
        if (item_rule_info.IsEnableEnchant()) {
            BYTE enchant = item_type->GetEnchant();
            item_slot->SetEnchant(enchant);
        }
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
        // EP2에서는 아이템 타입 정보의 랭크 항목을 소켓 개수로 사용한다.
        const BYTE number_of_socket = item_type->GetRank();
        item_slot->CreateSocketItem(number_of_socket, false);        
#endif // NA_000251_20100727_SOCKET_SYSTEM
        KindOfItemType::value_type kind_of_item_type = item_type->GetItemType();
        success = nsSlot::ItemTypeChanger::ChangeItemByType(item_slot, kind_of_item_type);
    }
    return success;
}

//==================================================================================================

ItemTypeList::ItemTypeList(void)
    : item_type_chunk_block_(NULL)
{
}

ItemTypeList::~ItemTypeList(void)
{
    Unload();
}

void ItemTypeList::Unload()
{
    item_type_hash_.clear();
    if (item_type_chunk_block_) {
        ::free(item_type_chunk_block_);
        item_type_chunk_block_ = NULL;
    }
}

void ItemTypeList::Init(char* pack_file_name)
{
    SetPackFileName(pack_file_name);
}

BOOL ItemTypeList::_Load(BOOL reloaded)
{
    __UNUSED(reloaded);
    const int max_rows = GetRowSize();
    ;{ // resource allocation
        if (item_type_chunk_block_) {
            ::free(item_type_chunk_block_);
        }
        const size_t chunk_size = sizeof(item_type_chunk_block_[0]) * max_rows;
        item_type_chunk_block_ = reinterpret_cast<ItemType*>(::malloc(chunk_size));
        ZeroMemory(item_type_chunk_block_, chunk_size);
    };
    //
    for (int row = 0; row < max_rows; ++row)
    {
        ItemType* const item_type = item_type_chunk_block_ + row;
        const DWORD item_type_index = GetDataDWORD("ItemTypeIndex", row);
        item_type->SetIndex(item_type_index);
        item_type->SetEnchant(GetDataBYTE("Enchant", row));
        KindOfItemType kind_of_type;
        kind_of_type.value = GetDataBYTE("bDivine", row);
        if (kind_of_type.value >= kind_of_type.Counts)
        {
            SUNLOG(eCRITICAL_LOG,
                   _T("|["__FUNCTION__"]|ItemTypeIndex=%d|bDivine=%d|")
                   _T("|Msg=bDivine has an index out of range, change to default value|"),
                   item_type_index, kind_of_type);
            kind_of_type.value = kind_of_type.Default;
        }
        item_type->SetItemType(kind_of_type.value);
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
        // EP2에서는 아이템 타입 정보의 랭크 항목을 소켓 개수로 사용한다.
        BYTE number_of_socket = GetDataBYTE("Grade", row);
        if (number_of_socket > SOCKET_MAX)
        {
            SUNLOG(eCRITICAL_LOG, 
                   _T("|%s|소켓 개수 범위가 옳바르지 않아서 0으로 초기화 합니다.|현재줄 = %u, 소켓개수 = %u|"), 
                   __FUNCTION__, 
                   row, 
                   number_of_socket);
            number_of_socket = 0;
        }
        item_type->SetRank(number_of_socket);
#else
        item_type->SetRank(GetDataBYTE("Grade", row));
#endif // _NA_000251_20100727_SOCKET_SYSTEM

        if (FindItemType(item_type_index) == NULL)
        {
            item_type_hash_[item_type_index] = item_type;
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, "Script Data Fail, |["__FUNCTION__"]| index = %d",
                   item_type_index);
            FASSERT(0);
        }
    }

    return true;
}

void ItemTypeList::Reload()
{
    //Load(m_pszFileName, true);
}

void ItemTypeList::Release()
{
    Unload();
}

BOOL ItemTypeList::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_ITEMTYPELIST:
        return _Load(reloaded);
    }

    return false;
}

