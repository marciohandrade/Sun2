#pragma once
#ifndef DBPROXY_GUILD_LOG_H
#define DBPROXY_GUILD_LOG_H

//==================================================================================================
//_NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
/*
 * GuildLog.h
 *
 * ��� �α� ������ ���� ���� ���̳ʸ� �α� ���ؿ� ���߱� ���� ���ۼ�
 */

#include "BaseLog.h"
#include "GuildData.h"

class Guild;
class GuildMember;

class GuildLog : public BaseLog
{
public:
    GuildLog();
    ~GuildLog();

    // ��� �� â��/��ü ����

    // ��� â��
    void LogGuildCreation(Guild* guild);
    // ��� ��ü ���
    void LogGuildBreakUpWaiting(Guild* guild);
    // ��� ��ü ���
    void LogGuildBreakUpCanceled(Guild* guild);
    // ��� ��ü �Ϸ�
    void LogGuildBreakUpCompleted(Guild* guild);
    // ���� â��
    void LogCorpsCreation(Guild* guild, GuildMember* commander, BYTE corpsNum);
    // ���� ��ü
    void LogCorpsBreakUp(Guild* guild, GuildMember* op, BYTE corpsNum);
    // �Ʒô�(ķ��) ����
    void LogCampCreation(Guild* guild, GuildMember* leader, BYTE corpsNum);
    // �Ʒô�(ķ��) ��ü
    void LogCampBreakUp(Guild* guild, GuildMember* op, BYTE corpsNum);

    // ��� ��� ����

    // ��� ��� ����
    void LogMemberJoin(Guild* guild, DWORD memberGuid, const TCHAR* memberName);
    // ��� ��� Ż��
    void LogMemberLeave(Guild* guild, GuildMember* member);
    // ��� ��� �߹�
    void LogMemberKickOut(Guild* guild, GuildMember* op, GuildMember* member);
    // ��� ��� ��å ����
    void LogDutyChange(Guild* guild, GuildMember* member, eGUILD_DUTY prevDuty);
    // ��� ��� ���� ����
    void LogDelegation(Guild* guild, GuildMember* member, BYTE right);

    // ��� �Ӽ� ����
    
    // ��� ��ũ��
    void LogRankUp(Guild* guild, BYTE prevGrade);
    // UP ����
    void LogUPDonation(Guild* guild, GuildMember* member, DWORD deltaUP, DWORD totalUP);

    // ��� �� ����

    // ��� ���� ����
    void LogGuildRelation(Guild* guild, GUILDGUID target_id, Guild* target, BYTE relation);
    // ��� ���� ����
    void LogGuildUnrelation(Guild* guild, GUILDGUID target_id, Guild* target, BYTE relation);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    // ��� ���� ��û
    void LogGuildJoinRequest(Guild* guild, CHARGUID char_guid, const TCHAR* char_name);

    // ��� �ü� ���׷��̵�
    void LogGuildFacilityUpgrade(Guild* guild, GuildMember* member, FCODE code, FLEVEL level, bool is_upgrade = true);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

private:
    void LogGuildCommon(int logType, Guild* guild);

    GuildData data_;
};

//==================================================================================================
extern GuildLog* g_guild_log;
//==================================================================================================

#endif //DBPROXY_GUILD_LOG_H