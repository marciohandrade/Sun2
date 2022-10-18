#include "StdAfx.h"
#include ".\GameBattleResult.h"

// _NA_0_20091118_GAMEROOM_BATTLE_RESULT

#include ".\Player.h"

GameBattleResult::GameBattleResult(void) :
enable_pvp_(false)
{
    battle_player_pool_.Initialize(20, 20, "BattlePlayer", AssertionLogger4Pool_Server);
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    total_droped_heim_ = 0;
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
}

GameBattleResult::~GameBattleResult(void)
{
    Release();
    battle_player_pool_.Release();
}

bool GameBattleResult::Init(bool enable_pvp)
{
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    total_droped_heim_ = 0;
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
    Release();
    enable_pvp_ = enable_pvp;
    return true;
}

VOID GameBattleResult::Release()
{
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        battle_player_pool_.Free(iter->second);
    }

    battle_player_hash_.clear();
}

void GameBattleResult::Clear()
{
    GameBattleResult::BattlePlayer* battle_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; )
    {
        battle_player = iter->second;
        Player* battle_find_player = PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
        if (battle_find_player == NULL)
        {
            battle_player_pool_.Free(iter->second);
            battle_player_hash_.erase(iter++);
        }
        else
        {
            LEVELTYPE level = battle_find_player->GetLevel();
            battle_player->battle_recorder.ClearRecord();
            battle_player->battle_recorder.DoRecord(BATTLE_RECORD_LEVEL, level);
            update_selector_.ResetUpdateKey(battle_player->update_key);
            ++iter;
        }
    }

    update_selector_.ResetUpdateSelector();
}

// 유저의 출입및 업데이트 정보를 보냄
bool GameBattleResult::SendBattleResult(Player* player, int team_flag)
{
    static MSG_CG_ZONE_PLAYER_BATTLE_RESULT_ACK result_ack;

    GameBattleResult::BattlePlayer* battle_player = FindBattlePlayer(player->GetCharGuid());
    if (battle_player == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "Character GUID[%d] is Not Exist", player->GetCharGuid());
        return false;
    }

    //if (battle_player->player != player)
    if (battle_player->char_guid != player->GetCharGuid())
    {
        SUNLOG(eCRITICAL_LOG, "Character GUID[%d] Player Pointer Miss", player->GetCharGuid());
        return false;
    }

    uint user_count = 0;
    CHARGUID guid = 0;
    BattlePlayer* exist_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        if (user_count >= ZONE_UPDATE_BATTLE_PLAYER_INFO::MAX_PLAYER)
            break;

        guid = iter->first;

        exist_player = iter->second;

        Player* exist_find_player = PlayerManager::Instance()->FindPlayerByCharKey(exist_player->char_guid);
        if (exist_find_player)
        {
            // 변경된 전투 정보 설정시 캐릭터 레벨은 여기에서 따로 체크한다.
            LEVELTYPE level = exist_find_player->GetLevel();
            DoRecord(exist_find_player, BATTLE_RECORD_LEVEL, level);
        }

        if (update_selector_.IsUpdated(battle_player->update_key, exist_player->update_key))
            continue;


        const BattleRecorder::Record& record = exist_player->battle_recorder.get_record();

        GAMEROOM_BATTLE_UPDATE_INFO& battle_info = result_ack.m_ZonePlayers.m_Info[user_count];
        battle_info.CharGuid             = guid;
        battle_info.wLevel               = record.level;
        battle_info.dwMaxDamageToEnemy    = record.max_damage_to_enemy;
        battle_info.dwTotalDamageByEnemy = record.total_damage_by_enemy;
        battle_info.dwTotalDamageToEnemy = record.total_damage_to_enemy;
        battle_info.dwTotalHealToParty   = record.total_heal_to_party;
        battle_info.wDeadCount           = record.dead_count;
        battle_info.wKillCount = enable_pvp_ ? record.player_kill_count : record.monster_kill_count;

        ++user_count;
    }

    update_selector_.DoUpdate(battle_player->update_key);

    // 결과값은 무조건 보내도록 한다.
    result_ack.m_ZonePlayers.m_PlayerCount = user_count;
    Player* battle_find_player = PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
    if (battle_find_player != NULL)
    {
        battle_find_player->SendPacket(&result_ack, result_ack.GetSize());
    }

    return true;
}


bool GameBattleResult::Enter(Player* player)
{
    CHARGUID guid = player->GetCharGuid();
    BattlePlayer* battle_player = FindBattlePlayer(guid);
    if (battle_player == NULL)
    {
        // 최초 진입시
        battle_player = (BattlePlayer*)battle_player_pool_.Alloc();
        ASSERT(battle_player);
        if (battle_player == NULL)
        {
            SUNLOG(eCRITICAL_LOG, "GameBattleResult::Enter() Memory Alloc Fail");
            return false;
        }

        battle_player_hash_.insert(BattlePlayerHash::value_type(guid, battle_player));

        //battle_player->player = player;
        battle_player->char_guid = player->GetCharGuid();
        update_team(battle_player);

        //LEVELTYPE level = battle_player->player->GetLevel();
        LEVELTYPE level = player->GetLevel();
        battle_player->battle_recorder.ClearRecord();
        battle_player->battle_recorder.DoRecord(BATTLE_RECORD_LEVEL, level);

        update_selector_.ResetUpdateKey(battle_player->update_key);
    }
    else
    {
        //battle_player->player = player;
        battle_player->char_guid = player->GetCharGuid();
        update_team(battle_player);

        LEVELTYPE level = player->GetLevel();
        battle_player->battle_recorder.DoRecord(BATTLE_RECORD_LEVEL, level);

        update_selector_.ResetUpdateKey(battle_player->update_key);

        update_selector_.DoDirty(battle_player->update_key);
    }

    NotifyEnterMessage(battle_player);

    return true;
}

void GameBattleResult::Leave(Player* player, bool abnormal_disconnect)
{
    CHARGUID guid = player->GetCharGuid();
    BattlePlayerHash::iterator iter = battle_player_hash_.find(guid);
    if (iter != battle_player_hash_.end())
    {
        BattlePlayer* battle_player = iter->second;
        NotifyLeaveMessage(battle_player, abnormal_disconnect);
        //battle_player->player = NULL;
        battle_player->char_guid = 0;
    }
}

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
void GameBattleResult::DoRecord(Character* character, eBATTLE_RECORD_TYPE record_type, int impact_value)
#else
void GameBattleResult::DoRecord(Character* character, eBATTLE_RECORD_TYPE record_type, uint impact_value)
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
{
    if (character == NULL)
        return;

    Player* player = character->IsEqualObjectKind(PLAYER_OBJECT) ? (Player*)character : NULL;
    if (player == NULL)
        return;

    CHARGUID guid = player->GetCharGuid();
    BattlePlayer* battle_player = FindBattlePlayer(guid);
    //if (battle_player && battle_player->player)
    if (battle_player && battle_player->char_guid != 0)
    {
        if (battle_player->battle_recorder.DoRecord(record_type, impact_value))
            update_selector_.DoDirty(battle_player->update_key);
    }
}

void GameBattleResult::update_team(GameBattleResult::BattlePlayer* battle_player)
{
    battle_player->team = 0;
}

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
void GameBattleResult::NotifyEnterMessage(GameBattleResult::BattlePlayer* battle_player)
{
    static MSG_CG_ZONE_INSERT_BATTLE_PLAYER_CMD msg_to_enter_player;
    static MSG_CG_ZONE_INSERT_BATTLE_PLAYER_CMD msg_to_exist_player;

    CHARGUID guid = battle_player->char_guid;
    Player* battle_find_player = PlayerManager::Instance()->FindPlayerByCharKey(guid);
    if (battle_find_player == NULL) {
        return;
    }
    ChaosZoneType chaoszone_type = battle_find_player->GetChaosZoneType();
    bool is_here_battleground = false;
    if (chaoszone_type == kBattleGround || chaoszone_type == eCHAOSZONE_TYPE_GOLDRUSH) {
        is_here_battleground = true;
    }

    // 기존 유저들에게 보내기 위한 새로운 유저의 정보 설정.
    // 새로운 유저는 1명이다.
    msg_to_exist_player.m_ZonePlayers.m_PlayerCount = 1;

    GAMEROOM_BATTLE_ENTER_INFO& enter_player_info = msg_to_exist_player.m_ZonePlayers.m_Info[0];
    enter_player_info.CharGuid = guid;
    enter_player_info.byClass  = battle_find_player->GetCharType();
    enter_player_info.byLevel  = static_cast<BYTE>(battle_find_player->GetLevel());
    enter_player_info.gender   = battle_find_player->GetGender();
    strncpy(enter_player_info.szName, battle_find_player->GetCharName(), MAX_CHARNAME_LENGTH);
    if (is_here_battleground == true) {
        enter_player_info.wTeam = (battle_find_player->GetTeamKey() == kBlue) ? HOME_TEAM : AWAY_TEAM;
    }
    else {
        enter_player_info.wTeam = HOME_TEAM; // Default Value
    }

    // 새로 들어온 유저에게 보내기위한 기존 유저들의 정보 설정.
    uint user_count = 0;
    
    if (battle_find_player->GetTeamKey() != kObserve)
    {
        BattlePlayer* exist_player = NULL;
        BattlePlayerHash::iterator iter = battle_player_hash_.begin();
        for ( ; iter != battle_player_hash_.end() ; ++iter)
        {
            // 자신에게는 보내지 않는다.
            if (iter->first == guid) {
                continue;
            }
            exist_player = iter->second;
            if (exist_player->char_guid == 0) {
                continue;
            }
            Player* exists_find_player = PlayerManager::Instance()->FindPlayerByCharKey(exist_player->char_guid);
            if (exists_find_player == NULL) {
                continue;
            }

            if (is_here_battleground == false) {
                // 전장이 아닐 경우
                enter_player_info.wTeam = (battle_player->team == exist_player->team) ? HOME_TEAM : AWAY_TEAM;
            }
            // 기존 유저들에게 새로운 유저의 참여를 통보한다.
            exists_find_player->SendPacket(&msg_to_exist_player, msg_to_exist_player.GetSize());

            // 새로운 유저에게 보내기 위한 기존 유저들의 정보를 설정한다.
#ifdef _NA_008270_20150511_MODIFY_OUT_OF_RANGE
            if (user_count < ZONE_INSERT_BATTLE_PLAYER_INFO::MAX_PLAYER - 1)
#else
            if (user_count < ZONE_INSERT_BATTLE_PLAYER_INFO::MAX_PLAYER)
#endif // _NA_008270_20150511_MODIFY_OUT_OF_RANGE
            {
                GAMEROOM_BATTLE_ENTER_INFO& exist_player_info = msg_to_enter_player.m_ZonePlayers.m_Info[user_count];
                exist_player_info.CharGuid = exists_find_player->GetCharGuid();
                exist_player_info.byClass  = exists_find_player->GetCharType();
                exist_player_info.byLevel  = static_cast<BYTE>(exists_find_player->GetLevel());
                strncpy(exist_player_info.szName, exists_find_player->GetCharName(), MAX_CHARNAME_LENGTH);
                exist_player_info.gender = exists_find_player->GetGender();
                if (is_here_battleground == true) {
                    exist_player_info.wTeam    = (exist_player->team == kBlue) ? HOME_TEAM : AWAY_TEAM;
                }
                else {
                    exist_player_info.wTeam    = (battle_player->team == exist_player->team) ? HOME_TEAM : AWAY_TEAM;
                }
                ++user_count;
            }
        }
    } // end if(battle_find_player->GetTeamKey() != kObserve)
    else // if(battle_find_player->GetTeamKey() == kObserve)
    {
        battle_find_player->SendPacket(&msg_to_enter_player, msg_to_enter_player.GetSize());
        return;
    }

    // 입장한 유저에게 보낼 정보에 자기자신의 정보도 넣어보낸다
    GAMEROOM_BATTLE_ENTER_INFO& self_player_info = msg_to_enter_player.m_ZonePlayers.m_Info[user_count];
    self_player_info.CharGuid = guid;
    self_player_info.byClass  = battle_find_player->GetCharType();
    self_player_info.byLevel  = static_cast<BYTE>(battle_find_player->GetLevel());
    strncpy(self_player_info.szName, battle_find_player->GetCharName(), MAX_CHARNAME_LENGTH);
    self_player_info.gender = battle_find_player->GetGender();
    if (is_here_battleground == true) {
        self_player_info.wTeam    = (battle_find_player->GetTeamKey() == kBlue) ? HOME_TEAM : AWAY_TEAM;
    }
    else {
        self_player_info.wTeam    = HOME_TEAM;
    }
    msg_to_enter_player.m_ZonePlayers.m_PlayerCount = ++user_count;

    // 새로운 유저에게 기존 유저들의 정보를 보낸다.
    battle_find_player->SendPacket(&msg_to_enter_player, msg_to_enter_player.GetSize());
}

void GameBattleResult::NotifyLeaveMessage(GameBattleResult::BattlePlayer* battle_player, bool abnormal_disconnect)
{
    static MSG_CG_ZONE_REMOVE_BATTLE_PLAYER_CMD msg_to_exist_player;

    //CHARGUID guid = battle_player->player->GetCharGuid();
    CHARGUID guid = battle_player->char_guid;

    // 새로들어온 유저의 정보 설정.
    GAMEROOM_BATTLE_LEAVE_INFO& leave_player_info = msg_to_exist_player.m_ZonePlayers.m_Info[0];
    msg_to_exist_player.m_ZonePlayers.m_PlayerCount = 1;
    leave_player_info.CharGuid = guid;
    leave_player_info.AbNormalDisconnect = abnormal_disconnect ? 1 : 0;

    BattlePlayer* exist_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        // 자신에게는 보내지 않는다.
        if (iter->first == guid)
            continue;

        exist_player = iter->second;
        //if (exist_player->player == NULL)
        if (exist_player->char_guid == 0)
            continue;

        // 기존 유저에게 새로운 유저의 나감을 알린다.
        Player* exist_find_player = PlayerManager::Instance()->FindPlayerByCharKey(exist_player->char_guid);
        if (exist_find_player != NULL)
        {
            exist_find_player->SendPacket(&msg_to_exist_player, msg_to_exist_player.GetSize());
        }
    }
}
#else
void GameBattleResult::NotifyEnterMessage(GameBattleResult::BattlePlayer* battle_player)
{
    static MSG_CG_ZONE_INSERT_BATTLE_PLAYER_CMD msg_to_enter_player;
    static MSG_CG_ZONE_INSERT_BATTLE_PLAYER_CMD msg_to_exist_player;

    //CHARGUID guid = battle_player->player->GetCharGuid();
    CHARGUID guid = battle_player->char_guid;

    Player* battle_find_player = PlayerManager::Instance()->FindPlayerByCharKey(battle_player->char_guid);
    if (battle_find_player == NULL)
    {
        return;
    }

    // 기존 유저들에게 보내기 위한 새로운 유저의 정보 설정.
    // 새로운 유저는 1명이다.
    msg_to_exist_player.m_ZonePlayers.m_PlayerCount = 1;

    GAMEROOM_BATTLE_ENTER_INFO& enter_player_info = msg_to_exist_player.m_ZonePlayers.m_Info[0];
    enter_player_info.CharGuid = guid;
    enter_player_info.byClass  = battle_find_player->GetCharType();
    enter_player_info.byLevel  = battle_find_player->GetLevel();
    enter_player_info.wTeam    = HOME_TEAM; // Default Value
#ifdef _NA_002050_20110216_ADD_GENDER
    enter_player_info.gender   = battle_find_player->GetGender();
#endif // _NA_002050_20110216_ADD_GENDER
    strncpy(enter_player_info.szName, battle_find_player->GetCharName(), MAX_CHARNAME_LENGTH);


    // 새로 들어온 유저에게 보내기위한 기존 유저들의 정보 설정.
    uint user_count = 0;
    BattlePlayer* exist_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        // 자신에게는 보내지 않는다.
        if (iter->first == guid)
            continue;

        exist_player = iter->second;
        if (exist_player->char_guid == 0)
            continue;

        Player* exists_find_player = PlayerManager::Instance()->FindPlayerByCharKey(exist_player->char_guid);
        if (exists_find_player == NULL)
        {
            continue;
        }


        // 기존 유저들에게 새로운 유저의 참여를 통보한다.
        enter_player_info.wTeam = (battle_player->team == exist_player->team) ? HOME_TEAM : AWAY_TEAM;
        exists_find_player->SendPacket(&msg_to_exist_player, msg_to_exist_player.GetSize());


        // 새로운 유저에게 보내기 위한 기존 유저들의 정보를 설정한다.
        if (user_count < ZONE_INSERT_BATTLE_PLAYER_INFO::MAX_PLAYER)
        {
            GAMEROOM_BATTLE_ENTER_INFO& exist_player_info = msg_to_enter_player.m_ZonePlayers.m_Info[user_count];
            exist_player_info.CharGuid = exists_find_player->GetCharGuid();
            exist_player_info.byClass  = exists_find_player->GetCharType();
            exist_player_info.byLevel  = exists_find_player->GetLevel();
            exist_player_info.wTeam    = (battle_player->team == exist_player->team) ? HOME_TEAM : AWAY_TEAM;
            strncpy(exist_player_info.szName, exists_find_player->GetCharName(), MAX_CHARNAME_LENGTH);
#ifdef _NA_002050_20110216_ADD_GENDER
            exist_player_info.gender = exists_find_player->GetGender();
#endif // _NA_002050_20110216_ADD_GENDER
            ++user_count;
        }
    }


    // 기존 유저들의 정보에 새로 들어온 유저의 정보를 추가한다.
    GAMEROOM_BATTLE_ENTER_INFO& self_player_info = msg_to_enter_player.m_ZonePlayers.m_Info[user_count];
    self_player_info.CharGuid = guid;
    self_player_info.byClass  = battle_find_player->GetCharType();
    self_player_info.byLevel  = battle_find_player->GetLevel();
    self_player_info.wTeam    = HOME_TEAM;
    strncpy(self_player_info.szName, battle_find_player->GetCharName(), MAX_CHARNAME_LENGTH);
#ifdef _NA_002050_20110216_ADD_GENDER
    self_player_info.gender = battle_find_player->GetGender();
#endif // _NA_002050_20110216_ADD_GENDER
    msg_to_enter_player.m_ZonePlayers.m_PlayerCount = ++user_count;

    // 새로운 유저에게 기존 유저들의 정보를 보낸다.
    battle_find_player->SendPacket(&msg_to_enter_player, msg_to_enter_player.GetSize());
}

void GameBattleResult::NotifyLeaveMessage(GameBattleResult::BattlePlayer* battle_player, bool abnormal_disconnect)
{
    static MSG_CG_ZONE_REMOVE_BATTLE_PLAYER_CMD msg_to_exist_player;

    //CHARGUID guid = battle_player->player->GetCharGuid();
    CHARGUID guid = battle_player->char_guid;

    // 새로들어온 유저의 정보 설정.
    GAMEROOM_BATTLE_LEAVE_INFO& leave_player_info = msg_to_exist_player.m_ZonePlayers.m_Info[0];
    msg_to_exist_player.m_ZonePlayers.m_PlayerCount = 1;
    leave_player_info.CharGuid = guid;
    leave_player_info.AbNormalDisconnect = abnormal_disconnect ? 1 : 0;

    BattlePlayer* exist_player = NULL;
    BattlePlayerHash::iterator iter = battle_player_hash_.begin();
    for ( ; iter != battle_player_hash_.end() ; ++iter)
    {
        // 자신에게는 보내지 않는다.
        if (iter->first == guid)
            continue;

        exist_player = iter->second;
        //if (exist_player->player == NULL)
        if (exist_player->char_guid == 0)
            continue;

        // 기존 유저에게 새로운 유저의 나감을 알린다.
        Player* exist_find_player = PlayerManager::Instance()->FindPlayerByCharKey(exist_player->char_guid);
        if (exist_find_player != NULL)
        {
            exist_find_player->SendPacket(&msg_to_exist_player, msg_to_exist_player.GetSize());
        }
    }
}
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#ifdef _NA_001990_ACSYSTEM_ADD
const BattleRecorder::Record* GameBattleResult::GetPlayerBattleResult(Player* player)
{
    CHARGUID guid = player->GetCharGuid();
    BattlePlayerHash::iterator iter = battle_player_hash_.find(guid);
    if (iter != battle_player_hash_.end())
    {
        BattlePlayer* battle_player = iter->second;
        return &battle_player->battle_recorder.get_record();
    }

    return NULL;
}
#endif //_NA_001990_ACSYSTEM_ADD

#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
void GameBattleResult::AddToTalDropHeim(MONEY add_heim)
{
    total_droped_heim_ += add_heim;
}
MONEY GameBattleResult::GetTotalDropHeim() const
{
    return total_droped_heim_;
}
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG 
