#include "StdAfx.h"
#include ".\maxguildmemberinfoparser.h"

MaxGuildMemberInfoParser::MaxGuildMemberInfoParser(void)
{
}

MaxGuildMemberInfoParser::~MaxGuildMemberInfoParser(void)
{
}

VOID	MaxGuildMemberInfoParser::Init( char * pszPackFileName )
{
	SetPackFileName( pszPackFileName );
}


VOID	MaxGuildMemberInfoParser::Release()
{
	MAX_GUILDMEMBER_INFO_MAP_ITER	iter;
	for( iter = m_mapMaxGuildMemberInfo.begin(); iter != m_mapMaxGuildMemberInfo.end(); ++iter )
	{
		MaxGuildMemberInfo* pInfo = iter->second;
		if( pInfo )
			delete pInfo;
	}

	m_mapMaxGuildMemberInfo.clear();

}

BOOL	MaxGuildMemberInfoParser::LoadScript( eSCRIPT_CODE ScriptCode, BOOL bReload )
{
	switch( ScriptCode )
	{
	case SCRIPT_MAX_GUILD_MEMBER_INFO:
		return _Load( bReload );
	}

	return FALSE;
}

//군단별 최대 인원수
BYTE	MaxGuildMemberInfoParser::GetMaxMemberByCorps( BYTE Grade, BYTE byCorpsNum )
{
	if( Grade >= MAX_GUILD_LEVEL )
		return 0;

	// 군단 최대개수는 직할 군단 포함이므로 -1 개 한다.( 군단 최대 개수는 직할군단(0) + 일반군단(7) 이다. )
	if( byCorpsNum >= MAX_GUILD_CORPS_NUM - 1 )
		return 0;

	// eGUILD_DUTY_MAX 는 각 군단별 총 합을 가지고 있는 인자다.
	MAX_GUILDMEMBER_INFO_MAP_ITER iter = m_mapMaxGuildMemberInfo.find( eGUILD_DUTY_MAX );
	if( iter == m_mapMaxGuildMemberInfo.end() )
		return 0;

	MaxGuildMemberInfo* pInfo = iter->second;
	if( !pInfo )		return 0;

	//직할군단이면.. 직할군단은 길드 등급별로 인원수가 다르다. 일반 군단은 똑같다.
	if( byCorpsNum == 0 ) 
		return pInfo->m_byBaseCorpsMember[ Grade - 1 ];
	else
		return pInfo->m_byNormalCorpsMember;

    assert(!"impossible reached point");
	//return 0;
}

// 해당 군단의 직책별 최대 인원수.
BYTE	MaxGuildMemberInfoParser::GetMaxCorpsMemberByDuty( BYTE Grade, BYTE byCorpsNum, eGUILD_DUTY Duty )
{
	if( Grade >= MAX_GUILD_LEVEL )
		return 0;

	// 군단 최대개수는 직할 군단 포함이므로 -1 개 한다.( 군단 최대 개수는 직할군단(0) + 일반군단(7) 이다. )
	if( byCorpsNum >= ( MAX_GUILD_CORPS_NUM - 1 ) )
		return 0;

	if( Duty >= eGUILD_DUTY_MAX )
		return 0;

	// eGUILD_DUTY_MAX 는 각 군단별 총 합을 가지고 있는 인자다.
	MAX_GUILDMEMBER_INFO_MAP_ITER iter = m_mapMaxGuildMemberInfo.find( Duty );
	if( iter == m_mapMaxGuildMemberInfo.end() )
		return 0;

	MaxGuildMemberInfo* pInfo = iter->second;
	if( !pInfo )		return 0;

	//직할군단이면.. 직할군단은 길드 등급별로 인원수가 다르다. 일반 군단은 똑같다.
	if( byCorpsNum == 0 ) 
		return pInfo->m_byBaseCorpsMember[ Grade - 1 ];
	else
		return pInfo->m_byNormalCorpsMember;

    assert(!"impossible reached point");
	//return 0;
}


//#pragma warning ( push )
//#pragma warning ( disable : 4244)

BOOL	MaxGuildMemberInfoParser::_Load( BOOL bReload )
{
    __UNUSED(bReload);
	MaxGuildMemberInfo* pSum = new MaxGuildMemberInfo;
	ZeroMemory( pSum, sizeof(pSum) );

	int nRowSize = GetRowSize();

	for( int nRow = 0; nRow < nRowSize; ++nRow )
	{
		MaxGuildMemberInfo* pInfo = new MaxGuildMemberInfo;

		pInfo->m_GuildDuty = (eGUILD_DUTY)GetDataInt( "iIndex", nRow );
		strncpy( pInfo->m_szGuildDutyName, GetDataString( "GuildDutyName", nRow ), MAX_CHARNAME_LENGTH );

		TCHAR szTemp[MAX_FIELD_NAME_SIZE];
		for( int i = 0 ; i < MAX_GUILD_LEVEL; i++ )
		{
			_snprintf( szTemp, MAX_FIELD_NAME_SIZE, "DirectCorps%dNum\0", i+1 );
			pInfo->m_byBaseCorpsMember[i]	= GetDataBYTE( szTemp, nRow );
			pSum->m_byBaseCorpsMember[i] += pInfo->m_byBaseCorpsMember[i];
		}
		pInfo->m_byNormalCorpsMember = GetDataBYTE( "CorpsNum", nRow );
		pSum->m_byNormalCorpsMember += pInfo->m_byNormalCorpsMember;

		if( !m_mapMaxGuildMemberInfo.insert( std::make_pair( pInfo->m_GuildDuty, pInfo ) ).second )
			delete pInfo;
	}

	strncpy( pSum->m_szGuildDutyName, "SUM", MAX_CHARNAME_LENGTH );
	pSum->m_GuildDuty = eGUILD_DUTY_MAX;

	if( !m_mapMaxGuildMemberInfo.insert( std::make_pair( pSum->m_GuildDuty, pSum ) ).second )
		delete pSum;

	return TRUE;
}

//#pragma warning ( pop )

MaxGuildMemberInfo*		MaxGuildMemberInfoParser::FindInfo( eGUILD_DUTY Duty )
{
	MAX_GUILDMEMBER_INFO_MAP_ITER	iter;
	iter = m_mapMaxGuildMemberInfo.find( Duty );
	if( iter != m_mapMaxGuildMemberInfo.end() )
	{
		MaxGuildMemberInfo* pInfo = iter->second;
		if( pInfo )
			return pInfo;
	}

	return NULL;
}
