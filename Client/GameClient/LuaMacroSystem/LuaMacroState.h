#pragma once
#ifdef _NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM

const LUA_FILE_NAME_LENGTH = 60;

#define WRITE_MACRO_LOG

void WriteMacroLog(TCHAR* message, BOOL force_write = FALSE);

class NPC;
class MapObject;

enum MacroType
{
    // 실행 단계 관리용
    Macro_RunMacro,
    Macro_EndMacro,

    // 실제 명령
    Macro_Move,
    Macro_MoveMap,
    Macro_NpcMeet,
    Macro_CreateRoom,
    Macro_JoinRoom,
    Macro_ExitRoom,

    Macro_QuestAccept,
    Macro_QuestReward,
    //Macro_QuestCancle,
    Macro_Attack,
    Macro_Wait,
    Macro_Jump,
    Macro_MoveAttack,

    Macro_TriggerClick,
    Macro_CollectClick,
    Macro_MissionReward,
    
    Macro_RuneStone,
    Macro_DummyMemory,

    Macro_Max
};

class MacroCommand
{
public:
    MacroType command_type_;

    union 
    {
        TCHAR file_name_[LUA_FILE_NAME_LENGTH];
        struct              // Macro_Move, Macro_Jump
        {
            float x_;
            float y_;
            float z_;
        } MovePosition;
        DWORD map_code_;    // Macro_MoveMap,
        DWORD npc_code_;    // Macro_NpcMeet,
        struct              // Macro_CreateRoom,
        {
            BYTE depth1_;
            BYTE depth2_;
            BYTE depth3_;
        } CreateRoomCommand;
        DWORD quest_code_;   // Macro_QuestAccept
        struct               // Macro_QuestReward
        {
            DWORD quest_code_;
            BYTE reward_index_;   
        } QuestReward;
        struct               // Macro_Attack
        {
            DWORD monster_code_;
            DWORD fight_time_;
        }Attack;
        DWORD wait_time_;    // Macro_Wait
        struct               // Macro_MoveAttack
        {
            DWORD monster_code_;
            float x_;
            float y_;
            float z_;
        }MoveAttack;
        BYTE select_item_;   //Macro_MissionReward
        BYTE index_;  //Macro_RuneStone
        DWORD size_;         //Macro_DummyMemory
    }value;
};

class MacroScript
{
    // 하나의 루아 스크립트 파일 단위
    // 실행중인 스크립트 파일 관리용
public:
    MacroScript()
    {
        ZeroMemory(script_name_, LUA_FILE_NAME_LENGTH);
    }

    TCHAR* GetFileName() { return script_name_; }
    void SetFileName(const TCHAR* file_name);
    void CallScript();
    
    void InsertMacroCommand(MacroCommand command);
    MacroCommand PopMacroCommand();

private:
    TCHAR script_name_[LUA_FILE_NAME_LENGTH];  // 스크립트 파일명

    typedef STLX_DEQUE<MacroCommand> MacroCommandQueue;
    MacroCommandQueue macro_queue_;  // 명령 큐
};


class MacroState
{
public:
    MacroState()
    {
        type_ = Macro_Max;
        tick_count_ = 0;
        is_satisfied_ = FALSE;
        wait_time_ = 0;
    }

    virtual void Init();
    virtual void Process(DWORD tick_) {}
    virtual void Action(){};
    virtual BOOL is_satisfied(DWORD tick_) { return TRUE; }
    
    int wait_time() { return wait_time_; }
    void set_wait_time(DWORD time) { wait_time_ = time; }
    BOOL is_satisfied_wait_time(DWORD tick_);

protected:
    void set_is_satisfied(BOOL is_bool) { is_satisfied_ = is_bool; }

    // 회복물약 먹기
    void UsePotion();
    // 아이템 줍기
    void PickupItem();

    MacroType type_;
    DWORD tick_count_;

    BOOL is_satisfied_;
    int wait_time_; // 조건 완료 후 대기시간
};
//
class MacroStateRunMacro : public MacroState
{
public:
    MacroStateRunMacro() { type_ = Macro_RunMacro; }
};
class MacroStateEndMacro : public MacroState
{
public:
    MacroStateEndMacro() { type_ = Macro_EndMacro; }
};

// 이동
class MacroStateMove : public MacroState
{
public:
    MacroStateMove() { type_ = Macro_Move; }
    
    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);

    void SetTargetPosition(float x, float y, float z);

private:
    WzVector target_position_;
    DWORD last_map_code_;
};
// 맵 이동
class MacroStateMoveMap : public MacroState
{
public:
    MacroStateMoveMap()
    {
        type_ = Macro_MoveMap;
        map_code_ = 0;
        wait_time_ = 0;
    }

    void Init();
    void Action();
    BOOL is_satisfied(DWORD tick_);

    void SetMapCode(DWORD map_code);

private:
    DWORD map_code_;
    DWORD wait_time_;
};
// npc 창 열기
class MacroStateNpcMeet : public MacroState
{
public:
    MacroStateNpcMeet() { type_ = Macro_NpcMeet; }

    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);

    void SetNpcCode(DWORD npc_code);

private:
    DWORD npc_code_;
    DWORD last_map_code_;
};
// 배틀존 방 생성
class MacroStateCreateRoom : public MacroState
{
public:
    MacroStateCreateRoom() { type_ = Macro_CreateRoom; }

    void Init();
    void Action();
    BOOL is_satisfied(DWORD tick_);

    void SetMenuIndex(BYTE depth1, BYTE depth2, BYTE depth3);

private:
    BYTE depth1_;
    BYTE depth2_;
    BYTE depth3_;
};
// 배틀존 입장
class MacroStateJoinRoom : public MacroState
{
public:
    MacroStateJoinRoom() { type_ = Macro_JoinRoom; }

    void Init();
    void Action();
    BOOL is_satisfied(DWORD tick_);

private:
        DWORD last_map_code_;
};
// 방 나가기
class MacroStateExitRoom : public MacroState
{
public:
    MacroStateExitRoom() { type_ = Macro_ExitRoom; }

    void Init();
    void Action();
    BOOL is_satisfied(DWORD tick_);

private:
    DWORD last_map_code_;
};
// 퀘스트수락
class MacroStateQuestAccept : public MacroState
{
public:
    MacroStateQuestAccept() { type_ = Macro_QuestAccept; }

    void Init();
    void Action();
    BOOL is_satisfied(DWORD tick_);
    
    void SetQuestCode(DWORD code) { quest_code_ = code; }

private:
    DWORD quest_code_;
};
// 퀘스트 완료
class MacroStateQuestReward : public MacroState
{
public:
    MacroStateQuestReward() { type_ = Macro_QuestReward; }

    void Init();
    void Action();
    BOOL is_satisfied(DWORD tick_);

    void SetQuestCode(DWORD code) { quest_code_ = code; }
    void SetRewardIndex(DWORD code) { reward_index_ = code; }

private:
    DWORD quest_code_;
    DWORD reward_index_;
};
// 몬스터 사냥
class MacroStateAttack : public MacroState
{
public:
    MacroStateAttack() { type_ = Macro_Attack; fight_time_ = 0; }

    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);
    
    void SetMonsterCode(DWORD monster_id) { target_monster_ = monster_id; }
    void SetTime(DWORD time) { fight_time_ = time; }

protected:
    Monster* GetNearestTarget(DWORD monster_code);
    void Attack(Monster* target);
    int GetSkillCode();

    DWORD last_map_code_;       // 시작 맵
    WzVector old_position_;     // 시작 위치
    DWORD total_tick_count_;    // 총 진행 시간
    BYTE current_slot_number_;  // 현재 슬롯 번호

    DWORD target_monster_;  // 타겟 몬스터, 0이면 모든 몬스터
    DWORD fight_time_;      // 전투 시간,   0이면 잡을때까지
};
// 대기
class MacroStateWait : public MacroState
{
public:
    MacroStateWait() { type_ = Macro_Wait; }

    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);

    void SetWaitTime(DWORD time) { wait_time_ = time; }
private:
    DWORD wait_time_;
};
// 점프
class MacroStateJump : public MacroState
{
public:
    MacroStateJump() { type_ = Macro_Jump; }

    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);
};
class MacroStateMoveAttack : public MacroStateAttack
{
public:
    MacroStateMoveAttack() { type_ = Macro_MoveAttack; fight_time_ = 0; }

    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);

    void SetTargetPosition(float x, float y, float z);
private:
    WzVector target_position_;
};
class MacroStateTriggerClick : public MacroState
{
public:
    MacroStateTriggerClick() { type_ = Macro_TriggerClick; }

    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);

private:
    DWORD last_map_code_;
    MapObject* target_object_;
};
class MacroStateCollectClick : public MacroState
{
public:
    MacroStateCollectClick() { type_ = Macro_CollectClick; }

    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);

private:
    DWORD last_map_code_;
    DWORD totol_tick_count_;
    MapObject* target_object_;
};
class MacroStateMissionReward : public MacroState
{
public:
    MacroStateMissionReward() { type_ = Macro_MissionReward; }

    void Init();
    void Action();
    BOOL is_satisfied(DWORD tick_);

    void set_select_item(BYTE select) { select_item_ = select; }

private:
    BYTE select_item_;
};
class MacroStateRuneStone : public MacroState
{
public:
    MacroStateRuneStone() { type_ = Macro_RuneStone; }

    void Init();
    void Action();
    void Process(DWORD tick_);
    BOOL is_satisfied(DWORD tick_);

    void set_index(BYTE index) { index_ = index; }

private:
    BYTE index_;
    DWORD check_delay_;   // 시작 후 일정시간 지나서 체크
};
class MacroStateDummyMemory : public MacroState
{
public:
    MacroStateDummyMemory() { type_ = Macro_DummyMemory;  memory_pointer_ = NULL; last_alloc_size_ = 0;}
    ~MacroStateDummyMemory();

    void Init();
    void Action();

    void set_alloc_size(DWORD size) { alloc_size_ = size; }

private:
    BYTE** memory_pointer_;
    DWORD alloc_size_;
    DWORD last_alloc_size_;
};
#endif //_NA_007192_20140428_CLIENT_LUA_MACRO_SYSTEM