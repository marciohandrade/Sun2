#pragma once

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "uibase.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "FTextRender.h"

class uiDominationTowerApplyBettingDlg: public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_BETTING_NOTICE_TEXT,
        DLGPOS_TOKEN_SLOT_BUTTON,
        DLGPOS_APPLY_HEIM_TEXT,
        DLGPOS_BETTING_COST_EDIT,
		DLGPOS_GUILD_BALANCE_TEXT,
		DLGPOS_BETTING_APPLY_BUTTON,
		DLGPOS_CANCEL_BUTTON,
		DLGPOS_CLOSE_BUTTON,

		DIALOG_MAX,
	};


public:
	uiDominationTowerApplyBettingDlg(InterfaceManager *pUIMan);
	virtual ~uiDominationTowerApplyBettingDlg();

public:
	virtual void Init(CDrawBase* pDrawBase);
	virtual void OnShowWindow(BOOL val);
	virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc( SI_MESSAGE * pMessage );

	void OnApplyButton();

private:
	POSTYPE getCtrlIDToPos( WzID wzId );

private:
	static WzID m_wzId[DIALOG_MAX];
	CCtrlStaticWZ* m_pNoticeText;
    CCtrlButtonWZ* m_pTokenSlotButton;
    CCtrlStaticWZ* m_pApplayHeim;
    CCtrlDxIMEEditBox* m_pBettingEdit;
	CCtrlStaticWZ* m_pHiemBalance;
	CCtrlButtonWZ* m_pBettingButton;
	CCtrlButtonWZ* m_pCancelButton;
	CCtrlButtonWZ* m_pCloseButton;

    FTextRender notice_text_render_;

    TCHAR m_szMoneyOld[INTERFACE_STRING_LENGTH];
    TCHAR m_szMoneyNew[INTERFACE_STRING_LENGTH];
    TCHAR m_szCurrentMoney[INTERFACE_STRING_LENGTH];
};

inline POSTYPE uiDominationTowerApplyBettingDlg::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}
	return DIALOG_MAX;
}
#endif //_NA_006826_20130722_DOMINATION_RENEWAL