#include "StdAfx.h"
#include "./GameGuild.h"

#include <Struct.h>
#include <GuildRankInfoParser.h>

#include "Party/GameParty.h"
#include "Party/GamePartyManager.h"
#include "GameGuildExtension.h"
#include "GameZone.h"

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "GuildFacilities.h"
#include "GuildFacilityInfoParser.h"
#include "ItemManager.h"
#include "SkillAttrCalculator.h"
#include "SkillInfoParser.h"
#include "SkillManager.h"
#include "StatusManager.h"
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
#include "GameDominationManager.h"
#endif //_NA_008334_20150608_SONNENSCHEIN
//==================================================================================================

__IMPL_GUILDPOOL(GameGuild);

//==================================================================================================

GameGuild::GameGuild()
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
: guild_facilities_(NULL)
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
, reward_option_mgr_(NULL)
#endif //_NA_008334_20150608_SONNENSCHEIN
{
    ZeroMemory(&guild_info_, sizeof(GAME_GUILD_INFO));
}

GameGuild::~GameGuild()
{
    Destroy();
}

void GameGuild::Destroy()
{
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* member = node.second;
        if (member) {
            member->Release();
            GameGuildMember::FREE(member);
        }
    };
    member_table_.clear();
    guild_relation_entry_.RelationRelease();
    guild_warehouse_.Release();

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    SAFE_DELETE(guild_facilities_);
#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#ifdef _NA_008334_20150608_SONNENSCHEIN
    SAFE_DELETE(reward_option_mgr_);
#endif //_NA_008334_20150608_SONNENSCHEIN
}

GameGuildMember* GameGuild::FindMember(CHARGUID char_guid) const
{
    GUILDMEMBER_HASH::const_iterator found_it = member_table_.find(char_guid);
    if (found_it == member_table_.end()) {
        return NULL;
    };

    GameGuildMember* guild_member = found_it->second;
    return guild_member;
}

void GameGuild::RemoveMember(CHARGUID char_guid) {
    member_table_.erase(char_guid);
}

void GameGuild::AddMember(GameGuildMember* guild_member, CHARGUID char_guid) {
    assert(guild_member);
    member_table_.insert(GUILDMEMBER_HASH::value_type(char_guid, guild_member));
}

void GameGuild::Withdraw(CHARGUID char_guid)
{
    GameGuildMember* guild_member = FindMember(char_guid);
    if (FlowControl::FCAssert(guild_member != NULL))
    {
        guild_member->Release();
        RemoveMember(char_guid);
        GameGuildMember::FREE(guild_member);
    };
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
void GameGuild::SetChuntingPVPInfo(CHUNTING_PVP_INFO& IN pvp_info)
{
    guild_info_.m_ChuntingPVPInfo = pvp_info;
}
#endif

void GameGuild::SetMemberDuty(CHARGUID char_guid, eGUILD_DUTY guild_duty)
{
    GameGuildMember* guild_member = FindMember(char_guid);
    if (guild_member == NULL) {
        return;
    };
    guild_member->SetDuty(guild_duty);
}

void GameGuild::CreateRearCamp(BYTE number_of_corps)
{
    if (number_of_corps == 0 || number_of_corps >= MAX_GUILD_CORPS_NUM) {
        return;
    }
    if (checked_rear_camps_[number_of_corps] != false)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Already Create RearCamp, CorpsNumber = %d",
               number_of_corps);
        return;
    }

    checked_rear_camps_[number_of_corps] = true;
}

void GameGuild::DestroyRearCamp(BYTE number_of_corps)
{
    if (number_of_corps == 0 || number_of_corps >= MAX_GUILD_CORPS_NUM) {
        return;
    };
    if (checked_rear_camps_[number_of_corps] != false)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Already Destroy RearCamp, CorpsNumber = %d",
               number_of_corps);
        return;
    }

    checked_rear_camps_[number_of_corps] = false;
}

BOOL GameGuild::IsCreateRearCamp(BYTE number_of_corps)
{
    if (number_of_corps == 0 || number_of_corps >= MAX_GUILD_CORPS_NUM) {
        return false;
    };
    return checked_rear_camps_[number_of_corps];
}

void GameGuild::Create(const GAME_GUILD_INFO& guild_info,
                       const BASE_GUILD_MEMBER_INFO& guild_member_info)
{
    guild_info_ = guild_info;
    AddMember(guild_member_info);
    guild_warehouse_.SetOwnerGuild(this);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    guild_facilities_ = new GuildFacilities;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#ifdef _NA_008334_20150608_SONNENSCHEIN
    reward_option_mgr_ = new GuildRewardOptionManager(this);
#endif //_NA_008334_20150608_SONNENSCHEIN
}

void GameGuild::DestroyMember(CHARGUID char_guid)
{
    GameGuildMember* guild_member = FindMember(char_guid);
    if (guild_member)
    {
        guild_member->Release();
        RemoveMember(char_guid);

        GameGuildMember::FREE(guild_member);
    }
}

void GameGuild::AddMember(const BASE_GUILD_MEMBER_INFO& guild_member_info)
{
    if (FindMember(guild_member_info.m_CharGuid))
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Member Exist = %u And Destroy ",
               guild_member_info.m_CharGuid);
        DestroyMember(guild_member_info.m_CharGuid);
    }

    GameGuildMember* guild_member = GameGuildMember::ALLOC();
    guild_member->Init(guild_member_info);
    AddMember(guild_member, guild_member->GetCharGuid());
}

void GameGuild::Join(const BASE_GUILD_MEMBER_INFO& guild_member_info)
{
    GameGuildMember* guild_member = GameGuildMember::ALLOC();
    guild_member->Init(guild_member_info);
    AddMember(guild_member, guild_member->GetCharGuid());
}

void GameGuild::SetGuildGrade(BYTE guild_grade)
{ 
    guild_info_.m_GuildGrade = guild_grade;

#if !defined(_NA_003923_20120130_GUILD_RENEWAL)
    if (guild_grade == GUILDGRADE_AT_GUILDWAREHOUSE_CREATE) 
    {
        guild_warehouse_.SetSlotTabNum(GUILDWAREHOUSE_TAB_COUNT_AT_GUILD_CREATE);
    }
#endif //_NA_003923_20120130_GUILD_RENEWAL
}

void GameGuild::PartyToLeaveByRelation(DWORD hostility_guild_guid)
{
    // 길드관계에 의한 파티쪽 처리는 일단 아무처리도 안하는 것으로 한다! 문제가 생기면 다시 생각!!
    // removed block by __NA_0_20081124_PARTY_REFACTORING_GAMESERVER
}

void GameGuild::ChangeGuildName(const TCHAR* change_guild_name)
{
    // 길드 매니저와 자신의 길드명을 바꾼다.
    util::GUILDNAMECOPY(guild_info_.m_tszGuildName, change_guild_name);

    // 길원들의 길드명을 바꾼다.
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        if (member == NULL) {
            continue;
        };
        Player* player = PlayerManager::Instance()->FindPlayerByName(member->GetCharName());
        if (player == NULL) {
            continue;
        };
        BASE_PLAYERINFO* pInfo = player->GetCharInfo();
        util::GUILDNAMECOPY(pInfo->m_tszGuildName, change_guild_name);
    }
}

int GameGuild::GetRelationGuid(STLX_VECTOR<GUILDGUID>& result_guild_guid_list,
                               eGUILD_RELATION relation_type)
{
    int size = guild_relation_entry_.GetRelationCount(relation_type);
    result_guild_guid_list.reserve(size);

    return guild_relation_entry_.GetRelationGuid(result_guild_guid_list, relation_type);
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
// message broadcasters...
//==================================================================================================

void GameGuild::SendPacketAll(MSG_BASE* msg, int msg_size, CHARGUID except_char_guid)
{
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER::InstanceOfBase();
    msg_brd.Init(static_cast<MSG_BASE_FORWARD*>(msg), msg_size);
    uint16* const begin_it = msg_brd.GetUsersStartPtr();

    PlayerManager* const player_manager = PlayerManager::Instance();
    const bool checked_skip_object = (except_char_guid == 0);
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* member = node.second;
        const DWORD char_guid = member->GetCharGuid();
        if (bool filtered = !(checked_skip_object || char_guid != except_char_guid)) {
            continue;
        };

        Player* const player = player_manager->FindPlayerByCharKey(char_guid);
        if (player == NULL) {
            continue;
        };

        DWORD bound_check = msg_brd.AddUser(begin_it, player->GetObjectKey());
        if (bound_check == msg_brd.MAX_USER_COUNT) {
            break;
        };
    }; //end of 'foreach member in guild_members'

    if (msg_brd.number_of_users) {
        g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    };
}

void GameGuild::_SendPacketMember(MSG_BASE* msg, uint16_t msg_size)
{
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER::InstanceOfBase();
    msg_brd.Init(static_cast<MSG_BASE_FORWARD*>(msg), msg_size);
    uint16_t* const begin_it = msg_brd.GetUsersStartPtr();
    //
    PlayerManager* const player_manager = PlayerManager::Instance();
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        Player* const player = player_manager->FindPlayerByCharKey(member->GetCharGuid());
        if (player == NULL) {
            continue;
        };
        if (player->GetField() == NULL) {
            continue;
        };

        DWORD bound_check = msg_brd.AddUser(begin_it, player->GetObjectKey());
        if (bound_check == msg_brd.MAX_USER_COUNT) {
            break;
        };
    }; //end of 'foreach member in guild_members'
    if (msg_brd.number_of_users) {
        g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    };
}

void GameGuild::_SendPacketMemberAround(MSG_BASE* msg, uint16_t msg_size,
                                        DWORD* const msg_object_key_slot)
{
    PlayerManager* const player_manager = PlayerManager::Instance();
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        Player* const player = player_manager->FindPlayerByCharKey(member->GetCharGuid());
        if (player == NULL) {
            continue;
        };
        if (player->GetField() == NULL) {
            continue;
        };
        *msg_object_key_slot = player->GetObjectKey();
        player->SendPacketAround(static_cast<MSG_BASE_FORWARD*>(msg), msg_size, true, 0);
    }
}

void GameGuild::_SendPacketFieldMember(MSG_BASE* msg, uint16_t msg_size, FIELDCODE field_code)
{
    MSG_AG_CONNECTION_PACKET_BROADCASTER_ONEtoN_SYN& msg_brd =
        MSG_ONEtoN_BROADCASTER::InstanceOfBase();
    msg_brd.Init(static_cast<MSG_BASE_FORWARD*>(msg), msg_size);
    uint16_t* const begin_it = msg_brd.GetUsersStartPtr();

    PlayerManager* const player_manager = PlayerManager::Instance();
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        Player* const player = player_manager->FindPlayerByCharKey(member->GetCharGuid());
        if (player == NULL) {
            continue;
        };
        GameField* game_field = player->GetField();
        if (game_field == NULL) {
            continue;
        };
        const FIELDCODE field_code_of_player = game_field->GetFieldCode();
        if (field_code != field_code_of_player) {
            continue;
        };
        DWORD bound_check = msg_brd.AddUser(begin_it, player->GetObjectKey());
        if (bound_check == msg_brd.MAX_USER_COUNT) {
            break;
        };
    }
    if (msg_brd.number_of_users) {
        g_pGameServer->SendToServer(AGENT_SERVER, &msg_brd, msg_brd.GetSendingSize());
    };
}

void GameGuild::_SendPacketMemberAndAroundAll(
    MSG_BASE* member_msg, uint16_t member_msg_size,
    MSG_BASE* arounding_msg, uint16_t arounding_msg_size,
    DWORD* const arounding_msg_object_key_slot)
{
    _SendPacketMember(member_msg, member_msg_size);

    PlayerManager* const player_manager = PlayerManager::Instance();
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        Player* const player = player_manager->FindPlayerByCharKey(member->GetCharGuid());
        if (player == NULL) {
            continue;
        };
        if (player->GetField() == NULL) {
            continue;
        };
        *arounding_msg_object_key_slot = player->GetObjectKey();
        player->SendPacketAround(
            static_cast<MSG_BASE_FORWARD*>(arounding_msg), arounding_msg_size, false, 0);
    };
}

int GameGuild::GetMemberCountInDominataionFiled( const MAPCODE map_code )
{
    int member_count_in_map = 0;

    PlayerManager* const player_manager = PlayerManager::Instance();
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        Player* const player = player_manager->FindPlayerByCharKey(member->GetCharGuid());
        if (player == NULL) {
            continue;
        };
        GameZone* game_zone = player->GetGameZonePtr();
        if (game_zone == NULL || game_zone->GetMapCode() != map_code) {
            continue;
        };

        ++member_count_in_map;    
    }

    return member_count_in_map;
}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
Player* GameGuild::GetGuildMaster()
{
    PlayerManager* const player_manager = PlayerManager::Instance();
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        Player* const player = player_manager->FindPlayerByCharKey(member->GetCharGuid());
        if (player == NULL) {
            continue;
        }
        if (player->GetField() == NULL) {
            continue;
        }
        if (member->GetDuty() == 1) {
            return player;
        }
    }
    return NULL;
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
GuildFacilities*const GameGuild::GetGuildFacilities()
{
    return guild_facilities_;
}

const RC::eGUILD_RESULT GameGuild::CanUpdateGuildFacility( Player* player, FCODE code, FLEVEL add_level /*= 1*/ )
{
    if (guild_facilities_ == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_FACILITY;
    }

    FLEVEL next_level = add_level;
    const GUILD_FACILITY_INFO* cur_facility = guild_facilities_->GetGuildFacility(code);
    if (cur_facility != NULL)
    {
        next_level += cur_facility->level;
    }

    GuildFacilityInfoParser* parser = GuildFacilityInfoParser::Instance();
    const FACILITY_INFO* next_info = parser->GetGuildFacilityInfo(code, next_level);
    if (next_info == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_FACILITY;
    }

    switch(next_info->upgrade_type)
    {
    case FUI_ONLY_ITEM: // 아이템으로 업그레이드
    case FUI_GUILDPOINTANDITEM: // 둘다 필요
        {
            ItemManager* const item_manager = player->GetItemManager();
            if (item_manager->ExistItem(SI_INVENTORY, next_info->upgrade_item_code, next_info->upgrade_item_num)
                < next_info->upgrade_item_num)
            {
                return RC::RC_GUILD_NOT_ENOUGH_UPGRADE_ITEM;
            }
        }
        break;
    case FUI_ONLY_GUILDPOINT: // 길드포인트로 업그레이드
        break;
    default: // 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Invalid facility upgrade type(%d)",next_info->upgrade_type);
            return RC::RC_GUILD_NOT_EXIST_FACILITY;
        }
        break;
    }

    return RC::RC_GUILD_SUCCESS;
}

const void GameGuild::UpdateGuildFacility( CHARGUID char_guid, GUILD_FACILITY_INFO info, FLEVEL cur_level, FLEVEL add_level )
{
    // 시설 변경
    guild_facilities_->SetGuildFacility(info);

    // 이전 시설 패시브 효과 삭제
    const GUILD_FACILITY_INFO*  prev = guild_facilities_->GetGuildFacility(info.code);
    if (prev != NULL)   //이전 효과가 적용이 되어있다면 삭제한다.
    {
        GUILD_FACILITY_MAP del_map;
        GUILD_FACILITY_INFO prev_info;
        prev_info.code = info.code;
        prev_info.level = cur_level;
        del_map.insert(std::make_pair(info.code, prev_info));
        ApplyGuildFacilityPassiveAllMember(false, &del_map);
    }
    
    // 새로운 시설 패시브 효과 추가
    GUILD_FACILITY_MAP add_map;
    add_map.insert(std::make_pair(info.code, info));
    ApplyGuildFacilityPassiveAllMember(true, &add_map);

    // 아이템 삭제
    Player* player = PlayerManager::Instance()->FindPlayerByCharKey(char_guid);
    if (player == NULL || add_level > 0)
    {
        return;
    }

    const FACILITY_INFO* f_info = GuildFacilityInfoParser::Instance()->GetGuildFacilityInfo(info.code, info.level);
    if (f_info == NULL)
    {
        return;
    }

    switch(f_info->upgrade_type)
    {
    case FUI_ONLY_ITEM: // 아이템으로 업그레이드
    case FUI_GUILDPOINTANDITEM: // 둘다 필요
        {
            SLOTCODE ItemTID = f_info->upgrade_item_code;
            POSTYPE Count = f_info->upgrade_item_num;
            MSG_CG_ITEM_LOSE_ACK item_msg;
            ItemManager* const item_manager = player->GetItemManager();
            RC::eITEM_RESULT result = item_manager->DeleteAllKindOfItem( 1, &ItemTID, &Count, &item_msg.m_ItemInfo);
            if (result == RC::RC_ITEM_SUCCESS) 
            {
                player->SendPacket(&item_msg, item_msg.GetSize());
                GAMELOG->LogMultiItem( ITEM_USE_GUILD_FACILITY_UPGRADE, player, 1, &ItemTID, &Count );
            }
            else
            {
                SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] failed update guild facility(%d)", f_info->upgrade_type);
            }
        }
        break;
    case FUI_ONLY_GUILDPOINT: // 길드포인트로 업그레이드 // GDBP에서 처리된다
        break;
    default: // 
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Invalid facility upgrade type(%d)",f_info->upgrade_type);
            return;
        }
        break;
    }
}

const void GameGuild::ApplyGuildFacilityPassive( Player* player, const bool is_add, const GUILD_FACILITY_MAP*  facility_map )
{
    if (player == NULL || facility_map == NULL)
    {
        return;
    }

    GuildFacilityInfoParser* parser = GuildFacilityInfoParser::Instance();

    SkillAttrCalculator attr_calculator(player->GetAttr());

    GUILD_FACILITY_MAP::const_iterator cur_iter = facility_map->begin();
    GUILD_FACILITY_MAP::const_iterator end_iter = facility_map->end();
    for (; cur_iter != end_iter; ++cur_iter)
    {
        FCODE fcode = cur_iter->second.code;
        FLEVEL flevel = cur_iter->second.level;
        const FACILITY_OPTION_PASSIVE* option = parser->GetGuildFacilityPassive(fcode, flevel);
        if (option == NULL)
        {
            continue;
        }

        if (option->option_value == 0)
        {
            continue;
        }

        if (option->option_type < eATTR_TYPE_INVALID || option->option_type >= _countof(g_ITEM_OPTION))
        {
            continue;
        }

        const eATTR_TYPE attr_type = g_ITEM_OPTION[option->option_type];
        if (is_add)
        {
            attr_calculator.AddAttr(attr_type, option->option_value_type, option->option_value, false);
        }
        else
        {
            attr_calculator.DelAttr(attr_type, option->option_value_type, option->option_value, false);
        }
    }
}

const void GameGuild::ApplyGuildFacilityPassiveAllMember( const bool is_add, const GUILD_FACILITY_MAP* facility_map )
{
    PlayerManager* const player_manager = PlayerManager::Instance();
    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* member = node.second;
        const DWORD char_guid = member->GetCharGuid();
        Player* const player = player_manager->FindPlayerByCharKey(char_guid);
        if (player == NULL) {
            continue;
        };
        ApplyGuildFacilityPassive(player, is_add, facility_map);
    };
}

const RC::eGUILD_RESULT GameGuild::ApplyGuildFacilityActive( Player* player, FCODE code )
{
    if (player == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_PLAYER;
    }

    if (player->Rider.IsSummoned())
    {
        return RC::RC_GUILD_NOT_BE_ACTIVATED_IN_RIDE;
    }

    StatusManager* status_mgr = player->GetStatusManager();
    if (status_mgr != NULL)
    {
        const StatusField& status_field = status_mgr->Status;
        if (status_field.IsSpreadWings() == true)
        {
            return RC::RC_GUILD_FACILITY_CANNOT_ACTIVATED_FLYING;
        }
    }

    const GUILD_FACILITY_INFO* facility_info = guild_facilities_->GetGuildFacility(code);
    if (facility_info == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_FACILITY;
    }
    
    GuildFacilityInfoParser* parser = GuildFacilityInfoParser::Instance();
    const FACILITY_OPTION_ACTIVE* active_info = parser->GetGuildFacilityActive(code, facility_info->level);
    if (active_info == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_FACILITY;
    }

    SLOTCODE skill_code = active_info->active_skill_code;
    if(SkillInfoParser::Instance()->GetSkillInfo(skill_code) == NULL)
    {
        return RC::RC_GUILD_NOT_EXIST_FACILITY_ACTIVE;
    }
    
    SLOTCODE item_code = active_info->active_item_code;
    POSTYPE item_count = active_info->active_item_count;
    ItemManager* const item_manager = player->GetItemManager();
    if (item_manager->ExistItem(SI_INVENTORY, item_code, item_count) < item_count)
    {
        return RC::RC_GUILD_NOT_ENOUGH_ACTIVE_ITEM;
    }

    MSG_CG_ITEM_LOSE_ACK item_msg;
    RC::eITEM_RESULT result = item_manager->DeleteAllKindOfItem( 1, &item_code, &item_count, &item_msg.m_ItemInfo);
    if (result == RC::RC_ITEM_SUCCESS) 
    {
        player->SendPacket(&item_msg, item_msg.GetSize());
        GAMELOG->LogMultiItem( ITEM_USE_GUILD_FACILITY_ACTIVE, player, 1, &item_code, &item_count );

        SkillOption skill_option;
        skill_option.Clear();
        skill_option.AddOption(skill_option.eOption_HoldupBuffPostDead);
        const WzVector* cur_pos = player->GetPosPtr();
        SkillInfo skill_info_data(player, skill_code, 0);
        skill_info_data.SetArgsObjectInfo(0, cur_pos, cur_pos, cur_pos);
        skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
            skill_info_data.kDefaultSkillEffect,
            skill_info_data.kDefaultSkillDelay,
            SKILL_FACTOR_NOMAL,
            static_cast<eATTACK_PROPENSITY>(ATTACK_PROPENSITY_NORMAL),
            0);
        skill_info_data.skill_option_.ApplyOption(skill_option);
        player->GetSkillManager()->RegisterSkill(SKILL_TYPE_ACTIVE, &skill_info_data);
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] failed use guild facility active (%d, %d, %d)", 
            player->GetCharGuid(), item_code, item_count);
    }

    //길드 시설 황성화 로그
    GAMELOG->LogGuildFacility(ACT_GUILD_FACILITY_ACTIVE, player, this, code, facility_info->level);

    return RC::RC_GUILD_SUCCESS;
}

#endif //_NA_007330_20140620_GUILD_SYSTEM_EXTENSION

#endif // _NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_008334_20150608_SONNENSCHEIN

VOID GameGuild::InitializeSonnenScheinRewardPhase()
{
    //초기화 전에 소넨샤인버프를 제거한다
    // 등급값 '변화'에 대해서는 해제 후 다시거는 것으로 한다 (길드버프도 있기때문)
    ApplyGuildRewardOptionForAllMembers(FALSE);
    SetSonnenScheinRewardPhase(0);
    ApplyGuildRewardOptionForAllMembers(TRUE);

    MSG_CG_GUILD_SONNENSCHEIN_UPDATE_CMD cmd_msg;
    cmd_msg.sonnenschein_phase = GetSonnenScheinRewardPhase();
    cmd_msg.m_dwPlayerKey = 0;
    SendPacketMemberAround(cmd_msg, INVALID_GUILDGUID);
}

VOID GameGuild::ApplyGuildRewardOptionForAllMembers( BOOL is_turn_on )
{
    reward_option_mgr_->SetRewardOptionMap(GetGuildGuid(), GetSonnenScheinRewardPhase());

    FOREACH_CONTAINER(const GUILDMEMBER_HASH::value_type& node, member_table_, GUILDMEMBER_HASH)
    {
        GameGuildMember* const member = node.second;
        if (member == NULL) {
            continue;
        };
        Player* player = PlayerManager::Instance()->FindPlayerByName(member->GetCharName());
        if (player == NULL) {
            continue;
        };

        reward_option_mgr_->ApplyRewardOption(player, is_turn_on);
    }
}

VOID GameGuild::ApplyGuildRewardOption( Player* const player, BOOL is_turn_on /*= TRUE*/ )
{
    reward_option_mgr_->SetRewardOptionMap(GetGuildGuid(), GetSonnenScheinRewardPhase());
    reward_option_mgr_->ApplyRewardOption(player, is_turn_on);
}

//==================================================================================================

VOID GuildRewardOptionManager::AddDominationRewardOption(const DominationInfoParser::SelectedRewardOptions& sel_options)
{
    DominationInfoParser::SelectedRewardOptions::const_iterator itr = sel_options.begin();
    for (itr; itr != sel_options.end(); ++itr)
    {
        const DominationInfoParser::SelectedRewardOption& option = itr->second;
        const AttrOptionInfo* option_info = option.option_info;

        _AddOption(option_info->m_wOptionKind, option_info->m_byOptionType, option_info->m_iOptionValue);
    }
}

VOID GuildRewardOptionManager::AddSonnenScheinRewardOption( const SScheinRewardInfo* reward_info )
{
    for (int count = 0; count < SScheinRewardInfo::REWARD_BUFF_SIZE; ++count)
    {
        const AttrOptionInfo* option_info = &(reward_info->reward_buff_option[count]);
        if (option_info->m_wOptionKind == 0 || option_info->m_iOptionValue == 0) {
            continue;
        }

        _AddOption(option_info->m_wOptionKind, option_info->m_byOptionType, option_info->m_iOptionValue);
    }
}

VOID GuildRewardOptionManager::ApplyRewardOption( Player* const player, BOOL is_turn_on /*= TRUE */ )
{
    if (player == NULL) {
        return;
    }

    GameGuildMember* member = game_guild_->FindMember(player->GetCharGuid());
    if (member == NULL) {
        return;
    }

    //버프적용flag값과 turnon값이 같은경우
    // TRUE == TRUE --> 버프가 걸려있는데 또 걸려고 한다
    // FALSE == FALSE --> 버프안걸려있는데 해제하려고 한다
    if (member->GetFlag_RewardOption() == is_turn_on) 
    {
        return;
    }

    std::map<WORD, AttrOptionInfo>::iterator itr = option_map_.begin();

    SkillAttrCalculator attr_calculator(player->GetAttr());
    for (itr; itr != option_map_.end(); ++itr)
    {
        AttrOptionInfo& option_info = itr->second;
        if (option_info.m_wOptionKind == 0 || option_info.m_iOptionValue == 0) {
            continue;
        }

        const eATTR_TYPE attr_type = g_ITEM_OPTION[option_info.m_wOptionKind];
        if (is_turn_on == TRUE) {
            attr_calculator.AddAttr(
                attr_type, option_info.m_byOptionType, option_info.m_iOptionValue, false);
        }
        else {
            attr_calculator.DelAttr(
                attr_type, option_info.m_byOptionType, option_info.m_iOptionValue, false);
        }

        SUNLOG( eDEV_LOG, "["__FUNCTION__"] 보상옵션 적용 완료(ADDorDel:%d)(Type:%d)(ValueType:%d)(Value:%d)", 
            (is_turn_on ? 1 : 0), attr_type, option_info.m_byOptionType, option_info.m_iOptionValue);
    }

    member->SetFlag_OptionActive(is_turn_on);
}

VOID GuildRewardOptionManager::SetRewardOptionMap(const GUILDGUID& guild_guid, const BYTE& sschein_phase)
{
    //옵션을 구할 때, option_map_을 리셋시킨후 구한다
    _ResetOptionMap();
    
    //Domination Option
    DominationInfoParser::TakenMaps taken_maps;
    GameDominationManager::Instance()->GetGuildTakenMaps(guild_guid, &taken_maps);

    const DominationInfoParser* const parser = DominationInfoParser::Instance();
    DominationInfoParser::SelectedRewardOptions sel_options = parser->GetRewardOption(taken_maps);

    AddDominationRewardOption(sel_options);

    //SonnenSchein Option
    if (sschein_phase != 0)
    {
        const SScheinRewardInfo* reward_info = 
            SonnenScheinRewardInfoParser::Instance()->FindData(sschein_phase);
        if (reward_info != NULL) 
        {
            AddSonnenScheinRewardOption(reward_info);
        }
    }
}

VOID GuildRewardOptionManager::_AddOption(WORD option_kind, BYTE option_type, INT option_value)
{
    std::map<WORD,AttrOptionInfo>::iterator founded = option_map_.find(option_kind);
    if (founded != option_map_.end())
    {
        //이미 있는옵션, 더 높은 Value의 옵션을 사용하도록 한다
        const AttrOptionInfo& registed = founded->second;
        if (option_value > registed.m_iOptionValue)
        {
            AttrOptionInfo option_info(option_kind, option_type, option_value);
            option_map_[option_kind] = option_info;
        }
    }
    else
    {
        AttrOptionInfo option_info(option_kind, option_type, option_value);
        option_map_[option_kind] = option_info;
    }
}

VOID GuildRewardOptionManager::_ResetOptionMap()
{
    option_map_.clear();
}

#endif //_NA_008334_20150608_SONNENSCHEIN
//==================================================================================================
//==================================================================================================
//==================================================================================================