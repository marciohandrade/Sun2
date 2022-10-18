#pragma once

struct MSG_CG_SHOP_BUY_STORAGE_LIST_ACK;
struct MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK;

namespace ConvenienceStore
{
    class StorageItem;


    class StorageItemList
    {
    private:
        int page_;
        int total_item_count_;
        std::vector<StorageItem*> storage_array_;

    public:
        StorageItemList();
        ~StorageItemList();

    public:

        int GetPage() { return page_; }
        int GetCountPerPage() { return storage_array_.size(); }
        int GetTotalItemCount() { return total_item_count_; }

        StorageItem* GetItemData( size_t index );

        void SetStorageData( MSG_CG_SHOP_BUY_STORAGE_LIST_ACK* storage_list );
        void SetStorageData( MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK* storage_list );

        void ClearStorage();
        bool EraseStorageItem( long storage_sequence, long storage_item_sequence, char item_type);

        StorageItem* Find( StorageItem* pData );
        StorageItemInfo* FindStorageItem(long storage_sequence, long storage_item_sequence, char item_type);
    };
};