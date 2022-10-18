#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "uiCharacterStatusHonorDescription.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "FameListParser.h"
#include "ReputeListParser.h"
#include "FTextParser.h"
#include "FTextSentence.h"

const DWORD uiCharacterStatusHonorDescription::kDialog_ID = StrToWzID("0406");

uiCharacterStatusHonorDescription::uiCharacterStatusHonorDescription(InterfaceManager* interface_manager)
:uiBase(interface_manager),honor_type_(0),honor_code_(0)
{

}

uiCharacterStatusHonorDescription::~uiCharacterStatusHonorDescription()
{

}

void uiCharacterStatusHonorDescription::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);

    GetControlWZ_byPosition(kControl_Text_S000)->SetTextWZ(" ");
}

void uiCharacterStatusHonorDescription::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
    }
    else
    {
        honor_type_ = 0;
        honor_code_ = 0;
    }
}

void uiCharacterStatusHonorDescription::OnUpdateSolarDialog()
{
    UpdateMouseWheel();

    CCtrlStaticWZ* description_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (description_control != NULL)
    {
        RECT control_rect = description_control->GetSizeRect();
        honor_description().SetRect(control_rect);
    }    
}

void uiCharacterStatusHonorDescription::OnRenderSolarDialog()
{
    honor_description().Render();
}

void uiCharacterStatusHonorDescription::UpdateMouseWheel()
{
    CControlWZ* scroll_control = GetControlWZ_byPosition(kControl_VScroll_V000);
    if (scroll_control != NULL)
    {
        RECT dialog_rect;
        GetDialogWZ()->GetClientRect(&dialog_rect);
        BYTE wheel_message = uiBase::GetMouseWheelStatus(&dialog_rect);

        SI_MESSAGE message;
        message.dwCtrlID = scroll_control->GetCtrlID();

        switch (wheel_message)
        {
        case MOUSE_WHEEL_UP:
            OnUpScroll(&message);
            break;
        case MOUSE_WHEEL_DOWN:
            OnDownScroll(&message);
            break;
        }
    }
}

void uiCharacterStatusHonorDescription::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    if (GetMan() == NULL)
    {
        return;
    }

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_PAGETOP:
    case RT_MSG_SB_LINETOP:
        OnUpScroll(message);
        break;
    case RT_MSG_SB_PAGEBOTTOM:
    case RT_MSG_SB_LINEBOTTOM:
        OnDownScroll(message);
        break;
    case RT_MSG_SB_THUMBTRACK:
    case RT_MSG_SB_THUMBPOSITION:
    case RT_MSG_SB_ENDSCROLL:
        OnThumbScroll(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiCharacterStatusHonorDescription::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004:
        {
            // 닫기 버튼
            ShowInterfaceWithSound(FALSE);
        }
        break;
    default:
        {
        }
    }
    return result;
}

bool uiCharacterStatusHonorDescription::OnUpScroll(SI_MESSAGE* message)
{
    bool result = true;

    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        --current_scroll_pos;
        current_scroll_pos = max(0, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
        honor_description().SetBeginLineIndex(current_scroll_pos);
    }

    return result;
}

bool uiCharacterStatusHonorDescription::OnDownScroll(SI_MESSAGE* message)
{
    bool result = true;

    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        int scroll_min;
        int scroll_max;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        ++current_scroll_pos;
        current_scroll_pos = min(scroll_max, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
        honor_description().SetBeginLineIndex(current_scroll_pos);
    }

    return result;
}

bool uiCharacterStatusHonorDescription::OnThumbScroll(SI_MESSAGE* message)
{
    bool result = true;

    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (scroll_control != NULL)
    {
        int scroll_min;
        int scroll_max;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        current_scroll_pos = max(0, current_scroll_pos);
        current_scroll_pos = min(scroll_max, current_scroll_pos);
        honor_description().SetBeginLineIndex(current_scroll_pos);
    }

    return result;
}

void uiCharacterStatusHonorDescription::SetHonorDescription(BYTE honor_type, WORD honor_code)
{
    if ((honor_type == 0) || 
        (honor_code == 0))
    {
        return;
    }

    bool is_same_honor_info = false;
    if ((honor_type_ == honor_type) && 
        (honor_code_ == honor_code))
    {
        is_same_honor_info = true;
    }

    if ((IsVisible() == TRUE) && 
        (is_same_honor_info == true))
    {
        ShowInterface(FALSE);
    }
    else
    {
        ShowInterface(TRUE);
        honor_type_ = honor_type;
        honor_code_ = honor_code;
    }

    CCtrlStaticWZ* description_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (description_control == NULL)
    {
        return;
    }

    FTextParser ftext_parser;
    FTextSentence ftext_sentence;
    TCHAR honor_string[INTERFACE_STRING_LENGTH] = {'\0', };
    RECT control_rect = description_control->GetSizeRect();
    honor_description().Init(GetDrawBase(), control_rect);
    honor_description().SetVCenterAlign(true);
    honor_description().ClearSentence();
    ftext_parser.SetDefaultFont(description_control->GetFontID(), description_control->GetTextColor());

    if (honor_type == HONOR_POINT_INFO::FAME)
    {
        FameListParser::FameInfo* fame_info = FameListParser::Instance()->GetFameInfo(honor_code);
        if (fame_info != NULL)
        {
            // 명성 타이틀
            g_InterfaceManager.GetInterfaceString(fame_info->fame_name_code, honor_string);
            GetControlWZ_byPosition(kControl_Text_S000)->SetTextWZ(honor_string);
            // 명성 설명
            g_InterfaceManager.GetInterfaceString(fame_info->fame_description_code, honor_string);
        }
    }
    else if (honor_type == HONOR_POINT_INFO::REPUTE)
    {
        ReputeListParser::ReputeInfo* repute_info = ReputeListParser::Instance()->GetReputeInfo(honor_code);
        if (repute_info != NULL)
        {
            TCHAR honor_info_string[INTERFACE_STRING_LENGTH] = {'\0', };
            // 명성 타이틀
            g_InterfaceManager.GetInterfaceString(repute_info->repute_name_code, honor_string);
            GetControlWZ_byPosition(kControl_Text_S000)->SetTextWZ(honor_string);
            // 명성 설명
            g_InterfaceManager.GetInterfaceString(repute_info->repute_description_code, honor_string);
        }        
    }
    else
    {
        GetControlWZ_byPosition(kControl_Text_S000)->SetTextWZ(" ");
    }

    if(ftext_parser.Parser(honor_string, ftext_sentence) == TRUE)
    {
        honor_description().Append(ftext_sentence);
    }
    honor_description().SetBeginLineIndex(0);
    honor_description().convertLineInfo();

    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    CControlWZ* scroll_image_control = GetControlWZ_byPosition(kControl_Picture_P001);
    if ((scroll_control != NULL) && 
        (scroll_image_control != NULL))
    {
        int max_scroll = honor_description().GetMaxLine();
        int draw_line = honor_description().GetDrawLineInRect();
        if (max_scroll > draw_line)
        {
            scroll_control->ShowWindowWZ(WZ_SHOW);
            scroll_image_control->ShowWindowWZ(WZ_SHOW);

            max_scroll -= draw_line;
            scroll_control->SetScrollRangeWZ(0, max_scroll);
            scroll_control->SetScrollPosWZ(0);
            scroll_control->SetChangeSizeThumb(30);
        }
        else
        {
            scroll_control->ShowWindowWZ(WZ_HIDE);
            scroll_image_control->ShowWindowWZ(WZ_HIDE);
        }        
    }
}

#endif //_NA_003027_20111013_HONOR_SYSTEM