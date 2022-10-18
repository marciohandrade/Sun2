#pragma once
#ifndef GAMESERVER_PLAYER_WINGS_H
#define GAMESERVER_PLAYER_WINGS_H

#ifdef _NA001955_110210_WING_COSTUME

class IPlayerWingsD;

//==================================================================================================
namespace ns_object {
;

class PlayerWings
{
public:
    enum eResult {
        eResult_Fail    = 0,// fail     Server -> Client(requester)
        eResult_1to1    ,   // success  Server -> Client(requester)
        eResult_1toN    ,   // success  Server -> Client(broadcast)
    };
    PlayerWings();
    PlayerWings(const IPlayerWingsD& rhs, Player* player);
    ~PlayerWings();

    static bool SpreadWings(Player* player,
        const WingControl& req_info,
        WingControl* const result, WingControlData* const broad_result_data,
        eResult* const notify_result);
    static bool FurlWings(Player* player,
        const WingControl& req_info,
        WingControl* const result, WingControlData* const broad_result_data,
        eResult* const notify_result);
private:
    bool Spread(const WingControl& req_info,
        WingControl* const result, WingControlData* const broad_result_data,
        eResult* const notify_result);
    bool Furl(const WingControl& req_info,
        WingControl* const result, WingControlData* const broad_result_data,
        eResult* const notify_result);
    //
    bool SpreadDefault(const WingControl& req_info,
        WingControl* const result, WingControlData* const broad_result_data,
        eResult* const notify_result, SCItemSlot** flying_item);
    bool SpreadOnRiding(const WingControl& req_info,
        WingControl* const result, WingControlData* const broad_result_data,
        eResult* const notify_result, SCItemSlot** flying_item);
    //
    const IPlayerWingsD& base_;
    struct Data {
        Player* player;
        bool spread_wings;
        SLOTIDX slot_index; // SI_EQUIPMENT(wings_item), SI_INVENTORY(riding_item)
        POSTYPE item_pos; // wings item position in equipment slot container
        SCItemSlot* wings_item; // summoned wings item
    } data_;
    //
    friend class IPlayerWingsD;
    __DISABLE_COPY(PlayerWings);
}; //end of class

}; //end of namespace

//==================================================================================================
#endif //_NA001955_110210_WING_COSTUME

#endif //GAMESERVER_PLAYER_WINGS_H