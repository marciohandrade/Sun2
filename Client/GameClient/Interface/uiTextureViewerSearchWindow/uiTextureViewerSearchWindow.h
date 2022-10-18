#pragma once

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER

#include "uiBase.h"
#include "DropdownControl.h"
#include "VScrollDialog.h"
#include "TreeController.h"

class uiTextureViewerMan;
class uiTextureViewerSearchWindow : public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0,
        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Text_T100,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_T001,
        kControl_Edit_E001,
        kControl_Edit_E002,
        kControl_List_L000,
        kControl_Picture_P003,
        kControl_Picture_P002,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P004,
        kControl_Picture_P005,
        kControl_Picture_P007,
        kControl_Picture_P006,
        kControl_Picture_P008,
        kControl_Scroll_V001,
        kControl_Picture_L000,
        kControl_Size,        
    };

public:
    enum UIMode
    {
        kCreateMode = 0,
        kModeMax,
    };

    struct st_File_Data
    {
        std::string title;
        std::string full_path;
    };

public:
    uiTextureViewerSearchWindow(InterfaceManager* ui_manager);
    virtual ~uiTextureViewerSearchWindow(void);

protected:
    virtual CCtrlVScrollWZ* vscroll_control();// 추상 메소드
    virtual bool GetScrollArea(RECT& area);   // 추상 메소드
public:
	virtual void Init(CDrawBase* draw_base);
	virtual void Release();
	virtual void OnUpdateSolarDialog();
	virtual void OnRenderSolarDialog();
	virtual void MessageProc(SI_MESSAGE* message);
	virtual void MouseOver_MsgProc(SI_MESSAGE* message);
	virtual void OnShowWindow(BOOL is_show);
	virtual void Process(DWORD tick_count);
private:
    void ModeButtonRender(const RECT& DestRect);
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    bool OnDoubleLButtonClick(SI_MESSAGE* message);
    void OnMouseWheelScroll(BYTE mouse_wheel_state);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
    bool OnVScrollThumb(SI_MESSAGE* msg);
    bool OnEditEnterKeyDown(SI_MESSAGE* message);
private:
    void RegisterESCKeyClose(BOOL is_value);
    static void ESCKeyProcess(DWORD wParam, DWORD lParam);
    void SetMode(BYTE mode);// 여기서 스트링 지정
    void TreeControllerMessageProc(SI_MESSAGE* message);
    void ViewerClose();
    void CheckModeButton();
private: // 파일 리스트에 관한 함수
    void FileList(char* file_path, HTREEITEM h_parent);
    void make_path(/*경로*/TCHAR* file_path, /*파일 이름*/TCHAR* file_name, /*출력*/TCHAR* full_path);
    void Change_Texture_Window_Texture(TCHAR* full_path);
    void SearchFileList(TCHAR* file_name);
private:
    void set_scroll_tick_move_value(const int& scroll_tick_move);
protected:
    void RegisterControl();
public:
    void StartTree(void); // 처음 //뷰어 를 입력 했을 때 Tree를 구성해 주기 위한 함수
    bool get_Is_start(void) const;
    bool get_Is_grid_mode(void) const;
private:
    std::vector<std::string> file_list_vector_;
    CCtrlListWZ* list_control_;
    CCtrlVScrollWZ* vscroll_control_;
    TreeController* tree_controller_;
    HTREEITEM       sel_item_; // 선택 된 값을 저장 해 놓고 비교해 가며 쓴다.
    uiTextureViewerMan* ui_texture_viewer_manager_;
    std::list<st_File_Data> file_list_;
    std::list<st_File_Data> edit_list_;
    bool Is_edit_mode_;
    HANDLE ui_edit_mode_texture_;
    RECT   edit_mode_texture_rect_;
    bool   Is_start;
    HANDLE ui_grid_mode_texture_;
    RECT   grid_mode_texture_rect_;
    bool   Is_grid_mode_;
    int    scroll_tick_move_;
};

#endif
#endif