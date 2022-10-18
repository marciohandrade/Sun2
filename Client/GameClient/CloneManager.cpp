//------------------------------------------------------------------------------
//  CloneManager.cpp
//  (C) 2005 Webzen Sunstudio
//------------------------------------------------------------------------------

#include "SunClientPrecompiledHeader.h"
#include "clonemanager.h"
#include "Clone.h"

CloneManager g_CloneManager;

//------------------------------------------------------------------------------
/**
*/
CloneManager::CloneManager(void)
{
	m_dwLastKey = 0;
}

//------------------------------------------------------------------------------
/**
*/
CloneManager::~CloneManager(void)
{
	Clear();
}

//------------------------------------------------------------------------------
/**
*/
void CloneManager::Clear( void )
{
	Clone_Map::iterator	it;
	Clone_Map::iterator end = m_hashmapClone.end();

	for( it = m_hashmapClone.begin() ; it != end; ++it )
	{
		(it->second)->Destroy();
		delete (it->second);
	}

	m_hashmapClone.clear();
}

//------------------------------------------------------------------------------
/**
*/
DWORD CloneManager::CreateClone(Object *pObj)
{
	DWORD dwCloneKey = ++m_dwLastKey;

	
	Clone *pClone = new Clone();	
	if (pClone) 
	{
		pClone->Init();
        pClone->Create(0);
		pClone->CopyAppearance(pObj);

		m_hashmapClone.insert( Clone_Map_Pair(dwCloneKey, pClone) );
		return dwCloneKey;
	}
	else
	{
		return 0;
	}
}

//------------------------------------------------------------------------------
/**
*/
DWORD CloneManager::CreateAttachmentClone( Object *pObj)
{
	DWORD dwCloneKey = ++m_dwLastKey;


	Clone *pClone = new Clone();	
	if (pClone) 
	{
		pClone->Init();
        pClone->Create(0);
		pClone->CopyAttachment(pObj);

		m_hashmapClone.insert( Clone_Map_Pair(dwCloneKey, pClone) );
		return dwCloneKey;
	}
	else
	{
		return 0;
	}
}


Clone *CloneManager::CreateNotManagedClone(Object *pObj)
{
	DWORD dwCloneKey = ++m_dwLastKey;
	Clone *pClone = new Clone();	

	if (pClone) 
	{
		pClone->Init();
        pClone->Create(0);
		pClone->CopyAppearanceExceptAni(pObj);
		return pClone;
	}


	return NULL;

	
}

//------------------------------------------------------------------------------
/**
*/
BOOL CloneManager::Delete( DWORD dwCloneKey )
{
	Clone_Map::iterator	it;
	it = m_hashmapClone.find( dwCloneKey );
	if( it == m_hashmapClone.end() )
	{
		// ¾ø´ç!
		return FALSE;
	}

	(it->second)->Destroy();
	delete (it->second);
	m_hashmapClone.erase( dwCloneKey );

	return TRUE;
}

//------------------------------------------------------------------------------
/**
*/
Clone * CloneManager::GetClone(DWORD dwKey)
{	
	Clone_Map_Itr	itr;
	itr = m_hashmapClone.find( dwKey );
	if (itr == m_hashmapClone.end())
	{
		
		return NULL;
	}
	else
	{		
		return itr->second;
	}
}

//------------------------------------------------------------------------------
/**
*/
void CloneManager::Process(DWORD dwTick)
{
	Clone_Map::iterator	it,temp;
	for( it = m_hashmapClone.begin() ; it != m_hashmapClone.end() ;)
	{
		BOOL bKill = (it->second)->Process(dwTick);
		if (bKill)
		{			
			temp = it;
			DWORD dwKey = (temp->first);
			it++;
			(temp->second)->Destroy();
			delete (temp->second);
			m_hashmapClone.erase( dwKey );
		}
		else
		{
			it++;
		}
	}
}