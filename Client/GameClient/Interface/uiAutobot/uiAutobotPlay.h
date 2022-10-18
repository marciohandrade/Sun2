#ifndef _uiAutobotPlay_h_
#define _uiAutobotPlay_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifdef _YMS_AUTOBOT_GAME

#include "uiBase.h"

//------------------------------------------------------------------------------ 
class uiAutobotPlay : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B999 = 0,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiAutobotPlay(InterfaceManager* ui_man);
    virtual ~uiAutobotPlay(void);

    virtual void    Init(CDrawBase* drawbase);
    virtual void    Release(void);
    virtual void    OnShowWindow(BOOL val);
    virtual void    Process(DWORD tick);
    virtual void    OnRenderSolarDialog();
    virtual void    NetworkProc(MSG_BASE* msgbase);
    virtual void    MessageProc(SI_MESSAGE* message);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
};

#endif _YMS_AUTOBOT_GAME

#endif