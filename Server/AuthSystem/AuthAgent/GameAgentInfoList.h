#pragma once

#include <hash_map>
#include <Singleton.h>



class GameAgentInfoList : public util::Singleton<GameAgentInfoList>
{
public:
    GameAgentInfoList();
    ~GameAgentInfoList();

    VOID    Release();

    // CHANGES: f101030.1L, change the specification to accept multi-agent on same ip address
    bool AddAgentInfo(const sGAME_AGENT_INFO* game_agent_info);
    void RemoveAgentInfo(WORD game_agent_id);
    
    const sGAME_AGENT_INFO* FindAgentInfo(WORD game_agent_id);
    // CHANGES: f101030.1L, change input arguments
    const sGAME_AGENT_INFO* FindAgentInfoForPrivateIPAddress(DWORD ip_address, uint16_t listen_port);

    VOID    Display();

private:
    typedef STLX_HASH_MAP<WORD, sGAME_AGENT_INFO*> GAMEAGENTINFO_HASH;
    GAMEAGENTINFO_HASH m_GameAgentInfoHash;
};
