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
	// 관련 유저들 dangling들 제거할 필요가 있다.
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
    //< 필드나 배틀 서버에 연결되어 있는 유저
    if (user_linked_index && (user_linked_index == server_session_index_))
    {
        user->SetDisconnectCode(RC::RC_DISCONNECT_SERVER_ERROR);
        // pUser->UnlinkServer(); GameServer에서 AgentSession이 끊어졌을 때 DBP로 Serialize 패킷을 날린다.
        user->DisconnectLinkedServerUser();

        SUNLOG(eFULL_LOG, _T("[DisconnectRelatededUsers][U:%d]:강제유저삭제"), user->GetUserKey());
    }
    return true;
}


//==================================================================================================

VOID GameServerSession::RemoveRelatedUsers()
{
	SUNLOG( eCRITICAL_LOG, _T("[RemoveRelatedUsers] [%s의 접속해제] : 관련된유저강제삭제"),
		GetServerType2String( GetServerType() )/*, UserManager::Instance()->GetNumberOfUsers()*/ );
	// 주석 이유 ServerFrame은 DoRelease이후 자기것 Release시킨다.
    ns_functor::DisconnectRelatededUsers disconnector(GetSessionIndex());
    UserManager::Instance()->Foreach(&disconnector);
}

