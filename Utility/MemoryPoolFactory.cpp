#include "UtilityCommon.h"

#include "MemoryPoolFactory.h"
//#include "MemoryPoolFactoryV.h"
//#include "MemoryPoolFactoryV2.h"


#if 0

//==================================================================================================
//==================================================================================================
//==================================================================================================
#elif MEMORY_POOL_FACTORY_VERSION >= 0x0300
//==================================================================================================
//==================================================================================================
//==================================================================================================

// 2007-6-15	:	Refactoring (Changed to virtual Memory Pool Router & Enhanced Exception Handling)
//                  ver 2.1 - written by Waverix
// 2011-05-14   : ver 3.0, refactoring, changes internal data structure
//                using shared memory architecture after client application.
//                this implements are stripped version based on WxFramework::MemoryPoolSizeBased
//                and WxFramework::MemoryPool.
util::IVMemoryPool::~IVMemoryPool()
{
}

namespace util { namespace internal {
;

struct MemoryPoolSizeBased::Token : public util::__LList<Token>
{
    void* ptr_;
};

struct MemoryPoolSizeBased::TokenBlock : public util::__LList<TokenBlock>
{
#pragma warning(push)
#pragma warning(disable:4200)
    MemoryPoolSizeBased::Token stream_[0];
#pragma warning(pop)
};

// NOTE: f110525.6L
// specification
// -------------------------------------------------------------------------------------------------
// | SharedMemoryHeaderSection |   NodeHdrPtr   | Node...                     |
// | alloc/free & leak check   | pool node mark | result of new, deletion ptr |
struct NodeHdrPtr
{
    void* parent_;
};

}}; //end of namespace
//==================================================================================================
//==================================================================================================
//==================================================================================================

util::internal::MemoryPoolSizeBased::MemoryPoolSizeBased(
    size_t type_size,
    func_object_lifetime instance_func,
    func_object_lifetime delete_func)
    : type_size_(type_size + sizeof(util::internal::NodeHdrPtr))
    , init_count_(0)
    , growth_count_(0)
    , type_name_("")
    , instance_func_(instance_func)
    , delete_func_(delete_func)
{
    util::LList::Init(&free_list_);
    util::LList::Init(&alloc_list_);
    util::LList::Init(&total_list_);
    ZeroMemory(&statistics_, sizeof(statistics_));
}

void util::internal::MemoryPoolSizeBased::Initialize(
    size_t init_size, size_t chunk_size, const char* type_name)
{
    init_count_ = init_size;
    growth_count_ = chunk_size;
    if (growth_count_ == 0) {
        chunk_size = (init_count_ >> 1);
        growth_count_ = (chunk_size == 0) ? 1 : chunk_size;
    };
    type_name_ = type_name ? type_name : "";

    if (init_count_) {
        AddBlock(init_count_);
    };
}

util::internal::MemoryPoolSizeBased::~MemoryPoolSizeBased()
{
    FreeAllElements();
}


void* util::internal::MemoryPoolSizeBased::Allocate()
{
    if (util::LList::IsEmpty(&free_list_)) {
        AddBlock(growth_count_);
    };
    Token* token = free_list_.next;
    util::LList::Delete(token);
    --statistics_.number_of_free_nodes;
    // 'util::internal::NodeHdrPtr*' == 'token->ptr_'
    util::internal::NodeHdrPtr* hdr = ((util::internal::NodeHdrPtr*)token->ptr_);
    hdr->parent_ = token; // CHANGES: f110708.1L
    //
    util::LList::AddPrev(token, &alloc_list_);
    ++statistics_.number_of_alloc_nodes;
    //
    void* ptr = hdr + 1;
    return ptr;
}

void util::internal::MemoryPoolSizeBased::Deallocate(void* element)
{
    // sync 꼬이는 문제가 발생할 수 있다. 좀 더 검증할 것.
    if (FlowControl::FCAssert(util::LList::IsEmpty(&alloc_list_) == false))
    {
        // '(util::internal::NodeHdrPtr*) + 1' == user control pointer
        util::internal::NodeHdrPtr* hdr = ((util::internal::NodeHdrPtr*)(element)) - 1;
    #if USING_SUN_SHARED_POOL
        // CHANGES: f110811.3L, fixed the client (user released version) memory leaks
        if (FlowControl::FCAssert(SharedMemoryPool::CheckValidAllocatedMemory(hdr)) == false) {
            return; // corrupted memory, leak occurrence
        };
    #endif
        if (hdr == NULL || hdr->parent_ == NULL) {
            return;
        }
        Token* token = reinterpret_cast<Token*>(hdr->parent_);
        if (FlowControl::FCAssert(token->ptr_ == hdr) == false) {
            return; // leak occurrence
        };
        // CHANGES: f110708.1L, prevent the overlapped free operation like the sun:r54580 case
        hdr->parent_ = NULL;
        //
        util::LList::Delete(token);
        --statistics_.number_of_alloc_nodes;

        util::LList::AddPrev(token, &free_list_);
        ++statistics_.number_of_free_nodes;
    }
}

void util::internal::MemoryPoolSizeBased::AddBlock(size_t element_count)
{
    const int token_node_size = int(sizeof(Token) * element_count);
    const int elements_size = int(element_count * type_size_);
    __TOUCHED(elements_size);
    //
#ifdef USING_SUN_SHARED_POOL_EXCEPT_MALLOC
    TokenBlock* block = (TokenBlock*)SharedMemoryPool::MemoryAllocate((sizeof(TokenBlock) + token_node_size), NULL, 0);
#else
    TokenBlock* block = (TokenBlock*)TAllocMalloc(sizeof(TokenBlock) + token_node_size);
#endif //USING_SUN_SHARED_POOL_EXCEPT_MALLOC
    ZeroMemory(block, sizeof(TokenBlock) + token_node_size);
    util::LList::Init(block);
    util::LList::AddPrev(block, &total_list_);
    ++statistics_.number_of_total_blocks;

    Token* const tokens = block->stream_;
    Token* token = tokens;
    for (size_t i = 0; i < element_count; ++i, ++token)
    {
        // init tokens
        util::LList::Init(token);
#ifdef USING_SUN_SHARED_POOL_EXCEPT_MALLOC
        token->ptr_ = SharedMemoryPool::MemoryAllocate(type_size_, NULL, 0);
#else
        token->ptr_ = TAllocMalloc(type_size_);
#endif //USING_SUN_SHARED_POOL_EXCEPT_MALLOC
        // 'util::internal::NodeHdrPtr*' == 'token->ptr_'
        util::internal::NodeHdrPtr* hdr = ((util::internal::NodeHdrPtr*)token->ptr_);
        hdr->parent_ = token;
        if (instance_func_) {
            instance_func_(hdr + 1);
        };
        util::LList::AddPrev(token, &free_list_);
        ++statistics_.number_of_free_nodes;
    };
}

#if _BUILD_FLAG_SHARED_MEMORY_NEWDEL_SYNC_CHECK_
void util::internal::MemoryPoolSizeBased::DeadBeef(Element* element)
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

void util::internal::MemoryPoolSizeBased::FreeAllElements()
{
    ;{
        size_t counter = 0;
        Token* root = static_cast<Token*>(&free_list_);
        Token* token = root->next;
        while (token != root)
        {
            Token* next_token = util::LList::Delete(token);
            util::internal::NodeHdrPtr* hdr =
                reinterpret_cast<util::internal::NodeHdrPtr*>(token->ptr_);
            void* ptr = hdr + 1;
            if (delete_func_) {
                delete_func_(ptr);
            };
#ifdef USING_SUN_SHARED_POOL_EXCEPT_MALLOC
            SharedMemoryPool::MemoryDeAllocate(hdr);
#else
            TAllocFree(hdr);
#endif //USING_SUN_SHARED_POOL_EXCEPT_MALLOC

            token = next_token;
            ++counter;
        };
        if (FlowControl::FCAssert(statistics_.number_of_free_nodes == counter) == false) {
            SUNLOG(eCRITICAL_LOG, _T("|Msg=mismatched free nodes (%u:%u) in '%s' factory"),
                   statistics_.number_of_free_nodes, counter, type_name_);
        };
        statistics_.number_of_free_nodes = 0;
    };
    ;{
        size_t counter = 0;
        Token* root = static_cast<Token*>(&alloc_list_);
        Token* token = root->next;
        while (token != root)
        {
            Token* next_token = util::LList::Delete(token);
            util::internal::NodeHdrPtr* hdr =
                reinterpret_cast<util::internal::NodeHdrPtr*>(token->ptr_);
            void* ptr = hdr + 1;
            if (delete_func_) {
                delete_func_(ptr);
            };
#ifdef USING_SUN_SHARED_POOL_EXCEPT_MALLOC
            SharedMemoryPool::MemoryDeAllocate(hdr);
#else
            TAllocFree(hdr);
#endif //USING_SUN_SHARED_POOL_EXCEPT_MALLOC

            token = next_token;
            ++counter;
        }
        if (FlowControl::FCAssert(statistics_.number_of_alloc_nodes == counter) == false) {
            SUNLOG(eCRITICAL_LOG, _T("|Msg=mismatched allocated nodes (%u:%u) in '%s' factory"),
                   statistics_.number_of_alloc_nodes, counter, type_name_);
        };
        statistics_.number_of_alloc_nodes = 0;
    };
    ;{
        size_t counter = 0;
        TokenBlock* root = static_cast<TokenBlock*>(&total_list_);
        TokenBlock* token = root->next;
        while (token != root)
        {
            TokenBlock* next_token = util::LList::Delete(token);
#ifdef USING_SUN_SHARED_POOL_EXCEPT_MALLOC
            SharedMemoryPool::MemoryDeAllocate(token);
#else
            TAllocFree(hdr);
#endif //USING_SUN_SHARED_POOL_EXCEPT_MALLOC
            token = next_token;
            ++counter;
        }
        if (FlowControl::FCAssert(statistics_.number_of_total_blocks == counter) == false) {
            SUNLOG(eCRITICAL_LOG, _T("|Msg=mismatched token block nodes (%u:%u) in '%s' factory"),
                   statistics_.number_of_total_blocks, counter, type_name_);
        };
        statistics_.number_of_total_blocks = 0;
    };
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
#elif MEMORY_POOL_FACTORY_VERSION >= 0x0210
//==================================================================================================
//==================================================================================================
//==================================================================================================


namespace util
{

	VOID	AssertionLogger4Pool_Default( DWORD errTYPE, ePOOL_ERROR* IN pINFO )
	{
		TCHAR* pErrTypeMsg = ePOOL_ERROR::INVALID_REFERENCE == errTYPE ? _T("Freeing Address is Can't Accessable")
			: ePOOL_ERROR::CANT_ALLOCATABLE == errTYPE ? _T("Can't Allocatable")
			: ePOOL_ERROR::CANT_DEALLOCATABLE == errTYPE ? _T("Can't DeAllocatable, STAT(Already Freed | Invalid Reference | Overrun)")
			: ePOOL_ERROR::NOT_MATCHED_ALLOCnDEALLOC == errTYPE ? _T("Free Count is not matched AllocCount")
			: NULL;

		//MessageOut( 3, _T("[POOL ASSERTION][TYPE:%s] \"%s\""), pINFO->m_pszTYPENAME, pErrTypeMsg );
		printf( _T("[POOL ASSERTION][TYPE:%s] \"%s\"\n"), pINFO->m_pszTYPENAME, pErrTypeMsg );

#	ifdef _DEBUG
		__debugbreak();
#	endif //
	}

} //

//==================================================================================================
//==================================================================================================
//==================================================================================================
#endif //MEMORY_POOL_FACTORY_VERSION >= 0x0210
//==================================================================================================
//==================================================================================================
//==================================================================================================
