#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef GAMESHOP_EVENT_H_
#define GAMESHOP_EVENT_H_


#if _MSC_VER > 1000
#pragma once
#endif

//#include <public/protocol_shop.h>

namespace GameShop
{
    class Event
    {
    public:
        Event();
        virtual ~Event();
        virtual void Process() = 0;
    };

    class EventQueue
    {
    public:
        EventQueue();
        ~EventQueue();
        void PushEvent(GameShop::Event* new_event);
        void Update();
    private:
        typedef std::list<GameShop::Event*> EVENTLIST;
        EVENTLIST        event_list_;
        CRITICAL_SECTION cs_;
    };


    //=======================================================
    //
    //
    //=======================================================
    class LogEvent : public Event
    {
    public:
        enum { max_path = 4200, };
        struct Property
        {
            char log_string_[max_path];
        };
        Property property_;

    public:
        LogEvent();
        virtual ~LogEvent();

        virtual void Process();
    };

    //=======================================================
    //
    //
    //=======================================================

    class ConnectEvent : public Event
    {
    public:
        struct Property
        {
            long result_code_;
            bool success_;
        };
        Property property_;

    public:
        ConnectEvent();
        virtual ~ConnectEvent();
        virtual void Process();
    };

    //=======================================================
    //
    //
    //=======================================================
    class DisConnectEvent : public Event
    {
    public:
        struct Property
        {
            long result_code_;
        };
        Property property_;
    public:
        DisConnectEvent();
        virtual ~DisConnectEvent();
        virtual void Process();
    };

    //=======================================================
    //
    //
    //=======================================================
    class SalesZoneScriptEvent : public Event
    {
    public:
        struct Property
        {
            long result_code_;
            ushort sales_zone_;
            ushort year_;
            ushort year_identity_;
        };
        Property property_;

    public:
        SalesZoneScriptEvent();
        virtual ~SalesZoneScriptEvent();
        virtual void Process();
    };

    //=======================================================
    //
    //
    //=======================================================
    struct CashInfoEvent : public Event
    {
    public:
        struct Property
        {
            DWORD account_seq_;
            double cash_sum_;
            double point_sum_;
            double mileage_sum_;
            double ppcash_sum_;
            long result_code_;
        };
        Property property_;

    public:
        CashInfoEvent();
        virtual ~CashInfoEvent();
        virtual void Process();
    };

    //=======================================================
    //
    //
    //=======================================================
    class BuyProductEvent : public Event
    {
    public:
        struct Property
        {
            DWORD account_seq_;
            long left_product_count_;
            long result_code_;
        };
        Property property_;

    public:
        BuyProductEvent();
        virtual ~BuyProductEvent();
        virtual void Process();
    };

    //=======================================================
    //
    //
    //=======================================================
    class GiftProductEvent : public Event
    {
    public:
        struct Property
        {
            DWORD sender_seq_;
            DWORD receiver_seq_;
            double limited_cash_;
            long left_product_count_;
            long result_code_;
        };
        Property property_;

    public:
        GiftProductEvent();
        virtual ~GiftProductEvent();
        virtual void Process();
    };

    //=======================================================
    //
    //
    //=======================================================
    class BuyStorageListEvent : public Event
    {
    public:
        struct Property
        {
            DWORD account_seq_; 
            int	result_code_;
            char storage_type_;
            int now_page_;
            int total_page_;
            int total_count_;
            int list_count_;
            ShopProtocol::STStorageNoGiftMessageEx storage_list_[BUY_STORAGE_ITEM_PER_PAGE];
        };
        Property property_;

    public:
        BuyStorageListEvent();
        virtual ~BuyStorageListEvent();
        virtual void Process();
    };


 
    //=======================================================
    //
    //
    //=======================================================
    class GiftStorageListEvent : public Event
    {
    public:
        struct Property
        {
            DWORD account_seq_; 
            int	result_code_;
            char storage_type_;
            int now_page_;
            int total_page_;
            int total_count_;
            int list_count_;
            ShopProtocol::STStorageEx storage_list_[GIFT_STORAGE_ITEM_PER_PAGE];
        };
        Property property_;

    public:
        GiftStorageListEvent();
        virtual ~GiftStorageListEvent();
        virtual void Process();
    };


    //=======================================================
    //
    //
    //=======================================================
    class UseStorageItemEvent : public Event
    {
    public:
        struct Property
        {
            DWORD account_seq_; 
            int	result_code_;
            uint game_item_code_;
            BYTE property_count_;
            long product_seq_; 						   
            long storage_seq_;
            long storage_item_seq_;
            int cash_type_;
            bool deduct_mileage_flag_;
            ShopProtocol::STItemProperty property_list_[SHOP_MAX_ITEM_PROPERTY_COUNT];
        };
        Property property_;

    public:
        UseStorageItemEvent();
        virtual ~UseStorageItemEvent();
        virtual void Process();
    };


    //=======================================================
    //
    //
    //=======================================================
    class UseStorageItemRollBackEvent : public Event
    {
    public:
        struct Property
        {
            DWORD account_seq_; 
            int	result_code_;
        };
        Property property_;

    public:
        UseStorageItemRollBackEvent();
        virtual ~UseStorageItemRollBackEvent();
        virtual void Process();
    };


}; //namespace GameShop

#endif //GAMESHOP_EVENT_H_

#endif // _NA_0_20110613_GAME_SHOP_RENEWAL