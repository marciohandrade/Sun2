#include "stdafx.h"
#include "./DropManager.h"

//==================================================================================================

#include <SCItemSlot.h>

#include "GameZone.h"
#include "GameField.h"

#include "StatusManager.h"

#include "DropRatioInfoList.h"

//==================================================================================================
//1. 깊이 우선 탐색으로 구현해 보았다.
//2. 재귀로 구현하였으나, 속도가 느리므로 비재귀로 수정하여 큐를 이용한 너비 우선 탐색으로 구현했다.
//4. 빨라지긴 했으나 너무 깊은 탐색으로 서버가 약 1~2초간 멈춘다.
//result: 사용하지 말것 -_-
// DONE: f100613.1L, (WAVERIX) 너무 어이 없는 처리를 수행한다.
// 알고리즘은 그럴싸 한데, 그걸 구현하는 로직은 거참... 시간 관계상 추후 정밀 검증하자.
// CHANGES: f100613.1L, change the usage about a searching data structure

namespace util { namespace internal {
;

typedef STLX_HASH_MAP<DWORD, bool> DuplicationChecker;

struct GMDropQueueCache
{
    struct Node;
    //
    GMDropQueueCache();
    ~GMDropQueueCache();
    void ClearMarking();
    Node* AllocNode();
    void MoveToPending(Node* node);
    //
    util::__LList<Node> queue_root_;
    util::__LList<Node> pending_root_;
    util::__LList<Node> free_list_;
    DuplicationChecker duplication_checker_;
};

struct GMDropQueue
{
    typedef GMDropQueueCache::Node Node;
    GMDropQueue(GMDropQueueCache* cache);
    ~GMDropQueue();
    bool empty() const;
    void push_back(DuplicationChecker::iterator checker_it);
    DWORD front() const;
    void pop_front();
    //
    GMDropQueueCache* cache_;
};

}}; //end of namespace

//==================================================================================================

struct util::internal::GMDropQueueCache::Node : public util::__LList<Node>
{
    Node();
    //
    DuplicationChecker::iterator checker_it;
};

inline util::internal::GMDropQueueCache::Node::Node() {
    LList::Init(this);
};

util::internal::GMDropQueueCache::GMDropQueueCache()
{
    LList::Init(&queue_root_);
    LList::Init(&pending_root_);
    LList::Init(&free_list_);
}

util::internal::GMDropQueueCache::~GMDropQueueCache()
{
    while (LList::IsEmpty(&free_list_) == false)
    {
        Node* it = static_cast<Node*>(free_list_.next);
        LList::Delete(it);
        delete it;
    }
    while (LList::IsEmpty(&pending_root_) == false)
    {
        Node* it = static_cast<Node*>(pending_root_.next);
        LList::Delete(it);
        delete it;
    }
    while (LList::IsEmpty(&queue_root_) == false)
    {
        Node* it = static_cast<Node*>(queue_root_.next);
        LList::Delete(it);
        delete it;
    }
    duplication_checker_.clear();
}

void util::internal::GMDropQueueCache::ClearMarking()
{
    while (LList::IsEmpty(&pending_root_) == false)
    {
        Node* it = static_cast<Node*>(pending_root_.next);
        it->checker_it->second = false;
        LList::Delete(it);
        LList::AddPrev(it, &free_list_);
    }
    while (LList::IsEmpty(&queue_root_) == false)
    {
        Node* it = static_cast<Node*>(queue_root_.next);
        it->checker_it->second = false;
        LList::Delete(it);
        LList::AddPrev(it, &free_list_);
    }
}

util::internal::GMDropQueueCache::Node*
util::internal::GMDropQueueCache::AllocNode()
{
    Node* node = NULL;
    if (LList::IsEmpty(&free_list_) == false)
    {
        Node* it = static_cast<Node*>(free_list_.next);
        LList::Delete(it);
        node = it;
    }
    else
    {
        node = new Node;
    }
    return node;
}

void util::internal::GMDropQueueCache::MoveToPending(Node* node)
{
    LList::Delete(node);
    LList::AddPrev(node, &pending_root_);
}

//==================================================================================================

util::internal::GMDropQueue::GMDropQueue(GMDropQueueCache* cache) : cache_(cache)
{
}

util::internal::GMDropQueue::~GMDropQueue() {
    cache_->ClearMarking();
}

bool util::internal::GMDropQueue::empty() const
{
    return LList::IsEmpty(&cache_->queue_root_) != false;
}

void util::internal::GMDropQueue::push_back(DuplicationChecker::iterator checker_it)
{
    Node* node = cache_->AllocNode();
    node->checker_it = checker_it;
    node->checker_it->second = true;
    LList::AddPrev(node, &cache_->queue_root_);
}

DWORD util::internal::GMDropQueue::front() const
{
    assert(LList::IsEmpty(&cache_->queue_root_) == false);
    const Node* node = static_cast<const Node*>(cache_->queue_root_.next);
    return node->checker_it->first;
}

void util::internal::GMDropQueue::pop_front()
{
    Node* node = static_cast<Node*>(cache_->queue_root_.next);
    cache_->MoveToPending(node);
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

void DropManager::GMDrop(GameField* game_field,
    const WzVector* origin_pos, Player* owner,
    DWORD monster_key, const BASE_NPCINFO* monster_info,
    eZONETYPE zone_type, DWORD drop_index,
    eNPC_GRADE npc_grade, float bonus_percentage,
    DWORD temp_player_key)
{
    using util::internal::DuplicationChecker;
    using util::internal::GMDropQueue;
    //재귀함수콜을 비재귀로 바꿀 큐
    static util::internal::GMDropQueueCache recursive_list_cache;
    static SCItemSlot dropped_item_slot;
    //
    //중복 인덱스 검사 (기획자가 스크립트를 무한반복 가능하게 해놓아 반복 체크 함)
    DuplicationChecker& drop_index_table = recursive_list_cache.duplication_checker_;
    GMDropQueue recursive_list(&recursive_list_cache);

    MONEY dropped_money = 0;
    DWORD decided_seed = 0;
    typedef DropRatioInfoList::RANDOMIZER_HASH RandominzerTable;
    typedef DropRatioInfoList::RANDOMIZER Randomizer;

    const RandominzerTable& randomizer_table = \
        drop_core_.GetDropRatioInfoList()->GetRandomizerTable();
    DuplicationChecker::iterator found = drop_index_table.find(drop_index);
    // 한번도 검색하지 않은 것이면
    if (found == drop_index_table.end())
    {
        DuplicationChecker::_Pairib result_insert = drop_index_table.insert(\
            DuplicationChecker::value_type(drop_index, false));
        recursive_list.push_back(result_insert.first);
    }
    else if (found->second == false) {
        recursive_list.push_back(found);
    };
    //
    GameZone* game_zone = game_field->GetGameZone();
    while (recursive_list.empty() == false)
    {   // skip that can't find in a drop script
        drop_index = recursive_list.front();
        RandominzerTable::const_iterator found_randomizer = randomizer_table.find(drop_index);
        recursive_list.pop_front();
        if (found_randomizer == randomizer_table.end()) {
            continue;
        };
        const Randomizer* randomizer = found_randomizer->second;
        const Randomizer::KeyTypeArray& drop_obj_array = randomizer->GetKeyTypeArray();
        const Randomizer::key_type* it = &drop_obj_array[0];
        const Randomizer::key_type* const end = &drop_obj_array[_countof(drop_obj_array)];
        for (; it != end; ++it)
        {
            const DropObj* drop_obj = *it;
            if (drop_obj == NULL) {
                continue;
            }
            const eDROP_OBJ_TYPES drop_obj_type = drop_obj->GetType();
            switch (drop_obj_type)
            {
            case DROP_OBJ_MONEY:
                {
                    const DropMoneyObj* drop_money_obj = static_cast<const DropMoneyObj*>(drop_obj);
                    const bool success = \
                        drop_core_.DropMoneyFunc(const_cast<DropMoneyObj*>(drop_money_obj),
                        zone_type, bonus_percentage, npc_grade,
                        dropped_money, decided_seed) != false;
                    if (success == false) {
                        break;
                    }
                    ASSERT(decided_seed != 0);
                    const bool is_jackpot = (decided_seed > 1);
                    const int log_type =  is_jackpot ? MONEY_MONSTER_JACKPOT_DROP
                        : MONEY_MONSTER_DROP;
                    // 몬스터 돈 드랍 로그
                    GAMELOG->LogMonsterMoneyDrop(log_type,
                        dropped_money, monster_info->m_MonsterCode, npc_grade,
                        game_zone->GetMapCode(), *origin_pos, game_zone->GetKey());
                    if (temp_player_key != 0) {
                        temp_player_key = GetSecondHeimOwnershipUserKey(owner);
                    }
                    DropMoney(is_jackpot, game_field, origin_pos,
                        monster_key, temp_player_key, dropped_money);
                }
                break;
            case DROP_OBJ_ITEM:
                {
                    const DropItemObj* drop_item_obj = static_cast<const DropItemObj*>(drop_obj);
                    if (drop_item_obj->GetGroupCode() & DROP_GROUP::LINK_ITEM)
                    {
                        int number_of_drop_items = 0;
                        const bool success = \
                            drop_core_.DropItemFunc(const_cast<DropItemObj*>(drop_item_obj),
                            npc_grade, dropped_item_slot, &number_of_drop_items) != false;
                        if (success == false) {
                            break;
                        }
                        if (temp_player_key != 0) {
                            temp_player_key = \
                                GetSecondItemOwnershipUserKey(owner, dropped_item_slot);
                        }
                        // NOTE: f110401.3L
                        for (int counter = 0; counter < number_of_drop_items; ++counter)
                        {
                            DropItem(game_field, origin_pos, monster_key, temp_player_key,
                                monster_info, dropped_item_slot);
                        };
                    }
                    else
                    {   //한번도 검색하지 않은 것이면
                        drop_index = drop_item_obj->GetItemCode();
                        DuplicationChecker::iterator found = \
                            drop_index_table.find(drop_index);
                        // 한번도 검색하지 않은 것이면
                        if (found == drop_index_table.end())
                        {
                            DuplicationChecker::_Pairib result_insert = drop_index_table.insert(\
                                DuplicationChecker::value_type(drop_index, false));
                            recursive_list.push_back(result_insert.first);
                        }
                        else if (found->second == false) {
                            recursive_list.push_back(found);
                        }
                    }
                }
                break;
            }; //end 'switch'
        }
    }
}; //end of 'DropManager::GMDrop'

//==================================================================================================
//==================================================================================================
