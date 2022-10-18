#pragma once
#include "uiBaseMan.h"

class uiQuickSkillMan :
	public uiBaseMan
{
public:
	uiQuickSkillMan(InterfaceManager *pUIMan);
	virtual ~uiQuickSkillMan(void);

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            OnInitialize();

};
