#ifndef _AutobotAreaDungeon_H_
#define _AutobotAreaDungeon_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

#include "Autobot/Area/AutobotArea.h"

class Autobot;

//------------------------------------------------------------------------
class AutobotAreaDungeon : public AutobotArea
{  
public:
    static AutobotAreaDungeon* Instance();
    ~AutobotAreaDungeon();

    virtual char* Name(){ return "AutobotAreaDungeon"; }
    virtual void Enter(Autobot* owner);
    virtual bool Execute(Autobot* owner);
    virtual void Exit(Autobot* owner){}

    AutobotAreaType GetAreaType() { return AutobotArea::AutobotArea_Hunt; }

private:
  AutobotAreaDungeon();
  
  //copy ctor and assignment should be private
  AutobotAreaDungeon(const AutobotAreaDungeon&);
  AutobotAreaDungeon& operator=(const AutobotAreaDungeon&);
 
};


#endif