#include "SunClientPrecompiledHeader.h"
#ifdef _DEV_VER
#include "uiGMItemOptionWindow/uiGMItemOptionWindow.h"
#include "uiGMWindowMan/uiGMWindowMan.h"
#include "uiGMItemWindow/uiGMItemWindow.h"
#include "GlobalFunc.h"
#include "Mouse.h"

const char* k_make_string = "//make [아이템코드] [수량] (중첩) (아이템타입) (인첸트) (소켓수) (각성)";

WzID uiGMItemOptionWindow::m_wzId[MAX_CONTROL] = 
{
	StrToWzID("E000"),//ITEM_NUMBER_EDIT,
	StrToWzID("BT00"),//INIT_BTN,
    StrToWzID("C001"),//BERSERKER_BTN,
    StrToWzID("C002"),//DRAGONKNIGHT_BTN,
    StrToWzID("C003"),//VALKYRIE_BTN,
    StrToWzID("C004"),//SHADOW_BTN,
    StrToWzID("C005"),//ELEMENTALIST_BTN,
	StrToWzID("B000"),//OPTION1_FORWARD_BTN,
	StrToWzID("B001"),//OPTION1_BACKWARD_BTN,
	StrToWzID("B002"),//OPTION2_FORWARD_BTN,
	StrToWzID("B003"),//OPTION2_BACKWARD_BTN,
	StrToWzID("B004"),//OPTION3_FORWARD_BTN,
	StrToWzID("B005"),//OPTION3_BACKWARD_BTN,
    StrToWzID("B006"),//OPTION4_FORWARD_BTN,
    StrToWzID("B007"),//OPTION4_BACKWARD_BTN, 
	StrToWzID("B011"),//OPTION5_FORWARD_BTN, 
	StrToWzID("B008"),//OPTION5_BACKWARD_BTN, 
	
	StrToWzID("T021"),//OPTION1_TEXT,
	StrToWzID("T022"),//OPTION2_TEXT,
	StrToWzID("T023"),//OPTION3_TEXT,
    StrToWzID("T024"),//OPTION4_TEXT,
	StrToWzID("S000"),//OPTION5_TEXT,

    StrToWzID("B010"), //FILTER1_FORWARD_BTN,
    StrToWzID("BT04"),//FILTER1_BACKWARD_BTN,
	                  
    StrToWzID("B012"), //FILTER2_FORWARD_BTN,
    StrToWzID("B009"),//FILTER2_BACKWARD_BTN,
	
	StrToWzID("ST10"),//FILTER1_TEXT,
	StrToWzID("ST11"),//FILTER2_TEXT,
                      
	StrToWzID("BT01"),//OK_BTN,
	StrToWzID("BT12"),//CANCLE_BTN,

    StrToWzID("T001"),//NUMBER_TEXT

    StrToWzID("ST00"),//TITLE_NAME
    StrToWzID("T002"),//OPTION_NAME
    StrToWzID("T006"),//FILTER_NAME
    StrToWzID("ST09"),//FULL_STRING    
};


uiGMItemOptionWindow::uiGMItemOptionWindow(InterfaceManager* ui_manager)
	:uiBase(ui_manager)
{
    static_edit_control_ = NULL;
    is_search_ = false;
    count_edit_box_ = NULL;
    full_cheat_string_ = NULL;
    ok_control_ = NULL;
    init_control_ = NULL;
}

uiGMItemOptionWindow::~uiGMItemOptionWindow(void)
{
}

void uiGMItemOptionWindow::InitParametas()
{
    if (ui_gm_window_manager_ != NULL)
    {
        //item_option_parametas_ = *(ui_gm_window_manager_->item_parametas());
        SetItemOptionParametas(ui_gm_window_manager_->item_parametas());
    }
    for (int i = PARAM_ITEM_CODE; i < PARAM_ITEM_MAX; ++i)
    {
        SetItemOptionCaptions(static_cast<PARAM_NAME>(i));
    }

    if (count_edit_box_)
    {
        count_edit_box_->ShowWindowWZ(WZ_HIDE);
    }
    if (item_option_parametas_.class_type == eCHAR_NONE)
    {
        SetClass(eCHAR_NONE);
    }
}

void uiGMItemOptionWindow::SetItemOptionParametas(ITEM_PARAMETAS* item_parameta)
{
    if (item_parameta)
    {
        item_option_parametas_ = *item_parameta;
    }

    for (int i = PARAM_ITEM_CODE; i < PARAM_ITEM_MAX; ++i)
    {
        SetItemOptionCaptions(static_cast<PARAM_NAME>(i));
    }
    if (item_option_parametas_.class_type == eCHAR_NONE)
    {
        SetClass(eCHAR_NONE);
    }
    if (count_edit_box_)
    {
        TCHAR text[32] = {0,};
        Sprintf(text, "%d", item_option_parametas_.item_count);
        count_edit_box_->SetTextWZ(text);
        count_edit_box_->ShowWindowWZ(WZ_HIDE);
    }
    if (IsChange())
    {
        if (ok_control_)
        {
            ok_control_->EnableWindowWZ(true);
        }
    }
    else
    {            
        if (ok_control_)
        {
            ok_control_->EnableWindowWZ(false);
        }
    }
}

void uiGMItemOptionWindow::Init(CDrawBase* drawbase)
{
	uiBase::Init(drawbase);

    TCHAR temp_string[128] = {0,};
    ui_gm_window_manager_ = static_cast<uiGMWindowMan*>(GetMan());

	count_edit_box_ = (CCtrlDxEditBox*)getControl(ITEM_NUMBER_EDIT);
	if (count_edit_box_)
	{
		count_edit_box_->SetLimitText(20);
		count_edit_box_->ClearText();
	}

    RECT rect;
    this->GetDialogWZ()->GetDlgSize(&rect);
    this->GetDialogWZ()->SetMoveRECT(rect);

    CControlWZ* control = getControl(TITLE_NAME);
    if (control)
    {
        control->SetTextWZ("GM 아이템 옵션");
    }
    control = getControl(OPTION_NAME);
    if (control)
    {
        control->SetTextWZ("생성 옵션");
    }
    control = getControl(FILTER_NAME);
    if (control)
    {
        control->SetTextWZ("검색 필터");
    }

    ok_control_ = static_cast<CCtrlButtonWZ*>(getControl(OK_BTN));
    if (ok_control_)
    {
        g_InterfaceManager.GetInterfaceString(100, temp_string); // 100: 확인
        ok_control_->SetTextWZ(temp_string);
    }
    control = getControl(CANCLE_BTN);
    if (control)
    {
        g_InterfaceManager.GetInterfaceString(101, temp_string); // 101: 취소
        control->SetTextWZ(temp_string);
    }
    init_control_ = static_cast<CCtrlButtonCheckWZ*>(getControl(INIT_BTN));
    if (init_control_)
    {
        g_InterfaceManager.GetInterfaceString(455, temp_string); // 455: 초기화
        init_control_->SetTextWZ(temp_string);
    }

    full_cheat_string_ = (CCtrlStaticWZ*)getControl(FULL_STRING);
    if (full_cheat_string_)
    {
        full_cheat_string_->SetTextWZ(k_make_string);
    }

    static_edit_control_ = static_cast<CCtrlStaticWZ*>(getControl(NUMBER_TEXT));
    assert(static_edit_control_);

    InitParametas();
}

void uiGMItemOptionWindow::SetClass(int class_type)
{
    uiGMItemWindow* dialog = GET_CAST_DIALOG(uiGMItemWindow, IM_GM_WINDOW_MANAGER::GM_ITEM_WINDOW);
    if (dialog)
    {
        dialog->SetClass(class_type);
    }
}

void uiGMItemOptionWindow::SetClassType(int class_type)
{ 
    item_option_parametas_.class_type = class_type; 
    if (IsChange()) // ParametaScroll() 먼저 적용후 체크해야한다.
    {
        if (ok_control_)
        {
            ok_control_->EnableWindowWZ(true);
        }
    }
    else
    {            
        if (ok_control_)
        {
            ok_control_->EnableWindowWZ(false);
        }
    }
}

void uiGMItemOptionWindow::Release()
{
}

void uiGMItemOptionWindow::MessageProc(SI_MESSAGE* message)
{
	switch(message->eResultMsg) 
	{
	case RT_MSG_LBUTTONCLICK:
		{
			OnLButtonClick(message);
		}
		break;
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            if (count_edit_box_)
            {
                count_edit_box_->ShowWindowWZ(WZ_HIDE);
            }
            ParametaScroll(PARAM_ITEM_COUNT, true);
            if (IsChange()) // ParametaScroll() 먼저 적용후 체크해야한다.
            {
                if (ok_control_)
                {
                    ok_control_->EnableWindowWZ(true);
                }
            }
            else
            {            
                if (ok_control_)
                {
                    ok_control_->EnableWindowWZ(false);
                }
            }
        }
        break;
    case RT_MSG_RBUTTONCLICK:
        {
            if (ui_gm_window_manager_->IsShowGMWindow(UI_GM_ITEM_RESET_WINDOW))
            {
                ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_RESET_WINDOW, false);
            }
        }
        break;
	}
}

void uiGMItemOptionWindow::OnLButtonClick(SI_MESSAGE* message)
{
    ITEM_PARAMETAS* item_parameta;
    DWORD item_count = 0;
    if (ui_gm_window_manager_ != NULL)
    {
        item_parameta = ui_gm_window_manager_->item_parametas();
        item_count = item_parameta->item_count;
    }

    PARAM_NAME parameta_name = PARAM_ITEM_MAX;
    bool is_down_scroll = true;
    bool result = true;
    DWORD control_id = message->dwCtrlID;
    POSTYPE control_postion = getCtrlIDToPos(control_id);    
    switch (control_postion)
    {
    case OPTION1_FORWARD_BTN:
        parameta_name = PARAM_ITEM_OVERLAPPING;
        break;
    case OPTION1_BACKWARD_BTN: is_down_scroll = false;
        parameta_name = PARAM_ITEM_OVERLAPPING;
        break;
    case OPTION2_FORWARD_BTN:
        parameta_name = PARAM_ITEM_TYPE;
        break;
    case OPTION2_BACKWARD_BTN: is_down_scroll = false;
        parameta_name = PARAM_ITEM_TYPE;
        break;
    case OPTION3_FORWARD_BTN:
        parameta_name = PARAM_ITEM_ENCHANT;
        break;
    case OPTION3_BACKWARD_BTN: is_down_scroll = false;
        parameta_name = PARAM_ITEM_ENCHANT;
        break;
    case OPTION4_FORWARD_BTN:
        parameta_name = PARAM_ITEM_SOCKET_COUNT;
        break;
    case OPTION4_BACKWARD_BTN: is_down_scroll = false;
        parameta_name = PARAM_ITEM_SOCKET_COUNT;
        break;
	case OPTION5_FORWARD_BTN:
		parameta_name = PARAM_ITEM_AWAKENING;
		break;
	case OPTION5_BACKWARD_BTN: is_down_scroll = false;
		parameta_name = PARAM_ITEM_AWAKENING;
		break;
    case FILTER1_FORWARD_BTN:
        parameta_name = PARAM_ITEM_MAIN_FILTER;
        break;
    case FILTER1_BACKWARD_BTN: is_down_scroll = false;
        parameta_name = PARAM_ITEM_MAIN_FILTER;
        break;
    case FILTER2_FORWARD_BTN:
        parameta_name = PARAM_ITEM_SUB_FILTER;
        break;
    case FILTER2_BACKWARD_BTN: is_down_scroll = false;
        parameta_name = PARAM_ITEM_SUB_FILTER;
        break;
    case ITEM_NUMBER_EDIT:
        if (static_edit_control_)
        {
            static_edit_control_->ShowWindowWZ(WZ_HIDE);
        }
        break;
    case INIT_BTN:
        {
            if (ui_gm_window_manager_)
            {
                ui_gm_window_manager_->ToggleGMWindow(UI_GM_ITEM_RESET_WINDOW);
            }
        }
        break;
    case OK_BTN:
        {
            if (ui_gm_window_manager_)
            {
                ParametaScroll(PARAM_ITEM_COUNT, true);
                ui_gm_window_manager_->SetItemParametas(&item_option_parametas_);
            }
            ShowInterface(FALSE);
        }
        break;
    case CANCLE_BTN:
        {
            SetClass(eCHAR_NONE);//일단 모두 해제 후 세팅을 해야 선택된 직업 중복으로 해제 되지 않는다.
            SetClass(item_parameta->class_type);
            ShowInterface(FALSE);
        }
        break;
    default:
        {
            TCHAR text[MAX_PATH] = {0,};
            RECT static_edit_rect = static_edit_control_->GetSizeRect();
            int mouse_x = Mouse::Instance()->GetMouseX();
            int mouse_y = Mouse::Instance()->GetMouseY();
            if (mouse_x > static_edit_rect.left && mouse_x < static_edit_rect.right
                && mouse_y > static_edit_rect.top && mouse_y < static_edit_rect.bottom)
            {
                static_edit_control_->ShowWindowWZ(WZ_HIDE);

                if (count_edit_box_)
                {
                    count_edit_box_->ShowWindowWZ(WZ_SHOW);
                    static_edit_control_->ShowWindowWZ(WZ_HIDE);  
                    Sprintf(text, "%d", item_parameta->item_count);
                    count_edit_box_->SetTextWZ(text);
                }
                control_postion = NUMBER_TEXT;
            }
            else
            {
                count_edit_box_->ShowWindowWZ(WZ_HIDE);
            }

            count_edit_box_->GetTextWZ(text, MAX_PATH);
            item_parameta->item_count = atoi(text);

            if (item_count != item_parameta->item_count)
            {
                parameta_name = PARAM_ITEM_COUNT;
            }
            else
            {
                parameta_name = PARAM_ITEM_MAX;
            }
        }
    }

    if (control_postion != ITEM_NUMBER_EDIT && control_postion != NUMBER_TEXT)
    {
        if (count_edit_box_)
        {
            count_edit_box_->ShowWindowWZ(WZ_HIDE);
        }
        SetItemOptionCaptions(PARAM_ITEM_COUNT);
    }

    if (parameta_name != PARAM_ITEM_MAX)
    {
        ParametaScroll(parameta_name, is_down_scroll);
        if (IsChange()) // ParametaScroll() 먼저 적용후 체크해야한다.
        {
            if (ok_control_)
            {
                ok_control_->EnableWindowWZ(true);
            }
        }
        else
        {            
            if (ok_control_)
            {
                ok_control_->EnableWindowWZ(false);
            }
        }
    }

    if (control_postion != INIT_BTN && ui_gm_window_manager_)
    {
        if (ui_gm_window_manager_->IsShowGMWindow(UI_GM_ITEM_RESET_WINDOW))
        {
            ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_RESET_WINDOW, false);
        }
    }
}

void uiGMItemOptionWindow::OnUpdateSolarDialog()
{
    if (ui_gm_window_manager_)
    {
        if (is_search_ == false && ui_gm_window_manager_->IsSearching() == true)
        {
            is_search_ = true;
            if (ok_control_)
            {
                ok_control_->EnableWindowWZ(false);
            }
        }
    }
}

void uiGMItemOptionWindow::OnShowWindow(BOOL val)
{
	if (val)
	{
        InitParametas();
        if (ok_control_)// IsChange()를 해야 하지만 방금 init했기 때문에 바뀔 이유가 없음
        {
            ok_control_->EnableWindowWZ(false);
        }
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_GM_Item_Option_Window;
        msg.wParam = IM_GM_WINDOW_MANAGER::GM_ITEM_OPTION_WINDOW;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
	}
    else
    {
        if (ui_gm_window_manager_)
        {
            //필터값 윈도우에 세팅
            TCHAR caption_string[INTERFACE_STRING_LENGTH] = {0,};
            if (ui_gm_window_manager_)
            {
                item_option_parametas_ = *(ui_gm_window_manager_->item_parametas());
            }
            SetCaptionMainFilter(caption_string);
            SetCaptionSubFilter(caption_string);
            ui_gm_window_manager_->SetCaptionFilter();
            ui_gm_window_manager_->ShowGMWindow(UI_GM_ITEM_RESET_WINDOW, false);
        }
        g_KeyQueueManager.DeleteMsg(eDoType_GM_Item_Option_Window);
    }

    uiGMItemWindow* dialog = GET_CAST_DIALOG(uiGMItemWindow, IM_GM_WINDOW_MANAGER::GM_ITEM_WINDOW);        
    if (dialog)
    {
        dialog->ShowClassIcon((val)?true:false);
    }
}

CControlWZ * uiGMItemOptionWindow::getControl(POSTYPE AtPos)
{
	return GetControlWZ( m_wzId[AtPos] );
}

POSTYPE uiGMItemOptionWindow::getCtrlIDToPos(WzID wzId)
{
	for( POSTYPE i = 0 ; i < MAX_CONTROL; ++i )
	{
		if( m_wzId[i] == wzId )
		{
			return i;
		}
	}

	return MAX_CONTROL;
}

void uiGMItemOptionWindow::ParametaScroll(PARAM_NAME parameta_name, bool is_down_scroll)
{
    switch (parameta_name)
    {
    case PARAM_ITEM_CODE: // none
        break;
    case PARAM_ITEM_COUNT: // none
        {
            if (count_edit_box_ != NULL)
            {
                TCHAR number_string[MAX_PATH] = {NULL, };
                count_edit_box_->GetTextWZ(number_string, MAX_PATH);
                if (_tcslen(number_string) > 0)
                {
                    item_option_parametas_.item_count = atoi(number_string);
                    if (item_option_parametas_.item_count > 1000)
                    {
                        item_option_parametas_.item_count = 1000;
                    }
                }
                else
                {
                    item_option_parametas_.item_count = 0;
                }
            }
        }
        break;
    case PARAM_ITEM_OVERLAPPING:
        if (item_option_parametas_.overlapping == true)
        {
            item_option_parametas_.overlapping = false;
        }
        else
        {
            item_option_parametas_.overlapping = true;
        }
        break;
    case PARAM_ITEM_TYPE:
        {
            if (is_down_scroll == false)
            {
                item_option_parametas_.item_type = static_cast<ITEM_TYPE>(item_option_parametas_.item_type + 1);
                if (item_option_parametas_.item_type ==ITEM_TYPE_MAX)
                {
                    item_option_parametas_.item_type = ITEM_TYPE_NORMAL;
                }
            }
            else// (is_down_scroll == true)
            {
                if (item_option_parametas_.item_type == ITEM_TYPE_NORMAL)
                {
                    item_option_parametas_.item_type = ITEM_TYPE_SKIN;
                }
                else
                {
                    item_option_parametas_.item_type = static_cast<ITEM_TYPE>(item_option_parametas_.item_type - 1);
                }
            }
        }
        break;
    case PARAM_ITEM_ENCHANT:
        {
            if (is_down_scroll == false)
            {
                ++item_option_parametas_.enchant_level;
                if (item_option_parametas_.enchant_level > ENCHANT_LEVEL_MAX)
                {
                    item_option_parametas_.enchant_level = 0;
                }
            }
            else// is_down_scroll == true
            {
                if (item_option_parametas_.enchant_level <= 0)
                {
                    item_option_parametas_.enchant_level = ENCHANT_LEVEL_MAX;
                }
                else
                {
                    --item_option_parametas_.enchant_level;
                }
            }
        }
        break;
    case PARAM_ITEM_SOCKET_COUNT:
        {
            if (is_down_scroll == false)
            {
                ++item_option_parametas_.socket_count;
                if (item_option_parametas_.socket_count > (SOCKET_MAX + 1))
                {
                    item_option_parametas_.socket_count = 0; // 소켓 미확인
                }
            }
            else//is_down_scroll == true
            {
                if (item_option_parametas_.socket_count == 0)
                {
                    item_option_parametas_.socket_count = (SOCKET_MAX + 1); // 소켓 없음
                }
                else
                {
                    --item_option_parametas_.socket_count;
                }
            }
        }
        break;
	case PARAM_ITEM_AWAKENING:
		{
			if (is_down_scroll == false)
			{
				++item_option_parametas_.awakening_level;
				if (item_option_parametas_.awakening_level > MAX_AWAKENING_GRADE)
				{
					item_option_parametas_.awakening_level = 0;
				}
			}
			else// is_down_scroll == true
			{
				if (item_option_parametas_.awakening_level <= 0)
				{
					item_option_parametas_.awakening_level = MAX_AWAKENING_GRADE;
				}
				else
				{
					--item_option_parametas_.awakening_level;
				}
			}
		}
		break;
    case PARAM_ITEM_MAIN_FILTER:
        {
            ScrollMainFilter(is_down_scroll);
            SetItemOptionCaptions(PARAM_ITEM_SUB_FILTER);
        }
        break;
    case PARAM_ITEM_SUB_FILTER:
        {
            ScrollSubFilter(is_down_scroll);
        }
        break;
    case PARAM_ITEM_CLASS:
        // none
        break;
    }

    //ui_gm_window_manager_->SetItemParametas(item_parameta);
    SetItemOptionCaptions(parameta_name);
}

void uiGMItemOptionWindow::ScrollSubFilter(bool is_down_scroll)
{
    FILTERS filter = item_option_parametas_.filter_main;

    if (is_down_scroll == false)
    {
        ++item_option_parametas_.filter_subs[filter];
    }

    BYTE max_value = 0;
    BYTE first_value = 0;
    BYTE last_value = 0;

    switch (filter)
    {
    case FILTER_ALL: // 전체 (필터 없음)
        // 없음
        max_value = 1;// 필터 없음 1개뿐
        break;
    case FILTER_EQUIP: // 장비 필터
        max_value = EQUIP_FITERL_MAX;
        break;
    case FILTER_WASTE: // 소모품 필터
        max_value = WASTE_FILTER_MAX;
        break;
    case FILTER_GRADE: // 등급 필터
        max_value = GRADE_FILTER_MAX;
        break;
    case FILTER_GENDER: // 성별 필터
        max_value = GENDER_FILTER_MAX;
        break;
    case FILTER_ETC: // 종류 필터
        max_value = ETC_FILTER_MAX;
        break;
    //FILTER_MAX,
    default:
        break;
    }

    last_value = max_value - 1; // max -1 = 마지막 인덱스
    if (is_down_scroll == false)
    {
        if (item_option_parametas_.filter_subs[filter] == max_value)
        {
            item_option_parametas_.filter_subs[filter] = first_value;
        }
    }
    else
    {
        if (item_option_parametas_.filter_subs[filter] == first_value)
        {
            item_option_parametas_.filter_subs[filter] = last_value;
        }
        else
        {
            --item_option_parametas_.filter_subs[filter];
        }
    }
}

RECT uiGMItemOptionWindow::GetResetControlPosition(RECT* control_rect)
{
    if (init_control_)
    {
        (*control_rect) = init_control_->GetSizeRect();
    }
    return (*control_rect);
}

void uiGMItemOptionWindow::ScrollMainFilter(bool is_down_scroll)
{
    if (is_down_scroll == false)
    {
        item_option_parametas_.filter_main = static_cast<FILTERS>(item_option_parametas_.filter_main + 1);
        if (item_option_parametas_.filter_main == FILTER_MAX)
        {
            item_option_parametas_.filter_main = FILTER_ALL;
        }
    }
    else   // is_down_scroll == true
    {
        if (item_option_parametas_.filter_main == FILTER_ALL)
        {
            item_option_parametas_.filter_main = FILTER_ETC;
        }
        else
        {
            item_option_parametas_.filter_main = static_cast<FILTERS>(item_option_parametas_.filter_main - 1);
        }
    }
}

void uiGMItemOptionWindow::SetItemOptionCaptions(PARAM_NAME parameta_name, TCHAR* clipboard_string/* = ""*/)
{
    TCHAR caption_string[128] = {0,};
    static TCHAR last_create_string[INTERFACE_STRING_LENGTH] = {0,};
    if (clipboard_string != NULL)
    {
        Sprintf(last_create_string, clipboard_string);
    }
    int control_position = -1;

    
    switch (parameta_name)
    {
    case PARAM_ITEM_CODE: // none
        break;
    case PARAM_ITEM_COUNT: // none
        control_position = NUMBER_TEXT;
        if (item_option_parametas_.item_count > 0)
        {
            Sprintf(caption_string, "생성개수: %d", item_option_parametas_.item_count);
        }
        else
        {
            Sprintf(caption_string, "생성개수: MAX (0 = MAX)");
        }
        if (static_edit_control_)
        {
            static_edit_control_->ShowWindowWZ(WZ_SHOW);
        }
        break;
    case PARAM_ITEM_OVERLAPPING:
        control_position = OPTION1_TEXT;
        if (item_option_parametas_.overlapping == true)
        {
            Sprintf(caption_string, "겹치기(가능)");
        }
        else
        {
            Sprintf(caption_string, "겹치기(불가)");
        }
        break;
    case PARAM_ITEM_TYPE:
        {
            control_position = OPTION2_TEXT;
            switch (item_option_parametas_.item_type)
            {
            case ITEM_TYPE_NORMAL:
                Sprintf(caption_string, "Type 일반");
                break;
            case ITEM_TYPE_DIVINE:
                Sprintf(caption_string, "Type 디바인");
                break;
            case ITEM_TYPE_ETHERIA:
                Sprintf(caption_string, "Type 에테리아");
                break;
            case ITEM_TYPE_DIVINE_ETHERIA:
                Sprintf(caption_string, "Type 디바인+에테리아");
                break;
            case ITEM_TYPE_SKIN:
                Sprintf(caption_string, "Type 스킨");
                break;
            default:
                Sprintf(caption_string, "Type 없음");
                break;
            }
        }
        break;
    case PARAM_ITEM_ENCHANT:
        {
            control_position = OPTION3_TEXT;
            Sprintf(caption_string, "인첸트 %d", item_option_parametas_.enchant_level);
        }
        break;
    case PARAM_ITEM_SOCKET_COUNT:
        {
            control_position = OPTION4_TEXT;
            if (item_option_parametas_.socket_count == 0)
            {
                Sprintf(caption_string, "소켓 미확인");
            }
            else if (item_option_parametas_.socket_count >= (SOCKET_MAX + 1))
            {
                Sprintf(caption_string, "소켓 없음");
            }
            else 
            {
                Sprintf(caption_string, "소켓수 %d", item_option_parametas_.socket_count);
            }
        }
        break;
	case PARAM_ITEM_AWAKENING:
		{
			control_position = OPTION5_TEXT;
			Sprintf(caption_string, "각성 %d", item_option_parametas_.awakening_level);
		}
		break;
    case PARAM_ITEM_MAIN_FILTER:
        {
            control_position = FILTER1_TEXT;
            SetCaptionMainFilter(caption_string);
        }
        break;
    case PARAM_ITEM_SUB_FILTER:
        {
            control_position = FILTER2_TEXT;
            SetCaptionSubFilter(caption_string);
        }
        break;
    case PARAM_ITEM_CLASS:
        {   
            //none;
        }
        break;
    default:
        break;
    }

    if (full_cheat_string_)
    {
        char format_string[INTERFACE_STRING_LENGTH] = {0,};
        char setting_value_string[INTERFACE_STRING_LENGTH] = {0,};
        if (item_option_parametas_.socket_count >= (SOCKET_MAX + 1))
        {
            strcpy(format_string, "%s\n//make %d %d %d %d %d %d\n클립보드://");
        }
        else
        {
            strcpy(format_string, "%s\n//make %d %d %d %d %d %d %d\n클립보드://");
        }

        WORD item_count = item_option_parametas_.item_count;
        DWORD item_code = item_option_parametas_.item_code;
        if (item_count == 0)
        {
            item_count = 1;
        }
        if (ui_gm_window_manager_)
        {
            item_code = ui_gm_window_manager_->GetLastMakeItemCode();
        }
        
        Sprintf(setting_value_string, format_string, k_make_string, item_code, item_count, 
            item_option_parametas_.overlapping, item_option_parametas_.item_type, 
            item_option_parametas_.enchant_level, item_option_parametas_.socket_count, item_option_parametas_.awakening_level);
        strcat(setting_value_string, last_create_string);

        full_cheat_string_->SetTextWZ(setting_value_string);
    }

    if (control_position >= 0)
    {
        CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(getControl(control_position));
        GlobalFunc::SetCaption(control, caption_string);

        if (item_option_parametas_.filter_main == FILTER_ALL)
        {   // 전체필터인 경우는 클래스 필터가 있어도 옵션 창에서는 
            // "필터 없음"으로 세팅 되어야 한다
            CCtrlStaticWZ* control = static_cast<CCtrlStaticWZ*>(getControl(FILTER2_TEXT));
            GlobalFunc::SetCaption(control, "필터 없음");
        }
    }
}

void uiGMItemOptionWindow::SetCaptionMainFilter(TCHAR* caption_string)
{
    FILTERS filter = item_option_parametas_.filter_main;
    switch (filter)
    {
    case FILTER_ALL:
        Sprintf(caption_string, "전체");
        break;
    case FILTER_EQUIP:
        Sprintf(caption_string, "장비");
        break;
    case FILTER_WASTE:
        Sprintf(caption_string, "소모품");
        break;
    case FILTER_GRADE:
        Sprintf(caption_string, "등급");
        break;
    case FILTER_GENDER:
        Sprintf(caption_string, "성별");
        break;
    case FILTER_ETC:
        Sprintf(caption_string, "종류");
        break;
    default:
        Sprintf(caption_string, "Unknown Main Filter");
        break;
    }
}

void uiGMItemOptionWindow::SetCaptionSubFilter(TCHAR* caption_string)
{
    FILTERS filter = item_option_parametas_.filter_main;
    BYTE sub_value = item_option_parametas_.filter_subs[filter];
    const size_t kbuffer_size = 32;

    TCHAR equip_filter_names[EQUIP_FITERL_MAX][kbuffer_size] = { "장비 전체", "무기", "방어구", "악세사리", "특수 악세사리" };
    TCHAR waste_filter_names[WASTE_FILTER_MAX][kbuffer_size] = { "소모품 전체", "사용 가능", "소모 전용", "물약" };
    TCHAR grade_filter_names[GRADE_FILTER_MAX][kbuffer_size] = { "일반 등급", "얼티메이트",
        "엘리트 1차", "저주받은 세트", "엘리트 1.5차", "엘리트 2차","엘리트 2.5차", "유니크 1차", "유니크 1.5차", "유니크 2차", "유니크 2.5차", "럭키 아이템", "투사의 엘리트 2차","엘리트 3차", "엘리트 3.5차", "엘리트 4차"};
    TCHAR gender_filter_names[GENDER_FILTER_MAX][kbuffer_size] = { "성별 무관", "남성", "여성", "남성전용", "여성전용" };
    TCHAR class_filter_names[kTempClassTypeMax][kbuffer_size] = { "모든 클래스", "버서커", "드래곤 나이트", "섀도우", "발키리", "엘레멘탈", 
        #ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            "미스틱",
        #endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
            "헬로이드"
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            ,"위치블레이드"
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    };
    TCHAR etc_filter_names[ETC_FILTER_MAX][kbuffer_size] = { "일반 아이템", "퀘스트 아이템", "AC 아이템", "인첸트/조합", "쟈드 아이템", "캐쉬 아이템", "펫 아이템", "탈것 아이템", "캐릭터 관련", "랜덤화아이템", "복권", "코스튬" };

    switch (filter)
    {
    case FILTER_ALL:
        if (item_option_parametas_.class_type != eCHAR_NONE)
        {
            sub_value = item_option_parametas_.class_type;
            Sprintf(caption_string, class_filter_names[sub_value]);
        } 
        else
        {
            Sprintf(caption_string, "필터 없음");
        }
        break;
    case FILTER_EQUIP:
        Sprintf(caption_string, equip_filter_names[sub_value]);
        break;
    case FILTER_WASTE:
        Sprintf(caption_string, waste_filter_names[sub_value]);
        break;
    case FILTER_GRADE:
        Sprintf(caption_string, grade_filter_names[sub_value]);
        break;
    case FILTER_GENDER:
        Sprintf(caption_string, gender_filter_names[sub_value]);
        break;
    case FILTER_ETC:
        Sprintf(caption_string, etc_filter_names[sub_value]);
        break;
    default:
        Sprintf(caption_string, "Unknown Sub Filter");
        break;
    }
}

void uiGMItemOptionWindow::SetSearch(bool value)
{
    if (value == false && ui_gm_window_manager_->IsSearching() == false)
    {
        is_search_ = false;
        if (ok_control_ && IsChange())
        {
            ok_control_->EnableWindowWZ(true);
        }
    }
}

bool uiGMItemOptionWindow::IsChange()
{
    if (ui_gm_window_manager_->IsSearching() == true)
    {
        return false;
    }

    ITEM_PARAMETAS* old_item_option_parametas;
    if (ui_gm_window_manager_ != NULL)
    {
        old_item_option_parametas = ui_gm_window_manager_->item_parametas();
    }

    if (old_item_option_parametas->item_count != item_option_parametas_.item_count)
    {
        return true;
    }
    if (old_item_option_parametas->overlapping != item_option_parametas_.overlapping)
    {
        return true;
    }
    if (old_item_option_parametas->item_type != item_option_parametas_.item_type)
    {
        return true;
    }
    if (old_item_option_parametas->enchant_level != item_option_parametas_.enchant_level)
    {
        return true;
    }
    if (old_item_option_parametas->socket_count != item_option_parametas_.socket_count)
    {
        return true;
    }
	if (old_item_option_parametas->awakening_level != item_option_parametas_.awakening_level)
	{
		return true;
	}
    BYTE filter_main = static_cast<BYTE>(item_option_parametas_.filter_main);
    if (old_item_option_parametas->filter_main != filter_main)
    {
        return true;
    }
    if (old_item_option_parametas->filter_subs[filter_main] != item_option_parametas_.filter_subs[filter_main])
    {
        return true;
    }
    if (old_item_option_parametas->class_type != item_option_parametas_.class_type)
    {
        return true;
    }
    return false;
}
#endif//_DEV_VER