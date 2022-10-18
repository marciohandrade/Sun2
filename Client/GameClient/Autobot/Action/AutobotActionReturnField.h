#ifndef _AutobotActionReturnField_H_
#define _AutobotActionReturnField_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionReturnField : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionReturnField* Instance();
    ~AutobotActionReturnField();

    virtual char* Name(){ return "AutobotActionReturnField"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner){ return true; }
    virtual void Exit(AutobotArea* owner){}
    virtual bool NetworkProc(AutobotArea* owner, MSG_BASE* packet);


private:
  AutobotActionReturnField();
  
  //copy ctor and assignment should be private
  AutobotActionReturnField(const AutobotActionReturnField&);
  AutobotActionReturnField& operator=(const AutobotActionReturnField&);

};


#endif