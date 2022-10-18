#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildMenu : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B001 = 0, // ���â��/�ػ�/�ػ�����
        kControl_Button_B028,     // ��巩ŷ
        kControl_Button_B011,     // ��帶ũ
        kControl_Button_B012,     // ���â��
        kControl_Button_B004,     // �ݱ�
        kControl_Text_S002,       // �ȳ��޽���

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildMenu(InterfaceManager* ui_manager);
    virtual ~uiGuildMenu();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void OnGuildCreateNDestroy();

    void UpdateGuildMenu();

};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
