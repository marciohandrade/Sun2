#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#define MAX_CONNECT_THREAD  16

class IoHandler;
class Session;
class SessionList;

//==================================================================================================
/// �ٸ� �������� �ɵ��� ������ ����ϴ� Ŭ����
/**
    @remarks
        Connect(char *, WORD)�� ȣ���ϸ� �׿� ���� �۾� �����尡 �����ȴ�.
    @par
        �۾� �����忡�� ������ �ϳ� �����Ͽ� ������ �õ��Ѵ�.
    @par
        �� ������ ���ῡ �����ϸ� ������ PreConnectedList�� �߰��ǰ� �����ϸ� ConnectFailList��
        �߰��ȴ�.
    @par
        ���� ������Ʈ�� PreConnectedList�� �ִ� ���ǵ鿡 ���ؼ��� OnConnect(true)�� ȣ��ǰ�,
        ConnectFailList�� �ִ� ���ǿ� ���ؼ��� OnConnect(false)�� ȣ��ȴ�.
    @see
        Session
*/
//==================================================================================================
//
class Connector
{
public:
    Connector();
    ~Connector();

    void Init(IoHandler* io_handler, DWORD number_of_threads);
    void Connect(Session* session);
    void Shutdown();

private:
    IoHandler* io_handler_;
    SessionList* connecting_list_;
    HANDLE event_handles_[2];
    bool shutdown_;
    DWORD number_of_threads_;
    HANDLE thread_handles_[MAX_CONNECT_THREAD];
    friend unsigned __stdcall connect_thread(LPVOID param);
};

#endif