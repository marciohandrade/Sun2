#include "stdafx.h"

#include "GuildData.h"

GuildData::~GuildData()
{
}

VOID GuildData::Init()
{
	BaseLogData::Init();

	version_ = (WORD)GUILDDATA_LAST_VERSION;

	LOG_INIT_BINARY(GuildGuid, 0)
	LOG_INIT_STRING(GuildName)
	LOG_INIT_BINARY(MasterCharGuid, 0)
	LOG_INIT_STRING(MasterCharName)
	LOG_INIT_BINARY(MemberCharGuid, 0)
	LOG_INIT_STRING(MemberCharName)
	LOG_INIT_BINARY(TargetCharGuid, 0)
	LOG_INIT_STRING(TargetCharName)
	LOG_INIT_BINARY(MemberRight, 0)
	LOG_INIT_BINARY(PrevMemberDuty, 0)
	LOG_INIT_BINARY(MemberDuty, 0)
	LOG_INIT_BINARY(PrevGuildGrade, 0)
	LOG_INIT_BINARY(GuildGrade, 0)
	LOG_INIT_BINARY(CorpsNum, 0)
	LOG_INIT_BINARY(DeltaUP, 0)
	LOG_INIT_BINARY(TotalUP, 0)
	LOG_INIT_BINARY(TotalGP, 0)
	LOG_INIT_BINARY(Relation, 0)
	LOG_INIT_BINARY(TargetGuildGuid, 0)
	LOG_INIT_STRING(TargetGuildName)
#ifdef _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
    LOG_INIT_BINARY(DefaultColumn1, 0)
    LOG_INIT_BINARY(DefaultColumn2, 0)
    LOG_INIT_STRING(DefaultColumn3)
    LOG_INIT_STRING(DefaultColumn4)
#endif //_NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
}

VOID GuildData::GetBodyDataText(TCHAR* pszBodyData, int nSize)
{
	if (!pszBodyData) { return; }

	_sntprintf( pszBodyData, nSize - 1, ",%u,%d,%s,%d,%s,%d,%u,%u" 
		, version_
		, propGuildGuid_
		, propGuildName_
		, propMasterCharGuid_
		, propMasterCharName_
		, propGuildGrade_
		, propTotalUP_
		, propTotalGP_
		);
	pszBodyData[nSize] = '\0';
}

VOID GuildData::Serialize(CLogArchive& rArchive, BOOL bAddText)
{
	BaseLogData::Serialize(rArchive, bAddText);

	rArchive << version_;
	//바이너리
	if (version_ >= 1)
	{
		LOG_WRITE_STATIC_BINARY(GuildGuid, rArchive)
		LOG_WRITE_STRING(GuildName, rArchive)
		LOG_WRITE_BINARY(MasterCharGuid, rArchive)
		LOG_WRITE_STRING(MasterCharName, rArchive)
		LOG_WRITE_BINARY(MemberCharGuid, rArchive)
		LOG_WRITE_STRING(MemberCharName, rArchive)
		LOG_WRITE_BINARY(TargetCharGuid, rArchive)
		LOG_WRITE_STRING(TargetCharName, rArchive)
		LOG_WRITE_BINARY(MemberRight, rArchive)
		LOG_WRITE_BINARY(PrevMemberDuty, rArchive)
		LOG_WRITE_BINARY(MemberDuty, rArchive)
		LOG_WRITE_BINARY(PrevGuildGrade, rArchive)
		LOG_WRITE_BINARY(GuildGrade, rArchive)
		LOG_WRITE_BINARY(CorpsNum, rArchive)
		LOG_WRITE_BINARY(DeltaUP, rArchive)
		LOG_WRITE_BINARY(TotalUP, rArchive)
		LOG_WRITE_BINARY(TotalGP, rArchive)
		LOG_WRITE_BINARY(Relation, rArchive)
		LOG_WRITE_BINARY(TargetGuildGuid, rArchive)
		LOG_WRITE_STRING(TargetGuildName, rArchive)
#ifdef _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
        LOG_WRITE_BINARY(DefaultColumn1, rArchive)
        LOG_WRITE_BINARY(DefaultColumn2, rArchive)
        LOG_WRITE_STRING(DefaultColumn3, rArchive)
        LOG_WRITE_STRING(DefaultColumn4, rArchive)
#endif //_NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
	}

	//텍스트 추가. 스냅샷은 택스트 로그 안 남긴다.
	if( bAddText && ( m_LogType < SNAPSHOT_REGULAR || m_LogType > SNAPSHOT_MAX ) )
	{		
		GetBodyDataText( m_szBodyText, MAX_TEXT_SIZE * sizeof(TCHAR)  );
		rArchive.WriteText(m_szBodyText, (MAX_TEXT_SIZE + 1) * sizeof(TCHAR) );
	}
}

VOID GuildData::Read(CLogArchive& rArchive)
{
	BaseLogData::Read(rArchive);

	rArchive >> version_;
	//바이너리
	if (version_ >= 1)
	{
		LOG_READ_BINARY(GuildGuid, rArchive)
		LOG_READ_STRING(GuildName, rArchive)
		LOG_READ_BINARY(MasterCharGuid, rArchive)
		LOG_READ_STRING(MasterCharName, rArchive)
		LOG_READ_BINARY(MemberCharGuid, rArchive)
		LOG_READ_STRING(MemberCharName, rArchive)
		LOG_READ_BINARY(TargetCharGuid, rArchive)
		LOG_READ_STRING(TargetCharName, rArchive)
		LOG_READ_BINARY(MemberRight, rArchive)
		LOG_READ_BINARY(PrevMemberDuty, rArchive)
		LOG_READ_BINARY(MemberDuty, rArchive)
		LOG_READ_BINARY(PrevGuildGrade, rArchive)
		LOG_READ_BINARY(GuildGrade, rArchive)
		LOG_READ_BINARY(CorpsNum, rArchive)
		LOG_READ_BINARY(DeltaUP, rArchive)
		LOG_READ_BINARY(TotalUP, rArchive)
		LOG_READ_BINARY(TotalGP, rArchive)
		LOG_READ_BINARY(Relation, rArchive)
		LOG_READ_BINARY(TargetGuildGuid, rArchive)
		LOG_READ_STRING(TargetGuildName, rArchive)
#ifdef _NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
        LOG_READ_BINARY(DefaultColumn1, rArchive)
        LOG_READ_BINARY(DefaultColumn2, rArchive)
        LOG_READ_STRING(DefaultColumn3, rArchive)
        LOG_READ_STRING(DefaultColumn4, rArchive)
#endif //_NA_007487_20140811_GAMELOG_GUILD_DEFAULTLOG
	}
}
