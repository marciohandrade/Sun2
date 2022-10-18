/*

작성자: 문상현
제작일: 2009-11-10

간단한설명: 네트웍 세션 펙토리

*/

#pragma once

#define SEND_AGENTSERVER   (1<<0)
#define SEND_CLIENT        (1<<1)
#define SEND_BOTH          SEND_AGENTSERVER | SEND_CLIENT

#include "AgentNetworkSession.hpp"
#include "ClientNetworkSession.hpp"

class NetWorkSessionFactory
{
public: 
    enum NETWORKTYPE {AGENTSVR, CLIENT};

    NetWorkSessionFactory()
    {
        m_Agent = new AgentNetWorkSession;
        m_Clinet = new ClientNetWorkSession;
    }
    ~NetWorkSessionFactory()
    {
        delete m_Agent;
        delete m_Clinet;
    }

public:
    INetWorkSession* const GetNetWorkSession(NETWORKTYPE type)
    {
        switch(type)
        {
        case AGENTSVR: return m_Agent;
        case CLIENT: return m_Clinet;
        }
        return NULL;
    }
    
private:
    AgentNetWorkSession*     m_Agent;
    ClientNetWorkSession*    m_Clinet;
};
