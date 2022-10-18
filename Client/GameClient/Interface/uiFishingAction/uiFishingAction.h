#pragma once

#include "uibase.h"

#ifdef _YJ_FISHING_SYSTEM_000933

class uiFishingAction : public uiBase
{
public:
	enum eDIALOG_FISHING_ACTION
	{
		eDIALOG_FISHING_ACTION_EXIT_BTN,
		eDIALOG_FISHING_ACTION_GAUGE_RECT_PIC,
		eDIALOG_FISHING_ACTION_RESULT_TEXT,
		eDIALOG_FISHING_ACTION_MAX,
	};

	uiFishingAction(InterfaceManager * pUIMan );
	~uiFishingAction();
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

private:
	static WzID		m_wzID[eDIALOG_FISHING_ACTION_MAX];
	BOOL			m_bShow;

	CCtrlStaticWZ*  m_pctrlResultText;
	CCtrlPictureWZ* m_pctrlGaugeRectPic;

	HANDLE			m_hGaugeTexture;
	float			m_fGaugeSize;
	DWORD			m_dwGaugeSpeed; //서버에서 받자.
	DWORD			m_dwTempGauge;
	DWORD			m_DwRealGauge;
	DWORD			m_dwOldTime;

public:
	void			SetTimer(DWORD dwTime);
};

inline CControlWZ * uiFishingAction::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_FISHING_ACTION_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiFishingAction::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_FISHING_ACTION_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_FISHING_ACTION_MAX;
}

#endif //_YJ_FISHING_SYSTEM_000933