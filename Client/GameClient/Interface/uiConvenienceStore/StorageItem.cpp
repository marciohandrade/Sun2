#include "SunClientPrecompiledHeader.h"
#include "StorageItem.h"


namespace ConvenienceStore
{

    StorageItem* StorageItem::Create( const StorageItemInfoBuy& ItemInfo )
    {
        if( product_list() == NULL )
            return NULL;

        CShopProduct* shop_product = NULL;
        if( product_list()->GetValueByKey(ItemInfo.product_sequence_, shop_product) )
        {
            ProductInfo product_info;
            if( FillProductInfo( shop_product, ItemInfo.price_sequence_, eCHAR_NONE, OUT product_info ) )
            {
                StorageItem* ItemData = new StorageItem();
                ItemData->object_type_ = OBJECT_TYPE_STORAGE_BUY;

                ItemData->product_info_ = product_info;
                ItemData->product_info_.SetShopProduct( shop_product );

                StorageItemInfoBuy* storage_item_info = new StorageItemInfoBuy();
                *storage_item_info = ItemInfo;

                ItemData->item_info_ptr_ = storage_item_info;
                *ItemData->item_info_ptr_ = ItemInfo;

                return ItemData;
            }


        }

        return NULL;
    }

    StorageItem* StorageItem::Create( const StorageItemInfoGift& ItemInfo )
    {
        if( product_list() == NULL )
            return NULL;

        CShopProduct* shop_product = NULL;
        if( product_list()->GetValueByKey(ItemInfo.product_sequence_, shop_product) )
        {
            ProductInfo product_info;
            if( FillProductInfo( shop_product, ItemInfo.price_sequence_, eCHAR_NONE, OUT product_info ) )
            {
                StorageItem* ItemData = new StorageItem();
                ItemData->object_type_ = OBJECT_TYPE_STORAGE_GIFT;

                ItemData->product_info_ = product_info;
                ItemData->product_info_.SetShopProduct( shop_product );

                StorageItemInfoGift* storage_gift_item_info = new StorageItemInfoGift();
                *storage_gift_item_info = ItemInfo;

                ItemData->item_info_ptr_ = storage_gift_item_info;
                *ItemData->item_info_ptr_ = ItemInfo;

                return ItemData;
            }
        }

        return NULL;
    }

    StorageItem::StorageItem()
    : item_info_ptr_(NULL)
    {


    }

    StorageItem::~StorageItem()
    {
        if( item_info_ptr_ != NULL )
        {
            delete item_info_ptr_;
            item_info_ptr_ = NULL;
        }

        if ( IconImage* pIconImage = product_info_.GetIconImage() )
        {
            ICONMGR()->RemoveIcon(pIconImage);
            product_info_.SetIconImage( NULL );
        }
    }

    bool StorageItem::Compare( long storage_sequence, long storage_item_sequence, char item_type )
    {
        return (item_info_ptr_->storage_sequence_ == storage_sequence && item_info_ptr_->storage_item_sequence_ == storage_item_sequence && item_info_ptr_->storage_item_type_ == item_type);
    }

    LPCTSTR StorageItem::GetSendCharacterName()
    {
        if( IsStorageBuy() )
        {
            return GLOBAL_EMPTY_STRING;
        }

        return ((StorageItemInfoGift*)item_info_ptr_)->send_character_name_;
    }

    LPCTSTR StorageItem::GetGiftMessage()
    {
        if( IsStorageBuy() )
        {
            return GLOBAL_EMPTY_STRING;
        }

        return ((StorageItemInfoGift*)item_info_ptr_)->gift_message_;
    }
}