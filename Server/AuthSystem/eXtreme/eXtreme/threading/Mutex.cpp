#include "eXtreme/eXtreme.h"
#include "Mutex.h"

using namespace eXtreme;

Mutex::Mutex()
{
    ::InitializeCriticalSection(&m_cs);
}

Mutex::~Mutex()
{
    ::DeleteCriticalSection(&m_cs);
}

bool Mutex::TryLock()
{
    //  return (TryEnterCriticalSection(&m_cs) == TRUE);
    return FALSE;
}

void Mutex::Lock()
{
    ::EnterCriticalSection(&m_cs);
}

void Mutex::Unlock()
{
    ::LeaveCriticalSection(&m_cs);
}