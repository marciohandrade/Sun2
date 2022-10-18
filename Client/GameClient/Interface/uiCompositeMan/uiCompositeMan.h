#pragma once
#include "uiBaseMan.h"

class ItemETCCompositeDlg ;
class SelectCompositeMenu ;
class uiCompositeMan :
	public uiBaseMan
{
public:
	uiCompositeMan(InterfaceManager *pUIMan);


public:
    virtual void            OnInitialize();


private:
	ItemETCCompositeDlg *	m_pETCComposeDialog;
	SelectCompositeMenu *	m_pSelectCompositeMenu;
};
