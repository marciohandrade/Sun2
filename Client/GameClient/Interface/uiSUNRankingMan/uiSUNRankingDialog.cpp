#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingDialog.h"
#include "uiSUNRankingMan.h"
#include "GlobalFunc.h"
#include "InterfaceManager.h"
#include "SUNRankingSlot.h"
#include "TextureListInfoHandleManager.h"
#include "SUNRankingControl/SUNRankingCharacterNameControl.h"

WzID uiSUNRankingDialog::wz_id_[kControl_Size] = 
{
	StrToWzID("C000"),
	StrToWzID("C001"),
	StrToWzID("C002"),
	StrToWzID("C003"),
	StrToWzID("C004"),
	StrToWzID("C005"),
	StrToWzID("C006"),

	StrToWzID("B000"),
	StrToWzID("B005"),
	StrToWzID("B002"),
	StrToWzID("B001"),

	StrToWzID("B012"),

	StrToWzID("B004"),
	StrToWzID("B003"),
	StrToWzID("L001"),
	StrToWzID("P003"),

	StrToWzID("L000"),
	StrToWzID("P008"),

	StrToWzID("V000"),

	StrToWzID("P006"),
	StrToWzID("P005"),
	StrToWzID("P010"),
	StrToWzID("P012"),
	StrToWzID("P013"),
	StrToWzID("P014"),
	StrToWzID("P015"),
	StrToWzID("P016"),
	StrToWzID("P017"),

	StrToWzID("L002"),

	StrToWzID("B007"),
};

uiSUNRankingDialog::uiSUNRankingDialog(InterfaceManager* ui_manager) : 
uiBase( ui_manager ),
ui_sun_ranking_manager_( NULL ),
vscroll_control_( NULL ),
move_pivot_(0),
sun_ranking_mode_(SUN_RANKING_BEST_OF_BEST),
wait_texture_(INVALID_HANDLE_VALUE),
is_init_(false)
{

}

uiSUNRankingDialog::~uiSUNRankingDialog(void)
{
}

void uiSUNRankingDialog::Init( CDrawBase* draw_base )
{
	uiBase::Init(draw_base);

	RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
	RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
	RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
	RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
	RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
	RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
	RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
	RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
	RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
	RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
	RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
	RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
	RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
	RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
	RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
	RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
	RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
	RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
	RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
	RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
	RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
	RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
	RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
	RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
	RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);
	RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);
	RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
	RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);
	RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
	RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);

	ui_sun_ranking_manager_ = GET_DIALOG_MANAGER(uiSUNRankingMan, UIMAN_SUNRANKING);
	if ( wait_texture_ == INVALID_HANDLE_VALUE )
	{
		wait_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Loading.tga",
			TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
	}
	// 애니메이션 텍스쳐 위치 (12프레임) 프레임당 (40 * 40) 4 x 3 개
	int index = 0;
	float width = 0.0f;
	float height = 0.0f;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			width = j * ANIMATION_FRAME_WIDTH;
			height = i * ANIMATION_FRAME_HEIGHT;

			texture_rectangles_[index].left = width;
			texture_rectangles_[index].top = height;
			texture_rectangles_[index].right = ANIMATION_FRAME_WIDTH;
			texture_rectangles_[index].bottom = ANIMATION_FRAME_HEIGHT;

			++index;
			if (index >= ANIMATION_FRAME)
			{
				break;
			}
		}
	}

	RankingArticleInit(); // 탭 버튼 초기화

	DropdownClassFilterInit(); // 드롭다운 초기화

	// 순위, 캐릭터명, 점수 메시지 무시
	for ( int i = kControl_Button_B000; i <= kControl_Button_B001; ++i )
	{
		CControlWZ* control_ptr = GetControlWZ_byPosition(i);
		if ( control_ptr )
		{
			control_ptr->SetDiscardMessage( true );
		}
	}

	// 기본 슬롯 보이지 않음
	for ( int i = kControl_Picture_P006; i <= kControl_Picture_P017; ++i )
	{
		CControlWZ* control_ptr = GetControlWZ_byPosition(i);
		if ( control_ptr )
		{
			control_ptr->ShowWindowWZ(WZ_HIDE);
		}
	}

	TextureListInfoHandleManager::Instance()->LoadIcon(my_ranking_background_image_index);
	TextureListInfoHandleManager::Instance()->LoadIcon(ranking_background_image_index);
	for ( BYTE i = eCHAR_BERSERKER; i < eCHAR_TYPE_MAX; ++i )
	{
		TextureListInfoHandleManager::Instance()->LoadIcon(class_texture_index[i]);
	}

	vscroll_control_ = 
		static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_VScroll_V000)));
	if ( vscroll_control_ )
	{
		vscroll_control_->ShowWindowWZ(WZ_SHOW);
		VScrollDialog::ResizeingVScrollControl(0, 0);
	}

	RECT rc = {0, };
	description_ftext_render_.Init(g_pSunRenderer, rc);
	set_sun_ranking_mode_( sun_ranking_mode_ + dropdown_class_filter_.CurrentUserData() );

	sun_ranking_mouse_wheel_.ResetMouseWheel();

	InitTimer();
}

void uiSUNRankingDialog::Release()
{
	get_dropdown_class_filter_().Release();
	SUNRankingSlotListIter list_iter;
	for ( int i = SUN_RANKING_BEST_OF_BEST; i < SUN_RANKING_MAX; ++i )
	{
		for ( list_iter = sun_ranking_slot_view_list_[i].begin(); 
			list_iter != sun_ranking_slot_view_list_[i].end(); )
		{
			delete (*list_iter);
			(*list_iter) = NULL;
			list_iter = sun_ranking_slot_view_list_[i].erase(list_iter);
		}
		sun_ranking_slot_view_list_[i].clear();
	}

	if (wait_texture_ != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(wait_texture_);
		wait_texture_ = INVALID_HANDLE_VALUE;
	}
	set_sun_ranking_mode_(SUN_RANKING_BEST_OF_BEST);
	InitTimer();
}

void uiSUNRankingDialog::NetworkProc( MSG_BASE* recv_packet )
{

}

void uiSUNRankingDialog::Process( DWORD tick )
{
	for ( int i = SUN_RANKING_BEST_OF_BEST; i < SUN_RANKING_MAX; ++i )
	{
		if ( current_time_[i] > tick )
			current_time_[i] -= tick;
		else
			current_time_[i] = 0;
	}
}

void uiSUNRankingDialog::RenderTimer( DWORD time )
{
	DWORD min_tick  = 0;
	DWORD sec_tick  = 0;

	if ( time )
	{
		DWORD time_temp1 = time / SEC_PER_TICK;

		if ( time_temp1 )
		{
			min_tick = time_temp1 / 60;
			sec_tick = time_temp1 % 60;
		}
	}

	TCHAR result_string[INTERFACE_STRING_LENGTH] = {0, };
	TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
	// 3619 : 갱신까지 남은시간: %s
	g_InterfaceManager.GetInterfaceString(3619, format_string);

	TCHAR time_string[16] = {0, };
	Snprintf(time_string, 16-1, _T("%02d\'%02d"), min_tick, sec_tick);
	Snprintf(result_string, INTERFACE_STRING_LENGTH-1, format_string, time_string);

	COLORREF font_color = WzColor_RGBA(255, 255, 0, 255);
	if ( 10 <= min_tick )
	{
		font_color = WzColor_RGBA(255, 0, 0, 255);
	}

	RECT dialog_rect; 
	this->GetDialogWZ()->GetClientRect(&dialog_rect);

	dialog_rect.left   += 18;
	dialog_rect.top    += 15;
	dialog_rect.right  = dialog_rect.left + 150;
	dialog_rect.bottom = dialog_rect.top + 12;
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont(StrToWzID("st11"));
	{
		g_pSunRenderer->x_pManagerTextOut->DrawText(result_string,
			&dialog_rect,
			font_color,
			WzColor_RGBA(0, 0, 0, 0),
			TP_HLEFT | TP_VCENTER,
			ETS_OUTLINE);
	}
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

void uiSUNRankingDialog::RenderSettleTime()
{
	if ( settle_date_.is_initialize == false )
	{
		return;
	}

	if ( settle_date_.is_convert == false )
	{
		return;
	}

	COLORREF font_color = WzColor_RGBA(255, 255, 0, 255);

	RECT dialog_rect;
	this->GetDialogWZ()->GetClientRect(&dialog_rect);
	
	g_pSunRenderer->x_pManagerTextOut->StartToUseFont(StrToWzID("st11"));
	{
		SIZE FontSize;
		g_pSunRenderer->x_pManagerTextOut->GetTextExtentPoint( 
			settle_date_.result_string, 
			_tcslen(settle_date_.result_string),
			&FontSize);

		const int left_margin = 40;
		const int top_margin = 30;
		dialog_rect.left = dialog_rect.right - (FontSize.cx + left_margin);
		dialog_rect.top  = dialog_rect.bottom - (FontSize.cy + top_margin);

		g_pSunRenderer->x_pManagerTextOut->DrawText(settle_date_.result_string,
			&dialog_rect,
			font_color,
			WzColor_RGBA(0, 0, 0, 0),
			TP_HLEFT | TP_VCENTER,
			ETS_OUTLINE);
	}
	g_pSunRenderer->x_pManagerTextOut->EndUseFont();
}

void uiSUNRankingDialog::OnUpdateSolarDialog()
{
	dropdown_class_filter_.Update();
	int mode = get_sun_ranking_mode_();
	if (isSunRankingModeValidCheck(mode) == false)
	{
		return;
	}

	UpdateMouseWheel();
	
	for ( SUNRankingSlotListIter iter = sun_ranking_slot_view_list_[mode].begin(); 
		  iter != sun_ranking_slot_view_list_[mode].end(); 
		  ++iter )
	{
		(*iter)->Update();
	}
	if ( SUNRankingSlot::get_is_message() == true )
	{
		SUNRankingSlot::set_is_message_(false);
	}

	CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_List_L002);
	if ( control_ptr )
	{
		description_ftext_render_.SetRect(control_ptr->GetSizeRect());
	}
}

void uiSUNRankingDialog::OnRenderSolarDialog()
{
	dropdown_class_filter_.OnDraw();

	int mode = get_sun_ranking_mode_();
	if (mode < 0 || mode >= SUN_RANKING_MAX)
	{
		return;
	}
	for ( SUNRankingSlotListIter iter = sun_ranking_slot_view_list_[mode].begin(); 
		iter != sun_ranking_slot_view_list_[mode].end(); 
		++iter )
	{
		(*iter)->Render();
	}

	SUNRankingSlotListIter begin_iter = sun_ranking_slot_view_list_[mode].begin();
	if ( begin_iter != sun_ranking_slot_view_list_[mode].end() )
	{
		(*begin_iter)->Render();
	}
	WaitPlayAnimation();

	description_ftext_render_.Render();

	RenderTimer( current_time_[get_sun_ranking_mode_()] );

#ifdef _NA_008016_20150331_SUN_RANKING_SYSTEM_SETTLE_DATE_VIEW
	RenderSettleTime();
#endif // _NA_008016_20150331_SUN_RANKING_SYSTEM_SETTLE_DATE_VIEW
}

void uiSUNRankingDialog::OnShowWindow( BOOL is_show )
{
	if ( is_show )
	{
		sun_ranking_mouse_wheel_.ResetMouseWheel();

		wait_animation_timer_.Off();
		wait_animation_timer_.On(1);

		ui_sun_ranking_manager_->set_Is_wait_receive_packet_(false);

		// 타이머 필요.
		if ( current_time_[get_sun_ranking_mode_()] == 0 )
		{
			ui_sun_ranking_manager_->Send_CG_ETC_SUN_RANKING_SYN(get_sun_ranking_mode_());
		}

#ifdef _NA_008016_20150331_SUN_RANKING_SYSTEM_SETTLE_DATE_VIEW
		if ( settle_date_.is_initialize == false )
		{
			ui_sun_ranking_manager_->Send_CG_ETC_SUN_RANKING_SETTLE_INFO_SYN();
		}
#endif // _NA_008016_20150331_SUN_RANKING_SYSTEM_SETTLE_DATE_VIEW

		keyMsg msg;
		ZeroMemory(&msg, sizeof(msg));
		msg.dwType = eDoType_SUNRanking_Dialog;
		msg.wParam = IM_SUNRANKING_MANAGER::SUNRANKING_DIALOG;
		msg.DoSomething = GlobalFunc::DoShowWindowEx;
		g_KeyQueueManager.PushBack(msg);
	}
	else
	{
		g_KeyQueueManager.DeleteMsg(eDoType_SUNRanking_Dialog);
	}
}

void uiSUNRankingDialog::InitTimer()
{
	for ( int i = SUN_RANKING_BEST_OF_BEST; i < SUN_RANKING_MAX; ++i )
	{
		current_time_[i] = 0;
	}
}

SUNRankingMouseWheel::SUNRankingMouseWheel() : 
init_time_for_accel_(0.0f),
is_accel_(false),
mouse_wheel_state_(MOUSEWHEEL_NONE)
{

}

SUNRankingMouseWheel::~SUNRankingMouseWheel()
{

}

bool SUNRankingMouseWheel::AccelMove(MOUSEWHEEL_STATE state, AMOVEVAL& val, const float& delta_time )
{
	static float meter_unit = 0.05f;
	static float second_unit = 1.0f;
	static float vello_unit  = meter_unit / second_unit;
	static float accel_unit  = meter_unit / (second_unit * second_unit);

	if ( mouse_wheel_state_ != state && mouse_wheel_state_ != MOUSEWHEEL_NONE && state != MOUSEWHEEL_MAX )
	{
		is_accel_ = true;
		init_time_for_accel_ = 0.0f;
		mouse_wheel_state_ = MOUSEWHEEL_NONE;
		return false;
	}

	if ( state == MOUSEWHEEL_MAX )
	{
		init_time_for_accel_ = 0.0f;
	}
	else
	{
		mouse_wheel_state_ = state;
	}
	
	if ( is_accel_ == false )
	{
		is_accel_ = true;
		init_time_for_accel_ = 0.0f;
	}

	init_time_for_accel_ += delta_time * second_unit;

	float accel = val.accel_;

	if ( val.speed_ > 0 )
		accel = -val.accel_;

	// v = v0 + at ( 근데 가속도가 음수니깐 마이너스 붙임 )
	float speed = val.speed_ * vello_unit + accel * accel_unit * init_time_for_accel_;

	if ( val.speed_ > 0 )
	{
		if ( speed < 0.0f )
		{
			is_accel_ = false;
			mouse_wheel_state_ = MOUSEWHEEL_NONE;
			return false;                  // 효과 끝나면 false 리턴
		}
	}

	switch ( state )
	{
	case MOUSEWHEEL_DOWN:
		{
			val.y_ += /*방향*/(int)(2.0f * speed);
		}
		break;
	case MOUSEWHEEL_UP:
		{
			val.y_ -= /*방향*/(int)(2.0f * speed);
		}
		break;
	}

	return true;
}

void SUNRankingMouseWheel::ResetMouseWheel()
{
	init_time_for_accel_;
	is_accel_ = false;
	mouse_wheel_state_ = MOUSEWHEEL_NONE;
}

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM
