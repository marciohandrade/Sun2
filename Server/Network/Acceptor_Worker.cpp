#include "stdafx.h"
#include "Acceptor.h"

#include "MsWinsockUtil.h"
#include "IoHandler.h"
#include "Session.h"
#include "SessionList.h"
#include "RecvBuffer.h"

//==================================================================================================
/**
    @remarks
        ���� ������ ���� ������ ó���ϴ� ��Ŀ ������
*/
//==================================================================================================
unsigned __stdcall accept_thread(LPVOID param)
{
    Acceptor* const acceptor = reinterpret_cast<Acceptor*>(param);
    IoHandler* const io_handler = acceptor->io_handler_;

    DWORD io_size = 0;
    OVERLAPPEDEX* overlapped = NULL;
    ULONG_PTR dummy = NULL;
    BYTE* received_buffer_ptr = NULL;
    int nLen;

    SOCKADDR* lpLocalSockaddr = NULL;
    SOCKADDR* lpRemoteSockaddr = NULL;
    int nLocalSockaddrLen = 0;
    int nRemoteSockaddrLen = 0;

    while (true)
    {
        BOOL success = GetQueuedCompletionStatus(acceptor->iocp_handle_,
            &io_size, &dummy, (LPOVERLAPPED*)&overlapped, INFINITE);
        // ������ ���� ����� ������ ���
        if (dummy == NULL) {
            break;
        }
        if (success == false)
        {
            int wsa_error = WSAGetLastError();

            if (wsa_error == ERROR_OPERATION_ABORTED)
            {
                // NOTE: old comments, ���� ������ ����� �� ���Ͽ� AcceptEx �ɾ���� ���ǵ鿡 ����
                // ERROR_OPERATION_ABORTED �߻�
                delete overlapped->session;
                continue;
            }
            else if (wsa_error != ERROR_NETNAME_DELETED)
            {
                // AcceptEx���� ERROR_NETNAME_DELETED�� ������ �ƴ�(�� �̿��� ������ continue)
                continue;
            }
        };
        //------------------------------------------------------------------------------------------
        switch (overlapped->operation_type) 
        {
        case ACCEPT_POSTED:
            {
                RecvBuffer* recv_buffer = overlapped->session->GetRecvBuffer();
                recv_buffer->GetRecvParam(&received_buffer_ptr, nLen);

                // SockAddr �޾ƿ���
                MsWinsockUtil::lpfnGetAcceptExSockAddrs_static_(received_buffer_ptr,
                    0,
                    sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
                    &lpLocalSockaddr, &nLocalSockaddrLen,
                    &lpRemoteSockaddr, &nRemoteSockaddrLen);

                overlapped->session->SetSockAddr((*(SOCKADDR_IN*)lpRemoteSockaddr));

                // �ʱ�ȭ�� ������ ������ PreAcceptedList�� �߰��Ѵ�.
                SessionList* session_list = io_handler->accepted_session_list_;
                session_list->Lock();
                session_list->push_back(overlapped->session);
                session_list->Unlock();
            };
            break;
        }; //end 'switch'
    }

    return 0;
}
