#include "SunClientPrecompiledHeader.h"
#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
#include "uiTextureViewerSearchWindow/uiTextureViewerSearchWindow.h"
#include "uiTextureViewerTextureWindow/uiTextureViewerTextureWindow.h"
#include "uiTextureViewerMan/uiTextureViewerMan.h"
#include "GlobalFunc.h"
#include "Mouse.h"

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);   // X 버튼
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);   // 생성
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);   // 입장예약(닫기)
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);   // 예약
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);   // 머징(변경)
    RegisterControlPosition(StrToWzID("TI00"), kControl_Text_T100);     // 방제목
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);     // 다이얼로그 제목
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);     // 이름(static)
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);     // 비번(static)
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_T001);    
    RegisterControlPosition(StrToWzID("E001"), kControl_Edit_E001);//이름
    RegisterControlPosition(StrToWzID("E002"), kControl_Edit_E002);//비번
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("V001"), kControl_Scroll_V001);
    RegisterControlPosition(StrToWzID("L000"), kControl_Picture_L000); // 그림그리는 곳
}

//-----------------------------------------------------------------------------------------//

uiTextureViewerSearchWindow::uiTextureViewerSearchWindow(InterfaceManager* ui_manager):
uiBase(ui_manager),
tree_controller_(NULL),
sel_item_(NULL),
Is_edit_mode_(false),
Is_start(false),
Is_grid_mode_(false),
scroll_tick_move_(1)
{
    ui_edit_mode_texture_ = INVALID_HANDLE_VALUE;
    ui_grid_mode_texture_ = INVALID_HANDLE_VALUE;
}

//-----------------------------------------------------------------------------------------//

uiTextureViewerSearchWindow::~uiTextureViewerSearchWindow(void)
{
	this->Release();
	CDialogWZ* dialog_ptr = GetDialogWZ();

	if (dialog_ptr != NULL)
	{
		STD_UI_INTERSECT_RECT& intersect_list = dialog_ptr->GetIntersectRectList();
		intersect_list.clear();
	}

    file_list_vector_.clear();

    if (tree_controller_)
    {
        tree_controller_->Release();

        delete tree_controller_;
        tree_controller_ = NULL;
    }

    file_list_.clear();

    if (ui_edit_mode_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(ui_edit_mode_texture_);
    }
    if (ui_grid_mode_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(ui_grid_mode_texture_);
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::Init( CDrawBase* draw_base )
{
    // 컨트롤Pos 지정
    RegisterControl();

    // 컨트롤러 셋팅
    SetMode(kCreateMode);

	uiBase::Init(draw_base);

    // 매니져 셋팅
	ui_texture_viewer_manager_ = static_cast<uiTextureViewerMan*> (GetMan());
    assert(ui_texture_viewer_manager_);

    // 기본적인 텍스쳐를 로딩해 놓는다.
    if (ui_edit_mode_texture_ == INVALID_HANDLE_VALUE)
    {
        ui_edit_mode_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\24_3_Skip.tga",
            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }
    if (ui_grid_mode_texture_ == INVALID_HANDLE_VALUE)
    {
        ui_grid_mode_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture("Data\\Interface\\24_3_Skip.tga",
            TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
    }

    // list_control_ 가져오기
    list_control_ = 
        static_cast<CCtrlListWZ*> (GetControlWZ(GetControlID_byPosition(kControl_Picture_L000)));
    assert(list_control_);

    if (list_control_)
    {
        list_control_->ShowWindowWZ(WZ_SHOW);

        // 한번의 휠 동작으로 3칸씩 이동 하도록 한다.
        set_scroll_tick_move_value(3);
    }

    vscroll_control_ = 
        static_cast<CCtrlVScrollWZ*> (GetControlWZ(GetControlID_byPosition(kControl_Scroll_V001)));
    assert(vscroll_control_);

    if (vscroll_control_)
    {
        // TreeController 생성
        tree_controller_ = TreeController::CreateTree(20, 20, vscroll_control_, list_control_);
        if(tree_controller_)
        {
            // 스크롤 Up이나 Down 일 때 3칸씩 움직이도록 한다.
            tree_controller_->set_scroll_tick_move_value(3);
        }
    }
    assert(tree_controller_);
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::Release()
{
    if (list_control_)
    {
        list_control_->DeleteItemAll();
        list_control_->SetCurSel(-1);
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::OnUpdateSolarDialog()
{
	CDialogWZ* dialog_ptr = GetDialogWZ();

    if (dialog_ptr == NULL)
        return;

    RECT control_rect;
    GetDialogWZ()->GetClientRect(&control_rect);

    uiTextureViewerTextureWindow* texture_viewer_texture_window_ptr = 
        static_cast<uiTextureViewerTextureWindow*> (static_cast<uiTextureViewerMan*>(GetMan())->
        GetTextureViewerWindow(UI_TEXTURE_VIEWER_TEXTURE_WINDOW));

    if (texture_viewer_texture_window_ptr == NULL)
        return;

    DWORD focus_dialog_id = 0;

    // 텍스쳐 윈도우가 뷰어 탐색기를 따라가도록 셋팅
    if (dialog_ptr && texture_viewer_texture_window_ptr)
    {
        texture_viewer_texture_window_ptr->MoveWindowWZ(control_rect.right, control_rect.top);
    }

    focus_dialog_id = IM_TEXTUREVIEWER_MANAGER::TEXTUREVIEWER_SEARCH_WINDOW;

    if (GetDialogWZ()->HaveFocus() == true && focus_dialog_id)
    {
        GlobalFunc::MoveToTopDialog(focus_dialog_id);
    }

    VScrollDialog::Update();
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::OnRenderSolarDialog()
{
    RECT DestRect = GetControlWZ_byPosition(kControl_Edit_E001)->GetSizeRect();

    ModeButtonRender(DestRect);
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::MessageProc( SI_MESSAGE* message )
{
    TreeControllerMessageProc(message);

    if(IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            OnLButtonClick(message);
        }
        break;
    case RT_MSG_LBUTTONDCLICK:
        {
            OnDoubleLButtonClick(message);
        }
        break;
    case RT_MSG_SB_LINETOP:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_Scroll_V001)
            {
                OnVScrollUp(message);
            }
        }
        break;
    case RT_MSG_SB_LINEBOTTOM:
        {
            if (GetControlPosition_byID(message->dwCtrlID) == kControl_Scroll_V001)
            {
                OnVScrollDown(message);
            }
        }
        break;
    case RT_MSG_EDIT_ENTERKEYDOWN:
        {
            OnEditEnterKeyDown(message);
        }
    default:
        {
            SolarDialog::MessageProc(message);
            VScrollDialog::MessageProc(message);
        }
        break;
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::MouseOver_MsgProc( SI_MESSAGE* message )
{

}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::OnShowWindow( BOOL is_show )
{
    // 윈도우가 감춰지거나 최소화 등등이 일어 날 때 호출 된다.
    // 초기 window 위치를 설정
    RECT rect = { 0, 0, 0, 0 };
    GetDialogWZ()->GetDlgSize(&rect);
    GetDialogWZ()->MoveWindowWZ(0.0f, 0.0f);

    // ESC 키를 누를 경우에 무슨 일을 할 것인지 결정한다.
    RegisterESCKeyClose(is_show);
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::Process( DWORD tick_count )
{

}

//-----------------------------------------------------------------------------------------//

CCtrlVScrollWZ* uiTextureViewerSearchWindow::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_Scroll_V001));
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::GetScrollArea( RECT& area )
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_List_L000)) == true)
    {
        area = GetControlWZ_byPosition(kControl_List_L000)->GetSizeRect();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004: // 닫기
    case kControl_Button_B001:
        {
            ViewerClose();
        }
        break;
    default:
        {

        }
        break;
    }

    CheckModeButton();

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::OnRButtonClick( SI_MESSAGE* message )
{
    bool result = true;

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::OnMouseOver( SI_MESSAGE* message )
{
    bool result = true;

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::OnDoubleLButtonClick( SI_MESSAGE* message )
{
    bool result = true;

    return result;
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::SetMode( BYTE mode )
{
    if (mode < kCreateMode || mode >= kModeMax)
    {
        return;
    }

    CControlWZ* control_ptr = NULL;
    control_ptr = GetControlWZ_byPosition(kControl_Button_B000);
    if (control_ptr) // 생성 버튼
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    control_ptr = GetControlWZ_byPosition(kControl_Button_B003);
    if (control_ptr) // 변경 버튼
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    control_ptr = GetControlWZ_byPosition(kControl_Text_T001);
    if (control_ptr) // 방 제목 텍스쳐
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    control_ptr = GetControlWZ_byPosition(kControl_Scroll_V001);
    if (control_ptr) // 스크롤
    {
        control_ptr->ShowWindowWZ(WZ_SHOW);
    }
    control_ptr = GetControlWZ_byPosition(kControl_Edit_E001);
    if (control_ptr) // Edit
    {
        control_ptr->ShowWindowWZ(WZ_SHOW);
    }    
    control_ptr = GetControlWZ_byPosition(kControl_Picture_P006);
    if (control_ptr) // Picture
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    control_ptr = GetControlWZ_byPosition(kControl_Picture_P008);
    if (control_ptr) // 열쇠 그림
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    control_ptr = GetControlWZ_byPosition(kControl_Edit_E002);
    if (control_ptr)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
    CCtrlStaticWZ* static_control_ptr = NULL;
    static_control_ptr = static_cast<CCtrlStaticWZ*> (GetControlWZ_byPosition(kControl_Text_T100));
    if (static_control_ptr)
    {        
        static_control_ptr->ShowWindowWZ(WZ_HIDE);
    }    
    control_ptr = GetControlWZ_byPosition(kControl_Picture_P000);
    if (control_ptr)
    {        
        control_ptr->ShowWindowWZ(WZ_HIDE);        
    }

    static_control_ptr = NULL;
    switch (mode)
    {
    case kCreateMode:
        {
            // 다이얼로그 제목
            static_control_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
            assert(static_control_ptr);
            if (static_control_ptr)
            {
                char* temp = "뷰어 탐색기";
                
                static_control_ptr->SetTextWZA(temp);
            }
        }
        break;
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::FileList( TCHAR* file_path, HTREEITEM h_parent )
{
    HANDLE hSrch;
    WIN32_FIND_DATA wfd;
    BOOL bResult = TRUE;
    TCHAR drive[_MAX_DRIVE];
    TCHAR dir[MAX_PATH];
    TCHAR newpath[MAX_PATH];

    hSrch = FindFirstFile(file_path, &wfd);
    if (hSrch == INVALID_HANDLE_VALUE)
    {
        return;
    }
    _splitpath(file_path, drive, dir, NULL, NULL);
    while (bResult)
    {
        // 디렉토리 파일일 경우
        if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (strcmp(wfd.cFileName, ".") && strcmp(wfd.cFileName, ".."))
            {
                // 디렉토리일 경우 여기서 일단 추가하고 FileList를 호출
                sprintf(newpath, "%s%s%s\\*.*", drive, dir, wfd.cFileName);
                HTREEITEM child_item = 
                    tree_controller_->AddTreeItem(wfd.cFileName, 
                    0, 
                    0, 
                    false, 
                    StrToWzID("st10"),
                    NULL, 
                    h_parent);

                st_File_Data st_data;
                st_data.title = wfd.cFileName;
                st_data.full_path = newpath;

                file_list_.push_back(st_data);

                FileList(newpath, child_item);
            }
        }
        else
        {
            // 파일일 경우
            int i = 0;
            TCHAR temp_char[256] = {0, };
            while (wfd.cFileName[i] != '.')
            {
                ++i;
            }
            ++i;
            int j = 0;
            while (wfd.cFileName[j] != NULL)
            {
                temp_char[j] = wfd.cFileName[i];
                ++i;
                ++j;
            }
            if (strcmp(temp_char, "DDS") == 0 || strcmp(temp_char, "dds") == 0)
            {
                // 여기서 파일들을 추가

                // 추가 하기 전에 Path들을 모두 붙여주어서 나중에 Texture를 로드 할 때 사용 할 수 있도록 한다.
                TCHAR full_path[MAX_PATH] = {0, };
                make_path(file_path, wfd.cFileName, full_path);

                std::string st_full_path(full_path);

                st_File_Data st_data;
                st_data.title = wfd.cFileName;
                st_data.full_path = full_path;
                
                HTREEITEM child_item = 
                    tree_controller_->AddTreeItem(wfd.cFileName, 
                    0, 
                    0, 
                    false, 
                    StrToWzID("st10"),
                    NULL, 
                    h_parent);

                // 선택되었을 때의 색을 지정 한다.
                child_item->tree_data.select_Color_ = RGBA(0, 255, 0, 255);

                file_list_.push_back(st_data);
            }
            if (strcmp(temp_char, _T("TGA")) == 0 || strcmp(temp_char, _T("tga")) == 0)
            {
                // 여기서 파일들을 추가

                // 추가 하기 전에 Path들을 모두 붙여주어서 나중에 Texture를 로드 할 때 사용 할 수 있도록 한다.
                TCHAR full_path[MAX_PATH] = {0, };
                make_path(file_path, wfd.cFileName, full_path);
                std::string st_full_path(full_path);
                st_File_Data st_data;
                st_data.title = wfd.cFileName;
                st_data.full_path = full_path;

                HTREEITEM child_item = 
                    tree_controller_->AddTreeItem(wfd.cFileName, 
                    0, 
                    0, 
                    false, 
                    StrToWzID("st10"),
                    NULL, 
                    h_parent);

                child_item->tree_data.select_Color_ = RGBA(0, 255, 0, 255);

                file_list_.push_back(st_data);
            }
            if (strcmp(temp_char, _T("JPG")) == 0 || strcmp(temp_char, _T("jpg")) == 0)
            {
                // 여기서 파일들을 추가
                // 추가 하기 전에 Path들을 모두 붙여주어서 나중에 Texture를 로드 할 때 사용 할 수 있도록 한다.
                TCHAR full_path[MAX_PATH] = {0, };
                make_path(file_path, wfd.cFileName, full_path);
                std::string st_full_path(full_path);
                st_File_Data st_data;
                st_data.title = wfd.cFileName;
                st_data.full_path = full_path;
                HTREEITEM child_item = 
                    tree_controller_->AddTreeItem(wfd.cFileName, 
                    0, 
                    0, 
                    false, 
                    StrToWzID("st10"),
                    NULL, 
                    h_parent);

                child_item->tree_data.select_Color_ = RGBA(0, 255, 0, 255);

                file_list_.push_back(st_data);
            }
            if (strcmp(temp_char, _T("BMP")) == 0 || strcmp(temp_char, _T("bmp")) == 0)
            {
                // 여기서 파일들을 추가
                // 추가 하기 전에 Path들을 모두 붙여주어서 나중에 Texture를 로드 할 때 사용 할 수 있도록 한다.
                TCHAR full_path[MAX_PATH] = {0, };
                make_path(file_path, wfd.cFileName, full_path);

                std::string st_full_path(full_path);

                st_File_Data st_data;
                st_data.title = wfd.cFileName;
                st_data.full_path = full_path;

                HTREEITEM child_item = 
                    tree_controller_->AddTreeItem(wfd.cFileName, 
                    0, 
                    0, 
                    false, 
                    StrToWzID("st10"),
                    NULL, 
                    h_parent);

                child_item->tree_data.select_Color_ = RGBA(0, 255, 0, 255);

                file_list_.push_back(st_data);
            }
        }
        bResult = FindNextFile(hSrch, &wfd);
    }
    FindClose(hSrch);
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::OnMouseWheelScroll( BYTE mouse_wheel_state )
{
    if (mouse_wheel_state == MOUSE_WHEEL_UP)
    {

    }
    else if (mouse_wheel_state == MOUSE_WHEEL_DOWN)
    {
        
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::RegisterESCKeyClose( BOOL is_value )
{
    // 창이 열릴 때에 등록 아닐 때에는 제거
    if (is_value)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_Texture_Viewer_Search_Window;
        msg.wParam = IM_TEXTUREVIEWER_MANAGER::TEXTUREVIEWER_SEARCH_WINDOW;
        msg.DoSomething = uiTextureViewerSearchWindow::ESCKeyProcess;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_Texture_Viewer_Search_Window);
    }
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::OnVScrollThumb( SI_MESSAGE* msg )
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V001);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    assert(scroll_control);
    if (scroll_control)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        current_scroll_pos = max(0, current_scroll_pos);
        current_scroll_pos = min(scroll_max, current_scroll_pos);
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(scroll_control->GetScrollPosWZ());
    }
    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::OnVScrollUp( SI_MESSAGE* message )
{
    bool result = true;
    
    if (VScrollDialog::OnVScrollThumb(message) == true)
    {
        WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V001);
        CCtrlVScrollWZ* scroll_control = 
            static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
        assert(scroll_control);
        if (scroll_control)
        {
            int scroll_min = 0;
            int scroll_max = 0;
            int current_scroll_pos = scroll_control->GetScrollPosWZ();
            scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);            
            current_scroll_pos -= scroll_tick_move_;
            current_scroll_pos = max(0, current_scroll_pos);
            scroll_control->SetScrollPosWZ(current_scroll_pos);
            GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(scroll_control->GetScrollPosWZ());
        }
    }

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::OnVScrollDown( SI_MESSAGE* message )
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V001);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    assert(scroll_control);
    if (scroll_control)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);        
        current_scroll_pos += scroll_tick_move_;
        current_scroll_pos = min(scroll_max, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
        GetListControlWZ(GetControlID_byPosition(kControl_List_L000))->SetBeginDrawIndex(scroll_control->GetScrollPosWZ());
    }

    return result;
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::ESCKeyProcess( DWORD wParam, DWORD lParam )
{
    uiTextureViewerSearchWindow* dialog = 
        GET_CAST_DIALOG(uiTextureViewerSearchWindow, IM_TEXTUREVIEWER_MANAGER::TEXTUREVIEWER_SEARCH_WINDOW);
    if (dialog == NULL)
    {
        return;
    }
    else
    {
        dialog->ShowWindow(FALSE);

        uiTextureViewerMan* manager = static_cast<uiTextureViewerMan*>(
            g_InterfaceManager.GetUserInterfaceManager(UIMAN_TEXTURE_VIEWER_MANAGER));

        if (manager)
        {
            uiTextureViewerTextureWindow* texture_viewer_texture_window = 
                static_cast<uiTextureViewerTextureWindow*>(manager->get_texture_viewer_item(UI_TEXTURE_VIEWER_TEXTURE_WINDOW));

            if (texture_viewer_texture_window)
            {
                // 텍스쳐를 뿌려주는 다이얼로그도 같이 보이지 않게 한다.
                texture_viewer_texture_window->ShowWindow(FALSE);
            }
        }
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::make_path(TCHAR* file_path, TCHAR* file_name, TCHAR* full_path)
{
    int count = 0;

    // 임시로 path 구성
    TCHAR full_path_temp[MAX_PATH] = {0, };

    // 먼저 경로 부터 넣어 준다.
    while (file_path[count] != '\0')
    {
        if(file_path[count] == '*')
            break;
        full_path_temp[count] = file_path[count];
        ++count;
    }

    // 파일 이름을 붙여 준다.
    int file_name_count = 0;
    while (file_name[file_name_count] != '\0')
    {
        full_path_temp[count] = file_name[file_name_count];

        ++count;
        ++file_name_count;
    }

    // 역슬래시를 찾아서 한번씩 더 붙여줘서 최종적으로 구성 해 준다.
    int full_path_temp_count = 0;
    int full_path_count = 0;

    while (full_path_temp[full_path_temp_count] != '\0')
    {
        // 최종 full_path 가 아닌 임시 full_path에 역슬래시가 존재 할 경우
        if(full_path_temp[full_path_temp_count] == '\\')
        {
            full_path[full_path_count] = full_path_temp[full_path_temp_count];
            ++full_path_count;
            full_path[full_path_count] = full_path_temp[full_path_temp_count];
        }
        else
        {
            full_path[full_path_count] = full_path_temp[full_path_temp_count];
        }
        ++full_path_temp_count;
        ++full_path_count;
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::Change_Texture_Window_Texture( TCHAR* full_path )
{
    uiTextureViewerMan* texture_viewer_man = static_cast<uiTextureViewerMan*>(GetMan());
    assert(texture_viewer_man);

    if (texture_viewer_man)
    {
        uiTextureViewerTextureWindow* texture_window_ptr = 
            static_cast<uiTextureViewerTextureWindow*>(texture_viewer_man->get_texture_viewer_item(UI_TEXTURE_VIEWER_TEXTURE_WINDOW));

        texture_window_ptr->change_texture(full_path);
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::SearchFileList( TCHAR* file_name )
{
    if (tree_controller_)
    {
        tree_controller_->DeleteAllItem();
    }

    edit_list_.clear();

    std::list<st_File_Data>::iterator iter;

    int strLength = strlen(file_name);

    if(strLength <= 0)
        return;

    for (iter = file_list_.begin();
        iter != file_list_.end();
        ++iter)
    {
        TCHAR* item_name = const_cast<char*>((*iter).title.c_str());

        if (strstr(item_name, file_name) != NULL)
        {
            HTREEITEM child_item = 
                tree_controller_->AddTreeItem(item_name, 
                0, 
                0, 
                false, 
                StrToWzID("st10"),
                NULL, 
                NULL);

            child_item->tree_data.select_Color_ = RGBA(0, 255, 0, 255);

            edit_list_.push_back((*iter));
        }
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::StartTree( void )
{
    if (tree_controller_)
    {
        // TreeController에 파일 리스트를 읽어와 적용
        FileList(_T("Data\\*.*"), NULL);
    }   

    int max_line_count = list_control_->GetListRowCnt();
    int draw_line_count = list_control_->GetDrawRowCount();
    int scroll_max_num = max(0, max_line_count - draw_line_count);
    VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);

    Is_start = true;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::get_Is_start( void ) const
{
    return Is_start;
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::TreeControllerMessageProc( SI_MESSAGE* message )
{
    // TreeController의 MessageProc
    if (tree_controller_)
    {
        tree_controller_->MessageProc(message);

        // Tree_Controller에서 선택 된 아이템이 있다면
        HTREEITEM sel_item = tree_controller_->GetSelItem();

        if (sel_item && sel_item->is_leaf() &&
            sel_item_ != sel_item)
        {
            //TCHAR full_path[MAX_PATH] = {0, };

            bool result = false;

            std::list<st_File_Data>::iterator iter;

            if (Is_edit_mode_)
            {
                // 에디트 모드일 때
                for (iter = edit_list_.begin();
                    iter != edit_list_.end();
                    ++iter)
                {
                    if (strcmp(sel_item->tree_data.tree_item_str_, (*iter).title.c_str()) == 0)
                    {
                        result = true;

                        break;
                    }
                }
            }
            else
            {
                for (iter = file_list_.begin();
                    iter != file_list_.end();
                    ++iter)
                {
                    if(strcmp(sel_item->tree_data.tree_item_str_, (*iter).title.c_str()) == 0)
                    {
                        result = true;

                        break;
                    }
                }
            }

            // Texture_Viewer_Texture_Window에 텍스처를 바꿔 준다.
            if (result == true)
            {
                result = false;
                int string_length = strlen((*iter).full_path.c_str());
                for (int i = 0; i < string_length; ++i)
                {
                    if ((*iter).full_path.c_str()[i] == '.')
                    {
                        result = true;
                    }
                }
                if (result == true)
                {
                    Change_Texture_Window_Texture(const_cast<TCHAR*>((*iter).full_path.c_str()));
                }
            }
            sel_item_ = sel_item;
        }
    }
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerSearchWindow::OnEditEnterKeyDown( SI_MESSAGE* message )
{
    bool result = true;

    TCHAR find_message[MAX_VENDOR_SEARCH_LENGTH + 1];
    memset(find_message , 0 , sizeof(find_message) );

    CCtrlDxEditBox* str_edit = 
        static_cast<CCtrlDxEditBox*>(GetControlWZ(GetControlID_byPosition(kControl_Edit_E001)));
    assert(str_edit);

    if (str_edit)
    {
        str_edit->GetTextWZ(find_message, MAX_VENDOR_SEARCH_LENGTH);

        if(strlen(find_message) <= 0)
        {
            result = false;
            return result;
        }

        SearchFileList(find_message);
        int max_line_count = list_control_->GetListRowCnt();
        int draw_line_count = list_control_->GetDrawRowCount();
        int scroll_max_num = max(0, max_line_count - draw_line_count);
        VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);
        Is_edit_mode_ = true;

        str_edit->SetTextWZA("");
    }

    return result;
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::ViewerClose()
{
    ShowInterface(FALSE);

    uiTextureViewerMan* manager = static_cast<uiTextureViewerMan*>(GetMan());
    assert(manager);

    uiTextureViewerTextureWindow* ui_ptr = 
        static_cast<uiTextureViewerTextureWindow*>(manager->get_texture_viewer_item(UI_TEXTURE_VIEWER_TEXTURE_WINDOW));
    assert(ui_ptr);

    if (ui_ptr)
    {
        ui_ptr->ShowInterface(FALSE);
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::CheckModeButton()
{
    POINT mouse_pt = Mouse::Instance()->GetMousePoint();

    if (PtInRect(&edit_mode_texture_rect_, mouse_pt))
    {
        if(Is_edit_mode_)
        {
            Is_edit_mode_ = !Is_edit_mode_;

            if(tree_controller_)
            {
                tree_controller_->DeleteAllItem();

                file_list_.clear();

                // TreeController에 파일 리스트를 읽어와 적용
                FileList(_T("Data\\*.*"), NULL);

                int max_line_count = list_control_->GetListRowCnt();
                int draw_line_count = list_control_->GetDrawRowCount();
                int scroll_max_num = max(0, max_line_count - draw_line_count);
                VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);
            }
        }
    }
    if (PtInRect(&grid_mode_texture_rect_, mouse_pt))
    {        
        
        Is_grid_mode_ = !Is_grid_mode_;        
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerSearchWindow::ModeButtonRender( const RECT& DestRect )
{
    St_TextureContainer* tex_info = NULL;
    tex_info = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(ui_edit_mode_texture_);
    RECT text_rect;

    // 모드 버튼 출력
    if (ui_edit_mode_texture_ != INVALID_HANDLE_VALUE
        && tex_info != NULL)
    {
        g_pSunRenderer->RenderTexture(ui_edit_mode_texture_,
            (float)DestRect.left - 80,
            (float)DestRect.top - 5,
            (float)82 - 6,
            (float)78 - 48,
            (float)6,
            (float)48,
            (float)82,
            (float)78,
            NULL);

        //int off_set = 10;
        //RECT text_rect = {DestRect.left - 90, DestRect.top - 5, DestRect.left + 82 - 6 - 90, 
        //    DestRect.top - 5 + 78 - 48 };
        text_rect.left = DestRect.left - 90;
        text_rect.top = DestRect.top - 5;
        text_rect.right = DestRect.left + 82 - 6 - 90;
        text_rect.bottom = DestRect.top - 5 + 78 - 48;
        edit_mode_texture_rect_ = text_rect; // 나중에 충돌체크를 위해 rect를 저장
        DWORD font_ID = StrToWzID("st11");
        g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);

        TCHAR mode_text[256] = {0, };

        // 현재 모드의 따라 반대의 모드 텍스트를 지정
        if (Is_edit_mode_)
        {
            sprintf(mode_text, "%s", "TreeMode");
        }
        else
        {
            sprintf(mode_text, "%s", "EditMode");
        }

        g_pSunRenderer->x_pManagerTextOut->DrawText(mode_text, &text_rect, WzColor_RGBA(255, 246, 104, 255), WzColor_RGBA(0, 0, 0, 0), TP_HCENTER | TP_VCENTER);

        g_pSunRenderer->x_pManagerTextOut->EndUseFont();
    }

    // 모드 버튼 출력    
    if (ui_grid_mode_texture_ != INVALID_HANDLE_VALUE
        && tex_info != NULL)
    {
        g_pSunRenderer->RenderTexture(ui_grid_mode_texture_,
            (float)DestRect.left - 80,
            (float)DestRect.top + (78 - 48),
            (float)82 - 6,
            (float)75 - 48,
            (float)6,
            (float)48,
            (float)82,
            (float)75,
            NULL);

        //int off_set = 10;
        RECT grid_rect = { DestRect.left - 90, DestRect.top + (75 - 48),
        DestRect.left - 90 + 82,
        DestRect.top + (75 - 48) + (75 - 48) };
        grid_mode_texture_rect_ = grid_rect; // 나중에 충돌체크를 위해 rect를 저장
        DWORD font_ID = StrToWzID("st11");
        g_pSunRenderer->x_pManagerTextOut->StartToUseFont(font_ID);

        TCHAR mode_text[256] = {0, };

        // 현재 모드의 따라 반대의 모드 텍스트를 지정
        if (Is_grid_mode_)
        {
            sprintf(mode_text, "%s", "HIDE GRID");
        }
        else
        {
            sprintf(mode_text, "%s", "VIEW GRID");
        }

        g_pSunRenderer->x_pManagerTextOut->DrawText(mode_text, &grid_mode_texture_rect_, WzColor_RGBA(255, 246, 104, 255), WzColor_RGBA(0, 0, 0, 0), TP_HCENTER | TP_VCENTER);

        g_pSunRenderer->x_pManagerTextOut->EndUseFont();
    }
}

bool uiTextureViewerSearchWindow::get_Is_grid_mode( void ) const
{
    return Is_grid_mode_;
}

void uiTextureViewerSearchWindow::set_scroll_tick_move_value( const int& scroll_tick_move )
{
    scroll_tick_move_ = scroll_tick_move;
}
//-----------------------------------------------------------------------------------------//

#endif
#endif