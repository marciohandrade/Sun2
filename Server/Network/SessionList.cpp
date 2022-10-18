#include "stdafx.h"
#include "SessionList.h"
#include "Session.h"

SessionList::SessionList()
{
    InitializeCriticalSectionAndSpinCount(&m_cs, 4000);
}

SessionList::~SessionList()
{
    Clear();
    DeleteCriticalSection(&m_cs);
}

void SessionList::Clear()
{
    Lock();
    SESSION_LIST::iterator it = this->begin();
    SESSION_LIST::const_iterator end = this->end();
    for (; it != end; ++it)
    {
        Session* session = *it;
        module::TAllocator<Session>::DeAllocate(session);
    }
    Unlock();

    clear();
}
