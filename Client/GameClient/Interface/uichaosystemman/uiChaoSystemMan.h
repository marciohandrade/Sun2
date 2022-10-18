#pragma once
#include "uiBaseMan.h"

class ChaoSystemDialog;
class uiChaoSystemMan :
	public uiBaseMan
{
public:
	uiChaoSystemMan(InterfaceManager *pUIMan);

public:
    virtual void            OnInitialize();
	virtual void            Process(DWORD dwTick);

	void                    SetTime(DWORD dwTime);
	DWORD                   GetTime();

private:
	ChaoSystemDialog *		m_pChaoSystemDialog;
};
