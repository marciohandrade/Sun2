#include "stdafx.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "GameShop.User.h"
#include "GameShop.EventProcessor.h"

namespace GameShop
{

User::User(GameShop::EventProcessor* event_processor)
: event_processor_(event_processor)
{
    event_processor->Link(true);
}

User::~User()
{
    event_processor_->Link(false);
}


}; //namespace GameShop

#endif // _NA_0_20110613_GAME_SHOP_RENEWAL