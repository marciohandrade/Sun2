#include "SunClientPrecompiledHeader.h"
#ifdef _DEV_VER
#include "uiGMWindowMan/uiGMWindowMan.h"
#include "uiGMItemResetWindow/uiGMItemResetWindow.h"
#include "GlobalFunc.h"
#include "Mouse.h"

const int k_LISTMARGIN= 8;
const int k_HEIGHT_GAP = 18;
const int k_WIDTH_GAP = 10;
const int k_DIALOGWIDTH = 67;

void uiGMItemResetWindow::RegisterControl()
{
    RegisterControlPosition(StrToWzID("L001"), kControl_Button_L001);
}

uiGMItemResetWindow::uiGMItemResetWindow(InterfaceManager* ui_manager) : uiBase(ui_manager)
{
    ui_gm_window_manager_ = NULL;
    list_control_ = NULL;
}

uiGMItemResetWindow::~uiGMItemResetWindow(void)
{
}

void uiGMItemResetWindow::Init(CDrawBase* drawbase)
{
    uiBase::Init(drawbase);
    ui_gm_window_manager_ = static_cast<uiGMWindowMan*>(GetMan());

    if (ui_gm_window_manager_ != NULL)
    {
        item_option_parametas_ = *(ui_gm_window_manager_->item_parametas());
    }

    RegisterControl();

    list_control_ = static_cast<CCtrlListWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Button_L001)));
    if (list_control_ != NULL)
    {
        TCHAR *reset_value_string[kReset_Type_MAX] = { "모두", "시작값", "옵션", "필터", /*"검색어",*/ "히스토리", "즐겨찾기"};

        int list_item_index = 0;
        for (int index = 0; index < kReset_Type_MAX;++index)
        {
            list_item_index = list_control_->InsertItemBack();
            GlobalFunc::SetItem(list_control_, list_item_index, reset_value_string[index], 
                StrToWzID("mn12"), DT_CENTER | DT_VCENTER, RGBA(0,0,0,0), 
                RGBA(255,255,255,192), RGBA(255,255,0,255));
        }
        list_control_->SetSellHeight(k_HEIGHT_GAP);
        list_control_->SetCurSel(0);

        int iListHeight = (kReset_Type_MAX * k_HEIGHT_GAP);
        RECT rect;
        GetDialogWZ()->GetClientRect(&rect);
        GetDialogWZ()->MoveWindowWZ((float)rect.left, (float)rect.top, 
            (float)(k_DIALOGWIDTH+k_WIDTH_GAP), (float)(iListHeight + k_HEIGHT_GAP));
        list_control_->MoveWindowWZ((float)(rect.left + (float)k_LISTMARGIN), 
            (float)(rect.top+k_LISTMARGIN), (float)k_DIALOGWIDTH, (float)iListHeight);
    } 
}

void uiGMItemResetWindow::Release()
{
}

void uiGMItemResetWindow::MessageProc(SI_MESSAGE* message)
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

void uiGMItemResetWindow::OnLButtonClick(SI_MESSAGE* message)
{
    POSTYPE position = static_cast<POSTYPE>(GetControlPosition_byID(message->dwCtrlID));  
    switch (position)
    {
    case kControl_Button_L001:
        {
            BYTE menu_position = list_control_->GetCurSel();
            switch (menu_position)
            {
            case kReset_Type_All:        // 모두
                ResetAll();
                break;
            case kReset_Type_Init:      // 실행시값
                ResetInit();
                break;
            case kReset_Type_Parameta:   // 속성
                ResetParameta();
                break;
            case kReset_Type_Filter:     // 필터
                ResetFilter();
                break;
            /*case kReset_Type_FindWord:   // 검색
                ResetFindWord();
                break;*/
            case kReset_Type_Created:    // 생성
                ResetHistroy();
                break;
            case kReset_Type_History:    // 즐겨찾기
                ResetBookmark();
                break;
            }
            ShowInterface(FALSE);
        }
        break;
    }
}

void uiGMItemResetWindow::OnUpdateSolarDialog()
{
    RECT control_rect = {0, 0, 0, 0};
    if (ui_gm_window_manager_)
    {
        ui_gm_window_manager_->GetResetWindowPosition(&control_rect);
        GetDialogWZ()->MoveWindowWZ((control_rect.right - 5.0f), (control_rect.top + 8.0f));
    }
}

void uiGMItemResetWindow::OnShowWindow(BOOL val)
{
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_GM_Item_Window;
        msg.wParam = IM_GM_WINDOW_MANAGER::GM_ITEM_RESET_WINDOW;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_GM_Item_Reset_Window);
    }
}

void uiGMItemResetWindow::ResetAll()
{
    ResetParameta();
    ResetFilter();
}

void uiGMItemResetWindow::ResetInit()
{
    if (ui_gm_window_manager_ == NULL)
    {
        return;
    }
    ui_gm_window_manager_->SetItemOptionParametas(&item_option_parametas_);
}

void uiGMItemResetWindow::ResetParameta()
{
    if (ui_gm_window_manager_ == NULL)
    {
        return;
    }

    ITEM_PARAMETAS* item_parametas = ui_gm_window_manager_->GetItemOption();
    if (item_parametas)
    {
        item_parametas->item_code = 0;
        item_parametas->item_count = 0;
        item_parametas->overlapping = true;
        item_parametas->item_type = ITEM_TYPE_NORMAL;
        item_parametas->enchant_level = 0;
        item_parametas->socket_count = 0;
        ui_gm_window_manager_->SetItemOptionParametas(item_parametas);
    }
}

void uiGMItemResetWindow::ResetFilter()
{
    if (ui_gm_window_manager_ == NULL)
    {
        return;
    }

    ITEM_PARAMETAS* item_parametas = ui_gm_window_manager_->GetItemOption();
    item_parametas->filter_main = FILTER_ALL;
    for (int i = 0; i < FILTER_MAX; ++i)
    {
        item_parametas->filter_subs[i] = 0;
    }
    item_parametas->class_type = eCHAR_NONE;
    ui_gm_window_manager_->SetItemOptionParametas(item_parametas);
}

void uiGMItemResetWindow::ResetFindWord()
{
}

void uiGMItemResetWindow::ResetHistroy()
{
    if (ui_gm_window_manager_)
    {
        ui_gm_window_manager_->DeleteAllHistroys();
    }
}

void uiGMItemResetWindow::ResetBookmark()
{
    if (ui_gm_window_manager_)
    {
        ui_gm_window_manager_->DeleteAllBookMarks();
    }
}
#endif//_DEV_VER