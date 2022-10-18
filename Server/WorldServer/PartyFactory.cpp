#include <stdafx.h>
#include "BaseZone.h"
#include ".\Partyfactory.h"
//#include ".\Party.h"

PartyFactory::PartyFactory()
{
	m_pPartyPool		= new util::CMemoryPoolFactory<CommChatZone>;
}

PartyFactory::~PartyFactory()
{
	delete m_pPartyPool;
}

VOID PartyFactory::Release()
{
	m_pPartyPool->Release();
}

VOID PartyFactory::Init( DWORD dwPartySIze )
{
	m_pPartyPool->Initialize( dwPartySIze, dwPartySIze/2+1 );
}

CommChatZone* PartyFactory::Alloc( WORD wPartyKey )
{
	CommChatZone *pParty = (CommChatZone *)m_pPartyPool->Alloc();

	pParty->Init();
	pParty->SetKey( wPartyKey );

	return pParty;
}

VOID PartyFactory::Free( CommChatZone* pParty )
{
	if( !pParty )		return;

	pParty->Release();
	m_pPartyPool->Free( pParty );
}
