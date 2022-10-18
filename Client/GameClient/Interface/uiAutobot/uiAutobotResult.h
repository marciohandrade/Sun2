#ifndef _uiAutobotResult_h_
#define _uiAutobotResult_h_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifdef _YMS_AUTOBOT_GAME

#include "uiBase.h"

//------------------------------------------------------------------------------ 
class uiAutobotResult : public uiBase
{
    friend class uiAutobotMan;
    enum UIControlPos
    {
        kControl_Button_B001 = 0,
        kControl_Button_B002,
        kControl_Text_T001,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_S003,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiAutobotResult(InterfaceManager* ui_man);
    virtual ~uiAutobotResult(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release(void);
    virtual void OnShowWindow(BOOL val);
    virtual void NetworkProc(MSG_BASE* msgbase);
    virtual void MessageProc(SI_MESSAGE* message);

    void SetResult(LONGLONG money, LONGLONG exp);

private:
    bool OnLButtonClick(SI_MESSAGE* message);

};

#endif //_YMS_AUTOBOT_GAME

#endif