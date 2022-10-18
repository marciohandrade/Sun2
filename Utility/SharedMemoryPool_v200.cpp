#include "UtilityCommon.h"
#include "SharedMemoryPool.h"
#include "LinkedListCS.h"

#include "../ProgramCommon/Version.h"

//==================================================================================================
//  <SharedMemoryPool>
//  <HISTORY>
//      07/09/07 - CheckIn (Waverix)
//      10/02/25 - remove macros '__NA000000_080816_EXSIZE_AND_MEMORY_OVERRUN_DETECT__'
//      10/02/25 - changes pool leak dectector to support common utility
//      10/05/10 - changes a coding style and
//                 adds a huge size memory tokenizing header which support a memory counter
//#define USING_SHARED_MEMORY_POOL_LEAK_DETECTOR // 메모리 누수 검출 도구 / Release모드에서도 탐지가 가능

//==================================================================================================
//
//using util::__LList;
//using util::LList;
//using util::PListNode;
#if SUN_CODE_BACKUP
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

const DWORD MAGIC_CODE_ALLOC     = ('S'<<16) | ('A'<<24);
const DWORD MAGIC_CODE_FREE      = ('S'<<16) | ('F'<<24);
const DWORD MAGIC_CODE_HDR_ALLOC = MAGIC_CODE_ALLOC;
const DWORD MAGIC_CODE_HDR_FREE  = MAGIC_CODE_FREE;
const DWORD MAGIC_CODE_HDR_MASK  = 0xFFFF0000;
const DWORD MAGIC_CODE_SZ_MASK   = 0x0000FFFF;

//==================================================================================================
//
class ISharedMemory
{
public:
    virtual ~ISharedMemory() { Release(); }
    virtual BOOL  Initialize(DWORD init_size = 0, DWORD growth = 100) = 0;
    virtual void  Release() {};
    virtual PVOID Alloc() = 0;
    virtual void  Free(PVOID ptr) = 0;

#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
    virtual DWORD GetTotalTokens() = 0;
    DWORD counter_for_allocations_;
    DWORD counter_for_deallocations_;
#endif //
};

//==================================================================================================
//
#pragma pack(push, 1)
template<typename _TYPE>
struct SHARED_POOL_VERIFIER
{
    DWORD   VERIFIER;
    _TYPE*  pNext;
};

#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)

struct MEMORY_LEAK_CHECKER
{
    util::__LList<void> LIST;
    PVOID   PTR[1];
    DWORD   _MARKER;

    MEMORY_LEAK_CHECKER() {
        util::LList::Init(&LIST); PTR[0] = 0; _MARKER = 0;
    }
    ~MEMORY_LEAK_CHECKER() {
        util::LList::Delete(&LIST);
    }
};

static void (*__SharedMemoryLeakDetect)(DWORD alloc_counter);
static void (*__SharedMemoryLeakCheck)();

#endif

struct /*__declspec(align(4))*/ sNODE_GENERNAL
{
    SHARED_POOL_VERIFIER<BYTE>  HEADER;
#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
    MEMORY_LEAK_CHECKER         LEAK_CHECKER;
#endif
    DWORD                       TYPE[1];
};

//==================================================================================================
//
static DWORD GetSizeN_VerifyBySharedPool(PVOID ptr)
{
    DWORD token_size = 0;
    sNODE_GENERNAL* node_base = reinterpret_cast<sNODE_GENERNAL*>(ptr);

    if (node_base)
    {
        //printf("offsetof %u\n", offsetof(sNODE_GENERNAL, TYPE));
        node_base = reinterpret_cast<sNODE_GENERNAL*>(\
            reinterpret_cast<BYTE*>(node_base) - offsetof(sNODE_GENERNAL, TYPE));

        const DWORD masking_value = MAGIC_CODE_HDR_MASK & node_base->HEADER.VERIFIER;
        if (MAGIC_CODE_HDR_ALLOC == masking_value || MAGIC_CODE_HDR_FREE == masking_value) {
            token_size = MAGIC_CODE_SZ_MASK & node_base->HEADER.VERIFIER;
        }
    }

    return token_size;
}
#pragma pack(pop)

//==================================================================================================
// <SharedMemoryBandwith>
template<typename WORD _SZ>
class SharedMemoryBandwith : public ISharedMemory
{
private:
    static const DWORD  MAGIC_CODE_ALLOC    = MAGIC_CODE_HDR_ALLOC | (_SZ);
    static const DWORD  MAGIC_CODE_FREE     = MAGIC_CODE_HDR_FREE | (_SZ);
    static const DWORD  MAGIC_CODE_TAILMARK = 0xDEADBEEF;

    struct sNODE;
    typedef SHARED_POOL_VERIFIER<sNODE> SHARED_POOL_VERIFIER_D;
private:
    struct __declspec(align(4)) sNODE
    {
    #pragma pack(push, 1)
        SHARED_POOL_VERIFIER_D  HEADER;
      #if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
        MEMORY_LEAK_CHECKER     LEAK_CHECKER;
      #endif
        BYTE                    TYPE[_SZ];
        DWORD                   OVERRUN_CHECKER;
    #pragma pack(pop)

        sNODE() {
            HEADER.VERIFIER = MAGIC_CODE_FREE;
        }
        inline BYTE* TypePtr() {
            return TYPE;
        }
        // CanAllocable 은 항상 성공해야 한다.
        inline bool CanAllocable() {
            return MAGIC_CODE_FREE == HEADER.VERIFIER;
        }
        inline bool CanDeAllocable(SHARED_POOL_VERIFIER_D* pROOT) {
            return pROOT->VERIFIER == HEADER.VERIFIER &&
                   OVERRUN_CHECKER == MAGIC_CODE_TAILMARK;
        }
        inline void SetAlloced() {
            HEADER.VERIFIER = MAGIC_CODE_ALLOC;
            OVERRUN_CHECKER = MAGIC_CODE_TAILMARK;
        }
        inline void SetFreed() {
            HEADER.VERIFIER = MAGIC_CODE_FREE;
        }

        inline static sNODE* GetHeader(PVOID node_ptr) {
            return reinterpret_cast<sNODE*>(\
                        reinterpret_cast<BYTE*>(node_ptr) - offsetof(sNODE_GENERNAL, TYPE));
        }

        inline static void InitRef(SHARED_POOL_VERIFIER_D* node_root) {
            node_root->VERIFIER = MAGIC_CODE_ALLOC;
        }

        //inline static DWORD GetSize(SHARED_POOL_VERIFIER_D* pROOT, sNODE* pNODE)
        //{
        //  DWORD dwNodeSize = 0;
        //  if ((pNODE->HEADER.m_dwCHECK == pROOT->m_dwCHECK)        //FreeNode일 경우
        //      || (pNODE->HEADER.m_pCHECK == pROOT->m_pdwCHECK)    // AllocedNode일 경우
        //      )
        //  {
        //      dwNodeSize = MAGIC_CODE_SZ_MASK & pROOT->m_dwCHECK;
        //  }
        //  return dwNodeSize;
        //}
    };

#pragma pack(push, 4)
    struct sNODE_BLOCK
    {
        sNODE_BLOCK* pNext;
        sNODE*       pNodeEND;
    #pragma warning(push)
    #pragma warning(disable:4200)
        sNODE        pNODE_HDR[0];
    #pragma warning(pop)
    };
#pragma pack(pop)
    //----------------------------------------------------------------------------------------------
    typedef SharedMemoryBandwith<_SZ> MEMORY_BANDWITH;

    //----------------------------------------------------------------------------------------------
public:
    SharedMemoryBandwith(DWORD init_size= 0, DWORD growth = 100)
        : node_block_root_(NULL),
          free_node_(NULL),
          growth_number_(0)
    {
        sNODE::InitRef(&root_);
        Initialize(init_size, growth);
    }
    ~SharedMemoryBandwith()
    {
        Release();
    }

    virtual BOOL Initialize(DWORD init_size = 0, DWORD growth = 100)
    {
        growth_number_ = growth;
        if (init_size) {
            total_number_of_tokens_ = 0;
            _AddBlock(init_size);
        }

        return true;
    }

    virtual void Release()
    {
        sNODE_BLOCK* node_block = node_block_root_;
        while (node_block != NULL)
        {
            sNODE_BLOCK* node_block_next = node_block->pNext;
            ::HeapFree(GetProcessHeap(), 0, node_block);
            node_block = node_block_next;
        }

        free_node_ = NULL;
        node_block_root_ = NULL;
    }

    virtual PVOID Alloc()
    {
        sNODE* alloc_node = free_node_;
        if (alloc_node == NULL)
        {
            _AddBlock(growth_number_);
            alloc_node = free_node_;
            if (alloc_node == NULL) {
                return NULL;
            }
        }
        free_node_ = free_node_->HEADER.pNext;
        //printf("B %p\tHDR %p\n", pAllocNode, pAllocNode->TYPE);
        alloc_node->SetAlloced();
        return alloc_node->TYPE;
    }

    virtual void Free(PVOID ptr)
    {
        sNODE* node = reinterpret_cast<sNODE*>(ptr);
        ;      node = node->GetHeader(node);
        if (node->CanDeAllocable(&root_))
        {
            node->SetFreed();
            node->HEADER.pNext = free_node_;
            free_node_ = node;
        //#ifdef _KOREA
            ;{
                // (CHANGES) (100113) (WAVERIX) to support memory life-time validation checker
                long* const end = reinterpret_cast<long*>(\
                    reinterpret_cast<char*>(node->TYPE) + sizeof(node->TYPE));
                for (long* it = reinterpret_cast<long*>(node->TYPE); it != end; ++it) {
                    *it = 0xFEEEFEEE;
                }
                //fill_n((DWORD*)pNode->TYPE, (INT)(sizeof(pNode->TYPE)/sizeof(DWORD)), 0xFEEEFEEE);
            };
        //#endif
            return;
        }

    #ifdef _DEBUG
        ASSERT(!"Can't DeAllocatable\n");
        __asm { int 3 };
    #else
        ASSERT(!"Can't DeAllocatable\n");
    #endif
    }


private:
    void _AddBlock(DWORD increase_number)
    {
        //printf("sizeof(sNODE_BLOCK) %u\tsizeof(sNODE) %u\n", sizeof(sNODE_BLOCK), sizeof(sNODE));
        const size_t required_size = sizeof(BYTE) *
                                     (sizeof(sNODE_BLOCK) + sizeof(sNODE) * increase_number);
        sNODE_BLOCK* node_block = reinterpret_cast<sNODE_BLOCK*>(
            ::HeapAlloc(GetProcessHeap(), 0, required_size));
        if (node_block == NULL) {
            return;
        }

        sNODE** ppPrev = &free_node_;
        sNODE* node = node_block->pNODE_HDR;
        sNODE* const node_end = reinterpret_cast<sNODE*>(\
            reinterpret_cast<BYTE*>(node_block->pNODE_HDR) + sizeof(sNODE) * increase_number);
        while (node < node_end)
        {
            *ppPrev = new (node) sNODE;
            ppPrev = &node->HEADER.pNext;

            node = reinterpret_cast<sNODE*>(reinterpret_cast<BYTE*>(node) + sizeof(sNODE));
        }

        *ppPrev = NULL;
        node_block->pNodeEND = node;
        node_block->pNext = node_block_root_;
        node_block_root_ = node_block;

        total_number_of_tokens_ += increase_number;
    }

    virtual DWORD GetTotalTokens() { return total_number_of_tokens_; }

private:
    sNODE_BLOCK* node_block_root_;
    sNODE* free_node_;
    SHARED_POOL_VERIFIER_D root_;
    DWORD growth_number_;
    DWORD total_number_of_tokens_;
};

//==================================================================================================
//
struct SMEMINFO
{
    enum eIDX
    {
        SIZE_8      = 1,            SIZE_16 , SIZE_24 ,       SIZE_32  ,
        SIZE_40     ,               SIZE_48 , SIZE_56 ,       SIZE_64  ,
        SIZE_72     ,               SIZE_80 , SIZE_88 ,       SIZE_96  ,
        SIZE_104    ,               SIZE_112, SIZE_120,       SIZE_128 ,
        SIZE_008_MAX,   // 17
        SIZE_144    = SIZE_008_MAX, SIZE_160, SIZE_176,       SIZE_192 ,
        SIZE_208    ,               SIZE_224, SIZE_240,       SIZE_256 ,
        SIZE_016_MAX,   // 25
        SIZE_288    = SIZE_016_MAX, SIZE_320, SIZE_352,       SIZE_384 ,
        SIZE_416    ,               SIZE_448, SIZE_480,       SIZE_512 ,
        SIZE_032_MAX,   // 33
        SIZE_576    = SIZE_032_MAX, SIZE_640, SIZE_704,       SIZE_768 ,
        SIZE_832    ,               SIZE_896, SIZE_960,       SIZE_1024,
        SIZE_1088   ,
        SIZE_064_MAX,   // 42
    };

    //enum eSIZE
    //{
    //  SIZE_8   = BUF_ALIGN_008_BY_IDX(1),
    //  SIZE_16  = BUF_ALIGN_008_BY_IDX(2),
    //  SIZE_24  = BUF_ALIGN_008_BY_IDX(3),
    //  SIZE_32  = BUF_ALIGN_008_BY_IDX(4),
    //  SIZE_40  = BUF_ALIGN_008_BY_IDX(5),
    //  SIZE_48  = BUF_ALIGN_008_BY_IDX(6),
    //  SIZE_56  = BUF_ALIGN_008_BY_IDX(7),
    //  SIZE_64  = BUF_ALIGN_008_BY_IDX(8),
    //  SIZE_72  = BUF_ALIGN_008_BY_IDX(9),
    //  SIZE_80  = BUF_ALIGN_008_BY_IDX(10),
    //  SIZE_88  = BUF_ALIGN_008_BY_IDX(11),
    //  SIZE_96  = BUF_ALIGN_008_BY_IDX(12),
    //  SIZE_104 = BUF_ALIGN_008_BY_IDX(13),
    //  SIZE_112 = BUF_ALIGN_008_BY_IDX(14),
    //  SIZE_120 = BUF_ALIGN_008_BY_IDX(15),
    //  SIZE_128 = BUF_ALIGN_008_BY_IDX(16),
    //  SIZE_008_MAX = BUF_ALIGN_008_BY_IDX(17),
    //};

    static const DWORD MAX_SHARED_MEMORY_008_SIZE  = 128;  // 8x
    static const DWORD MAX_SHARED_MEMORY_016_SIZE  = 256;  // 16x
    static const DWORD MAX_SHARED_MEMORY_032_SIZE  = 512;  // 32x
    static const DWORD MAX_SHARED_MEMORY_064_SIZE  = 1088; // 64x

    //static BYTE IDXofSIZE[MAX_SHARED_MEMORY_008_SIZE];
    static WORD SIZEofIDX[SIZE_064_MAX];

    static void Initialize();
};

//==================================================================================================

#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
//static MEMORY_LEAK_CHECKER    s_AllocedMemory;
static BYTE allocated_memory_list_s[sizeof(MEMORY_LEAK_CHECKER)];
#define ALLOCED_MEMORY_ENTRY()  (*(MEMORY_LEAK_CHECKER*)allocated_memory_list_s)
static int chunk_4096_alloc_counter_static[256];
static int chunk_1024x1024_alloc_counter_static[300];
#endif

//==================================================================================================

class SharedMemoryRouter
{
public:
    SharedMemoryRouter();
    ~SharedMemoryRouter();
    inline PVOID Alloc(size_t sz)
    {
        //sz = sizeof(SHARED_POOL_VERIFIER<BYTE>)+sz;
        if (SMEMINFO::MAX_SHARED_MEMORY_064_SIZE < sz)
        {
            const size_t resized_size = sz + sizeof(sNODE_GENERNAL);
            const bool huge_chunk = 1024 * 1024 < resized_size;
            char* redirect_ptr = (char*)::HeapAlloc(GetProcessHeap(), 0, resized_size);
            const size_t resized_chunk = huge_chunk ? BUF_ALIGN_1024x1024(resized_size)
                                                    : BUF_ALIGN_4096(resized_size);
            ((sNODE_GENERNAL*)redirect_ptr)->HEADER.VERIFIER = resized_chunk;
        #if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
            if (huge_chunk) {
                size_t bound_index = BUF_1024x1024_IDX_BY_SIZE(resized_chunk);
                if (bound_index < _countof(chunk_1024x1024_alloc_counter_static)) {
                    ++chunk_1024x1024_alloc_counter_static[bound_index];
                }
            }
            else {
                size_t bound_index = BUF_4096_IDX_BY_SIZE(resized_chunk);
                if (bound_index < _countof(chunk_4096_alloc_counter_static)) {
                    ++chunk_4096_alloc_counter_static[bound_index];
                }
            }
        #endif
            redirect_ptr += sizeof(sNODE_GENERNAL);
            return redirect_ptr;
        }
        //
        DWORD align_size = 0;
        DWORD index_by_size = 0;

        if (SMEMINFO::MAX_SHARED_MEMORY_032_SIZE < sz)
        {   // 64x  [SIZE_576=], 
            align_size = (DWORD)BUF_ALIGN_064(sz);
            index_by_size = BUF_064_IDX_BY_SIZE(align_size) + 24;
        }
        else if (SMEMINFO::MAX_SHARED_MEMORY_016_SIZE < sz)
        {   // 32x  [SIZE_288=25], 288=9~
            align_size = (DWORD)BUF_ALIGN_032(sz);
            index_by_size = BUF_032_IDX_BY_SIZE(align_size) + 16;
        }
        else if (SMEMINFO::MAX_SHARED_MEMORY_008_SIZE < sz)
        {   // 16x - [SIZE_144=17], 144=9~
            align_size = (DWORD)BUF_ALIGN_016(sz);
            index_by_size = BUF_016_IDX_BY_SIZE(align_size) + 8;
        }
        else
        {   // 8x
            align_size = (DWORD)BUF_ALIGN_008(sz);
            index_by_size = BUF_008_IDX_BY_SIZE(align_size);
        }

        ISharedMemory* shared_memory = ROUTER[index_by_size];
        if (index_by_size && shared_memory)
        {
            PVOID alloc_ptr = shared_memory->Alloc();
            if (alloc_ptr)
            {
                ++SharedMemoryPool::referenced_counter_s;

            #if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
                ;{
                    ++shared_memory->counter_for_allocations_;
                    static DWORD alloc_key_issuer_static = 0;
                    sNODE_GENERNAL* node_base = reinterpret_cast<sNODE_GENERNAL*>(\
                        reinterpret_cast<BYTE*>(alloc_ptr) - offsetof(sNODE_GENERNAL, TYPE));
                    MEMORY_LEAK_CHECKER* leak_checker = &node_base->LEAK_CHECKER;
                    leak_checker->PTR[0] = node_base;
                    leak_checker->_MARKER = ++alloc_key_issuer_static;
                    util::LList::Init(&leak_checker->LIST);
                    util::LList::AddPrev(&leak_checker->LIST, (util::PListNode)allocated_memory_list_s);
                    if (__SharedMemoryLeakDetect) {
                        __SharedMemoryLeakDetect(alloc_key_issuer_static);
                    }
                };
            #endif
            }
            return alloc_ptr;
        }

        return NULL;
    }

    inline void Free(PVOID ptr)
    {
        DWORD token_size = GetSizeN_VerifyBySharedPool(ptr);
        if (token_size /*|| 512보다 크면... 그 이후 관리도 감안해서...*/)
        {
            DWORD index_by_size;
            if (SMEMINFO::MAX_SHARED_MEMORY_032_SIZE < token_size) {
                index_by_size = BUF_064_IDX_BY_SIZE(token_size) + 24;
            }
            else if (SMEMINFO::MAX_SHARED_MEMORY_016_SIZE < token_size) {
                index_by_size = BUF_032_IDX_BY_SIZE(token_size) + 16;
            }
            else if (SMEMINFO::MAX_SHARED_MEMORY_008_SIZE < token_size) {
                index_by_size = BUF_016_IDX_BY_SIZE(token_size) + 8;
            }
            else {
                index_by_size = BUF_008_IDX_BY_SIZE(token_size);
            }

            ISharedMemory* shared_memory = ROUTER[index_by_size];
            if (index_by_size && shared_memory)
            {
                --SharedMemoryPool::referenced_counter_s;

            #if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
                ;{
                    ++shared_memory->counter_for_deallocations_;
                    sNODE_GENERNAL* const node_base = reinterpret_cast<sNODE_GENERNAL*>(\
                        reinterpret_cast<BYTE*>(ptr) - offsetof(sNODE_GENERNAL, TYPE));
                    util::LList::Delete(&node_base->LEAK_CHECKER.LIST);
                };
            #endif
                return shared_memory->Free(ptr);
            }
        }

        //__asm { int 3 };
        //::free(ptr);
        ;{
            char* redirect_ptr = (char*)ptr;
            redirect_ptr -= sizeof(sNODE_GENERNAL);
        #if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
            size_t resized_chunk = ((sNODE_GENERNAL*)redirect_ptr)->HEADER.VERIFIER;
            const bool huge_chunk = 1024 * 1024 < resized_chunk;
            if (huge_chunk) {
                if (BUF_1024x1024_IDX_BY_SIZE(resized_chunk) <
                    _countof(chunk_1024x1024_alloc_counter_static))
                {
                    --chunk_1024x1024_alloc_counter_static[BUF_1024x1024_IDX_BY_SIZE(resized_chunk)];
                }
            }
            else {
                if (BUF_4096_IDX_BY_SIZE(resized_chunk) <
                    _countof(chunk_4096_alloc_counter_static))
                {
                    --chunk_4096_alloc_counter_static[BUF_4096_IDX_BY_SIZE(resized_chunk)];
                }
            }
        #endif
            ::HeapFree(GetProcessHeap(), 0, redirect_ptr);
        };
    }

    ISharedMemory*  ROUTER[SMEMINFO::SIZE_064_MAX];

#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
    void UsedLogOutput()
    {
        OFSTRUCT sof;
        HANDLE handle = (HANDLE)(HFILE)OpenFile("./memory_usage.log", &sof, OF_CREATE);
        DWORD written;
        CHAR strmsg[16000];

        int write_offset = 0;
        for (DWORD i = 1; i < SMEMINFO::SIZE_064_MAX; ++i)
        {
            write_offset += _snprintf(&strmsg[write_offset], _countof(strmsg) - write_offset,
                    "|TokenSize=[%04u]|TotalTokens=%8u|AllocRequest=%8u|FreeRequest=%8u|\r\n",
                    SMEMINFO::SIZEofIDX[i],
                    ROUTER[i]->GetTotalTokens(),
                    ROUTER[i]->counter_for_allocations_, ROUTER[i]->counter_for_deallocations_);
        }
        WriteFile(handle, strmsg, write_offset, &written, NULL);

        write_offset = 0;
        for (int i = 1; i < _countof(chunk_4096_alloc_counter_static); ++i)
        {
            write_offset += _snprintf(&strmsg[write_offset], _countof(strmsg) - write_offset,
                    "|ChunkSize=[~%06u]|Current=%8d|\r\n",
                    i * 4096, chunk_4096_alloc_counter_static[i]);
        }
        WriteFile(handle, strmsg, write_offset, &written, NULL);

        write_offset = 0;
        for (int i = 1; i < _countof(chunk_1024x1024_alloc_counter_static); ++i)
        {
            write_offset += _snprintf(&strmsg[write_offset], _countof(strmsg) - write_offset,
                "|ChunkSize=[~%08u]|Current=%8d|\r\n",
                i * 1024 * 1024, chunk_1024x1024_alloc_counter_static[i]);
        }
        WriteFile(handle, strmsg, write_offset, &written, NULL);

        FlushFileBuffers(handle);
        CloseHandle(handle);
    }
#endif //
};

//==================================================================================================
//
int SharedMemoryPool::referenced_counter_s = 0;
static uint8_t shared_memory_pool_buffer[sizeof(SharedMemoryPool)];
static BYTE shared_memory_router_buffer[sizeof(SharedMemoryRouter)];

//==================================================================================================

void TestUnit_UsedLogOutput()
{
#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
    SharedMemoryRouter* router = reinterpret_cast<SharedMemoryRouter*>(shared_memory_router_buffer);
    router->UsedLogOutput();
#endif //
}

//==================================================================================================

#define SMPOOL_PTR()    reinterpret_cast<SharedMemoryPool*>(shared_memory_pool_buffer)
#define CHECK_SMPOOL_INSTANCED()    (SMPOOL_PTR()->instanced_tag_ == kInstancedTag)
#define SMROUTER_PTR(_pool_instance_) \
    reinterpret_cast<SharedMemoryRouter*>((_pool_instance_)->inter_block_)

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
    // initialized uninitialized buffer
    new (shared_memory_pool_buffer) SharedMemoryPool;
    return true;
}

void SharedMemoryPool::DestroyInstance()
{
    SMPOOL_PTR()->~SharedMemoryPool();
}

SharedMemoryPool::SharedMemoryPool()
    : instanced_tag_(kInstancedTag)
{
    static bool initialized_static = false;

    if (initialized_static == false)
    {
        SMEMINFO::Initialize();
        inter_block_ = new (shared_memory_router_buffer) SharedMemoryRouter;
        initialized_static = true;
        referenced_counter_s = 1;

    #if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
        new (allocated_memory_list_s) MEMORY_LEAK_CHECKER;
        ZeroMemory(chunk_4096_alloc_counter_static, sizeof(chunk_4096_alloc_counter_static));
        ZeroMemory(chunk_1024x1024_alloc_counter_static,
                   sizeof(chunk_1024x1024_alloc_counter_static));
    #endif
    }
}

SharedMemoryPool::~SharedMemoryPool()
{
    static bool release_static = false;

    if (release_static == false)
    {
        if (inter_block_)
        {
            SMROUTER_PTR(this)->~SharedMemoryRouter();
            inter_block_ = NULL;
        }
        release_static = true;
    }
}


void* SharedMemoryPool::MemoryAllocate(SIZE_T sz)
{
    SharedMemoryPool* shared_memory_pool = SharedMemoryPool::Instance();
    void* ptr = SMROUTER_PTR(shared_memory_pool)->Alloc(sz);
    return ptr;
}

void SharedMemoryPool::MemoryDeAllocate(void* ptr)
{
    SharedMemoryPool* shared_memory_pool = SharedMemoryPool::Instance();
    if ((ptr && shared_memory_pool) == false) {
        return;
    }
    SMROUTER_PTR(shared_memory_pool)->Free(ptr);

#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
    if (referenced_counter_s < 6)
    {
        __asm nop;
    }
#endif

    if (referenced_counter_s < 2) {
        SharedMemoryPool::DestroyInstance();
    }
}

void* SharedMemoryPool::RedirectNew_MemoryAllocate(size_t sz)
{
    static void* (*redirect_new_static)(SIZE_T) = &MemoryAllocate;
    // same as MemoryAllocate
    return (*redirect_new_static)(sz);
}

void  SharedMemoryPool::RedirectNew_MemoryDeallocate(void* ptr)
{
    static void (*redirect_new_static)(void*) = &MemoryDeAllocate;
    return (*redirect_new_static)(ptr);
}

//==================================================================================================
//
//BYTE SMEMINFO::IDXofSIZE[MAX_SHARED_MEMORY_008_SIZE];
WORD SMEMINFO::SIZEofIDX[SIZE_064_MAX];

void SMEMINFO::Initialize()
{
    //ZeroMemory(IDXofSIZE, sizeof(IDXofSIZE));
    ZeroMemory(SIZEofIDX, sizeof(SIZEofIDX));

    for (DWORD index=SIZE_8; index<SIZE_008_MAX; ++index)
    {
        WORD align_size = (WORD)BUF_ALIGN_008_BY_IDX(index);
        //IDXofSIZE[byAlignSize] = index;
        SIZEofIDX[index] = align_size;
    }

    for (DWORD index=SIZE_144; index<SIZE_016_MAX; ++index)
    {
        WORD align_size = (WORD)BUF_ALIGN_016_BY_IDX(index - 8);
        //IDXofSIZE[byAlignSize] = index;
        SIZEofIDX[index] = align_size;
    }

    for (DWORD index=SIZE_288; index<SIZE_032_MAX; ++index)
    {
        WORD align_size = (WORD)BUF_ALIGN_032_BY_IDX(index - 16);
        //IDXofSIZE[byAlignSize] = index;
        SIZEofIDX[index] = align_size;
    }

    for (DWORD index=SIZE_576; index<SIZE_064_MAX; ++index)
    {
        WORD align_size = (WORD)BUF_ALIGN_064_BY_IDX(index - 24);
        //IDXofSIZE[byAlignSize] = index;
        SIZEofIDX[index] = align_size;
    }
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
// source node list
//------------------------------------------------------------------------------
#define SMEMROUTER_DECL_LIST() \
    SMEMROUTER_DECL(8,      0, 204 );  /*  4K*/   \
    SMEMROUTER_DECL(16,     0, 9362 ); /*256K*/   \
    SMEMROUTER_DECL(24,     0, 1820 ); /* 64K*/   \
    SMEMROUTER_DECL(32,     0, 1489 ); /* 64K*/   \
    SMEMROUTER_DECL(40,     0, 1260 ); /* 64K*/   \
    SMEMROUTER_DECL(48,     0, 1092 ); /* 64K*/   \
    SMEMROUTER_DECL(56,     0, 60 );   /*  4K*/   \
    SMEMROUTER_DECL(64,     0, 107 );  /*  8K*/   \
    SMEMROUTER_DECL(72,     0, 48 );   /*  4K*/   \
    SMEMROUTER_DECL(80,     0, 356 );  /* 32K*/   \
    SMEMROUTER_DECL(88,     0, 40 );   /*  4K*/   \
    SMEMROUTER_DECL(96,     0, 37 );   /*  4K*/   \
    SMEMROUTER_DECL(104,    0, 35 );   /*  4K*/   \
    SMEMROUTER_DECL(112,    0, 33 );   /*  4K*/   \
    SMEMROUTER_DECL(120,    0, 31 );   /*  4K*/   \
    SMEMROUTER_DECL(128,    0, 234 );  /* 32K*/   \
    SMEMROUTER_DECL(144,    0, 52 );   /*  8K, 16x*/   \
    SMEMROUTER_DECL(160,    0, 190 );  /* 32K*/   \
    SMEMROUTER_DECL(176,    0, 43 );   /*  8K*/   \
    SMEMROUTER_DECL(192,    0, 40 );   /*  8K*/   \
    SMEMROUTER_DECL(208,    0, 37 );   /*  8K*/   \
    SMEMROUTER_DECL(224,    0, 34 );   /*  8K*/   \
    SMEMROUTER_DECL(240,    0, 32 );   /*  8K*/   \
    SMEMROUTER_DECL(256,    0, 30 );   /*  8K*/   \
    SMEMROUTER_DECL(288,    0, 13 );   /*  4K, 32x*/   \
    SMEMROUTER_DECL(320,    0, 12 );   /*  4K*/   \
    SMEMROUTER_DECL(352,    0, 11 );   /*  4K*/   \
    SMEMROUTER_DECL(384,    0, 10 );   /*  4K*/   \
    SMEMROUTER_DECL(416,    0, 9 );    /*  4K*/   \
    SMEMROUTER_DECL(448,    0, 8 );    /*  4K*/   \
    SMEMROUTER_DECL(480,    0, 8 );    /*  4K*/   \
    SMEMROUTER_DECL(512,    0, 7 );    /*  4K*/   \
    SMEMROUTER_DECL(576,    0, 111 );  /* 64K, 64x SCItemSlot*/   \
    SMEMROUTER_DECL(640,    0, 6 );    /*  4K*/   \
    SMEMROUTER_DECL(704,    0, 5 );    /*  4K*/   \
    SMEMROUTER_DECL(768,    0, 5 );    /*  4K*/   \
    SMEMROUTER_DECL(832,    0, 4 );    /*  4K*/   \
    SMEMROUTER_DECL(896,    0, 4 );    /*  4K*/   \
    SMEMROUTER_DECL(960,    0, 4 );    /*  4K*/   \
    SMEMROUTER_DECL(1024,   0, 3 );    /*  4K*/   \
    SMEMROUTER_DECL(1088,   0, 3 );    /*  4K*/ 


//==================================================================================================
//==================================================================================================
//==================================================================================================
//
#define SMEMROUTER_DECL(no, szI, szE)   \
    static unsigned char s_SharedMemoryToken##no[sizeof(SharedMemoryBandwith<no>)]

// declare memory block in .sdata
SMEMROUTER_DECL_LIST();

#undef SMEMROUTER_DECL

//==================================================================================================
//
SharedMemoryRouter::SharedMemoryRouter()
{
#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)

#define SMEMROUTER_DECL(no, szI, szE)   \
    ROUTER[SMEMINFO::SIZE_##no] = new (s_SharedMemoryToken##no) SharedMemoryBandwith<no>(szI, szE); \
    ROUTER[SMEMINFO::SIZE_##no]->counter_for_allocations_      = 0;        \
    ROUTER[SMEMINFO::SIZE_##no]->counter_for_deallocations_    = 0

#else

#define SMEMROUTER_DECL(no, szI, szE)   \
    ROUTER[SMEMINFO::SIZE_##no] = new (s_SharedMemoryToken##no) SharedMemoryBandwith<no>(szI, szE)

#endif
    // create instance nodes
    SMEMROUTER_DECL_LIST();

#undef SMEMROUTER_DECL

}

SharedMemoryRouter::~SharedMemoryRouter()
{
#define SMEMROUTER_DECL(no, szI, szE)   \
    ((SharedMemoryBandwith<no>*)s_SharedMemoryToken##no)->\
        SharedMemoryBandwith<no>::~SharedMemoryBandwith();    \
    ZeroMemory(s_SharedMemoryToken##no, sizeof(s_SharedMemoryToken##no))

    // release instance nodes
    SMEMROUTER_DECL_LIST();

#undef SMEMROUTER_DECL
}

//==================================================================================================

void _SetSharedMemoryLeakDetect(void (*userhook)(DWORD))
{
#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)
    __SharedMemoryLeakDetect = userhook;
#else
    __UNUSED(userhook);
#endif
}

BOOL TraceShareMemoryLeak(const char* const path_file_name)
{
#if defined(USING_SHARED_MEMORY_POOL_LEAK_DETECTOR)

    OFSTRUCT sof;
    HANDLE hOUTPUT = (HANDLE)(HFILE)OpenFile(path_file_name, &sof, OF_CREATE);
    DWORD dwWritten;
    CHAR strmsg[255];

    MEMORY_LEAK_CHECKER* pRoot = &ALLOCED_MEMORY_ENTRY();
    MEMORY_LEAK_CHECKER* pNode = (MEMORY_LEAK_CHECKER*)pRoot->LIST.next;
    for (int max_logging = 100;
         max_logging && (pNode != pRoot);
         pNode = (MEMORY_LEAK_CHECKER*)pNode->LIST.next)
    {
        _snprintf(strmsg, sizeof(strmsg),
                  "\tcase %8u: /*SMLeak {0x%p} vft {0x%p}*/\r\n",
                  pNode->_MARKER, &pNode->PTR[2], pNode->PTR[2]);
        WriteFile(hOUTPUT, strmsg, strlen(strmsg), &dwWritten, NULL);
        --max_logging;
    }

    FlushFileBuffers(hOUTPUT);
    CloseHandle(hOUTPUT);

    return true;

#else

    __UNUSED(path_file_name);
    return false;

#endif
}

#endif //SUN_CODE_BACKUP