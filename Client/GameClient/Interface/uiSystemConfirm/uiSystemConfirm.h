#ifndef     __USER_INTERFACE_SYSTEM_CONFIRM_H__
#define     __USER_INTERFACE_SYSTEM_CONFIRM_H__
//------------------------------------------------------------------------------
/**
    @class uiSystemConfirm

    system confirm

    (C) 2005 ckbang
*/
#include "uiSystemConfirm_def.h"
#include "uibase.h"
#include "interfacemanager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GlobalFunc.h"
class Command;
class uiSystemConfirm :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_YES=0,
        DLGPOS_BTN_NO,
        DLGPOS_LST_CONFIRM_MSG,
        DLGPOS_BTN_CLOSE,

        DIALOG_MAX,
    };

public:
    uiSystemConfirm(InterfaceManager *pUIMan);
    virtual ~uiSystemConfirm();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    void                    Clear();
	void                    ClearToken();

private:
    Command*               m_pOkCommand;
    Command*               m_pCancelCommand;
public:
    void                    SetOkCommand( Command* pCommand );
    void                    SetCancelCommand( Command* pCommand );

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    fn_SysConfirm           m_fnSysConfirm;
    fn_SysConfirmCloseButton m_fnSysConfirmCloseButton;
    void                    SetConfirmMessage(const TCHAR * pszMessage);
    void                    SetLockingEffect();
    bool                    GetLockingEffect();


    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    RenderRect(RECT rc, WzColor color);
    

	void					SetTitle(LPCTSTR pszTitle);
	void					SetSubject(LPCTSTR pszSubject);
	void					SetBtnStringID(DWORD dwOkID, DWORD dwCancleID);
    void                    SetShowCancleButton(bool is_show);

    //--------------------------------------------------------------------------
    // dialog functions
    //
public:
	DWORD					m_dwDialogKey;

protected:
    virtual void	        OnShowWindow( BOOL val );
    virtual void            OnUpdateSolarDialog();
    virtual void            OnRenderSolarDialog();

private:

protected:
    CControlWZ *	        getControl( POSTYPE AtPos );
    POSTYPE			        getCtrlIDToPos( WzID wzId );
    CCtrlStaticWZ *         getCtrlText(POSTYPE pos);
    static WzID		        m_wzId[DIALOG_MAX];
    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg ) {};         // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

	void					SetbShow(BOOL bShow);
    void                    OnClose();

	void					SetTimer(DWORD dwIntervalTime){ m_Timer.SetTimer(dwIntervalTime); }
	void					ClearTimer(){ m_Timer.Clear();}

    void                    SetTitleTextGap(BYTE gap) { m_TitleTextGap = gap; }
    //--------------------------------------------------------------------------
    // member vars
    //
private:
    bool                    m_bLockingEffect;
	BOOL					m_bShow;

    std::vector<util::_tstring> m_vstr; 
	std::vector<util::_tstring> m_vSubject; 

	util::_tstring			m_strTitle;
	
	DWORD					m_dwOKStrID;
	DWORD					m_dwCancleStrID;
	CCtrlStaticWZ *			m_pCtrlStatic[2];

    WzSimpleMesh            *m_pMessageMesh;


	util::Timer				m_Timer;
	bool					m_bDestroyWindow;
	DWORD					m_TimerKey;

    BYTE                    m_TitleTextGap;

};

//////////////////////////////////////////////////////////////////////////
//
inline
void
uiSystemConfirm::SetbShow(BOOL bShow)
{
	m_bShow = bShow;
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemConfirm::SetConfirmMessage(const TCHAR * pszMessage)
{
	SetBtnStringID( this->m_dwOKStrID, this->m_dwCancleStrID );

	
	CControlWZ * pLstControl = getControl( DLGPOS_LST_CONFIRM_MSG );
	if( pLstControl )
	{
		RECT rcCtrl = pLstControl->GetSizeRect();

		int iCtrlWidth = rcCtrl.right - rcCtrl.left - 14;

		GlobalFunc::ProcessWordWrapByFontSize( &this->m_vstr, const_cast<TCHAR *>(pszMessage), c_wiSystemMsgFont, _tcslen(pszMessage), iCtrlWidth );
	}

}

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiSystemConfirm::getCtrlIDToPos( WzID wzId )
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
uiSystemConfirm::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemConfirm::SetLockingEffect()
{
    this->m_bLockingEffect = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiSystemConfirm::GetLockingEffect()
{
    return this->m_bLockingEffect;
}


//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_SYSTEM_CONFIRM_H__
