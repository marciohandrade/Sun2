#ifndef _AutobotAreaField_H_
#define _AutobotAreaField_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

#include "Autobot/Area/AutobotArea.h"

class Autobot;

//------------------------------------------------------------------------
class AutobotAreaField : public AutobotArea
{  
public:
    static AutobotAreaField* Instance();
    ~AutobotAreaField();

    virtual char* Name(){ return "AutobotAreaField"; }
    virtual void Enter(Autobot* owner);
    virtual bool Execute(Autobot* owner);
    virtual void Exit(Autobot* owner){}
    virtual bool NetworkProc(Autobot* owner, MSG_BASE* packet);

    AutobotAreaType GetAreaType() { return AutobotArea::AutobotArea_Field; }

private:
  AutobotAreaField();
  
  //copy ctor and assignment should be private
  AutobotAreaField(const AutobotAreaField&);
  AutobotAreaField& operator=(const AutobotAreaField&);
 
};


#endif