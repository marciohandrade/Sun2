#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildDutyVest : public uiBase
{
    enum UIControlPos
    {
        kControl_CheckBT_C000 = 0,  // ���긶���� üũ
        kControl_CheckBT_C001,      // �������� üũ
        kControl_CheckBT_C002,      // ���� üũ

        kControl_Button_B000,       // ���긶���� ���Ѻ���
        kControl_Button_B002,       // �������� ���Ѻ���
        kControl_Button_B003,       // ���� ���Ѻ���

        kControl_Text_S001,         // ��å ���� ��� �̸�
        
        kControl_Button_B001,       // Ȯ��
        kControl_Button_B004,       // ���
        kControl_Button_B005,       // �ݱ�
        
        kControl_Size,
    };

    enum
    {
        kSubMaster = 0,
        kCommander,
        kSoldier,

        kMaxIndex,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildDutyVest(InterfaceManager* ui_manager);
    virtual ~uiGuildDutyVest();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnClickCheckDuty(int check_index);
    void OnClickViewAuthorityButton(int button_index);
    void OnClickVerify();

    void UpdateButtonState();

    int current_check_index() const { return current_check_index_; }
    void set_current_check_index(int value) { current_check_index_ = value; }

private:
    int current_check_index_;

};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL