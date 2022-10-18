#pragma once

#include "uibase.h"

#include "FTextRender.h"
#include "FTextSentence.h"

class ui_XMas_SantaDlg : public uiBase
{
public:
	enum eDIALOG_XMAS_SANTA_DLG
	{
		eDIALOG_XMAS_SANTA_DLG_TEXT_LIST,
		eDIALOG_XMAS_SANTA_DLG_OK_BTN,
		eDIALOG_XMAS_SANTA_DLG_EXIT_BTN,
		eDIALOG_XMAS_SANTA_DLG_MAX,
	};

	ui_XMas_SantaDlg(InterfaceManager * pUIMan );
	~ui_XMas_SantaDlg();
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
	void				AddFormatText();

private:
	FTextRender		m_FtextRender;
	FTextSentence	m_FSentence;
	BOOL m_bTextLoad;

	CCtrlListWZ*	 m_ctrlMainList;

	static WzID		m_wzID[eDIALOG_XMAS_SANTA_DLG_MAX];

	BOOL			m_bShow;
};

inline CControlWZ * ui_XMas_SantaDlg::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_XMAS_SANTA_DLG_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ui_XMas_SantaDlg::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_XMAS_SANTA_DLG_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_XMAS_SANTA_DLG_MAX;
}
