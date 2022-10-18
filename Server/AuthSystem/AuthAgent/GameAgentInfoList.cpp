#include "StdAfx.h"
#include ".\gameagentinfolist.h"
#include ".\AuthAgentFrame.h"
#include "GameAgentManager.h"

GameAgentInfoList::GameAgentInfoList()
{
}

GameAgentInfoList::~GameAgentInfoList()
{
}

VOID
GameAgentInfoList::Release()
{
    GAMEAGENTINFO_HASH::iterator it = m_GameAgentInfoHash.begin(), end = m_GameAgentInfoHash.end();
    while(it != end)
    {
        SAFE_DELETE(it->second);
        ++it;
    }
    m_GameAgentInfoHash.clear();
}

// CHANGES: f101030.1L, change the specification to accept multi-agent on same ip address
bool GameAgentInfoList::AddAgentInfo(const sGAME_AGENT_INFO* game_agent_info)
{
    sGAME_AGENT_INFO* making_info = new sGAME_AGENT_INFO;
    *making_info = *game_agent_info;
    //
    GAMEAGENTINFO_HASH::_Pairib result = \
        m_GameAgentInfoHash.insert(std::make_pair(making_info->game_agent_id_, making_info));
    bool success = result.second;
    if (success == false) {
        SAFE_DELETE(making_info);
    };
    return success;
}

void
GameAgentInfoList::RemoveAgentInfo(WORD GameAgentID)
{
    GAMEAGENTINFO_HASH::iterator it = m_GameAgentInfoHash.find(GameAgentID);
    if(it == m_GameAgentInfoHash.end())
        return;

    sGAME_AGENT_INFO* pNewInfo = (it->second);
    m_GameAgentInfoHash.erase(it);
    SAFE_DELETE(pNewInfo);
}

const sGAME_AGENT_INFO*
GameAgentInfoList::FindAgentInfo(WORD GameAgentID)
{
    GAMEAGENTINFO_HASH::iterator it = m_GameAgentInfoHash.find(GameAgentID);
    if(it != m_GameAgentInfoHash.end())
        return it->second;
    return NULL;
}

// CHANGES: f101030.1L, change input arguments
const sGAME_AGENT_INFO*
GameAgentInfoList::FindAgentInfoForPrivateIPAddress(DWORD ip_address, uint16_t listen_port)
{
    GAMEAGENTINFO_HASH::const_iterator it = m_GameAgentInfoHash.begin(),
                                       end = m_GameAgentInfoHash.end();
    for ( ; it != end; ++it)
    {
        const sGAME_AGENT_INFO* agent_info = it->second;
    #ifdef _NA000000_CONFIGURATION_TO_ACCEPT_MULTI_AGENT_ON_SAME_IP_ADDRESS
        if (agent_info->game_agent_port_ != listen_port) {
            continue;
        }
    #endif
        DWORD src_ip_address = IPOperation::ConvertStrIP2DWORD(agent_info->game_agent_private_ip_);
        if (src_ip_address != ip_address) {
            continue;
        };
        // found!
        return agent_info;
    }
    return NULL;
}

VOID
GameAgentInfoList::Display()
{
    SUNLOG(eCRITICAL_LOG,
           "[GAMEAGENTID] [Port] [GameAgentPublicIP] [GameAgentPrivateIP] [ConnectStatus] [MAX] : %d",
           m_GameAgentInfoHash.size());

    GameAgentManager* pGameAgentManager = GameAgentManager::Instance();
    GAMEAGENTINFO_HASH::iterator it = m_GameAgentInfoHash.begin(),
                                 end = m_GameAgentInfoHash.end();
    for( ; it != end; ++it)
    {
        sGAME_AGENT_INFO* pInfo = it->second;

        BOOL bConnectStatus = FALSE;
        if(pGameAgentManager->FindAgent(pInfo->game_agent_id_))
            bConnectStatus = TRUE;

        SUNLOG(eCRITICAL_LOG, "%d    %u    %s    %s    %d \n",
               pInfo->game_agent_id_, pInfo->game_agent_port_,
               pInfo->game_agent_public_ip_, pInfo->game_agent_private_ip_,
               bConnectStatus);
    }
}

















