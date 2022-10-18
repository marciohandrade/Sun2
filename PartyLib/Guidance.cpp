#include "StdAfx.h"
#include "Guidance.h"
#include "BaseParty.h"

/*
// (CHANGES) (100220) (WAVERIX) remove unused section

Guidance::Guidance()
{
	Initialize( MAX_GUIDANCE_PARTY_NUM );

	for( int i = 0; i < MAX_GUIDANCE_PARTY_NUM; ++i )
	{
		CreateParty( ePARTY_NORMAL, i+1, 0 );
	}
}

Guidance::~Guidance()
{
}

VOID Guidance::Init( WORD GuidanceKey, DWORD MasterKey )
{
	m_GuidanceKey = GuidanceKey;
	m_MasterKey = MasterKey;
	m_MasterPartyKey = 1;

	// 파티정보를 초기화한다.
	for( PARTY_MAP::iterator it = m_mapParties.begin(); it != m_mapParties.end(); ++it )
	{
		BaseParty *pBaseParty = it->second;
		pBaseParty->Init( pBaseParty->GetPartyKey(), 0 );
	}
}

VOID Guidance::Release()
{
	m_GuidanceKey = 0;
	m_MasterKey = 0;
	m_MasterPartyKey = 0;
}

RC::ePARTY_RESULT Guidance::ChangeGuidanceMaster( WORD PartyKey, DWORD NextMasterKey )
{
	if( m_MasterKey == NextMasterKey )	
		return RC::RC_PARTY_CANNOT_CHANGE_MASTER;

	if( !GetMemberInfo( PartyKey, NextMasterKey ) )
		return RC::RC_PARTY_ISNOT_MEMBEROFPARTY;

	// 가이던스장은 소속파티의 파티장이어야 한다.
	BasePartyManager::ChangeMaster( PartyKey, NextMasterKey );

	m_MasterKey = NextMasterKey;
	m_MasterPartyKey = PartyKey;

	return RC::RC_PARTY_SUCCESS;
}

BOOL Guidance::SetMemberInfo( WORD PartyKey, DWORD MemberKey, _PARTY_MEMBER_INFO & MemberInfo )
{
	BaseParty * pBaseParty = FindEditableParty( PartyKey );
	if( !pBaseParty )	return FALSE;

	return pBaseParty->SetMemberInfo( MemberKey, MemberInfo );
}

VOID Guidance::GetMemberInfoAll( GUIDANCE_MEMBER_TOTAL_INFO & MemberInfoAll )
{
	MemberInfoAll.m_Count = 0;

	const _PARTY_MEMBER_INFO * pMember = NULL;

	for( PARTY_MAP::iterator it = m_mapParties.begin(); it != m_mapParties.end(); ++it )
	{
		BaseParty *pBaseParty = it->second;

		if( MemberInfoAll.m_Count + pBaseParty->GetMemberNum() > MAX_GUIDANCE_MEMBER_NUM )	break;

		MemberInfoAll.m_Count += (BYTE)pBaseParty->GetMemberInfoAll( &MemberInfoAll.m_Slot[MemberInfoAll.m_Count] );
	}
}

*/
