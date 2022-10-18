#pragma once
#ifndef UTILITY_THREADPOOL_H
#define UTILITY_THREADPOOL_H

//==================================================================================================
/// IThreadInterface ��������� ���ϰ� �����ϱ� ���� �޴��� Ŭ����
/**
    @author
    Kim Min Wook < taiyo@webzen.co.kr >	
    @since
        2004. 10. 20
    @remarks
    - IThreadInterface ��������� �߰� AddThread() �ؼ� �����ؼ� �����Ѵ�.
    - BatchStartThread() ��������� �ϰ� ResumeThread()�Ѵ�.
    - BatchStopThread() ��������� �ϰ� EndTread()�Ѵ�.
    @history
    - 2004.11.11 : utility library�� �߰� & util namespace�� ���
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