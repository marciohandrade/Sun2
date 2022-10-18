#include "stdafx.h"


#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "GameShop.System.h"
#include "GameShop.Event.h"
#include "GameShop.EventProcessor.h"
#include "../GameServerEx.h"


namespace GameShop
{

void System::ErrorHandler(DWORD last_error, TCHAR* desc)
{
    SUNLOG(eCRITICAL_LOG, "GameShop::System::ErrorHandler Error : %d, Desc : %s", last_error, desc);
}

System::System()
{
}

System::~System()
{
    EVENT_PROCESSOR_HASH::iterator iter = event_processor_hash_.begin();
    for ( ; iter != event_processor_hash_.end() ; ++iter)
    {
        GameShop::EventProcessor* proc = iter->second;
        delete proc;
    }
    event_processor_hash_.clear();
}

void System::Init()
{
    server_info_.game_code_ = SHOP_GAMECODE;
    server_info_.sales_zone_ = SHOP_SALESZONE;
    server_info_.server_index_ = g_pGameServer->GetKey().GetWorldID();

    event_handler_.Init(this);
    event_handler_.CreateSession(GameShop::System::ErrorHandler);
}

void System::Update(uint delta_tick)
{
    event_queue_.Update();
    UpdateGarbage(delta_tick);
}

void System::UpdateGarbage(uint delta_tick)
{
    EVENT_PROCESSOR_HASH::iterator iter = event_processor_hash_.begin();
    for ( ; iter != event_processor_hash_.end() ; )
    {
        GameShop::EventProcessor* proc = iter->second;
        if (proc->ClearGarbage(delta_tick))
        {
            delete proc;
            iter = event_processor_hash_.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

GameShop::EventProcessor* System::FindEventProcessor(USERGUID user_guid)
{
    EVENT_PROCESSOR_HASH::iterator iter = event_processor_hash_.find(user_guid);
    if (iter != event_processor_hash_.end())
        return iter->second;
    return NULL;
}

GameShop::EventProcessor* System::CreateEventProcessor(USERGUID user_guid)
{
    GameShop::EventProcessor* processor = new GameShop::EventProcessor(user_guid, this);
    if (processor == NULL)
    {
        ASSERT(0);
        Log("[GameShop]CreateEventProcessor -> Alloc Memory Fail");
        return NULL;
    }
    event_processor_hash_[user_guid] = processor;
    return processor;
}

void System::Log(char* format, ...)
{
    LogEvent* new_event = new LogEvent;
    uint str_len = _countof(new_event->property_.log_string_) - 1;
    va_list marker;
    va_start(marker, format);
    _vsnprintf(new_event->property_.log_string_, str_len, format, marker);
    va_end(marker);
    new_event->property_.log_string_[str_len] = '\0';
    event_queue_.PushEvent(new_event);
}


}; // namespace GameShop



#endif // _NA_0_20110613_GAME_SHOP_RENEWAL