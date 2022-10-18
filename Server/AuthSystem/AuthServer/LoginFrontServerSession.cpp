#include "stdafx.h"
#include "LoginFrontServerSession.h"
#include "AuthFrame.h"
#include "ServerDataManager.h"
#include <ArchiveOfStream.h>
#include <AuthProtocol_FS.h>

LoginFrontServerSession::LoginFrontServerSession(void)
{

}

LoginFrontServerSession::~LoginFrontServerSession(void)
{

}

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
void LoginFrontServerSession::OnRedirect(
    DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(session_index, first_contact_msg);
    assert(session_index == GetSessionIndex());
    AuthFrame::Instance()->SendGameVersion_ToLoginFront(session_index);
    AuthFrame::Instance()->SendControlIp_ToLoginFront(session_index);

    SendServerList();

    m_ServerListTimer.SetTimer(SERVER_LIST_DELAY_TIME);
}

void
LoginFrontServerSession::OnDisconnect()
{
    m_ServerListTimer.DisableCheckTime();

    ServerSession::OnDisconnect();
}

void
LoginFrontServerSession::Update()
{
    if(m_ServerListTimer.IsExpired())
    {
        SendServerList();
    }
}

void
LoginFrontServerSession::SendServerList()
{
    // 변화되는 정보만 주기적으로 보내준다.
    MSG_SF_AUTH_SERVER_LIST_NTF sendServerList;
    sendServerList.m_Count = ServerDataManager::Instance()->GetDynamicServerListInfo(sendServerList.m_ServerDataList);
    AuthFrame::Instance()->SendToSessionType(LOGIN_FRONT_SERVER, &sendServerList, sendServerList.GetSize());
}



















