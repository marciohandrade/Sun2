#include "SunClientPrecompiledHeader.h"
#include "InterfaceManager.h"
#include "uiSocketMan/uiSocketMan.h"
#include "uiSocketComposite/uiSocketComposite.h"

const char  dialog_name_socket_composite[] = ("Data\\Interface\\71_11_Socket.iwz");
const DWORD compare_id_socket_composite = StrToWzID("7111");

uiSocketMan::uiSocketMan(InterfaceManager* ui_manager) 
: uiBaseMan(ui_manager)
{

}


void uiSocketMan::OnInitialize()
{
	ui_socket_composite_ = CreateUIDialog<uiSocketComposite>(IM_SOCKET_MANAGER::SOCKET_COMPOSITE, 
        dialog_name_socket_composite, this, TRUE);
}
