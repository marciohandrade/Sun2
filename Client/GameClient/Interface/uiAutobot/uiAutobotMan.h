#ifndef _uiAutobotMan_h_
#define _uiAutobotMan_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifdef _YMS_AUTOBOT_GAME

#include "uibaseman.h"

class uiAutobotPlay;
class uiAutobotConditionBattle;
class uiAutobotConditionSupport;

//------------------------------------------------------------------------------ 
class uiAutobotMan : public uiBaseMan
{
public:
	uiAutobotMan(InterfaceManager* manager);

    virtual void OnInitialize();

    bool UpdateCondition();

    void ChangeTab(DWORD dialog_id);
    void ShowTab();
    uiBase* active_tab() const { return active_tab_; }
    void set_active_tab(uiBase* val) { active_tab_ = val; }

private:
    uiBase* active_tab_;
};

#endif //_YMS_AUTOBOT_GAME

#endif
