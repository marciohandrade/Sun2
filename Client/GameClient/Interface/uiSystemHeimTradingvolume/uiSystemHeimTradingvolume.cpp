#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
#include "uiSystemHeimTradingvolume.h"
#include "uiSystemMan/uiSystemMan.h"
#include "HeimTradingvolumeSubject/HeimTradingvolumeSubject.h"
#include "GlobalFunc.h"
#include "KeyQueueManager.h"

DWORD uiSystemHeimTradingvolume::current_dialog_key_ = 0;

uiSystemHeimTradingvolume::uiSystemHeimTradingvolume(InterfaceManager* interface_manager) : 
heim_tradingvolume_subject_(NULL), 
parent_dialog_key_(0)
{
	set_expired_time(0);
}

uiSystemHeimTradingvolume::~uiSystemHeimTradingvolume(void)
{
}

void uiSystemHeimTradingvolume::Init( CDrawBase* draw_base )
{
	RegisterControlPosition(StrToWzID("T999"), kControl_Title_Text);
	RegisterControlPosition(StrToWzID("T001"), kControl_Message_Text);
	RegisterControlPosition(StrToWzID("S000"), kControl_Heim_Text);
	RegisterControlPosition(StrToWzID("TI00"), kControl_Verify_Button);
	RegisterControlPosition(StrToWzID("TI01"), kControl_Cancel_Button);

	CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Message_Text);
	if (control_ptr)
	{
		control_ptr->ShowWindowWZ(WZ_HIDE);
	}

	RECT rc = {0, };
	ftext_render_.Init(g_pSunRenderer, rc);
}

void uiSystemHeimTradingvolume::Release()
{
	SAFE_DELETE(heim_tradingvolume_subject_);
	ftext_render_.Release();
}

void uiSystemHeimTradingvolume::OnShowWindow( BOOL is_show )
{
	if (is_show == TRUE)
	{
		keyMsg keyqueue_message;
		ZeroMemory(&keyqueue_message, sizeof(keyqueue_message));
		keyqueue_message.dwType = GetDialogKey();
		keyqueue_message.wParam = 0;
		keyqueue_message.lParam = GetDialogKey();
		keyqueue_message.DoSomething = DoSystemHeimTradingvolume;
		g_KeyQueueManager.PushBack(keyqueue_message);
	}
	else
	{
		if (is_modal() == true)
		{
			this->GetUIMan()->UnLockDlgs();
			this->GetUIMan()->DeAttachFirstDialog(GetDialogKey());
		}

		parent_dialog_key_ = 0;

		g_KeyQueueManager.DeleteMsg(GetDialogKey());
	}
}

void uiSystemHeimTradingvolume::MessageProc( SI_MESSAGE* message )
{
	if (IsLock())
		return;

	if (GetMan() == NULL)
		return;

	switch (message->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
		{
			OnLButtonClick(message);
		}
		break;
	default:
		SolarDialog::MessageProc(message);
		break;
	}
}

void uiSystemHeimTradingvolume::OnUpdateSolarDialog()
{
	if (expired_time() > 0)
	{
		if (clock_function::GetTickCount() >= expired_time())
		{
			SetDestroyWindow(TRUE);
		}
	}

	CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Message_Text);
	if (control_ptr)
	{
		ftext_render_.SetRect(control_ptr->GetSizeRect());
	}

	if(0 == parent_dialog_key_)
		return;

	SolarDialog* parent_dialog = g_InterfaceManager.GetDialog(parent_dialog_key_);
	if(NULL == parent_dialog)
		return;

	if (parent_dialog->IsVisibleWindow() == FALSE)
	{
		SetDestroyWindow(TRUE);
	}
}

void uiSystemHeimTradingvolume::OnRenderSolarDialog()
{
	ftext_render_.Render();
}

bool uiSystemHeimTradingvolume::OnLButtonClick( SI_MESSAGE* message )
{
	bool result = true;
	DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
	switch (control_position)
	{
	case kControl_Verify_Button:
		{
			heim_tradingvolume_subject_->Execute();

			SetDestroyWindow(TRUE);
		}
		break;
	case kControl_Cancel_Button:
		{
			heim_tradingvolume_subject_->Cancel();

			SetDestroyWindow(TRUE);
		}
		break;
	default:
		{
		}
		break;
	}
	return result;
}

void uiSystemHeimTradingvolume::DoSystemHeimTradingvolume( DWORD l_param, DWORD r_param )
{
	uiBase* system_heim_tradingvolume = GET_DIALOG(r_param);
	if (system_heim_tradingvolume != NULL)
	{
		system_heim_tradingvolume->SetDestroyWindow(TRUE);
	}
}

void uiSystemHeimTradingvolume::SetTimer( const DWORD life_time )
{
	if (life_time == 0)
	{
		set_expired_time(0);
		return;
	}

	DWORD current_time = clock_function::GetTickCount();
	set_expired_time(current_time + life_time);
}

void uiSystemHeimTradingvolume::Clear()
{
	SAFE_DELETE(heim_tradingvolume_subject_);
}

void uiSystemHeimTradingvolume::SetData( HeimTradingvolumeSubject* heim_tradingvolume_subject )
{
	heim_tradingvolume_subject_ = heim_tradingvolume_subject;

	SettingInfo();
}

void uiSystemHeimTradingvolume::SettingInfo()
{
	if (heim_tradingvolume_subject_ == NULL)
	{
		SetDestroyWindow(TRUE);
		return;
	}

	MONEY price = heim_tradingvolume_subject_->GetTradingvolume();
	MONEY option_value = GAMEOPTION->GetHeimTradingvolumeNotification();

	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR total_string_option[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR total_string[INTERFACE_STRING_LENGTH] = {0, };	
	g_InterfaceManager.GetInterfaceString(2169, format_string);
	TCHAR money_char_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR money_string[INTERFACE_STRING_LENGTH] = {0, };
	std::string money_std_string;
	GlobalFunc::SetPointText(money_char_string, option_value);
	money_std_string = "<color=";
	money_std_string += GlobalFunc::GetMoneyColorHexForString(option_value);
	money_std_string += ">";
	Snprintf(money_string, INTERFACE_STRING_LENGTH-1, "%s%s%s", money_std_string.c_str(), money_char_string, "</color>");
	Snprintf(total_string_option, INTERFACE_STRING_LENGTH-1, format_string, money_string);
	Snprintf(total_string, INTERFACE_STRING_LENGTH-1, _T("%s%s%s"), "<font=st11><color=ffd77500>", total_string_option, "</font></color>");
	CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Message_Text);
	if (control_ptr)
	{
		if (ftext_parser_.Parser(total_string, ftext_sentence_))
		{
			ftext_render_.ClearSentence();
			ftext_render_.Append(ftext_sentence_);
			ftext_render_.SetRect(control_ptr->GetSizeRect());
		}
	}

	CCtrlStaticWZ* static_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Heim_Text));
	if (static_control)
	{
		TCHAR message[INTERFACE_STRING_LENGTH] = {0, };
		GlobalFunc::SetPointText(message, price);
		static_control->SetTextColor(GlobalFunc::GetMoneyColor(price));
		static_control->SetTextWZ(message);
	}
}

void uiSystemHeimTradingvolume::SetModal( const bool& is_modal )
{
	set_modal(true);

	DWORD dialog_key = GetDialogKey();

	g_InterfaceManager.AttachFirstDialog(dialog_key);
	g_InterfaceManager.LockDlgs(dialog_key);
}

#endif // _NA_008678_20160203_ADD_HEIM_TRADINGVOLUME_NOTIFICATION_OPTION
