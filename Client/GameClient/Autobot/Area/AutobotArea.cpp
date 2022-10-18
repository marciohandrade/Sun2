#include "SunClientPrecompiledHeader.h"
#include "AutobotArea.h"

#include <Autobot/Autobot.h>

//------------------------------------------------------------------------------
AutobotArea::AutobotArea()
{
    state_machine_ = new AutobotStateMachine<AutobotArea>(this);
}
//------------------------------------------------------------------------------
AutobotArea::~AutobotArea()
{
    SAFE_DELETE(state_machine_);
}
