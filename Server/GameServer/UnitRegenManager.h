//=============================================================================================================================
/// UnitRegenManager class
/**
	@author	Kim Min Wook < taiyo@webzen.com >
	@since	2006. 7. 31
	@note
		- Field가 초기화될때, Init()된다. 즉, 리젠될 몬스터들 정보가 m_WaitingHash에 추가된다.
		- Update() 마다 지젠 여부 타임아웃 체크 후, 리젠시킨다. GameField->EnterField();
		- Field의 NPC의 Update()가 False면 UnitRegenManager에 AddOnWaiting()함
		
	@todo
		- UnitRegen관련->Regen관련으로 변경할 예정
		- ObjectManager의 몬스터 처리를 제거할 예정 : 너무 꼬여 있음;;
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

	// 특정 리젠시간을 넘길 수 있음
	void AddOnWaiting( REGENID id, DWORD SomeRegenTime = 0 );
	void SpawnSector( DWORD dwSectorIndex );

private:
	bool Spawn( UnitRegenInfo * pUnitInfo );    //_NA_0_20100706_RANDOM_REGEN 삭제
	void Insert( REGENID id, UnitRegen * p ) { m_WaitingUnitRegenHash.insert( UNITREGEN_HASH::value_type(id, p) ); }
	void SpawnAll( UNITREGENINFO_PAIR value_type );
	void free( UNITREGEN_PAIR value_type );

private:
	__PTR_PROPERTY( GameField, GameField );
	__PTR_PROPERTY( UnitRegenInfoManager, UnitRegenInfoManager );
	UNITREGEN_HASH m_WaitingUnitRegenHash;
	static _POOL m_Pool;

	bool				m_bPreRegen;
	SET_SECTOR_REGEN	m_setRegenSector;	// 그 섹터의 리젠을 완료했는지 여부
};













