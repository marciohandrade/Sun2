#include "UtilityCommon.h"
#include "./SolarHashTable.h"

#include <algorithm>

//==================================================================================================
//==================================================================================================
//==================================================================================================
#if SOLAR_HASH_TABLE_VERSION >= 0x0200
//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace util { namespace internal {
;

const util::internal::WxHashTableBase::hash_type kHashMask =
    mpl::if_c<sizeof(void*) == sizeof(ulong), mpl::integral_c<ulong, 0xDEADBEEFul>,
    mpl::if_c<sizeof(void*) == sizeof(int64), mpl::integral_c<uint64_t, 0xDEADBEEFDEADBEEFull>,
    void>::type >::type::value;

extern const PrimeNode hash_primes[7 + 1];
static const PrimeNode* GetPrimeNode(size_t hint_bucket_size);
static WxHashTableBase::hash_type GetBaseHashValue(
    const PrimeNode* prime_node, WxHashTableBase::complex_type key);

}}; //end of namespace

const util::internal::PrimeNode util::internal::hash_primes[7 + 1] =
{
    {     7,     7 * 4 },
    {    19,    19 * 4 },
    {   103,   103 * 4 },
    {   601,   601 * 4 },
    {  2411,  2411 * 4 },
    {  9973,  9973 * 4 },
    { 81839, ULONG_MAX },
    {     0,         0 } // divide by 0 exception
};

static const util::internal::PrimeNode* util::internal::GetPrimeNode(size_t hint_bucket_size)
{
    const PrimeNode* it = hash_primes,
                   * const end = &hash_primes[_countof(hash_primes)];
    for (; it != end; ++it)
    {
        if (it->need_change_next > hint_bucket_size) {
            return it;
        };
    };

    return NULL;
};

__forceinline static util::internal::WxHashTableBase::hash_type
util::internal::GetBaseHashValue(const PrimeNode* prime_node, WxHashTableBase::complex_type key)
{
    return ((WxHashTableBase::hash_type(key) ^ kHashMask) % prime_node->prime);
}

//==================================================================================================

util::internal::WxHashTableBase::WxHashTableBase()
{
    ZeroMemory(this, sizeof(*this));
    util::LList::Init(&list_root_);
    prime_node_ = GetPrimeNode(1);
    list_root_.key = complex_type(-1);
    list_root_.data = complex_type(-1);
}

util::internal::WxHashTableBase::WxHashTableBase(ulong init_bucket_size)
{
    ZeroMemory(this, sizeof(*this));
    this->Initialize(init_bucket_size);
}

util::internal::WxHashTableBase::~WxHashTableBase()
{
    if (buckets_) {
        TAllocFree(buckets_);
    };
    WxHashNode* it = list_root_.next;
    while (it != &list_root_)
    {
        WxHashNode* delete_it = it;
        it = it->next;
        util::LList::Delete(delete_it);
        TAllocDelete(WxHashNode, delete_it);
    };
    ZeroMemory(this, sizeof(*this));
}

//
bool util::internal::WxHashTableBase::Initialize(ulong init_bucket_size)
{
    if (FlowControl::FCAssert(initialized_mark_ != kInitializedTag && buckets_ == NULL) == false) {
        return false;
    };
    ZeroMemory(this, sizeof(*this));
    prime_node_ = GetPrimeNode(init_bucket_size);
    buckets_ = reinterpret_cast<WxHashNode**>(\
        TAllocMalloc(sizeof(WxHashNode*) * (prime_node_->prime << 1)));
    ZeroMemory(buckets_, sizeof(WxHashNode*) * (prime_node_->prime << 1));
    util::LList::Init(&list_root_);
    list_root_.key = complex_type(-1);
    list_root_.data = complex_type(-1);
    initialized_mark_ = kInitializedTag;
    //
    return true;
}

util::internal::WxHashTableBase::node_type*
util::internal::WxHashTableBase::find(complex_type key) const
{
    hash_type hash_value = GetBaseHashValue(prime_node_, key) << 1;
    node_type* node_it = buckets_[hash_value];
    if (node_it == NULL) {
        return const_cast<node_type*>(&this->list_root_);
    };
    node_type* const family_end = buckets_[hash_value + 1];
    for (; node_it != family_end; node_it = node_it->next)
    {
        if (node_it->key == key) {
            return node_it;
        };
    };
    
    return const_cast<node_type*>(&this->list_root_);
}

bool util::internal::WxHashTableBase::insert(complex_type key, complex_type data)
{
#if SOLAR_HASH_TABLE_VERSION >= 0x0210
    if (prime_node_->need_change_next < this->number_of_nodes_) {
        make_balanced_buckets(this);
    };
#endif
    hash_type hash_value = GetBaseHashValue(prime_node_, key) << 1;
    node_type* node_it = buckets_[hash_value];

    struct NodeInstance
    {
        typedef util::internal::WxHashTableBase::node_type node_type;
        typedef util::internal::WxHashTableBase::complex_type complex_type;
        node_type* instance(complex_type key, complex_type data)
        {
            node_type* new_node = TAllocNew(node_type);
            util::LList::Init(new_node);
            new_node->key = key;
            new_node->data = data;
            return new_node;
        };
    } node_instance;
    //
    if (node_it == NULL)
    {
        node_type* new_node = node_instance.instance(key, data);
        //
        util::LList::AddPrev(new_node, &list_root_);
        buckets_[hash_value] = new_node;
        buckets_[hash_value + 1] = &list_root_;
        end_bucket_ = &buckets_[hash_value + 1];
        //
        node_type* prev_node = static_cast<node_type*>(new_node->prev);
        if (prev_node != &list_root_)
        {
            hash_type prev_hash_value = GetBaseHashValue(prime_node_, prev_node->key) << 1;
            assert(prev_hash_value != hash_value);
            buckets_[prev_hash_value + 1] = new_node;
        };
        //
        ++this->number_of_nodes_;
        return true;
    }
    else if (this->find(key) == &list_root_)
    {
        node_type* new_node = node_instance.instance(key, data);
        //
        util::LList::AddPrev(new_node, node_it);
        node_type* prev_node = static_cast<node_type*>(new_node->prev);
        if (prev_node != &list_root_)
        {
            hash_type prev_hash_value = GetBaseHashValue(prime_node_, prev_node->key) << 1;
            assert(prev_hash_value != hash_value);
            buckets_[prev_hash_value + 1] = new_node;
        }
        assert(end_bucket_);
        buckets_[hash_value] = new_node;
        //
        ++this->number_of_nodes_;
        return true;
    }
    else
    {
        //ASSERT(!"this table can't allow multi-keys");
    }
    //
    return false;
}

// if the operation is success, the result is the next node also changed to empty table.
util::internal::WxHashTableBase::node_type*
util::internal::WxHashTableBase::erase(node_type* const entry_it)
{
    if (entry_it == &list_root_) {
        return &list_root_;
    };
    const complex_type entry_key = entry_it->key;
    hash_type hash_value = GetBaseHashValue(prime_node_, entry_key) << 1;
    node_type* node_it = buckets_[hash_value];
    if (node_it == NULL) {
        return &list_root_;
    };
    node_type* const family_end = buckets_[hash_value + 1];
    for (; node_it != family_end; node_it = node_it->next)
    {
        if (node_it != entry_it) {
            continue;
        };
        // (prev_end) node (node_end)
        node_type* prev_node = node_it->prev;
        node_type* next_node = node_it->next;
        if (prev_node != &list_root_)
        {
            hash_type prev_hash_value = GetBaseHashValue(prime_node_, prev_node->key) << 1;
            if (hash_value != prev_hash_value)
            {
                buckets_[prev_hash_value + 1] = next_node;
                if (next_node == &list_root_) {
                    end_bucket_ = &buckets_[prev_hash_value + 1];
                };
            };
        };
        //
        util::LList::Delete(node_it);
        if (buckets_[hash_value] == node_it)
        {
            if (next_node == &list_root_) // changed to empty bucket (in prev node, entry_it's cleared)
            {
                buckets_[hash_value] = NULL;
                buckets_[hash_value + 1] = NULL;
            }
            else //if (next_node != &list_root_)
            {
                hash_type next_hash_value = GetBaseHashValue(prime_node_, next_node->key) << 1;
                if (hash_value == next_hash_value) {
                    buckets_[hash_value] = next_node;
                }
                else {
                    buckets_[hash_value] = NULL;
                    buckets_[hash_value + 1] = NULL;
                };
            }
        };
        // CHANGES: f110609.6L, fixed memory leaks
        TAllocDelete(node_type, node_it);
        //
        --this->number_of_nodes_;
        //
        return next_node;
    };
    return &list_root_;
}

bool util::internal::WxHashTableBase::erase(complex_type key)
{
    node_type* found_it = find(key);
    if (found_it == &list_root_) {
        return false;
    };
    this->erase(found_it);
    return true;
}

void util::internal::WxHashTableBase::clear()
{
    WxHashNode* it = static_cast<WxHashNode*>(list_root_.next);
    while (it != &list_root_)
    {
        WxHashNode* delete_it = it;
        it = it->next;
        util::LList::Delete(delete_it);
        TAllocDelete(WxHashNode, delete_it);
    };
    this->number_of_nodes_ = 0;
    this->end_bucket_ = NULL;
    ZeroMemory(buckets_, sizeof(WxHashNode*) * (prime_node_->prime << 1));
}

void util::internal::WxHashTableBase::make_balanced_buckets(WxHashTableBase* cur_hash)
{
    __TOUCHED(cur_hash);
#if SOLAR_HASH_TABLE_VERSION >= 0x0210
    // NOTE: f110529.3L, implemented
    const PrimeNode* next_prime = cur_hash->prime_node_ + 1;
    WxHashTableBase t_hash;
    ;{
        t_hash.prime_node_ = next_prime;
        t_hash.buckets_ = reinterpret_cast<WxHashNode**>(
            TAllocMalloc(sizeof(WxHashNode*) * (next_prime->prime << 1)));
        ZeroMemory(t_hash.buckets_, sizeof(WxHashNode*) * (next_prime->prime << 1));
        t_hash.initialized_mark_ = kInitializedTag;
    };
    //
    node_type* it = static_cast<node_type*>(cur_hash->list_root_.next);
    while (it != &cur_hash->list_root_)
    {
        node_type* new_node = it;
        it = it->next;
        util::LList::Delete(new_node);
        //
        hash_type hash_value = GetBaseHashValue(t_hash.prime_node_, new_node->key) << 1;
        node_type* node_it = t_hash.buckets_[hash_value];
        //
        if (node_it == NULL)
        {
            util::LList::AddPrev(new_node, &t_hash.list_root_);
            t_hash.buckets_[hash_value] = new_node;
            t_hash.buckets_[hash_value + 1] = &t_hash.list_root_;
            t_hash.end_bucket_ = &t_hash.buckets_[hash_value + 1];
            //
            node_type* prev_node = static_cast<node_type*>(new_node->prev);
            if (prev_node != &t_hash.list_root_)
            {
                hash_type prev_hash_value = GetBaseHashValue(t_hash.prime_node_, prev_node->key) << 1;
                assert(prev_hash_value != hash_value);
                t_hash.buckets_[prev_hash_value + 1] = new_node;
            };
            //
            ++t_hash.number_of_nodes_;
            continue;
        }
        else if (t_hash.find(new_node->key) == &t_hash.list_root_)
        {
            util::LList::AddPrev(new_node, node_it);
            node_type* prev_node = static_cast<node_type*>(new_node->prev);
            if (prev_node != &t_hash.list_root_)
            {
                hash_type prev_hash_value = GetBaseHashValue(t_hash.prime_node_, prev_node->key) << 1;
                assert(prev_hash_value != hash_value);
                t_hash.buckets_[prev_hash_value + 1] = new_node;
            }
            assert(t_hash.end_bucket_);
            t_hash.buckets_[hash_value] = new_node;
            //
            ++t_hash.number_of_nodes_;
            continue;
        }
        else {
            ASSERT(!"unexpected node");
        };
    };
    // swap
    ;{
        cur_hash->number_of_nodes_ = t_hash.number_of_nodes_;
        cur_hash->prime_node_ = t_hash.prime_node_;
        TAllocFree(cur_hash->buckets_);
        cur_hash->buckets_ = t_hash.buckets_;
        t_hash.buckets_ = NULL;
        assert(util::LList::IsEmpty(&cur_hash->list_root_));
        //
        node_type* first_node = util::LList::Delete(&t_hash.list_root_);
        util::LList::AddPrev(&cur_hash->list_root_, first_node);
        //
        *t_hash.end_bucket_ = &cur_hash->list_root_;
        cur_hash->end_bucket_ = t_hash.end_bucket_;
        //
    };
#endif //SOLAR_HASH_TABLE_VERSION >= 0x0210
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
#else //SOLAR_HASH_TABLE_VERSION < 0x0200
//==================================================================================================
//==================================================================================================
//==================================================================================================

__EMPTY_FILE_TAG();

//==================================================================================================
//==================================================================================================
//==================================================================================================
#endif //SOLAR_HASH_TABLE_VERSION < 0x0200
//==================================================================================================
//==================================================================================================
//==================================================================================================
