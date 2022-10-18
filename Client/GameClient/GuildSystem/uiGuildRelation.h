#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "VScrollDialog.h"
#include "GuildSystem/uiGuildRelationAsk.h"

class uiGuildRelation : public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        kControl_CheckBT_C001 = 0,  // 동맹 탭
        kControl_CheckBT_C002,      // 적대 탭
        kControl_CheckBT_C004,      // 적대대기 탭

        kControl_Button_B999,
        kControl_Button_B998,
        kControl_Button_B000,

        kControl_Button_B005,       // 관계 신청 버튼
        kControl_Button_B001,       // 관계 해제 버튼

        kControl_Text_T999,
        kControl_Text_S000,
        kControl_List_L000,
        kControl_VScroll_VS02,
        kControl_Size,
    };

    enum
    {
        kAllienceList = 0,
        kHostilityList,
        kWaitHostilityList,

        kMaxList,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildRelation(InterfaceManager* ui_manager);
    virtual ~uiGuildRelation();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void NetworkProc(MSG_BASE* recv_packet);

    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
protected:
    CCtrlVScrollWZ* vscroll_control();
    bool GetScrollArea(RECT& area);
    bool OnVScrollThumb(SI_MESSAGE* msg);
    bool OnVScrollUp(SI_MESSAGE* msg);
    bool OnVScrollDown(SI_MESSAGE* msg);
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    void OnClickRelationButton();
    void OnClickDeleteButton();

    void ShowRelationAsk(uiGuildRelationAsk::GuildAskType ask_type, TCHAR* guild_name);
    void SetCurrentListIndex(DWORD list_index);
    void UpdateGuildRelationList();
    void UpdateRelationButtonState();
    void UpdateWheelScroll();
    void RefreshTab();

    GUILD_RELATION* GetSelectedGuildRelation();

    DWORD current_list_index() const { return current_list_index_; }
    void set_current_list_index(DWORD value) { current_list_index_ = value; }

private:
    DWORD current_list_index_;
    
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
