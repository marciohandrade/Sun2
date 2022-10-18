// Object.cpp: implementation of the Object class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Object.h"
#include "GameField.h"

//==================================================================================================
//
const WzVector Object::kNullVector = { 0.0f, 0.0f, 0.0f };
//
//==================================================================================================
//
Object::Object()
    : object_node_(&link_node_.select.object_node), is_deleted_(false)
{
    ZeroMemory(&link_node_, sizeof(link_node_));
    util::LList::Init(&link_node_);
    ;{
        object_node_->object_type = OBJECT_OBJECT;
        object_node_->sector_index_current = INVALID_SECTOR_INDEX;
        object_node_->sector_index_previous = INVALID_SECTOR_INDEX;
        object_node_->object = this;
    };

    Init();
    object_node_->sector_index_previous = INVALID_SECTOR_INDEX;
}

//virtual
Object::~Object()
{
    util::LList::Delete(&link_node_);
    is_deleted_ = true;
}

void Object::_RegisterSearchReference(const ns_object::SearchReferences& search_reference)
{
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    using namespace ns_object;
    // NOTE: must be process after call SetObjectType
    switch (object_node_->object_type)
    {
    case PLAYER_OBJECT:
        if (SearchNode::PlayerNode* node = &link_node_.select.player_node)
        {
            node->current_hp = search_reference.hp;
        }
        break;
    case NPC_OBJECT:
    //case MONSTER_OBJECT:
    //case SUMMON_OBJECT:
    //case MAPNPC_OBJECT:
    //case LUCKYMON_OBJECT:
    //case TOTEMNPC_OBJECT:
    //case SSQMONSTER_OBJECT:
    //case CRYSTALWARP_OBJECT:
    //case FRIEND_MONSTER_OBJECT: //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    //case SYNC_MERCHANT_OBJECT: //_NA_0_20100222_UNIT_TRIGGERS_REGEN
        if (SearchNode::NpcNode* node = &link_node_.select.npc_node)
        {
            node->current_hp = search_reference.hp;
        }
        break;
    //case COLLECTION_OBJECT:
    //    if (SearchNode::ObjectNode* node = &link_node_.select.object_node)
    //    {
    //        __TOUCHED(node);
    //    }
    //    break;
    //case MONEY_OBJECT:
    //case ITEM_OBJECT:
    //    if (SearchNode::ItemNode* node = &link_node_.select.item_node)
    //    {
    //        __TOUCHED(node);
    //    }
    //    break;
    //
    };
}
//
//==============================================================================================

namespace obj_util {

bool IsValid(const Object* const obj, const char* function_str, const int line_number)
{
    if (obj == NULL)
    {
        return false;
    }
    if (obj->IsDeleted())
    {
        if (function_str != NULL)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                function_str, 
                line_number, 
                obj->GetObjectKey()
            );
        }
        return false;
    }
    return true;
}

} // obj_util