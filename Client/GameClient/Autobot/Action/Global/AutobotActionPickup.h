#ifndef _AutobotActionPickup_H_
#define _AutobotActionPickup_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionPickup : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionPickup* Instance();
    ~AutobotActionPickup();

    virtual char* Name(){ return "AutobotActionPickup"; }
    virtual void Enter(AutobotArea* owner){}
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner){}

private:
  AutobotActionPickup();
  
  //copy ctor and assignment should be private
  AutobotActionPickup(const AutobotActionPickup&);
  AutobotActionPickup& operator=(const AutobotActionPickup&);

};


#endif