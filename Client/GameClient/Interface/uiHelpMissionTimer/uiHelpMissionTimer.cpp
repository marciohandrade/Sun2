#include "SunClientPrecompiledHeader.h"
#include "uiHelpMissionTimer.h"
#include "uiHelpMan/uiHelpMan.h"
#include "TriggerCommon.h"
#include "GameFramework.h"

WzID uiHelpMissionTimer::controls_id_[DIALOG_MAX] = 
{
    StrToWzID("ST00"), //DLGPOS_TIMER_TEXT,
};

uiHelpMissionTimer::uiHelpMissionTimer(InterfaceManager *pUIMan):
mission_timer_text_(NULL),
ui_manager_(NULL),
progress_tIme_(INVALID_TIME),
progress_type_(INVALID_TIMER_TYPE)
{

}

uiHelpMissionTimer::~uiHelpMissionTimer()
{

}

void uiHelpMissionTimer::Init(CDrawBase* pDrawBase)
{
    mission_timer_text_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(controls_id_[DLGPOS_TIMER_TEXT]));
    assert(mission_timer_text_);

    ui_manager_ = static_cast<uiHelpMan*>(GetMan());
    mission_timer_.Clear();
}

void uiHelpMissionTimer::Release()
{

}

void uiHelpMissionTimer::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        MoveDefaultWindowPos();
#ifdef _NA_000000_20151026_TRIGGER_TIMER_SCENE_TYPE_REMOVE
		if (GameFramework::GetCurScene() == SCENE_TYPE_VILLAGE || 
			GameFramework::GetCurScene() == SCENE_TYPE_FIELD)
		{
			progress_type_ = INVALID_TIMER_TYPE;
			ShowInterface(FALSE);
		}
#else
        if (GameFramework::GetCurScene() != SCENE_TYPE_MISSION 
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            && GameFramework::GetCurScene() != SCENE_TYPE_GOLDRUSH
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            )
        {
            progress_type_ = INVALID_TIMER_TYPE;
            ShowInterface(FALSE);
        }
#endif // _NA_000000_20151026_TRIGGER_TIMER_SCENE_TYPE_REMOVE
    }
}

void uiHelpMissionTimer::OnUpdateSolarDialog()
{
    if (IsVisible() != TRUE)
    {
        return;
    }

    if (IsActiveTimer() != true)
    {
        ShowInterface(FALSE);
    }

    bool is_expired_timer = (mission_timer_.IsExpiredManual() == TRUE);
    if ((progress_type_ == eTIMER_CREATE_DECREASE) ||
        (progress_type_ == eTIMER_CREATE_INCREASE))
    {
        DWORD progress_time;
        if (progress_type_ == eTIMER_CREATE_INCREASE)
        {
            progress_time = mission_timer_.GetProcessTime();
        }
        else
        {
            progress_time = mission_timer_.GetRemainedTime();
        }
        SetTimerText(progress_time);

        if (is_expired_timer == true)
        {
            progress_type_ = 3;
            mission_timer_.SetTimer(5000);
        }
    }
    else if ((progress_type_ == eTIMER_DESTROY) &&
             (is_expired_timer == true))
    {
        progress_type_ = INVALID_TIMER_TYPE;
        progress_tIme_ = INVALID_TIME;
        mission_timer_.Clear();
        ShowInterface(FALSE);
    }
}

bool uiHelpMissionTimer::IsValidProgressType(BYTE progress_type)
{
    if ((progress_type == eTIMER_CREATE_DECREASE) ||
        (progress_type == eTIMER_CREATE_INCREASE) ||
        (progress_type == eTIMER_DESTROY))
    {
        return true;
    }
    return false;
}

void uiHelpMissionTimer::SetTimerText(DWORD current_tIme)
{
    if (mission_timer_text_ == NULL)
    {
        return;
    }
    
    // ms --> s
    current_tIme /= 1000;
    // 초단위가 변경되지 않으면 리턴, 불필요하게 컨트롤에 텍스트를 넣지 않도록
    if (progress_tIme_ == current_tIme)
    {
        return;
    }    
    progress_tIme_ = current_tIme;
    TCHAR current_time_string[INTERFACE_STRING_LENGTH] = {0, };
    Snprintf(current_time_string, INTERFACE_STRING_LENGTH, _T("%2d:%02d"), 
             (current_tIme / 60), (current_tIme % 60));
    mission_timer_text_->SetTextWZ(current_time_string);
}

void uiHelpMissionTimer::SetMissionTimer(DWORD require_tIme, BYTE progress_type)
{
    if (IsValidProgressType(progress_type))
    {
        progress_type_ = progress_type;
        mission_timer_.Clear();
        mission_timer_.SetTimer(require_tIme);        
    }
    else
    {
        ShowInterface(FALSE);
    }
}

