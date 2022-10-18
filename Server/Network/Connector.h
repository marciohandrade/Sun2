#ifndef __CONNECTOR_H__
#define __CONNECTOR_H__

#define MAX_CONNECT_THREAD  16

class IoHandler;
class Session;
class SessionList;

//==================================================================================================
/// 다른 서버로의 능동적 연결을 담당하는 클래스
/**
    @remarks
        Connect(char *, WORD)를 호출하면 그에 대한 작업 쓰레드가 생성된다.
    @par
        작업 쓰레드에서 세션을 하나 생성하여 연결을 시도한다.
    @par
        그 세션이 연결에 성공하면 서버의 PreConnectedList에 추가되고 실패하면 ConnectFailList에
        추가된다.
    @par
        서버 업데이트시 PreConnectedList에 있는 세션들에 대해서는 OnConnect(true)가 호출되고,
        ConnectFailList에 있는 세션에 대해서는 OnConnect(false)가 호출된다.
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