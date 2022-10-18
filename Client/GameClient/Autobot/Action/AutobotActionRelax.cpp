#include "SunClientPrecompiledHeader.h"
#include "AutobotActionRelax.h"
#include "Autobot/Autobot.h"
#include "Autobot/Condition/AutobotConditionManager.h"
#include "Autobot/Area/Field/AutobotAreaField.h"
#include "Autobot/Action/Global/AutobotActionTargeting.h"
#include "Autobot/Action/Global/AutobotActionInterval.h"

#include "Hero.h"
#include "HeroActionInput.h"
#include "ObjectManager.h"

//------------------------------------------------------------------------------
AutobotActionRelax* AutobotActionRelax::Instance()
{
  static AutobotActionRelax instance;

  return &instance;
}
//------------------------------------------------------------------------------
AutobotActionRelax::AutobotActionRelax()
{
}
//------------------------------------------------------------------------------
AutobotActionRelax::~AutobotActionRelax()
{
}
//------------------------------------------------------------------------------
void AutobotActionRelax::Enter(AutobotArea* owner)
{
    owner->state_machine()->PopGlobalState(AutobotActionTargeting::Instance());
    owner->state_machine()->PopGlobalState(AutobotActionInterval::Instance());

    if (g_pHero->GetCurState() != STATE::SIT)
    {
        g_HeroInput.OnSkill(SLOTCODE(eACTION_START_SKILL_CODE + eACTION_SIT_AND_STAND));
    }
}
//------------------------------------------------------------------------------
void AutobotActionRelax::Exit(AutobotArea* owner)
{
    //if (g_pHero->GetCurState() == STATE::SIT)
    //{
    //    g_HeroInput.OnSkill(SLOTCODE(eACTION_START_SKILL_CODE + eACTION_SIT_AND_STAND));
    //}
    owner->state_machine()->PushGlobalState(AutobotActionInterval::Instance());
    owner->state_machine()->PushGlobalState(AutobotActionTargeting::Instance());
}
//------------------------------------------------------------------------------
bool AutobotActionRelax::Execute(AutobotArea* owner)
{
    if (g_pHero->IsDead() == TRUE)
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }

    //------------------------------------------------------------------------------
    if (AutobotConditionManager::Instance()->hunting().hunting_type() == AutobotCondition_Hunting::kHuntingType_Party)
    {
        Object* object = g_ObjectManager.GetObject(AutobotConditionManager::Instance()->hunting().GetFollowPlayer());
        if (object != NULL)
        {
            WzVector gab = g_pHero->GetPosition() - object->GetPosition();
            if(VectorLength(&gab) > DISTANCE_FOLLOW_PLAYER)
            {
                owner->state_machine()->ChangeState("follow");
                return false;
            }
        }
    }

    //------------------------------------------------------------------------------
    if ((AutobotConditionManager::Instance()->recovery().IsRelax() == false) ||
        (AutobotActionTargeting::Instance()->IsEnemyTarget(NULL) == true) || 
        (g_pHero->GetCurState() != STATE::SIT))
    {
        owner->state_machine()->ChangeState("ready");
        return false;
    }
    
    return true;
}
