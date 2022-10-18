#include "stdafx.h"
#include "IoHandler.h"
#include "Acceptor.h"
//
#include "Session.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"


//==================================================================================================
/**
    @remarks
        I/O completion�� �����ϰ� ó���ϴ� ������
    @param  param
        IOCP ������ ������
*/
//==================================================================================================
unsigned __stdcall  io_thread(LPVOID param)
{
    IoHandler* const io_handler = reinterpret_cast<IoHandler*>(param);

    DWORD io_size = 0;
    Session* session = NULL;
    OVERLAPPEDEX* overlapped = NULL;

    while (1)
    {
        BOOL success = GetQueuedCompletionStatus(io_handler->iocp_handle_,
            &io_size, (PULONG_PTR)&session, (LPOVERLAPPED*)&overlapped, INFINITE);

        // �������� ���� ���� ����� ������ ���
        if (session == NULL) {
            break;
        }
        // GQCS�� ������ ��� Ȥ�� ���� ����
        if (!success)
        {
            if (GetLastError() == ERROR_NETNAME_DELETED)
            {
                // �̹� ������ ���� ��� ����
            #ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
                printf("[REMOVE][%d] GetLastError() == ERROR_NETNAME_DELETED\n", (int)session->GetSocket());
            #endif
                //session->GetSendBuffer()->ReleaseSendingLock();
                session->Remove();
            }
            continue;
        }

        // DISCONNECT_POSTED�� �ƴѵ��� IoSize�� 0�� ��� ���� ����
        if (overlapped->operation_type != DISCONNECT_POSTED && io_size == 0)
        {
        #ifdef _DEBUG
            session->OnLogString("[REMOVE][%d] io_size == 0\n", (int)session->GetSocket());
        #endif

        #ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
            printf("[REMOVE][%d] io_size == 0\n", (int)session->GetSocket());
        #endif
            //session->GetSendBuffer()->ReleaseSendingLock();
            session->Remove();
            continue;
        }

        // Completion ó��
        switch (overlapped->operation_type)
        {
        case SEND_POSTED:
            session->GetSendBuffer()->OnCompleted(io_size);
            break;

        case RECV_POSTED:           
            session->GetRecvBuffer()->OnCompleted(io_size);

            // ���� Recv  
            if (!session->PreRecv()) 
            {
            //#ifdef _DEBUG
            //    session->OnLogString("[REMOVE] PreRecv \n", (int)session->GetSocket());
            //#endif

            //#ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
            //    printf("[REMOVE] PreRecv \n", (int)session->GetSocket());
            //#endif
                // CHANGES: f110616.5L, f110616.1L, the failure of pre-receive operation cause
                // unstable socket status.
                //session->StopReception();
                session->Remove();
            }
            break;

        case DISCONNECT_POSTED:
            session->PreAccept(io_handler->acceptor_->GetListenSocket());
            break;
        }
    }

    return 0;
}
