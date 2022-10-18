#ifndef     __UIPARTYLEADER_DEFINE_H__
#define     __UIPARTYLEADER_DEFINE_H__
//------------------------------------------------------------------------------
struct MSG_CG_PARTY_MEMBER_INFO_CMD;

#define MAX_PARTYDLG_SKILL_EFFECT 10

#define COLORDEF_UIPARTY_LEADER_AT_TIME_DEAD    RGBA(100,100,100,255)
#define COLORDEF_UIPARTY_LEADER_AT_TIME_LIVE    RGBA(255,255,255,255)

//------------------------------------------------------------------------------
/**
    파티리더 정보
*/
class uicPartyLeaderInfo
{
public:
    uicPartyLeaderInfo() { this->Clear(); }
    ~uicPartyLeaderInfo() {}

    uicPartyLeaderInfo(const uicPartyLeaderInfo& src);
    uicPartyLeaderInfo& operator=(const uicPartyLeaderInfo& rhs);
    uicPartyLeaderInfo& operator=(const PARTY_MEMBER_INFO& rhs);
    uicPartyLeaderInfo& operator=(const PARTY_MEMBER_INFO* rhs);

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
uicPartyLeaderInfo::uicPartyLeaderInfo(const uicPartyLeaderInfo& src)
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
uicPartyLeaderInfo&
uicPartyLeaderInfo::operator=(const uicPartyLeaderInfo& rhs)
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
uicPartyLeaderInfo&
uicPartyLeaderInfo::operator=(const PARTY_MEMBER_INFO& rhs)
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
uicPartyLeaderInfo&
uicPartyLeaderInfo::operator=(const PARTY_MEMBER_INFO* rhs)
{
    memcpy (static_cast<PARTY_MEMBER_INFO *>(&memInfo),
            static_cast<const PARTY_MEMBER_INFO *>(rhs),
            sizeof (PARTY_MEMBER_INFO));

    this->bFill = true;

    return *this;
}

//------------------------------------------------------------------------------
#endif  //  __UIPARTYLEADER_DEFINE_H__
