#include "SunClientPrecompiledHeader.h"
#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "uiBase.h"
#include "DrawTexture.h"

class uiChaosZoneResult : public uiBase
{
    enum UIControlPos
    {
        kControl_Picture_P001, // 빨간 깃발
        kControl_Picture_P003, // 파란 깃발
        kControl_Picture_P004, // 드로우
        kControl_Picture_P005, // 로즈
        kControl_Picture_P007, // 윈
        kControl_Picture_P002, // 윗 스크롤막대
        kControl_Picture_P000, // 아래 스크롤막대
        kControl_Size,
    };
    /*enum
    {
        kFade_InOut_Time = 700,
        kShow_Dialog_Time = 5000,   //5초
    };*/

    enum AnimationProgress
    {
        kAnimaion_Progress_ShowUI, // 시작
        kAnimaion_Progress_Open,  // 열리는 상태
        kAnimaion_Progress_ShowResult, // 승패 출력
        kAnimaion_Progress_Close, // 닫히는 상태
        kAnimaion_Progress_HideUI, // 끝
        kAnimaion_Progress_End,
    };
    /*
    enum
    {
        //애니메이션 누적 틱
    }
    */

public:
    uiChaosZoneResult(InterfaceManager *ui_man);
    virtual ~uiChaosZoneResult();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual VOID Process(DWORD tick);
    virtual bool ShowInterface(BOOL val, BOOL showoff = FALSE);

    virtual void NetworkProc(MSG_BASE* /*pMsg*/) {};  // Pure
    virtual void MessageProc(SI_MESSAGE* pMessage);   // Pure

    void RegisterControl();
    void ShowChaosZoneResult(ChaosZoneResultType result_type);
    
private:
    void InitAnimaion(ChaosZoneResultType result_type);
    AnimationProgress animation_progress() { return animation_progress_; }
    void set_animation_progress(AnimationProgress animation_progress) { animation_progress_ = animation_progress; }

    DWORD CalculateProgress();
    void UpdateFadeAnimation(DWORD progerss_tick);
    void UpdateScrollAnimation(DWORD progerss_tick);
    void UpdateResultDrawAnimation(DWORD progerss_tick);    


protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    virtual void OnUpdateSolarDialog();

private:
    DWORD start_animation_tick_;
    AnimationProgress animation_progress_;
    
    ChaosZoneResultType result_type_;
    CCtrlPictureWZ* background_picture_pointer_;
    CCtrlPictureWZ* result_picture_pointer_;
    float effect_render_alpha_;
    TCHAR effect_string[INTERFACE_STRING_LENGTH];

    nDrawTexture::DrawTextureManager draw_texture_manager_;
};
//------------------------------------------------------------------------------




#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND