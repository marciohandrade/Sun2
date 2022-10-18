#ifndef _AutobotArea_H_
#define _AutobotArea_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

class NPC;
class Autobot;

//------------------------------------------------------------------------------
//! AutobotArea
//------------------------------------------------------------------------------
class AutobotArea : public AutobotState<Autobot>
{  
public:
    enum AutobotAreaType
    {
        AutobotArea_Village = 0,
        AutobotArea_Field,
        AutobotArea_Hunt,
        AutobotArea_Max
    };

public:
    ~AutobotArea();

    virtual void Enter(Autobot* owner){}
    virtual bool Execute(Autobot* owner) { return true; }
    virtual void Exit(Autobot* owner){}
    virtual bool NetworkProc(Autobot* owner, MSG_BASE* packet) { return true; }

    virtual AutobotAreaType GetAreaType() = 0;

    AutobotStateMachine<AutobotArea>* state_machine() const { return state_machine_; }

protected:
    AutobotArea();

private:
  AutobotStateMachine<AutobotArea>* state_machine_;
};


#endif