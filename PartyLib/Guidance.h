#pragma once

#include "BasePartyManager.h"
#include "BaseParty.h"

/*
// (CHANGES) (100220) (WAVERIX) remove unused section
class Guidance : public BasePartyManager
{
public:
	Guidance();
	virtual ~Guidance();

	virtual	VOID				Init( WORD GuidanceKey, DWORD MasterKey );
	virtual VOID				Release();

	virtual RC::ePARTY_RESULT	ChangeGuidanceMaster( WORD PartyKey, DWORD NextMasterKey );

	virtual eGUIDANCE_TYPE		GetType()				{ return eGUIDANCE_NORMAL; }
	inline WORD					GetGuidanceKey()		{ return m_GuidanceKey; }

	inline DWORD				GetMasterKey()			{ return m_MasterKey; }
	inline WORD					GetMasterPartyKey()		{ return m_MasterPartyKey; }

	VOID						GetMemberInfoAll( GUIDANCE_MEMBER_TOTAL_INFO & MemberInfoAll );
	BOOL						SetMemberInfo( WORD PartyKey, DWORD MemberKey, _PARTY_MEMBER_INFO & MemberInfo );
	
	template< class Operator >	VOID For_eachMembers( Operator & Opr );

private:
	WORD						m_GuidanceKey;
	DWORD						m_MasterKey;
	WORD						m_MasterPartyKey;
};

template< class Operator >
VOID Guidance::For_eachMembers( Operator & Opr )
{
	for( PARTY_MAP::iterator it = m_mapParties.begin(); it != m_mapParties.end(); it++ )
	{
		BaseParty *pBaseParty = it->second;
		pBaseParty->For_eachMembers( Opr );
	}
}

*/
