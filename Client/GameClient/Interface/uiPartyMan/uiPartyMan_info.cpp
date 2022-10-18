//------------------------------------------------------------------------------
//  uiPartyMan_info.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#include "uiPartyMan.h"


#pragma warning ( push )
#pragma warning(disable : 4800)

//------------------------------------------------------------------------------
/**
*/
bool
uiPartyMan::IsPartyMember(DWORD dwObjKey)
{
    if (!this->IsParty())
        return false;

    for (size_t i=0; i<m_vParty.size(); ++i)
    {
        if (m_vParty[i].CompKeySimple(dwObjKey))
            return true;
    }

    return false;
}
//------------------------------------------------------------------------------
/**
*/
DWORD
uiPartyMan::IsPartyMemberForName(const TCHAR* character_name)
{
    if (!this->IsParty())
    {
        return 0;
    }

    for (size_t i=0; i<m_vParty.size(); ++i)
    {
        DWORD member_key = m_vParty[i].CompareName(character_name);
        if (member_key != 0)
        {
            return member_key;
        }
    }
    return 0;
}
//------------------------------------------------------------------------------
/**
	오브젝트 키값으로 파티원정보얻기 없다면 NULL
*/
uicPartyMemberInfoPlus*
uiPartyMan::GetPartyMemberInfo(DWORD dwObjKey)
{
	if (!this->IsParty()) 
		return NULL;

	for (size_t i=0; i<m_vParty.size(); ++i)
	{
		if (m_vParty[i].CompKeySimple(dwObjKey)) 
			return &(m_vParty[i]);
	}

	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
uicPartyMemberInfoPlus*
uiPartyMan::GetPartyMemberInfoFromIndex(DWORD index)
{
	if (!this->IsParty())
	{
		return NULL;
	}
	
	if(index < m_vParty.size())
	{
		return &(m_vParty[index]);
	}
	return NULL;
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::ClearParty()
{
    this->m_vParty.clear();
    this->uiClearAll(); // 파티 정보 UI 클리어

    this->m_bImTheLeader = false;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    is_disconnected_uesr_kick_ = false;
    ZeroMemory(kick_player_name_,sizeof(kick_player_name_));
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
}

//------------------------------------------------------------------------------
/**
*/
bool
uiPartyMan::IsParty()
{
    return (bool)this->m_vParty.size();
}

//------------------------------------------------------------------------------
/**
*/
short
uiPartyMan::GetPartyNum()
{
    return this->m_vParty.size();
}
//------------------------------------------------------------------------------ 
int uiPartyMan::GetPartyIndexByObjectKey(DWORD object_key)
{
    for ( size_t i = 0; i< m_vParty.size(); ++i)
    {
        uicPartyMemberInfoPlus& member_info = m_vParty[i];
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        if (member_info.IsConnected() == true)
#else
        if (member_info.IsUnexpectedTerminate() == false)
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        {
            if (member_info.Get().m_dwMemberKey == object_key)
            {
                return i;
            }
        }
    }
    return -1;
}
//------------------------------------------------------------------------------ 
int uiPartyMan::GetPartyIndex(TCHAR* character_name)
{
    for ( size_t i = 0; i< m_vParty.size(); ++i)
    {
        uicPartyMemberInfoPlus& member_info = m_vParty[i];
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        if (member_info.IsConnected() == true)
#else
        if (member_info.IsUnexpectedTerminate() == false)
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        {
            if (StrnCmp(member_info.Get().m_szCharName, character_name, MAX_CHARNAME_LENGTH) == 0)
            {
                return i;
            }
        }
    }
    return -1;
}

//------------------------------------------------------------------------------
/**
*/
const uicPartyMemberInfoPlus&
uiPartyMan::GetParty(short idx)
{
    if ((idx < 0) || (idx >= static_cast<int>(m_vParty.size())))
    {
        assert (static_cast<size_t>(idx) < m_vParty.size());
        return m_vParty[0];
    }

    return this->m_vParty[idx];
}

//------------------------------------------------------------------------------
/**
*/
const uicPartyMemberInfoPlus*
uiPartyMan::GetPartyLeader()
{
    if (!this->IsParty())
    {
        return NULL;
    }
	
    for (int i=0; i<this->GetPartyNum(); ++i)
    {
		if (m_vParty[i].Get().m_dwMemberKey == GetMasterKey())
        {
            return &this->m_vParty[i];
        }
    }


    return NULL;
}
//------------------------------------------------------------------------------
/**
*/
STLX_VECTOR<uicPartyMemberInfoPlus>
uiPartyMan::GetPartyList()
{
    return this->m_vParty;
}

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::SetPartyList(const STLX_VECTOR<uicPartyMemberInfoPlus>& partyList)
{
	this->m_vParty = partyList;
}

#pragma warning ( pop )

//------------------------------------------------------------------------------
/**
*/
#ifdef _DH_BATTLEZONE2_
int uiPartyMan::GetPartyIndexFromObjectkey(DWORD object_key)
{
    //오브젝트키로 파티원 인덱스 구해오기
    if (this->IsParty() == false)
    {
        return false;
    }

    for (size_t i = 0; i< m_vParty.size(); ++i)
    {
        if (m_vParty[i].CompKeySimple(object_key))
            return i;
    }
    return -1;
}
#endif //_DH_BATTLEZONE2_

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
