#include "SunClientPrecompiledHeader.h"

#include "GlobalFunc.h"
#include "uiQuestList/uiQuestList.h"
#include "uiQuestMan/uiQuestMan_def.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
//------------------------------------------------------------------------------
uiQuestList::uiQuestList(InterfaceManager * pUIMan) : uiBase(pUIMan)
{
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("L003"), kControl_List_L003);
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckButton_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckButton_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckButton_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckButton_C003);

    for (int j= 0; j < kList_Max; ++j)
    {
        list_controls_[j] = NULL;
    }

    for (int i = 0; i < kButton_Max; ++i)
    {
        buttons_[i] = NULL;
        buttons_index[i] = 0;
        checkbuttons_[i] = NULL;
    }
    list_position_ = 0;
    last_list_position_ = 0;
}

uiQuestList::~uiQuestList()
{
    list_position_ = 0;
    last_list_position_ = 0;
    for (int i = 0; i < kButton_Max; ++i)
    {
        buttons_index[i] = 0;
    }

    if (GetDialogWZ() != NULL)
    {
        STD_UI_INTERSECT_RECT& intersect_list = GetDialogWZ()->GetIntersectRectList();
        intersect_list.clear();
    }

	
}

void uiQuestList::Init(CDrawBase* draw_base)
{
    list_controls_[kList_Background] = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L002)));
    list_controls_[kList_Text] = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L000)));
    list_controls_[kList_State_Background] = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L003)));
    list_controls_[kList_State] = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_List_L001)));

    buttons_[kRed] = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B000)));
    buttons_[kGreen] = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B003)));
    buttons_[kBlue] = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B004)));
    buttons_[kPurple] = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B006)));

    checkbuttons_[kRed] = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_CheckButton_C000)));
    checkbuttons_[kGreen] = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_CheckButton_C001)));
    checkbuttons_[kBlue] = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_CheckButton_C002)));
    checkbuttons_[kPurple] = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(kControl_CheckButton_C003)));

    for (int i = 0; i < kButton_Max; ++i)
    {
        if (buttons_[i])
        {
            buttons_rect[i] = buttons_[i]->GetSizeRect();
        }
        else
        {
            assert(!"Can't load button control from 83_12_QuestList.iwz for uiQuestList.cpp");
        }
        if (checkbuttons_[i])
        {
            checkbuttons_rect[i] = checkbuttons_[i]->GetSizeRect();
        }
        else
        {
            assert(!"Can't load check button control from 83_12_QuestList.iwz for uiQuestList.cpp");
        }
    }

   CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
        intersect_list.clear();
    }
}
//------------------------------------------------------------------------------
void uiQuestList::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    //if (GetManager() == NULL)
      //  return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------
bool uiQuestList::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));
    int quest_number = -1;
    switch (position)
    {
    case kControl_Button_B000:
        {
            quest_number = 0;
        }
        break;
    case kControl_Button_B001:
        {
            ScrollUp();
        }
        break;
    case kControl_Button_B002:
        {
            ScrollDown();
        }
        break;
    case kControl_Button_B003:
        {
            quest_number = 1;
        }
        break;
    case kControl_Button_B004:
        {
            quest_number = 2;
        }
        break;
    case kControl_Button_B006:
        {
            quest_number = 3;
        }
        break;
    case kControl_List_L000:
        {
        }
        break;
    default:
        {
        }
        break;
    }

    if (quest_number >= kRed && quest_number < kButton_Max)
    {
        uiQuestListDlg* quest_list_dialog = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
        if (quest_list_dialog)
        {
            quest_list_dialog->OpenQuest(quest_codes_[quest_number]);
        }
    }
    return result;
}

void uiQuestList::OnShowWindow(BOOL val)
{
    if (val)
    {
        SetQuestList();
    }
    else
    {
        //
    }
}

const DWORD c_ListSelBarColor	= RGBA(255,255,255,0);
const float c_line_height = 20.f;
void uiQuestList::SetQuestList()
{
    if (list_controls_[kList_Background] == NULL || list_controls_[kList_Text] == NULL ||
        list_controls_[kList_State] == NULL)
    {
        return;
    }

    int row = 0;

    for (int list_index = kList_Text; list_index < kList_Max; ++list_index)
    {
        list_controls_[list_index]->DeleteItemAll();
        list_controls_[list_index]->SetCurSel(-1);
        list_controls_[list_index]->SetColorSelBar(c_ListSelBarColor);
        list_controls_[list_index]->SetUseReport(false);
        list_controls_[list_index]->SetReportUseCnt(0);
        list_controls_[list_index]->SetSellHeight(static_cast<int>(c_line_height));
        list_controls_[list_index]->SetBeginDrawIndex(0);

        if ((list_index == kList_Text) || (list_index == kList_Background))
        {
            //긴 문자열인 경우 "..." 표시하기
            list_controls_[list_index]->SetTextReducing(true);
            list_controls_[list_index]->SetStartLineGapLeft(-1); // 왼쪽에 1픽셀이 잘리는 이유를 모르겠음. 일단 임시 조치
        }
    }
    //int list_size = battlezone_list_.size();

    int draw_line_count = list_controls_[kList_Text]->GetDrawRowCount();

    TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

    //=====================================================================================
    //	1.리스트컨트롤 데이터삽입
    //=====================================================================================
    DWORD font_color = RGBA(230,230,230,255);

    int index = 0;

    uiQuestMan* quest_man_ptr = static_cast<uiQuestMan *>(this->GetUIMan()->GetUserInterfaceManager(UIMAN_QUEST));
    
    if (quest_man_ptr == NULL)
    {
        return;
    }
    std::vector<sQuestPreview*> quest_vector_ptr = quest_man_ptr->get_quest_prog_content_map();

    for (int i = kRed; i < kButton_Max; ++i)
    {
        buttons_[i]->ShowWindowWZ(WZ_HIDE);
		checkbuttons_[i]->ShowWindowWZ(WZ_HIDE);
    }

    std::vector<sQuestPreview*>::iterator begin = quest_vector_ptr.begin();
    std::vector<sQuestPreview*>::iterator end = quest_vector_ptr.end();

    TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
    DWORD color_number = 4;// 4 = TEXT_COLOR_QUEST_FOR_PREVIEW_CONTENT

    RECT dialog_rect = {0,};
    GetDialogWZ()->GetClientRect(&dialog_rect);

    STD_UI_INTERSECT_RECT& intersect_list = this->GetDialogWZ()->GetIntersectRectList();
    intersect_list.clear();

    RECT enable_rect = {0,};
    for (int control_number = kControl_Button_B001; control_number <= kControl_Button_B002; ++control_number)
    {
        CCtrlButtonWZ* button_control_ptr = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(control_number)));
        if (button_control_ptr)
        {
            enable_rect = button_control_ptr->GetSizeRect();
            intersect_list.push_back(enable_rect);
        }
    }    

    DWORD colors[] = { WzColor_RGBA(254,65,61,255), WzColor_RGBA(110,225,74,255), 
        WzColor_RGBA(32,163,254,255), WzColor_RGBA(155,108,248,255), WzColor_RGBA(230,230,230,255),
        WzColor_RGBA(0,0,0,255), WzColor_RGBA(130,130,130,255)};

    DWORD title_state = TEXT_COLOR_QUEST_FOR_PREVIEW_NAME;
	bool stateCheck_first = false;
    for (; begin != end; ++begin)
    {
        sprintf(message, "");
        sQuestPreview* data = (*begin);

		eQuestPreviewType stateCheck_Title = eQUEST_PREVIEW_NONE;
        if (data->m_eType == eQUEST_PREVIEW_TITLE)
        {
			stateCheck_Title = eQUEST_PREVIEW_TITLE;
            title_state = data->state;
            index = list_controls_[kList_Text]->InsertItemBack();
            uiQuestListDlg* quest_list_dialog = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
            if (quest_list_dialog)
            {
                for (int i = 0; i < MAX_NAVI_CNT; ++i)
                {
                    if (quest_list_dialog->GetNaviQCodes(i) == data->quest_code)
                    {
                        color_number = i;
                        quest_codes_[i] = data->quest_code;
                        buttons_index[i] = index;

                        if (draw_line_count >=  index)
                        {
                            RECT button_rect = buttons_rect[kRed];
                            RECT check_rect = checkbuttons_rect[kRed];
                            buttons_[i]->MoveWindowWZ(
                                (dialog_rect.left + button_rect.left)*1.f, 
                                (dialog_rect.top + button_rect.top) + (index * c_line_height),
                                (button_rect.right-button_rect.left)*1.f,
                                (button_rect.bottom-button_rect.top)*1.f);
                            checkbuttons_[i]->MoveWindowWZ(
                                (dialog_rect.left + check_rect.left)*1.f,
                                (dialog_rect.top + check_rect.top) + (index * c_line_height),
                                (check_rect.right-check_rect.left)*1.f,
                                (check_rect.bottom-check_rect.top)*1.f);
                            buttons_[i]->ShowWindowWZ(WZ_SHOW);  
                            checkbuttons_[i]->ShowWindowWZ(WZ_SHOW);

                            enable_rect.left = (dialog_rect.left + button_rect.left);
                            enable_rect.top = (dialog_rect.top + button_rect.top) + static_cast<LONG>(index * c_line_height);
                            enable_rect.right = (enable_rect.left + button_rect.right - button_rect.left);
                            enable_rect.bottom = (enable_rect.top + button_rect.bottom - button_rect.top);
                            intersect_list.push_back(enable_rect);

                            if (checkbuttons_[i])
                            {
                                checkbuttons_[i]->SetCheckState(false);
                                checkbuttons_[i]->EnableWindowWZ(true);
                                switch (data->state)
                                {
                                case QS_COMPLETED: //TEXT_COLOR_QUEST_FOR_PREVIEW_COMPLETE
                                    {
                                        //9051	(완료)
                                        //g_InterfaceManager.GetInterfaceString(9051, message);
                                        checkbuttons_[i]->SetCheckState(true);
										stateCheck_first = true;
                                    }
                                    break;
                                case QS_FAILED: //TEXT_COLOR_QUEST_LIST_FAILED
                                    {
                                        // (실패) ( 9052 )
                                        //g_InterfaceManager.GetInterfaceString(9052, message); 
                                        checkbuttons_[i]->EnableWindowWZ(false);
										stateCheck_first = false;
                                    }
                                    break;
                                case QS_NORMAL: //TEXT_COLOR_QUEST_FOR_PREVIEW_NAME
                                    {
                                        //sprintf(message, "");
										stateCheck_first = false;
                                    }
                                    break;
                                }
                            }
                        }
                        break; //for
                    }
                }                 
            }
        }
        else
        {
            color_number = 4;
			stateCheck_Title = eQUEST_PREVIEW_MISSION;
        }
        strncpy(string, data->m_strContent.c_str(), INTERFACE_STRING_LENGTH);

        char* find_ptr = NULL;
        char* start_ptr = NULL;

        find_ptr = strstr(string, "(");
        while (find_ptr && (start_ptr = strstr(find_ptr+1, "(")))
        {
            find_ptr = start_ptr;
        }
        start_ptr = find_ptr;

        // (first_number / second_number)
        bool first_number = false;
        bool is_slash = false;
        bool second_number = false;
        bool is_end = false;
        BYTE complete_number = 0;
        BYTE need_number = 255;

        while (find_ptr != NULL && *find_ptr != '\0')
        {
            if (*find_ptr == ')')
            {
                is_end = true;
                break;
            }
            if (*find_ptr == '/')
            {
                is_slash = true;
            }
            if (is_slash == false)
            {
                if (first_number == false && '0' <= (*find_ptr) && '9' >= (*find_ptr))
                {
                    first_number = true;
                    complete_number = atoi(find_ptr);
                }
            }
            else
            {
                if ('0' <= (*find_ptr) && '9' >= (*find_ptr))
                {
                    second_number = true;
                    need_number = atoi(find_ptr);
                    break;
                }
            }
            ++find_ptr;
        }

        if (start_ptr && first_number && is_slash && second_number)
        {
            strncpy(message, start_ptr, INTERFACE_STRING_LENGTH);
            *start_ptr = '\0';
        }

        if (start_ptr && !first_number)
        {
            strncmp(start_ptr, message, INTERFACE_STRING_LENGTH);
        }

        WzColor title_color = data->m_wcColor;
        if (complete_number >= need_number)
        {
            //continue;//리스트에 넣지 않는다.
            color_number = 6;//회색화
            title_color = WzColor_RGBA(130, 130, 130, 255);
        }

		//(10/10)의 경우 같이 숫자 단위가 표시 되지 않는 경우가 있다!
		// 그경우 보여지지 않는 경우가 있음으로 완료표시를 따로 구분해야된다.

		if (stateCheck_Title == eQUEST_PREVIEW_MISSION)
		{
			if ( stateCheck_first && find_ptr == NULL)
			{
				color_number = 6;//회색화
				title_color = WzColor_RGBA(130, 130, 130, 255);
			}
		}
		
 

        if (data->m_eType != eQUEST_PREVIEW_TITLE)
        {
            index = list_controls_[kList_Text]->InsertItemBack();
        }
        list_controls_[kList_Text]->SetItemData(index, (void*)&(*begin));
        list_controls_[kList_Background]->InsertItemBack();
        list_controls_[kList_Background]->SetItemData(index, (void*)&(*begin));
        list_controls_[kList_State]->InsertItemBack();
        list_controls_[kList_State_Background]->InsertItemBack();

        GlobalFunc::SetItem(list_controls_[kList_State], index, kQuest_List_Title,
            message,
            StrToWzID("m212"),
            DT_CENTER,
            RGBA(255,0,0,0),
            RGBA(Red_WzColor(title_color), Green_WzColor(title_color), Blue_WzColor(title_color), Alpha_WzColor(title_color)),
            RGBA(255,0,0,0)
            );
        GlobalFunc::SetItem(list_controls_[kList_State_Background], index, kQuest_List_Title,
            message,
            StrToWzID("m212"),
            DT_CENTER,
            RGBA(255,0,0,0),
            RGBA(Red_WzColor(colors[5]), Green_WzColor(colors[5]), Blue_WzColor(colors[5]), Alpha_WzColor(colors[5])),
            RGBA(255,0,0,0)
            );

        GlobalFunc::SetItem(list_controls_[kList_Text], index, kQuest_List_Title,
            string,
            StrToWzID("m212"),
            DT_RIGHT,
            RGBA(255,0,0,0),
            RGBA(Red_WzColor(colors[color_number]), Green_WzColor(colors[color_number]), Blue_WzColor(colors[color_number]), Alpha_WzColor(colors[color_number])),
            RGBA(255,0,0,0)
            );
        GlobalFunc::SetItem(list_controls_[kList_Background], index, kQuest_List_Title,
            string,
            StrToWzID("m212"),
            DT_RIGHT,
            RGBA(255,0,0,0),
            RGBA(Red_WzColor(colors[5]), Green_WzColor(colors[5]), Blue_WzColor(colors[5]), Alpha_WzColor(colors[5])),
            RGBA(255,0,0,0)
            );
    }
#if WZENGINEVER >= 400
    int max_line_count = list_controls_[kList_Text]->GetListRowCnt();
    //int draw_line_count = list_controls_[kList_Text]->GetDrawRowCount();
    draw_line_count = list_controls_[kList_Text]->GetDrawRowCount();
    int scroll_max_num = max(0, max_line_count - draw_line_count);
    //VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);

    CCtrlButtonWZ* scroll_up = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B001)));
    CCtrlButtonWZ* scroll_down = static_cast<CCtrlButtonWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_B002)));
    if (scroll_up && scroll_down)
    {
        if (scroll_max_num <= 1)
        {
            //스클로바 숨김
            scroll_up->ShowWindowWZ(WZ_HIDE);
            scroll_down->ShowWindowWZ(WZ_HIDE);
        }
        else
        {
            scroll_up->ShowWindowWZ(WZ_SHOW);
            scroll_down->ShowWindowWZ(WZ_SHOW);
        }
    }
#endif //#if WZENGINEVER >= 400
}

void uiQuestList::ScrollUp()
{
    if (list_controls_[kList_Background] == NULL || list_controls_[kList_Text] == NULL ||
        list_controls_[kList_State] == NULL)
    {
        return;
    }

	STD_UI_INTERSECT_RECT& intersect_list = this->GetDialogWZ()->GetIntersectRectList();
	intersect_list.clear();

	CControlWZ* bt01 = GetControlWZ(GetControlID_byPosition(kControl_Button_B001));
	if(bt01)
	{
		intersect_list.push_back(bt01->GetSizeRect());
	}
	CControlWZ* bt02 = GetControlWZ(GetControlID_byPosition(kControl_Button_B002));
	if(bt02)
	{
		intersect_list.push_back(bt02->GetSizeRect());
	}

    --last_list_position_;
    int max_line_count = list_controls_[kList_Text]->GetListRowCnt();
    int draw_line_count = list_controls_[kList_Text]->GetDrawRowCount();
    int scroll_max_num = max(0, max_line_count - draw_line_count);

    if (last_list_position_ < 0)
    {
		for (int i = 0; i < kButton_Max; ++i)
		{
			if(buttons_[i]->GetState() == WZ_SHOW)
			{
				intersect_list.push_back(buttons_[i]->GetSizeRect());
			}
		}
        last_list_position_ = 0;
    }
    else
    {
        RECT dialog_rect = {0,};
        RECT enable_rect = {0,};
        GetDialogWZ()->GetClientRect(&dialog_rect);

        /*STD_UI_INTERSECT_RECT& intersect_list = this->GetDialogWZ()->GetIntersectRectList();
        intersect_list.clear();*/

        for (int i = 0; i < MAX_NAVI_CNT; ++i)
        {
            int index = buttons_index[i];
            index = buttons_index[i] - last_list_position_;

            RECT button_rect = buttons_rect[kRed];
            RECT check_rect = checkbuttons_rect[kRed];

            if (buttons_[i] == NULL || checkbuttons_[i] == NULL)
            {
                assert(!"uiQuestList.cpp::Scrollup() -> control is null");
                continue;
            }

            buttons_[i]->MoveWindowWZ(
                (dialog_rect.left + button_rect.left)*1.f, 
                (dialog_rect.top + button_rect.top) + (index * c_line_height),
                (button_rect.right-button_rect.left)*1.f,
                (button_rect.bottom-button_rect.top)*1.f);
            checkbuttons_[i]->MoveWindowWZ(
                (dialog_rect.left + check_rect.left)*1.f,
                (dialog_rect.top + check_rect.top) + (index * c_line_height),
                (check_rect.right-check_rect.left)*1.f,
                (check_rect.bottom-check_rect.top)*1.f);

            if (index >= 0 && index < draw_line_count)
            {
                buttons_[i]->ShowWindowWZ(WZ_SHOW);   
                checkbuttons_[i]->ShowWindowWZ(WZ_SHOW);
            }
            else
            {
                buttons_[i]->ShowWindowWZ(WZ_HIDE);                       
                checkbuttons_[i]->ShowWindowWZ(WZ_HIDE);
            }

			if(buttons_[i]->GetState() == WZ_SHOW)
			{
				intersect_list.push_back(buttons_[i]->GetSizeRect());
			}
        }
    }

    for (int list_index = kList_Text; list_index < kList_Max; ++list_index)
    {
        list_controls_[list_index]->SetBeginDrawIndex(last_list_position_);
    }
}

void uiQuestList::ScrollDown()
{
    if (list_controls_[kList_Background] == NULL || list_controls_[kList_Text] == NULL ||
        list_controls_[kList_State] == NULL)
    {
        return;
    }

	STD_UI_INTERSECT_RECT& intersect_list = this->GetDialogWZ()->GetIntersectRectList();
	intersect_list.clear();

	CControlWZ* bt01 = GetControlWZ(GetControlID_byPosition(kControl_Button_B001));
	if(bt01)
	{
		intersect_list.push_back(bt01->GetSizeRect());
	}
	CControlWZ* bt02 = GetControlWZ(GetControlID_byPosition(kControl_Button_B002));
	if(bt02)
	{
		intersect_list.push_back(bt02->GetSizeRect());
	}

    ++last_list_position_;

    int max_line_count = list_controls_[kList_Text]->GetListRowCnt();
    int draw_line_count = list_controls_[kList_Text]->GetDrawRowCount();
    int scroll_max_num = max(0, max_line_count - draw_line_count);

    if (last_list_position_ >= scroll_max_num)
    {
        last_list_position_ = scroll_max_num - 1;
    }    

    RECT dialog_rect = {0,};
    RECT enable_rect = {0,};
    GetDialogWZ()->GetClientRect(&dialog_rect);
    for (int i = 0; i < MAX_NAVI_CNT; ++i)
    {
        int index = buttons_index[i];
        index = buttons_index[i] - last_list_position_;
        RECT button_rect = buttons_rect[kRed];
        RECT check_rect = checkbuttons_rect[kRed];

        if (buttons_[i] == NULL || checkbuttons_[i] == NULL)
        {
            assert(!"uiQuestList.cpp::Scrollup() -> control is null");
            continue;
        }

        buttons_[i]->MoveWindowWZ(
            (dialog_rect.left + button_rect.left)*1.f, 
            (dialog_rect.top + button_rect.top) + (index * c_line_height),
            (button_rect.right-button_rect.left)*1.f,
            (button_rect.bottom-button_rect.top)*1.f);

        checkbuttons_[i]->MoveWindowWZ(
            (dialog_rect.left + check_rect.left)*1.f,
            (dialog_rect.top + check_rect.top) + (index * c_line_height),
            (check_rect.right-check_rect.left)*1.f,
            (check_rect.bottom-check_rect.top)*1.f);

        if (index >= 0 && index < draw_line_count)
        {
            buttons_[i]->ShowWindowWZ(WZ_SHOW);   
            checkbuttons_[i]->ShowWindowWZ(WZ_SHOW);
        }
        else
        {
            buttons_[i]->ShowWindowWZ(WZ_HIDE);                       
            checkbuttons_[i]->ShowWindowWZ(WZ_HIDE);
        }

		if(buttons_[i]->GetState() == WZ_SHOW)
		{
			intersect_list.push_back(buttons_[i]->GetSizeRect());
		}

    }

    for (int list_index = kList_Text; list_index < kList_Max; ++list_index)
    {
        list_controls_[list_index]->SetBeginDrawIndex(last_list_position_);
    }
}
/*void uiQuestList::OnUpdateSolarDialog()
{
    //움직일 일이 없으니 한번만 하는게 옳음, 버튼들을 일회성으로 이동
}*/
#endif //_NA_004644_20110506_QUEST_NAVIGATION