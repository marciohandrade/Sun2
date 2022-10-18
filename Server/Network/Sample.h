#include "IOCPServer.h"
#include "NetworkObject.h"

//-------------------------------------------------------------------------------------------------
// Sample Echo Server
//-------------------------------------------------------------------------------------------------

#ifdef _DEBUG
  #pragma comment(lib, "Network_d.lib")
#else
  #pragma comment(lib, "Network.lib")
#endif

//-------------------------------------------------------------------------------------------------
// User Class
//-------------------------------------------------------------------------------------------------
class User : public NetworkObject
{
protected:
    virtual void OnAccept(DWORD network_index) {};
    virtual void OnDisconnect() {};
    virtual void OnRecv(BYTE* pMsg, WORD size) { Send(pMsg, size); }
    virtual void OnConnect(BOOL success, DWORD network_index) {} 
};

//-------------------------------------------------------------------------------------------------
// Callback Functions
//-------------------------------------------------------------------------------------------------
NetworkObject* CreateAcceptedObject() { return new User; }
void DestroyAcceptedObject(NetworkObject* network_object) { delete network_object; }
void DestroyConnectedObject(NetworkObject* network_object) {delete network_object; }


//-------------------------------------------------------------------------------------------------
// Main
//-------------------------------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
    const DWORD CLIENT_IOHANDLER_KEY = 0;

    // IOCP ���� �ν��Ͻ� ����
    IOCPServer* iocp_server = new IOCPServer;

    // ��ũ���� ����
    IOHANDLER_DESC desc;

    desc.io_handler_key = CLIENT_IOHANDLER_KEY;
    desc.dwMaxAcceptSession = 1000;
    desc.dwMaxConnectSession = 0;
    desc.send_buffer_size = 60000;
    desc.recv_buffer_size = 60000;
    desc.timeout = 30000;
    desc.max_packet_size = 4096;
    desc.dwNumberOfIoThreads = 1;
    desc.dwNumberOfAcceptThreads = 1;
    desc.dwNumberOfConnectThreads = 0;
    desc.fnCreateAcceptedObject = CreateAcceptedObject;
    desc.fnDestroyAcceptedObject = DestroyAcceptedObject;
    desc.fnDestroyConnectedObject = DestroyConnectedObject;

    // IOCP ���� �ʱ�ȭ
    if (!iocp_server->Init(&desc, 1))
    {
        printf("IOCP ���� �ʱ�ȭ ����");
        return 0;
    }

    // ���� ����
    if (!iocp_server->StartListen(CLIENT_IOHANDLER_KEY, "10.1.28.134", 6000))
    {
        printf("���� ����");
        return 0;
    }

    // assert(iocp_server->IsListening(CLIENT_IOHANDLER_KEY) == true);

    printf("Server started!\n");

    // ���� �������� IOCP ���� ������Ʈ
    while (true)
    {
        Sleep(10);
        iocp_server->Update();
    }

    printf("Server is terminated...\n");

    // IOCP ���� �˴ٿ�
    iocp_server->Shutdown();

    // IOCP ���� �ν��Ͻ� �ı�
    delete iocp_server;

    return 0;
}
