#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef GAMESHOP_USER_H_
#define GAMESHOP_USER_H_


#if _MSC_VER > 1000
#pragma once
#endif

//#include "GameShopHeader.h"

namespace GameShop
{

class EventProcessor;

class User
{
public:
    User(GameShop::EventProcessor* event_processor);
    ~User(void);

    GameShop::EventProcessor* get_event_processor();

private:
    GameShop::EventProcessor* event_processor_;
};

//----------------------------------------------------------------------
inline GameShop::EventProcessor* User::get_event_processor()
{
    return event_processor_;
}

}; //namespace GameShop

#endif //GAMESHOP_USER_H_

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
