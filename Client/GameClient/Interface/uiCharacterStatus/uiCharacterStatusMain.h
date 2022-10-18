#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"

class uiCharacterStatusMain : public uiBase
{
    enum UIControlPos
    {
        kControl_Text_T999 = 0,     // ĳ���͸�
        kControl_TextImage_TI03,    // ����
        kControl_TextImage_TI02,    // Ŭ����
        kControl_TextImage_TI01,    // ����
        kControl_TextImage_TI00,    // Īȣ
        kControl_Picture_PI00,      // ��� ��ũ
        kControl_Button_B000,       // Īȣ����Ʈ ��ư
        kControl_TextImage_TI34,    // ���ӽð�
        kControl_TextImage_TI35,    // ����������Ʈ
        kControl_Button_BT01,       // ����������Ʈ �����ư
        kControl_Button_B999,       // �ݱ��ư

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
