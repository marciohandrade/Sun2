#include "SunClientPrecompiledHeader.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "uiChaosZoneTimer.h"
#include "ChaosSystem/ChaosSystemData.h"
#include "Interfacemanager.h"
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "ChaosSystem/ChaosSystemMan.h"
#include "uiBattleScoreMan/uiBattleScoreMan.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH


//------------------------------------------------------------------------------
/**
*/
uiChaosZoneTimer::uiChaosZoneTimer(InterfaceManager *ui_man) : uiBase(ui_man)
{

}
//------------------------------------------------------------------------------
/**
*/
uiChaosZoneTimer::~uiChaosZoneTimer()
{
    this->Release();
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneTimer::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControl();

    CControlWZ* control_pointer = static_cast<CControlWZ*>(GetControlWZ_byPosition(kControl_Picture_P000));
    if (control_pointer)
    {
        control_pointer->SetDiscardMessage(true);
    }
    control_pointer = static_cast<CControlWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
    if (control_pointer)
    {
        control_pointer->SetDiscardMessage(true);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneTimer::Release()
{

}
//------------------------------------------------------------------------------
/**
*/
bool uiChaosZoneTimer::ShowInterface(BOOL val, BOOL show_off)
{	
    return uiBase::ShowInterface(val, show_off);
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneTimer::OnShowWindow(BOOL val)
{
    if (val)
    {
        this->MoveDefaultWindowPos();
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneTimer::MessageProc(SI_MESSAGE* message)
{
    if (this->IsLock())
    {
        return;
    }

    /*
    switch (message->eResultMsg) 
    {
    SolarDialog::MessageProc(message);
    break;
    }
    */
    SolarDialog::MessageProc(message);
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneTimer::Process(DWORD tick)
{
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneTimer::OnUpdateSolarDialog()
{
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH    
    DWORD battle_time = 0;
    uiBattleScoreMan* battle_score_manager_ptr = GET_DIALOG_MANAGER(uiBattleScoreMan, UIMAN_BATTLE_SCORE);
    
    if (!battle_score_manager_ptr || 
        (battle_score_manager_ptr->GetIsInit() == false))
    {
        return;        
    }
    		
    switch (battle_score_manager_ptr->GetMode())
    {
    case uiBattleScoreMan::eBTS_NONE_MODE:
        {

        }
        break;
    case uiBattleScoreMan::eBTS_BATTLE_GROUND:
        {
            battle_time = ChaosSystem::GetBattleGroundData().GetBattleTime();
        }
        break;
    case uiBattleScoreMan::eBTS_GOLD_RUSH:
        {
            battle_time = ChaosSystem::GetGoldRushData().GetGoldRushTime();
        }
        break;
    default:
        {
            assert(!"맞지 않는 Mode이다");            
        }
        break;
    }        
#else
    DWORD battle_time = ChaosSystem::GetBattleGroundData().GetBattleTime();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    CCtrlStaticWZ* text_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S001));
    if (text_control)
    {
        int mili_second = battle_time % 1000;
        battle_time /= 1000;

        //int hour = battle_time / 3600;
        battle_time %= 3600;
        int minute = battle_time / 60;
        battle_time %= 60;
        int second = battle_time % 60;

        TCHAR message_string[INTERFACE_STRING_LENGTH] = {0,};
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};

        // 2692 %s
        g_InterfaceManager.GetInterfaceString(2692, temp_string, INTERFACE_STRING_LENGTH);
        Snprintf(message_string, INTERFACE_STRING_LENGTH-1, "%03d'%02d''%03d", minute, second, mili_second);

        text_control->SetTextWZ(message_string);
    }
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneTimer::OnRenderSolarDialog()
{
}
//------------------------------------------------------------------------------
/**
*/
void uiChaosZoneTimer::RegisterControl()
{
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("T001"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("T002"), kControl_Text_S001);
}
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND