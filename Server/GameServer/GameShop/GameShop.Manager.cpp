#include "stdafx.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "GameShop.Manager.h"
#include "GameShop.EventHandler.h"
#include "GameShop.EventProcessor.h"
#include "GameShop.User.h"

GameShop::Manager::Manager()
{
    ZeroMemory(&connection_info_, sizeof(connection_info_));
}

GameShop::Manager::~Manager()
{
    UserHash::iterator iter = user_hash_.begin();
    for ( ; iter != user_hash_.end() ; ++iter)
    {
        GameShop::User* user = iter->second;
        delete iter->second;
    }
    user_hash_.clear();
}

void GameShop::Manager::Initialize(const char* ip_address, WORD port)
{
    strncpy(connection_info_.ip_address, ip_address, _countof(connection_info_.ip_address));
    connection_info_.ip_address[_countof(connection_info_.ip_address) - 1] = '\0';
    connection_info_.port = port;
}

void GameShop::Manager::StartService()
{
    shop_system_.Init();
    update_timer_.SetTimer(8 * 1000);
}

void GameShop::Manager::Update(uint delta_tick)
{
    UpdateConnect(delta_tick);
    shop_system_.Update(delta_tick);
}

void GameShop::Manager::NotifyCloseShopToAllUser(RC::eSHOP_RESULT result)
{
    UserHash::iterator iter = user_hash_.begin();
    for ( ; iter != user_hash_.end() ; ++iter)
    {
        GameShop::User* user = iter->second;
        user->get_event_processor()->CloseShop(result);
        delete iter->second;
    }
    user_hash_.clear();
}

void GameShop::Manager::UpdateConnect(uint delta_tick)
{
    if (update_timer_.IsExpired() == false) 
        return;

    if (connection_info_.port == 0)
        return;

    if (strlen(connection_info_.ip_address) == 0)
        return;

    GameShop::EventHandler& event_handler = shop_system_.get_event_handler();
    if (event_handler.IsConnect() == false)
    {
        shop_system_.Log("Retry ShopServer Connect[%s:%d]", connection_info_.ip_address, connection_info_.port);
        event_handler.Connect(connection_info_.ip_address, connection_info_.port);
    }
}

void GameShop::Manager::Request_ScriptVersion(Player* player)
{
    GameShop::EventHandler& event_handler = shop_system_.get_event_handler();
    if (!event_handler.IsConnect())
    {
        MSG_CG_SHOP_VERSION_NAK nak_message;
        nak_message.error_code_ = RC::RC_SHOP_SERVER_CLOSED;
        player->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    ShopScriptVersion& version = shop_system_.get_script_version();
    if (!version.sales_zone_.IsValid())
    {
        MSG_CG_SHOP_VERSION_NAK nak_message;
        nak_message.error_code_ = RC::RC_SHOP_INVALID_SCRIPT_VERSION;
        player->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    MSG_CG_SHOP_VERSION_ACK ack_message;
    ack_message.sales_version_ = version.sales_zone_;
    player->SendPacket(&ack_message, sizeof(ack_message));
}

void GameShop::Manager::Request_ExistGiftNotify(Player* player)
{
    // 샵서버가 열려 있는가?
    GameShop::EventHandler& event_handler = shop_system_.get_event_handler();
    if (!event_handler.IsConnect())
    {
        MSG_CG_SHOP_EXIST_GIFT_NAK nak_message;
        player->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    USERGUID user_guid = player->GetUserGuid();
    GameShop::User* user = FindShopUser(user_guid);
    bool create_user = false;
    if (user == NULL)
    {
        user = CreateShopUser(user_guid);
        create_user = true;
    }

    if (user)
    {
        user->get_event_processor()->Request_ExistGiftList();
    }
    else
    {
        MSG_CG_SHOP_EXIST_GIFT_NAK nak_message;
        player->SendPacket(&nak_message, sizeof(nak_message));
    }

    if (create_user)
    {
        // 선물의 존재 여부 요청후 샵 유저 삭제.
        // 샵 유저가 삭제되어도 샵 유저와 연결되어있는 EventProcessor는 일정 시간(5초)동안 유지된다.
        // EventProcessor 가 유지 되는 동안 응답을 받는다.
        // EventProcessor는 응답 후 더이상 요청된 사항이 없는 경우 자동 삭제된다.
        DeleteShopUser(user_guid);
    }
}

void GameShop::Manager::OpenShop(Player* player, ShopScriptVersion& script_version)
{
    ShopScriptVersion& version = shop_system_.get_script_version();
    if (!version.sales_zone_.IsEqual(script_version.sales_zone_))
    {
        EventProcessor::SendFail_Open(player, RC::RC_SHOP_DIFFERENT_SCRIPT_VERSION);
        return;
    }

    USERGUID user_guid = player->GetUserGuid();
    GameShop::User* user = FindShopUser(user_guid);
    if (user == NULL)
    {
        user = CreateShopUser(user_guid);
    }

    if (user)
    {
        user->get_event_processor()->OpenShop();
    }
    else
    {
        EventProcessor::SendFail_Open(player, RC::RC_SHOP_CANNOT_USE_SHOP);
    }
}

void GameShop::Manager::CloseShop(Player* player)
{
    USERGUID user_guid = player->GetUserGuid();
    DeleteShopUser(user_guid);
}


GameShop::User* GameShop::Manager::CreateShopUser(USERGUID user_guid)
{
    GameShop::EventProcessor* processor = shop_system_.FindEventProcessor(user_guid);
    if (processor == NULL)
        processor = shop_system_.CreateEventProcessor(user_guid);

    GameShop::User* user = new GameShop::User(processor);
    user_hash_[user_guid] = user;
    return user;
}

GameShop::User* GameShop::Manager::FindShopUser(USERGUID user_guid)
{
    UserHash::iterator iter = user_hash_.find(user_guid);
    if (iter != user_hash_.end())
        return iter->second;
    return NULL;
}

void GameShop::Manager::DeleteShopUser(USERGUID user_guid)
{
    UserHash::iterator iter = user_hash_.find(user_guid);
    if (iter != user_hash_.end())
    {
        delete iter->second;
        user_hash_.erase(iter);
    }
}


GameShop::EventProcessor* GameShop::Manager::FindEventProcessor(USERGUID user_guid)
{
    return shop_system_.FindEventProcessor(user_guid);
}


#endif // _NA_0_20110613_GAME_SHOP_RENEWAL