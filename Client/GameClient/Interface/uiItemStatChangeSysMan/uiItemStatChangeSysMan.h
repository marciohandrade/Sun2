#pragma once

#include "uiBaseMan.h"


struct IM_ITEM_STAT_CHANGE_MANAGER
{
	static const DWORD  ITEM_STAT_CHANGE;
};

class uiItemStatChangeSys;
class uiItemStatChangeSysMan :
	public uiBaseMan
{
public:
	uiItemStatChangeSysMan(InterfaceManager *pUIMan);

	//--------------------------------------------------------------------------
	// init/release
	//
public:
	virtual void            OnInitialize();

private:
	uiItemStatChangeSys*	m_pItemStatChangeSysDlg;
};
