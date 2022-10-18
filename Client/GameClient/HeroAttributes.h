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

    //�ش� ������ �Լ��� ������ �ʴ´�.
    WGPL::Shuffle<float, true, 2> move_speed_modifier_;

protected:
private:
};