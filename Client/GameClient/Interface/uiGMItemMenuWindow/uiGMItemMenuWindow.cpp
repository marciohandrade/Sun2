#include "SunClientPrecompiledHeader.h"
#ifdef _DEV_VER
#include "uiGMWindowMan/uiGMWindowMan.h"
#include "uiGMItemMenuWindow/uiGMItemMenuWindow.h"
#include "GlobalFunc.h"
#include "Mouse.h"
#include "ItemInfoParser.h"

const int k_LISTMARGIN= 8;
const int k_HEIGHT_GAP = 18;
const int k_WIDTH_GAP = 10;
const int k_DIALOGWIDTH = 67;

TCHAR *kReset_Value_String[uiGMItemMenuWindow::kItem_Menu_MAX] = 
{ 
    "����", "�Ϲݻ���", "���׸���", "��Ų����", "���ϻ���", "���Ͼ���", "����Ƽ��", "��Ű����", 
    "���ӻ���", "���ã��", "��ϻ���",
    "1�ð�", "30��", "60��", "90��", "�����", "������"
};

const COLORREF kBackColor = RGBA(0,0,0,0);
const COLORREF kTextColor = RGBA(255,255,255,192);
const COLORREF kFocusTextColor = RGBA(255,255,0,255);
const DWORD kTextFont = StrToWzID("mn12");

void uiGMItemMenuWindow::RegisterControl()
{
    RegisterControlPosition(StrToWzID("L001"), kControl_Button_L001);
}

uiGMItemMenuWindow::uiGMItemMenuWindow(InterfaceManager* ui_manager) : uiBase(ui_manager)
{
    ui_gm_window_manager_ = NULL;
    list_control_ = NULL;
    selected_item_code_ = 0;
}

uiGMItemMenuWindow::~uiGMItemMenuWindow(void)
{
}

void uiGMItemMenuWindow::Init(CDrawBase* drawbase)
{
    uiBase::Init(drawbase);
    ui_gm_window_manager_ = static_cast<uiGMWindowMan*>(GetMan());

    if (ui_gm_window_manager_ != NULL)
    {
        item_option_parametas_ = *(ui_gm_window_manager_->item_parametas());
    }

    RegisterControl();
    menu_list.clear();

    list_control_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_L001)));
}

void uiGMItemMenuWindow::Release()
{
}

void uiGMItemMenuWindow::MessageProc(SI_MESSAGE* message)
{
    switch(message->eResultMsg) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            OnLButtonClick(message);
        }
        break;		
    case RT_MSG_MOUSEOVER:
        {
            POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));  
            switch(position)
            {
            case kControl_Button_L001:
                {
                    int mouse_y = Mouse::Instance()->GetMouseY();
                    RECT rc_listctrl = list_control_->GetSizeRect();
                    int mouse_position = mouse_y - rc_listctrl.top;

                    if(mouse_position > 0)
                    {
                        int cursel = mouse_position / k_HEIGHT_GAP;
                        list_control_->SetCurSel(cursel);
                    }
                }
                break;
            }
        }
        break;
    }
}

void uiGMItemMenuWindow::OnLButtonClick(SI_MESSAGE* message)
{
    POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));  
    switch (position)
    {
    case kControl_Button_L001:
        {
            BYTE menu_position = list_control_->GetCurSel();
            if (ProcessMenu(menu_position) == false)
            {
                ShowInterface(FALSE);
            }
        }
        break;
    }
}

bool uiGMItemMenuWindow::ProcessMenu(BYTE menu_position)
{
    size_t size = menu_list.size();
    if (size < 0)
    {
        return false;
    }

    bool is_create = true;
    bool is_bookmark_add = true;
    ITEM_PARAMETAS* item_parameta;
    if (ui_gm_window_manager_)
    {
        item_parameta = ui_gm_window_manager_->GetItemOption();
    }
    else
    {
        return false;
    }

    if (item_parameta == NULL)
    {
        return false;
    }
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(selected_item_code_);

    switch (menu_list[menu_position])
    {
    case kMenu_Create_Setting:       // ���� (������ ������)
        // none
        break;
    case kMenu_Create_Normal:     // �Ϲݻ���
        item_parameta->item_type = ITEM_TYPE_NORMAL;
        break;
    case kMenu_Create_Ethria:    // ���׸��� ����
        if (item_parameta->item_type == ITEM_TYPE_DIVINE)
        {
            item_parameta->item_type = ITEM_TYPE_DIVINE_ETHERIA;// ������� ��� ����� ���׸���
        }
        else
        {
            item_parameta->item_type = ITEM_TYPE_ETHERIA;// �Ϲ��� ��� ���׸���
        }
        break;
    case kMenu_Create_Skin:       // ��Ų ����
        item_parameta->item_type = ITEM_TYPE_SKIN;
        break;
    case kMenu_Create_Socket:     // ���� ����
        item_parameta->socket_count = 5;
        break;
    case kMenu_Create_NoSocket:   // ���� ���� ����
        item_parameta->socket_count = 6;
        break;
    case kMenu_Add_Bookmark:      // ���ã�� �߰�
        is_create = false;
        is_bookmark_add = true;
        break;
    case kMenu_Create_Visible_All:// ���� ����
        {
            ui_gm_window_manager_->CreateItemFromList();//ȭ�鿡 ���̴°� ��� ����
            return false;
        }
        break;
    case kMenu_Delete_Bookmark:   // ���ã�� ����
        is_create = false;
        is_bookmark_add = false;
        break;
    case kMenu_Create_Lucky:   // ��Ű ����
        {
            UpdateLuckyMenus();
            return true;
        }
    case kMenu_Create_Limited:    // ����Ƽ�� ����
        {
            ui_gm_window_manager_->CreateLimited(item_info, item_parameta);//����Ƽ�� ������ ����
            return false;
        }
        break;
    case kMenu_Lucky_1_Hour:   // ��Ű ������ �Ⱓ 1�ð�
    case kMenu_Lucky_30_Day:   // ��Ű ������ �Ⱓ 30��
    case kMenu_Lucky_60_Day:   // ��Ű ������ �Ⱓ 60��
    case kMenu_Lucky_90_Day:   // ��Ű ������ �Ⱓ 90��
    case kMenu_Lucky_Finite:   // ��Ű ������ �Ⱓ ����
    case kMenu_Lucky_Infinite: // ��Ű ������ �Ⱓ ������
        {
            int lucky_expiry = (menu_list[menu_position] - kMenu_Lucky_1_Hour);// �Ⱓ �� ���
            ui_gm_window_manager_->CreateItemLucky(item_info, item_parameta, lucky_expiry);//��Ű ������ ����
            return false;
        }
        break;
    default:
        return false;
    }

    if (is_create == true)
    {
        ui_gm_window_manager_->CreateItemFromParameta(item_info, item_parameta);
    }
    else
    {
        if (is_bookmark_add == true)
        {
            ui_gm_window_manager_->AddBookMark(selected_item_code_);
        }
        else
        {
            ui_gm_window_manager_->DeleteBookMark(selected_item_code_);
        }
    }
    return false;
}

void uiGMItemMenuWindow::UpdateMenus(BASE_ITEMINFO* item_info, bool is_bookmark_state)
{
    menu_list.clear();
    if (list_control_)
    {
        list_control_->DeleteItemAll();
    }

    if (item_info == NULL)
    {
        ShowInterface(false);
        return;
    }

    selected_item_code_ = item_info->m_Code;
    ITEM_PARAMETAS* item_parametas = ui_gm_window_manager_->item_parametas();

    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();

    menu_list.push_back(kMenu_Create_Setting);   // ����
    
    bool is_equip = (item_info->IsWeapon() || item_info->IsArmor());
    if (is_equip || item_info->m_wType == eITEMTYPE_PET)
    {
        if (item_parametas->item_type != ITEM_TYPE_NORMAL && is_equip)
        {
            menu_list.push_back(kMenu_Create_Normal);   // �Ϲݻ���
        }
        if (item_parametas->item_type != ITEM_TYPE_ETHERIA && item_parametas->item_type != ITEM_TYPE_DIVINE_ETHERIA)
        {
            menu_list.push_back(kMenu_Create_Ethria);   // ���׸��� ����
        }
        if (item_parametas->item_type != ITEM_TYPE_SKIN)
        {
            menu_list.push_back(kMenu_Create_Skin);     // ��Ų ����
        }
    }
    if (is_equip)
    {
        if (item_parametas->socket_count <= 0 || item_parametas->socket_count >= (SOCKET_MAX + 1))
        {   // ���� ��Ȯ��(0) �̰ų� ���� ����(6) �϶��� ���� ���� �޴� Ȱ��ȭ
            menu_list.push_back(kMenu_Create_Socket);   // ���� ����
        }
        else
        {
            menu_list.push_back(kMenu_Create_NoSocket);   // ���� ����
        }        
    }

    if (item_info->IsArmor())
    {
        switch (item_info->GetSubTypeDetail())
        {
        case ItemSubTypeDetail::kGeneral:
            if (item_info->m_Code < 10000) //����Ƽ��� 4�ڸ��� ���� ��
            {
                menu_list.push_back(kMenu_Create_Limited);   // ����Ƽ�� ����
            }
            break;
        case ItemSubTypeDetail::kFate:
            menu_list.push_back(kMenu_Create_Lucky);   // ��Ű ����
            break;
        }
    }

    menu_list.push_back(kMenu_Create_Visible_All);   // ���� ����

    if (is_bookmark_state == true)
    {
        menu_list.push_back(kMenu_Delete_Bookmark); // ���ã�� ����
    }
    else
    {
        menu_list.push_back(kMenu_Add_Bookmark);    // ���ã�� �߰�
    }

    UpdateMenuList(mouse_x, mouse_y);
}

void uiGMItemMenuWindow::UpdateMenuList(int mouse_x, int mouse_y)
{
    // ����Ʈ�� ���鼭 ����
    if (list_control_ != NULL)
    {
        size_t menu_size = menu_list.size();
        int list_item_index = 0;
        BYTE menu_id = 0;
        for (size_t index = 0; index < menu_size;++index)
        {
            list_item_index = list_control_->InsertItemBack();
            menu_id = menu_list[index];
            GlobalFunc::SetItem(list_control_, list_item_index, kReset_Value_String[menu_id], 
                kTextFont, DT_CENTER | DT_VCENTER, kBackColor, kTextColor, kFocusTextColor);
        }
        list_control_->SetSellHeight(k_HEIGHT_GAP);
        list_control_->SetCurSel(-1);

        int iListHeight = (menu_size * k_HEIGHT_GAP);
        RECT rect;
        GetDialogWZ()->GetClientRect(&rect);
        rect.top = mouse_y + 10;
        rect.left = mouse_x + 10;
        GetDialogWZ()->MoveWindowWZ((float)rect.left, (float)rect.top, 
            (float)(k_DIALOGWIDTH +k_WIDTH_GAP), (float)(iListHeight + k_HEIGHT_GAP));
        list_control_->MoveWindowWZ((float)(rect.left + (float)k_LISTMARGIN), 
            (float)(rect.top+k_LISTMARGIN), (float)k_DIALOGWIDTH, (float)iListHeight);
    }
    ShowInterface(TRUE);
}

void uiGMItemMenuWindow::UpdateLuckyMenus()
{
    menu_list.clear();
    if (list_control_)
    {
        list_control_->DeleteItemAll();
    }

    menu_list.push_back(kMenu_Lucky_1_Hour);   // 1�ð�
    menu_list.push_back(kMenu_Lucky_30_Day);   // 30��
    menu_list.push_back(kMenu_Lucky_60_Day);   // 60��
    menu_list.push_back(kMenu_Lucky_90_Day);   // 90��
    menu_list.push_back(kMenu_Lucky_Finite);   // ����
    menu_list.push_back(kMenu_Lucky_Infinite);   // ������

    int dialog_x = static_cast<int>(GetDialogWZ()->GetDlgX());
    int dialog_y = static_cast<int>(GetDialogWZ()->GetDlgY());
    UpdateMenuList(dialog_x - 10, dialog_y - 10);
}

void uiGMItemMenuWindow::OnShowWindow(BOOL val)
{
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_GM_Item_Menu_Window;
        msg.wParam = IM_GM_WINDOW_MANAGER::GM_ITEM_MENU_WINDOW;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_GM_Item_Menu_Window);
    }
}
#endif//_DEV_VER