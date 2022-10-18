#ifndef UTILITY_SHARED_MEMORY_LEAK_REPORTER_H
#define UTILITY_SHARED_MEMORY_LEAK_REPORTER_H
#pragma once

//==================================================================================================
// shared memory leak report tool interfaces for server
// internal module
//
// @history
//  - |10/02/24|waverix|creation to support shared memory pool node leak detection|


#ifdef _SERVER

//--------------------------------------------------------------------------------------------------
// declared in SharedMemoryPool.cpp
extern void _SetSharedMemoryLeakDetect(void (*userhook)(DWORD));
extern BOOL TraceShareMemoryLeak(const char* const pszFileName);
//
//==================================================================================================
//
namespace util {
;

#if !defined(MEMORY_LEAK_REPORT_FILE_FOR_SHARED)
    #error "need defined shared memory reporter configuration"
#endif

class MemoryLeakReporter
{
public:
    static int __cdecl RegisterSharedMemoryLeakReporter()
    {
        _CrtSetAllocHook(MemoryAllocHook);
        _SetSharedMemoryLeakDetect(SharedMemoryAllocHook);
        return 0;
    }
    static int __cdecl StartScanMemoryLeak()
    {
        return !!TraceShareMemoryLeak(MEMORY_LEAK_REPORT_FILE_FOR_SHARED);
    }
private:
    static void SharedMemoryAllocHook(DWORD alloc_number)
    {
    //#ifdef _DEBUG
        switch(alloc_number)
        {
        case 0:
        #include MEMORY_LEAK_REPORT_FILE_FOR_SHARED
            __asm nop; // <- break point
            break;
        }
    //#endif
    }
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

#pragma data_seg(push, ".CRT$XIC")
static WxPIFV instance_memory_leak_checker_static = \
    reinterpret_cast<WxPIFV>(&util::MemoryLeakReporter::RegisterSharedMemoryLeakReporter);
#pragma data_seg(pop)

#pragma data_seg(push, ".CRT$XPY")
static WxPIFV instance_memory_leak_detector_static = \
    reinterpret_cast<WxPIFV>(&util::MemoryLeakReporter::StartScanMemoryLeak);
#pragma data_seg(pop)

#endif
//--------------------------------------------------------------------------------------------------

#endif //_SERVER

#endif //UTILITY_SHARED_MEMORY_LEAK_REPORTER_H
