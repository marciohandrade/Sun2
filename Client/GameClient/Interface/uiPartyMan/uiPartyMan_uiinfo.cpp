//------------------------------------------------------------------------------
//  uiPartyMan_uiinfo.cpp
//  (C) 2005 ckbang
//------------------------------------------------------------------------------
#include "SunClientPrecompiledHeader.h"

#ifdef _CK_MOD_UI_PARTY

#include "uiPartyMan.h"

//------------------------------------------------------------------------------
/**
*/
void
uiPartyMan::uiClearParty()
{
    this->m_vuiParty.clear();
}

//------------------------------------------------------------------------------
/**
*/
bool
uiPartyMan::uiIsParty()
{
    return this->m_vuiParty.size();
}

//------------------------------------------------------------------------------
/**
*/
short
uiPartyMan::uiGetPartyNum()
{
    return this->m_vuiParty.size();
}

//------------------------------------------------------------------------------
/**
*/
const PARTY_MEMBER_INFO&
uiPartyMan::uiGetParty(short idx)
{
    assert (idx < m_vuiParty.size());
    return this->m_vuiParty[idx];
}

//------------------------------------------------------------------------------
/**
*/
const std::vector<PARTY_MEMBER_INFO>&
uiPartyMan::uiGetPartyList()
{
    return this->m_vuiParty;
}

#endif  // _CK_MOD_UI_PARTY

//------------------------------------------------------------------------------
//  EOF
//------------------------------------------------------------------------------
