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
        리슨 소켓을 열고 접속을 처리하는 워커 쓰레드
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
        // 쓰레드 종료 명령이 내려진 경우
        if (dummy == NULL) {
            break;
        }
        if (success == false)
        {
            int wsa_error = WSAGetLastError();

            if (wsa_error == ERROR_OPERATION_ABORTED)
            {
                // NOTE: old comments, 리슨 소켓을 종료시 그 소켓에 AcceptEx 걸어놓은 세션들에 대해
                // ERROR_OPERATION_ABORTED 발생
                delete overlapped->session;
                continue;
            }
            else if (wsa_error != ERROR_NETNAME_DELETED)
            {
                // AcceptEx에서 ERROR_NETNAME_DELETED는 에러가 아님(그 이외의 에러는 continue)
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

                // SockAddr 받아오기
                MsWinsockUtil::lpfnGetAcceptExSockAddrs_static_(received_buffer_ptr,
                    0,
                    sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
                    &lpLocalSockaddr, &nLocalSockaddrLen,
                    &lpRemoteSockaddr, &nRemoteSockaddrLen);

                overlapped->session->SetSockAddr((*(SOCKADDR_IN*)lpRemoteSockaddr));

                // 초기화에 성공한 세션을 PreAcceptedList에 추가한다.
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
