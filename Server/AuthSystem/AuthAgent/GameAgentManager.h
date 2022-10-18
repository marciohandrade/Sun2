#pragma once

#include <hash_map>
#include <Singleton.h>

class GameAgentServerSession;

class GameAgentManager : public util::Singleton<GameAgentManager>
{
public:
    GameAgentManager();
    ~GameAgentManager();

    typedef STLX_HASH_MAP<DWORD, GameAgentServerSession*> GAMEAGENT_HASH;

    BOOL AddAgent(DWORD key, GameAgentServerSession* pSession);
    void RemoveAgent(DWORD key);
    GameAgentServerSession* FindAgent(DWORD key);

    template<class Operator>
    void Foreach(Operator& op);

private:
    GAMEAGENT_HASH m_GameAgentHash;
};

//////////////////////////////////////////////////////////////////////////
template<class Operator>
void GameAgentManager::Foreach(Operator & op)
{
    GAMEAGENT_HASH::iterator it = m_GameAgentHash.begin(),
                             end = m_GameAgentHash.end();
    for( ; it != end; ++it)
        op(it->second);
}