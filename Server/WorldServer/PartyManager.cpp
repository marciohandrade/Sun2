#include "stdafx.h"
//#include ".\Party.h"'
#include "BaseZone.h"
#include ".\PartyManager.h"
#include ".\PartyFactory.h"
#include "User.h"

PartyManager::PartyManager()
{
	//PartyFactory::Instance()->Init( 9000 );	// 최대 15000명이라고 가정하고 2인당 1파티면 그 반값인 7500...조금 넉넉하게 잡았다
	//m_HashParty.Initialize( 9000 );
	PartyFactory::Instance()->Init( 50 );	// 최대 15000명이라고 가정하고 2인당 1파티면 그 반값인 7500...조금 넉넉하게 잡았다
	m_HashParty.Initialize( 50 );
	m_HashParty.clear();
}

PartyManager::~PartyManager()
{
	CommChatZone *pParty = NULL;
	PARTY_HASH_ITR iter;
	
	for( iter = m_HashParty.begin(); iter != m_HashParty.end(); iter++ )
	{
		pParty = (*iter);
		PartyFactory::Instance()->Free( pParty );
	}

	m_HashParty.clear();
	PartyFactory::Instance()->DestroyInstance();
}

CommChatZone *PartyManager::CreateParty( WORD wPartyKey )
{
	CommChatZone *pParty = PartyFactory::Instance()->Alloc( wPartyKey );

	pParty->SetZoneType( eZONETYPE_MAX );

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    pParty->SetRoomKey(0);
#endif  // NA_004694_20120412_1202_PARTY_ROOM_KEY

    m_HashParty.Add( pParty, wPartyKey );
	MessageOut( eFULL_LOG, "Create Party[P:%u]", wPartyKey );
	return pParty;
}

BOOL PartyManager::JoinParty( WORD wPartyKey, User *pJoinUser )
{
	CommChatZone *pParty = m_HashParty.GetData( wPartyKey );

	// 파티 없으면 복구 관련하여 뭔가 꼬인 것으로 추측된다. 일단 이 경우 파티 복구
	if( !pParty )
	{
		pParty = CreateParty( wPartyKey );
		MessageOut( eCRITICAL_LOG, "Create Party When No JoinParty[P:%u]", wPartyKey );
//		return FALSE;
	}

	pParty->AddUser( pJoinUser->GetCharGuid(), pJoinUser );
	pJoinUser->JoinParty( pParty );

	MessageOut( eFULL_LOG, "[Guid:%u][CharGuid:%u] Join Party[P:%u]", pJoinUser->GetGUID(), pJoinUser->GetCharGuid(), wPartyKey );
	return TRUE;
}

BOOL PartyManager::WithdrawParty( User *pWithdrawUser )
{
	CommChatZone *pParty = pWithdrawUser->GetParty();

	if( !pParty )
	{
		return FALSE;
	}

	pParty->RemoveUser( pWithdrawUser->GetCharGuid() );
	pWithdrawUser->RemoveParty();

	MessageOut( eFULL_LOG, "[Guid:%u][CharGuid:%u] Withdraw Party[P:%u]", pWithdrawUser->GetGUID(), pWithdrawUser->GetCharGuid(), 
																			pParty->GetKey() );

	// 파티원 없으면 강제로 파티 삭제
	if( !pParty->GetNumberOfUsers() )
	{
		MessageOut( eMIDDLE_LOG, "Party Destroy When No Party Member Remain[P:%u]", pParty->GetKey() );
		DestroyParty( static_cast<WORD>(pParty->GetKey()) );
		pParty = NULL;
	}

	return TRUE;
}

VOID PartyManager::DestroyParty( WORD wPartyKey )
{
	CommChatZone *pParty = m_HashParty.GetData( wPartyKey );

	if( !pParty )
	{
		return;
	}

	// 파티원들 전부 즐~
	pParty->Release();
	PartyFactory::Instance()->Free( pParty );
	m_HashParty.Remove( wPartyKey );
	MessageOut( eFULL_LOG, "Destroy Party[P:%u]", wPartyKey );
}

VOID PartyManager::RestoreParty( WORD wPartyKey, User *pUser )
{
	CommChatZone *pParty = m_HashParty.GetData( wPartyKey );

	if( !pParty )
	{
		pParty = CreateParty( wPartyKey );
	}

	pParty->AddUser( pUser->GetCharGuid(), pUser );
	pUser->JoinParty( pParty );

	MessageOut( eFULL_LOG, "[Guid:%u][CharGuid:%u] Restore Party[P:%u]", pUser->GetGUID(), pUser->GetCharGuid(), wPartyKey );
}

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
VOID PartyManager::SetRoomKey( const WORD party_key, const WORD room_key)
{
    CommChatZone* party = m_HashParty.GetData(party_key);

    if (party != NULL)
    {
        party->SetRoomKey(room_key);
    }
}
#endif  // NA_004694_20120412_1202_PARTY_ROOM_KEY
