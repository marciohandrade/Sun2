#include "StdAfx.h"
#include ".\WorldGuild.h"
#include ".\WorldGuildMember.h"
#include <Struct.h>

// SendAll()����
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

//��� ��å
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
	//����� ������ ALLOC���� MANAGER���� �ϰ� �´�.
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
	// ���� ��� ����̰�, ���� ��ü�� ���� ���� �� �Լ��� �۵��ȴ�.
	// �α� �ƿ����� �۾��� �� ���, ���� �ִ� ��� ����鿡�� �޼����� ���� �Ϸ���
	// �� �Լ� ȣ������ �ݵ�� ��� ������� ��ü�� ����ų�, �ƴϸ� �¶��� ���°� �ƴ϶�� ���� �����ض�.

	User* pUser = NULL;
	WorldGuildMember* pMember = NULL;

	for( GUILDMEMBER_HASH::iterator it = m_MemberHash.begin() ; it != m_MemberHash.end() ; ++it )
	{
		pMember = (*it);
		if( pMember )
		{
			// ��忡�� UserManager�� ������ ���� ������ ���� ���� �ʴ�. 
			// �ɼ� ������ �������� ���� �߷��� �Ѵ�. ���� ��� ������?
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
		// �� Ŭ�������� ���� ��������..
		CopyMemory( &Info, &m_Info, sizeof(Info) );
	}
	else
	{
		// �� Ŭ������ ���� ����..
		CopyMemory( &m_Info, &Info, sizeof(m_Info) );
	}
}

VOID		WorldGuild::SerializeBaseMemberInfo( INOUT BASE_GUILD_MEMBER_INFO& Info, eSERIALIZE se )
{
	if( se == SERIALIZE_LOAD )
	{
		//������ ����.. ������ CharGuid���� ����ִٰ� �����Ѵ�.
		WorldGuildMember* pMember = FindMember( Info.m_CharGuid );
		if( pMember )
			pMember->Serialize( Info, se );
	}
	else
	{
		//������ ��.
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
	// ���������� ��� ��� ������ ������ �����̴�.
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

