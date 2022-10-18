#include "WorldServerPch.hxx"
#include "./WorldUser.h"
#include "./WorldUserManager.h"

#include "Channel/WorldChannelManager.h"
#include "Channel/WorldChannel.h"
#include "Channel/ZoneCommunity.h"
#include "Channel/PartyCommunity.h"
#include "Channel/ChatRoomCommunity.h"

#include "Services/Friends/FriendMap.h"
#include "Services/Group/GroupManager.h"
#include "Services/Guild/WorldGuildManager.h"

//==================================================================================================

ns_world::WorldUser::WorldUser()
{
    ZeroMemory(&pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
    // initialize 'ICommunity' interface data objects
    ;{
        util::LList::Init(&channel_node_);
        util::LList::Init(&zone_node_);
        util::LList::Init(&zone_node_.sector_node);
        util::LList::Init(&party_node_);
        util::LList::Init(&chat_room_node_);

        channel_node_.base = this;
        zone_node_.base = this;
        zone_node_.sector_node.user = this;
        party_node_.base = this;
        chat_room_node_.base = this;
    };

    group_link_.Init(this);
    friend_link_.Init(this);
    block_char_link_.Init(this);
    revenge_link_.Init(this);
    memo_link_.Init(this);
};

ns_world::WorldUser::~WorldUser()
{
};

void ns_world::WorldUser::Init()
{
    ns_world::WorldUserManager::Logout(this, ns_functor::LogoutOption::Reuse(false));
}

void ns_world::WorldUser::Update()
{
}

bool ns_world::WorldUser::EnabledChatting()
{
    if ((channel_node_.link_channel == NULL) ||
        (channel_node_.link_channel->EnabledChat() == false))
    {
        return false;
    };
    if ((zone_node_.link_zone == NULL) ||
        (zone_node_.link_zone->EnabledChat() == false))
    {
        return false;
    };
    if (chatting_mode_.chatting_blocked) {
        return false;
    };

    return true;
}

void ns_world::WorldUser::NotifyChattingDisabledState(RC::eCHAT_RESULT result) const
{
    MSG_CW_CHAT_NORMAL_NAK msg_nak;
    msg_nak.m_wErrorCode = static_cast<uint16_t>(result);
    const_cast<ns_world::WorldUser*>(this)->SendPacket(&msg_nak, sizeof(msg_nak));
}

// message controllers
bool ns_world::WorldUser::EnabledSendMessage() const
{
    if (this->channel_node_.link_channel == NULL) {
        return false;
    };
    return true;
};

// send the actor marked message
bool ns_world::WorldUser::SendPacket(MSG_BASE* msg_base, uint16_t msg_size) const
{
    ns_world::community::WorldChannel* const channel = this->channel_node_.link_channel;
    if (channel == NULL) {
        return false;
    };
    msg_base->m_dwKey = user_account_info_.user_guid;
    channel->SendPacket(msg_base, msg_size);
    return true;
}

// message broadcaster
bool ns_world::WorldUser::SendPacketBelongToChannel(
    MSG_BASE* const msg_base, uint16_t msg_size) const
{
    if (connected_char_info_.zone_key == 0) {
        return false;
    };
    if (FlowControl::FCAssert(user_account_info_.channel != NULL) == false) {
        return false;
    };
    user_account_info_.channel->SendToAll(msg_base, msg_size);
    return true;
}

// message broadcaster
bool ns_world::WorldUser::SendPacketBelongToZone(
    MSG_BASE* const msg_base, uint16_t msg_size) const
{
    if (connected_char_info_.zone_key == 0) {
        return false;
    };
    if (FlowControl::FCAssert(user_account_info_.channel != NULL) == false) {
        return false;
    };
    ns_world::community::ZoneCommunity* zone = \
        user_account_info_.channel->FindZone(connected_char_info_.zone_key);
    if (zone == NULL) {
        return false;
    };
    zone->SendToAll(msg_base, msg_size);
    return true;
}

// message broadcaster
bool ns_world::WorldUser::SendPacketToParty(MSG_BASE* const msg_base, uint16_t msg_size) const
{
    if (party_node_.link_party == NULL) {
        return false;
    };

    return party_node_.link_party->SendToAll(msg_base, msg_size);
}

// message broadcaster
bool ns_world::WorldUser::SendPacketToGuild(MSG_BASE* const msg_base, uint16_t msg_size) const
{
    if (connected_char_info_.guild_guid == 0) {
        return false;
    };
    ns_world::WorldGuildManager* guild_manager = ns_world::WorldGuildManager::Instance();
    return guild_manager->SendGuildPacket(connected_char_info_.guild_guid, msg_base, msg_size);
}

// chatting message broadcaster
bool ns_world::WorldUser::SendNormalChat(MSG_BASE* const msg_base, uint16_t msg_size) const
{
    if (zone_node_.link_zone == NULL) {
        return false;
    };

    return zone_node_.link_zone->SendPacketAround(msg_base, msg_size, this);
}

// chatting message broadcaster
bool ns_world::WorldUser::SendPacketShouting(
    MSG_BASE* const msg_base, uint16_t msg_size,
    const ns_world::WorldShoutingArg* whole_shouting_arg)
{
    if (channel_node_.link_channel == NULL || zone_node_.link_zone == NULL) {
        return false;
    };

    // check shouting limitations

    if (whole_shouting_arg)
    {   // TODO: use arguments in whole_shouting_arg
        ns_world::WorldUserManager* user_manager = ns_world::WorldUserManager::Instance();
        user_manager->SendToAll(msg_base, msg_size);
        return true;
    };

    return channel_node_.link_channel->SendToAllInShoutRanges(msg_base, msg_size, this);
}

// DELETE: message broadcaster for user relations
bool ns_world::WorldUser::SendToAllOnlineFriends(MSG_BASE* msg_base, uint16_t msg_size)
{
#if WORLD_CONTENTS_BLOCK
    assert(!"need implement");
#endif
    return false;
}

// DELETE: message broadcaster for user relations
bool ns_world::WorldUser::SendToAllRevengers(MSG_BASE* msg_base, uint16_t msg_size)
{
#if WORLD_CONTENTS_BLOCK
    assert(!"need implement");
#endif
    return false;
}


//==================================================================================================
//==================================================================================================
//==================================================================================================

#if WORLD_CONTENTS_BLOCK

BOOL ns_world::WorldUser::CheckFriendList()
{
#ifndef _WHATTHEHELL_CHANGE_FRIENDCHECK_20060802
    if(m_bFriendList)
    {
        MessageOut(eCRITICAL_LOG, "[%s] Friend List Hack Check", m_strCharName.c_str());
        return false;
    }

    m_bFriendList = true;
    return true;
#else
    DWORD dwTempTick = WorldServer::Instance()->GetGlobalTick();;
    if(dwTempTick < m_dwLastFriendTick + 5000)    // 5초에 한번씩만 길드 확인
    {
        MessageOut(eFULL_LOG, "CheckFriendList Failed - Time Checked[CharGuid:%u]", GetCharGuid());
        return false;
    }

    m_dwLastFriendTick = dwTempTick;

    return true;
#endif
}

// 길드 리스트를 정해진 시간마다만 보내주도록 제한
BOOL ns_world::WorldUser::CheckGuildList()
{
    DWORD dwTempTick = WorldServer::Instance()->GetGlobalTick();

    if(dwTempTick < m_dwLastGuildTick + 5000) // 5초에 한번씩만 길드 확인
    {
        MessageOut(eFULL_LOG, "CheckGuildList Failed - Time Checked[CharGuid:%u]", GetCharGuid());
        return false;
    }

    m_dwLastGuildTick = dwTempTick;

    return true;
}

// 채팅 어뷰즈 체크용
BOOL ns_world::WorldUser::CheckChatTime()
{
    DWORD dwChatTime = GetTickCount();

    // 10분의 1초 이내 시간으로 채팅이 계속 날아오면 체크했다가 10회 이상이면 잘라버린다.
    if(dwChatTime <= m_dwLastChatTime + 100)
    {
        m_byChatCount++;
        m_dwLastChatTime = dwChatTime;

        if(m_byChatCount > 10)
        {
            MessageOut(eCRITICAL_LOG, "ns_world::WorldUser[%s][CharGuid:%u] Chat Abuse!!", m_strCharName.c_str(), m_CharGuid);
            Logout();
        }
        return false;
    }

    // 이상 없으면 초기화
    m_byChatCount = 0;
    m_dwLastChatTime = dwChatTime;

    return true;
}

BOOL ns_world::WorldUser::UpdateBlockTime()
{
    if(!IsChatBlocked())
    {
        MessageOut(eFULL_LOG, "Not Blocked ns_world::WorldUser[%u]", m_CharGuid);
        return false;
    }

    DWORD dwThisTime = GetTickCount();

    if(dwThisTime <= m_dwBlockLastTime)
    {
        MessageOut(eFULL_LOG, "?!!!! Invalid Block Time![%u]", m_CharGuid);
        //FreeChatBlock();
        return false;
    }

    // 시간 다 되었으면 채팅금지 해제
    if(m_dwBlockTime <= dwThisTime - m_dwBlockLastTime)
    {
//      UserManager::Instance()->RemoveChatBlockUser(this);
        MSG_DW_GM_CHATBLOCK_UPDATE_SYN  sendMsg;

        sendMsg.m_dwKey = m_CharGuid;
        sendMsg.m_dwRemainTime = 0;

        WorldServer::Instance()->SendGameDBProxy((BYTE*)&sendMsg, sizeof(sendMsg));

        MessageOut(eFULL_LOG, "End Block Time[%u]", m_CharGuid);
        //FreeChatBlock();
        return false;
    }

    DWORD dwTempTime = m_dwBlockTime - (dwThisTime - m_dwBlockLastTime);

    m_dwBlockLastTime = dwThisTime;

    MSG_DW_GM_CHATBLOCK_UPDATE_SYN  sendMsg;

    sendMsg.m_dwKey = m_CharGuid;
    sendMsg.m_dwRemainTime = dwTempTime / 60000;    // 저장할 때는 다시 분 단위 - 중간에 잘리는 부분은 그냥 버린다

    WorldServer::Instance()->SendGameDBProxy((BYTE*)&sendMsg, sizeof(sendMsg));

    m_dwBlockTime = dwTempTime;

    MessageOut(eFULL_LOG, "Block Time Updated[%u][%u]", m_CharGuid, m_dwBlockTime);
    return true;
}

BOOL ns_world::WorldUser::CheckMemoList()
{
    DWORD dwTempTick = WorldServer::Instance()->GetGlobalTick();

    if(dwTempTick < m_dwLastMemoTick + 5000)  // 5초에 한번씩만 메모 확인
    {
        MessageOut(eFULL_LOG, "CheckMemoList Failed - Time Checked[CharGuid:%u]", GetCharGuid());
        return false;
    }

    m_dwLastMemoTick = dwTempTick;

    return true;
}

//현재 이 함수는 월드 외치기 용으로 사용된다.
void ns_world::WorldUser::SendWorldChatToAll(BYTE *pMsg, WORD wSize, BOOL bNow)
{
    //1. 채팅블록된 유저인지 확인한다.
    if(IsChatBlocked())
    {
        SendChatBlock();
        return;
    }

    //2. 외치기 간격을 확인한다.
    if(!WorldServer::Instance()->CheckWorldShoutInterval(m_dwLastShoutTime))
    {
        MSG_CW_CHAT_SHOUT_NAK   sendMsg;

        sendMsg.m_wErrorCode = RC::RC_CHAT_ALREADYSHOUT;
        SendPacket((BYTE*)&sendMsg, sizeof(sendMsg));
        MessageOut(eFULL_LOG, "Shout Interval Checked[%u][CharGuid:%u][ZT:%u][ZK:%u]", GetGUID(), 
            GetCharGuid(), 
            GetStatus(), 
            GetZoneKey());
        return;
    }   

    // 기간제이거나 pc방 일경우 즉시 메시지를 클라에 보냄. 아닐경우 아이템 삭제확인 
    if(bNow)
        UserManager::Instance()->SendToAll(pMsg, wSize);
    else
        SendUseDecreeItem();    
}

void ns_world::WorldUser::SendUseDecreeItem()
{
    ServerSession* pAgentServer = ServerSessionManager::Instance()->FindAgentServer(GetChannelID());
    if(!pAgentServer)
    {
        MessageOut(eCRITICAL_LOG, "[ns_world::WorldUser::SendUseDecreeItem] Decree Item Use Send Error - No AgentServer!!![%u],[%s]", GetChannelID(), GetCharName().c_str());
    }   
    else
    {
        MSG_AW_ITEM_USE_DECREE_ITEM_SYN sendMsg;
        sendMsg.m_dwKey = m_dwGUID;
        sendMsg.m_dwCharGuid = m_CharGuid;
        pAgentServer->Send((BYTE*)&sendMsg, sizeof(sendMsg));
    }
}

void ns_world::WorldUser::SetLastChatMsg(const char* szMsg)
{
    strncpy(m_szLastChatMsg, szMsg, MAX_CHATMSG_SIZE);
    m_szLastChatMsg[MAX_CHATMSG_SIZE] = '\0';
}

#endif //WORLD_CONTENTS_BLOCK