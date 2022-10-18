#pragma once
#include "uibase.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"


class uiDominationTowerMainDlg: public uiBase
{
	enum eDIALOG_POS
	{
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        DLGPOS_GUILDMARK_PIC1,
        DLGPOS_GUILDRELATION_BACKPANEL_PIC,

        DLGPOS_ENTER_BUTTON,

        DLGPOS_STATIC_GUILD_NAME,
        DLGPOS_STATIC_GUILD_MASTER_NAME,

        DLGPOS_CLOSE_BUTTON,
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
        DLGPOS_TITLE_TEXT,
        DLGPOS_EXPLANATION_TEXT,
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

        DIALOG_MAX,
#else
		DLGPOS_GUILDMARK_PIC1,
		DLGPOS_GUILDMARK_PIC2,
		DLGPOS_GUILDRELATION_BACKPANEL_PIC,

		DLGPOS_ENTER_BUTTON,
		DLGPOS_BETTING_BUTTON,
		DLGPOS_TOWER_MANAGE_BUTTON,
        DLGPOS_JOIN_DOMINATION_BUTTON,
        DLGPOS_OBSERVER_DOMINATION_BUTTON,

		DLGPOS_CLOSE_BUTTON,

		DIALOG_MAX,
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
	};

public:
	uiDominationTowerMainDlg(InterfaceManager *pUIMan);
	virtual ~uiDominationTowerMainDlg();

public:
	virtual void Init(CDrawBase* pDrawBase);
	virtual void OnShowWindow(BOOL val);
	virtual void OnRenderSolarDialog();
	virtual void MessageProc( SI_MESSAGE * pMessage );

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else
    void RECV_DOMINATION_MEMBER_JOIN_ACK(MSG_CG_DOMINATION_MEMBER_JOIN_ACK* pMsg);
    void RECV_DOMINATION_MEMBER_LEAVE_ACK(MSG_CG_DOMINATION_MEMBER_LEAVE_ACK* pMsg);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    void SetShowButtonFlag(MSG_CG_DOMINATION_NOTICE_ACK* recv_packet);

private:
	POSTYPE getCtrlIDToPos( WzID wzId );

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    void OnClickDominationEnterButton();
#else
    void OnClickMemberJoinButton();
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    void set_domination_explanation_text();
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

private:
	static WzID m_wzId[DIALOG_MAX];
	CCtrlPictureWZ* m_pGuildMarkPic1;
    CCtrlPictureWZ* m_pRelationBackPic;
    CCtrlButtonWZ* m_pApplyButton;
    CCtrlButtonWZ* m_pCloseButton;
    
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    CCtrlStaticWZ* guild_name_;
    CCtrlStaticWZ* guild_master_name_;
#else
    CCtrlPictureWZ* m_pGuildMarkPic2;
    CCtrlButtonWZ* m_pBettingButton;
    CCtrlButtonWZ* m_pTowerManageButton;
    CCtrlButtonWZ* m_pJoinDominationButton;
    CCtrlButtonWZ* m_pObserverDominationButton;
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
#ifdef _NA_007115_20140313_MONDSCHIEN_RENEWAL
    CCtrlStaticWZ* domination_explanation_string_;
    CCtrlStaticWZ* dialog_title_string_;
#endif // _NA_007115_20140313_MONDSCHIEN_RENEWAL

    DWORD m_dwTooltipControl;
};

inline POSTYPE uiDominationTowerMainDlg::getCtrlIDToPos( WzID wzId )
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

