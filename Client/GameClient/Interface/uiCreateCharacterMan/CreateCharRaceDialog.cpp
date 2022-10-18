#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT
#include "uiCreateCharacterMan/CreateCharRaceDialog.h"
#include "CharacterCreationInfoParser.h"
#include "interfacemanager.h"
#include "GlobalFunc.h"
//------------------------------------------------------------------------------
void CreateCharRaceDialog::RegisterControl()
{
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
	RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
}

CreateCharRaceDialog::CreateCharRaceDialog(InterfaceManager* ui_manager) : uiBase(ui_manager)
{
    handle_texture_ = INVALID_HANDLE_VALUE;
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    handle_texture2_ = INVALID_HANDLE_VALUE;
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}

CreateCharRaceDialog::~CreateCharRaceDialog(void)
{
    Release();
}

void CreateCharRaceDialog::Init(CDrawBase* drawbase)
{
    RegisterControl();

    vertical_scroll_ = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    if (vertical_scroll_ == NULL)
    {
        assert(!"CreateCharRaceDialog 컨트롤 없음 kControl_VScroll_V000");
        assert(vertical_scroll_);
    }

    CCtrlStaticWZ* static_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
    if (!static_text)
    {
        assert(!"CreateCharRaceDialog 컨트롤 없음 kControl_Text_S003");
        return;
    }

    select_race_ = eCHAR_BERSERKER;
    handle_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg01.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    texture_information_ = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(handle_texture_);
    assert(texture_information_);

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    handle_texture2_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\Common_bg02.tga" ,TLS_ORIGINAL_SIZE | TLS_NO_MULTITHREAD);
    texture_information2_ = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(handle_texture2_);
    assert(texture_information2_);
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    RECT text_rect = static_text->GetSizeRect();
    ftext_render_.Init(GetDrawBase(), text_rect);

    static_text->SetTextWZ("");
    vertical_scroll_->SetScrollRangeWZ(0,0);
    vertical_scroll_->SetScrollPosWZ(0);

   /* if (mission_title_ != NULL)
    {
        TCHAR title_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
        name_code_ = 0;
        // 2173: %s 미션 진행 정보, %s = ""
        g_InterfaceManager.GetInterfaceString(mission_title_->GetTextID(), temp_string);
        sprintf(title_string, temp_string, "");
        mission_title_->SetTextWZ(title_string);
    }*/
    ftext_render_.SetMargin(0,0,0,-12);
    ftext_render_.SetGap(0,2);
    ftext_render_.convertLineInfo();
}

void CreateCharRaceDialog::Release()
{
    if (handle_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_texture_);
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (handle_texture2_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(handle_texture2_);
        handle_texture2_ = INVALID_HANDLE_VALUE;
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
}
//------------------------------------------------------------------------------
void CreateCharRaceDialog::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        {
            //.윗쪽 화살표를 눌렀다.
            //
            if (kControl_VScroll_V000 == GetControlPosition_byID(message->dwCtrlID))
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
            if (kControl_VScroll_V000 == GetControlPosition_byID(message->dwCtrlID))
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
            if (kControl_VScroll_V000 == GetControlPosition_byID(message->dwCtrlID))
            {
                int index = vertical_scroll_->GetScrollPosWZ();
                ftext_render_.SetBeginLineIndex(index);
            }
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void CreateCharRaceDialog::UpdateMouseWheel()
{
    CCtrlStaticWZ* static_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
    if(static_text == NULL || vertical_scroll_ == NULL)
    {
        return;
    }

    RECT list_rect = static_text->GetSizeRect();
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
//------------------------------------------------------------------------------
bool CreateCharRaceDialog::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;

    /*switch( message->eResultMsg )
    {
    
    }*/
    return result;
}

void CreateCharRaceDialog::OnRenderSolarDialog()
{
    ftext_render_.Render();

    CCtrlPictureWZ* picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(kControl_Picture_P004));

    RECT rect;
    float x = 0.0f;
    float y = 0.0f;

    if (picture_control)
    {
        rect = picture_control->GetSizeRect();
    }
    else
    {
        this->GetDialogWZ()->GetClientRect(&rect);
    }
    x = static_cast<float>(rect.left);
    y = static_cast<float>(rect.top);


#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if( select_race_ == eCHAR_HELLROID )
    {
        g_pSunRenderer->RenderTexture( 
            handle_texture_,
            x+1,
            y+1,
            43.0f,
            43.0f,
            773.0f,
            531.0f,
            43.0f,
            43.0f,
            NULL,
            0.0f
            );
    }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    else if( select_race_ == eCHAR_WITCHBLADE )
    {
        g_pSunRenderer->RenderTexture( 
            handle_texture2_,
            x+1,
            y+5,
            43.0f,
            43.0f,
            908.0f,
            240.0f,
            43.0f,
            43.0f,
            NULL,
            0.0f
            );
    }
#endif // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE 
    else
    {
        g_pSunRenderer->RenderTexture( 
            handle_texture_,
            x,
            y,
            45.0f,
            45.0f,
            342.0f + ((select_race_ - eCHAR_BERSERKER) * 45),
            533.0f,
            45.0f,
            45.0f,
            NULL,
            0.0f
            );
    }
#else // _NA_000000_20130812_NEW_CHARACTER_HELLROID

    g_pSunRenderer->RenderTexture( 
        handle_texture_,
        x,
        y,
        45.0f,
        45.0f,
        342.0f + ((select_race_ - eCHAR_BERSERKER) * 45),
        533.0f,
        45.0f,
        45.0f,
        NULL,
        0.0f
        );
#endif // _NA_000000_20130812_NEW_CHARACTER_HELLROID
}

bool CreateCharRaceDialog::AddFormatText(const TCHAR* message)
{
    // 정규화 텍스트 세팅
    static int posnumber = 0;

    TCHAR value_string[1024] = {0,};
    Snprintf(value_string, 1024 , _T("%s"), message);

    BOOL result = FALSE;
    ftext_render_.ClearSentence();
    result = ftext_parser_.Parser(value_string, ftext_sentence_);
    if(result)
    {
        ftext_render_.Append(ftext_sentence_);
        ftext_render_.convertLineInfo();
    }
    ftext_render_.SetBeginLineIndex(0);
    return true;
}

void CreateCharRaceDialog::SetRaceDescription(BYTE select_race)
{
    CCtrlStaticWZ* static_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
    if (static_text)
    {
        static_text->SetTextWZ(GlobalFunc::GetRaceString(static_cast<eCHAR_TYPE>(select_race)));
    }
    select_race_ = select_race;

    CREATION_INFO* creation_info = CharacterCreationInfoParser::Instance()->GetCreationInfo(select_race);
    if (creation_info)
    {
        DWORD race_code = creation_info->race_description_code;
        g_InterfaceManager.GetInterfaceString(race_code, complete_string_);
        AddFormatText(complete_string_);

        if (vertical_scroll_ != NULL)
        {
            int scroll_range = ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect();
            vertical_scroll_->SetScrollRangeWZ(0,scroll_range);
            vertical_scroll_->SetScrollPosWZ(scroll_range);
        }
    }
}

void CreateCharRaceDialog::OnUpdateSolarDialog()
{
    CCtrlStaticWZ* static_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S003));
    ftext_render_.SetRect(static_text->GetSizeRect());
    UpdateMouseWheel();
}
#endif//_NA_005003_20120612_NEW_CHARACTER_MYSTIC_FOR_CLIENT