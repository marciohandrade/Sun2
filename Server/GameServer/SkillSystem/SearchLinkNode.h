#pragma once
#ifndef GAMESERVER_SEARCH_LINK_NODE_H
#define GAMESERVER_SEARCH_LINK_NODE_H

//==================================================================================================
#include "SectorGrid.h"
using namespace nsGrid; // the nsGrid namespace is used to a working space. change to globally access.

namespace ns_object {
;

struct IRelation;
struct IFF;

//--------------------------------------------------------------------------------------------------

struct SearchNode
{
    static const eOBJECT_TYPE kNullObject = static_cast<eOBJECT_TYPE>(0);
    //------------------------------------------
    struct SharedNode
    {
        eOBJECT_TYPE object_type;  // Object Type(Player, Monster, FieldItem, NPC, etc)
        bool delete_reserved; // __NA000000_090310_GAMEFIELD_RECURSIVELY_CALL_PROBLEM_FIX__
        //
        DWORD object_key;
        GameField* game_field; // current game field
        const WzVector* object_position; // current object position in a game field
        SECTOR_IDX sector_index_current; // current sector index
        SECTOR_IDX sector_index_previous;// previous sector index
        const IRelation* relation;
    };
    //------------------------------------------
    struct ObjectNode : public SharedNode
    {
        Object* object;
        int* __padding0;
    };
    struct CharacterNode : public SharedNode
    {
        Character* character;
        int* current_hp;
    };
    struct PlayerNode : public SharedNode
    {
        Player* player;
        int* current_hp;
    };
    struct NpcNode : public SharedNode
    {
        NPC* npc;
        int* current_hp;
    };
    struct ItemNode : public SharedNode
    {
        Item* item;
        int* __padding0;
    };
    //------------------------------------------
    union UnionNode
    {
        SharedNode root_node;
        ObjectNode object_node;
        CharacterNode character_node;
        PlayerNode player_node;
        NpcNode npc_node;
        ItemNode item_node;
    };
    //------------------------------------------
    struct LinkNode : public util::__LList<LinkNode>
    {
        //
        UnionNode select;
    };
    //------------------------------------------
};

//--------------------------------------------------------------------------------------------------

struct SearchReferences
{
    int* hp;
};

//--------------------------------------------------------------------------------------------------

struct SearchNodeConstIterator
{
    const SearchNode::LinkNode* ClimbToNext(const SearchNodeConstIterator& end_it) const;
    const SearchNode::LinkNode* node;
};


inline const SearchNode::LinkNode*
SearchNodeConstIterator::ClimbToNext(const SearchNodeConstIterator& end_it) const
{
    while (this->node != end_it.node)
    {
        const SearchNode::LinkNode* cur_node = node;
        const_cast<SearchNodeConstIterator*>(this)->node = cur_node->next;

        if (cur_node->select.root_node.object_type != SearchNode::kNullObject) {
            return cur_node;
        }
    }
    return NULL;
};

//--------------------------------------------------------------------------------------------------

}; //end of namespace

//==================================================================================================

#endif //GAMESERVER_SEARCH_LINK_NODE_H