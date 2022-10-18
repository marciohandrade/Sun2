#include "SunClientPrecompiledHeader.h"
#pragma once
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "uiBase.h"
#include "DrawTexture.h"

class uiChaosZoneResult : public uiBase
{
    enum UIControlPos
    {
        kControl_Picture_P001, // ���� ���
        kControl_Picture_P003, // �Ķ� ���
        kControl_Picture_P004, // ��ο�
        kControl_Picture_P005, // ����
        kControl_Picture_P007, // ��
        kControl_Picture_P002, // �� ��ũ�Ѹ���
        kControl_Picture_P000, // �Ʒ� ��ũ�Ѹ���
        kControl_Size,
    };
    /*enum
    {
        kFade_InOut_Time = 700,
        kShow_Dialog_Time = 5000,   //5��
    };*/

    enum AnimationProgress
    {
        kAnimaion_Progress_ShowUI, // ����
        kAnimaion_Progress_Open,  // ������ ����
        kAnimaion_Progress_ShowResult, // ���� ���
        kAnimaion_Progress_Close, // ������ ����
        kAnimaion_Progress_HideUI, // ��
        kAnimaion_Progress_End,
    };
    /*
    enum
    {
        //�ִϸ��̼� ���� ƽ
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