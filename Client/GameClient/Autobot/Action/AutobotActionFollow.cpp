#include "SunClientPrecompiledHeader.h"
#include "AutobotActionFollow.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Field/AutobotAreaField.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"
#include "Autobot/Action/Global/AutobotActionInterval.h"

#include "Hero.h"
#include "HeroActionInput.h"
#include "ObjectManager.h"

//------------------------------------------------------------------------------
AutobotActionFollow* AutobotActionFollow::Instance()
{
  static AutobotActionFollow instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionFollow::AutobotActionFollow()
{
}
//------------------------------------------------------------------------------
AutobotActionFollow::~AutobotActionFollow()
{
}
//------------------------------------------------------------------------------
void AutobotActionFollow::Enter(AutobotArea* owner)
{
    owner->state_machine()->PopGlobalState(AutobotActionInterval::Instance());
    owner->state_machine()->PopGlobalState(AutobotActionTargeting::Instance());
}
//------------------------------------------------------------------------------
void AutobotActionFollow::Exit(AutobotArea* owner)
{
    if (g_HeroInput.GetFollowState() == TRUE)
    {
        g_HeroInput.SetFollowState(FALSE);
    }
    owner->state_machine()->PushGlobalState(AutobotActionInterval::Instance());
    owner->state_machine()->PushGlobalState(AutobotActionTargeting::Instance());
}
//------------------------------------------------------------------------------
bool AutobotActionFollow::Execute(AutobotArea* owner)
{
    if (g_pHero->IsDead() == TRUE)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    Object* object = g_ObjectManager.GetObject(AutobotConditionManager::Instance()->hunting().GetFollowPlayer());
    if (object == NULL)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    WzVector gab = g_pHero->GetPosition() - object->GetPosition();
    if(VectorLength(&gab) < DISTANCE_FOLLOW_PLAYER)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    if (g_HeroInput.GetFollowState() == FALSE)
    {
        g_HeroInput.SetFollowState(TRUE, object->GetObjectKey());
    }
    
    return true;
}
