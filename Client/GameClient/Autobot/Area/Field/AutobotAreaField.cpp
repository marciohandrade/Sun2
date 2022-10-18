#include "SunClientPrecompiledHeader.h"
#include "AutobotAreaField.h"
#include "Autobot/Action/AutobotActionMove.h"
#include "Autobot/Action/AutobotActionReadyField.h"
#include "Autobot/Action/AutobotActionAttack.h"
#include "Autobot/Action/AutobotActionRelax.h"
#include "Autobot/Action/AutobotActionDie.h"
#include "Autobot/Action/AutobotActionRecall.h"
#include "Autobot/Action/AutobotActionFollow.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"
#include "Autobot/Action/Global/AutobotActionPickup.h"
#include "Autobot/Action/Global/AutobotActionPotion.h"
#include "Autobot/Action/Global/AutobotActionInterval.h"
#include "Autobot/Action/Global/AutobotActionShouting.h"

#include "Autobot/Autobot.h"
#include "Autobot/Passpoint/AutobotPasspointManager.h"
#include "Autobot/Condition/AutobotConditionManager.h"

//------------------------------------------------------------------------------
AutobotAreaField* AutobotAreaField::Instance()
{
  static AutobotAreaField instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotAreaField::AutobotAreaField()
{
    state_machine()->PushGlobalState(AutobotActionPickup::Instance());
    state_machine()->PushGlobalState(AutobotActionPotion::Instance());
    state_machine()->PushGlobalState(AutobotActionInterval::Instance());
    state_machine()->PushGlobalState(AutobotActionTargeting::Instance());
    state_machine()->PushGlobalState(AutobotActionShouting::Instance());

    state_machine()->RegisterState("ready", AutobotActionReadyField::Instance());
    state_machine()->RegisterState("move", AutobotActionMove::Instance());
    state_machine()->RegisterState("attack", AutobotActionAttack::Instance());
    state_machine()->RegisterState("relax", AutobotActionRelax::Instance());
    state_machine()->RegisterState("die", AutobotActionDie::Instance());
    state_machine()->RegisterState("recall", AutobotActionRecall::Instance());
    state_machine()->RegisterState("follow", AutobotActionFollow::Instance());
}
//------------------------------------------------------------------------------
AutobotAreaField::~AutobotAreaField()
{
}
//------------------------------------------------------------------------------
void AutobotAreaField::Enter(Autobot* owner)
{
    AutobotPasspointManager::Instance()->Clear();

    state_machine()->ChangeState("ready");
}
//------------------------------------------------------------------------------
bool AutobotAreaField::Execute(Autobot* owner)
{
    return state_machine()->Update();
}
//------------------------------------------------------------------------------ 
bool AutobotAreaField::NetworkProc(Autobot* owner, MSG_BASE* packet)
{
    return state_machine()->NetworkProc(packet);
}