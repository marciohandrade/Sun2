#pragma once
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "uiBaseMan.h"

struct IM_ATTENDANCE_CHECK_MANAGER
{
    static const DWORD ATTENDANCE_CHECK_DIALOG;
};


class uiAttendanceCheckDialog;
class uiAttendanceCheckMan : public uiBaseMan
{
public:
    uiAttendanceCheckMan(InterfaceManager* ui_manager);
    virtual ~uiAttendanceCheckMan(void);

public:
    virtual void         OnInitialize();
    void toggle_window(BOOL close_window = FALSE);
    void first_open_window();   // 첫 접속 시 창열림(보상받을게 있다면)
    void set_recv_attendance_info(AttendanceInfo* attendance_info);
    AttendanceInfo& recv_attendance_info() { return recv_attendance_info_; }

    static void HotKeyToggleAttendanceCheckDialog();
private:
    uiAttendanceCheckDialog* ui_attendance_check_dialog_;
    AttendanceInfo recv_attendance_info_;
    BOOL first_open_;
};
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
