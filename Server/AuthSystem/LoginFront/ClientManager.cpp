#include "StdAfx.h"
#include ".\clientmanager.h"
#include ".\ClientSession.h"
#include <ServerGlobal.h>

#ifdef _JP_0_20101123_PMS
#include "LoginFrontFrame.h"
#endif

ClientManager::ClientManager(void)
{
}

ClientManager::~ClientManager(void)
{
}

BOOL
ClientManager::AddSession(DWORD key, ClientSession* pSession)
{
    std::pair<CLIENT_HASH::iterator, bool> rt =
        m_ClientHash.insert(CLIENT_HASH::value_type(key, pSession));
    if(rt.second)
    {
        pSession->Init();
#ifdef _JP_0_20101123_PMS
        LoginFrontFrame::Instance()->SetTotalNumberOfConnections();
#endif
    }
    return rt.second;
}

void
ClientManager::RemoveSession(DWORD key)
{
	CLIENT_HASH::iterator it = m_ClientHash.find(key);
	if(it == m_ClientHash.end())
		return;

	(it->second)->Release();
#ifdef _JP_0_20101123_PMS
    LoginFrontFrame::Instance()->SetTotalNumberOfConnections();
#endif
	m_ClientHash.erase(it);
}

ClientSession*
ClientManager::FindSession(DWORD key)
{
	CLIENT_HASH::iterator it = m_ClientHash.find(key);
	if(it != m_ClientHash.end())
		return it->second;
	return NULL;
}	

void
ClientManager::UpdateSession()
{
    CLIENT_HASH::iterator it = m_ClientHash.begin(),
        end = m_ClientHash.end();
    while(it != end)
    {
        BOOST_STATIC_WARNING(0);
        (it->second)->Update(); // (WAVERIX) (WARNING) ���� �߻��ϴ� �� ������ ��.
        ++it;
    }
}

bool
ClientManager::IsAllowIp(DWORD ip)
{
    MapControlIp::iterator iter;
    DWORD ipStart, ipEnd;
    bool isAllow = false;

    //  �޾ƿ� IP�� Host byte order�� �����ؾ� �Ѵ�.
    DWORD hbrIP = ::ntohl(ip);

    {
        //  1. ���� IP �뿪������ ���� �˻��Ѵ�.
        if(m_mapAllowIp.size() == 1)
        {
            //  ��� IP����� ���ٸ�, ��ü ������� �����Ѵ�.
            isAllow = true;
        }
        else
        {
            iter = m_mapAllowIp.upper_bound(hbrIP);
            iter--;

            ipStart = iter->first;
            ipEnd   = iter->second;
            if(ipStart <= hbrIP && ipEnd >= hbrIP)
            {
                isAllow = true;
            }
        }

        //  1.1. ���� IP �뿪�� ���Ͽ� ���ܵ� IP������ �˻��Ѵ�.
        if(isAllow == true)
        {
            iter = m_mapDenyIp.upper_bound(hbrIP);
            iter--;

            ipStart = iter->first;
            ipEnd   = iter->second;
            if(ipStart <= hbrIP && ipEnd >= hbrIP)
            {
                isAllow = false;
            }

            // ���� IP�� ��� ������ ����̴�
            if(m_mapExceptIP.size() > 1)
            {
                iter = m_mapExceptIP.upper_bound(hbrIP);
                iter--;

                ipStart = iter->first;
                ipEnd   = iter->second;
                if(ipStart <= hbrIP && ipEnd >= hbrIP)
                    isAllow = true;
            }
        }
    }

    return isAllow;
}

bool
ClientManager::IsOfficeIp(DWORD ip)
{
    MapControlIp::iterator	iter;
    DWORD ipStart, ipEnd;
    bool isAllow = false;

    //  �޾ƿ� IP�� Host byte order�� �����ؾ� �Ѵ�.
    DWORD hbrIP = ::ntohl(ip);

    {

        //  �系 IP �뿪�� ������ ��� false
        if(m_mapOfficeIp.size() == 1)
        {
            return false;
        }
        else
        {
            iter = m_mapOfficeIp.upper_bound(hbrIP);
            iter--;

            ipStart	= iter->first;
            ipEnd	= iter->second;
            if(ipStart <= hbrIP && ipEnd >= hbrIP)
                return true;
        }
    }

    return false;
}

DWORD
ClientManager::GetControlIpSize()
{
    //  �⺻������ ���IP�� ����IP, �系IP, ����IP��  �ּҰ��� �ϳ��� ���õǹǷ�, 4�� �� ���� �����Ѵ�.
    return (DWORD)(m_mapAllowIp.size() +
                   m_mapDenyIp.size() +
                   m_mapExceptIP.size() +
                   m_mapOfficeIp.size() -
                   4);
}

void
ClientManager::ClearControlIp()
{
    m_mapAllowIp.clear();
    m_mapDenyIp.clear();
    m_mapExceptIP.clear();
    m_mapOfficeIp.clear();

    //  upper_bound�� ����ϱ� ������ ���� ���� ��Ҵ� �׻� �����ؾ߸� �Ѵ�.
    m_mapAllowIp[1] = 1;
    m_mapDenyIp[1] = 1;
    m_mapExceptIP[1] = 1;
    m_mapOfficeIp[1] = 1;
}

void
ClientManager::AddControlIp(eIPTYPE IPType, DWORD ip, DWORD cnt)
{
    //  �޾ƿ� IP�� Host byte order�� �����ؾ� �Ѵ�.
    DWORD hbrIP = ::ntohl(ip);

    switch(IPType)
    {
    case eIPTYPE_COUNTRY:
        // ���� IP - ������ ��� IP
        m_mapAllowIp[hbrIP] = hbrIP + (cnt - 1);
        break;
    case eIPTYPE_EXCEPT:
        // ���� IP
        m_mapExceptIP[hbrIP] = hbrIP + (cnt - 1);
        break;
    case eIPTYPE_OFFICE:
        // �系 IP
        m_mapOfficeIp[hbrIP] = hbrIP + (cnt - 1);
        break;
    case eIPTYPE_INTERCEPT:
        // ���� IP
        m_mapDenyIp[hbrIP] = hbrIP + (cnt - 1);
        break;
    default:
        break;
    }
}

VOID
ClientManager::DisplayControlIPInfo()
{
    int StartIP = 0;
    int Count = 0;

    for(MapControlIp::iterator it = m_mapAllowIp.begin() ; it != m_mapAllowIp.end() ; it++)
    {
        StartIP = ::ntohl(it->first);
        Count = it->second - it->first + 1;
        if(it->first != 1)
            SUNLOG(eCRITICAL_LOG, "IPType[%d] %s + Count %d", eIPTYPE_COUNTRY, IPOperation::ConvertDWORD2StrIP(StartIP), Count);
    }

    for(MapControlIp::iterator it = m_mapExceptIP.begin() ; it != m_mapExceptIP.end() ; it++)
    {
        StartIP = ::ntohl(it->first);
        Count = it->second - it->first + 1;
        if(it->first != 1)
            SUNLOG(eCRITICAL_LOG, "IPType[%d] %s + Count %d", eIPTYPE_EXCEPT, IPOperation::ConvertDWORD2StrIP(StartIP), Count);
    }

    for(MapControlIp::iterator it = m_mapOfficeIp.begin() ; it != m_mapOfficeIp.end() ; it++)
    {
        StartIP = ::ntohl(it->first);
        Count = it->second - it->first + 1;
        if(it->first != 1)
            SUNLOG(eCRITICAL_LOG, "IPType[%d] %s + Count %d", eIPTYPE_OFFICE, IPOperation::ConvertDWORD2StrIP(StartIP), Count);
    }

    for(MapControlIp::iterator it = m_mapDenyIp.begin() ; it != m_mapDenyIp.end() ; it++)
    {
        StartIP = ::ntohl(it->first);
        Count = it->second - it->first + 1;
        if(it->first != 1)
            SUNLOG(eCRITICAL_LOG, "IPType[%d] %s + Count %d", eIPTYPE_INTERCEPT, IPOperation::ConvertDWORD2StrIP(StartIP), Count);
    }
}









