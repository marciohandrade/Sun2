#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef GAMESHOP_SYSTEM_H_
#define GAMESHOP_SYSTEM_H_


#if _MSC_VER > 1000
#pragma once
#endif

#include "GameShop.h"
#include "GameShop.Event.h"
#include "GameShop.EventHandler.h"

namespace GameShop
{

class EventHandler;
class EventProcessor;

class System
{
public:
    typedef STLX_HASH_MAP<USERGUID, GameShop::EventProcessor*> EVENT_PROCESSOR_HASH;

public:
    System(void);
    virtual ~System(void);

    static void ErrorHandler(DWORD last_error, TCHAR* desc);

    void Init();
    void Update(uint delta_tick);

    void Log(char* format, ...);

    GameShop::ServerInfo& get_server_info();
    GameShop::EventHandler& get_event_handler();
    ShopScriptVersion& get_script_version();
    GameShop::EventQueue& get_event_queue();

    GameShop::EventProcessor* FindEventProcessor(USERGUID user_guid);
    GameShop::EventProcessor* CreateEventProcessor(USERGUID user_guid);

private:
    void UpdateGarbage(uint delta_tick);

private:
    ShopScriptVersion       script_version_;
    GameShop::ServerInfo    server_info_;
    GameShop::EventHandler  event_handler_;
    EVENT_PROCESSOR_HASH    event_processor_hash_;
    GameShop::EventQueue    event_queue_;
    //
    __DISABLE_COPY(GameShop::System);
};

//------------------------------------------------------------------
inline GameShop::EventQueue& System::get_event_queue()
{
    return event_queue_;
}

//------------------------------------------------------------------
inline GameShop::ServerInfo& System::get_server_info()
{
    return server_info_;
}

//------------------------------------------------------------------
inline GameShop::EventHandler& System::get_event_handler()
{
    return event_handler_;
}


//------------------------------------------------------------------
inline ShopScriptVersion& System::get_script_version()
{
    return script_version_;
}

}; // namespace GameShop


#endif //GAMESHOP_SYSTEM_H_

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
