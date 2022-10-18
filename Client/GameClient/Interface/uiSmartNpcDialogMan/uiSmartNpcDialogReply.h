#pragma once

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM

#include "uiSmartNpcDialogMan/uiSmartNpcDialog.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextRender.h"
#include <SCItemSlotContainer.h>
#include <SCItemSlot.h>

class uiSmartNpcDialogReply : public uiSmartNpcDialog, public SCItemSlotContainer
{
public:
    uiSmartNpcDialogReply(InterfaceManager* ui_manager = &g_InterfaceManager);
    ~uiSmartNpcDialogReply();

    enum UIControlPos
    {
        kControl_Button_SLT0,  // 슬롯 (인덱스 0부터)
        kControl_Button_SLT1,
        kControl_Button_SLT2,
        kControl_Button_SLT3,
        kControl_Button_SLT4,

        kControl_Button_B999,  // x버튼
        kControl_Text_T999,
        kControl_List_L000,
        kControl_Button_B001,   // 예
        kControl_Button_B002,   // 아니오
        kControl_Button_B003,   // 취소
        kControl_Edit_E001,     // 에디트

        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Picture_P003,
        kControl_Picture_P004,
        kControl_Picture_P005,

        kControl_Picture_P006,  // 슬롯 배경
        kControl_Picture_P007,
        kControl_Picture_P008,
        kControl_Picture_P009,
        kControl_Picture_P010,

        kControl_Picture_P011,

        kControl_Size,
    };
    enum 
    {
        Button_Return_Ok = 1,
        Button_Return_No = 2,
        Button_Return_Cancle = 3
    };
    enum
    {
        kSlotTotalCount = kControl_Button_SLT4 - kControl_Button_SLT0 + 1,
    };

    VOID Init(CDrawBase* pDrawBase);
    virtual void Release();
    BOOL CreateControls(BYTE sub_type);
    void SetControlText(BYTE contorl_index, const TCHAR *text_massage);
    void SetControlEventID(BYTE contorl_index, DWORD event_id, DWORD step);
    
    void SetSlotItem(BYTE slot_index, DWORD item_code, DWORD item_number);
    void DeleteAllSlot();
    SCSlotContainer* GetSlotContainer();
    ItemUnitRenderer* GetItemUnitRender() const;
    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, SCSlot& IN inputed_slot);
    BOOL DeleteSlot(POSTYPE at_pos, SCSlot* slot_out);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    void RenderTooltip();

    void SetTextMessage(const TCHAR *text_massage);

    CDrawBase *	draw_;
    FTextRender ftext_render_;

    CCtrlListWZ* list_control_;

    EVENT_IDSTEP button_event_id[uiSmartNpcDialogMan::MaxReplyButton];

private:
    RC::eSLOT_INSERT_RESULT InsertSlot(POSTYPE at_pos, DWORD type_code, DWORD type_index);
    void ReFreshRenderSlot();

    ItemUnitRenderer* item_unit_renderer_;
};

#endif //_NA_000000_20140120_SMART_NPC_SYSTEM
