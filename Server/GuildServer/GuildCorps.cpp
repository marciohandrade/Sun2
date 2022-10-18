#include "StdAfx.h"
#include ".\guildcorps.h"

GuildCorps::GuildCorps(void)
{
	m_dwGuid = 0;
	m_byNumber = 0;
	m_bCreateRearCamp = FALSE;
}

GuildCorps::~GuildCorps(void)
{
	Destroy();
}

VOID		GuildCorps::Destroy()
{
	if( m_mapMember.size() == 0 )
		return;

	GUILD_CORPS_MEMBER_MAP_ITER iter;
	for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
	{
		GuildMember* pMember = iter->second;
		if( pMember )
		{
			pMember->ReleaseMember();
			GuildMember::FREE( pMember );
		}
	}

	m_mapMember.clear();
	m_mapMemberStr.clear();
}

VOID		GuildCorps::SerializeBaseMemberTotalInfo( INOUT BASE_GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		GUILD_CORPS_MEMBER_MAP_ITER iter;
		for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
		{
			GuildMember* pMember = iter->second;
			if( pMember )
			{
				pMember->SerializeBaseMemberInfo( info.m_MemberInfo[info.m_byMemberCount], se );
				info.m_byMemberCount++;
			}
		}
	}
	else
	{
		for( int i = 0 ; i < info.m_byMemberCount; i++ )
		{
			if( info.m_MemberInfo[i].m_byCorpsNum != GetNumber() )
				 continue;

			GuildMember* pMember = FindMember( info.m_MemberInfo[i].m_CharGuid );
			if( !pMember )
			{
				GuildMember* pNewMember = GuildMember::ALLOC();
				pNewMember->InitializeMember();
				pNewMember->SerializeBaseMemberInfo( info.m_MemberInfo[i], se );
				InsertMember( pNewMember );
			}
			else
				pMember->SerializeBaseMemberInfo( info.m_MemberInfo[i], se );
		}
	}
}


VOID		GuildCorps::SerializeMemberTotalInfo( INOUT GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		GUILD_CORPS_MEMBER_MAP_ITER iter;
		for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
		{
			GuildMember* pMember = iter->second;
			if( pMember )
			{
				pMember->SerializeMemberInfo( info.m_MemberInfo[info.m_byMemberCount], se );
				info.m_byMemberCount++;
			}
		}
	}
	else
	{
		for( int i = 0 ; i < info.m_byMemberCount; i++ )
		{
			if( info.m_MemberInfo[i].m_byCorpsNum != GetNumber() )
				 continue;

			GuildMember* pMember = FindMember( info.m_MemberInfo[i].m_CharGuid );
			if( !pMember )
			{
				GuildMember* pNewMember = GuildMember::ALLOC();
				pNewMember->InitializeMember();
				pNewMember->SerializeMemberInfo( info.m_MemberInfo[i], se );
				InsertMember( pNewMember );
			}
			else
				pMember->SerializeMemberInfo( info.m_MemberInfo[i], se );
		}
	}
}

VOID		GuildCorps::SerializeServerMemberTotalInfo( INOUT SERVER_GUILD_MEMBER_TOTAL_INFO& info, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		GUILD_CORPS_MEMBER_MAP_ITER iter;
		for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
		{
			GuildMember* pMember = iter->second;
			if( pMember )
			{
				pMember->SerializeServerMemberInfo( info.m_MemberInfo[info.m_byMemberCount], se );
				info.m_byMemberCount++;
			}
		}
	}
	else
	{
		for( int i = 0 ; i < info.m_byMemberCount; i++ )
		{
			if( info.m_MemberInfo[i].m_byCorpsNum != GetNumber() )
				 continue;

			GuildMember* pMember = FindMember( info.m_MemberInfo[i].m_CharGuid );
			if( !pMember )
			{
				GuildMember* pNewMember = GuildMember::ALLOC();
				pNewMember->InitializeMember();
				pNewMember->SerializeServerMemberInfo( info.m_MemberInfo[i], se );
				InsertMember( pNewMember );
			}
			else
				pMember->SerializeServerMemberInfo( info.m_MemberInfo[i], se );
		}
	}

}

GuildMember*	GuildCorps::CreateMember( CHARGUID CharGuid, const TCHAR* tszCharName, eGUILD_DUTY Duty, BYTE byClass, LEVELTYPE Level, BYTE byChaoState, BOOL bLogin )
{
	GuildMember* pExistMember = FindMember( CharGuid );
	if( pExistMember )		return NULL;

	GuildMember* pMember = GuildMember::ALLOC();
	pMember->InitializeMember();
	pMember->SetCharGuid( CharGuid );
	pMember->SetCharName( tszCharName );
	pMember->SetDuty( Duty );
	pMember->SetCorpsNumber( GetNumber() );
	pMember->SetClass( byClass );
	pMember->SetLevel( Level );
	pMember->SetChaoState( byChaoState );
	pMember->SetLogin( bLogin );

	if( !InsertMember( pMember ) )
	{
		pMember->ReleaseMember();
		GuildMember::FREE( pMember );
		return NULL;
	}
	
	return pMember;
}

// 중요!!!!!!!!!!!!!!!!
// 이함수는 절대로 멤버컨테이너를 루프 돌면서 호출하지 말것.
// 내부적으로 DeleteMember함수가 멤버컨테이너에서 해당 멤버를 삭제 시킴.
VOID		GuildCorps::DestroyMember( CHARGUID CharGuid )
{
	GuildMember* pMember = FindMember( CharGuid );
	if( !pMember )		return;

	pMember->ReleaseMember();
	DeleteMember( pMember->GetCharGuid() );
	GuildMember::FREE( pMember );
}

//기존의 길드 멤버를 군단에 가입 시킨다.
BOOL		GuildCorps::InsertMember( GuildMember* pMember )
{
	if( !pMember )		return FALSE;

	GuildMember* pExistMember = FindMember( pMember->GetCharGuid() );
	if( pExistMember )		return FALSE;

	GuildMember* pStrMember = FindMember( pMember->GetCharName() );
	if( pStrMember )		return FALSE;

	m_mapMember.insert( std::make_pair( pMember->GetCharGuid(), pMember ) );
	m_mapMemberStr.insert( std::make_pair( pMember->GetCharName(), pMember ) );

	return TRUE;
}

//단순히 군단에서 제외시킨다.
// 중요!!!!!!!!!!!!!!!!
// 이함수는 절대로 멤버컨테이너를 루프 돌면서 호출하지 말것.
// 내부적으로 DeleteMember함수가 멤버컨테이너에서 해당 멤버를 삭제 시킴.
VOID		GuildCorps::DeleteMember( CHARGUID CharGuid )
{
	GuildMember* pMember = FindMember( CharGuid );
	if( !pMember )		return;

	GUILD_CORPS_MEMBER_MAP_ITER iter;
	iter = m_mapMember.find( pMember->GetCharGuid() );
	if( iter != m_mapMember.end() )
		m_mapMember.erase( iter );

	GUILD_CORPS_MEMBER_STR_MAP_ITER iterStr;
	iterStr = m_mapMemberStr.find( pMember->GetCharName() );
	if( iterStr != m_mapMemberStr.end() )
		m_mapMemberStr.erase( iterStr );

}

GuildMember*	GuildCorps::FindMember( CHARGUID CharGuid )
{
	GUILD_CORPS_MEMBER_MAP_ITER iter;
	iter = m_mapMember.find( CharGuid );
	if( iter != m_mapMember.end() )
	{
		GuildMember* pMember = iter->second;
		if( pMember )
			return pMember;
	}

	return NULL;
}

GuildMember*	GuildCorps::FindMember( const CHAR* pszName )
{
	GUILD_CORPS_MEMBER_STR_MAP_ITER iter;
	iter = m_mapMemberStr.find( pszName );
	if( iter != m_mapMemberStr.end() )
	{
		GuildMember* pMember = iter->second;
		if( pMember )
			return pMember;
	}

	return NULL;
}

GuildMember*	GuildCorps::FindCommander()
{
	GUILD_CORPS_MEMBER_MAP_ITER iter;
	for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
	{
		GuildMember* pMember = iter->second;
		if( !pMember )		continue;

		//직할군단이면 길드장이 군단장 이다.
		if( GetNumber() == 0 )
		{
			if( pMember->GetDuty() == eGUILD_DUTY_MASTER )
				return pMember;
		}
		else
		{
			if( pMember->GetDuty() == eGUILD_DUTY_COMMANDER )
				return pMember;
		}
	}

	return NULL;
}

GuildMember*	GuildCorps::FindMemberByDuty(const eGUILD_DUTY eDuty)
{
	GUILD_CORPS_MEMBER_MAP_ITER iter;
	for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
	{
		GuildMember* pMember = iter->second;
		if(pMember && eDuty == pMember->GetDuty())
			return pMember;
	}

	return NULL;
}

GuildMember*	GuildCorps::FindTrainingLeader()
{
	GUILD_CORPS_MEMBER_MAP_ITER iter;
	for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
	{
		GuildMember* pMember = iter->second;
		if( !pMember )		continue;

		// 직할 군단은 훈련대장이 없다.
		if( GetNumber() > 0 )
		{
			if( pMember->GetDuty() == eGUILD_DUTY_CAMP_LEADER )
				return pMember;
		}
	}

	return NULL;
}

BOOL			GuildCorps::IsExistMemberDuty( eGUILD_DUTY eDuty )
{
	GUILD_CORPS_MEMBER_MAP_ITER iter;
	for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
	{
		GuildMember* pMember = iter->second;
		if( !pMember )		continue;

		if( pMember->GetDuty() == eDuty )
			return TRUE;
	}

	return FALSE;
}

//직책별 멤버들의 수
BYTE			GuildCorps::GetMemberCountByDuty( eGUILD_DUTY eDuty )
{
	BYTE byCount = 0;

	GUILD_CORPS_MEMBER_MAP_ITER iter;
	for( iter = m_mapMember.begin(); iter != m_mapMember.end(); ++iter )
	{
		GuildMember* pMember = iter->second;
		if( !pMember )		continue;

		if( pMember->GetDuty() == eDuty )
			byCount++;
	}

	return byCount;
}


