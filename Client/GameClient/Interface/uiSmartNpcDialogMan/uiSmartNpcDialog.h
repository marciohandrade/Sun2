#pragma once

#ifdef _NA_000000_20140120_SMART_NPC_SYSTEM
#include "uiSmartNpcDialogMan/uiSmartNpcDialogMan.h"
#include "uiBase.h"

class uiSmartNpcDialog : public uiBase
{
public:
    struct EVENT_IDSTEP
    {
        DWORD event_id;
        DWORD step;
    };

    uiSmartNpcDialog(InterfaceManager* ui_manager = &g_InterfaceManager);
    virtual inline ~uiSmartNpcDialog() {}

    virtual void SetControlText(BYTE contorl_index, const TCHAR *text_massage) = 0;
    virtual void SetControlEventID(BYTE contorl_index, DWORD event_id, DWORD step) = 0;


    // 필요한가?
    inline int GetID() { return id_; }
    inline void SetID(int id) { id_ = id; }
    inline BYTE GetSubType() { return dialog_sub_type_; }
    inline void SetSubType(BYTE sub_type) { dialog_sub_type_ = sub_type; }
    //
protected:
    virtual BOOL CreateControls(BYTE sub_type) = 0;

    CCtrlStaticWZ* CreateContorlStatic(DWORD dialog_key, DWORD control_id);
    CCtrlButtonWZ* CreateContorlButton(DWORD dialog_key, DWORD control_id);
    CCtrlButtonWZ* CreateContorlCloseButton(DWORD dialog_key, DWORD control_id);
    CCtrlListWZ* CreateContorlList(DWORD dialog_key, DWORD control_id);
    CCtrlVScrollWZ* CreateContorlVScroll(DWORD dialog_key, DWORD control_id);
    CCtrlPictureWZ* CreateContorlPicture(DWORD dialog_key, DWORD control_id);
    CCtrlMultilineEditWZ* CreateContorlEdit(DWORD dialog_key, DWORD control_id);
    
    BYTE dialog_type_;
    BYTE dialog_sub_type_;
    int id_;
};

#endif //_NA_000000_20140120_SMART_NPC_SYSTEM