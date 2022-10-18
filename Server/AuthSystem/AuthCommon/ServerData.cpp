#include "StdAfx.h"
#include ".\ServerData.h"


ServerData::ServerData()
    : m_GameAgentID(0)
    , m_CrowdBase(0)
    , m_GameAgentPort(0)
    , m_LoginUserCnt(0)
    , m_pAuthAgentSession(NULL)
{
    ZeroMemory(&m_BaseServerData, sizeof(m_BaseServerData));
    m_szGameAgentPublicIP[0] = '\0';
    m_szGameAgentPrivateIP[0] = '\0';
}

BOOL
ServerData::CheckServerCrowd()
{
    m_BaseServerData.m_CrowdGrade = m_LoginUserCnt / m_CrowdBase + 1;
    if(m_BaseServerData.m_CrowdGrade < 9)
        return TRUE;

    return FALSE;
}

void
ServerData::IncLoginUserCount()
{
    m_LoginUserCnt += 1;

    CheckServerCrowd();
}

void
ServerData::DecLoginUserCount()
{
    m_LoginUserCnt -= 1; 
    if(m_LoginUserCnt < 0)
        m_LoginUserCnt = 0;

    CheckServerCrowd();
}
