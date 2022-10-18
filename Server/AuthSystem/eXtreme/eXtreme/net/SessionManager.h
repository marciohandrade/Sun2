#ifndef  __eXtreme_SessionManager_H
#define  __eXtreme_SessionManager_H

#include <map>
#include <list>

#include "./ServerSession.h"
#include "eXtreme/threading/Mutex.h"

namespace eXtreme {

class SessionManager
{
    friend class ServerNetwork;

public:
    eXServerSession* Find(DWORD uid);

    eXServerSession* BindSession(SOCKET s, Socket::SocketAddr* addr);
    bool ActiveSession(eXServerSession* session);
    void InactiveSession(eXServerSession* session);

    int  GetActiveSessionCnt()   { return (int)m_mapActive.size(); }
    int  GetInactiveSessionCnt() { return (int)m_pool.size();      }

protected:
    SessionManager();
    virtual ~SessionManager();

    virtual eXServerSession* CreateSession();
    virtual void DeleteSession(eXServerSession* session);

    virtual void Update();
    virtual void UpdateInactive();

private:
    bool Create(int sessionCnt);
    void Destroy();


    typedef std::map<DWORD, eXServerSession*>   MapActive;
    typedef std::list<eXServerSession*>         ListPool;

    MapActive      m_mapActive;
    ListPool       m_pool;
    Mutex          m_mutexActive, m_mutexPool;
    ServerNetwork* m_serverNetwork;
};

}; //end of namespace 'eXtreme'

#endif