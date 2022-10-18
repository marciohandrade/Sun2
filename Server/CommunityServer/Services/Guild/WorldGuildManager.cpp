#include "WorldServerPch.hxx"
#include "./WorldGuildmanager.h"

#include <PacketStruct_WZ.h>

#include "WorldServer.h"
#include "Objects/WorldUser.h"

//==================================================================================================

ns_world::WorldGuildManager::WorldGuildManager()
{
    guild_table_.Initialize(MAX_GUILD_POOL_NUM);
}

ns_world::WorldGuildManager::~WorldGuildManager()
{
//  Release();
}

void ns_world::WorldGuildManager::Release()
{
    GUILD_HASH::iterator guild_it = guild_table_.begin();
    const GUILD_HASH::iterator guild_end = guild_table_.end();
    for (; guild_it != guild_end; ++guild_it)
    {
        ns_world::WorldGuild* guild = *guild_it;
        guild->Destroy();
        ns_world::WorldGuild::FREE(guild);
    }
    guild_table_.RemoveAll();
}

// 1. ����ػ��� ��û�� �̰� ��帶���;� �Ѵ� 
// 2. ���� ��峻�� ������ ��帶���� 1����̾�� �Ѵ� 
// 3. ���������� �¸��Ͽ� �������ϰ� ���� ���� ��忩�� �Ѵ�. 
RC::eGUILD_RESULT ns_world::WorldGuildManager::DestroyGuild(ns_world::WorldUser* user)
{
    GUILDGUID guild_guid = user->GetConnectedCharInfo().guild_guid;
    if (0 == guild_guid) {
        return RC::RC_GUILD_CANNOT_DESTROY_STATE;
    }
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL) {
        return RC::RC_GUILD_INVALID_STATE;
    };
    if (1 != guild->GetMemberNum()) {
        return RC::RC_GUILD_CANNOT_DESTROY_EXIST_MEMBER_STATE;
    };
/*
    MSG_GZ_GUILD_DESTROY_GUILD_SYN msg;
    msg.m_GuildGuid         = guild_guid;
    msg.m_MasterCharGuid    = user->GetCharGuid();
    user->SendToGuildServer(&msg, sizeof(msg));
*/
    return RC::RC_GUILD_SUCCESS;
}

void ns_world::WorldGuildManager::DestroyGuild(GUILDGUID guild_guid)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG,_T("|["__FUNCTION__"]|Msg=Fail, guild_guid = %u"), guild_guid);
        return;
    }

    //1. Manager���� ����
    RemoveGuild(guild->GetGuildGuid());
    //2. Member�� Pool���� ����.
    guild->Destroy();
    //3. Manager Pool���� ����.
    ns_world::WorldGuild::FREE(guild);
}

void ns_world::WorldGuildManager::WithdrawGuild(GUILDGUID guild_guid, CHARGUID char_guid)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG, _T("|["__FUNCTION__"]|Msg=Fail, guild_guid = %u, char_guid = %u"),
               guild_guid, char_guid);
        return;
    }

    guild->Withdraw(char_guid);
}


void ns_world::WorldGuildManager::DisplayerGuildInfo()
{
    DWORD dwNum = 0;
    for (GUILD_HASH::iterator it = guild_table_.begin() ; it != guild_table_.end() ; ++it)
    {
        dwNum += (*it)->GetMemberNum();
    }
    DISPMSG("Guild : %u, GuildMember : %u\n", guild_table_.GetDataNum(), dwNum);
}

//void ns_world::WorldGuildManager::SendGuildMessage(MSG_BASE* msg_base, uint16_t msg_size)
//{
//}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
void ns_world::WorldGuildManager::SetChuntingPVPInfo(
    GUILDGUID guild_guid, CHUNTING_PVP_INFO & IN PVPInfo)
{
    ns_world::WorldGuild *guild = FindGuild(guild_guid);
    if (guild == NULL)   return;

    guild->SetChuntingPVPInfo(PVPInfo);
}
#endif

//��� ��å ����
BOOL ns_world::WorldGuildManager::ChangeMemberDuty(
    GUILDGUID guild_guid, CHARGUID target_char_guid, eGUILD_DUTY guild_duty)
{
    ns_world::WorldGuild *guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=Guild Not Exist, guild_guid = %d"),
               guild_guid);
        return false;
    }

    if (!guild->ChangeMemberDuty(target_char_guid, guild_duty))
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=ChangeMemberDuty Fail, char_guid = %u, Duty = %d"),
               target_char_guid, guild_duty);
        return false;
    }

    return true;
}

BOOL ns_world::WorldGuildManager::SendGuildPacket(
    GUILDGUID guild_guid, MSG_BASE* msg_base, uint16_t msg_size)
{
    ns_world::WorldGuild *guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=Guild Not Exist, guild_guid = %d"),
               guild_guid);
        return false;
    }

    guild->SendAllMember(msg_base, msg_size);
    return true;
}

BOOL    ns_world::WorldGuildManager::CreateRearCamp(GUILDGUID guild_guid, BYTE corps_index)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)     return false;

    guild->CreateRearCamp(corps_index); 

    return true;
}

BOOL    ns_world::WorldGuildManager::DestroyRearCamp(GUILDGUID guild_guid, BYTE corps_index)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)     return false;

    guild->DestroyRearCamp(corps_index); 

    return true;
}

//�����庯��
void ns_world::WorldGuildManager::ChangeCommander(
    GUILDGUID guild_guid, CHARGUID target_char_guid,  BYTE corps_index)
{
    //���Ӽ��������� ������ ���� ���� �ʴ´�. 
    //���� ��� ����� ���ܹ�ȣ�� �ٲ��, ��å�� �ٲ�� �ȴ�.
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)   return;

    WorldGuildMember* pMember = guild->FindMember(target_char_guid);
    if (!pMember)  return;

    pMember->SetDuty(eGUILD_DUTY_COMMANDER);
    pMember->SetCorpsNum(corps_index);
}

//�����̵�.
void ns_world::WorldGuildManager::ChangeCorps(
    GUILDGUID guild_guid,  CHARGUID target_char_guid, BYTE corps_index)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)   return;

    WorldGuildMember* pMember = guild->FindMember(target_char_guid);
    if (!pMember)  return;

    pMember->SetCorpsNum(corps_index);
}

void ns_world::WorldGuildManager::CreateGuild(
    const WORLD_GUILD_INFO& rGuildInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo)
{
    if (FindGuild(rGuildInfo.m_GuildGuid))
    {
        //���� ��尡 �����ϸ�, ��带 ���� �Ѵ�.
        TCHAR ptszGuildName[MAX_GUILDNAME_LENGTH+1] = {0,};
        _tcsncpy(ptszGuildName, rGuildInfo.m_tszGuildName, MAX_GUILDNAME_LENGTH);
        RemoveGuild(rGuildInfo.m_GuildGuid);
    }

    ns_world::WorldGuild* guild = ns_world::WorldGuild::ALLOC();
    guild->Create(rGuildInfo, rMemberInfo);
    AddGuild(guild, guild->GetGuildGuid());
}

void ns_world::WorldGuildManager::LoginGuild(
    const WORLD_GUILD_INFO& GuildInfo, const BASE_GUILD_MEMBER_INFO& member_info)
{
    ns_world::WorldGuild* guild = FindGuild(GuildInfo.m_GuildGuid);

    // ��尡 �����ϸ� ����� �����ϰ�, ��尡 �������� ������ ���� �����Ѵ�.
    if (guild)
    {
        guild->CreateMember(member_info);
    }
    else
    {
        CreateGuild(GuildInfo, member_info);
    }
}

RC::eGUILD_RESULT ns_world::WorldGuildManager::LogoutGuild(
    GUILDGUID guild_guid, CHARGUID char_guid)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)
        return RC::RC_GUILD_NOT_EXIST_GUILD;

    // ��� ����� �ϳ�?
    WorldGuildMember* pMember = guild->FindMember(char_guid);
    if (!pMember)
        return RC::RC_GUILD_NOT_GUILD_MEMBER;

    // �α� �ƿ� �����Ƿ� ���弭�� ��ü���� �����Ѵ�.
    guild->DestroyMember(char_guid);

    // ������� 0�̸� ��带 �ı��Ѵ�.
    if (guild->GetMemberNum() == 0)
    {
        guild->Destroy();
        RemoveGuild(guild_guid);
        ns_world::WorldGuild::FREE(guild);
    }

    return RC::RC_GUILD_SUCCESS;
}

void ns_world::WorldGuildManager::GetCharName(
    GUILDGUID guild_guid, CHARGUID char_guid, OUT char* pszCharName, int nSize)
{
    pszCharName[0] = '\0';

    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)   return;

    WorldGuildMember* pMember = guild->FindMember(char_guid);
    if (!pMember)          return;

    strncpy(pszCharName, pMember->GetCharName(), nSize);
}

// ��� ������ �����Ѵ�.
// �� �Լ��� ���� ������ ���弭���� �����ϱ� �� ���弭���� ���� ��� �ܰ迡�� �̷�� ����.
// ���� ���� ���弭���� ������ ������ ���� ���� �����̴�.
void ns_world::WorldGuildManager::RequestGuildRestore(GUILDGUID guild_guid, CHARGUID char_guid)
{
    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (!dbproxy)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=GuildServer Not Connect, guild_guid = %u, char_guid = %u"),
               guild_guid, char_guid);
        return;
    }

    //1. ��尡 ������ ��� ���� ��û�Ѵ�. ��� ���� ��û�ÿ� ĳ���� �������� ���� ��û�Ѵ�.
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)
    {
        MSG_WZ_GUILD_RESTORE_SYN    synMsg;
        synMsg.m_GuildGuid = guild_guid;
        synMsg.m_CharGuid = char_guid;
        dbproxy->SendPacket(&synMsg, sizeof(synMsg));
        return;
    }

    //3. ��尡 �����ϴµ� ����� ������ ��� ������ ��û�Ѵ�.
    WorldGuildMember* pMember = guild->FindMember(char_guid);
    if (!pMember)
    {
        //����� ���� ��û.
        MSG_WZ_GUILD_MEMBER_INFO_REQ_SYN msg;
        msg.m_GuildGuid = guild_guid;
        msg.m_CharGuid = char_guid;
        dbproxy->SendPacket(&msg, sizeof(msg));
        return;
    }
    
}

void ns_world::WorldGuildManager::GuildRestore(
    const WORLD_GUILD_INFO& GuildInfo, const BASE_GUILD_MEMBER_INFO& member_info)
{
    ns_world::WorldGuild* guild = FindGuild(GuildInfo.m_GuildGuid);

    //1. ������尡 ������ ��� ����� �����Ѵ�.
    if (guild)
    {
        guild->CreateMember(member_info);
    }
    else
    {
        CreateGuild(GuildInfo, member_info);
    }
}

void ns_world::WorldGuildManager::SerializeWorldGuildInfo(
    GUILDGUID guild_guid, INOUT WORLD_GUILD_INFO& Info, eSERIALIZE se)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)       return;
    
    guild->SerializeWorldGuildInfo(Info, se);
}

void ns_world::WorldGuildManager::StoreBaseMemberInfo(
    GUILDGUID guild_guid, const BASE_GUILD_MEMBER_INFO& member_info)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL) {
        return;
    };
    guild->StoreBaseMemberInfo(member_info);
}

//void ns_world::WorldGuildManager::SerializeBaseMemberTotalInfo(GUILDGUID guild_guid, INOUT BASE_GUILD_MEMBER_TOTAL_INFO& Info, eSERIALIZE se)
//{
//  ns_world::WorldGuild* guild = FindGuild(guild_guid);
//  if (guild == NULL)       return;

//  guild->SerializeBaseMemberTotalInfo(Info, se);
//}

void ns_world::WorldGuildManager::KickMember(GUILDGUID guild_guid, CHARGUID char_guid)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)       return;

    guild->DestroyMember(char_guid);
}

void ns_world::WorldGuildManager::JoinGuild(
    GUILDGUID guild_guid, const BASE_GUILD_MEMBER_INFO& rMemberInfo)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)       return;

    guild->Join(rMemberInfo);
}


RC::eGUILD_RESULT   ns_world::WorldGuildManager::CanChangeNotice(
    GUILDGUID guild_guid, CHARGUID oper_char_guid,
    const util::StrArrayForGuildNotice& guild_notice, OUT size_t& notice_len_with_null)
{
    notice_len_with_null = 1;
    if (util::CheckNullTerminatedString(guild_notice) == false) {
        return RC::RC_GUILD_STRING_OVERFLOW;
    };

    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL) {
        return RC::RC_GUILD_NOT_EXIST_GUILD;
    };
    WorldGuildMember* guild_member = guild->FindMember(oper_char_guid);
    if (guild_member == NULL) {
        return RC::RC_GUILD_NOT_GUILD_MEMBER;
    };

    BOOST_STATIC_ASSERT(_countof(guild_notice) == MAX_GUILDNOTICE_LENGTH + 1);
    size_t notice_strlen_with_null_terminated = ::_tcslen(guild_notice) + 1;
    assert(notice_strlen_with_null_terminated <= _countof(guild_notice));

    notice_len_with_null = notice_strlen_with_null_terminated;

    return RC::RC_GUILD_SUCCESS;
}

BOOL        ns_world::WorldGuildManager::ChangeNotice(
    GUILDGUID guild_guid, const util::StrArrayForGuildNotice& pszNotice)
{
    ns_world::WorldGuild* guild = FindGuild(guild_guid);
    if (guild == NULL)
        return false;

    guild->SetGuildNotice(pszNotice);
    return true;
}


void ns_world::WorldGuildManager::SendDominationHeimReturnMemo(
    const MSG_WZ_GUILD_DOMINATION_RETURNHIME_CMD* msg)
{    
    BYTE data_count = msg->m_DataCount;

    if (data_count > _countof(msg->m_ReturnInfo)) //MAX_DOMINATION_AUCTION
    {
        assert(!"Memo count is over");
        SUNLOG(eCRITICAL_LOG,  __FUNCTION__"Memo count is over : %d", msg->m_MapCode);
        return;
    }

    ns_world::ServerSessionInWorld* const dbproxy = \
        ns_world::WorldServer::Instance()->GetGameDBProxy();
    if (dbproxy == NULL)
    {
        SUNLOG(eCRITICAL_LOG,  "["__FUNCTION__"]No GameDBProxy");
        return;
    }

    for (int index = 0; index < data_count; ++index)
    {
        MSG_DW_MEMO_SEND_SYN send_msg;
        send_msg.m_dwKey = 0;    // �ý������κ����� ����
        send_msg.m_SendGuid = 0; // �ý������κ����� ����
        //_NA_20100812_INTEGRATE_SYSTEM_MEMO_DOMINATION = {
        send_msg.m_typeSystemMemo = NORMAL_MEMO;
        send_msg.m_recvItemCode = 0;
        send_msg.m_amountCost = 0;
        send_msg.m_recvGuid = 0;
        //}
        _tcsncpy(send_msg.m_ptszRecvName, msg->m_ReturnInfo[index].m_tszMaster,
                 _countof(send_msg.m_ptszRecvName));
        send_msg.m_ptszRecvName[_countof(send_msg.m_ptszRecvName) - 1]='\0';

        _sntprintf(send_msg.m_ptszMemo, _countof(send_msg.m_ptszMemo), "%d|%d|%I64d|%s|", 
                   msg->m_ReturnInfo[index].m_CostType, 
                   msg->m_MapCode,
                   msg->m_ReturnInfo[index].m_TenderCost,
                   msg->m_tszGuild);
        send_msg.m_ptszMemo[_countof(send_msg.m_ptszMemo) - 1] = '\0';

        dbproxy->SendPacket(&send_msg, sizeof(send_msg));
    }
}

BOOL ns_world::WorldGuildManager::GetValueInMemo(
    const TCHAR* pszMemo, eSYSTEM_MEMO_TYPE& OUT type, MAPCODE& OUT mapcode)
{ 
    std::string strMemo;
    strMemo.assign(pszMemo);
    std::string::size_type firstPos;
    std::string::size_type secondPos;

    //ù��° : Ÿ���� ��´�.
    firstPos = 0;
    secondPos = strMemo.find('|');
    if (firstPos != std::string::npos) 
    {
        std::string tempString = strMemo.substr(firstPos, secondPos);
        type =(eSYSTEM_MEMO_TYPE) atoi(tempString.c_str());
    }
    else
    {
        MessageOut(eCRITICAL_LOG, "["__FUNCTION__"]Not Fount Memo Type");
        return false;
    }

    //�ι�° : MapCode�� ��´�.
    firstPos = strMemo.find('|');
    secondPos = strMemo.find('|', firstPos+1);
    if (secondPos != std::string::npos)
    {
        std::string tempString = strMemo.substr(firstPos+1, secondPos-1);
        mapcode = atoi(tempString.c_str());
    }
    else
    {
        MessageOut(eCRITICAL_LOG, "["__FUNCTION__"]Not Fount mapcode");
        return false;
    }

    return true;
}
