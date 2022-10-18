#pragma once
#include "uiBase.h"

#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"

class uiGuideSystemMan;
struct TotalGuide;
struct TotalGuidePage;

class uiGuideTutorialDialog : public uiBase
{
public:
    enum UIControlPos
    {
        kControl_Button_B000 = 0,
        kControl_Button_BT01,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Scroll_VS00,
        kControl_Size,
    };
    
    uiGuideTutorialDialog(InterfaceManager* ui_manager);
    virtual ~uiGuideTutorialDialog(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL val);
    void SetData(TotalGuideNotice::Index code, BYTE page);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RegisterControl();
    HANDLE LoadTextureImage(DWORD code);
    void UpdateImageRect();
    void SetPageButton(BYTE page);
    void OnMouseWheelScroll(BYTE mouse_wheel_state);
    bool OnUpScroll(SI_MESSAGE* message);
    bool OnDownScroll(SI_MESSAGE* message);
    bool OnThumbScroll(SI_MESSAGE* message);

    //void ControlSize(RECT control_rect);
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

    bool is_mini_;
    float line_height_;
    //int max_line_;
    int current_line_;
    FTextParser ftext_parser_;
    FTextSentence ftext_sentence_;
    FTextParam ftext_param_;
    FTextRender ftext_render_;

    HANDLE image_texture_;
};
