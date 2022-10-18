#ifndef _MSWINSOCKUTIL_H_
#define _MSWINSOCKUTIL_H_

class MsWinsockUtil
{
public:
    //dll에서 함수 포인터 얻어옴(꼭 활성화된 Socket으로)
    static void LoadExtensionFunction(SOCKET active_socket);

    //AcceptEx 함수 포인터
    static LPFN_ACCEPTEX             lpfnAccepteEx_static_;
    //TransMitFile 함수 포인터 
    static LPFN_TRANSMITFILE         lpfnTransmitFile_static_;
    //GetAcceptExSockaddrs 함수 포인터
    static LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockAddrs_static_;
    //DisconnectEx 함수 포인터.
    static LPFN_DISCONNECTEX         lpfnDisconnectEx_static_;

private:
    static bool LoadExtensionFunction(SOCKET active_socket, GUID function_guid, void** ppFunc);
};

#endif