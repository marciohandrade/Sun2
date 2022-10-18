#include "stdafx.h"
#include ".\PVPManager.h"

#include <PacketStruct_AG.h>

#include ".\Player.h"
#include ".\PVPInfo.h"
// _NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "PVPBattleResult.h"
#include "GamePVPRoom.h"

//==================================================================================================
// max wait interval until key press event after enter signal
const DWORD PVPManager::kWaitIntervalUntilKeyPressAfterEntered = 2 * 60 * 1000;

PVPManager::PVPManager(void)
    : pvp_room_(NULL)
    , inter_mode_(eInterMode_None)
    , additional_pvp_info_(NULL)
{
    pvp_player_infos_.Clear();
}

PVPManager::~PVPManager(void)
{
}

void 
PVPManager::Release()
{
    Init(NULL, NULL);
}

void 
PVPManager::Init(ADDITIONAL_PVPINFO* additional_pvp_info, GamePVPRoom* pvp_room)
{
    pvp_room_ = pvp_room;
    additional_pvp_info_ = additional_pvp_info;
    inter_mode_ = this->eInterMode_None;
    pvp_player_infos_.Clear();
    update_timer_.Stop();
}

void 
PVPManager::StartPVP()
{
    update_timer_.SetTimer(kDefaultTimerForUpdater);//1 * 1000
    inter_mode_ = eInterMode_WaitKeyPressEvent;
    mode_timer_.SetTimer(kWaitIntervalUntilKeyPressAfterEntered, true, false);

    DISPMSG("PVP 존 시작\n");

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    GameBattleResult* pResult = pvp_room_->GetGameBattleResult();
    if (pResult)
    {
        pResult->Clear();
    }
}

bool 
PVPManager::IsOpponentDieAll()
{
    const int number_of_members = pvp_player_infos_.number_of_members_;
    for (int index = 0; index < number_of_members; ++index)
    {
        const PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
        if (pvp_node->entered_ == false || pvp_node->closed_) 
        {
            continue;
        }
        
        assert(pvp_node->player_ != NULL);
        const PVPInfo& pvp_info = pvp_node->player_->GetPVPInfo();
        if (pvp_info.GetPVPState() != ePVP_USER_DIE)
        {
            return false;
        }
    }
    return true;
}

bool 
PVPManager::IsDrawCheck(Player* player)
{
    const PVPInfo& pvp_info = player->GetPVPInfo();
    if (pvp_info.GetPVPState() == ePVP_USER_ENTERED) {
        return true;
    }
    return false;
}

void 
PVPManager::EnterPlayer(Player* player)
{
    if (inter_mode_ == this->eInterMode_WaitClose) 
    {
        return;
    }
    const DWORD player_key = player->GetObjectKey();
    const int find_index = pvp_player_infos_.FindPvPControlIndex(player_key);
    if (find_index == pvp_player_infos_.kNotFoundIndex)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Can't find reserved pvp member : room key : %d, user key : %d",
            pvp_room_->GetKey(), 
            player->GetUserKey()
            );
        return;
    }

    PvPInfoContainer::Node* const pvp_node = &pvp_player_infos_.nodes_[find_index];
    pvp_node->ChangeToEntered(player);

    PVPInfo& pvp_info = player->GetPVPInfo();
    pvp_info.Clear();
    pvp_info.SetPVPState(ePVP_USER_ENTERED);
    pvp_info.SetTeam(pvp_node->team_no_);

    MSG_CG_PVP_INFO_CMD msg_info_cmd;
    pvp_player_infos_.MakeMessage(&msg_info_cmd, pvp_node);
    player->SendPacket(&msg_info_cmd, msg_info_cmd.GetSize());

    MSG_CG_PVP_INFO_BRD bmsg;
    bmsg.m_dwPlayerKey = player_key;
    bmsg.m_Team = pvp_node->team_no_;
    SendToAll(&bmsg, sizeof(bmsg), player_key);

    DISPMSG("PVP플레이어들어옴\n");
}

void 
PVPManager::LeavePlayer(Player* player)
{
    const DWORD player_key = player->GetObjectKey();
    const int find_index = pvp_player_infos_.FindPvPControlIndex(player_key);
    if (find_index == pvp_player_infos_.kNotFoundIndex)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Can't find pvp member : room key : %d, user key : %d",
               pvp_room_->GetKey(), player->GetUserKey());
        return;
    }
    PvPInfoContainer::Node* const pvp_node = &pvp_player_infos_.nodes_[find_index];
    pvp_node->ChangeToClosed();

    PVPInfo& pvp_info = player->GetPVPInfo();

    //상대 팀 혹은 다른 사람의 상태에 따라 Draw인지, Die인지 판정
    if (pvp_info.GetPVPState() == ePVP_USER_MATCH)
    {
        if (!IsOpponentDieAll()) 
        {
            pvp_info.IncDie();
        }
    }
    else if (IsDrawCheck(player))
    {
        pvp_info.IncDraw();
    }

    //포인트 계산
    pvp_info.CalcPoint();
    pvp_info.SetPVPState(ePVP_USER_NONE);

    DISPMSG("PVP플레이어나감\n");
}

RC::ePVP_RESULT 
PVPManager::PressKeyPlayer(Player* key_pressed_player)
{
    if (inter_mode_ == this->eInterMode_WaitClose) 
    {
        return RC::RC_PVP_INVALID_USER_STATE;
    }

    PVPInfo& pvp_info = key_pressed_player->GetPVPInfo();
    if (pvp_info.GetPVPState() != ePVP_USER_ENTERED) 
    {
        return RC::RC_PVP_INVALID_USER_STATE;
    }
    
    pvp_info.SetPVPState(ePVP_USER_KEYPRESSED);

    DISPMSG("PVP플레이어키누름\n");

    return RC::RC_PVP_SUCCESS;
}

bool 
PVPManager::CanChangeClearForActionMode()
{
    const int number_of_members = pvp_player_infos_.number_of_members_;
    for (int index = 0; index < number_of_members; ++index)
    {
        const PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
        //need separated condition check \
        //so that the current state is processed on a detached update.
        //if the entered_ is equal to false, will be expelled call by UpdateWaitPressKeyTimeoutEvent
        if (pvp_node->closed_) 
        {
            continue;
        }

        if (pvp_node->entered_ == false) 
        {
            return false;
        }

        assert(pvp_node->player_ != NULL);
        const PVPInfo& pvp_info = pvp_node->player_->GetPVPInfo();
        if (pvp_info.GetPVPState() != ePVP_USER_KEYPRESSED) 
        {
            return false;
        }
    }
    return true;
}

void 
PVPManager::UpdateWaitPressKeyTimeoutEvent()
{
    MSG_CG_PVP_END_BRD send_msg;

    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd = \
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.Init(&send_msg, sizeof(send_msg));
    uint16_t* const user_begin = msg_brd.GetUsersStartPtr();

    const int number_of_members = pvp_player_infos_.number_of_members_;
    for (int index = 0; index < number_of_members; ++index)
    {
        PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
        if (pvp_node->entered_ == false || pvp_node->closed_)
        {
            pvp_node->ChangeToClosed();
            continue;
        }
        assert(pvp_node->player_ != NULL);
        const PVPInfo& pvp_info = pvp_node->player_->GetPVPInfo();
        if (pvp_info.GetPVPState() == ePVP_USER_KEYPRESSED) 
        {
            continue;
        };
        DWORD check_bound = msg_brd.AddUser(user_begin, pvp_node->player_key_);
        __TOUCHED(check_bound);
    }

    if (msg_brd.number_of_users) 
    {
        g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    }
}

void 
PVPManager::UpdateEachNodeUpdateEvent()
{
    const int number_of_members = pvp_player_infos_.number_of_members_;
    for (int index = 0; index < number_of_members; ++index)
    {
        PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
        if (pvp_node->closed_ != false) 
        {
            continue;
        }
        assert(pvp_node->player_ != NULL);
        Player* player = pvp_node->player_;
        PVPInfo& pvp_info = player->GetPVPInfo();
        if (pvp_info.IsExpired())
        {
            ePVP_USER_STATE pvp_user_state = pvp_info.GetPVPState();
            switch (pvp_user_state)
            {
            case ePVP_USER_MATCHLESS: //case of change 'clear for action' to 'go in battle'
                OnStartMatchMode(player);
                OnStartMatchModeBRD(player);
                break;
            case ePVP_USER_DIE:
                OnRevival(player);  //resurrection
                OnStartMatchlessMode(player); //start 'clear for action'
                break;
            default:
                assert(!"Unexpected pvp state");
                break;
            }
        }
    }
}

void 
PVPManager::Update(DWORD delta_tick)
{
    if (inter_mode_ == this->eInterMode_WaitClose) {
        return;
    }
    
    bool need_update = update_timer_.IsExpired() != false;
    if (need_update == false) {
        return;
    }
    
    switch (inter_mode_)
    {
    case this->eInterMode_WaitKeyPressEvent:
        {
            bool need_to_change = mode_timer_.IsExpired() != false; // wait timeout in a room
            if (need_to_change) {
                UpdateWaitPressKeyTimeoutEvent();
                return;
            }
            need_to_change = CanChangeClearForActionMode();
            if (need_to_change) {
                StartClearForActionModeAll();
            }
        }
        break;
    case this->eInterMode_ClearForAction:
        {
            bool need_to_change = mode_timer_.IsExpired() != false;
            if (need_to_change) {
                inter_mode_ = this->eInterMode_ClearForActionBusyCheck;
                // no timer // mode_timer_.Configure(mode_timer_.LOOP, ...);
            }
            if (need_to_change == false) {
                break;
            }
        }
        // (NOTE) no break, same as eInterMode_ClearForActionBusyCheck, except timer
    case this->eInterMode_ClearForActionBusyCheck:
        if (IsContinuePVP()) {
            StartGoIntoBattleModeAll();
        }
        else {
            // no control, custom logic has active event \
            // which is automatic leave process by the client 
            ChangeToWaitCloseMode();
        }
        break;
    case this->eInterMode_GoIntoBattle:
        // no control, change to each node update logic
        UpdateEachNodeUpdateEvent();
        break;
    case this->eInterMode_WaitClose:
        // no control, custom logic has active event \
        // which is automatic leave process by the client 
        break;
    }
}

// 처음 호출됨
void 
PVPManager::StartClearForActionModeAll()
{
    inter_mode_ = this->eInterMode_ClearForAction;
    mode_timer_.SetTimer(PVP_MATCHLESSS_DELAY_TIME, true, false);

    const int number_of_members = pvp_player_infos_.number_of_members_;
    for (int index = 0; index < number_of_members; ++index)
    {
        const PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
        if (pvp_node->entered_ == false || pvp_node->closed_) {
            continue;
        }
        assert(pvp_node->player_ != NULL);
        PVPInfo& pvp_info = pvp_node->player_->GetPVPInfo();
        pvp_info.StopTimer();
        pvp_info.SetPVPState(ePVP_USER_MATCHLESS);
        DISPMSG("PVP[%u] : MSG_CG_PVP_MATCHLESS_MODE_START_CMD\n", pvp_node->player_key_);
    }
    MSG_CG_PVP_MATCHLESS_MODE_START_CMD msg_cmd;
    SendToAll(&msg_cmd, sizeof(msg_cmd), 0);
}

// 처음 호출됨
void 
PVPManager::StartGoIntoBattleModeAll()
{
    inter_mode_ = this->eInterMode_GoIntoBattle;
    mode_timer_.Stop();

    const int number_of_members = pvp_player_infos_.number_of_members_;
    for (int index = 0; index < number_of_members; ++index)
    {
        const PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
        if (pvp_node->entered_ == false || pvp_node->closed_) {
            continue;
        }
        assert(pvp_node->player_ != NULL);
        PVPInfo& pvp_info = pvp_node->player_->GetPVPInfo();
        pvp_info.StopTimer();
        pvp_info.SetPVPState(ePVP_USER_MATCH);
        DISPMSG("PVP[%u] : MSG_CG_PVP_MATCH_MODE_FIRST_START_CMD\n", pvp_node->player_->GetObjectKey());
        GAMELOG->WritePVPStart(pvp_node->player_);
    }
    MSG_CG_PVP_MATCH_MODE_FIRST_START_CMD msg_cmd;
    SendToAll(&msg_cmd, sizeof(msg_cmd), 0);
}

// 개인별 호출 메서드
void 
PVPManager::OnStartMatchlessMode(Player* player)
{
    PVPInfo& pvp_info = player->GetPVPInfo();
    pvp_info.StartTimer(PVP_MATCHLESSS_DELAY_TIME);
    pvp_info.SetPVPState(ePVP_USER_MATCHLESS);

    MSG_CG_PVP_MATCHLESS_MODE_START_CMD msg;
    player->SendPacket(&msg, sizeof(msg));
    DISPMSG("PVP[%u] : MSG_CG_PVP_MATCHLESS_MODE_START_CMD\n", player->GetObjectKey());
}

// 개인별 호출 메서드
void 
PVPManager::OnStartMatchMode(Player* player)
{
    PVPInfo& pvp_info = player->GetPVPInfo();
    pvp_info.StopTimer();
    pvp_info.SetPVPState(ePVP_USER_MATCH);

    MSG_CG_PVP_MATCH_MODE_START_CMD msg;
    player->SendPacket(&msg, sizeof(msg));
    DISPMSG("PVP[%u] : MSG_CG_PVP_MATCH_MODE_START_CMD\n", player->GetObjectKey());
}

void 
PVPManager::OnStartMatchModeBRD(Player* except_player)
{
    MSG_CG_PVP_MATCH_MODE_START_BRD msg;
    msg.m_dwPlayerKey = except_player->GetObjectKey();
    SendToAll(&msg, sizeof(msg), except_player->GetObjectKey());
    DISPMSG("MSG_CG_PVP_MATCH_MODE_START_BRD\n");
}

void 
PVPManager::OnRevival(Player* player)
{
    if (player->IsDead()) {
        player->OnResurrection(0, 1.f, 1.f);
    }
}


// (NOTE) old comment '@@@ OPT 최적화', no means
// this function must be called in a 'clear for action' state
bool 
PVPManager::IsContinuePVP()
{
    int team1 = 0, team2 = 0;
    const int number_of_members = pvp_player_infos_.number_of_members_;
    for (int index = 0; index < number_of_members; ++index)
    {
        const PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
        if (pvp_node->entered_ == false || pvp_node->closed_) {
            continue;
        }
        assert(pvp_node->player_ != NULL);
        pvp_node->team_no_ == 1 ? ++team1 : ++team2;
    }
    if (additional_pvp_info_->m_Mode == ePVP_TEAM_MODE)
    {   // determine end condition whether any team is empty
        if (team1 == 0 || team2 == 0) {
            return false;
        }
    }
    else
    {   // over than 2, use field 'team2'
        if (team2 <= 1) {
            return false;
        }
    }
    return true;
}

bool 
PVPManager::IsEndPVP()
{
    int max_kill = 0;
    // 개인전
    switch (additional_pvp_info_->m_Rule)
    {
#if !defined(_GLOBAL) || !defined(_RUSSIA)
    case ePVP_RULE_10: max_kill = 10; break;
    case ePVP_RULE_30: max_kill = 30; break;
#else
    case ePVP_RULE_10: max_kill = 1; break;
    case ePVP_RULE_30: max_kill = 10; break;
#endif
    case ePVP_RULE_INFINITY: 
        return false;
    }

    if (additional_pvp_info_->m_Mode == ePVP_TEAM_MODE)
    {
        int sum_of_team_score1 = 0, sum_of_team_score2 = 0;
        const int number_of_members = pvp_player_infos_.number_of_members_;
        for (int index = 0; index < number_of_members; ++index)
        {
            const PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
            if (pvp_node->entered_ == false || pvp_node->closed_) {
                continue;
            }
            assert(pvp_node->player_ != NULL);
            const PVPInfo& pvp_info = pvp_node->player_->GetPVPInfo();
            int number_of_kills = pvp_info.GetTotalKillNum();
            if (pvp_node->team_no_ == 1) {
                sum_of_team_score1 += number_of_kills;
            }
            else {
                sum_of_team_score2 += number_of_kills;
            }

            if (max_kill <= sum_of_team_score1 || max_kill <= sum_of_team_score2) {
                return true;
            }
        }
        DISPMSG("Max:%u,Cur1:%u,Cur2:%u\n", max_kill, sum_of_team_score1, sum_of_team_score2);
    }
    else
    {
        const int number_of_members = pvp_player_infos_.number_of_members_;
        for (int index = 0; index < number_of_members; ++index)
        {
            const PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
            if (pvp_node->entered_ == false || pvp_node->closed_) {
                continue;
            }
            assert(pvp_node->player_ != NULL);
            const PVPInfo& pvp_info = pvp_node->player_->GetPVPInfo();
            int number_of_kills = pvp_info.GetTotalKillNum();
            if (max_kill <= number_of_kills) {
                return true;
            }
            else {
                DISPMSG("Max:%u,Cur:%u\n", max_kill, number_of_kills);
            }
        }
    }
    return false;
}

// TODO(old comment)
// 1. 경험치 안 주게 처리
// 2. 부활시 경험치 안 깍이게
void 
PVPManager::OnPVPResult(Character* killer_character, Player* died_player)
{
    // 킬러를 찾는다.
    Player* killer = NULL;
    if (killer_character && (killer = killer_character->GetPlayerOwner()) != NULL) {
        killer = pvp_player_infos_.FindPvPPlayer(killer->GetObjectKey());
    }
    if (inter_mode_ == this->eInterMode_WaitClose) { 
        return OnRevival(died_player);
    }

    if (killer)
    {
        // 킬수 증가
        PVPInfo& killer_pvp_info = killer->GetPVPInfo();
        killer_pvp_info.IncKill();
        DWORD score = killer_pvp_info.CalcScore(killer->GetLevel(), died_player->GetLevel());
        //
        MSG_CG_PVP_SCORE_CMD msg_score;
        msg_score.m_Score = score;
        killer->SendPacket(&msg_score, sizeof(msg_score));
        // 나포함 + 주변 broad
        MSG_CG_PVP_RESULT_BRD msg_result;
        msg_result.m_KillerKey = killer->GetObjectKey();
        msg_result.m_DierKey = died_player->GetObjectKey();
        SendToAll(&msg_result, sizeof(msg_result));
    }

    // die수 증가
    PVPInfo& died_pvp_info = died_player->GetPVPInfo();
    died_pvp_info.IncDie();
    died_pvp_info.SetPVPState(ePVP_USER_DIE);
    died_pvp_info.StartTimer(PVP_REVIVAL_DELAY_TIME);

    // PVP 종료 여부 체크
    if (IsEndPVP())
    {
        ChangeToWaitCloseMode();

        MSG_CG_PVP_END_BRD msg_end;
        SendToAll(&msg_end, sizeof(msg_end));
        DISPMSG("END_PVP\n");
    }

    //PVP 결과 로그
    if (killer)
    {
        GAMELOG->WritePVPKillUser(killer, died_player, true);
        GAMELOG->WritePVPKillUser(died_player, killer, false);
    }
}

void 
PVPManager::SendToAll(MSG_BASE_FORWARD* send_msg , WORD size, DWORD except_object_key)
{
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd = \
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.Init(send_msg, size);
    uint16_t* const user_begin = msg_brd.GetUsersStartPtr();
    // no bound check, because of a limits of the pvp room is MAX_PARTYNUM

    const bool skip_check_key = except_object_key == 0;
    const int number_of_members = pvp_player_infos_.number_of_members_;
    for (int index = 0; index < number_of_members; ++index)
    {
        const PvPInfoContainer::Node* pvp_node = &pvp_player_infos_.nodes_[index];
        if (pvp_node->entered_ == false || pvp_node->closed_) {
            continue;
        }
        assert(pvp_node->player_ != NULL);
        if ((skip_check_key || except_object_key != pvp_node->player_key_) == false) {
            continue;
        }
        DWORD check_bound = msg_brd.AddUser(user_begin, pvp_node->player_key_);
        __TOUCHED(check_bound);
    }
    if (msg_brd.number_of_users) {
        g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    }
}

//==================================================================================================

static void PvPRuleCompileTimeChecker()
{   // UNUSED, only test function
    MSG_AG_ZONE_PVP_INFO_CMD msg_cmd;
    PvPInfoContainer pvp_player_info = { 0, };
    BOOST_STATIC_ASSERT(_countof(msg_cmd.m_Info) == _countof(pvp_player_info.nodes_) &&
                        _countof(msg_cmd.m_Info) == pvp_player_info.kNotFoundIndex &&
                        pvp_player_info.kNotFoundIndex == MAX_PARTYMEMBER_NUM);
};

bool 
PvPInfoContainer::AddPvPControl(DWORD player_key, BYTE team_no)
{
    int find_index = FindPvPControlIndex(player_key);
    if (find_index != kNotFoundIndex) 
    {
        return false;
    }

    Node* node = &nodes_[number_of_members_];
    node->player_key_ = player_key;
    node->team_no_ = team_no;
    ++number_of_members_;
    return true;
}

// make other player info message for the enter player
bool 
PvPInfoContainer::MakeMessage(MSG_CG_PVP_INFO_CMD* msg_cmd, const Node* enter_player_node)
{
    // make broadcast all member information message with my info
    // 나에게 팀원 정보를 보낸다.
    msg_cmd->m_Team = enter_player_node->team_no_;
    msg_cmd->m_Count = 0;

    BOOST_STATIC_ASSERT(_countof(nodes_) == _countof(msg_cmd->m_Info));
    for (int index = 0, result_index = 0; index < number_of_members_; ++index)
    {
        const Node* pvp_node = &nodes_[index];
        if (pvp_node->entered_ == false || pvp_node->closed_) {
            continue;
        }
        assert(pvp_node->player_ != NULL);
        MSG_CG_PVP_INFO_CMD::PVP_TEAM_INFO* team_info = &msg_cmd->m_Info[msg_cmd->m_Count];
        team_info->m_dwPlayerKey = pvp_node->player_key_;
        team_info->m_Team = pvp_node->team_no_;
        ++msg_cmd->m_Count;
    }
    return true;
};
