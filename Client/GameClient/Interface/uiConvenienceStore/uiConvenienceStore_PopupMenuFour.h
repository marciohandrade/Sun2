#pragma once

#include "uiBase.h"
#include "uiConvenienceStore_PopupMenu.h"

class uiConvenienceStoreMan;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_PopupMenuFour : public uiConvenienceStore_PopupMenu
{
    enum UIControlPos
    {
        kControl_Button_B001 = 0,
        kControl_Button_B000,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Size,
    };


public:
    static const DWORD kDialog_ID;

public:
    uiConvenienceStore_PopupMenuFour(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_PopupMenuFour(void);

    virtual void    Init(CDrawBase* drawbase);
    virtual void    Release(void);
    virtual void    OnShowWindow(BOOL val);
    virtual void    OnUpdateSolarDialog();
    virtual void    Process(DWORD tick);
    virtual void    NetworkProc(MSG_BASE* msgbase);
    virtual void    MessageProc(SI_MESSAGE* message);

    virtual void SetMenu(int button_index, DWORD string_code, bool is_enable, uiConvenienceStore_Dialog::PopupCommandType command_type, DWORD user_data); 

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
};
