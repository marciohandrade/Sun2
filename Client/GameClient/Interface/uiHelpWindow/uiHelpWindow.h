#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiHelpWindow class
//------------------------------------------------------------------------------

#include "uiHelpWindow_def.h"

#include "HelpInfoParser.h"
#include "FTextRender.h"
#include "uibase.h"
class uiHelpMan;

class uiHelpWindow: public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_EXIT,
        DLGPOS_BTN_OK,

		DLGPOS_TXT_TITLE,
		DLGPOS_TXT_SUBTITLE,
		DLGPOS_TXT_DESC,

		DLGPOS_SCR_VERTICAL,

        DIALOG_MAX,
    };

    enum
    {
        POS_BTN_SIZE = DLGPOS_BTN_OK - DLGPOS_BTN_EXIT + 1,
		POS_TXT_SIZE = DLGPOS_TXT_DESC - DLGPOS_TXT_TITLE + 1,
    };

	enum
	{
		TXT_TITLE,
		TXT_SUBTITLE,
		TXT_DESC,
	};

public:
    uiHelpWindow(InterfaceManager *pUIMan);
    virtual ~uiHelpWindow();

    //--------------------------------	------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();

protected:
    void                    InitControls();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    uiHelpMan*				GetManager();

    //--------------------------------------------------------------------------
    // functions
	void					renderHelpInfo();
	void					SetHelpInfo(BASE_HelpInfo& helpInfo);

	void					SetHelpStringCode(DWORD dwStringCode);
    //
public:
    //--------------------------------------------------------------------------
    // dialog functions
    //
private:
	BOOL					_isEnable();
public:
	
protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnRenderSolarDialog();

public:
	virtual void            UpdatePosition();

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg );         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiHelpMan*				m_pHelpMan;

	CCtrlStaticWZ*          m_pTxtControls[POS_TXT_SIZE];
    CCtrlButtonWZ*          m_pBtnControls[POS_BTN_SIZE];
	CCtrlVScrollWZ*         m_pScrollControl;

    bool                    m_bControlsInitialized;

	FTextRender				m_TextRender;
	BASE_HelpInfo			m_HelpInfo;

	int						m_iCurScrollPos;
	
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiHelpWindow::getCtrlIDToPos( WzID wzId )
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

//------------------------------------------------------------------------------
/**
*/
inline
CControlWZ *
uiHelpWindow::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiHelpMan*
uiHelpWindow::GetManager()
{
    return this->m_pHelpMan;
}


//------------------------------------------------------------------------------
