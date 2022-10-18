#include "SunClientPrecompiledHeader.h"
#include "AskPkPopupDialog.h"
#include "GameConst_Color.h"
#include "uiLoginChannelList/uiLoginChannelList.h"
#include "uiAssistMan/uiAssistMan.h"
#include "uiLoginMan/uiLoginMan.h"

//------------------------------------------------------------------------------ 
AskPkPopupDialog::AskPkPopupDialog(InterfaceManager* ui_man) : AskDialog(ui_man), auth_server_data_(0)
{

}

AskPkPopupDialog::~AskPkPopupDialog()
{
    auth_server_data_ = NULL;
}

void AskPkPopupDialog::OnUpdateSolarDialog()
{
	if (this->GetStatus() == ASK_STATUS_OK)
	{
		this->DoIt();
		SetStatus(ASK_STATUS_NORMAL);
	}
	else if (this->GetStatus() == ASK_STATUS_CANCEL)
	{
		this->Cancel();
		SetStatus(ASK_STATUS_NORMAL);
	}
}

void AskPkPopupDialog::MessageProc(SI_MESSAGE * pMessage)
{
	switch(pMessage->eResultMsg) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch(getCtrlIDToPos(pMessage->dwCtrlID))
			{
			case ASK_OK:
				{
					this->m_status = ASK_STATUS_OK;
				}
				break;

			case ASK_CANCEL:
				{
					this->m_status = ASK_STATUS_CANCEL;
				}
				break;
			}
		}
		break;
	default:
		{
			SolarDialog::MessageProc(pMessage);
		}
		break;
	}
}

void AskPkPopupDialog::OnShowWindow(BOOL val)
{
	if (val)
	{
		MoveCenter();
		SetTexts();
		g_InterfaceManager.LockDlgs(IM_ASSIST_MANAGER::PK_POPUP_DIALOG); // 화면 락
	}
	else
	{
		g_InterfaceManager.UnLockDlgs();
	}
}

void AskPkPopupDialog::SetTexts()
{
	// 6036 // 해당 서버는 PK가 가능한 서버입니다.\n신중하게 선택하여 주시기 바랍니다.
	TCHAR message[INTERFACE_STRING_LENGTH] ={0,};
	g_InterfaceManager.GetInterfaceString(6036, message);
	InsertMessage(message, c_InterfaceBKColor, c_InterfaceTextColor);
}
//------------------------------------------------------------------------------ 
void AskPkPopupDialog::DoIt()
{
#if NATION_BUILD_VERSION >= 1102
    uiLoginMan* login_man = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
    if (login_man != NULL)
    {
        login_man->SEND_AUTH_SERVER_SELECT_SYN(auth_server_data());
    }
#else
	POSTYPE pos		= ctrl_id_pos();
	int		svidx	= server_index(); 

	uiLoginChannelList* login_channel_list;
	login_channel_list = static_cast<uiLoginChannelList*>(
		GetUIMan()->GetDialog(InterfaceManager::DIALOG_LOGIN_CHANNEL_LIST));
	
	//여기에서 서버에 접속을 시도함
	login_channel_list->SelectChannel(pos, svidx);
#endif //#if NATION_BUILD_VERSION >= 1102
	
	ShowInterface(FALSE);
}
//------------------------------------------------------------------------------ 
void AskPkPopupDialog::Cancel()
{	
	ShowInterface(FALSE);
}


