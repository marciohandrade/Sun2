#include "StdAfx.h"
#include ".\GameGuildMember.h"

__IMPL_GUILDPOOL(GameGuildMember)
GameGuildMember::GameGuildMember(void) : option_applied_flag_(FALSE)
{
}

GameGuildMember::~GameGuildMember(void)
{
}

VOID GameGuildMember::Init( const BASE_GUILD_MEMBER_INFO& rInfo )
{
    SetFlag_OptionActive(FALSE);
	memcpy( &m_Info, &rInfo, sizeof(m_Info) );
}

void GameGuildMember::SetCharName( const char* char_name )
{
    strncpy(m_Info.m_tszCharName, char_name, MAX_CHARNAME_LENGTH+1);
    m_Info.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
}

VOID GameGuildMember::Release()
{
    ZeroMemory(&m_Info, sizeof(BASE_GUILD_MEMBER_INFO));
    SetFlag_OptionActive(FALSE);
}


