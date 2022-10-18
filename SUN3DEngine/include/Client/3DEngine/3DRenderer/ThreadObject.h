// threadobject.h

#ifndef _3DRENDERER_THREADOBJECT_H_
#define _3DRENDERER_THREADOBJECT_H_

//------------------------------------------------------------------------------
/**
    @class CThreadObject
*/
class CThreadObject
{
public:
    // ������/�Ҹ���
    CThreadObject();
    ~CThreadObject();

    // ������ ����/����
    void StartThread( void (*pFunc)(void*), void* pParam );
    void EndThread( DWORD dwWaitTime = INFINITE );

private:
    // ������ body
    static unsigned int __stdcall ThreadBody( void* pParam );

private:
    HANDLE m_hThread;
    BOOL m_bEndThread;

    void (*m_pFunction)( void* pParam );
    void* m_pParam;
};

#endif // _3DRENDERER_THREADOBJECT_H_