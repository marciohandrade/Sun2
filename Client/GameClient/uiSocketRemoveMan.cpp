#include "SunClientPrecompiledHeader.h"

#include "uiSocketRemoveMan.h"
#include "uiSocketRemove/uiSocketRemove.h"
#include "InterfaceManager.h"
//------------------------------------------------------------------------------
/**
*/
uiSocketRemoveMan::uiSocketRemoveMan(InterfaceManager *pUIMan) : uiBaseMan(pUIMan)
{

}

void uiSocketRemoveMan::OnInitialize()
{
    m_pSocketRemove = CreateUIDialog<uiSocketRemove>( 
        StrToWzID("0993"), 
        "Data\\Interface\\99_3_Event_socket.iwz", 
        this,
        TRUE);
}