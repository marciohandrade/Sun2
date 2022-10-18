#include "WorldServerPch.hxx"
#include "./WorldChannel.h"

#include <MapInfoParser.h>

#include "./WorldChannelManager.h"
#include "./ZoneCommunity.h"
#include "./PartyCommunity.h"
#include "./ChatRoomCommunity.h"
#include "./FieldInfo.h"

#include "Objects/WorldUser.h"
#include "Objects/WorldUserManager.h"
#include "Sessions/ServerSessionInWorld.h"

//==================================================================================================

ns_world::community::WorldChannel::WorldChannel(
    int channel_id, ns_world::ServerSessionInWorld* binded_session)
    : blocked_chat_(false)
    , channel_id_(channel_id)
    , binded_session_(binded_session)
{
    util::LList::Init(&user_root_);
    party_table_.Initialize(100);
    this->BuildMapInfo();
}

ns_world::community::WorldChannel::~WorldChannel()
{
    ;{
        PartyTable::iterator party_it = party_table_.begin();
        const PartyTable::iterator party_end = party_table_.end();
        for (; party_it != party_end; ++party_it)
        {
            ns_world::community::PartyCommunity* party = *party_it;
            TAllocDelete(ns_world::community::PartyCommunity, party);
        };
        party_table_.clear();
    };
    //
    ZoneTable* field_list_array[2] = { &dynamic_field_table_, &static_field_table_ };
    for (ZoneTable** table_it = field_list_array;
         table_it != &field_list_array[_countof(field_list_array)];
         ++table_it)
    {
        ZoneTable* const table = *table_it;
        ZoneTable::const_iterator it = table->begin();
        const ZoneTable::const_iterator end = table->end();
        for (; it != end; ++it)
        {
            ns_world::community::ZoneCommunity* zone = it->second;
            TAllocDelete(ns_world::community::ZoneCommunity, zone);
        };
        table->clear();
    };
    ;{
        ChannelUserNode* node_it = user_root_.next;
        while (node_it != &user_root_)
        {
            assert(!"can't entered at this point...");
            ChannelUserNode* node = node_it;
            node_it = node_it->next;
            ns_world::WorldUserManager::Logout(node->base, ns_functor::LogoutOption::Free(false));
        };
    };
    if (binded_session_)
    {
        binded_session_->BindChannel(NULL);
        binded_session_ = NULL;
    };
}

bool ns_world::community::WorldChannel::BuildMapInfo()
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));
    // NOTE: f110520.5L, f081029.x, make static field unique index
    // like villages, fields and domination fields.
    // the '1' key index reserved here to support the character selectable zone.
    const KEYTYPE kConstStaticZoneStartKey = 2;
    const MapInfoParser* const map_parser = MapInfoParser::Instance();
    const sMAPINFO_HASH& map_info_table = map_parser->GetMapInfoHash();

    //static_field_table_.clear();
    ns_world::WorldChannelManager* const channel_manager = ns_world::WorldChannelManager::Instance();
    FOREACH_CONTAINER(const sMAPINFO_HASH::value_type& node, map_info_table, sMAPINFO_HASH)
    {
        const sMAPINFO* const map_info = node.second;
        const eZONETYPE zone_type = map_info->GetZoneType4SharedCode();

        bool is_static_zone = zone_type == eZONETYPE_VILLAGE ||
                              zone_type == eZONETYPE_FIELD ||
                              zone_type == eZONETYPE_DOMINATION_FIELD;
        if (is_static_zone == false) {
            continue;
        };
        // if the zone has sub-fields, the ranges are incremented from start_zone_key.
        ASSERT(map_info->reserved_key_index_ != 0);
        KEYTYPE start_zone_key = map_info->reserved_key_index_;
        CONSOLELOG(eCRITICAL_LOG, _T("Adding Map='%d'"), map_info->MapCode);
        // NOTE: f110520.5L, the FCode index is sequential since 2011.05.03, f110503.6L.
        const MAPCODE* field_it = map_info->FCode,
                     * const field_end = &map_info->FCode[1];//&map_info->FCode[map_info->byFCount];
        for (; field_it != field_end; ++field_it, ++start_zone_key)
        {
            const FIELDCODE field_code = (*field_it);
            if (field_code == 0) { // assert(field_code != 0)
                break;
            };
            //
            FieldInfo* field_info = channel_manager->GetFieldInfo(field_code);
            assert(field_info != NULL);
            //
            ns_world::community::ZoneCommunity* zone = \
                TAllocNew(ns_world::community::ZoneCommunity)(start_zone_key, map_info, field_info);
            //
            static_field_table_.insert(ZoneTable::value_type(start_zone_key, zone)); // single loop
            CONSOLELOG(eCRITICAL_LOG, _T("    Added FieldInfo='%d'"), field_info->GetFieldCode());
        };
    };
    //----------------------------------------------------------------------------------------------
    // add linked zones
    struct LinkedZoneMaker
    {
        typedef STLX_MAP<MAPCODE, ns_world::community::ZoneCommunity*> LinkedZone;
        void Insert(MAPCODE map_code, ns_world::community::ZoneCommunity* zone)
        {
            linked_zone.insert(LinkedZone::value_type(map_code, zone));
        };
        ns_world::community::ShoutRange* FindShoutRange(
            ns_world::community::ShoutRangeTable* table, MAPCODE map_code) const
        {
            ns_world::community::ShoutRangeTable::iterator found = table->find(map_code);
            if (found == table->end()) {
                return NULL;
            };

            return &found->second;
        };
        ns_world::community::ZoneCommunity* Find(MAPCODE map_code) const
        {
            LinkedZone::const_iterator found = linked_zone.find(map_code);
            if (found == linked_zone.end()) {
                return NULL;
            };
            return found->second;
        };
        LinkedZone linked_zone;
    } builder;
    ;{
        ZoneTable::const_iterator field_it = static_field_table_.begin();
        const ZoneTable::const_iterator field_end = static_field_table_.end();
        for (; field_it != field_end; ++field_it)
        {
            ns_world::community::ZoneCommunity* zone = field_it->second;
            builder.Insert(zone->map_info_->MapCode, zone);
        };

        ns_world::WorldChannelManager* const channel_manager = \
            ns_world::WorldChannelManager::Instance();
        shout_range_table_ = channel_manager->GetSkeletonShoutRangeTable(); // copied

        field_it = static_field_table_.begin();
        for (; field_it != field_end; ++field_it)
        {
            ns_world::community::ZoneCommunity* zone = field_it->second;
            const MAPCODE map_code = zone->map_info_->MapCode;

            ns_world::community::ShoutRange* const shout_range = \
                builder.FindShoutRange(&shout_range_table_, map_code);
            if (shout_range != NULL)
            {
                for (int index = 0; index < _countof(shout_range->linked_map_codes); ++index)
                {
                    const MAPCODE link_map_code = shout_range->linked_map_codes[index];
                    if (link_map_code == 0) {
                        break;
                    };
                    shout_range->linked_zone[index] = builder.Find(link_map_code);
                };
            };
            zone->RegisterShoutRange(shout_range);
        };
    };

    return true;
    //
};

// ICommunity interface
ns_world::community::eCommunityType ns_world::community::WorldChannel::GetCommunityType() const
{
    return ns_world::community::eCommunity_WorldChannel;
}

bool ns_world::community::WorldChannel::EnabledChat() const
{
    return (blocked_chat_ == false);
}

void ns_world::community::WorldChannel::ConfigureChatEnabled(bool enabled)
{
    blocked_chat_ = !enabled;
}

// ICommunity interface
bool ns_world::community::WorldChannel::Enter(
    WorldUser* user, const ns_world::ConnectedCharInfo& char_info)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));
    __UNUSED(char_info);
    if (WorldChannel* channel = user->channel_node_.link_channel)
    {
        if (channel != this) {
            this->Leave(user, NULL);
        };
    };
    util::LList::Delete(&user->channel_node_);
    util::LList::AddPrev(&user->channel_node_, &user_root_);
    user->channel_node_.link_channel = this;

    return true;
}

// ICommunity interface
bool ns_world::community::WorldChannel::Leave(
    WorldUser* user, ns_world::ConnectedCharInfo* const backup_info)
{
    assert(!"can't entered. this method is unused");
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));

    if (backup_info != NULL) {
        *backup_info = user->connected_char_info_;
    };
    ns_world::WorldUserManager::Logout(user, ns_functor::LogoutOption::Reuse(true));
    //
    return true;
}

void ns_world::community::WorldChannel::SendPacket(MSG_BASE* msg_base, uint16_t msg_size) const
{
    binded_session_->SendPacket(msg_base, msg_size);
}

bool ns_world::community::WorldChannel::SendToAll(
    MSG_BASE* msg_base, uint16_t msg_size) const
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));
    const ZoneTable* field_list_array[2] = { &dynamic_field_table_, &static_field_table_ };
    for (const ZoneTable* const * table_it = field_list_array;
         table_it != &field_list_array[_countof(field_list_array)];
         ++table_it)
    {
        const ZoneTable* const table = *table_it;
        ZoneTable::const_iterator it = table->begin();
        const ZoneTable::const_iterator end = table->end();
        for (; it != end; ++it)
        {
            ns_world::community::ZoneCommunity* link_zone = it->second;
            link_zone->SendToAll(msg_base, msg_size);
        };
    };
    return true;
}

void ns_world::community::WorldChannel::SendToStaticFields(
    MSG_BASE* msg_base, uint16_t msg_size) const
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));
    ZoneTable::const_iterator zone_it = static_field_table_.begin();
    const ZoneTable::const_iterator zone_end = static_field_table_.end();
    for (; zone_it != zone_end; ++zone_it)
    {
        ns_world::community::ZoneCommunity* link_zone = zone_it->second;
        link_zone->SendToAll(msg_base, msg_size);
    };
}

void ns_world::community::WorldChannel::SendToBattleZones(
    MSG_BASE* msg_base, uint16_t msg_size) const
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));
    ZoneTable::const_iterator zone_it = dynamic_field_table_.begin();
    const ZoneTable::const_iterator zone_end = dynamic_field_table_.end();
    for (; zone_it != zone_end; ++zone_it)
    {
        ns_world::community::ZoneCommunity* link_zone = zone_it->second;
        link_zone->SendToAll(msg_base, msg_size);
    };
}

// NOTE: f110923.1L, migrated from WorldServer(old)::SendNormalChatToAll
bool ns_world::community::WorldChannel::SendToAllInShoutRanges(
    MSG_BASE* msg_base, uint16_t msg_size, ns_world::WorldUser* shouter) const
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));

    ns_world::community::ZoneCommunity* const single_zone = shouter->zone_node_.link_zone;
    if (single_zone == NULL) {
        return false;
    };

    // #1. find the linked zone list using 'shouter->zone_node_.link_zone'
    // #2. foreach (link_zone in the linked zone list)
    //          link_zone->SendToAll
    const ns_world::community::ShoutRange* shout_range = single_zone->shout_range_;
    if (shout_range == NULL) {
        return single_zone->SendToAll(msg_base, msg_size);
    };

    for (int index = 0; index < _countof(shout_range->linked_zone); ++index)
    {
        ns_world::community::ZoneCommunity* linked_zone = shout_range->linked_zone[index];
        if (linked_zone == NULL) {
            break;
        };
        linked_zone->SendToAll(msg_base, msg_size);
    };
    return true;
}

void ns_world::community::WorldChannel::DisplayInfo()
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));
};

void ns_world::community::WorldChannel::CreateParty(
    uint16_t party_key)
{
    ns_world::community::PartyCommunity* party = party_table_.GetData(party_key);
    if (FlowControl::FCAssert(party == NULL) == false)
    {
        party->Disperse();
    }
    else
    {
        party = TAllocNew(ns_world::community::PartyCommunity)(party_key);
        party_table_.insert(party_key, party);
    };
}

bool ns_world::community::WorldChannel::JoinParty(
    uint16_t party_key, ns_world::WorldUser* const join_user)
{
    ns_world::community::PartyCommunity* party = party_table_.GetData(party_key);
    if (party == NULL) {
        this->CreateParty(party_key);
        party = party_table_.GetData(party_key);
        assert(party != NULL);
    };
    bool result = party->Enter(join_user, join_user->connected_char_info_);
    if (result) {
        join_user->connected_char_info_.party_key = party_key;
    };
    return result;
}

bool ns_world::community::WorldChannel::WithdrawParty(
    uint16_t party_key, ns_world::WorldUser* const withdraw_user)
{
    ns_world::community::PartyCommunity* party = party_table_.GetData(party_key);
    if (FlowControl::FCAssert(party == NULL) == false) {
        return false;
    };

    bool result = party->Leave(withdraw_user, &withdraw_user->connected_char_info_);
    if (result) {
        withdraw_user->connected_char_info_.party_key = 0;
    };
    if (party->Empty())
    {
        TAllocDelete(ns_world::community::PartyCommunity, party);
        party = NULL;
        party_table_.erase(party_key);
    };
    return result;
}

bool ns_world::community::WorldChannel::DestroyParty(
    uint16_t party_key)
{
    PartyTable::iterator found = party_table_.find(party_key);
    if (FlowControl::FCAssert(found != party_table_.end()) == false) {
        return false;
    };

    ns_world::community::PartyCommunity* party = *found;
    assert(party != NULL);
    TAllocDelete(ns_world::community::PartyCommunity, party);
    party_table_.erase(found);
    return true;
}
//void ns_world::community::WorldChannel::RestoreParty(
//    uint16_t party_key, ns_world::WorldUser* user)
//{
//}

ns_world::community::ZoneCommunity*
ns_world::community::WorldChannel::FindZone(DWORD zone_key) const
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL2("waverix_world_trace",
        "|"__FUNCTION__"|{object}=", this)));
    const ZoneTable* field_list_array[2] = { &dynamic_field_table_, &static_field_table_ };
    for (const ZoneTable* const * table_it = field_list_array;
         table_it != &field_list_array[_countof(field_list_array)];
         ++table_it)
    {
        const ZoneTable* const table = *table_it;
        ZoneTable::const_iterator found = table->find(zone_key);
        if (found == table->end()) {
            continue;
        };
        ns_world::community::ZoneCommunity* zone = found->second;
        return zone;
    };
    return NULL;
}

// NOTE: f110928.1L, side-effect, create zone when the entering zone is not found
bool ns_world::community::WorldChannel::EnterZone(
    ns_world::WorldUser* const user,
    const ns_world::ConnectedCharInfo& char_info)
{
    if (char_info.zone_key == 0 || char_info.map_code == 0) {
        return false;
    };
    ns_world::community::ZoneCommunity* zone = FindZone(char_info.zone_key);
    if (zone == NULL)
    {
        const MapInfoParser* const map_parser = MapInfoParser::Instance();
        const sMAPINFO* map_info = map_parser->FindMapInfo(char_info.map_code);
        const eZONETYPE zone_type = map_info->GetZoneType4SharedCode();
        bool is_static_zone = zone_type == eZONETYPE_VILLAGE ||
                              zone_type == eZONETYPE_FIELD ||
                              zone_type == eZONETYPE_DOMINATION_FIELD;
        ASSERT(is_static_zone == false);

        ns_world::community::ZoneCommunity* new_zone = \
            TAllocNew(ns_world::community::ZoneCommunity)(char_info.zone_key, map_info, NULL);
        //
        dynamic_field_table_.insert(\
            ZoneTable::value_type(char_info.zone_key, new_zone)); // single loop
        zone = new_zone;
    };
    ASSERT(user->GetConnectedCharInfo().zone_key == char_info.zone_key);
    return zone->Enter(user, char_info);
}

// NOTE: f110928.1L, side-effect, destroy zone when the number of entered player is empty
bool ns_world::community::WorldChannel::LeaveZone(
    ns_world::WorldUser* const user)
{
    ns_world::community::ZoneCommunity* zone = user->zone_node_.link_zone;
    if (zone == NULL) {
        return false;
    };
    zone->Leave(user, NULL);
    if (zone->UsedSectorControl() == false && zone->Empty())
    {
        DWORD zone_key = zone->GetZoneKey();
        dynamic_field_table_.erase(zone_key);
        TAllocDelete(ns_world::community::ZoneCommunity, zone);
    };

    if (ns_world::ConnectedCharInfo* editable_char_info = &user->connected_char_info_)
    {
        editable_char_info->zone_type = 0;
        editable_char_info->zone_key = 0;
        editable_char_info->map_code = 0;
        editable_char_info->field_code = 0;
        ZeroMemory(&editable_char_info->position, sizeof(editable_char_info->position));
    };
    return true;
}

void ns_world::community::WorldChannel::LinkShoutRange(
    ns_world::community::ZoneCommunity* zone) const
{
    const MAPCODE map_code = zone->map_info_->MapCode;
    ns_world::community::ShoutRangeTable::const_iterator found = \
        shout_range_table_.find(map_code);
    if (found == shout_range_table_.end()) {
        return;
    };

    const ns_world::community::ShoutRange& shout_range = found->second;
    zone->RegisterShoutRange(&shout_range);
};

//==================================================================================================
//==================================================================================================
//==================================================================================================

#if WORLD_CONTENTS_BLOCK

void ns_world::community::WorldChannel::DisplayInfo()
{
	DWORD dwCharSelectUserNum = 0, dwVillageUserNum = 0, dwFieldUserNum = 0, dwBattleUserNum = 0;
	DWORD dwVillageNum = 0, dwFieldNum = 0, dwBattleNum = 0;
    DWORD dwDominationFieldUserNum = 0;
    DWORD dwDominationFieldNum = 0;

	for(USHORT i=eZONETYPE_LOBBY; i<eZONETYPE_MAX; i++)
	{
		switch(i)
		{
			case eZONETYPE_CHARSELECT:
			{
				dwCharSelectUserNum += m_dwZoneUserNum[i];
				dwCharSelectUserNum += m_dwZonePCRoomUserNum[i];
				break;
			}
			case eZONETYPE_VILLAGE:
			{
				dwVillageUserNum += m_dwZoneUserNum[i];
				dwVillageUserNum += m_dwZonePCRoomUserNum[i];
				dwVillageNum += m_dwZoneNum[i];
				break;
			}
			case eZONETYPE_FIELD:
			{
				dwFieldUserNum += m_dwZoneUserNum[i];
				dwFieldUserNum += m_dwZonePCRoomUserNum[i];
				dwFieldNum += m_dwZoneNum[i];
				break;
			}
            case eZONETYPE_DOMINATION_FIELD:
                {
                    dwDominationFieldUserNum += m_dwZoneUserNum[i];
                    dwDominationFieldUserNum += m_dwZonePCRoomUserNum[i];
                    dwDominationFieldNum += m_dwZoneNum[i];
                    break;
                }
			default:
			{
				dwBattleUserNum += m_dwZoneUserNum[i];
				dwBattleUserNum += m_dwZonePCRoomUserNum[i];
				dwBattleNum += m_dwZoneNum[i];
				break;
			}
		}
	}

	DISPMSG("▷ ns_world::community::WorldChannel No.%d: ", GetChannelID());
	DISPMSG("Total(%u) CharScene(%u) Village(%u/%u) Field(%u/%u) Domination(%u/%u) Battle(%u/%u)\n",
			m_mapUsers.size(), dwCharSelectUserNum, dwVillageNum, dwVillageUserNum, 
            dwFieldNum, dwFieldUserNum, dwDominationFieldNum, dwDominationFieldUserNum,
            dwBattleNum, dwBattleUserNum);
}

// 일반 유저, PC방 유저 전체 숫자 갱신
void ns_world::community::WorldChannel::RefreshUserNum()
{
	USER_MAP_ITER	iter;
	WorldUser			*pUser=NULL;

	m_dwNormalUserNum = 0;
	m_dwPCRoomUserNum = 0;

	for(int i = 0; i < eZONETYPE_MAX; i++)
	{
		m_dwZonePCRoomUserNum[i] = 0;
		m_dwZoneUserNum[i] = 0;
	}

	if(m_mapUsers.empty())
		return;

	for(iter = m_mapUsers.begin(); iter != m_mapUsers.end(); ++iter)
	{
		pUser = (*iter).second;
		if(!pUser)
			continue;

		if(pUser->GetPCRoomID())
		{
			m_dwZonePCRoomUserNum[pUser->GetStatus()]++;
			m_dwPCRoomUserNum++;
		}
		else
		{
			m_dwZoneUserNum[pUser->GetStatus()]++;
			m_dwNormalUserNum++;
		}
	}
}

#endif //WORLD_CONTENTS_BLOCK