#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingDialog.h"
#include "SUNRankingSlot.h"

void uiSUNRankingDialog::MessageProc( SI_MESSAGE* message )
{
	if ( IsLock() )
		return;

	if ( GetMan() == NULL )
		return;

	bool is_change = false;
	if ( dropdown_class_filter_.MessageProc( message, OUT is_change ) )
	{
		if ( is_change )
		{
			dropdown_class_filter_.CurrentUserData();
			int mode = get_sun_ranking_mode_();
			if ( mode % 2 == 1 )
			{
				set_sun_ranking_mode_(mode-1 + dropdown_class_filter_.CurrentUserData());
			}
			else
			{
				set_sun_ranking_mode_(mode + dropdown_class_filter_.CurrentUserData());
			}
			// 타이머 필요.
			if ( current_time_[get_sun_ranking_mode_()] == 0 )
			{
				ui_sun_ranking_manager_->Send_CG_ETC_SUN_RANKING_SYN(get_sun_ranking_mode_());
			}
		}
		return;
	}

	switch ( message->eResultMsg )
	{
	case RT_MSG_LBUTTONCLICK:
		{
			OnLButtonClick( message );
		}
		break;
	case RT_MSG_MOUSEOVER:
		{
			OnMouseOver( message );
		}
		break;
	case RT_MSG_SB_PAGETOP: // 윗쪽 몸통을 눌렀다.
	case RT_MSG_SB_LINETOP: // 윗쪽 화살표를 눌렀다.
		{
			OnVScrollUp( message );
		}
		break;
	case RT_MSG_SB_LINEBOTTOM:
	case RT_MSG_SB_PAGEBOTTOM:
		{
			OnVScrollDown( message );
		}
		break;
	case RT_MSG_SB_THUMBTRACK:    // 썸을 드래그 하고 있다.
	case RT_MSG_SB_THUMBPOSITION: // 썸을 드래그 한 후 마우스를 놓았다.
	case RT_MSG_SB_ENDSCROLL:     // 스크롤이 종료 되었다.
		{
			OnVScrollThumb( message );
		}
		break;
	default:
		SolarDialog::MessageProc(message);
		break;
	}
}

bool uiSUNRankingDialog::OnLButtonClick( SI_MESSAGE* message )
{
	DWORD control_position = GetControlPosition_byID( message->dwCtrlID );
	switch ( control_position )
	{
	case kControl_CheckBT_C000:
	case kControl_CheckBT_C001:
	case kControl_CheckBT_C002:
	case kControl_CheckBT_C003:
	case kControl_CheckBT_C004:
	case kControl_CheckBT_C005:
	case kControl_CheckBT_C006:
		{
			if ( ui_sun_ranking_manager_->get_Is_wait_receive_packet_() == true )
			{
				return true;
			}
			RankingArticleButtonClick( control_position );
		}
		break;
	case kControl_Button_B012:
		{
			ShowInterface( FALSE );
		}
		break;
	case kControl_Button_B003:
		{

		}
		break;
	case kControl_Button_B007:
		{
			OpenTutorialGuid();
		}
		break;
	}

	return true;
}

void uiSUNRankingDialog::RankingArticleButtonClick( const DWORD& position )
{
	CCtrlButtonCheckWZ* check_button = NULL;
	for ( int i = kControl_CheckBT_C000; i <= kControl_CheckBT_C006; ++i )
	{
		check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
		if ( check_button == NULL )
		{
			continue;
		}

		check_button->SetCheckState(false);

		if ( position == i )
		{			
			check_button->SetCheckState( true );
			int mode = get_sun_ranking_mode_();
			// 기존의 모드와 선택된 모드가 다를 경우.
			if ( mode != (i * MODE_MAX) + dropdown_class_filter_.CurrentUserData() )
			{
				set_sun_ranking_mode_((i * MODE_MAX) + dropdown_class_filter_.CurrentUserData());
				// 타이머 필요.
				if ( current_time_[get_sun_ranking_mode_()] == 0 )
				{
					ui_sun_ranking_manager_->Send_CG_ETC_SUN_RANKING_SYN(get_sun_ranking_mode_());
				}
			}
		}
	}
}

bool uiSUNRankingDialog::OnMouseOver( SI_MESSAGE* message )
{
	return true;
}

void uiSUNRankingDialog::UpdateMouseWheel()
{
	CControlWZ* picture_control = GetControlWZ_byPosition(kControl_Picture_P008);
	SUNRankingMouseWheel::MOUSEWHEEL_STATE state = sun_ranking_mouse_wheel_.get_mouse_wheel_state_();
	SUNRankingMouseWheel::MOUSEWHEEL_STATE local_state = state;
	bool is_differ_state = false;

	if ( picture_control )
	{
		RECT area = picture_control->GetSizeRect();
		BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);

		switch ( wheel_msg )
		{
		case MOUSE_WHEEL_UP:
			local_state = SUNRankingMouseWheel::MOUSEWHEEL_UP;
			is_differ_state = true;
			break;
		case MOUSE_WHEEL_DOWN:
			local_state = SUNRankingMouseWheel::MOUSEWHEEL_DOWN;
			is_differ_state = true;
			break;
		}
	}

	DWORD current_tick = g_pSunRenderer->x_Clock.GetCurrentTick();
	static DWORD last_time = current_tick;
	static float s_rate = 0.003f;
	float delta_tick = (float)(current_tick - last_time) * 0.003f;
	last_time = current_tick;
	// delta_tick 넣어 주면 된다.

	int range_min = 0;
	int range_max = 0;
	int current_position = vscroll_control_->GetScrollPosWZ();
	vscroll_control_->GetScrollRangeWZ(&range_min, &range_max);

	static short wheel = 2;
	static float fMoveSpeed = 180.0f;
	static float accel = 80.0f;
	
	moveval_.accel_ = accel;
	moveval_.speed_ = wheel * fMoveSpeed * 0.25f;
	moveval_.y_ = current_position; 

	bool is_change = false;
	if ( is_differ_state == true && state == local_state )
	{
		is_change = sun_ranking_mouse_wheel_.AccelMove(SUNRankingMouseWheel::MOUSEWHEEL_MAX, moveval_, delta_tick);
	}
	else
	{
		is_change = sun_ranking_mouse_wheel_.AccelMove(local_state, moveval_, delta_tick);
	}
	
	if ( is_change == true )
	{
		current_position = moveval_.y_;

		current_position = max(current_position, 0);
		current_position = min(current_position, range_max);
		vscroll_control_->SetScrollPosWZ(current_position);
		SUNRankingSlot::set_is_message_(true);
	}
}

bool uiSUNRankingDialog::OnVScrollUp( SI_MESSAGE* message )
{
	if ( vscroll_control_ )
	{
		int range_min = 0;
		int range_max = 0;
		int current_position = vscroll_control_->GetScrollPosWZ();
		vscroll_control_->GetScrollRangeWZ(&range_min, &range_max);
		current_position -= kdivide_slot;
		current_position = max(current_position, 0);
		current_position = min(current_position, range_max);
		vscroll_control_->SetScrollPosWZ(current_position);
		SUNRankingSlot::set_is_message_(true);
	}

	return true;
}

bool uiSUNRankingDialog::OnVScrollDown( SI_MESSAGE* message )
{
	if ( vscroll_control_ )
	{
		int range_min = 0;
		int range_max = 0;
		int current_position = vscroll_control_->GetScrollPosWZ();
		vscroll_control_->GetScrollRangeWZ(&range_min, &range_max);
		current_position += kdivide_slot;
		current_position = max(current_position, 0);
		current_position = min(current_position, range_max);
		vscroll_control_->SetScrollPosWZ(current_position);
		SUNRankingSlot::set_is_message_(true);
	}

	return true;
}

bool uiSUNRankingDialog::OnVScrollThumb( SI_MESSAGE* message )
{
	SUNRankingSlot::set_is_message_(true);

	return true;
}
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM