#include "stdafx.h"

#include <AgentServer.h>
#include <ServerSessions/WorldServerSession.h>
#include <PacketHandler/PacketHandler.h>

//==================================================================================================

namespace ns_functor {
;

#ifdef _NA002676_WORLD_SERVER_RENEWAL
struct WorldServiceRestorer : public ns_functor::IUserForeachFunctor
{
    WorldServiceRestorer(WorldServerSession* server_session);
    virtual ~WorldServiceRestorer();
    //
    virtual bool operator()(User* const user);
    //
    WorldServerSession* const server_session_;
    //
    __DISABLE_COPY(WorldServiceRestorer);
};

struct WorldServiceDisabledStateNotifier : public ns_functor::IUserForeachFunctor
{
    WorldServiceDisabledStateNotifier();
    virtual ~WorldServiceDisabledStateNotifier();
    //
    virtual bool operator()(User* const user);
    void NotifyToClients();
    //
    __DISABLE_COPY(WorldServiceDisabledStateNotifier);
};
#endif

}; //end of namespace

#ifdef _NA002676_WORLD_SERVER_RENEWAL

ns_functor::WorldServiceRestorer::WorldServiceRestorer(WorldServerSession* server_session)
    : server_session_(server_session)
{
};

ns_functor::WorldServiceRestorer::~WorldServiceRestorer()
{
};

bool ns_functor::WorldServiceRestorer::operator()(User* const user)
{
    // 접속 상태이므로 복구 패킷 전송
    MSG_AW_USER_RESTORE_CMD msg_cmd;
    msg_cmd.Clear();

    msg_cmd.m_dwKey = user->GetUserKey();
    //
    ;{
        msg_cmd.dwAuthUserID = user->GetAuthID();
        msg_cmd.dwUserGUID = msg_cmd.m_dwKey;
        CHECK_ID_LENGTH_BUFFER(msg_cmd.ptszAccountID);
        _tcsncpy(msg_cmd.ptszAccountID, user->GetUserID(), _countof(msg_cmd.ptszAccountID));
        msg_cmd.ptszAccountID[_countof(msg_cmd.ptszAccountID) - 1] = '\0';
        memcpy(msg_cmd.pszClientIP, user->GetClientIP(), sizeof(msg_cmd.pszClientIP));
    };

    const bool existance_character = user->GetSelectedCharIndex() != MAX_CHARACTER_LIST_NUM;
    if (existance_character)
    {
        // old condition = '(msg_cmd.byZoneType != eZONETYPE_CHARSELECT)'
        msg_cmd.CharGuid = user->GetSelectedCharGuid();
        _tcsncpy(msg_cmd.tszCharName, user->GetSelectedCharName(), _countof(msg_cmd.tszCharName));
        msg_cmd.tszCharName[_countof(msg_cmd.tszCharName) - 1] = _T('\0');

        msg_cmd.dwZoneKey = user->GetZoneKey();
        msg_cmd.byZoneType = user->GetZoneType();
        msg_cmd.map_code = user->GetSelectedCharMapCode();

        msg_cmd.wPartyKey = user->GetPartyState().GetPartyKey();
    };
    ;{
        msg_cmd.dwPCRoomID = user->GetReservedValue();    // PC방인지 아닌지 세팅
        msg_cmd.byGMGrade = user->GetSts();
    };
    if (existance_character)
    {
        msg_cmd.m_GuildGuid = user->GetSelectedCharGuildGuid();
        msg_cmd.m_CharLevel = user->GetCharLV();
        msg_cmd.m_CharClass = user->GetCharClass();
    };
    //
    server_session_->SendPacket(&msg_cmd, msg_cmd.GetSize());
    return true;
}

ns_functor::WorldServiceDisabledStateNotifier::WorldServiceDisabledStateNotifier()
{
};

ns_functor::WorldServiceDisabledStateNotifier::~WorldServiceDisabledStateNotifier()
{
};

bool ns_functor::WorldServiceDisabledStateNotifier::operator()(User* const user)
{
    user->ChangeWorldServiceEnabledState(false);
    return true;
}

void ns_functor::WorldServiceDisabledStateNotifier::NotifyToClients()
{
    MSG_CG_PREPARE_WORLD_CONNECT_ACK msg_ack;
    msg_ack.enabled = false;

    UserManager* const user_manager = UserManager::Instance();
    user_manager->SendMessageToAllUsers(&msg_ack, sizeof(msg_ack));
}
#endif

//==================================================================================================


WorldServerSession::WorldServerSession()
{
}

WorldServerSession::~WorldServerSession()
{
}

// CHANGES: f110617.1L
void WorldServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

void WorldServerSession::OnConnect(BOOL success, DWORD session_index)
{
    ServerSessionEx::OnConnect(success, session_index);
    if (success == false) {
        return;
    };

    // AgentServer 정보를 WorldServer에 알린다.
    AgentServer* const agent_server = AgentServer::GetInstance();
    ;{
        MSG_AW_AGENTSERVER_INFO_SYN msg_syn;
        msg_syn.dwChannelID = agent_server->GetKey().GetChannelID();
        Send((BYTE*)&msg_syn, sizeof(msg_syn));
    };
    ;{
        // 월드서버가 연결되었음을 게임서버에 알려준다.
        MSG_AG_WORLDSERVER_INFO_CMD msg_cmd;
        msg_cmd.m_bIsConnectedWorld = TRUE;
        agent_server->SendToGameServer(&msg_cmd, sizeof(msg_cmd));
    };
#ifdef _NA002676_WORLD_SERVER_RENEWAL
    if (UserManager* user_manager = UserManager::Instance())
    {
        ns_functor::WorldServiceRestorer restorer(this);
        user_manager->Foreach(&restorer);
    };
#endif
}

void WorldServerSession::OnDisconnect()
{
    ServerSessionEx::OnDisconnect();

    // 월드서버가 끊어졌음을 게임서버에 알려준다.
    MSG_AG_WORLDSERVER_INFO_CMD msgCMD;
    msgCMD.m_bIsConnectedWorld = FALSE;
    AgentServer::GetInstance()->SendToGameServer(&msgCMD, sizeof(msgCMD));

#ifdef _NA002676_WORLD_SERVER_RENEWAL
    ns_functor::WorldServiceDisabledStateNotifier notifier;
    UserManager::Instance()->Foreach(&notifier);
    notifier.NotifyToClients();
#endif
}

