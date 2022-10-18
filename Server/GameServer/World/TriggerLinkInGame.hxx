#ifndef __GAMESERVER_PLAYER_INL
    #error "Can't direct include this header file"
#endif
#pragma once
#ifndef GAMESERVER_TRIGGER_LINK_IN_GAME_HXX
#define GAMESERVER_TRIGGER_LINK_IN_GAME_HXX

namespace ns_trigger {
;

//==================================================================================================
//
class TriggerPlayerLinkInGame : public TriggerPlayerLink
{
public:
    TriggerPlayerLinkInGame() : player_(0) {}
    TriggerPlayerLinkInGame(Player* player)
        : player_(player)
    {}
    virtual ~TriggerPlayerLinkInGame() {}
    virtual bool IsValidState() const;

public: // to public
    Player* const player_;
    __DISABLE_COPY(TriggerPlayerLinkInGame);
};

// (CHANGES) (f100614.4L) add a slot type for a trigger area linker to support a multi-area link
class TriggerPlayerLinkInGameSlots
{
public:
    static const int kMaxNumberOfContains = 4;
    TriggerPlayerLinkInGameSlots() {
        player_ = NULL;
        util::LList::Init(&linked_areas_);
    }
    TriggerPlayerLinkInGameSlots(Player* player);
    ~TriggerPlayerLinkInGameSlots();

    TriggerPlayerLinkInGame* GetAreaLink(int area_id);
    TriggerPlayerLinkInGame* AllocEmptySlot();
    void FreeSlot(TriggerPlayerLinkInGame* area_link);
    // execute a replaced delete operation if trigger_manager is equal to null.
    void Disconnect(TriggerManagerEx* trigger_manager);
private:
    struct Pool;
    struct PoolNode;
    PoolNode* AllocNode();
    void FreeNode(PoolNode* node);

    Player* player_;
    util::__LList<PoolNode> linked_areas_;
    static Pool* pool_static_;
};

//==================================================================================================

class TriggerNpcLinkInGame : public TriggerNpcLink
{
public:
    TriggerNpcLinkInGame() : npc_(0) {}
    TriggerNpcLinkInGame(NPC* npc)
        : npc_(npc)
    {}
    virtual ~TriggerNpcLinkInGame() {}
    virtual bool IsValidState() const;

public: // to public
    NPC* const npc_;
    __DISABLE_COPY(TriggerNpcLinkInGame);
};

// CHANGES: f110311.4L, add a slot type for a trigger area linker to support a multi-area link.
// declared by _NA_002098_20110307_ADD_NPC_CONTROL_TRIGGER_BASE
class TriggerNpcLinkInGameSlots
{
public:
    static const int kMaxNumberOfContains = 4;
    TriggerNpcLinkInGameSlots() {
        npc_ = NULL;
        util::LList::Init(&linked_areas_);
    }
    TriggerNpcLinkInGameSlots(NPC* npc);
    ~TriggerNpcLinkInGameSlots();

    TriggerNpcLinkInGame* GetAreaLink(int area_id);
    TriggerNpcLinkInGame* AllocEmptySlot();
    void FreeSlot(TriggerNpcLinkInGame* area_link);
    // execute a replaced delete operation if trigger_manager is equal to null.
    void Disconnect(TriggerManagerEx* trigger_manager);
private:
    struct Pool;
    struct PoolNode;
    PoolNode* AllocNode();
    void FreeNode(PoolNode* node);

    NPC* npc_;
    util::__LList<PoolNode> linked_areas_;
    static Pool* pool_static_;
};

class TriggerNpcLinkMatchTable
{
public:
    struct AreaID_Node : public util::__LList<AreaID_Node>
    {
        AreaID_Node();
        ~AreaID_Node();
        void MakeToRootNode();
        int area_id;
        WzSpecialArea* special_area;
    };
    //
    TriggerNpcLinkMatchTable();
    ~TriggerNpcLinkMatchTable();

    void Init(GameField* game_field);
    bool IsExistTriggerAreaEvent() const;
    // the trigger event notification interface
    void CreateEventNpcTriggerAreaEvents(int area_id);
    void AddEventNpcTriggerAreaEvents(int area_id, CODETYPE unit_id);
    void AddEventMonsterTriggerAreaEvents( int area_id, CODETYPE monster_code);

    // the npc area event check interface
    bool CheckExistAreaEvent(CODETYPE unit_id) const;
    bool CheckExistMonsterAreaEvent(CODETYPE monster_code) const;
    //
    bool OnChangeTile(int prev_tile_no, int new_tile_no, NPC* npc) const;
    //
private:
    //                    Unit ID
    typedef STLX_HASH_SET<CODETYPE> AREA_EVENT_CONTAINING_UNITS;
    //                     TileNo    AreaID
    typedef STLX_HASH_MAP<CODETYPE, AreaID_Node> TILE_AREA_MATCHING_TABLE;
    //
    GameField* game_field_;
    AREA_EVENT_CONTAINING_UNITS area_event_containing_units_;
    AREA_EVENT_CONTAINING_UNITS area_event_containing_monster_code_;
    TILE_AREA_MATCHING_TABLE tile_area_matching_table_;
};

//==================================================================================================
}; //end of namespace

#endif //GAMESERVER_TRIGGER_LINK_IN_GAME_H