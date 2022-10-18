#pragma once

class PartyState
{
public:
	PartyState() { Init(); }
	virtual ~PartyState() {}

	virtual VOID Init()
	{
		m_GuidanceKey = 0;
		m_PartyKey = 0;
		m_OtherMemberKey = 0;
        invite_party_key_ = 0;
	}

	virtual VOID JoinParty( WORD GuidanceKey, WORD PartyKey )
	{
		m_GuidanceKey = GuidanceKey;
		m_PartyKey = PartyKey;
	}

	inline WORD		GetGuidanceKey()			{ return m_GuidanceKey; }
	inline WORD		GetPartyKey()	const		{ return m_PartyKey; }

	inline bool		IsGuidanceMember()			{ return m_GuidanceKey > 0 ? TRUE:FALSE; }
	inline bool		IsPartyMember()	const		{ return m_PartyKey > 0 ? TRUE:FALSE; }

	inline DWORD	GetOtherMemberKey()			{ return m_OtherMemberKey; }
	inline VOID		SetOtherMemberKey( DWORD OtherMemberKey )	{ m_OtherMemberKey = OtherMemberKey; }

	inline bool		operator == ( const PartyState& rhs  ) const
	{ 
		if( IsPartyMember() && rhs.IsPartyMember() )	
			return m_PartyKey == rhs.GetPartyKey();

		return false;
	}

	inline bool		operator != ( const PartyState& rhs  ) const
	{ 
		if( IsPartyMember() && rhs.IsPartyMember() )	
			return m_PartyKey != rhs.GetPartyKey();

		return true;
	}

    uint get_invite_party_key() const
    {
        return invite_party_key_;
    }

    void set_invite_party_key(uint key)
    {
        invite_party_key_ = key;
    }

private:
	WORD			m_GuidanceKey;
	WORD			m_PartyKey;
	DWORD			m_OtherMemberKey;	// 파티 참가, 초대를 요청한 상대방 MemberKey
    uint            invite_party_key_;
};













