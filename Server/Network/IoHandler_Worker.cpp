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
        I/O completion을 감시하고 처리하는 쓰레드
    @param  param
        IOCP 서버의 포인터
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

        // 스레드의 강제 종료 명령이 내려진 경우
        if (session == NULL) {
            break;
        }
        // GQCS가 실패인 경우 혹은 세션 제거
        if (!success)
        {
            if (GetLastError() == ERROR_NETNAME_DELETED)
            {
                // 이미 소켓이 닫힌 경우 제거
            #ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
                printf("[REMOVE][%d] GetLastError() == ERROR_NETNAME_DELETED\n", (int)session->GetSocket());
            #endif
                //session->GetSendBuffer()->ReleaseSendingLock();
                session->Remove();
            }
            continue;
        }

        // DISCONNECT_POSTED가 아닌데도 IoSize가 0인 경우 세션 제거
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

        // Completion 처리
        switch (overlapped->operation_type)
        {
        case SEND_POSTED:
            session->GetSendBuffer()->OnCompleted(io_size);
            break;

        case RECV_POSTED:           
            session->GetRecvBuffer()->OnCompleted(io_size);

            // 다음 Recv  
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
