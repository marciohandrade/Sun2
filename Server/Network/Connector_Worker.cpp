#include "stdafx.h"
#include "Connector.h"

#include "Session.h"
#include "SessionList.h"
#include "IoHandler.h"

//==================================================================================================
/**
    @remarks
        connect�� �õ��ϴ� ������
    @par
        connect ������ ������ ConnSuccessList�� �߰��ϰ� ���н� ConnFailureList�� �߰��Ѵ�.
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
            // Ŀ��Ʈ �̺�Ʈ
            SessionList* const connecting_list = connector->connecting_list_;
            while (!connecting_list->empty())
            {
                // ���� ��� ť���� ������ �ϳ� ������.
                connecting_list->Lock(); 
                Session* const session = connecting_list->front();
                connecting_list->pop_front();
                connecting_list->Unlock();

                // ���� �õ�
                int err = ::connect(session->GetSocket(),
                                    (SOCKADDR*)(session->GetSockAddr()), sizeof(SOCKADDR_IN));
                if (err == SOCKET_ERROR)
                {
                    // ���� ����
                    SessionList* connect_fail_list = connector->io_handler_->connect_fail_list_;
                    connect_fail_list->Lock();
                    connect_fail_list->push_back(session);
                    connect_fail_list->Unlock();
                }
                else
                {
                    // ���� ����
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
            // ������ ���� �̺�Ʈ
            break;
        }
    }

    return 0;
}
