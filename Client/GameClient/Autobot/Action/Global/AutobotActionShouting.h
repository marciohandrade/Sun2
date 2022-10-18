#ifndef _AutobotActionShouting_H_
#define _AutobotActionShouting_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class InventoryDialog;

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionShouting : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionShouting* Instance();
    ~AutobotActionShouting();

    virtual char* Name(){ return "AutobotActionShouting"; }
    virtual void Enter(AutobotArea* owner){}
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner){}

private:
  AutobotActionShouting();
  
  //copy ctor and assignment should be private
  AutobotActionShouting(const AutobotActionShouting&);
  AutobotActionShouting& operator=(const AutobotActionShouting&);
};


#endif