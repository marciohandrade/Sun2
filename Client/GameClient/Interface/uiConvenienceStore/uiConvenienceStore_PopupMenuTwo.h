#pragma once

#include "uiBase.h"
#include "uiConvenienceStore_PopupMenu.h"

class uiConvenienceStoreMan;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_PopupMenuTwo : public uiConvenienceStore_PopupMenu
{
    enum UIControlPos
    {
        kControl_Button_B001 = 0,
        kControl_Button_B000,
        kControl_Button_B002,
        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiConvenienceStore_PopupMenuTwo(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_PopupMenuTwo(void);

    virtual void    Init(CDrawBase* drawbase);
    virtual void    Release(void);
    virtual void    OnShowWindow(BOOL val);
    virtual void    OnUpdateSolarDialog();
    virtual void    Process(DWORD tick);
    virtual void    NetworkProc(MSG_BASE* msgbase);
    virtual void    MessageProc(SI_MESSAGE* message);

    //! 팝업 메뉴   2개일때 index : 0, 1, 사용
    //!            1      index : 2; 사용
    virtual void SetMenu(int button_index, DWORD string_code, bool is_enable, uiConvenienceStore_Dialog::PopupCommandType command_type, DWORD user_data); 

    virtual void Reset();


private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
};
