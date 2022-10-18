#include "StdAfx.h"
#include "BasePartyManager.h"
#include "BaseParty.h"


BasePartyManager::BasePartyManager()
{
}

BasePartyManager::~BasePartyManager()
{
}

void
BasePartyManager::Initialize(DWORD party_pool_size)
{
	party_pool_router_.Register<BaseParty>(ePARTY_FIELD);
	party_pool_router_.Initialize(ePARTY_FIELD, party_pool_size, party_pool_size/4);
}

void
BasePartyManager::Release()
{
    PARTY_MAP::iterator it = party_map_.begin(),
                        end = party_map_.end();
	for (; it != end; ++it)
	{
		BaseParty* const base_party = it->second;
        if (base_party == NULL)
        {
            continue;
        }
        const ePARTY_TYPE party_type = base_party->GetType();
		base_party->Release();
        party_pool_router_.Free(party_type, base_party);
	}

	party_map_.clear();
}

BaseParty*
BasePartyManager::CreateParty(ePARTY_TYPE party_type, WORD party_key, DWORD master_key)
{
	if (FindParty(party_key))
    {
        return NULL;
    }

	BaseParty* base_party = static_cast<BaseParty*>(party_pool_router_.Alloc(party_type));

	base_party->Init(party_key, master_key, party_type);
	
    party_map_.insert(PARTY_MAP::value_type(party_key, base_party));

	return base_party;
}

RC::ePARTY_RESULT
BasePartyManager::DestroyParty(WORD party_key)
{
	BaseParty* base_party = FindEditableParty(party_key);
	if (base_party == NULL)
    {
		return RC::RC_PARTY_PARTY_NOTEXIST;
    }
	
    party_map_.erase(party_key);

    const ePARTY_TYPE party_type = base_party->GetType();
    base_party->Release();
	party_pool_router_.Free(party_type, base_party);
	
	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
BasePartyManager::JoinParty(WORD party_key, const _PARTY_MEMBER_INFO& member_info)
{
	BaseParty* base_party = FindEditableParty(party_key);
	if(base_party == NULL)
    {
		return RC::RC_PARTY_PARTY_NOTEXIST;
    }

	return base_party->Join(member_info);
}

RC::ePARTY_RESULT
BasePartyManager::LeaveParty(WORD party_key, DWORD member_key)
{
	BaseParty* base_party = FindEditableParty(party_key);
	if (base_party == NULL)
    {
		return RC::RC_PARTY_PARTY_NOTEXIST;
    }

	return base_party->Leave(member_key);
}

RC::ePARTY_RESULT
BasePartyManager::ChangeMaster(WORD party_key, DWORD next_master_key)
{
	BaseParty* base_party = FindEditableParty(party_key);

	if (base_party == NULL)
    {
		return RC::RC_PARTY_PARTY_NOTEXIST;
    }

	return base_party->ChangeMaster(next_master_key);

}

const _PARTY_MEMBER_INFO*
BasePartyManager::GetMemberInfo(WORD party_key, DWORD member_key)
{
	BaseParty* base_party = FindEditableParty(party_key);
	if (base_party == NULL)
    {
		return NULL;
    }

	return base_party->GetMemberInfo(member_key);
}

const DWORD
BasePartyManager::GetMasterKey(WORD party_key)
{
	BaseParty* base_party = FindEditableParty(party_key);
	if (base_party == 0)
    {
		return 0;
    }

	return base_party->GetMasterKey();
}