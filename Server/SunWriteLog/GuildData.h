/*
 * GuildData.h
 *
 * ��� �α� ������ ���� ���� ���̳ʸ� �α� ���ؿ� ���߱� ���� ���ۼ�
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

	LOG_STATIC_PROP_BINARY(DWORD, GuildGuid)				// ��� ID
	LOG_PROP_STRING(GuildName, MAX_GUILDNAME_LENGTH)		// ����
	LOG_PROP_BINARY(0, DWORD, MasterCharGuid)				// ��� ������ ĳ���� ID
	LOG_PROP_STRING(MasterCharName, MAX_CHARNAME_LENGTH)	// ��� ������ ĳ���͸�
	LOG_PROP_BINARY(1, DWORD, MemberCharGuid)				// ��� ĳ���� ID
	LOG_PROP_STRING(MemberCharName, MAX_CHARNAME_LENGTH)	// ��� ĳ���͸�
	LOG_PROP_BINARY(2, DWORD, TargetCharGuid)				// ��� ĳ���� ID
	LOG_PROP_STRING(TargetCharName, MAX_CHARNAME_LENGTH)	// ��� ĳ���͸�
	LOG_PROP_BINARY(3, BYTE, MemberRight)					// ���� ����
	LOG_PROP_BINARY(4, BYTE, PrevMemberDuty)				// ���� ���� ��å
	LOG_PROP_BINARY(5, BYTE, MemberDuty)					// ���� ��å
	LOG_PROP_BINARY(6, BYTE, PrevGuildGrade)				// ���� ��� ���
	LOG_PROP_BINARY(7, BYTE, GuildGrade)					// ��� ���
	LOG_PROP_BINARY(8, BYTE, CorpsNum)						// ���� ��ȣ
	LOG_PROP_BINARY(9, DWORD, DeltaUP)						// UP ������
	LOG_PROP_BINARY(10, DWORD, TotalUP)						// �� UP
	LOG_PROP_BINARY(11, DWORD, TotalGP)						// �� GP
	LOG_PROP_BINARY(12, BYTE, Relation)						// ��� ��� ����
	LOG_PROP_BINARY(13, DWORD, TargetGuildGuid)				// ��� ��� ID
	LOG_PROP_STRING(TargetGuildName, MAX_GUILDNAME_LENGTH)	// ��� ����

#ifdef _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
    // Switch��, default���� ����ϴ� ����
    LOG_PROP_BINARY(14, INT, DefaultColumn1)				
    LOG_PROP_BINARY(15, INT, DefaultColumn2)				
    LOG_PROP_STRING(DefaultColumn3, MAX_GUILDLOG_COLUMNSIZE)	
    LOG_PROP_STRING(DefaultColumn4, MAX_GUILDLOG_COLUMNSIZE)	
#endif //_NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG

	LOG_DYNAMIC_PROP_CTOR(GuildData, LOG_GUILD)

protected:
	VOID GetBodyDataText(TCHAR* pszBodyData, int nSize);

	WORD version_; // ��� �α� ����
};

#endif // _GUILD_DATA_H_
