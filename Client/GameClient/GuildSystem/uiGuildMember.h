#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildMember : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,  // ���Ұ���
        kControl_CheckBT_C001,      // ����������
        kControl_CheckBT_C003,      // ���ü���
        kControl_CheckBT_C004,      // �����
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C006,      // ���PvE
#endif //_NA_008334_20150608_SONNENSCHEIN
        kControl_CheckBT_C002,      // ���α���

        kControl_Button_B000,       // ���� �̸�
        kControl_Button_B001,       // ����
        kControl_Button_B002,       // Ŭ����
        kControl_Button_B003,       // ��å
        kControl_Button_B004,       // ���嵵
        kControl_Button_B005,       // ���ӿ���

        kControl_Button_B006,       // ����ʴ� ��ư
        kControl_Button_B007,       // ������� ��ư
        kControl_Button_B008,       // ��å���� ��ư
        kControl_Button_B010,       // ���Ż�� ��ư
        kControl_Button_B009,       // ��������� ��ư

        kControl_List_L000,         // �������� ��� ����Ʈ
        kControl_VScroll_V000,      // ��ũ��

        kControl_CheckBT_C005,      // �������� ���� �����

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildMember(InterfaceManager* ui_manager);
    virtual ~uiGuildMember();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    GUILD_MEMBER_INFO* GetSelectedMemberInfo();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
    bool OnVScrollThumb(SI_MESSAGE* message);

    void OnClickInvateGuildMember();
    void OnClickDutyVest();
    void OnClickDelegateMaster();

    void UpdateMemberInfo();
    void UpdateButtonState();
    void UpdateWheelScroll();
    void RefreshTab();

private:

};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL