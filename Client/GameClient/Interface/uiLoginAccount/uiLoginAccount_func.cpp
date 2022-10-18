//------------------------------------------------------------------------------
//  uiLoginAccount_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginAccount.h"
#include "uiLoginMan/uiLoginMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
/**
*/
uiLoginMan*
uiLoginAccount::GetManager()
{
    uiLoginMan* loginMan =
        static_cast<uiLoginMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_LOGIN));
    assert (loginMan);
    return loginMan;
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::ConnectBtnOn()
{
    this->m_pBtnControl->SetTextColor(RGBA(240, 201, 121, 255));
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::ConnectBtnOff()
{
    this->m_pBtnControl->SetTextColor(RGBA(100, 100, 100, 180));
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::ClearAll()
{
    CCtrlDxIMEEditBox* editCtrl;

    editCtrl = static_cast<CCtrlDxIMEEditBox *>(m_pEdtControls[0]);
    if (editCtrl)
        editCtrl->ClearText();
    
    editCtrl = static_cast<CCtrlDxIMEEditBox *>(m_pEdtControls[1]);
    if (editCtrl)
        editCtrl->ClearText();
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::SetFocusID(bool clear)
{
    CCtrlDxIMEEditBox* editCtrl =
        static_cast<CCtrlDxIMEEditBox *>(m_pEdtControls[0]);
    assert (editCtrl);

    if (editCtrl)
    {
        if (clear)
            editCtrl->ClearText();
       
		GlobalFunc::SetFocus( (SolarDialog *)this,  editCtrl );
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginAccount::SetFocusPW(bool clear)
{
    CCtrlDxIMEEditBox* editCtrl =
        static_cast<CCtrlDxIMEEditBox *>(m_pEdtControls[1]);
    assert (editCtrl);

    if (editCtrl)
    {
        if (clear)
            editCtrl->ClearText();

		GlobalFunc::SetFocus( (SolarDialog *)this,  editCtrl );
    }
}
//------------------------------------------------------------------------------
/**
*/
BOOL uiLoginAccount::IsInvalidPasswordType(TCHAR * pszPassword)
{
	if( ! pszPassword )
		return FALSE;

	int iLength = _tcslen(pszPassword );
	
	if ( iLength <= 0)
		return FALSE;

	for ( int i = 0 ; i < iLength ; ++i)
	{
		if(isdigit(pszPassword[i]) )
		{

		}
		else if ( isalpha(pszPassword[i]) )
		{

		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;	
}

BOOL uiLoginAccount::IsIncludeChiness(TCHAR * pszMessage)
{
	int length = _tcslen(pszMessage);
	if( length <= 0)
		return FALSE;

	for ( int i = 0; i < length ; ++i)
	{
		if( _ismbblead( pszMessage[i] ) )
			return TRUE;
		
	}
	return FALSE;
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------

