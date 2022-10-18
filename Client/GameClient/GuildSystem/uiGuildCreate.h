#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildCreate : public uiBase
{
    enum UIControlPos
    {   
        kControl_Text_T101 =0,      // â�� �ʿ� ����
        kControl_Text_S000,         // â�� �ʿ� ����
        kControl_Text_S004,         // �ڽ��� ����
        kControl_Text_S005,         // �ڽ��� ���� ����
        kControl_Text_S006,         // ���� �Է� �ȳ��ؽ�Ʈ
        kControl_Edit_E000,         // ���� �Է� ����Ʈ

        kControl_Button_B001,       // ��� â�� ��ư
        kControl_Button_B002,       // ��� ��ư
        kControl_Button_B999,       // �ݱ� ��ư

        kControl_Size,
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildCreate(InterfaceManager* ui_manager);
    virtual ~uiGuildCreate();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

    void GetGuildName(TCHAR* OUT guild_name, int buffer_size);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnEditEnterDown(SI_MESSAGE* message);
    void OnClickGuildCreate();

    void UpdateCreateInfo();
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
