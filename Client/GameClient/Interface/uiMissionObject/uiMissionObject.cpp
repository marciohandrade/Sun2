#include "SunClientPrecompiledHeader.h"


#include "InterfaceManager.h"
#include "uiMissionObject/uiMissionObject.h"
#include "uiMissionObjectMan/uiMissionObjectMan.h"
#include "GlobalFunc.h"
#include "MissionInfoParser.h"
#include "MissionTextInfoParser.h"
#include "Mouse.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "../SoundEffect.h"

const DWORD MISSION_MAPCODE_RANGE = 10000; // 미션 맵 코드 범위, 만약 미션 외에도 목적 리스트를 사용하고 싶다면 변경 해야 합니다.
const DWORD IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT = StrToWzID("0504");

WzID uiMissionObject::wz_control_id_[DLGPOS_MAX] = 
{
    StrToWzID("L001"),  //eDIALOG_MISSION_OBJECT_LIST
    StrToWzID("B999"),  //eDIALOG_EXIT
    StrToWzID("VS00"),  //eDIALOG_VSCROLL
    StrToWzID("T000"),  //eDIALOG_TITTLE
    StrToWzID("B000"),  //eDIALOG_ALL_BUTTON,
    StrToWzID("B001"),  //eDIALOG_PLAY_BUTTON,
    StrToWzID("B002"),  //eDIALOG_COMPLETE_BUTTON,
};

uiMissionObject::uiMissionObject(InterfaceManager* ui_manager) : uiBase(ui_manager)
{
    vertical_scroll_ = NULL;
    mission_title_ = NULL;
    line_update_ = false; 
    tooltip_string_code = 0;
    mission_delay_tick_count_ = 0;
    reflash_line_convert_ = false;
}

uiMissionObject::~uiMissionObject()
{
    Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::Init(CDrawBase* draw_base)
{
	uiBase::Init(draw_base);	

	mission_object_list_ = (CCtrlListWZ*) get_control(eDIALOG_MISSION_OBJECT_LIST);  
	assert(mission_object_list_);

	mission_title_ = (CCtrlStaticWZ*) get_control(eDIALOG_TITTLE);
	assert(mission_title_);

    vertical_scroll_ = (CCtrlVScrollWZ*) get_control(eDIALOG_VSCROLL);
    assert(vertical_scroll_);

    for(int i = TYPE_ALL;  i < TYPE_BUTTON_MAX; ++i)
    {
        type_button_[i] = (CCtrlButtonWZ*) get_control(eDIALOG_ALL_BUTTON+i);
        assert(type_button_[i]);
    }
    name_code_ = 0;
    display_type_ = TYPE_ALL;
    ResetButton();

    SetMissionObjectMapCode(0,0);
    InitMissionList();
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::InitLineText()
{
    MissionTextInfo *line_string = NULL;
    // MissionText (1): <RETURN=1/>━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━<RETURN=1/>
    line_string = MissionTextInfoParser::Instance()->GetMissionTextInfo(1);
    if (!line_string || !line_string->mission_text)
    {
        sprintf(line_string_, "UnKnown Mission Text ID(1)");
    }
    else
    {
        sprintf(line_string_, line_string->mission_text);
    }
    line_string = MissionTextInfoParser::Instance()->GetMissionTextInfo(2);
    // MissionText (2): <COLOR=FF999999>%s(완료)</COLOR>%s
    if (!line_string || !line_string->mission_text)
    {
        sprintf(complete_string_, "UnKnown Mission Text ID(2)");
    }
    else
    {
        sprintf(complete_string_, line_string->mission_text);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::InitMissionList()
{
    if (!mission_object_list_)
    {
        assert(!"왜 리스트가 없어!!");
        return;
    }
    RECT list_rect = mission_object_list_->GetSizeRect();
    ftext_render_.Init(GetDrawBase(), list_rect);

    mission_object_list_->SetSellHeight(15);
    mission_object_list_->DeleteItemAll();

    vertical_scroll_ = (CCtrlVScrollWZ *)get_control(eDIALOG_VSCROLL);
    vertical_scroll_->SetScrollRangeWZ(0,0);
    vertical_scroll_->SetScrollPosWZ(0);

    if (mission_title_ != NULL)
    {
        TCHAR title_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        name_code_ = 0;
        // 2173: %s 미션 진행 정보, %s = ""
        g_InterfaceManager.GetInterfaceString(mission_title_->GetTextID(), temp_string);
        sprintf(title_string, temp_string, "");
        mission_title_->SetTextWZ(title_string);
    }
    _MissionObject* data = NULL;
    MISSION_OBJECT_LIST_ITR itr;
    MISSION_OBJECT_LIST_ITR end = mission_object_all_.end();

    if (mission_object_all_.size() > 0)
    {
        for(itr = mission_object_all_.begin(); itr != end; ++itr)
        {
            data = (*itr);
            delete data;
        }
    }
    mission_object_all_.clear();

    ftext_render_.SetMargin(0,0,0,-12);
    ftext_render_.SetGap(0,-2);
    ftext_render_.convertLineInfo();
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::SetMissionObjectMapCode(DWORD map_code, DWORD field_code)
{ 
    if (map_code_ != map_code)
    {
        if (mission_object_all_.size() > 0)
        {
            InitMissionList();//초기화
        }
        if (map_code < MISSION_MAPCODE_RANGE)
        {
            InitLineText();
            name_code_ = MissionInfoParser::Instance()->GetMissionInfo(&mission_object_all_, map_code);
            // 2173: %s 미션 진행 정보, %s = name_code
            if (mission_title_ != NULL)
            {
                TCHAR title_string[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
                TCHAR map_string[INTERFACE_STRING_LENGTH] = {0,};
                // 2173: %s 미션 진행 정보, %s = ""
                g_InterfaceManager.GetInterfaceString(mission_title_->GetTextID(), temp_string);
                g_InterfaceManager.GetInterfaceString(name_code_, map_string);
                sprintf(title_string, temp_string, map_string);
                mission_title_->SetTextWZ(title_string);
            }
        }
    }
    map_code_ = map_code; 
    field_code_ = field_code; 
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::Release()
{
    _MissionObject* data = NULL;
    MISSION_OBJECT_LIST_ITR itr;
    MISSION_OBJECT_LIST_ITR end = mission_object_all_.end();

    if (mission_object_all_.size() > 0)
    {
        for(itr = mission_object_all_.begin(); itr != end; ++itr)
        {
            data = (*itr);
            delete data;
        }
    }
    mission_object_all_.clear();
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::OnUpdateSolarDialog()
{
    ftext_render_.SetRect(mission_object_list_->GetSizeRect());
    updateMouseWheel();
    UpdateToolTip();
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::OnRenderSolarDialog()
{
    if (reflash_line_convert_)
    {
        ftext_render_.SetRect(mission_object_list_->GetSizeRect());
        ftext_render_.convertLineInfo();
        reflash_line_convert_ = false;
    }
    ftext_render_.Render();

    if (line_update_)
    {
        line_update_ = false;
        ftext_render_.SetBeginLineIndex(ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect());
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::Process(DWORD tick_count)
{
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::OnShowWindow(BOOL open_value)
{
    SolarDialog::OnShowWindow(open_value);
	if(open_value)
	{
        MissionDataReflash();
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType=eDoType_dialog_mission_object;
        msg.wParam=IM_MISSION_OBJECT_MANAGER::MISSION_OBJECT;
        msg.DoSomething=GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
	}
	else
	{
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_mission_object);
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::UpdateToolTip()
{
    int iMouseX = Mouse::Instance()->GetMouseX();
    int iMouseY = Mouse::Instance()->GetMouseY();

    // 스킵처리 
    // 인터페이스에서 Dialog 바탕에서는 Mouse Over가 오지 않는다.
    bool is_mouse_over = false;
    int loop_end = DLGPOS_MAX;
    for (POSTYPE position = 0; position < loop_end; ++position)
    {
        CControlWZ* control = get_control(position);
        if(control)
        {
            RECT control_rect = control->GetSizeRect();
            if (control_rect.left <= iMouseX && control_rect.right >= iMouseX)
            {
                if (control_rect.top <= iMouseY && control_rect.bottom >= iMouseY)
                {
                    is_mouse_over = true;
                    break;
                }
            }
        }
    }

    if (!is_mouse_over)
    {
        tooltip_string_code = 0;
        m_MouseOverObject.eMouseOverType = eMOUSE_OVER_MAX;
    }
    else
    {
        if (tooltip_string_code > 0)
        {
            uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            if (tooltip_manager)
            {
                tooltip_manager->RegisterTooltipForWareNInventoryLock(tooltip_string_code);
				tooltip_manager->SetTooltipDialogID(GetDialogKey());

            }
        }
    }
    int b = ftext_render_.GetBeginLineIndex();
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::MissionDataReflash()
{
    MISSION_OBJECT_LIST *current_list = NULL;

    ftext_render_.SetVCenterAlign(false);
    ftext_render_.ClearSentence();
    current_list = &mission_object_all_;

    int complete_count = 0;
    int process_count = 0;
    int total_count = 0;

    _MissionObject* data = NULL;
    MISSION_OBJECT_LIST_ITR itr;
    MISSION_OBJECT_LIST_ITR end = current_list->end();
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    bool push_data = false;  // 데이터 삽입 여부
    MissionTextInfo *mission_text_infomation = NULL;

    for(itr = current_list->begin(); itr != end; ++itr)
    {
        ftext_parser_.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);  //ARGB

        data = (*itr);
        mission_text_infomation = MissionTextInfoParser::Instance()->GetMissionTextInfo(data->mission_text_code);

        if (!mission_text_infomation || !mission_text_infomation->mission_text)
        {
            return;				
        }

        if (data->is_show == false)
        {
            continue;// 보이는 상태가 아니면 스킵
        }
        push_data = false;
        switch(display_type_)
        {
        case TYPE_ALL:
            push_data = true;
            break;
        case TYPE_PLAYING:
            if (data->is_complete == false)
            {
                push_data = true;
            }
            break;
        case TYPE_COMPLETE:
            if (data->is_complete == true)
            {
                push_data = true;
            }
            break;
        }

        if (data->is_complete == true)
        {
            //ftext_render_.SetAlphaValue(128);
            sprintf(temp_string, "<COLOR=FF999999>%s</COLOR>%s%s", 
                mission_text_infomation->mission_text, complete_string_, line_string_);
            ++complete_count;
        }
        else
        {
            sprintf(temp_string, "%s%s", mission_text_infomation->mission_text, line_string_);
            ++process_count;
        }
        ++total_count;

        if (push_data == true)
        {
            AddFormatText(temp_string);
        }
    }
    ftext_render_.convertLineInfo();
    if (vertical_scroll_ != NULL)
    {
        int scroll_range = ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect();
        vertical_scroll_->SetScrollRangeWZ(0,scroll_range);
        vertical_scroll_->SetScrollPosWZ(scroll_range);
    }

    if (mission_title_ != NULL && name_code_ != 0)
    {
        TCHAR title_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR map_string[INTERFACE_STRING_LENGTH] = {0,};
        // 2173: %s 미션 진행 정보, %s = ""
        g_InterfaceManager.GetInterfaceString(mission_title_->GetTextID(), temp_string);
        g_InterfaceManager.GetInterfaceString(name_code_, map_string);
        sprintf(title_string, temp_string, map_string);
        switch(display_type_)
        {
        case TYPE_ALL:
            sprintf(temp_string, "%s(%d/%d)", title_string, complete_count, total_count);
            break;
        case TYPE_PLAYING:
            sprintf(temp_string, "%s(%d/%d)", title_string, process_count, total_count);
            break;
        case TYPE_COMPLETE:
            sprintf(temp_string, "%s(%d/%d)", title_string, complete_count, total_count);
        default:
            mission_title_->SetTextWZ(title_string);
        }
        mission_title_->SetTextWZ(temp_string);
    }
}
//------------------------------------------------------------------------------
/**
*/
bool uiMissionObject::MissionObjectComplete(DWORD trigger_id)
{
    bool is_new_change = false;
    MISSION_OBJECT_LIST *current_list = &mission_object_all_;
    _MissionObject* data = NULL;
    MISSION_OBJECT_LIST_ITR itr;
    MISSION_OBJECT_LIST_ITR end = current_list->end();
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

    if (current_list->size() <= 0)
    {
        return false;
    }
    for(itr = current_list->begin(); itr != end; ++itr)
    {
        data = (*itr);
        if(data->is_complete == true)
        {
            continue;
        }
        else if (data->complete_field_code == field_code_ && data->complete_trigger_id == trigger_id)// 미완료 리스트에서 찾기
        {
            data->is_complete = true;
            is_new_change = true;
        }
    }

    if (is_new_change)
    {
        MissionNotice();    // 미션 변경 알림 공지
    }
    return is_new_change;
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::MissionNotice()
{
    DWORD current_tick = clock_function::GetTickCount();
    if (current_tick >= mission_delay_tick_count_)
    {
        mission_delay_tick_count_ = current_tick + MISSION_OBJECT_DELAY;
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        // 미션 진행 정보가 갱신되었습니다. "O"키를 눌러 확인하실 수 있습니다.
        g_InterfaceManager.GetInterfaceString(2177, temp_string, INTERFACE_STRING_LENGTH);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, temp_string);
        // 사운드 출력
        g_SoundEffect.PlayUI(UI_SOUND_MISSION_OBJECT);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::CheckMissionObjectShow(DWORD trigger_id)
{
    bool is_new_change = false;
    bool is_complete_change = false;
    _MissionObject* data = NULL;
    MISSION_OBJECT_LIST_ITR itr;
    MISSION_OBJECT_LIST_ITR end = mission_object_all_.end();

    if (mission_object_all_.size() <= 0)
    {
        return;
    }
    for(itr = mission_object_all_.begin(); itr != end; ++itr)
    {
        data = (*itr);
        if (data->is_show == false)
        {
            data->show_count = 0;
            for (int i = 0; i < _MissionObject::MAX_OPEN_TRIGGER_ID; ++i)
            {
                if (data->field_code != field_code_)
                {
                    continue;
                }

                if (data->open_trigger_id[i] == 0)
                {
                    data->show_count = data->show_count + 1;
                }
                else if (data->open_trigger_id[i] == trigger_id)
                {
                    data->open_trigger_id[i] = 0;
                    data->show_count = data->show_count + 1;
                }

                if (data->show_count >= _MissionObject::MAX_OPEN_TRIGGER_ID)
                {
                    data->is_show = true;
                    is_new_change = true;
                }
            }
        }
    }

    if (is_new_change)
    {
        MissionNotice(); // 미션 알림 공지
    }
    is_complete_change= MissionObjectComplete(trigger_id);
    
    if (is_new_change || is_complete_change)
    {
        MissionDataReflash();// 갱신하고 리턴
    }
    line_update_ = true;
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::ResetButton()
{
    for (int i = TYPE_ALL; i < TYPE_BUTTON_MAX; ++i)
    {
        type_button_[i]->ShowWindowWZ(WZ_HIDE);
    }
    switch(display_type_)
    {
    case TYPE_ALL:
        if (type_button_[TYPE_ALL])
        {
            type_button_[TYPE_ALL]->ShowWindowWZ(WZ_SHOW);
        }
        break;
    case TYPE_PLAYING:
        if (type_button_[TYPE_PLAYING])
        {
            type_button_[TYPE_PLAYING]->ShowWindowWZ(WZ_SHOW);
        }
        break;
    case TYPE_COMPLETE:
        if (type_button_[TYPE_COMPLETE])
        {
            type_button_[TYPE_COMPLETE]->ShowWindowWZ(WZ_SHOW);
        }
        break;
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::MessageProc(SI_MESSAGE* message)
{
	switch (message->eResultMsg)
	{
	case RT_MSG_LBUTTONCLICK:
		{
			switch (get_control_id_to_pos(message->dwCtrlID))
			{
			case eDIALOG_EXIT:
				{
                    this->ShowInterface(FALSE);
				}
				break;
            case eDIALOG_ALL_BUTTON:
            case eDIALOG_PLAY_BUTTON:
            case eDIALOG_COMPLETE_BUTTON:
                {
                    ++display_type_;
                    if (display_type_ > TYPE_COMPLETE)
                    {
                        display_type_ = TYPE_ALL;
                    }
                    ResetButton();
                    MissionDataReflash();
                }
                break;
			}
		}
		break;
    case RT_MSG_SB_LINETOP:
        {
            //.윗쪽 화살표를 눌렀다.
            //
            if (eDIALOG_VSCROLL == get_control_id_to_pos(message->dwCtrlID))
            {
                int index = vertical_scroll_->GetScrollPosWZ();
                --index;
                index = max(0, index);
                ftext_render_.SetBeginLineIndex( index );
                vertical_scroll_->SetScrollPosWZ( index );
            }
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            //.아랫쪽 화살표를 눌렀다
            if (eDIALOG_VSCROLL == get_control_id_to_pos(message->dwCtrlID))
            {
                int index = vertical_scroll_->GetScrollPosWZ();
                ++index;
                index = min((int)ftext_render_.GetMaxLine(), index);
                ftext_render_.SetBeginLineIndex( index );
                vertical_scroll_->SetScrollPosWZ( index );
            }
        }
        break;
    case RT_MSG_SB_THUMBPOSITION:			//. 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
    case RT_MSG_SB_THUMBTRACK:
        {
            //.썸을 드래그 하고 있다.
            if (eDIALOG_VSCROLL == get_control_id_to_pos(message->dwCtrlID))
            {
                int index = vertical_scroll_->GetScrollPosWZ();
                ftext_render_.SetBeginLineIndex(index);
            }
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
	}
}
//------------------------------------------------------------------------------
/**
*/
void uiMissionObject::NetworkProc(MSG_BASE* message)
{
}
//----------------------------------------------------------------------------
/**
*/
bool uiMissionObject::AddFormatText(const TCHAR* message)
{
    // 정규화 텍스트 세팅
    static int posnumber = 0;

    TCHAR value_string[1024] = {0,};
    Snprintf(value_string, 1024 , _T("%s"), message);

    BOOL result = FALSE;
    result = ftext_parser_.Parser(value_string, ftext_sentence_);
    if(result)
    {
        ftext_render_.Append(ftext_sentence_);
    }
    //ftext_render_.SetBeginLineIndex(0);
    return true;
}

void uiMissionObject::updateMouseWheel()
{
    if(mission_object_list_ == NULL || vertical_scroll_ == NULL)
    {
        return;
    }

    RECT list_rect = mission_object_list_->GetSizeRect();
    switch (GetMouseWheelStatus(&list_rect))
    {
    case MOUSE_WHEEL_UP:
        {
            int index = vertical_scroll_->GetScrollPosWZ();
            --index;
            index = max(0, index);
            ftext_render_.SetBeginLineIndex(index);
            vertical_scroll_->SetScrollPosWZ(index);
        }
        break;
    case MOUSE_WHEEL_DOWN:
        {
            int index = vertical_scroll_->GetScrollPosWZ();
            ++index;
            index = min((int)ftext_render_.GetMaxLine(), index);
            ftext_render_.SetBeginLineIndex(index);
            vertical_scroll_->SetScrollPosWZ(index);
        }
        break;
    default:
        break;
    }
}
//----------------------------------------------------------------------------
/**
*/
void uiMissionObject::MouseOver_MsgProc(SI_MESSAGE* message)
{
    switch (message->eResultMsg) 
    {
    case RT_MSG_MOUSEOVER:
        {
            POSTYPE position = get_control_id_to_pos(message->dwCtrlID);
            switch (position)
            {
            case eDIALOG_ALL_BUTTON:
                tooltip_string_code = 2174;
                break;
            case eDIALOG_PLAY_BUTTON:
                tooltip_string_code = 2176;
                break;
            case eDIALOG_COMPLETE_BUTTON:
                tooltip_string_code = 2175;
                break;
            default:
                tooltip_string_code = 0;
            }
        }
        break;
    }
}

void uiMissionObject::UpdatePosition()
{
    //SolarDialog::UpdatePosition();
    reflash_line_convert_ = true;
}
