#include "stdafx.h"
#include "Connector.h"

#include "Session.h"
#include "SessionList.h"
#include "IoHandler.h"

//==================================================================================================
/**
    @remarks
        connect를 시도하는 쓰레드
    @par
        connect 성공시 세션을 ConnSuccessList에 추가하고 실패시 ConnFailureList에 추가한다.
*/
//==================================================================================================
unsigned __stdcall connect_thread(LPVOID param)
{
    Connector* const connector = reinterpret_cast<Connector*>(param);
    //
    while (connector->shutdown_ == false)
    {
        DWORD wait_result = WaitForMultipleObjects(2, connector->event_handles_, false, INFINITE);

        if (wait_result - WAIT_OBJECT_0 == 0)
        {
            // 커넥트 이벤트
            SessionList* const connecting_list = connector->connecting_list_;
            while (!connecting_list->empty())
            {
                // 접속 대기 큐에서 세션을 하나 꺼낸다.
                connecting_list->Lock(); 
                Session* const session = connecting_list->front();
                connecting_list->pop_front();
                connecting_list->Unlock();

                // 접속 시도
                int err = ::connect(session->GetSocket(),
                                    (SOCKADDR*)(session->GetSockAddr()), sizeof(SOCKADDR_IN));
                if (err == SOCKET_ERROR)
                {
                    // 접속 실패
                    SessionList* connect_fail_list = connector->io_handler_->connect_fail_list_;
                    connect_fail_list->Lock();
                    connect_fail_list->push_back(session);
                    connect_fail_list->Unlock();
                }
                else
                {
                    // 접속 성공
                    SessionList* connect_success_list = \
                        connector->io_handler_->connect_success_list_;
                    connect_success_list->Lock();
                    connect_success_list->push_back(session);
                    connect_success_list->Unlock();
                }
            }
        }
        else if (wait_result - WAIT_OBJECT_0 == 1)
        {
            // 쓰레드 종료 이벤트
            break;
        }
    }

    return 0;
}
