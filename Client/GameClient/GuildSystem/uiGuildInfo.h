#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"
#include "TabCheckButtonControlManager.h"

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
class uiToolTipMan;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
class uiGuildInfo : public uiBase
{
    enum UIControlPos
    {
        kControl_TextImage_TI02 = 0,// ��忬��
        kControl_TextImage_I006,    // ����
        kControl_TextImage_I007,    // ��帶����
        kControl_TextImage_I008,    // ��巩ŷ
        kControl_TextImage_I009,    // ��巹��
        kControl_TextImage_I010,    // �������

        kControl_CheckBT_C000,      // ��尡�Կ�û����üũ��ư
        kControl_Text_S004,         // ��尡�Կ�û����
        kControl_Text_S005,         // �������

        kControl_Button_B999,       // �ݱ�
        kControl_Button_BT01,       // ���α�� �� ���Կ�û

        kControl_Picture_PI00,      // ��帶ũ        

        kControl_Size,
    };

public:
    enum TabDialog
    {
        kGuildIntro = 0,
        kGuildMember, 
        kGuildInstitution, 
        kGuildManagement, 
#ifdef _NA_008334_20150608_SONNENSCHEIN
        kGuildPvE, 
#endif //_NA_008334_20150608_SONNENSCHEIN
        kGuildLog, 

        kTab_Max,
    };

    static const DWORD kDialog_ID;

public:
    uiGuildInfo(InterfaceManager* ui_manager);
    virtual ~uiGuildInfo();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    void InitTabController();
    void MoveSubDialog();

    bool show_flag() const { return show_flag_; }
    void set_show_flag(bool value) { show_flag_ = value; }

    TabCheckButtonControllManager& tab_button_controller() { return tab_button_controller_; }

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnClickDonateGuildCoin();

    void UpdateGuildInfo();
    void UpdateButtonState();

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    void RenderTooltip();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

private:
    bool show_flag_;    

    TabCheckButtonControllManager tab_button_controller_;
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    uiToolTipMan* tooltip_manager_ptr_;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL