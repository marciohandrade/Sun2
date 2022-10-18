#include "UtilityCommon.h"
#include "SharedMemoryPool.h"
#include "LinkedListCS.h"

#include "../ProgramCommon/Version.h"

//==================================================================================================
//  Name: SharedMemoryPool
//  @history
//      07/09/07 - CheckIn (Waverix)
//      10/02/25 - remove macros '__NA000000_080816_EXSIZE_AND_MEMORY_OVERRUN_DETECT__'
//      10/02/25 - changes pool leak dectector to support common utility
//      10/05/10 - changes a coding style and
//                 adds a huge size memory tokenizing header which support a memory counter
//      10/05/19 - changed to first instancing approach based on singleton
//      10/05/26 - detached shared code block from template implements
//      11/07/05 - heap control extention and
//                 full streaming index calculation without the bytes alignment modification
//               - upgrade to v0250, see the reference f110706.3L.
//==================================================================================================

#if USING_SUN_SHARED_POOL_REPORT
    // 메모리 누수 검출 도구 / Release모드에서도 탐지가 가능
    #define USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR  (1)
#else
    // 메모리 누수 검출 도구 / Release모드에서도 탐지가 가능
    #define USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR  (0)
#endif

BOOST_STATIC_ASSERT(SharedMemoryPool::kVersion == 0x0250);
//==================================================================================================
//
#define BUF_ALIGN_008(__size)           (((__size)+((8)-1))&(~((8)-1)))
#define BUF_ALIGN_008_BY_IDX(__idx)     ((__idx)<<3)
#define BUF_008_IDX_BY_SIZE(__size)     ((__size)>>3)

#define BUF_ALIGN_016(__size)           (((__size)+((16)-1))&(~((16)-1)))
#define BUF_ALIGN_016_BY_IDX(__idx)     ((__idx)<<4)  // 8x에 연속된 계산이 이뤄져야 하므로 이것만 사용할 수는 없다. 사용사례 참고할 것
#define BUF_016_IDX_BY_SIZE(__size)     ((__size)>>4) // 상동

#define BUF_ALIGN_032(__size)           (((__size)+((32)-1))&(~((32)-1)))
#define BUF_ALIGN_032_BY_IDX(__idx)     ((__idx)<<5)  // 16x에 연속된 계산이 이뤄져야 하므로 이것만 사용할 수는 없다. 사용사례 참고할 것
#define BUF_032_IDX_BY_SIZE(__size)     ((__size)>>5) // 상동

#define BUF_ALIGN_064(__size)           (((__size)+((64)-1))&(~((64)-1)))
#define BUF_ALIGN_064_BY_IDX(__idx)     ((__idx)<<6)  // 64x에 연속된 계산이 이뤄져야 하므로 이것만 사용할 수는 없다. 사용사례 참고할 것
#define BUF_064_IDX_BY_SIZE(__size)     ((__size)>>6) // 상동

// reference pool
#define BUF_ALIGN_4096(__size)          (((__size)+((4096)-1))&(~((4096)-1)))
#define BUF_4096_IDX_BY_SIZE(__size)    ((__size)>>12) // 상동
#define BUF_ALIGN_1024x1024(__size)         (((__size)+((1048576)-1))&(~((1048576)-1)))
#define BUF_1024x1024_IDX_BY_SIZE(__size)   ((__size)>>20) // 상동

namespace util { namespace memory {
;

//==================================================================================================

struct SharedMemoryTable;

enum eCheckMemoryNodeResult
{
    eError_NotDefined,
    eError_Success_PoolManagedToken,
    eError_Success_PoolUnmanagedToken,
    eError_Fail_Null_Pointer,
    eError_Unknown_HeapToken,
    eError_Fail_BufferOverrunedToken, // not control
    eError_Fail_CorruptedHeaderToken, // not control
};
//
struct SharedPoolVerifier
{
    ulong verifier;
    void* next;
};

//==================================================================================================
// NOTE: f110526.3L, f110513.7L, this class is used by SharedMemoryPoolNode<_size>
class SharedMemoryPoolSizeBased
{
public:
    struct Token; // allocated/deallocated management
    struct TokenBlock;// Token chunking
    struct MemoryForUse; // support memory
    struct MemoryBlockForUse; // memory chunking
    //----------------------------------------------------------------------------------------------
    static const uint16_t kMagicCodeAlloc = ('S') | ('A'<<8);
    static const uint16_t kMagicCodeFree  = ('s') | ('F'<<8);
    static const uint16_t kMagicCodeTailConv = 0x0101; // kMagicCode(Alloc/Free) (+/-) this
    //
    union UsingTag
    {
        ulong composed;
        struct {
            uint16_t size; // requested size
            uint16_t tag;  // allocated or deallocated tag (magic key)
        };
    };
    struct TokenVerifier
    {
        UsingTag using_tag;
        Token* parent;
    };
    // allocated/deallocated management
    struct Token : public util::__LList<Token>
    {
        UsingTag using_tag;
        MemoryForUse* link;
        const char* function;
        int line_no;
        int issued_number;
    };
    // Token chunking
    struct TokenBlock : public util::__LList<TokenBlock>
    {
    #pragma warning(push)
    #pragma warning(disable:4200)
        Token stream[0];
    #pragma warning(pop)
    };
    // support memory
    struct MemoryForUse
    {
        TokenVerifier header[1];
        // next specification
        //uint8_t type_stream[type_size];
        // ...
        // TokenVerifier tail; // overrun check & comparison
    };
    struct MemoryBlockForUse : public util::__LList<MemoryBlockForUse>
    {
    #pragma warning(push)
    #pragma warning(disable:4200)
        MemoryForUse stream[0]; // (MemoryForUse + tail + alignment) * n
    #pragma warning(pop)
    };
    //
    struct Statistics {
        size_t number_of_free_nodes;
        size_t number_of_alloc_nodes;
        size_t number_of_total_blocks;
        size_t number_of_free_requested;
        size_t number_of_alloc_requested;
    };
    //----------------------------------------------------------------------------------------------

    SharedMemoryPoolSizeBased(); // not unused, brutely clread
    SharedMemoryPoolSizeBased(size_t type_size, size_t chunk_size);
    //~SharedMemoryPoolSizeBased(); <- not use destructor

    void* Allocate(SharedMemoryTable* const table,
        size_t requested_size, const char* name, const int line_no);
    void Deallocate(SharedMemoryTable* const table, MemoryForUse* memory_for_use);

    void FreeAllElements(SharedMemoryTable* const table);
    //
    const Statistics& GetStatistics() const;
    static MemoryForUse* CheckNodeHeader(void* element, eCheckMemoryNodeResult* const result);
    static size_t GetMaxMemoryForUseSize(size_t request_size);
private:
    void AddBlock(SharedMemoryTable* const table);
    //----------------------------------------------------------------------------------------------
    // fields...
    const size_t type_size_;
    const size_t growth_count_; // page-fit
    util::__LList<Token> free_list_;
    util::__LList<Token> alloc_list_;
    util::__LList<TokenBlock> total_list_;
    util::__LList<MemoryBlockForUse> memory_list_;
    Statistics statistics_;
    //
    friend struct SharedMemoryTable;
    __DISABLE_COPY(SharedMemoryPoolSizeBased);
};

//==================================================================================================

struct SharedMemoryTable
{
    enum ePoolIndex
    {
        kSizeIndex_None = 0,
        // 8x based
        kSizeIndex_0008, // very busible access, level 5
        kSizeIndex_0016, // level 4
        kSizeIndex_0024, // level 4
        kSizeIndex_0032, // level 4
        kSizeIndex_0040, // level 4
        kSizeIndex_0048, // STLX_MAP, node size, level 2
        kSizeIndex_0056,
        kSizeIndex_0064,
        kSizeIndex_0072,
        kSizeIndex_0080,
        kSizeIndex_0088,
        kSizeIndex_0096,
        kSizeIndex_0104,
        kSizeIndex_0112,
        kSizeIndex_0120,
        kSizeIndex_0128,
        // 32x based
        kSizeIndex_0160,
        kSizeIndex_0192, // level 3
        kSizeIndex_0224,
        kSizeIndex_0256,
        // 64x based
        kSizeIndex_0320,
        kSizeIndex_0384,
        kSizeIndex_0448,
        kSizeIndex_0512,
        // 128x based
        kSizeIndex_0640, // SCItemSlot, level 4
        kSizeIndex_0768,
        kSizeIndex_0896,
        kSizeIndex_1024,
        kSizeIndex_1152,
        kSizeIndex_1280, // STLX_HASH_MAP, bucket secondary extention, level 2
        kSizeIndex_1408,
        //
        kSizeIndex_Counts,
    };
    enum ePoolInfo
    {
        //------------------------------
        kSize008xUpperbound = 128,
        kSize032xUpperbound = 256,
        kSize064xUpperbound = 512,
        kSize128xUpperbound = 1408,
        kSizeOverUpperbound = kSize128xUpperbound,
        kStreamingSizeCounts = int(kSizeOverUpperbound / 8) + 1, // 176 + 1
    };
    struct TokenSizeNode
    {
        uint16_t pool_index;
        uint16_t token_size;
    };
    //----------------------------------------------------------------------------------------------
    SharedMemoryTable();
    ~SharedMemoryTable();
    //
    void* Allocate(size_t size, const char* name, const int line_no);
    void Deallocate(void* ptr);
    //
    bool ReportMemoryLeaks(const char* path_file_name);
    bool ReportCurrentMemoryUsages();
    static void TouchMemoryLeakReportFile(const char* path_file_name);
    static void ReportRuntimeFailedObject(eCheckMemoryNodeResult error,
        void* ptr, SharedMemoryPoolSizeBased::MemoryForUse* memory_for_use);
    //----------------------------------------------------------------------------------------------
    uint8_t padding0_[236];
    int initialized_;
    //HANDLE handle_token_block_;
    //HANDLE handle_using_block_;
    int issued_ordered_number_;
    int reference_counter_;
    TokenSizeNode index_by_size8x_[kStreamingSizeCounts];
    uint8_t padding1_[60];
    SharedMemoryPoolSizeBased size_pool_instance_[kSizeIndex_Counts];
    uint8_t padding2_[256];
    SharedMemoryPoolSizeBased::Statistics chunk_statistics_4096[256];
    SharedMemoryPoolSizeBased::Statistics chunk_statistics_1024x1024[256];
    //
};

//--------------------------------------------------------------------------------------------------

SharedMemoryPoolSizeBased::MemoryForUse* _CheckPoolUnmanagedHeap(
    eCheckMemoryNodeResult* const result,
    void* element, SharedMemoryPoolSizeBased::MemoryForUse* memory_for_use);

SharedMemoryPoolSizeBased::MemoryForUse* _CheckPoolManagedHeap(
    eCheckMemoryNodeResult* const result,
    void* element, SharedMemoryPoolSizeBased::MemoryForUse* memory_for_use);

}}; //end of namespace

//==================================================================================================
// static heap data
static void (*__fnSharedMemoryLeakDetect)(DWORD alloc_counter);
static void (*__fnSharedMemoryLeakCheck)();
static uint8_t shared_memory_pool_block[sizeof(SharedMemoryPool)];
static uint8_t shared_memory_table_block[sizeof(util::memory::SharedMemoryTable)];

//==================================================================================================
// implements

__forceinline util::memory::SharedMemoryPoolSizeBased::MemoryForUse*
util::memory::_CheckPoolUnmanagedHeap(util::memory::eCheckMemoryNodeResult* const result,
    void* element, util::memory::SharedMemoryPoolSizeBased::MemoryForUse* memory_for_use)
{
    using namespace util::memory;
    assert(memory_for_use->header->parent == element);
    SharedMemoryPoolSizeBased::TokenVerifier tail_check =
        *reinterpret_cast<SharedMemoryPoolSizeBased::TokenVerifier*>(\
            reinterpret_cast<char*>(element) + memory_for_use->header->using_tag.composed);
    //tail_check.using_tag.tag -= SharedMemoryPoolSizeBased::kMagicCodeTailConv;
    if (memory_for_use->header->using_tag.composed == tail_check.using_tag.composed &&
        memory_for_use->header->parent == tail_check.parent)
    {
        *result = eError_Success_PoolUnmanagedToken;
        return memory_for_use;
    }
    else if (memory_for_use->header->parent != tail_check.parent)
    {
        *result = eError_Fail_BufferOverrunedToken;
        return memory_for_use;
    }
    else //memory_for_use->header->using_tag.composed != tail_check->using_tag.composed
    {   // ...
        *result = eError_Fail_BufferOverrunedToken;
        return memory_for_use;
    }
}

__forceinline util::memory::SharedMemoryPoolSizeBased::MemoryForUse*
util::memory::_CheckPoolManagedHeap(util::memory::eCheckMemoryNodeResult* const result,
    void* element, util::memory::SharedMemoryPoolSizeBased::MemoryForUse* memory_for_use)
{
    using namespace util::memory;
    const SharedMemoryPoolSizeBased::UsingTag using_tag =
        memory_for_use->header->using_tag;
    if (using_tag.tag == util::memory::SharedMemoryPoolSizeBased::kMagicCodeAlloc ||
        using_tag.tag == util::memory::SharedMemoryPoolSizeBased::kMagicCodeFree)
    {
        SharedMemoryPoolSizeBased::TokenVerifier tail_check =
            *reinterpret_cast<SharedMemoryPoolSizeBased::TokenVerifier*>(\
                reinterpret_cast<char*>(element) + using_tag.size);
        tail_check.using_tag.tag -= SharedMemoryPoolSizeBased::kMagicCodeTailConv;
        if (using_tag.composed == tail_check.using_tag.composed)
        {
            *result = eError_Success_PoolManagedToken;
            return memory_for_use;
        }
        else
        {
            *result = eError_Fail_BufferOverrunedToken;
            return memory_for_use;
        };
    }
    else
    {
        *result = eError_Fail_CorruptedHeaderToken;
        return memory_for_use;
    };
}

util::memory::SharedMemoryPoolSizeBased::MemoryForUse*
util::memory::SharedMemoryPoolSizeBased::CheckNodeHeader(
    void* element, eCheckMemoryNodeResult* const result)
{
    MemoryForUse* memory_for_use = reinterpret_cast<MemoryForUse*>(element);
    if (memory_for_use == NULL) {
        *result = eError_Fail_Null_Pointer; // error: null pointer
        return NULL;
    };
    --memory_for_use;
    if (Token* parent = memory_for_use->header->parent)
    {
        if (parent == element)
        {   // large size token, not managed in pool
            return _CheckPoolUnmanagedHeap(result, element, memory_for_use);
        }
        else
        {   // managed node in the size-based pool or normal heap containing corrupted header
            return _CheckPoolManagedHeap(result, element, memory_for_use);
        };
    };
    // error handling
    *result = eError_Unknown_HeapToken; // error: unexpected pointer, can't control
    return memory_for_use;
};

//==================================================================================================
// SharedMemoryPool implementations
//
#define SMPOOL_PTR()    reinterpret_cast<SharedMemoryPool*>(shared_memory_pool_block)
#define CHECK_SMPOOL_INSTANCED()    (SMPOOL_PTR()->instanced_tag_ == kInstancedTag)
#define SMROUTER_PTR(_pool_instance_) \
    reinterpret_cast<util::memory::SharedMemoryTable*>((_pool_instance_)->inter_block_)

__forceinline SharedMemoryPool* SharedMemoryPool::Instance()
{
    if (CHECK_SMPOOL_INSTANCED()) {
        return SMPOOL_PTR();
    };
    SharedMemoryPool::CreateInstance();
    return SMPOOL_PTR();
};

bool SharedMemoryPool::CreateInstance()
{
    if (CHECK_SMPOOL_INSTANCED()) {
        return true;
    }
    // initialize buffer block
    new (shared_memory_pool_block) SharedMemoryPool;
    return true;
}

bool SharedMemoryPool::CheckInstanced()
{
    return CHECK_SMPOOL_INSTANCED();
}

void SharedMemoryPool::DestroyInstance()
{
    SMPOOL_PTR()->~SharedMemoryPool();
    *(ulong*)shared_memory_pool_block = 0x00011000; // released dummy value
}

SharedMemoryPool::SharedMemoryPool()
    : instanced_tag_(kInstancedTag)
{
    static bool initialized_static = false;

    if (initialized_static == false)
    {
        ZeroMemory(shared_memory_table_block, sizeof(shared_memory_table_block));
        inter_block_ = new (shared_memory_table_block) util::memory::SharedMemoryTable;
        initialized_static = true;
    };
}

SharedMemoryPool::~SharedMemoryPool()
{
    static bool released_static = false;

    if (released_static == false)
    {
        if (inter_block_)
        {
            SMROUTER_PTR(this)->~SharedMemoryTable();
            inter_block_ = NULL;
        }
        released_static = true;
    }
}


void* SharedMemoryPool::MemoryAllocate(size_t size, const char* name, const int line_no)
{
    SharedMemoryPool* shared_memory_pool = SharedMemoryPool::Instance();
    void* ptr = SMROUTER_PTR(shared_memory_pool)->Allocate(size, name, line_no);
    return ptr;
}

void SharedMemoryPool::MemoryDeAllocate(void* ptr)
{
    SharedMemoryPool* shared_memory_pool = SharedMemoryPool::Instance();
    if ((ptr && shared_memory_pool) == false) {
        return;
    }
    SMROUTER_PTR(shared_memory_pool)->Deallocate(ptr);

    int reference_counter = SMROUTER_PTR(shared_memory_pool)->reference_counter_;
    if (reference_counter < 2) {
        SharedMemoryPool::DestroyInstance();
    }
}

void* SharedMemoryPool::RedirectNew_MemoryAllocate(size_t size,
    const char* name, const int line_no, SharedMemoryPool*)
{
    static void* (*redirect_new_static)(size_t, const char*, const int) = \
        &SharedMemoryPool::MemoryAllocate;
    // same as MemoryAllocate
    return (*redirect_new_static)(size, name, line_no);
}

void  SharedMemoryPool::RedirectNew_MemoryDeallocate(void* ptr)
{
    static void (*redirect_new_static)(void*) = &SharedMemoryPool::MemoryDeAllocate;
    return (*redirect_new_static)(ptr);
}

// NOTE: f110708.5L, added the token validation routine
bool SharedMemoryPool::CheckValidAllocatedMemory(void* ptr)
{
    SharedMemoryPool* shared_memory_pool = SharedMemoryPool::Instance();
    if ((ptr && shared_memory_pool) == false) {
        return false;
    };
    util::memory::eCheckMemoryNodeResult result = util::memory::eError_NotDefined;
    util::memory::SharedMemoryPoolSizeBased::MemoryForUse* memory_for_use = \
        util::memory::SharedMemoryPoolSizeBased::CheckNodeHeader(ptr, &result);
    if (result == util::memory::eError_Success_PoolManagedToken ||
        result == util::memory::eError_Success_PoolUnmanagedToken)
    {
        return true;
    };
    return false;
};
//==================================================================================================
//==================================================================================================
//==================================================================================================

util::memory::SharedMemoryPoolSizeBased::SharedMemoryPoolSizeBased()
    : type_size_(0)
    , growth_count_(0)
{
}

util::memory::SharedMemoryPoolSizeBased::SharedMemoryPoolSizeBased(
    size_t type_size, size_t chunk_size)
    : type_size_(type_size)
    , growth_count_(chunk_size)
{
    util::LList::Init(&free_list_);
    util::LList::Init(&alloc_list_);
    util::LList::Init(&total_list_);
    util::LList::Init(&memory_list_);
}

void* util::memory::SharedMemoryPoolSizeBased::Allocate(SharedMemoryTable* const table,
    size_t requested_size, const char* name, const int line_no)
{
    if (util::LList::IsEmpty(&free_list_)) {
        AddBlock(table);
    };
    Token* token = free_list_.next;
    util::LList::Delete(token);
    --statistics_.number_of_free_nodes;
    //
    token->function = name;
    token->line_no = line_no;
    token->issued_number = ++table->issued_ordered_number_;
    if (TokenVerifier* verifier = token->link->header)
    {
        verifier->parent = token; //                   must be pre-checked
        verifier->using_tag.size = static_cast<uint16_t>(requested_size);
        verifier->using_tag.tag = this->kMagicCodeAlloc;
        token->using_tag = verifier->using_tag;
    };

    uint8_t* ptr = reinterpret_cast<uint8_t*>(token->link->header + 1);
    if (TokenVerifier* tail = reinterpret_cast<TokenVerifier*>(ptr + requested_size)) {
        tail->parent = token;
        tail->using_tag = token->using_tag;
        tail->using_tag.tag += this->kMagicCodeTailConv;
    };
    util::LList::AddPrev(token, &alloc_list_);
    ++statistics_.number_of_alloc_nodes;
    ++statistics_.number_of_alloc_requested;
    //
    
    return ptr;
}

void util::memory::SharedMemoryPoolSizeBased::Deallocate(SharedMemoryTable* const table,
    MemoryForUse* memory_for_use)
{
    // validated memory_for_use
    Token* const token = memory_for_use->header->parent;
    if (token->link != memory_for_use ||
        token->using_tag.composed != memory_for_use->header->using_tag.composed ||
        token->using_tag.tag != this->kMagicCodeAlloc ||
        util::LList::IsEmpty(&alloc_list_))
    {
    #if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
        // memory leak occurrence
        if (token->function != NULL && ::IsBadReadPtr(token->function, sizeof(char*))) {
            token->function = NULL;
        };
        uint8_t* used_ptr = reinterpret_cast<uint8_t*>(memory_for_use->header + 1);
        SUNLOG(eCRITICAL_LOG,
               _T("|MemoryCheckReport|Msg=unexpected free operation")
               _T("|MemInfo=size(%d), tag([%02X:%02X])|Suspicious Location='%s', %d lines")
               _T("|Dump=[%02X][%02X][%02X][%02X]|"),
               token->function, token->line_no,
               token->using_tag.size,
               (token->using_tag.tag & 0xFF), ((token->using_tag.tag >> 8) & 0xFF),
               used_ptr[0], used_ptr[1], used_ptr[2], used_ptr[3]);
    #endif
    #ifdef _SERVER
        assert(!"corrupted!");
    #else
        ASSERT(!"corrupted!");
    #endif
        return;
    }
    else
    {   // validated memory
        ulong* dest_it = reinterpret_cast<ulong*>(memory_for_use + 1);
        const ulong* const dest_upperbound = &dest_it[this->type_size_ >> 2];
        for (; dest_it < dest_upperbound; ++dest_it) {
            *dest_it = 0xFEEEEEEE;
        };
    };
    //----------------------------------------------------------------------------------------------
    ;{
        util::LList::Delete(token);
        --statistics_.number_of_alloc_nodes;

        util::LList::AddPrev(token, &free_list_);
        ++statistics_.number_of_free_nodes;
        //
        ++statistics_.number_of_free_requested;
    };
    //
    if (UsingTag* tag = &token->using_tag) {
        tag->composed = 0;
        tag->tag = this->kMagicCodeFree;
    };
}

__forceinline size_t
util::memory::SharedMemoryPoolSizeBased::GetMaxMemoryForUseSize(size_t request_size)
{
    //assert(request_size != 0 && BUF_ALIGN_008(request_size) == request_size);
    //                         header                    body                        tail
    size_t block_size = sizeof(MemoryForUse) + BUF_ALIGN_008(request_size) + sizeof(TokenVerifier);
    assert(block_size == BUF_ALIGN_008(block_size));
    return block_size;
};

void util::memory::SharedMemoryPoolSizeBased::AddBlock(SharedMemoryTable* const table)
{
    const size_t token_array_size = sizeof(TokenBlock) + (sizeof(Token) * growth_count_);
    const size_t one_token_memory_for_use = GetMaxMemoryForUseSize(type_size_);
    const size_t chunked_elements_size = sizeof(MemoryBlockForUse) +
                                         (growth_count_ * one_token_memory_for_use);
    //
    TokenBlock* const block = (TokenBlock*)::HeapAlloc(\
        ::GetProcessHeap(), 0, token_array_size);
    ;{
        ZeroMemory(block, token_array_size);
        util::LList::Init(block);
        util::LList::AddPrev(block, &total_list_);
        ++statistics_.number_of_total_blocks;
    };
    MemoryBlockForUse* const use_block = (MemoryBlockForUse*)::malloc(chunked_elements_size);
        //(MemoryBlockForUse*)::HeapAlloc(table->handle_using_block_, 0, chunked_elements_size);
    ;{
        util::LList::Init(use_block);
        util::LList::AddPrev(use_block, &memory_list_);
        // same 'statistics_.number_of_total_blocks'
    };

    Token* token = block->stream;
    MemoryForUse* memory_for_use = use_block->stream;
#define NextMemoryForUse(__ptr__) \
    reinterpret_cast<MemoryForUse*>(reinterpret_cast<uint8_t*>(__ptr__) + one_token_memory_for_use)

    for (size_t i = 0; i < growth_count_;
         ++i, ++token, (memory_for_use = NextMemoryForUse(memory_for_use)))
    {
        // init tokens
        util::LList::Init(token);
        token->link = memory_for_use;
        if (UsingTag* tag = &token->using_tag)
        {
            tag->composed = 0;
            tag->tag = this->kMagicCodeFree;
            memory_for_use->header->parent = token;
            memory_for_use->header->using_tag = token->using_tag;
        };
        util::LList::AddPrev(token, &free_list_);
        ++statistics_.number_of_free_nodes;
    };
}

#if _BUILD_FLAG_SHARED_MEMORY_NEWDEL_SYNC_CHECK_
void util::memory::SharedMemoryPoolSizeBased::DeadBeef(Element* element)
{
    int* it = &element->deadBeef;
    int* itend = (int*)((byte*)element + elementSize_);
    while(it < itend)
        *it++ = 0xDEADBEEF;
}
#endif


//------------------------------------------------------------------------------
//  ()
//  name:  
//  usage:  

void util::memory::SharedMemoryPoolSizeBased::FreeAllElements(SharedMemoryTable* const table)
{
    ;{
        size_t counter = 0;
        TokenBlock* root = static_cast<TokenBlock*>(&total_list_);
        TokenBlock* token = root->next;
        while (token != root)
        {
            TokenBlock* next_token = util::LList::Delete(token);
            ::HeapFree(::GetProcessHeap(), 0, token);
            token = next_token;
            ++counter;
        };
        ASSERT(statistics_.number_of_total_blocks == counter);
    };
    ;{
        size_t counter = 0;
        MemoryBlockForUse* root = static_cast<MemoryBlockForUse*>(&memory_list_);
        MemoryBlockForUse* token = root->next;
        while (token != root)
        {
            MemoryBlockForUse* next_token = util::LList::Delete(token);
            ::free(token);
            //::HeapFree(table->handle_using_block_, 0, token);
            token = next_token;
            ++counter;
        };
        ASSERT(statistics_.number_of_total_blocks == counter);
    };
    ZeroMemory(&statistics_, sizeof(statistics_));
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

void* util::memory::SharedMemoryTable::Allocate(
    size_t requested_size, const char* name, const int line_no)
{
    size_t resized = BUF_ALIGN_008(requested_size);
    if (resized == 0) {
        // NOTE: f110708.6L, some of applications has internal memory check routine
        // that the size 0 test. for example, the DirectX 9.0c, 2006, Feb. version differ
        // after version.
        //assert(!"do you really want to require size 0 allocation?");
        requested_size = 1;
        resized = BUF_ALIGN_008(1);
    };
    void* allocated_ptr = NULL;
    if (resized <= this->kSizeOverUpperbound)
    {
        size_t index = (resized >> 3);
        if (const TokenSizeNode* pool_info = &this->index_by_size8x_[index]) {
            index = pool_info->pool_index;
            assert(pool_info->token_size == resized);
        };
        if (SharedMemoryPoolSizeBased* pool = &this->size_pool_instance_[index]) {
            allocated_ptr = pool->Allocate(this, requested_size, name, line_no);
        };
        
    #if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
        if (__fnSharedMemoryLeakDetect) {
            __fnSharedMemoryLeakDetect(issued_ordered_number_);
        };
    #endif
    }
    else
    {
        size_t block_size = sizeof(SharedMemoryPoolSizeBased::MemoryForUse) +
                            BUF_ALIGN_008(requested_size) +
                            sizeof(SharedMemoryPoolSizeBased::TokenVerifier);
        SharedMemoryPoolSizeBased::MemoryForUse* head_ptr = \
            reinterpret_cast<SharedMemoryPoolSizeBased::MemoryForUse*>(::malloc(block_size));
        if (head_ptr == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, "[%s:%d] malloc fail : %s, %d, size=%d",
                __FUNCTION__, __LINE__, name, line_no, requested_size);
            return NULL;
        }

        allocated_ptr = (uint8_t*)(head_ptr) + sizeof(*head_ptr);
        if (SharedMemoryPoolSizeBased::TokenVerifier* header = head_ptr->header) {
            header->parent = (SharedMemoryPoolSizeBased::Token*)(allocated_ptr);
            header->using_tag.composed = static_cast<ulong>(requested_size);
        };
        if (SharedMemoryPoolSizeBased::TokenVerifier* tail =
                reinterpret_cast<SharedMemoryPoolSizeBased::TokenVerifier*>(
                    (uint8_t*)(allocated_ptr) + requested_size))
        {
            *tail = *head_ptr->header;
        };
    #if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
        const bool huge_chunk = (1024 * 1024) < block_size;
        SharedMemoryPoolSizeBased::Statistics* statistic = NULL;
        if (huge_chunk) {
            size_t bound_index = BUF_1024x1024_IDX_BY_SIZE(block_size);
            if (bound_index < _countof(chunk_statistics_1024x1024)) {
                statistic = &chunk_statistics_1024x1024[bound_index];
            }
        }
        else {
            size_t bound_index = BUF_4096_IDX_BY_SIZE(block_size);
            if (bound_index < _countof(chunk_statistics_4096)) {
                statistic = &chunk_statistics_4096[bound_index];
            }
        };
        if (statistic != NULL) {
            ++statistic->number_of_alloc_nodes;
            ++statistic->number_of_alloc_requested;
        };
    #endif
        
    }
    if (allocated_ptr) {
        ++reference_counter_;
        if (reference_counter_ < 0) {
            SUNLOG(eCRITICAL_LOG, "[%s:%d] reference_counter_ overflow accured", 
                __FUNCTION__, __LINE__);
        }
    };
    return allocated_ptr;
}

void util::memory::SharedMemoryTable::Deallocate(void* element)
{
    eCheckMemoryNodeResult result = util::memory::eError_NotDefined;
    SharedMemoryPoolSizeBased::MemoryForUse* memory_for_use = \
        SharedMemoryPoolSizeBased::CheckNodeHeader(element, &result);
    if (result == util::memory::eError_Success_PoolManagedToken)
    {
        size_t used_size = memory_for_use->header->using_tag.size;
        size_t resized = BUF_ALIGN_008(used_size);
        size_t index = (resized >> 3);
        ;      index = this->index_by_size8x_[index].pool_index;
        SharedMemoryPoolSizeBased* const pool = &this->size_pool_instance_[index];
        pool->Deallocate(this, memory_for_use);
        --reference_counter_;
        return;
    }
    else if (result == util::memory::eError_Success_PoolUnmanagedToken)
    {
    #if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
        size_t block_size = memory_for_use->header->using_tag.composed;
        const bool huge_chunk = (1024 * 1024) < block_size;
        SharedMemoryPoolSizeBased::Statistics* statistic = NULL;
        if (huge_chunk) {
            size_t bound_index = BUF_1024x1024_IDX_BY_SIZE(block_size);
            if (bound_index < _countof(chunk_statistics_1024x1024)) {
                statistic = &chunk_statistics_1024x1024[bound_index];
            }
        }
        else {
            size_t bound_index = BUF_4096_IDX_BY_SIZE(block_size);
            if (bound_index < _countof(chunk_statistics_4096)) {
                statistic = &chunk_statistics_4096[bound_index];
            }
        };
        if (statistic != NULL) {
            ++statistic->number_of_free_nodes;
            ++statistic->number_of_free_requested;
        };
    #endif
        --reference_counter_;
        ::free(memory_for_use);
        return;
    }
    // error handling
    this->ReportRuntimeFailedObject(result, element, memory_for_use);
    __debugbreak();
}

//==================================================================================================

#define CALCULATE_PAGE_FIT_IN_INFO(__info__, __aligned_size__)

#define SM_NODE_LIST() \
    /* 8x based */ \
    SM_NODE_DECL(kSizeIndex_0008, 8, 8, 5290) /* 124K*/ \
    SM_NODE_DECL(kSizeIndex_0016, 16, 16, 4905) /* 128K*/ \
    SM_NODE_DECL(kSizeIndex_0024, 24, 24, 3174) /* 124K*/ \
    SM_NODE_DECL(kSizeIndex_0032, 32, 32, 2645) /* 124K*/ \
    SM_NODE_DECL(kSizeIndex_0040, 40, 40, 146) /* 8K*/ \
    SM_NODE_DECL(kSizeIndex_0048, 48, 48, 127) /* 8K*/ \
    SM_NODE_DECL(kSizeIndex_0056, 56, 56, 56) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0064, 64, 64, 51) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0072, 72, 72, 46) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0080, 80, 80, 213) /* 20K*/ \
    SM_NODE_DECL(kSizeIndex_0088, 88, 88, 1142) /* 116K*/ \
    SM_NODE_DECL(kSizeIndex_0096, 96, 96, 73) /* 8K*/ \
    SM_NODE_DECL(kSizeIndex_0104, 104, 104, 34) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0112, 112, 112, 31) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0120, 120, 120, 30) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0128, 128, 128, 28) /* 4K*/ \
    /* 32x based */ \
    SM_NODE_DECL(kSizeIndex_0160, 160, 160, 93) /* 16K*/ \
    SM_NODE_DECL(kSizeIndex_0192, 192, 192, 315) /* 64K*/ \
    SM_NODE_DECL(kSizeIndex_0224, 224, 224, 17) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0256, 256, 256, 15) /* 4K*/ \
    /* 64x based */ \
    SM_NODE_DECL(kSizeIndex_0320, 320, 320, 12) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0384, 384, 384, 10) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0448, 448, 448, 8) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0512, 512, 512, 7) /* 4K*/ \
    /* 128x based */ \
    SM_NODE_DECL(kSizeIndex_0640, 640, 640, 231) /* 148K*/ \
    SM_NODE_DECL(kSizeIndex_0768, 768, 768, 5) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_0896, 896, 896, 4) /* 4K*/ \
    SM_NODE_DECL(kSizeIndex_1024, 1024, 1024, 63) /* 64K*/ \
    SM_NODE_DECL(kSizeIndex_1152, 1152, 1152, 7) /* 8K*/ \
    SM_NODE_DECL(kSizeIndex_1280, 1280, 1280, 22) /* 28K*/ \
    SM_NODE_DECL(kSizeIndex_1408, 1408, 1408, 23) /* 32K*/

//==================================================================================================

util::memory::SharedMemoryTable::SharedMemoryTable()
{
    //FitInPageInfo fitin_pageinfo;
    memset(padding0_, 0xFF, sizeof(padding0_));
    memset(padding1_, 0xCC, sizeof(padding1_));
    memset(padding2_, 0xCD, sizeof(padding2_));
    ++issued_ordered_number_;
    ++reference_counter_;
    BOOST_STATIC_ASSERT(kStreamingSizeCounts < 256);
    BOOST_STATIC_ASSERT((BUF_ALIGN_008(0) == 0) && (BUF_ALIGN_008(1) == 8));
    // NOTE: f110706.3L, create two private heaps to support 'TokenBlock' and 'memory token for use'
    // the reason of using each separated heap protect the unexpected status
    // of the 'memory token for use' like a buffer overrun.
    ;{
        //this->handle_token_block_ = ::HeapCreate(0, 0, 0);
        //assert(handle_token_block_ != INVALID_HANDLE_VALUE);
        //this->handle_using_block_ = ::HeapCreate(0, 0, 0);
        //assert(handle_using_block_ != INVALID_HANDLE_VALUE);
    };

    // start 0 size.
    for (size_t token_size = 0; token_size <= kSizeOverUpperbound; (token_size += 8))
    {
        size_t array_index = (token_size >> 3);
        if (token_size <= kSize008xUpperbound)
        {
            size_t recalc_size = token_size - 0;
            index_by_size8x_[array_index].pool_index = static_cast<uint16_t>(recalc_size >> 3);
            index_by_size8x_[array_index].token_size = static_cast<uint16_t>(token_size);
        }
        else if (token_size <= kSize032xUpperbound)
        {
            size_t recalc_size = token_size - kSize008xUpperbound;
            index_by_size8x_[array_index].pool_index = static_cast<uint16_t>(\
                (recalc_size >> 5) + ((recalc_size & (32 - 1)) ? 1 : 0) + kSizeIndex_0128);
            index_by_size8x_[array_index].token_size = static_cast<uint16_t>(token_size);
        }
        else if (token_size <= kSize064xUpperbound)
        {
            size_t recalc_size = token_size - kSize032xUpperbound;
            index_by_size8x_[array_index].pool_index = static_cast<uint16_t>(\
                (recalc_size >> 6) + ((recalc_size & (64 - 1)) ? 1 : 0) + kSizeIndex_0256);
            index_by_size8x_[array_index].token_size = static_cast<uint16_t>(token_size);
        }
        else if (token_size <= kSize128xUpperbound)
        {
            size_t recalc_size = token_size - kSize064xUpperbound;
            index_by_size8x_[array_index].pool_index = static_cast<uint16_t>(\
                (recalc_size >> 7) + ((recalc_size & (128 - 1)) ? 1 : 0) + kSizeIndex_0512);
            index_by_size8x_[array_index].token_size = static_cast<uint16_t>(token_size);
        }
    };
    assert(*(ulong*)padding0_ == 0xFFFFFFFF);
    assert(*(ulong*)padding1_ == 0xCCCCCCCC);
    assert(*(ulong*)padding2_ == 0xCDCDCDCD);
    //
    //----------------------------------------------------------------------------------------------
    if (util::memory::SharedMemoryPoolSizeBased* pool = &this->size_pool_instance_[kSizeIndex_None])
    {
        // nothing to do
    };
#undef SM_NODE_DECL
#define SM_NODE_DECL(_size_index_, _byte_alignment_size_, _dummy_, _growth_) \
    if (util::memory::SharedMemoryPoolSizeBased* pool = &this->size_pool_instance_[_size_index_]) \
    { \
        CALCULATE_PAGE_FIT_IN_INFO(&fitin_pageinfo, (_byte_alignment_size_)); \
        new (pool) util::memory::SharedMemoryPoolSizeBased((_byte_alignment_size_), (_growth_)); \
    };
    //----------------------------------------------------------------------------------------------
    SM_NODE_LIST(); // instance
    //----------------------------------------------------------------------------------------------

    this->initialized_ = true;
}

util::memory::SharedMemoryTable::~SharedMemoryTable()
{
    this->initialized_ = false;
#undef SM_NODE_DECL
#define SM_NODE_DECL(_size_index_, _byte_alignment_size_, _dummy_, _growth_) \
    if (util::memory::SharedMemoryPoolSizeBased* pool = &this->size_pool_instance_[_size_index_]) \
    { \
        pool->FreeAllElements(this); \
    };
    //----------------------------------------------------------------------------------------------
    SM_NODE_LIST(); // release
    //----------------------------------------------------------------------------------------------
    //if (this->handle_token_block_ != INVALID_HANDLE_VALUE) {
    //    ::HeapDestroy(this->handle_token_block_);
    //    this->handle_token_block_ = INVALID_HANDLE_VALUE;
    //};
    //if (this->handle_using_block_ != INVALID_HANDLE_VALUE) {
    //    ::HeapDestroy(this->handle_using_block_);
    //    this->handle_using_block_ = INVALID_HANDLE_VALUE;
    //};
}

//==================================================================================================

void _SetSharedMemoryLeakDetect(void (*userhook)(DWORD))
{
#if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
    __fnSharedMemoryLeakDetect = userhook;
    return;
#else
    __UNUSED(userhook);
#endif
}

void TestUnit_UsedLogOutput()
{
#if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
    if (SharedMemoryPool::CheckInstanced() == false) {
        return;
    };
    SharedMemoryPool* shared_memory_pool = SMPOOL_PTR();
    SMROUTER_PTR(shared_memory_pool)->ReportCurrentMemoryUsages();
    return;
#endif //
}

BOOL TraceShareMemoryLeak(const char* const path_file_name)
{
#if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
    if (SharedMemoryPool::CheckInstanced() == false) {
        util::memory::SharedMemoryTable::TouchMemoryLeakReportFile(path_file_name);
        return true;
    };
    SharedMemoryPool* shared_memory_pool = SMPOOL_PTR();
    return SMROUTER_PTR(shared_memory_pool)->ReportMemoryLeaks(path_file_name);
    //
#else
    __UNUSED(path_file_name);
    return true;
#endif
}

void util::memory::SharedMemoryTable::ReportRuntimeFailedObject(eCheckMemoryNodeResult error,
    void* ptr, SharedMemoryPoolSizeBased::MemoryForUse* memory_for_use)
{
#if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
    switch (error)
    {
    case util::memory::eError_NotDefined:
        ASSERT(!"unexpected error");
        return;
    case util::memory::eError_Success_PoolManagedToken:
    case util::memory::eError_Success_PoolUnmanagedToken:
        ASSERT(!"can't entered with this result");
        return;
    case util::memory::eError_Fail_Null_Pointer:
        ASSERT(!"requested null pointer releasing");
        return;
    };
    SharedMemoryPoolSizeBased::UsingTag verifier = memory_for_use->header->using_tag;
    uint8_t* used_ptr = reinterpret_cast<uint8_t*>(ptr);
    if ((error != util::memory::eError_Fail_BufferOverrunedToken) ||
        (void*)memory_for_use->header->parent == used_ptr)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|MemoryCheckReport|Msg=token validation error logging#1, Error(%d)")
               _T("|MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)")
               _T("|Dump=")
               _T("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X|"),
               error, memory_for_use, verifier.size,
               (verifier.tag & 0xFF), ((verifier.tag >> 8) & 0xFF),
               *(ulong*)memory_for_use,
               used_ptr[ 0], used_ptr[ 1], used_ptr[ 2], used_ptr[ 3],
               used_ptr[ 4], used_ptr[ 5], used_ptr[ 6], used_ptr[ 7],
               used_ptr[ 8], used_ptr[ 9], used_ptr[10], used_ptr[11],
               used_ptr[12], used_ptr[13], used_ptr[14], used_ptr[15]);
    }
    else
    {
        util::memory::SharedMemoryPoolSizeBased::Token* const token = memory_for_use->header->parent;
        // ... dangerous...
        SUNLOG(eCRITICAL_LOG,
               _T("|MemoryCheckReport|Msg=token validation error logging#2, Error(%d)")
               _T("|MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)")
               _T("|Location='%s', %d lines|ExtraMsg='%s'")
               _T("|Dump=")
               _T("%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X|"),
               error, used_ptr, token->using_tag.size,
               (token->using_tag.tag & 0xFF), ((token->using_tag.tag >> 8) & 0xFF),
               *(ulong*)used_ptr,
               token->function, token->line_no,
               "suspicious buffer overrun object",
               used_ptr[ 0], used_ptr[ 1], used_ptr[ 2], used_ptr[ 3],
               used_ptr[ 4], used_ptr[ 5], used_ptr[ 6], used_ptr[ 7],
               used_ptr[ 8], used_ptr[ 9], used_ptr[10], used_ptr[11],
               used_ptr[12], used_ptr[13], used_ptr[14], used_ptr[15]);
    }
    assert(!"corrupted!");
    return;
#else
    __UNUSED((error, ptr, memory_for_use));
    return;
#endif
}

void util::memory::SharedMemoryTable::TouchMemoryLeakReportFile(const char* path_file_name)
{
#if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
    HANDLE file_handle = ::CreateFile(path_file_name,
                                      GENERIC_WRITE,
                                      FILE_SHARE_WRITE,
                                      NULL,
                                      CREATE_ALWAYS,
                                      FILE_ATTRIBUTE_NORMAL,
                                      NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        return;
    };
    ::CloseHandle(file_handle);
    return;
#else
    __UNUSED(path_file_name);
    return;
#endif
}

bool util::memory::SharedMemoryTable::ReportMemoryLeaks(const char* path_file_name)
{
#if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
    const int kMaxOutputCounter = 100;
    if (this->initialized_ == false) {
        return true; // already free.
    };
  #if defined(_SERVER)
    HANDLE file_handle = ::CreateFile(path_file_name,
                                      GENERIC_WRITE,
                                      FILE_SHARE_WRITE,
                                      NULL,
                                      CREATE_ALWAYS,
                                      FILE_ATTRIBUTE_NORMAL,
                                      NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        return false;
    };
  #endif
    //
    char buffer[4096];
    DWORD file_written = 0;
    int written = 0;
    __TOUCHED((path_file_name, file_written));
    SharedMemoryPoolSizeBased* pool_it = &this->size_pool_instance_[1]; // 0 is unused
    const SharedMemoryPoolSizeBased* const pool_end =
        &this->size_pool_instance_[_countof(this->size_pool_instance_)];
    for (int counter = 0; pool_it != pool_end; ++pool_it)
    {
        util::__LList<SharedMemoryPoolSizeBased::Token>* const token_end = &pool_it->alloc_list_;
        SharedMemoryPoolSizeBased::Token* token_it = pool_it->alloc_list_.next;
        for (; token_it != token_end; token_it = token_it->next)
        {
            if (counter >= kMaxOutputCounter) {
                buffer[written++] = '/';
                buffer[written++] = '/';
            };
            //
            SharedMemoryPoolSizeBased::MemoryForUse* const memory_for_use = token_it->link;
            ulong* const used_ptr = reinterpret_cast<ulong*>(memory_for_use + 1);
            uint8_t* const used_ptr_dump = reinterpret_cast<uint8_t*>(used_ptr);
            SharedMemoryPoolSizeBased::TokenVerifier tail_tag = \
                *reinterpret_cast<SharedMemoryPoolSizeBased::TokenVerifier*>(
                    used_ptr_dump + token_it->using_tag.size);
            tail_tag.using_tag.tag -= SharedMemoryPoolSizeBased::kMagicCodeTailConv;
            const bool matched_link = (token_it == memory_for_use->header->parent);
            const bool matched_headtag = (token_it->using_tag.composed ==
                                          memory_for_use->header->using_tag.composed);
            const bool matched_tail = (token_it == tail_tag.parent) &&
                (token_it->using_tag.composed == tail_tag.using_tag.composed);
            if ((matched_headtag) && (matched_link) && (matched_tail))
            {   // 000
                written += _snprintf(&buffer[written], _countof(buffer) - written,
                    "    case %8u: // |MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)|"
                    "Location='%s', %d lines|Dump=[%02X][%02X][%02X][%02X]|ExtraMsg=(none)\r\n",
                    token_it->issued_number, used_ptr, token_it->using_tag.size,
                    (token_it->using_tag.tag & 0xFF), ((token_it->using_tag.tag >> 8) & 0xFF),
                    *used_ptr,
                    token_it->function, token_it->line_no,
                    used_ptr_dump[0], used_ptr_dump[1], used_ptr_dump[2], used_ptr_dump[3]);
                ++counter;
            }
            else if ((matched_headtag) && (matched_link) && (!matched_tail))
            {   // 001
                written += _snprintf(&buffer[written], _countof(buffer) - written,
                    "    case %8u: // |MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)|"
                    "Location='%s', %d lines|Dump=[%02X][%02X][%02X][%02X]|ExtraMsg='%s'\r\n",
                    token_it->issued_number, used_ptr, token_it->using_tag.size,
                    (token_it->using_tag.tag & 0xFF), ((token_it->using_tag.tag >> 8) & 0xFF),
                    *used_ptr,
                    token_it->function, token_it->line_no,
                    used_ptr_dump[0], used_ptr_dump[1], used_ptr_dump[2], used_ptr_dump[3],
                    "suspicious buffer overrun object");
                ++counter;
            }
            else if ((matched_headtag) && (!matched_link) && (matched_tail))
            {   // 010 -- very rarely
                written += _snprintf(&buffer[written], _countof(buffer) - written,
                    "//  case %8u: |MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)|"
                    "Location='%s', %d lines|Dump=[%02X][%02X][%02X][%02X]|ExtraMsg='%s'\r\n",
                    token_it->issued_number, used_ptr, token_it->using_tag.size,
                    (token_it->using_tag.tag & 0xFF), ((token_it->using_tag.tag >> 8) & 0xFF),
                    *used_ptr,
                    token_it->function, token_it->line_no,
                    used_ptr_dump[0], used_ptr_dump[1], used_ptr_dump[2], used_ptr_dump[3],
                    "corrupted");
            }
            else if ((matched_headtag) && (!matched_link) && (!matched_tail))
            {   // 011 -- very rarely
                written += _snprintf(&buffer[written], _countof(buffer) - written,
                    "    case %8u: // |MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)|"
                    "Location='%s', %d lines|Dump=[%02X][%02X][%02X][%02X]|ExtraMsg='%s'\r\n",
                    token_it->issued_number, used_ptr, token_it->using_tag.size,
                    (token_it->using_tag.tag & 0xFF), ((token_it->using_tag.tag >> 8) & 0xFF),
                    *used_ptr,
                    token_it->function, token_it->line_no,
                    used_ptr_dump[0], used_ptr_dump[1], used_ptr_dump[2], used_ptr_dump[3],
                    "corrupted & overruned");
                ++counter;
            }
            else if ((!matched_headtag) && (matched_link) && (matched_tail))
            {   // 100 -- rarely
                written += _snprintf(&buffer[written], _countof(buffer) - written,
                    "    case %8u: // |MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)|"
                    "Location='%s', %d lines|Dump=[%02X][%02X][%02X][%02X]|ExtraMsg='%s'\r\n",
                    token_it->issued_number, used_ptr, token_it->using_tag.size,
                    (token_it->using_tag.tag & 0xFF), ((token_it->using_tag.tag >> 8) & 0xFF),
                    *used_ptr,
                    token_it->function, token_it->line_no,
                    used_ptr_dump[0], used_ptr_dump[1], used_ptr_dump[2], used_ptr_dump[3],
                    "may be overruned by previous token");
                ++counter;
            }
            else if ((!matched_headtag) && (matched_link) && (!matched_tail))
            {   // 101 -- vary rarely
                written += _snprintf(&buffer[written], _countof(buffer) - written,
                    "    case %8u: // |MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)|"
                    "Location='%s', %d lines|Dump=[%02X][%02X][%02X][%02X]|ExtraMsg='%s'\r\n",
                    token_it->issued_number, used_ptr, token_it->using_tag.size,
                    (token_it->using_tag.tag & 0xFF), ((token_it->using_tag.tag >> 8) & 0xFF),
                    *used_ptr,
                    token_it->function, token_it->line_no,
                    used_ptr_dump[0], used_ptr_dump[1], used_ptr_dump[2], used_ptr_dump[3],
                    "may be overruned by previous token and overruned");
                ++counter;
            }
            else if ((!matched_headtag) && (!matched_link) && (matched_tail))
            {   // 110 -- rarely
                written += _snprintf(&buffer[written], _countof(buffer) - written,
                    "//  case %8u: |MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)|"
                    "Location='%s', %d lines|Dump=[%02X][%02X][%02X][%02X]|ExtraMsg='%s'\r\n",
                    token_it->issued_number, used_ptr, token_it->using_tag.size,
                    (token_it->using_tag.tag & 0xFF), ((token_it->using_tag.tag >> 8) & 0xFF),
                    *used_ptr,
                    token_it->function, token_it->line_no,
                    used_ptr_dump[0], used_ptr_dump[1], used_ptr_dump[2], used_ptr_dump[3],
                    "may be overruned by previous token");
            }
            else if ((!matched_headtag) && (!matched_link) && (!matched_tail))
            {   // 111 -- rarely
                written += _snprintf(&buffer[written], _countof(buffer) - written,
                    "//  case %8u: |MemInfo=ptr(0x%08X), size(%d), tag([%02X:%02X]), vft(0x%08X)|"
                    "Location='%s', %d lines|Dump=[%02X][%02X][%02X][%02X]|ExtraMsg='%s'\r\n",
                    token_it->issued_number, used_ptr, token_it->using_tag.size,
                    (token_it->using_tag.tag & 0xFF), ((token_it->using_tag.tag >> 8) & 0xFF),
                    *used_ptr,
                    token_it->function, token_it->line_no,
                    used_ptr_dump[0], used_ptr_dump[1], used_ptr_dump[2], used_ptr_dump[3],
                    "may be overruned by previous token, fully overruned");
            }
            //
        #if defined(_SERVER)
            ::WriteFile(file_handle, buffer, written, &file_written, NULL);
        #else
            OutputDebugString(buffer);
        #endif
            written = 0;
            //
        };
    };
  #if defined(_SERVER)
    ::FlushFileBuffers(file_handle);
    ::CloseHandle(file_handle);
  #endif
    return true;
#else // !USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
    __UNUSED(path_file_name);
    return true;
#endif //
}

bool util::memory::SharedMemoryTable::ReportCurrentMemoryUsages()
{
#if USING_SUN_SHARED_MEMORY_POOL_LEAK_DETECTOR
    if (this->initialized_ == false) {
        return true; // already free.
    };
    HANDLE file_handle = ::CreateFile("./memory_usage.log",
                                      GENERIC_WRITE,
                                      FILE_SHARE_WRITE,
                                      NULL,
                                      CREATE_ALWAYS,
                                      FILE_ATTRIBUTE_NORMAL,
                                      NULL);
    if (file_handle == INVALID_HANDLE_VALUE) {
        return false;
    };
    //
    char buffer[32768];
    int written = 0;

    SharedMemoryPoolSizeBased* pool_it = &this->size_pool_instance_[1]; // 0 is unused
    const SharedMemoryPoolSizeBased* const pool_end =
        &this->size_pool_instance_[_countof(this->size_pool_instance_)];
    for (int counter = 0; pool_it != pool_end; ++pool_it)
    {
        const SharedMemoryPoolSizeBased::Statistics& statistics = pool_it->statistics_;

        written += _snprintf(&buffer[written], _countof(buffer) - written,
                "|TokenSize=[%4u]|TotalTokens=%8u"
                "|NumberOfAllocated=%8u|NumberOfFreed=%8u"
                "|NumberOfAllocRequest=%8u|NumberOfFreeRequest=%8u|\r\n",
                pool_it->type_size_, statistics.number_of_total_blocks * pool_it->growth_count_,
                statistics.number_of_alloc_nodes, statistics.number_of_free_nodes,
                statistics.number_of_alloc_requested, statistics.number_of_free_requested);
    };
    DWORD file_written = 0;
    ::WriteFile(file_handle, buffer, written, &file_written, NULL);
    //
    ;{
        written = 0;
        const SharedMemoryPoolSizeBased::Statistics* non_pool_it = this->chunk_statistics_4096,
            * const non_pool_end = &this->chunk_statistics_4096[_countof(this->chunk_statistics_4096)];
        for (; non_pool_it != non_pool_end; ++non_pool_it)
        {
            written += _snprintf(&buffer[written], _countof(buffer) - written,
                "|NonPool|MaxSize=[%2uKBytes]|"
                "|NumberOfAllocated=%8u|NumberOfFreed=%8u"
                "|NumberOfAllocRequest=%8u|NumberOfFreeRequest=%8u|\r\n",
                ((static_cast<int>(non_pool_it - this->chunk_statistics_4096) + 1) * 4),
                non_pool_it->number_of_alloc_nodes, non_pool_it->number_of_free_nodes,
                non_pool_it->number_of_alloc_requested, non_pool_it->number_of_free_requested);
        }
        ::WriteFile(file_handle, buffer, written, &file_written, NULL);
    };
    ;{
        written = 0;
        const SharedMemoryPoolSizeBased::Statistics* non_pool_it = this->chunk_statistics_1024x1024,
            * const non_pool_end = &this->chunk_statistics_1024x1024[\
                _countof(this->chunk_statistics_1024x1024)];
        for (; non_pool_it != non_pool_end; ++non_pool_it)
        {
            written += _snprintf(&buffer[written], _countof(buffer) - written,
                "|NonPool|MaxSize=[%2uMBytes]|"
                "|NumberOfAllocated=%8u|NumberOfFreed=%8u"
                "|NumberOfAllocRequest=%8u|NumberOfFreeRequest=%8u|\r\n",
                (static_cast<int>(non_pool_it - this->chunk_statistics_1024x1024) + 1),
                non_pool_it->number_of_alloc_nodes, non_pool_it->number_of_free_nodes,
                non_pool_it->number_of_alloc_requested, non_pool_it->number_of_free_requested);
        }
        ::WriteFile(file_handle, buffer, written, &file_written, NULL);
    };
    
    ::FlushFileBuffers(file_handle);
    ::CloseHandle(file_handle);
    return true;
#else
    return true;
#endif
}