#pragma once
#include "uiBase.h"
#include "FTextRender.h"

struct Achievement_ParserInfo;

class uiAreaConquestCompliment : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B000 = 0,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Picture_P000,
        kControl_Size,
    };

    enum
    {
        kFade_InOut_Time = 700,
        kShow_Compliment_Time = 5000,   //5√ 
    };

public:
    uiAreaConquestCompliment(InterfaceManager* ui_manager);
    virtual ~uiAreaConquestCompliment(void);

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();

    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);

    void RegisterControl();
    void ShowACCompliment(WORD index_number);
    bool IsHaveCompliment()  { return (ac_complimen_list_.empty() != true); };
    void DeleteList();

private:
    void SetACComplimentData(WORD index_number);
    float GetFadeValue();

private:
    std::list<WORD> ac_complimen_list_;
    util::Timer complimen_timer_;
    bool is_mouse_over_;

#ifdef _SCJ_LONG_TEXT_REDUCING
    TCHAR compliment_text_[INTERFACE_STRING_LENGTH]; 
#endif //_SCJ_LONG_TEXT_REDUCING
    Achievement_ParserInfo* current_info_;
};

inline float uiAreaConquestCompliment::GetFadeValue()
{
    float fade_value = 1.0f;

    DWORD ProgressTime = complimen_timer_.GetProcessTime();

    if ((ProgressTime <= kFade_InOut_Time) && 
        (is_mouse_over_ == false))
    {
        fade_value = static_cast<float>(ProgressTime) / kFade_InOut_Time;		
    }
    else if (ProgressTime >= (kShow_Compliment_Time - kFade_InOut_Time))
    {
        fade_value = 
            1.0f - static_cast<float>(ProgressTime - (kShow_Compliment_Time - kFade_InOut_Time)) / kFade_InOut_Time;
    }

    return fade_value;
}
