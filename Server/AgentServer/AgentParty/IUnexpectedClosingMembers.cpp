#include "stdafx.h"
#include "UnexpectedClosingMembers.h"
#include "IUnexpectedClosingMembers.h"

IUnexpectedClosingMembers::~IUnexpectedClosingMembers()
{
    Clear();
}

void
IUnexpectedClosingMembers::Clear()
{
    if(m_pMembers)
    {
        TAllocDelete(UnexpectedClosingMembers, m_pMembers);
        m_pMembers = NULL;
    }
}

void
IUnexpectedClosingMembers::Init(WORD party_key)
{
    Clear();
    m_wPartyKey = party_key;
}

void
IUnexpectedClosingMembers::Update(AgentParty* const pAgentParty)
{
    if(m_pMembers)
        m_pMembers->Update(pAgentParty);
}

BOOL
IUnexpectedClosingMembers::UpdateChar(sUClosingUser& rCUser)
{
    if(m_pMembers)
        return m_pMembers->UpdateChar(rCUser);

    return FALSE;
}

void
IUnexpectedClosingMembers::AddUClosingUser(const DWORD char_guid, const TCHAR* pCharName,
                                           PartyReturnInfo& rInfo)
{
    if(!m_pMembers)
    {
        m_pMembers = TAllocNew(UnexpectedClosingMembers);
    }

    m_pMembers->AddChar(char_guid, pCharName, rInfo);
}

void
IUnexpectedClosingMembers::RemoveUClosingUser(const DWORD char_guid)
{
    if(m_pMembers)
    {
        m_pMembers->RemoveChar(char_guid);
        if(m_pMembers->IsEmpty())
        {
            TAllocDelete(UnexpectedClosingMembers, m_pMembers);
            m_pMembers = NULL;
        }
    }
}

sUClosingUser*
IUnexpectedClosingMembers::FindUClosingUser(const DWORD char_guid)
{
    if(m_pMembers)
        return m_pMembers->FindChar(char_guid);

    return NULL;
}

BOOL
IUnexpectedClosingMembers::ChangeStatus(const DWORD char_guid, const eUSR_STATUS IN rStatus)
{
    if(m_pMembers)
        return m_pMembers->ChangeStatus(char_guid, rStatus);

    return FALSE;
}

BOOL
IUnexpectedClosingMembers::SetFirstStatus(const DWORD char_guid)
{
    if(m_pMembers)
        return m_pMembers->SetFirstStatus(char_guid);

    return FALSE;
}

