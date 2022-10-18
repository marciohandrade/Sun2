#ifndef _AutobotActionRecall_H_
#define _AutobotActionRecall_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionRecall : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionRecall* Instance();
    ~AutobotActionRecall();

    virtual char* Name(){ return "AutobotActionRecall"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner);
    virtual bool NetworkProc(AutobotArea* owner, MSG_BASE* packet);


private:
  AutobotActionRecall();
  
  //copy ctor and assignment should be private
  AutobotActionRecall(const AutobotActionRecall&);
  AutobotActionRecall& operator=(const AutobotActionRecall&);

  bool UseRecallItem();

};


#endif