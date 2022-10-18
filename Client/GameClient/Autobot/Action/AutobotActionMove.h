#ifndef _AutobotActionMove_H_
#define _AutobotActionMove_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class AutobotPasspoint;
class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionMove : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionMove* Instance();
    ~AutobotActionMove();

    virtual char* Name(){ return "AutobotActionMove"; }
    virtual void Enter(AutobotArea* owner);
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner);

private:
  AutobotActionMove();
  
  //copy ctor and assignment should be private
  AutobotActionMove(const AutobotActionMove&);
  AutobotActionMove& operator=(const AutobotActionMove&);

  bool ProcessField(AutobotArea* owner);
  void ActionCheckPoint(AutobotPasspoint* check_point);

};


#endif