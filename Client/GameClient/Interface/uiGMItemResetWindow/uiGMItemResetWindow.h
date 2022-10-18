#pragma once
#ifdef _DEV_VER
#include "uiBase.h"

class uiGMWindowMan;
class uiGMItemResetWindow : public uiBase
{
public:
    enum
    {
        kReset_Type_All,        // 모두
        kReset_Type_Init ,      // 실행시값
        kReset_Type_Parameta,   // 속성
        kReset_Type_Filter,     // 필터
        //kReset_Type_FindWord,   // 검색
        kReset_Type_Created,    // 생성
        kReset_Type_History,    // 히스토리(검색+생성)
        kReset_Type_MAX
    };

    enum UIControlPos
    {
        kControl_Button_L001,
        kControl_Size
    };

    uiGMItemResetWindow(InterfaceManager* ui_manager);
    virtual ~uiGMItemResetWindow(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnUpdateSolarDialog();
    virtual void OnShowWindow(BOOL val);

    void ResetFilter();
private:
    void RegisterControl();
    void OnLButtonClick(SI_MESSAGE* message);

    void ResetAll();
    void ResetInit();
    void ResetParameta();
    void ResetFindWord();
    void ResetHistroy();
    void ResetBookmark();

private:
    uiGMWindowMan* ui_gm_window_manager_;
    CCtrlListWZ* list_control_;
    ITEM_PARAMETAS item_option_parametas_;
};
#endif//_DEV_VER