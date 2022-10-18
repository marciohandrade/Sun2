#ifndef _AutobotAreaVillage_H_
#define _AutobotAreaVillage_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "Autobot/StateMachine/AutobotState.h"
#include "Autobot/StateMachine/AutobotStateMachine.h"

#include "Autobot/Area/AutobotArea.h"

struct EXTRA_NPCINFO;
class NPC;
class MapObject;
class Autobot;

//------------------------------------------------------------------------
class AutobotAreaVillage : public AutobotArea
{  
public:
    static AutobotAreaVillage* Instance();
    ~AutobotAreaVillage();

    virtual char* Name(){ return "AutobotAreaVillage"; }
    virtual void Enter(Autobot* owner);
    virtual bool Execute(Autobot* owner);
    virtual void Exit(Autobot* owner);
    virtual bool NetworkProc(Autobot* owner, MSG_BASE* packet);

    AutobotAreaType GetAreaType() { return AutobotArea::AutobotArea_Village; }

    //NPC* shop_npc() { return shop_npc_; }
    //MapObject* waypoint() { return waypoint_; }

    //bool is_shop_finish() const { return shop_finish_; }
    //void set_shop_finish(bool val) { shop_finish_ = val; }
    bool is_return_field() const { return return_field_; }
    void set_return_field(bool val) { return_field_ = val; }

private:
    AutobotAreaVillage();

    //copy ctor and assignment should be private
    AutobotAreaVillage(const AutobotAreaVillage&);
    AutobotAreaVillage& operator=(const AutobotAreaVillage&);

    //NPC* shop_npc_;   //!< 잡화 상점 npc
    ////MapObject* waypoint_;

    //bool shop_finish_;
    bool return_field_;
};


#endif