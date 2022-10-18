#ifndef  __eXtreme_AsyncResult_H
#define  __eXtreme_AsyncResult_H

namespace eXtreme {

enum IOEventType
{
    Event_None    = 0x000000,
    Event_Accept  = 0x000001,
    Event_Connect = 0x000002,
    Event_Close   = 0x000004,
    Event_Send    = 0x000008,
    Event_Receive = 0x000010
};

class EventHandler;

///  비동기 I/O 작업의 결과를 받아올 정보 구조체
typedef struct tag_AsyncResult : OVERLAPPED
{
    IOEventType     eventType;
    unsigned long   transBytes;
    unsigned long   error;
    EventHandler*   handler;
    // WSARecv 관련 recvbuffer를 막바로 넘겨줄경우 환형 처리가 어설퍼 버퍼 사이즈 끝으로 갈수록 받을수 있는 양이 적어져 recv buffer 따로 분리
    char            szData[10240];
} AsyncResult;

}; //end of namespace 'eXtreme'

#endif