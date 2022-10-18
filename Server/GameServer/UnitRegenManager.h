//=============================================================================================================================
/// UnitRegenManager class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 31
	@note
		- Field�� �ʱ�ȭ�ɶ�, Init()�ȴ�. ��, ������ ���͵� ������ m_WaitingHash�� �߰��ȴ�.
		- Update() ���� ���� ���� Ÿ�Ӿƿ� üũ ��, ������Ų��. GameField->EnterField();
		- Field�� NPC�� Update()�� False�� UnitRegenManager�� AddOnWaiting()��
		
	@todo
		- UnitRegen����->Regen�������� ������ ����
		- ObjectManager�� ���� ó���� ������ ���� : �ʹ� ���� ����;;
	@history

*/
//=============================================================================================================================

#pragma once

#include ".\UnitRegenInfoManager.h"
#include "UnitRegen.h"
#include <MemoryPoolFactory.h>
#include <hash_map>

class GameField;
class Monster;
class RegenLocation;

typedef STLX_SET<int>				SET_SECTOR_REGEN;
typedef SET_SECTOR_REGEN::iterator	SET_SECTOR_REGEN_ITR;

class UnitRegenManager
{
	typedef util::CMemoryPoolFactory<UnitRegen>		_POOL;
	typedef STLX_HASH_MAP<REGENID, UnitRegen *>		UNITREGEN_HASH;
	typedef std::pair<REGENID, UnitRegen *>			UNITREGEN_PAIR;
public:
	UnitRegenManager(void);
	~UnitRegenManager(void){}

	static void CreatePool( DWORD dwPoolSize );

	void Init( GameField * pField, UnitRegenInfoManager * pInfo, bool bPreRegen );
	void Release();

	__inline UnitRegen * Find( REGENID id )
	{
		UNITREGEN_HASH::iterator it = m_WaitingUnitRegenHash.find(id);
		if( it != m_WaitingUnitRegenHash.end() )
			return it->second;
		return NULL;
	}

	void Update();

	int GetCount() const 
    { 
        return (int)m_WaitingUnitRegenHash.size(); 
    }

	// Ư�� �����ð��� �ѱ� �� ����
	void AddOnWaiting( REGENID id, DWORD SomeRegenTime = 0 );
	void SpawnSector( DWORD dwSectorIndex );

private:
	bool Spawn( UnitRegenInfo * pUnitInfo );    //_NA_0_20100706_RANDOM_REGEN ����
	void Insert( REGENID id, UnitRegen * p ) { m_WaitingUnitRegenHash.insert( UNITREGEN_HASH::value_type(id, p) ); }
	void SpawnAll( UNITREGENINFO_PAIR value_type );
	void free( UNITREGEN_PAIR value_type );

private:
	__PTR_PROPERTY( GameField, GameField );
	__PTR_PROPERTY( UnitRegenInfoManager, UnitRegenInfoManager );
	UNITREGEN_HASH m_WaitingUnitRegenHash;
	static _POOL m_Pool;

	bool				m_bPreRegen;
	SET_SECTOR_REGEN	m_setRegenSector;	// �� ������ ������ �Ϸ��ߴ��� ����
};













