#include "SunClientPrecompiledHeader.h"
#include "AutobotAreaDungeon.h"
#include "Autobot/Action/AutobotActionMove.h"
#include "Autobot/Action/AutobotActionReadyField.h"
#include "Autobot/Action/AutobotActionAttack.h"
#include "Autobot/Action/AutobotActionRelax.h"
#include "Autobot/Action/AutobotActionDie.h"
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
AutobotAreaDungeon* AutobotAreaDungeon::Instance()
{
  static AutobotAreaDungeon instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotAreaDungeon::AutobotAreaDungeon()
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
    state_machine()->RegisterState("follow", AutobotActionFollow::Instance());
}
//------------------------------------------------------------------------------
AutobotAreaDungeon::~AutobotAreaDungeon()
{
}
//------------------------------------------------------------------------------
void AutobotAreaDungeon::Enter(Autobot* owner)
{
    AutobotPasspointManager::Instance()->Clear();

    state_machine()->ChangeState("ready");
}
//------------------------------------------------------------------------------
bool AutobotAreaDungeon::Execute(Autobot* owner)
{
    return state_machine()->Update();
}
