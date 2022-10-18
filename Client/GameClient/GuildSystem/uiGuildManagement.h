#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "VScrollDialog.h"

class MultiVScrollDialog;
class uiGuildManagement : public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,
        kControl_CheckBT_C001,
        kControl_CheckBT_C003,
        kControl_CheckBT_C004,
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C005,
#endif //_NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C002,

        kControl_Button_B005,       // ������ ����
        kControl_Button_B001,       //
        kControl_Button_B002,
        kControl_List_L001,         // ������Ʈ
        kControl_List_L000,         // ���븮��Ʈ
        kControl_List_L002,         // �����⸮��Ʈ
        kControl_VScroll_V001,      // ���͸���Ʈ ��ũ��
        kControl_VScroll_V003,      // ���븮��Ʈ ��ũ��
        kControl_VScroll_V002,      // �����⸮��Ʈ ��ũ��

        kControl_TextImage_I000, // ���ͱ���
        kControl_TextImage_I001, // �������
        kControl_TextImage_I003, // ���������

        kControl_TextImage_I002, // ����������
        kControl_TextImage_I005, // ����Ƚ��
        kControl_TextImage_I004, // ���Ƚ��
        kControl_TextImage_I006, // ���Ƚ��

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildManagement(InterfaceManager* ui_manager);
    virtual ~uiGuildManagement();

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
    MultiVScrollDialog* enemy_vscroll(){ return enemy_vscroll_; }
    MultiVScrollDialog* wait_vscroll(){ return wait_vscroll_; }
    void set_enemy_vscroll(MultiVScrollDialog* vscoll_control){ enemy_vscroll_ = vscoll_control; }
    void set_wait_vscroll(MultiVScrollDialog* vscoll_control){ wait_vscroll_ = vscoll_control; }
private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);

    void OnClickGuildRelationButton();
    void OnClickDominationManagementButton();
    void UpdateDominationInfo();
    void UpdateGuildRelationList();

    void RefreshTab();

    MultiVScrollDialog* enemy_vscroll_;
    MultiVScrollDialog* wait_vscroll_;
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
