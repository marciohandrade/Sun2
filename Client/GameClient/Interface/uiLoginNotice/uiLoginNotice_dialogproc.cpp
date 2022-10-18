//------------------------------------------------------------------------------
//  uiLoginNotice_dialogproc.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#ifdef _CK_MOD_UI_LOGIN
#else
#include "uiLoginNotice.h"

//------------------------------------------------------------------------------
WzID uiLoginNotice::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("TI00"),  // DLGPOS_BTN_GO_HOMEPAGE=0,
    StrToWzID("B999"),  // DLGPOS_BTN_CLOSE,
    StrToWzID("T001"),  // DLGPOS_TXT_NOTICE,
    StrToWzID("L001"),  // DLGPOS_LST_NOTICE,
    StrToWzID("VS00"),  // DLGPOS_SCR_VERTICAL,
};

//------------------------------------------------------------------------------
/**
    called to show request
*/
void
uiLoginNotice::showWindow( BOOL val )
{
    this->m_bShow = val;

    if (val)
    {
        this->OffPending();
        this->OffLock();
    }
    else
    {
        this->OnPending();
        this->OnLock();
    }
}

//------------------------------------------------------------------------------
/**
*/
void
uiLoginNotice::MessageProc( SI_MESSAGE * pMessage )
{
    if (this->IsLock())
        return;

    switch( pMessage->eResultMsg ) 
    {
    case RT_MSG_LBUTTONDOWN:
        {
        }
        break;

	default:
		{
			SolarDialog::MessageProc( pMessage );
		}
		break;
    }
}

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------

#endif