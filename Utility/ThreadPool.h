#pragma once
#ifndef UTILITY_THREADPOOL_H
#define UTILITY_THREADPOOL_H

//==================================================================================================
/// IThreadInterface 스레드들을 편리하게 관리하기 위한 메니져 클래스
/**
    @author
    Kim Min Wook < taiyo@webzen.co.kr >	
    @since
        2004. 10. 20
    @remarks
    - IThreadInterface 스레드들을 추가 AddThread() 해서 통합해서 관리한다.
    - BatchStartThread() 스래드들을 일괄 ResumeThread()한다.
    - BatchStopThread() 스래드들을 일괄 EndTread()한다.
    @history
    - 2004.11.11 : utility library에 추가 & util namespace에 등록
    - 2011.05.14 : CMemoryPoolFactory that is thread un-safe version since sun:r53663
                   changes to thread safe version
*/
//==================================================================================================

#include <LinkedListCS.h>

namespace util {
;
class IThreadInterface;

//==================================================================================================

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    void AddThread(IThreadInterface* thread, BOOL suspended = false);
    void SetAutoRelease(BOOL auto_released);
    void BatchStopThread();
    void BatchStartThread();

private:
    struct ThreadNode : public util::__LList<ThreadNode>
    {
        IThreadInterface* thread;
    };
    //
    void _release();
    // data fields...
    bool auto_release_mode_;
    size_t number_of_threads_;
    util::__LList<ThreadNode> thread_list_;
};

}; //end of namespace

//==================================================================================================

inline void util::ThreadPool::SetAutoRelease(BOOL auto_released) {
    auto_release_mode_ = (auto_released != false);
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //UTILITY_THREADPOOL_H