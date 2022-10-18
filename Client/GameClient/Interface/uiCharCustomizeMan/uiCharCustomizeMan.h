
#pragma once
#include "uiBaseMan.h"

class InterfaceManager;

struct IM_CHAR_CUSTOMIZE_MGR
{
	static const DWORD CHAR_CUSTOMIZE_DLG;			
};

class uiCharCustomizeMan : public uiBaseMan
{
public:
	uiCharCustomizeMan(InterfaceManager * pUIMAN);

public:
	virtual void            OnInitialize();
	
};
