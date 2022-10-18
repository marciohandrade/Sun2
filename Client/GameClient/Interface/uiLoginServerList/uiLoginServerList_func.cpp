//------------------------------------------------------------------------------
//  uiLoginServerList_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginServerList.h"
#include "SceneBase.h"
#include "LoginScene.h"
#include "InterfaceManager.h"
#include <Protocol_CL.h>
#include <PacketStruct_CL.h>
#include "LoadScene.h"
#include "uiLoginChannelList/uiLoginChannelList.h"
#include "uiLoginMan/uiLoginMan.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiSystemMan/uiSystemMan.h"

//------------------------------------------------------------------------------
/**
*/
uiLoginMan*
uiLoginServerList::GetManager()
{
    uiLoginMan* loginMan = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
    assert (loginMan);
    return loginMan;
}

//------------------------------------------------------------------------------
/**
*/
INT
uiLoginServerList::GetCurrentSelectedServerIdx()
{
    CCtrlListWZ * pLCtrl = (CCtrlListWZ *)getControl( DLGPOS_LST_SERVERS );
    if (pLCtrl)
    {
        if (pLCtrl->GetCurSel() == -1)
            return -1;

		return (int)pLCtrl->GetItemData(pLCtrl->GetCurSel());
    }

    return -1;
}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
