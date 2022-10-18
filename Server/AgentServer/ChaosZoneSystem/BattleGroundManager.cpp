#include "StdAfx.h"

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#include "ChaosZoneSystem/BattleGroundManager.h"
#include <ChaosZoneSystem/BattleGroundMatchingSystem.h>
#include <AgentParty/AgentPartyManager.h>
#include <MapInfoParser.h>
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>
#include <ServerSessions/BattleServerSession.h>
#include <PacketHandler/Handler_FromClient.h>
#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
#include <TimeCheck/BattleGroundOpenTimeManager.h>
#include <ServerOptionParserEx.h>
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM

#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
#include <ServerOptionParserEx.h>
#endif


BattleGroundManager::BattleGroundManager()
{
    battle_ground_member_pool_ = NULL;
    battle_ground_member_list_pool_ = NULL;
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    battleground_matching_system_ = NULL;
}

BattleGroundManager::~BattleGroundManager()
{
}

void BattleGroundManager::Init()
{
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    battleground_matching_system_ = new BattleGroundMatchingSystem();
    battleground_matching_system_->Init();

    key_generater_.Create(1, 6000);
    battle_ground_member_pool_ = new BattleGroundMemberPool();
    battle_ground_member_list_pool_ = new BattleGroundMemberListPool();

    battle_ground_member_pool_->Initialize(60);
    battle_ground_member_list_pool_->Initialize(10);
}

void BattleGroundManager::Release()
{
    // 전체 대기열 Release
    BattleGroundMemberMap::const_iterator const_itr_members = reservation_total_members_.begin();
    for (; const_itr_members != reservation_total_members_.end(); ++const_itr_members)
    {
        BattleGroundMemberInfo* const battle_ground_member = const_itr_members->second;
        if (battle_ground_member == NULL)
        {
            continue;
        }
        battle_ground_member_pool_->Free(battle_ground_member);
    }
    reservation_total_members_.clear();
    
    // 그룹별 대기열 Release
    StandbyGroupMap::const_iterator const_itr_groups = standby_groups_.begin();
    for (; const_itr_groups != standby_groups_.end(); ++const_itr_groups)
    {
        BattleGroundMemberList* const memeber_list = const_itr_groups->second;
        if (memeber_list == NULL)
        {
            continue;
        }
        battle_ground_member_list_pool_->Free(memeber_list);
    }
    standby_groups_.clear();

    SAFE_DELETE(battle_ground_member_pool_);
    SAFE_DELETE(battle_ground_member_list_pool_);

    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    if (battleground_matching_system_)
        battleground_matching_system_->Release();
    SAFE_DELETE(battleground_matching_system_);
}

void BattleGroundManager::Update()
{
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    battleground_matching_system_->Update();

    // 인원수가 과반이 넘고 첫번째 예약자가 20분이상 대기했을 경우 강제매칭
    const BattleGroundInfoParser* const battle_ground_info_parser = \
                                                                 BattleGroundInfoParser::Instance();

    CTime current_time = util::TimeSync::_time64(NULL);

    StandbyGroupMap::const_iterator const_itr = standby_groups_.begin();
    for (; const_itr != standby_groups_.end(); ++const_itr)
    {
        BattleGroundMemberList* const member_list = const_itr->second;
        if (member_list == NULL)
        {
            continue;
        }

        const BattleGroundInfo* const battle_ground_Info = \
            battle_ground_info_parser->FindData(const_itr->first);
        int extract_list = static_cast<int>(battle_ground_Info->team_max);
    
        if ((extract_list%2) != 0)
            extract_list += 1;

        if ((int)member_list->size() < extract_list) // 과반 초과일 경우..
        {
            continue;
        }

        // 대기열의 첫번째 예약자의 시간..
        BattleGroundMemberInfo* const first_member_info = member_list->front();
        if (first_member_info == NULL)
        {
            continue;
        }

        if (current_time < 
            first_member_info->reservation_time + CTimeSpan(0, 0, 0, battle_ground_Info->max_match_time))
        {
            continue;
        }

        // 강제매칭
        //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
        if (battleground_matching_system_->CreateMatchingGroup(first_member_info->map_code) == false) {
            continue;
        }
    }
}

bool BattleGroundManager::LeaveRoom(User* const user, const ZONEKEY zone_key)
{
    if (user == NULL)
    {
        return false;
    }

    BattleGroundMemberInfo* const member_info = FindBattleGroundMember(user->GetUserKey());
    if (member_info == NULL)
    {
        return false;
    }
    
    // 해당 유저를 매칭 그룹에서 제거
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    if (battleground_matching_system_->RemoveMemberMatchingGroup(member_info) == false) {
        return false;
    }

    // 전체 대기열에서 제거
    RemoveReservationMember(user->GetUserKey());

    // 매칭 그룹리스트에 아무도 없으면 리스트 제거
    //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
    MatchingGroup* const matching_group = \
        battleground_matching_system_->FindMatchingGroupByZoneKey(zone_key);

    if (matching_group == NULL)
    {
        return false;
    }

    if (matching_group->matching_group_list.size() <= 0)
    {
        //_NA_007065_20140217_BATTLEGROUND_REFACTORYING
        if (battleground_matching_system_->RemoveMatchigGroup(matching_group->group_key) == false) {
            return false;
        }
    }

    // 그룹키 및 팀 초기화..
    user->SetGroupKey(0);
    user->SetTeam(kChaosZoneTeamNone);
    return true;
}

RC::eBATTLE_GROUND_RESULT BattleGroundManager::CanReservation(const User* user, 
                                                              const MAPCODE map_code)
{
    if (user == NULL)
    {
        return RC::RC_BATTLE_GROUND_IS_NOT_USER;
    }
#ifdef _NA_007135_20140402_MODIFY_BATTLE_GROUND_RESERVATION_AVAILABLE_IN_BATTLE_GROUND_CHANNEL
    AgentServer* pAgentServer = AgentServer::GetInstance();
    SERVER_KEY rKey(pAgentServer->GetKey());

    ServerOptionParserEx* const server_option_parser = ServerOptionParserEx::Instance();
    const int by_channel = server_option_parser->GetServerOption().m_byBattleGroundChannel;

    // 전장 예약 가능 채널이 -1일 경우 모든 채널에서 예약이 가능하다
    if (by_channel != rKey.GetChannelID() && by_channel != -1)
    {
        return RC::RC_BATTLE_GROUND_RESERVATION_NOT_AVAILABLE_CHANNEL;
    }
#endif

#ifdef _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM
    ServerOptionParserEx* const server_option = ServerOptionParserEx::Instance();
    const BYTE time_control_system_active =
        server_option->GetServerOption().battle_ground_time_control_system;
    
    // 시간제어 활성화 및 입장시간 확인
    if (time_control_system_active == 1 &&
        BattleGroundOpenTimeManager::Instance()->IsOpenBattleGround() == false)
    {
        return RC::RC_BATTLE_GROUND_NONADMISSION_TIME;
    }
#endif // _NA_006538_BATTLE_GROUND_TIME_CONTROL_SYSTEM

    // 유저 상태 확인..
    if (user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
    {
        return RC::RC_BATTLE_GROUND_RESERVATION_FAIL;
    }

    // 파티 여부 확인..
    if ((const_cast<User*>(user))->GetPartyState().GetPartyKey() != 0)
    {
        return RC::RC_BATTLE_GROUND_ALREADY_EXIST_PARTY;
    }

    //입장 제한 여부
    const BattleGroundInfoParser* const battle_ground_Info_parser = BattleGroundInfoParser::Instance();
    const BattleGroundInfo* battle_ground_Info = battle_ground_Info_parser->FindData(map_code);
    if (battle_ground_Info == NULL)
    {
        return RC::RC_BATTLE_GROUND_NOT_INCORRECT_MAPCODE;
    }

    // 제한 레벌
    if (user->GetSelectedCharLV() < battle_ground_Info->limit_level)
    {
        return RC::RC_BATTLE_GROUND_IS_NOT_QUALIFIED_LEVEL;
    }

    BattleGroundMemberMap::const_iterator it = reservation_total_members_.find(user->GetUserKey());
    if (it != reservation_total_members_.end())
    {
        // 이미 전장 예약중..대기열에 들어있음..
        return RC::RC_BATTLE_GROUND_RESERVATION_ALREADY;
    }

    return RC::RC_BATTLE_GROUND_RESERVATION_SUCCESS;
}

bool BattleGroundManager::InsertReservationMember(const USERGUID guid, MAPCODE map_code, 
                                                  int equip_item_score, eCHAR_TYPE class_code, 
                                                  bool is_healer)
{
    BattleGroundMemberMap::const_iterator itr = reservation_total_members_.find(guid);
    if (itr != reservation_total_members_.end()) {
        // 이미 전장 예약중..대기열에 들어있음..
        return false;
    }

    BattleGroundMemberInfo* const battle_ground_member = 
        static_cast<BattleGroundMemberInfo*>(battle_ground_member_pool_->Alloc());

    battle_ground_member->user_guid = guid;
    battle_ground_member->map_code = map_code;
    battle_ground_member->reservation_key = key_generater_.GetKey();
    battle_ground_member->group_key = 0;
    battle_ground_member->reservation_time = util::TimeSync::_time64(NULL);
    battle_ground_member->member_state = kStandby;
    battle_ground_member->team_division = kChaosZoneTeamNone;
    battle_ground_member->equip_item_score = equip_item_score;
    battle_ground_member->class_code = class_code;
    battle_ground_member->is_healer = is_healer;

    reservation_total_members_.insert( \
        BattleGroundMemberMap::value_type(guid,battle_ground_member));

    return true;
}

void BattleGroundManager::RemoveReservationMember(const USERGUID guid)
{
    BattleGroundMemberMap::iterator itr = reservation_total_members_.find(guid);
    if (itr == reservation_total_members_.end()) {
        return;
    }

    BattleGroundMemberInfo* const battle_ground_member = itr->second;
    key_generater_.RestoreKey(battle_ground_member->reservation_key);
    battle_ground_member_pool_->Free(battle_ground_member);
    reservation_total_members_.erase(itr);
}


void BattleGroundManager::InsertMemberStandbyGroup(const BattleGroundMemberInfo* battle_ground_member)
{
    if (battle_ground_member == NULL)
    {
        return;
    }

    StandbyGroupMap::const_iterator const_itr = standby_groups_.find(battle_ground_member->map_code);
    if (const_itr != standby_groups_.end())
    {
        BattleGroundMemberList* const battle_ground_member_list = const_itr->second;
        battle_ground_member_list->push_back( \
            const_cast<BattleGroundMemberInfo*>(battle_ground_member));
    }
    else 
    {
        MAPCODE map_code = battle_ground_member->map_code;
        BattleGroundMemberList* const battle_ground_member_list = \
            static_cast<BattleGroundMemberList*>(battle_ground_member_list_pool_->Alloc());

        battle_ground_member_list->push_back( \
            const_cast<BattleGroundMemberInfo*>(battle_ground_member));

        standby_groups_.insert( \
            StandbyGroupMap::value_type(map_code,battle_ground_member_list));
    }
}

bool BattleGroundManager::RemoveMemberStandbyGroup(const BattleGroundMemberInfo* battle_ground_member)
{
    if (battle_ground_member == NULL)
    {
        return false;
    }

    StandbyGroupMap::const_iterator const_itr = standby_groups_.find(battle_ground_member->map_code);
    if (const_itr == standby_groups_.end())
    {
        return false;
    }

    BattleGroundMemberList* const battle_ground_member_list = const_itr->second;

    BattleGroundMemberList::iterator member_list_itr = battle_ground_member_list->begin();
    for ( ;member_list_itr != battle_ground_member_list->end(); ++member_list_itr)
    {
        const BattleGroundMemberInfo* find_member = *member_list_itr;
        if (battle_ground_member->user_guid == find_member->user_guid)
        {
            battle_ground_member_list->erase(member_list_itr);
            return true;
        }
    }

    return false;
}

bool BattleGroundManager::IsValidReservationMember(const USERGUID guid)
{
    if (reservation_total_members_.find(guid) == reservation_total_members_.end())
    {
        return false;
    }
    return true;
}

bool BattleGroundManager::IsValidStandbyGroupMember(const USERGUID guid)
{
    BattleGroundMemberInfo* const member_info = FindBattleGroundMember(guid);
    if (member_info == NULL)
    {
        return false;
    }

    BattleGroundMemberList* memeber_list = FindStandbyGroupMemberList(member_info->map_code);
    if (memeber_list == NULL)
    {
        return false;
    }
    
    BattleGroundMemberList::const_iterator const_itr = memeber_list->begin();
    for (; const_itr != memeber_list->end(); ++const_itr)
    {
        BattleGroundMemberInfo* const member = *const_itr;
        if (member && member->user_guid == guid)
        {
            return true;
        }
    }

    return false;
}

bool BattleGroundManager::ReturnUserInsertStandbyGroupList(const BattleGroundMemberInfo* battle_ground_member)
{
    if (battle_ground_member == NULL)
    {
        return false;
    }

    StandbyGroupMap::const_iterator const_itr = standby_groups_.find(battle_ground_member->map_code);
    if (const_itr != standby_groups_.end())
    {
        BattleGroundMemberList* const battle_ground_member_list = const_itr->second;

        // push_front...
        battle_ground_member_list->insert( \
            battle_ground_member_list->begin(), \
            const_cast<BattleGroundMemberInfo*>(battle_ground_member));

        return true;
    }
    return false;
}

BattleGroundMemberList* BattleGroundManager::FindStandbyGroupMemberList(const MAPCODE map_code)
{
    StandbyGroupMap::iterator itr = standby_groups_.find(map_code);
    if (itr != standby_groups_.end())
    {
        return itr->second;
    }
    return NULL;
}

BattleGroundMemberInfo* BattleGroundManager::FindBattleGroundMember(const USERGUID guid)
{
    BattleGroundMemberMap::const_iterator const_itr = reservation_total_members_.find(guid);
    if (const_itr == reservation_total_members_.end())
    {
        return NULL;
    }
    return const_itr->second;
}
void BattleGroundManager::SendPacketAllMember(const BattleGroundMemberList* member_list, 
                                                                           MSG_BASE* msg, WORD size)
{
    UserManager* const user_manager = UserManager::Instance();
    BattleGroundMemberList::const_iterator const_itr = member_list->begin();
    for (; const_itr != member_list->end(); ++const_itr)
    {
        const BattleGroundMemberInfo* const member = *const_itr;
        if (member== NULL)
        {
            continue;
        }
        User* const user = user_manager->GetUser(member->user_guid);
        if (user == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user : GUID(%d)", member->user_guid);
            continue;
        }
        user->SendPacket(msg, size);
    }
}

//_NA_007065_20140217_BATTLEGROUND_REFACTORYING
bool BattleGroundManager::CheckMatching( const MAPCODE map_code )
{
    const BattleGroundMemberList* const member_list = FindStandbyGroupMemberList(map_code);
    if (member_list == NULL) {
        return false;
    }

    if (battleground_matching_system_->CanCreateMatchingGroup(map_code) == false) {
        return false;
    }
    if (battleground_matching_system_->CreateMatchingGroup(map_code) == false) {
        //매칭그룹 만들기 실패, 메시지 전달한다.
        return false;
    }

    return true;
}

#ifdef _NA_007065_20140217_CHAOSZONE_GOLDRUSH
void BattleGroundManager::ProcessPacket_CG( const ChaosZoneCategory& category_type, User* user, 
                                            MSG_BASE* packet, WORD size )
{
    switch(category_type)
    {
    case kCategoryBattleGround:
        {
            handler_cg_battle_ground_.OnCG_ProcessPacket(user, packet, size);
        }break;
    case eCHAOSZONE_CATEGORY_GOLDRUSH:
        {
            handler_cg_goldrush_.OnCG_ProcessPacket(user, packet, size);
        }break;
    }
}

void BattleGroundManager::ProcessPacket_AG( const ChaosZoneCategory& category_type, GameServerSession* server_session, 
                                            MSG_BASE* packet, WORD size )
{
    switch(category_type)
    {
    case kCategoryBattleGround:
        {
            handler_ag_battle_ground_.OnAG_ProcessPacket(server_session, packet, size);
        }break;
    case eCHAOSZONE_CATEGORY_GOLDRUSH:
        {
            handler_ag_goldrush_.OnAG_ProcessPacket(server_session, packet, size);
        }break;
    }
}
#endif //_NA_007065_20140217_CHAOSZONE_GOLDRUSH

#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND