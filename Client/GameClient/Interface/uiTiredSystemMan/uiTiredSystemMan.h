#pragma once
#include "uiBaseMan.h"

struct IM_TIRED_SYSTEM_MANAGER
{
	static const DWORD TIRED_SYSTEM;
};

class TiredSystemDialog;
class uiTiredSystemMan :
	public uiBaseMan
{
public:
	uiTiredSystemMan(InterfaceManager *pUIMan);


public:
	virtual void            OnInitialize();


private:
	TiredSystemDialog *		m_pTiredSystemDialog;
};
