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
        (it->second)->Update(); // (WAVERIX) (WARNING) 문제 발생하는 지 검증할 것.
        ++it;
    }
}

bool
ClientManager::IsAllowIp(DWORD ip)
{
    MapControlIp::iterator iter;
    DWORD ipStart, ipEnd;
    bool isAllow = false;

    //  받아온 IP를 Host byte order로 변경해야 한다.
    DWORD hbrIP = ::ntohl(ip);

    {
        //  1. 허용된 IP 대역인지를 먼저 검사한다.
        if(m_mapAllowIp.size() == 1)
        {
            //  허용 IP목록이 없다면, 전체 허용으로 설정한다.
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

        //  1.1. 허용된 IP 대역에 한하여 차단된 IP인지를 검사한다.
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

            // 예외 IP인 경우 무조건 허용이다
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

    //  받아온 IP를 Host byte order로 변경해야 한다.
    DWORD hbrIP = ::ntohl(ip);

    {

        //  사내 IP 대역이 없으면 당근 false
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
    //  기본적으로 허용IP와 금지IP, 사내IP, 예외IP에  최소값이 하나씩 세팅되므로, 4을 뺀 값을 리턴한다.
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

    //  upper_bound를 사용하기 때문에 가장 작은 요소는 항상 존재해야만 한다.
    m_mapAllowIp[1] = 1;
    m_mapDenyIp[1] = 1;
    m_mapExceptIP[1] = 1;
    m_mapOfficeIp[1] = 1;
}

void
ClientManager::AddControlIp(eIPTYPE IPType, DWORD ip, DWORD cnt)
{
    //  받아온 IP를 Host byte order로 변경해야 한다.
    DWORD hbrIP = ::ntohl(ip);

    switch(IPType)
    {
    case eIPTYPE_COUNTRY:
        // 국가 IP - 기존의 허용 IP
        m_mapAllowIp[hbrIP] = hbrIP + (cnt - 1);
        break;
    case eIPTYPE_EXCEPT:
        // 예외 IP
        m_mapExceptIP[hbrIP] = hbrIP + (cnt - 1);
        break;
    case eIPTYPE_OFFICE:
        // 사내 IP
        m_mapOfficeIp[hbrIP] = hbrIP + (cnt - 1);
        break;
    case eIPTYPE_INTERCEPT:
        // 금지 IP
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









