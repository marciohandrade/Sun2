#pragma once

#include "uiBase.h"
#ifdef _YMS_TREECONTROL
#include "./TreeMenuGroup.h"
#endif//_YMS_TREECONTROL

#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"


class uiGuideSystemMan;
struct TotalGuide;
struct TotalGuidePage;

class uiGuideInfomationlDialog : public uiBase
{
public:
    enum UIControlPos
    {
        kControl_Button_BT01 = 0,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Scroll_VS00,
        kControl_List_L000,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Scroll_V000,
        kControl_Size,
    };
    
    uiGuideInfomationlDialog(InterfaceManager* ui_manager);
    virtual ~uiGuideInfomationlDialog(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL val);

#ifdef _YMS_TREECONTROL
    TreeMenuGroup& tree_menu() { return tree_menu_; }
#endif//_YMS_TREECONTROL

    void SelectList(TotalGuideNotice::Index code);
    void AllTreeHide();
    void ResetData();

    bool SaveData(TotalGuideNotice::Index  category_code, TotalGuideNotice::Index code, bool is_show);
    void SetData(TotalGuideNotice::Index code, BYTE page);
    bool AddData(TotalGuideNotice::Index code);
    void DeleteData(TotalGuideNotice::Index  category_code, TotalGuideNotice::Index code);
        
    bool IsHaveNoticeGuide();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RegisterControl();
    HANDLE LoadTextureImage(DWORD code);
    void UpdateImageRect();
    void ResetUI();
    void InitFromLevel();
    void InitFromData();
    void OnMouseWheelScroll(BYTE mouse_wheel_state);
    bool OnUpScroll(SI_MESSAGE* message);
    bool OnDownScroll(SI_MESSAGE* message);
    bool OnThumbScroll(SI_MESSAGE* message);

    static void uiGuideInfomationlDialog::Callback_InfoTreeRender(CCtrlListWZ* list_control, 
        CCtrlListWZ::CListSellData* sell_data, 
        int sell_index, 
        RECT& render_area);

    uiGuideSystemMan* ui_guide_system_manager_;
    TotalGuide* guide_page_;
    BYTE current_page_;
    BYTE last_page_;
    const TotalGuidePage* page_infomation_;

    int image_width_;
    int image_height_;
    RECT image_rect_;
    RECT text_rect_;
    RECT view_rect_;

    float line_height_;
    int current_line_;
    FTextParser ftext_parser_;
    FTextSentence ftext_sentence_;
    FTextParam ftext_param_;
    FTextRender ftext_render_;
#ifdef _YMS_TREECONTROL
    TreeMenuGroup tree_menu_;
#endif//_YMS_TREECONTROL
    HANDLE image_texture_;
    BYTE list_notice_count_[TotalGuideNotice::kMaxSize];
};
