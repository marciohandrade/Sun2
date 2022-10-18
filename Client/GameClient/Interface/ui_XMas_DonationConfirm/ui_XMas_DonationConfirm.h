#pragma once

#include "uibase.h"

#include "FTextRender.h"
#include "FTextSentence.h"

class ui_XMas_DonationConfirm : public uiBase
{
public:
	enum eDIALOG_XMAS_DONATION_CONFIRM
	{
		eDIALOG_XMAS_DONATION_CONFIRM_TEXT_LIST,
		eDIALOG_XMAS_DONATION_CONFIRM_EDIT,
		eDIALOG_XMAS_DONATION_CONFIRM_OK_BTN,
		eDIALOG_XMAS_DONATION_CONFIRM_CANCEL_BTN,
		eDIALOG_XMAS_DONATION_CONFIRM_EXIT_BTN,
		eDIALOG_XMAS_DONATION_CONFIRM_MAX,
	};

	ui_XMas_DonationConfirm(InterfaceManager * pUIMan );
	~ui_XMas_DonationConfirm();
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
	bool				checkDigit(const TCHAR *str);

private:
	FTextRender		m_FtextRender;
	FTextSentence	m_FSentence;
	BOOL m_bTextLoad;

	CCtrlListWZ*	m_ctrlMainList;
	CCtrlDxIMEEditBox*	m_ctrlMoneyEdit;

	static WzID		m_wzID[eDIALOG_XMAS_DONATION_CONFIRM_MAX];

	BOOL			m_bShow;

	TCHAR			m_szMoneyOld[INTERFACE_STRING_LENGTH + 1];
	TCHAR			m_szMoneyNew[INTERFACE_STRING_LENGTH + 1];
};

inline CControlWZ * ui_XMas_DonationConfirm::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_XMAS_DONATION_CONFIRM_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ui_XMas_DonationConfirm::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_XMAS_DONATION_CONFIRM_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_XMAS_DONATION_CONFIRM_MAX;
}
