#include "SunClientPrecompiledHeader.h"
#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM
#include "uiSUNRankingDialog.h"
#include "SUNRankingSlot.h"
#include "InterfaceManager.h"
#include "Hero.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "ExpValueInfoParser.h"
#include "GlobalFunc.h"

void uiSUNRankingDialog::RankingArticleInit()
{
	CCtrlButtonCheckWZ* check_button = NULL;
	for ( int i = kControl_CheckBT_C001; i <= kControl_CheckBT_C006; ++i )
	{
		check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(i));
		if ( check_button )
		{
			check_button->SetCheckState(false);
		}
	}

	// 첫 번째 항목을 체크 상태로 한다.
	check_button = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C000));
	if ( check_button )
	{
		check_button->SetCheckState( true );
	}
}

void uiSUNRankingDialog::DropdownClassFilterInit()
{
	dropdown_class_filter_.Init(GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B004)),
		GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B003)),
		GetListControlWZ(GetControlID_byPosition(kControl_List_L001)),
		NULL,
		NULL/*GetPictureControlWZ(GetControlID_byPosition(kControl_Picture_P003))*/);

	if ( dropdown_class_filter_.list_control() )
	{
		dropdown_class_filter_.list_control()->SetUnuseSelBar( true );
		dropdown_class_filter_.list_control()->DeleteItemAll();
		dropdown_class_filter_.list_control()->SetStartLineGapTop( 10 );
		dropdown_class_filter_.list_control()->SetSellHeight(16);
	}

	// 3600 : 개인랭킹
	// 3601 : 길드원랭킹
	dropdown_class_filter_.InsertItem( 3600, (void*)PRIVATE_MODE, StrToWzID("st10"), DT_VCENTER | DT_CENTER );
	dropdown_class_filter_.InsertItem( 3601, (void*)GUILD_MODE, StrToWzID("st10"), DT_VCENTER | DT_CENTER );
	dropdown_class_filter_.Select( 0 );
}

void uiSUNRankingDialog::ResizeVscroll( const LONG& scroll_count )
{
	// 여기서 VScroll max와 pos를 셋팅해준다.
	RECT dest_rect;
	CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Picture_P008);
	if ( control_ptr )
	{
		dest_rect = control_ptr->GetSizeRect();
	}

	RECT my_slot_rect = dest_rect;
	my_slot_rect.bottom = (dest_rect.bottom - dest_rect.top) / kranking_screen_view_max_slot_count;
	dest_rect.top = dest_rect.top - my_slot_rect.bottom;
	dest_rect.bottom = dest_rect.bottom - my_slot_rect.bottom;

	move_pivot_ = my_slot_rect.bottom / kdivide_slot;
	int scroll_max = kdivide_slot * (scroll_count - kranking_screen_view_max_slot_count);
	if ( vscroll_control_ )
	{
		vscroll_control_->SetScrollRangeWZ(0, scroll_max);
		vscroll_control_->SetScrollPosWZ(0);
		if ( scroll_max <= 0 )
		{
			vscroll_control_->SetChangeSizeThumb(0);
		}
		else
		{
			float thumb_size = (kranking_screen_view_max_slot_count / (float)scroll_count) * 100;
			vscroll_control_->SetChangeSizeThumb((int)thumb_size);
		}
	}

	SUNRankingSlot::set_move_pivot_(move_pivot_);
}

void uiSUNRankingDialog::set_sun_ranking_mode_( const int& sun_ranking_mode )
{
	if ( sun_ranking_mode < 0 || sun_ranking_mode >= SUN_RANKING_MAX )
	{
		return;
	}

	sun_ranking_mode_ = sun_ranking_mode;

	int string_index = 0;
	// 텍스트
	switch ( sun_ranking_mode_ )
	{
	case SUN_RANKING_BEST_OF_BEST + PRIVATE_MODE:
	case SUN_RANKING_BEST_OF_BEST + GUILD_MODE:
		{
			// 3610 : <font=mn12><color=ffebe0c8>진정한 랭커, 다방면에 출중한 원정대 최고의 전사는 누구인가?</font></color>
			string_index = 3610;
		}
		break;
	case SUN_RANKING_ELETE_WARRIOR + PRIVATE_MODE:
	case SUN_RANKING_ELETE_WARRIOR + GUILD_MODE:
		{
			// 3611 : <font=mn12><color=ffebe0c8>점령전과 공성전에서 가장 많은 킬을 기록한 PVP의 최강자!</font></color>
			string_index = 3611;
		}
		break;
	case SUN_RANKING_LEVEL_KING + PRIVATE_MODE:
	case SUN_RANKING_LEVEL_KING + GUILD_MODE:
		{
			// 3612 : <font=mn12><color=ffebe0c8>열정과 끈기의 상징, SUN 최고 레벨은 누구인가</font></color>
			string_index = 3612;
		}
		break;
	case SUN_RANKING_LEGEND_EQUIP + PRIVATE_MODE:
	case SUN_RANKING_LEGEND_EQUIP + GUILD_MODE:
		{
			// 3613 : <font=mn12><color=ffebe0c8>수많은 전투를 승리로 이끈 전설적인 장비</font></color>
			string_index = 3613;
		}
		break;
	case SUN_RANKING_STRONG_ONE_SHOT + PRIVATE_MODE:
	case SUN_RANKING_STRONG_ONE_SHOT + GUILD_MODE:
		{
			// 3614 : <font=mn12><color=ffebe0c8>배틀존에서 가장 강력한 최고데지미량을 기록한 치명적인 딜러!</font></color>
			string_index = 3614;
		}
		break;
	case SUN_RANKING_BEST_ATTACKKER + PRIVATE_MODE:
	case SUN_RANKING_BEST_ATTACKKER + GUILD_MODE:
		{
			// 3615 : <font=mn12><color=ffebe0c8>배틀존에서 가장 높은 누적 데지미량을 기록한 최고의 공격수!</font></color>
			string_index = 3615;
		}
		break;
	case SUN_RANKING_MASTER_OF_HEAL + PRIVATE_MODE:
	case SUN_RANKING_MASTER_OF_HEAL + GUILD_MODE:
		{
			// 3616 : <font=mn12><color=ffebe0c8>배틀존에서 가장 높은 치유량을 기록한 치유의 달인!</font></color>
			string_index = 3616;
		}
		break;
	}

	if ( string_index != 0 )
	{
		TCHAR format_string[INTERFACE_STRING_LENGTH] = {0, };
		g_InterfaceManager.GetInterfaceString(string_index, format_string);

		CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_List_L002);
		if ( control_ptr != NULL)
		{
			RECT control_rect = control_ptr->GetSizeRect();
			if (description_ftext_parser_.Parser(format_string, description_ftext_sentence_))
			{
				description_ftext_render_.ClearSentence();
				description_ftext_render_.Append(description_ftext_sentence_);
				description_ftext_sentence_.Release();
				description_ftext_render_.SetRect(control_rect);
			}
		}
	}

	ResizeVscroll( sun_ranking_slot_view_list_[get_sun_ranking_mode_()].size() );
}

bool uiSUNRankingDialog::isSunRankingModeValidCheck(const int& mode)
{
	if (mode < 0 || mode >= SUN_RANKING_MAX)
	{
		return false;
	}

	return true;
}

void uiSUNRankingDialog::WaitPlayAnimation()
{
	if (ui_sun_ranking_manager_->get_Is_wait_receive_packet_() == false)
	{
		return;
	}

	static FRECT img_rect = {0,};

	CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Picture_P008);
	if ( control_ptr == NULL )
	{
		return;
	}

	img_rect.left = static_cast<float>(control_ptr->GetSizeRect().left);
	img_rect.top = static_cast<float>(control_ptr->GetSizeRect().top);
	img_rect.right = static_cast<float>(control_ptr->GetSizeRect().right);
	img_rect.bottom = static_cast<float>(control_ptr->GetSizeRect().bottom);    

	img_rect.left = img_rect.left + ((img_rect.right - img_rect.left - ANIMATION_FRAME_WIDTH) / 2) + 0.5f;
	img_rect.top = img_rect.top + ((img_rect.bottom - img_rect.top - ANIMATION_FRAME_HEIGHT) / 2);

	int frame = 30;//초당 애니메이션 프래임
	int count = ((wait_animation_timer_.SpentTime() / frame) % ANIMATION_FRAME);

	g_pSunRenderer->RenderTexture(wait_texture_, img_rect.left, img_rect.top, 
		ANIMATION_FRAME_WIDTH, ANIMATION_FRAME_HEIGHT, 
		texture_rectangles_[count].left, texture_rectangles_[count].top, 
		texture_rectangles_[count].right, texture_rectangles_[count].bottom);
	++count;
}

void uiSUNRankingDialog::OpenTutorialGuid()
{
	uiGuideSystemMan* ui_guide_system_manager = GET_DIALOG_MANAGER(uiGuideSystemMan, UIMAN_GUIDE_SYSTEM);
	if ( ui_guide_system_manager )
	{
		ui_guide_system_manager->OpenGuideInfoDialog();
		ui_guide_system_manager->OpenGuideDialog(uiGuideSystemMan::kDialogType_Information, 6007);
	}
}

void uiSUNRankingDialog::UpdateSlotList( SunRankingInfoForClient* infos, const int& my_ranking, const SCORE& my_score )
{
	if ( infos == NULL )
	{
		return;
	}	

	int mode = get_sun_ranking_mode_();

	if ( isSunRankingModeValidCheck(mode) == false )
	{
		return;
	}

	SUNRankingSlotListIter list_iter;	
	for ( list_iter = sun_ranking_slot_view_list_[mode].begin(); 
		list_iter != sun_ranking_slot_view_list_[mode].end(); )
	{
		delete (*list_iter);
		(*list_iter) = NULL;
		list_iter = sun_ranking_slot_view_list_[mode].erase(list_iter);
	}
	sun_ranking_slot_view_list_[mode].clear();

	SUN_RANKING_SCORE_INFO my_ranking_score_info;
	CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Picture_P008);
	SUNRankingSlot* ranking_slot = new SUNRankingSlot;
	if ( mode == SUN_RANKING_LEVEL_KING || mode == SUN_RANKING_LEVEL_KING + 1 )
	{
		my_ranking_score_info.is_level_value_ = true;
		my_ranking_score_info.score_ = ExpValueInfoParser::Instance()->CalculateAccumExpToLevel(my_score);
		my_ranking_score_info.extra_score_info_ = CalculateExpToPercent((LEVELTYPE)my_ranking_score_info.score_, my_score);
	}
	else
	{
		my_ranking_score_info.score_ = my_score;
	}
	ranking_slot->Init(control_ptr, vscroll_control_, 0, g_pHero->GetClass(), my_ranking, g_pHero->GetName(), my_ranking_score_info);
	sun_ranking_slot_view_list_[mode].push_back(ranking_slot);

	int rank = 1;
	int real_rank = 1;
	int prev_rank = 1;
	for ( int i = 0; i < MSG_CG_ETC_SUN_RANKING_ACK::MAX_INFO_COUNT; ++i )
	{
		if ( infos[i].class_code == eCHAR_NONE )
		{
			continue;
		}

		real_rank = rank;
		if ( i != 0 )
		{
			if ( infos[i-1].score == infos[i].score )
			{
				real_rank = prev_rank;
			}
		}

		SUN_RANKING_SCORE_INFO ranking_score_info;
		if ( mode == SUN_RANKING_LEVEL_KING || mode == SUN_RANKING_LEVEL_KING + 1 )
		{
			ranking_score_info.is_level_value_ = true;
			ranking_score_info.score_ = ExpValueInfoParser::Instance()->CalculateAccumExpToLevel(infos[i].score);
			ranking_score_info.extra_score_info_ = CalculateExpToPercent((LEVELTYPE)ranking_score_info.score_, infos[i].score);
		}
		else
		{
			ranking_score_info.score_ = infos[i].score;
		}

		ranking_slot = new SUNRankingSlot;
		ranking_slot->Init(control_ptr, vscroll_control_, i+1, infos[i].class_code, real_rank, infos[i].char_name, ranking_score_info );
		sun_ranking_slot_view_list_[mode].push_back(ranking_slot);
		prev_rank = real_rank;
		++rank;
	}

#ifdef _DEV_VER
	current_time_[mode] = 10000;   // 개발 버전에서는 2초 셋팅
#else
	current_time_[mode] = 300000; // 5분 셋팅
#endif // _DEV_VER

	ResizeVscroll( sun_ranking_slot_view_list_[mode].size() );
}

float uiSUNRankingDialog::CalculateExpToPercent( const LEVELTYPE& level, const EXPTYPE& accum_exp )
{
	float fExpPercent=0.0f;

	EXPTYPE destexp = 0;
	EXPTYPE curaccumexp = 0;

	BASE_ACCUM_EXP_INFO* accum_exp_info = NULL;
	accum_exp_info = ExpValueInfoParser::Instance()->GetAccumExpInfo(level + 1);
	if ( accum_exp_info == NULL )
	{
		destexp = 0;
	}
	else
	{
		destexp = accum_exp_info->m_AccumExp;
	}

	accum_exp_info = ExpValueInfoParser::Instance()->GetAccumExpInfo(level);
	if ( accum_exp_info == NULL )
	{
		curaccumexp = 0;		
	}
	else
	{
		curaccumexp = accum_exp_info->m_AccumExp;
	}
	EXPTYPE curexp = accum_exp;

	__int64 iXpForNextLevel = (__int64)destexp - (__int64)curaccumexp;
	__int64 iCurrentXpForNextLevel = (__int64)curexp - (__int64)curaccumexp;

	if (iXpForNextLevel < 1)
	{
		iXpForNextLevel = 1;
	}

	if (iCurrentXpForNextLevel < 0)
	{
		iCurrentXpForNextLevel = 0;
	}

	fExpPercent = ((float)iCurrentXpForNextLevel / (float)iXpForNextLevel) * 100.0f;

	fExpPercent = min(100.0f,fExpPercent);

	return fExpPercent;
}

void uiSUNRankingDialog::set_settle_date_( const SETTLE_DATE& settle_date )
{
	settle_date_ = settle_date;
	settle_date_.ConvertTimeString();
}

void uiSUNRankingDialog::ResetSettleDate()
{
	settle_date_.type = 0;
	settle_date_.day  = 0;
	settle_date_.time = 0;
	ZeroMemory(settle_date_.result_string, INTERFACE_STRING_LENGTH);
	settle_date_.is_initialize = false;
	settle_date_.is_convert = false;
}

void uiSUNRankingDialog::ResetSUNRankingDialog()
{
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

	set_sun_ranking_mode_(SUN_RANKING_BEST_OF_BEST);
	InitTimer();
	RankingArticleInit();
	dropdown_class_filter_.Select(0);
}

void uiSUNRankingDialog::ResultMessageProc( const RC::eSUN_RANKING_RESULT& result_code )
{
	TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
	int string_index = 0;
	switch ( result_code )
	{
	case RC::RC_SUN_RANKING_SETTLE:
		{
			// 랭킹 정산 중 입니다.
			string_index = 3635;
		}
		break;

	case RC::RC_SUN_RANKING_INVALID_RANKING_TYPE:
		{
			// 알수 없는 랭킹 타입.
			string_index = 3636;
		}
		break;
	}

	if ( string_index > 0 )
	{
		g_InterfaceManager.GetInterfaceString(string_index, temp_string);
		GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CENTER_SHOW, temp_string);
	}
}

#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM