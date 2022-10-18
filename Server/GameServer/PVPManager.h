#pragma once
#ifndef GAMESERVER_PVP_MANAGER_H
#define GAMESERVER_PVP_MANAGER_H

#include "./PVPCommon.h"
#include "./PVPInfo.h"
#include "Player.h"

//==================================================================================================
//struct PVP_PLAYER_INFO <- moved to PVPCommon.h
//enum ePVP_ZONE_STATE <- moved to PVPCommon.h
//typedef util::SolarHashTable<PVP_PLAYER_INFO*> PVP_PLAYER_INFO_HASH;

class GamePVPRoom; // _NA_0_20091118_GAMEROOM_BATTLE_RESULT

//==================================================================================================

class PVPManager
{
public:
    static const DWORD kDefaultTimerForUpdater = 1 * 1000;
    static const DWORD kWaitIntervalUntilKeyPressAfterEntered;
    PVPManager(void);
    ~PVPManager(void);

    //PVP���� �ʱ�ȭ
    void Init(
        ADDITIONAL_PVPINFO* additional_pvp_info, 
        GamePVPRoom* pvp_room
        );

    //PvPInfoContainer�� �÷��̾� ���� �߰�
    void AddPVPInfo(
        DWORD player_key, 
        BYTE team_no
        );

    //PlayerEnterSync�� ���� ����
    void EnterPlayer(
        Player* player
        );

    //�����µ� ���¿� ���� Draw����, Die���� ����
    void LeavePlayer(
        Player* player
        );

    //ó�� ���ͼ� Ű�� ������ ��
    RC::ePVP_RESULT PressKeyPlayer(
        Player* key_pressed_player
        );

    //�װų�, ���� ����, PVP���� ��� �� ������Ʈ 
    void OnPVPResult(
        Character* killer, 
        Player* died_player
        );

    //PVPPlayerInfo�� �� ���� ���� PVP ���� ����
    void StartPVP();

    void Update(
        DWORD delta_tick
        );

    void Release();


private:

    bool IsOpponentDieAll();

    bool CanChangeClearForActionMode();
    
    bool IsContinuePVP();
    
    bool IsEndPVP();

    bool IsDrawCheck(
        Player* player
        );
    
    //������� ���۽�
    void OnStartMatchlessMode(
        Player* player
        );

    //PVP��� ���۽�
    void OnStartMatchMode(
        Player* player
        );

    //���� PVP��� ���۽� Brd
    void OnStartMatchModeBRD(
        Player* player
        );

    //��Ȱ ���۽�
    void OnRevival(
        Player* player
        );

    void SendToAll(
        MSG_BASE_FORWARD* pMsg, 
        WORD size, 
        DWORD except_object_key = 0
        );

    void UpdateWaitPressKeyTimeoutEvent();

    void UpdateEachNodeUpdateEvent();

    void ChangeToWaitCloseMode();

    void StartClearForActionModeAll();

    void StartGoIntoBattleModeAll();

private:
    enum eInterMode {
        eInterMode_None = 0,
        eInterMode_WaitKeyPressEvent,
        eInterMode_ClearForAction,
        eInterMode_ClearForActionBusyCheck,
        eInterMode_GoIntoBattle,
        eInterMode_WaitClose,
        eInterMode_Counts
    };

    GamePVPRoom* pvp_room_;
    eInterMode inter_mode_;
    ADDITIONAL_PVPINFO* additional_pvp_info_;
    ITimerBase update_timer_;
    ITimerBase mode_timer_;

    PvPInfoContainer pvp_player_infos_;
    class OperatorUpdate;
    friend class OperatorUpdate;
};

//==================================================================================================

inline void PVPManager::AddPVPInfo(DWORD player_key, BYTE team_no)
{
    pvp_player_infos_.AddPvPControl(player_key, team_no);
}

inline void PVPManager::ChangeToWaitCloseMode()
{
    inter_mode_ = this->eInterMode_WaitClose;
}

#endif //GAMESERVER_PVP_MANAGER_H

