#include "stdafx.h"
#include ".\ShopInfoParser.h"
#include <ItemInfoParser.h>
#include <ItemTypeList.h>

//==================================================================================================

const SHOPINFO::SHOP_ITEM SHOPINFO::empty_shop_item_static_ = { 0, };

SHOPINFO::SHOPINFO()
    :shop_list_id_(0)
    ,shop_item_hash_(NULL)
    #ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
    ,shop_type_(SHOP_TYPE_MAX)
    ,currency_item_(0)
    #endif
{
#if !defined(_SERVER)
    shop_item_hash_ = new SHOP_ITEM_HASH;
#endif
    if (shop_item_hash_) {
        shop_item_hash_->Initialize(_MAX_SELLITEM_NUM * _MAX_TAB_NUM);
    }
}

SHOPINFO::~SHOPINFO()
{
    Release();
}

void SHOPINFO::Init(DWORD shop_list_id, const char* shop_name)
{
    if (shop_item_hash_) {
        FASSERT(shop_item_hash_->GetDataNum() == 0);
    }
    shop_list_id_ = shop_list_id;
    strncpy(shop_name_debug_, shop_name, _countof(shop_name_debug_));
    shop_name_debug_[_countof(shop_name_debug_) - 1] = '\0';
    ZeroMemory(shop_item_table_, sizeof(shop_item_table_));
}

void SHOPINFO::Release()
{
    DestroyItemAll();
#if !defined(_SERVER)
    SAFE_DELETE(shop_item_hash_);
#endif
}

void SHOPINFO::CreateItem(const SHOP_ITEM& shop_item_data)
{
    FASSERT(shop_item_data.m_byTabIndex < _MAX_TAB_NUM &&
            shop_item_data.m_byItemIndex < _MAX_SELLITEM_NUM);
    SHOP_ITEM* const shop_item = \
        &shop_item_table_[shop_item_data.m_byTabIndex][shop_item_data.m_byItemIndex];
    *shop_item = shop_item_data;

    if (shop_item_hash_) {
        shop_item_hash_->Add(shop_item, shop_item_data.m_SellItemCode);
    }
}
/*
void SHOPINFO::CreateItem(BYTE tab_index, BYTE slot_pos,
                          SLOTCODE item_code, DURATYPE number_of_items)
{
    FASSERT(tab_index < _MAX_TAB_NUM);
    FASSERT(slot_pos < _MAX_SELLITEM_NUM);
    SHOP_ITEM* const shop_item = &shop_item_table_[tab_index][slot_pos];
    // assert : shop_item == cleared stream
    shop_item->m_byTabIndex = tab_index;
    shop_item->m_byItemIndex = slot_pos;
    shop_item->m_SellItemCode = item_code;
    shop_item->m_SellItemNum = number_of_items;

    //  FASSERT(NULL == shop_item_hash_.GetData(pNewItem->m_SellItemCode));
    // 중복개수가 들어 가면서 m_SellItemCode가 중복될 수가 있다.
    if (shop_item_hash_) {
        shop_item_hash_->Add(shop_item, item_code);
    }
}
*/
void SHOPINFO::DestroyItem(int tab_index)
{
    if (tab_index < 0 || tab_index >= _MAX_TAB_NUM) {
        return;
    }
    //
    if (shop_item_hash_)
    {
        shop_item_hash_->SetFirst();
        while (SHOP_ITEM* pItem = shop_item_hash_->GetNext())
        {
            if (pItem->m_byTabIndex == tab_index)
            {
                shop_item_hash_->Remove(pItem->m_SellItemCode);
            }
        }
    }
}

void SHOPINFO::DestroyItemAll()
{
    if (shop_item_hash_) {
        shop_item_hash_->RemoveAll();
    }
}

#if !defined(_SERVER)
bool SHOPINFO::HasSellItemByItemType( eITEMTYPE item_type )
{
    for ( int i = 0; i < SHOPINFO::_MAX_TAB_NUM; ++i)
    {
        for ( int j = 0; j < SHOPINFO::_MAX_SELLITEM_NUM; ++j )
        {
            SHOP_ITEM* shop_item = GetItemForIndex(i, j);
            if (shop_item == NULL)
            {
                continue;
            }

            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(shop_item->m_SellItemCode);
            if (item_info == NULL)
            {
                continue;
            }

            if (item_info->m_wType == item_type)
            {
                return true;
            }
        }
    }

    return false;
}

bool SHOPINFO::HasSellItemByItemCode( SLOTCODE item_code )
{
    for ( int i = 0; i < SHOPINFO::_MAX_TAB_NUM; ++i)
    {
        for ( int j = 0; j < SHOPINFO::_MAX_SELLITEM_NUM; ++j )
        {
            SHOP_ITEM* shop_item = GetItemForIndex(i, j);
            if (shop_item == NULL)
            {
                continue;
            }

            if( shop_item->m_SellItemCode == item_code )
            {
                return true;
            }
        }
    }

    return false;
}
#endif


//==================================================================================================

ShopInfoParser::ShopInfoParser()
{
}

ShopInfoParser::~ShopInfoParser()
{
    Release();
}

void ShopInfoParser::Release()
{
    Unload();
}

void ShopInfoParser::Init(DWORD unused)
{
    __UNUSED(unused);
}

void ShopInfoParser::Init(DWORD unused, char* pack_file_name)
{
    Init(unused);
    SetPackFileName(pack_file_name);
}

void ShopInfoParser::Unload()
{
    FOREACH_CONTAINER(const SHOP_INFO_TABLE::value_type& node, shop_info_table_, SHOP_INFO_TABLE)
    {
        const SHOPINFO* shop_info = node.second;
        delete shop_info;
    }
    shop_info_table_.clear();
}

void ShopInfoParser::Reload()
{
    //  Load(m_pszFileName, true);
}

BOOL ShopInfoParser::_Load(BOOL reloaded)
{
    const int max_rows = GetRowSize();
    //
    char shop_name[MAX_SHOPNAME_LENGTH + 1];

    char field_name_code[MAX_FIELD_NAME_SIZE] = "wItemCode";
    const size_t field_name_code_len = strlen(field_name_code);

    char field_name_number[MAX_FIELD_NAME_SIZE] = "byItemNum";
    const size_t field_name_number_len = strlen(field_name_number);

    char field_name_IGtype[MAX_FIELD_NAME_SIZE] = "IGType";
    const size_t field_name_IGtype_len = strlen(field_name_IGtype);

#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
    char field_name_ItemCost[MAX_FIELD_NAME_SIZE] = "ItemCost";
    const size_t field_name_ItemCost_len = strlen(field_name_ItemCost);
#endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
    char field_name_fame_area[MAX_FIELD_NAME_SIZE] = "FameArea";
    const size_t field_name_fame_area_len = strlen(field_name_fame_area);
    
    char field_name_repute_area[MAX_FIELD_NAME_SIZE] = "ReputeArea";
    const size_t field_name_repute_area_len = strlen(field_name_repute_area);
    
    char field_name_fame_rating[MAX_FIELD_NAME_SIZE] = "FameRating";
    const size_t field_name_fame_rating_len = strlen(field_name_fame_rating);
    
    char field_name_repute_rating[MAX_FIELD_NAME_SIZE] = "ReputeRating";
    const size_t field_name_repute_rating_len = strlen(field_name_repute_rating);
#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP
    char field_name_timelimit[MAX_FIELD_NAME_SIZE] = "TimeLimit";
    const size_t field_name_timelimit_len = strlen(field_name_timelimit);
#endif //_NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP

    //
    //char field_name_buffer[MAX_FIELD_NAME_SIZE];
    ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    ItemTypeList* const item_type_parser = ItemTypeList::Instance();
    //
    for (int row = 0; row < max_rows; ++row)
    {       
        const DWORD shop_list_id = GetDataDWORD("dwID", row);
        const int tab_index = GetDataInt("iShopTab", row);
        FASSERT(tab_index < SHOPINFO::_MAX_TAB_NUM);

        // debug field
        strncpy(shop_name, GetDataString("Name", row), _countof(shop_name));
        shop_name[_countof(shop_name) - 1] = '\0';

        SHOPINFO* shop_info = GetShopList(shop_list_id);
        if (shop_info == NULL) 
        {
            shop_info = new SHOPINFO;
            shop_info->Init(shop_list_id, shop_name);
            shop_info_table_.insert(std::make_pair(shop_list_id, shop_info));
        }
        else
        {
            if (reloaded) {
                shop_info->DestroyItem(tab_index);
            }
            else {
                // 무시~
            }
        }
        //_NA_0_20091119_HEIM_LOTTO_ACCUM_CTRL_BY_SHOP
        shop_info->lottoAccumRatio[tab_index] = GetDataInt("LottoRatio", row);
#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
        shop_info->shop_type_ = static_cast<NPC_SHOP_TYPE>(GetDataBYTE("Type", row));
        shop_info->currency_item_ = GetDataWORD("Currency", row);
#endif

        for (int i = 0; i < SHOPINFO::_MAX_SELLITEM_NUM; ++i)
        {
            const char char_index = static_cast<char>(i + 1);
            const char offset0_char = \
                static_cast<char>((char_index < 10) ? '0' + char_index : '0' + (char_index / 10));
            const char offset1_char = \
                static_cast<char>((char_index < 10) ? '\0' : '0' + (char_index % 10));
            const char offset2_char = '\0';
            
            field_name_code[field_name_code_len + 0] = offset0_char;
            field_name_code[field_name_code_len + 1] = offset1_char;
            field_name_code[field_name_code_len + 2] = offset2_char;
            const SLOTCODE item_code = GetDataWORD(field_name_code, row);
            if (item_code == 0) 
            {
                continue;
            }
            
            field_name_number[field_name_number_len + 0] = offset0_char;
            field_name_number[field_name_number_len + 1] = offset1_char;
            field_name_number[field_name_number_len + 2] = offset2_char;
            const DURATYPE number_of_items = GetDataBYTE(field_name_number, row);
            const BASE_ITEMINFO* item_info = item_parser->GetItemInfo(item_code);
            FASSERT(NULL != item_info);
            FASSERT(number_of_items != 0 && number_of_items <= item_info->m_byDupNum);
            
            //_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
            field_name_IGtype[field_name_IGtype_len + 0] = offset0_char;
            field_name_IGtype[field_name_IGtype_len + 1] = offset1_char;
            field_name_IGtype[field_name_IGtype_len + 2] = offset2_char;
            const uint16_t item_type_index = GetDataWORD(field_name_IGtype, row);
            const ItemType* item_type = item_type_parser->FindItemType(item_type_index);

        #ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
            field_name_ItemCost[field_name_ItemCost_len + 0] = offset0_char;
            field_name_ItemCost[field_name_ItemCost_len + 1] = offset1_char;
            field_name_ItemCost[field_name_ItemCost_len + 2] = offset2_char;
            const DWORD item_cost = GetDataDWORD(field_name_ItemCost, row);
            FASSERT(item_cost >= 0 && item_cost < 0xFFFFFFFF);
        #endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
            field_name_fame_area[field_name_fame_area_len + 0] = offset0_char;
            field_name_fame_area[field_name_fame_area_len + 1] = offset1_char;
            field_name_fame_area[field_name_fame_area_len + 2] = offset2_char;
            const WORD fame_area = GetDataWORD(field_name_fame_area, row);

            field_name_repute_area[field_name_repute_area_len + 0] = offset0_char;
            field_name_repute_area[field_name_repute_area_len + 1] = offset1_char;
            field_name_repute_area[field_name_repute_area_len + 2] = offset2_char;
            const WORD repute_area = GetDataWORD(field_name_repute_area, row);

            field_name_fame_rating[field_name_fame_rating_len + 0] = offset0_char;
            field_name_fame_rating[field_name_fame_rating_len + 1] = offset1_char;
            field_name_fame_rating[field_name_fame_rating_len + 2] = offset2_char;
            const BYTE fame_rating = GetDataBYTE(field_name_fame_rating, row);

            field_name_repute_rating[field_name_repute_rating_len + 0] = offset0_char;
            field_name_repute_rating[field_name_repute_rating_len + 1] = offset1_char;
            field_name_repute_rating[field_name_repute_rating_len + 2] = offset2_char;
            const DWORD repute_rating = GetDataDWORD(field_name_repute_rating, row);

#endif //_NA_003027_20111013_HONOR_SYSTEM

#ifdef _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP
            field_name_timelimit[field_name_timelimit_len + 0] = offset0_char;
            field_name_timelimit[field_name_timelimit_len + 1] = offset1_char;
            field_name_timelimit[field_name_timelimit_len + 2] = offset2_char;
            const DWORD timelimit = GetDataDWORD(field_name_timelimit, row);
#endif //_NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP

            SHOPINFO::SHOP_ITEM making_shop_item = { 0, };
            making_shop_item.m_byTabIndex = static_cast<BYTE>(tab_index);
            making_shop_item.m_byItemIndex = static_cast<POSTYPE>(i);
            making_shop_item.m_SellItemCode = item_code;
            making_shop_item.m_SellItemNum = number_of_items;
            making_shop_item.item_type_ = item_type;
        #ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
            making_shop_item.item_cost_ = (WORD)item_cost;
        #endif

#ifdef _NA_003027_20111013_HONOR_SYSTEM
            making_shop_item.fame_area_ = fame_area;
            making_shop_item.repute_area_ = repute_area;
            making_shop_item.fame_rating_ = fame_rating;
            making_shop_item.repute_rating_ = repute_rating;
#endif //_NA_003027_20111013_HONOR_SYSTEM
#ifdef _NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP
            making_shop_item.timelimit_ = timelimit;
#endif //_NA_007444_20140722_TIMELIMITITEM_SELL_ON_SHOP

            shop_info->CreateItem(making_shop_item);
        }
    }

    return true;
}

BOOL ShopInfoParser::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_SHOPINFO:
        return _Load(reloaded);
    }

    return false;
}

#if !defined(_SERVER)
bool ShopInfoParser::HasSellItemByItemType( DWORD shop_list_id, eITEMTYPE item_type )
{
    if( SHOPINFO* shop_info = GetShopList( shop_list_id ) )
    {
        return shop_info->HasSellItemByItemType( item_type );
    }
    return false;
}

bool ShopInfoParser::HasSellItemByItemCode( DWORD shop_list_id, SLOTCODE item_code )
{
    if( SHOPINFO* shop_info = GetShopList( shop_list_id ) )
    {
        return shop_info->HasSellItemByItemCode( item_code );
    }
    return false;
}
#endif
