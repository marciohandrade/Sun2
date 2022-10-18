#ifndef UTILITY_SHARED_MEMORY_LEAK_REPORTER_H
#define UTILITY_SHARED_MEMORY_LEAK_REPORTER_H
#pragma once

//==================================================================================================
// shared memory leak report tool interfaces for server
// internal module
//
// @history
//  - |10/02/24|waverix|creation to support shared memory pool node leak detection|


//#ifdef _SERVER

//--------------------------------------------------------------------------------------------------
// declared in SharedMemoryPool.cpp
extern void _SetSharedMemoryLeakDetect(void (*userhook)(DWORD));
extern BOOL TraceShareMemoryLeak(const char* const pszFileName);
//
//==================================================================================================
//
namespace util {
;

//#if SUN_CODE_BACKUP // sample codes
//
//// 하기 2개의 전처리기를 실행파일 main.cpp에서 설정하고 탐지한다.
//// @desc 메모리 누수 리포터 사용 여부
//#define USE_MEMORY_LEAK_REPORT              0
//// @desc 공유된 메모리 누수 출력 파일명
//#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "../_bin/memory_leak_client_shared.txt"
//
//#endif

//#if !defined(MEMORY_LEAK_REPORT_FILE_FOR_SHARED)
//    #error "need defined shared memory reporter configuration"
//#endif

class MemoryLeakReporter
{
public:
    static int __cdecl RegisterSharedMemoryLeakReporter()
    {
        //_CrtSetAllocHook(MemoryAllocHook);
        //_SetSharedMemoryLeakDetect(SharedMemoryAllocHook);
        return 0;
    }
    static int __cdecl StartScanMemoryLeak()
    {
        return !!TraceShareMemoryLeak(NULL);
    }
private:
    //static void SharedMemoryAllocHook(DWORD alloc_number)
    //{
    ////#ifdef _DEBUG
    //    switch(alloc_number)
    //    {
    //    case 0:
    //    #include MEMORY_LEAK_REPORT_FILE_FOR_SHARED
    //        __asm nop; // <- break point
    //        break;
    //    }
    ////#endif
    //}
    // @ref : MSDN sample copied
    static int MemoryAllocHook(int allocType, void* userData, size_t size,
                               int blockType, long requestNumber,
                               const unsigned char* filename, int lineNumber)
    {
        __UNUSED((allocType, userData, size, blockType, filename, lineNumber));
        requestNumber; // used
#pragma warning(push)
#pragma warning(disable : 4065)
        // find specific number logic
        switch (requestNumber)
        {
        /*
        case 9099; // <- 찾고자 하는 번호 기입할 것
            __asm nop; // <- break point;
            break;
        */
        default:
            break;
        };
        // search ranged block
        /*
        if (requestNumber > 1000 && requestNumber < 10000)
        {
            // search size
            if (size == 128) {
                __asm nop; // <- break point
            }
            __asm nop; // <- break point
        }
        */
#pragma warning(pop)
        return 1;
    }
};


}; //end of namespace

//--------------------------------------------------------------------------------------------------
#if USING_SUN_SHARED_POOL_REPORT

// reference : WxFramework v100220
typedef int (__cdecl *WxPIFV)(void);

#pragma data_seg(push, ".CRT$XIZ")
static WxPIFV instance_memory_leak_checker_static = \
    reinterpret_cast<WxPIFV>(&MemoryLeakReporter::RegisterSharedMemoryLeakReporter);
#pragma data_seg(pop)

#pragma data_seg(push, ".CRT$XPB")
static WxPIFV instance_memory_leak_detector_static = \
    reinterpret_cast<WxPIFV>(&MemoryLeakReporter::StartScanMemoryLeak);
#pragma data_seg(pop)

#endif
//--------------------------------------------------------------------------------------------------

//==================================================================================================  
#if USING_SUN_SHARED_POOL
#pragma push_macro("new")
#undef new
void* operator new (size_t size)// _THROW1(std::bad_alloc)  
{  
    return SharedMemoryPool::RedirectNew_MemoryAllocate(size, NULL, 0, NULL);  
}

void* operator new[] (size_t size)// _THROW1(std::bad_alloc)  
{  
    return SharedMemoryPool::RedirectNew_MemoryAllocate(size, NULL, 0, NULL);  
}

void* operator new(size_t size, const char* name, size_t line_no)
{
    return SharedMemoryPool::RedirectNew_MemoryAllocate(size, name, line_no, NULL);  
}

void* operator new[] (size_t size, const char* name, size_t line_no)
{
    return SharedMemoryPool::RedirectNew_MemoryAllocate(size, name, line_no, NULL);  
}

void operator delete (void* ptr)// _THROW0()  
{  
    return SharedMemoryPool::RedirectNew_MemoryDeallocate(ptr);  
}

void operator delete[] (void* ptr)// _THROW0()  
{  
    return SharedMemoryPool::RedirectNew_MemoryDeallocate(ptr);  
}

void  operator delete(void* ptr, const char*, size_t)
{   // unused
    return SharedMemoryPool::RedirectNew_MemoryDeallocate(ptr);  
}

void  operator delete[] (void* ptr, const char*, size_t)
{   // unused
    return SharedMemoryPool::RedirectNew_MemoryDeallocate(ptr);  
}

#pragma pop_macro("new")
#endif //USING_SUN_SHARED_POOL
//==================================================================================================  


#endif //UTILITY_SHARED_MEMORY_LEAK_REPORTER_H
