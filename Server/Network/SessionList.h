#ifndef NETWORK_MODULE_SESSIONLIST_H
#define NETWORK_MODULE_SESSIONLIST_H

//==================================================================================================

#include <process.h>
#include <list>

class Session;

typedef std::list<Session*, module::RedirectedAllocator<Session*> > SESSION_LIST;
//typedef SESSION_LIST::iterator SESSION_LIST_ITER;

//==================================================================================================
/// 동기화를 제공하는 세션 해쉬맵
//==================================================================================================
//
class SessionList : public SESSION_LIST
{
public:
    SessionList();
    ~SessionList();

    void Clear();

    void Lock() { EnterCriticalSection(&m_cs); }
    void Unlock() { LeaveCriticalSection(&m_cs); }

private:
    CRITICAL_SECTION m_cs;
};

#endif //NETWORK_MODULE_SESSIONLIST_H
