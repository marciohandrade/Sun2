#include "SunClientPrecompiledHeader.h"
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

#include "uiChaosZoneVoteResult.h"


#include "GlobalFunc.h"
#include "Application.h"


uiChaosZoneVoteResult::uiChaosZoneVoteResult(InterfaceManager *ui_man) : uiBase(ui_man)
{
}

uiChaosZoneVoteResult::~uiChaosZoneVoteResult()
{
    this->Release();
}

void uiChaosZoneVoteResult::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    
    RegisterControlPosition(StrToWzID("S000"), kControl_Title_Text);
    RegisterControlPosition(StrToWzID("S002"), kControl_Message_Text);

    CControlWZ* control_pointer = GetControlWZ_byPosition(kControl_Title_Text);
    if (control_pointer)
    {
        // 2766 추방투표
        control_pointer->SetTextID(2766);
    }
}

void uiChaosZoneVoteResult::Release()
{

}

bool uiChaosZoneVoteResult::ShowInterface(BOOL val, BOOL show_off)
{	
    return uiBase::ShowInterface(val, show_off);
}

void uiChaosZoneVoteResult::OnShowWindow(BOOL val)
{
    if (val)
    {
        //SolarDialog::MoveCenter();
        RECT rect;
        GetDialogWZ()->GetDlgSize(&rect);

        int width = rect.right;
        int height = rect.bottom;
        int mw = width >> 1;
        int mh = height >> 1;

        POINT pt = g_pApplication->Get2DModeMapping();
        //long pw = (pt.x >> 1) - mw;
        //long ph = (pt.y >> 1) - mh;
        long pw = (long)((pt.x * 0.65f) - mw);
        long ph = (long)((pt.y * 0.75f) - mh);

        GetDialogWZ()->MoveWindowWZ((float)pw, (float)ph);

        this->OffLock();
        this->OffPending();
	}
}

void uiChaosZoneVoteResult::MessageProc(SI_MESSAGE* message)
{
    if (this->IsLock())
        return;

    SolarDialog::MessageProc(message);
}

void uiChaosZoneVoteResult::Process(DWORD tick)
{
    if (IsVisible())
    {
        current_time_ = clock_function::GetTickCount();

        if (vote_end_time_ < current_time_)
        {
            this->ShowInterface(FALSE);
        }
    }
}


void uiChaosZoneVoteResult::OnUpdateSolarDialog()
{
    TCHAR	temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	message_string1[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	message_string2[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	message_string3[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	message_string4[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	result_string[INTERFACE_STRING_LENGTH] = {0,};

    // 2780 %s님의 추방투표 진행 현황
    g_InterfaceManager.GetInterfaceStringFormat(message_string1, INTERFACE_STRING_LENGTH, 2780, character_name());

    _itot(vote_yes_count(), temp_string, 10);
    //2771	찬성 : %s표
    g_InterfaceManager.GetInterfaceStringFormat(message_string2, INTERFACE_STRING_LENGTH, 2771, temp_string);

    _itot(vote_no_count(), temp_string, 10);
    //2772	반대 : %s표
    g_InterfaceManager.GetInterfaceStringFormat(message_string3, INTERFACE_STRING_LENGTH, 2772, temp_string);

    int remain_time = vote_end_time_ - current_time_ - 1;
    _itot((max(remain_time,0)/1000) + 1, temp_string, 10);
    // 2768 투표마감까지 남은 시간: %s초
    g_InterfaceManager.GetInterfaceStringFormat(message_string4, INTERFACE_STRING_LENGTH, 2768, temp_string);

    Snprintf(result_string, INTERFACE_STRING_LENGTH-1, "%s\n%s\n%s\n%s",
        message_string1, message_string2, message_string3, message_string4);
    CControlWZ* text_control = GetControlWZ_byPosition(kControl_Message_Text);
    if (text_control)
    {
        text_control->SetTextWZ(result_string);
    }
}

void uiChaosZoneVoteResult::OnRenderSolarDialog()
{
}



#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM