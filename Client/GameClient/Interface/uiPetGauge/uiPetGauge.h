#ifndef _uiPetGauge_h_
#define _uiPetGauge_h_

#if _MSC_VER > 1000
#pragma once
#endif

//! interface lib와 같이 배포되어야 함.

#include "uibase.h"

//------------------------------------------------------------------------------
class uiPetGauge : public uiBase
{
public:
    enum UIControlPos
    {
        kControl_Button_B000 = 0,
        kControl_Text_S000,
        kControl_Text_S001,
        kControl_Picture_P000,
        kControl_Picture_P001,
        kControl_Picture_P002,
        kControl_Picture_P003,
        kControl_Size,
    };

public:
	uiPetGauge(InterfaceManager* ui_man);
	virtual ~uiPetGauge();

	virtual void Init(CDrawBase* draw_base);
	virtual void Release();
	virtual void render(WzVector pet_position);
	virtual void OnShowWindow(BOOL val);
	//virtual void UpdatePosition();
	virtual void MessageProc(SI_MESSAGE* message);
	virtual void OnRenderSolarDialog();

    void NextShowType();

    void set_fullness(int fullness, int max_value);
    void set_intimacy(int intimacy, int max_value);

	int ClickMessage();

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    void ChangeShowType();

    float original_fullness_control_size() const { return original_fullness_control_size_; }
    float original_intimacy_control_size() const { return original_intimacy_control_size_; }
    CControlWZ* control(UIControlPos pos) { if (pos < kControl_Size) return control_ptr_[pos];  return NULL; }

    POINT& dialog_position() { return dialog_position_; }
    void set_dialog_position(int x, int y) { dialog_position_.x = x; dialog_position_.y = y; }

    float original_fullness_control_size_;
    float original_intimacy_control_size_;
    CControlWZ* control_ptr_[kControl_Size];

    POINT dialog_position_;
};


#endif // _uiPetGauge_h_
