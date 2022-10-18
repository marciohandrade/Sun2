#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"

class uiCharacterStatusMain : public uiBase
{
    enum UIControlPos
    {
        kControl_Text_T999 = 0,     // 캐릭터명
        kControl_TextImage_TI03,    // 레벨
        kControl_TextImage_TI02,    // 클래스
        kControl_TextImage_TI01,    // 길드명
        kControl_TextImage_TI00,    // 칭호
        kControl_Picture_PI00,      // 길드 마크
        kControl_Button_B000,       // 칭호리스트 버튼
        kControl_TextImage_TI34,    // 접속시간
        kControl_TextImage_TI35,    // 접속자포인트
        kControl_Button_BT01,       // 접속자포인트 보상버튼
        kControl_Button_B999,       // 닫기버튼

        kControl_Size,
    };
public:
    static const DWORD kDialog_ID;

public:
    uiCharacterStatusMain(InterfaceManager* interface_manager);
    ~uiCharacterStatusMain();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);    

    void InitAccumulateTimer() { accumulate_timer_.SetTimer(0); }

private:
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    void RenderGuildMark();

    void MoveSubDialog();

    bool OnLButtonClick(SI_MESSAGE* message);

private:
    Timer accumulate_timer_;
};

#endif //_NA_003027_20111013_HONOR_SYSTEM
