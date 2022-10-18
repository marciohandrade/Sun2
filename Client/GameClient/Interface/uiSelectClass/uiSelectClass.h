#pragma once
#if !defined(_NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES)
#include "uiBase.h"

class SelectClass : public uiBase
{
    enum UIControlPos
    {
        kControl_Check_C001 = 0,
        kControl_Check_C003,
        kControl_Check_C004,
        kControl_Check_C005,
        kControl_Check_C006,
        kControl_Size,
    };

public:
    SelectClass(InterfaceManager* ui_manager);
    virtual ~SelectClass();

    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void MessageProc(SI_MESSAGE * message);
    
    void SetCurrentClassType(eCHAR_TYPE select_class_type){ m_CurrentClassType = select_class_type; }
    void SetCurrentClassCode(BYTE selected_class_code){ m_CurrentClassCode = selected_class_code; }
    BYTE GetSelectedClassCode(){ return m_CurrentClassCode; }
    void OnExcute();
    void SelectCreateClass(eCHAR_TYPE select_class_type, BYTE select_class_code);
    void DisplayClassDescription(eCHAR_TYPE class_type);

protected:
    virtual void OnShowWindow(BOOL val);
    virtual void OnRenderSolarDialog();
    void OnLButtonClick(SI_MESSAGE* message);
    void OnLButtonDoubleClick(SI_MESSAGE* message);
    void HideClass(BYTE class_type);
    void ShowClass();

private:
    void RegisterControl();
    void AnimateSlide(DWORD current_tick_count);
    void RenderCircle(DWORD tick_count);

    eCHAR_TYPE m_CurrentClassType;
    BYTE m_CurrentClassCode;
    RECT m_ControlPositionRect[kControl_Size];
    DWORD m_LastObjectKey;
    HANDLE m_SkillEffectHandle;

    int m_PositionAmount;
    //DWORD start_tick_;
    //DWORD end_tick_;
    DWORD m_LastTickCount;
    DWORD m_FrameCount;
};
#endif // _NA_006774_20130617_ARRANGE_UNNECESSARY_RESOURCES

