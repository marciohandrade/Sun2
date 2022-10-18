#include "StdAfx.h"
#include ".\WorldGuild.h"
#include ".\WorldGuildMember.h"
#include <Struct.h>

// SendAll()때문
#include "User.h"
#include "UserManager.h"


__IMPL_GUILDPOOL(WorldGuild)
WorldGuild::WorldGuild(void)
{
	m_MemberHash.Initialize(_MAX_GUILDMEMBER_NUM);
	ZeroMemory(&m_Info, sizeof(WORLD_GUILD_INFO));
}

WorldGuild::~WorldGuild(void)
{
//	Destroy();
}


VOID WorldGuild::Destroy()
{
	for( GUILDMEMBER_HASH::iterator it = m_MemberHash.begin() ; it != m_MemberHash.end() ; ++it )
	{
		(*it)->Release();
		WorldGuildMember::FREE((*it));
	}
	m_MemberHash.RemoveAll();
}

VOID WorldGuild::Withdraw( CHARGUID CharGuid )
{
	WorldGuildMember * pMember = FindMember(CharGuid);
	if( !pMember )
	{
		SUNLOG( eCRITICAL_LOG, "[WorldGuild::Withdraw] Error - No Guild Member!!![CharGuid:%u]", CharGuid );
		return;
	}
	
	RemoveMember( CharGuid );
	WorldGuildMember::FREE(pMember);
}

#ifdef __NA_000000_20070528_TAIWAN_GUILD_PVP
VOID WorldGuild::SetChuntingPVPInfo( CHUNTING_PVP_INFO & IN PVPInfo )
{
	m_Info.m_ChuntingPVPInfo = PVPInfo;
}
#endif

//길드 직책
BOOL WorldGuild::ChangeMemberDuty(CHARGUID TargetCharGuid, eGUILD_DUTY eGuildDuty)
{
	WorldGuildMember *pMember = FindMember(TargetCharGuid);
	if(!pMember)
	{
		SUNLOG( eCRITICAL_LOG, "[WorldGuild::ChangeMemberDuty] GuildMember Not Exist, CharGuid = %d", TargetCharGuid );
		return FALSE;
	}

	pMember->SetDuty(eGuildDuty);

	return TRUE;
}

VOID	WorldGuild::CreateRearCamp( BYTE byCorpsNum )
{
	if( byCorpsNum == 0 || byCorpsNum >= MAX_GUILD_CORPS_NUM )
		return;

	if( m_bRearCamp[byCorpsNum] == TRUE )
	{
		SUNLOG( eCRITICAL_LOG, "[WorldGuild::CreateRearCamp] Already Create RearCamp, CorpsNumber = %d", byCorpsNum );
		return;
	}

	m_bRearCamp[byCorpsNum] = TRUE;
}

VOID	WorldGuild::DestroyRearCamp( BYTE byCorpsNum )
{
	if( byCorpsNum == 0 || byCorpsNum >= MAX_GUILD_CORPS_NUM )
		return;

	if( m_bRearCamp[byCorpsNum] == TRUE )
	{
		SUNLOG( eCRITICAL_LOG, "[WorldGuild::DestroyRearCamp] Already Destroy RearCamp, CorpsNumber = %d", byCorpsNum );
		return;
	}

	m_bRearCamp[byCorpsNum] = FALSE;
}

VOID	WorldGuild::Create( const WORLD_GUILD_INFO& rInfo, const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
	//길드의 생성은 ALLOC으로 MANAGER에서 하고 온다.
	CopyMemory( &m_Info, &rInfo, sizeof(m_Info) );
	CreateMember( rMemberInfo );
}

VOID	WorldGuild::CreateMember( const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
	if( FindMember( rMemberInfo.m_CharGuid ) )
		DestroyMember( rMemberInfo.m_CharGuid );

	WorldGuildMember* pMember = WorldGuildMember::ALLOC();
	pMember->Serialize( const_cast<BASE_GUILD_MEMBER_INFO&>(rMemberInfo), SERIALIZE_STORE ); 
	AddMember( pMember, pMember->GetCharGuid() );
}

VOID	WorldGuild::DestroyMember( const CHARGUID& CharGuid )
{
	WorldGuildMember* pMember = FindMember( CharGuid );
	if( pMember )
	{
		pMember->Release();
		RemoveMember( CharGuid );
		WorldGuildMember::FREE( pMember );
	}
}

VOID WorldGuild::SendAllMember( BYTE* pSendMsg, WORD wSize )
{
	// 현재 길드 멤버이고, 유저 객체가 있을 때만 이 함수가 작동된다.
	// 로그 아웃같은 작업을 할 경우, 남아 있는 길드 멤버들에게 메세지를 전달 하려면
	// 이 함수 호출전에 반드시 길드 멤버에서 객체를 지우거나, 아니면 온라인 상태가 아니라는 것을 세팅해라.

	User* pUser = NULL;
	WorldGuildMember* pMember = NULL;

	for( GUILDMEMBER_HASH::iterator it = m_MemberHash.begin() ; it != m_MemberHash.end() ; ++it )
	{
		pMember = (*it);
		if( pMember )
		{
			// 길드에서 UserManager를 가져다 쓰는 구조는 별로 좋지 않다. 
			// 될수 있으면 연관성을 떨어 뜨려야 한다. 좋은 방법 없을까?
			pUser = UserManager::Instance()->FindUserByCharGuid( pMember->GetCharGuid() );
			if( pUser )
				pUser->SendPacket( pSendMsg, wSize );
		}
	}
}

VOID WorldGuild::SendCorpsMember( BYTE byCorpsNum, BYTE* pSendMsg, WORD wSize )
{
	User* pUser = NULL;
	WorldGuildMember* pMember = NULL;

	for( GUILDMEMBER_HASH::iterator it = m_MemberHash.begin() ; it != m_MemberHash.end() ; ++it )
	{
		pMember = (*it);
		if( pMember && pMember->GetCorpsNum() == byCorpsNum )
		{
			pUser = UserManager::Instance()->FindUserByCharGuid( pMember->GetCharGuid() );
			if( pUser )
				pUser->SendPacket( pSendMsg, wSize );
		}
	}
}

VOID		WorldGuild::SerializeWorldGuildInfo( INOUT WORLD_GUILD_INFO& Info, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		// 현 클래스에서 값을 가져갈때..
		CopyMemory( &Info, &m_Info, sizeof(Info) );
	}
	else
	{
		// 현 클래스에 값을 쓸때..
		CopyMemory( &m_Info, &Info, sizeof(m_Info) );
	}
}

VOID		WorldGuild::SerializeBaseMemberInfo( INOUT BASE_GUILD_MEMBER_INFO& Info, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		//가지고 갈때.. 무조건 CharGuid값은 들어있다고 가정한다.
		WorldGuildMember* pMember = FindMember( Info.m_CharGuid );
		if( pMember )
			pMember->Serialize( Info, se );
	}
	else
	{
		//저장할 때.
		WorldGuildMember* pMember = FindMember( Info.m_CharGuid );
		if( pMember )
			DestroyMember( Info.m_CharGuid );

		CreateMember( Info );
	}
}

//VOID		WorldGuild::SerializeBaseMemberTotalInfo( INOUT BASE_GUILD_MEMBER_TOTAL_INFO& Info, eSERIALIZE se )
//{
//	if( se == SERIALIZE_LOAD )
//	{
//		Info.m_byMemberCount = 0;
//		WorldGuildMember* pMember = NULL;

//		for( GUILDMEMBER_HASH::iterator it = m_MemberHash.begin() ; it != m_MemberHash.end() ; ++it )
//		{
//			pMember = (*it);
//			if( pMember )
//			{
//				pMember->Serialize( Info.m_MemberInfo[Info.m_byMemberCount], se );
//				Info.m_byMemberCount++;
//			}
//		}
//	}
//	else
//	{
//		for( int i = 0 ; i < Info.m_byMemberCount; ++i )
//			CreateMember( Info.m_MemberInfo[i]);
//	}
//}

VOID	 WorldGuild::Join( const BASE_GUILD_MEMBER_INFO& rMemberInfo )
{
	// 내부적으로 길드 멤버 생성과 동일한 로직이다.
	CreateMember( rMemberInfo );
}

const	TCHAR*		WorldGuild::GetGuildNotice()
{
	return m_Info.m_szGuildNotice; 
}

VOID WorldGuild::SetGuildNotice( const util::StrArrayForGuildNotice& ptszNotice )
{
    _tcsncpy(m_Info.m_szGuildNotice, ptszNotice, _countof(m_Info.m_szGuildNotice));
    m_Info.m_szGuildNotice[_countof(m_Info.m_szGuildNotice) - 1] = _T('\0');
}

VOID WorldGuild::ChangeGuildName( const TCHAR* pszGuildName )
{
	memset(m_Info.m_tszGuildName, 0, MAX_GUILDNAME_LENGTH); 
	_tcsncpy(m_Info.m_tszGuildName, pszGuildName, MAX_GUILDNAME_LENGTH);
	m_Info.m_tszGuildName[MAX_GUILDNAME_LENGTH] = '\0'; 
}

