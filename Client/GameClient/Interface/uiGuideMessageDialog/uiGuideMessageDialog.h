#pragma once
#include "uiBase.h"

#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "FTextRender.h"

class uiGuideSystemMan;
class uiGuideMessageDialog : public uiBase
{
    enum
    {
        kFade_InOut_Time = 700, // 페이드 진행 시간
        kShow_Compliment_Time = 1000,   // 대기할 시간
    };

public:
    enum UIControlPos
    {
        kControl_Button_BT01 = 0,
        kControl_Button_B000,
        kControl_Button_B001,
        kControl_Button_B002,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Size,
    };
    
    uiGuideMessageDialog(InterfaceManager* ui_manager);
    virtual ~uiGuideMessageDialog(void);

    virtual void Init(CDrawBase* drawbase);
    virtual void Release();

    virtual void MessageProc(SI_MESSAGE* message);
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void OnShowWindow(BOOL val);
    void SetOpenType(BYTE button_type);
    void ShowWindowFade(BOOL val);
    static void ShowFade(DWORD wParam,DWORD lParam); // 콜백함수용
    void AddData(WORD message_code);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void RegisterControl();
    float GetFadeValue();
    void SetData(WORD message_code);
    void PopData();
    void BlinkButton();

    util::Timer dialog_timer_;
    std::list<WORD> guide_message_list_;
    bool is_fade_in_;
    uiGuideSystemMan* ui_guide_system_manager_;
    BYTE open_type_;
    BYTE button_type_;
    TotalGuideNotice::Index guide_code_;
    float fade_value_;
    uiGuideSystemMan* guide_system_manager_;

    FTextParser ftext_parser_;
    FTextSentence ftext_sentence_;
    FTextParam ftext_param_;
    FTextRender ftext_render_;
};

inline float uiGuideMessageDialog::GetFadeValue()
{
    float fade_value = 1.0f;

    DWORD ProgressTime = dialog_timer_.GetProcessTime();

    if (dialog_timer_.GetRemainedTime() > 0)
    {
        fade_value = static_cast<float>(ProgressTime) / kFade_InOut_Time;
        if (fade_value > 1.0f)
        {
            fade_value = 1.0f;
        }
    }
    return fade_value;
}
