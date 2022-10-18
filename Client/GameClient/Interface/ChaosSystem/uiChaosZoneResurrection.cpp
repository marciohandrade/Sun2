#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiChaosZoneResurrection.h"
#include "ChaosSystem/ChaosSystemPacketFunction.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "uiPartyMan/uiPartyMan.h"
#include "GlobalFunc.h"
#include "GameFrameWork.h"
#include "Application.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "SceneBase.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

WzID uiChaosZoneResurrection::m_wzId[DIALOG_MAX] = 
{ 
    StrToWzID("S000"),  // caption
    StrToWzID("S002"),  // text
};

//------------------------------------------------------------------------------
/**
*/
uiChaosZoneResurrection::uiChaosZoneResurrection(InterfaceManager *ui_man) : uiBase(ui_man)
{
}

//------------------------------------------------------------------------------
/**
*/
uiChaosZoneResurrection::~uiChaosZoneResurrection()
{
    this->Release();
}

//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResurrection::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    
    CControlWZ* text_control = getControl(DLGPOS_TEXT);
    if (text_control)
    {
        text_control->SetTextWZ(NULL);
    }
}

//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneResurrection::Release()
{

}

bool uiChaosZoneResurrection::ShowInterface(BOOL val, BOOL show_off)
{	
    return uiBase::ShowInterface(val, show_off);
}


void uiChaosZoneResurrection::ResurrectionRefuse()
{
    //ChaosSystem::PacketFunction::SEND_CG_BATTLE_GROUND_RESURECTION_REFUSE_SYN();
    this->ShowInterface(FALSE);
}

void uiChaosZoneResurrection::OnShowWindow(BOOL val)
{
    if (val)
    {
        SolarDialog::MoveCenter();
        //RECT rect;
        //GetDialogWZ()->GetDlgSize(&rect);

        //int width = rect.right;
        //int height = rect.bottom;
        //int mw = width >> 1;
        //int mh = height >> 1;

        //POINT pt = g_pApplication->Get2DModeMapping();
        //long pw = (pt.x >> 1) - mw;
        //long ph = (pt.y >> 1) - mh;
        ////long ph = (long)((pt.y * 0.33f) - mh);

        //GetDialogWZ()->MoveWindowWZ((float)pw, (float)ph);


        current_tick_ = 0;

        this->OffLock();
        this->OffPending();
	}
}

void uiChaosZoneResurrection::MessageProc(SI_MESSAGE* message)
{
    if (this->IsLock())
        return;

    switch (message->eResultMsg) 
    {
    case RT_MSG_LBUTTONCLICK:
        {
            //switch (getCtrlIDToPos(message->dwCtrlID))
            //{
            //case DLGPOS_OK:
            //    {
            //        //ResurrectionRefuse(); // 부활 거부
            //        this->ShowInterface(FALSE);
            //    }
            //    break;
            //}
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}

void uiChaosZoneResurrection::Process(DWORD tick)
{
    current_tick_ = clock_function::GetTickCount();

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    if (GameFramework::GetCurrentScene() && 
        GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_BATTLEGROUND)
    {
        if (ChaosSystem::GetBattleGroundData().player_state() != kPlayerStateStandbyResurection)
        {
            this->ShowInterface(FALSE);
        }
    }
    else if (GameFramework::GetCurrentScene() && 
        GameFramework::GetCurrentScene()->GetType() == SCENE_TYPE_GOLDRUSH)
    {
        if (ChaosSystem::GetGoldRushData().player_state() != kPlayerStateStandbyResurection)
        {
            this->ShowInterface(FALSE);
        }
    }
#else
    if (ChaosSystem::GetBattleGroundData().player_state() != kPlayerStateStandbyResurection)
    {
        this->ShowInterface(FALSE);
    }
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
}


void uiChaosZoneResurrection::OnUpdateSolarDialog()
{
    TCHAR	temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	message_string[INTERFACE_STRING_LENGTH] = {0,};

    int remain_time = resurrection_tick_ - current_tick_;
    _itot((max(remain_time,0)/1000) + 1, temp_string, 10);
    // 2601 %s초 후에 부활합니다.
    g_InterfaceManager.GetInterfaceStringFormat(message_string, INTERFACE_STRING_LENGTH, 2601, temp_string);

    CControlWZ* text_control = getControl(DLGPOS_TEXT);
    if (text_control)
    {
        text_control->SetTextWZ(message_string);
    }
}

void uiChaosZoneResurrection::OnRenderSolarDialog()
{
    /*TCHAR	temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR	message_string[INTERFACE_STRING_LENGTH] = {0,};

    int remain_time = resurrection_tick_ - current_tick_;
    _itot((max(remain_time,0)/1000) + 1, temp_string, 10);
    // 2601 %s초 후에 부활합니다.
    g_InterfaceManager.GetInterfaceStringFormat(message_string, INTERFACE_STRING_LENGTH, 2601, temp_string);

    CControlWZ* text_control = getControl(DLGPOS_TEXT);
    if (text_control)
    {
        text_control->SetTextWZ(message_string);
    }*/


    /*std::vector<util::_tstring> vector_message;
    vector_message.clear();

    CControlWZ* list_control = getControl(DLGPOS_MSG);
    if (list_control)
    {
        RECT control_rect;
        RECT rect;
        control_rect = list_control->GetSizeRect();
        int list_width = control_rect.right - control_rect.left - 20;

        GlobalFunc::ProcessWordWrapByFontSize(&vector_message,
            message_string,
            StrToWzID("mn12"),
            _tcslen(message_string),
            list_width);

        this->GetDrawBase()->x_pManagerTextOut->StartToUseFont(StrToWzID("mn12"));
        for (size_t i = 0; i < vector_message.size(); ++i)
        {
            SetRect(&rect,
                control_rect.left + 2,
                control_rect.top + (16*i),
                control_rect.right,
                control_rect.top + (16*i) + 16);

            TCHAR *string_pointer = const_cast<TCHAR *>(vector_message[i].c_str());

            if (string_pointer) 
            {
                this->GetDrawBase()->x_pManagerTextOut->DrawText(
                    string_pointer,
                    &rect,
                    WzColor_RGBA(255, 133, 50, 255),
                    WzColor_RGBA(0,0,0,0),
                    TP_HLEFT);
            }
        }
        this->GetDrawBase()->x_pManagerTextOut->EndUseFont();
    }*/
}



#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND