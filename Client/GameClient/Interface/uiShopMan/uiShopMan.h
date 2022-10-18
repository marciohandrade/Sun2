#pragma once
#include "uibaseman.h"

class ShopDialog;
class uiShopMan :
	public uiBaseMan
{
public:
	uiShopMan(InterfaceManager *pUIMan);

public:
    virtual void            OnInitialize();

private:
	ShopDialog *			m_pShopDialog;
};
