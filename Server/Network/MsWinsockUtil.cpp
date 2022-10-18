#include "stdafx.h"
#include "MsWinsockUtil.h"

void MsWinsockUtil::LoadExtensionFunction(SOCKET active_socket)
{
    //AcceptEx 함수 가져오기 (dll에서..)
    GUID acceptex_guid = WSAID_ACCEPTEX;
    LoadExtensionFunction(active_socket, acceptex_guid, (void**) &lpfnAccepteEx_static_);

    //TransmitFile 함수 가져오기 (dll에서..)
    GUID transmitfile_guid = WSAID_TRANSMITFILE;
    LoadExtensionFunction(active_socket, transmitfile_guid, (void**) &lpfnTransmitFile_static_);

    //GetAcceptExSockaddrs 함수 가져오기
    GUID guidGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;
    LoadExtensionFunction(active_socket, guidGetAcceptExSockaddrs, (void**) &lpfnGetAcceptExSockAddrs_static_);

    //DisconnectEx 함수 가져오기
    GUID guidDisconnectEx = WSAID_DISCONNECTEX;
    LoadExtensionFunction(active_socket, guidDisconnectEx, (void**) &lpfnDisconnectEx_static_);
}

bool MsWinsockUtil::LoadExtensionFunction(SOCKET active_socket, GUID function_guid, void **ppFunc)
{
    DWORD dwBytes = 0;

    if (0 != WSAIoctl(active_socket, 
                     SIO_GET_EXTENSION_FUNCTION_POINTER,
                     &function_guid,
                     sizeof(GUID),
                     ppFunc,
                     sizeof(void *),
                     &dwBytes,
                     0,
                     0))
    {
        return false;
    }

    return true;
}

LPFN_ACCEPTEX             MsWinsockUtil::lpfnAccepteEx_static_ = NULL;
LPFN_TRANSMITFILE         MsWinsockUtil::lpfnTransmitFile_static_ = NULL;
LPFN_GETACCEPTEXSOCKADDRS MsWinsockUtil::lpfnGetAcceptExSockAddrs_static_ = NULL;
LPFN_DISCONNECTEX         MsWinsockUtil::lpfnDisconnectEx_static_ = NULL;