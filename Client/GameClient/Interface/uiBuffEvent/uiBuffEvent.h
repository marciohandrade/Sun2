#pragma once

#include "uibase.h"

class uiBuffEvent : public uiBase
{
public:
	enum eDIALOG_BUFF_EVENT
	{
		eDIALOG_BUFF_EVENT_OK_BTN,
		eDIALOG_BUFF_EVENT_EXIT_BTN,
#ifdef _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
        eDIALOG_BUFF_EVENT_STATIC_LEVELRANGE,
        eDIALOG_BUFF_EVENT_STATIC_LEVELRANGE_PCBANG,
#endif //_NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
		eDIALOG_BUFF_EVENT_MAX,
	};

	uiBuffEvent(InterfaceManager * pUIMan );
	~uiBuffEvent();
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
#ifdef _NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL
	void				SetTextControl();
#endif //_NA_008395_20150710_EVENT_CRYSTAL_UI_TEXT_CONTROL



private:
	static WzID		m_wzID[eDIALOG_BUFF_EVENT_MAX];

	CCtrlButtonWZ* m_pBtnOk;

	BOOL			m_bShow;
};

inline CControlWZ * uiBuffEvent::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_BUFF_EVENT_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE uiBuffEvent::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_BUFF_EVENT_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_BUFF_EVENT_MAX;
}
