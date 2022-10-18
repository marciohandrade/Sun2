#ifndef     __USER_INTERFACE_SYSTEM_IDNUMBER_CONFIRM_H__
#define     __USER_INTERFACE_SYSTEM_IDNUMBER_CONFIRM_H__
//------------------------------------------------------------------------------
/**
    @class uiSystemIdNumberConfirm

    system confirm

    (C) 2005 ckbang
*/
#include "uiSystemIdNumberConfirm_def.h"
#include "uibase.h"
#include "uiSystemMan/uiSystemMan.h"
#include "Interfacemanager.h"
#include "GlobalFunc.h"
class uiSystemIdNumberConfirm :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_YES=0,
        DLGPOS_BTN_NO,
		DLGPOS_EDIT,
        DLGPOS_LST_CONFIRM_MSG,
        DIALOG_MAX,
    };

public:
    uiSystemIdNumberConfirm(InterfaceManager *pUIMan);
    virtual ~uiSystemIdNumberConfirm();

    //--------------------------------------------------------------------------
    // init/release
    //
public:
    virtual void            Init(CDrawBase* pDrawBase);
    virtual void            Release();
    void                    Clear();
	void                    ClearToken();

    //--------------------------------------------------------------------------
    // helper/comms.
    //
public:
    fn_SysConfirmString     m_fnSysConfirmString;
#ifdef _NA_000000_20130114_RENEWER_UI
    void                    SetConfirmMessage(const TCHAR * pszMessage,bool bTapcheck=false);
#else
	void                    SetConfirmMessage(const TCHAR * pszMessage);
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
    void                    SetLockingEffect();
    bool                    GetLockingEffect();

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    RenderRect(RECT rc, WzColor color);
    

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
    CCtrlStaticWZ *         getCtrlText(POSTYPE pos);
    static WzID		        m_wzId[DIALOG_MAX];

    //--------------------------------------------------------------------------
    // procedure
    //
public:
    virtual void	        NetworkProc( MSG_BASE * pMsg ) {};      // Pure
    virtual void	        MessageProc( SI_MESSAGE * pMessage );   // Pure

    //--------------------------------------------------------------------------
    // member vars
    //
private:
    bool                    m_bLockingEffect;

    std::vector<util::_tstring> m_vstr;

    WzSimpleMesh            *m_pMessageMesh;
    
};

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemIdNumberConfirm::SetConfirmMessage(const TCHAR * pszMessage
#ifdef _NA_000000_20130114_RENEWER_UI
											,bool bTapcheck
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
											)
{
	RECT rcCtrl;
	CControlWZ * pLstControl = getControl( DLGPOS_LST_CONFIRM_MSG );

	if ( pLstControl )
	{
		rcCtrl = pLstControl->GetSizeRect();

		int iCtrlWidth = rcCtrl.right - rcCtrl.left - 10;
#ifdef _NA_000000_20130114_RENEWER_UI
		GlobalFunc::ProcessWordWrapByFontSize2( &this->m_vstr, const_cast<TCHAR *>(pszMessage), c_wiSystemMsgFont, _tcslen(pszMessage), iCtrlWidth ,false,bTapcheck);
#else
		GlobalFunc::ProcessWordWrapByFontSize( &this->m_vstr, const_cast<TCHAR *>(pszMessage), c_wiSystemMsgFont, _tcslen(pszMessage), iCtrlWidth );
#endif//#ifdef _NA_000000_20130114_RENEWER_UI
	}

}

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiSystemIdNumberConfirm::getCtrlIDToPos( WzID wzId )
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
uiSystemIdNumberConfirm::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemIdNumberConfirm::SetLockingEffect()
{
    this->m_bLockingEffect = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiSystemIdNumberConfirm::GetLockingEffect()
{
    return this->m_bLockingEffect;
}


//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_SYSTEM_IDNUMBER_CONFIRM_H__
