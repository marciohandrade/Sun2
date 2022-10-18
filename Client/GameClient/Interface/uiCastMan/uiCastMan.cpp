#include "SunClientPrecompiledHeader.h"
#include "uicastman.h"
#include "interfacemanager.h"
#include "CastLeaveFieldDialog.h"
#include "CastDialog.h"

uiCastMan::uiCastMan(InterfaceManager *pUIMan):
uiBaseMan(pUIMan)
{
    m_pCastLeaveField	= NULL;
    m_pCastDlg			= NULL;
}

//------------------------------------------------------------------------------
/**
*/
void uiCastMan::OnInitialize()
{
    m_pCastLeaveField = new CastLeaveFieldDialog;
    if ( !g_InterfaceManager.LoadInterface(
        m_pCastLeaveField ,
        "Data\\Interface\\28_Cast_Return.iwz",
        StrToWzID("0281"), 
        FALSE) 
        )
    {

    }

    m_pCastDlg = new CastDialog;
    if ( !g_InterfaceManager.LoadInterface(
        m_pCastDlg,
        "Data\\Interface\\28_Cast_Window.iwz",
        StrToWzID("0280"),
        FALSE) 
        )
    {

    }
}

