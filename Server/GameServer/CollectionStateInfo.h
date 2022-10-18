#pragma once
//=======================================================================================================================
/// CollectionStateInfo class
/*
@author
Lee Su Tek
@since
2009. 2. 23
@remarks
@note
채집품의 현재 상황을 나타낸다.
하나의 타이머를 가지며, Active 시 collectable -> Collecting -> Respawning -> spawn -> collectable 단계로 변한다.

collectable : 채집 가능한 상태.
Collecting :  m_Collector가 채집중.
Respawning :  m_Collector가 채집을 정상 완료, 리스폰 대기상태.
spawn : collectable 로 전이

@history

*/
//=======================================================================================================================

#include <MemoryPoolFactory.h>
#include "StructInPacket.h"

struct CollectionInfo;


class CollectionStateInfo
{
public:
	enum { collectable = 0, Collecting, Collected, Respawning, spawn, cancel };

	__DECL_CUSTOMPOOL_PTR( CollectionStateInfo )

public:
	CollectionStateInfo();
	virtual ~CollectionStateInfo() {}

public:
	bool Init( Collection* pCollection, CHARGUID Collector, WzVector& CollectorPos );
	void Release();

	//.//
	DWORD GetObjectKey()	{ return m_pMapObjectInfo->dwMapObjectID; }
	WzID  GetCollectionID() { return m_pMapObjectInfo->CollectionID;  }

	CHARGUID  GetCollector()	{ return m_Collector;	 }
	WzVector& GetCollectorPos() { return m_CollectorPos; }

	DWORD GetRemainedTime() { return m_Timer.GetRemainedTime(); }

	//생성된 SCCollection는 무조건 m_pData가 NULL이 아니다.
	CollectionInfo*	GetCollectionInfo()	{ return m_pData; }
	Collection*		GetCollection()		{ return m_pCollection; }
	MAPOBJECT_INFO* GetMapObjectInfo()  { return m_pMapObjectInfo; }
	//.//

	bool StartCollect();
	bool StartRespawn();

	void Cancel();
	bool CanCancel(CHARGUID Collector);

	bool update(OUT BYTE* stat);

private:
	bool _ChangeStat();
	CollectionInfo* _FindCollectionInfo( WzID CollectionID );

private:
	CHARGUID	m_Collector;			// 채집자.
	WzVector	m_CollectorPos;			// 채집자 위치.

	BYTE		m_stat;					// 채집상태 (채집중, 재생중)	
	util::Timer	m_Timer;				// 채집시간, 채집물 재생시간

	CollectionInfo* m_pData;			// 채집정보	(스크립트 정보)	
	MAPOBJECT_INFO*	m_pMapObjectInfo;	// 채집정보	(맵툴 정보)		
	Collection*		m_pCollection;		// 채집물 오브젝트
};
