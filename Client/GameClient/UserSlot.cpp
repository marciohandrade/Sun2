//------------------------------------------------------------------------------
//	UserSlot
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "userslot.h"
#include "ObjectManager.h"


//----------------------------------------------------------------------------
/**
*/
UserSlot::UserSlot(void) : m_eSlotState(OPENED),
						   m_dwPlayerKey(0),
                           m_dwBattleZonePlayerKey(0)
{
	Clear();
}

//----------------------------------------------------------------------------
/**
*/
UserSlot::~UserSlot(void)
{
	Clear();
}


//----------------------------------------------------------------------------
/**
*/
void		
UserSlot::Clear()
{
    if (m_dwBattleZonePlayerKey > 0)
    {
        g_ObjectManager.Delete(m_dwBattleZonePlayerKey);
        m_dwBattleZonePlayerKey = 0;
    }


	this->m_eSlotState	= OPENED;
	this->m_dwPlayerKey = 0;
	this->m_iTeam		= 0;

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    ZeroMemory(m_szCharName, sizeof(m_szCharName));
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
}

//----------------------------------------------------------------------------
/**
*/
void		
UserSlot::SetSlotState(eSLOT_STATE eState)
{
	this->m_eSlotState = eState;
}


//----------------------------------------------------------------------------
/**
*/
void
UserSlot::SetPlayerKey(DWORD dwPlayerKey)
{
	this->m_dwPlayerKey = dwPlayerKey;
}

//----------------------------------------------------------------------------
/**
*/
void		
UserSlot::SetTeam(int iTeam)
{
	this->m_iTeam = iTeam;
}


//----------------------------------------------------------------------------
/**
*/
UserSlot::eSLOT_STATE 
UserSlot::GetSlotState() const
{
	return this->m_eSlotState;
}


DWORD 
UserSlot::GetPlayerKey() const
{
	return this->m_dwPlayerKey;
}

//----------------------------------------------------------------------------
/**
*/
int		
UserSlot::GetTeam() const
{
	return this->m_iTeam;
}