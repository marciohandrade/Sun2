#pragma once

#include <Singleton.h>
#include <MemoryPoolFactory.h>

class CommChatZone;

class PartyFactory : public util::Singleton<PartyFactory>
{
public:
	PartyFactory();
	virtual ~PartyFactory();

	VOID									Release();
	VOID									Init( DWORD dwPartySize );

	CommChatZone*							Alloc( WORD wPartyKey );
	VOID									Free( CommChatZone* pParty);

protected:
	util::CMemoryPoolFactory<CommChatZone>		*m_pPartyPool;
};
