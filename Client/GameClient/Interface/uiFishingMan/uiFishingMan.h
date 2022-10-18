#include "uiBaseMan.h"

#ifdef _YJ_FISHING_SYSTEM_000933
#pragma once
class uiFishingShop;
class uiFishingTrade;
class uiFishingAction;

class uiFishingMan : public uiBaseMan
{
public:
	uiFishingMan( InterfaceManager * pUIMAN);

public:
	virtual void            OnInitialize();

private:
    uiFishingShop*			m_pFishingShop;
	uiFishingTrade*			m_pFishingTrade;
	uiFishingAction*		m_pFishingAction;

};

#endif // _YJ_FISHING_SYSTEM_000933