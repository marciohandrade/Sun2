#pragma once
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "uiAttendanceCheckMan.h"

class CalendarSlot
{
public:
    CalendarSlot();
    ~CalendarSlot();

    void SetString(TCHAR* string);
    void SetForceType(int force_type);
    void ShowCheckType(eATTENDANCE_INFO attendance_info) { attendance_info_ = attendance_info; }
    void ShowSlotImage(BOOL is_show);
    void SetItem(DWORD reward_code);

    CODETYPE item_code() { return item_code_; }
    CODETYPE item_type_index() { return item_type_index_; }
    DWORD    item_limit_time() { return item_limit_time_; }

    void set_static_control(CControlWZ* control) { static_control_ = control; }
    void set_force_control(CControlWZ* control) { force_control_ = control; }
    void set_check_control(CControlWZ* control);
    void set_item_control(CControlWZ* control) { item_control_ = control; }

    CControlWZ* static_control() { return static_control_; }
    CControlWZ* force_control() { return force_control_; }
    CControlWZ* check_control() { return check_control_; } 
    CControlWZ* item_control() { return item_control_; }


    void Render();
    BOOL IsMouseOver(int mouse_x, int mouse_y);

private:
    CODETYPE item_code_;
    DWORD    item_type_index_;
    DWORD    item_limit_time_;
    eATTENDANCE_INFO attendance_info_;

    IconImage*  icon_image_; // 아이템아이콘

    //
    CControlWZ* static_control_; // 텍스트
    CControlWZ* force_control_;  // 강조이미지
    CControlWZ* check_control_;  // 체크이미지
    CControlWZ* item_control_;   // 아이템슬롯
    //
};
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
