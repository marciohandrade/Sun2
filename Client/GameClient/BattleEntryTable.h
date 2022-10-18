#ifndef BATTLE_ENTRY_TABLE_H
#define BATTLE_ENTRY_TABLE_H

class BattleRoomEntry;

typedef stdext::hash_map<DWORD, BattleRoomEntry *>	BATTLE_TABLE_HASH;
typedef BATTLE_TABLE_HASH::iterator				BATTLE_TABLE_ITR;
typedef pair<DWORD, BattleRoomEntry *>		BATTLE_TABLE_PAIR;

class BattleEntryTable
{
public:
	BattleEntryTable(void);
	virtual ~BattleEntryTable(void);

public:
	void				Init();
	void				Release();

	BattleRoomEntry *	AllocEntry();

public:
	BOOL Insert( DWORD dwKey, BattleRoomEntry * pBattleEntry );

	BOOL Delete(DWORD dwKey);

	void DeleteAll(void);

	BattleRoomEntry *	findByName( const TCHAR * pctszName );
	
	BATTLE_TABLE_HASH::iterator	GetBegin();
	BATTLE_TABLE_HASH::iterator	GetEnd();
	
	BattleRoomEntry *	find(DWORD dwKey);            

	BattleRoomEntry *	findRoomKey(DWORD dwKey);

private:
	BATTLE_TABLE_HASH	m_BattleRoomTable;
};

#endif
// BATTLE_ENTRY_TABLE_H