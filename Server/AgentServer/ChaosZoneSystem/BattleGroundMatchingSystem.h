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
    
    bool CheckMatching(const MAPCODE map_code); //��Ī Ȯ��
    bool CanCreateMatchingGroup(const MAPCODE map_code); // ��Ī ���� Ȯ��
    bool CreateMatchingGroup(const MAPCODE map_code); // ��Ī �׷� ����
    MatchingGroup* OnMatching(const MAPCODE map_code,const BattleGroundMemberList* member_list); // ��Ī(1����Ī)
    bool RemoveMatchigGroup(const KEYTYPE group_key); // ��Ī �׷� ����
    MatchingGroup* FindMatchingGroup(const KEYTYPE group_key); // ��Ī �׷� ã��
    MatchingGroup* FindMatchingGroupByZoneKey(const KEYTYPE zone_key); // ��Ī �׷� ã��
    
    bool CreateInstanceRoom(const KEYTYPE group_key, const MAPCODE map_code); // ����(�ν���Ʈ ��) ����
    bool OnTeamMatching(MatchingGroup* const matching_group); // team��Ī(2����Ī)
    bool SendMembersToBattleGround(MatchingGroup* const matching_group); // ����(�ν���Ʈ ��)���� �̵�
    bool OnCreateParty(MatchingGroup* const matching_group); // ���� ��Ƽ ����
    bool EnterTeam(const BattleGroundMemberList member_list); // GameServer�� ������ ����

    bool MatchingGroupCancel(const KEYTYPE group_key); // ��Ī ���
    bool ReturnUserInsertStandbyGroupList(MatchingGroup* const matching_group); // ���� ��⿭�� �̵�
    bool RemoveMemberMatchingGroup(const BattleGroundMemberInfo* member); // ��Ī�׷쳻 ��� ����

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
    MatchingGroupMap matching_groups_map_;      // ��Ī �׷� ����Ʈ
    ITimerBase update_timer_;
    
};

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND