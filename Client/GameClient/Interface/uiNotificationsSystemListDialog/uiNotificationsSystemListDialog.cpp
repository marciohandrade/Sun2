#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "uiNotificationsSystemListDialog.h"
#include "uiNotificationsSystemMan/uiNotificationsSystemMan.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#include "Application.h"
#include "GameConst_Color.h"

WzID uiNotificationsSystemListDialog::wz_id_[DIALOG_MAX] = 
{
	StrToWzID("L000"), 
	StrToWzID("V000"), 
};

uiNotificationsSystemListDialog::uiNotificationsSystemListDialog(InterfaceManager* ui_manager) : 
uiBase(ui_manager), 
is_show_(FALSE), 
notifications_list_(NULL), 
notifications_vscroll_(NULL), 
current_scroll_value_(0), 
max_scroll_value_(0), 
notifications_system_manager_(NULL)
{
}

uiNotificationsSystemListDialog::~uiNotificationsSystemListDialog(void)
{
	ClearList();
}

void uiNotificationsSystemListDialog::Init( CDrawBase* draw_base )
{
	uiBase::Init(draw_base);

	RegisterControlPosition(StrToWzID("L000"), DLGPOS_NOTIFICATIONS_LIST);
	RegisterControlPosition(StrToWzID("V000"), DLGPOS_NOTIFICATIONS_VSCROLL);

	notifications_list_ = static_cast<CCtrlListWZ*>(getControl(DLGPOS_NOTIFICATIONS_LIST));
	notifications_list_->SetSellHeight(21);
	notifications_list_->SetSellHeight(16);
	notifications_list_->SetStartLineGapTop(5);
	notifications_list_->SetStartLineGapLeft(8);

	notifications_vscroll_ = static_cast<CCtrlVScrollWZ*>(getControl(DLGPOS_NOTIFICATIONS_VSCROLL));
	notifications_vscroll_->SetChangeSizeThumb(33);

	notifications_system_manager_ = static_cast<uiNotificationsSystemMan*>(GetMan());

	VScrollDialog::ResizeingVScrollControl(0, 0);
}

void uiNotificationsSystemListDialog::Release()
{
	if ( notifications_list_ )
	{
		notifications_list_->DeleteItemAll();
	}
	notifications_deque_.clear();
}

void uiNotificationsSystemListDialog::OnShowWindow( BOOL is_show )
{
	if ( is_show )
	{
		MoveDefaultPosition();
	}
	else
	{

	}
}

void uiNotificationsSystemListDialog::OnUpdateSolarDialog()
{
	VScrollDialog::Update();

	UpdateScrollPosition();
}

void uiNotificationsSystemListDialog::OnRenderSolarDialog()
{

}

void uiNotificationsSystemListDialog::MessageProc( SI_MESSAGE* message )
{
	switch ( message->eResultMsg )
	{
	case RT_MSG_SB_LINETOP:
		{
			switch (getCtrlIDToPos(message->dwCtrlID))
			{
			case DLGPOS_NOTIFICATIONS_VSCROLL:
				{
					if (current_scroll_value_ > 0)
					{
						current_scroll_value_ -= 1;
						notifications_vscroll_->SetScrollPosWZ(current_scroll_value_);
						notifications_list_->SetBeginDrawIndex(current_scroll_value_);
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_SB_LINEBOTTOM:
		{
			switch(getCtrlIDToPos(message->dwCtrlID))
			{
			case DLGPOS_NOTIFICATIONS_VSCROLL:
				{
					if (current_scroll_value_ < max_scroll_value_)
					{
						current_scroll_value_ += 1;
						notifications_vscroll_->SetScrollPosWZ(current_scroll_value_);
						notifications_list_->SetBeginDrawIndex(current_scroll_value_);
					}
				}
				break;
			}
		}
		break;

	case RT_MSG_LBUTTONCLICK:
		{
			// no operation
		}
		break;

	default:
		{
			SolarDialog::MessageProc( message );
			VScrollDialog::MessageProc(message);
		}
	}
}

void uiNotificationsSystemListDialog::MoveDefaultPosition()
{
	POINT pt = g_pApplication->Get2DModeMapping();
	RECT rcClient;
	GetDialogWZ()->GetClientRect(&rcClient);

	POINT MyInitialPoint;
	MyInitialPoint.x = pt.x - (rcClient.right- rcClient.left);//-386;
	MyInitialPoint.y =  pt.y - (rcClient.bottom - rcClient.top + 125) - 4;
	this->MoveWindowWZ((float)MyInitialPoint.x, (float)MyInitialPoint.y);
}

void uiNotificationsSystemListDialog::MoveDefaultWindowPos()
{
	SolarDialog::MoveDefaultWindowPos();

	MoveDefaultPosition();
}

POSTYPE uiNotificationsSystemListDialog::getCtrlIDToPos( WzID wzId )
{
	for( POSTYPE i = 0 ; i < DIALOG_MAX ; ++i )
	{
		if( wz_id_[i] == wzId )
		{
			return i;
		}
	}
	return DIALOG_MAX;
}

void uiNotificationsSystemListDialog::UpdateScrollPosition()
{
	int total = notifications_list_->GetListRowCnt();//입력된 총 라인수를 구한다.
	if( total > MAX_SHOW_NOTIFICATIONS_LINE )
	{
		current_scroll_value_ = notifications_vscroll_->GetScrollPosWZ();
	}
	else
	{
		current_scroll_value_ = 0;
	}
	notifications_list_->SetBeginDrawIndex( current_scroll_value_ );
}

void uiNotificationsSystemListDialog::PushBackNotificationsMsg( std::string notifications_message, 
															    const NotificationType& notification_type, 
																const NotificationInfo& info )
{
	if (notifications_list_ == NULL)
	{
		return;
	}

	if (notifications_vscroll_ == NULL)
	{
		return;
	}

	int size = notifications_list_->GetListRowCnt();
	if ( size < MAX_NOTIFICATION_COUNT * 2 )
	{
		notifications_deque_.push_back(notifications_message);
		AddString(const_cast<TCHAR*>(notifications_message.c_str()), notification_type, info);
	}
	else
	{
		std::deque<std::string>::iterator iter = notifications_deque_.begin();
		std::string begin_message = *iter;

		notifications_deque_.pop_front();
		notifications_deque_.push_back(notifications_message);
		AddString(const_cast<TCHAR*>(notifications_message.c_str()), notification_type, info);
	}
}

void uiNotificationsSystemListDialog::AddString( TCHAR* message, const NotificationType& notification_type, const NotificationInfo& info )
{
	int size = notifications_list_->GetListRowCnt();

	std::vector<util::_tstring> str_vector;

	RECT rect_size = notifications_list_->GetSize();

	int Interval = 20;

	GlobalFunc::ProcessWordWrapByFontSize(&str_vector, const_cast<TCHAR*>(message), StrToWzID("mn12"), _tcslen(message), rect_size.right - Interval);

	if ( size >= MAX_NOTIFICATION_COUNT * 2 )
	{
		// 라인 수만큼 지움
		for (int count = str_vector.size(); count > 0; --count)
		{
			notifications_list_->DeleteItem(0);
			--current_scroll_value_;
		}
		if (current_scroll_value_ < 0)
			current_scroll_value_ = 0;
	}

	for ( unsigned int i = 0; i < str_vector.size(); ++i )
	{
		int index = notifications_list_->InsertItemBack();

		WzColor text_color = WzColor_RGBA(255, 255, 0, 255);
		if ( notifications_system_manager_ )
		{
			text_color = notifications_system_manager_->GetNotificationsTextColor(info, notification_type);
		}

		GlobalFunc::SetItem(notifications_list_, index, 0, const_cast<TCHAR*>(str_vector[i].c_str()), StrToWzID("mn12"),
			DT_LEFT | DT_VCENTER, 
			RGBA(0, 0, 0, 0), 
			GlobalFunc::WzColorToRGBA(text_color), 
			GlobalFunc::WzColorToRGBA(text_color));
	}
	UpdateScrollBar();
}

void uiNotificationsSystemListDialog::UpdateScrollBar()
{
	notifications_list_->SetUnuseSelBar(FALSE);
	max_scroll_value_ = notifications_list_->GetListRowCnt() - MAX_SHOW_NOTIFICATIONS_LINE;

	if(max_scroll_value_ <= 0)
	{
		max_scroll_value_ = 0;
		current_scroll_value_ = 0;
		notifications_list_->SetBeginDrawIndex(current_scroll_value_);
	}

	notifications_vscroll_->SetScrollRangeWZ(0,max_scroll_value_);

	if ( max_scroll_value_ - current_scroll_value_ <= 7 || current_scroll_value_ > max_scroll_value_)
	{
		current_scroll_value_ = max_scroll_value_;
	}

	notifications_vscroll_->SetScrollPosWZ(current_scroll_value_);		
	notifications_list_->SetBeginDrawIndex(current_scroll_value_);
}

void uiNotificationsSystemListDialog::ClearList()
{
	if ( notifications_list_ )
	{
		notifications_list_->DeleteItemAll();
	}
	notifications_deque_.clear();
}

bool uiNotificationsSystemListDialog::OnVScrollUp( SI_MESSAGE* message )
{
	if (current_scroll_value_ > 0)
	{
		current_scroll_value_ -= 1;
		notifications_vscroll_->SetScrollPosWZ(current_scroll_value_);
		notifications_list_->SetBeginDrawIndex(current_scroll_value_);
		return true;
	}

	return false;
}

bool uiNotificationsSystemListDialog::OnVScrollDown( SI_MESSAGE* message )
{
	if (current_scroll_value_ < max_scroll_value_)
	{
		current_scroll_value_ += 1;
		notifications_vscroll_->SetScrollPosWZ(current_scroll_value_);
		notifications_list_->SetBeginDrawIndex(current_scroll_value_);
		return true;
	}

	return false;
}

bool uiNotificationsSystemListDialog::OnVScrollThumb( SI_MESSAGE* message )
{
	if (VScrollDialog::OnVScrollThumb(message) == true)
	{
		notifications_list_->SetBeginDrawIndex(notifications_vscroll_->GetScrollPosWZ());
		return true;
	}
	return false;
}

CCtrlVScrollWZ* uiNotificationsSystemListDialog::vscroll_control()
{
	return GetVScrollControlWZ(GetControlID_byPosition(DLGPOS_NOTIFICATIONS_VSCROLL));
}

bool uiNotificationsSystemListDialog::GetScrollArea( RECT& area )
{
	if (IsOverControl(GetControlWZ_byPosition(DLGPOS_NOTIFICATIONS_LIST)) == true)
	{
		area = GetControlWZ_byPosition(DLGPOS_NOTIFICATIONS_LIST)->GetSizeRect();
		return true;
	}
	return false;
}

#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
