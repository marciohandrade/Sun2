#include "SunClientPrecompiledHeader.h"
#include "uiDominationZoneMarkDlg.h"
#include "interfacemanager.h"
#include "globalfunc.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "Mouse.h"
#include "OptionListParser.h"


#include "DominationInfoParser.h"

//------------------------------------------------------------------------------
 WzID uiDominationZoneMarkDlg::m_wzId[DIALOG_MAX] = 
 {
#ifdef _NA_000000_20130114_RENEWER_UI
	 StrToWzID("PI00"),	//DLGPOS_ZONEMARK_PIC1
	 StrToWzID("P000"),
	 StrToWzID("P001"),
  #ifdef _NA_008334_20150608_SONNENSCHEIN
     StrToWzID("P002"),
     StrToWzID("P003"),
  #endif //_NA_008334_20150608_SONNENSCHEIN
#else
	StrToWzID("PI00"),	//DLGPOS_ZONEMARK_PIC1
	StrToWzID("PI01"),
	StrToWzID("PI02"),
	StrToWzID("PI03"),
	StrToWzID("PI07"),
	StrToWzID("PI06"),
	StrToWzID("PI05"),
	StrToWzID("PI04"),	//DLGPOS_ZONEMARK_PIC8
    StrToWzID("P000"),	//DLGPOS_ZONEMARK_PIC10		
    StrToWzID("P001"),	
    StrToWzID("P002"),	
    StrToWzID("P003"),	
    StrToWzID("P004"),	
    StrToWzID("P005"),	
    StrToWzID("P006"),	
    StrToWzID("P007"),	// DLGPOS_ZONEMARK_PIC16
#endif
 };

//------------------------------------------------------------------------------
/**
*/
uiDominationZoneMarkDlg::uiDominationZoneMarkDlg(InterfaceManager *pUIMan)
:uiBase(pUIMan)
{

#ifdef _NA_000000_20130114_RENEWER_UI
	handle_text_Texture_ = g_pSunRenderer->x_pManagerTexture->\
		LoadTexture("Data\\Interface\\Common_bg02.tga" 
		,TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

	size_zone_marker_ = 0;
#else
	for (int i = 0; i < MAX_ZONEMARK; ++i)
	{
		m_pZoneMarkPic[i] = NULL;
	}
	max_count_zonemark_ = 0;
	
#endif

#ifdef _NA_008334_20150608_SONNENSCHEIN
    sonnenschein_level_ = 0;
#endif //_NA_008334_20150608_SONNENSCHEIN
}

//------------------------------------------------------------------------------
/**
*/
uiDominationZoneMarkDlg::~uiDominationZoneMarkDlg()
{

}

//------------------------------------------------------------------------------
/**
*/
void uiDominationZoneMarkDlg::Clear()
{	
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationZoneMarkDlg::Init(CDrawBase* pDrawBase)
{
	uiBase::Init(pDrawBase);



#ifdef _NA_000000_20130114_RENEWER_UI
	calculated_option_map_.clear();
	calculate_option_list_.clear();

	CCtrlPictureWZ* zonemark_picture = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PICl00]));
	zonemark_picture->ShowWindowWZ(WZ_SHOW);

	//존마크 최대치는 99개로 한정/
	max_count_zonemark_ = 99;
#else
	for (int Index = 0; Index < MAX_ZONEMARK; ++Index)
	{
		m_pZoneMarkPic[Index] = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PIC1+Index]));
		if (m_pZoneMarkPic[Index] != NULL)
		{
			++max_count_zonemark_;
		}
	}
#endif

    player_zonemark_reward_info_.GetRewardOptionInfo().reserve(MAX_ZONEMARK);
}

//------------------------------------------------------------------------------
/**
*/
void uiDominationZoneMarkDlg::OnShowWindow(BOOL val)
{
	if (val)
	{
		RECT rc;
		//MoveWindowPos();
        //MoveDefaultWindowPos();
		GetDialogWZ()->GetClientRect(&rc);
		DisableInterSectRect(rc);

        if (player_zonemark_reward_info_.GetRewardCount() == 0
#ifdef _NA_008334_20150608_SONNENSCHEIN
            && (sonnenschein_level_ == 0)
#endif //_NA_008334_20150608_SONNENSCHEIN
        )
		{
			ShowInterface(FALSE);
		}
	}
	else
	{
	}
}


//------------------------------------------------------------------------------
/**
*/
void uiDominationZoneMarkDlg::OnRenderSolarDialog()
{
	//마우스 오버시 툴팁 출력
	int			iMouseX = Mouse::Instance()->GetMouseX();
	int			iMouseY = Mouse::Instance()->GetMouseY();
#ifdef _NA_000000_20130114_RENEWER_UI
	render_mark_count_Image();

	CCtrlPictureWZ* zonemark_picture = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PICl00]));
	uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);

	if(zonemark_picture && tooltip_manager)
	{
		RECT rc = zonemark_picture->GetSizeRect();
		if(iMouseX > rc.left && 
			iMouseX < rc.right && 
			iMouseY > rc.top && 
			iMouseY < rc.bottom)
		{
			for(int index = 0 ; index < (int)calculate_option_list_.size() ; ++index)
			{
				tooltip_manager->RegisterTooltip(calculate_option_list_[index].option_name_, -1, 20, StrToWzID("m212"), WzColor_RGBA(255,255,255,255));
				tooltip_manager->SetTooltipDialogID( GetDialogKey() );
			}
		}
	}
#else
	RECT rc;
	DWORD dwOptionCount = player_zonemark_reward_info_.GetRewardCount();
    dwOptionCount = min(dwOptionCount, max_count_zonemark_);

	for (DWORD RewardIndex = 0; RewardIndex < dwOptionCount; ++RewardIndex)
	{
        if (m_pZoneMarkPic[RewardIndex] == NULL)
        {
            continue;
        }

		rc = m_pZoneMarkPic[RewardIndex]->GetSizeRect();
		if(iMouseX > rc.left && 
           iMouseX < rc.right && 
           iMouseY > rc.top && 
           iMouseY < rc.bottom)
		{
			player_zonemark_reward_info_.RenderTooltip(RewardIndex, false, GetDialogKey());
			break;
		}
	}	
#endif

#ifdef _NA_008334_20150608_SONNENSCHEIN
    if (sonnenschein_level_)
    {
        zonemark_picture = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PIC003]));
        if(zonemark_picture && tooltip_manager)
        {
            RECT rc = zonemark_picture->GetSizeRect();
            if(iMouseX > rc.left && 
                iMouseX < rc.right && 
                iMouseY > rc.top && 
                iMouseY < rc.bottom)
            {
                TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };
                TCHAR notice_Score[6];
                sprintf(notice_Score,"%d",sonnenschein_level_);

                g_InterfaceManager.GetInterfaceStringFormat(tooltip_string, INTERFACE_STRING_LENGTH, 3517, notice_Score);
                tooltip_manager->RegisterTooltip(tooltip_string, -1, 20, StrToWzID("m212"), RGBA(76,240,4,255)); // WzColor_RGBA(255,255,255,255)
                tooltip_manager->SetTooltipDialogID(GetDialogKey());
            }
        }
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
}

//------------------------------------------------------------------------------
/**
*/
#ifdef _NA_008334_20150608_SONNENSCHEIN
void uiDominationZoneMarkDlg::SetZoneMarkInfo(STLX_VECTOR<MAPCODE>& domination_state_info, BYTE sonnenschein_level)
#else 
void uiDominationZoneMarkDlg::SetZoneMarkInfo(STLX_VECTOR<MAPCODE>& domination_state_info)
#endif //_NA_008334_20150608_SONNENSCHEIN
{
    player_zonemark_reward_info_.SetZoneMarkInfo(domination_state_info
#ifdef _NA_008334_20150608_SONNENSCHEIN
        , sonnenschein_level
#endif //_NA_008334_20150608_SONNENSCHEIN
        );

#ifdef _NA_008334_20150608_SONNENSCHEIN
    sonnenschein_level_ = sonnenschein_level;
#endif //_NA_008334_20150608_SONNENSCHEIN

    size_t count_of_mark = player_zonemark_reward_info_.GetRewardCount();

#ifdef _NA_000000_20130114_RENEWER_UI
	count_of_mark = min(count_of_mark, max_count_zonemark_);
    size_zone_marker_ = (int)count_of_mark;
#else
	count_of_mark = min(count_of_mark, max_count_zonemark_);
#endif

#ifdef _NA_000000_20130114_RENEWER_UI

	CCtrlPictureWZ* zonemark_picture = static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PICl00]));
	zonemark_picture->ShowWindowWZ(WZ_SHOW);

	option_domination_calu();

#else
    for (DWORD index = 0; index < max_count_zonemark_; ++index)
    {
        if (m_pZoneMarkPic[index] == NULL)
        {
            continue;
        }

        if (index < count_of_mark)
        {
            m_pZoneMarkPic[index]->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            m_pZoneMarkPic[index]->ShowWindowWZ(WZ_HIDE);
        }
    }
#endif

}
#ifdef _NA_000000_20130114_RENEWER_UI

VOID uiDominationZoneMarkDlg::Release()
{
	if (handle_text_Texture_ != INVALID_HANDLE_VALUE)
	{
		g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_text_Texture_);
		handle_text_Texture_ = INVALID_HANDLE_VALUE;
	}

	calculated_option_map_.clear();
	calculate_option_list_.clear();
}

void uiDominationZoneMarkDlg::returnImageIndextoRect( int mark_count,int* score_index )
{
	int reset_count = mark_count;

	CheckModifyReturnValue(reset_count,score_index);

	//10의 단위와 1의 단위를 끈어야 된다.
}

int uiDominationZoneMarkDlg::CheckModifyReturnValue( int mark_count,int* array_count )
{
	int iDigitcount = 0;

	while (mark_count >= 0)
	{
		DWORD number = mark_count % 10;
		array_count[iDigitcount] = number;
		mark_count /= 10;
		iDigitcount++;

		if (mark_count == 0) 
			break;
	}

	int startindex = 0;
	int backindex = iDigitcount - 1;

	while(startindex < backindex)
	{
		array_count[startindex] ^= array_count[backindex];
		array_count[backindex] ^= array_count[startindex];
		array_count[startindex] ^= array_count[backindex];

		startindex++;
		backindex--;
	}
	return iDigitcount;
}

void uiDominationZoneMarkDlg::render_mark_count_Image()
{
	int Array_Output[5] = {0,};

	int count = CheckModifyReturnValue(size_zone_marker_,Array_Output);

	WzColor wcRadarPointColor[4];
	for (int a = 0; a < 4; a++)
	{
		wcRadarPointColor[a] = WzColor_RGBA(255,255,255,255);
	}

	//10이하 0~9
	if(size_zone_marker_ < 10)
	{
		int temp = Array_Output[0];
		Array_Output[0] = Array_Output[1];
		Array_Output[1] = temp;
	}

	//시작 위치값
	CCtrlPictureWZ* text_picture1= static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PIC000]));
	RECT rect = text_picture1->GetSizeRect();

	int number_size_x = 13;
	int number_size_y = 19;

	int source_x = 0;
	int source_y = 511;
	float desc_rect_left = (float)rect.left;

	//처음 0이라는 이미지를 그린뒤
	//그뒤 10의 자리가 있을경우 뒤집에 넣는다.
	for(int i = 1; i >= 0; --i)
	{
		text_picture1= static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PIC001-i]));
		rect = text_picture1->GetSizeRect();

		//배열의 반복은 역순으로
		source_x = 0 + ((number_size_x) * Array_Output[1-i]);

		float desc_rect_left = (float)rect.left;

		g_pSunRenderer->RenderTexture(
			handle_text_Texture_,
			(float)desc_rect_left,
			(float)rect.top,
			(float)number_size_x,
			(float)number_size_y,
			(float)source_x,
			(float)source_y,
			(float)number_size_x,
			(float)number_size_y,
			wcRadarPointColor);
	}

#ifdef _NA_008334_20150608_SONNENSCHEIN
    if (sonnenschein_level_ != 0)
    {
        text_picture1= static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PIC002]));
        if (text_picture1)
        {
            // " - " 출력
            rect = text_picture1->GetSizeRect();
            g_pSunRenderer->RenderTexture(
                handle_text_Texture_,
                (float)rect.left,
                (float)rect.top,
                (float)number_size_x,
                (float)number_size_y,
                (float)130,
                (float)source_y,
                (float)number_size_x,
                (float)number_size_y,
                wcRadarPointColor);
        }
        
        text_picture1= static_cast<CCtrlPictureWZ*>(GetControlWZ(m_wzId[DLGPOS_ZONEMARK_PIC003]));
        if (text_picture1)
        {
            // 단계값 출력
            rect = text_picture1->GetSizeRect();
            g_pSunRenderer->RenderTexture(
                handle_text_Texture_,
                (float)rect.left,
                (float)rect.top,
                (float)number_size_x,
                (float)number_size_y,
                (float)sonnenschein_level_ * number_size_x,
                (float)source_y,
                (float)number_size_x,
                (float)number_size_y,
                wcRadarPointColor);
        }
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
}

void uiDominationZoneMarkDlg::option_domination_calu()
{
	calculated_option_map().clear();
	calculate_option_list_.clear();

	CalculatedOption temp_option;

	const DominationInfoParser::SelectedRewardOptions& selected_option_info = player_zonemark_reward_info_.GetSelectedRewardOptionInfo();

	DominationInfoParser::SelectedRewardOptions::const_iterator begin_itr = selected_option_info.begin();
	DominationInfoParser::SelectedRewardOptions::const_iterator end_itr = selected_option_info.end();
	for ( ; begin_itr != end_itr; ++begin_itr)
	{
		const DominationInfoParser::SelectedRewardOption& selected_option = begin_itr->second;

		DWORD option_kind = selected_option.option_info->m_wOptionKind;
		CalculatedOptionMap::iterator found_itr = calculated_option_map().find(option_kind);
		if (found_itr == calculated_option_map().end())
		{
			calculated_option_map().insert(
				CalculatedOptionMap::value_type(option_kind, CalculatedOption()));
			found_itr = calculated_option_map().find(option_kind);
		}
		CalculatedOption& temp_option = found_itr->second;
		temp_option.OptionKind = option_kind;

		if (selected_option.option_info->m_byOptionType == VALUE_TYPE_VALUE)
		{
			temp_option.OptionValue += selected_option.option_info->m_iOptionValue;
		}
		else
		{
			temp_option.OptionRatio += selected_option.option_info->m_iOptionValue;
		}

		BASE_OptionList* option_list = OptionListParser::Instance()->GetOptionList(option_kind);
		if (option_list != NULL)
		{
			temp_option.OptionNC = option_list->mdwNameCode;
		}
	}

	{
		calculate_option_list_.clear();

		TCHAR option_string[INTERFACE_STRING_LENGTH] = {'\0', };
		TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };

		CalculatedOptionMap::iterator begin_itr = calculated_option_map().begin();
		CalculatedOptionMap::iterator end_itr = calculated_option_map().end();
		for ( ; begin_itr != end_itr; ++begin_itr)
		{
			CalculatedOption& calculated_option = begin_itr->second;

			// 옵션명
			g_InterfaceManager.GetInterfaceString(calculated_option.OptionNC, option_string);
			strcat(option_string, " : ");

			// 수치
			bool is_first_value = true;
			if (calculated_option.OptionValue != 0)
			{
				Snprintf(temp_string, INTERFACE_STRING_LENGTH, _T("%+d"), calculated_option.OptionValue);
				BASE_OptionList* option_list = OptionListParser::Instance()->GetOptionList(calculated_option.OptionKind);
				GlobalFunc::AddOptionSuffix(VALUE_TYPE_VALUE, option_list, temp_string);
				strcat(option_string, temp_string);
				is_first_value = false;
			}
			if (calculated_option.OptionRatio != 0)
			{
				Snprintf(temp_string, 
					INTERFACE_STRING_LENGTH, 
					_T("%+.1f%%"), 
					(calculated_option.OptionRatio / 10.0f));
				if (is_first_value == false)
				{
					strcat(option_string, ", ");
				}
				strcat(option_string, temp_string);
			}

			OptionStringMark mark_tooltip;
			mark_tooltip.SetOptionMark(option_string);

			calculate_option_list_.push_back(mark_tooltip);
		}
	}


}

void uiDominationZoneMarkDlg::OnUpdateSolarDialog()
{

}

#endif
//------------------------------------------------------------------------------
/**
*/
