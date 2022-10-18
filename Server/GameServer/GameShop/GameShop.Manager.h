#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef GAMESHOP_MANAGER_H_
#define GAMESHOP_MANAGER_H_


#if _MSC_VER > 1000
#pragma once
#endif


#include "GameShop.System.h"

namespace GameShop
{

class User;
class EventProcessor;

class Manager : public util::Singleton<GameShop::Manager>
{
public:
    Manager(void);
    ~Manager(void);

    void Initialize(const char* ip_address, WORD port);
    void StartService(); // NOTE: f111005.4L
    void Update(uint delta_tick);
    void OpenShop(Player* player, ShopScriptVersion& script_version);
    void CloseShop(Player* player);

    void Request_ScriptVersion(Player* player);
    void Request_ExistGiftNotify(Player* player);


    GameShop::EventProcessor* FindEventProcessor(USERGUID user_guid);
    void NotifyCloseShopToAllUser(RC::eSHOP_RESULT result = RC::RC_SHOP_CHANGED_SCRIPT_VERSION);

    GameShop::System& get_shop_system();

private:
    GameShop::User* CreateShopUser(USERGUID user_guid);
    GameShop::User* FindShopUser(USERGUID user_guid);
    void DeleteShopUser(USERGUID user_guid);
    void UpdateConnect(uint delta_tick);

private:
    typedef STLX_HASH_MAP<USERGUID, GameShop::User*> UserHash;

    util::ITimerBase update_timer_;
    //
    UserHash user_hash_;
    GameShop::System shop_system_;
    //
    struct {
        char ip_address[30];
        WORD port;
    } connection_info_;
    //
    __DISABLE_COPY(GameShop::Manager);
};


//----------------------------------------------------------------------
inline GameShop::System& Manager::get_shop_system()
{
    return shop_system_;
}


}; //namespace GameShop


#endif //GAMESHOP_MANAGER_H_

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
