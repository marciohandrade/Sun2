#pragma once

#include "uiBase.h"

class uiMenuMan;
//------------------------------------------------------------------------------ 
class uiTutorialSkip: public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B999 = 0,
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

    static void Callback_TutorialSkip(bool is_yes);

public:
    uiTutorialSkip (InterfaceManager* ui_man);
    virtual ~uiTutorialSkip (void);

    virtual uiMenuMan* GetManager();
    virtual void MessageProc(SI_MESSAGE* message);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
};
