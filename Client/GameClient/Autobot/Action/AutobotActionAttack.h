#ifndef _AutobotActionAttack_H_
#define _AutobotActionAttack_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionAttack : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionAttack* Instance();
    ~AutobotActionAttack();

    virtual char* Name(){ return "AutobotActionAttack"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner);

    bool GetAreaSkillTargetPosition(SLOTCODE skill_code, WzVector& target_pos);

private:
  AutobotActionAttack();
  
  //copy ctor and assignment should be private
  AutobotActionAttack(const AutobotActionAttack&);
  AutobotActionAttack& operator=(const AutobotActionAttack&);

  bool ProcessFollowParty(AutobotArea* owner);
  bool ProcessAreaRange(AutobotArea* owner);
};


#endif