#include "stdafx.h"

#include "Guild/Guild.h"

#include "GuildLog.h"

GuildLog::GuildLog()
{
}

GuildLog::~GuildLog()
{
}

void GuildLog::LogGuildCreation(Guild* guild)
{
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_CRTE, guild);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogGuildBreakUpWaiting(Guild* guild)
{
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_DTRY_WAIT, guild);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogGuildBreakUpCanceled(Guild* guild)
{
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_DTRY_CACL, guild);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogGuildBreakUpCompleted(Guild* guild)
{
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_DTRY_CPLT, guild);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogCorpsCreation(Guild* guild, GuildMember* commander, BYTE corpsNum)
{
    if (!IsRun() || !guild || !commander) { return; }

    LogGuildCommon(GLD_CORP_CRTE, guild);

    data_.SetMemberCharGuid(commander->GetCharGuid());
    data_.SetMemberCharName(commander->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetCorpsNum(corpsNum);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogCorpsBreakUp(Guild* guild, GuildMember* op, BYTE corpsNum)
{
    if (!IsRun() || !guild || !op) { return; }

    LogGuildCommon(GLD_CORP_DTRY, guild);

    data_.SetMemberCharGuid(op->GetCharGuid());
    data_.SetMemberCharName(op->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetCorpsNum(corpsNum);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogCampCreation(Guild* guild, GuildMember* leader, BYTE corpsNum)
{
    if (!IsRun() || !guild || !leader) { return; }

    LogGuildCommon(GLD_CAMP_CRT, guild);

    data_.SetMemberCharGuid(leader->GetCharGuid());
    data_.SetMemberCharName(leader->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetCorpsNum(corpsNum);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogCampBreakUp(Guild* guild, GuildMember* op, BYTE corpsNum)
{
    if (!IsRun() || !guild || !op) { return; }

    LogGuildCommon(GLD_CAMP_DTRY, guild);

    data_.SetMemberCharGuid(op->GetCharGuid());
    data_.SetMemberCharName(op->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetCorpsNum(corpsNum);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogMemberJoin(Guild* guild, DWORD memberGuid, const TCHAR* memberName)
{
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_MEM_JOIN, guild);

    data_.SetMemberCharGuid(memberGuid);
    data_.SetMemberCharName(memberName, MAX_CHARNAME_LENGTH);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogMemberLeave(Guild* guild, GuildMember* member)
{
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_MEM_OUT, guild);

    data_.SetMemberCharGuid(member->GetCharGuid());
    data_.SetMemberCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogMemberKickOut(Guild* guild, GuildMember* op, GuildMember* member)
{
    if (!IsRun() || !guild) { return; }

    LogGuildCommon(GLD_MEM_KICK, guild);

    data_.SetMemberCharGuid(op->GetCharGuid());
    data_.SetMemberCharName(op->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetTargetCharGuid(member->GetCharGuid());
    data_.SetTargetCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogDutyChange(Guild* guild, GuildMember* member, BYTE prevDuty)
{
    if (!IsRun() || !guild || !member) { return; }

    LogGuildCommon(GLD_DUTY_CHG, guild);

    data_.SetMemberCharGuid(member->GetCharGuid());
    data_.SetMemberCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetPrevMemberDuty(prevDuty);
    data_.SetMemberDuty((BYTE)member->GetDuty());

    m_LogMng.Serialize(data_);
}

void GuildLog::LogDelegation(Guild* guild, GuildMember* member, BYTE right)
{
    if (!IsRun() || !guild || !member) { return; }

    LogGuildCommon(GLD_RIGT_DLGT, guild);

    data_.SetMemberCharGuid(member->GetCharGuid());
    data_.SetMemberCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetMemberRight(right);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogRankUp(Guild* guild, BYTE prevGrade)
{
    if (!IsRun() || !guild) { return; }
    
    LogGuildCommon(GULD_LEV_UP, guild);

    data_.SetPrevGuildGrade(prevGrade);
    data_.SetGuildGrade((BYTE)guild->GetGuildGrade());
    data_.SetTotalUP(guild->GetGuildUP());
    data_.SetTotalGP(guild->GetGuildGP());

    m_LogMng.Serialize(data_);
}

void GuildLog::LogUPDonation(Guild* guild, GuildMember* member, DWORD deltaUP, DWORD totalUP)
{
    if (!IsRun() || !guild || !member) { return; }

    LogGuildCommon(GLD_UP_GIVE, guild);

    data_.SetMemberCharGuid(member->GetCharGuid());
    data_.SetMemberCharName(member->GetCharName(), MAX_CHARNAME_LENGTH);
    data_.SetDeltaUP(deltaUP);
    data_.SetTotalUP(totalUP);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogGuildRelation(Guild* guild, GUILDGUID target_id, Guild* target, BYTE relation)
{
    if (!IsRun() || !guild ) {
        return;
    }

    LogGuildCommon(GLD_REL_SET, guild);

    data_.SetRelation(relation);
    data_.SetTargetGuildGuid(target_id);
    const TCHAR* guild_name = ( target != NULL ? target->GetGuildName() : "[Unknown]" );
    data_.SetTargetGuildName(guild_name, MAX_GUILDNAME_LENGTH);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogGuildUnrelation(Guild* guild, GUILDGUID target_id, Guild* target, BYTE relation)
{
    if (!IsRun() || !guild ) {
        return;
    }

    LogGuildCommon(GLD_REL_CACL, guild);

    data_.SetRelation(relation);
    data_.SetTargetGuildGuid(target_id);
    const TCHAR* guild_name = ( target != NULL ? target->GetGuildName() : "[Unknown]" );
    data_.SetTargetGuildName(guild_name, MAX_GUILDNAME_LENGTH);

    m_LogMng.Serialize(data_);
}

void GuildLog::LogGuildCommon(int logType, Guild* guild)
{
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
}
