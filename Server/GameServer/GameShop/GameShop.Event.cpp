#include "stdafx.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include <strsafe.h>
#include "GameShop.Event.h"
#include "GameShop.Manager.h"
#include "GameShop.EventProcessor.h"

namespace GameShop
{

Event::Event()
{
}
Event::~Event()
{
}


EventQueue::EventQueue()
{
    ::InitializeCriticalSection(&cs_);
}

EventQueue::~EventQueue()
{
    ::EnterCriticalSection(&cs_);
    EVENTLIST::iterator iter = event_list_.begin();
    for ( ; iter != event_list_.end() ; ++iter)
    {
        GameShop::Event* new_event = (*iter);
        new_event->Process();
        delete new_event;
    }
    event_list_.clear();
    ::LeaveCriticalSection(&cs_);

    ::DeleteCriticalSection(&cs_);
}

void EventQueue::PushEvent(GameShop::Event* new_event)
{
    ::EnterCriticalSection(&cs_);
    event_list_.push_back(new_event);
    ::LeaveCriticalSection(&cs_);
}

void EventQueue::Update()
{
    GameShop::Event* new_event = NULL;
    ::EnterCriticalSection(&cs_);
    if (!event_list_.empty())
    {
        EVENTLIST::iterator iter = event_list_.begin();
        new_event = (*iter);
        event_list_.erase(iter);
    }
    ::LeaveCriticalSection(&cs_);

    if (new_event)
    {
        new_event->Process();
        delete new_event;
    }
}




class LogEventFilePath
{
public:
    LogEventFilePath()
    {
        log_path_[0] = '\0';
    }

    ~LogEventFilePath()
    {
    }

    char* GetLogFilePath()
    {
        if (strlen(log_path_) == 0)
        {
            char current_path[MAX_PATH];
            char ini_file[MAX_PATH];
            char temp_path[50];
            ::GetCurrentDirectory(MAX_PATH, current_path);

            StringCchPrintf(ini_file, MAX_PATH, "%s\\GameServer.ini", current_path);
            __READ_CONFIG_STRING(temp_path, "LOGFILE", "SUN_LOGFILE_PATH", "", ini_file);

            StringCchPrintf(log_path_, MAX_PATH, "%s", temp_path);
        }
        return log_path_;
    }
private:
    char log_path_[MAX_PATH];

};

LogEventFilePath g_logfilepath;

LogEvent::LogEvent()
{
}

LogEvent::~LogEvent()
{
}

void LogEvent::Process()
{
    char* logfilepath = g_logfilepath.GetLogFilePath();

    SYSTEMTIME local_time;
    GetLocalTime(&local_time);

    char file_path[MAX_PATH];
    StringCchPrintf(file_path, MAX_PATH, "%s\\Shop__Date_%04d%02d%02d.wzl", logfilepath, local_time.wYear, local_time.wMonth, local_time.wDay);

    char log_string[LogEvent::max_path];
    uint str_len = _countof(property_.log_string_) - 1;
    property_.log_string_[str_len] = '\0';
    StringCchPrintf(log_string, LogEvent::max_path, "[%02d:%02d:%02d]%s", local_time.wHour, local_time.wMinute, local_time.wSecond, property_.log_string_);

    uint log_length = strlen(log_string);
    HANDLE file_handle = CreateFile(file_path, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  //File »ý¼º 
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        DWORD write;
        DWORD file_pos = ::SetFilePointer(file_handle, 0, NULL, FILE_END);
        LockFile(file_handle, file_pos, 0, file_pos + log_length, 0);
        WriteFile(file_handle, log_string, log_length, &write, NULL); 
        UnlockFile(file_handle, file_pos, 0, file_pos + log_length, 0);     
        CloseHandle(file_handle); 
    }
}




ConnectEvent::ConnectEvent()
{
}

ConnectEvent::~ConnectEvent()
{
}

void ConnectEvent::Process()
{
    SUNLOG(eCRITICAL_LOG, "[GameShop]Shop Server Connect[%s][error : %d]", property_.success_?"SUCCESS":"FAIL", 
                                                                           property_.result_code_);

    if (property_.success_)
    {
        GameShop::Manager* manager = GameShop::Manager::Instance();
        GameShop::System& shop_system = manager->get_shop_system();
        GameShop::EventHandler& handler = shop_system.get_event_handler();
        GameShop::ServerInfo& server_info = shop_system.get_server_info();
        handler.InquireSalesZoneScriptVersion(server_info.game_code_, server_info.sales_zone_);
    }
}

DisConnectEvent::DisConnectEvent()
{
}

DisConnectEvent::~DisConnectEvent()
{
}

void DisConnectEvent::Process()
{
    SUNLOG(eCRITICAL_LOG, "[GameShop]Shop Server Disconnect!![error : %d]", property_.result_code_);
    GameShop::Manager* manager = GameShop::Manager::Instance();
    manager->NotifyCloseShopToAllUser(RC::RC_SHOP_SERVER_CLOSED);
}


SalesZoneScriptEvent::SalesZoneScriptEvent()
{
}

SalesZoneScriptEvent::~SalesZoneScriptEvent()
{
}

void SalesZoneScriptEvent::Process()
{
    if (property_.result_code_ == 0)
    {
        GameShop::Manager* manager = GameShop::Manager::Instance();
        GameShop::System& shop_system = manager->get_shop_system();

        ShopScriptVersion& script_version = shop_system.get_script_version();
        script_version.sales_zone_.sales_zone_ = property_.sales_zone_;
        script_version.sales_zone_.year_ = property_.year_;
        script_version.sales_zone_.year_identity_ = property_.year_identity_;
        manager->NotifyCloseShopToAllUser();
    }

    SUNLOG(eCRITICAL_LOG, "[GameShop]Script Version : %d.%d.%d Result : %d", property_.sales_zone_, 
                                                                             property_.year_, 
                                                                             property_.year_identity_, 
                                                                             property_.result_code_);
}




CashInfoEvent::CashInfoEvent()
{
}

CashInfoEvent::~CashInfoEvent()
{
}

void CashInfoEvent::Process()
{
    GameShop::Manager* manager = GameShop::Manager::Instance();
    GameShop::EventProcessor* proc = manager->FindEventProcessor(property_.account_seq_);
    if (proc == NULL)
    {
        ASSERT(0);
        SUNLOG(eCRITICAL_LOG, "[GameShop]CashInfoEvent -> Not Found Event Processor : %d", property_.account_seq_);
        return;
    }

    CashInfo cash_info;
    cash_info.cash_ = property_.cash_sum_;
    cash_info.mileage_ = property_.mileage_sum_;
    cash_info.pp_cash_ = property_.ppcash_sum_;
    proc->Receive_CashInfo(property_.result_code_, cash_info);
}



BuyProductEvent::BuyProductEvent()
{
}

BuyProductEvent::~BuyProductEvent()
{
}

void BuyProductEvent::Process()
{
    GameShop::Manager* manager = GameShop::Manager::Instance();
    GameShop::EventProcessor* proc = manager->FindEventProcessor(property_.account_seq_);
    if (proc == NULL)
    {
        ASSERT(0);
        SUNLOG(eCRITICAL_LOG, "[GameShop]BuyProductEvent -> Not Found Event Processor : %d", property_.account_seq_);
        return;
    }
    proc->Receive_BuyItem(property_.result_code_, property_.left_product_count_);
}



GiftProductEvent::GiftProductEvent()
{
}

GiftProductEvent::~GiftProductEvent()
{
}

void GiftProductEvent::Process()
{
    GameShop::Manager* manager = GameShop::Manager::Instance();
    GameShop::EventProcessor* proc = manager->FindEventProcessor(property_.sender_seq_);
    if (proc == NULL)
    {
        ASSERT(0);
        SUNLOG(eCRITICAL_LOG, "[GameShop]GiftProductEvent -> Not Found Event Processor : %d", property_.sender_seq_);
        return;
    }
    proc->Receive_SendGift(property_.result_code_, property_.receiver_seq_, property_.limited_cash_, property_.left_product_count_);
}




BuyStorageListEvent::BuyStorageListEvent()
{
}

BuyStorageListEvent::~BuyStorageListEvent()
{
}

void BuyStorageListEvent::Process()
{
    GameShop::Manager* manager = GameShop::Manager::Instance();
    GameShop::EventProcessor* proc = manager->FindEventProcessor(property_.account_seq_);
    if (proc == NULL)
    {
        ASSERT(0);
        SUNLOG(eCRITICAL_LOG, "[GameShop]BuyStorageListEvent -> Not Found Event Processor : %d", property_.account_seq_);
        return;
    }

    proc->Receive_BuyStorageList(property_.result_code_,
                                 property_.storage_type_,
                                 property_.now_page_, 
                                 property_.total_page_,
                                 property_.total_count_,
                                 property_.list_count_,
                                 property_.storage_list_);
}



GiftStorageListEvent::GiftStorageListEvent()
{
}

GiftStorageListEvent::~GiftStorageListEvent()
{
}

void GiftStorageListEvent::Process()
{
    GameShop::Manager* manager = GameShop::Manager::Instance();
    GameShop::EventProcessor* proc = manager->FindEventProcessor(property_.account_seq_);
    if (proc == NULL)
    {
        ASSERT(0);
        SUNLOG(eCRITICAL_LOG, "[GameShop]GiftStorageListEvent -> Not Found Event Processor : %d", property_.account_seq_);
        return;
    }

    proc->Receive_GiftStorageList(property_.result_code_,
                                  property_.storage_type_,
                                  property_.now_page_, 
                                  property_.total_page_,
                                  property_.total_count_,
                                  property_.list_count_,
                                  property_.storage_list_);
}



UseStorageItemEvent::UseStorageItemEvent()
{
}

UseStorageItemEvent::~UseStorageItemEvent()
{
}

void UseStorageItemEvent::Process()
{
    GameShop::Manager* manager = GameShop::Manager::Instance();
    GameShop::EventProcessor* proc = manager->FindEventProcessor(property_.account_seq_);
    if (proc == NULL)
    {
        ASSERT(0);
        SUNLOG(eCRITICAL_LOG, "[GameShop]UseStorageItemEvent -> Not Found Event Processor : %d", property_.account_seq_);
        return;
    }

    proc->Receive_UseStorageItem(property_.result_code_, 
                                 property_.game_item_code_,
                                 property_.product_seq_,
                                 property_.storage_seq_,
                                 property_.storage_item_seq_,
                                 property_.property_count_,
                                 property_.property_list_, 
                                 property_.cash_type_);
}


UseStorageItemRollBackEvent::UseStorageItemRollBackEvent()
{
}

UseStorageItemRollBackEvent::~UseStorageItemRollBackEvent()
{
}

void UseStorageItemRollBackEvent::Process()
{
    GameShop::Manager* manager = GameShop::Manager::Instance();
    GameShop::EventProcessor* proc = manager->FindEventProcessor(property_.account_seq_);
    if (proc == NULL)
    {
        ASSERT(0);
        SUNLOG(eCRITICAL_LOG, "[GameShop]UseStorageItemEvent -> Not Found Event Processor : %d", property_.account_seq_);
        return;
    }

    proc->Receive_Item_RollBack(property_.result_code_); 
}


}; //namespace GameShop

#endif // _NA_0_20110613_GAME_SHOP_RENEWAL