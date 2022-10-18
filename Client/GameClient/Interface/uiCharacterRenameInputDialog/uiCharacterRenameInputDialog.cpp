#include "SunClientPrecompiledHeader.h"
#ifdef _JP_20110222_INGAME_CHARNAME_CHANGE
#include "uiCharacterRenameInputDialog/uiCharacterRenameInputDialog.h"
#include "uiCharacterRenameDialog/uiCharacterRenameDialog.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "uiLoginBack/uiLoginBackTop.h"
#include "InterfaceManager.h"
#include "CharacterScene.h"
#include "GlobalFunc.h"
#include "uiSystemConfirm/uiSystemConfirm.h"
#include "uiSystemVerify/uiSystemVerify.h"
#include "ObjectManager.h"
#include "Player.h"
#include "FTextParser.h"
#include "FTextSentence.h"

const DWORD kNotice_Text_Color = RGBATOARGB(200, 200, 200, 255);
const DWORD kCaret_Show_Color = WzColor_RGBA(255, 255, 255, 255);
const DWORD kCaret_Hide_Color = WzColor_RGBA(255, 255, 255, 0);

void FunctionConfirmContinue(bool is_continue)
{
    uiCharacterRenameDialog* rename_dialog = GET_CAST_DIALOG(uiCharacterRenameDialog, IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME);
    if (rename_dialog == NULL)
    {
        return;
    }

    if (is_continue == true)
    {
        rename_dialog->ShowInterface(TRUE);
        rename_dialog->RefreshNeedRenameList();
    }
    else
    {
        rename_dialog->ShowInterface(FALSE);
        // ĳ���� ���� ���� gui ���̱�
        g_CharSelectScene.ShowCharacterSelectSceneUI();
    }
}

uiCharacterRenameInputDialog::uiCharacterRenameInputDialog(InterfaceManager* ui_manager):
uiBase(ui_manager)
{
}

uiCharacterRenameInputDialog::~uiCharacterRenameInputDialog()
{

}

void uiCharacterRenameInputDialog::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControl();
}

void uiCharacterRenameInputDialog::Release()
{

}

void uiCharacterRenameInputDialog::NetworkProc(MSG_BASE* recv_packet)
{
    if (recv_packet->m_byCategory == CG_CHARINFO)
    {
        switch(recv_packet->m_byProtocol)
        {
        case CG_CHARINFO_CHARACTERID_CHECK_ACK:
            {
                // ��밡�� �޽��� ��� / ����ϱ� ��ư ����
                ConfirmUseName();
            }
            break;
        case CG_CHARINFO_CHARACTERID_CHECK_NAK:
            {
                // ��� �Ұ� �޽��� ���
                NoticeCannotUseName();
            }
            break;
        }
    }
    else if (recv_packet->m_byCategory == CG_CONNECTION)
    {
        switch(recv_packet->m_byProtocol)
        {
        case CG_CHARNAME_CHANGE_ACK:
            {
                // �̸� ���� ���� - �޽��� ���
                ProcessSuccessRename(recv_packet);
            }
            break;
        case CG_CHARNAME_CHANGE_NAK:
            {
                // �̸� ���� ���� - �����ڵ� ó��
                ProcessErrorCode(recv_packet);
            }
            break;
        }
    }
}

void uiCharacterRenameInputDialog::MessageProc( SI_MESSAGE * message )
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_EDIT_ENTERKEYDOWN:
        ProcessRenameFlow();
        break;
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void uiCharacterRenameInputDialog::OnShowWindow( BOOL val )
{
    if (val)
    {
        g_InterfaceManager.LockDlgs(IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME_INPUT);
    }
    else
    {
        g_InterfaceManager.UnLockDlgs();
        uiBase* rename_dialog = GET_DIALOG(IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME);
        if ((rename_dialog != NULL) && 
            (rename_dialog->IsVisible() != FALSE))
        {
            g_InterfaceManager.LockDlgs(IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME);
        }
        rename_flow_state_ = State_None;
    }
}

void uiCharacterRenameInputDialog::OnRenderSolarDialog()
{
    WzID text_control_id = GetControlID_byPosition(kControl_Text_T001);
    CCtrlStaticWZ* notice_text = static_cast<CCtrlStaticWZ*>(GetControlWZ(text_control_id));
    if (notice_text != NULL)
    {
        RECT text_rect = notice_text->GetSizeRect();
        notice_text_.SetRect(text_rect);
        notice_text_.Render();
    }
}

void uiCharacterRenameInputDialog::RegisterControl()
{
    RegisterControlPosition(StrToWzID("BT00"), kControl_Button_BT00);
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("TI00"), kControl_TextImage_TI00);
    RegisterControlPosition(StrToWzID("TI01"), kControl_TextImage_TI01);
}

bool uiCharacterRenameInputDialog::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_TextImage_TI00:
        {
            // �ߺ�Ȯ��/����ϱ� ��ư
            ProcessRenameFlow();
        }
        break;
    case kControl_TextImage_TI01:
    case kControl_Button_BT00:
        {
            ShowInterface(FALSE);
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiCharacterRenameInputDialog::ProcessRenameFlow()
{
    switch (rename_flow_state_)
    {
    case State_Check_Name:
        {
            WzID control_id = GetControlID_byPosition(kControl_Edit_E000);
            CCtrlDxIMEEditBox* rename_edit_control = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(control_id));
            if (rename_edit_control != NULL)
            {
                TCHAR inpute_rename[INTERFACE_STRING_LENGTH] = {'\0', };
                rename_edit_control->GetTextWZ(inpute_rename, INTERFACE_STRING_LENGTH - 1);
                rename_ = inpute_rename;
                CheckCharacterRename(inpute_rename);
            }            
        }
        break;
    case State_Confirm_Use:
        {
            SendCharacterNameChangePacket();            
        }
        break;
    }
}
 
void uiCharacterRenameInputDialog::InitInputDialog()
{
    // �ؽ�Ʈ ����, ��ư�ؽ�Ʈ, ����Ʈ �ʱ� ����
    TCHAR control_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
    // �ȳ��޽��� ����
    // 2265 "%s"�� ĳ���͸��� �����մϴ�.\n������ ĳ���͸��� �Է��Ͻ� ��, �ߺ�Ȯ���� ���ּ���.
    g_InterfaceManager.GetInterfaceString(2265, temp_string);
    Snprintf(control_string, INTERFACE_STRING_LENGTH - 1, temp_string, current_name_.c_str());
    SetNoticeText(control_string);

    // �ߺ�Ȯ�� ��ư ����
    WzID control_id = GetControlID_byPosition(kControl_TextImage_TI00);
    CCtrlImageStaticWZ* button_control = static_cast<CCtrlImageStaticWZ*>(GetControlWZ(control_id));
    if (button_control != NULL)
    {
        // 2266 �ߺ�Ȯ��
        g_InterfaceManager.GetInterfaceString(2266, control_string);
        button_control->SetTextWZ(control_string);
    }

    control_id = GetControlID_byPosition(kControl_Edit_E000);
    CCtrlDxIMEEditBox* edit_control = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(control_id));
    if (edit_control != NULL)
    {
        edit_control->ClearText();
        edit_control->SetFontID(StrToWzID("mn12"));
        edit_control->EnableWindowWZ(true);
        edit_control->SetColorCaret(kCaret_Show_Color);// ĳ�� ����
    }

    rename_flow_state_ = State_Check_Name;
}

void uiCharacterRenameInputDialog::CheckCharacterRename(char* rename)
{
    TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
    int name_length =_tcslen(rename);
    bool is_vaild_name = true;

    // ��Ÿ �̸�üũ
    if (name_length == 0)
    {
        //	ĳ���� �̸��� �Է��ϼ���
        g_InterfaceManager.GetInterfaceString(eST_INPUT_CHARACTER_NAME, message, MAX_MESSAGE_LENGTH);
        is_vaild_name = false;
    }
    else if (name_length < 4)
    {
        // �̸��� ���� 4�� �ѱ� 2�� �̻� �Է����ּ���
        g_InterfaceManager.GetInterfaceString(2276 , message ,INTERFACE_STRING_LENGTH);
        is_vaild_name = false;
    }
    else if (name_length > MAX_CHARNAME_LENGTH)
    {
        //	�̸��� ���� 16�� �ѱ� 8�� �̳��� �Է��� �ֽʽÿ�.
        g_InterfaceManager.GetInterfaceString(2277, message, MAX_MESSAGE_LENGTH);
        is_vaild_name = false;
    }
    else if(g_slangNameFilter.IsHaveReplace(rename))
    {
        //	���� �ܾ� ���
        g_InterfaceManager.GetInterfaceString(2275, message, MAX_MESSAGE_LENGTH);
        is_vaild_name = false;
    }
#if defined(__KR) || defined(_KOREA)
    else if(g_slangNameFilter.CheckIncludeString(rename))
    {
        //	���� �ܾ� ���
        g_InterfaceManager.GetInterfaceString(2275, message, MAX_MESSAGE_LENGTH);
        is_vaild_name = false;
    }
# else	//#if defined(__KR) || defined(_KOREA)
    else if(GlobalFunc::CheckIncludeStringOnAbroad(rename))
    {
        //	���� �ܾ� ���
        g_InterfaceManager.GetInterfaceString(2275, message, MAX_MESSAGE_LENGTH);
        is_vaild_name = false;
    }
#endif	// #if defined(__KR) || defined(_KOREA)

    if (is_vaild_name == true)
    {
        // �ߺ� üũ��Ŷ ����
        g_CharSelectScene.SendCharacterIdCheckSyn(rename);
        rename_flow_state_ = State_None;
    }
    else
    {
        SetNoticeText(message);
    }
}

void uiCharacterRenameInputDialog::NoticeCannotUseName()
{
    TCHAR control_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };

    // 2267 "%s"�� �̹� ������Դϴ�.\n�ٸ� ĳ���͸��� �Է����ּ���.
    g_InterfaceManager.GetInterfaceString(2267, temp_string);
    Snprintf(control_string, INTERFACE_STRING_LENGTH - 1, temp_string, rename_.c_str());
    SetNoticeText(control_string);
    
    rename_flow_state_ = State_Check_Name;
}

void uiCharacterRenameInputDialog::ConfirmUseName()
{
    TCHAR control_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };

    // 2268 "%s"�� ����� ������ ĳ���͸��Դϴ�.\n�ش� ĳ���͸��� ����Ͻðڽ��ϱ�?
    g_InterfaceManager.GetInterfaceString(2268, temp_string);
    Snprintf(control_string, INTERFACE_STRING_LENGTH - 1, temp_string, rename_.c_str());
    SetNoticeText(control_string);

    WzID control_id = GetControlID_byPosition(kControl_TextImage_TI00);
    CCtrlImageStaticWZ* button_control = static_cast<CCtrlImageStaticWZ*>(GetControlWZ(control_id));
    if (button_control != NULL)
    {
        // 2269 ����ϱ�
        g_InterfaceManager.GetInterfaceString(2269, control_string);
        button_control->SetTextWZ(control_string);
    }

    control_id = GetControlID_byPosition(kControl_Edit_E000);
    CCtrlDxIMEEditBox* rename_edit_control = static_cast<CCtrlDxIMEEditBox*>(GetControlWZ(control_id));
    if (rename_edit_control != NULL)
    {
        rename_edit_control->EnableWindowWZ(false);
        rename_edit_control->SetColorCaret(kCaret_Hide_Color);// ĳ�� ����
    }

    rename_flow_state_ = State_Confirm_Use;
}

void uiCharacterRenameInputDialog::SendCharacterNameChangePacket()
{
    MSG_CG_CHARNAME_CHANGE_SYN send_packet;
    send_packet.slot_index_ = character_slot_index_;
    strncpy(&(send_packet.character_name_[0]), rename_.c_str(), MAX_CHARNAME_LENGTH);
    send_packet.character_name_[MAX_CHARNAME_LENGTH] = '\0';
    GlobalFunc::SendPacket(CI_GAMESERVERIDX, &send_packet, sizeof(send_packet));
    
    rename_flow_state_ = State_None;
}

void uiCharacterRenameInputDialog::ProcessSuccessRename(MSG_BASE* recv_packet)
{
    MSG_CG_CHARNAME_CHANGE_ACK* success_rename = static_cast<MSG_CG_CHARNAME_CHANGE_ACK*>(recv_packet);

    ShowInterface(FALSE);

    TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR temp_message[INTERFACE_STRING_LENGTH] = {'\0', };
    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager == NULL)
    {
        return;
    }

    int number_need_rename =  g_CharSelectScene.GetNumOfNeedRename();

    BYTE slot_index = success_rename->slot_index_;
    CharListInfo* character_info = g_CharSelectScene.GetCharacterInfoBySlotIndex(slot_index);
    if (character_info != NULL)
    {
        strncpy(character_info->CharacterInfo.m_tszCharName, success_rename->character_name_, MAX_CHARNAME_LENGTH);
        character_info->CharacterInfo.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
        Player* selected_character = static_cast<Player*>(g_ObjectManager.GetObject(character_info->dwObjectKey));
        if (selected_character != NULL)
        {
            selected_character->SetName(success_rename->character_name_);
        }
    }
    g_CharSelectScene.UnsetRenameStatusByIndex(slot_index);

    if (number_need_rename > 1)
    {
        // case 1 ���� �� ������ ĳ���Ͱ� ���� �ִ� ���
        // 2273 ĳ���͸� ������ �Ϸ�Ǿ����ϴ�.\n�ٸ� �ߺ� ĳ���͸� ��� �����Ͻðڽ��ϱ�?
        g_InterfaceManager.GetInterfaceString(2273, message);
        DWORD dialog_id = ui_system_manager->Sys_ConfirmLock(message, FunctionConfirmContinue);
        uiSystemConfirm* confirm_dialog = GET_CAST_DIALOG(uiSystemConfirm, dialog_id);
        if (confirm_dialog != NULL)
        {
            // 2271 ��� �����ϱ�
            // 2272 ���� �����ϱ�
            confirm_dialog->SetBtnStringID(2271, 2272);
        }
    }
    else
    {
        // case 2 ���� �� ������ ĳ���Ͱ� ���� ���

        // 2274 ĳ���͸� ������ �Ϸ�Ǿ����ϴ�.\n�����ϼ̽��ϴ�.
        g_InterfaceManager.GetInterfaceString(2274, message);
        ui_system_manager->Sys_VerifyLock(message);
        uiBase* rename_dialog = GET_DIALOG(IM_CHARACTERSELECT_MANAGER::CHARACTER_SELECT_RENAME);
        if (rename_dialog != NULL)
        {
            rename_dialog->ShowWindow(FALSE);
        }
        g_CharSelectScene.ShowCharacterSelectSceneUI();
    }
}

void uiCharacterRenameInputDialog::ProcessErrorCode(MSG_BASE* recv_packet)
{
    MSG_CG_CHARNAME_CHANGE_NAK* rename_nak = static_cast<MSG_CG_CHARNAME_CHANGE_NAK*>(recv_packet);

    ShowInterface(FALSE);

    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager == NULL)
    {
        return;
    }

    TCHAR message[INTERFACE_STRING_LENGTH] = {'\0', };

    switch (rename_nak->result_)
    {
    case MSG_CG_CHARNAME_CHANGE_NAK::SUCCEED:
    case MSG_CG_CHARNAME_CHANGE_NAK::DB_FAIL:
    case MSG_CG_CHARNAME_CHANGE_NAK::DONOT_RENAME:
    case MSG_CG_CHARNAME_CHANGE_NAK::INDEX_INVALID:
        {
            // 70017 ��� �� �ٽ� �õ� �Ͻʽÿ�.
            g_InterfaceManager.GetInterfaceString(70017, message);
            ui_system_manager->Sys_VerifyLock(message);
#ifdef _DEV_VER
            GlobalFunc::DebugOutputMsg(eOUTPUT_MSG_TOP_SHOW, "CG_CHARNAME_CHANGE_NAK : ResultCode(%d) SlotIndex(%d)", rename_nak->result_, rename_nak->slot_index_);
#endif //_DEV_VER
        }
        break;

    case MSG_CG_CHARNAME_CHANGE_NAK::NOT_CONDITION:
        {
            g_InterfaceManager.GetInterfaceString(2279, message);
            ui_system_manager->Sys_VerifyLock(message);
        }
        break;
    }
}

void uiCharacterRenameInputDialog::SetNoticeText(TCHAR* notice_text)
{
    WzID control_id = GetControlID_byPosition(kControl_Text_T001);
    CCtrlStaticWZ* notice_text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ(control_id));
    if (notice_text_control == NULL)
    {
        return;
    }

    FTextParser ftext_parser;
    FTextSentence ftext_sentence;
    RECT text_rect = notice_text_control->GetSizeRect();

    notice_text_.Init(GetDrawBase(), text_rect);
    notice_text_.SetHCenterAlign(true);
    notice_text_.SetVCenterAlign(true);
    notice_text_.ClearSentence();

    ftext_parser.SetDefaultFont(StrToWzID("mn12"), kNotice_Text_Color);
    ftext_parser.Parser(notice_text, ftext_sentence);
    notice_text_.Append(ftext_sentence);
    notice_text_.convertLineInfo();
    notice_text_.SetBeginLineIndex(0);
}

#endif //_JP_20110222_INGAME_CHARNAME_CHANGE