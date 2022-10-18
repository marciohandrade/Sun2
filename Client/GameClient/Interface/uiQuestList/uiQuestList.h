#pragma once
#ifdef _NA_004644_20110506_QUEST_NAVIGATION

#include "uiQuestMan/uiQuestMan.h"

class uiQuestList : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B000 = 0,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B006,
        kControl_List_L000,
        kControl_List_L002,
        kControl_List_L001,
        kControl_List_L003,
        kControl_CheckButton_C000,
        kControl_CheckButton_C001,
        kControl_CheckButton_C002,
        kControl_CheckButton_C003,
        kControl_Size,
    };

    enum
    {
        kQuest_List_Title = 0,
        kQuest_List_Progress,
        kQuest_List_Max,
    };

    enum
    {
        kList_Text = 0,
        kList_Background,
        kList_State,
        kList_State_Background,
        kList_Max,
    };

    enum
    {
        kRed = 0,
        kGreen,
        kBlue,
        kPurple,
        kButton_Max,
    };
public:
    uiQuestList(InterfaceManager* ui_manager);
    virtual ~uiQuestList(void);
    void SetQuestList();

private:
    virtual VOID Init(CDrawBase * pDrawBase);
    virtual void OnShowWindow(BOOL show_value);
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    virtual void MessageProc(SI_MESSAGE* message);
    void ScrollUp();
    void ScrollDown();

    CCtrlListWZ* list_controls_[kList_Max];
    CCtrlButtonWZ* buttons_[kButton_Max];
    CCtrlButtonCheckWZ* checkbuttons_[kButton_Max];
    RECT buttons_rect[kButton_Max];
    RECT checkbuttons_rect[kButton_Max];
    QCODE quest_codes_[kButton_Max];
    int buttons_index[kButton_Max];

    int list_position_;
    int last_list_position_;
};
#endif //_NA_004644_20110506_QUEST_NAVIGATION