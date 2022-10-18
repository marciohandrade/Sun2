#pragma once

//////////////////////////////////////////////////////////////////////////
//  @history
//  - check-in  : lovesusun,
//  - 090514    : waverix,      rearrange

#include "BaseParty.h"

class BasePartyManager
{
public:
	typedef STLX_MAP<WORD, BaseParty*>	PARTY_MAP;

	BasePartyManager();
	virtual ~BasePartyManager();

    virtual void Release();

    virtual void Initialize(
        DWORD party_pool_size
        );

	virtual BaseParty* CreateParty(
        ePARTY_TYPE party_type, 
        WORD party_key, 
        DWORD master_key
        );
	
    virtual RC::ePARTY_RESULT DestroyParty(
        WORD party_key
        );
	
    virtual RC::ePARTY_RESULT JoinParty(
        WORD party_key, 
        const _PARTY_MEMBER_INFO& MemberInfo
        );
	
    virtual RC::ePARTY_RESULT LeaveParty(
        WORD party_key, 
        DWORD member_key
        );

	virtual RC::ePARTY_RESULT ChangeMaster(
        WORD party_key, 
        DWORD next_master_key
        );

    const BaseParty* const FindParty(const WORD party_key) const
    {
        if (party_key == 0)
        {
            return NULL;
        }
        PARTY_MAP::const_iterator it = party_map_.find(party_key);
        if (it == party_map_.end()) 
        {
            return NULL;
        }
        return it->second;
    }

	const _PARTY_MEMBER_INFO* GetMemberInfo(
        WORD party_key, 
        DWORD member_key
        );

	const DWORD GetMasterKey(
        WORD party_key
        );

	template< class Operator >	
    BOOL For_eachMembers(const WORD party_key, Operator& op) const
    {
        BaseParty* const base_party = FindEditableParty(party_key);
        if (base_party == NULL)
        {
            return FALSE;
        }
        base_party->For_eachMembers(op);
        return TRUE;
    }

protected:
	BaseParty* FindEditableParty(const WORD party_key) const
    {
        if (party_key == 0)
        {
            return NULL;
        }
        PARTY_MAP::const_iterator it = party_map_.find(party_key);
        if (it == party_map_.end()) 
        {
            return NULL;
        }
        return it->second;
    }

protected:
	util::PoolRouter<ePARTY_MAX> party_pool_router_;
	PARTY_MAP party_map_;
};