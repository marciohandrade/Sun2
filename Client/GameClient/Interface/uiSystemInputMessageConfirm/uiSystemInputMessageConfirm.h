#ifndef     __USER_INTERFACE_SYSTEM_INPUT_MESSAGE_CONFIRM_H__
#define     __USER_INTERFACE_SYSTEM_INPUT_MESSAGE_CONFIRM_H__
//------------------------------------------------------------------------------
/**
    @class uiSystemInputMessageConfirm
*/
#include "uiSystemInputMessageConfirm_def.h"
#include "uibase.h"
#include "interfacemanager.h"
#include "uiSystemMan/uiSystemMan.h"
#include "GlobalFunc.h"

class uiSystemInputMessageConfirm :
    public uiBase
{
    enum eDIALOG_POS
    {
        DLGPOS_BTN_YES=0,
        DLGPOS_BTN_NO,
		DLGPOS_EDIT,
        DLGPOS_TEXT_CONFIRM_MSG,
		DLGPOS_TITLE,
		DLGPOS_CLOSE,
        DIALOG_MAX,
    };

public:
    uiSystemInputMessageConfirm(InterfaceManager *pUIMan);
    virtual ~uiSystemInputMessageConfirm();

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
    void                    SetConfirmMessage(const TCHAR * pszMessage);
    void                    SetLockingEffect();
    bool                    GetLockingEffect();
	void                    SetEditMessage(TCHAR* szEditMessage);
	void					SetTitleMessage(TCHAR* szEditMessage);

    void                    SetNumberic(bool numberic);

    //--------------------------------------------------------------------------
    // functions
    //
public:
    void                    RenderRect(RECT rc, WzColor color);
    
	void					SetFocus( void );

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
    WzSimpleMesh            *m_pMessageMesh;
    std::vector<util::_tstring> m_vstr;

};

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemInputMessageConfirm::SetConfirmMessage(const TCHAR * pszMessage)
{
	CCtrlStaticWZ* pTextCtrl = (CCtrlStaticWZ *)getControl(DLGPOS_TEXT_CONFIRM_MSG);
	RECT rcCtrl;
	if(pTextCtrl)
	{
		rcCtrl = pTextCtrl->GetSizeRect();

		int iCtrlWidth = rcCtrl.right - rcCtrl.left - 10;

		GlobalFunc::ProcessWordWrapByFontSize( &this->m_vstr, const_cast<TCHAR *>(pszMessage), c_wiSystemMsgFont, _tcslen(pszMessage), iCtrlWidth );
	}
}

//------------------------------------------------------------------------------
/**
*/
inline
POSTYPE
uiSystemInputMessageConfirm::getCtrlIDToPos( WzID wzId )
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
uiSystemInputMessageConfirm::getControl( POSTYPE AtPos )
{
    assert( AtPos < DIALOG_MAX );

    return GetControlWZ( m_wzId[AtPos] );
}

//------------------------------------------------------------------------------
/**
*/
inline
void
uiSystemInputMessageConfirm::SetLockingEffect()
{
    this->m_bLockingEffect = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
bool
uiSystemInputMessageConfirm::GetLockingEffect()
{
    return this->m_bLockingEffect;
}


//------------------------------------------------------------------------------
#endif  //  __USER_INTERFACE_SYSTEM_IDNUMBER_CONFIRM_H__
