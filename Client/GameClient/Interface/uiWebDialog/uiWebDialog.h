#pragma once
//------------------------------------------------------------------------------
//
//    작업자: 이지연
//    uiWebDialog class
//------------------------------------------------------------------------------

#include "uiWebDialog_def.h"
#include "uiBase.h"

#ifdef _KIKI_UI_WEB_DIALOG

class uiWebDialogMan;

class uiWebDialog : public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_EXIT,
        DLGPOS_WEB_CTRL,

        DIALOG_MAX,
    };


public:
    uiWebDialog(InterfaceManager *pUIMan);
    virtual ~uiWebDialog();

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
    uiWebDialogMan*             GetManager();

    //--------------------------------------------------------------------------
    // functions
    //
private:
	BOOL					_isEnable();

public:

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:

protected:
    virtual void	        OnShowWindow( BOOL val );

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

			void			Navigate( WCHAR * pURL, BOOL bWindowShow );
			void			BlankPage();

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    uiWebDialogMan*			m_pWebDialogMan;

	CCtrlWebBrowserWZ *		m_pWebControl;
    bool                    m_bControlsInitialized;
};

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiWebDialog::getCtrlIDToPos( WzID wzId )
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
uiWebDialog::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
uiWebDialogMan*
uiWebDialog::GetManager()
{
    return this->m_pWebDialogMan;
}

//------------------------------------------------------------------------------

#endif