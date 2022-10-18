#include "SunClientPrecompiledHeader.h"

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "uiSmartNpcDialogMan/uiSmartNpcDialogMan.h"
#include "interfacemanager.h"
#include "uiBase.h"

#include "uiSmartNpcDialogMan/uiSmartNpcDialogMenu.h"
#include "uiSmartNpcDialogMan/uiSmartNpcDialogReply.h"

#include "json/json.h"

const DWORD IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU = g_IDGenerator.GenerateID();
const DWORD IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY = g_IDGenerator.GenerateID();

class uiSmartNpcDialogMenu;
class uiSmartNpcDialogReply;


uiSmartNpcDialogMan::uiSmartNpcDialogMan()
{
}

uiSmartNpcDialogMan::~uiSmartNpcDialogMan()
{
}
uiSmartNpcDialog* uiSmartNpcDialogMan::ParserSmartDialogPacket(SmartDialogType type, CHAR* data_)
{
    Json::Value root;
    Json::Reader reader;

    bool parsingSuccessful = reader.parse(data_, root);
    if (parsingSuccessful == false)
    {
        return NULL;
    }

    int sub_type = root.get("TYPE", "999").asInt(); /*SmartDialogType_Max = 999*/

    switch (type)
    {
    case SmartDialogType_Menu:
        {
            const Json::Value buttons = root["BUTTONS"];
            int count = buttons.size();
            uiSmartNpcDialog* smart_npc_dialog = CreateSmartDialogTypeMenu(sub_type, count);
            if (smart_npc_dialog)
            {
                int id = root.get("ID", "0").asInt();
                smart_npc_dialog->SetID(id);
                std::string title = root.get("TITLE", "").asString();
                smart_npc_dialog->SetControlText(uiSmartNpcDialogMenu::kControl_Text_T999, title.c_str());
                std::string message = root.get("DIALOG", "").asString();
                smart_npc_dialog->SetControlText(uiSmartNpcDialogMenu::kControl_List_L000, message.c_str());
                
                std::string button_text;
                DWORD button_event_id;
                DWORD button_step;
                for (int i = 0; i < count; ++i)
                {
                    button_text = buttons[i]["TEXT"].asString();
                    smart_npc_dialog->SetControlText(uiSmartNpcDialogMenu::kControl_Button_B001 + i, button_text.c_str());
                    button_event_id = buttons[i]["EID"].asInt();
                    button_step = buttons[i]["STEP"].asInt();
                    smart_npc_dialog->SetControlEventID(uiSmartNpcDialogMenu::kControl_Button_B001 + i, button_event_id, button_step);
                }

                return smart_npc_dialog;
            }
        }
        break;
    case SmartDialogType_Reply:
        {
            uiSmartNpcDialog* smart_npc_dialog = CreateSmartDialogTypeReply(sub_type);
            if (smart_npc_dialog)
            {
                TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
                g_InterfaceManager.GetInterfaceString(155360, string, INTERFACE_STRING_LENGTH); // 155360 알림
                std::string title = root.get("TITLE", string).asString();
                smart_npc_dialog->SetControlText(uiSmartNpcDialogReply::kControl_Text_T999, title.c_str());
                std::string message = root.get("string", "").asString();
                smart_npc_dialog->SetControlText(uiSmartNpcDialogReply::kControl_List_L000, message.c_str());

                
                g_InterfaceManager.GetInterfaceString(100, string, INTERFACE_STRING_LENGTH);
                smart_npc_dialog->SetControlText(uiSmartNpcDialogReply::kControl_Button_B001, string); // 100 확인
                g_InterfaceManager.GetInterfaceString(155537, string, INTERFACE_STRING_LENGTH);
                smart_npc_dialog->SetControlText(uiSmartNpcDialogReply::kControl_Button_B002, string); // 155537 아니오
                g_InterfaceManager.GetInterfaceString(101, string, INTERFACE_STRING_LENGTH);
                smart_npc_dialog->SetControlText(uiSmartNpcDialogReply::kControl_Button_B003, string); // 101 취소
                

                // 슬롯이면
                uiSmartNpcDialogReply* reply_dialog = static_cast<uiSmartNpcDialogReply*>(smart_npc_dialog);
                reply_dialog->DeleteAllSlot();
                const Json::Value sloats = root["SLOTS"];
                int count = sloats.size();

                DWORD item_code;
                DWORD item_number;
                for (int i = 0; i < count; ++i)
                {
                    item_code = sloats[i]["ITEMCODE"].asInt();
                    item_number = sloats[i]["ITEMNUMBER"].asInt();
                    reply_dialog->SetSlotItem(i, item_code, item_number);
                }
                return smart_npc_dialog;
            }
        }
        break;
    }
    
    return NULL;
    //

}
uiSmartNpcDialog* uiSmartNpcDialogMan::CreateSmartNpcDialog(SmartDialogType type, BYTE sub_type)
{
    switch (type)
    {
    case SmartDialogType_Menu:
        {
            return CreateSmartDialogTypeMenu(1, sub_type);
        }
        break;
    case SmartDialogType_Reply:
        {
            return CreateSmartDialogTypeReply(sub_type);
        }
        break;
    }

    return NULL;
}

BOOL uiSmartNpcDialogMan::CreateSmartDialog(DWORD dialog_key)
{
    CDialogManager* dialog_manager = g_InterfaceManager.GetDialogManager();
    if (dialog_manager == NULL)
    {
        return FALSE;
    }

    CDialogWZ* dialog_pointer = dialog_manager->CreateDialogWZ(dialog_key, 16);
    if (dialog_pointer == NULL)
    {
        return FALSE;
    }

    dialog_pointer->GetIntersectRectList().clear();
    RECT rect;
    rect.left   = 11;
    rect.top    = 11;
    rect.right  = 405;
    rect.bottom = 20;
    dialog_pointer->SetMoveRECT(rect);
    
    CControlWZ* client_control = dialog_pointer->GetClientWZ();
    if (client_control == NULL)
    {
        dialog_manager->RemoveDialogWZ(dialog_key);
        return FALSE;
    }
    client_control->MoveWindowWZ(0, 0, 352, 558); //0, 0, 446, 406

    SI_TSUBINFO image_sub_info;
    image_sub_info.Initialize();
    image_sub_info.m_blIs = true;

    image_sub_info.m_sImageName = "Common_bg01.tga";
    image_sub_info.m_blUseGradation = false;
    image_sub_info.m_fTileCornerX = 46;
    image_sub_info.m_fTileCornerY = 48;

    image_sub_info.m_nType = SI_TSUBINFO::DrawType_TileExpand;
    image_sub_info.m_sFRect.fTopX = 0;
    image_sub_info.m_sFRect.fTopY = 611;
    image_sub_info.m_sFRect.fWidth = 113;
    image_sub_info.m_sFRect.fHeight = 371;
    image_sub_info.m_color = 0xffffffff; //RGBA(255,255,255,255);

    client_control->SetCtrlDetailImageFormat(E_BASE, &image_sub_info);

    client_control->SetIsOutline(false);
    client_control->SetOutlineColor(RGBA(0,0,0,255));
    
    return TRUE;
}

uiSmartNpcDialog* uiSmartNpcDialogMan::CreateSmartDialogTypeMenu(BYTE sub_type, BYTE button_count)
{
    if (button_count > MaxMenuButton)
    {
        // 버튼 최대 수 초과
        return NULL;
    }

    DWORD dialog_key = IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_MENU;
    uiSmartNpcDialogMenu *smart_dialog = new uiSmartNpcDialogMenu();
    smart_dialog->SetButtonCount(button_count);
    smart_dialog->SetSubType(sub_type);

    uiBase* ui_base = GET_CAST_DIALOG(uiBase, dialog_key);
    if (ui_base != NULL)
    {
        // 이미 있으면 제거
        g_InterfaceManager.DeleteInterface(dialog_key);
    }
    smart_dialog->SetDialogKey(dialog_key);

    // 다이얼로그 생성
    if (CreateSmartDialog(dialog_key) == FALSE)
    {
        SAFE_DELETE(smart_dialog);
        return NULL;
    }

    CDialogWZ *dialog_pointer = g_InterfaceManager.GetDialogManager()->GetDialogWZ(dialog_key);
    dialog_pointer->GetClientWZ()->MoveWindowWZ(0, 0, 310, 458);

    // 인터페이스 연결
    if (g_InterfaceManager.InsertInterface(smart_dialog, dialog_pointer, TRUE))
    {
        smart_dialog->SetActivated(true);
    }
    else
    {
        SAFE_DELETE(smart_dialog);
        return NULL;
    }

    // 컨트롤 생성
    if (smart_dialog->CreateControls(sub_type) == FALSE)
    {
        g_InterfaceManager.DeleteInterface(dialog_key);
        return NULL;
    }

    return smart_dialog;

}
uiSmartNpcDialog* uiSmartNpcDialogMan::CreateSmartDialogTypeReply(BYTE sub_type)
{
    if (sub_type <= 0 || sub_type >= SmartDialogSubType_Max)
    {
        // 잘못된 서브타입
        return NULL;
    }
    DWORD dialog_key = IM_SMARTNPCDIALOG_MANAGER::SMARTNPCDIALOG_REPLY;
    uiSmartNpcDialogReply *smart_dialog = new uiSmartNpcDialogReply();
    smart_dialog->SetSubType(sub_type);

    uiBase* ui_base = GET_CAST_DIALOG(uiBase, dialog_key);
    if (ui_base != NULL)
    {
        // 이미 있으면 제거
        g_InterfaceManager.DeleteInterface(dialog_key);
    }
    smart_dialog->SetDialogKey(dialog_key);

    // 다이얼로그 생성
    if (CreateSmartDialog(dialog_key) == FALSE)
    {
        SAFE_DELETE(smart_dialog);
        return NULL;
    }

    CDialogWZ *dialog_pointer = g_InterfaceManager.GetDialogManager()->GetDialogWZ(dialog_key);
    //
    static float test_size_int = 280;
    static float test_size_int2 = 170;
    //
    dialog_pointer->GetClientWZ()->MoveWindowWZ(0, 0, test_size_int, test_size_int2);

    // 인터페이스 연결
    if (g_InterfaceManager.InsertInterface(smart_dialog, dialog_pointer, TRUE))
    {
        smart_dialog->SetActivated(true);
    }
    else
    {
        SAFE_DELETE(smart_dialog);
        return NULL;
    }

    // 컨트롤 생성
    if (smart_dialog->CreateControls(sub_type) == FALSE)
    {
        g_InterfaceManager.DeleteInterface(dialog_key);
        return NULL;
    }

    return smart_dialog;

}


#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
