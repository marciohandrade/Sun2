#ifndef _AutobotActionFollow_H_
#define _AutobotActionFollow_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionFollow : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionFollow* Instance();
    ~AutobotActionFollow();

    virtual char* Name(){ return "AutobotActionFollow"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner);

private:
  AutobotActionFollow();
  
  //copy ctor and assignment should be private
  AutobotActionFollow(const AutobotActionFollow&);
  AutobotActionFollow& operator=(const AutobotActionFollow&);

};


#endif