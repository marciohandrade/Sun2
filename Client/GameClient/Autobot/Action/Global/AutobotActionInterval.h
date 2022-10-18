#ifndef _AutobotActionInterval_H_
#define _AutobotActionInterval_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class Object;
class InventoryDialog;

class AutobotArea;

//------------------------------------------------------------------------
class AutobotActionInterval : public AutobotState<AutobotArea>
{  
public:
    static AutobotActionInterval* Instance();
    ~AutobotActionInterval();

    virtual char* Name(){ return "AutobotActionInterval"; }
    virtual void Enter(AutobotArea* owner){}
    virtual bool Execute(AutobotArea* owner);
    virtual void Exit(AutobotArea* owner){}

private:
  AutobotActionInterval();
  
  //copy ctor and assignment should be private
  AutobotActionInterval(const AutobotActionInterval&);
  AutobotActionInterval& operator=(const AutobotActionInterval&);

  bool ProcessSkill(AutobotArea* owner, SLOTCODE skill_code, DWORD object_key);
  DWORD GetDeadMemberKey();

};


#endif