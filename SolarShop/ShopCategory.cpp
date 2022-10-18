
#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL




#include "./ShopCategory.h"

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#include "./ShopItem.h"


//===============================================================================================
//
// class Category
//
//===============================================================================================
ShopCategory::ShopCategory()
: parent_(NULL)
, category_index_(SHOP_CATEGORY_INVALID)
, allowed_visitors_(true)
, shopitem_count_per_page_(1)
, child_shopitem_changed_count_(0)
{

}

ShopCategory::~ShopCategory()
{

}

void ShopCategory::Initialize(eSHOP_CATEGORY category_index, uint shopitem_count_per_page)
{
    parent_ = NULL;
    allowed_visitors_ = true;
    child_shopitem_changed_count_ = 0;
    category_index_ = category_index;
    child_category_array_.clear();
    child_shopitem_array_.clear();
    shopitem_count_per_page_ = shopitem_count_per_page;
}

void ShopCategory::AttachCategory(ShopCategory* child_category)
{
    child_category->parent_ = this;
    child_category_array_.push_back(child_category);
    child_shopitem_change();
}

void ShopCategory::InsertShopItem(ShopItem* shopitem)
{
    child_shopitem_array_.push_back(shopitem);
    child_shopitem_change();
}

void ShopCategory::RemoveShopItem(ShopItem* shopitem)
{
    ShopItemArray::iterator iter = child_shopitem_array_.begin();
    for (; iter != child_shopitem_array_.end(); ++iter)
    {
        if (*iter == shopitem)
        {
            child_shopitem_array_.erase(iter);
            child_shopitem_change();
            break;
        }
    }
}

void ShopCategory::RemoveShopItemAll()
{
    child_shopitem_array_.clear();
    child_shopitem_change();
}

void ShopCategory::Search(STLX_DEQUE<ShopItem*>& shopitem_array, uint shopitem_filter)
{
    if (!child_category_array_.empty())
    {
        size_t count = child_category_array_.size();
        for (uint idx = 0 ; idx < count ; ++idx)
        {
            if (child_category_array_[idx]->allowed_visitors_)
                child_category_array_[idx]->Search(shopitem_array, shopitem_filter);
        }
    }

    uint char_filter = SHOP_FILTER_CHAR_ALL & shopitem_filter;
    uint slogan_filter = SHOP_FILTER_SLOGAN_ALL & shopitem_filter;
    size_t count = child_shopitem_array_.size();
    for (uint idx = 0 ; idx < count ; ++idx)
    {
        if (!(char_filter & child_shopitem_array_[idx]->GetItemFilter()))
            continue;

        if (slogan_filter != SHOP_FILTER_NONE)
        {
            if (slogan_filter != (slogan_filter & child_shopitem_array_[idx]->GetItemFilter()))
                continue;
        }

        shopitem_array.push_back(child_shopitem_array_[idx]);
    }
}

void ShopCategory::child_shopitem_change()
{
    ++child_shopitem_changed_count_;
    ShopCategory* parent = parent_;
    while(parent)
    {
        ++(parent->child_shopitem_changed_count_);
        parent = parent->parent_;
    }
}


//==================================================================================================




#else //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING




//==================================================================================================


#include <ArchiveOfStream.h>

#include "./ShopItem.h"
#include "./ShopManager.h"


ShopCategory::ShopCategory()
    : m_Category(CT_ALLITEM)
{
}

ShopCategory::~ShopCategory()
{
}

void ShopCategory::Release()
{
    m_Category = CT_ALLITEM;
    m_ShopItemList.clear();
}

ShopItem* ShopCategory::FindShopItem(const ITEMGUID guid)
{
    SHOPITEM_LIST::iterator it = m_ShopItemList.begin(), end = m_ShopItemList.end();
    for (; it != end ; ++it)
    {
        ShopItem* shop_item = *it;
        if (shop_item->GetGuid() == guid) {
            return shop_item;
        }
    }
    return NULL;
}

void ShopCategory::RemoveShopItem(ShopItem* pShopItem)
{
    SHOPITEM_LIST::iterator it = m_ShopItemList.begin(), end = m_ShopItemList.end();
    for (; it != end ;)
    {
        ShopItem* shop_item = *it;
        if (shop_item->GetGuid() == pShopItem->GetGuid()) 
        {
            m_ShopItemList.erase(it++);
        }
        else
            ++it;
    }
}

void ShopCategory::AddShopItem(ShopItem* pShopItem)
{
    ASSERT(0 != pShopItem->GetGuid());
#ifdef _DEBUG
    SHOPITEM_LIST::iterator it = m_ShopItemList.begin(), end = m_ShopItemList.end();
    for (; it != end ; ++it)
    {
        ShopItem* shop_item = *it;
        ASSERT(shop_item != pShopItem);
        ASSERT(shop_item->GetGuid() != pShopItem->GetGuid());
        if (shop_item == pShopItem || 
            shop_item->GetGuid() == pShopItem->GetGuid()) return;
    }
#endif
    m_ShopItemList.push_back(pShopItem);
}

void ShopCategory::Serialize(ShopManager& mgr IN, BYTE page, ArchiveOfStream& ar, eSHOP_SERIALIZE se)
{
    switch(se)
    {
    case eSHOP_SERIALIZE_ITEM_LIST_SAVE:
        {
            // 카테고리 반드시 존재
            BYTE Count=0;
            ITEMGUID Guid = 0;
            ar >> Count;
            for (BYTE i = 0 ; i < Count ; ++i)
            {
                ar >> Guid;
                ShopItem* pShopItem = mgr.FindShopItem(Guid);
                if (!pShopItem)
                {
                    pShopItem = mgr.CreateShopItem(GetSeperateType());
                    pShopItem->Release();
                    ShopItem::ShopItemData* shop_item_data = pShopItem->GetEditableShopItemData();
                    shop_item_data->item_guid_ = Guid;
                    shop_item_data->category_ = static_cast<uint8_t>(GetCategory());

                    mgr.AddShopItem(pShopItem);
                }
                this->AddShopItem(pShopItem);
                pShopItem->Serialize(mgr, ar, se);
            }
        }break;
    case eSHOP_SERIALIZE_ITEM_LIST_LOAD:
        {
            // 페이지 처리 필요
            int StartRecordIndex = page* GetItemNumPerPage();
            int EndRecordIndex = StartRecordIndex + GetItemNumPerPage();

            if ((int)m_ShopItemList.size() > StartRecordIndex)
            {
                if ((int)m_ShopItemList.size() >= EndRecordIndex)
                    ar << (BYTE)(GetItemNumPerPage());
                else
                    ar << (BYTE)(m_ShopItemList.size() - StartRecordIndex);
            }
            else
            {
                ar << (BYTE)(0);
                return;
            }

            int Index = 0;
            SHOPITEM_LIST::iterator it = m_ShopItemList.begin(), end = m_ShopItemList.end();
            for (; it != end && Index < EndRecordIndex ; ++it, ++Index)
            {
                ShopItem* pItem = (*it);
                if (StartRecordIndex <= Index)
                {
                    int ReserveSize = ar.GetBufferSize() - ar.GetSize();
                    int ItemMaxSize = pItem->GetType() == ShopItem::SINGLE
                        ? sizeof(SingleItem) : sizeof(PackageItem);
                    if (ReserveSize < ItemMaxSize)
                        break;
                    pItem->Serialize(mgr, ar, se);
                }
            }
        }break;
    }

}

void ShopCategory::Display()
{
    std::cout << "[Category:" << m_Category << "]-------------------------------------------------------------------" << std::endl;
    SHOPITEM_LIST::iterator it = m_ShopItemList.begin(), end = m_ShopItemList.end();
    for (; it != end ; ++it)
    {
        (*it)->Display();
    }
}


#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
