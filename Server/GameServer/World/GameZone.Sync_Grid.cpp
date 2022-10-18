#include "stdafx.h"
#include "GameZone.h"
#include "GameField.h"

#include <float.h>

#include "World/HighLoadBalancer.h"
#include "FieldInfo.h"
#include "Field.h"
#include "Sector.h"
//#include "View.h"
#include "MovementChecker.h"
#include "TargetFinder.h"
#include "Status/StatusManager.h"

#include "ItemManager.h"
#include "Item.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "Summoned.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

//==================================================================================================
// @history
//  2008.08.22, waverix, created
//  2009.11.xx~2010.04.22, lst1024, worked related to domination
//  2011.02.23, waverix, applied about the enhanced target search algorithm
//  2011.05.31, waverix, remove the broadcast upper boundary limitation using 1toN broadcaster.
//              f110531.4L, this module was defined that current version grater than 0906
//--------------------------------------------------------------------------------------------------

void GameField::SendExPacket(Player* const receiver,
    const DWORD number_of_msgs, BYTE** const IN msg_array, WORD* const msg_size_array)
{
    GameZone* const game_zone = (receiver == NULL) ? NULL : receiver->GetGameZonePtr();
    if (game_zone == NULL) {
        return;
    };
    // NOTE: f110531.4L
    m_pHLB->AppendTimeSyncMsgArray(number_of_msgs, msg_array, msg_size_array);
    //receiver->GetServerSession()->SendEx(number_of_msgs, msg_array, msg_size_array);
}


//--------------------------------------------------------------------------------------------------


void GameField::SendExPacketAround(const SECTOR_IDX coordinates,
    const DWORD number_of_msgs, BYTE** const IN msg_array, WORD* const msg_size_array,
    const DWORD except_object_key)
{
    SectorGrid3x3 OUT grid;
    if (m_SectorGrid.GetAroundSectors(coordinates, &grid) == false) {
        return;
    };
    const bool check_skip_object = !except_object_key;

    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.InitByArray(msg_array, msg_size_array, number_of_msgs);
    uint16_t* const user_begin = msg_brd.GetUsersStartPtr();

    //  Center를 먼저 처리하면서 주변 섹터 처리
    GRID3x3_CENTER_PRIOR_SWAP(grid);
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        };
        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        if (sector->GetPlayerNum() == 0) {
            continue;
        };
        ns_object::SearchNodeConstIterator it = sector->GetPlayerRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetPlayerEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Player* player = link_node->select.player_node.player;
            if (obj_util::IsValid(player) == false)
            {
                continue;
            }
            const DWORD object_key = player->GetObjectKey();

            if (!(check_skip_object || (object_key != except_object_key))) {
                continue;
            };
            DWORD bound_check = msg_brd.AddUser(user_begin, object_key);
            if (bound_check == msg_brd.MAX_USER_COUNT) {
                break;
            };
        }
    };

    if (msg_brd.number_of_users)
    {
        // NOTE: f110531.4L
        m_pHLB->AppendTimeSyncMsg(&msg_brd, msg_brd.GetSendingSize());
        //g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
        return;
    }
}


//==================================================================================================

//_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM

//--------------------------------------------------------------------------------------------------
// CHANGES: f110308.1L, since version 1102, various resolution list had not
// flexible broadcasting anymore. so that the below logic related to surroundings dynamic calculator
// is more simple that is applied an optimization algorithm for sector searching.
const SectorGridVR*
GameField::GetSectorGridVR(const SECTOR_IDX coordinates, const float radius) const
{
    Sector* const sector = m_SectorGrid.GetSector(coordinates);
    if (sector == NULL) {
        return NULL;
    };

    typedef FieldInfo::Resolutions::array_type  array_type;
    const array_type& resolutions = m_pFieldInfo->SectorGridVResolutions;

    const DWORD diameter = static_cast<DWORD>(radius * 2.f);

    const SectorGridVR* sector_grid_vr = &resolutions[FieldInfo::VRES_100_PERCENT];
    if (false) {}
    // NOTE: f110308.1L, added new resolution to use more closed ranges
    else if (diameter <= resolutions[FieldInfo::VRES_800_PERCENT].SectorXYDistance * 3) {
        sector_grid_vr = &resolutions[FieldInfo::VRES_800_PERCENT];
    }
    else if (diameter <= resolutions[FieldInfo::VRES_400_PERCENT].SectorXYDistance * 3) {
        sector_grid_vr = &resolutions[FieldInfo::VRES_400_PERCENT];
    }
    else if (diameter <= resolutions[FieldInfo::VRES_200_PERCENT].SectorXYDistance * 3) {
        sector_grid_vr = &resolutions[FieldInfo::VRES_200_PERCENT];
    }
    return sector_grid_vr;
}

//==================================================================================================

// private method
void GameField::SendPacketAroundForSync_(const SECTOR_IDX coordinates,
        const BYTE _TAG, WAR_TAG_V* const IN war_msg, const WORD war_msg_size,
        BOOLEAN post_action)
{
    __UNUSED(post_action);
    if (coordinates == INVALID_SECTOR_INDEX) {
        return;
    };
    if (m_PlayerHash.empty()) {
        return;
    };
    //
    SectorGrid3x3 OUT grid;
    if (!m_SectorGrid.GetAroundSectors(coordinates, &grid)) {
        return;
    }
    // NOTE: f110531.4L, set to default ''ENABLE_FIELD_HIGH_LOAD_BALANCING
    // redesigned by _NA001605_EP2_CHANGE_SKILL_BROADCASTER_
    WAR_MSG_HDR* const header = m_pHLB->GetHeader();
    WAR_TAG_V* war_msg_in_buffer = NULL;
    // assertion(header_size != 0)
    ;{  // prepare message
        WAR_MSG_HDR::MSG_SIZE_T offset = war_msg_size + header->size;
        if (HighLoadBalancer::kMaxRegionStreamSize <=
            (sizeof(nsGrid::SYNC_MSG_LINKEDLIST) * 9 + offset))
        {
            HighLoadBalancer::Instance()->Update(this, true);
            offset = war_msg_size + header->size;
        };
        war_msg_in_buffer = reinterpret_cast<WAR_TAG_V*>(
            reinterpret_cast<uint8_t*>(header + 1) + header->size);
        CopyMemory(war_msg_in_buffer, war_msg, war_msg_size);
        war_msg_in_buffer->_TAG  = _TAG;
        header->size = offset;
    };
    // assert(war_msg_in_buffer != NULL);

    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        }

        Sector* const sector = m_SectorGrid.AddSectorMsgNode(\
            header, sector_idx, war_msg_in_buffer, war_msg_size);
        __TOUCHED(sector); // for debug
    };
}


//--------------------------------------------------------------------------------------------------


void GameField::SendExPacketToSector(const SECTOR_IDX sector_idx,
        const DWORD number_of_msgs, BYTE** const IN msg_array, WORD* const msg_size_array,
        const DWORD except_object_key)
{
    if (sector_idx == INVALID_SECTOR_INDEX)
        return;

    Sector* const sector = m_SectorGrid.GetSector(sector_idx);
    if (sector->GetPlayerNum() == 0)
        return;

    const bool check_skip_object   = !except_object_key;

    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.InitByArray(msg_array, msg_size_array, number_of_msgs);
    uint16_t* const user_begin  = msg_brd.GetUsersStartPtr();

    ns_object::SearchNodeConstIterator it = sector->GetPlayerRoot();
    const ns_object::SearchNodeConstIterator end = sector->GetPlayerEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Player* player = link_node->select.player_node.player;
        if (obj_util::IsValid(player) == false)
        {
            continue;
        }

        const DWORD object_key = player->GetObjectKey();

        if (!(check_skip_object || (object_key != except_object_key)))
            continue;

        DWORD bound_check = msg_brd.AddUser(user_begin, object_key);
        if (bound_check == msg_brd.MAX_USER_COUNT) {
            break;
        };
    }

    if (msg_brd.number_of_users)
    {
        // NOTE: f110531.4L
        m_pHLB->AppendTimeSyncMsg(&msg_brd, msg_brd.GetSendingSize());
        //g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
        return;
    }
}

/*
void
View::SendExPacketAround(const DWORD number_of_msgs,
                         BYTE** const IN msg_array, WORD* const msg_size_array,
                         const BOOLEAN bSendToMe, const GUILDGUID other_guid_guid)
{
    GameField* const pGameField = m_pOwner->GetField();
    if (!pGameField)
        return;

    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd = MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.InitByArray(msg_array, msg_size_array, number_of_msgs);
    DWORD* const user_begin  = msg_brd.GetUsersStartPtr();
    DWORD bound_check       = msg_brd.MAX_USER_COUNT;

    if (bSendToMe)
        msg_brd.AddUser(user_begin, m_pOwner->GetObjectKey());

    FOREACH_CONTAINER(const view_hash::value_type& rNode, m_ReferedList, view_hash)
    {
        Player* const pReferedPlayer = rNode.second;
        bound_check = msg_brd.AddUser(user_begin, pReferedPlayer->GetObjectKey());
        if (bound_check == msg_brd.MAX_USER_COUNT)
        {
            m_pHLB->AppendTimeSyncMsg(&msg_brd, msg_brd.GetSendingSize());
            msg_brd.InitByArray(msg_array, msg_size_array, number_of_msgs);
        };
    }

    if (msg_brd.m_byUserCount)
    {
        pGameField->SendPacket(m_pOwner, &msg_brd, msg_brd.GetSendingSize()); // GameField를 이용한다.
    }
}
*/
//implemented by __NA001283_20090225_COLLECTION_USE
// (CHANGES) (f100504.1L) change the interface specification to support a complex condition
// the replacement facility for SendXXPacketToSector<IsSendPlayer>
void GameField::SendExPacketToSector(const SECTOR_IDX sector_idx,
        const DWORD number_of_msgs, BYTE** const IN msg_array, WORD* const msg_size_array,
        IObjectFinder* finder)
{
    if (sector_idx == INVALID_SECTOR_INDEX) {
        return;
    }
    Sector* const sector = m_SectorGrid.GetSector(sector_idx);
    if (sector->GetPlayerNum() == 0) {
        return;
    }
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd = \
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.InitByArray(msg_array, msg_size_array, number_of_msgs);
    uint16_t* const player_key_begin = msg_brd.GetUsersStartPtr();
    //
    ns_object::SearchNodeConstIterator it = sector->GetPlayerRoot();
    const ns_object::SearchNodeConstIterator end = sector->GetPlayerEnd();

    while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
    {
        Player* player = link_node->select.player_node.player;
        if (obj_util::IsValid(player) == false)
        {
            continue;
        }
        if (false == finder->operator()(player)) {
            continue;
        }
        //SUNLOG(eDEV_LOG, "[Collect 활성화 정보 보냄] ObjectKey[%d], Name[%s]",
        //       object_key, player->GetCharName());
        const DWORD object_key = player->GetObjectKey();
        DWORD bound_check = msg_brd.AddUser(player_key_begin, object_key);
        if (bound_check == msg_brd.MAX_USER_COUNT) {
            break;
        };
    }

    if (msg_brd.number_of_users)
    {
        // NOTE: f110531.4L
        m_pHLB->AppendTimeSyncMsg(&msg_brd, msg_brd.GetSendingSize());
        //g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
        return;
    }
}

//--------------------------------------------------------------------------------------------------

Character* GameField::SearchTarget(const TargetFinderArg& find_arg)
{
    TargetFinder* target_finder = NULL;

#define FINDER_STATIC_MARK(name)    target_finder = new (stream) name(find_arg);
    
    BYTE stream[max(sizeof(LowHPRatioTargetFinder),
                max(sizeof(LowHPTargetFinder),
                max(sizeof(LowLevelTargetFinder),
                max(sizeof(HighMPTargetFinder),
                max(sizeof(DeadTargetFinder),
                max(sizeof(NearestTargetFinder),
                max(sizeof(UnitIDTargetFinder),
                max(sizeof(MapNpcObjectTargetFinder),
                max(sizeof(NearestMonsterCodeTargetFinder), 0))))))))) ];

    Character* const owner = find_arg.owner_;

    switch (find_arg.search_type_)
    {
    case eRARGET_NOT_SEARCH:
    case eRARGET_SEARCH_NOT_SEARCH_AND_TARGET_CHNAGE: //_NA001385_20090924_DOMINATION_MAPNPC_AI_BUG
        return NULL;
    
    case eRARGET_SEARCH_NONTARGET_LOW_HP:
        if (owner->GetTargetChar())
            return NULL;
        else 
            FINDER_STATIC_MARK(LowHPRatioTargetFinder);

        break;

    case eRARGET_SEARCH_LOW_HPRATIO: FINDER_STATIC_MARK(LowHPRatioTargetFinder); break;
    case eRARGET_SEARCH_LOW_HP:      FINDER_STATIC_MARK(LowHPTargetFinder);      break;
    case eRARGET_SEARCH_LOW_LEVEL:   FINDER_STATIC_MARK(LowLevelTargetFinder);   break;
    case eRARGET_SEARCH_HIGH_MP:     FINDER_STATIC_MARK(HighMPTargetFinder);     break;
    case eRARGET_SEARCH_CORPSE:      FINDER_STATIC_MARK(DeadTargetFinder);       break;
    case eRARGET_SEARCH_UNITID:      FINDER_STATIC_MARK(UnitIDTargetFinder);     break;
    case eRARGET_SEARCH_MAPOBJECTID: FINDER_STATIC_MARK(MapNpcObjectTargetFinder);       break;
    case eRARGET_SEARCH_MONSTERCODE: FINDER_STATIC_MARK(NearestMonsterCodeTargetFinder); break;
    default:                         FINDER_STATIC_MARK(NearestTargetFinder);    break;
    }

    const float sight_range = owner->GetSightRange();

    const SECTOR_IDX coordinates = owner->GetSectorIndex();
    const SectorGridVR* const sector_grid_vr = GetSectorGridVR(coordinates, sight_range);
    const WzVector* const finder_pos = owner->GetPosPtr();

    SectorGrid3x3 OUT grid;
    ViewBoxCubic sector_boundary;
    if (sector_grid_vr->GetAroundSectors(finder_pos, &grid, &sector_boundary) == false) {
        return NULL;
    };
    const WzVector sight_range_vector = {
        sight_range, sight_range, (sight_range < 10.f) ? 5.f : (sight_range / 2) };

    ViewBoxCubic search_boundary;
    search_boundary.SetInit3D(finder_pos, &sight_range_vector);

    Character* found_satisfied_char = NULL;

    //  Center를 먼저 처리하면서 주변 섹터 처리
    GRID3x3_CENTER_PRIOR_SWAP(grid);
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        };
        Sector* const sector = m_SectorGrid.GetSector(sector_idx);

        ns_object::SearchNodeConstIterator it = sector->GetCharacterRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetCharacterEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Character* const character = link_node->select.character_node.character;
            if (obj_util::IsValid(character) == false)
            {
                continue;
            }
            if (character == owner) {
                continue;
            };
            const WzVector* const target_pos = character->GetPosPtr();
            if (!sector_boundary.IsValidCoordinateXY(target_pos)) {
                continue;
            };
            if (!search_boundary.IsValidCoordinateXYZ(target_pos)) {
                continue;
            };

            const StatusField& status_field = character->GetStatusManager()->Status;
            const StatusField::POLICY_T kSkipStatusField =
                status_field.ENABLE_OBSERVER_MODE | 
                status_field.ENABLE_CHARACTER_TRANSPARENT;

            if (bool skipped = (status_field.FIELD & kSkipStatusField) != 0) {
                continue;
            };
            //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
            switch (find_arg.target_type_) 
            {
            case SKILL_TARGET_ENEMY:
            case SKILL_TARGET_AREA:
            case SKILL_TARGET_REACHABLE_ENEMY:
            case SKILL_TARGET_ENEMY_PLAYER:
            case SKILL_TARGET_ENEMY_AND_ME:
                {
                    const bool preemptived =
                        !(status_field.FIELD & status_field.ENABLE_NON_PREEMPTIVE_ATTACK);
                    if (false == preemptived &&
                        false == owner->IsEqualObjectKind(SUMMON_OBJECT) &&
                        false == owner->IsEqualObjectKind(SSQMONSTER_OBJECT))
                    {
                        continue;
                    }
                }
                break;
            };

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
            if (character->IsEqualObjectKind(SUMMON_OBJECT) == true &&
                character->GetSummonType() == eSUMMON_IMMOVABLE)
            {
                continue;
            }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
            if (bool stopped = ((*target_finder)(character)) != false)
            {
                found_satisfied_char = target_finder->GetTarget(); // for debug
                return found_satisfied_char;
            };
        }; //end 'foreach char in sector'
    }; //end 'foreach sector in sectors'
    //
    found_satisfied_char = target_finder->GetTarget();
    return found_satisfied_char;
}


//--------------------------------------------------------------------------------------------------
//#ifdef _NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM

// returns : whether selected targets are exist.
bool GameField::FindTargetsForNpcAI(
        const ns_object::SearchOption::NpcAI& search_option,
        ns_object::SearchOption::NpcAI::result_type* const search_result)
{
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    using namespace ns_object;
    //
    const SECTOR_IDX coordinates = search_option.actor->GetSectorIndex();
    const nsGrid::SectorGridVR* sector_grid_vr =
        GetSectorGridVR(coordinates, search_option.arguments.ranges_of_application);
    if (sector_grid_vr == NULL) {
        return false;
    };
    const ns_object::SearchOption::Arguments& arguments = search_option.arguments;
    nsGrid::SectorGrid3x3 OUT grid;
    ;{
        bool success = sector_grid_vr->GetAroundSectors(
            arguments.application_center_pos, &grid, NULL);
        if (success == false) {
            return false;
        }
    };

    ViewBoxCubic damage_box;
    // CHANGES: f110621.3L, changed the damage box ranges according to the attack range formation
    if (search_option.GetApplicationRangeBox(&damage_box,
            (is_war_castle_ ? 30.0f : 15.0f)) == false)
    {
        return false;
    };
    //
    ;{
        ZeroMemory(search_result, sizeof(*search_result));
    };
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
    struct WxAutoClosure {
        WxAutoClosure(ns_object::SearchOption::NpcAI::result_type* input_search_result)
            : search_result(input_search_result) {}
        ~WxAutoClosure() {
            WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_npc_ai_search", search_result);)) };
        ns_object::SearchOption::NpcAI::result_type* const search_result;
    } wx_closure(search_result);
#endif

    const int& number_of_selected_chars = search_result->number_of_selected_chars;
    //const bool skip_target = (except_object_key != 0);
    GRID3x3_CENTER_PRIOR_SWAP(grid); //  Center를 먼저 처리하면서 주변 섹터 처리
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        };
        if (_countof(search_result->targets) <= number_of_selected_chars) {
            return true;
        };

        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        ns_object::SearchNodeConstIterator it = sector->GetCharacterRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetCharacterEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Character* const target = link_node->select.character_node.character;
            if (obj_util::IsValid(target) == false)
            {
                continue;
            }
            if (search_option.actor == target) {
                continue;
            };
            // shared block
            if (search_option.IsSatisfiedObjectFindingConditions(
                    &damage_box, search_option.actor, target) == false)
            {
                continue;
            }
            if (search_option.IsSatisfiedObjectIFFRelations(
                    search_option.actor, target, NULL, NULL) == false)
            {
                continue;
            }

            if (search_option.AppendToResultIfIsSatisfiedObject(
                    search_option.actor, target, search_result) == search_option.eStop)
            {
                return true;
            }
        }
    };
    //
    return true;
}

bool GameField::FindTargetsUsingSingleSearch(
        const ns_object::SearchOption::SingleSearch& search_option,
        ns_object::SearchOption::SingleSearch::result_type* const search_result)
{
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    using namespace ns_object;
    // NOTE: f110527.4L, set default value to prevent caller don't control this result.
    search_result->number_of_selected_chars = 0;
    //
    const SECTOR_IDX coordinates = search_option.actor->GetSectorIndex();
    const nsGrid::SectorGridVR* sector_grid_vr =
        GetSectorGridVR(coordinates, search_option.arguments.ranges_of_application);
    if (sector_grid_vr == NULL) {
        return false;
    };
    const ns_object::SearchOption::Arguments& arguments = search_option.arguments;
    nsGrid::SectorGrid3x3 OUT grid;
    ;{
        bool success = sector_grid_vr->GetAroundSectors(
            arguments.application_center_pos, &grid, NULL);
        if (success == false) {
            return false;
        }
    };

    ViewBoxCubic damage_box;
    // CHANGES: f110621.3L, changed the damage box ranges according to the attack range formation
    if (search_option.GetApplicationRangeBox(&damage_box,
            (is_war_castle_ ? 30.0f : 15.0f)) == false)
    {
        return false;
    };
    //
    if (search_option.PrefetchResult(search_result) == false) {
        return false;
    };
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
    struct WxAutoClosure {
        WxAutoClosure(ns_object::SearchOption::SingleSearch::result_type* input_search_result)
            : search_result(input_search_result) {}
        ~WxAutoClosure() {
            WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_singlesearch", search_result);)) };
        ns_object::SearchOption::SingleSearch::result_type* const search_result;
    } wx_closure(search_result);
#endif
    //
    const int& number_of_selected_chars = search_result->number_of_selected_chars;
    //const bool skip_target = (except_object_key != 0);
    GRID3x3_CENTER_PRIOR_SWAP(grid); //  Center를 먼저 처리하면서 주변 섹터 처리
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        };
        if (_countof(search_result->targets) <= number_of_selected_chars) {
            return true;
        };

        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        ns_object::SearchNodeConstIterator it = sector->GetCharacterRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetCharacterEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Character* const target = link_node->select.character_node.character;
            if (obj_util::IsValid(target) == false)
            {
                continue;
            }
            if (search_option.actor == target) {
                continue;
            }
            // shared block
            if (search_option.IsSatisfiedObjectFindingConditions(
                    &damage_box, search_option.actor, target) == false)
            {
                continue;
            }
            if (search_option.IsSatisfiedObjectIFFRelations(
                    search_option.actor, target, NULL, NULL) == false)
            {
                continue;
            }

            if (search_option.AppendToResultIfIsSatisfiedObject(
                    search_option.actor, target, search_result) == search_option.eStop)
            {
                return true;
            }
        }
    };
    //
    return true;
}

//--------------------------------------------------------------------------------------------------

void GameField::SendAIMessageAroundExceptMe(NPC* const sender,
        AI_MSG* const ai_msg, const WORD ai_msg_size,
        const DWORD delay)
{
    __UNUSED(delay); // TODO: damduk, 나중에는 사용할 수 있도록 하자.
    if (sender == NULL) {
        return;
    };
    const SECTOR_IDX coordinates = sender->GetSectorIndex();
    SectorGrid3x3 OUT grid;
    if (m_SectorGrid.GetAroundSectors(coordinates, &grid) == false) {
        return;
    };
    //  Center를 먼저 처리하면서 주변 섹터 처리
    GRID3x3_CENTER_PRIOR_SWAP(grid);
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        };
        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        ns_object::SearchNodeConstIterator it = sector->GetNpcRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetNpcEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            NPC* npc = link_node->select.npc_node.npc;
            if (obj_util::IsValid(npc) == false)
            {
                continue;
            }
            if (sender != npc) {
                npc->SendAIMessage(ai_msg, ai_msg_size);
            };
        }
    };

    return;
}


//--------------------------------------------------------------------------------------------------


void GameField::SendExPacketToAll(const DWORD number_of_msgs,
        BYTE** const IN msg_array, WORD* const msg_size_array,
        const DWORD except_object_key)
{
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();
    msg_brd.InitByArray(msg_array, msg_size_array, number_of_msgs);
    uint16_t* const user_begin = msg_brd.GetUsersStartPtr();

    const bool check_skip_object = !except_object_key;
    FOREACH_CONTAINER(const _PLAYER_HASH::value_type& node, m_PlayerHash, _PLAYER_HASH)
    {
        Player* const player = node.second;
        const DWORD object_key = player->GetObjectKey();
        if (!(check_skip_object || (object_key != except_object_key))) {
            continue;
        };
        DWORD bound_check = msg_brd.AddUser(user_begin, object_key);
        if (bound_check == msg_brd.MAX_USER_COUNT) {
            break;
        };
    };

    if (msg_brd.number_of_users)
    {
        // NOTE: f110531.4L
        m_pHLB->AppendTimeSyncMsg(&msg_brd, msg_brd.GetSendingSize());
        //g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    };
}

//--------------------------------------------------------------------------------------------------
// __NA000000_090716_HIGH_LOAD_BALANCED_NETWORK_TUNEUP__
// (WAVERIX) (090716) (ADD-IN) for High-load balanced network support
void GameField::SendExPacketToAgent(const DWORD number_of_msgs,
        BYTE** const IN msg_array, WORD* const msg_size_array)
{
    // NOTE: f110531.4L
    m_pHLB->AppendTimeSyncMsgArray(number_of_msgs, msg_array, msg_size_array);
}

//--------------------------------------------------------------------------------------------------

BOOL GameField::TeleportObject(Character* const character, const WzVector* dest_pos,
                               const BOOLEAN forced)
{
    if (WzVector_FiniteCheck(dest_pos) == false) {
        return false;
    };
    if (IsMovableCoordinates(dest_pos) == false) {
        return false;
    };
    const SECTOR_IDX prev_sector_index = character->GetSectorIndex();
    character->SetPos(dest_pos);
    // f090218.x, waverix, fixed. '잘못된 좌표 설정 문제 수정'
    dest_pos = character->GetPosPtr();

    const StatusField& status_field = character->GetStatusManager()->Status;
    const bool send_to_actor_only = !!(status_field.FIELD & status_field.ENABLE_OBSERVER_MODE);
    const bool is_player = character->IsEqualObjectKind(PLAYER_OBJECT);
    if (is_player)
    {
        Player* const player = static_cast<Player*>(character);
        player->GetMovementChecker()->Reset();
    #ifdef _NA000000_SPEED_HACK_PREVENTION_SUPPORT
        player->GetSyncTuner()->OnWarp();
    #endif
    }

    MSG_CG_MAP_TELEPORT_BRD msg_brd;
    msg_brd.m_dwObjectKey    = character->GetObjectKey();
    msg_brd.vPos             = *dest_pos;
    msg_brd.m_bForcedCommand = forced;

    if (send_to_actor_only) {
        character->SendPacket(&msg_brd, sizeof(msg_brd));
    }
    else {
        SendPacketAround(prev_sector_index, &msg_brd, sizeof(msg_brd));
    };

    const SECTOR_IDX latest_sector_index = GetSectorIndex(dest_pos);
    CheckAndApplyChangeSector(character, latest_sector_index);
    // CHANGES: f110926.1L, 
    if (is_player && (prev_sector_index != latest_sector_index)) {
        static_cast<Player*>(character)->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedPos);
    };
    return true;
}


//--------------------------------------------------------------------------------------------------


RC::eSYNC_RESULT GameField::JumpObject(Character* const player, const WzVector* const dest_pos)
{
    if (WzVector_FiniteCheck(dest_pos) == false) {
        return RC::RC_SYNC_DEST_POSITION_IS_INVALID;
    }
    // (WAVERIX)(080817) add-check
    if (IsMovableCoordinates(dest_pos) == false) {
        return RC::RC_SYNC_DEST_POSITION_IS_INVALID;
    }
    const SECTOR_IDX prev_sector_index = player->GetSectorIndex();
    // (CHANGE)(20100413)(hukim)
    // 평지에서 점프라도 언덕 같은 경우 기울기를 고려 높이차를 허용하고 
    // 경로 탐색기를 사용하여 점프할 경로를 찾을 수 있으면

    const WzVector* const cur_pos = player->GetPosPtr();
    WzVector new_dest_pos = *dest_pos;
    const float height_diff = fabs(cur_pos->z - new_dest_pos.z);
    const float kMaxHeightTolerance = 3.0f;
    bool is_success_set_path = false;

    if ((is_success_set_path = (height_diff < kMaxHeightTolerance)) == true && 
        (is_success_set_path = FindPath(player, &new_dest_pos) != false) == true)
    {
        // (CHANGE)(20100412)(hukim)
        // 경로 탐색기를 사용해서 경로를 설정한다.
        // 순간이동 처럼 즉시 목적지로 현재 위치를 설정하는 것이 아니기 때문에
        // 섹터 변경 처리는 플레이어 상태 갱신 주기 동안 처리한다.

        int jump_tile_route_list[_MAX_PATHROUTE_SIZE];
        CPathExplorer* const path_explorer = player->GetPathExplorer();
        const int number_of_jump_tile_route = path_explorer->GetTileRoute(jump_tile_route_list);
        
        // (CHANGE)(20100413)(hukim)
        // 경로 탐색기가 발견한 경로 수가 최대치를 넘는다는 것은 다른 곳으로 빙둘러서 이동하여
        // 점프 목적지에 도찰할 수 있음을 의미한다. 이런 경우는 점프로 올라갈 수 있는 난간이나
        // 건너갈 수 있는 장애물에 해당 되므로 이전 방식대로 목적지로 바로 이동시킬 필요가 있다.

        enum { kMaxJumpTileRouteTolerance = 7 };
        if ((is_success_set_path = (number_of_jump_tile_route < kMaxJumpTileRouteTolerance)) == true)
        {
            nsAI::MovingPlanner* const moving_planner = GetMovingPlanner();
            is_success_set_path = !!moving_planner->PathPlan_SetPath(player,
                &new_dest_pos, jump_tile_route_list, number_of_jump_tile_route);

            // (CHANGE)(20100412)(hukim)
            // 경로 설정에 실패하면 이렇게 처리하는 것이 정석이라고 생각되지만 이전 플로우와의 호환성과
            // 방어적 목적으로 이전 방식대로 목적지로 바로 이동한다.

            //if (is_success_set_path == false)
            //{
            //    return RC::RC_SYNC_SETPATH_ERROR;
            //}    
        }
    }

    // (CHANGE)(20100412)(hukim)
    // 점프로 올라갈 수 있는 난간이나 점프로 건너갈 수 있는 장애물 같은 경우 경로 탐색에 실패하기 때문에 
    // 경로 탐색기를 사용한 방식으로 실패하면 이전 방식대로 목적지로 순간이동 처럼 바로 이동한다.

    if (is_success_set_path == false)
    {
        player->SetPos(dest_pos);
        new_dest_pos = *(player->GetPosPtr()); // (WAVERIX) (090218) (BUG-FIX) 잘못된 좌표 설정 문제 수정

        const SECTOR_IDX dest_sector = GetSectorIndex(&new_dest_pos);
        CheckAndApplyChangeSector(player, dest_sector);
        // CHANGES: f110926.1L, 
        if (prev_sector_index != dest_sector) {
            static_cast<Player*>(player)->NotifyChangedStatus(PlayerStatEvent::eFlag_ChangedPos);
        };
    }

    return RC::RC_SYNC_SUCCESS;
}

//--------------------------------------------------------------------------------------------------
// to support gm command 'pickup'
bool GameField::PickupAllItems(Player* gm_player, bool use_duplicable_slot_mode)
{
    // (NOTE) skipped all events
    // (WARNING) must be use only for test.
    if (gm_player->IsGMUser() == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|Msg=try request 'pick up all of items', by not GM player|UserID=%s|"),
               gm_player->GetUserID());
        return false;
    }
    //
    GameField* const game_field = gm_player->GetField();
    if (game_field == NULL) {
        return false;
    }
    const SECTOR_IDX center_index = gm_player->GetSectorIndex();
    if (center_index == INVALID_SECTOR_INDEX) {
        return false;
    }
    //
    SectorGrid3x3 OUT grid;
    if (m_SectorGrid.GetAroundSectors(center_index, &grid) == false) {
        return false;
    }
    //
    SCItemSlot* item_slots[nsSlot::ItemQuery::kMaxNumberOfPosSlots] = { 0, };
    Item* items[_countof(item_slots)] = { NULL, };
    MONEY sum_of_money = 0ull;
    int number_of_items = 0;

    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (number_of_items >= _countof(item_slots)) {
            break;
        }
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        }
        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        ns_object::SearchNodeConstIterator it = sector->GetItemRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetItemEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            if (number_of_items >= _countof(item_slots)) {
                break;
            }
            Item* item = link_node->select.item_node.item;
            if (obj_util::IsValid(item) == false)
            {
                continue;
            }
            if (item->IsDeleteReserved()) {
                continue;
            }
            if (item->IsEqualObjectKind(MONEY_OBJECT)) {
                sum_of_money += item->GetMoney();
                // (NOTE) safe operation. request delete, not delete
                game_field->DestroyObject(item);
            }
            else
            {
                SCItemSlot& item_in_item = item->GetItemSlot();
                const nsSlot::ItemRuleInfo& item_rule_info = item_in_item.GetItemRuleInfo();
                if (item_rule_info.is_valid_item != false) {
                    items[number_of_items] = item;
                    item_slots[number_of_items] = &item_in_item;
                    ++number_of_items;
                }
            }
        }
    }; //end 'FOREACH'

    if (sum_of_money != 0)
    {
        gm_player->PlusMoney(sum_of_money);
        ;{
            MSG_CG_ITEM_PICK_MONEY_ACK money_msg;
            money_msg.m_Money = gm_player->GetMoney();
            money_msg.m_BonusRatio = 0;
            gm_player->SendPacket(&money_msg, sizeof(money_msg));
        };
    }
    if (number_of_items == 0) {
        return true;
    }

    const char* error_occurred = NULL;
    ;{ // pick up all items
        using nsSlot::ItemQuery;
        ItemManager* const item_manager = gm_player->GetItemManager();
        //
        MSG_CG_ITEM_PICK_ACK msg_ack;
        ItemQuery pickup_query;
        if (ItemQuery::MakeQueryInverseSelect(&pickup_query, gm_player, SI_INVENTORY) == false) {
            return false;
        }
        SCItemSlot** item_link = item_slots;
        for (int index = 0;
             index < number_of_items && item_link != &item_slots[_countof(item_slots)];
             ++index, ++item_link)
        {
            // CHANGES: f101122.2L, fixed mismatch number of items problem
            // on the duplicated item pickup operation
            SCItemSlot* item_slot = *item_link;
            int registered_index = \
                pickup_query.AddArg_ItemSlot(*item_slot, item_slot->GetNum(), false);
            if (registered_index < 0) {
                error_occurred = "index out of ranges after the calling of the AddArg_ItemSlot";
                break;
            }
        }
        //
        if (use_duplicable_slot_mode == false) {
            pickup_query.ChangeToFindEmptySlotOnlyMode();
        }
        const nsSlot::ItemQueryResult result = item_manager->Query(&pickup_query);
        if (result.result_of_inverse_select == 0) {
            return true; // inventory full
        }
        // TODO(WAVERIX) need more thinking about result handling
        const ItemQuery::QueryInfo& query_info = pickup_query.query_info();
        if (result.result_of_select != query_info.sum_of_queried_items) {
            //error_occurred = "result.result_of_select != query_info.sum_of_queried_items";
            ; // (NOTE) (f100727.2L) can delete process whether a real acquired item
        }
        for (int i = 0; i < query_info.number_of_queries; ++i)
        {
            const nsSlot::ItemQuery::QueryInfo::Node& node = query_info.nodes[i];
            if (node.number_of_items == node.sum_of_items) {
                // (NOTE) safe operation. request delete, not delete
                game_field->DestroyObject(items[i]);
            }
        }
        if (item_manager->ObtainAllKindOfItemByQuery(pickup_query) == false) {
            error_occurred = "item_manager->ObtainAllKindOfItemByQuery";
        }
        pickup_query.MakeSerializedStream(&msg_ack.m_ItemInfo);
        gm_player->SendPacket(&msg_ack, msg_ack.GetSize());
        pickup_query.WriteToGameLogBasedOnResult(gm_player, ITEM_PICK);
    };
    if (error_occurred != NULL) {
        SUNLOG(eCRITICAL_LOG,
               _T("|Msg=the result of the 'pick up items' gm command cause a loss of items|")
               _T("|Detailed=%s|"), error_occurred);
    };
    return true;
}

//--------------------------------------------------------------------------------------------------

// (CHANGES) (f100503.3L) change the interface specification so that 
// a finder has the same of the ObjectFinder<Item> only
NPC* GameField::SearchNeighborNpc(SECTOR_IDX sector_index, DWORD object_key) const
{
    SectorGrid3x3 OUT grid;
    if (m_SectorGrid.GetAroundSectors(sector_index, &grid) == false) {
        return NULL;
    }
    GRID3x3_CENTER_PRIOR_SWAP(grid);
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        }
        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        ns_object::SearchNodeConstIterator it = sector->GetNpcRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetNpcEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            NPC* npc = link_node->select.npc_node.npc;
            if (obj_util::IsValid(npc) == false)
            {
                continue;
            }
            // (NOTE) the logic before 2008 has the modification problem of the list in loop
            // currently the list is not modified.
            const DWORD npc_key = npc->GetObjectKey();
            bool found = (npc_key == object_key);
            if (found == false) {
                continue;
            }
            // finded!
            if (npc->IsDeleteReserved()) {
                return NULL;
            }
            return npc;
        }
    }
    return NULL;
}
// (CHANGES) (f100503.3L) change the interface specification so that 
// a finder has the same of the ObjectFinder<Item> only
NPC* GameField::SearchNeighborNpc(SECTOR_IDX sector_index, const IObjectFinder* filter) const
{
    SectorGrid3x3 OUT grid;
    if (m_SectorGrid.GetAroundSectors(sector_index, &grid) == false) {
        return NULL;
    }
    GRID3x3_CENTER_PRIOR_SWAP(grid);
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        }
        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        ns_object::SearchNodeConstIterator it = sector->GetNpcRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetNpcEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            NPC* npc = link_node->select.npc_node.npc;
            if (obj_util::IsValid(npc) == false)
            {
                continue;
            }
            // (NOTE) the logic before 2008 has the modification problem of the list in loop
            // currently the list is not modified.
            if (npc->IsDeleteReserved()) {
                continue;
            }
            if (filter->operator()(npc)) {
                return npc;
            }
        }
    }
    return NULL;
}

//--------------------------------------------------------------------------------------------------

// (CHANGES) (f100503.3L) change the interface specification so that 
// a finder has the same of the ObjectFinder<Item> only
Item* GameField::SearchNeighborItem(SECTOR_IDX sector_index, DWORD object_key) const
{
    SectorGrid3x3 OUT grid;
    if (m_SectorGrid.GetAroundSectors(sector_index, &grid) == false) {
        return NULL;
    }
    GRID3x3_CENTER_PRIOR_SWAP(grid);
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        }
        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        ns_object::SearchNodeConstIterator it = sector->GetItemRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetItemEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Item* item = link_node->select.item_node.item;
            if (obj_util::IsValid(item) == false)
            {
                continue;
            }
            const DWORD item_key = item->GetObjectKey();
            bool found = item_key == object_key;
            if (found == false) {
                continue;
            }
            // found!
            if (item->IsDeleteReserved()) {
                return NULL;
            }
            return item;
        }
    }
    return NULL;
}
// (CHANGES) (f100503.3L) change the interface specification so that 
// a finder has the same of the ObjectFinder<Item> only
Item* GameField::SearchNeighborItem(SECTOR_IDX sector_index, const IObjectFinder* filter) const
{
    SectorGrid3x3 OUT grid;
    if (m_SectorGrid.GetAroundSectors(sector_index, &grid) == false) {
        return NULL;
    }
    GRID3x3_CENTER_PRIOR_SWAP(grid);
    FOREACH_ARRAY(const SECTOR_IDX sector_idx, grid.Grid, SectorGrid3x3::GridArray)
    {
        if (sector_idx == INVALID_SECTOR_INDEX) {
            continue;
        }
        Sector* const sector = m_SectorGrid.GetSector(sector_idx);
        ns_object::SearchNodeConstIterator it = sector->GetItemRoot();
        const ns_object::SearchNodeConstIterator end = sector->GetItemEnd();

        while (const ns_object::SearchNode::LinkNode* link_node = it.ClimbToNext(end))
        {
            Item* item = link_node->select.item_node.item;
            if (obj_util::IsValid(item) == false)
            {
                continue;
            }
            if (item->IsDeleteReserved()) {
                continue;
            }
            if (filter->operator()(item)) {
                return item;
            }
        }
    }
    return NULL;
}

//--------------------------------------------------------------------------------------------------

void GameZone::SendPacketToAll(MSG_BASE_FORWARD* const msg_base, const WORD msg_size,
                               const DWORD except_object_key)
{
    // NOTE: f110531.5L, it enabled send all user without broadcast boundary
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd = \
        MSG_ONEtoN_BROADCASTER_INTERNAL::InstanceOfBase();

    msg_brd.Init(msg_base, msg_size);
    uint16_t* const user_begin  = msg_brd.GetUsersStartPtr();

    const bool check_skip_object = !except_object_key;
    FOREACH_CONTAINER(const _PLAYER_HASH::value_type& rNode, m_PlayerHash, _PLAYER_HASH)
    {
        Player* const player = rNode.second;
        if (obj_util::IsValid(player, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }

        const DWORD object_key = player->GetObjectKey();
        if (!(check_skip_object || (object_key != except_object_key))) {
            continue;
        };
        DWORD bound_check = msg_brd.AddUser(user_begin, object_key);
        if (bound_check == msg_brd.MAX_USER_COUNT) {
            break;
        };
    }

    if (msg_brd.number_of_users) {
        g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    };
}

