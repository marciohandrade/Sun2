#pragma once

#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

#define MAX_GUILD_LEVEL		7

struct MaxGuildMemberInfo
{
	TCHAR			m_szGuildDutyName[MAX_CHARNAME_LENGTH];
	eGUILD_DUTY		m_GuildDuty;
	BYTE			m_byBaseCorpsMember[MAX_GUILD_LEVEL]; //길드레벨별 직할 군단 최대 멤버 수
	BYTE			m_byNormalCorpsMember;  //군단 최대 멤버 수
};

typedef		std::map< eGUILD_DUTY, MaxGuildMemberInfo* >		MAX_GUILDMEMBER_INFO_MAP;
typedef		MAX_GUILDMEMBER_INFO_MAP::iterator					MAX_GUILDMEMBER_INFO_MAP_ITER;


class MaxGuildMemberInfoParser : public util::Singleton<MaxGuildMemberInfoParser>, public IParser
{
public:
	MaxGuildMemberInfoParser(void);
	~MaxGuildMemberInfoParser(void);

public:
	VOID					Init( char * pszPackFileName );
	VOID					Release();
	MaxGuildMemberInfo*		FindInfo( eGUILD_DUTY Duty );
	BYTE					GetMaxMemberByCorps( BYTE Grade, BYTE byCorpsNum );								// 군단별 최대 인원수
	BYTE					GetMaxCorpsMemberByDuty( BYTE Grade, BYTE byCorpsNum, eGUILD_DUTY Duty );		// 해당 군단의 직책별 최대 인원수.

public:
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL					_Load( BOOL bReload );

private:
	MAX_GUILDMEMBER_INFO_MAP		m_mapMaxGuildMemberInfo;
};

