#pragma once

#include "uiBase.h"
#include "uiConvenienceStore_Dialog.h"
#include "TabCheckButtonControl.h"
#include "VScrollDialog.h"

class uiConvenienceStoreMan;

//------------------------------------------------------------------------------ 
class uiConvenienceStore_FriendList : public uiConvenienceStore_Dialog, public VScrollDialog
{
public:
    enum UIControlPos
    {
        kControl_CheckBT_C001 = 0,
        kControl_CheckBT_CT01,
        kControl_Button_B000,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_List_L000,
        kControl_VScroll_V000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Picture_P003,
        kControl_Picture_P004,
        kControl_Picture_P000,
        kControl_Picture_P005,
        kControl_Size,
    };
    enum TabMenuType
    {
        kTabMenu_Friend = 0,
        kTabMenu_Guild,
        kTabMenu_Max,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiConvenienceStore_FriendList(InterfaceManager* ui_man);
    virtual ~uiConvenienceStore_FriendList(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void OnShowWindow(BOOL val);
    virtual void Process(DWORD tick){}
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog(){}
    virtual void MessageProc(SI_MESSAGE* message);

    void RefreshFriendList();

    TabCheckButtonControl& menu_tab() { return menu_tab_; }

protected:
    virtual CCtrlVScrollWZ* vscroll_control();
    virtual bool GetScrollArea(RECT& area);
    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLDButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    void AddFriendList(CCtrlListWZ* list_control, const TCHAR* char_name, LEVELTYPE level, eCHAR_TYPE char_type);

    TabCheckButtonControl menu_tab_;
};

