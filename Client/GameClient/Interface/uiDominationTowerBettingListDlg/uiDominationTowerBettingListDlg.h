#pragma once

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "uibase.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"

class uiDominationTowerBettingListDlg: public uiBase
{
    enum eDIALOG_POS
    {		
		DLGPOS_BETTING_LIST,
		DLGPOS_BETTING_BUTTON,
		DLGPOS_CANCEL_BUTTON,

		DLGPOS_CLOSE_BUTTON,

		DLGPOS_GUILDNAME_TEXT,
		DLGPOS_DATE_TEXT,
		DLGPOS_HEIM_TEXT,

        DIALOG_MAX,
    };
	enum
	{
		LIST_COL = 3,
	};

public:
    uiDominationTowerBettingListDlg(InterfaceManager *pUIMan);
    virtual ~uiDominationTowerBettingListDlg();

public:
	virtual void Init(CDrawBase* pDrawBase);
	virtual void OnShowWindow(BOOL val);
	virtual void MessageProc( SI_MESSAGE * pMessage );

	void SetBettingListData(DOMINATION_ACTION_INFO* pListInfo, DWORD dwCount);
    void SetBettingButton(BOOL bCheck);

private:
	POSTYPE getCtrlIDToPos( WzID wzId );

private:
	static WzID m_wzId[DIALOG_MAX];
	CCtrlListWZ* m_pBettingList;
	CCtrlButtonWZ* m_pBettingButton;
	CCtrlButtonWZ* m_pCancelButton;
	CCtrlButtonWZ* m_pCloseButton;
	CCtrlStaticWZ* m_pTextControl[LIST_COL];
};

inline POSTYPE uiDominationTowerBettingListDlg::getCtrlIDToPos( WzID wzId )
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