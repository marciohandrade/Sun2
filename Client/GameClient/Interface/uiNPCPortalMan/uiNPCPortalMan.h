#pragma once
#include "uiBaseMan.h"

class NPCPortalDialog;
class uiNPCPortalMan :
	public uiBaseMan
{
public:
	uiNPCPortalMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();


private:
	NPCPortalDialog *		m_pNPCPortalDialog;
};
