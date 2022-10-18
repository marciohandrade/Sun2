#include "StdAfx.h"
#include "./SunNetworkApi.h"

#include "../FrostShieldLib/shieldSecurity.h"

//! 프로젝트 의존성
//#ifdef _DEBUG
//    #pragma comment(lib, "../lib/gameShieldLib_d.lib") 
//#else
//    #pragma comment(lib, "../lib/gameShieldLib.lib") 
//#endif

namespace sun_network_api
{
    NetworkSecurity network_security_ = kNetworkSecurity_none;

    //------------------------------------------------------------------------------
    NetworkSecurity get_network_security()
    { 
        return network_security_; 
    }
    //------------------------------------------------------------------------------
    void set_network_security(NetworkSecurity network_security)
    {
        network_security_ = network_security;
    }

    //------------------------------------------------------------------------------
    void sun_initialize()
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            frostInitialize("./Frost/gameShieldDll.dll");
        }
    }
    //------------------------------------------------------------------------------
    void sun_finalize()
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            frostFinalize();
        }
    }
    //------------------------------------------------------------------------------
    void sun_set_username(const char* user_name)
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            frostSetUserName(user_name);
        }
    }
    //------------------------------------------------------------------------------
    int sun_connect(SOCKET s, const struct sockaddr* name, int namelen)
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            return frostConnect(s, name, namelen);
        }
        return connect(s, name, namelen);
    }
    //------------------------------------------------------------------------------
    int sun_send(SOCKET s, const char* buf, int len, int flags)
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            return frostSend(s, buf, len, flags);
        }
        return send(s, buf, len, flags);
    }
    //------------------------------------------------------------------------------
    int sun_WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, 
                    DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped, 
                    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            return frostWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, 
                        dwFlags, lpOverlapped, lpCompletionRoutine);
        }
        return WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, 
                        dwFlags, lpOverlapped, lpCompletionRoutine);
    }
    //------------------------------------------------------------------------------
    int sun_sendto(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            return frostSendto(s, buf, len, flags, to, tolen);
        }
        return sendto(s, buf, len, flags, to, tolen);
    }
    //------------------------------------------------------------------------------
    int sun_recv(SOCKET s, char* buf, int len, int flags)
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            return frostRecv(s, buf, len, flags);
        }
        return recv(s, buf, len, flags);
    }
    //------------------------------------------------------------------------------
    int sun_WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, 
                    LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped, 
                    LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            return frostWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, 
                        lpFlags, lpOverlapped, lpCompletionRoutine);
        }
        return WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, 
                        lpFlags, lpOverlapped, lpCompletionRoutine);
    }
    //------------------------------------------------------------------------------
    int sun_recvfrom(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen)
    {
        if (get_network_security() == kNetworkSecurity_Frost)
        {
            return frostRecvfrom(s, buf, len, flags, from, fromlen);
        }
        return recvfrom(s, buf, len, flags, from, fromlen);
    }
#ifdef _DEBUG
    //------------------------------------------------------------------------------
    NetworkDebugFile::NetworkDebugFile(const char* filename, DWORD packet_size)
    {
        strcpy(filename_, filename);
        packet_size_ = packet_size;
        start_time_ = GetTickCount();
    }
    //------------------------------------------------------------------------------
    NetworkDebugFile::~NetworkDebugFile()
    {
        FILE* fp = fopen(filename_, "at");
        if (fp)
        {
            fprintf(fp, "%d, %d\n", packet_size_, GetTickCount() - start_time_);
            fclose(fp);
        }
    }
#endif
};
