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
    //���� �ܰ踦 �������ܰ�� �ٷοø���
    VOID SetLastRewardPhase() {reward_phase_ = GetMaxRewardPhase(); }
    BYTE GetCurRewardPhase() {return reward_phase_;}

    VOID InputTargetList(const SScheinRewardInfo* info);
    BOOL PopTarget(MONSTERCODE target_code);
    // �ܰ踦 �Ѿ�� ���� Ÿ�� ��� ���ŵǾ��°�
    BOOL IsTargetAllDeath(); 

    VOID SetCurTargetType(BYTE target_type) {cur_target_type_ = target_type;}
    BYTE GetCurTargetType() {return cur_target_type_;}
private:
    VOID _PushTarget(MONSTERCODE target_code);

private:
    // �ҳٻ��� ���� �ְ��ް�
    BYTE MAX_REWARD_PHASE;

    BYTE reward_phase_;
    
    //�� �ܰ迡�� �����ܰ�� �Ѿ�� ���� Ÿ�������� ����Ѵ�
    BYTE cur_target_type_;
    std::vector<MONSTERCODE> cur_target_list_;
};

class SonnenSchein : public ChaosZone
{
    enum 
    {
        UPDATE_TIME = 500, //0.5�ʸ��� ������Ʈ ����
        ROOM_DESTROY_TIME = 5 * 60 * 1000,  //5��, End�������� �� �Ҹ� �����ð� 
        // 10��, ���� ��, �÷��̾� ���� ���ð�(stampSuccess -> kick user)
        // 10��, END���� ����, ���� ������������� ���ð�
        ROOM_END_WAIT_TIME = 10 * 1000, 
        TIME_SYNC_CYCLE = 30 * 1000,    // �ð�����ȭ �ֱ�
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

    //SonnenSchein���� ó�� ������ �÷��̾ ����Ѵ� 
    VOID SetInitialCreater(CHARGUID char_guid, GUILDGUID guild_guid);

    virtual bool EnterPlayer(Player* const player);
    virtual void LeavePlayer(Player* const player);

    virtual void EnterTeam(const ChaosZoneTeam team_key, const BYTE player_count, 
        DWORD* user_key_list) {}

    virtual void DeadCharacter(Character* const dead_character, Character* const kill_character);
    virtual eUSER_RELATION_TYPE IsFriendPVP(Player* attacker,Player* target);

    virtual RC::eSTATUS_RESULT ResurrectionAtSafezone( Player *player );

    GUILDGUID GetRoomGuildGuid() {return room_guild_guid_;} 

     //���� ����--------------------------------------------------------------------------------
     //���� ���� ó��

     VOID SuccessStamp(Player* player);

     VOID SetStampStartTime();
     VOID InitStampCheckTime() {stamp_start_time_ = 0;}

     VOID SetEventState(const eSONNENSCHEIN_EVENT_STATE& state);
     const eSONNENSCHEIN_EVENT_STATE GetEventState() {return event_state_;}

private:
    //�����Ϸ��� �÷��̾��� ��� üũ
    BOOL _CheckEnterPlayerGuild(Player* enter_player);
    //Starting Area�� ID�� ���Ѵ�
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
    eSONNENSCHEIN_EVENT_STATE event_state_; //sonnenschein ����
    sDOMINATION_EVENT_INFO event_info_;     // ������(��Ʈ����) �̺�Ʈ����
    BOOL event_loaded_;

    CHARGUID creater_char_guid_;
    GUILDGUID   room_guild_guid_;

    PLAYER_HASH player_hash_; // ���� ���

    // �ð� ����
    DWORD stamp_start_time_;  // ���� ���� �ð�
    DWORD run_time_;          // �ҳٻ��� ���� �ð�

    util::Timer	update_timer_;      // ������Ʈ �ֱ� ����
    util::Timer room_destroy_timer_;    //End���� ����, �� �Ҹ������ �����ð�
    util::Timer kick_user_wait_timer_;  //End���� ���� �÷��̾� ������������� ���ð�
    util::Timer stamp_success_wait_timer_;  // ���μ��� -> �÷��̾� ���� ���� ���ð�

    util::Timer time_sync_cycle_;            // Ŭ��� �ð�����ȭ ���� Ÿ�̸�
    util::Timer sonnenschein_run_timer_; // �ҳٻ��� ���� Ÿ�̸�

    // �������ó��
    SonnenScheinRewardStatus reward_status_;

    // GM��ɾ ���� ����
    // �ҳٻ��� ��� ������Ʈ ���� �÷���
    BOOL is_process_phase_update_;
};



#endif //_NA_008334_20150608_SONNENSCHEIN