#include "StdAfx.h"
#include ".\gameagentmanager.h"
#include ".\GameAgentServerSession.h"
#include ".\AuthAgentFrame.h"

GameAgentManager::GameAgentManager(void)
{
}

GameAgentManager::~GameAgentManager(void)
{
}

BOOL
GameAgentManager::AddAgent(DWORD key, GameAgentServerSession* pSession)
{
    std::pair<GAMEAGENT_HASH::iterator, bool> rt =
        m_GameAgentHash.insert(GAMEAGENT_HASH::value_type(key, pSession));
    return rt.second;
}

void
GameAgentManager::RemoveAgent(DWORD key)
{
    GAMEAGENT_HASH::iterator it = m_GameAgentHash.find(key);
    if(it == m_GameAgentHash.end())
        return;

    m_GameAgentHash.erase(it);
}

GameAgentServerSession*
GameAgentManager::FindAgent(DWORD key)
{
    GAMEAGENT_HASH::iterator it = m_GameAgentHash.find(key);
    if(it != m_GameAgentHash.end())
        return it->second;
    return NULL;
}    

