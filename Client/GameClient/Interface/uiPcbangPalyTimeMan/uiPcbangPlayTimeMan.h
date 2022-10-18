
#pragma once
#include "uiBaseMan.h"

class PcbangPlayTimeDialog;
class uiPcbangPlayTimeMan :
	public uiBaseMan
{
public:
	uiPcbangPlayTimeMan(InterfaceManager *pUIMan);

public:
    virtual void            OnInitialize();

private:
	PcbangPlayTimeDialog *		m_pPcbangPlayTimeDialog;
};
