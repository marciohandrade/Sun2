#ifndef     __UIPARTYMEMBER_DEFINE_H__
#define     __UIPARTYMEMBER_DEFINE_H__
//------------------------------------------------------------------------------

#define MAX_PARTYMEMDLG_SKILL_EFFECT 10

#define COLORDEF_UIPARTY_MEMBER_AT_TIME_DEAD    RGBA(100,100,100,255)
#define COLORDEF_UIPARTY_MEMBER_AT_TIME_LIVE    RGBA(255,255,255,255)


//------------------------------------------------------------------------------
/**
    파티원 정보
*/
class uicPartyMemberInfo
{
public:
    uicPartyMemberInfo() { this->Clear(); }
    ~uicPartyMemberInfo() {}

    uicPartyMemberInfo(const uicPartyMemberInfo& src);
    uicPartyMemberInfo& operator=(const uicPartyMemberInfo& rhs);
    uicPartyMemberInfo& operator=(const PARTY_MEMBER_INFO& rhs);
    uicPartyMemberInfo& operator=(const PARTY_MEMBER_INFO* rhs);
    inline
        void Clear()
    {
        bFill = false;
    }

    inline
        bool IsFill()
    {
        return bFill;
    }

    inline
        const PARTY_MEMBER_INFO& GetInfo()
    {
        assert (bFill);
        return memInfo;
    }

    inline
        const PARTY_MEMBER_INFO* GetInfoPtr()
    {
        assert (bFill);
        return &memInfo;
    }
private:
	PARTY_MEMBER_INFO   memInfo;
    bool                bFill;
};

//------------------------------------------------------------------------------
/**
*/
inline
uicPartyMemberInfo::uicPartyMemberInfo(const uicPartyMemberInfo& src)
{
    memcpy (static_cast<PARTY_MEMBER_INFO *>(&memInfo),
            static_cast<const PARTY_MEMBER_INFO *>(&src.memInfo),
            sizeof (PARTY_MEMBER_INFO));
    this->bFill = true;
}

//------------------------------------------------------------------------------
/**
*/
inline
uicPartyMemberInfo&
uicPartyMemberInfo::operator=(const uicPartyMemberInfo& rhs)
{
    memcpy (static_cast<PARTY_MEMBER_INFO *>(&memInfo),
            static_cast<const PARTY_MEMBER_INFO *>(&rhs.memInfo),
            sizeof (PARTY_MEMBER_INFO));
    this->bFill = true;

    return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
uicPartyMemberInfo&
uicPartyMemberInfo::operator=(const PARTY_MEMBER_INFO& rhs)
{
	memcpy (static_cast<PARTY_MEMBER_INFO *>(&memInfo),
		static_cast<const PARTY_MEMBER_INFO *>(&rhs),
		sizeof (PARTY_MEMBER_INFO));
	this->bFill = true;
	return *this;
}

//------------------------------------------------------------------------------
/**
*/
inline
uicPartyMemberInfo&
uicPartyMemberInfo::operator=(const PARTY_MEMBER_INFO* rhs)
{

	memcpy (static_cast<PARTY_MEMBER_INFO *>(&memInfo),
		static_cast<const PARTY_MEMBER_INFO *>(rhs),
		sizeof (PARTY_MEMBER_INFO));
	this->bFill = true;
	return *this;
}
//------------------------------------------------------------------------------
#endif  //  __UIPARTYMEMBER_DEFINE_H__
