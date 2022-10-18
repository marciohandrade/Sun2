#pragma once

#include "uibase.h"

#ifdef _YJ_FISHING_SYSTEM_000933

class uiFishingShop : public uiBase
{
public:
	enum eDIALOG_FISHING_SHOP
	{
		eDIALOG_FISHING_SHOP_EXIT_BTN,
		eDIALOG_FISHING_SHOP_SHOP_BTN,
		eDIALOG_FISHING_SHOP_TRADE_BTN,

		eDIALOG_FISHING_SHOP_MAX,
	};

	uiFishingShop(InterfaceManager * pUIMan );
	~uiFishingShop();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();

private:
	static WzID		m_wzID[eDIALOG_FISHING_SHOP_MAX];

	BOOL			m_bShow;
};

inline CControlWZ * uiFishingShop::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_FISHING_SHOP_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiFishingShop::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_FISHING_SHOP_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_FISHING_SHOP_MAX;
}

#endif //_YJ_FISHING_SYSTEM_000933