#pragma once

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <Singleton.h>
#include <atltime.h>
#include <ChaosZoneSystem/Handler_CG_BATTLE_GROUND.h>
#include <ChaosZoneSystem/Handler_AG_BATTLE_GROUND.h>
#include <BattleGroundInfoParser.h>
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
#include "ChaosZoneSystem/Handler_CG_GOLDRUSH.h"
#include "ChaosZoneSystem/Handler_AG_GOLDRUSH.h"
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
class BattleGroundMatchingSystem;

enum BattleGroundMemberState
{
    kStandby = 0,       // 대기열 상태
    kStandbyPriority,   // 대기열 우선순위 상태
};

struct BattleGroundMemberInfo
{
    USERGUID user_guid;                 //유저 아이디
    MAPCODE map_code;                   //맵코드
    KEYTYPE reservation_key;            //예약 키
    KEYTYPE group_key;                  //그룹 키
    CTime reservation_time;             //예약 시간
    BattleGroundMemberState member_state;    //대기열 상태
    ChaosZoneTeam team_division;        //팀 구분
    int equip_item_score;               //장비평점
    eCHAR_TYPE class_code;              //클래스 코드 
    bool is_healer;                     //힐러여부
    
    BattleGroundMemberInfo() : 
        user_guid(0),
        map_code(0),
        reservation_key(0),
        group_key(0),
        reservation_time(0),
        member_state(kStandby),
        team_division(kChaosZoneTeamNone),
        equip_item_score(0),
        class_code(eCHAR_NONE),
        is_healer(false)
    {
    }
};


typedef STLX_VECTOR<BattleGroundMemberInfo*> BattleGroundMemberList;

class BattleGroundManager : public util::Singleton<BattleGroundManager>
{  
public:
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    enum eBATTLEGROUND_TYPE {
        eBATTLEGROUND_STRONGPOINT,      // 거점 쟁탈전(ex:자피르/서리 격전지)
        eBATTLEGROUND_GOLDRUSH,         // 골드러시
        eBATTLEGROUND_MAX
    };
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

    BattleGroundManager();
    ~BattleGroundManager();
    void Init();
    void Release();
    void Update();

    bool LeaveRoom(User* const user, const ZONEKEY zone_key);

    bool InsertReservationMember(const USERGUID guid, MAPCODE map_code, int equip_item_score, 
                                 eCHAR_TYPE class_code, bool is_healer);
    void RemoveReservationMember(const USERGUID guid);
    RC::eBATTLE_GROUND_RESULT CanReservation(const User* user, const MAPCODE map_code);
    
    bool IsValidReservationMember(const USERGUID guid);
    bool IsValidStandbyGroupMember(const USERGUID guid);
    bool ReturnUserInsertStandbyGroupList(const BattleGroundMemberInfo* battle_ground_member);
    
    BattleGroundMemberList* FindStandbyGroupMemberList(const MAPCODE map_code);
    BattleGroundMemberInfo* FindBattleGroundMember(const USERGUID guid);

    void InsertMemberStandbyGroup(const BattleGroundMemberInfo* battle_ground_member);
    bool RemoveMemberStandbyGroup(const BattleGroundMemberInfo* battle_ground_member);
    void SendPacketAllMember(const BattleGroundMemberList* member_list, MSG_BASE* msg, WORD size);

    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    bool CheckMatching(const MAPCODE map_code);
    
#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    void ProcessPacket_CG(const ChaosZoneCategory& category_type, User* user, MSG_BASE* packet, WORD size);
    void ProcessPacket_AG(const ChaosZoneCategory& category_type, GameServerSession* server_session, MSG_BASE* packet, WORD size);
#else
    inline Handler_CG_BATTLE_GROUND& GetCGPacketHandler() { return handler_cg_battle_ground_; }
    inline Handler_AG_BATTLE_GROUND& GetAGPacketHandler() { return handler_ag_battle_ground_; }
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

private:
    typedef STLX_HASH_MAP<USERGUID,BattleGroundMemberInfo*> BattleGroundMemberMap;
    typedef STLX_HASH_MAP<MAPCODE,BattleGroundMemberList*> StandbyGroupMap;

    typedef util::CMemoryPoolFactory<BattleGroundMemberInfo> BattleGroundMemberPool;
    typedef util::CMemoryPoolFactory<BattleGroundMemberList> BattleGroundMemberListPool;

private:
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    BattleGroundMatchingSystem* battleground_matching_system_;

    util::CObjKeyGenerator<WORD> key_generater_; // 예약접수 번호 발급

    // 대기풀
    BattleGroundMemberPool* battle_ground_member_pool_;
    BattleGroundMemberListPool* battle_ground_member_list_pool_;

    BattleGroundMemberMap reservation_total_members_; // 예약한 모든 대기자 리스트
    StandbyGroupMap standby_groups_; // 전장 맵별 대기자 리스트

    Handler_CG_BATTLE_GROUND handler_cg_battle_ground_;
    Handler_AG_BATTLE_GROUND handler_ag_battle_ground_;

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    Handler_CG_GOLDRUSH handler_cg_goldrush_;
    Handler_AG_GOLDRUSH handler_ag_goldrush_;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
};

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND