#pragma once
#include "uiBaseMan.h"

class EnduranceDIalog;
class uiEnduranceMan :
	public uiBaseMan
{
public:
	uiEnduranceMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();


private:
	EnduranceDIalog*		m_pEnduranceDialog;
};
