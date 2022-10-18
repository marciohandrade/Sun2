#pragma once

#include <hash_map>
#include <Singleton.h>

enum eSERVICE_STATE
{
    eSERVICE_ALLOW        = 0,
    eSERVICE_IP_STOP      = (1 << 0),
    eSERVICE_PATTERN_STOP = (1 << 1),
    eSERVICE_STOP         = (1 << 2) - 1, 
};

class ClientSession;

class ClientManager : public util::Singleton<ClientManager>
{
public:
    ClientManager();
    ~ClientManager();

    void UpdateSession();

    BOOL AddSession(DWORD key, ClientSession* pSession);
    void RemoveSession(DWORD key);
    ClientSession* FindSession(DWORD key);

    DWORD GetControlIpSize();
    void  ClearControlIp();
    void  AddControlIp(eIPTYPE IPType, DWORD ip, DWORD cnt);

    bool  IsAllowIp(DWORD ip);
    bool  IsOfficeIp(DWORD ip);

    VOID  DisplayControlIPInfo();

private:
    typedef STLX_HASH_MAP<DWORD, ClientSession*> CLIENT_HASH;
    CLIENT_HASH m_ClientHash;

    typedef STLX_MAP<DWORD, DWORD> MapControlIp;
    MapControlIp m_mapAllowIp,
                 m_mapDenyIp,
                 m_mapExceptIP;
    MapControlIp m_mapOfficeIp;
};



