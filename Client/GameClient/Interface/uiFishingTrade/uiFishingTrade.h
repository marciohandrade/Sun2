#pragma once

#include "uibase.h"

#ifdef _YJ_FISHING_SYSTEM_000933

class uiFishingTrade : public uiBase
{
public:
	enum eDIALOG_FISHING_TRADE
	{
		eDIALOG_FISHING_TRADE_EXIT_BTN,
		eDIALOG_FISHING_TRADE_TRADE_BTN,
		eDIALOG_FISHING_TRADE_CANCEL_BTN,

		eDIALOG_FISHING_TRADE_SLOT_BTN,

		eDIALOG_FISHING_TRADE_MAX,
	};

	uiFishingTrade(InterfaceManager * pUIMan );
	~uiFishingTrade();
	virtual void		Init(CDrawBase * pDrawBase);
	virtual void		Release();
	virtual void		OnUpdateSolarDialog();
	virtual void		OnRenderSolarDialog();
	virtual void		Process(DWORD dwTick);	
	virtual void		OnShowWindow(BOOL val);
	virtual void		MessageProc(SI_MESSAGE * pMessage);

protected:
	CControlWZ *		getControl(POSTYPE AtPos);
	POSTYPE				getControlIDToPos(WzID wzID);
	void				InitControl();

	void				UpdateToolTip();
	void				RenderTooltip();

private:
	static WzID		m_wzID[eDIALOG_FISHING_TRADE_MAX];
	BOOL			m_bShow;

	CCtrlButtonWZ*  m_pctrlSlotBtn;
	BOOL			m_bOnSlot;
	HANDLE			m_hSlotTexture;
	BOOL			m_bRenderToolTip;
	CODETYPE		m_dwItemCode;
};

inline CControlWZ * uiFishingTrade::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_FISHING_TRADE_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiFishingTrade::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_FISHING_TRADE_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_FISHING_TRADE_MAX;
}

#endif //_YJ_FISHING_SYSTEM_000933