#pragma once

#include "uibase.h"

#include "FTextRender.h"
#include "FTextSentence.h"

class ui_XMas_Confirm : public uiBase
{
public:
	enum eDIALOG_XMAS_CONFIRM
	{
		eDIALOG_XMAS_CONFIRM_TITLE_TEXT,
		eDIALOG_XMAS_CONFIRM_TEXT_LIST,
		eDIALOG_XMAS_CONFIRM_OK_BTN,
		eDIALOG_XMAS_CONFIRM_CANCEL_BTN,
		eDIALOG_XMAS_CONFIRM_EXIT_BTN,
		eDIALOG_XMAS_CONFIRM_MAX,
	};

	enum eXMAS_DLG_TYPE
	{
		eXMAS_DLG_TYPE_PERIOD_ITEM,
		eXMAS_DLG_TYPE_DONATION,
		eXMAS_DLG_MAX
	};

	ui_XMas_Confirm(InterfaceManager * pUIMan );
	~ui_XMas_Confirm();
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
	void				MoneyProcess();

public:
	void				SetDlgType(eXMAS_DLG_TYPE eDlgType){m_eDlgType = eDlgType;}
	void				SetDonationMoney(TCHAR* szMoney);

private:
	static WzID		m_wzID[eDIALOG_XMAS_CONFIRM_MAX];

	FTextRender		m_FtextRender;
	FTextSentence	m_FSentence;
	BOOL m_bTextLoad;

	CCtrlStaticWZ*	m_ctrlTitleStatic;
	CCtrlListWZ*    m_ctrlMainList;
	CCtrlButtonWZ*  m_ctrlOKBtn;
	CCtrlButtonWZ*  m_ctrlCancelBtn;

	BOOL			m_bShow;

	eXMAS_DLG_TYPE		m_eDlgType;

	TCHAR m_szDonationMoney[INTERFACE_STRING_LENGTH + 1];
	MONEY			m_CurMoney;
};

inline CControlWZ * ui_XMas_Confirm::getControl(POSTYPE AtPos)
{
	assert(AtPos < eDIALOG_XMAS_CONFIRM_MAX);
	return GetControlWZ(m_wzID[AtPos]);
}

inline POSTYPE ui_XMas_Confirm::getControlIDToPos(WzID wzID)
{
	for ( int i = 0; i < eDIALOG_XMAS_CONFIRM_MAX ; ++i)
	{
		if ( m_wzID[i] == wzID)
			return i;
	}
	return eDIALOG_XMAS_CONFIRM_MAX;
}

inline void ui_XMas_Confirm::SetDonationMoney(TCHAR* szMoney)
{
    if (szMoney == NULL)
    {
        assert(!"ui_XMas_Confirm::SetDonationMoney(TCHAR* szMoney) - String is null");
        return;
    }

	int iStrLen = strlen(szMoney);
	if(0 == iStrLen)
    {
		assert(!"ui_XMas_Confirm::SetDonationMoney(TCHAR* szMoney) - String Error");
    }
	StrCopy(m_szDonationMoney, szMoney);
}