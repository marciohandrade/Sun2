#include "WorldServerPch.hxx"
#include "./WorldGuild.h"

#include "Objects/WorldUserManager.h"
#include "Objects/WorldUser.h"
//
#include "Services/Guild/WorldGuildMember.h"

__IMPL_GUILDPOOL(ns_world::WorldGuild);

ns_world::WorldGuild::WorldGuild()
{
    member_table_.Initialize(_MAX_GUILDMEMBER_NUM);
    ZeroMemory(&world_guild_info_, sizeof(world_guild_info_));
}

ns_world::WorldGuild::~WorldGuild()
{
//  Destroy();
}

void ns_world::WorldGuild::Destroy()
{
    GUILDMEMBER_HASH::iterator member_it = member_table_.begin();
    const GUILDMEMBER_HASH::iterator member_end = member_table_.end();
    for (; member_it != member_end; ++member_it)
    {
        ns_world::WorldGuildMember* guild_member = *member_it;
        guild_member->Release();
        ns_world::WorldGuildMember::FREE(guild_member);
    };
    member_table_.RemoveAll();
}

void ns_world::WorldGuild::Withdraw(CHARGUID char_guid)
{
    ns_world::WorldGuildMember* guild_member = FindMember(char_guid);
    if (guild_member == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=Error - No Guild Member!!![char_guid:%u]|"),
               char_guid);
        return;
    }
    
    RemoveMember(char_guid);
    ns_world::WorldGuildMember::FREE(guild_member);
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
void ns_world::WorldGuild::SetChuntingPVPInfo(CHUNTING_PVP_INFO & IN PVPInfo)
{
    world_guild_info_.m_ChuntingPVPInfo = PVPInfo;
}
#endif

//길드 직책
BOOL ns_world::WorldGuild::ChangeMemberDuty(CHARGUID target_char_guid, eGUILD_DUTY guild_duty)
{
    ns_world::WorldGuildMember* guild_member = FindMember(target_char_guid);
    if (guild_member == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=GuildMember Not Exist, char_guid = %d|"),
               target_char_guid);
        return false;
    };

    guild_member->SetDuty(guild_duty);

    return true;
}

void ns_world::WorldGuild::CreateRearCamp(BYTE corps_index)
{
    if (corps_index == 0 || corps_index >= MAX_GUILD_CORPS_NUM) {
        return;
    };
    if (rear_camp_[corps_index] == true)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=Already Create RearCamp, CorpsNumber = %d"),
               corps_index);
        return;
    };

    rear_camp_[corps_index] = true;
}

void ns_world::WorldGuild::DestroyRearCamp(BYTE corps_index)
{
    if (corps_index == 0 || corps_index >= MAX_GUILD_CORPS_NUM)
        return;

    if (rear_camp_[corps_index] == true)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=Already Destroy RearCamp, CorpsNumber = %d"),
               corps_index);
        return;
    }

    rear_camp_[corps_index] = false;
}

void ns_world::WorldGuild::Create(
    const WORLD_GUILD_INFO& world_guild_info, const BASE_GUILD_MEMBER_INFO& member_info)
{
    //길드의 생성은 ALLOC으로 MANAGER에서 하고 온다.
    world_guild_info_ = world_guild_info;
    CreateMember(member_info);
}

void ns_world::WorldGuild::CreateMember(const BASE_GUILD_MEMBER_INFO& member_info)
{
    if (FindMember(member_info.m_CharGuid)) {
        DestroyMember(member_info.m_CharGuid);
    };
    ns_world::WorldGuildMember* guild_member = ns_world::WorldGuildMember::ALLOC();
    guild_member->Serialize(const_cast<BASE_GUILD_MEMBER_INFO&>(member_info), SERIALIZE_STORE);
    AddMember(guild_member, guild_member->GetCharGuid());
}

void ns_world::WorldGuild::DestroyMember(const CHARGUID& char_guid)
{
    ns_world::WorldGuildMember* guild_member = FindMember(char_guid);
    if (guild_member)
    {
        guild_member->Release();
        RemoveMember(char_guid);
        ns_world::WorldGuildMember::FREE(guild_member);
    }
}

void ns_world::WorldGuild::SendAllMember(MSG_BASE* msg_base, WORD msg_size)
{
    // 현재 길드 멤버이고, 유저 객체가 있을 때만 이 함수가 작동된다.
    // 로그 아웃같은 작업을 할 경우, 남아 있는 길드 멤버들에게 메세지를 전달 하려면
    // 이 함수 호출전에 반드시 길드 멤버에서 객체를 지우거나, 아니면 온라인 상태가 아니라는 것을 세팅해라.
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    //
    GUILDMEMBER_HASH::iterator member_it = member_table_.begin();
    const GUILDMEMBER_HASH::iterator member_end = member_table_.end();
    for (; member_it != member_end; ++member_it)
    {
        ns_world::WorldGuildMember* guild_member = *member_it;
        // NOTE: old-comments, 길드에서 UserManager를 가져다 쓰는 구조는 별로 좋지 않다. 
        // 될수 있으면 연관성을 떨어 뜨려야 한다. 좋은 방법 없을까?
        ns_world::WorldUser* user = user_manager->FindUserByCharGuid(guild_member->GetCharGuid());
        if (user == NULL) {
            continue;
        };
        user->SendPacket(msg_base, msg_size);
    };
}

void ns_world::WorldGuild::SendCorpsMember(BYTE corps_index, MSG_BASE* msg_base, WORD msg_size)
{
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    //
    GUILDMEMBER_HASH::iterator member_it = member_table_.begin();
    const GUILDMEMBER_HASH::iterator member_end = member_table_.end();
    for (; member_it != member_end; ++member_it)
    {
        ns_world::WorldGuildMember* guild_member = *member_it;
        if (guild_member->GetCorpsNum() == corps_index)
        {
            ns_world::WorldUser* user = \
                user_manager->FindUserByCharGuid(guild_member->GetCharGuid());
            if (user == NULL) {
                continue;
            };
            user->SendPacket(msg_base, msg_size);
        }
    };
}

void ns_world::WorldGuild::SerializeWorldGuildInfo(
    INOUT WORLD_GUILD_INFO& world_guild_info, eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        // 현 클래스에서 값을 가져갈때..
        world_guild_info = world_guild_info_;
    }
    else
    {
        // 현 클래스에 값을 쓸때..
        world_guild_info_ = world_guild_info;
    }
}

#if SUN_CODE_SAMPLE
void ns_world::WorldGuild::SerializeBaseMemberInfo(
    INOUT BASE_GUILD_MEMBER_INFO& base_guild_member_info, eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        //가지고 갈때.. 무조건 char_guid값은 들어있다고 가정한다.
        ns_world::WorldGuildMember* guild_member = FindMember(base_guild_member_info.m_CharGuid);
        if (guild_member) {
            guild_member->Serialize(base_guild_member_info, se);
        };
    }
    else
    {
        //저장할 때.
        ns_world::WorldGuildMember* guild_member = FindMember(base_guild_member_info.m_CharGuid);
        if (guild_member) {
            DestroyMember(base_guild_member_info.m_CharGuid);
        };
        CreateMember(base_guild_member_info);
    }
}
#endif //SUN_CODE_SAMPLE

// NOTE: f110915.1L, replacement about SerializeBaseMemberInfo(SERIALIZE_STORE)
void ns_world::WorldGuild::StoreBaseMemberInfo(const BASE_GUILD_MEMBER_INFO& member_info)
{
    ns_world::WorldGuildMember* guild_member = FindMember(member_info.m_CharGuid);
    if (guild_member) {
        DestroyMember(member_info.m_CharGuid);
    };
    CreateMember(member_info);
}

void ns_world::WorldGuild::Join(const BASE_GUILD_MEMBER_INFO& member_info)
{
    // 내부적으로 길드 멤버 생성과 동일한 로직이다.
    CreateMember(member_info);
}

const util::StrArrayForGuildNotice& ns_world::WorldGuild::GetGuildNotice()
{
    return world_guild_info_.m_szGuildNotice;
}

void ns_world::WorldGuild::SetGuildNotice(const util::StrArrayForGuildNotice& ptszNotice)
{
    // TODO: f110825.2L, check boundary
    strncpy(world_guild_info_.m_szGuildNotice, ptszNotice,
            _countof(world_guild_info_.m_szGuildNotice));
    world_guild_info_.m_szGuildNotice[_countof(world_guild_info_.m_szGuildNotice) - 1] = _T('\0');
}

void ns_world::WorldGuild::ChangeGuildName(const util::StrArrayForGuildName& guild_name)
{
    // TODO: f110825.2L, check boundary
    _tcsncpy(world_guild_info_.m_tszGuildName, guild_name,
             _countof(world_guild_info_.m_tszGuildName));
    world_guild_info_.m_tszGuildName[_countof(world_guild_info_.m_tszGuildName) - 1] = _T('\0');
}
