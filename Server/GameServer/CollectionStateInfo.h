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
ä��ǰ�� ���� ��Ȳ�� ��Ÿ����.
�ϳ��� Ÿ�̸Ӹ� ������, Active �� collectable -> Collecting -> Respawning -> spawn -> collectable �ܰ�� ���Ѵ�.

collectable : ä�� ������ ����.
Collecting :  m_Collector�� ä����.
Respawning :  m_Collector�� ä���� ���� �Ϸ�, ������ ������.
spawn : collectable �� ����

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

	//������ SCCollection�� ������ m_pData�� NULL�� �ƴϴ�.
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
	CHARGUID	m_Collector;			// ä����.
	WzVector	m_CollectorPos;			// ä���� ��ġ.

	BYTE		m_stat;					// ä������ (ä����, �����)	
	util::Timer	m_Timer;				// ä���ð�, ä���� ����ð�

	CollectionInfo* m_pData;			// ä������	(��ũ��Ʈ ����)	
	MAPOBJECT_INFO*	m_pMapObjectInfo;	// ä������	(���� ����)		
	Collection*		m_pCollection;		// ä���� ������Ʈ
};
