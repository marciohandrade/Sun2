#include "UtilityCommon.h"
#include "./ObjKeyGenerator.h"

#if __OBJKEYGEN_VERSION_CTRL == 0x0130

//==================================================================================================

namespace util { namespace internal {
;
// NOTE: f111006.4L, this file define prefetched declaration for CObjKeyGenerator<typename>
// it can do solve that the problem of redefinition type in 'Apply Code Changes' is fails.
#define PREFETCHED_DECLARATION(type) \
    BOOST_JOIN(util::CObjKeyGenerator<type> A_TypeOf_, ##type)

struct PrefetchedCObjKeyGeneratorDeclarations
{
    PREFETCHED_DECLARATION(uint8_t);
    PREFETCHED_DECLARATION(uint16_t);
    PREFETCHED_DECLARATION(uint32_t);
    PREFETCHED_DECLARATION(ulong);
};

}}; //end of namespace

//==================================================================================================
// version 0.1.3.0
namespace util { namespace internal {
;

ObjectKeyGeneratorBase::~ObjectKeyGeneratorBase()
{
    if (total_array_) {
        ::free(total_array_);
        total_array_ = NULL;
    }
    lower_bound_ = upper_bound_ = 0;
    number_of_free_ = 0;
    free_key_set_.clear();
    initialized_ = false;
}

bool ObjectKeyGeneratorBase::Create(key_type start_key, key_type end_key)
{
    // zero is used as the empty key
    bool satisfied_create = initialized_ == false &&
                            (start_key != 0 && start_key < end_key) &&
                            free_key_set_.empty();
    if (FlowControl::FCAssert(satisfied_create) == false) {
        return false;
    }
    //
    lower_bound_ = start_key;
    upper_bound_ = end_key + 1 ; // [lb, end_key] -> [lb, up)
    //
    const key_type range = upper_bound_ - lower_bound_;
    if (!FlowControl::FCAssert(kMaxKeyBandwidth > range)) {
        return false;
    }
    ;{
        const size_t total_size = sizeof(AlignNode) * range;
        void* ptr = ::malloc(total_size);
        ZeroMemory(ptr, total_size);
        total_array_ = reinterpret_cast<AlignNode*>(ptr);
    };
    AlignNode* it = total_array_,
             * const end = total_array_ + range;
    for ( ; it != end; ++it) {
        free_key_set_.push_back(it);
    }
    number_of_free_ = range;
    initialized_ = true;
    //
    return true;
}

ObjectKeyGeneratorBase::key_type
ObjectKeyGeneratorBase::GetKey()
{
    KeySet::iterator it = free_key_set_.begin();
    while (it != free_key_set_.end())
    {
        AlignNode* align = *it;
        free_key_set_.pop_front();
        //
        if (align->color_ == kColorOfFree) {
            align->color_ = kColorOfIssued;
            key_type offset = static_cast<key_type>(align - total_array_);
            const key_type issue_key = lower_bound_ + offset;
            if (number_of_free_) {
                --number_of_free_;
            }
            return issue_key;
        }
        // already aligned
        it = free_key_set_.begin();
    }
    ASSERT(!"|Critical=\"can't issue key resource! empty.\"|");
    return kEmptyKey;
}

bool ObjectKeyGeneratorBase::RestoreKey(key_type key)
{
    if (FlowControl::FCAssert(lower_bound_ <= key && key < upper_bound_) == false) {
        return false;
    }
    //
    const key_type offset = key - lower_bound_;
    AlignNode* align = &total_array_[offset];
    if (FlowControl::FCAssert(align->color_ == kColorOfIssued) == false) {
        return false;
    }
    ++number_of_free_;
    align->color_ = kColorOfFree;
    free_key_set_.push_back(align);
    return true;
}

bool ObjectKeyGeneratorBase::ReserveKey(key_type key)
{
    if (FlowControl::FCAssert(lower_bound_ <= key && key < upper_bound_) == false) {
        return false;
    }
    //
    const key_type offset = key - lower_bound_;
    AlignNode* align = &total_array_[offset];
    if (FlowControl::FCAssert(align->color_ == kColorOfFree) == false) {
        return false;
    }
    //
    if (number_of_free_) {
        --number_of_free_;
    }
    align->color_ = kColorOfIssued;
    // (NOTE) use 'copy on write' mechanism related to changes of a free_key_set_
    return true;
}

}}; //end of namespace

#endif //__OBJKEYGEN_VERSION_CTRL == 0x0130


