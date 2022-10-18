#pragma once
#include "uiBase.h"


class uiGuideSystemMan;

class uiGuideTutorialOpenDialog : public uiBase
{
public:
    enum UIControlPos
    {
        kControl_Button_B001 = 0,
        kControl_Picture_P000,
        kControl_Size,
    };
    
    uiGuideTutorialOpenDialog(InterfaceManager* ui_manager);
    virtual ~uiGuideTutorialOpenDialog(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL val);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RegisterControl();

    uiGuideSystemMan* ui_guide_system_manager_;

};
