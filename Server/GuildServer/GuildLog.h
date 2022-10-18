/*
 * GuildLog.h
 *
 * 길드 로그 형식을 기존 게임 바이너리 로그 기준에 맞추기 위해 재작성
 */
#ifndef _GUILD_LOG_H_
#define _GUILD_LOG_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "BaseLog.h"
#include "GuildData.h"

class Guild;
class GuildMember;

class GuildLog : public BaseLog
{
public:
    GuildLog();
    ~GuildLog();

    // 길드 등 창설/해체 관련

    // 길드 창설
    void LogGuildCreation(Guild* guild);
    // 길드 해체 대기
    void LogGuildBreakUpWaiting(Guild* guild);
    // 길드 해체 취소
    void LogGuildBreakUpCanceled(Guild* guild);
    // 길드 해체 완료
    void LogGuildBreakUpCompleted(Guild* guild);
    // 군단 창설
    void LogCorpsCreation(Guild* guild, GuildMember* commander, BYTE corpsNum);
    // 군단 해체
    void LogCorpsBreakUp(Guild* guild, GuildMember* op, BYTE corpsNum);
    // 훈련대(캠프) 설립
    void LogCampCreation(Guild* guild, GuildMember* leader, BYTE corpsNum);
    // 훈련대(캠프) 해체
    void LogCampBreakUp(Guild* guild, GuildMember* op, BYTE corpsNum);

    // 길드 멤버 관련

    // 길드 멤버 가입
    void LogMemberJoin(Guild* guild, DWORD memberGuid, const TCHAR* memberName);
    // 길드 멤버 탈퇴
    void LogMemberLeave(Guild* guild, GuildMember* member);
    // 길드 멤버 추방
    void LogMemberKickOut(Guild* guild, GuildMember* op, GuildMember* member);
    // 길드 멤버 직책 변경
    void LogDutyChange(Guild* guild, GuildMember* member, BYTE prevDuty);
    // 길드 멤버 권한 위임
    void LogDelegation(Guild* guild, GuildMember* member, BYTE right);

    // 길드 속성 관련
    
    // 길드 랭크업
    void LogRankUp(Guild* guild, BYTE prevGrade);
    // UP 적립
    void LogUPDonation(Guild* guild, GuildMember* member, DWORD deltaUP, DWORD totalUP);

    // 길드 간 관계

    // 길드 관계 설정
    void LogGuildRelation(Guild* guild, GUILDGUID target_id, Guild* target, BYTE relation);
    // 길드 관계 해제
    void LogGuildUnrelation(Guild* guild, GUILDGUID target_id, Guild* target, BYTE relation);

private:
    void LogGuildCommon(int logType, Guild* guild);

    GuildData data_;
};

#endif // _GUILD_LOG_H_
