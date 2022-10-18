//------------------------------------------------------------------------------
//  uiLoginChannelList_func.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"
#include "uiLoginChannelList.h"
#include "SceneBase.h"
#include "LoginScene.h"
#include "InterfaceManager.h"
#include <Protocol_CL.h>
#include <PacketStruct_CL.h>
#include "LoadScene.h"
#include "uiLoginMan/uiLoginMan.h"
#include "uiLoginServerList/uiLoginServerList.h"
#include "interfacemanager.h"
#include "globalfunc.h"
//------------------------------------------------------------------------------
/**
*/
uiLoginMan*
uiLoginChannelList::GetManager()
{
    uiLoginMan* loginMan = GET_DIALOG_MANAGER(uiLoginMan , UIMAN_LOGIN);
    assert (loginMan);
    return loginMan;
}

//------------------------------------------------------------------------------
/**
*/
INT
uiLoginChannelList::TestSelectedAuthServerGroup()
{
	uiLoginServerList * serverList = GET_CAST_DIALOG( uiLoginServerList, IM_LOGIN_MANAGER::LOGIN_SERVERLIST );
    
    if (!serverList)
        return -1;

    if (!GetManager())
        return -1;

	if (!GetManager()->isRecvGroupServerData())
		return -1;

    return serverList->GetCurrentSelectedServerIdx();
}


//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
