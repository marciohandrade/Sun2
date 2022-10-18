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

//��� ��å
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
    //����� ������ ALLOC���� MANAGER���� �ϰ� �´�.
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
    // ���� ��� ����̰�, ���� ��ü�� ���� ���� �� �Լ��� �۵��ȴ�.
    // �α� �ƿ����� �۾��� �� ���, ���� �ִ� ��� ����鿡�� �޼����� ���� �Ϸ���
    // �� �Լ� ȣ������ �ݵ�� ��� ������� ��ü�� ����ų�, �ƴϸ� �¶��� ���°� �ƴ϶�� ���� �����ض�.
    ns_world::WorldUserManager* const user_manager = ns_world::WorldUserManager::Instance();
    //
    GUILDMEMBER_HASH::iterator member_it = member_table_.begin();
    const GUILDMEMBER_HASH::iterator member_end = member_table_.end();
    for (; member_it != member_end; ++member_it)
    {
        ns_world::WorldGuildMember* guild_member = *member_it;
        // NOTE: old-comments, ��忡�� UserManager�� ������ ���� ������ ���� ���� �ʴ�. 
        // �ɼ� ������ �������� ���� �߷��� �Ѵ�. ���� ��� ������?
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
        // �� Ŭ�������� ���� ��������..
        world_guild_info = world_guild_info_;
    }
    else
    {
        // �� Ŭ������ ���� ����..
        world_guild_info_ = world_guild_info;
    }
}

#if SUN_CODE_SAMPLE
void ns_world::WorldGuild::SerializeBaseMemberInfo(
    INOUT BASE_GUILD_MEMBER_INFO& base_guild_member_info, eSERIALIZE se)
{
    if (se == SERIALIZE_LOAD)
    {
        //������ ����.. ������ char_guid���� ����ִٰ� �����Ѵ�.
        ns_world::WorldGuildMember* guild_member = FindMember(base_guild_member_info.m_CharGuid);
        if (guild_member) {
            guild_member->Serialize(base_guild_member_info, se);
        };
    }
    else
    {
        //������ ��.
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
    // ���������� ��� ��� ������ ������ �����̴�.
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
