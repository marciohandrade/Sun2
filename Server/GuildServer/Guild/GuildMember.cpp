#include "StdAfx.h"
#include ".\guildmember.h"
#include <Struct.h>
#include <GuildRightInfoParser.h>

__IMPL_GUILDPOOL(GuildMember)
GuildMember::GuildMember(void)
{
}

GuildMember::~GuildMember(void)
{
}

VOID GuildMember::InitializeMember()
{
	ZeroMemory( &m_MemberInfo, sizeof(m_MemberInfo) );
	InitDelegateRight();
	m_byTotalInfoReqCnt = 0;
}

VOID GuildMember::ReleaseMember()
{

}

VOID GuildMember::Join( CHARGUID guid, const TCHAR* tszCharName, BYTE byClass, LEVELTYPE CharLV )
{
	SetCharGuid( guid );
	SetCharName( tszCharName );
	SetClass( byClass );
	SetLevel( CharLV );
	SetLogin( TRUE );
	SetDuty(eGUILD_DUTY_REGULAR_SOLDIER);
	SetCorpsNumber(0);
	SetChaoState(eCHAO_STATE_NORMAL);						// 카오인 놈이 길드 가입이 가능한가?
}


VOID GuildMember::SerializeBaseMemberInfo( INOUT BASE_GUILD_MEMBER_INFO& rInfo, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		CopyMemory( &rInfo, &m_MemberInfo, sizeof(rInfo) );
	}
	else
	{
		// 조건체크..
		if( rInfo.m_eGuildDuty == eGUILD_DUTY_NONE || rInfo.m_eGuildDuty >= eGUILD_DUTY_MAX )
			SUNLOG( eCRITICAL_LOG, "[GuildMember::Serialize] Invalid Value, CharName = %s, Duty = %d", rInfo.m_tszCharName, rInfo.m_eGuildDuty );

		CopyMemory( &m_MemberInfo, &rInfo, sizeof(m_MemberInfo) );
	}
}

VOID GuildMember::SerializeMemberInfo( INOUT GUILD_MEMBER_INFO&  rInfo, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		CopyMemory( &rInfo, &m_MemberInfo, sizeof(rInfo) );
	}
	else
	{
		// 조건체크..
		if( rInfo.m_eGuildDuty == eGUILD_DUTY_NONE || rInfo.m_eGuildDuty >= eGUILD_DUTY_MAX )
			SUNLOG( eCRITICAL_LOG, "[GuildMember::Serialize] Invalid Value, CharName = %s, Duty = %d", rInfo.m_tszCharName, rInfo.m_eGuildDuty );

		CopyMemory( &m_MemberInfo, &rInfo, sizeof(m_MemberInfo) );
	}
}

VOID GuildMember::SerializeServerMemberInfo( INOUT SERVER_GUILD_MEMBER_INFO&  rInfo, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		CopyMemory( &rInfo, &m_MemberInfo, sizeof(rInfo) );
	}
	else
	{
		// 조건체크..
		if( rInfo.m_eGuildDuty == eGUILD_DUTY_NONE || rInfo.m_eGuildDuty >= eGUILD_DUTY_MAX )
			SUNLOG( eCRITICAL_LOG, "[GuildMember::Serialize] Invalid Value, CharName = %s, Duty = %d", rInfo.m_tszCharName, rInfo.m_eGuildDuty );

		CopyMemory( &m_MemberInfo, &rInfo, sizeof(m_MemberInfo) );
	}
}

BOOL	GuildMember::CheckDelegateRight( eGUILD_RIGHTS Right )
{
	for( int i = 0 ; i < MAX_GUILD_DELEGATE_RIGHT; i++ )
	{
		if( m_MemberInfo.m_DelegateRight[i].m_Right == Right )
			return m_MemberInfo.m_DelegateRight[i].m_bRightUse;
	}

	return FALSE;
}

VOID	GuildMember::GetDelegateRight( OUT GUILD_RIGHT_USE_INFO* pDelegateRight )
{
	CopyMemory( pDelegateRight, m_MemberInfo.m_DelegateRight, sizeof( GUILD_RIGHT_USE_INFO )*MAX_GUILD_DELEGATE_RIGHT );
}

VOID	GuildMember::SetDelegateRight( GUILD_RIGHT_USE_INFO* pDelegateRight )
{
	if( !pDelegateRight )		return;

	CopyMemory( m_MemberInfo.m_DelegateRight, pDelegateRight, sizeof( GUILD_RIGHT_USE_INFO )*MAX_GUILD_DELEGATE_RIGHT );
}

VOID	GuildMember::InitDelegateRight()
{
	ZeroMemory( m_MemberInfo.m_DelegateRight, sizeof(m_MemberInfo.m_DelegateRight) );
	m_MemberInfo.m_DelegateRight[0].m_Right = eGUILD_RIGHTS_RELATION;
	m_MemberInfo.m_DelegateRight[1].m_Right = eGUILD_RIGHTS_RANKUP;
	m_MemberInfo.m_DelegateRight[2].m_Right = eGUILD_RIGHTS_UP2GP;
	m_MemberInfo.m_DelegateRight[3].m_Right = eGUILD_RIGHTS_USE_UPGP;
	m_MemberInfo.m_DelegateRight[4].m_Right = eGUILD_RIGHTS_SEIGE_WARFARE;
	m_MemberInfo.m_DelegateRight[5].m_Right = eGUILD_RIGHTS_GUILD_MARK;
}

VOID	GuildMember::SetCharName( const TCHAR* pszCharName )
{
	strncpy( m_MemberInfo.m_tszCharName, pszCharName, MAX_CHARNAME_LENGTH );
}

VOID	GuildMember::SetLogin( BOOL bLogin )
{ 
	m_MemberInfo.m_bLogin = bLogin;
}

