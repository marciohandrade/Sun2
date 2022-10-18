#pragma once
#ifndef GAMESERVER_ETHEREALIZE_IMPL_H
#define GAMESERVER_ETHEREALIZE_IMPL_H

namespace nsSlot {
;

struct EtheriaControlOption;

//==================================================================================================

// implemented by _NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_
class EtheriaImpl
{
public:
    enum eResult {
        eResult_Fail    = 0,// fail     Server -> Client(requester)
        eResult_1to1    ,   // success  Server -> Client(requester)
        eResult_1toN    ,   // related to casting  Server -> Client(broadcast)
    };
    EtheriaImpl() { ZeroMemory(this, sizeof(*this)); }
    eResult Casting(const EtheriaControl& req_info,
                    EtheriaControl* const result, EtheriaControlData* const broad_result_data);
    eResult ExtractToEtheria(const EtheriaControl& req_info,
                             EtheriaControl* const result, EtheriaControlData* const result_data);
    eResult CombineWithEtheria(const EtheriaControl& req_info,
                               EtheriaControl* const result, EtheriaControlData* const result_data);
private:
    // call by a Player
    void SetInstance(Player* player, EtheriaControlOption* control_option);
    //
    Player* player_;
    EtheriaControlOption* control_option_;
    friend class Player;
    __DISABLE_COPY(EtheriaImpl);
};

//==================================================================================================

struct EtheriaFailHandler
{
    EtheriaFailHandler(EtheriaControl* result, EtheriaControlData* result_data,
                       const EtheriaControl& req_info,
                       Player* player)
        : player_(player), req_info_(req_info), result_(result), result_data_(result_data)
    {}
    //
    void SetFailResult(uint8_t result_type, uint16_t result_code, bool show_log);
    void OutputLog();
    //
    Player* const player_;
    const EtheriaControl& req_info_;
    EtheriaControl* const result_;
    EtheriaControlData* const result_data_;
    __DISABLE_COPY(EtheriaFailHandler);
};

}; //end of namespace

#endif //GAMESERVER_ETHEREALIZE_IMPL_H