#pragma once

#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#include <ChaosZoneSystem/TeamBattleGroundManager.h>
#include <atltime.h>

struct TeamMatchingGroup
{
    enum MatchinGroupState
    {
        kStateNone = 0,
        kCreatedRoom,
        kStartBattleGround
    };

    KEYTYPE group_key;
    CTime matching_time;
    TeamBattleGroundMemberList matching_group_list;
    MatchinGroupState state;
    //
    struct {
        MAPCODE map_code;
        KEYTYPE zone_key;
    };
};

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
class TeamBattleGroundMatchingSystem
{
public:
    TeamBattleGroundMatchingSystem() {}
    ~TeamBattleGroundMatchingSystem() {}
    
    void Init();
    void Release();
    void Update();

    bool CheckMatching(const MAPCODE map_code); //매칭 확인
    bool CanCreateMatchingGroup(const MAPCODE map_code); // 매칭 조건 확인
    bool CreateMatchingGroup(const MAPCODE map_code); // 매칭 그룹 생성
    bool MatchingGroupCancel(const KEYTYPE group_key); // 매칭 취소
    TeamMatchingGroup* FindMatchingGroup(const KEYTYPE group_key); // 매칭 그룹 찾기
    TeamMatchingGroup* FindMatchingGroupByZoneKey(const KEYTYPE zone_key); // 매칭 그룹 찾기
    
    TeamMatchingGroup* OnMatching(); // 1차 매칭
    bool RemoveMatchigGroup(const KEYTYPE group_key); // 매칭 그룹 제거

    bool RemoveMemberMatchingGroup(const CHARGUID char_guid, const KEYTYPE group_key);

    bool CreateInstanceRoom(const KEYTYPE group_key, const MAPCODE map_code); // 전장(인스턴트 룸) 생성
    bool OnCreateTeam(TeamMatchingGroup* const matching_group);
    bool EnterTeam(const TeamBattleGroundMemberList member_list);
    bool SendMembersToBattleGround(TeamMatchingGroup* const matching_group);
    bool IsValidZoneKey(const KEYTYPE zone_key) const;
    KEYTYPE GetZoneKey() const;

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
    typedef util::CMemoryPoolFactory<TeamMatchingGroup> TeamMatchingGroupPool;
    TeamMatchingGroupPool* team_matching_group_pool_;

    typedef STLX_HASH_MAP<KEYTYPE, TeamMatchingGroup*> TeamMatchingGroupMap;
    TeamMatchingGroupMap team_matching_groups_map_;      // 매칭 그룹 리스트

    ITimerBase update_timer_;
};
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME