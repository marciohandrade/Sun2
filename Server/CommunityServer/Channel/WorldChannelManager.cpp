#include "WorldServerPch.hxx"
#include "./WorldChannelManager.h"

#include <MapInfoParser.h>

#include "./WorldChannel.h"
#include "./FieldInfo.h"
//
#include "WorldServer.h"
#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
#include "Channel/ChatRoomCommunity.h"
//#include "Services/pms/PMSManagerInWorld.h"

//==================================================================================================

ns_world::WorldChannelManager* ns_world::WorldChannelManager::instance_static_ = NULL;

ns_world::WorldChannelManager* ns_world::WorldChannelManager::CreateInstance()
{
    // if the old util::Singleton support interface don't exist, the below code need implements
    static struct AutoClosure {
        AutoClosure(ns_world::WorldChannelManager** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            if (*static_table_) {
                TAllocDelete(ns_world::WorldChannelManager, *static_table_);
            };
            *static_table_ = NULL;
        }
        ns_world::WorldChannelManager** static_table_;
    } auto_closure(&instance_static_);
    //
    if (instance_static_ != NULL) {
        return instance_static_;
    }
    //
    ns_world::WorldChannelManager* provider = TAllocNew(ns_world::WorldChannelManager);
    // initialization...
    instance_static_ = provider;
    //
    return instance_static_;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

ns_world::WorldChannelManager::WorldChannelManager()
    : shutdown_process_(false)
{
}

ns_world::WorldChannelManager::~WorldChannelManager()
{
}

void ns_world::WorldChannelManager::Init()
{
    if (const ns_world::WorldServer::Options* options =
            &ns_world::WorldServer::Instance()->GetOptions())
    {
        statistics_timer_.SetTimer(options->statistics_interval);
    };
    community_key_generator_.Create(10000, 10300);
    //
#if !defined(__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__)
    if (this->LoadFieldInfo() == false) {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=Critical! can't load field info."));
    };
#endif
}

void ns_world::WorldChannelManager::Release()
{
    // nothing to do.
};

void ns_world::WorldChannelManager::Shutdown()
{
    shutdown_process_ = true;
    ;{
        CHANNEL_MAP::const_iterator it = channel_list_.begin();
        const CHANNEL_MAP::const_iterator end = channel_list_.end();

        for (; it != end; ++it)
        {
            const ChannelPair& channel_pair = *it;
            TAllocDelete(ns_world::community::WorldChannel, channel_pair.channel);
        };

        channel_list_.clear();
    };
    ;{
        FieldInfoTable::const_iterator it = field_info_table_.begin();
        const FieldInfoTable::const_iterator end = field_info_table_.end();

        for (; it != end; ++it)
        {
            ns_world::community::FieldInfo* field_info = it->second;
            TAllocDelete(ns_world::community::FieldInfo, field_info);
        };

        field_info_table_.clear();
    };
    ;{
        ChatRoomTable::const_iterator it = chat_room_table_.begin();
        const ChatRoomTable::const_iterator end = chat_room_table_.end();

        for (; it != end; ++it)
        {
            ns_world::community::ChatRoomCommunity* chat_room = it->second;
            TAllocDelete(ns_world::community::ChatRoomCommunity, chat_room);
        };
    };
};

BOOL ns_world::WorldChannelManager::LoadScript(eSCRIPT_CODE script_code, BOOL reloaded)
{
    switch (script_code)
    {
    case SCRIPT_SHOUTRANGE:
        return _LoadLinkedZoneFile(reloaded != false);
    }

    return false;
}

bool ns_world::WorldChannelManager::_LoadLinkedZoneFile(bool reloaded)
{
    __UNUSED(reloaded);
    shout_range_table_.clear();

    ns_world::community::ShoutRange shout_range;
    static const char* linked_map_codes_columns[_countof(shout_range.linked_map_codes)] =
    {
        "LField1", "LField2", "LField3", "LField4", "LField5", "LField6", "LField7"
    };

    const int max_rows = GetRowSize();
    for (int row = 0; row < max_rows; ++row)
    {
        ZeroMemory(&shout_range, sizeof(shout_range));
        shout_range.map_code = this->GetDataWORD("MapCode", row);
        shout_range.zone_type = this->GetDataBYTE("MKind", row);
        for (int index = 0; index < _countof(shout_range.linked_map_codes); ++index)
        {
            if (MAPCODE* dest = &shout_range.linked_map_codes[index]) {
                *dest = GetDataWORD(linked_map_codes_columns[index], row);
            };
        };
        strncpy(shout_range.map_name, this->GetDataString("MName", row),
                _countof(shout_range.map_name));
        shout_range.map_name[_countof(shout_range.map_name) - 1] = '\0';

        ns_world::community::ShoutRangeTable::_Pairib result = shout_range_table_.insert(\
            ns_world::community::ShoutRangeTable::value_type(shout_range.map_code, shout_range));
        ASSERT(result.second);
    }; //end 'foreach row in rows'

    return true;
}

bool ns_world::WorldChannelManager::AddChannel(
    DWORD channel_id,
    ns_world::ServerSessionInWorld* binding_session)
{
    ;{
        CHANNEL_MAP::const_iterator it = channel_list_.begin();
        const CHANNEL_MAP::const_iterator end = channel_list_.end();
        for (; it != end; ++it)
        {
            const ChannelPair& channel_pair = *it;
            if (channel_pair.session != binding_session) {
                continue;
            };
            ASSERT(!"unexpected state");
            return false;
        };
    };

    ns_world::community::WorldChannel* channel = \
        TAllocNew(ns_world::community::WorldChannel)(channel_id, binding_session);
    binding_session->BindChannel(channel);
    ChannelPair channel_pair = { channel, binding_session };
    channel_list_.push_back(channel_pair);
#ifdef _JP_0_20101123_PMS
    WorldServer::Instance()->GetPMSManager()->SetChannelCount(DWORD(channel_list_.size()));
#endif
    SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=added channel(%d)|"), channel_id);
    return true;
}

bool ns_world::WorldChannelManager::RemoveChannel(
    ns_world::ServerSessionInWorld* binding_session)
{
    if (shutdown_process_) {
        ASSERT(channel_list_.empty());
        return true;
    };
    CHANNEL_MAP::iterator it = channel_list_.begin();
    const CHANNEL_MAP::const_iterator end = channel_list_.end();
    for (; it != end; ++it)
    {
        const ChannelPair& channel_pair = *it;
        if (channel_pair.session != binding_session) {
            continue;
        };
        break;
    };
    if (it == end) {
        ASSERT(!"unexpected state");
        return false;
    };

    const ChannelPair& channel_pair = *it;
    if (ns_world::WorldUserManager* user_manager = ns_world::WorldUserManager::Instance()) {
        user_manager->OnCloseChannel(channel_pair.channel);
    };
    const int channel_id = channel_pair.channel->GetChannelID(); __TOUCHED(channel_id);
    SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=removed channel(%d)|"), channel_id);
    //
    TAllocDelete(ns_world::community::WorldChannel, channel_pair.channel);
    channel_list_.erase(it);
#ifdef _JP_0_20101123_PMS
    WorldServer::Instance()->GetPMSManager()->SetChannelCount(DWORD(channel_list_.size()));
#endif
    return true;
}

ns_world::community::WorldChannel* ns_world::WorldChannelManager::FindChannel(
    ns_world::ServerSessionInWorld* agent_session)
{
    return agent_session->GetChannel();
}

ns_world::community::WorldChannel* ns_world::WorldChannelManager::FindChannel(
    DWORD channel_id) const
{
    CHANNEL_MAP::const_iterator it = channel_list_.begin();
    const CHANNEL_MAP::const_iterator end = channel_list_.end();
    for (; it != end; ++it)
    {
        const ChannelPair& channel_pair = *it;
        if (channel_pair.session->GetChannelID() == channel_id) {
            return channel_pair.channel;
        };
    };
    return NULL;
}
/*
void ns_world::WorldChannelManager::SendToAllChannel(BYTE *pMsg, WORD wSize)
{
    CHANNEL_MAP_ITER    it;
    ns_world::community::WorldChannel             *pChannel;
    for (it = channel_list_.begin(); it != channel_list_.end(); ++it)
    {
        pChannel = it->second;
        pChannel->SendToAll(pMsg, wSize);
    }
}
*/

void ns_world::WorldChannelManager::SendToStaticFields(MSG_BASE* msg_base, uint16_t msg_size)
{
    CHANNEL_MAP::const_iterator it = channel_list_.begin();
    const CHANNEL_MAP::const_iterator end = channel_list_.end();
    for (; it != end; ++it)
    {
        ns_world::community::WorldChannel* channel = (*it).channel;
        channel->SendToStaticFields(msg_base, msg_size);
    };
}

void ns_world::WorldChannelManager::SendToBattleZones(MSG_BASE* msg_base, uint16_t msg_size)
{
    CHANNEL_MAP::const_iterator it = channel_list_.begin();
    const CHANNEL_MAP::const_iterator end = channel_list_.end();
    for (; it != end; ++it)
    {
        ns_world::community::WorldChannel* channel = (*it).channel;
        channel->SendToBattleZones(msg_base, msg_size);
    };
}

void ns_world::WorldChannelManager::DisplayChannelInfo()
{
    CHANNEL_MAP::const_iterator it = channel_list_.begin();
    const CHANNEL_MAP::const_iterator end = channel_list_.end();
    for (; it != end; ++it)
    {
        ns_world::community::WorldChannel* channel = (*it).channel;
        channel->DisplayInfo();
    };
}

//==================================================================================================
// ChatRoom service interfaces

ns_world::community::ChatRoomCommunity*
ns_world::WorldChannelManager::GetLinkedChatRoom(ns_world::WorldUser* user)
{
    return user->chat_room_node_.chat_room;
};

ulong ns_world::WorldChannelManager::GetChatRoomGuid(\
    const util::StrArrayForRoomTitle& input_chat_room_name)
{
    util::StrArrayForRoomTitle chat_room_name;
    _tcsncpy(chat_room_name, input_chat_room_name, _countof(chat_room_name));
    chat_room_name[_countof(chat_room_name) - 1] = _T('\0');
    _tcslwr(chat_room_name);
    return util::StreamHashKey::GetStringHashKey(chat_room_name);
};

ns_world::community::ChatRoomCommunity*
ns_world::WorldChannelManager::GetChatRoomCommunity(ulong chatroom_guid) const
{
    ChatRoomTable::const_iterator found = chat_room_table_.find(chatroom_guid);
    if (found == chat_room_table_.end()) {
        return NULL;
    };
    ns_world::community::ChatRoomCommunity* chat_room = found->second;
    return chat_room;
}

ns_world::community::ChatRoomCommunity*
ns_world::WorldChannelManager::GetChatRoomCommunityByName(
    const util::StrArrayForRoomTitle& input_chat_room_name) const
{
    ulong chat_room_guid = this->GetChatRoomGuid(input_chat_room_name);
    return this->GetChatRoomCommunity(chat_room_guid);
}

bool ns_world::WorldChannelManager::EnterChatRoomCommunity(
    ns_world::WorldUser* user, const util::StrArrayForRoomTitle& input_chat_room_name)
{
    // TODO: check whether the user is already joined other chat_room.
    if (user->chat_room_node_.chat_room != NULL)
    {
        MSG_CW_CHAT_CHANNEL_JOIN_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_CHAT_ALREADYINROOM;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg='%s' is already joined the channel (%s)|"),
               user->GetCharName(), user->chat_room_node_.chat_room->chat_room_name_);
        return false;
    };

    util::StrArrayForRoomTitle chat_room_name;
    _tcsncpy(chat_room_name, input_chat_room_name, _countof(chat_room_name));
    chat_room_name[_countof(chat_room_name) - 1] = _T('\0');

    MSG_CW_CHAT_CHANNEL_JOIN_ACK msg_ack;
    MSG_CW_CHAT_CHANNEL_JOIN_BRD msg_brd;
    ;{
        ZeroMemory(&msg_ack, sizeof(msg_ack));
        new (&msg_ack) MSG_CW_CHAT_CHANNEL_JOIN_ACK;
        ZeroMemory(&msg_brd, sizeof(msg_brd));
        new (&msg_brd) MSG_CW_CHAT_CHANNEL_JOIN_BRD;

        msg_brd.m_byChannelID = static_cast<uint8_t>(\
            user->GetUserAccountInfo().channel->GetChannelID());
        _tcsncpy(msg_brd.m_ptszJoinMember, user->GetCharName(), _countof(msg_brd.m_ptszJoinMember));
        msg_brd.m_ptszJoinMember[_countof(msg_brd.m_ptszJoinMember) - 1] = _T('\0');
    };

    ns_world::community::ChatRoomCommunity* chat_room = GetChatRoomCommunityByName(chat_room_name);
    if (chat_room == NULL)
    {   // NOTE: if this flow result is fails, ChatRoomCommunity object is release by Update.
        chat_room = TAllocNew(ns_world::community::ChatRoomCommunity);
        chat_room->RegisterRoomName(chat_room_name);
        msg_ack.m_bCreated = true;
    };

    size_t number_of_joined_users = chat_room->GetNumberOfJoinedUsers();
    if (number_of_joined_users >= chat_room->kMaxNumberOfJoinedMembers)
    {
        MSG_CW_CHAT_CHANNEL_JOIN_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_CHAT_UNKNOWN;
        user->SendPacket(&msg_nak, sizeof(msg_nak));

        SUNLOG(eFULL_LOG,
            _T("|["__FUNCTION__"]|Msg=this chatroom('%s') cannot take on any more user, (%d)|"),
            chat_room_name, number_of_joined_users);
        return false;
    };

    // 기존 멤버들에게 신규유저 들어온다고 알려주고
    BOOST_STATIC_ASSERT(MAX_CHATZONE_USER == chat_room->kMaxNumberOfJoinedMembers &&
                        MAX_CHATZONE_USER <= UCHAR_MAX);
    // CHANGES: f110908.1L, remove '+1', custom logic = "(BYTE)number_of_users + 1"
    msg_brd.m_byUserNum = static_cast<uint8_t>(number_of_joined_users);
    chat_room->SendToAll(&msg_brd, sizeof(msg_brd));

    if (chat_room->Enter(user, user->GetConnectedCharInfo()) == false)
    {
        MSG_CW_CHAT_CHANNEL_JOIN_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_CHAT_UNKNOWN;
        user->SendPacket(&msg_nak, sizeof(msg_nak));
        // 들어오지 않았다는 걸 확인시켜준다
        MSG_CW_CHAT_CHANNEL_OUT_BRD out_msg_brd;
        _tcsncpy(out_msg_brd.m_ptszOutName, msg_brd.m_ptszJoinMember,
                 _countof(out_msg_brd.m_ptszOutName)); // null-terminated
        out_msg_brd.m_byUserNum = msg_brd.m_byUserNum;

        chat_room->SendToAll(&out_msg_brd, sizeof(out_msg_brd));
        return false;
    };
    // user->JoinChannelChatRoom( pChatZone ); <- chat_root->Enter
    number_of_joined_users = chat_room->GetNumberOfJoinedUsers();
    msg_brd.m_byUserNum = static_cast<uint8_t>(number_of_joined_users);
    user->SendPacket(&msg_ack, sizeof(msg_ack));

    return true;
}

bool ns_world::WorldChannelManager::LeaveChatRoomCommunity(ns_world::WorldUser* user)
{
    ns_world::community::ChatRoomUserNode* const user_chat_node = &user->chat_room_node_;
    if (user_chat_node->chat_room == NULL) {
        ASSERT(util::LList::IsEmpty(user_chat_node));
        return false;
    };
    ns_world::ConnectedCharInfo origin_info = user->GetConnectedCharInfo();
    return user_chat_node->chat_room->Leave(user, &origin_info);
}

bool ns_world::WorldChannelManager::LoadChatRoomMembers(ns_world::WorldUser* user,
    ns_functor::ChatRoomMemberListFilter::MemberInfoArray& result_member_list,
    uint8_t* const result_number_of_members) const
{
    *result_number_of_members = 0;
    //
    ns_world::community::ChatRoomUserNode* const user_chat_node = &user->chat_room_node_;
    const ns_world::community::ChatRoomCommunity* const chat_room = user_chat_node->chat_room;
    if (FlowControl::FCAssert(chat_room != NULL) == false) {
        return false;
    };
    BOOST_STATIC_ASSERT(MAX_CHATZONE_USER == chat_room->kMaxNumberOfJoinedMembers &&
                        MAX_CHATZONE_USER == _countof(result_member_list));

    const ns_world::community::ChatRoomUserNode* it = chat_room->user_list_.next;
    for (int counter = 0;
         counter < chat_room->kMaxNumberOfJoinedMembers && it != &chat_room->user_list_;
         it = it->next, ++(*result_number_of_members), ++counter)
    {
        util::StrArrayForCharName& dest_spot = result_member_list[*result_number_of_members];
        _tcsncpy(dest_spot, it->base->GetCharName(), _countof(dest_spot));
    };
    return true;
}
//==================================================================================================


void ns_world::WorldChannelManager::Update()
{
#if WORLD_CONTENTS_BLOCK
    // 일정 시간마다 동접 정보 전송
    DWORD           dwTotalCount=0, dwVillageCount=0, dwBattleZoneCount=0;

    // 10초마다 체크...더럽게 빨리 하네...-0-
    if (statistics_timer_.IsExpired())
    {
        CHANNEL_MAP_ITER    it;
        ns_world::community::WorldChannel *pChannel;
        MSG_MW_USERCOUNT_SYN    sendMsg;
        BYTE                    byChannel;
        DWORD                   dwTotalTemp = 0;    // 전체 동접

        sendMsg.byChannelCount = 0;
        for (it = channel_list_.begin(); it != channel_list_.end(); ++it)
        {
            pChannel = it->second;
            byChannel = sendMsg.byChannelCount;
            sendMsg.ChannelInfo[byChannel].channel_id = pChannel->GetChannelID();

            pChannel->RefreshUserNum();         // 일단 매번 유저 숫자 갱신 - Inc/Dec 쓸 경우 예외 상황에 대한 대처가 제대로 안 된다

            // 임시로 로딩중이거나 필드 지역들에 포함되지 않는 곳에 있는 놈들을 배틀존 동접에 합산한다
            // 합산 방법: 수정된 배틀존 동접 = 배틀존 동접 + (남는 동접)
            //                               = 배틀존 동접 + (전체 동접 - 마을 동접 - 배틀존 동접)
            //                               = 전체 동접 - 마을 동접

            sendMsg.ChannelInfo[byChannel].dwVillageCount = pChannel->GetZoneUserNum(eZONETYPE_CHARSELECT) 
                                                            + pChannel->GetZoneUserNum(eZONETYPE_VILLAGE)
                                                            + pChannel->GetZoneUserNum(eZONETYPE_DOMINATION_FIELD)
                                                            + pChannel->GetZoneUserNum(eZONETYPE_FIELD);

            if (pChannel->GetNumerOfNormalUsers() < sendMsg.ChannelInfo[byChannel].dwVillageCount)
            {
                MessageCount(eCRITICAL_LOG, "ns_world::community::WorldChannel[%u] WorldUser Statistics Error! Not Correct Field UserNum!![%u/%u]", 
                                                                                        pChannel->GetNumerOfNormalUsers(),
                                                                                        sendMsg.ChannelInfo[byChannel].dwVillageCount);
                sendMsg.ChannelInfo[byChannel].dwBattleZoneCount = 0;
            }
            else
            {
                sendMsg.ChannelInfo[byChannel].dwBattleZoneCount = pChannel->GetNumerOfNormalUsers()
                                                                - sendMsg.ChannelInfo[byChannel].dwVillageCount;
            }

            sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount = pChannel->GetZonePCRoomUserNum(eZONETYPE_CHARSELECT)
                                                                + pChannel->GetZonePCRoomUserNum(eZONETYPE_VILLAGE)
                                                                + pChannel->GetZonePCRoomUserNum(eZONETYPE_DOMINATION_FIELD)
                                                                + pChannel->GetZonePCRoomUserNum(eZONETYPE_FIELD);

            if (pChannel->GetNumerOfPCRoomUsers() < sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount)
            {
                MessageCount(eCRITICAL_LOG, "ns_world::community::WorldChannel[%u] WorldUser Statistics Error! Not Correct Field UserNum!![%u/%u]", 
                                                                                    pChannel->GetNumerOfNormalUsers(),
                                                                                    sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount);
                sendMsg.ChannelInfo[byChannel].dwBattleZonePCRoomCount = 0;
            }
            else
            {
                sendMsg.ChannelInfo[byChannel].dwBattleZonePCRoomCount = pChannel->GetNumerOfPCRoomUsers()
                                                                        - sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount;
            }

            MessageCount(eFULL_LOG, "Connected Users[ns_world::community::WorldChannel%d]:Total(%u)Field(%u/%u)Battle(%u/%u)",
                            sendMsg.ChannelInfo[byChannel].channel_id, 
                            pChannel->GetTotalUserNum(), 
                            sendMsg.ChannelInfo[byChannel].dwVillageCount, sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount, 
                            sendMsg.ChannelInfo[byChannel].dwBattleZoneCount, sendMsg.ChannelInfo[byChannel].dwBattleZonePCRoomCount);

            dwTotalCount += pChannel->GetTotalUserNum();
            dwVillageCount += sendMsg.ChannelInfo[byChannel].dwVillageCount + sendMsg.ChannelInfo[byChannel].dwVillagePCRoomCount;
            dwBattleZoneCount += sendMsg.ChannelInfo[byChannel].dwBattleZoneCount + sendMsg.ChannelInfo[byChannel].dwBattleZonePCRoomCount;
            sendMsg.byChannelCount++;
        }

        MessageCount(eCRITICAL_LOG, "Total Users:Total(%u)Field(%u)Battle(%u)", dwTotalCount, dwVillageCount, dwBattleZoneCount);

        // ServerInfo를 파일에서 받지 않았는데
        if (!WorldServerInfoParser::Instance()->IsServerInfoFromFile())
        {
            // 마스터 서버에 접속도 안 돼 있으면 동접 보고에 실패한 거다!
            ServerSession *pMasterServer = ServerSessionManager::Instance()->GetMasterServer();
            if (!pMasterServer)
            {
                MessageOut(eCRITICAL_LOG, "Connect Statistics Send Failed - No Master Server");
                return;
            }
            pMasterServer->Send((BYTE*)&sendMsg, sendMsg.GetSize());
            MessageOut(eFULL_LOG, "Send Connect Statistics to Master Server");
        }
    }
#endif //WORLD_CONTENTS_BLOCK
}

ns_world::community::FieldInfo* ns_world::WorldChannelManager::GetFieldInfo(DWORD field_code)
{
    FieldInfoTable::const_iterator found = field_info_table_.find(field_code);
    if (found == field_info_table_.end()) {
        return NULL;
    };

    ns_world::community::FieldInfo* field_info = found->second;
    return field_info;
}

bool ns_world::WorldChannelManager::LoadFieldInfo()
{
    //  (WAVERIX)(080605)(reduce error rates)
    MapInfoParser* const map_info_parser = MapInfoParser::Instance();
    const sMAPINFO_HASH& map_table = map_info_parser->GetMapInfoHash();
    if (map_table.empty())
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=map table is empty"));
        return true;
    };

    // 마을 맵인 경우만 실제로 읽는다
    //  (WAVERIX)(080718)(memory leak buf-fix) : [해외] solarauth와의 재 접속시 문제가 야기된다.
    //  - field_info_table_는 LoadFieldInfo가 호출되는 시점에만 추가된다.
    //  - 따라서 field_info_table_에 값이 존재시 그냥 true반환하기로 한다.
    if (field_info_table_.empty() == false)
    {
        SUNLOG(eCRITICAL_LOG, "Already Loaded Fields");
        return true;
    };

    CDiscAccess disc_access;
    disc_access.Create();

    FOREACH_CONTAINER(const sMAPINFO_HASH::value_type& map_node, map_table, sMAPINFO_HASH)
    {
        const sMAPINFO* const map_info = map_node.second;
        const eZONETYPE zone_type = map_info->GetZoneType4SharedCode();

        bool used_sector_control = (zone_type == eZONETYPE_FIELD) ||
                                   (zone_type == eZONETYPE_VILLAGE) ||
                                   (zone_type == eZONETYPE_DOMINATION_FIELD);

        for (int index = 0; index < _countof(map_info->FCode); ++index)
        {
            const FIELDCODE field_code = map_info->FCode[index];
            if (field_code == 0) {
                continue;
            };
            if (used_sector_control == false) {
                continue;
            };
            if (this->GetFieldInfo(field_code)) {
                continue;
            };

            ns_world::community::FieldInfo* const field_info = \
                TAllocNew(ns_world::community::FieldInfo);
            if (!field_info->Create(map_info_parser->GetFieldInfo(field_code),
                    true, &disc_access))
            {
                continue;
            };

            //const FIELDCODE field_code = field_info->GetFieldCode();
            const FieldInfoTable::_Pairib inserted = field_info_table_.insert(\
                FieldInfoTable::value_type(field_code, field_info));

            if (FlowControl::FCAssert(inserted.second))
            {
                SUNLOG(eFULL_LOG, "FieldInfo[%u] Template Add Completed", field_code);
            }
            else
            {
                SUNLOG(eCRITICAL_LOG, "FieldInfo[%u] Duplicated Info %u\n", field_code);
                delete field_info;
            };
        }; //end 'for'
    }; //end 'foreach map_info in map_infos

    return true;
}

