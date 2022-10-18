#ifndef  __eXtreme_Thread_H
#define  __eXtreme_Thread_H

namespace eXtreme {

class Thread
{
public:
    Thread();
    virtual ~Thread();

    bool Start();
    BOOL Terminate(DWORD exitCode = 0);
    virtual BOOL Run() = 0;

    bool WaitForExitEvent();
    void NotifyExitState();

protected:
    HANDLE m_handleThread;
    DWORD  m_threadID;

    HANDLE m_hExitSynEvent;
    HANDLE m_hExitAckEvent;

private:
    static unsigned int __stdcall Runner(LPVOID parameter);
};

}; //end of namespace 'eXtreme'

#endif