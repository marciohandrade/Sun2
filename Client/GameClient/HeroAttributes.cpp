#include "SunClientPrecompiledHeader.h"

#include <PlayerAttributes.h>
#include "HeroAttributes.h"


HeroAttributes::HeroAttributes()
{

}
//------------------------------------------------------------------------------
HeroAttributes::~HeroAttributes()
{

}
//------------------------------------------------------------------------------
void HeroAttributes::Clear()
{
    PlayerAttributes::Clear();
    move_speed_modifier_ = 0.0f;
}
//------------------------------------------------------------------------------
void HeroAttributes::Update()
{
    PlayerAttributes::Update();
    move_speed_modifier_ = GetMoveSpeedRatio() * 0.01f;
}
//------------------------------------------------------------------------------
void HeroAttributes::UpdateEx()
{
    PlayerAttributes::UpdateEx();
    move_speed_modifier_ = GetMoveSpeedRatio() * 0.01f;
}
//------------------------------------------------------------------------------
void HeroAttributes::SetCharInfo(const BASE_PLAYERINFO* player_info, 
                                 int recover_hp,
                                 int recover_mp,
                                 int recover_sd)
{
    PlayerAttributes::SetCharInfo(player_info, recover_hp, recover_mp, recover_sd);

    move_speed_modifier_ = GetMoveSpeedRatio() * 0.01f;
}