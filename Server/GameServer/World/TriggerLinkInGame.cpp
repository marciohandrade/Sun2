#include "stdafx.h"
#include "Player.h" // internally include header 'TriggerLinkInGame.hxx'
//
#include "TriggerManagerEx.h"
#include "MovingPlanner.h"

//==================================================================================================
//
bool ns_trigger::TriggerPlayerLinkInGame::IsValidState() const
{
    GameField* game_field = (player_ == NULL) ? NULL : player_->GetField();
    if (game_field == NULL) {
        return false;
    };
    eZONE_STATE zone_state = player_->GetGameZoneState();
    if ((zone_state & ePRS_AT_ZONE) != ePRS_AT_ZONE) {
        return false;
    };
    ePLAYER_BEHAVE_STATE behaveState = player_->GetBehave();
    if (behaveState != PLAYER_BEHAVE_IDLE_STATE) {
        return false;
    };
    return true;
}

//==================================================================================================

struct ns_trigger::TriggerPlayerLinkInGameSlots::PoolNode
    : public util::__LList<TriggerPlayerLinkInGameSlots::PoolNode>
{
    ns_trigger::TriggerPlayerLinkInGame link;
};

struct ns_trigger::TriggerPlayerLinkInGameSlots::Pool
{
    typedef ns_trigger::TriggerPlayerLinkInGameSlots::PoolNode Node;

    Pool()
    {
        util::LList::Init(&free_root_);
    }
    ~Pool()
    {
        while (util::LList::IsEmpty(&free_root_) == false)
        {
            Node* it = static_cast<Node*>(free_root_.next);
            util::LList::Delete(it);
            TAllocDelete(Node, it);
        }
    }

    Node* Alloc(Player* player)
    {
        Node* node = NULL;
        if (util::LList::IsEmpty(&free_root_)) {
            node = TAllocNew(Node);
            util::LList::Init(node);
        }
        else {
            node = static_cast<Node*>(free_root_.next);
            util::LList::Delete(node);
        }
        //util::LList::AddPrev(node, &alloc_root_);
        new (&node->link) TriggerPlayerLinkInGame(player);
        return node;
    }

    void Free(Node* node)
    {
        node->link.Delete();
        util::LList::Delete(node);
        util::LList::AddPrev(node, &free_root_);
    }

    Node free_root_;
    __DISABLE_COPY(Pool);
};

ns_trigger::TriggerPlayerLinkInGameSlots::Pool*
ns_trigger::TriggerPlayerLinkInGameSlots::pool_static_ = NULL;

//==================================================================================================
// (CHANGES) (f100614.4L) add a slot type for a trigger area linker to support a multi-area link
ns_trigger::TriggerPlayerLinkInGameSlots::TriggerPlayerLinkInGameSlots(Player* player)
{
    player_ = player;
    util::LList::Init(&linked_areas_);
}

ns_trigger::TriggerPlayerLinkInGameSlots::~TriggerPlayerLinkInGameSlots()
{
    Pool::Node* it = linked_areas_.next;
    for (; it != &linked_areas_; )
    {
        Pool::Node* node = it;
        it = it->next;
        pool_static_->Free(node);
    }
}

ns_trigger::TriggerPlayerLinkInGame*
ns_trigger::TriggerPlayerLinkInGameSlots::GetAreaLink(int area_id)
{
    if (util::LList::IsEmpty(&linked_areas_)) {
        return NULL;
    }
    Pool::Node* it = linked_areas_.next;
    for (; it != &linked_areas_; it = it->next)
    {
        if (it->link.area_id_ == area_id && util::LList::IsEmpty(&it->link) == false) {
            return &it->link;
        }
    }
    return NULL;
}

ns_trigger::TriggerPlayerLinkInGame*
ns_trigger::TriggerPlayerLinkInGameSlots::AllocEmptySlot()
{
    static TriggerPlayerLinkInGameSlots::Pool pool_static;
    pool_static_ = &pool_static;
    Pool::Node* node = pool_static_->Alloc(player_);
    util::LList::AddPrev(node, &linked_areas_);
    return &node->link;
}

void ns_trigger::TriggerPlayerLinkInGameSlots::FreeSlot(TriggerPlayerLinkInGame* area_link)
{
    assert(util::LList::IsEmpty(&linked_areas_) == false);
    Pool::Node* node = CONTAINING_RECORD(area_link, Pool::Node, link);
    pool_static_->Free(node);
}

// execute a replaced delete operation if trigger_manager is equal to null.
void ns_trigger::TriggerPlayerLinkInGameSlots::Disconnect(TriggerManagerEx* trigger_manager)
{
    if (util::LList::IsEmpty(&linked_areas_)) {
        return;
    }
    Pool::Node* it = linked_areas_.next;
    for (; it != &linked_areas_; )
    {
        TriggerPlayerLinkInGame* link = &it->link;
        if (trigger_manager) {
            trigger_manager->Disconnected(link);
        }
        else {
            link->Delete();
        }
        Pool::Node* free_node = it;
        it = it->next;
        pool_static_->Free(free_node);
    }
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

//==================================================================================================
//
bool ns_trigger::TriggerNpcLinkInGame::IsValidState() const
{
    GameField* game_field = (npc_ == NULL) ? NULL : npc_->GetField();
    if (game_field == NULL) {
        return false;
    };
    if (npc_->IsDeleteReserved()) {
        return false;
    };
    eZONE_STATE zone_state = npc_->GetGameZoneState();
    if ((zone_state & ePRS_AT_ZONE) != ePRS_AT_ZONE) {
        return false;
    };

    return true;
}

//==================================================================================================

struct ns_trigger::TriggerNpcLinkInGameSlots::PoolNode
    : public util::__LList<TriggerNpcLinkInGameSlots::PoolNode>
{
    ns_trigger::TriggerNpcLinkInGame link;
};

struct ns_trigger::TriggerNpcLinkInGameSlots::Pool
{
    typedef ns_trigger::TriggerNpcLinkInGameSlots::PoolNode Node;

    Pool()
    {
        util::LList::Init(&free_root_);
    }
    ~Pool()
    {
        while (util::LList::IsEmpty(&free_root_) == false)
        {
            Node* it = static_cast<Node*>(free_root_.next);
            util::LList::Delete(it);
            TAllocDelete(Node, it);
        }
    }

    Node* Alloc(NPC* npc)
    {
        Node* node = NULL;
        if (util::LList::IsEmpty(&free_root_)) {
            node = TAllocNew(Node);
            util::LList::Init(node);
        }
        else {
            node = static_cast<Node*>(free_root_.next);
            util::LList::Delete(node);
        }
        //util::LList::AddPrev(node, &alloc_root_);
        new (&node->link) TriggerNpcLinkInGame(npc);
        return node;
    }

    void Free(Node* node)
    {
        node->link.Delete();
        util::LList::Delete(node);
        util::LList::AddPrev(node, &free_root_);
    }

    Node free_root_;
    __DISABLE_COPY(Pool);
};

ns_trigger::TriggerNpcLinkInGameSlots::Pool*
ns_trigger::TriggerNpcLinkInGameSlots::pool_static_ = NULL;

//==================================================================================================
// CHANGES: f110311.4L add a slot type for a trigger area linker to support a multi-area link
ns_trigger::TriggerNpcLinkInGameSlots::TriggerNpcLinkInGameSlots(NPC* npc)
{
    npc_ = npc;
    util::LList::Init(&linked_areas_);
}

ns_trigger::TriggerNpcLinkInGameSlots::~TriggerNpcLinkInGameSlots()
{
    Pool::Node* it = linked_areas_.next;
    for (; it != &linked_areas_; )
    {
        Pool::Node* node = it;
        it = it->next;
        pool_static_->Free(node);
    }
}

ns_trigger::TriggerNpcLinkInGame*
ns_trigger::TriggerNpcLinkInGameSlots::GetAreaLink(int area_id)
{
    if (util::LList::IsEmpty(&linked_areas_)) {
        return NULL;
    }
    Pool::Node* it = linked_areas_.next;
    for (; it != &linked_areas_; it = it->next)
    {
        if (it->link.area_id_ == area_id && util::LList::IsEmpty(&it->link) == false) {
            return &it->link;
        }
    }
    return NULL;
}

ns_trigger::TriggerNpcLinkInGame*
ns_trigger::TriggerNpcLinkInGameSlots::AllocEmptySlot()
{
    static TriggerNpcLinkInGameSlots::Pool pool_static;
    pool_static_ = &pool_static;
    Pool::Node* node = pool_static_->Alloc(npc_);
    util::LList::AddPrev(node, &linked_areas_);
    return &node->link;
}

void ns_trigger::TriggerNpcLinkInGameSlots::FreeSlot(TriggerNpcLinkInGame* area_link)
{
    assert(util::LList::IsEmpty(&linked_areas_) == false);
    Pool::Node* node = CONTAINING_RECORD(area_link, Pool::Node, link);
    pool_static_->Free(node);
}

// execute a replaced delete operation if trigger_manager is equal to null.
void ns_trigger::TriggerNpcLinkInGameSlots::Disconnect(TriggerManagerEx* trigger_manager)
{
    if (util::LList::IsEmpty(&linked_areas_)) {
        return;
    }
#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
    // temporary block
    Pool::Node* it = linked_areas_.next;
    for (; it != &linked_areas_; )
    {
        TriggerNpcLinkInGame* link = &it->link;
        if (trigger_manager) {
            TriggerAreaManagerForNpc* trigger_area_manager =
                const_cast<TriggerAreaManagerForNpc*>(trigger_manager->GetAreaManagerForNpc());
            TriggerAreaForNpc::ForceLeave(trigger_area_manager, link);
        }
        else {
            link->Delete();
        }
        Pool::Node* free_node = it;
        it = it->next;
        pool_static_->Free(free_node);
    }
#endif
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

ns_trigger::TriggerNpcLinkMatchTable::AreaID_Node::AreaID_Node()
{
    util::LList::Init(this);
}

ns_trigger::TriggerNpcLinkMatchTable::AreaID_Node::~AreaID_Node()
{
    util::LList::Delete(this);
}

void ns_trigger::TriggerNpcLinkMatchTable::AreaID_Node::MakeToRootNode()
{
    util::LList::Init(this);
    area_id = -1;
    special_area = NULL;
}

//==================================================================================================

ns_trigger::TriggerNpcLinkMatchTable::TriggerNpcLinkMatchTable()
    : game_field_(NULL)
{
}

ns_trigger::TriggerNpcLinkMatchTable::~TriggerNpcLinkMatchTable()
{
    area_event_containing_units_.clear();
    area_event_containing_monster_code_.clear();
    //
    FOREACH_CONTAINER(TILE_AREA_MATCHING_TABLE::value_type& node, tile_area_matching_table_,
                      TILE_AREA_MATCHING_TABLE)
    {
        AreaID_Node* const root = &node.second;
        AreaID_Node* it = root->next;
        while (it != root)
        {
            // for safe iteration
            AreaID_Node* temp = it->next;
            TAllocDelete(AreaID_Node, it);
            it = temp;
        }
    };
    tile_area_matching_table_.clear();
}

#ifdef _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE

void ns_trigger::TriggerNpcLinkMatchTable::Init(GameField* game_field)
{
    game_field_ = game_field;
    area_event_containing_units_.clear();
    area_event_containing_monster_code_.clear();
    tile_area_matching_table_.clear();
}

bool ns_trigger::TriggerNpcLinkMatchTable::IsExistTriggerAreaEvent() const
{
    if (game_field_ == NULL) {
        return false;
    };

    if (area_event_containing_units_.empty() &&
        area_event_containing_monster_code_.empty()) {
        return false;
    };
    if (tile_area_matching_table_.empty()) {
        return false;
    }
    return true;
}

void ns_trigger::TriggerNpcLinkMatchTable::CreateEventNpcTriggerAreaEvents(int area_id)
{
    if (game_field_ == NULL) {
        return;
    };

    CWorldBase* const world_base = game_field_->GetFieldInfo()->GetWorldBase();
    const int number_of_special_areas = world_base->GetNumberOfSpecialArea();
    WzSpecialArea* special_area = NULL;
    if (!FlowControl::FCAssert(area_id >= 0 && area_id < number_of_special_areas &&
            (special_area = world_base->GetSpecialAreaByIndex(area_id)) != NULL))
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=can't find special area (index = %d)"),
               area_id);
        return;
    }

    const WzBoundingVolume& bounding_volume = special_area->m_wbvRegion;
    if (bounding_volume.m_eBvType != BVT_PATH_TILE_INDEX) {
        return; //
    };

    nsAI::MovingPlanner* const moving_planner = game_field_->GetMovingPlanner();
    const BvPathTileIndex& path_tiles = bounding_volume.m_BvPathTileIndex;
    // apply tile event tag in the game field
    const DWORD* tile_it = path_tiles.m_pdwData,
               * const tile_end = &path_tiles.m_pdwData[path_tiles.m_nCount];
    for ( ; tile_it != tile_end; ++tile_it)
    {
        const int tile_no = *tile_it;
        //const nsAI::TileEvent* tile_event;
        const nsAI::eCharEvent char_event = nsAI::eCharEvent_ExistNpcTileEvent;
        //
        const nsAI::PlanCmd::NpcAreaEvent args(nsAI::PlanCmd::eNpcAreaEvent_Regist,
                                               tile_no, char_event);
        moving_planner->PathPlan_Command(args);
        //
        //
        TILE_AREA_MATCHING_TABLE::iterator exist_it = tile_area_matching_table_.find(tile_no);
        const bool first_entry = (exist_it == tile_area_matching_table_.end());
        AreaID_Node* const root = &tile_area_matching_table_[tile_no];
        if (first_entry) {
            root->MakeToRootNode();
        };

        ;{
            const AreaID_Node* it = root->next;
            for ( ; it != root; it = it->next)
            {
                if (it->area_id == area_id) {
                    break;
                };
            }

            if (it == root)
            {   // not found
                AreaID_Node* const new_item = TAllocNew(AreaID_Node);
                new_item->area_id = area_id;
                new_item->special_area = special_area;
                util::LList::AddPrev(new_item, root);
            }
        };
    }
}

void ns_trigger::TriggerNpcLinkMatchTable::AddEventNpcTriggerAreaEvents(
    int area_id, CODETYPE unit_id)
{
    CreateEventNpcTriggerAreaEvents(area_id);
    //-------------------------------------------------------
    ;{
        AREA_EVENT_CONTAINING_UNITS::const_iterator exist =
            area_event_containing_units_.find(unit_id);
        if (exist == area_event_containing_units_.end()) {
            area_event_containing_units_.insert(unit_id);
        }
    };
}

void ns_trigger::TriggerNpcLinkMatchTable::AddEventMonsterTriggerAreaEvents(
    int area_id, CODETYPE monster_code)
{
    CreateEventNpcTriggerAreaEvents(area_id);
    //-------------------------------------------------------
    ;{
        AREA_EVENT_CONTAINING_UNITS::const_iterator exist =
            area_event_containing_monster_code_.find(monster_code);
        if (exist == area_event_containing_monster_code_.end()) {
            area_event_containing_monster_code_.insert(monster_code);
        }
    };
}

//==================================================================================================

namespace ns_trigger {
;
struct TriggerAreaEventSelector
{
    TriggerAreaEventSelector(NPC* input_npc);
    void ProcessEvent();
    //
    NPC* npc;
    //
    int number_of_prev_areas;
    int number_of_post_areas;
    int number_of_leave_events;
    int number_of_enter_events;

    CODETYPE prev_areas[16];
    CODETYPE post_areas[16];
    CODETYPE leave_events[16];
    CODETYPE enter_events[16];
};

}; //end of namespace
//
ns_trigger::TriggerAreaEventSelector::TriggerAreaEventSelector(NPC* input_npc)
{
    ZeroMemory(this, sizeof(*this));
    npc = input_npc;
}

void ns_trigger::TriggerAreaEventSelector::ProcessEvent()
{
    ;{
        CODETYPE* dest_it = leave_events;
        //
        CODETYPE* left_it = prev_areas;
        const CODETYPE* const left_end = &prev_areas[_countof(prev_areas)];
        //
        for ( ; left_it != left_end && *left_it; ++left_it)
        {
            bool found = false;
            for (CODETYPE* right_it = post_areas;
                    right_it != &post_areas[_countof(post_areas)] && *right_it;
                    ++right_it)
            {
                if ((*right_it) == (*left_it)) {
                    found = true;
                    break;
                }
            }
            if (found == false) {
                *dest_it++ = *left_it;
            }
        }
    };
    ;{
        CODETYPE* dest_it = enter_events;
        //
        CODETYPE* left_it = post_areas;
        const CODETYPE* const left_end = &post_areas[_countof(post_areas)];
        //
        const CODETYPE* const right_end = &prev_areas[_countof(prev_areas)];
        for ( ; left_it != left_end && *left_it; ++left_it)
        {
            bool found = false;
            for (CODETYPE* right_it = prev_areas;
                    right_it != right_end && *right_it;
                    ++right_it)
            {
                if ((*right_it) == (*left_it)) {
                    found = true;
                    break;
                }
            }
            if (found == false) {
                *dest_it++ = *left_it;
            }
        }
    };
    //
    GameField* const game_field = npc->GetField();
    TriggerManagerEx* const trigger_manager = game_field->GetTriggerManager();
    ns_trigger::TriggerNpcLinkInGameSlots* const trigger_links = npc->GetTriggerLinks();

    ;{
        TRIGGER_NPC_LEAVE_AREA msg;
        msg.object_key = npc->GetObjectKey();
        //msg.area_id = 
        //msg.link =

        CODETYPE* leave_it = leave_events;
        const CODETYPE* const leave_end = &leave_events[_countof(leave_events)];
        for ( ; leave_it != leave_end && *leave_it; ++leave_it)
        {
            msg.area_id = (*leave_it);
            msg.link = trigger_links->GetAreaLink(msg.area_id);
            if (msg.link == NULL) {
                continue;
            }
            trigger_manager->OnLeaveAreaForNpc(&msg);
            trigger_links->FreeSlot(
                static_cast<ns_trigger::TriggerNpcLinkInGame*>(msg.link));
        }
    };
    ;{
        TRIGGER_NPC_ENTER_AREA msg;
        msg.object_key = npc->GetObjectKey();
        //msg.area_id = 
        //msg.link =

        CODETYPE* enter_it = enter_events;
        const CODETYPE* const enter_end = &enter_events[_countof(enter_events)];
        for ( ; enter_it != enter_end && *enter_it; ++enter_it)
        {
            msg.area_id = (*enter_it);
            msg.link = trigger_links->AllocEmptySlot();
            if (trigger_manager->OnEnterAreaForNpc(&msg) == false) {
                trigger_links->FreeSlot(
                    static_cast<ns_trigger::TriggerNpcLinkInGame*>(msg.link));
            };
        }

    };
};

//==================================================================================================
//
bool ns_trigger::TriggerNpcLinkMatchTable::OnChangeTile(
    int prev_tile_no, int new_tile_no, NPC* npc) const
{
    if (prev_tile_no == new_tile_no) {
        return false;
    };

    // NOTE: assumed area_event_containing_units_ include the inputed npc
    TriggerAreaEventSelector event_selector(npc);
    //
    TILE_AREA_MATCHING_TABLE::const_iterator prev_it = tile_area_matching_table_.find(prev_tile_no);
    if (prev_it != tile_area_matching_table_.end())
    {
        const AreaID_Node* const root = &prev_it->second;
        AreaID_Node* area_it = root->next;
        CODETYPE* dest_it = event_selector.prev_areas;
        const CODETYPE* const dest_end = &event_selector.prev_areas[
            _countof(event_selector.prev_areas)];
        for ( ; area_it != root; area_it = area_it->next)
        {
            *dest_it++ = area_it->area_id;
            if (dest_it == dest_end)
            {
                ASSERT(!"too many duplicated areas");
                break;
            }
        }
        event_selector.number_of_prev_areas =
            static_cast<int>(dest_it - event_selector.prev_areas);
    };
    TILE_AREA_MATCHING_TABLE::const_iterator post_it = tile_area_matching_table_.find(new_tile_no);
    if (post_it != tile_area_matching_table_.end())
    {
        const AreaID_Node* const root = &post_it->second;
        AreaID_Node* area_it = root->next;
        CODETYPE* dest_it = event_selector.post_areas;
        const CODETYPE* const dest_end = &event_selector.post_areas[
            _countof(event_selector.post_areas)];
        for ( ; area_it != root; area_it = area_it->next)
        {
            *dest_it++ = area_it->area_id;
            if (dest_it == dest_end)
            {
                ASSERT(!"too many duplicated areas");
                break;
            }
        }
        event_selector.number_of_post_areas =
            static_cast<int>(dest_it - event_selector.post_areas);
    };
    //
    event_selector.ProcessEvent(); //enter & leave trigger events
    //
    return true;
}

// the npc area event check interface
bool ns_trigger::TriggerNpcLinkMatchTable::CheckExistAreaEvent(CODETYPE unit_id) const
{
    AREA_EVENT_CONTAINING_UNITS::const_iterator exist =
        area_event_containing_units_.find(unit_id);
    if (exist == area_event_containing_units_.end()) {
        return false;
    }
    return true;
}

bool ns_trigger::TriggerNpcLinkMatchTable::CheckExistMonsterAreaEvent( CODETYPE monster_code ) const
{
    AREA_EVENT_CONTAINING_UNITS::const_iterator exist =
        area_event_containing_monster_code_.find(monster_code);
    if (exist == area_event_containing_monster_code_.end()) {
        return false;
    }
    return true;
}

#endif //
//==================================================================================================
