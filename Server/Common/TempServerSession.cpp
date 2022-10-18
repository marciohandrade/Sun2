#include "stdafx.h"
#include "./TempServerSession.h"

#include <ServerGlobal.h>
#include <PacketStruct_ServerCommon.h>

#include "./ServerFrame.h"

//==================================================================================================

__IMPL_CUSTOMPOOL_PTR(TempServerSession);

//==================================================================================================

TempServerSession::TempServerSession(void)
{
}

TempServerSession::~TempServerSession(void)
{
}

void TempServerSession::Init()
{
	ServerSession::Init();

	m_bFirst = TRUE;
}


void TempServerSession::OnRecv(BYTE* msg, WORD msg_size)
{
    ASSERT(m_bFirst != false);
    m_bFirst = false;
    // NOTE: f110617.1L, After using dual buffer for reception, ... hummmm. ...
    MSG_SERVER_TYPE* recv_msg = (MSG_SERVER_TYPE *)msg;
    const bool valid_message = recv_msg->m_byCategory == SERVERCOMMON &&
                               recv_msg->m_byProtocol == SERVERCOMMON_SERVERTYPE &&
                               sizeof(*recv_msg) == msg_size;
    if (valid_message == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=received unexpected 1st message, ")
               _T("C:P=%03d:%03d, size=%d|"),
               recv_msg->m_byCategory, recv_msg->m_byProtocol, msg_size);
        return;
    };
    //----------------------------------------------------------------------------------------------
    ServerFrame* const server_frame = GetFrame();
    // <Waverix> [9/8/2006]
    // 새롭게 바꾸는 GameServer의 경우 ServerType에 대한 검사에 취약한 점이 존재한다.
    // 만에 하나를 위해 여기서 검사를 수행하는 것으로 하자. 그런데... 로그는 없어도 되나???
    ServerSession* new_session = NULL;
    const eSERVER_TYPE server_type = static_cast<eSERVER_TYPE>(recv_msg->m_byServerType);
    if (DWORD(server_type) < DWORD(MAX_SERVER)) {
        new_session = server_frame->AllocServerSession(server_type);
    };
    if (new_session == NULL)
    {
        Disconnect();
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=unknown server (%d)"), server_type);
        return;
    }
    //
    Redirect(new_session);
    //
    // TempServerSession의 인덱스를 새로운 세션에 전달해줌!
    DWORD session_index = GetSessionIndex();
    server_frame->RemoveSession(session_index);
    server_frame->FreeServerSession(this);
    //
    new_session->OnRedirect(session_index, recv_msg);
}