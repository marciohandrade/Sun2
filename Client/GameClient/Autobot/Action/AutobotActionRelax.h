#ifndef _AutobotActionRelax_H_
#define _AutobotActionRelax_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionRelax : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionRelax* Instance();
    ~AutobotActionRelax();

    virtual char* Name(){ return "AutobotActionRelax"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner);

private:
  AutobotActionRelax();
  
  //copy ctor and assignment should be private
  AutobotActionRelax(const AutobotActionRelax&);
  AutobotActionRelax& operator=(const AutobotActionRelax&);

};


#endif