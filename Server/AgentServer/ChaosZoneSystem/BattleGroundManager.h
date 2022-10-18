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
    kStandby = 0,       // ��⿭ ����
    kStandbyPriority,   // ��⿭ �켱���� ����
};

struct BattleGroundMemberInfo
{
    USERGUID user_guid;                 //���� ���̵�
    MAPCODE map_code;                   //���ڵ�
    KEYTYPE reservation_key;            //���� Ű
    KEYTYPE group_key;                  //�׷� Ű
    CTime reservation_time;             //���� �ð�
    BattleGroundMemberState member_state;    //��⿭ ����
    ChaosZoneTeam team_division;        //�� ����
    int equip_item_score;               //�������
    eCHAR_TYPE class_code;              //Ŭ���� �ڵ� 
    bool is_healer;                     //��������
    
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
        eBATTLEGROUND_STRONGPOINT,      // ���� ��Ż��(ex:���Ǹ�/���� ������)
        eBATTLEGROUND_GOLDRUSH,         // ��巯��
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

    util::CObjKeyGenerator<WORD> key_generater_; // �������� ��ȣ �߱�

    // ���Ǯ
    BattleGroundMemberPool* battle_ground_member_pool_;
    BattleGroundMemberListPool* battle_ground_member_list_pool_;

    BattleGroundMemberMap reservation_total_members_; // ������ ��� ����� ����Ʈ
    StandbyGroupMap standby_groups_; // ���� �ʺ� ����� ����Ʈ

    Handler_CG_BATTLE_GROUND handler_cg_battle_ground_;
    Handler_AG_BATTLE_GROUND handler_ag_battle_ground_;

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
    Handler_CG_GOLDRUSH handler_cg_goldrush_;
    Handler_AG_GOLDRUSH handler_ag_goldrush_;
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH
};

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND