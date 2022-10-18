#pragma once

#ifdef _NA_008334_20150608_SONNENSCHEIN
#include <World/ChaosZoneSystem/ChaosZone.h>

struct SScheinRewardInfo;
class SonnenScheinRewardStatus
{
public:
    SonnenScheinRewardStatus():
      reward_phase_(0), MAX_REWARD_PHASE(0) {cur_target_list_.clear();}
    ~SonnenScheinRewardStatus() {}

    VOID Init();
    VOID SetMaxRewardPhase(BYTE phase) {MAX_REWARD_PHASE = phase;}
    BYTE GetMaxRewardPhase() {return MAX_REWARD_PHASE;}
    VOID IncreaseRewardPhase();
    //현재 단계를 마지막단계로 바로올린다
    VOID SetLastRewardPhase() {reward_phase_ = GetMaxRewardPhase(); }
    BYTE GetCurRewardPhase() {return reward_phase_;}

    VOID InputTargetList(const SScheinRewardInfo* info);
    BOOL PopTarget(MONSTERCODE target_code);
    // 단계를 넘어가기 위한 타겟 모두 제거되었는가
    BOOL IsTargetAllDeath(); 

    VOID SetCurTargetType(BYTE target_type) {cur_target_type_ = target_type;}
    BYTE GetCurTargetType() {return cur_target_type_;}
private:
    VOID _PushTarget(MONSTERCODE target_code);

private:
    // 소넨샤인 보상 최고등급값
    BYTE MAX_REWARD_PHASE;

    BYTE reward_phase_;
    
    //현 단계에서 다음단계로 넘어가기 위한 타겟정보를 기억한다
    BYTE cur_target_type_;
    std::vector<MONSTERCODE> cur_target_list_;
};

class SonnenSchein : public ChaosZone
{
    enum 
    {
        UPDATE_TIME = 500, //0.5초마다 업데이트 진행
        ROOM_DESTROY_TIME = 5 * 60 * 1000,  //5분, End상태이후 방 소멸 유예시간 
        // 10초, 각인 후, 플레이어 강퇴 대기시간(stampSuccess -> kick user)
        // 10초, END상태 이후, 유저 강제퇴장까지의 대기시간
        ROOM_END_WAIT_TIME = 10 * 1000, 
        TIME_SYNC_CYCLE = 30 * 1000,    // 시간동기화 주기
    }; 
public:
    SonnenSchein() : creater_char_guid_(0)
        , room_guild_guid_(0)
        , stamp_start_time_(0)
        , event_loaded_(FALSE)
        , is_process_phase_update_(FALSE) {}
    virtual ~SonnenSchein() {}
    
    virtual void Init(GameInstanceDungeon* const instance_dungeon);
    virtual void Release();
    virtual void Update();

    VOID SetSonnenScheinRewardPhase();

    //SonnenSchein방을 처음 생성한 플레이어를 기억한다 
    VOID SetInitialCreater(CHARGUID char_guid, GUILDGUID guild_guid);

    virtual bool EnterPlayer(Player* const player);
    virtual void LeavePlayer(Player* const player);

    virtual void EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, 
        DWORD* user_key_list) {}

    virtual void DeadCharacter(Character* const dead_character, Character* const kill_character);
    virtual eUSER_RELATION_TYPE IsFriendPVP(Player* attacker,Player* target);

    virtual RC::eSTATUS_RESULT ResurrectionAtSafezone( Player *player );

    GUILDGUID GetRoomGuildGuid() {return room_guild_guid_;} 

     //각인 관련--------------------------------------------------------------------------------
     //각인 성공 처리

     VOID SuccessStamp(Player* player);

     VOID SetStampStartTime();
     VOID InitStampCheckTime() {stamp_start_time_ = 0;}

     VOID SetEventState(const eSONNENSCHEIN_EVENT_STATE& state);
     const eSONNENSCHEIN_EVENT_STATE GetEventState() {return event_state_;}

private:
    //입장하려는 플레이어의 길드 체크
    BOOL _CheckEnterPlayerGuild(Player* enter_player);
    //Starting Area의 ID를 구한다
    WzID _GetStartingArea();

    VOID _IncreaseRewardPhase();
    VOID _SetNextPhaseTargetInfo();

    VOID _UpdateCurEventState();
    
    VOID _SendTimeSync();
    VOID _SendTimeSyncForPlayer(Player* player);
    VOID _SetRunTime();
    DWORD _GetRunTime(const WORD& day_of_week, const DWORD time) const;

    VOID _WriteSonnenScheinLog();
private:
    eSONNENSCHEIN_EVENT_STATE event_state_; //sonnenschein 상태
    sDOMINATION_EVENT_INFO event_info_;     // 점령전(몬트샤인) 이벤트정보
    BOOL event_loaded_;

    CHARGUID creater_char_guid_;
    GUILDGUID   room_guild_guid_;

    PLAYER_HASH player_hash_; // 입장 멤버

    // 시간 관련
    DWORD stamp_start_time_;  // 각인 시작 시간
    DWORD run_time_;          // 소넨샤인 진행 시간

    util::Timer	update_timer_;      // 업데이트 주기 조절
    util::Timer room_destroy_timer_;    //End상태 이후, 방 소멸까지의 유예시간
    util::Timer kick_user_wait_timer_;  //End상태 이후 플레이어 강제퇴장까지의 대기시간
    util::Timer stamp_success_wait_timer_;  // 각인성공 -> 플레이어 강퇴 간의 대기시간

    util::Timer time_sync_cycle_;            // 클라와 시간동기화 위한 타이머
    util::Timer sonnenschein_run_timer_; // 소넨샤인 진행 타이머

    // 보상관련처리
    SonnenScheinRewardStatus reward_status_;

    // GM명령어를 위한 변수
    // 소넨샤인 등급 업데이트 여부 플래그
    BOOL is_process_phase_update_;
};



#endif //_NA_008334_20150608_SONNENSCHEIN