#include "stdafx.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "GameShop.EventHandler.h"
#include "GameShop.System.h"
#include "GameShop.Event.h"


namespace GameShop
{

EventHandler::EventHandler()
: connect_(false)
, shop_system_(NULL)
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::OnNetConnect(bool success, DWORD error)
{
    connect_ = (error == 0) ? true : false;
    ConnectEvent* new_event = new ConnectEvent;
    new_event->property_.result_code_ = error;
    new_event->property_.success_ = success;
    shop_system_->get_event_queue().PushEvent(new_event);
}

void EventHandler::OnNetSend(int size)
{
}

void EventHandler::OnNetClose(DWORD error)
{
    connect_ = false;
    DisConnectEvent* new_event = new DisConnectEvent;
    new_event->property_.result_code_ = error;
    shop_system_->get_event_queue().PushEvent(new_event);
}

// 상품 샵 스크립트 최신 버전 정보 조회
void EventHandler::OnInquireSalesZoneScriptVersion(unsigned short SalesZone, 
                                                   unsigned short Year, 
                                                   unsigned short YearIdentity, 
                                                   long ResultCode)
{
    SalesZoneScriptEvent* new_event = new SalesZoneScriptEvent;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.sales_zone_ = SalesZone;
    new_event->property_.year_ = Year;
    new_event->property_.year_identity_ = YearIdentity;
    shop_system_->get_event_queue().PushEvent(new_event);
}


// 배너 스크립트 최신 버전 정보 조회
void EventHandler::OnInquireBannerZoneScriptVersion(unsigned short BannerZone, 
                                                    unsigned short Year, 
                                                    unsigned short YearIdentity, 
                                                    long ResultCode)
{
}


// MU KR, Battery, R2
void EventHandler::OnInquireCash(DWORD AccountSeq, 
                                 double CashSum, 
                                 double PointSum, 
                                 double MileageSum, 
                                 int DetailCount, 
                                 ShopProtocol::STCashDetail Detail[], 
                                 long ResultCode)
{
    CashInfoEvent* new_event = new CashInfoEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.cash_sum_ = CashSum;
    new_event->property_.ppcash_sum_ = 0;
    new_event->property_.mileage_sum_ = MileageSum;
    new_event->property_.result_code_ = ResultCode;
    shop_system_->get_event_queue().PushEvent(new_event);
}


// MU GB, SUN Global, SUN China
void EventHandler::OnInquireCash(DWORD AccountSeq, 
                                 double CashSum, 
                                 double PointSum, 
                                 double MileageSum, 
                                 int ListCount, 
                                 ShopProtocol::STCashDetail_GB Detail[], 
                                 long ResultCode)
{
    CashInfoEvent* new_event = new CashInfoEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.cash_sum_ = 0;
    new_event->property_.ppcash_sum_ = 0;

    for (uint idx = 0 ; idx < ListCount ; ++idx)
    {
        switch (Detail[idx].CashTypeCode)
        {
        case 508:
            new_event->property_.cash_sum_ = Detail[idx].Value;
            break;
        case 509:
            new_event->property_.ppcash_sum_ = Detail[idx].Value;
            break;
        }
    } 

    new_event->property_.point_sum_ = PointSum;
    new_event->property_.mileage_sum_ = MileageSum;
    new_event->property_.result_code_ = ResultCode;
    shop_system_->get_event_queue().PushEvent(new_event);
}

// SUN JAPAN(GameChu, GameOn), SUN RUSSIA
void EventHandler::OnInquireCash(DWORD AccountSeq, 
                                 long ResultCode,
                                 long OutBoundResultCode,
                                 double CashSum, 
                                 double MileageSum)
{
    CashInfoEvent* new_event = new CashInfoEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.cash_sum_ = CashSum;
    new_event->property_.ppcash_sum_ = 0;
    new_event->property_.mileage_sum_ = MileageSum;
    new_event->property_.result_code_ = ResultCode;
    shop_system_->get_event_queue().PushEvent(new_event);
}

// Hx
void EventHandler::OnInquireCash(DWORD AccountSeq, 
                                 char AccoundID[MAX_ACCOUNTID_LENGTH], 
                                 double CashSum, 
                                 double PointSum, 
                                 double MileageSum, 
                                 int DetailCount, 
                                 ShopProtocol::STCashDetail Detail[], 
                                 long ResultCode,
                                 long OutBoundResultCode)
{
}


// MU KR, GB, Battery, R2
void EventHandler::OnBuyProduct(DWORD AccountSeq, 
                                long ResultCode, 
                                long LeftProductCount)
{
    BuyProductEvent* new_event = new BuyProductEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.left_product_count_ = LeftProductCount;
    shop_system_->get_event_queue().PushEvent(new_event);
}

// MU JP - GameChu, GameOn, SUN CHINA
void EventHandler::OnBuyProduct(DWORD AccountSeq, 
                                long ResultCode,
                                long OutBoundResultCode,
                                long LeftProductCount)
{
    BuyProductEvent* new_event = new BuyProductEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.left_product_count_ = LeftProductCount;
    shop_system_->get_event_queue().PushEvent(new_event);
}

// Hx
void EventHandler::OnBuyProduct(DWORD AccountSeq, 
                                char AccoundID[MAX_ACCOUNTID_LENGTH], 
                                long LeftProductCount, 
                                long ResultCode,
                                long OutBoundResultCode)
{
}

// MU KR, GB, Battery, R2
void EventHandler::OnGiftProduct(DWORD SenderSeq, 
                                 DWORD ReceiverSeq, 
                                 double LimitedCash, 
                                 long LeftProductCount, 
                                 long ResultCode)
{
    GiftProductEvent* new_event = new GiftProductEvent;
    new_event->property_.sender_seq_ = SenderSeq;
    new_event->property_.receiver_seq_ = ReceiverSeq;
    new_event->property_.limited_cash_ = LimitedCash;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.left_product_count_ = LeftProductCount;
    shop_system_->get_event_queue().PushEvent(new_event);
}

// MU JP - GameChu, GameOn 
void EventHandler::OnGiftProduct(DWORD SenderAccountSeq, 
                                 long ResultCode,
                                 long OutBoundResultCode,
                                 DWORD ReceiverAccountSeq, 
                                 long LeftProductCount)
{
    GiftProductEvent* new_event = new GiftProductEvent;
    new_event->property_.sender_seq_ = SenderAccountSeq;
    new_event->property_.receiver_seq_ = ReceiverAccountSeq;
    new_event->property_.limited_cash_ = 0.0f;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.left_product_count_ = LeftProductCount;
    shop_system_->get_event_queue().PushEvent(new_event);
}

// Hx
void EventHandler::OnGiftProduct(DWORD SenderAccountSeq, 
                                 char SenderAccountID[MAX_ACCOUNTID_LENGTH], 
                                 WCHAR SenderCharName[MAX_CHARACTERID_LENGTH], 
                                 DWORD ReceiverAccountSeq, 
                                 char ReceiverAccountID[MAX_ACCOUNTID_LENGTH], 
                                 WCHAR ReceiverCharName[MAX_CHARACTERID_LENGTH], 
                                 WCHAR Message[MAX_MESSAGE_LENGTH], 
                                 double LimitedCash, 
                                 long LeftProductCount, 
                                 long ResultCode,
                                 long OutBoundResultCode)
{
}

void EventHandler::OnInquireStorageList(DWORD AccountID, 
                                        long ListCount, 
                                        long ResultCode, 
                                        ShopProtocol::STStorage StorageList[])
{
}

void EventHandler::OnInquireStorageListPage(DWORD AccountID, 
                                            int	ResultCode, 
                                            char StorageType,
                                            int NowPage,
                                            int TotalPage,
                                            int TotalCount,
                                            int ListCount, 
                                            ShopProtocol::STStorage StorageList[])
{
}

void EventHandler::OnInquireStorageListPageNoGiftMessage(DWORD AccountID, 
                                                         int ResultCode, 
                                                         char StorageType,
                                                         int NowPage,
                                                         int TotalPage,
                                                         int TotalCount,
                                                         int ListCount, 
                                                         ShopProtocol::STStorageNoGiftMessage StorageList[])
{
}

// 보관함 조회 결과 + 만료시점 추가
void EventHandler::OnInquireStorageListEx(DWORD AccountID, 
                                    long ListCount, 
                                    long ResultCode, 
                                    ShopProtocol::STStorageEx StorageListEx[])
{

}

// 보관함 페이지 조회 결과 + 만료시점 추가
void EventHandler::OnInquireStorageListPageEx(DWORD AccountID, 
                                        int	ResultCode, 
                                        char  StorageType,
                                        int	NowPage,
                                        int   TotalPage,
                                        int   TotalCount,
                                        int   ListCount, 
                                        ShopProtocol::STStorageEx StorageListEx[])
{
    GiftStorageListEvent* new_event = new GiftStorageListEvent;
    new_event->property_.account_seq_ = AccountID;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.storage_type_ = StorageType;
    new_event->property_.now_page_ = NowPage;
    new_event->property_.total_page_ = TotalPage;
    new_event->property_.total_count_ = TotalCount;
    new_event->property_.list_count_ = ListCount;
    memcpy(new_event->property_.storage_list_, StorageListEx, sizeof(StorageListEx[0]) * ListCount);
    shop_system_->get_event_queue().PushEvent(new_event);
}

// 보관함 페이지 조회 결과 - 선물 메시지 제외 + 만료시점 추가
void EventHandler::OnInquireStorageListPageNoGiftMessageEx(DWORD AccountID, 
                                                     int   ResultCode, 
                                                     char  StorageType,
                                                     int   NowPage,
                                                     int   TotalPage,
                                                     int   TotalCount,
                                                     int   ListCount, 
                                                     ShopProtocol::STStorageNoGiftMessageEx StorageListEx[])
{
    BuyStorageListEvent* new_event = new BuyStorageListEvent;
    new_event->property_.account_seq_ = AccountID;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.storage_type_ = StorageType;
    new_event->property_.now_page_ = NowPage;
    new_event->property_.total_page_ = TotalPage;
    new_event->property_.total_count_ = TotalCount;
    new_event->property_.list_count_ = ListCount;
    memcpy(new_event->property_.storage_list_, StorageListEx, sizeof(StorageListEx[0]) * ListCount);
    shop_system_->get_event_queue().PushEvent(new_event);
}

void EventHandler::OnGiftCash(DWORD SenderSeq, 
                              DWORD ReceiverSeq, 
                              long ResultCode, 
                              double GiftSendLimit)
{
}

void EventHandler::OnInquireBuyGiftPossibility(DWORD AccountID, 
                                               long ResultCode, 
                                               long ItemBuy, 
                                               long Present, 
                                               double MyPresentCash, 
                                               double MyPresentSendLimit)
{
}

void EventHandler::OnInquireEventProductList(DWORD AccountSeq, 
                                             long ProductDisplaySeq, 
                                             int PackagesCount, 
                                             long Packages[])
{

}

void EventHandler::OnInquireProductLeftCount(long PackageSeq, long LeftCount)
{
}

void EventHandler::OnUpdateVersion(long GameCode, 
                                   unsigned short SalesZone, 
                                   unsigned short year, 
                                   unsigned short yearIdentity)
{
    SalesZoneScriptEvent* new_event = new SalesZoneScriptEvent;
    new_event->property_.result_code_ = 0;
    new_event->property_.sales_zone_ = SalesZone;
    new_event->property_.year_ = year;
    new_event->property_.year_identity_ = yearIdentity;
    shop_system_->get_event_queue().PushEvent(new_event);
}

void EventHandler::OnUpdateBannerVersion(long GameCode, 
                                         unsigned short BannerZone, 
                                         unsigned short year, 
                                         unsigned short yearIdentity)
{
}

void EventHandler::OnUseStorage(DWORD AccountSeq, 
                                long ResultCode, 
                                char InGameProductID[MAX_TYPENAME_LENGTH], 
                                BYTE PropertyCount, 
                                long ProductSeq, 						   
                                long StorageSeq, 
                                long StorageItemSeq, 
                                ShopProtocol::STItemProperty PropertyList[])
{
    UseStorageItemEvent* new_event = new UseStorageItemEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.game_item_code_ = (uint)atoi(InGameProductID);
    new_event->property_.property_count_ = PropertyCount;
    new_event->property_.product_seq_ = ProductSeq;
    new_event->property_.storage_seq_ = StorageSeq;
    new_event->property_.storage_item_seq_ = StorageItemSeq;
    new_event->property_.cash_type_ = 0;
    if (SHOP_MAX_ITEM_PROPERTY_COUNT < PropertyCount)
        PropertyCount = SHOP_MAX_ITEM_PROPERTY_COUNT;
    memcpy(new_event->property_.property_list_, PropertyList, sizeof(PropertyList[0]) * PropertyCount);
    shop_system_->get_event_queue().PushEvent(new_event);
}

void EventHandler::OnUseStorageEx(DWORD AccountSeq, 
                                  long ResultCode, 
                                  char InGameProductID[MAX_TYPENAME_LENGTH], 
                                  BYTE PropertyCount, 
                                  long ProductSeq, 						   
                                  long StorageSeq, 
                                  long StorageItemSeq, 
                                  ShopProtocol::STItemProperty PropertyList[],
                                  int CashType)
{
    UseStorageItemEvent* new_event = new UseStorageItemEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.game_item_code_ = (uint)atoi(InGameProductID);
    new_event->property_.property_count_ = PropertyCount;
    new_event->property_.product_seq_ = ProductSeq;
    new_event->property_.storage_seq_ = StorageSeq;
    new_event->property_.storage_item_seq_ = StorageItemSeq;
    new_event->property_.cash_type_ = CashType;
    new_event->property_.deduct_mileage_flag_ = false;
    if (SHOP_MAX_ITEM_PROPERTY_COUNT < PropertyCount)
        PropertyCount = SHOP_MAX_ITEM_PROPERTY_COUNT;
    memcpy(new_event->property_.property_list_, PropertyList, sizeof(PropertyList[0]) * PropertyCount);
    shop_system_->get_event_queue().PushEvent(new_event);
}

// 보관함 상품 사용하기 결과 Ex : SUN_CN
//	DeductMileageFlag 추가 (false : 캐시상품(0), true : 마일리지 상품(1))
void EventHandler::OnUseStorageExDeductMileageFlag(DWORD	AccountSeq,
                                                    long	ResultCode,
                                                    char	InGameProductID[MAX_TYPENAME_LENGTH],
                                                    BYTE	PropertyCount,
                                                    long	ProductSeq,
                                                    long	StorageSeq,
                                                    long	StorageItemSeq,
                                                    ShopProtocol::STItemProperty PropertyList[],
                                                    bool	DeductMileageFlag)
{
    UseStorageItemEvent* new_event = new UseStorageItemEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.result_code_ = ResultCode;
    new_event->property_.game_item_code_ = (uint)atoi(InGameProductID);
    new_event->property_.property_count_ = PropertyCount;
    new_event->property_.product_seq_ = ProductSeq;
    new_event->property_.storage_seq_ = StorageSeq;
    new_event->property_.storage_item_seq_ = StorageItemSeq;
    new_event->property_.cash_type_ = 0;
    new_event->property_.deduct_mileage_flag_ = DeductMileageFlag;
    if (SHOP_MAX_ITEM_PROPERTY_COUNT < PropertyCount)
        PropertyCount = SHOP_MAX_ITEM_PROPERTY_COUNT;
    memcpy(new_event->property_.property_list_, PropertyList, sizeof(PropertyList[0]) * PropertyCount);
    shop_system_->get_event_queue().PushEvent(new_event);
}
// 보관함 상품 사용하기 롤백
void EventHandler::OnRollbackUseStorage(DWORD AccountSeq, long ResultCode)
{
    UseStorageItemRollBackEvent* new_event = new UseStorageItemRollBackEvent;
    new_event->property_.account_seq_ = AccountSeq;
    new_event->property_.result_code_ = ResultCode;
    shop_system_->get_event_queue().PushEvent(new_event);
}

void EventHandler::OnMileageDeduct(DWORD AccountSeq, long ResultCode)
{
}

void EventHandler::OnMileageSave(DWORD AccountSeq, long ResultCode)
{
}

void EventHandler::OnMileageLiveSaveUp(DWORD AccountSeq, long ResultCode)
{
}

void EventHandler::OnThrowStorage(DWORD AccountSeq, long ResultCode)
{
}

void EventHandler::OnItemSerialUpdate(DWORD AccountSeq, long ResultCode)
{
}

void EventHandler::OnInquireInGamePointValue(long ResultCode,
                                             long PointCount,
                                             ShopProtocol::STPointDetail PointList[])
{
}


// 로그를 써야할때 보내는 이벤트 메소드
void EventHandler::WriteLog(char* szMsg)
{
    LogEvent* new_event = new LogEvent;

    uint str_len = _countof(new_event->property_.log_string_) - 1;
    strncpy(new_event->property_.log_string_, szMsg, str_len);
    new_event->property_.log_string_[str_len] = '\0';

    shop_system_->get_event_queue().PushEvent(new_event);
}

}; //namaspace GameShop

#endif // _NA_0_20110613_GAME_SHOP_RENEWAL