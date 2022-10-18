#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef GAMESHOP_H_
#define GAMESHOP_H_


#if _MSC_VER > 1000
#pragma once
#endif


namespace GameShop
{

class Transaction
{
public:
    enum Type
    {
        transaction_none = 0,
        transaction_cash = (1),
        transaction_buy_storage_list = (1 << 1),
        transaction_gift_storage_list = (1 << 2),
        transaction_buy_item = (1 << 3),
        transaction_send_gift = (1 << 4),
        transaction_use_storage_item = (1 << 5),
        transaction_character_name_check = (1 << 6),
        transaction_item_serial_update = (1 << 7),
        transaction_item_roll_back = (1 << 8),
        transaction_exist_gift_notify = (1 << 9),
    };

public:
    Transaction()
    : transaction_(Transaction::transaction_none)
    , accumulator_(0)
    {
    }
    ~Transaction()
    {
    }
    void Add(Transaction::Type type_)
    {
        accumulator_ = 0;
        transaction_ |= type_;
    }
    void Sub(Transaction::Type type_)
    {
        accumulator_ = 0;
        transaction_ &= ~(type_);
    }
    uint Get() const
    {
        return transaction_;
    }
    void Update(uint delta_tick, uint& dead_transaction)
    {
        dead_transaction = Transaction::transaction_none;
        if (transaction_ == Transaction::transaction_none)
            return;

        accumulator_ += delta_tick;
        if (accumulator_ >= 5000)
        {
            dead_transaction = transaction_;
            transaction_ = Transaction::transaction_none;
            accumulator_ = 0;
        }
    }
    bool CanUse(Transaction::Type type_)
    {
        // TODO : 각 트렌젝션의 상황에 따른 허용 여부.
        uint temp = Transaction::transaction_none;

        if (transaction_ & Transaction::transaction_exist_gift_notify)
            return false;

        if (transaction_ & Transaction::transaction_item_serial_update)
            return false;

        if (transaction_ & Transaction::transaction_item_roll_back)
            return false;

        switch (type_)
        {
        case Transaction::transaction_cash:
            {
                temp = Transaction::transaction_none;
                temp |= Transaction::transaction_cash;
                temp |= Transaction::transaction_send_gift;
                temp |= Transaction::transaction_buy_item;
                if (temp & transaction_)
                    return false;
            }
            break;
        case Transaction::transaction_buy_storage_list:
            {
                temp = Transaction::transaction_none;
                temp |= Transaction::transaction_buy_storage_list;
                temp |= Transaction::transaction_use_storage_item;
                temp |= Transaction::transaction_send_gift;
                temp |= Transaction::transaction_buy_item;
                if (temp & transaction_)
                    return false;
            }
            break;
        case Transaction::transaction_gift_storage_list:
            {
                temp = Transaction::transaction_none;
                temp |= Transaction::transaction_gift_storage_list;
                temp |= Transaction::transaction_use_storage_item;
                temp |= Transaction::transaction_send_gift;
                temp |= Transaction::transaction_buy_item;
                if (temp & transaction_)
                    return false;
            }
            break;
        case Transaction::transaction_buy_item:
            {
                temp = Transaction::transaction_none;
                temp |= Transaction::transaction_buy_item;
                temp |= Transaction::transaction_send_gift;
                if (temp & transaction_)
                    return false;
            }
            break;
        case Transaction::transaction_send_gift:
            {
                temp = Transaction::transaction_none;
                temp |= Transaction::transaction_send_gift;
                temp |= Transaction::transaction_buy_item;
                if (temp & transaction_)
                    return false;
            }
            break;
        case Transaction::transaction_use_storage_item:
            {
                temp = Transaction::transaction_none;
                temp |= Transaction::transaction_use_storage_item;
                temp |= Transaction::transaction_buy_storage_list;
                temp |= Transaction::transaction_gift_storage_list;
                temp |= Transaction::transaction_send_gift;
                temp |= Transaction::transaction_buy_item;
                if (temp & transaction_)
                    return false;
            }
            break;
        case Transaction::transaction_character_name_check:
            {
                temp = Transaction::transaction_none;
                temp |= Transaction::transaction_character_name_check;
                temp |= Transaction::transaction_use_storage_item;
                temp |= Transaction::transaction_buy_storage_list;
                temp |= Transaction::transaction_gift_storage_list;
                temp |= Transaction::transaction_send_gift;
                temp |= Transaction::transaction_buy_item;
                if (temp & transaction_)
                    return false;
            }
            break;
        }
        return true;
    }
private:
    uint transaction_;
    uint accumulator_;
};



class ItemRelay
{
public:
    ItemRelay()
    : item_array_(NULL)
    , item_count_(0)
    , item_index_(0)
    {
    }

    ~ItemRelay()
    {
        Clear();
    }

    bool Push(ShopItemInfo* shop_item, uint count)
    {
        if(item_array_ || shop_item == NULL || count == 0)
        {
            ASSERT(0);
            SUNLOG(eCRITICAL_LOG, "GameShop::ItemRelay::Push Fail");
            return false;
        }
        item_array_ = new ShopItemInfoEx[count];
        item_count_ = count;
        item_index_ = 0;

        for (uint idx = 0 ; idx < item_count_ ; ++idx)
        {
            item_array_[idx].cash_type_ = shop_item[idx].cash_type_;
            item_array_[idx].price_sequence_ = shop_item[idx].price_sequence_;
            item_array_[idx].package_product_sequence_ = shop_item[idx].package_product_sequence_;
            item_array_[idx].product_display_sequence_ = shop_item[idx].product_display_sequence_;
            item_array_[idx].error_code_ = -1;
            item_array_[idx].product_left_count_ = 0;
        }

        return true;
    }

    bool IsEmpty()
    {
        return (item_index_ >= item_count_) ? true : false;
    }

    ShopItemInfoEx* Get()
    {
        if (item_array_ && (item_index_ < item_count_))
        {
            return &(item_array_[item_index_]);
        }
        return NULL;
    }

    void Pop()
    {
        if (item_array_ && (item_index_ < item_count_))
            ++item_index_;
    }

    void SaveResult(int error_code, int product_left_count)
    {
        if (item_array_ && (item_index_ < item_count_))
        {
            item_array_[item_index_].error_code_ = error_code;
            item_array_[item_index_].product_left_count_ = product_left_count;
        }
    }

    void Destroy(ShopItemInfoEx* OUT item, uint* OUT count)
    {
        if (item && item_array_ && count)
        {
            memcpy(item, item_array_, sizeof(ShopItemInfoEx)*item_count_);
            *count = item_count_;
        }

        if (item_array_)
        {
            delete [] item_array_;
            item_array_ = NULL;
        }

        item_count_ = 0;
        item_index_ = 0;
    }

    void Clear()
    {
        Destroy(NULL, NULL);
    }

private:
    ShopItemInfoEx* item_array_;
    uint item_count_;
    uint item_index_;
};



struct GiftReceiver
{
    bool find_;
    USERGUID user_guid_;
    USERGUID character_guid_;
    char character_name_[MAX_CHARNAME_LENGTH+1];
    char gift_message_[GIFT_MESSAGE_LENGTH];
    char guild_name_[MAX_GUILDNAME_LENGTH + 1];

    void Clear()
    {
        find_ = false;
        user_guid_ = 0;
        character_guid_ = 0;
    }

    void ApplyReceiver(USERGUID user_guid, USERGUID character_guid, const char* receiver, const char* guid_name)
    {
        user_guid_ = user_guid;
        character_guid_ = character_guid;
        _tcsncpy(character_name_, receiver, _countof(character_name_));
        _tcsncpy(guild_name_, guid_name, _countof(guild_name_));
        guild_name_[_countof(guild_name_) - 1] = '\0';
        character_name_[_countof(character_name_) - 1] = '\0'; 
    }

    bool MessageTo(const char* receiver, const char* message)
    {
        if (user_guid_ == 0)
            return false;

        if (character_guid_ == 0)
            return false;

        if (0 == _tcsncmp(character_name_, receiver, _countof(character_name_)))
        {
            _tcsncpy(gift_message_, message, _countof(gift_message_));
            gift_message_[_countof(gift_message_) - 1] = '\0';
            return true;
        }
        return false;
    }
};

struct ServerInfo
{
    ServerInfo()
    {
        game_code_ = SHOP_GAMECODE;
        sales_zone_ = SHOP_SALESZONE;
    }
    long game_code_;
    long sales_zone_;
    long server_index_;
    long server_guid_;
};


struct ItemSerial
{
    ItemSerial()
    {
        Clear();
    }

    void Clear()
    {
        item_code_ = 0;
        item_pos_ = 0;
        storage_sequence_ = 0;         // 보관함 순번
        storage_item_sequence_ = 0;    // 보관함 상품 시퀀스
    }

    SLOTCODE item_code_;
    POSTYPE item_pos_;
    long storage_sequence_;         // 보관함 순번
    long storage_item_sequence_;    // 보관함 상품 시퀀스
};


}; //namespace GameShop

#endif //GAMESHOP_H_

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
