#pragma once
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "uiBase.h"

class uiGuildRanking : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B001 = 0,   // 닫기
        kControl_Button_B002,       // 이전페이지 버튼
        kControl_Button_B003,       // 다음페이지 버튼
        kControl_Text_S008,         // 현재페이지 표시

        kControl_List_L000,         // 랭킹리스트

        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Text_S002,
        kControl_Text_S003,
        kControl_Text_S004,
        kControl_Text_S005,
        kControl_Text_S006,
        kControl_Text_S007,
        

        kControl_Size,
    };

    enum
    {
        kMaxPage = 5,
        kInfoPerPage = 10, 
    };

public:
    static const DWORD kDialog_ID;

public:
    uiGuildRanking(InterfaceManager* ui_manager);
    virtual ~uiGuildRanking();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void OnUpdateSolarDialog();

    void UpdateRankList();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnLButtonDClick(SI_MESSAGE* message);
    void OnClickPrevPageButton();
    void OnClickNextPageButton();
    void ShowGuildInfo();

    int current_page() const { return current_page_; }
    void set_current_page(int value) { current_page_ = value; }

private:
    int current_page_;
};

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
