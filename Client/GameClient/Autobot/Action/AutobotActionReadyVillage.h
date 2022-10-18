#ifndef _AutobotActionReadyVillage_H_
#define _AutobotActionReadyVillage_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionReadyVillage : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionReadyVillage* Instance();
    ~AutobotActionReadyVillage();

    virtual char* Name(){ return "AutobotActionReadyVillage"; }
    virtual void Enter(AutobotArea* owner){}
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner){}

private:
  AutobotActionReadyVillage();
  
  //copy ctor and assignment should be private
  AutobotActionReadyVillage(const AutobotActionReadyVillage&);
  AutobotActionReadyVillage& operator=(const AutobotActionReadyVillage&);

};


#endif