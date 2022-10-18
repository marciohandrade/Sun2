#include "UtilityCommon.h"
#include "./chunk_stl.h"

// NOTE: f110328.2L, this file define prefetched declaration for chunk_stl types.
// it can do solve that the problem of redefinition type in 'Apply Code Changes'

namespace util {
;
template<class Allocator>
struct AllocatorDeclaration
{
};

#define PREFETCHED_DECLARATION(type) \
    BOOST_JOIN(util::AllocatorDeclaration<util::CustomAllocator<type> > A_TypeOf_, ##type)

struct PrefetchedDeclarations
{
    PREFETCHED_DECLARATION(int8_t);
    PREFETCHED_DECLARATION(uint8_t);
    PREFETCHED_DECLARATION(int16_t);
    PREFETCHED_DECLARATION(uint16_t);
    PREFETCHED_DECLARATION(int32_t);
    PREFETCHED_DECLARATION(uint32_t);
    PREFETCHED_DECLARATION(long);
    PREFETCHED_DECLARATION(ulong);
    PREFETCHED_DECLARATION(int64_t);
    PREFETCHED_DECLARATION(uint64_t);
};

}; //end of namespace

