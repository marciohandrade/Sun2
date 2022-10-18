#include "SunClientPrecompiledHeader.h"
#include "battleentrytable.h"
#include "BattleRoomEntry.h"
#include "InterfaceManager.h"

BattleEntryTable::BattleEntryTable(void)
{
	Init();
}

BattleEntryTable::~BattleEntryTable(void)
{
	Release();
}


void BattleEntryTable::Init()
{

}

BattleRoomEntry * BattleEntryTable::AllocEntry()
{
	BattleRoomEntry * pTemp = new BattleRoomEntry;
	assert(pTemp);
	return pTemp;
}

void BattleEntryTable::Release()
{
	DeleteAll();
}

BOOL BattleEntryTable::Insert( DWORD dwKey, BattleRoomEntry * pBattleEntry )
{
	BATTLE_TABLE_ITR itr = m_BattleRoomTable.find(dwKey);
	if (m_BattleRoomTable.end() != itr) 
	{
		return FALSE;
	}

	if ( findRoomKey(pBattleEntry->m_dwSerial) )
	{
		return FALSE;
	}

	BattleRoomEntry * pNewBattleRoomEntry = AllocEntry();

	*pNewBattleRoomEntry = *pBattleEntry;
    	
	m_BattleRoomTable.insert(BATTLE_TABLE_PAIR(dwKey, pNewBattleRoomEntry ));

	return TRUE;
}


BOOL BattleEntryTable::Delete(DWORD dwKey)
{
	BATTLE_TABLE_ITR itr = m_BattleRoomTable.find(dwKey);

	BattleRoomEntry * pBattleRoomEntry = static_cast<BattleRoomEntry * >(itr->second);
	if (!pBattleRoomEntry)
	{
		return FALSE;
	}
	else
	{
		delete pBattleRoomEntry;
		m_BattleRoomTable.erase(itr);
	}	

	return TRUE;
}

void BattleEntryTable::DeleteAll(void)
{
	BattleRoomEntry * pBattleRoomEntry = NULL;
	BATTLE_TABLE_ITR itr = m_BattleRoomTable.begin();

	while (itr != m_BattleRoomTable.end())
	{
		pBattleRoomEntry = static_cast<BattleRoomEntry *>(itr->second);
		assert(pBattleRoomEntry);	// 목록이 있는데 왜 포인터가 널이야 말도안되

		delete pBattleRoomEntry;
		m_BattleRoomTable.erase(itr++);
	}
}


BATTLE_TABLE_HASH::iterator BattleEntryTable::GetBegin()
{
	return m_BattleRoomTable.begin();
}


BATTLE_TABLE_HASH::iterator BattleEntryTable::GetEnd()
{
	return m_BattleRoomTable.end();
}

BattleRoomEntry * BattleEntryTable::find(DWORD dwKey)
{
	BATTLE_TABLE_ITR itr = m_BattleRoomTable.find(dwKey);

	if (m_BattleRoomTable.end() != itr)
		return 	static_cast<BattleRoomEntry *>(itr->second);
	else 
		return NULL;
}

BattleRoomEntry * BattleEntryTable::findRoomKey(DWORD dwKey)
{
	BATTLE_TABLE_ITR itr = m_BattleRoomTable.begin();

	for ( itr; itr != m_BattleRoomTable.end(); ++itr )
	{	
		BattleRoomEntry * pBattleRoom = 
			static_cast<BattleRoomEntry *>(itr->second);
		if (pBattleRoom->m_dwSerial == dwKey)
		{
			return pBattleRoom;
		}
	}

	return NULL;
}
