#include "SunClientPrecompiledHeader.h"
#include "StorageItemList.h"
#include "StorageItem.h"


namespace ConvenienceStore
{


    StorageItemList::StorageItemList()
    {
        page_ = 1;
        total_item_count_ = 0;
    }

    StorageItemList::~StorageItemList()
    {
        ClearStorage();
    }


    StorageItem* StorageItemList::GetItemData( size_t index )
    {
        if( index >= storage_array_.size() )
            return NULL;
        return storage_array_.at(index);
    }

    void StorageItemList::ClearStorage()
    {
        page_ = 0;
        total_item_count_ = 0;

        for( size_t i=0;i!=storage_array_.size();++i)
        {
            delete storage_array_[i];
        }
        storage_array_.clear();
    }

    void StorageItemList::SetStorageData( MSG_CG_SHOP_BUY_STORAGE_LIST_ACK* storage_list )
    {
        ClearStorage();

    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL    
        page_ = storage_list->page_;
        total_item_count_ = storage_list->total_item_count_;

        for( int i=0;i<storage_list->item_count_;++i)
        {
            if( StorageItem* item = StorageItem::Create( storage_list->item_info_[i] ) )
            {
                storage_array_.push_back( item );
            }
        }
    #endif //#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    }

    void StorageItemList::SetStorageData( MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK* storage_list )
    {
        ClearStorage();

    #ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
        page_ = storage_list->page_;
        total_item_count_ = storage_list->total_item_count_;

        for( int i=0;i<storage_list->item_count_;++i)
        {
            if( StorageItem* item = StorageItem::Create( storage_list->item_info_[i] ) )
            {
                storage_array_.push_back( item );
            }
        }
    #endif //_NA_0_20110613_GAME_SHOP_RENEWAL
    }

    StorageItem* StorageItemList::Find( StorageItem* pData )
    {
        for( size_t i=0;i!=storage_array_.size();++i)
        {
            StorageItem* item = storage_array_[i];
            if( item == pData )
            {
                return item;
            }
        }
        return NULL;
    }

    StorageItemInfo* StorageItemList::FindStorageItem(long storage_sequence, long storage_item_sequence, char item_type)
    {
        for( size_t i=0;i!=storage_array_.size();++i)
        {
            StorageItem* item = storage_array_[i];
            if( item->Compare( storage_sequence, storage_item_sequence, item_type ) )
            {
                return item->GetItemInfo();
            }
        }
        return NULL;
    }

    bool StorageItemList::EraseStorageItem( long storage_sequence, long storage_item_sequence, char item_type)
    {
        for( size_t i=0;i!=storage_array_.size();++i)
        {
            StorageItem* item = storage_array_[i];
            if( item->Compare( storage_sequence, storage_item_sequence, item_type ) )
            {
                storage_array_.erase( storage_array_.begin() + i );
                delete item;
                return true;
            }
        }
        return false;
    }
}