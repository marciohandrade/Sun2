#pragma once

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "uibase.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"


class uiDominationTowerApplyBettingDlg2: public uiBase
{
	enum eDIALOG_POS
	{
		DLGPOS_BETTING_NOTICE_TEXT,
        DLGPOS_TOKEN_SLOT_BUTTON,
        DLGPOS_BETTING_COST_EDIT,
		DLGPOS_BETTING_APPLY_BUTTON,
		DLGPOS_CANCEL_BUTTON,
		DLGPOS_CLOSE_BUTTON,

		DIALOG_MAX,
	};

public:
	uiDominationTowerApplyBettingDlg2(InterfaceManager *pUIMan);
	virtual ~uiDominationTowerApplyBettingDlg2();

public:
	virtual void Init(CDrawBase* pDrawBase);
	virtual void OnShowWindow(BOOL val);
	virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    CControlWZ* GetTokenSlotControl()   { return token_slot_button_; }

	virtual void MessageProc(SI_MESSAGE* pMessage);

    void OnApplyButton();

private:
	POSTYPE getCtrlIDToPos(WzID control_id);

private:
	static WzID controls_id_[DIALOG_MAX];
	CCtrlStaticWZ* notice_text_;
    CCtrlButtonWZ* token_slot_button_;
	CCtrlButtonWZ* betting_button_;
	CCtrlButtonWZ* cancel_button_;
	CCtrlButtonWZ* close_button_;
    CCtrlDxIMEEditBox* betting_edit_;

    TCHAR money_old[INTERFACE_STRING_LENGTH];
    TCHAR money_new[INTERFACE_STRING_LENGTH];
    TCHAR current_money[INTERFACE_STRING_LENGTH];
};

inline POSTYPE uiDominationTowerApplyBettingDlg2::getCtrlIDToPos(WzID control_id)
{
    for( POSTYPE index = 0; index < DIALOG_MAX; ++index )
    {
        if(controls_id_[index] == control_id)
        {
            return index;
        }
    }
    return DIALOG_MAX;
}

#endif //_NA_006826_20130722_DOMINATION_RENEWAL