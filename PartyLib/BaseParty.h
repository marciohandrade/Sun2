#pragma once

//////////////////////////////////////////////////////////////////////////
//  @history
//  - check-in  : lovesusun,
//  - 090514    : waverix,      rearrange

class BaseParty
{
public:
    typedef STLX_MAP<DWORD, _PARTY_MEMBER_INFO> MEMBER_MAP;

    BaseParty();
    virtual ~BaseParty();

    virtual VOID Init(WORD party_key, DWORD master_key, ePARTY_TYPE party_type);
    virtual VOID Release();

    virtual RC::ePARTY_RESULT Join(const _PARTY_MEMBER_INFO& member_info);
    virtual RC::ePARTY_RESULT Leave(DWORD member_key);
    virtual RC::ePARTY_RESULT ChangeMaster(DWORD next_master_key);

    virtual ePARTY_TYPE GetType()const  { return party_type_; }
    inline WORD GetPartyKey()const   { return party_key_; }

    inline DWORD GetMasterKey()const  { return master_key_; }

    inline VOID SetTargetKey(DWORD target_key)   { target_key_ = target_key; }
    inline DWORD GetTargetKey()  { return target_key_; }

    inline VOID SetDistributionType(eITEM_DISTRIBUTION_TYPE Type)   { distribution_type_ = Type; }
    inline BYTE GetDistributionType()   { return (BYTE)distribution_type_; }

    inline BYTE GetMemberNum() const { return static_cast<BYTE>(party_member_map_.size()); }
    const _PARTY_MEMBER_INFO* GetHeadUser();
    const _PARTY_MEMBER_INFO* GetMemberInfo(DWORD member_key)const;
    inline const MEMBER_MAP& GetMemberList() const { return party_member_map_; }
    VOID GetMemberInfoAll(PARTY_MEMBER_TOTAL_INFO & member_info_all);
    DWORD GetMemberInfoAll(_GUIDANCE_MEMBER_INFO* member_info_all);
    BOOL SetMemberInfo(DWORD member_key, _PARTY_MEMBER_INFO& member_info);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    VOID SetMemeberReadyState(DWORD member_key, BOOL ready_state);
    BOOL GetMemeberReadyState(DWORD member_key)const;

    VOID SetMemeberTeam(DWORD member_key, BYTE selected_team);
    BYTE GetMemeberTeam(DWORD member_key)const;

    BOOL IsReadyToAllMember();
    VOID ResetReadyStateAllMember();

    VOID SetMemberMapCode(DWORD member_key, MAPCODE map_code);

    template<class Operator>
    VOID For_eachMembers(Operator & Opr);

protected:
	MEMBER_MAP party_member_map_;

private:
	WORD party_key_;
	DWORD master_key_;
	DWORD target_key_;
	eITEM_DISTRIBUTION_TYPE distribution_type_;
    ePARTY_TYPE party_type_;
};

template< class Operator >
VOID BaseParty::For_eachMembers(Operator& Opr)
{
    MEMBER_MAP::iterator it = party_member_map_.begin(),
                         end = party_member_map_.end();
    while (it != end)
    {
        MEMBER_MAP::iterator temp_it = it;
        ++it;
        Opr(&temp_it->second);
    }
}



