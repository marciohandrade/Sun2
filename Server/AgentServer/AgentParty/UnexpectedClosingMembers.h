#pragma once

#include "IUnexpectedClosingMembers.h"

class AgentParty;

class UnexpectedClosingMembers
{
public:
    typedef STLX_LIST<sUClosingUser> UCLOSING_USER_LIST;

    UnexpectedClosingMembers() {}
    ~UnexpectedClosingMembers() {}

    void Update(AgentParty* const pAgentParty);
    BOOL UpdateChar(sUClosingUser& rCUser);

    inline void Clear()     { m_List.clear(); }
    inline BOOL IsEmpty()   { return m_List.empty(); }

    sUClosingUser* FindChar(const DWORD char_guid);
    void AddChar(const DWORD char_guid, const TCHAR* pCharName, PartyReturnInfo& rInfo);
    void RemoveChar(const DWORD char_guid);

    BOOL ChangeStatus(const DWORD char_guid, const IUCM_T::eUSR_STATUS IN rStatus);
    BOOL SetFirstStatus(const DWORD char_guid);

private:
    UCLOSING_USER_LIST::iterator _FindChar(const DWORD char_guid);

    UCLOSING_USER_LIST m_List;
};

