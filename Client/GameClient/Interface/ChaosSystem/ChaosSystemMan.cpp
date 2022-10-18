#include "SunClientPrecompiledHeader.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "ChaosSystem/ChaosSystemData.h"
#include "ChaosSystemMan.h"
#include "uiChaosZoneBattle.h"
#include "uiChaosZoneRankBattle.h"
#include "uiChaosZoneSSQ.h"
#include "uiChaosZoneArena.h"
#include "uiChaosZonePVP.h"
#include "uiChaosZoneHistory.h"
#include "uiChaosZoneInfo.h"
#include "uiChaosZoneResurrection.h"
#include "uiChaosZonePassword.h"
#include "uiChaosZoneResult.h"
#include "uiChaosZoneTimer.h"
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
  #include "uiChaosZoneVote.h"
  #include "uiChaosZoneVoteResult.h"
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#include "uiChaosZoneGoldRush.h"
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

#include "uiBaseMan.h"
#include "GlobalFunc.h"
#include "LoadScene.h"
#include "Map.h"
#include "CastDialog.h"

#include "ChaosZoneSystem/PacketStruct_ChaosZoneSystem.h"


const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_BATTLE = StrToWzID("0642");
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_GOLDRUSH = StrToWzID("0649");
#else
const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_RANK_BATTLE = 0;//g_IDGenerator.GenerateID();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_ARENA = 0;//g_IDGenerator.GenerateID();
const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_SSQ = StrToWzID("0643");
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
  const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP = StrToWzID("0646");
#else
  const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP = 0;
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP
const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_HISTORY = StrToWzID("0647");
const DWORD IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_INFO = StrToWzID("0644");
const DWORD	IM_CHAOS_SYSTEM_MANAGER::BATTLE_GROUND_RESURRECTION_POPUP = g_IDGenerator.GenerateID();
const DWORD	IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PASSWORD = g_IDGenerator.GenerateID();
const DWORD	IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_RESULT = StrToWzID("0648");
const DWORD	IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_TIMER = StrToWzID("0908");
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
const DWORD	IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE = g_IDGenerator.GenerateID();;
const DWORD	IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE_RESULT = g_IDGenerator.GenerateID();;
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
const DWORD IM_CHAOS_SYSTEM_MANAGER::UI_END = -1;


DWORD chaos_system_windows[] = {IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_BATTLE,  // 카오스존 ui 탭 배열
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_GOLDRUSH,
#else
    IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_RANK_BATTLE,
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_ARENA,
    IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_SSQ,
    IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP,
    IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_HISTORY,
    IM_CHAOS_SYSTEM_MANAGER::UI_END};

//------------------------------------------------------------------------------
uiChaosSystemMan::uiChaosSystemMan(InterfaceManager *ui_manager_ptr) : uiBaseMan(ui_manager_ptr),
ui_chaoszone_battle_(NULL),
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
ui_chaoszone_goldrush_(NULL),
#else
ui_chaoszone_rank_battle_(NULL),
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
ui_chaoszone_ssq_(NULL),
ui_chaoszone_arena_(NULL),
ui_chaoszone_pvp_(NULL),
ui_chaoszone_history_(NULL),
ui_chaoszone_info_(NULL),
ui_chaoszone_resurrection_(NULL),
ui_chaoszone_password_(NULL),

wait_second_(5)
{
    window_position_.top = 0;
    window_position_.left = 0;
    window_position_.right = -1; // right 값이 -1 인 경우는 초기화가 안된 것으로 간주
    window_position_.bottom = 0;


#if NATION_BUILD_VERSION >= 1404
    last_window_id_ = IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_BATTLE;
#elif defined(_NA_007114_20140313_CHAOSZONE_GOLDRUSH)
    last_window_id_ = IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_GOLDRUSH;
#elif defined(_NA_000000_20120914_CHAOS_ZONE_BATTLE_GROUND_ACTIVATION)
    last_window_id_ = IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_BATTLE;
#else
    last_window_id_ = IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP;
#endif

}

//------------------------------------------------------------------------------
uiChaosSystemMan::~uiChaosSystemMan()
{

}

//------------------------------------------------------------------------------
void uiChaosSystemMan::OnInitialize()
{
    ui_chaoszone_battle_ = CreateUIDialog<uiChaosZoneBattle>(
        uiChaosZoneBattle::kDialog_ID,
        "Data\\Interface\\64_2_chaos_Zone.iwz", this);

    //ui_chaoszone_rank_battle_ = CreateUIDialog<uiChaosZoneRankBattle>(uiChaosZoneRankBattle::kDialog_ID, "Data\\Interface\\73_Guild_Create.iwz", this);

    ui_chaoszone_ssq_ = CreateUIDialog<uiChaosZoneSSQ>(
        uiChaosZoneSSQ::kDialog_ID,
        "Data\\Interface\\64_3_chaos_Zone.iwz", this);

    //ui_chaoszone_arena_ = CreateUIDialog<uiChaosZoneArena>(uiChaosZoneArena::kDialog_ID, "Data\\Interface\\73_29_Guild_MarkCombination.iwz", this);

#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    ui_chaoszone_pvp_ = CreateUIDialog<uiChaosZonePVP>(
        IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PVP,
        "Data\\Interface\\64_6_chaos_Zone.iwz", this);
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

    ui_chaoszone_history_ = CreateUIDialog<uiChaosZoneHistory>(
        IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_HISTORY,
        "Data\\Interface\\64_7_chaos_Zone.iwz", this, TRUE);

    ui_chaoszone_info_ = CreateUIDialog<uiChaosZoneInfo>(
        uiChaosZoneInfo::kDialog_ID,
        "Data\\Interface\\64_4_chaos_info.iwz", this);

    ui_chaoszone_resurrection_ = CreateUIDialog<uiChaosZoneResurrection>(
        IM_CHAOS_SYSTEM_MANAGER::BATTLE_GROUND_RESURRECTION_POPUP,
        "Data\\Interface\\82_8_Asking_Window.iwz", this, TRUE, TRUE);


#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
    ui_chaoszone_password_ = CreateUIDialog<uiChaosZonePassword>(
        IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_PASSWORD,
        "Data\\Interface\\62_2_Input_Password.iwz", this, FALSE, TRUE);
#endif //_NA_006088_20120910_CHAOS_ZONE_FREE_PVP

    ui_chaoszone_result_ = CreateUIDialog<uiChaosZoneResult>(
        IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_RESULT,
        "Data\\Interface\\64_8_chaos_result.iwz", this, TRUE);

    ui_chaoszone_timer_ = CreateUIDialog<uiChaosZoneTimer>(
        IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_TIMER,
        "Data\\Interface\\90_8_timer.iwz", this, TRUE);

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    ui_chaoszone_vote_ = CreateUIDialog<uiChaosZoneVote>(
        IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE,
        "Data\\Interface\\82_9_Asking_Window.iwz", this, TRUE, TRUE);
    ui_chaoszone_vote_result_ = CreateUIDialog<uiChaosZoneVoteResult>(
        IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_VOTE_RESULT,
        "Data\\Interface\\82_8_Asking_Window.iwz", this, TRUE, TRUE);
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    ui_chaoszone_goldrush_ = CreateUIDialog<uiChaosZoneGoldRush>(
        IM_CHAOS_SYSTEM_MANAGER::CHAOSZONE_GOLDRUSH,
        "Data\\Interface\\64_9_chaos_Zone.iwz", this);    
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    chaos_system_windows_size_ = 0;
    int i = 0;
    while (chaos_system_windows[i] != IM_CHAOS_SYSTEM_MANAGER::UI_END)
    {
        ++chaos_system_windows_size_;
        ++i;
    }
}

//------------------------------------------------------------------------------
void uiChaosSystemMan::OnRelease()
{

}
void uiChaosSystemMan::Process(DWORD tick_count)
{
    if (wait_timer_.Check() != TIMER_STAT_NONE)
    {
        if (wait_timer_.Check() == TIMER_STAT_FINISH)
        {
            WaitGague(false);
        }
        else
        {
            CastDialog* cast_dialog = static_cast<CastDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING));
            if (cast_dialog)
            {
                cast_dialog->SetGage((wait_timer_.SpentTime() / (wait_second_ * 10.0f)));
            }
        }
    }
}
//------------------------------------------------------------------------------
void uiChaosSystemMan::OnReset()
{
    // 캐릭터 선택시 호출됨 : 초기화 시킬 사항들은 여기서 처리   
    if (ui_chaoszone_battle_)
    {
        ui_chaoszone_battle_->OnReset();
    }

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    if (ui_chaoszone_goldrush_)
    {
        ui_chaoszone_goldrush_->OnReset();
    }

    // 카오즈 시스템 데이터 초기화
    ChaosSystem::GetChaosZoneData().Clear();
    // 골드러시 데이터 초기화
    ChaosSystem::GetGoldRushData().Clear();
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    // 전장 데이터 초기화
    ChaosSystem::GetBattleGroundData().Clear();
}

//------------------------------------------------------------------------------
void uiChaosSystemMan::NetworkProc(MSG_BASE* recv_packet)
{
}

//------------------------------------------------------------------------------
void uiChaosSystemMan::NetworkProcWorld(MSG_BASE* recv_packet)
{
}

//------------------------------------------------------------------------------
void uiChaosSystemMan::InitPKBooleans()
{
    m_vPKBool.clear();
    for (int index = ePKBOOL_BATTLEGROUND_BASE; index < ePKBOOL_BATTLEGROUND_MAX; ++index)
    {
        uicPacketControl packet_control;
        m_vPKBool.push_back(packet_control);
    }
}

//------------------------------------------------------------------------------
void uiChaosSystemMan::OpenChaosZone(DWORD tab_index, bool value)
{
    if (tab_index < 0 || tab_index >= chaos_system_windows_size_)
    {
        assert(!"ChaosSystemMan.cpp::OpenChaosZone() : Unvalid tab Index (유효하지 않은 탭 인덱스입니다.)");
        return;
    }
    ShowChaosZone(chaos_system_windows[tab_index], true);
}

//------------------------------------------------------------------------------
void uiChaosSystemMan::ShowChaosZone(DWORD chaos_window_id, bool value)
{
    uiBase *ui_chaos_window_ptr = NULL;
    ui_chaos_window_ptr = GET_CAST_DIALOG(uiBase, chaos_window_id);

    if (ui_chaos_window_ptr == NULL)
    {
        return;
    }

    if (value == true)
    {
        last_window_id_ = chaos_window_id;
    }

    int i = 0;
    while (chaos_system_windows[i] != IM_CHAOS_SYSTEM_MANAGER::UI_END)//일단 관련창을 모두 닫고
    {
        ui_chaos_window_ptr = GET_CAST_DIALOG(uiBase, chaos_system_windows[i]);
        if (ui_chaos_window_ptr && ui_chaos_window_ptr->IsVisibleWindow())//chaos_system_windows[i] == 0 이면 NULL
        {
            ui_chaos_window_ptr->ShowInterface(FALSE);
            if (ui_chaos_window_ptr->GetDialogWZ())
            {
                window_position_.left = static_cast<LONG>(ui_chaos_window_ptr->GetDialogWZ()->GetDlgX());
                window_position_.top = static_cast<LONG>(ui_chaos_window_ptr->GetDialogWZ()->GetDlgY());
                window_position_.right = 0;//초기화 확인용
            }
        }
        ++i;
    }

    ui_chaos_window_ptr = GET_CAST_DIALOG(uiBase, chaos_window_id); //위에서 ui_chaos_window_ptr값이 변경되기 때문에 다시 받아야 한다.
    ui_chaos_window_ptr->ShowInterface(value?TRUE:FALSE);
    if (value == FALSE) // 닫을때 위치저장
    {
        if (ui_chaos_window_ptr->GetDialogWZ())
        {
            window_position_.left = static_cast<LONG>(ui_chaos_window_ptr->GetDialogWZ()->GetDlgX());
            window_position_.top = static_cast<LONG>(ui_chaos_window_ptr->GetDialogWZ()->GetDlgY());
            window_position_.right = 0;//초기화 확인용
        }
    }
    else if (window_position_.right == 0)// 열때 저장된 위치로 열기
    {
        ui_chaos_window_ptr->MoveWindowWZ(window_position_.left, window_position_.top);
    }
}

void uiChaosSystemMan::WaitGague(bool is_show, int second /* = 5 */, bool is_ban /* = false */)
{
    CastDialog* cast_dialog = static_cast<CastDialog*>(g_InterfaceManager.GetDialog(InterfaceManager::DIALOG_CASTING));
    if (is_show)
    {
        wait_timer_.On(second);
        wait_second_ = second;
        is_ban_cast_ = is_ban; // 추방 캐스팅인지

        if (cast_dialog)
        {
            g_InterfaceManager.HideAllWindows();
            g_InterfaceManager.ShowGameInterface(TRUE);

            // 5116	잠시만 기다려 주십시오.
            int string_code = 5116;

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
            if (is_ban_cast_ == true)
            {
                // 2781 전장에서 추방 중 입니다.
                string_code = 2781;
            }
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM

            TCHAR message[INTERFACE_STRING_LENGTH] = {0,};
            g_InterfaceManager.GetInterfaceString(string_code, message);
            cast_dialog->SetText(message);
            cast_dialog->ShowWindow(TRUE);
        }
        ChaosSystem::GetChaosZoneData().set_is_wait(true);

        g_InterfaceManager.LockDlgsAll();
    }
    else
    {
        wait_timer_.Off();
        if (cast_dialog)
        {
            cast_dialog->ShowWindow(FALSE);
            cast_dialog->SetText("");

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
            // 추방 당하는 캐스팅이 끝날 때 마을로 가기 시전
            if (is_ban_cast_ == true)
            {
                if (GlobalFunc::IsBattleGroundScene() == TRUE
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                    || GlobalFunc::IsGoldRushScene() == TRUE
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
                    )
                {
                    BattleScene::BackToTheVillageSyn();
                }
            }
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
        }
    }

}

void uiChaosSystemMan::Unlocks()
{
    ChaosSystem::GetChaosZoneData().set_is_wait(false);
    g_InterfaceManager.UnLockDlgs();
}

void uiChaosSystemMan::HotKeyToggleChaoszoneDialog()
{
    if ((g_pMap) &&
        (g_pMap->IsVill() == FALSE) && 
        (g_pMap->IsField() == FALSE))
    {
        return;
    }

    uiChaosSystemMan* chaos_system_manager = GET_DIALOG_MANAGER(uiChaosSystemMan, UIMAN_CHAOS);
    if (chaos_system_manager != NULL)
    {
        chaos_system_manager->ToggleChaoszoneDialog();
    }
    
}
//------------------------------------------------------------------------------
void uiChaosSystemMan::ToggleChaoszoneDialog(bool is_force_close)
{
    uiBase *ui_chaos_window_ptr = GET_CAST_DIALOG(uiBase, last_window_id_);
    if (ui_chaos_window_ptr)
    {
        if ((ui_chaos_window_ptr->IsVisible()) || (is_force_close == true))
        {
            ShowChaosZone(last_window_id_, false);
            
            g_KeyQueueManager.DeleteMsg(eDoType_ChaoszoneDialog);
        }
        else
        {
            ShowChaosZone(last_window_id_, true);
            
            keyMsg msg;
            ZeroMemory(&msg,sizeof(msg));
            msg.dwType		= eDoType_ChaoszoneDialog;
            msg.DoSomething	= GlobalFunc::DoChaoszoneDialog;
            g_KeyQueueManager.PushBack(msg);
        }
    }
}

#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
DWORD uiChaosSystemMan::get_pending_dialog_id()
{    
    for (unsigned int index = 0; index < chaos_system_windows_size_; ++index)
    {
        uiBase *ui_chaos_window_ptr = GET_CAST_DIALOG(uiBase, chaos_system_windows[index]);
        if (ui_chaos_window_ptr)
        {
            if (ui_chaos_window_ptr->IsVisible())
            {
                // 현재 보여지고 있는 dialog이다.
                return chaos_system_windows[index];
            }
        }
    }

    // 아무것도 열려있지 않다.
    return 0;
}
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND