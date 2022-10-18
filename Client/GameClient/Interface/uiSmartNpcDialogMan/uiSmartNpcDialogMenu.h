#pragma once

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM

#include "uiSmartNpcDialogMan/uiSmartNpcDialog.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"

class uiSmartNpcDialogMenu : public uiSmartNpcDialog
{
public:
    enum UIControlPos
    {
        kControl_Button_B999,
        kControl_Text_T999,

        kControl_List_L000,
        kControl_VScroll_V001,

        
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Button_B003,
        kControl_Button_B004,
        kControl_Button_B005,
        kControl_Button_B006,

        kControl_Size,
    };



    uiSmartNpcDialogMenu(InterfaceManager* ui_manager = &g_InterfaceManager);
    ~uiSmartNpcDialogMenu();

    VOID Init(CDrawBase* pDrawBase);
    BOOL CreateControls(BYTE sub_type);
    void SetControlText(BYTE contorl_index, const TCHAR *text_massage);
    void SetControlEventID(BYTE contorl_index, DWORD event_id, DWORD step);

public:
    void SetButtonCount(BYTE count) { button_count_ = count; }
protected:
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();
    virtual void OnShowWindow( BOOL val );
    virtual void MessageProc(SI_MESSAGE* message);
    bool OnLButtonClick(SI_MESSAGE* message);

    void SetTextMessage(const TCHAR *text_massage);

private:

    void UpdateMouseWheel();

    CDrawBase *	draw_;
    FTextRender ftext_render_;

    CCtrlVScrollWZ* scroll_control_;
    CCtrlListWZ* list_control_;

    int button_count_; // ¹öÆ° ¼ö
    EVENT_IDSTEP button_event_id[uiSmartNpcDialogMan::MaxMenuButton];

};

#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
