#include "stdafx.h"

#include <AgentServer.h>
#include <ServerSessions/GameServerSession.h>
#include <ServerSessions/ServerSessionManager.h>
#include <PacketHandler/PacketHandler.h>

#include <UserSessions/UserManager.h>

GameServerSession::GameServerSession()
{
}

GameServerSession::~GameServerSession()
{
}

VOID	GameServerSession::Init()
{
	ServerSessionEx::Init();
}

VOID	GameServerSession::Release()
{
	// ���� ������ dangling�� ������ �ʿ䰡 �ִ�.
}

//VOID GameServerSession::OnRecv( BYTE *pMsg, WORD wSize )
//{
//}
//

VOID GameServerSession::OnDisconnect()
{
	RemoveRelatedUsers();
	ServerSessionManager::Instance()->RemoveServer( this );
	ServerSessionEx::OnDisconnect();
}

//==================================================================================================
namespace ns_functor {
;

class DisconnectRelatededUsers : public ns_functor::IUserForeachFunctor
{
public:
    DisconnectRelatededUsers(DWORD session_index)
        : server_session_index_(session_index) {}
    ~DisconnectRelatededUsers() {}

    virtual bool operator()(User* const user);
    //
private:
    const DWORD server_session_index_;
    //
    __DISABLE_COPY(DisconnectRelatededUsers);
};

}; //end of namespace
//==================================================================================================

bool ns_functor::DisconnectRelatededUsers::operator()(User* const user)
{
    const DWORD user_linked_index = user->GetServerSessionIndex();
    //< �ʵ峪 ��Ʋ ������ ����Ǿ� �ִ� ����
    if (user_linked_index && (user_linked_index == server_session_index_))
    {
        user->SetDisconnectCode(RC::RC_DISCONNECT_SERVER_ERROR);
        // pUser->UnlinkServer(); GameServer���� AgentSession�� �������� �� DBP�� Serialize ��Ŷ�� ������.
        user->DisconnectLinkedServerUser();

        SUNLOG(eFULL_LOG, _T("[DisconnectRelatededUsers][U:%d]:������������"), user->GetUserKey());
    }
    return true;
}


//==================================================================================================

VOID GameServerSession::RemoveRelatedUsers()
{
	SUNLOG( eCRITICAL_LOG, _T("[RemoveRelatedUsers] [%s�� ��������] : ���õ�������������"),
		GetServerType2String( GetServerType() )/*, UserManager::Instance()->GetNumberOfUsers()*/ );
	// �ּ� ���� ServerFrame�� DoRelease���� �ڱ�� Release��Ų��.
    ns_functor::DisconnectRelatededUsers disconnector(GetSessionIndex());
    UserManager::Instance()->Foreach(&disconnector);
}

