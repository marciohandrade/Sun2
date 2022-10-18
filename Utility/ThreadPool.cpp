#include "UtilityCommon.h"
#include "./ThreadPool.h"
#include "./IThreadInterface.h"

//==================================================================================================

util::ThreadPool::ThreadPool()
    : auto_release_mode_(true)
    , number_of_threads_(0)
{
    util::LList::Init(&thread_list_);
}

util::ThreadPool::~ThreadPool()
{
    _release();
}

void util::ThreadPool::_release()
{
    BatchStopThread();
    if (number_of_threads_ == 0) {
        return;
    };

    ;{
        HANDLE* const thread_handles = new HANDLE[number_of_threads_];
        ;{
            HANDLE* thread_it = thread_handles;
            for (ThreadNode* it = thread_list_.next; it != &thread_list_; it = it->next)
            {
                *thread_it++ = it->thread->GetHandle();
            }
        };

        WaitForMultipleObjects(DWORD(number_of_threads_), thread_handles, true, INFINITE);
        delete[] thread_handles;
    };

    ;{
        ThreadNode* it = thread_list_.next;
        while (it != &thread_list_)
        {
            ThreadNode* next_node = it->next;
            //
            if (auto_release_mode_) {
                delete it->thread;
            };
            delete it;
            //
            it = next_node;
        };
        number_of_threads_ = 0;
    };
}

void util::ThreadPool::AddThread(IThreadInterface* thread, BOOL suspended)
{
    ThreadNode* new_node = new ThreadNode;

    util::LList::Init(new_node);
    new_node->thread = thread;
    util::LList::AddPrev(new_node, &thread_list_);
    ++number_of_threads_;

    thread->StartThread(suspended);
}

void util::ThreadPool::BatchStopThread()
{
    for (ThreadNode* it = thread_list_.next; it != &thread_list_; it = it->next)
    {
        IThreadInterface* thread = it->thread;
        thread->EndThread();
    }
}

void util::ThreadPool::BatchStartThread()
{
    for (ThreadNode* it = thread_list_.next; it != &thread_list_; it = it->next)
    {
        IThreadInterface* thread = it->thread;
        thread->ResumeThread();
    }
}
