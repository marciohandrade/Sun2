#pragma once

#include "./WGPL/WGPL.h"
#include "PlayerAttributes.h"

class PlayerAttributes;

//------------------------------------------------------------------------------
class HeroAttributes : public PlayerAttributes
{
public:
    HeroAttributes();
    virtual ~HeroAttributes();
    virtual void Clear();
    virtual void Update();
    virtual void UpdateEx();
    virtual void SetCharInfo(const BASE_PLAYERINFO* player_info,
        int recover_hp, 
        int recover_mp, 
        int recover_sd);

    //해당 변수는 함수로 감싸지 않는다.
    WGPL::Shuffle<float, true, 2> move_speed_modifier_;

protected:
private:
};