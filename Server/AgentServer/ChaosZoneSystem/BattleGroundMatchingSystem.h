#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <atltime.h>
class GameServerSession;
struct BattleGroundMemberInfo;
typedef STLX_VECTOR<BattleGroundMemberInfo*> BattleGroundMemberList;

enum MatchinGroupState
{
    kStateNone = 0,
    kWaitCreateRoom,
    kCreatedRoom,
    kStartBattleGround
};

struct MatchingGroup
{
    MatchinGroupState matching_state;
    CTime matching_time;
    KEYTYPE group_key;
    BattleGroundMemberList matching_group_list;
    
    //
    struct {
        MAPCODE map_code;
        KEYTYPE zone_key;
    };
};

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
class BattleGroundMatchingSystem
{
public:
    BattleGroundMatchingSystem() {}
    ~BattleGroundMatchingSystem() {}

    void Init();
    void Release();
    void Update();
    
    bool CheckMatching(const MAPCODE map_code); //매칭 확인
    bool CanCreateMatchingGroup(const MAPCODE map_code); // 매칭 조건 확인
    bool CreateMatchingGroup(const MAPCODE map_code); // 매칭 그룹 생성
    MatchingGroup* OnMatching(const MAPCODE map_code,const BattleGroundMemberList* member_list); // 매칭(1차매칭)
    bool RemoveMatchigGroup(const KEYTYPE group_key); // 매칭 그룹 제거
    MatchingGroup* FindMatchingGroup(const KEYTYPE group_key); // 매칭 그룹 찾기
    MatchingGroup* FindMatchingGroupByZoneKey(const KEYTYPE zone_key); // 매칭 그룹 찾기
    
    bool CreateInstanceRoom(const KEYTYPE group_key, const MAPCODE map_code); // 전장(인스턴트 룸) 생성
    bool OnTeamMatching(MatchingGroup* const matching_group); // team매칭(2차매칭)
    bool SendMembersToBattleGround(MatchingGroup* const matching_group); // 전장(인스턴트 룸)으로 이동
    bool OnCreateParty(MatchingGroup* const matching_group); // 강제 파티 생성
    bool EnterTeam(const BattleGroundMemberList member_list); // GameServer에 팀정보 셋팅

    bool MatchingGroupCancel(const KEYTYPE group_key); // 매칭 취소
    bool ReturnUserInsertStandbyGroupList(MatchingGroup* const matching_group); // 전장 대기열로 이동
    bool RemoveMemberMatchingGroup(const BattleGroundMemberInfo* member); // 매칭그룹내 멤버 제거

private:
    inline KEYTYPE _AllocKey()
    { 
        return static_cast<KEYTYPE>(ObjKeyGender::Instance()->GetKey(OBJKEYTYPE_BATTLE_GROUND_ROOM_KEY)); 
    }
    inline void _FreeKey(KEYTYPE Key) 
    { 
        ObjKeyGender::Instance()->RestoreKey(OBJKEYTYPE_BATTLE_GROUND_ROOM_KEY, Key);
    }

private:
    typedef STLX_HASH_MAP<KEYTYPE,MatchingGroup*> MatchingGroupMap;
    typedef util::CMemoryPoolFactory<MatchingGroup> MatchingGroupPool;

private:
    MatchingGroupPool* matching_group_pool_;
    MatchingGroupMap matching_groups_map_;      // 매칭 그룹 리스트
    ITimerBase update_timer_;
    
};

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND