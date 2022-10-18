#include "StdAfx.h"
#include ".\WorldGuildMember.h"

__IMPL_GUILDPOOL(WorldGuildMember)
WorldGuildMember::WorldGuildMember(void)
{
}

WorldGuildMember::~WorldGuildMember(void)
{
}

VOID	WorldGuildMember::Serialize( INOUT BASE_GUILD_MEMBER_INFO& info, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		CopyMemory( &info, &m_Info, sizeof(info) );
	}
	else
	{
		CopyMemory( &m_Info, &info, sizeof(m_Info) );
	}
}

void WorldGuildMember::SetCharName( const char* char_name )
{
    strncpy(m_Info.m_tszCharName, char_name, MAX_CHARNAME_LENGTH+1);
    m_Info.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
}

