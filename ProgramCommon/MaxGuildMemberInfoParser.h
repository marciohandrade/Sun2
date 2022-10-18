#pragma once

#include <Singleton.h>
#include <SolarFileReader.h>
#include <ScriptCode.IParser.h>

#define MAX_GUILD_LEVEL		7

struct MaxGuildMemberInfo
{
	TCHAR			m_szGuildDutyName[MAX_CHARNAME_LENGTH];
	eGUILD_DUTY		m_GuildDuty;
	BYTE			m_byBaseCorpsMember[MAX_GUILD_LEVEL]; //��巹���� ���� ���� �ִ� ��� ��
	BYTE			m_byNormalCorpsMember;  //���� �ִ� ��� ��
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
	BYTE					GetMaxMemberByCorps( BYTE Grade, BYTE byCorpsNum );								// ���ܺ� �ִ� �ο���
	BYTE					GetMaxCorpsMemberByDuty( BYTE Grade, BYTE byCorpsNum, eGUILD_DUTY Duty );		// �ش� ������ ��å�� �ִ� �ο���.

public:
	virtual	BOOL			LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload );

private:
	BOOL					_Load( BOOL bReload );

private:
	MAX_GUILDMEMBER_INFO_MAP		m_mapMaxGuildMemberInfo;
};

