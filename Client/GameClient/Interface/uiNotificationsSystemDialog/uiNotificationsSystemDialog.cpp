#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008283_20150511_NOTIFICATIONS_SYSTEM
#include "uiNotificationsSystemDialog.h"
#include "uiNotificationsSystemMan/uiNotificationsSystemMan.h"
#include "InterfaceManager.h"
#include "Application.h"

WzID uiNotificationsSystemDialog::wz_id_[DIALOG_MAX] = 
{
	StrToWzID("L001"), 
	StrToWzID("L000"), 
	StrToWzID("S000"), 
	StrToWzID("B001"), 
};

uiNotificationsSystemDialog::uiNotificationsSystemDialog(InterfaceManager* ui_manager) : 
uiBase(ui_manager), 
is_show_(FALSE)
{

}

uiNotificationsSystemDialog::~uiNotificationsSystemDialog(void)
{
}

void uiNotificationsSystemDialog::Init( CDrawBase* draw_base )
{
	uiBase::Init(draw_base);

	notifications_text_control_ = static_cast<CCtrlListWZ*>(GetControlWZ(wz_id_[DLGPOS_NOTIFICATIONS_TEXT]));
	notifications_special_text_control_ = static_cast<CCtrlListWZ*>(GetControlWZ(wz_id_[DLGPOS_NOTIFICATIONS_SPECIAL_LIST]));

	notifications_special_text_control_->ShowWindowWZ(WZ_HIDE);

	eye_button_control_ = static_cast<CCtrlButtonWZ*>(GetControlWZ(wz_id_[DLGPOS_EYE_BTN]));

	notifications_text_rect_ = notifications_text_control_->GetSizeRect();
	const int width_offset = 7;
	const int right_width_offset = 3;
	notifications_text_rect_.left += width_offset;
	notifications_text_rect_.right -= right_width_offset;
	notifications_text_rect_.right = notifications_text_rect_.right + (notifications_text_rect_.right - notifications_text_rect_.left);

	notifications_system_manager_ = static_cast<uiNotificationsSystemMan*>(GetMan());

	move_offset_ = 0;
	second_move_offset_ = 0;
	is_second_move_ = false;
}

void uiNotificationsSystemDialog::Release()
{
}

void uiNotificationsSystemDialog::OnShowWindow( BOOL is_show )
{
	if ( is_show )
	{
		move_offset_ = 0;
		second_move_offset_ = 0;
		is_second_move_ = false;
		is_show_ = TRUE;
		MoveDefaultPosition();
	}
	else
	{
		move_offset_ = 0;
		second_move_offset_ = 0;
		is_second_move_ = false;
		is_show_ = FALSE;
	}
}

void uiNotificationsSystemDialog::OnUpdateSolarDialog()
{
	if (FALSE == is_show_)
		return;

	//DWORD tick = clock_function::GetTickCount();
	static DWORD prev_tick = clock_function::GetTickCount();
	DWORD cur_tick = clock_function::GetTickCount();

	DWORD tick = cur_tick - prev_tick;

	if (tick < 3)
	{
		return;
	}

	prev_tick = cur_tick;

	notifications_system_manager_->CheckNextNotificationsShow();

	std::vector<HANDLE>& temp_text_texture_vector = notifications_system_manager_->get_text_texture_vector();

	if ( temp_text_texture_vector.empty() == true )
	{
		return;
	}

	static bool is_change = false;

	if ( notifications_text_rect_.left > notifications_text_control_->GetSizeRect().left )
	{
		notifications_text_rect_.left -= 1;
		
		if ( notifications_text_rect_.left <= notifications_text_control_->GetSizeRect().left )
		{
			notifications_text_rect_.left = notifications_text_control_->GetSizeRect().left;
			is_change = true;
		}
	}

	if ( notifications_text_rect_.left <= notifications_text_control_->GetSizeRect().left )
	{
		if ( is_change == false )
		{
			++move_offset_;
		}
		else
		{
			is_change = false;
		}
	}

	if ( is_second_move_ == true )
	{
		++second_move_offset_;
	}
}

void uiNotificationsSystemDialog::OnRenderSolarDialog()
{
	std::vector<HANDLE>& temp_text_texture_vector = notifications_system_manager_->get_text_texture_vector();

	if ( temp_text_texture_vector.empty() == true )
	{
		return;
	}

	if ( temp_text_texture_vector[0] != INVALID_HANDLE_VALUE )
	{
		St_TextureContainer* TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(temp_text_texture_vector[0]);

		if ( TexInfo )
		{
			// 세로 가운데 정렬.
			int height_differ = notifications_text_rect_.bottom - notifications_text_rect_.top;
			int image_height_half = TexInfo->m_nImageHeight / 2;
			int result_height = notifications_text_rect_.top + (height_differ / 2) - (image_height_half);

			if ( notifications_text_rect_.left > notifications_text_control_->GetSizeRect().left /* + width_offset */)
			{
				int result_width = notifications_text_rect_.right - notifications_text_rect_.left;
				if ( result_width > TexInfo->m_nImageWidth )
				{
					result_width = TexInfo->m_nImageWidth;
				}

				g_pSunRenderer->RenderTexture(temp_text_texture_vector[0],
					(float)notifications_text_rect_.left,
					(float)result_height,
					(float)result_width, 
					(float)TexInfo->m_nImageHeight,
					(float)0,
					(float)0,
					(float)result_width, 
					(float)TexInfo->m_nImageHeight,
					NULL);
			}
			else
			{
				int result_width = notifications_text_rect_.right - notifications_text_rect_.left;
				if ( result_width > TexInfo->m_nImageWidth - move_offset_ )
				{
					result_width = TexInfo->m_nImageWidth - (int)move_offset_;
				}
				g_pSunRenderer->RenderTexture(temp_text_texture_vector[0],
					(float)notifications_text_rect_.left,
					(float)result_height,
					(float)result_width,
					(float)TexInfo->m_nImageHeight,
					(float)move_offset_, 
					(float)0,
					(float)result_width,
					(float)TexInfo->m_nImageHeight,
					NULL);

				if ( temp_text_texture_vector.size() == 1 && 
					 notifications_text_rect_.left + result_width < notifications_text_control_->GetSize().left )
				{
					notifications_system_manager_->ClearTextTextureVector();
					notifications_system_manager_->DeleteNotificationsListBegin();
					ResetUIType();
				}

				if ( temp_text_texture_vector.size() > 1 )
				{
					for (unsigned int i = 1; i < temp_text_texture_vector.size(); ++i)
					{
						int second_width = 0;

						TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(temp_text_texture_vector[i]);

						second_width = TexInfo->m_nImageWidth;
						
						if (notifications_text_rect_.right < notifications_text_rect_.left + result_width + second_width)
						{
							second_width = notifications_text_rect_.right - (notifications_text_rect_.left + result_width);
						}

						int rect_left = notifications_text_rect_.left + result_width;

						if ( rect_left > notifications_text_control_->GetSize().left )
						{
							g_pSunRenderer->RenderTexture(temp_text_texture_vector[i],
								(float)notifications_text_rect_.left + result_width,
								(float)result_height,
								(float)second_width, 
								(float)TexInfo->m_nImageHeight,
								(float)0,
								(float)0,
								(float)second_width, 
								(float)TexInfo->m_nImageHeight,
								NULL);
						}
						else
						{
							is_second_move_ = true;
							rect_left = notifications_text_control_->GetSize().left;
							second_width = second_width - (int)second_move_offset_;
							
							g_pSunRenderer->RenderTexture(temp_text_texture_vector[i], 
								(float)rect_left,  
								(float)result_height, 
								(float)second_width,  
								(float)TexInfo->m_nImageHeight, 
								(float)second_move_offset_, 
								0, 
								(float)second_width,    
								(float)TexInfo->m_nImageHeight, 
								NULL);

							if ( rect_left + second_width < notifications_text_control_->GetSize().left )
							{
								notifications_system_manager_->ClearTextTextureVector();
								notifications_system_manager_->DeleteNotificationsListBegin();
								ResetUIType();
							}
						}
					}
				}
			}
		}
	}
}

void uiNotificationsSystemDialog::MessageProc( SI_MESSAGE* message )
{
	switch ( message->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch ( getCtrlIDToPos(message->dwCtrlID) )
			{
			case DLGPOS_EYE_BTN:
				{
					notifications_system_manager_->ToggleNotificationsList();
				}
				break;
			}
		}
		break;
	default:
		{
			SolarDialog::MessageProc( message );
		}
	}
}

void uiNotificationsSystemDialog::ResetNotificationsTextRect()
{
	notifications_text_rect_ = notifications_text_control_->GetSizeRect();
	const int width_offset = 7;
	const int right_width_offset = 3;
	notifications_text_rect_.left += width_offset;
	notifications_text_rect_.right -= right_width_offset;
	notifications_text_rect_.left = notifications_text_rect_.right;
}

void uiNotificationsSystemDialog::MoveDefaultPosition()
{
	POINT pt = g_pApplication->Get2DModeMapping();
	RECT rcClient;
	GetDialogWZ()->GetClientRect(&rcClient);

	POINT MyInitialPoint;
	MyInitialPoint.x = pt.x - (rcClient.right- rcClient.left);//-386;
	MyInitialPoint.y =  pt.y - (rcClient.bottom - rcClient.top + 100) - 4;
	this->MoveWindowWZ((float)MyInitialPoint.x, (float)MyInitialPoint.y);
}

POSTYPE uiNotificationsSystemDialog::getCtrlIDToPos( WzID wzId )
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

void uiNotificationsSystemDialog::MoveDefaultWindowPos()
{
	SolarDialog::MoveDefaultWindowPos();

	MoveDefaultPosition();
}

void uiNotificationsSystemDialog::ResetNotificationsTime()
{
	move_offset_ = 0;

	second_move_offset_ = 0;

	is_second_move_ = false;
}

void uiNotificationsSystemDialog::ResetUIType()
{
	if ( notifications_special_text_control_ )
	{
		notifications_special_text_control_->ShowWindowWZ(WZ_HIDE);
	}
}

void uiNotificationsSystemDialog::SpecialTextUIShow()
{
	if ( notifications_special_text_control_ )
	{
		notifications_special_text_control_->ShowWindowWZ(WZ_SHOW);
	}
}

#endif // _NA_008283_20150511_NOTIFICATIONS_SYSTEM
