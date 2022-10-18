#include "SunClientPrecompiledHeader.h"
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

#include "ChaosSystemMan.h"
#include "uiChaosZoneVote.h"
#include "uiChaosZoneVoteResult.h"
#include "GlobalFunc.h"
#include "Application.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
//------------------------------------------------------------------------------
/**
*/
uiChaosZoneVote::uiChaosZoneVote(InterfaceManager *ui_man) : uiBase(ui_man)
{
}

//------------------------------------------------------------------------------
/**
*/
uiChaosZoneVote::~uiChaosZoneVote()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneVote::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("S000"), kControl_Title_Text);
    RegisterControlPosition(StrToWzID("S003"), kControl_Message_Text);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll);
    RegisterControlPosition(StrToWzID("P000"), kControl_VScroll_Picture);
    RegisterControlPosition(StrToWzID("B001"), kControl_Verify_Button);
    RegisterControlPosition(StrToWzID("B000"), kControl_Confirm_Button);
    RegisterControlPosition(StrToWzID("B028"), kControl_Rejection_Button);
    RegisterControlPosition(StrToWzID("B004"), kControl_Close_Button);


    CControlWZ* control_pointer = NULL;
    for (int i = kControl_Close_Button; i <= kControl_Verify_Button; ++i)
    {
        control_pointer = GetControlWZ_byPosition(i);
        if (control_pointer)
        {
            control_pointer->ShowWindowWZ(WZ_HIDE);
        }
    }
    control_pointer = GetControlWZ_byPosition(kControl_Title_Text);
    if (control_pointer)
    {
        // 2766 추방투표
        control_pointer->SetTextID(2766);
    }
    control_pointer = GetControlWZ_byPosition(kControl_Confirm_Button);
    if (control_pointer)
    {
        // 2770 찬성
        control_pointer->SetTextID(2769);
    }
    control_pointer = GetControlWZ_byPosition(kControl_Rejection_Button);
    if (control_pointer)
    {
        // 2770 반대
        control_pointer->SetTextID(2770);
    }

    ZeroMemory(character_name_, MAX_CHARNAME_LENGTH+1);
    current_time_ = 0;
    
}

//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneVote::Release()
{

}

bool uiChaosZoneVote::ShowInterface(BOOL val, BOOL show_off)
{	
    return uiBase::ShowInterface(val, show_off);
}

void uiChaosZoneVote::OnShowWindow(BOOL val)
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

void uiChaosZoneVote::MessageProc(SI_MESSAGE* message)
{
    if (this->IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            OnLButtonClick(message);
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}

bool uiChaosZoneVote::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Confirm_Button:
    //case kControl_Close_Button:
        {
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            if (GlobalFunc::IsBattleGroundScene() == TRUE)
            {
                ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_VOTE_SYN(true);
            }
            else if (GlobalFunc::IsGoldRushScene() == TRUE)
            {
                ChaosSystem::PacketFunction::SEND_CG_GOLDRUSH_VOTE_SYN(true);
            }
#else
            ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_VOTE_SYN(true);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            this->ShowInterface(false);
        }
        break;
    case kControl_Rejection_Button:
        {
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            if (GlobalFunc::IsBattleGroundScene() == TRUE)
            {
                ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_VOTE_SYN(false);
            }
            else if (GlobalFunc::IsGoldRushScene() == TRUE)
            {
                ChaosSystem::PacketFunction::SEND_CG_GOLDRUSH_VOTE_SYN(false);
            }
#else
            ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_VOTE_SYN(false);
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
            this->ShowInterface(false);
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiChaosZoneVote::Process(DWORD tick)
{
    if (IsVisible())
    {
        current_time_ = clock_function::GetTickCount();

        if (vote_end_time_ < current_time_)
        {
            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
            // 2774 찬반의사를 결정하지 않아 시스템에서 찬성으로 투표하였습니다.
            g_InterfaceManager.GetInterfaceString(2774, message, INTERFACE_STRING_LENGTH);
            GlobalFunc::OutputMsg_Warning(eOUTPUT_MSG_CHAT, message);

            this->ShowInterface(FALSE);
        }
    }
}


void uiChaosZoneVote::OnUpdateSolarDialog()
{
    TCHAR	temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	message_string1[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	message_string2[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	result_string[INTERFACE_STRING_LENGTH] = {0,};

    //2767 %s님의 추방에 찬성하시나요? \n만장일치로 찬성표가 되어야만 추방이 가능합니다.
    g_InterfaceManager.GetInterfaceStringFormat(message_string1, INTERFACE_STRING_LENGTH, 2767, character_name());

    int remain_time = vote_end_time_ - current_time_ - 1;
    _itot((max(remain_time,0)/1000) + 1, temp_string, 10);
    // 2768 투표마감까지 남은 시간: %s초
    g_InterfaceManager.GetInterfaceStringFormat(message_string2, INTERFACE_STRING_LENGTH, 2768, temp_string);

    Snprintf(result_string, INTERFACE_STRING_LENGTH-1, "%s\n\n%s", message_string1, message_string2);
    CControlWZ* text_control = GetControlWZ_byPosition(kControl_Message_Text);
    if (text_control)
    {
        text_control->SetTextWZ(result_string);
    }
}

void uiChaosZoneVote::OnRenderSolarDialog()
{
}

#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM