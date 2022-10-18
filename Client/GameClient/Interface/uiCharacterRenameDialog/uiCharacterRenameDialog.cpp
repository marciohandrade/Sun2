#include "SunClientPrecompiledHeader.h"
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
#include "uiCharacterRenameDialog.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "uiCharacterRenameInputDialog/uiCharacterRenameInputDialog.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "InterfaceManager.h"
#include "CharacterScene.h"
#include "Application.h"
#include "FTextParser.h"
#include "FTextSentence.h"

uiCharacterRenameDialog::uiCharacterRenameDialog(InterfaceManager* ui_manager):
uiBase(ui_manager)
{
}

uiCharacterRenameDialog::~uiCharacterRenameDialog()
{
}

void uiCharacterRenameDialog::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControl();
    InitNoticeText();
}

void uiCharacterRenameDialog::Release()
{ 

}

void uiCharacterRenameDialog::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

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

void uiCharacterRenameDialog::OnShowWindow(BOOL val)
{
    if (val)
    {   
    }
    else
    {
        g_CharSelectScene.InitRenameProcess();
        g_InterfaceManager.UnLockDlgs();
    }
}

void uiCharacterRenameDialog::OnRenderSolarDialog()
{
    POINT screen_size = g_pApplication->Get2DModeMapping();
    g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,100), 
                              0.0f, 
                              0.0f, 
                              static_cast<float>(screen_size.x), 
                              static_cast<float>(screen_size.y));
    GetDialogWZ()->OnDraw(true);

    WzID text_control_id = GetControlID_byPosition(kControl_Text_ST00);
    CCtrlStaticWZ* notice_text = static_cast<CCtrlStaticWZ*>(GetControlWZ(text_control_id));
    if (notice_text != NULL)
    {
        RECT text_rect = notice_text->GetSizeRect();
        notice_text_.SetRect(text_rect);
        notice_text_.Render();
    }
}

void uiCharacterRenameDialog::RegisterControl()
{
    RegisterControlPosition(StrToWzID("BT62"), kControl_Button_BT62);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_Exit);
    RegisterControlPosition(StrToWzID("TI00"), kControl_TextImage_TI00);
    RegisterControlPosition(StrToWzID("ST00"), kControl_Text_ST00);
}

bool uiCharacterRenameDialog::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_BT62:
    case kControl_Button_B000:
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B006:
    case kControl_Button_B007:
    case kControl_Button_B008:
    case kControl_Button_B009:
        {
            // 변경하기 버튼
            bool success_select = SelectRename(control_position - kControl_Button_BT62);
            if (success_select == true)
            {
                // 이름변경입력UI 출력
                uiCharacterRenameInputDialog* rename_input_dialog = GET_CAST_DIALOG(uiCharacterRenameInputDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME_INPUT);
                assert(rename_input_dialog);
                if (rename_input_dialog != NULL)
                {
                    rename_input_dialog->SetRenameData(selected_rename_.first, selected_rename_.second);
                    rename_input_dialog->ShowInterface(TRUE);
                }
            }
        }
        break;
    case kControl_Button_Exit:
    case kControl_TextImage_TI00:
        {
            // X버튼, 나중에 변경하기 버튼
            ShowInterface(FALSE);
            // 캐릭터 선택 관련 gui 보이기
            g_CharSelectScene.ShowCharacterSelectSceneUI();
        }
        break;
    default:
        break;
    }
    return result;
}

void uiCharacterRenameDialog::RefreshNeedRenameList()
{
    if (g_CharSelectScene.NeedRenameProcess() == false)
    {
        ShowInterface(FALSE);
        // 캐릭터 선택 관련 gui 보이기
        g_InterfaceManager.ShowWindow(InterfaceManager::DIALOG_CHARSELECT,TRUE);
        uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
        if (login_back_top != NULL)
        {
            login_back_top->ShowInterface(TRUE);
            login_back_top->SetSceneTitle(50);
        }
        return;
    }

    need_rename_list_.clear();
    for (BYTE index = 0; index < g_CharSelectScene.GetCharacterSlotNumber(); ++index)
    {
        if (g_CharSelectScene.NeedRenameByIndex(index) == true)
        {
            // 이름을 얻어와서 인덱스와 엮어서 리스트에 저장
            CharListInfo* char_info = g_CharSelectScene.GetCharacterInfoBySlotIndex(index);
            if (char_info == NULL)
            {
                continue;
            }
            RenameData rename_data;
            rename_data.first = char_info->CharacterInfo.m_bySlot;
            rename_data.second = char_info->CharacterInfo.m_tszCharName;
            need_rename_list_.push_back(rename_data);
        }
    }

    // 리스트업이 되어 있으면 이름 및 버튼 세팅
    WzID text_control_id = 0;
    WzID button_control_id = 0;
    CCtrlStaticWZ* name_text = NULL;
    CCtrlButtonWZ* rename_button = NULL;
    RenameList::iterator rename_itr = need_rename_list_.begin();
    RenameList::iterator rename_itr_end = need_rename_list_.end();
	int index;
    for (index = 0; index < MAX_CHARACTER_LIST_NUM; ++index)
    {   
        text_control_id = GetControlID_byPosition(kControl_Text_S000 + index);
        button_control_id = GetControlID_byPosition(kControl_Button_BT62 + index);
        name_text = static_cast<CCtrlStaticWZ*>(GetControlWZ(text_control_id));
        rename_button = static_cast<CCtrlButtonWZ*>(GetControlWZ(button_control_id));
        if ((name_text == NULL) ||
            (rename_button == NULL))
        {
            //assert(name_text && rename_button);
            continue;
        }

        if (rename_itr != rename_itr_end)
        {
            // 리스트의 내용을 출력
            RenameData rename_data = (*rename_itr);
            name_text->SetTextWZ(const_cast<TCHAR*>(rename_data.second.c_str()));
            name_text->ShowWindowWZ(WZ_SHOW);
            rename_button->ShowWindowWZ(WZ_SHOW);
            
            ++rename_itr;
        }
        else
        {
            name_text->ShowWindowWZ(WZ_HIDE);
            rename_button->ShowWindowWZ(WZ_HIDE);
        }
    }
}

bool uiCharacterRenameDialog::SelectRename( int list_index )
{
    RenameList::iterator rename_itr = need_rename_list_.begin();
    RenameList::iterator rename_itr_end = need_rename_list_.end();
    for (int index = 0; rename_itr != rename_itr_end; ++index, ++rename_itr)
    {
        if (index == list_index)
        {
            selected_rename_ = (*rename_itr);
            return true;
        }
    }
    return false;
}

void uiCharacterRenameDialog::InitNoticeText()
{
    WzID text_control_id = GetControlID_byPosition(kControl_Text_ST00);
    CCtrlStaticWZ* notice_text = static_cast<CCtrlStaticWZ*>(GetControlWZ(text_control_id));
    if (notice_text == NULL)
    {
        return;
    }

    notice_text->ShowWindowWZ(WZ_HIDE);
    WzID text_font = notice_text->GetFontID();
    WzID text_color = notice_text->GetTextColor();

    FTextParser ftext_parser;
    FTextSentence ftext_sentence;
    RECT text_rect = notice_text->GetSizeRect();

    notice_text_.Init(GetDrawBase(), text_rect);
    notice_text_.SetHCenterAlign(true);
    notice_text_.ClearSentence();

    text_color = RGBATOARGB(GetRValue(text_color),GetGValue(text_color),GetBValue(text_color), GetAValue(text_color));
    ftext_parser.SetDefaultFont(text_font, text_color);
    
    ftext_parser.Parser(g_InterfaceManager.GetInterfaceString(2258), ftext_sentence);
    notice_text_.Append(ftext_sentence);
    notice_text_.convertLineInfo();
    notice_text_.SetBeginLineIndex(0);
}	



#endif //_JP_20110222_INGAME_CHARNAME_CHANGE