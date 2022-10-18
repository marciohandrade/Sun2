#ifndef _AutobotActionTargeting_H_
#define _AutobotActionTargeting_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class Object;
class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionTargeting : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionTargeting* Instance();
    ~AutobotActionTargeting();

    virtual char* Name(){ return "AutobotActionTargeting"; }
    virtual void Enter(AutobotArea* owner){}
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner){}

    bool IsEnemyTarget(OUT DWORD* object_key);
    bool IsFriendTarget(OUT DWORD* object_key);
    bool HasAttackHeroByAvoidMonster();
    Monster* GetNearestMonster();
    void TargetOff();

private:
  AutobotActionTargeting();
  
  //copy ctor and assignment should be private
  AutobotActionTargeting(const AutobotActionTargeting&);
  AutobotActionTargeting& operator=(const AutobotActionTargeting&);

};


#endif