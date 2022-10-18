#include "SunClientPrecompiledHeader.h"

#include "AutobotPasspoint.h"

#include "Hero.h"
#include "HeroActionInput.h"
#include "ObjectManager.h"

//------------------------------------------------------------------------------
AutobotPasspoint::AutobotPasspoint()
{
    passpoint_type_ = kPasspoint_Max; 
    objectkey_ = 0;
    ZeroMemory(&position_, sizeof(position_)); 
    action_id_ = ACTION_INVALID;
}
//------------------------------------------------------------------------------
AutobotPasspoint::AutobotPasspoint(PasspointType passpoint_type, DWORD objectkey, 
                                   WzVector& position, eActionID action_id)
{
    passpoint_type_ = passpoint_type;
    objectkey_ = objectkey;
    position_ = position; 
    action_id_ = action_id;
}
//------------------------------------------------------------------------------
bool AutobotPasspoint::IsArrival(WzVector& pos)
{
    PLAYER_ACTION action;
    action.ActionID = action_id();
    float check_length = max(g_HeroInput.GetActionLength(action), 0.1f);

    WzVector gab = position() - pos;

    if (VectorLength(&gab) <= check_length)
    {
        return true;
    }

    return false;
}