//------------------------------------------------------------------------------
//	
//	(C) 2005 y2jinc
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "userslotset.h"
#include "userslot.h"


//----------------------------------------------------------------------------
/**
*/
UserSlotSet::UserSlotSet(void) : m_pUserSlotArr(NULL)
{
}

//----------------------------------------------------------------------------
/**
*/
UserSlotSet::~UserSlotSet(void)
{
	Release();
}


//----------------------------------------------------------------------------
/**
*/
void 
UserSlotSet::Initialize(DWORD dwInitSlotNum)
{
    if (m_pUserSlotArr == NULL)
    {
	    m_pUserSlotArr = new UserSlot[dwInitSlotNum];
	    assert(m_pUserSlotArr);
    }

    m_dwMaxUserNum = dwInitSlotNum;

    FlushUsers();
}

//----------------------------------------------------------------------------
/** 유저정보만 모두 비워주는 함수
*/
void UserSlotSet::FlushUsers()
{
	assert(m_pUserSlotArr);

	for (int i = 0; i < (int)GetMaxUser(); ++i)
	{
		m_pUserSlotArr[i].Clear();
	}

	m_dwCurUserNum = 0;
}


//----------------------------------------------------------------------------
/**
*/
int		
UserSlotSet::findUserIndex(DWORD dwPlayerKey)
{
	assert(m_pUserSlotArr);
	
	for (int i = 0; i < (int)this->GetMaxUser(); ++i)
	{
		if (m_pUserSlotArr[i].GetPlayerKey() == dwPlayerKey)
		{
			return i;
		}
	}

	return INVALID_USER_SLOT_INDEX;
}


//----------------------------------------------------------------------------
/**
	@remark
		- [prev]assert(this->m_pUserSlotArr);
				assert(index < GetMaxUser());
				assert(m_pUserSlotArr[index].GetSlotState() == OPENED);

		- [post] state set BLOCK 
*/
void
UserSlotSet::SetBlockIndex(int index)
{
	assert(this->m_pUserSlotArr);
	assert((DWORD)index < GetMaxUser());

	m_pUserSlotArr[index].SetSlotState(UserSlot::BLOCK);
}


//----------------------------------------------------------------------------
/**
*/
void		
UserSlotSet::SetOpenIndex(int index)
{
	assert(this->m_pUserSlotArr);
	assert((DWORD)index < GetMaxUser());
	assert(m_pUserSlotArr[index].GetSlotState() == UserSlot::BLOCK || 
			m_pUserSlotArr[index].GetSlotState() == UserSlot::FILL );

	if (m_pUserSlotArr[index].GetSlotState() != UserSlot::OPENED &&
		m_pUserSlotArr[index].GetSlotState() != UserSlot::BLOCK )
	{
		--this->m_dwCurUserNum;
	}

	m_pUserSlotArr[index].Clear();
}


//----------------------------------------------------------------------------
/**
	@prev
		assert(index);
		assert(INVALID_USER_SLOT_INDEX != this->findUserIndex(index));
		assert(m_pUserSlotArr[index].GetSlotState() == UserSlot::FILL);

	@post
		user is set ready state
*/
void 
UserSlotSet::SetReadyIndex(int index)
{
#ifdef _DH_BATTLEZONE2_
    // 리뉴얼된 배틀존에서는 방장이 시작시 레디가 보내진다. 따라서 index = 0이 가능
#else
	assert(index);
#endif//_DH_BATTLEZONE2_
	assert(INVALID_USER_SLOT_INDEX != index);
	assert(	m_pUserSlotArr[index].GetSlotState() == UserSlot::FILL );

	if ( INVALID_USER_SLOT_INDEX != index &&  
		 this->m_pUserSlotArr[index].GetSlotState() == UserSlot::FILL)
	{
		this->m_pUserSlotArr[index].SetSlotState( UserSlot::READY );
	}
}

//----------------------------------------------------------------------------
/**
	@prev
		assert(index);
		assert(	m_pUserSlotArr[index].GetSlotState() == UserSlot::READY );

	@post
		user is set ready cancle state
*/
void		
UserSlotSet::SetReadyCancleIndex(int index)
{
#ifdef _DH_BATTLEZONE2_
#else
	assert(index);
#endif//_DH_BATTLEZONE2_
	assert(INVALID_USER_SLOT_INDEX != index);
	assert(	m_pUserSlotArr[index].GetSlotState() == UserSlot::READY );

	if ( INVALID_USER_SLOT_INDEX != index && 
		 this->m_pUserSlotArr[index].GetSlotState() == UserSlot::READY ) 
	{
		this->m_pUserSlotArr[index].SetSlotState(UserSlot::FILL);
	}
}


//----------------------------------------------------------------------------
/**
*/
void		
UserSlotSet::SetTeamIndex(int index, int iTeam)
{
	if (this->m_pUserSlotArr[index].GetSlotState() == UserSlot::FILL || 
		this->m_pUserSlotArr[index].GetSlotState() == UserSlot::READY )
	{
		this->m_pUserSlotArr[index].SetTeam(iTeam);
	}
	else
	{
		assert(0);
#if defined ( _DEBUG )
		_asm int 3;
#endif
	}	
}


//----------------------------------------------------------------------------
/**
*/
BOOL 
UserSlotSet::LeaveUser(DWORD dwPlayerKey)
{
	UserSlot * pUserSlot = getUserSlot(dwPlayerKey);	

	if (pUserSlot)
	{
		pUserSlot->Clear();
		--m_dwCurUserNum;
	}
	else
	{
		return FALSE;
	}

	return TRUE;
}


//----------------------------------------------------------------------------
/**
*/
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
BOOL 
UserSlotSet::LeaveUser(char* character_name)
{
    for (int i = 0; i < (int)this->GetMaxUser(); ++i)
    {
        if (StrnCmp(m_pUserSlotArr[i].GetPlayerName(), character_name, MAX_CHARNAME_LENGTH) == 0)
        {
            m_pUserSlotArr[i].Clear();
            --m_dwCurUserNum;

            return TRUE;
        }
    }

    return FALSE;
}
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS

//----------------------------------------------------------------------------
/**
*/
void 
UserSlotSet::ReleaseUserAll()
{
	SAFE_DELETE_ARRAY(this->m_pUserSlotArr);

	this->m_dwCurUserNum = 0;
	this->m_dwMaxUserNum = 0;
}

//----------------------------------------------------------------------------
/**
*/
void 
UserSlotSet::Release()
{
	ReleaseUserAll();
}


//----------------------------------------------------------------------------
/**
*/
DWORD		
UserSlotSet::GetMaxUser() const
{
	return this->m_dwMaxUserNum;
}


//----------------------------------------------------------------------------
/**
*/
DWORD	
UserSlotSet::GetNumUser() const
{
	return this->m_dwCurUserNum;
}

//----------------------------------------------------------------------------
/**
*/
DWORD		
UserSlotSet::GetNonBlockNum() const
{
	DWORD dwNum = 0;
	for(int i = 0; i < (int)GetMaxUser(); ++i)
	{
		if (UserSlot::BLOCK != this->m_pUserSlotArr[i].GetSlotState() )
		{
			dwNum++;
		}
	}

	return dwNum;
}

//----------------------------------------------------------------------------
/**
*/
DWORD		
UserSlotSet::GetBlockNum() const
{
	DWORD dwNum = 0;
	for(int i = 0; i < (int)GetMaxUser(); ++i)
	{
		if (UserSlot::BLOCK == this->m_pUserSlotArr[i].GetSlotState() )
		{
			dwNum++;
		}
	}

	return dwNum;
}


//----------------------------------------------------------------------------
/**
*/
UserSlot *	
UserSlotSet::GetUserByIndex(int index)
{
    assert(this->m_pUserSlotArr);
    assert((DWORD)index < this->GetMaxUser());

    if ((index >= 0) && ((DWORD)index < GetMaxUser()))
    {
        return &this->m_pUserSlotArr[index];
    }
    return NULL;
}


UserSlot *	UserSlotSet::GetUserByKey(DWORD dwPlayerKey )
{
	int index = this->findUserIndex(dwPlayerKey );
	if (INVALID_USER_SLOT_INDEX == index )
	{
		return NULL;
	}

	return this->GetUserByIndex(index);
}


//----------------------------------------------------------------------------
/**
*/
int	
UserSlotSet::GetUserIndexFromKey(DWORD dwPlayerKey )
{
	return this->findUserIndex(dwPlayerKey );
}


//----------------------------------------------------------------------------
/**
*/
BOOL	
UserSlotSet::IsExistEmptySlot() const
{
	for(int i = 0; i < (int)GetMaxUser(); ++i)
	{
		if (UserSlot::OPENED == this->m_pUserSlotArr[i].GetSlotState() )
		{
			return TRUE;
		}
	}

	return FALSE;
}


//----------------------------------------------------------------------------
/**
*/
UserSlotSet::eRT_USERSLOT_SET
UserSlotSet::AddUser(UserSlot & EnterUserSlot)
{
	if (INVALID_USER_SLOT_INDEX != this->findUserIndex(EnterUserSlot.GetPlayerKey()) )
	{
		return EXIST_ALEADY_USER_SLOT;
	}

	int index = getEmptySlot();
	if (INVALID_USER_SLOT_INDEX == index)
	{
		return FULL_USER_SLOT;
	}

	this->m_pUserSlotArr[index].SetPlayerKey( EnterUserSlot.GetPlayerKey() );
    this->m_pUserSlotArr[index].SetBattleZonePlayerKey(EnterUserSlot.GetBattleZonePlayerKey());
	this->m_pUserSlotArr[index].SetSlotState( UserSlot::FILL );
#ifdef _DH_BATTLEZONE2_
    this->m_pUserSlotArr[index].SetTeam(EnterUserSlot.GetTeam());
#endif//_DH_BATTLEZONE2_
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    this->m_pUserSlotArr[index].SetPlayerName(EnterUserSlot.GetPlayerName());
#endif //_NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
        
	++this->m_dwCurUserNum;

	return SUCCESS;
}


//----------------------------------------------------------------------------
/**
*/
int		
UserSlotSet::getEmptySlot()
{
	for(int i = 0; i < (int)GetMaxUser(); ++i)
	{
		if (UserSlot::OPENED == this->m_pUserSlotArr[i].GetSlotState() )
		{
			return i;
		}
	}

	return INVALID_USER_SLOT_INDEX;

}

//----------------------------------------------------------------------------
/**
*/
UserSlot * 
UserSlotSet::getUserSlot(DWORD dwPlayerKey)
{
	int index = findUserIndex(dwPlayerKey);
	if ( INVALID_USER_SLOT_INDEX == index )
	{
		return NULL;
	}

	return &m_pUserSlotArr[index];
}