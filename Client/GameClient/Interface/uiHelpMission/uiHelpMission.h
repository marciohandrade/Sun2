#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiHelpMission class
//------------------------------------------------------------------------------

#include "uibase.h"

const int c_iMaxHelpNum = 4;

//const DWORD c_ListSelBarColor	= RGBA(255,255,255,0);
//const DWORD c_ListBKColor		= RGBA(255,255,255,0);
//const DWORD c_ListTextColor		= RGBA(255,255,255,255);
//const DWORD c_ListSelTextColor	= RGBA(255,255,255,255);

typedef struct _HELP_MISSION_ITEM
{
	DWORD dwDCode;
	DWORD dwCode;
	IconImage *pImage;
} HELP_MISSION_ITEM;

class uiHelpMan;

class uiHelpMission: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_HM_BACK_1,
		DLGPOS_HM_BACK_2,
		DLGPOS_HM_BACK_3,
		DLGPOS_HM_BACK_4,

		DLGPOS_HM_ITEM_1,
		DLGPOS_HM_ITEM_2,
		DLGPOS_HM_ITEM_3,
		DLGPOS_HM_ITEM_4,

		DLGPOS_HM_BTN_OFF_1,
		DLGPOS_HM_BTN_OFF_2,
		DLGPOS_HM_BTN_OFF_3,
		DLGPOS_HM_BTN_OFF_4,

		DLGPOS_HM_BTN_ON_1,
		DLGPOS_HM_BTN_ON_2,
		DLGPOS_HM_BTN_ON_3,
		DLGPOS_HM_BTN_ON_4,

		DLGPOS_HM_BTN_WIN_OFF,
		DLGPOS_HM_BTN_WIN_ON,

		DLGPOS_HM_VSCR,

		DLGPOS_HM_TXT_ITEM_NAME_1,
		DLGPOS_HM_TXT_ITEM_NAME_2,
		DLGPOS_HM_TXT_ITEM_NAME_3,
		DLGPOS_HM_TXT_ITEM_NAME_4,

		DLGPOS_HM_LST_ITEM_DESC_1,
		DLGPOS_HM_LST_ITEM_DESC_2,
		DLGPOS_HM_LST_ITEM_DESC_3,
		DLGPOS_HM_LST_ITEM_DESC_4,

		DLGPOS_DLG_EVENTRECT,
#ifdef _NA_000000_20130114_RENEWER_UI
		DLGPOS_PICTURE_ITEM_1,
		DLGPOS_PICTURE_ITEM_2,
		DLGPOS_PICTURE_ITEM_3,
		DLGPOS_PICTURE_ITEM_4,
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
        DIALOG_HM_MAX,
    };

    enum
    {
        POS_BTN_SIZE = DLGPOS_HM_BTN_WIN_ON - DLGPOS_HM_BACK_1 + 1,
		POS_TXT_SIZE = DLGPOS_HM_TXT_ITEM_NAME_4 - DLGPOS_HM_TXT_ITEM_NAME_1 + 1,
    };

	

public:
    uiHelpMission(InterfaceManager *pUIMan);
    virtual ~uiHelpMission();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

	virtual BOOL			InterSectRect( POINT point );

protected:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiHelpMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();
	void					_addIconImage(int iIndex, DWORD dwCode,DWORD dwDCode);
	void					_removeIconImage(int iIndex);
	void					_removeAllIconImage();

	void					_clearMissionItemList();
	void					_setMissionItem(int iIndex, DWORD dwNCode, DWORD dwDCode, DWORD dwImgCode);
	void					_showMissionItem(int iIndex, bool bShow);

	bool					_isAllHideMissionItem();
	
	void					UpScroll();
	void					DownScroll();

	void					renderToolTip(int nIndex,CControlWZ *pCtrl,RECT &rc);

public:
	void					UpdateDisplay(int iNewItemTextID = -1);
	void					RefreshDisplay();

	void					ShowAllMissionItem(bool bShow);
	

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_HM_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiHelpMan*				m_pHelpMan;

    CCtrlListWZ*            m_pListItemDesc[c_iMaxHelpNum];
    CCtrlButtonWZ*          m_pBtnBack[c_iMaxHelpNum];
	CCtrlButtonWZ*          m_pBtnItemImage[c_iMaxHelpNum];
	CCtrlButtonWZ*          m_pBtnItemOff[c_iMaxHelpNum];
	CCtrlButtonWZ*          m_pBtnItemOn[c_iMaxHelpNum];
	CCtrlStaticWZ*			m_pTxtItemName[c_iMaxHelpNum];

#ifdef _NA_000000_20130114_RENEWER_UI
	CCtrlPictureWZ*			m_pPictureItem[c_iMaxHelpNum];
#endif//#ifdef _NA_000000_20130114_RENEWER_UI

	CCtrlButtonWZ*          m_pBtnOff;
	CCtrlButtonWZ*          m_pBtnOn;

	CCtrlVScrollWZ*			m_pScroll;

	HELP_MISSION_ITEM		m_HelpMissionItem[c_iMaxHelpNum];

	CCtrlButtonWZ*          m_pEventRect;

		

    bool                    m_bControlsInitialized;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiHelpMission::getCtrlIDToPos( WzID wzId )
{
    for( POSTYPE i = 0 ; i < DIALOG_HM_MAX ; ++i )
    {
        if( m_wzId[i] == wzId )
        {
            return i;
        }
    }
    return DIALOG_HM_MAX;
}

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiHelpMission::getControl( POSTYPE AtPos )
{
    ASSERT( AtPos < DIALOG_HM_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiHelpMan*
uiHelpMission::GetManager()
{
    return this->m_pHelpMan;
}

//------------------------------------------------------------------------------

