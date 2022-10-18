#pragma once
#include "uiBase.h"
#include "FTextRender.h"

class uiCreateClassInfo : public uiBase
{
    enum UIControlPos
    {
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Picture_P003,
        kControl_VScroll_V000,
        kControl_Static_S000,
        kControl_Size,
    };
public:
    uiCreateClassInfo(InterfaceManager* ui_manager);
    virtual ~uiCreateClassInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * message);
    void AddClassInfo(BYTE class_type, DWORD class_description);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

    void OnThumbScroll();
    void OnUpScrollButton();
    void OnDownScrollButton();

    void AnimateOpen();
    void AnimateClose();

private:
    void RegisterControl();

private:
    FTextRender class_info_;
    CCtrlPictureWZ* background_;
    CCtrlPictureWZ* image_left_;
    CCtrlPictureWZ* image_right_;
    CCtrlStaticWZ* text_area_;
    FRECT basic_position_;
    float center_position_;
    bool is_animate_;
    bool is_open_ani_;
    DWORD start_tick_;
    DWORD end_tick_;
    DWORD base_width_;
};

