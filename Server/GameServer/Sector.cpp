#include "stdafx.h"
#include "Sector.h"
#include "AIParamParser.h"
#include "PublicMath.h"
#include "Player.h"
#include "GameServerEx.h"


Sector::Sector()
    : sector_index_(INVALID_SECTOR_INDEX)
    , number_of_references_(0)
    , number_of_players_(0)
{
    ZeroMemory(&delimiter0_, sizeof(delimiter0_));
    util::LList::Init(&player_root_);
    ZeroMemory(&delimiter1_, sizeof(delimiter1_));
    util::LList::Init(&npc_root_);
    ZeroMemory(&delimiter2_, sizeof(delimiter2_));
    util::LList::Init(&item_root_);
    ZeroMemory(&delimiter3_, sizeof(delimiter3_));
    util::LList::Init(&etc_root_);
}

Sector::~Sector()
{
    ASSERT(util::LList::IsEmpty(&player_root_) &&
           util::LList::IsEmpty(&npc_root_) &&
           util::LList::IsEmpty(&item_root_) &&
           util::LList::IsEmpty(&etc_root_));
}


void Sector::Init(DWORD sector_index)
{
    sector_index_ = sector_index;
    number_of_references_ = 0;
    number_of_players_ = 0;
    //
    ;{
        util::LList::Delete(&player_root_);
        util::LList::Delete(&npc_root_);
        util::LList::Delete(&item_root_);
        util::LList::Delete(&etc_root_);
        //
        util::LList::AddNext(&npc_root_, &player_root_);
        util::LList::AddNext(&item_root_, &npc_root_);
        util::LList::AddNext(&etc_root_, &item_root_);
        assert(&player_end_ == &npc_root_ &&
               &npc_end_ == &item_root_ &&
               &item_end_ == &etc_root_ &&
               &etc_end_ == &player_root_);
        // Player -> Npc -> Item -> Etc -> (cycle) Player
    };
}

void Sector::Release()
{
    using namespace ns_object;
    sector_index_ = INVALID_SECTOR_INDEX;
    number_of_references_ = 0;
    number_of_players_ = 0;
    //
    SearchNode::LinkNode* link_node = static_cast<SearchNode::LinkNode*>(&player_root_);
    while (SearchNode::LinkNode* next_node = util::LList::Delete(link_node))
    {
        if (util::LList::IsEmpty(next_node)) {
            break;
        }
        link_node = next_node;
    }
}

bool Sector::IsExistInSector(Object* object) const
{
    if (object == NULL) {
        return false;
    };
    SECTOR_IDX sector_index = object->GetSectorIndex();
    if (this->sector_index_ != sector_index) {
        return false;
    }
    assert(util::LList::IsEmpty(object->link_node_.next) == false);
    return true;
}

RC::eSECTOR_REULT Sector::EnterObject(Object* object)
{
    if (this->Add(object) == false) {
        return RC::RC_SECTOR_FAILED;
    }

    //Player의 OnLeaveObject()의 맨처음에서 enter_ack를 처리하면 됨 : 이동
    object->OnEnterSector();   //< enter_ack처리

    OnEnterThisToNeighbor(object);
    OnEnterNeighborToThis(object);

    return RC::RC_SECTOR_SUCCESS;
}

RC::eSECTOR_REULT Sector::AddObject(Object* object)
{
    if (this->Add(object) == false) {
        return RC::RC_SECTOR_FAILED;
    }

    return RC::RC_SECTOR_SUCCESS;
}

RC::eSECTOR_REULT Sector::LeaveObject(Object* object)
{
    if (this->Remove(object) == false) {
        return RC::RC_SECTOR_FAILED;
    }

    OnLeaveThisToNeighbor(object);

    //OnLeaveNeighborToThis(object); //< 주변 정보 자신에게 안보냄 클라에서 초기화(클리어)

    return RC::RC_SECTOR_SUCCESS;
}

RC::eSECTOR_REULT Sector::RemoveObject(Object* object)
{
    if (this->Remove(object) == false) {
        return RC::RC_SECTOR_FAILED;
    }

    OnLeaveThisToNeighbor(object);

    OnLeaveNeighborToThis(object);

    return RC::RC_SECTOR_SUCCESS;
}

void Sector::Update(DWORD delta_tick)
{
}

void Sector::UpdateReferenceCount(Object* object, BOOL entered)
{
    const eOBJECT_TYPE object_type = object->object_node_->object_type;
    if ((object_type & PLAYER_OBJECT) == PLAYER_OBJECT)
    {
        if (false) {
        }
        else if (entered) {
            ++number_of_references_;
        }
        else if (number_of_references_) {
            --number_of_references_;
        }
    }
}

bool Sector::Add(Object* object)
{
    if (!FlowControl::FCAssert(util::LList::IsEmpty(&object->link_node_))) {
        return false;
    };
    //
    ns_object::SearchNode::LinkNode* root = NULL;
    const eOBJECT_TYPE object_type = object->GetObjectType();
    switch (object_type)
    {
    case PLAYER_OBJECT:
        root = static_cast<ns_object::SearchNode::LinkNode*>(&player_root_);
        ++number_of_players_;
        break;
    case NPC_OBJECT:
    case MONSTER_OBJECT:
    case SUMMON_OBJECT:
    case MAPNPC_OBJECT:
    case LUCKYMON_OBJECT:
    case TOTEMNPC_OBJECT:
    case SSQMONSTER_OBJECT:
    case CRYSTALWARP_OBJECT:
    case FRIEND_MONSTER_OBJECT: //_NA_0_20100222_UNIT_TRIGGERS_FRIEND_MONSTER
    case SYNC_MERCHANT_OBJECT: //_NA_0_20100222_UNIT_TRIGGERS_REGEN
        root = static_cast<ns_object::SearchNode::LinkNode*>(&npc_root_);
        break;
    case COLLECTION_OBJECT:
        root = static_cast<ns_object::SearchNode::LinkNode*>(&etc_root_);
        break;
    case MONEY_OBJECT:
    case ITEM_OBJECT:
        root = static_cast<ns_object::SearchNode::LinkNode*>(&item_root_);
        break;
    }

    if (root == NULL) {
        return false;
    };

    util::LList::AddNext(&object->link_node_, root);
    //
    return true;
}

bool Sector::Remove(Object* object)
{
    if (!FlowControl::FCAssert(util::LList::IsEmpty(&object->link_node_) == false)) {
        return false;
    };
    //
    const eOBJECT_TYPE object_type = object->GetObjectType();

    if ((object_type & PLAYER_OBJECT) == PLAYER_OBJECT)
    {
        if (number_of_players_ != 0) {
            --number_of_players_;
        };
    }
    else
    {
        // nothing to do
    }

    util::LList::Delete(&object->link_node_);
    //
    return true;
}

