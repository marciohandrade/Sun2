#ifndef _AutobotActionReadyField_H_
#define _AutobotActionReadyField_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionReadyField : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionReadyField* Instance();
    ~AutobotActionReadyField();

    virtual char* Name(){ return "AutobotActionReadyField"; }
    virtual void Enter(AutobotArea* owner){}
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner){}

private:
  AutobotActionReadyField();
  
  //copy ctor and assignment should be private
  AutobotActionReadyField(const AutobotActionReadyField&);
  AutobotActionReadyField& operator=(const AutobotActionReadyField&);

  bool ProcessHuntingFree(AutobotArea* owner);
  bool ProcessFollowParty(AutobotArea* owner);

};


#endif