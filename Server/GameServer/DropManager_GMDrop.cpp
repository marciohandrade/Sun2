#include "stdafx.h"
#include "./DropManager.h"

//==================================================================================================

#include <SCItemSlot.h>

#include "GameZone.h"
#include "GameField.h"

#include "StatusManager.h"

#include "DropRatioInfoList.h"

//==================================================================================================
//1. ���� �켱 Ž������ ������ ���Ҵ�.
//2. ��ͷ� �����Ͽ�����, �ӵ��� �����Ƿ� ����ͷ� �����Ͽ� ť�� �̿��� �ʺ� �켱 Ž������ �����ߴ�.
//4. �������� ������ �ʹ� ���� Ž������ ������ �� 1~2�ʰ� �����.
//result: ������� ���� -_-
// DONE: f100613.1L, (WAVERIX) �ʹ� ���� ���� ó���� �����Ѵ�.
// �˰����� �׷��� �ѵ�, �װ� �����ϴ� ������ ����... �ð� ����� ���� ���� ��������.
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
    //����Լ����� ����ͷ� �ٲ� ť
    static util::internal::GMDropQueueCache recursive_list_cache;
    static SCItemSlot dropped_item_slot;
    //
    //�ߺ� �ε��� �˻� (��ȹ�ڰ� ��ũ��Ʈ�� ���ѹݺ� �����ϰ� �س��� �ݺ� üũ ��)
    DuplicationChecker& drop_index_table = recursive_list_cache.duplication_checker_;
    GMDropQueue recursive_list(&recursive_list_cache);

    MONEY dropped_money = 0;
    DWORD decided_seed = 0;
    typedef DropRatioInfoList::RANDOMIZER_HASH RandominzerTable;
    typedef DropRatioInfoList::RANDOMIZER Randomizer;

    const RandominzerTable& randomizer_table = \
        drop_core_.GetDropRatioInfoList()->GetRandomizerTable();
    DuplicationChecker::iterator found = drop_index_table.find(drop_index);
    // �ѹ��� �˻����� ���� ���̸�
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
                    // ���� �� ��� �α�
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
                    {   //�ѹ��� �˻����� ���� ���̸�
                        drop_index = drop_item_obj->GetItemCode();
                        DuplicationChecker::iterator found = \
                            drop_index_table.find(drop_index);
                        // �ѹ��� �˻����� ���� ���̸�
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
