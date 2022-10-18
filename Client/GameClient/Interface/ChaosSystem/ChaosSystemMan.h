#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include "uiBaseMan.h"

class uiChaosZoneBattle;
class uiChaosZoneRankBattle;
class uiChaosZoneSSQ;
class uiChaosZoneArena;
class uiChaosZonePVP;
class uiChaosZoneHistory;
class uiChaosZoneInfo;
class uiChaosZoneResurrection;
class uiChaosZonePassword;
class uiChaosZoneResult;
class uiChaosZoneTimer;
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
class uiChaosZoneVote;
class uiChaosZoneVoteResult;
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
class uiChaosZoneGoldRush;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

struct IM_CHAOS_SYSTEM_MANAGER
{
    static const DWORD CHAOSZONE_BATTLE;
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    static const DWORD CHAOSZONE_GOLDRUSH;
#else
    static const DWORD CHAOSZONE_RANK_BATTLE;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    static const DWORD CHAOSZONE_ARENA;
    static const DWORD CHAOSZONE_SSQ;
    static const DWORD CHAOSZONE_PVP;   
    static const DWORD CHAOSZONE_HISTORY;
    static const DWORD CHAOSZONE_INFO;
    static const DWORD UI_END;  //여기까지 메인ui 탭 개수

    static const DWORD	BATTLE_GROUND_RESURRECTION_POPUP;
    static const DWORD	CHAOSZONE_PASSWORD;
    static const DWORD	CHAOSZONE_RESULT;
    static const DWORD	CHAOSZONE_TIMER;

#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    static const DWORD	CHAOSZONE_VOTE;
    static const DWORD	CHAOSZONE_VOTE_RESULT;
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    static const DWORD CHAOSZONE_WIDE_INFO;
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
};

class uiChaosSystemMan : public uiBaseMan
{
public:
    enum ePacketPending
    {
        ePKBOOL_BATTLEGROUND_BASE = 0,
        ePKBOOL_BATTLEGROUND_RESERVATION_SYN,
        ePKBOOL_BATTLEGROUND_RESERVATION_CANCLE_SYN,
        ePKBOOL_BATTLEGROUND_GET_OPEN_TIME_SYN,
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        ePKBOOL_GOLDRUSH_BASE,
        ePKBOOL_GOLDRUSH_RESERVATION_SYN,
        ePKBOOL_GOLDRUSH_RESERVATION_CANCLE_SYN,
        ePKBOOL_GOLDRUSH_GET_OPEN_TIME_SYN, // 이게 왜 있지?
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH
        ePKBOOL_BATTLEGROUND_MAX,
    };    

public:
    uiChaosSystemMan(InterfaceManager *ui_manager_ptr);
    virtual ~uiChaosSystemMan();
    virtual void OnInitialize();
    virtual void OnRelease();
    virtual void Process(DWORD tick_count);
    virtual void OnReset();
    virtual void NetworkProc(MSG_BASE* recv_packet);
    virtual void NetworkProcWorld(MSG_BASE* recv_packet);
    virtual void InitPKBooleans();
    void ShowChaosZone(DWORD chaos_window_id, bool is_show);
    void OpenChaosZone(DWORD tab_index, bool value);

    static void HotKeyToggleChaoszoneDialog();
    void ToggleChaoszoneDialog(bool is_force_close = false);

    void WaitGague(bool is_show, int second = 5, bool is_ban = false);
    void Unlocks();
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    DWORD get_pending_dialog_id();    
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

private:
    uiChaosZoneBattle* ui_chaoszone_battle_;
    uiChaosZoneRankBattle* ui_chaoszone_rank_battle_;
    uiChaosZoneSSQ* ui_chaoszone_ssq_;
    uiChaosZoneArena* ui_chaoszone_arena_;
    uiChaosZonePVP* ui_chaoszone_pvp_;
    uiChaosZoneHistory* ui_chaoszone_history_;
    uiChaosZoneInfo* ui_chaoszone_info_;
    uiChaosZoneResurrection* ui_chaoszone_resurrection_;
    uiChaosZonePassword* ui_chaoszone_password_;
    uiChaosZoneResult* ui_chaoszone_result_;
    uiChaosZoneTimer* ui_chaoszone_timer_;
#ifdef _NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
    uiChaosZoneVote* ui_chaoszone_vote_;
    uiChaosZoneVoteResult* ui_chaoszone_vote_result_;
#endif //_NA_006800_20130703_BATTLE_GROUND_VOTE_SYSTEM
#ifdef _NA_007114_20140313_CHAOSZONE_GOLDRUSH
    uiChaosZoneGoldRush* ui_chaoszone_goldrush_;    
#endif // _NA_007114_20140313_CHAOSZONE_GOLDRUSH

    RECT window_position_;
    DWORD chaos_system_windows_size_;
    DWORD last_window_id_;

    CTimerSimple wait_timer_; // 로딩 전 캐스팅 타이머
    int wait_second_;
    bool is_ban_cast_; // 로딩 전 캐스팅이 추방 캐스팅인지 (기본은 입장)
};
#endif//_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND