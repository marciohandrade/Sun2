/*
 * GuildData.h
 *
 * 길드 로그 형식을 기존 게임 바이너리 로그 기준에 맞추기 위해 재작성
 */
#ifndef _GUILD_DATA_H_
#define _GUILD_DATA_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "BaseLogData.h"

class DLL_DECLARE GuildData : public BaseLogData
{
public:
	~GuildData();

	VOID Init();

	VOID Serialize(CLogArchive& rArchive, BOOL bAddText = FALSE);
	VOID Read(CLogArchive& rArchive);

	LOG_STATIC_PROP_BINARY(DWORD, GuildGuid)				// 길드 ID
	LOG_PROP_STRING(GuildName, MAX_GUILDNAME_LENGTH)		// 길드명
	LOG_PROP_BINARY(0, DWORD, MasterCharGuid)				// 길드 마스터 캐릭터 ID
	LOG_PROP_STRING(MasterCharName, MAX_CHARNAME_LENGTH)	// 길드 마스터 캐릭터명
	LOG_PROP_BINARY(1, DWORD, MemberCharGuid)				// 멤버 캐릭터 ID
	LOG_PROP_STRING(MemberCharName, MAX_CHARNAME_LENGTH)	// 멤버 캐릭터명
	LOG_PROP_BINARY(2, DWORD, TargetCharGuid)				// 대상 캐릭터 ID
	LOG_PROP_STRING(TargetCharName, MAX_CHARNAME_LENGTH)	// 대상 캐릭터명
	LOG_PROP_BINARY(3, BYTE, MemberRight)					// 길드원 권한
	LOG_PROP_BINARY(4, BYTE, PrevMemberDuty)				// 이전 길드원 직책
	LOG_PROP_BINARY(5, BYTE, MemberDuty)					// 길드원 직책
	LOG_PROP_BINARY(6, BYTE, PrevGuildGrade)				// 이전 길드 등급
	LOG_PROP_BINARY(7, BYTE, GuildGrade)					// 길드 등급
	LOG_PROP_BINARY(8, BYTE, CorpsNum)						// 군단 번호
	LOG_PROP_BINARY(9, DWORD, DeltaUP)						// UP 변동량
	LOG_PROP_BINARY(10, DWORD, TotalUP)						// 총 UP
	LOG_PROP_BINARY(11, DWORD, TotalGP)						// 총 GP
	LOG_PROP_BINARY(12, BYTE, Relation)						// 상대 길드 관계
	LOG_PROP_BINARY(13, DWORD, TargetGuildGuid)				// 상대 길드 ID
	LOG_PROP_STRING(TargetGuildName, MAX_GUILDNAME_LENGTH)	// 상대 길드명

#ifdef _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
    // Switch문, default에서 사용하는 변수
    LOG_PROP_BINARY(14, INT, DefaultColumn1)				
    LOG_PROP_BINARY(15, INT, DefaultColumn2)				
    LOG_PROP_STRING(DefaultColumn3, MAX_GUILDLOG_COLUMNSIZE)	
    LOG_PROP_STRING(DefaultColumn4, MAX_GUILDLOG_COLUMNSIZE)	
#endif //_NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG

	LOG_DYNAMIC_PROP_CTOR(GuildData, LOG_GUILD)

protected:
	VOID GetBodyDataText(TCHAR* pszBodyData, int nSize);

	WORD version_; // 길드 로그 버전
};

#endif // _GUILD_DATA_H_
