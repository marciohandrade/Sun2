#include "SunClientPrecompiledHeader.h"
#include "uiCharacterSelectList.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "uiLoginMan/uiLoginMan.h"
#include "CharacterScene.h"
#include "InterfaceManager.h"
#include "FTextParser.h"
#include "FTextRender.h"
#include "MapInfoParser.h"
#include "GlobalFunc.h"
#include "CharSelectDialog.h"
#include "objectmanager.h"
#include "Player.h"

namespace
{
    const WzID kCharacterNameFont = StrToWzID("st11");
    const WzID kLevelFont = StrToWzID("st11");
    const WzID kCurrentMapNameFont = StrToWzID("st10");

    const DWORD kCharacterNameColor = WzColor_RGBA(255, 255, 255, 255);
    const DWORD kLevelColor = WzColor_RGBA(255, 199, 4, 255);
    const DWORD kCurrentMapNameColor = WzColor_RGBA(160, 160, 160, 255);
    const DWORD kCloseSlotColor = WzColor_RGBA(255, 0, 0, 200);

    const int kListLeftMargin = 10;
};

uiCharacterSelectList::uiCharacterSelectList(InterfaceManager* ui_manager):
uiBase(ui_manager)
{

}

uiCharacterSelectList::~uiCharacterSelectList()
{

}

void uiCharacterSelectList::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    RegisterControl();
}

void uiCharacterSelectList::RegisterControl()
{
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);//캐릭터슬롯1
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
    RegisterControlPosition(StrToWzID("C007"), kControl_CheckBT_C007);
    RegisterControlPosition(StrToWzID("C008"), kControl_CheckBT_C008);
    RegisterControlPosition(StrToWzID("C009"), kControl_CheckBT_C009);//캐릭터슬롯10
}

void uiCharacterSelectList::Release()
{

}

void uiCharacterSelectList::MessageProc( SI_MESSAGE * message )
{
    if (IsLock())
        return;

    if (g_CharSelectScene.m_bPickCharLock == TRUE)
    {
        return;
    }

    static bool is_double_click = false;
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            if (is_double_click == true)
            {
                is_double_click = false;
            }
            else
            {
                OnLButtonClick(message);
            }            
        }        
        break;
    case RT_MSG_LBUTTONDCLICK:
        OnLButtonDClick(message);
        is_double_click = true;
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void uiCharacterSelectList::OnShowWindow(BOOL val)
{
    if (val)
    {
        SetServerName();
        CharacterListUp();
    }
    else
    {

    }
}

void uiCharacterSelectList::OnRenderSolarDialog()
{
    for (int slot_index = 0; slot_index < kMaxListSlot; ++slot_index)
    {
        character_info_text_[slot_index].Render();
    }
}

void uiCharacterSelectList::OnUpdateSolarDialog()
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();;
        BYTE mouse_wheel_state = GetMouseWheelStatus(&enable_rect);

        if ((mouse_wheel_state == MOUSE_WHEEL_UP) || 
            g_Input.GetState(DIK_UP, KS_UP) ||
            g_Input.GetState(DIK_LEFT, KS_UP))
        {
            OnSelectPrevCharacter();
        }
        else if ((mouse_wheel_state == MOUSE_WHEEL_DOWN) || 
                 g_Input.GetState(DIK_DOWN, KS_UP) ||
                 g_Input.GetState(DIK_RIGHT, KS_UP))
        {
            OnSelectNextCharacter();
        }
    }
}

bool uiCharacterSelectList::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C004:
    case kControl_CheckBT_C005:
    case kControl_CheckBT_C006:
    case kControl_CheckBT_C007:
    case kControl_CheckBT_C008:
    case kControl_CheckBT_C009:
        {
            SetSelectedCharacterListIndex(control_position - kControl_CheckBT_C000);
        }
        break;
    default:
        break;
    }
    return result;
}

bool uiCharacterSelectList::OnLButtonDClick( SI_MESSAGE* message )
{
    BYTE make_and_makeable_slot = g_CharSelectScene.GetCharacterSlotNumber();
    BYTE list_slot_size = character_slot_list_.size();

    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);

    if (list_slot_size <= control_position &&
        make_and_makeable_slot > control_position)
    {
        CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
        if (charater_select_dialog)
        {
            charater_select_dialog->OnCreateCharacter();
        }
        return false;
    }

    if (g_CharSelectScene.GetCurSelectCharacterInfo() &&
        (g_CharSelectScene.GetCurSelectCharacterInfo()->CharacterInfo.m_DelChk != 0))
    {
        CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
        if (charater_select_dialog)
        {
            charater_select_dialog->OnDeleteCharacter();
        }
        return false;
    }

    switch (control_position)
    {
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C004:
    case kControl_CheckBT_C005:
    case kControl_CheckBT_C006:
    case kControl_CheckBT_C007:
    case kControl_CheckBT_C008:
    case kControl_CheckBT_C009:
        {
            SetSelectedCharacterListIndex(control_position - kControl_CheckBT_C000);

            g_Input.Reset();
            TCHAR notice_message[INTERFACE_STRING_LENGTH] = {'\0', };
            WzColor	notice_color = WzColor_RGBA(255, 80, 80, 255);

            if( g_CharSelectScene.SendCharSelectPacket() == FALSE )
            {
                //	선택된 캐릭터가 없습니다
                g_InterfaceManager.GetInterfaceString(eST_NOT_SELECTED_CHARACTER, notice_message, INTERFACE_STRING_LENGTH);
                g_InterfaceManager.ShowMessageBox(notice_message, 3000, &notice_color, 0);
            }
        }
        break;

    default:
        break;
    }
    return result;
}

void uiCharacterSelectList::SetServerName()
{
    uiLoginBackTop* login_back_top = GET_CAST_DIALOG(uiLoginBackTop, uiLoginBackTop::kDialog_ID);
    if (login_back_top == NULL)
    {
        return;
    }

    uiLoginMan* login_manager = GET_DIALOG_MANAGER(uiLoginMan, UIMAN_LOGIN);
    if (login_manager == NULL)
    {
        return;
    }

    AuthSetverData* server_data = login_manager->GetCurrentServerData();
    if (server_data == NULL)
    {
        return;
    }

#ifdef _DH_LAST_SELECTED_CHARACTER
    uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
    if (select_manager)
    {
        select_manager->SetLastSelectServer(static_cast<WORD>(server_data->m_GroupID), static_cast<WORD>(server_data->m_ChannelID));
    }
#endif//_DH_LAST_SELECTED_CHARACTER

    TCHAR* server_name = const_cast<TCHAR*>(server_data->m_GroupName.c_str());
    login_back_top->SetSceneTitleString(server_name);
}

void uiCharacterSelectList::CharacterListUp()
{
    character_slot_list_.clear();
    selected_list_index_ = 0;

    for (int slot_index = 0; slot_index < MAX_CHARACTER_LIST_NUM; ++slot_index)
    {
        CharListInfo* character_info = g_CharSelectScene.GetCharacterInfoBySlotIndex(slot_index);
        if (character_info == NULL)
        {
            continue;
        }
        character_slot_list_.push_back(slot_index);
    }

#ifdef _DH_LAST_SELECTED_CHARACTER //캐릭터 삭제시 및 갱신시 선택 부분이 다시 처리 되게 된다.
    uiCharacterSelectMan* select_manager = GET_DIALOG_MANAGER(uiCharacterSelectMan, UIMAN_CHARACTERSELECT);
    if (select_manager && select_manager->last_selected_header().last_select_version > 0 && 
        select_manager->last_selected_header().last_select_version <= LAST_SELECT_VERSION)
    {
        int selected_slot_index = select_manager->FindLastSelectedSlotIndex(select_manager->last_selected_header().last_select_server);
        selected_list_index_ = GetListIndexFromSlotIndex(selected_slot_index);
    }
#endif//_DH_LAST_SELECTED_CHARACTER

    RefreshSelectList();
    SetSelectedCharacterListIndex(selected_list_index_);
}

void uiCharacterSelectList::RefreshSelectList()
{
    int list_size = static_cast<int>(character_slot_list_.size());
    assert(list_size <= kMaxListSlot);

    ShowListSlotByCount(list_size); // 빈슬롯 사용 못하게

    for (int slot_index = 0; slot_index < kMaxListSlot; ++slot_index)
    {
        if (slot_index >= static_cast<int>(character_slot_list_.size()))
        {
            SetCharacterSlot(slot_index);
        }
        else
        {
            CharListInfo* character_info = g_CharSelectScene.GetCharacterInfoBySlotIndex(character_slot_list_[slot_index]);
            if (character_info == NULL)
            {
                continue;
            }

            TCHAR buffer_string[INTERFACE_STRING_LENGTH] = {0,};
            TCHAR delete_string[INTERFACE_STRING_LENGTH] = "%d:%d";

            g_InterfaceManager.GetInterfaceString(70560, buffer_string);

            Player* player = static_cast<Player*>(g_ObjectManager.GetPlayer(character_info->dwObjectKey));

            if (character_info->CharacterInfo.m_DelChk != 0 && player)
            {
                int day = 0 , hour = 0 , minute = 0;
                int RemainTotalTime = player->GetCharDelRemainTime();
                int remainder = RemainTotalTime;
                // 1일 보다 큰가
                if( RemainTotalTime >= 86400)
                {
                    day = RemainTotalTime / 86400;
                    remainder = RemainTotalTime % 86400;
                }
                // 1시간 보다 큰가
                if( RemainTotalTime >= 3600 )
                {
                    hour = remainder / 3600;
                    remainder = RemainTotalTime % 3600;
                }
                // 분 단위 인가
                minute = remainder / 60;
                sprintf(delete_string, buffer_string, day, (hour+1));
            }

            CLIENT_CHARACTER_PART* part_info = &(character_info->CharacterInfo);
            SetCharacterSlotInfo(slot_index, 
                part_info->m_tszCharName, 
                part_info->m_DelChk,
                part_info->m_LV, 
                part_info->m_byClass, 
                part_info->m_iRegion,
                delete_string);
        }
    }
}

void uiCharacterSelectList::ShowListSlotByCount(int slot_count)
{
    BYTE make_and_makeable_slot = g_CharSelectScene.GetCharacterSlotNumber();
    for (int slot_index = 0; slot_index < kMaxListSlot; ++slot_index)
    {
        DWORD control_id = GetControlID_byPosition(kControl_CheckBT_C000 + slot_index);
        CControlWZ* check_control = GetControlWZ(control_id);
        if (check_control == NULL)
        {
            continue;
        }

        //ENUM_STATEWZ control_state = (slot_index >= slot_count) ? WZ_HIDE : WZ_SHOW;

        //if (slot_index >= slot_count)
        if (slot_index >= make_and_makeable_slot)
        {
            check_control->EnableWindowWZ(false);
        }
        else
        {
            check_control->EnableWindowWZ(true);
        }
        //check_control->ShowWindowWZ(control_state);
        character_info_text_[slot_index].ClearSentence();
    }

    CharSelectDialog* charater_select_dialog = GET_CAST_DIALOG(CharSelectDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_DIALOG);
    if (charater_select_dialog)
    {
        charater_select_dialog->EnableButton((slot_count < make_and_makeable_slot), CharSelectDialog::CharSelect_Create);
    }
}

void uiCharacterSelectList::SetCharacterSlot(int list_index)
{
    if (list_index < 0 || list_index >= kMaxListSlot)
    {
        return;
    }

    DWORD control_id = GetControlID_byPosition(kControl_CheckBT_C000 + list_index);
    CCtrlButtonCheckWZ* check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(control_id));
    if (check_control == NULL)
    {
        return;
    }

    TCHAR character_info_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR class_string[INTERFACE_STRING_LENGTH] = {'\0', };
    FTextParser parser;
    FTextSentence sentence;
    RECT control_rect = check_control->GetSizeRect();

    character_info_text_[list_index].Init(GetDrawBase(), control_rect);
    character_info_text_[list_index].SetVCenterAlign(true);
    character_info_text_[list_index].SetHCenterAlign(true);
    character_info_text_[list_index].SetMargin(kListLeftMargin, 
        FTextRender::DEFAULT_MARGIN_TOP, 
        kListLeftMargin, 
        FTextRender::DEFAULT_MARGIN_BOTTOM);

    // 캐릭터 이름
    BYTE make_and_makeable_slot = g_CharSelectScene.GetCharacterSlotNumber();
    if (list_index < make_and_makeable_slot)
    {
        g_InterfaceManager.GetInterfaceString(5993, character_info_string); // 5993: 캐릭터를 생성 하실 수 있습니다.
        parser.SetDefaultFont(kCharacterNameFont, kLevelColor);
    }
    else
    {
        g_InterfaceManager.GetInterfaceString(5994, character_info_string); // 5994: 캐릭터를 생성 하실 수 없습니다.
        parser.SetDefaultFont(kCharacterNameFont, kCloseSlotColor);
    }
    
    parser.Parser(character_info_string, sentence);
    character_info_text_[list_index].Append(sentence);

    // 캐릭터 레벨 & 클래스
    /*int class_name_code = GlobalFunc::GetClassNameCode(class_type);
    g_InterfaceManager.GetInterfaceString(class_name_code, class_string);
    Snprintf(character_info_string, INTERFACE_STRING_LENGTH - 1, "%d %s", level, class_string);
    parser.SetDefaultFont(kLevelFont, kLevelColor);
    parser.Parser(character_info_string, sentence);
    character_info_text_[list_index].Append(sentence);

    // 현재지역
    const sMAPINFO* current_map_info = MapInfoParser::Instance()->FindMapInfo(current_mapcode);
    if (current_map_info != NULL)
    {
        g_InterfaceManager.GetInterfaceString(current_map_info->dwNCode, character_info_string);
        parser.SetDefaultFont(kCurrentMapNameFont, kCurrentMapNameColor);
        parser.Parser(character_info_string, sentence);
        character_info_text_[list_index].Append(sentence);
    }*/
    character_info_text_[list_index].convertLineInfo();
}

void uiCharacterSelectList::SetCharacterSlotInfo(int list_index, 
                                                 TCHAR* character_name, 
                                                 BYTE delete_check,
                                                 int level, 
                                                 BYTE class_type, 
                                                 MAPCODE current_mapcode,
                                                 TCHAR* day_for_delete)
{
    if (character_name == NULL)
    {
        return;
    }

    if (list_index < 0 || list_index >= kMaxListSlot)
    {
        return;
    }

    DWORD control_id = GetControlID_byPosition(kControl_CheckBT_C000 + list_index);
    CCtrlButtonCheckWZ* check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(control_id));
    if (check_control == NULL)
    {
        return;
    }

    TCHAR character_info_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR class_string[INTERFACE_STRING_LENGTH] = {'\0', };
    FTextParser parser;
    FTextSentence sentence;
    RECT control_rect = check_control->GetSizeRect();

    character_info_text_[list_index].Init(GetDrawBase(), control_rect);
    character_info_text_[list_index].SetVCenterAlign(true);
    character_info_text_[list_index].SetHCenterAlign(false);
    character_info_text_[list_index].SetMargin(kListLeftMargin, 
        FTextRender::DEFAULT_MARGIN_TOP, 
        kListLeftMargin, 
        FTextRender::DEFAULT_MARGIN_BOTTOM);

    // 캐릭터 이름
    Snprintf(character_info_string, INTERFACE_STRING_LENGTH - 1, character_name);

    if (delete_check > 0)
    {
        parser.SetDefaultFont(kCharacterNameFont, kCloseSlotColor); // 삭제 대기중이면 빨간색
    }
    else
    {
        parser.SetDefaultFont(kCharacterNameFont, kCharacterNameColor);
    }    
    parser.Parser(character_info_string, sentence);
    character_info_text_[list_index].Append(sentence);

    // 캐릭터 레벨 & 클래스
    int class_name_code = GlobalFunc::GetClassNameCode(class_type);
    g_InterfaceManager.GetInterfaceString(class_name_code, class_string);
    Snprintf(character_info_string, INTERFACE_STRING_LENGTH - 1, "%d %s", level, class_string);
    parser.SetDefaultFont(kLevelFont, kLevelColor);
    parser.Parser(character_info_string, sentence);
    character_info_text_[list_index].Append(sentence);

    // 현재지역
    const sMAPINFO* current_map_info = MapInfoParser::Instance()->FindMapInfo(current_mapcode);
    if (delete_check > 0)
    { // 삭제 대기중이면 삭제 대기 메시지
        g_InterfaceManager.GetInterfaceString(5038, class_string);
        Snprintf(character_info_string, INTERFACE_STRING_LENGTH, "%s [%s]", class_string, day_for_delete);
        parser.SetDefaultFont(kCurrentMapNameFont, kCurrentMapNameColor);
        parser.Parser(character_info_string, sentence);
        character_info_text_[list_index].Append(sentence);
    }
    else
    {
        if (current_map_info != NULL)
        {
            g_InterfaceManager.GetInterfaceString(current_map_info->dwNCode, character_info_string);
            parser.SetDefaultFont(kCurrentMapNameFont, kCurrentMapNameColor);
            parser.Parser(character_info_string, sentence);
            character_info_text_[list_index].Append(sentence);
        }
    }

    character_info_text_[list_index].convertLineInfo();
}

void uiCharacterSelectList::SetSelectedCharacterSlotIndex(int slot_index)
{
    int list_size = static_cast<int>(character_slot_list_.size());
    int list_index = -1;
    
    for (int index = 0; index < list_size; ++index)
    {
        if (character_slot_list_[index] == slot_index)
        {
            list_index = index;
            break;
        }
    }

    SetSelectedCharacterListIndex(list_index);
}

void uiCharacterSelectList::SetSelectedCharacterListIndex(int list_index)
{
    int list_size = static_cast<int>(character_slot_list_.size());
    if (list_size < 1)
    {
        list_index = -1;
    }

    selected_list_index_ = list_index;

    for (int index = 0; index < kMaxListSlot; ++index)
    {
        DWORD control_id = GetControlID_byPosition(kControl_CheckBT_C000 + index);
        CCtrlButtonCheckWZ* check_control = static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(control_id));
        if (check_control == NULL || check_control->GetDrawState() == E_DISABLE)
        {
            continue;
        }

        bool enable_state = (list_index == index) ? true : false;
        check_control->SetCheckState(enable_state);
    }

    g_CharSelectScene.ShowSelectCharacter(GetSelectedCharacterSlotIndex());
}

void uiCharacterSelectList::OnSelectPrevCharacter()
{
    --selected_list_index_;
    selected_list_index_ = max(selected_list_index_, 0);
    
    SetSelectedCharacterListIndex(selected_list_index_);
}

void uiCharacterSelectList::OnSelectNextCharacter()
{
    ++selected_list_index_;
    selected_list_index_ = min(selected_list_index_, kMaxListSlot);
    selected_list_index_ = min(selected_list_index_, static_cast<int>(character_slot_list_.size() - 1)) ;
    if (selected_list_index_ == -1)
    {
        selected_list_index_ = 0;
    }

    SetSelectedCharacterListIndex(selected_list_index_);
}

int uiCharacterSelectList::GetSelectedCharacterSlotIndex()
{
    int list_size = static_cast<int>(character_slot_list_.size());
    if ((list_size < 1) || 
        (list_size <= selected_list_index_))
    {
        return -1;
    }
    int index = character_slot_list_[selected_list_index_];
    return index;
}
//------------------------------------------------------------------------------
int uiCharacterSelectList::GetListIndexFromSlotIndex(int slot_index)
{
    std::vector<int>::iterator begin = character_slot_list_.begin(); 
    std::vector<int>::iterator end = character_slot_list_.end();
    for (int index = 0; begin != end; ++begin, ++index)
    {
        if ((*begin) == slot_index)
        {
            return index;
        }
    }
    return 0;
}
