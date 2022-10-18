#include "UtilityCommon.h"
#include "./SafeMemoryPoolFactory.h"

// SafeMemoryPoolFactory.h: interface for the CMemoryPoolFactory class.
//--------------------------------------------------------------------------------------------------

#if 0

#elif SAFE_MEMORY_POOL_VERSION >= 0x0210

namespace util {
;

SafeMemoryPoolFactoryShared::_LoggingHandlerFunc
SafeMemoryPoolFactoryShared::log_handler_static_ = NULL;

void
SafeMemoryPoolFactoryShared::RegisterLoggingHandler(_LoggingHandlerFunc handler)
{
    log_handler_static_ = handler;
}

SafeMemoryPoolFactoryShared::SafeMemoryPoolFactoryShared(const size_t type_size)
    : type_size_(type_size)
    , chunk_size_(0)
    , type_name_(NULL)
{
}

SafeMemoryPoolFactoryShared::~SafeMemoryPoolFactoryShared()
{
    QueueHash::const_iterator it = total_list_.begin(),
                              end = total_list_.end();
    for (; it != end; ++it)
    {
        const NodeContainer& node = it->second;
        if (node.aquired_ != false)
        {
            //리소스 반환하지 않은 사례가 있다.
            Log(type_name_, node.node_ptr_, type_size_);
        #if _DEBUG
            __debugbreak();
        #endif
        }

        if (node.node_ptr_) {
            TAllocFree(node.node_ptr_);
        }
    }
}

void
SafeMemoryPoolFactoryShared::Initialize(size_t chunk_size)
{
    assert(chunk_size  != 0);
    assert(chunk_size_ == 0);

    chunk_size_ = chunk_size;
} 

SafeMemoryPoolFactoryShared::node_type_ptr
SafeMemoryPoolFactoryShared::Alloc()
{
    if (free_queue_.empty()) {
        ExpandSize(chunk_size_);
    }
    NodeContainer* node = free_queue_.front();
    assert(node->aquired_ == false && node->node_ptr_ != 0);
    node->aquired_ = true;
    free_queue_.pop_front();

    ZeroMemory(node->node_ptr_, type_size_);
    return node->node_ptr_;
}

BOOLEAN
SafeMemoryPoolFactoryShared::Free(node_type_ptr ptr)
{
    QueueHash::iterator it = total_list_.find(ptr);
    if (FlowControl::FCAssert(it != total_list_.end()))
    {
        NodeContainer& node = it->second;
        assert(node.aquired_ != false && node.node_ptr_ == ptr);
        node.aquired_ = false;
        free_queue_.push_back(&node);
        return true;
    }
    return false;
}

DWORD
SafeMemoryPoolFactoryShared::GetNumberOfBands() const
{
    return (DWORD)total_list_.size();
}

DWORD
SafeMemoryPoolFactoryShared::GetAvailableNumberOfTypes() const
{
    return (DWORD)free_queue_.size();
}

void
SafeMemoryPoolFactoryShared::SetName(const char* type_name)
{
    type_name_ = type_name;
}

void
SafeMemoryPoolFactoryShared::ExpandSize(size_t add_size)
{
    for (size_t loop = add_size; loop; --loop)
    {
        node_type_ptr token = TAllocMalloc(type_size_);
        NodeContainer& node = total_list_[token];
        node.aquired_ = false;
        node.node_ptr_ = token;
        free_queue_.push_back(&node);
    }
}

void
SafeMemoryPoolFactoryShared::Log(const TCHAR* type_name, void* mem_ptr, size_t mem_size)
{
    if (log_handler_static_ != NULL) {
        log_handler_static_(type_name, mem_ptr, mem_size);
    }
    else {
        printf("[Memory Report]|Release resource problem, alloc/free mismatch - Pool"
               "|Name=%s|Pointer=0x%08X|Size=%d|\n",
               type_name, static_cast<int>(reinterpret_cast<__int64>(mem_ptr)), mem_size);
    }
}

}; //end of namespace

#endif  //SAFE_MEMORY_POOL_VERSION

