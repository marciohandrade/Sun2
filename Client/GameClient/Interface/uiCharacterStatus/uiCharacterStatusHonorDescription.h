#pragma once

#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "uiBase.h"
#include "FTextRender.h"

class uiCharacterStatusHonorDescription : public uiBase
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0, // 닫기 버튼
        kControl_Text_S000,       // 명예/평판 타이틀
        kControl_Text_S001,       // 명예/평판 설명
        kControl_VScroll_V000,    // 스크롤 컨트롤
        kControl_Picture_P001,    // 스크롤 배경이미지

        kControl_Size,
    };
public:
    static const DWORD kDialog_ID;

public:
    uiCharacterStatusHonorDescription(InterfaceManager* interface_manager);
    ~uiCharacterStatusHonorDescription();

    virtual void Init(CDrawBase* draw_base);
    virtual void OnShowWindow(BOOL is_show);
    virtual void MessageProc(SI_MESSAGE* message);

    void SetHonorDescription(BYTE honor_type, WORD honor_code);
 
private:
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    
    void UpdateMouseWheel();

    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnUpScroll(SI_MESSAGE* message);
    bool OnDownScroll(SI_MESSAGE* message);
    bool OnThumbScroll(SI_MESSAGE* message);

    FTextRender& honor_description() { return honor_description_; }

private:
    BYTE honor_type_;
    WORD honor_code_;

    FTextRender honor_description_;
};

#endif //_NA_003027_20111013_HONOR_SYSTEM
