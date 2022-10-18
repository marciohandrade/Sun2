#include "SunClientPrecompiledHeader.h"

#include "TabCheckButtonControlManager.h"
#include "uiBase.h"
#include "InterfaceManager.h"
#include "DlgPosInfoParser.h"
#include "GlobalFunc.h"

TabCheckButtonControllManager::TabCheckButtonControllManager()
{
    tab_dialog_container_.clear();
    ZeroMemory(&tab_dialog_data_, sizeof(tab_dialog_data_));
}

TabCheckButtonControllManager::~TabCheckButtonControllManager()
{
    
}

bool TabCheckButtonControllManager::Initialize(DWORD tab_num, 
                                              DWORD key_queue_type, 
                                              POINT* dialog_position, 
                                              DWORD init_tab_index/* = 0*/)
{
    if (tab_num < 1)
    {
        return false;
    }

    tab_dialog_container().resize(tab_num, NULL);
    tab_dialog_data().key_queue_type = key_queue_type;
    tab_dialog_data().current_tab_index = init_tab_index;
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	tab_dialog_data().prev_tab_index = init_tab_index;
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    if (dialog_position != NULL)
    {
        tab_dialog_data().dialog_position_x = static_cast<float>(dialog_position->x);
        tab_dialog_data().dialog_position_y = static_cast<float>(dialog_position->y);
    }

    return true;
}

void TabCheckButtonControllManager::PushTab(DWORD tab_index, uiBase* tab_dialog)
{
    if (CheckValidTabIndex(tab_index) == false)
    {
        return;
    }

    tab_dialog_container()[tab_index] = tab_dialog;
}

void TabCheckButtonControllManager::ShowDialog(DWORD tab_index, bool is_show)
{
    if (CheckValidTabIndex(tab_index) == false)
    {
        return;
    }

    uiBase* dialog_ptr = tab_dialog_container()[tab_index];
    if (dialog_ptr == NULL)
    {
        return;
    }

    // 키큐해제
    ReleaseKeyQueueType();

    if (is_show == true)
    {
        // 키큐등록
        RegisterKeyQueueType(dialog_ptr);

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		tab_dialog_data().prev_tab_index = tab_dialog_data().current_tab_index;
#endif // _NA_008472_20150903_IMPROVED_BALANCE

        // 현재다이얼로그 세팅
        tab_dialog_data().current_tab_index = tab_index;

        dialog_ptr->ShowInterfaceWithSound(TRUE);

        dialog_ptr->MoveWindowWZ(tab_dialog_data().dialog_position_x, 
            tab_dialog_data().dialog_position_y);
    }
    else
    {
        if (tab_dialog_data().current_tab_index != tab_index)
        {
            return;
        }

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
		tab_dialog_data().prev_tab_index = tab_dialog_data().current_tab_index;
#endif // _NA_008472_20150903_IMPROVED_BALANCE

        dialog_ptr->ShowInterfaceWithSound(FALSE);

        SaveDialogPosition(dialog_ptr);
    }
}

void TabCheckButtonControllManager::ShowCurrentDialog(bool is_show)
{
    ShowDialog(tab_dialog_data().current_tab_index, is_show);
}

void TabCheckButtonControllManager::OnClickTab(DWORD tab_index)
{
    if (CheckValidTabIndex(tab_index) == false)
    {
        return;
    }

    DWORD current_tab_index = tab_dialog_data().current_tab_index;
    if (tab_index == current_tab_index)
    {
        return;
    }

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
	tab_dialog_data().prev_tab_index = tab_dialog_data().current_tab_index;
#endif // _NA_008472_20150903_IMPROVED_BALANCE
    tab_dialog_data().current_tab_index = tab_index;

    uiBase* dialog_ptr = tab_dialog_container()[current_tab_index];
    if (dialog_ptr == NULL)
    {
        return;
    }

    dialog_ptr->ShowInterface(FALSE);
    ReleaseKeyQueueType();
    SaveDialogPosition(dialog_ptr);
    
    dialog_ptr = tab_dialog_container()[tab_index];
    if (dialog_ptr == NULL)
    {
        return;
    }

    dialog_ptr->ShowInterface(TRUE);
    RegisterKeyQueueType(dialog_ptr);
    dialog_ptr->MoveWindowWZ(tab_dialog_data().dialog_position_x, 
        tab_dialog_data().dialog_position_y);
}

uiBase* TabCheckButtonControllManager::GetTabDialog(DWORD tab_index)
{
    if (CheckValidTabIndex(tab_index) == false)
    {
        return NULL;
    }

    return tab_dialog_container()[tab_index];
}

uiBase* TabCheckButtonControllManager::GetCurrentTabDialog()
{
    if (CheckValidTabIndex(tab_dialog_data().current_tab_index) == false)
    {
        return NULL;
    }

    return tab_dialog_container()[tab_dialog_data().current_tab_index];
}

void TabCheckButtonControllManager::RegisterKeyQueueType(uiBase* tab_dialog)
{
    if (tab_dialog == NULL)
    {
        return;
    }

    if (tab_dialog_data().key_queue_type == 0)
    {
        return;
    }

    keyMsg key_message;
    ZeroMemory(&key_message, sizeof(key_message));
    key_message.dwType = tab_dialog_data().key_queue_type;
    key_message.wParam = tab_dialog->GetDialogWZ()->GetDlgID();
    key_message.DoSomething = GlobalFunc::DoShowWindowEx;
    g_KeyQueueManager.PushBack(key_message);
}

void TabCheckButtonControllManager::ReleaseKeyQueueType()
{
    if (tab_dialog_data().key_queue_type == 0)
    {
        return;
    }

    g_KeyQueueManager.DeleteMsg(tab_dialog_data().key_queue_type);
}

void TabCheckButtonControllManager::SaveDialogPosition(uiBase* tab_dialog)
{
    if (tab_dialog != NULL)
    {
        RECT dialog_rect;
        tab_dialog->GetDialogWZ()->GetClientRect(&dialog_rect);

        tab_dialog_data().dialog_position_x = static_cast<float>(dialog_rect.left);
        tab_dialog_data().dialog_position_y = static_cast<float>(dialog_rect.top);
    }
}

bool TabCheckButtonControllManager::CheckValidTabIndex(DWORD tab_index)
{
    if (tab_index >= tab_dialog_container().size())
    {
        return false;
    }

    return true;
}

void TabCheckButtonControllManager::UpdateDialogFocus(uiBase* main_dialog)
{
    if (main_dialog == NULL)
    {
        return;
    }

    uiBase* sub_dialog = GetCurrentTabDialog();
    if (sub_dialog == NULL)
    {
        return;
    }

    if (main_dialog->HaveFocus() == TRUE)
    {
        GlobalFunc::MoveToTopDialog(sub_dialog->GetDialogWZ()->GetDlgID());
    }
    else if (sub_dialog->HaveFocus() == TRUE)
    {
        GlobalFunc::MoveToTopDialog(main_dialog->GetDialogWZ()->GetDlgID());
    }
}
