#pragma once

#include "uiBaseMan.h"

struct IM_FATEITEM_SYSTEM
{
	static const DWORD FATEITEM_DLG;
};


class uiFateItemSystem;
class uiFateItemSystemMan :
	public uiBaseMan
{
public:
	uiFateItemSystemMan(InterfaceManager *pUIMan);

public:
	virtual	void			OnInitialize();

private:
	uiFateItemSystem *		m_pUIFateItemSystem;
};

