#ifndef _MSWINSOCKUTIL_H_
#define _MSWINSOCKUTIL_H_

class MsWinsockUtil
{
public:
    //dll���� �Լ� ������ ����(�� Ȱ��ȭ�� Socket����)
    static void LoadExtensionFunction(SOCKET active_socket);

    //AcceptEx �Լ� ������
    static LPFN_ACCEPTEX             lpfnAccepteEx_static_;
    //TransMitFile �Լ� ������ 
    static LPFN_TRANSMITFILE         lpfnTransmitFile_static_;
    //GetAcceptExSockaddrs �Լ� ������
    static LPFN_GETACCEPTEXSOCKADDRS lpfnGetAcceptExSockAddrs_static_;
    //DisconnectEx �Լ� ������.
    static LPFN_DISCONNECTEX         lpfnDisconnectEx_static_;

private:
    static bool LoadExtensionFunction(SOCKET active_socket, GUID function_guid, void** ppFunc);
};

#endif