#ifndef _AutobotActionShop_H_
#define _AutobotActionShop_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionShop : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionShop* Instance();
    ~AutobotActionShop();

    virtual char* Name(){ return "AutobotActionShop"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner);

private:
  AutobotActionShop();
  
  //copy ctor and assignment should be private
  AutobotActionShop(const AutobotActionShop&);
  AutobotActionShop& operator=(const AutobotActionShop&);
};


#endif