#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildCreate : public uiBase
{
    enum UIControlPos
    {   
        kControl_Text_T101 =0,      // 창설 필요 레벨
        kControl_Text_S000,         // 창설 필요 하임
        kControl_Text_S004,         // 자신의 레벨
        kControl_Text_S005,         // 자신의 소지 하임
        kControl_Text_S006,         // 길드명 입력 안내텍스트
        kControl_Edit_E000,         // 길드명 입력 에디트

        kControl_Button_B001,       // 길드 창설 버튼
        kControl_Button_B002,       // 취소 버튼
        kControl_Button_B999,       // 닫기 버튼

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
