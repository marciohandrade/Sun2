#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
#include "uiAttendanceCheckMan.h"
#include "uiAttendanceCheckDialog.h"

const DWORD IM_ATTENDANCE_CHECK_MANAGER::ATTENDANCE_CHECK_DIALOG = StrToWzID("0320");

uiAttendanceCheckMan::uiAttendanceCheckMan(InterfaceManager* ui_manager_ptr) : 
uiBaseMan(ui_manager_ptr)
{
     ui_attendance_check_dialog_ = NULL;
     first_open_ = FALSE;
}

uiAttendanceCheckMan::~uiAttendanceCheckMan(void)
{
}

void uiAttendanceCheckMan::OnInitialize()
{
    ui_attendance_check_dialog_ = CreateUIDialog<uiAttendanceCheckDialog>(
        IM_ATTENDANCE_CHECK_MANAGER::ATTENDANCE_CHECK_DIALOG,
        "Data\\Interface\\32_attend_check.iwz", this, TRUE);
}

void uiAttendanceCheckMan::toggle_window(BOOL close_window)
{
    if (ui_attendance_check_dialog_ == NULL)
    {
        return;
    }

    if ((ui_attendance_check_dialog_->IsVisibleWindow() == TRUE) ||
        (close_window == TRUE))
    {
        ui_attendance_check_dialog_->ShowInterface(FALSE);
    }
    else
    {
        ui_attendance_check_dialog_->ShowInterface(TRUE);
    }
}

void uiAttendanceCheckMan::first_open_window()
{
    // 첫 접속 시 창열림(보상받을게 있다면)
    if (ui_attendance_check_dialog_ && first_open_ == FALSE)
    {
        first_open_ = TRUE;
        int reward_possible = recv_attendance_info_.GetAttendanceInfoCount(eATTENDANCE_INFO_ATTENDED);
        if (reward_possible > 0)
        {
            ui_attendance_check_dialog_->ShowInterface(TRUE);
        }
    }
}

void uiAttendanceCheckMan::set_recv_attendance_info(AttendanceInfo* attendance_info)
{
    if (attendance_info == NULL)
    {
        return;
    }

    memcpy(&recv_attendance_info_, attendance_info, sizeof(AttendanceInfo));

    if (ui_attendance_check_dialog_)
    {
        ui_attendance_check_dialog_->set_default_month(attendance_info->GetMonth());
        ui_attendance_check_dialog_->UpdateCalendar(attendance_info->GetYear(), attendance_info->GetMonth());
    }
}
void uiAttendanceCheckMan::HotKeyToggleAttendanceCheckDialog()
{
    uiAttendanceCheckMan* attendance_man = GET_DIALOG_MANAGER(uiAttendanceCheckMan, UIMAN_ATTENDANCE_CHECK);
    if (attendance_man)
    {
        attendance_man->toggle_window();
    }
}
#endif // _NA_007932_20150112_ATTENDANCE_CHECK_SYSTEM
