#include "stdafx.h"
#include "./GuildLog.h"

#include "Services/Guild/Guild.h"

//==================================================================================================

GuildLog* g_guild_log = NULL;

//==================================================================================================

GuildLog::GuildLog()
{
}

GuildLog::~GuildLog()
{
}

void GuildLog::LogGuildCreation(Guild* guild)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_CRTE, guild);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogGuildBreakUpWaiting(Guild* guild)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_DTRY_WAIT, guild);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogGuildBreakUpCanceled(Guild* guild)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_DTRY_CACL, guild);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogGuildBreakUpCompleted(Guild* guild)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_DTRY_CPLT, guild);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogCorpsCreation(Guild* guild, GuildMember* commander, BYTE corpsNum)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild || !commander) { return; }

    LogGuildCommon(GLD_CORP_CRTE, guild);

    data_.SetMemberCharGuid(commander->GetCharGuid());
    data_.SetMemberCharName(commander->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetCorpsNum(corpsNum);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogCorpsBreakUp(Guild* guild, GuildMember* op, BYTE corpsNum)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild || !op) { return; }

    LogGuildCommon(GLD_CORP_DTRY, guild);

    data_.SetMemberCharGuid(op->GetCharGuid());
    data_.SetMemberCharName(op->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetCorpsNum(corpsNum);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogCampCreation(Guild* guild, GuildMember* leader, BYTE corpsNum)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild || !leader) { return; }

    LogGuildCommon(GLD_CAMP_CRT, guild);

    data_.SetMemberCharGuid(leader->GetCharGuid());
    data_.SetMemberCharName(leader->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetCorpsNum(corpsNum);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogCampBreakUp(Guild* guild, GuildMember* op, BYTE corpsNum)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild || !op) { return; }

    LogGuildCommon(GLD_CAMP_DTRY, guild);

    data_.SetMemberCharGuid(op->GetCharGuid());
    data_.SetMemberCharName(op->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetCorpsNum(corpsNum);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogMemberJoin(Guild* guild, DWORD memberGuid, const TCHAR* memberName)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_MEM_JOIN, guild);

    data_.SetMemberCharGuid(memberGuid);
    data_.SetMemberCharName(memberName, MAX_CHARNAME_LENGTH);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogMemberLeave(Guild* guild, GuildMember* member)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_MEM_OUT, guild);

    data_.SetMemberCharGuid(member->GetCharGuid());
    data_.SetMemberCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogMemberKickOut(Guild* guild, GuildMember* op, GuildMember* member)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_MEM_KICK, guild);

    data_.SetMemberCharGuid(op->GetCharGuid());
    data_.SetMemberCharName(op->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetTargetCharGuid(member->GetCharGuid());
    data_.SetTargetCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogDutyChange(Guild* guild, GuildMember* member, eGUILD_DUTY prevDuty)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild || !member) { return; }

    LogGuildCommon(GLD_DUTY_CHG, guild);

    data_.SetMemberCharGuid(member->GetCharGuid());
    data_.SetMemberCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetPrevMemberDuty(BYTE(prevDuty));
    data_.SetMemberDuty((BYTE)member->GetDuty());

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogDelegation(Guild* guild, GuildMember* member, BYTE right)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild || !member) { return; }

    LogGuildCommon(GLD_RIGT_DLGT, guild);

    data_.SetMemberCharGuid(member->GetCharGuid());
    data_.SetMemberCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetMemberRight(right);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogRankUp(Guild* guild, BYTE prevGrade)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }
    
    LogGuildCommon(GULD_LEV_UP, guild);

    data_.SetPrevGuildGrade(prevGrade);
    data_.SetGuildGrade((BYTE)guild->GetGuildGrade());
    data_.SetTotalUP(guild->GetGuildUP());
    data_.SetTotalGP(guild->GetGuildGP());

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogUPDonation(Guild* guild, GuildMember* member, DWORD deltaUP, DWORD totalUP)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild || !member) { return; }

    LogGuildCommon(GLD_UP_GIVE, guild);

    data_.SetMemberCharGuid(member->GetCharGuid());
    data_.SetMemberCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetDeltaUP(deltaUP);
    data_.SetTotalUP(totalUP);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogGuildRelation(Guild* guild, GUILDGUID target_id, Guild* target, BYTE relation)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild ) {
        return;
    }

    LogGuildCommon(GLD_REL_SET, guild);

    data_.SetRelation(relation);
    data_.SetTargetGuildGuid(target_id);
    const TCHAR* guild_name = ( target != NULL ? target->GetGuildName() : "[Unknown]" );
    data_.SetTargetGuildName(guild_name, MAX_GUILDNAME_LENGTH);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogGuildUnrelation(Guild* guild, GUILDGUID target_id, Guild* target, BYTE relation)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild ) {
        return;
    }

    LogGuildCommon(GLD_REL_CACL, guild);

    data_.SetRelation(relation);
    data_.SetTargetGuildGuid(target_id);
    const TCHAR* guild_name = ( target != NULL ? target->GetGuildName() : "[Unknown]" );
    data_.SetTargetGuildName(guild_name, MAX_GUILDNAME_LENGTH);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogGuildCommon(int logType, Guild* guild)
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    data_.Init();

    data_.SetLogType(logType);

    data_.SetGuildGuid(guild->GetGuildGuid());
    data_.SetGuildName(guild->GetGuildName(), MAX_GUILDNAME_LENGTH);

    GuildMember* master = guild->FindMaster();
    if (master)
    {
        data_.SetMasterCharGuid(master->GetCharGuid());
        data_.SetMasterCharName(master->GetCharName(), MAX_CHARNAME_LENGTH);
    }
#endif
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

void GuildLog::LogGuildJoinRequest( Guild* guild, CHARGUID char_guid, const TCHAR* char_name )
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_JOIN_REQUEST, guild);

    data_.SetDefaultColumn1(char_guid);
    data_.SetDefaultColumn3(char_name, MAX_CHARNAME_LENGTH);

    m_LogMng.Serialize(data_);
#endif
}

void GuildLog::LogGuildFacilityUpgrade( Guild* guild, GuildMember* member, FCODE code, FLEVEL level, bool is_upgrade /*= true*/ )
{
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    if (!IsRun() || !guild) { return; }

    if (is_upgrade == true)
    {
        LogGuildCommon(GLD_FACILITY_UPGRADE, guild);
    }
    else
    {
        LogGuildCommon(GLD_FACILITY_DOWNGRADE, guild);
    }

    data_.SetDefaultColumn1(code);
    data_.SetDefaultColumn2(level);

    if (member != NULL)
    {
        TCHAR buff[MAX_GUILDLOG_COLUMNSIZE];
        memset(buff, 0, sizeof(buff));
        _snprintf(buff, MAX_GUILDLOG_COLUMNSIZE, "%d", member->GetCharGuid());
        data_.SetDefaultColumn3(buff, MAX_GUILDLOG_COLUMNSIZE);
        data_.SetDefaultColumn4(member->GetCharName(), MAX_CHARNAME_LENGTH);
    }

    m_LogMng.Serialize(data_);
#endif
}

#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

//
