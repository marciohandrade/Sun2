#pragma once

#include "uiBaseMan.h"

class uiSocketRemove;
class uiSocketRemoveMan :
	public uiBaseMan
{
public:
	uiSocketRemoveMan(InterfaceManager *pUIMan);

public:
    virtual void            OnInitialize();

private:
	uiSocketRemove *		m_pSocketRemove;
};