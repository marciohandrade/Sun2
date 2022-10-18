#ifndef _AutobotActionDie_H_
#define _AutobotActionDie_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionDie : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionDie* Instance();
    ~AutobotActionDie();

    virtual char* Name(){ return "AutobotActionDie"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner);
    virtual bool NetworkProc(AutobotArea* owner, MSG_BASE* packet);

private:
  AutobotActionDie();
  
  //copy ctor and assignment should be private
  AutobotActionDie(const AutobotActionDie&);
  AutobotActionDie& operator=(const AutobotActionDie&);

};


#endif