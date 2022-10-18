#include "StdAfx.h"
#include ".\UnitListOfGroup.h"
#include ".\NPC.h"

__IMPL_CUSTOMPOOL_PTR( UnitListOfGroup )
UnitListOfGroup::UnitListOfGroup(void)
{
}

UnitListOfGroup::~UnitListOfGroup(void)
{
}

VOID UnitListOfGroup::SendAIMessage( AI_MSG *pMsg, WORD wSize, DWORD dwDelay, DWORD dwExceptObjKey )
{
	UNIT_MEMBER_HASH::const_iterator it = m_Hash.begin(), end = m_Hash.end();
	while( it != end )
	{
		if( it->second->GetObjectKey() != dwExceptObjKey )
			it->second->SendAIMessage( pMsg, wSize, dwDelay );
		++it;
	}
}